use std::cell::RefCell;
use std::collections::{BTreeMap, BTreeSet, HashMap};
use std::marker::PhantomData;

use crate::fixups::facts::walk;
use crate::fixups::facts::{
    AnonymousStructFact, ArrayElementPointerOriginFact, AsciiNumericSign, AstPath,
    AtomicGlobalFact, AtomicLocalFact, BindingId, BindingKind, BorrowAliasReason,
    BufferPointerFieldFact, CStringLiteralFact, CallArgFact, CallArgPinning, CallCallee,
    CalleeAllocSummaryFact, CallsiteFact, CastFact, ConstValue, ControlFlowFact,
    ControlFlowSubject, CountedLoopFact, CountedSliceLoopFact, DefUseFact, EffectFact,
    EffectSubject, FileOwnershipFact, FixupFacts, FunctionId, HeapOwnershipFact,
    InterproceduralAllocCallerFact, InterproceduralAllocEligibilityFact, LazyInitSingletonFact,
    NulTermination, NullCheckDominanceFact, NullCheckProof, OptionBoxAssignKind,
    OptionBoxComparison, OptionBoxLocalCandidate, PathSegment, PlaceFact, PointerComparisonFact,
    PointerComparisonKind, PointerOptionSafetyFact, PrintfCallFact, PtrLenSliceFact, Purity,
    StringBufferFact, StringBufferKind, StringCopyRewrite, StringCopyRewriteFact,
    StringLibcUseFact, StringParamLiftFact, StringPointerViewFact, StringRecoveryCandidate,
    StructFieldOwnershipFact, ValueSubject,
};
use crate::fixups::salsa::SalsaFacts;
use crate::function_identity::{CallBinding, FunctionIdentity, Known};
use crate::rust_ast::{
    Attr, Block, Expr, ExternDecl, FnDef, FnParam, GenericParam, ImplBlock, ImplItem, IndentStmt,
    Item, MatchArm, Method, Pattern, Prim, Program, RecordDef, RustValue, Stmt, StructDef,
    StructFields, TraitBound, Type, Visibility,
};

use super::item::StatementRef;
use super::switch;
use super::va_list;
use super::{
    AnonymousStructField, AnonymousStructPlan, AnonymousStructSet, ArrayElementPointerOrigin,
    AtomicCompareExchangeChain, AtomicGlobalPromotion, AtomicLocalPromotion, AtomicPromotionSet,
    BindingAccess, BindingCategory, BindingDefUse, BindingRef, BindingUse, BindingUses,
    BufferPointerField, BufferPointerFields, ByteExtent, ByteRepresentation, ByteSource, ByteView,
    DefinitionGroup, DefinitionGroupUsers, DefinitionKind, DefinitionLocation, DefinitionSelector,
    DefinitionSite, DefinitionUsers, DispatchRegion, EnumVariantRef, Evidence, EvidenceDetail,
    ExprSite, ExpressionEffects, ExpressionKind, ExpressionPlace, ExpressionRef, ExpressionRole,
    ExpressionValues, ExternFn, FieldRef, FileOwnership, FileOwnershipFacts, FileUse,
    FunctionCallDomain, FunctionReachability, FunctionRef, HeapOwnership, HeapOwnershipFacts,
    HeapReallocation, HeapUse, ItemReferences, LazySingletonPlan, LazySingletonSet, MatchArmRef,
    NulPosition, NullaryMethodCall, ParameterRef, PointerMutability, Predicate, Proof, PtrLenPlan,
    PtrLenPlanSet, QueryResult, ReferenceDomain, Rejection, RejectionReason, ResolvedValue,
    SliceLoopFact, StableExpr, StatementContainerRef, StatementRange, StringCopyAction,
    StringCopySite, StringLibcUse, SwitchDispatch, TypeUseRef, Usage, UseSiteRef, VaListAlias,
    ValueSite,
};

