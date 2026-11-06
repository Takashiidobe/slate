use super::cfg::{TargetConditions, holds};
use super::matrix::Matrix;
use super::splice::Variants;
use crate::backend::engine::macro_templates::rename_templates;
use crate::backend::engine::unlabeled_jump;
use crate::backend::link::signatures::{self, UnitCompiles};
use crate::backend::rust_ast::{
    Attr, Cfg, CrateAttr, Expr, ExternDecl, Item, MacroOutcome, MacroParam, Program, Stmt,
    macro_template_base,
};
use crate::backend::{self, Exports};
use crate::compile_commands::CompileCommand;
use crate::frontend::directive_translate::{factor_cfgs, item_key, target_cfg};
use crate::frontend::distinct_functions::{self, SectionCandidates};
use crate::frontend::{self, lowerer};
use rayon::prelude::*;
use slate_parser::files::Files;
use slate_parser::ir::{Linkage, Module};
use slate_parser::target_info::TargetInfo;
use std::collections::{BTreeMap, BTreeSet, HashMap, HashSet};
use std::fmt::Write;
use std::path::{Path, PathBuf};
use std::sync::{Arc, Mutex};

const MAX_RELEVANT_FEATURES: usize = 12;
const MAX_ENUMERATED_FEATURES: usize = 20;
const MAX_REPORTED_MISMATCHES: usize = 20;
const MAX_EXHAUSTIVE_PROJECTIONS: usize = 1024;
const SAMPLE_SEED: u64 = 0x5eed_0f5a_a7e0_0001;
const UNTRANSLATED_TARGET: &str = "this crate was not translated for this target";

pub struct Unit {
    pub stem: String,
    pub path: PathBuf,
    pub module: Module,
    pub files: Files,
}

pub struct Translated {
    pub programs: Vec<(String, Program)>,
    pub shared_types: Option<Program>,
    pub section_candidates: Vec<SectionCandidates>,
}

#[derive(Clone, Copy, PartialEq, Eq)]
pub enum Sections {
    ByProject,
    Deferred,
}

pub fn rust_ident(name: &str) -> String {
    let mut out: String = name
        .chars()
        .map(|c| {
            if c.is_ascii_alphanumeric() || c == '_' {
                c
            } else {
                '_'
            }
        })
        .collect();
    if out.is_empty() || out.starts_with(|c: char| c.is_ascii_digit()) {
        out.insert(0, '_');
    }
    out
}

pub fn collect_all_errors<T>(results: Vec<Result<T, String>>) -> Result<Vec<T>, String> {
    let mut values = Vec::new();
    let mut errors = Vec::new();
    for result in results {
        match result {
            Ok(value) => values.push(value),
            Err(error) => errors.push(error),
        }
    }
    if errors.is_empty() {
        Ok(values)
    } else {
        Err(errors.join("\n"))
    }
}

type CommonOwners = BTreeMap<String, (String, bool)>;

fn common_owners<'a>(modules: impl Iterator<Item = (&'a str, &'a Module)>) -> CommonOwners {
    let mut owners: CommonOwners = BTreeMap::new();
    for (stem, module) in modules {
        for global in &module.globals {
            if !global.definition || !matches!(global.linkage, Linkage::External) {
                continue;
            }
            let strong = !global.common;
            owners
                .entry(global.variable.name.clone())
                .and_modify(|owner| {
                    if strong && !owner.1 {
                        *owner = (stem.to_string(), true);
                    }
                })
                .or_insert((stem.to_string(), strong));
        }
    }
    owners
}

fn imports_of(stem: &str, module: &Module, owners: &CommonOwners) -> BTreeSet<String> {
    module
        .globals
        .iter()
        .filter(|global| {
            global.common
                && owners
                    .get(global.variable.name.as_str())
                    .is_some_and(|(owner, _)| owner != stem)
        })
        .map(|global| global.variable.name.clone())
        .collect()
}

pub fn imported_commons(units: &[Unit]) -> BTreeMap<String, BTreeSet<String>> {
    let owners = common_owners(units.iter().map(|unit| (unit.stem.as_str(), &unit.module)));
    units
        .iter()
        .map(|unit| {
            (
                unit.stem.clone(),
                imports_of(&unit.stem, &unit.module, &owners),
            )
        })
        .collect()
}

