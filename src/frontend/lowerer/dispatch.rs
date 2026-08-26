use super::*;

impl<'a, 'b> FunctionLowerer<'a, 'b> {
    pub(super) fn ast_floating_literal(
        &self,
        loc: Option<&SourceLocation>,
    ) -> Option<FloatingLiteralFact> {
        self.parent.floating_literal_at_source_location(loc?)
    }

    pub(super) fn lower_block(&mut self, block: &inst::Block) {
        self.value_types.extend(block.args.iter().cloned());
        let mut index = 0;
        while index < block.ops.len() {
            if self.needs_alloca_layout_preservation && matches!(block.ops[index], Op::Alloca(_)) {
                let end = block.ops[index..]
                    .iter()
                    .take_while(|candidate| matches!(candidate, Op::Alloca(_)))
                    .count()
                    + index;
                if end - index > 1 {
                    let allocas: Vec<_> = block.ops[index..end]
                        .iter()
                        .filter_map(|candidate| match candidate {
                            Op::Alloca(alloca) => Some(alloca.clone()),
                            _ => None,
                        })
                        .collect();
                    if self.alloca_group_is_lowerable(&allocas) {
                        self.lower_alloca_group(&allocas);
                        index = end;
                        continue;
                    }
                }
            }
            self.asm_output_places.clear();
            if let Op::Asm(asm) = &block.ops[index]
                && asm_template_has_labels(&asm.asm_string)
                && let Some(result) = &asm.res
                && let Some(store) = block.ops[index + 1..].iter().find_map(|candidate| {
                    let Op::Store(store) = candidate else {
                        return None;
                    };
                    (store.value == *result).then_some(store)
                })
                && let Some(place) = self.place_expr(&store.addr)
            {
                self.asm_output_places.insert(result.clone(), place);
            }
            let op = block.ops[index].clone();
            self.lower_op(op.clone());
            self.force_cross_block_materialization(&op);
            index += 1;
        }
    }

    pub(super) fn force_cross_block_materialization(&mut self, op: &Op) {
        if self.dispatch.is_none() {
            return;
        }
        let mut results = Vec::new();
        op.for_each_result(|id, ty| results.push((id.clone(), ty.clone())));
        for (result, result_ty) in results {
            let Some(name) = self
                .dispatch
                .as_ref()
                .unwrap()
                .cross_block_names
                .get(&result)
                .cloned()
            else {
                continue;
            };
            let already_materialized = matches!(
                self.values.get(&result),
                Some(Val::Expr(Expr::Var(v))) if v.as_str() == name
            );
            if already_materialized {
                continue;
            }
            let Some(Val::Expr(current)) = self.values.get(&result).cloned() else {
                continue;
            };
            let ty = self.parent.rust_type(&result_ty);
            let default = self.parent.default_value_expr(&ty);
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
            self.push_stmt(Self::assign_stmt(Expr::Var(name.clone().into()), current));
            self.values
                .insert(result, Val::Expr(Expr::Var(name.into())));
        }
    }

    pub(super) fn lower_region_ops(&mut self, region: &inst::Region) {
        for block in &region.blocks {
            self.lower_block(block);
        }
    }

