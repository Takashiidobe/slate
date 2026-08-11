use crate::backend::rust_ast::ExternFnDecl;

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
            if *tag == "ld" {
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

pub fn render_shim_c_source(shims: &[ExternFnDecl]) -> String {
    let mut blocks = vec!["#include <stdio.h>\n#include <stdlib.h>".to_string()];
    for shim in shims {
        if shim.name == "__slate_strtold" {
            blocks.push(STRTOLD_SHIM.to_string());
        } else if let Some(trampoline) = render_trampoline(&shim.name) {
            blocks.push(trampoline);
        }
    }
    blocks.join("\n")
}
