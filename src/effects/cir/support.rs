use crate::cir::ir::{Attr, Op};
use crate::effects::{IntWidth, Value};
use crate::rust_ast::{AtomicOrdering, AtomicRmwOp};
use std::collections::BTreeMap;

pub(super) fn values_to_bytes(values: &[Value]) -> Vec<u8> {
    values.iter().map(|value| value_as_u8(*value)).collect()
}

pub(super) fn first_result(op: &Op) -> &str {
    op.results.first().map(String::as_str).unwrap_or_default()
}

pub(super) fn attr_str<'a>(op: &'a Op, key: &str) -> Option<&'a str> {
    op.attrs.get(key).and_then(Attr::as_str)
}

pub(super) fn attr_int(op: &Op, key: &str) -> Option<i64> {
    op.attrs.get(key).and_then(Attr::as_int)
}

pub(super) fn result_type(op: &Op) -> Option<&str> {
    let ty = op.ty.as_deref()?;
    let idx = ty.rfind("->")?;
    Some(ty[idx + 2..].trim())
}

pub(super) fn result_type_for_operand(ty: Option<&str>, operand: usize) -> Option<&str> {
    let ty = ty?;
    let params = ty.strip_prefix('(')?.split(") -> ").next()?;
    split_top_level(params, ',').get(operand).copied()
}

pub(super) fn int_type_width_signed(ty: &str) -> Option<(bool, u32)> {
    let rest = ty.trim().strip_prefix('!')?;
    let signed = match rest.as_bytes().first()? {
        b's' => true,
        b'u' => false,
        _ => return None,
    };
    let bits: u32 = rest[1..].strip_suffix('i')?.parse().ok()?;
    Some((signed, bits))
}

pub(super) fn int_width(bits: u32) -> IntWidth {
    match bits {
        8 => IntWidth::W8,
        16 => IntWidth::W16,
        32 => IntWidth::W32,
        64 => IntWidth::W64,
        128 => IntWidth::W128,
        _ => IntWidth::PointerSized,
    }
}

pub(super) fn rust_ordering(mem_order: i64) -> AtomicOrdering {
    match mem_order {
        0 => AtomicOrdering::Relaxed,
        1 | 2 => AtomicOrdering::Acquire,
        3 => AtomicOrdering::Release,
        4 => AtomicOrdering::AcqRel,
        _ => AtomicOrdering::SeqCst,
    }
}

pub(super) fn load_ordering(mem_order: i64) -> AtomicOrdering {
    match mem_order {
        0 => AtomicOrdering::Relaxed,
        1 | 2 => AtomicOrdering::Acquire,
        _ => AtomicOrdering::SeqCst,
    }
}

pub(super) fn store_ordering(mem_order: i64) -> AtomicOrdering {
    match mem_order {
        0 => AtomicOrdering::Relaxed,
        3 => AtomicOrdering::Release,
        _ => AtomicOrdering::SeqCst,
    }
}

pub(super) fn atomic_rmw_op(binop: i64) -> AtomicRmwOp {
    match binop {
        0 => AtomicRmwOp::Add,
        1 => AtomicRmwOp::Sub,
        2 => AtomicRmwOp::And,
        3 => AtomicRmwOp::Xor,
        4 => AtomicRmwOp::Or,
        5 => AtomicRmwOp::Nand,
        6 => AtomicRmwOp::Max,
        _ => AtomicRmwOp::Min,
    }
}

pub(super) fn atomic_rmw_value(op: AtomicRmwOp, old: Value, operand: Value) -> Value {
    let (width, signed) = match old {
        Value::Int { width, signed, .. } => (width, signed),
        other => panic!("effects::cir: expected atomic int value, found {other:?}"),
    };
    let Value::Int { value: old, .. } = old else {
        unreachable!();
    };
    let Value::Int { value: operand, .. } = operand else {
        panic!("effects::cir: expected atomic int operand");
    };
    let value = match op {
        AtomicRmwOp::Add => old.wrapping_add(operand),
        AtomicRmwOp::Sub => old.wrapping_sub(operand),
        AtomicRmwOp::And => old & operand,
        AtomicRmwOp::Xor => old ^ operand,
        AtomicRmwOp::Or => old | operand,
        AtomicRmwOp::Nand => !(old & operand),
        AtomicRmwOp::Max => old.max(operand),
        AtomicRmwOp::Min => old.min(operand),
    };
    Value::Int {
        width,
        signed,
        value,
    }
}

