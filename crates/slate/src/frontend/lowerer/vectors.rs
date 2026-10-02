use super::*;

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
        self.lower_type(&value.ty)?;
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
        Ok(match (op, element) {
            (ir::UnaryArithOp::Not, ir::NumericType::Integer { .. }) => Expr::Unary {
                op: rust::UnaryOp::Not,
                expr: Box::new(lowered),
            },
            (ir::UnaryArithOp::Neg, ir::NumericType::Integer { signed: false, .. }) => {
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
        self.lower_type(&value.ty)?;
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
                type_args: vec![self.lower_type(&ir::Type::Numeric(to))?],
            }),
            args: vec![self.lower_value(operand)?],
            binding: CallBinding::Generated,
        })
    }
}
