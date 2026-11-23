use super::*;
use slate_analysis::Borrow;

fn method(recv: Expr, method: &str, args: Vec<Expr>) -> Expr {
    Expr::MethodCall {
        recv: Box::new(recv),
        method: method.into(),
        args,
        unsafe_: rust::Unsafe(false),
    }
}

fn closure(name: &str, body: Expr) -> Expr {
    Expr::Closure {
        params: vec![name.into()],
        body: Box::new(body),
    }
}

pub(super) fn unwrap(expr: Expr) -> Expr {
    method(expr, "unwrap", Vec::new())
}

pub(super) fn present(borrow: Borrow) -> Borrow {
    Borrow {
        nullable: false,
        ..borrow
    }
}

fn null_pointer(raw: &rust::Type) -> Expr {
    let rust::Type::Ptr { mutable, inner } = raw else {
        return Expr::Value(rust::RustValue::NullPtr);
    };
    Expr::Call {
        func: Box::new(Expr::GenericPath {
            path: rust::Path::new(
                ["core", "ptr", if *mutable { "null_mut" } else { "null" }].map(rust::Ident::from),
            ),
            type_args: vec![(**inner).clone()],
        }),
        args: Vec::new(),
        binding: CallBinding::Generated,
        unsafe_: rust::Unsafe(false),
    }
}

pub(super) fn raw_view(binding: Expr, borrow: Borrow, raw: rust::Type) -> Expr {
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
        return Expr::Cast {
            expr: Box::new(binding),
            ty: raw,
        };
    }
    let pointer = method(
        binding,
        if borrow.mutable {
            "as_mut_ptr"
        } else {
            "as_ptr"
        },
        Vec::new(),
    );
    match raw {
        rust::Type::Ptr { mutable, .. } if mutable != borrow.mutable => Expr::Cast {
            expr: Box::new(pointer),
            ty: raw,
        },
        _ => pointer,
    }
}

pub(super) fn convert_present(expr: Expr, source: Borrow, target: Borrow) -> Expr {
    match (source.many, target.many) {
        (false, false) | (true, true) => expr,
        (true, false) => borrows::first_element(target.mutable, expr),
        (false, true) if source.mutable => {
            borrows::one_element_slice(target.mutable, borrows::reborrow(target.mutable, expr))
        }
        (false, true) => borrows::one_element_slice(false, expr),
    }
}

impl FunctionLowerer<'_, '_> {
    fn optional_binding(&mut self, id: BindingId, borrow: Borrow) -> Expr {
        let binding = Expr::var(self.binding_name(id).as_str().into());
        if borrow.mutable {
            self.reassigned_borrows.insert(id);
            method(binding, "as_deref_mut", Vec::new())
        } else {
            binding
        }
    }

    pub(super) fn borrowed_var(&mut self, id: BindingId, borrow: Borrow) -> Expr {
        if !borrow.nullable {
            return Expr::var(self.binding_name(id).as_str().into());
        }
        unwrap(self.optional_binding(id, borrow))
    }

    pub(super) fn optional_read(&mut self, id: BindingId, borrow: Borrow, raw: rust::Type) -> Expr {
        let name = self.binding_name(id);
        let binding = self.optional_binding(id, borrow);
        let view = raw_view(
            Expr::var(name.as_str().into()),
            present(borrow),
            raw.clone(),
        );
        method(
            binding,
            "map_or",
            vec![null_pointer(&raw), closure(name.as_str(), view)],
        )
    }

    pub(super) fn lifted_return(&self, value: &ir::Value) -> Option<Borrow> {
        let ValueKind::Call {
            callee: ir::Callee::Direct(id),
            ..
        } = &value.node.value
        else {
            return None;
        };
        self.tables.return_borrow(*id)
    }

    pub(super) fn lower_lifted_call(&mut self, value: &ir::Value) -> Result<Expr> {
        let outer = self.lifted_call.replace(value.node.id);
        let lowered = self.lower_value(value);
        self.lifted_call = outer;
        lowered
    }

    pub(super) fn lower_lifted_call_raw(&mut self, value: &ir::Value) -> Result<Option<Expr>> {
        if self.lifted_call == Some(value.node.id) {
            return Ok(None);
        }
        let Some(borrow) = self.lifted_return(value) else {
            return Ok(None);
        };
        let call = self.lower_lifted_call(value)?;
        let raw = self.lower_type(&value.ty)?;
        if !borrow.nullable {
            return Ok(Some(raw_view(call, borrow, raw)));
        }
        let view = raw_view(Expr::var("r".into()), present(borrow), raw.clone());
        Ok(Some(method(
            call,
            "map_or",
            vec![null_pointer(&raw), closure("r", view)],
        )))
    }

