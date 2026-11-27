use foldhash::HashMap;
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
    ReadProjection,
    Initialize,
    Write,
    Escape,
    Argument(BorrowArgument),
    Capture,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct BorrowArgument {
    pub callee: BindingId,
    pub index: usize,
    pub array: bool,
}

#[derive(Debug, Default, Clone, PartialEq, Eq)]
pub struct BindingMutability {
    pub required: bool,
    pub arguments: Vec<BorrowArgument>,
}

#[derive(Default)]
pub(crate) struct DefUse {
    uses: HashMap<BindingId, Vec<(u32, Access)>>,
    locals: Vec<BindingId>,
    bodies: HashMap<BindingId, Range<u32>>,
    switches: HashMap<BindingId, u32>,
    cases: Vec<(u32, BindingId)>,
    labels: Vec<u32>,
    opaque: Vec<u32>,
    next: u32,
}

impl DefUse {
    pub fn of(body: &[Span<Statement>]) -> Self {
        let mut walker = Walker::default();
        walker.statements(body);
        walker.def_use
    }

    pub fn for_mutability(
        function: &Function,
        types: &HashMap<TypeId, &TypeDefinitionKind>,
    ) -> Option<Self> {
        let mut walker = Walker {
            types: Some(types),
            ..Walker::default()
        };
        walker.statements(function.body.as_deref()?);
        let mut def_use = walker.def_use;
        if let Parameters::Prototype { fixed, .. } = &function.parameters {
            def_use
                .locals
                .extend(fixed.iter().map(|param| param.value.id));
        }
        Some(def_use)
    }

    pub fn escapes(&self, binding: BindingId) -> bool {
        self.uses.get(&binding).is_some_and(|uses| {
            uses.iter().any(|(_, access)| {
                matches!(
                    access,
                    Access::Escape | Access::Argument(_) | Access::ReadProjection
                )
            })
        })
    }

    pub fn written_once(&self, binding: BindingId) -> bool {
        let uses = self.uses.get(&binding).map(Vec::as_slice).unwrap_or(&[]);
        uses.iter()
            .filter(|(_, access)| matches!(access, Access::Write | Access::Initialize))
            .count()
            == 1
            && uses.iter().all(|(_, access)| {
                matches!(access, Access::Write | Access::Initialize | Access::Read)
            })
    }

    pub fn single_read(&self, binding: BindingId) -> bool {
        matches!(
            self.uses.get(&binding).map(Vec::as_slice),
            Some([(_, Access::Read)])
        )
    }

    pub fn mentions(&self, binding: BindingId) -> bool {
        self.uses.contains_key(&binding)
    }

    pub fn matches_reads_and_writes(
        &self,
        binding: BindingId,
        reads: usize,
        writes: usize,
    ) -> bool {
        let Some(uses) = self.uses.get(&binding) else {
            return false;
        };
        let counts =
            uses.iter()
                .try_fold(
                    (0, 0, 0),
                    |(initializers, reads, writes), (_, access)| match access {
                        Access::Initialize => Some((initializers + 1, reads, writes)),
                        Access::Read => Some((initializers, reads + 1, writes)),
                        Access::Write => Some((initializers, reads, writes + 1)),
                        _ => None,
                    },
                );
        counts == Some((1, reads, writes))
    }

    pub fn whole_body_structured(&self) -> bool {
        self.structured(&(0..self.next))
    }

    pub fn only_writes(&self, binding: BindingId, count: usize) -> bool {
        let uses = self.uses.get(&binding).map(Vec::as_slice).unwrap_or(&[]);
        uses.len() == count
            && uses
                .iter()
                .all(|(_, access)| matches!(access, Access::Write | Access::Initialize))
    }

    pub fn mutability(&self, binding: BindingId) -> BindingMutability {
        self.uses
            .get(&binding)
            .map_or_else(BindingMutability::default, |uses| Self::mutability_of(uses))
    }

