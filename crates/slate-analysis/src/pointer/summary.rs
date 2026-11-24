mod body;
mod calls;
mod heap;
mod sources;

const STACK_RED_ZONE: usize = 512 << 10;
const STACK_GROWTH: usize = 16 << 20;

use super::ownership::Owned;
use super::{ESCAPE, MACRO, NULLABLE, OFFSET, SHARED, WRITE};
use crate::def_use::DefUse;
use crate::identity::FunctionIdentity;
use crate::program::{FunctionId, Program, ProgramBinding, UnitId};
use foldhash::{HashMap, HashMapExt, HashSet, HashSetExt};
use heap::Ownership;
use slate_parser::ast::{NodeId, Span};
use slate_parser::ir::{
    self, BindingId, Callee, Evaluation, MacroValues, MacroWrap, NumericType, Parameters, Place,
    PlaceKind, Statement, Type, Value, Visitor,
};
pub use sources::fixed_array_position;
use sources::pointee;
use std::cell::OnceCell;

#[derive(Clone, Copy, PartialEq, Eq)]
pub(super) enum NodeKind {
    Parameter(usize),
    Local,
    Temporary,
    Return,
}

pub(super) struct Node {
    pub(super) binding: BindingId,
    pub(super) kind: NodeKind,
    pub(super) byte_elements: bool,
    pub(super) opaque_pointee: bool,
    pub(super) flags: u8,
    pub(super) raw: bool,
    pub(super) reads: u32,
}

#[derive(Clone)]
pub(super) enum Shape {
    Array {
        length: u64,
        offset: u64,
        element: Type,
        object: Option<BindingId>,
    },
    Allocation {
        pointer: BindingId,
        count: BindingId,
        element: Type,
    },
    Binding(BindingId),
    Constant(u64),
    Other,
}

pub(super) struct CallSite {
    pub(super) callee: FunctionId,
    pub(super) arguments: Vec<(usize, u32)>,
    pub(super) objects: Vec<(usize, BindingId)>,
    pub(super) unproven: Vec<usize>,
    pub(super) nulls: Vec<usize>,
    pub(super) parts: Vec<usize>,
    pub(super) pinned: bool,
    pub(super) shapes: Vec<Shape>,
    pub(super) literals: Vec<bool>,
}

pub(super) struct Summary {
    pub(super) parameters: Vec<BindingId>,
    pub(super) lengths: Vec<bool>,
    pub(super) pointees: Vec<Option<Type>>,
    pub(super) written: HashSet<BindingId>,
    pub(super) stored: HashSet<BindingId>,
    pub(super) reads: HashMap<BindingId, u32>,
    pub(super) nodes: Vec<Node>,
    pub(super) unions: Vec<(u32, u32)>,
    pub(super) copies: Vec<(u32, u32)>,
    pub(super) calls: Vec<CallSite>,
    pub(super) results: Vec<(u32, FunctionId)>,
    pub(super) returned: Option<u32>,
    pub(super) returned_from: Vec<u32>,
    pub(super) projected: Vec<u32>,
    pub(super) address_taken: Vec<FunctionId>,
    pub(super) leaked: HashSet<BindingId>,
    pub(super) automatic: HashSet<BindingId>,
    pub(super) derived: Vec<(u32, BindingId)>,
    pub(super) owned: Vec<Owned>,
    pub(super) opaque: bool,
}

impl Summary {
    pub(super) fn unwritten_parameter(&self, binding: BindingId) -> Option<usize> {
        if self.written.contains(&binding) {
            return None;
        }
        self.parameters
            .iter()
            .position(|&parameter| parameter == binding)
    }
}

pub(super) struct Inputs<'a, 'm> {
    program: &'a Program<'m>,
    identities: &'a HashMap<ProgramBinding, FunctionIdentity>,
    inert: HashSet<ProgramBinding>,
    constants: HashSet<ProgramBinding>,
    pub(super) mutable_statics: HashSet<ProgramBinding>,
    strings: HashSet<ProgramBinding>,
}

