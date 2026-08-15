use crate::backend::rust_ast::{Abi, CLIB_RECORD_TYPES, CLibType, Ident, Prim, RustValue, Type};
use std::collections::HashMap;

#[derive(Debug, Clone, PartialEq)]
pub(crate) enum CType {
    Void,
    Bool,
    Char,
    UChar,
    Short,
    UShort,
    Int,
    UInt,
    Long,
    ULong,
    LongLong,
    ULongLong,
    Int128,
    UInt128,
    Float,
    Double,
    LDouble,
    Ptr(Box<CType>),
    Array {
        base: Box<CType>,
        len: Option<i64>,
    },
    Func {
        return_ty: Box<CType>,
        params: Vec<CType>,
        is_variadic: bool,
    },
    Record {
        tag: String,
        is_union: bool,
    },
    Enum {
        tag: String,
    },
    Unknown(String),
}

#[derive(Debug, Clone)]
pub(crate) struct RecordInfo {
    pub(crate) is_union: bool,
    pub(crate) fields: Vec<(String, CType)>,
    pub(crate) packed: bool,
    pub(crate) align_override: Option<u32>,
}

pub(crate) type RecordRegistry = HashMap<String, RecordInfo>;

const QUALIFIER_WORDS: &[&str] = &[
    "const",
    "volatile",
    "restrict",
    "_Atomic",
    "_Nonnull",
    "_Nullable",
    "_Null_unspecified",
];

fn strip_qualifiers(s: &str) -> &str {
    let mut s = s.trim();
    loop {
        let before = s;
        for word in QUALIFIER_WORDS {
            if let Some(rest) = s.strip_prefix(word)
                && rest.starts_with(char::is_whitespace)
            {
                s = rest.trim_start();
            }
            if let Some(rest) = s.strip_suffix(word)
                && (rest.is_empty() || rest.ends_with(char::is_whitespace) || rest.ends_with('*'))
            {
                s = rest.trim_end();
            }
        }
        if s == before {
            break;
        }
    }
    s
}

impl From<&str> for CType {
    fn from(s: &str) -> CType {
        match s {
            "void" => CType::Void,
            "_Bool" | "bool" => CType::Bool,
            "char" | "signed char" => CType::Char,
            "unsigned char" => CType::UChar,
            "short" | "short int" | "signed short" | "signed short int" => CType::Short,
            "unsigned short" | "unsigned short int" => CType::UShort,
            "int" | "signed" | "signed int" => CType::Int,
            "unsigned int" | "unsigned" => CType::UInt,
            "long" | "long int" | "signed long" | "signed long int" => CType::Long,
            "unsigned long" | "unsigned long int" => CType::ULong,
            "long long" | "long long int" | "signed long long" | "signed long long int" => {
                CType::LongLong
            }
            "unsigned long long" | "unsigned long long int" => CType::ULongLong,
            "__int128" => CType::Int128,
            "unsigned __int128" => CType::UInt128,
            "float" => CType::Float,
            "double" => CType::Double,
            "long double" => CType::LDouble,
            other => CType::Unknown(other.to_string()),
        }
    }
}

fn trailing_array_bracket(s: &str) -> Option<usize> {
    if !s.ends_with(']') {
        return None;
    }
    s.find('[')
}

fn parse_array(base: &str, dims: &str) -> CType {
    let dims = dims.trim();
    let first_close = dims.find(']').unwrap_or(dims.len());
    let len_str = &dims[1..first_close];
    let rest = &dims[(first_close + 1).min(dims.len())..];
    let len = len_str.trim().parse::<i64>().ok();
    let elem = if rest.is_empty() {
        CType::parse(strip_qualifiers(base))
    } else {
        parse_array(base, rest)
    };
    CType::Array {
        base: Box::new(elem),
        len,
    }
}

fn trailing_paren_params(s: &str) -> Option<usize> {
    if !s.ends_with(')') {
        return None;
    }
    let bytes = s.as_bytes();
    let mut depth = 0i32;
    let mut i = bytes.len();
    while i > 0 {
        i -= 1;
        match bytes[i] {
            b')' => depth += 1,
            b'(' => {
                depth -= 1;
                if depth == 0 {
                    return Some(i);
                }
            }
            _ => {}
        }
    }
    None
}

