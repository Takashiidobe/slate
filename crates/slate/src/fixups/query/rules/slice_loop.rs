use crate::fixups::facts::{
    CountedLoopBound, CountedLoopIndexUse, CountedLoopStart, CountedLoopStep, SliceLoopAccess,
};
use crate::fixups::support::walk;
use crate::fixups::trace::Pass;
use crate::rust_ast::{Expr, Ident, IndentStmt, RustValue, Stmt, Type, UnaryOp};

use super::super::item::StatementMatch;
use super::super::{
    EditSet, ItemCaseContext, LetStmtPattern, LoopStmtPattern, QueryRule, Rejection,
    StatementSequence,
};

pub(in crate::fixups) fn rewrite() -> QueryRule<StatementSequence<2>> {
    QueryRule::new(
        Pass::SliceLoop,
        "rewrite_counted_loop_to_slice_iter",
        StatementSequence::new(),
    )
    .case("zero_step_one", rewrite_case)
}

fn rewrite_case(
    case: &mut ItemCaseContext<'_, '_>,
    matched: &StatementMatch<2>,
) -> Result<EditSet, Rejection> {
    let [index_stmt, loop_stmt] = case.statements(matched)?;
    let Some(index_name) = LetStmtPattern::any().matches(&index_stmt.stmt, &()) else {
        return Err(case.reject());
    };
    let index_name = index_name.to_string();
    let Some(loop_body) = LoopStmtPattern::unlabeled().matches(&loop_stmt.stmt, &()) else {
        return Err(case.reject());
    };
    case.require(loop_body.len() >= 2)?;

    let statement = matched.statement(1);
    let fact = case.fact(|query| query.counted_slice_loop(&statement))?;
    case.require(fact.start == CountedLoopStart::Zero)?;
    case.require(fact.bound == CountedLoopBound::SliceLen)?;
    case.require(fact.step == CountedLoopStep::One)?;
    case.require(matches!(
        fact.index_use,
        CountedLoopIndexUse::SliceIndexOnly | CountedLoopIndexUse::SliceIndexAndValue
    ))?;

    let slice_name = fact.slice.name.clone();
    let method = match fact.access {
        SliceLoopAccess::ReadOnly => "iter",
        SliceLoopAccess::Mutable => "iter_mut",
    };
    let mut body = loop_body[1..loop_body.len() - 1].to_vec();
    let Some(item_name) = extract_materialized_item(&mut body, &slice_name, &index_name) else {
        return Err(case.reject());
    };
    rewrite_index_uses(&mut body, &slice_name, &index_name, &item_name);

    let iter = Expr::MethodCall {
        recv: Box::new(Expr::Var(Ident::new(slice_name))),
        method: method.into(),
        args: Vec::new(),
    };

    let (pat, iter) = if fact.index_use == CountedLoopIndexUse::SliceIndexAndValue {
        let Some(orig_ty) = fact.index.ty.clone() else {
            return Err(case.reject());
        };
        if orig_ty != Type::parse("usize") {
            body.insert(0, index_cast_shadow(&index_name, orig_ty, &body));
        }
        (
            format!("({index_name}, {item_name})"),
            Expr::MethodCall {
                recv: Box::new(iter),
                method: "enumerate".into(),
                args: Vec::new(),
            },
        )
    } else {
        (item_name, iter)
    };

    Ok(EditSet::replace_statements(
        matched.target().clone(),
        vec![IndentStmt {
            depth: loop_stmt.depth,
            stmt: Stmt::For { pat, iter, body },
        }],
    ))
}

fn index_cast_shadow(index_name: &str, orig_ty: Type, body: &[IndentStmt]) -> IndentStmt {
    let depth = body.first().map_or(0, |indent| indent.depth);
    IndentStmt {
        depth,
        stmt: Stmt::Let {
            name: index_name.to_string(),
            mutable: false,
            ty: Some(orig_ty.clone()),
            init: Some(Expr::Cast {
                expr: Box::new(Expr::Var(Ident::new(index_name))),
                ty: orig_ty,
            }),
        },
    }
}

