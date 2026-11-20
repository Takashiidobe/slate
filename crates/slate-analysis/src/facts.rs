use crate::identity::{FunctionIdentity, identify};
use crate::program::{Program, ProgramBinding, UnitId};
use foldhash::{HashMap, HashMapExt};
use slate_parser::ast::NodeId;
use slate_parser::ir::BindingId;

#[derive(Debug, Default, Clone, PartialEq, Eq)]
pub struct BindingFacts {}

#[derive(Debug, Default, Clone, PartialEq, Eq)]
pub struct ValueFacts {}

#[derive(Debug, Default)]
pub struct Facts {
    bindings: HashMap<ProgramBinding, BindingFacts>,
    values: HashMap<NodeId, ValueFacts>,
    identities: HashMap<ProgramBinding, FunctionIdentity>,
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
    Facts {
        identities,
        ..Facts::default()
    }
}
