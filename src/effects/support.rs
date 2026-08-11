use std::collections::HashMap;

use crate::effects::{
    ArgShapeKind, Construct, EResult, EffectError, Found, IntWidth, Location, OptionValue, Value,
    ValueKind,
};
use crate::rust_ast::{
    AtomicOrdering, AtomicRmwOp, AtomicType, BinOp, Expr, Path, Prim, RustValue, Type, UnaryOp,
};
use std::borrow::Borrow;

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

pub(super) fn is_box_new_call(expr: &Expr) -> bool {
    match path_name(expr).as_deref() {
        Some(name) => name.starts_with("Box::<") && name.ends_with(">::new"),
        None => false,
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

pub(super) fn recv_name(expr: &Expr) -> EResult<&str> {
    match expr {
        Expr::Var(ident) => Ok(ident.as_str()),
        other => Err(EffectError::unsupported(
            Construct::AtomicReceiver,
            other.clone(),
        )),
    }
}

pub(super) enum AtomicPtrTarget<'a> {
    Local(&'a str),
    Field { base: &'a Expr, field: &'a str },
}

pub(super) fn atomic_ptr_target(expr: &Expr) -> EResult<AtomicPtrTarget<'_>> {
    match expr {
        Expr::Cast { expr, .. } => atomic_ptr_target(expr),
        Expr::AddrOf { expr, .. } => atomic_ptr_place(expr),
        Expr::Macro { name, args } if name == "std::ptr::addr_of_mut" => {
            let [arg] = args.as_slice() else {
                return Err(EffectError::arg_shape(
                    Construct::AddrOfMut,
                    ArgShapeKind::OneArgument,
                ));
            };
            atomic_ptr_place(arg)
        }
        other => Err(EffectError::unsupported(
            Construct::AtomicPointerPlace,
            other.clone(),
        )),
    }
}

fn atomic_ptr_place(expr: &Expr) -> EResult<AtomicPtrTarget<'_>> {
    match expr {
        Expr::Var(ident) => Ok(AtomicPtrTarget::Local(ident.as_str())),
        Expr::Field { base, field } => Ok(AtomicPtrTarget::Field { base, field }),
        other => Err(EffectError::unsupported(
            Construct::AtomicPointerPlace,
            other.clone(),
        )),
    }
}

pub(super) fn ordering_expr(expr: &Expr) -> EResult<AtomicOrdering> {
    match expr {
        Expr::Path(path) => ordering_from_name(path.segments.last().map(|s| s.as_str())),
        Expr::Var(ident) => ordering_from_name(ident.as_str().rsplit("::").next()),
        other => Err(EffectError::unsupported(
            Construct::AtomicOrdering,
            other.clone(),
        )),
    }
}

pub(super) fn ordering_from_name(name: Option<&str>) -> EResult<AtomicOrdering> {
    match name {
        Some("Relaxed") => Ok(AtomicOrdering::Relaxed),
        Some("Acquire") => Ok(AtomicOrdering::Acquire),
        Some("Release") => Ok(AtomicOrdering::Release),
        Some("AcqRel") => Ok(AtomicOrdering::AcqRel),
        Some("SeqCst") => Ok(AtomicOrdering::SeqCst),
        other => Err(EffectError::unsupported(
            Construct::AtomicOrdering,
            other.map(str::to_string),
        )),
    }
}

pub(super) fn open_effect(expr: &Expr) -> EResult<Option<OpenEffect>> {
    match expr {
        Expr::Call { func, args, .. } if is_path(func, &["std", "io", "BufReader", "new"]) => {
            let [inner] = args.as_slice() else {
                return Err(EffectError::arg_shape(
                    Construct::BufReaderNew,
                    ArgShapeKind::OneArgument,
                ));
            };
            open_effect(inner)
        }
        Expr::MethodCall { recv, method, .. }
            if matches!(method.as_str(), "unwrap" | "unwrap_or_else") =>
        {
            open_effect(recv)
        }
        Expr::MethodCall { recv, method, args } if method == "open" => {
            let [Expr::Str(path)] = args.as_slice() else {
                return Err(EffectError::arg_shape(
                    Construct::OpenOptionsOpen,
                    ArgShapeKind::StringLiteralPath,
                ));
            };
            Ok(Some(OpenEffect {
                path: path.clone(),
                mode: open_options_mode(recv)?,
            }))
        }
        _ => Ok(None),
    }
}

