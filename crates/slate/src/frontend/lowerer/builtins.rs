use super::*;

impl<'a, 'b> FunctionLowerer<'a, 'b> {
    pub(super) fn lower_ffs(&mut self, op: &Op) {
        let Some((result, _)) = op.results.first() else {
            return;
        };
        let Some(value) = op.operands.first() else {
            return;
        };
        let result_ty = op_result_type(op);
        let ty = result_ty
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or(Type::Prim(Prim::I32));
        let value = self.operand_expr(value);
        let expr = Expr::If {
            cond: Box::new(Expr::Binary {
                op: BinOp::Eq,
                lhs: Box::new(value.clone()),
                rhs: Box::new(Expr::Value(RustValue::I64(0))),
            }),
            then_expr: Box::new(Expr::Value(RustValue::I64(0))),
            else_expr: Box::new(Expr::Binary {
                op: BinOp::Add,
                lhs: Box::new(Expr::Cast {
                    expr: Box::new(Expr::MethodCall {
                        recv: Box::new(value),
                        method: "trailing_zeros".into(),
                        args: Vec::new(),
                    }),
                    ty,
                }),
                rhs: Box::new(Expr::Value(RustValue::I64(1))),
            }),
        };
        self.materialize_expr(result, expr, result_ty);
    }

    pub(super) fn lower_clrsb(&mut self, op: &Op) {
        let Some((result, _)) = op.results.first() else {
            return;
        };
        let Some(value) = op.operands.first() else {
            return;
        };
        let result_ty = op_result_type(op);
        let ty = result_ty
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or(Type::Prim(Prim::I32));
        let value = self.operand_expr(value);
        let sign_stripped = Expr::If {
            cond: Box::new(Expr::Binary {
                op: BinOp::Lt,
                lhs: Box::new(value.clone()),
                rhs: Box::new(Expr::Value(RustValue::I64(0))),
            }),
            then_expr: Box::new(Expr::Unary {
                op: UnaryOp::Not,
                expr: Box::new(value.clone()),
            }),
            else_expr: Box::new(value),
        };
        let expr = Expr::Binary {
            op: BinOp::Sub,
            lhs: Box::new(Expr::Cast {
                expr: Box::new(Expr::MethodCall {
                    recv: Box::new(sign_stripped),
                    method: "leading_zeros".into(),
                    args: Vec::new(),
                }),
                ty,
            }),
            rhs: Box::new(Expr::Value(RustValue::I64(1))),
        };
        self.materialize_expr(result, expr, result_ty);
    }

    pub(super) fn lower_binary_method(&mut self, op: &Op, method: &str) {
        let Some((result, _)) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        let lhs = self.operand_expr(&op.operands[0]);
        let rhs = self.operand_expr(&op.operands[1]);
        let result_ty = op_result_type(op);
        let rust_ty = result_ty
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or(Type::Prim(Prim::F64));
        let expr = if type_mentions_long_double(&rust_ty) {
            let shim = match method {
                "copysign" => "__slate_f80_copysign",
                "max" => "__slate_f80_fmax",
                "min" => "__slate_f80_fmin",
                _ => {
                    self.emit_todo("long double binary operation");
                    return;
                }
            };
            Expr::Call {
                binding: crate::function_identity::CallBinding::Generated,
                func: Box::new(Expr::Var(shim.into())),
                args: vec![lhs, rhs],
            }
        } else {
            Expr::MethodCall {
                recv: Box::new(lhs),
                method: method.into(),
                args: vec![rhs],
            }
        };
        self.materialize_expr(result, expr, result_ty);
    }

    pub(super) fn lower_known_binary_method(
        &mut self,
        op: &Op,
        known: crate::function_identity::Known,
        method: &str,
    ) {
        if self.lower_known_libc_op(op, known) {
            self.lower_binary_method(op, method);
        }
    }

