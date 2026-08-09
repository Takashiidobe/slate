use crate::fixups::facts::{
    self, AnonymousStructFact, ArrayElementPointerOriginFact, AsciiNumericStringFact, AstPath,
    AtomicGlobalFact, AtomicLocalFact, BindingFact, BindingId, BindingTypeFact, BorrowAliasFact,
    BorrowAliasReason, BorrowAliasState, BufferPointerFieldFact, CStringLiteralFact, CallArgFact,
    CallSignatureFact, CalleeAllocSummaryFact, CallsiteFact, CastFact, ControlFlowFact,
    ControlFlowSubject, CountedLoopFact, CountedSliceLoopFact, DefUseFact, EffectFact,
    EffectSubject, FileOwnershipFact, FunctionFact, FunctionId, HeapOwnershipFact,
    InterproceduralAllocCallerFact, InterproceduralAllocEligibilityFact, LazyInitSingletonFact,
    LoopFact, NullCheckDominanceFact, OptionBoxComparison, OptionBoxLocalCandidate, PlaceFact,
    PointerComparisonFact, PointerOptionSafetyFact, PrintfCallFact, PtrLenSliceFact, SignatureId,
    StaticDeclFact, StringCopyRewriteFact, StringLiftPlanFact, StringParamLiftFact,
    StringRecoveryCandidate, StructFieldOwnershipFact, ValueFact,
};
use crate::fixups::query::{
    AnonymousStructSet, AtomicPromotionSet, BufferPointerFields, ByteSource, ByteView, CallRecord,
    DefinitionGroup, DefinitionGroupUsers, DefinitionSite, DefinitionUsers, ExprSite,
    FileOwnershipFacts, FunctionRef, HeapOwnershipFacts, InterproceduralAllocCallerInput,
    LazySingletonSet, NulPosition, OptionBoxComparisonInput, OptionBoxLocalPlanInput,
    PtrLenPlanSet, QueryResult, ReferenceDomain, SliceLoopFact, StableExpr, StatementRef,
};
use crate::rust_ast::{Expr, FnDef, Item, Program};
use salsa::Setter;
use std::collections::{BTreeMap, BTreeSet};

#[salsa::db]
pub(in crate::fixups) trait FixupDb: salsa::Database {}

#[salsa::db]
#[derive(Default)]
pub(in crate::fixups) struct Database {
    storage: salsa::Storage<Self>,
}

#[salsa::db]
impl salsa::Database for Database {}

#[salsa::db]
impl FixupDb for Database {}

#[salsa::input(singleton)]
pub(in crate::fixups) struct ProgramInput {
    #[returns(ref)]
    pub(in crate::fixups) program: Program,
}

#[salsa::interned]
pub(in crate::fixups) struct FunctionInput<'db> {
    #[returns(copy)]
    pub(in crate::fixups) program: ProgramInput,
    #[returns(copy)]
    pub(in crate::fixups) function: FunctionId<'db>,
}

