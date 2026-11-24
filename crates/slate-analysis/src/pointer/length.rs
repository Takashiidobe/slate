use super::borrow;
use super::flow::Flow;
use super::ownership::OwnershipFacts;
use super::summary::{Shape, Summaries};
use crate::counted_loops::{self, CountedLoop};
use crate::def_use::DefUse;
use crate::effects::{Effects, FunctionEffects};
use crate::heap_ir::read_binding;
use crate::program::{FunctionId, Program, ProgramBinding};
use foldhash::{HashMap, HashMapExt, HashSet, HashSetExt};
use slate_parser::ast::NodeId;
use slate_parser::ir::{
    self, BindingId, Function, Place, PlaceKind, Statement, Type, Value, ValueKind, Visitor,
};

pub(super) type Lengths = HashMap<(usize, usize), usize>;

type Pair = (usize, usize, usize);
type Pairs = HashSet<Pair>;

pub(super) fn prove(
    program: &Program<'_>,
    index: &Summaries,
    flow: &Flow,
    owned: &OwnershipFacts,
    mutable_statics: &HashSet<ProgramBinding>,
    counted_loops: &[HashMap<BindingId, CountedLoop>],
) -> (Lengths, Vec<bool>) {
    let mut active = candidate_pairs(index);
    let effects = StaticArrays::new(program, index, mutable_statics, &flow.leaked_statics);

    let mut bounded = HashMap::new();
    loop {
        let mut next: Pairs = active
            .iter()
            .copied()
            .filter(|&pair| {
                proves_pair(
                    program,
                    index,
                    &effects,
                    &active,
                    &mut bounded,
                    counted_loops,
                    pair,
                )
            })
            .collect();
        retain_unique_pairs(&mut next);

        let covered: HashSet<(usize, usize)> = next
            .iter()
            .map(|&(function, pointer, _)| (function, pointer))
            .collect();
        let unproven = borrow::unproven(index, flow, owned, &covered);
        next.retain(|&(function, pointer, _)| {
            can_borrow(index, flow, &unproven, function, pointer)
        });
        if next == active {
            let lengths = next
                .into_iter()
                .map(|(function, pointer, length)| ((function, pointer), length))
                .collect();
            return (lengths, unproven);
        }
        active = next;
    }
}

fn candidate_pairs(index: &Summaries) -> Pairs {
    let Summaries {
        functions: summaries,
        candidates,
        parameters,
        incoming,
        ..
    } = index;
    let mut active = HashSet::new();
    for (function, summary) in summaries.iter().enumerate() {
        let Some(summary) = summary else { continue };
        if !candidates[function] || incoming[function].is_empty() {
            continue;
        }
        let unwritten = |index: usize| !summary.written.contains(&summary.parameters[index]);
        for pointer in 0..summary.parameters.len() {
            if !parameters[function].contains_key(&pointer) || !unwritten(pointer) {
                continue;
            }
            for (length, &integer) in summary.lengths.iter().enumerate() {
                if integer && unwritten(length) {
                    active.insert((function, pointer, length));
                }
            }
        }
    }

    active
}

struct StaticArrays<'a> {
    effects: Effects,
    reaching: HashMap<ProgramBinding, HashSet<FunctionId>>,
    leaked: &'a HashSet<ProgramBinding>,
}

impl<'a> StaticArrays<'a> {
    fn new(
        program: &Program<'_>,
        index: &Summaries,
        mutable_statics: &HashSet<ProgramBinding>,
        leaked: &'a HashSet<ProgramBinding>,
    ) -> Self {
        let summaries = &index.functions;
        let effects = Effects::new(
            &summaries
                .iter()
                .enumerate()
                .map(|(function, summary)| {
                    let summary = summary.as_ref()?;
                    let unit = program.function(FunctionId(function as u32))?.unit;
                    let names = summary
                        .reads
                        .keys()
                        .chain(&summary.written)
                        .map(|&binding| ProgramBinding { unit, binding })
                        .filter(|binding| mutable_statics.contains(binding))
                        .collect();
                    Some(FunctionEffects {
                        names,
                        opaque: summary.opaque,
                        callees: summary.calls.iter().map(|site| site.callee).collect(),
                    })
                })
                .collect::<Vec<_>>(),
        );
        let mut reaching: HashMap<ProgramBinding, HashSet<FunctionId>> = HashMap::new();
        for (function, summary) in summaries.iter().enumerate() {
            let Some(summary) = summary else { continue };
            let Some(entry) = program.function(FunctionId(function as u32)) else {
                continue;
            };
            for site in &summary.calls {
                for shape in &site.shapes {
                    if let Shape::Array {
                        object: Some(binding),
                        ..
                    } = shape
                    {
                        let object = ProgramBinding {
                            unit: entry.unit,
                            binding: *binding,
                        };
                        reaching
                            .entry(object)
                            .or_insert_with(|| effects.reaching(object));
                    }
                }
            }
        }
        Self {
            effects,
            reaching,
            leaked,
        }
    }

    fn untouched(&self, callee: FunctionId, object: ProgramBinding) -> bool {
        !self.leaked.contains(&object)
            && !self.effects.opaque(callee)
            && self
                .reaching
                .get(&object)
                .is_some_and(|reached| !reached.contains(&callee))
    }
}