pub fn translate_units(
    units: &[Unit],
    mut imported: BTreeMap<String, BTreeSet<String>>,
    explicit_docs_only: bool,
    liftable_root: Option<&str>,
    sections: Sections,
    named_c_types: bool,
) -> Result<Translated, String> {
    let jobs: Vec<_> = units
        .iter()
        .map(|unit| {
            let options = lowerer::LowerOptions {
                export_symbols: true,
                explicit_docs_only,
                imported_commons: imported.remove(&unit.stem).unwrap_or_default(),
                unit: unit.stem.clone(),
                named_c_types,
            };
            (unit, options)
        })
        .collect();
    let results: Vec<Result<_, String>> = jobs
        .into_par_iter()
        .map(|(unit, options)| {
            frontend::lower_module(&unit.module, &unit.files, &options)
                .map_err(|error| format!("{}: {error}", unit.path.display()))
        })
        .collect();
    let lowered = collect_all_errors(results)?;
    let address_taken: Vec<_> = units
        .iter()
        .zip(&lowered)
        .map(|(unit, lowered)| distinct_functions::Unit {
            module: &unit.module,
            address_taken: &lowered.address_taken,
        })
        .collect();
    let protected = match sections {
        Sections::ByProject => distinct_functions::mergeable_address_taken(&address_taken),
        Sections::Deferred => vec![BTreeSet::new(); units.len()],
    };
    let section_candidates = address_taken
        .iter()
        .map(distinct_functions::section_candidates)
        .collect();
    let exports = match liftable_root {
        Some(root) => Exports::Liftable {
            address_taken: distinct_functions::address_taken_symbols(&address_taken),
            modules: units
                .iter()
                .map(|unit| {
                    let module = (unit.stem != root).then(|| format!("__slate_unit_{}", unit.stem));
                    backend::rust_ast::Path::new(
                        std::iter::once("crate".to_string())
                            .chain(module)
                            .map(backend::rust_ast::Ident::new),
                    )
                })
                .collect(),
        },
        None => Exports::Fixed,
    };
    let mut programs: Vec<_> = lowered.into_iter().map(|lowered| lowered.program).collect();
    let shared_types = backend::apply_interprocedural(&mut programs, &exports);
    let programs = units
        .par_iter()
        .zip(programs)
        .zip(&protected)
        .map(|((unit, program), functions)| {
            let mut program = backend::apply_local(program, &unit.module.target);
            backend::place_in_distinct_sections(&mut program, &unit.stem, functions);
            (unit.stem.clone(), program)
        })
        .collect();
    Ok(Translated {
        programs,
        shared_types,
        section_candidates,
    })
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Projection {
    Skip,
    Sample(usize),
    All,
}

pub struct Merged {
    pub units: Vec<(String, Program)>,
    pub shared_macros: Option<Program>,
    pub root: Option<String>,
    pub features: BTreeSet<String>,
    pub target_guard: Item,
    report: String,
    pub mismatches: usize,
    pub signature_mismatches: usize,
}

impl Merged {
    pub fn render(&self) -> &str {
        &self.report
    }
}

#[derive(Clone, PartialEq, Eq, Hash)]
struct Entry {
    key: String,
    item: Item,
}

type Interner = Mutex<HashSet<Arc<Entry>>>;

fn intern(interner: &Interner, entry: Entry) -> Arc<Entry> {
    let mut known = interner.lock().expect("no compile panics");
    if let Some(found) = known.get(&entry) {
        return found.clone();
    }
    let entry = Arc::new(entry);
    known.insert(entry.clone());
    entry
}

fn entries(program: &Program) -> Vec<Entry> {
    let mut seen: HashMap<String, usize> = HashMap::new();
    let mut out = Vec::with_capacity(program.items.len());
    let mut push = |key: String, item: Item| {
        let count = seen.entry(key.clone()).or_default();
        let key = match *count {
            0 => key,
            nth => format!("{key}#{nth}"),
        };
        *count += 1;
        out.push(Entry { key, item });
    };
    for item in &program.items {
        match item {
            Item::CrateAttrs(_) => {}
            Item::ExternBlock { abi, decls } => {
                for decl in decls {
                    let name = match decl {
                        ExternDecl::Fn(decl) => decl.name.as_str(),
                        ExternDecl::Static { name, .. } => name.as_str(),
                    };
                    push(
                        format!("extern:{abi}:{name}"),
                        Item::ExternBlock {
                            abi: abi.clone(),
                            decls: vec![decl.clone()],
                        },
                    );
                }
            }
            item => push(item_key(item), item.clone()),
        }
    }
    out
}

fn crate_attrs(program: &Program) -> impl Iterator<Item = &CrateAttr> {
    program.items.iter().flat_map(|item| match item {
        Item::CrateAttrs(attrs) => attrs.as_slice(),
        _ => &[],
    })
}

#[derive(Clone)]
struct Compiled {
    mask: u32,
    candidates: SectionCandidates,
    entries: Vec<Arc<Entry>>,
    attrs: Vec<CrateAttr>,
    macro_outcomes: Arc<[MacroOutcome]>,
}

struct TargetUnit {
    interner: Interner,
    source: PathBuf,
    relevant: Vec<String>,
    compiled: Vec<Compiled>,
    infeasible: Vec<(u32, String)>,
    tried: BTreeSet<u32>,
    programs: Mutex<BTreeMap<u64, u32>>,
    reused: BTreeSet<u32>,
    defines_main: bool,
}

struct TargetBuild {
    name: String,
    cfg: Cfg,
    properties: BTreeSet<Cfg>,
    owners: CommonOwners,
    units: Vec<TargetUnit>,
}

fn subset(relevant: &[String], mask: u32) -> Vec<String> {
    relevant
        .iter()
        .enumerate()
        .filter(|(index, _)| mask >> index & 1 == 1)
        .map(|(_, name)| name.clone())
        .collect()
}

fn parse(command: CompileCommand) -> Result<Unit, String> {
    let stem = stem_of(&command.file)?;
    let path = command.file;
    frontend::preprocess_unit(&path, command.args)
        .and_then(frontend::PreprocessedUnit::parse_module)
        .map_err(|error| format!("{}: {error}", path.display()))
        .map(|(module, files)| Unit {
            stem,
            path,
            module,
            files,
        })
}

fn stem_of(path: &Path) -> Result<String, String> {
    path.file_stem()
        .and_then(|stem| stem.to_str())
        .map(rust_ident)
        .ok_or_else(|| format!("bad file stem: {}", path.display()))
}

fn defines_main(module: &Module) -> bool {
    module
        .functions
        .iter()
        .any(|function| function.name == "main" && function.body.is_some())
}

fn project_macro_outcomes(module: &Module, program: &Program) -> Arc<[MacroOutcome]> {
    let project_macros: HashSet<&str> = module
        .macros
        .definitions
        .iter()
        .filter(|definition| {
            definition.parameters.is_some() && definition.provenance.system_header.is_none()
        })
        .map(|definition| &*definition.name)
        .collect();
    program
        .macro_outcomes
        .iter()
        .filter(|outcome| project_macros.contains(outcome.name.as_str()))
        .cloned()
        .collect()
}

fn macro_report(builds: &[TargetBuild]) -> String {
    let mut per_macro: BTreeMap<&str, BTreeMap<Option<&'static str>, usize>> = BTreeMap::new();
    let mut per_reason: BTreeMap<&'static str, usize> = BTreeMap::new();
    let distinct = builds
        .iter()
        .flat_map(|build| &build.units)
        .flat_map(|unit| {
            unit.compiled
                .iter()
                .filter(|compiled| !unit.reused.contains(&compiled.mask))
        });
    for compiled in distinct {
        for outcome in compiled.macro_outcomes.iter() {
            *per_macro
                .entry(&outcome.name)
                .or_default()
                .entry(outcome.inline)
                .or_default() += 1;
            if let Some(reason) = outcome.inline {
                *per_reason.entry(reason).or_default() += 1;
            }
        }
    }
    let lifted = per_macro
        .values()
        .filter(|outcomes| outcomes.contains_key(&None))
        .count();
    let mut report = format!(
        "macros: {} function-like, {lifted} lifted at least once (expansions counted per distinct compile)\n",
        per_macro.len()
    );
    let mut reasons: Vec<_> = per_reason.into_iter().collect();
    reasons.sort_by_key(|(reason, count)| (std::cmp::Reverse(*count), *reason));
    for (reason, count) in reasons {
        let _ = writeln!(report, "  inline {reason}: {count}");
    }
    for (name, outcomes) in per_macro {
        let inline = outcomes
            .iter()
            .filter_map(|(reason, count)| reason.map(|reason| format!("{reason} {count}")))
            .collect::<Vec<_>>()
            .join(", ");
        let lifted = outcomes.get(&None).copied().unwrap_or(0);
        let _ = writeln!(report, "  macro {name}: {lifted} lifted, inline: {inline}");
    }
    report
}

fn compile_isolated(
    path: PathBuf,
    preprocessed: frontend::PreprocessedUnit,
    owners: &CommonOwners,
    interner: &Interner,
) -> Result<(Compiled, bool), String> {
    let stem = stem_of(&path)?;
    let (module, files) = preprocessed
        .parse_module()
        .map_err(|error| format!("{}: {error}", path.display()))?;
    let unit = Unit {
        stem,
        path,
        module,
        files,
    };
    let imported = BTreeMap::from([(
        unit.stem.clone(),
        imports_of(&unit.stem, &unit.module, owners),
    )]);
    let has_main = defines_main(&unit.module);
    let translated = translate_units(
        std::slice::from_ref(&unit),
        imported,
        false,
        None,
        Sections::Deferred,
        true,
    )?;
    let (_, program) = translated
        .programs
        .into_iter()
        .next()
        .expect("one unit translates to one program");
    let candidates = translated
        .section_candidates
        .into_iter()
        .next()
        .expect("one unit has one set of section candidates");
    let macro_outcomes = project_macro_outcomes(&unit.module, &program);
    Ok((
        Compiled {
            mask: 0,
            attrs: crate_attrs(&program).cloned().collect(),
            macro_outcomes,
            candidates,
            entries: entries(&program)
                .into_iter()
                .map(|entry| intern(interner, entry))
                .collect(),
        },
        has_main,
    ))
}

fn matrix_target_info(matrix: &Matrix, target: &str) -> Result<TargetInfo, String> {
    let known = matrix
        .targets
        .iter()
        .find(|known| known.name == target)
        .ok_or_else(|| format!("unknown matrix target {target}"))?;
    TargetInfo::for_triple(&known.triple).map_err(|error| error.to_string())
}

type ExploredRow = (Vec<String>, u64, Option<Result<(Compiled, bool), String>>);

fn explore_unit(
    matrix: &Matrix,
    target: &str,
    project: &Path,
    source: &Path,
    owners: &CommonOwners,
) -> (super::plan::UnitPlan, Vec<ExploredRow>, Interner) {
    let interner = Interner::default();
    let mut seen = BTreeSet::new();
    let mut rows: Vec<ExploredRow> = Vec::new();
    let mut on_run = |names: Vec<String>, relevant: usize, unit: frontend::PreprocessedUnit| {
        if relevant > MAX_RELEVANT_FEATURES {
            return;
        }
        let fingerprint = unit.fingerprint();
        let result = seen
            .insert(fingerprint)
            .then(|| compile_isolated(unit.path().to_path_buf(), unit, owners, &interner));
        rows.push((names, fingerprint, result));
    };
    let plan = super::plan::relevant_features(matrix, project, target, source, &mut on_run);
    (plan, rows, interner)
}

fn feature_mask(relevant: &[String], names: &[String]) -> u32 {
    names.iter().fold(0, |mask, name| {
        let bit = relevant
            .iter()
            .position(|known| known == name)
            .expect("explored features are relevant");
        mask | 1 << bit
    })
}

fn explored_unit(
    plan: &super::plan::UnitPlan,
    rows: Vec<ExploredRow>,
    interner: Interner,
) -> TargetUnit {
    let mut unit = TargetUnit {
        interner,
        source: plan.source.clone(),
        relevant: plan.relevant.clone(),
        compiled: Vec::new(),
        infeasible: Vec::new(),
        tried: BTreeSet::new(),
        programs: Mutex::new(BTreeMap::new()),
        reused: BTreeSet::new(),
        defines_main: false,
    };
    let mut programs = BTreeMap::new();
    let mut repeats = Vec::new();
    for (names, fingerprint, result) in rows {
        let mask = feature_mask(&unit.relevant, &names);
        let first = *programs.entry(fingerprint).or_insert(mask);
        let outcome = match result {
            Some(Ok((compiled, has_main))) => RowOutcome::Built(compiled, has_main),
            Some(Err(error)) => RowOutcome::Failed(error),
            None => RowOutcome::Repeats(first),
        };
        repeats.extend(unit.record(mask, outcome).map(|first| (mask, first)));
    }
    for (mask, first) in repeats {
        unit.reuse(mask, first);
    }
    unit.programs = Mutex::new(programs);
    unit
}

fn prepare_target(matrix: &Matrix, project: &Path, target: &str) -> Result<TargetBuild, String> {
    let defaults = matrix
        .commands(project, target, &[])
        .map_err(|error| error.to_string())?;
    let parsed: Vec<Unit> = defaults
        .into_par_iter()
        .filter_map(|command| parse(command).ok())
        .collect();
    let owners = common_owners(parsed.iter().map(|unit| (unit.stem.as_str(), &unit.module)));
    let explored: Vec<_> = matrix
        .sources
        .par_iter()
        .map(|source| explore_unit(matrix, target, project, source, &owners))
        .collect();
    if let Some((unit, error)) = explored
        .iter()
        .find_map(|(unit, ..)| unit.error.as_ref().map(|error| (unit, error)))
    {
        let first = error.lines().next().unwrap_or_default();
        return Err(format!(
            "target {target}: {} could not be planned: {first}",
            unit.source.display()
        ));
    }

    let mut built = Vec::with_capacity(explored.len());
    for (unit, rows, interner) in explored {
        if unit.relevant.len() > MAX_RELEVANT_FEATURES {
            return Err(format!(
                "target {target}: {} depends on {} features, the limit is {MAX_RELEVANT_FEATURES}",
                unit.source.display(),
                unit.relevant.len()
            ));
        }
        built.push(explored_unit(&unit, rows, interner));
    }
    let info = matrix_target_info(matrix, target)?;
    Ok(TargetBuild {
        name: target.to_string(),
        cfg: target_cfg(&info),
        properties: super::cfg::target_properties(&info),
        owners,
        units: built,
    })
}

fn is_valid(matrix: &Matrix, relevant: &[String], mask: u32) -> bool {
    !matrix.is_invalid_combination(&subset(relevant, mask))
}

fn valid_rows(matrix: &Matrix, relevant: &[String]) -> impl Iterator<Item = u32> {
    (0u32..1 << relevant.len()).filter(|mask| is_valid(matrix, relevant, *mask))
}

type Job = (usize, usize, u32);

impl TargetUnit {
    fn add_compiled(&mut self, compiled: Compiled) {
        let at = self
            .compiled
            .partition_point(|known| known.mask < compiled.mask);
        self.compiled.insert(at, compiled);
    }

    fn add_infeasible(&mut self, mask: u32, error: String) {
        let at = self.infeasible.partition_point(|known| known.0 < mask);
        self.infeasible.insert(at, (mask, error));
    }

    fn reuse(&mut self, mask: u32, first: u32) {
        self.reused.insert(mask);
        if let Some(found) = self.compiled.iter().find(|known| known.mask == first) {
            let compiled = Compiled {
                mask,
                ..found.clone()
            };
            self.add_compiled(compiled);
        } else if let Some((_, error)) = self.infeasible.iter().find(|known| known.0 == first) {
            let error = error.clone();
            self.add_infeasible(mask, error);
        }
    }

    fn record(&mut self, mask: u32, outcome: RowOutcome) -> Option<u32> {
        self.tried.insert(mask);
        match outcome {
            RowOutcome::Built(compiled, has_main) => {
                self.defines_main |= has_main;
                self.add_compiled(Compiled { mask, ..compiled });
                None
            }
            RowOutcome::Failed(error) => {
                let first = error.lines().next().unwrap_or_default().to_string();
                self.add_infeasible(mask, first);
                None
            }
            RowOutcome::Repeats(first) => Some(first),
        }
    }

    fn claim(&self, fingerprint: u64, mask: u32) -> Option<u32> {
        let mut programs = self.programs.lock().expect("no compile panics");
        let first = *programs.entry(fingerprint).or_insert(mask);
        (first != mask).then_some(first)
    }
}

enum RowOutcome {
    Built(Compiled, bool),
    Failed(String),
    Repeats(u32),
}

fn compile_row(
    matrix: &Matrix,
    project: &Path,
    build: &TargetBuild,
    unit: &TargetUnit,
    mask: u32,
) -> Result<RowOutcome, String> {
    let command = matrix
        .command(
            project,
            &build.name,
            &subset(&unit.relevant, mask),
            &unit.source,
        )
        .map_err(|error| error.to_string())?;
    let path = command.file;
    let preprocessed = frontend::preprocess_unit(&path, command.args)
        .map_err(|error| format!("{}: {error}", path.display()))?;
    if let Some(first) = unit.claim(preprocessed.fingerprint(), mask) {
        return Ok(RowOutcome::Repeats(first));
    }
    let (compiled, has_main) = compile_isolated(path, preprocessed, &build.owners, &unit.interner)?;
    Ok(RowOutcome::Built(compiled, has_main))
}

fn compile_rows(matrix: &Matrix, project: &Path, builds: &mut [TargetBuild], jobs: Vec<Job>) {
    let outcomes: Vec<RowOutcome> = jobs
        .par_iter()
        .map(|&(target, index, mask)| {
            let build = &builds[target];
            compile_row(matrix, project, build, &build.units[index], mask)
                .unwrap_or_else(RowOutcome::Failed)
        })
        .collect();
    let mut repeats = Vec::new();
    for ((target, index, mask), outcome) in jobs.into_iter().zip(outcomes) {
        let unit = &mut builds[target].units[index];
        if let Some(first) = unit.record(mask, outcome) {
            repeats.push((target, index, mask, first));
        }
    }
    for (target, index, mask, first) in repeats {
        builds[target].units[index].reuse(mask, first);
    }
}

fn place_sections(build: &mut TargetBuild) -> Result<(), String> {
    let taken: BTreeSet<String> = build
        .units
        .iter()
        .flat_map(|unit| &unit.compiled)
        .flat_map(|compiled| compiled.candidates.taken_symbols.iter().cloned())
        .collect();
    for unit in &mut build.units {
        let stem = stem_of(&unit.source)?;
        for compiled in &mut unit.compiled {
            let mut protected = compiled.candidates.taken_here.clone();
            protected.extend(
                compiled
                    .candidates
                    .exported
                    .iter()
                    .filter(|(symbol, _)| taken.contains(*symbol))
                    .map(|(_, name)| name.clone()),
            );
            for entry in &mut compiled.entries {
                if let Item::Fn(function) = &entry.item
                    && protected.contains(function.name.as_str())
                {
                    let mut placed = Entry::clone(entry);
                    backend::place_item_in_distinct_section(&mut placed.item, &stem);
                    *entry = intern(&unit.interner, placed);
                }
            }
        }
        unit.interner = Interner::default();
    }
    Ok(())
}

type Families = HashMap<String, String>;

fn template_families(builds: &[TargetBuild]) -> Families {
    type Interface<'a> = (&'a [MacroParam], bool);
    type Compiles = BTreeSet<(usize, usize, usize)>;
    let mut defined: BTreeMap<&str, (Interface, Compiles)> = BTreeMap::new();
    for (target, build) in builds.iter().enumerate() {
        for (unit_index, unit) in build.units.iter().enumerate() {
            for (compile, compiled) in unit.compiled.iter().enumerate() {
                for entry in &compiled.entries {
                    if let Item::MacroRules {
                        name,
                        params,
                        value,
                        ..
                    } = &entry.item
                        && macro_template_base(name).is_some()
                    {
                        defined
                            .entry(name)
                            .or_insert_with(|| ((params, *value), BTreeSet::new()))
                            .1
                            .insert((target, unit_index, compile));
                    }
                }
            }
        }
    }
    let mut by_base: BTreeMap<&str, Vec<_>> = BTreeMap::new();
    for (name, (interface, compiles)) in &defined {
        by_base
            .entry(macro_template_base(name).unwrap_or_default())
            .or_default()
            .push((*name, interface, compiles));
    }
    let mut families = Families::new();
    for (_, mut templates) in by_base {
        templates.sort_by(|a, b| b.2.len().cmp(&a.2.len()).then_with(|| a.0.cmp(b.0)));
        let mut chosen: Vec<(&str, &Interface, BTreeSet<_>)> = Vec::new();
        for (name, interface, compiles) in templates {
            match chosen
                .iter_mut()
                .find(|(_, known, occupied)| *known == interface && occupied.is_disjoint(compiles))
            {
                Some((leader, _, occupied)) => {
                    occupied.extend(compiles.iter().copied());
                    families.insert(name.to_string(), leader.to_string());
                }
                None => chosen.push((name, interface, compiles.clone())),
            }
        }
    }
    families
}

