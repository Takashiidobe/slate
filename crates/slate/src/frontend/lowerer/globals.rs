use super::*;
use slate_parser::target_info::{LARGE_ARRAY_BYTES, TargetFamily, TargetOs};

pub(super) fn lower_string_global(global: &ir::Global) -> Option<Result<Vec<u8>>> {
    if !global.variable.name.starts_with('.') {
        return None;
    }
    let ir::ValueKind::CodeUnits(units) = &global.variable.initializer.as_ref()?.node.value else {
        return None;
    };
    if !matches!(
        &global.variable.ty,
        ir::Type::Array { element, .. }
            if matches!(**element, ir::Type::Numeric(ir::NumericType::Integer { width: 8, .. }))
    ) {
        return None;
    }
    Some(
        units
            .iter()
            .map(|unit| {
                u8::try_from(*unit).map_err(|_| {
                    Failure::from(Construct::Global {
                        name: global.variable.name.clone(),
                        detail: "wide string".into(),
                    })
                })
            })
            .collect(),
    )
}

pub(super) fn register_global(
    global: &ir::Global,
    target: &TargetInfo,
) -> Option<Result<&'static str>> {
    let register = global.variable.register.as_ref()?;
    let name = match (target.family, register.canonical) {
        (TargetFamily::X86_64, Some("sp")) => "rsp",
        (TargetFamily::X86_64, Some("bp")) => "rbp",
        (TargetFamily::X86, Some("sp")) => "esp",
        (TargetFamily::X86, Some("bp")) => "ebp",
        (TargetFamily::X86_64 | TargetFamily::X86, _) => return None,
        _ => {
            return Some(Err(Construct::Global {
                name: global.variable.name.clone(),
                detail: format!("register variable `{}`", register.spelling),
            }
            .into()));
        }
    };
    Some(Ok(name))
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn lower_register_read(
        &mut self,
        register: &str,
        value: &ir::Value,
    ) -> Result<Expr> {
        if !matches!(
            self.tables.resolve_type(&value.ty),
            ir::Type::Pointer { .. } | ir::Type::Numeric(ir::NumericType::Integer { .. })
        ) {
            return Err(unsupported_value(value));
        }
        let ty = self.lower_type(&value.ty)?;
        let temp = self.next_temp();
        Ok(Expr::Unsafe(Box::new(rust::Block {
            stmts: vec![
                Stmt::Let {
                    name: temp.clone(),
                    mutable: false,
                    ty: Some(rust::Type::Prim(Prim::Usize)),
                    init: None,
                },
                Stmt::InlineAsm(rust::InlineAsm {
                    template: format!("mov {{}}, {register}"),
                    operands: vec![rust::AsmOperand::Out {
                        reg: rust::AsmReg::Class("reg".into()),
                        late: true,
                        value: Expr::Var(temp.as_str().into()),
                    }],
                    options: Vec::new(),
                    kind: rust::AsmKind::Statement,
                }),
            ],
            tail: Some(Box::new(Expr::Cast {
                expr: Box::new(Expr::Var(temp.as_str().into())),
                ty,
            })),
        })))
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum AlignWrapper {
    Exact(u64),
    LargeArray,
}

impl AlignWrapper {
    pub(super) fn name(self) -> String {
        match self {
            Self::Exact(alignment) => format!("__SlateAlign{alignment}"),
            Self::LargeArray => "__SlateAlignLargeArray".into(),
        }
    }
}

pub(super) fn dll_import_symbol(global: &ir::Global, target: &TargetInfo) -> Option<String> {
    if global.definition
        || global.symbol.dll_storage != Some(ir::DllStorage::Import)
        || target.os != TargetOs::Windows
    {
        return None;
    }
    let symbol = global
        .symbol
        .asm_name
        .as_deref()
        .unwrap_or(&global.variable.name);
    Some(match target.family {
        TargetFamily::X86 => format!("_imp__{symbol}"),
        _ => format!("__imp_{symbol}"),
    })
}

impl Tables<'_> {
    pub(super) fn over_alignment(
        &self,
        ty: &ir::Type,
        alignment: Option<u64>,
    ) -> Option<AlignWrapper> {
        if self.named_c_types && self.has_large_array_alignment(ty, alignment) {
            return Some(AlignWrapper::LargeArray);
        }
        let alignment = alignment?;
        let (_, natural) = self.storage_of(ty)?;
        (alignment > natural).then_some(AlignWrapper::Exact(alignment))
    }

    fn has_large_array_alignment(&self, ty: &ir::Type, alignment: Option<u64>) -> bool {
        if !matches!(
            ty,
            ir::Type::Array {
                length: Some(_),
                ..
            }
        ) {
            return false;
        }
        let Some((size, natural)) = self.storage_of(ty) else {
            return false;
        };
        let abi = self
            .target
            .large_array_alignment()
            .map_or(natural, |large| natural.max(large));
        size >= LARGE_ARRAY_BYTES && alignment.unwrap_or(natural) == abi
    }

    pub(super) fn is_constant_initializer(&self, value: &ir::Value) -> bool {
        match &value.node.value {
            ValueKind::Constant(_)
            | ValueKind::LayoutQuery { .. }
            | ValueKind::CodeUnits(_)
            | ValueKind::Null
            | ValueKind::LabelAddress(_)
            | ValueKind::FunctionDecay { .. } => true,
            ValueKind::Block(block) => block.captures.is_empty(),
            ValueKind::Read {
                place:
                    ir::Place {
                        kind: PlaceKind::Binding(id),
                        ..
                    },
                ordering: None,
            } if let Some(initializer) = self.constexprs.get(id) => {
                self.is_constant_initializer(initializer)
            }
            ValueKind::Copy { operand, .. } => self.is_constant_initializer(operand),
            ValueKind::ArrayDecay { place, .. } | ValueKind::AddressOf(place) => {
                self.is_constant_address(place)
            }
            ValueKind::PointerOffset {
                pointer, amount, ..
            } => self.is_constant_initializer(pointer) && self.is_constant_initializer(amount),
            ValueKind::Convert {
                kind: ir::ConversionKind::RealToComplex,
                operand,
                ..
            } => self.is_constant_initializer(operand),
            ValueKind::Convert { .. } if self.is_long_double(&value.ty) => {
                self.fold_long_double(value).is_some()
            }
            ValueKind::Convert { operand, .. } if self.is_long_double(&operand.ty) => {
                self.fold_narrowed_long_double(value).is_some()
                    || self.fold_integer(value).is_some()
            }
            ValueKind::Convert { .. } if self.is_complex_long_double(&value.ty) => {
                self.fold_complex_long_double(value).is_some()
            }
            ValueKind::Convert { operand, .. } if self.is_complex_long_double(&operand.ty) => {
                self.fold_complex_bits(value).is_some()
            }
            ValueKind::Convert { operand, .. } => {
                !self.is_long_double(&operand.ty)
                    && !self.is_long_double(&value.ty)
                    && self.is_constant_initializer(operand)
            }
            ValueKind::Aggregate { members, .. } => members
                .iter()
                .all(|member| self.is_constant_initializer(&member.value)),
            ValueKind::Arith { .. } | ValueKind::Unary { .. }
                if self.complex_float_kind(&value.ty).is_some() =>
            {
                self.fold_complex_bits(value).is_some()
            }
            ValueKind::Unary { operand, .. } => {
                if self.is_long_double(&value.ty) {
                    self.fold_long_double(value).is_some()
                } else {
                    self.is_constant_initializer(operand)
                }
            }
            ValueKind::Arith { left, right, .. } => {
                if self.is_long_double(&value.ty) {
                    self.fold_long_double(value).is_some()
                } else {
                    self.is_constant_initializer(left) && self.is_constant_initializer(right)
                }
            }
            ValueKind::Compare { left, right, .. } => [left, right].iter().all(|operand| {
                !self.is_long_double(&operand.ty)
                    && !matches!(self.resolve_type(&operand.ty), ir::Type::Pointer { .. })
                    && self.is_constant_initializer(operand)
            }),
            ValueKind::Logical { left, right, .. } => {
                self.is_constant_initializer(left) && self.is_constant_initializer(right)
            }
            ValueKind::Conditional {
                condition,
                then_value,
                else_value,
            } => {
                !self.bridges_long_double(&value.ty)
                    && [condition, then_value, else_value]
                        .iter()
                        .all(|operand| self.is_constant_initializer(operand))
            }
            ValueKind::PointerDifference { left, right, .. } => matches!(
                (&left.node.value, &right.node.value),
                (ValueKind::LabelAddress(_), ValueKind::LabelAddress(_))
            ),
            _ => false,
        }
    }

    pub(super) fn flexible_member<'g>(
        &self,
        global: &'g ir::Global,
    ) -> Option<(usize, &'g ir::Type)> {
        let ValueKind::Aggregate { members, .. } =
            &global.variable.initializer.as_ref()?.node.value
        else {
            return None;
        };
        let fields = self.record_fields(&global.variable.ty)?;
        members.iter().find_map(|member| {
            let ir::AggregateTarget::Field(index) = member.target else {
                return None;
            };
            let ir::Type::Array { length: None, .. } = self.resolve_type(&fields.get(index)?.ty)
            else {
                return None;
            };
            matches!(
                self.resolve_type(&member.value.ty),
                ir::Type::Array {
                    length: Some(1..),
                    ..
                }
            )
            .then_some((index, &member.value.ty))
        })
    }

    pub(super) fn is_constant_address(&self, place: &ir::Place) -> bool {
        match &place.kind {
            PlaceKind::Binding(id) if self.indirect_globals.contains(id) => false,
            PlaceKind::Binding(id) => {
                self.statics.contains(id)
                    || self.strings.contains_key(id)
                    || self.names.contains_key(id)
            }
            PlaceKind::Field { base, .. } => self.is_constant_address(base),
            PlaceKind::CompoundLiteral {
                storage: ir::StorageDuration::Static,
                initializer,
                ..
            } => self.is_constant_initializer(initializer),
            PlaceKind::Deref(pointer) => self.is_constant_initializer(pointer),
            PlaceKind::Index { base, index } => {
                self.is_constant_initializer(base) && self.is_constant_initializer(index)
            }
            _ => false,
        }
    }

    pub(super) fn holds_address_integer(&self, value: &ir::Value) -> bool {
        match &value.node.value {
            ValueKind::Aggregate { members, .. } => members
                .iter()
                .any(|member| self.holds_address_integer(&member.value)),
            _ => self.is_address_integer(value),
        }
    }

    pub(super) fn is_address_integer(&self, value: &ir::Value) -> bool {
        let pointer_sized = |ty: &ir::Type| {
            matches!(
                self.resolve_type(ty),
                ir::Type::Numeric(ir::NumericType::Integer { .. })
            ) && self
                .storage_of(ty)
                .is_some_and(|(size, _)| size * 8 == u64::from(self.target.pointer_width))
        };
        if !pointer_sized(&value.ty) {
            return false;
        }
        match &value.node.value {
            ValueKind::Convert {
                kind: ir::ConversionKind::PtrToInt,
                operand,
                ..
            } => {
                self.null_address_offset(operand).is_none() && self.is_constant_initializer(operand)
            }
            ValueKind::Convert { operand, .. } => {
                pointer_sized(&operand.ty) && self.is_address_integer(operand)
            }
            ValueKind::Arith {
                op: ir::ArithOp::Add,
                left,
                right,
                ..
            } => {
                (self.is_address_integer(left) && self.fold_integer(right).is_some())
                    || (self.fold_integer(left).is_some() && self.is_address_integer(right))
            }
            ValueKind::Arith {
                op: ir::ArithOp::Sub,
                left,
                right,
                ..
            } => self.is_address_integer(left) && self.fold_integer(right).is_some(),
            _ => false,
        }
    }
}

