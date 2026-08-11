use super::*;

pub(super) fn type_array_len(ty: &Type) -> Option<u64> {
    match ty {
        Type::Array { len, .. } => Some(*len),
        _ => None,
    }
}

pub(super) fn byte_array_elems(bytes: &[u8], ty: &Type) -> Vec<Expr> {
    let signed = matches!(
        ty,
        Type::Array {
            elem,
            ..
        } if matches!(&**elem, Type::Prim(Prim::I8))
    );
    bytes
        .iter()
        .map(|byte| {
            let value = if signed {
                i64::from(i8::from_ne_bytes([*byte]))
            } else {
                i64::from(*byte)
            };
            Expr::Value(RustValue::I64(value))
        })
        .collect()
}

fn standard_record_fields(name: &str) -> Option<Vec<RecordField>> {
    let i8_ty = || Type::Prim(Prim::I8);
    let i32_ty = || Type::Prim(Prim::I32);
    let i64_ty = || Type::Prim(Prim::I64);
    let i8_ptr_ty = || Type::Ptr {
        mutable: true,
        inner: Box::new(Type::Prim(Prim::I8)),
    };
    let fields = match name {
        "div_t" => vec![("quot".into(), i32_ty()), ("rem".into(), i32_ty())],
        "ldiv_t" | "lldiv_t" | "imaxdiv_t" => {
            vec![("quot".into(), i64_ty()), ("rem".into(), i64_ty())]
        }
        "tm" => vec![
            ("tm_sec".into(), i32_ty()),
            ("tm_min".into(), i32_ty()),
            ("tm_hour".into(), i32_ty()),
            ("tm_mday".into(), i32_ty()),
            ("tm_mon".into(), i32_ty()),
            ("tm_year".into(), i32_ty()),
            ("tm_wday".into(), i32_ty()),
            ("tm_yday".into(), i32_ty()),
            ("tm_isdst".into(), i32_ty()),
            ("tm_gmtoff".into(), i64_ty()),
            ("tm_zone".into(), i8_ptr_ty()),
        ],
        "lconv" => vec![
            ("decimal_point".into(), i8_ptr_ty()),
            ("thousands_sep".into(), i8_ptr_ty()),
            ("grouping".into(), i8_ptr_ty()),
            ("int_curr_symbol".into(), i8_ptr_ty()),
            ("currency_symbol".into(), i8_ptr_ty()),
            ("mon_decimal_point".into(), i8_ptr_ty()),
            ("mon_thousands_sep".into(), i8_ptr_ty()),
            ("mon_grouping".into(), i8_ptr_ty()),
            ("positive_sign".into(), i8_ptr_ty()),
            ("negative_sign".into(), i8_ptr_ty()),
            ("int_frac_digits".into(), i8_ty()),
            ("frac_digits".into(), i8_ty()),
            ("p_cs_precedes".into(), i8_ty()),
            ("p_sep_by_space".into(), i8_ty()),
            ("n_cs_precedes".into(), i8_ty()),
            ("n_sep_by_space".into(), i8_ty()),
            ("p_sign_posn".into(), i8_ty()),
            ("n_sign_posn".into(), i8_ty()),
            ("int_p_cs_precedes".into(), i8_ty()),
            ("int_p_sep_by_space".into(), i8_ty()),
            ("int_n_cs_precedes".into(), i8_ty()),
            ("int_n_sep_by_space".into(), i8_ty()),
            ("int_p_sign_posn".into(), i8_ty()),
            ("int_n_sign_posn".into(), i8_ty()),
        ],
        _ => return None,
    };
    Some(
        fields
            .into_iter()
            .map(|(name, ty)| RecordField {
                comments: Vec::new(),
                name,
                ty,
            })
            .collect(),
    )
}

pub(super) fn standard_record_def(name: &str) -> RecordDef {
    RecordDef {
        comments: Vec::new(),
        vis: Visibility::Private,
        field_vis: Visibility::Private,
        is_union: false,
        allow_non_camel_case: true,
        name: name.to_string(),
        fields: standard_record_fields(name).unwrap_or_default(),
        packed: None,
        align: None,
    }
}

pub(super) fn standard_record_default_expr(ty: &str) -> Option<Expr> {
    let fields = standard_record_fields(ty)?;
    Some(Expr::StructLit {
        name: ty.into(),
        fields: fields
            .into_iter()
            .map(|field| (field.name.into_string(), default_value_for_type(&field.ty)))
            .collect(),
    })
}

pub(super) fn zero_for_cir_type(ty: &str) -> Expr {
    default_value_for_type(&rust_type(ty))
}

