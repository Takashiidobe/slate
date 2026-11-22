use foldhash::{HashMap, HashSet};
use slate_parser::ast::{NodeId, Span};
use slate_parser::ir::{BindingId, MacroValues, Module, Parameters, Statement, Visitor, walk_body};

use super::inline_temporaries::{children, evaluations};
use super::structure_goto::jump_lists;
use super::truth_canonicalize::negation;

type Body = Vec<Span<Statement>>;

pub(super) fn transform(module: &mut Module) {
    let mut retired = Vec::new();
    let Module {
        functions,
        macro_values,
        globals,
        ..
    } = module;
    let mut outer: HashSet<String> = globals
        .iter()
        .map(|global| global.value.variable.name.clone())
        .chain(functions.iter().map(|function| function.value.name.clone()))
        .collect();
    for function in functions {
        if let Some(body) = &mut function.value.body {
            let parameters: Vec<String> = match &function.value.parameters {
                Parameters::Prototype { fixed, .. } => fixed
                    .iter()
                    .filter_map(|parameter| parameter.value.name.clone())
                    .collect(),
                Parameters::Unprototyped => Vec::new(),
            };
            let added: Vec<String> = parameters
                .into_iter()
                .filter(|name| outer.insert(name.clone()))
                .collect();
            let mut pass = Pass {
                macro_values,
                outer: &outer,
                locals: names(body).0,
                cases: false,
                retired: &mut retired,
            };
            while pass.list(body, None) {}
            pass.cases = true;
            while pass.list(body, None) {}
            for name in added {
                outer.remove(&name);
            }
        }
    }
    for id in retired {
        module.c_types.nodes.remove(&id);
        module.metadata.remove(&id);
    }
}

struct Pass<'a> {
    macro_values: &'a MacroValues,
    outer: &'a HashSet<String>,
    locals: HashMap<String, usize>,
    cases: bool,
    retired: &'a mut Vec<NodeId>,
}

