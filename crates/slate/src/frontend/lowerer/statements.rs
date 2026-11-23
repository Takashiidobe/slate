use super::*;

impl FunctionLowerer<'_, '_> {
    pub(super) fn lower_known_if(
        &mut self,
        condition: bool,
        then_body: &[Span<ir::Statement>],
        else_body: Option<&[Span<ir::Statement>]>,
    ) -> Result<Stmt> {
        let body = if condition {
            then_body
        } else {
            else_body.unwrap_or_default()
        };
        if body.is_empty() {
            return Ok(Stmt::Block(rust::Block::default()));
        }
        Ok(Stmt::Scope {
            body: self.lower_statement_list(body)?,
        })
    }

    pub(super) fn break_label(&self, id: BindingId) -> rust::Label {
        rust::Label::new(format!("__slate_break_{}", self.local_index(id)))
    }

    pub(super) fn continue_label(&self, id: BindingId) -> rust::Label {
        rust::Label::new(format!("__slate_continue_{}", self.local_index(id)))
    }

    fn loop_label(&self, id: BindingId, body: &mut [Stmt]) -> Option<rust::Label> {
        let label = self.break_label(id);
        structure::label_needed(body, &label).then_some(label)
    }

    fn continue_block(&self, id: BindingId, body: Vec<Stmt>) -> Stmt {
        let label = self.continue_label(id);
        if structure::uses_label_or_exprs(&body, &label) {
            Stmt::LabeledBlock { label, body }
        } else {
            Stmt::Scope { body }
        }
    }

    pub(super) fn lower_assignment(&mut self, place: &ir::Place, value: Expr) -> Result<Stmt> {
        if let Some(store) = self.lower_heap_store(place, &value) {
            return Ok(store);
        }
        if place.access.volatile {
            if let Some(write) = self.bit_unit_write(place, value.clone(), true)? {
                return Ok(write);
            }
            self.reject_unaligned_volatile(place)?;
            return Ok(Stmt::Expr(unsafe_op(Expr::Call {
                func: Box::new(Expr::var("core::ptr::write_volatile".into())),
                args: vec![self.lower_address(place, true)?, value],
                binding: CallBinding::Generated,
                unsafe_: rust::Unsafe(true),
            })));
        }
        if let Some(lane) = self.lower_lane_write(place, &value)? {
            return Ok(lane);
        }
        if let Some(erased) = self.erased_ancestor(place)? {
            let operand = self.next_temp();
            let pointer = self.next_temp();
            let address = self.lower_address(erased, true)?;
            let (copy, assignment) = self.with_erased_copy(erased, |lowerer| {
                lowerer.lower_assignment(place, Expr::var(operand.as_str().into()))
            })?;
            let pointer_var = || Expr::var(pointer.as_str().into());
            return Ok(Stmt::Scope {
                body: vec![
                    Stmt::Let {
                        origin: rust::LocalOrigin::Synthetic,
                        name: operand.clone(),
                        mutable: false,
                        ty: None,
                        init: Some(value),
                    },
                    Stmt::Let {
                        origin: rust::LocalOrigin::Synthetic,
                        name: pointer.clone(),
                        mutable: false,
                        ty: None,
                        init: Some(address),
                    },
                    Stmt::Let {
                        origin: rust::LocalOrigin::Synthetic,
                        name: copy.clone(),
                        mutable: true,
                        ty: None,
                        init: Some(Expr::MethodCall {
                            recv: Box::new(pointer_var()),
                            method: "read_unaligned".into(),
                            args: Vec::new(),
                            unsafe_: rust::Unsafe(true),
                        }),
                    },
                    assignment,
                    Stmt::Expr(Expr::MethodCall {
                        recv: Box::new(pointer_var()),
                        method: "write_unaligned".into(),
                        args: vec![Expr::var(copy.as_str().into())],
                        unsafe_: rust::Unsafe(true),
                    }),
                ],
            });
        }
        if let Some(address) = self.erased_field_address(place, true)? {
            return Ok(Stmt::Expr(Expr::MethodCall {
                recv: Box::new(address),
                method: "write_unaligned".into(),
                args: vec![value],
                unsafe_: rust::Unsafe(true),
            }));
        }
        if let Some(unaligned) = self.lower_unaligned_write(place, &value)? {
            return Ok(unaligned);
        }
        if self.bit_unit_in_packed(place)
            && let Some(write) = self.bit_unit_write(place, value.clone(), false)?
        {
            return Ok(write);
        }
        let assignment = match self.bit_field_accessor(place, "set")? {
            Some((storage, setter)) if is_trivial_operand(&value) => Stmt::Expr(Expr::MethodCall {
                recv: Box::new(storage),
                method: setter,
                args: vec![value],
                unsafe_: rust::Unsafe(false),
            }),
            Some((storage, setter)) => {
                let operand = self.next_temp();
                Stmt::Scope {
                    body: vec![
                        Stmt::Let {
                            origin: rust::LocalOrigin::Synthetic,
                            name: operand.clone(),
                            mutable: false,
                            ty: None,
                            init: Some(value),
                        },
                        Stmt::Expr(Expr::MethodCall {
                            recv: Box::new(storage),
                            method: setter,
                            args: vec![Expr::var(operand.into())],
                            unsafe_: rust::Unsafe(false),
                        }),
                    ],
                }
            }
            None => Stmt::Assign {
                target: self.lower_place(place)?,
                value: if self.support_scalar_field(place) {
                    Expr::Cast {
                        expr: Box::new(value),
                        ty: rust::Type::Custom("_".into()),
                    }
                } else {
                    value
                },
            },
        };
        Ok(assignment)
    }

