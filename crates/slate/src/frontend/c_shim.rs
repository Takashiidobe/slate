use crate::backend::rust_ast::{ExternDecl, ExternFnDecl, Item, Program};
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
        "usize" => "size_t",
        "f32" => "float",
        "f64" => "double",
        "bool" => "_Bool",
        "c" => "char",
        "v" => "void",
        "ld" => "double",
        "f80" => "__slate_f80",
        "cf80" => "__slate_cf80",
        "lq" => "long double",
        _ => "void *",
    }
    .to_string()
}

fn native_c_type(tag: &str) -> String {
    match tag {
        "f80" | "ld" => "long double".to_string(),
        "pf80" => "long double *".to_string(),
        "cf80" => "_Complex long double".to_string(),
        "pcf80" => "_Complex long double *".to_string(),
        _ => c_type_for_tag(tag),
    }
}

fn render_variadic_trampoline(name: &str) -> Option<String> {
    let (fixed, variadic) = name.strip_prefix("__slate_vcall__")?.split_once("__")?;
    let mut fixed = fixed.split('_');
    let ret = c_type_for_tag(fixed.next()?.strip_prefix('r')?);
    let fixed = fixed.map(c_type_for_tag).collect::<Vec<_>>();
    let variadic = variadic.split('_').collect::<Vec<_>>();
    let params = fixed
        .iter()
        .cloned()
        .chain(variadic.iter().map(|tag| c_type_for_tag(tag)))
        .enumerate()
        .map(|(i, ty)| format!("{ty} _{i}"))
        .collect::<Vec<_>>()
        .join(", ");
    let args = (0..fixed.len())
        .map(|i| format!("_{i}"))
        .chain(variadic.iter().enumerate().map(|(j, tag)| {
            let i = fixed.len() + j;
            if *tag == "f80" {
                format!("__slate_f80_load(_{i})")
            } else {
                format!("_{i}")
            }
        }))
        .collect::<Vec<_>>()
        .join(", ");
    let call = format!("(({ret} (*)({}, ...))_f)({args})", fixed.join(", "));
    let body = if ret == "void" {
        format!("{call};")
    } else {
        format!("return {call};")
    };
    Some(format!(
        "{ret} {name}(void *_f, {params}) {{\n    {body}\n}}\n"
    ))
}

fn render_trampoline(name: &str) -> Option<String> {
    if name.starts_with("__slate_vcall__") {
        return render_variadic_trampoline(name);
    }
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
            } else if *tag == "cf80" {
                format!("__slate_cf80_load(_{i})")
            } else if *tag == "pcf80" {
                format!("__slate_cf80_load(*_{i})")
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
    let undeclared = callback_callee.is_none() && header_for_shim_name(name).is_none();
    let target = if undeclared {
        format!("__slate_extern_{callee}")
    } else {
        callback_callee.unwrap_or(callee).to_string()
    };
    let call = format!("{target}({args})");
    let body = if ret_tag == "v" {
        format!("{call};")
    } else if ret_tag == "cf80" {
        format!("return __slate_cf80_store({call});")
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
    } else if undeclared {
        let native_params = arg_tags
            .iter()
            .map(|tag| native_c_type(tag))
            .collect::<Vec<_>>();
        let native_params = if native_params.is_empty() {
            "void".to_string()
        } else {
            native_params.join(", ")
        };
        format!(
            "extern {} {target}({native_params}) __asm__(\"{callee}\");\n",
            native_c_type(ret_tag)
        )
    } else {
        String::new()
    };
    Some(format!(
        "{prototype}{ret_c_type} {name}({params}) {{\n    {body}\n}}\n"
    ))
}

pub(crate) const F80_SHIMS: &str = include_str!("./shims/long_double.c");
pub(crate) const FENV_SHIMS: &str = include_str!("./shims/fenv.c");

pub fn collect_program_shims(program: &Program) -> Vec<ExternFnDecl> {
    program
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
        .collect()
}

fn header_for_shim_name(name: &str) -> Option<&'static str> {
    let rest = name.strip_prefix("__slate_")?;
    let callee = rest.split("__").next()?;
    let callee = callee.strip_prefix("cb_").unwrap_or(callee);
    crate::function_identity::Known::from_symbol(callee)
        .map(crate::function_identity::Known::header)
}

pub fn render_shim_c_source_for_names(names: &BTreeSet<String>) -> String {
    let headers = names
        .iter()
        .filter_map(|name| header_for_shim_name(name))
        .filter(|header| !header.starts_with("bits/"))
        .collect::<BTreeSet<_>>();
    let mut blocks = vec![
        std::iter::once("#define _GNU_SOURCE".to_string())
            .chain(
                headers
                    .into_iter()
                    .map(|header| format!("#include <{header}>")),
            )
            .collect::<Vec<_>>()
            .join("\n"),
    ];
    if names
        .iter()
        .any(|name| name.contains("f80") || name.starts_with("__slate_f128_"))
    {
        blocks.push(F80_SHIMS.to_string());
    }
    if names.iter().any(|name| name.starts_with("__slate_fenv_")) {
        blocks.push(FENV_SHIMS.to_string());
    }
    for name in names {
        if name.starts_with("__slate_fenv_") {
            continue;
        } else if let Some(trampoline) = render_trampoline(name) {
            blocks.push(trampoline);
        }
    }
    blocks.join("\n")
}
