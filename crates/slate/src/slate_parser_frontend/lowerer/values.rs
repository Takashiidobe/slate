use super::*;

pub(super) fn lower_evaluation_statements(
    evaluation: &ir::Evaluation,
    cx: &Context,
) -> Result<Vec<Stmt>> {
    let mut statements = lower_statement_list(&evaluation.statements, cx)?;
    if !matches!(evaluation.value.node.value, ValueKind::Void) {
        statements.push(Stmt::Expr(lower_value(&evaluation.value, cx)?));
    }
    Ok(statements)
}

pub(super) fn lower_evaluation(
    evaluation: &ir::Evaluation,
    cx: &Context,
) -> Result<(Vec<Stmt>, Expr)> {
    let statements = lower_statement_list(&evaluation.statements, cx)?;
    let condition = lower_condition(&evaluation.value, cx)?;
    Ok((statements, condition))
}

pub(super) fn lower_condition(value: &ir::Value, cx: &Context) -> Result<Expr> {
    let condition = lower_value(value, cx)?;
    if matches!(value.ty, ir::Type::Bool) {
        Ok(condition)
    } else if is_long_double(cx, &value.ty) {
        Ok(Expr::Binary {
            op: BinOp::Ne,
            lhs: Box::new(condition),
            rhs: Box::new(long_double_literal(0)),
        })
    } else {
        Ok(Expr::Binary {
            op: BinOp::Ne,
            lhs: Box::new(condition),
            rhs: Box::new(lower_number(cx, &Number::Integer(0u32.into()), &value.ty)?),
        })
    }
}

