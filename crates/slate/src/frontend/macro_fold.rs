use foldhash::{HashMap, HashSet};
use slate_parser::ast::{MacroExpansionId, NodeId, Span};
use slate_parser::ir::*;

pub(super) struct Folded {
    block: NodeId,
    resume: Option<NodeId>,
    expansions: Vec<MacroExpansionId>,
}

pub(super) fn fold_hoisted_runs(module: &mut Module) -> Vec<Folded> {
    let runs = std::mem::take(&mut module.macro_hoisted_runs);
    if runs.is_empty() {
        return Vec::new();
    }
    let globals: HashSet<BindingId> = module
        .globals
        .iter()
        .map(|global| global.value.variable.id)
        .collect();
    let mut ordered: Vec<(NodeId, HoistedRun)> = runs.into_iter().collect();
    ordered.sort_by_key(|(root, run)| (std::cmp::Reverse(run.statements.len()), root.0));
    let mut functions = std::mem::take(&mut module.functions);
    let facts: Vec<Option<FunctionFacts>> = functions
        .iter_mut()
        .map(|function| FunctionFacts::new(&mut function.value, &globals))
        .collect();
    let mut folded = Vec::new();
    let mut moved: HashSet<MacroExpansionId> = HashSet::default();
    let mut failed: HashSet<MacroExpansionId> = HashSet::default();
    for (root, run) in ordered {
        let Some(wraps) = module.macro_values.get(&root).cloned() else {
            continue;
        };
        let expansions: Vec<MacroExpansionId> = wraps.iter().map(|wrap| wrap.expansion()).collect();
        let mut target = None;
        for (function, facts) in functions.iter_mut().zip(&facts) {
            let (Some(body), Some(facts)) = (&mut function.value.body, facts) else {
                continue;
            };
            let mut search = Fold {
                run: &run,
                facts,
                outcome: None,
            };
            walk_function_body(body, &mut search);
            if let Some(outcome) = search.outcome {
                target = outcome;
                break;
            }
        }
        let Some((node, resume)) = target else {
            failed.extend(expansions);
            continue;
        };
        module.macro_values.remove(&root);
        module.macro_values.entry(node).or_default().extend(wraps);
        if node != run.value
            && let Some(c) = module.c_types.nodes.get(&run.value).copied()
        {
            module.c_types.nodes.insert(node, c);
        }
        moved.extend(expansions.iter().copied());
        if node != run.value {
            folded.push(Folded {
                block: node,
                resume,
                expansions,
            });
        }
    }
    module.functions = functions;
    for expansion in moved.difference(&failed) {
        if module.macro_rejections.get(expansion) == Some(&"hoist_split_region") {
            module.macro_rejections.remove(expansion);
        }
    }
    folded
}

pub(super) fn unfold_rejected(
    module: &mut Module,
    folded: Vec<Folded>,
    kept: &HashSet<MacroExpansionId>,
) {
    let pending: HashMap<NodeId, Option<NodeId>> = folded
        .into_iter()
        .filter(|folded| {
            !folded
                .expansions
                .iter()
                .any(|expansion| kept.contains(expansion))
        })
        .map(|folded| (folded.block, folded.resume))
        .collect();
    if pending.is_empty() {
        return;
    }
    let mut unfold = Unfold { pending };
    for function in &mut module.functions {
        if let Some(body) = &mut function.value.body {
            walk_body(body, &mut unfold);
        }
    }
}

struct FunctionFacts {
    safe: HashSet<BindingId>,
    references: HashMap<BindingId, usize>,
}

impl FunctionFacts {
    fn new(function: &mut Function, globals: &HashSet<BindingId>) -> Option<Self> {
        let body = function.body.as_mut()?;
        let mut scan = Scan::default();
        if let Parameters::Prototype { fixed, .. } = &function.parameters {
            scan.locals
                .extend(fixed.iter().map(|parameter| parameter.value.id));
        }
        walk_function_body(body, &mut scan);
        let safe = scan
            .locals
            .difference(&scan.escaped)
            .filter(|binding| !globals.contains(binding))
            .copied()
            .collect();
        Some(Self {
            safe,
            references: scan.references,
        })
    }
}

