use super::*;

type Statement = Span<ir::Statement>;

enum Node<'a> {
    End,
    Jump(usize),
    Statement(&'a Statement, Option<usize>),
    Value(&'a ir::Value, usize),
    Branch(&'a ir::Value, usize, usize),
    Switch(&'a ir::Value, Vec<Case<'a>>, usize),
    Indirect(&'a ir::Value, Vec<(BindingId, usize)>),
}

impl Node<'_> {
    fn successors(&self) -> Vec<usize> {
        match self {
            Self::End | Self::Statement(_, None) => Vec::new(),
            Self::Jump(next) | Self::Statement(_, Some(next)) | Self::Value(_, next) => {
                vec![*next]
            }
            Self::Branch(_, yes, no) => vec![*yes, *no],
            Self::Switch(_, cases, default) => cases
                .iter()
                .map(|case| case.entry)
                .chain([*default])
                .collect(),
            Self::Indirect(_, targets) => targets.iter().map(|(_, entry)| *entry).collect(),
        }
    }

    fn retarget(&mut self, targets: &[usize]) {
        match self {
            Self::End | Self::Statement(_, None) => {}
            Self::Jump(next) | Self::Statement(_, Some(next)) | Self::Value(_, next) => {
                *next = targets[*next];
            }
            Self::Branch(_, yes, no) => {
                *yes = targets[*yes];
                *no = targets[*no];
            }
            Self::Switch(_, cases, default) => {
                for case in cases {
                    case.entry = targets[case.entry];
                }
                *default = targets[*default];
            }
            Self::Indirect(_, entries) => {
                for (_, entry) in entries {
                    *entry = targets[*entry];
                }
            }
        }
    }
}

struct Case<'a> {
    start: &'a ir::Value,
    end: Option<&'a ir::Value>,
    entry: usize,
}

#[derive(Default)]
struct Graph<'a> {
    nodes: Vec<Node<'a>>,
    labels: HashMap<BindingId, usize>,
    breaks: HashMap<BindingId, usize>,
    continues: HashMap<BindingId, usize>,
    cases: HashMap<BindingId, Vec<Case<'a>>>,
    defaults: HashMap<BindingId, usize>,
    locals: Vec<(BindingId, &'a ir::Type, Option<u64>)>,
    targets: Vec<BindingId>,
}

pub(super) fn label_code(id: BindingId) -> usize {
    id.0 as usize + 1
}

fn collect_labels(statements: &[Statement], labels: &mut Vec<BindingId>) {
    for statement in statements {
        if let ir::Statement::Label { id, .. } = statement.value {
            labels.push(id);
        }
        for body in children(&statement.value) {
            collect_labels(body, labels);
        }
    }
}

pub(super) fn children(statement: &ir::Statement) -> Vec<&[Statement]> {
    match statement {
        ir::Statement::Block(body)
        | ir::Statement::Switch { body, .. }
        | ir::Statement::Case { body, .. }
        | ir::Statement::Default { body, .. }
        | ir::Statement::Label { body, .. } => vec![body],
        ir::Statement::If {
            then_body,
            else_body,
            ..
        } => {
            vec![then_body, else_body.as_deref().unwrap_or_default()]
        }
        ir::Statement::While {
            condition, body, ..
        }
        | ir::Statement::DoWhile {
            condition, body, ..
        } => vec![&condition.statements, body],
        ir::Statement::For {
            init,
            condition,
            increment,
            body,
            ..
        } => vec![
            init,
            condition
                .as_ref()
                .map(|e| e.statements.as_slice())
                .unwrap_or_default(),
            increment
                .as_ref()
                .map(|e| e.statements.as_slice())
                .unwrap_or_default(),
            body,
        ],
        _ => Vec::new(),
    }
}

pub(super) fn needs_dispatch(statements: &[Statement]) -> bool {
    statements.iter().any(|statement| {
        matches!(
            statement.value,
            ir::Statement::Goto(_) | ir::Statement::ComputedGoto(_) | ir::Statement::Label { .. }
        ) || children(statement).into_iter().any(needs_dispatch)
            || match &statement.value {
                ir::Statement::Switch { body, .. } => {
                    let body = match body.as_slice() {
                        [single] => match &single.value {
                            ir::Statement::Block(inner) => inner.as_slice(),
                            _ => body,
                        },
                        _ => body,
                    };
                    body.iter()
                        .find(|statement| !matches!(statement.value, ir::Statement::Comment(_)))
                        .is_some_and(|first| {
                            !matches!(
                                first.value,
                                ir::Statement::Case { .. } | ir::Statement::Default { .. }
                            )
                        })
                }
                _ => false,
            }
    })
}

impl<'a> Graph<'a> {
    fn blocks(&mut self, entry: usize) -> (usize, Vec<Vec<Node<'a>>>) {
        let count = self.nodes.len();
        let mut targets = vec![usize::MAX; count];
        let mut visiting = vec![false; count];
        for start in 0..count {
            if targets[start] != usize::MAX {
                continue;
            }
            let mut path = Vec::new();
            let mut current = start;
            while targets[current] == usize::MAX && !visiting[current] {
                let Node::Jump(next) = self.nodes[current] else {
                    targets[current] = current;
                    break;
                };
                visiting[current] = true;
                path.push(current);
                current = next;
            }
            let target = if targets[current] == usize::MAX {
                current
            } else {
                targets[current]
            };
            for index in path {
                targets[index] = target;
                visiting[index] = false;
            }
        }
        let entry = targets[entry];
        for node in &mut self.nodes {
            node.retarget(&targets);
        }
        let mut reachable = vec![false; count];
        let mut predecessors = vec![0; count];
        let mut leaders = vec![false; count];
        leaders[entry] = true;
        let mut stack = vec![entry];
        while let Some(index) = stack.pop() {
            if reachable[index] {
                continue;
            }
            reachable[index] = true;
            let node = &self.nodes[index];
            let mut successors = node.successors();
            successors.sort_unstable();
            successors.dedup();
            for next in successors {
                predecessors[next] += 1;
                if matches!(
                    node,
                    Node::Branch(..) | Node::Switch(..) | Node::Indirect(..)
                ) {
                    leaders[next] = true;
                }
                stack.push(next);
            }
        }
        for (index, predecessors) in predecessors.into_iter().enumerate() {
            leaders[index] |= predecessors != 1;
        }
        let mut blocks = Vec::new();
        let mut states = vec![usize::MAX; count];
        for start in 0..count {
            if !reachable[start] || !leaders[start] {
                continue;
            }
            let mut block = Vec::new();
            let mut current = start;
            loop {
                states[current] = blocks.len();
                block.push(current);
                let next = match self.nodes[current] {
                    Node::Jump(next) | Node::Statement(_, Some(next)) | Node::Value(_, next) => {
                        next
                    }
                    _ => break,
                };
                if leaders[next] {
                    break;
                }
                current = next;
            }
            blocks.push(block);
        }
        let mut nodes: Vec<_> = std::mem::take(&mut self.nodes)
            .into_iter()
            .map(Some)
            .collect();
        let blocks = blocks
            .into_iter()
            .map(|block| {
                block
                    .into_iter()
                    .map(|index| {
                        let mut node = nodes[index].take().unwrap();
                        node.retarget(&states);
                        node
                    })
                    .collect()
            })
            .collect();
        (states[entry], blocks)
    }

    fn push(&mut self, node: Node<'a>) -> usize {
        let index = self.nodes.len();
        self.nodes.push(node);
        index
    }

    fn label(&mut self, id: BindingId) -> usize {
        if let Some(index) = self.labels.get(&id) {
            return *index;
        }
        let index = self.push(Node::End);
        self.labels.insert(id, index);
        index
    }

    fn list(&mut self, statements: &'a [Statement], mut next: usize) -> usize {
        for statement in statements.iter().rev() {
            next = self.statement(statement, next);
        }
        next
    }

    fn evaluation(&mut self, evaluation: &'a ir::Evaluation, next: usize) -> usize {
        let value = self.push(Node::Value(&evaluation.value, next));
        self.list(&evaluation.statements, value)
    }

    fn statement(&mut self, statement: &'a Statement, next: usize) -> usize {
        match &statement.value {
            ir::Statement::Block(body) => self.list(body, next),
            ir::Statement::Label { id, body, .. } => {
                let entry = self.list(body, next);
                let label = self.label(*id);
                self.nodes[label] = Node::Jump(entry);
                label
            }
            ir::Statement::Goto(id) => self.label(*id),
            ir::Statement::ComputedGoto(target) => {
                let targets = self
                    .targets
                    .clone()
                    .into_iter()
                    .map(|id| (id, self.label(id)))
                    .collect();
                self.push(Node::Indirect(target, targets))
            }
            ir::Statement::Break(id) => self.breaks[id],
            ir::Statement::Continue(id) => self.continues[id],
            ir::Statement::If {
                condition,
                then_body,
                else_body,
            } => {
                let yes = self.list(then_body, next);
                let no = self.list(else_body.as_deref().unwrap_or_default(), next);
                self.push(Node::Branch(condition, yes, no))
            }
            ir::Statement::While {
                id,
                condition,
                body,
            }
            | ir::Statement::DoWhile {
                id,
                condition,
                body,
            } => {
                let branch = self.push(Node::End);
                let test = self.list(&condition.statements, branch);
                self.breaks.insert(*id, next);
                self.continues.insert(*id, test);
                let entry = self.list(body, test);
                self.nodes[branch] = Node::Branch(&condition.value, entry, next);
                if matches!(statement.value, ir::Statement::DoWhile { .. }) {
                    entry
                } else {
                    test
                }
            }
            ir::Statement::For {
                id,
                init,
                condition,
                increment,
                body,
            } => {
                let branch = self.push(Node::End);
                let test = match condition {
                    Some(condition) => self.list(&condition.statements, branch),
                    None => branch,
                };
                let step = match increment {
                    Some(increment) => self.evaluation(increment, test),
                    None => test,
                };
                self.breaks.insert(*id, next);
                self.continues.insert(*id, step);
                let entry = self.list(body, step);
                self.nodes[branch] = match condition {
                    Some(condition) => Node::Branch(&condition.value, entry, next),
                    None => Node::Jump(entry),
                };
                self.list(init, test)
            }
            ir::Statement::Switch {
                id,
                discriminant,
                body,
            } => {
                self.breaks.insert(*id, next);
                self.list(body, next);
                let cases = self.cases.remove(id).unwrap_or_default();
                let default = self.defaults.remove(id).unwrap_or(next);
                self.push(Node::Switch(discriminant, cases, default))
            }
            ir::Statement::Case {
                switch,
                start,
                end,
                body,
            } => {
                let entry = self.list(body, next);
                self.cases.entry(*switch).or_default().push(Case {
                    start,
                    end: end.as_ref(),
                    entry,
                });
                entry
            }
            ir::Statement::Default { switch, body } => {
                let entry = self.list(body, next);
                self.defaults.insert(*switch, entry);
                entry
            }
            ir::Statement::Temporary { id, ty, .. } => {
                self.locals.push((*id, ty, None));
                self.push(Node::Statement(statement, Some(next)))
            }
            ir::Statement::Let(variable) => {
                self.locals
                    .push((variable.id, &variable.ty, variable.alignment));
                self.push(Node::Statement(statement, Some(next)))
            }
            ir::Statement::Return(_) => self.push(Node::Statement(statement, None)),
            _ => self.push(Node::Statement(statement, Some(next))),
        }
    }
}

fn state(index: usize) -> Expr {
    Expr::Value(rust::RustValue::Usize(index))
}

fn jump(index: usize) -> Vec<Stmt> {
    vec![
        Stmt::Assign {
            target: Expr::Var("__slate_state".into()),
            value: state(index),
        },
        Stmt::Continue(Some(rust::Label::new("__slate_dispatch"))),
    ]
}

fn slot_name(id: BindingId) -> String {
    format!("__slate_slot_{}", id.0)
}

pub(super) fn slot_pointer(id: BindingId) -> Expr {
    Expr::Var(slot_name(id).into())
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn lower_dispatch(&mut self, statements: &[Statement]) -> Result<Vec<Stmt>> {
        let mut graph = Graph::default();
        collect_labels(statements, &mut graph.targets);
        let end = graph.push(Node::End);
        let entry = graph.list(statements, end);
        let (entry, blocks) = graph.blocks(entry);
        self.dispatch_bindings
            .extend(graph.locals.iter().map(|(id, ..)| *id));
        let mut lowered = Vec::new();
        for (id, ir_ty, alignment) in graph.locals {
            if self.tables.variably_modified(ir_ty) {
                return Err(unsupported_type(ir_ty));
            }
            let ty = self.lower_type(ir_ty)?;
            let stored = match self.tables.over_alignment(ir_ty, alignment) {
                Some(alignment) => {
                    let name = binding_name(id, &self.tables.bindings);
                    self.align_wrapped(&name, alignment, ty.clone())?
                }
                None => ty.clone(),
            };
            let storage = format!("__slate_storage_{}", id.0);
            lowered.push(Stmt::Let {
                name: storage.clone(),
                mutable: true,
                ty: Some(rust::Type::Generic {
                    name: "std::mem::MaybeUninit".into(),
                    args: vec![stored],
                }),
                init: Some(Expr::Call {
                    func: Box::new(Expr::Var("std::mem::MaybeUninit::uninit".into())),
                    args: Vec::new(),
                    binding: CallBinding::Generated,
                }),
            });
            let pointer = rust::Type::Ptr {
                mutable: true,
                inner: Box::new(ty),
            };
            lowered.push(Stmt::Let {
                name: slot_name(id),
                mutable: false,
                ty: Some(pointer.clone()),
                init: Some(Expr::Cast {
                    expr: Box::new(Expr::AddrOf {
                        mutable: true,
                        expr: Box::new(Expr::Var(storage.into())),
                    }),
                    ty: pointer,
                }),
            });
        }
        let mut arms = Vec::new();
        for (index, block) in blocks.into_iter().enumerate() {
            let last = block.len() - 1;
            let mut body = Vec::new();
            for (position, node) in block.into_iter().enumerate() {
                let terminal = position == last;
                body.extend(match node {
                    Node::End => vec![Stmt::Break(Some(rust::Label::new("__slate_dispatch")))],
                    Node::Jump(next) => jump(next),
                    Node::Statement(statement, next) => {
                        let local = match &statement.value {
                            ir::Statement::Temporary {
                                id, initializer, ..
                            } => Some((*id, initializer.as_ref())),
                            ir::Statement::Let(variable) => {
                                Some((variable.id, variable.initializer.as_ref()))
                            }
                            _ => None,
                        };
                        let mut body = match local {
                            Some((id, Some(initializer))) => vec![Stmt::Expr(Expr::Call {
                                func: Box::new(Expr::Var("std::ptr::write".into())),
                                args: vec![slot_pointer(id), self.lower_value(initializer)?],
                                binding: CallBinding::Generated,
                            })],
                            Some((_, None)) => Vec::new(),
                            None => vec![self.lower_statement(statement)?],
                        };
                        if let Some(next) = next.filter(|_| terminal) {
                            body.extend(jump(next));
                        }
                        body
                    }
                    Node::Value(value, next) => {
                        let mut body = if matches!(value.node.value, ValueKind::Void) {
                            Vec::new()
                        } else {
                            vec![Stmt::Expr(self.lower_value(value)?)]
                        };
                        if terminal {
                            body.extend(jump(next));
                        }
                        body
                    }
                    Node::Branch(condition, yes, no) => vec![Stmt::If {
                        cond: self.lower_condition(condition)?,
                        then_body: jump(yes),
                        else_body: jump(no),
                    }],
                    Node::Switch(value, cases, default) => {
                        let name = self.next_temp();
                        let mut dispatch = jump(default);
                        for case in cases {
                            let discriminant = Expr::Var(name.as_str().into());
                            let start = self.lower_value(case.start)?;
                            let cond = match case.end {
                                Some(end) => Expr::Binary {
                                    op: BinOp::And,
                                    lhs: Box::new(Expr::Binary {
                                        op: BinOp::Ge,
                                        lhs: Box::new(discriminant.clone()),
                                        rhs: Box::new(start),
                                    }),
                                    rhs: Box::new(Expr::Binary {
                                        op: BinOp::Le,
                                        lhs: Box::new(discriminant),
                                        rhs: Box::new(self.lower_value(end)?),
                                    }),
                                },
                                None => Expr::Binary {
                                    op: BinOp::Eq,
                                    lhs: Box::new(discriminant),
                                    rhs: Box::new(start),
                                },
                            };
                            dispatch = vec![Stmt::If {
                                cond,
                                then_body: jump(case.entry),
                                else_body: dispatch,
                            }];
                        }
                        let mut body = vec![Stmt::Let {
                            name,
                            mutable: false,
                            ty: Some(self.lower_type(&value.ty)?),
                            init: Some(self.lower_value(value)?),
                        }];
                        body.extend(dispatch);
                        body
                    }
                    Node::Indirect(target, entries) => {
                        let mut arms: Vec<_> = entries
                            .into_iter()
                            .map(|(id, entry)| rust::MatchArm {
                                pattern: rust::Pattern::I64(label_code(id) as i64),
                                body: jump(entry),
                            })
                            .collect();
                        arms.push(rust::MatchArm {
                            pattern: rust::Pattern::Wildcard,
                            body: vec![Stmt::Expr(Expr::Macro {
                                name: "unreachable".into(),
                                args: Vec::new(),
                            })],
                        });
                        vec![Stmt::Match {
                            expr: Expr::Cast {
                                expr: Box::new(self.lower_value(target)?),
                                ty: rust::Type::Prim(Prim::Usize),
                            },
                            arms,
                        }]
                    }
                });
            }
            arms.push(rust::MatchArm {
                pattern: rust::Pattern::I64(index as i64),
                body,
            });
        }
        arms.push(rust::MatchArm {
            pattern: rust::Pattern::Wildcard,
            body: vec![Stmt::Expr(Expr::Macro {
                name: "unreachable".into(),
                args: Vec::new(),
            })],
        });
        lowered.push(Stmt::Let {
            name: "__slate_state".into(),
            mutable: true,
            ty: Some(rust::Type::Prim(Prim::Usize)),
            init: Some(state(entry)),
        });
        lowered.push(Stmt::Unsafe {
            body: rust::Block {
                stmts: vec![Stmt::Loop {
                    label: Some(rust::Label::new("__slate_dispatch")),
                    body: vec![Stmt::Match {
                        expr: Expr::Var("__slate_state".into()),
                        arms,
                    }],
                }],
                tail: None,
            },
        });
        Ok(lowered)
    }
}
