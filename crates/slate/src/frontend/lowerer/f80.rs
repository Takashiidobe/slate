use super::*;

pub(super) fn long_double_literal(bits: u128) -> Expr {
    Expr::TupleStructLit {
        name: long_double::LONG_DOUBLE_TY.into(),
        fields: vec![Expr::ArrayLit(
            bits.to_le_bytes()[..10]
                .iter()
                .map(|byte| Expr::Value(rust::RustValue::I64(i64::from(*byte))))
                .collect(),
        )],
    }
}

pub(super) fn long_double_shim(name: &str, operand: Expr) -> Expr {
    Expr::Call {
        func: Box::new(Expr::Var(name.into())),
        args: vec![operand],
        binding: CallBinding::Generated,
    }
}

pub(super) fn long_double_bridge_tags<'a>(
    callee: &str,
    types: impl IntoIterator<Item = &'a rust::Type>,
) -> Result<Vec<String>> {
    types
        .into_iter()
        .map(|ty| match long_double::long_double_shim_type_tag(ty) {
            tag if tag == "x" => Err(Construct::LongDouble {
                detail: format!(
                    "call to {callee} passing {}",
                    crate::backend::codegen::type_to_string(ty)
                ),
            }
            .into()),
            tag => Ok(tag),
        })
        .collect()
}

pub(super) fn function_is_variadic(function: &ir::Function) -> bool {
    matches!(
        function.parameters,
        ir::Parameters::Prototype { variadic: true, .. }
    )
}

pub(super) fn long_double_export_name(symbol: &str, definition: &FnDef) -> Result<String> {
    let unsupported = || {
        Failure::from(Construct::LongDouble {
            detail: format!("export of {symbol}"),
        })
    };
    if symbol.contains("__") {
        return Err(unsupported());
    }
    let ret = definition.ret.clone().unwrap_or(rust::Type::Unit);
    let tags = long_double_bridge_tags(
        symbol,
        std::iter::once(&ret).chain(definition.params.iter().map(|param| &param.ty)),
    )?;
    if tags.iter().any(|tag| tag == "cf80") {
        return Err(unsupported());
    }
    Ok(format!("__slate_ld__{symbol}__r{}", tags.join("_")))
}