pub(super) fn open_options_mode(expr: &Expr) -> EResult<String> {
    let mut read = false;
    let mut write = false;
    let mut append = false;
    let mut create = false;
    let mut truncate = false;
    let mut current = expr;
    loop {
        match current {
            Expr::Call { func, args, .. }
                if args.is_empty() && is_path(func, &["std", "fs", "OpenOptions", "new"]) =>
            {
                break;
            }
            Expr::MethodCall { recv, method, args } => {
                let [Expr::Value(RustValue::Bool(value))] = args.as_slice() else {
                    return Err(EffectError::arg_shape(
                        Construct::OpenOptionsMethod,
                        ArgShapeKind::BoolLiteral,
                    ));
                };
                match method.as_str() {
                    "read" => read = *value,
                    "write" => write = *value,
                    "append" => append = *value,
                    "create" => create = *value,
                    "truncate" => truncate = *value,
                    other => {
                        return Err(EffectError::unsupported(
                            Construct::OpenOptionsMethod,
                            other,
                        ));
                    }
                }
                current = recv;
            }
            other => {
                return Err(EffectError::unsupported(
                    Construct::OpenOptionsChain,
                    other.clone(),
                ));
            }
        }
    }
    Ok(match (read, write, append, create, truncate) {
        (false, true, false, true, true) => "w",
        (true, false, false, false, false) => "r",
        (false, false, true, true, false) => "a",
        (true, true, false, false, false) => "r+",
        (true, true, false, true, true) => "w+",
        (true, false, true, true, false) => "a+",
        _ => {
            return Err(EffectError::unsupported(
                Construct::OpenOptionsMode,
                Found::OpenOptionsFlags,
            ));
        }
    }
    .to_string())
}

pub(super) fn array_pointer_name(expr: &Expr) -> EResult<&str> {
    match expr {
        Expr::Cast { expr, .. } => array_pointer_name(expr),
        Expr::MethodCall { recv, method, args } if method == "as_mut_ptr" && args.is_empty() => {
            collection_name(recv)
        }
        Expr::ArrayPtr { array, .. } => collection_name(array),
        other => Err(EffectError::unsupported(
            Construct::ArrayPointerArg,
            other.clone(),
        )),
    }
}