impl<'a, 'm> Inputs<'a, 'm> {
    pub(super) fn new(
        program: &'a Program<'m>,
        identities: &'a HashMap<ProgramBinding, FunctionIdentity>,
    ) -> Self {
        let mut inert = HashSet::new();
        for (unit, entry) in program.units() {
            for function in &entry.module.functions {
                if function.value.semantics.memory == Some(ir::MemoryEffects::None) {
                    inert.insert(ProgramBinding {
                        unit,
                        binding: function.value.id,
                    });
                }
            }
        }
        let mut constants = HashSet::new();
        let mut mutable_statics = HashSet::new();
        let mut strings = HashSet::new();
        for (unit, entry) in program.units() {
            for global in &entry.module.globals {
                let binding = ProgramBinding {
                    unit,
                    binding: global.variable.id,
                };
                if global.variable.is_const {
                    constants.insert(binding);
                } else if global.definition
                    && matches!(global.linkage, ir::Linkage::Internal)
                    && !global.variable.access.volatile
                    && !global.variable.access.atomic
                {
                    mutable_statics.insert(binding);
                }
                if crate::constant_strings::literal_code_units(&global.variable)
                    .is_some_and(|units| units.iter().all(|unit| (1..=255).contains(unit)))
                {
                    strings.insert(binding);
                }
            }
        }

        Self {
            program,
            identities,
            inert,
            constants,
            mutable_statics,
            strings,
        }
    }
}

pub(super) fn collect(inputs: &Inputs<'_, '_>) -> Vec<Option<Summary>> {
    use rayon::prelude::*;

    (0..inputs.program.functions().len() as u32)
        .into_par_iter()
        .map(|index| summarize(inputs, FunctionId(index)))
        .collect()
}

pub(super) struct Summaries {
    pub(super) functions: Vec<Option<Summary>>,
    pub(super) base: Vec<u32>,
    pub(super) candidates: Vec<bool>,
    pub(super) parameters: Vec<HashMap<usize, u32>>,
    pub(super) incoming: Vec<Vec<(usize, usize)>>,
}

impl Summaries {
    pub(super) fn new(
        program: &Program<'_>,
        summaries: Vec<Option<Summary>>,
    ) -> (Self, HashSet<ProgramBinding>) {
        let count = summaries.len();
        let mut base = Vec::with_capacity(count + 1);
        base.push(0u32);
        for summary in &summaries {
            let nodes = summary.as_ref().map_or(0, |summary| summary.nodes.len());
            base.push(base[base.len() - 1] + nodes as u32);
        }
        let mut address_taken = vec![false; count];
        let mut leaked_statics = HashSet::new();
        for summary in summaries.iter().flatten() {
            for function in &summary.address_taken {
                address_taken[function.0 as usize] = true;
            }
        }
        for (unit, entry) in program.units() {
            let mut named = Named(Vec::new());
            for global in &entry.module.globals {
                if let Some(initializer) = &global.variable.initializer {
                    ir::walk_value(&mut initializer.clone(), &mut named);
                }
            }
            for binding in named.0 {
                if let Some(function) = program.callee(unit, &Callee::Direct(binding)) {
                    address_taken[function.0 as usize] = true;
                }
                leaked_statics.insert(ProgramBinding { unit, binding });
            }
        }
        let candidates: Vec<bool> = program
            .functions()
            .map(|(id, entry)| {
                candidate(
                    &entry.function.value,
                    program.exported(id),
                    address_taken[id.0 as usize],
                )
            })
            .collect();
        let parameters: Vec<HashMap<usize, u32>> = summaries
            .iter()
            .zip(&base)
            .map(|(summary, &offset)| {
                let mut parameters = HashMap::new();
                for (node, entry) in summary
                    .iter()
                    .flat_map(|summary| summary.nodes.iter().enumerate())
                {
                    if let NodeKind::Parameter(index) = entry.kind {
                        parameters.insert(index, offset + node as u32);
                    }
                }
                parameters
            })
            .collect();

        let mut incoming: Vec<Vec<(usize, usize)>> = vec![Vec::new(); count];
        for (function, summary) in summaries.iter().enumerate() {
            let Some(summary) = summary else { continue };
            for (index, site) in summary.calls.iter().enumerate() {
                incoming[site.callee.0 as usize].push((function, index));
            }
        }

        (
            Self {
                functions: summaries,
                base,
                candidates,
                parameters,
                incoming,
            },
            leaked_statics,
        )
    }

    pub(super) fn count_nodes(&self) -> usize {
        self.base[self.functions.len()] as usize
    }
}

