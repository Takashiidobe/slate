use super::*;

#[derive(Clone, Copy)]
pub(super) struct WideInt {
    width: u32,
    pub(super) signed: bool,
    bytes: u64,
    align: u32,
}

impl WideInt {
    pub(super) fn rust_type(self) -> rust::Type {
        rust::Type::Custom(format!(
            "bitint::{}<{}, {}, {}, bitint::A{}>",
            if self.signed { "BInt" } else { "BUint" },
            self.width,
            self.width.div_ceil(64),
            self.bytes,
            self.align,
        ))
    }

    pub(super) fn exceeds_i128(self) -> bool {
        self.width > 128
    }

    pub(super) fn switch_operand(self, expr: Expr) -> (Expr, rust::Type) {
        if self.exceeds_i128() {
            return (expr, self.rust_type());
        }
        let (method, prim) = if self.signed {
            ("to_i128", Prim::I128)
        } else {
            ("to_u128", Prim::U128)
        };
        (
            Expr::MethodCall {
                recv: Box::new(expr),
                method: method.into(),
                args: Vec::new(),
            },
            rust::Type::Prim(prim),
        )
    }

    fn function(self, name: &str) -> Expr {
        Expr::Var(
            format!(
                "<{}>::{name}",
                crate::backend::codegen::type_to_string(&self.rust_type())
            )
            .into(),
        )
    }

    fn call(self, name: &str, args: Vec<Expr>) -> Expr {
        Expr::Call {
            func: Box::new(self.function(name)),
            args,
            binding: CallBinding::Generated,
        }
    }
}

