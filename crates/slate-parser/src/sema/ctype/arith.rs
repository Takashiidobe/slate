use super::{CTypeKind, CTypes, FixedType, FloatKind, IntRank, QualType, layout::rank_width};
use crate::sema::numeric::ResolveError;
use crate::target_info::TargetInfo;

#[derive(Clone, Copy)]
struct Integer {
    width: u32,
    signed: bool,
    rank: u32,
    standard: bool,
}

impl CTypes {
    pub fn int(&mut self) -> QualType {
        self.qual(CTypeKind::Int {
            rank: IntRank::Int,
            signed: true,
        })
    }

    pub fn size_type(&mut self, target: &TargetInfo) -> QualType {
        self.pointer_integer(target, false)
    }

    pub fn ptrdiff_type(&mut self, target: &TargetInfo) -> QualType {
        self.pointer_integer(target, true)
    }

    fn pointer_integer(&mut self, target: &TargetInfo, signed: bool) -> QualType {
        let rank = if target.pointer_width == target.int_width {
            IntRank::Int
        } else if target.pointer_width == target.long_width {
            IntRank::Long
        } else {
            IntRank::LongLong
        };
        self.qual(CTypeKind::Int { rank, signed })
    }

    fn integer(&self, q: QualType, target: &TargetInfo) -> Option<Integer> {
        let (width, signed, rank, standard) = match self.canonical_kind(q) {
            CTypeKind::Bool => (1, false, 0, true),
            CTypeKind::Char => (8, target.char_signed, 1, true),
            CTypeKind::SChar => (8, true, 1, true),
            CTypeKind::UChar => (8, false, 1, true),
            CTypeKind::Int { rank, signed } => {
                (rank_width(*rank, target), *signed, *rank as u32 + 2, true)
            }
            CTypeKind::BitInt { width, signed } => (*width, *signed, 0, false),
            _ => return None,
        };
        Some(Integer {
            width,
            signed,
            rank,
            standard,
        })
    }

    pub fn integer_promotion(
        &mut self,
        q: QualType,
        bits: Option<u32>,
        target: &TargetInfo,
    ) -> QualType {
        let q = self.unqualified(q);
        let Some(integer) = self.integer(q, target) else {
            return q;
        };
        if !integer.standard {
            return q;
        }
        let width = bits.unwrap_or(integer.width);
        if integer.rank <= IntRank::Int as u32 + 2
            || bits.is_some_and(|width| width < target.int_width)
        {
            return self.qual(CTypeKind::Int {
                rank: IntRank::Int,
                signed: integer.signed || width < target.int_width,
            });
        }
        q
    }

    pub fn default_promotion(&mut self, q: QualType, target: &TargetInfo) -> QualType {
        if matches!(
            self.canonical_kind(q),
            CTypeKind::Float(FloatKind::Float | FloatKind::Fp16)
        ) {
            self.qual(CTypeKind::Float(FloatKind::Double))
        } else {
            self.integer_promotion(q, None, target)
        }
    }

    pub fn unsigned_counterpart(&mut self, q: QualType) -> Result<QualType, ResolveError> {
        self.integer_signedness(q, false)
    }

    pub fn integer_signedness(
        &mut self,
        q: QualType,
        signed: bool,
    ) -> Result<QualType, ResolveError> {
        let kind = match self.canonical_kind(q) {
            CTypeKind::Char | CTypeKind::SChar | CTypeKind::UChar => {
                if signed {
                    CTypeKind::SChar
                } else {
                    CTypeKind::UChar
                }
            }
            CTypeKind::Int { rank, .. } => CTypeKind::Int {
                rank: *rank,
                signed,
            },
            CTypeKind::BitInt { width, .. } => CTypeKind::BitInt {
                width: *width,
                signed,
            },
            _ => {
                return Err(ResolveError::Unsupported(
                    "unsigned counterpart of noninteger",
                ));
            }
        };
        Ok(self.qual(kind))
    }

    pub fn fixed(&self, q: QualType) -> Option<FixedType> {
        match self.canonical_kind(q) {
            CTypeKind::FixedPoint(fixed) => Some(*fixed),
            _ => None,
        }
    }

