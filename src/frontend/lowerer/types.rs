use super::*;

pub(super) fn rust_type(cir_ty: &str) -> Type {
    rust_type_with_aliases(cir_ty, &BTreeMap::new())
}

// True if the region contains a `cir.continue` that targets the enclosing loop,
// i.e. one not swallowed by a nested loop. `cir.if`/`cir.scope`/`cir.switch`
// bodies are transparent, so we recurse through them.
pub(super) fn region_has_direct_continue(region: &Region) -> bool {
    region
        .blocks
        .iter()
        .any(|block| ops_have_direct_continue(&block.ops))
}

pub(super) fn ops_have_direct_continue(ops: &[Op]) -> bool {
    ops.iter().any(|op| match op.kind() {
        CirOpKind::Continue => true,
        CirOpKind::For | CirOpKind::While | CirOpKind::Do => false,
        _ => op.regions.iter().any(region_has_direct_continue),
    })
}

pub(super) fn rust_type_with_aliases(cir_ty: &str, aliases: &BTreeMap<String, String>) -> Type {
    let ty = cir_ty.trim();
    if is_cir_va_list_type(ty) {
        return Type::VaList;
    }
    if let Some(expanded) = aliases.get(ty) {
        if (expanded.starts_with("!cir.struct<{") || expanded.starts_with("!cir.union<{"))
            && let Some(name) = ty.strip_prefix("!rec_")
        {
            return Type::Custom(rust_record_name(name));
        }
        return rust_type_with_aliases(expanded, aliases);
    }
    if ty == "()" || ty.is_empty() {
        Type::Unit
    } else if ty == "!void" || ty == "!cir.void" {
        Type::CLib(CLibType::VOID)
    } else if ty == "!cir.bool" {
        Type::Prim(Prim::Bool)
    } else if ty == "!s32i" || ty == "!cir.int<s, 32>" {
        Type::Prim(Prim::I32)
    } else if ty == "!u32i" || ty == "!cir.int<u, 32>" {
        Type::Prim(Prim::U32)
    } else if ty == "!s16i" || ty == "!cir.int<s, 16>" {
        Type::Prim(Prim::I16)
    } else if ty == "!u16i" || ty == "!cir.int<u, 16>" {
        Type::Prim(Prim::U16)
    } else if ty == "!s8i" || ty == "!cir.int<s, 8>" {
        Type::Prim(Prim::I8)
    } else if ty == "!u8i" || ty == "!cir.int<u, 8>" {
        Type::Prim(Prim::U8)
    } else if ty == "!s64i" || ty == "!cir.int<s, 64>" {
        Type::Prim(Prim::I64)
    } else if ty == "!u64i" || ty == "!cir.int<u, 64>" {
        Type::Prim(Prim::U64)
    } else if ty == "!s128i"
        || ty == "!cir.int<s, 128>"
        || ty == "!s128i_bitint"
        || ty == "!cir.int<s, 128, bitint>"
    {
        Type::Prim(Prim::I128)
    } else if ty == "!u128i"
        || ty == "!cir.int<u, 128>"
        || ty == "!u128i_bitint"
        || ty == "!cir.int<u, 128, bitint>"
    {
        Type::Prim(Prim::U128)
    } else if ty == "!cir.float" {
        Type::Prim(Prim::F32)
    } else if ty == "!cir.double" {
        Type::Prim(Prim::F64)
    } else if ty == "!cir.f128" || is_quad_long_double(ty) {
        Type::Prim(Prim::F128)
    } else if is_long_double(ty) && crate::cir::emit::uses_f64_long_double_abi() {
        Type::Prim(Prim::F64)
    } else if is_long_double(ty) {
        Type::LongDouble
    } else if let Some(inner) = ty
        .strip_prefix("!cir.complex<")
        .and_then(|s| s.strip_suffix('>'))
    {
        Type::Complex(Box::new(rust_type_with_aliases(inner, aliases)))
    } else if let Some((inner, len)) = parse_cir_vector_type(ty) {
        Type::Array {
            elem: Box::new(rust_type_with_aliases(&inner, aliases)),
            len,
        }
    } else if let Some(inner) = ty
        .strip_prefix("!cir.ptr<")
        .and_then(|s| s.strip_suffix('>'))
    {
        if let Some(fn_ty) = cir_fn_type_to_type(inner, aliases) {
            fn_ty
        } else {
            Type::Ptr {
                mutable: true,
                inner: Box::new(rust_type_with_aliases(inner, aliases)),
            }
        }
    } else if let Some((inner, len)) = parse_cir_array_type(ty) {
        Type::Array {
            elem: Box::new(rust_type_with_aliases(&inner, aliases)),
            len,
        }
    } else if let Some(name) = cir_record_name(ty) {
        clib_record_type(name)
            .map(Type::CLib)
            .unwrap_or_else(|| Type::Custom(rust_record_name(name)))
    } else {
        Type::Prim(Prim::I32)
    }
}