fn split_top_level_commas(s: &str) -> Vec<&str> {
    let mut parts = Vec::new();
    let mut depth = 0i32;
    let mut start = 0usize;
    for (i, c) in s.char_indices() {
        match c {
            '(' | '[' => depth += 1,
            ')' | ']' => depth -= 1,
            ',' if depth == 0 => {
                parts.push(s[start..i].trim());
                start = i + 1;
            }
            _ => {}
        }
    }
    parts.push(s[start..].trim());
    parts
}

fn parse_param_list(s: &str) -> (Vec<CType>, bool) {
    let s = s.trim();
    if s.is_empty() || s == "void" {
        return (Vec::new(), false);
    }
    let mut is_variadic = false;
    let mut parts = split_top_level_commas(s);
    if parts.last() == Some(&"...") {
        is_variadic = true;
        parts.pop();
    }
    let params = parts
        .into_iter()
        .filter(|p| !p.is_empty())
        .map(|p| CType::parse(strip_qualifiers(p)))
        .collect();
    (params, is_variadic)
}

impl CType {
    pub(crate) fn parse(spelling: &str) -> CType {
        let s = strip_qualifiers(spelling.trim());

        if let Some(inner) = s.strip_suffix('*') {
            return CType::Ptr(Box::new(CType::parse(strip_qualifiers(inner))));
        }

        if let Some(open) = trailing_array_bracket(s) {
            let base = &s[..open];
            let dims = &s[open..];
            return parse_array(base, dims);
        }

        if let Some(params_start) = trailing_paren_params(s) {
            let ret = &s[..params_start];
            let params_str = &s[params_start + 1..s.len() - 1];
            let (params, is_variadic) = parse_param_list(params_str);
            return CType::Func {
                return_ty: Box::new(CType::parse(strip_qualifiers(ret))),
                params,
                is_variadic,
            };
        }

        if let Some(tag) = s.strip_prefix("struct ") {
            return CType::Record {
                tag: format!("struct {}", tag.trim()),
                is_union: false,
            };
        }
        if let Some(tag) = s.strip_prefix("union ") {
            return CType::Record {
                tag: format!("union {}", tag.trim()),
                is_union: true,
            };
        }
        if let Some(tag) = s.strip_prefix("enum ") {
            return CType::Enum {
                tag: format!("enum {}", tag.trim()),
            };
        }

        CType::from(s)
    }

    pub(crate) fn is_flonum(&self) -> bool {
        matches!(self, CType::Float | CType::Double | CType::LDouble)
    }

    pub(crate) fn is_unsigned(&self) -> bool {
        matches!(
            self,
            CType::UChar
                | CType::UShort
                | CType::UInt
                | CType::ULong
                | CType::ULongLong
                | CType::UInt128
        )
    }

    pub(crate) fn is_pointerish(&self) -> bool {
        matches!(self, CType::Ptr(_) | CType::Array { .. })
    }

    pub(crate) fn size(&self) -> i64 {
        match self {
            CType::Void => 1,
            CType::Bool | CType::Char | CType::UChar => 1,
            CType::Short | CType::UShort => 2,
            CType::Int | CType::UInt => 4,
            CType::Long | CType::ULong | CType::LongLong | CType::ULongLong => 8,
            CType::Int128 | CType::UInt128 => 16,
            CType::Float => 4,
            CType::Double => 8,
            CType::LDouble => 16,
            CType::Ptr(_) => 8,
            CType::Enum { .. } => 4,
            _ => 8,
        }
    }

    pub(crate) fn int_prim(&self) -> Prim {
        match self.size() {
            1 if self.is_unsigned() => Prim::U8,
            1 => Prim::I8,
            2 if self.is_unsigned() => Prim::U16,
            2 => Prim::I16,
            4 if self.is_unsigned() => Prim::U32,
            4 => Prim::I32,
            16 if self.is_unsigned() => Prim::U128,
            16 => Prim::I128,
            _ if self.is_unsigned() => Prim::U64,
            _ => Prim::I64,
        }
    }

