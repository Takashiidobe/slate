use crate::backend::link::signatures::{self, UnitCompiles};
use crate::backend::rust_ast::{Attr, Cfg, CrateAttr, ExternDecl, Item, Program};
use crate::backend::{self, Exports};
use crate::compile_commands::CompileCommand;
use crate::frontend::directive_translate::{factor_cfgs, item_key, target_cfg};
use crate::frontend::distinct_functions::{self, SectionCandidates};
use crate::frontend::{self, lowerer};
use crate::project_hoist;
use crate::project_matrix::Matrix;
use crate::project_plan;
use crate::project_splice::{self, Variants};
use rayon::prelude::*;
use slate_parser::files::Files;
use slate_parser::ir::{Linkage, Module};
use slate_parser::target_info::TargetInfo;
use std::cell::RefCell;
use std::collections::{BTreeMap, BTreeSet, HashMap};
use std::fmt::Write;
use std::path::{Path, PathBuf};

const MAX_RELEVANT_FEATURES: usize = 12;
const MAX_ENUMERATED_FEATURES: usize = 20;
const MAX_REPORTED_MISMATCHES: usize = 20;
const SAMPLE_SEED: u64 = 0x5eed_0f5a_a7e0_0001;

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
) -> Result<Translated, String> {
    let jobs: Vec<_> = units
        .iter()
        .map(|unit| {
            let options = lowerer::LowerOptions {
                export_symbols: true,
                explicit_docs_only,
                imported_commons: imported.remove(&unit.stem).unwrap_or_default(),
                unit: unit.stem.clone(),
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
    pub root: Option<String>,
    pub features: BTreeSet<String>,
    report: String,
    pub mismatches: usize,
    pub signature_mismatches: usize,
}

impl Merged {
    pub fn render(&self) -> &str {
        &self.report
    }
}

#[derive(Clone)]
struct Entry {
    key: String,
    item: Item,
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

struct Compiled {
    mask: u32,
    program: Program,
    candidates: SectionCandidates,
    entries: Vec<Entry>,
    attrs: Vec<CrateAttr>,
}

struct TargetUnit {
    source: PathBuf,
    relevant: Vec<String>,
    compiled: Vec<Compiled>,
    infeasible: Vec<(u32, String)>,
    tried: BTreeSet<u32>,
    defines_main: bool,
}

struct TargetBuild {
    name: String,
    cfg: Cfg,
    owners: CommonOwners,
    taken: BTreeSet<String>,
    units: Vec<TargetUnit>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Coverage {
    Refined,
    FullTable,
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
    frontend::parse_module_with_compiler_args(&path, command.args)
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

fn compile_isolated(
    command: CompileCommand,
    owners: &CommonOwners,
    mask: u32,
) -> Result<(Compiled, bool), String> {
    let unit = parse(command)?;
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
    Ok((
        Compiled {
            mask,
            attrs: crate_attrs(&program).cloned().collect(),
            program,
            candidates,
            entries: Vec::new(),
        },
        has_main,
    ))
}

fn matrix_cfg(matrix: &Matrix, target: &str) -> Result<Cfg, String> {
    let known = matrix
        .targets
        .iter()
        .find(|known| known.name == target)
        .ok_or_else(|| format!("unknown matrix target {target}"))?;
    let info = TargetInfo::for_triple(&known.triple).map_err(|error| error.to_string())?;
    Ok(target_cfg(&info))
}

fn prepare_target(matrix: &Matrix, project: &Path, target: &str) -> Result<TargetBuild, String> {
    let plan = project_plan::plan(matrix, project, Some(target), project_plan::Mode::Plan)
        .map_err(|error| error.to_string())?;
    let units = &plan.targets[0].units;
    if let Some((unit, error)) = units
        .iter()
        .find_map(|unit| unit.error.as_ref().map(|error| (unit, error)))
    {
        let first = error.lines().next().unwrap_or_default();
        return Err(format!(
            "target {target}: {} could not be planned: {first}",
            unit.source.display()
        ));
    }
    let defaults = matrix
        .commands(project, target, &[])
        .map_err(|error| error.to_string())?;
    let parsed: Vec<Unit> = defaults
        .into_par_iter()
        .filter_map(|command| parse(command).ok())
        .collect();
    let owners = common_owners(parsed.iter().map(|unit| (unit.stem.as_str(), &unit.module)));

    let mut built = Vec::with_capacity(units.len());
    for unit in units {
        if unit.relevant.len() > MAX_RELEVANT_FEATURES {
            return Err(format!(
                "target {target}: {} depends on {} features, the limit is {MAX_RELEVANT_FEATURES}",
                unit.source.display(),
                unit.relevant.len()
            ));
        }
        built.push(TargetUnit {
            source: unit.source.clone(),
            relevant: unit.relevant.clone(),
            compiled: Vec::new(),
            infeasible: Vec::new(),
            tried: BTreeSet::new(),
            defines_main: false,
        });
    }
    Ok(TargetBuild {
        name: target.to_string(),
        cfg: matrix_cfg(matrix, target)?,
        owners,
        taken: BTreeSet::new(),
        units: built,
    })
}

fn is_valid(matrix: &Matrix, relevant: &[String], mask: u32) -> bool {
    !matrix.is_invalid_combination(&subset(relevant, mask))
}

fn seed_rows(matrix: &Matrix, relevant: &[String], coverage: Coverage) -> BTreeSet<u32> {
    let bits = relevant.len();
    let mut rows: BTreeSet<u32> = match coverage {
        Coverage::FullTable => (0u32..1 << bits).collect(),
        Coverage::Refined => {
            let mut all_on = (1u32 << bits) - 1;
            while all_on != 0 && !is_valid(matrix, relevant, all_on) {
                all_on &= !(1 << (u32::BITS - 1 - all_on.leading_zeros()));
            }
            std::iter::once(0)
                .chain((0..bits).map(|bit| 1u32 << bit))
                .chain([all_on])
                .collect()
        }
    };
    rows.retain(|mask| is_valid(matrix, relevant, *mask));
    rows
}

type Job = (usize, usize, u32);

fn compile_rows(matrix: &Matrix, project: &Path, builds: &mut [TargetBuild], jobs: Vec<Job>) {
    let results: Vec<_> = jobs
        .par_iter()
        .map(|&(target, index, mask)| {
            let build = &builds[target];
            let unit = &build.units[index];
            let command = matrix
                .command(
                    project,
                    &build.name,
                    &subset(&unit.relevant, mask),
                    &unit.source,
                )
                .map_err(|error| error.to_string())?;
            compile_isolated(command, &build.owners, mask)
        })
        .collect();
    for ((target, index, mask), result) in jobs.into_iter().zip(results) {
        let unit = &mut builds[target].units[index];
        unit.tried.insert(mask);
        match result {
            Ok((compiled, has_main)) => {
                unit.defines_main |= has_main;
                let at = unit.compiled.partition_point(|known| known.mask < mask);
                unit.compiled.insert(at, compiled);
            }
            Err(error) => {
                let first = error.lines().next().unwrap_or_default().to_string();
                let at = unit.infeasible.partition_point(|known| known.0 < mask);
                unit.infeasible.insert(at, (mask, first));
            }
        }
    }
}

fn place_sections(build: &mut TargetBuild) -> Result<bool, String> {
    let taken: BTreeSet<String> = build
        .units
        .iter()
        .flat_map(|unit| &unit.compiled)
        .flat_map(|compiled| compiled.candidates.taken_symbols.iter().cloned())
        .collect();
    let changed = taken != build.taken;
    for unit in &mut build.units {
        let stem = stem_of(&unit.source)?;
        for compiled in &mut unit.compiled {
            if !changed && !compiled.entries.is_empty() {
                continue;
            }
            let mut protected = compiled.candidates.taken_here.clone();
            protected.extend(
                compiled
                    .candidates
                    .exported
                    .iter()
                    .filter(|(symbol, _)| taken.contains(*symbol))
                    .map(|(_, name)| name.clone()),
            );
            let mut placed = compiled.program.clone();
            backend::place_in_distinct_sections(&mut placed, &stem, &protected);
            compiled.entries = entries(&placed);
        }
    }
    build.taken = taken;
    Ok(changed)
}

fn untried_rows(matrix: &Matrix, unit: &TargetUnit) -> Vec<u32> {
    (0u32..1 << unit.relevant.len())
        .filter(|mask| !unit.tried.contains(mask) && is_valid(matrix, &unit.relevant, *mask))
        .collect()
}

fn hitting_rows(mut open: Vec<Vec<u32>>) -> Vec<u32> {
    let mut chosen = Vec::new();
    while !open.is_empty() {
        let mut counts: BTreeMap<u32, usize> = BTreeMap::new();
        for rows in &open {
            for row in rows {
                *counts.entry(*row).or_default() += 1;
            }
        }
        let best = counts
            .iter()
            .max_by_key(|(row, count)| (**count, std::cmp::Reverse(**row)))
            .map(|(row, _)| *row)
            .expect("open row sets are not empty");
        open.retain(|rows| !rows.contains(&best));
        chosen.push(best);
    }
    chosen
}

fn undecided_rows(
    merged: &MergedUnit,
    target: usize,
    unit: &TargetUnit,
    untried: &[u32],
) -> Vec<u32> {
    let care: BTreeSet<u32> = unit.compiled.iter().map(|compiled| compiled.mask).collect();
    let bits = unit.relevant.len();
    let mut open = Vec::new();
    for (on, within) in &merged.gates {
        open.extend(ambiguous_rows(&on[target], &within[target], bits, untried));
    }
    for variants in &merged.claims {
        let covers: Vec<Vec<Implicant>> = variants
            .iter()
            .map(|on| minimize(&on[target], &care, bits))
            .collect();
        let union: BTreeSet<u32> = variants
            .iter()
            .flat_map(|on| on[target].iter().copied())
            .collect();
        let present = minimize(&union, &care, bits);
        for &row in untried {
            let claims = covers
                .iter()
                .filter(|cover| cover.iter().any(|implicant| implicant.covers(row)))
                .count();
            let expected = present.iter().any(|implicant| implicant.covers(row));
            if claims > 1 || (expected && claims == 0) {
                open.push(vec![row]);
            }
        }
    }
    hitting_rows(open)
}

fn refine(
    matrix: &Matrix,
    project: &Path,
    builds: &mut [TargetBuild],
    stems: &[String],
) -> Result<Vec<MergedUnit>, String> {
    let mut merged: Vec<Option<MergedUnit>> = stems.iter().map(|_| None).collect();
    let mut dirty: BTreeSet<usize> = (0..stems.len()).collect();
    loop {
        for build in builds.iter_mut() {
            if place_sections(build)? {
                dirty = (0..stems.len()).collect();
            }
        }
        let shared: &[TargetBuild] = builds;
        let rebuilt: Vec<(usize, MergedUnit)> = dirty
            .par_iter()
            .map(|&index| (index, merge_unit(&stems[index], index, shared)))
            .collect();
        for (index, unit) in rebuilt {
            merged[index] = Some(unit);
        }
        let units: Vec<(usize, usize)> = dirty
            .iter()
            .flat_map(|&index| (0..shared.len()).map(move |target| (target, index)))
            .collect();
        let jobs: Vec<Job> = units
            .par_iter()
            .flat_map_iter(|&(target, index)| {
                let unit = &shared[target].units[index];
                let untried = untried_rows(matrix, unit);
                let result = merged[index].as_ref().expect("dirty units are merged");
                let mut rows = if unit.compiled.is_empty() {
                    untried
                } else {
                    undecided_rows(result, target, unit, &untried)
                };
                rows.sort_unstable();
                rows.into_iter().map(move |mask| (target, index, mask))
            })
            .collect();
        if jobs.is_empty() {
            return Ok(merged
                .into_iter()
                .map(|unit| unit.expect("every unit is merged"))
                .collect());
        }
        dirty = jobs.iter().map(|(_, index, _)| *index).collect();
        compile_rows(matrix, project, builds, jobs);
    }
}

#[derive(Clone, Copy, PartialEq, Eq)]
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

fn ambiguous_rows(
    on: &BTreeSet<u32>,
    within: &BTreeSet<u32>,
    bits: usize,
    untried: &[u32],
) -> Vec<Vec<u32>> {
    if on.is_empty() || within.is_empty() {
        return Vec::new();
    }
    let primes = candidates(on, within, bits);
    let chosen = select_cover(on, &primes);
    let decided = |row: u32| chosen.iter().any(|implicant| implicant.covers(row));
    let literals = |implicant: &Implicant| bits as u32 - implicant.dashes.count_ones();
    let simplest = chosen.iter().map(literals).max().unwrap_or(0);
    let all_on = (1u32 << bits) - 1;
    let mut open: Vec<Vec<u32>> = primes
        .iter()
        .filter(|prime| {
            !chosen.contains(prime)
                && literals(prime) <= simplest
                && on.iter().any(|mask| prime.covers(*mask))
        })
        .map(|prime| {
            untried
                .iter()
                .copied()
                .filter(|row| prime.covers(*row) && !decided(*row))
                .collect::<Vec<u32>>()
        })
        .filter(|rows| !rows.is_empty())
        .collect();
    for implicant in &chosen {
        for witness in [implicant.value, implicant.value | implicant.dashes] {
            if untried.contains(&witness) {
                open.push(vec![witness]);
            }
        }
        let positive = implicant.value & !implicant.dashes;
        for bit in 0..bits {
            let probe = all_on & !(1 << bit);
            if positive >> bit & 1 == 1 && untried.contains(&probe) {
                open.push(vec![probe]);
            }
        }
    }
    open
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

fn combined_cfg(groups: BTreeMap<Option<Cfg>, Vec<Cfg>>, targets: usize) -> Option<Cfg> {
    if let [(condition, members)] = groups.iter().collect::<Vec<_>>().as_slice()
        && members.len() == targets
    {
        return (*condition).clone();
    }
    let mut terms: Vec<Cfg> = groups
        .into_iter()
        .map(|(condition, members)| {
            let targets = factor_cfgs(members);
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
    item: Item,
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
    gates: BTreeSet<Gate>,
    claims: Vec<Vec<OnSets>>,
}

type OnSets = Vec<BTreeSet<u32>>;
type Gate = (OnSets, OnSets);

fn hoist_values(key: &str, values: &[Value]) -> Option<project_hoist::Hoisted> {
    if key.contains('#') {
        return None;
    }
    let variants = values
        .iter()
        .map(|value| match &value.item {
            Item::Fn(function) => Some(function),
            _ => None,
        })
        .collect::<Option<Vec<_>>>()?;
    project_hoist::hoist(&variants)
}

fn splice_values(
    key: &str,
    values: &[Value],
    gate: &mut impl FnMut(&Variants, &Variants) -> Option<Cfg>,
) -> Option<(Item, Vec<(Variants, Item)>)> {
    if key.contains('#') {
        return None;
    }
    match &values[0].item {
        Item::Fn(_) => {
            let variants = values
                .iter()
                .map(|value| match &value.item {
                    Item::Fn(function) => Some(function),
                    _ => None,
                })
                .collect::<Option<Vec<_>>>()?;
            project_splice::splice_function(&variants, gate)
                .map(|spliced| (Item::Fn(spliced.function), spliced.bindings))
        }
        Item::Record(_) => {
            let variants = values
                .iter()
                .map(|value| match &value.item {
                    Item::Record(record) => Some(record),
                    _ => None,
                })
                .collect::<Option<Vec<_>>>()?;
            project_splice::splice_record(&variants, gate)
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

fn merge_unit(stem: &str, index: usize, targets: &[TargetBuild]) -> MergedUnit {
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
                    .position(|value| value.item == entry.item)
                    .unwrap_or_else(|| {
                        values.push(Value {
                            item: entry.item.clone(),
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
        gates: BTreeSet::new(),
        claims: Vec::new(),
    };
    let gates: RefCell<BTreeSet<Gate>> = RefCell::new(BTreeSet::new());
    let cfg_within = |on: &[BTreeSet<u32>], within: &[BTreeSet<u32>]| -> Option<Cfg> {
        let mut groups: BTreeMap<Option<Cfg>, Vec<Cfg>> = BTreeMap::new();
        let mut present = 0;
        for (target_index, target) in targets.iter().enumerate() {
            if within[target_index].is_empty() {
                continue;
            }
            present += 1;
            let on = &on[target_index];
            if on.is_empty() {
                continue;
            }
            let relevant = &target.units[index].relevant;
            let implicants = minimize(on, &within[target_index], relevant.len());
            groups
                .entry(condition(&implicants, relevant))
                .or_default()
                .push(target.cfg.clone());
        }
        combined_cfg(groups, present)
    };
    let recorded_within = |on: &[BTreeSet<u32>], within: &[BTreeSet<u32>]| -> Option<Cfg> {
        gates.borrow_mut().insert((on.to_vec(), within.to_vec()));
        cfg_within(on, within)
    };
    let cfg_of = |on: &[BTreeSet<u32>]| recorded_within(on, &cares);
    for key in order {
        let values = table.remove(&key).expect("ordered key is in the table");
        if values.len() > 1
            && key.starts_with("fn:")
            && let Some(hoisted) = hoist_values(&key, &values)
        {
            out.hoisted.push((key.clone(), values.len()));
            out.claims
                .push(values.iter().map(|value| value.on.clone()).collect());
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
                        recorded_within(
                            &union_on(&values, on, targets.len()),
                            &union_on(&values, within, targets.len()),
                        )
                    })
                    .clone()
            };
            if let Some((item, bindings)) = splice_values(&key, &values, &mut gate) {
                out.spliced.push((key.clone(), values.len()));
                if !bindings.is_empty() {
                    out.claims.push(
                        bindings
                            .iter()
                            .map(|(on, _)| union_on(&values, on, targets.len()))
                            .collect(),
                    );
                }
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
        if values.len() > 1 && !matches!(values[0].item, Item::Comment(_)) {
            out.claims
                .push(values.iter().map(|value| value.on.clone()).collect());
        }
        for value in values {
            let cfg = if matches!(value.item, Item::Comment(_)) {
                let cfg = cfg_within(&value.on, &cares);
                if cfg.is_some() {
                    continue;
                }
                cfg
            } else {
                cfg_of(&value.on)
            };
            out.gated += usize::from(cfg.is_some());
            out.entries.push(MergedEntry {
                cfg,
                key: key.clone(),
                item: value.item,
                binding: false,
            });
        }
    }
    out.gates = gates.into_inner();
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
    Program {
        items: items.into_iter().flatten().collect(),
    }
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

fn holds(cfg: &Cfg, atoms: &BTreeSet<Cfg>) -> bool {
    match cfg {
        Cfg::Flag(_) | Cfg::Opt { .. } => atoms.contains(cfg),
        Cfg::Not(inner) => !holds(inner, atoms),
        Cfg::Any(items) => items.iter().any(|item| holds(item, atoms)),
        Cfg::All(items) => items.iter().all(|item| holds(item, atoms)),
    }
}

fn target_atoms(cfg: &Cfg, out: &mut BTreeSet<Cfg>) {
    match cfg {
        Cfg::All(items) => items.iter().for_each(|item| target_atoms(item, out)),
        atom => {
            out.insert(atom.clone());
        }
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
    let translated =
        translate_units(&units, imported, false, None, Sections::ByProject).map_err(|_| ())?;
    let mut atoms = BTreeSet::new();
    target_atoms(&target.cfg, &mut atoms);
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
        return project_splice::resolve_record(merged, &holds_here) == *direct;
    }
    let (Item::Fn(merged), Item::Fn(direct)) = (merged, direct) else {
        return false;
    };
    let mut expanded = merged.clone();
    project_hoist::expand(&mut expanded, bindings);
    let mut resolved = project_splice::resolve_function(&expanded, &holds_here);
    resolved
        .attrs
        .retain(|attr| !matches!(attr, Attr::LinkSection(_)) || direct.attrs.contains(attr));
    resolved == project_splice::resolve_function(direct, &holds_here)
}

fn check_projection(
    matrix: &Matrix,
    project: &Path,
    targets: &[TargetBuild],
    merged: &[MergedUnit],
    projection: Projection,
) -> Result<ProjectionResult, String> {
    let configs = projection_configs(matrix, projection)?;
    let jobs: Vec<(&TargetBuild, &Vec<String>)> = targets
        .iter()
        .flat_map(|target| configs.iter().map(move |config| (target, config)))
        .collect();
    let results: Vec<_> = jobs
        .par_iter()
        .map(|(target, config)| check_config(matrix, project, target, merged, config))
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
    coverage: Coverage,
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
    let mut stems = BTreeSet::new();
    for source in &matrix.sources {
        let stem = stem_of(source)?;
        if !stems.insert(stem.clone()) {
            return Err(format!("matrix sources map twice to module {stem}"));
        }
    }
    let mut builds = Vec::with_capacity(names.len());
    for name in &names {
        builds.push(prepare_target(matrix, project, name)?);
    }
    let mut jobs: Vec<Job> = Vec::new();
    for (target, build) in builds.iter().enumerate() {
        for (index, unit) in build.units.iter().enumerate() {
            jobs.extend(
                seed_rows(matrix, &unit.relevant, coverage)
                    .into_iter()
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
    let merged = refine(matrix, project, &mut builds, &stems)?;
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
        .map(|build| build.units.iter().map(|unit| unit.tried.len()).sum())
        .collect();
    let result = check_projection(matrix, project, &builds, &merged, projection)?;

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
                    .map(|compiled| (compiled.mask, &compiled.program))
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
    Ok(Merged {
        units: merged
            .iter()
            .map(|unit| (unit.stem.clone(), program_of(unit)))
            .collect(),
        root,
        features,
        report,
        mismatches: result.mismatches.len(),
        signature_mismatches: signature_mismatches.len(),
    })
}
