use crate::fixups::trace::Pass;
use crate::rust_ast::{BinOp, Expr, Prim, Type};

use super::super::{
    AssignmentValue, EditSet, ExprSite, ExpressionRef, ItemCaseContext, Predicate, QueryRule,
    Rejection, RejectionReason,
};

pub(in crate::fixups) fn rewrite() -> QueryRule<AssignmentValue> {
    QueryRule::new(
        Pass::UnnecessaryCasts,
        "strip_unnecessary_assignment_cast",
        AssignmentValue,
    )
    .case("resolved", narrowing_promotion_cast)
}

/// `(narrow)((int)a + (int)b)` where the promotion to `int` and the
/// truncation back to `narrow` are both redundant given the operands'
/// recorded cast facts - `a + b` already means the same thing.
fn narrowing_promotion_cast(
    case: &mut ItemCaseContext<'_, '_>,
    expression: &ExpressionRef,
) -> Result<EditSet, Rejection> {
    let site = &expression.site;

    let Some(Expr::Cast { expr, ty: outer_ty }) = case.expr(site).cloned() else {
        return Err(case.reject_at(Predicate::Cast, site, RejectionReason::UnsupportedShape));
    };
    let Type::Prim(target) = outer_ty else {
        return Err(case.reject_at(Predicate::Cast, site, RejectionReason::UnsupportedShape));
    };
    if !is_promoted_narrow_int(target) {
        return Err(case.reject_at(Predicate::Cast, site, RejectionReason::UnsupportedShape));
    }
    let Expr::Binary { op, lhs, rhs } = *expr else {
        return Err(case.reject_at(Predicate::Cast, site, RejectionReason::UnsupportedShape));
    };
    if !matches!(op, BinOp::Add) {
        return Err(case.reject_at(Predicate::Cast, site, RejectionReason::UnsupportedShape));
    }

    let outer = case.fact(|query| query.cast_at(site))?;
    if !type_is_prim(outer.from.as_ref(), Prim::I32) || !owned_type_is_prim(&outer.to, target) {
        return Err(Rejection::new(
            Predicate::Cast,
            Some(site.clone()),
            RejectionReason::Contradicted,
            Vec::new(),
        ));
    }

    let binary_site = case.fact(|query| query.expression(site))?;
    let binary_site = case.fact(|query| query.expression(&query.child(&binary_site.site, 0)))?;
    let lhs_site = case.fact(|query| query.expression(&query.child(&binary_site.site, 0)))?;
    let rhs_site = case.fact(|query| query.expression(&query.child(&binary_site.site, 1)))?;
    let lhs = stripped_operand(case, &lhs, target, &lhs_site.site)?;
    let rhs = stripped_operand(case, &rhs, target, &rhs_site.site)?;

    Ok(EditSet::replace_expression(
        site.clone(),
        Expr::Binary {
            op,
            lhs: Box::new(lhs),
            rhs: Box::new(rhs),
        },
    ))
}

fn stripped_operand(
    case: &mut ItemCaseContext<'_, '_>,
    operand: &Expr,
    target: Prim,
    site: &ExprSite,
) -> Result<Expr, Rejection> {
    let Expr::Cast { expr, ty } = operand else {
        return Err(Rejection::new(
            Predicate::Cast,
            Some(site.clone()),
            RejectionReason::UnsupportedShape,
            Vec::new(),
        ));
    };
    if !owned_type_is_prim(ty, Prim::I32) {
        return Err(Rejection::new(
            Predicate::Cast,
            Some(site.clone()),
            RejectionReason::UnsupportedShape,
            Vec::new(),
        ));
    }
    let fact = case.fact(|query| query.cast_at(site))?;
    if !type_is_prim(fact.from.as_ref(), target) || !owned_type_is_prim(&fact.to, Prim::I32) {
        return Err(Rejection::new(
            Predicate::Cast,
            Some(site.clone()),
            RejectionReason::Contradicted,
            Vec::new(),
        ));
    }
    Ok((**expr).clone())
}

fn type_is_prim(ty: Option<&Type>, expected: Prim) -> bool {
    matches!(ty, Some(Type::Prim(actual)) if *actual == expected)
}

fn owned_type_is_prim(ty: &Type, expected: Prim) -> bool {
    matches!(ty, Type::Prim(actual) if *actual == expected)
}

fn is_promoted_narrow_int(prim: Prim) -> bool {
    matches!(prim, Prim::I8 | Prim::U8 | Prim::I16 | Prim::U16)
}
