use crate::byte_buffers::{self, ArrayAlias};
use crate::c_string_arrays;
use crate::constant_strings::{self, ConstantString};
use crate::counted_loops::{self, CountedLoop};
use crate::def_use::{BindingMutability, DefUse};
use crate::guarded_heap::{self, GuardedHeap, GuardedHeapStatement, HeapElement};
use crate::identity::{FunctionIdentity, identify};
use crate::pointer::{self, Borrow, HeapStatement, PointerFact};
use crate::printf::{self, PrintfFormat};
use crate::program::{Program, ProgramBinding, UnitId};
use foldhash::{HashMap, HashMapExt, HashSet};
use slate_parser::ast::NodeId;
use slate_parser::ir::{BindingId, for_each_function_ref};

#[derive(Debug, Default, Clone, PartialEq, Eq)]
pub struct BindingFacts {
    pub pointer: Option<PointerFact>,
    pub returns: Option<Borrow>,
    pub constant_string: Option<ConstantString>,
    pub array_alias: Option<ArrayAlias>,
    pub c_string_array: bool,
    pub mutability: Option<BindingMutability>,
}

#[derive(Debug, Default, Clone, PartialEq, Eq)]
pub struct ValueFacts {
    pub elided_release: bool,
}

#[derive(Debug, Default)]
pub struct Facts {
    bindings: HashMap<ProgramBinding, BindingFacts>,
    block_captures: HashMap<(ProgramBinding, BindingId), BindingMutability>,
    values: HashMap<(UnitId, NodeId), ValueFacts>,
    identities: HashMap<ProgramBinding, FunctionIdentity>,
    counted_loops: Vec<HashMap<BindingId, CountedLoop>>,
    guarded_heap: Vec<GuardedHeap>,
    printf_formats: Vec<HashMap<NodeId, PrintfFormat>>,
    heap_statements: HashMap<(UnitId, NodeId), HeapStatement>,
}

impl Facts {
    pub fn unit(&self, unit: UnitId) -> UnitFacts<'_> {
        UnitFacts { unit, facts: self }
    }
}

#[derive(Debug, Clone, Copy)]
pub struct UnitFacts<'f> {
    unit: UnitId,
    facts: &'f Facts,
}

impl<'f> UnitFacts<'f> {
    fn key(&self, binding: BindingId) -> ProgramBinding {
        ProgramBinding {
            unit: self.unit,
            binding,
        }
    }

    pub fn binding(&self, binding: BindingId) -> Option<&'f BindingFacts> {
        self.facts.bindings.get(&self.key(binding))
    }

    pub fn block_capture(
        &self,
        function: BindingId,
        capture: BindingId,
    ) -> Option<&'f BindingMutability> {
        self.facts
            .block_captures
            .get(&(self.key(function), capture))
    }

    pub fn value(&self, node: NodeId) -> Option<&'f ValueFacts> {
        self.facts.values.get(&(self.unit, node))
    }

    pub fn counted_loop(&self, statement: BindingId) -> Option<&'f CountedLoop> {
        self.facts
            .counted_loops
            .get(self.unit.0 as usize)
            .and_then(|loops| loops.get(&statement))
    }

    pub fn guarded_heap_statement(&self, statement: NodeId) -> Option<GuardedHeapStatement> {
        self.facts.guarded_heap[self.unit.0 as usize]
            .statements
            .get(&statement)
            .copied()
    }

    pub fn heap_statement(&self, statement: NodeId) -> Option<HeapStatement> {
        self.facts
            .heap_statements
            .get(&(self.unit, statement))
            .copied()
    }

    pub fn heap_element(&self, pointer: NodeId) -> Option<HeapElement> {
        self.facts.guarded_heap[self.unit.0 as usize]
            .elements
            .get(&pointer)
            .copied()
    }

    pub fn heap_has_uninitialized_slots(&self, binding: BindingId) -> bool {
        self.facts.guarded_heap[self.unit.0 as usize]
            .uninitialized
            .contains(&binding)
    }

    pub fn printf_format(&self, call: NodeId) -> Option<&'f PrintfFormat> {
        self.facts.printf_formats[self.unit.0 as usize].get(&call)
    }

    pub fn identity(&self, function: BindingId) -> FunctionIdentity {
        self.facts
            .identities
            .get(&self.key(function))
            .copied()
            .unwrap_or_default()
    }
}

