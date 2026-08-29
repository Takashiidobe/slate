use super::*;
use crate::function_identity;

impl<'a, 'b> FunctionLowerer<'a, 'b> {}

fn overflow_for_result_width(
    arithmetic_overflow: Expr,
    value: Expr,
    wide_signed: bool,
    result_signed: bool,
    result_bits: u32,
) -> Expr {
    let range_overflow = match (wide_signed, result_signed) {
        (true, true) => {
            let (min, max) = if result_bits == 128 {
                (i128::MIN, i128::MAX)
            } else {
                let magnitude = 1i128 << (result_bits - 1);
                (-magnitude, magnitude - 1)
            };
            Expr::Binary {
                op: BinOp::Or,
                lhs: Box::new(Expr::Binary {
                    op: BinOp::Lt,
                    lhs: Box::new(value.clone()),
                    rhs: Box::new(Expr::Value(RustValue::I128(min))),
                }),
                rhs: Box::new(Expr::Binary {
                    op: BinOp::Gt,
                    lhs: Box::new(value),
                    rhs: Box::new(Expr::Value(RustValue::I128(max))),
                }),
            }
        }
        (true, false) => {
            let max = if result_bits == 128 {
                u128::MAX
            } else {
                (1u128 << result_bits) - 1
            };
            Expr::Binary {
                op: BinOp::Or,
                lhs: Box::new(Expr::Binary {
                    op: BinOp::Lt,
                    lhs: Box::new(value.clone()),
                    rhs: Box::new(Expr::Value(RustValue::I128(0))),
                }),
                rhs: Box::new(Expr::Binary {
                    op: BinOp::Gt,
                    lhs: Box::new(Expr::Cast {
                        expr: Box::new(value),
                        ty: Type::Prim(Prim::U128),
                    }),
                    rhs: Box::new(Expr::Value(RustValue::U128(max))),
                }),
            }
        }
        (false, true) => {
            let max = if result_bits == 128 {
                i128::MAX as u128
            } else {
                (1u128 << (result_bits - 1)) - 1
            };
            Expr::Binary {
                op: BinOp::Gt,
                lhs: Box::new(value),
                rhs: Box::new(Expr::Value(RustValue::U128(max))),
            }
        }
        (false, false) => {
            let max = if result_bits == 128 {
                u128::MAX
            } else {
                (1u128 << result_bits) - 1
            };
            Expr::Binary {
                op: BinOp::Gt,
                lhs: Box::new(value),
                rhs: Box::new(Expr::Value(RustValue::U128(max))),
            }
        }
    };
    Expr::Binary {
        op: BinOp::Or,
        lhs: Box::new(arithmetic_overflow),
        rhs: Box::new(range_overflow),
    }
}

impl<'a, 'b> FunctionLowerer<'a, 'b> {
    pub(super) fn lower_rotate(&mut self, op: &inst::Rotate) {
        self.materialize_expr(
            &op.result,
            Expr::MethodCall {
                recv: Box::new(self.operand_expr(&op.input)),
                method: if op.rotate_left {
                    "rotate_left".into()
                } else {
                    "rotate_right".into()
                },
                args: vec![self.operand_expr(&op.amount)],
            },
            Some(&op.result_ty),
        );
    }

