use super::*;
use slate_analysis::Borrow;

pub(super) fn same_pointee(from: &ir::Type, to: &ir::Type) -> bool {
    matches!((from, to), (ir::Type::Pointer { pointee: left, .. }, ir::Type::Pointer { pointee: right, .. }) if left == right)
}

pub(super) fn c_string_type() -> rust::Type {
    rust::Type::Ref {
        mutable: false,
        inner: Box::new(rust::Type::Custom("core::ffi::CStr".into())),
    }
}

pub(super) fn reborrow(mutable: bool, expr: Expr) -> Expr {
    Expr::Ref {
        mutable,
        expr: Box::new(Expr::Unary {
            op: rust::UnaryOp::Deref(rust::Pointee::Ref),
            expr: Box::new(expr),
        }),
    }
}

pub(super) fn first_element(mutable: bool, slice: Expr) -> Expr {
    Expr::Ref {
        mutable,
        expr: Box::new(Expr::Index {
            base: Box::new(slice),
            index: Box::new(Expr::Value(rust::RustValue::Usize(0))),
        }),
    }
}

pub(super) fn one_element_slice(mutable: bool, element: Expr) -> Expr {
    Expr::Call {
        func: Box::new(Expr::var(
            if mutable {
                "core::slice::from_mut"
            } else {
                "core::slice::from_ref"
            }
            .into(),
        )),
        args: vec![element],
        binding: CallBinding::Generated,
        unsafe_: rust::Unsafe(false),
    }
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn borrow_of(&self, binding: BindingId) -> Option<Borrow> {
        if self.unborrowed.contains(&binding) || self.switch_lets.contains(&binding) {
            return None;
        }
        let borrow = self.tables.borrow_of(binding)?;
        Some(if self.present.contains(&binding) {
            optional::present(borrow)
        } else {
            borrow
        })
    }

    pub(super) fn c_string_of(&self, binding: BindingId) -> bool {
        !self.unborrowed.contains(&binding)
            && !self.switch_lets.contains(&binding)
            && self.tables.c_string_of(binding)
    }

    pub(super) fn lower_c_string_argument(&mut self, value: &ir::Value) -> Result<Expr> {
        match &value.node.value {
            ValueKind::Copy { operand, .. } => self.lower_c_string_argument(operand),
            ValueKind::Convert {
                kind: ir::ConversionKind::PointerCast,
                operand,
                ..
            } if same_pointee(&operand.ty, &value.ty) => self.lower_c_string_argument(operand),
            ValueKind::ArrayDecay {
                place:
                    ir::Place {
                        kind: PlaceKind::Binding(id),
                        ..
                    },
                length: Some(_),
            } if let Some((0, content)) = self
                .tables
                .strings
                .get(id)
                .and_then(|bytes| bytes.split_last())
                && !content.contains(&0) =>
            {
                Ok(Expr::CStr(content.to_vec()))
            }
            ValueKind::Read {
                place:
                    ir::Place {
                        kind: PlaceKind::Binding(id),
                        ..
                    },
                ordering: None,
            } if self.c_string_of(*id) => Ok(Expr::var(self.binding_name(*id).as_str().into())),
            _ => Err(Invariant::UnprovenBorrow(value.to_string()).into()),
        }
    }

    pub(super) fn element_view<'v>(
        &self,
        value: &'v ir::Value,
    ) -> Option<(BindingId, &'v ir::Value)> {
        let ValueKind::PointerOffset {
            pointer,
            amount,
            subtract: false,
            ..
        } = &value.node.value
        else {
            return None;
        };
        let (id, borrow) = self.borrowed_binding(pointer)?;
        let mut index = &**amount;
        while let ValueKind::Convert { operand, .. } | ValueKind::Copy { operand, .. } =
            &index.node.value
        {
            index = operand;
        }
        let pure = matches!(
            &index.node.value,
            ValueKind::Constant(_)
                | ValueKind::Read {
                    place: ir::Place {
                        kind: PlaceKind::Binding(_),
                        ..
                    },
                    ordering: None,
                }
        );
        (borrow.many && pure && !self.packed_pointee(pointer)).then_some((id, &**amount))
    }

    fn borrowed_place(&self, place: &ir::Place) -> Option<(BindingId, Borrow, Option<ir::Value>)> {
        let PlaceKind::Deref(pointer) = &place.kind else {
            return None;
        };
        if self.packed_pointee(pointer) {
            return None;
        }
        if let Some((id, borrow)) = self.borrowed_binding(pointer) {
            return Some((id, borrow, None));
        }
        if let ValueKind::Read {
            place:
                ir::Place {
                    kind: PlaceKind::Binding(temporary),
                    ..
                },
            ordering: None,
        } = &pointer.node.value
            && let Some((id, amount, _)) = self.views.get(temporary)
        {
            return Some((*id, self.borrow_of(*id)?, Some(amount.clone())));
        }
        let ValueKind::PointerOffset {
            pointer,
            amount,
            subtract: false,
            ..
        } = &pointer.node.value
        else {
            return None;
        };
        let (id, borrow) = self.borrowed_binding(pointer)?;
        borrow
            .many
            .then_some((id, borrow, Some((**amount).clone())))
    }

    fn packed_pointee(&self, pointer: &ir::Value) -> bool {
        let ir::Type::Pointer { pointee, .. } = self.tables.resolve_type(&pointer.ty) else {
            return false;
        };
        matches!(
            self.tables.resolve_type(pointee),
            ir::Type::Defined(id)
                if matches!(
                    self.dependencies.records.get(&id.0),
                    Some(Record::Built(record)) if record.packed.is_some()
                )
        )
    }

    fn root_borrow(&self, place: &ir::Place, through_unions: bool) -> Option<Borrow> {
        match &place.kind {
            PlaceKind::Deref(_) => self.borrowed_place(place).map(|(_, borrow, _)| borrow),
            PlaceKind::Field { base, .. } if through_unions || !self.tables.is_union(&base.ty) => {
                self.root_borrow(base, through_unions)
            }
            PlaceKind::ComplexPart { base, .. } | PlaceKind::Lane { base, .. } => {
                self.root_borrow(base, through_unions)
            }
            #[expect(
                slate::sensitive_wildcard,
                reason = "only dereferences of borrowed bindings and their members are safe"
            )]
            _ => None,
        }
    }

    pub(super) fn borrowed_root(&self, place: &ir::Place) -> bool {
        self.root_borrow(place, false).is_some()
    }

    pub(super) fn shared_root(&self, place: &ir::Place) -> bool {
        self.root_borrow(place, true)
            .is_some_and(|borrow| !borrow.mutable)
    }

    pub(super) fn lower_borrowed_place(&mut self, place: &ir::Place) -> Result<Option<Expr>> {
        if let PlaceKind::Deref(pointer) = &place.kind
            && let Some(borrow) = self.lifted_return(pointer)
        {
            let call = self.lower_lifted_call(pointer)?;
            let call = if borrow.nullable {
                optional::unwrap(call)
            } else {
                call
            };
            return Ok(Some(if borrow.many {
                Expr::Index {
                    base: Box::new(call),
                    index: Box::new(Expr::Value(rust::RustValue::Usize(0))),
                }
            } else {
                Expr::Unary {
                    op: rust::UnaryOp::Deref(rust::Pointee::Ref),
                    expr: Box::new(call),
                }
            }));
        }
        let Some((id, borrow, index)) = self.borrowed_place(place) else {
            return Ok(None);
        };
        let binding = self.borrowed_var(id, borrow);
        if !borrow.many {
            return Ok(Some(Expr::Unary {
                op: rust::UnaryOp::Deref(rust::Pointee::Ref),
                expr: Box::new(binding),
            }));
        }
        let index = match index {
            Some(amount) => self.lower_index(&amount)?,
            None => Expr::Value(rust::RustValue::Usize(0)),
        };
        Ok(Some(Expr::Index {
            base: Box::new(binding),
            index: Box::new(index),
        }))
    }

    pub(super) fn borrow_type(&mut self, ty: &ir::Type, borrow: Borrow) -> Result<rust::Type> {
        let rust::Type::Ptr { inner, .. } = self.lower_type(ty)? else {
            return Err(Invariant::UnprovenBorrow(ty.to_string()).into());
        };
        let reference = rust::Type::Ref {
            mutable: borrow.mutable,
            inner: if borrow.many {
                Box::new(rust::Type::Slice(inner))
            } else {
                inner
            },
        };
        Ok(if borrow.nullable {
            rust::Type::Generic {
                name: "Option".into(),
                args: vec![reference],
            }
        } else {
            reference
        })
    }

    pub(super) fn borrowed_read(&mut self, value: &ir::Value) -> Result<Option<Expr>> {
        let ValueKind::Read {
            place:
                ir::Place {
                    kind: PlaceKind::Binding(id),
                    ..
                },
            ordering: None,
        } = &value.node.value
        else {
            return Ok(None);
        };
        if let Some((_, _, initializer)) = self.views.get(id) {
            let initializer = initializer.clone();
            return self.lower_value(&initializer).map(Some);
        }
        if let Some(initializer) = self.array_aliases.get(id) {
            let initializer = initializer.clone();
            return self.lower_value(&initializer).map(Some);
        }
        let local = self.string_local(*id);
        if matches!(local, Some(LocalString::Str(_))) {
            return Err(Invariant::UnprovenBorrow(value.to_string()).into());
        }
        if self.c_string_of(*id) || local.is_some() {
            return Ok(Some(Expr::Cast {
                expr: Box::new(Expr::MethodCall {
                    recv: Box::new(Expr::var(self.binding_name(*id).as_str().into())),
                    method: "as_ptr".into(),
                    args: Vec::new(),
                    unsafe_: rust::Unsafe(false),
                }),
                ty: self.lower_type(&value.ty)?,
            }));
        }
        let Some(borrow) = self.borrow_of(*id) else {
            return Ok(None);
        };
        let raw = self.lower_type(&value.ty)?;
        if borrow.nullable {
            return Ok(Some(self.optional_read(*id, borrow, raw)));
        }
        let binding = Expr::var(self.binding_name(*id).as_str().into());
        Ok(Some(optional::raw_view(binding, borrow, raw)))
    }

    pub(super) fn borrowed_binding(&self, pointer: &ir::Value) -> Option<(BindingId, Borrow)> {
        let ValueKind::Read {
            place:
                ir::Place {
                    kind: PlaceKind::Binding(id),
                    ..
                },
            ordering: None,
        } = &pointer.node.value
        else {
            return None;
        };
        Some((*id, self.borrow_of(*id)?))
    }

    pub(super) fn lower_sized_borrow(
        &mut self,
        pointer: &ir::Value,
        length: &ir::Value,
        target: Borrow,
    ) -> Result<Expr> {
        let mut peeled = pointer;
        while let ValueKind::Copy { operand, .. }
        | ValueKind::Convert {
            kind: ir::ConversionKind::PointerCast,
            operand,
            ..
        } = &peeled.node.value
            && same_pointee(&operand.ty, &peeled.ty)
        {
            peeled = operand;
        }
        let safe = match &peeled.node.value {
            ValueKind::ArrayDecay { place, .. } => !self.place_is_unsafe(place),
            #[expect(
                slate::sensitive_wildcard,
                reason = "only safe arrays and borrowed bindings borrow without a length"
            )]
            _ => self.borrowed_binding(peeled).is_some(),
        };
        if safe {
            return self.lower_borrow(pointer, target);
        }
        let raw = self.lower_value(pointer)?;
        let count = self.lower_value(length)?;
        Ok(unsafe_op(Expr::Call {
            func: Box::new(Expr::var(
                if target.mutable {
                    "core::slice::from_raw_parts_mut"
                } else {
                    "core::slice::from_raw_parts"
                }
                .into(),
            )),
            args: vec![
                raw,
                Expr::Cast {
                    expr: Box::new(count),
                    ty: rust::Type::Prim(Prim::Usize),
                },
            ],
            binding: CallBinding::Generated,
            unsafe_: rust::Unsafe(false),
        }))
    }

    pub(super) fn lower_borrow(&mut self, value: &ir::Value, target: Borrow) -> Result<Expr> {
        if target.nullable {
            return self.lower_optional(value, target);
        }
        match &value.node.value {
            ValueKind::Copy { operand, .. } => self.lower_borrow(operand, target),
            ValueKind::Convert {
                kind: ir::ConversionKind::PointerCast,
                operand,
                ..
            } if same_pointee(&operand.ty, &value.ty) => self.lower_borrow(operand, target),
            ValueKind::Conditional {
                condition,
                then_value,
                else_value,
            } => Ok(Expr::If {
                cond: Box::new(self.lower_condition(condition)?),
                then_expr: Box::new(self.lower_borrow(then_value, target)?),
                else_expr: Box::new(self.lower_borrow(else_value, target)?),
            }),
            ValueKind::AddressOf(
                place @ ir::Place {
                    kind:
                        PlaceKind::Binding(_) | PlaceKind::ComplexPart { .. } | PlaceKind::Field { .. },
                    ..
                },
            ) => {
                if self.misaligned_in_rust(place) {
                    return Err(Construct::Place {
                        ir: place.to_string(),
                    }
                    .into());
                }
                let mut object = Expr::Ref {
                    mutable: target.mutable,
                    expr: Box::new(self.lower_place(place)?),
                };
                if self.through_union(place) {
                    object = unsafe_op(object);
                }
                Ok(if target.many {
                    one_element_slice(target.mutable, object)
                } else {
                    object
                })
            }
            ValueKind::ArrayDecay {
                place:
                    place @ ir::Place {
                        kind: PlaceKind::Binding(_),
                        ..
                    },
                length: Some(_),
            } => {
                if let PlaceKind::Binding(id) = place.kind
                    && let Some(bytes) = self.tables.strings.get(&id)
                    && !target.mutable
                    && let rust::Type::Array { elem, .. } = self.lower_type(&place.ty)?
                    && let rust::Type::Prim(prim @ (rust::Prim::I8 | rust::Prim::U8)) = *elem
                {
                    let elements = bytes
                        .iter()
                        .map(|&byte| {
                            let value = match prim {
                                rust::Prim::I8 => i128::from(byte as i8),
                                _ => i128::from(byte),
                            };
                            Expr::Value(rust::RustValue::TypedInt(value, prim))
                        })
                        .collect();
                    let literal = Expr::Ref {
                        mutable: false,
                        expr: Box::new(Expr::ArrayLit(elements)),
                    };
                    return Ok(if target.many {
                        literal
                    } else {
                        first_element(
                            false,
                            Expr::Unary {
                                op: rust::UnaryOp::Deref(rust::Pointee::Ref),
                                expr: Box::new(literal),
                            },
                        )
                    });
                }
                let array = self.lower_place(place)?;
                Ok(if target.many {
                    Expr::MethodCall {
                        recv: Box::new(array),
                        method: if target.mutable {
                            "as_mut_slice"
                        } else {
                            "as_slice"
                        }
                        .into(),
                        args: Vec::new(),
                        unsafe_: rust::Unsafe(false),
                    }
                } else {
                    first_element(target.mutable, array)
                })
            }
            ValueKind::AddressOf(
                place @ ir::Place {
                    kind: PlaceKind::Deref(_),
                    ..
                },
            ) if let Some((id, source, index)) = self.borrowed_place(place)
                && (source.mutable || !target.mutable) =>
            {
                self.lower_element_borrow(id, source, index, target)
            }
            ValueKind::PointerOffset {
                pointer,
                amount,
                subtract: false,
                ..
            } if let Some((id, source)) = self.borrowed_binding(pointer)
                && source.many
                && (source.mutable || !target.mutable)
                && !self.packed_pointee(pointer) =>
            {
                self.lower_element_borrow(id, source, Some((**amount).clone()), target)
            }
            _ if let Some(source) = self.lifted_return(value)
                && (source.mutable || !target.mutable) =>
            {
                let call = self.lower_lifted_call(value)?;
                let call = if source.nullable {
                    Expr::MethodCall {
                        recv: Box::new(call),
                        method: "unwrap".into(),
                        args: Vec::new(),
                        unsafe_: rust::Unsafe(false),
                    }
                } else {
                    call
                };
                Ok(optional::convert_present(call, source, target))
            }
            _ if let Some((id, source)) = self.borrowed_binding(value)
                && (source.mutable || !target.mutable) =>
            {
                let binding = self.borrowed_var(id, source);
                if self.owned(id) && target.many {
                    return Ok(Expr::Ref {
                        mutable: target.mutable,
                        expr: Box::new(binding),
                    });
                }
                Ok(match (source.many, target.many) {
                    (false, false) | (true, true) => binding,
                    (true, false) => first_element(target.mutable, binding),
                    (false, true) if source.mutable => {
                        one_element_slice(target.mutable, reborrow(target.mutable, binding))
                    }
                    (false, true) => one_element_slice(false, binding),
                })
            }
            _ if !target.many => {
                let raw = self.lower_value(value)?;
                Ok(Expr::Ref {
                    mutable: target.mutable,
                    expr: Box::new(Expr::Unary {
                        op: rust::UnaryOp::Deref(rust::Pointee::Raw),
                        expr: Box::new(raw),
                    }),
                })
            }
            _ => Err(Invariant::UnprovenBorrow(value.to_string()).into()),
        }
    }
}
