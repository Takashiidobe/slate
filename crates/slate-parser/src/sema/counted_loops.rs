use crate::ast::Span;
use crate::ir::*;
use num_bigint::BigInt;
use std::collections::{HashMap, HashSet};

#[derive(Default)]
struct Uses {
    writes: HashSet<BindingId>,
    escapes: HashSet<BindingId>,
    switches: HashSet<BindingId>,
    cases: HashSet<BindingId>,
    labels: bool,
    opaque: bool,
}

impl Uses {
    fn of(body: &[Span<Statement>]) -> Self {
        let mut uses = Self::default();
        uses.statements(body);
        uses
    }

    fn touches(&self, id: BindingId) -> bool {
        self.writes.contains(&id) || self.escapes.contains(&id)
    }

    fn statements(&mut self, body: &[Span<Statement>]) {
        for statement in body {
            super::deep(|| self.statement(&statement.value));
        }
    }

    fn evaluation(&mut self, evaluation: &Evaluation) {
        self.statements(&evaluation.statements);
        self.value(&evaluation.value);
    }

    fn statement(&mut self, statement: &Statement) {
        match statement {
            Statement::Comment(_)
            | Statement::Null
            | Statement::Fence { .. }
            | Statement::Return(None)
            | Statement::Break(_)
            | Statement::Continue(_)
            | Statement::Goto(_)
            | Statement::SehLeave(_) => {}
            Statement::Temporary { initializer, .. } => {
                if let Some(value) = initializer {
                    self.value(value);
                }
            }
            Statement::Let(variable) => self.variable(variable),
            Statement::Write { place, value, .. } => {
                self.target(place);
                self.value(value);
            }
            Statement::Expression(value)
            | Statement::Return(Some(value))
            | Statement::ComputedGoto(value) => self.value(value),
            Statement::Block(body) => self.statements(body),
            Statement::If {
                condition,
                then_body,
                else_body,
            } => {
                self.value(condition);
                self.statements(then_body);
                if let Some(body) = else_body {
                    self.statements(body);
                }
            }
            Statement::While {
                condition, body, ..
            }
            | Statement::DoWhile {
                body, condition, ..
            } => {
                self.evaluation(condition);
                self.statements(body);
            }
            Statement::For {
                init,
                condition,
                increment,
                body,
                ..
            } => {
                self.statements(init);
                if let Some(condition) = condition {
                    self.evaluation(condition);
                }
                if let Some(increment) = increment {
                    self.evaluation(increment);
                }
                self.statements(body);
            }
            Statement::Switch {
                id,
                discriminant,
                body,
            } => {
                self.switches.insert(*id);
                self.value(discriminant);
                self.statements(body);
            }
            Statement::Case {
                switch,
                start,
                end,
                body,
            } => {
                self.cases.insert(*switch);
                self.value(start);
                if let Some(end) = end {
                    self.value(end);
                }
                self.statements(body);
            }
            Statement::Default { switch, body } => {
                self.cases.insert(*switch);
                self.statements(body);
            }
            Statement::Asm(asm) => {
                self.opaque = true;
                for operand in &asm.operands {
                    match &operand.kind {
                        AsmOperandKind::In(value) => self.value(value),
                        AsmOperandKind::InOut { place, input, .. } => {
                            self.place(place);
                            if let Some(input) = input {
                                self.value(&input.value);
                            }
                        }
                        AsmOperandKind::InPlace(place)
                        | AsmOperandKind::Out { place, .. }
                        | AsmOperandKind::Memory { place, .. } => self.place(place),
                        AsmOperandKind::Symbol(symbol) => {
                            self.escapes.insert(symbol.binding);
                        }
                    }
                }
            }
            Statement::Label { body, .. } => {
                self.labels = true;
                self.statements(body);
            }
            Statement::SehTry { body, handler, .. } => {
                self.opaque = true;
                self.statements(body);
                match handler {
                    SehHandler::Except { filter, body } => {
                        self.evaluation(filter);
                        self.statements(body);
                    }
                    SehHandler::Finally(body) => self.statements(body),
                }
            }
            Statement::OmpParallel(parallel) => {
                self.opaque = true;
                for value in [&parallel.num_threads, &parallel.condition]
                    .into_iter()
                    .flatten()
                {
                    self.value(value);
                }
                for shared in &parallel.shared {
                    self.escapes.insert(shared.binding);
                }
                self.privates(&parallel.private);
                self.statements(&parallel.body);
            }
            Statement::OmpFor(omp) => {
                self.opaque = true;
                self.privates(&omp.private);
                for omp_loop in &omp.loops {
                    self.variable(&omp_loop.variable);
                    self.value(&omp_loop.lower);
                    self.value(&omp_loop.upper);
                    self.value(&omp_loop.step);
                }
                self.statements(&omp.body);
            }
        }
    }