pub(super) fn comparator_name(expr: &Expr) -> EResult<&str> {
    match expr {
        Expr::Call { func, args, .. } if is_path(func, &["Some"]) => {
            let [arg] = args.as_slice() else {
                return Err(EffectError::arg_shape(
                    Construct::SomeComparator,
                    ArgShapeKind::OneArgument,
                ));
            };
            comparator_name(arg)
        }
        Expr::Var(ident) => Ok(ident.as_str()),
        other => Err(EffectError::unsupported(
            Construct::ComparatorArg,
            other.clone(),
        )),
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

pub(super) fn value_as_i32(value: impl Borrow<Value>) -> EResult<i32> {
    match value.borrow() {
        Value::Int { value, .. } => Ok(*value as i32),
        other => Err(EffectError::type_mismatch(ValueKind::Int, other.clone())),
    }
}

pub(super) fn rust_value_to_value(rv: &RustValue) -> Value {
    match rv {
        RustValue::I64(v) => int32(*v as i128),
        RustValue::I128(v) => int32(*v),
        RustValue::U128(v) => int32(*v as i128),
        RustValue::TypedInt(v, _) => int32(*v),
        RustValue::TypedUInt(v, _) => int32(*v as i128),
        RustValue::Usize(v) => int32(*v as i128),
        RustValue::Float(v) => Value::Float(v.into_inner()),
        RustValue::Bool(b) => Value::Bool(*b),
        RustValue::None => Value::Option(None),
        RustValue::NullPtr => Value::Null,
    }
}

pub(super) fn int32(value: i128) -> Value {
    Value::Int {
        width: IntWidth::W32,
        signed: true,
        value,
    }
}

pub(super) fn int_byte_size(value: &Value) -> EResult<u64> {
    match value {
        Value::Int { width, .. } => Ok(match width {
            IntWidth::W8 => 1,
            IntWidth::W16 => 2,
            IntWidth::W32 => 4,
            IntWidth::W64 | IntWidth::PointerSized => 8,
            IntWidth::W128 => 16,
        }),
        other => Err(EffectError::type_mismatch(ValueKind::Int, other.clone())),
    }
}

pub(super) fn option_value(value: Value) -> EResult<OptionValue> {
    match value {
        Value::Int {
            width,
            signed,
            value,
        } => Ok(OptionValue::Int {
            width,
            signed,
            value,
        }),
        Value::Bool(value) => Ok(OptionValue::Bool(value)),
        Value::Ref(loc) => Ok(OptionValue::Ref(loc)),
        Value::Function(name) => Ok(OptionValue::Function(name)),
        other => Err(EffectError::unsupported(Construct::SomePayload, other)),
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
        OptionValue::Function(name) => Value::Function(name),
    }
}

pub(super) fn option_unwrap(value: Value) -> EResult<Value> {
    match value {
        Value::Option(Some(OptionValue::Int {
            width,
            signed,
            value,
        })) => Ok(Value::Int {
            width,
            signed,
            value,
        }),
        Value::Option(Some(OptionValue::Bool(value))) => Ok(Value::Bool(value)),
        Value::Option(Some(OptionValue::Ref(loc))) => Ok(Value::Ref(loc)),
        Value::Option(Some(OptionValue::Function(name))) => Ok(Value::Function(name)),
        value @ Value::Option(None) => Err(EffectError::type_mismatch(ValueKind::Option, value)),
        other => Err(EffectError::type_mismatch(ValueKind::Option, other)),
    }
}

pub(super) fn option_is_none(value: Value) -> EResult<Value> {
    match value {
        Value::Option(value) => Ok(Value::Bool(value.is_none())),
        other => Err(EffectError::type_mismatch(ValueKind::Option, other)),
    }
}

pub(super) fn option_is_some(value: Value) -> EResult<Value> {
    match value {
        Value::Option(value) => Ok(Value::Bool(value.is_some())),
        other => Err(EffectError::type_mismatch(ValueKind::Option, other)),
    }
}

pub(super) fn cast_value_to_type(value: Value, ty: &Type) -> EResult<Value> {
    if matches!(ty, Type::Prim(Prim::Bool)) {
        return match value {
            Value::Bool(value) => Ok(Value::Bool(value)),
            Value::Int { value, .. } => Ok(Value::Bool(value != 0)),
            other => Ok(other),
        };
    }
    if matches!(ty, Type::Ptr { .. }) {
        return match value {
            Value::Int { value, .. } => Ok(Value::Int {
                width: IntWidth::PointerSized,
                signed: false,
                value: truncate_int(value, IntWidth::PointerSized, false),
            }),
            other => Ok(other),
        };
    }
    if matches!(ty, Type::Prim(Prim::F32)) {
        return Ok(match value {
            Value::Float(value) => Value::Float(value as f32 as f64),
            Value::Int { value, .. } => Value::Float(value as f32 as f64),
            Value::Bool(value) => Value::Float(i32::from(value) as f32 as f64),
            other => other,
        });
    }
    if matches!(ty, Type::Prim(Prim::F64 | Prim::F128)) {
        return Ok(match value {
            Value::Float(value) => Value::Float(value),
            Value::Int { value, .. } => Value::Float(value as f64),
            Value::Bool(value) => Value::Float(i32::from(value) as f64),
            other => other,
        });
    }
    if matches!(ty, Type::LongDouble | Type::Complex(_)) {
        return Ok(value);
    }
    if matches!(ty, Type::Ref { .. } | Type::FnPtr { .. })
        || matches!(ty, Type::Custom(_))
        || matches!(ty, Type::Generic { name, .. } if name == "Result")
    {
        return Ok(value);
    }
    let Some((width, signed, _)) = scalar_type_shape(ty) else {
        return Err(EffectError::unsupported(
            Construct::CastTargetType,
            ty.clone(),
        ));
    };
    Ok(match value {
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
            Type::Prim(Prim::F32) => Value::Float(value as f32 as f64),
            Type::Prim(Prim::F64 | Prim::F128) => Value::Float(value),
            _ => Value::Float(value),
        },
        other => other,
    })
}

pub(super) fn scalar_type_shape(ty: &Type) -> Option<(IntWidth, bool, u64)> {
    let ty = ty.peel_aligned();
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
        Prim::F32 | Prim::F64 | Prim::F128 => return None,
    })
}

