use super::*;
use clang_ir::enums::RecordMemberKind as CirRecordMemberKind;
pub(super) fn rust_type(cir_ty: &CirType) -> Type {
    rust_type_with_aliases(cir_ty, &BTreeMap::new(), false)
}

// True if the region contains a `cir.continue` that targets the enclosing loop,
// i.e. one not swallowed by a nested loop. `cir.if`/`cir.scope`/`cir.switch`
// bodies are transparent, so we recurse through them.
pub(super) fn named_scalar_type(name: &str) -> Option<Type> {
    match name {
        "s32i" => return Some(Type::Prim(Prim::I32)),
        "u32i" => return Some(Type::Prim(Prim::U32)),
        "s16i" => return Some(Type::Prim(Prim::I16)),
        "u16i" => return Some(Type::Prim(Prim::U16)),
        "s8i" => return Some(Type::Prim(Prim::I8)),
        "u8i" => return Some(Type::Prim(Prim::U8)),
        "s64i" => return Some(Type::Prim(Prim::I64)),
        "u64i" => return Some(Type::Prim(Prim::U64)),
        "s128i" | "s128i_bitint" => return Some(Type::Prim(Prim::I128)),
        "u128i" | "u128i_bitint" => return Some(Type::Prim(Prim::U128)),
        _ => {}
    }
    let (signed, rest) = match name.as_bytes().first()? {
        b's' => (true, &name[1..]),
        b'u' => (false, &name[1..]),
        _ => return None,
    };
    let rest = rest.strip_suffix("_bitint").unwrap_or(rest);
    let bits: u32 = rest.strip_suffix('i')?.parse().ok()?;
    Some(bitint_type(signed, bits))
}

pub(super) fn rust_type_with_aliases(
    cir_ty: &CirType,
    aliases: &BTreeMap<String, CirType>,
    va_list_boxed: bool,
) -> Type {
    if let Some(va_list_ty) = va_list_shaped_type(cir_ty, aliases, va_list_boxed) {
        return va_list_ty;
    }
    match cir_ty {
        CirType::Named(name) => {
            if let Some(ty) = named_scalar_type(name) {
                return ty;
            }
            let alias_name = name.trim_start_matches('!');
            match aliases.get(name).or_else(|| aliases.get(alias_name)) {
                Some(
                    CirType::Struct {
                        name: record_name, ..
                    }
                    | CirType::Union {
                        name: record_name, ..
                    },
                ) => record_struct_type(record_name.as_deref(), name.strip_prefix("rec_")),
                Some(expanded) => rust_type_with_aliases(expanded, aliases, va_list_boxed),
                None if alias_name.starts_with("rec_") => {
                    record_struct_type(None, alias_name.strip_prefix("rec_"))
                }
                None => Type::Prim(Prim::I32),
            }
        }
        CirType::Bool => Type::Prim(Prim::Bool),
        CirType::Void => Type::CLib(CLibType::VOID),
        CirType::Int {
            is_signed, width, ..
        } => scalar_int_type(*is_signed, *width),
        CirType::Fp16 => Type::Prim(Prim::F16),
        CirType::Single => Type::Prim(Prim::F32),
        CirType::Double => Type::Prim(Prim::F64),
        CirType::Fp128 => Type::Prim(Prim::F128),
        CirType::Fp80 | CirType::LongDouble { .. } => {
            if is_quad_long_double(cir_ty) {
                Type::Prim(Prim::F128)
            } else if crate::cir::emit::uses_f64_long_double_abi() {
                Type::Prim(Prim::F64)
            } else {
                Type::LongDouble
            }
        }
        CirType::Complex { element_type } => Type::Complex(Box::new(rust_type_with_aliases(
            element_type,
            aliases,
            va_list_boxed,
        ))),
        CirType::Vector {
            element_type, size, ..
        } => Type::Array {
            elem: Box::new(rust_type_with_aliases(element_type, aliases, va_list_boxed)),
            len: *size,
        },
        CirType::Pointer { pointee, .. } => {
            if let Some(fn_ty) = cir_fn_type_to_type(pointee, aliases, va_list_boxed) {
                fn_ty
            } else {
                Type::Ptr {
                    mutable: true,
                    inner: Box::new(rust_type_with_aliases(pointee, aliases, va_list_boxed)),
                }
            }
        }
        CirType::Array { element_type, size } => Type::Array {
            elem: Box::new(rust_type_with_aliases(element_type, aliases, va_list_boxed)),
            len: *size,
        },
        CirType::Struct { name, .. } => record_struct_type(name.as_deref(), None),
        CirType::Union { name, .. } => record_struct_type(name.as_deref(), None),
        CirType::Func { .. }
        | CirType::FunctionType { .. }
        | CirType::Integer(_)
        | CirType::Index
        | CirType::Dialect { .. }
        | CirType::Bf16
        | CirType::CatchToken
        | CirType::CleanupToken
        | CirType::DataMember { .. }
        | CirType::EhToken
        | CirType::Method { .. }
        | CirType::VPtr => Type::Prim(Prim::I32),
    }
}