fn rename_families(build: &mut TargetBuild, families: &Families) {
    for unit in &mut build.units {
        let mut done: HashMap<*const Entry, Arc<Entry>> = HashMap::new();
        for compiled in &mut unit.compiled {
            for entry in &mut compiled.entries {
                let renamed = done.entry(Arc::as_ptr(entry)).or_insert_with(|| {
                    let mut item = entry.item.clone();
                    rename_templates(std::slice::from_mut(&mut item), families);
                    if item == entry.item {
                        return entry.clone();
                    }
                    let key = match item {
                        Item::MacroRules { .. } => item_key(&item),
                        _ => entry.key.clone(),
                    };
                    intern(&unit.interner, Entry { key, item })
                });
                *entry = renamed.clone();
            }
        }
    }
}

#[derive(Clone, Copy)]
struct Implicant {
    value: u32,
    dashes: u32,
}

impl Implicant {
    fn covers(&self, mask: u32) -> bool {
        mask & !self.dashes == self.value
    }
}

fn prime_implicants(cover: &BTreeSet<u32>, bits: usize) -> Vec<Implicant> {
    let mut current: BTreeSet<(u32, u32)> = cover.iter().map(|value| (0, *value)).collect();
    let mut primes = Vec::new();
    while !current.is_empty() {
        let mut next = BTreeSet::new();
        let mut merged = BTreeSet::new();
        for &(dashes, value) in &current {
            for bit in 0..bits {
                let flag = 1u32 << bit;
                if dashes & flag != 0 || value & flag != 0 {
                    continue;
                }
                if current.contains(&(dashes, value | flag)) {
                    merged.insert((dashes, value));
                    merged.insert((dashes, value | flag));
                    next.insert((dashes | flag, value));
                }
            }
        }
        primes.extend(
            current
                .iter()
                .filter(|entry| !merged.contains(entry))
                .map(|&(dashes, value)| Implicant { value, dashes }),
        );
        current = next;
    }
    primes
}

