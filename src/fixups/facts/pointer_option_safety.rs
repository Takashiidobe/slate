use std::collections::BTreeSet;

use crate::fixups::facts::walk;
use crate::fixups::facts::{
    self, AstPath, BindingFact, BindingTypeFact, FunctionId, PathSegment, PointerComparisonFact,
    PointerComparisonKind, PointerOptionSafetyFact, Site,
};
use crate::rust_ast::{BinOp, CLibType, Expr, FnDef, IndentStmt, RustValue, Stmt, Type};
pub(in crate::fixups) fn collect_for_function<'db>(
    function: FunctionId<'db>,
    f: &FnDef,
    bindings: &[BindingFact<'db>],
    binding_types: &[BindingTypeFact<'db>],
    union_records: &BTreeSet<String>,
) -> (
    Vec<PointerOptionSafetyFact<'db>>,
    Vec<PointerComparisonFact<'db>>,
) {
    let safety = collect_safety_for_function(function, f, bindings, binding_types, union_records);
    let mut comparisons = Vec::new();
    collect_comparisons_for_function(function, f, bindings, binding_types, &mut comparisons);
    (safety, comparisons)
}

fn collect_safety_for_function<'db>(
    function: FunctionId<'db>,
    f: &FnDef,
    bindings: &[BindingFact<'db>],
    binding_types: &[BindingTypeFact<'db>],
    union_records: &BTreeSet<String>,
) -> Vec<PointerOptionSafetyFact<'db>> {
    let mut disqualified = BTreeSet::new();

    walk::body_exprs(&f.body, &mut |expr| {
        check_arithmetic_and_casts(expr, &mut disqualified);
    });
    walk_stmts_for_union_sourcing(
        &f.body,
        bindings,
        binding_types,
        function,
        union_records,
        &mut disqualified,
    );

    bindings
        .iter()
        .filter(|binding| binding.function == function)
        .filter(|binding| {
            facts::binding_type_ast(binding_types, binding.id)
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

fn walk_stmts_for_union_sourcing<'db>(
    body: &[IndentStmt],
    bindings: &[BindingFact<'db>],
    binding_types: &[BindingTypeFact<'db>],
    function: FunctionId<'db>,
    union_records: &BTreeSet<String>,
    disqualified: &mut BTreeSet<String>,
) {
    for indent in body {
        walk_stmt_for_union_sourcing(
            &indent.stmt,
            bindings,
            binding_types,
            function,
            union_records,
            disqualified,
        );
    }
}

fn walk_stmt_for_union_sourcing<'db>(
    stmt: &Stmt,
    bindings: &[BindingFact<'db>],
    binding_types: &[BindingTypeFact<'db>],
    function: FunctionId<'db>,
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
                bindings,
                binding_types,
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
                bindings,
                binding_types,
                function,
                union_records,
                disqualified,
            );
        }
        _ => {}
    }
    walk::nested_bodies_with_path(stmt, &mut Vec::new(), &mut |nested, _| {
        walk_stmts_for_union_sourcing(
            nested,
            bindings,
            binding_types,
            function,
            union_records,
            disqualified,
        );
    });
}

fn check_union_source<'db>(
    name: &str,
    source: &Expr,
    bindings: &[BindingFact<'db>],
    binding_types: &[BindingTypeFact<'db>],
    function: FunctionId<'db>,
    union_records: &BTreeSet<String>,
    disqualified: &mut BTreeSet<String>,
) {
    if let Expr::Field { base, .. } = peel_casts(source)
        && is_union_typed(base, bindings, binding_types, function, union_records)
    {
        disqualified.insert(name.to_string());
    }
}

fn is_union_typed<'db>(
    base: &Expr,
    bindings: &[BindingFact<'db>],
    binding_types: &[BindingTypeFact<'db>],
    function: FunctionId<'db>,
    union_records: &BTreeSet<String>,
) -> bool {
    let Expr::Var(name) = peel_casts(base) else {
        return false;
    };
    let Some(binding) = facts::binding_named(bindings, function, name.as_str()) else {
        return false;
    };
    let Some(ty) = facts::binding_type_ast(binding_types, binding) else {
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

fn is_pointer_expr<'db>(
    bindings: &[BindingFact<'db>],
    binding_types: &[BindingTypeFact<'db>],
    function: FunctionId<'db>,
    expr: &Expr,
) -> bool {
    let Expr::Var(name) = peel_casts(expr) else {
        return false;
    };
    facts::binding_named(bindings, function, name.as_str())
        .and_then(|binding| facts::binding_type_ast(binding_types, binding))
        .is_some_and(|ty| matches!(ty, Type::Ptr { .. }))
}

fn comparison_kind<'db>(
    bindings: &[BindingFact<'db>],
    binding_types: &[BindingTypeFact<'db>],
    function: FunctionId<'db>,
    lhs: &Expr,
    rhs: &Expr,
) -> Option<PointerComparisonKind> {
    let lhs_null = is_null_expr(lhs);
    let rhs_null = is_null_expr(rhs);
    if lhs_null && rhs_null {
        return None;
    }
    if lhs_null {
        return is_pointer_expr(bindings, binding_types, function, rhs)
            .then_some(PointerComparisonKind::NullCompare);
    }
    if rhs_null {
        return is_pointer_expr(bindings, binding_types, function, lhs)
            .then_some(PointerComparisonKind::NullCompare);
    }
    if is_pointer_expr(bindings, binding_types, function, lhs)
        && is_pointer_expr(bindings, binding_types, function, rhs)
    {
        return Some(PointerComparisonKind::IdentityCompare);
    }
    None
}

fn collect_comparisons_for_function<'db>(
    function: FunctionId<'db>,
    f: &FnDef,
    bindings: &[BindingFact<'db>],
    binding_types: &[BindingTypeFact<'db>],
    out: &mut Vec<PointerComparisonFact<'db>>,
) {
    let mut path: Vec<PathSegment> = Vec::new();
    walk::body_exprs_with_path(&f.body, &mut path, &mut |expr, path| {
        if let Expr::Binary {
            op: BinOp::Eq | BinOp::Ne,
            lhs,
            rhs,
        } = expr
            && let Some(kind) = comparison_kind(bindings, binding_types, function, lhs, rhs)
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
