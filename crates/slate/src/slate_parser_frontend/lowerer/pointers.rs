use super::*;

pub(super) fn byte_pointer_type() -> rust::Type {
    rust::Type::Ptr {
        mutable: false,
        inner: Box::new(rust::Type::Prim(Prim::U8)),
    }
}

pub(super) fn byte_pointer(pointer: &ir::Value, cx: &Context) -> Result<Expr> {
    let expr = Box::new(lower_value(pointer, cx)?);
    Ok(match lower_type(cx, &pointer.ty)? {
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
    value: &ir::Value,
    pointer: &ir::Value,
    amount: &ir::Value,
    subtract: bool,
    element: &ir::Type,
    cx: &Context,
) -> Result<Expr> {
    let offset = Expr::Cast {
        expr: Box::new(lower_value(amount, cx)?),
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
        ir::Type::Void => Expr::Cast {
            expr: Box::new(Expr::Unsafe(Box::new(rust::Block {
                stmts: Vec::new(),
                tail: Some(Box::new(Expr::MethodCall {
                    recv: Box::new(byte_pointer(pointer, cx)?),
                    method: "offset".into(),
                    args: vec![offset],
                })),
            }))),
            ty: lower_type(cx, &value.ty)?,
        },
        ir::Type::Function { .. } => Expr::Transmute {
            from: byte_pointer_type(),
            to: lower_type(cx, &value.ty)?,
            expr: Box::new(Expr::MethodCall {
                recv: Box::new(byte_pointer(pointer, cx)?),
                method: "wrapping_offset".into(),
                args: vec![offset],
            }),
        },
        _ => Expr::Unsafe(Box::new(rust::Block {
            stmts: Vec::new(),
            tail: Some(Box::new(Expr::MethodCall {
                recv: Box::new(lower_value(pointer, cx)?),
                method: "offset".into(),
                args: vec![offset],
            })),
        })),
    })
}

pub(super) fn lower_pointer_difference(
    value: &ir::Value,
    left: &ir::Value,
    right: &ir::Value,
    element: &ir::Type,
    cx: &Context,
) -> Result<Expr> {
    Ok(match element {
        ir::Type::Void => Expr::Cast {
            expr: Box::new(Expr::Unsafe(Box::new(rust::Block {
                stmts: Vec::new(),
                tail: Some(Box::new(Expr::MethodCall {
                    recv: Box::new(byte_pointer(left, cx)?),
                    method: "offset_from".into(),
                    args: vec![byte_pointer(right, cx)?],
                })),
            }))),
            ty: lower_type(cx, &value.ty)?,
        },
        ir::Type::Function { .. } => Expr::Cast {
            expr: Box::new(Expr::MethodCall {
                recv: Box::new(Expr::Cast {
                    expr: Box::new(byte_pointer(left, cx)?),
                    ty: rust::Type::Prim(Prim::Isize),
                }),
                method: "wrapping_sub".into(),
                args: vec![Expr::Cast {
                    expr: Box::new(byte_pointer(right, cx)?),
                    ty: rust::Type::Prim(Prim::Isize),
                }],
            }),
            ty: lower_type(cx, &value.ty)?,
        },
        _ => Expr::Cast {
            expr: Box::new(Expr::Unsafe(Box::new(rust::Block {
                stmts: Vec::new(),
                tail: Some(Box::new(Expr::MethodCall {
                    recv: Box::new(lower_value(left, cx)?),
                    method: "offset_from".into(),
                    args: vec![Expr::Cast {
                        expr: Box::new(lower_value(right, cx)?),
                        ty: lower_type(cx, &left.ty)?,
                    }],
                })),
            }))),
            ty: lower_type(cx, &value.ty)?,
        },
    })
}

pub(super) fn lower_null(value: &ir::Value, cx: &Context) -> Result<Expr> {
    if matches!(&value.ty, ir::Type::Pointer { pointee, .. } if matches!(**pointee, ir::Type::Function { .. }))
    {
        lower_type(cx, &value.ty)?;
        return Ok(Expr::Var("None".into()));
    }
    let rust::Type::Ptr { mutable, inner } = lower_type(cx, &value.ty)? else {
        return Err(super::Error::Unsupported(format!(
            "value {}",
            value.display(false)
        )));
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
