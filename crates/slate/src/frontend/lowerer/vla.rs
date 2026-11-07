use super::*;

impl Tables<'_> {
    pub(super) fn variably_modified(&self, ty: &ir::Type) -> bool {
        match self.resolve_type(ty) {
            ir::Type::VariableArray { .. } => true,
            ir::Type::Array { element, .. } => self.variably_modified(element),
            ir::Type::Defined(_) => self.variable_record(ty).is_some(),
            _ => false,
        }
    }

    pub(super) fn variable_record(&self, ty: &ir::Type) -> Option<(&ir::RecordLayout, &ir::Type)> {
        let ir::Type::Defined(id) = self.resolve_type(ty) else {
            return None;
        };
        let ir::TypeDefinitionKind::Record {
            fields: Some(fields),
            layout: Some(layout),
            ..
        } = &self.types.get(id)?.kind
        else {
            return None;
        };
        let tail = &fields.last()?.ty;
        self.variably_modified(tail).then_some((layout, tail))
    }
}

fn usize_value(value: u64) -> Expr {
    Expr::Value(rust::RustValue::Usize(value as usize))
}

fn as_usize(expr: Expr) -> Expr {
    Expr::Cast {
        expr: Box::new(expr),
        ty: rust::Type::Prim(Prim::Usize),
    }
}

fn product(factors: Vec<Expr>) -> Expr {
    factors
        .into_iter()
        .reduce(|lhs, rhs| Expr::Binary {
            op: BinOp::Mul,
            lhs: Box::new(lhs),
            rhs: Box::new(rhs),
        })
        .unwrap_or_else(|| usize_value(1))
}

impl FunctionLowerer<'_, '_> {
    fn vla_layout(&mut self, ty: &ir::Type, factors: &mut Vec<Expr>) -> Result<rust::Type> {
        let tables = self.tables;
        match tables.resolve_type(ty) {
            ir::Type::VariableArray {
                element,
                extent: ir::VariableExtent::Captured(id),
            } => {
                factors.push(as_usize(self.lower_binding(*id)));
                self.vla_layout(element, factors)
            }
            ir::Type::VariableArray { .. } | ir::Type::Defined(_)
                if tables.variably_modified(ty) =>
            {
                Err(unsupported_type(ty))
            }
            ir::Type::Array {
                element,
                length: Some(length),
            } if tables.variably_modified(element) => {
                factors.push(usize_value(*length));
                self.vla_layout(element, factors)
            }
            resolved => self.lower_type(resolved),
        }
    }

    pub(super) fn runtime_stride(&mut self, ty: &ir::Type) -> Result<Expr> {
        let mut factors = Vec::new();
        let element = self.vla_layout(ty, &mut factors)?;
        factors.push(Expr::Call {
            func: Box::new(Expr::Var(
                format!(
                    "std::mem::size_of::<{}>",
                    crate::backend::codegen::type_to_string(&element)
                )
                .into(),
            )),
            args: Vec::new(),
            binding: CallBinding::Generated,
        });
        Ok(Expr::Cast {
            expr: Box::new(product(factors)),
            ty: rust::Type::Prim(Prim::Isize),
        })
    }

    pub(super) fn lower_vla_let(&mut self, variable: &ir::Variable) -> Result<Stmt> {
        let (storage, buffer) = self.vla_buffer(variable)?;
        Ok(Stmt::Let {
            name: self.binding_name(variable.id),
            mutable: true,
            ty: Some(storage),
            init: Some(buffer),
        })
    }

    fn record_buffer_layout(&mut self, ty: &ir::Type) -> Result<Option<(rust::Type, Expr)>> {
        let tables = self.tables;
        let Some((layout, tail)) = tables.variable_record(ty) else {
            return Ok(None);
        };
        let unit = match layout.align {
            1 => Prim::U8,
            2 => Prim::U16,
            4 => Prim::U32,
            8 => Prim::U64,
            16 => Prim::U128,
            _ => return Err(unsupported_type(ty)),
        };
        let offset = layout.offsets.last().copied().unwrap_or(0);
        let tail_bytes = as_usize(self.runtime_stride(tail)?);
        let bytes = Expr::Binary {
            op: BinOp::Add,
            lhs: Box::new(usize_value(offset + layout.align - 1)),
            rhs: Box::new(tail_bytes),
        };
        let count = Expr::Binary {
            op: BinOp::Div,
            lhs: Box::new(bytes),
            rhs: Box::new(usize_value(layout.align)),
        };
        Ok(Some((rust::Type::Prim(unit), count)))
    }

