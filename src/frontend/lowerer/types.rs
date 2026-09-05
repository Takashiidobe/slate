use super::*;
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
                        name: record_name,
                        members,
                        ..
                    }
                    | CirType::Union {
                        name: record_name,
                        members,
                        ..
                    },
                ) => {
                    let alias_key = name.strip_prefix("rec_");
                    if record_name.is_none()
                        && alias_key.is_some_and(is_abi_coercion_record_name)
                        && let Some(members) = members
                    {
                        Type::Custom(abi_coercion_canonical_name(members, aliases))
                    } else {
                        record_struct_type(record_name.as_deref(), alias_key)
                    }
                }
                Some(expanded) => rust_type_with_aliases(expanded, aliases, va_list_boxed),
                None if alias_name.starts_with("rec_") => {
                    record_struct_type(None, alias_name.strip_prefix("rec_"))
                }
                None => Type::Prim(Prim::I32),
            }
        }
        CirType::Bool => Type::Prim(Prim::Bool),
        CirType::Void => Type::CLib(CLibType::VOID),
        CirType::BitField {
            storage_type: Some(storage_type),
            ..
        } => rust_type_with_aliases(storage_type, aliases, va_list_boxed),
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
            } else if crate::frontend::toolchain::uses_f64_long_double_abi() {
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
        | CirType::BitField { .. }
        | CirType::CatchToken
        | CirType::CleanupToken
        | CirType::CudaDeviceTexture
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

pub(super) fn bitint_cast_expr(result_ty: &Type, src_ty: &Type, value: Expr) -> Option<Expr> {
    let (result_name, result_bits, result_limbs, result_bytes) = bitint_generic_parts(result_ty)?;
    let (src_name, ..) = bitint_generic_parts(src_ty)?;
    let method = if src_name == "bitint::BInt" {
        "from_bint"
    } else {
        "from_buint"
    };
    Some(Expr::Call {
        binding: crate::function_identity::CallBinding::Generated,
        func: Box::new(Expr::Var(
            format!("{result_name}::<{result_bits}, {result_limbs}, {result_bytes}>::{method}")
                .into(),
        )),
        args: vec![value],
    })
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
    let ret = if matches!(ret, Type::CLib(CLibType::VOID)) {
        Type::Unit
    } else {
        ret
    };
    Some(Type::FnPtr {
        abi: Abi::CUnwind,
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
        | Type::String
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
        | Type::String
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
    ty.pointee()
        .is_some_and(|inner| inner.as_function().is_some())
}

pub(super) fn is_cir_void_pointer_type(ty: &CirType) -> bool {
    ty.pointee()
        .is_some_and(|inner| matches!(inner, CirType::Void))
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
    name.strip_prefix("anon_struct").is_some_and(|suffix| {
        suffix.is_empty() || suffix.starts_with('_') || suffix.chars().all(|c| c.is_ascii_digit())
    })
}

fn ctype_abi_sig(ty: &crate::frontend::c_ast::CType) -> String {
    use crate::frontend::c_ast::CType;
    match ty {
        CType::Int { signed, bits } => format!("{}{bits}", if *signed { "i" } else { "u" }),
        CType::Char { signed } => format!("{}8", if *signed { "i" } else { "u" }),
        CType::Float { bits } => format!("f{bits}"),
        CType::Bool => "b".to_string(),
        CType::Ptr(_) | CType::FuncPtr { .. } => "p".to_string(),
        CType::Array(inner, len) => format!("a{}x{}", len.unwrap_or(0), ctype_abi_sig(inner)),
        CType::Void => "v".to_string(),
        CType::Record(name) => format!("r{}", sanitize_ident(name).into_string()),
        CType::Enum(name) => format!("e{}", sanitize_ident(name).into_string()),
        CType::Complex(inner) => format!("c{}", ctype_abi_sig(inner)),
    }
}

pub(super) fn abi_coercion_canonical_name(
    members: &[CirType],
    aliases: &BTreeMap<String, CirType>,
) -> String {
    let mut name = String::from("anon_struct");
    for member in members {
        name.push('_');
        name.push_str(&ctype_abi_sig(&cir_type_to_ctype(member, aliases)));
    }
    name
}

pub(super) fn canonical_alias_record_name(
    alias_key: &str,
    ty: &CirType,
    aliases: &BTreeMap<String, CirType>,
) -> Option<String> {
    if let Some(name) = slate_record_name(ty) {
        return Some(name.to_string());
    }
    let stripped = alias_key.strip_prefix("rec_")?;
    if is_abi_coercion_record_name(stripped)
        && let CirType::Struct {
            members: Some(members),
            ..
        }
        | CirType::Union {
            members: Some(members),
            ..
        } = ty
    {
        return Some(abi_coercion_canonical_name(members, aliases));
    }
    Some(stripped.to_string())
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

pub(super) fn slate_record_name(ty: &CirType) -> Option<&str> {
    ty.record_name().or_else(|| match ty {
        CirType::Named(name) => name.strip_prefix("rec_"),
        _ => None,
    })
}

pub(super) fn anon_alias_key<'a>(
    ty: &'a CirType,
    aliases: &BTreeMap<String, CirType>,
) -> Option<&'a str> {
    let CirType::Named(name) = ty else {
        return None;
    };
    let expanded = aliases.get(name)?;
    let record_name = slate_record_name(expanded).or_else(|| name.strip_prefix("rec_"))?;
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
            if let Some(inner) = ty.pointee() {
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
            if let Some(inner) = expanded.pointee() {
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

pub(super) fn resolve_type_alias<'a>(
    ty: &'a CirType,
    aliases: &'a BTreeMap<String, CirType>,
) -> &'a CirType {
    let mut ty = ty;
    let mut seen = BTreeSet::new();
    while let CirType::Named(name) = ty {
        if !seen.insert(name.clone()) {
            break;
        }
        match aliases.get(name) {
            Some(expanded) => ty = expanded,
            None => break,
        }
    }
    ty
}

pub(super) fn resolved_integer_parts(
    ty: &CirType,
    aliases: &BTreeMap<String, CirType>,
) -> Option<(bool, u32)> {
    resolve_type_alias(ty, aliases)
        .as_integer()
        .map(|(signed, width, _)| (signed, width))
}

pub(super) fn fenv_is_constrained(fenv: &Option<Attr>) -> bool {
    let Some(Attr::Fenv {
        dynamic_rounding_mode,
        strict_except,
        ..
    }) = fenv
    else {
        return false;
    };
    strict_except.as_deref() == Some("true") || dynamic_rounding_mode.as_deref() == Some("unknown")
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
    if let Some(inner) = ty.pointee() {
        if let Some((inputs, optional_return_type, _)) = inner.as_function() {
            let params = inputs
                .iter()
                .map(|param| cir_type_to_ctype(param, aliases))
                .collect();
            let ret = optional_return_type
                .map(|output| cir_type_to_ctype(output, aliases))
                .unwrap_or(CType::Void);
            return CType::FuncPtr {
                ret: Box::new(ret),
                params,
            };
        }
        return CType::Ptr(Box::new(cir_type_to_ctype(inner, aliases)));
    }
    if let CirType::BitField {
        storage_type: Some(storage_type),
        ..
    } = ty
    {
        return cir_type_to_ctype(storage_type, aliases);
    }
    match ty {
        CirType::Void => return CType::Void,
        CirType::Bool => return CType::Bool,
        CirType::Fp16 => return CType::Float { bits: 16 },
        CirType::Single => return CType::Float { bits: 32 },
        CirType::Double => return CType::Float { bits: 64 },
        CirType::Fp128 => return CType::Float { bits: 128 },
        CirType::Complex { element_type } => {
            return CType::Complex(Box::new(cir_type_to_ctype(element_type, aliases)));
        }
        _ => {}
    }
    if is_quad_long_double(ty) {
        return CType::Float { bits: 128 };
    }
    if is_long_double(ty) {
        return CType::Float { bits: 80 };
    }
    if let Some((signed, bits)) = resolved_integer_parts(ty, aliases) {
        return CType::Int { signed, bits };
    }
    if let Some((element, size)) = ty.as_array() {
        return CType::Array(Box::new(cir_type_to_ctype(element, aliases)), Some(size));
    }
    if let Some((element, size, _)) = ty.as_vector() {
        return CType::Array(Box::new(cir_type_to_ctype(element, aliases)), Some(size));
    }
    // resolve records through the alias table so anon fields keep their dotted name.
    let named = match ty {
        CirType::Named(name) => aliases
            .get(name)
            .and_then(slate_record_name)
            .or_else(|| slate_record_name(ty)),
        _ => slate_record_name(ty),
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
