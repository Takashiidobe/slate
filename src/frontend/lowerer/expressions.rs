use super::*;

impl<'a, 'b> FunctionLowerer<'a, 'b> {
    pub(super) fn lower_complex_create(
        &mut self,
        result: &str,
        result_ty: &CirType,
        real: &str,
        imag: &str,
    ) {
        self.materialize_expr(
            result,
            Expr::StructLit {
                name: COMPLEX_TY.into(),
                fields: vec![
                    ("re".into(), self.operand_expr(real)),
                    ("im".into(), self.operand_expr(imag)),
                ],
            },
            Some(result_ty),
        );
    }

    pub(super) fn lower_complex_addsub(
        &mut self,
        result: &str,
        result_ty: &CirType,
        lhs: &str,
        rhs: &str,
        component_op: BinOp,
    ) {
        let lhs = self.operand_expr(lhs);
        let rhs = self.operand_expr(rhs);
        let field = |base: Expr, name: &str| Expr::Field {
            base: Box::new(base),
            field: name.into(),
        };
        self.materialize_expr(
            result,
            Expr::StructLit {
                name: COMPLEX_TY.into(),
                fields: vec![
                    (
                        "re".into(),
                        Expr::Binary {
                            op: component_op,
                            lhs: Box::new(field(lhs.clone(), "re")),
                            rhs: Box::new(field(rhs.clone(), "re")),
                        },
                    ),
                    (
                        "im".into(),
                        Expr::Binary {
                            op: component_op,
                            lhs: Box::new(field(lhs, "im")),
                            rhs: Box::new(field(rhs, "im")),
                        },
                    ),
                ],
            },
            Some(result_ty),
        );
    }

    pub(super) fn lower_complex_part(
        &mut self,
        result: &str,
        result_ty: &CirType,
        src: &str,
        field: &str,
    ) {
        self.materialize_expr(
            result,
            Expr::Field {
                base: Box::new(self.operand_expr(src)),
                field: field.into(),
            },
            Some(result_ty),
        );
    }

    pub(super) fn lower_complex_part_ptr(
        &mut self,
        result: &str,
        result_ty: &CirType,
        base_ptr: &str,
        field: &str,
    ) {
        let base = self.place_or_deref_expr(base_ptr);
        let unsafe_access =
            self.place_expr(base_ptr).is_none() || self.ptr_requires_unsafe(base_ptr);
        self.member_ptrs.insert(
            result.to_string(),
            MemberPtr {
                base,
                field: field.into(),
                field_ty: result_ty.pointee().map(|ty| self.parent.rust_type(ty)),
                unsafe_access,
                bitfield_name: None,
                bitfield_unaligned: false,
                field_is_trailing: false,
            },
        );
    }

    pub(super) fn lower_complex_mul(
        &mut self,
        result: &str,
        result_ty: &CirType,
        lhs: &str,
        rhs: &str,
    ) {
        if let Some(call) = self.complex_runtime_binop(result_ty, lhs, rhs, "__mulsc3", "__muldc3")
        {
            self.materialize_expr(result, call, Some(result_ty));
            return;
        }
        let lhs = self.operand_expr(lhs);
        let rhs = self.operand_expr(rhs);
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
                name: COMPLEX_TY.into(),
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
            Some(result_ty),
        );
    }

    pub(super) fn lower_complex_div(
        &mut self,
        result: &str,
        result_ty: &CirType,
        lhs: &str,
        rhs: &str,
    ) {
        if let Some(call) = self.complex_runtime_binop(result_ty, lhs, rhs, "__divsc3", "__divdc3")
        {
            self.materialize_expr(result, call, Some(result_ty));
            return;
        }
        let lhs = self.operand_expr(lhs);
        let rhs = self.operand_expr(rhs);
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
                name: COMPLEX_TY.into(),
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
            Some(result_ty),
        );
    }

    pub(super) fn lower_complex_conj(&mut self, result: &str, result_ty: &CirType, src: &str) {
        let src = self.operand_expr(src);
        self.materialize_expr(
            result,
            Expr::StructLit {
                name: COMPLEX_TY.into(),
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
            Some(result_ty),
        );
    }

    pub(super) fn complex_runtime_binop(
        &mut self,
        result_ty: &CirType,
        lhs: &str,
        rhs: &str,
        f32_name: &str,
        f64_name: &str,
    ) -> Option<Expr> {
        let inner = match result_ty {
            CirType::Complex { element_type } => element_type.as_ref(),
            _ => return None,
        };
        let name = match inner {
            CirType::Single => f32_name,
            CirType::Double => f64_name,
            _ => return None,
        };
        let lhs = self.operand_expr(lhs);
        let rhs = self.operand_expr(rhs);
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
