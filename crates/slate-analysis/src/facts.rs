use crate::counted_loops::{self, CountedLoop};
use crate::identity::{FunctionIdentity, identify};
use crate::inline_temporaries;
use crate::pointer::{self, PointerFact};
use crate::program::{Program, ProgramBinding, UnitId};
use foldhash::{HashMap, HashMapExt, HashSet, HashSetExt};
use slate_parser::ast::NodeId;
use slate_parser::ir::BindingId;

#[derive(Debug, Default, Clone, PartialEq, Eq)]
pub struct BindingFacts {
    pub pointer: Option<PointerFact>,
}

#[derive(Debug, Default, Clone, PartialEq, Eq)]
pub struct ValueFacts {}

#[derive(Debug, Default)]
pub struct Facts {
    bindings: HashMap<ProgramBinding, BindingFacts>,
    values: HashMap<NodeId, ValueFacts>,
    identities: HashMap<ProgramBinding, FunctionIdentity>,
    counted_loops: Vec<HashMap<BindingId, CountedLoop>>,
    inline_temporaries: Vec<HashSet<BindingId>>,
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

    pub fn value(&self, node: NodeId) -> Option<&'f ValueFacts> {
        self.facts.values.get(&node)
    }

    pub fn counted_loop(&self, statement: BindingId) -> Option<&'f CountedLoop> {
        self.facts
            .counted_loops
            .get(self.unit.0 as usize)
            .and_then(|loops| loops.get(&statement))
    }

    pub fn inline_temporary(&self, temporary: BindingId) -> bool {
        self.facts
            .inline_temporaries
            .get(self.unit.0 as usize)
            .is_some_and(|temporaries| temporaries.contains(&temporary))
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
    let counted_loops = program
        .units()
        .map(|(_, program_unit)| {
            let mut loops = HashMap::new();
            for function in &program_unit.module.functions {
                counted_loops::analyze(&function.value, &mut loops);
            }
            loops
        })
        .collect();
    let inline_temporaries = program
        .units()
        .map(|(_, program_unit)| {
            let mut temporaries = HashSet::new();
            let captures = inline_temporaries::record_captures(program_unit.module);
            for function in &program_unit.module.functions {
                inline_temporaries::analyze(&function.value, &captures, &mut temporaries);
            }
            temporaries
        })
        .collect();
    let pointers = pointer::analyze(program, &identities);
    let bindings = pointers
        .bindings
        .into_iter()
        .map(|(binding, fact)| {
            (
                binding,
                BindingFacts {
                    pointer: Some(fact),
                },
            )
        })
        .collect();
    Facts {
        bindings,
        values: HashMap::new(),
        identities,
        counted_loops,
        inline_temporaries,
    }
}
