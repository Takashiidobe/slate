use std::collections::{BTreeMap, BTreeSet, HashMap};

use crate::fixups::facts::walk::RawBinding;
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
use crate::fixups::query::TouchedItems;
use crate::rust_ast::{EnumDef, Expr, FnDef, Item, Program, RecordDef, StructDef};
use salsa::Setter;
use salsa::plumbing::{AsId, FromId};

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

#[salsa::input]
pub(in crate::fixups) struct FunctionInput {
    pub(in crate::fixups) function: salsa::Id,
    #[returns(ref)]
    pub(in crate::fixups) body: FnDef,
    #[returns(ref)]
    pub(in crate::fixups) bindings: Vec<RawBinding>,
    #[returns(ref)]
    pub(in crate::fixups) binding_types: Vec<facts::walk::RawBindingType>,
    #[returns(ref)]
    pub(in crate::fixups) loops: Vec<facts::walk::RawLoop>,
}

#[salsa::input]
pub(in crate::fixups) struct AllFunctions {
    #[returns(ref)]
    pub(in crate::fixups) functions: Vec<FunctionInput>,
}

/// Lifetime-erased mirror of `CallSignatureFact`, storable in a
/// `#[salsa::input]` field (see `facts::walk::RawBinding` for why). Extern
/// signatures never populate `CallSignatureSource::Function`, but the
/// erased shape has to cover it anyway since it mirrors the real enum.
#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) enum RawCallSignatureSource {
    Function(salsa::Id),
    Extern {
        item_index: usize,
        decl_index: usize,
    },
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct RawCallSignature {
    id: SignatureId,
    name: String,
    source: RawCallSignatureSource,
    params: Vec<facts::CallParamFact>,
    variadic: bool,
    ret: Option<crate::rust_ast::Type>,
}

fn erase_call_signature(fact: &CallSignatureFact<'_>) -> RawCallSignature {
    RawCallSignature {
        id: fact.id,
        name: fact.name.clone(),
        source: match fact.source {
            facts::CallSignatureSource::Function(function) => {
                RawCallSignatureSource::Function(function.as_id())
            }
            facts::CallSignatureSource::Extern {
                item_index,
                decl_index,
            } => RawCallSignatureSource::Extern {
                item_index,
                decl_index,
            },
        },
        params: fact.params.clone(),
        variadic: fact.variadic,
        ret: fact.ret.clone(),
    }
}

fn hydrate_call_signature(raw: &RawCallSignature) -> CallSignatureFact<'_> {
    CallSignatureFact {
        id: raw.id,
        name: raw.name.clone(),
        source: match raw.source {
            RawCallSignatureSource::Function(function) => {
                facts::CallSignatureSource::Function(FunctionId::from_id(function))
            }
            RawCallSignatureSource::Extern {
                item_index,
                decl_index,
            } => facts::CallSignatureSource::Extern {
                item_index,
                decl_index,
            },
        },
        params: raw.params.clone(),
        variadic: raw.variadic,
        ret: raw.ret.clone(),
    }
}

#[salsa::input]
pub(in crate::fixups) struct DefinitionsInput {
    #[returns(ref)]
    pub(in crate::fixups) records: Vec<RecordDef>,
    #[returns(ref)]
    pub(in crate::fixups) structs: Vec<StructDef>,
    #[returns(ref)]
    pub(in crate::fixups) enums: Vec<EnumDef>,
    #[returns(ref)]
    pub(in crate::fixups) extern_call_signatures: Vec<RawCallSignature>,
    #[returns(ref)]
    pub(in crate::fixups) statics: Vec<StaticDeclFact>,
}

#[salsa::tracked]
impl DefinitionsInput {
    #[salsa::tracked(returns(ref))]
    fn union_records(self, db: &dyn FixupDb) -> BTreeSet<String> {
        self.records(db)
            .iter()
            .filter(|record| record.is_union)
            .map(|record| record.name.clone())
            .collect()
    }

    #[salsa::tracked(returns(ref))]
    fn struct_field_ownership(self, db: &dyn FixupDb) -> Vec<StructFieldOwnershipFact> {
        facts::struct_field_ownership::collect(self.records(db).iter())
    }

    #[salsa::tracked(returns(ref))]
    fn anonymous_structs(self, db: &dyn FixupDb) -> Vec<AnonymousStructFact> {
        facts::anonymous_structs::collect(self.records(db).iter())
    }
}