fn record_struct_type(name: Option<&str>, alias_key: Option<&str>) -> Type {
    let Some(name) = name.or(alias_key) else {
        return Type::Prim(Prim::I32);
    };
    clib_record_type(name)
        .map(Type::CLib)
        .unwrap_or_else(|| Type::Custom(rust_record_name(name)))
}

fn scalar_int_type(signed: bool, width: u32) -> Type {
    match (signed, width) {
        (true, 8) => Type::Prim(Prim::I8),
        (false, 8) => Type::Prim(Prim::U8),
        (true, 16) => Type::Prim(Prim::I16),
        (false, 16) => Type::Prim(Prim::U16),
        (true, 32) => Type::Prim(Prim::I32),
        (false, 32) => Type::Prim(Prim::U32),
        (true, 64) => Type::Prim(Prim::I64),
        (false, 64) => Type::Prim(Prim::U64),
        (true, 128) => Type::Prim(Prim::I128),
        (false, 128) => Type::Prim(Prim::U128),
        _ => bitint_type(signed, width),
    }
}

pub(super) fn bitint_type(signed: bool, bits: u32) -> Type {
    let limbs = bits.div_ceil(64);
    let bytes = bitint_storage_bytes(bits);
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
            Type::Custom(bytes.to_string()),
        ],
    }
}

fn bitint_storage_bytes(bits: u32) -> u32 {
    match bits {
        0..=8 => 1,
        9..=16 => 2,
        17..=32 => 4,
        33..=64 => 8,
        _ => bits.div_ceil(64) * 8,
    }
}

pub(super) fn bitint_generic_parts(ty: &Type) -> Option<(&str, &str, &str, &str)> {
    let Type::Generic { name, args } = ty else {
        return None;
    };
    if name != "bitint::BInt" && name != "bitint::BUint" {
        return None;
    }
    let [Type::Custom(bits), Type::Custom(limbs), Type::Custom(bytes)] = args.as_slice() else {
        return None;
    };
    Some((name.as_str(), bits.as_str(), limbs.as_str(), bytes.as_str()))
}

// `bitint::{BInt,BUint}` are ordinary structs, not native Rust integer
// types, so a bare numeric literal can't coerce into them the way it
// coerces into e.g. `u32`; every construction goes through `from_decimal_str`,
// which (unlike `from_i128`) has no magnitude ceiling.
pub(super) fn bitint_from_decimal_str_expr(ty: &Type, digits: &str) -> Option<Expr> {
    let (name, bits, limbs, bytes) = bitint_generic_parts(ty)?;
    Some(Expr::Call {
        binding: crate::function_identity::CallBinding::Generated,
        func: Box::new(Expr::Var(
            format!("{name}::<{bits}, {limbs}, {bytes}>::from_decimal_str").into(),
        )),
        args: vec![Expr::Str(digits.to_string())],
    })
}

pub(super) fn bitint_from_int_expr(ty: &Type, value: Expr, signed: bool) -> Option<Expr> {
    let (name, bits, limbs, bytes) = bitint_generic_parts(ty)?;
    let (method, wide_ty) = if signed {
        ("from_i128", Type::Prim(Prim::I128))
    } else {
        ("from_u128", Type::Prim(Prim::U128))
    };
    Some(Expr::Call {
        binding: crate::function_identity::CallBinding::Generated,
        func: Box::new(Expr::Var(
            format!("{name}::<{bits}, {limbs}, {bytes}>::{method}").into(),
        )),
        args: vec![Expr::Cast {
            expr: Box::new(value),
            ty: wide_ty,
        }],
    })
}

pub(super) fn bitint_to_int_expr(ty: &Type, value: Expr) -> Option<(Expr, bool)> {
    let (name, _, _, _) = bitint_generic_parts(ty)?;
    let signed = name == "bitint::BInt";
    Some((
        Expr::MethodCall {
            recv: Box::new(value),
            method: if signed { "to_i128" } else { "to_u128" }.into(),
            args: Vec::new(),
        },
        signed,
    ))
}

pub(super) fn bitint_zero_expr(ty: &Type) -> Option<Expr> {
    let (name, bits, limbs, bytes) = bitint_generic_parts(ty)?;
    Some(Expr::Var(
        format!("{name}::<{bits}, {limbs}, {bytes}>::ZERO").into(),
    ))
}

fn record_name_matches_va_list(name: &str) -> bool {
    name == "__va_list_tag" || name == "__va_list" || name == "__builtin_va_list"
}