impl Pass<'_> {
    fn list(&mut self, body: &mut Body, exit: Option<Exit>) -> bool {
        let mut changed = false;
        let last = body.len().saturating_sub(1);
        for (index, statement) in body.iter_mut().enumerate() {
            changed |= self.descend(statement, (index == last).then_some(exit).flatten());
        }
        let mut index = 0;
        while index < body.len() {
            if self.sink(body, index, exit) || self.else_arm(body, index, exit) {
                changed = true;
                continue;
            }
            if self.dissolve(body, index) {
                changed = true;
                continue;
            }
            index += 1;
        }
        if let (Some(exit @ Exit::Break(_)), Some(last)) = (exit, body.last())
            && exit.is(&last.value)
            && let Some(last) = body.pop()
        {
            self.retired.push(last.id);
            changed = true;
        }
        changed
    }

    fn descend(&mut self, statement: &mut Span<Statement>, exit: Option<Exit>) -> bool {
        let mut changed = false;
        match &mut statement.value {
            Statement::LabeledBlock { id, body } => {
                let id = *id;
                changed |= self.list(body, Some(Exit::Break(id)));
            }
            Statement::Block(body) => changed |= self.list(body, exit),
            Statement::Switch { id, body, .. } => {
                changed |= absorb(body, *id);
                if let Some(Exit::Break(exit)) = exit.filter(|_| self.cases) {
                    changed |= retarget_cases(body, *id, exit);
                }
                changed |= self.list(body, None);
            }
            Statement::If {
                then_body,
                else_body,
                ..
            } => {
                changed |= self.list(then_body, exit);
                if let Some(else_body) = else_body {
                    changed |= self.list(else_body, exit);
                }
            }
            Statement::For { id, init, body, .. } => {
                let id = *id;
                changed |= self.list(init, None);
                changed |= self.list(body, Some(Exit::Continue(id)));
            }
            Statement::While { id, body, .. } | Statement::DoWhile { id, body, .. } => {
                let id = *id;
                changed |= self.list(body, Some(Exit::Continue(id)));
            }
            other => {
                for child in children(other) {
                    changed |= self.list(child, None);
                }
            }
        }
        for evaluation in evaluations(&mut statement.value) {
            changed |= self.list(&mut evaluation.statements, None);
        }
        changed
    }

    fn sink(&mut self, body: &mut Body, index: usize, exit: Option<Exit>) -> bool {
        let Statement::LabeledBlock { id, body: inner } = &mut body[index].value else {
            return false;
        };
        let id = *id;
        if !inner.last().is_some_and(|last| diverges(&last.value))
            || nested_breaks(inner, id) != 1
            || all_breaks(inner, id) != 1
            || self.shadows(inner)
        {
            return false;
        }
        let end = body[index + 1..]
            .iter()
            .position(|statement| diverges(&statement.value))
            .map(|offset| index + 1 + offset);
        let (end, fall) = match (end, exit) {
            (Some(end), _) => (end + 1, None),
            (None, Some(exit)) => (body.len(), Some(exit)),
            (None, None) => return false,
        };
        if end == index + 1 || !movable(&body[index + 1..end], &body[end..]) {
            return false;
        }
        let mut tail: Body = body.drain(index + 1..end).collect();
        if let Some(exit) = fall {
            let (spelling, expansion) = tail
                .last()
                .map_or((body[index].spelling, body[index].expansion), |last| {
                    (last.spelling, last.expansion)
                });
            tail.push(Span::new(exit.jump(), spelling, expansion));
        }
        let block = body.remove(index);
        self.retired.push(block.id);
        let Statement::LabeledBlock {
            body: mut inner, ..
        } = block.value
        else {
            return false;
        };
        replace(&mut inner, id, &mut Some(tail), self.retired);
        body.splice(index..index, inner);
        true
    }

    fn else_arm(&self, body: &mut Body, index: usize, exit: Option<Exit>) -> bool {
        let (exit, end) = match (exit, body.last().map(|last| &last.value)) {
            (Some(exit @ Exit::Break(_)), _) => (exit, body.len()),
            (_, Some(Statement::Break(id))) => (Exit::Break(*id), body.len() - 1),
            (_, _) => return false,
        };
        if index + 1 >= end || !movable(&body[index + 1..end], &[]) {
            return false;
        }
        let breaks =
            |arm: &[Span<Statement>]| tail(arm).last().is_some_and(|last| exit.is(&last.value));
        let Statement::If {
            then_body,
            else_body,
            folded: None,
            ..
        } = &mut body[index].value
        else {
            return false;
        };
        let into_else = match else_body {
            None if breaks(then_body) => true,
            None => return false,
            Some(else_body) => {
                if breaks(then_body) && !ends(else_body) {
                    true
                } else if breaks(else_body) && !ends(then_body) {
                    false
                } else {
                    return false;
                }
            }
        };
        let rest: Body = body.drain(index + 1..end).collect();
        let Statement::If {
            then_body,
            else_body,
            ..
        } = &mut body[index].value
        else {
            return false;
        };
        match (into_else, else_body.as_mut()) {
            (true, Some(arm)) => tail_mut(arm).extend(rest),
            (true, None) => {
                tail_mut(then_body).pop();
                *else_body = Some(rest);
            }
            (false, _) => tail_mut(then_body).extend(rest),
        }
        if let Statement::If {
            condition,
            then_body,
            else_body,
            ..
        } = &mut body[index].value
            && then_body.is_empty()
            && let Some(negated) = negation(condition, self.macro_values)
            && let Some(arm) = else_body.take()
        {
            *condition = negated;
            *then_body = arm;
        }
        true
    }

    fn shadows(&self, inner: &mut Body) -> bool {
        let (declared, scoped) = names(inner);
        scoped
            || declared.iter().any(|(name, count)| {
                self.outer.contains(name) || self.locals.get(name).copied().unwrap_or(0) > *count
            })
    }

    fn dissolve(&mut self, body: &mut Body, index: usize) -> bool {
        let Statement::LabeledBlock { id, body: inner } = &mut body[index].value else {
            return false;
        };
        if all_breaks(inner, *id) != 0 {
            return false;
        }
        let inner = std::mem::take(inner);
        if inner.iter().any(|statement| {
            matches!(
                statement.value,
                Statement::Let(_) | Statement::Temporary { .. }
            )
        }) {
            body[index].value = Statement::Block(inner);
            return true;
        }
        self.retired.push(body[index].id);
        body.splice(index..=index, inner);
        true
    }
}