pub(super) fn default_value_for_type(ty: &Type) -> Expr {
    match ty {
        Type::Prim(Prim::Bool) => Expr::Value(RustValue::Bool(false)),
        Type::Prim(Prim::F32 | Prim::F64) => Expr::Value(0.0.into()),
        Type::Prim(Prim::F128) => Expr::HexFloat("0.0f128".into()),
        Type::Ptr { .. } => Expr::Value(RustValue::NullPtr),
        Type::FnPtr { .. } => Expr::Value(RustValue::None),
        _ => Expr::Value(RustValue::I64(0)),
    }
}

pub(super) fn expr_int_value(expr: &Expr) -> Option<i128> {
    match expr {
        Expr::Value(RustValue::I64(value)) => Some(i128::from(*value)),
        Expr::Value(RustValue::I128(value)) => Some(*value),
        _ => None,
    }
}

pub(super) fn bitfield_info_number(info: &str, key: &str) -> Option<u32> {
    let rest = info.split(key).nth(1)?;
    let end = rest
        .find(|c: char| !c.is_ascii_digit())
        .unwrap_or(rest.len());
    rest[..end].parse().ok()
}

pub(super) fn render_array_literal_expr(elems: &[Expr], len: usize, default: Expr) -> Expr {
    let mut out: Vec<Expr> = elems.iter().take(len).cloned().collect();
    out.resize(len, default);
    Expr::ArrayLit(out)
}

pub(super) fn vector_index_expr(base: Expr, index: u64) -> Expr {
    Expr::Index {
        base: Box::new(base),
        index: Box::new(Expr::Value(RustValue::Usize(index as usize))),
    }
}

pub(super) fn parse_cir_scalar_expr(s: &str) -> Option<Expr> {
    parse_cir_int(s)
        .map(int_value_expr)
        .or_else(|| parse_cir_uint128(s).map(|n| Expr::Value(RustValue::U128(n))))
        .or_else(|| parse_cir_fp_expr(s))
        .or_else(|| parse_cir_bool(s).map(|b| Expr::Value(RustValue::Bool(b))))
        .or_else(|| {
            s.trim_start()
                .starts_with("#cir.ptr<null>")
                .then_some(Expr::Value(RustValue::NullPtr))
        })
}

pub(super) fn parse_cir_const_vector(s: &str) -> Option<Expr> {
    let s = s.trim_start();
    let start = s.find("#cir.const_vector<[")?;
    let rest = &s[start + "#cir.const_vector<[".len()..];
    let close = rest.find("]>")?;
    Some(Expr::ArrayLit(
        split_top_level(&rest[..close], ',')
            .into_iter()
            .map(str::trim)
            .filter(|part| !part.is_empty())
            .map(parse_cir_scalar_expr)
            .collect::<Option<Vec<_>>>()?,
    ))
}

pub(super) fn parse_cir_global_view(s: &str) -> Option<&str> {
    let s = s.trim_start().strip_prefix("#cir.global_view<@")?;
    let end = s.find('>')?;
    Some(s[..end].trim_matches('"'))
}

pub(super) fn parse_cir_global_views(s: &str) -> Vec<&str> {
    s.match_indices("#cir.global_view<@")
        .filter_map(|(start, _)| parse_cir_global_view(&s[start..]))
        .collect()
}

pub(super) fn parse_cir_global_view_array(s: &str) -> Vec<&str> {
    let s = s.trim_start();
    if !s.starts_with("#cir.const_array<[") {
        return Vec::new();
    }
    let Some(open) = s.find('[') else {
        return Vec::new();
    };
    let Some(close) = s.rfind(']') else {
        return Vec::new();
    };
    split_top_level(&s[open + 1..close], ',')
        .into_iter()
        .filter_map(parse_cir_global_view)
        .collect()
}

pub(super) fn int_value_expr(n: i128) -> Expr {
    Expr::Value(match i64::try_from(n) {
        Ok(n) => RustValue::I64(n),
        Err(_) => RustValue::I128(n),
    })
}

pub(super) fn int_pattern(n: i128) -> Pattern {
    match i64::try_from(n) {
        Ok(n) => Pattern::I64(n),
        Err(_) => Pattern::I128(n),
    }
}

pub(super) fn parse_cir_fp_expr(s: &str) -> Option<Expr> {
    parse_cir_fp(s).map(fp_literal_expr)
}

pub(super) fn parse_cir_f128_expr(s: &str) -> Option<Expr> {
    let fp = cir_fp_text(s)?;
    if let Some(hex) = fp.strip_prefix("0x").or_else(|| fp.strip_prefix("0X")) {
        let bits = u128::from_str_radix(hex, 16).ok()?;
        Some(Expr::HexFloat(format!("f128::from_bits(0x{bits:032x})")))
    } else {
        Some(Expr::HexFloat(format!("{fp}f128")))
    }
}