pub(super) fn is_cir_va_list_record_type(
    ty: &CirType,
    aliases: &BTreeMap<String, CirType>,
) -> bool {
    match ty {
        CirType::Struct { name, .. } => name.as_deref().is_some_and(record_name_matches_va_list),
        CirType::Named(name) => match aliases.get(name) {
            Some(resolved) => is_cir_va_list_record_type(resolved, aliases),
            None => false,
        },
        _ => false,
    }
}

pub(super) fn is_cir_va_list_type(ty: &CirType, aliases: &BTreeMap<String, CirType>) -> bool {
    va_list_shaped_type(ty, aliases, false).is_some()
}

pub(super) fn is_cir_va_list_value_type(ty: &CirType, aliases: &BTreeMap<String, CirType>) -> bool {
    matches!(va_list_shaped_type(ty, aliases, false), Some(ty) if !matches!(ty, Type::Ptr { .. }))
}

pub(super) fn is_boxed_va_args_type(ty: &Type) -> bool {
    matches!(ty, Type::Custom(name) if name == "__SlateVaArgs")
}

pub(super) fn empty_va_args_expr() -> Expr {
    Expr::Call {
        binding: crate::function_identity::CallBinding::Generated,
        func: Box::new(Expr::Var("__SlateVaArgs::empty".into())),
        args: vec![],
    }
}

pub(super) fn va_list_shaped_type(
    ty: &CirType,
    aliases: &BTreeMap<String, CirType>,
    boxed: bool,
) -> Option<Type> {
    let value_ty = || {
        if boxed {
            Type::Custom("__SlateVaArgs".into())
        } else {
            Type::VaList
        }
    };
    if is_cir_va_list_record_type(ty, aliases) {
        return Some(value_ty());
    }
    if let CirType::Pointer { pointee: inner, .. } = ty
        && is_cir_va_list_record_type(inner, aliases)
    {
        return Some(value_ty());
    }
    if let CirType::Array {
        element_type: element,
        size: 1,
    } = ty
        && is_cir_va_list_record_type(element, aliases)
    {
        return Some(value_ty());
    }
    if let CirType::Array {
        element_type: element,
        size,
    } = ty
        && let Some(inner) = va_list_shaped_type(element, aliases, boxed)
    {
        return Some(Type::Array {
            elem: Box::new(inner),
            len: *size,
        });
    }
    if let CirType::Pointer { pointee: inner, .. } = ty
        && let Some(inner_ty) = va_list_shaped_type(inner, aliases, boxed)
    {
        return Some(Type::Ptr {
            mutable: true,
            inner: Box::new(inner_ty),
        });
    }
    None
}

pub(super) fn function_type_contains_va_list(ty: &CirType) -> bool {
    fn walk(ty: &CirType) -> bool {
        match ty {
            CirType::Struct { name, .. } => {
                name.as_deref().is_some_and(record_name_matches_va_list)
            }
            CirType::Named(name) => record_name_matches_va_list(name),
            CirType::Pointer { pointee: inner, .. }
            | CirType::Array {
                element_type: inner,
                ..
            } => walk(inner),
            _ => false,
        }
    }
    match ty {
        CirType::Func {
            inputs,
            optional_return_type,
            ..
        } => inputs.iter().any(walk) || optional_return_type.as_deref().is_some_and(walk),
        _ => false,
    }
}

pub(super) fn cir_fn_type_to_type(
    ty: &CirType,
    aliases: &BTreeMap<String, CirType>,
    va_list_boxed: bool,
) -> Option<Type> {
    let CirType::Func {
        inputs,
        optional_return_type: output,
        var_arg,
    } = ty
    else {
        return None;
    };
    let mut params: Vec<Type> = inputs
        .iter()
        .map(|param| rust_type_with_aliases(param, aliases, va_list_boxed))
        .collect();
    if *var_arg {
        params.push(Type::Variadic);
    }
    let ret = output
        .as_deref()
        .map(|output| rust_type_with_aliases(output, aliases, va_list_boxed))
        .unwrap_or(Type::CLib(CLibType::VOID));
    // a `void`-returning function is `Type::Unit`, not `Type::CLib(VOID)` -
    // the latter is only for `void*` pointee positions, and a mismatch here
    // means function items (which return `()`) don't unify with this fn
    // pointer type (e.g. `Option<unsafe extern "C" fn() -> c_void>`).
    let ret = if matches!(ret, Type::CLib(CLibType::VOID)) {
        Type::Unit
    } else {
        ret
    };
    Some(Type::FnPtr {
        abi: Abi::C,
        params,
        ret: Box::new(ret),
    })
}