    fn privates(&mut self, privates: &[OmpPrivate]) {
        for private in privates {
            self.escapes.insert(private.original);
            self.variable(&private.copy);
        }
    }

    fn variable(&mut self, variable: &Variable) {
        if variable.cleanup.is_some() {
            self.escapes.insert(variable.id);
        }
        if let Some(value) = &variable.initializer {
            self.value(value);
        }
    }

    fn target(&mut self, place: &Place) {
        match &place.kind {
            PlaceKind::Binding(id) => {
                self.writes.insert(*id);
            }
            _ => self.place(place),
        }
    }

    fn place(&mut self, place: &Place) {
        match &place.kind {
            PlaceKind::Binding(id) => {
                self.escapes.insert(*id);
            }
            PlaceKind::Deref(value) => self.value(value),
            PlaceKind::CompoundLiteral { initializer, .. }
            | PlaceKind::Temporary { initializer, .. } => self.value(initializer),
            PlaceKind::ComplexPart { base, .. }
            | PlaceKind::Field { base, .. }
            | PlaceKind::Swizzle { base, .. } => self.place(base),
            PlaceKind::Lane { base, index } => {
                self.place(base);
                self.value(index);
            }
            PlaceKind::Index { base, index } => {
                self.value(base);
                self.value(index);
            }
        }
    }

    fn value(&mut self, value: &Value) {
        super::deep(|| self.value_kind(&value.node.value));
    }

    fn value_kind(&mut self, kind: &ValueKind) {
        match kind {
            ValueKind::Read { place, .. } => {
                if !matches!(place.kind, PlaceKind::Binding(_)) {
                    self.place(place);
                }
            }
            ValueKind::AddressOf(place)
            | ValueKind::ArrayDecay { place, .. }
            | ValueKind::FunctionDecay { place }
            | ValueKind::VaArg { list: place, .. }
            | ValueKind::VaStart { list: place, .. }
            | ValueKind::VaEnd { list: place, .. } => self.place(place),
            ValueKind::Store { place, value, .. } => {
                self.target(place);
                self.value(value);
            }
            ValueKind::Update {
                place, computation, ..
            } => {
                self.target(place);
                self.value(computation);
            }
            ValueKind::CompareExchange {
                place,
                expected,
                desired,
                ..
            } => {
                self.target(place);
                self.value(expected);
                self.value(desired);
            }
            ValueKind::Overflow {
                left,
                right,
                result,
                ..
            } => {
                self.value(left);
                self.value(right);
                self.target(result);
            }
            ValueKind::VaCopy {
                destination,
                source,
                ..
            } => {
                self.place(destination);
                self.place(source);
            }
            ValueKind::Call {
                callee, arguments, ..
            } => {
                if let Callee::Indirect(value) = callee {
                    self.value(value);
                }
                for argument in arguments {
                    self.value(argument);
                }
            }
            ValueKind::Arith { left, right, .. }
            | ValueKind::Compare { left, right, .. }
            | ValueKind::Logical { left, right, .. }
            | ValueKind::PointerDifference { left, right, .. }
            | ValueKind::Sequence { left, right }
            | ValueKind::PointerOffset {
                pointer: left,
                amount: right,
                ..
            }
            | ValueKind::Lane {
                vector: left,
                index: right,
            } => {
                self.value(left);
                self.value(right);
            }
            ValueKind::Conditional {
                condition,
                then_value,
                else_value,
            } => {
                self.value(condition);
                self.value(then_value);
                self.value(else_value);
            }
            ValueKind::Aggregate { members, .. } => {
                for member in members {
                    self.value(&member.value);
                }
            }
            ValueKind::Shuffle { left, right, mask } => {
                self.value(left);
                if let Some(right) = right {
                    self.value(right);
                }
                if let ShuffleMask::Dynamic(mask) = mask {
                    self.value(mask);
                }
            }
            ValueKind::Intrinsic { arguments, .. } => {
                for argument in arguments {
                    self.value(argument);
                }
            }
            ValueKind::Capture { extent, value, .. } => {
                self.value(extent);
                self.value(value);
            }
            ValueKind::Copy { operand, .. }
            | ValueKind::Unary { operand, .. }
            | ValueKind::FloatClass { operand, .. }
            | ValueKind::Convert { operand, .. } => self.value(operand),
            ValueKind::StatementExpression(evaluation) => self.evaluation(evaluation),
            ValueKind::Fence { .. }
            | ValueKind::OldValue
            | ValueKind::Constant(_)
            | ValueKind::Null
            | ValueKind::LabelAddress(_)
            | ValueKind::Seh(_)
            | ValueKind::Void
            | ValueKind::CodeUnits(_) => {}
        }
    }
}

