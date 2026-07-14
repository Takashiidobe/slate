use crate::fixups::facts::{AstPath, FixupFacts, FunctionId, PathSegment};
use crate::fixups::support::walk;
use crate::rust_ast::{Expr, IndentStmt, Stmt};

pub(in crate::fixups) fn fixup(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
) -> bool {
    fixup_at(body, function, facts, &mut Vec::new())
}

fn fixup_at(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
) -> bool {
    for index in 0..body.len() {
        let mut changed = false;
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            walk::nested_body_vecs_mut_with_path(&mut body[index].stmt, path, &mut |body, path| {
                if !changed {
                    changed = fixup_at(body, function, facts, path);
                }
            })
        });
        if changed {
            return true;
        }
    }

    if body.len() < 2 {
        return false;
    }

    for index in 0..body.len() - 1 {
        let Some(init) = final_return_temp(body, index, function, facts, path) else {
            continue;
        };
        body[index + 1].stmt = Stmt::Return(Some(init));
        body.remove(index);
        return true;
    }

    false
}

fn final_return_temp(
    body: &[IndentStmt],
    index: usize,
    function: FunctionId,
    facts: &FixupFacts,
    body_path: &[PathSegment],
) -> Option<Expr> {
    let Stmt::Let {
        name,
        mutable: false,
        init: Some(init),
        ..
    } = &body[index].stmt
    else {
        return None;
    };
    if !is_temp_name(name) {
        return None;
    }
    let Stmt::Return(Some(Expr::Var(returned))) = &body[index + 1].stmt else {
        return None;
    };
    if returned.as_str() != name {
        return None;
    }
    let def_path = AstPath(stmt_path(body_path, index));
    let return_path = AstPath(stmt_path(body_path, index + 1));
    let binding = facts.binding_by_local_path(function, name, &def_path)?;
    let uses = facts
        .def_use
        .iter()
        .find(|fact| fact.function == function && fact.binding == binding)?;
    if uses.reads != [return_path] || !uses.writes.is_empty() {
        return None;
    }

    Some(init.clone())
}

fn is_temp_name(name: &str) -> bool {
    name.strip_prefix("_v")
        .is_some_and(|rest| !rest.is_empty() && rest.bytes().all(|b| b.is_ascii_digit()))
}

fn stmt_path(body_path: &[PathSegment], index: usize) -> Vec<PathSegment> {
    let mut path = body_path.to_vec();
    path.push(PathSegment::Stmt(index));
    path
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Block, Item, Program, UnaryOp};

    fn after(stmts: Vec<Stmt>) -> String {
        let mut program = Program {
            items: vec![Item::Fn(func(vec![], Some("i32"), stmts))],
        };
        loop {
            let analyzed = crate::fixups::facts::analyze(program.clone());
            let facts = analyzed.facts;
            let Item::Fn(f) = &mut program.items[0] else {
                unreachable!();
            };
            if !fixup(&mut f.body, FunctionId(0), &facts) {
                break;
            }
        }
        program.emit()
    }

    #[test]
    fn collapses_adjacent_return_temp() {
        let out = after(vec![
            temp("keep", "i32", int(1)),
            temp("_v0", "i32", var("keep")),
            Stmt::Return(Some(var("_v0"))),
        ]);

        assert_eq!(
            out,
            "\
fn f() -> i32 {
    let keep: i32 = 1;
    return keep;
}
"
        );
    }

    #[test]
    fn collapses_adjacent_unsafe_deref_temp() {
        let out = after(vec![
            temp("slot", "*mut i32", var("p")),
            temp(
                "_v0",
                "i32",
                Expr::Unsafe(Box::new(Block {
                    stmts: vec![],
                    tail: Some(Box::new(Expr::Unary {
                        op: UnaryOp::Deref,
                        expr: Box::new(var("slot")),
                    })),
                })),
            ),
            Stmt::Return(Some(var("_v0"))),
        ]);

        assert!(out.contains("return unsafe { *slot };"));
        assert!(!out.contains("let _v0"));
    }

    #[test]
    fn keeps_used_temp_materialized() {
        let out = after(vec![
            temp("_v0", "i32", int(1)),
            temp("copy", "i32", var("_v0")),
            Stmt::Return(Some(var("_v0"))),
        ]);

        assert!(out.contains("let _v0: i32 = 1;"));
        assert!(out.contains("return _v0;"));
    }

    #[test]
    fn keeps_non_adjacent_temp_materialized() {
        let out = after(vec![
            temp("_v0", "i32", int(1)),
            temp("other", "i32", int(2)),
            Stmt::Return(Some(var("_v0"))),
        ]);

        assert!(out.contains("let _v0: i32 = 1;"));
        assert!(out.contains("return _v0;"));
    }

    #[test]
    fn keeps_non_synthetic_temp_materialized() {
        let out = after(vec![
            temp("value", "i32", int(1)),
            Stmt::Return(Some(var("value"))),
        ]);

        assert!(out.contains("let value: i32 = 1;"));
        assert!(out.contains("return value;"));
    }
}
