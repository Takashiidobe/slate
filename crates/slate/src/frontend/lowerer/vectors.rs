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
                | Prim::F16
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
        "core::simd::Simd"
    };
    Expr::var(format!("{base}::{method}").into())
}

fn map_vector_lanes(ty: &rust::Type, vector: Expr, map: Expr) -> Expr {
    let lanes = Expr::MethodCall {
        recv: Box::new(vector),
        method: "to_array".into(),
        args: Vec::new(),
        unsafe_: rust::Unsafe(false),
    };
    Expr::Call {
        func: Box::new(vector_constructor(ty, "from_array")),
        args: vec![Expr::MethodCall {
            recv: Box::new(lanes),
            method: "map".into(),
            args: vec![map],
            unsafe_: rust::Unsafe(false),
        }],
        binding: CallBinding::Generated,
        unsafe_: rust::Unsafe(false),
    }
}

impl Tables<'_> {
    pub(super) fn vector_element(&self, ty: &ir::Type) -> Option<ir::NumericType> {
        match self.resolve_type(ty) {
            ir::Type::Vector { element, .. } => Some(*element),
            _ => None,
        }
    }
}

fn lane_index(lane: u32) -> Expr {
    Expr::Value(rust::RustValue::U128(lane.into()))
}

fn swizzle_lane_access(address: Expr, lane: Expr, method: &str, args: Vec<Expr>) -> Expr {
    unsafe_op(Expr::MethodCall {
        recv: Box::new(Expr::MethodCall {
            recv: Box::new(address),
            method: "add".into(),
            args: vec![lane],
            unsafe_: rust::Unsafe(true),
        }),
        method: method.into(),
        args,
        unsafe_: rust::Unsafe(true),
    })
}

impl FunctionLowerer<'_, '_> {
    fn swizzle_access(
        &mut self,
        place: &ir::Place,
        mutable: bool,
    ) -> Result<Option<(Expr, Vec<Expr>)>> {
        let (projection, index) = match &place.kind {
            PlaceKind::Lane { base, index } => (&**base, Some(&**index)),
            _ => (place, None),
        };
        let PlaceKind::Swizzle { base, lanes } = &projection.kind else {
            return Ok(None);
        };
        let unsupported = || Construct::Place {
            ir: place.to_string(),
        };
        let mut base = &**base;
        let mut lanes = lanes.clone();
        while let PlaceKind::Swizzle {
            base: inner,
            lanes: mapping,
        } = &base.kind
        {
            for lane in &mut lanes {
                *lane = *mapping.get(*lane as usize).ok_or_else(unsupported)?;
            }
            base = inner;
        }

        let element = self
            .tables
            .vector_element(&base.ty)
            .ok_or_else(unsupported)?;
        let vector = self.lower_type(&base.ty)?;
        let element = self.lower_type(&ir::Type::Numeric(element))?;
        let address = Expr::Cast {
            expr: Box::new(Expr::Cast {
                expr: Box::new(self.lower_address(base, mutable)?),
                ty: rust::Type::Ptr {
                    mutable,
                    inner: Box::new(vector),
                },
            }),
            ty: rust::Type::Ptr {
                mutable,
                inner: Box::new(element),
            },
        };
        let lanes: Vec<_> = lanes.into_iter().map(lane_index).collect();
        let lanes = match index {
            Some(index) => vec![Expr::Index {
                base: Box::new(Expr::ArrayLit(lanes)),
                index: Box::new(self.lower_index(index)?),
            }],
            None => lanes,
        };
        Ok(Some((address, lanes)))
    }

