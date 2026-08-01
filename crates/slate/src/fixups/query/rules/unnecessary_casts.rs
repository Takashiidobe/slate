use crate::fixups::trace::Pass;
use crate::rust_ast::{BinOp, Expr, Prim, Type};

use super::super::{
    Evidence, ExprRule, ExprSite, Predicate, QueryContext, QueryResult, Rejection, RejectionReason,
    ReplaceExpr, RuleCase, RuleIdentity, RuleResult,
};

pub(in crate::fixups) struct NarrowingPromotionCastRule {
    identity: RuleIdentity,
}

pub(in crate::fixups) fn rewrite() -> NarrowingPromotionCastRule {
    NarrowingPromotionCastRule {
        identity: RuleIdentity::new(Pass::UnnecessaryCasts, "strip_unnecessary_assignment_cast"),
    }
}

impl ExprRule for NarrowingPromotionCastRule {
    type Candidate = ExprSite;

    fn identity(&self) -> RuleIdentity {
        self.identity.clone()
    }

    fn candidates(&self, query: &QueryContext<'_>) -> Vec<ExprSite> {
        query.assign_value_sites()
    }

    fn target(&self, candidate: &ExprSite) -> ExprSite {
        candidate.clone()
    }

    fn cases(&self, query: &QueryContext<'_>, candidate: &ExprSite) -> Vec<RuleCase> {
        vec![RuleCase::new(
            "resolved",
            RuleResult::from(narrowing_promotion_cast(query, candidate)),
        )]
    }
}

/// `(narrow)((int)a + (int)b)` where the promotion to `int` and the
/// truncation back to `narrow` are both redundant given the operands'
/// recorded cast facts - `a + b` already means the same thing.
fn narrowing_promotion_cast(
    query: &QueryContext<'_>,
    site: &ExprSite,
) -> Result<ReplaceExpr, Rejection> {
    let reject = |reason| Rejection::new(Predicate::Cast, Some(site.clone()), reason, Vec::new());

    let Some(Expr::Cast { expr, ty: outer_ty }) = query.expr(site) else {
        return Err(reject(RejectionReason::UnsupportedShape));
    };
    let Type::Prim(target) = outer_ty else {
        return Err(reject(RejectionReason::UnsupportedShape));
    };
    let target = *target;
    if !is_promoted_narrow_int(target) {
        return Err(reject(RejectionReason::UnsupportedShape));
    }
    let Expr::Binary { op, lhs, rhs } = &**expr else {
        return Err(reject(RejectionReason::UnsupportedShape));
    };
    let op = *op;
    if !matches!(op, BinOp::Add) {
        return Err(reject(RejectionReason::UnsupportedShape));
    }

    let mut evidence = Vec::new();
    let outer = prove(&mut evidence, query.cast_at(site))?;
    if !type_is_prim(outer.from.as_ref(), Prim::I32) || !owned_type_is_prim(&outer.to, target) {
        return Err(Rejection::new(
            Predicate::Cast,
            Some(site.clone()),
            RejectionReason::Contradicted,
            evidence,
        ));
    }

    let binary_site = query.child(site, 0);
    let lhs_site = query.child(&binary_site, 0);
    let rhs_site = query.child(&binary_site, 1);
    let lhs = stripped_operand(query, &mut evidence, lhs, target, &lhs_site)?;
    let rhs = stripped_operand(query, &mut evidence, rhs, target, &rhs_site)?;

    Ok(ReplaceExpr::new(
        site.clone(),
        Expr::Binary {
            op,
            lhs: Box::new(lhs),
            rhs: Box::new(rhs),
        },
    )
    .with_evidence(evidence))
}

fn stripped_operand(
    query: &QueryContext<'_>,
    evidence: &mut Vec<Evidence>,
    operand: &Expr,
    target: Prim,
    site: &ExprSite,
) -> Result<Expr, Rejection> {
    let Expr::Cast { expr, ty } = operand else {
        return Err(Rejection::new(
            Predicate::Cast,
            Some(site.clone()),
            RejectionReason::UnsupportedShape,
            evidence.clone(),
        ));
    };
    if !owned_type_is_prim(ty, Prim::I32) {
        return Err(Rejection::new(
            Predicate::Cast,
            Some(site.clone()),
            RejectionReason::UnsupportedShape,
            evidence.clone(),
        ));
    }
    let fact = prove(evidence, query.cast_at(site))?;
    if !type_is_prim(fact.from.as_ref(), target) || !owned_type_is_prim(&fact.to, Prim::I32) {
        return Err(Rejection::new(
            Predicate::Cast,
            Some(site.clone()),
            RejectionReason::Contradicted,
            evidence.clone(),
        ));
    }
    Ok((**expr).clone())
}

fn prove<T>(evidence: &mut Vec<Evidence>, result: QueryResult<T>) -> Result<T, Rejection> {
    match result {
        Ok(proof) => {
            evidence.extend(proof.evidence);
            Ok(proof.value)
        }
        Err(mut rejection) => {
            let mut combined = evidence.clone();
            combined.append(&mut rejection.evidence);
            rejection.evidence = combined;
            Err(rejection)
        }
    }
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
