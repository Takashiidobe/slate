use super::*;
use crate::ast::{MacroExpansionId, Span};

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum CaptureKind {
    Escape,
    Read,
}

pub trait Visitor {
    fn keep(&self, _expansion: MacroExpansionId) -> bool {
        true
    }
    fn statement(&mut self, _statement: &Statement) {}
    fn value_kind(&mut self, _kind: &ValueKind) {}
    fn place_kind(&mut self, _kind: &PlaceKind) {}
    fn enter_expansion(&mut self, _expansion: MacroExpansionId) {}
    fn leave_expansion(&mut self, _expansion: MacroExpansionId) {}
    fn enter_value_hole(&mut self, _expansion: MacroExpansionId) {}
    fn leave_value_hole(&mut self, _expansion: MacroExpansionId) {}
    fn enter_place_hole(&mut self, _expansion: MacroExpansionId, _place: &Place) {}
    fn leave_place_hole(&mut self, _expansion: MacroExpansionId) {}
    fn binding(&mut self, _binding: BindingId) {}
    fn capture(&mut self, _binding: BindingId, _kind: CaptureKind) {}
    fn macro_value(&mut self, _wrap: MacroWrap, _value: &Value) {}
    fn rewrite_place(&mut self, _place: &mut Place) {}
    fn rewrite_value(&mut self, _value: &mut Value) {}
    fn rewrite_statement(&mut self, _statement: &mut Statement) {}
    fn rewrite_statement_list(&mut self, _body: &mut Vec<Span<Statement>>) {}
}

struct Walker<'a, V> {
    visitor: &'a mut V,
    macro_values: Option<&'a MacroValues>,
    inner_functions: InnerFunctions,
}

#[derive(Clone, Copy, PartialEq, Eq)]
enum InnerFunctions {
    Descend,
    Capture,
}

impl<'a, V: Visitor> Walker<'a, V> {
    fn new(
        visitor: &'a mut V,
        macro_values: Option<&'a MacroValues>,
        inner_functions: InnerFunctions,
    ) -> Self {
        Self {
            visitor,
            macro_values,
            inner_functions,
        }
    }
}

pub fn walk_body<V: Visitor>(body: &mut Vec<Span<Statement>>, visitor: &mut V) {
    Walker::new(visitor, None, InnerFunctions::Descend).statements(body);
}

pub fn walk_value<V: Visitor>(value: &mut Value, visitor: &mut V) {
    Walker::new(visitor, None, InnerFunctions::Descend).value(value);
}

pub fn walk_body_with_macros<V: Visitor>(
    body: &mut Vec<Span<Statement>>,
    macro_values: &MacroValues,
    visitor: &mut V,
) {
    Walker::new(visitor, Some(macro_values), InnerFunctions::Descend).statements(body);
}

pub fn walk_value_with_macros<V: Visitor>(
    value: &mut Value,
    macro_values: &MacroValues,
    visitor: &mut V,
) {
    Walker::new(visitor, Some(macro_values), InnerFunctions::Descend).value(value);
}

pub fn walk_function_body<V: Visitor>(body: &mut Vec<Span<Statement>>, visitor: &mut V) {
    Walker::new(visitor, None, InnerFunctions::Capture).statements(body);
}

pub fn walk_function_value<V: Visitor>(value: &mut Value, visitor: &mut V) {
    Walker::new(visitor, None, InnerFunctions::Capture).value(value);
}

pub fn walk_function_body_with_macros<V: Visitor>(
    body: &mut Vec<Span<Statement>>,
    macro_values: &MacroValues,
    visitor: &mut V,
) {
    Walker::new(visitor, Some(macro_values), InnerFunctions::Capture).statements(body);
}

pub fn walk_function_value_with_macros<V: Visitor>(
    value: &mut Value,
    macro_values: &MacroValues,
    visitor: &mut V,
) {
    Walker::new(visitor, Some(macro_values), InnerFunctions::Capture).value(value);
}

pub fn for_each_inner_function(
    body: &mut Vec<Span<Statement>>,
    each: &mut impl FnMut(&mut Function),
) {
    struct Inner<'a, F>(&'a mut F);
    impl<F: FnMut(&mut Function)> Visitor for Inner<'_, F> {
        fn rewrite_statement(&mut self, statement: &mut Statement) {
            if let Statement::NestedFunction(nested) = statement {
                (self.0)(&mut nested.function);
            }
        }

        fn rewrite_value(&mut self, value: &mut Value) {
            if let ValueKind::Block(block) = &mut value.node.value {
                (self.0)(&mut block.function);
            }
        }
    }
    walk_function_body(body, &mut Inner(each));
}

