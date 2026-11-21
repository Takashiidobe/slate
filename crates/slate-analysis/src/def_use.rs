use foldhash::{HashMap, HashMapExt};
use slate_parser::ast::Span;
use slate_parser::ir::*;
use std::ops::Range;

const STACK_RED_ZONE: usize = 512 << 10;
const STACK_GROWTH: usize = 16 << 20;

pub(crate) fn deep<T>(recurse: impl FnOnce() -> T) -> T {
    stacker::maybe_grow(STACK_RED_ZONE, STACK_GROWTH, recurse)
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) enum Access {
    Read,
    Write,
    Escape,
    Capture,
}

pub(crate) struct DefUse {
    uses: HashMap<BindingId, Vec<(u32, Access)>>,
    bodies: HashMap<BindingId, Range<u32>>,
    switches: HashMap<BindingId, u32>,
    cases: Vec<(u32, BindingId)>,
    labels: Vec<u32>,
    opaque: Vec<u32>,
    ordered: bool,
    position: u32,
    next: u32,
}

impl DefUse {
    pub fn of(body: &[Span<Statement>]) -> Self {
        let mut def_use = Self {
            uses: HashMap::new(),
            bodies: HashMap::new(),
            switches: HashMap::new(),
            cases: Vec::new(),
            labels: Vec::new(),
            opaque: Vec::new(),
            ordered: true,
            position: 0,
            next: 0,
        };
        def_use.statements(body);
        def_use
    }

    pub fn escapes(&self, binding: BindingId) -> bool {
        self.uses
            .get(&binding)
            .is_some_and(|uses| uses.iter().any(|(_, access)| *access == Access::Escape))
    }

    pub fn ordered(&self) -> bool {
        self.ordered
    }

    pub fn single_read(&self, binding: BindingId) -> bool {
        matches!(
            self.uses.get(&binding).map(Vec::as_slice),
            Some([(_, Access::Read)])
        )
    }

    pub fn touched_in(&self, binding: BindingId, region: &Range<u32>) -> bool {
        self.uses.get(&binding).is_some_and(|uses| {
            uses.iter().any(|(position, access)| {
                matches!(access, Access::Write | Access::Escape) && region.contains(position)
            })
        })
    }

    pub fn body(&self, statement: BindingId) -> Option<Range<u32>> {
        self.bodies.get(&statement).cloned()
    }

    pub fn structured(&self, region: &Range<u32>) -> bool {
        !self.labels.iter().any(|label| region.contains(label))
            && !self.opaque.iter().any(|opaque| region.contains(opaque))
            && self.cases.iter().all(|(case, switch)| {
                !region.contains(case)
                    || self
                        .switches
                        .get(switch)
                        .is_some_and(|switch| region.contains(switch))
            })
    }

    fn record(&mut self, binding: BindingId, access: Access) {
        self.uses
            .entry(binding)
            .or_default()
            .push((self.position, access));
    }

    fn region(&mut self, body: &[Span<Statement>]) -> Range<u32> {
        let start = self.next;
        self.statements(body);
        start..self.next
    }

    fn statements(&mut self, body: &[Span<Statement>]) {
        for statement in body {
            deep(|| {
                let outer = self.position;
                self.position = self.next;
                self.next += 1;
                self.statement(&statement.value);
                self.position = outer;
            });
        }
    }

    fn evaluation(&mut self, evaluation: &Evaluation) {
        self.statements(&evaluation.statements);
        self.value(&evaluation.value);
    }

