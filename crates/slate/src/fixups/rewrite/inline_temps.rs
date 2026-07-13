//! Inline single-use pure temps directly on the statement list. The temp's init
//! is spliced as an `Expr` subtree into its use site and precedence-aware
//! rendering elides redundant parens.

use crate::fixups::facts::{
    AstPath, EffectKind, EffectSubject, FixupFacts, FunctionId, PathSegment, Purity,
};
use crate::fixups::support::walk;
use crate::rust_ast::{Block, Expr, IndentStmt, Stmt};

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
    if inline_nested_temps(body, function, facts, path) {
        return true;
    }
    for i in 0..body.len() {
        let mut def_path = path.clone();
        def_path.push(PathSegment::Stmt(i));
        let Stmt::Let {
            name,
            mutable: false,
            init: Some(init),
            ..
        } = &body[i].stmt
        else {
            continue;
        };
        if !is_temp_name(name) || !is_pure_expr(function, facts, &def_path) {
            continue;
        }
        let Some(binding) = facts.binding_by_local_path(function, name, &AstPath(def_path.clone()))
        else {
            continue;
        };
        let name = name.clone();
        let init = init.clone();
        let Some(use_index) = single_safe_use(body, i, binding, function, facts, path) else {
            continue;
        };
        if body[use_index].stmt.substitute_var(&name, &init) {
            body.remove(i);
            return true;
        }
    }
    false
}

fn inline_nested_temps(
    body: &mut [IndentStmt],
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
) -> bool {
    for (index, stmt) in body.iter_mut().enumerate() {
        if walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            match &mut stmt.stmt {
                Stmt::If {
                    then_body,
                    else_body,
                    ..
                }
                | Stmt::LetIf {
                    then_body,
                    else_body,
                    ..
                } => {
                    walk::with_path_segment(path, PathSegment::Then, |path| {
                        fixup_at(then_body, function, facts, path)
                    }) || walk::with_path_segment(path, PathSegment::Else, |path| {
                        fixup_at(else_body, function, facts, path)
                    })
                }
                Stmt::Loop { body, .. } => {
                    walk::with_path_segment(path, PathSegment::LoopBody, |path| {
                        fixup_at(body, function, facts, path)
                    })
                }
                Stmt::Scope { body } => {
                    walk::with_path_segment(path, PathSegment::ScopeBody, |path| {
                        fixup_at(body, function, facts, path)
                    })
                }
                Stmt::LabeledBlock { body, .. } => {
                    walk::with_path_segment(path, PathSegment::LabeledBody, |path| {
                        fixup_at(body, function, facts, path)
                    })
                }
                Stmt::Unsafe { body } => {
                    let Block { stmts, tail } = body;
                    let _ = tail;
                    walk::with_path_segment(path, PathSegment::UnsafeBody, |path| {
                        fixup_at(stmts, function, facts, path)
                    })
                }
                _ => false,
            }
        }) {
            return true;
        }
    }
    false
}

fn single_safe_use(
    body: &[IndentStmt],
    def_index: usize,
    binding: crate::fixups::facts::BindingId,
    function: FunctionId,
    facts: &FixupFacts,
    body_path: &[PathSegment],
) -> Option<usize> {
    let reads = &facts.def_use(binding)?.reads;
    if reads.len() != 1 {
        return None;
    }
    let use_index = direct_stmt_index(body_path, &reads[0])?;
    if use_index <= def_index || use_index >= body.len() {
        return None;
    }
    let use_path = stmt_path(body_path, use_index);
    if stmt_contains_call(function, facts, &use_path)
        || is_receiver_use(&body[use_index].stmt, binding_name(facts, binding)?)
    {
        return None;
    }
    for index in def_index + 1..use_index {
        let path = stmt_path(body_path, index);
        if !is_pure_temp_let(&body[index].stmt, function, facts, &path) {
            return None;
        }
    }
    Some(use_index)
}

fn is_pure_temp_let(
    stmt: &Stmt,
    function: FunctionId,
    facts: &FixupFacts,
    path: &[PathSegment],
) -> bool {
    let Stmt::Let {
        name,
        init: Some(_),
        ..
    } = stmt
    else {
        return false;
    };
    is_temp_name(name) && is_pure_expr(function, facts, path)
}

fn is_pure_expr(function: FunctionId, facts: &FixupFacts, path: &[PathSegment]) -> bool {
    facts
        .effect(function, EffectSubject::Expr, &AstPath(path.to_vec()))
        .is_some_and(|fact| fact.purity == Purity::MovablePure)
}

fn stmt_contains_call(function: FunctionId, facts: &FixupFacts, path: &[PathSegment]) -> bool {
    facts
        .effect(function, EffectSubject::Stmt, &AstPath(path.to_vec()))
        .is_some_and(|fact| {
            fact.effects.contains(&EffectKind::ReadOnlyCall)
                || fact.effects.contains(&EffectKind::UnknownCall)
                || fact.effects.contains(&EffectKind::MethodCall)
                || fact.effects.contains(&EffectKind::MacroExpansion)
        })
}

