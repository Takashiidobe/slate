use crate::backend::rust_ast::{ExternDecl, ExternFnDecl, Item, Prim, Program, RecordDef, Type};
use crate::function_identity::FunctionIdentity;
use std::collections::BTreeSet;

fn c_type_for_tag(tag: &str) -> String {
    if let Some(inner) = tag.strip_prefix('p') {
        return match inner {
            "i8" | "u8" => "char *".to_string(),
            "x" => "void *".to_string(),
            "f80" => "long double *".to_string(),
            other => format!("{} *", c_type_for_tag(other)),
        };
    }
    match tag {
        "i8" => "signed char",
        "u8" => "unsigned char",
        "i16" => "short",
        "u16" => "unsigned short",
        "i32" => "int",
        "u32" => "unsigned int",
        "i64" => "long long",
        "u64" => "unsigned long long",
        "isize" => "long",
        "usize" => "unsigned long",
        "f32" => "float",
        "f64" => "double",
        "bool" => "_Bool",
        "v" => "void",
        "ld" => "double",
        "f80" => "__slate_f80",
        "lq" => "long double",
        _ => "void *",
    }
    .to_string()
}

fn c_type_for_rust_type(ty: &Type, bridge: bool) -> String {
    match ty {
        Type::Prim(Prim::I8) => "signed char".into(),
        Type::Prim(Prim::U8) => "unsigned char".into(),
        Type::Prim(Prim::I16) => "short".into(),
        Type::Prim(Prim::U16) => "unsigned short".into(),
        Type::Prim(Prim::I32) => "int".into(),
        Type::Prim(Prim::U32) => "unsigned int".into(),
        Type::Prim(Prim::I64) => "long long".into(),
        Type::Prim(Prim::U64) => "unsigned long long".into(),
        Type::Prim(Prim::Isize) => "long".into(),
        Type::Prim(Prim::Usize) => "unsigned long".into(),
        Type::Prim(Prim::F32) => "float".into(),
        Type::Prim(Prim::F64) => "double".into(),
        Type::Prim(Prim::Bool) => "_Bool".into(),
        Type::LongDouble if bridge => "__slate_f80".into(),
        Type::LongDouble => "long double".into(),
        Type::Ptr { inner, .. } if matches!(inner.as_ref(), Type::LongDouble) => {
            "long double *".into()
        }
        Type::Ptr { .. } => "void *".into(),
        Type::FnPtr { params, ret, .. } => {
            let ret = c_type_for_rust_type(ret, false);
            let params = params
                .iter()
                .map(|param| c_type_for_rust_type(param, false))
                .collect::<Vec<_>>()
                .join(", ");
            format!("{ret} (*)({params})")
        }
        Type::Custom(name) => format!("struct {name}"),
        Type::Unit => "void".into(),
        _ => "void *".into(),
    }
}

fn render_typed_shim(shim: &ExternFnDecl) -> Option<String> {
    let callback = shim.name.strip_prefix("__slate_ld_").map(str::to_string);
    let (callee, signature_bridge) = if let Some(original) = &callback {
        (original.clone(), false)
    } else {
        let rest = shim.name.strip_prefix("__slate_")?;
        (rest.split("__").next()?.to_string(), true)
    };
    let ret_ty = shim.ret.as_ref();
    let ret_c = ret_ty
        .map(|ty| c_type_for_rust_type(ty, signature_bridge))
        .unwrap_or_else(|| "void".into());
    let params = shim
        .params
        .iter()
        .enumerate()
        .map(|(i, param)| match &param.ty {
            Type::FnPtr { params, ret, .. } => {
                let ret = c_type_for_rust_type(ret, false);
                let params = params
                    .iter()
                    .map(|param| c_type_for_rust_type(param, false))
                    .collect::<Vec<_>>()
                    .join(", ");
                format!("{ret} (*_{i})({params})")
            }
            ty => format!("{} _{i}", c_type_for_rust_type(ty, signature_bridge)),
        })
        .collect::<Vec<_>>()
        .join(", ");
    let args = shim
        .params
        .iter()
        .enumerate()
        .map(|(i, param)| match &param.ty {
            Type::LongDouble if callback.is_some() => format!("__slate_f80_store(_{i})"),
            Type::LongDouble => format!("__slate_f80_load(_{i})"),
            _ => format!("_{i}"),
        })
        .collect::<Vec<_>>()
        .join(", ");
    let call = format!("{callee}({args})");
    let body = if ret_ty.is_none() || ret_ty.is_some_and(Type::is_unit) {
        format!("{call};")
    } else if callback.is_some() && matches!(ret_ty, Some(Type::LongDouble)) {
        format!("return __slate_f80_load({call});")
    } else if matches!(ret_ty, Some(Type::LongDouble)) {
        format!("return __slate_f80_store({call});")
    } else {
        format!("return {call};")
    };
    let prototype = if let Some(original) = &callback {
        let rust_ret = ret_ty
            .map(|ty| c_type_for_rust_type(ty, true))
            .unwrap_or_else(|| "void".into());
        let rust_params = shim
            .params
            .iter()
            .map(|param| c_type_for_rust_type(&param.ty, true))
            .collect::<Vec<_>>()
            .join(", ");
        format!("{rust_ret} {original}({rust_params});\n")
    } else if shim.identity == FunctionIdentity::Unknown {
        let c_ret = ret_ty
            .map(|ty| c_type_for_rust_type(ty, false))
            .unwrap_or_else(|| "void".into());
        let c_params = shim
            .params
            .iter()
            .map(|param| c_type_for_rust_type(&param.ty, false))
            .collect::<Vec<_>>()
            .join(", ");
        format!("{c_ret} {callee}({c_params});\n")
    } else {
        String::new()
    };
    Some(format!(
        "{prototype}{ret_c} {}({params}) {{\n    {body}\n}}\n",
        shim.name
    ))
}

