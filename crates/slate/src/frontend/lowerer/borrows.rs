use super::*;
use slate_analysis::Borrow;

fn same_pointee(from: &ir::Type, to: &ir::Type) -> bool {
    matches!((from, to), (ir::Type::Pointer { pointee: left, .. }, ir::Type::Pointer { pointee: right, .. }) if left == right)
}

fn reborrow(mutable: bool, expr: Expr) -> Expr {
    Expr::Ref {
        mutable,
        expr: Box::new(Expr::Unary {
            op: rust::UnaryOp::Deref,
            expr: Box::new(expr),
        }),
    }
}

fn first_element(mutable: bool, slice: Expr) -> Expr {
    Expr::Ref {
        mutable,
        expr: Box::new(Expr::Index {
            base: Box::new(slice),
            index: Box::new(Expr::Value(rust::RustValue::Usize(0))),
        }),
    }
}

fn one_element_slice(mutable: bool, element: Expr) -> Expr {
    Expr::Call {
        func: Box::new(Expr::Var(
            if mutable {
                "core::slice::from_mut"
            } else {
                "core::slice::from_ref"
            }
            .into(),
        )),
        args: vec![element],
        binding: CallBinding::Generated,
    }
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn borrow_of(&self, binding: BindingId) -> Option<Borrow> {
        if self.unborrowed.contains(&binding) || self.switch_lets.contains(&binding) {
            return None;
        }
        self.tables.borrow_of(binding)
    }

    fn borrowed_place<'p>(
        &self,
        place: &'p ir::Place,
    ) -> Option<(BindingId, Borrow, Option<&'p ir::Value>)> {
        let PlaceKind::Deref(pointer) = &place.kind else {
            return None;
        };
        if self.packed_pointee(pointer) {
            return None;
        }
        if let Some((id, borrow)) = self.borrowed_binding(pointer) {
            return Some((id, borrow, None));
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
        borrow.many.then_some((id, borrow, Some(amount)))
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
        let Some((id, borrow, index)) = self.borrowed_place(place) else {
            return Ok(None);
        };
        let binding = Expr::Var(self.binding_name(id).as_str().into());
        if !borrow.many {
            return Ok(Some(Expr::Unary {
                op: rust::UnaryOp::Deref,
                expr: Box::new(binding),
            }));
        }
        let index = match index {
            Some(amount) => Expr::Cast {
                expr: Box::new(self.lower_value(amount)?),
                ty: rust::Type::Prim(Prim::Usize),
            },
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
        Ok(rust::Type::Ref {
            mutable: borrow.mutable,
            inner: if borrow.many {
                Box::new(rust::Type::Slice(inner))
            } else {
                inner
            },
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
        let Some(borrow) = self.borrow_of(*id) else {
            return Ok(None);
        };
        let binding = Expr::Var(self.binding_name(*id).as_str().into());
        let raw = self.lower_type(&value.ty)?;
        if !borrow.many {
            let binding = match &raw {
                rust::Type::Ptr {
                    mutable: true,
                    inner,
                } if !borrow.mutable => Expr::Cast {
                    expr: Box::new(binding),
                    ty: rust::Type::Ptr {
                        mutable: false,
                        inner: inner.clone(),
                    },
                },
                _ => binding,
            };
            return Ok(Some(Expr::Cast {
                expr: Box::new(binding),
                ty: raw,
            }));
        }
        let pointer = Expr::MethodCall {
            recv: Box::new(binding),
            method: if borrow.mutable {
                "as_mut_ptr"
            } else {
                "as_ptr"
            }
            .into(),
            args: Vec::new(),
        };
        Ok(Some(match raw {
            rust::Type::Ptr { mutable, .. } if mutable != borrow.mutable => Expr::Cast {
                expr: Box::new(pointer),
                ty: raw,
            },
            _ => pointer,
        }))
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

    pub(super) fn lower_borrow(&mut self, value: &ir::Value, target: Borrow) -> Result<Expr> {
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
                    kind: PlaceKind::Binding(_),
                    ..
                },
            ) => {
                let object = Expr::Ref {
                    mutable: target.mutable,
                    expr: Box::new(self.lower_place(place)?),
                };
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
                    }
                } else {
                    first_element(target.mutable, array)
                })
            }
            _ if let Some((id, source)) = self.borrowed_binding(value)
                && (source.mutable || !target.mutable) =>
            {
                let binding = Expr::Var(self.binding_name(id).as_str().into());
                Ok(match (source.many, target.many) {
                    (false, false) | (true, true) if source.mutable => {
                        reborrow(target.mutable, binding)
                    }
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
                Ok(Expr::Unsafe(Box::new(rust::Block {
                    stmts: Vec::new(),
                    tail: Some(Box::new(reborrow(target.mutable, raw))),
                })))
            }
            _ => Err(Invariant::UnprovenBorrow(value.to_string()).into()),
        }
    }
}
