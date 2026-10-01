use super::*;

pub(super) fn lower_assignment(cx: &Context, place: &ir::Place, value: Expr) -> Result<Stmt> {
    let assignment = Stmt::Assign {
        target: lower_place(place, cx)?,
        value,
    };
    Ok(if place_is_unsafe(cx, place) {
        Stmt::Unsafe {
            body: rust::Block {
                stmts: vec![assignment],
                tail: None,
            },
        }
    } else {
        assignment
    })
}

pub(super) fn lower_statement(statement: &ir::Statement, cx: &Context) -> Result<Stmt> {
    Ok(match statement {
        ir::Statement::Temporary {
            id,
            ty,
            initializer,
            ..
        } => Stmt::Let {
            name: binding_name(*id, &cx.bindings),
            mutable: false,
            ty: Some(lower_type(cx, ty)?),
            init: initializer
                .as_ref()
                .map(|value| lower_value(value, cx))
                .transpose()?,
        },
        ir::Statement::Let(variable) => {
            let init = variable
                .initializer
                .as_ref()
                .map(|value| lower_value(value, cx))
                .transpose()?;
            let init = match (init, &variable.ty) {
                (Some(init), _) => Some(init),
                (
                    None,
                    ir::Type::Array {
                        element,
                        length: Some(length),
                    },
                ) => match lower_type(cx, element)? {
                    ty @ (rust::Type::Prim(_) | rust::Type::Ptr { .. }) => {
                        Some(Expr::ArrayRepeat {
                            elem: Box::new(Expr::Cast {
                                expr: Box::new(Expr::Value(rust::RustValue::I64(0))),
                                ty,
                            }),
                            len: *length as usize,
                        })
                    }
                    _ => Some(zeroed()),
                },
                (None, ty) => Some(match lower_type(cx, ty)? {
                    rust::Type::Prim(Prim::Bool) => Expr::Value(rust::RustValue::Bool(false)),
                    ty @ rust::Type::Prim(_) => Expr::Cast {
                        expr: Box::new(Expr::Value(rust::RustValue::I64(0))),
                        ty,
                    },
                    _ => zeroed(),
                }),
            };
            Stmt::Let {
                name: binding_name(variable.id, &cx.bindings),
                mutable: true,
                ty: Some(lower_type(cx, &variable.ty)?),
                init,
            }
        }
        ir::Statement::Expression(value) => match &value.node.value {
            ValueKind::Void | ValueKind::VaEnd { .. } => Stmt::Block(rust::Block::default()),
            ValueKind::VaStart { list } => {
                lower_assignment(cx, list, clone_va_list(Expr::Var(VA_ARGS.into())))?
            }
            ValueKind::VaCopy {
                destination,
                source,
            } => lower_assignment(cx, destination, clone_va_list(lower_place(source, cx)?))?,
            _ => Stmt::Expr(lower_value(value, cx)?),
        },
        ir::Statement::Write { place, value, .. } => {
            lower_assignment(cx, place, lower_value(value, cx)?)?
        }
        ir::Statement::Return(value) => Stmt::Return(
            value
                .as_ref()
                .map(|value| lower_value(value, cx))
                .transpose()?,
        ),
        ir::Statement::Block(body) => Stmt::Scope {
            body: body
                .iter()
                .map(|statement| lower_statement(statement, cx))
                .collect::<Result<Vec<_>>>()?,
        },
        ir::Statement::If {
            condition,
            then_body,
            else_body,
        } => Stmt::If {
            cond: lower_condition(condition, cx)?,
            then_body: lower_statement_list(then_body, cx)?,
            else_body: else_body
                .as_ref()
                .map(|body| lower_statement_list(body, cx))
                .transpose()?
                .unwrap_or_default(),
        },
        ir::Statement::While {
            id,
            condition,
            body,
        } => {
            let body = lower_statement_list(body, cx)?;
            let (mut prefix, condition) = lower_evaluation(condition, cx)?;
            prefix.push(Stmt::If {
                cond: Expr::Unary {
                    op: rust::UnaryOp::Not,
                    expr: Box::new(condition),
                },
                then_body: vec![Stmt::Break(None)],
                else_body: Vec::new(),
            });
            prefix.push(Stmt::LabeledBlock {
                label: continue_label(*id),
                body,
            });
            Stmt::Loop {
                label: Some(break_label(*id)),
                body: prefix,
            }
        }
        ir::Statement::For {
            id,
            init,
            condition,
            increment,
            body,
        } => {
            let mut statements = lower_statement_list(init, cx)?;
            let body = lower_statement_list(body, cx)?;
            let mut loop_body = Vec::new();
            if let Some(condition) = condition {
                let (prefix, condition) = lower_evaluation(condition, cx)?;
                loop_body.extend(prefix);
                loop_body.push(Stmt::If {
                    cond: Expr::Unary {
                        op: rust::UnaryOp::Not,
                        expr: Box::new(condition),
                    },
                    then_body: vec![Stmt::Break(None)],
                    else_body: Vec::new(),
                });
            }
            loop_body.push(Stmt::LabeledBlock {
                label: continue_label(*id),
                body,
            });
            if let Some(increment) = increment {
                loop_body.extend(lower_evaluation_statements(increment, cx)?);
            }
            statements.push(Stmt::Loop {
                label: Some(break_label(*id)),
                body: loop_body,
            });
            Stmt::Scope { body: statements }
        }
        ir::Statement::DoWhile {
            id,
            body,
            condition,
        } => {
            let body = lower_statement_list(body, cx)?;
            let mut loop_body = vec![Stmt::LabeledBlock {
                label: continue_label(*id),
                body,
            }];
            let (prefix, condition) = lower_evaluation(condition, cx)?;
            loop_body.extend(prefix);
            loop_body.push(Stmt::If {
                cond: Expr::Unary {
                    op: rust::UnaryOp::Not,
                    expr: Box::new(condition),
                },
                then_body: vec![Stmt::Break(None)],
                else_body: Vec::new(),
            });
            Stmt::Loop {
                label: Some(break_label(*id)),
                body: loop_body,
            }
        }
        ir::Statement::Switch {
            id,
            discriminant,
            body,
        } => lower_switch(*id, discriminant, body, cx)?,
        ir::Statement::Break(id) => Stmt::Break(Some(break_label(*id))),
        ir::Statement::Continue(id) => Stmt::Break(Some(continue_label(*id))),
        ir::Statement::Null => Stmt::Block(rust::Block::default()),
        _ => {
            return Err(super::Error::Unsupported(format!(
                "statement {statement:?}"
            )));
        }
    })
}

pub(super) fn lower_statement_list(
    statements: &[slate_parser::ast::Span<ir::Statement>],
    cx: &Context,
) -> Result<Vec<Stmt>> {
    statements
        .iter()
        .map(|statement| lower_statement(statement, cx))
        .collect()
}

pub(super) fn break_label(id: BindingId) -> rust::Label {
    rust::Label::new(format!("__slate_break_{}", id.0))
}

pub(super) fn continue_label(id: BindingId) -> rust::Label {
    rust::Label::new(format!("__slate_continue_{}", id.0))
}
