use super::heap::Allocation;
use super::{
    Named, NodeKind, Reason, STACK_GROWTH, STACK_RED_ZONE, Use, WRITE, Walker, place_bindings,
    tracked_pointer,
};
use foldhash::HashMap;
use slate_parser::ast::Span;
use slate_parser::ir::{
    self, BindingId, Callee, MacroWrap, Place, PlaceKind, SehHandler, Statement, StorageDuration,
    Type, Value, ValueKind, Visitor,
};

impl Walker<'_, '_> {
    pub(super) fn value(&mut self, value: &Value) {
        let wraps = self.macro_values.get(&value.node.id);
        let expansions = wraps.map_or(0, |wraps| {
            wraps
                .iter()
                .filter(|wrap| matches!(wrap, MacroWrap::Value(_)))
                .count() as u32
        });
        self.macro_depth += expansions;
        stacker::maybe_grow(STACK_RED_ZONE, STACK_GROWTH, || self.value_kind(value));
        self.macro_depth -= expansions;
        if wraps.is_some_and(|wraps| {
            wraps
                .iter()
                .any(|wrap| matches!(wrap, MacroWrap::Argument { .. }))
        }) {
            let mut named = Named(Vec::new());
            ir::walk_value(&mut value.clone(), &mut named);
            self.macro_mentions(named);
        }
    }

