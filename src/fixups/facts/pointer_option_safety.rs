use std::collections::BTreeSet;

use crate::fixups::facts::walk;
use crate::fixups::facts::{
    AstPath, FixupFacts, FunctionId, PathSegment, PointerComparisonFact, PointerComparisonKind,
    PointerOptionSafetyFact, Site,
};
use crate::rust_ast::{
    BinOp, CLibType, Expr, FnDef, IndentStmt, Item, Program, RustValue, Stmt, Type,
};

pub(in crate::fixups) fn collect_facts(program: &Program, facts: &mut FixupFacts) {
    facts.pointer_option_safety.clear();
    facts.pointer_comparisons.clear();
    let union_records = union_record_names(program);
    let mut safety = Vec::new();
    let mut comparisons = Vec::new();
    for (item_index, item) in program.items.iter().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        safety.extend(collect_safety_for_function(
            function,
            f,
            facts,
            &union_records,
        ));
        collect_comparisons_for_function(function, f, facts, &mut comparisons);
    }
    facts.pointer_option_safety = safety;
    facts.pointer_comparisons = comparisons;
}

pub(in crate::fixups) fn collect_for_function(
    function: FunctionId,
    f: &FnDef,
    facts: &FixupFacts,
    union_records: &BTreeSet<String>,
) -> (Vec<PointerOptionSafetyFact>, Vec<PointerComparisonFact>) {
    let safety = collect_safety_for_function(function, f, facts, union_records);
    let mut comparisons = Vec::new();
    collect_comparisons_for_function(function, f, facts, &mut comparisons);
    (safety, comparisons)
}

pub(in crate::fixups) fn union_record_names(program: &Program) -> BTreeSet<String> {
    program
        .items
        .iter()
        .filter_map(|item| match item {
            Item::Record(record) if record.is_union => Some(record.name.clone()),
            _ => None,
        })
        .collect()
}

fn collect_safety_for_function(
    function: FunctionId,
    f: &FnDef,
    facts: &FixupFacts,
    union_records: &BTreeSet<String>,
) -> Vec<PointerOptionSafetyFact> {
    let mut disqualified = BTreeSet::new();

    walk::body_exprs(&f.body, &mut |expr| {
        check_arithmetic_and_casts(expr, &mut disqualified);
    });
    walk_stmts_for_union_sourcing(&f.body, facts, function, union_records, &mut disqualified);

    facts
        .bindings
        .iter()
        .filter(|binding| binding.function == function)
        .filter(|binding| {
            facts
                .binding_type_ast(binding.id)
                .is_some_and(|ty| matches!(ty, Type::Ptr { .. }))
        })
        .map(|binding| PointerOptionSafetyFact {
            function,
            binding: binding.id,
            eligible: !disqualified.contains(&binding.name),
        })
        .collect()
}

fn check_arithmetic_and_casts(expr: &Expr, disqualified: &mut BTreeSet<String>) {
    match expr {
        Expr::MethodCall { recv, method, args }
            if (method == "add" || method == "offset") && args.len() == 1 =>
        {
            if let Expr::Var(name) = peel_casts(recv)
                && !is_zero_constant(&args[0])
            {
                disqualified.insert(name.as_str().to_string());
            }
        }
        Expr::Cast { expr: inner, ty } => {
            if is_int_or_void_ptr(ty)
                && let Expr::Var(name) = peel_casts(inner)
            {
                disqualified.insert(name.as_str().to_string());
            }
        }
        _ => {}
    }
}

fn walk_stmts_for_union_sourcing(
    body: &[IndentStmt],
    facts: &FixupFacts,
    function: FunctionId,
    union_records: &BTreeSet<String>,
    disqualified: &mut BTreeSet<String>,
) {
    for indent in body {
        walk_stmt_for_union_sourcing(&indent.stmt, facts, function, union_records, disqualified);
    }
}

fn walk_stmt_for_union_sourcing(
    stmt: &Stmt,
    facts: &FixupFacts,
    function: FunctionId,
    union_records: &BTreeSet<String>,
    disqualified: &mut BTreeSet<String>,
) {
    match stmt {
        Stmt::Let {
            name,
            init: Some(init),
            ..
        } => {
            check_union_source(
                name.as_str(),
                init,
                facts,
                function,
                union_records,
                disqualified,
            );
        }
        Stmt::Assign {
            target: Expr::Var(name),
            value,
        } => {
            check_union_source(
                name.as_str(),
                value,
                facts,
                function,
                union_records,
                disqualified,
            );
        }
        _ => {}
    }
    walk::nested_bodies_with_path(stmt, &mut Vec::new(), &mut |nested, _| {
        walk_stmts_for_union_sourcing(nested, facts, function, union_records, disqualified);
    });
}

