use foldhash::{HashMap, HashSet};
use slate_parser::ast::{NodeId, Span};
use slate_parser::ir::{
    AsmPiece, BindingId, Evaluation, Module, NestedFunction, Statement, Value, ValueKind, Visitor,
    walk_body, walk_value,
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
        let mut threads = HashMap::default();
        scan_threads(body, &mut Vec::new(), &mut threads);
        if !threads.is_empty() {
            thread(body, &mut Vec::new(), &threads, &mut survey.gotos);
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
        self.labels(body);
    }

    fn labels(&mut self, body: &mut Body) {
        let mut index = 0;
        while index < body.len() {
            let Statement::Label { id, .. } = body[index].value else {
                index += 1;
                continue;
            };
            let total = self.gotos.get(&id).copied().unwrap_or(0);
            if total == 0 {
                self.dissolve(body, index);
                continue;
            }
            match self.structure(body, index, id, total) {
                Some(next) => index = next,
                None => index += 1,
            }
        }
    }

    fn structure(
        &mut self,
        body: &mut Body,
        label: usize,
        id: BindingId,
        total: usize,
    ) -> Option<usize> {
        let (forward, ahead) = sites(&body[..label], id);
        let (backward, behind) = sites(&body[label..], id);
        if ahead + behind != total {
            return None;
        }
        let end = backward.last().map(|last| label + last + 1);
        if let Some(end) = end
            && !movable(&body[label..end], &body[end..])
        {
            return None;
        }
        if let Some(&start) = forward.first()
            && !movable(&body[start..label], &body[label..])
        {
            return None;
        }
        self.gotos.remove(&id);
        if let Some(end) = end {
            self.wrap_loop(body, label, end, id, behind);
        }
        let position = match forward.first() {
            Some(&start) => {
                self.wrap_block(body, start, label, id);
                start + 1
            }
            None => label,
        };
        if let Some(&start) = forward.first() {
            self.revisit(&mut body[start]);
        }
        if end.is_some() {
            self.revisit(&mut body[position]);
            return Some(position + 1);
        }
        self.dissolve(body, position);
        Some(position)
    }

    fn revisit(&mut self, statement: &mut Span<Statement>) {
        for child in children(&mut statement.value) {
            self.labels(child);
        }
    }

    fn wrap_block(&mut self, body: &mut Body, start: usize, label: usize, id: BindingId) {
        let block = self.module.fresh_binding();
        let mut range: Body = body.drain(start..label).collect();
        for statement in &mut range {
            retarget(statement, id, &Statement::Break(block));
        }
        let (spelling, expansion) = (range[0].spelling, range[0].expansion);
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
    }

    fn wrap_loop(
        &mut self,
        body: &mut Body,
        label: usize,
        end: usize,
        id: BindingId,
        gotos: usize,
    ) {
        let Statement::Label { body: inner, .. } = &mut body[label].value else {
            return;
        };
        let mut range = std::mem::take(inner);
        let looped = self.module.fresh_binding();
        let mut drained = body.drain(label..end);
        let Some(head) = drained.next() else {
            return;
        };
        range.extend(drained);
        self.retired.push(head.id);
        let (spelling, expansion) = (head.spelling, head.expansion);
        let condition = match range.pop() {
            Some(Span {
                id: node,
                value:
                    Statement::If {
                        condition,
                        then_body,
                        else_body: None,
                        folded: None,
                    },
                ..
            }) if gotos == 1 && only_goto(&then_body, id) => {
                self.retired.push(node);
                self.retired.extend(then_body.iter().map(|goto| goto.id));
                Some(condition)
            }
            Some(last) => {
                range.push(last);
                None
            }
            None => None,
        };
        let statement = match condition {
            Some(value) => {
                let split = condition_prefix(&range, &value);
                let statements = range.split_off(split);
                Statement::DoWhile {
                    id: looped,
                    body: range,
                    condition: Evaluation { statements, value },
                }
            }
            None => {
                for statement in &mut range {
                    retarget(statement, id, &Statement::Continue(looped));
                }
                match range.last() {
                    Some(last) if matches!(last.value, Statement::Continue(target) if target == looped) => {
                        if let Some(last) = range.pop() {
                            self.retired.push(last.id);
                        }
                    }
                    Some(last) if ends_in_jump(&last.value) => {}
                    last => {
                        let (spelling, expansion) = last.map_or((spelling, expansion), |last| {
                            (last.spelling, last.expansion)
                        });
                        range.push(Span::new(Statement::Break(looped), spelling, expansion));
                    }
                }
                Statement::For {
                    id: looped,
                    init: Vec::new(),
                    condition: None,
                    increment: None,
                    body: range,
                }
            }
        };
        body.insert(label, Span::new(statement, spelling, expansion));
    }

    fn dissolve(&mut self, body: &mut Body, index: usize) {
        if let Statement::Label { body: inner, .. } = &mut body[index].value {
            let inner = std::mem::take(inner);
            self.retired.push(body[index].id);
            body.splice(index..=index, inner);
        }
    }
}

fn scan_threads(
    body: &[Span<Statement>],
    enclosing: &mut Vec<(BindingId, bool)>,
    threads: &mut HashMap<BindingId, BindingId>,
) {
    let dirty: Vec<bool> = enclosing.iter().map(|(_, dirty)| *dirty).collect();
    scan_list(body, enclosing, threads);
    for ((_, flag), dirty) in enclosing.iter_mut().zip(dirty) {
        *flag = dirty;
    }
}

