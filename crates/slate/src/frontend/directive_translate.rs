use super::preprocess::{
    self, Branch, DirectiveDisposition, DirectiveKind, DirectiveName, PredExpr, Preprocessing,
};
use super::{self as frontend, c_ast};
use crate::backend;
use crate::backend::rust_ast::{Attr, Cfg, Expr, Item, Program, TraitRef, Type};
use crate::ctx;
use rayon::prelude::*;
use std::collections::{BTreeMap, BTreeSet};
use std::path::{Path, PathBuf};
use thiserror::Error;

const MAX_CFG_VARIANTS: usize = 16;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ConditionalBoundary {
    Directive,
    Endif,
}

impl std::fmt::Display for ConditionalBoundary {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::Directive => f.write_str("conditional directive"),
            Self::Endif => f.write_str("#endif"),
        }
    }
}

#[derive(Debug, Error)]
pub enum DirectiveError {
    #[error("read {path}: {source}")]
    Read {
        path: PathBuf,
        #[source]
        source: std::io::Error,
    },
    #[error("translate: {message}")]
    UnsupportedDirective { line: usize, message: String },
    #[error(
        "translate: {directive} is guarded by predicate `{predicate}` which does not map to a known Rust cfg"
    )]
    UnmappableDirectiveGuard {
        line: usize,
        directive: String,
        predicate: String,
    },
    #[error(
        "translate: {boundary} at line {line} is inside a function or record body; only whole-item (top-level) #if regions can be merged as Rust cfg items"
    )]
    ConditionalInBody {
        line: usize,
        boundary: ConditionalBoundary,
    },
    #[error(
        "translate: configuration variant cap exceeded: {variants} branch variants across {regions} conditional region(s), cap is {cap}; region at line {line} would make cfg recovery too expensive"
    )]
    VariantCapExceeded {
        variants: usize,
        regions: usize,
        cap: usize,
        line: usize,
    },
    #[error(
        "translate: predicate `{predicate}` at line {line} does not map to a known Rust cfg; cannot emit a whole-item cfg attribute"
    )]
    UnmappablePredicate { line: usize, predicate: String },
    #[error(
        "translate: could not construct a configuration selecting the branch at line {line} (predicate `{predicate}`); negated or interdependent predicates are not yet supported"
    )]
    UnselectableBranch { line: usize, predicate: String },
    #[error("preprocess {path}: {source}")]
    Preprocess {
        path: PathBuf,
        #[source]
        source: preprocess::PreprocessError,
    },
    #[error("load CIR for {path}: {source}")]
    Cir {
        path: PathBuf,
        #[source]
        source: crate::frontend::cir_input::ModuleError,
    },
    #[error("load Clang AST for {path}: {source}")]
    Ast {
        path: PathBuf,
        #[source]
        source: c_ast::AstError,
    },
    #[error("lowering failed for {path}:{diagnostics}")]
    Lowering {
        path: PathBuf,
        diagnostics: ctx::Diagnostics,
    },
    #[error("format generated Rust: {message}")]
    Format { message: String },
    #[error("resolve target `{target}`: {source}")]
    Target {
        target: String,
        #[source]
        source: super::toolchain::TargetError,
    },
}

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

pub fn translate_directives(path: &Path) -> Result<String, DirectiveError> {
    translate_directives_with_args(path, &[])
}

pub fn translate_directives_with_args(
    path: &Path,
    extra_args: &[String],
) -> Result<String, DirectiveError> {
    let program = translate_directives_program_with_args(path, extra_args)?;
    format_program(&program)
}

fn translate_directives_program_with_args(
    path: &Path,
    extra_args: &[String],
) -> Result<Program, DirectiveError> {
    let (source, _raw) = preprocess::read_source(path).map_err(|source| DirectiveError::Read {
        path: path.to_path_buf(),
        source,
    })?;
    let directive_pp = preprocess::record(&source, &BTreeMap::new());
    let directive_items = directive_items(&directive_pp)?;
    let plan = match plan_configs(&source)? {
        None => {
            let mut program = translate_one(path, extra_args)?.program;
            insert_directive_items(&mut program, directive_items);
            return Ok(program);
        }
        Some(plan) => plan,
    };

    let baseline = translate_one(path, extra_args)?;
    let mut variants = Vec::new();
    for config in plan.configs {
        let mut clang_args = extra_args.to_vec();
        clang_args.extend(config.clang_args.iter().cloned());
        let translation = translate_one(path, &clang_args)?;
        variants.push(Variant {
            config,
            program: translation.program,
            item_lines: translation.item_lines,
        });
    }
    let mut program = merge_variants(&baseline, &variants, &plan.pp);
    insert_directive_items(&mut program, directive_items);
    Ok(program)
}