    pub(super) fn vla_vec_type(&mut self, ty: &ir::Type) -> Result<rust::Type> {
        if let Some((unit, _)) = self.record_buffer_layout(ty)? {
            return Ok(rust::Type::Generic {
                name: "Vec".into(),
                args: vec![unit],
            });
        }
        let element = self.vla_layout(ty, &mut Vec::new())?;
        Ok(rust::Type::Generic {
            name: "Vec".into(),
            args: vec![element],
        })
    }

    pub(super) fn vla_buffer(&mut self, variable: &ir::Variable) -> Result<(rust::Type, Expr)> {
        if matches!(variable.auto_init, Some(ir::AutoInit::Pattern(_))) {
            return Err(unsupported_type(&variable.ty));
        }
        if let Some(initializer) = &variable.initializer
            && !matches!(
                &initializer.node.value,
                ValueKind::Aggregate { members, zero_fill: true } if members.is_empty()
            )
        {
            return Err(unsupported_value(initializer));
        }
        let (element, count) = match self.record_buffer_layout(&variable.ty)? {
            Some(layout) => layout,
            None => {
                let mut factors = Vec::new();
                let element = self.vla_layout(&variable.ty, &mut factors)?;
                (element, product(factors))
            }
        };
        let storage = rust::Type::Generic {
            name: "Vec".into(),
            args: vec![element],
        };
        let buffer = "__slate_vla";
        Ok((
            storage.clone(),
            Expr::Block(Box::new(rust::Block {
                stmts: vec![
                    Stmt::Let {
                        name: buffer.into(),
                        mutable: true,
                        ty: Some(storage),
                        init: Some(Expr::Call {
                            func: Box::new(Expr::Var("Vec::with_capacity".into())),
                            args: vec![count.clone()],
                            binding: CallBinding::Generated,
                        }),
                    },
                    Stmt::Unsafe {
                        body: rust::Block {
                            stmts: vec![Stmt::Expr(Expr::MethodCall {
                                recv: Box::new(Expr::MethodCall {
                                    recv: Box::new(Expr::Var(buffer.into())),
                                    method: "as_mut_ptr".into(),
                                    args: Vec::new(),
                                }),
                                method: "write_bytes".into(),
                                args: vec![
                                    Expr::Value(rust::RustValue::TypedUInt(0, Prim::U8)),
                                    count,
                                ],
                            })],
                            tail: None,
                        },
                    },
                ],
                tail: Some(Box::new(Expr::Var(buffer.into()))),
            })),
        ))
    }

    pub(super) fn vla_address(&mut self, place: &ir::Place) -> Result<Option<Expr>> {
        if !self.tables.variably_modified(&place.ty) {
            return Ok(None);
        }
        Ok(match place.kind {
            PlaceKind::Binding(id) if let Some(pointer) = self.shared_pointers.get(&id) => {
                Some(Expr::Var(pointer.as_str().into()))
            }
            PlaceKind::Binding(id) => Some(Expr::Cast {
                expr: Box::new(Expr::MethodCall {
                    recv: Box::new(self.lower_binding(id)),
                    method: "as_mut_ptr".into(),
                    args: Vec::new(),
                }),
                ty: rust::Type::Ptr {
                    mutable: true,
                    inner: Box::new(self.lower_type(&place.ty)?),
                },
            }),
            PlaceKind::Deref(ref pointer) => Some(self.lower_value(pointer)?),
            PlaceKind::Field { bits: None, .. } => Some(Expr::Cast {
                expr: Box::new(Expr::Unary {
                    op: rust::UnaryOp::Raw(rust::Raw::Mut),
                    expr: Box::new(self.lower_place(place)?),
                }),
                ty: rust::Type::Ptr {
                    mutable: true,
                    inner: Box::new(self.lower_type(&place.ty)?),
                },
            }),
            _ => None,
        })
    }
}
