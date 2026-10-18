use super::*;

const SLATE_VECTOR: &str = "__slate_vector::__SlateVector";

pub(super) fn slate_vector_type(
    lane: rust::Type,
    lanes: u64,
    alignment: u64,
) -> Option<rust::Type> {
    if !matches!(
        lane,
        rust::Type::Prim(
            Prim::I8
                | Prim::U8
                | Prim::I16
                | Prim::U16
                | Prim::I32
                | Prim::U32
                | Prim::I64
                | Prim::U64
                | Prim::I128
                | Prim::U128
                | Prim::F32
                | Prim::F64
        )
    ) || !alignment.is_power_of_two()
        || alignment > 4096
    {
        return None;
    }
    Some(rust::Type::Generic {
        name: SLATE_VECTOR.into(),
        args: vec![
            lane,
            rust::Type::Custom(lanes.to_string()),
            rust::Type::Custom(format!("__slate_vector::Align{alignment}")),
        ],
    })
}

pub(super) fn is_slate_vector(ty: &rust::Type) -> bool {
    matches!(ty, rust::Type::Generic { name, .. } if name == SLATE_VECTOR)
}

pub(super) fn vector_constructor(ty: &rust::Type, method: &str) -> Expr {
    let base = if is_slate_vector(ty) {
        SLATE_VECTOR
    } else {
        "std::simd::Simd"
    };
    Expr::Var(format!("{base}::{method}").into())
}