fn extract_materialized_item(
    body: &mut Vec<IndentStmt>,
    slice_name: &str,
    index_name: &str,
) -> Option<String> {
    for index in 0..body.len() {
        if let Some(name) = extract_assigned_materialized_item(body, index, slice_name, index_name)
        {
            return Some(name);
        }
        if let Stmt::Let {
            name,
            init: Some(init),
            ..
        } = &body[index].stmt
            && !is_synthetic_temp(name)
            && is_slice_index(init, slice_name, index_name)
        {
            let name = name.clone();
            body.remove(index);
            substitute_remaining(body, index, &name);
            return Some(name);
        }
        let mut found = None;
        walk::nested_body_vecs_mut_with_path(
            &mut body[index].stmt,
            &mut Vec::new(),
            &mut |nested, _path| {
                if found.is_none() {
                    found = extract_materialized_item(nested, slice_name, index_name);
                }
            },
        );
        if let Some(name) = found {
            return Some(name);
        }
    }
    None
}

fn extract_assigned_materialized_item(
    body: &mut Vec<IndentStmt>,
    index: usize,
    slice_name: &str,
    index_name: &str,
) -> Option<String> {
    let [
        IndentStmt {
            stmt:
                Stmt::Let {
                    name: item_name,
                    init: Some(item_init),
                    ..
                },
            ..
        },
        IndentStmt {
            stmt:
                Stmt::Let {
                    name: temp_name,
                    init: Some(temp_init),
                    ..
                },
            ..
        },
        IndentStmt {
            stmt:
                Stmt::Assign {
                    target,
                    value: assign_value,
                },
            ..
        },
    ] = body.get(index..index + 3)?
    else {
        return None;
    };
    if is_synthetic_temp(item_name)
        || !is_zero(item_init)
        || !is_synthetic_temp(temp_name)
        || !is_slice_index(temp_init, slice_name, index_name)
        || !matches!(target, Expr::Var(name) if name.as_str() == item_name)
        || !matches!(assign_value, Expr::Var(name) if name.as_str() == temp_name)
    {
        return None;
    }
    let item_name = item_name.clone();
    body.splice(index..index + 3, []);
    substitute_remaining(body, index, &item_name);
    Some(item_name)
}

fn substitute_remaining(body: &mut [IndentStmt], start: usize, name: &str) {
    let replacement = item_deref(name);
    for indent in &mut body[start..] {
        indent.stmt.substitute_var(name, &replacement);
    }
}

fn is_synthetic_temp(name: &str) -> bool {
    name.strip_prefix("_v")
        .is_some_and(|suffix| !suffix.is_empty() && suffix.chars().all(|ch| ch.is_ascii_digit()))
}

fn is_zero(expr: &Expr) -> bool {
    matches!(
        expr,
        Expr::Value(RustValue::I64(0) | RustValue::Usize(0) | RustValue::I128(0))
    )
}

fn rewrite_index_uses(
    body: &mut [IndentStmt],
    slice_name: &str,
    index_name: &str,
    item_name: &str,
) {
    for indent in body {
        walk::stmt_exprs_mut_with(&mut indent.stmt, &mut |expr| {
            if is_slice_index(expr, slice_name, index_name) {
                *expr = item_deref(item_name);
                return false;
            }
            true
        });
    }
}

fn is_slice_index(expr: &Expr, slice_name: &str, index_name: &str) -> bool {
    let Expr::Index { base, index } = expr else {
        return false;
    };
    matches!(&**base, Expr::Var(name) if name.as_str() == slice_name)
        && is_index_expr(index, index_name)
}

fn is_index_expr(expr: &Expr, index_name: &str) -> bool {
    match expr {
        Expr::Var(name) => name.as_str() == index_name,
        Expr::Cast { expr, .. } => is_index_expr(expr, index_name),
        _ => false,
    }
}

fn item_deref(item_name: &str) -> Expr {
    Expr::Unary {
        op: UnaryOp::Deref,
        expr: Box::new(Expr::Var(Ident::new(item_name))),
    }
}
