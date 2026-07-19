use std::collections::HashMap;

use crate::effects::{IntWidth, Location, OptionValue, Value};
use crate::rust_ast::{
    AtomicOrdering, AtomicRmwOp, AtomicType, BinOp, Expr, Path, RustValue, Type,
};

pub(super) struct OpenEffect {
    pub(super) path: String,
    pub(super) mode: String,
}

pub(super) fn is_path(expr: &Expr, segments: &[&str]) -> bool {
    match expr {
        Expr::Path(Path { segments: actual }) => {
            actual.len() == segments.len()
                && actual.iter().zip(segments).all(|(a, b)| a.as_str() == *b)
        }
        Expr::Var(ident) => ident.as_str() == segments.join("::"),
        _ => false,
    }
}

pub(super) fn path_name(expr: &Expr) -> Option<String> {
    match expr {
        Expr::Path(Path { segments }) => Some(
            segments
                .iter()
                .map(|segment| segment.as_str())
                .collect::<Vec<_>>()
                .join("::"),
        ),
        Expr::Var(ident) => Some(ident.as_str().to_string()),
        _ => None,
    }
}

pub(super) fn recv_name(expr: &Expr) -> &str {
    match expr {
        Expr::Var(ident) => ident.as_str(),
        other => panic!("effects::rust_ast: unsupported atomic receiver `{other:?}`"),
    }
}

pub(super) enum AtomicPtrTarget<'a> {
    Local(&'a str),
    Field { base: &'a Expr, field: &'a str },
}

pub(super) fn atomic_ptr_target(expr: &Expr) -> AtomicPtrTarget<'_> {
    match expr {
        Expr::Cast { expr, .. } => atomic_ptr_target(expr),
        Expr::AddrOf { expr, .. } => atomic_ptr_place(expr),
        Expr::Macro { name, args } if name == "std::ptr::addr_of_mut" => {
            let [arg] = args.as_slice() else {
                panic!("effects::rust_ast: addr_of_mut! expects one argument");
            };
            atomic_ptr_place(arg)
        }
        other => panic!("effects::rust_ast: unsupported atomic pointer place `{other:?}`"),
    }
}

fn atomic_ptr_place(expr: &Expr) -> AtomicPtrTarget<'_> {
    match expr {
        Expr::Var(ident) => AtomicPtrTarget::Local(ident.as_str()),
        Expr::Field { base, field } => AtomicPtrTarget::Field { base, field },
        other => panic!("effects::rust_ast: unsupported atomic pointer place `{other:?}`"),
    }
}

pub(super) fn ordering_expr(expr: &Expr) -> AtomicOrdering {
    match expr {
        Expr::Path(path) => ordering_from_name(path.segments.last().map(|s| s.as_str())),
        Expr::Var(ident) => ordering_from_name(ident.as_str().rsplit("::").next()),
        other => panic!("effects::rust_ast: unsupported atomic ordering `{other:?}`"),
    }
}

pub(super) fn ordering_from_name(name: Option<&str>) -> AtomicOrdering {
    match name {
        Some("Relaxed") => AtomicOrdering::Relaxed,
        Some("Acquire") => AtomicOrdering::Acquire,
        Some("Release") => AtomicOrdering::Release,
        Some("AcqRel") => AtomicOrdering::AcqRel,
        Some("SeqCst") => AtomicOrdering::SeqCst,
        other => panic!("effects::rust_ast: unsupported atomic ordering `{other:?}`"),
    }
}

pub(super) fn open_effect(expr: &Expr) -> Option<OpenEffect> {
    match expr {
        Expr::MethodCall { recv, method, args }
            if matches!(method.as_str(), "unwrap" | "unwrap_or_else") =>
        {
            open_effect(recv)
        }
        Expr::MethodCall { recv, method, args } if method == "open" => {
            let [Expr::Str(path)] = args.as_slice() else {
                panic!("effects::rust_ast: OpenOptions::open expects a string literal path");
            };
            Some(OpenEffect {
                path: path.clone(),
                mode: open_options_mode(recv),
            })
        }
        _ => None,
    }
}

