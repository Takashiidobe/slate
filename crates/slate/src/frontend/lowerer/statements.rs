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
            if let Some(write) = self.volatile_bit_field_write(place, value.clone())? {
                return Ok(write);
            }
            return Ok(Stmt::Expr(Expr::Unsafe(Box::new(rust::Block {
                stmts: Vec::new(),
                tail: Some(Box::new(Expr::Call {
                    func: Box::new(Expr::Var("std::ptr::write_volatile".into())),
                    args: vec![self.lower_address(place, true)?, value],
                    binding: CallBinding::Generated,
                })),
            }))));
        }
        if let Some(lane) = self.lower_lane_write(place, &value)? {
            return Ok(lane);
        }
        if let Some(address) = self.erased_field_address(place, true)? {
            return Ok(Stmt::Unsafe {
                body: rust::Block {
                    stmts: vec![Stmt::Expr(Expr::MethodCall {
                        recv: Box::new(address),
                        method: "write_unaligned".into(),
                        args: vec![value],
                    })],
                    tail: None,
                },
            });
        }
        if let Some(unaligned) = self.lower_unaligned_write(place, &value)? {
            return Ok(unaligned);
        }
        let assignment = match self.bit_field_accessor(place, "set")? {
            Some((storage, setter)) if is_trivial_operand(&value) => Stmt::Expr(Expr::MethodCall {
                recv: Box::new(storage),
                method: setter,
                args: vec![value],
            }),
            Some((storage, setter)) => {
                let operand = self.next_temp();
                Stmt::Scope {
                    body: vec![
                        Stmt::Let {
                            name: operand.clone(),
                            mutable: false,
                            ty: None,
                            init: Some(value),
                        },
                        Stmt::Expr(Expr::MethodCall {
                            recv: Box::new(storage),
                            method: setter,
                            args: vec![Expr::Var(operand.into())],
                        }),
                    ],
                }
            }
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
            ir::Statement::Comment(comment) => {
                self.dependencies.emitted_comments.insert(statement.id);
                Stmt::Comment(comments::comment(comment))
            }
            ir::Statement::Temporary {
                id,
                ty,
                initializer,
                ..
            } => Stmt::Let {
                name: binding_name(*id, &self.tables.bindings),
                mutable: true,
                ty: Some(self.lower_type(ty)?),
                init: initializer
                    .as_ref()
                    .map(|value| self.lower_value(value))
                    .transpose()?,
            },
            ir::Statement::Let(variable) if self.tables.variably_modified(&variable.ty) => {
                self.lower_vla_let(variable)?
            }
            ir::Statement::Let(variable) => {
                if variable.register.is_some() {
                    self.register_locals.insert(variable.id);
                }
                let alignment = self.tables.over_alignment(&variable.ty, variable.alignment);
                if alignment.is_some() {
                    self.aligned_locals.insert(variable.id);
                }
                let outer = self.declaring.replace(Some((variable.id, false)));
                let init = variable
                    .initializer
                    .as_ref()
                    .map(|value| self.lower_value(value))
                    .transpose();
                let self_referential = matches!(self.declaring.replace(outer), Some((_, true)));
                let mut init = init?;
                let self_initializer = init.take_if(|_| self_referential);
                let auto_value = match variable.auto_init {
                    Some(auto) if init.is_none() && self_initializer.is_none() => {
                        self.auto_init_value(&variable.ty, auto)?
                    }
                    _ => None,
                };
                let auto_init = variable.auto_init.filter(|_| auto_value.is_none());
                let init = init.or(auto_value);
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
                let ty = self.lower_type(&variable.ty)?;
                let (ty, init) = match alignment {
                    Some(alignment) => (
                        self.align_wrapped(&variable.name, alignment, ty)?,
                        init.map(|init| align_wrap(alignment, init)),
                    ),
                    None => (ty, init),
                };
                if let Some(value) = self_initializer {
                    let place = ir::Place {
                        ty: variable.ty.clone(),
                        kind: PlaceKind::Binding(variable.id),
                        access: variable.access,
                    };
                    self.after_declaration = Some(self.lower_assignment(&place, value)?);
                } else if let Some(auto) = auto_init {
                    let writes = self.auto_init_writes(
                        &variable.ty,
                        auto,
                        self.local_bytes(variable.id),
                        true,
                    )?;
                    if !writes.is_empty() {
                        self.after_declaration = Some(Stmt::Unsafe {
                            body: rust::Block {
                                stmts: writes,
                                tail: None,
                            },
                        });
                    }
                }
                Stmt::Let {
                    name: binding_name(variable.id, &self.tables.bindings),
                    mutable: true,
                    ty: Some(ty),
                    init,
                }
            }
            ir::Statement::Expression(value) => match &value.node.value {
                ValueKind::Void | ValueKind::VaEnd { .. } => Stmt::Block(rust::Block::default()),
                ValueKind::VaStart {
                    list,
                    convention: ir::VaConvention::Target,
                } => {
                    let target = self.lower_type(&list.ty)?;
                    self.lower_assignment(list, static_va_list(Expr::Var(VA_ARGS.into()), target))?
                }
                ValueKind::VaCopy {
                    destination,
                    source,
                    convention,
                } => {
                    let unsafe_source = self.tables.place_is_unsafe(source);
                    let source = self.lower_place(source)?;
                    let source = match convention {
                        ir::VaConvention::Target if unsafe_source => {
                            clone_va_list(through_raw_pointer(source))
                        }
                        ir::VaConvention::Target => clone_va_list(source),
                        ir::VaConvention::Win64 => source,
                    };
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
            ir::Statement::Asm(asm) => self.lower_asm(statement, asm, &[], None)?,
            ir::Statement::OmpParallel(region) => self.lower_omp_parallel(statement, region)?,
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
        let mut lowered = Vec::with_capacity(statements.len());
        for statement in statements {
            lowered.extend(self.lower_statement(statement)?);
            if let ir::Statement::Let(variable) = &statement.value
                && let Some(cleanup) = variable.cleanup
            {
                lowered.extend(self.cleanup_guard(variable, cleanup)?);
            }
        }
        Ok(lowered)
    }

    fn cleanup_guard(&mut self, variable: &ir::Variable, cleanup: BindingId) -> Result<[Stmt; 2]> {
        self.dependencies.cleanup = true;
        let names = &self.tables.names;
        let handler = names
            .get(&cleanup)
            .ok_or(Invariant::UnknownCallee(cleanup))?;
        let target = binding_name(variable.id, &self.tables.bindings);
        let pointer = format!("__slate_cleanup_ptr_{}", variable.id.0);
        let call = Expr::Call {
            func: Box::new(Expr::Var(handler.rust.as_str().into())),
            args: vec![Expr::Cast {
                expr: Box::new(Expr::Var(pointer.as_str().into())),
                ty: rust::Type::Custom("*mut _".into()),
            }],
            binding: CallBinding::Direct {
                identity: handler.identity,
                canonical_type: None,
                trusted_declaration: None,
                trusted_headers: Default::default(),
            },
        };
        Ok([
            Stmt::Let {
                name: pointer,
                mutable: false,
                ty: None,
                init: Some(Expr::Cast {
                    expr: Box::new(Expr::AddrOf {
                        mutable: true,
                        expr: Box::new(Expr::Var(target.as_str().into())),
                    }),
                    ty: rust::Type::Custom("*mut _".into()),
                }),
            },
            Stmt::Let {
                name: format!("__slate_cleanup_{}", variable.id.0),
                mutable: false,
                ty: None,
                init: Some(Expr::Call {
                    func: Box::new(Expr::Var("__slate_cleanup::Guard".into())),
                    args: vec![Expr::Closure {
                        params: Vec::new(),
                        body: Box::new(if handler.is_unsafe {
                            Expr::Unsafe(Box::new(rust::Block {
                                stmts: Vec::new(),
                                tail: Some(Box::new(call)),
                            }))
                        } else {
                            call
                        }),
                    }],
                    binding: CallBinding::Generated,
                }),
            },
        ])
    }

    pub(super) fn lower_statement(
        &mut self,
        statement: &slate_parser::ast::Span<ir::Statement>,
    ) -> Result<Vec<Stmt>> {
        let lowered = self
            .lower_statement_node(statement)
            .map_err(|error| error.used_at(Site::of(statement)))?;
        Ok(std::iter::once(lowered)
            .chain(self.after_declaration.take())
            .collect())
    }
}

fn is_trivial_operand(value: &Expr) -> bool {
    match value {
        Expr::Value(_) | Expr::Var(_) => true,
        Expr::Cast { expr, .. } | Expr::Unary { expr, .. } => is_trivial_operand(expr),
        _ => false,
    }
}
