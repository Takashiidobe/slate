use crate::counted_loops::{integer_constant, plain_access, plain_local};
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
    },
    Present {
        binding: BindingId,
        elide_release: bool,
    },
    Remove,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct HeapElement {
    pub binding: BindingId,
    pub index: u64,
}

#[derive(Debug, Default)]
pub(crate) struct GuardedHeap {
    pub statements: HashMap<NodeId, GuardedHeapStatement>,
    pub elements: HashMap<NodeId, HeapElement>,
}

pub(crate) fn analyze(
    program: &Program<'_>,
    unit: UnitId,
    function: &Function,
    identities: &HashMap<ProgramBinding, FunctionIdentity>,
    hidden: &HashSet<BindingId>,
    uses: &DefUse,
    out: &mut GuardedHeap,
) {
    let module = program.unit(unit).module;
    if !module.hosted || module.nostdinc || !uses.whole_body_structured() {
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
            || hidden.contains(&binding)
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
        let [size] = arguments.as_slice() else {
            continue;
        };
        let Some(count) = extent(size, element, module.target.pointer_width) else {
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
        if !builtin(callee, Known::Malloc) || !checks_presence(condition, binding) {
            continue;
        }
        let mut proof = Cursor {
            module,
            binding,
            element,
            count,
            offset: 0,
            initialized: HashSet::new(),
            reads: 1,
            writes: 1,
            elements: HashMap::new(),
            advances: Vec::new(),
        };
        let Some(release) = proof.prove(arm(then_body), &builtin) else {
            continue;
        };
        if !uses.matches_reads_and_writes(binding, proof.reads, proof.writes) {
            continue;
        }
        found_guard = true;
        out.statements.extend([
            (declaration.id, GuardedHeapStatement::Declare),
            (write.id, GuardedHeapStatement::Allocate { binding, count }),
            (
                guard.id,
                GuardedHeapStatement::Present {
                    binding,
                    elide_release: false,
                },
            ),
            (release, GuardedHeapStatement::Remove),
        ]);
        out.statements.extend(
            proof
                .advances
                .into_iter()
                .map(|id| (id, GuardedHeapStatement::Remove)),
        );
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

fn extent(size: &Value, element: &Type, pointer_width: u32) -> Option<u64> {
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
                (bytes, integer_constant(right)?.try_into().ok()?)
            } else {
                (size_of(right)?, integer_constant(left)?.try_into().ok()?)
            }
        }
        _ => (size_of(size)?, 1),
    };
    let Type::Numeric(NumericType::Integer { width, signed, .. }) = size.ty else {
        return None;
    };
    let limit = 1_u128
        .checked_shl((pointer_width.checked_sub(1)?).min(width.checked_sub(u32::from(signed))?))?;
    (count > 0 && element_size > 0 && u128::from(element_size) * u128::from(count) < limit)
        .then_some(count)
}

fn peel_pointer(mut value: &Value) -> &Value {
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

fn checks_presence(value: &Value, binding: BindingId) -> bool {
    matches!(
        &value.node.value,
        ValueKind::Compare { op: CompareOp::Ne, left, right, .. }
            if (reads_binding(left, binding) && is_null(right))
                || (is_null(left) && reads_binding(right, binding))
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
    initialized: HashSet<u64>,
    reads: usize,
    writes: usize,
    elements: HashMap<NodeId, HeapElement>,
    advances: Vec<NodeId>,
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
                self.advances.push(statement.id);
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
                    if let Some(index) = self.access(place)? {
                        self.initialized.insert(index);
                    }
                }
                Statement::Let(variable) if plain_local(variable) && !variable.byref => {
                    if let Some(value) = &variable.initializer {
                        self.read(value)?;
                    }
                }
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
                    for argument in arguments {
                        self.read(argument)?;
                    }
                }
                _ => return None,
            }
        }
        None
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

    fn access(&mut self, place: &Place) -> Option<Option<u64>> {
        let PlaceKind::Deref(pointer) = &place.kind else {
            return Some(None);
        };
        let Some(index) = self.pointer(pointer) else {
            return Some(None);
        };
        if !plain_access(&place.access) || place.ty != *self.element || index >= self.count {
            return None;
        }
        let element = HeapElement {
            binding: self.binding,
            index,
        };
        if self
            .elements
            .insert(pointer.node.id, element)
            .is_some_and(|prior| prior != element)
        {
            return None;
        }
        self.reads += 1;
        Some(Some(index))
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
                    let index = self.access(place)?;
                    return index
                        .is_none_or(|index| self.initialized.contains(&index))
                        .then_some(());
                }
                _ => return Some(()),
            }
        }
    }
}