struct Scope {
    plain: HashSet<BindingId>,
}

pub(super) fn annotate(module: &mut Module) {
    for function in &mut module.functions {
        let function = &mut function.value;
        let Some(body) = &mut function.body else {
            continue;
        };
        let whole = Uses::of(body);
        let mut plain = HashSet::new();
        if let Parameters::Prototype { fixed, .. } = &function.parameters {
            plain.extend(
                fixed
                    .iter()
                    .filter(|parameter| plain_access(&parameter.value.access))
                    .map(|parameter| parameter.value.id),
            );
        }
        collect_locals(body, &mut plain);
        plain.retain(|id| !whole.escapes.contains(id));
        mark(body, &Scope { plain });
    }
}

fn plain_access(access: &Access) -> bool {
    !access.volatile && !access.atomic
}

fn collect_locals(body: &[Span<Statement>], plain: &mut HashSet<BindingId>) {
    for statement in body {
        super::deep(|| match &statement.value {
            Statement::Let(variable) => {
                if variable.storage == StorageDuration::Automatic
                    && plain_access(&variable.access)
                    && variable.register.is_none()
                    && variable.cleanup.is_none()
                {
                    plain.insert(variable.id);
                }
            }
            Statement::Block(body)
            | Statement::While { body, .. }
            | Statement::DoWhile { body, .. }
            | Statement::Switch { body, .. }
            | Statement::Case { body, .. }
            | Statement::Default { body, .. }
            | Statement::Label { body, .. } => collect_locals(body, plain),
            Statement::If {
                then_body,
                else_body,
                ..
            } => {
                collect_locals(then_body, plain);
                if let Some(body) = else_body {
                    collect_locals(body, plain);
                }
            }
            Statement::For { init, body, .. } => {
                collect_locals(init, plain);
                collect_locals(body, plain);
            }
            _ => {}
        });
    }
}

fn mark(body: &mut [Span<Statement>], scope: &Scope) {
    for statement in body {
        super::deep(|| match &mut statement.value {
            Statement::Block(body)
            | Statement::While { body, .. }
            | Statement::DoWhile { body, .. }
            | Statement::Switch { body, .. }
            | Statement::Case { body, .. }
            | Statement::Default { body, .. }
            | Statement::Label { body, .. } => mark(body, scope),
            Statement::If {
                then_body,
                else_body,
                ..
            } => {
                mark(then_body, scope);
                if let Some(body) = else_body {
                    mark(body, scope);
                }
            }
            Statement::For {
                init,
                condition,
                increment,
                body,
                counted,
                ..
            } => {
                mark(init, scope);
                mark(body, scope);
                *counted = recognize(init, condition.as_ref(), increment.as_ref(), body, scope)
                    .map(Box::new);
            }
            _ => {}
        });
    }
}

fn recognize(
    init: &[Span<Statement>],
    condition: Option<&Evaluation>,
    increment: Option<&Evaluation>,
    body: &[Span<Statement>],
    scope: &Scope,
) -> Option<CountedLoop> {
    let Statement::Let(index) = &init.last()?.value else {
        return None;
    };
    let Type::Numeric(NumericType::Integer { .. }) = index.ty else {
        return None;
    };
    if !scope.plain.contains(&index.id) {
        return None;
    }
    let start = index.initializer.clone()?;
    let condition = condition?;
    if !condition.statements.is_empty() {
        return None;
    }
    let ValueKind::Compare {
        op, left, right, ..
    } = &condition.value.node.value
    else {
        return None;
    };
    let (end, inclusive) = match op {
        CompareOp::Lt if reads(left, index.id) => (right, false),
        CompareOp::Gt if reads(right, index.id) => (left, false),
        CompareOp::Le if reads(left, index.id) => (right, true),
        CompareOp::Ge if reads(right, index.id) => (left, true),
        _ => return None,
    };
    if end.ty != index.ty {
        return None;
    }
    if inclusive && !below_max(end, &index.ty) {
        return None;
    }
    let mut end_reads = HashSet::new();
    if !invariant(end, &mut end_reads)
        || end_reads.contains(&index.id)
        || !end_reads.iter().all(|id| scope.plain.contains(id))
    {
        return None;
    }
    if !unit_increment(increment?, index.id) {
        return None;
    }
    let uses = Uses::of(body);
    if uses.labels
        || uses.opaque
        || !uses.cases.is_subset(&uses.switches)
        || uses.touches(index.id)
        || end_reads.iter().any(|id| uses.touches(*id))
    {
        return None;
    }
    Some(CountedLoop {
        index: index.id,
        start,
        end: (**end).clone(),
        inclusive,
    })
}

