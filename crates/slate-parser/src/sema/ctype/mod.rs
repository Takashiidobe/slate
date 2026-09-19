pub(super) mod arith;
pub(super) mod compat;
pub(super) mod convert;
mod layout;
mod render;

use std::collections::HashMap;

use crate::ir::{BindingId, TypeId};

pub use render::CTypeMetadata;

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub struct CTypeId(u32);

#[derive(Debug, Clone, Copy, Default, PartialEq, Eq, Hash)]
pub struct Qualifiers {
    pub is_const: bool,
    pub is_volatile: bool,
    pub is_restrict: bool,
    pub is_atomic: bool,
}

impl Qualifiers {
    pub const NONE: Self = Self {
        is_const: false,
        is_volatile: false,
        is_restrict: false,
        is_atomic: false,
    };

    pub const CONST: Self = Self {
        is_const: true,
        ..Self::NONE
    };

    pub const ATOMIC: Self = Self {
        is_atomic: true,
        ..Self::NONE
    };

    pub fn union(self, other: Self) -> Self {
        Self {
            is_const: self.is_const || other.is_const,
            is_volatile: self.is_volatile || other.is_volatile,
            is_restrict: self.is_restrict || other.is_restrict,
            is_atomic: self.is_atomic || other.is_atomic,
        }
    }

    pub fn without(self, other: Self) -> Self {
        Self {
            is_const: self.is_const && !other.is_const,
            is_volatile: self.is_volatile && !other.is_volatile,
            is_restrict: self.is_restrict && !other.is_restrict,
            is_atomic: self.is_atomic && !other.is_atomic,
        }
    }

    pub fn includes(self, other: Self) -> bool {
        other.without(self).is_empty()
    }

    pub fn is_empty(self) -> bool {
        self == Self::NONE
    }
}

