use crate::backend::rust_ast::ExternFnDecl;
use std::collections::BTreeSet;

fn c_type_for_tag(tag: &str) -> String {
    if let Some(inner) = tag.strip_prefix('p') {
        return match inner {
            "i8" | "u8" => "char *".to_string(),
            "x" => "void *".to_string(),
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
        "ld" => "double",
        "f80" => "__slate_f80",
        "lq" => "long double",
        _ => "void *",
    }
    .to_string()
}

fn render_trampoline(name: &str) -> Option<String> {
    let rest = name.strip_prefix("__slate_")?;
    let sep = rest.find("__")?;
    let callee = &rest[..sep];
    let tags: Vec<&str> = rest[sep + 2..].split('_').collect();
    let params = tags
        .iter()
        .enumerate()
        .map(|(i, tag)| format!("{} _{i}", c_type_for_tag(tag)))
        .collect::<Vec<_>>()
        .join(", ");
    let args = tags
        .iter()
        .enumerate()
        .map(|(i, tag)| {
            if *tag == "f80" {
                format!("__slate_f80_load(_{i})")
            } else if *tag == "ld" {
                format!("(long double)_{i}")
            } else {
                format!("_{i}")
            }
        })
        .collect::<Vec<_>>()
        .join(", ");
    Some(format!(
        "int {name}({params}) {{\n    return {callee}({args});\n}}\n"
    ))
}

const STRTOLD_SHIM: &str = "void __slate_strtold(char *nptr, char **endptr, double *out) {\n    *out = (double)strtold(nptr, endptr);\n}\n";

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
    let names: BTreeSet<String> = shims.iter().map(|shim| shim.name.clone()).collect();
    render_shim_c_source_for_names(&names)
}

pub fn render_shim_c_source_for_names(names: &BTreeSet<String>) -> String {
    let mut blocks = vec!["#include <stdio.h>\n#include <stdlib.h>".to_string()];
    if names.iter().any(|name| {
        name.starts_with("__slate_f80_") || name.contains("__f80") || name.ends_with("_f80")
    }) {
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
