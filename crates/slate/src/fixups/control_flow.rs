use std::collections::BTreeSet;

use crate::fixups::facts::{
    AstPath, ControlFlowExit, ControlFlowFact, ControlFlowSubject, FixupFacts, FunctionId,
    PathSegment,
};
use crate::fixups::support::walk;
use crate::rust_ast::{Block, IndentStmt, Item, Label, Program, Stmt};

pub(super) fn collect_facts(program: &Program, facts: &mut FixupFacts) {
    facts.control_flow.clear();
    let mut all = Vec::new();
    for (item_index, item) in program.items.iter().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        let mut collector = Collector {
            function,
            facts: Vec::new(),
        };
        collector.body(&f.body, &mut Vec::new(), true);
        all.extend(collector.facts);
    }
    facts.control_flow = all;
}

struct Collector {
    function: FunctionId,
    facts: Vec<ControlFlowFact>,
}

#[derive(Debug, Clone)]
struct Summary {
    reachable: bool,
    falls_through: bool,
    exits: BTreeSet<ControlFlowExit>,
    has_unreachable_tail: bool,
    expression_eligible: bool,
}

impl Summary {
    fn plain(reachable: bool) -> Self {
        Self {
            reachable,
            falls_through: reachable,
            exits: BTreeSet::new(),
            has_unreachable_tail: false,
            expression_eligible: false,
        }
    }

    fn terminal(reachable: bool, exit: ControlFlowExit) -> Self {
        let mut exits = BTreeSet::new();
        if reachable {
            exits.insert(exit);
        }
        Self {
            reachable,
            falls_through: false,
            exits,
            has_unreachable_tail: false,
            expression_eligible: false,
        }
    }

    fn single_exit(&self) -> bool {
        self.reachable && !self.falls_through && self.exits.len() == 1
    }
}

