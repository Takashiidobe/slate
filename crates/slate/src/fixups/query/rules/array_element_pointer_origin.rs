use crate::fixups::facts::PathSegment;
use crate::fixups::trace::Pass;
use crate::rust_ast::{BinOp, Expr, UnaryOp};

use super::super::{ArrayElementPointerOrigin, Local, ValueCaseContext, ValueEdit, ValueRule};

pub(in crate::fixups) fn rewrite() -> ValueRule {
    ValueRule::new(
        Pass::ArrayElementPointerOrigin,
        "rewrite_array_element_pointer_origins",
        Local::default(),
    )
    .case("known_origin", |case| {
        let origin = case.pointer_origin()?;
        let mut edits = Vec::new();
        let mut covered: Vec<Vec<PathSegment>> = Vec::new();
        let mut skip_prefix: Option<Vec<PathSegment>> = None;
        for site in case.exprs()? {
            if skip_prefix
                .as_ref()
                .is_some_and(|prefix| site.path.0.starts_with(prefix.as_slice()))
            {
                continue;
            }
            let Some(rewritten) = case
                .expr(&site)
                .cloned()
                .and_then(|expr| array_element_read(case, &expr, &origin))
            else {
                continue;
            };
            skip_prefix = Some(site.path.0.clone());
            covered.push(site.path.0.clone());
            edits.push(ValueEdit::replace_expr(site, rewritten));
        }
        let still_referenced = case.uses()?.iter().any(|use_site| {
            !covered
                .iter()
                .any(|prefix| use_site.path.0.starts_with(prefix.as_slice()))
        });
        if !still_referenced {
            edits.push(ValueEdit::remove_stmt(case.site()));
        }
        Ok(edits)
    })
}

fn array_element_read(
    case: &mut ValueCaseContext<'_, '_>,
    expr: &Expr,
    origin: &ArrayElementPointerOrigin,
) -> Option<Expr> {
    match expr {
        Expr::Unsafe(block) if block.stmts.is_empty() => {
            array_element_read(case, block.tail.as_ref()?, origin)
        }
        Expr::Unary {
            op: UnaryOp::Deref,
            expr,
        } => indexed_array_element_pointer(expr, origin),
        Expr::Cast { expr, ty } => {
            array_element_pointer_diff(case, expr, origin).map(|expr| Expr::Cast {
                expr: Box::new(expr),
                ty: ty.clone(),
            })
        }
        _ => array_element_pointer_diff(case, expr, origin),
    }
}

fn indexed_array_element_pointer(expr: &Expr, origin: &ArrayElementPointerOrigin) -> Option<Expr> {
    let Expr::Var(name) = peel_array_element_unsafe(expr) else {
        return None;
    };
    (name.as_str() == origin.pointer_name).then(|| Expr::Index {
        base: Box::new(Expr::Var(origin.base_name.as_str().into())),
        index: Box::new(origin.index.clone()),
    })
}

fn array_element_pointer_diff(
    case: &mut ValueCaseContext<'_, '_>,
    expr: &Expr,
    origin: &ArrayElementPointerOrigin,
) -> Option<Expr> {
    let Expr::MethodCall { recv, method, args } = peel_array_element_unsafe(expr) else {
        return None;
    };
    if method != "offset_from" || args.len() != 1 {
        return None;
    }
    let Expr::Var(recv_name) = peel_array_element_unsafe(recv) else {
        return None;
    };
    if recv_name.as_str() != origin.pointer_name {
        return None;
    }
    let arg = args[0].clone();
    let rhs = pointer_origin_for(case, &arg, origin)?;
    if origin.base_name != rhs.base_name {
        return None;
    }
    Some(Expr::Binary {
        op: BinOp::Sub,
        lhs: Box::new(origin.index.clone()),
        rhs: Box::new(rhs.index),
    })
}

fn pointer_origin_for(
    case: &mut ValueCaseContext<'_, '_>,
    expr: &Expr,
    current: &ArrayElementPointerOrigin,
) -> Option<ArrayElementPointerOrigin> {
    let Expr::Var(name) = peel_array_element_unsafe(expr) else {
        return None;
    };
    if name.as_str() == current.pointer_name {
        return Some(current.clone());
    }
    case.pointer_origin_named(name.as_str()).ok()
}

fn peel_array_element_unsafe(expr: &Expr) -> &Expr {
    match expr {
        Expr::Unsafe(block) if block.stmts.is_empty() => block
            .tail
            .as_deref()
            .map_or(expr, peel_array_element_unsafe),
        _ => expr,
    }
}