pub(super) fn open_options_mode(expr: &Expr) -> String {
    let mut read = false;
    let mut write = false;
    let mut append = false;
    let mut create = false;
    let mut truncate = false;
    let mut current = expr;
    loop {
        match current {
            Expr::Call { func, args }
                if args.is_empty() && is_path(func, &["std", "fs", "OpenOptions", "new"]) =>
            {
                break;
            }
            Expr::MethodCall { recv, method, args } => {
                let [Expr::Value(RustValue::Bool(value))] = args.as_slice() else {
                    panic!("effects::rust_ast: OpenOptions::{method} expects a bool literal");
                };
                match method.as_str() {
                    "read" => read = *value,
                    "write" => write = *value,
                    "append" => append = *value,
                    "create" => create = *value,
                    "truncate" => truncate = *value,
                    other => panic!("effects::rust_ast: unsupported OpenOptions method `{other}`"),
                }
                current = recv;
            }
            other => panic!("effects::rust_ast: unsupported OpenOptions chain `{other:?}`"),
        }
    }
    match (read, write, append, create, truncate) {
        (false, true, false, true, true) => "w",
        (true, false, false, false, false) => "r",
        (false, false, true, true, false) => "a",
        (true, true, false, false, false) => "r+",
        (true, true, false, true, true) => "w+",
        (true, false, true, true, false) => "a+",
        other => panic!("effects::rust_ast: unsupported OpenOptions mode {other:?}"),
    }
    .to_string()
}

pub(super) fn c_string_expr(expr: &Expr) -> String {
    String::from_utf8_lossy(&c_string_expr_bytes(expr)).into_owned()
}

pub(super) fn c_string_expr_bytes(expr: &Expr) -> Vec<u8> {
    let bytes = match expr {
        Expr::Cast { expr, .. } => return c_string_expr_bytes(expr),
        Expr::MethodCall { recv, method, args } if method == "as_ptr" && args.is_empty() => {
            c_string_expr_bytes(recv)
        }
        Expr::ByteStr(bytes) => bytes.clone(),
        Expr::CStr(bytes) => {
            let mut bytes = bytes.clone();
            bytes.push(0);
            bytes
        }
        other => panic!("effects::rust_ast: unsupported C string expression `{other:?}`"),
    };
    bytes.into_iter().take_while(|byte| *byte != 0).collect()
}

pub(super) fn array_pointer_name(expr: &Expr) -> &str {
    match expr {
        Expr::Cast { expr, .. } => array_pointer_name(expr),
        Expr::MethodCall { recv, method, args } if method == "as_mut_ptr" && args.is_empty() => {
            collection_name(recv)
        }
        Expr::ArrayPtr { array, .. } => collection_name(array),
        other => panic!("effects::rust_ast: unsupported array pointer argument `{other:?}`"),
    }
}

pub(super) fn comparator_name(expr: &Expr) -> &str {
    match expr {
        Expr::Call { func, args } if is_path(func, &["Some"]) => {
            let [arg] = args.as_slice() else {
                panic!("effects::rust_ast: Some comparator expects one argument");
            };
            comparator_name(arg)
        }
        Expr::Var(ident) => ident.as_str(),
        other => panic!("effects::rust_ast: unsupported comparator argument `{other:?}`"),
    }
}

pub(super) fn restore_scalar(
    scalars: &mut HashMap<String, Value>,
    name: &str,
    previous: Option<Value>,
) {
    match previous {
        Some(value) => {
            scalars.insert(name.to_string(), value);
        }
        None => {
            scalars.remove(name);
        }
    }
}

pub(super) fn value_as_i32(value: impl std::borrow::Borrow<Value>) -> i32 {
    match value.borrow() {
        Value::Int { value, .. } => *value as i32,
        other => panic!("effects::rust_ast: expected an integer exit code, found {other:?}"),
    }
}

pub(super) fn rust_value_to_value(rv: &RustValue) -> Value {
    match rv {
        RustValue::I64(v) => int32(*v as i128),
        RustValue::I128(v) => int32(*v),
        RustValue::Usize(v) => int32(*v as i128),
        RustValue::Float(v) => Value::Float(*v),
        RustValue::Bool(b) => Value::Bool(*b),
        RustValue::None => Value::Option(None),
        RustValue::NullPtr => Value::Null,
    }
}

// Literals never carry an `IntWidth` tag of their own in the emitted AST — see
// `src/effects/mod.rs` doc comment on why no trace comparison depends on an
// intermediate scalar's declared width, only on the destination it's written
// through.
pub(super) fn int32(value: i128) -> Value {
    Value::Int {
        width: IntWidth::W32,
        signed: true,
        value,
    }
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
        other => panic!("effects::rust_ast: buffer element must be an integer, found {other:?}"),
    }
}