pub fn analyze(program: &Program<'_>) -> Facts {
    let mut identities = HashMap::new();
    for (unit, program_unit) in program.units() {
        for function in &program_unit.module.functions {
            let identity = identify(function, program_unit.files, program_unit.module.hosted);
            if identity != FunctionIdentity::Unknown {
                identities.insert(
                    ProgramBinding {
                        unit,
                        binding: function.value.id,
                    },
                    identity,
                );
            }
        }
    }
    let counted_loops: Vec<_> = program
        .units()
        .map(|(_, program_unit)| {
            let mut loops = HashMap::new();
            for function in &program_unit.module.functions {
                counted_loops::analyze(&function.value, &mut loops);
            }
            loops
        })
        .collect();
    let pointers = pointer::analyze(program, &identities);
    let mut bindings: HashMap<ProgramBinding, BindingFacts> = pointers
        .bindings
        .into_iter()
        .map(|(binding, fact)| {
            (
                binding,
                BindingFacts {
                    pointer: Some(fact),
                    ..BindingFacts::default()
                },
            )
        })
        .collect();
    for (function, borrow) in pointers.returns {
        bindings.entry(function).or_default().returns = Some(borrow);
    }
    let mut block_captures = HashMap::default();
    let mut guarded_heap = Vec::new();
    let mut printf_formats = Vec::new();
    for (unit, program_unit) in program.units() {
        let module = program_unit.module;
        let literals = constant_strings::literals(module);
        let identity = |binding| match identities.get(&ProgramBinding { unit, binding }) {
            Some(FunctionIdentity::Known(known)) => Some(*known),
            _ => None,
        };
        let callees = constant_strings::known_callees(module, identity, |known| {
            known.string_arguments().is_some()
        });
        let buffer_callees = constant_strings::known_callees(module, identity, |known| {
            known.buffer_arguments().is_some()
        });
        let known = constant_strings::known_callees(module, identity, |_| true);
        printf_formats.push(printf::analyze(program, unit, module, &known, &literals));
        let mut c_arrays = HashSet::default();
        for function in &module.functions {
            c_string_arrays::analyze(&function.value, &known, &module.macro_values, &mut c_arrays);
        }
        for &binding in &c_arrays {
            bindings
                .entry(ProgramBinding { unit, binding })
                .or_default()
                .c_string_array = true;
        }
        let mut aliases = HashMap::new();
        for function in &module.functions {
            byte_buffers::analyze(
                &function.value,
                &buffer_callees,
                &module.macro_values,
                &mut aliases,
            );
        }
        for (binding, alias) in aliases {
            bindings
                .entry(ProgramBinding { unit, binding })
                .or_default()
                .array_alias = Some(alias);
        }
        let c_strings: HashSet<BindingId> = bindings
            .iter()
            .filter(|(key, facts)| {
                key.unit == unit && facts.pointer.is_some_and(|pointer| pointer.c_string)
            })
            .map(|(key, _)| key.binding)
            .collect();
        let context = constant_strings::Context {
            literals: &literals,
            callees: &callees,
            c_strings: &c_strings,
            c_arrays: &c_arrays,
            macro_values: &module.macro_values,
        };
        let mut strings = HashMap::new();
        for function in &module.functions {
            constant_strings::analyze(&function.value, &context, &mut strings);
        }
        let hidden: HashSet<_> = module
            .macro_hidden_arguments
            .values()
            .flatten()
            .copied()
            .collect();
        let mut heap = GuardedHeap::default();
        let types = module
            .types
            .iter()
            .map(|ty| (ty.value.id, &ty.kind))
            .collect();
        for_each_function_ref(module, |function, captures| {
            let Some(uses) = DefUse::for_mutability(function, &types) else {
                return;
            };
            guarded_heap::analyze(
                program,
                unit,
                function,
                &identities,
                &guarded_heap::HeapInputs {
                    hidden: &hidden,
                    uses: &uses,
                    loops: &counted_loops[unit.0 as usize],
                },
                &mut heap,
            );
            for (binding, mutability) in uses.binding_mutability() {
                bindings
                    .entry(ProgramBinding { unit, binding })
                    .or_default()
                    .mutability = Some(mutability);
            }
            let owner = ProgramBinding {
                unit,
                binding: function.id,
            };
            for capture in captures {
                if !capture.by_reference && !hidden.contains(&capture.id) {
                    block_captures.insert((owner, capture.id), uses.mutability(capture.id));
                }
            }
        });
        guarded_heap.push(heap);
        for binding in hidden {
            bindings
                .entry(ProgramBinding { unit, binding })
                .or_default()
                .mutability = None;
        }
        for (binding, bytes) in strings {
            bindings
                .entry(ProgramBinding { unit, binding })
                .or_default()
                .constant_string = Some(bytes);
        }
    }
    let values = pointers
        .elided_releases
        .into_iter()
        .map(|key| {
            (
                key,
                ValueFacts {
                    elided_release: true,
                },
            )
        })
        .collect();
    Facts {
        bindings,
        block_captures,
        values,
        identities,
        counted_loops,
        guarded_heap,
        printf_formats,
        heap_statements: pointers.heap_statements,
    }
}