struct TargetVariant {
    cfg: Cfg,
    program: Program,
}

fn os_cfg(os: &str) -> Cfg {
    match os {
        "windows" => Cfg::Flag("windows".into()),
        _ => Cfg::Opt {
            key: "target_os".into(),
            value: os.into(),
        },
    }
}

pub fn translate_targets_with_args(
    path: &Path,
    extra_args: &[String],
    targets: &[String],
) -> Result<String, DirectiveError> {
    let mut deduped_targets = Vec::with_capacity(targets.len());
    let mut seen_targets = BTreeSet::new();
    for target in targets {
        if seen_targets.insert(target.clone()) {
            deduped_targets.push(target.clone());
        }
    }

    let variants: Vec<TargetVariant> = deduped_targets
        .par_iter()
        .map(|target| {
            let config = super::toolchain::target_config(target).map_err(|source| {
                DirectiveError::Target {
                    target: target.clone(),
                    source,
                }
            })?;
            let program = super::toolchain::with_target_override(target, || {
                translate_directives_program_with_args(path, extra_args)
            })?;
            let mut atoms = vec![
                Cfg::Opt {
                    key: "target_arch".into(),
                    value: config.arch.into(),
                },
                os_cfg(config.os),
            ];
            if !config.env.is_empty() {
                atoms.push(Cfg::Opt {
                    key: "target_env".into(),
                    value: config.env.into(),
                });
            }
            Ok(TargetVariant {
                cfg: Cfg::All(atoms),
                program,
            })
        })
        .collect::<Result<_, DirectiveError>>()?;
    format_program(&merge_target_variants(&variants))
}

fn cfg_atom_key(cfg: &Cfg) -> Option<String> {
    match cfg {
        Cfg::Flag(name) => Some(format!("flag:{name}")),
        Cfg::Opt { key, .. } => Some(key.clone()),
        Cfg::Not(_) | Cfg::Any(_) | Cfg::All(_) => None,
    }
}

fn cfg_atoms(cfg: &Cfg) -> Option<Vec<Cfg>> {
    match cfg {
        Cfg::All(atoms) => atoms
            .iter()
            .map(|atom| cfg_atom_key(atom).map(|_| atom.clone()))
            .collect(),
        atom if cfg_atom_key(atom).is_some() => Some(vec![atom.clone()]),
        _ => None,
    }
}

fn factor_cfgs(cfgs: Vec<Cfg>) -> Cfg {
    if cfgs.len() == 1 {
        return cfgs.into_iter().next().unwrap();
    }
    let fallback = || Cfg::Any(cfgs.clone());

    let Some(atom_lists): Option<Vec<Vec<Cfg>>> = cfgs.iter().map(cfg_atoms).collect() else {
        return fallback();
    };
    let template_keys: Vec<String> = atom_lists[0]
        .iter()
        .map(|atom| cfg_atom_key(atom).expect("cfg_atoms only returns keyed atoms"))
        .collect();
    let same_shape = atom_lists.iter().all(|atoms| {
        atoms.len() == template_keys.len()
            && atoms
                .iter()
                .map(|atom| cfg_atom_key(atom).expect("cfg_atoms only returns keyed atoms"))
                .eq(template_keys.iter().cloned())
    });
    if !same_shape {
        return fallback();
    }

    let mut varying_positions = Vec::new();
    for (i, _) in template_keys.iter().enumerate() {
        let mut distinct: Vec<&Cfg> = Vec::new();
        for atoms in &atom_lists {
            if !distinct.contains(&&atoms[i]) {
                distinct.push(&atoms[i]);
            }
        }
        if distinct.len() > 1 {
            varying_positions.push(i);
        }
    }
    let [varying] = varying_positions[..] else {
        return fallback();
    };

    let mut result_atoms = Vec::with_capacity(template_keys.len());
    for i in 0..template_keys.len() {
        if i == varying {
            let mut values: Vec<Cfg> = Vec::new();
            for atoms in &atom_lists {
                if !values.contains(&atoms[i]) {
                    values.push(atoms[i].clone());
                }
            }
            result_atoms.push(Cfg::Any(values));
        } else {
            result_atoms.push(atom_lists[0][i].clone());
        }
    }
    Cfg::All(result_atoms)
}