pub(super) fn option_value(value: Value) -> OptionValue {
    match value {
        Value::Int {
            width,
            signed,
            value,
        } => OptionValue::Int {
            width,
            signed,
            value,
        },
        Value::Bool(value) => OptionValue::Bool(value),
        Value::Ref(loc) => OptionValue::Ref(loc),
        other => panic!("effects::rust_ast: unsupported Some payload `{other:?}`"),
    }
}

pub(super) fn option_value_to_value(value: OptionValue) -> Value {
    match value {
        OptionValue::Int {
            width,
            signed,
            value,
        } => Value::Int {
            width,
            signed,
            value,
        },
        OptionValue::Bool(value) => Value::Bool(value),
        OptionValue::Ref(loc) => Value::Ref(loc),
    }
}

pub(super) fn option_unwrap(value: Value) -> Value {
    match value {
        Value::Option(Some(OptionValue::Int {
            width,
            signed,
            value,
        })) => Value::Int {
            width,
            signed,
            value,
        },
        Value::Option(Some(OptionValue::Bool(value))) => Value::Bool(value),
        Value::Option(Some(OptionValue::Ref(loc))) => Value::Ref(loc),
        Value::Option(None) => panic!("effects::rust_ast: unwrap on None"),
        other => panic!("effects::rust_ast: unwrap on non-option `{other:?}`"),
    }
}

pub(super) fn option_is_none(value: Value) -> Value {
    match value {
        Value::Option(value) => Value::Bool(value.is_none()),
        other => panic!("effects::rust_ast: is_none on non-option `{other:?}`"),
    }
}

pub(super) fn option_is_some(value: Value) -> Value {
    match value {
        Value::Option(value) => Value::Bool(value.is_some()),
        other => panic!("effects::rust_ast: is_some on non-option `{other:?}`"),
    }
}

pub(super) fn cast_value_to_type(value: Value, ty: &Type) -> Value {
    if matches!(ty, Type::Prim(crate::rust_ast::Prim::Bool)) {
        return match value {
            Value::Bool(value) => Value::Bool(value),
            Value::Int { value, .. } => Value::Bool(value != 0),
            other => other,
        };
    }
    if matches!(ty, Type::Ptr { .. }) {
        return match value {
            Value::Int { value, .. } => Value::Int {
                width: IntWidth::PointerSized,
                signed: false,
                value: truncate_int(value, IntWidth::PointerSized, false),
            },
            other => other,
        };
    }
    let Some((width, signed, _)) = scalar_type_shape(ty) else {
        return value;
    };
    match value {
        Value::Int { value, .. } => Value::Int {
            width,
            signed,
            value: truncate_int(value, width, signed),
        },
        Value::Bool(value) => Value::Int {
            width,
            signed,
            value: i128::from(value),
        },
        Value::Float(value) => match ty {
            Type::Prim(crate::rust_ast::Prim::F32) => Value::Float(value as f32 as f64),
            Type::Prim(crate::rust_ast::Prim::F64) => Value::Float(value),
            _ => Value::Float(value),
        },
        other => other,
    }
}

pub(super) fn scalar_type_shape(ty: &Type) -> Option<(IntWidth, bool, u64)> {
    let Type::Prim(prim) = ty else {
        return None;
    };
    use crate::rust_ast::Prim;
    Some(match prim {
        Prim::I8 => (IntWidth::W8, true, 1),
        Prim::U8 => (IntWidth::W8, false, 1),
        Prim::I16 => (IntWidth::W16, true, 2),
        Prim::U16 => (IntWidth::W16, false, 2),
        Prim::I32 => (IntWidth::W32, true, 4),
        Prim::U32 => (IntWidth::W32, false, 4),
        Prim::I64 => (IntWidth::W64, true, 8),
        Prim::U64 => (IntWidth::W64, false, 8),
        Prim::I128 => (IntWidth::W128, true, 16),
        Prim::U128 => (IntWidth::W128, false, 16),
        Prim::Isize => (IntWidth::PointerSized, true, 8),
        Prim::Usize => (IntWidth::PointerSized, false, 8),
        Prim::Bool => (IntWidth::W8, false, 1),
        Prim::F32 | Prim::F64 => return None,
    })
}

