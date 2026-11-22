use super::*;
use crate::function_identity::Known;

fn peel_pointer(mut value: &ir::Value) -> &ir::Value {
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

fn usize_of(expr: Expr) -> Expr {
    Expr::Cast {
        expr: Box::new(expr),
        ty: rust::Type::Prim(Prim::Usize),
    }
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn owned(&self, binding: BindingId) -> bool {
        !self.unborrowed.contains(&binding)
            && !self.switch_lets.contains(&binding)
            && self.tables.owner_of(binding).is_some()
    }

    pub(super) fn owned_read(&self, value: &ir::Value) -> Option<BindingId> {
        let mut value = value;
        while let ValueKind::Copy { operand, .. } | ValueKind::Convert { operand, .. } =
            &value.node.value
        {
            value = operand;
        }
        let ValueKind::Read {
            place:
                ir::Place {
                    kind: PlaceKind::Binding(id),
                    ..
                },
            ordering: None,
        } = &value.node.value
        else {
            return None;
        };
        self.owned(*id).then_some(*id)
    }

    fn allocator_call<'v>(&self, value: &'v ir::Value) -> Option<(Known, &'v [ir::Value])> {
        let ValueKind::Call {
            callee, arguments, ..
        } = &peel_pointer(value).node.value
        else {
            return None;
        };
        Some((self.known_callee(callee)?, arguments.as_slice()))
    }

    pub(super) fn lower_owned_let(&mut self, variable: &ir::Variable) -> Result<Option<Stmt>> {
        if !self.owned(variable.id) {
            return Ok(None);
        }
        let Some(initializer) = &variable.initializer else {
            return Ok(None);
        };
        let size = match self.allocator_call(initializer) {
            Some((Known::Malloc, [size])) => usize_of(self.lower_value(size)?),
            Some((Known::Calloc, [count, size])) => Expr::Binary {
                op: BinOp::Mul,
                lhs: Box::new(usize_of(self.lower_value(count)?)),
                rhs: Box::new(usize_of(self.lower_value(size)?)),
            },
            _ => return Err(Invariant::UnprovenBorrow(initializer.to_string()).into()),
        };
        let ir::Type::Pointer { pointee, .. } = self.tables.resolve_type(&variable.ty) else {
            return Err(Invariant::UnprovenBorrow(variable.ty.to_string()).into());
        };
        let element = self.lower_type(pointee)?;
        let mutable = self
            .borrow_of(variable.id)
            .is_some_and(|borrow| borrow.mutable);
        Ok(Some(Stmt::Let {
            origin: rust::LocalOrigin::Source,
            name: self.binding_name(variable.id),
            mutable,
            ty: Some(rust::Type::Generic {
                name: "Vec".into(),
                args: vec![element],
            }),
            init: Some(Expr::VecRepeat {
                elem: Box::new(Expr::Value(rust::RustValue::I64(0))),
                len: Box::new(size),
            }),
        }))
    }

    pub(super) fn lower_reallocation(
        &mut self,
        binding: BindingId,
        value: &ir::Value,
    ) -> Result<Option<Stmt>> {
        if !self.owned(binding) {
            return Ok(None);
        }
        let Some((Known::Realloc, [pointer, size])) = self.allocator_call(value) else {
            return Err(Invariant::UnprovenBorrow(value.to_string()).into());
        };
        if self.owned_read(pointer) != Some(binding) {
            return Err(Invariant::UnprovenBorrow(value.to_string()).into());
        }
        Ok(Some(Stmt::Expr(Expr::MethodCall {
            recv: Box::new(Expr::var(self.binding_name(binding).as_str().into())),
            method: "resize".into(),
            args: vec![
                usize_of(self.lower_value(size)?),
                Expr::Value(rust::RustValue::I64(0)),
            ],
            unsafe_: rust::Unsafe(false),
        })))
    }

    pub(super) fn lower_release(&self, arguments: &[ir::Value]) -> Option<Expr> {
        let [pointer] = arguments else {
            return None;
        };
        let binding = self.owned_read(pointer)?;
        Some(Expr::Call {
            func: Box::new(Expr::var("drop".into())),
            args: vec![Expr::var(self.binding_name(binding).as_str().into())],
            binding: CallBinding::Generated,
            unsafe_: rust::Unsafe(false),
        })
    }

    pub(super) fn owned_null_compare(&self, left: &ir::Value, right: &ir::Value) -> bool {
        let null = |value: &ir::Value| {
            matches!(
                peel_pointer(value).node.value,
                ValueKind::Null | ValueKind::Constant(_)
            )
        };
        (null(right) && self.owned_read(left).is_some())
            || (null(left) && self.owned_read(right).is_some())
    }
}
