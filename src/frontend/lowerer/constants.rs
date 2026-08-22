use super::*;
use rustc_apfloat::Float;
use std::collections::HashMap;
use std::sync::{Mutex, OnceLock};

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

pub(super) fn cir_string_bytes(text: &str) -> Option<Vec<u8>> {
    text.chars()
        .map(|ch| u8::try_from(u32::from(ch)).ok())
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

pub(super) fn zero_for_cir_type(ty: &CirType) -> Expr {
    default_value_for_type(&rust_type(ty))
}

pub(super) fn default_value_for_type(ty: &Type) -> Expr {
    if let Some(expr) = bitint_zero_expr(ty) {
        return expr;
    }
    match ty {
        Type::Prim(Prim::Bool) => Expr::Value(RustValue::Bool(false)),
        Type::Prim(Prim::F32 | Prim::F64) => Expr::Value(0.0.into()),
        Type::Prim(Prim::F16) => Expr::HexFloat("0.0f16".into()),
        Type::Prim(Prim::F128) => Expr::HexFloat("0.0f128".into()),
        Type::LongDouble => long_double_zero_expr(),
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

pub(super) fn fp_literal_expr(fp: String) -> Expr {
    fp.parse::<f64>()
        .map(|n| Expr::Value(n.into()))
        .unwrap_or_else(|_| Expr::HexFloat(fp))
}

pub(super) fn fp_literal_expr_for_type(ty: Option<&Type>, fp: String) -> Expr {
    if matches!(ty, Some(Type::LongDouble)) && !crate::cir::emit::uses_f64_long_double_abi() {
        f80_literal_expr(&fp).unwrap_or_else(|| {
            let value = fp_literal_expr(fp);
            Expr::Call {
                binding: crate::function_identity::CallBinding::Generated,
                func: Box::new(Expr::Var("__slate_f80_from_f64".into())),
                args: vec![value],
            }
        })
    } else {
        fp_literal_expr(fp)
    }
}

fn f80_cache() -> &'static Mutex<HashMap<String, [u8; 10]>> {
    static CACHE: OnceLock<Mutex<HashMap<String, [u8; 10]>>> = OnceLock::new();
    CACHE.get_or_init(|| Mutex::new(HashMap::new()))
}

fn host_f80_bytes(fp: &str) -> Option<[u8; 10]> {
    if let Some(cached) = f80_cache().lock().ok()?.get(fp).copied() {
        return Some(cached);
    }
    let bits = fp
        .parse::<rustc_apfloat::ieee::X87DoubleExtended>()
        .ok()?
        .to_bits();
    let mut bytes = [0u8; 10];
    bytes.copy_from_slice(&bits.to_be_bytes()[6..]);
    bytes.reverse();
    f80_cache().lock().ok()?.insert(fp.to_string(), bytes);
    Some(bytes)
}

pub(super) fn f80_literal_expr(fp: &str) -> Option<Expr> {
    let bytes = host_f80_bytes(fp)?;
    Some(f80_bytes_expr(&bytes))
}

pub(super) fn f80_literal_bits_expr(bits: &str) -> Option<Expr> {
    let bits = u128::from_str_radix(bits, 16).ok()?;
    let mut bytes = [0u8; 10];
    bytes.copy_from_slice(&bits.to_be_bytes()[6..]);
    bytes.reverse();
    Some(f80_bytes_expr(&bytes))
}

fn f80_bytes_expr(bytes: &[u8; 10]) -> Expr {
    let elems = byte_array_elems(
        bytes,
        &Type::Array {
            elem: Box::new(Type::Prim(Prim::U8)),
            len: 10,
        },
    );
    Expr::TupleStructLit {
        name: LONG_DOUBLE_TY.into(),
        fields: vec![Expr::ArrayLit(elems)],
    }
}

pub(super) enum CirComplexComponent {
    Int(i128),
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
        name: COMPLEX_TY.into(),
        fields: vec![
            ("re".into(), complex_component_expr(inner, re)),
            ("im".into(), complex_component_expr(inner, im)),
        ],
    }
}

pub(super) fn complex_component_expr(ty: Option<&Type>, component: CirComplexComponent) -> Expr {
    match component {
        CirComplexComponent::Int(value) => int_value_expr(value),
        CirComplexComponent::Float(value) => fp_literal_expr_for_type(ty, value),
    }
}

pub(super) fn complex_component_from_attr(attr: &Attr) -> Option<CirComplexComponent> {
    match attr {
        Attr::Int { value, .. } => Some(CirComplexComponent::Int(*value)),
        Attr::CirInt { value, .. } => value.parse().ok().map(CirComplexComponent::Int),
        Attr::Float { text, .. } => fp_text_value(text).map(CirComplexComponent::Float),
        Attr::CirFloat { value, .. } => fp_text_value(value).map(CirComplexComponent::Float),
        _ => None,
    }
}

/// Renders a scalar (non-aggregate) attribute directly: `#cir.int<>`,
/// `#cir.fp<>`, `#cir.bool<>`/`#true`/`#false`, and `#cir.ptr<null>` -
/// other `!cir.ptr` forms, e.g. int-to-pointer casts, need the destination
/// type and are handled by the caller instead.
pub(super) fn scalar_attr_expr(attr: &Attr) -> Option<Expr> {
    match attr {
        Attr::Int { value, .. } => Some(int_value_expr(*value)),
        Attr::CirInt { value, .. } => value.parse().ok().map(int_value_expr),
        Attr::Float { text, .. } => fp_text_value(text).map(fp_literal_expr),
        Attr::CirFloat { value, .. } => fp_text_value(value).map(fp_literal_expr),
        Attr::CirBool { value, .. } | Attr::Bool(value) => {
            Some(Expr::Value(RustValue::Bool(*value)))
        }
        Attr::ConstPtr { value, .. } if is_null_ptr_value(value) => {
            Some(Expr::Value(RustValue::NullPtr))
        }
        _ => None,
    }
}

/// `#cir.ptr<null>` parses to a zero `Attribute::Int` with no type suffix,
/// which is otherwise indistinguishable from a real `#cir.ptr<0 : ty>` -
/// MLIR's own printer renders both the same way, so this is the only signal.
pub(super) fn is_null_ptr_value(value: &Attr) -> bool {
    matches!(value, Attr::Int { value: 0, ty: None })
}

/// Renders a `#cir.fp<...>` literal's already-extracted text (e.g.
/// `Attribute::CirFloat::text`) as a Rust float literal or `fN::from_bits` call.
pub(super) fn fp_text_value(text: &str) -> Option<String> {
    let text = text.trim();
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

pub(super) fn decode_cir_string(s: &str) -> Vec<u8> {
    clang_ir::decode_escaped_bytes(s.as_bytes())
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
