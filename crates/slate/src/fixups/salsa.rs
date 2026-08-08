use std::collections::{BTreeSet, HashMap};

use crate::fixups::facts::{
    self, ArrayElementPointerOriginFact, AstPath, AtomicLocalFact, BindingFact, BindingId,
    BindingTypeFact, BorrowAliasFact, BorrowAliasReason, BorrowAliasState, BufferPointerFieldFact,
    CallSignatureFact, CallSignatureSource, CalleeAllocSummaryFact, CallsiteFact, CastFact,
    ControlFlowFact, ControlFlowSubject, CountedLoopFact, CountedSliceLoopFact, DefUseFact,
    EffectFact, EffectSubject, FixupFacts, FunctionFact, FunctionId, HeapOwnershipFact,
    InterproceduralAllocCallerFact, InterproceduralAllocEligibilityFact, LoopFact,
    NullCheckDominanceFact, OptionBoxComparison, OptionBoxLocalCandidate, PlaceFact,
    PointerComparisonFact, PointerOptionSafetyFact, StructFieldOwnershipFact, ValueFact,
};
use crate::fixups::query::TouchedItems;
use crate::rust_ast::{EnumDef, FnDef, Item, Program, RecordDef, StructDef};
use salsa::Setter;

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

#[salsa::input]
pub(in crate::fixups) struct FunctionInput {
    pub(in crate::fixups) function: FunctionId,
    #[returns(ref)]
    pub(in crate::fixups) body: FnDef,
    #[returns(ref)]
    pub(in crate::fixups) bindings: Vec<BindingFact>,
    #[returns(ref)]
    pub(in crate::fixups) binding_types: Vec<BindingTypeFact>,
    #[returns(ref)]
    pub(in crate::fixups) loops: Vec<LoopFact>,
    #[returns(ref)]
    pub(in crate::fixups) call_signatures: Vec<CallSignatureFact>,
    #[returns(ref)]
    pub(in crate::fixups) callsites: Vec<CallsiteFact>,
}

#[salsa::input]
pub(in crate::fixups) struct DefinitionsInput {
    #[returns(ref)]
    pub(in crate::fixups) records: Vec<RecordDef>,
    #[returns(ref)]
    pub(in crate::fixups) structs: Vec<StructDef>,
    #[returns(ref)]
    pub(in crate::fixups) enums: Vec<EnumDef>,
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
}

#[salsa::tracked]
impl FunctionInput {
    #[salsa::tracked(returns(ref))]
    fn def_use(self, db: &dyn FixupDb) -> Vec<DefUseFact> {
        let local_facts = FixupFacts {
            bindings: self.bindings(db).clone(),
            ..FixupFacts::default()
        };
        facts::def_use::collect_for_function(*self.function(db), self.body(db), &local_facts)
    }

    #[salsa::tracked(returns(ref))]
    fn effects(self, db: &dyn FixupDb) -> Vec<EffectFact> {
        facts::effects::collect_for_function(*self.function(db), self.body(db))
    }

    #[salsa::tracked(returns(ref))]
    fn values(self, db: &dyn FixupDb) -> Vec<ValueFact> {
        let local_facts = FixupFacts {
            bindings: self.bindings(db).clone(),
            ..FixupFacts::default()
        };
        facts::values::collect_for_function(*self.function(db), self.body(db), &local_facts)
    }

    #[salsa::tracked(returns(ref))]
    fn strings(self, db: &dyn FixupDb) -> facts::strings::Collected {
        let local_facts = FixupFacts {
            bindings: self.bindings(db).clone(),
            binding_types: self.binding_types(db).clone(),
            ..FixupFacts::default()
        };
        facts::strings::collect_for_function(*self.function(db), self.body(db), &local_facts)
    }

    #[salsa::tracked(returns(ref))]
    fn counted_loops(self, db: &dyn FixupDb) -> (Vec<CountedLoopFact>, Vec<CountedSliceLoopFact>) {
        let mut local_facts = FixupFacts {
            bindings: self.bindings(db).clone(),
            loops: self.loops(db).clone(),
            ..FixupFacts::default()
        };
        facts::counted_loop::collect_for_function(
            *self.function(db),
            self.body(db),
            &mut local_facts,
        );
        (local_facts.counted_loops, local_facts.counted_slice_loops)
    }