    pub(super) fn lower_op(&mut self, op: Op) {
        self.record_result_types(&op);
        let () = {
            match op {
                Op::Add(value) if value.saturated => {
                    return self.lower_saturating_arith(
                        &value.result,
                        &value.result_ty,
                        &value.lhs,
                        &value.rhs,
                        "saturating_add",
                    );
                }
                Op::Add(value) => {
                    return self.lower_int_arith(
                        &value.result,
                        Some(&value.result_ty),
                        &value.lhs,
                        &value.rhs,
                        BinOp::Add,
                    );
                }
                Op::Sub(value) if value.saturated => {
                    return self.lower_saturating_arith(
                        &value.result,
                        &value.result_ty,
                        &value.lhs,
                        &value.rhs,
                        "saturating_sub",
                    );
                }
                Op::Sub(value) => {
                    return self.lower_int_arith(
                        &value.result,
                        Some(&value.result_ty),
                        &value.lhs,
                        &value.rhs,
                        BinOp::Sub,
                    );
                }
                Op::Mul(value) => {
                    return self.lower_int_arith(
                        &value.result,
                        Some(&value.result_ty),
                        &value.lhs,
                        &value.rhs,
                        BinOp::Mul,
                    );
                }
                Op::Div(value) => {
                    return self.lower_int_arith(
                        &value.result,
                        Some(&value.result_ty),
                        &value.lhs,
                        &value.rhs,
                        BinOp::Div,
                    );
                }
                Op::Rem(value) => {
                    return self.lower_int_arith(
                        &value.result,
                        Some(&value.result_ty),
                        &value.lhs,
                        &value.rhs,
                        BinOp::Rem,
                    );
                }
                Op::And(value) => {
                    return self.lower_int_arith(
                        &value.result,
                        Some(&value.result_ty),
                        &value.lhs,
                        &value.rhs,
                        BinOp::BitAnd,
                    );
                }
                Op::Or(value) => {
                    return self.lower_int_arith(
                        &value.result,
                        Some(&value.result_ty),
                        &value.lhs,
                        &value.rhs,
                        BinOp::BitOr,
                    );
                }
                Op::Xor(value) => {
                    return self.lower_int_arith(
                        &value.result,
                        Some(&value.result_ty),
                        &value.lhs,
                        &value.rhs,
                        BinOp::BitXor,
                    );
                }
                Op::Fadd(value) => {
                    return self.lower_fenv_binary(
                        &value.result,
                        Some(&value.result_ty),
                        &value.lhs,
                        &value.rhs,
                        BinOp::Add,
                        fenv_is_constrained(&value.fenv),
                    );
                }
                Op::Fsub(value) => {
                    return self.lower_fenv_binary(
                        &value.result,
                        Some(&value.result_ty),
                        &value.lhs,
                        &value.rhs,
                        BinOp::Sub,
                        fenv_is_constrained(&value.fenv),
                    );
                }
                Op::Fmul(value) => {
                    return self.lower_fenv_binary(
                        &value.result,
                        Some(&value.result_ty),
                        &value.lhs,
                        &value.rhs,
                        BinOp::Mul,
                        fenv_is_constrained(&value.fenv),
                    );
                }
                Op::Fdiv(value) => {
                    return self.lower_fenv_binary(
                        &value.result,
                        Some(&value.result_ty),
                        &value.lhs,
                        &value.rhs,
                        BinOp::Div,
                        fenv_is_constrained(&value.fenv),
                    );
                }
                Op::Inc(value) => {
                    return self.lower_step(
                        &value.result,
                        &value.result_ty,
                        &value.input,
                        BinOp::Add,
                    );
                }
                Op::Dec(value) => {
                    return self.lower_step(
                        &value.result,
                        &value.result_ty,
                        &value.input,
                        BinOp::Sub,
                    );
                }
                Op::Not(value) => {
                    return self.lower_not(&value.result, &value.result_ty, &value.input);
                }
                Op::Shift(value) => {
                    return self.lower_int_arith(
                        &value.result,
                        Some(&value.result_ty),
                        &value.value,
                        &value.amount,
                        if value.is_shiftleft {
                            BinOp::Shl
                        } else {
                            BinOp::Shr
                        },
                    );
                }
                Op::Rotate(value) => return self.lower_rotate(&value),
                Op::AddOverflow(value) => {
                    return self.lower_overflow_arith(
                        (
                            &value.result,
                            &value.result_ty,
                            &value.overflow,
                            &value.overflow_ty,
                        ),
                        &value.lhs,
                        &value.rhs,
                        "overflowing_add",
                    );
                }
                Op::SubOverflow(value) => {
                    return self.lower_overflow_arith(
                        (
                            &value.result,
                            &value.result_ty,
                            &value.overflow,
                            &value.overflow_ty,
                        ),
                        &value.lhs,
                        &value.rhs,
                        "overflowing_sub",
                    );
                }
                Op::MulOverflow(value) => {
                    return self.lower_overflow_arith(
                        (
                            &value.result,
                            &value.result_ty,
                            &value.overflow,
                            &value.overflow_ty,
                        ),
                        &value.lhs,
                        &value.rhs,
                        "overflowing_mul",
                    );
                }
                Op::Acos(value) => {
                    return self.lower_unary_method(
                        &value.result,
                        Some(&value.result_ty),
                        &value.src,
                        "acos",
                    );
                }
                Op::Asin(value) => {
                    return self.lower_unary_method(
                        &value.result,
                        Some(&value.result_ty),
                        &value.src,
                        "asin",
                    );
                }
                Op::Atan(value) => {
                    return self.lower_unary_method(
                        &value.result,
                        Some(&value.result_ty),
                        &value.src,
                        "atan",
                    );
                }
                Op::Cos(value) => {
                    let fenv = fenv_is_constrained(&value.fenv);
                    return self.lower_fenv_unary(
                        &value.result,
                        &value.result_ty,
                        &value.src,
                        "cos",
                        fenv,
                        |this| {
                            this.lower_known_unary_method(
                                &value.result,
                                &value.result_ty,
                                &value.src,
                                value.loc.as_ref(),
                                Known::Cos,
                                "cos",
                            );
                        },
                    );
                }
                Op::Exp(value) => {
                    let fenv = fenv_is_constrained(&value.fenv);
                    return self.lower_fenv_unary(
                        &value.result,
                        &value.result_ty,
                        &value.src,
                        "exp",
                        fenv,
                        |this| {
                            this.lower_known_unary_method(
                                &value.result,
                                &value.result_ty,
                                &value.src,
                                value.loc.as_ref(),
                                Known::Exp,
                                "exp",
                            );
                        },
                    );
                }
                Op::Exp2(value) => {
                    let fenv = fenv_is_constrained(&value.fenv);
                    return self.lower_fenv_unary(
                        &value.result,
                        &value.result_ty,
                        &value.src,
                        "exp2",
                        fenv,
                        |this| {
                            this.lower_known_unary_method(
                                &value.result,
                                &value.result_ty,
                                &value.src,
                                value.loc.as_ref(),
                                Known::Exp2,
                                "exp2",
                            );
                        },
                    );
                }
                Op::Log(value) => {
                    let fenv = fenv_is_constrained(&value.fenv);
                    return self.lower_fenv_unary(
                        &value.result,
                        &value.result_ty,
                        &value.src,
                        "log",
                        fenv,
                        |this| {
                            this.lower_known_unary_method(
                                &value.result,
                                &value.result_ty,
                                &value.src,
                                value.loc.as_ref(),
                                Known::Log,
                                "ln",
                            );
                        },
                    );
                }
                Op::Log10(value) => {
                    let fenv = fenv_is_constrained(&value.fenv);
                    return self.lower_fenv_unary(
                        &value.result,
                        &value.result_ty,
                        &value.src,
                        "log10",
                        fenv,
                        |this| {
                            this.lower_known_unary_method(
                                &value.result,
                                &value.result_ty,
                                &value.src,
                                value.loc.as_ref(),
                                Known::Log10,
                                "log10",
                            );
                        },
                    );
                }
                Op::Log2(value) => {
                    let fenv = fenv_is_constrained(&value.fenv);
                    return self.lower_fenv_unary(
                        &value.result,
                        &value.result_ty,
                        &value.src,
                        "log2",
                        fenv,
                        |this| {
                            this.lower_known_unary_method(
                                &value.result,
                                &value.result_ty,
                                &value.src,
                                value.loc.as_ref(),
                                Known::Log2,
                                "log2",
                            );
                        },
                    );
                }
                Op::Sin(value) => {
                    let fenv = fenv_is_constrained(&value.fenv);
                    return self.lower_fenv_unary(
                        &value.result,
                        &value.result_ty,
                        &value.src,
                        "sin",
                        fenv,
                        |this| {
                            this.lower_known_unary_method(
                                &value.result,
                                &value.result_ty,
                                &value.src,
                                value.loc.as_ref(),
                                Known::Sin,
                                "sin",
                            );
                        },
                    );
                }
                Op::Sqrt(value) => {
                    let fenv = fenv_is_constrained(&value.fenv);
                    return self.lower_fenv_unary(
                        &value.result,
                        &value.result_ty,
                        &value.src,
                        "sqrt",
                        fenv,
                        |this| {
                            this.lower_known_unary_method(
                                &value.result,
                                &value.result_ty,
                                &value.src,
                                value.loc.as_ref(),
                                Known::Sqrt,
                                "sqrt",
                            );
                        },
                    );
                }
                Op::Tan(value) => {
                    return self.lower_known_unary_method(
                        &value.result,
                        &value.result_ty,
                        &value.src,
                        value.loc.as_ref(),
                        Known::Tan,
                        "tan",
                    );
                }
                Op::Const(value) => return self.lower_const(&value),
                Op::Alloca(value) => return self.lower_alloca(&value),
                Op::GetGlobal(value) => return self.lower_get_global(&value),
                Op::GetMember(value) => return self.lower_get_member(&value),
                Op::Asm(value) => return self.lower_asm(&value),
                Op::Goto(value) => return self.lower_goto(&value),
                Op::IndirectBr(value) => return self.lower_indirect_br(&value),
                Op::IndirectGoto(value) => return self.lower_indirect_goto(&value),
                Op::Label(_) | Op::Condition(_) => return,
                Op::Load(value) => return self.lower_load(&value),
                Op::Br(value) => return self.lower_br(&value),
                Op::Call(value) => return self.lower_call(&value),
                Op::Brcond(value) => return self.lower_brcond(&value),
                Op::Expect(value) => return self.lower_expect(&value),
                Op::Trap(_) => return self.lower_trap(),
                Op::Unreachable(_) => return self.lower_unreachable(),
                Op::Scope(value) => return self.lower_scope(&value),
                Op::Switch(value) => return self.lower_switch(&value),
                Op::SwitchFlat(value) => return self.lower_switch_flat(&value),
                Op::CleanupScope(value) => return self.lower_cleanup_scope(&value),
                Op::Stackrestore(_) => return,
                Op::Ternary(value) => return self.lower_ternary(&value),
                Op::For(value) => return self.lower_for(&value),
                Op::While(value) => return self.lower_while(&value),
                Op::Do(value) => return self.lower_do(&value),
                Op::Break(_) => return self.lower_break(),
                Op::Continue(_) => return self.lower_continue(),
                Op::Store(value) => return self.lower_store(&value),
                Op::Copy(value) => return self.lower_copy(&value),
                Op::Cast(value) => return self.lower_cast(&value),
                Op::GetElement(value) => return self.lower_get_element(&value),
                Op::PtrStride(value) => return self.lower_ptr_stride(&value),
                Op::PtrDiff(value) => return self.lower_ptr_diff(&value),
                Op::Cmp(value) => return self.lower_cmp(&value),
                Op::Select(value) => return self.lower_select(&value),
                Op::Abs(value) => {
                    return self.lower_abs(&value.result, Some(&value.result_ty), &value.src);
                }
                Op::Fabs(value) => {
                    let fenv = fenv_is_constrained(&value.fenv);
                    return self.lower_fenv_unary(
                        &value.result,
                        &value.result_ty,
                        &value.src,
                        "fabs",
                        fenv,
                        |this| {
                            this.lower_abs(&value.result, Some(&value.result_ty), &value.src);
                        },
                    );
                }
                Op::Bitreverse(value) => {
                    return self.lower_unary_method(
                        &value.result,
                        Some(&value.result_ty),
                        &value.input,
                        "reverse_bits",
                    );
                }
                Op::ByteSwap(value) => {
                    return self.lower_unary_method(
                        &value.result,
                        Some(&value.result_ty),
                        &value.input,
                        "swap_bytes",
                    );
                }
                Op::Ceil(value) => {
                    let fenv = fenv_is_constrained(&value.fenv);
                    return self.lower_fenv_unary(
                        &value.result,
                        &value.result_ty,
                        &value.src,
                        "ceil",
                        fenv,
                        |this| {
                            this.lower_unary_method(
                                &value.result,
                                Some(&value.result_ty),
                                &value.src,
                                "ceil",
                            );
                        },
                    );
                }
                Op::Clz(value) => {
                    return self.lower_unary_method(
                        &value.result,
                        Some(&value.result_ty),
                        &value.input,
                        "leading_zeros",
                    );
                }
                Op::Ctz(value) => {
                    return self.lower_unary_method(
                        &value.result,
                        Some(&value.result_ty),
                        &value.input,
                        "trailing_zeros",
                    );
                }
                Op::Floor(value) => {
                    let fenv = fenv_is_constrained(&value.fenv);
                    return self.lower_fenv_unary(
                        &value.result,
                        &value.result_ty,
                        &value.src,
                        "floor",
                        fenv,
                        |this| {
                            this.lower_unary_method(
                                &value.result,
                                Some(&value.result_ty),
                                &value.src,
                                "floor",
                            );
                        },
                    );
                }
                Op::Nearbyint(value) => {
                    let fenv = fenv_is_constrained(&value.fenv);
                    return self.lower_fenv_unary(
                        &value.result,
                        &value.result_ty,
                        &value.src,
                        "nearbyint",
                        fenv,
                        |this| {
                            this.lower_unary_method(
                                &value.result,
                                Some(&value.result_ty),
                                &value.src,
                                "round_ties_even",
                            );
                        },
                    );
                }
                Op::Rint(value) => {
                    let fenv = fenv_is_constrained(&value.fenv);
                    return self.lower_fenv_unary(
                        &value.result,
                        &value.result_ty,
                        &value.src,
                        "rint",
                        fenv,
                        |this| {
                            this.lower_unary_method(
                                &value.result,
                                Some(&value.result_ty),
                                &value.src,
                                "round_ties_even",
                            );
                        },
                    );
                }
                Op::Popcount(value) => {
                    return self.lower_unary_method(
                        &value.result,
                        Some(&value.result_ty),
                        &value.input,
                        "count_ones",
                    );
                }
                Op::Round(value) => {
                    let fenv = fenv_is_constrained(&value.fenv);
                    return self.lower_fenv_unary(
                        &value.result,
                        &value.result_ty,
                        &value.src,
                        "round",
                        fenv,
                        |this| {
                            this.lower_unary_method(
                                &value.result,
                                Some(&value.result_ty),
                                &value.src,
                                "round",
                            );
                        },
                    );
                }
                Op::Roundeven(value) => {
                    let fenv = fenv_is_constrained(&value.fenv);
                    return self.lower_fenv_unary(
                        &value.result,
                        &value.result_ty,
                        &value.src,
                        "roundeven",
                        fenv,
                        |this| {
                            this.lower_unary_method(
                                &value.result,
                                Some(&value.result_ty),
                                &value.src,
                                "round_ties_even",
                            );
                        },
                    );
                }
                Op::Trunc(value) => {
                    let fenv = fenv_is_constrained(&value.fenv);
                    return self.lower_fenv_unary(
                        &value.result,
                        &value.result_ty,
                        &value.src,
                        "trunc",
                        fenv,
                        |this| {
                            this.lower_unary_method(
                                &value.result,
                                Some(&value.result_ty),
                                &value.src,
                                "trunc",
                            );
                        },
                    );
                }
                Op::Minus(value) => {
                    return self.lower_neg(&value.result, &value.result_ty, &value.input);
                }
                Op::Fneg(value) => {
                    return self.lower_neg(&value.result, &value.result_ty, &value.input);
                }
                Op::Parity(value) => {
                    return self.lower_parity(&value.result, Some(&value.result_ty), &value.input);
                }
                Op::Assume(value) => return self.lower_assume(&value),
                Op::If(value) => return self.lower_if(&value),
                Op::Ffs(value) => {
                    return self.lower_ffs(&value.result, Some(&value.result_ty), &value.input);
                }
                Op::Clrsb(value) => {
                    return self.lower_clrsb(&value.result, Some(&value.result_ty), &value.input);
                }
                Op::Signbit(value) => {
                    return self.lower_signbit(&value.res, &value.res_ty, &value.input);
                }
                Op::Atan2(value) => {
                    return self.lower_binary_method(
                        &value.result,
                        Some(&value.result_ty),
                        &value.lhs,
                        &value.rhs,
                        "atan2",
                    );
                }
                Op::Fmaximum(value) => {
                    return self.lower_binary_method(
                        &value.result,
                        Some(&value.result_ty),
                        &value.lhs,
                        &value.rhs,
                        "max",
                    );
                }
                Op::Fminimum(value) => {
                    return self.lower_binary_method(
                        &value.result,
                        Some(&value.result_ty),
                        &value.lhs,
                        &value.rhs,
                        "min",
                    );
                }
                Op::Fmod(value) => {
                    if self.lower_known_libc_binary(
                        &value.result,
                        &value.result_ty,
                        &value.lhs,
                        &value.rhs,
                        value.loc.as_ref(),
                        Known::Fmod,
                    ) {
                        self.materialize_expr(
                            &value.result,
                            Expr::Binary {
                                op: BinOp::Rem,
                                lhs: Box::new(self.operand_expr(&value.lhs)),
                                rhs: Box::new(self.operand_expr(&value.rhs)),
                            },
                            Some(&value.result_ty),
                        );
                    }
                    return;
                }
                Op::Pow(value) => {
                    let fenv = fenv_is_constrained(&value.fenv);
                    return self.lower_fenv_binary_op(
                        &value.result,
                        &value.result_ty,
                        (&value.lhs, &value.rhs),
                        "pow",
                        fenv,
                        |this| {
                            this.lower_known_binary_method(
                                &value.result,
                                &value.result_ty,
                                (&value.lhs, &value.rhs),
                                value.loc.as_ref(),
                                Known::Pow,
                                "powf",
                            );
                        },
                    );
                }
                Op::Copysign(value) => {
                    let fenv = fenv_is_constrained(&value.fenv);
                    return self.lower_fenv_binary_op(
                        &value.result,
                        &value.result_ty,
                        (&value.lhs, &value.rhs),
                        "copysign",
                        fenv,
                        |this| {
                            this.lower_binary_method(
                                &value.result,
                                Some(&value.result_ty),
                                &value.lhs,
                                &value.rhs,
                                "copysign",
                            );
                        },
                    );
                }
                Op::Fmaxnum(value) => {
                    let fenv = fenv_is_constrained(&value.fenv);
                    return self.lower_fenv_binary_op(
                        &value.result,
                        &value.result_ty,
                        (&value.lhs, &value.rhs),
                        "fmax",
                        fenv,
                        |this| {
                            this.lower_binary_method(
                                &value.result,
                                Some(&value.result_ty),
                                &value.lhs,
                                &value.rhs,
                                "max",
                            );
                        },
                    );
                }
                Op::Fminnum(value) => {
                    let fenv = fenv_is_constrained(&value.fenv);
                    return self.lower_fenv_binary_op(
                        &value.result,
                        &value.result_ty,
                        (&value.lhs, &value.rhs),
                        "fmin",
                        fenv,
                        |this| {
                            this.lower_binary_method(
                                &value.result,
                                Some(&value.result_ty),
                                &value.lhs,
                                &value.rhs,
                                "min",
                            );
                        },
                    );
                }
                Op::Fma(value) => {
                    if fenv_is_constrained(&value.fenv)
                        && let Some(bits) = fenv_scalar_bits(&value.result_ty)
                    {
                        let shim = format!("__slate_fenv_fma_f{bits}");
                        self.parent.uses_fenv_shims.set(true);
                        let a = self.operand_expr(&value.a);
                        let b = self.operand_expr(&value.b);
                        let c = self.operand_expr(&value.c);
                        self.materialize_expr(
                            &value.result,
                            Expr::Call {
                                binding: crate::function_identity::CallBinding::Generated,
                                func: Box::new(Expr::Var(shim.into())),
                                args: vec![a, b, c],
                            },
                            Some(&value.result_ty),
                        );
                        return;
                    }
                    return self.lower_ternary_method(
                        &value.result,
                        &value.result_ty,
                        (&value.a, &value.b, &value.c),
                        "mul_add",
                    );
                }
                Op::Fmuladd(value) => {
                    return self.lower_fmuladd(
                        &value.result,
                        &value.result_ty,
                        (&value.a, &value.b, &value.c),
                    );
                }
                Op::Modf(value) => return self.lower_modf(&value),
                Op::Llrint(value) => {
                    return self.lower_unary_cast_method(
                        &value.result,
                        &value.result_ty,
                        &value.src,
                        "round_ties_even",
                    );
                }
                Op::Llround(value) => {
                    return self.lower_known_unary_cast_method(
                        &value.result,
                        &value.result_ty,
                        &value.src,
                        value.loc.as_ref(),
                        Known::Llround,
                        "round",
                    );
                }
                Op::Lrint(value) => {
                    return self.lower_unary_cast_method(
                        &value.result,
                        &value.result_ty,
                        &value.src,
                        "round_ties_even",
                    );
                }
                Op::Lround(value) => {
                    return self.lower_known_unary_cast_method(
                        &value.result,
                        &value.result_ty,
                        &value.src,
                        value.loc.as_ref(),
                        Known::Lround,
                        "round",
                    );
                }
                Op::LibcMemcpy(value) => {
                    return self.lower_mem_copy(
                        &value.dst,
                        &value.src,
                        &value.len,
                        value.loc.as_ref(),
                        false,
                    );
                }
                Op::LibcMemmove(value) => {
                    return self.lower_mem_copy(
                        &value.dst,
                        &value.src,
                        &value.len,
                        value.loc.as_ref(),
                        true,
                    );
                }
                Op::LibcMemset(value) => return self.lower_mem_set(&value),
                Op::LibcMemchr(value) => return self.lower_mem_chr(&value),
                Op::VaStart(value) => return self.lower_va_start(&value),
                Op::VaCopy(value) => return self.lower_va_copy(&value),
                Op::VaArg(value) => return self.lower_va_arg(&value),
                Op::VaEnd(_) => return,
                Op::EhSetjmp(value) => return self.lower_eh_setjmp(&value),
                Op::EhLongjmp(value) => return self.lower_eh_longjmp(&value),
                Op::FrameAddress(value) => {
                    return self.lower_opaque_pointer(&value.result, &value.result_ty, true);
                }
                Op::Return(value) => return self.lower_return(&value),
                Op::Yield(_) => return,
                Op::ReturnAddress(value) => {
                    return self.lower_opaque_pointer(&value.result, &value.result_ty, true);
                }
                Op::BlockAddress(value) => {
                    return self.lower_opaque_pointer(&value.addr, &value.addr_ty, true);
                }
                Op::Stacksave(value) => {
                    return self.lower_opaque_pointer(&value.result, &value.result_ty, false);
                }
                Op::Prefetch(_) => return,
                Op::IsFpClass(value) => return self.lower_is_fp_class(&value),
                Op::ComplexCreate(value) => {
                    return self.lower_complex_create(
                        &value.result,
                        &value.result_ty,
                        &value.real,
                        &value.imag,
                    );
                }
                Op::ComplexAdd(value) => {
                    return self.lower_complex_addsub(
                        &value.result,
                        &value.result_ty,
                        &value.lhs,
                        &value.rhs,
                        BinOp::Add,
                    );
                }
                Op::ComplexSub(value) => {
                    return self.lower_complex_addsub(
                        &value.result,
                        &value.result_ty,
                        &value.lhs,
                        &value.rhs,
                        BinOp::Sub,
                    );
                }
                Op::ComplexMul(value) => {
                    return self.lower_complex_mul(
                        &value.result,
                        &value.result_ty,
                        &value.lhs,
                        &value.rhs,
                    );
                }
                Op::ComplexDiv(value) => {
                    return self.lower_complex_div(
                        &value.result,
                        &value.result_ty,
                        &value.lhs,
                        &value.rhs,
                    );
                }
                Op::ComplexConj(value) => {
                    return self.lower_complex_conj(
                        &value.result,
                        &value.result_ty,
                        &value.operand,
                    );
                }
                Op::ComplexReal(value) => {
                    return self.lower_complex_part(
                        &value.result,
                        &value.result_ty,
                        &value.operand,
                        "re",
                    );
                }
                Op::ComplexImag(value) => {
                    return self.lower_complex_part(
                        &value.result,
                        &value.result_ty,
                        &value.operand,
                        "im",
                    );
                }
                Op::ComplexRealPtr(value) => {
                    return self.lower_complex_part_ptr(
                        &value.result,
                        &value.result_ty,
                        &value.operand,
                        "re",
                    );
                }
                Op::ComplexImagPtr(value) => {
                    return self.lower_complex_part_ptr(
                        &value.result,
                        &value.result_ty,
                        &value.operand,
                        "im",
                    );
                }
                Op::ExtractMember(value) => return self.lower_extract_member(&value),
                Op::InsertMember(value) => return self.lower_insert_member(&value),
                Op::VecCreate(value) => return self.lower_vec_create(&value),
                Op::VecExtract(value) => return self.lower_vec_extract(&value),
                Op::VecInsert(value) => return self.lower_vec_insert(&value),
                Op::VecShuffle(value) => return self.lower_vec_shuffle(&value),
                Op::VecShuffleDynamic(value) => return self.lower_vec_shuffle_dynamic(&value),
                Op::VecSplat(value) => return self.lower_vec_splat(&value),
                Op::VecCmp(value) => return self.lower_vec_cmp(&value),
                Op::IsConstant(value) => return self.lower_is_constant(&value),
                Op::Objsize(value) => return self.lower_objsize(&value),
                Op::AtomicFetch(value) => return self.lower_atomic_fetch(&value),
                Op::AtomicXchg(value) => return self.lower_atomic_xchg(&value),
                Op::AtomicCmpxchg(value) => return self.lower_atomic_cmpxchg(&value),
                Op::AtomicFence(value) => return self.lower_atomic_fence(&value),
                Op::AtomicTestAndSet(value) => return self.lower_atomic_test_and_set(&value),
                Op::AtomicClear(value) => return self.lower_atomic_clear(&value),
                Op::GetBitfield(value) => return self.lower_get_bitfield(&value),
                Op::SetBitfield(value) => return self.lower_set_bitfield(&value),
                Op::CallLlvmIntrinsic(value) => return self.lower_call_llvm_intrinsic(&value),
                _ => {}
            }
        };
        self.emit_todo("instruction without lowering");
    }

    pub(super) fn record_result_types(&mut self, op: &Op) {
        op.for_each_result(|id, ty| {
            self.value_types.insert(id.clone(), ty.clone());
        });
    }

    pub(super) fn value_type(&self, value: &str) -> Option<&CirType> {
        self.value_types.get(value)
    }
}