pub(super) fn const_value(raw: &str, ty: Option<&str>) -> Value {
    if raw.contains("#cir.ptr<null>") {
        return Value::Null;
    }
    int_const_value(raw, ty)
}

pub(super) fn int_const_value(raw: &str, ty: Option<&str>) -> Value {
    let start = raw
        .find("#cir.int<")
        .expect("cir.const: expected #cir.int<>");
    let rest = &raw[start + "#cir.int<".len()..];
    let end = rest.find('>').expect("cir.const: unterminated #cir.int<>");
    let value: i128 = rest[..end].parse().expect("cir.const: non-integer literal");
    let (signed, bits) = ty.and_then(int_type_width_signed).unwrap_or((true, 32));
    Value::Int {
        width: int_width(bits),
        signed,
        value,
    }
}

pub(super) fn global_const_array_values(op: &Op) -> Option<Vec<Value>> {
    let raw = attr_str(op, "initial_value")?;
    let ty = attr_str(op, "sym_type")?;
    let (elem_ty, len) = cir_array_ty(ty)?;
    if let Some(bytes) = parse_cir_const_string_array(raw) {
        let (signed, bits) = int_type_width_signed(elem_ty)?;
        let mut bytes = bytes;
        bytes.resize(len, 0);
        return Some(
            bytes
                .into_iter()
                .take(len)
                .map(|byte| Value::Int {
                    width: int_width(bits),
                    signed,
                    value: byte as i128,
                })
                .collect(),
        );
    }
    parse_cir_const_numeric_array(raw, elem_ty)
}

pub(super) fn global_const_aggregate_values(
    op: &Op,
    aliases: &BTreeMap<String, String>,
) -> Option<Vec<(u64, Value)>> {
    let raw = attr_str(op, "initial_value")?;
    let ty = attr_str(op, "sym_type")?;
    let mut values = Vec::new();
    collect_const_aggregate_values(raw, ty, aliases, 0, &mut values)?;
    Some(values)
}

fn collect_const_aggregate_values(
    raw: &str,
    ty: &str,
    aliases: &BTreeMap<String, String>,
    base: u64,
    out: &mut Vec<(u64, Value)>,
) -> Option<()> {
    if int_type_width_signed(ty).is_some() {
        out.push((base, int_const_value(raw, Some(ty))));
        return Some(());
    }
    let ty = expand_type_alias(ty, aliases);
    if let Some((elem_ty, _)) = cir_array_ty(ty) {
        for (index, elem_raw) in const_aggregate_elements(raw).into_iter().enumerate() {
            let offset = base + index as u64 * cir_type_size(elem_ty, aliases)?;
            collect_const_aggregate_values(elem_raw, elem_ty, aliases, offset, out)?;
        }
        return Some(());
    }
    let fields = cir_struct_fields(ty, aliases)?;
    for (index, elem_raw) in const_aggregate_elements(raw).into_iter().enumerate() {
        let field_ty = fields.get(index).copied()?;
        let offset = base + cir_struct_field_offset(ty, index, aliases)?;
        collect_const_aggregate_values(elem_raw, field_ty, aliases, offset, out)?;
    }
    Some(())
}

fn const_aggregate_elements(raw: &str) -> Vec<&str> {
    let Some(start) = raw.find("<{").or_else(|| raw.find("<[")) else {
        return Vec::new();
    };
    let open = raw.as_bytes()[start + 1] as char;
    let close = if open == '{' { '}' } else { ']' };
    let inner_start = start + 2;
    let mut depth = 0usize;
    for (rel, ch) in raw[inner_start..].char_indices() {
        match ch {
            '<' => depth += 1,
            '>' => depth = depth.saturating_sub(1),
            _ => {}
        }
        if ch == close && depth == 0 {
            return split_top_level(&raw[inner_start..inner_start + rel], ',');
        }
    }
    Vec::new()
}

pub(super) fn parse_cir_const_string_array(raw: &str) -> Option<Vec<u8>> {
    let start = raw.find("#cir.const_array<\"")? + "#cir.const_array<\"".len();
    let rest = &raw[start..];
    let end = rest.find('"')?;
    let mut bytes = decode_cir_string(&rest[..end]);
    if raw.contains("trailing_zeros") {
        bytes.push(0);
    }
    Some(bytes)
}