    #[salsa::tracked(returns(ref))]
    fn casts(self, db: &dyn FixupDb) -> Vec<CastFact> {
        let local_facts = FixupFacts {
            bindings: self.bindings(db).clone(),
            functions: vec![FunctionFact {
                id: *self.function(db),
                name: self.body(db).name.clone(),
                item_index: 0,
            }],
            call_signatures: self.call_signatures(db).clone(),
            callsites: self.callsites(db).clone(),
            ..FixupFacts::default()
        };
        facts::casts::collect_for_function(*self.function(db), self.body(db), &local_facts)
    }

    #[salsa::tracked(returns(ref))]
    fn places(self, db: &dyn FixupDb) -> Vec<PlaceFact> {
        facts::places::collect_for_function(*self.function(db), self.body(db))
    }

    #[salsa::tracked(returns(ref))]
    fn control_flow(self, db: &dyn FixupDb) -> Vec<ControlFlowFact> {
        facts::control_flow::collect_for_function(*self.function(db), self.body(db))
    }

    #[salsa::tracked(returns(ref))]
    fn borrow_alias(self, db: &dyn FixupDb) -> Vec<BorrowAliasFact> {
        let local_facts = FixupFacts {
            bindings: self.bindings(db).clone(),
            ..FixupFacts::default()
        };
        facts::borrow_alias::collect_for_function(*self.function(db), self.body(db), &local_facts)
    }

    #[salsa::tracked(returns(ref))]
    fn array_element_pointer_origins(self, db: &dyn FixupDb) -> Vec<ArrayElementPointerOriginFact> {
        let local_facts = FixupFacts {
            bindings: self.bindings(db).clone(),
            binding_types: self.binding_types(db).clone(),
            def_use: self.def_use(db).clone(),
            ..FixupFacts::default()
        };
        facts::array_element_pointer_origin::collect_for_function(
            *self.function(db),
            self.body(db),
            &local_facts,
        )
    }

    #[salsa::tracked(returns(ref))]
    fn null_check_dominance(self, db: &dyn FixupDb) -> Vec<NullCheckDominanceFact> {
        let local_facts = FixupFacts {
            bindings: self.bindings(db).clone(),
            control_flow: self.control_flow(db).clone(),
            ..FixupFacts::default()
        };
        facts::null_check_dominance::collect_for_function(
            *self.function(db),
            self.body(db),
            &local_facts,
        )
    }

    #[salsa::tracked(returns(ref))]
    fn atomic_locals(self, db: &dyn FixupDb) -> Vec<AtomicLocalFact> {
        facts::atomic_locals::collect_for_function(*self.function(db), self.body(db))
    }

    #[salsa::tracked(returns(ref))]
    fn pointer_option_safety(
        self,
        db: &dyn FixupDb,
        definitions: DefinitionsInput,
    ) -> (Vec<PointerOptionSafetyFact>, Vec<PointerComparisonFact>) {
        let local_facts = FixupFacts {
            bindings: self.bindings(db).clone(),
            binding_types: self.binding_types(db).clone(),
            ..FixupFacts::default()
        };
        facts::pointer_option_safety::collect_for_function(
            *self.function(db),
            self.body(db),
            &local_facts,
            definitions.union_records(db),
        )
    }

    #[salsa::tracked(returns(ref))]
    fn option_box(
        self,
        db: &dyn FixupDb,
    ) -> (Vec<OptionBoxLocalCandidate>, Vec<OptionBoxComparison>) {
        let local_facts = FixupFacts {
            bindings: self.bindings(db).clone(),
            ..FixupFacts::default()
        };
        facts::option_box_locals::collect_for_function(
            *self.function(db),
            self.body(db),
            &local_facts,
        )
    }

