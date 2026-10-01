use super::*;

impl<'m> Tables<'m> {
    pub(super) fn resolve_type<'a>(&self, mut ty: &'a ir::Type) -> &'a ir::Type
    where
        'm: 'a,
    {
        while let ir::Type::Defined(id) = ty
            && let Some(
                ir::TypeDefinitionKind::Alias(inner)
                | ir::TypeDefinitionKind::Enum {
                    underlying: Some(inner),
                    ..
                },
            ) = self.types.get(id).map(|d| &d.kind)
        {
            ty = inner;
        }
        ty
    }

    pub(super) fn record_fields<'a>(
        &self,
        ty: &'a ir::Type,
    ) -> Option<&'a [slate_parser::ast::Span<ir::Field>]>
    where
        'm: 'a,
    {
        let ir::Type::Defined(id) = self.resolve_type(ty) else {
            return None;
        };
        match &self.types.get(id)?.kind {
            ir::TypeDefinitionKind::Record {
                kind: ir::RecordKind::Struct,
                fields: Some(fields),
                ..
            } => Some(fields),
            _ => None,
        }
    }

    pub(super) fn storage_of(&self, ty: &ir::Type) -> Option<(u64, u64)> {
        match self.resolve_type(ty) {
            ir::Type::Defined(id) => match &self.types.get(id)?.kind {
                ir::TypeDefinitionKind::Record {
                    layout: Some(layout),
                    ..
                } => Some((layout.size, layout.align)),
                _ => None,
            },
            ir::Type::Array {
                element,
                length: Some(length),
            } => self
                .storage_of(element)
                .map(|(size, align)| (size * length, align)),
            ty => self
                .target
                .storage_of(ty.clone())
                .ok()
                .map(|layout| (layout.size_bytes, layout.alignment_bytes.into())),
        }
    }
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn lower_record(&mut self, id: TypeId) -> Result<String> {
        let tables = self.tables;
        let name = tables.record_names[&id].clone();
        match self.needs.records.borrow().get(&id.0) {
            Some(Some(Err(error))) => return Err(super::Error::Unsupported(error.clone())),
            Some(_) => return Ok(name),
            None => {}
        }
        self.needs.records.borrow_mut().insert(id.0, None);
        let record =
            self.build_record(&tables.types[&id].kind, &name)
                .map_err(|error| match error {
                    super::Error::Unsupported(message) => message,
                    error => error.to_string(),
                });
        self.needs
            .records
            .borrow_mut()
            .insert(id.0, Some(record.clone()));
        record.map(|_| name).map_err(super::Error::Unsupported)
    }

    pub(super) fn build_record(
        &mut self,
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
            let (field_size, field_align) = self
                .tables
                .storage_of(&field.ty)
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
                ty: self.lower_type(&field.ty)?,
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

    pub(super) fn lower_type(&mut self, ty: &ir::Type) -> Result<rust::Type> {
        let primitive = match ty {
            ir::Type::Void => return Ok(rust::Type::Unit),
            ir::Type::Defined(id) => {
                return match self.tables.types.get(id).map(|definition| &definition.kind) {
                    Some(
                        ir::TypeDefinitionKind::Alias(inner)
                        | ir::TypeDefinitionKind::Enum {
                            underlying: Some(inner),
                            ..
                        },
                    ) => self.lower_type(inner),
                    Some(ir::TypeDefinitionKind::Record { .. }) => {
                        Ok(rust::Type::Custom(self.lower_record(*id)?))
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
                self.needs.long_double = true;
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
                        .map(|ty| self.lower_type(ty))
                        .collect::<Result<_>>()?,
                    ret: Box::new(match return_type {
                        Some(ret) => self.lower_type(ret)?,
                        None => rust::Type::Unit,
                    }),
                });
            }
            ir::Type::Pointer {
                pointee, is_const, ..
            } => {
                return Ok(rust::Type::Ptr {
                    mutable: !is_const,
                    inner: Box::new(self.lower_type(pointee)?),
                });
            }
            ir::Type::Array {
                element,
                length: Some(length),
            } => {
                return Ok(rust::Type::Array {
                    elem: Box::new(self.lower_type(element)?),
                    len: *length,
                });
            }
            ir::Type::VaList => return Ok(rust::Type::VaList),
            _ => return Err(super::Error::Unsupported(format!("type {ty}"))),
        };
        Ok(rust::Type::Prim(primitive))
    }
}