impl FunctionLowerer<'_, '_> {
    fn address_image(
        &mut self,
        value: &ir::Value,
        image: rust::Type,
        wrapped: bool,
    ) -> Result<Expr> {
        let storage = self.next_temp();
        let pointer = self.next_temp();
        let mut stmts = vec![
            Stmt::Let {
                name: storage.clone(),
                mutable: true,
                ty: Some(maybe_uninit(image)),
                init: Some(Expr::Call {
                    func: Box::new(Expr::Var("std::mem::MaybeUninit::zeroed".into())),
                    args: Vec::new(),
                    binding: CallBinding::Generated,
                }),
            },
            Stmt::Let {
                name: pointer.clone(),
                mutable: false,
                ty: None,
                init: Some(Expr::MethodCall {
                    recv: Box::new(Expr::Var(storage.as_str().into())),
                    method: "as_mut_ptr".into(),
                    args: Vec::new(),
                }),
            },
        ];
        let mut base = Expr::Unary {
            op: rust::UnaryOp::Deref,
            expr: Box::new(Expr::Var(pointer.as_str().into())),
        };
        if wrapped {
            base = Expr::TupleField {
                base: Box::new(base),
                index: 0,
            };
        }
        self.in_static_initializer = true;
        self.address_image_fill = true;
        let filled = match &value.node.value {
            ValueKind::Aggregate { members, .. } => {
                self.fill_aggregate(&value.ty, members, &base, true, &mut stmts)
            }
            _ => self
                .address_integer_write(base, value)
                .map(|write| stmts.push(write)),
        };
        self.in_static_initializer = false;
        self.address_image_fill = false;
        filled?;
        Ok(Expr::Unsafe(Box::new(rust::Block {
            stmts,
            tail: Some(Box::new(Expr::Var(storage.as_str().into()))),
        })))
    }

