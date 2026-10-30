use super::expand::PPToken;
use super::syntax::{Directive, directive_spelling, identifier};
use super::{Preprocessor, is_defined_operator};
use crate::ast::{FileId, Loc, Span};
use crate::lexer::Token;
use std::collections::{BTreeSet, HashMap};
use std::path::Path;

const COMMAND_LINE: &str = "<command line>";

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash, Default)]
pub struct ContextId(u32);

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash, Default)]
pub struct MacroSet(u32);

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct MacroEventId(u32);

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct ConditionalId(u32);

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct Branch {
    pub conditional: ConditionalId,
    pub arm: u32,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash, Default)]
pub struct TokenTrace {
    pub context: ContextId,
    pub macros: MacroSet,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ArmState {
    Taken,
    NotTaken,
    NotEvaluated,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord)]
pub struct MacroRead {
    pub name: String,
    pub defined: bool,
    pub event: Option<MacroEventId>,
    pub skipped: Vec<MacroEventId>,
}

#[derive(Debug, Clone)]
pub struct Arm {
    pub directive: &'static str,
    pub loc: Loc,
    pub state: ArmState,
    pub names: Vec<String>,
    pub reads: Vec<MacroRead>,
}

#[derive(Debug, Clone)]
pub struct Conditional {
    pub context: ContextId,
    pub arms: Vec<Arm>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum MacroEventKind {
    Define,
    Undef,
}

#[derive(Debug, Clone)]
pub struct MacroEvent {
    pub name: String,
    pub kind: MacroEventKind,
    pub loc: Loc,
    pub context: ContextId,
    pub command_line: bool,
    pub skipped: bool,
}

#[derive(Debug, Clone)]
pub struct Expansion {
    pub name: String,
    pub site: Loc,
    pub trace: TokenTrace,
    pub empty: bool,
}

#[derive(Debug, Clone, Default)]
pub struct Dependencies {
    pub branches: BTreeSet<Branch>,
    pub events: BTreeSet<MacroEventId>,
    pub roots: BTreeSet<String>,
}

#[derive(Debug)]
pub struct PreprocessTrace {
    pub main_file: Option<FileId>,
    pub conditionals: Vec<Conditional>,
    pub events: Vec<MacroEvent>,
    pub expansions: Vec<Expansion>,
    contexts: Vec<(ContextId, Option<Branch>)>,
    sets: Vec<Vec<MacroEventId>>,
    interned_contexts: HashMap<(ContextId, Branch), ContextId>,
    interned_sets: HashMap<Vec<MacroEventId>, MacroSet>,
    unions: HashMap<(MacroSet, MacroSet), MacroSet>,
    open: Vec<ConditionalId>,
    current: ContextId,
    reads: Vec<MacroRead>,
    reading: bool,
    undefined: HashMap<String, MacroEventId>,
    skipped: HashMap<String, Vec<MacroEventId>>,
}

impl PreprocessTrace {
    fn new() -> Self {
        Self {
            main_file: None,
            conditionals: Vec::new(),
            events: Vec::new(),
            expansions: Vec::new(),
            contexts: vec![(ContextId(0), None)],
            sets: vec![Vec::new()],
            interned_contexts: HashMap::new(),
            interned_sets: HashMap::from([(Vec::new(), MacroSet(0))]),
            unions: HashMap::new(),
            open: Vec::new(),
            current: ContextId(0),
            reads: Vec::new(),
            reading: false,
            undefined: HashMap::new(),
            skipped: HashMap::new(),
        }
    }

    pub fn conditional(&self, id: ConditionalId) -> &Conditional {
        &self.conditionals[id.0 as usize]
    }

    pub fn event(&self, id: MacroEventId) -> &MacroEvent {
        &self.events[id.0 as usize]
    }

    pub fn macros(&self, set: MacroSet) -> &[MacroEventId] {
        &self.sets[set.0 as usize]
    }

    pub fn branches(&self, context: ContextId) -> Vec<Branch> {
        let mut branches = Vec::new();
        let mut current = context;
        while let (parent, Some(branch)) = self.contexts[current.0 as usize] {
            branches.push(branch);
            current = parent;
        }
        branches.reverse();
        branches
    }

    pub fn dependencies(&self, trace: TokenTrace) -> Dependencies {
        let mut result = Dependencies::default();
        let mut contexts = vec![trace.context];
        let mut events: Vec<MacroEventId> = self.macros(trace.macros).to_vec();
        loop {
            if let Some(context) = contexts.pop() {
                for branch in self.branches(context) {
                    if !result.branches.insert(branch) {
                        continue;
                    }
                    let arms = &self.conditional(branch.conditional).arms;
                    for arm in &arms[..=branch.arm as usize] {
                        for read in &arm.reads {
                            events.extend(&read.skipped);
                            match read.event {
                                Some(event) => events.push(event),
                                None => {
                                    result.roots.insert(read.name.clone());
                                }
                            }
                        }
                    }
                }
            } else if let Some(event) = events.pop() {
                if result.events.insert(event) {
                    let event = self.event(event);
                    contexts.push(event.context);
                    if event.command_line {
                        result.roots.insert(event.name.clone());
                    }
                }
            } else {
                return result;
            }
        }
    }

