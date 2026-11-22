use crate::counted_loops::{for_each_let, plain_local};
use crate::def_use::DefUse;
use foldhash::{HashMap, HashMapExt};
use slate_parser::ir::*;

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

pub(crate) fn analyze(
    function: &Function,
    literals: &HashMap<BindingId, Vec<u8>>,
    out: &mut HashMap<BindingId, Vec<u8>>,
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
    for (&binding, initializer) in &initializers {
        if let Some(bytes) = resolve(initializer, literals, &initializers, 0) {
            out.insert(binding, bytes);
        }
    }
}

fn resolve(
    value: &Value,
    literals: &HashMap<BindingId, Vec<u8>>,
    initializers: &HashMap<BindingId, &Value>,
    depth: usize,
) -> Option<Vec<u8>> {
    if depth > 8 {
        return None;
    }
    match &value.node.value {
        ValueKind::Copy { operand, .. }
        | ValueKind::Convert {
            kind: ConversionKind::PointerCast,
            operand,
            ..
        } => resolve(operand, literals, initializers, depth),
        ValueKind::ArrayDecay {
            place:
                Place {
                    kind: PlaceKind::Binding(id),
                    ..
                },
            ..
        } => literals.get(id).cloned(),
        ValueKind::Read {
            place:
                Place {
                    kind: PlaceKind::Binding(id),
                    ..
                },
            ordering: None,
        } => resolve(initializers.get(id)?, literals, initializers, depth + 1),
        _ => None,
    }
}