    pub(super) fn lower_cmp(&mut self, op: &inst::Cmp) {
        if fenv_is_constrained(&op.fenv)
            && let Some(bits) = self.value_type(&op.lhs).and_then(fenv_scalar_bits)
            && let Some(name) = fenv_cmp_name(op.kind)
        {
            let shim = format!("__slate_fenv_{name}_f{bits}");
            self.parent.uses_fenv_shims.set(true);
            let lhs = self.operand_expr(&op.lhs);
            let rhs = self.operand_expr(&op.rhs);
            self.materialize_expr(
                &op.result,
                Expr::Call {
                    binding: crate::function_identity::CallBinding::Generated,
                    func: Box::new(Expr::Var(shim.into())),
                    args: vec![lhs, rhs],
                },
                Some(&op.result_ty),
            );
            return;
        }
        if let Some(expr) = self.lower_function_pointer_null_cmp(&op.lhs, &op.rhs, op.kind) {
            self.materialize_expr(&op.result, expr, Some(&op.result_ty));
            return;
        }
        let lhs_ty = self.value_type(&op.lhs);
        let rhs_ty = self.value_type(&op.rhs);
        let concrete_function_symbols = lhs_ty.zip(rhs_ty).is_some_and(|(lhs, rhs)| {
            is_cir_function_pointer_type(lhs)
                && is_cir_function_pointer_type(rhs)
                && matches!(self.values.get(&op.lhs), Some(Val::Global(_)))
                && matches!(self.values.get(&op.rhs), Some(Val::Global(_)))
        });
        let operand =
            |this: &Self, value: &str, ty: Option<&CirType>, counterpart: Option<&CirType>| {
                if concrete_function_symbols {
                    this.function_pointer_byte_operand_expr(value)
                } else if let Some(target) = counterpart
                    .filter(|c| is_cir_function_pointer_type(c))
                    .filter(|_| ty.is_some_and(is_cir_function_pointer_type))
                    .map(|c| this.parent.rust_type(c))
                    .and_then(|target_ty| this.named_function_coerced_to(value, &target_ty))
                {
                    target
                } else {
                    ty.map_or_else(
                        || this.operand_expr(value),
                        |ty| this.call_arg_expr(value, ty),
                    )
                }
            };
        let lhs = operand(self, &op.lhs, lhs_ty, rhs_ty);
        let rhs = operand(self, &op.rhs, rhs_ty, lhs_ty);
        let expr = match op.kind {
            CmpOpKind::One => Expr::Binary {
                op: BinOp::Or,
                lhs: Box::new(Expr::Binary {
                    op: BinOp::Lt,
                    lhs: Box::new(lhs.clone()),
                    rhs: Box::new(rhs.clone()),
                }),
                rhs: Box::new(Expr::Binary {
                    op: BinOp::Gt,
                    lhs: Box::new(lhs),
                    rhs: Box::new(rhs),
                }),
            },
            CmpOpKind::Uno => Expr::Binary {
                op: BinOp::Or,
                lhs: Box::new(Expr::Binary {
                    op: BinOp::Ne,
                    lhs: Box::new(lhs.clone()),
                    rhs: Box::new(lhs),
                }),
                rhs: Box::new(Expr::Binary {
                    op: BinOp::Ne,
                    lhs: Box::new(rhs.clone()),
                    rhs: Box::new(rhs),
                }),
            },
            kind => {
                let op = match kind {
                    CmpOpKind::Lt => BinOp::Lt,
                    CmpOpKind::Le => BinOp::Le,
                    CmpOpKind::Gt => BinOp::Gt,
                    CmpOpKind::Ge => BinOp::Ge,
                    CmpOpKind::Eq => BinOp::Eq,
                    CmpOpKind::Ne => BinOp::Ne,
                    CmpOpKind::One | CmpOpKind::Uno => unreachable!(),
                };
                Expr::Binary {
                    op,
                    lhs: Box::new(lhs),
                    rhs: Box::new(rhs),
                }
            }
        };
        self.materialize_expr(&op.result, expr, Some(&op.result_ty));
    }

    pub(super) fn lower_select(&mut self, op: &inst::Select) {
        let true_expr = self.fn_ptr_aware_operand_expr(
            &op.true_value,
            Some(&op.result_ty),
            Self::function_pointer_operand_expr,
            Self::value_or_place_address_expr,
        );
        let false_expr = self.fn_ptr_aware_operand_expr(
            &op.false_value,
            Some(&op.result_ty),
            Self::function_pointer_operand_expr,
            Self::value_or_place_address_expr,
        );
        self.materialize_expr(
            &op.result,
            Expr::If {
                cond: Box::new(self.operand_expr(&op.condition)),
                then_expr: Box::new(true_expr),
                else_expr: Box::new(false_expr),
            },
            Some(&op.result_ty),
        );
    }

