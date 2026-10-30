use crate::frontend;
use crate::project_matrix::{Matrix, MatrixError};
use rayon::prelude::*;
use slate_parser::ast::{FileId, Span};
use slate_parser::files::Files;
use slate_parser::lexer::{Token, token_spelling};
use slate_parser::pp::{ArmState, PPNode, PPNodeKind, PreprocessTrace, TokenTrace};
use std::collections::{BTreeMap, BTreeSet, HashMap, VecDeque};
use std::fmt::Write;
use std::path::{Path, PathBuf};

const MAX_RUNS: usize = 4096;
const MAX_EXHAUSTIVE_FEATURES: usize = 16;
const NO_ARM: u32 = u32::MAX;

type Config = BTreeSet<usize>;
type BranchKey = (u32, u32);

#[derive(Debug)]
pub struct Plan {
    pub targets: Vec<TargetPlan>,
}

#[derive(Debug)]
pub struct TargetPlan {
    pub name: String,
    pub units: Vec<UnitPlan>,
}

#[derive(Debug, Default)]
pub struct UnitPlan {
    pub source: PathBuf,
    pub relevant: Vec<String>,
    pub configurations: Vec<Vec<String>>,
    pub elements: usize,
    pub runs: usize,
    pub excluded: usize,
    pub failed: usize,
    pub rejected: Vec<Rejected>,
    pub truncated: bool,
    pub error: Option<String>,
    pub exhaustive: Option<Exhaustive>,
}

#[derive(Debug)]
pub struct Rejected {
    pub configuration: Vec<String>,
    pub message: String,
}

#[derive(Debug)]
pub struct Exhaustive {
    pub compiles: usize,
    pub elements: usize,
    pub missed: usize,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Mode {
    Plan,
    CheckExhaustive,
}

pub fn plan(
    matrix: &Matrix,
    project: &Path,
    target: Option<&str>,
    mode: Mode,
) -> Result<Plan, MatrixError> {
    if let Some(target) = target
        && !matrix.targets.iter().any(|known| known.name == target)
    {
        return Err(MatrixError::UnknownTarget(target.to_string()));
    }
    if mode == Mode::CheckExhaustive && matrix.features.len() > MAX_EXHAUSTIVE_FEATURES {
        return Err(MatrixError::TooManyFeaturesToEnumerate {
            features: matrix.features.len(),
            limit: MAX_EXHAUSTIVE_FEATURES,
        });
    }
    let names: Vec<&str> = matrix
        .targets
        .iter()
        .map(|known| known.name.as_str())
        .filter(|name| target.is_none_or(|target| target == *name))
        .collect();
    let jobs: Vec<(&str, &PathBuf)> = names
        .iter()
        .flat_map(|name| matrix.sources.iter().map(move |source| (*name, source)))
        .collect();
    let mut units = jobs
        .into_par_iter()
        .map(|(name, source)| plan_unit(matrix, project, name, source, mode))
        .collect::<Vec<UnitPlan>>()
        .into_iter();
    Ok(Plan {
        targets: names
            .into_iter()
            .map(|name| TargetPlan {
                name: name.to_string(),
                units: units.by_ref().take(matrix.sources.len()).collect(),
            })
            .collect(),
    })
}

fn plan_unit(matrix: &Matrix, project: &Path, target: &str, source: &Path, mode: Mode) -> UnitPlan {
    let mut explorer = Explorer::new(matrix, project, target, source);
    explorer.explore();
    let mut unit = explorer.finish();
    if mode == Mode::CheckExhaustive {
        let mut exhaustive = Explorer::new(matrix, project, target, source);
        exhaustive.interned = explorer.interned.clone();
        exhaustive.enumerate();
        let compiles = exhaustive.select().len();
        let universe = exhaustive.universe();
        let planned = explorer.universe();
        unit.exhaustive = Some(Exhaustive {
            compiles,
            elements: universe.len(),
            missed: universe.difference(&planned).count(),
        });
    }
    unit
}

impl Plan {
    pub fn unplanned(&self) -> usize {
        self.targets
            .iter()
            .flat_map(|target| &target.units)
            .filter(|unit| unit.error.is_some())
            .count()
    }