pub(super) fn type_mentions_long_double(ty: &Type) -> bool {
    match ty {
        Type::LongDouble => true,
        Type::Complex(inner) => type_mentions_long_double(inner),
        Type::Generic { args, .. } => args.iter().any(type_mentions_long_double),
        Type::Ref { inner, .. } => type_mentions_long_double(inner),
        Type::Slice(elem) => type_mentions_long_double(elem),
        Type::Ptr { inner, .. } => type_mentions_long_double(inner),
        Type::Array { elem, .. } => type_mentions_long_double(elem),
        Type::FnPtr { params, ret, .. } => {
            params.iter().any(type_mentions_long_double) || type_mentions_long_double(ret)
        }
        Type::Prim(_)
        | Type::Custom(_)
        | Type::TyVar(_)
        | Type::CLib(_)
        | Type::VaList
        | Type::Str
        | Type::Unit
        | Type::Variadic
        | Type::Never => false,
    }
}

pub(super) fn type_mentions_complex(ty: &Type) -> bool {
    match ty {
        Type::Complex(_) => true,
        Type::Ref { inner, .. } => type_mentions_complex(inner),
        Type::Slice(elem) => type_mentions_complex(elem),
        Type::Ptr { inner, .. } => type_mentions_complex(inner),
        Type::Array { elem, .. } => type_mentions_complex(elem),
        Type::FnPtr { params, ret, .. } => {
            params.iter().any(type_mentions_complex) || type_mentions_complex(ret)
        }
        Type::Generic { args, .. } => args.iter().any(type_mentions_complex),
        Type::Prim(_)
        | Type::Custom(_)
        | Type::LongDouble
        | Type::TyVar(_)
        | Type::CLib(_)
        | Type::VaList
        | Type::Str
        | Type::Unit
        | Type::Variadic
        | Type::Never => false,
    }
}

fn type_mentions(ty: &Type, is_target: &impl Fn(&Type) -> bool) -> bool {
    if is_target(ty) {
        return true;
    }
    match ty {
        Type::Complex(inner) | Type::Ref { inner, .. } | Type::Slice(inner) => {
            type_mentions(inner, is_target)
        }
        Type::Ptr { inner, .. } | Type::Array { elem: inner, .. } => {
            type_mentions(inner, is_target)
        }
        Type::FnPtr { params, ret, .. } => {
            params.iter().any(|ty| type_mentions(ty, is_target)) || type_mentions(ret, is_target)
        }
        Type::Generic { args, .. } => args.iter().any(|ty| type_mentions(ty, is_target)),
        _ => false,
    }
}

pub(super) fn type_mentions_f128(ty: &Type) -> bool {
    type_mentions(ty, &|ty| matches!(ty, Type::Prim(Prim::F128)))
}

pub(super) fn type_mentions_f16(ty: &Type) -> bool {
    type_mentions(ty, &|ty| matches!(ty, Type::Prim(Prim::F16)))
}

pub(super) fn is_cir_function_pointer_type(ty: &CirType) -> bool {
    matches!(ty, CirType::Pointer { pointee: inner, .. } if matches!(**inner, CirType::Func { .. }))
}

pub(super) fn is_cir_void_pointer_type(ty: &CirType) -> bool {
    matches!(ty, CirType::Pointer { pointee: inner, .. } if matches!(**inner, CirType::Void))
}

pub(super) fn parse_cir_array_type(ty: &CirType) -> Option<(&CirType, u64)> {
    match ty {
        CirType::Array {
            element_type: element,
            size,
        } => Some((element, *size)),
        _ => None,
    }
}

pub(super) fn parse_cir_vector_type(ty: &CirType) -> Option<(&CirType, u64)> {
    match ty {
        CirType::Vector {
            element_type: element,
            size,
            ..
        } => Some((element, *size)),
        _ => None,
    }
}

pub(super) fn is_complex_long_double_coercion_type(
    ty: &CirType,
    aliases: &BTreeMap<String, CirType>,
) -> bool {
    let CirType::Named(name) = ty else {
        return false;
    };
    let Some(CirType::Struct {
        members: Some(members),
        ..
    }) = aliases.get(name)
    else {
        return false;
    };
    members.len() == 2 && members.iter().all(is_long_double)
}

pub(super) fn is_abi_coercion_record_name(name: &str) -> bool {
    name.strip_prefix("anon_struct")
        .is_some_and(|suffix| suffix.chars().all(|c| c.is_ascii_digit()))
}

pub(super) fn is_void_ptr_type(ty: &Type) -> bool {
    matches!(ty, Type::Ptr { inner, .. } if matches!(**inner, Type::CLib(c) if c == CLibType::VOID))
}

pub(super) fn cast_void_ptr_call_args(
    args: Vec<Expr>,
    arg_types: &[&CirType],
    param_types: &[Type],
) -> Vec<Expr> {
    args.into_iter()
        .enumerate()
        .map(|(i, arg)| {
            let Some(param_ty) = param_types.get(i).filter(|ty| is_void_ptr_type(ty)) else {
                return arg;
            };
            let Some(arg_ty) = arg_types.get(i) else {
                return arg;
            };
            if is_cir_function_pointer_type(arg_ty) || is_cir_void_pointer_type(arg_ty) {
                return arg;
            }
            if !matches!(arg_ty, CirType::Pointer { .. }) {
                return arg;
            }
            Expr::Cast {
                expr: Box::new(arg),
                ty: param_ty.clone(),
            }
        })
        .collect()
}