impl Tables<'_> {
    pub(super) fn vector_element(&self, ty: &ir::Type) -> Option<ir::NumericType> {
        match self.resolve_type(ty) {
            ir::Type::Vector { element, .. } => Some(*element),
            _ => None,
        }
    }
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn lower_shuffle(
        &mut self,
        value: &ir::Value,
        left: &ir::Value,
        right: Option<&ir::Value>,
        mask: &ir::ShuffleMask,
    ) -> Result<Expr> {
        let ir::ShuffleMask::Lanes(lanes) = mask else {
            return Err(unsupported_value(value));
        };
        let result = self.lower_type(&value.ty)?;
        let operands = std::iter::once(left).chain(right);
        if is_slate_vector(&result)
            || operands
                .clone()
                .map(|operand| self.lower_type(&operand.ty).map(|ty| is_slate_vector(&ty)))
                .collect::<Result<Vec<_>>>()?
                .contains(&true)
        {
            let width = match self.tables.resolve_type(&left.ty) {
                ir::Type::Vector { lanes, .. } => u64::from(*lanes),
                _ => return Err(unsupported_value(value)),
            };
            let mut stmts = Vec::new();
            let mut sources = Vec::new();
            for operand in operands {
                let lowered = self.lower_value(operand)?;
                let array = Expr::MethodCall {
                    recv: Box::new(lowered),
                    method: "to_array".into(),
                    args: Vec::new(),
                };
                sources.push(self.bind(&mut stmts, array));
            }
            let lanes = lanes
                .iter()
                .map(|lane| {
                    let lane = u64::from(lane.unwrap_or(0));
                    let (source, index) = if lane < width {
                        (sources.first(), lane)
                    } else {
                        (sources.get(1), lane - width)
                    };
                    Ok(Expr::Index {
                        base: Box::new(source.ok_or_else(|| unsupported_value(value))?.clone()),
                        index: Box::new(Expr::Value(rust::RustValue::U128(index.into()))),
                    })
                })
                .collect::<Result<Vec<_>>>()?;
            return Ok(Expr::Block(Box::new(rust::Block {
                stmts,
                tail: Some(Box::new(Expr::Call {
                    func: Box::new(vector_constructor(&result, "from_array")),
                    args: vec![Expr::ArrayLit(lanes)],
                    binding: CallBinding::Generated,
                })),
            })));
        }
        let mut args = vec![self.lower_value(left)?];
        if let Some(right) = right {
            args.push(self.lower_value(right)?);
        }
        args.push(Expr::ArrayLit(
            lanes
                .iter()
                .map(|lane| Expr::Value(rust::RustValue::I64(i64::from(lane.unwrap_or(0)))))
                .collect(),
        ));
        Ok(Expr::Macro {
            name: "std::simd::simd_swizzle".into(),
            args,
        })
    }

    pub(super) fn lower_vector_compare(
        &mut self,
        value: &ir::Value,
        op: ir::CompareOp,
        left: &ir::Value,
        right: &ir::Value,
    ) -> Result<Expr> {
        let element = self
            .tables
            .vector_element(&value.ty)
            .ok_or_else(|| unsupported_value(value))?;
        let result = self.lower_type(&value.ty)?;
        let lane = self.lower_type(&ir::Type::Numeric(element))?;
        let mut stmts = Vec::new();
        let mut arrays = Vec::new();
        for operand in [left, right] {
            let vector = self.next_temp();
            stmts.push(Stmt::Let {
                name: vector.clone(),
                mutable: false,
                ty: Some(self.lower_type(&operand.ty)?),
                init: Some(self.lower_value(operand)?),
            });
            let array = Expr::MethodCall {
                recv: Box::new(Expr::Var(vector.into())),
                method: "to_array".into(),
                args: Vec::new(),
            };
            arrays.push(self.bind(&mut stmts, array));
        }
        let at = |array: &Expr| Expr::Index {
            base: Box::new(array.clone()),
            index: Box::new(Expr::Var("__lane".into())),
        };
        let compare = Expr::Binary {
            op: match op {
                ir::CompareOp::Eq => BinOp::Eq,
                ir::CompareOp::Ne => BinOp::Ne,
                ir::CompareOp::Lt => BinOp::Lt,
                ir::CompareOp::Le => BinOp::Le,
                ir::CompareOp::Gt => BinOp::Gt,
                ir::CompareOp::Ge => BinOp::Ge,
            },
            lhs: Box::new(at(&arrays[0])),
            rhs: Box::new(at(&arrays[1])),
        };
        let mask = Expr::Unary {
            op: rust::UnaryOp::Neg,
            expr: Box::new(Expr::Cast {
                expr: Box::new(compare),
                ty: lane,
            }),
        };
        let temp = self.next_temp();
        stmts.push(Stmt::Let {
            name: temp.clone(),
            mutable: false,
            ty: Some(result.clone()),
            init: Some(Expr::Call {
                func: Box::new(vector_constructor(&result, "from_array")),
                args: vec![Expr::Call {
                    func: Box::new(Expr::Var("std::array::from_fn".into())),
                    args: vec![Expr::Closure {
                        params: vec!["__lane".into()],
                        body: Box::new(mask),
                    }],
                    binding: CallBinding::Generated,
                }],
                binding: CallBinding::Generated,
            }),
        });
        Ok(Expr::Block(Box::new(rust::Block {
            stmts,
            tail: Some(Box::new(Expr::Var(temp.into()))),
        })))
    }

    pub(super) fn lower_vector_unary(
        &mut self,
        value: &ir::Value,
        op: ir::UnaryArithOp,
        operand: &ir::Value,
    ) -> Result<Expr> {
        let element = self
            .tables
            .vector_element(&value.ty)
            .ok_or_else(|| unsupported_value(value))?;
        let lowered = self.lower_value(operand)?;
        let fallback = is_slate_vector(&self.lower_type(&value.ty)?);
        Ok(match (op, element) {
            (ir::UnaryArithOp::Not, ir::NumericType::Integer { .. }) => Expr::Unary {
                op: rust::UnaryOp::Not,
                expr: Box::new(lowered),
            },
            (ir::UnaryArithOp::Neg, ir::NumericType::Integer { signed: false, .. })
                if !fallback =>
            {
                Expr::Binary {
                    op: BinOp::Sub,
                    lhs: Box::new(Expr::Call {
                        func: Box::new(Expr::Var("std::simd::Simd::splat".into())),
                        args: vec![Expr::Value(rust::RustValue::I64(0))],
                        binding: CallBinding::Generated,
                    }),
                    rhs: Box::new(lowered),
                }
            }
            (ir::UnaryArithOp::Neg, _) => Expr::Unary {
                op: rust::UnaryOp::Neg,
                expr: Box::new(lowered),
            },
            _ => return Err(unsupported_value(value)),
        })
    }

    pub(super) fn lower_vector_convert(
        &mut self,
        value: &ir::Value,
        kind: ir::ConversionKind,
        operand: &ir::Value,
    ) -> Result<Expr> {
        let (Some(from), Some(to)) = (
            self.tables.vector_element(&operand.ty),
            self.tables.vector_element(&value.ty),
        ) else {
            return Err(unsupported_value(value));
        };
        let lane_cast = matches!(
            kind,
            ir::ConversionKind::Widen
                | ir::ConversionKind::Truncate
                | ir::ConversionKind::Reinterpret
                | ir::ConversionKind::IntToFloat
                | ir::ConversionKind::FloatToInt
                | ir::ConversionKind::FloatWiden
                | ir::ConversionKind::FloatNarrow
        );
        if !lane_cast {
            return Err(unsupported_value(value));
        }
        let target = self.lower_type(&value.ty)?;
        let lane = self.lower_type(&ir::Type::Numeric(to))?;
        if is_slate_vector(&target) || is_slate_vector(&self.lower_type(&operand.ty)?) {
            let array = Expr::MethodCall {
                recv: Box::new(self.lower_value(operand)?),
                method: "to_array".into(),
                args: Vec::new(),
            };
            return Ok(Expr::Call {
                func: Box::new(vector_constructor(&target, "from_array")),
                args: vec![Expr::MethodCall {
                    recv: Box::new(array),
                    method: "map".into(),
                    args: vec![Expr::Closure {
                        params: vec!["__lane".into()],
                        body: Box::new(Expr::Cast {
                            expr: Box::new(Expr::Var("__lane".into())),
                            ty: lane,
                        }),
                    }],
                }],
                binding: CallBinding::Generated,
            });
        }
        let trait_name = match from {
            ir::NumericType::Integer { signed: true, .. } => "SimdInt",
            ir::NumericType::Integer { signed: false, .. } => "SimdUint",
            ir::NumericType::Float(_) => "SimdFloat",
        };
        Ok(Expr::Call {
            func: Box::new(Expr::GenericPath {
                path: rust::Path::new(
                    ["std", "simd", "num", trait_name, "cast"].map(rust::Ident::from),
                ),
                type_args: vec![lane],
            }),
            args: vec![self.lower_value(operand)?],
            binding: CallBinding::Generated,
        })
    }
}
