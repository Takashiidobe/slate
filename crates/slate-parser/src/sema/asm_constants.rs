use super::expression::Lowerer;
use super::fold::{integer_number, integer_with_objects};
use crate::ast::Span;
use crate::compiler_options::Optimization;
use crate::ir::*;
use foldhash::{HashMap, HashMapExt, HashSet, HashSetExt};

type Known = HashMap<BindingId, Value>;

#[derive(Default)]
struct Uses {
    bindings: HashMap<BindingId, usize>,
    reads: HashMap<BindingId, usize>,
    callees: Vec<BindingId>,
    locals: Vec<Variable>,
    rejected: bool,
    nested: bool,
}

impl Uses {
    fn of(body: &mut Vec<Span<Statement>>) -> Self {
        let mut uses = Self::default();
        walk_body(body, &mut uses);
        uses
    }

    fn stable(&self, id: BindingId) -> bool {
        self.bindings.get(&id) == self.reads.get(&id)
    }

    fn references(&self) -> impl Iterator<Item = BindingId> + '_ {
        self.bindings
            .keys()
            .copied()
            .chain(self.callees.iter().copied())
    }
}

impl Visitor for Uses {
    fn statement(&mut self, statement: &Statement) {
        match statement {
            Statement::Let(variable) => {
                self.callees.extend(variable.cleanup);
                self.locals.push(variable.clone())
            }
            Statement::Asm(asm) => {
                self.rejected |= asm.alternative.is_none() && !asm.rejected.is_empty()
            }
            Statement::NestedFunction(_) => self.nested = true,
            _ => {}
        }
    }

    fn value_kind(&mut self, kind: &ValueKind) {
        match kind {
            ValueKind::Read { place, .. } => {
                if let PlaceKind::Binding(id) = place.without_macro_holes().kind {
                    *self.reads.entry(id).or_default() += 1;
                }
            }
            ValueKind::Call {
                callee: Callee::Direct(id),
                ..
            } => self.callees.push(*id),
            _ => {}
        }
    }

    fn binding(&mut self, binding: BindingId) {
        *self.bindings.entry(binding).or_default() += 1;
    }
}

struct Redirect<'a> {
    lower: &'a mut Lowerer,
    known: &'a Known,
    candidates: &'a HashSet<BindingId>,
}

impl Visitor for Redirect<'_> {
    fn rewrite_value(&mut self, value: &mut Value) {
        let ValueKind::Call {
            callee: Callee::Direct(callee),
            arguments,
            ..
        } = &mut value.node.value
        else {
            return;
        };
        if !self.candidates.contains(callee) {
            return;
        }
        let arguments = arguments
            .iter()
            .map(|argument| self.lower.known_value(argument, self.known))
            .collect();
        if let Some(clone) = self.lower.specialize(*callee, arguments, self.candidates) {
            *callee = clone;
        }
    }
}

struct Reselect<'a> {
    lower: &'a Lowerer,
    known: &'a Known,
}

impl Visitor for Reselect<'_> {
    fn rewrite_statement(&mut self, statement: &mut Statement) {
        if let Statement::Asm(asm) = statement {
            self.lower.reselect(asm, self.known);
        }
    }
}

fn entry_prefix(body: &[Span<Statement>], entry: &Known) -> usize {
    if entry.is_empty() {
        return 0;
    }
    let mut count = 0;
    for statement in body {
        let mut single = vec![statement.clone()];
        let mut targets = JumpTargets::default();
        walk_body(&mut single, &mut targets);
        if targets.labels {
            break;
        }
        let uses = Uses::of(&mut single);
        if entry.keys().all(|id| uses.stable(*id)) {
            count += 1;
            continue;
        }
        if matches!(statement.value, Statement::Write { .. } | Statement::Asm(_)) {
            count += 1;
        }
        break;
    }
    count
}

#[derive(Default)]
pub(super) struct JumpTargets {
    labels: bool,
    switches: HashSet<BindingId>,
    cases: Vec<BindingId>,
}

impl JumpTargets {
    fn enterable(self) -> bool {
        self.labels
            || self
                .cases
                .iter()
                .any(|switch| !self.switches.contains(switch))
    }