#[salsa::tracked]
impl<'db> ProgramInput {
    #[salsa::tracked(returns(ref))]
    pub(in crate::fixups) fn base_walk(self, db: &dyn FixupDb) -> facts::walk::BaseWalk {
        facts::walk::BaseWalk::new(db, self.program(db))
    }

    #[salsa::tracked(returns(ref))]
    pub(in crate::fixups) fn functions(self, db: &'db dyn FixupDb) -> Vec<FunctionInput<'db>> {
        self.base_walk(db)
            .function_facts()
            .into_iter()
            .map(|fact| FunctionInput::new(db, self, fact.id))
            .collect()
    }

    #[salsa::tracked(returns(ref))]
    fn extern_call_signatures(self, db: &'db dyn FixupDb) -> Vec<CallSignatureFact<'db>> {
        facts::calls::collect_extern_signatures(self.program(db))
    }

    #[salsa::tracked(returns(ref))]
    fn statics(self, db: &dyn FixupDb) -> Vec<StaticDeclFact> {
        self.program(db)
            .items
            .iter()
            .filter_map(|item| match item {
                Item::Static {
                    name,
                    mutable,
                    ty,
                    init,
                    ..
                } => Some(StaticDeclFact {
                    name: name.clone(),
                    mutable: *mutable,
                    ty: ty.clone(),
                    init: init.clone(),
                }),
                _ => None,
            })
            .collect()
    }

    #[salsa::tracked(returns(ref))]
    fn union_records(self, db: &dyn FixupDb) -> BTreeSet<String> {
        self.program(db)
            .items
            .iter()
            .filter_map(|item| match item {
                Item::Record(record) if record.is_union => Some(record),
                _ => None,
            })
            .map(|record| record.name.clone())
            .collect()
    }

    #[salsa::tracked(returns(ref))]
    pub(in crate::fixups) fn struct_field_ownership(
        self,
        db: &dyn FixupDb,
    ) -> Vec<StructFieldOwnershipFact> {
        facts::struct_field_ownership::collect(self.program(db).items.iter().filter_map(|item| {
            match item {
                Item::Record(record) => Some(record),
                _ => None,
            }
        }))
    }

    #[salsa::tracked(returns(ref))]
    pub(in crate::fixups) fn anonymous_structs(self, db: &dyn FixupDb) -> Vec<AnonymousStructFact> {
        facts::anonymous_structs::collect(self.program(db).items.iter().filter_map(
            |item| match item {
                Item::Record(record) => Some(record),
                _ => None,
            },
        ))
    }
}

#[salsa::tracked]
impl<'db> FunctionInput<'db> {
    fn function_id(self, db: &'db dyn FixupDb) -> FunctionId<'db> {
        self.function(db)
    }

    #[salsa::tracked(returns(ref))]
    pub(in crate::fixups) fn body(self, db: &dyn FixupDb) -> FnDef {
        let name = self.function(db).name(db);
        self.program(db)
            .program(db)
            .items
            .iter()
            .find_map(|item| match item {
                Item::Fn(body) if body.name == *name => Some(body.clone()),
                _ => None,
            })
            .expect("interned function must exist in the program")
    }

    #[salsa::tracked(returns(ref))]
    pub(in crate::fixups) fn bindings_typed(self, db: &'db dyn FixupDb) -> Vec<BindingFact<'db>> {
        self.program(db)
            .base_walk(db)
            .binding_facts()
            .into_iter()
            .filter(|fact| fact.function == self.function(db))
            .collect()
    }

    #[salsa::tracked(returns(ref))]
    fn binding_ids_by_name(self, db: &'db dyn FixupDb) -> BTreeMap<String, Vec<BindingId<'db>>> {
        let mut by_name = BTreeMap::<String, Vec<BindingId<'db>>>::new();
        for binding in self.bindings_typed(db) {
            by_name
                .entry(binding.name.clone())
                .or_default()
                .push(binding.id);
        }
        by_name
    }

    #[salsa::tracked(returns(ref))]
    pub(in crate::fixups) fn binding_types_typed(
        self,
        db: &'db dyn FixupDb,
    ) -> Vec<BindingTypeFact<'db>> {
        let bindings: BTreeSet<_> = self.bindings_typed(db).iter().map(|fact| fact.id).collect();
        self.program(db)
            .base_walk(db)
            .binding_type_facts()
            .into_iter()
            .filter(|fact| bindings.contains(&fact.binding))
            .collect()
    }

    #[salsa::tracked(returns(ref))]
    fn loops_typed(self, db: &'db dyn FixupDb) -> Vec<LoopFact<'db>> {
        self.program(db)
            .base_walk(db)
            .loop_facts()
            .into_iter()
            .filter(|fact| fact.function == self.function(db))
            .collect()
    }

    #[salsa::tracked(returns(ref))]
    fn def_use(self, db: &dyn FixupDb) -> Vec<DefUseFact<'db>> {
        facts::def_use::collect_for_function(
            self.function_id(db),
            self.body(db),
            self.bindings_typed(db),
        )
    }

    #[salsa::tracked(returns(ref))]
    fn def_use_by_binding(self, db: &dyn FixupDb) -> BTreeMap<BindingId<'db>, DefUseFact<'db>> {
        self.def_use(db)
            .iter()
            .map(|fact| (fact.binding, fact.clone()))
            .collect()
    }

    #[salsa::tracked(returns(ref))]
    pub(in crate::fixups) fn effects(self, db: &dyn FixupDb) -> Vec<EffectFact<'db>> {
        facts::effects::collect_for_function(self.function_id(db), self.body(db))
    }

    #[salsa::tracked(returns(ref))]
    fn effects_by_key(
        self,
        db: &dyn FixupDb,
    ) -> BTreeMap<(EffectSubject, AstPath), EffectFact<'db>> {
        self.effects(db)
            .iter()
            .map(|fact| ((fact.subject, fact.site.path.clone()), fact.clone()))
            .collect()
    }

    #[salsa::tracked(returns(ref))]
    pub(in crate::fixups) fn values(self, db: &dyn FixupDb) -> Vec<ValueFact<'db>> {
        facts::values::collect_for_function(
            self.function_id(db),
            self.body(db),
            self.bindings_typed(db),
        )
    }

    #[salsa::tracked(returns(ref))]
    pub(in crate::fixups) fn strings(self, db: &dyn FixupDb) -> facts::strings::Collected<'db> {
        facts::strings::collect_for_function(
            self.function_id(db),
            self.body(db),
            self.bindings_typed(db),
            self.binding_types_typed(db),
        )
    }

    #[salsa::tracked(returns(ref))]
    pub(in crate::fixups) fn counted_loops(
        self,
        db: &dyn FixupDb,
    ) -> (Vec<CountedLoopFact<'db>>, Vec<CountedSliceLoopFact<'db>>) {
        facts::counted_loop::collect_for_function(
            self.function_id(db),
            self.body(db),
            self.bindings_typed(db),
            self.loops_typed(db),
        )
    }

    #[salsa::tracked(returns(ref))]
    fn local_call_signature(self, db: &dyn FixupDb) -> CallSignatureFact<'db> {
        facts::calls::local_call_signature(self.function_id(db), self.body(db))
    }

    #[salsa::tracked(returns(ref))]
    fn own_callsites(
        self,
        db: &dyn FixupDb,
        all_functions: ProgramInput,
        definitions: ProgramInput,
    ) -> Vec<CallsiteFact<'db>> {
        let function = self.function_id(db);
        all_functions
            .callsites(db, definitions)
            .iter()
            .filter(|fact| fact.site.function == function)
            .cloned()
            .collect()
    }

    #[salsa::tracked(returns(ref))]
    fn casts(
        self,
        db: &dyn FixupDb,
        all_functions: ProgramInput,
        definitions: ProgramInput,
    ) -> Vec<CastFact<'db>> {
        let function = self.function_id(db);
        let functions = vec![FunctionFact {
            id: function,
            name: self.body(db).name.clone(),
            item_index: 0,
        }];
        let call_signatures = vec![self.local_call_signature(db).clone()];
        let bindings = self.bindings_typed(db);
        facts::casts::collect_for_function(
            function,
            self.body(db),
            bindings,
            &functions,
            &call_signatures,
            self.own_callsites(db, all_functions, definitions),
        )
    }

    #[salsa::tracked(returns(ref))]
    fn places(self, db: &dyn FixupDb) -> Vec<PlaceFact<'db>> {
        facts::places::collect_for_function(self.function_id(db), self.body(db))
    }

    #[salsa::tracked(returns(ref))]
    fn control_flow(self, db: &dyn FixupDb) -> Vec<ControlFlowFact<'db>> {
        facts::control_flow::collect_for_function(self.function_id(db), self.body(db))
    }

    #[salsa::tracked(returns(ref))]
    fn borrow_alias(self, db: &dyn FixupDb) -> Vec<BorrowAliasFact<'db>> {
        facts::borrow_alias::collect_for_function(
            self.function_id(db),
            self.body(db),
            self.bindings_typed(db),
        )
    }

    #[salsa::tracked(returns(ref))]
    fn array_element_pointer_origins(
        self,
        db: &dyn FixupDb,
    ) -> Vec<ArrayElementPointerOriginFact<'db>> {
        facts::array_element_pointer_origin::collect_for_function(
            self.function_id(db),
            self.body(db),
            self.bindings_typed(db),
            self.binding_types_typed(db),
            self.def_use(db),
        )
    }

    #[salsa::tracked(returns(ref))]
    fn null_check_dominance(self, db: &dyn FixupDb) -> Vec<NullCheckDominanceFact<'db>> {
        facts::null_check_dominance::collect_for_function(
            self.function_id(db),
            self.body(db),
            self.bindings_typed(db),
            self.control_flow(db),
        )
    }

    #[salsa::tracked(returns(ref))]
    pub(in crate::fixups) fn atomic_locals(self, db: &dyn FixupDb) -> Vec<AtomicLocalFact<'db>> {
        facts::atomic_locals::collect_for_function(self.function_id(db), self.body(db))
    }

    #[salsa::tracked(returns(ref))]
    fn pointer_option_safety(
        self,
        db: &dyn FixupDb,
        definitions: ProgramInput,
    ) -> (
        Vec<PointerOptionSafetyFact<'db>>,
        Vec<PointerComparisonFact<'db>>,
    ) {
        facts::pointer_option_safety::collect_for_function(
            self.function_id(db),
            self.body(db),
            self.bindings_typed(db),
            self.binding_types_typed(db),
            definitions.union_records(db),
        )
    }

    #[salsa::tracked(returns(ref))]
    pub(in crate::fixups) fn option_box(
        self,
        db: &dyn FixupDb,
    ) -> (
        Vec<OptionBoxLocalCandidate<'db>>,
        Vec<OptionBoxComparison<'db>>,
    ) {
        facts::option_box_locals::collect_for_function(
            self.function_id(db),
            self.body(db),
            self.bindings_typed(db),
        )
    }

    #[salsa::tracked(returns(ref))]
    pub(in crate::fixups) fn buffer_pointer_fields(
        self,
        db: &dyn FixupDb,
    ) -> Vec<BufferPointerFieldFact<'db>> {
        facts::buffer_cursor::collect_for_function(
            self.function_id(db),
            &self.body(db).body,
            self.bindings_typed(db),
            self.binding_types_typed(db),
        )
    }

    #[salsa::tracked(returns(ref))]
    pub(in crate::fixups) fn heap_ownership(self, db: &dyn FixupDb) -> Vec<HeapOwnershipFact<'db>> {
        facts::heap_ownership::collect_for_function(
            self.function_id(db),
            &self.body(db).body,
            self.bindings_typed(db),
        )
    }

    #[salsa::tracked(returns(ref))]
    pub(in crate::fixups) fn file_ownership(self, db: &dyn FixupDb) -> Vec<FileOwnershipFact<'db>> {
        facts::file_ownership::collect_for_function(
            self.function_id(db),
            &self.body(db).body,
            self.bindings_typed(db),
        )
    }

    #[salsa::tracked(returns(ref))]
    fn printf_calls(self, db: &dyn FixupDb) -> Vec<PrintfCallFact<'db>> {
        let strings = self.strings(db);
        facts::printf::collect_for_function(
            self.function_id(db),
            self.body(db),
            &strings.buffers,
            &strings.pointer_views,
        )
    }

    #[salsa::tracked(returns(ref))]
    fn ascii_numeric_strings(self, db: &dyn FixupDb) -> Vec<AsciiNumericStringFact<'db>> {
        facts::strings::collect_ascii_numeric_strings(&self.strings(db).buffers)
    }

    #[salsa::tracked(returns(ref))]
    pub(in crate::fixups) fn callee_alloc_summary(
        self,
        db: &dyn FixupDb,
    ) -> Option<CalleeAllocSummaryFact<'db>> {
        facts::callee_alloc_summary::collect_for_function(self.function_id(db), self.body(db))
    }

    #[salsa::tracked(returns(ref))]
    fn ptr_len_candidates(self, db: &dyn FixupDb) -> Vec<facts::ptr_len::Candidate<'db>> {
        facts::ptr_len::candidates_for_function(
            self.function_id(db),
            self.body(db),
            self.bindings_typed(db),
        )
    }

    #[salsa::tracked(returns(ref))]
    fn string_param_candidates(
        self,
        db: &dyn FixupDb,
    ) -> Vec<facts::string_params::Candidate<'db>> {
        facts::string_params::candidates_for_function(
            self.function_id(db),
            self.body(db),
            self.bindings_typed(db),
        )
    }

    #[salsa::tracked(returns(ref))]
    fn lazy_singleton_shape(self, db: &dyn FixupDb) -> Option<(String, String, Expr)> {
        facts::lazy_singleton::match_body(&self.body(db).body)
    }

    #[salsa::tracked(returns(ref))]
    fn c_string_literals(self, db: &dyn FixupDb) -> Vec<CStringLiteralFact<'db>> {
        facts::c_strings::collect_for_function(self.function_id(db), self.body(db))
    }

    #[salsa::tracked(returns(ref))]
    fn string_rewrite_facts(
        self,
        db: &dyn FixupDb,
        all_functions: ProgramInput,
        definitions: ProgramInput,
    ) -> (
        Vec<StringLiftPlanFact<'db>>,
        Vec<StringCopyRewriteFact<'db>>,
    ) {
        let strings = self.strings(db);
        let bindings = self.bindings_typed(db);
        let snapshot = facts::strings::RewriteSnapshot {
            bindings,
            def_use: self.def_use(db),
            values: self.values(db),
            string_buffers: &strings.buffers,
            string_pointer_views: &strings.pointer_views,
            string_libc_uses: &strings.libc_uses,
            printf_calls: self.printf_calls(db),
            callsites: all_functions.callsites(db, definitions),
            string_param_lifts: all_functions.string_param_lifts(db, definitions),
        };
        facts::strings::collect_rewrite_facts_for_function(
            self.function_id(db),
            self.body(db),
            &snapshot,
        )
    }
}