fn candidates(on: &BTreeSet<u32>, care: &BTreeSet<u32>, bits: usize) -> Vec<Implicant> {
    let cover: BTreeSet<u32> = (0u32..1 << bits)
        .filter(|mask| on.contains(mask) || !care.contains(mask))
        .collect();
    prime_implicants(&cover, bits)
}

fn select_cover(on: &BTreeSet<u32>, primes: &[Implicant]) -> Vec<Implicant> {
    let mut uncovered = on.clone();
    let mut chosen = Vec::new();
    while !uncovered.is_empty() {
        let best = primes
            .iter()
            .max_by_key(|implicant| {
                let gain = uncovered
                    .iter()
                    .filter(|mask| implicant.covers(**mask))
                    .count();
                (
                    gain,
                    implicant.dashes.count_ones(),
                    std::cmp::Reverse(implicant.value),
                )
            })
            .copied()
            .expect("every on-set assignment has a covering implicant");
        uncovered.retain(|mask| !best.covers(*mask));
        chosen.push(best);
    }
    chosen
}

fn minimize(on: &BTreeSet<u32>, care: &BTreeSet<u32>, bits: usize) -> Vec<Implicant> {
    select_cover(on, &candidates(on, care, bits))
}

fn feature_atom(name: &str) -> Cfg {
    Cfg::Opt {
        key: "feature".into(),
        value: name.into(),
    }
}