    pub fn render(&self, features: usize) -> String {
        let mut out = String::new();
        let mut total = 0;
        for target in &self.targets {
            let compiles: usize = target
                .units
                .iter()
                .map(|unit| unit.configurations.len())
                .sum();
            let runs: usize = target.units.iter().map(|unit| unit.runs).sum();
            let unplanned = target
                .units
                .iter()
                .filter(|unit| unit.error.is_some())
                .count();
            total += compiles;
            let _ = write!(
                out,
                "target {}: {compiles} compiles from {runs} preprocessor runs, naive {}",
                target.name,
                naive(features, target.units.len()),
            );
            if unplanned > 0 {
                let _ = write!(out, ", {unplanned} units unplanned");
            }
            let _ = writeln!(out);
            for unit in &target.units {
                render_unit(&mut out, unit);
            }
        }
        let _ = writeln!(
            out,
            "total: {total} compiles across {} targets",
            self.targets.len()
        );
        out
    }
}

fn naive(features: usize, units: usize) -> String {
    u32::try_from(features)
        .ok()
        .and_then(|features| 1u128.checked_shl(features))
        .and_then(|subsets| subsets.checked_mul(units as u128))
        .map_or_else(|| "overflow".to_string(), |naive| naive.to_string())
}

fn render_unit(out: &mut String, unit: &UnitPlan) {
    let _ = write!(
        out,
        "  {}: {} compiles, {} relevant, {} elements, {} runs",
        unit.source.display(),
        unit.configurations.len(),
        unit.relevant.len(),
        unit.elements,
        unit.runs,
    );
    if unit.excluded > 0 {
        let _ = write!(out, ", {} hit #error", unit.excluded);
    }
    if unit.failed > 0 {
        let _ = write!(out, ", {} failed preprocessing", unit.failed);
    }
    if !unit.rejected.is_empty() {
        let _ = write!(out, ", {} failed sema", unit.rejected.len());
    }
    if unit.truncated {
        let _ = write!(out, ", run limit reached");
    }
    if let Some(exhaustive) = &unit.exhaustive {
        let _ = write!(
            out,
            ", exhaustive {} compiles {} elements {} missed",
            exhaustive.compiles, exhaustive.elements, exhaustive.missed
        );
    }
    let _ = writeln!(out);
    if let Some(error) = &unit.error {
        let first = error.lines().next().unwrap_or_default();
        let _ = writeln!(out, "    error: {first}");
    }
    for configuration in &unit.configurations {
        let _ = writeln!(out, "    [{}]", configuration.join(","));
    }
    for rejected in &unit.rejected {
        let first = rejected.message.lines().next().unwrap_or_default();
        let _ = writeln!(
            out,
            "    rejected [{}]: {first}",
            rejected.configuration.join(",")
        );
    }
}

#[derive(Clone, Copy, PartialEq, Eq)]
enum Outcome {
    Clean,
    HitError,
    Failed,
}

struct Run {
    elements: Vec<u32>,
    outcome: Outcome,
}

struct Pending {
    branch: Option<BranchKey>,
    config: Config,
}

struct Explorer<'a> {
    matrix: &'a Matrix,
    project: &'a Path,
    target: &'a str,
    source: &'a Path,
    feature_of_macro: HashMap<String, Vec<usize>>,
    interned: HashMap<String, u32>,
    runs: BTreeMap<Config, Run>,
    reached: BTreeSet<BranchKey>,
    attempts: BTreeMap<BranchKey, usize>,
    pending: VecDeque<Pending>,
    relevant: Config,
    pairs: BTreeSet<Config>,
    rejected: BTreeMap<Config, String>,
    accepted: BTreeSet<Config>,
    truncated: bool,
    error: Option<String>,
}

impl<'a> Explorer<'a> {
    fn new(matrix: &'a Matrix, project: &'a Path, target: &'a str, source: &'a Path) -> Self {
        let mut feature_of_macro: HashMap<String, Vec<usize>> = HashMap::new();
        for (index, feature) in matrix.features.iter().enumerate() {
            for define in &feature.defines {
                let name = define.split(['=', '(']).next().unwrap_or(define);
                feature_of_macro
                    .entry(name.to_string())
                    .or_default()
                    .push(index);
            }
        }
        Self {
            matrix,
            project,
            target,
            source,
            feature_of_macro,
            interned: HashMap::new(),
            runs: BTreeMap::new(),
            reached: BTreeSet::new(),
            attempts: BTreeMap::new(),
            pending: VecDeque::new(),
            relevant: Config::new(),
            pairs: BTreeSet::new(),
            rejected: BTreeMap::new(),
            accepted: BTreeSet::new(),
            truncated: false,
            error: None,
        }
    }