    fn lower_statement_node(
        &mut self,
        statement: &slate_parser::ast::Span<ir::Statement>,
    ) -> Result<Stmt> {
        if let Some(action) = self.tables.facts.guarded_heap_statement(statement.id) {
            return self.lower_guarded_heap(&statement.value, action);
        }
        if let Some(lowered) = self.lower_heap_statement(statement)? {
            return Ok(lowered);
        }
        Ok(match &statement.value {
            ir::Statement::Comment(comment) => {
                self.dependencies.emitted_comments.insert(statement.id);
                Stmt::Comment(comments::comment(comment))
            }
            ir::Statement::Temporary {
                id,
                initializer: Some(initializer),
                ..
            } if let Some((pointer, amount)) = self.element_view(initializer) => {
                self.views
                    .insert(*id, (pointer, amount.clone(), initializer.clone()));
                Stmt::Block(rust::Block::default())
            }
            ir::Statement::Temporary {
                id,
                ty,
                initializer,
                ..
            } => Stmt::Let {
                origin: rust::LocalOrigin::Synthetic,
                name: self.binding_name(*id),
                mutable: self.binding_requires_mut(*id),
                ty: Some(self.lower_binding_type(ty, *id)?),
                init: initializer
                    .as_ref()
                    .map(|value| self.lower_value(value))
                    .transpose()?,
            },
            ir::Statement::Let(variable) if self.switch_lets.contains(&variable.id) => {
                match &variable.initializer {
                    Some(value) => {
                        let place = ir::Place::new(
                            variable.ty.clone(),
                            PlaceKind::Binding(variable.id),
                            variable.access,
                        );
                        let value = self.lower_value(value)?;
                        self.lower_assignment(&place, value)?
                    }
                    None => Stmt::Block(rust::Block::default()),
                }
            }
            ir::Statement::Let(variable) if variable.byref => self.lower_byref_let(variable)?,
            ir::Statement::Let(variable)
                if let Some(initializer) = &variable.initializer
                    && self.array_alias(variable.id) =>
            {
                self.array_aliases.insert(variable.id, initializer.clone());
                Stmt::Block(rust::Block::default())
            }
            ir::Statement::Let(variable) if self.tables.variably_modified(&variable.ty) => {
                self.lower_vla_let(variable)?
            }
            ir::Statement::Let(variable) => {
                if variable.register.is_some() {
                    self.register_locals.insert(variable.id);
                }
                if let Some(local) = self.string_local(variable.id) {
                    let (ty, init) = match local {
                        LocalString::Str(text) => (
                            rust::Type::Ref {
                                mutable: false,
                                inner: Box::new(rust::Type::Custom("str".into())),
                            },
                            Expr::Str(text),
                        ),
                        LocalString::CStr(bytes) => (borrows::c_string_type(), Expr::CStr(bytes)),
                    };
                    return Ok(Stmt::Let {
                        origin: rust::LocalOrigin::Source,
                        name: self.binding_name(variable.id),
                        mutable: false,
                        ty: Some(ty),
                        init: Some(init),
                    });
                }
                if let Some(owned) = self.lower_owned_let(variable)? {
                    return Ok(owned);
                }
                let alignment = self.tables.over_alignment(&variable.ty, variable.alignment);
                if let Some(borrow) = self.borrow_of(variable.id) {
                    if let Some(initializer) = &variable.initializer
                        && alignment.is_none()
                        && variable.register.is_none()
                    {
                        return Ok(Stmt::Let {
                            origin: rust::LocalOrigin::Source,
                            name: self.binding_name(variable.id),
                            mutable: self.binding_requires_mut(variable.id),
                            ty: Some(self.borrow_type(&variable.ty, borrow)?),
                            init: Some(self.lower_borrow(initializer, borrow)?),
                        });
                    }
                    self.unborrowed.insert(variable.id);
                }
                if alignment.is_some() {
                    self.aligned_locals.insert(variable.id);
                }
                let in_place_members = match variable.initializer.as_ref() {
                    Some(ir::Value {
                        ty,
                        node:
                            Span {
                                value: ValueKind::Aggregate { members, .. },
                                ..
                            },
                    }) if self.zeroes_variable_padding(variable)
                        && !matches!(self.tables.resolve_type(ty), ir::Type::Vector { .. }) =>
                    {
                        Some(members)
                    }
                    _ => None,
                };
                let outer = self.declaring.replace(Some((variable.id, false)));
                let init = variable
                    .initializer
                    .as_ref()
                    .filter(|_| in_place_members.is_none())
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
                let init = Some(match init {
                    Some(init) => init,
                    None => match &variable.ty {
                        ir::Type::Array {
                            element,
                            length: Some(length),
                        } => match self.lower_type(element)? {
                            rust::Type::Prim(Prim::Bool) => Expr::ArrayRepeat {
                                elem: Box::new(Expr::Value(rust::RustValue::Bool(false))),
                                len: *length as usize,
                            },
                            ty @ (rust::Type::Prim(_) | rust::Type::Ptr { .. }) => {
                                Expr::ArrayRepeat {
                                    elem: Box::new(Expr::Cast {
                                        expr: Box::new(Expr::Value(rust::RustValue::I64(0))),
                                        ty,
                                    }),
                                    len: *length as usize,
                                }
                            }
                            _ => zeroed(),
                        },
                        ir::Type::Sizeless(name) if let Some(zero) = sve::sve_zero(name) => zero,
                        #[expect(
                            slate::sensitive_wildcard,
                            reason = "fixed arrays and SVE types have their own zero; every other type zeroes by its lowered Rust type"
                        )]
                        ty => match self.lower_type(ty)? {
                            rust::Type::Prim(Prim::Bool) => {
                                Expr::Value(rust::RustValue::Bool(false))
                            }
                            ty @ rust::Type::Prim(_) => Expr::Cast {
                                expr: Box::new(Expr::Value(rust::RustValue::I64(0))),
                                ty,
                            },
                            _ => zeroed(),
                        },
                    },
                });
                let ty = self.lower_binding_type(&variable.ty, variable.id)?;
                let (ty, init) = match alignment {
                    Some(wrapper) => (
                        self.align_wrapped(&variable.name, wrapper, ty)?,
                        init.map(|init| align_wrap(wrapper, init)),
                    ),
                    None => (ty, init),
                };
                let in_place_fill = match in_place_members {
                    Some(members) => {
                        let base = self.lower_binding(variable.id);
                        let mut stmts = Vec::new();
                        self.fill_aggregate(&variable.ty, members, &base, true, &mut stmts)?;
                        if let Some(clear) =
                            self.clear_padding_at(self.local_bytes(variable.id), &variable.ty)
                        {
                            stmts.push(Stmt::Expr(clear));
                        }
                        Some(Stmt::Expr(Expr::Block(Box::new(rust::Block {
                            stmts,
                            tail: None,
                        }))))
                    }
                    None => None,
                };
                if let Some(value) = self_initializer {
                    let place = ir::Place::new(
                        variable.ty.clone(),
                        PlaceKind::Binding(variable.id),
                        variable.access,
                    );
                    self.after_declaration = Some(self.lower_assignment(&place, value)?);
                } else if let Some(fill) = in_place_fill {
                    self.after_declaration = Some(fill);
                } else if self.zeroes_variable_padding(variable)
                    && let Some(clear) =
                        self.clear_padding_at(self.local_bytes(variable.id), &variable.ty)
                {
                    self.after_declaration = Some(Stmt::Expr(clear));
                } else if let Some(auto) = auto_init {
                    let writes = self.auto_init_writes(
                        &variable.ty,
                        auto,
                        self.local_bytes(variable.id),
                        true,
                    )?;
                    if !writes.is_empty() {
                        self.after_declaration = Some(Stmt::Block(rust::Block {
                            stmts: writes,
                            tail: None,
                        }));
                    }
                }
                let name = self.binding_name(variable.id);
                let mutable =
                    self.after_declaration.is_some() || self.binding_requires_mut(variable.id);
                let init = match init {
                    Some(Expr::If { cond, then_expr, else_expr })
                        if variable.initializer.as_ref().is_some_and(|value| {
                            matches!(&value.node.value,
                                ValueKind::Conditional { then_value, else_value, .. }
                                if matches!(then_value.node.value, ValueKind::StatementExpression(_))
                                    && matches!(else_value.node.value, ValueKind::StatementExpression(_)))
                        }) =>
                    {
                        let (then_body, then_value) = initializer_arm(*then_expr);
                        let (else_body, else_value) = initializer_arm(*else_expr);
                        return Ok(Stmt::LetIf {
                            name,
                            mutable,
                            ty: Some(ty),
                            cond: *cond,
                            then_body,
                            then_value,
                            else_body,
                            else_value,
                        });
                    }
                    init => init,
                };
                Stmt::Let {
                    origin: if variable.initializer.is_none()
                        && variable.auto_init.is_none()
                        && variable.cleanup.is_none()
                        && self.after_declaration.is_none()
                    {
                        rust::LocalOrigin::SourceUninitialized
                    } else {
                        rust::LocalOrigin::Source
                    },
                    name,
                    mutable,
                    ty: Some(ty),
                    init,
                }
            }
            ir::Statement::Expression(value) => match &value.node.value {
                ValueKind::Void | ValueKind::VaEnd { .. } => Stmt::Block(rust::Block::default()),
                ValueKind::Call { .. } if self.elided_release(value) => {
                    Stmt::Block(rust::Block::default())
                }
                ValueKind::VaStart {
                    list,
                    convention: ir::VaConvention::Target,
                } => {
                    let target = self.lower_type(&list.ty)?;
                    self.lower_assignment(list, static_va_list(Expr::var(VA_ARGS.into()), target))?
                }
                ValueKind::VaCopy {
                    destination,
                    source,
                    convention,
                } => {
                    let unsafe_source = self.place_is_unsafe(source);
                    let source = self.lower_place(source)?;
                    let source = match convention {
                        ir::VaConvention::Target if unsafe_source => {
                            unsafe_op(clone_va_list(through_raw_pointer(source)))
                        }
                        ir::VaConvention::Target => clone_va_list(source),
                        ir::VaConvention::Win64 => source,
                    };
                    self.lower_assignment(destination, source)?
                }
                _ => {
                    let outer = self.discarded_call.replace(value.node.id);
                    let lowered = self.lower_value(value);
                    self.discarded_call = outer;
                    Stmt::Expr(lowered?)
                }
            },
            ir::Statement::Write {
                place:
                    place @ ir::Place {
                        kind: PlaceKind::Binding(id),
                        ..
                    },
                value,
                ordering: None,
                ..
            } if !place.access.volatile
                && let Some(reallocation) = self.lower_reallocation(*id, value)? =>
            {
                reallocation
            }
            ir::Statement::Write {
                place:
                    place @ ir::Place {
                        kind: PlaceKind::Binding(id),
                        ..
                    },
                value,
                ordering: None,
                ..
            } if !place.access.volatile
                && let Some(borrow) = self.borrow_of(*id) =>
            {
                self.reassigned_borrows.insert(*id);
                Stmt::Assign {
                    target: Expr::var(self.binding_name(*id).as_str().into()),
                    value: self.lower_borrow(value, borrow)?,
                }
            }
            ir::Statement::Write {
                place,
                value,
                ordering,
                ..
            } => {
                let updates_in_place = updates_in_place(place, value);
                let value = self.lower_value(value)?;
                match ordering {
                    Some(ordering) => {
                        Stmt::Expr(self.lower_atomic_access(place, ordering, Some(value))?)
                    }
                    None if updates_in_place => {
                        compound_assignment(self.lower_assignment(place, value)?)
                    }
                    None => self.lower_assignment(place, value)?,
                }
            }
            ir::Statement::Return(value) => {
                self.mark_macro_sites_returning();
                Stmt::Return(
                    value
                        .as_ref()
                        .map(|value| self.lower_returned(value))
                        .transpose()?,
                )
            }
            ir::Statement::Block(body) => Stmt::Scope {
                body: self.lower_statement_list(body)?,
            },
            ir::Statement::LabeledBlock { id, body } => Stmt::LabeledBlock {
                label: self.break_label(*id),
                body: self.lower_statement_list(body)?,
            },
            ir::Statement::MacroExpansion { expansion, body } => {
                self.lower_macro_site(*expansion, body)?
            }
            ir::Statement::If {
                condition,
                then_body,
                else_body,
                folded: None,
            } if let Some(known) = self.owned_null_condition(condition) => {
                self.lower_known_if(known, then_body, else_body.as_deref())?
            }
            ir::Statement::If {
                condition,
                then_body,
                else_body,
                folded: None,
            } if let Some(lowered) = self.lower_if_let(
                condition,
                then_body,
                else_body.as_deref().unwrap_or_default(),
            )? =>
            {
                lowered
            }
            ir::Statement::If {
                condition,
                then_body,
                else_body,
                folded,
            } => Stmt::If {
                cond: self.lower_folded(condition, folded.is_some(), Self::lower_condition)?,
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
                self.direct_continues.insert(*id);
                let body = self.lower_statement_list(body)?;
                if condition.statements.is_empty() {
                    let cond = self.lower_condition(&condition.value)?;
                    if matches!(cond, Expr::Value(rust::RustValue::Bool(true))) {
                        let mut body = body;
                        let label = self.loop_label(*id, &mut body);
                        return Ok(Stmt::Loop { label, body });
                    }
                    let mut stmts = body;
                    let label = self.loop_label(*id, &mut stmts);
                    Stmt::While {
                        label,
                        cond,
                        body: rust::Block { stmts, tail: None },
                    }
                } else {
                    let (mut loop_body, condition) = self.lower_exit_condition(condition)?;
                    loop_body.push(Stmt::If {
                        cond: condition,
                        then_body: vec![Stmt::Break(None)],
                        else_body: Vec::new(),
                    });
                    loop_body.push(Stmt::Scope { body });
                    let label = self.loop_label(*id, &mut loop_body);
                    Stmt::Loop {
                        label,
                        body: loop_body,
                    }
                }
            }
            ir::Statement::For {
                id,
                init,
                condition,
                increment,
                body,
            } => {
                if let Some(counted) = self.tables.facts.counted_loop(*id)
                    && let Some(lowered) = self.lower_counted_for(*id, init, counted, body)?
                {
                    return Ok(lowered);
                }
                let mut statements = self.lower_statement_list(init)?;
                if increment.is_none() {
                    self.direct_continues.insert(*id);
                }
                let body = self.lower_statement_list(body)?;
                let mut loop_body = Vec::new();
                let head = match condition {
                    Some(condition) if condition.statements.is_empty() => {
                        Some(self.lower_condition(&condition.value)?).filter(|cond| {
                            !matches!(cond, Expr::Value(rust::RustValue::Bool(true)))
                        })
                    }
                    Some(condition) => {
                        let (prefix, exit) = self.lower_exit_condition(condition)?;
                        loop_body.extend(prefix);
                        loop_body.push(Stmt::If {
                            cond: exit,
                            then_body: vec![Stmt::Break(None)],
                            else_body: Vec::new(),
                        });
                        None
                    }
                    None => None,
                };
                match increment {
                    Some(increment) => {
                        loop_body.push(self.continue_block(*id, body));
                        loop_body.extend(self.lower_evaluation_statements(increment)?);
                    }
                    None => loop_body.push(Stmt::Scope { body }),
                }
                let label = self.loop_label(*id, &mut loop_body);
                statements.push(match head {
                    Some(cond) => Stmt::While {
                        label,
                        cond,
                        body: rust::Block {
                            stmts: loop_body,
                            tail: None,
                        },
                    },
                    None => Stmt::Loop {
                        label,
                        body: loop_body,
                    },
                });
                Stmt::Scope { body: statements }
            }
            ir::Statement::DoWhile {
                id,
                body,
                condition,
            } => {
                let body = self.lower_statement_list(body)?;
                let mut loop_body = vec![self.continue_block(*id, body)];
                let (prefix, condition) = self.lower_exit_condition(condition)?;
                loop_body.extend(prefix);
                loop_body.push(Stmt::If {
                    cond: condition,
                    then_body: vec![Stmt::Break(None)],
                    else_body: Vec::new(),
                });
                let label = self.loop_label(*id, &mut loop_body);
                Stmt::Loop {
                    label,
                    body: loop_body,
                }
            }
            ir::Statement::Switch {
                id,
                discriminant,
                body,
                folded,
            } => self
                .lower_switch(*id, discriminant, folded.as_ref(), body)
                .map_err(|error| error.within(Site::of(statement), "in switch".into()))?,
            ir::Statement::Break(id) => Stmt::Break(Some(self.break_label(*id))),
            ir::Statement::Continue(id) if self.direct_continues.contains(id) => {
                Stmt::Continue(Some(self.break_label(*id)))
            }
            ir::Statement::Continue(id) => Stmt::Break(Some(self.continue_label(*id))),
            ir::Statement::Fence { ordering, scope } => {
                Stmt::Expr(self.lower_fence(*scope, ordering).ok_or_else(|| {
                    Failure::from(Construct::Statement {
                        kind: "Fence".into(),
                        ir: statement.display().to_string(),
                    })
                })?)
            }
            ir::Statement::Asm(asm) => self.lower_asm(statement, asm, 0, None)?,
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
        let outer = self.present.clone();
        let lowered = self.lower_regions(statements);
        self.present = outer;
        lowered
    }