    fn intern_set(&mut self, set: Vec<MacroEventId>) -> MacroSet {
        if let Some(id) = self.interned_sets.get(&set) {
            return *id;
        }
        let id = MacroSet(self.sets.len() as u32);
        self.sets.push(set.clone());
        self.interned_sets.insert(set, id);
        id
    }

    fn union(&mut self, left: MacroSet, right: MacroSet) -> MacroSet {
        if left == right || right == MacroSet(0) {
            return left;
        }
        if left == MacroSet(0) {
            return right;
        }
        let key = (left.min(right), left.max(right));
        if let Some(id) = self.unions.get(&key) {
            return *id;
        }
        let mut merged = self.macros(left).to_vec();
        merged.extend_from_slice(self.macros(right));
        merged.sort_unstable();
        merged.dedup();
        let id = self.intern_set(merged);
        self.unions.insert(key, id);
        id
    }

    fn with_event(&mut self, set: MacroSet, event: MacroEventId) -> MacroSet {
        let single = self.intern_set(vec![event]);
        self.union(set, single)
    }

    fn with_skipped(&mut self, set: MacroSet, name: &str) -> MacroSet {
        let Some(skipped) = self.skipped.get(name).cloned() else {
            return set;
        };
        let skipped = self.intern_set(skipped);
        self.union(set, skipped)
    }

    fn merge(&mut self, left: TokenTrace, right: TokenTrace) -> TokenTrace {
        TokenTrace {
            context: if left.context == ContextId(0) {
                right.context
            } else {
                left.context
            },
            macros: self.union(left.macros, right.macros),
        }
    }

    fn read(&mut self, read: MacroRead) {
        if !self.reads.contains(&read) {
            self.reads.push(read);
        }
    }

    fn enter(&mut self, parent: ContextId, branch: Branch) {
        self.current = match self.interned_contexts.get(&(parent, branch)) {
            Some(context) => *context,
            None => {
                let context = ContextId(self.contexts.len() as u32);
                self.contexts.push((parent, Some(branch)));
                self.interned_contexts.insert((parent, branch), context);
                context
            }
        };
    }
}

impl Preprocessor<'_> {
    pub fn enable_trace(&mut self) {
        self.trace = Some(Box::new(PreprocessTrace::new()));
    }

    pub fn take_trace(&mut self) -> Option<PreprocessTrace> {
        let mut trace = self.trace.take()?;
        trace.main_file = self.main_file;
        Some(*trace)
    }

    pub(super) fn trace_context(&self) -> TokenTrace {
        TokenTrace {
            context: self
                .trace
                .as_ref()
                .map_or_else(ContextId::default, |trace| trace.current),
            macros: MacroSet::default(),
        }
    }

    pub(super) fn trace_of_tokens(&self, count: usize) -> Vec<TokenTrace> {
        if self.trace.is_some() {
            vec![self.trace_context(); count]
        } else {
            Vec::new()
        }
    }

    pub(super) fn trace_open(&mut self) {
        if let Some(trace) = &mut self.trace {
            let id = ConditionalId(trace.conditionals.len() as u32);
            trace.conditionals.push(Conditional {
                context: trace.current,
                arms: Vec::new(),
            });
            trace.open.push(id);
            trace.reads.clear();
            trace.reading = true;
        }
    }

    pub(super) fn trace_begin_arm(&mut self) {
        if let Some(trace) = &mut self.trace {
            trace.reads.clear();
            trace.reading = true;
        }
    }

    pub(super) fn trace_arm(&mut self, directive: &Directive, taken: Option<bool>) {
        if self.trace.is_none() {
            return;
        }
        let source = self.source(directive.loc.file);
        let names: Vec<String> = directive
            .arguments
            .iter()
            .filter_map(|token| identifier(source, token))
            .filter(|name| name != "defined")
            .collect::<BTreeSet<_>>()
            .into_iter()
            .collect();
        if taken.is_none() {
            for name in &names {
                self.trace_read_macro(name);
            }
        }
        let Some(trace) = &mut self.trace else {
            return;
        };
        trace.reading = false;
        let Some(&id) = trace.open.last() else {
            return;
        };
        let reads = std::mem::take(&mut trace.reads);
        let conditional = &mut trace.conditionals[id.0 as usize];
        let arm = conditional.arms.len() as u32;
        let parent = conditional.context;
        conditional.arms.push(Arm {
            directive: directive_spelling(directive.name),
            loc: directive.loc,
            state: match taken {
                Some(true) => ArmState::Taken,
                Some(false) => ArmState::NotTaken,
                None => ArmState::NotEvaluated,
            },
            names,
            reads,
        });
        trace.enter(
            parent,
            Branch {
                conditional: id,
                arm,
            },
        );
    }