fn condition(implicants: &[Implicant], relevant: &[String]) -> Option<Cfg> {
    let mut terms = Vec::new();
    for implicant in implicants {
        let mut atoms = Vec::new();
        for (bit, name) in relevant.iter().enumerate() {
            let flag = 1u32 << bit;
            if implicant.dashes & flag != 0 {
                continue;
            }
            let atom = feature_atom(name);
            atoms.push(if implicant.value & flag != 0 {
                atom
            } else {
                Cfg::Not(Box::new(atom))
            });
        }
        if atoms.is_empty() {
            return None;
        }
        terms.push(if atoms.len() == 1 {
            atoms.remove(0)
        } else {
            Cfg::All(atoms)
        });
    }
    Some(if terms.len() == 1 {
        terms.remove(0)
    } else {
        Cfg::Any(terms)
    })
}

fn conjoin(left: Cfg, right: Cfg) -> Cfg {
    let mut atoms = match left {
        Cfg::All(atoms) => atoms,
        other => vec![other],
    };
    match right {
        Cfg::All(more) => atoms.extend(more),
        other => atoms.push(other),
    }
    Cfg::All(atoms)
}

fn combined_cfg(
    groups: BTreeMap<Option<Cfg>, u64>,
    present: u64,
    conditions: &TargetConditions,
) -> Option<Cfg> {
    if let [(condition, members)] = groups.iter().collect::<Vec<_>>().as_slice()
        && **members == present
    {
        return (*condition).clone();
    }
    let mut terms: Vec<Cfg> = groups
        .into_iter()
        .map(|(condition, members)| {
            let targets = conditions.select(members, present);
            match condition {
                Some(condition) => conjoin(targets, condition),
                None => targets,
            }
        })
        .collect();
    Some(if terms.len() == 1 {
        terms.remove(0)
    } else {
        Cfg::Any(terms)
    })
}

struct Value {
    entry: Arc<Entry>,
    on: Vec<BTreeSet<u32>>,
}

struct MergedEntry {
    cfg: Option<Cfg>,
    key: String,
    item: Item,
    binding: bool,
}

struct MergedUnit {
    stem: String,
    entries: Vec<MergedEntry>,
    attrs: Vec<CrateAttr>,
    gated: usize,
    duplicated: Vec<(String, usize)>,
    hoisted: Vec<(String, usize)>,
    spliced: Vec<(String, usize)>,
}

fn hoist_values(key: &str, values: &[Value]) -> Option<super::hoist::Hoisted> {
    if key.contains('#') {
        return None;
    }
    let variants = values
        .iter()
        .map(|value| match &value.entry.item {
            Item::Fn(function) => Some(function),
            _ => None,
        })
        .collect::<Option<Vec<_>>>()?;
    super::hoist::hoist(&variants)
}

fn unscoped_macros(targets: &[TargetBuild], index: usize) -> HashSet<String> {
    let mut bodies: BTreeMap<&str, Vec<(&[Stmt], bool)>> = BTreeMap::new();
    for target in targets {
        for compiled in &target.units[index].compiled {
            for entry in &compiled.entries {
                if let Item::MacroRules {
                    name, body, value, ..
                } = &entry.item
                {
                    bodies.entry(name).or_default().push((body, *value));
                }
            }
        }
    }
    let mut unscoped: HashSet<String> = HashSet::new();
    loop {
        let found: Vec<&str> = bodies
            .iter()
            .filter(|(name, _)| !unscoped.contains(**name))
            .filter(|(_, variants)| {
                variants.iter().any(|(body, value)| {
                    body.iter().any(|stmt| {
                        (!value && matches!(stmt, Stmt::Let { .. } | Stmt::LetIf { .. }))
                            || unlabeled_jump(stmt, &|name| unscoped.contains(name))
                    })
                })
            })
            .map(|(name, _)| *name)
            .collect();
        if found.is_empty() {
            return unscoped;
        }
        unscoped.extend(found.into_iter().map(str::to_string));
    }
}

fn splice_values(
    key: &str,
    values: &[Value],
    gate: &mut impl FnMut(&Variants, &Variants) -> Option<Cfg>,
    unscoped: &HashSet<String>,
) -> Option<(Item, Vec<(Variants, Item)>)> {
    if key.contains('#') {
        return None;
    }
    match &values[0].entry.item {
        Item::Fn(_) => {
            let variants = values
                .iter()
                .map(|value| match &value.entry.item {
                    Item::Fn(function) => Some(function),
                    _ => None,
                })
                .collect::<Option<Vec<_>>>()?;
            super::splice::splice_function(&variants, gate, &|name| unscoped.contains(name))
                .map(|spliced| (Item::Fn(spliced.function), spliced.bindings))
        }
        Item::Record(_) => {
            let variants = values
                .iter()
                .map(|value| match &value.entry.item {
                    Item::Record(record) => Some(record),
                    _ => None,
                })
                .collect::<Option<Vec<_>>>()?;
            super::splice::splice_record(&variants, gate)
                .map(|record| (Item::Record(record), Vec::new()))
        }
        _ => None,
    }
}

fn union_on(values: &[Value], chosen: &Variants, targets: usize) -> Vec<BTreeSet<u32>> {
    let mut on = vec![BTreeSet::new(); targets];
    for &variant in chosen {
        for (all, masks) in on.iter_mut().zip(&values[variant].on) {
            all.extend(masks);
        }
    }
    on
}

