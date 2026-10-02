use super::*;

fn odd_width(ty: &ir::Type) -> Option<(u32, bool, Prim)> {
    let ir::Type::Numeric(ir::NumericType::Integer { width, signed, .. }) = *ty else {
        return None;
    };
    let container = match (width, signed) {
        (8 | 16 | 32 | 64 | 128, _) | (129.., _) | (0, _) => return None,
        (..8, true) => Prim::I8,
        (..8, false) => Prim::U8,
        (..16, true) => Prim::I16,
        (..16, false) => Prim::U16,
        (..32, true) => Prim::I32,
        (..32, false) => Prim::U32,
        (..64, true) => Prim::I64,
        (..64, false) => Prim::U64,
        (_, true) => Prim::I128,
        (_, false) => Prim::U128,
    };
    Some((width, signed, container))
}

fn prim_width(prim: Prim) -> u32 {
    match prim {
        Prim::I8 | Prim::U8 => 8,
        Prim::I16 | Prim::U16 => 16,
        Prim::I32 | Prim::U32 => 32,
        Prim::I64 | Prim::U64 => 64,
        _ => 128,
    }
}

pub(super) fn zeroed() -> Expr {
    Expr::Unsafe(Box::new(rust::Block {
        stmts: Vec::new(),
        tail: Some(Box::new(Expr::Call {
            func: Box::new(Expr::Var("std::mem::zeroed".into())),
            args: Vec::new(),
            binding: CallBinding::Generated,
        })),
    }))
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn lower_evaluation_statements(
        &mut self,
        evaluation: &ir::Evaluation,
    ) -> Result<Vec<Stmt>> {
        let mut statements = self.lower_statement_list(&evaluation.statements)?;
        if !matches!(evaluation.value.node.value, ValueKind::Void) {
            statements.push(Stmt::Expr(self.lower_value(&evaluation.value)?));
        }
        Ok(statements)
    }

    pub(super) fn lower_evaluation(
        &mut self,
        evaluation: &ir::Evaluation,
    ) -> Result<(Vec<Stmt>, Expr)> {
        let statements = self.lower_statement_list(&evaluation.statements)?;
        let condition = self.lower_condition(&evaluation.value)?;
        Ok((statements, condition))
    }

    pub(super) fn lower_condition(&mut self, value: &ir::Value) -> Result<Expr> {
        let condition = self.lower_value(value)?;
        if matches!(value.ty, ir::Type::Bool) {
            Ok(condition)
        } else if self.tables.is_long_double(&value.ty) {
            Ok(Expr::Binary {
                op: BinOp::Ne,
                lhs: Box::new(condition),
                rhs: Box::new(long_double_literal(0)),
            })
        } else {
            Ok(Expr::Binary {
                op: BinOp::Ne,
                lhs: Box::new(condition),
                rhs: Box::new(self.lower_number(&Number::Integer(0u32.into()), &value.ty)?),
            })
        }
    }

    pub(super) fn lower_value(&mut self, value: &ir::Value) -> Result<Expr> {
        self.lower_value_node(value)
            .map_err(|error| error.at(Site::of(&value.node)))
    }

    fn lower_value_node(&mut self, value: &ir::Value) -> Result<Expr> {
        Ok(match &value.node.value {
            ValueKind::Constant(number) => self.lower_number(number, &value.ty)?,
            ValueKind::CodeUnits(units) => {
                let ir::Type::Array { element, .. } = &value.ty else {
                    return Err(unsupported_value(value));
                };
                let signed_width = match **element {
                    ir::Type::Numeric(ir::NumericType::Integer {
                        width,
                        signed: true,
                        ..
                    }) => Some(128 - width),
                    _ => None,
                };
                let element = self.lower_type(element)?;
                Expr::ArrayLit(
                    units
                        .iter()
                        .map(|unit| Expr::Cast {
                            expr: Box::new(Expr::Value(match signed_width {
                                Some(shift) => {
                                    rust::RustValue::I128((i128::from(*unit) << shift) >> shift)
                                }
                                None => rust::RustValue::U128(u128::from(*unit)),
                            })),
                            ty: element.clone(),
                        })
                        .collect(),
                )
            }
            ValueKind::VaArg { list } if self.tables.is_long_double(&value.ty) => {
                Expr::Unsafe(Box::new(rust::Block {
                    stmts: Vec::new(),
                    tail: Some(Box::new(Expr::Call {
                        func: Box::new(Expr::Var("__slate_f80_va_arg".into())),
                        args: vec![Expr::AddrOf {
                            mutable: true,
                            expr: Box::new(self.lower_place(list)?),
                        }],
                        binding: CallBinding::Generated,
                    })),
                }))
            }
            ValueKind::VaArg { list } => Expr::Unsafe(Box::new(rust::Block {
                stmts: Vec::new(),
                tail: Some(Box::new(Expr::MethodCallGeneric {
                    recv: Box::new(self.lower_place(list)?),
                    method: "next_arg".into(),
                    type_args: vec![self.lower_type(&value.ty)?],
                    args: Vec::new(),
                })),
            })),
            ValueKind::Read {
                place,
                ordering: None,
            } if place.access.volatile => Expr::Unsafe(Box::new(rust::Block {
                stmts: Vec::new(),
                tail: Some(Box::new(Expr::Call {
                    func: Box::new(Expr::Var("std::ptr::read_volatile".into())),
                    args: vec![self.lower_address(place, false)?],
                    binding: CallBinding::Generated,
                })),
            })),
            ValueKind::Read {
                place,
                ordering: None,
            } => {
                let lowered = match self.bit_field_accessor(place, "get")? {
                    Some((storage, getter)) => Expr::MethodCall {
                        recv: Box::new(storage),
                        method: getter,
                        args: Vec::new(),
                    },
                    None => self.lower_place(place)?,
                };
                let lowered = if matches!(value.ty, ir::Type::VaList) {
                    clone_va_list(lowered)
                } else {
                    lowered
                };
                if self.tables.place_is_unsafe(place) {
                    Expr::Unsafe(Box::new(rust::Block {
                        stmts: Vec::new(),
                        tail: Some(Box::new(lowered)),
                    }))
                } else {
                    lowered
                }
            }
            ValueKind::Copy { operand, .. } => self.lower_value(operand)?,
            ValueKind::AddressOf(place) => match place.kind {
                PlaceKind::Deref(ref pointer) => self.lower_value(pointer)?,
                _ => {
                    let address = Expr::AddrOf {
                        mutable: !matches!(
                            self.tables.resolve_type(&value.ty),
                            ir::Type::Pointer { is_const: true, .. }
                        ),
                        expr: Box::new(self.lower_place(place)?),
                    };
                    if self.tables.place_is_unsafe(place) {
                        Expr::Unsafe(Box::new(rust::Block {
                            stmts: Vec::new(),
                            tail: Some(Box::new(address)),
                        }))
                    } else {
                        address
                    }
                }
            },
            ValueKind::Convert {
                kind: ir::ConversionKind::VectorSplat,
                operand,
                ..
            } => {
                self.lower_type(&value.ty)?;
                Expr::Call {
                    func: Box::new(Expr::Var("std::simd::Simd::splat".into())),
                    args: vec![self.lower_value(operand)?],
                    binding: CallBinding::Generated,
                }
            }
            ValueKind::Convert {
                kind: ir::ConversionKind::VectorBitCast,
                operand,
                ..
            } => Expr::Transmute {
                from: self.lower_type(&operand.ty)?,
                to: self.lower_type(&value.ty)?,
                expr: Box::new(self.lower_value(operand)?),
            },
            ValueKind::Lane { vector, index } => Expr::Index {
                base: Box::new(self.lower_value(vector)?),
                index: Box::new(Expr::Cast {
                    expr: Box::new(self.lower_value(index)?),
                    ty: rust::Type::Prim(Prim::Usize),
                }),
            },
            ValueKind::Convert { operand, .. }
                if self.tables.is_long_double(&operand.ty)
                    || self.tables.is_long_double(&value.ty) =>
            {
                self.lower_long_double_conversion(operand, &value.ty)?
            }
            ValueKind::Convert { operand, .. }
                if matches!(
                    self.tables.resolve_type(&operand.ty),
                    ir::Type::Vector { .. }
                ) || matches!(self.tables.resolve_type(&value.ty), ir::Type::Vector { .. }) =>
            {
                return Err(unsupported_value(value));
            }
            ValueKind::Convert { operand, .. } => {
                let from = match odd_width(self.tables.resolve_type(&operand.ty)) {
                    Some((_, _, container)) => rust::Type::Prim(container),
                    None => self.lower_type(&operand.ty)?,
                };
                let expr = Box::new(self.lower_value(operand)?);
                let Some((width, signed, container)) =
                    odd_width(self.tables.resolve_type(&value.ty))
                else {
                    return Ok(convert_function_pointer(
                        from,
                        self.lower_type(&value.ty)?,
                        expr,
                    ));
                };
                let contained = Expr::Cast {
                    expr,
                    ty: rust::Type::Prim(container),
                };
                if signed {
                    let shift = Box::new(Expr::Value(rust::RustValue::I64(i64::from(
                        prim_width(container) - width,
                    ))));
                    Expr::Binary {
                        op: BinOp::Shr,
                        lhs: Box::new(Expr::Binary {
                            op: BinOp::Shl,
                            lhs: Box::new(contained),
                            rhs: shift.clone(),
                        }),
                        rhs: shift,
                    }
                } else {
                    Expr::Binary {
                        op: BinOp::BitAnd,
                        lhs: Box::new(contained),
                        rhs: Box::new(Expr::Cast {
                            expr: Box::new(Expr::Value(rust::RustValue::U128(
                                (1u128 << width) - 1,
                            ))),
                            ty: rust::Type::Prim(container),
                        }),
                    }
                }
            }
            ValueKind::ArrayDecay { place, .. } => {
                let bytes = match place.kind {
                    PlaceKind::Binding(id) => self.tables.strings.get(&id),
                    _ => None,
                };
                let Some(bytes) = bytes else {
                    let array = self.lower_place(place)?;
                    let decayed = Expr::Cast {
                        expr: Box::new(
                            if self.tables.place_is_static(place)
                                || matches!(
                                    self.tables.resolve_type(&place.ty),
                                    ir::Type::Array { length: None, .. }
                                )
                            {
                                Expr::AddrOf {
                                    mutable: !matches!(
                                        self.tables.resolve_type(&value.ty),
                                        ir::Type::Pointer { is_const: true, .. }
                                    ),
                                    expr: Box::new(array),
                                }
                            } else {
                                Expr::MethodCall {
                                    recv: Box::new(array),
                                    method: if matches!(
                                        value.ty,
                                        ir::Type::Pointer { is_const: true, .. }
                                    ) {
                                        "as_ptr"
                                    } else {
                                        "as_mut_ptr"
                                    }
                                    .into(),
                                    args: Vec::new(),
                                }
                            },
                        ),
                        ty: self.lower_type(&value.ty)?,
                    };
                    return Ok(if self.tables.place_is_unsafe(place) {
                        Expr::Unsafe(Box::new(rust::Block {
                            stmts: Vec::new(),
                            tail: Some(Box::new(decayed)),
                        }))
                    } else {
                        decayed
                    });
                };
                Expr::Cast {
                    expr: Box::new(Expr::MethodCall {
                        recv: Box::new(Expr::ByteStr(bytes.clone())),
                        method: "as_ptr".into(),
                        args: Vec::new(),
                    }),
                    ty: self.lower_type(&value.ty)?,
                }
            }
            ValueKind::Arith {
                op: op @ (ir::ArithOp::Add | ir::ArithOp::Sub | ir::ArithOp::Mul),
                left,
                right,
                semantics:
                    ir::ArithSema::Integer {
                        overflow: ir::Overflow::Wrap,
                    },
            } if !matches!(self.tables.resolve_type(&value.ty), ir::Type::Vector { .. }) => {
                Expr::MethodCall {
                    recv: Box::new(self.lower_value(left)?),
                    method: format!(
                        "wrapping_{}",
                        overflow_method(*op).ok_or_else(|| unsupported_value(value))?
                    ),
                    args: vec![self.lower_value(right)?],
                }
            }
            ValueKind::Overflow {
                op,
                left,
                right,
                result,
            } => self.lower_overflow(value, *op, left, right, result)?,
            ValueKind::Arith {
                op, left, right, ..
            } => Expr::Binary {
                op: match op {
                    ir::ArithOp::Add => BinOp::Add,
                    ir::ArithOp::Sub => BinOp::Sub,
                    ir::ArithOp::Mul => BinOp::Mul,
                    ir::ArithOp::Div => BinOp::Div,
                    ir::ArithOp::Rem => BinOp::Rem,
                    ir::ArithOp::And => BinOp::BitAnd,
                    ir::ArithOp::Or => BinOp::BitOr,
                    ir::ArithOp::Xor => BinOp::BitXor,
                    ir::ArithOp::Shl => BinOp::Shl,
                    ir::ArithOp::Shr => BinOp::Shr,
                    _ => return Err(unsupported_value(value)),
                },
                lhs: Box::new(self.lower_value(left)?),
                rhs: Box::new(self.lower_value(right)?),
            },
            ValueKind::Unary {
                op: ir::UnaryArithOp::Neg,
                operand,
                ..
            } if matches!(
                value.ty,
                ir::Type::Numeric(ir::NumericType::Float(
                    ir::FloatType::F32 | ir::FloatType::F64 | ir::FloatType::F80
                ))
            ) =>
            {
                Expr::Unary {
                    op: rust::UnaryOp::Neg,
                    expr: Box::new(self.lower_value(operand)?),
                }
            }
            ValueKind::Unary {
                op,
                operand,
                semantics,
            } if matches!(
                value.ty,
                ir::Type::Bool | ir::Type::Numeric(ir::NumericType::Integer { .. })
            ) =>
            {
                let operand = self.lower_value(operand)?;
                match (op, semantics) {
                    (ir::UnaryArithOp::Not, _) => Expr::Unary {
                        op: rust::UnaryOp::Not,
                        expr: Box::new(operand),
                    },
                    (
                        ir::UnaryArithOp::Neg,
                        ir::ArithSema::Integer {
                            overflow: ir::Overflow::Undefined,
                        },
                    ) => Expr::Unary {
                        op: rust::UnaryOp::Neg,
                        expr: Box::new(operand),
                    },
                    (
                        ir::UnaryArithOp::Neg,
                        ir::ArithSema::Integer {
                            overflow: ir::Overflow::Wrap,
                        },
                    ) => Expr::MethodCall {
                        recv: Box::new(operand),
                        method: "wrapping_neg".into(),
                        args: Vec::new(),
                    },
                    _ => {
                        return Err(unsupported_value(value));
                    }
                }
            }
            ValueKind::Logical { op, left, right } if matches!(value.ty, ir::Type::Bool) => {
                Expr::Binary {
                    op: match op {
                        ir::LogicalOp::And => BinOp::And,
                        ir::LogicalOp::Or => BinOp::Or,
                    },
                    lhs: Box::new(self.lower_condition(left)?),
                    rhs: Box::new(self.lower_condition(right)?),
                }
            }
            ValueKind::Conditional {
                condition,
                then_value,
                else_value,
            } => {
                let select = Expr::If {
                    cond: Box::new(self.lower_condition(condition)?),
                    then_expr: Box::new(self.lower_value(then_value)?),
                    else_expr: Box::new(self.lower_value(else_value)?),
                };
                match self.lower_type(&value.ty)? {
                    ty @ rust::Type::FnPtr { .. } => {
                        let temp = self.next_temp();
                        Expr::Block(Box::new(rust::Block {
                            stmts: vec![Stmt::Let {
                                name: temp.clone(),
                                mutable: false,
                                ty: Some(ty),
                                init: Some(select),
                            }],
                            tail: Some(Box::new(Expr::Var(temp.into()))),
                        }))
                    }
                    _ => select,
                }
            }
            ValueKind::Compare { left, .. }
                if matches!(self.tables.resolve_type(&left.ty), ir::Type::Vector { .. }) =>
            {
                return Err(unsupported_value(value));
            }
            ValueKind::Compare {
                op, left, right, ..
            } => Expr::Binary {
                op: match op {
                    ir::CompareOp::Eq => BinOp::Eq,
                    ir::CompareOp::Ne => BinOp::Ne,
                    ir::CompareOp::Lt => BinOp::Lt,
                    ir::CompareOp::Le => BinOp::Le,
                    ir::CompareOp::Gt => BinOp::Gt,
                    ir::CompareOp::Ge => BinOp::Ge,
                },
                lhs: Box::new(self.lower_value(left)?),
                rhs: Box::new(self.lower_value(right)?),
            },
            ValueKind::PointerOffset {
                pointer,
                amount,
                subtract,
                element,
                ..
            } => self.lower_pointer_offset(value, pointer, amount, *subtract, element)?,
            ValueKind::PointerDifference {
                left,
                right,
                element,
            } => self.lower_pointer_difference(value, left, right, element)?,
            ValueKind::FunctionDecay {
                place:
                    ir::Place {
                        kind: PlaceKind::Binding(id),
                        ..
                    },
            } if self.tables.intrinsics.contains_key(id)
                || self.tables.function_type_has_vector(&value.ty) =>
            {
                return Err(unsupported_value(value));
            }
            ValueKind::FunctionDecay {
                place:
                    ir::Place {
                        kind: PlaceKind::Binding(id),
                        ..
                    },
            } => match self.tables.names.get(id) {
                Some(name) if !name.is_extern => {
                    self.dependencies.address_taken.insert(name.rust.clone());
                    Expr::Call {
                        func: Box::new(Expr::Var("Some".into())),
                        args: vec![Expr::Var(name.rust.as_str().into())],
                        binding: CallBinding::Generated,
                    }
                }
                Some(name) => {
                    let address = rust::Type::Ptr {
                        mutable: false,
                        inner: Box::new(rust::Type::Unit),
                    };
                    Expr::Transmute {
                        from: address.clone(),
                        to: self.lower_type(&value.ty)?,
                        expr: Box::new(Expr::Cast {
                            expr: Box::new(Expr::Var(name.rust.as_str().into())),
                            ty: address,
                        }),
                    }
                }
                None => {
                    return Err(unsupported_value(value));
                }
            },
            ValueKind::Null => self.lower_null(value)?,
            ValueKind::Call {
                callee: ir::Callee::Direct(id),
                arguments,
                ..
            } if self.tables.intrinsics.contains_key(id) => {
                self.lower_intrinsic(*id, self.tables.intrinsics[id], arguments, &value.ty)?
            }
            ValueKind::Call {
                callee: ir::Callee::Direct(id),
                arguments,
                ..
            } if self.tables.names.get(id).is_some_and(|name| name.is_extern)
                && std::iter::once(&value.ty)
                    .chain(arguments.iter().map(|argument| &argument.ty))
                    .any(|ty| self.tables.holds_long_double(ty)) =>
            {
                self.lower_long_double_bridge(&self.tables.names[id], arguments, &value.ty)?
            }
            ValueKind::Call {
                callee: ir::Callee::Direct(id),
                signature: ir::Type::Function { parameters, .. },
                arguments,
                ..
            } if self
                .tables
                .names
                .get(id)
                .is_some_and(|name| name.is_variadic)
                && arguments
                    .iter()
                    .skip(parameters.len())
                    .any(|argument| self.tables.holds_long_double(&argument.ty)) =>
            {
                self.lower_long_double_variadic_trampoline(
                    &self.tables.names[id],
                    arguments,
                    parameters.len(),
                    &value.ty,
                )?
            }
            ValueKind::Call {
                callee, arguments, ..
            } => self.lower_call(callee, arguments)?,
            ValueKind::Aggregate { members, zero_fill } => {
                self.lower_aggregate(&value.ty, members, *zero_fill)?
            }
            _ => {
                return Err(unsupported_value(value));
            }
        })
    }

    pub(super) fn lower_aggregate(
        &mut self,
        ty: &ir::Type,
        members: &[ir::AggregateMember],
        zero_fill: bool,
    ) -> Result<Expr> {
        let unsupported = || {
            Failure::from(Construct::Value {
                kind: "Aggregate".into(),
                ir: ty.to_string(),
            })
        };
        if let ir::Type::Vector { element, lanes } = self.tables.resolve_type(ty) {
            self.lower_type(ty)?;
            let array = ir::Type::Array {
                element: Box::new(ir::Type::Numeric(*element)),
                length: Some((*lanes).into()),
            };
            return Ok(Expr::Call {
                func: Box::new(Expr::Var("std::simd::Simd::from_array".into())),
                args: vec![self.lower_aggregate(&array, members, zero_fill)?],
                binding: CallBinding::Generated,
            });
        }
        let lowered_ty = self.lower_type(ty)?;
        let fields = self.tables.record_fields(ty);
        let resolved = self.tables.resolve_type(ty);
        if fields.is_none() && !matches!(resolved, ir::Type::Array { .. }) {
            return Err(unsupported());
        }
        let field_name = |index: usize| {
            fields
                .and_then(|fields| fields.get(index))
                .map(|field| field_name(field, index))
                .ok_or_else(unsupported)
        };
        let complete = !zero_fill && fields.is_none_or(|fields| fields.len() == members.len()) && !self.tables.is_union(ty) && !self.tables.has_bit_fields(ty) && members.iter().enumerate().all(|(position, member)| {
            matches!(
                (&member.target, fields),
                (ir::AggregateTarget::Field(index), Some(_)) if *index == position
            ) || matches!(
                (&member.target, resolved),
                (ir::AggregateTarget::Index(index), ir::Type::Array { .. }) if *index == position as u64
            )
        });
        if complete {
            let values = members
                .iter()
                .map(|member| self.lower_value(&member.value))
                .collect::<Result<Vec<_>>>()?;
            return Ok(match (&lowered_ty, fields) {
                (rust::Type::Custom(name), Some(_)) => Expr::StructLit {
                    name: name.clone(),
                    fields: (0..values.len())
                        .map(field_name)
                        .collect::<Result<Vec<_>>>()?
                        .into_iter()
                        .zip(values)
                        .collect(),
                },
                _ => Expr::ArrayLit(values),
            });
        }
        let target = self.next_temp();
        let mut stmts = vec![Stmt::Let {
            name: target.clone(),
            mutable: true,
            ty: Some(lowered_ty),
            init: Some(zeroed()),
        }];
        for member in members {
            if let ir::AggregateTarget::Field(index) = member.target
                && let Some(unit) = self.tables.bit_unit_of(ty, index)
            {
                let setter = Stmt::Expr(Expr::MethodCall {
                    recv: Box::new(Expr::Field {
                        base: Box::new(Expr::Var(target.as_str().into())),
                        field: bit_unit_name(unit),
                    }),
                    method: format!("__set_{}", field_name(index)?),
                    args: vec![self.lower_value(&member.value)?],
                });
                stmts.push(if self.tables.is_union(ty) {
                    Stmt::Unsafe {
                        body: rust::Block {
                            stmts: vec![setter],
                            tail: None,
                        },
                    }
                } else {
                    setter
                });
                continue;
            }
            let place = match member.target {
                ir::AggregateTarget::Field(index) => Expr::Field {
                    base: Box::new(Expr::Var(target.as_str().into())),
                    field: field_name(index)?,
                },
                ir::AggregateTarget::Index(index) => Expr::Index {
                    base: Box::new(Expr::Var(target.as_str().into())),
                    index: Box::new(Expr::Value(rust::RustValue::U128(index.into()))),
                },
                ir::AggregateTarget::Range { .. } => return Err(unsupported()),
            };
            stmts.push(Stmt::Assign {
                target: place,
                value: self.lower_value(&member.value)?,
            });
        }
        Ok(Expr::Block(Box::new(rust::Block {
            stmts,
            tail: Some(Box::new(Expr::Var(target.as_str().into()))),
        })))
    }

    pub(super) fn lower_number(&mut self, number: &Number, ty: &ir::Type) -> Result<Expr> {
        let value = match number {
            Number::Bool(value) => rust::RustValue::Bool(*value),
            Number::Integer(value) => rust::RustValue::U128(
                value
                    .to_string()
                    .parse()
                    .map_err(|_| unsupported_constant(number))?,
            ),
            Number::SignedInteger(value) => rust::RustValue::I128(
                value
                    .to_string()
                    .parse()
                    .map_err(|_| unsupported_constant(number))?,
            ),
            Number::FloatBits(bits) => {
                let (literal, finite, name) = match ty {
                    ir::Type::Numeric(ir::NumericType::Float(ir::FloatType::F32)) => {
                        let value = f32::from_bits(*bits as u32);
                        (format!("{value:?}"), value.is_finite(), "f32")
                    }
                    ir::Type::Numeric(ir::NumericType::Float(ir::FloatType::F64)) => {
                        let value = f64::from_bits(*bits as u64);
                        (format!("{value:?}"), value.is_finite(), "f64")
                    }
                    ir::Type::Numeric(ir::NumericType::Float(ir::FloatType::F80)) => {
                        self.lower_type(ty)?;
                        return Ok(long_double_literal(*bits));
                    }
                    _ => return Err(unsupported_constant(number)),
                };
                if !finite {
                    return Ok(Expr::HexFloat(format!("{name}::from_bits({bits:#x})")));
                }
                return Ok(match literal.strip_prefix('-') {
                    Some(magnitude) => Expr::Unary {
                        op: rust::UnaryOp::Neg,
                        expr: Box::new(Expr::HexFloat(format!("{magnitude}{name}"))),
                    },
                    None => Expr::HexFloat(format!("{literal}{name}")),
                });
            }
            _ => return Err(unsupported_constant(number)),
        };
        Ok(Expr::Cast {
            expr: Box::new(Expr::Value(value)),
            ty: self.lower_type(ty)?,
        })
    }
}

pub(super) fn unsupported_value(value: &ir::Value) -> Failure {
    Construct::Value {
        kind: variant_name(&value.node.value),
        ir: value.display(false).to_string(),
    }
    .into()
}

fn unsupported_constant(number: &Number) -> Failure {
    Construct::Value {
        kind: "Constant".into(),
        ir: format!("{number:?}"),
    }
    .into()
}
