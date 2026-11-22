use crate::counted_loops::{for_each_let, plain_local};
use crate::def_use::DefUse;
use crate::identity::Known;
use foldhash::{HashMap, HashMapExt, HashSet};
use slate_parser::ast::MacroExpansionId;
use slate_parser::ir::*;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct ConstantString {
    pub bytes: Vec<u8>,
    pub readers: Vec<Known>,
    pub only_read_as_string: bool,
}

impl Known {
    pub fn string_arguments(self) -> Option<(&'static [usize], usize)> {
        Some(match self {
            Known::StrLen | Known::Atoi | Known::Atol | Known::Atoll | Known::Atof => (&[0], 1),
            Known::StrNLen => (&[0], 2),
            Known::StrCmp | Known::StrSpn | Known::StrCSpn => (&[0, 1], 2),
            Known::StrNCmp => (&[0, 1], 3),
            #[expect(
                slate::sensitive_wildcard,
                reason = "only these functions read their arguments as strings and nothing else"
            )]
            _ => return None,
        })
    }
}

pub(crate) fn literals(module: &Module) -> HashMap<BindingId, Vec<u8>> {
    module
        .globals
        .iter()
        .filter_map(|global| {
            let variable = &global.value.variable;
            let ValueKind::CodeUnits(units) = &variable.initializer.as_ref()?.node.value else {
                return None;
            };
            if !variable.name.starts_with('.')
                || !matches!(
                    &variable.ty,
                    Type::Array { element, .. }
                        if matches!(**element, Type::Numeric(NumericType::Integer { width: 8, .. }))
                )
            {
                return None;
            }
            let (0, content) = units.split_last()? else {
                return None;
            };
            let bytes = content
                .iter()
                .map(|unit| u8::try_from(*unit).ok())
                .collect::<Option<Vec<u8>>>()?;
            Some((variable.id, bytes))
        })
        .collect()
}

pub(crate) fn string_callees(
    module: &Module,
    identity: impl Fn(BindingId) -> Option<Known>,
) -> HashMap<BindingId, Known> {
    module
        .functions
        .iter()
        .filter_map(|function| {
            let known = identity(function.value.id)?;
            (function.name.trim_start_matches("__builtin_") == known.symbol()
                && known.string_arguments().is_some())
            .then_some((function.value.id, known))
        })
        .collect()
}

pub(crate) struct Context<'a> {
    pub literals: &'a HashMap<BindingId, Vec<u8>>,
    pub callees: &'a HashMap<BindingId, Known>,
    pub c_strings: &'a HashSet<BindingId>,
    pub macro_values: &'a MacroValues,
}

pub(crate) fn analyze(
    function: &Function,
    context: &Context,
    out: &mut HashMap<BindingId, ConstantString>,
) {
    let Some(body) = &function.body else {
        return;
    };
    let def_use = DefUse::of(body);
    let mut initializers = HashMap::new();
    for_each_let(body, &mut |variable| {
        if plain_local(variable)
            && matches!(variable.ty, Type::Pointer { .. })
            && let Some(initializer) = &variable.initializer
            && def_use.written_once(variable.id)
        {
            initializers.insert(variable.id, initializer);
        }
    });
    let candidates: HashMap<BindingId, Vec<u8>> = initializers
        .keys()
        .filter_map(|&binding| {
            let bytes = resolve(initializers[&binding], context, &initializers, 0)?;
            Some((binding, bytes))
        })
        .collect();
    if candidates.is_empty() {
        return;
    }
    let mut uses = Uses {
        context,
        candidates: &candidates,
        depth: 0,
        reads: HashMap::new(),
        calls: Vec::new(),
    };
    walk_function_body_with_macros(&mut body.clone(), context.macro_values, &mut uses);
    let Uses { reads, calls, .. } = uses;
    let mut viewable: HashSet<BindingId> = candidates.keys().copied().collect();
    loop {
        let lifted = lifted_reads(&calls, &viewable);
        let only_lifted = |binding: &BindingId| {
            lifted
                .get(binding)
                .is_some_and(|(count, _)| Some(count) == reads.get(binding))
        };
        let before = viewable.len();
        viewable.retain(|binding| !candidates[binding].contains(&0) || only_lifted(binding));
        if viewable.len() == before {
            for binding in viewable {
                let Some((_, readers)) = lifted.get(&binding) else {
                    continue;
                };
                out.insert(
                    binding,
                    ConstantString {
                        bytes: candidates[&binding].clone(),
                        readers: readers.clone(),
                        only_read_as_string: only_lifted(&binding),
                    },
                );
            }
            return;
        }
    }
}