    pub(super) fn lower_ternary_method(&mut self, op: &Op, method: &str) {
        let Some((result, _)) = op.results.first() else {
            return;
        };
        if op.operands.len() < 3 {
            return;
        }
        let a = self.operand_expr(&op.operands[0]);
        let b = self.operand_expr(&op.operands[1]);
        let c = self.operand_expr(&op.operands[2]);
        let result_ty = op_result_type(op);
        let rust_ty = result_ty
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or(Type::Prim(Prim::F64));
        let expr = if type_mentions_long_double(&rust_ty) {
            let shim = match method {
                "mul_add" => "__slate_f80_fma",
                _ => {
                    self.emit_todo("long double ternary operation");
                    return;
                }
            };
            Expr::Call {
                binding: crate::function_identity::CallBinding::Generated,
                func: Box::new(Expr::Var(shim.into())),
                args: vec![a, b, c],
            }
        } else {
            Expr::MethodCall {
                recv: Box::new(a),
                method: method.into(),
                args: vec![b, c],
            }
        };
        self.materialize_expr(result, expr, result_ty);
    }

    pub(super) fn lower_signbit(&mut self, op: &Op) {
        let Some((result, _)) = op.results.first() else {
            return;
        };
        let Some(value) = op.operands.first() else {
            return;
        };
        let operand_ty = op_operand_types(op).first();
        if operand_ty.is_some_and(is_wrapped_long_double) {
            self.materialize_expr(
                result,
                Expr::Call {
                    binding: crate::function_identity::CallBinding::Generated,
                    func: Box::new(Expr::Var("__slate_f80_signbit".into())),
                    args: vec![self.operand_expr(value)],
                },
                op_result_type(op),
            );
            return;
        }
        let value = self.float_predicate_operand_expr(value, operand_ty);
        self.materialize_expr(
            result,
            Expr::MethodCall {
                recv: Box::new(value),
                method: "is_sign_negative".into(),
                args: vec![],
            },
            op_result_type(op),
        );
    }

    pub(super) fn lower_is_fp_class(&mut self, op: &Op) {
        let Some((result, _)) = op.results.first() else {
            return;
        };
        let Some(value) = op.operands.first() else {
            return;
        };
        let Some(flags) = attr_int(op, "flags") else {
            return;
        };
        let operand_ty = op_operand_types(op).first();
        if operand_ty.is_some_and(is_wrapped_long_double) {
            self.materialize_expr(
                result,
                Expr::Call {
                    binding: crate::function_identity::CallBinding::Generated,
                    func: Box::new(Expr::Var("__slate_f80_is_fp_class".into())),
                    args: vec![self.operand_expr(value), Expr::Value(RustValue::I64(flags))],
                },
                op_result_type(op),
            );
            return;
        }
        let value = self.float_predicate_operand_expr(value, operand_ty);
        let mut parts = Vec::new();
        if flags & 0x3 != 0 {
            parts.push(Expr::MethodCall {
                recv: Box::new(value.clone()),
                method: "is_nan".into(),
                args: vec![],
            });
        }
        if flags & 0x4 != 0 {
            parts.push(Expr::Binary {
                op: BinOp::Eq,
                lhs: Box::new(value.clone()),
                rhs: Box::new(Expr::Path(Path::new(
                    ["f64", "NEG_INFINITY"].map(Ident::from),
                ))),
            });
        }
        if flags & 0x8 != 0 {
            parts.push(Self::and_expr(
                Expr::MethodCall {
                    recv: Box::new(value.clone()),
                    method: "is_normal".into(),
                    args: vec![],
                },
                Expr::MethodCall {
                    recv: Box::new(value.clone()),
                    method: "is_sign_negative".into(),
                    args: vec![],
                },
            ));
        }
        if flags & 0x10 != 0 {
            parts.push(Self::and_expr(
                Expr::MethodCall {
                    recv: Box::new(value.clone()),
                    method: "is_subnormal".into(),
                    args: vec![],
                },
                Expr::MethodCall {
                    recv: Box::new(value.clone()),
                    method: "is_sign_negative".into(),
                    args: vec![],
                },
            ));
        }
        if flags & 0x20 != 0 {
            parts.push(Self::and_expr(
                Expr::Binary {
                    op: BinOp::Eq,
                    lhs: Box::new(value.clone()),
                    rhs: Box::new(Expr::Value(0.0.into())),
                },
                Expr::MethodCall {
                    recv: Box::new(value.clone()),
                    method: "is_sign_negative".into(),
                    args: vec![],
                },
            ));
        }
        if flags & 0x40 != 0 {
            parts.push(Self::and_expr(
                Expr::Binary {
                    op: BinOp::Eq,
                    lhs: Box::new(value.clone()),
                    rhs: Box::new(Expr::Value(0.0.into())),
                },
                Expr::Unary {
                    op: UnaryOp::Not,
                    expr: Box::new(Expr::MethodCall {
                        recv: Box::new(value.clone()),
                        method: "is_sign_negative".into(),
                        args: vec![],
                    }),
                },
            ));
        }
        if flags & 0x80 != 0 {
            parts.push(Self::and_expr(
                Expr::MethodCall {
                    recv: Box::new(value.clone()),
                    method: "is_subnormal".into(),
                    args: vec![],
                },
                Expr::Unary {
                    op: UnaryOp::Not,
                    expr: Box::new(Expr::MethodCall {
                        recv: Box::new(value.clone()),
                        method: "is_sign_negative".into(),
                        args: vec![],
                    }),
                },
            ));
        }
        if flags & 0x100 != 0 {
            parts.push(Self::and_expr(
                Expr::MethodCall {
                    recv: Box::new(value.clone()),
                    method: "is_normal".into(),
                    args: vec![],
                },
                Expr::Unary {
                    op: UnaryOp::Not,
                    expr: Box::new(Expr::MethodCall {
                        recv: Box::new(value.clone()),
                        method: "is_sign_negative".into(),
                        args: vec![],
                    }),
                },
            ));
        }
        if flags & 0x200 != 0 {
            parts.push(Expr::Binary {
                op: BinOp::Eq,
                lhs: Box::new(value),
                rhs: Box::new(Expr::Path(Path::new(["f64", "INFINITY"].map(Ident::from)))),
            });
        }
        let expr = if parts.is_empty() {
            Expr::Value(RustValue::Bool(false))
        } else {
            Self::or_exprs(parts)
        };
        self.materialize_expr(result, expr, op_result_type(op));
    }

