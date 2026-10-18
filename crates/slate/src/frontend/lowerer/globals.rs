use super::*;
use slate_parser::target_info::TargetFamily;

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

pub(super) fn align_wrapper(alignment: u64) -> String {
    format!("__SlateAlign{alignment}")
}

impl Tables<'_> {
    pub(super) fn over_alignment(&self, ty: &ir::Type, alignment: Option<u64>) -> Option<u64> {
        let alignment = alignment?;
        let (_, natural) = self.storage_of(ty)?;
        (alignment > natural).then_some(alignment)
    }

    pub(super) fn is_constant_initializer(&self, value: &ir::Value) -> bool {
        match &value.node.value {
            ValueKind::Constant(_)
            | ValueKind::CodeUnits(_)
            | ValueKind::Null
            | ValueKind::LabelAddress(_)
            | ValueKind::FunctionDecay { .. } => true,
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
            ValueKind::Convert { operand, .. }
                if self.is_complex_long_double(&value.ty)
                    || self.is_complex_long_double(&operand.ty) =>
            {
                self.fold_complex_long_double(value).is_some()
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
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn lower_static(
        &mut self,
        global: &ir::Global,
    ) -> Result<(rust::Type, Option<Expr>)> {
        let tables = self.tables;
        let variable = &global.variable;
        let symbol = ir::SymbolAttributes {
            visibility: None,
            used: false,
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
        let ty = self.lower_type(&variable.ty)?;
        if !global.definition {
            return Ok((ty, None));
        }
        let flexible = tables.flexible_member(global);
        let ty = match flexible {
            Some((index, sized)) => self.flexible_storage(global, index, sized)?,
            None => ty,
        };
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
        let Some(&alignment) = tables.over_aligned.get(&variable.id) else {
            return Ok((ty, Some(init)));
        };
        Ok((
            self.align_wrapped(&variable.name, alignment, ty)?,
            Some(align_wrap(alignment, init)),
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
        alignment: u64,
        ty: rust::Type,
    ) -> Result<rust::Type> {
        self.dependencies
            .align_wrappers
            .insert(u32::try_from(alignment).map_err(|_| Construct::Global {
                name: name.into(),
                detail: format!("alignment {alignment}"),
            })?);
        Ok(rust::Type::Generic {
            name: align_wrapper(alignment),
            args: vec![ty],
        })
    }
}

pub(super) fn align_wrap(alignment: u64, value: Expr) -> Expr {
    Expr::Call {
        func: Box::new(Expr::Var(align_wrapper(alignment).into())),
        args: vec![value],
        binding: CallBinding::Generated,
    }
}
