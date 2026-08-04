use std::collections::{BTreeMap, BTreeSet};

use crate::fixups::facts::walk;
use crate::fixups::facts::{
    AstPath, FixupFacts, FunctionId, OptionBoxAssignKind, OptionBoxAssignment, OptionBoxComparison,
    OptionBoxLocalCandidate, PathSegment,
};
use crate::function_identity::{Known, known_call};
use crate::rust_ast::{BinOp, Expr, FnDef, IndentStmt, Item, Program, RustValue, Stmt, Type};

pub(in crate::fixups) fn collect_facts(program: &Program, facts: &mut FixupFacts) {
    facts.option_box_locals.clear();
    facts.option_box_comparisons.clear();
    let mut all = Vec::new();
    let mut all_comparisons = Vec::new();
    for (item_index, item) in program.items.iter().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        let let_defs = collect_let_defs(&f.body);
        let candidates = collect_for_function(function, f, facts);
        let candidate_names: BTreeSet<String> = candidates.iter().map(|c| c.name.clone()).collect();
        let mut comparisons = Vec::new();
        collect_comparisons(
            &f.body,
            &mut Vec::new(),
            function,
            &candidate_names,
            &let_defs,
            &mut comparisons,
        );
        all.extend(candidates);
        all_comparisons.extend(comparisons);
    }
    facts.option_box_locals = all;
    facts.option_box_comparisons = all_comparisons;
}

fn collect_comparisons(
    body: &[IndentStmt],
    path: &mut Vec<PathSegment>,
    function: FunctionId,
    candidate_names: &BTreeSet<String>,
    let_defs: &BTreeMap<String, Expr>,
    comparisons: &mut Vec<OptionBoxComparison>,
) {
    for (index, indent) in body.iter().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            if let Stmt::If { cond, .. } = &indent.stmt
                && let Some((lhs, rhs, negate)) = comparison_shape(cond, candidate_names, let_defs)
            {
                comparisons.push(OptionBoxComparison {
                    function,
                    if_stmt_path: AstPath(path.clone()),
                    lhs,
                    rhs,
                    negate,
                });
            }
            walk::nested_bodies_with_path(&indent.stmt, path, &mut |nested, path| {
                collect_comparisons(
                    nested,
                    path,
                    function,
                    candidate_names,
                    let_defs,
                    comparisons,
                );
            });
        });
    }
}

fn resolve_cond<'e>(let_defs: &'e BTreeMap<String, Expr>, expr: &'e Expr, depth: u32) -> &'e Expr {
    if depth == 0 {
        return expr;
    }
    if let Expr::Var(name) = peel_casts(expr)
        && let Some(def) = let_defs.get(name.as_str())
    {
        return resolve_cond(let_defs, def, depth - 1);
    }
    expr
}

fn comparison_shape(
    cond: &Expr,
    candidate_names: &BTreeSet<String>,
    let_defs: &BTreeMap<String, Expr>,
) -> Option<(String, String, bool)> {
    let Expr::Binary { op, lhs, rhs } = resolve_cond(let_defs, cond, 4) else {
        return None;
    };
    let negate = match op {
        BinOp::Eq => false,
        BinOp::Ne => true,
        _ => return None,
    };
    let Expr::Var(lhs_name) = peel_casts(lhs) else {
        return None;
    };
    let Expr::Var(rhs_name) = peel_casts(rhs) else {
        return None;
    };
    if lhs_name.as_str() == rhs_name.as_str() {
        return None;
    }
    if candidate_names.contains(lhs_name.as_str()) && candidate_names.contains(rhs_name.as_str()) {
        Some((
            lhs_name.as_str().to_string(),
            rhs_name.as_str().to_string(),
            negate,
        ))
    } else {
        None
    }
}