#[derive(Default)]
struct Scan {
    locals: HashSet<BindingId>,
    escaped: HashSet<BindingId>,
    references: HashMap<BindingId, usize>,
}

impl Visitor for Scan {
    fn statement(&mut self, statement: &Statement) {
        match statement {
            Statement::Temporary { id, .. } => {
                self.locals.insert(*id);
            }
            Statement::Let(variable) if variable.storage == StorageDuration::Automatic => {
                self.locals.insert(variable.id);
            }
            #[expect(
                slate::sensitive_wildcard,
                reason = "only automatic declarations introduce foldable locals"
            )]
            _ => {}
        }
    }

    fn value_kind(&mut self, kind: &ValueKind) {
        match kind {
            ValueKind::AddressOf(place) | ValueKind::ArrayDecay { place, .. } => {
                if let Some(binding) = root_binding(place) {
                    self.escaped.insert(binding);
                }
            }
            #[expect(
                slate::sensitive_wildcard,
                reason = "only address-taking kinds let a local escape"
            )]
            _ => {}
        }
    }

    fn place_kind(&mut self, kind: &PlaceKind) {
        if let PlaceKind::Binding(binding) = kind {
            *self.references.entry(*binding).or_default() += 1;
        }
    }

    fn capture(&mut self, binding: BindingId, _kind: CaptureKind) {
        self.escaped.insert(binding);
        *self.references.entry(binding).or_default() += 1;
    }
}

fn root_binding(place: &Place) -> Option<BindingId> {
    match &place.kind {
        PlaceKind::Binding(binding) => Some(*binding),
        PlaceKind::Field { base, .. }
        | PlaceKind::ComplexPart { base, .. }
        | PlaceKind::Lane { base, .. }
        | PlaceKind::Swizzle { base, .. } => root_binding(base),
        PlaceKind::Deref(_)
        | PlaceKind::Index { .. }
        | PlaceKind::CompoundLiteral { .. }
        | PlaceKind::Temporary { .. } => None,
    }
}

#[derive(Default)]
struct Effects {
    reads: HashSet<BindingId>,
    writes: HashSet<BindingId>,
    declared: HashSet<BindingId>,
    memory_read: bool,
    memory_write: bool,
    opaque: bool,
    control: bool,
}

impl Visitor for Effects {
    fn statement(&mut self, statement: &Statement) {
        match statement {
            Statement::Write { place, .. } => match place.kind {
                PlaceKind::Binding(binding) => {
                    self.writes.insert(binding);
                }
                PlaceKind::Deref(_)
                | PlaceKind::CompoundLiteral { .. }
                | PlaceKind::Temporary { .. }
                | PlaceKind::ComplexPart { .. }
                | PlaceKind::Field { .. }
                | PlaceKind::Index { .. }
                | PlaceKind::Lane { .. }
                | PlaceKind::Swizzle { .. } => self.memory_write = true,
            },
            Statement::Temporary { id, .. } => {
                self.declared.insert(*id);
            }
            Statement::Let(variable) => {
                self.declared.insert(variable.id);
                if variable.storage != StorageDuration::Automatic {
                    self.control = true;
                }
            }
            Statement::Comment(_)
            | Statement::Null
            | Statement::Expression(_)
            | Statement::Block(_)
            | Statement::If { .. } => {}
            Statement::Fence { .. }
            | Statement::Return(_)
            | Statement::LabeledBlock { .. }
            | Statement::While { .. }
            | Statement::DoWhile { .. }
            | Statement::For { .. }
            | Statement::Switch { .. }
            | Statement::Case { .. }
            | Statement::Default { .. }
            | Statement::Break(_)
            | Statement::Continue(_)
            | Statement::Goto(_)
            | Statement::ComputedGoto(_)
            | Statement::Asm(_)
            | Statement::Label { .. }
            | Statement::MacroExpansion { .. }
            | Statement::NestedFunction(_)
            | Statement::OmpParallel(_)
            | Statement::OmpFor(_)
            | Statement::SehTry { .. }
            | Statement::SehLeave(_) => self.control = true,
        }
    }