    fn lower_regions(
        &mut self,
        statements: &[slate_parser::ast::Span<ir::Statement>],
    ) -> Result<Vec<Stmt>> {
        let Some(region) = control_flow::residual(statements) else {
            return self.lower_structured_list(statements);
        };
        let mut lowered = self.lower_structured_list(&statements[..region.start])?;
        lowered.extend(self.lower_dispatch(&statements[region.clone()])?);
        lowered.extend(self.lower_structured_list(&statements[region.end..])?);
        Ok(lowered)
    }

    fn lower_structured_list(
        &mut self,
        statements: &[slate_parser::ast::Span<ir::Statement>],
    ) -> Result<Vec<Stmt>> {
        let mut lowered = Vec::with_capacity(statements.len());
        for statement in statements {
            if let ir::Statement::If {
                condition,
                then_body,
                else_body: None,
                folded: None,
            } = &statement.value
                && let Some(guard) = self.lower_let_else(condition, then_body)?
            {
                lowered.push(guard);
                continue;
            }
            lowered.extend(self.lower_statement(statement)?);
            if let ir::Statement::Let(variable) = &statement.value
                && let Some(cleanup) = variable.cleanup
            {
                lowered.extend(self.cleanup_guard(variable, cleanup)?);
            }
        }
        Ok(lowered)
    }