fn reads(value: &Value, id: BindingId) -> bool {
    matches!(
        &value.node.value,
        ValueKind::Read { place, ordering: None }
            if matches!(place.kind, PlaceKind::Binding(read) if read == id)
    )
}

fn invariant(value: &Value, reads: &mut HashSet<BindingId>) -> bool {
    match &value.node.value {
        ValueKind::Constant(_) => true,
        ValueKind::Copy { operand, .. }
        | ValueKind::Convert { operand, .. }
        | ValueKind::Unary { operand, .. } => invariant(operand, reads),
        ValueKind::Arith { left, right, .. } => invariant(left, reads) && invariant(right, reads),
        ValueKind::Read {
            place:
                Place {
                    kind: PlaceKind::Binding(id),
                    access,
                    ..
                },
            ordering: None,
        } if plain_access(access) => {
            reads.insert(*id);
            true
        }
        _ => false,
    }
}

fn unit_increment(increment: &Evaluation, index: BindingId) -> bool {
    if !matches!(increment.value.node.value, ValueKind::Void) {
        return false;
    }
    let mut temporaries: HashMap<BindingId, &Value> = HashMap::new();
    let mut written = None;
    for statement in &increment.statements {
        match &statement.value {
            Statement::Temporary {
                id,
                initializer: Some(value),
                ..
            } => {
                temporaries.insert(*id, resolve(value, &temporaries));
            }
            Statement::Write {
                place:
                    Place {
                        kind: PlaceKind::Binding(id),
                        ..
                    },
                value,
                ordering: None,
                ..
            } if *id == index && written.is_none() => {
                written = Some(resolve(value, &temporaries));
            }
            _ => return false,
        }
    }
    let Some(ValueKind::Arith {
        op: ArithOp::Add,
        left,
        right,
        ..
    }) = written.map(|value| &value.node.value)
    else {
        return false;
    };
    reads(resolve(left, &temporaries), index)
        && integer_constant(right).is_some_and(|one| one == BigInt::from(1))
}

fn resolve<'a>(value: &'a Value, temporaries: &HashMap<BindingId, &'a Value>) -> &'a Value {
    match &value.node.value {
        ValueKind::Read {
            place:
                Place {
                    kind: PlaceKind::Binding(id),
                    ..
                },
            ..
        } => temporaries.get(id).copied().unwrap_or(value),
        _ => value,
    }
}

fn integer_range(ty: &Type) -> Option<(BigInt, BigInt)> {
    let Type::Numeric(NumericType::Integer { width, signed, .. }) = ty else {
        return None;
    };
    Some(if *signed {
        let half = BigInt::from(1) << (width - 1);
        (-half.clone(), half - 1)
    } else {
        (BigInt::from(0), (BigInt::from(1) << width) - 1)
    })
}

fn integer_constant(value: &Value) -> Option<BigInt> {
    let constant = match &value.node.value {
        ValueKind::Constant(Number::Integer(value)) => BigInt::from(value.clone()),
        ValueKind::Constant(Number::SignedInteger(value)) => value.clone(),
        ValueKind::Copy { operand, .. } | ValueKind::Convert { operand, .. } => {
            integer_constant(operand)?
        }
        _ => return None,
    };
    let (min, max) = integer_range(&value.ty)?;
    (min <= constant && constant <= max).then_some(constant)
}

fn below_max(end: &Value, ty: &Type) -> bool {
    match (integer_constant(end), integer_range(ty)) {
        (Some(end), Some((_, max))) => end < max,
        _ => false,
    }
}
