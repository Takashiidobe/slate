use crate::def_use::{DefUse, captured, deep};
use foldhash::{HashSet, HashSetExt};
use slate_parser::ast::Span;
use slate_parser::ir::*;

const MAX_DISTANCE: usize = 32;

pub(crate) fn record_captures(module: &Module) -> HashSet<BindingId> {
    let mut captures = HashSet::new();
    for definition in &module.types {
        if let TypeDefinitionKind::Record {
            fields: Some(fields),
            ..
        } = &definition.value.kind
        {
            for field in fields {
                captures.extend(captured(&field.ty));
            }
        }
    }
    captures
}

pub(crate) fn analyze(
    function: &Function,
    record_captures: &HashSet<BindingId>,
    inlined: &mut HashSet<BindingId>,
) {
    let Some(body) = &function.body else {
        return;
    };
    let def_use = DefUse::of(body);
    let mut locals = HashSet::new();
    if let Parameters::Prototype { fixed, .. } = &function.parameters {
        locals.extend(
            fixed
                .iter()
                .filter(|parameter| plain(&parameter.value.access))
                .map(|parameter| parameter.value.id),
        );
    }
    collect_locals(body, &mut locals);
    locals.retain(|id| !def_use.escapes(*id));
    let context = Context {
        def_use: &def_use,
        locals: &locals,
        record_captures,
    };
    context.statements(body, inlined);
}

fn plain(access: &Access) -> bool {
    !access.volatile && !access.atomic
}

fn collect_locals(body: &[Span<Statement>], locals: &mut HashSet<BindingId>) {
    for statement in body {
        deep(|| match &statement.value {
            Statement::Let(variable) => {
                if variable.storage == StorageDuration::Automatic && plain(&variable.access) {
                    locals.insert(variable.id);
                }
            }
            Statement::Temporary { id, .. } => {
                locals.insert(*id);
            }
            #[expect(
                slate::sensitive_wildcard,
                reason = "every other statement only nests lists, which children() lists"
            )]
            other => {
                for list in children(other) {
                    collect_locals(list, locals);
                }
                for evaluation in evaluations(other) {
                    collect_locals(&evaluation.statements, locals);
                }
            }
        });
    }
}

fn children(statement: &Statement) -> Vec<&[Span<Statement>]> {
    match statement {
        Statement::Block(body)
        | Statement::While { body, .. }
        | Statement::DoWhile { body, .. }
        | Statement::Switch { body, .. }
        | Statement::Case { body, .. }
        | Statement::Default { body, .. }
        | Statement::Label { body, .. } => vec![body],
        Statement::If {
            then_body,
            else_body,
            ..
        } => {
            let mut lists = vec![then_body.as_slice()];
            if let Some(body) = else_body {
                lists.push(body);
            }
            lists
        }
        Statement::For { init, body, .. } => vec![init, body],
        Statement::SehTry { body, handler, .. } => match handler {
            SehHandler::Except { body: handler, .. } | SehHandler::Finally(handler) => {
                vec![body, handler]
            }
        },
        Statement::MacroExpansion { .. }
        | Statement::NestedFunction(_)
        | Statement::OmpParallel(_)
        | Statement::OmpFor(_)
        | Statement::Comment(_)
        | Statement::Null
        | Statement::Fence { .. }
        | Statement::Temporary { .. }
        | Statement::Let(_)
        | Statement::Write { .. }
        | Statement::Expression(_)
        | Statement::Return(_)
        | Statement::Break(_)
        | Statement::Continue(_)
        | Statement::Goto(_)
        | Statement::ComputedGoto(_)
        | Statement::Asm(_)
        | Statement::SehLeave(_) => Vec::new(),
    }
}

fn evaluations(statement: &Statement) -> Vec<&Evaluation> {
    match statement {
        Statement::While { condition, .. } | Statement::DoWhile { condition, .. } => {
            vec![condition]
        }
        Statement::For {
            condition,
            increment,
            ..
        } => condition.iter().chain(increment.iter()).collect(),
        Statement::SehTry {
            handler: SehHandler::Except { filter, .. },
            ..
        } => vec![filter],
        #[expect(
            slate::sensitive_wildcard,
            reason = "only loops and SEH filters carry evaluations with hoisted statements"
        )]
        _ => Vec::new(),
    }
}

#[derive(Default)]
struct Summary {
    effects: bool,
    memory: bool,
    reads: HashSet<BindingId>,
    writes: HashSet<BindingId>,
}