    pub(super) fn of_statements(body: &mut Vec<Span<Statement>>) -> bool {
        let mut targets = Self::default();
        walk_body(body, &mut targets);
        targets.enterable()
    }

    pub(super) fn of_switch(id: BindingId, body: &mut Vec<Span<Statement>>) -> bool {
        let mut targets = Self::default();
        targets.switches.insert(id);
        walk_body(body, &mut targets);
        targets.enterable()
    }

    fn of_value(value: &mut Value) -> bool {
        let mut targets = Self::default();
        walk_value(value, &mut targets);
        targets.enterable()
    }
}

impl Visitor for JumpTargets {
    fn statement(&mut self, statement: &Statement) {
        match statement {
            Statement::Label { .. } => self.labels = true,
            Statement::Switch { id, .. } => {
                self.switches.insert(*id);
            }
            Statement::Case { switch, .. } | Statement::Default { switch, .. } => {
                self.cases.push(*switch)
            }
            _ => {}
        }
    }
}

struct DeadArms<'a> {
    lower: &'a Lowerer,
    unresolved: &'a HashSet<BindingId>,
    pruned: bool,
}

impl DeadArms<'_> {
    fn taken(
        &self,
        condition: &mut Value,
        unanswered_arm: impl FnOnce(bool) -> Uses,
    ) -> Option<bool> {
        if let Some(taken) = self.lower.branch_integer(condition) {
            return Some(taken != 0.into());
        }
        let taken = self.lower.folded_condition(condition)?;
        let mut tested = Uses::default();
        walk_value(condition, &mut tested);
        (self.needs_resolution(&tested) || self.needs_resolution(&unanswered_arm(taken)))
            .then_some(taken)
    }

    fn needs_resolution(&self, uses: &Uses) -> bool {
        uses.rejected
            || uses
                .callees
                .iter()
                .any(|callee| self.unresolved.contains(callee))
    }
}

impl Visitor for DeadArms<'_> {
    fn rewrite_statement(&mut self, statement: &mut Statement) {
        let Statement::If {
            condition,
            then_body,
            else_body,
            ..
        } = statement
        else {
            return;
        };
        let Some(taken) = self.taken(condition, |taken| {
            if taken {
                else_body.as_mut().map(Uses::of).unwrap_or_default()
            } else {
                Uses::of(then_body)
            }
        }) else {
            return;
        };
        let (kept, mut dropped) = if taken {
            (
                std::mem::take(then_body),
                else_body.take().unwrap_or_default(),
            )
        } else {
            (
                else_body.take().unwrap_or_default(),
                std::mem::take(then_body),
            )
        };
        if JumpTargets::of_statements(&mut dropped) {
            let (then_value, else_value) = if taken {
                (kept, dropped)
            } else {
                (dropped, kept)
            };
            *then_body = then_value;
            *else_body = Some(else_value);
            return;
        }
        *statement = Statement::Block(kept);
        self.pruned = true;
    }

    fn rewrite_value(&mut self, value: &mut Value) {
        let ValueKind::Conditional {
            condition,
            then_value,
            else_value,
        } = &mut value.node.value
        else {
            return;
        };
        let Some(taken) = self.taken(condition, |taken| {
            let mut uses = Uses::default();
            let dropped = if taken {
                &mut **else_value
            } else {
                &mut **then_value
            };
            walk_value(dropped, &mut uses);
            uses
        }) else {
            return;
        };
        let (kept, dropped) = if taken {
            (then_value, else_value)
        } else {
            (else_value, then_value)
        };
        if kept.ty != value.ty || JumpTargets::of_value(dropped) {
            return;
        }
        *value = kept.as_ref().clone();
        self.pruned = true;
    }
}

#[derive(Default)]
pub(super) struct Specializations {
    clones: HashMap<(BindingId, Vec<String>), Option<BindingId>>,
    active: HashSet<BindingId>,
    count: usize,
}