impl Tables<'_> {
    pub(super) fn is_long_double(&self, ty: &ir::Type) -> bool {
        matches!(
            self.resolve_type(ty),
            ir::Type::Numeric(ir::NumericType::Float(ir::FloatType::F80))
        )
    }

    pub(super) fn holds_long_double(&self, ty: &ir::Type) -> bool {
        self.is_long_double(ty)
            || match self.resolve_type(ty) {
                ir::Type::Array { element, .. } => self.holds_long_double(element),
                ty => self.record_fields(ty).is_some_and(|fields| {
                    fields.iter().any(|field| self.holds_long_double(&field.ty))
                }),
            }
    }

    pub(super) fn passes_long_double(&self, function: &ir::Function) -> bool {
        types::fixed_parameters(&function.parameters)
            .iter()
            .map(|parameter| &parameter.ty)
            .chain(&function.return_type)
            .any(|ty| self.holds_long_double(ty))
    }

    pub(super) fn function_type_passes_long_double(&self, ty: &ir::Type) -> bool {
        match self.resolve_type(ty) {
            ir::Type::Pointer { pointee, .. } => self.function_type_passes_long_double(pointee),
            ir::Type::Function {
                parameters,
                return_type,
                ..
            } => parameters
                .iter()
                .chain(return_type.as_deref())
                .any(|ty| self.holds_long_double(ty)),
            _ => false,
        }
    }
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn lower_long_double_conversion(
        &mut self,
        operand: &ir::Value,
        ty: &ir::Type,
    ) -> Result<Expr> {
        let lowered = self.lower_value(operand)?;
        let (from, to) = (
            self.tables.is_long_double(&operand.ty),
            self.tables.is_long_double(ty),
        );
        let shim = match (from, to) {
            (true, true) => return Ok(lowered),
            (false, true) => long_double::f80_cast_from_name(&self.lower_type(&operand.ty)?),
            _ => long_double::f80_cast_to_name(&self.lower_type(ty)?),
        };
        let shim = shim.ok_or_else(|| {
            Failure::from(Construct::LongDouble {
                detail: format!("conversion {} -> {ty}", operand.ty),
            })
        })?;
        Ok(long_double_shim(shim, lowered))
    }

    pub(super) fn lower_long_double_bridge(
        &mut self,
        function: &FunctionName,
        arguments: &[ir::Value],
        ret: &ir::Type,
    ) -> Result<Expr> {
        let callee = function.rust.as_str();
        if (function.is_variadic && crate::function_identity::Known::from_symbol(callee).is_none())
            || callee.contains("__")
        {
            return Err(Construct::LongDouble {
                detail: format!("call to {callee}"),
            }
            .into());
        }
        let params = arguments
            .iter()
            .map(|argument| self.lower_type(&argument.ty))
            .collect::<Result<Vec<_>>>()?;
        let ret = self.lower_type(ret)?;
        let tags = long_double_bridge_tags(callee, std::iter::once(&ret).chain(&params))?;
        let name = format!("__slate_{callee}__r{}", tags.join("_"));
        let args = arguments
            .iter()
            .map(|argument| self.lower_value(argument))
            .collect::<Result<Vec<_>>>()?;
        Ok(self.call_c_bridge(name, params, ret, args, function.identity))
    }

    pub(super) fn lower_long_double_variadic_trampoline(
        &mut self,
        function: &FunctionName,
        arguments: &[ir::Value],
        fixed: usize,
        ret: &ir::Type,
    ) -> Result<Expr> {
        let callee = function.rust.as_str();
        let params = arguments
            .iter()
            .map(|argument| self.lower_type(&argument.ty))
            .collect::<Result<Vec<_>>>()?;
        let ret = self.lower_type(ret)?;
        let fixed_tags =
            long_double_bridge_tags(callee, std::iter::once(&ret).chain(&params[..fixed]))?;
        let variadic_tags = long_double_bridge_tags(callee, &params[fixed..])?;
        let name = format!(
            "__slate_vcall__r{}__{}",
            fixed_tags.join("_"),
            variadic_tags.join("_")
        );
        let code_pointer = rust::Type::Ptr {
            mutable: false,
            inner: Box::new(rust::Type::Unit),
        };
        let args = std::iter::once(Ok(Expr::Cast {
            expr: Box::new(Expr::Var(callee.into())),
            ty: code_pointer.clone(),
        }))
        .chain(arguments.iter().map(|argument| self.lower_value(argument)))
        .collect::<Result<Vec<_>>>()?;
        Ok(self.call_c_bridge(
            name,
            std::iter::once(code_pointer).chain(params).collect(),
            ret,
            args,
            FunctionIdentity::Unknown,
        ))
    }

    fn long_double_pointer_tags(&mut self, ty: &ir::Type, subject: &str) -> Result<Vec<String>> {
        let unsupported = || {
            Failure::from(Construct::LongDouble {
                detail: format!("function pointer {subject}"),
            })
        };
        let (parameters, return_type) = match self.tables.resolve_type(ty) {
            ir::Type::Pointer { pointee, .. } => match self.tables.resolve_type(pointee) {
                ir::Type::Function {
                    parameters,
                    return_type,
                    variadic: false,
                    prototyped: true,
                    ..
                } => (parameters, return_type),
                _ => return Err(unsupported()),
            },
            _ => return Err(unsupported()),
        };
        let ret = match return_type {
            Some(ty) => self.lower_type(ty)?,
            None => rust::Type::Unit,
        };
        let params = parameters
            .iter()
            .map(|ty| self.lower_type(ty))
            .collect::<Result<Vec<_>>>()?;
        let tags = long_double_bridge_tags(subject, std::iter::once(&ret).chain(&params))?;
        if tags.iter().any(|tag| tag == "cf80") {
            return Err(unsupported());
        }
        Ok(tags)
    }

    pub(super) fn lower_long_double_function_address(
        &mut self,
        value: &ir::Value,
        id: BindingId,
    ) -> Result<Expr> {
        let name = &self.tables.names[&id];
        let tags = self.long_double_pointer_tags(&value.ty, &name.rust)?;
        let entry = format!("__slate_ldfp_{}_{}", self.tables.unit, name.rust);
        let link_name = if name.is_extern || name.exported {
            Some(
                self.tables
                    .builtin_library(id)
                    .map_or_else(|| name.rust.clone(), str::to_string),
            )
        } else {
            self.dependencies.long_double_exports.insert(
                name.rust.clone(),
                format!("__slate_ld__{entry}__r{}", tags.join("_")),
            );
            None
        };
        self.dependencies
            .native_entries
            .entry(entry.clone())
            .or_insert_with(|| rust::ExternFnDecl {
                attrs: link_name.map(Attr::LinkName).into_iter().collect(),
                name: entry.clone(),
                identity: FunctionIdentity::Unknown,
                declared_type: None,
                trusted_headers: Default::default(),
                params: Vec::new(),
                variadic: false,
                ret: None,
                safe: false,
            });
        let address = rust::Type::Ptr {
            mutable: false,
            inner: Box::new(rust::Type::Unit),
        };
        Ok(Expr::Transmute {
            from: address.clone(),
            to: self.lower_type(&value.ty)?,
            expr: Box::new(Expr::Cast {
                expr: Box::new(Expr::Var(entry.into())),
                ty: address,
            }),
        })
    }

    pub(super) fn lower_long_double_indirect_call(
        &mut self,
        pointer: &ir::Value,
        arguments: &[ir::Value],
        ret: &ir::Type,
    ) -> Result<Expr> {
        let tags = self.long_double_pointer_tags(&pointer.ty, "call")?;
        let name = format!("__slate_icall__r{}", tags.join("_"));
        let code_pointer = rust::Type::Ptr {
            mutable: false,
            inner: Box::new(rust::Type::Unit),
        };
        let params = arguments
            .iter()
            .map(|argument| self.lower_type(&argument.ty))
            .collect::<Result<Vec<_>>>()?;
        let ret = self.lower_type(ret)?;
        let callee = Expr::Transmute {
            from: self.lower_type(&pointer.ty)?,
            to: code_pointer.clone(),
            expr: Box::new(self.lower_value(pointer)?),
        };
        let args = std::iter::once(Ok(callee))
            .chain(arguments.iter().map(|argument| self.lower_value(argument)))
            .collect::<Result<Vec<_>>>()?;
        Ok(self.call_c_bridge(
            name,
            std::iter::once(code_pointer).chain(params).collect(),
            ret,
            args,
            FunctionIdentity::Unknown,
        ))
    }
}

