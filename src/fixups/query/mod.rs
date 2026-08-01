mod context;
mod definition;
mod field;
mod patterns;
mod plan;
mod program;
mod program_recipe;
mod proof;
mod recipe;
mod rewrite;
mod rule;
pub(in crate::fixups) mod rules;
mod stmt_window;
mod views;

use context::default_value;
pub(in crate::fixups) use context::{CallRecord, CallTarget, QueryContext};
pub(in crate::fixups) use definition::{
    DefinitionPlanBuilder, DefinitionRule, delete_definition, replace_body,
};
pub(in crate::fixups) use field::Field;
pub(in crate::fixups) use patterns::{
    Definition, ExternFn, FnCall, LetStmtPattern, Local, LoopStmtPattern, NullaryMethodCall, Value,
};
pub(in crate::fixups) use plan::TouchedItems;
pub(in crate::fixups) use program::{ProgramPlanBuilder, ProgramRule};
pub(in crate::fixups) use program_recipe::{
    ProgramRecipe, rewrite_anonymous_structs, rewrite_lazy_singletons, rewrite_ptr_len,
};
pub(in crate::fixups) use proof::{
    Evidence, EvidenceDetail, Predicate, Proof, QueryResult, Rejection, RejectionReason,
};
pub(in crate::fixups) use recipe::{
    ExprRecipe, FunctionBodyRecipe, byte_position, known_index, memchr_fallback_body,
    pointer_at_or_null, process_exit, rewrite_heap_ownership, rewrite_inline_temp,
    rewrite_string_lift,
};
pub(in crate::fixups) use rewrite::{
    CaseRejection, ExprPlanBuilder, ExprRule, ReplaceExpr, RuleCase, RuleCaseIdentity,
    RuleIdentity, RuleResult,
};
pub(in crate::fixups) use rule::CallRule;
pub(in crate::fixups) use stmt_window::{StmtWindowPlanBuilder, StmtWindowRule};
use views::{
    AnonymousStructField, AnonymousStructPlan, HeapOwnershipPlan, HeapOwnershipReallocPlan,
    LazySingletonPlan, PtrLenPlan, StringLiftPlan,
};
pub(in crate::fixups) use views::{
    AnonymousStructSet, ByteExtent, ByteRepresentation, ByteSource, ByteView, DefinitionGroup,
    DefinitionKind, DefinitionLocation, DefinitionSelector, DefinitionSite, ExprSite,
    HeapOwnershipPlanSet, InlineTempPlan, LazySingletonSet, NulPosition, Phase, PointerMutability,
    PtrLenPlanSet, ResolvedValue, StableExpr, StmtWindowSite, StringLiftPlanSet, Usage,
    ZeroGroupUsers, ZeroUsers,
};