pub(super) fn type_size(ty: &Type) -> Option<u64> {
    match ty {
        Type::Prim(crate::rust_ast::Prim::F32) => Some(4),
        Type::Prim(crate::rust_ast::Prim::F64) => Some(8),
        Type::Prim(_) => scalar_type_shape(ty).map(|(_, _, size)| size),
        Type::Ptr { .. } | Type::FnPtr { .. } | Type::Ref { .. } => Some(8),
        Type::Array { elem, len } => Some(type_size(elem)? * *len),
        _ => None,
    }
}

pub(super) fn pointer_elem_size_from_type(ty: &Type) -> Option<u64> {
    let Type::Ptr { inner, .. } = ty else {
        return None;
    };
    type_size(inner).or(Some(1))
}

pub(super) fn slice_elem_shape(ty: &Type) -> Option<(IntWidth, bool, u64)> {
    let elem = match ty {
        Type::Slice(elem) => elem.as_ref(),
        Type::Ref { inner, .. } => match inner.as_ref() {
            Type::Slice(elem) => elem.as_ref(),
            _ => return None,
        },
        _ => return None,
    };
    scalar_type_shape(elem)
}

pub(super) fn array_elem_shape(ty: &Type) -> Option<(IntWidth, bool, u64, u64)> {
    let Type::Array { elem, len } = ty else {
        return None;
    };
    let (width, signed, size) = scalar_type_shape(elem)?;
    Some((width, signed, size, *len))
}

pub(super) fn is_cstr_ref_ty(ty: &Type) -> bool {
    matches!(
        ty,
        Type::Ref {
            inner,
            ..
        } if matches!(inner.as_ref(), Type::Custom(name) if name == "core::ffi::CStr")
    )
}

pub(super) fn is_str_ref_ty(ty: &Type) -> bool {
    matches!(ty, Type::Ref { inner, .. } if matches!(inner.as_ref(), Type::Str))
}

pub(super) fn collection_name(expr: &Expr) -> &str {
    match expr {
        Expr::Var(ident) => ident.as_str(),
        Expr::MethodCall { recv, method, args }
            if args.is_empty()
                && matches!(
                    method.as_str(),
                    "as_slice" | "as_mut_slice" | "as_bytes" | "to_bytes"
                ) =>
        {
            collection_name(recv)
        }
        other => panic!("effects::rust_ast: unsupported collection expression `{other:?}`"),
    }
}

pub(super) fn is_once_lock_ty(ty: &Type) -> bool {
    matches!(ty, Type::Generic { name, .. } if name == "std::sync::OnceLock")
}

pub(super) fn value_as_i128(value: impl std::borrow::Borrow<Value>) -> i128 {
    match value.borrow() {
        Value::Int { value, .. } => *value,
        Value::Bool(value) => i128::from(*value),
        other => panic!("effects::rust_ast: expected an integer value, found {other:?}"),
    }
}

pub(super) fn value_as_u64(value: impl std::borrow::Borrow<Value>) -> u64 {
    value_as_i128(value) as u64
}

pub(super) fn value_as_bool(value: impl std::borrow::Borrow<Value>) -> bool {
    match value.borrow() {
        Value::Bool(b) => *b,
        other => panic!("effects::rust_ast: expected a bool value, found {other:?}"),
    }
}