    #[salsa::tracked(returns(ref))]
    fn buffer_pointer_fields(self, db: &dyn FixupDb) -> Vec<BufferPointerFieldFact> {
        let local_facts = FixupFacts {
            bindings: self.bindings(db).clone(),
            binding_types: self.binding_types(db).clone(),
            ..FixupFacts::default()
        };
        facts::buffer_cursor::collect_for_function(
            *self.function(db),
            &self.body(db).body,
            &local_facts,
        )
    }

    #[salsa::tracked(returns(ref))]
    fn heap_ownership(self, db: &dyn FixupDb) -> Vec<HeapOwnershipFact> {
        let local_facts = FixupFacts {
            bindings: self.bindings(db).clone(),
            ..FixupFacts::default()
        };
        facts::heap_ownership::collect_for_function(
            *self.function(db),
            &self.body(db).body,
            &local_facts,
        )
    }

    #[salsa::tracked(returns(ref))]
    fn callee_alloc_summary(self, db: &dyn FixupDb) -> Option<CalleeAllocSummaryFact> {
        facts::callee_alloc_summary::collect_for_function(*self.function(db), self.body(db))
    }
}

pub(in crate::fixups) struct SalsaFacts {
    db: Database,
    functions: HashMap<FunctionId, FunctionInput>,
    definitions: Option<DefinitionsInput>,
}

impl SalsaFacts {
    pub(in crate::fixups) fn new() -> Self {
        Self {
            db: Database::default(),
            functions: HashMap::new(),
            definitions: None,
        }
    }

    pub(in crate::fixups) fn sync_all(&mut self, program: &Program, facts: &FixupFacts) {
        self.sync_definitions(program);
        for function_fact in &facts.functions {
            self.sync_function(program, facts, function_fact);
        }
        self.functions
            .retain(|id, _| facts.functions.iter().any(|fact| fact.id == *id));
    }

    pub(in crate::fixups) fn sync_touched(
        &mut self,
        pre_edit_facts: &FixupFacts,
        program: &Program,
        touched: &TouchedItems,
    ) {
        let facts = pre_edit_facts;
        if touched.unbounded {
            self.sync_all(program, facts);
            return;
        }
        for &item_index in &touched.in_place {
            if let Some(function_fact) = facts
                .functions
                .iter()
                .find(|fact| fact.item_index == item_index)
            {
                self.sync_function(program, facts, function_fact);
            }
        }
        for &item_index in &touched.removed {
            if let Some(function_fact) = facts
                .functions
                .iter()
                .find(|fact| fact.item_index == item_index)
            {
                self.functions.remove(&function_fact.id);
            }
        }
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
        match self.definitions {
            Some(input) => {
                input.set_records(&mut self.db).to(records);
                input.set_structs(&mut self.db).to(structs);
                input.set_enums(&mut self.db).to(enums);
            }
            None => {
                self.definitions = Some(DefinitionsInput::new(&self.db, records, structs, enums));
            }
        }
    }

    fn sync_function(
        &mut self,
        program: &Program,
        facts: &FixupFacts,
        function_fact: &FunctionFact,
    ) {
        let Some(Item::Fn(body)) = program.items.get(function_fact.item_index) else {
            return;
        };
        let bindings: Vec<BindingFact> = facts
            .bindings
            .iter()
            .filter(|binding| binding.function == function_fact.id)
            .cloned()
            .collect();
        let binding_types: Vec<BindingTypeFact> = facts
            .binding_types
            .iter()
            .filter(|binding_type| {
                bindings
                    .iter()
                    .any(|binding| binding.id == binding_type.binding)
            })
            .cloned()
            .collect();
        let loops: Vec<LoopFact> = facts
            .loops
            .iter()
            .filter(|loop_fact| loop_fact.function == function_fact.id)
            .cloned()
            .collect();
        let callsites: Vec<CallsiteFact> = facts
            .callsites
            .iter()
            .filter(|callsite| callsite.site.function == function_fact.id)
            .cloned()
            .collect();
        let call_signatures: Vec<CallSignatureFact> = facts
            .call_signatures
            .iter()
            .filter(|signature| signature.source == CallSignatureSource::Function(function_fact.id))
            .cloned()
            .collect();
        match self.functions.get(&function_fact.id) {
            Some(&input) => {
                input.set_body(&mut self.db).to(body.clone());
                input.set_bindings(&mut self.db).to(bindings);
                input.set_binding_types(&mut self.db).to(binding_types);
                input.set_loops(&mut self.db).to(loops);
                input.set_call_signatures(&mut self.db).to(call_signatures);
                input.set_callsites(&mut self.db).to(callsites);
            }
            None => {
                let input = FunctionInput::new(
                    &self.db,
                    function_fact.id,
                    body.clone(),
                    bindings,
                    binding_types,
                    loops,
                    call_signatures,
                    callsites,
                );
                self.functions.insert(function_fact.id, input);
            }
        }
    }

