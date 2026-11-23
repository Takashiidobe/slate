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

fn null_value(value: &ir::Value) -> bool {
    let mut value = value;
    while let ValueKind::Copy { operand, .. } | ValueKind::Convert { operand, .. } =
        &value.node.value
    {
        value = operand;
    }
    matches!(value.node.value, ValueKind::Null)
        || matches!(value.node.value, ValueKind::Constant(_))
            && matches!(value.ty, ir::Type::Pointer { .. })
}

fn peel_pointer_casts(value: &ir::Value) -> &ir::Value {
    let mut value = value;
    while let ValueKind::Copy { operand, .. }
    | ValueKind::Convert {
        kind: ir::ConversionKind::PointerCast,
        operand,
        ..
    } = &value.node.value
    {
        value = operand;
    }
    value
}

fn diverges(statements: &[slate_parser::ast::Span<ir::Statement>]) -> bool {
    match statements.last().map(|statement| &statement.value) {
        Some(ir::Statement::Return(_) | ir::Statement::Break(_) | ir::Statement::Continue(_)) => {
            true
        }
        Some(ir::Statement::Block(body)) => diverges(body),
        _ => false,
    }
}

fn some_pattern(name: &str) -> rust::Pattern {
    rust::Pattern::TupleStruct {
        name: "Some".into(),
        fields: vec![rust::Pattern::Binding(name.into())],
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
        let pointer = peel_pointer_casts(if null_value(right) {
            left
        } else if null_value(left) {
            right
        } else {
            return None;
        });
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

    fn unwritten(&self, id: BindingId) -> bool {
        self.tables
            .facts
            .binding(id)
            .and_then(|facts| facts.mutability.as_ref())
            .is_some_and(|fact| !fact.required && fact.arguments.is_empty())
    }

    fn presence(&self, condition: &ir::Value) -> Option<(BindingId, Borrow, bool)> {
        match &condition.node.value {
            ValueKind::Copy { operand, .. } => self.presence(operand),
            ValueKind::Unary {
                op: ir::UnaryArithOp::Not,
                operand,
                ..
            } if condition.ty == ir::Type::Bool => self
                .presence(operand)
                .map(|(id, borrow, present)| (id, borrow, !present)),
            ValueKind::Compare {
                op: op @ (ir::CompareOp::Eq | ir::CompareOp::Ne),
                left,
                right,
                ..
            } => {
                let pointer = peel_pointer_casts(if null_value(right) {
                    left
                } else if null_value(left) {
                    right
                } else {
                    return None;
                });
                let (id, borrow) = self.borrowed_binding(pointer)?;
                (borrow.nullable && self.unwritten(id)).then_some((
                    id,
                    borrow,
                    *op == ir::CompareOp::Ne,
                ))
            }
            #[expect(
                slate::sensitive_wildcard,
                reason = "only null comparisons and their negations test presence"
            )]
            _ => None,
        }
    }

    fn unwrapped<T>(
        &mut self,
        id: BindingId,
        lower: impl FnOnce(&mut Self) -> Result<T>,
    ) -> Result<T> {
        self.present.insert(id);
        let lowered = lower(self);
        self.present.remove(&id);
        lowered
    }

    fn present_condition(
        &mut self,
        condition: &ir::Value,
    ) -> Option<(BindingId, Borrow, Option<ir::Value>)> {
        if let ValueKind::Logical {
            op: ir::LogicalOp::And,
            left,
            right,
        } = &condition.node.value
            && let Some((id, borrow, true)) = self.presence(left)
        {
            return Some((id, borrow, Some((**right).clone())));
        }
        match self.presence(condition)? {
            (id, borrow, true) => Some((id, borrow, None)),
            (_, _, false) => None,
        }
    }

    fn let_some(&mut self, id: BindingId, borrow: Borrow) -> (rust::Pattern, Expr) {
        let pattern = some_pattern(self.binding_name(id).as_str());
        (pattern, self.optional_binding(id, borrow))
    }

    pub(super) fn lower_if_let(
        &mut self,
        condition: &ir::Value,
        then_body: &[slate_parser::ast::Span<ir::Statement>],
        else_body: &[slate_parser::ast::Span<ir::Statement>],
    ) -> Result<Option<Stmt>> {
        let Some((id, borrow, rest)) = self.present_condition(condition) else {
            return Ok(None);
        };
        let (pattern, scrutinee) = self.let_some(id, borrow);
        let (rest, then_body) = self.unwrapped(id, |this| {
            let rest = rest
                .as_ref()
                .map(|rest| this.lower_condition(rest))
                .transpose()?;
            Ok((rest, this.lower_statement_list(then_body)?))
        })?;
        let binding = Expr::Let {
            pattern,
            expr: Box::new(scrutinee),
        };
        Ok(Some(Stmt::If {
            cond: match rest {
                Some(rest) => Expr::Binary {
                    op: rust::BinOp::And,
                    lhs: Box::new(binding),
                    rhs: Box::new(rest),
                },
                None => binding,
            },
            then_body,
            else_body: self.lower_statement_list(else_body)?,
        }))
    }

    pub(super) fn lower_if_let_value(
        &mut self,
        condition: &ir::Value,
        then_value: &ir::Value,
        else_value: &ir::Value,
        lower: impl Fn(&mut Self, &ir::Value) -> Result<Expr>,
    ) -> Result<Option<Expr>> {
        let Some((id, borrow, None)) = self.present_condition(condition) else {
            return Ok(None);
        };
        let (pattern, scrutinee) = self.let_some(id, borrow);
        let then_expr = self.unwrapped(id, |this| lower(this, then_value))?;
        Ok(Some(Expr::If {
            cond: Box::new(Expr::Let {
                pattern,
                expr: Box::new(scrutinee),
            }),
            then_expr: Box::new(then_expr),
            else_expr: Box::new(lower(self, else_value)?),
        }))
    }

    pub(super) fn lower_let_else(
        &mut self,
        condition: &ir::Value,
        then_body: &[slate_parser::ast::Span<ir::Statement>],
    ) -> Result<Option<Stmt>> {
        let Some((id, borrow, false)) = self.presence(condition) else {
            return Ok(None);
        };
        if !diverges(then_body) {
            return Ok(None);
        }
        let else_body = self.lower_statement_list(then_body)?;
        let (pattern, init) = self.let_some(id, borrow);
        self.present.insert(id);
        Ok(Some(Stmt::LetElse {
            pattern,
            init,
            else_body,
        }))
    }
}
