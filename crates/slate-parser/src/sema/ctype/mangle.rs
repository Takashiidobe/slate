use super::{CTypeKind, CTypes, Extent, FloatKind, IntRank, QualType};
use crate::ir::{PointerSpace, TypeDefinition};

pub fn itanium_function_symbol(
    types: &mut CTypes,
    definitions: &[TypeDefinition],
    name: &str,
    params: &[QualType],
    variadic: bool,
) -> Option<String> {
    let params: Vec<QualType> = params
        .iter()
        .map(|param| adjust_parameter(types, *param))
        .collect();
    let types = &*types;
    let mut mangler = Mangler::new(types, definitions, true);
    let mut out = format!("_Z{}{}", name.len(), name);
    if params.is_empty() && !variadic {
        out.push('v');
    }
    for param in &params {
        out.push_str(&mangler.ty(*param)?);
    }
    if variadic {
        out.push('z');
    }
    Some(out)
}

fn adjust_parameter(types: &mut CTypes, param: QualType) -> QualType {
    let desugared = types.desugar(param);
    let unqualified = QualType::new(desugared.ty);
    let pointee = match types.kind(desugared.ty) {
        CTypeKind::Array { element, .. } => *element,
        CTypeKind::Function { .. } => unqualified,
        _ => return unqualified,
    };
    QualType::new(types.intern(CTypeKind::Pointer(pointee, PointerSpace::Default)))
}

struct Mangler<'a> {
    types: &'a CTypes,
    definitions: &'a [TypeDefinition],
    compress: bool,
    dictionary: Vec<String>,
}

impl<'a> Mangler<'a> {
    fn new(types: &'a CTypes, definitions: &'a [TypeDefinition], compress: bool) -> Self {
        Self {
            types,
            definitions,
            compress,
            dictionary: Vec::new(),
        }
    }

    fn raw(&self, q: QualType) -> Option<String> {
        Mangler::new(self.types, self.definitions, false).ty_uncached(q)
    }

    fn ty(&mut self, q: QualType) -> Option<String> {
        let q = self.types.desugar(q);
        let kind = self.types.kind(q.ty);
        if q.quals == Default::default() && is_builtin(kind) {
            return self.ty_uncached(q);
        }
        if !self.compress {
            return self.ty_uncached(q);
        }
        let key = self.raw(q)?;
        if let Some(index) = self.dictionary.iter().position(|entry| *entry == key) {
            return Some(substitution(index));
        }
        let out = self.ty_uncached(q)?;
        if !self.dictionary.contains(&key) {
            self.dictionary.push(key);
        }
        Some(out)
    }

    fn ty_uncached(&mut self, q: QualType) -> Option<String> {
        let q = self.types.desugar(q);
        let quals = q.quals;
        if quals.is_const || quals.is_volatile || quals.is_restrict || quals.is_atomic {
            let base = self.ty(QualType::new(q.ty))?;
            let mut prefix = String::new();
            if quals.is_atomic {
                prefix.push_str("U7_Atomic");
            }
            if quals.is_restrict {
                prefix.push('r');
            }
            if quals.is_volatile {
                prefix.push('V');
            }
            if quals.is_const {
                prefix.push('K');
            }
            return Some(prefix + &base);
        }
        match self.types.kind(q.ty).clone() {
            CTypeKind::Void => Some("v".into()),
            CTypeKind::Bool => Some("b".into()),
            CTypeKind::Char => Some("c".into()),
            CTypeKind::SChar => Some("a".into()),
            CTypeKind::UChar => Some("h".into()),
            CTypeKind::Int { rank, signed } => Some(integer(rank, signed).into()),
            CTypeKind::BitInt { width, signed } => {
                Some(format!("D{}{}_", if signed { 'B' } else { 'U' }, width))
            }
            CTypeKind::Float(kind) => float(kind).map(str::to_owned),
            CTypeKind::Complex(component) => {
                Some(format!("C{}", self.ty(QualType::new(component))?))
            }
            CTypeKind::Imaginary(kind) => Some(format!("G{}", float(kind)?)),
            CTypeKind::Vector { element, lanes, .. } => {
                Some(format!("Dv{}_{}", lanes, self.ty(element)?))
            }
            CTypeKind::NullPtr => Some("Dn".into()),
            CTypeKind::Record { id, .. } | CTypeKind::Enum(id) => {
                let id = self.types.tag_class(id);
                let name = self
                    .definitions
                    .iter()
                    .find(|definition| definition.id == id)?
                    .name
                    .as_deref()?;
                Some(format!("{}{}", name.len(), name))
            }
            CTypeKind::Pointer(pointee, PointerSpace::Default) => {
                Some(format!("P{}", self.ty(pointee)?))
            }
            CTypeKind::Array { element, extent } => {
                let bound = match extent {
                    Extent::Fixed(n) => n.to_string(),
                    Extent::Incomplete | Extent::Variable(_) => String::new(),
                };
                Some(format!("A{}_{}", bound, self.ty(element)?))
            }
            CTypeKind::Function {
                ret,
                params,
                variadic,
                ..
            } => {
                let mut out = String::from("F");
                out.push_str(&self.ty(ret)?);
                if params.is_empty() && !variadic {
                    out.push('v');
                }
                for param in &params {
                    out.push_str(&self.ty(*param)?);
                }
                if variadic {
                    out.push('z');
                }
                out.push('E');
                Some(out)
            }
            _ => None,
        }
    }
}

fn is_builtin(kind: &CTypeKind) -> bool {
    matches!(
        kind,
        CTypeKind::Void
            | CTypeKind::Bool
            | CTypeKind::Char
            | CTypeKind::SChar
            | CTypeKind::UChar
            | CTypeKind::Int { .. }
            | CTypeKind::BitInt { .. }
            | CTypeKind::Float(_)
            | CTypeKind::NullPtr
    )
}

fn integer(rank: IntRank, signed: bool) -> &'static str {
    match (rank, signed) {
        (IntRank::Short, true) => "s",
        (IntRank::Short, false) => "t",
        (IntRank::Int, true) => "i",
        (IntRank::Int, false) => "j",
        (IntRank::Long, true) => "l",
        (IntRank::Long, false) => "m",
        (IntRank::LongLong, true) => "x",
        (IntRank::LongLong, false) => "y",
        (IntRank::Int128, true) => "n",
        (IntRank::Int128, false) => "o",
    }
}

fn float(kind: FloatKind) -> Option<&'static str> {
    Some(match kind {
        FloatKind::Float => "f",
        FloatKind::Double => "d",
        FloatKind::LongDouble => "e",
        FloatKind::Float128 => "g",
        FloatKind::Fp16 => "Dh",
        FloatKind::Float16 => "DF16_",
        FloatKind::BFloat16 => "DF16b",
        FloatKind::Float32 => "DF32_",
        FloatKind::Float64 => "DF64_",
        FloatKind::Float32x => "DF32x",
        FloatKind::Float64x => "DF64x",
        FloatKind::Decimal32 => "Df",
        FloatKind::Decimal64 => "Dd",
        FloatKind::Decimal128 => "De",
        FloatKind::Decimal64x => return None,
    })
}

fn substitution(index: usize) -> String {
    if index == 0 {
        return "S_".into();
    }
    let mut n = index - 1;
    let mut digits = Vec::new();
    loop {
        digits.push(b"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[n % 36]);
        n /= 36;
        if n == 0 {
            break;
        }
    }
    digits.reverse();
    format!(
        "S{}_",
        String::from_utf8(digits).expect("base-36 digits are ascii")
    )
}