impl From<crate::ast::Qualifiers> for Qualifiers {
    fn from(qualifiers: crate::ast::Qualifiers) -> Self {
        Self {
            is_const: qualifiers.is_const,
            is_volatile: qualifiers.is_volatile,
            is_restrict: qualifiers.is_restrict,
            is_atomic: qualifiers.is_atomic,
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub struct QualType {
    pub ty: CTypeId,
    pub quals: Qualifiers,
}

impl QualType {
    pub fn new(ty: CTypeId) -> Self {
        Self {
            ty,
            quals: Qualifiers::NONE,
        }
    }

    pub fn with(self, quals: Qualifiers) -> Self {
        Self {
            ty: self.ty,
            quals: self.quals.union(quals),
        }
    }

    pub fn local_unqualified(self) -> Self {
        Self::new(self.ty)
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum IntRank {
    Short,
    Int,
    Long,
    LongLong,
    Int128,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub enum FloatKind {
    Float16,
    Fp16,
    Float,
    Double,
    LongDouble,
    Float128,
    Decimal32,
    Decimal64,
    Decimal128,
}

impl FloatKind {
    pub fn is_decimal(self) -> bool {
        matches!(self, Self::Decimal32 | Self::Decimal64 | Self::Decimal128)
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub enum Extent {
    Incomplete,
    Fixed(u64),
    Variable(Option<BindingId>),
}

#[derive(Debug, Clone, PartialEq, Eq, Hash)]
pub enum CTypeKind {
    Void,
    Bool,
    Char,
    SChar,
    UChar,
    Int {
        rank: IntRank,
        signed: bool,
    },
    BitInt {
        width: u32,
        signed: bool,
    },
    Float(FloatKind),
    Complex(CTypeId),
    Imaginary(FloatKind),
    Vector {
        element: QualType,
        lanes: u32,
        bytes: u64,
    },
    VaList,
    Record {
        id: TypeId,
        union: bool,
    },
    Enum(TypeId),
    Pointer(QualType),
    Array {
        element: QualType,
        extent: Extent,
    },
    Function {
        ret: QualType,
        params: Vec<QualType>,
        variadic: bool,
        prototyped: bool,
    },
    Typedef {
        name: String,
        underlying: QualType,
    },
    TypeOf {
        spelling: String,
        underlying: QualType,
    },
    AtomicSpecifier(QualType),
}

impl CTypeKind {
    pub fn is_sugar(&self) -> bool {
        matches!(
            self,
            Self::Typedef { .. } | Self::TypeOf { .. } | Self::AtomicSpecifier(_)
        )
    }
}

struct Entry {
    kind: CTypeKind,
    canonical: QualType,
}

#[derive(Default)]
pub struct CTypes {
    entries: Vec<Entry>,
    ids: HashMap<CTypeKind, CTypeId>,
    enum_underlying: HashMap<TypeId, QualType>,
}

impl CTypes {
    pub fn set_enum_underlying(&mut self, id: TypeId, underlying: QualType) {
        self.enum_underlying.insert(id, underlying);
    }

    pub fn enum_underlying(&self, q: QualType) -> Option<QualType> {
        match self.canonical_kind(q) {
            CTypeKind::Enum(id) => self.enum_underlying.get(id).copied(),
            _ => None,
        }
    }

    pub fn kind(&self, id: CTypeId) -> &CTypeKind {
        &self.entries[id.0 as usize].kind
    }

    pub fn intern(&mut self, kind: CTypeKind) -> CTypeId {
        if let Some(id) = self.ids.get(&kind) {
            return *id;
        }
        let canonical = self.canonical_form(&kind);
        let id = CTypeId(self.entries.len() as u32);
        self.entries.push(Entry {
            kind: kind.clone(),
            canonical: canonical.unwrap_or(QualType::new(id)),
        });
        self.ids.insert(kind, id);
        id
    }

    pub fn qual(&mut self, kind: CTypeKind) -> QualType {
        QualType::new(self.intern(kind))
    }

    fn canonical_form(&mut self, kind: &CTypeKind) -> Option<QualType> {
        match kind {
            CTypeKind::Typedef { underlying, .. } | CTypeKind::TypeOf { underlying, .. } => {
                Some(self.canonical(*underlying))
            }
            CTypeKind::AtomicSpecifier(inner) => {
                Some(self.canonical(*inner).with(Qualifiers::ATOMIC))
            }
            CTypeKind::Pointer(pointee) => {
                let canonical = self.canonical(*pointee);
                (canonical != *pointee).then(|| self.qual(CTypeKind::Pointer(canonical)))
            }
            CTypeKind::Array { element, extent } => {
                let canonical = self.canonical(*element);
                let element_type = canonical.local_unqualified();
                if element_type == *element {
                    return None;
                }
                let array = self.intern(CTypeKind::Array {
                    element: element_type,
                    extent: *extent,
                });
                Some(QualType {
                    ty: array,
                    quals: canonical.quals,
                })
            }
            CTypeKind::Vector {
                element,
                lanes,
                bytes,
            } => {
                let canonical = self.canonical(*element).local_unqualified();
                (canonical != *element).then(|| {
                    self.qual(CTypeKind::Vector {
                        element: canonical,
                        lanes: *lanes,
                        bytes: *bytes,
                    })
                })
            }
            CTypeKind::Function {
                ret,
                params,
                variadic,
                prototyped,
            } => {
                let canonical_ret = self.canonical(*ret);
                let canonical_params = params
                    .iter()
                    .map(|param| {
                        let adjusted = self.adjust_parameter(*param);
                        self.canonical(adjusted).local_unqualified()
                    })
                    .collect::<Vec<_>>();
                (canonical_ret != *ret || canonical_params != *params).then(|| {
                    self.qual(CTypeKind::Function {
                        ret: canonical_ret,
                        params: canonical_params,
                        variadic: *variadic,
                        prototyped: *prototyped,
                    })
                })
            }
            _ => None,
        }
    }

    pub fn canonical(&self, q: QualType) -> QualType {
        self.entries[q.ty.0 as usize].canonical.with(q.quals)
    }

    pub fn canonical_kind(&self, q: QualType) -> &CTypeKind {
        self.kind(self.canonical(q).ty)
    }

    pub fn quals(&self, q: QualType) -> Qualifiers {
        self.canonical(q).quals
    }

    pub fn same(&self, a: QualType, b: QualType) -> bool {
        self.canonical(a) == self.canonical(b)
    }

    pub fn desugar(&self, mut q: QualType) -> QualType {
        loop {
            match self.kind(q.ty) {
                CTypeKind::Typedef { underlying, .. } | CTypeKind::TypeOf { underlying, .. } => {
                    q = underlying.with(q.quals);
                }
                CTypeKind::AtomicSpecifier(inner) => {
                    q = inner.with(q.quals).with(Qualifiers::ATOMIC);
                }
                _ => return q,
            }
        }
    }

    pub fn unqualified(&mut self, q: QualType) -> QualType {
        let bare = q.local_unqualified();
        if self.quals(bare).is_empty() {
            return bare;
        }
        match self.kind(q.ty).clone() {
            CTypeKind::Typedef { underlying, .. } | CTypeKind::TypeOf { underlying, .. } => {
                self.unqualified(underlying)
            }
            CTypeKind::AtomicSpecifier(inner) => self.unqualified(inner),
            CTypeKind::Array { element, extent } => {
                let element = self.unqualified(element);
                self.qual(CTypeKind::Array { element, extent })
            }
            _ => bare,
        }
    }

    pub fn pointer(&mut self, pointee: QualType) -> QualType {
        self.qual(CTypeKind::Pointer(pointee))
    }

    pub fn pointee(&self, q: QualType) -> Option<QualType> {
        match self.kind(self.desugar(q).ty) {
            CTypeKind::Pointer(pointee) => Some(*pointee),
            _ => None,
        }
    }

    pub fn element(&self, q: QualType) -> Option<(QualType, Extent)> {
        let q = self.desugar(q);
        match self.kind(q.ty) {
            CTypeKind::Array { element, extent } => Some((element.with(q.quals), *extent)),
            _ => None,
        }
    }

    pub fn function_parts(&self, q: QualType) -> Option<(QualType, &[QualType], bool, bool)> {
        match self.kind(self.desugar(q).ty) {
            CTypeKind::Function {
                ret,
                params,
                variadic,
                prototyped,
            } => Some((*ret, params, *variadic, *prototyped)),
            _ => None,
        }
    }

    pub fn is_void(&self, q: QualType) -> bool {
        matches!(self.canonical_kind(q), CTypeKind::Void)
    }

    pub fn is_array(&self, q: QualType) -> bool {
        matches!(self.canonical_kind(q), CTypeKind::Array { .. })
    }

    pub fn is_function(&self, q: QualType) -> bool {
        matches!(self.canonical_kind(q), CTypeKind::Function { .. })
    }

    pub fn is_pointer(&self, q: QualType) -> bool {
        matches!(self.canonical_kind(q), CTypeKind::Pointer(_))
    }

    pub fn adjust_parameter(&mut self, q: QualType) -> QualType {
        if let Some((element, _)) = self.element(q) {
            return self.pointer(element);
        }
        if self.is_function(q) {
            return self.pointer(q.local_unqualified());
        }
        q
    }

    pub fn lvalue_conversion(&mut self, q: QualType) -> QualType {
        if let Some((element, _)) = self.element(q) {
            return self.pointer(element);
        }
        if self.is_function(q) {
            return self.pointer(q.local_unqualified());
        }
        self.unqualified(q)
    }
}