pub(super) fn cir_record_name(ty: &CirType) -> Option<&str> {
    match ty {
        CirType::Named(name) => name.strip_prefix("rec_"),
        CirType::Struct { name, .. } | CirType::Union { name, .. } => name.as_deref(),
        _ => None,
    }
}

pub(super) fn cir_ptr_pointee(ty: &CirType) -> Option<&CirType> {
    match ty {
        CirType::Pointer { pointee: inner, .. } => Some(inner),
        _ => None,
    }
}

pub(super) fn anon_alias_key<'a>(
    ty: &'a CirType,
    aliases: &BTreeMap<String, CirType>,
) -> Option<&'a str> {
    let CirType::Named(name) = ty else {
        return None;
    };
    let expanded = aliases.get(name)?;
    let record_name = cir_record_name(expanded).or_else(|| name.strip_prefix("rec_"))?;
    (record_name.starts_with("anon.") || record_name.starts_with("anon_")).then_some(name.as_str())
}

pub(super) fn collect_anon_alias_keys(
    ty: &CirType,
    aliases: &BTreeMap<String, CirType>,
    out: &mut BTreeSet<String>,
) {
    collect_anon_alias_keys_inner(ty, aliases, out, &mut BTreeSet::new());
    fn collect_anon_alias_keys_inner(
        ty: &CirType,
        aliases: &BTreeMap<String, CirType>,
        out: &mut BTreeSet<String>,
        seen: &mut BTreeSet<String>,
    ) {
        let CirType::Named(name) = ty else {
            if let Some(inner) = cir_ptr_pointee(ty) {
                collect_anon_alias_keys_inner(inner, aliases, out, seen);
            } else if let CirType::Array {
                element_type: element,
                ..
            } = ty
            {
                collect_anon_alias_keys_inner(element, aliases, out, seen);
            }
            return;
        };
        if !seen.insert(name.clone()) {
            return;
        }
        if let Some(key) = anon_alias_key(ty, aliases) {
            out.insert(key.to_string());
        }
        if let Some(expanded) = aliases.get(name) {
            if let Some(inner) = cir_ptr_pointee(expanded) {
                collect_anon_alias_keys_inner(inner, aliases, out, seen);
            } else if let CirType::Array {
                element_type: element,
                ..
            } = expanded
            {
                collect_anon_alias_keys_inner(element, aliases, out, seen);
            } else if let CirType::Struct {
                members: Some(members),
                ..
            }
            | CirType::Union {
                members: Some(members),
                ..
            } = expanded
            {
                for field_ty in members {
                    collect_anon_alias_keys_inner(field_ty, aliases, out, seen);
                }
            }
        }
    }
}

pub(super) fn parse_cir_int_type(ty: &CirType) -> Option<(bool, u32)> {
    match ty {
        CirType::Int {
            is_signed, width, ..
        } => Some((*is_signed, *width)),
        CirType::Named(name) => {
            let (signed, rest) = match name.as_bytes().first()? {
                b's' => (true, &name[1..]),
                b'u' => (false, &name[1..]),
                _ => return None,
            };
            let rest = rest.strip_suffix("_bitint").unwrap_or(rest);
            Some((signed, rest.strip_suffix('i')?.parse().ok()?))
        }
        _ => None,
    }
}

pub(super) fn fenv_scalar_bits(ty: &CirType) -> Option<u32> {
    match ty {
        CirType::Single => Some(32),
        CirType::Double => Some(64),
        _ => None,
    }
}

pub(super) fn fenv_binop_name(op: BinOp) -> Option<&'static str> {
    match op {
        BinOp::Add => Some("add"),
        BinOp::Sub => Some("sub"),
        BinOp::Mul => Some("mul"),
        BinOp::Div => Some("div"),
        BinOp::Rem => Some("rem"),
        _ => None,
    }
}

pub(super) fn fenv_cmp_name(kind: clang_ir::enums::CmpOpKind) -> Option<&'static str> {
    use clang_ir::enums::CmpOpKind;
    match kind {
        CmpOpKind::Lt => Some("lt"),
        CmpOpKind::Le => Some("le"),
        CmpOpKind::Gt => Some("gt"),
        CmpOpKind::Ge => Some("ge"),
        CmpOpKind::Eq => Some("eq"),
        CmpOpKind::Ne => Some("ne"),
        _ => None,
    }
}