pub(super) fn type_size(ty: &Type) -> Option<u64> {
    let ty = ty.peel_aligned();
    match ty {
        Type::Prim(Prim::F32) => Some(4),
        Type::Prim(Prim::F64) => Some(8),
        Type::Prim(Prim::F128) => Some(16),
        Type::Prim(_) => scalar_type_shape(ty).map(|(_, _, size)| size),
        Type::LongDouble => Some(16),
        Type::Complex(inner) => Some(type_size(inner)? * 2),
        Type::Ptr { .. } | Type::FnPtr { .. } | Type::Ref { .. } => Some(8),
        Type::Array { elem, len } => Some(type_size(elem)? * *len),
        _ => None,
    }
}

pub(super) fn pointer_elem_size_from_type(ty: &Type) -> Option<u64> {
    let ty = ty.peel_aligned();
    let Type::Ptr { inner, .. } = ty else {
        return None;
    };
    type_size(inner).or(Some(1))
}

pub(super) fn slice_elem_shape(ty: &Type) -> Option<(IntWidth, bool, u64)> {
    let ty = ty.peel_aligned();
    let elem = match ty {
        Type::Slice(elem) => Some(elem.as_ref()),
        Type::Ref { inner, .. } => match inner.as_ref() {
            Type::Slice(elem) => Some(elem.as_ref()),
            Type::Array { elem, .. } => Some(elem.as_ref()),
            _ => None,
        },
        _ => None,
    }?;
    scalar_type_shape(elem)
}

pub(super) fn box_elem_shape(ty: &Type) -> Option<(IntWidth, bool, u64)> {
    let ty = ty.peel_aligned();
    let elem = match ty {
        Type::Generic { name, args } if name == "Box" => scalar_type_shape(args.first()?),
        Type::Custom(name) => {
            let elem = name
                .strip_prefix("Box<")
                .and_then(|rest| rest.strip_suffix('>'))?;
            scalar_type_shape(&Type::parse(elem))
        }
        _ => None,
    }?;
    Some(elem)
}