    fn explore(&mut self) {
        self.observe(&Config::new());
        loop {
            while let Some(pending) = self.pending.pop_front() {
                if pending
                    .branch
                    .is_none_or(|key| !self.reached.contains(&key))
                {
                    self.observe(&pending.config);
                }
            }
            let mut next: Vec<Config> = self
                .relevant
                .iter()
                .map(|feature| Config::from([*feature]))
                .collect();
            next.push(self.all_relevant());
            next.extend(self.pairs.iter().cloned());
            let mut seen = BTreeSet::new();
            for config in next {
                if self.is_new(&config) && seen.insert(config.clone()) {
                    self.pending.push_back(Pending {
                        branch: None,
                        config,
                    });
                }
            }
            if self.pending.is_empty() {
                return;
            }
        }
    }

    fn enumerate(&mut self) {
        let count = self.matrix.features.len();
        for bits in 0u32..1 << count {
            let config: Config = (0..count).filter(|index| bits >> index & 1 == 1).collect();
            if !self.is_invalid(&config) {
                self.observe_unlimited(&config);
            }
        }
    }

    fn all_relevant(&self) -> Config {
        let mut config = self.relevant.clone();
        while self.is_invalid(&config) {
            let Some(last) = config.last().copied() else {
                break;
            };
            config.remove(&last);
        }
        config
    }

    fn names(&self, config: &Config) -> Vec<String> {
        config
            .iter()
            .map(|index| self.matrix.features[*index].name.clone())
            .collect()
    }

    fn is_invalid(&self, config: &Config) -> bool {
        self.matrix.is_invalid_combination(&self.names(config))
    }

    fn is_new(&self, config: &Config) -> bool {
        !self.runs.contains_key(config) && !self.is_invalid(config)
    }

    fn observe(&mut self, config: &Config) {
        if self.runs.contains_key(config) {
            return;
        }
        if self.runs.len() >= MAX_RUNS {
            self.truncated = true;
            return;
        }
        self.observe_unlimited(config);
    }

    fn observe_unlimited(&mut self, config: &Config) {
        let command =
            self.matrix
                .command(self.project, self.target, &self.names(config), self.source);
        let result = command
            .map_err(|error| error.to_string())
            .and_then(|command| {
                frontend::preprocess_traced(&command.file, command.args)
                    .map_err(|error| error.to_string())
            });
        match result {
            Ok((nodes, files, trace, diagnostics)) => {
                let clean = diagnostics
                    .iter()
                    .all(|diagnostic| diagnostic.severity == miette::Severity::Warning);
                let outcome = if clean {
                    Outcome::Clean
                } else {
                    Outcome::HitError
                };
                let elements = self.analyze(config, &nodes, &files, &trace);
                self.runs.insert(config.clone(), Run { elements, outcome });
            }
            Err(error) => {
                if config.is_empty() {
                    self.error = Some(error);
                }
                self.runs.insert(
                    config.clone(),
                    Run {
                        elements: Vec::new(),
                        outcome: Outcome::Failed,
                    },
                );
            }
        }
    }

    fn intern(&mut self, key: String) -> u32 {
        let next = self.interned.len() as u32;
        *self.interned.entry(key).or_insert(next)
    }

    fn features_of(&self, roots: &BTreeSet<String>) -> Config {
        roots
            .iter()
            .filter_map(|root| self.feature_of_macro.get(root))
            .flatten()
            .copied()
            .collect()
    }

