mod context;
mod field;
mod item;
mod parameter;
mod patterns;
mod plan;
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
#[allow(unused_imports)]
pub(in crate::fixups) use patterns::{
    AssignmentValue, Binding, Definition, EnumVariant, ExprPattern, ExternFn, FnCall, Function,
    LetStmtPattern, Local, LoopStmtPattern, MatchArm, NullaryMethodCall, Parameter, RecordField,
    StatementContainer, StatementSequence, TypeUse, Value, WholeProgram,
};
pub(in crate::fixups) use plan::TouchedItems;
pub(in crate::fixups) use program_recipe::{
    rewrite_anonymous_structs, rewrite_lazy_singletons, rewrite_ptr_len,
};
pub(in crate::fixups) use proof::{
    Evidence, EvidenceDetail, Predicate, Proof, QueryResult, Rejection, RejectionReason,
};
pub(in crate::fixups) use recipe::{
    ExprRecipe, FunctionBodyRecipe, byte_position, initialize_local, known_index,
    memchr_fallback_body, pointer_at_or_null, process_exit, rewrite_heap_ownership,
};
pub(in crate::fixups) use rewrite::{CaseRejection, RuleCaseIdentity, RuleIdentity};
use views::{
    AnonymousStructField, AnonymousStructPlan, ArrayElementPointerOrigin, HeapOwnershipPlan,
    HeapOwnershipReallocPlan, LazySingletonPlan, PtrLenPlan,
};
pub(in crate::fixups) use views::{
    AnonymousStructSet, BindingAccess, BindingCategory, BindingDefUse, BindingRef, BindingUse,
    BindingUses, BufferPointerField, BufferPointerFields, ByteExtent, ByteRepresentation,
    ByteSource, ByteView, DefinitionGroup, DefinitionGroupUsers, DefinitionKind,
    DefinitionLocation, DefinitionSelector, DefinitionSite, DefinitionUsers, EnumVariantRef,
    ExprSite, ExpressionEffects, ExpressionKind, ExpressionPlace, ExpressionRef, ExpressionRole,
    ExpressionValues, FieldRef, FunctionRef, HeapOwnershipPlanSet, ItemReferences,
    LazySingletonSet, MatchArmRef, NulPosition, ParameterRef, ParameterRemoval, Phase,
    PointerMutability, ProgramRef, PtrLenPlanSet, ReferenceDomain, ResolvedValue, StableExpr,
    StatementContainerRef, StatementRange, TypeUseKind, TypeUseRef, Usage, UseSiteRef, ValueSite,
};