pub(super) fn fp_literal_expr(fp: String) -> Expr {
    fp.parse::<f64>()
        .map(|n| Expr::Value(n.into()))
        .unwrap_or_else(|_| Expr::HexFloat(fp))
}

pub(super) fn typed_fp_literal_expr(ty: Option<&Type>, fp: String) -> Expr {
    let value = fp_literal_expr(fp);
    if matches!(ty, Some(Type::LongDouble)) {
        Expr::Call {
            binding: crate::function_identity::CallBinding::Generated,
            func: Box::new(Expr::Var(LONG_DOUBLE_TY.into())),
            args: vec![value],
        }
    } else {
        value
    }
}

pub(super) enum CirComplexComponent {
    Int(i128),
    Uint(u128),
    Float(String),
}

pub(super) fn complex_const_expr(
    ty: Option<&Type>,
    re: CirComplexComponent,
    im: CirComplexComponent,
) -> Expr {
    let inner = match ty {
        Some(Type::Complex(inner)) => Some(inner.as_ref()),
        _ => None,
    };
    Expr::StructLit {
        name: "Complex".into(),
        fields: vec![
            ("re".into(), complex_component_expr(inner, re)),
            ("im".into(), complex_component_expr(inner, im)),
        ],
    }
}

pub(super) fn complex_component_expr(ty: Option<&Type>, component: CirComplexComponent) -> Expr {
    match component {
        CirComplexComponent::Int(value) => int_value_expr(value),
        CirComplexComponent::Uint(value) => Expr::Value(RustValue::U128(value)),
        CirComplexComponent::Float(value) => typed_fp_literal_expr(ty, value),
    }
}

pub(super) fn cir_int_digits(s: &str) -> Option<&str> {
    let start = s.find("#cir.int<")? + "#cir.int<".len();
    let rest = &s[start..];
    let end = rest.find('>')?;
    Some(&rest[..end])
}

// i128 so a full-range `!u64i` value (e.g. SIG_ERR = (void(*)(int))-1, which CIR
// prints as the unsigned bit pattern 18446744073709551615) survives as a valid
// unsigned literal rather than overflowing i64 and collapsing to 0.
pub(super) fn parse_cir_int(s: &str) -> Option<i128> {
    cir_int_digits(s)?.parse().ok()
}

// separate from parse_cir_int: a u128 constant above i128::MAX (e.g. near
// UINT128_MAX) doesn't fit in i128, so this is the fallback for scalar-expr
// construction, not for callers that need a plain i128 (offsets, lengths, ...).
pub(super) fn parse_cir_uint128(s: &str) -> Option<u128> {
    cir_int_digits(s)?.parse().ok()
}

pub(super) fn parse_cir_bool(s: &str) -> Option<bool> {
    match s.trim() {
        "#true" => return Some(true),
        "#false" => return Some(false),
        _ => {}
    }
    let start = s.find("#cir.bool<")? + "#cir.bool<".len();
    let rest = &s[start..];
    let end = rest.find('>')?;
    match rest[..end].trim() {
        "true" => Some(true),
        "false" => Some(false),
        _ => None,
    }
}

pub(super) fn parse_cir_fp(s: &str) -> Option<String> {
    let text = cir_fp_text(s)?;
    if text.starts_with("0x") || text.starts_with("0X") {
        let bits = u64::from_str_radix(&text[2..], 16).ok()?;
        return match text.len() - 2 {
            8 => Some(format!("f32::from_bits(0x{bits:08x})")),
            16 => Some(format!("f64::from_bits(0x{bits:016x})")),
            _ => None,
        };
    }
    Some(text.to_string())
}

pub(super) fn cir_fp_text(s: &str) -> Option<&str> {
    let start = s.find("#cir.fp<")? + "#cir.fp<".len();
    let rest = &s[start..];
    let end = rest.find('>')?;
    Some(rest[..end].trim())
}

pub(super) fn parse_cir_const_complex(
    s: &str,
) -> Option<(CirComplexComponent, CirComplexComponent)> {
    let start = s.find("#cir.const_complex<")? + "#cir.const_complex<".len();
    let inner = &s[start..];
    let parts = split_top_level(inner, ',');
    let re = parse_cir_complex_component(parts.first()?.trim())?;
    let im = parse_cir_complex_component(parts.get(1)?.trim())?;
    Some((re, im))
}

pub(super) fn parse_cir_complex_component(s: &str) -> Option<CirComplexComponent> {
    parse_cir_int(s)
        .map(CirComplexComponent::Int)
        .or_else(|| parse_cir_uint128(s).map(CirComplexComponent::Uint))
        .or_else(|| parse_cir_fp(s).map(CirComplexComponent::Float))
}