    fn value_kind(&mut self, kind: &ValueKind) {
        match kind {
            ValueKind::Read {
                ordering: Some(_), ..
            }
            | ValueKind::Store { .. }
            | ValueKind::Update { .. }
            | ValueKind::CompareExchange { .. }
            | ValueKind::Fence { .. }
            | ValueKind::Call { .. }
            | ValueKind::VaArg { .. }
            | ValueKind::VaStart { .. }
            | ValueKind::VaEnd { .. }
            | ValueKind::VaCopy { .. }
            | ValueKind::Intrinsic { .. }
            | ValueKind::Overflow { .. }
            | ValueKind::Copy { .. }
            | ValueKind::Seh(_)
            | ValueKind::Block(_) => self.opaque = true,
            #[expect(
                slate::sensitive_wildcard,
                reason = "every other kind only computes from its operands and places"
            )]
            _ => {}
        }
    }

    fn place_kind(&mut self, kind: &PlaceKind) {
        match kind {
            PlaceKind::Binding(binding) => {
                self.reads.insert(*binding);
            }
            PlaceKind::Deref(_)
            | PlaceKind::Index { .. }
            | PlaceKind::CompoundLiteral { .. }
            | PlaceKind::Temporary { .. } => self.memory_read = true,
            PlaceKind::ComplexPart { .. }
            | PlaceKind::Field { .. }
            | PlaceKind::Lane { .. }
            | PlaceKind::Swizzle { .. } => {}
        }
    }

    fn capture(&mut self, _binding: BindingId, _kind: CaptureKind) {
        self.opaque = true;
    }
}

impl Effects {
    fn of_statements(statements: &[Span<Statement>]) -> Self {
        let mut effects = Self::default();
        let mut copy = statements.to_vec();
        walk_function_body(&mut copy, &mut effects);
        effects
    }

    fn of_values(values: &[&Value]) -> Self {
        let mut effects = Self::default();
        for value in values {
            let mut copy = (*value).clone();
            walk_function_value(&mut copy, &mut effects);
        }
        effects
    }

    fn commutes_with(&self, run: &Self, safe: &HashSet<BindingId>) -> bool {
        if self.opaque || self.control || self.memory_write || !self.writes.is_empty() {
            return false;
        }
        if !self.reads.is_disjoint(&run.writes) {
            return false;
        }
        if run.opaque || run.memory_write {
            return !self.memory_read && self.reads.iter().all(|binding| safe.contains(binding));
        }
        true
    }
}

fn roots(statement: &mut Statement) -> Vec<&mut Value> {
    match statement {
        Statement::Temporary {
            initializer: Some(value),
            ..
        }
        | Statement::Expression(value)
        | Statement::Return(Some(value))
        | Statement::If {
            condition: value, ..
        }
        | Statement::Switch {
            discriminant: value,
            ..
        }
        | Statement::Write { value, .. } => vec![value],
        Statement::Let(variable) => variable.initializer.iter_mut().collect(),
        #[expect(
            slate::sensitive_wildcard,
            reason = "a hoisted value is consumed only by a statement's own operand"
        )]
        _ => Vec::new(),
    }
}

fn evaluated_before_bodies(statement: &Statement) -> Vec<Value> {
    match statement {
        Statement::Temporary {
            initializer: Some(value),
            ..
        }
        | Statement::Expression(value)
        | Statement::Return(Some(value))
        | Statement::If {
            condition: value, ..
        }
        | Statement::Switch {
            discriminant: value,
            ..
        } => vec![value.clone()],
        Statement::Write { place, value, .. } => vec![
            value.clone(),
            Value {
                ty: value.ty.clone(),
                node: value.node.derive(ValueKind::Read {
                    place: place.clone(),
                    ordering: None,
                }),
            },
        ],
        Statement::Let(variable) => variable.initializer.iter().cloned().collect(),
        #[expect(
            slate::sensitive_wildcard,
            reason = "statements outside roots never consume a hoisted value"
        )]
        _ => Vec::new(),
    }
}

