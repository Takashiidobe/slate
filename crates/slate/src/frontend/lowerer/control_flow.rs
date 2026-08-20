use super::*;

impl<'a, 'b> FunctionLowerer<'a, 'b> {
    pub(super) fn lower_return(&mut self, op: &TypedReturn) {
        self.lower_return_value(op.input.first());
    }

    pub(super) fn lower_return_raw(&mut self, op: &Op) {
        self.lower_return_value(op.operands.first());
    }

    fn lower_return_value(&mut self, operand: Option<&String>) {
        let value = operand.map(|operand| self.operand_expr(operand));
        if self.is_main {
            let code = value.unwrap_or(Expr::Value(RustValue::I64(0)));
            let dtor_stmts: Vec<Stmt> = self
                .parent
                .dtor_calls
                .iter()
                .map(|name| hook_call_stmt(name, &self.parent.unsafe_functions))
                .collect();
            for stmt in dtor_stmts {
                self.push_stmt(stmt);
            }
            self.push_stmt(Stmt::Expr(Expr::Call {
                binding: crate::function_identity::CallBinding::Generated,
                func: Box::new(Expr::Path(Path::new(
                    ["std", "process", "exit"].map(Ident::from),
                ))),
                args: vec![Expr::Cast {
                    expr: Box::new(code),
                    ty: Type::Prim(Prim::I32),
                }],
            }));
        } else if let Some(value) = value {
            self.push_stmt(Stmt::Return(Some(value)));
        } else {
            self.push_stmt(Stmt::Return(None));
        }
    }

    pub(super) fn lower_scope(&mut self, op: &Op) {
        let body = self.capture_body(|this| {
            for region in &op.regions {
                this.lower_region_ops(region);
            }
        });
        self.push_stmt(Stmt::Scope { body });
    }

    pub(super) fn lower_cleanup_scope(&mut self, op: &Op) {
        let Some(cleanup) = op.regions.get(1) else {
            self.emit_todo("cir.cleanup.scope");
            return;
        };
        let mut saw_stackrestore = false;
        for block in &cleanup.blocks {
            for cleanup_op in &block.ops {
                match cleanup_op.kind() {
                    CirOpKind::Load | CirOpKind::Yield => {}
                    CirOpKind::Stackrestore => saw_stackrestore = true,
                    _ => {
                        self.emit_todo("cir.cleanup.scope");
                        return;
                    }
                }
            }
        }
        if !saw_stackrestore {
            self.emit_todo("cir.cleanup.scope");
            return;
        }
        let body = self.capture_body(|this| {
            if let Some(region) = op.regions.first() {
                this.lower_region_ops(region);
            }
        });
        self.push_stmt(Stmt::Scope { body });
    }

    pub(super) fn lower_if(&mut self, op: &Op) {
        let Some(cond) = op.operands.first() else {
            self.emit_todo("cir.if");
            return;
        };
        let cond = self.operand_expr(cond);
        let then_body = self.capture_body(|this| {
            if let Some(region) = op.regions.first() {
                this.lower_region_ops(region);
            }
        });
        let has_else = op
            .regions
            .get(1)
            .is_some_and(|region| region.blocks.iter().any(|block| !block.ops.is_empty()));
        let else_body = if has_else {
            self.capture_body(|this| this.lower_region_ops(&op.regions[1]))
        } else {
            Vec::new()
        };
        self.push_stmt(Stmt::If {
            cond,
            then_body,
            else_body,
        });
    }

    pub(super) fn not_expr(expr: Expr) -> Expr {
        Expr::Unary {
            op: UnaryOp::Not,
            expr: Box::new(expr),
        }
    }

    pub(super) fn break_stmt(label: Option<Label>) -> IndentStmt {
        IndentStmt {
            depth: 0,
            stmt: Stmt::Break(label),
        }
    }

    pub(super) fn indent_stmt(stmt: Stmt) -> IndentStmt {
        IndentStmt { depth: 0, stmt }
    }

    pub(super) fn guard_break(cond: Expr, label: Option<Label>) -> Stmt {
        Stmt::If {
            cond: Self::not_expr(cond),
            then_body: vec![Self::break_stmt(label)],
            else_body: Vec::new(),
        }
    }