fn render_trampoline(name: &str) -> Option<String> {
    let rest = name.strip_prefix("__slate_")?;
    let sep = rest.find("__")?;
    let callee = &rest[..sep];
    let callback_callee = callee.strip_prefix("cb_");
    let tags: Vec<&str> = rest[sep + 2..].split('_').collect();
    let (ret_tag, arg_tags) = match tags.first() {
        Some(tag) if tag.starts_with('r') && tag.len() > 1 => (&tag[1..], &tags[1..]),
        _ => ("i32", &tags[..]),
    };
    let ret_c_type = if callback_callee.is_some() && ret_tag == "f80" {
        "long double".to_string()
    } else {
        c_type_for_tag(ret_tag)
    };
    let params = arg_tags
        .iter()
        .enumerate()
        .map(|(i, tag)| {
            let ty = if callback_callee.is_some() && *tag == "f80" {
                "long double".to_string()
            } else {
                c_type_for_tag(tag)
            };
            format!("{ty} _{i}")
        })
        .collect::<Vec<_>>()
        .join(", ");
    let args = arg_tags
        .iter()
        .enumerate()
        .map(|(i, tag)| {
            if callback_callee.is_some() && *tag == "f80" {
                format!("__slate_f80_store(_{i})")
            } else if *tag == "f80" {
                format!("__slate_f80_load(_{i})")
            } else if *tag == "pf80" {
                format!("_{i}")
            } else if *tag == "ld" {
                format!("(long double)_{i}")
            } else {
                format!("_{i}")
            }
        })
        .collect::<Vec<_>>()
        .join(", ");
    let call = format!("{}({args})", callback_callee.unwrap_or(callee));
    let body = if ret_tag == "v" {
        format!("{call};")
    } else if callback_callee.is_some() && ret_tag == "f80" {
        format!("return __slate_f80_load({call});")
    } else if ret_tag == "f80" {
        format!("return __slate_f80_store({call});")
    } else {
        format!("return {call};")
    };
    let prototype = if callback_callee.is_some() {
        let rust_ret = if ret_tag == "f80" {
            "__slate_f80".to_string()
        } else {
            c_type_for_tag(ret_tag)
        };
        let rust_params = arg_tags
            .iter()
            .map(|tag| {
                if *tag == "f80" {
                    "__slate_f80".to_string()
                } else {
                    c_type_for_tag(tag)
                }
            })
            .collect::<Vec<_>>()
            .join(", ");
        format!(
            "{rust_ret} {}({rust_params});\n",
            callback_callee.unwrap_or(callee)
        )
    } else {
        String::new()
    };
    Some(format!(
        "{prototype}{ret_c_type} {name}({params}) {{\n    {body}\n}}\n"
    ))
}

const STRTOLD_SHIM: &str = "__slate_f80 __slate_strtold(char *nptr, char **endptr) {\n    return __slate_f80_store(strtold(nptr, endptr));\n}\n";

const F80_SHIMS: &str = r#"#include <stdint.h>
#include <string.h>

typedef struct {
    unsigned char bytes[10];
} __attribute__((aligned(16))) __slate_f80;

static long double __slate_f80_load(__slate_f80 value) {
    long double out = 0.0L;
    memcpy(&out, value.bytes, sizeof(value.bytes));
    return out;
}

static __slate_f80 __slate_f80_store(long double value) {
    __slate_f80 out;
    memcpy(out.bytes, &value, sizeof(out.bytes));
    return out;
}

