use crate::fixups::idents::stmt_ident_count;
use crate::fixups::trace::Pass;
use crate::rust_ast::{BinOp, Expr, IndentStmt, RustValue, Stmt, UnaryOp};

use super::super::item::StatementMatch;
use super::super::{EditSet, ItemCaseContext, QueryRule, Rejection, StatementSequence};

pub(in crate::fixups) fn rewrite() -> QueryRule<StatementSequence<2>> {
    QueryRule::new(
        Pass::SliceReduce,
        "rewrite_slice_reduction",
        StatementSequence::new(),
    )
    .case("accumulator_fold", rewrite_case)
}

fn rewrite_case(
    case: &mut ItemCaseContext<'_, '_>,
    matched: &StatementMatch<2>,
) -> Result<EditSet, Rejection> {
    let [accum_stmt, loop_stmt] = case.statements(matched)?;
    let Stmt::Let {
        name: accum_name,
        mutable: true,
        ty,
        init: Some(init),
    } = &accum_stmt.stmt
    else {
        return Err(case.reject());
    };
    let Stmt::For {
        pat: item_name,
        iter,
        body,
    } = &loop_stmt.stmt
    else {
        return Err(case.reject());
    };
    let Expr::MethodCall { recv, method, args } = iter else {
        return Err(case.reject());
    };
    case.require(method == "iter" && args.is_empty())?;
    let Expr::Var(slice_name) = &**recv else {
        return Err(case.reject());
    };
    let [reduce_stmt] = body.as_slice() else {
        return Err(case.reject());
    };
    let Stmt::CompoundAssign { target, op, value } = &reduce_stmt.stmt else {
        return Err(case.reject());
    };
    case.require(matches!(target, Expr::Var(name) if name.as_str() == accum_name.as_str()))?;
    case.require(is_bare_item_deref(value, item_name.as_str()))?;

    let preceding = case.preceding_statements(matched)?;
    case.require(
        preceding
            .iter()
            .all(|indent| stmt_ident_count(&indent.stmt, accum_name.as_str()) == 0),
    )?;

    let Some(init_call) = reduction_call(slice_name.as_str(), *op, init) else {
        return Err(case.reject());
    };

    Ok(EditSet::replace_statements(
        matched.target().clone(),
        vec![IndentStmt {
            depth: loop_stmt.depth,
            stmt: Stmt::Let {
                name: accum_name.clone(),
                mutable: false,
                ty: ty.clone(),
                init: Some(init_call),
            },
        }],
    ))
}

fn is_bare_item_deref(expr: &Expr, item_name: &str) -> bool {
    matches!(expr, Expr::Unary { op: UnaryOp::Deref, expr }
        if matches!(&**expr, Expr::Var(name) if name.as_str() == item_name))
}

fn reduction_call(slice_name: &str, op: BinOp, init: &Expr) -> Option<Expr> {
    let iter_expr = Expr::MethodCall {
        recv: Box::new(Expr::Var(slice_name.into())),
        method: "iter".into(),
        args: Vec::new(),
    };
    match op {
        BinOp::Add if integer_value(init) == Some(0) => Some(Expr::MethodCall {
            recv: Box::new(iter_expr),
            method: "sum".into(),
            args: Vec::new(),
        }),
        BinOp::Mul if integer_value(init) == Some(1) => Some(Expr::MethodCall {
            recv: Box::new(iter_expr),
            method: "product".into(),
            args: Vec::new(),
        }),
        BinOp::BitAnd | BinOp::BitOr | BinOp::BitXor => Some(Expr::MethodCall {
            recv: Box::new(iter_expr),
            method: "fold".into(),
            args: vec![
                init.clone(),
                Expr::Closure {
                    params: vec!["acc".into(), "x".into()],
                    body: Box::new(Expr::Binary {
                        op,
                        lhs: Box::new(Expr::Var("acc".into())),
                        rhs: Box::new(Expr::Unary {
                            op: UnaryOp::Deref,
                            expr: Box::new(Expr::Var("x".into())),
                        }),
                    }),
                },
            ],
        }),
        _ => None,
    }
}

fn integer_value(expr: &Expr) -> Option<i128> {
    match expr {
        Expr::Value(RustValue::I64(n)) => Some(i128::from(*n)),
        Expr::Value(RustValue::I128(n)) => Some(*n),
        Expr::Cast { expr, .. } => integer_value(expr),
        _ => None,
    }
}