fn is_receiver_use(stmt: &Stmt, name: &str) -> bool {
    walk::stmt_exprs_any(stmt, &mut |expr| {
        let receiver = match expr {
            Expr::MethodCall { recv, .. } | Expr::MethodCallGeneric { recv, .. } => Some(&**recv),
            Expr::Field { base, .. } | Expr::TupleField { base, .. } => Some(&**base),
            _ => None,
        };
        matches!(receiver, Some(Expr::Var(v)) if v.as_str() == name)
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

fn direct_stmt_index(body_path: &[PathSegment], read: &AstPath) -> Option<usize> {
    let rest = read.0.strip_prefix(body_path)?;
    match rest {
        [PathSegment::Stmt(index), ..] => Some(*index),
        _ => None,
    }
}

fn binding_name(facts: &FixupFacts, binding: crate::fixups::facts::BindingId) -> Option<&str> {
    facts
        .bindings
        .iter()
        .find(|fact| fact.id == binding)
        .map(|fact| fact.name.as_str())
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{BinOp, Item, Program, Type};

    fn inlined(stmts: Vec<Stmt>) -> String {
        let mut program = Program {
            items: vec![Item::Fn(func(vec![], None, stmts))],
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
    fn inlines_single_use_scalar_temps() {
        let out = inlined(vec![
            let_mut("a", "i32", int(0)),
            temp("_v0", "i32", int(20)),
            assign("a", var("_v0")),
            temp("_v1", "i32", int(5)),
            temp("_v2", "i32", var("a")),
            temp("_v3", "i32", bin(BinOp::Sub, var("_v2"), var("_v1"))),
            assign("a", var("_v3")),
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let mut a: i32 = 0;
    a = 20;
    a = a - 5;
}
"
        );
    }

    #[test]
    fn keeps_temp_that_feeds_a_branch_value() {
        // a temp used once in the branch body (the store) and once as the branch's
        // yielded value must not be inlined away, or the tail reference dangles.
        let out = inlined(vec![Stmt::LetIf {
            name: "_v0".into(),
            mutable: false,
            ty: Some(Type::parse("i32")),
            cond: var("c"),
            then_body: vec![
                IndentStmt {
                    depth: 2,
                    stmt: temp("_v1", "i32", bin(BinOp::Add, var("a"), int(1))),
                },
                IndentStmt {
                    depth: 2,
                    stmt: assign("a", var("_v1")),
                },
            ],
            then_value: var("_v1"),
            else_body: vec![],
            else_value: int(0),
        }]);

        assert!(
            out.contains("let _v1: i32 = a + 1;"),
            "binding must survive, got:\n{out}"
        );
        assert_eq!(
            out.matches("_v1").count(),
            3,
            "def + store use + branch value"
        );
    }

    #[test]
    fn does_not_inline_call_results() {
        let out = inlined(vec![
            temp("_v0", "i32", call("g", vec![])),
            temp("_v1", "i32", var("_v0")),
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i32 = g();
    let _v1: i32 = _v0;
}
"
        );
    }

    #[test]
    fn does_not_cross_side_effecting_statement() {
        let out = inlined(vec![
            temp("_v0", "i32", var("a")),
            Stmt::Expr(call("printf", vec![var("_v1")])),
            assign("b", var("_v0")),
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i32 = a;
    printf(_v1);
    b = _v0;
}
"
        );
    }

    #[test]
    fn does_not_inline_impure_intrinsics() {
        let out = inlined(vec![
            temp(
                "_v0",
                "i32",
                call("std::ptr::read_volatile", vec![var("p")]),
            ),
            assign("b", var("_v0")),
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i32 = std::ptr::read_volatile(p);
    b = _v0;
}
"
        );
    }

    #[test]
    fn does_not_inline_method_receivers_that_need_type_annotations() {
        let out = inlined(vec![
            temp("_v0", "i32", int(2147483647)),
            temp("_v1", "i32", int(1)),
            Stmt::Let {
                name: "_v2".into(),
                mutable: false,
                ty: None,
                init: Some(Expr::MethodCall {
                    recv: Box::new(var("_v0")),
                    method: "overflowing_add".into(),
                    args: vec![var("_v1")],
                }),
            },
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i32 = 2147483647;
    let _v1: i32 = 1;
    let _v2 = _v0.overflowing_add(_v1);
}
"
        );
    }

    #[test]
    fn does_not_inline_call_arguments_that_need_type_annotations() {
        let out = inlined(vec![
            temp("_v0", "i64", int(9223372036854775807)),
            Stmt::Let {
                name: "_v1".into(),
                mutable: false,
                ty: Some(Type::parse("i32")),
                init: Some(call("printf", vec![var("_v0")])),
            },
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i64 = 9223372036854775807;
    let _v1: i32 = printf(_v0);
}
"
        );
    }
}
