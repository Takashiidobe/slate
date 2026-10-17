use super::*;
use slate_parser::target_info::{Endian, TargetEnvironment};

pub(super) const BIT_UNIT_MODULE: &str = "__slate_bits";

pub(super) fn fixed_parameters(
    parameters: &ir::Parameters,
) -> &[slate_parser::ast::Span<ir::Parameter>] {
    match parameters {
        ir::Parameters::Prototype { fixed, .. } => fixed,
        ir::Parameters::Unprototyped => &[],
    }
}

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

    pub(super) fn function_type_has_vector(&self, ty: &ir::Type) -> bool {
        match self.resolve_type(ty) {
            ir::Type::Pointer { pointee, .. } => self.function_type_has_vector(pointee),
            ir::Type::Function {
                parameters,
                return_type,
                ..
            } => parameters
                .iter()
                .chain(return_type.as_deref())
                .any(|ty| matches!(self.resolve_type(ty), ir::Type::Vector { .. })),
            _ => false,
        }
    }

    pub(super) fn function_has_vector(&self, function: &ir::Function) -> bool {
        fixed_parameters(&function.parameters)
            .iter()
            .map(|parameter| &parameter.ty)
            .chain(&function.return_type)
            .any(|ty| matches!(self.resolve_type(ty), ir::Type::Vector { .. }))
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
                fields: Some(fields),
                ..
            } => Some(fields),
            _ => None,
        }
    }

    pub(super) fn bit_unit_of(&self, ty: &ir::Type, index: usize) -> Option<usize> {
        let ir::Type::Defined(id) = self.resolve_type(ty) else {
            return None;
        };
        match &self.types.get(id)?.kind {
            ir::TypeDefinitionKind::Record {
                fields: Some(fields),
                layout: Some(layout),
                ..
            } if fields.get(index)?.bit_width.is_some() => layout.field_units[index],
            _ => None,
        }
    }

    pub(super) fn has_bit_fields(&self, ty: &ir::Type) -> bool {
        self.record_fields(ty)
            .is_some_and(|fields| fields.iter().any(|field| field.bit_width.is_some()))
    }

    pub(super) fn has_padding(&self, ty: &ir::Type) -> bool {
        let Some(fields) = self.record_fields(ty) else {
            return false;
        };
        let Some((size, _)) = self.storage_of(ty) else {
            return false;
        };
        let covered: Option<u64> = fields
            .iter()
            .map(|field| self.storage_of(&field.ty).map(|(size, _)| size))
            .sum();
        covered.is_none_or(|covered| covered != size)
    }

    pub(super) fn is_union(&self, ty: &ir::Type) -> bool {
        let ir::Type::Defined(id) = self.resolve_type(ty) else {
            return false;
        };
        matches!(
            self.types.get(id).map(|definition| &definition.kind),
            Some(ir::TypeDefinitionKind::Record {
                kind: ir::RecordKind::Union,
                ..
            })
        )
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
            ir::Type::Array { element, length } => self
                .storage_of(element)
                .map(|(size, align)| (size * length.unwrap_or(0), align)),
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
        match self.dependencies.records.get(&id.0) {
            Some(Record::Failed(error)) => return Err(error.clone()),
            Some(Record::Building | Record::Built(_)) => return Ok(name),
            None => {}
        }
        self.dependencies.records.insert(id.0, Record::Building);
        match self.build_record(id, &tables.types[&id].kind, &name) {
            Ok(record) => {
                self.dependencies
                    .records
                    .insert(id.0, Record::Built(record));
                Ok(name)
            }
            Err(error) => {
                let error = error
                    .at(Site::of(tables.types[&id]))
                    .within(Site::of(tables.types[&id]), format!("in record `{name}`"));
                self.dependencies
                    .records
                    .insert(id.0, Record::Failed(error.clone()));
                Err(error)
            }
        }
    }

    pub(super) fn build_record(
        &mut self,
        id: TypeId,
        kind: &ir::TypeDefinitionKind,
        name: &str,
    ) -> Result<rust::RecordDef> {
        let ir::TypeDefinitionKind::Record {
            kind: record_kind,
            fields,
            layout,
        } = kind
        else {
            return Err(unsupported_record(name, "kind"));
        };
        let is_union = matches!(record_kind, ir::RecordKind::Union);
        let record = |fields, packed: Option<u64>, align: Option<u64>| rust::RecordDef {
            comments: Vec::new(),
            trailing_comments: Vec::new(),
            vis: rust::Visibility::Private,
            field_vis: rust::Visibility::Private,
            is_union,
            name: name.to_owned(),
            fields,
            packed: packed.map(|packed| packed as u32),
            align: align.map(|align| align as u32),
        };
        let fields = fields.as_deref().unwrap_or_default();
        let Some(layout) = layout else {
            return match fields {
                [] => Ok(record(Vec::new(), None, None)),
                _ => Err(unsupported_record(name, "layout")),
            };
        };
        let pack = self.record_pack(fields, layout);
        let mut lowered = Vec::new();
        let owner = self.tables.types[&id].id;
        let mut pending = Vec::new();
        let mut end = 0u64;
        let mut align = 1u64;
        let mut units = BTreeSet::new();
        for (index, field) in fields.iter().enumerate() {
            pending.extend(self.claim_comments(field.id));
            if (field.access.atomic && !self.tables.atomic_scalar(&field.ty))
                || (field.access.volatile && field.bit_width.is_some())
            {
                return Err(unsupported_record(name, &format!("field {index}")).at(Site::of(field)));
            }
            let (field_name, ty, offset, size, field_align) = if field.bit_width.is_some() {
                let Some(unit) = layout.field_units[index] else {
                    continue;
                };
                if !units.insert(unit) {
                    continue;
                }
                let storage = &layout.bit_units[unit];
                let (ty, unit_align) = self.lower_bit_unit(id, name, fields, layout, unit)?;
                (
                    bit_unit_name(unit),
                    ty,
                    storage.offset,
                    storage.size,
                    unit_align,
                )
            } else {
                let (size, field_align) = self.tables.storage_of(&field.ty).ok_or_else(|| {
                    unsupported_record(name, &format!("layout of {}", field.ty)).at(Site::of(field))
                })?;
                let ty = self
                    .lower_type(&field.ty)
                    .map_err(|error| error.at(Site::of(field)))?;
                if pack.is_some() && self.contains_raised_align(&field.ty) {
                    return Err(unsupported_record(name, "layout").at(Site::of(field)));
                }
                (
                    field_name(field, index),
                    ty,
                    layout.offsets[index],
                    size,
                    pack.map_or(field_align, |pack| field_align.min(pack)),
                )
            };
            if is_union {
                if offset != 0 {
                    return Err(unsupported_record(name, "layout"));
                }
            } else {
                let natural = end.next_multiple_of(field_align);
                if natural != offset {
                    if offset < natural || offset % field_align != 0 {
                        return Err(unsupported_record(name, "layout"));
                    }
                    lowered.push(padding_field(lowered.len(), offset - end));
                }
            }
            end = end.max(offset + size);
            align = align.max(field_align);
            lowered.push(rust::RecordField {
                comments: std::mem::take(&mut pending),
                name: field_name.into(),
                ty,
            });
        }
        if layout.align < align {
            return Err(unsupported_record(name, "layout"));
        }
        let raised = (layout.align > align).then_some(layout.align);
        if raised.is_some() && pack.is_some() {
            return Err(unsupported_record(name, "layout"));
        }
        if end.next_multiple_of(layout.align) != layout.size {
            if layout.size < end || layout.size % layout.align != 0 {
                return Err(unsupported_record(name, "layout"));
            }
            let padding = if is_union {
                layout.size
            } else {
                layout.size - end
            };
            lowered.push(padding_field(lowered.len(), padding));
        }
        let mut record = record(lowered, pack, raised);
        record.comments = self.claim_comments(owner);
        record.trailing_comments = pending;
        record
            .trailing_comments
            .extend(self.claim_inner_comments(owner));
        Ok(record)
    }

    fn record_pack(
        &self,
        fields: &[slate_parser::ast::Span<ir::Field>],
        layout: &ir::RecordLayout,
    ) -> Option<u64> {
        fields
            .iter()
            .enumerate()
            .filter(|(_, field)| field.bit_width.is_none())
            .any(|(index, field)| {
                self.tables.storage_of(&field.ty).is_some_and(|(_, align)| {
                    align > layout.align || !layout.offsets[index].is_multiple_of(align)
                })
            })
            .then_some(layout.align)
    }

    fn contains_raised_align(&self, ty: &ir::Type) -> bool {
        match self.tables.resolve_type(ty) {
            ir::Type::Array { element, .. } => self.contains_raised_align(element),
            ir::Type::Defined(id) => {
                let raised = matches!(
                    self.dependencies.records.get(&id.0),
                    Some(Record::Built(record)) if record.align.is_some()
                );
                raised
                    || matches!(
                        self.tables.types.get(id).map(|definition| &definition.kind),
                        Some(ir::TypeDefinitionKind::Record { fields: Some(fields), .. })
                            if fields.iter().any(|field| self.contains_raised_align(&field.ty))
                    )
            }
            _ => false,
        }
    }

    fn lower_bit_unit(
        &mut self,
        id: TypeId,
        name: &str,
        fields: &[slate_parser::ast::Span<ir::Field>],
        layout: &ir::RecordLayout,
        unit: usize,
    ) -> Result<(rust::Type, u64)> {
        if !matches!(self.tables.target.endian, Endian::Little) {
            return Err(unsupported_record(name, "big-endian bit-fields"));
        }
        let storage = &layout.bit_units[unit];
        let has_wide_member = fields.iter().enumerate().any(|(index, field)| {
            layout.field_units[index] == Some(unit)
                && field.name.is_some()
                && self.tables.wide_int(&field.ty).is_some()
        });
        if has_wide_member {
            return self.lower_wide_bit_unit(id, name, fields, layout, unit);
        }
        let (backing, unit_align) = match storage.size {
            1 | 2 | 4 | 8 | 16
                if storage.offset.is_multiple_of(storage.size) && storage.size <= layout.align =>
            {
                let prim = match storage.size {
                    1 => Prim::U8,
                    2 => Prim::U16,
                    4 => Prim::U32,
                    8 => Prim::U64,
                    _ => Prim::U128,
                };
                (rust::Type::Prim(prim), storage.size)
            }
            size => (byte_array(size), 1),
        };
        let mut members = Vec::new();
        let mut cursor = 0u64;
        for (index, field) in fields.iter().enumerate() {
            if layout.field_units[index] != Some(unit) || field.name.is_none() {
                continue;
            }
            let (Some(width), Some(bit_offset)) = (field.bit_width, layout.bit_offsets[index])
            else {
                continue;
            };
            let start = bit_offset - storage.offset * 8;
            push_bit_padding(&mut members, start - cursor);
            members.push(rust::StructField {
                attrs: vec![bits_attr(u64::from(width), false)],
                name: field_name(field, index),
                ty: self
                    .lower_type(&field.ty)
                    .map_err(|error| error.at(Site::of(field)))?,
            });
            cursor = start + u64::from(width);
        }
        push_bit_padding(&mut members, storage.size * 8 - cursor);
        let wrapper = format!("__SlateBits{}U{unit}", id.0);
        let mut args = vec![rust::AttrArg::Type(backing)];
        args.push(rust::AttrArg::Named(
            "c_names".into(),
            Box::new(rust::AttrArg::Bool(true)),
        ));
        for feature in [
            "new",
            "from_into_bits",
            "from_traits",
            "default",
            "debug",
            "builder",
            "bit_ops",
        ] {
            args.push(rust::AttrArg::Named(
                feature.into(),
                Box::new(rust::AttrArg::Bool(false)),
            ));
        }
        self.dependencies
            .bit_units
            .push(Item::Struct(rust::StructDef {
                attrs: vec![Attr::Call {
                    path: rust::Path::new(["bitfields", "bitfield"].map(rust::Ident::from)),
                    args,
                }],
                vis: rust::Visibility::Pub,
                field_vis: rust::Visibility::Pub,
                generics: Vec::new(),
                name: wrapper.clone(),
                fields: rust::StructFields::Named(members),
            }));
        Ok((
            rust::Type::Custom(format!("{BIT_UNIT_MODULE}::{wrapper}")),
            unit_align,
        ))
    }

    fn lower_wide_bit_unit(
        &mut self,
        id: TypeId,
        name: &str,
        fields: &[slate_parser::ast::Span<ir::Field>],
        layout: &ir::RecordLayout,
        unit: usize,
    ) -> Result<(rust::Type, u64)> {
        let storage = &layout.bit_units[unit];
        let wrapper = format!("__SlateBits{}U{unit}", id.0);
        let unit_type = rust::Type::Custom(format!("{BIT_UNIT_MODULE}::{wrapper}"));
        let literal = |value: u64| Expr::Value(rust::RustValue::U128(u128::from(value)));
        let bytes = |mutable: bool| Expr::Ref {
            mutable,
            expr: Box::new(Expr::Field {
                base: Box::new(Expr::Var("self".into())),
                field: "bytes".into(),
            }),
        };
        let mut methods = Vec::new();
        for (index, field) in fields.iter().enumerate() {
            if layout.field_units[index] != Some(unit) || field.name.is_none() {
                continue;
            }
            let (Some(width), Some(bit_offset)) = (field.bit_width, layout.bit_offsets[index])
            else {
                continue;
            };
            let start = bit_offset - storage.offset * 8;
            let member_type = self
                .lower_type(&field.ty)
                .map_err(|error| error.at(Site::of(field)))?;
            let member = field_name(field, index);
            let range = [literal(start), literal(u64::from(width))];
            let call = |function: &str, mut args: Vec<Expr>| Expr::Call {
                func: Box::new(Expr::Var(function.into())),
                args: {
                    args.extend(range.clone());
                    args
                },
                binding: CallBinding::Generated,
            };
            let value = || Expr::Var("value".into());
            let (getter, setter) = if self.tables.wide_int(&field.ty).is_some() {
                let type_name = crate::backend::codegen::type_to_string(&member_type);
                (
                    call(
                        &format!("<{type_name}>::from_bit_range"),
                        vec![bytes(false)],
                    ),
                    Expr::MethodCall {
                        recv: Box::new(value()),
                        method: "write_bit_range".into(),
                        args: [bytes(true)].into_iter().chain(range).collect(),
                    },
                )
            } else {
                let rust::Type::Prim(prim) = member_type else {
                    return Err(unsupported_record(name, "bit-field type").at(Site::of(field)));
                };
                let signed = matches!(
                    prim,
                    Prim::I8 | Prim::I16 | Prim::I32 | Prim::I64 | Prim::I128 | Prim::Isize
                );
                let loaded = call(
                    if signed {
                        "bitint::load_bits_signed"
                    } else {
                        "bitint::load_bits"
                    },
                    vec![bytes(false)],
                );
                let getter = if prim == Prim::Bool {
                    Expr::Binary {
                        op: BinOp::Ne,
                        lhs: Box::new(loaded),
                        rhs: Box::new(literal(0)),
                    }
                } else {
                    Expr::Cast {
                        expr: Box::new(loaded),
                        ty: rust::Type::Prim(prim),
                    }
                };
                let setter = Expr::Call {
                    func: Box::new(Expr::Var("bitint::store_bits".into())),
                    args: vec![
                        bytes(true),
                        literal(start),
                        literal(u64::from(width)),
                        Expr::Cast {
                            expr: Box::new(value()),
                            ty: rust::Type::Prim(Prim::U128),
                        },
                    ],
                    binding: CallBinding::Generated,
                };
                (getter, setter)
            };
            methods.push(rust::ImplItem::Method(rust::Method {
                is_const: true,
                name: format!("__get_{member}"),
                self_kind: rust::SelfKind::Ref,
                params: Vec::new(),
                ret: Some(self.lower_type(&field.ty)?),
                body: getter,
            }));
            methods.push(rust::ImplItem::Method(rust::Method {
                is_const: true,
                name: format!("__set_{member}"),
                self_kind: rust::SelfKind::RefMut,
                params: vec![rust::FnParam {
                    comments: Vec::new(),
                    name: "value".into(),
                    mutable: false,
                    ty: self.lower_type(&field.ty)?,
                }],
                ret: None,
                body: setter,
            }));
        }
        self.dependencies
            .bit_units
            .push(Item::Struct(rust::StructDef {
                attrs: vec![Attr::Derive(vec![rust::Derive::Clone, rust::Derive::Copy])],
                vis: rust::Visibility::Pub,
                field_vis: rust::Visibility::Pub,
                generics: Vec::new(),
                name: wrapper.clone(),
                fields: rust::StructFields::Named(vec![rust::StructField {
                    attrs: Vec::new(),
                    name: "bytes".into(),
                    ty: byte_array(storage.size),
                }]),
            }));
        self.dependencies
            .bit_unit_impls
            .push(Item::Impl(rust::ImplBlock {
                generics: Vec::new(),
                trait_: None,
                self_ty: unit_type.clone(),
                items: methods,
            }));
        Ok((unit_type, 1))
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
                    Some(_) => Err(unsupported_type(ty)),
                    None => Err(Invariant::UnresolvedType(*id).into()),
                };
            }
            ir::Type::Bool => Prim::Bool,
            ir::Type::Numeric(ir::NumericType::Integer { .. })
                if let Some(wide) = self.tables.wide_int(ty) =>
            {
                self.dependencies.huge_stack |= wide.is_huge();
                return Ok(wide.rust_type());
            }
            ir::Type::Numeric(ir::NumericType::Integer {
                width,
                signed,
                bit_precise,
            }) => match (*width, *signed) {
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
                _ if *bit_precise && let Some((_, _, container)) = odd_width(ty) => container,
                _ => return Err(unsupported_type(ty)),
            },
            ir::Type::Numeric(ir::NumericType::Float(ir::FloatType::F16)) => {
                self.dependencies.f16 = true;
                Prim::F16
            }
            ir::Type::Numeric(ir::NumericType::Float(ir::FloatType::F128)) => {
                self.dependencies.f128 = true;
                self.dependencies.libgcc |= matches!(
                    self.tables.target.environment,
                    TargetEnvironment::Gnu
                        | TargetEnvironment::GnuEabi
                        | TargetEnvironment::GnuEabiHf
                );
                Prim::F128
            }
            ir::Type::Numeric(ir::NumericType::Float(ir::FloatType::F32)) => Prim::F32,
            ir::Type::Numeric(ir::NumericType::Float(ir::FloatType::F64)) => Prim::F64,
            ir::Type::Numeric(ir::NumericType::Float(ir::FloatType::F80)) => {
                self.dependencies.long_double = true;
                return Ok(rust::Type::LongDouble);
            }
            ir::Type::Pointer { .. } if self.tables.function_type_has_vector(ty) => {
                return Err(unsupported_type(ty));
            }
            ir::Type::Pointer { pointee, .. } if matches!(**pointee, ir::Type::Function { .. }) => {
                let ir::Type::Function {
                    return_type,
                    parameters,
                    variadic,
                    prototyped: true,
                    convention,
                } = &**pointee
                else {
                    return Err(unsupported_type(ty));
                };
                let Some(abi) = self.tables.convention_abi(*convention).filter(|abi| {
                    *abi == rust::Abi::CUnwind || !self.tables.function_type_passes_long_double(ty)
                }) else {
                    return Err(unsupported_type(ty));
                };
                let mut params = parameters
                    .iter()
                    .map(|ty| self.lower_type(ty))
                    .collect::<Result<Vec<_>>>()?;
                if *variadic {
                    params.push(rust::Type::Variadic);
                }
                return Ok(rust::Type::FnPtr {
                    abi,
                    params,
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
            ir::Type::Array { element, length } => {
                return Ok(rust::Type::Array {
                    elem: Box::new(self.lower_type(element)?),
                    len: length.unwrap_or(0),
                });
            }
            ir::Type::VariableArray { element, .. } => {
                return Ok(rust::Type::Array {
                    elem: Box::new(self.lower_type(element)?),
                    len: 0,
                });
            }
            ir::Type::Vector { element, lanes } => {
                let ty = intrinsics::simd_type(
                    self.lower_type(&ir::Type::Numeric(*element))?,
                    (*lanes).into(),
                )
                .ok_or_else(|| unsupported_type(ty))?;
                self.dependencies.simd = true;
                return Ok(ty);
            }
            ir::Type::VaList => return Ok(rust::Type::VaList),
            ir::Type::Complex(component)
                if matches!(
                    component,
                    ir::NumericType::Integer { .. }
                        | ir::NumericType::Float(
                            ir::FloatType::F16
                                | ir::FloatType::F32
                                | ir::FloatType::F64
                                | ir::FloatType::F80
                                | ir::FloatType::F128
                        )
                ) =>
            {
                if matches!(component, ir::NumericType::Float(ir::FloatType::F80)) {
                    self.dependencies.complex_long_double = true;
                }
                return Ok(rust::Type::Complex(Box::new(
                    self.lower_type(&ir::Type::Numeric(*component))?,
                )));
            }
            _ => return Err(unsupported_type(ty)),
        };
        Ok(rust::Type::Prim(primitive))
    }
}