    pub(super) fn capture_body<F>(&mut self, f: F) -> Vec<IndentStmt>
    where
        F: FnOnce(&mut Self),
    {
        let outer_body = std::mem::take(&mut self.body);
        let outer_indent = self.indent;
        self.indent = 0;
        f(self);
        let body = std::mem::take(&mut self.body);
        self.body = outer_body;
        self.indent = outer_indent;
        body
    }

    pub(super) fn lower_condition_region_expr(&mut self, region: &Region) -> Expr {
        let mut condition = Expr::Value(RustValue::Bool(true));
        for block in &region.blocks {
            for op in &block.ops {
                if op.kind() == CirOpKind::Condition {
                    if let Some(operand) = op.operands.first() {
                        condition = self.operand_expr(operand);
                    }
                } else {
                    self.lower_op(op);
                }
            }
        }
        condition
    }

    pub(super) fn lower_for_loop_body(
        &mut self,
        op: &Op,
        break_label: Option<Label>,
        continue_label: Option<Label>,
    ) -> Vec<IndentStmt> {
        self.capture_body(|this| {
            let cond = this.lower_condition_region_expr(&op.regions[0]);
            this.push_stmt(Self::guard_break(cond, None));
            if let Some(label) = &continue_label {
                this.loop_stack.push(LoopFrame {
                    break_label: break_label.clone(),
                    continue_label: continue_label.clone(),
                    is_loop: true,
                });
                let body = this.capture_body(|this| this.lower_region_ops(&op.regions[1]));
                this.loop_stack.pop();
                this.push_stmt(Stmt::LabeledBlock {
                    label: label.clone(),
                    body,
                });
            } else {
                this.loop_stack.push(LoopFrame {
                    break_label,
                    continue_label: None,
                    is_loop: true,
                });
                this.lower_region_ops(&op.regions[1]);
                this.loop_stack.pop();
            }
            this.lower_region_ops(&op.regions[2]);
        })
    }

    pub(super) fn lower_do_loop_body(
        &mut self,
        op: &Op,
        break_label: Option<Label>,
        continue_label: Option<Label>,
    ) -> Vec<IndentStmt> {
        self.capture_body(|this| {
            if let Some(label) = &continue_label {
                let block = this.capture_body(|this| {
                    this.loop_stack.push(LoopFrame {
                        break_label: break_label.clone(),
                        continue_label: continue_label.clone(),
                        is_loop: true,
                    });
                    this.lower_region_ops(&op.regions[0]);
                    this.loop_stack.pop();
                });
                this.push_stmt(Stmt::LabeledBlock {
                    label: label.clone(),
                    body: block,
                });
            } else {
                this.loop_stack.push(LoopFrame {
                    break_label: break_label.clone(),
                    continue_label: None,
                    is_loop: true,
                });
                this.lower_region_ops(&op.regions[0]);
                this.loop_stack.pop();
            }
            let cond = this.lower_condition_region_expr(&op.regions[1]);
            this.push_stmt(Self::guard_break(cond, break_label));
        })
    }

    pub(super) fn lower_while_loop_body(
        &mut self,
        op: &Op,
        break_label: Option<Label>,
        continue_label: Option<Label>,
    ) -> Vec<IndentStmt> {
        self.capture_body(|this| {
            let cond = this.lower_condition_region_expr(&op.regions[0]);
            this.push_stmt(Self::guard_break(cond, None));
            if let Some(label) = &continue_label {
                this.loop_stack.push(LoopFrame {
                    break_label: break_label.clone(),
                    continue_label: continue_label.clone(),
                    is_loop: true,
                });
                let body = this.capture_body(|this| this.lower_region_ops(&op.regions[1]));
                this.loop_stack.pop();
                this.push_stmt(Stmt::LabeledBlock {
                    label: label.clone(),
                    body,
                });
            } else {
                this.loop_stack.push(LoopFrame {
                    break_label,
                    continue_label: None,
                    is_loop: true,
                });
                this.lower_region_ops(&op.regions[1]);
                this.loop_stack.pop();
            }
        })
    }