    pub(super) fn trace_close(&mut self) {
        if let Some(trace) = &mut self.trace
            && let Some(id) = trace.open.pop()
        {
            trace.current = trace.conditional(id).context;
        }
    }

    pub(super) fn trace_macro_event(
        &mut self,
        name: &str,
        kind: MacroEventKind,
        loc: Loc,
    ) -> Option<MacroEventId> {
        let command_line = self.files.path(loc.file) == Path::new(COMMAND_LINE);
        let trace = self.trace.as_mut()?;
        let id = MacroEventId(trace.events.len() as u32);
        trace.events.push(MacroEvent {
            name: name.to_string(),
            kind,
            loc,
            context: trace.current,
            command_line,
            skipped: false,
        });
        match kind {
            MacroEventKind::Define => {
                trace.undefined.remove(name);
            }
            MacroEventKind::Undef => {
                trace.undefined.insert(name.to_string(), id);
            }
        }
        Some(id)
    }

    pub(super) fn trace_skipped_macro(&mut self, directive: &Directive, kind: MacroEventKind) {
        if self.trace.is_none() {
            return;
        }
        let source = self.source(directive.loc.file);
        let Some(name) = directive
            .arguments
            .first()
            .and_then(|token| identifier(source, token))
        else {
            return;
        };
        if let Some(trace) = &mut self.trace {
            let id = MacroEventId(trace.events.len() as u32);
            trace.events.push(MacroEvent {
                name: name.clone(),
                kind,
                loc: directive.loc,
                context: trace.current,
                command_line: false,
                skipped: true,
            });
            trace.skipped.entry(name).or_default().push(id);
        }
    }

    pub(super) fn trace_read_macro(&mut self, name: &str) {
        if self.trace.is_none() {
            return;
        }
        let entry = self.macros.get(name);
        let defined = entry.is_some() || is_defined_operator(name, self.dialect.flavor());
        let definition = entry.and_then(|entry| entry.trace_event);
        if let Some(trace) = &mut self.trace {
            let event = definition.or_else(|| trace.undefined.get(name).copied());
            let skipped = trace.skipped.get(name).cloned().unwrap_or_default();
            trace.read(MacroRead {
                name: name.to_string(),
                defined,
                event,
                skipped,
            });
        }
    }

    pub(super) fn trace_condition_names(&mut self, tokens: &[Span<Token>]) {
        if self.trace.is_none() {
            return;
        }
        for token in tokens {
            if let Token::Ident(name) = &token.value
                && !matches!(name.as_str(), "defined" | "true" | "false")
            {
                self.trace_read_macro(name);
            }
        }
    }

    pub(super) fn trace_unexpanded(&mut self, mut token: PPToken) -> PPToken {
        if let Some(trace) = &mut self.trace
            && let Token::Ident(name) = &token.token.value
            && !self.macros.contains_key(name.as_str())
        {
            if let Some(&event) = trace.undefined.get(name.as_str()) {
                token.trace.macros = trace.with_event(token.trace.macros, event);
            }
            token.trace.macros = trace.with_skipped(token.trace.macros, name);
        }
        token
    }

    pub(super) fn trace_expansion(
        &mut self,
        name: &str,
        invoker: &PPToken,
        closer: Option<TokenTrace>,
        mut replacement: Vec<PPToken>,
    ) -> Vec<PPToken> {
        if self.trace.is_none() {
            return replacement;
        }
        self.trace_read_if_reading(name);
        let event = self.macros.get(name).and_then(|entry| entry.trace_event);
        let Some(trace) = &mut self.trace else {
            return replacement;
        };
        let mut inherited = invoker.trace;
        if let Some(closer) = closer {
            inherited = trace.merge(inherited, closer);
        }
        if let Some(event) = event {
            inherited.macros = trace.with_event(inherited.macros, event);
        }
        inherited.macros = trace.with_skipped(inherited.macros, name);
        for token in &mut replacement {
            token.trace = trace.merge(token.trace, inherited);
        }
        if !trace.reading {
            trace.expansions.push(Expansion {
                name: name.to_string(),
                site: invoker.token.expansion,
                trace: inherited,
                empty: replacement.is_empty(),
            });
        }
        replacement
    }

    fn trace_read_if_reading(&mut self, name: &str) {
        if self.trace.as_ref().is_some_and(|trace| trace.reading) {
            self.trace_read_macro(name);
        }
    }

    pub(super) fn trace_merged<'t>(
        &mut self,
        tokens: impl IntoIterator<Item = &'t PPToken>,
    ) -> TokenTrace {
        let Some(trace) = &mut self.trace else {
            return TokenTrace::default();
        };
        tokens
            .into_iter()
            .fold(TokenTrace::default(), |merged, token| {
                trace.merge(merged, token.trace)
            })
    }
}