impl Tables<'_> {
    pub(super) fn fold_long_double(&self, value: &ir::Value) -> Option<u128> {
        use rustc_apfloat::{Float, ieee::X87DoubleExtended};
        if !self.is_long_double(&value.ty) {
            return None;
        }
        let operand = |value: &ir::Value| {
            self.fold_long_double(value)
                .map(X87DoubleExtended::from_bits)
        };
        match &value.node.value {
            ValueKind::Constant(ir::Number::FloatBits(bits)) => Some(*bits),
            ValueKind::Unary {
                op: ir::UnaryArithOp::Neg,
                operand: inner,
                ..
            } => Some((-operand(inner)?).to_bits()),
            ValueKind::Convert {
                kind: ir::ConversionKind::IntToFloat,
                operand: inner,
                semantics:
                    ir::ConversionSema::IntToFloat {
                        floating:
                            ir::FloatingSemantics {
                                rounding: ir::Rounding::NearestEven,
                                ..
                            },
                        ..
                    },
                ..
            } => Some(
                X87DoubleExtended::from_i128(self.fold_integer(inner)?)
                    .value
                    .to_bits(),
            ),
            ValueKind::Convert {
                kind: ir::ConversionKind::FloatWiden,
                operand: inner,
                ..
            } => self.widen_to_f80(inner),
            ValueKind::Arith {
                op,
                left,
                right,
                semantics:
                    ir::ArithSema::Floating {
                        floating:
                            ir::FloatingSemantics {
                                rounding: ir::Rounding::NearestEven,
                                ..
                            },
                        ..
                    },
            } => {
                let (left, right) = (operand(left)?, operand(right)?);
                let result = match op {
                    ir::ArithOp::Add => left + right,
                    ir::ArithOp::Sub => left - right,
                    ir::ArithOp::Mul => left * right,
                    ir::ArithOp::Div => left / right,
                    _ => return None,
                };
                Some(result.value.to_bits())
            }
            _ => None,
        }
    }

    pub(super) fn is_complex_long_double(&self, ty: &ir::Type) -> bool {
        matches!(
            self.resolve_type(ty),
            ir::Type::Complex(ir::NumericType::Float(ir::FloatType::F80))
        )
    }

    pub(super) fn fold_complex_long_double(&self, value: &ir::Value) -> Option<[u128; 2]> {
        let ValueKind::Convert {
            kind: ir::ConversionKind::ComplexConvert,
            operand,
            ..
        } = &value.node.value
        else {
            return None;
        };
        let ValueKind::Aggregate { members, .. } = &operand.node.value else {
            return None;
        };
        if !self.is_complex_long_double(&value.ty) {
            return None;
        }
        let mut parts = [0u128; 2];
        for member in members {
            let ir::AggregateTarget::Index(index @ (0 | 1)) = member.target else {
                return None;
            };
            parts[index as usize] = self.widen_to_f80(&member.value)?;
        }
        Some(parts)
    }

    fn widen_to_f80(&self, value: &ir::Value) -> Option<u128> {
        use rustc_apfloat::{
            Float, FloatConvert,
            ieee::{Double, Single, X87DoubleExtended},
        };
        let bits = self.fold_real_constant(value)?;
        let widened: X87DoubleExtended = match self.resolve_type(&value.ty) {
            ir::Type::Numeric(ir::NumericType::Float(ir::FloatType::F64)) => {
                Double::from_bits(bits).convert(&mut false).value
            }
            ir::Type::Numeric(ir::NumericType::Float(ir::FloatType::F32)) => {
                Single::from_bits(bits).convert(&mut false).value
            }
            _ => return None,
        };
        Some(widened.to_bits())
    }

    fn fold_real_constant(&self, value: &ir::Value) -> Option<u128> {
        let sign = match self.resolve_type(&value.ty) {
            ir::Type::Numeric(ir::NumericType::Float(ir::FloatType::F64)) => 1u128 << 63,
            ir::Type::Numeric(ir::NumericType::Float(ir::FloatType::F32)) => 1u128 << 31,
            _ => return None,
        };
        match &value.node.value {
            ValueKind::Constant(ir::Number::FloatBits(bits)) => Some(*bits),
            ValueKind::Unary {
                op: ir::UnaryArithOp::Neg,
                operand,
                ..
            } => Some(self.fold_real_constant(operand)? ^ sign),
            ValueKind::Convert {
                kind: ir::ConversionKind::ComplexToReal,
                operand,
                ..
            } => match &operand.node.value {
                ValueKind::Aggregate { members, .. } => members
                    .iter()
                    .find(|member| matches!(member.target, ir::AggregateTarget::Index(0)))
                    .and_then(|member| self.fold_real_constant(&member.value)),
                _ => None,
            },
            _ => None,
        }
    }

    fn fold_integer(&self, value: &ir::Value) -> Option<i128> {
        let ir::Type::Numeric(ir::NumericType::Integer { width, signed, .. }) =
            *self.resolve_type(&value.ty)
        else {
            return None;
        };
        let raw = match &value.node.value {
            ValueKind::Constant(ir::Number::Integer(number)) => i128::try_from(number).ok()?,
            ValueKind::Constant(ir::Number::SignedInteger(number)) => {
                i128::try_from(number).ok()?
            }
            ValueKind::Unary {
                op: ir::UnaryArithOp::Neg,
                operand,
                ..
            } => self.fold_integer(operand)?.wrapping_neg(),
            ValueKind::Arith {
                op, left, right, ..
            } => {
                let (left, right) = (self.fold_integer(left)?, self.fold_integer(right)?);
                match op {
                    ir::ArithOp::Add => left.wrapping_add(right),
                    ir::ArithOp::Sub => left.wrapping_sub(right),
                    ir::ArithOp::Mul => left.wrapping_mul(right),
                    ir::ArithOp::Shl if (0..i128::from(width)).contains(&right) => left << right,
                    _ => return None,
                }
            }
            ValueKind::Convert {
                kind: ir::ConversionKind::Widen | ir::ConversionKind::Truncate,
                operand,
                ..
            } => self.fold_integer(operand)?,
            _ => return None,
        };
        if width >= 128 {
            return (signed || raw >= 0).then_some(raw);
        }
        let shift = 128 - width;
        Some(if signed {
            (raw << shift) >> shift
        } else {
            ((raw as u128) << shift >> shift) as i128
        })
    }
}
