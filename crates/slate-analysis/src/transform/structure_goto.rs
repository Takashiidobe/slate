use foldhash::{HashMap, HashSet};
use slate_parser::ast::{NodeId, Span};
use slate_parser::ir::{
    AsmPiece, BindingId, Module, NestedFunction, Statement, ValueKind, Visitor, walk_body,
};

use super::inline_temporaries::{children, evaluations};

type Body = Vec<Span<Statement>>;

pub(super) fn transform(module: &mut Module) {
    let mut functions = std::mem::take(&mut module.functions);
    let mut retired = Vec::new();
    for function in &mut functions {
        let Some(body) = &mut function.value.body else {
            continue;
        };
        let mut survey = Survey::default();
        walk_body(body, &mut survey);
        if survey.indirect {
            continue;
        }
        let mut pass = StructureGoto {
            module,
            gotos: survey.gotos,
            retired: &mut retired,
        };
        pass.list(body);
    }
    module.functions = functions;
    for id in retired {
        module.c_types.nodes.remove(&id);
        module.metadata.remove(&id);
    }
}

#[derive(Default)]
struct Survey {
    gotos: HashMap<BindingId, usize>,
    indirect: bool,
}

impl Visitor for Survey {
    fn statement(&mut self, statement: &Statement) {
        match statement {
            Statement::Goto(label) => *self.gotos.entry(*label).or_default() += 1,
            Statement::ComputedGoto(_) => self.indirect = true,
            Statement::Asm(asm) => {
                self.indirect |= !asm.labels.is_empty()
                    || asm
                        .pieces
                        .iter()
                        .any(|piece| matches!(piece, AsmPiece::EntryLabel { .. }));
            }
            _ => {}
        }
    }

    fn value_kind(&mut self, kind: &ValueKind) {
        self.indirect |= matches!(kind, ValueKind::LabelAddress(_));
    }
}

struct StructureGoto<'a> {
    module: &'a mut Module,
    gotos: HashMap<BindingId, usize>,
    retired: &'a mut Vec<NodeId>,
}

impl StructureGoto<'_> {
    fn list(&mut self, body: &mut Body) {
        for statement in body.iter_mut() {
            for child in children(&mut statement.value) {
                self.list(child);
            }
            for evaluation in evaluations(&mut statement.value) {
                self.list(&mut evaluation.statements);
            }
        }
        let mut index = 0;
        while index < body.len() {
            let Statement::Label { id, .. } = body[index].value else {
                index += 1;
                continue;
            };
            if self.gotos.get(&id).copied().unwrap_or(0) == 0 {
                self.dissolve(body, index);
                continue;
            }
            match self.forward(body, index, id) {
                Some(start) => {
                    index = start + 1;
                    self.dissolve(body, index);
                }
                None => index += 1,
            }
        }
    }

    fn forward(&mut self, body: &mut Body, label: usize, id: BindingId) -> Option<usize> {
        let mut start = None;
        let mut reached = 0;
        for (index, statement) in body[..label].iter().enumerate() {
            let count = jumps(statement, id);
            if count != 0 {
                start.get_or_insert(index);
                reached += count;
            }
        }
        let start = start?;
        if reached != self.gotos[&id] || !movable(&body[start..label], &body[label..]) {
            return None;
        }
        let block = self.module.fresh_binding();
        let mut range: Body = body.drain(start..label).collect();
        for statement in &mut range {
            retarget(statement, id, block);
        }
        self.gotos.remove(&id);
        let spelling = range[0].spelling;
        let expansion = range[0].expansion;
        body.insert(
            start,
            Span::new(
                Statement::LabeledBlock {
                    id: block,
                    body: range,
                },
                spelling,
                expansion,
            ),
        );
        Some(start)
    }

    fn dissolve(&mut self, body: &mut Body, index: usize) {
        if let Statement::Label { body: inner, .. } = &mut body[index].value {
            let inner = std::mem::take(inner);
            self.retired.push(body[index].id);
            body.splice(index..=index, inner);
        }
    }
}