    pub(super) fn lower_switch(&mut self, op: &Op) {
        let Some(selector) = op.operands.first() else {
            self.emit_todo("cir.switch");
            return;
        };
        let Some(region) = op.regions.first() else {
            self.emit_todo("cir.switch");
            return;
        };
        let selector_rust_ty = op
            .operands
            .first()
            .and_then(|value| self.value_type(value))
            .map(|ty| self.parent.rust_type(ty));
        let bitint_ty = selector_rust_ty.filter(|ty| bitint_generic_parts(ty).is_some());
        if self.lower_duff_switch(selector, region, bitint_ty.as_ref()) {
            return;
        }
        let Some(cases): Option<Vec<_>> = region
            .blocks
            .iter()
            .flat_map(|block| &block.ops)
            .filter(|op| op.kind() == CirOpKind::Case)
            .map(|op| switch_case(op, bitint_ty.as_ref()))
            .collect()
        else {
            self.emit_todo("cir.switch: unrepresentable case value");
            return;
        };
        if cases.is_empty() {
            return;
        }

        let n = self.label_counter;
        self.label_counter += 1;
        let label = Label::new(format!("__switch{n}"));
        let selector_name = format!("__switch_value{n}");
        let case_name = format!("__switch_case{n}");
        let default_index = cases.iter().position(|case| case.is_default);
        let fallback = default_index.map(|index| index as i64).unwrap_or(-1);
        let selector = self.operand_expr(selector);

        let mut selector_arms = Vec::new();
        for (index, case) in cases.iter().enumerate() {
            for pattern in &case.patterns {
                selector_arms.push(ExprMatchArm {
                    pattern: pattern.clone(),
                    value: Expr::Value(RustValue::I64(index as i64)),
                });
            }
        }
        selector_arms.push(ExprMatchArm {
            pattern: Pattern::Wildcard,
            value: Expr::Value(RustValue::I64(fallback)),
        });

        let mut case_arms = Vec::new();
        self.loop_stack.push(LoopFrame {
            break_label: Some(label.clone()),
            continue_label: None,
            is_loop: false,
        });
        for (index, case) in cases.iter().enumerate() {
            let mut body = self.capture_body(|this| this.lower_region_ops(case.region));
            if !region_ends_control_flow(case.region) {
                if index + 1 < cases.len() {
                    body.push(Self::indent_stmt(Self::assign_stmt(
                        Expr::Var(case_name.clone().into()),
                        Expr::Value(RustValue::I64((index + 1) as i64)),
                    )));
                    body.push(Self::indent_stmt(Stmt::Continue(Some(label.clone()))));
                } else {
                    body.push(Self::indent_stmt(Stmt::Break(Some(label.clone()))));
                }
            }
            case_arms.push(MatchArm {
                pattern: int_pattern(index as i128),
                body,
            });
        }
        self.loop_stack.pop();
        case_arms.push(MatchArm {
            pattern: Pattern::Wildcard,
            body: vec![Self::indent_stmt(Stmt::Break(Some(label.clone())))],
        });

        let body = vec![
            Self::indent_stmt(Stmt::Let {
                name: selector_name.clone(),
                mutable: false,
                ty: None,
                init: Some(selector),
            }),
            Self::indent_stmt(Stmt::Let {
                name: case_name.clone(),
                mutable: true,
                ty: Some(Type::Prim(Prim::I32)),
                init: Some(Expr::Match {
                    expr: Box::new(Expr::Var(selector_name.into())),
                    arms: selector_arms,
                }),
            }),
            Self::indent_stmt(Stmt::Loop {
                label: Some(label),
                body: vec![Self::indent_stmt(Stmt::Match {
                    expr: Expr::Var(case_name.into()),
                    arms: case_arms,
                })],
            }),
        ];
        self.push_stmt(Stmt::Scope { body });
    }