fn merge_unit(
    stem: &str,
    index: usize,
    targets: &[TargetBuild],
    conditions: &TargetConditions,
) -> MergedUnit {
    let mut order: Vec<String> = Vec::new();
    let mut table: HashMap<String, Vec<Value>> = HashMap::new();
    let mut attrs = Vec::new();
    for (target_index, target) in targets.iter().enumerate() {
        for compiled in &target.units[index].compiled {
            for attr in &compiled.attrs {
                if !attrs.contains(attr) {
                    attrs.push(attr.clone());
                }
            }
            let mut previous: Option<&str> = None;
            for entry in &compiled.entries {
                let values = table.entry(entry.key.clone()).or_insert_with(|| {
                    let at = previous
                        .and_then(|previous| order.iter().position(|key| key == previous))
                        .map_or(0, |position| position + 1);
                    order.insert(at, entry.key.clone());
                    Vec::new()
                });
                let position = values
                    .iter()
                    .position(|value| value.entry == *entry)
                    .unwrap_or_else(|| {
                        values.push(Value {
                            entry: entry.clone(),
                            on: vec![BTreeSet::new(); targets.len()],
                        });
                        values.len() - 1
                    });
                values[position].on[target_index].insert(compiled.mask);
                previous = Some(&entry.key);
            }
        }
    }
    let cares: Vec<BTreeSet<u32>> = targets
        .iter()
        .map(|target| {
            target.units[index]
                .compiled
                .iter()
                .map(|compiled| compiled.mask)
                .collect()
        })
        .collect();
    let mut out = MergedUnit {
        stem: stem.to_string(),
        entries: Vec::new(),
        attrs,
        gated: 0,
        duplicated: Vec::new(),
        hoisted: Vec::new(),
        spliced: Vec::new(),
    };
    let cfg_within = |on: &[BTreeSet<u32>], within: &[BTreeSet<u32>]| -> Option<Cfg> {
        let mut groups: BTreeMap<Option<Cfg>, u64> = BTreeMap::new();
        let mut present = 0u64;
        for (target_index, target) in targets.iter().enumerate() {
            if within[target_index].is_empty() {
                continue;
            }
            present |= 1 << target_index;
            let on = &on[target_index];
            if on.is_empty() {
                continue;
            }
            let relevant = &target.units[index].relevant;
            let implicants = minimize(on, &within[target_index], relevant.len());
            *groups.entry(condition(&implicants, relevant)).or_default() |= 1 << target_index;
        }
        combined_cfg(groups, present, conditions)
    };
    let cfg_of = |on: &[BTreeSet<u32>]| cfg_within(on, &cares);
    let unscoped = unscoped_macros(targets, index);
    for key in order {
        let values = table.remove(&key).expect("ordered key is in the table");
        if values.len() > 1
            && key.starts_with("fn:")
            && let Some(hoisted) = hoist_values(&key, &values)
        {
            out.hoisted.push((key.clone(), values.len()));
            let mut on = vec![BTreeSet::new(); targets.len()];
            for (value, binding) in values.iter().zip(hoisted.bindings) {
                for (all, variant) in on.iter_mut().zip(&value.on) {
                    all.extend(variant);
                }
                let cfg = cfg_of(&value.on);
                out.gated += usize::from(cfg.is_some());
                out.entries.push(MergedEntry {
                    cfg,
                    key: item_key(&binding),
                    item: binding,
                    binding: true,
                });
            }
            let cfg = cfg_of(&on);
            out.gated += usize::from(cfg.is_some());
            out.entries.push(MergedEntry {
                cfg,
                key,
                item: Item::Fn(hoisted.function),
                binding: false,
            });
            continue;
        }
        if values.len() > 1 {
            let mut cache: HashMap<(Variants, Variants), Option<Cfg>> = HashMap::new();
            let mut gate = |on: &Variants, within: &Variants| {
                cache
                    .entry((on.clone(), within.clone()))
                    .or_insert_with(|| {
                        cfg_within(
                            &union_on(&values, on, targets.len()),
                            &union_on(&values, within, targets.len()),
                        )
                    })
                    .clone()
            };
            if let Some((item, bindings)) = splice_values(&key, &values, &mut gate, &unscoped) {
                out.spliced.push((key.clone(), values.len()));
                for (on, binding) in bindings {
                    let cfg = cfg_of(&union_on(&values, &on, targets.len()));
                    out.gated += usize::from(cfg.is_some());
                    out.entries.push(MergedEntry {
                        cfg,
                        key: item_key(&binding),
                        item: binding,
                        binding: true,
                    });
                }
                let all: Variants = (0..values.len()).collect();
                let cfg = cfg_of(&union_on(&values, &all, targets.len()));
                out.gated += usize::from(cfg.is_some());
                out.entries.push(MergedEntry {
                    cfg,
                    key,
                    item,
                    binding: false,
                });
                continue;
            }
        }
        if values.len() > 1 && key.starts_with("fn:") {
            out.duplicated.push((key.clone(), values.len()));
        }
        for value in values {
            let cfg = cfg_of(&value.on);
            if cfg.is_some() && matches!(value.entry.item, Item::Comment(_)) {
                continue;
            }
            out.gated += usize::from(cfg.is_some());
            out.entries.push(MergedEntry {
                cfg,
                key: key.clone(),
                item: value.entry.item.clone(),
                binding: false,
            });
        }
    }
    out
}

fn program_of(unit: &MergedUnit) -> Program {
    let mut items: Vec<Option<Item>> = Vec::with_capacity(unit.entries.len() + 1);
    if !unit.attrs.is_empty() {
        items.push(Some(Item::CrateAttrs(unit.attrs.clone())));
    }
    let mut blocks: BTreeMap<(Option<Cfg>, String), (usize, Vec<ExternDecl>)> = BTreeMap::new();
    for entry in &unit.entries {
        if let Item::ExternBlock { abi, decls } = &entry.item {
            blocks
                .entry((entry.cfg.clone(), abi.clone()))
                .or_insert_with(|| {
                    items.push(None);
                    (items.len() - 1, Vec::new())
                })
                .1
                .extend(decls.iter().cloned());
            continue;
        }
        items.push(Some(gate(entry.cfg.clone(), entry.item.clone())));
    }
    for ((cfg, abi), (position, decls)) in blocks {
        items[position] = Some(gate(cfg, Item::ExternBlock { abi, decls }));
    }
    let (macros, mut items): (Vec<Item>, Vec<Item>) =
        items.into_iter().flatten().partition(|item| match item {
            Item::MacroRules { .. } => true,
            Item::Cfg { item, .. } => matches!(**item, Item::MacroRules { .. }),
            _ => false,
        });
    let at = items
        .iter()
        .position(|item| !matches!(item, Item::CrateAttrs(_) | Item::Comment(_)))
        .unwrap_or(items.len());
    items.splice(at..at, macros);
    Program {
        items,
        ..Default::default()
    }
}

fn macro_rules_name(item: &Item) -> Option<&str> {
    match item {
        Item::MacroRules { name, .. } => Some(name),
        Item::Cfg { item, .. } => macro_rules_name(item),
        _ => None,
    }
}

fn share_macros(units: &mut [(String, Program)]) -> Option<Program> {
    let mut definitions: BTreeMap<String, Vec<Vec<Item>>> = BTreeMap::new();
    for (_, program) in units.iter() {
        let mut in_unit: BTreeMap<String, Vec<Item>> = BTreeMap::new();
        for item in &program.items {
            if let Some(name) = macro_rules_name(item) {
                in_unit
                    .entry(name.to_string())
                    .or_default()
                    .push(item.clone());
            }
        }
        for (name, variants) in in_unit {
            definitions.entry(name).or_default().push(variants);
        }
    }
    let mut items = Vec::new();
    let mut shared: BTreeMap<String, Vec<Item>> = BTreeMap::new();
    for (name, in_units) in definitions {
        let mut best: Option<(&Vec<Item>, usize)> = None;
        for variants in &in_units {
            let count = in_units.iter().filter(|other| *other == variants).count();
            if count > 1 && best.is_none_or(|(_, known)| count > known) {
                best = Some((variants, count));
            }
        }
        if let Some((variants, _)) = best {
            items.extend(variants.iter().cloned());
            shared.insert(name, variants.clone());
        }
    }
    if items.is_empty() {
        return None;
    }
    for (_, program) in units {
        let defined = |name: &str| {
            let variants: Vec<&Item> = program
                .items
                .iter()
                .filter(|item| macro_rules_name(item) == Some(name))
                .collect();
            shared.get(name).is_some_and(|known| {
                known.len() == variants.len() && known.iter().zip(&variants).all(|(a, b)| a == *b)
            })
        };
        let moved: BTreeSet<String> = shared
            .keys()
            .filter(|name| defined(name))
            .cloned()
            .collect();
        program
            .items
            .retain(|item| macro_rules_name(item).is_none_or(|name| !moved.contains(name)));
    }
    Some(Program {
        items,
        ..Default::default()
    })
}

fn gate(cfg: Option<Cfg>, item: Item) -> Item {
    match cfg {
        Some(cfg) => Item::Cfg {
            cfg,
            item: Box::new(item),
        },
        None => item,
    }
}