impl Collector {
    fn body(
        &mut self,
        body: &[IndentStmt],
        path: &mut Vec<PathSegment>,
        reachable: bool,
    ) -> Summary {
        let mut current_reachable = reachable;
        let mut exits = BTreeSet::new();
        let mut has_unreachable_tail = false;
        let mut last_expression_eligible = false;

        for (index, indent) in body.iter().enumerate() {
            walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
                let summary = self.stmt(&indent.stmt, path, current_reachable);
                if !current_reachable {
                    has_unreachable_tail = true;
                } else {
                    exits.extend(summary.exits.iter().cloned());
                    has_unreachable_tail |= summary.has_unreachable_tail;
                    current_reachable = summary.falls_through;
                    last_expression_eligible = summary.expression_eligible;
                }
            });
        }

        let summary = Summary {
            reachable,
            falls_through: current_reachable,
            exits,
            has_unreachable_tail,
            expression_eligible: reachable
                && current_reachable
                && !has_unreachable_tail
                && !body.is_empty()
                && last_expression_eligible,
        };
        self.push(ControlFlowSubject::Body, path, &summary);
        summary
    }

    fn block(&mut self, block: &Block, path: &mut Vec<PathSegment>, reachable: bool) -> Summary {
        let mut summary = self.body(&block.stmts, path, reachable);
        if summary.falls_through && block.tail.is_some() {
            summary.expression_eligible = !summary.has_unreachable_tail && summary.exits.is_empty();
        } else if block.tail.is_none() {
            summary.expression_eligible = false;
        }
        self.push(ControlFlowSubject::Block, path, &summary);
        summary
    }

    fn stmt(&mut self, stmt: &Stmt, path: &mut Vec<PathSegment>, reachable: bool) -> Summary {
        let summary = match stmt {
            Stmt::Let { .. }
            | Stmt::Assign { .. }
            | Stmt::CompoundAssign { .. }
            | Stmt::Expr(_) => Summary {
                expression_eligible: reachable && matches!(stmt, Stmt::Expr(_)),
                ..Summary::plain(reachable)
            },
            Stmt::Return(_) => Summary::terminal(reachable, ControlFlowExit::Return),
            Stmt::Break(label) => {
                Summary::terminal(reachable, ControlFlowExit::Break(label_name(label)))
            }
            Stmt::Continue(label) => {
                Summary::terminal(reachable, ControlFlowExit::Continue(label_name(label)))
            }
            Stmt::If {
                then_body,
                else_body,
                ..
            } => self.branch(then_body, else_body, path, reachable),
            Stmt::LetIf {
                then_body,
                else_body,
                ..
            } => self.branch(then_body, else_body, path, reachable),
            Stmt::Loop { label, body } => self.loop_stmt(label.as_ref(), body, path, reachable),
            Stmt::While { body, .. } => self.while_stmt(body, path, reachable),
            Stmt::Scope { body } => walk::with_path_segment(path, PathSegment::ScopeBody, |path| {
                self.body(body, path, reachable)
            }),
            Stmt::LabeledBlock { label, body } => self.labeled_block(label, body, path, reachable),
            Stmt::Unsafe { body } => {
                walk::with_path_segment(path, PathSegment::UnsafeBody, |path| {
                    self.block(body, path, reachable)
                })
            }
            Stmt::Block(body) => walk::with_path_segment(path, PathSegment::BlockBody, |path| {
                self.block(body, path, reachable)
            }),
            Stmt::Match { arms, .. } => {
                let mut exits = BTreeSet::new();
                let mut falls_through = arms.is_empty() && reachable;
                let mut has_unreachable_tail = false;
                for (index, arm) in arms.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::MatchArm(index), |path| {
                        let summary = self.body(&arm.body, path, reachable);
                        exits.extend(summary.exits);
                        falls_through |= summary.falls_through;
                        has_unreachable_tail |= summary.has_unreachable_tail;
                    });
                }
                Summary {
                    reachable,
                    falls_through,
                    exits,
                    has_unreachable_tail,
                    expression_eligible: false,
                }
            }
        };
        self.push(ControlFlowSubject::Stmt, path, &summary);
        summary
    }

    fn branch(
        &mut self,
        then_body: &[IndentStmt],
        else_body: &[IndentStmt],
        path: &mut Vec<PathSegment>,
        reachable: bool,
    ) -> Summary {
        let then_summary = walk::with_path_segment(path, PathSegment::Then, |path| {
            self.body(then_body, path, reachable)
        });
        let else_summary = walk::with_path_segment(path, PathSegment::Else, |path| {
            self.body(else_body, path, reachable)
        });
        let mut exits = then_summary.exits;
        exits.extend(else_summary.exits);
        let falls_through = then_summary.falls_through || else_summary.falls_through;
        Summary {
            reachable,
            falls_through,
            exits,
            has_unreachable_tail: then_summary.has_unreachable_tail
                || else_summary.has_unreachable_tail,
            expression_eligible: false,
        }
    }

    fn loop_stmt(
        &mut self,
        label: Option<&Label>,
        body: &[IndentStmt],
        path: &mut Vec<PathSegment>,
        reachable: bool,
    ) -> Summary {
        let body_summary = walk::with_path_segment(path, PathSegment::LoopBody, |path| {
            self.body(body, path, reachable)
        });
        let target = label.map(|label| label.as_str());
        let mut exits = BTreeSet::new();
        let mut falls_through = false;
        for exit in body_summary.exits {
            match &exit {
                ControlFlowExit::Break(label) if label_matches(label.as_deref(), target) => {
                    falls_through = reachable;
                }
                ControlFlowExit::Continue(label) if label_matches(label.as_deref(), target) => {}
                _ => {
                    exits.insert(exit);
                }
            }
        }
        Summary {
            reachable,
            falls_through,
            exits,
            has_unreachable_tail: body_summary.has_unreachable_tail,
            expression_eligible: false,
        }
    }

    fn while_stmt(
        &mut self,
        body: &Block,
        path: &mut Vec<PathSegment>,
        reachable: bool,
    ) -> Summary {
        let body_summary = walk::with_path_segment(path, PathSegment::WhileBody, |path| {
            self.block(body, path, reachable)
        });
        let mut exits = BTreeSet::new();
        for exit in body_summary.exits {
            match exit {
                ControlFlowExit::Break(None) | ControlFlowExit::Continue(None) => {}
                _ => {
                    exits.insert(exit);
                }
            }
        }
        Summary {
            reachable,
            falls_through: reachable,
            exits,
            has_unreachable_tail: body_summary.has_unreachable_tail,
            expression_eligible: false,
        }
    }

    fn labeled_block(
        &mut self,
        label: &Label,
        body: &[IndentStmt],
        path: &mut Vec<PathSegment>,
        reachable: bool,
    ) -> Summary {
        let body_summary = walk::with_path_segment(path, PathSegment::LabeledBody, |path| {
            self.body(body, path, reachable)
        });
        let mut exits = BTreeSet::new();
        let mut falls_through = body_summary.falls_through;
        for exit in body_summary.exits {
            match &exit {
                ControlFlowExit::Break(Some(target)) if target == label.as_str() => {
                    falls_through = reachable;
                }
                _ => {
                    exits.insert(exit);
                }
            }
        }
        Summary {
            reachable,
            falls_through,
            exits,
            has_unreachable_tail: body_summary.has_unreachable_tail,
            expression_eligible: false,
        }
    }

    fn push(&mut self, subject: ControlFlowSubject, path: &[PathSegment], summary: &Summary) {
        self.facts.push(ControlFlowFact {
            function: self.function,
            subject,
            path: AstPath(path.to_vec()),
            reachable: summary.reachable,
            falls_through: summary.falls_through,
            exits: summary.exits.clone(),
            single_exit: summary.single_exit(),
            has_unreachable_tail: summary.has_unreachable_tail,
            expression_eligible: summary.expression_eligible,
        });
    }
}

fn label_name(label: &Option<Label>) -> Option<String> {
    label.as_ref().map(|label| label.as_str().to_string())
}