pub(super) fn is_cir_va_list_record_type(ty: &str) -> bool {
    cir_record_name(ty).is_some_and(|name| {
        name == "__va_list_tag" || name == "__va_list" || name == "__builtin_va_list"
    })
}

pub(super) fn is_cir_va_list_type(ty: &str) -> bool {
    let ty = ty.trim();
    is_cir_va_list_record_type(ty)
        || cir_ptr_inner(ty).is_some_and(is_cir_va_list_record_type)
        || parse_cir_array_type(ty)
            .is_some_and(|(elem, len)| len == 1 && is_cir_va_list_record_type(&elem))
}

pub(super) fn function_type_contains_va_list(ty: &str) -> bool {
    ty.contains("!rec___va_list_tag")
        || ty.contains("!rec___va_list")
        || ty.contains("!rec___builtin_va_list")
}

pub(super) fn cir_fn_type_to_type(ty: &str, aliases: &BTreeMap<String, String>) -> Option<Type> {
    let inner = ty
        .trim()
        .strip_prefix("!cir.func<")
        .and_then(|s| s.strip_suffix('>'))?;
    let (params, ret) = split_top_level_arrow(inner).unwrap_or((inner, "()"));
    let params = params.trim().trim_start_matches('(').trim_end_matches(')');
    let params = split_top_level(params, ',')
        .into_iter()
        .map(str::trim)
        .filter(|s| !s.is_empty())
        .map(|param| {
            if param == "..." {
                Type::Variadic
            } else {
                rust_type_with_aliases(param, aliases)
            }
        })
        .collect::<Vec<_>>();
    let ret = rust_type_with_aliases(ret.trim(), aliases);
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

pub(super) fn type_mentions_f128(ty: &Type) -> bool {
    match ty {
        Type::Prim(Prim::F128) => true,
        Type::Complex(inner) | Type::Ref { inner, .. } | Type::Slice(inner) => {
            type_mentions_f128(inner)
        }
        Type::Ptr { inner, .. } | Type::Array { elem: inner, .. } => type_mentions_f128(inner),
        Type::FnPtr { params, ret, .. } => {
            params.iter().any(type_mentions_f128) || type_mentions_f128(ret)
        }
        Type::Generic { args, .. } => args.iter().any(type_mentions_f128),
        _ => false,
    }
}

pub(super) fn is_cir_function_pointer_type(ty: &str) -> bool {
    ty.trim()
        .strip_prefix("!cir.ptr<")
        .and_then(|s| s.strip_suffix('>'))
        .is_some_and(|inner| inner.trim().starts_with("!cir.func<"))
}

pub(super) fn is_cir_void_pointer_type(ty: &str) -> bool {
    ty.trim()
        .strip_prefix("!cir.ptr<")
        .and_then(|s| s.strip_suffix('>'))
        .is_some_and(|inner| matches!(inner.trim(), "!void" | "!cir.void"))
}

pub(super) fn is_void_ptr_type(ty: &Type) -> bool {
    matches!(ty, Type::Ptr { inner, .. } if matches!(**inner, Type::CLib(c) if c == CLibType::VOID))
}

pub(super) fn cast_void_ptr_call_args(
    args: Vec<Expr>,
    arg_types: &[&str],
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
            if !arg_ty.starts_with("!cir.ptr<") {
                return arg;
            }
            Expr::Cast {
                expr: Box::new(arg),
                ty: param_ty.clone(),
            }
        })
        .collect()
}

