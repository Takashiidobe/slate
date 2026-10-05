use super::*;

impl Tables<'_> {
    fn null_address_offset(&self, value: &ir::Value) -> Option<u64> {
        match &value.node.value {
            ValueKind::Null => Some(0),
            ValueKind::Convert { operand, .. }
                if matches!(self.resolve_type(&value.ty), ir::Type::Pointer { .. })
                    && matches!(self.resolve_type(&operand.ty), ir::Type::Pointer { .. }) =>
            {
                self.null_address_offset(operand)
            }
            ValueKind::AddressOf(place) => self.null_place_offset(place),
            _ => None,
        }
    }

    fn null_place_offset(&self, place: &ir::Place) -> Option<u64> {
        match &place.kind {
            PlaceKind::Deref(pointer) => self.null_address_offset(pointer),
            PlaceKind::Field {
                base,
                index,
                bits: None,
            } => {
                let ir::Type::Defined(id) = self.resolve_type(&base.ty) else {
                    return None;
                };
                let ir::TypeDefinitionKind::Record {
                    layout: Some(layout),
                    ..
                } = &self.types.get(id)?.kind
                else {
                    return None;
                };
                self.null_place_offset(base)?
                    .checked_add(*layout.offsets.get(*index)?)
            }
            _ => None,
        }
    }
}

pub(super) fn byte_pointer_type() -> rust::Type {
    rust::Type::Ptr {
        mutable: false,
        inner: Box::new(rust::Type::Prim(Prim::U8)),
    }
}