    // cir.select(cond, t, f) is a pure value pick; all three operands are already
    // materialized, so it collapses to a Rust `if` expression.
    // cir.ternary has two value-yielding regions; clang emits it for the NaN-recovery
    // arm of complex `*` (the taken branch calls __muldc3). Lower to an `if` whose
    // block bodies run each region's ops and tail-yield the region result.
    pub(super) fn lower_ternary(&mut self, op: &inst::Ternary) {
        let (Some(result), Some(result_ty)) = (&op.result, &op.result_ty) else {
            return;
        };
        let cond = self.operand_expr(&op.cond);
        let name = self.next_temp();
        let ty = self.parent.rust_type(result_ty);
        let (then_body, then_value) = self.lower_yield_region(&op.true_region);
        let (else_body, else_value) = self.lower_yield_region(&op.false_region);
        self.push_stmt(Stmt::LetIf {
            name: name.clone(),
            mutable: false,
            ty: Some(ty),
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
        });
        self.values
            .insert(result.to_string(), Val::Expr(Expr::Var(name.into())));
    }

    // Lower every op in a region, capturing the terminating cir.yield's operand as
    // the region's tail value instead of lowering the yield itself.
    pub(super) fn lower_yield_region(&mut self, region: &inst::Region) -> (Vec<IndentStmt>, Expr) {
        let mut yielded = Expr::Todo("cir.yield".into());
        let body = self.capture_body(|this| {
            for block in &region.blocks {
                for op in &block.ops {
                    if let Op::Yield(yield_op) = op {
                        if let Some(operand) = yield_op.args.first() {
                            yielded = this.value_or_place_address_expr(operand);
                        }
                    } else {
                        this.lower_op(op.clone());
                    }
                }
            }
        });
        (body, yielded)
    }

    pub(super) fn lower_binary(
        &mut self,
        result: &str,
        result_ty: Option<&CirType>,
        lhs: &str,
        rhs: &str,
        rust_op: BinOp,
    ) {
        if let Some((_, len, _)) = result_ty.and_then(CirType::as_vector) {
            self.materialize_expr(
                result,
                self.vector_binary_expr(lhs, rhs, len, rust_op),
                result_ty,
            );
            return;
        }
        let lhs = self.operand_expr(lhs);
        let rhs = self.operand_expr(rhs);
        self.materialize_expr(
            result,
            Expr::Binary {
                op: rust_op,
                lhs: Box::new(lhs),
                rhs: Box::new(rhs),
            },
            result_ty,
        );
    }

    pub(super) fn lower_fenv_binary(
        &mut self,
        result: &str,
        result_ty: Option<&CirType>,
        lhs: &str,
        rhs: &str,
        rust_op: BinOp,
        fenv: bool,
    ) {
        if fenv
            && let Some(bits) = result_ty.and_then(fenv_scalar_bits)
            && let Some(op_name) = fenv_binop_name(rust_op)
        {
            let shim = format!("__slate_fenv_{op_name}_f{bits}");
            self.parent.uses_fenv_shims.set(true);
            let lhs = self.operand_expr(lhs);
            let rhs = self.operand_expr(rhs);
            self.materialize_expr(
                result,
                Expr::Call {
                    binding: crate::function_identity::CallBinding::Generated,
                    func: Box::new(Expr::Var(shim.into())),
                    args: vec![lhs, rhs],
                },
                result_ty,
            );
            return;
        }
        self.lower_binary(result, result_ty, lhs, rhs, rust_op);
    }

    pub(super) fn lower_saturating_arith(
        &mut self,
        result: &str,
        result_ty: &CirType,
        lhs: &str,
        rhs: &str,
        rust_method: &str,
    ) {
        if let Some((_, len, _)) = result_ty.as_vector() {
            let lhs = self.operand_expr(lhs);
            let rhs = self.operand_expr(rhs);
            let elems = (0..len)
                .map(|i| Expr::MethodCall {
                    recv: Box::new(vector_index_expr(lhs.clone(), i)),
                    method: rust_method.to_string(),
                    args: vec![vector_index_expr(rhs.clone(), i)],
                })
                .collect();
            self.materialize_expr(result, Expr::ArrayLit(elems), Some(result_ty));
            return;
        }
        self.materialize_expr(
            result,
            Expr::MethodCall {
                recv: Box::new(self.operand_expr(lhs)),
                method: rust_method.to_string(),
                args: vec![self.operand_expr(rhs)],
            },
            Some(result_ty),
        );
    }

