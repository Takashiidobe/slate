use foldhash::{HashMap, HashSet};
use slate_parser::ast::{NodeId, Span};
use slate_parser::ir::{
    AsmPiece, BindingId, Evaluation, Module, Place, PlaceKind, Statement, StorageDuration, Type,
    Value, ValueKind, Variable, Visitor, walk_body, walk_value,
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
        let mut forwards = Forwards::default();
        walk_body(body, &mut forwards);
        let resolved = forwards.resolve();
        if !resolved.is_empty() {
            forward(body, &resolved, &mut survey.gotos);
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

#[derive(Default)]
struct Forwards(HashMap<BindingId, BindingId>);

impl Visitor for Forwards {
    fn statement(&mut self, statement: &Statement) {
        let Statement::Label { id, body, .. } = statement else {
            return;
        };
        let mut first = body.first();
        while let Some(Statement::Label { body, .. }) = first.map(|first| &first.value) {
            first = body.first();
        }
        if let Some(Statement::Goto(target)) = first.map(|first| &first.value)
            && target != id
        {
            self.0.insert(*id, *target);
        }
    }
}

impl Forwards {
    fn resolve(&self) -> HashMap<BindingId, BindingId> {
        let mut resolved = HashMap::default();
        for &label in self.0.keys() {
            let mut seen = HashSet::default();
            let mut target = label;
            while let Some(&next) = self.0.get(&target) {
                if !seen.insert(target) {
                    break;
                }
                target = next;
            }
            if !self.0.contains_key(&target) {
                resolved.insert(label, target);
            }
        }
        resolved
    }
}

fn forward(
    body: &mut Body,
    resolved: &HashMap<BindingId, BindingId>,
    gotos: &mut HashMap<BindingId, usize>,
) {
    for statement in body {
        if let Statement::Goto(label) = statement.value
            && let Some(&target) = resolved.get(&label)
        {
            statement.value = Statement::Goto(target);
            if let Some(count) = gotos.get_mut(&label) {
                *count -= 1;
            }
            *gotos.entry(target).or_default() += 1;
            continue;
        }
        for child in children(&mut statement.value) {
            forward(child, resolved, gotos);
        }
        for evaluation in evaluations(&mut statement.value) {
            forward(&mut evaluation.statements, resolved, gotos);
        }
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
        self.lift(body);
        self.labels(body);
    }

    fn lift(&mut self, body: &mut Body) {
        let mut index = 0;
        while index < body.len() {
            if !self.lift_at(body, index) {
                index += 1;
            }
        }
    }

    fn escaping(&self, list: &[Span<Statement>]) -> Option<usize> {
        list.iter().position(|statement| {
            matches!(statement.value, Statement::Label { id, .. }
                if self.gotos.get(&id).copied().unwrap_or(0)
                    > list.iter().map(|statement| jumps(statement, id)).sum())
        })
    }

    fn lift_at(&mut self, body: &mut Body, index: usize) -> bool {
        let container = &body[index];
        let (spelling, expansion) = (container.spelling, container.expansion);
        match &container.value {
            Statement::Block(inner) => {
                let Some(split) = self.escaping(inner) else {
                    return false;
                };
                let Some(hoisted) = movable(&inner[..split], &inner[split..]) else {
                    return false;
                };
                let Statement::Block(inner) = &mut body[index].value else {
                    return false;
                };
                let tail = scoped(inner.split_off(split));
                let mut declarations = Vec::new();
                *inner = extract(std::mem::take(inner), &hoisted, &mut declarations);
                if inner.is_empty() {
                    self.retired.push(body[index].id);
                    body.splice(index..=index, tail);
                } else {
                    body.splice(index + 1..index + 1, tail);
                }
                body.splice(index..index, declarations);
                true
            }
            Statement::If {
                then_body,
                else_body,
                folded: None,
                ..
            } => {
                let (in_then, split) = match self.escaping(then_body) {
                    Some(split) => (true, split),
                    None => match else_body.as_ref().and_then(|arm| self.escaping(arm)) {
                        Some(split) => (false, split),
                        None => return false,
                    },
                };
                let arm = if in_then {
                    then_body
                } else {
                    else_body.as_ref().unwrap_or(then_body)
                };
                let Statement::Label { id, .. } = arm[split].value else {
                    return false;
                };
                if body[index + 1..]
                    .iter()
                    .any(|statement| jumps(statement, id) != 0)
                {
                    return false;
                }
                let Some(hoisted) = movable(&arm[..split], &arm[split..]) else {
                    return false;
                };
                let Statement::If {
                    then_body,
                    else_body,
                    ..
                } = &mut body[index].value
                else {
                    return false;
                };
                let (arm, other) = if in_then {
                    (then_body, else_body.get_or_insert_with(Vec::new))
                } else {
                    let Some(arm) = else_body.as_mut() else {
                        return false;
                    };
                    (arm, then_body)
                };
                let mut tail = scoped(arm.split_off(split));
                let mut declarations = Vec::new();
                *arm = extract(std::mem::take(arm), &hoisted, &mut declarations);
                if !other.last().is_some_and(|last| ends_in_jump(&last.value)) {
                    let join = self.module.fresh_binding();
                    other.push(Span::new(Statement::Goto(join), spelling, expansion));
                    self.gotos.insert(join, 1);
                    tail.push(Span::new(
                        Statement::Label {
                            id: join,
                            name: "join".to_owned(),
                            body: Vec::new(),
                        },
                        spelling,
                        expansion,
                    ));
                }
                if let Statement::If { else_body, .. } = &mut body[index].value
                    && else_body.as_ref().is_some_and(Vec::is_empty)
                {
                    *else_body = None;
                }
                body.splice(index + 1..index + 1, tail);
                body.splice(index..index, declarations);
                true
            }
            #[expect(
                slate::sensitive_wildcard,
                reason = "labels move out of plain blocks and if arms only"
            )]
            _ => false,
        }
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
        let mut label = label;
        let mut end = backward.last().map(|last| label + last + 1);
        let mut start = forward.first().copied();
        let looped = match end {
            Some(end) => Some(movable(&body[label..end], &body[end..])?),
            None => None,
        };
        let blocked = match start {
            Some(start) => Some(movable(&body[start..label], &body[label..])?),
            None => None,
        };
        if let (Some(first), Some(hoisted)) = (start, &blocked) {
            let (first, moved) = hoist(body, first, label, hoisted);
            end = end.map(|end| end + moved - label);
            label = moved;
            start = Some(first);
        }
        let block_end = label;
        if let (Some(last), Some(hoisted)) = (end, &looped) {
            let (first, last) = hoist(body, label, last, hoisted);
            label = first;
            end = Some(last);
        }
        self.gotos.remove(&id);
        if let Some(end) = end {
            self.wrap_loop(body, label, end, id, behind);
        }
        let position = match start {
            Some(start) => {
                self.wrap_block(body, start, block_end, id);
                self.revisit(&mut body[start]);
                label - (block_end - start) + 1
            }
            None => label,
        };
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

pub(super) fn jump_lists(statement: &Statement) -> Vec<&Body> {
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

fn movable(range: &[Span<Statement>], rest: &[Span<Statement>]) -> Option<HashSet<BindingId>> {
    let mut declared = HashMap::default();
    if !declarations(range, &mut declared) {
        return None;
    }
    let mut switches = Switches::default();
    let mut range = range.to_vec();
    walk_body(&mut range, &mut switches);
    if switches.cases.difference(&switches.owned).next().is_some() {
        return None;
    }
    let mut uses = Uses {
        declared: &declared,
        used: HashSet::default(),
    };
    let mut rest = rest.to_vec();
    walk_body(&mut rest, &mut uses);
    uses.used
        .iter()
        .all(|id| declared.get(id).copied().unwrap_or(false))
        .then_some(uses.used)
}

fn hoistable(variable: &Variable) -> bool {
    if variable.storage != StorageDuration::Automatic {
        return true;
    }
    !variable.is_const
        && !variable.constexpr
        && !variable.byref
        && (variable.initializer.is_none()
            || matches!(
                variable.ty,
                Type::Bool | Type::Numeric(_) | Type::Pointer { .. }
            ))
}

fn hoist(
    body: &mut Body,
    start: usize,
    end: usize,
    hoisted: &HashSet<BindingId>,
) -> (usize, usize) {
    if hoisted.is_empty() {
        return (start, end);
    }
    let mut declarations = Vec::new();
    let range = extract(body.drain(start..end).collect(), hoisted, &mut declarations);
    let (count, length) = (declarations.len(), range.len());
    body.splice(start..start, declarations.into_iter().chain(range));
    (start + count, start + count + length)
}

fn extract(range: Body, hoisted: &HashSet<BindingId>, declarations: &mut Body) -> Body {
    let mut kept = Vec::with_capacity(range.len());
    for mut statement in range {
        match &mut statement.value {
            Statement::Let(variable) if hoisted.contains(&variable.id) => {
                if variable.storage == StorageDuration::Automatic
                    && let Some(value) = variable.initializer.take()
                {
                    let place = Place {
                        ty: variable.ty.clone(),
                        kind: PlaceKind::Binding(variable.id),
                        access: variable.access,
                    };
                    kept.push(Span::new(
                        Statement::Write {
                            place,
                            value,
                            ordering: None,
                            unsequenced: false,
                        },
                        statement.spelling,
                        statement.expansion,
                    ));
                }
                declarations.push(statement);
                continue;
            }
            Statement::Label { body, .. } => {
                *body = extract(std::mem::take(body), hoisted, declarations);
            }
            _ => {}
        }
        kept.push(statement);
    }
    kept
}

fn scoped(mut tail: Body) -> Body {
    let labeled = match tail.first() {
        Some(Span {
            value: Statement::Label { body, .. },
            ..
        }) => body.as_slice(),
        _ => &[],
    };
    let scoped = tail.iter().chain(labeled).any(|statement| {
        matches!(&statement.value, Statement::Let(variable)
            if variable.cleanup.is_some() || variable.ty.is_variably_modified())
    });
    if !scoped {
        return tail;
    }
    let rest: Body = tail.drain(1..).collect();
    if let Some(Span {
        value: Statement::Label { body, .. },
        spelling,
        expansion,
        ..
    }) = tail.first_mut()
    {
        let mut inner = std::mem::take(body);
        inner.extend(rest);
        *body = vec![Span::new(Statement::Block(inner), *spelling, *expansion)];
    }
    tail
}

fn declarations(range: &[Span<Statement>], declared: &mut HashMap<BindingId, bool>) -> bool {
    for statement in range {
        match &statement.value {
            Statement::Let(variable) => {
                if variable.cleanup.is_some() || variable.ty.is_variably_modified() {
                    return false;
                }
                declared.insert(variable.id, hoistable(variable));
            }
            Statement::Temporary { id, .. } => {
                declared.insert(*id, false);
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
    declared: &'a HashMap<BindingId, bool>,
    used: HashSet<BindingId>,
}

impl Uses<'_> {
    fn note(&mut self, binding: BindingId) {
        if self.declared.contains_key(&binding) {
            self.used.insert(binding);
        }
    }
}

impl Visitor for Uses<'_> {
    fn binding(&mut self, binding: BindingId) {
        self.note(binding);
    }

    fn statement(&mut self, statement: &Statement) {
        if let Statement::NestedFunction(nested) = statement {
            for capture in &nested.captures {
                self.note(capture.id);
            }
        }
    }

    fn value_kind(&mut self, kind: &ValueKind) {
        if let ValueKind::Block(block) = kind {
            for capture in &block.captures {
                self.note(capture.id);
            }
        }
    }
}
