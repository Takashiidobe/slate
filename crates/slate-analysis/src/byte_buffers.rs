use crate::counted_loops::{for_each_let, integer_constant, plain_local};
use crate::def_use::DefUse;
use crate::identity::Known;
use foldhash::{HashMap, HashMapExt};
use slate_parser::ast::MacroExpansionId;
use slate_parser::ir::*;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct ArrayAlias {
    pub array: BindingId,
    pub offset: u64,
}

impl Known {
    pub fn buffer_arguments(self) -> Option<(&'static [usize], usize)> {
        Some(match self {
            Known::MemCpy | Known::MemMove | Known::MemCmp => (&[0, 1], 3),
            Known::MemSet => (&[0], 3),
            #[expect(
                slate::sensitive_wildcard,
                reason = "only these functions take byte buffers and a length"
            )]
            _ => return None,
        })
    }
}

fn byte_array(ty: &Type) -> Option<u64> {
    match ty {
        Type::Array {
            element,
            length: Some(length),
        } if matches!(
            **element,
            Type::Numeric(NumericType::Integer { width: 8, .. })
        ) =>
        {
            Some(*length)
        }
        #[expect(
            slate::sensitive_wildcard,
            reason = "only fixed-length byte arrays are buffers with a known extent"
        )]
        _ => None,
    }
}

pub(crate) fn analyze(
    function: &Function,
    callees: &HashMap<BindingId, Known>,
    macro_values: &MacroValues,
    out: &mut HashMap<BindingId, ArrayAlias>,
) {
    let Some(body) = &function.body else {
        return;
    };
    let def_use = DefUse::of(body);
    let mut names: HashMap<&str, usize> = HashMap::new();
    let mut arrays = HashMap::new();
    let mut initializers = Vec::new();
    for_each_let(body, &mut |variable| {
        *names.entry(variable.name.as_str()).or_default() += 1;
        if !plain_local(variable) {
            return;
        }
        if let Some(length) = byte_array(&variable.ty) {
            arrays.insert(variable.id, (variable.name.as_str(), length));
        } else if matches!(variable.ty, Type::Pointer { .. })
            && let Some(initializer) = &variable.initializer
            && def_use.written_once(variable.id)
            && !def_use.escapes(variable.id)
            && !macro_values.contains_key(&initializer.node.id)
        {
            initializers.push((variable.id, initializer));
        }
    });
    arrays.retain(|_, (name, _)| names.get(name) == Some(&1));
    let candidates: HashMap<BindingId, ArrayAlias> = initializers
        .into_iter()
        .filter_map(|(binding, initializer)| {
            Some((binding, resolve(initializer, &arrays, macro_values)?))
        })
        .collect();
    if candidates.is_empty() {
        return;
    }
    let mut uses = Uses {
        callees,
        candidates: &candidates,
        depth: 0,
        reads: HashMap::new(),
        buffer_reads: HashMap::new(),
    };
    walk_function_body_with_macros(&mut body.clone(), macro_values, &mut uses);
    let Uses {
        reads,
        buffer_reads,
        ..
    } = uses;
    for (binding, alias) in candidates {
        if reads
            .get(&binding)
            .is_some_and(|reads| buffer_reads.get(&binding) == Some(reads))
        {
            out.insert(binding, alias);
        }
    }
}

fn resolve(
    value: &Value,
    arrays: &HashMap<BindingId, (&str, u64)>,
    macro_values: &MacroValues,
) -> Option<ArrayAlias> {
    if macro_values.contains_key(&value.node.id) {
        return None;
    }
    match &value.node.value {
        ValueKind::Copy { operand, .. }
        | ValueKind::Convert {
            kind: ConversionKind::PointerCast,
            operand,
            ..
        } => resolve(operand, arrays, macro_values),
        ValueKind::ArrayDecay {
            place:
                Place {
                    kind: PlaceKind::Binding(id),
                    ..
                },
            ..
        } if arrays.contains_key(id) => Some(ArrayAlias {
            array: *id,
            offset: 0,
        }),
        ValueKind::PointerOffset {
            pointer,
            amount,
            subtract: false,
            element: Type::Numeric(NumericType::Integer { width: 8, .. }),
            ..
        } => {
            let alias = resolve(pointer, arrays, macro_values)?;
            let offset = alias
                .offset
                .checked_add(u64::try_from(integer_constant(amount)?).ok()?)?;
            let (_, length) = arrays.get(&alias.array)?;
            (offset <= *length).then_some(ArrayAlias { offset, ..alias })
        }
        #[expect(
            slate::sensitive_wildcard,
            reason = "only a decayed local byte array, its casts and constant offsets alias it"
        )]
        _ => None,
    }
}

struct Uses<'a> {
    callees: &'a HashMap<BindingId, Known>,
    candidates: &'a HashMap<BindingId, ArrayAlias>,
    depth: u32,
    reads: HashMap<BindingId, usize>,
    buffer_reads: HashMap<BindingId, usize>,
}

impl Uses<'_> {
    fn buffer_argument(&self, value: &Value) -> Option<BindingId> {
        match &value.node.value {
            ValueKind::Copy { operand, .. }
            | ValueKind::Convert {
                kind: ConversionKind::PointerCast,
                operand,
                ..
            } => self.buffer_argument(operand),
            ValueKind::PointerOffset {
                pointer,
                amount,
                subtract: false,
                ..
            } if integer_constant(amount).is_some() => self.buffer_argument(pointer),
            ValueKind::Read {
                place:
                    Place {
                        kind: PlaceKind::Binding(id),
                        ..
                    },
                ordering: None,
            } if self.candidates.contains_key(id) => Some(*id),
            #[expect(
                slate::sensitive_wildcard,
                reason = "only a candidate read, its casts and constant offsets name its buffer"
            )]
            _ => None,
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
                && let Some(&known) = self.callees.get(id)
                && let Some((buffers, arity)) = known.buffer_arguments()
                && arguments.len() == arity =>
            {
                for &index in buffers {
                    if let Some(binding) = self.buffer_argument(&arguments[index]) {
                        *self.buffer_reads.entry(binding).or_default() += 1;
                    }
                }
            }
            #[expect(
                slate::sensitive_wildcard,
                reason = "only reads of candidates and buffer calls matter"
            )]
            _ => {}
        }
    }
}
