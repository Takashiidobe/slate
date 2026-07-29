mod context;
mod proof;
mod rewrite;
mod views;

pub(in crate::fixups) use context::{CallRecord, CallTarget, QueryContext};
pub(in crate::fixups) use proof::{
    Evidence, EvidenceDetail, Predicate, Proof, QueryResult, Rejection, RejectionReason,
};
pub(in crate::fixups) use rewrite::{
    ApplyReport, CaseRejection, ExprPlan, ExprPlanBuilder, ExprRule, PlanDiagnostic, ReplaceExpr,
    RuleCase, RuleCaseIdentity, RuleIdentity, RuleResult,
};
pub(in crate::fixups) use views::{
    ByteExtent, ByteRepresentation, ByteSource, ByteView, ExprSite, NulPosition, PointerMutability,
    StableExpr,
};