    pub(super) fn lower_swizzle_read(&mut self, place: &ir::Place) -> Result<Option<Expr>> {
        let Some((address, lanes)) = self.swizzle_access(place, false)? else {
            return Ok(None);
        };
        let mut stmts = Vec::new();
        let address = self.bind(&mut stmts, address);
        let mut reads = lanes
            .into_iter()
            .map(|lane| swizzle_lane_access(address.clone(), lane, "read_unaligned", Vec::new()));
        let result = if matches!(place.kind, PlaceKind::Lane { .. }) {
            reads.next().ok_or_else(|| Construct::Place {
                ir: place.to_string(),
            })?
        } else {
            let ty = self.lower_type(&place.ty)?;
            let name = self.next_temp();
            stmts.push(Stmt::Let {
                origin: rust::LocalOrigin::Synthetic,
                name: name.clone(),
                mutable: false,
                ty: Some(ty.clone()),
                init: Some(Expr::Call {
                    func: Box::new(vector_constructor(&ty, "from_array")),
                    args: vec![Expr::ArrayLit(reads.collect())],
                    binding: CallBinding::Generated,
                    unsafe_: rust::Unsafe(false),
                }),
            });
            Expr::var(name.as_str().into())
        };
        Ok(Some(Expr::Block(Box::new(rust::Block {
            stmts,
            tail: Some(Box::new(result)),
        }))))
    }

    pub(super) fn lower_swizzle_write(
        &mut self,
        place: &ir::Place,
        value: &Expr,
    ) -> Result<Option<Stmt>> {
        let Some((address, lanes)) = self.swizzle_access(place, true)? else {
            return Ok(None);
        };
        let mut body = Vec::new();
        let operand = self.bind(&mut body, value.clone());
        let address = self.bind(&mut body, address);
        for (index, lane) in lanes.into_iter().enumerate() {
            let value = if matches!(place.kind, PlaceKind::Lane { .. }) {
                operand.clone()
            } else {
                Expr::Index {
                    base: Box::new(operand.clone()),
                    index: Box::new(Expr::Value(rust::RustValue::U128(index as u128))),
                }
            };
            body.push(Stmt::Expr(swizzle_lane_access(
                address.clone(),
                lane,
                "write_unaligned",
                vec![value],
            )));
        }
        Ok(Some(Stmt::Scope { body }))
    }

    pub(super) fn lower_elementwise_abs(
        &mut self,
        value: &ir::Value,
        arguments: &[ir::Value],
    ) -> Result<Expr> {
        let [operand] = arguments else {
            return Err(unsupported_value(value));
        };
        let vector = self.tables.vector_element(&value.ty);
        let element = match vector {
            Some(element) => element,
            None => match self.tables.resolve_type(&value.ty) {
                ir::Type::Numeric(numeric) => *numeric,
                _ => return Err(unsupported_value(value)),
            },
        };
        let lane = |recv: Expr| -> Result<Expr> {
            let method = match element {
                ir::NumericType::Integer {
                    signed: true,
                    bit_precise: false,
                    ..
                } => "wrapping_abs",
                ir::NumericType::Integer {
                    signed: false,
                    bit_precise: false,
                    ..
                } => return Ok(recv),
                ir::NumericType::Float(ir::FloatType::F32 | ir::FloatType::F64) => "abs",
                _ => return Err(unsupported_value(value)),
            };
            Ok(Expr::MethodCall {
                recv: Box::new(recv),
                method: method.into(),
                args: Vec::new(),
                unsafe_: rust::Unsafe(false),
            })
        };
        let lowered = self.lower_value(operand)?;
        if vector.is_none() {
            return lane(lowered);
        }
        Ok(map_vector_lanes(
            &self.lower_type(&value.ty)?,
            lowered,
            Expr::Closure {
                params: vec!["__lane".into()],
                body: Box::new(lane(Expr::var("__lane".into()))?),
            },
        ))
    }

