use super::*;

pub(super) fn break_label(id: BindingId) -> rust::Label {
    rust::Label::new(format!("__slate_break_{}", id.0))
}

pub(super) fn continue_label(id: BindingId) -> rust::Label {
    rust::Label::new(format!("__slate_continue_{}", id.0))
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn lower_assignment(&mut self, place: &ir::Place, value: Expr) -> Result<Stmt> {
        if place.access.volatile {
            return Ok(Stmt::Expr(Expr::Unsafe(Box::new(rust::Block {
                stmts: Vec::new(),
                tail: Some(Box::new(Expr::Call {
                    func: Box::new(Expr::Var("std::ptr::write_volatile".into())),
                    args: vec![self.lower_address(place, true)?, value],
                    binding: CallBinding::Generated,
                })),
            }))));
        }
        let assignment = match self.bit_field_accessor(place, "set")? {
            Some((storage, setter)) => Stmt::Expr(Expr::MethodCall {
                recv: Box::new(storage),
                method: setter,
                args: vec![value],
            }),
            None => Stmt::Assign {
                target: self.lower_place(place)?,
                value,
            },
        };
        Ok(if self.tables.place_is_unsafe(place) {
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

    fn lower_statement_node(
        &mut self,
        statement: &slate_parser::ast::Span<ir::Statement>,
    ) -> Result<Stmt> {
        Ok(match &statement.value {
            ir::Statement::Temporary {
                id,
                ty,
                initializer,
                ..
            } => Stmt::Let {
                name: binding_name(*id, &self.tables.bindings),
                mutable: false,
                ty: Some(self.lower_type(ty)?),
                init: initializer
                    .as_ref()
                    .map(|value| self.lower_value(value))
                    .transpose()?,
            },
            ir::Statement::Let(variable) => {
                let init = variable
                    .initializer
                    .as_ref()
                    .map(|value| self.lower_value(value))
                    .transpose()?;
                let init = match (init, &variable.ty) {
                    (Some(init), _) => Some(init),
                    (
                        None,
                        ir::Type::Array {
                            element,
                            length: Some(length),
                        },
                    ) => match self.lower_type(element)? {
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
                    (None, ty) => Some(match self.lower_type(ty)? {
                        rust::Type::Prim(Prim::Bool) => Expr::Value(rust::RustValue::Bool(false)),
                        ty @ rust::Type::Prim(_) => Expr::Cast {
                            expr: Box::new(Expr::Value(rust::RustValue::I64(0))),
                            ty,
                        },
                        _ => zeroed(),
                    }),
                };
                Stmt::Let {
                    name: binding_name(variable.id, &self.tables.bindings),
                    mutable: true,
                    ty: Some(self.lower_type(&variable.ty)?),
                    init,
                }
            }
            ir::Statement::Expression(value) => match &value.node.value {
                ValueKind::Void | ValueKind::VaEnd { .. } => Stmt::Block(rust::Block::default()),
                ValueKind::VaStart { list } => {
                    self.lower_assignment(list, clone_va_list(Expr::Var(VA_ARGS.into())))?
                }
                ValueKind::VaCopy {
                    destination,
                    source,
                } => {
                    let source = clone_va_list(self.lower_place(source)?);
                    self.lower_assignment(destination, source)?
                }
                _ => Stmt::Expr(self.lower_value(value)?),
            },
            ir::Statement::Write {
                place,
                value,
                ordering,
                ..
            } => {
                let value = self.lower_value(value)?;
                match ordering {
                    Some(ordering) => {
                        Stmt::Expr(self.lower_atomic_access(place, ordering, Some(value))?)
                    }
                    None => self.lower_assignment(place, value)?,
                }
            }
            ir::Statement::Return(value) => Stmt::Return(
                value
                    .as_ref()
                    .map(|value| self.lower_value(value))
                    .transpose()?,
            ),
            ir::Statement::Block(body) => Stmt::Scope {
                body: self.lower_statement_list(body)?,
            },
            ir::Statement::If {
                condition,
                then_body,
                else_body,
            } => Stmt::If {
                cond: self.lower_condition(condition)?,
                then_body: self.lower_statement_list(then_body)?,
                else_body: else_body
                    .as_ref()
                    .map(|body| self.lower_statement_list(body))
                    .transpose()?
                    .unwrap_or_default(),
            },
            ir::Statement::While {
                id,
                condition,
                body,
            } => {
                let body = self.lower_statement_list(body)?;
                let (mut prefix, condition) = self.lower_evaluation(condition)?;
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
                let mut statements = self.lower_statement_list(init)?;
                let body = self.lower_statement_list(body)?;
                let mut loop_body = Vec::new();
                if let Some(condition) = condition {
                    let (prefix, condition) = self.lower_evaluation(condition)?;
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
                    loop_body.extend(self.lower_evaluation_statements(increment)?);
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
                let body = self.lower_statement_list(body)?;
                let mut loop_body = vec![Stmt::LabeledBlock {
                    label: continue_label(*id),
                    body,
                }];
                let (prefix, condition) = self.lower_evaluation(condition)?;
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
            } => self
                .lower_switch(*id, discriminant, body)
                .map_err(|error| error.within(Site::of(statement), "in switch".into()))?,
            ir::Statement::Break(id) => Stmt::Break(Some(break_label(*id))),
            ir::Statement::Continue(id) => Stmt::Break(Some(continue_label(*id))),
            ir::Statement::Fence { ordering, scope } => {
                Stmt::Expr(self.lower_fence(*scope, ordering).ok_or_else(|| {
                    Failure::from(Construct::Statement {
                        kind: "Fence".into(),
                        ir: statement.display().to_string(),
                    })
                })?)
            }
            ir::Statement::Null => Stmt::Block(rust::Block::default()),
            _ => {
                return Err(Construct::Statement {
                    kind: variant_name(&statement.value),
                    ir: statement.display().to_string(),
                }
                .into());
            }
        })
    }

    pub(super) fn lower_statement_list(
        &mut self,
        statements: &[slate_parser::ast::Span<ir::Statement>],
    ) -> Result<Vec<Stmt>> {
        statements
            .iter()
            .map(|statement| self.lower_statement(statement))
            .collect()
    }

    pub(super) fn lower_statement(
        &mut self,
        statement: &slate_parser::ast::Span<ir::Statement>,
    ) -> Result<Stmt> {
        self.lower_statement_node(statement)
            .map_err(|error| error.used_at(Site::of(statement)))
    }
}