pub(super) fn field_name(field: &ir::Field, index: usize) -> String {
    match &field.name {
        Some(name) => path_keyword_free(name.as_str()),
        None => format!("__slate_anon_{index}"),
    }
}

pub(super) fn bit_unit_name(unit: usize) -> String {
    format!("__slate_bits_{unit}")
}

fn byte_array(len: u64) -> rust::Type {
    rust::Type::Array {
        elem: Box::new(rust::Type::Prim(Prim::U8)),
        len,
    }
}

fn padding_field(position: usize, len: u64) -> rust::RecordField {
    rust::RecordField {
        comments: Vec::new(),
        name: format!("__slate_pad_{position}").into(),
        ty: byte_array(len),
    }
}

fn bits_attr(width: u64, padding: bool) -> Attr {
    let mut args = vec![rust::AttrArg::UInt(width)];
    if padding {
        args.push(rust::AttrArg::Named(
            "access".into(),
            Box::new(rust::AttrArg::Type(rust::Type::Custom("na".into()))),
        ));
    }
    Attr::Call {
        path: rust::Path::new([rust::Ident::from("bits")]),
        args,
    }
}

fn push_bit_padding(members: &mut Vec<rust::StructField>, mut width: u64) {
    while width > 0 {
        let chunk = width.min(128);
        let prim = match chunk {
            0..=8 => Prim::U8,
            9..=16 => Prim::U16,
            17..=32 => Prim::U32,
            33..=64 => Prim::U64,
            _ => Prim::U128,
        };
        members.push(rust::StructField {
            attrs: vec![bits_attr(chunk, true)],
            name: format!("__slate_pad_{}", members.len()),
            ty: rust::Type::Prim(prim),
        });
        width -= chunk;
    }
}

pub(super) fn unsupported_type(ty: &ir::Type) -> Failure {
    Construct::Type { ty: ty.to_string() }.into()
}

fn unsupported_record(name: &str, detail: &str) -> Failure {
    Construct::Record {
        name: name.into(),
        detail: detail.into(),
    }
    .into()
}