fn check_union_source(
    name: &str,
    source: &Expr,
    facts: &FixupFacts,
    function: FunctionId,
    union_records: &BTreeSet<String>,
    disqualified: &mut BTreeSet<String>,
) {
    if let Expr::Field { base, .. } = peel_casts(source)
        && is_union_typed(base, facts, function, union_records)
    {
        disqualified.insert(name.to_string());
    }
}

fn is_union_typed(
    base: &Expr,
    facts: &FixupFacts,
    function: FunctionId,
    union_records: &BTreeSet<String>,
) -> bool {
    let Expr::Var(name) = peel_casts(base) else {
        return false;
    };
    let Some(binding) = facts.binding_named(function, name.as_str()) else {
        return false;
    };
    let Some(ty) = facts.binding_type_ast(binding) else {
        return false;
    };
    record_name(ty).is_some_and(|record_name| union_records.contains(record_name))
}

fn record_name(ty: &Type) -> Option<&str> {
    match ty {
        Type::Custom(name) => Some(name.as_str()),
        Type::Ptr { inner, .. } | Type::Ref { inner, .. } => record_name(inner),
        _ => None,
    }
}

fn is_zero_constant(expr: &Expr) -> bool {
    matches!(expr, Expr::Value(RustValue::I64(0)))
}

fn is_int_or_void_ptr(ty: &Type) -> bool {
    match ty {
        Type::Prim(prim) => !matches!(prim, crate::rust_ast::Prim::Bool),
        Type::Ptr { inner, .. } => matches!(&**inner, Type::CLib(CLibType::Void)),
        _ => false,
    }
}

fn peel_casts(expr: &Expr) -> &Expr {
    match expr {
        Expr::Cast { expr, .. } => peel_casts(expr),
        Expr::Unsafe(block) | Expr::Block(block) if block.stmts.is_empty() => {
            match block.tail.as_deref() {
                Some(inner) => peel_casts(inner),
                None => expr,
            }
        }
        _ => expr,
    }
}

fn is_null_expr(expr: &Expr) -> bool {
    matches!(peel_casts(expr), Expr::Value(RustValue::NullPtr))
}

fn is_pointer_expr(facts: &FixupFacts, function: FunctionId, expr: &Expr) -> bool {
    let Expr::Var(name) = peel_casts(expr) else {
        return false;
    };
    facts
        .binding_named(function, name.as_str())
        .and_then(|binding| facts.binding_type_ast(binding))
        .is_some_and(|ty| matches!(ty, Type::Ptr { .. }))
}

fn comparison_kind(
    facts: &FixupFacts,
    function: FunctionId,
    lhs: &Expr,
    rhs: &Expr,
) -> Option<PointerComparisonKind> {
    let lhs_null = is_null_expr(lhs);
    let rhs_null = is_null_expr(rhs);
    if lhs_null && rhs_null {
        return None;
    }
    if lhs_null {
        return is_pointer_expr(facts, function, rhs).then_some(PointerComparisonKind::NullCompare);
    }
    if rhs_null {
        return is_pointer_expr(facts, function, lhs).then_some(PointerComparisonKind::NullCompare);
    }
    if is_pointer_expr(facts, function, lhs) && is_pointer_expr(facts, function, rhs) {
        return Some(PointerComparisonKind::IdentityCompare);
    }
    None
}

fn collect_comparisons_for_function(
    function: FunctionId,
    f: &FnDef,
    facts: &FixupFacts,
    out: &mut Vec<PointerComparisonFact>,
) {
    let mut path: Vec<PathSegment> = Vec::new();
    walk::body_exprs_with_path(&f.body, &mut path, &mut |expr, path| {
        if let Expr::Binary {
            op: BinOp::Eq | BinOp::Ne,
            lhs,
            rhs,
        } = expr
            && let Some(kind) = comparison_kind(facts, function, lhs, rhs)
        {
            out.push(PointerComparisonFact {
                site: Site {
                    function,
                    path: AstPath(path.clone()),
                },
                kind,
            });
        }
    });
}
