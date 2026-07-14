use crate::preprocess::{self, Branch, DirectiveKind, PredExpr, Preprocessing};
use crate::rust_ast::{Cfg, Item, Program};
use crate::{c_ast, cir, ctx, fixups, lower};
use std::collections::{BTreeMap, BTreeSet};
use std::path::Path;

const MAX_CFG_VARIANTS: usize = 16;

#[derive(Debug, Clone)]
struct CfgConfig {
    rust_cfg: Cfg,
    clang_args: Vec<String>,
    chain_idx: usize,
    branch_idx: usize,
}

struct Variant {
    config: CfgConfig,
    program: Program,
    item_lines: BTreeMap<String, usize>,
}

struct Translation {
    program: Program,
    item_lines: BTreeMap<String, usize>,
}

struct CfgPlan {
    pp: Preprocessing,
    configs: Vec<CfgConfig>,
}

pub fn translate_cfg(path: &Path) -> Result<String, String> {
    let source =
        std::fs::read_to_string(path).map_err(|e| format!("read {}: {e}", path.display()))?;
    let plan = match plan_configs(&source)? {
        // No conditional compilation: fall back to plain single-config lowering.
        None => return Ok(translate_one(path, &[])?.program.emit()),
        Some(plan) => plan,
    };

    let baseline = translate_one(path, &[])?;
    let mut variants = Vec::new();
    for config in plan.configs {
        let translation = translate_one(path, &config.clang_args)?;
        variants.push(Variant {
            config,
            program: translation.program,
            item_lines: translation.item_lines,
        });
    }
    Ok(merge_variants(&baseline, &variants, &plan.pp).emit())
}

