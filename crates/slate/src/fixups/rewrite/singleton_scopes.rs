use crate::fixups::facts::PathSegment;
use crate::fixups::support::walk;
use crate::rust_ast::{IndentStmt, Stmt};

pub(in crate::fixups) fn fixup(body: &mut Vec<IndentStmt>) -> bool {
    fixup_at(body, &mut Vec::new())
}

fn fixup_at(body: &mut Vec<IndentStmt>, path: &mut Vec<PathSegment>) -> bool {
    for index in 0..body.len() {
        let mut changed = false;
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            walk::nested_body_vecs_mut_with_path(&mut body[index].stmt, path, &mut |body, path| {
                if !changed {
                    changed = fixup_at(body, path);
                }
            });
        });
        if changed {
            return true;
        }

        if unwrap_singleton_scope(&mut body[index]) {
            return true;
        }
    }
    false
}

fn unwrap_singleton_scope(indent: &mut IndentStmt) -> bool {
    let Stmt::Scope { body } = &mut indent.stmt else {
        return false;
    };
    if body.len() != 1 || !is_unwrappable(&body[0].stmt) {
        return false;
    }

    let child = body.pop().expect("singleton scope body");
    indent.stmt = child.stmt;
    true
}

fn is_unwrappable(stmt: &Stmt) -> bool {
    matches!(
        stmt,
        Stmt::If { .. } | Stmt::Loop { .. } | Stmt::Match { .. }
    )
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Item, MatchArm, Pattern, Program};

    fn after(stmts: Vec<Stmt>) -> String {
        let mut program = Program {
            items: vec![Item::Fn(func(vec![], None, stmts))],
        };
        let Item::Fn(f) = &mut program.items[0] else {
            unreachable!();
        };
        while fixup(&mut f.body) {}
        program.emit()
    }

    fn stmt(stmt: Stmt) -> IndentStmt {
        IndentStmt { depth: 0, stmt }
    }

    #[test]
    fn unwraps_singleton_if_scope() {
        let got = after(vec![Stmt::Scope {
            body: vec![stmt(Stmt::If {
                cond: bin(crate::rust_ast::BinOp::Lt, var("n"), int(10)),
                then_body: vec![stmt(assign("n", int(10)))],
                else_body: vec![],
            })],
        }]);

        assert!(got.contains("    if n < 10 {\n        n = 10;\n    }\n"));
        assert!(!got.contains("    {\n        if n < 10"));
    }

    #[test]
    fn unwraps_nested_singleton_if_scope() {
        let got = after(vec![Stmt::If {
            cond: var("ok"),
            then_body: vec![stmt(Stmt::Scope {
                body: vec![stmt(Stmt::If {
                    cond: var("inner"),
                    then_body: vec![stmt(assign("x", int(1)))],
                    else_body: vec![],
                })],
            })],
            else_body: vec![],
        }]);

        assert!(
            got.contains("    if ok {\n        if inner {\n            x = 1;\n        }\n    }\n")
        );
        assert!(!got.contains("    if ok {\n        {\n"));
    }

    #[test]
    fn unwraps_singleton_loop_and_match_scopes() {
        let got = after(vec![
            Stmt::Scope {
                body: vec![stmt(Stmt::Loop {
                    label: None,
                    body: vec![stmt(Stmt::Break(None))],
                })],
            },
            Stmt::Scope {
                body: vec![stmt(Stmt::Match {
                    expr: var("x"),
                    arms: vec![MatchArm {
                        pattern: Pattern::Wildcard,
                        body: vec![stmt(assign("x", int(1)))],
                    }],
                })],
            },
        ]);

        assert!(got.contains("    loop {\n        break;\n    }\n"));
        assert!(
            got.contains("    match x {\n        _ => {\n            x = 1;\n        }\n    }\n")
        );
        assert!(!got.contains("    {\n        loop"));
        assert!(!got.contains("    {\n        match"));
    }

    #[test]
    fn keeps_scope_with_declaration() {
        let got = after(vec![Stmt::Scope {
            body: vec![stmt(let_mut("x", "i32", int(1)))],
        }]);

        assert!(got.contains("    {\n        let mut x: i32 = 1;\n    }\n"));
    }

    #[test]
    fn keeps_scope_with_multiple_statements() {
        let got = after(vec![Stmt::Scope {
            body: vec![stmt(assign("x", int(1))), stmt(assign("y", int(2)))],
        }]);

        assert!(got.contains("    {\n        x = 1;\n        y = 2;\n    }\n"));
    }

    #[test]
    fn keeps_singleton_labeled_block_scope() {
        let got = after(vec![Stmt::Scope {
            body: vec![stmt(Stmt::LabeledBlock {
                label: "done".into(),
                body: vec![stmt(Stmt::Break(Some("done".into())))],
            })],
        }]);

        assert!(
            got.contains("    {\n        'done: {\n            break 'done;\n        }\n    }\n")
        );
    }
}