pub(super) fn convert_function_pointer(from: rust::Type, to: rust::Type, expr: Box<Expr>) -> Expr {
    let usize_ty = rust::Type::Prim(Prim::Usize);
    match (&from, &to) {
        (rust::Type::FnPtr { .. }, _) | (_, rust::Type::FnPtr { .. }) if from == to => *expr,
        (rust::Type::FnPtr { .. }, rust::Type::Ptr { .. } | rust::Type::FnPtr { .. })
        | (rust::Type::Ptr { .. }, rust::Type::FnPtr { .. }) => Expr::Transmute { from, to, expr },
        (rust::Type::FnPtr { .. }, _) => Expr::Cast {
            expr: Box::new(Expr::Transmute {
                from,
                to: usize_ty,
                expr,
            }),
            ty: to,
        },
        (_, rust::Type::FnPtr { .. }) => Expr::Transmute {
            from: usize_ty.clone(),
            to,
            expr: Box::new(Expr::Cast { expr, ty: usize_ty }),
        },
        _ => Expr::Cast { expr, ty: to },
    }
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn byte_pointer(&mut self, pointer: &ir::Value) -> Result<Expr> {
        let expr = Box::new(self.lower_value(pointer)?);
        Ok(match self.lower_type(&pointer.ty)? {
            from @ rust::Type::FnPtr { .. } => Expr::Transmute {
                from,
                to: byte_pointer_type(),
                expr,
            },
            _ => Expr::Cast {
                expr,
                ty: byte_pointer_type(),
            },
        })
    }

    pub(super) fn lower_pointer_offset(
        &mut self,
        value: &ir::Value,
        pointer: &ir::Value,
        amount: &ir::Value,
        subtract: bool,
        element: &ir::Type,
    ) -> Result<Expr> {
        let offset = Expr::Cast {
            expr: Box::new(self.lower_value(amount)?),
            ty: rust::Type::Prim(Prim::Isize),
        };
        let offset = if subtract {
            Expr::Unary {
                op: rust::UnaryOp::Neg,
                expr: Box::new(offset),
            }
        } else {
            offset
        };
        Ok(match element {
            _ if self.tables.variably_modified(element) => Expr::Unsafe(Box::new(rust::Block {
                stmts: Vec::new(),
                tail: Some(Box::new(Expr::MethodCall {
                    recv: Box::new(self.lower_value(pointer)?),
                    method: "byte_offset".into(),
                    args: vec![Expr::Binary {
                        op: BinOp::Mul,
                        lhs: Box::new(offset),
                        rhs: Box::new(self.runtime_stride(element)?),
                    }],
                })),
            })),
            ir::Type::Void => Expr::Cast {
                expr: Box::new(Expr::Unsafe(Box::new(rust::Block {
                    stmts: Vec::new(),
                    tail: Some(Box::new(Expr::MethodCall {
                        recv: Box::new(self.byte_pointer(pointer)?),
                        method: if matches!(pointer.node.value, ValueKind::LabelAddress(_)) {
                            "wrapping_offset"
                        } else {
                            "offset"
                        }
                        .into(),
                        args: vec![offset],
                    })),
                }))),
                ty: self.lower_type(&value.ty)?,
            },
            ir::Type::Function { .. } => Expr::Transmute {
                from: byte_pointer_type(),
                to: self.lower_type(&value.ty)?,
                expr: Box::new(Expr::MethodCall {
                    recv: Box::new(self.byte_pointer(pointer)?),
                    method: "wrapping_offset".into(),
                    args: vec![offset],
                }),
            },
            _ => Expr::Unsafe(Box::new(rust::Block {
                stmts: Vec::new(),
                tail: Some(Box::new(Expr::MethodCall {
                    recv: Box::new(self.lower_value(pointer)?),
                    method: "offset".into(),
                    args: vec![offset],
                })),
            })),
        })
    }

    pub(super) fn lower_pointer_difference(
        &mut self,
        value: &ir::Value,
        left: &ir::Value,
        right: &ir::Value,
        element: &ir::Type,
    ) -> Result<Expr> {
        if let (Some(left), Some(right)) = (
            self.tables.null_address_offset(left),
            self.tables.null_address_offset(right),
        ) {
            let stride = match element {
                ir::Type::Void | ir::Type::Function { .. } => 1,
                _ => self
                    .tables
                    .storage_of(element)
                    .map(|(size, _)| size)
                    .filter(|size| *size != 0)
                    .ok_or_else(|| unsupported_value(value))?,
            };
            return Ok(Expr::Cast {
                expr: Box::new(Expr::Value(rust::RustValue::I128(
                    (i128::from(left) - i128::from(right)) / i128::from(stride),
                ))),
                ty: self.lower_type(&value.ty)?,
            });
        }
        Ok(match element {
            _ if self.tables.variably_modified(element) => Expr::Cast {
                expr: Box::new(Expr::Binary {
                    op: BinOp::Div,
                    lhs: Box::new(Expr::Unsafe(Box::new(rust::Block {
                        stmts: Vec::new(),
                        tail: Some(Box::new(Expr::MethodCall {
                            recv: Box::new(self.byte_pointer(left)?),
                            method: "offset_from".into(),
                            args: vec![self.byte_pointer(right)?],
                        })),
                    }))),
                    rhs: Box::new(self.runtime_stride(element)?),
                }),
                ty: self.lower_type(&value.ty)?,
            },
            ir::Type::Void => Expr::Cast {
                expr: Box::new(Expr::Unsafe(Box::new(rust::Block {
                    stmts: Vec::new(),
                    tail: Some(Box::new(Expr::MethodCall {
                        recv: Box::new(self.byte_pointer(left)?),
                        method: "offset_from".into(),
                        args: vec![self.byte_pointer(right)?],
                    })),
                }))),
                ty: self.lower_type(&value.ty)?,
            },
            ir::Type::Function { .. } => Expr::Cast {
                expr: Box::new(Expr::MethodCall {
                    recv: Box::new(Expr::Cast {
                        expr: Box::new(self.byte_pointer(left)?),
                        ty: rust::Type::Prim(Prim::Isize),
                    }),
                    method: "wrapping_sub".into(),
                    args: vec![Expr::Cast {
                        expr: Box::new(self.byte_pointer(right)?),
                        ty: rust::Type::Prim(Prim::Isize),
                    }],
                }),
                ty: self.lower_type(&value.ty)?,
            },
            _ => Expr::Cast {
                expr: Box::new(Expr::Unsafe(Box::new(rust::Block {
                    stmts: Vec::new(),
                    tail: Some(Box::new(Expr::MethodCall {
                        recv: Box::new(self.lower_value(left)?),
                        method: "offset_from".into(),
                        args: vec![Expr::Cast {
                            expr: Box::new(self.lower_value(right)?),
                            ty: self.lower_type(&left.ty)?,
                        }],
                    })),
                }))),
                ty: self.lower_type(&value.ty)?,
            },
        })
    }

    pub(super) fn lower_null(&mut self, value: &ir::Value) -> Result<Expr> {
        if matches!(&value.ty, ir::Type::Pointer { pointee, .. } if matches!(**pointee, ir::Type::Function { .. }))
        {
            self.lower_type(&value.ty)?;
            return Ok(Expr::Var("None".into()));
        }
        let rust::Type::Ptr { mutable, inner } = self.lower_type(&value.ty)? else {
            return Err(unsupported_value(value));
        };
        Ok(Expr::Call {
            func: Box::new(Expr::Var(
                format!(
                    "std::ptr::{}::<{}>",
                    if mutable { "null_mut" } else { "null" },
                    crate::backend::codegen::type_to_string(&inner)
                )
                .into(),
            )),
            args: Vec::new(),
            binding: CallBinding::Generated,
        })
    }
}