impl Tables<'_> {
    pub(super) fn wide_int(&self, ty: &ir::Type) -> Option<WideInt> {
        let ir::Type::Numeric(
            numeric @ ir::NumericType::Integer {
                width,
                signed,
                bit_precise: true,
            },
        ) = *self.resolve_type(ty)
        else {
            return None;
        };
        if width <= 64 {
            return None;
        }
        let layout = self.target.storage_of(ir::Type::Numeric(numeric)).ok()?;
        if width == 128 && layout.alignment_bytes == 16 {
            return None;
        }
        Some(WideInt {
            width,
            signed,
            bytes: layout.size_bytes,
            align: layout.alignment_bytes,
        })
    }
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn wide_int_constant(&mut self, wide: WideInt, number: &Number) -> Result<Expr> {
        let (name, value) = match number {
            Number::Integer(value) => match u128::try_from(value) {
                Ok(value) => ("from_u128", rust::RustValue::U128(value)),
                Err(_) => {
                    return Ok(wide.call("from_decimal_str", vec![Expr::Str(value.to_string())]));
                }
            },
            Number::SignedInteger(value) => match i128::try_from(value) {
                Ok(value) => ("from_i128", rust::RustValue::I128(value)),
                Err(_) => {
                    return Ok(wide.call("from_decimal_str", vec![Expr::Str(value.to_string())]));
                }
            },
            _ => return Err(unsupported_constant(number)),
        };
        Ok(wide.call(name, vec![Expr::Value(value)]))
    }

    pub(super) fn lower_wide_int_conversion(
        &mut self,
        value: &ir::Value,
        operand: &ir::Value,
    ) -> Result<Expr> {
        let source = self.tables.wide_int(&operand.ty);
        let target = self.tables.wide_int(&value.ty);
        let lowered = self.lower_value(operand)?;
        match (source, target) {
            (Some(source), Some(target)) => Ok(target.call(
                if source.signed {
                    "from_bint"
                } else {
                    "from_buint"
                },
                vec![lowered],
            )),
            (None, Some(target)) => {
                let signed = match self.tables.resolve_type(&operand.ty) {
                    ir::Type::Numeric(ir::NumericType::Integer { signed, .. }) => *signed,
                    ir::Type::Bool => false,
                    _ => return Err(unsupported_value(value)),
                };
                let (name, prim) = if signed {
                    ("from_i128", Prim::I128)
                } else {
                    ("from_u128", Prim::U128)
                };
                Ok(target.call(
                    name,
                    vec![Expr::Cast {
                        expr: Box::new(lowered),
                        ty: rust::Type::Prim(prim),
                    }],
                ))
            }
            (Some(source), None) => {
                if !matches!(
                    self.tables.resolve_type(&value.ty),
                    ir::Type::Numeric(ir::NumericType::Integer { .. })
                ) {
                    return Err(unsupported_value(value));
                }
                let wide = Expr::MethodCall {
                    recv: Box::new(lowered),
                    method: if source.signed { "to_i128" } else { "to_u128" }.into(),
                    args: Vec::new(),
                };
                self.narrow_integer(wide, &value.ty)
            }
            (None, None) => Err(unsupported_value(value)),
        }
    }

    pub(super) fn lower_wide_int_arith(
        &mut self,
        op: ir::ArithOp,
        left: &ir::Value,
        right: &ir::Value,
    ) -> Result<Option<Expr>> {
        let method = match op {
            ir::ArithOp::Add => "wrapping_add",
            ir::ArithOp::Sub => "wrapping_sub",
            ir::ArithOp::Mul => "wrapping_mul",
            ir::ArithOp::Div => "wrapping_div",
            ir::ArithOp::Rem => "wrapping_rem",
            ir::ArithOp::And => "bitand_bits",
            ir::ArithOp::Or => "bitor_bits",
            ir::ArithOp::Xor => "bitxor_bits",
            _ => return Ok(None),
        };
        Ok(Some(Expr::MethodCall {
            recv: Box::new(self.lower_value(left)?),
            method: method.into(),
            args: vec![self.lower_value(right)?],
        }))
    }

    pub(super) fn lower_small_bitint_arith(
        &mut self,
        value: &ir::Value,
        op: ir::ArithOp,
        left: &ir::Value,
        right: &ir::Value,
    ) -> Result<Option<Expr>> {
        let method = match op {
            ir::ArithOp::Add => "wrapping_add",
            ir::ArithOp::Sub => "wrapping_sub",
            ir::ArithOp::Mul => "wrapping_mul",
            ir::ArithOp::Div => "wrapping_div",
            ir::ArithOp::Rem => "wrapping_rem",
            ir::ArithOp::And | ir::ArithOp::Or | ir::ArithOp::Xor => {
                return Ok(Some(Expr::Binary {
                    op: match op {
                        ir::ArithOp::And => BinOp::BitAnd,
                        ir::ArithOp::Or => BinOp::BitOr,
                        _ => BinOp::BitXor,
                    },
                    lhs: Box::new(self.lower_value(left)?),
                    rhs: Box::new(self.lower_value(right)?),
                }));
            }
            _ => return Ok(None),
        };
        let wrapped = Expr::MethodCall {
            recv: Box::new(self.lower_value(left)?),
            method: method.into(),
            args: vec![self.lower_value(right)?],
        };
        self.narrow_integer(wrapped, &value.ty).map(Some)
    }

    pub(super) fn lower_wide_int_compare(
        &mut self,
        op: ir::CompareOp,
        left: &ir::Value,
        right: &ir::Value,
    ) -> Result<Expr> {
        Ok(Expr::Binary {
            op: match op {
                ir::CompareOp::Eq => BinOp::Eq,
                ir::CompareOp::Ne => BinOp::Ne,
                ir::CompareOp::Lt => BinOp::Lt,
                ir::CompareOp::Le => BinOp::Le,
                ir::CompareOp::Gt => BinOp::Gt,
                ir::CompareOp::Ge => BinOp::Ge,
            },
            lhs: Box::new(Expr::MethodCall {
                recv: Box::new(self.lower_value(left)?),
                method: "compare".into(),
                args: vec![self.lower_value(right)?],
            }),
            rhs: Box::new(Expr::Value(rust::RustValue::I64(0))),
        })
    }
}