    pub(super) fn address_integer_write(&mut self, place: Expr, value: &ir::Value) -> Result<Stmt> {
        let opaque = rust::Type::Ptr {
            mutable: false,
            inner: Box::new(rust::Type::Unit),
        };
        Ok(Stmt::Expr(Expr::MethodCall {
            recv: Box::new(Expr::Cast {
                expr: Box::new(Expr::AddrOf {
                    mutable: true,
                    expr: Box::new(place),
                }),
                ty: rust::Type::Ptr {
                    mutable: true,
                    inner: Box::new(opaque),
                },
            }),
            method: "write".into(),
            args: vec![self.lower_address_integer(value)?],
        }))
    }

    fn lower_address_integer(&mut self, value: &ir::Value) -> Result<Expr> {
        let offset = |address: Expr, amount: i128| Expr::MethodCall {
            recv: Box::new(address),
            method: "wrapping_byte_offset".into(),
            args: vec![Expr::Value(rust::RustValue::TypedInt(amount, Prim::Isize))],
        };
        let tables = self.tables;
        match &value.node.value {
            ValueKind::Convert {
                kind: ir::ConversionKind::PtrToInt,
                operand,
                ..
            } => Ok(Expr::Transmute {
                from: self.lower_value_type(operand)?,
                to: rust::Type::Ptr {
                    mutable: false,
                    inner: Box::new(rust::Type::Unit),
                },
                expr: Box::new(self.lower_value(operand)?),
            }),
            ValueKind::Convert { operand, .. } => self.lower_address_integer(operand),
            ValueKind::Arith {
                op, left, right, ..
            } => {
                let (address, amount) = match tables.fold_integer(right) {
                    Some(amount) if tables.is_address_integer(left) => (left, amount),
                    _ => (right, tables.fold_integer(left).unwrap_or_default()),
                };
                let amount = if *op == ir::ArithOp::Sub {
                    -amount
                } else {
                    amount
                };
                Ok(offset(self.lower_address_integer(address)?, amount))
            }
            _ => Err(unsupported_value(value)),
        }
    }