pub(super) fn parse_cir_array_type(ty: &str) -> Option<(String, u64)> {
    let inner = ty
        .trim()
        .strip_prefix("!cir.array<")
        .and_then(|s| s.strip_suffix('>'))?;
    let (element, len) = inner.rsplit_once(" x ")?;
    Some((element.trim().to_string(), len.trim().parse().ok()?))
}

pub(super) fn parse_cir_vector_type(ty: &str) -> Option<(String, u64)> {
    let inner = ty
        .trim()
        .strip_prefix("!cir.vector<")
        .and_then(|s| s.strip_suffix('>'))?;
    let (len, element) = inner.split_once(" x ")?;
    Some((element.trim().to_string(), len.trim().parse().ok()?))
}

pub(super) fn parse_rust_array_type(ty: &str) -> Option<(&str, u64)> {
    let inner = ty
        .trim()
        .strip_prefix('[')
        .and_then(|s| s.strip_suffix(']'))?;
    let (element, len) = inner.rsplit_once(';')?;
    Some((element.trim(), len.trim().parse().ok()?))
}

pub(super) fn cir_record_name(ty: &str) -> Option<&str> {
    if let Some(name) = ty.strip_prefix("!rec_") {
        return Some(name);
    }
    let rest = ty
        .strip_prefix("!cir.union<\"")
        .or_else(|| ty.strip_prefix("!cir.struct<\""))?;
    rest.split_once('"').map(|(name, _)| name)
}

pub(super) fn op_type_return(ty: &str) -> Option<&str> {
    split_top_level_arrow(ty).map(|(_, ret)| ret.trim())
}

pub(super) fn cir_ptr_pointee(ty: &str) -> Option<&str> {
    ty.trim()
        .strip_prefix("!cir.ptr<")
        .and_then(|s| s.strip_suffix('>'))
}

pub(super) fn anon_alias_key(ty: &str, aliases: &BTreeMap<String, String>) -> Option<String> {
    let ty = ty.trim();
    let expanded = aliases.get(ty)?;
    let name = cir_record_name(expanded).or_else(|| ty.strip_prefix("!rec_"))?;
    (name.starts_with("anon.") || name.starts_with("anon_") || name == "__once_flag")
        .then(|| ty.to_string())
}

pub(super) fn collect_anon_alias_keys(
    ty: &str,
    aliases: &BTreeMap<String, String>,
    out: &mut BTreeSet<String>,
) {
    collect_anon_alias_keys_inner(ty, aliases, out, &mut BTreeSet::new());
}

pub(super) fn collect_anon_alias_keys_inner(
    ty: &str,
    aliases: &BTreeMap<String, String>,
    out: &mut BTreeSet<String>,
    seen: &mut BTreeSet<String>,
) {
    let ty = ty.trim();
    if !seen.insert(ty.to_string()) {
        return;
    }
    if let Some(key) = anon_alias_key(ty, aliases) {
        out.insert(key);
    }
    if let Some(inner) = cir_ptr_pointee(ty) {
        collect_anon_alias_keys_inner(inner, aliases, out, seen);
    } else if let Some((elem, _)) = parse_cir_array_type(ty) {
        collect_anon_alias_keys_inner(&elem, aliases, out, seen);
    } else if let Some(expanded) = aliases.get(ty)
        && let (Some(open), Some(close)) = (expanded.find('{'), expanded.rfind('}'))
    {
        for field_ty in split_top_level(&expanded[open + 1..close], ',') {
            collect_anon_alias_keys_inner(field_ty, aliases, out, seen);
        }
    }
}

pub(super) fn parse_cir_int_type(ty: &str) -> Option<(bool, u32)> {
    let ty = ty.trim();
    if let Some(rest) = ty
        .strip_prefix("!cir.int<")
        .and_then(|s| s.strip_suffix('>'))
    {
        let mut parts = rest.split(',').map(str::trim);
        let signed = match parts.next()? {
            "s" => true,
            "u" => false,
            _ => return None,
        };
        return Some((signed, parts.next()?.parse().ok()?));
    }
    let rest = ty.strip_prefix('!')?;
    let rest = rest.strip_suffix("_bitint").unwrap_or(rest);
    let signed = match rest.as_bytes().first()? {
        b's' => true,
        b'u' => false,
        _ => return None,
    };
    let bits = rest[1..].strip_suffix('i')?.parse().ok()?;
    Some((signed, bits))
}