fn projection_configs(matrix: &Matrix, projection: Projection) -> Result<Vec<Vec<String>>, String> {
    let names: Vec<String> = matrix
        .features
        .iter()
        .map(|feature| feature.name.clone())
        .collect();
    let valid = |mask: u64| -> Option<Vec<String>> {
        let set: Vec<String> = names
            .iter()
            .enumerate()
            .filter(|(index, _)| mask >> index & 1 == 1)
            .map(|(_, name)| name.clone())
            .collect();
        (!matrix.is_invalid_combination(&set)).then_some(set)
    };
    let total = 1u64.checked_shl(names.len() as u32);
    match projection {
        Projection::Skip => Ok(Vec::new()),
        Projection::All => {
            if names.len() > MAX_ENUMERATED_FEATURES {
                return Err(format!(
                    "cannot enumerate {} matrix features, the limit is {MAX_ENUMERATED_FEATURES}",
                    names.len()
                ));
            }
            Ok((0..1u64 << names.len()).filter_map(valid).collect())
        }
        Projection::Sample(count) => {
            let mut state = SAMPLE_SEED;
            let mut chosen = BTreeSet::new();
            let space = total.unwrap_or(u64::MAX);
            let mut attempts = 0;
            while chosen.len() < count && attempts < count.saturating_mul(16) {
                attempts += 1;
                state = state
                    .wrapping_mul(6_364_136_223_846_793_005)
                    .wrapping_add(1_442_695_040_888_963_407);
                let mask = (state >> 16) % space;
                chosen.insert(mask);
            }
            Ok(chosen.into_iter().filter_map(valid).collect())
        }
    }
}

struct ProjectionResult {
    checked: usize,
    skipped: usize,
    mismatches: Vec<String>,
}

fn check_config(
    matrix: &Matrix,
    project: &Path,
    target: &TargetBuild,
    merged: &[MergedUnit],
    families: &Families,
    features: &[String],
) -> Result<Vec<String>, ()> {
    let commands = matrix
        .commands(project, &target.name, features)
        .map_err(|_| ())?;
    let mut units = Vec::with_capacity(commands.len());
    for command in commands {
        units.push(parse(command).map_err(|_| ())?);
    }
    let imported = imported_commons(&units);
    let mut translated = translate_units(&units, imported, false, None, Sections::ByProject, true)
        .map_err(|_| ())?;
    for (_, program) in &mut translated.programs {
        rename_templates(&mut program.items, families);
    }
    let mut atoms = target.properties.clone();
    atoms.extend(features.iter().map(|name| feature_atom(name)));
    let mut mismatches = Vec::new();
    let context = format!("{} [{}]", target.name, features.join(","));
    for (stem, program) in &translated.programs {
        let unit = merged
            .iter()
            .find(|unit| unit.stem == *stem)
            .expect("direct translation uses the merged units");
        let mut resolved: BTreeMap<&str, &Item> = BTreeMap::new();
        let mut bindings: BTreeMap<&str, &Item> = BTreeMap::new();
        for entry in &unit.entries {
            if matches!(entry.item, Item::Comment(_)) {
                continue;
            }
            if !entry.cfg.as_ref().is_none_or(|cfg| holds(cfg, &atoms)) {
                continue;
            }
            let previous = if entry.binding {
                let (Item::Const { name, .. } | Item::TypeAlias { name, .. }) = &entry.item else {
                    unreachable!("hoists bind only consts and type aliases");
                };
                bindings.insert(name.as_str(), &entry.item)
            } else {
                resolved.insert(entry.key.as_str(), &entry.item)
            };
            if previous.is_some() {
                mismatches.push(format!("{context}: {stem}: {} is ambiguous", entry.key));
            }
        }
        let direct = entries(program);
        for entry in &direct {
            if matches!(entry.item, Item::Comment(_)) {
                continue;
            }
            match resolved.remove(entry.key.as_str()) {
                Some(item) if same_except_added_section(item, &entry.item, &bindings, &atoms) => {}
                Some(_) => mismatches.push(format!("{context}: {stem}: {} differs", entry.key)),
                None => mismatches.push(format!("{context}: {stem}: {} is missing", entry.key)),
            }
        }
        mismatches.extend(
            resolved
                .keys()
                .map(|key| format!("{context}: {stem}: {key} is extra")),
        );
    }
    Ok(mismatches)
}

fn same_except_added_section(
    merged: &Item,
    direct: &Item,
    bindings: &BTreeMap<&str, &Item>,
    atoms: &BTreeSet<Cfg>,
) -> bool {
    if merged == direct {
        return true;
    }
    let holds_here = |cfg: &Cfg| holds(cfg, atoms);
    if let (Item::Record(merged), Item::Record(direct)) = (merged, direct) {
        return super::splice::resolve_record(merged, &holds_here) == *direct;
    }
    let (Item::Fn(merged), Item::Fn(direct)) = (merged, direct) else {
        return false;
    };
    let mut expanded = merged.clone();
    super::hoist::expand(&mut expanded, bindings);
    let mut resolved = super::splice::resolve_function(&expanded, &holds_here);
    resolved
        .attrs
        .retain(|attr| !matches!(attr, Attr::LinkSection(_)) || direct.attrs.contains(attr));
    resolved == super::splice::resolve_function(direct, &holds_here)
}

fn projects_target(matrix: &Matrix, name: &str, projection_triples: &[String]) -> bool {
    projection_triples.is_empty()
        || matrix
            .targets
            .iter()
            .any(|target| target.name == name && projection_triples.contains(&target.triple))
}

fn check_projection(
    matrix: &Matrix,
    project: &Path,
    targets: &[TargetBuild],
    merged: &[MergedUnit],
    families: &Families,
    projection: Projection,
    projection_triples: &[String],
) -> Result<ProjectionResult, String> {
    let configs = projection_configs(matrix, projection)?;
    let jobs: Vec<(&TargetBuild, &Vec<String>)> = targets
        .iter()
        .filter(|target| projects_target(matrix, &target.name, projection_triples))
        .flat_map(|target| configs.iter().map(move |config| (target, config)))
        .collect();
    let results: Vec<_> = jobs
        .par_iter()
        .map(|(target, config)| check_config(matrix, project, target, merged, families, config))
        .collect();
    let mut outcome = ProjectionResult {
        checked: 0,
        skipped: 0,
        mismatches: Vec::new(),
    };
    for result in results {
        match result {
            Ok(mismatches) => {
                outcome.checked += 1;
                outcome.mismatches.extend(mismatches);
            }
            Err(()) => outcome.skipped += 1,
        }
    }
    Ok(outcome)
}