fn diverges(statement: &Statement) -> bool {
    match statement {
        Statement::Return(_)
        | Statement::Break(_)
        | Statement::Continue(_)
        | Statement::Goto(_)
        | Statement::ComputedGoto(_) => true,
        Statement::If {
            then_body,
            else_body: Some(else_body),
            ..
        } => ends(then_body) && ends(else_body),
        Statement::Block(body) | Statement::Case { body, .. } | Statement::Default { body, .. } => {
            ends(body)
        }
        Statement::Switch { id, body, .. } => {
            ends(body)
                && defaulted(body, *id)
                && body.iter().all(|statement| jumps(statement, *id) == 0)
        }
        #[expect(
            slate::sensitive_wildcard,
            reason = "only jumps and arms that all end in jumps leave the list"
        )]
        _ => false,
    }
}

fn jumps(statement: &Span<Statement>, id: BindingId) -> usize {
    usize::from(matches!(statement.value, Statement::Break(target) if target == id))
        + jump_lists(&statement.value)
            .into_iter()
            .flatten()
            .map(|statement| jumps(statement, id))
            .sum::<usize>()
}

fn defaulted(body: &[Span<Statement>], id: BindingId) -> bool {
    body.iter().any(|statement| {
        matches!(statement.value, Statement::Default { switch, .. } if switch == id)
            || (!matches!(statement.value, Statement::Switch { .. })
                && jump_lists(&statement.value)
                    .into_iter()
                    .any(|list| defaulted(list, id)))
    })
}

fn tail(body: &[Span<Statement>]) -> &[Span<Statement>] {
    match body.last().map(|last| &last.value) {
        Some(Statement::Block(inner)) if !declares(inner) => tail(inner),
        _ => body,
    }
}

fn tail_mut(body: &mut Body) -> &mut Body {
    if matches!(body.last().map(|last| &last.value), Some(Statement::Block(inner)) if !declares(inner))
        && let Some(Span {
            value: Statement::Block(inner),
            ..
        }) = body.last_mut()
    {
        return tail_mut(inner);
    }
    body
}

fn declares(body: &[Span<Statement>]) -> bool {
    body.iter()
        .any(|statement| matches!(statement.value, Statement::Let(_)))
}

fn ends(body: &[Span<Statement>]) -> bool {
    body.last().is_some_and(|last| diverges(&last.value))
}

fn nested_breaks(body: &mut Body, id: BindingId) -> usize {
    body.iter_mut()
        .map(|statement| {
            usize::from(matches!(statement.value, Statement::Break(target) if target == id))
                + children(&mut statement.value)
                    .into_iter()
                    .map(|child| nested_breaks(child, id))
                    .sum::<usize>()
        })
        .sum()
}

fn all_breaks(body: &mut Body, id: BindingId) -> usize {
    let mut breaks = Breaks { id, count: 0 };
    walk_body(body, &mut breaks);
    breaks.count
}

struct Breaks {
    id: BindingId,
    count: usize,
}

impl Visitor for Breaks {
    fn statement(&mut self, statement: &Statement) {
        if matches!(statement, Statement::Break(target) if *target == self.id) {
            self.count += 1;
        }
    }
}

fn names(body: &mut Body) -> (HashMap<String, usize>, bool) {
    let mut names = Names::default();
    walk_body(body, &mut names);
    (names.declared, names.scoped)
}

#[derive(Default)]
struct Names {
    declared: HashMap<String, usize>,
    scoped: bool,
}

impl Visitor for Names {
    fn statement(&mut self, statement: &Statement) {
        match statement {
            Statement::Let(variable) => {
                self.scoped |= variable.cleanup.is_some() || variable.ty.is_variably_modified();
                if !variable.name.is_empty() {
                    *self.declared.entry(variable.name.clone()).or_default() += 1;
                }
            }
            Statement::SehTry { .. } => self.scoped = true,
            #[expect(
                slate::sensitive_wildcard,
                reason = "only declarations and SEH scopes affect moving code into a scope"
            )]
            _ => {}
        }
    }
}