    fn value_kind(&mut self, value: &Value) {
        match &value.node.value {
            ValueKind::Constant(_)
            | ValueKind::LayoutQuery { .. }
            | ValueKind::Null
            | ValueKind::Seh(_)
            | ValueKind::Void
            | ValueKind::CodeUnits(_)
            | ValueKind::Fence { .. }
            | ValueKind::OldValue => {}
            ValueKind::LabelAddress(_) => self.ownership.jumps = true,
            ValueKind::Copy { operand, .. } => self.value(operand),
            ValueKind::Aggregate { members, .. } => {
                for member in members {
                    self.value(&member.value);
                    self.leak(&member.value, 0, Reason::Aggregate);
                }
            }
            ValueKind::ArrayDecay { place, .. } | ValueKind::AddressOf(place) => {
                if let PlaceKind::Binding(function) = place.kind
                    && let Some(id) = self.program.callee(self.unit, &Callee::Direct(function))
                {
                    self.address_taken.push(id);
                }
                let mut base = place;
                while let PlaceKind::Field { base: inner, .. }
                | PlaceKind::ComplexPart { base: inner, .. } = &base.kind
                {
                    base = inner;
                }
                if !std::ptr::eq(base, place)
                    && let PlaceKind::Deref(pointer) = &base.kind
                {
                    let roots = self.sources(pointer).roots;
                    self.projected
                        .extend(roots.into_iter().map(|(node, _)| node));
                }
                self.place(place, Use::Address)
            }
            ValueKind::FunctionDecay { place } => {
                if let PlaceKind::Binding(function) = place.kind
                    && let Some(id) = self.program.callee(self.unit, &Callee::Direct(function))
                {
                    self.address_taken.push(id);
                }
            }
            ValueKind::Store {
                place,
                value,
                ordering,
            } => {
                self.value(value);
                self.place(place, Use::Write);
                if ordering.is_some() || place.access.atomic {
                    self.atomic(place);
                }
                self.store(place, value);
            }
            ValueKind::Update {
                place,
                computation,
                ordering,
                ..
            } => {
                self.value(computation);
                self.place(place, Use::Write);
                if ordering.is_some() || place.access.atomic {
                    self.atomic(place);
                }
                self.rebase(place);
            }
            ValueKind::CompareExchange {
                place,
                expected,
                desired,
                ..
            } => {
                self.value(expected);
                self.value(desired);
                self.place(place, Use::Write);
                self.atomic(place);
                self.leak(expected, 0, Reason::Atomic);
                self.leak(desired, 0, Reason::Atomic);
            }
            ValueKind::PointerOffset {
                pointer, amount, ..
            } => {
                self.value(pointer);
                self.value(amount);
                self.offset(pointer);
            }
            ValueKind::PointerDifference { left, right, .. } => {
                self.value(left);
                self.value(right);
                self.offset(left);
                self.offset(right);
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
            ValueKind::Sequence { left, right } => {
                self.value(left);
                self.value(right);
            }
            ValueKind::Call {
                callee, arguments, ..
            } => self.call(value, callee, arguments),
            ValueKind::Read { place, ordering } => {
                self.place(place, Use::Read);
                if ordering.is_some() || place.access.atomic {
                    self.atomic(place);
                }
            }
            ValueKind::VaArg { list, .. }
            | ValueKind::VaStart { list, .. }
            | ValueKind::VaEnd { list, .. } => self.place(list, Use::Write),
            ValueKind::VaCopy {
                destination,
                source,
                ..
            } => {
                self.place(destination, Use::Write);
                self.place(source, Use::Read);
            }
            ValueKind::StatementExpression(evaluation) => self.evaluation(evaluation),
            ValueKind::Block(literal) => {
                for capture in &literal.captures {
                    if let Some(node) = self.node(capture.id) {
                        self.escape(node, WRITE, Reason::BlockCapture);
                    }
                }
            }
            ValueKind::Capture { extent, value, .. } => {
                self.value(extent);
                self.value(value);
            }
            ValueKind::Convert { operand, .. } => {
                self.value(operand);
                if tracked_pointer(&operand.ty).is_some()
                    && tracked_pointer(&value.ty).is_none()
                    && value.ty != Type::Bool
                {
                    self.leak(operand, 0, Reason::PointerToInteger);
                }
            }
            ValueKind::Arith { left, right, .. } | ValueKind::Logical { left, right, .. } => {
                self.value(left);
                self.value(right);
            }
            ValueKind::Overflow {
                left,
                right,
                result,
                ..
            } => {
                self.value(left);
                self.value(right);
                self.place(result, Use::Write);
            }
            ValueKind::Unary { operand, .. } | ValueKind::FloatClass { operand, .. } => {
                self.value(operand)
            }
            ValueKind::Compare { left, right, .. } => {
                self.value(left);
                self.value(right);
            }
            ValueKind::Lane { vector, index } => {
                self.value(vector);
                self.value(index);
            }
            ValueKind::Shuffle { left, right, .. } => {
                self.value(left);
                if let Some(right) = right {
                    self.value(right);
                }
            }
            ValueKind::Intrinsic { arguments, .. } => {
                for argument in arguments {
                    self.value(argument);
                    self.leak(argument, 0, Reason::Intrinsic);
                }
            }
        }
    }

    fn escape_all(&mut self, statement: &Span<Statement>) {
        self.opaque = true;
        let mut escaper = Escaper {
            index: &self.index,
            hits: Vec::new(),
        };
        ir::walk_body(&mut vec![statement.clone()], &mut escaper);
        for node in escaper.hits {
            self.escape(node, WRITE, Reason::OpenMp);
        }
        let mut named = Named(Vec::new());
        ir::walk_body(&mut vec![statement.clone()], &mut named);
        self.leaked.extend(named.0.iter().copied());
        self.written.extend(named.0);
    }

    fn asm(&mut self, asm: &ir::InlineAsm) {
        let executes = !asm.template.trim().is_empty();
        self.opaque |= executes;
        for operand in &asm.operands {
            match &operand.kind {
                ir::AsmOperandKind::In(value) => self.asm_input(value, executes),
                ir::AsmOperandKind::InPlace(place) => self.asm_place(place, Use::Read),
                ir::AsmOperandKind::Out { place, .. } => self.asm_place(place, Use::Write),
                ir::AsmOperandKind::InOut { place, input, .. } => {
                    match input {
                        Some(tied) => self.asm_input(&tied.value, executes),
                        None => self.asm_place(place, Use::Read),
                    }
                    self.asm_place(place, Use::Write);
                }
                ir::AsmOperandKind::Memory { place, access } => {
                    if matches!(access, ir::AsmAccess::Read | ir::AsmAccess::ReadWrite) {
                        self.asm_place(place, Use::Read);
                    }
                    if matches!(access, ir::AsmAccess::Write | ir::AsmAccess::ReadWrite) {
                        self.asm_place(place, Use::Write);
                    }
                }
                ir::AsmOperandKind::Symbol(_) => {}
            }
        }
    }

    fn asm_input(&mut self, value: &Value, executes: bool) {
        self.value(value);
        if executes && tracked_pointer(&value.ty).is_some() {
            self.leak(value, WRITE, Reason::AsmOperand);
        }
    }

    fn asm_place(&mut self, place: &Place, access: Use) {
        self.place(place, access);
        let mut named = Named(Vec::new());
        place_bindings(place, &mut named);
        self.leaked.extend(named.0);
        if tracked_pointer(&place.ty).is_none() {
            return;
        }
        if let PlaceKind::Binding(binding) = place.kind
            && let Some(node) = self.node(binding)
        {
            match access {
                Use::Read => self.escape(node, WRITE, Reason::AsmOperand),
                Use::Write | Use::Address => self.make_raw(node, Reason::AsmOutput),
            }
        }
    }

    pub(super) fn statement(&mut self, statement: &Span<Statement>) {
        match &statement.value {
            Statement::Comment(_)
            | Statement::Null
            | Statement::Fence { .. }
            | Statement::Return(None)
            | Statement::Break(_)
            | Statement::Continue(_)
            | Statement::SehLeave(_) => {}
            Statement::Goto(_) => self.ownership.jumps = true,
            Statement::Temporary {
                id,
                ty,
                initializer,
                ..
            } => {
                let node = self.register(*id, ty, NodeKind::Temporary);
                if let Some(initializer) = initializer {
                    self.value(initializer);
                    match node {
                        Some(node) => self.assign(node, initializer),
                        None => self.leak(initializer, 0, Reason::InitializeUntracked),
                    }
                }
            }
            Statement::Let(variable) => {
                let node = if matches!(variable.storage, StorageDuration::Automatic)
                    && !variable.name.is_empty()
                {
                    self.register(variable.id, &variable.ty, NodeKind::Local)
                } else {
                    None
                };
                if let Some(node) = node
                    && (variable.cleanup.is_some() || variable.byref)
                {
                    self.escape(node, WRITE, Reason::CleanupOrByref);
                }
                if let Some(handler) = variable.cleanup
                    && let Some(id) = self.program.callee(self.unit, &Callee::Direct(handler))
                {
                    self.address_taken.push(id);
                }
                if let Some(node) = node
                    && variable.initializer.is_none()
                {
                    self.make_raw(node, Reason::Uninitialized);
                }
                if matches!(variable.storage, StorageDuration::Automatic)
                    && !variable.byref
                    && variable.register.is_none()
                {
                    self.automatic.insert(variable.id);
                }
                if let Some(initializer) = &variable.initializer {
                    self.value(initializer);
                    if matches!(variable.storage, StorageDuration::Automatic)
                        && let Some(allocation) = self.allocation(&variable.ty, initializer)
                    {
                        self.allocations.insert(variable.id, allocation);
                    }
                    if node.is_some() && !variable.access.volatile && self.fresh(initializer) {
                        self.ownership.allocated.insert(
                            variable.id,
                            Allocation {
                                depth: self.ownership.depth,
                                scope: self.ownership.scope,
                            },
                        );
                    }
                    match node {
                        Some(node) => self.assign(node, initializer),
                        None => self.leak(initializer, 0, Reason::InitializeUntracked),
                    }
                }
            }
            Statement::Write {
                place,
                value,
                ordering,
                ..
            } => {
                self.value(value);
                self.place(place, Use::Write);
                if let PlaceKind::Binding(binding) = place.kind
                    && ordering.is_none()
                    && !place.access.volatile
                    && self.reallocates(binding, value)
                {
                    *self.ownership.supported_writes.entry(binding).or_default() += 1;
                    self.ownership.direct_resizes.insert(binding);
                }
                self.store(place, value);
            }
            Statement::Expression(value) => self.value(value),
            Statement::ComputedGoto(value) => {
                self.ownership.jumps = true;
                self.value(value);
            }
            Statement::Return(Some(value)) => {
                self.value(value);
                match self.returned {
                    Some(node) => self.flow_return(node, value),
                    None => self.leak(value, 0, Reason::ReturnUntracked),
                }
            }
            Statement::Block(body)
            | Statement::LabeledBlock { body, .. }
            | Statement::Default { body, .. } => self.statements(body),
            Statement::Label { body, .. } => {
                self.ownership.jumps = true;
                self.statements(body);
            }
            Statement::MacroExpansion { body, .. } => {
                self.macro_depth += 1;
                self.statements(body);
                self.macro_depth -= 1;
            }
            Statement::If {
                condition,
                then_body,
                else_body,
                ..
            } => {
                self.value(condition);
                let then_release = else_body
                    .as_ref()
                    .and_then(|_| self.terminal_release(then_body));
                self.statements(then_body);
                let then_release = self.ownership.releases.last().and_then(|release| {
                    (Some(release.call) == then_release)
                        .then_some((release.binding, release.position))
                });
                if let Some(else_body) = else_body {
                    let else_release = self.terminal_release(else_body);
                    self.statements(else_body);
                    if let Some((binding, position)) = then_release
                        && let Some(release) = self.ownership.releases.last()
                        && Some(release.call) == else_release
                        && release.binding == binding
                    {
                        self.ownership
                            .exclusive_releases
                            .insert(position, release.position);
                    }
                }
            }
            Statement::While {
                condition, body, ..
            }
            | Statement::DoWhile {
                condition, body, ..
            } => self.looped(|walker| {
                walker.evaluation(condition);
                walker.statements(body);
            }),
            Statement::For {
                init,
                condition,
                increment,
                body,
                ..
            } => {
                self.statements(init);
                self.looped(|walker| {
                    if let Some(condition) = condition {
                        walker.evaluation(condition);
                    }
                    if let Some(increment) = increment {
                        walker.evaluation(increment);
                    }
                    walker.statements(body);
                });
            }
            Statement::Switch {
                discriminant, body, ..
            } => {
                self.value(discriminant);
                self.statements(body);
            }
            Statement::Case {
                start, end, body, ..
            } => {
                self.value(start);
                if let Some(end) = end {
                    self.value(end);
                }
                self.statements(body);
            }
            Statement::SehTry { body, handler, .. } => {
                self.ownership.jumps = true;
                self.statements(body);
                match handler {
                    SehHandler::Except { filter, body } => {
                        self.evaluation(filter);
                        self.statements(body);
                    }
                    SehHandler::Finally(body) => self.statements(body),
                }
            }
            Statement::Asm(asm) => self.asm(asm),
            Statement::OmpParallel(_) | Statement::OmpFor(_) => self.escape_all(statement),
            Statement::NestedFunction(nested) => {
                for capture in &nested.captures {
                    if let Some(node) = self.node(capture.id) {
                        self.escape(node, WRITE, Reason::NestedCapture);
                    }
                    self.leaked.insert(capture.id);
                    self.written.insert(capture.id);
                }
            }
        }
    }
}
struct Escaper<'a> {
    index: &'a HashMap<BindingId, u32>,
    hits: Vec<u32>,
}

impl Visitor for Escaper<'_> {
    fn binding(&mut self, binding: BindingId) {
        self.hits.extend(self.index.get(&binding).copied());
    }
}