    // The batch crate builds with `overflow-checks = false`, so plain `+`/`-`/`*`
    // wrap two's-complement just like clang's `-O0` C — no `wrapping_*` needed.
    // `/` and `%` still trap on div-by-zero and INT_MIN/-1 on both sides, so the
    // generator avoids those.
    pub(super) fn lower_int_arith(
        &mut self,
        result: &str,
        ty: Option<&CirType>,
        lhs: &str,
        rhs: &str,
        rust_op: BinOp,
    ) {
        if let Some((_, len, _)) = ty.and_then(CirType::as_vector) {
            self.materialize_expr(result, self.vector_binary_expr(lhs, rhs, len, rust_op), ty);
            return;
        }
        if let Some(folded) = self.fold_int_arith(lhs, rhs, rust_op) {
            self.macro_arith_values.insert(result.to_string(), folded);
            if let Some(expr) = self.next_macro_const_expr(folded, ty) {
                self.materialize_expr(result, expr, ty);
                return;
            }
        }
        let lhs = self.operand_expr(lhs);
        let rhs = self.operand_expr(rhs);
        self.materialize_expr(
            result,
            Expr::Binary {
                op: rust_op,
                lhs: Box::new(lhs),
                rhs: Box::new(rhs),
            },
            ty,
        );
    }

    pub(super) fn known_arith_value(&self, operand: &str) -> Option<i128> {
        self.const_int_values
            .get(operand)
            .copied()
            .or_else(|| self.macro_arith_values.get(operand).copied())
    }

    pub(super) fn fold_int_arith(&self, lhs: &str, rhs: &str, op: BinOp) -> Option<i128> {
        let l = self.known_arith_value(lhs)?;
        let r = self.known_arith_value(rhs)?;
        match op {
            BinOp::Add => Some(l + r),
            BinOp::Sub => Some(l - r),
            BinOp::Mul => Some(l * r),
            BinOp::Div if r != 0 => Some(l / r),
            BinOp::Rem if r != 0 => Some(l % r),
            BinOp::BitAnd => Some(l & r),
            BinOp::BitOr => Some(l | r),
            BinOp::BitXor => Some(l ^ r),
            BinOp::Shl => u32::try_from(r).ok().and_then(|shift| l.checked_shl(shift)),
            BinOp::Shr => u32::try_from(r).ok().and_then(|shift| l.checked_shr(shift)),
            _ => None,
        }
    }

    pub(super) fn fold_unary_arith(&self, operand: &str, op: UnaryOp) -> Option<i128> {
        let v = self.known_arith_value(operand)?;
        match op {
            UnaryOp::Neg => Some(-v),
            UnaryOp::Not => Some(!v),
            _ => None,
        }
    }

    fn checked_arith_pair(&mut self, lhs: Expr, rhs: Expr, rust_method: &str) -> (Expr, Expr) {
        let pair = self.next_temp();
        self.push_stmt(Stmt::Let {
            name: pair.clone(),
            mutable: false,
            ty: None,
            init: Some(Expr::MethodCall {
                recv: Box::new(lhs),
                method: rust_method.to_string(),
                args: vec![rhs],
            }),
        });
        (
            Expr::Field {
                base: Box::new(Expr::Var(pair.clone().into())),
                field: "0".into(),
            },
            Expr::Field {
                base: Box::new(Expr::Var(pair.into())),
                field: "1".into(),
            },
        )
    }

    fn finish_checked_arith(
        &mut self,
        results: (&str, &CirType, &str, &CirType),
        wide_result: Expr,
        wide_overflow: Expr,
        wide_signed: bool,
    ) {
        match resolved_integer_parts(results.1, &self.parent.aliases) {
            Some((result_signed, result_bits)) if result_bits <= 128 => {
                let result_rust_ty = self.parent.rust_type(results.1);
                let narrowed =
                    bitint_from_int_expr(&result_rust_ty, wide_result.clone(), wide_signed)
                        .unwrap_or_else(|| Expr::Cast {
                            expr: Box::new(wide_result.clone()),
                            ty: result_rust_ty,
                        });
                self.materialize_expr(results.0, narrowed, Some(results.1));
                let overflow = overflow_for_result_width(
                    wide_overflow,
                    wide_result,
                    wide_signed,
                    result_signed,
                    result_bits,
                );
                self.materialize_expr(results.2, overflow, Some(results.3));
            }
            _ => {
                self.materialize_expr(results.0, wide_result, Some(results.1));
                self.materialize_expr(results.2, wide_overflow, Some(results.3));
            }
        }
    }