fn collect_for_function(
    function: FunctionId,
    f: &FnDef,
    facts: &FixupFacts,
) -> Vec<OptionBoxLocalCandidate> {
    let let_defs = collect_let_defs(&f.body);
    let mut candidates = Vec::new();
    for (index, indent) in f.body.iter().enumerate() {
        let Stmt::Let {
            name,
            ty: Some(ty),
            init: Some(init),
            ..
        } = &indent.stmt
        else {
            continue;
        };
        let Type::Ptr { inner: elem_ty, .. } = ty else {
            continue;
        };
        if !is_null_expr(init) {
            continue;
        }
        let Some(binding) = facts.binding_named(function, name.as_str()) else {
            continue;
        };
        let decl_path = AstPath(vec![PathSegment::Stmt(index)]);
        let mut assignments = Vec::new();
        let mut ok = true;
        collect_assignments(
            &f.body,
            &mut Vec::new(),
            name.as_str(),
            &let_defs,
            &mut assignments,
            &mut ok,
        );
        if !ok || assignments.is_empty() {
            continue;
        }
        let mut deref_paths = Vec::new();
        walk::body_exprs_with_path(&f.body, &mut Vec::new(), &mut |expr, path| {
            if let Expr::Unary {
                op: crate::rust_ast::UnaryOp::Deref,
                expr: inner,
            } = expr
                && let Expr::Var(inner_name) = peel_casts(inner)
                && inner_name.as_str() == name.as_str()
            {
                deref_paths.push(AstPath(path.clone()));
            }
        });
        candidates.push(OptionBoxLocalCandidate {
            function,
            binding,
            name: name.clone(),
            elem_ty: (**elem_ty).clone(),
            decl_path,
            assignments,
            deref_paths,
        });
    }
    candidates
}

fn collect_let_defs(body: &[IndentStmt]) -> BTreeMap<String, Expr> {
    let mut defs = BTreeMap::new();
    collect_let_defs_in(body, &mut defs);
    defs
}

fn collect_let_defs_in(body: &[IndentStmt], defs: &mut BTreeMap<String, Expr>) {
    for indent in body {
        if let Stmt::Let {
            name,
            init: Some(init),
            ..
        } = &indent.stmt
        {
            defs.insert(name.clone(), init.clone());
        }
        walk::nested_bodies_with_path(&indent.stmt, &mut Vec::new(), &mut |nested, _| {
            collect_let_defs_in(nested, defs);
        });
    }
}

fn collect_assignments(
    body: &[IndentStmt],
    path: &mut Vec<PathSegment>,
    name: &str,
    let_defs: &BTreeMap<String, Expr>,
    assignments: &mut Vec<OptionBoxAssignment>,
    ok: &mut bool,
) {
    for (index, indent) in body.iter().enumerate() {
        if let Stmt::Assign {
            target: Expr::Var(target_name),
            value,
        } = &indent.stmt
            && target_name.as_str() == name
        {
            let kind = if is_null_expr(value) {
                Some((OptionBoxAssignKind::Null, None))
            } else if let Some(malloc_name) = malloc_sourced_name(value, let_defs) {
                let alloc_source =
                    find_let_index(&body[..index], malloc_name.as_str()).map(|malloc_index| {
                        let mut malloc_path = path.clone();
                        malloc_path.push(PathSegment::Stmt(malloc_index));
                        AstPath(malloc_path)
                    });
                Some((OptionBoxAssignKind::Alloc, alloc_source))
            } else {
                None
            };
            match kind {
                Some((kind, alloc_source)) => {
                    let mut assign_path = path.clone();
                    assign_path.push(PathSegment::Stmt(index));
                    assignments.push(OptionBoxAssignment {
                        path: AstPath(assign_path),
                        kind,
                        alloc_source,
                    });
                }
                None => *ok = false,
            }
        }
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            walk::nested_bodies_with_path(&indent.stmt, path, &mut |nested, path| {
                collect_assignments(nested, path, name, let_defs, assignments, ok);
            });
        });
    }
}

fn find_let_index(body: &[IndentStmt], name: &str) -> Option<usize> {
    body.iter().position(|indent| {
        matches!(&indent.stmt, Stmt::Let { name: let_name, .. } if let_name.as_str() == name)
    })
}

fn malloc_sourced_name(expr: &Expr, let_defs: &BTreeMap<String, Expr>) -> Option<String> {
    match peel_casts(expr) {
        Expr::Var(name) => let_defs
            .get(name.as_str())
            .is_some_and(is_malloc_call)
            .then(|| name.as_str().to_string()),
        _ => None,
    }
}

fn is_malloc_call(expr: &Expr) -> bool {
    matches!(peel_casts(expr), call @ Expr::Call { .. } if known_call(call) == Some(Known::Malloc))
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
