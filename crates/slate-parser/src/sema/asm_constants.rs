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
        walk_macro_structure(body, &mut uses);
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

impl MacroStructureVisitor for Uses {
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

impl MacroStructureVisitor for Redirect<'_> {
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

impl MacroStructureVisitor for Reselect<'_> {
    fn rewrite_statement(&mut self, statement: &mut Statement) {
        if let Statement::Asm(asm) = statement {
            self.lower.reselect(asm, self.known);
        }
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
        let mut unresolved = HashSet::new();
        let mut calls = HashMap::new();
        for index in 0..self.module.functions.len() {
            let function = &self.module.functions[index].value;
            let id = function.id;
            let Some(mut body) = function.body.clone() else {
                continue;
            };
            let known = self.known_locals(&mut body, Known::new());
            walk_macro_structure(
                &mut body,
                &mut Reselect {
                    lower: self,
                    known: &known,
                },
            );
            let uses = Uses::of(&mut body);
            if uses.rejected {
                unresolved.insert(id);
            }
            calls.insert(id, uses.callees);
            self.module.functions[index].value.body = Some(body);
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
            walk_macro_structure(
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
        for (parameter, argument) in fixed.iter().zip(arguments) {
            if let Some(argument) = argument
                && uses.stable(parameter.value.id)
            {
                known.insert(parameter.value.id, argument);
            }
        }
        if known.is_empty() || self.specializations.active.contains(&callee) {
            return None;
        }
        let mut key: Vec<_> = known
            .iter()
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
        walk_macro_structure(
            &mut body,
            &mut Reselect {
                lower: self,
                known: &known,
            },
        );
        walk_macro_structure(
            &mut body,
            &mut Redirect {
                lower: self,
                known: &known,
                candidates,
            },
        );
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
                    walk_value_structure(initializer, &mut uses);
                    referenced.extend(uses.references());
                }
            }
            let before = self.module.functions.len();
            self.module.functions.retain(|function| {
                let function = &function.value;
                referenced.contains(&function.id)
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