    pub(super) fn lower_duff_switch(
        &mut self,
        selector: &str,
        region: &Region,
        bitint_ty: Option<&Type>,
    ) -> bool {
        let Some(duff) = duff_switch(region, bitint_ty) else {
            return false;
        };
        let n = self.label_counter;
        self.label_counter += 1;
        let case_name = format!("__switch_case{n}");
        let mut selector_arms = Vec::new();
        for (index, case) in duff.cases.iter().enumerate() {
            for pattern in &case.patterns {
                selector_arms.push(ExprMatchArm {
                    pattern: pattern.clone(),
                    value: Expr::Value(RustValue::I64(index as i64)),
                });
            }
        }
        selector_arms.push(ExprMatchArm {
            pattern: Pattern::Wildcard,
            value: Expr::Value(RustValue::I64(-1)),
        });
        let loop_body = self.capture_body(|this| {
            this.loop_stack.push(LoopFrame {
                break_label: None,
                continue_label: None,
                is_loop: true,
            });
            for index in 0..duff.cases.len() {
                let then_body = this.capture_body(|this| {
                    if index == 0 {
                        for op in &duff.prefix {
                            this.lower_op(op);
                            this.force_cross_block_materialization(op);
                        }
                    } else {
                        this.lower_region_ops(duff.cases[index].region);
                    }
                });
                this.push_stmt(Stmt::If {
                    cond: Expr::Binary {
                        op: BinOp::Le,
                        lhs: Box::new(Expr::Var(case_name.clone().into())),
                        rhs: Box::new(Expr::Value(RustValue::I64(index as i64))),
                    },
                    then_body,
                    else_body: Vec::new(),
                });
            }
            this.loop_stack.pop();
            this.push_assign(
                Expr::Var(case_name.clone().into()),
                Expr::Value(RustValue::I64(0)),
            );
            let condition = this.lower_condition_region_expr(duff.condition);
            this.push_stmt(Self::guard_break(condition, None));
        });
        self.push_stmt(Stmt::Scope {
            body: vec![
                Self::indent_stmt(Stmt::Let {
                    name: case_name.clone(),
                    mutable: true,
                    ty: Some(Type::Prim(Prim::I32)),
                    init: Some(Expr::Match {
                        expr: Box::new(self.operand_expr(selector)),
                        arms: selector_arms,
                    }),
                }),
                Self::indent_stmt(Stmt::If {
                    cond: Expr::Binary {
                        op: BinOp::Ge,
                        lhs: Box::new(Expr::Var(case_name.into())),
                        rhs: Box::new(Expr::Value(RustValue::I64(0))),
                    },
                    then_body: vec![Self::indent_stmt(Stmt::Loop {
                        label: None,
                        body: loop_body,
                    })],
                    else_body: Vec::new(),
                }),
            ],
        });
        true
    }

    pub(super) fn lower_for(&mut self, op: &Op) {
        if op.regions.len() < 3 {
            self.emit_todo("cir.for");
            return;
        }
        let (break_label, continue_label) = if region_has_direct_continue(&op.regions[1]) {
            let n = self.label_counter;
            self.label_counter += 1;
            (
                Some(Label::new(format!("__loop{n}"))),
                Some(Label::new(format!("__continue{n}"))),
            )
        } else {
            (None, None)
        };
        let body = self.lower_for_loop_body(op, break_label.clone(), continue_label);
        self.push_stmt(Stmt::Loop {
            label: break_label,
            body,
        });
    }

    pub(super) fn lower_while(&mut self, op: &Op) {
        if op.regions.len() < 2 {
            self.emit_todo("cir.while");
            return;
        }
        let (break_label, continue_label) = if region_has_direct_continue(&op.regions[1]) {
            let n = self.label_counter;
            self.label_counter += 1;
            (
                Some(Label::new(format!("__loop{n}"))),
                Some(Label::new(format!("__continue{n}"))),
            )
        } else {
            (None, None)
        };
        let body = self.lower_while_loop_body(op, break_label.clone(), continue_label);
        self.push_stmt(Stmt::Loop {
            label: break_label,
            body,
        });
    }

    pub(super) fn lower_do(&mut self, op: &Op) {
        if op.regions.len() < 2 {
            self.emit_todo("cir.do");
            return;
        }
        let (break_label, continue_label) = if region_has_direct_continue(&op.regions[0]) {
            let n = self.label_counter;
            self.label_counter += 1;
            (
                Some(Label::new(format!("__loop{n}"))),
                Some(Label::new(format!("__continue{n}"))),
            )
        } else {
            (None, None)
        };
        let body = self.lower_do_loop_body(op, break_label.clone(), continue_label);
        self.push_stmt(Stmt::Loop {
            label: break_label,
            body,
        });
    }

    pub(super) fn lower_break(&mut self) {
        let label = self.loop_stack.last().and_then(|f| f.break_label.clone());
        self.push_stmt(Stmt::Break(label));
    }

    pub(super) fn lower_continue(&mut self) {
        let label = self
            .loop_stack
            .iter()
            .rev()
            .find(|frame| frame.is_loop)
            .and_then(|f| f.continue_label.clone());
        match label {
            Some(label) => self.push_stmt(Stmt::Break(Some(label))),
            None => self.push_stmt(Stmt::Continue(None)),
        }
    }