    fn analyze(
        &mut self,
        config: &Config,
        nodes: &[PPNode],
        files: &Files,
        trace: &PreprocessTrace,
    ) -> Vec<u32> {
        let mut roots: HashMap<TokenTrace, Config> = HashMap::new();
        let mut elements = BTreeSet::new();
        let mut chunk = String::new();
        let mut chunk_start = None;
        let mut parens = 0usize;
        let mut braces = 0usize;
        let mut item = Item::default();
        let mut items = Vec::new();
        let mut finish_chunk = |explorer: &mut Self, chunk: &mut String, start: &mut Option<_>| {
            if let Some(token) = start.take() {
                let key = element_key(files, token, chunk);
                elements.insert(explorer.intern(key));
            }
            chunk.clear();
        };
        for node in nodes {
            let (tokens, traces, pragma) = match &node.value {
                PPNodeKind::Comment { .. } => continue,
                PPNodeKind::Code { tokens, trace, .. } => (tokens, trace, false),
                PPNodeKind::Pragma { tokens, trace, .. } => (tokens, trace, true),
            };
            for (token, token_trace) in tokens.iter().zip(traces) {
                let features = roots
                    .entry(*token_trace)
                    .or_insert_with(|| self.features_of(&trace.dependencies(*token_trace).roots));
                item.features.extend(features.iter().copied());
                if let Token::Ident(name) = &token.value
                    && let Some(named) = self.feature_of_macro.get(name.as_str())
                {
                    item.features.extend(named.iter().copied());
                }
                item.start
                    .get_or_insert((token.expansion.file, token.expansion.offset));
                item.end = token.expansion.offset;
                chunk_start.get_or_insert(token);
                if !chunk.is_empty() {
                    chunk.push(' ');
                }
                chunk.push_str(&token_spelling(token));
                match token.value {
                    Token::LParen => parens += 1,
                    Token::RParen => parens = parens.saturating_sub(1),
                    Token::LBrace => braces += 1,
                    Token::RBrace => braces = braces.saturating_sub(1),
                    _ => {}
                }
                if !pragma
                    && parens == 0
                    && matches!(token.value, Token::Semi | Token::LBrace | Token::RBrace)
                {
                    finish_chunk(self, &mut chunk, &mut chunk_start);
                    if braces == 0 && matches!(token.value, Token::Semi | Token::RBrace) {
                        items.push(std::mem::take(&mut item));
                    }
                }
            }
            if pragma {
                finish_chunk(self, &mut chunk, &mut chunk_start);
            }
        }
        finish_chunk(self, &mut chunk, &mut chunk_start);
        items.push(item);

        let mut by_start: HashMap<FileId, BTreeMap<usize, usize>> = HashMap::new();
        for (index, item) in items.iter().enumerate() {
            if let Some((file, start)) = item.start {
                by_start.entry(file).or_default().insert(start, index);
            }
        }
        for expansion in &trace.expansions {
            let features = roots
                .entry(expansion.trace)
                .or_insert_with(|| self.features_of(&trace.dependencies(expansion.trace).roots));
            let containing = by_start
                .get(&expansion.site.file)
                .and_then(|starts| starts.range(..=expansion.site.offset).next_back())
                .map(|(_, index)| *index)
                .filter(|index| expansion.site.offset <= items[*index].end);
            match containing {
                Some(index) => items[index].features.extend(features.iter().copied()),
                None => self.relevant.extend(features.iter().copied()),
            }
        }
        for item in items {
            self.relevant.extend(item.features.iter().copied());
            let features: Vec<usize> = item.features.into_iter().collect();
            for (index, left) in features.iter().enumerate() {
                for right in &features[index + 1..] {
                    self.pairs.insert(Config::from([*left, *right]));
                }
            }
        }
        self.observe_branches(config, files, trace);
        elements.into_iter().collect()
    }

    fn observe_branches(&mut self, config: &Config, files: &Files, trace: &PreprocessTrace) {
        let mut unseen = Vec::new();
        for (index, conditional) in trace.conditionals.iter().enumerate() {
            let Some(first) = conditional.arms.first() else {
                continue;
            };
            let id = self.intern(format!(
                "{}:{}",
                files.path(first.loc.file).display(),
                first.loc.offset
            ));
            let has_else = conditional
                .arms
                .last()
                .is_some_and(|arm| arm.directive == "#else");
            if first.state != ArmState::NotEvaluated {
                let taken = conditional
                    .arms
                    .iter()
                    .position(|arm| arm.state == ArmState::Taken)
                    .map_or(NO_ARM, |arm| arm as u32);
                self.reached.insert((id, taken));
            }
            let features = self.features_of(&trace.conditional_dependencies(index).roots);
            if features.is_empty() {
                continue;
            }
            self.relevant.extend(features.iter().copied());
            let implicit = (!has_else).then_some(NO_ARM);
            for arm in (0..conditional.arms.len() as u32).chain(implicit) {
                unseen.push(((id, arm), features.clone()));
            }
        }
        for (key, features) in unseen {
            if self.reached.contains(&key) {
                continue;
            }
            let attempts = self.attempts.entry(key).or_default();
            if *attempts > features.len() {
                continue;
            }
            *attempts += 1;
            let flips = features.iter().map(|feature| {
                let mut flipped = config.clone();
                if !flipped.remove(feature) {
                    flipped.insert(*feature);
                }
                flipped
            });
            let all_on: Config = config.union(&features).copied().collect();
            let all_off: Config = config.difference(&features).copied().collect();
            let candidates: Vec<Config> = flips.chain([all_on, all_off]).collect();
            for candidate in candidates {
                if self.is_new(&candidate) {
                    self.pending.push_back(Pending {
                        branch: Some(key),
                        config: candidate,
                    });
                }
            }
        }
    }

