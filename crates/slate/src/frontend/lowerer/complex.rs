use super::*;

const COMPLEX: &str = "num_complex::Complex";

fn complex_literal(re: Expr, im: Expr) -> Expr {
    Expr::StructLit {
        name: COMPLEX.into(),
        fields: vec![("re".into(), re), ("im".into(), im)],
    }
}

fn method(recv: Expr, name: &str, args: Vec<Expr>) -> Expr {
    Expr::MethodCall {
        recv: Box::new(recv),
        method: name.into(),
        args,
    }
}

fn component_zero(component: ir::NumericType) -> Expr {
    match component {
        ir::NumericType::Float(ir::FloatType::F80) => long_double_literal(0),
        ir::NumericType::Float(_) => Expr::Value(rust::RustValue::Float(0.0.into())),
        _ => Expr::Value(rust::RustValue::I64(0)),
    }
}

fn is_f80(component: ir::NumericType) -> bool {
    matches!(component, ir::NumericType::Float(ir::FloatType::F80))
}

fn part(base: &Expr, field: &str) -> Expr {
    Expr::Field {
        base: Box::new(base.clone()),
        field: field.into(),
    }
}

fn shim(name: &str, args: Vec<Expr>) -> Expr {
    Expr::Call {
        func: Box::new(Expr::Var(name.into())),
        args,
        binding: CallBinding::Generated,
    }
}

fn binary(op: BinOp, lhs: Expr, rhs: Expr) -> Expr {
    Expr::Binary {
        op,
        lhs: Box::new(lhs),
        rhs: Box::new(rhs),
    }
}

fn negate(expr: Expr) -> Expr {
    Expr::Unary {
        op: rust::UnaryOp::Neg,
        expr: Box::new(expr),
    }
}

