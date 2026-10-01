use super::*;

pub(super) fn lower_string_global(global: &ir::Global) -> Option<Result<Vec<u8>>> {
    if !global.variable.name.starts_with('.') {
        return None;
    }
    let ir::ValueKind::CodeUnits(units) = &global.variable.initializer.as_ref()?.node.value else {
        return None;
    };
    Some(
        units
            .iter()
            .map(|unit| {
                u8::try_from(*unit).map_err(|_| super::Error::Unsupported("wide string".into()))
            })
            .collect(),
    )
}

pub(super) fn over_alignment(cx: &Context, variable: &ir::Variable) -> Option<u64> {
    let alignment = variable.alignment?;
    let abi_alignment = matches!(variable.ty, ir::Type::Array { .. })
        && variable.alignment == cx.target.large_array_alignment();
    let (_, natural) = storage_of(cx, &variable.ty)?;
    (!abi_alignment && alignment > natural).then_some(alignment)
}

pub(super) fn align_wrapper(alignment: u64) -> String {
    format!("__SlateAlign{alignment}")
}

pub(super) fn lower_static(
    global: &ir::Global,
    cx: &Context,
) -> Result<(rust::Type, Option<Expr>)> {
    let variable = &global.variable;
    let abi_alignment = matches!(variable.ty, ir::Type::Array { .. })
        && variable.alignment == cx.target.large_array_alignment();
    if !matches!(variable.storage, ir::StorageDuration::Static)
        || (variable.alignment.is_some()
            && !abi_alignment
            && storage_of(cx, &variable.ty).is_none())
        || !variable.access.is_plain()
        || global.symbol != ir::SymbolAttributes::default()
    {
        return Err(super::Error::Unsupported(format!(
            "global {} attributes",
            variable.name
        )));
    }
    let ty = lower_type(cx, &variable.ty)?;
    if !global.definition {
        return Ok((ty, None));
    }
    let init = match &variable.initializer {
        None => zeroed(),
        Some(value) if is_constant_initializer(value, cx) => lower_value(value, cx)?,
        Some(_) => {
            return Err(super::Error::Unsupported(format!(
                "initialized global {}",
                variable.name
            )));
        }
    };
    Ok(match cx.over_aligned.get(&variable.id) {
        Some(&alignment) => (
            rust::Type::Generic {
                name: align_wrapper(alignment),
                args: vec![ty],
            },
            Some(Expr::Call {
                func: Box::new(Expr::Var(align_wrapper(alignment).into())),
                args: vec![init],
                binding: CallBinding::Generated,
            }),
        ),
        None => (ty, Some(init)),
    })
}

pub(super) fn is_constant_initializer(value: &ir::Value, cx: &Context) -> bool {
    match &value.node.value {
        ValueKind::Constant(_) | ValueKind::CodeUnits(_) | ValueKind::Null => true,
        ValueKind::ArrayDecay { place, .. } | ValueKind::AddressOf(place) => {
            is_constant_address(place, cx)
        }
        ValueKind::PointerOffset {
            pointer, amount, ..
        } => {
            is_constant_initializer(pointer, cx)
                && matches!(amount.node.value, ValueKind::Constant(_))
        }
        ValueKind::Convert { operand, .. } => {
            !is_long_double(cx, &operand.ty)
                && !is_long_double(cx, &value.ty)
                && is_constant_initializer(operand, cx)
        }
        ValueKind::Aggregate { members, .. } => members
            .iter()
            .all(|member| is_constant_initializer(&member.value, cx)),
        _ => false,
    }
}

pub(super) fn is_constant_address(place: &ir::Place, cx: &Context) -> bool {
    match &place.kind {
        PlaceKind::Binding(id) => cx.statics.contains(id) || cx.strings.contains_key(id),
        PlaceKind::Field { base, .. } => is_constant_address(base, cx),
        PlaceKind::Deref(pointer) => is_constant_initializer(pointer, cx),
        PlaceKind::Index { base, index } => {
            is_constant_initializer(base, cx) && matches!(index.node.value, ValueKind::Constant(_))
        }
        _ => false,
    }
}
