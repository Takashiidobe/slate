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
mod value;
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
    rewrite_unused_param,
};
pub(in crate::fixups) use proof::{
    Evidence, EvidenceDetail, Predicate, Proof, QueryResult, Rejection, RejectionReason,
};
pub(in crate::fixups) use recipe::{
    ExprRecipe, FunctionBodyRecipe, byte_position, known_index, memchr_fallback_body,
    pointer_at_or_null, process_exit, rewrite_buffer_cursor, rewrite_heap_ownership,
    rewrite_inline_temp, rewrite_zero_init,
};
pub(in crate::fixups) use rewrite::{
    CaseRejection, ExprPlanBuilder, ExprRule, ReplaceExpr, RuleCase, RuleCaseIdentity,
    RuleIdentity, RuleResult,
};
pub(in crate::fixups) use rule::CallRule;
pub(in crate::fixups) use stmt_window::{StmtWindowPlanBuilder, StmtWindowRule};
pub(in crate::fixups) use value::{
    ValueCaseContext, ValueEdit, ValuePlanBuilder, ValueRule, same_container,
};
use views::{
    AnonymousStructField, AnonymousStructPlan, ArrayElementPointerOrigin, BufferCursorPlan,
    HeapOwnershipPlan, HeapOwnershipReallocPlan, LazySingletonPlan, ParamSite, PtrLenPlan,
    UnusedParamPlan, UnusedTypeDefinitionSet, ZeroInitPlan,
};
pub(in crate::fixups) use views::{
    AnonymousStructSet, BindingDefUse, BindingRef, ByteExtent, ByteRepresentation, ByteSource,
    ByteView, DefinitionGroup, DefinitionKind, DefinitionLocation, DefinitionSelector,
    DefinitionSite, ExprSite, HeapOwnershipPlanSet, InlineTempPlan, LazySingletonSet, NulPosition,
    Phase, PointerMutability, PtrLenPlanSet, ResolvedValue, StableExpr, StmtWindowSite, Usage,
    ValueSite, ZeroGroupUsers, ZeroUsers,
};