pub(super) fn parse_cir_const_numeric_array(raw: &str, elem_ty: &str) -> Option<Vec<Value>> {
    if elem_ty.trim().starts_with("!cir.ptr<") {
        return parse_cir_block_address_array(raw);
    }
    let start = raw.find("#cir.const_array<[")? + "#cir.const_array<[".len();
    let rest = &raw[start..];
    let end = rest.find("]>")?;
    let (signed, bits) = int_type_width_signed(elem_ty)?;
    rest[..end]
        .split("#cir.int<")
        .skip(1)
        .map(|part| {
            let end = part.find('>')?;
            let value = part[..end].parse::<i128>().ok()?;
            Some(Value::Int {
                width: int_width(bits),
                signed,
                value,
            })
        })
        .collect::<Option<Vec<_>>>()
}

fn parse_cir_block_address_array(raw: &str) -> Option<Vec<Value>> {
    let mut values = Vec::new();
    let mut rest = raw;
    while let Some(start) = rest.find("#cir.block_addr_info<") {
        rest = &rest[start + "#cir.block_addr_info<".len()..];
        let label_start = rest.find('"')? + 1;
        let label_rest = &rest[label_start..];
        let label_end = label_rest.find('"')?;
        let label = Box::leak(label_rest[..label_end].to_string().into_boxed_str());
        values.push(Value::BlockLabel(label));
        rest = &label_rest[label_end + 1..];
    }
    (!values.is_empty()).then_some(values)
}

pub(super) fn decode_cir_string(s: &str) -> Vec<u8> {
    let mut bytes = Vec::new();
    let mut chars = s.chars().peekable();
    while let Some(ch) = chars.next() {
        if ch == '\\' {
            let Some(a) = chars.next() else {
                break;
            };
            let Some(b) = chars.next() else {
                bytes.push(a as u8);
                break;
            };
            if let (Some(hi), Some(lo)) = (a.to_digit(16), b.to_digit(16)) {
                bytes.push((hi * 16 + lo) as u8);
            } else {
                bytes.push(a as u8);
                bytes.push(b as u8);
            }
        } else {
            bytes.push(ch as u8);
        }
    }
    bytes
}

pub(super) fn cir_array_ty(ty: &str) -> Option<(&str, usize)> {
    let inner = ty.trim().strip_prefix("!cir.array<")?.strip_suffix('>')?;
    let (elem, len) = inner.rsplit_once(" x ")?;
    Some((elem.trim(), len.trim().parse().ok()?))
}

pub(super) fn int_byte_size(value: &Value) -> u64 {
    match value {
        Value::Int { width, .. } => match width {
            IntWidth::W8 => 1,
            IntWidth::W16 => 2,
            IntWidth::W32 => 4,
            IntWidth::W64 | IntWidth::PointerSized => 8,
            IntWidth::W128 => 16,
        },
        Value::BlockLabel(_) | Value::Ref(_) | Value::Null => 8,
        other => panic!("effects::cir: buffer element must be an integer, found {other:?}"),
    }
}

pub(super) fn value_as_u64(value: Value) -> u64 {
    match value {
        Value::Int { value, .. } => value as u64,
        other => panic!("effects::cir: expected integer value, found {other:?}"),
    }
}

pub(super) fn value_as_i128(value: Value) -> i128 {
    match value {
        Value::Int { value, .. } => value,
        other => panic!("effects::cir: expected integer value, found {other:?}"),
    }
}

pub(super) fn truncate_bitfield(
    value: i128,
    bits: u32,
    signed: bool,
    result_ty: Option<&str>,
) -> Value {
    let mask = (1i128 << bits) - 1;
    let mut value = value & mask;
    if signed {
        let sign_bit = 1i128 << (bits - 1);
        if value & sign_bit != 0 {
            value |= !mask;
        }
    }
    let (result_signed, result_bits) = result_ty
        .and_then(int_type_width_signed)
        .unwrap_or((signed, bits));
    Value::Int {
        width: int_width(result_bits),
        signed: result_signed,
        value,
    }
}

pub(super) fn bitfield_slot_size(op: &Op) -> u64 {
    result_type(op)
        .and_then(int_type_width_signed)
        .map(|(_, bits)| (bits / 8) as u64)
        .unwrap_or(4)
}

pub(super) fn value_as_u8(value: Value) -> u8 {
    match value {
        Value::Int { value, .. } => value as u8,
        other => panic!("effects::cir: expected byte value, found {other:?}"),
    }
}

pub(super) fn pointee_byte_size(ptr_ty: Option<&str>) -> Option<u64> {
    let inner = ptr_ty?
        .trim()
        .strip_prefix("!cir.ptr<")?
        .strip_suffix('>')?;
    if inner.trim().starts_with("!cir.ptr<") {
        return Some(8);
    }
    let (_, bits) = int_type_width_signed(inner)?;
    Some((bits / 8) as u64)
}