/// Derive the whole-item cfg matrix from the recorded conditional regions
/// (slate-lq0.2). Returns `None` when the source has no conditional regions.
///
/// Refuses, with a diagnostic, anything that cannot be stitched as whole Rust
/// items: fragment cuts inside a function/record body, predicates that do not
/// map to a known Rust `cfg`, or a plan that exceeds the bounded variant cap.
fn plan_configs(source: &str) -> Result<Option<CfgPlan>, String> {
    let pp = preprocess::record(source, &BTreeMap::new());
    if pp.chains.is_empty() {
        return Ok(None);
    }

    let depths = line_start_depths(source);
    let depth_at = |line: usize| depths.get(line.saturating_sub(1)).copied().unwrap_or(0);
    for chain in &pp.chains {
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
    }

    let variant_count: usize = pp.chains.iter().map(|chain| chain.branches.len()).sum();
    if variant_count > MAX_CFG_VARIANTS {
        return Err(format!(
            "translate-cfg: configuration variant cap exceeded: {variant_count} branch variants \
             across {} conditional region(s), cap is {MAX_CFG_VARIANTS}; region at line {} would \
             make cfg recovery too expensive",
            pp.chains.len(),
            pp.chains
                .get(MAX_CFG_VARIANTS)
                .or_else(|| pp.chains.last())
                .map(|chain| chain.open_line)
                .unwrap_or(1)
        ));
    }

    for chain in &pp.chains {
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
    }

    let mut configs = Vec::new();
    for (chain_idx, chain) in pp.chains.iter().enumerate() {
        for (branch_idx, branch) in chain.branches.iter().enumerate() {
            let selected = selected_branches(&pp, chain_idx, branch_idx);
            let mut atoms = BTreeSet::new();
            let mut defines = BTreeSet::new();
            for (selected_chain, selected_branch) in &selected {
                let selected_chain = &pp.chains[*selected_chain];
                let selected_branch = &selected_chain.branches[*selected_branch];
                atoms.extend(chain_atoms(&selected_chain.branches));
                defines.extend(branch_defines(selected_branch));
            }
            let clang_args = pin_args(&atoms, &defines);
            if !selects_only(source, &selected, &defines) {
                return Err(format!(
                    "translate-cfg: could not construct a configuration selecting the branch at \
                     line {} (predicate `{}`); negated or interdependent predicates are not yet \
                     supported",
                    branch.directive_line,
                    branch.raw_predicate.as_deref().unwrap_or("(else)")
                ));
            }
            configs.push(CfgConfig {
                rust_cfg: selected_cfg(&pp, &selected),
                clang_args,
                chain_idx,
                branch_idx,
            });
        }
    }
    Ok(Some(CfgPlan { pp, configs }))
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

/// Macros to `-D` for a branch: positive `defined(...)` atoms outside negation.
/// `#else` defines nothing (every chain atom is undefined).
fn branch_defines(branch: &Branch) -> BTreeSet<String> {
    if branch.kind == DirectiveKind::Else {
        return BTreeSet::new();
    }
    let mut atoms = BTreeSet::new();
    collect_positive_atoms(&branch.predicate, true, &mut atoms);
    atoms
}

fn collect_positive_atoms(expr: &PredExpr, positive: bool, out: &mut BTreeSet<String>) {
    match expr {
        PredExpr::Defined(name) if positive => {
            out.insert(name.clone());
        }
        PredExpr::Defined(_) => {}
        PredExpr::Not(inner) => collect_positive_atoms(inner, !positive, out),
        PredExpr::And(items) | PredExpr::Or(items) => {
            for item in items {
                collect_positive_atoms(item, positive, out);
            }
        }
        PredExpr::Opaque(_) => {}
    }
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

fn selected_branches(
    pp: &Preprocessing,
    chain_idx: usize,
    branch_idx: usize,
) -> Vec<(usize, usize)> {
    let mut selected = Vec::new();
    let mut current = Some((chain_idx, branch_idx));
    while let Some((idx, branch)) = current {
        selected.push((idx, branch));
        current = parent_branch(pp, idx);
    }
    selected.reverse();
    selected
}

fn parent_branch(pp: &Preprocessing, chain_idx: usize) -> Option<(usize, usize)> {
    let chain = &pp.chains[chain_idx];
    pp.chains
        .iter()
        .enumerate()
        .filter(|(idx, parent)| {
            *idx != chain_idx
                && parent.open_line < chain.open_line
                && chain.endif_line < parent.endif_line
        })
        .max_by_key(|(_, parent)| parent.open_line)
        .and_then(|(parent_idx, parent)| {
            parent
                .branches
                .iter()
                .enumerate()
                .find(|(_, branch)| {
                    branch.body_start <= chain.open_line && chain.open_line <= branch.body_end
                })
                .map(|(branch_idx, _)| (parent_idx, branch_idx))
        })
}

fn selected_cfg(pp: &Preprocessing, selected: &[(usize, usize)]) -> Cfg {
    let mut cfgs = Vec::new();
    for (chain_idx, branch_idx) in selected {
        cfgs.push(
            pp.chains[*chain_idx].branches[*branch_idx]
                .rust_cfg
                .clone()
                .expect("planned branch has cfg"),
        );
    }
    match cfgs.len() {
        0 => Cfg::All(Vec::new()),
        1 => cfgs.remove(0),
        _ => Cfg::All(cfgs),
    }
}

/// Confirm, via the oracle, that `defines` makes the requested branch active in
/// every selected chain — guards against the define-atoms heuristic being wrong
/// for negated or interdependent predicates.
fn selects_only(source: &str, selected: &[(usize, usize)], defines: &BTreeSet<String>) -> bool {
    let macros: BTreeMap<String, String> = defines
        .iter()
        .map(|atom| (atom.clone(), "1".to_string()))
        .collect();
    let check = preprocess::record(source, &macros);
    selected.iter().all(|(chain_idx, branch_idx)| {
        let Some(chain) = check.chains.get(*chain_idx) else {
            return false;
        };
        chain
            .branches
            .iter()
            .enumerate()
            .all(|(idx, branch)| branch.active == Some(idx == *branch_idx))
    })
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

fn translate_one(path: &Path, clang_args: &[String]) -> Result<Translation, String> {
    let cir_text = cir::emit::emit_generic_with_args(path, clang_args)?;
    let module = cir::parse_module(&cir_text)?;
    let unit = c_ast::parse_file_with_args(path, clang_args)?;
    let item_lines = item_lines(&unit);

    let mut ctx = ctx::Ctx::default();
    let program = lower::lower(&module, &unit, &mut ctx);
    for d in &ctx.diagnostics.items {
        eprintln!("{:?}: {}", d.severity, d.message);
    }
    if ctx.diagnostics.has_errors() {
        return Err(format!("lowering failed for {}", path.display()));
    }
    Ok(Translation {
        program: fixups::apply(program),
        item_lines,
    })
}

fn item_lines(unit: &c_ast::Unit) -> BTreeMap<String, usize> {
    let mut lines = BTreeMap::new();
    for function in &unit.functions {
        if function.body.is_some()
            && let Some(loc) = function.loc
        {
            lines.insert(format!("fn:{}", function.name), loc.line as usize);
        }
    }
    lines
}

/// Stitch the per-config programs into one: items that are identical across
/// every config in which they appear are emitted once; items that differ are
/// emitted once per config, each wrapped in its `#[cfg(..)]` gate. Item identity
/// is [`item_key`]; first-seen order is preserved.
fn merge_variants(baseline: &Translation, variants: &[Variant], pp: &Preprocessing) -> Program {
    let mut gated = Vec::new();
    let mut emitted = BTreeSet::new();
    for chain_idx in 0..pp.chains.len() {
        for variant in variants
            .iter()
            .filter(|variant| variant.config.chain_idx == chain_idx)
        {
            let branch = &pp.chains[variant.config.chain_idx].branches[variant.config.branch_idx];
            for item in &variant.program.items {
                let key = item_key(item);
                let Some(line) = variant.item_lines.get(&key).copied() else {
                    continue;
                };
                if !line_in_direct_branch(pp, variant.config.chain_idx, branch, line) {
                    continue;
                }
                if emitted.insert((variant.config.rust_cfg.render(), key)) {
                    gated.push(Item::Cfg {
                        cfg: variant.config.rust_cfg.clone(),
                        item: Box::new(item.clone()),
                    });
                }
            }
        }
    }

    let mut items = Vec::new();
    for item in &baseline.program.items {
        if let Item::CrateAttrs(_) = item {
            items.push(item.clone())
        }
    }
    items.extend(gated);
    for item in &baseline.program.items {
        if matches!(item, Item::CrateAttrs(_)) {
            continue;
        }
        let key = item_key(item);
        let in_cfg_region = baseline
            .item_lines
            .get(&key)
            .is_some_and(|line| line_in_any_chain(pp, *line));
        if !in_cfg_region {
            items.push(item.clone());
        }
    }
    Program { items }
}

fn line_in_any_chain(pp: &Preprocessing, line: usize) -> bool {
    pp.chains
        .iter()
        .any(|chain| chain.open_line <= line && line <= chain.endif_line)
}

fn line_in_direct_branch(
    pp: &Preprocessing,
    chain_idx: usize,
    branch: &Branch,
    line: usize,
) -> bool {
    if line < branch.body_start || line > branch.body_end {
        return false;
    }
    !pp.chains.iter().enumerate().any(|(idx, child)| {
        idx != chain_idx
            && branch.body_start <= child.open_line
            && child.endif_line <= branch.body_end
            && child.open_line <= line
            && line <= child.endif_line
    })
}

#[cfg(test)]
fn merge_legacy_variants(variants: &[Variant]) -> Program {
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
        Item::Comment(comment) => format!("comment:{}", comment.lines.join("\n")),
        Item::ExternBlock { .. } => "extern".into(),
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
        Item::Enum(e) => format!("enum:{}", e.name),
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
            unsafe_: false,
            extern_c: false,
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
                    chain_idx: 0,
                    branch_idx: 0,
                },
                program: Program {
                    items: vec![fn_item("os_code", 10), fn_item("main", 0)],
                },
                item_lines: BTreeMap::new(),
            },
            Variant {
                config: CfgConfig {
                    rust_cfg: Cfg::Opt {
                        key: "target_os".into(),
                        value: "linux".into(),
                    },
                    clang_args: Vec::new(),
                    chain_idx: 0,
                    branch_idx: 1,
                },
                program: Program {
                    items: vec![fn_item("os_code", 20), fn_item("main", 0)],
                },
                item_lines: BTreeMap::new(),
            },
        ];
        let out = merge_legacy_variants(&variants).emit();
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
