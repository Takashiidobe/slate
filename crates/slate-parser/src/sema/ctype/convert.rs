use super::{CTypeKind, CTypes, IntRank, QualType};
use crate::diagnostics::Warning;
use crate::sema::numeric::ResolveError;

#[derive(Clone, Copy, PartialEq, Eq)]
pub enum ConversionContext {
    Assign,
    Arg,
    Return,
    Cast,
}

impl ConversionContext {
    fn is_assignment(self) -> bool {
        !matches!(self, Self::Cast)
    }
}

#[derive(Clone, Copy, PartialEq, Eq)]
pub enum CastKind {
    Identity,
    RecordCopy,
    Arithmetic,
    Vector,
    EnumToInt,
    IntToEnum,
    Pointer,
    PtrToInt,
    PtrToBool,
    IntToPtr,
    NullPointer,
}

pub struct Conversion {
    pub kind: CastKind,
    pub warning: Option<(Warning, &'static str)>,
}

impl Conversion {
    fn plain(kind: CastKind) -> Self {
        Self {
            kind,
            warning: None,
        }
    }

    fn warned(kind: CastKind, warning: Warning, message: &'static str) -> Self {
        Self {
            kind,
            warning: Some((warning, message)),
        }
    }
}

impl CTypes {
    pub fn classify_conversion(
        &mut self,
        from: QualType,
        to: QualType,
        context: ConversionContext,
        is_null_pointer_constant: bool,
    ) -> Result<Conversion, ResolveError> {
        if self.is_void(to) {
            return Err(ResolveError::Invalid("conversion to void"));
        }
        if self.is_void(from) {
            return Err(ResolveError::Invalid("conversion from void"));
        }
        if self.is_function(to) || self.is_array(to) {
            return Err(ResolveError::Invalid("conversion to a function or array"));
        }
        let (from_view, to_view) = (self.unqualified_view(from), self.unqualified_view(to));
        let record = matches!(self.canonical_kind(to), CTypeKind::Record { .. });
        if self.same(from_view, to_view)
            || (record && self.compatible_unqualified(from_view, to_view))
        {
            return Ok(Conversion::plain(if record && context.is_assignment() {
                CastKind::RecordCopy
            } else {
                CastKind::Identity
            }));
        }
        if self.is_record(from) || self.is_record(to) {
            return Err(ResolveError::Invalid(
                "conversion between a struct or union and an unrelated type",
            ));
        }
        if self.is_vector(from) || self.is_vector(to) {
            return Ok(Conversion::plain(CastKind::Vector));
        }
        if (self.is_fixed_point(from) || self.is_fixed_point(to))
            && (self.is_complex_domain(from) || self.is_complex_domain(to))
        {
            return Err(ResolveError::Invalid(
                "conversion between a fixed-point type and a complex or imaginary type",
            ));
        }
        if self.enum_underlying(from).is_some() {
            return Ok(Conversion::plain(CastKind::EnumToInt));
        }
        if self.enum_underlying(to).is_some() {
            return Ok(Conversion::plain(CastKind::IntToEnum));
        }
        if self.is_pointer(to) {
            return self.convert_to_pointer(from, to, context, is_null_pointer_constant);
        }
        if self.is_pointer(from) {
            if self.is_floating(to) {
                return Err(ResolveError::Invalid(
                    "conversion between pointer and floating type",
                ));
            }
            if !self.is_arithmetic(to) {
                return Err(ResolveError::Invalid("unsupported pointer conversion"));
            }
            if matches!(self.canonical_kind(to), CTypeKind::Bool) {
                return Ok(Conversion::plain(CastKind::PtrToBool));
            }
            if context.is_assignment() {
                return Ok(Conversion::warned(
                    CastKind::PtrToInt,
                    Warning::IntConversion,
                    "incompatible pointer to integer conversion",
                ));
            }
            return Ok(Conversion::plain(CastKind::PtrToInt));
        }
        if self.is_arithmetic(from) && self.is_arithmetic(to) {
            return Ok(Conversion::plain(CastKind::Arithmetic));
        }
        Err(ResolveError::Unsupported(
            "incompatible or unsupported conversion",
        ))
    }