pub(super) fn cir_type_to_ctype(
    ty: &CirType,
    aliases: &BTreeMap<String, CirType>,
) -> crate::frontend::c_ast::CType {
    use crate::frontend::c_ast::CType;
    if let Some(inner) = cir_ptr_pointee(ty) {
        return CType::Ptr(Box::new(cir_type_to_ctype(inner, aliases)));
    }
    match ty {
        CirType::Void => return CType::Void,
        CirType::Bool => return CType::Bool,
        CirType::Fp16 => return CType::Float { bits: 16 },
        CirType::Single => return CType::Float { bits: 32 },
        CirType::Double => return CType::Float { bits: 64 },
        CirType::Fp128 => return CType::Float { bits: 128 },
        _ => {}
    }
    if is_quad_long_double(ty) {
        return CType::Float { bits: 128 };
    }
    if is_long_double(ty) {
        return CType::Float { bits: 80 };
    }
    if let Some((signed, bits)) = parse_cir_int_type(ty) {
        return CType::Int { signed, bits };
    }
    if let CirType::Array {
        element_type: element,
        size,
    } = ty
    {
        return CType::Array(Box::new(cir_type_to_ctype(element, aliases)), Some(*size));
    }
    if let CirType::Vector {
        element_type: element,
        size,
        ..
    } = ty
    {
        return CType::Array(Box::new(cir_type_to_ctype(element, aliases)), Some(*size));
    }
    // resolve records through the alias table so anon fields keep their dotted name.
    let named = match ty {
        CirType::Named(name) => aliases
            .get(name)
            .and_then(cir_record_name)
            .or_else(|| cir_record_name(ty)),
        _ => cir_record_name(ty),
    };
    if let Some(name) = named {
        return CType::Record(name.to_string());
    }
    // non-record, non-scalar aliases (e.g. bare `!void`, `!bool` top-level
    // aliases) still need expanding - only record aliases keep the raw key
    // for dotted-name derivation above.
    if let CirType::Named(name) = ty
        && let Some(expanded) = aliases.get(name)
    {
        return cir_type_to_ctype(expanded, aliases);
    }
    CType::Int {
        signed: true,
        bits: 32,
    }
}

pub(super) fn reconcile_anonymous_member_types(
    module: &Module,
    records: &mut BTreeMap<String, crate::frontend::c_ast::Record>,
    anonymous_header_records: &[crate::frontend::c_ast::Record],
) -> Vec<crate::frontend::c_ast::Record> {
    use crate::frontend::c_ast::CType;

    let anonymous_header_records: BTreeMap<String, &crate::frontend::c_ast::Record> =
        anonymous_header_records
            .iter()
            .map(|record| (sanitize_ident(&record.name).into_string(), record))
            .collect();
    let mut reconciled = Vec::new();
    loop {
        let present_names: BTreeSet<String> = records.keys().cloned().collect();
        let bitfield_storage_names: BTreeSet<String> = records
            .iter()
            .filter(|(_, record)| {
                record
                    .fields
                    .iter()
                    .any(|field| field.name.starts_with("__bitfield_"))
            })
            .map(|(name, _)| name.clone())
            .collect();
        let mut additions = BTreeMap::new();
        for record in records.values_mut() {
            let Some(expanded) = module.generic.type_aliases.values().find_map(|expanded| {
                let (CirType::Struct { name, .. } | CirType::Union { name, .. }) = expanded else {
                    return None;
                };
                name.as_deref().filter(|name| {
                    sanitize_ident(name).as_str() == sanitize_ident(&record.name).as_str()
                })?;
                Some(expanded)
            }) else {
                continue;
            };
            let (CirType::Struct {
                members: Some(members),
                ..
            }
            | CirType::Union {
                members: Some(members),
                ..
            }) = expanded
            else {
                continue;
            };
            if members.len() != record.fields.len() {
                continue;
            }
            for (index, field) in record.fields.iter_mut().enumerate() {
                let cir_ty = cir_type_to_ctype(&members[index], &module.generic.type_aliases);
                if field.name == format!("__slate_anon_{index}") {
                    field.ty = cir_ty;
                    continue;
                }
                let (CType::Record(ast_name), CType::Record(cir_name)) = (&field.ty, &cir_ty)
                else {
                    continue;
                };
                let ast_name = sanitize_ident(ast_name).into_string();
                let cir_key = sanitize_ident(cir_name).into_string();
                if ast_name == cir_key {
                    continue;
                }
                if bitfield_storage_names.contains(&cir_key) {
                    field.ty = cir_ty;
                    continue;
                }
                if present_names.contains(&ast_name) {
                    continue;
                }
                let Some(header_record) = anonymous_header_records.get(&ast_name) else {
                    continue;
                };
                let mut header_record = (*header_record).clone();
                header_record.name = cir_name.clone();
                additions.entry(cir_key).or_insert(header_record);
                field.ty = cir_ty;
            }
        }
        if additions.is_empty() {
            break;
        }
        for (name, record) in additions {
            records.insert(name, record.clone());
            reconciled.push(record);
        }
    }
    reconciled
}