fn absorb(body: &mut Body, switch: BindingId) -> bool {
    let mut changed = false;
    let mut index = 0;
    while index < body.len() {
        match &mut body[index].value {
            Statement::Block(inner) => changed |= absorb(inner, switch),
            Statement::Case { switch: owner, .. } | Statement::Default { switch: owner, .. }
                if *owner == switch =>
            {
                let end = body[index + 1..]
                    .iter()
                    .position(|statement| {
                        matches!(
                            statement.value,
                            Statement::Case { .. } | Statement::Default { .. }
                        )
                    })
                    .map_or(body.len(), |offset| index + 1 + offset);
                if end > index + 1 && movable(&body[index + 1..end], &[]) {
                    let rest: Body = body.drain(index + 1..end).collect();
                    if let Statement::Case { body: case, .. }
                    | Statement::Default { body: case, .. } = &mut body[index].value
                    {
                        innermost(case).extend(rest);
                        changed = true;
                    }
                }
            }
            #[expect(
                slate::sensitive_wildcard,
                reason = "case labels sit directly in the switch body or its blocks"
            )]
            _ => {}
        }
        index += 1;
    }
    changed
}

fn innermost(body: &mut Body) -> &mut Body {
    if let [only] = body.as_slice()
        && matches!(
            only.value,
            Statement::Case { .. } | Statement::Default { .. }
        )
        && let Statement::Case { body: inner, .. } | Statement::Default { body: inner, .. } =
            &mut body[0].value
    {
        return innermost(inner);
    }
    body
}

#[derive(Clone, Copy)]
enum Exit {
    Break(BindingId),
    Continue(BindingId),
}

impl Exit {
    fn is(self, statement: &Statement) -> bool {
        match (self, statement) {
            (Self::Break(id), Statement::Break(target))
            | (Self::Continue(id), Statement::Continue(target)) => id == *target,
            #[expect(
                slate::sensitive_wildcard,
                reason = "only the matching jump reaches the end of the list"
            )]
            _ => false,
        }
    }

    fn jump(self) -> Statement {
        match self {
            Self::Break(id) => Statement::Break(id),
            Self::Continue(id) => Statement::Continue(id),
        }
    }
}

fn retarget_cases(body: &mut Body, switch: BindingId, exit: BindingId) -> bool {
    let mut changed = false;
    for statement in body {
        match &mut statement.value {
            Statement::Case {
                switch: owner,
                body,
                ..
            }
            | Statement::Default {
                switch: owner,
                body,
            } if *owner == switch => {
                if let Some(last) = tail_mut(body).last_mut()
                    && matches!(last.value, Statement::Break(target) if target == exit)
                {
                    last.value = Statement::Break(switch);
                    changed = true;
                }
            }
            Statement::Block(inner) => changed |= retarget_cases(inner, switch, exit),
            #[expect(
                slate::sensitive_wildcard,
                reason = "case labels sit directly in the switch body or its blocks"
            )]
            _ => {}
        }
    }
    changed
}

fn movable(range: &[Span<Statement>], rest: &[Span<Statement>]) -> bool {
    let mut targets = Targets::default();
    walk_body(&mut range.to_vec(), &mut targets);
    if targets.found {
        return false;
    }
    let declared: Vec<BindingId> = range
        .iter()
        .filter_map(|statement| match &statement.value {
            Statement::Let(variable) => Some(variable.id),
            Statement::Temporary { id, .. } => Some(*id),
            #[expect(
                slate::sensitive_wildcard,
                reason = "only declarations bind names later statements could read"
            )]
            _ => None,
        })
        .collect();
    if declared.is_empty() || rest.is_empty() {
        return true;
    }
    let mut reads = Reads {
        declared,
        found: false,
    };
    walk_body(&mut rest.to_vec(), &mut reads);
    !reads.found
}

#[derive(Default)]
struct Targets {
    found: bool,
}

impl Visitor for Targets {
    fn statement(&mut self, statement: &Statement) {
        self.found |= matches!(
            statement,
            Statement::Label { .. } | Statement::Case { .. } | Statement::Default { .. }
        );
    }
}

struct Reads {
    declared: Vec<BindingId>,
    found: bool,
}

impl Visitor for Reads {
    fn binding(&mut self, binding: BindingId) {
        self.found |= self.declared.contains(&binding);
    }
}

fn replace(body: &mut Body, id: BindingId, tail: &mut Option<Body>, retired: &mut Vec<NodeId>) {
    let mut index = 0;
    while index < body.len() {
        if tail.is_none() {
            return;
        }
        if matches!(body[index].value, Statement::Break(target) if target == id)
            && let Some(tail) = tail.take()
        {
            retired.push(body[index].id);
            body.splice(index..=index, tail);
            return;
        }
        for child in children(&mut body[index].value) {
            replace(child, id, tail, retired);
        }
        index += 1;
    }
}
