use crate::preprocess::{
    self, Branch, DirectiveDisposition, DirectiveKind, DirectiveName, PredExpr, Preprocessing,
};
use crate::rust_ast::{Attr, Cfg, Expr, Item, Program, Type};
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

pub fn translate_directives(path: &Path) -> Result<String, String> {
    let source =
        std::fs::read_to_string(path).map_err(|e| format!("read {}: {e}", path.display()))?;
    let directive_pp = preprocess::record(&source, &BTreeMap::new());
    let directive_items = directive_items(&directive_pp)?;
    let plan = match plan_configs(&source)? {
        None => {
            let mut program = translate_one(path, &[])?.program;
            insert_directive_items(&mut program, directive_items);
            return Ok(program.emit());
        }
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
    let mut program = merge_variants(&baseline, &variants, &plan.pp);
    insert_directive_items(&mut program, directive_items);
    Ok(program.emit())
}

fn directive_items(pp: &Preprocessing) -> Result<Vec<Item>, String> {
    let mut items = Vec::new();
    let mut warning_index = 0;
    for directive in pp.directives.iter().filter(|directive| {
        matches!(
            directive.name,
            DirectiveName::Error | DirectiveName::Warning
        ) || directive.disposition() == DirectiveDisposition::UnsupportedSemantic
    }) {
        if directive.is_poison_pragma() {
            continue;
        }
        if directive.is_clang_resolved_pragma() && directive.condition.is_none() {
            continue;
        }
        if directive.disposition() == DirectiveDisposition::UnsupportedSemantic
            && directive.condition.is_none()
        {
            return Err(format!(
                "translate-directives: {}",
                directive.unsupported_message()
            ));
        }
        let cfg = directive
            .condition
            .as_ref()
            .map(|condition| {
                preprocess::pred_to_cfg(condition).ok_or_else(|| {
                    format!(
                        "translate-directives: {} is guarded by predicate `{}` which does not map to a known Rust cfg",
                        match directive.name {
                            DirectiveName::Error => format!("#error at line {}", directive.line_start),
                            DirectiveName::Warning => format!("#warning at line {}", directive.line_start),
                            _ => directive.unsupported_message(),
                        },
                        preprocess::predicate_text(condition)
                    )
                })
            })
            .transpose()?;
        if directive.name == DirectiveName::Warning {
            items.extend(warning_items(
                &directive.raw_payload,
                warning_index,
                cfg,
                WarningBackend::Standalone,
            ));
            warning_index += 1;
            continue;
        }
        let message = if directive.name == DirectiveName::Error {
            directive.raw_payload.clone()
        } else {
            directive.unsupported_message()
        };
        let item = Item::Macro {
            name: "compile_error".into(),
            args: vec![Expr::Str(message)],
        };
        items.push(cfg.map_or(item.clone(), |cfg| Item::Cfg {
            cfg,
            item: Box::new(item),
        }));
    }
    Ok(items)
}

#[derive(Debug, Clone, Copy)]
pub enum WarningBackend {
    Standalone,
    SupportMacro,
}

pub fn warning_items(
    message: &str,
    index: usize,
    cfg: Option<Cfg>,
    backend: WarningBackend,
) -> Vec<Item> {
    let items = match backend {
        WarningBackend::Standalone => vec![
            Item::Const {
                attrs: vec![Attr::Deprecated(message.into())],
                name: format!("__SLATE_WARNING_{index}"),
                ty: Type::Unit,
                init: Expr::Block(Box::default()),
            },
            Item::Const {
                attrs: Vec::new(),
                name: "_".into(),
                ty: Type::Unit,
                init: Expr::Var(format!("__SLATE_WARNING_{index}").into()),
            },
        ],
        WarningBackend::SupportMacro => vec![Item::Macro {
            name: "slate_support::warning".into(),
            args: vec![Expr::Str(message.into())],
        }],
    };
    match cfg {
        Some(cfg) => items
            .into_iter()
            .map(|item| Item::Cfg {
                cfg: cfg.clone(),
                item: Box::new(item),
            })
            .collect(),
        None => items,
    }
}

pub fn insert_directive_items(program: &mut Program, items: Vec<Item>) {
    let index = program
        .items
        .iter()
        .take_while(|item| matches!(item, Item::CrateAttrs(_)))
        .count();
    program.items.splice(index..index, items);
}

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
                    "translate-directives: conditional directive at line {} is inside a function or record \
                     body; only whole-item (top-level) #if regions can be merged as Rust cfg items",
                    branch.directive_line
                ));
            }
        }
        if depth_at(chain.endif_line) > 0 {
            return Err(format!(
                "translate-directives: #endif at line {} is inside a function or record body; only \
                 whole-item (top-level) #if regions can be merged as Rust cfg items",
                chain.endif_line
            ));
        }
    }

    let variant_count: usize = pp.chains.iter().map(|chain| chain.branches.len()).sum();
    if variant_count > MAX_CFG_VARIANTS {
        return Err(format!(
            "translate-directives: configuration variant cap exceeded: {variant_count} branch variants \
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
                    "translate-directives: predicate `{}` at line {} does not map to a known Rust cfg; \
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
                    "translate-directives: could not construct a configuration selecting the branch at \
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

fn chain_atoms(branches: &[Branch]) -> BTreeSet<String> {
    let mut atoms = BTreeSet::new();
    for branch in branches {
        collect_atoms(&branch.predicate, &mut atoms);
    }
    atoms
}

fn collect_atoms(expr: &PredExpr, out: &mut BTreeSet<String>) {
    match expr {
        PredExpr::Constant(_) => {}
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
        PredExpr::Constant(_) => {}
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
    let source =
        std::fs::read_to_string(path).map_err(|e| format!("read {}: {e}", path.display()))?;
    let pp = preprocess::record(&source, &BTreeMap::new());
    let sanitized: Vec<_> = pp
        .directives
        .iter()
        .filter(|directive| {
            matches!(
                directive.name,
                DirectiveName::Error | DirectiveName::Warning
            ) || directive.disposition() == DirectiveDisposition::UnsupportedSemantic
                && (!directive.is_clang_resolved_pragma()
                    || directive.condition.is_some() && !directive.is_poison_pragma())
        })
        .collect();
    let input = preprocess::clang_input(path, &source, &sanitized)?;
    let mut frontend_args = clang_args.to_vec();
    frontend_args.extend_from_slice(input.extra_args());
    let cir_text = cir::emit::emit_generic_with_args(path, &frontend_args)?;
    let module = cir::parse_module(&cir_text)?;
    let unit = c_ast::parse_file_with_args(path, &frontend_args)?;
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
    Program {
        items,
        ..Program::default()
    }
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

pub fn item_key(item: &Item) -> String {
    match item {
        Item::CrateAttrs(_) => "crate-attrs".into(),
        Item::Comment(comment) => format!("comment:{}", comment.lines.join("\n")),
        Item::ExternBlock { .. } => "extern".into(),
        Item::Fn(f) => format!("fn:{}", f.name),
        Item::Static { name, .. } => format!("static:{name}"),
        Item::Const { name, .. } => format!("const:{name}"),
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
        Item::Impl(im) => format!(
            "impl:{}:{}",
            im.trait_.map(|t| t.path()).unwrap_or(""),
            im.self_ty.render()
        ),
        Item::Macro { name, args } => format!(
            "macro:{name}:{}",
            args.iter().map(Expr::render).collect::<Vec<_>>().join(",")
        ),
        Item::Cfg { item, .. } => item_key(item),
        Item::SupportModule(module) => format!("support-module:{}", module.name),
    }
}