pub(super) fn cir_type_to_ctype(
    ty: &str,
    aliases: &BTreeMap<String, String>,
) -> crate::frontend::c_ast::CType {
    use crate::frontend::c_ast::CType;
    let ty = ty.trim();
    if let Some(inner) = cir_ptr_pointee(ty) {
        return CType::Ptr(Box::new(cir_type_to_ctype(inner, aliases)));
    }
    match ty {
        "!cir.void" | "!void" => return CType::Void,
        "!cir.bool" => return CType::Bool,
        "!cir.float" => return CType::Float { bits: 32 },
        "!cir.double" => return CType::Float { bits: 64 },
        "!cir.f128" => return CType::Float { bits: 128 },
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
    if let Some((elem, len)) = parse_cir_array_type(ty) {
        return CType::Array(Box::new(cir_type_to_ctype(&elem, aliases)), Some(len));
    }
    // resolve records through the alias table so anon fields keep their dotted name.
    if let Some(name) = aliases
        .get(ty)
        .and_then(|expanded| cir_record_name(expanded))
        .or_else(|| cir_record_name(ty))
    {
        return CType::Record(name.to_string());
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
        let mut additions = BTreeMap::new();
        for record in records.values_mut() {
            let Some(expanded) = module.aliases.values().find(|expanded| {
                cir_record_name(expanded).is_some_and(|name| {
                    sanitize_ident(name).as_str() == sanitize_ident(&record.name).as_str()
                })
            }) else {
                continue;
            };
            let (Some(open), Some(close)) = (expanded.find('{'), expanded.rfind('}')) else {
                continue;
            };
            let field_types = split_top_level(&expanded[open + 1..close], ',');
            if field_types.len() != record.fields.len() {
                continue;
            }
            for (index, field) in record.fields.iter_mut().enumerate() {
                let cir_ty = cir_type_to_ctype(field_types[index], &module.aliases);
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
                if ast_name == cir_key || present_names.contains(&ast_name) {
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
    ops: &[Op],
    aliases: &BTreeMap<String, String>,
    needed: &mut BTreeSet<String>,
    field_names: &mut BTreeMap<(String, i64), String>,
) {
    for op in ops {
        match op.kind() {
            CirOpKind::Alloca => {
                if let Some(ty) = op
                    .ty
                    .as_deref()
                    .and_then(op_type_return)
                    .and_then(cir_ptr_pointee)
                {
                    collect_anon_alias_keys(ty, aliases, needed);
                }
            }
            CirOpKind::GetMember => {
                if let (Some(key), Some(index), Some(name)) = (
                    op.ty
                        .as_deref()
                        .and_then(split_top_level_arrow)
                        .and_then(|(inputs, _)| inputs.trim().strip_prefix('(')?.strip_suffix(')'))
                        .and_then(|inputs| split_top_level(inputs, ',').first().copied())
                        .and_then(cir_ptr_pointee)
                        .and_then(|pointee| anon_alias_key(pointee, aliases)),
                    op.attrs.get("index_attr").and_then(Attr::as_int),
                    op.attrs
                        .get("name")
                        .and_then(Attr::as_str)
                        .filter(|name| !name.is_empty()),
                ) {
                    field_names.insert((key, index), name.to_string());
                }
            }
            CirOpKind::Global => {
                if let Some(ty) = attr_str(op, "sym_type") {
                    collect_anon_alias_keys(ty, aliases, needed);
                }
            }
            CirOpKind::CallLlvmIntrinsic => {
                if let Some(ty) = op_result_type(op) {
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
    ops: &[Op],
    aliases: &BTreeMap<String, String>,
    member_slots: &mut BTreeMap<String, (String, i64)>,
    bitfield_slots: &mut BTreeSet<(String, i64)>,
) {
    for op in ops {
        if op.kind() == CirOpKind::GetMember
            && let (Some(result), Some(key), Some(index)) = (
                op.results.first(),
                op.ty
                    .as_deref()
                    .and_then(split_top_level_arrow)
                    .and_then(|(inputs, _)| inputs.trim().strip_prefix('(')?.strip_suffix(')'))
                    .and_then(|inputs| split_top_level(inputs, ',').first().copied())
                    .and_then(cir_ptr_pointee)
                    .and_then(|pointee| anon_alias_key(pointee, aliases)),
                op.attrs.get("index_attr").and_then(Attr::as_int),
            )
        {
            member_slots.insert(result.clone(), (key, index));
        }
        if matches!(op.kind(), CirOpKind::GetBitfield | CirOpKind::SetBitfield)
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
    collect_local_record_field_names(&cir.ops, &cir.aliases, &mut field_names);

    let mut resolved = BTreeMap::new();
    for (base_sanitized, mut candidates) in by_name {
        let mut family: Vec<(String, Vec<String>)> = Vec::new();
        for (alias_key, expanded) in &cir.aliases {
            let Some(cir_name) = cir_record_name(expanded) else {
                continue;
            };
            if sanitize_ident(cir_record_base_name(cir_name)).into_string() != base_sanitized {
                continue;
            }
            let (Some(open), Some(close)) = (expanded.find('{'), expanded.rfind('}')) else {
                continue;
            };
            let field_count = split_top_level(&expanded[open + 1..close], ',')
                .iter()
                .filter(|field| !field.trim().is_empty())
                .count();
            let mut names = Vec::with_capacity(field_count);
            for index in 0..field_count {
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

pub(super) fn any_alias_key(ty: &str, aliases: &BTreeMap<String, String>) -> Option<String> {
    let ty = ty.trim();
    let expanded = aliases.get(ty)?;
    cir_record_name(expanded).or_else(|| ty.strip_prefix("!rec_"))?;
    Some(ty.to_string())
}

pub(super) fn collect_local_record_field_names(
    ops: &[Op],
    aliases: &BTreeMap<String, String>,
    field_names: &mut BTreeMap<(String, i64), String>,
) {
    for op in ops {
        if op.kind() == CirOpKind::GetMember
            && let (Some(key), Some(index), Some(name)) = (
                op.ty
                    .as_deref()
                    .and_then(split_top_level_arrow)
                    .and_then(|(inputs, _)| inputs.trim().strip_prefix('(')?.strip_suffix(')'))
                    .and_then(|inputs| split_top_level(inputs, ',').first().copied())
                    .and_then(cir_ptr_pointee)
                    .and_then(|pointee| any_alias_key(pointee, aliases)),
                op.attrs.get("index_attr").and_then(Attr::as_int),
                op.attrs
                    .get("name")
                    .and_then(Attr::as_str)
                    .filter(|name| !name.is_empty()),
            )
        {
            field_names.insert((key, index), name.to_string());
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
        &module.ops,
        &module.aliases,
        &mut member_slots,
        &mut bitfield_slots,
    );
    collect_anon_record_info(&module.ops, &module.aliases, &mut needed, &mut field_names);

    let mut frontier: Vec<String> = needed.iter().cloned().collect();
    while let Some(key) = frontier.pop() {
        let Some(expanded) = module.aliases.get(&key) else {
            continue;
        };
        let (Some(open), Some(close)) = (expanded.find('{'), expanded.rfind('}')) else {
            continue;
        };
        for field_ty in split_top_level(&expanded[open + 1..close], ',') {
            let mut field_keys = BTreeSet::new();
            collect_anon_alias_keys(field_ty.trim(), &module.aliases, &mut field_keys);
            for field_key in field_keys {
                if needed.insert(field_key.clone()) {
                    frontier.push(field_key);
                }
            }
        }
    }

    let mut records = Vec::new();
    for key in &needed {
        let Some(expanded) = module.aliases.get(key) else {
            continue;
        };
        let Some(name) = cir_record_name(expanded).or_else(|| key.strip_prefix("!rec_")) else {
            continue;
        };
        let is_union = expanded.trim_start().starts_with("!cir.union");
        let (Some(open), Some(close)) = (expanded.find('{'), expanded.rfind('}')) else {
            continue;
        };
        let fields = split_top_level(&expanded[open + 1..close], ',')
            .iter()
            .map(|s| s.trim())
            .filter(|s| !s.is_empty())
            .enumerate()
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
                ty: cir_type_to_ctype(field_ty, &module.aliases),
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