    fn universe(&self) -> BTreeSet<u32> {
        self.runs
            .iter()
            .filter(|(config, run)| {
                run.outcome == Outcome::Clean && !self.rejected.contains_key(*config)
            })
            .flat_map(|(_, run)| run.elements.iter().copied())
            .collect()
    }

    fn greedy(&self) -> Vec<Config> {
        let mut uncovered = self.universe();
        let mut candidates: Vec<(&Config, &Run)> = self
            .runs
            .iter()
            .filter(|(config, run)| {
                run.outcome == Outcome::Clean && !self.rejected.contains_key(*config)
            })
            .collect();
        candidates.sort_by_key(|(config, _)| (config.len(), (*config).clone()));
        let mut chosen = Vec::new();
        let Some((default, _)) = candidates.first() else {
            return chosen;
        };
        let default = (*default).clone();
        while !uncovered.is_empty() {
            let mut best: Option<(usize, &Config, &Run)> = None;
            for (config, run) in &candidates {
                let gain = run
                    .elements
                    .iter()
                    .filter(|element| uncovered.contains(element))
                    .count();
                if best.is_none_or(|(most, ..)| gain > most) {
                    best = Some((gain, config, run));
                }
            }
            let Some((_, config, run)) = best else {
                break;
            };
            for element in &run.elements {
                uncovered.remove(element);
            }
            chosen.push(config.clone());
        }
        if chosen.is_empty() {
            chosen.push(default);
        }
        chosen
    }

    fn select(&mut self) -> Vec<Config> {
        loop {
            let chosen = self.greedy();
            let mut failed = false;
            for config in &chosen {
                if self.accepted.contains(config) {
                    continue;
                }
                match self.check(config) {
                    Ok(()) => {
                        self.accepted.insert(config.clone());
                    }
                    Err(message) => {
                        self.rejected.insert(config.clone(), message);
                        failed = true;
                    }
                }
            }
            if !failed {
                return chosen;
            }
        }
    }

    fn check(&self, config: &Config) -> Result<(), String> {
        let command = self
            .matrix
            .command(self.project, self.target, &self.names(config), self.source)
            .map_err(|error| error.to_string())?;
        frontend::parse_module_with_compiler_args(&command.file, command.args)
            .map(|_| ())
            .map_err(|error| error.to_string())
    }

    fn count_outcome(&self, outcome: Outcome) -> usize {
        self.runs
            .values()
            .filter(|run| run.outcome == outcome)
            .count()
    }

    fn finish(&mut self) -> UnitPlan {
        let chosen = self.select();
        let mut error = self.error.clone();
        if chosen.is_empty() && error.is_none() {
            error = Some(match self.rejected.values().next() {
                Some(message) => format!("every configuration failed: {message}"),
                None => "every configuration reached #error".to_string(),
            });
        }
        UnitPlan {
            source: self.source.to_path_buf(),
            relevant: self.names(&self.relevant),
            configurations: chosen.iter().map(|config| self.names(config)).collect(),
            elements: self.universe().len(),
            runs: self.runs.len(),
            excluded: self.count_outcome(Outcome::HitError),
            failed: self.count_outcome(Outcome::Failed),
            rejected: self
                .rejected
                .iter()
                .map(|(config, message)| Rejected {
                    configuration: self.names(config),
                    message: message.clone(),
                })
                .collect(),
            truncated: self.truncated,
            error,
            exhaustive: None,
        }
    }
}

#[derive(Default)]
struct Item {
    features: Config,
    start: Option<(FileId, usize)>,
    end: usize,
}

fn element_key(files: &Files, token: &Span<Token>, text: &str) -> String {
    format!(
        "{}:{}\0{text}",
        files.path(token.expansion.file).display(),
        token.expansion.offset
    )
}