__slate_f80 __slate_f80_add(__slate_f80 a, __slate_f80 b) {
    return __slate_f80_store(__slate_f80_load(a) + __slate_f80_load(b));
}

__slate_f80 __slate_f80_sub(__slate_f80 a, __slate_f80 b) {
    return __slate_f80_store(__slate_f80_load(a) - __slate_f80_load(b));
}

__slate_f80 __slate_f80_mul(__slate_f80 a, __slate_f80 b) {
    return __slate_f80_store(__slate_f80_load(a) * __slate_f80_load(b));
}

__slate_f80 __slate_f80_div(__slate_f80 a, __slate_f80 b) {
    return __slate_f80_store(__slate_f80_load(a) / __slate_f80_load(b));
}

__slate_f80 __slate_f80_neg(__slate_f80 a) {
    return __slate_f80_store(-__slate_f80_load(a));
}

__slate_f80 __slate_f80_abs(__slate_f80 a) {
    return __slate_f80_store(fabsl(__slate_f80_load(a)));
}

__slate_f80 __slate_f80_ceil(__slate_f80 a) {
    return __slate_f80_store(ceill(__slate_f80_load(a)));
}

__slate_f80 __slate_f80_floor(__slate_f80 a) {
    return __slate_f80_store(floorl(__slate_f80_load(a)));
}

__slate_f80 __slate_f80_round(__slate_f80 a) {
    return __slate_f80_store(roundl(__slate_f80_load(a)));
}

__slate_f80 __slate_f80_trunc(__slate_f80 a) {
    return __slate_f80_store(truncl(__slate_f80_load(a)));
}

_Bool __slate_f80_signbit(__slate_f80 a) {
    return signbit(__slate_f80_load(a));
}

_Bool __slate_f80_lt(__slate_f80 a, __slate_f80 b) {
    return __slate_f80_load(a) < __slate_f80_load(b);
}

_Bool __slate_f80_le(__slate_f80 a, __slate_f80 b) {
    return __slate_f80_load(a) <= __slate_f80_load(b);
}

_Bool __slate_f80_gt(__slate_f80 a, __slate_f80 b) {
    return __slate_f80_load(a) > __slate_f80_load(b);
}

_Bool __slate_f80_ge(__slate_f80 a, __slate_f80 b) {
    return __slate_f80_load(a) >= __slate_f80_load(b);
}

_Bool __slate_f80_eq(__slate_f80 a, __slate_f80 b) {
    return __slate_f80_load(a) == __slate_f80_load(b);
}

_Bool __slate_f80_ne(__slate_f80 a, __slate_f80 b) {
    return __slate_f80_load(a) != __slate_f80_load(b);
}

__slate_f80 __slate_f80_from_i8(int8_t a) { return __slate_f80_store((long double)a); }
__slate_f80 __slate_f80_from_u8(uint8_t a) { return __slate_f80_store((long double)a); }
__slate_f80 __slate_f80_from_i16(int16_t a) { return __slate_f80_store((long double)a); }
__slate_f80 __slate_f80_from_u16(uint16_t a) { return __slate_f80_store((long double)a); }
__slate_f80 __slate_f80_from_i32(int32_t a) { return __slate_f80_store((long double)a); }
__slate_f80 __slate_f80_from_u32(uint32_t a) { return __slate_f80_store((long double)a); }
__slate_f80 __slate_f80_from_i64(int64_t a) { return __slate_f80_store((long double)a); }
__slate_f80 __slate_f80_from_u64(uint64_t a) { return __slate_f80_store((long double)a); }
__slate_f80 __slate_f80_from_f32(float a) { return __slate_f80_store((long double)a); }
__slate_f80 __slate_f80_from_f64(double a) { return __slate_f80_store((long double)a); }
__slate_f80 __slate_f80_from_bool(_Bool a) { return __slate_f80_store((long double)a); }

int8_t __slate_f80_to_i8(__slate_f80 a) { return (int8_t)__slate_f80_load(a); }
uint8_t __slate_f80_to_u8(__slate_f80 a) { return (uint8_t)__slate_f80_load(a); }
int16_t __slate_f80_to_i16(__slate_f80 a) { return (int16_t)__slate_f80_load(a); }
uint16_t __slate_f80_to_u16(__slate_f80 a) { return (uint16_t)__slate_f80_load(a); }
int32_t __slate_f80_to_i32(__slate_f80 a) { return (int32_t)__slate_f80_load(a); }
uint32_t __slate_f80_to_u32(__slate_f80 a) { return (uint32_t)__slate_f80_load(a); }
int64_t __slate_f80_to_i64(__slate_f80 a) { return (int64_t)__slate_f80_load(a); }
uint64_t __slate_f80_to_u64(__slate_f80 a) { return (uint64_t)__slate_f80_load(a); }
float __slate_f80_to_f32(__slate_f80 a) { return (float)__slate_f80_load(a); }
double __slate_f80_to_f64(__slate_f80 a) { return (double)__slate_f80_load(a); }
_Bool __slate_f80_to_bool(__slate_f80 a) { return __slate_f80_load(a) != 0.0L; }
"#;