    pub(super) fn lower_overflow_arith(
        &mut self,
        results: (&str, &CirType, &str, &CirType),
        lhs: &str,
        rhs: &str,
        rust_method: &str,
    ) {
        let operand_ty = self.value_type(lhs);
        let operand_rust_ty = operand_ty
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or(Type::Prim(Prim::I32));
        let operand_int_ty =
            operand_ty.and_then(|ty| resolved_integer_parts(ty, &self.parent.aliases));

        if bitint_generic_parts(&operand_rust_ty).is_some()
            && operand_int_ty.is_some_and(|(_, bits)| bits <= 128)
        {
            let (lhs, wide_signed) =
                bitint_to_int_expr(&operand_rust_ty, self.operand_expr(lhs)).unwrap();
            let (rhs, _) = bitint_to_int_expr(&operand_rust_ty, self.operand_expr(rhs)).unwrap();
            let (wide_result, wide_overflow) = self.checked_arith_pair(lhs, rhs, rust_method);
            self.finish_checked_arith(results, wide_result, wide_overflow, wide_signed);
            return;
        }

        if bitint_generic_parts(&operand_rust_ty).is_some() {
            let lhs = self.operand_expr(lhs);
            let rhs = self.operand_expr(rhs);
            let (pair_result, wide_overflow) = self.checked_arith_pair(lhs, rhs, rust_method);
            let (wide_result, wide_signed) =
                bitint_to_int_expr(&operand_rust_ty, pair_result).unwrap();
            self.finish_checked_arith(results, wide_result, wide_overflow, wide_signed);
            return;
        }

        let lhs = self.operand_expr(lhs);
        let rhs = self.operand_expr(rhs);
        let wide_signed = operand_int_ty.is_none_or(|(signed, _)| signed);
        let (wide_result, wide_overflow) = self.checked_arith_pair(lhs, rhs, rust_method);
        self.finish_checked_arith(results, wide_result, wide_overflow, wide_signed);
    }

    pub(super) fn lower_step(
        &mut self,
        result: &str,
        result_ty: &CirType,
        value: &str,
        rust_op: BinOp,
    ) {
        self.materialize_expr(
            result,
            Expr::Binary {
                op: rust_op,
                lhs: Box::new(self.operand_expr(value)),
                rhs: Box::new(Expr::Value(RustValue::I64(1))),
            },
            Some(result_ty),
        );
    }

    // cir.shift carries the isShiftleft unit attr for `<<`; its absence means `>>`.
    // Rust's `>>` is arithmetic on signed and logical on unsigned, matching C by type.
    // cir.not is C's unary `~`; Rust spells integer bitwise complement `!`.
    pub(super) fn lower_not(&mut self, result: &str, result_ty: &CirType, value: &str) {
        if let Some((_, len, _)) = result_ty.as_vector() {
            let value = self.operand_expr(value);
            self.materialize_expr(
                result,
                Expr::ArrayLit(
                    (0..len)
                        .map(|i| Expr::Unary {
                            op: UnaryOp::Not,
                            expr: Box::new(vector_index_expr(value.clone(), i)),
                        })
                        .collect(),
                ),
                Some(result_ty),
            );
            return;
        }
        if let Some(folded) = self.fold_unary_arith(value, UnaryOp::Not) {
            self.macro_arith_values.insert(result.to_string(), folded);
        }
        self.materialize_expr(
            result,
            Expr::Unary {
                op: UnaryOp::Not,
                expr: Box::new(self.operand_expr(value)),
            },
            Some(result_ty),
        );
    }

