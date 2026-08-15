use crate::backend::rust_ast::{Abi, CLIB_RECORD_TYPES, CLibType, Ident, Prim, Type};
use crate::parse::ast::{NodeId, Type as CType};
use std::collections::HashMap;

pub(crate) fn resolve_aggregate<'a>(ty: &'a CType, types: &'a HashMap<NodeId, CType>) -> &'a CType {
    let id = match ty {
        CType::Struct { id, .. } | CType::Union { id, .. } | CType::Enum { id, .. } => *id,
        _ => return ty,
    };
    types.get(&id).unwrap_or(ty)
}

pub(crate) fn lower_type(ty: &CType, types: &HashMap<NodeId, CType>) -> Type {
    match ty {
        CType::Void => Type::Unit,
        CType::Bool => Type::Prim(Prim::Bool),
        CType::Char => Type::Prim(Prim::I8),
        CType::UChar => Type::Prim(Prim::U8),
        CType::Short => Type::Prim(Prim::I16),
        CType::UShort => Type::Prim(Prim::U16),
        CType::Int => Type::Prim(Prim::I32),
        CType::UInt => Type::Prim(Prim::U32),
        CType::Long => Type::Prim(Prim::I64),
        CType::ULong => Type::Prim(Prim::U64),
        CType::Float => Type::Prim(Prim::F32),
        CType::Double => Type::Prim(Prim::F64),
        CType::LDouble if crate::cir::emit::uses_f64_long_double_abi() => Type::Prim(Prim::F64),
        CType::LDouble => Type::LongDouble,
        CType::F128 => Type::Prim(Prim::F128),
        CType::FloatComplex => Type::Complex(Box::new(Type::Prim(Prim::F32))),
        CType::DoubleComplex => Type::Complex(Box::new(Type::Prim(Prim::F64))),
        CType::LDoubleComplex => Type::Complex(Box::new(lower_type(&CType::LDouble, types))),
        CType::IntComplex(base) => Type::Complex(Box::new(lower_type(base, types))),
        CType::NullPtr => Type::Ptr {
            mutable: true,
            inner: Box::new(Type::Unit),
        },
        CType::Enum { base, .. } => lower_type(base, types),
        CType::Ptr(inner) => match resolve_aggregate(inner, types) {
            CType::Void => Type::Ptr {
                mutable: true,
                inner: Box::new(Type::CLib(CLibType::VOID)),
            },
            CType::Func {
                return_ty,
                params,
                is_variadic,
            } => lower_fn_ptr(return_ty, params, *is_variadic, types),
            _ => Type::Ptr {
                mutable: true,
                inner: Box::new(lower_type(inner, types)),
            },
        },
        CType::Atomic(inner) => lower_type(inner, types),
        CType::Func {
            return_ty,
            params,
            is_variadic,
        } => lower_fn_ptr(return_ty, params, *is_variadic, types),
        CType::Struct { tag, id, .. } => Type::Custom(aggregate_name(tag.as_deref(), *id)),
        CType::Union { tag, id, .. } => Type::Custom(aggregate_name(tag.as_deref(), *id)),
        CType::Array { base, len } => Type::Array {
            elem: Box::new(lower_type(base, types)),
            len: (*len).max(0) as u64,
        },
        CType::Vector { base, len } => Type::Array {
            elem: Box::new(lower_type(base, types)),
            len: (*len).max(0) as u64,
        },
        CType::Vla { base, .. } => Type::Ptr {
            mutable: true,
            inner: Box::new(lower_type(base, types)),
        },
        CType::BitInt { width, is_signed } => bitint_type(*is_signed, *width as u32),
    }
}

fn lower_fn_ptr(
    return_ty: &CType,
    params: &[(String, CType)],
    is_variadic: bool,
    types: &HashMap<NodeId, CType>,
) -> Type {
    Type::FnPtr {
        abi: Abi::C,
        params: params
            .iter()
            .map(|(_, ty)| lower_type(ty, types))
            .chain(is_variadic.then_some(Type::Variadic))
            .collect(),
        ret: Box::new(lower_type(return_ty, types)),
    }
}

fn aggregate_name(tag: Option<&str>, id: NodeId) -> String {
    match tag {
        Some(tag) => rust_record_name(tag),
        None => format!("__anon_{id}"),
    }
}

pub(crate) fn aggregate_type_name(ty: &CType) -> String {
    match ty {
        CType::Struct { tag, id, .. } | CType::Union { tag, id, .. } => {
            aggregate_name(tag.as_deref(), *id)
        }
        _ => unreachable!("aggregate_type_name called with non-aggregate type"),
    }
}

fn rust_record_name(name: &str) -> String {
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

fn bitint_type(signed: bool, bits: u32) -> Type {
    let limbs = bits.div_ceil(64);
    Type::Generic {
        name: (if signed {
            "bitint::BInt"
        } else {
            "bitint::BUint"
        })
        .into(),
        args: vec![
            Type::Custom(bits.to_string()),
            Type::Custom(limbs.to_string()),
        ],
    }
}