fn jump_lists(statement: &Statement) -> Vec<&Body> {
    match statement {
        Statement::Block(body)
        | Statement::LabeledBlock { body, .. }
        | Statement::While { body, .. }
        | Statement::DoWhile { body, .. }
        | Statement::Switch { body, .. }
        | Statement::Case { body, .. }
        | Statement::Default { body, .. }
        | Statement::Label { body, .. } => {
            let mut lists = vec![body];
            if let Statement::While { condition, .. } | Statement::DoWhile { condition, .. } =
                statement
            {
                lists.push(&condition.statements);
            }
            lists
        }
        Statement::If {
            then_body,
            else_body,
            ..
        } => std::iter::once(then_body).chain(else_body).collect(),
        Statement::For {
            init,
            condition,
            increment,
            body,
            ..
        } => [Some(init), Some(body)]
            .into_iter()
            .flatten()
            .chain(condition.iter().map(|evaluation| &evaluation.statements))
            .chain(increment.iter().map(|evaluation| &evaluation.statements))
            .collect(),
        _ => Vec::new(),
    }
}

fn jumps(statement: &Span<Statement>, label: BindingId) -> usize {
    let own = usize::from(matches!(statement.value, Statement::Goto(id) if id == label));
    own + jump_lists(&statement.value)
        .into_iter()
        .flatten()
        .map(|statement| jumps(statement, label))
        .sum::<usize>()
}

fn retarget(statement: &mut Span<Statement>, label: BindingId, block: BindingId) {
    if matches!(statement.value, Statement::Goto(id) if id == label) {
        statement.value = Statement::Break(block);
        return;
    }
    for child in children(&mut statement.value) {
        for statement in child {
            retarget(statement, label, block);
        }
    }
    for evaluation in evaluations(&mut statement.value) {
        for statement in &mut evaluation.statements {
            retarget(statement, label, block);
        }
    }
}

fn movable(range: &[Span<Statement>], rest: &[Span<Statement>]) -> bool {
    let mut declared = HashSet::default();
    if !declarations(range, &mut declared) {
        return false;
    }
    let mut switches = Switches::default();
    let mut range = range.to_vec();
    walk_body(&mut range, &mut switches);
    if switches.cases.difference(&switches.owned).next().is_some() {
        return false;
    }
    let mut uses = Uses {
        declared: &declared,
        used: false,
    };
    let mut rest = rest.to_vec();
    walk_body(&mut rest, &mut uses);
    !uses.used
}

fn declarations(range: &[Span<Statement>], declared: &mut HashSet<BindingId>) -> bool {
    for statement in range {
        match &statement.value {
            Statement::Let(variable) => {
                if variable.cleanup.is_some() || variable.ty.is_variably_modified() {
                    return false;
                }
                declared.insert(variable.id);
            }
            Statement::Temporary { id, .. } => {
                declared.insert(*id);
            }
            Statement::Label { body, .. } => {
                if !declarations(body, declared) {
                    return false;
                }
            }
            _ => {}
        }
    }
    true
}

#[derive(Default)]
struct Switches {
    owned: HashSet<BindingId>,
    cases: HashSet<BindingId>,
}

impl Visitor for Switches {
    fn statement(&mut self, statement: &Statement) {
        match statement {
            Statement::Switch { id, .. } => {
                self.owned.insert(*id);
            }
            Statement::Case { switch, .. } | Statement::Default { switch, .. } => {
                self.cases.insert(*switch);
            }
            _ => {}
        }
    }
}

struct Uses<'a> {
    declared: &'a HashSet<BindingId>,
    used: bool,
}

impl Visitor for Uses<'_> {
    fn binding(&mut self, binding: BindingId) {
        self.used |= self.declared.contains(&binding);
    }

    fn statement(&mut self, statement: &Statement) {
        if let Statement::NestedFunction(nested) = statement {
            self.used |= captures(nested, self.declared);
        }
    }

    fn value_kind(&mut self, kind: &ValueKind) {
        if let ValueKind::Block(block) = kind {
            self.used |= block
                .captures
                .iter()
                .any(|capture| self.declared.contains(&capture.id));
        }
    }
}

fn captures(nested: &NestedFunction, declared: &HashSet<BindingId>) -> bool {
    nested
        .captures
        .iter()
        .any(|capture| declared.contains(&capture.id))
}