impl<V: Visitor> Walker<'_, V> {
    fn statements(&mut self, body: &mut Vec<Span<Statement>>) {
        let mut position = 0;
        while position < body.len() {
            self.statement(&mut body[position].value);
            match &mut body[position].value {
                Statement::MacroExpansion {
                    expansion,
                    body: inner,
                } if !self.visitor.keep(*expansion) => {
                    let inner = std::mem::take(inner);
                    let count = inner.len();
                    body.splice(position..=position, inner);
                    position += count;
                }
                _ => position += 1,
            }
        }
        self.visitor.rewrite_statement_list(body);
    }

    fn evaluation(&mut self, evaluation: &mut Evaluation) {
        self.statements(&mut evaluation.statements);
        self.value(&mut evaluation.value);
    }

    fn variable(&mut self, variable: &mut Variable) {
        if let Some(initializer) = &mut variable.initializer {
            self.value(initializer);
        }
    }

    fn privates(&mut self, privates: &mut [OmpPrivate]) {
        for private in privates {
            self.variable(&mut private.copy);
        }
    }

    fn statement(&mut self, statement: &mut Statement) {
        self.visitor.statement(statement);
        match statement {
            Statement::Comment(_)
            | Statement::Null
            | Statement::Return(None)
            | Statement::Break(_)
            | Statement::Continue(_)
            | Statement::Goto(_)
            | Statement::SehLeave(_) => {}
            Statement::Fence { ordering, .. } => self.atomicity(ordering),
            Statement::MacroExpansion { expansion, body } => {
                self.visitor.enter_expansion(*expansion);
                self.statements(body);
                self.visitor.leave_expansion(*expansion);
            }
            Statement::Block(body) | Statement::LabeledBlock { body, .. } => self.statements(body),
            Statement::NestedFunction(nested) => match self.inner_functions {
                InnerFunctions::Descend => {
                    if let Some(body) = &mut nested.function.body {
                        self.statements(body);
                    }
                }
                InnerFunctions::Capture => {
                    for capture in &nested.captures {
                        self.visitor.capture(capture.id, CaptureKind::Escape);
                    }
                }
            },
            Statement::Temporary { initializer, .. } => {
                if let Some(value) = initializer {
                    self.value(value);
                }
            }
            Statement::Let(variable) => self.variable(variable),
            Statement::Write {
                place,
                value,
                ordering,
                ..
            } => {
                self.place(place);
                self.value(value);
                self.ordering(ordering);
            }
            Statement::Expression(value)
            | Statement::Return(Some(value))
            | Statement::ComputedGoto(value) => self.value(value),
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
            Statement::Default { body, .. } | Statement::Label { body, .. } => {
                self.statements(body);
            }
            Statement::Asm(asm) => {
                for operand in &mut asm.operands {
                    match &mut operand.kind {
                        AsmOperandKind::In(value) => self.value(value),
                        AsmOperandKind::InOut { place, input, .. } => {
                            self.place(place);
                            if let Some(input) = input {
                                self.value(&mut input.value);
                            }
                        }
                        AsmOperandKind::InPlace(place)
                        | AsmOperandKind::Out { place, .. }
                        | AsmOperandKind::Memory { place, .. } => self.place(place),
                        AsmOperandKind::Symbol(_) => {}
                    }
                }
            }
            Statement::SehTry { body, handler, .. } => {
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
                for value in [&mut parallel.num_threads, &mut parallel.condition]
                    .into_iter()
                    .flatten()
                {
                    self.value(value);
                }
                self.privates(&mut parallel.private);
                self.statements(&mut parallel.body);
            }
            Statement::OmpFor(omp) => {
                if let Some(chunk) = omp.schedule.as_mut().and_then(|s| s.chunk.as_mut()) {
                    self.value(chunk);
                }
                self.privates(&mut omp.private);
                for omp_loop in &mut omp.loops {
                    self.variable(&mut omp_loop.variable);
                    self.value(&mut omp_loop.lower);
                    self.value(&mut omp_loop.upper);
                    self.value(&mut omp_loop.step);
                }
                self.statements(&mut omp.body);
            }
        }
        self.visitor.rewrite_statement(statement);
    }

    fn ordering(&mut self, ordering: &mut Option<Atomicity>) {
        if let Some(ordering) = ordering {
            self.atomicity(ordering);
        }
    }

    fn atomicity(&mut self, ordering: &mut Atomicity) {
        self.memory_order(&mut ordering.order);
        self.sync_scope(&mut ordering.scope);
    }

    fn memory_order(&mut self, order: &mut MemoryOrder) {
        if let MemoryOrder::Dynamic(value) = order {
            self.value(value);
        }
    }

    fn sync_scope(&mut self, scope: &mut SyncScope) {
        if let SyncScope::Dynamic(value) = scope {
            self.value(value);
        }
    }

    fn place(&mut self, place: &mut Place) {
        let wraps = self
            .macro_values
            .and_then(|table| table.get(&place.id))
            .map_or(&[][..], Vec::as_slice);
        for wrap in wraps.iter().rev() {
            match *wrap {
                MacroWrap::Value(expansion) => self.visitor.enter_expansion(expansion),
                MacroWrap::Argument { expansion, .. } => {
                    self.visitor.enter_place_hole(expansion, place);
                }
            }
        }
        crate::sema::deep(|| self.place_kind(place));
        for wrap in wraps {
            match *wrap {
                MacroWrap::Value(expansion) => self.visitor.leave_expansion(expansion),
                MacroWrap::Argument { expansion, .. } => self.visitor.leave_place_hole(expansion),
            }
        }
    }

    fn place_kind(&mut self, place: &mut Place) {
        self.visitor.place_kind(&place.kind);
        match &mut place.kind {
            PlaceKind::Binding(binding) => self.visitor.binding(*binding),
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
        self.visitor.rewrite_place(place);
    }

    fn value(&mut self, value: &mut Value) {
        let wraps = self
            .macro_values
            .and_then(|table| table.get(&value.node.id))
            .map_or(&[][..], Vec::as_slice);
        for wrap in wraps.iter().rev() {
            self.visitor.macro_value(*wrap, value);
            match *wrap {
                MacroWrap::Value(expansion) => self.visitor.enter_expansion(expansion),
                MacroWrap::Argument { expansion, .. } => self.visitor.enter_value_hole(expansion),
            }
        }
        crate::sema::deep(|| self.value_kind(value));
        for wrap in wraps {
            match *wrap {
                MacroWrap::Value(expansion) => self.visitor.leave_expansion(expansion),
                MacroWrap::Argument { expansion, .. } => self.visitor.leave_value_hole(expansion),
            }
        }
    }

    fn value_kind(&mut self, whole: &mut Value) {
        self.visitor.value_kind(&whole.node.value);
        match &mut whole.node.value {
            ValueKind::Read { place, ordering } => {
                self.place(place);
                self.ordering(ordering);
            }
            ValueKind::AddressOf(place)
            | ValueKind::ArrayDecay { place, .. }
            | ValueKind::FunctionDecay { place }
            | ValueKind::VaArg { list: place, .. }
            | ValueKind::VaStart { list: place, .. }
            | ValueKind::VaEnd { list: place, .. } => self.place(place),
            ValueKind::Store {
                place,
                value,
                ordering,
            } => {
                self.place(place);
                self.value(value);
                self.ordering(ordering);
            }
            ValueKind::Update {
                place,
                computation,
                ordering,
                ..
            } => {
                self.place(place);
                self.value(computation);
                self.ordering(ordering);
            }
            ValueKind::CompareExchange {
                place,
                expected,
                desired,
                success,
                failure,
                sync_scope,
                ..
            } => {
                self.place(place);
                self.value(expected);
                self.value(desired);
                self.memory_order(success);
                self.memory_order(failure);
                self.sync_scope(sync_scope);
            }
            ValueKind::Overflow {
                left,
                right,
                result,
                ..
            } => {
                self.value(left);
                self.value(right);
                self.place(result);
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
                    self.value(&mut member.value);
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
            ValueKind::Block(block) => match self.inner_functions {
                InnerFunctions::Descend => {
                    if let Some(body) = &mut block.function.body {
                        self.statements(body);
                    }
                }
                InnerFunctions::Capture => {
                    for capture in &block.captures {
                        self.visitor.capture(capture.id, capture.kind());
                    }
                }
            },
            ValueKind::Fence { ordering, .. } => self.atomicity(ordering),
            ValueKind::OldValue
            | ValueKind::Constant(_)
            | ValueKind::LayoutQuery { .. }
            | ValueKind::Null
            | ValueKind::LabelAddress(_)
            | ValueKind::Seh(_)
            | ValueKind::Void
            | ValueKind::CodeUnits(_) => {}
        }
        self.visitor.rewrite_value(whole);
    }
}
