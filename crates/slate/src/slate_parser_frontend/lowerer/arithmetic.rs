use super::*;

pub(super) fn overflow_method(op: ir::ArithOp) -> Result<&'static str> {
    Ok(match op {
        ir::ArithOp::Add => "add",
        ir::ArithOp::Sub => "sub",
        ir::ArithOp::Mul => "mul",
        _ => return Err(super::Error::Unsupported(format!("overflow {op}"))),
    })
}

pub(super) fn lower_overflow(
    cx: &Context,
    op: ir::ArithOp,
    left: &ir::Value,
    right: &ir::Value,
    result: &ir::Place,
) -> Result<Expr> {
    let method = overflow_method(op)?;
    for ty in [&left.ty, &right.ty, &result.ty] {
        if !matches!(
            resolve_type(cx, ty),
            ir::Type::Numeric(ir::NumericType::Integer { width, .. }) if *width <= 64
        ) {
            return Err(super::Error::Unsupported(format!("overflow operand {ty}")));
        }
    }
    let wide = rust::Type::Prim(Prim::I128);
    let target = lower_type(cx, &result.ty)?;
    let [lhs, rhs, exact] = [cx.next_temp(), cx.next_temp(), cx.next_temp()];
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
                    expr: Box::new(lower_value(left, cx)?),
                    ty: wide.clone(),
                },
            ),
            bind(
                &rhs,
                Expr::Cast {
                    expr: Box::new(lower_value(right, cx)?),
                    ty: wide.clone(),
                },
            ),
            bind(&exact, call(format!("wrapping_{method}"))),
            lower_assignment(cx, result, truncated.clone())?,
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
