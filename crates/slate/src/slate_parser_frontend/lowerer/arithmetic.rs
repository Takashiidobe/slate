use super::*;

pub(super) fn overflow_method(op: ir::ArithOp) -> Option<&'static str> {
    match op {
        ir::ArithOp::Add => Some("add"),
        ir::ArithOp::Sub => Some("sub"),
        ir::ArithOp::Mul => Some("mul"),
        _ => None,
    }
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn lower_overflow(
        &mut self,
        value: &ir::Value,
        op: ir::ArithOp,
        left: &ir::Value,
        right: &ir::Value,
        result: &ir::Place,
    ) -> Result<Expr> {
        let method = overflow_method(op).ok_or_else(|| unsupported_value(value))?;
        for ty in [&left.ty, &right.ty, &result.ty] {
            if !matches!(
                self.tables.resolve_type(ty),
                ir::Type::Numeric(ir::NumericType::Integer { width, .. }) if *width <= 64
            ) {
                return Err(unsupported_value(value));
            }
        }
        let wide = rust::Type::Prim(Prim::I128);
        let target = self.lower_type(&result.ty)?;
        let [lhs, rhs, exact] = [self.next_temp(), self.next_temp(), self.next_temp()];
        let var = |name: &String| Expr::Var(name.as_str().into());
        let bind = |name: &String, init: Expr| Stmt::Let {
            name: name.clone(),
            mutable: false,
            ty: Some(wide.clone()),
            init: Some(init),
        };
        let call = |method: String| Expr::MethodCall {
            recv: Box::new(var(&lhs)),
            method,
            args: vec![var(&rhs)],
        };
        let truncated = Expr::Cast {
            expr: Box::new(var(&exact)),
            ty: target.clone(),
        };
        Ok(Expr::Block(Box::new(rust::Block {
            stmts: vec![
                bind(
                    &lhs,
                    Expr::Cast {
                        expr: Box::new(self.lower_value(left)?),
                        ty: wide.clone(),
                    },
                ),
                bind(
                    &rhs,
                    Expr::Cast {
                        expr: Box::new(self.lower_value(right)?),
                        ty: wide.clone(),
                    },
                ),
                bind(&exact, call(format!("wrapping_{method}"))),
                self.lower_assignment(result, truncated.clone())?,
            ],
            tail: Some(Box::new(Expr::Binary {
                op: BinOp::Or,
                lhs: Box::new(Expr::MethodCall {
                    recv: Box::new(call(format!("checked_{method}"))),
                    method: "is_none".into(),
                    args: Vec::new(),
                }),
                rhs: Box::new(Expr::Binary {
                    op: BinOp::Ne,
                    lhs: Box::new(Expr::Cast {
                        expr: Box::new(truncated),
                        ty: wide.clone(),
                    }),
                    rhs: Box::new(var(&exact)),
                }),
            })),
        })))
    }
}