    pub(in crate::fixups) fn def_use(
        &self,
        function: FunctionId,
        binding: BindingId,
    ) -> Option<&DefUseFact> {
        let input = *self.functions.get(&function)?;
        input
            .def_use(&self.db)
            .iter()
            .find(|fact| fact.binding == binding)
    }

    pub(in crate::fixups) fn effect(
        &self,
        function: FunctionId,
        subject: EffectSubject,
        path: &AstPath,
    ) -> Option<&EffectFact> {
        let input = *self.functions.get(&function)?;
        input
            .effects(&self.db)
            .iter()
            .find(|fact| fact.subject == subject && &fact.site.path == path)
    }

    pub(in crate::fixups) fn values_for(&self, function: FunctionId) -> &[ValueFact] {
        let Some(&input) = self.functions.get(&function) else {
            return &[];
        };
        input.values(&self.db)
    }

    pub(in crate::fixups) fn string_buffer_at(
        &self,
        function: FunctionId,
        path: &AstPath,
    ) -> Option<&facts::StringBufferFact> {
        let input = *self.functions.get(&function)?;
        input
            .strings(&self.db)
            .buffers
            .iter()
            .find(|buffer| &buffer.site.path == path)
    }

    pub(in crate::fixups) fn string_buffer(
        &self,
        binding: BindingId,
    ) -> Option<&facts::StringBufferFact> {
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
        function: FunctionId,
    ) -> &[facts::StringPointerViewFact] {
        let Some(&input) = self.functions.get(&function) else {
            return &[];
        };
        &input.strings(&self.db).pointer_views
    }

    pub(in crate::fixups) fn string_libc_use(
        &self,
        function: FunctionId,
        path: &AstPath,
    ) -> Option<&facts::StringLibcUseFact> {
        let input = *self.functions.get(&function)?;
        input
            .strings(&self.db)
            .libc_uses
            .iter()
            .find(|libc| &libc.site.path == path)
    }

    pub(in crate::fixups) fn counted_loop(
        &self,
        function: FunctionId,
        loop_path: &AstPath,
    ) -> Option<&CountedLoopFact> {
        let input = *self.functions.get(&function)?;
        input
            .counted_loops(&self.db)
            .0
            .iter()
            .find(|fact| &fact.site.loop_path == loop_path)
    }

    pub(in crate::fixups) fn counted_slice_loop(
        &self,
        function: FunctionId,
        loop_path: &AstPath,
    ) -> Option<&CountedSliceLoopFact> {
        let input = *self.functions.get(&function)?;
        input
            .counted_loops(&self.db)
            .1
            .iter()
            .find(|fact| &fact.site.loop_path == loop_path)
    }

    pub(in crate::fixups) fn cast_at(
        &self,
        function: FunctionId,
        path: &AstPath,
    ) -> Option<&CastFact> {
        let input = *self.functions.get(&function)?;
        input
            .casts(&self.db)
            .iter()
            .find(|fact| &fact.site.path == path)
    }

    pub(in crate::fixups) fn place(
        &self,
        function: FunctionId,
        path: &AstPath,
    ) -> Option<&PlaceFact> {
        let input = *self.functions.get(&function)?;
        input
            .places(&self.db)
            .iter()
            .find(|fact| &fact.site.path == path)
    }