pub(super) fn apply_binop(op: BinOp, a: Value, b: Value) -> Value {
    match op {
        BinOp::Eq | BinOp::Ne | BinOp::Lt | BinOp::Le | BinOp::Gt | BinOp::Ge => {
            if matches!((&a, &b), (Value::Ref(_), Value::Ref(_))) {
                let (Value::Ref(left), Value::Ref(right)) = (&a, &b) else {
                    unreachable!();
                };
                return Value::Bool(match op {
                    BinOp::Eq => left == right,
                    BinOp::Ne => left != right,
                    _ => panic!("effects::rust_ast: unsupported pointer comparison `{op:?}`"),
                });
            }
            if matches!(
                (&a, &b),
                (Value::Ref(_), Value::Null) | (Value::Null, Value::Ref(_))
            ) {
                return Value::Bool(match op {
                    BinOp::Eq => false,
                    BinOp::Ne => true,
                    _ => panic!("effects::rust_ast: unsupported pointer/null comparison `{op:?}`"),
                });
            }
            if matches!((&a, &b), (Value::Null, Value::Null)) {
                return Value::Bool(match op {
                    BinOp::Eq => true,
                    BinOp::Ne => false,
                    _ => panic!("effects::rust_ast: unsupported null comparison `{op:?}`"),
                });
            }
            if matches!(
                (&a, &b),
                (Value::File(_), Value::Null) | (Value::Null, Value::File(_))
            ) {
                return Value::Bool(match op {
                    BinOp::Eq => false,
                    BinOp::Ne => true,
                    _ => panic!("effects::rust_ast: unsupported file/null comparison `{op:?}`"),
                });
            }
            if let (Value::Float(a), Value::Float(b)) = (&a, &b) {
                return Value::Bool(match op {
                    BinOp::Eq => a == b,
                    BinOp::Ne => a != b,
                    BinOp::Lt => a < b,
                    BinOp::Le => a <= b,
                    BinOp::Gt => a > b,
                    BinOp::Ge => a >= b,
                    _ => unreachable!(),
                });
            }
            let (a_int, b_int) = (value_as_i128(a), value_as_i128(b));
            Value::Bool(match op {
                BinOp::Eq => a_int == b_int,
                BinOp::Ne => a_int != b_int,
                BinOp::Lt => a_int < b_int,
                BinOp::Le => a_int <= b_int,
                BinOp::Gt => a_int > b_int,
                BinOp::Ge => a_int >= b_int,
                _ => unreachable!(),
            })
        }
        BinOp::And | BinOp::Or => {
            panic!("effects::rust_ast: {op:?} must short-circuit, not reach apply_binop")
        }
        _ => {
            if let (Value::Float(a), Value::Float(b)) = (&a, &b) {
                return Value::Float(match op {
                    BinOp::Add => a + b,
                    BinOp::Sub => a - b,
                    BinOp::Mul => a * b,
                    BinOp::Div => a / b,
                    _ => panic!("effects::rust_ast: unsupported float binop `{op:?}`"),
                });
            }
            let (width, signed) = match a {
                Value::Int { width, signed, .. } => (width, signed),
                other => panic!("effects::rust_ast: expected int operand, found {other:?}"),
            };
            let (a, b) = (value_as_i128(a), value_as_i128(b));
            let value = match op {
                BinOp::Add => a.wrapping_add(b),
                BinOp::Sub => a.wrapping_sub(b),
                BinOp::Mul => a.wrapping_mul(b),
                BinOp::Div => a.wrapping_div(b),
                BinOp::Rem => a.wrapping_rem(b),
                BinOp::BitAnd => a & b,
                BinOp::BitOr => a | b,
                BinOp::BitXor => a ^ b,
                BinOp::Shl => truncate_int(a, width, signed).wrapping_shl(b as u32),
                BinOp::Shr if signed => truncate_int(a, width, signed).wrapping_shr(b as u32),
                BinOp::Shr => unsigned_shr(a, width, b as u32),
                _ => unreachable!(),
            };
            Value::Int {
                width,
                signed,
                value: truncate_int(value, width, signed),
            }
        }
    }
}

pub(super) fn truncate_int(value: i128, width: IntWidth, signed: bool) -> i128 {
    let bits = match width {
        IntWidth::W8 => 8,
        IntWidth::W16 => 16,
        IntWidth::W32 => 32,
        IntWidth::W64 | IntWidth::PointerSized => 64,
        IntWidth::W128 => return value,
    };
    let mask = (1i128 << bits) - 1;
    let value = value & mask;
    if signed && value & (1i128 << (bits - 1)) != 0 {
        value | !mask
    } else {
        value
    }
}

fn unsigned_shr(value: i128, width: IntWidth, shift: u32) -> i128 {
    match width {
        IntWidth::W8 => ((value as u8) >> shift) as i128,
        IntWidth::W16 => ((value as u16) >> shift) as i128,
        IntWidth::W32 => ((value as u32) >> shift) as i128,
        IntWidth::W64 | IntWidth::PointerSized => ((value as u64) >> shift) as i128,
        IntWidth::W128 => ((value as u128) >> shift) as i128,
    }
}

fn atomic_type_shape(ty: &AtomicType) -> (IntWidth, bool) {
    match ty {
        AtomicType::I8 => (IntWidth::W8, true),
        AtomicType::U8 => (IntWidth::W8, false),
        AtomicType::I16 => (IntWidth::W16, true),
        AtomicType::U16 => (IntWidth::W16, false),
        AtomicType::I32 => (IntWidth::W32, true),
        AtomicType::U32 => (IntWidth::W32, false),
        AtomicType::I64 => (IntWidth::W64, true),
        AtomicType::U64 => (IntWidth::W64, false),
        AtomicType::Isize => (IntWidth::PointerSized, true),
        AtomicType::Usize => (IntWidth::PointerSized, false),
        AtomicType::Bool => (IntWidth::W8, false),
    }
}

