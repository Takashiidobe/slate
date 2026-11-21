use crate::def_use::{DefUse, captured, deep};
use foldhash::{HashSet, HashSetExt};
use slate_parser::ast::Span;
use slate_parser::ir::*;

const MAX_DISTANCE: usize = 32;

pub(crate) fn transform(module: &mut Module) {
    let captures = record_captures(module);
    let Module {
        functions,
        macro_values,
        ..
    } = module;
    for function in functions {
        rewrite(&mut function.value, &captures, macro_values);
    }
}

pub(super) fn record_captures(module: &Module) -> HashSet<BindingId> {
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

fn rewrite(
    function: &mut Function,
    record_captures: &HashSet<BindingId>,
    macro_values: &MacroValues,
) {
    let Some(body) = &mut function.body else {
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
        macro_values,
    };
    context.statements(body);
}

fn plain(access: &Access) -> bool {
    !access.volatile && !access.atomic
}

fn collect_locals(body: &mut [Span<Statement>], locals: &mut HashSet<BindingId>) {
    for statement in body {
        deep(|| match &mut statement.value {
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
                    collect_locals(&mut evaluation.statements, locals);
                }
            }
        });
    }
}

pub(super) fn children(statement: &mut Statement) -> Vec<&mut Vec<Span<Statement>>> {
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
            let mut lists = vec![then_body];
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

pub(super) fn evaluations(statement: &mut Statement) -> Vec<&mut Evaluation> {
    match statement {
        Statement::While { condition, .. } | Statement::DoWhile { condition, .. } => {
            vec![condition]
        }
        Statement::For {
            condition,
            increment,
            ..
        } => condition.iter_mut().chain(increment.iter_mut()).collect(),
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
    macro_values: &'a MacroValues,
}

enum Site {
    Following(usize),
    Tail,
}

impl Context<'_> {
    fn statements(&self, body: &mut Vec<Span<Statement>>) {
        self.list(body, None);
        for statement in body {
            deep(|| {
                for list in children(&mut statement.value) {
                    self.statements(list);
                }
                for evaluation in evaluations(&mut statement.value) {
                    self.list(&mut evaluation.statements, Some(&mut evaluation.value));
                    self.statements_nested(&mut evaluation.statements);
                }
            });
        }
    }

    fn statements_nested(&self, body: &mut [Span<Statement>]) {
        for statement in body {
            deep(|| {
                for list in children(&mut statement.value) {
                    self.statements(list);
                }
            });
        }
    }

    fn list(&self, body: &mut Vec<Span<Statement>>, mut tail: Option<&mut Value>) {
        let mut index = 0;
        while index < body.len() {
            let Some(site) = self.site(&body[index].value, &body[index + 1..], tail.as_deref())
            else {
                index += 1;
                continue;
            };
            let (head, following) = body.split_at_mut(index + 1);
            let Statement::Temporary {
                id, initializer, ..
            } = &mut head[index].value
            else {
                index += 1;
                continue;
            };
            let values = match site {
                Site::Following(distance) => own_values(&mut following[distance].value),
                Site::Tail => tail.as_deref_mut().into_iter().collect(),
            };
            *initializer = substitute(values, *id, initializer.take());
            if initializer.is_some() {
                index += 1;
            } else {
                body.remove(index);
            }
        }
    }

    fn site(
        &self,
        statement: &Statement,
        following: &[Span<Statement>],
        tail: Option<&Value>,
    ) -> Option<Site> {
        let Statement::Temporary {
            id,
            ty,
            initializer: Some(initializer),
            unsequenced: false,
        } = statement
        else {
            return None;
        };
        if initializer.ty != *ty
            || !self.def_use.single_read(*id)
            || self.record_captures.contains(id)
        {
            return None;
        }
        let mut producer = Scan::new(self, None);
        producer.value(initializer);
        if producer.found || producer.rejected {
            return None;
        }
        self.movable(*id, &producer.summary, following, tail)
    }

    fn movable(
        &self,
        temporary: BindingId,
        producer: &Summary,
        following: &[Span<Statement>],
        tail: Option<&Value>,
    ) -> Option<Site> {
        let mut between = Scan::new(self, Some(temporary));
        for (distance, statement) in following.iter().enumerate() {
            if distance >= MAX_DISTANCE {
                return None;
            }
            let mut own = Scan::new(self, Some(temporary));
            own.own_values(&statement.value);
            if own.found {
                return own
                    .accepts(producer, &between.summary)
                    .then_some(Site::Following(distance));
            }
            between.statement(&statement.value);
            if between.found || between.rejected {
                return None;
            }
        }
        let mut own = Scan::new(self, Some(temporary));
        own.value(tail?);
        (own.found && own.accepts(producer, &between.summary)).then_some(Site::Tail)
    }
}

fn own_values(statement: &mut Statement) -> Vec<&mut Value> {
    match statement {
        Statement::Expression(value)
        | Statement::Return(Some(value))
        | Statement::If {
            condition: value, ..
        }
        | Statement::Switch {
            discriminant: value,
            ..
        }
        | Statement::Temporary {
            initializer: Some(value),
            ..
        } => vec![value],
        Statement::Write { place, value, .. } => {
            let mut values = vec![value];
            place_values(place, &mut values);
            values
        }
        Statement::Let(variable) => variable.initializer.iter_mut().collect(),
        #[expect(
            slate::sensitive_wildcard,
            reason = "mirrors Scan::own_values: no other statement has a value evaluated once before its nested statements"
        )]
        _ => Vec::new(),
    }
}

fn place_values<'p>(place: &'p mut Place, values: &mut Vec<&'p mut Value>) {
    match &mut place.kind {
        PlaceKind::Binding(_) | PlaceKind::MacroArg { .. } => {}
        PlaceKind::Deref(value)
        | PlaceKind::CompoundLiteral {
            initializer: value, ..
        }
        | PlaceKind::Temporary {
            initializer: value, ..
        } => values.push(value),
        PlaceKind::ComplexPart { base, .. }
        | PlaceKind::Field { base, .. }
        | PlaceKind::Swizzle { base, .. } => place_values(base, values),
        PlaceKind::Lane { base, index } => {
            place_values(base, values);
            values.push(index);
        }
        PlaceKind::Index { base, index } => {
            values.push(base);
            values.push(index);
        }
    }
}

struct Substitute {
    temporary: BindingId,
    initializer: Option<Value>,
}

impl Visitor for Substitute {
    fn rewrite_value(&mut self, value: &mut Value) {
        if let ValueKind::Read {
            place:
                Place {
                    kind: PlaceKind::Binding(binding),
                    ..
                },
            ..
        } = &value.node.value
            && *binding == self.temporary
            && let Some(initializer) = self.initializer.take()
        {
            *value = initializer;
        }
    }
}

fn substitute(
    values: Vec<&mut Value>,
    temporary: BindingId,
    initializer: Option<Value>,
) -> Option<Value> {
    let mut substitute = Substitute {
        temporary,
        initializer,
    };
    for value in values {
        if substitute.initializer.is_none() {
            break;
        }
        walk_value(value, &mut substitute);
    }
    substitute.initializer
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

    fn accepts(&self, producer: &Summary, between: &Summary) -> bool {
        !self.rejected
            && !(self.conditional && producer.effectful())
            && !producer.conflicts(between)
            && !producer.conflicts(&self.summary)
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
        if self.context.macro_values.contains_key(&value.node.id) {
            self.nested(|scan| deep(|| scan.value_kind(&value.node.value)));
        } else {
            deep(|| self.value_kind(&value.node.value));
        }
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
            ValueKind::StatementExpression(evaluation) => {
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
