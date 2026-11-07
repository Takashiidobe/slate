use super::*;
use slate_parser::sema::ctype::{CTypeKind, CTypes, IntRank, QualType};
use slate_parser::target_info::{Endian, TargetEnvironment, TargetFamily, TargetOs};

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

    pub(super) fn union_field_fills_every_byte(&self, ty: &ir::Type, index: usize) -> bool {
        if !self.is_union(ty) || self.has_bit_fields(ty) {
            return false;
        }
        let Some(field) = self.record_fields(ty).and_then(|fields| fields.get(index)) else {
            return false;
        };
        !self.has_padding_bytes(&field.ty)
            && self.storage_of(&field.ty).map(|(size, _)| size)
                == self.storage_of(ty).map(|(size, _)| size)
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

    fn erased_storage(&self, size: u64, align: u64) -> (rust::Type, u64) {
        [
            (16, Prim::U16),
            (32, Prim::U32),
            (64, Prim::U64),
            (128, Prim::U128),
        ]
        .into_iter()
        .find_map(|(width, prim)| {
            let unit = self
                .storage_of(&ir::Type::Numeric(ir::NumericType::Integer {
                    width,
                    signed: false,
                    bit_precise: false,
                }))
                .filter(|&(unit_size, unit_align)| {
                    unit_align == align && size.is_multiple_of(unit_size)
                })?;
            Some((
                rust::Type::Array {
                    elem: Box::new(rust::Type::Prim(prim)),
                    len: size / unit.0,
                },
                align,
            ))
        })
        .unwrap_or_else(|| (byte_array(size), 1))
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
            ir::Type::VariableArray { element, .. } => {
                self.storage_of(element).map(|(_, align)| (0, align))
            }
            ty => self
                .rust_layout
                .storage_of(ty.clone())
                .ok()
                .map(|layout| (layout.size_bytes, layout.alignment_bytes.into())),
        }
    }
}