pub(super) fn collect_anon_record_info(
    ops: &[Operation],
    aliases: &BTreeMap<String, CirType>,
    needed: &mut BTreeSet<String>,
    field_names: &mut BTreeMap<(String, i64), String>,
) {
    for op in ops {
        match op.mnemonic() {
            "alloca" => {
                if let Some((_, ty)) = op.results.first()
                    && let Some(inner) = cir_ptr_pointee(ty)
                {
                    collect_anon_alias_keys(inner, aliases, needed);
                }
            }
            "get_member" => {
                if let Some(pointee) = op.operand_types.first().and_then(cir_ptr_pointee)
                    && let Some(key) = anon_alias_key(pointee, aliases)
                {
                    needed.insert(key.to_string());
                    if let (Some(index), Some(name)) = (
                        op.attr("index_attr").and_then(Attr::as_int),
                        op.attr("name")
                            .and_then(Attr::as_str)
                            .filter(|name| !name.is_empty()),
                    ) {
                        field_names.insert((key.to_string(), index as i64), name.to_string());
                    }
                }
            }
            "global" => {
                if let Some(CirType::Named(name)) = op.attr("sym_type").and_then(Attr::as_type) {
                    let ty = CirType::Named(name.clone());
                    collect_anon_alias_keys(&ty, aliases, needed);
                }
            }
            "call_llvm_intrinsic" => {
                if let Some((_, ty)) = op.results.first() {
                    collect_anon_alias_keys(ty, aliases, needed);
                }
            }
            _ => {}
        }
        for region in &op.regions {
            for block in &region.blocks {
                collect_anon_record_info(&block.ops, aliases, needed, field_names);
            }
        }
    }
}

pub(super) fn collect_anon_bitfield_slots(
    ops: &[Operation],
    aliases: &BTreeMap<String, CirType>,
    member_slots: &mut BTreeMap<String, (String, i64)>,
    bitfield_slots: &mut BTreeSet<(String, i64)>,
) {
    for op in ops {
        if op.mnemonic() == "get_member"
            && let (Some((result, _)), Some(key), Some(index)) = (
                op.results.first(),
                op.operand_types
                    .first()
                    .and_then(cir_ptr_pointee)
                    .and_then(|pointee| anon_alias_key(pointee, aliases)),
                op.attr("index_attr").and_then(Attr::as_int),
            )
        {
            member_slots.insert(result.clone(), (key.to_string(), index as i64));
        }
        if matches!(op.mnemonic(), "get_bitfield" | "set_bitfield")
            && let Some(slot) = op
                .operands
                .first()
                .and_then(|operand| member_slots.get(operand))
        {
            bitfield_slots.insert(slot.clone());
        }
        for region in &op.regions {
            for block in &region.blocks {
                collect_anon_bitfield_slots(&block.ops, aliases, member_slots, bitfield_slots);
            }
        }
    }
}

pub(super) fn resolve_local_record_collisions(
    cir: &Module,
    ast_records: &[crate::frontend::c_ast::Record],
) -> BTreeMap<String, crate::frontend::c_ast::Record> {
    let mut by_name: BTreeMap<String, Vec<&crate::frontend::c_ast::Record>> = BTreeMap::new();
    for record in ast_records {
        by_name
            .entry(sanitize_ident(&record.name).into_string())
            .or_default()
            .push(record);
    }
    by_name.retain(|_, records| records.len() > 1);
    if by_name.is_empty() {
        return BTreeMap::new();
    }

    let mut field_names: BTreeMap<(String, i64), String> = BTreeMap::new();
    collect_local_record_field_names(
        &cir.generic.ops,
        &cir.generic.type_aliases,
        &mut field_names,
    );

    let mut resolved = BTreeMap::new();
    for (base_sanitized, mut candidates) in by_name {
        let mut family: Vec<(String, Vec<String>)> = Vec::new();
        for (alias_key, expanded) in &cir.generic.type_aliases {
            let (CirType::Struct {
                name,
                members: Some(members),
                ..
            }
            | CirType::Union {
                name,
                members: Some(members),
                ..
            }) = expanded
            else {
                continue;
            };
            let Some(cir_name) = name.as_deref() else {
                continue;
            };
            if sanitize_ident(cir_record_base_name(cir_name)).into_string() != base_sanitized {
                continue;
            }
            let mut names = Vec::with_capacity(members.len());
            for index in 0..members.len() {
                let Some(name) = field_names.get(&(alias_key.clone(), index as i64)) else {
                    names.clear();
                    break;
                };
                names.push(name.clone());
            }
            family.push((sanitize_ident(cir_name).into_string(), names));
        }
        for (rust_key, wanted_names) in &family {
            if wanted_names.is_empty() {
                continue;
            }
            if let Some(pos) = candidates.iter().position(|record| {
                record
                    .fields
                    .iter()
                    .map(|field| field.name.as_str())
                    .eq(wanted_names.iter().map(String::as_str))
            }) {
                let mut matched = candidates.remove(pos).clone();
                matched.name = rust_key.clone();
                resolved.insert(rust_key.clone(), matched);
            }
        }
    }
    resolved
}