    pub(in crate::fixups) fn control_flow(
        &self,
        function: FunctionId,
        subject: ControlFlowSubject,
        path: &AstPath,
    ) -> Option<&ControlFlowFact> {
        let input = *self.functions.get(&function)?;
        input
            .control_flow(&self.db)
            .iter()
            .find(|fact| fact.subject == subject && &fact.site.path == path)
    }

    pub(in crate::fixups) fn borrow_alias_reasons(
        &self,
        function: FunctionId,
        binding: BindingId,
    ) -> Option<&BTreeSet<BorrowAliasReason>> {
        let input = *self.functions.get(&function)?;
        input
            .borrow_alias(&self.db)
            .iter()
            .find(|fact| fact.binding == binding)
            .map(|fact| &fact.reasons)
    }

    pub(in crate::fixups) fn binding_requires_mut(
        &self,
        function: FunctionId,
        binding: BindingId,
    ) -> bool {
        let Some(&input) = self.functions.get(&function) else {
            return false;
        };
        let borrow_alias = input.borrow_alias(&self.db);
        let is_mut = |id: BindingId| {
            borrow_alias
                .iter()
                .find(|fact| fact.binding == id)
                .is_some_and(|fact| fact.state != BorrowAliasState::ReadOnly)
        };
        if is_mut(binding) {
            return true;
        }
        let Some(target) = input.bindings(&self.db).iter().find(|b| b.id == binding) else {
            return false;
        };
        input
            .bindings(&self.db)
            .iter()
            .filter(|other| other.name == target.name)
            .any(|other| is_mut(other.id))
    }

    pub(in crate::fixups) fn array_element_pointer_origins(
        &self,
        function: FunctionId,
    ) -> &[ArrayElementPointerOriginFact] {
        let Some(&input) = self.functions.get(&function) else {
            return &[];
        };
        input.array_element_pointer_origins(&self.db)
    }

    pub(in crate::fixups) fn null_check_dominance_at(
        &self,
        function: FunctionId,
        deref_path: &AstPath,
    ) -> Option<&NullCheckDominanceFact> {
        let input = *self.functions.get(&function)?;
        input
            .null_check_dominance(&self.db)
            .iter()
            .find(|fact| facts::walk::paths_overlap(&fact.deref_site.path.0, &deref_path.0))
    }

    pub(in crate::fixups) fn atomic_locals(&self) -> Vec<&AtomicLocalFact> {
        self.functions
            .values()
            .flat_map(|&input| input.atomic_locals(&self.db).iter())
            .collect()
    }

    pub(in crate::fixups) fn pointer_option_safety_of(
        &self,
        function: FunctionId,
        binding: BindingId,
    ) -> Option<&PointerOptionSafetyFact> {
        let input = *self.functions.get(&function)?;
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
        function: FunctionId,
        path: &AstPath,
    ) -> Option<&PointerComparisonFact> {
        let input = *self.functions.get(&function)?;
        let definitions = self.definitions?;
        input
            .pointer_option_safety(&self.db, definitions)
            .1
            .iter()
            .find(|fact| &fact.site.path == path)
    }

    pub(in crate::fixups) fn option_box_local_candidates(
        &self,
        function: FunctionId,
    ) -> &[OptionBoxLocalCandidate] {
        let Some(&input) = self.functions.get(&function) else {
            return &[];
        };
        &input.option_box(&self.db).0
    }

    pub(in crate::fixups) fn option_box_comparisons(
        &self,
        function: FunctionId,
    ) -> &[OptionBoxComparison] {
        let Some(&input) = self.functions.get(&function) else {
            return &[];
        };
        &input.option_box(&self.db).1
    }

    pub(in crate::fixups) fn buffer_pointer_fields(
        &self,
        function: FunctionId,
    ) -> &[BufferPointerFieldFact] {
        let Some(&input) = self.functions.get(&function) else {
            return &[];
        };
        input.buffer_pointer_fields(&self.db)
    }

