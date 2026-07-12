use crate::{c_ast, cir, ctx, fixups, lower};
use std::collections::{BTreeMap, BTreeSet};
use std::path::Path;

#[derive(Debug, Clone)]
struct CfgConfig {
    name: &'static str,
    rust_cfg: &'static str,
    clang_args: Vec<String>,
}

#[derive(Debug, Clone)]
struct Variant {
    config: CfgConfig,
    items: Vec<TopItem>,
}

#[derive(Debug, Clone)]
struct TopItem {
    key: String,
    text: String,
}

pub fn translate_cfg(path: &Path) -> Result<String, String> {
    let configs = configs_for(path)?;
    let mut variants = Vec::new();
    for config in configs {
        let text = translate_one(path, &config.clang_args)?;
        variants.push(Variant {
            config,
            items: split_top_items(&text)?,
        });
    }
    merge_variants(&variants)
}

fn translate_one(path: &Path, clang_args: &[String]) -> Result<String, String> {
    let cir_text = cir::emit::emit_generic_with_args(path, clang_args)?;
    let module = cir::parse_module(&cir_text)?;
    let unit = c_ast::parse_file_with_args(path, clang_args)?;

    let mut ctx = ctx::Ctx::default();
    let program = lower::lower(&module, &unit, &mut ctx);
    for d in &ctx.diagnostics.items {
        eprintln!("{:?}: {}", d.severity, d.message);
    }
    if ctx.diagnostics.has_errors() {
        return Err(format!("lowering failed for {}", path.display()));
    }
    Ok(fixups::apply(program).emit())
}

fn configs_for(path: &Path) -> Result<Vec<CfgConfig>, String> {
    let name = path
        .file_name()
        .and_then(|name| name.to_str())
        .ok_or_else(|| format!("missing file name: {}", path.display()))?;
    match name {
        "os_targets.c" => Ok(vec![
            cfg(
                "windows",
                "windows",
                &["-D_WIN32", "-U__linux__", "-U__APPLE__"],
            ),
            cfg(
                "linux",
                "target_os = \"linux\"",
                &["-U_WIN32", "-D__linux__", "-U__APPLE__"],
            ),
            cfg(
                "apple",
                "target_vendor = \"apple\"",
                &["-U_WIN32", "-U__linux__", "-D__APPLE__"],
            ),
            cfg(
                "other",
                "not(any(windows, target_os = \"linux\", target_vendor = \"apple\"))",
                &["-U_WIN32", "-U__linux__", "-U__APPLE__"],
            ),
        ]),
        "arch_targets.c" => Ok(vec![
            cfg(
                "x86_64",
                "target_arch = \"x86_64\"",
                &[
                    "-D__x86_64__",
                    "-U_M_X64",
                    "-U__aarch64__",
                    "-U_M_ARM64",
                    "-U__arm__",
                    "-U_M_ARM",
                ],
            ),
            cfg(
                "aarch64",
                "target_arch = \"aarch64\"",
                &[
                    "-U__x86_64__",
                    "-U_M_X64",
                    "-D__aarch64__",
                    "-U_M_ARM64",
                    "-U__arm__",
                    "-U_M_ARM",
                ],
            ),
            cfg(
                "arm",
                "target_arch = \"arm\"",
                &[
                    "-U__x86_64__",
                    "-U_M_X64",
                    "-U__aarch64__",
                    "-U_M_ARM64",
                    "-D__arm__",
                    "-U_M_ARM",
                ],
            ),
            cfg(
                "other",
                "not(any(target_arch = \"x86_64\", target_arch = \"aarch64\", target_arch = \"arm\"))",
                &[
                    "-U__x86_64__",
                    "-U_M_X64",
                    "-U__aarch64__",
                    "-U_M_ARM64",
                    "-U__arm__",
                    "-U_M_ARM",
                ],
            ),
        ]),
        "ndebug.c" => Ok(vec![
            cfg("release", "not(debug_assertions)", &["-DNDEBUG"]),
            cfg("debug", "debug_assertions", &["-UNDEBUG"]),
        ]),
        _ => Err(format!(
            "translate-cfg: no built-in cfg matrix for {}",
            path.display()
        )),
    }
}

fn cfg(name: &'static str, rust_cfg: &'static str, args: &[&str]) -> CfgConfig {
    CfgConfig {
        name,
        rust_cfg,
        clang_args: args.iter().map(|arg| arg.to_string()).collect(),
    }
}

