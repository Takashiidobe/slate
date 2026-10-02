use super::*;

type Statement = Span<ir::Statement>;

enum Node<'a> {
    End,
    Jump(usize),
    Statement(&'a Statement, Option<usize>),
    Value(&'a ir::Value, usize),
    Branch(&'a ir::Value, usize, usize),
    Switch(&'a ir::Value, Vec<Case<'a>>, usize),
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
    locals: Vec<(BindingId, &'a ir::Type)>,
}

fn children(statement: &ir::Statement) -> Vec<&[Statement]> {
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
            ir::Statement::Goto(_) | ir::Statement::Label { .. }
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
                    body.first().is_some_and(|first| {
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
                self.locals.push((*id, ty));
                self.push(Node::Statement(statement, Some(next)))
            }
            ir::Statement::Let(variable) => {
                self.locals.push((variable.id, &variable.ty));
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
        let end = graph.push(Node::End);
        let entry = graph.list(statements, end);
        self.dispatch_bindings
            .extend(graph.locals.iter().map(|(id, _)| *id));
        let mut lowered = Vec::new();
        for (id, ty) in graph.locals {
            let ty = self.lower_type(ty)?;
            let storage = format!("__slate_storage_{}", id.0);
            lowered.push(Stmt::Let {
                name: storage.clone(),
                mutable: true,
                ty: Some(rust::Type::Generic {
                    name: "std::mem::MaybeUninit".into(),
                    args: vec![ty.clone()],
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
        for (index, node) in graph.nodes.into_iter().enumerate() {
            let body = match node {
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
                    if let Some(next) = next {
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
                    body.extend(jump(next));
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
            };
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