pub fn merge(
    matrix: &Matrix,
    project: &Path,
    targets: &[String],
    projection: Projection,
    projection_triples: &[String],
) -> Result<Merged, String> {
    let names: Vec<String> = if targets.is_empty() {
        matrix
            .targets
            .iter()
            .map(|target| target.name.clone())
            .collect()
    } else {
        targets.to_vec()
    };
    if names.len() > super::cfg::MAX_TARGETS {
        return Err(format!(
            "cannot merge {} targets, the limit is {}",
            names.len(),
            super::cfg::MAX_TARGETS
        ));
    }
    if projection == Projection::All {
        let projected = names
            .iter()
            .filter(|name| projects_target(matrix, name, projection_triples))
            .count();
        let checks = projection_configs(matrix, projection)?.len() * projected;
        if checks > MAX_EXHAUSTIVE_PROJECTIONS {
            return Err(format!(
                "--check-projection all needs {checks} whole-project translations, \
                 the limit is {MAX_EXHAUSTIVE_PROJECTIONS}; pass a count to sample instead"
            ));
        }
    }
    let mut stems = BTreeSet::new();
    for source in &matrix.sources {
        let stem = stem_of(source)?;
        if !stems.insert(stem.clone()) {
            return Err(format!("matrix sources map twice to module {stem}"));
        }
    }
    slate_parser::pp::reuse_lexed_sources();
    let prepared: Vec<Result<TargetBuild, String>> = names
        .par_iter()
        .map(|name| prepare_target(matrix, project, name))
        .collect();
    let mut builds = prepared.into_iter().collect::<Result<Vec<_>, _>>()?;
    let mut jobs: Vec<Job> = Vec::new();
    for (target, build) in builds.iter().enumerate() {
        for (index, unit) in build.units.iter().enumerate() {
            jobs.extend(
                valid_rows(matrix, &unit.relevant)
                    .filter(|mask| !unit.tried.contains(mask))
                    .map(|mask| (target, index, mask)),
            );
        }
    }
    compile_rows(matrix, project, &mut builds, jobs);
    let mut cfgs = BTreeMap::new();
    for build in &builds {
        if let Some(other) = cfgs.insert(build.cfg.clone(), build.name.clone()) {
            return Err(format!(
                "matrix targets {other} and {} have the same Rust cfg {}",
                build.name,
                build.cfg.render()
            ));
        }
    }
    let stems: Vec<String> = matrix
        .sources
        .iter()
        .map(|source| stem_of(source))
        .collect::<Result<_, _>>()?;
    let conditions = TargetConditions::new(
        builds
            .iter()
            .map(|build| build.properties.clone())
            .collect(),
    );
    let mut shared = 0;
    let mut signature_mismatches = Vec::new();
    for build in &builds {
        let units: Vec<UnitCompiles> = build
            .units
            .iter()
            .zip(&stems)
            .map(|(unit, stem)| UnitCompiles {
                stem,
                relevant: &unit.relevant,
                compiled: unit
                    .compiled
                    .iter()
                    .map(|compiled| {
                        let items = compiled.entries.iter().map(|entry| &entry.item).collect();
                        (compiled.mask, items)
                    })
                    .collect(),
            })
            .collect();
        let outcome =
            signatures::check(&units, &|features| matrix.is_invalid_combination(features));
        shared += outcome.shared;
        signature_mismatches.extend(outcome.mismatches.into_iter().map(|mismatch| {
            format!(
                "{} [{}]: {}: {} `{}` vs {} `{}`",
                build.name,
                mismatch.features.join(","),
                mismatch.symbol,
                mismatch.left.0,
                mismatch.left.1,
                mismatch.right.0,
                mismatch.right.1
            )
        }));
    }
    let families = template_families(&builds);
    for build in &mut builds {
        rename_families(build, &families);
        place_sections(build)?;
    }
    let merged: Vec<MergedUnit> = stems
        .par_iter()
        .enumerate()
        .map(|(index, stem)| merge_unit(stem, index, &builds, &conditions))
        .collect();
    for build in &builds {
        for unit in &build.units {
            if unit.compiled.is_empty() {
                let reason = unit
                    .infeasible
                    .first()
                    .map_or("no configurations", |(_, error)| error.as_str());
                return Err(format!(
                    "target {}: no configuration of {} translated: {reason}",
                    build.name,
                    unit.source.display()
                ));
            }
        }
    }
    let compiles: Vec<usize> = builds
        .iter()
        .map(|build| {
            build
                .units
                .iter()
                .map(|unit| unit.tried.len() - unit.reused.len())
                .sum()
        })
        .collect();
    let result = check_projection(
        matrix,
        project,
        &builds,
        &merged,
        &families,
        projection,
        projection_triples,
    )?;

    let roots: Vec<&str> = merged
        .iter()
        .enumerate()
        .filter(|(index, _)| builds.iter().any(|build| build.units[*index].defines_main))
        .map(|(_, unit)| unit.stem.as_str())
        .collect();
    let root = match roots.as_slice() {
        [root] => Some(root.to_string()),
        [] => None,
        _ => return Err(format!("multiple units define main: {}", roots.join(", "))),
    };

    let mut report = String::new();
    for (build, count) in builds.iter().zip(&compiles) {
        let _ = writeln!(report, "target {}: {count} compiles", build.name);
        for (unit, source) in build.units.iter().zip(&matrix.sources) {
            let _ = writeln!(
                report,
                "  {}: [{}] {} compiled",
                source.display(),
                unit.relevant.join(","),
                unit.compiled.len()
            );
            if !unit.reused.is_empty() {
                let _ = writeln!(
                    report,
                    "    {} assignments preprocess like a compiled one",
                    unit.reused.len()
                );
            }
            for (mask, message) in &unit.infeasible {
                let features = subset(&unit.relevant, *mask);
                let _ = writeln!(report, "    infeasible [{}]: {message}", features.join(","));
            }
        }
    }
    let _ = writeln!(
        report,
        "total: {} compiles across {} targets",
        compiles.iter().sum::<usize>(),
        builds.len()
    );
    for unit in &merged {
        let _ = writeln!(
            report,
            "unit {}: {} items, {} gated, {} duplicated functions",
            unit.stem,
            unit.entries.len(),
            unit.gated,
            unit.duplicated.len()
        );
        for (key, variants) in &unit.duplicated {
            let _ = writeln!(report, "  duplicated {key}: {variants} variants");
        }
        for (key, variants) in &unit.hoisted {
            let _ = writeln!(report, "  hoisted {key}: {variants} variants");
        }
        for (key, variants) in &unit.spliced {
            let _ = writeln!(report, "  spliced {key}: {variants} variants");
        }
    }
    report.push_str(&macro_report(&builds));
    if projection != Projection::Skip {
        let _ = writeln!(
            report,
            "projection: {} checked, {} not translatable, {} mismatches",
            result.checked,
            result.skipped,
            result.mismatches.len()
        );
        for mismatch in result.mismatches.iter().take(MAX_REPORTED_MISMATCHES) {
            let _ = writeln!(report, "  {mismatch}");
        }
    }
    let _ = writeln!(
        report,
        "signatures: {shared} shared symbols, {} mismatches",
        signature_mismatches.len()
    );
    for mismatch in signature_mismatches.iter().take(MAX_REPORTED_MISMATCHES) {
        let _ = writeln!(report, "  {mismatch}");
    }
    let features = matrix
        .features
        .iter()
        .map(|feature| feature.name.clone())
        .collect();
    let target_guard = gate(
        Some(Cfg::Not(Box::new(factor_cfgs(
            builds.iter().map(|build| build.cfg.clone()).collect(),
        )))),
        Item::Macro {
            name: "compile_error".into(),
            args: vec![Expr::Str(UNTRANSLATED_TARGET.into())],
        },
    );
    let mut units: Vec<(String, Program)> = merged
        .iter()
        .map(|unit| (unit.stem.clone(), program_of(unit)))
        .collect();
    crate::backend::engine::macro_templates::assign_template_names(
        units.iter_mut().map(|(_, program)| program),
    );
    let shared_macros = share_macros(&mut units);
    Ok(Merged {
        units,
        shared_macros,
        root,
        features,
        target_guard,
        report,
        mismatches: result.mismatches.len(),
        signature_mismatches: signature_mismatches.len(),
    })
}