    pub(super) fn lower_elementwise_bitreverse(
        &mut self,
        value: &ir::Value,
        arguments: &[ir::Value],
    ) -> Result<Expr> {
        let [operand] = arguments else {
            return Err(unsupported_value(value));
        };
        let lowered = self.lower_value(operand)?;
        if self.tables.wide_int(&value.ty).is_some() {
            return Ok(Expr::MethodCall {
                recv: Box::new(lowered),
                method: "reverse_bits".into(),
                args: Vec::new(),
                unsafe_: rust::Unsafe(false),
            });
        }

        let vector_element = self.tables.vector_element(&value.ty);
        let integer = vector_element.map_or_else(
            || self.tables.resolve_type(&value.ty).clone(),
            ir::Type::Numeric,
        );
        let ir::Type::Numeric(ir::NumericType::Integer { width, .. }) = integer else {
            return Err(unsupported_value(value));
        };
        let rust::Type::Prim(prim) = self.lower_type(&integer)? else {
            return Err(unsupported_value(value));
        };
        let padding = prim_width(prim) - width;
        let reverse_bits =
            Expr::var(format!("{}::reverse_bits", prim.resolved().spelling()).into());
        if vector_element.is_some() {
            if padding != 0 {
                return Err(unsupported_value(value));
            }
            return Ok(map_vector_lanes(
                &self.lower_type(&value.ty)?,
                lowered,
                reverse_bits,
            ));
        }

        let reversed = Expr::Call {
            func: Box::new(reverse_bits),
            args: vec![lowered],
            binding: CallBinding::Generated,
            unsafe_: rust::Unsafe(false),
        };
        if padding == 0 {
            return Ok(reversed);
        }
        Ok(Expr::Binary {
            op: BinOp::Shr,
            lhs: Box::new(reversed),
            rhs: Box::new(Expr::Value(rust::RustValue::I64(i64::from(padding)))),
        })
    }

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
                    unsafe_: rust::Unsafe(false),
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
                    unsafe_: rust::Unsafe(false),
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
            name: "core::simd::simd_swizzle".into(),
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
            let lowered = self.lower_value(operand)?;
            let recv = if matches!(lowered, Expr::Var { .. }) {
                lowered
            } else {
                let vector = self.next_temp();
                stmts.push(Stmt::Let {
                    origin: rust::LocalOrigin::Synthetic,
                    name: vector.clone(),
                    mutable: false,
                    ty: Some(self.lower_type(&operand.ty)?),
                    init: Some(lowered),
                });
                Expr::var(vector.into())
            };
            let array = Expr::MethodCall {
                recv: Box::new(recv),
                method: "to_array".into(),
                args: Vec::new(),
                unsafe_: rust::Unsafe(false),
            };
            arrays.push(self.bind(&mut stmts, array));
        }
        let at = |array: &Expr| Expr::Index {
            base: Box::new(array.clone()),
            index: Box::new(Expr::var("__lane".into())),
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
            origin: rust::LocalOrigin::Synthetic,
            name: temp.clone(),
            mutable: false,
            ty: Some(result.clone()),
            init: Some(Expr::Call {
                func: Box::new(vector_constructor(&result, "from_array")),
                args: vec![Expr::Call {
                    func: Box::new(Expr::var("core::array::from_fn".into())),
                    args: vec![Expr::Closure {
                        params: vec!["__lane".into()],
                        body: Box::new(mask),
                    }],
                    binding: CallBinding::Generated,
                    unsafe_: rust::Unsafe(false),
                }],
                binding: CallBinding::Generated,
                unsafe_: rust::Unsafe(false),
            }),
        });
        Ok(Expr::Block(Box::new(rust::Block {
            stmts,
            tail: Some(Box::new(Expr::var(temp.into()))),
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
                        func: Box::new(Expr::var("core::simd::Simd::splat".into())),
                        args: vec![Expr::Value(rust::RustValue::I64(0))],
                        binding: CallBinding::Generated,
                        unsafe_: rust::Unsafe(false),
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
                unsafe_: rust::Unsafe(false),
            };
            return Ok(Expr::Call {
                func: Box::new(vector_constructor(&target, "from_array")),
                args: vec![Expr::MethodCall {
                    recv: Box::new(array),
                    method: "map".into(),
                    args: vec![Expr::Closure {
                        params: vec!["__lane".into()],
                        body: Box::new(Expr::Cast {
                            expr: Box::new(Expr::var("__lane".into())),
                            ty: lane,
                        }),
                    }],
                    unsafe_: rust::Unsafe(false),
                }],
                binding: CallBinding::Generated,
                unsafe_: rust::Unsafe(false),
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
                    ["core", "simd", "num", trait_name, "cast"].map(rust::Ident::from),
                ),
                type_args: vec![lane],
            }),
            args: vec![self.lower_value(operand)?],
            binding: CallBinding::Generated,
            unsafe_: rust::Unsafe(false),
        })
    }

    pub(super) fn lower_vector_select(
        &mut self,
        value: &ir::Value,
        arguments: &[ir::Value],
    ) -> Result<Expr> {
        let [mask, left, right] = arguments else {
            return Err(unsupported_value(value));
        };
        let (
            ir::Type::Vector { element, lanes, .. },
            ir::Type::Numeric(ir::NumericType::Integer { width, .. }),
        ) = (
            self.tables.resolve_type(&value.ty).clone(),
            self.tables.resolve_type(&mask.ty).clone(),
        )
        else {
            return Err(unsupported_value(value));
        };
        let lane_width = match element {
            ir::NumericType::Integer { width, .. } => width,
            ir::NumericType::Float(ir::FloatType::F32) => 32,
            ir::NumericType::Float(ir::FloatType::F64) => 64,
            ir::NumericType::Float(_) => return Err(unsupported_value(value)),
        };
        if !matches!(lane_width, 8 | 16 | 32 | 64) {
            return Err(unsupported_value(value));
        }
        let unsigned = match width {
            8 => Prim::U8,
            16 => Prim::U16,
            32 => Prim::U32,
            64 => Prim::U64,
            _ => return Err(unsupported_value(value)),
        };
        if is_slate_vector(&self.lower_type(&value.ty)?) {
            return Err(unsupported_value(value));
        }
        let bits = Expr::Cast {
            expr: Box::new(Expr::Cast {
                expr: Box::new(self.lower_value(mask)?),
                ty: rust::Type::Prim(unsigned),
            }),
            ty: rust::Type::Prim(Prim::U64),
        };
        let mask = Expr::Call {
            func: Box::new(Expr::var(
                format!("core::simd::Mask::<i{lane_width}, {lanes}>::from_bitmask").into(),
            )),
            args: vec![bits],
            binding: CallBinding::Generated,
            unsafe_: rust::Unsafe(false),
        };
        Ok(Expr::Call {
            func: Box::new(Expr::var("core::simd::Select::select".into())),
            args: vec![mask, self.lower_value(left)?, self.lower_value(right)?],
            binding: CallBinding::Generated,
            unsafe_: rust::Unsafe(false),
        })
    }

    pub(super) fn lower_vector_sign_bitmask(
        &mut self,
        value: &ir::Value,
        arguments: &[ir::Value],
    ) -> Result<Expr> {
        let [vector] = arguments else {
            return Err(unsupported_value(value));
        };
        let (
            &ir::Type::Vector {
                element:
                    ir::NumericType::Integer {
                        signed: true,
                        width,
                        ..
                    },
                lanes,
                ..
            },
            ir::Type::Numeric(ir::NumericType::Integer { .. }),
        ) = (
            self.tables.resolve_type(&vector.ty),
            self.tables.resolve_type(&value.ty),
        )
        else {
            return Err(unsupported_value(value));
        };
        if is_slate_vector(&self.lower_type(&vector.ty)?) {
            return Err(unsupported_value(value));
        }
        let negative = Expr::Call {
            func: Box::new(Expr::var("core::simd::cmp::SimdPartialOrd::simd_lt".into())),
            args: vec![
                self.lower_value(vector)?,
                Expr::Call {
                    func: Box::new(Expr::var(
                        format!("core::simd::Simd::<i{width}, {lanes}>::default").into(),
                    )),
                    args: vec![],
                    binding: CallBinding::Generated,
                    unsafe_: rust::Unsafe(false),
                },
            ],
            binding: CallBinding::Generated,
            unsafe_: rust::Unsafe(false),
        };
        Ok(Expr::Cast {
            expr: Box::new(Expr::MethodCall {
                recv: Box::new(negative),
                method: "to_bitmask".into(),
                args: vec![],
                unsafe_: rust::Unsafe(false),
            }),
            ty: self.lower_type(&value.ty)?,
        })
    }

    pub(super) fn lower_masked_load(
        &mut self,
        value: &ir::Value,
        arguments: &[ir::Value],
    ) -> Result<Expr> {
        let [pointer, passthrough, mask] = arguments else {
            return Err(unsupported_value(value));
        };
        let (enable, lane) = self.lane_enable(value, &value.ty, mask)?;
        let pointer = Expr::Cast {
            expr: Box::new(self.lower_value(pointer)?),
            ty: rust::Type::Ptr {
                mutable: false,
                inner: Box::new(lane),
            },
        };
        Ok(Expr::Call {
            func: Box::new(Expr::var("core::simd::Simd::load_select_ptr".into())),
            args: vec![pointer, enable, self.lower_value(passthrough)?],
            binding: CallBinding::Generated,
            unsafe_: rust::Unsafe(false),
        })
    }

    pub(super) fn lower_masked_store(
        &mut self,
        value: &ir::Value,
        arguments: &[ir::Value],
    ) -> Result<Expr> {
        let [pointer, vector, mask] = arguments else {
            return Err(unsupported_value(value));
        };
        let (enable, lane) = self.lane_enable(value, &vector.ty, mask)?;
        let pointer = Expr::Cast {
            expr: Box::new(self.lower_value(pointer)?),
            ty: rust::Type::Ptr {
                mutable: true,
                inner: Box::new(lane),
            },
        };
        Ok(Expr::Call {
            func: Box::new(Expr::var("core::simd::Simd::store_select_ptr".into())),
            args: vec![self.lower_value(vector)?, pointer, enable],
            binding: CallBinding::Generated,
            unsafe_: rust::Unsafe(false),
        })
    }

    fn lane_enable(
        &mut self,
        value: &ir::Value,
        vector: &ir::Type,
        mask: &ir::Value,
    ) -> Result<(Expr, rust::Type)> {
        let (
            ir::Type::Vector { element, lanes, .. },
            ir::Type::Numeric(ir::NumericType::Integer { width, .. }),
        ) = (
            self.tables.resolve_type(vector).clone(),
            self.tables.resolve_type(&mask.ty).clone(),
        )
        else {
            return Err(unsupported_value(value));
        };
        let lane_width = match element {
            ir::NumericType::Integer { width, .. } => width,
            ir::NumericType::Float(ir::FloatType::F32) => 32,
            ir::NumericType::Float(ir::FloatType::F64) => 64,
            ir::NumericType::Float(_) => return Err(unsupported_value(value)),
        };
        let unsigned = match width {
            8 => Prim::U8,
            16 => Prim::U16,
            32 => Prim::U32,
            64 => Prim::U64,
            _ => return Err(unsupported_value(value)),
        };
        let vector_type = self.lower_type(vector)?;
        if !matches!(lane_width, 8 | 16 | 32 | 64) || is_slate_vector(&vector_type) {
            return Err(unsupported_value(value));
        }
        let lane = self.lower_type(&ir::Type::Numeric(element))?;
        let bits = Expr::Cast {
            expr: Box::new(Expr::Cast {
                expr: Box::new(self.lower_value(mask)?),
                ty: rust::Type::Prim(unsigned),
            }),
            ty: rust::Type::Prim(Prim::U64),
        };
        let enable = Expr::Call {
            func: Box::new(Expr::var(
                format!("core::simd::Mask::<i{lane_width}, {lanes}>::from_bitmask").into(),
            )),
            args: vec![bits],
            binding: CallBinding::Generated,
            unsafe_: rust::Unsafe(false),
        };
        Ok((enable, lane))
    }
}