fn merge_target_variants(variants: &[TargetVariant]) -> Program {
    let mut order = Vec::new();
    let mut seen_keys = BTreeSet::new();
    let mut by_key: BTreeMap<String, Vec<(usize, Item)>> = BTreeMap::new();

    for (vi, variant) in variants.iter().enumerate() {
        for item in &variant.program.items {
            if matches!(item, Item::CrateAttrs(_)) {
                continue;
            }
            let key = item_key(item);
            if seen_keys.insert(key.clone()) {
                order.push(key.clone());
            }
            by_key.entry(key).or_default().push((vi, item.clone()));
        }
    }

    let mut items = Vec::new();
    for item in &variants[0].program.items {
        if let Item::CrateAttrs(_) = item {
            items.push(item.clone());
        }
    }

    for key in order {
        let entries = &by_key[&key];
        let mut groups: Vec<(Item, Vec<usize>)> = Vec::new();
        for (vi, item) in entries {
            match groups.iter_mut().find(|(existing, _)| existing == item) {
                Some(group) => group.1.push(*vi),
                None => groups.push((item.clone(), vec![*vi])),
            }
        }
        for (item, vis) in groups {
            if vis.len() == variants.len() {
                items.push(item);
                continue;
            }
            let cfgs: BTreeSet<Cfg> = vis.iter().map(|&vi| variants[vi].cfg.clone()).collect();
            let cfg = factor_cfgs(cfgs.into_iter().collect());
            items.push(Item::Cfg {
                cfg,
                item: Box::new(item),
            });
        }
    }
    Program { items }
}

pub fn should_auto_expand(source: &str) -> bool {
    let pp = preprocess::record(source, &BTreeMap::new());
    if pp.chains.is_empty() {
        return false;
    }
    let depths = line_start_depths(source);
    let depth_at = |line: usize| depths.get(line.saturating_sub(1)).copied().unwrap_or(0);
    pp.chains.iter().all(|chain| {
        depth_at(chain.endif_line) == 0
            && chain.branches.iter().all(|branch| {
                depth_at(branch.directive_line) == 0 && branch.rust_cfg.is_some() && {
                    let mut atoms = BTreeSet::new();
                    collect_atoms(&branch.predicate, &mut atoms);
                    (branch.kind == DirectiveKind::Else || !atoms.is_empty())
                        && atoms.iter().all(|atom| preprocess::is_target_macro(atom))
                }
            })
    })
}

fn format_program(program: &Program) -> Result<String, DirectiveError> {
    backend::format_rust(&program.emit()).map_err(|message| DirectiveError::Format { message })
}