impl Summary {
    fn effectful(&self) -> bool {
        self.effects || !self.writes.is_empty()
    }

    fn conflicts(&self, other: &Summary) -> bool {
        if self.effectful() {
            other.effectful()
                || other.memory
                || !self.writes.is_disjoint(&other.reads)
                || !self.reads.is_disjoint(&other.writes)
        } else {
            !self.reads.is_disjoint(&other.writes) || (self.memory && other.effects)
        }
    }
}

struct Context<'a> {
    def_use: &'a DefUse,
    locals: &'a HashSet<BindingId>,
    record_captures: &'a HashSet<BindingId>,
}

impl Context<'_> {
    fn statements(&self, body: &[Span<Statement>], inlined: &mut HashSet<BindingId>) {
        self.list(body, None, inlined);
        for statement in body {
            deep(|| {
                for list in children(&statement.value) {
                    self.statements(list, inlined);
                }
                for evaluation in evaluations(&statement.value) {
                    self.list(&evaluation.statements, Some(&evaluation.value), inlined);
                    self.statements_nested(&evaluation.statements, inlined);
                }
            });
        }
    }

    fn statements_nested(&self, body: &[Span<Statement>], inlined: &mut HashSet<BindingId>) {
        for statement in body {
            deep(|| {
                for list in children(&statement.value) {
                    self.statements(list, inlined);
                }
            });
        }
    }

    fn list(
        &self,
        body: &[Span<Statement>],
        tail: Option<&Value>,
        inlined: &mut HashSet<BindingId>,
    ) {
        for (index, statement) in body.iter().enumerate() {
            let Statement::Temporary {
                id,
                ty,
                initializer: Some(initializer),
                unsequenced: false,
            } = &statement.value
            else {
                continue;
            };
            if initializer.ty != *ty
                || !self.def_use.single_read(*id)
                || self.record_captures.contains(id)
            {
                continue;
            }
            let mut producer = Scan::new(self, None);
            producer.value(initializer);
            if producer.found || producer.rejected {
                continue;
            }
            if self.movable(*id, &producer.summary, &body[index + 1..], tail) {
                inlined.insert(*id);
            }
        }
    }

    fn movable(
        &self,
        temporary: BindingId,
        producer: &Summary,
        following: &[Span<Statement>],
        tail: Option<&Value>,
    ) -> bool {
        let mut between = Scan::new(self, Some(temporary));
        for (distance, statement) in following.iter().enumerate() {
            if distance >= MAX_DISTANCE {
                return false;
            }
            let mut own = Scan::new(self, Some(temporary));
            own.own_values(&statement.value);
            if own.found {
                return !own.rejected
                    && !(own.conditional && producer.effectful())
                    && !producer.conflicts(&between.summary)
                    && !producer.conflicts(&own.summary);
            }
            between.statement(&statement.value);
            if between.found || between.rejected {
                return false;
            }
        }
        let Some(tail) = tail else {
            return false;
        };
        let mut own = Scan::new(self, Some(temporary));
        own.value(tail);
        own.found
            && !own.rejected
            && !(own.conditional && producer.effectful())
            && !producer.conflicts(&between.summary)
            && !producer.conflicts(&own.summary)
    }
}

struct Scan<'c, 'a> {
    context: &'c Context<'a>,
    temporary: Option<BindingId>,
    summary: Summary,
    found: bool,
    rejected: bool,
    conditional: bool,
    conditional_depth: u32,
    nested_depth: u32,
}

impl<'c, 'a> Scan<'c, 'a> {
    fn new(context: &'c Context<'a>, temporary: Option<BindingId>) -> Self {
        Self {
            context,
            temporary,
            summary: Summary::default(),
            found: false,
            rejected: false,
            conditional: false,
            conditional_depth: 0,
            nested_depth: 0,
        }
    }