    pub(super) fn non_index_names(
        &self,
        params: &[FnParam],
        body: &[slate_parser::ast::Span<ir::Statement>],
    ) -> HashSet<String> {
        let mut lets = Vec::new();
        let mut indices = HashSet::new();
        collect_lets(self.tables.facts, body, &mut lets, &mut indices);
        lets.into_iter()
            .filter(|id| !indices.contains(id))
            .map(|id| self.binding_name(id))
            .chain(params.iter().map(|param| param.name.clone()))
            .collect()
    }

    fn lower_counted_for(
        &mut self,
        id: BindingId,
        init: &[slate_parser::ast::Span<ir::Statement>],
        counted: &slate_analysis::CountedLoop,
        body: &[slate_parser::ast::Span<ir::Statement>],
    ) -> Result<Option<Stmt>> {
        let Some((last, prefix)) = init.split_last() else {
            return Ok(None);
        };
        let ir::Statement::Let(index) = &last.value else {
            return Ok(None);
        };
        let name = self.binding_name(index.id);
        if index.id != counted.index
            || self.non_index_names.contains(&name)
            || self
                .tables
                .over_alignment(&index.ty, index.alignment)
                .is_some()
        {
            return Ok(None);
        }
        let (mut start, mut end) = (counted.start.clone(), counted.end.clone());
        if self.macro_template_check {
            ir::strip_all_macro_structure_in_value(&mut start);
            ir::strip_all_macro_structure_in_value(&mut end);
        }
        let outer = self.declaring.replace(Some((index.id, false)));
        let start = self.lower_value(&start);
        if matches!(self.declaring.replace(outer), Some((_, true))) {
            return Ok(None);
        }
        let start = start?;
        let mut end = self.lower_value(&end)?;
        if counted.inclusive {
            end = Expr::Binary {
                op: rust::BinOp::Add,
                lhs: Box::new(end),
                rhs: Box::new(Expr::Value(rust::RustValue::I64(1))),
            };
        }
        let mut statements = self.lower_statement_list(prefix)?;
        self.direct_continues.insert(id);
        let mut body = self.lower_statement_list(body)?;
        statements.push(Stmt::For {
            label: self.loop_label(id, &mut body),
            pat: name,
            iter: Expr::Range {
                start: Box::new(start),
                end: Box::new(end),
            },
            body,
        });
        Ok(Some(Stmt::Scope { body: statements }))
    }