    pub(super) fn lower_static(
        &mut self,
        global: &ir::Global,
    ) -> Result<(rust::Type, Option<Expr>)> {
        self.owner = owner_name(&global.variable.name);
        let tables = self.tables;
        let variable = &global.variable;
        let symbol = ir::SymbolAttributes {
            asm_name: None,
            visibility: None,
            used: false,
            include: false,
            section: None,
            retain: false,
            fallback: None,
            weak: global.symbol.weak && global.definition,
            dll_storage: global
                .symbol
                .dll_storage
                .filter(|storage| global.definition || *storage != ir::DllStorage::Import),
            ..global.symbol.clone()
        };
        if matches!(variable.storage, ir::StorageDuration::Automatic)
            || (variable.alignment.is_some() && tables.storage_of(&variable.ty).is_none())
            || (variable.access.atomic
                && !tables.atomic_unpadded(&variable.ty)
                && !tables.target.triple.contains("linux"))
            || symbol != ir::SymbolAttributes::default()
        {
            return Err(Construct::Global {
                name: variable.name.clone(),
                detail: "attributes".into(),
            }
            .into());
        }
        let ty = self.lower_binding_type(&variable.ty, variable.id)?;
        if !global.definition {
            return Ok((ty, None));
        }
        let flexible = tables.flexible_member(global);
        let ty = match flexible {
            Some((index, sized)) => self.flexible_storage(global, index, sized)?,
            None => ty,
        };
        if tables.address_images.contains(&variable.id)
            && let Some(value) = &variable.initializer
        {
            let wrapper = tables.over_aligned.get(&variable.id).copied();
            let storage = match wrapper {
                Some(wrapper) => self.align_wrapped(&variable.name, wrapper, ty)?,
                None => ty,
            };
            let image = image_name(&binding_name(variable.id, &tables.bindings));
            self.dependencies.image_aliases.push(Item::TypeAlias {
                name: image.clone(),
                ty: storage,
            });
            let image = rust::Type::Custom(image);
            let init = self.address_image(value, image.clone(), wrapper.is_some())?;
            return Ok((maybe_uninit(image), Some(init)));
        }
        let init = match &variable.initializer {
            None => zeroed(),
            Some(value) if tables.is_constant_initializer(value) => {
                self.in_static_initializer = true;
                let lowered = match (&value.node.value, flexible) {
                    (ValueKind::Aggregate { members, zero_fill }, Some(_)) => {
                        self.lower_aggregate_into(&value.ty, members, *zero_fill, Some(ty.clone()))
                    }
                    _ => self.lower_value(value),
                };
                self.in_static_initializer = false;
                lowered?
            }
            Some(value) => {
                return Err(Failure::from(Construct::Global {
                    name: variable.name.clone(),
                    detail: format!("non-constant initializer {}", value.display(false)),
                })
                .at(Site::of(&value.node)));
            }
        };
        let Some(&wrapper) = tables.over_aligned.get(&variable.id) else {
            return Ok((ty, Some(init)));
        };
        Ok((
            self.align_wrapped(&variable.name, wrapper, ty)?,
            Some(align_wrap(wrapper, init)),
        ))
    }