    // N1169 6.3.1.x: a fixed-point operand absorbs an integer operand, yields to
    // a floating one, and against another fixed-point type takes the wider kind
    // and rank, signed if either is, saturating if either is
    pub fn usual_fixed_type(&mut self, a: QualType, b: QualType) -> Result<QualType, ResolveError> {
        let (left, right) = (self.fixed(a), self.fixed(b));
        if let (Some(left), Some(right)) = (left, right) {
            return Ok(self.qual(CTypeKind::FixedPoint(FixedType {
                kind: left.kind.max(right.kind),
                rank: left.rank.max(right.rank),
                signed: left.signed || right.signed,
                saturating: left.saturating || right.saturating,
            })));
        }
        let (fixed, other) = if left.is_some() { (a, b) } else { (b, a) };
        if self.is_floating(other) {
            if !matches!(self.canonical_kind(other), CTypeKind::Float(_)) {
                return Err(ResolveError::Invalid(
                    "complex or imaginary operand with a fixed-point operand",
                ));
            }
            return Ok(self.unqualified(other));
        }
        if !self.is_integer(other) {
            return Err(ResolveError::Invalid(
                "operand of a fixed-point operator must be arithmetic",
            ));
        }
        Ok(self.unqualified(fixed))
    }

    pub fn arithmetic_component(&mut self, q: QualType) -> QualType {
        match self.canonical_kind(q) {
            CTypeKind::Complex(component) => QualType::new(*component),
            CTypeKind::Imaginary(kind) => self.qual(CTypeKind::Float(*kind)),
            _ => self.unqualified(q),
        }
    }

    pub fn usual_real_type(
        &mut self,
        a: QualType,
        b: QualType,
        target: &TargetInfo,
    ) -> Result<QualType, ResolveError> {
        let a = self.arithmetic_component(a);
        let b = self.arithmetic_component(b);
        let af = match self.canonical_kind(a) {
            CTypeKind::Float(kind) => Some(*kind),
            _ => None,
        };
        let bf = match self.canonical_kind(b) {
            CTypeKind::Float(kind) => Some(*kind),
            _ => None,
        };
        if let (Some(a), Some(b)) = (af, bf)
            && a.is_decimal() != b.is_decimal()
        {
            return Err(ResolveError::Invalid(
                "mixing decimal and binary floating operands",
            ));
        }
        if af.is_some() || bf.is_some() {
            return Ok(match (af, bf) {
                (Some(af), Some(bf)) => {
                    if float_rank(af) >= float_rank(bf) {
                        a
                    } else {
                        b
                    }
                }
                (Some(_), None) => a,
                _ => b,
            });
        }
        let a = self.integer_promotion(a, None, target);
        let b = self.integer_promotion(b, None, target);
        let ai = self
            .integer(a, target)
            .ok_or(ResolveError::Unsupported("non-arithmetic operand"))?;
        let bi = self
            .integer(b, target)
            .ok_or(ResolveError::Unsupported("non-arithmetic operand"))?;
        let order = if ai.standard && bi.standard {
            ai.rank.cmp(&bi.rank)
        } else {
            (ai.width, ai.standard).cmp(&(bi.width, bi.standard))
        };
        let (higher, hi, lower, lo) = if order.is_ge() {
            (a, ai, b, bi)
        } else {
            (b, bi, a, ai)
        };
        if hi.signed == lo.signed || !hi.signed {
            Ok(higher)
        } else if order.is_eq() {
            Ok(lower)
        } else if hi.width > lo.width {
            Ok(higher)
        } else {
            self.unsigned_counterpart(higher)
        }
    }
}

fn float_rank(kind: FloatKind) -> u32 {
    match kind {
        FloatKind::Fp16 | FloatKind::Float16 => 0,
        FloatKind::Float | FloatKind::Decimal32 => 1,
        FloatKind::Double | FloatKind::Decimal64 => 2,
        FloatKind::LongDouble => 3,
        FloatKind::Float128 | FloatKind::Decimal128 => 4,
    }
}