fn merge_variants(variants: &[Variant]) -> Result<String, String> {
    let mut order = Vec::new();
    let mut seen = BTreeSet::new();
    let mut by_key: BTreeMap<String, Vec<(CfgConfig, String)>> = BTreeMap::new();

    for variant in variants {
        for item in &variant.items {
            if seen.insert(item.key.clone()) {
                order.push(item.key.clone());
            }
            by_key
                .entry(item.key.clone())
                .or_default()
                .push((variant.config.clone(), item.text.clone()));
        }
    }

    let mut out = String::new();
    for key in order {
        let entries = by_key
            .remove(&key)
            .ok_or_else(|| format!("missing item group for {key}"))?;
        if all_text_equal(&entries) {
            push_item(&mut out, &entries[0].1);
            continue;
        }
        for (config, text) in entries {
            push_cfg_item(&mut out, config.rust_cfg, &text);
        }
    }
    Ok(out)
}

fn all_text_equal(entries: &[(CfgConfig, String)]) -> bool {
    entries
        .first()
        .is_none_or(|(_, first)| entries.iter().all(|(_, text)| text == first))
}

fn push_item(out: &mut String, item: &str) {
    if !out.is_empty() {
        out.push('\n');
    }
    out.push_str(item.trim_end());
    out.push('\n');
}

fn push_cfg_item(out: &mut String, rust_cfg: &str, item: &str) {
    if !out.is_empty() {
        out.push('\n');
    }
    out.push_str(&format!("#[cfg({rust_cfg})]\n"));
    out.push_str(item.trim_end());
    out.push('\n');
}

fn split_top_items(text: &str) -> Result<Vec<TopItem>, String> {
    let mut items = Vec::new();
    let mut current = Vec::new();
    let mut brace_depth = 0i32;
    let mut paren_depth = 0i32;

    for line in text.lines() {
        if current.is_empty() && line.trim().is_empty() {
            continue;
        }
        current.push(line.to_string());
        update_depths(line, &mut brace_depth, &mut paren_depth);
        if brace_depth == 0 && paren_depth == 0 && item_boundary(line) {
            let item_text = current.join("\n");
            items.push(TopItem {
                key: item_key(&item_text),
                text: item_text,
            });
            current.clear();
        }
    }

    if !current.is_empty() {
        if brace_depth != 0 || paren_depth != 0 {
            return Err("translate-cfg: unterminated generated Rust item".into());
        }
        let item_text = current.join("\n");
        items.push(TopItem {
            key: item_key(&item_text),
            text: item_text,
        });
    }
    Ok(items)
}

fn item_boundary(line: &str) -> bool {
    let trimmed = line.trim();
    trimmed.ends_with('}')
        || trimmed.ends_with(';')
        || trimmed == ")]"
        || (trimmed.starts_with("#![") && trimmed.ends_with(']'))
}

fn update_depths(line: &str, brace_depth: &mut i32, paren_depth: &mut i32) {
    for byte in line.bytes() {
        match byte {
            b'{' => *brace_depth += 1,
            b'}' => *brace_depth -= 1,
            b'(' => *paren_depth += 1,
            b')' => *paren_depth -= 1,
            _ => {}
        }
    }
}

fn item_key(text: &str) -> String {
    if text.trim_start().starts_with("unsafe extern") {
        return "extern".into();
    }
    if text.trim_start().starts_with("#![allow") {
        return "allow".into();
    }
    if let Some(name) = fn_name(text) {
        return format!("fn:{name}");
    }
    text.lines().next().unwrap_or_default().trim().to_string()
}

fn fn_name(text: &str) -> Option<String> {
    let line = text
        .lines()
        .find(|line| line.contains("fn "))
        .map(str::trim)?;
    let after = line.split_once("fn ")?.1;
    let name = after.split_once('(')?.0.trim();
    if name.is_empty() {
        None
    } else {
        Some(name.to_string())
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn splits_allow_extern_and_functions() {
        let text = "\
#![allow(
    dead_code
)]

unsafe extern \"C\" {
    fn printf(_0: *mut i8, ...) -> i32;
}

fn f() -> i32 {
    return 1;
}

fn main() {
    std::process::exit(f());
}
";
        let items = split_top_items(text).unwrap();
        assert_eq!(
            items
                .iter()
                .map(|item| item.key.as_str())
                .collect::<Vec<_>>(),
            ["allow", "extern", "fn:f", "fn:main"]
        );
    }

    #[test]
    fn splits_single_line_allow_attribute_before_function() {
        let text = "\
#![allow(dead_code)]

fn f() -> i32 {
    return 1;
}
";
        let items = split_top_items(text).unwrap();
        assert_eq!(
            items
                .iter()
                .map(|item| item.key.as_str())
                .collect::<Vec<_>>(),
            ["allow", "fn:f"]
        );
    }
}