    fn flexible_storage(
        &mut self,
        global: &ir::Global,
        index: usize,
        sized: &ir::Type,
    ) -> Result<rust::Type> {
        let tables = self.tables;
        let unsupported = |detail: &str| {
            Failure::from(Construct::Global {
                name: global.variable.name.clone(),
                detail: detail.into(),
            })
        };
        if tables.over_aligned.contains_key(&global.variable.id)
            || tables.has_bit_fields(&global.variable.ty)
        {
            return Err(unsupported("flexible array member storage"));
        }
        let ir::Type::Defined(id) = *tables.resolve_type(&global.variable.ty) else {
            return Err(unsupported("flexible array member storage"));
        };
        let ir::TypeDefinitionKind::Record {
            kind,
            fields: Some(fields),
            layout: Some(layout),
        } = &tables.types[&id].kind
        else {
            return Err(unsupported("flexible array member storage"));
        };
        let (size, _) = tables
            .storage_of(sized)
            .ok_or_else(|| unsupported("flexible array member storage"))?;
        let mut fields = fields.clone();
        fields[index].value.ty = sized.clone();
        let layout = ir::RecordLayout {
            size: layout
                .size
                .max(layout.offsets[index] + size)
                .next_multiple_of(layout.align),
            ..layout.clone()
        };
        let name = format!(
            "__SlateStorage_{}",
            binding_name(global.variable.id, &tables.bindings)
        );
        let record = self.build_record(
            id,
            &ir::TypeDefinitionKind::Record {
                kind: *kind,
                fields: Some(fields),
                layout: Some(layout),
            },
            &name,
        )?;
        self.dependencies
            .flexible_records
            .push(Item::Record(record));
        Ok(rust::Type::Custom(name))
    }

    pub(super) fn align_wrapped(
        &mut self,
        name: &str,
        wrapper: AlignWrapper,
        ty: rust::Type,
    ) -> Result<rust::Type> {
        match wrapper {
            AlignWrapper::Exact(alignment) => {
                self.dependencies
                    .align_wrappers
                    .insert(u32::try_from(alignment).map_err(|_| Construct::Global {
                        name: name.into(),
                        detail: format!("alignment {alignment}"),
                    })?);
            }
            AlignWrapper::LargeArray => self.dependencies.large_array_wrapper = true,
        }
        Ok(rust::Type::Generic {
            name: wrapper.name(),
            args: vec![ty],
        })
    }
}

pub(super) fn image_name(binding: &str) -> String {
    format!("__SlateImage_{binding}")
}

fn maybe_uninit(ty: rust::Type) -> rust::Type {
    rust::Type::Generic {
        name: "std::mem::MaybeUninit".into(),
        args: vec![ty],
    }
}

pub(super) fn align_wrap(wrapper: AlignWrapper, value: Expr) -> Expr {
    Expr::Call {
        func: Box::new(Expr::Var(wrapper.name().into())),
        args: vec![value],
        binding: CallBinding::Generated,
    }
}
