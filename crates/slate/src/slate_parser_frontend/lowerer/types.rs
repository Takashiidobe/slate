use super::*;

pub(super) fn resolve_type<'a>(cx: &Context<'a>, mut ty: &'a ir::Type) -> &'a ir::Type {
    while let ir::Type::Defined(id) = ty
        && let Some(
            ir::TypeDefinitionKind::Alias(inner)
            | ir::TypeDefinitionKind::Enum {
                underlying: Some(inner),
                ..
            },
        ) = cx.types.get(id).map(|d| &d.kind)
    {
        ty = inner;
    }
    ty
}

pub(super) fn record_fields<'a>(
    cx: &Context<'a>,
    ty: &'a ir::Type,
) -> Option<&'a [slate_parser::ast::Span<ir::Field>]> {
    let ir::Type::Defined(id) = resolve_type(cx, ty) else {
        return None;
    };
    match &cx.types.get(id)?.kind {
        ir::TypeDefinitionKind::Record {
            kind: ir::RecordKind::Struct,
            fields: Some(fields),
            ..
        } => Some(fields),
        _ => None,
    }
}

pub(super) fn storage_of(cx: &Context, ty: &ir::Type) -> Option<(u64, u64)> {
    match resolve_type(cx, ty) {
        ir::Type::Defined(id) => match &cx.types.get(id)?.kind {
            ir::TypeDefinitionKind::Record {
                layout: Some(layout),
                ..
            } => Some((layout.size, layout.align)),
            _ => None,
        },
        ir::Type::Array {
            element,
            length: Some(length),
        } => storage_of(cx, element).map(|(size, align)| (size * length, align)),
        ty => cx
            .target
            .storage_of(ty.clone())
            .ok()
            .map(|layout| (layout.size_bytes, layout.alignment_bytes.into())),
    }
}

pub(super) fn lower_record(cx: &Context, id: TypeId) -> Result<String> {
    let name = cx.record_names[&id].clone();
    match cx.records.borrow().get(&id.0) {
        Some(Some(Err(error))) => return Err(super::Error::Unsupported(error.clone())),
        Some(_) => return Ok(name),
        None => {}
    }
    cx.records.borrow_mut().insert(id.0, None);
    let record = build_record(cx, &cx.types[&id].kind, &name).map_err(|error| match error {
        super::Error::Unsupported(message) => message,
        error => error.to_string(),
    });
    cx.records.borrow_mut().insert(id.0, Some(record.clone()));
    record.map(|_| name).map_err(super::Error::Unsupported)
}

pub(super) fn build_record(
    cx: &Context,
    kind: &ir::TypeDefinitionKind,
    name: &str,
) -> Result<rust::RecordDef> {
    let ir::TypeDefinitionKind::Record {
        kind: ir::RecordKind::Struct,
        fields,
        layout,
    } = kind
    else {
        return Err(super::Error::Unsupported(format!("record {name}")));
    };
    let mut lowered = Vec::new();
    let mut end = 0u64;
    let mut align = 1u64;
    for (index, field) in fields.iter().flatten().enumerate() {
        let (Some(field_name), None, true) =
            (&field.name, field.bit_width, field.access.is_plain())
        else {
            return Err(super::Error::Unsupported(format!(
                "field {index} of record {name}"
            )));
        };
        let (field_size, field_align) = storage_of(cx, &field.ty)
            .ok_or_else(|| super::Error::Unsupported(format!("layout of {}", field.ty)))?;
        let offset = end.next_multiple_of(field_align);
        if layout.as_ref().map(|layout| layout.offsets[index]) != Some(offset) {
            return Err(super::Error::Unsupported(format!(
                "layout of record {name}"
            )));
        }
        end = offset + field_size;
        align = align.max(field_align);
        lowered.push(rust::RecordField {
            comments: Vec::new(),
            name: field_name.as_str().into(),
            ty: lower_type(cx, &field.ty)?,
        });
    }
    if let Some(layout) = layout
        && (layout.align != align || layout.size != end.next_multiple_of(align))
    {
        return Err(super::Error::Unsupported(format!(
            "layout of record {name}"
        )));
    }
    Ok(rust::RecordDef {
        comments: Vec::new(),
        vis: rust::Visibility::Private,
        field_vis: rust::Visibility::Private,
        is_union: false,
        allow_non_camel_case: !is_camel_case(name),
        name: name.to_owned(),
        fields: lowered,
        packed: None,
        align: None,
    })
}

pub(super) fn lower_type(cx: &Context, ty: &ir::Type) -> Result<rust::Type> {
    let primitive = match ty {
        ir::Type::Void => return Ok(rust::Type::Unit),
        ir::Type::Defined(id) => {
            return match cx.types.get(id).map(|definition| &definition.kind) {
                Some(
                    ir::TypeDefinitionKind::Alias(inner)
                    | ir::TypeDefinitionKind::Enum {
                        underlying: Some(inner),
                        ..
                    },
                ) => lower_type(cx, inner),
                Some(ir::TypeDefinitionKind::Record { .. }) => {
                    Ok(rust::Type::Custom(lower_record(cx, *id)?))
                }
                _ => Err(super::Error::Unsupported(format!("type {ty}"))),
            };
        }
        ir::Type::Bool => Prim::Bool,
        ir::Type::Numeric(ir::NumericType::Integer { width, signed, .. }) => {
            match (*width, *signed) {
                (8, true) => Prim::I8,
                (16, true) => Prim::I16,
                (32, true) => Prim::I32,
                (64, true) => Prim::I64,
                (128, true) => Prim::I128,
                (8, false) => Prim::U8,
                (16, false) => Prim::U16,
                (32, false) => Prim::U32,
                (64, false) => Prim::U64,
                (128, false) => Prim::U128,
                _ => return Err(super::Error::Unsupported(format!("integer type {ty}"))),
            }
        }
        ir::Type::Numeric(ir::NumericType::Float(ir::FloatType::F32)) => Prim::F32,
        ir::Type::Numeric(ir::NumericType::Float(ir::FloatType::F64)) => Prim::F64,
        ir::Type::Numeric(ir::NumericType::Float(ir::FloatType::F80)) => {
            cx.long_double.set(true);
            return Ok(rust::Type::LongDouble);
        }
        ir::Type::Pointer { pointee, .. } if matches!(**pointee, ir::Type::Function { .. }) => {
            let ir::Type::Function {
                return_type,
                parameters,
                variadic: false,
                prototyped: true,
                convention: ir::CallConv::C,
            } = &**pointee
            else {
                return Err(super::Error::Unsupported(format!("type {ty}")));
            };
            return Ok(rust::Type::FnPtr {
                abi: rust::Abi::Rust,
                params: parameters
                    .iter()
                    .map(|ty| lower_type(cx, ty))
                    .collect::<Result<_>>()?,
                ret: Box::new(match return_type {
                    Some(ret) => lower_type(cx, ret)?,
                    None => rust::Type::Unit,
                }),
            });
        }
        ir::Type::Pointer {
            pointee, is_const, ..
        } => {
            return Ok(rust::Type::Ptr {
                mutable: !is_const,
                inner: Box::new(lower_type(cx, pointee)?),
            });
        }
        ir::Type::Array {
            element,
            length: Some(length),
        } => {
            return Ok(rust::Type::Array {
                elem: Box::new(lower_type(cx, element)?),
                len: *length,
            });
        }
        ir::Type::VaList => return Ok(rust::Type::VaList),
        _ => return Err(super::Error::Unsupported(format!("type {ty}"))),
    };
    Ok(rust::Type::Prim(primitive))
}