    pub fn binding_mutability(&self) -> impl Iterator<Item = (BindingId, BindingMutability)> + '_ {
        self.locals.iter().map(|&id| (id, self.mutability(id)))
    }

    fn mutability_of(uses: &[(u32, Access)]) -> BindingMutability {
        if uses
            .iter()
            .any(|(_, access)| matches!(access, Access::Write | Access::Escape))
        {
            return BindingMutability {
                required: true,
                arguments: Vec::new(),
            };
        }
        BindingMutability {
            required: false,
            arguments: uses
                .iter()
                .filter_map(|(_, access)| match access {
                    Access::Argument(argument) => Some(*argument),
                    _ => None,
                })
                .collect(),
        }
    }

    pub fn straight_line_writes(
        &self,
        binding: BindingId,
        statements: usize,
        writes: usize,
    ) -> bool {
        self.next as usize == statements && self.only_writes(binding, writes)
    }

    pub fn touched_in(&self, binding: BindingId, region: &Range<u32>) -> bool {
        self.uses.get(&binding).is_some_and(|uses| {
            uses.iter().any(|(position, access)| {
                !matches!(access, Access::Read | Access::Capture) && region.contains(position)
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
}

pub fn single_read_initializers(body: &[Span<Statement>]) -> impl Iterator<Item = BindingId> {
    let uses = DefUse::of(body);
    body.iter().filter_map(move |statement| {
        let Statement::Let(variable) = &statement.value else {
            return None;
        };
        uses.matches_reads_and_writes(variable.id, 1, 0)
            .then_some(variable.id)
    })
}

#[derive(Default)]
struct Walker<'a> {
    def_use: DefUse,
    position: u32,
    types: Option<&'a HashMap<TypeId, &'a TypeDefinitionKind>>,
}

impl Walker<'_> {
    fn record(&mut self, binding: BindingId, access: Access) {
        self.def_use
            .uses
            .entry(binding)
            .or_default()
            .push((self.position, access));
    }

    fn region(&mut self, body: &[Span<Statement>]) -> Range<u32> {
        let start = self.def_use.next;
        self.statements(body);
        start..self.def_use.next
    }

    fn statements(&mut self, body: &[Span<Statement>]) {
        for statement in body {
            deep(|| {
                let outer = self.position;
                self.position = self.def_use.next;
                self.def_use.next += 1;
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
            Statement::MacroExpansion { body, .. }
            | Statement::Block(body)
            | Statement::LabeledBlock { body, .. } => self.statements(body),
            Statement::Comment(_)
            | Statement::Null
            | Statement::Fence { .. }
            | Statement::Return(None)
            | Statement::Break(_)
            | Statement::Continue(_)
            | Statement::SehLeave(_)
            | Statement::Goto(_) => {}
            Statement::ComputedGoto(value) => self.value(value),
            Statement::Temporary {
                id,
                ty,
                initializer,
                ..
            } => {
                if self.types.is_some() {
                    self.def_use.locals.push(*id);
                }
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
                    self.capture(capture.id, CaptureKind::Escape);
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
                self.def_use.bodies.insert(*id, body);
            }
            Statement::DoWhile {
                id,
                body,
                condition,
            } => {
                let body = self.region(body);
                self.def_use.bodies.insert(*id, body);
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
                self.def_use.bodies.insert(*id, body);
            }
            Statement::Switch {
                id,
                discriminant,
                body,
                ..
            } => {
                self.def_use.switches.insert(*id, self.position);
                self.value(discriminant);
                let body = self.region(body);
                self.def_use.bodies.insert(*id, body);
            }
            Statement::Case {
                switch,
                start,
                end,
                body,
            } => {
                self.def_use.cases.push((self.position, *switch));
                self.value(start);
                if let Some(end) = end {
                    self.value(end);
                }
                self.statements(body);
            }
            Statement::Default { switch, body } => {
                self.def_use.cases.push((self.position, *switch));
                self.statements(body);
            }
            Statement::Asm(asm) => {
                self.def_use.opaque.push(self.position);
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
                self.def_use.labels.push(self.position);
                self.statements(body);
            }
            Statement::SehTry { body, handler, .. } => {
                self.def_use.opaque.push(self.position);
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
                self.def_use.opaque.push(self.position);
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
                self.def_use.opaque.push(self.position);
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
        if self.types.is_some() {
            self.def_use.locals.push(variable.id);
        }
        self.captures(&variable.ty);
        if variable.cleanup.is_some() {
            self.record(variable.id, Access::Escape);
        }
        if let Some(value) = &variable.initializer {
            self.record(variable.id, Access::Initialize);
            self.value(value);
        }
    }

    fn target(&mut self, place: &Place) {
        self.captures(&place.ty);
        match &place.kind {
            PlaceKind::Binding(id) => self.record(*id, Access::Write),
            _ => self.place(place),
        }
    }

    fn update(&mut self, place: &Place) {
        if let PlaceKind::Binding(id) = place.kind {
            self.record(id, Access::Read);
        }
        self.target(place);
    }

    fn place(&mut self, place: &Place) {
        self.access_place(place, Access::Escape);
    }

    fn aligned_array(&self, mut place: &Place) -> bool {
        while let PlaceKind::Field {
            base, bits: None, ..
        } = &place.kind
        {
            let Some(types) = self.types else {
                return false;
            };
            let mut ty = &base.ty;
            while let Type::Defined(id) = ty
                && let Some(TypeDefinitionKind::Alias(inner)) = types.get(id)
            {
                ty = inner;
            }
            if !place.access.is_plain()
                || !matches!(ty, Type::Defined(id) if matches!(
                    types.get(id),
                    Some(TypeDefinitionKind::Record { layout: Some(layout), .. })
                        if !layout.underaligned.contains(&true)
                ))
            {
                return false;
            }
            place = base;
        }
        matches!(place.kind, PlaceKind::Binding(_)) && place.access.is_plain()
    }

    fn access_place(&mut self, place: &Place, access: Access) {
        self.captures(&place.ty);
        match &place.kind {
            PlaceKind::Binding(id) => self.record(*id, access),
            PlaceKind::Deref(value)
                if access == Access::ReadProjection
                    && let ValueKind::PointerOffset {
                        pointer,
                        amount,
                        subtract: false,
                        element,
                        ..
                    } = &value.node.value
                    && let ValueKind::ArrayDecay {
                        place: array,
                        length: Some(_),
                    } = &pointer.node.value
                    && let Type::Array {
                        length: Some(1..),
                        element: array_element,
                    } = &array.ty
                    && **array_element == *element
                    && self.aligned_array(array)
                    && captured(&array.ty).is_empty() =>
            {
                self.access_place(array, access);
                self.value(amount);
            }
            PlaceKind::Deref(value) => self.value(value),
            PlaceKind::CompoundLiteral { initializer, .. }
            | PlaceKind::Temporary { initializer, .. } => self.value(initializer),
            PlaceKind::ComplexPart { base, .. }
            | PlaceKind::Field { base, .. }
            | PlaceKind::Swizzle { base, .. } => self.access_place(base, access),
            PlaceKind::Lane { base, index } => {
                self.access_place(base, access);
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

    fn argument(&mut self, argument: &Value, callee: BindingId, index: usize) {
        let mut value = argument;
        while let ValueKind::Copy { operand, .. }
        | ValueKind::Convert {
            kind: ConversionKind::PointerCast,
            operand,
            ..
        } = &value.node.value
            && matches!((&operand.ty, &value.ty), (Type::Pointer { pointee: from, .. }, Type::Pointer { pointee: to, .. }) if from == to)
        {
            self.captures(&value.ty);
            value = operand;
        }
        let (place, array) = match &value.node.value {
            ValueKind::AddressOf(place) => (place, false),
            ValueKind::ArrayDecay {
                place,
                length: Some(_),
            } if matches!(
                place.ty,
                Type::Array {
                    length: Some(1..),
                    ..
                }
            ) && captured(&place.ty).is_empty() =>
            {
                (place, true)
            }
            _ => {
                self.value(value);
                return;
            }
        };
        if matches!(place.kind, PlaceKind::Binding(_)) {
            self.captures(&value.ty);
            self.access_place(
                place,
                Access::Argument(BorrowArgument {
                    callee,
                    index,
                    array,
                }),
            );
        } else {
            self.value(value);
        }
    }

    fn capture(&mut self, binding: BindingId, kind: CaptureKind) {
        self.record(
            binding,
            match kind {
                CaptureKind::Escape => Access::Escape,
                CaptureKind::Read => Access::Capture,
            },
        );
    }

    fn captures(&mut self, ty: &Type) {
        for binding in captured(ty) {
            self.record(binding, Access::Capture);
        }
    }

    fn value_kind(&mut self, kind: &ValueKind) {
        match kind {
            ValueKind::Read { place, ordering } => match place.kind {
                PlaceKind::Binding(id) => self.record(id, Access::Read),
                _ if ordering.is_none() && place.access.is_plain() => {
                    self.access_place(place, Access::ReadProjection);
                }
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
                for (index, argument) in arguments.iter().enumerate() {
                    match callee {
                        Callee::Direct(callee) => self.argument(argument, *callee, index),
                        Callee::Indirect(_) => self.value(argument),
                    }
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
            ValueKind::Block(block) => {
                for capture in &block.captures {
                    self.capture(capture.id, capture.kind());
                }
            }
            ValueKind::LabelAddress(_)
            | ValueKind::Fence { .. }
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