fn candidate(function: &ir::Function, exported: bool, address_taken: bool) -> bool {
    !address_taken
        && !exported
        && function.name != "main"
        && matches!(
            function.parameters,
            Parameters::Prototype {
                variadic: false,
                ..
            }
        )
        && !function.semantics.naked
        && !function.semantics.interrupt
}

#[derive(Default)]
struct Sources {
    roots: Vec<(u32, bool)>,
    objects: Vec<BindingId>,
    null: bool,
    opaque: bool,
    retyped: bool,
}

#[derive(Clone, Copy, PartialEq, Eq)]
enum Use {
    Read,
    Write,
    Address,
}

fn tracked_pointer(ty: &Type) -> Option<bool> {
    let Type::Pointer { pointee, .. } = ty else {
        return None;
    };
    if matches!(**pointee, Type::Function { .. }) {
        return None;
    }
    Some(matches!(
        **pointee,
        Type::Numeric(NumericType::Integer { width: 8, .. })
    ))
}

struct Walker<'a, 'm> {
    program: &'a Program<'m>,
    identities: &'a HashMap<ProgramBinding, FunctionIdentity>,
    inert: &'a HashSet<ProgramBinding>,
    constants: &'a HashSet<ProgramBinding>,
    mutable_statics: &'a HashSet<ProgramBinding>,
    strings: &'a HashSet<ProgramBinding>,
    unit: UnitId,
    nodes: Vec<Node>,
    index: HashMap<BindingId, u32>,
    unions: Vec<(u32, u32)>,
    copies: Vec<(u32, u32)>,
    calls: Vec<CallSite>,
    results: Vec<(u32, FunctionId)>,
    result_nodes: HashMap<NodeId, u32>,
    call_results: HashSet<u32>,
    returned: Option<u32>,
    returned_from: Vec<u32>,
    projected: Vec<u32>,
    address_taken: Vec<FunctionId>,
    automatic: HashSet<BindingId>,
    macro_depth: u32,
    macro_values: &'m MacroValues,
    written: HashSet<BindingId>,
    stored: HashSet<BindingId>,
    reads: HashMap<BindingId, u32>,
    allocations: HashMap<BindingId, (BindingId, Type)>,
    leaked: HashSet<BindingId>,
    derived: Vec<(u32, BindingId)>,
    ownership: Ownership,
    function_body: &'m [Span<Statement>],
    def_use: OnceCell<DefUse>,
    opaque: bool,
}

fn length_type(ty: &Type) -> bool {
    matches!(
        ty,
        Type::Numeric(NumericType::Integer {
            bit_precise: false,
            ..
        })
    )
}

impl Walker<'_, '_> {
    fn register(&mut self, binding: BindingId, ty: &Type, kind: NodeKind) -> Option<u32> {
        let node = self.anonymous(binding, ty, kind)?;
        self.index.insert(binding, node);
        Some(node)
    }

    fn anonymous(&mut self, binding: BindingId, ty: &Type, kind: NodeKind) -> Option<u32> {
        let byte_elements = tracked_pointer(ty)?;
        let node = self.nodes.len() as u32;
        self.nodes.push(Node {
            binding,
            kind,
            byte_elements,
            opaque_pointee: matches!(ty, Type::Pointer { pointee, .. } if **pointee == Type::Void),
            flags: 0,
            raw: false,
            reads: 0,
        });
        Some(node)
    }

    fn node(&self, binding: BindingId) -> Option<u32> {
        self.index.get(&binding).copied()
    }

    fn flag(&mut self, node: u32, flags: u8) {
        self.nodes[node as usize].flags |= flags;
    }

    fn mark(&mut self, value: &Value, flags: u8) {
        let sources = self.sources(value);
        for (node, _) in sources.roots {
            self.flag(node, flags);
        }
        if flags & (WRITE | ESCAPE) != 0 {
            self.stored.extend(sources.objects.iter().copied());
        }
        if flags & ESCAPE != 0 {
            self.leaked.extend(sources.objects);
        }
    }

    fn offset(&mut self, pointer: &Value) {
        for (node, moved) in self.sources(pointer).roots {
            if !moved {
                self.flag(node, OFFSET);
            }
        }
    }

    fn atomic(&mut self, place: &Place) {
        let mut sources = Sources::default();
        self.collect_address_sources(place, false, &mut sources);
        for (node, _) in sources.roots {
            self.flag(node, ESCAPE | WRITE);
        }
    }