fn call_operands(value: Value) -> Vec<Value> {
    let Value { ty, node } = value;
    let site = node.erased();
    match node.value {
        ValueKind::Call {
            callee, arguments, ..
        } => match callee {
            Callee::Indirect(callee) => std::iter::once(*callee).chain(arguments).collect(),
            Callee::Direct(_) => arguments,
        },
        kind => vec![Value {
            ty,
            node: site.with_value(kind),
        }],
    }
}

fn is_gap(statement: &Statement) -> bool {
    match statement {
        Statement::Temporary {
            initializer: Some(_),
            ..
        } => true,
        Statement::Let(variable) => {
            variable.initializer.is_some() && variable.storage == StorageDuration::Automatic
        }
        #[expect(
            slate::sensitive_wildcard,
            reason = "only a declaration with a pure initializer may sit between a run and its value"
        )]
        _ => false,
    }
}

struct Extract {
    target: NodeId,
    nested: usize,
    guarded: usize,
    found: Option<(Value, bool)>,
}

impl Extract {
    fn new(target: NodeId) -> Self {
        Self {
            target,
            nested: 0,
            guarded: 0,
            found: None,
        }
    }

    fn take(statement: &mut Statement, target: NodeId) -> Option<(Value, bool)> {
        let mut extract = Self::new(target);
        for root in roots(statement) {
            walk_function_value(root, &mut extract);
            if extract.found.is_some() {
                break;
            }
        }
        extract.found
    }
}

fn scoping(kind: &ValueKind) -> (bool, bool) {
    (
        matches!(kind, ValueKind::StatementExpression(_)),
        matches!(
            kind,
            ValueKind::Conditional { .. } | ValueKind::Logical { .. }
        ),
    )
}

impl Visitor for Extract {
    fn value_kind(&mut self, kind: &ValueKind) {
        let (nested, guarded) = scoping(kind);
        self.nested += usize::from(nested);
        self.guarded += usize::from(guarded);
    }

    fn rewrite_value(&mut self, value: &mut Value) {
        let (nested, guarded) = scoping(&value.node.value);
        self.nested -= usize::from(nested);
        self.guarded -= usize::from(guarded);
        if value.node.id == self.target && self.nested == 0 && self.found.is_none() {
            let placeholder = Value {
                ty: value.ty.clone(),
                node: value.node.clone().with_value(ValueKind::Void),
            };
            self.found = Some((std::mem::replace(value, placeholder), self.guarded > 0));
        }
    }
}

struct Put {
    target: NodeId,
    value: Option<Value>,
}

impl Visitor for Put {
    fn rewrite_value(&mut self, value: &mut Value) {
        if value.node.id == self.target
            && matches!(value.node.value, ValueKind::Void)
            && let Some(replacement) = self.value.take()
        {
            *value = replacement;
        }
    }
}

fn put(statement: &mut Statement, target: NodeId, value: Value) {
    let mut put = Put {
        target,
        value: Some(value),
    };
    for root in roots(statement) {
        walk_function_value(root, &mut put);
        if put.value.is_none() {
            break;
        }
    }
}

struct Fold<'a> {
    run: &'a HoistedRun,
    facts: &'a FunctionFacts,
    outcome: Option<Option<(NodeId, Option<NodeId>)>>,
}

impl Visitor for Fold<'_> {
    fn rewrite_statement_list(&mut self, list: &mut Vec<Span<Statement>>) {
        if self.outcome.is_some() {
            return;
        }
        if self.run.statements.is_empty() {
            if list.iter_mut().any(|statement| {
                let mut extract = Extract::new(self.run.value);
                for root in roots(&mut statement.value) {
                    walk_function_value(root, &mut extract);
                }
                if let Some((value, _)) = extract.found {
                    put(&mut statement.value, self.run.value, value);
                    true
                } else {
                    false
                }
            }) {
                self.outcome = Some(Some((self.run.value, None)));
            }
            return;
        }
        let Some(start) = list
            .iter()
            .position(|statement| statement.id == self.run.statements[0])
        else {
            return;
        };
        self.outcome = Some(fold(list, start, self.run, self.facts));
    }
}