    fn map_present(expr: Expr, source: Borrow, target: Borrow) -> Expr {
        if source.many == target.many && (source.mutable == target.mutable) {
            return expr;
        }
        let converted = match convert_present(Expr::var("r".into()), source, target) {
            converted if source.many == target.many => borrows::reborrow(false, converted),
            converted => converted,
        };
        method(expr, "map", vec![closure("r", converted)])
    }

    pub(super) fn lower_optional(&mut self, value: &ir::Value, target: Borrow) -> Result<Expr> {
        match &value.node.value {
            ValueKind::Copy { operand, .. } => return self.lower_optional(operand, target),
            ValueKind::Convert {
                kind: ir::ConversionKind::PointerCast,
                operand,
                ..
            } if borrows::same_pointee(&operand.ty, &value.ty) => {
                return self.lower_optional(operand, target);
            }
            ValueKind::Null => return Ok(Expr::Value(rust::RustValue::None)),
            ValueKind::Constant(_) if matches!(value.ty, ir::Type::Pointer { .. }) => {
                return Ok(Expr::Value(rust::RustValue::None));
            }
            ValueKind::Conditional {
                condition,
                then_value,
                else_value,
            } => {
                return Ok(Expr::If {
                    cond: Box::new(self.lower_condition(condition)?),
                    then_expr: Box::new(self.lower_optional(then_value, target)?),
                    else_expr: Box::new(self.lower_optional(else_value, target)?),
                });
            }
            #[expect(
                slate::sensitive_wildcard,
                reason = "every other value is an optional binding, a lifted call, or a present borrow"
            )]
            _ => {}
        }
        if let Some((id, source)) = self.borrowed_binding(value)
            && source.nullable
            && (source.mutable || !target.mutable)
        {
            let binding = self.optional_binding(id, source);
            return Ok(Self::map_present(binding, source, target));
        }
        if let Some(source) = self.lifted_return(value)
            && source.nullable
            && (source.mutable || !target.mutable)
        {
            let call = self.lower_lifted_call(value)?;
            return Ok(Self::map_present(call, source, target));
        }
        Ok(Expr::Call {
            func: Box::new(Expr::var("Some".into())),
            args: vec![self.lower_borrow(value, present(target))?],
            binding: CallBinding::Generated,
            unsafe_: rust::Unsafe(false),
        })
    }

    pub(super) fn lower_returned(&mut self, value: &ir::Value) -> Result<Expr> {
        match self.returns {
            Some(borrow) => self.lower_borrow(value, borrow),
            None => self.lower_value(value),
        }
    }

    pub(super) fn lower_element_borrow(
        &mut self,
        id: BindingId,
        source: Borrow,
        index: Option<ir::Value>,
        target: Borrow,
    ) -> Result<Expr> {
        let base = self.borrowed_var(id, source);
        let Some(amount) = index else {
            return Ok(convert_present(base, source, target));
        };
        let index = self.lower_index(&amount)?;
        Ok(if target.many {
            Expr::TupleField {
                base: Box::new(method(
                    base,
                    if target.mutable {
                        "split_at_mut"
                    } else {
                        "split_at"
                    },
                    vec![index],
                )),
                index: 1,
            }
        } else {
            Expr::Ref {
                mutable: target.mutable,
                expr: Box::new(Expr::Index {
                    base: Box::new(base),
                    index: Box::new(index),
                }),
            }
        })
    }

    pub(super) fn null_check(
        &self,
        equal: bool,
        left: &ir::Value,
        right: &ir::Value,
    ) -> Option<Expr> {
        let null = |value: &ir::Value| {
            let mut value = value;
            while let ValueKind::Copy { operand, .. } | ValueKind::Convert { operand, .. } =
                &value.node.value
            {
                value = operand;
            }
            matches!(value.node.value, ValueKind::Null)
                || matches!(value.node.value, ValueKind::Constant(_))
                    && matches!(value.ty, ir::Type::Pointer { .. })
        };
        let pointer = if null(right) {
            left
        } else if null(left) {
            right
        } else {
            return None;
        };
        let mut pointer = pointer;
        while let ValueKind::Copy { operand, .. }
        | ValueKind::Convert {
            kind: ir::ConversionKind::PointerCast,
            operand,
            ..
        } = &pointer.node.value
        {
            pointer = operand;
        }
        let never = Some(Expr::Value(rust::RustValue::Bool(!equal)));
        if self.owned_read(pointer).is_some() {
            return never;
        }
        let (id, borrow) = self.borrowed_binding(pointer)?;
        if !borrow.nullable {
            return never;
        }
        Some(method(
            Expr::var(self.binding_name(id).as_str().into()),
            if equal { "is_none" } else { "is_some" },
            Vec::new(),
        ))
    }
}
