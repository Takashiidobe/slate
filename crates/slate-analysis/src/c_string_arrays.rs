use crate::byte_buffers::byte_array;
use crate::counted_loops::{for_each_let, integer_constant, plain_local};
use crate::identity::Known;
use foldhash::{HashMap, HashMapExt, HashSet};
use slate_parser::ast::MacroExpansionId;
use slate_parser::ir::*;

fn modeled_argument(known: Known, index: usize) -> bool {
    if let Some((strings, _)) = known.string_arguments()
        && strings.contains(&index)
    {
        return true;
    }
    matches!(
        (known, index),
        (Known::StrCpy | Known::StrCat, 0 | 1)
            | (Known::FGets | Known::SNPrintf, 0)
            | (Known::Printf | Known::Puts | Known::FPuts, _)
            | (Known::FPrintf, 1..)
            | (Known::SNPrintf, 2..)
    )
}

fn zero(value: &Value) -> bool {
    integer_constant(value).is_some_and(|value| value.bits() == 0)
}

fn terminated(initializer: Option<&Value>, length: u64) -> bool {
    let Some(initializer) = initializer else {
        return true;
    };
    match &initializer.node.value {
        ValueKind::CodeUnits(units) => (units.len() as u64) < length || units.contains(&0),
        ValueKind::Aggregate {
            zero_fill: true, ..
        } => true,
        #[expect(
            slate::sensitive_wildcard,
            reason = "only a string literal or a zero-filled initializer is known to hold a terminator"
        )]
        _ => false,
    }
}

pub(crate) fn analyze(
    function: &Function,
    callees: &HashMap<BindingId, Known>,
    macro_values: &MacroValues,
    out: &mut HashSet<BindingId>,
) {
    let Some(body) = &function.body else {
        return;
    };
    let mut names: HashMap<&str, usize> = HashMap::new();
    let mut candidates = Vec::new();
    for_each_let(body, &mut |variable| {
        *names.entry(variable.name.as_str()).or_default() += 1;
        if plain_local(variable)
            && let Some(length) = byte_array(&variable.ty)
            && terminated(variable.initializer.as_ref(), length)
        {
            candidates.push((variable.id, variable.name.as_str()));
        }
    });
    let candidates: HashSet<BindingId> = candidates
        .into_iter()
        .filter(|(_, name)| names.get(name) == Some(&1))
        .map(|(id, _)| id)
        .collect();
    if candidates.is_empty() {
        return;
    }
    let mut uses = Uses {
        callees,
        candidates: &candidates,
        depth: 0,
        places: HashMap::new(),
        modeled: HashMap::new(),
    };
    walk_function_body_with_macros(&mut body.clone(), macro_values, &mut uses);
    let Uses {
        places, modeled, ..
    } = uses;
    out.extend(
        candidates
            .into_iter()
            .filter(|binding| places.get(binding) == modeled.get(binding)),
    );
}

struct Uses<'a> {
    callees: &'a HashMap<BindingId, Known>,
    candidates: &'a HashSet<BindingId>,
    depth: u32,
    places: HashMap<BindingId, usize>,
    modeled: HashMap<BindingId, usize>,
}

impl Uses<'_> {
    fn decayed(&self, value: &Value) -> Option<BindingId> {
        match &value.node.value {
            ValueKind::Copy { operand, .. }
            | ValueKind::Convert {
                kind: ConversionKind::PointerCast,
                operand,
                ..
            } => self.decayed(operand),
            ValueKind::ArrayDecay {
                place:
                    Place {
                        kind: PlaceKind::Binding(id),
                        ..
                    },
                ..
            } if self.candidates.contains(id) => Some(*id),
            #[expect(
                slate::sensitive_wildcard,
                reason = "only a decayed candidate names its array"
            )]
            _ => None,
        }
    }

    fn element(&self, place: &Place) -> Option<BindingId> {
        match &place.kind {
            PlaceKind::Index { base, .. } => self.decayed(base),
            PlaceKind::Deref(pointer) => match &pointer.node.value {
                ValueKind::PointerOffset { pointer, .. } => self.decayed(pointer),
                #[expect(
                    slate::sensitive_wildcard,
                    reason = "a dereferenced decay or offset decay is an element"
                )]
                _ => self.decayed(pointer),
            },
            #[expect(
                slate::sensitive_wildcard,
                reason = "only an indexed element of a candidate is a modeled access"
            )]
            _ => None,
        }
    }

    fn model(&mut self, binding: BindingId) {
        *self.modeled.entry(binding).or_default() += 1;
    }
}

impl Visitor for Uses<'_> {
    fn enter_expansion(&mut self, _expansion: MacroExpansionId) {
        self.depth += 1;
    }

    fn leave_expansion(&mut self, _expansion: MacroExpansionId) {
        self.depth -= 1;
    }

    fn statement(&mut self, statement: &Statement) {
        if self.depth == 0
            && let Statement::Write { place, value, .. } = statement
            && zero(value)
            && let Some(binding) = self.element(place)
        {
            self.model(binding);
        }
    }

    fn place_kind(&mut self, kind: &PlaceKind) {
        if let PlaceKind::Binding(id) = kind
            && self.candidates.contains(id)
        {
            *self.places.entry(*id).or_default() += 1;
        }
    }

    fn value_kind(&mut self, kind: &ValueKind) {
        if self.depth > 0 {
            return;
        }
        match kind {
            ValueKind::Read { place, .. } => {
                if let Some(binding) = self.element(place) {
                    self.model(binding);
                }
            }
            ValueKind::Store { place, value, .. } if zero(value) => {
                if let Some(binding) = self.element(place) {
                    self.model(binding);
                }
            }
            ValueKind::Call {
                callee: Callee::Direct(id),
                arguments,
                ..
            } if let Some(&known) = self.callees.get(id) => {
                for (index, argument) in arguments.iter().enumerate() {
                    if modeled_argument(known, index)
                        && let Some(binding) = self.decayed(argument)
                    {
                        self.model(binding);
                    }
                }
            }
            #[expect(
                slate::sensitive_wildcard,
                reason = "only element reads, zero stores and modeled calls are modeled uses"
            )]
            _ => {}
        }
    }
}