macro_rules! query_cache {
    ($(
        $(#[$attr:meta])*
        fn $name:ident(& $slf:tt $(, $arg:ident : $arg_ty:ty)*) -> QueryResult<$ret:ty>;
        key: $key_ty:ty = $key:expr;
        $body:block
    )*) => {
        #[derive(Default)]
        struct QueryCache<'snapshot> {
            $($(#[$attr])* $name: RefCell<HashMap<$key_ty, QueryResult<$ret>>>,)*
        }

        impl<'snapshot> QueryContext<'snapshot> {
            $(
                $(#[$attr])*
                pub(in crate::fixups) fn $name(&$slf, $($arg: $arg_ty),*) -> QueryResult<$ret> {
                    cached(&$slf.cache.$name, $key, || $body)
                }
            )*
        }
    };
}

fn cached<K, V>(cache: &RefCell<HashMap<K, V>>, key: K, compute: impl FnOnce() -> V) -> V
where
    K: Eq + std::hash::Hash + Clone,
    V: Clone,
{
    if let Some(hit) = cache.borrow().get(&key) {
        return hit.clone();
    }
    let value = compute();
    cache.borrow_mut().insert(key, value.clone());
    value
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord)]
pub(in crate::fixups) enum CallTarget {
    Known(Known),
    Generated(String),
    Direct(String),
    Indirect,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct NullCheckDominance {
    pub(in crate::fixups) proof: NullCheckProof,
    pub(in crate::fixups) guard_stmt: Option<StatementRef>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct OptionBoxLocalPlanInput {
    pub(in crate::fixups) binding: BindingRef,
    pub(in crate::fixups) elem_ty: Type,
    pub(in crate::fixups) decl_stmt: StatementRef,
    pub(in crate::fixups) assignments: Vec<OptionBoxAssignmentInput>,
    pub(in crate::fixups) deref_sites: Vec<ExprSite>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct OptionBoxAssignmentInput {
    pub(in crate::fixups) stmt: StatementRef,
    pub(in crate::fixups) kind: OptionBoxAssignKind,
    pub(in crate::fixups) alloc_source: Option<StatementRef>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct OptionBoxComparisonInput {
    pub(in crate::fixups) if_stmt: StatementRef,
    pub(in crate::fixups) lhs: String,
    pub(in crate::fixups) rhs: String,
    pub(in crate::fixups) negate: bool,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct InterproceduralAllocCallerInput {
    pub(in crate::fixups) caller: FunctionRef,
    pub(in crate::fixups) pointer_name: String,
    pub(in crate::fixups) decl_stmt: StatementRef,
    pub(in crate::fixups) call_temp_stmt: StatementRef,
    pub(in crate::fixups) free_stmt: Option<StatementRef>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct CallRecord {
    pub(in crate::fixups) site: ExprSite,
    pub(in crate::fixups) trivial_unsafe_site: Option<ExprSite>,
    pub(in crate::fixups) target: CallTarget,
    pub(in crate::fixups) args: Vec<ExprSite>,
    pub(in crate::fixups) evidence: Vec<Evidence>,
}

pub(in crate::fixups) struct QueryContext<'snapshot> {
    program: &'snapshot Program,
    facts: &'snapshot FixupFacts,
    calls: BTreeMap<(CallTarget, usize), Vec<CallRecord>>,
    calls_by_site: BTreeMap<ExprSite, CallRecord>,
    expression_sites: Vec<ExprSite>,
    expression_parents: BTreeMap<ExprSite, ExprSite>,
    assignment_values: BTreeSet<ExprSite>,
    expression_roles: BTreeMap<ExprSite, BTreeSet<ExpressionRole>>,
    definitions: BTreeMap<DefinitionSelector, Vec<DefinitionSite>>,
    symbol_uses: BTreeMap<String, Vec<usize>>,
    use_domain_complete: bool,
    cache: QueryCache<'snapshot>,
    salsa: Option<&'snapshot SalsaFacts>,
}

impl<'snapshot> QueryContext<'snapshot> {
    pub(in crate::fixups) fn new(
        program: &'snapshot Program,
        facts: &'snapshot FixupFacts,
    ) -> Self {
        let mut calls: BTreeMap<(CallTarget, usize), Vec<CallRecord>> = BTreeMap::new();
        let mut calls_by_site = BTreeMap::new();
        let mut expression_sites = Vec::new();
        let mut assignment_values = BTreeSet::new();
        let mut assignment_targets = BTreeSet::new();
        let mut assignment_role_values = BTreeSet::new();
        let mut definitions = BTreeMap::<DefinitionSelector, Vec<DefinitionSite>>::new();
        let mut symbol_uses = BTreeMap::<String, Vec<usize>>::new();
        let mut use_domain_complete = true;
        for (item_index, item) in program.items.iter().enumerate() {
            index_definitions(item, item_index, &mut definitions);
            use_domain_complete &= item_use_domain_complete(item);
            index_item_uses(item, item_index, &mut symbol_uses);
            if let Item::Fn(function) = item {
                collect_assign_value_sites(
                    item_index,
                    &function.body,
                    &mut Vec::new(),
                    &mut assignment_targets,
                    &mut assignment_role_values,
                    &mut assignment_values,
                );
                walk::body_exprs_with_path(&function.body, &mut Vec::new(), &mut |expr, path| {
                    let site = expression_site(item_index, path);
                    expression_sites.push(site.clone());
                    let Expr::Call {
                        func,
                        args,
                        binding,
                    } = expr
                    else {
                        return;
                    };
                    let target = call_target(func, binding);
                    let arg_sites = (0..args.len())
                        .map(|index| child_site(&site, index + 1))
                        .collect::<Vec<_>>();
                    let evidence = vec![Evidence {
                        predicate: Predicate::Call,
                        site: site.clone(),
                        detail: EvidenceDetail::IndexedCall {
                            target: target.clone(),
                            arity: args.len(),
                        },
                    }];
                    let record = CallRecord {
                        trivial_unsafe_site: trivial_unsafe_site(program, &site),
                        site,
                        target: target.clone(),
                        args: arg_sites,
                        evidence,
                    };
                    calls_by_site.insert(record.site.clone(), record.clone());
                    if let Some(wrapper) = &record.trivial_unsafe_site {
                        calls_by_site.insert(wrapper.clone(), record.clone());
                    }
                    calls
                        .entry((target.clone(), args.len()))
                        .or_default()
                        .push(record);
                });
            }
        }
        let expression_set = expression_sites.iter().cloned().collect::<BTreeSet<_>>();
        let expression_parents = expression_sites
            .iter()
            .filter_map(|site| {
                (1..site.path.0.len()).rev().find_map(|length| {
                    let parent = expression_site(site.item_index, &site.path.0[..length]);
                    expression_set
                        .contains(&parent)
                        .then(|| (site.clone(), parent))
                })
            })
            .collect();
        let mut expression_roles = BTreeMap::<ExprSite, BTreeSet<ExpressionRole>>::new();
        for site in assignment_targets {
            expression_roles
                .entry(site)
                .or_default()
                .insert(ExpressionRole::AssignmentTarget);
        }
        for site in assignment_role_values {
            expression_roles
                .entry(site)
                .or_default()
                .insert(ExpressionRole::AssignmentValue);
        }
        for call in calls_by_site.values() {
            expression_roles
                .entry(call.site.clone())
                .or_default()
                .insert(ExpressionRole::Call);
            expression_roles
                .entry(child_site(&call.site, 0))
                .or_default()
                .insert(ExpressionRole::CallCallee);
            for (index, argument) in call.args.iter().enumerate() {
                expression_roles
                    .entry(argument.clone())
                    .or_default()
                    .insert(ExpressionRole::CallArgument(index));
            }
        }
        Self {
            program,
            facts,
            calls,
            calls_by_site,
            expression_sites,
            expression_parents,
            assignment_values,
            expression_roles,
            definitions,
            symbol_uses,
            use_domain_complete,
            cache: QueryCache::default(),
            salsa: None,
        }
    }

    pub(in crate::fixups) fn with_salsa(mut self, salsa: &'snapshot SalsaFacts) -> Self {
        self.salsa = Some(salsa);
        self
    }

    fn def_use_fact(&self, function: FunctionId, binding: BindingId) -> Option<&DefUseFact> {
        match self.salsa {
            Some(salsa) => salsa.def_use(function, binding),
            None => self.facts.def_use(binding),
        }
    }

    fn effect_fact(
        &self,
        function: FunctionId,
        subject: EffectSubject,
        path: &AstPath,
    ) -> Option<&EffectFact> {
        match self.salsa {
            Some(salsa) => salsa.effect(function, subject, path),
            None => self.facts.effect(function, subject, path),
        }
    }

    fn values_at(
        &self,
        function: FunctionId,
        subject: ValueSubject,
        path: &AstPath,
    ) -> Vec<ConstValue> {
        match self.salsa {
            Some(salsa) => salsa
                .values_for(function)
                .iter()
                .filter(|fact| fact.subject == subject && &fact.site.path == path)
                .map(|fact| fact.value.clone())
                .collect(),
            None => self
                .facts
                .values_at(function, subject, path)
                .cloned()
                .collect(),
        }
    }

    fn value_matches(
        &self,
        function: FunctionId,
        subject: ValueSubject,
        value: ConstValue,
    ) -> bool {
        match self.salsa {
            Some(salsa) => salsa
                .values_for(function)
                .iter()
                .any(|fact| fact.subject == subject && fact.value == value),
            None => self.facts.values.iter().any(|fact| {
                fact.site.function == function && fact.subject == subject && fact.value == value
            }),
        }
    }

    fn string_buffer_fact_at(
        &self,
        function: FunctionId,
        path: &AstPath,
    ) -> Option<&StringBufferFact> {
        match self.salsa {
            Some(salsa) => salsa.string_buffer_at(function, path),
            None => self.facts.string_buffer_at(function, path),
        }
    }

    fn string_buffer_fact(&self, binding: BindingId) -> Option<&StringBufferFact> {
        match self.salsa {
            Some(salsa) => salsa.string_buffer(binding),
            None => self.facts.string_buffer(binding),
        }
    }

    fn string_pointer_view_facts(
        &self,
        function: FunctionId,
        binding: BindingId,
    ) -> Vec<&StringPointerViewFact> {
        match self.salsa {
            Some(salsa) => salsa
                .string_pointer_views(function)
                .iter()
                .filter(|view| view.source == binding)
                .collect(),
            None => self
                .facts
                .string_pointer_views
                .iter()
                .filter(|view| view.site.function == function && view.source == binding)
                .collect(),
        }
    }

    fn string_libc_use_fact(
        &self,
        function: FunctionId,
        path: &AstPath,
    ) -> Option<&StringLibcUseFact> {
        match self.salsa {
            Some(salsa) => salsa.string_libc_use(function, path),
            None => self.facts.string_libc_use(function, path),
        }
    }

    fn counted_loop_fact(
        &self,
        function: FunctionId,
        loop_path: &AstPath,
    ) -> Option<&CountedLoopFact> {
        match self.salsa {
            Some(salsa) => salsa.counted_loop(function, loop_path),
            None => {
                self.facts.counted_loops.iter().find(|fact| {
                    fact.site.function == function && fact.site.loop_path == *loop_path
                })
            }
        }
    }

    fn counted_slice_loop_fact(
        &self,
        function: FunctionId,
        loop_path: &AstPath,
    ) -> Option<&CountedSliceLoopFact> {
        match self.salsa {
            Some(salsa) => salsa.counted_slice_loop(function, loop_path),
            None => {
                self.facts.counted_slice_loops.iter().find(|fact| {
                    fact.site.function == function && fact.site.loop_path == *loop_path
                })
            }
        }
    }

    fn cast_fact_at(&self, function: FunctionId, path: &AstPath) -> Option<&CastFact> {
        match self.salsa {
            Some(salsa) => salsa.cast_at(function, path),
            None => self.facts.cast_at(function, path),
        }
    }

    fn place_fact(&self, function: FunctionId, path: &AstPath) -> Option<&PlaceFact> {
        match self.salsa {
            Some(salsa) => salsa.place(function, path),
            None => self.facts.place(function, path),
        }
    }

    fn control_flow_fact(
        &self,
        function: FunctionId,
        subject: ControlFlowSubject,
        path: &AstPath,
    ) -> Option<&ControlFlowFact> {
        match self.salsa {
            Some(salsa) => salsa.control_flow(function, subject, path),
            None => self.facts.control_flow(function, subject, path),
        }
    }

    fn binding_requires_mut_fact(&self, function: FunctionId, binding: BindingId) -> bool {
        match self.salsa {
            Some(salsa) => salsa.binding_requires_mut(function, binding),
            None => self.facts.binding_requires_mut(binding),
        }
    }

    fn borrow_alias_reasons_fact(
        &self,
        function: FunctionId,
        binding: BindingId,
    ) -> Option<BTreeSet<BorrowAliasReason>> {
        match self.salsa {
            Some(salsa) => salsa.borrow_alias_reasons(function, binding).cloned(),
            None => self
                .facts
                .borrow_alias
                .iter()
                .find(|fact| fact.binding == binding)
                .map(|fact| fact.reasons.clone()),
        }
    }

    fn array_element_pointer_origin_facts(
        &self,
        function: FunctionId,
    ) -> Vec<&ArrayElementPointerOriginFact> {
        match self.salsa {
            Some(salsa) => salsa
                .array_element_pointer_origins(function)
                .iter()
                .collect(),
            None => self
                .facts
                .array_element_pointer_origins
                .iter()
                .filter(|fact| fact.site.function == function)
                .collect(),
        }
    }

    fn null_check_dominance_at(
        &self,
        function: FunctionId,
        deref_path: &AstPath,
    ) -> Option<&NullCheckDominanceFact> {
        match self.salsa {
            Some(salsa) => salsa.null_check_dominance_at(function, deref_path),
            None => self.facts.null_check_dominance_at(function, deref_path),
        }
    }

    fn atomic_local_facts(&self) -> Vec<&AtomicLocalFact> {
        match self.salsa {
            Some(salsa) => salsa.atomic_locals().iter().collect(),
            None => self.facts.atomic_locals.iter().collect(),
        }
    }

    fn option_box_local_candidate_facts(
        &self,
        function: FunctionId,
    ) -> Vec<&OptionBoxLocalCandidate> {
        match self.salsa {
            Some(salsa) => salsa.option_box_local_candidates(function).iter().collect(),
            None => self
                .facts
                .option_box_locals
                .iter()
                .filter(|candidate| candidate.function == function)
                .collect(),
        }
    }

    fn option_box_comparison_facts(&self, function: FunctionId) -> Vec<&OptionBoxComparison> {
        match self.salsa {
            Some(salsa) => salsa.option_box_comparisons(function).iter().collect(),
            None => self
                .facts
                .option_box_comparisons
                .iter()
                .filter(|comparison| comparison.function == function)
                .collect(),
        }
    }

    fn buffer_pointer_field_facts(&self, function: FunctionId) -> Vec<&BufferPointerFieldFact> {
        match self.salsa {
            Some(salsa) => salsa.buffer_pointer_fields(function).iter().collect(),
            None => self
                .facts
                .buffer_pointer_fields
                .iter()
                .filter(|fact| fact.site.function == function)
                .collect(),
        }
    }

    fn heap_ownership_fact_list(&self, function: FunctionId) -> Vec<&HeapOwnershipFact> {
        match self.salsa {
            Some(salsa) => salsa.heap_ownership(function).iter().collect(),
            None => self
                .facts
                .heap_ownership
                .iter()
                .filter(|fact| fact.function == function)
                .collect(),
        }
    }

    fn file_ownership_fact_list(&self, function: FunctionId) -> Vec<&FileOwnershipFact> {
        match self.salsa {
            Some(salsa) => salsa.file_ownership(function).iter().collect(),
            None => self
                .facts
                .file_ownership
                .iter()
                .filter(|fact| fact.function == function)
                .collect(),
        }
    }

    fn printf_call_fact(&self, function: FunctionId, path: &AstPath) -> Option<PrintfCallFact> {
        match self.salsa {
            Some(salsa) => salsa
                .printf_calls(function)
                .iter()
                .find(|fact| &fact.site.path == path)
                .cloned(),
            None => self.facts.printf_call(function, path).cloned(),
        }
    }

    fn callee_alloc_summary_fact(&self, function: FunctionId) -> Option<&CalleeAllocSummaryFact> {
        match self.salsa {
            Some(salsa) => salsa.callee_alloc_summary(function),
            None => self
                .facts
                .callee_alloc_summaries
                .iter()
                .find(|summary| summary.function == function),
        }
    }

    fn interprocedural_alloc_eligibility_fact(
        &self,
        function: FunctionId,
    ) -> Option<InterproceduralAllocEligibilityFact> {
        match self.salsa {
            Some(salsa) => {
                let (eligibility, _) = salsa.interprocedural_alloc();
                eligibility
                    .iter()
                    .find(|fact| fact.function == function)
                    .cloned()
            }
            None => self
                .facts
                .interprocedural_alloc_eligibility
                .iter()
                .find(|fact| fact.function == function)
                .cloned(),
        }
    }

    fn interprocedural_alloc_caller_facts(
        &self,
        function: FunctionId,
    ) -> Vec<InterproceduralAllocCallerFact> {
        match self.salsa {
            Some(salsa) => {
                let (_, callers) = salsa.interprocedural_alloc();
                callers
                    .iter()
                    .filter(|caller| caller.callee == function)
                    .cloned()
                    .collect()
            }
            None => self
                .facts
                .interprocedural_alloc_callers
                .iter()
                .filter(|caller| caller.callee == function)
                .cloned()
                .collect(),
        }
    }

    fn pointer_option_safety_of(
        &self,
        function: FunctionId,
        binding: BindingId,
    ) -> Option<&PointerOptionSafetyFact> {
        match self.salsa {
            Some(salsa) => salsa.pointer_option_safety_of(function, binding),
            None => self.facts.pointer_option_safety_of(function, binding),
        }
    }

    #[expect(
        dead_code,
        reason = "query API surface not yet wired into a fixup rule"
    )]
    fn pointer_comparison_at(
        &self,
        function: FunctionId,
        path: &AstPath,
    ) -> Option<&PointerComparisonFact> {
        match self.salsa {
            Some(salsa) => salsa.pointer_comparison_at(function, path),
            None => self.facts.pointer_comparison_at(function, path),
        }
    }

    #[expect(
        dead_code,
        reason = "query API surface not yet wired into a fixup rule"
    )]
    fn struct_field_ownership_fact_list(&self) -> Vec<&StructFieldOwnershipFact> {
        match self.salsa {
            Some(salsa) => salsa.struct_field_ownership().iter().collect(),
            None => self.facts.struct_field_ownership.iter().collect(),
        }
    }

    fn callsite_fact(&self, function: FunctionId, path: &AstPath) -> Option<CallsiteFact> {
        match self.salsa {
            Some(salsa) => salsa.callsite(function, path),
            None => self.facts.callsite(function, path).cloned(),
        }
    }

    fn call_arg_fact(
        &self,
        function: FunctionId,
        path: &AstPath,
    ) -> Option<(CallsiteFact, CallArgFact)> {
        match self.salsa {
            Some(salsa) => salsa.call_arg_at(function, path),
            None => self
                .facts
                .call_arg_at(function, path)
                .map(|(callsite, arg)| (callsite.clone(), arg.clone())),
        }
    }

    fn ptr_len_slice_facts(&self) -> &[PtrLenSliceFact] {
        match self.salsa {
            Some(salsa) => salsa.ptr_len_slices(),
            None => &self.facts.ptr_len_slices,
        }
    }

    fn string_param_lift_facts(&self) -> &[StringParamLiftFact] {
        match self.salsa {
            Some(salsa) => salsa.string_param_lifts(),
            None => &self.facts.string_param_lifts,
        }
    }

    fn anonymous_struct_facts(&self) -> &[AnonymousStructFact] {
        match self.salsa {
            Some(salsa) => salsa.anonymous_structs(),
            None => &self.facts.anonymous_structs,
        }
    }

    fn lazy_init_singleton_facts(&self) -> &[LazyInitSingletonFact] {
        match self.salsa {
            Some(salsa) => salsa.lazy_init_singletons(),
            None => &self.facts.lazy_init_singletons,
        }
    }

    fn liftable_string_bindings_fact(
        &self,
        function: FunctionId,
        recovery: StringRecoveryCandidate,
    ) -> BTreeSet<BindingId> {
        match self.salsa {
            Some(salsa) => salsa.liftable_string_bindings(function, recovery),
            None => self.facts.liftable_string_bindings(function, recovery),
        }
    }

    fn string_use_allowed_fact(
        &self,
        function: FunctionId,
        use_path: &AstPath,
        binding: BindingId,
        recovery: StringRecoveryCandidate,
        liftable: &BTreeSet<BindingId>,
    ) -> bool {
        match self.salsa {
            Some(salsa) => {
                salsa.string_use_allowed(function, use_path, binding, recovery, liftable)
            }
            None => self
                .facts
                .string_use_allowed(function, use_path, binding, recovery, liftable),
        }
    }

    fn atomic_global_facts(&self) -> &[AtomicGlobalFact] {
        match self.salsa {
            Some(salsa) => salsa.atomic_globals(),
            None => &self.facts.atomic_globals,
        }
    }

    fn c_string_literal_fact(
        &self,
        function: FunctionId,
        receiver_path: &AstPath,
    ) -> Option<&CStringLiteralFact> {
        match self.salsa {
            Some(salsa) => salsa.c_string_literal(function, receiver_path),
            None => self.facts.c_string_literal(function, receiver_path),
        }
    }

    fn string_copy_rewrite_facts(&self, function: FunctionId) -> Vec<&StringCopyRewriteFact> {
        match self.salsa {
            Some(salsa) => salsa
                .string_copy_rewrites(function)
                .iter()
                .filter(|fact| fact.site.function == function)
                .collect(),
            None => self
                .facts
                .string_copy_rewrites
                .iter()
                .filter(|fact| fact.site.function == function)
                .collect(),
        }
    }

    pub(in crate::fixups) fn all_calls(&self) -> impl Iterator<Item = &CallRecord> {
        self.calls.values().flatten()
    }

    pub(in crate::fixups) fn symbol_use_count(&self, name: &str) -> usize {
        self.symbol_uses.get(name).map_or(0, Vec::len)
    }

    pub(in crate::fixups) fn call_arg_types(&self, call: &CallRecord) -> Vec<Option<Type>> {
        let Some(function) = self.facts.function_by_item_index(call.site.item_index) else {
            return vec![None; call.args.len()];
        };
        call.args
            .iter()
            .map(|arg| {
                self.call_arg_fact(function, &arg.path)
                    .and_then(|(_, arg_fact)| arg_fact.declared_ty.clone())
            })
            .collect()
    }

    pub(in crate::fixups) fn local_value(
        &self,
        window: &StatementRange,
        name: &str,
    ) -> ResolvedValue {
        let mut def_path = window.path.0.clone();
        def_path.push(PathSegment::Stmt(window.start));
        self.resolved_value_at(window.item_index, &AstPath(def_path), name)
    }

    #[expect(
        dead_code,
        reason = "query API surface not yet wired into a fixup rule"
    )]
    pub(in crate::fixups) fn value_local(&self, site: &ValueSite, name: &str) -> ResolvedValue {
        self.resolved_value_at(site.item_index, &site.path, name)
    }

    pub(in crate::fixups) fn binding_at(
        &self,
        item_index: usize,
        definition: &AstPath,
        name: &str,
    ) -> QueryResult<BindingRef> {
        let predicate = Predicate::Binding;
        let site = expression_site(item_index, &definition.0);
        let function = self
            .facts
            .function_by_item_index(item_index)
            .ok_or_else(|| {
                Rejection::new(
                    predicate,
                    Some(site.clone()),
                    RejectionReason::MissingEvidence,
                    Vec::new(),
                )
            })?;
        let id = self
            .facts
            .binding_by_local_path(function, name, definition)
            .ok_or_else(|| {
                Rejection::new(
                    predicate,
                    Some(site.clone()),
                    RejectionReason::MissingEvidence,
                    Vec::new(),
                )
            })?;
        let evidence = vec![Evidence {
            predicate,
            site,
            detail: EvidenceDetail::Binding {
                name: name.to_string(),
            },
        }];
        Ok(Proof::new(
            BindingRef {
                item_index,
                function_name: self
                    .facts
                    .function_name(function)
                    .unwrap_or_default()
                    .to_string(),
                name: name.to_string(),
                definition: definition.clone(),
                kind: BindingCategory::Local,
                ty: self.facts.binding_type_ast(id).cloned(),
                id,
            },
            evidence,
        ))
    }

    pub(in crate::fixups) fn binding_def_use(
        &self,
        binding: &BindingRef,
    ) -> QueryResult<BindingDefUse> {
        let predicate = Predicate::DefUse;
        let site = expression_site(binding.item_index, &binding.definition.0);
        let function = self.facts.function_by_item_index(binding.item_index);
        let fact = function
            .and_then(|function| self.def_use_fact(function, binding.id))
            .ok_or_else(|| {
                Rejection::new(
                    predicate,
                    Some(site.clone()),
                    RejectionReason::MissingEvidence,
                    Vec::new(),
                )
            })?;
        let reads = fact
            .reads
            .iter()
            .map(|path| self.use_site(binding.item_index, path))
            .collect::<Option<Vec<_>>>()
            .ok_or_else(|| {
                Rejection::new(
                    predicate,
                    Some(site.clone()),
                    RejectionReason::MissingEvidence,
                    Vec::new(),
                )
            })?;
        let writes = fact
            .writes
            .iter()
            .map(|path| self.use_site(binding.item_index, path))
            .collect::<Option<Vec<_>>>()
            .ok_or_else(|| {
                Rejection::new(
                    predicate,
                    Some(site.clone()),
                    RejectionReason::MissingEvidence,
                    Vec::new(),
                )
            })?;
        let evidence = vec![Evidence {
            predicate,
            site,
            detail: EvidenceDetail::DefUse {
                reads: reads.len(),
                writes: writes.len(),
            },
        }];
        Ok(Proof::new(
            BindingDefUse {
                binding: binding.clone(),
                reads,
                writes,
            },
            evidence,
        ))
    }

    pub(in crate::fixups) fn binding_initializer(
        &self,
        binding: &BindingRef,
    ) -> QueryResult<ExpressionRef> {
        let statement = StatementRef {
            item_index: binding.item_index,
            path: binding.definition.clone(),
        };
        let (initializer, evidence) = self.statement_initializer(&statement)?.into_parts();
        let Some(initializer) = initializer else {
            return Err(Rejection::new(
                Predicate::Expression,
                Some(statement_evidence_site(&statement)),
                RejectionReason::MissingEvidence,
                evidence,
            ));
        };
        Ok(Proof::new(initializer, evidence))
    }

    pub(in crate::fixups) fn enclosing_statement(
        &self,
        expression: &ExpressionRef,
    ) -> QueryResult<StatementRef> {
        let Some(index) = expression
            .site
            .path
            .0
            .iter()
            .rposition(|segment| matches!(segment, PathSegment::Stmt(_)))
        else {
            return Err(Rejection::new(
                Predicate::Statement,
                Some(expression.site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let statement = StatementRef {
            item_index: expression.site.item_index,
            path: AstPath(expression.site.path.0[..=index].to_vec()),
        };
        let proof = self.statement(&statement)?;
        Ok(Proof::new(statement, proof.evidence))
    }

    pub(in crate::fixups) fn enclosing_statements(
        &self,
        statement: &StatementRef,
    ) -> QueryResult<&'snapshot [IndentStmt]> {
        let predicate = Predicate::StatementRegion;
        let Some(container) = statement.container() else {
            return Err(Rejection::new(
                predicate,
                Some(statement_evidence_site(statement)),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let body = self.statement_container(&container).ok_or_else(|| {
            Rejection::new(
                predicate,
                Some(statement_evidence_site(statement)),
                RejectionReason::MissingEvidence,
                Vec::new(),
            )
        })?;
        Ok(Proof::new(
            body,
            vec![Evidence {
                predicate,
                site: statement_evidence_site(statement),
                detail: EvidenceDetail::StatementRegion {
                    statements: body.len(),
                },
            }],
        ))
    }

    pub(in crate::fixups) fn statement_reachable(
        &self,
        statement: &StatementRef,
    ) -> QueryResult<bool> {
        let predicate = Predicate::StatementReachable;
        let site = statement_evidence_site(statement);
        let Some(function) = self.facts.function_by_item_index(statement.item_index) else {
            return Err(Rejection::new(
                predicate,
                Some(site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let reachable = self
            .control_flow_fact(function, ControlFlowSubject::Stmt, &statement.path)
            .is_some_and(|fact| fact.reachable);
        Ok(Proof::new(
            reachable,
            vec![Evidence {
                predicate,
                site,
                detail: EvidenceDetail::StatementReachable { reachable },
            }],
        ))
    }

    fn use_site(&self, item_index: usize, path: &AstPath) -> Option<UseSiteRef> {
        if matches!(path.0.last(), Some(PathSegment::Stmt(_))) {
            let statement = StatementRef {
                item_index,
                path: path.clone(),
            };
            return self
                .statement_tail(&statement)
                .is_some()
                .then_some(UseSiteRef::Statement(statement));
        }
        let expression = ExpressionRef {
            site: expression_site(item_index, &path.0),
        };
        if self.expr(&expression.site).is_some() {
            return Some(UseSiteRef::Expression(expression));
        }
        let index = path
            .0
            .iter()
            .rposition(|segment| matches!(segment, PathSegment::Stmt(_)))?;
        let statement = StatementRef {
            item_index,
            path: AstPath(path.0[..=index].to_vec()),
        };
        self.statement_tail(&statement)
            .is_some()
            .then_some(UseSiteRef::Statement(statement))
    }

    pub(in crate::fixups) fn binding_value(&self, binding: &BindingRef) -> ResolvedValue {
        let usage = self
            .facts
            .function_by_item_index(binding.item_index)
            .and_then(|function| self.def_use_fact(function, binding.id))
            .map(|fact| Usage {
                reads: fact.reads.len(),
                writes: fact.writes.len(),
            });
        ResolvedValue {
            ty: binding.ty.clone(),
            usage,
            purity: None,
        }
    }

    pub(in crate::fixups) fn binding_requires_mut(
        &self,
        binding: &BindingRef,
    ) -> QueryResult<bool> {
        let predicate = Predicate::BindingRequiresMut;
        let required = match self.facts.function_by_item_index(binding.item_index) {
            Some(function) => self.binding_requires_mut_fact(function, binding.id),
            None => self.facts.binding_requires_mut(binding.id),
        };
        Ok(Proof::new(
            required,
            vec![Evidence {
                predicate,
                site: expression_site(binding.item_index, &binding.definition.0),
                detail: EvidenceDetail::BindingRequiresMut { required },
            }],
        ))
    }

    pub(in crate::fixups) fn borrow_alias_reasons(
        &self,
        binding: &BindingRef,
    ) -> QueryResult<Option<BTreeSet<BorrowAliasReason>>> {
        let predicate = Predicate::BorrowAliasReasons;
        let reasons = match self.facts.function_by_item_index(binding.item_index) {
            Some(function) => self.borrow_alias_reasons_fact(function, binding.id),
            None => self
                .facts
                .borrow_alias
                .iter()
                .find(|fact| fact.binding == binding.id)
                .map(|fact| fact.reasons.clone()),
        };
        let evidence = vec![Evidence {
            predicate,
            site: expression_site(binding.item_index, &binding.definition.0),
            detail: EvidenceDetail::BorrowAliasReasons {
                tracked: reasons.is_some(),
                reasons: reasons.as_ref().map_or(0, BTreeSet::len),
            },
        }];
        Ok(Proof::new(reasons, evidence))
    }

    pub(in crate::fixups) fn binding_type(&self, binding: &BindingRef) -> QueryResult<Type> {
        let predicate = Predicate::ExpressionType;
        let site = expression_site(binding.item_index, &binding.definition.0);
        let ty = binding.ty.clone().ok_or_else(|| {
            Rejection::new(
                predicate,
                Some(site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            )
        })?;
        Ok(Proof::new(
            ty.clone(),
            vec![Evidence {
                predicate,
                site,
                detail: EvidenceDetail::ExpressionType { ty },
            }],
        ))
    }

    #[expect(
        dead_code,
        reason = "query API surface not yet wired into a fixup rule"
    )]
    pub(in crate::fixups) fn binding_usage(&self, binding: &BindingRef) -> QueryResult<Usage> {
        let proof = self.binding_def_use(binding)?;
        let usage = Usage {
            reads: proof.value.reads.len(),
            writes: proof.value.writes.len(),
        };
        Ok(Proof::new(usage, proof.evidence))
    }

    #[expect(
        dead_code,
        reason = "query API surface not yet wired into a fixup rule"
    )]
    pub(in crate::fixups) fn binding_resolved_value(
        &self,
        binding: &BindingRef,
    ) -> QueryResult<ResolvedValue> {
        let (ty, mut evidence) = self.binding_type(binding)?.into_parts();
        let (usage, usage_evidence) = self.binding_usage(binding)?.into_parts();
        evidence.extend(usage_evidence);
        Ok(Proof::new(
            ResolvedValue {
                ty: Some(ty),
                usage: Some(usage),
                purity: None,
            },
            evidence,
        ))
    }

    fn resolved_value_at(
        &self,
        item_index: usize,
        def_path: &AstPath,
        name: &str,
    ) -> ResolvedValue {
        let Some(function) = self.facts.function_by_item_index(item_index) else {
            return ResolvedValue {
                ty: None,
                usage: None,
                purity: None,
            };
        };
        let binding = self.facts.binding_by_local_path(function, name, def_path);
        let ty = binding.and_then(|binding| self.facts.binding_type_ast(binding).cloned());
        let usage = binding
            .and_then(|binding| self.def_use_fact(function, binding))
            .map(|uses| Usage {
                reads: uses.reads.len(),
                writes: uses.writes.len(),
            });
        let purity = self
            .effect_fact(function, EffectSubject::Expr, def_path)
            .map(|effect| effect.purity);
        ResolvedValue { ty, usage, purity }
    }

    pub(in crate::fixups) fn extern_fn(&self, matcher: &ExternFn) -> QueryResult<()> {
        let predicate = Predicate::ExternFn;
        let mut saw_name = false;
        for (item_index, item) in self.program.items.iter().enumerate() {
            let Item::ExternBlock { decls, .. } = item else {
                continue;
            };
            for decl in decls {
                let ExternDecl::Fn(function) = decl else {
                    continue;
                };
                if !matcher.name.matches(&function.name, &()) {
                    continue;
                }
                saw_name = true;
                let arity = function.params.len();
                let ret = function.ret.clone();
                if !matcher.arity.matches(&arity, &()) || !matcher.returns.matches(&ret, &()) {
                    continue;
                }
                let site = expression_site(item_index, &[]);
                return Ok(Proof::new(
                    (),
                    vec![Evidence {
                        predicate,
                        site,
                        detail: EvidenceDetail::ExternFnDeclaration {
                            name: function.name.clone(),
                            arity,
                            returns_never: matches!(ret, Some(Type::Never)),
                        },
                    }],
                ));
            }
        }
        Err(Rejection::new(
            predicate,
            None,
            if saw_name {
                RejectionReason::Contradicted
            } else {
                RejectionReason::MissingEvidence
            },
            Vec::new(),
        ))
    }

    pub(in crate::fixups) fn all_exprs(&self, item_index: usize) -> QueryResult<Vec<ExprSite>> {
        let predicate = Predicate::AllExprs;
        let evidence_site = expression_site(item_index, &[]);
        let Some(Item::Fn(function)) = self.program.items.get(item_index) else {
            return Err(Rejection::new(
                predicate,
                Some(evidence_site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let mut sites = Vec::new();
        walk::body_exprs_with_path(&function.body, &mut Vec::new(), &mut |_, path| {
            sites.push(expression_site(item_index, path));
        });
        let evidence = vec![Evidence {
            predicate,
            site: evidence_site,
            detail: EvidenceDetail::AllExprs { count: sites.len() },
        }];
        Ok(Proof::new(sites, evidence))
    }

    pub(in crate::fixups) fn expression_sites(&self) -> Vec<ExprSite> {
        self.expression_sites.clone()
    }

    pub(in crate::fixups) fn expression_roles(
        &self,
        expression: &ExpressionRef,
    ) -> BTreeSet<ExpressionRole> {
        self.expression_roles
            .get(&expression.site)
            .cloned()
            .unwrap_or_default()
    }

    pub(in crate::fixups) fn expression_kind(
        &self,
        expression: &ExpressionRef,
    ) -> Option<ExpressionKind> {
        self.expr(&expression.site).map(expression_kind)
    }

    pub(in crate::fixups) fn parent_expression_kind(
        &self,
        expression: &ExpressionRef,
    ) -> Option<ExpressionKind> {
        let parent = self.expression_parents.get(&expression.site)?;
        self.expr(parent).map(expression_kind)
    }

    pub(in crate::fixups) fn ancestor_expression_kinds(
        &self,
        expression: &ExpressionRef,
    ) -> Vec<ExpressionKind> {
        let mut kinds = Vec::new();
        let mut site = &expression.site;
        while let Some(parent) = self.expression_parents.get(site) {
            if let Some(kind) = self.expr(parent).map(expression_kind) {
                kinds.push(kind);
            }
            site = parent;
        }
        kinds
    }

    pub(in crate::fixups) fn expression(&self, site: &ExprSite) -> QueryResult<ExpressionRef> {
        let predicate = Predicate::Expression;
        if self.expr(site).is_none() {
            return Err(Rejection::new(
                predicate,
                Some(site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        }
        Ok(Proof::new(
            ExpressionRef { site: site.clone() },
            vec![Evidence {
                predicate,
                site: site.clone(),
                detail: EvidenceDetail::Expression,
            }],
        ))
    }

    pub(in crate::fixups) fn expression_dependencies(
        &self,
        expression: &ExpressionRef,
    ) -> QueryResult<Vec<ExpressionRef>> {
        let predicate = Predicate::ExpressionDependencies;
        let dependencies = self
            .expression_sites
            .iter()
            .filter(|site| {
                site.item_index == expression.site.item_index
                    && site.path.0.starts_with(&expression.site.path.0)
                    && matches!(self.expr(site), Some(Expr::Var(_) | Expr::Path(_)))
            })
            .cloned()
            .map(|site| ExpressionRef { site })
            .collect::<Vec<_>>();
        Ok(Proof::new(
            dependencies.clone(),
            vec![Evidence {
                predicate,
                site: expression.site.clone(),
                detail: EvidenceDetail::ExpressionDependencies {
                    count: dependencies.len(),
                },
            }],
        ))
    }

    pub(in crate::fixups) fn statement_expression(
        &self,
        statement: &StatementRef,
        index: usize,
    ) -> QueryResult<ExpressionRef> {
        let mut path = statement.path.0.clone();
        path.push(PathSegment::Expr(index));
        self.expression(&expression_site(statement.item_index, &path))
    }

    pub(in crate::fixups) fn statement_initializer(
        &self,
        statement: &StatementRef,
    ) -> QueryResult<Option<ExpressionRef>> {
        let mut statement_proof = self.statement(statement)?;
        let Stmt::Let { init, .. } = &statement_proof.value.stmt else {
            return Err(Rejection::new(
                Predicate::Expression,
                Some(statement_evidence_site(statement)),
                RejectionReason::UnsupportedShape,
                statement_proof.evidence,
            ));
        };
        if init.is_none() {
            return Ok(Proof::new(None, statement_proof.evidence));
        }
        let mut initializer = self.statement_expression(statement, 0)?;
        statement_proof.evidence.append(&mut initializer.evidence);
        Ok(Proof::new(
            Some(initializer.value),
            statement_proof.evidence,
        ))
    }

    pub(in crate::fixups) fn statement_is_movable_declaration(
        &self,
        statement: &StatementRef,
    ) -> QueryResult<bool> {
        let (indent, mut evidence) = self.statement(statement)?.into_parts();
        if !matches!(indent.stmt, Stmt::Let { .. }) {
            return Ok(Proof::new(false, evidence));
        }
        let (initializer, initializer_evidence) =
            self.statement_initializer(statement)?.into_parts();
        evidence.extend(initializer_evidence);
        let Some(initializer) = initializer else {
            return Ok(Proof::new(true, evidence));
        };
        let (effects, effect_evidence) = self.expression_effects(&initializer)?.into_parts();
        evidence.extend(effect_evidence);
        Ok(Proof::new(effects.purity == Purity::MovablePure, evidence))
    }

    pub(in crate::fixups) fn declaration_uses_in_statement(
        &self,
        declaration: &StatementRef,
        statement: &StatementRef,
    ) -> QueryResult<BindingUses> {
        let (binding, mut evidence) = self.statement_binding(declaration)?.into_parts();
        let (uses, uses_evidence) = self
            .binding_uses_in_statement(&binding, statement)?
            .into_parts();
        evidence.extend(uses_evidence);
        Ok(Proof::new(uses, evidence))
    }

    pub(in crate::fixups) fn statement_in_container(
        &self,
        container: &StatementContainerRef,
        expression: &ExpressionRef,
    ) -> QueryResult<StatementRef> {
        let predicate = Predicate::Statement;
        if container.item_index != expression.site.item_index {
            return Err(Rejection::new(
                predicate,
                Some(expression.site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        }
        let Some([PathSegment::Stmt(index), ..]) = expression
            .site
            .path
            .0
            .strip_prefix(container.path.0.as_slice())
        else {
            return Err(Rejection::new(
                predicate,
                Some(expression.site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let mut path = container.path.0.clone();
        path.push(PathSegment::Stmt(*index));
        let statement = StatementRef {
            item_index: container.item_index,
            path: AstPath(path),
        };
        let proof = self.statement(&statement)?;
        Ok(Proof::new(statement, proof.evidence))
    }

    pub(in crate::fixups) fn statements_between(
        &self,
        start: &StatementRef,
        end: &StatementRef,
    ) -> QueryResult<Vec<StatementRef>> {
        let predicate = Predicate::Statement;
        let Some(start_container) = start.container() else {
            return Err(Rejection::new(
                predicate,
                Some(statement_evidence_site(start)),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let Some(end_container) = end.container() else {
            return Err(Rejection::new(
                predicate,
                Some(statement_evidence_site(end)),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let (Some(start_index), Some(end_index)) = (start.index(), end.index()) else {
            return Err(Rejection::new(
                predicate,
                Some(statement_evidence_site(start)),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        if start_container != end_container || start_index >= end_index {
            return Err(Rejection::new(
                predicate,
                Some(statement_evidence_site(start)),
                RejectionReason::Contradicted,
                Vec::new(),
            ));
        }
        let mut evidence = self.statement(start)?.evidence;
        evidence.extend(self.statement(end)?.evidence);
        let statements = (start_index + 1..end_index)
            .map(|index| {
                let mut path = start_container.path.0.clone();
                path.push(PathSegment::Stmt(index));
                StatementRef {
                    item_index: start.item_index,
                    path: AstPath(path),
                }
            })
            .collect();
        Ok(Proof::new(statements, evidence))
    }

    pub(in crate::fixups) fn following_statements(
        &self,
        statement: &StatementRef,
    ) -> QueryResult<Vec<StatementRef>> {
        let predicate = Predicate::StatementRegion;
        let Some(container) = statement.container() else {
            return Err(Rejection::new(
                predicate,
                Some(statement_evidence_site(statement)),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let Some(index) = statement.index() else {
            return Err(Rejection::new(
                predicate,
                Some(statement_evidence_site(statement)),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let body = self.statement_container(&container).ok_or_else(|| {
            Rejection::new(
                predicate,
                Some(statement_evidence_site(statement)),
                RejectionReason::MissingEvidence,
                Vec::new(),
            )
        })?;
        let statements = (index + 1..body.len())
            .map(|index| {
                let mut path = container.path.0.clone();
                path.push(PathSegment::Stmt(index));
                StatementRef {
                    item_index: statement.item_index,
                    path: AstPath(path),
                }
            })
            .collect::<Vec<_>>();
        let mut evidence = self.statement(statement)?.evidence;
        evidence.push(Evidence {
            predicate,
            site: statement_evidence_site(statement),
            detail: EvidenceDetail::StatementRegion {
                statements: statements.len(),
            },
        });
        Ok(Proof::new(statements, evidence))
    }

    pub(in crate::fixups) fn statement_range(
        &self,
        start: &StatementRef,
        end: &StatementRef,
    ) -> QueryResult<StatementRange> {
        let predicate = Predicate::StatementRegion;
        let (Some(start_container), Some(end_container)) = (start.container(), end.container())
        else {
            return Err(Rejection::new(
                predicate,
                Some(statement_evidence_site(start)),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let (Some(start_index), Some(end_index)) = (start.index(), end.index()) else {
            return Err(Rejection::new(
                predicate,
                Some(statement_evidence_site(start)),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let mut evidence = self.statement(start)?.evidence;
        evidence.extend(self.statement(end)?.evidence);
        if start_container != end_container || start_index > end_index {
            return Err(Rejection::new(
                predicate,
                Some(statement_evidence_site(start)),
                RejectionReason::Contradicted,
                evidence,
            ));
        }
        let statements = end_index - start_index + 1;
        evidence.push(Evidence {
            predicate,
            site: statement_evidence_site(start),
            detail: EvidenceDetail::StatementRegion { statements },
        });
        Ok(Proof::new(
            StatementRange {
                item_index: start.item_index,
                path: start_container.path,
                start: start_index,
                end: end_index + 1,
            },
            evidence,
        ))
    }

    pub(in crate::fixups) fn atomic_compare_exchange_shape(
        &self,
        statements: &[StatementRef; 6],
    ) -> QueryResult<AtomicCompareExchangeChain> {
        let predicate = Predicate::StatementRegion;
        let site = statement_evidence_site(&statements[0]);
        let indices = statements
            .iter()
            .map(StatementRef::index)
            .collect::<Option<Vec<_>>>()
            .ok_or_else(|| {
                Rejection::new(
                    predicate,
                    Some(site.clone()),
                    RejectionReason::MissingEvidence,
                    Vec::new(),
                )
            })?;
        if !indices.windows(2).all(|pair| pair[1] == pair[0] + 1) {
            return Err(Rejection::new(
                predicate,
                Some(site),
                RejectionReason::Contradicted,
                Vec::new(),
            ));
        }

        let (_, mut evidence) = self
            .statement_range(&statements[0], &statements[5])?
            .into_parts();
        let mut body = Vec::with_capacity(statements.len());
        for statement in statements {
            let (indent, mut statement_evidence) = self.statement(statement)?.into_parts();
            body.push(indent.clone());
            evidence.append(&mut statement_evidence);
        }
        let Some(chain) = super::atomic::compare_exchange_chain(&body) else {
            return Err(Rejection::new(
                predicate,
                Some(site),
                RejectionReason::UnsupportedShape,
                evidence,
            ));
        };
        Ok(Proof::new(chain, evidence))
    }

    pub(in crate::fixups) fn statement(
        &self,
        statement: &StatementRef,
    ) -> QueryResult<&'snapshot IndentStmt> {
        let site = statement_evidence_site(statement);
        let value = self
            .statement_tail(statement)
            .and_then(|tail| tail.first())
            .ok_or_else(|| {
                Rejection::new(
                    Predicate::Statement,
                    Some(site.clone()),
                    RejectionReason::MissingEvidence,
                    Vec::new(),
                )
            })?;
        Ok(Proof::new(
            value,
            vec![Evidence {
                predicate: Predicate::Statement,
                site,
                detail: EvidenceDetail::Statement,
            }],
        ))
    }

    pub(in crate::fixups) fn statement_binding(
        &self,
        statement: &StatementRef,
    ) -> QueryResult<BindingRef> {
        let proof = self.statement(statement)?;
        let name = match &proof.value.stmt {
            Stmt::Let { name, .. } | Stmt::LetIf { name, .. } => name,
            _ => {
                return Err(Rejection::new(
                    Predicate::Binding,
                    Some(statement_evidence_site(statement)),
                    RejectionReason::UnsupportedShape,
                    proof.evidence,
                ));
            }
        };
        let mut binding = self.binding_at(statement.item_index, &statement.path, name)?;
        let mut evidence = proof.evidence;
        evidence.append(&mut binding.evidence);
        Ok(Proof::new(binding.value, evidence))
    }

    pub(in crate::fixups) fn statement_effects(
        &self,
        statement: &StatementRef,
    ) -> QueryResult<ExpressionEffects> {
        let predicate = Predicate::ExpressionEffects;
        let site = statement_evidence_site(statement);
        let function = self
            .facts
            .function_by_item_index(statement.item_index)
            .ok_or_else(|| {
                Rejection::new(
                    predicate,
                    Some(site.clone()),
                    RejectionReason::MissingEvidence,
                    Vec::new(),
                )
            })?;
        let fact = self
            .effect_fact(function, EffectSubject::Expr, &statement.path)
            .ok_or_else(|| {
                Rejection::new(
                    predicate,
                    Some(site.clone()),
                    RejectionReason::MissingEvidence,
                    Vec::new(),
                )
            })?;
        Ok(Proof::new(
            ExpressionEffects {
                purity: fact.purity,
                effects: fact.effects.clone(),
            },
            vec![Evidence {
                predicate,
                site,
                detail: EvidenceDetail::ExpressionEffects {
                    purity: fact.purity,
                    effects: fact.effects.len(),
                },
            }],
        ))
    }

    pub(in crate::fixups) fn observed_statement_effects(
        &self,
        statement: &StatementRef,
    ) -> QueryResult<Option<ExpressionEffects>> {
        let Some(function) = self.facts.function_by_item_index(statement.item_index) else {
            return Ok(Proof::new(None, Vec::new()));
        };
        let Some(fact) = self.effect_fact(function, EffectSubject::Stmt, &statement.path) else {
            return Ok(Proof::new(None, Vec::new()));
        };
        let effects = ExpressionEffects {
            purity: fact.purity,
            effects: fact.effects.clone(),
        };
        Ok(Proof::new(
            Some(effects),
            vec![Evidence {
                predicate: Predicate::ExpressionEffects,
                site: statement_evidence_site(statement),
                detail: EvidenceDetail::ExpressionEffects {
                    purity: fact.purity,
                    effects: fact.effects.len(),
                },
            }],
        ))
    }

    pub(in crate::fixups) fn parent_expression(
        &self,
        expression: &ExpressionRef,
    ) -> QueryResult<ExpressionRef> {
        let predicate = Predicate::ParentExpression;
        let parent = self
            .expression_parents
            .get(&expression.site)
            .cloned()
            .ok_or_else(|| {
                Rejection::new(
                    predicate,
                    Some(expression.site.clone()),
                    RejectionReason::MissingEvidence,
                    Vec::new(),
                )
            })?;
        Ok(Proof::new(
            ExpressionRef {
                site: parent.clone(),
            },
            vec![Evidence {
                predicate,
                site: parent,
                detail: EvidenceDetail::ParentExpression,
            }],
        ))
    }

    pub(in crate::fixups) fn argument_position(
        &self,
        expression: &ExpressionRef,
    ) -> QueryResult<(ExpressionRef, usize)> {
        let predicate = Predicate::ArgumentPosition;
        let parent = self.parent_expression(expression)?;
        if !matches!(self.expr(&parent.value.site), Some(Expr::Call { .. })) {
            return Err(Rejection::new(
                predicate,
                Some(expression.site.clone()),
                RejectionReason::UnsupportedShape,
                parent.evidence.clone(),
            ));
        }
        let Some(slot) = expression
            .site
            .path
            .0
            .last()
            .and_then(|segment| match segment {
                PathSegment::Expr(index) => index.checked_sub(1),
                _ => None,
            })
        else {
            return Err(Rejection::new(
                predicate,
                Some(expression.site.clone()),
                RejectionReason::UnsupportedShape,
                parent.evidence.clone(),
            ));
        };
        let mut evidence = parent.evidence.clone();
        evidence.push(Evidence {
            predicate,
            site: expression.site.clone(),
            detail: EvidenceDetail::ArgumentPosition { slot },
        });
        Ok(Proof::new((parent.value, slot), evidence))
    }

    #[expect(
        dead_code,
        reason = "query API surface not yet wired into a fixup rule"
    )]
    pub(in crate::fixups) fn ancestor_expressions(
        &self,
        expression: &ExpressionRef,
    ) -> QueryResult<Vec<ExpressionRef>> {
        let mut ancestors = Vec::new();
        let mut evidence = Vec::new();
        let mut current = expression.clone();
        while self.expression_parents.contains_key(&current.site) {
            let proof = self.parent_expression(&current)?;
            evidence.extend(proof.evidence);
            current = proof.value;
            ancestors.push(current.clone());
        }
        Ok(Proof::new(ancestors, evidence))
    }

    pub(in crate::fixups) fn expression_call(
        &self,
        expression: &ExpressionRef,
    ) -> QueryResult<CallRecord> {
        let predicate = Predicate::Call;
        let call = self.calls_by_site.get(&expression.site).ok_or_else(|| {
            Rejection::new(
                predicate,
                Some(expression.site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            )
        })?;
        Ok(Proof::new(call.clone(), call.evidence.clone()))
    }

    pub(in crate::fixups) fn expression_values(
        &self,
        expression: &ExpressionRef,
    ) -> QueryResult<ExpressionValues> {
        let predicate = Predicate::ExpressionValues;
        let function = self.function(&expression.site).ok_or_else(|| {
            Rejection::new(
                predicate,
                Some(expression.site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            )
        })?;
        let values = self.values_at(function, ValueSubject::Expr, &expression.site.fact_path);
        if values.is_empty() {
            return Err(Rejection::new(
                predicate,
                Some(expression.site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        }
        Ok(Proof::new(
            ExpressionValues {
                values: values.clone(),
            },
            vec![Evidence {
                predicate,
                site: expression.site.clone(),
                detail: EvidenceDetail::ExpressionValues {
                    count: values.len(),
                },
            }],
        ))
    }

    pub(in crate::fixups) fn expression_effects(
        &self,
        expression: &ExpressionRef,
    ) -> QueryResult<ExpressionEffects> {
        let predicate = Predicate::ExpressionEffects;
        let function = self.function(&expression.site).ok_or_else(|| {
            Rejection::new(
                predicate,
                Some(expression.site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            )
        })?;
        let fact = self
            .effect_fact(function, EffectSubject::Expr, &expression.site.path)
            .or_else(|| self.effect_fact(function, EffectSubject::Expr, &expression.site.fact_path))
            .ok_or_else(|| {
                Rejection::new(
                    predicate,
                    Some(expression.site.clone()),
                    RejectionReason::MissingEvidence,
                    Vec::new(),
                )
            })?;
        Ok(Proof::new(
            ExpressionEffects {
                purity: fact.purity,
                effects: fact.effects.clone(),
            },
            vec![Evidence {
                predicate,
                site: expression.site.clone(),
                detail: EvidenceDetail::ExpressionEffects {
                    purity: fact.purity,
                    effects: fact.effects.len(),
                },
            }],
        ))
    }

    pub(in crate::fixups) fn expression_place(
        &self,
        expression: &ExpressionRef,
    ) -> QueryResult<ExpressionPlace> {
        let predicate = Predicate::ExpressionPlace;
        let function = self.function(&expression.site).ok_or_else(|| {
            Rejection::new(
                predicate,
                Some(expression.site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            )
        })?;
        let fact = self
            .place_fact(function, &expression.site.fact_path)
            .ok_or_else(|| {
                Rejection::new(
                    predicate,
                    Some(expression.site.clone()),
                    RejectionReason::MissingEvidence,
                    Vec::new(),
                )
            })?;
        Ok(Proof::new(
            ExpressionPlace {
                access: fact.access,
                kind: fact.kind.clone(),
                readable: fact.readable,
                assignable: fact.assignable,
                ordinary_slot: fact.ordinary_slot,
            },
            vec![Evidence {
                predicate,
                site: expression.site.clone(),
                detail: EvidenceDetail::ExpressionPlace {
                    access: fact.access,
                    ordinary_slot: fact.ordinary_slot,
                },
            }],
        ))
    }

    #[expect(
        dead_code,
        reason = "query API surface not yet wired into a fixup rule"
    )]
    pub(in crate::fixups) fn expression_type(
        &self,
        expression: &ExpressionRef,
    ) -> QueryResult<Type> {
        let predicate = Predicate::ExpressionType;
        let expr = self.expr(&expression.site).ok_or_else(|| {
            Rejection::new(
                predicate,
                Some(expression.site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            )
        })?;
        let mut evidence = Vec::new();
        let ty = match expr {
            Expr::Cast { ty, .. } => Some(ty.clone()),
            Expr::Value(value) => rust_value_type(value),
            Expr::Str(_) => Some(Type::Ref {
                mutable: false,
                inner: Box::new(Type::Str),
            }),
            Expr::ByteStr(_) => Some(Type::Ref {
                mutable: false,
                inner: Box::new(Type::Slice(Box::new(Type::Prim(Prim::U8)))),
            }),
            Expr::CStr(_) => Some(Type::Ref {
                mutable: false,
                inner: Box::new(Type::Custom("core::ffi::CStr".into())),
            }),
            Expr::Var(_) => {
                let proof = self.expression_binding(expression)?;
                evidence.extend(proof.evidence);
                proof.value.ty
            }
            Expr::Ref { mutable, .. } => {
                let proof = self.expression_type(&ExpressionRef {
                    site: child_site(&expression.site, 0),
                })?;
                evidence.extend(proof.evidence);
                Some(Type::Ref {
                    mutable: *mutable,
                    inner: Box::new(proof.value),
                })
            }
            Expr::AddrOf { mutable, .. } => {
                let proof = self.expression_type(&ExpressionRef {
                    site: child_site(&expression.site, 0),
                })?;
                evidence.extend(proof.evidence);
                Some(Type::Ptr {
                    mutable: *mutable,
                    inner: Box::new(proof.value),
                })
            }
            Expr::Unary { .. } => {
                let proof = self.expression_type(&ExpressionRef {
                    site: child_site(&expression.site, 0),
                })?;
                evidence.extend(proof.evidence);
                Some(proof.value)
            }
            Expr::Binary { .. } => {
                let left = self.expression_type(&ExpressionRef {
                    site: child_site(&expression.site, 0),
                })?;
                let right = self.expression_type(&ExpressionRef {
                    site: child_site(&expression.site, 1),
                })?;
                evidence.extend(left.evidence);
                evidence.extend(right.evidence);
                (left.value == right.value).then_some(left.value)
            }
            Expr::Call { .. } => {
                let function = self.function(&expression.site);
                function
                    .and_then(|function| {
                        self.facts
                            .callsite(function, &expression.site.fact_path)
                            .or_else(|| self.facts.callsite(function, &expression.site.path))
                    })
                    .and_then(|call| call.ret.clone())
            }
            _ => None,
        }
        .ok_or_else(|| {
            Rejection::new(
                predicate,
                Some(expression.site.clone()),
                RejectionReason::MissingEvidence,
                evidence.clone(),
            )
        })?;
        evidence.push(Evidence {
            predicate,
            site: expression.site.clone(),
            detail: EvidenceDetail::ExpressionType { ty: ty.clone() },
        });
        Ok(Proof::new(ty, evidence))
    }

    pub(in crate::fixups) fn expression_binding(
        &self,
        expression: &ExpressionRef,
    ) -> QueryResult<BindingRef> {
        let predicate = Predicate::Binding;
        let Some(Expr::Var(name)) = self.expr(&expression.site) else {
            return Err(Rejection::new(
                predicate,
                Some(expression.site.clone()),
                RejectionReason::UnsupportedShape,
                Vec::new(),
            ));
        };
        let function = self.function(&expression.site).ok_or_else(|| {
            Rejection::new(
                predicate,
                Some(expression.site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            )
        })?;
        let mut candidates =
            self.facts
                .bindings_read_under(function, name.as_str(), &expression.site.fact_path);
        candidates.extend(self.facts.bindings_written_under(
            function,
            name.as_str(),
            &expression.site.fact_path,
        ));
        candidates.sort();
        candidates.dedup();
        if candidates.len() != 1 {
            return Err(Rejection::new(
                predicate,
                Some(expression.site.clone()),
                if candidates.is_empty() {
                    RejectionReason::MissingEvidence
                } else {
                    RejectionReason::Ambiguous
                },
                Vec::new(),
            ));
        }
        let binding = self
            .all_bindings()
            .into_iter()
            .find(|binding| binding.id == candidates[0])
            .ok_or_else(|| {
                Rejection::new(
                    predicate,
                    Some(expression.site.clone()),
                    RejectionReason::MissingEvidence,
                    Vec::new(),
                )
            })?;
        Ok(Proof::new(
            binding,
            vec![Evidence {
                predicate,
                site: expression.site.clone(),
                detail: EvidenceDetail::Binding {
                    name: name.to_string(),
                },
            }],
        ))
    }

    pub(in crate::fixups) fn binding_uses(&self, binding: &BindingRef) -> QueryResult<BindingUses> {
        let predicate = Predicate::BindingUses;
        let function = self
            .facts
            .function_by_item_index(binding.item_index)
            .ok_or_else(|| {
                Rejection::new(
                    predicate,
                    Some(expression_site(binding.item_index, &binding.definition.0)),
                    RejectionReason::MissingEvidence,
                    Vec::new(),
                )
            })?;
        let mut uses = Vec::new();
        for site in self
            .expression_sites()
            .into_iter()
            .filter(|site| site.item_index == binding.item_index)
        {
            let Some(Expr::Var(name)) = self.expr(&site) else {
                continue;
            };
            if name.as_str() != binding.name {
                continue;
            }
            let reads = self
                .facts
                .bindings_read_under(function, name.as_str(), &site.fact_path);
            let writes =
                self.facts
                    .bindings_written_under(function, name.as_str(), &site.fact_path);
            let read = reads.as_slice() == [binding.id];
            let write = writes.as_slice() == [binding.id];
            let access = match (read, write) {
                (true, true) => Some(BindingAccess::ReadWrite),
                (true, false) => Some(BindingAccess::Read),
                (false, true) => Some(BindingAccess::Write),
                (false, false) => None,
            };
            if let Some(access) = access {
                uses.push(BindingUse {
                    site: UseSiteRef::Expression(ExpressionRef { site }),
                    access,
                });
            }
        }
        if let Some(def_use) = self.def_use_fact(function, binding.id) {
            for read in &def_use.reads {
                let covered = uses.iter().any(|usage| {
                    matches!(usage.access, BindingAccess::Read | BindingAccess::ReadWrite)
                        && usage.expression().is_some_and(|expression| {
                            walk::paths_overlap(
                                &read.0,
                                &coarse_def_use_path(&expression.site.fact_path).0,
                            )
                        })
                });
                if !covered && let Some(site) = self.use_site(binding.item_index, read) {
                    merge_binding_use(&mut uses, site, BindingAccess::Read);
                }
            }
            for write in &def_use.writes {
                let covered = uses.iter().any(|usage| {
                    matches!(
                        usage.access,
                        BindingAccess::Write | BindingAccess::ReadWrite
                    ) && usage.expression().is_some_and(|expression| {
                        walk::paths_overlap(
                            &write.0,
                            &coarse_def_use_path(&expression.site.fact_path).0,
                        )
                    })
                });
                if !covered && let Some(site) = self.use_site(binding.item_index, write) {
                    merge_binding_use(&mut uses, site, BindingAccess::Write);
                }
            }
        }
        let reads = uses
            .iter()
            .filter(|usage| matches!(usage.access, BindingAccess::Read | BindingAccess::ReadWrite))
            .count();
        let writes = uses
            .iter()
            .filter(|usage| {
                matches!(
                    usage.access,
                    BindingAccess::Write | BindingAccess::ReadWrite
                )
            })
            .count();
        Ok(Proof::new(
            BindingUses {
                binding: binding.clone(),
                uses,
            },
            vec![Evidence {
                predicate,
                site: expression_site(binding.item_index, &binding.definition.0),
                detail: EvidenceDetail::BindingUses { reads, writes },
            }],
        ))
    }

    pub(in crate::fixups) fn binding_uses_in_statement(
        &self,
        binding: &BindingRef,
        statement: &StatementRef,
    ) -> QueryResult<BindingUses> {
        let (mut uses, mut evidence) = self.binding_uses(binding)?.into_parts();
        uses.uses.retain(|usage| match &usage.site {
            UseSiteRef::Expression(expression) => {
                expression.site.path.0.starts_with(&statement.path.0)
            }
            UseSiteRef::Statement(use_statement) => {
                use_statement.path.0.starts_with(&statement.path.0)
            }
        });
        let reads = uses
            .uses
            .iter()
            .filter(|usage| matches!(usage.access, BindingAccess::Read | BindingAccess::ReadWrite))
            .count();
        let writes = uses
            .uses
            .iter()
            .filter(|usage| {
                matches!(
                    usage.access,
                    BindingAccess::Write | BindingAccess::ReadWrite
                )
            })
            .count();
        evidence.push(Evidence {
            predicate: Predicate::BindingUses,
            site: statement_evidence_site(statement),
            detail: EvidenceDetail::BindingUses { reads, writes },
        });
        Ok(Proof::new(uses, evidence))
    }

    pub(in crate::fixups) fn binding_uses_in_expression(
        &self,
        binding: &BindingRef,
        expression: &ExpressionRef,
    ) -> QueryResult<BindingUses> {
        let (mut uses, mut evidence) = self.binding_uses(binding)?.into_parts();
        uses.uses.retain(|usage| match &usage.site {
            UseSiteRef::Expression(use_expression) => use_expression
                .site
                .path
                .0
                .starts_with(&expression.site.path.0),
            UseSiteRef::Statement(statement) => {
                walk::paths_overlap(&statement.path.0, &expression.site.fact_path.0)
            }
        });
        let reads = uses
            .uses
            .iter()
            .filter(|usage| matches!(usage.access, BindingAccess::Read | BindingAccess::ReadWrite))
            .count();
        let writes = uses
            .uses
            .iter()
            .filter(|usage| {
                matches!(
                    usage.access,
                    BindingAccess::Write | BindingAccess::ReadWrite
                )
            })
            .count();
        evidence.push(Evidence {
            predicate: Predicate::BindingUses,
            site: expression.site.clone(),
            detail: EvidenceDetail::BindingUses { reads, writes },
        });
        Ok(Proof::new(uses, evidence))
    }

    pub(in crate::fixups) fn assign_value_sites(&self) -> &BTreeSet<ExprSite> {
        &self.assignment_values
    }

    /// The recorded from/to types of the cast Clang's CIR observed at `site`,
    /// independent of any particular rewrite's shape - callers do their own
    /// structural matching before consulting this.
    pub(in crate::fixups) fn cast_at(&self, site: &ExprSite) -> QueryResult<CastFact> {
        let predicate = Predicate::Cast;
        let Some(function) = self.facts.function_by_item_index(site.item_index) else {
            return Err(Rejection::new(
                predicate,
                Some(site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let Some(fact) = self.cast_fact_at(function, &site.path) else {
            return Err(Rejection::new(
                predicate,
                Some(site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let evidence = vec![Evidence {
            predicate,
            site: site.clone(),
            detail: EvidenceDetail::Cast {
                to: fact.to.clone(),
            },
        }];
        Ok(Proof::new(fact.clone(), evidence))
    }

    pub(in crate::fixups) fn null_check_dominates(
        &self,
        binding: &BindingRef,
        deref_site: &ExprSite,
    ) -> QueryResult<NullCheckDominance> {
        let predicate = Predicate::NullCheckDominance;
        let Some(function) = self.facts.function_by_item_index(deref_site.item_index) else {
            return Err(Rejection::new(
                predicate,
                Some(deref_site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let Some(fact) = self.null_check_dominance_at(function, &deref_site.path) else {
            return Err(Rejection::new(
                predicate,
                Some(deref_site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let matches_binding = self
            .facts
            .binding_name(fact.binding)
            .is_some_and(|name| name == binding.name);
        if !matches_binding {
            return Err(Rejection::new(
                predicate,
                Some(deref_site.clone()),
                RejectionReason::Contradicted,
                Vec::new(),
            ));
        }
        let guard_stmt = fact.guard_site.as_ref().map(|site| StatementRef {
            item_index: deref_site.item_index,
            path: site.path.clone(),
        });
        let result = NullCheckDominance {
            proof: fact.proof,
            guard_stmt,
        };
        let evidence = vec![Evidence {
            predicate,
            site: deref_site.clone(),
            detail: EvidenceDetail::NullCheckDominance { proof: fact.proof },
        }];
        Ok(Proof::new(result, evidence))
    }

    pub(in crate::fixups) fn pointer_option_eligible(
        &self,
        binding: &BindingRef,
    ) -> QueryResult<()> {
        let predicate = Predicate::PointerOptionSafety;
        let Some(function) = self.facts.function_by_item_index(binding.item_index) else {
            return Err(Rejection::new(
                predicate,
                Some(expression_site(binding.item_index, &binding.definition.0)),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let Some(resolved) =
            self.facts
                .binding_by_local_path(function, binding.name.as_str(), &binding.definition)
        else {
            return Err(Rejection::new(
                predicate,
                Some(expression_site(binding.item_index, &binding.definition.0)),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let Some(fact) = self.pointer_option_safety_of(function, resolved) else {
            return Err(Rejection::new(
                predicate,
                Some(expression_site(binding.item_index, &binding.definition.0)),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        if !fact.eligible {
            return Err(Rejection::new(
                predicate,
                Some(expression_site(binding.item_index, &binding.definition.0)),
                RejectionReason::Contradicted,
                Vec::new(),
            ));
        }
        let evidence = vec![Evidence {
            predicate,
            site: expression_site(binding.item_index, &binding.definition.0),
            detail: EvidenceDetail::PointerOptionSafety { eligible: true },
        }];
        Ok(Proof::new((), evidence))
    }

    #[expect(
        dead_code,
        reason = "query API surface not yet wired into a fixup rule"
    )]
    pub(in crate::fixups) fn pointer_comparison_kind(
        &self,
        site: &ExprSite,
    ) -> QueryResult<PointerComparisonKind> {
        let predicate = Predicate::PointerComparisonKind;
        let Some(function) = self.facts.function_by_item_index(site.item_index) else {
            return Err(Rejection::new(
                predicate,
                Some(site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let Some(fact) = self.pointer_comparison_at(function, &site.path) else {
            return Err(Rejection::new(
                predicate,
                Some(site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let evidence = vec![Evidence {
            predicate,
            site: site.clone(),
            detail: EvidenceDetail::PointerComparisonKind { kind: fact.kind },
        }];
        Ok(Proof::new(fact.kind, evidence))
    }

    pub(in crate::fixups) fn callsite_at(&self, site: &ExprSite) -> QueryResult<CallCallee> {
        let predicate = Predicate::Callsite;
        let Some(function) = self.facts.function_by_item_index(site.item_index) else {
            return Err(Rejection::new(
                predicate,
                Some(site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let Some(fact) = self
            .callsite_fact(function, &site.fact_path)
            .or_else(|| self.callsite_fact(function, &site.path))
        else {
            return Err(Rejection::new(
                predicate,
                Some(site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let evidence = vec![Evidence {
            predicate,
            site: site.clone(),
            detail: EvidenceDetail::Callsite {
                direct: matches!(fact.callee, CallCallee::Direct { .. }),
            },
        }];
        Ok(Proof::new(fact.callee.clone(), evidence))
    }

    pub(in crate::fixups) fn call_argument_pinning(
        &self,
        site: &ExprSite,
    ) -> QueryResult<(CallArgPinning, bool, Option<Type>)> {
        let predicate = Predicate::CallArgumentPinning;
        let Some(function) = self.facts.function_by_item_index(site.item_index) else {
            return Err(Rejection::new(
                predicate,
                Some(site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let Some((_, arg)) = self
            .call_arg_fact(function, &site.fact_path)
            .or_else(|| self.call_arg_fact(function, &site.path))
        else {
            return Err(Rejection::new(
                predicate,
                Some(site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let evidence = vec![Evidence {
            predicate,
            site: site.clone(),
            detail: EvidenceDetail::CallArgumentPinning {
                pinning: arg.pinning,
                variadic: arg.variadic,
            },
        }];
        Ok(Proof::new(
            (arg.pinning, arg.variadic, arg.declared_ty.clone()),
            evidence,
        ))
    }

    pub(in crate::fixups) fn printf_call_at(&self, site: &ExprSite) -> QueryResult<PrintfCallFact> {
        let predicate = Predicate::PrintfCall;
        let Some(function) = self.facts.function_by_item_index(site.item_index) else {
            return Err(Rejection::new(
                predicate,
                Some(site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let Some(fact) = self
            .printf_call_fact(function, &site.fact_path)
            .or_else(|| self.printf_call_fact(function, &site.path))
        else {
            return Err(Rejection::new(
                predicate,
                Some(site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let evidence = vec![Evidence {
            predicate,
            site: site.clone(),
            detail: EvidenceDetail::PrintfCall {
                args: fact.arg_paths.len(),
                known_format: fact.format.is_some(),
            },
        }];
        Ok(Proof::new(fact, evidence))
    }

    pub(in crate::fixups) fn has_printf_extern(&self) -> bool {
        self.program.items.iter().any(|item| {
            matches!(item, Item::ExternBlock { decls, .. } if decls.iter().any(|decl| {
                matches!(decl, ExternDecl::Fn(f) if f.identity == FunctionIdentity::Known(Known::Printf))
            }))
        })
    }

    fn has_setlocale_extern(&self) -> bool {
        self.program.items.iter().any(|item| {
            matches!(item, Item::ExternBlock { decls, .. } if decls.iter().any(|decl| {
                matches!(decl, ExternDecl::Fn(f) if f.name == "setlocale")
            }))
        })
    }

    pub(in crate::fixups) fn setlocale_calls_stay_c(&self) -> bool {
        if !self.has_setlocale_extern() {
            return true;
        }
        let mut stable = true;
        for item in &self.program.items {
            let Item::Fn(f) = unwrap_cfg(item) else {
                continue;
            };
            let mut locals = BTreeMap::new();
            collect_let_initializers(&f.body, &mut locals);
            walk::body_exprs(&f.body, &mut |expr| {
                if let Expr::Call { func, args, .. } = expr
                    && matches!(func.as_ref(), Expr::Var(name) if name.as_str() == "setlocale")
                    && !args.get(1).is_some_and(|arg| locale_arg_is_c(arg, &locals))
                {
                    stable = false;
                }
            });
        }
        stable
    }

    pub(in crate::fixups) fn c_string_literal(&self, site: &ExprSite) -> QueryResult<Vec<u8>> {
        let predicate = Predicate::CStringLiteral;
        let Some(function) = self.facts.function_by_item_index(site.item_index) else {
            return Err(Rejection::new(
                predicate,
                Some(site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let Some(fact) = self.c_string_literal_fact(function, &site.path) else {
            return Err(Rejection::new(
                predicate,
                Some(site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let evidence = vec![Evidence {
            predicate,
            site: site.clone(),
            detail: EvidenceDetail::CStringLiteral {
                bytes: fact.bytes.len(),
            },
        }];
        Ok(Proof::new(fact.bytes.clone(), evidence))
    }

    pub(in crate::fixups) fn child(&self, site: &ExprSite, index: usize) -> ExprSite {
        child_site(site, index)
    }

    pub(in crate::fixups) fn string_buffer(
        &self,
        site: &ValueSite,
    ) -> QueryResult<StringBufferFact> {
        let predicate = Predicate::StringBuffer;
        let evidence_site = expression_site(site.item_index, &site.path.0);
        let Some(function) = self.facts.function_by_item_index(site.item_index) else {
            return Err(Rejection::new(
                predicate,
                Some(evidence_site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let Some(buffer) = self.string_buffer_fact_at(function, &site.path) else {
            return Err(Rejection::new(
                predicate,
                Some(evidence_site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let evidence = vec![Evidence {
            predicate,
            site: evidence_site,
            detail: EvidenceDetail::StringBuffer {
                bytes: buffer.bytes.as_ref().map_or(0, Vec::len),
            },
        }];
        Ok(Proof::new(buffer.clone(), evidence))
    }

    pub(in crate::fixups) fn value_uses(
        &self,
        site: &ValueSite,
        name: &str,
    ) -> QueryResult<Vec<ExprSite>> {
        let predicate = Predicate::ReadPath;
        let evidence_site = expression_site(site.item_index, &site.path.0);
        let Some(function) = self.facts.function_by_item_index(site.item_index) else {
            return Err(Rejection::new(
                predicate,
                Some(evidence_site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let Some(binding) = self.facts.binding_by_local_path(function, name, &site.path) else {
            return Err(Rejection::new(
                predicate,
                Some(evidence_site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let sites = self
            .facts
            .def_use(binding)
            .map(|uses| {
                uses.reads
                    .iter()
                    .chain(uses.writes.iter())
                    .map(|path| expression_site(site.item_index, &path.0))
                    .collect()
            })
            .unwrap_or_default();
        let evidence = vec![Evidence {
            predicate,
            site: evidence_site,
            detail: EvidenceDetail::Binding {
                name: name.to_string(),
            },
        }];
        Ok(Proof::new(sites, evidence))
    }

    pub(in crate::fixups) fn string_pointer_view_sites(
        &self,
        site: &ValueSite,
        name: &str,
    ) -> QueryResult<Vec<ExprSite>> {
        let predicate = Predicate::ReadPath;
        let evidence_site = expression_site(site.item_index, &site.path.0);
        let Some(function) = self.facts.function_by_item_index(site.item_index) else {
            return Err(Rejection::new(
                predicate,
                Some(evidence_site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let Some(binding) = self.facts.binding_by_local_path(function, name, &site.path) else {
            return Err(Rejection::new(
                predicate,
                Some(evidence_site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let sites = self
            .string_pointer_view_facts(function, binding)
            .into_iter()
            .map(|view| expression_site(site.item_index, &view.site.path.0))
            .collect();
        let evidence = vec![Evidence {
            predicate,
            site: evidence_site,
            detail: EvidenceDetail::Binding {
                name: name.to_string(),
            },
        }];
        Ok(Proof::new(sites, evidence))
    }

    pub(in crate::fixups) fn string_use_allows_lift(
        &self,
        site: &ValueSite,
        name: &str,
        use_site: &ExprSite,
        recovery: StringRecoveryCandidate,
    ) -> QueryResult<bool> {
        let predicate = Predicate::StringUse;
        let Some(function) = self.facts.function_by_item_index(site.item_index) else {
            return Err(Rejection::new(
                predicate,
                Some(use_site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let Some(binding) = self.facts.binding_by_local_path(function, name, &site.path) else {
            return Err(Rejection::new(
                predicate,
                Some(use_site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let liftable = self.liftable_string_bindings_fact(function, recovery);
        let allowed =
            self.string_use_allowed_fact(function, &use_site.path, binding, recovery, &liftable);
        let evidence = vec![Evidence {
            predicate,
            site: use_site.clone(),
            detail: EvidenceDetail::StringUse { allowed },
        }];
        Ok(Proof::new(allowed, evidence))
    }

    pub(in crate::fixups) fn string_copy_rewrite_sites(
        &self,
        site: &ValueSite,
        name: &str,
    ) -> QueryResult<Vec<StringCopySite>> {
        let predicate = Predicate::ReadPath;
        let evidence_site = expression_site(site.item_index, &site.path.0);
        let Some(function) = self.facts.function_by_item_index(site.item_index) else {
            return Err(Rejection::new(
                predicate,
                Some(evidence_site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let Some(binding) = self.facts.binding_by_local_path(function, name, &site.path) else {
            return Err(Rejection::new(
                predicate,
                Some(evidence_site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let bindings = self.all_bindings();
        let mut sites = Vec::new();
        for fact in self
            .string_copy_rewrite_facts(function)
            .into_iter()
            .filter(|fact| fact.dst == binding)
        {
            let resolve = |source: BindingId| bindings.iter().find(|b| b.id == source).cloned();
            let action = match &fact.rewrite {
                StringCopyRewrite::AssignLiteral(text) => {
                    StringCopyAction::AssignLiteral(text.clone())
                }
                StringCopyRewrite::AssignOwned(source) => {
                    StringCopyAction::AssignOwned(resolve(*source).ok_or_else(|| {
                        Rejection::new(
                            predicate,
                            Some(evidence_site.clone()),
                            RejectionReason::IncompleteDomain,
                            Vec::new(),
                        )
                    })?)
                }
                StringCopyRewrite::PushLiteral(text) => StringCopyAction::PushLiteral(text.clone()),
                StringCopyRewrite::PushOwned(source) => {
                    StringCopyAction::PushOwned(resolve(*source).ok_or_else(|| {
                        Rejection::new(
                            predicate,
                            Some(evidence_site.clone()),
                            RejectionReason::IncompleteDomain,
                            Vec::new(),
                        )
                    })?)
                }
            };
            sites.push(StringCopySite {
                statement: StatementRef {
                    item_index: site.item_index,
                    path: fact.site.path.clone(),
                },
                action,
            });
        }
        let evidence = vec![Evidence {
            predicate,
            site: evidence_site,
            detail: EvidenceDetail::Binding {
                name: name.to_string(),
            },
        }];
        Ok(Proof::new(sites, evidence))
    }

    pub(in crate::fixups) fn string_libc_use(&self, site: &ExprSite) -> QueryResult<StringLibcUse> {
        let predicate = Predicate::StringLibcUse;
        let Some(function) = self.facts.function_by_item_index(site.item_index) else {
            return Err(Rejection::new(
                predicate,
                Some(site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let Some(usage) = self.string_libc_use_fact(function, &site.fact_path) else {
            return Err(Rejection::new(
                predicate,
                Some(site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let bindings = self.all_bindings();
        let mut pointer_args = Vec::new();
        for &binding_id in &usage.pointer_args {
            let Some(binding) = bindings.iter().find(|b| b.id == binding_id).cloned() else {
                return Err(Rejection::new(
                    predicate,
                    Some(site.clone()),
                    RejectionReason::IncompleteDomain,
                    Vec::new(),
                ));
            };
            pointer_args.push(binding);
        }
        let evidence = vec![Evidence {
            predicate,
            site: site.clone(),
            detail: EvidenceDetail::StringLibcUse {
                callee: usage.callee,
            },
        }];
        Ok(Proof::new(
            StringLibcUse {
                callee: usage.callee,
                pointer_args,
            },
            evidence,
        ))
    }

    pub(in crate::fixups) fn ascii_numeric_sign(
        &self,
        binding: &BindingRef,
    ) -> QueryResult<AsciiNumericSign> {
        let predicate = Predicate::AsciiNumericSign;
        let site = expression_site(binding.item_index, &binding.definition.0);
        let Some(sign) = self
            .facts
            .ascii_numeric_string(binding.id)
            .map(|fact| fact.sign)
        else {
            return Err(Rejection::new(
                predicate,
                Some(site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let evidence = vec![Evidence {
            predicate,
            site,
            detail: EvidenceDetail::AsciiNumericSign { sign },
        }];
        Ok(Proof::new(sign, evidence))
    }

    pub(in crate::fixups) fn binding_constant_zero(&self, binding: &BindingRef) -> QueryResult<()> {
        let predicate = Predicate::ValueGuard;
        let site = expression_site(binding.item_index, &binding.definition.0);
        let Some(function) = self.facts.function_by_item_index(binding.item_index) else {
            return Err(Rejection::new(
                predicate,
                Some(site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let is_zero = self.value_matches(
            function,
            ValueSubject::Binding(binding.id),
            ConstValue::Zero,
        );
        if !is_zero {
            return Err(Rejection::new(
                predicate,
                Some(site),
                RejectionReason::Contradicted,
                Vec::new(),
            ));
        }
        let evidence = vec![Evidence {
            predicate,
            site,
            detail: EvidenceDetail::ValueGuard,
        }];
        Ok(Proof::new((), evidence))
    }

    pub(in crate::fixups) fn pointer_origin(
        &self,
        site: &ValueSite,
        name: &str,
    ) -> QueryResult<ArrayElementPointerOrigin> {
        let predicate = Predicate::ArrayElementPointerOrigin;
        let evidence_site = expression_site(site.item_index, &site.path.0);
        let Some(function) = self.facts.function_by_item_index(site.item_index) else {
            return Err(Rejection::new(
                predicate,
                Some(evidence_site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let mut origins = self
            .array_element_pointer_origin_facts(function)
            .into_iter()
            .filter_map(|fact| {
                Some((
                    self.facts.binding_name(fact.pointer)?.to_string(),
                    ArrayElementPointerOrigin {
                        pointer_name: self.facts.binding_name(fact.pointer)?.to_string(),
                        base_name: self.facts.binding_name(fact.base)?.to_string(),
                        index: fact.index.clone(),
                    },
                ))
            })
            .collect::<BTreeMap<_, _>>();
        let Item::Fn(function_item) = &self.program.items[site.item_index] else {
            return Err(Rejection::new(
                predicate,
                Some(evidence_site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        collect_array_element_pointer_aliases(&function_item.body, &mut origins);
        let Some(origin) = origins.remove(name) else {
            return Err(Rejection::new(
                predicate,
                Some(evidence_site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let evidence = vec![Evidence {
            predicate,
            site: evidence_site,
            detail: EvidenceDetail::ArrayElementPointerOrigin { origins: 1 },
        }];
        Ok(Proof::new(origin, evidence))
    }

    pub(in crate::fixups) fn has_anonymous_structs(&self) -> bool {
        !self.anonymous_struct_facts().is_empty()
    }

    pub(in crate::fixups) fn has_lazy_singletons(&self) -> bool {
        !self.lazy_init_singleton_facts().is_empty()
    }

    pub(in crate::fixups) fn has_ptr_len_slices(&self) -> bool {
        !self.ptr_len_slice_facts().is_empty()
    }

    pub(in crate::fixups) fn has_sort_search_calls(&self) -> bool {
        self.all_calls().any(|call| {
            matches!(
                call.target,
                CallTarget::Known(Known::Qsort | Known::Bsearch)
            )
        })
    }

    pub(in crate::fixups) fn has_atomic_promotions(&self) -> bool {
        !self.atomic_local_facts().is_empty() || !self.atomic_global_facts().is_empty()
    }

    pub(super) fn snapshot_program(&self) -> &'snapshot Program {
        self.program
    }

    pub(super) fn snapshot_facts(&self) -> &'snapshot FixupFacts {
        self.facts
    }

    pub(in crate::fixups) fn expr(&self, site: &ExprSite) -> Option<&'snapshot Expr> {
        walk::target_expr_at_path(self.program, site.item_index, &site.path)
    }

    pub(in crate::fixups) fn all_definitions(&self) -> impl Iterator<Item = &DefinitionSite> {
        self.definitions.values().flatten()
    }

    pub(in crate::fixups) fn all_functions(&self) -> Vec<FunctionRef> {
        self.facts
            .functions
            .iter()
            .filter_map(|fact| {
                let Item::Fn(function) = unwrap_cfg(self.program.items.get(fact.item_index)?)
                else {
                    return None;
                };
                Some(FunctionRef {
                    item_index: fact.item_index,
                    name: function.name.clone(),
                    id: fact.id,
                })
            })
            .collect()
    }

    pub(in crate::fixups) fn all_bindings(&self) -> Vec<BindingRef> {
        self.facts
            .bindings
            .iter()
            .filter_map(|binding| {
                let item_index = self.facts.function_item_index(binding.function)?;
                Some(BindingRef {
                    item_index,
                    function_name: self
                        .facts
                        .function_name(binding.function)
                        .unwrap_or_default()
                        .to_string(),
                    name: binding.name.clone(),
                    definition: binding.path.clone(),
                    kind: match binding.kind {
                        BindingKind::Param { index } => BindingCategory::Parameter { index },
                        BindingKind::Local => BindingCategory::Local,
                    },
                    ty: self.facts.binding_type_ast(binding.id).cloned(),
                    id: binding.id,
                })
            })
            .collect()
    }

    pub(in crate::fixups) fn function_def(
        &self,
        function: &FunctionRef,
    ) -> Option<&'snapshot FnDef> {
        let Item::Fn(definition) = unwrap_cfg(self.program.items.get(function.item_index)?) else {
            return None;
        };
        (definition.name == function.name).then_some(definition)
    }

    pub(in crate::fixups) fn parameter_function(
        &self,
        parameter: &ParameterRef,
    ) -> QueryResult<FunctionRef> {
        let predicate = Predicate::Function;
        let site = expression_site(
            parameter.binding.item_index,
            &parameter.binding.definition.0,
        );
        if self.parameter_def(parameter).is_none() {
            return Err(Rejection::new(
                predicate,
                Some(site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        }
        let function = self
            .all_functions()
            .into_iter()
            .find(|function| function.item_index == parameter.binding.item_index)
            .ok_or_else(|| {
                Rejection::new(
                    predicate,
                    Some(site.clone()),
                    RejectionReason::MissingEvidence,
                    Vec::new(),
                )
            })?;
        Ok(Proof::new(
            function.clone(),
            vec![Evidence {
                predicate,
                site,
                detail: EvidenceDetail::Function {
                    name: function.name,
                },
            }],
        ))
    }

    pub(in crate::fixups) fn parameter_uses(
        &self,
        parameter: &ParameterRef,
    ) -> QueryResult<BindingUses> {
        if self.parameter_def(parameter).is_none() {
            return Err(Rejection::new(
                Predicate::BindingUses,
                Some(expression_site(
                    parameter.binding.item_index,
                    &parameter.binding.definition.0,
                )),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        }
        self.binding_uses(&parameter.binding)
    }

    pub(in crate::fixups) fn direct_calls(
        &self,
        function: &FunctionRef,
    ) -> QueryResult<Vec<CallRecord>> {
        let (_, mut evidence) = self.function_snapshot(function)?.into_parts();
        let calls = self
            .all_calls()
            .filter(
                |call| matches!(&call.target, CallTarget::Direct(name) if name == &function.name),
            )
            .cloned()
            .collect::<Vec<_>>();
        evidence.extend(calls.iter().flat_map(|call| call.evidence.clone()));
        evidence.push(Evidence {
            predicate: Predicate::DirectCalls,
            site: expression_site(function.item_index, &[]),
            detail: EvidenceDetail::DirectCalls {
                function: function.name.clone(),
                calls: calls.len(),
                references: self.symbol_use_count(&function.name),
            },
        });
        Ok(Proof::new(calls, evidence))
    }

    pub(in crate::fixups) fn function_reachability(
        &self,
        function: &FunctionRef,
    ) -> QueryResult<FunctionReachability> {
        let (definition, mut evidence) = self.function_snapshot(function)?.into_parts();
        let direct_calls = self.direct_calls(function)?;
        evidence.extend(direct_calls.evidence);
        let externally_reachable = definition.name == "main"
            || definition.vis == Visibility::Pub
            || definition.abi.is_some()
            || definition.attrs.iter().any(exporting_attr);
        let address_exposed = self.symbol_use_count(&function.name) != direct_calls.value.len();
        let reachability = FunctionReachability {
            externally_reachable,
            address_exposed,
        };
        evidence.push(Evidence {
            predicate: Predicate::FunctionReachability,
            site: expression_site(function.item_index, &[]),
            detail: EvidenceDetail::FunctionReachability {
                function: function.name.clone(),
                externally_reachable,
                address_exposed,
            },
        });
        Ok(Proof::new(reachability, evidence))
    }

    pub(in crate::fixups) fn function_call_domain(
        &self,
        function: &FunctionRef,
    ) -> QueryResult<FunctionCallDomain> {
        let predicate = Predicate::FunctionCallDomain;
        let site = expression_site(function.item_index, &[]);
        let (reachability, mut evidence) = self.function_reachability(function)?.into_parts();
        if !self.use_domain_complete || reachability.address_exposed {
            return Err(Rejection::new(
                predicate,
                Some(site),
                RejectionReason::IncompleteDomain,
                evidence,
            ));
        }
        if reachability.externally_reachable {
            return Err(Rejection::new(
                predicate,
                Some(site),
                RejectionReason::Contradicted,
                evidence,
            ));
        }
        let (calls, call_evidence) = self.direct_calls(function)?.into_parts();
        evidence.extend(call_evidence);
        evidence.push(Evidence {
            predicate,
            site,
            detail: EvidenceDetail::FunctionCallDomain {
                function: function.name.clone(),
                calls: calls.len(),
            },
        });
        Ok(Proof::new(
            FunctionCallDomain {
                function: function.clone(),
                calls,
            },
            evidence,
        ))
    }

    pub(in crate::fixups) fn call_argument(
        &self,
        call: &CallRecord,
        index: usize,
    ) -> QueryResult<ExpressionRef> {
        let predicate = Predicate::CallArgument;
        let Some(indexed) = self.calls_by_site.get(&call.site) else {
            return Err(Rejection::new(
                predicate,
                Some(call.site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        if indexed != call {
            return Err(Rejection::new(
                predicate,
                Some(call.site.clone()),
                RejectionReason::Contradicted,
                indexed.evidence.clone(),
            ));
        }
        let Some(argument) = call.args.get(index) else {
            return Err(Rejection::new(
                predicate,
                Some(call.site.clone()),
                RejectionReason::OutOfRange,
                call.evidence.clone(),
            ));
        };
        let (expression, mut evidence) = self.expression(argument)?.into_parts();
        evidence.extend(call.evidence.clone());
        evidence.push(Evidence {
            predicate,
            site: argument.clone(),
            detail: EvidenceDetail::CallArgument { index },
        });
        Ok(Proof::new(expression, evidence))
    }

    pub(in crate::fixups) fn definition_function(
        &self,
        definition: &DefinitionSite,
    ) -> QueryResult<FunctionRef> {
        let predicate = Predicate::Function;
        let site = definition_evidence_site(definition);
        let function = self
            .all_functions()
            .into_iter()
            .find(|function| {
                function.item_index == definition.location.item_index()
                    && function.name == definition.name
            })
            .ok_or_else(|| {
                Rejection::new(
                    predicate,
                    Some(site.clone()),
                    RejectionReason::MissingEvidence,
                    Vec::new(),
                )
            })?;
        Ok(Proof::new(
            function.clone(),
            vec![Evidence {
                predicate,
                site,
                detail: EvidenceDetail::Function {
                    name: function.name,
                },
            }],
        ))
    }

    pub(in crate::fixups) fn function_snapshot(
        &self,
        function: &FunctionRef,
    ) -> QueryResult<&'snapshot FnDef> {
        let predicate = Predicate::Function;
        let site = expression_site(function.item_index, &[]);
        let definition = self.function_def(function).ok_or_else(|| {
            Rejection::new(
                predicate,
                Some(site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            )
        })?;
        Ok(Proof::new(
            definition,
            vec![Evidence {
                predicate,
                site,
                detail: EvidenceDetail::Function {
                    name: function.name.clone(),
                },
            }],
        ))
    }

    pub(in crate::fixups) fn dispatch_regions(
        &self,
        function: &FunctionRef,
    ) -> QueryResult<Vec<DispatchRegion>> {
        let (definition, mut evidence) = self.function_snapshot(function)?.into_parts();
        let regions = crate::fixups::facts::goto::recognize_dispatch_loops(&definition.body)
            .into_iter()
            .map(|dispatch| DispatchRegion {
                state_declaration: StatementRef {
                    item_index: function.item_index,
                    path: AstPath(vec![PathSegment::Stmt(dispatch.let_index)]),
                },
                dispatch_loop: StatementRef {
                    item_index: function.item_index,
                    path: AstPath(vec![PathSegment::Stmt(dispatch.loop_index)]),
                },
                depth: definition.body[dispatch.loop_index].depth,
                dispatch,
            })
            .collect::<Vec<_>>();
        evidence.push(Evidence {
            predicate: Predicate::StatementRegion,
            site: expression_site(function.item_index, &[]),
            detail: EvidenceDetail::StatementRegion {
                statements: regions.len() * 2,
            },
        });
        Ok(Proof::new(regions, evidence))
    }

    pub(in crate::fixups) fn switch_dispatch_flat(
        &self,
        statements: &[StatementRef; 3],
    ) -> QueryResult<(SwitchDispatch, StatementRange, usize)> {
        let predicate = Predicate::SwitchDispatch;
        let site = statement_evidence_site(&statements[0]);
        let mut evidence = Vec::new();
        let mut body = Vec::with_capacity(statements.len());
        for statement in statements {
            let (indent, mut statement_evidence) = self.statement(statement)?.into_parts();
            body.push(indent.clone());
            evidence.append(&mut statement_evidence);
        }
        let Some(dispatch) = switch::flat_dispatch(&body).filter(switch::is_eligible) else {
            return Err(Rejection::new(
                predicate,
                Some(site),
                RejectionReason::UnsupportedShape,
                evidence,
            ));
        };
        let mut target = statements[0].range();
        target.end = target.start + body.len();
        let mut depth = body[0].depth;
        if let Some((scope_ref, scope_indent)) = self.enclosing_lone_scope(&target) {
            target = scope_ref.range();
            depth = scope_indent.depth;
        }
        evidence.push(Evidence {
            predicate,
            site,
            detail: EvidenceDetail::SwitchDispatch {
                cases: dispatch.cases.len(),
            },
        });
        Ok(Proof::new((dispatch, target, depth), evidence))
    }

    fn enclosing_lone_scope(
        &self,
        target: &StatementRange,
    ) -> Option<(StatementRef, &'snapshot IndentStmt)> {
        if target.start != 0 {
            return None;
        }
        let (last, prefix) = target.path.0.split_last()?;
        if !matches!(last, PathSegment::ScopeBody) {
            return None;
        }
        let scope_ref = StatementRef {
            item_index: target.item_index,
            path: AstPath(prefix.to_vec()),
        };
        let indent = self.statement_tail(&scope_ref)?.first()?;
        let Stmt::Scope { body } = &indent.stmt else {
            return None;
        };
        (body.len() == target.end - target.start).then_some((scope_ref, indent))
    }

    pub(in crate::fixups) fn va_list_alias(
        &self,
        function: &FunctionRef,
    ) -> QueryResult<VaListAlias> {
        let predicate = Predicate::VaListAlias;
        let site = expression_site(function.item_index, &[]);
        let missing = || {
            Rejection::new(
                predicate,
                Some(site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            )
        };

        let variadic_params: Vec<ParameterRef> = self
            .all_parameters()
            .into_iter()
            .filter(|parameter| {
                parameter.binding.item_index == function.item_index
                    && self
                        .parameter_def(parameter)
                        .is_some_and(|def| matches!(def.ty, Type::Variadic))
            })
            .collect();
        let [param] = variadic_params.as_slice() else {
            return Err(missing());
        };
        let (param_def_use, mut evidence) = self.binding_def_use(&param.binding)?.into_parts();
        let [param_read] = param_def_use.reads.as_slice() else {
            return Err(missing());
        };
        let UseSiteRef::Statement(param_read) = param_read else {
            return Err(missing());
        };
        if !param_def_use.writes.is_empty() {
            return Err(missing());
        }

        let (bindings, binding_evidence) = self.function_bindings(function)?.into_parts();
        evidence.extend(binding_evidence);
        let mut candidates = Vec::new();
        for local in bindings
            .iter()
            .filter(|binding| matches!(binding.kind, BindingCategory::Local))
            .filter(|binding| binding.ty.as_ref() == Some(&Type::VaList))
        {
            let decl_ref = StatementRef {
                item_index: function.item_index,
                path: local.definition.clone(),
            };
            let Some(decl) = self.statement_tail(&decl_ref).and_then(|tail| tail.first()) else {
                continue;
            };
            if !matches!(&decl.stmt, Stmt::Let { init: None, .. }) {
                continue;
            }
            let Ok(local_def_use) = self.binding_def_use(local) else {
                continue;
            };
            let [write] = local_def_use.value.writes.as_slice() else {
                continue;
            };
            let UseSiteRef::Statement(assign_ref) = write else {
                continue;
            };
            if assign_ref != param_read {
                continue;
            }
            let Some(assign) = self
                .statement_tail(assign_ref)
                .and_then(|tail| tail.first())
            else {
                continue;
            };
            let Stmt::Assign { value, .. } = &assign.stmt else {
                continue;
            };
            if !va_list::is_clone_of(value, &param.binding.name) {
                continue;
            }
            if bindings.iter().any(|other| {
                matches!(other.kind, BindingCategory::Parameter { .. })
                    && other.id != param.binding.id
                    && other.name == local.name
            }) {
                continue;
            }
            candidates.push(VaListAlias {
                param_index: param.index,
                local_name: local.name.clone(),
                local_decl: local.definition.clone(),
                clone_assign: assign_ref.path.clone(),
            });
        }

        let [alias] = candidates.as_slice() else {
            return Err(missing());
        };
        evidence.push(Evidence {
            predicate,
            site,
            detail: EvidenceDetail::VaListAlias {
                param_index: alias.param_index,
            },
        });
        Ok(Proof::new(alias.clone(), evidence))
    }

    pub(in crate::fixups) fn function_bindings(
        &self,
        function: &FunctionRef,
    ) -> QueryResult<Vec<BindingRef>> {
        let mut proof = self.function_snapshot(function)?;
        let bindings = self
            .all_bindings()
            .into_iter()
            .filter(|binding| binding.item_index == function.item_index)
            .collect();
        Ok(Proof::new(bindings, std::mem::take(&mut proof.evidence)))
    }

    pub(in crate::fixups) fn function_expressions(
        &self,
        function: &FunctionRef,
    ) -> QueryResult<Vec<ExpressionRef>> {
        let mut proof = self.function_snapshot(function)?;
        let expressions = self
            .expression_sites
            .iter()
            .filter(|site| site.item_index == function.item_index)
            .cloned()
            .map(|site| ExpressionRef { site })
            .collect();
        Ok(Proof::new(expressions, std::mem::take(&mut proof.evidence)))
    }

    pub(in crate::fixups) fn statement_tail(
        &self,
        statement: &StatementRef,
    ) -> Option<&'snapshot [IndentStmt]> {
        let (PathSegment::Stmt(index), container_path) = statement.path.0.split_last()? else {
            return None;
        };
        let Item::Fn(function) = unwrap_cfg(self.program.items.get(statement.item_index)?) else {
            return None;
        };
        statement_container_at(&function.body, container_path)?.get(*index..)
    }

    pub(in crate::fixups) fn all_parameters(&self) -> Vec<ParameterRef> {
        self.all_bindings()
            .into_iter()
            .filter_map(|binding| {
                let BindingCategory::Parameter { index } = binding.kind else {
                    return None;
                };
                Some(ParameterRef { binding, index })
            })
            .collect()
    }

    pub(in crate::fixups) fn parameter_def(
        &self,
        parameter: &ParameterRef,
    ) -> Option<&'snapshot FnParam> {
        let Item::Fn(function) = unwrap_cfg(self.program.items.get(parameter.binding.item_index)?)
        else {
            return None;
        };
        let definition = function.params.get(parameter.index)?;
        (definition.name == parameter.binding.name).then_some(definition)
    }

    pub(in crate::fixups) fn statement_container(
        &self,
        container: &StatementContainerRef,
    ) -> Option<&'snapshot [IndentStmt]> {
        let Item::Fn(function) = unwrap_cfg(self.program.items.get(container.item_index)?) else {
            return None;
        };
        statement_container_at(&function.body, &container.path.0)
    }

    #[expect(
        dead_code,
        reason = "query API surface not yet wired into a fixup rule"
    )]
    pub(in crate::fixups) fn match_arm(&self, arm: &MatchArmRef) -> Option<&'snapshot MatchArm> {
        let statement = self.statement_tail(&arm.statement)?.first()?;
        let Stmt::Match { arms, .. } = &statement.stmt else {
            return None;
        };
        arms.get(arm.index)
    }

    #[expect(
        dead_code,
        reason = "query API surface not yet wired into a fixup rule"
    )]
    pub(in crate::fixups) fn field(
        &self,
        field: &FieldRef,
    ) -> Option<(Option<&'snapshot str>, &'snapshot Type)> {
        match unwrap_cfg(self.program.items.get(field.item_index)?) {
            Item::Record(record) => record
                .fields
                .get(field.index)
                .map(|field| (Some(field.name.as_str()), &field.ty)),
            Item::Struct(definition) => match &definition.fields {
                StructFields::Named(fields) => fields
                    .get(field.index)
                    .map(|(name, ty)| (Some(name.as_str()), ty)),
                StructFields::Tuple(fields) => fields.get(field.index).map(|ty| (None, ty)),
            },
            _ => None,
        }
    }

    #[expect(
        dead_code,
        reason = "query API surface not yet wired into a fixup rule"
    )]
    pub(in crate::fixups) fn enum_variant(
        &self,
        variant: &EnumVariantRef,
    ) -> Option<&'snapshot crate::rust_ast::EnumConst> {
        let Item::Enum(definition) = unwrap_cfg(self.program.items.get(variant.item_index)?) else {
            return None;
        };
        definition.variants.get(variant.index)
    }

    #[expect(
        dead_code,
        reason = "query API surface not yet wired into a fixup rule"
    )]
    pub(in crate::fixups) fn type_use(&self, type_use: &TypeUseRef) -> Option<&'snapshot Type> {
        match type_use {
            TypeUseRef::FunctionReturn(function) => self.function_def(function)?.ret.as_ref(),
            TypeUseRef::Parameter(parameter) => Some(&self.parameter_def(parameter)?.ty),
            TypeUseRef::Field(field) => Some(self.field(field)?.1),
        }
    }

    #[expect(
        dead_code,
        reason = "query API surface not yet wired into a fixup rule"
    )]
    pub(in crate::fixups) fn definitions_in_group(
        &self,
        group: &DefinitionGroup,
    ) -> Vec<DefinitionSite> {
        self.definitions
            .values()
            .flatten()
            .filter(|definition| definition.group.as_ref() == Some(group))
            .cloned()
            .collect()
    }

    fn count_definition_users(
        &self,
        definition: &DefinitionSite,
        definition_items: &BTreeSet<usize>,
    ) -> usize {
        definition
            .symbols
            .iter()
            .map(|symbol| {
                self.symbol_uses
                    .get(symbol)
                    .map(|uses| {
                        uses.iter()
                            .filter(|item_index| !definition_items.contains(item_index))
                            .count()
                    })
                    .unwrap_or(0)
            })
            .sum()
    }

    fn function(&self, site: &ExprSite) -> Option<FunctionId> {
        self.program.items.get(site.item_index)?;
        self.facts.function_by_item_index(site.item_index)
    }

    fn count_matches_source_len(&self, source: &ByteSource<'snapshot>, count: &ExprSite) -> bool {
        let Some(count) = self.expr(count) else {
            return false;
        };
        count_matches_source_len(source, count)
    }

    fn constant_values<T: Ord>(
        &self,
        predicate: Predicate,
        site: &ExprSite,
        convert: impl Fn(&ConstValue) -> Result<Option<T>, RejectionReason>,
    ) -> Result<Vec<T>, Rejection> {
        let function = self.function(site).ok_or_else(|| {
            Rejection::new(
                predicate,
                Some(site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            )
        })?;
        let mut values = BTreeSet::new();
        for value in self
            .facts
            .values_at(function, ValueSubject::Expr, &site.fact_path)
        {
            match convert(value) {
                Ok(Some(value)) => {
                    values.insert(value);
                }
                Ok(None) => {}
                Err(reason) => {
                    return Err(Rejection::new(
                        predicate,
                        Some(site.clone()),
                        reason,
                        Vec::new(),
                    ));
                }
            }
        }
        if values.is_empty() {
            return Err(Rejection::new(
                predicate,
                Some(site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        }
        Ok(values.into_iter().collect())
    }
}

fn statement_container_at<'body>(
    body: &'body [IndentStmt],
    path: &[PathSegment],
) -> Option<&'body [IndentStmt]> {
    let [PathSegment::Stmt(index), rest @ ..] = path else {
        return path.is_empty().then_some(body);
    };
    let stmt = &body.get(*index)?.stmt;
    match (stmt, rest) {
        (
            Stmt::If { then_body, .. } | Stmt::LetIf { then_body, .. },
            [PathSegment::Then, rest @ ..],
        ) => statement_container_at(then_body, rest),
        (
            Stmt::If { else_body, .. } | Stmt::LetIf { else_body, .. },
            [PathSegment::Else, rest @ ..],
        ) => statement_container_at(else_body, rest),
        (Stmt::Loop { body, .. }, [PathSegment::LoopBody, rest @ ..]) => {
            statement_container_at(body, rest)
        }
        (Stmt::For { body, .. }, [PathSegment::ForBody, rest @ ..]) => {
            statement_container_at(body, rest)
        }
        (Stmt::Scope { body }, [PathSegment::ScopeBody, rest @ ..]) => {
            statement_container_at(body, rest)
        }
        (Stmt::LabeledBlock { body, .. }, [PathSegment::LabeledBody, rest @ ..]) => {
            statement_container_at(body, rest)
        }
        (Stmt::Unsafe { body }, [PathSegment::UnsafeBody, rest @ ..]) => {
            statement_container_at(&body.stmts, rest)
        }
        (Stmt::While { body, .. }, [PathSegment::WhileBody, rest @ ..]) => {
            statement_container_at(&body.stmts, rest)
        }
        (Stmt::Block(body), [PathSegment::BlockBody, rest @ ..]) => {
            statement_container_at(&body.stmts, rest)
        }
        (Stmt::Match { arms, .. }, [PathSegment::MatchArm(index), rest @ ..]) => {
            statement_container_at(&arms.get(*index)?.body, rest)
        }
        _ => None,
    }
}

query_cache! {
    fn anonymous_structs(&self) -> QueryResult<AnonymousStructSet>;
    key: () = ();
    {
        let anonymous_structs = self.anonymous_struct_facts();
        let records = self
            .program
            .items
            .iter()
            .enumerate()
            .filter_map(|(item_index, item)| match item {
                Item::Record(record) if !record.is_union && record.name.starts_with("anon_") => {
                    Some((item_index, record))
                }
                _ => None,
            })
            .collect::<Vec<_>>();
        let generated = anonymous_structs
            .iter()
            .map(|fact| fact.generated_name.as_str())
            .collect::<BTreeSet<_>>();
        let originals = anonymous_structs
            .iter()
            .map(|fact| fact.original_name.as_str())
            .collect::<BTreeSet<_>>();
        let record_names = records
            .iter()
            .map(|(_, record)| record.name.as_str())
            .collect::<BTreeSet<_>>();
        let occupied = self
            .program
            .items
            .iter()
            .filter_map(item_type_name)
            .collect::<BTreeSet<_>>();
        let conflicts = generated.intersection(&occupied).count()
            + anonymous_structs.len().saturating_sub(generated.len())
            + anonymous_structs.len().saturating_sub(originals.len())
            + records.len().saturating_sub(record_names.len());
        let complete = !records.is_empty()
            && records.len() == anonymous_structs.len()
            && conflicts == 0
            && anonymous_structs.iter().all(|fact| {
                records.iter().any(|(_, record)| {
                    record.name == fact.original_name
                        && record.fields.len() == fact.fields.len()
                        && record
                            .fields
                            .iter()
                            .zip(&fact.fields)
                            .all(|(field, fact)| field.name.as_str() == fact.name)
                })
            });
        let site = ExprSite {
            item_index: records.first().map(|(index, _)| *index).unwrap_or(0),
            path: AstPath(Vec::new()),
            fact_path: AstPath(Vec::new()),
        };
        let evidence = vec![Evidence {
            predicate: Predicate::AnonymousStructDomain,
            site: site.clone(),
            detail: EvidenceDetail::AnonymousStructDomain {
                records: records.len(),
                facts: anonymous_structs.len(),
                conflicts,
                complete,
            },
        }];
        if !complete {
            return Err(Rejection::new(
                Predicate::AnonymousStructDomain,
                Some(site),
                RejectionReason::IncompleteDomain,
                evidence,
            ));
        }
        let structs = anonymous_structs
            .iter()
            .map(|fact| {
                let item_index = records
                    .iter()
                    .find_map(|(item_index, record)| {
                        (record.name == fact.original_name).then_some(*item_index)
                    })
                    .unwrap();
                AnonymousStructPlan {
                    item_index,
                    original_name: fact.original_name.clone(),
                    generated_name: fact.generated_name.clone(),
                    fields: fact
                        .fields
                        .iter()
                        .map(|field| AnonymousStructField {
                            name: field.name.clone(),
                            ty: field.ty.clone(),
                        })
                        .collect(),
                }
            })
            .collect();
        Ok(Proof::new(AnonymousStructSet { structs }, evidence))
    }

    fn definition_users(&self, definition: &DefinitionSite) -> QueryResult<DefinitionUsers>;
    key: DefinitionLocation = definition.location.clone();
    {
        let users = self.count_definition_users(
            definition,
            &BTreeSet::from([definition.location.item_index()]),
        );
        let complete = self.use_domain_complete && !definition.externally_reachable;
        let evidence = vec![Evidence {
            predicate: Predicate::ZeroUsers,
            site: definition_evidence_site(definition),
            detail: EvidenceDetail::UseDomain {
                name: definition.name.clone(),
                users,
                complete,
            },
        }];
        if !complete {
            return Err(Rejection::new(
                Predicate::ZeroUsers,
                Some(definition_evidence_site(definition)),
                RejectionReason::IncompleteDomain,
                evidence,
            ));
        }
        let site = definition_evidence_site(definition);
        Ok(Proof::new(
            DefinitionUsers {
                definition: definition.clone(),
                users,
                site,
            },
            evidence,
        ))
    }

    fn definition_group_users(&self, group: &DefinitionGroup) -> QueryResult<DefinitionGroupUsers>;
    key: DefinitionGroup = group.clone();
    {
        let definitions = self.definitions_in_group(group);
        let definition_items = definitions
            .iter()
            .map(|definition| definition.location.item_index())
            .collect::<BTreeSet<_>>();
        let users = definitions
            .iter()
            .map(|definition| self.count_definition_users(definition, &definition_items))
            .sum();
        let complete = !definitions.is_empty()
            && self.use_domain_complete
            && definitions
                .iter()
                .all(|definition| !definition.externally_reachable);
        let site = definitions
            .first()
            .map(definition_evidence_site)
            .unwrap_or_else(|| ExprSite {
                item_index: 0,
                path: AstPath(Vec::new()),
                fact_path: AstPath(Vec::new()),
            });
        let evidence = vec![Evidence {
            predicate: Predicate::ZeroGroupUsers,
            site: site.clone(),
            detail: EvidenceDetail::GroupUseDomain {
                group: group.clone(),
                definitions: definitions.len(),
                users,
                complete,
            },
        }];
        if !complete {
            return Err(Rejection::new(
                Predicate::ZeroGroupUsers,
                Some(site),
                RejectionReason::IncompleteDomain,
                evidence,
            ));
        }
        Ok(Proof::new(
            DefinitionGroupUsers {
                group: group.clone(),
                users,
                site,
            },
            evidence,
        ))
    }

    fn byte_source(&self, site: &ExprSite) -> QueryResult<ByteSource<'snapshot>>;
    key: ExprSite = site.clone();
    {
        let predicate = Predicate::ByteSource;
        let function = self.function(site).ok_or_else(|| {
            Rejection::new(
                predicate,
                Some(site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            )
        })?;
        let pointer = self
            .facts
            .string_pointer_view(function, &site.fact_path)
            .ok_or_else(|| {
                Rejection::new(
                    predicate,
                    Some(site.clone()),
                    RejectionReason::UnsupportedShape,
                    Vec::new(),
                )
            })?;
        let (shape_name, shape_mutable) = self
            .expr(site)
            .and_then(stable_pointer_source)
            .ok_or_else(|| {
                Rejection::new(
                    predicate,
                    Some(site.clone()),
                    RejectionReason::UnsupportedShape,
                    Vec::new(),
                )
            })?;
        let name = self
            .facts
            .binding_name(pointer.source)
            .ok_or_else(|| {
                Rejection::new(
                    predicate,
                    Some(site.clone()),
                    RejectionReason::MissingEvidence,
                    Vec::new(),
                )
            })?
            .to_string();
        if shape_name != name || shape_mutable != pointer.mutable {
            return Err(Rejection::new(
                predicate,
                Some(site.clone()),
                RejectionReason::Contradicted,
                Vec::new(),
            ));
        }
        let ty = self.facts.binding_type_ast(pointer.source).ok_or_else(|| {
            Rejection::new(
                predicate,
                Some(site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            )
        })?;
        let buffer = self.string_buffer_fact(pointer.source);
        let representation = buffer
            .map(|buffer| representation_for_buffer(buffer.kind))
            .or_else(|| representation_for_type(ty))
            .ok_or_else(|| {
                Rejection::new(
                    predicate,
                    Some(site.clone()),
                    RejectionReason::UnsupportedShape,
                    Vec::new(),
                )
            })?;
        let mutability = if pointer.mutable {
            PointerMutability::Mut
        } else {
            PointerMutability::Const
        };
        let extent = byte_extent(ty);
        let evidence = vec![
            Evidence {
                predicate,
                site: site.clone(),
                detail: EvidenceDetail::Binding { name: name.clone() },
            },
            Evidence {
                predicate,
                site: site.clone(),
                detail: EvidenceDetail::PointerView {
                    representation,
                    mutability,
                },
            },
            Evidence {
                predicate,
                site: site.clone(),
                detail: EvidenceDetail::Extent(extent),
            },
        ];
        Ok(Proof::new(
            ByteSource {
                site: site.clone(),
                name,
                representation,
                mutability,
                extent,
                binding: pointer.source,
                snapshot: PhantomData,
            },
            evidence,
        ))
    }

    fn const_u8(&self, site: &ExprSite) -> QueryResult<u8>;
    key: ExprSite = site.clone();
    {
        let values = self.constant_values(Predicate::ConstantU8, site, |value| match value {
            ConstValue::Integer(value) => u8::try_from(*value)
                .map(Some)
                .map_err(|_| RejectionReason::OutOfRange),
            ConstValue::Usize(value) => u8::try_from(*value)
                .map(Some)
                .map_err(|_| RejectionReason::OutOfRange),
            ConstValue::Zero => Ok(Some(0)),
            _ => Ok(None),
        })?;
        if values.len() != 1 {
            return Err(Rejection::new(
                Predicate::ConstantU8,
                Some(site.clone()),
                RejectionReason::Ambiguous,
                Vec::new(),
            ));
        }
        let value = *values.first().unwrap();
        Ok(Proof::new(
            value,
            vec![Evidence {
                predicate: Predicate::ConstantU8,
                site: site.clone(),
                detail: EvidenceDetail::ConstantU8(value),
            }],
        ))
    }

    fn const_usize(&self, site: &ExprSite) -> QueryResult<usize>;
    key: ExprSite = site.clone();
    {
        let values = self.constant_values(Predicate::ConstantUsize, site, |value| match value {
            ConstValue::Integer(value) => usize::try_from(*value)
                .map(Some)
                .map_err(|_| RejectionReason::OutOfRange),
            ConstValue::Usize(value) => Ok(Some(*value)),
            ConstValue::Zero => Ok(Some(0)),
            ConstValue::ArrayLength(value) => Ok(Some(*value)),
            _ => Ok(None),
        })?;
        if values.len() != 1 {
            return Err(Rejection::new(
                Predicate::ConstantUsize,
                Some(site.clone()),
                RejectionReason::Ambiguous,
                Vec::new(),
            ));
        }
        let value = *values.first().unwrap();
        Ok(Proof::new(
            value,
            vec![Evidence {
                predicate: Predicate::ConstantUsize,
                site: site.clone(),
                detail: EvidenceDetail::ConstantUsize(value),
            }],
        ))
    }

    fn full_byte_view(&self, source: &ByteSource<'snapshot>, count: &ExprSite) -> QueryResult<ByteView<'snapshot>>;
    key: (ExprSite, ExprSite) = (source.site.clone(), count.clone());
    {
        let constant = self.const_usize(count);
        if let (ByteExtent::Constant(extent), Ok(count_proof)) = (source.extent, &constant) {
            if extent != count_proof.value {
                return Err(Rejection::new(
                    Predicate::FullByteView,
                    Some(count.clone()),
                    RejectionReason::Contradicted,
                    count_proof.evidence.clone(),
                ));
            }
            let mut evidence = count_proof.evidence.clone();
            evidence.extend(self.pure(count)?.evidence);
            evidence.push(Evidence {
                predicate: Predicate::FullByteView,
                site: source.site.clone(),
                detail: EvidenceDetail::Extent(source.extent),
            });
            return Ok(Proof::new(
                ByteView {
                    source: source.clone(),
                    extent: source.extent,
                },
                evidence,
            ));
        }
        if self.count_matches_source_len(source, count) {
            return Ok(Proof::new(
                ByteView {
                    source: source.clone(),
                    extent: source.extent,
                },
                vec![Evidence {
                    predicate: Predicate::FullByteView,
                    site: count.clone(),
                    detail: EvidenceDetail::SourceLength,
                }],
            ));
        }
        let evidence = constant
            .map(|proof| proof.evidence)
            .unwrap_or_else(|rejection| rejection.evidence);
        Err(Rejection::new(
            Predicate::FullByteView,
            Some(count.clone()),
            RejectionReason::MissingEvidence,
            evidence,
        ))
    }

    fn first_nul(&self, source: &ByteSource<'snapshot>) -> QueryResult<NulPosition>;
    key: BindingId = source.binding;
    {
        let predicate = Predicate::FirstNul;
        let Some(buffer) = self.string_buffer_fact(source.binding) else {
            return Err(Rejection::new(
                predicate,
                Some(source.site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        if buffer.interior_nul {
            return Err(Rejection::new(
                predicate,
                Some(source.site.clone()),
                RejectionReason::Contradicted,
                Vec::new(),
            ));
        }
        if !buffer.ascii_only {
            let reason = if buffer.bytes.is_some() {
                RejectionReason::Contradicted
            } else {
                RejectionReason::MissingEvidence
            };
            return Err(Rejection::new(
                predicate,
                Some(source.site.clone()),
                reason,
                Vec::new(),
            ));
        }
        if !matches!(
            buffer.nul_termination,
            NulTermination::Terminated | NulTermination::AllZero
        ) {
            let reason = if buffer.nul_termination == NulTermination::Unterminated {
                RejectionReason::Contradicted
            } else {
                RejectionReason::MissingEvidence
            };
            return Err(Rejection::new(
                predicate,
                Some(source.site.clone()),
                reason,
                Vec::new(),
            ));
        }
        let position = buffer
            .bytes
            .as_ref()
            .map(|bytes| NulPosition::Constant(bytes.len()))
            .unwrap_or(NulPosition::ByteLength);
        Ok(Proof::new(
            position,
            vec![Evidence {
                predicate,
                site: source.site.clone(),
                detail: EvidenceDetail::NulPosition(position),
            }],
        ))
    }

    fn prefix_contains(&self, count: &ExprSite, nul: NulPosition) -> QueryResult<()>;
    key: (ExprSite, NulPosition) = (count.clone(), nul);
    {
        let count_proof = self.const_usize(count)?;
        let count_stable = self.pure(count)?;
        let NulPosition::Constant(nul) = nul else {
            return Err(Rejection::new(
                Predicate::PrefixContains,
                Some(count.clone()),
                RejectionReason::MissingEvidence,
                count_proof.evidence,
            ));
        };
        if nul >= count_proof.value {
            return Err(Rejection::new(
                Predicate::PrefixContains,
                Some(count.clone()),
                RejectionReason::Contradicted,
                count_proof.evidence,
            ));
        }
        let count_value = count_proof.value;
        let mut evidence = count_proof.evidence;
        evidence.extend(count_stable.evidence);
        evidence.push(Evidence {
            predicate: Predicate::PrefixContains,
            site: count.clone(),
            detail: EvidenceDetail::PrefixContains {
                count: count_value,
                nul,
            },
        });
        Ok(Proof::new((), evidence))
    }

    fn pure(&self, site: &ExprSite) -> QueryResult<StableExpr>;
    key: ExprSite = site.clone();
    {
        let predicate = Predicate::MovablePure;
        let function = self.function(site).ok_or_else(|| {
            Rejection::new(
                predicate,
                Some(site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            )
        })?;
        let effect = self
            .effect_fact(function, EffectSubject::Expr, &site.path)
            .or_else(|| self.effect_fact(function, EffectSubject::Expr, &site.fact_path))
            .ok_or_else(|| {
                Rejection::new(
                    predicate,
                    Some(site.clone()),
                    RejectionReason::MissingEvidence,
                    Vec::new(),
                )
            })?;
        if effect.purity != Purity::MovablePure {
            return Err(Rejection::new(
                predicate,
                Some(site.clone()),
                RejectionReason::Contradicted,
                Vec::new(),
            ));
        }
        Ok(Proof::new(
            StableExpr { site: site.clone() },
            vec![Evidence {
                predicate,
                site: site.clone(),
                detail: EvidenceDetail::MovablePure,
            }],
        ))
    }

    fn counted_loop(&self, statement: &StatementRef) -> QueryResult<CountedLoopFact>;
    key: StatementRef = statement.clone();
    {
        let predicate = Predicate::CountedLoop;
        let evidence_site = statement_evidence_site(statement);
        let function = self.facts.function_by_item_index(statement.item_index).ok_or_else(|| {
            Rejection::new(
                predicate,
                Some(evidence_site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            )
        })?;
        let Some(fact) = self.counted_loop_fact(function, &statement.path) else {
            return Err(Rejection::new(
                predicate,
                Some(evidence_site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        Ok(Proof::new(
            fact.clone(),
            vec![Evidence {
                predicate,
                site: evidence_site,
                detail: EvidenceDetail::CountedLoop {
                    start: fact.start,
                    step: fact.step,
                    index_use: fact.index_use,
                },
            }],
        ))
    }

    fn counted_slice_loop(&self, statement: &StatementRef) -> QueryResult<SliceLoopFact>;
    key: StatementRef = statement.clone();
    {
        let predicate = Predicate::CountedSliceLoop;
        let evidence_site = statement_evidence_site(statement);
        let function = self.facts.function_by_item_index(statement.item_index).ok_or_else(|| {
            Rejection::new(
                predicate,
                Some(evidence_site.clone()),
                RejectionReason::MissingEvidence,
                Vec::new(),
            )
        })?;
        let Some(fact) = self.counted_slice_loop_fact(function, &statement.path) else {
            return Err(Rejection::new(
                predicate,
                Some(evidence_site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let bindings = self.all_bindings();
        let Some(index) = bindings.iter().find(|binding| binding.id == fact.index).cloned()
        else {
            return Err(Rejection::new(
                predicate,
                Some(evidence_site),
                RejectionReason::IncompleteDomain,
                Vec::new(),
            ));
        };
        let Some(slice) = bindings.iter().find(|binding| binding.id == fact.slice).cloned()
        else {
            return Err(Rejection::new(
                predicate,
                Some(evidence_site),
                RejectionReason::IncompleteDomain,
                Vec::new(),
            ));
        };
        Ok(Proof::new(
            SliceLoopFact {
                index,
                slice,
                start: fact.start,
                bound: fact.bound,
                step: fact.step,
                index_use: fact.index_use,
                access: fact.access,
            },
            vec![Evidence {
                predicate,
                site: evidence_site,
                detail: EvidenceDetail::CountedSliceLoop {
                    index_use: fact.index_use,
                    access: fact.access,
                },
            }],
        ))
    }

    fn lazy_singletons(&self) -> QueryResult<LazySingletonSet>;
    key: () = ();
    {
        let predicate = Predicate::LazySingletonDomain;
        let mut singletons = Vec::new();
        for singleton in self.lazy_init_singleton_facts() {
            let Some(function_item_index) = self.facts.function_item_index(singleton.function)
            else {
                return Err(Rejection::new(
                    predicate,
                    None,
                    RejectionReason::IncompleteDomain,
                    Vec::new(),
                ));
            };
            let Some(function_name) = self.facts.function_name(singleton.function) else {
                return Err(Rejection::new(
                    predicate,
                    None,
                    RejectionReason::IncompleteDomain,
                    Vec::new(),
                ));
            };
            if !matches!(
                self.program.items.get(function_item_index),
                Some(Item::Fn(f)) if f.name == function_name
            ) {
                return Err(Rejection::new(
                    predicate,
                    None,
                    RejectionReason::Contradicted,
                    Vec::new(),
                ));
            }
            let Some(payload_item_index) = static_item_index(self.program, &singleton.payload_name)
            else {
                return Err(Rejection::new(
                    predicate,
                    None,
                    RejectionReason::IncompleteDomain,
                    Vec::new(),
                ));
            };
            let Some(flag_item_index) = static_item_index(self.program, &singleton.flag_name)
            else {
                return Err(Rejection::new(
                    predicate,
                    None,
                    RejectionReason::IncompleteDomain,
                    Vec::new(),
                ));
            };
            singletons.push(LazySingletonPlan {
                function_item_index,
                payload_item_index,
                payload_name: singleton.payload_name.clone(),
                payload_ty: singleton.payload_ty.clone(),
                init_expr: singleton.init_expr.clone(),
                flag_item_index,
                flag_name: singleton.flag_name.clone(),
            });
        }
        let site = expression_site(
            singletons.first().map_or(0, |plan| plan.function_item_index),
            &[],
        );
        let evidence = vec![Evidence {
            predicate,
            site,
            detail: EvidenceDetail::LazySingletonDomain {
                singletons: singletons.len(),
            },
        }];
        Ok(Proof::new(LazySingletonSet { singletons }, evidence))
    }

    fn atomic_promotions(&self) -> QueryResult<AtomicPromotionSet>;
    key: () = ();
    {
        let predicate = Predicate::AtomicPromotionDomain;
        let mut locals = Vec::new();
        for fact in self.atomic_local_facts() {
            let Some(function_item_index) = self.facts.function_item_index(fact.function) else {
                return Err(Rejection::new(
                    predicate,
                    None,
                    RejectionReason::IncompleteDomain,
                    Vec::new(),
                ));
            };
            locals.push(AtomicLocalPromotion {
                function_item_index,
                name: fact.name.clone(),
                ty: fact.ty,
            });
        }
        let mut globals = Vec::new();
        for fact in self.atomic_global_facts() {
            if !static_exists(&self.program.items, &fact.name) {
                return Err(Rejection::new(
                    predicate,
                    None,
                    RejectionReason::IncompleteDomain,
                    Vec::new(),
                ));
            }
            globals.push(AtomicGlobalPromotion {
                name: fact.name.clone(),
                ty: fact.ty,
            });
        }
        let evidence = vec![Evidence {
            predicate,
            site: expression_site(locals.first().map_or(0, |plan| plan.function_item_index), &[]),
            detail: EvidenceDetail::AtomicPromotionDomain {
                locals: locals.len(),
                globals: globals.len(),
            },
        }];
        Ok(Proof::new(AtomicPromotionSet { locals, globals }, evidence))
    }

    fn heap_ownership_facts(&self, function: &FunctionRef) -> QueryResult<HeapOwnershipFacts>;
    key: FunctionId = function.id;
    {
        let predicate = Predicate::HeapOwnershipFacts;
        let site = expression_site(function.item_index, &[]);
        let bindings = self.all_bindings();
        let binding = |id| bindings.iter().find(|binding| binding.id == id).cloned();
        let statement = |path: &AstPath| StatementRef {
            item_index: function.item_index,
            path: path.clone(),
        };
        let mut owners = Vec::new();
        for fact in self.heap_ownership_fact_list(function.id)
        {
            let Some(pointer) = binding(fact.pointer) else {
                return Err(Rejection::new(
                    predicate,
                    Some(site.clone()),
                    RejectionReason::IncompleteDomain,
                    Vec::new(),
                ));
            };
            let Some(allocation_temp) = binding(fact.allocation_temp) else {
                return Err(Rejection::new(
                    predicate,
                    Some(site.clone()),
                    RejectionReason::IncompleteDomain,
                    Vec::new(),
                ));
            };
            let size_temp = match fact.size_temp {
                Some(id) => Some(binding(id).ok_or_else(|| {
                    Rejection::new(
                        predicate,
                        Some(site.clone()),
                        RejectionReason::IncompleteDomain,
                        Vec::new(),
                    )
                })?),
                None => None,
            };
            let free_temp = match fact.free_temp {
                Some(id) => Some(binding(id).ok_or_else(|| {
                    Rejection::new(
                        predicate,
                        Some(site.clone()),
                        RejectionReason::IncompleteDomain,
                        Vec::new(),
                    )
                })?),
                None => None,
            };
            let aliases = fact
                .aliases
                .iter()
                .map(|id| binding(*id))
                .collect::<Option<Vec<_>>>()
                .ok_or_else(|| {
                    Rejection::new(
                        predicate,
                        Some(site.clone()),
                        RejectionReason::IncompleteDomain,
                        Vec::new(),
                    )
                })?;
            let reallocations = fact
                .reallocations
                .iter()
                .map(|realloc| {
                    Some(HeapReallocation {
                        source_temp: match realloc.source_temp {
                            Some(id) => Some(binding(id)?),
                            None => None,
                        },
                        allocation_temp: binding(realloc.allocation_temp)?,
                        size_temp: match realloc.size_temp {
                            Some(id) => Some(binding(id)?),
                            None => None,
                        },
                        allocation_statement: statement(&realloc.allocation_path),
                        assignment_statement: statement(&realloc.assign_path),
                        new_extent: realloc.new_extent.clone(),
                        init: realloc.init,
                        resize: realloc.resize,
                    })
                })
                .collect::<Option<Vec<_>>>()
                .ok_or_else(|| {
                    Rejection::new(
                        predicate,
                        Some(site.clone()),
                        RejectionReason::IncompleteDomain,
                        Vec::new(),
                    )
                })?;
            owners.push(HeapOwnership {
                pointer,
                allocation_temp,
                size_temp,
                free_temp,
                aliases,
                pointer_statement: statement(&fact.pointer_path),
                allocation_statement: statement(&fact.allocation_path),
                assignment_statement: statement(&fact.assign_path),
                free_statement: statement(&fact.free_path),
                elem_ty: fact.elem_ty.clone(),
                allocation: fact.allocation,
                extent: fact.extent.clone(),
                init: fact.init,
                read_safety: fact.read_safety,
                uses: fact
                    .uses
                    .iter()
                    .map(|usage| HeapUse {
                        statement: statement(&usage.path),
                        kind: usage.kind.clone(),
                    })
                    .collect(),
                reallocations,
            });
        }
        if owners.is_empty() {
            return Err(Rejection::new(
                predicate,
                Some(site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        }
        let evidence = vec![Evidence {
            predicate,
            site,
            detail: EvidenceDetail::HeapOwnershipFacts {
                owners: owners.len(),
            },
        }];
        Ok(Proof::new(HeapOwnershipFacts { owners }, evidence))
    }

    fn file_ownership_facts(&self, function: &FunctionRef) -> QueryResult<FileOwnershipFacts>;
    key: FunctionId = function.id;
    {
        let predicate = Predicate::FileOwnershipFacts;
        let site = expression_site(function.item_index, &[]);
        let bindings = self.all_bindings();
        let binding = |id| bindings.iter().find(|binding| binding.id == id).cloned();
        let statement = |path: &AstPath| StatementRef {
            item_index: function.item_index,
            path: path.clone(),
        };
        let mut owners = Vec::new();
        for fact in self.file_ownership_fact_list(function.id) {
            let Some(handle) = binding(fact.handle) else {
                return Err(Rejection::new(
                    predicate,
                    Some(site.clone()),
                    RejectionReason::IncompleteDomain,
                    Vec::new(),
                ));
            };
            if binding(fact.open_temp).is_none() {
                return Err(Rejection::new(
                    predicate,
                    Some(site.clone()),
                    RejectionReason::IncompleteDomain,
                    Vec::new(),
                ));
            };
            if let Some(id) = fact.close_temp
                && binding(id).is_none()
            {
                return Err(Rejection::new(
                    predicate,
                    Some(site.clone()),
                    RejectionReason::IncompleteDomain,
                    Vec::new(),
                ));
            }
            owners.push(FileOwnership {
                handle,
                handle_statement: statement(&fact.handle_path),
                open_statement: statement(&fact.open_path),
                assign_statement: statement(&fact.assign_path),
                mode: fact.mode,
                uses: fact
                    .uses
                    .iter()
                    .map(|usage| FileUse {
                        statement: statement(&usage.path),
                        kind: usage.kind,
                    })
                    .collect(),
            });
        }
        if owners.is_empty() {
            return Err(Rejection::new(
                predicate,
                Some(site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        }
        let evidence = vec![Evidence {
            predicate,
            site,
            detail: EvidenceDetail::FileOwnershipFacts {
                owners: owners.len(),
            },
        }];
        Ok(Proof::new(FileOwnershipFacts { owners }, evidence))
    }

    fn ptr_len_slices(&self) -> QueryResult<PtrLenPlanSet>;
    key: () = ();
    {
        let predicate = Predicate::PtrLenSlice;
        let slices = self.ptr_len_slice_facts();
        let plans = ptr_len_plans_from_facts(slices, self.facts);
        let site = expression_site(plans.first().map_or(0, |plan| plan.item_index), &[]);
        if plans.is_empty() {
            return Err(Rejection::new(
                predicate,
                Some(site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        }
        let evidence = vec![Evidence {
            predicate,
            site,
            detail: EvidenceDetail::PtrLenSlice { plans: plans.len() },
        }];
        Ok(Proof::new(PtrLenPlanSet { plans }, evidence))
    }

    #[expect(dead_code, reason = "query API surface not yet wired into a fixup rule")]
    fn struct_field_ownership_facts(&self) -> QueryResult<Vec<StructFieldOwnershipFact>>;
    key: () = ();
    {
        let predicate = Predicate::StructFieldOwnership;
        let fields: Vec<StructFieldOwnershipFact> = self
            .struct_field_ownership_fact_list()
            .into_iter()
            .cloned()
            .collect();
        let evidence = vec![Evidence {
            predicate,
            site: expression_site(0, &[]),
            detail: EvidenceDetail::StructFieldOwnership {
                fields: fields.len(),
            },
        }];
        Ok(Proof::new(fields, evidence))
    }

    fn callee_alloc_summary(&self, function: &FunctionRef) -> QueryResult<CalleeAllocSummaryFact>;
    key: FunctionId = function.id;
    {
        let predicate = Predicate::CalleeAllocSummary;
        let site = expression_site(function.item_index, &[]);
        let Some(summary) = self.callee_alloc_summary_fact(function.id).cloned() else {
            return Err(Rejection::new(
                predicate,
                Some(site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let evidence = vec![Evidence {
            predicate,
            site,
            detail: EvidenceDetail::CalleeAllocSummary {
                function: function.name.clone(),
            },
        }];
        Ok(Proof::new(summary, evidence))
    }

    fn interprocedural_alloc_eligibility(&self, function: &FunctionRef) -> QueryResult<InterproceduralAllocEligibilityFact>;
    key: FunctionId = function.id;
    {
        let predicate = Predicate::InterproceduralAllocEligibility;
        let site = expression_site(function.item_index, &[]);
        let Some(fact) = self.interprocedural_alloc_eligibility_fact(function.id) else {
            return Err(Rejection::new(
                predicate,
                Some(site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let evidence = vec![Evidence {
            predicate,
            site,
            detail: EvidenceDetail::InterproceduralAllocEligibility {
                function: function.name.clone(),
                eligible: fact.eligible,
            },
        }];
        Ok(Proof::new(fact, evidence))
    }

    fn interprocedural_alloc_chain(&self, function: &FunctionRef) -> QueryResult<Vec<FunctionRef>>;
    key: FunctionId = function.id;
    {
        let predicate = Predicate::InterproceduralAllocEligibility;
        let site = expression_site(function.item_index, &[]);
        let Some(fact) = self.interprocedural_alloc_eligibility_fact(function.id) else {
            return Err(Rejection::new(
                predicate,
                Some(site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        };
        let functions = self.all_functions();
        let mut chain = Vec::new();
        for member in &fact.chain {
            let Some(member_ref) = functions.iter().find(|candidate| candidate.id == *member).cloned()
            else {
                return Err(Rejection::new(
                    predicate,
                    Some(site),
                    RejectionReason::IncompleteDomain,
                    Vec::new(),
                ));
            };
            chain.push(member_ref);
        }
        let evidence = vec![Evidence {
            predicate,
            site,
            detail: EvidenceDetail::InterproceduralAllocEligibility {
                function: function.name.clone(),
                eligible: fact.eligible,
            },
        }];
        Ok(Proof::new(chain, evidence))
    }

    fn interprocedural_alloc_callers(&self, function: &FunctionRef) -> QueryResult<Vec<InterproceduralAllocCallerInput>>;
    key: FunctionId = function.id;
    {
        let predicate = Predicate::InterproceduralAllocCallers;
        let site = expression_site(function.item_index, &[]);
        let functions = self.all_functions();
        let mut inputs = Vec::new();
        for caller_fact in self.interprocedural_alloc_caller_facts(function.id) {
            let Some(caller_ref) = functions
                .iter()
                .find(|candidate| candidate.id == caller_fact.caller)
                .cloned()
            else {
                return Err(Rejection::new(
                    predicate,
                    Some(site),
                    RejectionReason::IncompleteDomain,
                    Vec::new(),
                ));
            };
            inputs.push(InterproceduralAllocCallerInput {
                pointer_name: caller_fact.pointer_name.clone(),
                decl_stmt: StatementRef {
                    item_index: caller_ref.item_index,
                    path: caller_fact.decl_path.clone(),
                },
                call_temp_stmt: StatementRef {
                    item_index: caller_ref.item_index,
                    path: caller_fact.call_temp_path.clone(),
                },
                free_stmt: caller_fact.free_path.as_ref().map(|path| StatementRef {
                    item_index: caller_ref.item_index,
                    path: path.clone(),
                }),
                caller: caller_ref,
            });
        }
        let evidence = vec![Evidence {
            predicate,
            site,
            detail: EvidenceDetail::InterproceduralAllocCallers {
                function: function.name.clone(),
                callers: inputs.len(),
            },
        }];
        Ok(Proof::new(inputs, evidence))
    }

    fn option_box_local_candidates(&self, function: &FunctionRef) -> QueryResult<Vec<OptionBoxLocalPlanInput>>;
    key: FunctionId = function.id;
    {
        let predicate = Predicate::OptionBoxLocalCandidates;
        let site = expression_site(function.item_index, &[]);
        let bindings = self.all_bindings();
        let mut inputs = Vec::new();
        for candidate in self.option_box_local_candidate_facts(function.id) {
            let Some(binding) = bindings
                .iter()
                .find(|binding| binding.id == candidate.binding)
                .cloned()
            else {
                return Err(Rejection::new(
                    predicate,
                    Some(site.clone()),
                    RejectionReason::IncompleteDomain,
                    Vec::new(),
                ));
            };
            let assignments = candidate
                .assignments
                .iter()
                .map(|assignment| OptionBoxAssignmentInput {
                    stmt: StatementRef {
                        item_index: function.item_index,
                        path: assignment.path.clone(),
                    },
                    kind: assignment.kind,
                    alloc_source: assignment.alloc_source.as_ref().map(|path| StatementRef {
                        item_index: function.item_index,
                        path: path.clone(),
                    }),
                })
                .collect();
            let deref_sites = candidate
                .deref_paths
                .iter()
                .map(|path| expression_site(function.item_index, &path.0))
                .collect();
            inputs.push(OptionBoxLocalPlanInput {
                binding,
                elem_ty: candidate.elem_ty.clone(),
                decl_stmt: StatementRef {
                    item_index: function.item_index,
                    path: candidate.decl_path.clone(),
                },
                assignments,
                deref_sites,
            });
        }
        if inputs.is_empty() {
            return Err(Rejection::new(
                predicate,
                Some(site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        }
        let evidence = vec![Evidence {
            predicate,
            site,
            detail: EvidenceDetail::OptionBoxLocalCandidates {
                count: inputs.len(),
            },
        }];
        Ok(Proof::new(inputs, evidence))
    }

    fn option_box_comparisons(&self, function: &FunctionRef) -> QueryResult<Vec<OptionBoxComparisonInput>>;
    key: FunctionId = function.id;
    {
        let predicate = Predicate::OptionBoxComparisons;
        let site = expression_site(function.item_index, &[]);
        let inputs: Vec<OptionBoxComparisonInput> = self
            .option_box_comparison_facts(function.id)
            .into_iter()
            .map(|comparison| OptionBoxComparisonInput {
                if_stmt: StatementRef {
                    item_index: function.item_index,
                    path: comparison.if_stmt_path.clone(),
                },
                lhs: comparison.lhs.clone(),
                rhs: comparison.rhs.clone(),
                negate: comparison.negate,
            })
            .collect();
        let evidence = vec![Evidence {
            predicate,
            site,
            detail: EvidenceDetail::OptionBoxComparisons {
                count: inputs.len(),
            },
        }];
        Ok(Proof::new(inputs, evidence))
    }

    fn string_param_lift_indices(&self, function: &FunctionRef) -> QueryResult<Vec<usize>>;
    key: FunctionId = function.id;
    {
        let predicate = Predicate::StringParamLift;
        let site = expression_site(function.item_index, &[]);
        let mut indices = self
            .string_param_lift_facts()
            .iter()
            .filter(|fact| fact.callee == function.id)
            .map(|fact| fact.index)
            .collect::<Vec<_>>();
        indices.sort_unstable();
        if indices.is_empty() {
            return Err(Rejection::new(
                predicate,
                Some(site),
                RejectionReason::MissingEvidence,
                Vec::new(),
            ));
        }
        let evidence = vec![Evidence {
            predicate,
            site,
            detail: EvidenceDetail::StringParamLift,
        }];
        Ok(Proof::new(indices, evidence))
    }

    fn calls_in(&self, function: &FunctionRef) -> QueryResult<Vec<CallRecord>>;
    key: FunctionId = function.id;
    {
        let (_, evidence) = self.function_snapshot(function)?.into_parts();
        let calls = self
            .all_calls()
            .filter(|call| call.site.item_index == function.item_index)
            .cloned()
            .collect::<Vec<_>>();
        Ok(Proof::new(calls, evidence))
    }

    fn function_by_name(&self, name: &str) -> QueryResult<FunctionRef>;
    key: String = name.to_string();
    {
        let predicate = Predicate::Function;
        let site = expression_site(0, &[]);
        let function = self
            .all_functions()
            .into_iter()
            .find(|function| function.name == name)
            .ok_or_else(|| {
                Rejection::new(
                    predicate,
                    Some(site.clone()),
                    RejectionReason::MissingEvidence,
                    Vec::new(),
                )
            })?;
        let evidence = vec![Evidence {
            predicate,
            site,
            detail: EvidenceDetail::Function {
                name: function.name.clone(),
            },
        }];
        Ok(Proof::new(function, evidence))
    }

    fn buffer_pointer_fields(&self, function: &FunctionRef) -> QueryResult<BufferPointerFields>;
    key: FunctionId = function.id;
    {
        let predicate = Predicate::BufferPointerFields;
        let site = expression_site(function.item_index, &[]);
        let bindings = self.all_bindings();
        let fields = self.buffer_pointer_field_facts(function.id).into_iter()
            .filter_map(|fact| {
                let buffer = bindings.iter().find(|binding| binding.id == fact.buffer)?.clone();
                let array = bindings.iter().find(|binding| binding.id == fact.array)?.clone();
                let array_len = self.facts.binding_type(fact.array)
                    .and_then(buffer_cursor_array_len_from_rendered_type)?;
                Some(BufferPointerField {
                    buffer,
                    array,
                    assignment: StatementRef {
                        item_index: function.item_index,
                        path: fact.site.path.clone(),
                    },
                    array_len,
                })
            })
            .collect::<Vec<_>>();
        if fields.is_empty() {
            return Err(Rejection::new(predicate, Some(site), RejectionReason::MissingEvidence, Vec::new()));
        }
        let evidence = vec![Evidence {
            predicate,
            site,
            detail: EvidenceDetail::BufferPointerFields { fields: fields.len() },
        }];
        Ok(Proof::new(BufferPointerFields { fields }, evidence))
    }

    fn reference_domain(&self) -> QueryResult<ReferenceDomain>;
    key: () = ();
    {
        let predicate = Predicate::ReferenceDomain;
        let site = expression_site(0, &[]);
        if !self.use_domain_complete {
            return Err(Rejection::new(
                predicate,
                Some(site),
                RejectionReason::IncompleteDomain,
                Vec::new(),
            ));
        }
        let definitions = self.all_definitions().cloned().collect::<Vec<_>>();
        let items = self
            .program
            .items
            .iter()
            .enumerate()
            .map(|(item_index, item)| ItemReferences {
                item_index,
                symbols: unused_item_refs(item),
            })
            .collect::<Vec<_>>();
        let evidence = vec![Evidence {
            predicate,
            site,
            detail: EvidenceDetail::ReferenceDomain {
                definitions: definitions.len(),
                items: items.len(),
            },
        }];
        Ok(Proof::new(ReferenceDomain { definitions, items }, evidence))
    }
}

fn collect_array_element_pointer_aliases(
    body: &[IndentStmt],
    origins: &mut BTreeMap<String, ArrayElementPointerOrigin>,
) {
    let mut changed = true;
    while changed {
        changed = false;
        collect_array_element_pointer_aliases_once(body, origins, &mut changed);
    }
}

fn collect_array_element_pointer_aliases_once(
    body: &[IndentStmt],
    origins: &mut BTreeMap<String, ArrayElementPointerOrigin>,
    changed: &mut bool,
) {
    for indent in body {
        let alias = match &indent.stmt {
            Stmt::Let {
                name,
                init: Some(Expr::Var(source)),
                ..
            } => Some((name.as_str(), source.as_str())),
            Stmt::Assign {
                target: Expr::Var(name),
                value: Expr::Var(source),
            } => Some((name.as_str(), source.as_str())),
            _ => None,
        };
        if let Some((name, source)) = alias
            && let Some(origin) = origins.get(source).cloned()
            && !origins.contains_key(name)
        {
            origins.insert(
                name.to_string(),
                ArrayElementPointerOrigin {
                    pointer_name: name.to_string(),
                    ..origin
                },
            );
            *changed = true;
        }
        walk::nested_body_vecs_with_path(&indent.stmt, &mut Vec::new(), &mut |nested, _| {
            collect_array_element_pointer_aliases_once(nested, origins, changed);
        });
    }
}

fn buffer_cursor_array_len_from_rendered_type(ty: &str) -> Option<usize> {
    let (_, len) = ty.rsplit_once(';')?;
    len.trim_end_matches(']').trim().parse().ok()
}

fn collect_assign_value_sites(
    item_index: usize,
    body: &[IndentStmt],
    path: &mut Vec<PathSegment>,
    targets: &mut BTreeSet<ExprSite>,
    role_values: &mut BTreeSet<ExprSite>,
    assignment_values: &mut BTreeSet<ExprSite>,
) {
    for (index, indent) in body.iter().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            if matches!(
                &indent.stmt,
                Stmt::Assign { .. } | Stmt::CompoundAssign { .. }
            ) {
                let mut target_path = path.clone();
                target_path.push(PathSegment::Expr(0));
                targets.insert(expression_site(item_index, &target_path));
                let mut value_path = path.clone();
                value_path.push(PathSegment::Expr(1));
                let value_site = expression_site(item_index, &value_path);
                role_values.insert(value_site.clone());
                if matches!(&indent.stmt, Stmt::Assign { .. }) {
                    assignment_values.insert(value_site);
                }
            }
            walk::nested_body_vecs_with_path(&indent.stmt, path, &mut |nested, path| {
                collect_assign_value_sites(
                    item_index,
                    nested,
                    path,
                    targets,
                    role_values,
                    assignment_values,
                );
            });
        });
    }
}

fn expression_kind(expr: &Expr) -> ExpressionKind {
    match expr {
        Expr::Call { .. } | Expr::MethodCall { .. } | Expr::MethodCallGeneric { .. } => {
            ExpressionKind::Call
        }
        Expr::Cast { .. } | Expr::Transmute { .. } => ExpressionKind::Cast,
        Expr::Field { .. } | Expr::TupleField { .. } => ExpressionKind::Field,
        Expr::Index { .. } => ExpressionKind::Index,
        Expr::Value(_) | Expr::Str(_) | Expr::HexFloat(_) | Expr::ByteStr(_) | Expr::CStr(_) => {
            ExpressionKind::Literal
        }
        Expr::Var(_) | Expr::Path(_) => ExpressionKind::Variable,
        _ => ExpressionKind::Other,
    }
}

fn index_definitions(
    item: &Item,
    item_index: usize,
    definitions: &mut BTreeMap<DefinitionSelector, Vec<DefinitionSite>>,
) {
    match item {
        Item::Fn(function) => {
            let site = DefinitionSite {
                location: DefinitionLocation::Item(item_index),
                kind: DefinitionKind::Function,
                name: function.name.clone(),
                symbols: vec![function.name.clone()],
                group: None,
                externally_reachable: function.vis == Visibility::Pub
                    || function.abi.is_some()
                    || function.attrs.iter().any(exporting_attr),
            };
            definitions
                .entry(DefinitionSelector {
                    kind: site.kind,
                    name: site.name.clone(),
                })
                .or_default()
                .push(site);
        }
        Item::ExternBlock { decls, .. } => {
            for (decl_index, decl) in decls.iter().enumerate() {
                let (kind, name, group) = match decl {
                    ExternDecl::Fn(function) => (
                        DefinitionKind::ExternFunction,
                        function.name.clone(),
                        match function.identity {
                            FunctionIdentity::Known(known) => {
                                Some(DefinitionGroup::Header(known.header().into()))
                            }
                            FunctionIdentity::Unknown => None,
                        },
                    ),
                    ExternDecl::Static { name, .. } => {
                        (DefinitionKind::ExternStatic, name.clone(), None)
                    }
                };
                let site = DefinitionSite {
                    location: DefinitionLocation::ExternDecl {
                        item_index,
                        decl_index,
                    },
                    kind,
                    symbols: vec![name.clone()],
                    name,
                    group,
                    externally_reachable: false,
                };
                definitions
                    .entry(DefinitionSelector {
                        kind: site.kind,
                        name: site.name.clone(),
                    })
                    .or_default()
                    .push(site);
            }
        }
        Item::SupportModule(module) => {
            let name = module.name.as_str().to_owned();
            let site = DefinitionSite {
                location: DefinitionLocation::Item(item_index),
                kind: DefinitionKind::SupportModule,
                name: name.clone(),
                symbols: module.exports.iter().map(qualified_path).collect(),
                group: Some(DefinitionGroup::SupportModule(name)),
                externally_reachable: false,
            };
            definitions
                .entry(DefinitionSelector {
                    kind: site.kind,
                    name: site.name.clone(),
                })
                .or_default()
                .push(site);
        }
        Item::Struct(def) => {
            if !attrs_have_used(&def.attrs) {
                push_type_definition(
                    definitions,
                    item_index,
                    DefinitionKind::Struct,
                    &def.name,
                    vec![def.name.clone()],
                );
            }
        }
        Item::Record(def) => {
            push_type_definition(
                definitions,
                item_index,
                DefinitionKind::Record,
                &def.name,
                vec![def.name.clone()],
            );
        }
        Item::Enum(def) => {
            if !attrs_have_used(&def.attrs) {
                let mut symbols = vec![def.name.clone()];
                symbols.extend(def.variants.iter().map(|variant| variant.name.clone()));
                push_type_definition(
                    definitions,
                    item_index,
                    DefinitionKind::Enum,
                    &def.name,
                    symbols,
                );
            }
        }
        Item::Cfg { item, .. } => index_definitions(item, item_index, definitions),
        _ => {}
    }
}

fn push_type_definition(
    definitions: &mut BTreeMap<DefinitionSelector, Vec<DefinitionSite>>,
    item_index: usize,
    kind: DefinitionKind,
    name: &str,
    symbols: Vec<String>,
) {
    let site = DefinitionSite {
        location: DefinitionLocation::Item(item_index),
        kind,
        name: name.to_string(),
        symbols,
        group: None,
        externally_reachable: false,
    };
    definitions
        .entry(DefinitionSelector {
            kind: site.kind,
            name: site.name.clone(),
        })
        .or_default()
        .push(site);
}

fn attrs_have_used(attrs: &[Attr]) -> bool {
    attrs.iter().any(|attr| matches!(attr, Attr::Used(_)))
}

fn unwrap_cfg(item: &Item) -> &Item {
    match item {
        Item::Cfg { item, .. } => unwrap_cfg(item),
        _ => item,
    }
}

fn collect_let_initializers<'a>(body: &'a [IndentStmt], out: &mut BTreeMap<&'a str, &'a Expr>) {
    for indent in body {
        match &indent.stmt {
            Stmt::Let {
                name,
                init: Some(init),
                ..
            } => {
                out.insert(name.as_str(), init);
            }
            Stmt::If {
                then_body,
                else_body,
                ..
            }
            | Stmt::LetIf {
                then_body,
                else_body,
                ..
            } => {
                collect_let_initializers(then_body, out);
                collect_let_initializers(else_body, out);
            }
            Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
                collect_let_initializers(body, out);
            }
            Stmt::For { body, .. } => collect_let_initializers(body, out),
            Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
                collect_let_initializers(&body.stmts, out);
            }
            Stmt::Match { arms, .. } => {
                for arm in arms {
                    collect_let_initializers(&arm.body, out);
                }
            }
            _ => {}
        }
    }
}

fn locale_arg_is_c(expr: &Expr, locals: &BTreeMap<&str, &Expr>) -> bool {
    match expr {
        Expr::Cast { expr, .. } | Expr::Ref { expr, .. } | Expr::AddrOf { expr, .. } => {
            locale_arg_is_c(expr, locals)
        }
        Expr::Var(name) => locals
            .get(name.as_str())
            .is_some_and(|init| locale_arg_is_c(init, locals)),
        Expr::Call { func, args, .. } => {
            args.is_empty()
                && matches!(
                    func.as_ref(),
                    Expr::Path(path) if path.segments.len() == 3
                        && path.segments[0].as_str() == "std"
                        && path.segments[1].as_str() == "ptr"
                        && path.segments[2].as_str() == "null_mut"
                )
        }
        Expr::MethodCall { recv, method, args } => {
            method == "as_ptr" && args.is_empty() && is_c_locale_string(recv)
        }
        _ => false,
    }
}

fn is_c_locale_string(expr: &Expr) -> bool {
    matches!(expr, Expr::CStr(bytes) if bytes == b"C" || bytes == b"POSIX")
        || matches!(expr, Expr::ByteStr(bytes) if bytes == b"C\0" || bytes == b"POSIX\0")
}

fn unused_item_refs(item: &Item) -> BTreeSet<String> {
    let mut refs = BTreeSet::new();
    unused_item_collect_item_refs(item, &mut refs);
    refs
}

fn unused_item_collect_item_refs(item: &Item, refs: &mut BTreeSet<String>) {
    match item {
        Item::Fn(f) => unused_item_fn_refs(f, refs),
        Item::Static { ty, init, .. } => {
            unused_item_collect_type_refs(ty, refs);
            unused_item_collect_expr_refs(init, refs);
        }
        Item::Const { ty, init, .. } => {
            unused_item_collect_type_refs(ty, refs);
            unused_item_collect_expr_refs(init, refs);
        }
        Item::ExternBlock { decls, .. } => {
            for decl in decls {
                match decl {
                    ExternDecl::Fn(f) => {
                        for param in &f.params {
                            unused_item_fn_param_refs(param, refs);
                        }
                        if let Some(ret) = &f.ret {
                            unused_item_collect_type_refs(ret, refs);
                        }
                    }
                    ExternDecl::Static { ty, .. } => unused_item_collect_type_refs(ty, refs),
                }
            }
        }
        Item::Struct(def) => unused_item_collect_struct_refs(def, refs),
        Item::Record(def) => unused_item_collect_record_refs(def, refs),
        Item::Impl(block) => unused_item_impl_refs(block, refs),
        Item::Cfg { item, .. } => unused_item_collect_item_refs(item, refs),
        Item::Use { path } => {
            for segment in &path.segments {
                refs.insert(segment.as_str().to_owned());
            }
        }
        Item::Enum(_)
        | Item::Macro { .. }
        | Item::Comment(_)
        | Item::CrateAttrs(_)
        | Item::Mod { .. }
        | Item::SupportModule(_) => {}
    }
}

fn unused_item_fn_refs(f: &FnDef, refs: &mut BTreeSet<String>) {
    for param in &f.params {
        unused_item_fn_param_refs(param, refs);
    }
    if let Some(ret) = &f.ret {
        unused_item_collect_type_refs(ret, refs);
    }
    unused_item_collect_body_refs(&f.body, refs);
}

fn unused_item_fn_param_refs(param: &FnParam, refs: &mut BTreeSet<String>) {
    unused_item_collect_type_refs(&param.ty, refs);
}

fn unused_item_impl_refs(block: &ImplBlock, refs: &mut BTreeSet<String>) {
    for param in &block.generics {
        unused_item_generic_param_refs(param, refs);
    }
    unused_item_collect_type_refs(&block.self_ty, refs);
    for item in &block.items {
        match item {
            ImplItem::AssocType { ty, .. } => unused_item_collect_type_refs(ty, refs),
            ImplItem::Method(method) => unused_item_method_refs(method, refs),
        }
    }
}

fn unused_item_method_refs(method: &Method, refs: &mut BTreeSet<String>) {
    for param in &method.params {
        unused_item_fn_param_refs(param, refs);
    }
    if let Some(ret) = &method.ret {
        unused_item_collect_type_refs(ret, refs);
    }
    unused_item_collect_expr_refs(&method.body, refs);
}

fn unused_item_generic_param_refs(param: &GenericParam, refs: &mut BTreeSet<String>) {
    for bound in &param.bounds {
        unused_item_trait_bound_refs(bound, refs);
    }
}

fn unused_item_trait_bound_refs(bound: &TraitBound, refs: &mut BTreeSet<String>) {
    for (_, ty) in &bound.assoc {
        unused_item_collect_type_refs(ty, refs);
    }
}

fn unused_item_collect_struct_refs(def: &StructDef, refs: &mut BTreeSet<String>) {
    for param in &def.generics {
        unused_item_generic_param_refs(param, refs);
    }
    match &def.fields {
        StructFields::Tuple(fields) => {
            for ty in fields {
                unused_item_collect_type_refs(ty, refs);
            }
        }
        StructFields::Named(fields) => {
            for (_, ty) in fields {
                unused_item_collect_type_refs(ty, refs);
            }
        }
    }
}

fn unused_item_collect_record_refs(def: &RecordDef, refs: &mut BTreeSet<String>) {
    for field in &def.fields {
        unused_item_collect_type_refs(&field.ty, refs);
    }
}

fn unused_item_collect_body_refs(body: &[IndentStmt], refs: &mut BTreeSet<String>) {
    for indent in body {
        unused_item_collect_stmt_refs(&indent.stmt, refs);
    }
}

fn unused_item_collect_block_refs(block: &Block, refs: &mut BTreeSet<String>) {
    unused_item_collect_body_refs(&block.stmts, refs);
    if let Some(tail) = &block.tail {
        unused_item_collect_expr_refs(tail, refs);
    }
}

fn unused_item_collect_stmt_refs(stmt: &Stmt, refs: &mut BTreeSet<String>) {
    match stmt {
        Stmt::Let { ty, init, .. } => {
            if let Some(ty) = ty {
                unused_item_collect_type_refs(ty, refs);
            }
            if let Some(init) = init {
                unused_item_collect_expr_refs(init, refs);
            }
        }
        Stmt::LetIf {
            ty,
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            if let Some(ty) = ty {
                unused_item_collect_type_refs(ty, refs);
            }
            unused_item_collect_expr_refs(cond, refs);
            unused_item_collect_body_refs(then_body, refs);
            unused_item_collect_expr_refs(then_value, refs);
            unused_item_collect_body_refs(else_body, refs);
            unused_item_collect_expr_refs(else_value, refs);
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            unused_item_collect_expr_refs(target, refs);
            unused_item_collect_expr_refs(value, refs);
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => unused_item_collect_expr_refs(expr, refs),
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
        Stmt::Unsafe { body } | Stmt::Block(body) => unused_item_collect_block_refs(body, refs),
        Stmt::While { cond, body } => {
            unused_item_collect_expr_refs(cond, refs);
            unused_item_collect_block_refs(body, refs);
        }
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            unused_item_collect_expr_refs(cond, refs);
            unused_item_collect_body_refs(then_body, refs);
            unused_item_collect_body_refs(else_body, refs);
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            unused_item_collect_body_refs(body, refs)
        }
        Stmt::For { iter, body, .. } => {
            unused_item_collect_expr_refs(iter, refs);
            unused_item_collect_body_refs(body, refs);
        }
        Stmt::Match { expr, arms } => {
            unused_item_collect_expr_refs(expr, refs);
            for arm in arms {
                unused_item_collect_match_arm_refs(arm, refs);
            }
        }
        Stmt::InlineAsm(_) => {}
    }
}

fn unused_item_collect_match_arm_refs(arm: &MatchArm, refs: &mut BTreeSet<String>) {
    unused_item_collect_pattern_refs(&arm.pattern, refs);
    unused_item_collect_body_refs(&arm.body, refs);
}

fn unused_item_collect_pattern_refs(pattern: &Pattern, refs: &mut BTreeSet<String>) {
    match pattern {
        Pattern::TupleStruct { name, fields } => {
            refs.insert(name.as_str().to_owned());
            for field in fields {
                unused_item_collect_pattern_refs(field, refs);
            }
        }
        Pattern::Wildcard
        | Pattern::Binding(_)
        | Pattern::I64(_)
        | Pattern::I128(_)
        | Pattern::U128(_)
        | Pattern::InclusiveRange { .. } => {}
    }
}

fn unused_item_collect_expr_refs(expr: &Expr, refs: &mut BTreeSet<String>) {
    match expr {
        Expr::Var(name) => {
            refs.insert(name.as_str().to_owned());
            unused_item_collect_layout_call_type_ref(name.as_str(), refs);
        }
        Expr::Path(path) => {
            for segment in &path.segments {
                refs.insert(segment.as_str().to_owned());
            }
        }
        Expr::Unary { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Closure { body: expr, .. }
        | Expr::AtomicNew { value: expr, .. } => unused_item_collect_expr_refs(expr, refs),
        Expr::Cast { expr, ty } => {
            unused_item_collect_expr_refs(expr, refs);
            unused_item_collect_type_refs(ty, refs);
        }
        Expr::Transmute { from, to, expr } => {
            unused_item_collect_type_refs(from, refs);
            unused_item_collect_type_refs(to, refs);
            unused_item_collect_expr_refs(expr, refs);
        }
        Expr::Block(block) | Expr::Unsafe(block) => unused_item_collect_block_refs(block, refs),
        Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. } => {
            if let Some(ptr) = place.ptr_expr() {
                unused_item_collect_expr_refs(ptr, refs);
            }
        }
        Expr::AtomicStore { place, value, .. }
        | Expr::AtomicFetch { place, value, .. }
        | Expr::AtomicSwap { place, value, .. } => {
            if let Some(ptr) = place.ptr_expr() {
                unused_item_collect_expr_refs(ptr, refs);
            }
            unused_item_collect_expr_refs(value, refs);
        }
        Expr::AtomicCompareExchange {
            place,
            expected,
            desired,
            ..
        } => {
            if let Some(ptr) = place.ptr_expr() {
                unused_item_collect_expr_refs(ptr, refs);
            }
            unused_item_collect_expr_refs(expected, refs);
            unused_item_collect_expr_refs(desired, refs);
        }
        Expr::Binary { lhs, rhs, .. }
        | Expr::Range {
            start: lhs,
            end: rhs,
        }
        | Expr::Index {
            base: lhs,
            index: rhs,
        } => {
            unused_item_collect_expr_refs(lhs, refs);
            unused_item_collect_expr_refs(rhs, refs);
        }
        Expr::Call { func, args, .. } => {
            unused_item_collect_expr_refs(func, refs);
            for arg in args {
                unused_item_collect_expr_refs(arg, refs);
            }
        }
        Expr::MethodCall { recv, args, .. } => {
            unused_item_collect_expr_refs(recv, refs);
            for arg in args {
                unused_item_collect_expr_refs(arg, refs);
            }
        }
        Expr::MethodCallGeneric {
            recv,
            type_args,
            args,
            ..
        } => {
            unused_item_collect_expr_refs(recv, refs);
            for ty in type_args {
                unused_item_collect_type_refs(ty, refs);
            }
            for arg in args {
                unused_item_collect_expr_refs(arg, refs);
            }
        }
        Expr::Field { base, .. }
        | Expr::TupleField { base, .. }
        | Expr::ArrayPtr { array: base, .. } => unused_item_collect_expr_refs(base, refs),
        Expr::StructLit { name, fields } => {
            refs.insert(name.clone());
            for (_, value) in fields {
                unused_item_collect_expr_refs(value, refs);
            }
        }
        Expr::TupleStructLit { name, fields } => {
            refs.insert(name.clone());
            for value in fields {
                unused_item_collect_expr_refs(value, refs);
            }
        }
        Expr::ArrayLit(elems) | Expr::VecLit(elems) => {
            for elem in elems {
                unused_item_collect_expr_refs(elem, refs);
            }
        }
        Expr::ArrayRepeat { elem, .. } => unused_item_collect_expr_refs(elem, refs),
        Expr::VecRepeat { elem, len } => {
            unused_item_collect_expr_refs(elem, refs);
            unused_item_collect_expr_refs(len, refs);
        }
        Expr::Macro { name, args } => {
            refs.insert(name.clone());
            for arg in args {
                unused_item_collect_expr_refs(arg, refs);
            }
        }
        Expr::Match { expr, arms } => {
            unused_item_collect_expr_refs(expr, refs);
            for arm in arms {
                unused_item_collect_pattern_refs(&arm.pattern, refs);
                unused_item_collect_expr_refs(&arm.value, refs);
            }
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            unused_item_collect_expr_refs(cond, refs);
            unused_item_collect_expr_refs(then_expr, refs);
            unused_item_collect_expr_refs(else_expr, refs);
        }
        Expr::CopyNonoverlapping { src, dst, .. } => {
            unused_item_collect_expr_refs(src, refs);
            unused_item_collect_expr_refs(dst, refs);
        }
        Expr::PtrCopy {
            src, dst, count, ..
        } => {
            unused_item_collect_expr_refs(src, refs);
            unused_item_collect_expr_refs(dst, refs);
            unused_item_collect_expr_refs(count, refs);
        }
        Expr::WriteBytes { dst, val, count } => {
            unused_item_collect_expr_refs(dst, refs);
            unused_item_collect_expr_refs(val, refs);
            unused_item_collect_expr_refs(count, refs);
        }
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::CStr(_)
        | Expr::Todo(_)
        | Expr::AtomicFence { .. } => {}
    }
}

fn unused_item_collect_layout_call_type_ref(name: &str, refs: &mut BTreeSet<String>) {
    let Some(ty) = name
        .strip_prefix("std::mem::size_of::<")
        .or_else(|| name.strip_prefix("std::mem::align_of::<"))
        .and_then(|rest| rest.strip_suffix('>'))
    else {
        return;
    };
    refs.insert(ty.to_string());
}

fn unused_item_collect_type_refs(ty: &Type, refs: &mut BTreeSet<String>) {
    match ty {
        Type::Custom(name) => {
            refs.insert(name.clone());
        }
        Type::TyVar(name) => {
            refs.insert(name.as_str().to_owned());
        }
        Type::Generic { name, args } => {
            refs.insert(name.clone());
            for arg in args {
                unused_item_collect_type_refs(arg, refs);
            }
        }
        Type::Complex(inner)
        | Type::Slice(inner)
        | Type::Ptr { inner, .. }
        | Type::Ref { inner, .. } => unused_item_collect_type_refs(inner, refs),
        Type::Array { elem, .. } => unused_item_collect_type_refs(elem, refs),
        Type::FnPtr { params, ret, .. } => {
            for param in params {
                unused_item_collect_type_refs(param, refs);
            }
            unused_item_collect_type_refs(ret, refs);
        }
        Type::Prim(_)
        | Type::LongDouble
        | Type::CLib(_)
        | Type::VaList
        | Type::Str
        | Type::Unit
        | Type::Variadic
        | Type::Never => {}
    }
}

fn trivial_unsafe_site(program: &Program, call: &ExprSite) -> Option<ExprSite> {
    let path = &call.path.0;
    if !path.ends_with(&[PathSegment::UnsafeBody, PathSegment::BlockTail]) {
        return None;
    }
    let parent = expression_site(call.item_index, &path[..path.len() - 2]);
    let Expr::Unsafe(block) = walk::target_expr_at_path(program, parent.item_index, &parent.path)?
    else {
        return None;
    };
    (block.stmts.is_empty() && block.tail.is_some()).then_some(parent)
}

fn exporting_attr(attr: &Attr) -> bool {
    matches!(
        attr,
        Attr::NoMangle
            | Attr::WeakLinkage
            | Attr::ExternWeakLinkage
            | Attr::Used(_)
            | Attr::LinkSection(_)
    )
}

fn index_item_uses(item: &Item, item_index: usize, uses: &mut BTreeMap<String, Vec<usize>>) {
    let mut index_expr = |expr: &Expr| index_expr_use(expr, item_index, uses);
    match item {
        Item::Fn(function) => walk::body_exprs(&function.body, &mut index_expr),
        Item::Static { init, .. } | Item::Const { init, .. } => walk::exprs(init, &mut index_expr),
        Item::Impl(block) => {
            for item in &block.items {
                if let ImplItem::Method(method) = item {
                    walk::exprs(&method.body, &mut index_expr);
                }
            }
        }
        Item::Macro { args, .. } => {
            for arg in args {
                walk::exprs(arg, &mut index_expr);
            }
        }
        Item::Cfg { item, .. } => index_item_uses(item, item_index, uses),
        Item::Use { path } => {
            for segment in &path.segments {
                uses.entry(segment.as_str().to_owned())
                    .or_default()
                    .push(item_index);
            }
        }
        Item::Comment(_)
        | Item::CrateAttrs(_)
        | Item::Mod { .. }
        | Item::ExternBlock { .. }
        | Item::Enum(_)
        | Item::Record(_)
        | Item::Struct(_)
        | Item::SupportModule(_) => {}
    }
}

fn item_use_domain_complete(item: &Item) -> bool {
    match item {
        Item::Cfg { item, .. } => item_use_domain_complete(item),
        _ => true,
    }
}

fn index_expr_use(expr: &Expr, item_index: usize, uses: &mut BTreeMap<String, Vec<usize>>) {
    match expr {
        Expr::Var(name) => uses
            .entry(name.as_str().to_owned())
            .or_default()
            .push(item_index),
        Expr::Path(path) => uses
            .entry(qualified_path(path))
            .or_default()
            .push(item_index),
        _ => {}
    }
}

fn qualified_path(path: &crate::rust_ast::Path) -> String {
    path.segments
        .iter()
        .map(crate::rust_ast::Ident::as_str)
        .collect::<Vec<_>>()
        .join("::")
}

fn ptr_len_plans_from_facts(slices: &[PtrLenSliceFact], facts: &FixupFacts) -> Vec<PtrLenPlan> {
    let mut grouped = BTreeMap::<(FunctionId, BindingId), Vec<&PtrLenSliceFact>>::new();
    for fact in slices {
        grouped
            .entry((fact.callee, fact.ptr_param))
            .or_default()
            .push(fact);
    }
    let mut plans = Vec::new();
    for ((function, ptr_param), calls) in grouped {
        let Some(function_fact) = facts.functions.iter().find(|fact| fact.id == function) else {
            continue;
        };
        let Some(ptr_binding) = facts
            .bindings
            .iter()
            .find(|binding| binding.id == ptr_param)
        else {
            continue;
        };
        let BindingKind::Param { index: ptr_index } = ptr_binding.kind else {
            continue;
        };
        let mutable = calls.iter().any(|call| call.mutable);
        plans.push(PtrLenPlan {
            item_index: function_fact.item_index,
            function_name: function_fact.name.clone(),
            ptr_index,
            ptr_name: ptr_binding.name.clone(),
            mutable,
            elem: calls[0].elem_ty.clone(),
        });
    }
    plans
}

pub(super) fn default_value(ty: &Type) -> Expr {
    match ty {
        Type::Prim(Prim::Bool) => Expr::Value(RustValue::Bool(false)),
        Type::Prim(Prim::F32 | Prim::F64) => Expr::Value(RustValue::Float(0.0.into())),
        Type::Prim(Prim::F128) => Expr::HexFloat("0.0f128".into()),
        Type::Ptr { .. } => Expr::Value(RustValue::NullPtr),
        _ => Expr::Value(RustValue::I64(0)),
    }
}

fn definition_evidence_site(definition: &DefinitionSite) -> ExprSite {
    ExprSite {
        item_index: definition.location.item_index(),
        path: AstPath(Vec::new()),
        fact_path: AstPath(Vec::new()),
    }
}

fn expression_site(item_index: usize, path: &[PathSegment]) -> ExprSite {
    let path = AstPath(path.to_vec());
    ExprSite {
        item_index,
        fact_path: fact_path(&path),
        path,
    }
}

fn statement_evidence_site(statement: &StatementRef) -> ExprSite {
    expression_site(statement.item_index, &statement.path.0)
}

fn child_site(parent: &ExprSite, index: usize) -> ExprSite {
    let mut path = parent.path.clone();
    path.0.push(PathSegment::Expr(index));
    ExprSite {
        item_index: parent.item_index,
        fact_path: fact_path(&path),
        path,
    }
}

fn fact_path(path: &AstPath) -> AstPath {
    let mut fact = path.0.clone();
    if let Some(stmt) = fact
        .iter()
        .rposition(|segment| matches!(segment, PathSegment::Stmt(_)))
        && matches!(fact.get(stmt + 1), Some(PathSegment::Expr(_)))
    {
        fact.remove(stmt + 1);
    } else if let Some(stmt) = fact
        .iter()
        .rposition(|segment| matches!(segment, PathSegment::Stmt(_)))
        && matches!(
            fact.get(stmt + 1..stmt + 3),
            Some([PathSegment::Then | PathSegment::Else, PathSegment::Expr(0)])
        )
    {
        fact.remove(stmt + 2);
    }
    AstPath(fact)
}

fn coarse_def_use_path(path: &AstPath) -> AstPath {
    AstPath(
        path.0
            .iter()
            .filter(|segment| !matches!(segment, PathSegment::Expr(_)))
            .cloned()
            .collect(),
    )
}

fn merge_binding_use(uses: &mut Vec<BindingUse>, site: UseSiteRef, access: BindingAccess) {
    if let Some(existing) = uses.iter_mut().find(|usage| usage.site == site) {
        existing.access = match (existing.access, access) {
            (BindingAccess::Read, BindingAccess::Write)
            | (BindingAccess::Write, BindingAccess::Read) => BindingAccess::ReadWrite,
            (existing, _) => existing,
        };
    } else {
        uses.push(BindingUse { site, access });
    }
}

#[expect(
    dead_code,
    reason = "query API surface not yet wired into a fixup rule"
)]
fn rust_value_type(value: &RustValue) -> Option<Type> {
    match value {
        RustValue::I64(_) => Some(Type::Prim(Prim::I64)),
        RustValue::Usize(_) => Some(Type::Prim(Prim::Usize)),
        RustValue::I128(_) => Some(Type::Prim(Prim::I128)),
        RustValue::U128(_) => Some(Type::Prim(Prim::U128)),
        RustValue::Float(_) => Some(Type::Prim(Prim::F64)),
        RustValue::Bool(_) => Some(Type::Prim(Prim::Bool)),
        RustValue::None | RustValue::NullPtr => None,
    }
}

fn call_target(func: &Expr, binding: &CallBinding) -> CallTarget {
    let name = match func {
        Expr::Var(name) => Some(name.as_str()),
        _ => None,
    };
    match binding {
        CallBinding::Generated => name
            .map(|name| CallTarget::Generated(name.to_string()))
            .unwrap_or(CallTarget::Indirect),
        CallBinding::Direct {
            identity: FunctionIdentity::Known(known),
            ..
        } => CallTarget::Known(*known),
        CallBinding::Direct { .. } => name
            .map(|name| CallTarget::Direct(name.to_string()))
            .unwrap_or(CallTarget::Indirect),
        CallBinding::Indirect => CallTarget::Indirect,
    }
}

fn stable_pointer_source(expr: &Expr) -> Option<(&str, bool)> {
    match expr {
        Expr::Cast { expr, .. } => stable_pointer_source(expr),
        Expr::ArrayPtr { array, mutable } => {
            let Expr::Var(name) = &**array else {
                return None;
            };
            Some((name.as_str(), *mutable))
        }
        _ => {
            let (method, recv) = pointer_method().matches(expr, &())?;
            let Expr::Var(name) = recv else {
                return None;
            };
            Some((name.as_str(), method == "as_mut_ptr"))
        }
    }
}

fn pointer_method() -> NullaryMethodCall {
    NullaryMethodCall::one_of(&["as_ptr", "as_mut_ptr"])
}

fn count_matches_source_len(source: &ByteSource<'_>, count: &Expr) -> bool {
    match count {
        Expr::Cast { expr, .. } => count_matches_source_len(source, expr),
        _ => NullaryMethodCall::named("len")
            .matches(count, &())
            .is_some_and(|(_, recv)| matches_source_expr(source, recv)),
    }
}

fn matches_source_expr(source: &ByteSource<'_>, expr: &Expr) -> bool {
    match expr {
        Expr::Var(name) => name.as_str() == source.name,
        _ => {
            source.representation == ByteRepresentation::Str
                && NullaryMethodCall::named("as_bytes")
                    .matches(expr, &())
                    .is_some_and(|(_, recv)| matches_source_expr(source, recv))
        }
    }
}

fn representation_for_buffer(kind: StringBufferKind) -> ByteRepresentation {
    match kind {
        StringBufferKind::CharArray => ByteRepresentation::Collection,
        StringBufferKind::BorrowedBytes => ByteRepresentation::Bytes,
        StringBufferKind::BorrowedCStr => ByteRepresentation::CStr,
        StringBufferKind::BorrowedStr | StringBufferKind::OwnedString => ByteRepresentation::Str,
    }
}

fn representation_for_type(ty: &Type) -> Option<ByteRepresentation> {
    match ty.peel_aligned() {
        Type::Array { elem, .. } | Type::Slice(elem)
            if matches!(&**elem, Type::Prim(Prim::I8 | Prim::U8)) =>
        {
            Some(ByteRepresentation::Collection)
        }
        Type::Ref { inner, .. } => match &**inner {
            Type::Slice(elem) if matches!(&**elem, Type::Prim(Prim::I8 | Prim::U8)) => {
                Some(ByteRepresentation::Bytes)
            }
            Type::Str => Some(ByteRepresentation::Str),
            Type::Custom(name) if name == "core::ffi::CStr" => Some(ByteRepresentation::CStr),
            _ => None,
        },
        Type::Custom(name) if name == "String" => Some(ByteRepresentation::Str),
        _ => None,
    }
}

fn byte_extent(ty: &Type) -> ByteExtent {
    match ty.peel_aligned() {
        Type::Array { len, elem } if matches!(&**elem, Type::Prim(Prim::I8 | Prim::U8)) => {
            usize::try_from(*len)
                .map(ByteExtent::Constant)
                .unwrap_or(ByteExtent::Dynamic)
        }
        _ => ByteExtent::Dynamic,
    }
}

fn item_type_name(item: &Item) -> Option<&str> {
    match item {
        Item::Record(record) => Some(record.name.as_str()),
        Item::Struct(record) => Some(record.name.as_str()),
        Item::Enum(record) => Some(record.name.as_str()),
        Item::Cfg { item, .. } => item_type_name(item),
        _ => None,
    }
}

fn static_item_index(program: &Program, name: &str) -> Option<usize> {
    program.items.iter().position(
        |item| matches!(item, Item::Static { name: static_name, .. } if static_name == name),
    )
}

fn static_exists(items: &[Item], name: &str) -> bool {
    items.iter().any(|item| match item {
        Item::Static {
            name: static_name, ..
        } => static_name == name,
        Item::Cfg { item, .. } => static_exists(std::slice::from_ref(item), name),
        _ => false,
    })
}