pub(super) fn lower_value(value: &ir::Value, cx: &Context) -> Result<Expr> {
    Ok(match &value.node.value {
        ValueKind::Constant(number) => lower_number(cx, number, &value.ty)?,
        ValueKind::CodeUnits(units) => {
            let ir::Type::Array { element, .. } = &value.ty else {
                return Err(super::Error::Unsupported(format!(
                    "code units of {}",
                    value.ty
                )));
            };
            let signed_width = match **element {
                ir::Type::Numeric(ir::NumericType::Integer {
                    width,
                    signed: true,
                    ..
                }) => Some(128 - width),
                _ => None,
            };
            let element = lower_type(cx, element)?;
            Expr::ArrayLit(
                units
                    .iter()
                    .map(|unit| Expr::Cast {
                        expr: Box::new(Expr::Value(match signed_width {
                            Some(shift) => {
                                rust::RustValue::I128((i128::from(*unit) << shift) >> shift)
                            }
                            None => rust::RustValue::U128(u128::from(*unit)),
                        })),
                        ty: element.clone(),
                    })
                    .collect(),
            )
        }
        ValueKind::VaArg { list } if is_long_double(cx, &value.ty) => {
            Expr::Unsafe(Box::new(rust::Block {
                stmts: Vec::new(),
                tail: Some(Box::new(Expr::Call {
                    func: Box::new(Expr::Var("__slate_f80_va_arg".into())),
                    args: vec![Expr::AddrOf {
                        mutable: true,
                        expr: Box::new(lower_place(list, cx)?),
                    }],
                    binding: CallBinding::Generated,
                })),
            }))
        }
        ValueKind::VaArg { list } => Expr::Unsafe(Box::new(rust::Block {
            stmts: Vec::new(),
            tail: Some(Box::new(Expr::MethodCallGeneric {
                recv: Box::new(lower_place(list, cx)?),
                method: "next_arg".into(),
                type_args: vec![lower_type(cx, &value.ty)?],
                args: Vec::new(),
            })),
        })),
        ValueKind::Read {
            place,
            ordering: None,
        } => {
            let lowered = lower_place(place, cx)?;
            let lowered = if matches!(value.ty, ir::Type::VaList) {
                clone_va_list(lowered)
            } else {
                lowered
            };
            if place_is_unsafe(cx, place) {
                Expr::Unsafe(Box::new(rust::Block {
                    stmts: Vec::new(),
                    tail: Some(Box::new(lowered)),
                }))
            } else {
                lowered
            }
        }
        ValueKind::Copy { operand, .. } => lower_value(operand, cx)?,
        ValueKind::AddressOf(place) => match place.kind {
            PlaceKind::Deref(ref pointer) => lower_value(pointer, cx)?,
            _ => {
                let address = Expr::AddrOf {
                    mutable: true,
                    expr: Box::new(lower_place(place, cx)?),
                };
                if place_is_unsafe(cx, place) {
                    Expr::Unsafe(Box::new(rust::Block {
                        stmts: Vec::new(),
                        tail: Some(Box::new(address)),
                    }))
                } else {
                    address
                }
            }
        },
        ValueKind::Convert { operand, .. }
            if is_long_double(cx, &operand.ty) || is_long_double(cx, &value.ty) =>
        {
            lower_long_double_conversion(cx, operand, &value.ty)?
        }
        ValueKind::Convert { operand, .. } => Expr::Cast {
            expr: Box::new(lower_value(operand, cx)?),
            ty: lower_type(cx, &value.ty)?,
        },
        ValueKind::ArrayDecay { place, .. } => {
            let bytes = match place.kind {
                PlaceKind::Binding(id) => cx.strings.get(&id),
                _ => None,
            };
            let Some(bytes) = bytes else {
                let array = lower_place(place, cx)?;
                let decayed = Expr::Cast {
                    expr: Box::new(if place_is_static(cx, place) {
                        Expr::AddrOf {
                            mutable: true,
                            expr: Box::new(array),
                        }
                    } else {
                        Expr::MethodCall {
                            recv: Box::new(array),
                            method: if matches!(value.ty, ir::Type::Pointer { is_const: true, .. })
                            {
                                "as_ptr"
                            } else {
                                "as_mut_ptr"
                            }
                            .into(),
                            args: Vec::new(),
                        }
                    }),
                    ty: lower_type(cx, &value.ty)?,
                };
                return Ok(if place_is_unsafe(cx, place) {
                    Expr::Unsafe(Box::new(rust::Block {
                        stmts: Vec::new(),
                        tail: Some(Box::new(decayed)),
                    }))
                } else {
                    decayed
                });
            };
            Expr::Cast {
                expr: Box::new(Expr::MethodCall {
                    recv: Box::new(Expr::ByteStr(bytes.clone())),
                    method: "as_ptr".into(),
                    args: Vec::new(),
                }),
                ty: lower_type(cx, &value.ty)?,
            }
        }
        ValueKind::Arith {
            op: op @ (ir::ArithOp::Add | ir::ArithOp::Sub | ir::ArithOp::Mul),
            left,
            right,
            semantics:
                ir::ArithSema::Integer {
                    overflow: ir::Overflow::Wrap,
                },
        } => Expr::MethodCall {
            recv: Box::new(lower_value(left, cx)?),
            method: format!("wrapping_{}", overflow_method(*op)?),
            args: vec![lower_value(right, cx)?],
        },
        ValueKind::Overflow {
            op,
            left,
            right,
            result,
        } => lower_overflow(cx, *op, left, right, result)?,
        ValueKind::Arith {
            op, left, right, ..
        } => Expr::Binary {
            op: match op {
                ir::ArithOp::Add => BinOp::Add,
                ir::ArithOp::Sub => BinOp::Sub,
                ir::ArithOp::Mul => BinOp::Mul,
                ir::ArithOp::Div => BinOp::Div,
                ir::ArithOp::Rem => BinOp::Rem,
                ir::ArithOp::And => BinOp::BitAnd,
                ir::ArithOp::Or => BinOp::BitOr,
                ir::ArithOp::Xor => BinOp::BitXor,
                ir::ArithOp::Shl => BinOp::Shl,
                ir::ArithOp::Shr => BinOp::Shr,
                _ => return Err(super::Error::Unsupported(format!("arithmetic {op}"))),
            },
            lhs: Box::new(lower_value(left, cx)?),
            rhs: Box::new(lower_value(right, cx)?),
        },
        ValueKind::Unary {
            op: ir::UnaryArithOp::Neg,
            operand,
            ..
        } if matches!(
            value.ty,
            ir::Type::Numeric(ir::NumericType::Float(
                ir::FloatType::F32 | ir::FloatType::F64 | ir::FloatType::F80
            ))
        ) =>
        {
            Expr::Unary {
                op: rust::UnaryOp::Neg,
                expr: Box::new(lower_value(operand, cx)?),
            }
        }
        ValueKind::Unary {
            op,
            operand,
            semantics,
        } if matches!(
            value.ty,
            ir::Type::Bool | ir::Type::Numeric(ir::NumericType::Integer { .. })
        ) =>
        {
            let operand = lower_value(operand, cx)?;
            match (op, semantics) {
                (ir::UnaryArithOp::Not, _) => Expr::Unary {
                    op: rust::UnaryOp::Not,
                    expr: Box::new(operand),
                },
                (
                    ir::UnaryArithOp::Neg,
                    ir::ArithSema::Integer {
                        overflow: ir::Overflow::Undefined,
                    },
                ) => Expr::Unary {
                    op: rust::UnaryOp::Neg,
                    expr: Box::new(operand),
                },
                (
                    ir::UnaryArithOp::Neg,
                    ir::ArithSema::Integer {
                        overflow: ir::Overflow::Wrap,
                    },
                ) => Expr::MethodCall {
                    recv: Box::new(operand),
                    method: "wrapping_neg".into(),
                    args: Vec::new(),
                },
                _ => {
                    return Err(super::Error::Unsupported(format!(
                        "value {}",
                        value.display(false)
                    )));
                }
            }
        }
        ValueKind::Logical { op, left, right } if matches!(value.ty, ir::Type::Bool) => {
            Expr::Binary {
                op: match op {
                    ir::LogicalOp::And => BinOp::And,
                    ir::LogicalOp::Or => BinOp::Or,
                },
                lhs: Box::new(lower_condition(left, cx)?),
                rhs: Box::new(lower_condition(right, cx)?),
            }
        }
        ValueKind::Conditional {
            condition,
            then_value,
            else_value,
        } => {
            let select = Expr::If {
                cond: Box::new(lower_condition(condition, cx)?),
                then_expr: Box::new(lower_value(then_value, cx)?),
                else_expr: Box::new(lower_value(else_value, cx)?),
            };
            match lower_type(cx, &value.ty)? {
                ty @ rust::Type::FnPtr { .. } => {
                    let temp = cx.next_temp();
                    Expr::Block(Box::new(rust::Block {
                        stmts: vec![Stmt::Let {
                            name: temp.clone(),
                            mutable: false,
                            ty: Some(ty),
                            init: Some(select),
                        }],
                        tail: Some(Box::new(Expr::Var(temp.into()))),
                    }))
                }
                _ => select,
            }
        }
        ValueKind::Compare {
            op, left, right, ..
        } => Expr::Binary {
            op: match op {
                ir::CompareOp::Eq => BinOp::Eq,
                ir::CompareOp::Ne => BinOp::Ne,
                ir::CompareOp::Lt => BinOp::Lt,
                ir::CompareOp::Le => BinOp::Le,
                ir::CompareOp::Gt => BinOp::Gt,
                ir::CompareOp::Ge => BinOp::Ge,
            },
            lhs: Box::new(lower_value(left, cx)?),
            rhs: Box::new(lower_value(right, cx)?),
        },
        ValueKind::PointerOffset {
            pointer,
            amount,
            subtract,
            element,
            ..
        } => lower_pointer_offset(value, pointer, amount, *subtract, element, cx)?,
        ValueKind::PointerDifference {
            left,
            right,
            element,
        } => lower_pointer_difference(value, left, right, element, cx)?,
        ValueKind::FunctionDecay {
            place:
                ir::Place {
                    kind: PlaceKind::Binding(id),
                    ..
                },
        } => match cx.names.get(id) {
            Some(name) if !name.is_extern => Expr::Call {
                func: Box::new(Expr::Var("Some".into())),
                args: vec![Expr::Var(name.rust.as_str().into())],
                binding: CallBinding::Generated,
            },
            _ => {
                return Err(super::Error::Unsupported(format!(
                    "value {}",
                    value.display(false)
                )));
            }
        },
        ValueKind::Null => lower_null(value, cx)?,
        ValueKind::Call {
            callee: ir::Callee::Direct(id),
            arguments,
            ..
        } if cx.names.get(id).is_some_and(|name| name.is_extern)
            && std::iter::once(&value.ty)
                .chain(arguments.iter().map(|argument| &argument.ty))
                .any(|ty| holds_long_double(cx, ty)) =>
        {
            lower_long_double_bridge(cx, &cx.names[id], arguments, &value.ty)?
        }
        ValueKind::Call {
            callee: ir::Callee::Direct(id),
            signature: ir::Type::Function { parameters, .. },
            arguments,
            ..
        } if cx.names.get(id).is_some_and(|name| name.is_variadic)
            && arguments
                .iter()
                .skip(parameters.len())
                .any(|argument| holds_long_double(cx, &argument.ty)) =>
        {
            lower_long_double_variadic_trampoline(
                cx,
                &cx.names[id],
                arguments,
                parameters.len(),
                &value.ty,
            )?
        }
        ValueKind::Call {
            callee, arguments, ..
        } => lower_call(callee, arguments, cx)?,
        ValueKind::Aggregate { members, zero_fill } => {
            lower_aggregate(cx, &value.ty, members, *zero_fill)?
        }
        _ => {
            return Err(super::Error::Unsupported(format!(
                "value {}",
                value.display(false)
            )));
        }
    })
}

