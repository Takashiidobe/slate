use crate::counted_loops::{CountedLoop, integer_constant, plain_access, plain_local};
use crate::def_use::DefUse;
use crate::identity::{FunctionIdentity, Known};
use crate::pointer::{release_endings, released_binding};
use crate::program::{Program, ProgramBinding, UnitId};
use foldhash::{HashMap, HashMapExt, HashSet, HashSetExt};
use slate_parser::ast::{NodeId, Span};
use slate_parser::ir::*;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum GuardedHeapStatement {
    Declare,
    Allocate {
        binding: BindingId,
        count: u64,
        zeroed: bool,
    },
    Present {
        binding: BindingId,
        elide_release: bool,
    },
    Resize {
        binding: BindingId,
        count: u64,
    },
    ResizeFailed(BindingId),
    ZeroFill {
        binding: BindingId,
        count: u64,
        overwrite: bool,
    },
    Remove,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum HeapIndex {
    Constant(u64),
    Binding(BindingId),
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct HeapElement {
    pub binding: BindingId,
    pub index: HeapIndex,
    pub append: bool,
}

#[derive(Debug, Default)]
pub(crate) struct GuardedHeap {
    pub statements: HashMap<NodeId, GuardedHeapStatement>,
    pub elements: HashMap<NodeId, HeapElement>,
    pub uninitialized: HashSet<BindingId>,
}

pub(crate) struct HeapInputs<'a> {
    pub hidden: &'a HashSet<BindingId>,
    pub uses: &'a DefUse,
    pub loops: &'a HashMap<BindingId, CountedLoop>,
}

pub(crate) fn analyze(
    program: &Program<'_>,
    unit: UnitId,
    function: &Function,
    identities: &HashMap<ProgramBinding, FunctionIdentity>,
    inputs: &HeapInputs<'_>,
    out: &mut GuardedHeap,
) {
    let module = program.unit(unit).module;
    if !module.hosted || module.nostdinc || !inputs.uses.whole_body_structured() {
        return;
    }
    let Some(body) = &function.body else {
        return;
    };
    let builtin = |callee: &Callee, known| {
        let Callee::Direct(binding) = callee else {
            return false;
        };
        identities.get(&ProgramBinding {
            unit,
            binding: *binding,
        }) == Some(&FunctionIdentity::Known(known))
            && program.callee(unit, callee).is_none()
    };
    let mut found_guard = false;
    let mut statements = code(body);
    while let Some(declaration) = statements.next() {
        let Statement::Let(variable) = &declaration.value else {
            continue;
        };
        let Some(element) = integer_pointee(&variable.ty) else {
            continue;
        };
        let binding = variable.id;
        if !plain_local(variable)
            || !variable.access.is_plain()
            || variable.byref
            || variable.alignment.is_some()
            || inputs.hidden.contains(&binding)
            || !variable.initializer.as_ref().is_some_and(is_null)
        {
            continue;
        }
        let mut following = statements.clone();
        let (Some(allocation), Some(guard)) = (following.next(), following.next()) else {
            break;
        };
        let Statement::If {
            then_body,
            else_body: None,
            folded: None,
            ..
        } = &allocation.value
        else {
            continue;
        };
        let mut allocation_body = code(arm(then_body));
        let Some(write) = allocation_body
            .next()
            .filter(|_| allocation_body.next().is_none())
        else {
            continue;
        };
        let Some(value) = write_binding(write, binding) else {
            continue;
        };
        let ValueKind::Call {
            callee, arguments, ..
        } = &peel_pointer(value).node.value
        else {
            continue;
        };
        let (size, multiplier, zeroed) = match arguments.as_slice() {
            [size] if builtin(callee, Known::Malloc) => (size, 1, false),
            [count, size] if builtin(callee, Known::Calloc) => {
                let Some(count) = unsigned_constant(count) else {
                    continue;
                };
                (size, count, true)
            }
            _ => continue,
        };
        let Some(count) = extent(size, element, module.target.pointer_width, multiplier) else {
            continue;
        };
        let Statement::If {
            condition,
            then_body,
            folded: None,
            ..
        } = &guard.value
        else {
            continue;
        };
        if !checks_null(condition, binding, CompareOp::Ne) {
            continue;
        }
        let mut proof = Cursor {
            module,
            binding,
            element,
            count,
            offset: 0,
            prefix: if zeroed { count } else { 0 },
            sparse: false,
            initialized: HashSet::new(),
            reads: 1,
            writes: 1,
            elements: HashMap::new(),
            actions: Vec::new(),
            inputs,
        };
        let Some(release) = proof.prove(arm(then_body), &builtin) else {
            continue;
        };
        if !inputs
            .uses
            .matches_reads_and_writes(binding, proof.reads, proof.writes)
        {
            continue;
        }
        found_guard = true;
        if proof.sparse {
            out.uninitialized.insert(binding);
        }
        out.statements.extend([
            (declaration.id, GuardedHeapStatement::Declare),
            (
                write.id,
                GuardedHeapStatement::Allocate {
                    binding,
                    count,
                    zeroed,
                },
            ),
            (
                guard.id,
                GuardedHeapStatement::Present {
                    binding,
                    elide_release: false,
                },
            ),
            (release, GuardedHeapStatement::Remove),
        ]);
        out.statements.extend(proof.actions);
        out.elements.extend(proof.elements);
    }
    if found_guard {
        let endings = release_endings(body, |statement| match out.statements.get(&statement.id) {
            Some(GuardedHeapStatement::Present { binding, .. }) => Some(*binding),
            _ => released_binding(&statement.value, |callee| builtin(callee, Known::Free)),
        });
        for (statement, ending) in body.iter().zip(endings) {
            if ending.elides_release(true)
                && let Some(GuardedHeapStatement::Present { elide_release, .. }) =
                    out.statements.get_mut(&statement.id)
            {
                *elide_release = true;
            }
        }
    }
}

fn arm(body: &[Span<Statement>]) -> &[Span<Statement>] {
    let mut statements = code(body);
    match (statements.next(), statements.next()) {
        (
            Some(Span {
                value: Statement::Block(inner),
                ..
            }),
            None,
        ) => inner,
        _ => body,
    }
}

fn code(body: &[Span<Statement>]) -> impl Iterator<Item = &Span<Statement>> + Clone {
    body.iter()
        .filter(|stmt| !matches!(stmt.value, Statement::Comment(_) | Statement::Null))
}

fn integer_pointee(ty: &Type) -> Option<&Type> {
    let Type::Pointer {
        pointee,
        access,
        space: PointerSpace::Default,
        ..
    } = ty
    else {
        return None;
    };
    (access.is_plain()
        && matches!(
            **pointee,
            Type::Numeric(NumericType::Integer {
                width: 8 | 16 | 32 | 64,
                bit_precise: false,
                ..
            })
        ))
    .then_some(pointee)
}

fn unsigned_constant(value: &Value) -> Option<u64> {
    integer_constant(value)?.try_into().ok()
}

fn extent(size: &Value, element: &Type, pointer_width: u32, multiplier: u64) -> Option<u64> {
    let size_of = |value: &Value| match &value.node.value {
        ValueKind::LayoutQuery {
            query: LayoutQuery::SizeOf { ty, .. },
            amount,
        } if ty == element => Some(*amount),
        _ => None,
    };
    let (element_size, count) = match &size.node.value {
        ValueKind::Arith {
            op: ArithOp::Mul,
            left,
            right,
            ..
        } => {
            if let Some(bytes) = size_of(left) {
                (bytes, unsigned_constant(right)?)
            } else {
                (size_of(right)?, unsigned_constant(left)?)
            }
        }
        _ => (size_of(size)?, 1),
    };
    let count = count.checked_mul(multiplier)?;
    let Type::Numeric(NumericType::Integer { width, signed, .. }) = size.ty else {
        return None;
    };
    let limit = 1_u128
        .checked_shl((pointer_width.checked_sub(1)?).min(width.checked_sub(u32::from(signed))?))?;
    (count > 0 && element_size > 0 && u128::from(element_size) * u128::from(count) < limit)
        .then_some(count)
}

pub(crate) fn peel_pointer(mut value: &Value) -> &Value {
    while let ValueKind::Copy { operand, .. }
    | ValueKind::Convert {
        kind: ConversionKind::PointerCast,
        operand,
        ..
    } = &value.node.value
        && matches!(
            (&value.ty, &operand.ty),
            (
                Type::Pointer {
                    space: PointerSpace::Default,
                    ..
                },
                Type::Pointer {
                    space: PointerSpace::Default,
                    ..
                }
            )
        )
    {
        value = operand;
    }
    value
}

fn is_null(value: &Value) -> bool {
    matches!(peel_pointer(value).node.value, ValueKind::Null)
}

fn reads_binding(value: &Value, binding: BindingId) -> bool {
    matches!(
        &value.node.value,
        ValueKind::Read { place, ordering: None }
            if plain_access(&place.access) && matches!(place.kind, PlaceKind::Binding(id) if id == binding)
    )
}

fn checks_null(value: &Value, binding: BindingId, relation: CompareOp) -> bool {
    matches!(
        &value.node.value,
        ValueKind::Compare { op, left, right, .. }
            if *op == relation && ((reads_binding(left, binding) && is_null(right))
                || (is_null(left) && reads_binding(right, binding)))
    )
}

fn write_binding(statement: &Span<Statement>, binding: BindingId) -> Option<&Value> {
    match &statement.value {
        Statement::Write {
            place,
            value,
            ordering: None,
            ..
        } if plain_access(&place.access)
            && matches!(place.kind, PlaceKind::Binding(id) if id == binding) =>
        {
            Some(value)
        }
        _ => None,
    }
}

struct Cursor<'a> {
    module: &'a Module,
    binding: BindingId,
    element: &'a Type,
    count: u64,
    offset: u64,
    prefix: u64,
    sparse: bool,
    initialized: HashSet<u64>,
    reads: usize,
    writes: usize,
    elements: HashMap<NodeId, HeapElement>,
    actions: Vec<(NodeId, GuardedHeapStatement)>,
    inputs: &'a HeapInputs<'a>,
}