    pub(super) fn lower_dispatch(&mut self, body: &Region, returns_value: bool) {
        let n = self.label_counter;
        self.label_counter += 1;
        let loop_label = Label::new(format!("__dispatch{n}"));
        let state_var = format!("__state{n}");

        let mut label_to_state = BTreeMap::new();
        let mut block_to_state = BTreeMap::new();
        for (i, block) in body.blocks.iter().enumerate() {
            let key = block.label.clone().unwrap_or_else(|| format!("bb{i}"));
            block_to_state.insert(key, i);
            for op in &block.ops {
                if op.kind() == CirOpKind::Label
                    && let Some(label) = attr_str(op, "label")
                {
                    label_to_state.insert(label.to_string(), i);
                }
            }
        }
        self.dispatch = Some(DispatchCtx {
            loop_label: loop_label.clone(),
            state_var: state_var.clone(),
            label_to_state,
            block_to_state,
            cross_block_names: BTreeMap::new(),
            block_args: BTreeMap::new(),
            pending_hoists: Vec::new(),
        });
        self.hoisting_allocas = true;
        for block in &body.blocks {
            for op in &block.ops {
                if op.kind() == CirOpKind::Alloca
                    && let Some(TypedOp::Alloca(alloca)) = TypedOp::from_operation(op)
                {
                    self.lower_alloca(&alloca);
                    self.hoisted.insert(alloca.addr);
                }
            }
        }
        self.hoisting_allocas = false;
        for ssa in &cross_block_live_values(body) {
            if self.hoisted.contains(ssa) {
                continue;
            }
            let name = self.next_temp();
            self.values
                .insert(ssa.clone(), Val::Expr(Expr::Var(name.clone().into())));
            self.dispatch
                .as_mut()
                .unwrap()
                .cross_block_names
                .insert(ssa.clone(), name);
        }
        for (i, block) in body.blocks.iter().enumerate() {
            if i == 0 || block.args.is_empty() {
                continue;
            }
            let mut names = Vec::new();
            for (arg_ssa, arg_ty) in &block.args {
                let name = self.next_temp();
                let ty = self.parent.rust_type(arg_ty);
                let default = self.parent.default_value_expr(&ty);
                self.values
                    .insert(arg_ssa.clone(), Val::Expr(Expr::Var(name.clone().into())));
                self.dispatch
                    .as_mut()
                    .unwrap()
                    .pending_hoists
                    .push(Self::indent_stmt(Stmt::Let {
                        name: name.clone(),
                        mutable: true,
                        ty: Some(ty),
                        init: Some(default),
                    }));
                names.push(name);
            }
            self.dispatch.as_mut().unwrap().block_args.insert(i, names);
        }

        let mut arms = Vec::new();
        for (i, block) in body.blocks.iter().enumerate() {
            let mut body = self.capture_body(|this| this.lower_block(block));
            if !block_diverges(block) {
                body.push(Self::indent_stmt(Self::assign_stmt(
                    Expr::Var(state_var.clone().into()),
                    Expr::Value(RustValue::I64((i + 1) as i64)),
                )));
                body.push(Self::indent_stmt(Stmt::Continue(Some(loop_label.clone()))));
            }
            arms.push(MatchArm {
                pattern: int_pattern(i as i128),
                body,
            });
        }
        let fallthrough = if returns_value {
            Stmt::Expr(Expr::Macro {
                name: "unreachable".into(),
                args: Vec::new(),
            })
        } else {
            Stmt::Break(Some(loop_label.clone()))
        };
        arms.push(MatchArm {
            pattern: Pattern::Wildcard,
            body: vec![Self::indent_stmt(fallthrough)],
        });
        let pending_hoists = std::mem::take(&mut self.dispatch.as_mut().unwrap().pending_hoists);
        for hoist in pending_hoists {
            self.body.push(hoist);
        }
        self.push_stmt(Stmt::Let {
            name: state_var.clone(),
            mutable: true,
            ty: Some(Type::Prim(Prim::I32)),
            init: Some(Expr::Value(RustValue::I64(0))),
        });
        self.push_stmt(Stmt::Loop {
            label: Some(loop_label),
            body: vec![Self::indent_stmt(Stmt::Match {
                expr: Expr::Var(state_var.into()),
                arms,
            })],
        });
        self.dispatch = None;
    }

