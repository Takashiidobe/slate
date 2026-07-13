use crate::rust_ast::{Block, IndentStmt, Item, Program, Stmt};

#[derive(Debug, Clone)]
pub(super) struct AnalyzedProgram {
    pub(super) program: Program,
    pub(super) facts: FixupFacts,
}

#[derive(Debug, Default, Clone)]
pub(super) struct FixupFacts {
    pub(super) functions: Vec<FunctionFact>,
    pub(super) bindings: Vec<BindingFact>,
    pub(super) loops: Vec<LoopFact>,
    pub(super) relations: Vec<FactRelation>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct FunctionId(pub(super) usize);

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct BindingId(pub(super) usize);

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct LoopId(pub(super) usize);

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct FunctionFact {
    pub(super) id: FunctionId,
    pub(super) name: String,
    pub(super) item_index: usize,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct BindingFact {
    pub(super) id: BindingId,
    pub(super) function: FunctionId,
    pub(super) name: String,
    pub(super) kind: BindingKind,
    pub(super) path: AstPath,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) enum BindingKind {
    Param { index: usize },
    Local,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct LoopFact {
    pub(super) id: LoopId,
    pub(super) function: FunctionId,
    pub(super) kind: LoopKind,
    pub(super) path: AstPath,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) enum LoopKind {
    Loop,
    While,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct FactRelation {
    pub(super) kind: RelationKind,
    pub(super) members: Vec<SemanticId>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) enum RelationKind {
    Supersedes,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum SemanticId {
    Function(FunctionId),
    Binding(BindingId),
    Loop(LoopId),
}

#[derive(Debug, Default, Clone, PartialEq, Eq)]
pub(super) struct AstPath(pub(super) Vec<PathSegment>);

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) enum PathSegment {
    Stmt(usize),
    Then,
    Else,
    LoopBody,
    ScopeBody,
    LabeledBody,
    MatchArm(usize),
    UnsafeBody,
    WhileBody,
    BlockBody,
}

pub(super) fn analyze(program: Program) -> AnalyzedProgram {
    let mut collector = Collector::default();
    collector.program(&program);
    AnalyzedProgram {
        program,
        facts: collector.facts,
    }
}

#[derive(Default)]
struct Collector {
    facts: FixupFacts,
}

impl Collector {
    fn program(&mut self, program: &Program) {
        for (item_index, item) in program.items.iter().enumerate() {
            let Item::Fn(f) = item else {
                continue;
            };
            let function = self.push_function(f.name.clone(), item_index);
            for (index, param) in f.params.iter().enumerate() {
                self.push_binding(
                    function,
                    param.name.clone(),
                    BindingKind::Param { index },
                    AstPath::default(),
                );
            }
            self.body(function, &f.body, &mut Vec::new());
        }
    }

    fn push_function(&mut self, name: String, item_index: usize) -> FunctionId {
        let id = FunctionId(self.facts.functions.len());
        self.facts.functions.push(FunctionFact {
            id,
            name,
            item_index,
        });
        id
    }

    fn push_binding(
        &mut self,
        function: FunctionId,
        name: String,
        kind: BindingKind,
        path: AstPath,
    ) -> BindingId {
        let id = BindingId(self.facts.bindings.len());
        self.facts.bindings.push(BindingFact {
            id,
            function,
            name,
            kind,
            path,
        });
        id
    }

    fn push_loop(&mut self, function: FunctionId, kind: LoopKind, path: AstPath) -> LoopId {
        let id = LoopId(self.facts.loops.len());
        self.facts.loops.push(LoopFact {
            id,
            function,
            kind,
            path,
        });
        id
    }

    fn body(&mut self, function: FunctionId, body: &[IndentStmt], path: &mut Vec<PathSegment>) {
        for (index, indent) in body.iter().enumerate() {
            path.push(PathSegment::Stmt(index));
            self.stmt(function, &indent.stmt, path);
            path.pop();
        }
    }

    fn block(&mut self, function: FunctionId, block: &Block, path: &mut Vec<PathSegment>) {
        self.body(function, &block.stmts, path);
    }

    fn stmt(&mut self, function: FunctionId, stmt: &Stmt, path: &mut Vec<PathSegment>) {
        match stmt {
            Stmt::Let { name, .. } => {
                self.push_binding(
                    function,
                    name.clone(),
                    BindingKind::Local,
                    AstPath(path.clone()),
                );
            }
            Stmt::LetIf {
                name,
                then_body,
                else_body,
                ..
            } => {
                self.push_binding(
                    function,
                    name.clone(),
                    BindingKind::Local,
                    AstPath(path.clone()),
                );
                path.push(PathSegment::Then);
                self.body(function, then_body, path);
                path.pop();
                path.push(PathSegment::Else);
                self.body(function, else_body, path);
                path.pop();
            }
            Stmt::If {
                then_body,
                else_body,
                ..
            } => {
                path.push(PathSegment::Then);
                self.body(function, then_body, path);
                path.pop();
                path.push(PathSegment::Else);
                self.body(function, else_body, path);
                path.pop();
            }
            Stmt::Loop { body, .. } => {
                self.push_loop(function, LoopKind::Loop, AstPath(path.clone()));
                path.push(PathSegment::LoopBody);
                self.body(function, body, path);
                path.pop();
            }
            Stmt::Scope { body } => {
                path.push(PathSegment::ScopeBody);
                self.body(function, body, path);
                path.pop();
            }
            Stmt::LabeledBlock { body, .. } => {
                path.push(PathSegment::LabeledBody);
                self.body(function, body, path);
                path.pop();
            }
            Stmt::Match { arms, .. } => {
                for (index, arm) in arms.iter().enumerate() {
                    path.push(PathSegment::MatchArm(index));
                    self.body(function, &arm.body, path);
                    path.pop();
                }
            }
            Stmt::Unsafe { body } => {
                path.push(PathSegment::UnsafeBody);
                self.block(function, body, path);
                path.pop();
            }
            Stmt::While { body, .. } => {
                self.push_loop(function, LoopKind::While, AstPath(path.clone()));
                path.push(PathSegment::WhileBody);
                self.block(function, body, path);
                path.pop();
            }
            Stmt::Block(body) => {
                path.push(PathSegment::BlockBody);
                self.block(function, body, path);
                path.pop();
            }
            Stmt::Assign { .. }
            | Stmt::CompoundAssign { .. }
            | Stmt::Expr(_)
            | Stmt::Return(_)
            | Stmt::Break(_)
            | Stmt::Continue(_) => {}
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Block, FnDef, Item, Pattern, Program, Stmt, Visibility};

    fn named(name: &str, stmts: Vec<Stmt>) -> FnDef {
        let mut f = func(vec![param("arg0", "i32")], None, stmts);
        f.name = name.into();
        f
    }

    #[test]
    fn assigns_deterministic_function_and_param_ids() {
        let program = Program {
            items: vec![
                Item::Fn(named("first", Vec::new())),
                Item::Fn(named("second", Vec::new())),
            ],
        };

        let analyzed = analyze(program);

        assert_eq!(
            analyzed
                .facts
                .functions
                .iter()
                .map(|f| (f.id, f.name.as_str(), f.item_index))
                .collect::<Vec<_>>(),
            vec![(FunctionId(0), "first", 0), (FunctionId(1), "second", 1)]
        );
        assert_eq!(
            analyzed
                .facts
                .bindings
                .iter()
                .map(|b| (b.id, b.function, b.name.as_str(), b.kind.clone()))
                .collect::<Vec<_>>(),
            vec![
                (
                    BindingId(0),
                    FunctionId(0),
                    "arg0",
                    BindingKind::Param { index: 0 }
                ),
                (
                    BindingId(1),
                    FunctionId(1),
                    "arg0",
                    BindingKind::Param { index: 0 }
                )
            ]
        );
    }

    #[test]
    fn records_nested_local_paths_and_loop_ids() {
        let program = Program {
            items: vec![Item::Fn(named(
                "f",
                vec![
                    let_mut("outer", "i32", int(0)),
                    Stmt::Scope {
                        body: vec![IndentStmt {
                            depth: 2,
                            stmt: Stmt::Loop {
                                label: None,
                                body: vec![IndentStmt {
                                    depth: 3,
                                    stmt: let_mut("inner", "i32", int(1)),
                                }],
                            },
                        }],
                    },
                    Stmt::While {
                        cond: var("outer"),
                        body: Block {
                            stmts: vec![IndentStmt {
                                depth: 2,
                                stmt: let_mut("while_local", "i32", int(2)),
                            }],
                            tail: None,
                        },
                    },
                ],
            ))],
        };

        let analyzed = analyze(program);

        assert_eq!(
            analyzed
                .facts
                .bindings
                .iter()
                .map(|b| (b.name.as_str(), b.path.clone()))
                .collect::<Vec<_>>(),
            vec![
                ("arg0", AstPath::default()),
                ("outer", AstPath(vec![PathSegment::Stmt(0)])),
                (
                    "inner",
                    AstPath(vec![
                        PathSegment::Stmt(1),
                        PathSegment::ScopeBody,
                        PathSegment::Stmt(0),
                        PathSegment::LoopBody,
                        PathSegment::Stmt(0)
                    ])
                ),
                (
                    "while_local",
                    AstPath(vec![
                        PathSegment::Stmt(2),
                        PathSegment::WhileBody,
                        PathSegment::Stmt(0)
                    ])
                )
            ]
        );
        assert_eq!(
            analyzed
                .facts
                .loops
                .iter()
                .map(|l| (l.id, l.kind.clone(), l.path.clone()))
                .collect::<Vec<_>>(),
            vec![
                (
                    LoopId(0),
                    LoopKind::Loop,
                    AstPath(vec![
                        PathSegment::Stmt(1),
                        PathSegment::ScopeBody,
                        PathSegment::Stmt(0)
                    ])
                ),
                (
                    LoopId(1),
                    LoopKind::While,
                    AstPath(vec![PathSegment::Stmt(2)])
                )
            ]
        );
    }

    #[test]
    fn records_branch_and_match_local_paths() {
        let program = Program {
            items: vec![Item::Fn(FnDef {
                vis: Visibility::Private,
                unsafe_extern_c: false,
                name: "f".into(),
                params: Vec::new(),
                ret: None,
                body: vec![IndentStmt {
                    depth: 1,
                    stmt: Stmt::Match {
                        expr: var("x"),
                        arms: vec![crate::rust_ast::MatchArm {
                            pattern: Pattern::Wildcard,
                            body: vec![IndentStmt {
                                depth: 2,
                                stmt: Stmt::LetIf {
                                    name: "choice".into(),
                                    mutable: false,
                                    ty: None,
                                    cond: var("x"),
                                    then_body: vec![IndentStmt {
                                        depth: 3,
                                        stmt: let_mut("then_local", "i32", int(1)),
                                    }],
                                    then_value: int(1),
                                    else_body: vec![IndentStmt {
                                        depth: 3,
                                        stmt: let_mut("else_local", "i32", int(2)),
                                    }],
                                    else_value: int(2),
                                },
                            }],
                        }],
                    },
                }],
            })],
        };

        let analyzed = analyze(program);

        assert_eq!(
            analyzed
                .facts
                .bindings
                .iter()
                .map(|b| (b.name.as_str(), b.path.clone()))
                .collect::<Vec<_>>(),
            vec![
                (
                    "choice",
                    AstPath(vec![
                        PathSegment::Stmt(0),
                        PathSegment::MatchArm(0),
                        PathSegment::Stmt(0)
                    ])
                ),
                (
                    "then_local",
                    AstPath(vec![
                        PathSegment::Stmt(0),
                        PathSegment::MatchArm(0),
                        PathSegment::Stmt(0),
                        PathSegment::Then,
                        PathSegment::Stmt(0)
                    ])
                ),
                (
                    "else_local",
                    AstPath(vec![
                        PathSegment::Stmt(0),
                        PathSegment::MatchArm(0),
                        PathSegment::Stmt(0),
                        PathSegment::Else,
                        PathSegment::Stmt(0)
                    ])
                )
            ]
        );
    }

    #[test]
    fn analysis_preserves_program_output() {
        let program = Program {
            items: vec![Item::Fn(named(
                "f",
                vec![let_mut("x", "i32", int(0)), Stmt::Return(Some(var("x")))],
            ))],
        };
        let before = program.emit();

        let analyzed = analyze(program);

        assert_eq!(analyzed.program.emit(), before);
        assert!(analyzed.facts.relations.is_empty());
    }
}
