use super::super::ownership::Owned;
use super::super::{HeapAction, HeapStatement};
use super::{ESCAPE, MACRO, NodeKind, Reason, Use, Walker};
use crate::counted_loops::{integer_constant, plain_local};
use crate::def_use::DefUse;
use crate::heap_ir::{
    Ending, binding_write, code_statements, is_null_pointer, peel_pointer, plain_read,
    pointer_null_test, release_endings, released_binding,
};
use crate::identity::Known;
use foldhash::{HashMap, HashMapExt, HashSet};
use slate_parser::ast::{NodeId, Span};
use slate_parser::ir::{self, BindingId, Callee, PlaceKind, Statement, Value, ValueKind, Visitor};

pub(super) struct Release {
    pub(super) binding: BindingId,
    pub(super) call: NodeId,
    pub(super) position: u32,
    pub(super) depth: u32,
    pub(super) scope: u32,
    pub(super) ending: Ending,
}

#[derive(Clone, Copy)]
pub(super) struct Allocation {
    pub(super) depth: u32,
    pub(super) scope: u32,
}

#[derive(Clone, Copy)]
enum ResizeOrigin {
    Guarded { declaration: NodeId, check: NodeId },
    UncheckedWrite,
}

#[derive(Default)]
pub(super) struct Ownership {
    pub(super) allocated: HashMap<BindingId, Allocation>,
    pub(super) writes: HashMap<BindingId, u32>,
    pub(super) supported_writes: HashMap<BindingId, u32>,
    pub(super) direct_resizes: HashSet<BindingId>,
    pub(super) fallible_resizes: HashSet<BindingId>,
    pub(super) last_read: HashMap<BindingId, u32>,
    pub(super) releases: Vec<Release>,
    pub(super) exclusive_releases: HashMap<u32, u32>,
    pub(super) statements: HashMap<NodeId, HeapStatement>,
    resize_origins: HashMap<BindingId, ResizeOrigin>,
    pub(super) position: u32,
    pub(super) depth: u32,
    pub(super) scope: u32,
    pub(super) scopes: u32,
    pub(super) ending: Ending,
    pub(super) jumps: bool,
}

impl Ownership {
    fn elided(&self, binding: BindingId) -> impl Iterator<Item = NodeId> + '_ {
        let scope = self
            .allocated
            .get(&binding)
            .map(|allocation| allocation.scope);
        self.releases
            .iter()
            .filter(move |release| {
                release.binding == binding
                    && release.ending.elides_release(Some(release.scope) == scope)
            })
            .map(|release| release.call)
    }

    fn owns(&self, binding: BindingId) -> bool {
        let Some(&Allocation { depth, .. }) = self.allocated.get(&binding) else {
            return false;
        };
        let last = self.last_read.get(&binding).copied();
        !self.jumps
            && !(self.direct_resizes.contains(&binding) && self.fallible_resizes.contains(&binding))
            && self.writes.get(&binding) == self.supported_writes.get(&binding)
            && self
                .releases
                .iter()
                .filter(|release| release.binding == binding)
                .all(|release| {
                    release.ending == Ending::Returns
                        || (release.depth == depth
                            && (Some(release.position) == last
                                || self
                                    .exclusive_releases
                                    .get(&release.position)
                                    .is_some_and(|&other| Some(other) == last)))
                })
    }
}

impl Walker<'_, '_> {
    pub(super) fn statements(&mut self, body: &[Span<Statement>]) {
        let endings = release_endings(body, |statement| {
            released_binding(&statement.value, |callee| {
                self.known(callee) == Some(Known::Free)
            })
        });
        self.ownership.scopes += 1;
        let outer = std::mem::replace(&mut self.ownership.scope, self.ownership.scopes);
        for (index, (statement, ending)) in body.iter().zip(endings).enumerate() {
            let outer = std::mem::replace(&mut self.ownership.ending, ending);
            if self.macro_depth == 0 {
                self.plan_heap_statement(statement, &body[index + 1..]);
            }
            match self
                .ownership
                .statements
                .get(&statement.id)
                .map(|fact| fact.action)
            {
                Some(HeapAction::RemoveWrite)
                    if let Statement::Write { place, value, .. } = &statement.value
                        && let PlaceKind::Binding(binding) = place.kind =>
                {
                    self.value(value);
                    self.place(place, Use::Write);
                    *self.ownership.supported_writes.entry(binding).or_default() += 1;
                    if is_null_pointer(value) {
                        self.store(place, value);
                    } else if let Some(node) = self.node(binding) {
                        self.make_raw(node, Reason::HeapWrite);
                    }
                }
                Some(HeapAction::ClearedCheck(_))
                    if let Statement::If {
                        then_body,
                        else_body,
                        ..
                    } = &statement.value =>
                {
                    self.statements(then_body);
                    if let Some(body) = else_body {
                        self.statements(body);
                    }
                }
                _ => self.statement(statement),
            }
            self.ownership.ending = outer;
        }
        self.ownership.scope = outer;
    }