pub(super) fn array_elem_shape(ty: &Type) -> Option<(IntWidth, bool, u64, u64)> {
    let ty = ty.peel_aligned();
    let Type::Array { elem, len } = ty else {
        return None;
    };
    match elem.as_ref() {
        Type::Prim(Prim::F32) => Some((IntWidth::W32, false, 4, *len)),
        Type::Prim(Prim::F64) => Some((IntWidth::W64, false, 8, *len)),
        Type::Prim(Prim::F128) => Some((IntWidth::W128, false, 16, *len)),
        _ => {
            let (width, signed, size) = scalar_type_shape(elem)?;
            Some((width, signed, size, *len))
        }
    }
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

pub(super) fn collection_name(expr: &Expr) -> EResult<&str> {
    match expr {
        Expr::Var(ident) => Ok(ident.as_str()),
        Expr::Ref { expr, .. } => collection_name(expr),
        Expr::Unary {
            op: UnaryOp::Deref,
            expr,
        } => collection_name(expr),
        Expr::MethodCall { recv, method, args }
            if args.is_empty()
                && matches!(
                    method.as_str(),
                    "as_slice" | "as_mut_slice" | "as_bytes" | "to_bytes"
                ) =>
        {
            collection_name(recv)
        }
        other => Err(EffectError::unsupported(
            Construct::CollectionExpr,
            other.clone(),
        )),
    }
}

pub(super) fn is_once_lock_ty(ty: &Type) -> bool {
    matches!(ty, Type::Generic { name, .. } if name == "std::sync::OnceLock")
}

pub(super) fn value_as_i128(value: impl Borrow<Value>) -> EResult<i128> {
    match value.borrow() {
        Value::Int { value, .. } => Ok(*value),
        Value::Bool(value) => Ok(i128::from(*value)),
        other => Err(EffectError::type_mismatch(ValueKind::Int, other.clone())),
    }
}

pub(super) fn value_as_u64(value: impl Borrow<Value>) -> EResult<u64> {
    Ok(value_as_i128(value)? as u64)
}

pub(super) fn value_as_bool(value: impl Borrow<Value>) -> EResult<bool> {
    match value.borrow() {
        Value::Bool(b) => Ok(*b),
        other => Err(EffectError::type_mismatch(ValueKind::Bool, other.clone())),
    }
}

pub(super) fn apply_binop(op: BinOp, a: Value, b: Value) -> EResult<Value> {
    match op {
        BinOp::Eq | BinOp::Ne | BinOp::Lt | BinOp::Le | BinOp::Gt | BinOp::Ge => {
            if let (Value::Bytes(a), Value::Bytes(b)) = (&a, &b) {
                let ord = compare_bytes(a, b);
                return Ok(Value::Bool(match op {
                    BinOp::Eq => ord == 0,
                    BinOp::Ne => ord != 0,
                    BinOp::Lt => ord < 0,
                    BinOp::Le => ord <= 0,
                    BinOp::Gt => ord > 0,
                    BinOp::Ge => ord >= 0,
                    _ => unreachable!(),
                }));
            }
            if matches!((&a, &b), (Value::Ref(_), Value::Ref(_))) {
                let (Value::Ref(left), Value::Ref(right)) = (&a, &b) else {
                    unreachable!();
                };
                return Ok(Value::Bool(match op {
                    BinOp::Eq => left == right,
                    BinOp::Ne => left != right,
                    _ => {
                        return Err(EffectError::unsupported(Construct::PointerComparison, op));
                    }
                }));
            }
            if let (Value::Function(left), Value::Function(right)) = (&a, &b) {
                return Ok(Value::Bool(match op {
                    BinOp::Eq => left == right,
                    BinOp::Ne => left != right,
                    _ => {
                        return Err(EffectError::unsupported(Construct::PointerComparison, op));
                    }
                }));
            }
            if matches!(
                (&a, &b),
                (Value::Ref(_), Value::Null) | (Value::Null, Value::Ref(_))
            ) {
                return Ok(Value::Bool(match op {
                    BinOp::Eq => false,
                    BinOp::Ne => true,
                    _ => {
                        return Err(EffectError::unsupported(
                            Construct::PointerNullComparison,
                            op,
                        ));
                    }
                }));
            }
            if matches!(
                (&a, &b),
                (Value::Int { .. }, Value::Null) | (Value::Null, Value::Int { .. })
            ) {
                let is_null = match (&a, &b) {
                    (Value::Int { value, .. }, Value::Null)
                    | (Value::Null, Value::Int { value, .. }) => *value == 0,
                    _ => unreachable!(),
                };
                return Ok(Value::Bool(match op {
                    BinOp::Eq => is_null,
                    BinOp::Ne => !is_null,
                    _ => {
                        return Err(EffectError::unsupported(
                            Construct::PointerNullComparison,
                            op,
                        ));
                    }
                }));
            }
            if matches!((&a, &b), (Value::Null, Value::Null)) {
                return Ok(Value::Bool(match op {
                    BinOp::Eq => true,
                    BinOp::Ne => false,
                    _ => {
                        return Err(EffectError::unsupported(Construct::NullComparison, op));
                    }
                }));
            }
            if matches!(
                (&a, &b),
                (Value::File(_), Value::Null) | (Value::Null, Value::File(_))
            ) {
                return Ok(Value::Bool(match op {
                    BinOp::Eq => false,
                    BinOp::Ne => true,
                    _ => {
                        return Err(EffectError::unsupported(Construct::FileNullComparison, op));
                    }
                }));
            }
            if let (Value::Float(a), Value::Float(b)) = (&a, &b) {
                return Ok(Value::Bool(match op {
                    BinOp::Eq => a == b,
                    BinOp::Ne => a != b,
                    BinOp::Lt => a < b,
                    BinOp::Le => a <= b,
                    BinOp::Gt => a > b,
                    BinOp::Ge => a >= b,
                    _ => unreachable!(),
                }));
            }
            let (a_int, b_int) = (value_as_i128(a)?, value_as_i128(b)?);
            Ok(Value::Bool(match op {
                BinOp::Eq => a_int == b_int,
                BinOp::Ne => a_int != b_int,
                BinOp::Lt => a_int < b_int,
                BinOp::Le => a_int <= b_int,
                BinOp::Gt => a_int > b_int,
                BinOp::Ge => a_int >= b_int,
                _ => unreachable!(),
            }))
        }
        BinOp::And | BinOp::Or => Err(EffectError::internal(format!(
            "{op:?} must short-circuit, not reach apply_binop"
        ))),
        _ => {
            if let (Value::Float(a), Value::Float(b)) = (&a, &b) {
                return Ok(Value::Float(match op {
                    BinOp::Add => a + b,
                    BinOp::Sub => a - b,
                    BinOp::Mul => a * b,
                    BinOp::Div => a / b,
                    _ => {
                        return Err(EffectError::unsupported(Construct::FloatBinop, op));
                    }
                }));
            }
            let (width, signed) = match a {
                Value::Int { width, signed, .. } => (width, signed),
                other => {
                    return Err(EffectError::type_mismatch(ValueKind::Int, other));
                }
            };
            let (a, b) = (value_as_i128(a)?, value_as_i128(b)?);
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
            Ok(Value::Int {
                width,
                signed,
                value: truncate_int(value, width, signed),
            })
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

fn compare_bytes(left: &[u8], right: &[u8]) -> i8 {
    for (a, b) in left.iter().cloned().zip(right.iter().cloned()) {
        if a != b {
            return if a < b { -1 } else { 1 };
        }
    }
    match left.len().cmp(&right.len()) {
        std::cmp::Ordering::Less => -1,
        std::cmp::Ordering::Equal => 0,
        std::cmp::Ordering::Greater => 1,
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

pub(super) fn atomic_rmw_value(op: AtomicRmwOp, old: Value, operand: Value) -> EResult<Value> {
    if let Value::Ref(loc) = old {
        let delta = value_as_i128(&operand)?;
        let byte_offset = match op {
            AtomicRmwOp::Add => (loc.byte_offset as i128 + delta) as u64,
            AtomicRmwOp::Sub => (loc.byte_offset as i128 - delta) as u64,
            other => {
                return Err(EffectError::unsupported(Construct::AtomicPointerRmw, other));
            }
        };
        return Ok(Value::Ref(Location {
            alloc: loc.alloc,
            byte_offset,
        }));
    }
    let binop = match op {
        AtomicRmwOp::Add => BinOp::Add,
        AtomicRmwOp::Sub => BinOp::Sub,
        AtomicRmwOp::And => BinOp::BitAnd,
        AtomicRmwOp::Xor => BinOp::BitXor,
        AtomicRmwOp::Or => BinOp::BitOr,
        AtomicRmwOp::Nand => {
            let and = apply_binop(BinOp::BitAnd, old, operand)?;
            return match and {
                Value::Int {
                    width,
                    signed,
                    value,
                } => Ok(Value::Int {
                    width,
                    signed,
                    value: !value,
                }),
                other => Err(EffectError::type_mismatch(ValueKind::Int, other)),
            };
        }
        AtomicRmwOp::Max => {
            return Ok(int32(value_as_i128(old)?.max(value_as_i128(operand)?)));
        }
        AtomicRmwOp::Min => {
            return Ok(int32(value_as_i128(old)?.min(value_as_i128(operand)?)));
        }
    };
    apply_binop(binop, old, operand)
}

pub(super) fn vec_elem_shape(ty: &Type) -> Option<(IntWidth, bool, u64)> {
    let ty = ty.peel_aligned();
    let elem = match ty {
        Type::Generic { name, args } if name == "Vec" => return scalar_type_shape(args.first()?),
        Type::Custom(name) => name
            .strip_prefix("Vec<")
            .and_then(|rest| rest.strip_suffix('>'))?,
        _ => return None,
    };
    let Type::Prim(prim) = Type::parse(elem) else {
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
        Prim::Bool | Prim::F32 | Prim::F64 | Prim::F128 => return None,
    })
}