fn scan_list(
    body: &[Span<Statement>],
    enclosing: &mut Vec<(BindingId, bool)>,
    threads: &mut HashMap<BindingId, BindingId>,
) {
    for statement in body {
        match &statement.value {
            Statement::Let(variable) if variable.cleanup.is_some() => {
                for (_, dirty) in enclosing.iter_mut() {
                    *dirty = true;
                }
            }
            Statement::Label { id, body, .. } => {
                if let Some(Statement::Break(target)) = body.first().map(|first| &first.value)
                    && enclosing.contains(&(*target, false))
                {
                    threads.insert(*id, *target);
                }
            }
            _ => {}
        }
        match &statement.value {
            Statement::For {
                id,
                init,
                condition,
                increment,
                body,
            } => {
                let dirty: Vec<bool> = enclosing.iter().map(|(_, dirty)| *dirty).collect();
                scan_list(init, enclosing, threads);
                enclosing.push((*id, false));
                scan_threads(body, enclosing, threads);
                for evaluation in condition.iter().chain(increment) {
                    scan_threads(&evaluation.statements, enclosing, threads);
                }
                enclosing.pop();
                for ((_, flag), dirty) in enclosing.iter_mut().zip(dirty) {
                    *flag = dirty;
                }
            }
            other => {
                let breakable = breakable(other);
                enclosing.extend(breakable.map(|id| (id, false)));
                for list in jump_lists(other) {
                    scan_threads(list, enclosing, threads);
                }
                if breakable.is_some() {
                    enclosing.pop();
                }
            }
        }
    }
}

fn thread(
    body: &mut Body,
    enclosing: &mut Vec<BindingId>,
    threads: &HashMap<BindingId, BindingId>,
    gotos: &mut HashMap<BindingId, usize>,
) {
    for statement in body {
        if let Statement::Goto(label) = statement.value
            && let Some(&target) = threads.get(&label)
            && enclosing.contains(&target)
        {
            statement.value = Statement::Break(target);
            if let Some(count) = gotos.get_mut(&label) {
                *count -= 1;
            }
            continue;
        }
        let breakable = breakable(&statement.value);
        let is_for = matches!(statement.value, Statement::For { .. });
        let mut lists = children(&mut statement.value).into_iter();
        if is_for && let Some(init) = lists.next() {
            thread(init, enclosing, threads, gotos);
        }
        enclosing.extend(breakable);
        for list in lists {
            thread(list, enclosing, threads, gotos);
        }
        for evaluation in evaluations(&mut statement.value) {
            thread(&mut evaluation.statements, enclosing, threads, gotos);
        }
        if breakable.is_some() {
            enclosing.pop();
        }
    }
}

fn breakable(statement: &Statement) -> Option<BindingId> {
    match statement {
        Statement::For { id, .. }
        | Statement::While { id, .. }
        | Statement::DoWhile { id, .. }
        | Statement::Switch { id, .. }
        | Statement::LabeledBlock { id, .. } => Some(*id),
        #[expect(
            slate::sensitive_wildcard,
            reason = "only loops, switches and labeled blocks are break targets"
        )]
        _ => None,
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

fn sites(range: &[Span<Statement>], label: BindingId) -> (Vec<usize>, usize) {
    let mut indices = Vec::new();
    let mut total = 0;
    for (index, statement) in range.iter().enumerate() {
        let count = jumps(statement, label);
        if count != 0 {
            indices.push(index);
            total += count;
        }
    }
    (indices, total)
}

fn condition_prefix(range: &[Span<Statement>], condition: &Value) -> usize {
    let mut reads = Reads::default();
    walk_value(&mut condition.clone(), &mut reads);
    let mut split = range.len();
    loop {
        let declared = range[..split]
            .iter()
            .position(|statement| match &statement.value {
                Statement::Let(variable) => reads.0.contains(&variable.id),
                Statement::Temporary { id, .. } => reads.0.contains(id),
                _ => false,
            });
        let Some(declared) = declared else {
            return split;
        };
        walk_body(&mut range[declared..split].to_vec(), &mut reads);
        split = declared;
    }
}

#[derive(Default)]
struct Reads(HashSet<BindingId>);

impl Visitor for Reads {
    fn binding(&mut self, binding: BindingId) {
        self.0.insert(binding);
    }
}

fn only_goto(body: &[Span<Statement>], label: BindingId) -> bool {
    matches!(body, [single] if matches!(single.value, Statement::Goto(id) if id == label))
}

fn ends_in_jump(statement: &Statement) -> bool {
    matches!(
        statement,
        Statement::Return(_)
            | Statement::Break(_)
            | Statement::Continue(_)
            | Statement::Goto(_)
            | Statement::ComputedGoto(_)
    )
}

fn retarget(statement: &mut Span<Statement>, label: BindingId, jump: &Statement) {
    if matches!(statement.value, Statement::Goto(id) if id == label) {
        statement.value = jump.clone();
        return;
    }
    for child in children(&mut statement.value) {
        for statement in child {
            retarget(statement, label, jump);
        }
    }
    for evaluation in evaluations(&mut statement.value) {
        for statement in &mut evaluation.statements {
            retarget(statement, label, jump);
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
