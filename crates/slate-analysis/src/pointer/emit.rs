use super::flow::Flow;
use super::length::Lengths;
use super::ownership::OwnershipFacts;
use super::summary::{NodeKind, Shape, Summaries, Summary};
use super::{Borrow, Length, Origin, Owner, PointerFact, PointerFacts, PointerRole};
use crate::program::{FunctionId, Program, ProgramBinding};
use foldhash::{HashMap, HashMapExt, HashSet};
use slate_parser::ir::{Callee, Parameters};

pub(super) fn facts(
    program: &Program<'_>,
    index: &Summaries,
    flow: &Flow,
    ownership: OwnershipFacts,
    lengths: &Lengths,
    unproven: &[Option<Origin>],
    c_strings: &HashSet<(usize, usize)>,
) -> PointerFacts {
    let OwnershipFacts {
        nodes: owned,
        elided_releases,
        heap_statements,
    } = ownership;
    let mut returns = HashMap::new();
    emit_returns(program, index, flow, unproven, &mut returns);
    let mut facts = PointerFacts {
        bindings: emit_bindings(program, index, flow, &owned, lengths, unproven, c_strings),
        returns,
        elided_releases,
        heap_statements,
    };

    copy_to_declarations(program, index, &mut facts);
    facts
}

fn emit_returns(
    program: &Program<'_>,
    index: &Summaries,
    flow: &Flow,
    unproven: &[Option<Origin>],
    returns: &mut HashMap<ProgramBinding, Borrow>,
) {
    let Summaries {
        functions: summaries,
        base,
        ..
    } = index;
    for (function, summary) in summaries.iter().enumerate() {
        let Some(summary) = summary else { continue };
        let Some(entry) = program.function(FunctionId(function as u32)) else {
            continue;
        };
        let Some(node) = summary.returned else {
            continue;
        };
        let global = base[function] + node;
        let class = flow.class_of(global) as usize;
        let item = &summary.nodes[node as usize];
        if let Some(borrow) = flow.borrow(global, item.byte_elements).filter(|_| {
            flow.lifts_return(function) && unproven[class].is_none() && !item.opaque_pointee
        }) {
            returns.insert(
                ProgramBinding {
                    unit: entry.unit,
                    binding: entry.function.value.id,
                },
                borrow,
            );
        }
    }
}

fn emit_bindings(
    program: &Program<'_>,
    index: &Summaries,
    flow: &Flow,
    owned: &HashMap<u32, bool>,
    lengths: &Lengths,
    unproven: &[Option<Origin>],
    c_strings: &HashSet<(usize, usize)>,
) -> HashMap<ProgramBinding, PointerFact> {
    let Summaries {
        functions: summaries,
        base,
        candidates,
        ..
    } = index;
    let mut bindings = HashMap::new();
    for (function, summary) in summaries.iter().enumerate() {
        let Some(summary) = summary else { continue };
        let Some(entry) = program.function(FunctionId(function as u32)) else {
            continue;
        };
        for (node, item) in summary.nodes.iter().enumerate() {
            let role = match item.kind {
                NodeKind::Parameter(_) if candidates[function] => PointerRole::Parameter,
                NodeKind::Local => PointerRole::Local,
                NodeKind::Parameter(_) | NodeKind::Temporary | NodeKind::Return => continue,
            };
            let global = base[function] + node as u32;
            let class = flow.class_of(global) as usize;
            let length = match item.kind {
                NodeKind::Parameter(index) => length_fact(summary, function, index, lengths),
                NodeKind::Local | NodeKind::Temporary | NodeKind::Return => None,
            };
            let borrow = flow
                .borrow(global, item.byte_elements)
                .filter(|borrow| {
                    !item.opaque_pointee
                        && unproven[class].is_none()
                        && (role == PointerRole::Parameter || !borrow.mutable)
                })
                .map(|borrow| Borrow {
                    many: borrow.many || length.is_some(),
                    ..borrow
                });
            let ownership = owned.get(&global);
            let borrow = match ownership {
                Some(&mutable) => Some(Borrow {
                    mutable,
                    many: true,
                    nullable: false,
                }),
                None => borrow,
            };
            bindings.insert(
                ProgramBinding {
                    unit: entry.unit,
                    binding: item.binding,
                },
                PointerFact {
                    role,
                    borrow,
                    length,
                    c_string: matches!(item.kind, NodeKind::Parameter(index) if c_strings.contains(&(function, index))),
                    owner: ownership.map(|_| Owner::Vec),
                },
            );
        }
    }
    bindings
}

fn length_fact(
    summary: &Summary,
    function: usize,
    index: usize,
    lengths: &Lengths,
) -> Option<Length> {
    let &parameter = lengths.get(&(function, index))?;
    let binding = summary.parameters[parameter];
    let forwarded = summary
        .calls
        .iter()
        .flat_map(|site| {
            let callee = site.callee.0 as usize;
            site.shapes
                .iter()
                .enumerate()
                .filter(move |&(index, shape)| {
                    matches!(shape, Shape::Binding(read) if *read == binding)
                        && lengths
                            .iter()
                            .any(|(&(function, _), &length)| function == callee && length == index)
                })
        })
        .count() as u32;
    Some(Length {
        parameter,
        read: summary.reads.get(&binding).copied().unwrap_or(0) > forwarded,
    })
}

fn copy_to_declarations(program: &Program<'_>, index: &Summaries, facts: &mut PointerFacts) {
    let summaries = &index.functions;
    for (unit, entry) in program.units() {
        for declaration in entry
            .module
            .functions
            .iter()
            .filter(|f| f.value.body.is_none())
        {
            let Some(function) = program.callee(unit, &Callee::Direct(declaration.value.id)) else {
                continue;
            };
            let (Some(summary), Some(definition)) = (
                summaries[function.0 as usize].as_ref(),
                program.function(function),
            ) else {
                continue;
            };
            if let Some(&borrow) = facts.returns.get(&ProgramBinding {
                unit: definition.unit,
                binding: definition.function.value.id,
            }) {
                facts.returns.insert(
                    ProgramBinding {
                        unit,
                        binding: declaration.value.id,
                    },
                    borrow,
                );
            }
            let Parameters::Prototype { fixed, .. } = &declaration.value.parameters else {
                continue;
            };
            for (parameter, binding) in fixed.iter().zip(&summary.parameters) {
                if let Some(&fact) = facts.bindings.get(&ProgramBinding {
                    unit: definition.unit,
                    binding: *binding,
                }) && fact.role == PointerRole::Parameter
                {
                    facts.bindings.insert(
                        ProgramBinding {
                            unit,
                            binding: parameter.value.id,
                        },
                        fact,
                    );
                }
            }
        }
    }
}
