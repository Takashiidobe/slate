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
    pub(super) fn lower_cmp(&mut self, op: &Op) {
        let Some((result, result_ty)) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        if let Some(expr) = self.lower_function_pointer_null_cmp(op) {
            self.materialize_expr(result, expr, Some(result_ty));
            return;
        }
        let kind = attr_int(op, "kind");
        let Some(operand_types) = self.operand_types(op) else {
            return;
        };
        let concrete_function_symbols = operand_types
            .first()
            .zip(operand_types.get(1))
            .is_some_and(|(lhs, rhs)| {
                is_cir_function_pointer_type(lhs)
                    && is_cir_function_pointer_type(rhs)
                    && matches!(self.values.get(&op.operands[0]), Some(Val::Global(_)))
                    && matches!(self.values.get(&op.operands[1]), Some(Val::Global(_)))
            });
        let operand = |this: &Self, index: usize| {
            if concrete_function_symbols {
                this.function_pointer_byte_operand_expr(&op.operands[index])
            } else {
                operand_types.get(index).map_or_else(
                    || this.operand_expr(&op.operands[index]),
                    |ty| this.call_arg_expr(&op.operands[index], ty),
                )
            }
        };
        let lhs = operand(self, 0);
        let rhs = operand(self, 1);
        let expr = match kind {
            Some(6) => Expr::Binary {
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
            Some(7) => Expr::Binary {
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
            Some(kind) => {
                let op = match kind {
                    0 => BinOp::Lt,
                    1 => BinOp::Le,
                    2 => BinOp::Gt,
                    3 => BinOp::Ge,
                    4 => BinOp::Eq,
                    5 => BinOp::Ne,
                    _ => return,
                };
                Expr::Binary {
                    op,
                    lhs: Box::new(lhs),
                    rhs: Box::new(rhs),
                }
            }
            None => return,
        };
        self.materialize_expr(result, expr, Some(result_ty));
    }

    pub(super) fn lower_select(&mut self, op: &Op) {
        let Some((result, result_ty)) = op.results.first() else {
            return;
        };
        let (Some(condition), Some(true_value), Some(false_value)) =
            (op.operands.first(), op.operands.get(1), op.operands.get(2))
        else {
            return;
        };
        let true_expr = self.fn_ptr_aware_operand_expr(
            true_value,
            Some(result_ty),
            Self::function_pointer_operand_expr,
            Self::value_or_place_address_expr,
        );
        let false_expr = self.fn_ptr_aware_operand_expr(
            false_value,
            Some(result_ty),
            Self::function_pointer_operand_expr,
            Self::value_or_place_address_expr,
        );
        self.materialize_expr(
            result,
            Expr::If {
                cond: Box::new(self.operand_expr(condition)),
                then_expr: Box::new(true_expr),
                else_expr: Box::new(false_expr),
            },
            Some(result_ty),
        );
    }

    // cir.select(cond, t, f) is a pure value pick; all three operands are already
    // materialized, so it collapses to a Rust `if` expression.
    // cir.ternary has two value-yielding regions; clang emits it for the NaN-recovery
    // arm of complex `*` (the taken branch calls __muldc3). Lower to an `if` whose
    // block bodies run each region's ops and tail-yield the region result.
    pub(super) fn lower_ternary(&mut self, op: &Op) {
        let Some((result, _)) = op.results.first() else {
            return;
        };
        let Some(cond) = op.operands.first() else {
            return;
        };
        if op.regions.len() < 2 {
            self.emit_todo("cir.ternary");
            return;
        }
        let cond = self.operand_expr(cond);
        let name = self.next_temp();
        let ty = op_result_type(op)
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or(Type::Prim(Prim::I32));
        let (then_body, then_value) = self.lower_yield_region(&op.regions[0]);
        let (else_body, else_value) = self.lower_yield_region(&op.regions[1]);
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
    pub(super) fn lower_yield_region(&mut self, region: &Region) -> (Vec<IndentStmt>, Expr) {
        let mut yielded = Expr::Todo("cir.yield".into());
        let body = self.capture_body(|this| {
            for block in &region.blocks {
                for op in &block.ops {
                    if op.kind() == CirOpKind::Yield {
                        if let Some(operand) = op.operands.first() {
                            yielded = this.value_or_place_address_expr(operand);
                        }
                    } else {
                        this.lower_op(op);
                    }
                }
            }
        });
        (body, yielded)
    }

    pub(super) fn lower_binary_typed(
        &mut self,
        result: &str,
        result_ty: Option<&CirType>,
        lhs: &str,
        rhs: &str,
        rust_op: BinOp,
    ) {
        if let Some((_, len)) = result_ty.and_then(parse_cir_vector_type) {
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

    pub(super) fn lower_saturating_arith_typed(
        &mut self,
        result: &str,
        result_ty: &CirType,
        lhs: &str,
        rhs: &str,
        rust_method: &str,
    ) {
        if let Some((_, len)) = parse_cir_vector_type(result_ty) {
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
    pub(super) fn lower_int_arith_typed(
        &mut self,
        result: &str,
        ty: Option<&CirType>,
        lhs: &str,
        rhs: &str,
        rust_op: BinOp,
    ) {
        if let Some((_, len)) = ty.and_then(parse_cir_vector_type) {
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
        match parse_cir_int_type(results.1) {
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

    pub(super) fn lower_overflow_arith_typed(
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
        let operand_int_ty = operand_ty.and_then(parse_cir_int_type);

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

    pub(super) fn lower_step_typed(
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
    pub(super) fn lower_not_typed(&mut self, result: &str, result_ty: &CirType, value: &str) {
        if let Some((_, len)) = parse_cir_vector_type(result_ty) {
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

    pub(super) fn lower_neg_typed(&mut self, result: &str, result_ty: &CirType, value: &str) {
        if let Some((elem_ty, len)) = parse_cir_vector_type(result_ty) {
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

    pub(super) fn lower_abs_typed(
        &mut self,
        result: &str,
        result_ty: Option<&CirType>,
        value: &str,
    ) {
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

    pub(super) fn lower_unary_method_typed(
        &mut self,
        result: &str,
        result_ty: Option<&CirType>,
        value: &str,
        method: &str,
    ) {
        let value = self.operand_expr(value);
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
            Expr::MethodCall {
                recv: Box::new(value),
                method: method.into(),
                args: vec![],
            }
        };
        self.materialize_expr(result, expr, result_ty);
    }

    pub(super) fn lower_known_unary_method_typed(
        &mut self,
        result: &str,
        result_ty: &CirType,
        value: &str,
        loc: Option<&SourceLocation>,
        known: function_identity::Known,
        method: &str,
    ) {
        if self.lower_known_libc_unary_typed(result, result_ty, value, loc, known) {
            self.lower_unary_method_typed(result, Some(result_ty), value, method);
        }
    }

    pub(super) fn lower_unary_cast_method(&mut self, op: &Op, method: &str) {
        let Some((result, _)) = op.results.first() else {
            return;
        };
        let Some(value) = op.operands.first() else {
            return;
        };
        let result_ty = op_result_type(op);
        let ty = result_ty
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or(Type::Prim(Prim::I64));
        let expr = Expr::Cast {
            expr: Box::new(Expr::MethodCall {
                recv: Box::new(self.operand_expr(value)),
                method: method.into(),
                args: Vec::new(),
            }),
            ty,
        };
        self.materialize_expr(result, expr, result_ty);
    }

    pub(super) fn lower_known_unary_cast_method(
        &mut self,
        op: &Op,
        known: function_identity::Known,
        method: &str,
    ) {
        if self.lower_known_libc_op(op, known) {
            self.lower_unary_cast_method(op, method);
        }
    }

    pub(super) fn lower_parity_typed(
        &mut self,
        result: &str,
        result_ty: Option<&CirType>,
        value: &str,
    ) {
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
    pub(super) fn lower_expect(&mut self, op: &Op) {
        let Some((result, _)) = op.results.first() else {
            return;
        };
        let Some(value) = op.operands.first() else {
            return;
        };
        self.materialize_expr(result, self.operand_expr(value), op_result_type(op));
    }

    pub(super) fn lower_assume(&mut self, op: &Op) {
        let Some(cond) = op.operands.first() else {
            return;
        };
        self.push_stmt(Stmt::Expr(Self::unsafe_expr(Expr::Call {
            binding: function_identity::CallBinding::Generated,
            func: Box::new(Expr::Path(Path::new(
                ["core", "hint", "assert_unchecked"].map(Ident::from),
            ))),
            args: vec![self.operand_expr(cond)],
        })));
    }
}
