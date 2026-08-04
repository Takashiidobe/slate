mod array_env;
mod atomic;
mod context;
mod control_flow;
mod ctype_classify;
mod field;
mod item;
mod mem_cmp;
mod mem_move;
mod mem_set;
mod nullable_pointer;
mod patterns;
mod plan;
mod printf;
mod program_recipe;
mod proof;
mod ptr_copy;
mod recipe;
mod remove_mut;
mod retval;
mod rewrite;
pub(in crate::fixups) mod rules;
mod slice_index;
mod sort_search;
mod stdio;
mod switch;
mod va_list;
mod var_aliases;
mod views;

use context::default_value;
pub(in crate::fixups) use context::{
    CallRecord, CallTarget, InterproceduralAllocCallerInput, OptionBoxLocalPlanInput, QueryContext,
};
pub(in crate::fixups) use field::Field;
pub(in crate::fixups) use item::{
    EditSet, ItemCaseContext, ItemPlanBuilder, QueryRule, StatementRef, same_statement_container,
};
use nullable_pointer::{
    find_distance_observation, null_comparison, nullable_pointer_option, preferred_option_name,
    removable_alias_decl, supported_observation, transparent_alias_value,
};
pub(in crate::fixups) use patterns::{
    AssignmentValue, Binding, Definition, ExprPattern, ExternFn, FnCall, Function, LetStmtPattern,
    Local, LoopStmtPattern, NullaryMethodCall, Parameter, StatementSequence, Value, WholeProgram,
};
pub(in crate::fixups) use plan::TouchedItems;
use printf::{PrintfStream, printf_macro, sprintf_format_expr, sprintf_worst_case_len};
pub(in crate::fixups) use program_recipe::{
    rewrite_anonymous_structs, rewrite_atomic_locals, rewrite_lazy_singletons,
    rewrite_printf_fallback, rewrite_ptr_len, rewrite_sort_search,
};
pub(in crate::fixups) use proof::{
    Evidence, EvidenceDetail, Predicate, Proof, QueryResult, Rejection, RejectionReason,
};
pub(in crate::fixups) use recipe::{
    ExprRecipe, FunctionBodyRecipe, HeapOwnershipPlan, HeapOwnershipReallocPlan,
    InterproceduralAllocCalleePlan, InterproceduralAllocCallerPlan, NullablePointerAlias,
    NullablePointerPlan, OptionBoxComparisonPlan, OptionBoxLocalPlan, byte_position,
    collapse_atomic_compare_exchange, initialize_local, known_index, pointer_at_or_null,
    preserve_assert_result, process_exit, recover_assert, rewrite_file_ownership,
    rewrite_heap_ownership, rewrite_interprocedural_alloc_callee,
    rewrite_interprocedural_alloc_caller, rewrite_nullable_pointer, rewrite_option_box_locals,
};
pub(in crate::fixups) use rewrite::{CaseRejection, RuleCaseIdentity, RuleIdentity};
use views::{
    AnonymousStructField, AnonymousStructPlan, ArrayElementPointerOrigin, AtomicGlobalPromotion,
    AtomicLocalPromotion, LazySingletonPlan, PtrLenPlan,
};
pub(in crate::fixups) use views::{
    AnonymousStructSet, AtomicCompareExchangeChain, AtomicPromotionSet, BindingAccess,
    BindingCategory, BindingDefUse, BindingRef, BindingUse, BindingUses, BufferPointerField,
    BufferPointerFields, ByteExtent, ByteRepresentation, ByteSource, ByteView, DefinitionGroup,
    DefinitionGroupUsers, DefinitionKind, DefinitionLocation, DefinitionSelector, DefinitionSite,
    DefinitionUsers, DispatchRegion, EnumVariantRef, ExprSite, ExpressionEffects, ExpressionKind,
    ExpressionPlace, ExpressionRef, ExpressionRole, ExpressionValues, FieldRef, FileOwnership,
    FileOwnershipFacts, FileUse, FunctionCallDomain, FunctionReachability, FunctionRef,
    HeapOwnership, HeapOwnershipFacts, HeapReallocation, HeapUse, ItemReferences, LazySingletonSet,
    MatchArmRef, NulPosition, ParameterRef, Phase, PointerMutability, ProgramRef, PtrLenPlanSet,
    ReferenceDomain, ResolvedValue, SliceLoopFact, StableExpr, StatementContainerRef,
    StatementRange, StringCopyAction, StringCopySite, StringLibcUse, SwitchDispatch, TypeUseKind,
    TypeUseRef, Usage, UseSiteRef, VaListAlias, ValueSite,
};