fn lifted_reads(
    calls: &[(Known, Vec<Argument>)],
    viewable: &HashSet<BindingId>,
) -> HashMap<BindingId, (usize, Vec<Known>)> {
    let mut lifted: HashMap<BindingId, (usize, Vec<Known>)> = HashMap::new();
    for (known, arguments) in calls {
        let liftable = arguments.iter().all(|argument| match argument {
            Argument::Local(binding) => viewable.contains(binding),
            Argument::Viewed => true,
            Argument::Other => false,
        });
        if !liftable {
            continue;
        }
        for argument in arguments {
            if let Argument::Local(binding) = argument {
                let (count, readers) = lifted.entry(*binding).or_default();
                *count += 1;
                if !readers.contains(known) {
                    readers.push(*known);
                }
            }
        }
    }
    lifted
}

enum Argument {
    Local(BindingId),
    Viewed,
    Other,
}

struct Uses<'a> {
    context: &'a Context<'a>,
    candidates: &'a HashMap<BindingId, Vec<u8>>,
    depth: u32,
    reads: HashMap<BindingId, usize>,
    calls: Vec<(Known, Vec<Argument>)>,
}

impl Uses<'_> {
    fn argument(&self, value: &Value) -> Argument {
        if self.context.macro_values.contains_key(&value.node.id) {
            return Argument::Other;
        }
        match &value.node.value {
            ValueKind::Copy { operand, .. }
            | ValueKind::Convert {
                kind: ConversionKind::PointerCast,
                operand,
                ..
            } => self.argument(operand),
            ValueKind::ArrayDecay {
                place:
                    Place {
                        kind: PlaceKind::Binding(id),
                        ..
                    },
                ..
            } if self.context.literals.contains_key(id) => Argument::Viewed,
            ValueKind::Read {
                place:
                    Place {
                        kind: PlaceKind::Binding(id),
                        ..
                    },
                ordering: None,
            } if self.candidates.contains_key(id) => Argument::Local(*id),
            ValueKind::Read {
                place:
                    Place {
                        kind: PlaceKind::Binding(id),
                        ..
                    },
                ordering: None,
            } if self.context.c_strings.contains(id) => Argument::Viewed,
            #[expect(
                slate::sensitive_wildcard,
                reason = "only literals, constant-string locals and C-string parameters have a string view"
            )]
            _ => Argument::Other,
        }
    }
}

impl Visitor for Uses<'_> {
    fn enter_expansion(&mut self, _expansion: MacroExpansionId) {
        self.depth += 1;
    }

    fn leave_expansion(&mut self, _expansion: MacroExpansionId) {
        self.depth -= 1;
    }

    fn value_kind(&mut self, kind: &ValueKind) {
        match kind {
            ValueKind::Read {
                place:
                    Place {
                        kind: PlaceKind::Binding(id),
                        ..
                    },
                ..
            } if self.candidates.contains_key(id) => *self.reads.entry(*id).or_default() += 1,
            ValueKind::Call {
                callee: Callee::Direct(id),
                arguments,
                ..
            } if self.depth == 0
                && let Some(&known) = self.context.callees.get(id)
                && let Some((strings, arity)) = known.string_arguments()
                && arguments.len() == arity =>
            {
                let arguments = strings
                    .iter()
                    .map(|&index| self.argument(&arguments[index]))
                    .collect();
                self.calls.push((known, arguments));
            }
            #[expect(
                slate::sensitive_wildcard,
                reason = "only reads of candidates and string-reading calls matter"
            )]
            _ => {}
        }
    }
}

fn resolve(
    value: &Value,
    context: &Context,
    initializers: &HashMap<BindingId, &Value>,
    depth: usize,
) -> Option<Vec<u8>> {
    if depth > 8 || context.macro_values.contains_key(&value.node.id) {
        return None;
    }
    match &value.node.value {
        ValueKind::Copy { operand, .. }
        | ValueKind::Convert {
            kind: ConversionKind::PointerCast,
            operand,
            ..
        } => resolve(operand, context, initializers, depth),
        ValueKind::ArrayDecay {
            place:
                Place {
                    kind: PlaceKind::Binding(id),
                    ..
                },
            ..
        } => context.literals.get(id).cloned(),
        ValueKind::Read {
            place:
                Place {
                    kind: PlaceKind::Binding(id),
                    ..
                },
            ordering: None,
        } => resolve(initializers.get(id)?, context, initializers, depth + 1),
        _ => None,
    }
}