fn label_matches(label: Option<&str>, target: Option<&str>) -> bool {
    match (label, target) {
        (None, _) => true,
        (Some(label), Some(target)) => label == target,
        (Some(_), None) => false,
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::facts::{ControlFlowExit, ControlFlowSubject};
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Block, IndentStmt, MatchArm, Pattern};

    fn analyzed(stmts: Vec<Stmt>) -> FixupFacts {
        crate::fixups::facts::analyze(Program {
            items: vec![Item::Fn(func(vec![], None, stmts))],
        })
        .facts
    }

    fn fact_at(facts: &FixupFacts, subject: ControlFlowSubject, path: AstPath) -> &ControlFlowFact {
        facts
            .control_flow
            .iter()
            .find(|fact| fact.subject == subject && fact.path == path)
            .unwrap()
    }

    fn ind(stmt: Stmt) -> IndentStmt {
        IndentStmt { depth: 1, stmt }
    }

    #[test]
    fn straight_line_return_marks_unreachable_tail() {
        let facts = analyzed(vec![
            Stmt::Return(Some(int(1))),
            Stmt::Expr(var("unreachable")),
        ]);

        let return_fact = fact_at(
            &facts,
            ControlFlowSubject::Stmt,
            AstPath(vec![PathSegment::Stmt(0)]),
        );
        assert!(!return_fact.falls_through);
        assert!(return_fact.single_exit);
        assert!(return_fact.exits.contains(&ControlFlowExit::Return));

        let tail = fact_at(
            &facts,
            ControlFlowSubject::Stmt,
            AstPath(vec![PathSegment::Stmt(1)]),
        );
        assert!(!tail.reachable);

        let body = fact_at(&facts, ControlFlowSubject::Body, AstPath::default());
        assert!(body.has_unreachable_tail);
    }

    #[test]
    fn branches_merge_fallthrough_and_return_exits() {
        let facts = analyzed(vec![Stmt::If {
            cond: var("cond"),
            then_body: vec![ind(Stmt::Return(Some(int(1))))],
            else_body: vec![ind(Stmt::Expr(var("x")))],
        }]);

        let branch = fact_at(
            &facts,
            ControlFlowSubject::Stmt,
            AstPath(vec![PathSegment::Stmt(0)]),
        );
        assert!(branch.falls_through);
        assert!(!branch.single_exit);
        assert!(branch.exits.contains(&ControlFlowExit::Return));
    }

    #[test]
    fn all_returning_match_is_single_return_exit() {
        let facts = analyzed(vec![Stmt::Match {
            expr: var("x"),
            arms: vec![
                MatchArm {
                    pattern: Pattern::Wildcard,
                    body: vec![ind(Stmt::Return(Some(int(1))))],
                },
                MatchArm {
                    pattern: Pattern::I64(2),
                    body: vec![ind(Stmt::Return(Some(int(2))))],
                },
            ],
        }]);

        let fact = fact_at(
            &facts,
            ControlFlowSubject::Stmt,
            AstPath(vec![PathSegment::Stmt(0)]),
        );
        assert!(!fact.falls_through);
        assert!(fact.single_exit);
        assert_eq!(fact.exits, BTreeSet::from([ControlFlowExit::Return]));
    }

    #[test]
    fn loop_consumes_matching_break_and_continue() {
        let facts = analyzed(vec![Stmt::Loop {
            label: Some("outer".into()),
            body: vec![
                ind(Stmt::If {
                    cond: var("cond"),
                    then_body: vec![ind(Stmt::Continue(Some("outer".into())))],
                    else_body: vec![],
                }),
                ind(Stmt::Break(Some("outer".into()))),
            ],
        }]);

        let loop_fact = fact_at(
            &facts,
            ControlFlowSubject::Stmt,
            AstPath(vec![PathSegment::Stmt(0)]),
        );
        assert!(loop_fact.falls_through);
        assert!(loop_fact.exits.is_empty());

        let break_fact = fact_at(
            &facts,
            ControlFlowSubject::Stmt,
            AstPath(vec![
                PathSegment::Stmt(0),
                PathSegment::LoopBody,
                PathSegment::Stmt(1),
            ]),
        );
        assert!(break_fact.reachable);
    }

    #[test]
    fn labeled_block_consumes_matching_break_only() {
        let facts = analyzed(vec![Stmt::LabeledBlock {
            label: "done".into(),
            body: vec![ind(Stmt::Break(Some("done".into())))],
        }]);

        let fact = fact_at(
            &facts,
            ControlFlowSubject::Stmt,
            AstPath(vec![PathSegment::Stmt(0)]),
        );
        assert!(fact.falls_through);
        assert!(fact.exits.is_empty());
    }

    #[test]
    fn block_tail_makes_expression_eligible_block() {
        let facts = analyzed(vec![Stmt::Block(Block {
            stmts: vec![ind(temp("x", "i32", int(1)))],
            tail: Some(Box::new(var("x"))),
        })]);

        let block = fact_at(
            &facts,
            ControlFlowSubject::Block,
            AstPath(vec![PathSegment::Stmt(0), PathSegment::BlockBody]),
        );
        assert!(block.expression_eligible);
        assert!(block.falls_through);
        assert!(block.exits.is_empty());
    }
}