#[salsa::tracked]
impl<'db> ProgramInput {
    #[salsa::tracked(returns(ref))]
    pub(in crate::fixups) fn atomic_locals(self, db: &dyn FixupDb) -> Vec<AtomicLocalFact<'db>> {
        self.functions(db)
            .iter()
            .flat_map(|&input| input.atomic_locals(db).iter().cloned())
            .collect()
    }

    #[salsa::tracked(returns(ref))]
    pub(in crate::fixups) fn interprocedural_alloc(
        self,
        db: &dyn FixupDb,
    ) -> (
        Vec<InterproceduralAllocEligibilityFact<'db>>,
        Vec<InterproceduralAllocCallerFact<'db>>,
    ) {
        let bindings: Vec<Vec<BindingFact<'db>>> = self
            .functions(db)
            .iter()
            .map(|&input| input.bindings_typed(db).clone())
            .collect();
        let functions: Vec<facts::interprocedural_alloc_eligibility::FunctionSummary> = self
            .functions(db)
            .iter()
            .zip(&bindings)
            .map(
                |(&input, bindings)| facts::interprocedural_alloc_eligibility::FunctionSummary {
                    id: input.function_id(db),
                    name: input.body(db).name.as_str(),
                    body: &input.body(db).body,
                    bindings,
                    callee_alloc_summary: input.callee_alloc_summary(db).as_ref(),
                },
            )
            .collect();
        facts::interprocedural_alloc_eligibility::collect(&functions)
    }

    #[salsa::tracked(returns(ref))]
    fn call_signature_table(
        self,
        db: &dyn FixupDb,
        definitions: ProgramInput,
    ) -> (Vec<CallSignatureFact<'db>>, BTreeMap<String, SignatureId>) {
        let mut signatures: Vec<CallSignatureFact<'db>> = self
            .functions(db)
            .iter()
            .map(|&input| input.local_call_signature(db).clone())
            .collect();
        signatures.extend(definitions.extern_call_signatures(db).iter().cloned());
        for (index, signature) in signatures.iter_mut().enumerate() {
            signature.id = SignatureId(index);
        }
        let by_name = signatures
            .iter()
            .map(|signature| (signature.name.clone(), signature.id))
            .collect();
        (signatures, by_name)
    }

    #[salsa::tracked(returns(ref))]
    fn callsites(self, db: &dyn FixupDb, definitions: ProgramInput) -> Vec<CallsiteFact<'db>> {
        let (signatures, by_name) = self.call_signature_table(db, definitions);
        let mut all = Vec::new();
        for &input in self.functions(db) {
            all.extend(facts::calls::collect_callsites_for_function(
                input.function_id(db),
                &input.body(db).body,
                input.bindings_typed(db),
                signatures,
                by_name,
            ));
        }
        all
    }

    #[salsa::tracked(returns(ref))]
    pub(in crate::fixups) fn ptr_len_slices(
        self,
        db: &dyn FixupDb,
        definitions: ProgramInput,
    ) -> Vec<PtrLenSliceFact<'db>> {
        let bodies: facts::walk::Bodies = self
            .functions(db)
            .iter()
            .map(|&input| (input.function_id(db), input.body(db)))
            .collect();
        let candidates: Vec<facts::ptr_len::Candidate> = self
            .functions(db)
            .iter()
            .flat_map(|&input| input.ptr_len_candidates(db).iter().cloned())
            .collect();
        let mut bindings = Vec::new();
        let mut binding_types = Vec::new();
        let mut def_use = Vec::new();
        let mut function_by_name = BTreeMap::new();
        for &input in self.functions(db) {
            bindings.extend(input.bindings_typed(db).iter().cloned());
            binding_types.extend(input.binding_types_typed(db).iter().cloned());
            def_use.extend(input.def_use(db).iter().cloned());
            function_by_name.insert(input.body(db).name.clone(), input.function_id(db));
        }
        let callsites = self.callsites(db, definitions);
        let snapshot = facts::ptr_len::Snapshot {
            bindings: &bindings,
            binding_types: &binding_types,
            def_use: &def_use,
            callsites,
            function_by_name: &function_by_name,
        };
        facts::ptr_len::compute(&bodies, &snapshot, candidates)
    }

    #[salsa::tracked(returns(ref))]
    pub(in crate::fixups) fn string_param_lifts(
        self,
        db: &dyn FixupDb,
        definitions: ProgramInput,
    ) -> Vec<StringParamLiftFact<'db>> {
        let bodies: facts::walk::Bodies = self
            .functions(db)
            .iter()
            .map(|&input| (input.function_id(db), input.body(db)))
            .collect();
        let candidates: Vec<facts::string_params::Candidate> = self
            .functions(db)
            .iter()
            .flat_map(|&input| input.string_param_candidates(db).iter().cloned())
            .collect();
        let mut bindings = Vec::new();
        let mut binding_types = Vec::new();
        let mut def_use = Vec::new();
        let mut string_buffers = Vec::new();
        let mut string_libc_uses = Vec::new();
        for &input in self.functions(db) {
            bindings.extend(input.bindings_typed(db).iter().cloned());
            binding_types.extend(input.binding_types_typed(db).iter().cloned());
            def_use.extend(input.def_use(db).iter().cloned());
            let strings = input.strings(db);
            string_buffers.extend(strings.buffers.iter().cloned());
            string_libc_uses.extend(strings.libc_uses.iter().cloned());
        }
        let callsites = self.callsites(db, definitions);
        let snapshot = facts::string_params::Snapshot {
            bindings: &bindings,
            binding_types: &binding_types,
            def_use: &def_use,
            callsites,
            string_buffers: &string_buffers,
            string_libc_uses: &string_libc_uses,
        };
        facts::string_params::compute(&bodies, &snapshot, candidates)
    }

    #[salsa::tracked(returns(ref))]
    pub(in crate::fixups) fn lazy_init_singletons(
        self,
        db: &dyn FixupDb,
        definitions: ProgramInput,
    ) -> Vec<LazyInitSingletonFact<'db>> {
        let bodies: facts::walk::Bodies = self
            .functions(db)
            .iter()
            .map(|&input| (input.function_id(db), input.body(db)))
            .collect();
        let shapes: BTreeMap<FunctionId<'db>, (String, String, Expr)> = self
            .functions(db)
            .iter()
            .filter_map(|&input| {
                input
                    .lazy_singleton_shape(db)
                    .clone()
                    .map(|shape| (input.function_id(db), shape))
            })
            .collect();
        facts::lazy_singleton::compute(&shapes, &bodies, definitions.statics(db))
    }

    #[salsa::tracked(returns(ref))]
    pub(in crate::fixups) fn atomic_globals(
        self,
        db: &dyn FixupDb,
        definitions: ProgramInput,
    ) -> Vec<AtomicGlobalFact> {
        let bodies: facts::walk::Bodies = self
            .functions(db)
            .iter()
            .map(|&input| (input.function_id(db), input.body(db)))
            .collect();
        facts::atomic_locals::compute_atomic_globals(definitions.statics(db), &bodies)
    }
}