    fn statement(&mut self, statement: &Statement) {
        match statement {
            Statement::MacroExpansion { body, .. } | Statement::Block(body) => {
                self.statements(body)
            }
            Statement::Comment(_)
            | Statement::Null
            | Statement::Fence { .. }
            | Statement::Return(None)
            | Statement::Break(_)
            | Statement::Continue(_)
            | Statement::SehLeave(_) => {}
            Statement::Goto(_) => self.ordered = false,
            Statement::ComputedGoto(value) => {
                self.ordered = false;
                self.value(value);
            }
            Statement::Temporary {
                ty, initializer, ..
            } => {
                self.captures(ty);
                if let Some(value) = initializer {
                    self.value(value);
                }
            }
            Statement::Let(variable) => self.variable(variable),
            Statement::Write { place, value, .. } => {
                self.target(place);
                self.value(value);
            }
            Statement::Expression(value) | Statement::Return(Some(value)) => self.value(value),
            Statement::NestedFunction(nested) => {
                for capture in &nested.captures {
                    self.record(capture.id, Access::Escape);
                }
                if let Some(body) = &nested.function.body {
                    self.statements(body);
                }
            }
            Statement::If {
                condition,
                then_body,
                else_body,
                ..
            } => {
                self.value(condition);
                self.statements(then_body);
                if let Some(body) = else_body {
                    self.statements(body);
                }
            }
            Statement::While {
                id,
                condition,
                body,
            } => {
                self.evaluation(condition);
                let body = self.region(body);
                self.bodies.insert(*id, body);
            }
            Statement::DoWhile {
                id,
                body,
                condition,
            } => {
                let body = self.region(body);
                self.bodies.insert(*id, body);
                self.evaluation(condition);
            }
            Statement::For {
                id,
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
                let body = self.region(body);
                self.bodies.insert(*id, body);
            }
            Statement::Switch {
                id,
                discriminant,
                body,
                ..
            } => {
                self.switches.insert(*id, self.position);
                self.value(discriminant);
                let body = self.region(body);
                self.bodies.insert(*id, body);
            }
            Statement::Case {
                switch,
                start,
                end,
                body,
            } => {
                self.cases.push((self.position, *switch));
                self.value(start);
                if let Some(end) = end {
                    self.value(end);
                }
                self.statements(body);
            }
            Statement::Default { switch, body } => {
                self.cases.push((self.position, *switch));
                self.statements(body);
            }
            Statement::Asm(asm) => {
                self.opaque.push(self.position);
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
                            self.record(symbol.binding, Access::Escape);
                        }
                    }
                }
            }
            Statement::Label { body, .. } => {
                self.labels.push(self.position);
                self.ordered = false;
                self.statements(body);
            }
            Statement::SehTry { body, handler, .. } => {
                self.opaque.push(self.position);
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
                self.opaque.push(self.position);
                for value in [&parallel.num_threads, &parallel.condition]
                    .into_iter()
                    .flatten()
                {
                    self.value(value);
                }
                for shared in &parallel.shared {
                    self.record(shared.binding, Access::Escape);
                }
                self.privates(&parallel.private);
                self.statements(&parallel.body);
            }
            Statement::OmpFor(omp) => {
                self.opaque.push(self.position);
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
            self.record(private.original, Access::Escape);
            self.variable(&private.copy);
        }
    }

    fn variable(&mut self, variable: &Variable) {
        self.captures(&variable.ty);
        if variable.cleanup.is_some() {
            self.record(variable.id, Access::Escape);
        }
        if let Some(value) = &variable.initializer {
            self.record(variable.id, Access::Write);
            self.value(value);
        }
    }

    fn target(&mut self, place: &Place) {
        self.captures(&place.ty);
        match &place.kind {
            PlaceKind::Binding(id) => self.record(*id, Access::Write),
            PlaceKind::MacroArg { place, .. } => self.target(place),
            _ => self.place(place),
        }
    }

    fn update(&mut self, place: &Place) {
        if let PlaceKind::Binding(id) = place.without_macro_holes().kind {
            self.record(id, Access::Read);
        }
        self.target(place);
    }

    fn place(&mut self, place: &Place) {
        self.captures(&place.ty);
        match &place.kind {
            PlaceKind::Binding(id) => self.record(*id, Access::Escape),
            PlaceKind::Deref(value) => self.value(value),
            PlaceKind::MacroArg { place, .. } => self.place(place),
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
        self.captures(&value.ty);
        deep(|| self.value_kind(&value.node.value));
    }

    fn captures(&mut self, ty: &Type) {
        for binding in captured(ty) {
            self.record(binding, Access::Capture);
        }
    }

    fn value_kind(&mut self, kind: &ValueKind) {
        match kind {
            ValueKind::Read { place, .. } => match place.kind {
                PlaceKind::Binding(id) => self.record(id, Access::Read),
                _ => self.place(place),
            },
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
                self.update(place);
                self.value(computation);
            }
            ValueKind::CompareExchange {
                place,
                expected,
                desired,
                ..
            } => {
                self.update(place);
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
            | ValueKind::MacroArg { value: operand, .. }
            | ValueKind::Convert { operand, .. } => self.value(operand),
            ValueKind::StatementExpression(evaluation)
            | ValueKind::MacroExpansion { evaluation, .. } => self.evaluation(evaluation),
            ValueKind::Block(block) => {
                if let Some(body) = &block.function.body {
                    self.statements(body);
                }
            }
            ValueKind::LabelAddress(_) => self.ordered = false,
            ValueKind::Fence { .. }
            | ValueKind::OldValue
            | ValueKind::Constant(_)
            | ValueKind::LayoutQuery { .. }
            | ValueKind::Null
            | ValueKind::Seh(_)
            | ValueKind::Void
            | ValueKind::CodeUnits(_) => {}
        }
    }
}

pub(crate) fn captured(ty: &Type) -> Vec<BindingId> {
    let mut bindings = Vec::new();
    let mut pending = vec![ty];
    while let Some(ty) = pending.pop() {
        match ty {
            Type::VariableArray { element, extent } => {
                if let VariableExtent::Captured(binding) = extent {
                    bindings.push(*binding);
                }
                pending.push(element);
            }
            Type::Array { element, .. } => pending.push(element),
            Type::Pointer { pointee, .. } => pending.push(pointee),
            Type::BlockPointer { function } => pending.push(function),
            Type::Function {
                return_type,
                parameters,
                ..
            } => {
                pending.extend(return_type.as_deref());
                pending.extend(parameters);
            }
            Type::Void
            | Type::Bool
            | Type::Numeric(_)
            | Type::Complex(_)
            | Type::Imaginary(_)
            | Type::FixedPoint(_)
            | Type::Vector { .. }
            | Type::VaList
            | Type::Sizeless(_)
            | Type::Defined(_) => {}
        }
    }
    bindings
}