pub(super) fn cast_to_atomic_type(value: Value, ty: &AtomicType) -> Value {
    if matches!(ty, AtomicType::Bool) {
        return match value {
            Value::Bool(b) => Value::Bool(b),
            Value::Int { value, .. } => Value::Bool(value != 0),
            other => other,
        };
    }
    let (width, signed) = atomic_type_shape(ty);
    cast_to_int_shape(value, width, signed)
}

fn cast_to_int_shape(value: Value, width: IntWidth, signed: bool) -> Value {
    match value {
        Value::Int { value, .. } => Value::Int {
            width,
            signed,
            value: truncate_int(value, width, signed),
        },
        Value::Bool(b) => Value::Int {
            width,
            signed,
            value: i128::from(b),
        },
        other => other,
    }
}

pub(super) fn match_atomic_shape(value: Value, reference: &Value) -> Value {
    match reference {
        Value::Int { width, signed, .. } => cast_to_int_shape(value, *width, *signed),
        Value::Bool(_) => match value {
            Value::Bool(b) => Value::Bool(b),
            Value::Int { value, .. } => Value::Bool(value != 0),
            other => other,
        },
        _ => value,
    }
}

pub(super) fn atomic_rmw_value(op: AtomicRmwOp, old: Value, operand: Value) -> Value {
    if let Value::Ref(loc) = old {
        let delta = value_as_i128(&operand);
        let byte_offset = match op {
            AtomicRmwOp::Add => (loc.byte_offset as i128 + delta) as u64,
            AtomicRmwOp::Sub => (loc.byte_offset as i128 - delta) as u64,
            other => panic!("effects::rust_ast: unsupported atomic pointer rmw `{other:?}`"),
        };
        return Value::Ref(Location {
            alloc: loc.alloc,
            byte_offset,
        });
    }
    let binop = match op {
        AtomicRmwOp::Add => BinOp::Add,
        AtomicRmwOp::Sub => BinOp::Sub,
        AtomicRmwOp::And => BinOp::BitAnd,
        AtomicRmwOp::Xor => BinOp::BitXor,
        AtomicRmwOp::Or => BinOp::BitOr,
        AtomicRmwOp::Nand => {
            let and = apply_binop(BinOp::BitAnd, old, operand);
            return match and {
                Value::Int {
                    width,
                    signed,
                    value,
                } => Value::Int {
                    width,
                    signed,
                    value: !value,
                },
                other => panic!("effects::rust_ast: atomic nand expected int, found {other:?}"),
            };
        }
        AtomicRmwOp::Max => {
            return int32(value_as_i128(old).max(value_as_i128(operand)));
        }
        AtomicRmwOp::Min => {
            return int32(value_as_i128(old).min(value_as_i128(operand)));
        }
    };
    apply_binop(binop, old, operand)
}

/// `Vec<T>`'s element width/signedness/byte-size, read off the local's
/// declared type — the emitted AST carries no width on integer literals
/// themselves, so the element type is the only source of truth for it.
pub(super) fn vec_elem_shape(ty: &Type) -> Option<(IntWidth, bool, u64)> {
    let Type::Generic { name, args } = ty else {
        return None;
    };
    if name != "Vec" {
        return None;
    }
    let Type::Prim(prim) = args.first()? else {
        return None;
    };
    use crate::rust_ast::Prim;
    Some(match prim {
        Prim::I8 => (IntWidth::W8, true, 1),
        Prim::U8 => (IntWidth::W8, false, 1),
        Prim::I16 => (IntWidth::W16, true, 2),
        Prim::U16 => (IntWidth::W16, false, 2),
        Prim::I32 => (IntWidth::W32, true, 4),
        Prim::U32 => (IntWidth::W32, false, 4),
        Prim::I64 => (IntWidth::W64, true, 8),
        Prim::U64 => (IntWidth::W64, false, 8),
        Prim::I128 => (IntWidth::W128, true, 16),
        Prim::U128 => (IntWidth::W128, false, 16),
        Prim::Isize => (IntWidth::PointerSized, true, 8),
        Prim::Usize => (IntWidth::PointerSized, false, 8),
        Prim::Bool | Prim::F32 | Prim::F64 => return None,
    })
}