pub(super) fn parse_cir_const_array(s: &str) -> Option<Vec<u8>> {
    let s = s.trim_start();
    if !s.starts_with("#cir.const_array<\"") {
        return None;
    }
    let start = s.find('"')? + 1;
    let rest = &s[start..];
    let end = rest.find('"')?;
    Some(decode_cir_string(&rest[..end]))
}

/// Parse the numeric form `#cir.const_array<[#cir.int<1> : !s32i, ...]>` into
/// per-element Rust literals. Returns `None` for the string form (handled by
/// [`parse_cir_const_array`]) or any element we cannot render.
pub(super) fn parse_cir_const_array_elems(s: &str) -> Option<Vec<Expr>> {
    let s = s.trim_start();
    if !s.starts_with("#cir.const_array<[") {
        return None;
    }
    let open = s.find('[')?;
    let close = s.rfind(']')?;
    let inner = &s[open + 1..close];
    split_top_level(inner, ',')
        .into_iter()
        .map(str::trim)
        .filter(|part| !part.is_empty())
        .map(|part| {
            if is_cir_aggregate_init(part) {
                return None; // array of aggregates → render_const_value handles it
            }
            parse_cir_scalar_expr(part)
        })
        .collect()
}

pub(super) fn parse_cir_block_addr_labels(s: &str) -> Option<Vec<String>> {
    let s = s.trim_start();
    if !s.starts_with("#cir.const_array<[") {
        return None;
    }
    let open = s.find('[')?;
    let close = s.rfind(']')?;
    let mut labels = Vec::new();
    for part in split_top_level(&s[open + 1..close], ',') {
        let part = part.trim();
        if part.is_empty() {
            continue;
        }
        if !part.contains("#cir.block_addr_info<") {
            return None;
        }
        let start = part.find('"')? + 1;
        let rest = &part[start..];
        let end = rest.find('"')?;
        labels.push(rest[..end].to_string());
    }
    (!labels.is_empty()).then_some(labels)
}

/// A `cir.global` initializer that is a struct/union or nested-aggregate array,
/// rendered on demand by [`FunctionLowerer::render_const_value_expr`].
pub(super) fn is_cir_aggregate_init(raw: &str) -> bool {
    let raw = raw.trim_start();
    raw.starts_with("#cir.const_record<") || raw.starts_with("#cir.const_array<[")
}

pub(super) fn decode_cir_string(s: &str) -> Vec<u8> {
    let mut bytes = Vec::new();
    let mut chars = s.chars().peekable();
    while let Some(c) = chars.next() {
        if c != '\\' {
            bytes.push(c as u8);
            continue;
        }
        if chars.peek() == Some(&'\\') {
            chars.next();
            bytes.push(b'\\');
            continue;
        }
        let mut hex = String::new();
        while hex.len() < 2 && chars.peek().is_some_and(|c| c.is_ascii_hexdigit()) {
            hex.push(chars.next().unwrap());
        }
        if hex.is_empty() {
            bytes.push(b'\\');
        } else if let Ok(value) = u8::from_str_radix(&hex, 16) {
            bytes.push(value);
        }
    }
    bytes
}

pub(super) fn sanitize_ident(s: &str) -> Ident {
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
    // `crate`/`self`/`Self`/`super` can't be raw identifiers, so mangle them instead.
    if matches!(out.as_str(), "crate" | "self" | "Self" | "super") {
        out.push('_');
    }
    Ident::from(out)
}

pub(super) fn split_top_level_arrow(s: &str) -> Option<(&str, &str)> {
    let mut angle = 0usize;
    let mut paren = 0usize;
    let bytes = s.as_bytes();
    let mut i = 0usize;
    while i + 1 < bytes.len() {
        match bytes[i] as char {
            '<' => angle += 1,
            '>' if i > 0 && bytes[i - 1] == b'-' => {}
            '>' => angle = angle.saturating_sub(1),
            '(' => paren += 1,
            ')' => paren = paren.saturating_sub(1),
            '-' if bytes[i + 1] == b'>' && angle == 0 && paren == 0 => {
                return Some((&s[..i], &s[i + 2..]));
            }
            _ => {}
        }
        i += 1;
    }
    None
}

pub(super) fn split_top_level(s: &str, delimiter: char) -> Vec<&str> {
    let mut parts = Vec::new();
    let mut start = 0usize;
    let mut angle = 0usize;
    let mut paren = 0usize;
    let mut prev = '\0';
    for (i, c) in s.char_indices() {
        match c {
            '<' => angle += 1,
            '>' if prev == '-' => {}
            '>' => angle = angle.saturating_sub(1),
            '(' => paren += 1,
            ')' => paren = paren.saturating_sub(1),
            c if c == delimiter && angle == 0 && paren == 0 => {
                parts.push(&s[start..i]);
                start = i + c.len_utf8();
            }
            _ => {}
        }
        prev = c;
    }
    parts.push(&s[start..]);
    parts
}
