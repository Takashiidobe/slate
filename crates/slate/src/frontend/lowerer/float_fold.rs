use super::*;
use rustc_apfloat::{
    Float, FloatConvert,
    ieee::{BFloatS, DoubleS, HalfS, IeeeFloat, QuadS, Semantics, SingleS, X87DoubleExtendedS},
};

pub(super) trait FoldSemantics: Semantics {
    const KIND: ir::FloatType;
}

impl FoldSemantics for BFloatS {
    const KIND: ir::FloatType = ir::FloatType::BF16;
}

impl FoldSemantics for HalfS {
    const KIND: ir::FloatType = ir::FloatType::F16;
}

impl FoldSemantics for SingleS {
    const KIND: ir::FloatType = ir::FloatType::F32;
}

impl FoldSemantics for DoubleS {
    const KIND: ir::FloatType = ir::FloatType::F64;
}

impl FoldSemantics for X87DoubleExtendedS {
    const KIND: ir::FloatType = ir::FloatType::F80;
}

impl FoldSemantics for QuadS {
    const KIND: ir::FloatType = ir::FloatType::F128;
}

type Pair<S> = [IeeeFloat<S>; 2];

fn convert<T: Semantics, S: Semantics>(value: IeeeFloat<T>) -> IeeeFloat<S> {
    value.convert(&mut false).value
}

fn nearest_even(floating: &ir::FloatingSemantics) -> bool {
    matches!(floating.rounding, ir::Rounding::NearestEven)
}

fn unit<S: Semantics>(value: IeeeFloat<S>, infinite: bool) -> IeeeFloat<S> {
    let magnitude = if infinite {
        IeeeFloat::<S>::from_u128(1).value
    } else {
        IeeeFloat::<S>::ZERO
    };
    magnitude.copy_sign(value)
}

fn complex_mul<S: Semantics>([a, b]: Pair<S>, [c, d]: Pair<S>, basic: bool) -> Pair<S> {
    let (ac, bd, ad, bc) = ((a * c).value, (b * d).value, (a * d).value, (b * c).value);
    let (re, im) = ((ac - bd).value, (ad + bc).value);
    if basic || !(re.is_nan() && im.is_nan()) {
        return [re, im];
    }
    let (mut a, mut b, mut c, mut d) = (a, b, c, d);
    let zero_nan = |value: &mut IeeeFloat<S>| {
        if value.is_nan() {
            *value = IeeeFloat::<S>::ZERO.copy_sign(*value);
        }
    };
    let mut recalc = false;
    if a.is_infinite() || b.is_infinite() {
        a = unit(a, a.is_infinite());
        b = unit(b, b.is_infinite());
        zero_nan(&mut c);
        zero_nan(&mut d);
        recalc = true;
    }
    if c.is_infinite() || d.is_infinite() {
        c = unit(c, c.is_infinite());
        d = unit(d, d.is_infinite());
        zero_nan(&mut a);
        zero_nan(&mut b);
        recalc = true;
    }
    if !recalc && [ac, bd, ad, bc].iter().any(|value| value.is_infinite()) {
        for value in [&mut a, &mut b, &mut c, &mut d] {
            zero_nan(value);
        }
        recalc = true;
    }
    if !recalc {
        return [re, im];
    }
    let inf = IeeeFloat::<S>::INFINITY;
    let re = ((a * c).value - (b * d).value).value;
    let im = ((a * d).value + (b * c).value).value;
    [(inf * re).value, (inf * im).value]
}

fn complex_div<S: Semantics>([a, b]: Pair<S>, [c, d]: Pair<S>, basic: bool) -> Pair<S> {
    let numerators = |[a, b]: Pair<S>, [c, d]: Pair<S>| {
        [
            ((a * c).value + (b * d).value).value,
            ((b * c).value - (a * d).value).value,
        ]
    };
    if basic {
        let denom = ((c * c).value + (d * d).value).value;
        return numerators([a, b], [c, d]).map(|part| (part / denom).value);
    }
    let max = c.abs().max(d.abs());
    let mut logb = 0;
    let (mut c, mut d) = (c, d);
    if max.is_finite() {
        logb = max.ilogb();
        c = c.scalbn(-logb);
        d = d.scalbn(-logb);
    }
    let denom = ((c * c).value + (d * d).value).value;
    let [re, im] = numerators([a, b], [c, d]).map(|part| (part / denom).value.scalbn(-logb));
    if !(re.is_nan() && im.is_nan()) {
        return [re, im];
    }
    if denom.is_pos_zero() && (!a.is_nan() || !b.is_nan()) {
        let inf = IeeeFloat::<S>::INFINITY.copy_sign(c);
        return [(inf * a).value, (inf * b).value];
    }
    if (a.is_infinite() || b.is_infinite()) && c.is_finite() && d.is_finite() {
        let units = [unit(a, a.is_infinite()), unit(b, b.is_infinite())];
        return numerators(units, [c, d]).map(|part| (IeeeFloat::<S>::INFINITY * part).value);
    }
    if max.is_infinite() && a.is_finite() && b.is_finite() {
        let units = [unit(c, c.is_infinite()), unit(d, d.is_infinite())];
        return numerators([a, b], units).map(|part| (IeeeFloat::<S>::ZERO * part).value);
    }
    [re, im]
}

