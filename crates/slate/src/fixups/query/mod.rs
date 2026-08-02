mod context;
mod field;
mod item;
mod parameter;
mod patterns;
mod plan;
mod program;
mod program_recipe;
mod proof;
mod recipe;
mod rewrite;
pub(in crate::fixups) mod rules;
mod views;

use context::default_value;
pub(in crate::fixups) use context::{CallRecord, CallTarget, QueryContext};
pub(in crate::fixups) use field::Field;
pub(in crate::fixups) use item::{
    EditSet, ItemCaseContext, ItemPlanBuilder, QueryRule, same_statement_container,
};
pub(in crate::fixups) use patterns::{
    AssignmentValue, Binding, Definition, ExternFn, FnCall, LetStmtPattern, Local, LoopStmtPattern,
    NullaryMethodCall, StatementSequence, Value,
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
    pointer_at_or_null, process_exit, rewrite_buffer_cursor, rewrite_heap_ownership,
    rewrite_inline_temp, rewrite_zero_init,
};
pub(in crate::fixups) use rewrite::{CaseRejection, RuleCaseIdentity, RuleIdentity};
use views::{
    AnonymousStructField, AnonymousStructPlan, ArrayElementPointerOrigin, BufferCursorPlan,
    HeapOwnershipPlan, HeapOwnershipReallocPlan, LazySingletonPlan, PtrLenPlan, ZeroInitPlan,
};
pub(in crate::fixups) use views::{
    AnonymousStructSet, BindingAccess, BindingCategory, BindingDefUse, BindingRef, BindingUse,
    BindingUses, ByteExtent, ByteRepresentation, ByteSource, ByteView, DefinitionGroup,
    DefinitionKind, DefinitionLocation, DefinitionSelector, DefinitionSite, ExprSite,
    ExpressionEffects, ExpressionPlace, ExpressionRef, ExpressionValues, FunctionRef,
    HeapOwnershipPlanSet, InlineTempPlan, ItemReferences, LazySingletonSet, NulPosition,
    ParameterRemoval, Phase, PointerMutability, PtrLenPlanSet, ReferenceDomain, ResolvedValue,
    StableExpr, StatementRange, Usage, ValueSite, ZeroGroupUsers, ZeroUsers,
};
