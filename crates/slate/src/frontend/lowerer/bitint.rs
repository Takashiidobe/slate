use super::*;

#[derive(Clone, Copy)]
pub(super) struct WideInt {
    width: u32,
    pub(super) signed: bool,
    bytes: u64,
    align: u32,
}

impl WideInt {
    pub(super) fn fits_registers(self) -> bool {
        self.bytes <= 16
    }

    pub(super) fn is_huge(self) -> bool {
        self.bytes >= 1 << 16
    }

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

    fn bytes_literal(self, bytes: &[u8]) -> Expr {
        Expr::Call {
            func: Box::new(Expr::Var(
                format!(
                    "<{}>::from_le_bytes::<{}, {}>",
                    crate::backend::codegen::type_to_string(&self.rust_type()),
                    bytes.len() * 8,
                    bytes.len()
                )
                .into(),
            )),
            args: vec![Expr::ArrayLit(
                bytes
                    .iter()
                    .map(|byte| Expr::Value(rust::RustValue::I64(i64::from(*byte))))
                    .collect(),
            )],
            binding: CallBinding::Generated,
        }
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

pub(super) fn is_stdc_bit(builtin: &str) -> bool {
    matches!(
        builtin,
        "__builtin_stdc_leading_zeros"
            | "__builtin_stdc_leading_ones"
            | "__builtin_stdc_trailing_zeros"
            | "__builtin_stdc_trailing_ones"
            | "__builtin_stdc_first_leading_zero"
            | "__builtin_stdc_first_leading_one"
            | "__builtin_stdc_first_trailing_zero"
            | "__builtin_stdc_first_trailing_one"
            | "__builtin_stdc_count_zeros"
            | "__builtin_stdc_count_ones"
            | "__builtin_stdc_has_single_bit"
            | "__builtin_stdc_bit_width"
            | "__builtin_stdc_bit_floor"
            | "__builtin_stdc_bit_ceil"
    )
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn lower_stdc_bit(
        &mut self,
        value: &ir::Value,
        builtin: &str,
        arguments: &[ir::Value],
    ) -> Result<Expr> {
        let [operand] = arguments else {
            return Err(unsupported_value(value));
        };
        let wide = self.tables.wide_int(&operand.ty);
        let (shape, width) = match (wide, self.tables.bit_shape(&operand.ty)) {
            (Some(wide), _) => (BitShape::Plain, wide.width),
            (None, Some(BitShape::Small { width, container })) => {
                (BitShape::Small { width, container }, width)
            }
            (None, Some(BitShape::Plain)) => match self.lower_type(&operand.ty)? {
                rust::Type::Prim(prim) => (BitShape::Plain, prim_width(prim)),
                _ => return Err(unsupported_value(value)),
            },
            (None, None) => return Err(unsupported_value(value)),
        };
        let operation = builtin.trim_start_matches("__builtin_stdc_");
        let bits = self.next_temp();
        let var = |name: &str| Expr::Var(name.into());
        let u32_literal = |value: u32| Expr::Cast {
            expr: Box::new(Expr::Value(rust::RustValue::U128(u128::from(value)))),
            ty: rust::Type::Prim(Prim::U32),
        };
        let binary = |op: BinOp, lhs: Expr, rhs: Expr| Expr::Binary {
            op,
            lhs: Box::new(lhs),
            rhs: Box::new(rhs),
        };
        let conditional = |cond: Expr, then_expr: Expr, else_expr: Expr| Expr::If {
            cond: Box::new(cond),
            then_expr: Box::new(then_expr),
            else_expr: Box::new(else_expr),
        };
        let bind = |name: &str, init: Expr| Stmt::Let {
            name: name.to_string(),
            mutable: false,
            ty: None,
            init: Some(init),
        };
        let inverse = if operation.ends_with("_ones")
            || operation == "first_leading_zero"
            || operation == "first_trailing_zero"
        {
            let flipped = Expr::Unary {
                op: rust::UnaryOp::Not,
                expr: Box::new(var(&bits)),
            };
            Some(if wide.is_some() {
                flipped
            } else {
                self.narrow_integer(flipped, &operand.ty)?
            })
        } else {
            None
        };
        let inverse_expr = inverse.unwrap_or_else(|| var(&bits));
        let operand_value = self.lower_value(operand)?;
        let result = self.lower_type(&value.ty)?;
        let counted = |recv: Expr, method: &str| bit_count_expr(shape, recv, method);
        let finish = |expr: Expr| Expr::Cast {
            expr: Box::new(expr),
            ty: result.clone(),
        };
        let first = |this: &mut Self, source: Expr, method: &str| -> Expr {
            let count = this.next_temp();
            Expr::Block(Box::new(rust::Block {
                stmts: vec![bind(&count, counted(source, method))],
                tail: Some(Box::new(conditional(
                    binary(BinOp::Eq, var(&count), u32_literal(width)),
                    u32_literal(0),
                    binary(BinOp::Add, var(&count), u32_literal(1)),
                ))),
            }))
        };
        let shifted = |this: &mut Self, amount: Expr| -> Result<Expr> {
            let one = this.lower_number(&Number::Integer(1u32.into()), &operand.ty)?;
            if wide.is_some() {
                return Ok(Expr::MethodCall {
                    recv: Box::new(one),
                    method: "shl_bits".into(),
                    args: vec![Expr::Cast {
                        expr: Box::new(amount),
                        ty: rust::Type::Prim(Prim::Usize),
                    }],
                });
            }
            let zero = this.lower_number(&Number::Integer(0u32.into()), &operand.ty)?;
            let shifted = Expr::MethodCall {
                recv: Box::new(Expr::MethodCall {
                    recv: Box::new(one),
                    method: "checked_shl".into(),
                    args: vec![amount],
                }),
                method: "unwrap_or".into(),
                args: vec![zero],
            };
            this.narrow_integer(shifted, &operand.ty)
        };
        let leading = || counted(var(&bits), "leading_zeros");
        let tail = match operation {
            "leading_zeros" => finish(leading()),
            "leading_ones" => finish(counted(inverse_expr.clone(), "leading_zeros")),
            "trailing_zeros" => finish(counted(var(&bits), "trailing_zeros")),
            "trailing_ones" => finish(counted(inverse_expr.clone(), "trailing_zeros")),
            "first_leading_one" => finish(first(self, var(&bits), "leading_zeros")),
            "first_trailing_one" => finish(first(self, var(&bits), "trailing_zeros")),
            "first_leading_zero" => finish(first(self, inverse_expr.clone(), "leading_zeros")),
            "first_trailing_zero" => finish(first(self, inverse_expr.clone(), "trailing_zeros")),
            "count_ones" => finish(counted(var(&bits), "count_ones")),
            "count_zeros" => finish(binary(
                BinOp::Sub,
                u32_literal(width),
                counted(var(&bits), "count_ones"),
            )),
            "has_single_bit" => {
                binary(BinOp::Eq, counted(var(&bits), "count_ones"), u32_literal(1))
            }
            "bit_width" => finish(binary(BinOp::Sub, u32_literal(width), leading())),
            "bit_floor" => {
                let zero = self.lower_number(&Number::Integer(0u32.into()), &operand.ty)?;
                let amount = binary(BinOp::Sub, u32_literal(width - 1), leading());
                conditional(
                    binary(BinOp::Eq, leading(), u32_literal(width)),
                    zero,
                    shifted(self, amount)?,
                )
            }
            "bit_ceil" => {
                let one = self.lower_number(&Number::Integer(1u32.into()), &operand.ty)?;
                let amount = binary(BinOp::Sub, u32_literal(width), leading());
                let count = counted(var(&bits), "count_ones");
                conditional(
                    binary(BinOp::Eq, count.clone(), u32_literal(0)),
                    one,
                    conditional(
                        binary(BinOp::Eq, count, u32_literal(1)),
                        var(&bits),
                        shifted(self, amount)?,
                    ),
                )
            }
            _ => return Err(unsupported_value(value)),
        };
        Ok(Expr::Block(Box::new(rust::Block {
            stmts: vec![bind(&bits, operand_value)],
            tail: Some(Box::new(tail)),
        })))
    }

    pub(super) fn lower_stdc_rotate(
        &mut self,
        value: &ir::Value,
        method: &str,
        arguments: &[ir::Value],
    ) -> Result<Expr> {
        let [operand, count] = arguments else {
            return Err(unsupported_value(value));
        };
        let count_ty = count.ty.clone();
        let count = self.lower_value(count)?;
        let recv = self.lower_value(operand)?;
        let call = |recv: Expr, method: &str, args: Vec<Expr>| Expr::MethodCall {
            recv: Box::new(recv),
            method: method.into(),
            args,
        };
        let modulus = |width: u32| Expr::Cast {
            expr: Box::new(Expr::Value(rust::RustValue::U128(u128::from(width)))),
            ty: rust::Type::Prim(Prim::U32),
        };
        let remainder = |width: u32| match self.tables.wide_int(&count_ty) {
            Some(wide) => call(
                count.clone(),
                if wide.signed {
                    "rem_euclid_u32"
                } else {
                    "rem_u32"
                },
                vec![modulus(width)],
            ),
            None => Expr::Cast {
                expr: Box::new(call(
                    Expr::Cast {
                        expr: Box::new(count.clone()),
                        ty: rust::Type::Prim(Prim::I64),
                    },
                    "rem_euclid",
                    vec![Expr::Value(rust::RustValue::I64(i64::from(width)))],
                )),
                ty: rust::Type::Prim(Prim::U32),
            },
        };
        if let Some(wide) = self.tables.wide_int(&operand.ty) {
            return Ok(call(
                recv,
                &format!("{method}_bits"),
                vec![remainder(wide.width)],
            ));
        }
        match self.tables.bit_shape(&operand.ty) {
            Some(BitShape::Plain) => {
                let ty = self.lower_type(&operand.ty)?;
                let rust::Type::Prim(prim) = ty else {
                    return Err(unsupported_value(value));
                };
                Ok(call(recv, method, vec![remainder(prim_width(prim))]))
            }
            Some(BitShape::Small { width, .. }) => {
                let (towards, away) = if method == "rotate_left" {
                    (BinOp::Shl, BinOp::Shr)
                } else {
                    (BinOp::Shr, BinOp::Shl)
                };
                let (operand_name, shift_name) = (self.next_temp(), self.next_temp());
                let var = |name: &str| Expr::Var(name.into());
                let literal = |value: u32| Expr::Cast {
                    expr: Box::new(Expr::Value(rust::RustValue::U128(u128::from(value)))),
                    ty: rust::Type::Prim(Prim::U32),
                };
                let rotated = Expr::Binary {
                    op: BinOp::BitOr,
                    lhs: Box::new(Expr::Binary {
                        op: towards,
                        lhs: Box::new(var(&operand_name)),
                        rhs: Box::new(var(&shift_name)),
                    }),
                    rhs: Box::new(Expr::Binary {
                        op: away,
                        lhs: Box::new(var(&operand_name)),
                        rhs: Box::new(Expr::Binary {
                            op: BinOp::Rem,
                            lhs: Box::new(Expr::Binary {
                                op: BinOp::Sub,
                                lhs: Box::new(literal(width)),
                                rhs: Box::new(var(&shift_name)),
                            }),
                            rhs: Box::new(literal(width)),
                        }),
                    }),
                };
                let tail = self.narrow_integer(rotated, &operand.ty)?;
                Ok(Expr::Block(Box::new(rust::Block {
                    stmts: vec![
                        Stmt::Let {
                            name: operand_name,
                            mutable: false,
                            ty: None,
                            init: Some(recv),
                        },
                        Stmt::Let {
                            name: shift_name,
                            mutable: false,
                            ty: None,
                            init: Some(remainder(width)),
                        },
                    ],
                    tail: Some(Box::new(tail)),
                })))
            }
            None => Err(unsupported_value(value)),
        }
    }

    pub(super) fn wide_int_constant(&mut self, wide: WideInt, number: &Number) -> Result<Expr> {
        let (name, value) = match number {
            Number::Integer(value) => match u128::try_from(value) {
                Ok(value) => ("from_u128", rust::RustValue::U128(value)),
                Err(_) => {
                    let mut bytes = value.to_bytes_le();
                    bytes.push(0);
                    return Ok(wide.bytes_literal(&bytes));
                }
            },
            Number::SignedInteger(value) => match i128::try_from(value) {
                Ok(value) => ("from_i128", rust::RustValue::I128(value)),
                Err(_) => return Ok(wide.bytes_literal(&value.to_signed_bytes_le())),
            },
            _ => return Err(unsupported_constant(number)),
        };
        Ok(wide.call(name, vec![Expr::Value(value)]))
    }

    fn float_kind(&self, ty: &ir::Type) -> Option<ir::FloatType> {
        match self.tables.resolve_type(ty) {
            ir::Type::Numeric(ir::NumericType::Float(
                kind @ (ir::FloatType::F16
                | ir::FloatType::F32
                | ir::FloatType::F64
                | ir::FloatType::F80
                | ir::FloatType::F128),
            )) => Some(*kind),
            _ => None,
        }
    }

    fn wide_int_to_float(&self, kind: ir::FloatType, expr: Expr) -> Expr {
        let method = |recv: Expr, name: &str, args: Vec<Expr>| Expr::MethodCall {
            recv: Box::new(recv),
            method: name.into(),
            args,
        };
        let ieee = |expr: Expr, frac: i64, exp: i64| {
            method(
                expr,
                "to_ieee_bits",
                vec![
                    Expr::Value(rust::RustValue::I64(frac)),
                    Expr::Value(rust::RustValue::I64(exp)),
                    Expr::Value(rust::RustValue::Bool(false)),
                ],
            )
        };
        let from_bits = |prim: &str, bits: Expr| Expr::Call {
            func: Box::new(Expr::Var(format!("{prim}::from_bits").into())),
            args: vec![bits],
            binding: CallBinding::Generated,
        };
        match kind {
            ir::FloatType::F32 => method(expr, "to_f32", Vec::new()),
            ir::FloatType::F64 => method(expr, "to_f64", Vec::new()),
            ir::FloatType::F16 => from_bits(
                "f16",
                Expr::Cast {
                    expr: Box::new(ieee(expr, 10, 5)),
                    ty: rust::Type::Prim(Prim::U16),
                },
            ),
            ir::FloatType::F128 => from_bits("f128", ieee(expr, 112, 15)),
            _ => Expr::Call {
                func: Box::new(Expr::Var(long_double::LONG_DOUBLE_TY.into())),
                args: vec![method(expr, "to_f80_bytes", Vec::new())],
                binding: CallBinding::Generated,
            },
        }
    }

    fn float_to_wide_int(&self, wide: WideInt, kind: ir::FloatType, expr: Expr) -> Expr {
        let ieee = |frac: i64, exp: i64| {
            let bits = Expr::Cast {
                expr: Box::new(Expr::MethodCall {
                    recv: Box::new(expr.clone()),
                    method: "to_bits".into(),
                    args: Vec::new(),
                }),
                ty: rust::Type::Prim(Prim::U128),
            };
            wide.call(
                "from_ieee_bits",
                vec![
                    bits,
                    Expr::Value(rust::RustValue::I64(frac)),
                    Expr::Value(rust::RustValue::I64(exp)),
                    Expr::Value(rust::RustValue::Bool(false)),
                ],
            )
        };
        match kind {
            ir::FloatType::F32 => wide.call("from_f32", vec![expr]),
            ir::FloatType::F64 => wide.call("from_f64", vec![expr]),
            ir::FloatType::F16 => ieee(10, 5),
            ir::FloatType::F128 => ieee(112, 15),
            _ => wide.call(
                "from_f80_bytes",
                vec![Expr::TupleField {
                    base: Box::new(expr),
                    index: 0,
                }],
            ),
        }
    }

    pub(super) fn lower_wide_int_conversion(
        &mut self,
        value: &ir::Value,
        operand: &ir::Value,
    ) -> Result<Expr> {
        let source = self.tables.wide_int(&operand.ty);
        let target = self.tables.wide_int(&value.ty);
        let lowered = self.lower_value(operand)?;
        if source.is_some()
            && let Some(kind) = self.float_kind(&value.ty)
        {
            return Ok(self.wide_int_to_float(kind, lowered));
        }
        if let Some(target) = target
            && let Some(kind) = self.float_kind(&operand.ty)
        {
            return Ok(self.float_to_wide_int(target, kind, lowered));
        }
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
            ir::ArithOp::Shl | ir::ArithOp::Shr => {
                let amount = self.lower_shift_amount(right)?;
                return Ok(Some(Expr::MethodCall {
                    recv: Box::new(self.lower_value(left)?),
                    method: if op == ir::ArithOp::Shl {
                        "shl_bits"
                    } else {
                        "shr_bits"
                    }
                    .into(),
                    args: vec![amount],
                }));
            }
            _ => return Ok(None),
        };
        Ok(Some(Expr::MethodCall {
            recv: Box::new(self.lower_value(left)?),
            method: method.into(),
            args: vec![self.lower_value(right)?],
        }))
    }

    fn lower_shift_amount(&mut self, amount: &ir::Value) -> Result<Expr> {
        let lowered = self.lower_value(amount)?;
        let narrow = match self.tables.wide_int(&amount.ty) {
            Some(wide) => Expr::MethodCall {
                recv: Box::new(lowered),
                method: if wide.signed { "to_i128" } else { "to_u128" }.into(),
                args: Vec::new(),
            },
            None => lowered,
        };
        Ok(Expr::Cast {
            expr: Box::new(narrow),
            ty: rust::Type::Prim(Prim::Usize),
        })
    }

    fn integer_to_accumulator(&self, wide: WideInt, ty: &ir::Type, expr: Expr) -> Expr {
        if let Some(source) = self.tables.wide_int(ty) {
            return wide.call(
                if source.signed {
                    "from_bint"
                } else {
                    "from_buint"
                },
                vec![expr],
            );
        }
        let signed = matches!(
            self.tables.resolve_type(ty),
            ir::Type::Numeric(ir::NumericType::Integer { signed: true, .. })
        );
        let (name, prim) = if signed {
            ("from_i128", Prim::I128)
        } else {
            ("from_u128", Prim::U128)
        };
        wide.call(
            name,
            vec![Expr::Cast {
                expr: Box::new(expr),
                ty: rust::Type::Prim(prim),
            }],
        )
    }

    fn accumulator_to_integer(&mut self, expr: Expr, ty: &ir::Type) -> Result<Expr> {
        if let Some(target) = self.tables.wide_int(ty) {
            return Ok(target.call("from_bint", vec![expr]));
        }
        let low = Expr::MethodCall {
            recv: Box::new(expr),
            method: "to_i128".into(),
            args: Vec::new(),
        };
        self.narrow_integer(low, ty)
    }

    pub(super) fn lower_wide_overflow(
        &mut self,
        value: &ir::Value,
        method: &str,
        left: &ir::Value,
        right: &ir::Value,
        result: &ir::Place,
    ) -> Result<Expr> {
        let width_of = |this: &Self, ty: &ir::Type| match this.tables.resolve_type(ty) {
            ir::Type::Numeric(ir::NumericType::Integer { width, .. }) => Some(*width),
            _ => None,
        };
        let (Some(left_width), Some(right_width), Some(result_width)) = (
            width_of(self, &left.ty),
            width_of(self, &right.ty),
            width_of(self, &result.ty),
        ) else {
            return Err(unsupported_value(value));
        };
        let bits = (2 * left_width.max(right_width) + 2).max(result_width + 1);
        let accumulator = WideInt {
            width: bits,
            signed: true,
            bytes: u64::from(bits.div_ceil(64)) * 8,
            align: 8,
        };
        let [lhs, rhs, exact, narrowed] = [
            self.next_temp(),
            self.next_temp(),
            self.next_temp(),
            self.next_temp(),
        ];
        let var = |name: &String| Expr::Var(name.as_str().into());
        let bind = |name: &String, ty: Option<rust::Type>, init: Expr| Stmt::Let {
            name: name.clone(),
            mutable: false,
            ty,
            init: Some(init),
        };
        let accumulator_type = Some(accumulator.rust_type());
        let left_value = self.lower_value(left)?;
        let right_value = self.lower_value(right)?;
        let lhs_init = self.integer_to_accumulator(accumulator, &left.ty, left_value);
        let rhs_init = self.integer_to_accumulator(accumulator, &right.ty, right_value);
        let narrowed_init = self.accumulator_to_integer(var(&exact), &result.ty)?;
        let result_type = self.lower_type(&result.ty)?;
        let back = self.integer_to_accumulator(accumulator, &result.ty, var(&narrowed));
        Ok(Expr::Block(Box::new(rust::Block {
            stmts: vec![
                bind(&lhs, accumulator_type.clone(), lhs_init),
                bind(&rhs, accumulator_type.clone(), rhs_init),
                bind(
                    &exact,
                    accumulator_type,
                    Expr::MethodCall {
                        recv: Box::new(var(&lhs)),
                        method: format!("wrapping_{method}"),
                        args: vec![var(&rhs)],
                    },
                ),
                bind(&narrowed, Some(result_type), narrowed_init),
                self.lower_assignment(result, var(&narrowed))?,
            ],
            tail: Some(Box::new(Expr::Binary {
                op: BinOp::Ne,
                lhs: Box::new(Expr::MethodCall {
                    recv: Box::new(var(&exact)),
                    method: "compare".into(),
                    args: vec![back],
                }),
                rhs: Box::new(Expr::Value(rust::RustValue::I64(0))),
            })),
        })))
    }

    pub(super) fn lower_elementwise_saturating(
        &mut self,
        value: &ir::Value,
        method: &str,
        arguments: &[ir::Value],
    ) -> Result<Expr> {
        let [left, right] = arguments else {
            return Err(unsupported_value(value));
        };
        if let Some(ir::NumericType::Integer { signed, .. }) = self.tables.vector_element(&value.ty)
        {
            self.lower_type(&value.ty)?;
            return Ok(Expr::Call {
                func: Box::new(Expr::Var(
                    format!(
                        "std::simd::num::{}::{method}",
                        if signed { "SimdInt" } else { "SimdUint" }
                    )
                    .into(),
                )),
                args: vec![self.lower_value(left)?, self.lower_value(right)?],
                binding: CallBinding::Generated,
            });
        }
        let ir::Type::Numeric(ir::NumericType::Integer { signed, .. }) =
            *self.tables.resolve_type(&value.ty)
        else {
            return Err(unsupported_value(value));
        };
        let saturated = Expr::MethodCall {
            recv: Box::new(self.lower_value(left)?),
            method: method.into(),
            args: vec![self.lower_value(right)?],
        };
        let Some((width, _, container)) = odd_width(self.tables.resolve_type(&value.ty)) else {
            return Ok(saturated);
        };
        let (low, high) = if signed {
            (-(1i128 << (width - 1)), (1i128 << (width - 1)) - 1)
        } else {
            (0, (1i128 << width) - 1)
        };
        let bound = |bound: i128| Expr::Cast {
            expr: Box::new(Expr::Value(rust::RustValue::I128(bound))),
            ty: rust::Type::Prim(container),
        };
        Ok(Expr::MethodCall {
            recv: Box::new(saturated),
            method: "clamp".into(),
            args: vec![bound(low), bound(high)],
        })
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
            ir::ArithOp::Shl | ir::ArithOp::Shr => {
                let amount = self.lower_shift_amount(right)?;
                let shifted = Expr::MethodCall {
                    recv: Box::new(self.lower_value(left)?),
                    method: if op == ir::ArithOp::Shl {
                        "wrapping_shl"
                    } else {
                        "wrapping_shr"
                    }
                    .into(),
                    args: vec![Expr::Cast {
                        expr: Box::new(amount),
                        ty: rust::Type::Prim(Prim::U32),
                    }],
                };
                return if op == ir::ArithOp::Shl {
                    self.narrow_integer(shifted, &value.ty).map(Some)
                } else {
                    Ok(Some(shifted))
                };
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

#[derive(Clone, Copy)]
pub(super) enum BitShape {
    Plain,
    Small { width: u32, container: Prim },
}

impl Tables<'_> {
    pub(super) fn bit_shape(&self, ty: &ir::Type) -> Option<BitShape> {
        let resolved = self.resolve_type(ty);
        match odd_width(resolved) {
            None => Some(BitShape::Plain),
            Some((width, _, container)) if is_small_bit_precise(resolved) => {
                Some(BitShape::Small { width, container })
            }
            Some(_) => None,
        }
    }
}

fn unsigned_prim(prim: Prim) -> Prim {
    match prim {
        Prim::I8 => Prim::U8,
        Prim::I16 => Prim::U16,
        Prim::I32 => Prim::U32,
        Prim::I64 => Prim::U64,
        Prim::I128 => Prim::U128,
        other => other,
    }
}

pub(super) fn bit_count_expr(shape: BitShape, recv: Expr, method: &str) -> Expr {
    let call = |recv: Expr, method: &str, args: Vec<Expr>| Expr::MethodCall {
        recv: Box::new(recv),
        method: method.into(),
        args,
    };
    let BitShape::Small { width, container } = shape else {
        return call(recv, method, Vec::new());
    };
    let unsigned = unsigned_prim(container);
    let literal = |value: u128, prim: Prim| Expr::Cast {
        expr: Box::new(Expr::Value(rust::RustValue::U128(value))),
        ty: rust::Type::Prim(prim),
    };
    let view = Expr::Binary {
        op: BinOp::BitAnd,
        lhs: Box::new(Expr::Cast {
            expr: Box::new(recv),
            ty: rust::Type::Prim(unsigned),
        }),
        rhs: Box::new(literal((1u128 << width) - 1, unsigned)),
    };
    let count = call(view, method, Vec::new());
    match method {
        "leading_zeros" => Expr::Binary {
            op: BinOp::Sub,
            lhs: Box::new(count),
            rhs: Box::new(literal(
                u128::from(prim_width(container) - width),
                Prim::U32,
            )),
        },
        "trailing_zeros" => call(count, "min", vec![literal(u128::from(width), Prim::U32)]),
        _ => count,
    }
}