impl Cursor<'_> {
    fn prove(
        &mut self,
        body: &[Span<Statement>],
        builtin: &impl Fn(&Callee, Known) -> bool,
    ) -> Option<NodeId> {
        let mut statements = code(body).peekable();
        while let Some(statement) = statements.next() {
            if let Some(value) = write_binding(statement, self.binding) {
                self.offset = self.pointer(value)?;
                self.reads += 1;
                self.writes += 1;
                self.actions
                    .push((statement.id, GuardedHeapStatement::Remove));
                continue;
            }
            match &statement.value {
                Statement::Write {
                    place,
                    value,
                    ordering: None,
                    ..
                } => {
                    self.read(value)?;
                    if let Some((pointer, index)) = self.access(place)? {
                        let append = index == self.prefix;
                        self.record_element(pointer, HeapIndex::Constant(index), append)?;
                        self.initialize(index);
                    }
                }
                Statement::Let(variable) if plain_local(variable) && !variable.byref => {
                    if let Some(count) = self.resize_count(variable, builtin) {
                        self.prove_resize(
                            statement.id,
                            variable.id,
                            count,
                            statements.next()?,
                            statements.next()?,
                            builtin,
                        )?;
                        continue;
                    }
                    if let Some(value) = &variable.initializer {
                        self.read(value)?;
                    }
                }
                Statement::For { id, body, .. } => self.fill_loop(*id, body)?,
                Statement::Expression(Value {
                    node:
                        Span {
                            value:
                                ValueKind::Call {
                                    callee, arguments, ..
                                },
                            ..
                        },
                    ..
                }) => {
                    if builtin(callee, Known::Free)
                        && let [pointer] = arguments.as_slice()
                        && statements.peek().is_none()
                        && self.pointer(peel_pointer(pointer)) == Some(0)
                    {
                        self.reads += 1;
                        return Some(statement.id);
                    }
                    if builtin(callee, Known::MemSet)
                        && let [pointer, fill, size] = arguments.as_slice()
                        && unsigned_constant(fill) == Some(0)
                        && self.pointer(peel_pointer(pointer)) == Some(0)
                        && self.extent(size) == Some(self.count)
                    {
                        self.actions.push((
                            statement.id,
                            GuardedHeapStatement::ZeroFill {
                                binding: self.binding,
                                count: self.count,
                                overwrite: self.prefix != 0,
                            },
                        ));
                        self.reads += 1;
                        self.prefix = self.count;
                        self.initialized.clear();
                        continue;
                    }
                    for argument in arguments {
                        self.read(argument)?;
                    }
                }
                _ => return None,
            }
        }
        None
    }

    fn extent(&self, value: &Value) -> Option<u64> {
        extent(value, self.element, self.module.target.pointer_width, 1)
    }

    fn initialize(&mut self, index: u64) {
        if index == self.prefix {
            self.prefix += 1;
            while self.initialized.remove(&self.prefix) {
                self.prefix += 1;
            }
        } else if index > self.prefix {
            self.sparse = true;
            self.initialized.insert(index);
        }
    }

    fn fill_loop(&mut self, id: BindingId, body: &[Span<Statement>]) -> Option<()> {
        let counted = self.inputs.loops.get(&id)?;
        let start = unsigned_constant(&counted.start)?;
        let end = unsigned_constant(&counted.end)?.checked_add(u64::from(counted.inclusive))?;
        let append = start == self.prefix;
        if (!append && end > self.prefix) || start >= end || end > self.count || self.offset != 0 {
            return None;
        }
        let mut statements = code(arm(body));
        let Statement::Write {
            place,
            value,
            ordering: None,
            ..
        } = &statements.next()?.value
        else {
            return None;
        };
        let PlaceKind::Deref(pointer) = &place.kind else {
            return None;
        };
        let ValueKind::PointerOffset {
            pointer: base,
            amount,
            subtract: false,
            element,
            ..
        } = &pointer.node.value
        else {
            return None;
        };
        if statements.next().is_some()
            || element != self.element
            || place.ty != *self.element
            || !plain_access(&place.access)
            || !reads_binding(base, self.binding)
            || !reads_binding(amount, counted.index)
            || [pointer.node.id, base.node.id, amount.node.id]
                .iter()
                .any(|id| self.module.macro_values.contains_key(id))
        {
            return None;
        }
        self.read(value)?;
        self.record_element(pointer.node.id, HeapIndex::Binding(counted.index), append)?;
        if append {
            self.prefix = end;
            while self.initialized.remove(&self.prefix) {
                self.prefix += 1;
            }
        }
        Some(())
    }

    fn resize_count(
        &self,
        result: &Variable,
        builtin: &impl Fn(&Callee, Known) -> bool,
    ) -> Option<u64> {
        let initializer = result.initializer.as_ref()?;
        let ValueKind::Call {
            callee, arguments, ..
        } = &peel_pointer(initializer).node.value
        else {
            return None;
        };
        let [pointer, size] = arguments.as_slice() else {
            return None;
        };
        let pointer = peel_pointer(pointer);
        if !builtin(callee, Known::Realloc)
            || result.ty != pointer.ty
            || !reads_binding(pointer, self.binding)
            || self.offset != 0
            || self.module.macro_values.contains_key(&initializer.node.id)
            || self.inputs.hidden.contains(&result.id)
            || !self.inputs.uses.matches_reads_and_writes(result.id, 2, 0)
        {
            return None;
        }
        let count = self.extent(size)?;
        (count > self.count).then_some(count)
    }

    fn prove_resize(
        &mut self,
        attempt: NodeId,
        result: BindingId,
        count: u64,
        failure: &Span<Statement>,
        commit: &Span<Statement>,
        builtin: &impl Fn(&Callee, Known) -> bool,
    ) -> Option<()> {
        let Statement::If {
            condition,
            then_body,
            else_body: None,
            folded: None,
        } = &failure.value
        else {
            return None;
        };
        if !checks_null(condition, result, CompareOp::Eq)
            || self.module.macro_values.contains_key(&condition.node.id)
            || !reads_binding(write_binding(commit, self.binding)?, result)
        {
            return None;
        }
        let mut statements = code(arm(then_body)).peekable();
        while let Some(Span {
            value: Statement::Let(variable),
            ..
        }) = statements.peek()
        {
            if !plain_local(variable) || variable.byref {
                return None;
            }
            let initializer = variable.initializer.as_ref()?;
            if !self.plain_failure_value(initializer, true) {
                return None;
            }
            self.read(initializer)?;
            statements.next();
        }
        let release = statements.next()?;
        let Statement::Expression(value) = &release.value else {
            return None;
        };
        let ValueKind::Call {
            callee, arguments, ..
        } = &value.node.value
        else {
            return None;
        };
        let [pointer] = arguments.as_slice() else {
            return None;
        };
        if !builtin(callee, Known::Free) || self.pointer(peel_pointer(pointer)) != Some(0) {
            return None;
        }
        let Statement::Return(value) = &statements.next()?.value else {
            return None;
        };
        if statements.next().is_some()
            || value
                .as_ref()
                .is_some_and(|value| !self.plain_failure_value(value, false))
        {
            return None;
        }
        self.actions.extend([
            (
                attempt,
                GuardedHeapStatement::Resize {
                    binding: self.binding,
                    count,
                },
            ),
            (failure.id, GuardedHeapStatement::ResizeFailed(result)),
            (release.id, GuardedHeapStatement::Remove),
            (commit.id, GuardedHeapStatement::Remove),
        ]);
        self.count = count;
        self.reads += 2;
        self.writes += 1;
        Some(())
    }

    fn pointer(&self, value: &Value) -> Option<u64> {
        if self.module.macro_values.contains_key(&value.node.id) {
            return None;
        }
        match &value.node.value {
            ValueKind::PointerOffset {
                pointer,
                amount,
                subtract,
                element,
                ..
            } if element == self.element
                && reads_binding(pointer, self.binding)
                && !self.module.macro_values.contains_key(&pointer.node.id) =>
            {
                let amount: i128 = integer_constant(amount)?.try_into().ok()?;
                let amount = if *subtract {
                    amount.checked_neg()?
                } else {
                    amount
                };
                let offset = i128::from(self.offset).checked_add(amount)?;
                (0..=i128::from(self.count))
                    .contains(&offset)
                    .then_some(offset as u64)
            }
            _ if reads_binding(value, self.binding) => Some(self.offset),
            _ => None,
        }
    }

    fn access(&self, place: &Place) -> Option<Option<(NodeId, u64)>> {
        let PlaceKind::Deref(pointer) = &place.kind else {
            return Some(None);
        };
        let Some(index) = self.pointer(pointer) else {
            return Some(None);
        };
        if !plain_access(&place.access) || place.ty != *self.element || index >= self.count {
            return None;
        }
        Some(Some((pointer.node.id, index)))
    }

    fn record_element(&mut self, pointer: NodeId, index: HeapIndex, append: bool) -> Option<()> {
        let element = HeapElement {
            binding: self.binding,
            index,
            append,
        };
        if self
            .elements
            .insert(pointer, element)
            .is_some_and(|prior| prior != element)
        {
            return None;
        }
        self.reads += 1;
        Some(())
    }

    fn read(&mut self, mut value: &Value) -> Option<()> {
        loop {
            if self.module.macro_values.contains_key(&value.node.id)
                || self.module.macro_written_casts.contains_key(&value.node.id)
            {
                return Some(());
            }
            match &value.node.value {
                ValueKind::Copy { operand, .. } | ValueKind::Convert { operand, .. } => {
                    value = operand
                }
                ValueKind::Read {
                    place,
                    ordering: None,
                } => {
                    if let Some((pointer, index)) = self.access(place)? {
                        if index >= self.prefix && !self.initialized.contains(&index) {
                            return None;
                        }
                        self.record_element(pointer, HeapIndex::Constant(index), false)?;
                    }
                    return Some(());
                }
                _ => return Some(()),
            }
        }
    }
    fn plain_failure_value(&self, mut value: &Value, allow_element: bool) -> bool {
        while matches!(value.ty, Type::Numeric(NumericType::Integer { .. })) {
            match &value.node.value {
                ValueKind::Copy { operand, .. } | ValueKind::Convert { operand, .. } => {
                    value = operand
                }
                ValueKind::Constant(_) => return true,
                ValueKind::Read {
                    place,
                    ordering: None,
                } if plain_access(&place.access) => {
                    return match place.kind {
                        PlaceKind::Binding(binding) => !self
                            .module
                            .globals
                            .iter()
                            .any(|global| global.variable.id == binding),
                        PlaceKind::Deref(_) => {
                            allow_element && matches!(self.access(place), Some(Some(_)))
                        }
                        _ => false,
                    };
                }
                _ => return false,
            }
        }
        false
    }
}