fn directive_items(pp: &Preprocessing) -> Result<Vec<Item>, DirectiveError> {
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
            return Err(DirectiveError::UnsupportedDirective {
                line: directive.line_start,
                message: directive.unsupported_message(),
            });
        }
        let cfg = directive
            .condition
            .as_ref()
            .map(|condition| {
                preprocess::pred_to_cfg(condition).ok_or_else(|| {
                    DirectiveError::UnmappableDirectiveGuard {
                        line: directive.line_start,
                        directive: match directive.name {
                            DirectiveName::Error => {
                                format!("#error at line {}", directive.line_start)
                            }
                            DirectiveName::Warning => {
                                format!("#warning at line {}", directive.line_start)
                            }
                            _ => directive.unsupported_message(),
                        },
                        predicate: preprocess::predicate_text(condition),
                    }
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
                attrs: vec![Attr::Deprecated(Some(message.into()))],
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

fn plan_configs(source: &str) -> Result<Option<CfgPlan>, DirectiveError> {
    let pp = preprocess::record(source, &BTreeMap::new());
    if pp.chains.is_empty() {
        return Ok(None);
    }

    let depths = line_start_depths(source);
    let depth_at = |line: usize| depths.get(line.saturating_sub(1)).copied().unwrap_or(0);
    for chain in &pp.chains {
        for branch in &chain.branches {
            if depth_at(branch.directive_line) > 0 {
                return Err(DirectiveError::ConditionalInBody {
                    line: branch.directive_line,
                    boundary: ConditionalBoundary::Directive,
                });
            }
        }
        if depth_at(chain.endif_line) > 0 {
            return Err(DirectiveError::ConditionalInBody {
                line: chain.endif_line,
                boundary: ConditionalBoundary::Endif,
            });
        }
    }

    let variant_count: usize = pp.chains.iter().map(|chain| chain.branches.len()).sum();
    if variant_count > MAX_CFG_VARIANTS {
        return Err(DirectiveError::VariantCapExceeded {
            variants: variant_count,
            regions: pp.chains.len(),
            cap: MAX_CFG_VARIANTS,
            line: pp
                .chains
                .get(MAX_CFG_VARIANTS)
                .or_else(|| pp.chains.last())
                .map(|chain| chain.open_line)
                .unwrap_or(1),
        });
    }

    for chain in &pp.chains {
        for branch in &chain.branches {
            if branch.rust_cfg.is_none() {
                return Err(DirectiveError::UnmappablePredicate {
                    line: branch.directive_line,
                    predicate: branch
                        .raw_predicate
                        .clone()
                        .unwrap_or_else(|| "(else)".into()),
                });
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
                return Err(DirectiveError::UnselectableBranch {
                    line: branch.directive_line,
                    predicate: branch
                        .raw_predicate
                        .clone()
                        .unwrap_or_else(|| "(else)".into()),
                });
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

fn translate_one(path: &Path, clang_args: &[String]) -> Result<Translation, DirectiveError> {
    let (source, raw) = preprocess::read_source(path).map_err(|source| DirectiveError::Read {
        path: path.to_path_buf(),
        source,
    })?;
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
    let input = preprocess::clang_input(path, &raw, &sanitized).map_err(|source| {
        DirectiveError::Preprocess {
            path: path.to_path_buf(),
            source,
        }
    })?;
    let mut frontend_args = clang_args.to_vec();
    frontend_args.extend_from_slice(input.extra_args());
    let module =
        crate::frontend::cir_input::emit_module(path, &frontend_args).map_err(|source| {
            DirectiveError::Cir {
                path: path.to_path_buf(),
                source,
            }
        })?;
    let unit = c_ast::parse_file_with_args(path, &frontend_args).map_err(|source| {
        DirectiveError::Ast {
            path: path.to_path_buf(),
            source,
        }
    })?;
    let item_lines = item_lines(&unit);

    let mut ctx = ctx::Ctx::default();
    let program = frontend::lower(&module, &unit, &mut ctx);
    if ctx.diagnostics.has_errors() {
        return Err(DirectiveError::Lowering {
            path: path.to_path_buf(),
            diagnostics: ctx.diagnostics,
        });
    }
    Ok(Translation {
        program: backend::apply(program),
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

pub fn item_key(item: &Item) -> String {
    match item {
        Item::CrateAttrs(_) => "crate-attrs".into(),
        Item::Comment(comment) => format!("comment:{}", comment.lines.join("\n")),
        Item::ExternBlock { .. } => "extern".into(),
        Item::Fn(f) => format!("fn:{}", f.name),
        Item::Static { name, .. } => format!("static:{name}"),
        Item::Const { name, .. } => format!("const:{name}"),
        Item::Mod { name } => format!("mod:{name}"),
        Item::InlineMod { name, .. } => format!("inline-mod:{name}"),
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
            match &im.trait_ {
                Some(TraitRef::Std(t)) => t.path().to_string(),
                Some(TraitRef::From(ty)) => format!("From<{}>", ty.render()),
                None => String::new(),
            },
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
