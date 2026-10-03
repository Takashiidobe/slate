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
            | ValueKind::FunctionDecay { .. } => true,
            ValueKind::ArrayDecay { place, .. } | ValueKind::AddressOf(place) => {
                self.is_constant_address(place)
            }
            ValueKind::PointerOffset {
                pointer, amount, ..
            } => self.is_constant_initializer(pointer) && self.is_constant_initializer(amount),
            ValueKind::Convert { operand, .. } => {
                !self.is_long_double(&operand.ty)
                    && !self.is_long_double(&value.ty)
                    && self.is_constant_initializer(operand)
            }
            ValueKind::Aggregate { members, .. } => members
                .iter()
                .all(|member| self.is_constant_initializer(&member.value)),
            ValueKind::Unary { operand, .. } => {
                !self.is_long_double(&value.ty) && self.is_constant_initializer(operand)
            }
            ValueKind::Arith { left, right, .. } => {
                !self.is_long_double(&value.ty)
                    && self.is_constant_initializer(left)
                    && self.is_constant_initializer(right)
            }
            ValueKind::Compare { left, right, .. } => [left, right].iter().all(|operand| {
                !self.is_long_double(&operand.ty)
                    && !matches!(self.resolve_type(&operand.ty), ir::Type::Pointer { .. })
                    && self.is_constant_initializer(operand)
            }),
            ValueKind::Logical { left, right, .. } => {
                self.is_constant_initializer(left) && self.is_constant_initializer(right)
            }
            _ => false,
        }
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
            visibility: global
                .symbol
                .visibility
                .filter(|visibility| *visibility != ir::Visibility::Default),
            ..global.symbol.clone()
        };
        if matches!(variable.storage, ir::StorageDuration::Automatic)
            || (variable.alignment.is_some() && tables.storage_of(&variable.ty).is_none())
            || (variable.access.atomic && !tables.atomic_scalar(&variable.ty))
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
        let init = match &variable.initializer {
            None => zeroed(),
            Some(value) if tables.is_constant_initializer(value) => self.lower_value(value)?,
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