    pub(super) fn cleanup_call(&mut self, cleanup: BindingId, pointer: Expr) -> Result<Expr> {
        let handler = self
            .tables
            .names
            .get(&cleanup)
            .ok_or(Invariant::UnknownCallee(cleanup))?;
        let call = Expr::Call {
            func: Box::new(Expr::var(handler.rust.as_str().into())),
            args: vec![Expr::Cast {
                expr: Box::new(pointer),
                ty: rust::Type::Custom("*mut _".into()),
            }],
            binding: CallBinding::Direct {
                identity: handler.identity,
                canonical_type: None,
                trusted_declaration: None,
                trusted_headers: Default::default(),
            },
            unsafe_: rust::Unsafe(false),
        };
        Ok(if handler.is_unsafe {
            unsafe_op(call)
        } else {
            call
        })
    }

    fn cleanup_guard(&mut self, variable: &ir::Variable, cleanup: BindingId) -> Result<[Stmt; 2]> {
        let target = self.binding_name(variable.id);
        let pointer = format!("__slate_cleanup_ptr_{}", self.local_index(variable.id));
        let call = self.cleanup_call(cleanup, Expr::var(pointer.as_str().into()))?;
        Ok([
            Stmt::Let {
                origin: rust::LocalOrigin::Synthetic,
                name: pointer,
                mutable: false,
                ty: None,
                init: Some(Expr::Cast {
                    expr: Box::new(Expr::AddrOf {
                        mutable: true,
                        expr: Box::new(Expr::var(target.as_str().into())),
                    }),
                    ty: rust::Type::Custom("*mut _".into()),
                }),
            },
            Stmt::Let {
                origin: rust::LocalOrigin::Synthetic,
                name: format!("__slate_cleanup_{}", self.local_index(variable.id)),
                mutable: false,
                ty: None,
                init: Some(Expr::Call {
                    func: Box::new(Expr::var("::slate_support::Guard".into())),
                    args: vec![Expr::Ref {
                        mutable: true,
                        expr: Box::new(Expr::Closure {
                            params: Vec::new(),
                            body: Box::new(call),
                        }),
                    }],
                    binding: CallBinding::Generated,
                    unsafe_: rust::Unsafe(false),
                }),
            },
        ])
    }