    pub(super) fn lower_goto(&mut self, op: &Op) {
        let Some(dispatch) = &self.dispatch else {
            return;
        };
        let target = attr_str(op, "label")
            .and_then(|l| dispatch.label_to_state.get(l))
            .map(|state| {
                (
                    *state,
                    dispatch.state_var.clone(),
                    dispatch.loop_label.clone(),
                )
            });
        match target {
            Some((state, state_var, loop_label)) => {
                self.push_assign(
                    Expr::Var(state_var.into()),
                    Expr::Value(RustValue::I64(state as i64)),
                );
                self.push_stmt(Stmt::Continue(Some(loop_label)));
            }
            None => self.emit_todo("cir.goto: unknown label"),
        }
    }

    pub(super) fn lower_br(&mut self, op: &TypedBr) {
        self.lower_br_impl(&op.dest_operands, &op.successors);
    }

    fn lower_br_impl(&mut self, operands: &[String], successors: &[String]) {
        let Some(dispatch) = &self.dispatch else {
            return;
        };
        if let Some(target) = operands
            .first()
            .and_then(|operand| self.indirect_target_values.get(operand))
        {
            let state_var = dispatch.state_var.clone();
            let loop_label = dispatch.loop_label.clone();
            let target = target.clone();
            self.push_assign(Expr::Var(state_var.into()), target);
            self.push_stmt(Stmt::Continue(Some(loop_label)));
            return;
        }
        let target = successors
            .first()
            .and_then(|bb| dispatch.block_to_state.get(bb))
            .map(|state| {
                (
                    *state,
                    dispatch.state_var.clone(),
                    dispatch.loop_label.clone(),
                    dispatch.block_args.get(state).cloned(),
                )
            });
        match target {
            Some((state, state_var, loop_label, arg_names)) => {
                if let Some(names) = arg_names {
                    for (name, operand) in names.iter().zip(operands.iter()) {
                        let value = self.operand_expr(operand);
                        self.push_stmt(Self::assign_stmt(Expr::Var(name.clone().into()), value));
                    }
                }
                self.push_assign(
                    Expr::Var(state_var.into()),
                    Expr::Value(RustValue::I64(state as i64)),
                );
                self.push_stmt(Stmt::Continue(Some(loop_label)));
            }
            None => self.emit_todo("cir.br: unknown successor"),
        }
    }

    pub(super) fn lower_indirect_br(&mut self, op: &Op) {
        let Some(dispatch) = &self.dispatch else {
            return;
        };
        if let Some(target) = op
            .operands
            .first()
            .and_then(|operand| self.indirect_target_values.get(operand))
        {
            let state_var = dispatch.state_var.clone();
            let loop_label = dispatch.loop_label.clone();
            let target = target.clone();
            self.push_assign(Expr::Var(state_var.into()), target);
            self.push_stmt(Stmt::Continue(Some(loop_label)));
        } else {
            self.lower_unreachable();
        }
    }

    pub(super) fn lower_brcond(&mut self, op: &Op) {
        let Some(dispatch) = &self.dispatch else {
            self.emit_todo("cir.brcond: not in dispatch context");
            return;
        };
        let Some(cond) = op.operands.first() else {
            self.emit_todo("cir.brcond: missing operand");
            return;
        };
        let cond_expr = self.operand_expr(cond);
        let [true_bb, false_bb] = op.successors.as_slice() else {
            self.emit_todo("cir.brcond: expected two successors");
            return;
        };
        let (Some(true_state), Some(false_state)) = (
            dispatch.block_to_state.get(true_bb).copied(),
            dispatch.block_to_state.get(false_bb).copied(),
        ) else {
            self.emit_todo("cir.brcond: unknown successor");
            return;
        };
        let state_var = dispatch.state_var.clone();
        let loop_label = dispatch.loop_label.clone();
        let true_args = dispatch.block_args.get(&true_state).cloned();
        let false_args = dispatch.block_args.get(&false_state).cloned();
        let mut groups = successor_operand_groups(op).into_iter();
        let true_ops = groups.next().unwrap_or_default();
        let false_ops = groups.next().unwrap_or_default();

        let mut then_body = Vec::new();
        if let Some(names) = &true_args {
            for (name, operand) in names.iter().zip(true_ops.iter()) {
                let value = self.operand_expr(operand);
                then_body.push(Self::indent_stmt(Self::assign_stmt(
                    Expr::Var(name.clone().into()),
                    value,
                )));
            }
        }
        then_body.push(Self::indent_stmt(Self::assign_stmt(
            Expr::Var(state_var.clone().into()),
            Expr::Value(RustValue::I64(true_state as i64)),
        )));
        let mut else_body = Vec::new();
        if let Some(names) = &false_args {
            for (name, operand) in names.iter().zip(false_ops.iter()) {
                let value = self.operand_expr(operand);
                else_body.push(Self::indent_stmt(Self::assign_stmt(
                    Expr::Var(name.clone().into()),
                    value,
                )));
            }
        }
        else_body.push(Self::indent_stmt(Self::assign_stmt(
            Expr::Var(state_var.into()),
            Expr::Value(RustValue::I64(false_state as i64)),
        )));
        self.push_stmt(Stmt::If {
            cond: cond_expr,
            then_body,
            else_body,
        });
        self.push_stmt(Stmt::Continue(Some(loop_label)));
    }