impl Lowerer {
    pub(super) fn constant_asm_operands(&mut self) {
        if self.types.dialect().options().codegen.optimization == Optimization::None {
            return;
        }
        for index in 0..self.module.functions.len() {
            let Some(mut body) = self.module.functions[index].value.body.take() else {
                continue;
            };
            let known = self.known_locals(&mut body, Known::new());
            walk_body(
                &mut body,
                &mut Reselect {
                    lower: self,
                    known: &known,
                },
            );
            self.module.functions[index].value.body = Some(body);
        }
        let mut unresolved = self.unresolved_functions();
        if self.prune_dead_arms(&unresolved) {
            unresolved = self.unresolved_functions();
        }
        let candidates: HashSet<BindingId> = self
            .module
            .functions
            .iter()
            .filter(|function| {
                unresolved.contains(&function.value.id)
                    && matches!(function.value.semantics.inlining, Some(Inlining::Always))
                    && !function.value.semantics.naked
            })
            .map(|function| function.value.id)
            .collect();
        if candidates.is_empty() {
            return;
        }
        let originals = self.module.functions.len();
        for index in 0..originals {
            let Some(mut body) = self.module.functions[index].value.body.take() else {
                continue;
            };
            let known = self.known_locals(&mut body, Known::new());
            walk_body(
                &mut body,
                &mut Redirect {
                    lower: self,
                    known: &known,
                    candidates: &candidates,
                },
            );
            self.module.functions[index].value.body = Some(body);
        }
        self.drop_unreferenced(&unresolved);
    }

    fn unresolved_functions(&mut self) -> HashSet<BindingId> {
        let mut unresolved = HashSet::new();
        let mut calls = HashMap::new();
        for function in &mut self.module.functions {
            let id = function.value.id;
            let Some(body) = &mut function.value.body else {
                continue;
            };
            let uses = Uses::of(body);
            if uses.rejected {
                unresolved.insert(id);
            }
            calls.insert(id, uses.callees);
        }
        let mut changed = true;
        while changed {
            changed = false;
            for (caller, callees) in &calls {
                if !unresolved.contains(caller)
                    && callees.iter().any(|callee| unresolved.contains(callee))
                {
                    unresolved.insert(*caller);
                    changed = true;
                }
            }
        }
        unresolved
    }

    fn prune_dead_arms(&mut self, unresolved: &HashSet<BindingId>) -> bool {
        let mut pruned = false;
        for index in 0..self.module.functions.len() {
            if !unresolved.contains(&self.module.functions[index].value.id) {
                continue;
            }
            let Some(mut body) = self.module.functions[index].value.body.take() else {
                continue;
            };
            let mut dead = DeadArms {
                lower: self,
                unresolved,
                pruned: false,
            };
            walk_body(&mut body, &mut dead);
            pruned |= dead.pruned;
            self.module.functions[index].value.body = Some(body);
        }
        pruned
    }

    pub(super) fn folded_condition(&self, condition: &Value) -> Option<bool> {
        self.folded_integer(condition)
            .map(|value| value != 0.into())
    }

    pub(super) fn folded_integer(&self, value: &Value) -> Option<num_bigint::BigInt> {
        integer_with_objects(
            value,
            self.types.fold_target(),
            self.types.entities.constants(),
        )
    }

    fn known_locals(&self, body: &mut Vec<Span<Statement>>, mut known: Known) -> Known {
        let uses = Uses::of(body);
        for local in &uses.locals {
            if local.storage == StorageDuration::Automatic
                && !local.access.volatile
                && uses.stable(local.id)
                && let Some(initializer) = &local.initializer
                && let Some(value) = self.known_value(initializer, &known)
            {
                known.insert(local.id, value);
            }
        }
        known
    }

    fn known_value(&self, value: &Value, known: &Known) -> Option<Value> {
        let value = super::asm::substitute(value, known);
        if matches!(
            value.ty,
            Type::Numeric(NumericType::Integer { .. }) | Type::Bool
        ) && let Some(constant) = integer_with_objects(
            &value,
            self.types.fold_target(),
            self.types.entities.constants(),
        ) {
            let mut folded = value;
            folded.node.value = ValueKind::Constant(integer_number(&folded.ty, constant));
            return Some(folded);
        }
        self.symbol(&value).is_some().then_some(value)
    }