impl Tables<'_> {
    fn rust_layout_matches_c(&self, ty: &ir::Type) -> bool {
        match self.resolve_type(ty) {
            ir::Type::Bool
            | ir::Type::Numeric(
                ir::NumericType::Integer {
                    bit_precise: false, ..
                }
                | ir::NumericType::Float(ir::FloatType::F32 | ir::FloatType::F64),
            )
            | ir::Type::Pointer {
                space: ir::PointerSpace::Default,
                ..
            } => true,
            ir::Type::Numeric(ir::NumericType::Float(ir::FloatType::F80)) => self
                .target
                .storage_of(ir::Type::Numeric(ir::NumericType::Float(
                    ir::FloatType::F80,
                )))
                .is_ok_and(|layout| {
                    layout.size_bytes == 10u64.next_multiple_of(layout.alignment_bytes.into())
                }),
            ir::Type::Array {
                element,
                length: Some(_),
            } => self.rust_layout_matches_c(element),
            ir::Type::Defined(id) => match self.types.get(id).map(|definition| &definition.kind) {
                Some(ir::TypeDefinitionKind::Record {
                    fields: Some(fields),
                    layout: Some(_),
                    ..
                }) => fields
                    .iter()
                    .all(|field| !field.access.atomic && self.rust_layout_matches_c(&field.ty)),
                _ => false,
            },
            _ => false,
        }
    }
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn lower_layout_query(
        &mut self,
        value: &ir::Value,
        query: &ir::LayoutQuery,
        amount: u64,
    ) -> Result<Expr> {
        let (ir::LayoutQuery::SizeOf { ty: queried, c }
        | ir::LayoutQuery::AlignOf { ty: queried, c }
        | ir::LayoutQuery::OffsetOf { ty: queried, c, .. }) = query;
        let storage = self.tables.storage_of(queried);
        let amount_matches = match query {
            ir::LayoutQuery::SizeOf { .. } => storage.is_some_and(|(size, _)| size == amount),
            ir::LayoutQuery::AlignOf { .. } => storage.is_some_and(|(_, align)| align == amount),
            ir::LayoutQuery::OffsetOf { .. } => true,
        };
        if amount_matches
            && self.tables.rust_layout_matches_c(queried)
            && let Ok(ty) = self.lower_c_type(queried, Some(*c))
            && let Some(query) = self.rust_layout_query(query, ty, amount)
        {
            return Ok(Expr::Cast {
                expr: Box::new(Expr::LayoutQuery {
                    query,
                    amount: rust::TargetAmount(amount),
                }),
                ty: self.lower_value_type(value)?,
            });
        }
        let mut literal = self.lower_number(&ir::Number::Integer(amount.into()), &value.ty)?;
        if let Expr::Cast { ty, .. } = &mut literal {
            *ty = self.lower_value_type(value)?;
        }
        Ok(literal)
    }

    fn rust_layout_query(
        &self,
        query: &ir::LayoutQuery,
        ty: rust::Type,
        amount: u64,
    ) -> Option<rust::LayoutQuery> {
        match query {
            ir::LayoutQuery::SizeOf { .. } => Some(rust::LayoutQuery::SizeOf(ty)),
            ir::LayoutQuery::AlignOf { .. } => Some(rust::LayoutQuery::AlignOf(ty)),
            ir::LayoutQuery::OffsetOf {
                ty: root, members, ..
            } => {
                let fields = self.offset_of_fields(root, members, amount)?;
                Some(rust::LayoutQuery::OffsetOf { ty, fields })
            }
        }
    }

    fn offset_of_fields(
        &self,
        root: &ir::Type,
        members: &[String],
        amount: u64,
    ) -> Option<Vec<String>> {
        let tables = self.tables;
        let mut current = root;
        let mut offset = 0;
        let mut names = Vec::with_capacity(members.len());
        for member in members {
            let ir::Type::Defined(id) = tables.resolve_type(current) else {
                return None;
            };
            let ir::TypeDefinitionKind::Record {
                fields: Some(fields),
                layout: Some(layout),
                ..
            } = &tables.types.get(id)?.kind
            else {
                return None;
            };
            let (index, field) = fields
                .iter()
                .enumerate()
                .find(|(_, field)| field.name.as_ref() == Some(member))?;
            if field.bit_width.is_some() || self.dependencies.erased_fields.contains(&(id.0, index))
            {
                return None;
            }
            offset += layout.offsets.get(index)?;
            names.push(field_name(field, index));
            current = &field.ty;
        }
        (offset == amount).then_some(names)
    }

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
        let record = |fields: Vec<_>, packed: Option<u64>, align: Option<u64>| rust::RecordDef {
            comments: Vec::new(),
            trailing_comments: Vec::new(),
            vis: rust::Visibility::Private,
            field_vis: rust::Visibility::Private,
            is_union: is_union && !fields.is_empty(),
            name: name.to_owned(),
            fields,
            packed: packed.map(|packed| packed as u32),
            align: align.map(|align| align as u32),
            opaque: false,
        };
        let fields = fields.as_deref().unwrap_or_default();
        let Some(layout) = layout else {
            return match fields {
                [] => Ok(rust::RecordDef {
                    opaque: true,
                    ..record(Vec::new(), None, None)
                }),
                _ => Err(unsupported_record(name, "layout")),
            };
        };
        let pack = self.record_pack(fields, layout);
        let byte_layout = pack.is_some_and(|pack| {
            fields
                .iter()
                .enumerate()
                .filter(|(_, field)| field.bit_width.is_none())
                .any(|(index, field)| {
                    self.tables.storage_of(&field.ty).is_some_and(|(_, align)| {
                        !layout.offsets[index].is_multiple_of(align.min(pack))
                    })
                })
        });
        let pack = pack.filter(|_| !byte_layout);
        let mut lowered = Vec::new();
        let owner = self.tables.types[&id].id;
        let mut pending = Vec::new();
        let mut end = 0u64;
        let mut align = 1u64;
        let mut units = BTreeSet::new();
        for (index, field) in fields.iter().enumerate() {
            pending.extend(self.claim_comments(field.id));
            if (field.access.atomic
                && !self.tables.atomic_scalar(&field.ty)
                && !self.tables.target.triple.contains("linux"))
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
                    .lower_c_type(&field.ty, self.tables.c_types.nodes.get(&field.id).copied())
                    .map_err(|error| error.at(Site::of(field)))?;
                let field_align = if byte_layout {
                    1
                } else {
                    pack.map_or(field_align, |pack| field_align.min(pack))
                };
                let (ty, field_align) = if byte_layout
                    || (pack.is_some() && self.contains_raised_align(&field.ty))
                {
                    self.dependencies.erased_fields.insert((id.0, index));
                    let (storage, storage_align) = self.tables.erased_storage(size, field_align);
                    (
                        rust::Type::Generic {
                            name: "std::mem::MaybeUninit".into(),
                            args: vec![storage],
                        },
                        storage_align,
                    )
                } else {
                    (ty, field_align)
                };
                (
                    field_name(field, index),
                    ty,
                    layout.offsets[index],
                    size,
                    field_align,
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
                cfg: None,
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
            ir::Type::Numeric(ir::NumericType::Float(ir::FloatType::F80)) => true,
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
                let provided = self
                    .tables
                    .rust_layout
                    .storage_of(ir::Type::Numeric(ir::NumericType::integer(
                        storage.size as u32 * 8,
                        false,
                    )))
                    .map_or(storage.size, |layout| u64::from(layout.alignment_bytes));
                (rust::Type::Prim(prim), storage.size.min(provided))
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

    pub(super) fn lower_value_type(&mut self, value: &ir::Value) -> Result<rust::Type> {
        self.lower_c_type(&value.ty, self.tables.c_types.value(value))
    }

    pub(super) fn lower_binding_type(
        &mut self,
        ty: &ir::Type,
        binding: ir::BindingId,
    ) -> Result<rust::Type> {
        self.lower_c_type(ty, self.tables.c_types.bindings.get(&binding).copied())
    }

    pub(super) fn lower_c_type(
        &mut self,
        ty: &ir::Type,
        c: Option<QualType>,
    ) -> Result<rust::Type> {
        let Some(c) = c.filter(|_| self.tables.named_c_types) else {
            return self.lower_type(ty);
        };
        let tables = self.tables;
        let types = &tables.c_types.types;
        let shape = types.kind(types.desugar(c).ty);
        match (ty, shape) {
            (ir::Type::Defined(id), _) => match tables.types.get(id).map(|d| &d.kind) {
                Some(ir::TypeDefinitionKind::Alias(inner)) => self.lower_c_type(inner, Some(c)),
                _ => self.lower_type(ty),
            },
            (ir::Type::Numeric(ir::NumericType::Integer { .. }), _) => {
                let lowered = self.lower_type(ty)?;
                let rust::Type::Prim(resolved) = lowered else {
                    return Ok(lowered);
                };
                Ok(rust::Type::Prim(self.named_prim(c, resolved)))
            }
            (
                ir::Type::Pointer {
                    pointee, is_const, ..
                },
                CTypeKind::Pointer(c_pointee, _),
            ) if tables.is_file(pointee) => self.lower_type(ty),
            (
                ir::Type::Pointer {
                    pointee, is_const, ..
                },
                CTypeKind::Pointer(c_pointee, _),
            ) if !matches!(**pointee, ir::Type::Function { .. }) => Ok(rust::Type::Ptr {
                mutable: !is_const,
                inner: Box::new(self.lower_c_type(pointee, Some(*c_pointee))?),
            }),
            (ir::Type::Pointer { pointee, .. }, CTypeKind::Pointer(c_pointee, _)) => {
                let lowered = self.lower_type(ty)?;
                let (
                    ir::Type::Function {
                        return_type,
                        parameters,
                        ..
                    },
                    CTypeKind::Function {
                        ret: c_ret,
                        params: c_params,
                        ..
                    },
                    rust::Type::FnPtr { abi, params, ret },
                ) = (
                    &**pointee,
                    types.kind(types.desugar(*c_pointee).ty),
                    lowered.clone(),
                )
                else {
                    return Ok(lowered);
                };
                if parameters.len() != c_params.len() {
                    return Ok(lowered);
                }
                let mut named = params;
                for (index, parameter) in parameters.iter().enumerate() {
                    named[index] = self.lower_c_type(parameter, Some(c_params[index]))?;
                }
                let ret = match return_type {
                    Some(return_type) => Box::new(self.lower_c_type(return_type, Some(*c_ret))?),
                    None => ret,
                };
                Ok(rust::Type::FnPtr {
                    abi,
                    params: named,
                    ret,
                })
            }
            (
                ir::Type::Array { element, length },
                CTypeKind::Array {
                    element: c_element, ..
                },
            ) => Ok(rust::Type::Array {
                elem: Box::new(self.lower_c_type(element, Some(*c_element))?),
                len: length.unwrap_or(0),
            }),
            _ => self.lower_type(ty),
        }
    }

    fn support_record_path(&self, id: TypeId) -> Option<&'static str> {
        let definition = self.tables.types.get(&id)?;
        definition.provenance.system_header?;
        crate::libc_catalog::record_path(definition, &self.tables.target.triple)
    }

    fn named_prim(&mut self, c: QualType, resolved: Prim) -> Prim {
        let Some(name) = known_c_name(&self.tables.c_types.types, c, &self.tables.target.triple)
            .filter(|name| {
                !self
                    .tables
                    .record_names
                    .values()
                    .any(|record| record == name)
            })
        else {
            return resolved;
        };
        let (width, signed) = match resolved {
            Prim::I8 => (8, true),
            Prim::I16 => (16, true),
            Prim::I32 => (32, true),
            Prim::I64 => (64, true),
            Prim::U8 => (8, false),
            Prim::U16 => (16, false),
            Prim::U32 => (32, false),
            Prim::U64 => (64, false),
            _ => return resolved,
        };
        self.dependencies.named_prims.insert(name, resolved);
        Prim::Named(rust::NamedPrim {
            name,
            width,
            signed,
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
                    Some(ir::TypeDefinitionKind::Record { .. })
                        if self.tables.stdio.file_records.contains(id) =>
                    {
                        self.dependencies.stdio_layout_used = true;
                        Ok(stdio::file_type())
                    }
                    Some(ir::TypeDefinitionKind::Record { .. })
                        if let Some(path) = self.support_record_path(*id) =>
                    {
                        Ok(rust::Type::Custom(path.into()))
                    }
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
            ir::Type::Pointer { pointee, .. } if matches!(**pointee, ir::Type::Function { .. }) => {
                let ir::Type::Function {
                    return_type,
                    parameters,
                    variadic,
                    convention,
                    ..
                } = &**pointee
                else {
                    return Err(unsupported_type(ty));
                };
                let Some(abi) = self
                    .tables
                    .convention_abi(*convention)
                    .filter(|abi| match abi {
                        rust::Abi::CUnwind => true,
                        rust::Abi::VectorcallUnwind => {
                            !*variadic
                                && parameters
                                    .iter()
                                    .chain(return_type.as_deref())
                                    .all(|ty| self.tables.vectorcall_scalar(ty))
                        }
                        rust::Abi::FastcallUnwind => {
                            self.tables.fastcall_matches_rustc(parameters)
                                && !self.tables.function_type_passes_long_double(ty)
                        }
                        _ => !self.tables.function_type_passes_long_double(ty),
                    })
                else {
                    return Err(unsupported_type(ty));
                };
                self.dependencies.vectorcall |= abi == rust::Abi::VectorcallUnwind;
                let abi = match (self.tables.function_type_has_vector(ty), variadic) {
                    (false, _) => abi,
                    (true, false) => rust::Abi::Rust,
                    (true, true) => return Err(unsupported_type(ty)),
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
            } if self.tables.is_file(pointee) => {
                return Ok(rust::Type::Ptr {
                    mutable: !is_const,
                    inner: Box::new(stdio::file_type()),
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
            ir::Type::BlockPointer { .. } => {
                self.dependencies.blocks = true;
                return Ok(rust::Type::Ptr {
                    mutable: true,
                    inner: Box::new(rust::Type::Custom("std::ffi::c_void".into())),
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
                let lane = self.lower_type(&ir::Type::Numeric(*element))?;
                if let Some(simd) = intrinsics::simd_type(lane.clone(), (*lanes).into()) {
                    self.dependencies.simd = true;
                    return Ok(simd);
                }
                let (_, alignment) = self
                    .tables
                    .storage_of(ty)
                    .ok_or_else(|| unsupported_type(ty))?;
                let ty = vectors::slate_vector_type(lane, (*lanes).into(), alignment)
                    .ok_or_else(|| unsupported_type(ty))?;
                self.dependencies.slate_vector = true;
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

const FIXED_WIDTH_TYPEDEFS: &[&str] = &[
    "int8_t",
    "int16_t",
    "int32_t",
    "int64_t",
    "uint8_t",
    "uint16_t",
    "uint32_t",
    "uint64_t",
    "__int8_t",
    "__int16_t",
    "__int32_t",
    "__int64_t",
    "__uint8_t",
    "__uint16_t",
    "__uint32_t",
    "__uint64_t",
    "u_int8_t",
    "u_int16_t",
    "u_int32_t",
    "u_int64_t",
];

const NAMED_TYPEDEFS: &[&str] = &[
    "size_t",
    "ssize_t",
    "ptrdiff_t",
    "intptr_t",
    "uintptr_t",
    "intmax_t",
    "uintmax_t",
    "wchar_t",
    "wint_t",
    "off_t",
    "off64_t",
    "time_t",
    "clock_t",
    "pid_t",
    "uid_t",
    "gid_t",
    "mode_t",
    "dev_t",
    "ino_t",
    "nlink_t",
    "socklen_t",
];

fn known_c_name(types: &CTypes, mut c: QualType, target: &str) -> Option<&'static str> {
    loop {
        c = match types.kind(c.ty) {
            CTypeKind::Typedef {
                name, underlying, ..
            } => {
                if FIXED_WIDTH_TYPEDEFS.contains(&name.as_str()) {
                    return None;
                }
                match name.as_str() {
                    "__size_t" => return Some("size_t"),
                    "__ptrdiff_t" => return Some("ptrdiff_t"),
                    _ => {}
                }
                if let Some(known) = NAMED_TYPEDEFS.iter().find(|known| **known == name) {
                    return Some(known);
                }
                if let Some(symbol) =
                    crate::libc_catalog::lookup(name, target, crate::libc_catalog::Kind::Type)
                {
                    return Some(symbol.name);
                }
                *underlying
            }
            CTypeKind::TypeOf { underlying, .. } => *underlying,
            CTypeKind::AtomicSpecifier(inner) => *inner,
            CTypeKind::Char => return Some("c_char"),
            CTypeKind::Int {
                rank: IntRank::Long,
                signed: true,
            } => return Some("c_long"),
            CTypeKind::Int {
                rank: IntRank::Long,
                signed: false,
            } => return Some("c_ulong"),
            _ => return None,
        };
    }
}

fn core_ffi_definition(name: &str, target: &TargetInfo) -> Option<Prim> {
    let long_is_64 = target.long_width == 64;
    Some(match name {
        "c_char" if target.char_signed => Prim::I8,
        "c_char" => Prim::U8,
        "c_long" if long_is_64 => Prim::I64,
        "c_long" => Prim::I32,
        "c_ulong" if long_is_64 => Prim::U64,
        "c_ulong" => Prim::U32,
        _ => return None,
    })
}

fn slate_support_definition(name: &str, target: &TargetInfo) -> Option<Prim> {
    let wide = target.pointer_width == 64;
    let windows = matches!(target.os, TargetOs::Windows);
    let musl = target.triple.contains("musl");
    let darwin = matches!(target.os, TargetOs::Darwin);
    let freebsd = matches!(target.os, TargetOs::FreeBsd);
    let x86_64 = matches!(target.family, TargetFamily::X86_64);
    let narrow_glibc_or_bionic =
        !wide && !musl && matches!(target.os, TargetOs::Linux | TargetOs::Android);
    Some(match name {
        "intmax_t" | "off64_t" => Prim::I64,
        "uintmax_t" => Prim::U64,
        "pid_t" => Prim::I32,
        "uid_t" | "gid_t" => Prim::U32,
        "socklen_t" if windows => Prim::I32,
        "socklen_t" => Prim::U32,
        "wint_t" if windows => Prim::U16,
        "wint_t" if darwin || freebsd => Prim::I32,
        "wint_t" => Prim::U32,
        "time_t" if !wide && matches!(target.os, TargetOs::Android) => Prim::I32,
        "time_t" => Prim::I64,
        "clock_t" if darwin => Prim::U64,
        "clock_t" if windows || freebsd || !wide => Prim::I32,
        "clock_t" => Prim::I64,
        "mode_t" if darwin || freebsd => Prim::U16,
        "mode_t" => Prim::U32,
        "dev_t" if darwin => Prim::I32,
        "dev_t" if windows => Prim::U32,
        "dev_t" if !wide && matches!(target.os, TargetOs::Android) => Prim::U32,
        "dev_t" => Prim::U64,
        "ino_t" if windows => Prim::U16,
        "ino_t" if narrow_glibc_or_bionic || (darwin && x86_64) => Prim::U32,
        "ino_t" => Prim::U64,
        "nlink_t" if darwin => Prim::U16,
        "nlink_t" if freebsd || (x86_64 && matches!(target.os, TargetOs::Linux)) => Prim::U64,
        "nlink_t" => Prim::U32,
        "size_t" | "uintptr_t" if wide => Prim::U64,
        "size_t" | "uintptr_t" => Prim::U32,
        "ssize_t" | "ptrdiff_t" | "intptr_t" if wide => Prim::I64,
        "ssize_t" | "ptrdiff_t" | "intptr_t" => Prim::I32,
        "off_t" if windows => Prim::I32,
        "off_t" if narrow_glibc_or_bionic => Prim::I32,
        "off_t" => Prim::I64,
        "wchar_t" if windows => Prim::U16,
        "wchar_t" if x86_64 || matches!(target.family, TargetFamily::X86) || darwin => Prim::I32,
        "wchar_t" => Prim::U32,
        _ => return None,
    })
}

fn use_item(segments: &[&str]) -> Item {
    Item::Use {
        vis: rust::Visibility::Private,
        path: rust::Path::new(segments.iter().copied().map(rust::Ident::from)),
        alias: None,
    }
}

pub(super) fn named_prim_item(name: &str, resolved: Prim, target: &TargetInfo) -> Item {
    if core_ffi_definition(name, target) == Some(resolved) {
        use_item(&["core", "ffi", name])
    } else if let Some(symbol) =
        crate::libc_catalog::lookup(name, &target.triple, crate::libc_catalog::Kind::Type)
        && slate_support_definition(name, target).is_none_or(|definition| definition == resolved)
    {
        use_item(&symbol.rust_path.split("::").collect::<Vec<_>>())
    } else if slate_support_definition(name, target) == Some(resolved) {
        use_item(&["slate_support", name])
    } else {
        Item::TypeAlias {
            name: name.to_string(),
            ty: rust::Type::Prim(resolved),
        }
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
        cfg: None,
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