    fn flow_return(&mut self, returned: u32, value: &Value) {
        let sources = self.sources(value);
        let element = self.element_of(value);
        for (source, moved) in sources.roots {
            if (moved && element != Some(source))
                || !matches!(
                    self.nodes[source as usize].kind,
                    NodeKind::Local | NodeKind::Parameter(_)
                )
            {
                self.nodes[returned as usize].raw = true;
            }
            self.returned_from.push(source);
        }
        if sources.opaque || sources.retyped {
            self.nodes[returned as usize].raw = true;
        }
        if sources.null {
            self.flag(returned, NULLABLE);
        }
        for object in sources.objects {
            if self.mutable_statics.contains(&ProgramBinding {
                unit: self.unit,
                binding: object,
            }) {
                self.derived.push((returned, object));
            } else {
                self.leaked.insert(object);
            }
        }
    }

    fn macro_mentions(&mut self, named: Named) {
        for binding in named.0 {
            if let Some(node) = self.node(binding) {
                self.flag(node, MACRO);
            }
        }
    }

    fn aligned_local_part(&self, place: &Place) -> bool {
        place.access.align.is_none()
            && match &place.kind {
                PlaceKind::Binding(binding) => self.automatic.contains(binding),
                PlaceKind::ComplexPart { base, .. } => self.aligned_local_part(base),
                PlaceKind::Field {
                    base,
                    index,
                    bits: None,
                } => {
                    self.program.field_aligned(self.unit, &base.ty, *index)
                        && self.aligned_local_part(base)
                }
                #[expect(
                    slate::sensitive_wildcard,
                    reason = "a borrow of a part is proven only along fields and complex parts of an automatic object"
                )]
                _ => false,
            }
    }

    fn store(&mut self, place: &Place, value: &Value) {
        match &place.kind {
            PlaceKind::Binding(binding) => match self.node(*binding) {
                Some(node) => self.assign(node, value),
                None => self.mark(value, ESCAPE),
            },
            PlaceKind::Deref(_)
            | PlaceKind::CompoundLiteral { .. }
            | PlaceKind::Temporary { .. }
            | PlaceKind::ComplexPart { .. }
            | PlaceKind::Field { .. }
            | PlaceKind::Index { .. }
            | PlaceKind::Lane { .. }
            | PlaceKind::Swizzle { .. } => self.mark(value, ESCAPE),
        }
    }

    fn place(&mut self, place: &Place, access: Use) {
        match &place.kind {
            PlaceKind::Binding(binding) => {
                if access == Use::Read {
                    *self.reads.entry(*binding).or_default() += 1;
                    self.ownership.position += 1;
                    self.ownership
                        .last_read
                        .insert(*binding, self.ownership.position);
                } else {
                    self.written.insert(*binding);
                    if access == Use::Write {
                        self.stored.insert(*binding);
                        *self.ownership.writes.entry(*binding).or_default() += 1;
                    }
                }
                if let Some(node) = self.node(*binding) {
                    match access {
                        Use::Read => self.nodes[node as usize].reads += 1,
                        Use::Address => self.flag(node, ESCAPE | WRITE),
                        Use::Write => {}
                    }
                }
            }
            PlaceKind::Deref(pointer) => {
                self.value(pointer);
                if access == Use::Write {
                    self.mark(pointer, WRITE);
                }
            }
            PlaceKind::CompoundLiteral { initializer, .. }
            | PlaceKind::Temporary { initializer, .. } => {
                self.value(initializer);
                self.mark(initializer, ESCAPE);
            }
            PlaceKind::ComplexPart { base, .. }
            | PlaceKind::Field { base, .. }
            | PlaceKind::Swizzle { base, .. } => self.place(base, access),
            PlaceKind::Lane { base, index } => {
                self.place(base, access);
                self.value(index);
            }
            PlaceKind::Index { base, index } => {
                self.value(base);
                self.value(index);
                self.offset(base);
                if access == Use::Write {
                    self.mark(base, WRITE);
                }
            }
        }
        if self.macro_values.get(&place.id).is_some_and(|wraps| {
            wraps
                .iter()
                .any(|wrap| matches!(wrap, MacroWrap::Argument { .. }))
        }) {
            let mut named = Named(Vec::new());
            place_bindings(place, &mut named);
            self.macro_mentions(named);
        }
    }

    fn evaluation(&mut self, evaluation: &Evaluation) {
        self.statements(&evaluation.statements);
        self.value(&evaluation.value);
    }
}