    fn specialize(
        &mut self,
        callee: BindingId,
        arguments: Vec<Option<Value>>,
        candidates: &HashSet<BindingId>,
    ) -> Option<BindingId> {
        let index = self
            .module
            .functions
            .iter()
            .position(|function| function.value.id == callee)?;
        let original = &self.module.functions[index];
        let Parameters::Prototype { fixed, .. } = &original.value.parameters else {
            return None;
        };
        let mut body = original.value.body.clone()?;
        let uses = Uses::of(&mut body);
        if uses.nested
            || uses
                .locals
                .iter()
                .any(|local| local.storage != StorageDuration::Automatic)
        {
            return None;
        }
        let mut known = Known::new();
        let mut entry = Known::new();
        for (parameter, argument) in fixed.iter().zip(arguments) {
            if let Some(argument) = argument {
                if uses.stable(parameter.value.id) {
                    known.insert(parameter.value.id, argument);
                } else {
                    entry.insert(parameter.value.id, argument);
                }
            }
        }
        let prefix = entry_prefix(&body, &entry);
        if prefix == 0 {
            entry.clear();
        }
        if (known.is_empty() && entry.is_empty()) || self.specializations.active.contains(&callee) {
            return None;
        }
        let mut key: Vec<_> = known
            .iter()
            .chain(&entry)
            .map(|(id, value)| format!("{}={value}", id.0))
            .collect();
        key.sort();
        let key = (callee, key);
        if let Some(clone) = self.specializations.clones.get(&key) {
            return *clone;
        }
        let mut function = original.clone();
        self.specializations.active.insert(callee);
        let known = self.known_locals(&mut body, known);
        let mut rest = body.split_off(prefix);
        let mut at_entry = known.clone();
        at_entry.extend(entry);
        for (part, known) in [(&mut body, &at_entry), (&mut rest, &known)] {
            walk_body(part, &mut Reselect { lower: self, known });
            walk_body(
                part,
                &mut Redirect {
                    lower: self,
                    known,
                    candidates,
                },
            );
        }
        body.append(&mut rest);
        self.specializations.active.remove(&callee);
        let uses = Uses::of(&mut body);
        let resolved = !uses.rejected && !uses.callees.iter().any(|id| candidates.contains(id));
        let clone = resolved.then(|| {
            let id = BindingId(self.next_id);
            self.next_id += 1;
            self.specializations.count += 1;
            let name = format!(
                "{}__const{}",
                function.value.name, self.specializations.count
            );
            function.value.id = id;
            function.value.name = name;
            function.value.linkage = Linkage::Internal;
            function.value.symbol = SymbolAttributes {
                section: function.value.symbol.section.take(),
                ..SymbolAttributes::default()
            };
            function.value.semantics.inline_only = false;
            function.value.semantics.discardable = false;
            function.value.semantics.weak_odr = false;
            function.value.body = Some(body);
            if let Some(c) = self.types.entities.ty(&callee) {
                self.types.entities.declare(id, c, false);
            }
            self.module.functions.push(function);
            id
        });
        self.specializations.clones.insert(key, clone);
        clone
    }

    fn drop_unreferenced(&mut self, unresolved: &HashSet<BindingId>) {
        let named: HashSet<String> = self
            .module
            .functions
            .iter()
            .map(|function| &function.value.symbol)
            .chain(
                self.module
                    .globals
                    .iter()
                    .map(|global| &global.value.symbol),
            )
            .flat_map(|symbol| [&symbol.alias, &symbol.ifunc, &symbol.weakref])
            .flatten()
            .cloned()
            .collect();
        loop {
            let mut referenced = HashSet::new();
            for function in &mut self.module.functions {
                let id = function.value.id;
                if let Some(body) = &mut function.value.body {
                    referenced.extend(Uses::of(body).references().filter(|used| *used != id));
                }
            }
            for global in &mut self.module.globals {
                if let Some(initializer) = &mut global.value.variable.initializer {
                    let mut uses = Uses::default();
                    walk_value(initializer, &mut uses);
                    referenced.extend(uses.references());
                }
            }
            let before = self.module.functions.len();
            self.module.functions.retain(|function| {
                let function = &function.value;
                referenced.contains(&function.id)
                    || named.contains(&function.name)
                    || !unresolved.contains(&function.id)
                    || !matches!(function.linkage, Linkage::Internal)
                    || function.body.is_none()
                    || function.symbol.used
                    || function.symbol.retain
                    || function.semantics.constructor.is_some()
                    || function.semantics.destructor.is_some()
            });
            if self.module.functions.len() == before {
                return;
            }
        }
    }
}
