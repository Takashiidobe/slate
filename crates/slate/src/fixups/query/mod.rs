mod context;
mod definition;
mod proof;
mod recipe;
mod rewrite;
mod rule;
pub(in crate::fixups) mod rules;
mod views;

pub(in crate::fixups) use context::{CallRecord, CallTarget, QueryContext};
pub(in crate::fixups) use definition::{
    DefinitionApplyReport, DefinitionPlan, DefinitionPlanBuilder, DefinitionPlanDiagnostic,
    DefinitionRule, DeleteDefinition,
};
pub(in crate::fixups) use proof::{
    Evidence, EvidenceDetail, Predicate, Proof, QueryResult, Rejection, RejectionReason,
};
pub(in crate::fixups) use recipe::{
    ExprRecipe, SearchIndex, byte_position, known_index, pointer_at_or_null,
};
pub(in crate::fixups) use rewrite::{
    ApplyReport, CaseRejection, ExprPlan, ExprPlanBuilder, ExprRule, PlanDiagnostic, ReplaceExpr,
    RuleCase, RuleCaseIdentity, RuleIdentity, RuleResult,
};
pub(in crate::fixups) use rule::{CallArg, CallCaseContext, CallRule};
pub(in crate::fixups) use views::{
    ByteExtent, ByteRepresentation, ByteSource, ByteView, DefinitionKind, DefinitionLocation,
    DefinitionSelector, DefinitionSite, ExprSite, NulPosition, PointerMutability, StableExpr,
    ZeroUsers,
};