impl Tables<'_> {
    pub(super) fn float_kind(&self, ty: &ir::Type) -> Option<ir::FloatType> {
        match self.resolve_type(ty) {
            ir::Type::Numeric(ir::NumericType::Float(kind)) => Some(*kind),
            _ => None,
        }
    }

    pub(super) fn complex_float_kind(&self, ty: &ir::Type) -> Option<ir::FloatType> {
        match self.resolve_type(ty) {
            ir::Type::Complex(ir::NumericType::Float(kind)) => Some(*kind),
            _ => None,
        }
    }

    pub(super) fn fold_float<S: FoldSemantics>(&self, value: &ir::Value) -> Option<IeeeFloat<S>> {
        if self.float_kind(&value.ty)? != S::KIND {
            return None;
        }
        match &value.node.value {
            ValueKind::Constant(ir::Number::FloatBits(bits)) => Some(IeeeFloat::from_bits(*bits)),
            ValueKind::Unary {
                op: ir::UnaryArithOp::Neg,
                operand,
                ..
            } => Some(-self.fold_float::<S>(operand)?),
            ValueKind::Convert {
                kind: ir::ConversionKind::IntToFloat,
                operand,
                semantics: ir::ConversionSema::IntToFloat { floating, .. },
                ..
            } if nearest_even(floating) => {
                Some(IeeeFloat::<S>::from_i128(self.fold_integer(operand)?).value)
            }
            ValueKind::Convert {
                kind: ir::ConversionKind::FloatWiden,
                operand,
                ..
            } => self.fold_float_as::<S>(operand),
            ValueKind::Convert {
                kind: ir::ConversionKind::FloatNarrow,
                operand,
                semantics: ir::ConversionSema::Floating(floating),
                ..
            } if nearest_even(floating) => self.fold_float_as::<S>(operand),
            ValueKind::Convert {
                kind: ir::ConversionKind::ComplexToReal,
                operand,
                ..
            } => Some(self.fold_complex_as::<S>(operand)?[0]),
            ValueKind::Arith {
                op,
                left,
                right,
                semantics: ir::ArithSema::Floating { floating, .. },
            } if nearest_even(floating) => {
                let (left, right) = (self.fold_float::<S>(left)?, self.fold_float::<S>(right)?);
                let result = match op {
                    ir::ArithOp::Add => left + right,
                    ir::ArithOp::Sub => left - right,
                    ir::ArithOp::Mul => left * right,
                    ir::ArithOp::Div => left / right,
                    _ => return None,
                };
                Some(result.value)
            }
            _ => None,
        }
    }

    fn fold_float_as<S: FoldSemantics>(&self, value: &ir::Value) -> Option<IeeeFloat<S>> {
        Some(match self.float_kind(&value.ty)? {
            ir::FloatType::BF16 => convert(self.fold_float::<BFloatS>(value)?),
            ir::FloatType::F16 => convert(self.fold_float::<HalfS>(value)?),
            ir::FloatType::F32 => convert(self.fold_float::<SingleS>(value)?),
            ir::FloatType::F64 => convert(self.fold_float::<DoubleS>(value)?),
            ir::FloatType::F80 => convert(self.fold_float::<X87DoubleExtendedS>(value)?),
            ir::FloatType::F128 => convert(self.fold_float::<QuadS>(value)?),
            _ => return None,
        })
    }

    pub(super) fn fold_complex<S: FoldSemantics>(&self, value: &ir::Value) -> Option<Pair<S>> {
        if self.complex_float_kind(&value.ty)? != S::KIND {
            return None;
        }
        match &value.node.value {
            ValueKind::Aggregate { members, .. } => {
                let mut parts = [IeeeFloat::<S>::ZERO; 2];
                for member in members {
                    let ir::AggregateTarget::Index(index @ (0 | 1)) = member.target else {
                        return None;
                    };
                    parts[index as usize] = self.fold_float_as::<S>(&member.value)?;
                }
                Some(parts)
            }
            ValueKind::Convert {
                kind: ir::ConversionKind::ComplexConvert,
                operand,
                ..
            } => self.fold_complex_as::<S>(operand),
            ValueKind::Convert {
                kind: ir::ConversionKind::RealToComplex,
                operand,
                ..
            } => Some([self.fold_float_as::<S>(operand)?, IeeeFloat::<S>::ZERO]),
            ValueKind::Unary {
                op: ir::UnaryArithOp::Neg,
                operand,
                ..
            } => Some(self.fold_complex::<S>(operand)?.map(|part| -part)),
            ValueKind::Unary {
                op: ir::UnaryArithOp::Not,
                operand,
                ..
            } => {
                let [re, im] = self.fold_complex::<S>(operand)?;
                Some([re, -im])
            }
            ValueKind::Arith {
                op,
                left,
                right,
                semantics: ir::ArithSema::ComplexFloating { floating, range },
            } if nearest_even(floating) => {
                let basic = matches!(range, ir::ComplexRange::Basic);
                if matches!(range, ir::ComplexRange::Improved) && matches!(op, ir::ArithOp::Div) {
                    return None;
                }
                let side = |operand: &ir::Value| {
                    if self.complex_float_kind(&operand.ty).is_some() {
                        self.fold_complex::<S>(operand)
                            .map(|[re, im]| (re, Some(im)))
                    } else {
                        self.fold_float::<S>(operand).map(|re| (re, None))
                    }
                };
                let ((a, b), (c, d)) = (side(left)?, side(right)?);
                let zero = IeeeFloat::<S>::ZERO;
                Some(match (op, b, d) {
                    (ir::ArithOp::Add, ..) => [
                        (a + c).value,
                        match (b, d) {
                            (Some(b), Some(d)) => (b + d).value,
                            (Some(b), None) => b,
                            (None, Some(d)) => d,
                            (None, None) => return None,
                        },
                    ],
                    (ir::ArithOp::Sub, ..) => [
                        (a - c).value,
                        match (b, d) {
                            (Some(b), Some(d)) => (b - d).value,
                            (Some(b), None) => b,
                            (None, Some(d)) => -d,
                            (None, None) => return None,
                        },
                    ],
                    (ir::ArithOp::Mul, Some(b), Some(d)) => {
                        complex_mul([a, b], [c, d], !matches!(range, ir::ComplexRange::Full))
                    }
                    (ir::ArithOp::Mul, Some(b), None) => [(a * c).value, (b * c).value],
                    (ir::ArithOp::Mul, None, Some(d)) => [(a * c).value, (a * d).value],
                    (ir::ArithOp::Div, Some(b), Some(d)) => complex_div([a, b], [c, d], basic),
                    (ir::ArithOp::Div, Some(b), None) => [(a / c).value, (b / c).value],
                    (ir::ArithOp::Div, None, Some(d)) => complex_div([a, zero], [c, d], basic),
                    _ => return None,
                })
            }
            _ => None,
        }
    }

    fn fold_complex_as<S: FoldSemantics>(&self, value: &ir::Value) -> Option<Pair<S>> {
        Some(match self.complex_float_kind(&value.ty)? {
            ir::FloatType::BF16 => self.fold_complex::<BFloatS>(value)?.map(convert),
            ir::FloatType::F16 => self.fold_complex::<HalfS>(value)?.map(convert),
            ir::FloatType::F32 => self.fold_complex::<SingleS>(value)?.map(convert),
            ir::FloatType::F64 => self.fold_complex::<DoubleS>(value)?.map(convert),
            ir::FloatType::F80 => self.fold_complex::<X87DoubleExtendedS>(value)?.map(convert),
            ir::FloatType::F128 => self.fold_complex::<QuadS>(value)?.map(convert),
            _ => return None,
        })
    }

    pub(super) fn fold_complex_bits(&self, value: &ir::Value) -> Option<[u128; 2]> {
        Some(match self.complex_float_kind(&value.ty)? {
            ir::FloatType::BF16 => self.fold_complex::<BFloatS>(value)?.map(Float::to_bits),
            ir::FloatType::F16 => self.fold_complex::<HalfS>(value)?.map(Float::to_bits),
            ir::FloatType::F32 => self.fold_complex::<SingleS>(value)?.map(Float::to_bits),
            ir::FloatType::F64 => self.fold_complex::<DoubleS>(value)?.map(Float::to_bits),
            ir::FloatType::F80 => self
                .fold_complex::<X87DoubleExtendedS>(value)?
                .map(Float::to_bits),
            ir::FloatType::F128 => self.fold_complex::<QuadS>(value)?.map(Float::to_bits),
            _ => return None,
        })
    }
}