pub fn render_shim_c_source(shims: &[ExternFnDecl]) -> String {
    let mut blocks = vec!["#include <stdio.h>\n#include <stdlib.h>\n#include <math.h>".to_string()];
    let has_f80 = shims.iter().any(|shim| {
        shim.name.contains("f80")
            || shim.name == "__slate_strtold"
            || shim.name.starts_with("__slate_ld_")
    });
    if has_f80 {
        blocks.push(F80_SHIMS.to_string());
    }
    for shim in shims {
        if shim.name == "__slate_strtold" {
            blocks.push(STRTOLD_SHIM.to_string());
        } else if shim.name.starts_with("__slate_f80_") {
            continue;
        } else if let Some(source) = render_typed_shim(shim) {
            blocks.push(source);
        }
    }
    blocks.join("\n")
}

pub fn render_shim_c_source_for_program(program: &Program) -> String {
    let shims: Vec<ExternFnDecl> = program
        .items
        .iter()
        .filter_map(|item| match item {
            Item::ExternBlock { decls, .. } => Some(decls),
            _ => None,
        })
        .flatten()
        .filter_map(|decl| match decl {
            ExternDecl::Fn(shim) => Some(shim.clone()),
            ExternDecl::Static { .. } => None,
        })
        .collect();
    let mut blocks = vec!["#include <stdio.h>\n#include <stdlib.h>\n#include <math.h>".to_string()];
    let has_f80 = shims.iter().any(|shim| {
        shim.name.contains("f80")
            || shim.name == "__slate_strtold"
            || shim.name.starts_with("__slate_ld_")
    });
    if has_f80 {
        blocks.push(F80_SHIMS.to_string());
    }
    for record in long_double_records(program) {
        blocks.push(render_record_def(record));
    }
    for shim in &shims {
        if shim.name == "__slate_strtold" {
            blocks.push(STRTOLD_SHIM.to_string());
        } else if shim.name.starts_with("__slate_f80_") {
            continue;
        } else if let Some(source) = render_typed_shim(shim) {
            blocks.push(source);
        }
    }
    blocks.join("\n")
}

fn long_double_records(program: &Program) -> Vec<&RecordDef> {
    program
        .items
        .iter()
        .filter_map(|item| match item {
            Item::Record(record) => Some(record),
            _ => None,
        })
        .filter(|record| {
            record
                .fields
                .iter()
                .any(|field| rust_type_has_long_double(&field.ty))
        })
        .collect()
}

fn render_record_def(record: &RecordDef) -> String {
    let keyword = if record.is_union { "union" } else { "struct" };
    let fields = record
        .fields
        .iter()
        .map(|field| {
            format!(
                "    {} {};",
                c_type_for_rust_type(&field.ty, false),
                field.name.as_str()
            )
        })
        .collect::<Vec<_>>()
        .join("\n");
    format!("{keyword} {} {{\n{fields}\n}};\n", record.name)
}

fn rust_type_has_long_double(ty: &Type) -> bool {
    match ty {
        Type::LongDouble => true,
        Type::Complex(inner)
        | Type::Ref { inner, .. }
        | Type::Slice(inner)
        | Type::Ptr { inner, .. }
        | Type::Array { elem: inner, .. } => rust_type_has_long_double(inner),
        Type::FnPtr { params, ret, .. } => {
            params.iter().any(rust_type_has_long_double) || rust_type_has_long_double(ret)
        }
        Type::Generic { args, .. } => args.iter().any(rust_type_has_long_double),
        _ => false,
    }
}

pub fn render_shim_c_source_for_names(names: &BTreeSet<String>) -> String {
    let mut blocks = vec!["#include <stdio.h>\n#include <stdlib.h>\n#include <math.h>".to_string()];
    if names
        .iter()
        .any(|name| name.contains("f80") || name == "__slate_strtold")
    {
        blocks.push(F80_SHIMS.to_string());
    }
    for name in names {
        if name == "__slate_strtold" {
            blocks.push(STRTOLD_SHIM.to_string());
        } else if let Some(trampoline) = render_trampoline(name) {
            blocks.push(trampoline);
        }
    }
    blocks.join("\n")
}