    fn own_values(&mut self, statement: &Statement) {
        match statement {
            Statement::Expression(value) | Statement::Return(Some(value)) => {
                self.value(value);
            }
            Statement::If { condition, .. } => {
                self.value(condition);
            }
            Statement::Switch { discriminant, .. } => {
                self.value(discriminant);
            }
            Statement::Write {
                place,
                value,
                ordering,
                ..
            } => self.assign(place, value, ordering.is_some()),
            Statement::Let(variable) => {
                if variable.cleanup.is_some() {
                    self.summary.effects = true;
                }
                if let Some(value) = &variable.initializer {
                    self.value(value);
                }
            }
            Statement::Temporary {
                initializer: Some(value),
                ..
            } => {
                self.value(value);
            }
            #[expect(
                slate::sensitive_wildcard,
                reason = "every other statement has no value evaluated once before its nested statements"
            )]
            _ => {}
        }
    }

    fn nested<T>(&mut self, scan: impl FnOnce(&mut Self) -> T) -> T {
        self.nested_depth += 1;
        let result = scan(self);
        self.nested_depth -= 1;
        result
    }

    fn conditionally<T>(&mut self, scan: impl FnOnce(&mut Self) -> T) -> T {
        self.conditional_depth += 1;
        let result = scan(self);
        self.conditional_depth -= 1;
        result
    }

    fn statements(&mut self, body: &[Span<Statement>]) {
        for statement in body {
            deep(|| self.statement(&statement.value));
        }
    }

    fn evaluation(&mut self, evaluation: &Evaluation) {
        self.statements(&evaluation.statements);
        self.value(&evaluation.value);
    }

    fn statement(&mut self, statement: &Statement) {
        match statement {
            Statement::Comment(_) | Statement::Null => {}
            Statement::Expression(_)
            | Statement::Write { .. }
            | Statement::Let(_)
            | Statement::Temporary { .. } => self.own_values(statement),
            Statement::Return(value) => {
                self.summary.effects = true;
                if let Some(value) = value {
                    self.value(value);
                }
            }
            Statement::Block(body) => self.statements(body),
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
            Statement::MacroExpansion { body, .. } => self.nested(|scan| scan.statements(body)),
            Statement::Fence { .. }
            | Statement::Break(_)
            | Statement::Continue(_)
            | Statement::Goto(_)
            | Statement::SehLeave(_)
            | Statement::Case { .. }
            | Statement::Default { .. }
            | Statement::Label { .. }
            | Statement::ComputedGoto(_)
            | Statement::Asm(_)
            | Statement::SehTry { .. }
            | Statement::OmpParallel(_)
            | Statement::OmpFor(_)
            | Statement::NestedFunction(_) => self.rejected = true,
        }
    }

    fn effect_unless_ancestor(&mut self, found_before: bool) {
        if found_before || !self.found {
            self.summary.effects = true;
        }
    }

    fn assign(&mut self, place: &Place, value: &Value, atomic: bool) {
        let before = self.found;
        self.value(value);
        if self.found && !before {
            self.address(place);
        } else {
            if atomic {
                self.summary.effects = true;
            }
            self.target(place);
        }
    }

    fn local(&self, binding: BindingId) -> bool {
        self.context.locals.contains(&binding)
    }

    fn target(&mut self, place: &Place) {
        match &place.without_macro_holes().kind {
            PlaceKind::Binding(binding) if self.local(*binding) && plain(&place.access) => {
                self.summary.writes.insert(*binding);
            }
            #[expect(slate::sensitive_wildcard, reason = "any other place may alias memory")]
            _ => {
                self.summary.effects = true;
                self.address(place);
            }
        }
    }

    fn update(&mut self, place: &Place) {
        if let PlaceKind::Binding(binding) = place.without_macro_holes().kind
            && self.local(binding)
            && plain(&place.access)
        {
            self.summary.reads.insert(binding);
        }
        self.target(place);
    }

    fn read(&mut self, place: &Place) {
        if !plain(&place.access) {
            self.summary.effects = true;
        }
        match &place.kind {
            PlaceKind::Binding(binding) if self.temporary == Some(*binding) => {
                self.found = true;
                if self.conditional_depth > 0 {
                    self.conditional = true;
                }
                if self.nested_depth > 0 {
                    self.rejected = true;
                }
            }
            PlaceKind::Binding(binding) if self.local(*binding) => {
                self.summary.reads.insert(*binding);
            }
            PlaceKind::Binding(_) => self.summary.memory = true,
            PlaceKind::MacroArg { place, .. } => self.nested(|scan| scan.read(place)),
            PlaceKind::Field { base, .. }
            | PlaceKind::ComplexPart { base, .. }
            | PlaceKind::Swizzle { base, .. } => self.read(base),
            PlaceKind::Lane { base, index } => {
                self.read(base);
                self.value(index);
            }
            PlaceKind::Deref(_)
            | PlaceKind::Index { .. }
            | PlaceKind::CompoundLiteral { .. }
            | PlaceKind::Temporary { .. } => {
                self.summary.memory = true;
                self.address(place);
            }
        }
    }

    fn address(&mut self, place: &Place) {
        match &place.kind {
            PlaceKind::Binding(binding) => {
                if self.temporary == Some(*binding) {
                    self.found = true;
                    self.rejected = true;
                }
            }
            PlaceKind::Deref(value) => self.value(value),
            PlaceKind::MacroArg { place, .. } => self.nested(|scan| scan.address(place)),
            PlaceKind::CompoundLiteral { initializer, .. }
            | PlaceKind::Temporary { initializer, .. } => self.value(initializer),
            PlaceKind::ComplexPart { base, .. }
            | PlaceKind::Field { base, .. }
            | PlaceKind::Swizzle { base, .. } => self.address(base),
            PlaceKind::Lane { base, index } => {
                self.address(base);
                self.value(index);
            }
            PlaceKind::Index { base, index } => {
                self.value(base);
                self.value(index);
            }
        }
    }

    fn value(&mut self, value: &Value) {
        deep(|| self.value_kind(&value.node.value));
    }

    fn value_kind(&mut self, kind: &ValueKind) {
        match kind {
            ValueKind::Read { place, ordering } => {
                if ordering.is_some() {
                    self.summary.effects = true;
                }
                self.read(place);
            }
            ValueKind::AddressOf(place)
            | ValueKind::ArrayDecay { place, .. }
            | ValueKind::FunctionDecay { place } => self.address(place),
            ValueKind::VaArg { list: place, .. }
            | ValueKind::VaStart { list: place, .. }
            | ValueKind::VaEnd { list: place, .. } => {
                self.summary.effects = true;
                self.address(place);
            }
            ValueKind::VaCopy {
                destination,
                source,
                ..
            } => {
                self.summary.effects = true;
                self.address(destination);
                self.address(source);
            }
            ValueKind::Store {
                place,
                value,
                ordering,
                ..
            } => self.assign(place, value, ordering.is_some()),
            ValueKind::Update {
                place,
                computation,
                ordering,
                ..
            } => {
                if ordering.is_some() {
                    self.summary.effects = true;
                }
                self.update(place);
                self.value(computation);
            }
            ValueKind::CompareExchange {
                place,
                expected,
                desired,
                ..
            } => {
                let before = self.found;
                self.address(place);
                self.value(expected);
                self.value(desired);
                self.effect_unless_ancestor(before);
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
            ValueKind::Call {
                callee, arguments, ..
            } => {
                let before = self.found;
                if let Callee::Indirect(value) = callee {
                    self.value(value);
                }
                for argument in arguments {
                    self.value(argument);
                }
                self.effect_unless_ancestor(before);
            }
            ValueKind::Intrinsic { arguments, .. } => {
                let before = self.found;
                for argument in arguments {
                    self.value(argument);
                }
                self.effect_unless_ancestor(before);
            }
            ValueKind::Logical { left, right, .. } => {
                self.value(left);
                self.conditionally(|scan| scan.value(right));
            }
            ValueKind::Conditional {
                condition,
                then_value,
                else_value,
            } => {
                self.value(condition);
                self.conditionally(|scan| {
                    scan.value(then_value);
                    scan.value(else_value);
                });
            }
            ValueKind::Arith { left, right, .. }
            | ValueKind::Compare { left, right, .. }
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
            ValueKind::Capture { extent, value, .. } => {
                self.value(extent);
                self.value(value);
            }
            ValueKind::Copy { operand, .. }
            | ValueKind::Unary { operand, .. }
            | ValueKind::FloatClass { operand, .. }
            | ValueKind::Convert { operand, .. } => self.value(operand),
            ValueKind::MacroArg { value: operand, .. } => {
                self.nested(|scan| scan.value(operand));
            }
            ValueKind::StatementExpression(evaluation)
            | ValueKind::MacroExpansion { evaluation, .. } => {
                self.nested(|scan| scan.evaluation(evaluation));
            }
            ValueKind::Block(block) => {
                self.summary.effects = true;
                if let Some(body) = &block.function.body {
                    self.nested(|scan| scan.statements(body));
                }
            }
            ValueKind::Fence { .. } | ValueKind::Seh(_) => self.summary.effects = true,
            ValueKind::OldValue
            | ValueKind::Constant(_)
            | ValueKind::LayoutQuery { .. }
            | ValueKind::Null
            | ValueKind::LabelAddress(_)
            | ValueKind::Void
            | ValueKind::CodeUnits(_) => {}
        }
    }
}