fn parts_block(stmts: Vec<Stmt>, re: Expr, im: Expr) -> Expr {
    Expr::Block(Box::new(rust::Block {
        stmts,
        tail: Some(Box::new(complex_literal(re, im))),
    }))
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn bind(&mut self, stmts: &mut Vec<Stmt>, init: Expr) -> Expr {
        let temp = self.next_temp();
        stmts.push(Stmt::Let {
            name: temp.clone(),
            mutable: false,
            ty: None,
            init: Some(init),
        });
        Expr::Var(temp.as_str().into())
    }

    fn conjugate(&mut self, operand: Expr) -> Expr {
        let mut stmts = Vec::new();
        let z = self.bind(&mut stmts, operand);
        parts_block(stmts, part(&z, "re"), negate(part(&z, "im")))
    }

    fn convert_component(
        &mut self,
        expr: Expr,
        from: ir::NumericType,
        to: ir::NumericType,
    ) -> Result<Expr> {
        let target = self.lower_type(&ir::Type::Numeric(to))?;
        let source = self.lower_type(&ir::Type::Numeric(from))?;
        let name = match (is_f80(from), is_f80(to)) {
            (true, true) => return Ok(expr),
            (false, false) => {
                return Ok(Expr::Cast {
                    expr: Box::new(expr),
                    ty: target,
                });
            }
            (false, true) => long_double::f80_cast_from_name(&source),
            (true, false) => long_double::f80_cast_to_name(&target),
        };
        let name = name.ok_or_else(|| {
            Failure::from(Construct::LongDouble {
                detail: format!("complex conversion {from} -> {to}"),
            })
        })?;
        Ok(shim(name, vec![expr]))
    }

    fn lower_complex_long_double_arith(
        &mut self,
        op: ir::ArithOp,
        (lhs, left_complex): (Expr, bool),
        (rhs, right_complex): (Expr, bool),
        basic: bool,
    ) -> Option<Expr> {
        let mul = if basic {
            "__slate_cf80_mul_basic"
        } else {
            "__slate_cf80_mul"
        };
        let div = if basic {
            "__slate_cf80_div_basic"
        } else {
            "__slate_cf80_div"
        };
        let mut stmts = Vec::new();
        Some(match (op, left_complex, right_complex) {
            (ir::ArithOp::Add, true, true) => shim("__slate_cf80_add", vec![lhs, rhs]),
            (ir::ArithOp::Sub, true, true) => shim("__slate_cf80_sub", vec![lhs, rhs]),
            (ir::ArithOp::Mul, true, true) => shim(mul, vec![lhs, rhs]),
            (ir::ArithOp::Div, true, true) => shim(div, vec![lhs, rhs]),
            (ir::ArithOp::Div, false, true) => {
                shim(div, vec![complex_literal(lhs, long_double_literal(0)), rhs])
            }
            (op, true, false) => {
                let z = self.bind(&mut stmts, lhs);
                let r = self.bind(&mut stmts, rhs);
                let (re, im) = match op {
                    ir::ArithOp::Add => (binary(BinOp::Add, part(&z, "re"), r), part(&z, "im")),
                    ir::ArithOp::Sub => (binary(BinOp::Sub, part(&z, "re"), r), part(&z, "im")),
                    ir::ArithOp::Mul => (
                        binary(BinOp::Mul, part(&z, "re"), r.clone()),
                        binary(BinOp::Mul, part(&z, "im"), r),
                    ),
                    ir::ArithOp::Div => (
                        binary(BinOp::Div, part(&z, "re"), r.clone()),
                        binary(BinOp::Div, part(&z, "im"), r),
                    ),
                    _ => return None,
                };
                parts_block(stmts, re, im)
            }
            (op, false, true) => {
                let r = self.bind(&mut stmts, lhs);
                let z = self.bind(&mut stmts, rhs);
                let (re, im) = match op {
                    ir::ArithOp::Add => (binary(BinOp::Add, r, part(&z, "re")), part(&z, "im")),
                    ir::ArithOp::Sub => (
                        binary(BinOp::Sub, r, part(&z, "re")),
                        negate(part(&z, "im")),
                    ),
                    ir::ArithOp::Mul => (
                        binary(BinOp::Mul, r.clone(), part(&z, "re")),
                        binary(BinOp::Mul, r, part(&z, "im")),
                    ),
                    _ => return None,
                };
                parts_block(stmts, re, im)
            }
            _ => return None,
        })
    }

    fn complex_component(&self, ty: &ir::Type) -> Option<ir::NumericType> {
        match self.tables.resolve_type(ty) {
            ir::Type::Complex(component) => Some(*component),
            _ => None,
        }
    }

    pub(super) fn lower_complex_value(&mut self, value: &ir::Value) -> Result<Option<Expr>> {
        match &value.node.value {
            ValueKind::Convert { .. } | ValueKind::Arith { .. } | ValueKind::Unary { .. }
                if self.in_static_initializer
                    && (!matches!(value.node.value, ValueKind::Convert { .. })
                        || self.tables.is_complex_long_double(&value.ty))
                    && let Some(component) = self.complex_component(&value.ty)
                    && let Some([re, im]) = self.tables.fold_complex_bits(value) =>
            {
                self.lower_type(&value.ty)?;
                let ty = ir::Type::Numeric(component);
                Ok(Some(complex_literal(
                    self.lower_number(&ir::Number::FloatBits(re), &ty)?,
                    self.lower_number(&ir::Number::FloatBits(im), &ty)?,
                )))
            }
            ValueKind::Aggregate { members, .. } if self.complex_component(&value.ty).is_some() => {
                let component = self.complex_component(&value.ty).into_iter().next();
                self.lower_type(&value.ty)?;
                let mut parts: [Option<Expr>; 2] = [None, None];
                for member in members {
                    let ir::AggregateTarget::Index(index @ (0 | 1)) = member.target else {
                        return Err(unsupported_value(value));
                    };
                    parts[index as usize] = Some(self.lower_value(&member.value)?);
                }
                let zero = || {
                    component
                        .map(component_zero)
                        .ok_or_else(|| unsupported_value(value))
                };
                let [re, im] = parts;
                Ok(Some(complex_literal(
                    re.map_or_else(zero, Ok)?,
                    im.map_or_else(zero, Ok)?,
                )))
            }
            ValueKind::Convert {
                kind: ir::ConversionKind::RealToComplex,
                operand,
                ..
            } => {
                let Some(component) = self.complex_component(&value.ty) else {
                    return Ok(None);
                };
                self.lower_type(&value.ty)?;
                Ok(Some(complex_literal(
                    self.lower_value(operand)?,
                    component_zero(component),
                )))
            }
            ValueKind::Convert {
                kind: ir::ConversionKind::ComplexToReal,
                operand,
                ..
            } if self.complex_component(&operand.ty).is_some() => {
                self.lower_type(&operand.ty)?;
                Ok(Some(Expr::Field {
                    base: Box::new(self.lower_value(operand)?),
                    field: "re".into(),
                }))
            }
            ValueKind::Convert {
                kind: ir::ConversionKind::ComplexConvert,
                operand,
                ..
            } if let Some(from) = self.complex_component(&operand.ty) => {
                let to = self
                    .complex_component(&value.ty)
                    .ok_or_else(|| unsupported_value(value))?;
                self.lower_type(&operand.ty)?;
                let lowered = self.lower_value(operand)?;
                let mut stmts = Vec::new();
                let z = self.bind(&mut stmts, lowered);
                let re = self.convert_component(part(&z, "re"), from, to)?;
                let im = self.convert_component(part(&z, "im"), from, to)?;
                Ok(Some(parts_block(stmts, re, im)))
            }
            ValueKind::Arith {
                op,
                left,
                right,
                semantics,
            } if self.complex_component(&value.ty).is_some() => {
                self.lower_type(&value.ty)?;
                let basic = matches!(
                    semantics,
                    ir::ArithSema::ComplexFloating {
                        range: ir::ComplexRange::Basic,
                        ..
                    }
                );
                let left_complex = self.complex_component(&left.ty).is_some();
                let right_complex = self.complex_component(&right.ty).is_some();
                let lhs = self.lower_value(left)?;
                let rhs = self.lower_value(right)?;
                if self.complex_component(&value.ty).is_some_and(is_f80) {
                    return self
                        .lower_complex_long_double_arith(
                            *op,
                            (lhs, left_complex),
                            (rhs, right_complex),
                            basic,
                        )
                        .map(Some)
                        .ok_or_else(|| unsupported_value(value));
                }
                let binary = |op| Expr::Binary {
                    op,
                    lhs: Box::new(lhs.clone()),
                    rhs: Box::new(rhs.clone()),
                };
                Ok(Some(match (op, left_complex, right_complex) {
                    (ir::ArithOp::Add, ..) => binary(BinOp::Add),
                    (ir::ArithOp::Sub, ..) => binary(BinOp::Sub),
                    (ir::ArithOp::Mul, true, true) => method(
                        lhs,
                        "c_mul",
                        vec![rhs, Expr::Value(rust::RustValue::Bool(basic))],
                    ),
                    (ir::ArithOp::Mul, ..) => binary(BinOp::Mul),
                    (ir::ArithOp::Div, true, true) => method(
                        lhs,
                        "c_div",
                        vec![rhs, Expr::Value(rust::RustValue::Bool(basic))],
                    ),
                    (ir::ArithOp::Div, true, false) => binary(BinOp::Div),
                    (ir::ArithOp::Div, false, _) => {
                        let component = self
                            .complex_component(&value.ty)
                            .ok_or_else(|| unsupported_value(value))?;
                        method(
                            complex_literal(lhs, component_zero(component)),
                            "c_div",
                            vec![rhs, Expr::Value(rust::RustValue::Bool(basic))],
                        )
                    }
                    _ => return Err(unsupported_value(value)),
                }))
            }
            ValueKind::Unary {
                op: op @ (ir::UnaryArithOp::Neg | ir::UnaryArithOp::Not),
                operand,
                ..
            } if self.complex_component(&value.ty).is_some() => {
                self.lower_type(&value.ty)?;
                let operand = self.lower_value(operand)?;
                let long_double = self.complex_component(&value.ty).is_some_and(is_f80);
                Ok(Some(match op {
                    ir::UnaryArithOp::Neg if long_double => shim("__slate_cf80_neg", vec![operand]),
                    ir::UnaryArithOp::Neg => negate(operand),
                    _ if long_double => self.conjugate(operand),
                    _ => method(operand, "conj", Vec::new()),
                }))
            }
            ValueKind::Compare {
                op: op @ (ir::CompareOp::Eq | ir::CompareOp::Ne),
                left,
                right,
                ..
            } if let Some(component) = self
                .complex_component(&left.ty)
                .or_else(|| self.complex_component(&right.ty)) =>
            {
                self.lower_type(&ir::Type::Complex(component))?;
                let mut sides = [None, None];
                for (side, operand) in sides.iter_mut().zip([left, right]) {
                    let lowered = self.lower_value(operand)?;
                    *side = Some(if self.complex_component(&operand.ty).is_some() {
                        lowered
                    } else {
                        complex_literal(lowered, component_zero(component))
                    });
                }
                let [Some(lhs), Some(rhs)] = sides else {
                    return Err(unsupported_value(value));
                };
                if self.in_static_initializer && !is_f80(component) {
                    let (compare, combine) = if matches!(op, ir::CompareOp::Eq) {
                        (BinOp::Eq, BinOp::And)
                    } else {
                        (BinOp::Ne, BinOp::Or)
                    };
                    let [re, im] = ["re", "im"].map(|field| Expr::Binary {
                        op: compare,
                        lhs: Box::new(part(&lhs, field)),
                        rhs: Box::new(part(&rhs, field)),
                    });
                    return Ok(Some(binary(combine, re, im)));
                }
                Ok(Some(Expr::Binary {
                    op: if matches!(op, ir::CompareOp::Eq) {
                        BinOp::Eq
                    } else {
                        BinOp::Ne
                    },
                    lhs: Box::new(lhs),
                    rhs: Box::new(rhs),
                }))
            }
            ValueKind::Call {
                callee: ir::Callee::Direct(id),
                arguments,
                ..
            } if let [argument] = arguments.as_slice()
                && let Some(part) = self.tables.builtin_name(*id).and_then(complex_builtin) =>
            {
                self.lower_type(&argument.ty)?;
                let operand = self.lower_value(argument)?;
                Ok(Some(match part {
                    "conj" if self.complex_component(&argument.ty).is_some_and(is_f80) => {
                        self.conjugate(operand)
                    }
                    "conj" => method(operand, "conj", Vec::new()),
                    field => Expr::Field {
                        base: Box::new(operand),
                        field: field.into(),
                    },
                }))
            }
            _ => Ok(None),
        }
    }
}

fn complex_builtin(name: &str) -> Option<&'static str> {
    match name {
        "__builtin_creal" | "__builtin_crealf" | "__builtin_creall" => Some("re"),
        "__builtin_cimag" | "__builtin_cimagf" | "__builtin_cimagl" => Some("im"),
        "__builtin_conj" | "__builtin_conjf" | "__builtin_conjl" => Some("conj"),
        _ => None,
    }
}