struct Named(Vec<BindingId>);

fn place_bindings(place: &Place, named: &mut Named) {
    match &place.kind {
        PlaceKind::Binding(binding) => named.0.push(*binding),
        PlaceKind::Deref(value) => ir::walk_value(&mut value.clone(), named),
        PlaceKind::Index { base, index } => {
            ir::walk_value(&mut base.clone(), named);
            ir::walk_value(&mut index.clone(), named);
        }
        PlaceKind::Lane { base, index } => {
            place_bindings(base, named);
            ir::walk_value(&mut index.clone(), named);
        }
        PlaceKind::Field { base, .. }
        | PlaceKind::ComplexPart { base, .. }
        | PlaceKind::Swizzle { base, .. } => place_bindings(base, named),
        PlaceKind::CompoundLiteral { initializer, .. }
        | PlaceKind::Temporary { initializer, .. } => {
            ir::walk_value(&mut initializer.clone(), named)
        }
    }
}

impl Visitor for Named {
    fn binding(&mut self, binding: BindingId) {
        self.0.push(binding);
    }
}

fn summarize(inputs: &Inputs<'_, '_>, function: FunctionId) -> Option<Summary> {
    let Inputs {
        program,
        identities,
        inert,
        constants,
        mutable_statics,
        strings,
    } = inputs;
    let entry = program.function(function)?;
    let mut walker = Walker {
        program,
        identities,
        inert,
        constants,
        mutable_statics,
        strings,
        derived: Vec::new(),
        opaque: false,
        unit: entry.unit,
        nodes: Vec::new(),
        index: HashMap::new(),
        unions: Vec::new(),
        copies: Vec::new(),
        calls: Vec::new(),
        results: Vec::new(),
        result_nodes: HashMap::new(),
        call_results: HashSet::new(),
        returned: None,
        returned_from: Vec::new(),
        projected: Vec::new(),
        address_taken: Vec::new(),
        automatic: HashSet::new(),
        macro_depth: 0,
        macro_values: &program.unit(entry.unit).module.macro_values,
        written: HashSet::new(),
        stored: HashSet::new(),
        reads: HashMap::new(),
        allocations: HashMap::new(),
        leaked: HashSet::new(),
        ownership: Ownership::default(),
        function_body: entry.function.body.as_deref().unwrap_or_default(),
        def_use: OnceCell::new(),
    };
    if let Some(return_type) = &entry.function.return_type {
        walker.returned = walker.anonymous(entry.function.value.id, return_type, NodeKind::Return);
    }
    let mut parameters = Vec::new();
    let mut lengths = Vec::new();
    let mut pointees = Vec::new();
    if let Parameters::Prototype { fixed, .. } = &entry.function.parameters {
        for (index, parameter) in fixed.iter().enumerate() {
            parameters.push(parameter.value.id);
            lengths.push(length_type(&parameter.value.ty));
            pointees.push(pointee(&parameter.value.ty).cloned());
            walker.automatic.insert(parameter.value.id);
            walker.register(
                parameter.value.id,
                &parameter.value.ty,
                NodeKind::Parameter(index),
            );
        }
    }
    if let Some(body) = &entry.function.body {
        walker.statements(body);
    }
    for &(source, destination) in &walker.copies {
        if walker.nodes[destination as usize].kind == NodeKind::Local
            && walker.nodes[source as usize].reads > 1
        {
            walker.nodes[source as usize].flags |= SHARED;
        }
    }
    let owned = walker.owned_candidates();
    Some(Summary {
        parameters,
        lengths,
        pointees,
        written: walker.written,
        stored: walker.stored,
        reads: walker.reads,
        nodes: walker.nodes,
        unions: walker.unions,
        copies: walker.copies,
        calls: walker.calls,
        results: walker.results,
        returned: walker.returned,
        returned_from: walker.returned_from,
        projected: walker.projected,
        address_taken: walker.address_taken,
        leaked: walker.leaked,
        automatic: walker.automatic,
        derived: walker.derived,
        owned,
        opaque: walker.opaque,
    })
}
