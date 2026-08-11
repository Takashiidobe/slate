use super::*;

impl<'a, 'b> FunctionLowerer<'a, 'b> {
    pub(super) fn lower_complex_create(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        self.materialize_expr(
            result,
            Expr::StructLit {
                name: "Complex".into(),
                fields: vec![
                    ("re".into(), self.operand_expr(&op.operands[0])),
                    ("im".into(), self.operand_expr(&op.operands[1])),
                ],
            },
            op_result_type(op),
        );
    }

    pub(super) fn lower_complex_part(&mut self, op: &Op, field: &str) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(src) = op.operands.first() else {
            return;
        };
        self.materialize_expr(
            result,
            Expr::Field {
                base: Box::new(self.operand_expr(src)),
                field: field.into(),
            },
            op_result_type(op),
        );
    }

    pub(super) fn lower_complex_part_ptr(&mut self, op: &Op, field: &str) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(base_ptr) = op.operands.first() else {
            return;
        };
        let base = self.place_or_deref_expr(base_ptr);
        let unsafe_access =
            self.place_expr(base_ptr).is_none() || self.ptr_requires_unsafe(base_ptr);
        self.member_ptrs.insert(
            result.clone(),
            MemberPtr {
                base,
                field: field.into(),
                field_ty: op_result_type(op)
                    .and_then(cir_ptr_pointee)
                    .map(|ty| self.parent.rust_type(ty)),
                unsafe_access,
                bitfield_storage: false,
            },
        );
    }

    pub(super) fn lower_complex_mul(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        if let Some(call) = self.complex_runtime_binop(op, "__mulsc3", "__muldc3") {
            self.materialize_expr(result, call, op_result_type(op));
            return;
        }
        let lhs = self.operand_expr(&op.operands[0]);
        let rhs = self.operand_expr(&op.operands[1]);
        let field = |base: Expr, name: &str| Expr::Field {
            base: Box::new(base),
            field: name.into(),
        };
        let ac = Expr::Binary {
            op: BinOp::Mul,
            lhs: Box::new(field(lhs.clone(), "re")),
            rhs: Box::new(field(rhs.clone(), "re")),
        };
        let bd = Expr::Binary {
            op: BinOp::Mul,
            lhs: Box::new(field(lhs.clone(), "im")),
            rhs: Box::new(field(rhs.clone(), "im")),
        };
        let ad = Expr::Binary {
            op: BinOp::Mul,
            lhs: Box::new(field(lhs.clone(), "re")),
            rhs: Box::new(field(rhs.clone(), "im")),
        };
        let bc = Expr::Binary {
            op: BinOp::Mul,
            lhs: Box::new(field(lhs, "im")),
            rhs: Box::new(field(rhs, "re")),
        };
        self.materialize_expr(
            result,
            Expr::StructLit {
                name: "Complex".into(),
                fields: vec![
                    (
                        "re".into(),
                        Expr::Binary {
                            op: BinOp::Sub,
                            lhs: Box::new(ac),
                            rhs: Box::new(bd),
                        },
                    ),
                    (
                        "im".into(),
                        Expr::Binary {
                            op: BinOp::Add,
                            lhs: Box::new(ad),
                            rhs: Box::new(bc),
                        },
                    ),
                ],
            },
            op_result_type(op),
        );
    }

    pub(super) fn lower_complex_div(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        if let Some(call) = self.complex_runtime_binop(op, "__divsc3", "__divdc3") {
            self.materialize_expr(result, call, op_result_type(op));
            return;
        }
        let lhs = self.operand_expr(&op.operands[0]);
        let rhs = self.operand_expr(&op.operands[1]);
        let field = |base: Expr, name: &str| Expr::Field {
            base: Box::new(base),
            field: name.into(),
        };
        let c = field(rhs.clone(), "re");
        let d = field(rhs.clone(), "im");
        let denom = self.next_temp();
        self.push_stmt(Stmt::Let {
            name: denom.clone(),
            mutable: false,
            ty: None,
            init: Some(Expr::Binary {
                op: BinOp::Add,
                lhs: Box::new(Expr::Binary {
                    op: BinOp::Mul,
                    lhs: Box::new(c.clone()),
                    rhs: Box::new(c),
                }),
                rhs: Box::new(Expr::Binary {
                    op: BinOp::Mul,
                    lhs: Box::new(d.clone()),
                    rhs: Box::new(d),
                }),
            }),
        });
        let denom_expr = || Expr::Var(denom.clone().into());
        let ac = Expr::Binary {
            op: BinOp::Mul,
            lhs: Box::new(field(lhs.clone(), "re")),
            rhs: Box::new(field(rhs.clone(), "re")),
        };
        let bd = Expr::Binary {
            op: BinOp::Mul,
            lhs: Box::new(field(lhs.clone(), "im")),
            rhs: Box::new(field(rhs.clone(), "im")),
        };
        let bc = Expr::Binary {
            op: BinOp::Mul,
            lhs: Box::new(field(lhs.clone(), "im")),
            rhs: Box::new(field(rhs.clone(), "re")),
        };
        let ad = Expr::Binary {
            op: BinOp::Mul,
            lhs: Box::new(field(lhs, "re")),
            rhs: Box::new(field(rhs, "im")),
        };
        self.materialize_expr(
            result,
            Expr::StructLit {
                name: "Complex".into(),
                fields: vec![
                    (
                        "re".into(),
                        Expr::Binary {
                            op: BinOp::Div,
                            lhs: Box::new(Expr::Binary {
                                op: BinOp::Add,
                                lhs: Box::new(ac),
                                rhs: Box::new(bd),
                            }),
                            rhs: Box::new(denom_expr()),
                        },
                    ),
                    (
                        "im".into(),
                        Expr::Binary {
                            op: BinOp::Div,
                            lhs: Box::new(Expr::Binary {
                                op: BinOp::Sub,
                                lhs: Box::new(bc),
                                rhs: Box::new(ad),
                            }),
                            rhs: Box::new(denom_expr()),
                        },
                    ),
                ],
            },
            op_result_type(op),
        );
    }

    pub(super) fn lower_complex_conj(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(src) = op.operands.first() else {
            return;
        };
        let src = self.operand_expr(src);
        self.materialize_expr(
            result,
            Expr::StructLit {
                name: "Complex".into(),
                fields: vec![
                    (
                        "re".into(),
                        Expr::Field {
                            base: Box::new(src.clone()),
                            field: "re".into(),
                        },
                    ),
                    (
                        "im".into(),
                        Expr::Unary {
                            op: UnaryOp::Neg,
                            expr: Box::new(Expr::Field {
                                base: Box::new(src),
                                field: "im".into(),
                            }),
                        },
                    ),
                ],
            },
            op_result_type(op),
        );
    }

    pub(super) fn complex_runtime_binop(
        &mut self,
        op: &Op,
        f32_name: &str,
        f64_name: &str,
    ) -> Option<Expr> {
        let inner = op_result_type(op).and_then(cir_complex_inner)?;
        let name = if inner == "!cir.float" {
            f32_name
        } else if inner == "!cir.double" {
            f64_name
        } else {
            return None;
        };
        let lhs = self.operand_expr(&op.operands[0]);
        let rhs = self.operand_expr(&op.operands[1]);
        let part = |base: Expr, field: &str| Expr::Field {
            base: Box::new(base),
            field: field.into(),
        };
        self.parent.uses_complex.set(true);
        Some(Self::unsafe_expr(Expr::Call {
            binding: crate::function_identity::CallBinding::Generated,
            func: Box::new(Expr::Var(name.into())),
            args: vec![
                part(lhs.clone(), "re"),
                part(lhs, "im"),
                part(rhs.clone(), "re"),
                part(rhs, "im"),
            ],
        }))
    }
}