    pub(super) fn lower_neg(&mut self, result: &str, result_ty: &CirType, value: &str) {
        if let Some((elem_ty, len, _)) = result_ty.as_vector() {
            let value = self.operand_expr(value);
            let elem_rust_ty = self.parent.rust_type(elem_ty);
            let elem_is_wrapping_int = matches!(
                &elem_rust_ty,
                Type::Prim(
                    Prim::I8
                        | Prim::I16
                        | Prim::I32
                        | Prim::I64
                        | Prim::I128
                        | Prim::Isize
                        | Prim::U8
                        | Prim::U16
                        | Prim::U32
                        | Prim::U64
                        | Prim::U128
                        | Prim::Usize
                )
            );
            self.materialize_expr(
                result,
                Expr::ArrayLit(
                    (0..len)
                        .map(|i| {
                            let elem = vector_index_expr(value.clone(), i);
                            if elem_is_wrapping_int {
                                Expr::MethodCall {
                                    recv: Box::new(elem),
                                    method: "wrapping_neg".into(),
                                    args: vec![],
                                }
                            } else {
                                Expr::Unary {
                                    op: UnaryOp::Neg,
                                    expr: Box::new(elem),
                                }
                            }
                        })
                        .collect(),
                ),
                Some(result_ty),
            );
            return;
        }
        if let Some(folded) = self.fold_unary_arith(value, UnaryOp::Neg) {
            self.macro_arith_values.insert(result.to_string(), folded);
        }
        let operand_ty = self.value_type(value);
        let value = self.operand_expr(value);
        let rust_ty = self.parent.rust_type(result_ty);
        let is_wrapping_int = matches!(
            &rust_ty,
            Type::Prim(
                Prim::I8
                    | Prim::I16
                    | Prim::I32
                    | Prim::I64
                    | Prim::I128
                    | Prim::Isize
                    | Prim::U8
                    | Prim::U16
                    | Prim::U32
                    | Prim::U64
                    | Prim::U128
                    | Prim::Usize
            )
        );
        let expr = if operand_ty.is_some_and(|ty| matches!(ty, CirType::Bool)) {
            let cast = Expr::Cast {
                expr: Box::new(value),
                ty: rust_ty,
            };
            if is_wrapping_int {
                Expr::MethodCall {
                    recv: Box::new(cast),
                    method: "wrapping_neg".into(),
                    args: vec![],
                }
            } else {
                Expr::Unary {
                    op: UnaryOp::Neg,
                    expr: Box::new(cast),
                }
            }
        } else if is_wrapping_int {
            Expr::MethodCall {
                recv: Box::new(value),
                method: "wrapping_neg".into(),
                args: vec![],
            }
        } else {
            Expr::Unary {
                op: UnaryOp::Neg,
                expr: Box::new(value),
            }
        };
        self.materialize_expr(result, expr, Some(result_ty));
    }

    pub(super) fn lower_abs(&mut self, result: &str, result_ty: Option<&CirType>, value: &str) {
        if let Some((elem_ty, len, _)) = result_ty.and_then(CirType::as_vector) {
            let is_int_elem = match elem_ty {
                CirType::Int { .. } => true,
                CirType::Named(name) => matches!(
                    named_scalar_type(name),
                    Some(Type::Prim(
                        Prim::I8
                            | Prim::I16
                            | Prim::I32
                            | Prim::I64
                            | Prim::I128
                            | Prim::Isize
                            | Prim::U8
                            | Prim::U16
                            | Prim::U32
                            | Prim::U64
                            | Prim::U128
                            | Prim::Usize
                    ))
                ),
                _ => false,
            };
            let method = if is_int_elem { "wrapping_abs" } else { "abs" };
            let value = self.operand_expr(value);
            let elems = (0..len)
                .map(|i| Expr::MethodCall {
                    recv: Box::new(vector_index_expr(value.clone(), i)),
                    method: method.into(),
                    args: vec![],
                })
                .collect();
            self.materialize_expr(result, Expr::ArrayLit(elems), result_ty);
            return;
        }
        let value = self.operand_expr(value);
        let rust_ty = result_ty
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or(Type::Prim(Prim::I32));
        let expr = if matches!(
            &rust_ty,
            Type::Prim(Prim::I8 | Prim::I16 | Prim::I32 | Prim::I64)
        ) {
            Expr::MethodCall {
                recv: Box::new(value),
                method: "wrapping_abs".into(),
                args: vec![],
            }
        } else if type_mentions_long_double(&rust_ty) {
            Expr::Call {
                binding: function_identity::CallBinding::Generated,
                func: Box::new(Expr::Var("__slate_f80_abs".into())),
                args: vec![value],
            }
        } else {
            Expr::MethodCall {
                recv: Box::new(value),
                method: "abs".into(),
                args: vec![],
            }
        };
        self.materialize_expr(result, expr, result_ty);
    }