    pub(in crate::fixups) fn heap_ownership(&self, function: FunctionId) -> &[HeapOwnershipFact] {
        let Some(&input) = self.functions.get(&function) else {
            return &[];
        };
        input.heap_ownership(&self.db)
    }

    pub(in crate::fixups) fn callee_alloc_summary(
        &self,
        function: FunctionId,
    ) -> Option<&CalleeAllocSummaryFact> {
        let input = *self.functions.get(&function)?;
        input.callee_alloc_summary(&self.db).as_ref()
    }

    pub(in crate::fixups) fn interprocedural_alloc(
        &self,
    ) -> (
        Vec<InterproceduralAllocEligibilityFact>,
        Vec<InterproceduralAllocCallerFact>,
    ) {
        let functions: Vec<facts::interprocedural_alloc_eligibility::FunctionSummary> = self
            .functions
            .values()
            .map(
                |&input| facts::interprocedural_alloc_eligibility::FunctionSummary {
                    id: *input.function(&self.db),
                    name: input.body(&self.db).name.as_str(),
                    body: &input.body(&self.db).body,
                    bindings: input.bindings(&self.db),
                    callee_alloc_summary: input.callee_alloc_summary(&self.db).as_ref(),
                },
            )
            .collect();
        facts::interprocedural_alloc_eligibility::collect(&functions)
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
}

// do not delete: fact-build timing harness for the slate-kby1 salsa migration.
// covers the acceptance criterion on slate-kby1.1 and gives the same signal
// for each later phase (slate-kby1.3 through .6). keep it until that epic
// tree is closed and its numbers are re-verified against e2e (nextest) green
// at each phase, per slate-kby1's own migration doc (docs/salsa-migration.md).
#[cfg(test)]
mod temp_bench {
    use std::path::Path;
    use std::time::Instant;

    use super::SalsaFacts;
    use crate::fixups::facts;
    use crate::fixups::query::TouchedItems;

    #[test]
    fn timing() {
        let (_, program) =
            crate::api::lowered_program(Path::new("tests/fixtures/function_provenance.c")).unwrap();

        let legacy_start = Instant::now();
        let facts::AnalyzedProgram { facts, .. } = facts::analyze(&program);
        let legacy_cold = legacy_start.elapsed();

        let legacy_start = Instant::now();
        let _ = facts::analyze(&program);
        let legacy_repeat = legacy_start.elapsed();

        println!("functions in fixture: {}", facts.functions.len());

        let read_all = |salsa_facts: &SalsaFacts| {
            for function in &facts.functions {
                for binding in facts.bindings.iter().filter(|b| b.function == function.id) {
                    let _ = salsa_facts.def_use(function.id, binding.id);
                }
            }
        };

        let salsa_cold_start = Instant::now();
        let mut salsa_facts = SalsaFacts::new();
        salsa_facts.sync_all(&program, &facts);
        read_all(&salsa_facts);
        let salsa_cold = salsa_cold_start.elapsed();

        let salsa_full_resync_start = Instant::now();
        salsa_facts.sync_all(&program, &facts);
        read_all(&salsa_facts);
        let salsa_full_resync = salsa_full_resync_start.elapsed();

        let touched_function = facts.functions.first().unwrap();
        let touched = TouchedItems {
            in_place: vec![touched_function.item_index],
            removed: Vec::new(),
            unbounded: false,
        };
        let touched_start = Instant::now();
        salsa_facts.sync_touched(&facts, &program, &touched);
        read_all(&salsa_facts);
        let touched_one = touched_start.elapsed();

        println!("legacy facts::analyze (cold):        {legacy_cold:?}");
        println!("legacy facts::analyze (repeat):       {legacy_repeat:?}");
        println!("salsa cold (sync_all + read all):     {salsa_cold:?}");
        println!("salsa full unconditional re-sync:     {salsa_full_resync:?}");
        println!("salsa sync_touched, 1 of N touched:   {touched_one:?}");
    }
}