#[salsa::tracked]
impl<'db> FunctionInput {
    fn function_id(self, db: &'db dyn FixupDb) -> FunctionId<'db> {
        FunctionId::from_id(*self.function(db))
    }

    fn bindings_typed(self, db: &'db dyn FixupDb) -> Vec<BindingFact<'db>> {
        self.bindings(db)
            .iter()
            .map(facts::walk::hydrate_binding)
            .collect()
    }

    fn binding_types_typed(self, db: &'db dyn FixupDb) -> Vec<BindingTypeFact<'db>> {
        self.binding_types(db)
            .iter()
            .map(facts::walk::hydrate_binding_type)
            .collect()
    }

    fn loops_typed(self, db: &'db dyn FixupDb) -> Vec<LoopFact<'db>> {
        self.loops(db)
            .iter()
            .map(facts::walk::hydrate_loop)
            .collect()
    }

    #[salsa::tracked(returns(ref))]
    fn def_use(self, db: &dyn FixupDb) -> Vec<DefUseFact<'db>> {
        facts::def_use::collect_for_function(
            self.function_id(db),
            self.body(db),
            &self.bindings_typed(db),
        )
    }

    #[salsa::tracked(returns(ref))]
    fn effects(self, db: &dyn FixupDb) -> Vec<EffectFact<'db>> {
        facts::effects::collect_for_function(self.function_id(db), self.body(db))
    }

    #[salsa::tracked(returns(ref))]
    fn values(self, db: &dyn FixupDb) -> Vec<ValueFact<'db>> {
        facts::values::collect_for_function(
            self.function_id(db),
            self.body(db),
            &self.bindings_typed(db),
        )
    }

    #[salsa::tracked(returns(ref))]
    fn strings(self, db: &dyn FixupDb) -> facts::strings::Collected<'db> {
        facts::strings::collect_for_function(
            self.function_id(db),
            self.body(db),
            &self.bindings_typed(db),
            &self.binding_types_typed(db),
        )
    }

    #[salsa::tracked(returns(ref))]
    fn counted_loops(
        self,
        db: &dyn FixupDb,
    ) -> (Vec<CountedLoopFact<'db>>, Vec<CountedSliceLoopFact<'db>>) {
        facts::counted_loop::collect_for_function(
            self.function_id(db),
            self.body(db),
            &self.bindings_typed(db),
            &self.loops_typed(db),
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
        all_functions: AllFunctions,
        definitions: DefinitionsInput,
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
        all_functions: AllFunctions,
        definitions: DefinitionsInput,
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
            &bindings,
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
            &self.bindings_typed(db),
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
            &self.bindings_typed(db),
            &self.binding_types_typed(db),
            self.def_use(db),
        )
    }

    #[salsa::tracked(returns(ref))]
    fn null_check_dominance(self, db: &dyn FixupDb) -> Vec<NullCheckDominanceFact<'db>> {
        facts::null_check_dominance::collect_for_function(
            self.function_id(db),
            self.body(db),
            &self.bindings_typed(db),
            self.control_flow(db),
        )
    }

    #[salsa::tracked(returns(ref))]
    fn atomic_locals(self, db: &dyn FixupDb) -> Vec<AtomicLocalFact<'db>> {
        facts::atomic_locals::collect_for_function(self.function_id(db), self.body(db))
    }

    #[salsa::tracked(returns(ref))]
    fn pointer_option_safety(
        self,
        db: &dyn FixupDb,
        definitions: DefinitionsInput,
    ) -> (
        Vec<PointerOptionSafetyFact<'db>>,
        Vec<PointerComparisonFact<'db>>,
    ) {
        facts::pointer_option_safety::collect_for_function(
            self.function_id(db),
            self.body(db),
            &self.bindings_typed(db),
            &self.binding_types_typed(db),
            definitions.union_records(db),
        )
    }

    #[salsa::tracked(returns(ref))]
    fn option_box(
        self,
        db: &dyn FixupDb,
    ) -> (
        Vec<OptionBoxLocalCandidate<'db>>,
        Vec<OptionBoxComparison<'db>>,
    ) {
        facts::option_box_locals::collect_for_function(
            self.function_id(db),
            self.body(db),
            &self.bindings_typed(db),
        )
    }

    #[salsa::tracked(returns(ref))]
    fn buffer_pointer_fields(self, db: &dyn FixupDb) -> Vec<BufferPointerFieldFact<'db>> {
        facts::buffer_cursor::collect_for_function(
            self.function_id(db),
            &self.body(db).body,
            &self.bindings_typed(db),
            &self.binding_types_typed(db),
        )
    }

    #[salsa::tracked(returns(ref))]
    fn heap_ownership(self, db: &dyn FixupDb) -> Vec<HeapOwnershipFact<'db>> {
        facts::heap_ownership::collect_for_function(
            self.function_id(db),
            &self.body(db).body,
            &self.bindings_typed(db),
        )
    }

    #[salsa::tracked(returns(ref))]
    fn file_ownership(self, db: &dyn FixupDb) -> Vec<FileOwnershipFact<'db>> {
        facts::file_ownership::collect_for_function(
            self.function_id(db),
            &self.body(db).body,
            &self.bindings_typed(db),
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
    fn callee_alloc_summary(self, db: &dyn FixupDb) -> Option<CalleeAllocSummaryFact<'db>> {
        facts::callee_alloc_summary::collect_for_function(self.function_id(db), self.body(db))
    }

    #[salsa::tracked(returns(ref))]
    fn ptr_len_candidates(self, db: &dyn FixupDb) -> Vec<facts::ptr_len::Candidate<'db>> {
        facts::ptr_len::candidates_for_function(
            self.function_id(db),
            self.body(db),
            &self.bindings_typed(db),
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
            &self.bindings_typed(db),
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
        all_functions: AllFunctions,
        definitions: DefinitionsInput,
    ) -> (
        Vec<StringLiftPlanFact<'db>>,
        Vec<StringCopyRewriteFact<'db>>,
    ) {
        let strings = self.strings(db);
        let bindings = self.bindings_typed(db);
        let snapshot = facts::strings::RewriteSnapshot {
            bindings: &bindings,
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
impl<'db> AllFunctions {
    #[salsa::tracked(returns(ref))]
    fn atomic_locals(self, db: &dyn FixupDb) -> Vec<AtomicLocalFact<'db>> {
        self.functions(db)
            .iter()
            .flat_map(|&input| input.atomic_locals(db).iter().cloned())
            .collect()
    }

    #[salsa::tracked(returns(ref))]
    fn interprocedural_alloc(
        self,
        db: &dyn FixupDb,
    ) -> (
        Vec<InterproceduralAllocEligibilityFact<'db>>,
        Vec<InterproceduralAllocCallerFact<'db>>,
    ) {
        let bindings: Vec<Vec<BindingFact<'db>>> = self
            .functions(db)
            .iter()
            .map(|&input| input.bindings_typed(db))
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
        definitions: DefinitionsInput,
    ) -> (Vec<CallSignatureFact<'db>>, BTreeMap<String, SignatureId>) {
        let mut signatures: Vec<CallSignatureFact<'db>> = self
            .functions(db)
            .iter()
            .map(|&input| input.local_call_signature(db).clone())
            .collect();
        signatures.extend(
            definitions
                .extern_call_signatures(db)
                .iter()
                .map(hydrate_call_signature),
        );
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
    fn callsites(self, db: &dyn FixupDb, definitions: DefinitionsInput) -> Vec<CallsiteFact<'db>> {
        let (signatures, by_name) = self.call_signature_table(db, definitions);
        let mut all = Vec::new();
        for &input in self.functions(db) {
            all.extend(facts::calls::collect_callsites_for_function(
                input.function_id(db),
                &input.body(db).body,
                &input.bindings_typed(db),
                signatures,
                by_name,
            ));
        }
        all
    }

    #[salsa::tracked(returns(ref))]
    fn ptr_len_slices(
        self,
        db: &dyn FixupDb,
        definitions: DefinitionsInput,
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
            bindings.extend(input.bindings_typed(db));
            binding_types.extend(input.binding_types_typed(db));
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
    fn string_param_lifts(
        self,
        db: &dyn FixupDb,
        definitions: DefinitionsInput,
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
            bindings.extend(input.bindings_typed(db));
            binding_types.extend(input.binding_types_typed(db));
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
    fn lazy_init_singletons(
        self,
        db: &dyn FixupDb,
        definitions: DefinitionsInput,
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
    fn atomic_globals(
        self,
        db: &dyn FixupDb,
        definitions: DefinitionsInput,
    ) -> Vec<AtomicGlobalFact> {
        let bodies: facts::walk::Bodies = self
            .functions(db)
            .iter()
            .map(|&input| (input.function_id(db), input.body(db)))
            .collect();
        facts::atomic_locals::compute_atomic_globals(definitions.statics(db), &bodies)
    }
}

enum Dirty {
    Clean,
    Touched(TouchedItems),
    Everything,
}

pub(in crate::fixups) struct SalsaFacts {
    db: Database,
    program: ProgramInput,
    functions: HashMap<salsa::Id, FunctionInput>,
    all_functions: AllFunctions,
    definitions: Option<DefinitionsInput>,
    base: facts::walk::BaseWalk,
    dirty: Dirty,
}

impl SalsaFacts {
    pub(in crate::fixups) fn new_empty() -> Self {
        let db = Database::default();
        let program = ProgramInput::new(&db, Program::default());
        let all_functions = AllFunctions::new(&db, Vec::new());
        Self {
            db,
            program,
            functions: HashMap::new(),
            all_functions,
            definitions: None,
            base: facts::walk::BaseWalk::default(),
            dirty: Dirty::Clean,
        }
    }

    pub(in crate::fixups) fn mark_touched(&mut self, touched: &TouchedItems) {
        if touched.unbounded {
            self.dirty = Dirty::Everything;
            return;
        }
        if touched.in_place.is_empty() && touched.removed.is_empty() {
            return;
        }
        match &mut self.dirty {
            Dirty::Everything => {}
            Dirty::Clean => self.dirty = Dirty::Touched(touched.clone()),
            Dirty::Touched(existing) => existing.merge(touched.clone()),
        }
    }

    pub(in crate::fixups) fn mark_everything_dirty(&mut self) {
        self.dirty = Dirty::Everything;
    }

    pub(in crate::fixups) fn resolve(&mut self, program: &Program) {
        match std::mem::replace(&mut self.dirty, Dirty::Clean) {
            Dirty::Clean => {}
            Dirty::Touched(_) | Dirty::Everything => {
                self.program.set_program(&mut self.db).to(program.clone());
                self.base = facts::walk::BaseWalk::new(&self.db, program);
                self.sync_all(program);
            }
        }
    }

    fn sync_all(&mut self, program: &Program) {
        self.sync_definitions(program);
        let function_facts = self.base.function_facts();
        for function_fact in &function_facts {
            self.sync_function(program, function_fact);
        }
        let live: std::collections::HashSet<salsa::Id> =
            function_facts.iter().map(|fact| fact.id.as_id()).collect();
        self.functions.retain(|id, _| live.contains(id));
        self.sync_all_functions();
    }

    fn sync_all_functions(&mut self) {
        let mut sorted: Vec<(&salsa::Id, &FunctionInput)> = self.functions.iter().collect();
        sorted.sort_by_key(|(id, _)| **id);
        let functions: Vec<FunctionInput> = sorted.into_iter().map(|(_, &input)| input).collect();
        self.all_functions.set_functions(&mut self.db).to(functions);
    }

    fn sync_definitions(&mut self, program: &Program) {
        let records: Vec<RecordDef> = program
            .items
            .iter()
            .filter_map(|item| match item {
                Item::Record(record) => Some(record.clone()),
                _ => None,
            })
            .collect();
        let structs: Vec<StructDef> = program
            .items
            .iter()
            .filter_map(|item| match item {
                Item::Struct(s) => Some(s.clone()),
                _ => None,
            })
            .collect();
        let enums: Vec<EnumDef> = program
            .items
            .iter()
            .filter_map(|item| match item {
                Item::Enum(e) => Some(e.clone()),
                _ => None,
            })
            .collect();
        let extern_call_signatures: Vec<RawCallSignature> =
            facts::calls::collect_extern_signatures(program)
                .iter()
                .map(erase_call_signature)
                .collect();
        let statics: Vec<StaticDeclFact> = program
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
            .collect();
        match self.definitions {
            Some(input) => {
                input.set_records(&mut self.db).to(records);
                input.set_structs(&mut self.db).to(structs);
                input.set_enums(&mut self.db).to(enums);
                input
                    .set_extern_call_signatures(&mut self.db)
                    .to(extern_call_signatures);
                input.set_statics(&mut self.db).to(statics);
            }
            None => {
                self.definitions = Some(DefinitionsInput::new(
                    &self.db,
                    records,
                    structs,
                    enums,
                    extern_call_signatures,
                    statics,
                ));
            }
        }
    }

    fn sync_function(&mut self, program: &Program, function_fact: &FunctionFact<'_>) {
        let Some(Item::Fn(body)) = program.items.get(function_fact.item_index) else {
            return;
        };
        let bindings: Vec<RawBinding> = self
            .base
            .binding_facts()
            .iter()
            .filter(|binding| binding.function == function_fact.id)
            .map(facts::walk::erase_binding)
            .collect();
        let binding_types: Vec<facts::walk::RawBindingType> = self
            .base
            .binding_type_facts()
            .iter()
            .filter(|binding_type| {
                bindings
                    .iter()
                    .any(|binding| binding.id == binding_type.binding.as_id())
            })
            .map(facts::walk::erase_binding_type)
            .collect();
        let loops: Vec<facts::walk::RawLoop> = self
            .base
            .loop_facts()
            .iter()
            .filter(|loop_fact| loop_fact.function == function_fact.id)
            .map(facts::walk::erase_loop)
            .collect();
        match self.functions.get(&function_fact.id.as_id()) {
            Some(&input) => {
                input.set_body(&mut self.db).to(body.clone());
                input.set_bindings(&mut self.db).to(bindings);
                input.set_binding_types(&mut self.db).to(binding_types);
                input.set_loops(&mut self.db).to(loops);
            }
            None => {
                let input = FunctionInput::new(
                    &self.db,
                    function_fact.id.as_id(),
                    body.clone(),
                    bindings,
                    binding_types,
                    loops,
                );
                self.functions.insert(function_fact.id.as_id(), input);
            }
        }
    }

    pub(in crate::fixups) fn def_use(
        &self,
        function: FunctionId<'_>,
        binding: BindingId<'_>,
    ) -> Option<&DefUseFact<'_>> {
        let input = *self.functions.get(&function.as_id())?;
        input
            .def_use(&self.db)
            .iter()
            .find(|fact| fact.binding == binding)
    }

    pub(in crate::fixups) fn effect(
        &self,
        function: FunctionId<'_>,
        subject: EffectSubject,
        path: &AstPath,
    ) -> Option<&EffectFact<'_>> {
        let input = *self.functions.get(&function.as_id())?;
        input
            .effects(&self.db)
            .iter()
            .find(|fact| fact.subject == subject && &fact.site.path == path)
    }

    pub(in crate::fixups) fn values_for(&self, function: FunctionId<'_>) -> &[ValueFact<'_>] {
        let Some(&input) = self.functions.get(&function.as_id()) else {
            return &[];
        };
        input.values(&self.db)
    }

    pub(in crate::fixups) fn string_buffer_at(
        &self,
        function: FunctionId<'_>,
        path: &AstPath,
    ) -> Option<&facts::StringBufferFact<'_>> {
        let input = *self.functions.get(&function.as_id())?;
        input
            .strings(&self.db)
            .buffers
            .iter()
            .find(|buffer| &buffer.site.path == path)
    }

    pub(in crate::fixups) fn string_buffer(
        &self,
        binding: BindingId<'_>,
    ) -> Option<&facts::StringBufferFact<'_>> {
        self.functions.values().find_map(|&input| {
            input
                .strings(&self.db)
                .buffers
                .iter()
                .find(|buffer| buffer.binding == binding)
        })
    }

    pub(in crate::fixups) fn string_pointer_views(
        &self,
        function: FunctionId<'_>,
    ) -> &[facts::StringPointerViewFact<'_>] {
        let Some(&input) = self.functions.get(&function.as_id()) else {
            return &[];
        };
        &input.strings(&self.db).pointer_views
    }

    pub(in crate::fixups) fn string_libc_use(
        &self,
        function: FunctionId<'_>,
        path: &AstPath,
    ) -> Option<&facts::StringLibcUseFact<'_>> {
        let input = *self.functions.get(&function.as_id())?;
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
        let (Some(&input), Some(definitions)) =
            (self.functions.get(&function.as_id()), self.definitions)
        else {
            return BTreeSet::new();
        };
        input
            .string_rewrite_facts(&self.db, self.all_functions, definitions)
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
        let (Some(&input), Some(definitions)) =
            (self.functions.get(&function.as_id()), self.definitions)
        else {
            return false;
        };
        let strings = input.strings(&self.db);
        let bindings = input.bindings_typed(&self.db);
        let snapshot = facts::strings::RewriteSnapshot {
            bindings: &bindings,
            def_use: input.def_use(&self.db),
            values: input.values(&self.db),
            string_buffers: &strings.buffers,
            string_pointer_views: &strings.pointer_views,
            string_libc_uses: &strings.libc_uses,
            printf_calls: input.printf_calls(&self.db),
            callsites: self.all_functions.callsites(&self.db, definitions),
            string_param_lifts: self.all_functions.string_param_lifts(&self.db, definitions),
        };
        facts::strings::use_allowed(function, use_path, &snapshot, binding, recovery, liftable)
    }

    pub(in crate::fixups) fn string_copy_rewrites(
        &self,
        function: FunctionId<'_>,
    ) -> &[StringCopyRewriteFact<'_>] {
        let (Some(&input), Some(definitions)) =
            (self.functions.get(&function.as_id()), self.definitions)
        else {
            return &[];
        };
        &input
            .string_rewrite_facts(&self.db, self.all_functions, definitions)
            .1
    }

    pub(in crate::fixups) fn counted_loop(
        &self,
        function: FunctionId<'_>,
        loop_path: &AstPath,
    ) -> Option<&CountedLoopFact<'_>> {
        let input = *self.functions.get(&function.as_id())?;
        input
            .counted_loops(&self.db)
            .0
            .iter()
            .find(|fact| &fact.site.loop_path == loop_path)
    }

    pub(in crate::fixups) fn counted_slice_loop(
        &self,
        function: FunctionId<'_>,
        loop_path: &AstPath,
    ) -> Option<&CountedSliceLoopFact<'_>> {
        let input = *self.functions.get(&function.as_id())?;
        input
            .counted_loops(&self.db)
            .1
            .iter()
            .find(|fact| &fact.site.loop_path == loop_path)
    }

    pub(in crate::fixups) fn cast_at(
        &self,
        function: FunctionId<'_>,
        path: &AstPath,
    ) -> Option<&CastFact<'_>> {
        let input = *self.functions.get(&function.as_id())?;
        let definitions = self.definitions?;
        input
            .casts(&self.db, self.all_functions, definitions)
            .iter()
            .find(|fact| &fact.site.path == path)
    }

    pub(in crate::fixups) fn place(
        &self,
        function: FunctionId<'_>,
        path: &AstPath,
    ) -> Option<&PlaceFact<'_>> {
        let input = *self.functions.get(&function.as_id())?;
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
        let input = *self.functions.get(&function.as_id())?;
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
        let input = *self.functions.get(&function.as_id())?;
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
        let Some(function) = self.functions.iter().find_map(|(&function, &input)| {
            input
                .bindings_typed(&self.db)
                .iter()
                .any(|fact| fact.id == binding)
                .then_some(function)
        }) else {
            return false;
        };
        self.binding_requires_mut(FunctionId::from_id(function), binding)
    }

    pub(in crate::fixups) fn borrow_alias_reasons_by_binding(
        &self,
        binding: BindingId<'_>,
    ) -> Option<BTreeSet<BorrowAliasReason>> {
        self.functions.values().find_map(|&input| {
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
        let Some(&input) = self.functions.get(&function.as_id()) else {
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
        let Some(&input) = self.functions.get(&function.as_id()) else {
            return &[];
        };
        input.array_element_pointer_origins(&self.db)
    }

    pub(in crate::fixups) fn null_check_dominance_at(
        &self,
        function: FunctionId<'_>,
        deref_path: &AstPath,
    ) -> Option<&NullCheckDominanceFact<'_>> {
        let input = *self.functions.get(&function.as_id())?;
        input
            .null_check_dominance(&self.db)
            .iter()
            .find(|fact| facts::walk::paths_overlap(&fact.deref_site.path.0, &deref_path.0))
    }

    pub(in crate::fixups) fn atomic_locals(&self) -> &[AtomicLocalFact<'_>] {
        self.all_functions.atomic_locals(&self.db)
    }

    pub(in crate::fixups) fn pointer_option_safety_of(
        &self,
        function: FunctionId<'_>,
        binding: BindingId<'_>,
    ) -> Option<&PointerOptionSafetyFact<'_>> {
        let input = *self.functions.get(&function.as_id())?;
        let definitions = self.definitions?;
        input
            .pointer_option_safety(&self.db, definitions)
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
        let input = *self.functions.get(&function.as_id())?;
        let definitions = self.definitions?;
        input
            .pointer_option_safety(&self.db, definitions)
            .1
            .iter()
            .find(|fact| &fact.site.path == path)
    }

    pub(in crate::fixups) fn c_string_literal(
        &self,
        function: FunctionId<'_>,
        receiver_path: &AstPath,
    ) -> Option<&CStringLiteralFact<'_>> {
        let input = *self.functions.get(&function.as_id())?;
        input
            .c_string_literals(&self.db)
            .iter()
            .find(|fact| &fact.receiver_path == receiver_path)
    }

    pub(in crate::fixups) fn option_box_local_candidates(
        &self,
        function: FunctionId<'_>,
    ) -> &[OptionBoxLocalCandidate<'_>] {
        let Some(&input) = self.functions.get(&function.as_id()) else {
            return &[];
        };
        &input.option_box(&self.db).0
    }

    pub(in crate::fixups) fn option_box_comparisons(
        &self,
        function: FunctionId<'_>,
    ) -> &[OptionBoxComparison<'_>] {
        let Some(&input) = self.functions.get(&function.as_id()) else {
            return &[];
        };
        &input.option_box(&self.db).1
    }

    pub(in crate::fixups) fn buffer_pointer_fields(
        &self,
        function: FunctionId<'_>,
    ) -> &[BufferPointerFieldFact<'_>] {
        let Some(&input) = self.functions.get(&function.as_id()) else {
            return &[];
        };
        input.buffer_pointer_fields(&self.db)
    }

    pub(in crate::fixups) fn heap_ownership(
        &self,
        function: FunctionId<'_>,
    ) -> &[HeapOwnershipFact<'_>] {
        let Some(&input) = self.functions.get(&function.as_id()) else {
            return &[];
        };
        input.heap_ownership(&self.db)
    }

    pub(in crate::fixups) fn file_ownership(
        &self,
        function: FunctionId<'_>,
    ) -> &[FileOwnershipFact<'_>] {
        let Some(&input) = self.functions.get(&function.as_id()) else {
            return &[];
        };
        input.file_ownership(&self.db)
    }

    pub(in crate::fixups) fn printf_calls(
        &self,
        function: FunctionId<'_>,
    ) -> &[PrintfCallFact<'_>] {
        let Some(&input) = self.functions.get(&function.as_id()) else {
            return &[];
        };
        input.printf_calls(&self.db)
    }

    pub(in crate::fixups) fn callee_alloc_summary(
        &self,
        function: FunctionId<'_>,
    ) -> Option<&CalleeAllocSummaryFact<'_>> {
        let input = *self.functions.get(&function.as_id())?;
        input.callee_alloc_summary(&self.db).as_ref()
    }

    pub(in crate::fixups) fn interprocedural_alloc(
        &self,
    ) -> &(
        Vec<InterproceduralAllocEligibilityFact<'_>>,
        Vec<InterproceduralAllocCallerFact<'_>>,
    ) {
        self.all_functions.interprocedural_alloc(&self.db)
    }

    pub(in crate::fixups) fn callsites(&self) -> &[CallsiteFact<'_>] {
        let Some(definitions) = self.definitions else {
            return &[];
        };
        self.all_functions.callsites(&self.db, definitions)
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
        let Some(definitions) = self.definitions else {
            return &[];
        };
        self.all_functions.ptr_len_slices(&self.db, definitions)
    }

    pub(in crate::fixups) fn string_param_lifts(&self) -> &[StringParamLiftFact<'_>] {
        let Some(definitions) = self.definitions else {
            return &[];
        };
        self.all_functions.string_param_lifts(&self.db, definitions)
    }

    pub(in crate::fixups) fn lazy_init_singletons(&self) -> &[LazyInitSingletonFact<'_>] {
        let Some(definitions) = self.definitions else {
            return &[];
        };
        self.all_functions
            .lazy_init_singletons(&self.db, definitions)
    }

    #[expect(
        dead_code,
        reason = "query API surface not yet wired into a fixup rule"
    )]
    pub(in crate::fixups) fn struct_field_ownership(&self) -> &[StructFieldOwnershipFact] {
        let Some(definitions) = self.definitions else {
            return &[];
        };
        definitions.struct_field_ownership(&self.db)
    }

    pub(in crate::fixups) fn anonymous_structs(&self) -> &[AnonymousStructFact] {
        let Some(definitions) = self.definitions else {
            return &[];
        };
        definitions.anonymous_structs(&self.db)
    }

    pub(in crate::fixups) fn atomic_globals(&self) -> &[AtomicGlobalFact] {
        let Some(definitions) = self.definitions else {
            return &[];
        };
        self.all_functions.atomic_globals(&self.db, definitions)
    }

    pub(in crate::fixups) fn function_by_item_index(
        &self,
        item_index: usize,
    ) -> Option<FunctionId<'_>> {
        self.base.function_by_item_index(item_index)
    }

    pub(in crate::fixups) fn function_item_index(&self, function: FunctionId<'_>) -> Option<usize> {
        self.base.function_item_index(function)
    }

    pub(in crate::fixups) fn function_name(&self, function: FunctionId<'_>) -> Option<&str> {
        self.base.function_name(function)
    }

    pub(in crate::fixups) fn function_by_name(&self, name: &str) -> Option<FunctionId<'_>> {
        self.base.function_by_name(name)
    }

    pub(in crate::fixups) fn function_facts(&self) -> Vec<FunctionFact<'_>> {
        self.base.function_facts()
    }

    pub(in crate::fixups) fn binding_facts(&self) -> Vec<BindingFact<'_>> {
        self.base.binding_facts()
    }

    pub(in crate::fixups) fn binding_by_local_path<'db>(
        &'db self,
        function: FunctionId<'db>,
        name: &str,
        path: &AstPath,
    ) -> Option<BindingId<'db>> {
        let input = *self.functions.get(&function.as_id())?;
        facts::binding_by_local_path(&input.bindings_typed(&self.db), function, name, path)
    }

    pub(in crate::fixups) fn binding_type(&self, binding: BindingId<'_>) -> Option<String> {
        self.functions.values().find_map(|&input| {
            facts::binding_type(&input.binding_types_typed(&self.db), binding).map(str::to_string)
        })
    }

    pub(in crate::fixups) fn binding_type_ast(
        &self,
        binding: BindingId<'_>,
    ) -> Option<crate::rust_ast::Type> {
        self.functions.values().find_map(|&input| {
            facts::binding_type_ast(&input.binding_types_typed(&self.db), binding).cloned()
        })
    }

    pub(in crate::fixups) fn binding_name(&self, binding: BindingId<'_>) -> Option<String> {
        self.functions.values().find_map(|&input| {
            facts::binding_name(&input.bindings_typed(&self.db), binding).map(str::to_string)
        })
    }

    pub(in crate::fixups) fn bindings_read_under(
        &self,
        function: FunctionId<'_>,
        name: &str,
        path: &AstPath,
    ) -> Vec<BindingId<'_>> {
        let Some(&input) = self.functions.get(&function.as_id()) else {
            return Vec::new();
        };
        let query_path = facts::def_use_query_path(path);
        let bindings = input.bindings_typed(&self.db);
        input
            .def_use(&self.db)
            .iter()
            .filter(|fact| {
                bindings
                    .iter()
                    .any(|binding| binding.id == fact.binding && binding.name == name)
                    && fact
                        .reads
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
        let Some(&input) = self.functions.get(&function.as_id()) else {
            return Vec::new();
        };
        let query_path = facts::def_use_query_path(path);
        let bindings = input.bindings_typed(&self.db);
        input
            .def_use(&self.db)
            .iter()
            .filter(|fact| {
                bindings
                    .iter()
                    .any(|binding| binding.id == fact.binding && binding.name == name)
                    && fact
                        .writes
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
        let Some(&input) = self.functions.get(&function.as_id()) else {
            return Vec::new();
        };
        let binding_types = input.binding_types_typed(&self.db);
        input
            .bindings_typed(&self.db)
            .iter()
            .filter_map(|binding| {
                let rendered = facts::binding_type(&binding_types, binding.id)?;
                Some((binding.name.clone(), rendered.to_string()))
            })
            .collect()
    }

    pub(in crate::fixups) fn ascii_numeric_string(
        &self,
        binding: BindingId<'_>,
    ) -> Option<&AsciiNumericStringFact<'_>> {
        self.functions.values().find_map(|&input| {
            input
                .ascii_numeric_strings(&self.db)
                .iter()
                .find(|fact| fact.binding == binding)
        })
    }

    pub(in crate::fixups) fn string_pointer_view_at<'db>(
        &'db self,
        function: FunctionId<'db>,
        path: &AstPath,
    ) -> Option<&'db facts::StringPointerViewFact<'db>> {
        facts::string_pointer_view(self.string_pointer_views(function), function, path)
    }
}