    pub(super) fn lower_freeze(&mut self, op: &inst::Freeze) {
        let value = self.operand_expr(&op.input);
        self.materialize_expr(&op.result, value, Some(&op.result_ty));
    }

    pub(super) fn lower_unary_method(
        &mut self,
        result: &str,
        result_ty: Option<&CirType>,
        value: &str,
        method: &str,
    ) {
        let value = self.operand_expr(value);
        if let Some((_, len, _)) = result_ty.and_then(CirType::as_vector) {
            let elems = (0..len)
                .map(|i| Expr::MethodCall {
                    recv: Box::new(vector_index_expr(value.clone(), i)),
                    method: method.into(),
                    args: vec![],
                })
                .collect();
            self.materialize_expr(result, Expr::ArrayLit(elems), result_ty);
            return;
        }
        let rust_ty = result_ty
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or(Type::Prim(Prim::F64));
        let expr = if type_mentions_long_double(&rust_ty) {
            let shim = match method {
                "abs" => "__slate_f80_abs",
                "ceil" => "__slate_f80_ceil",
                "floor" => "__slate_f80_floor",
                "round" => "__slate_f80_round",
                "trunc" => "__slate_f80_trunc",
                "round_ties_even" => "__slate_f80_rint",
                _ => {
                    self.emit_todo("long double unary operation");
                    return;
                }
            };
            Expr::Call {
                binding: function_identity::CallBinding::Generated,
                func: Box::new(Expr::Var(shim.into())),
                args: vec![value],
            }
        } else {
            let call = Expr::MethodCall {
                recv: Box::new(value),
                method: method.into(),
                args: vec![],
            };
            if matches!(method, "trailing_zeros" | "leading_zeros" | "count_ones") {
                Expr::Cast {
                    expr: Box::new(call),
                    ty: rust_ty.clone(),
                }
            } else {
                call
            }
        };
        self.materialize_expr(result, expr, result_ty);
    }

    pub(super) fn lower_known_unary_method(
        &mut self,
        result: &str,
        result_ty: &CirType,
        value: &str,
        loc: Option<&SourceLocation>,
        known: function_identity::Known,
        method: &str,
    ) {
        if self.lower_known_libc_unary(result, result_ty, value, loc, known) {
            self.lower_unary_method(result, Some(result_ty), value, method);
        }
    }

    pub(super) fn lower_unary_cast_method(
        &mut self,
        result: &str,
        result_ty: &CirType,
        value: &str,
        method: &str,
    ) {
        let ty = self.parent.rust_type(result_ty);
        let expr = Expr::Cast {
            expr: Box::new(Expr::MethodCall {
                recv: Box::new(self.operand_expr(value)),
                method: method.into(),
                args: Vec::new(),
            }),
            ty,
        };
        self.materialize_expr(result, expr, Some(result_ty));
    }

    pub(super) fn lower_known_unary_cast_method(
        &mut self,
        result: &str,
        result_ty: &CirType,
        value: &str,
        loc: Option<&SourceLocation>,
        known: function_identity::Known,
        method: &str,
    ) {
        if self.lower_known_libc_unary(result, result_ty, value, loc, known) {
            self.lower_unary_cast_method(result, result_ty, value, method);
        }
    }

    pub(super) fn lower_parity(&mut self, result: &str, result_ty: Option<&CirType>, value: &str) {
        let expr = Expr::Binary {
            op: BinOp::BitAnd,
            lhs: Box::new(Expr::MethodCall {
                recv: Box::new(self.operand_expr(value)),
                method: "count_ones".into(),
                args: Vec::new(),
            }),
            rhs: Box::new(Expr::Value(RustValue::I64(1))),
        };
        self.materialize_expr(result, expr, result_ty);
    }
    pub(super) fn lower_expect(&mut self, op: &inst::Expect) {
        self.materialize_expr(&op.result, self.operand_expr(&op.val), Some(&op.result_ty));
    }

    pub(super) fn lower_assume(&mut self, op: &inst::Assume) {
        self.push_stmt(Stmt::Expr(Self::unsafe_expr(Expr::Call {
            binding: function_identity::CallBinding::Generated,
            func: Box::new(Expr::Path(Path::new(
                ["core", "hint", "assert_unchecked"].map(Ident::from),
            ))),
            args: vec![self.operand_expr(&op.predicate)],
        })));
    }
}