    pub(super) fn lower_statement(
        &mut self,
        statement: &slate_parser::ast::Span<ir::Statement>,
    ) -> Result<Vec<Stmt>> {
        if let ir::Statement::NestedFunction(nested) = &statement.value {
            self.lower_nested_function(statement, nested)
                .map_err(|error| error.used_at(Site::of(statement)))?;
            return Ok(Vec::new());
        }
        let outer = std::mem::take(&mut self.pending);
        let lowered = self.lower_statement_node(statement);
        let before = std::mem::replace(&mut self.pending, outer);
        let lowered = lowered.map_err(|error| error.used_at(Site::of(statement)))?;
        let diverges = matches!(
            &statement.value,
            ir::Statement::Expression(ir::Value {
                node: Span {
                    value: ValueKind::Call {
                        callee: ir::Callee::Direct(id),
                        ..
                    },
                    ..
                },
                ..
            }) if self.tables.returns_through_unit(*id)
        );
        Ok(before
            .into_iter()
            .chain(std::iter::once(lowered))
            .chain(self.after_declaration.take())
            .chain(diverges.then(|| {
                Stmt::Expr(unsafe_op(Expr::Call {
                    func: Box::new(Expr::var("core::hint::unreachable_unchecked".into())),
                    args: Vec::new(),
                    binding: CallBinding::Generated,
                    unsafe_: rust::Unsafe(true),
                }))
            }))
            .collect())
    }
}