pub(super) fn expand_type_alias<'a>(ty: &'a str, aliases: &'a BTreeMap<String, String>) -> &'a str {
    aliases.get(ty.trim()).map_or(ty, String::as_str)
}

pub(super) fn cir_type_size(ty: &str, aliases: &BTreeMap<String, String>) -> Option<u64> {
    let ty = ty.trim();
    if let Some((_, bits)) = int_type_width_signed(ty) {
        return Some((bits / 8) as u64);
    }
    let ty = expand_type_alias(ty, aliases).trim();
    if ty.starts_with("!cir.ptr<") {
        return Some(8);
    }
    if let Some((elem, len)) = cir_array_ty(ty) {
        return Some(cir_type_size(elem, aliases)? * len as u64);
    }
    let fields = cir_struct_fields(ty, aliases)?;
    fields
        .into_iter()
        .map(|field| cir_type_size(field, aliases))
        .sum()
}

pub(super) fn cir_is_aggregate_type(ty: &str, aliases: &BTreeMap<String, String>) -> bool {
    let ty = expand_type_alias(ty.trim(), aliases).trim();
    ty.starts_with("!cir.struct<") || ty.starts_with("!cir.array<")
}

pub(super) fn cir_ptr_pointee<'a>(
    ty: &'a str,
    aliases: &'a BTreeMap<String, String>,
) -> Option<&'a str> {
    expand_type_alias(ty.trim(), aliases)
        .trim()
        .strip_prefix("!cir.ptr<")?
        .strip_suffix('>')
}

pub(super) fn cir_struct_field_offset(
    record_ty: &str,
    index: usize,
    aliases: &BTreeMap<String, String>,
) -> Option<u64> {
    let fields = cir_struct_fields(record_ty, aliases)?;
    fields
        .into_iter()
        .take(index)
        .map(|field| cir_type_size(field, aliases))
        .sum()
}

pub(super) fn cir_struct_fields<'a>(
    ty: &'a str,
    aliases: &'a BTreeMap<String, String>,
) -> Option<Vec<&'a str>> {
    let ty = expand_type_alias(ty.trim(), aliases).trim();
    let start = ty.find('{')?;
    let end = ty.rfind('}')?;
    Some(split_top_level(&ty[start + 1..end], ','))
}

pub(super) fn pointee_type_size(
    ptr_ty: Option<&str>,
    aliases: &BTreeMap<String, String>,
) -> Option<u64> {
    let pointee = cir_ptr_pointee(ptr_ty?, aliases)?;
    cir_type_size(pointee, aliases)
}

pub(super) fn bitfield_info(raw: &str, aliases: &BTreeMap<String, String>) -> BitfieldInfo {
    let raw = aliases.get(raw.trim()).map_or(raw, String::as_str);
    let name = parse_named_attr(raw, "name")
        .unwrap_or_default()
        .trim_matches('"')
        .to_string();
    let size = parse_named_attr(raw, "size")
        .and_then(|value| value.parse().ok())
        .expect("cir.bitfield_info: missing size");
    let offset = parse_named_attr(raw, "offset")
        .and_then(|value| value.parse().ok())
        .expect("cir.bitfield_info: missing offset");
    let signed = parse_named_attr(raw, "is_signed") == Some("true");
    BitfieldInfo {
        name,
        size,
        offset,
        signed,
    }
}

pub(super) struct BitfieldInfo {
    pub(super) name: String,
    pub(super) size: u32,
    pub(super) offset: u32,
    pub(super) signed: bool,
}

fn parse_named_attr<'a>(raw: &'a str, key: &str) -> Option<&'a str> {
    let start = raw.find(&format!("{key} = "))? + key.len() + 3;
    let rest = &raw[start..];
    let end = rest.find(',').unwrap_or(rest.len());
    Some(rest[..end].trim().trim_end_matches('>'))
}

pub(super) fn split_top_level(s: &str, sep: char) -> Vec<&str> {
    let mut out = Vec::new();
    let mut start = 0usize;
    let mut angle = 0usize;
    for (idx, ch) in s.char_indices() {
        match ch {
            '<' => angle += 1,
            '>' => angle = angle.saturating_sub(1),
            ch if ch == sep && angle == 0 => {
                out.push(s[start..idx].trim());
                start = idx + ch.len_utf8();
            }
            _ => {}
        }
    }
    out.push(s[start..].trim());
    out
}
