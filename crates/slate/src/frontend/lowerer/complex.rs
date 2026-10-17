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
        ir::NumericType::Float(_) => Expr::Value(rust::RustValue::Float(0.0.into())),
        _ => Expr::Value(rust::RustValue::I64(0)),
    }
}

impl FunctionLowerer<'_, '_> {
    fn complex_component(&self, ty: &ir::Type) -> Option<ir::NumericType> {
        match self.tables.resolve_type(ty) {
            ir::Type::Complex(component) => Some(*component),
            _ => None,
        }
    }

    pub(super) fn lower_complex_value(&mut self, value: &ir::Value) -> Result<Option<Expr>> {
        match &value.node.value {
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
            } if self.complex_component(&operand.ty).is_some() => {
                let target = self.lower_type(&ir::Type::Numeric(
                    self.complex_component(&value.ty)
                        .ok_or_else(|| unsupported_value(value))?,
                ))?;
                self.lower_type(&operand.ty)?;
                let temp = self.next_temp();
                let part = |field: &str| Expr::Cast {
                    expr: Box::new(Expr::Field {
                        base: Box::new(Expr::Var(temp.as_str().into())),
                        field: field.into(),
                    }),
                    ty: target.clone(),
                };
                let literal = complex_literal(part("re"), part("im"));
                Ok(Some(Expr::Block(Box::new(rust::Block {
                    stmts: vec![Stmt::Let {
                        name: temp.clone(),
                        mutable: false,
                        ty: None,
                        init: Some(self.lower_value(operand)?),
                    }],
                    tail: Some(Box::new(literal)),
                }))))
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
                Ok(Some(match op {
                    ir::UnaryArithOp::Neg => Expr::Unary {
                        op: rust::UnaryOp::Neg,
                        expr: Box::new(operand),
                    },
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
        "__builtin_creal" | "__builtin_crealf" => Some("re"),
        "__builtin_cimag" | "__builtin_cimagf" => Some("im"),
        "__builtin_conj" | "__builtin_conjf" => Some("conj"),
        _ => None,
    }
}