fn proves_pair(
    program: &Program<'_>,
    index: &Summaries,
    effects: &StaticArrays<'_>,
    active: &Pairs,
    bounded: &mut HashMap<Pair, bool>,
    counted_loops: &[HashMap<BindingId, CountedLoop>],
    (function, pointer, length): Pair,
) -> bool {
    let Summaries {
        functions: summaries,
        incoming,
        ..
    } = index;
    let Some(callee) = &summaries[function] else {
        return false;
    };
    let Some(element) = &callee.pointees[pointer] else {
        return false;
    };
    incoming[function].iter().all(|&(caller, index)| {
        let Some(summary) = &summaries[caller] else {
            return false;
        };
        let site = &summary.calls[index];
        if site.pinned {
            return false;
        }
        let (Some(callee_entry), Some(caller_entry)) = (
            program.function(FunctionId(function as u32)),
            program.function(FunctionId(caller as u32)),
        ) else {
            return false;
        };
        let same = |source: &Type| {
            program.same_type((caller_entry.unit, source), (callee_entry.unit, element))
        };
        match (site.shapes.get(pointer), site.shapes.get(length)) {
            (
                Some(Shape::Array {
                    length: extent,
                    offset,
                    element: source,
                    object,
                }),
                Some(Shape::Constant(count)),
            ) => {
                count <= &(extent - offset)
                    && same(source)
                    && ((*offset == 0 && extent == count)
                        || *bounded
                            .entry((function, pointer, length))
                            .or_insert_with(|| {
                                accesses_within_count(
                                    &callee_entry.function.value,
                                    callee.parameters[pointer],
                                    callee.parameters[length],
                                    &counted_loops[callee_entry.unit.0 as usize],
                                )
                            }))
                    && object.is_none_or(|binding| {
                        effects.untouched(
                            FunctionId(function as u32),
                            ProgramBinding {
                                unit: caller_entry.unit,
                                binding,
                            },
                        )
                    })
            }
            (
                Some(Shape::Allocation {
                    pointer: binding,
                    count,
                    element: source,
                }),
                Some(Shape::Binding(passed)),
            ) => {
                count == passed
                    && same(source)
                    && !summary.written.contains(binding)
                    && !summary.written.contains(count)
            }
            (Some(Shape::Binding(source)), Some(Shape::Binding(passed))) => {
                match (
                    summary.unwritten_parameter(*source),
                    summary.unwritten_parameter(*passed),
                ) {
                    (Some(source), Some(passed)) => active.contains(&(caller, source, passed)),
                    _ => false,
                }
            }
            _ => false,
        }
    })
}

fn accesses_within_count(
    function: &Function,
    pointer: BindingId,
    count: BindingId,
    loops: &HashMap<BindingId, CountedLoop>,
) -> bool {
    let Some(body) = &function.body else {
        return false;
    };
    let def_use = DefUse::of(body);
    if !def_use.whole_body_structured() || def_use.escapes(count) {
        return false;
    }
    let mut uses = PrefixUses {
        pointer,
        count,
        loops,
        bounded: HashMap::new(),
        reads: HashMap::new(),
    };
    ir::walk_function_body(&mut body.clone(), &mut uses);
    !uses.reads.is_empty() && uses.reads == uses.bounded
}

struct PrefixUses<'a> {
    pointer: BindingId,
    count: BindingId,
    loops: &'a HashMap<BindingId, CountedLoop>,
    bounded: HashMap<NodeId, usize>,
    reads: HashMap<NodeId, usize>,
}

impl Visitor for PrefixUses<'_> {
    fn statement(&mut self, statement: &Statement) {
        if let Statement::For { id, body, .. } = statement
            && let Some(loop_) = self.loops.get(id)
            && !loop_.inclusive
            && read_binding(&loop_.end) == Some(self.count)
            && counted_loops::integer_constant(&loop_.start)
                .is_some_and(|start| start.sign() != num_bigint::Sign::Minus)
        {
            let mut accesses = BoundedReads {
                index: loop_.index,
                pointer: self.pointer,
                bounded: &mut self.bounded,
            };
            ir::walk_function_body(&mut body.clone(), &mut accesses);
        }
    }

    fn rewrite_value(&mut self, value: &mut Value) {
        if read_binding(value) == Some(self.pointer) {
            *self.reads.entry(value.node.id).or_default() += 1;
        }
    }
}

struct BoundedReads<'a> {
    index: BindingId,
    pointer: BindingId,
    bounded: &'a mut HashMap<NodeId, usize>,
}

impl Visitor for BoundedReads<'_> {
    fn value_kind(&mut self, kind: &ValueKind) {
        if let ValueKind::Read {
            place:
                Place {
                    kind: PlaceKind::Deref(address),
                    ..
                },
            ..
        } = kind
            && let ValueKind::PointerOffset {
                pointer,
                amount,
                subtract: false,
                ..
            } = &address.node.value
            && read_binding(pointer) == Some(self.pointer)
            && read_binding(amount) == Some(self.index)
        {
            *self.bounded.entry(pointer.node.id).or_default() += 1;
        }
    }
}

fn retain_unique_pairs(next: &mut Pairs) {
    let mut uses: HashMap<(usize, bool, usize), usize> = HashMap::new();
    for &(function, pointer, length) in next.iter() {
        *uses.entry((function, false, pointer)).or_default() += 1;
        *uses.entry((function, true, length)).or_default() += 1;
    }
    next.retain(|&(function, pointer, length)| {
        uses[&(function, false, pointer)] == 1 && uses[&(function, true, length)] == 1
    });
}

fn can_borrow(
    index: &Summaries,
    flow: &Flow,
    unproven: &[bool],
    function: usize,
    pointer: usize,
) -> bool {
    let node = index.parameters[function][&pointer];
    let class = flow.class_of(node) as usize;
    let item = index.functions[function]
        .as_ref()
        .map(|summary| &summary.nodes[(node - index.base[function]) as usize]);
    !unproven[class]
        && item.is_some_and(|item| {
            !item.opaque_pointee
                && flow
                    .borrow(node, item.byte_elements)
                    .is_some_and(|borrow| !borrow.nullable)
        })
}