pub(super) fn cir_record_base_name(name: &str) -> &str {
    match name.rsplit_once('.') {
        Some((base, suffix))
            if !suffix.is_empty() && suffix.bytes().all(|b| b.is_ascii_digit()) =>
        {
            base
        }
        _ => name,
    }
}

pub(super) fn any_alias_key<'a>(
    ty: &'a CirType,
    aliases: &BTreeMap<String, CirType>,
) -> Option<&'a str> {
    let CirType::Named(name) = ty else {
        return None;
    };
    let expanded = aliases.get(name)?;
    cir_record_name(expanded).or_else(|| name.strip_prefix("rec_"))?;
    Some(name.as_str())
}

pub(super) fn collect_local_record_field_names(
    ops: &[Operation],
    aliases: &BTreeMap<String, CirType>,
    field_names: &mut BTreeMap<(String, i64), String>,
) {
    for op in ops {
        if op.mnemonic() == "get_member"
            && let (Some(key), Some(index), Some(name)) = (
                op.operand_types
                    .first()
                    .and_then(cir_ptr_pointee)
                    .and_then(|pointee| any_alias_key(pointee, aliases)),
                op.attr("index_attr").and_then(Attr::as_int),
                op.attr("name")
                    .and_then(Attr::as_str)
                    .filter(|name| !name.is_empty()),
            )
        {
            field_names.insert((key.to_string(), index as i64), name.to_string());
        }
        for region in &op.regions {
            for block in &region.blocks {
                collect_local_record_field_names(&block.ops, aliases, field_names);
            }
        }
    }
}

pub fn anon_local_records(module: &Module) -> Vec<crate::frontend::c_ast::Record> {
    let mut needed = BTreeSet::new();
    let mut field_names = BTreeMap::new();
    let mut bitfield_slots = BTreeSet::new();
    let mut member_slots = BTreeMap::new();
    collect_anon_bitfield_slots(
        &module.generic.ops,
        &module.generic.type_aliases,
        &mut member_slots,
        &mut bitfield_slots,
    );
    collect_anon_record_info(
        &module.generic.ops,
        &module.generic.type_aliases,
        &mut needed,
        &mut field_names,
    );

    let mut frontier: Vec<String> = needed.iter().cloned().collect();
    while let Some(key) = frontier.pop() {
        let members = match module.generic.type_aliases.get(&key) {
            Some(CirType::Struct { members, .. }) | Some(CirType::Union { members, .. }) => {
                members.as_deref().unwrap_or_default()
            }
            _ => continue,
        };
        for field_ty in members {
            let mut field_keys = BTreeSet::new();
            collect_anon_alias_keys(field_ty, &module.generic.type_aliases, &mut field_keys);
            for field_key in field_keys {
                if needed.insert(field_key.clone()) {
                    frontier.push(field_key);
                }
            }
        }
    }

    let mut records = Vec::new();
    for key in &needed {
        let (name, members, member_kinds, is_union) = match module.generic.type_aliases.get(key) {
            Some(CirType::Struct {
                name,
                members: Some(members),
                member_kinds,
                ..
            }) => (name, members, member_kinds, false),
            Some(CirType::Union {
                name,
                members: Some(members),
                member_kinds,
                ..
            }) => (name, members, member_kinds, true),
            _ => continue,
        };
        let Some(name) = name.as_deref().or_else(|| key.strip_prefix("rec_")) else {
            continue;
        };
        let fields = members
            .iter()
            .enumerate()
            .filter(|(i, _)| {
                matches!(
                    member_kinds.get(*i),
                    Some(CirRecordMemberKind::Data | CirRecordMemberKind::Pad)
                )
            })
            .map(|(i, field_ty)| crate::frontend::c_ast::Decl {
                name: if bitfield_slots.contains(&(key.clone(), i as i64)) {
                    format!("__bitfield_{i}")
                } else {
                    field_names
                        .get(&(key.clone(), i as i64))
                        .cloned()
                        .unwrap_or_else(|| format!("__slate_anon_{i}"))
                },
                comments: Vec::new(),
                ty: cir_type_to_ctype(field_ty, &module.generic.type_aliases),
                bit_width: bitfield_slots
                    .contains(&(key.clone(), i as i64))
                    .then_some(0),
            })
            .collect();
        records.push(crate::frontend::c_ast::Record {
            name: name.to_string(),
            comments: Vec::new(),
            kind: if is_union {
                RecordKind::Union
            } else {
                RecordKind::Struct
            },
            fields,
            packed: None,
            align: None,
        });
    }
    records
}