    fn plan_heap_statement(&mut self, statement: &Span<Statement>, following: &[Span<Statement>]) {
        let mut following = code_statements(following);
        if let Some(owner) = released_binding(&statement.value, |callee| {
            self.library_call(callee, Known::Free)
        }) {
            if !self.ownership.allocated.contains_key(&owner) {
                return;
            }
            let Some(clear) = following.next() else {
                return;
            };
            if !binding_write(&clear.value)
                .is_some_and(|(target, value)| target == owner && is_null_pointer(value))
            {
                return;
            }
            self.heap_statement(clear.id, owner, HeapAction::RemoveWrite);
            if let Some(check) = following.next()
                && let Statement::If {
                    condition,
                    folded: None,
                    ..
                } = &check.value
                && let Some((binding, equal)) = pointer_null_test(condition)
                && binding == owner
                && !self.macro_values.contains_key(&condition.node.id)
            {
                self.heap_statement(check.id, owner, HeapAction::ClearedCheck(equal));
            }
            return;
        }
        let Statement::Let(result) = &statement.value else {
            return;
        };
        let Some(initializer) = &result.initializer else {
            return;
        };
        let ValueKind::Call {
            callee, arguments, ..
        } = &peel_pointer(initializer).node.value
        else {
            return;
        };
        if !plain_local(result)
            || result.byref
            || result.alignment.is_some()
            || self.macro_values.contains_key(&initializer.node.id)
        {
            return;
        }
        if let [size] = arguments.as_slice()
            && self.library_call(callee, Known::Malloc)
            && let Some(next) = following.next()
        {
            if let Some(origin) = self.resize_origin(statement.id, result.id, size, next) {
                self.ownership.resize_origins.insert(result.id, origin);
            }
            return;
        }
        let [pointer, size] = arguments.as_slice() else {
            return;
        };
        if !self.library_call(callee, Known::Realloc) {
            return;
        }
        let pointer = peel_pointer(pointer);
        let Some(owner) = plain_read(pointer) else {
            return;
        };
        if result.ty != pointer.ty
            || !self.ownership.allocated.contains_key(&owner)
            || !positive_size(size)
        {
            return;
        }
        let Some(&origin) = self.ownership.resize_origins.get(&owner) else {
            return;
        };
        let (Some(check), Some(commit)) = (following.next(), following.next()) else {
            return;
        };
        let Statement::If {
            condition,
            then_body,
            else_body: None,
            folded: None,
        } = &check.value
        else {
            return;
        };
        if pointer_null_test(condition) != Some((result.id, true))
            || !self.plain_failure_return(then_body)
            || !binding_write(&commit.value).is_some_and(|(target, value)| {
                target == owner && plain_read(value) == Some(result.id)
            })
            || self.macro_values.contains_key(&condition.node.id)
            || self
                .program
                .unit(self.unit)
                .module
                .macro_hidden_arguments
                .values()
                .flatten()
                .any(|&binding| binding == result.id)
            || !self
                .def_use
                .get_or_init(|| DefUse::of(self.function_body))
                .matches_reads_and_writes(result.id, 2, 0)
        {
            return;
        }
        if let ResizeOrigin::Guarded { declaration, check } = origin {
            self.heap_statement(declaration, owner, HeapAction::TryAllocate);
            self.heap_statement(check, owner, HeapAction::AllocationFailed);
        }
        self.heap_statement(statement.id, owner, HeapAction::TryResize);
        self.heap_statement(check.id, owner, HeapAction::ResizeFailed(result.id));
        self.heap_statement(commit.id, owner, HeapAction::RemoveWrite);
        self.ownership.fallible_resizes.insert(owner);
    }

    fn resize_origin(
        &self,
        declaration: NodeId,
        binding: BindingId,
        size: &Value,
        next: &Span<Statement>,
    ) -> Option<ResizeOrigin> {
        if let Statement::If {
            condition,
            then_body,
            else_body: None,
            folded: None,
        } = &next.value
            && pointer_null_test(condition) == Some((binding, true))
            && !self.macro_values.contains_key(&condition.node.id)
            && positive_size(size)
            && self.plain_failure_return(then_body)
            && !DefUse::of(then_body).mentions(binding)
        {
            return Some(ResizeOrigin::Guarded {
                declaration,
                check: next.id,
            });
        }
        if let Statement::Write {
            place,
            value,
            ordering: None,
            ..
        } = &next.value
            && let PlaceKind::Deref(pointer) = &place.kind
            && plain_read(zero_offset_base(pointer)) == Some(binding)
            && integer_constant(value).is_some()
        {
            return Some(ResizeOrigin::UncheckedWrite);
        }
        None
    }

