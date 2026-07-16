//! Drop the binding of a synthetic temp that only holds an unused call result.
//!
//! Baseline lowering spills every call into its own `let` temp, even when the C
//! source discards the result: `let _v6 = unsafe { printf(...) };`. When such a
//! temp is never read, the binding is pure noise — the call still has to run for
//! its side effects, so the `let _v = <call>;` becomes a bare `<call>;`.
//!
//! Only temps whose initializer performs a call are touched (the side effect is
//! preserved), and only when the temp is unused in its entire lexical scope, so
//! a used call result is left materialized.

use crate::fixups::facts::{
    AstPath, EffectKind, EffectSubject, FixupFacts, FunctionId, PathSegment,
};
use crate::fixups::support::walk;
use crate::rust_ast::{Block, IndentStmt, Stmt};

pub(in crate::fixups) fn fixup(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
) {
    scope(body, function, facts, &mut Vec::new());
}

fn scope(
    stmts: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
) {
    for (index, stmt) in stmts.iter_mut().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            recurse(&mut stmt.stmt, function, facts, path);
        });
    }
    for i in 0..stmts.len() {
        let def_path = stmt_path(path, i);
        let name = match &stmts[i].stmt {
            Stmt::Let {
                name,
                mutable: false,
                init: Some(_),
                ..
            } if is_temp_name(name) => name.clone(),
            _ => continue,
        };
        if !init_has_call(function, facts, &def_path) {
            continue;
        }
        let Some(binding) = facts.binding_by_local_path(function, &name, &AstPath(def_path)) else {
            continue;
        };
        if facts
            .def_use(binding)
            .is_none_or(|fact| !fact.reads.is_empty())
        {
            continue;
        }
        let init = match &mut stmts[i].stmt {
            Stmt::Let { init, .. } => init.take(),
            _ => None,
        };
        if let Some(init) = init {
            stmts[i].stmt = Stmt::Expr(init);
        }
    }
}

fn recurse(stmt: &mut Stmt, function: FunctionId, facts: &FixupFacts, path: &mut Vec<PathSegment>) {
    match stmt {
        Stmt::If {
            then_body,
            else_body,
            ..
        } => {
            walk::with_path_segment(path, PathSegment::Then, |path| {
                scope(then_body, function, facts, path);
            });
            walk::with_path_segment(path, PathSegment::Else, |path| {
                scope(else_body, function, facts, path);
            });
        }
        Stmt::LetIf {
            then_body,
            else_body,
            ..
        } => {
            walk::with_path_segment(path, PathSegment::Then, |path| {
                scope(then_body, function, facts, path);
            });
            walk::with_path_segment(path, PathSegment::Else, |path| {
                scope(else_body, function, facts, path);
            });
        }
        Stmt::Loop { body, .. } => {
            walk::with_path_segment(path, PathSegment::LoopBody, |path| {
                scope(body, function, facts, path);
            });
        }
        Stmt::Scope { body } => {
            walk::with_path_segment(path, PathSegment::ScopeBody, |path| {
                scope(body, function, facts, path);
            });
        }
        Stmt::LabeledBlock { body, .. } => {
            walk::with_path_segment(path, PathSegment::LabeledBody, |path| {
                scope(body, function, facts, path);
            });
        }
        Stmt::Unsafe { body } => {
            walk::with_path_segment(path, PathSegment::UnsafeBody, |path| {
                scope_block(body, function, facts, path);
            });
        }
        Stmt::While { body, .. } => {
            walk::with_path_segment(path, PathSegment::WhileBody, |path| {
                scope_block(body, function, facts, path);
            });
        }
        Stmt::Block(body) => {
            walk::with_path_segment(path, PathSegment::BlockBody, |path| {
                scope_block(body, function, facts, path);
            });
        }
        Stmt::Match { arms, .. } => {
            for (index, arm) in arms.iter_mut().enumerate() {
                walk::with_path_segment(path, PathSegment::MatchArm(index), |path| {
                    scope(&mut arm.body, function, facts, path);
                });
            }
        }
        _ => {}
    }
}

fn scope_block(
    block: &mut Block,
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
) {
    scope(&mut block.stmts, function, facts, path);
}

fn init_has_call(function: FunctionId, facts: &FixupFacts, path: &[PathSegment]) -> bool {
    facts
        .effect(function, EffectSubject::Expr, &AstPath(path.to_vec()))
        .is_some_and(|fact| {
            fact.effects.contains(&EffectKind::ReadOnlyCall)
                || fact.effects.contains(&EffectKind::UnknownCall)
                || fact.effects.contains(&EffectKind::MethodCall)
                || fact.effects.contains(&EffectKind::MacroExpansion)
        })
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
    use crate::rust_ast::{Expr, Item, MatchArm, Pattern, Program, Type};

    fn dropped(stmts: Vec<Stmt>) -> String {
        let mut program = Program {
            items: vec![Item::Fn(func(vec![], None, stmts))],
        };
        let analyzed = crate::fixups::facts::analyze(program.clone());
        let facts = analyzed.facts;
        let Item::Fn(f) = &mut program.items[0] else {
            unreachable!();
        };
        fixup(&mut f.body, FunctionId(0), &facts);
        program.emit()
    }

    #[test]
    fn drops_unused_call_result_binding() {
        let out = dropped(vec![
            temp("_v0", "i32", var("a")),
            temp("_v1", "i32", call("printf", vec![var("fmt"), var("_v0")])),
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i32 = a;
    printf(fmt, _v0);
}
"
        );
    }

    #[test]
    fn preserves_unsafe_block_wrapper() {
        let out = dropped(vec![Stmt::Let {
            name: "_v1".into(),
            mutable: false,
            ty: Some(Type::parse("i32")),
            init: Some(Expr::Unsafe(Box::new(Block {
                stmts: vec![],
                tail: Some(Box::new(call("printf", vec![var("fmt")]))),
            }))),
        }]);

        assert_eq!(
            out,
            "\
fn f() {
    unsafe { printf(fmt) };
}
"
        );
    }

    #[test]
    fn drops_unused_call_result_binding_inside_match_arm() {
        let out = dropped(vec![Stmt::Match {
            expr: var("state"),
            arms: vec![MatchArm {
                pattern: Pattern::I64(0),
                body: vec![IndentStmt {
                    depth: 0,
                    stmt: temp("_v1", "i32", call("printf", vec![var("fmt")])),
                }],
            }],
        }]);

        assert_eq!(
            out,
            "\
fn f() {
    match state {
        0 => {
            printf(fmt);
        }
    }
}
"
        );
    }

    #[test]
    fn keeps_used_call_result() {
        let out = dropped(vec![
            temp("_v0", "i32", call("getchar", vec![])),
            assign("b", var("_v0")),
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i32 = getchar();
    b = _v0;
}
"
        );
    }

    #[test]
    fn leaves_pure_unused_temp_alone() {
        let out = dropped(vec![temp("_v0", "i32", int(5))]);

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i32 = 5;
}
"
        );
    }
}