    pub(crate) fn int_value(&self, value: i128) -> RustValue {
        if self.is_unsigned() {
            RustValue::TypedUInt(value as u128, self.int_prim())
        } else {
            RustValue::TypedInt(value, self.int_prim())
        }
    }
}

impl CType {
    pub(crate) fn lower(&self, records: &RecordRegistry) -> Type {
        match self {
            CType::Void => Type::Unit,
            CType::Bool => Type::Prim(Prim::Bool),
            CType::Char => Type::Prim(Prim::I8),
            CType::UChar => Type::Prim(Prim::U8),
            CType::Short => Type::Prim(Prim::I16),
            CType::UShort => Type::Prim(Prim::U16),
            CType::Int => Type::Prim(Prim::I32),
            CType::UInt => Type::Prim(Prim::U32),
            CType::Long | CType::LongLong => Type::Prim(Prim::I64),
            CType::ULong | CType::ULongLong => Type::Prim(Prim::U64),
            CType::Int128 => Type::Prim(Prim::I128),
            CType::UInt128 => Type::Prim(Prim::U128),
            CType::Float => Type::Prim(Prim::F32),
            CType::Double => Type::Prim(Prim::F64),
            CType::LDouble if crate::cir::emit::uses_f64_long_double_abi() => Type::Prim(Prim::F64),
            CType::LDouble => Type::LongDouble,
            CType::Enum { .. } => Type::Prim(Prim::I32),
            CType::Ptr(inner) => match inner.as_ref() {
                CType::Void => Type::Ptr {
                    mutable: true,
                    inner: Box::new(Type::CLib(CLibType::VOID)),
                },
                CType::Func {
                    return_ty,
                    params,
                    is_variadic,
                } => Self::lower_fn_ptr(return_ty, params, *is_variadic, records),
                _ => Type::Ptr {
                    mutable: true,
                    inner: Box::new(inner.lower(records)),
                },
            },
            CType::Func {
                return_ty,
                params,
                is_variadic,
            } => Self::lower_fn_ptr(return_ty, params, *is_variadic, records),
            CType::Record { tag, .. } => match clib_record_type(bare_tag_name(tag)) {
                Some(clib) => Type::CLib(clib),
                None => Type::Custom(rust_record_name(tag)),
            },
            CType::Array { base, len } => Type::Array {
                elem: Box::new(base.lower(records)),
                len: len.unwrap_or(0).max(0) as u64,
            },
            CType::Unknown(_) => Type::Prim(Prim::I32),
        }
    }

    fn lower_fn_ptr(
        return_ty: &CType,
        params: &[CType],
        is_variadic: bool,
        records: &RecordRegistry,
    ) -> Type {
        Type::FnPtr {
            abi: Abi::C,
            params: params
                .iter()
                .map(|ty| ty.lower(records))
                .chain(is_variadic.then_some(Type::Variadic))
                .collect(),
            ret: Box::new(return_ty.lower(records)),
        }
    }
}

fn bare_tag_name(tag: &str) -> &str {
    tag.strip_prefix("struct ")
        .or_else(|| tag.strip_prefix("union "))
        .unwrap_or(tag)
}

pub(crate) fn rust_record_name(tag: &str) -> String {
    let name = bare_tag_name(tag);
    clib_record_type(name)
        .map(|ty| ty.c_name().to_string())
        .unwrap_or_else(|| sanitize_ident(name).into_string())
}

fn clib_record_type(name: &str) -> Option<CLibType> {
    let name = match name {
        "__mbstate_t" => "mbstate_t",
        _ => name,
    };
    CLIB_RECORD_TYPES
        .iter()
        .copied()
        .find(|ty| ty.c_name() == name)
}

fn sanitize_ident(s: &str) -> Ident {
    let mut out = String::new();
    for (i, c) in s.chars().enumerate() {
        if (i == 0 && (c.is_ascii_alphabetic() || c == '_'))
            || (i > 0 && (c.is_ascii_alphanumeric() || c == '_'))
        {
            out.push(c);
        } else {
            out.push('_');
        }
    }
    if out.is_empty() {
        return Ident::from("_tmp");
    }
    if matches!(out.as_str(), "crate" | "self" | "Self" | "super") {
        out.push('_');
    }
    Ident::from(out)
}