fn initializer_arm(expr: Expr) -> (Vec<Stmt>, Expr) {
    if let Expr::Block(mut block) = expr {
        if let Some(tail) = block.tail.take() {
            return (block.stmts, *tail);
        }
        (Vec::new(), Expr::Block(block))
    } else {
        (Vec::new(), expr)
    }
}

fn collect_lets(
    facts: slate_analysis::UnitFacts<'_>,
    statements: &[slate_parser::ast::Span<ir::Statement>],
    lets: &mut Vec<BindingId>,
    indices: &mut HashSet<BindingId>,
) {
    for statement in statements {
        match &statement.value {
            ir::Statement::Let(variable) => lets.push(variable.id),
            ir::Statement::For { id, .. } => {
                if let Some(counted) = facts.counted_loop(*id) {
                    indices.insert(counted.index);
                }
            }
            _ => {}
        }
        for body in control_flow::children(&statement.value) {
            collect_lets(facts, body, lets, indices);
        }
    }
}

fn is_trivial_operand(value: &Expr) -> bool {
    match value {
        Expr::Value(_) | Expr::Var { .. } => true,
        Expr::Cast { expr, .. } | Expr::Unary { expr, .. } => is_trivial_operand(expr),
        _ => false,
    }
}

fn updates_in_place(place: &ir::Place, value: &ir::Value) -> bool {
    let ir::ValueKind::Arith {
        op:
            ir::ArithOp::Add
            | ir::ArithOp::Sub
            | ir::ArithOp::Mul
            | ir::ArithOp::Div
            | ir::ArithOp::Rem
            | ir::ArithOp::And
            | ir::ArithOp::Or
            | ir::ArithOp::Xor
            | ir::ArithOp::Shl
            | ir::ArithOp::Shr,
        left,
        ..
    } = &value.node.value
    else {
        return false;
    };
    let ir::ValueKind::Read {
        place: read,
        ordering: None,
    } = &left.node.value
    else {
        return false;
    };
    !place.access.volatile && value.ty == place.ty && left.ty == place.ty && same_place(place, read)
}