fn fold(
    list: &mut Vec<Span<Statement>>,
    start: usize,
    run: &HoistedRun,
    facts: &FunctionFacts,
) -> Option<(NodeId, Option<NodeId>)> {
    let end = start + run.statements.len();
    if list.len() < end
        || !list[start..end]
            .iter()
            .zip(&run.statements)
            .all(|(statement, id)| statement.id == *id)
    {
        return None;
    }
    let effects = Effects::of_statements(&list[start..end]);
    if effects.control {
        return None;
    }
    let mut consumer = None;
    for (index, statement) in list.iter_mut().enumerate().skip(end) {
        if let Some(found) = Extract::take(&mut statement.value, run.value) {
            consumer = Some((index, found));
            break;
        }
        if !is_gap(&statement.value) {
            return None;
        }
    }
    let (index, (value, guarded)) = consumer?;
    let gap: Vec<Value> = list[end..index]
        .iter()
        .flat_map(|statement| evaluated_before_bodies(&statement.value))
        .collect();
    let siblings: Vec<Value> = evaluated_before_bodies(&list[index].value)
        .into_iter()
        .flat_map(call_operands)
        .chain(gap)
        .collect();
    let siblings = Effects::of_values(&siblings.iter().collect::<Vec<_>>());
    let local = effects.declared.iter().all(|binding| {
        let mut count = ReferenceCount {
            binding: *binding,
            count: 0,
        };
        let mut copy = list[start..end].to_vec();
        walk_function_body(&mut copy, &mut count);
        let mut copy = value.clone();
        walk_function_value(&mut copy, &mut count);
        facts.references.get(binding).copied().unwrap_or(0) == count.count
    });
    if guarded || !local || !siblings.commutes_with(&effects, &facts.safe) {
        put(&mut list[index].value, run.value, value);
        return None;
    }
    let resume = list[end].id;
    let statements: Vec<Span<Statement>> = list.drain(start..end).collect();
    let node = value
        .node
        .derive(ValueKind::StatementExpression(Box::new(Evaluation {
            statements,
            value: value.clone(),
        })));
    let id = node.id;
    let block = Value {
        ty: value.ty.clone(),
        node,
    };
    put(
        &mut list[index - run.statements.len()].value,
        run.value,
        block,
    );
    Some((id, Some(resume)))
}

struct ReferenceCount {
    binding: BindingId,
    count: usize,
}

impl Visitor for ReferenceCount {
    fn place_kind(&mut self, kind: &PlaceKind) {
        if matches!(kind, PlaceKind::Binding(binding) if *binding == self.binding) {
            self.count += 1;
        }
    }

    fn capture(&mut self, binding: BindingId, _kind: CaptureKind) {
        if binding == self.binding {
            self.count += 1;
        }
    }
}

struct Unfold {
    pending: HashMap<NodeId, Option<NodeId>>,
}

impl Visitor for Unfold {
    fn rewrite_statement_list(&mut self, list: &mut Vec<Span<Statement>>) {
        let mut index = 0;
        while index < list.len() && !self.pending.is_empty() {
            let mut spliced = false;
            let blocks: Vec<NodeId> = self.pending.keys().copied().collect();
            for block in blocks {
                let Some((value, _)) = Extract::take(&mut list[index].value, block) else {
                    continue;
                };
                let resume = self.pending.remove(&block).flatten();
                let ValueKind::StatementExpression(evaluation) = value.node.value else {
                    put(&mut list[index].value, block, value);
                    continue;
                };
                let Evaluation { statements, value } = *evaluation;
                put(&mut list[index].value, block, value);
                let at = resume
                    .and_then(|resume| list[..index].iter().position(|s| s.id == resume))
                    .unwrap_or(index);
                let count = statements.len();
                list.splice(at..at, statements);
                index += count;
                spliced = true;
                break;
            }
            if !spliced {
                index += 1;
            }
        }
    }
}
