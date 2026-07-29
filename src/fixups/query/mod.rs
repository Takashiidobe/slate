mod context;
mod definition;
mod program;
mod program_recipe;
mod proof;
mod recipe;
mod rewrite;
mod rule;
pub(in crate::fixups) mod rules;
mod views;

pub(in crate::fixups) use context::{CallRecord, CallTarget, QueryContext};
pub(in crate::fixups) use definition::{
    DefinitionApplyReport, DefinitionCaseContext, DefinitionPlan, DefinitionPlanBuilder,
    DefinitionPlanDiagnostic, DefinitionRecipe, DefinitionRule, delete_definition, replace_body,
};
pub(in crate::fixups) use program::{
    ProgramApplyReport, ProgramCaseContext, ProgramPlan, ProgramPlanBuilder, ProgramPlanDiagnostic,
    ProgramRule,
};
pub(in crate::fixups) use program_recipe::{ProgramRecipe, rewrite_anonymous_structs};
pub(in crate::fixups) use proof::{
    Evidence, EvidenceDetail, Predicate, Proof, QueryResult, Rejection, RejectionReason,
};
pub(in crate::fixups) use recipe::{
    ExprRecipe, FunctionBodyRecipe, SearchIndex, byte_position, known_index, memchr_fallback_body,
    pointer_at_or_null, process_exit,
};
pub(in crate::fixups) use rewrite::{
    ApplyReport, CaseRejection, ExprPlan, ExprPlanBuilder, ExprRule, PlanDiagnostic, ReplaceExpr,
    RuleCase, RuleCaseIdentity, RuleIdentity, RuleResult,
};
pub(in crate::fixups) use rule::{CallArg, CallCaseContext, CallRule};
use views::{AnonymousStructField, AnonymousStructPlan};
pub(in crate::fixups) use views::{
    AnonymousStructSet, ByteExtent, ByteRepresentation, ByteSource, ByteView, DefinitionGroup,
    DefinitionKind, DefinitionLocation, DefinitionSelector, DefinitionSite, ExprSite, NulPosition,
    PointerMutability, StableExpr, ZeroGroupUsers, ZeroUsers,
};