    pub(super) fn lower_switch_flat(&mut self, op: &Op) {
        let Some(dispatch) = &self.dispatch else {
            self.emit_todo("cir.switch.flat: not in dispatch context");
            return;
        };
        let Some(selector) = op.operands.first() else {
            self.emit_todo("cir.switch.flat: missing operand");
            return;
        };
        let selector_expr = self.operand_expr(selector);
        let Some((default_bb, case_bbs)) = op.successors.split_first() else {
            self.emit_todo("cir.switch.flat: missing successors");
            return;
        };
        let Some(default_state) = dispatch.block_to_state.get(default_bb).copied() else {
            self.emit_todo("cir.switch.flat: unknown default successor");
            return;
        };
        let selector_rust_ty = op
            .operands
            .first()
            .and_then(|value| self.value_type(value))
            .map(|ty| self.parent.rust_type(ty));
        let bitint_ty = selector_rust_ty.filter(|ty| bitint_generic_parts(ty).is_some());
        let Some(case_patterns) = switch_flat_case_patterns(op, bitint_ty.as_ref()) else {
            self.emit_todo("cir.switch.flat: unrepresentable case value");
            return;
        };
        let state_var = dispatch.state_var.clone();
        let loop_label = dispatch.loop_label.clone();
        let mut case_states = Vec::new();
        for bb in case_bbs {
            let Some(state) = dispatch.block_to_state.get(bb).copied() else {
                self.emit_todo("cir.switch.flat: unknown case successor");
                return;
            };
            case_states.push(state);
        }
        let default_args = dispatch.block_args.get(&default_state).cloned();
        let case_args: Vec<Option<Vec<String>>> = case_states
            .iter()
            .map(|state| dispatch.block_args.get(state).cloned())
            .collect();
        let mut operand_groups = successor_operand_groups(op).into_iter();
        let default_ops = operand_groups.next().unwrap_or_default();
        let mut arms = Vec::new();
        for ((pattern, state), (names, ops)) in case_patterns
            .iter()
            .zip(case_states.iter())
            .zip(case_args.iter().zip(operand_groups))
        {
            let mut body = Vec::new();
            if let Some(names) = names {
                for (name, operand) in names.iter().zip(ops.iter()) {
                    let value_expr = self.operand_expr(operand);
                    body.push(Self::indent_stmt(Self::assign_stmt(
                        Expr::Var(name.clone().into()),
                        value_expr,
                    )));
                }
            }
            body.push(Self::indent_stmt(Self::assign_stmt(
                Expr::Var(state_var.clone().into()),
                Expr::Value(RustValue::I64(*state as i64)),
            )));
            arms.push(MatchArm {
                pattern: pattern.clone(),
                body,
            });
        }
        let mut default_body = Vec::new();
        if let Some(names) = &default_args {
            for (name, operand) in names.iter().zip(default_ops.iter()) {
                let value_expr = self.operand_expr(operand);
                default_body.push(Self::indent_stmt(Self::assign_stmt(
                    Expr::Var(name.clone().into()),
                    value_expr,
                )));
            }
        }
        default_body.push(Self::indent_stmt(Self::assign_stmt(
            Expr::Var(state_var.into()),
            Expr::Value(RustValue::I64(default_state as i64)),
        )));
        arms.push(MatchArm {
            pattern: Pattern::Wildcard,
            body: default_body,
        });
        self.push_stmt(Stmt::Match {
            expr: selector_expr,
            arms,
        });
        self.push_stmt(Stmt::Continue(Some(loop_label)));
    }
}