pub(in crate::fixups) struct SalsaFacts {
    db: Database,
    program: ProgramInput,
}

impl SalsaFacts {
    pub(in crate::fixups) fn new_empty() -> Self {
        let db = Database::default();
        let program = ProgramInput::new(&db, Program::default());
        Self { db, program }
    }

    pub(in crate::fixups) fn set_program(&mut self, program: &Program) {
        self.program.set_program(&mut self.db).to(program.clone());
    }

    pub(in crate::fixups) fn byte_source<'db>(
        &'db self,
        site: &ExprSite,
    ) -> QueryResult<ByteSource<'db>> {
        self.program.byte_source(&self.db, site.clone()).clone()
    }

    pub(in crate::fixups) fn const_u8(&self, site: &ExprSite) -> QueryResult<u8> {
        self.program.const_u8(&self.db, site.clone()).clone()
    }

    pub(in crate::fixups) fn const_usize(&self, site: &ExprSite) -> QueryResult<usize> {
        self.program.const_usize(&self.db, site.clone()).clone()
    }

    #[expect(
        dead_code,
        reason = "query API surface not yet wired into a fixup rule"
    )]
    pub(in crate::fixups) fn pure(&self, site: &ExprSite) -> QueryResult<StableExpr> {
        self.program.pure(&self.db, site.clone()).clone()
    }

    pub(in crate::fixups) fn full_byte_view<'db>(
        &'db self,
        source: &ByteSource<'_>,
        count: &ExprSite,
    ) -> QueryResult<ByteView<'db>> {
        self.program
            .full_byte_view(&self.db, source.site.clone(), count.clone())
            .clone()
    }

    pub(in crate::fixups) fn first_nul(&self, source: &ByteSource<'_>) -> QueryResult<NulPosition> {
        self.program
            .first_nul(&self.db, source.site.clone())
            .clone()
    }

    pub(in crate::fixups) fn prefix_contains(
        &self,
        count: &ExprSite,
        nul: NulPosition,
    ) -> QueryResult<()> {
        self.program
            .prefix_contains(&self.db, count.clone(), nul)
            .clone()
    }

    pub(in crate::fixups) fn proof_counted_loop<'db>(
        &'db self,
        statement: &StatementRef,
    ) -> QueryResult<CountedLoopFact<'db>> {
        self.program
            .counted_loop(&self.db, statement.clone())
            .clone()
    }

    pub(in crate::fixups) fn proof_counted_slice_loop<'db>(
        &'db self,
        statement: &StatementRef,
    ) -> QueryResult<SliceLoopFact<'db>> {
        self.program
            .counted_slice_loop(&self.db, statement.clone())
            .clone()
    }

    pub(in crate::fixups) fn proof_lazy_singletons(&self) -> QueryResult<LazySingletonSet> {
        self.program.lazy_singletons(&self.db).clone()
    }

    pub(in crate::fixups) fn proof_atomic_promotions(&self) -> QueryResult<AtomicPromotionSet> {
        self.program.atomic_promotions(&self.db).clone()
    }

    pub(in crate::fixups) fn proof_callee_alloc_summary<'db>(
        &'db self,
        function: &FunctionRef<'_>,
    ) -> QueryResult<CalleeAllocSummaryFact<'db>> {
        self.program
            .callee_alloc_summary(&self.db, function.item_index)
            .clone()
    }

    pub(in crate::fixups) fn proof_interprocedural_alloc_eligibility<'db>(
        &'db self,
        function: &FunctionRef<'_>,
    ) -> QueryResult<InterproceduralAllocEligibilityFact<'db>> {
        self.program
            .interprocedural_alloc_eligibility(&self.db, function.item_index)
            .clone()
    }

    pub(in crate::fixups) fn proof_interprocedural_alloc_chain<'db>(
        &'db self,
        function: &FunctionRef<'_>,
    ) -> QueryResult<Vec<FunctionRef<'db>>> {
        self.program
            .interprocedural_alloc_chain(&self.db, function.item_index)
            .clone()
    }

    pub(in crate::fixups) fn proof_interprocedural_alloc_callers<'db>(
        &'db self,
        function: &FunctionRef<'_>,
    ) -> QueryResult<Vec<InterproceduralAllocCallerInput<'db>>> {
        self.program
            .interprocedural_alloc_callers(&self.db, function.item_index)
            .clone()
    }

    pub(in crate::fixups) fn proof_ptr_len_slices(&self) -> QueryResult<PtrLenPlanSet> {
        self.program.ptr_len_slice_plans(&self.db).clone()
    }

    pub(in crate::fixups) fn proof_struct_field_ownership(
        &self,
    ) -> QueryResult<Vec<StructFieldOwnershipFact>> {
        self.program.proof_struct_field_ownership(&self.db).clone()
    }

    pub(in crate::fixups) fn proof_string_param_lift_indices(
        &self,
        function: &FunctionRef<'_>,
    ) -> QueryResult<Vec<usize>> {
        self.program
            .string_param_lift_indices(&self.db, function.item_index)
            .clone()
    }

    pub(in crate::fixups) fn proof_function_by_name<'db>(
        &'db self,
        name: &str,
    ) -> QueryResult<FunctionRef<'db>> {
        self.program
            .proof_function_by_name(&self.db, name.to_string())
            .clone()
    }

    pub(in crate::fixups) fn proof_option_box_local_candidates<'db>(
        &'db self,
        function: &FunctionRef<'_>,
    ) -> QueryResult<Vec<OptionBoxLocalPlanInput<'db>>> {
        self.program
            .option_box_local_candidates(&self.db, function.item_index)
            .clone()
    }

    pub(in crate::fixups) fn proof_option_box_comparisons(
        &self,
        function: &FunctionRef<'_>,
    ) -> QueryResult<Vec<OptionBoxComparisonInput>> {
        self.program
            .option_box_comparisons(&self.db, function.item_index)
            .clone()
    }

    pub(in crate::fixups) fn proof_buffer_pointer_fields<'db>(
        &'db self,
        function: &FunctionRef<'_>,
    ) -> QueryResult<BufferPointerFields<'db>> {
        self.program
            .proof_buffer_pointer_fields(&self.db, function.item_index)
            .clone()
    }

    pub(in crate::fixups) fn proof_calls_in(
        &self,
        function: &FunctionRef<'_>,
    ) -> QueryResult<Vec<CallRecord>> {
        self.program.calls_in(&self.db, function.item_index).clone()
    }

    pub(in crate::fixups) fn proof_reference_domain(&self) -> QueryResult<ReferenceDomain> {
        self.program.reference_domain(&self.db).clone()
    }

    pub(in crate::fixups) fn proof_anonymous_structs(&self) -> QueryResult<AnonymousStructSet> {
        self.program.proof_anonymous_structs(&self.db).clone()
    }

    pub(in crate::fixups) fn proof_definition_users(
        &self,
        definition: &DefinitionSite,
    ) -> QueryResult<DefinitionUsers> {
        self.program
            .definition_users(&self.db, definition.clone())
            .clone()
    }

    pub(in crate::fixups) fn proof_definition_group_users(
        &self,
        group: &DefinitionGroup,
    ) -> QueryResult<DefinitionGroupUsers> {
        self.program
            .definition_group_users(&self.db, group.clone())
            .clone()
    }

    pub(in crate::fixups) fn proof_heap_ownership<'db>(
        &'db self,
        function: &FunctionRef<'_>,
    ) -> QueryResult<HeapOwnershipFacts<'db>> {
        self.program
            .proof_heap_ownership(&self.db, function.item_index)
            .clone()
    }

    pub(in crate::fixups) fn proof_file_ownership<'db>(
        &'db self,
        function: &FunctionRef<'_>,
    ) -> QueryResult<FileOwnershipFacts<'db>> {
        self.program
            .proof_file_ownership(&self.db, function.item_index)
            .clone()
    }

    fn function_input<'db>(&'db self, function: FunctionId<'_>) -> Option<FunctionInput<'db>> {
        let name = function.name(&self.db);
        self.program
            .functions(&self.db)
            .iter()
            .copied()
            .find(|input| input.function(&self.db).name(&self.db) == name)
    }

    pub(in crate::fixups) fn def_use<'a>(
        &'a self,
        function: FunctionId<'a>,
        binding: BindingId<'a>,
    ) -> Option<&'a DefUseFact<'a>> {
        let input = self.function_input(function)?;
        input.def_use_by_binding(&self.db).get(&binding)
    }

    pub(in crate::fixups) fn effect(
        &self,
        function: FunctionId<'_>,
        subject: EffectSubject,
        path: &AstPath,
    ) -> Option<&EffectFact<'_>> {
        let input = self.function_input(function)?;
        input.effects_by_key(&self.db).get(&(subject, path.clone()))
    }

    pub(in crate::fixups) fn values_for(&self, function: FunctionId<'_>) -> &[ValueFact<'_>] {
        let Some(input) = self.function_input(function) else {
            return &[];
        };
        input.values(&self.db)
    }

    pub(in crate::fixups) fn string_buffer_at(
        &self,
        function: FunctionId<'_>,
        path: &AstPath,
    ) -> Option<&facts::StringBufferFact<'_>> {
        let input = self.function_input(function)?;
        input
            .strings(&self.db)
            .buffers
            .iter()
            .find(|buffer| &buffer.site.path == path)
    }

    pub(in crate::fixups) fn string_pointer_views(
        &self,
        function: FunctionId<'_>,
    ) -> &[facts::StringPointerViewFact<'_>] {
        let Some(input) = self.function_input(function) else {
            return &[];
        };
        &input.strings(&self.db).pointer_views
    }

    pub(in crate::fixups) fn string_libc_use(
        &self,
        function: FunctionId<'_>,
        path: &AstPath,
    ) -> Option<&facts::StringLibcUseFact<'_>> {
        let input = self.function_input(function)?;
        input
            .strings(&self.db)
            .libc_uses
            .iter()
            .find(|libc| &libc.site.path == path)
    }

    pub(in crate::fixups) fn liftable_string_bindings(
        &self,
        function: FunctionId<'_>,
        recovery: StringRecoveryCandidate,
    ) -> BTreeSet<BindingId<'_>> {
        let Some(input) = self.function_input(function) else {
            return BTreeSet::new();
        };
        input
            .string_rewrite_facts(&self.db, self.program, self.program)
            .0
            .iter()
            .filter(|plan| plan.recovery == recovery)
            .map(|plan| plan.binding)
            .collect()
    }

    pub(in crate::fixups) fn string_use_allowed(
        &self,
        function: FunctionId<'_>,
        use_path: &AstPath,
        binding: BindingId<'_>,
        recovery: StringRecoveryCandidate,
        liftable: &BTreeSet<BindingId<'_>>,
    ) -> bool {
        let Some(input) = self.function_input(function) else {
            return false;
        };
        let strings = input.strings(&self.db);
        let bindings = input.bindings_typed(&self.db);
        let snapshot = facts::strings::RewriteSnapshot {
            bindings,
            def_use: input.def_use(&self.db),
            values: input.values(&self.db),
            string_buffers: &strings.buffers,
            string_pointer_views: &strings.pointer_views,
            string_libc_uses: &strings.libc_uses,
            printf_calls: input.printf_calls(&self.db),
            callsites: self.program.callsites(&self.db, self.program),
            string_param_lifts: self.program.string_param_lifts(&self.db, self.program),
        };
        facts::strings::use_allowed(function, use_path, &snapshot, binding, recovery, liftable)
    }

    pub(in crate::fixups) fn string_copy_rewrites(
        &self,
        function: FunctionId<'_>,
    ) -> &[StringCopyRewriteFact<'_>] {
        let Some(input) = self.function_input(function) else {
            return &[];
        };
        &input
            .string_rewrite_facts(&self.db, self.program, self.program)
            .1
    }

    pub(in crate::fixups) fn cast_at(
        &self,
        function: FunctionId<'_>,
        path: &AstPath,
    ) -> Option<&CastFact<'_>> {
        let input = self.function_input(function)?;
        input
            .casts(&self.db, self.program, self.program)
            .iter()
            .find(|fact| &fact.site.path == path)
    }

    pub(in crate::fixups) fn place(
        &self,
        function: FunctionId<'_>,
        path: &AstPath,
    ) -> Option<&PlaceFact<'_>> {
        let input = self.function_input(function)?;
        input
            .places(&self.db)
            .iter()
            .find(|fact| &fact.site.path == path)
    }

    pub(in crate::fixups) fn control_flow(
        &self,
        function: FunctionId<'_>,
        subject: ControlFlowSubject,
        path: &AstPath,
    ) -> Option<&ControlFlowFact<'_>> {
        let input = self.function_input(function)?;
        input
            .control_flow(&self.db)
            .iter()
            .find(|fact| fact.subject == subject && &fact.site.path == path)
    }

    pub(in crate::fixups) fn borrow_alias_reasons(
        &self,
        function: FunctionId<'_>,
        binding: BindingId<'_>,
    ) -> Option<&BTreeSet<BorrowAliasReason>> {
        let input = self.function_input(function)?;
        input
            .borrow_alias(&self.db)
            .iter()
            .find(|fact| fact.binding == binding)
            .map(|fact| &fact.reasons)
    }

    pub(in crate::fixups) fn binding_requires_mut_by_binding(
        &self,
        binding: BindingId<'_>,
    ) -> bool {
        let Some(function) = self.program.functions(&self.db).iter().find_map(|&input| {
            input
                .bindings_typed(&self.db)
                .iter()
                .any(|fact| fact.id == binding)
                .then(|| input.function(&self.db))
        }) else {
            return false;
        };
        self.binding_requires_mut(function, binding)
    }

    pub(in crate::fixups) fn borrow_alias_reasons_by_binding(
        &self,
        binding: BindingId<'_>,
    ) -> Option<BTreeSet<BorrowAliasReason>> {
        self.program.functions(&self.db).iter().find_map(|&input| {
            input
                .borrow_alias(&self.db)
                .iter()
                .find(|fact| fact.binding == binding)
                .map(|fact| fact.reasons.clone())
        })
    }

    pub(in crate::fixups) fn binding_requires_mut(
        &self,
        function: FunctionId<'_>,
        binding: BindingId<'_>,
    ) -> bool {
        let Some(input) = self.function_input(function) else {
            return false;
        };
        let borrow_alias = input.borrow_alias(&self.db);
        let is_mut = |id: BindingId<'_>| {
            borrow_alias
                .iter()
                .find(|fact| fact.binding == id)
                .is_some_and(|fact| fact.state != BorrowAliasState::ReadOnly)
        };
        if is_mut(binding) {
            return true;
        }
        let bindings = input.bindings_typed(&self.db);
        let Some(target) = bindings.iter().find(|b| b.id == binding) else {
            return false;
        };
        bindings
            .iter()
            .filter(|other| other.name == target.name)
            .any(|other| is_mut(other.id))
    }

    pub(in crate::fixups) fn array_element_pointer_origins(
        &self,
        function: FunctionId<'_>,
    ) -> &[ArrayElementPointerOriginFact<'_>] {
        let Some(input) = self.function_input(function) else {
            return &[];
        };
        input.array_element_pointer_origins(&self.db)
    }

    pub(in crate::fixups) fn null_check_dominance_at(
        &self,
        function: FunctionId<'_>,
        deref_path: &AstPath,
    ) -> Option<&NullCheckDominanceFact<'_>> {
        let input = self.function_input(function)?;
        input
            .null_check_dominance(&self.db)
            .iter()
            .find(|fact| facts::walk::paths_overlap(&fact.deref_site.path.0, &deref_path.0))
    }

    pub(in crate::fixups) fn atomic_locals(&self) -> &[AtomicLocalFact<'_>] {
        self.program.atomic_locals(&self.db)
    }

    pub(in crate::fixups) fn pointer_option_safety_of(
        &self,
        function: FunctionId<'_>,
        binding: BindingId<'_>,
    ) -> Option<&PointerOptionSafetyFact<'_>> {
        let input = self.function_input(function)?;
        input
            .pointer_option_safety(&self.db, self.program)
            .0
            .iter()
            .find(|fact| fact.binding == binding)
    }

    #[expect(
        dead_code,
        reason = "query API surface not yet wired into a fixup rule"
    )]
    pub(in crate::fixups) fn pointer_comparison_at(
        &self,
        function: FunctionId<'_>,
        path: &AstPath,
    ) -> Option<&PointerComparisonFact<'_>> {
        let input = self.function_input(function)?;
        input
            .pointer_option_safety(&self.db, self.program)
            .1
            .iter()
            .find(|fact| &fact.site.path == path)
    }

    pub(in crate::fixups) fn c_string_literal(
        &self,
        function: FunctionId<'_>,
        receiver_path: &AstPath,
    ) -> Option<&CStringLiteralFact<'_>> {
        let input = self.function_input(function)?;
        input
            .c_string_literals(&self.db)
            .iter()
            .find(|fact| &fact.receiver_path == receiver_path)
    }

    pub(in crate::fixups) fn printf_calls(
        &self,
        function: FunctionId<'_>,
    ) -> &[PrintfCallFact<'_>] {
        let Some(input) = self.function_input(function) else {
            return &[];
        };
        input.printf_calls(&self.db)
    }

    pub(in crate::fixups) fn callsites(&self) -> &[CallsiteFact<'_>] {
        self.program.callsites(&self.db, self.program)
    }

    pub(in crate::fixups) fn callsite(
        &self,
        function: FunctionId<'_>,
        path: &AstPath,
    ) -> Option<CallsiteFact<'_>> {
        self.callsites()
            .iter()
            .find(|fact| fact.site.function == function && &fact.site.path == path)
            .cloned()
    }

    pub(in crate::fixups) fn call_arg_at(
        &self,
        function: FunctionId<'_>,
        path: &AstPath,
    ) -> Option<(CallsiteFact<'_>, CallArgFact)> {
        self.callsites().iter().find_map(|callsite| {
            if callsite.site.function != function {
                return None;
            }
            callsite
                .args
                .iter()
                .find(|arg| &arg.path == path)
                .cloned()
                .map(|arg| (callsite.clone(), arg))
        })
    }

    pub(in crate::fixups) fn ptr_len_slices(&self) -> &[PtrLenSliceFact<'_>] {
        self.program.ptr_len_slices(&self.db, self.program)
    }

    pub(in crate::fixups) fn lazy_init_singletons(&self) -> &[LazyInitSingletonFact<'_>] {
        self.program.lazy_init_singletons(&self.db, self.program)
    }

    #[expect(
        dead_code,
        reason = "query API surface not yet wired into a fixup rule"
    )]
    pub(in crate::fixups) fn struct_field_ownership(&self) -> &[StructFieldOwnershipFact] {
        self.program.struct_field_ownership(&self.db)
    }

    pub(in crate::fixups) fn anonymous_structs(&self) -> &[AnonymousStructFact] {
        self.program.anonymous_structs(&self.db)
    }

    pub(in crate::fixups) fn atomic_globals(&self) -> &[AtomicGlobalFact] {
        self.program.atomic_globals(&self.db, self.program)
    }

    pub(in crate::fixups) fn function_by_item_index(
        &self,
        item_index: usize,
    ) -> Option<FunctionId<'_>> {
        self.program
            .base_walk(&self.db)
            .function_by_item_index(item_index)
    }

    pub(in crate::fixups) fn function_item_index(&self, function: FunctionId<'_>) -> Option<usize> {
        self.program
            .base_walk(&self.db)
            .function_item_index(function)
    }

    pub(in crate::fixups) fn function_name(&self, function: FunctionId<'_>) -> Option<&str> {
        self.program.base_walk(&self.db).function_name(function)
    }

    pub(in crate::fixups) fn function_by_name(&self, name: &str) -> Option<FunctionId<'_>> {
        self.program.base_walk(&self.db).function_by_name(name)
    }

    pub(in crate::fixups) fn function_facts(&self) -> Vec<FunctionFact<'_>> {
        self.program.base_walk(&self.db).function_facts()
    }

    pub(in crate::fixups) fn binding_facts(&self) -> Vec<BindingFact<'_>> {
        self.program.base_walk(&self.db).binding_facts()
    }

    pub(in crate::fixups) fn binding_by_local_path<'db>(
        &'db self,
        function: FunctionId<'db>,
        name: &str,
        path: &AstPath,
    ) -> Option<BindingId<'db>> {
        let input = self.function_input(function)?;
        facts::binding_by_local_path(input.bindings_typed(&self.db), function, name, path)
    }

    pub(in crate::fixups) fn binding_type_ast(
        &self,
        binding: BindingId<'_>,
    ) -> Option<crate::rust_ast::Type> {
        self.program.functions(&self.db).iter().find_map(|&input| {
            facts::binding_type_ast(input.binding_types_typed(&self.db), binding).cloned()
        })
    }

    pub(in crate::fixups) fn binding_name(&self, binding: BindingId<'_>) -> Option<String> {
        self.program.functions(&self.db).iter().find_map(|&input| {
            facts::binding_name(input.bindings_typed(&self.db), binding).map(str::to_string)
        })
    }

    pub(in crate::fixups) fn bindings_read_under(
        &self,
        function: FunctionId<'_>,
        name: &str,
        path: &AstPath,
    ) -> Vec<BindingId<'_>> {
        let Some(input) = self.function_input(function) else {
            return Vec::new();
        };
        let query_path = facts::def_use_query_path(path);
        let by_binding = input.def_use_by_binding(&self.db);
        input
            .binding_ids_by_name(&self.db)
            .get(name)
            .into_iter()
            .flatten()
            .filter_map(|id| by_binding.get(id))
            .filter(|fact| {
                fact.reads
                    .iter()
                    .any(|read| facts::walk::paths_overlap(&read.0, &query_path.0))
            })
            .map(|fact| fact.binding)
            .collect()
    }

    pub(in crate::fixups) fn bindings_written_under(
        &self,
        function: FunctionId<'_>,
        name: &str,
        path: &AstPath,
    ) -> Vec<BindingId<'_>> {
        let Some(input) = self.function_input(function) else {
            return Vec::new();
        };
        let query_path = facts::def_use_query_path(path);
        let by_binding = input.def_use_by_binding(&self.db);
        input
            .binding_ids_by_name(&self.db)
            .get(name)
            .into_iter()
            .flatten()
            .filter_map(|id| by_binding.get(id))
            .filter(|fact| {
                fact.writes
                    .iter()
                    .any(|write| facts::walk::paths_overlap(&write.0, &query_path.0))
            })
            .map(|fact| fact.binding)
            .collect()
    }

    pub(in crate::fixups) fn binding_names_and_types(
        &self,
        function: FunctionId<'_>,
    ) -> Vec<(String, String)> {
        let Some(input) = self.function_input(function) else {
            return Vec::new();
        };
        let binding_types = input.binding_types_typed(&self.db);
        input
            .bindings_typed(&self.db)
            .iter()
            .filter_map(|binding| {
                let rendered = facts::binding_type(binding_types, binding.id)?;
                Some((binding.name.clone(), rendered.to_string()))
            })
            .collect()
    }

    pub(in crate::fixups) fn ascii_numeric_string(
        &self,
        binding: BindingId<'_>,
    ) -> Option<&AsciiNumericStringFact<'_>> {
        self.program.functions(&self.db).iter().find_map(|&input| {
            input
                .ascii_numeric_strings(&self.db)
                .iter()
                .find(|fact| fact.binding == binding)
        })
    }
}
