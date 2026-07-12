use crate::preprocess::{self, Branch, DirectiveKind, PredExpr};
use crate::rust_ast::{Cfg, Item, Program};
use crate::{c_ast, cir, ctx, fixups, lower};
use std::collections::{BTreeMap, BTreeSet};
use std::path::Path;

#[derive(Debug, Clone)]
struct CfgConfig {
    rust_cfg: Cfg,
    clang_args: Vec<String>,
}

struct Variant {
    config: CfgConfig,
    program: Program,
}

pub fn translate_cfg(path: &Path) -> Result<String, String> {
    let source =
        std::fs::read_to_string(path).map_err(|e| format!("read {}: {e}", path.display()))?;
    let configs = match plan_configs(&source)? {
        // No conditional compilation: fall back to plain single-config lowering.
        None => return Ok(translate_one(path, &[])?.emit()),
        Some(configs) => configs,
    };

    let mut variants = Vec::new();
    for config in configs {
        let program = translate_one(path, &config.clang_args)?;
        variants.push(Variant { config, program });
    }
    Ok(merge_variants(&variants).emit())
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

fn translate_one(path: &Path, clang_args: &[String]) -> Result<Program, String> {
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
    Ok(fixups::apply(program))
}

/// Stitch the per-config programs into one: items that are identical across
/// every config in which they appear are emitted once; items that differ are
/// emitted once per config, each wrapped in its `#[cfg(..)]` gate. Item identity
/// is [`item_key`]; first-seen order is preserved.
fn merge_variants(variants: &[Variant]) -> Program {
    let mut order = Vec::new();
    let mut seen = BTreeSet::new();
    let mut by_key: BTreeMap<String, Vec<(Cfg, Item)>> = BTreeMap::new();

    for variant in variants {
        for item in &variant.program.items {
            let key = item_key(item);
            if seen.insert(key.clone()) {
                order.push(key.clone());
            }
            by_key
                .entry(key)
                .or_default()
                .push((variant.config.rust_cfg.clone(), item.clone()));
        }
    }

    let mut items = Vec::new();
    for key in order {
        let entries = by_key.remove(&key).expect("key recorded but group absent");
        if all_items_equal(&entries) {
            items.push(entries.into_iter().next().unwrap().1);
        } else {
            for (cfg, item) in entries {
                items.push(Item::Cfg {
                    cfg,
                    item: Box::new(item),
                });
            }
        }
    }
    Program { items }
}

fn all_items_equal(entries: &[(Cfg, Item)]) -> bool {
    let mut rendered = entries.iter().map(|(_, item)| render_item(item));
    match rendered.next() {
        None => true,
        Some(first) => rendered.all(|item| item == first),
    }
}

fn render_item(item: &Item) -> String {
    Program {
        items: vec![item.clone()],
    }
    .emit()
}

/// A stable identity for an item across configs, so the same logical item
/// (e.g. `fn os_code`) is grouped even when its body differs per config.
fn item_key(item: &Item) -> String {
    match item {
        Item::CrateAttrs(_) => "crate-attrs".into(),
        Item::ExternBlock { .. } => "extern".into(),
        Item::Func(f) => format!("fn:{}", f.name),
        Item::Fn(f) => format!("fn:{}", f.name),
        Item::Static { name, .. } => format!("static:{name}"),
        Item::Mod { name } => format!("mod:{name}"),
        Item::Use { path } => format!(
            "use:{}",
            path.segments
                .iter()
                .map(|s| s.as_str())
                .collect::<Vec<_>>()
                .join("::")
        ),
        Item::Enum(consts) => format!(
            "enum:{}",
            consts.first().map(|c| c.name.as_str()).unwrap_or_default()
        ),
        Item::Record(r) => format!("record:{}", r.name),
        Item::Struct(s) => format!("struct:{}", s.name),
        Item::Impl(im) => format!("impl:{}", im.self_ty.render()),
        Item::Cfg { item, .. } => item_key(item),
        Item::Raw(s) => s.lines().next().unwrap_or_default().trim().to_string(),
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    use crate::rust_ast::{FnDef, Visibility};

    fn fn_item(name: &str, ret: i64) -> Item {
        Item::Fn(FnDef {
            vis: Visibility::Private,
            unsafe_extern_c: false,
            name: name.to_string(),
            params: Vec::new(),
            ret: None,
            body: vec![crate::rust_ast::IndentStmt {
                depth: 1,
                stmt: crate::rust_ast::Stmt::Return(Some(crate::rust_ast::Expr::Value(
                    crate::rust_ast::RustValue::I64(ret),
                ))),
            }],
        })
    }

    #[test]
    fn item_key_groups_same_named_functions() {
        assert_eq!(item_key(&fn_item("os_code", 10)), "fn:os_code");
        assert_eq!(item_key(&fn_item("os_code", 20)), "fn:os_code");
        assert_eq!(item_key(&Item::CrateAttrs(Vec::new())), "crate-attrs");
    }

    #[test]
    fn differing_items_are_gated_and_identical_items_collapse() {
        let variants = vec![
            Variant {
                config: CfgConfig {
                    rust_cfg: Cfg::Flag("windows".into()),
                    clang_args: Vec::new(),
                },
                program: Program {
                    items: vec![fn_item("os_code", 10), fn_item("main", 0)],
                },
            },
            Variant {
                config: CfgConfig {
                    rust_cfg: Cfg::Opt {
                        key: "target_os".into(),
                        value: "linux".into(),
                    },
                    clang_args: Vec::new(),
                },
                program: Program {
                    items: vec![fn_item("os_code", 20), fn_item("main", 0)],
                },
            },
        ];
        let out = merge_variants(&variants).emit();
        assert!(out.contains("#[cfg(windows)]\nfn os_code()"));
        assert!(out.contains("#[cfg(target_os = \"linux\")]\nfn os_code()"));
        // `main` is identical across configs, so it is emitted once, ungated.
        assert_eq!(out.matches("fn main()").count(), 1);
        assert!(!out.contains("#[cfg(windows)]\nfn main"));
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