fn same_place(a: &ir::Place, b: &ir::Place) -> bool {
    if a.access.volatile || b.access.volatile {
        return false;
    }
    match (&a.kind, &b.kind) {
        (PlaceKind::Binding(a), PlaceKind::Binding(b)) => a == b,
        (PlaceKind::Deref(a), PlaceKind::Deref(b)) => same_operand(a, b),
        (
            PlaceKind::Field {
                base: a,
                index: i,
                bits: None,
            },
            PlaceKind::Field {
                base: b,
                index: j,
                bits: None,
            },
        ) => i == j && same_place(a, b),
        (PlaceKind::Index { base: a, index: i }, PlaceKind::Index { base: b, index: j }) => {
            same_operand(a, b) && same_operand(i, j)
        }
        _ => false,
    }
}

fn same_operand(a: &ir::Value, b: &ir::Value) -> bool {
    a.ty == b.ty
        && match (&a.node.value, &b.node.value) {
            (ir::ValueKind::Constant(a), ir::ValueKind::Constant(b)) => a == b,
            (
                ir::ValueKind::Read {
                    place: a,
                    ordering: None,
                },
                ir::ValueKind::Read {
                    place: b,
                    ordering: None,
                },
            ) => same_place(a, b),
            (
                ir::ValueKind::ArrayDecay { place: a, .. },
                ir::ValueKind::ArrayDecay { place: b, .. },
            ) => same_place(a, b),
            (
                ir::ValueKind::PointerOffset {
                    pointer: a,
                    amount: i,
                    subtract: s,
                    ..
                },
                ir::ValueKind::PointerOffset {
                    pointer: b,
                    amount: j,
                    subtract: t,
                    ..
                },
            ) => s == t && same_operand(a, b) && same_operand(i, j),
            _ => false,
        }
}

pub(super) fn fold_compound_assignments(stmts: &mut [Stmt]) {
    for stmt in stmts {
        match stmt {
            Stmt::Scope { body } => fold_compound_assignments(body),
            Stmt::Block(body) => fold_compound_assignments(&mut body.stmts),
            _ => {
                let assignment = std::mem::replace(stmt, Stmt::Break(None));
                *stmt = compound_assignment(assignment);
            }
        }
    }
}

fn compound_assignment(assignment: Stmt) -> Stmt {
    match assignment {
        Stmt::Assign {
            target,
            value:
                Expr::Binary {
                    op:
                        op @ (rust::BinOp::Add
                        | rust::BinOp::Sub
                        | rust::BinOp::Mul
                        | rust::BinOp::Div
                        | rust::BinOp::Rem
                        | rust::BinOp::BitAnd
                        | rust::BinOp::BitOr
                        | rust::BinOp::BitXor
                        | rust::BinOp::Shl
                        | rust::BinOp::Shr),
                    lhs,
                    rhs,
                },
        } if *lhs == target => Stmt::CompoundAssign {
            target,
            op,
            value: *rhs,
        },
        other => other,
    }
}
