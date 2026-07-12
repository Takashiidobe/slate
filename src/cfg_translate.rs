use crate::preprocess::{self, Branch, DirectiveKind, PredExpr};
use crate::{c_ast, cir, ctx, fixups, lower};
use std::collections::{BTreeMap, BTreeSet};
use std::path::Path;

#[derive(Debug, Clone)]
struct CfgConfig {
    rust_cfg: String,
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
    let source =
        std::fs::read_to_string(path).map_err(|e| format!("read {}: {e}", path.display()))?;
    let configs = match plan_configs(&source)? {
        // No conditional compilation: fall back to plain single-config lowering.
        None => return translate_one(path, &[]),
        Some(configs) => configs,
    };

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

/// Derive the whole-item cfg matrix from the recorded conditional regions
/// (slate-lq0.2). Returns `None` when the source has no conditional regions.
///
/// Refuses, with a diagnostic, anything that cannot be stitched as whole Rust
/// items: fragment cuts inside a function/record body, predicates that do not
/// map to a known Rust `cfg`, and (for now) nested or multiple top-level chains.
fn plan_configs(source: &str) -> Result<Option<Vec<CfgConfig>>, String> {
    let pp = preprocess::record(source, &BTreeMap::new());
    if pp.chains.is_empty() {
        return Ok(None);
    }
    if pp.chains.len() > 1 {
        return Err(format!(
            "translate-cfg: {} conditional chains found; whole-item merge supports a single \
             top-level #if/#endif chain (nested and sibling chains are not yet supported)",
            pp.chains.len()
        ));
    }
    let chain = &pp.chains[0];

    let depths = line_start_depths(source);
    let depth_at = |line: usize| depths.get(line.saturating_sub(1)).copied().unwrap_or(0);
    for branch in &chain.branches {
        if depth_at(branch.directive_line) > 0 {
            return Err(format!(
                "translate-cfg: conditional directive at line {} is inside a function or record \
                 body; only whole-item (top-level) #if regions can be merged as Rust cfg items",
                branch.directive_line
            ));
        }
    }
    if depth_at(chain.endif_line) > 0 {
        return Err(format!(
            "translate-cfg: #endif at line {} is inside a function or record body; only \
             whole-item (top-level) #if regions can be merged as Rust cfg items",
            chain.endif_line
        ));
    }

    for branch in &chain.branches {
        if branch.rust_cfg.is_none() {
            return Err(format!(
                "translate-cfg: predicate `{}` at line {} does not map to a known Rust cfg; \
                 cannot emit a whole-item cfg attribute",
                branch.raw_predicate.as_deref().unwrap_or("(else)"),
                branch.directive_line
            ));
        }
    }

    let chain_atoms = chain_atoms(&chain.branches);
    let mut configs = Vec::new();
    for (idx, branch) in chain.branches.iter().enumerate() {
        let defines = branch_defines(branch);
        let clang_args = pin_args(&chain_atoms, &defines);
        if !selects_only(source, idx, &defines) {
            return Err(format!(
                "translate-cfg: could not construct a configuration selecting the branch at \
                 line {} (predicate `{}`); negated or interdependent predicates are not yet \
                 supported",
                branch.directive_line,
                branch.raw_predicate.as_deref().unwrap_or("(else)")
            ));
        }
        configs.push(CfgConfig {
            rust_cfg: branch.rust_cfg.clone().unwrap(),
            clang_args,
        });
    }
    Ok(Some(configs))
}

/// Every macro named by a `defined(...)` atom anywhere in the chain.
fn chain_atoms(branches: &[Branch]) -> BTreeSet<String> {
    let mut atoms = BTreeSet::new();
    for branch in branches {
        collect_atoms(&branch.predicate, &mut atoms);
    }
    atoms
}

fn collect_atoms(expr: &PredExpr, out: &mut BTreeSet<String>) {
    match expr {
        PredExpr::Defined(name) => {
            out.insert(name.clone());
        }
        PredExpr::Not(inner) => collect_atoms(inner, out),
        PredExpr::And(items) | PredExpr::Or(items) => {
            for item in items {
                collect_atoms(item, out);
            }
        }
        PredExpr::Opaque(_) => {}
    }
}

/// Macros to `-D` for a branch: the positive `defined(...)` atoms of its own
/// predicate. `#else` defines nothing (every chain atom is undefined).
fn branch_defines(branch: &Branch) -> BTreeSet<String> {
    if branch.kind == DirectiveKind::Else {
        return BTreeSet::new();
    }
    let mut atoms = BTreeSet::new();
    collect_atoms(&branch.predicate, &mut atoms);
    atoms
}

/// Pin every chain atom: `-D` those in `defines`, `-U` the rest, so the active
/// branch is fully determined by this configuration.
fn pin_args(chain_atoms: &BTreeSet<String>, defines: &BTreeSet<String>) -> Vec<String> {
    chain_atoms
        .iter()
        .map(|atom| {
            if defines.contains(atom) {
                format!("-D{atom}")
            } else {
                format!("-U{atom}")
            }
        })
        .collect()
}

/// Confirm, via the oracle, that `defines` makes branch `idx` the single active
/// branch of the (only) chain — guards against the define-atoms heuristic being
/// wrong for negated or interdependent predicates.
fn selects_only(source: &str, idx: usize, defines: &BTreeSet<String>) -> bool {
    let macros: BTreeMap<String, String> = defines
        .iter()
        .map(|atom| (atom.clone(), "1".to_string()))
        .collect();
    let check = preprocess::record(source, &macros);
    let Some(chain) = check.chains.first() else {
        return false;
    };
    chain
        .branches
        .iter()
        .enumerate()
        .all(|(j, branch)| branch.active == Some(j == idx))
}

/// Brace depth at the start of each 1-based source line (index `line - 1`),
/// ignoring braces inside comments, string/char literals, and on preprocessor
/// lines.
fn line_start_depths(source: &str) -> Vec<i32> {
    let mut depths = Vec::new();
    let mut depth = 0i32;
    let mut in_block = false;
    for line in source.lines() {
        depths.push(depth);
        let is_pp = !in_block && line.trim_start().starts_with('#');
        let bytes = line.as_bytes();
        let mut i = 0;
        let mut in_str: Option<u8> = None;
        while i < bytes.len() {
            let b = bytes[i];
            if in_block {
                if b == b'*' && bytes.get(i + 1) == Some(&b'/') {
                    in_block = false;
                    i += 2;
                } else {
                    i += 1;
                }
                continue;
            }
            if let Some(quote) = in_str {
                if b == b'\\' {
                    i += 2;
                } else {
                    if b == quote {
                        in_str = None;
                    }
                    i += 1;
                }
                continue;
            }
            match b {
                b'/' if bytes.get(i + 1) == Some(&b'/') => break,
                b'/' if bytes.get(i + 1) == Some(&b'*') => {
                    in_block = true;
                    i += 2;
                }
                b'"' | b'\'' => {
                    in_str = Some(b);
                    i += 1;
                }
                b'{' if !is_pp => {
                    depth += 1;
                    i += 1;
                }
                b'}' if !is_pp => {
                    depth -= 1;
                    i += 1;
                }
                _ => i += 1,
            }
        }
    }
    depths
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
            push_cfg_item(&mut out, &config.rust_cfg, &text);
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

    #[test]
    fn brace_depth_is_zero_for_top_level_directives() {
        let src = "#if defined(_WIN32)\nint f(void) { return 1; }\n#endif\n";
        let depths = line_start_depths(src);
        assert_eq!(depths[0], 0); // #if line
        assert_eq!(depths[2], 0); // #endif line
    }

    #[test]
    fn brace_depth_is_nonzero_for_directive_inside_a_body() {
        let src = "int f(void) {\n#if defined(__linux__)\n    return 1;\n#endif\n}\n";
        let depths = line_start_depths(src);
        assert_eq!(depths[0], 0); // fn opening line
        assert!(depths[1] > 0); // #if line, inside the body
    }

    #[test]
    fn braces_in_strings_and_comments_do_not_count() {
        let src = "const char *s = \"{{{\"; // }}}\n#if defined(_WIN32)\nx\n#endif\n";
        let depths = line_start_depths(src);
        assert_eq!(depths[1], 0);
    }

    #[test]
    fn collects_defined_atoms_across_boolean_ops() {
        let expr = PredExpr::Or(vec![
            PredExpr::Defined("__x86_64__".into()),
            PredExpr::Not(Box::new(PredExpr::Defined("_M_X64".into()))),
        ]);
        let mut atoms = BTreeSet::new();
        collect_atoms(&expr, &mut atoms);
        assert!(atoms.contains("__x86_64__"));
        assert!(atoms.contains("_M_X64"));
    }
}