    fn convert_to_pointer(
        &mut self,
        from: QualType,
        to: QualType,
        context: ConversionContext,
        is_null_pointer_constant: bool,
    ) -> Result<Conversion, ResolveError> {
        if is_null_pointer_constant {
            return Ok(Conversion::plain(CastKind::NullPointer));
        }
        if !self.is_pointer(from) {
            if self.is_floating(from) {
                return Err(ResolveError::Invalid(
                    "conversion between pointer and floating type",
                ));
            }
            if !self.is_arithmetic(from) {
                return Err(ResolveError::Invalid("unsupported conversion to pointer"));
            }
            if context.is_assignment() {
                return Ok(Conversion::warned(
                    CastKind::IntToPtr,
                    Warning::IntConversion,
                    "incompatible integer to pointer conversion",
                ));
            }
            return Ok(Conversion::plain(CastKind::IntToPtr));
        }
        if !context.is_assignment() {
            return Ok(Conversion::plain(CastKind::Pointer));
        }
        let from_pointee = self
            .pointee(from)
            .ok_or(ResolveError::Unsupported("pointer without a pointee"))?;
        let to_pointee = self
            .pointee(to)
            .ok_or(ResolveError::Unsupported("pointer without a pointee"))?;
        let dropped = self
            .quals(from_pointee)
            .without(self.quals(to_pointee))
            .without(super::Qualifiers {
                is_restrict: true,
                ..super::Qualifiers::NONE
            });
        let unqualified_from = self.unqualified(from_pointee);
        let unqualified_to = self.unqualified(to_pointee);
        let related = self.is_void(from_pointee)
            || self.is_void(to_pointee)
            || self.compatible_unqualified(unqualified_from, unqualified_to);
        if related && dropped.is_empty() {
            return Ok(Conversion::plain(CastKind::Pointer));
        }
        if related {
            if dropped.is_atomic {
                return Ok(Conversion::warned(
                    CastKind::Pointer,
                    Warning::IncompatiblePointerTypes,
                    "pointer conversion drops _Atomic from the pointee",
                ));
            }
            return Ok(Conversion::warned(
                CastKind::Pointer,
                Warning::IncompatiblePointerTypesDiscardsQualifiers,
                "pointer conversion discards qualifiers",
            ));
        }
        if self.differ_only_in_sign(unqualified_from, unqualified_to) {
            return Ok(Conversion::warned(
                CastKind::Pointer,
                Warning::PointerSign,
                "conversion between pointers to integer types with different sign",
            ));
        }
        if self.compatible_ignoring_qualifiers(unqualified_from, unqualified_to) {
            return Ok(Conversion::warned(
                CastKind::Pointer,
                Warning::IncompatiblePointerTypesDiscardsQualifiers,
                "pointer conversion discards qualifiers in nested pointer types",
            ));
        }
        Ok(Conversion::warned(
            CastKind::Pointer,
            Warning::IncompatiblePointerTypes,
            "incompatible pointer types",
        ))
    }

    fn compatible_ignoring_qualifiers(&self, a: QualType, b: QualType) -> bool {
        match (self.pointee(a), self.pointee(b)) {
            (Some(a), Some(b)) => {
                let (a, b) = (a.local_unqualified(), b.local_unqualified());
                self.compatible_ignoring_qualifiers(a, b)
            }
            _ => self.compatible_unqualified(a, b),
        }
    }

    fn differ_only_in_sign(&self, a: QualType, b: QualType) -> bool {
        match (self.integer_shape(a), self.integer_shape(b)) {
            (Some(a_shape), Some(b_shape)) => a_shape == b_shape && !self.same(a, b),
            _ => false,
        }
    }

    fn integer_shape(&self, q: QualType) -> Option<IntegerShape> {
        match self.canonical_kind(q) {
            CTypeKind::Char | CTypeKind::SChar | CTypeKind::UChar => Some(IntegerShape::Char),
            CTypeKind::Int { rank, .. } => Some(IntegerShape::Rank(*rank)),
            CTypeKind::BitInt { width, .. } => Some(IntegerShape::BitInt(*width)),
            _ => None,
        }
    }

    pub fn unqualified_view(&self, q: QualType) -> QualType {
        self.canonical(q).local_unqualified()
    }

    pub fn is_record(&self, q: QualType) -> bool {
        matches!(self.canonical_kind(q), CTypeKind::Record { .. })
    }

    pub fn is_vector(&self, q: QualType) -> bool {
        matches!(self.canonical_kind(q), CTypeKind::Vector { .. })
    }

    pub fn is_floating(&self, q: QualType) -> bool {
        matches!(
            self.canonical_kind(q),
            CTypeKind::Float(_) | CTypeKind::Complex(_) | CTypeKind::Imaginary(_)
        )
    }

    pub fn is_integer(&self, q: QualType) -> bool {
        matches!(
            self.canonical_kind(q),
            CTypeKind::Bool
                | CTypeKind::Char
                | CTypeKind::SChar
                | CTypeKind::UChar
                | CTypeKind::Int { .. }
                | CTypeKind::BitInt { .. }
                | CTypeKind::Enum(_)
        )
    }

    pub fn is_complex_domain(&self, q: QualType) -> bool {
        matches!(
            self.canonical_kind(q),
            CTypeKind::Complex(_) | CTypeKind::Imaginary(_)
        )
    }

    pub fn is_fixed_point(&self, q: QualType) -> bool {
        matches!(self.canonical_kind(q), CTypeKind::FixedPoint(_))
    }

    pub fn is_arithmetic(&self, q: QualType) -> bool {
        self.is_integer(q) || self.is_floating(q) || self.is_fixed_point(q)
    }

    pub fn is_scalar(&self, q: QualType) -> bool {
        self.is_arithmetic(q) || self.is_pointer(q)
    }
}

#[derive(PartialEq, Eq)]
enum IntegerShape {
    Char,
    Rank(IntRank),
    BitInt(u32),
}