pub(super) fn zeroed() -> Expr {
    Expr::Unsafe(Box::new(rust::Block {
        stmts: Vec::new(),
        tail: Some(Box::new(Expr::Call {
            func: Box::new(Expr::Var("std::mem::zeroed".into())),
            args: Vec::new(),
            binding: CallBinding::Generated,
        })),
    }))
}

pub(super) fn lower_aggregate(
    cx: &Context,
    ty: &ir::Type,
    members: &[ir::AggregateMember],
    zero_fill: bool,
) -> Result<Expr> {
    let unsupported = || super::Error::Unsupported(format!("aggregate of {ty}"));
    let lowered_ty = lower_type(cx, ty)?;
    let fields = record_fields(cx, ty);
    let resolved = resolve_type(cx, ty);
    if fields.is_none() && !matches!(resolved, ir::Type::Array { .. }) {
        return Err(unsupported());
    }
    let field_name = |index: usize| {
        fields
            .and_then(|fields| fields.get(index))
            .and_then(|field| field.name.clone())
            .ok_or_else(unsupported)
    };
    let complete = !zero_fill && members.iter().enumerate().all(|(position, member)| {
        matches!(
            (&member.target, fields),
            (ir::AggregateTarget::Field(index), Some(_)) if *index == position
        ) || matches!(
            (&member.target, resolved),
            (ir::AggregateTarget::Index(index), ir::Type::Array { .. }) if *index == position as u64
        )
    });
    if complete {
        let values = members
            .iter()
            .map(|member| lower_value(&member.value, cx))
            .collect::<Result<Vec<_>>>()?;
        return Ok(match (&lowered_ty, fields) {
            (rust::Type::Custom(name), Some(_)) => Expr::StructLit {
                name: name.clone(),
                fields: (0..values.len())
                    .map(field_name)
                    .collect::<Result<Vec<_>>>()?
                    .into_iter()
                    .zip(values)
                    .collect(),
            },
            _ => Expr::ArrayLit(values),
        });
    }
    let target = cx.next_temp();
    let mut stmts = vec![Stmt::Let {
        name: target.clone(),
        mutable: true,
        ty: Some(lowered_ty),
        init: Some(zeroed()),
    }];
    for member in members {
        let place = match member.target {
            ir::AggregateTarget::Field(index) => Expr::Field {
                base: Box::new(Expr::Var(target.as_str().into())),
                field: field_name(index)?,
            },
            ir::AggregateTarget::Index(index) => Expr::Index {
                base: Box::new(Expr::Var(target.as_str().into())),
                index: Box::new(Expr::Value(rust::RustValue::U128(index.into()))),
            },
            ir::AggregateTarget::Range { .. } => return Err(unsupported()),
        };
        stmts.push(Stmt::Assign {
            target: place,
            value: lower_value(&member.value, cx)?,
        });
    }
    Ok(Expr::Block(Box::new(rust::Block {
        stmts,
        tail: Some(Box::new(Expr::Var(target.as_str().into()))),
    })))
}