    fn plain_failure_return(&self, body: &[Span<Statement>]) -> bool {
        if !terminal_statement(body)
            .is_some_and(|statement| matches!(statement.value, Statement::Return(_)))
        {
            return false;
        }
        let module = self.program.unit(self.unit).module;
        let errno = module.globals.iter().find_map(|global| {
            matches!(global.variable.name.as_str(), "errno" | "_errno")
                .then_some(global.variable.id)
        });
        let mut effects = FailureEffects {
            walker: self,
            errno,
            observed: false,
        };
        ir::walk_function_body_with_macros(&mut body.to_vec(), self.macro_values, &mut effects);
        !effects.observed
    }

    fn library_call(&self, callee: &Callee, known: Known) -> bool {
        self.known(callee) == Some(known) && self.program.callee(self.unit, callee).is_none()
    }

    fn heap_statement(&mut self, id: NodeId, owner: BindingId, action: HeapAction) {
        self.ownership
            .statements
            .insert(id, HeapStatement { owner, action });
    }

    pub(super) fn terminal_release(&self, body: &[Span<Statement>]) -> Option<NodeId> {
        let last = terminal_statement(body)?;
        match &last.value {
            Statement::Expression(value)
                if released_binding(&last.value, |callee| {
                    self.library_call(callee, Known::Free)
                })
                .is_some() =>
            {
                Some(value.node.id)
            }
            _ => None,
        }
    }

    pub(super) fn looped(&mut self, walk: impl FnOnce(&mut Self)) {
        self.ownership.depth += 1;
        walk(self);
        self.ownership.depth -= 1;
    }

    pub(super) fn owned_candidates(&mut self) -> Vec<Owned> {
        let mut heap_statements: HashMap<_, Vec<_>> = HashMap::new();
        for (id, statement) in self.ownership.statements.drain() {
            heap_statements
                .entry(statement.owner)
                .or_default()
                .push((id, statement));
        }
        self.nodes
            .iter()
            .enumerate()
            .filter(|(_, item)| {
                item.kind == NodeKind::Local
                    && item.byte_elements
                    && !item.opaque_pointee
                    && item.flags & (ESCAPE | MACRO) == 0
                    && self.ownership.owns(item.binding)
            })
            .map(|(node, item)| Owned {
                node: node as u32,
                written: self.ownership.writes.contains_key(&item.binding),
                elided: self.ownership.elided(item.binding).collect(),
                statements: heap_statements.remove(&item.binding).unwrap_or_default(),
            })
            .collect()
    }
}

struct FailureEffects<'w, 'a, 'm> {
    walker: &'w Walker<'a, 'm>,
    errno: Option<BindingId>,
    observed: bool,
}

impl Visitor for FailureEffects<'_, '_, '_> {
    fn binding(&mut self, binding: BindingId) {
        self.observed |= self.errno == Some(binding);
    }

    fn value_kind(&mut self, kind: &ValueKind) {
        self.observed |= match kind {
            ValueKind::Call { callee, .. } => !self.walker.library_call(callee, Known::Free),
            ValueKind::Intrinsic { .. } => true,
            _ => false,
        };
    }
}

fn terminal_statement(mut body: &[Span<Statement>]) -> Option<&Span<Statement>> {
    loop {
        let last = code_statements(body).next_back()?;
        let Statement::Block(inner) = &last.value else {
            return Some(last);
        };
        body = inner;
    }
}

fn zero_offset_base(value: &Value) -> &Value {
    match &value.node.value {
        ValueKind::PointerOffset {
            pointer, amount, ..
        } if integer_constant(amount)
            .is_some_and(|amount| amount.sign() == num_bigint::Sign::NoSign) =>
        {
            pointer
        }
        _ => value,
    }
}

fn positive_size(value: &Value) -> bool {
    if let Some(constant) =
        integer_constant(value).or_else(|| slate_parser::sema::fold_integer(value))
    {
        return constant.sign() == num_bigint::Sign::Plus;
    }
    match &value.node.value {
        ValueKind::Copy { operand, .. } | ValueKind::Convert { operand, .. }
            if operand.ty == value.ty =>
        {
            positive_size(operand)
        }
        ValueKind::Conditional {
            then_value,
            else_value,
            ..
        } => positive_size(then_value) && positive_size(else_value),
        _ => false,
    }
}