    pub(super) fn float_predicate_operand_expr(&self, operand: &str, ty: Option<&CirType>) -> Expr {
        let value = self.operand_expr(operand);
        match ty {
            Some(ty) if is_wrapped_long_double(ty) => Expr::Field {
                base: Box::new(value),
                field: "0".into(),
            },
            Some(CirType::Single) => Expr::Cast {
                expr: Box::new(value),
                ty: Type::Prim(Prim::F64),
            },
            _ => value,
        }
    }

    pub(super) fn lower_modf(&mut self, op: &Op) {
        if op.results.len() < 2 {
            return;
        }
        let Some(value) = op.operands.first() else {
            return;
        };
        let value = self.operand_expr(value);
        let result_types = op_result_types(op);
        if result_types
            .first()
            .is_some_and(|ty| is_wrapped_long_double(ty))
        {
            self.materialize_expr(
                &op.results[0].0,
                Expr::Call {
                    binding: crate::function_identity::CallBinding::Generated,
                    func: Box::new(Expr::Var("__slate_f80_fract".into())),
                    args: vec![value.clone()],
                },
                result_types.first().copied(),
            );
            self.materialize_expr(
                &op.results[1].0,
                Expr::Call {
                    binding: crate::function_identity::CallBinding::Generated,
                    func: Box::new(Expr::Var("__slate_f80_trunc".into())),
                    args: vec![value],
                },
                result_types.get(1).copied(),
            );
            return;
        }
        self.materialize_expr(
            &op.results[0].0,
            Expr::MethodCall {
                recv: Box::new(value.clone()),
                method: "fract".into(),
                args: vec![],
            },
            result_types.first().copied(),
        );
        self.materialize_expr(
            &op.results[1].0,
            Expr::MethodCall {
                recv: Box::new(value),
                method: "trunc".into(),
                args: vec![],
            },
            result_types.get(1).copied(),
        );
    }
    pub(super) fn lower_function_pointer_null_cmp(&self, op: &Op) -> Option<Expr> {
        let kind = attr_int(op, "kind")?;
        let (nonnull_operand, method) = match (
            self.is_function_pointer_null_operand(&op.operands[0]),
            self.is_function_pointer_null_operand(&op.operands[1]),
            kind,
        ) {
            (false, true, 4) => (&op.operands[0], "is_none"),
            (false, true, 5) => (&op.operands[0], "is_some"),
            (true, false, 4) => (&op.operands[1], "is_none"),
            (true, false, 5) => (&op.operands[1], "is_some"),
            _ => return None,
        };
        Some(Expr::MethodCall {
            recv: Box::new(self.function_pointer_operand_expr(nonnull_operand)),
            method: method.into(),
            args: Vec::new(),
        })
    }
}