pub(super) fn lower_number(cx: &Context, number: &Number, ty: &ir::Type) -> Result<Expr> {
    let value = match number {
        Number::Bool(value) => rust::RustValue::Bool(*value),
        Number::Integer(value) => rust::RustValue::U128(
            value
                .to_string()
                .parse()
                .map_err(|_| super::Error::Unsupported(format!("integer constant {value}")))?,
        ),
        Number::SignedInteger(value) => rust::RustValue::I128(
            value
                .to_string()
                .parse()
                .map_err(|_| super::Error::Unsupported(format!("integer constant {value}")))?,
        ),
        Number::FloatBits(bits) => {
            let (literal, finite, name) = match ty {
                ir::Type::Numeric(ir::NumericType::Float(ir::FloatType::F32)) => {
                    let value = f32::from_bits(*bits as u32);
                    (format!("{value:?}"), value.is_finite(), "f32")
                }
                ir::Type::Numeric(ir::NumericType::Float(ir::FloatType::F64)) => {
                    let value = f64::from_bits(*bits as u64);
                    (format!("{value:?}"), value.is_finite(), "f64")
                }
                ir::Type::Numeric(ir::NumericType::Float(ir::FloatType::F80)) => {
                    lower_type(cx, ty)?;
                    return Ok(long_double_literal(*bits));
                }
                _ => return Err(super::Error::Unsupported(format!("constant {number:?}"))),
            };
            if !finite {
                return Ok(Expr::HexFloat(format!("{name}::from_bits({bits:#x})")));
            }
            return Ok(match literal.strip_prefix('-') {
                Some(magnitude) => Expr::Unary {
                    op: rust::UnaryOp::Neg,
                    expr: Box::new(Expr::HexFloat(format!("{magnitude}{name}"))),
                },
                None => Expr::HexFloat(format!("{literal}{name}")),
            });
        }
        _ => return Err(super::Error::Unsupported(format!("constant {number:?}"))),
    };
    Ok(Expr::Cast {
        expr: Box::new(Expr::Value(value)),
        ty: lower_type(cx, ty)?,
    })
}
