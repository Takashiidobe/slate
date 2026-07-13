//! Fuse a zero-initialized declaration with the assignment that immediately
//! overwrites it, when the assignment does not read the placeholder.

use crate::fixups::facts::{
    AstPath, ConstValue, FixupFacts, FunctionId, PathSegment, PlaceAccess, PlaceKind, ValueSubject,
};
use crate::fixups::idents::expr_ident;
use crate::fixups::support::walk;
use crate::rust_ast::{Block, IndentStmt, Stmt};

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
    if fixup_nested(body, function, facts, path) {
        return true;
    }
    for i in 0..body.len().saturating_sub(1) {
        let decl_path = stmt_path(path, i);
        let Stmt::Let {
            name,
            mutable: true,
            ty: Some(_),
            init: Some(_),
        } = &body[i].stmt
        else {
            continue;
        };
        let Some(binding) =
            facts.binding_by_local_path(function, name, &AstPath(decl_path.clone()))
        else {
            continue;
        };
        if !binding_is_zero(function, facts, binding, &decl_path) {
            continue;
        }
        let assign_path = stmt_path(path, i + 1);
        let Stmt::Assign { target, value } = &body[i + 1].stmt else {
            continue;
        };
        if expr_ident(target) != Some(name.as_str())
            || !assignment_writes_binding(function, facts, binding, &assign_path)
            || assignment_reads_binding(facts, binding, &assign_path)
        {
            continue;
        }
        let value = value.clone();
        if let Stmt::Let { init, .. } = &mut body[i].stmt {
            *init = Some(value);
        }
        body.remove(i + 1);
        return true;
    }
    false
}

fn fixup_nested(
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

fn binding_is_zero(
    function: FunctionId,
    facts: &FixupFacts,
    binding: crate::fixups::facts::BindingId,
    path: &[PathSegment],
) -> bool {
    facts.has_value(
        function,
        ValueSubject::Binding(binding),
        &AstPath(path.to_vec()),
        &ConstValue::Zero,
    )
}

fn assignment_writes_binding(
    function: FunctionId,
    facts: &FixupFacts,
    binding: crate::fixups::facts::BindingId,
    path: &[PathSegment],
) -> bool {
    let Some(name) = facts.binding_name(binding) else {
        return false;
    };
    facts
        .place(function, &AstPath(path.to_vec()))
        .is_some_and(|fact| {
            fact.access == PlaceAccess::Write
                && fact.ordinary_slot
                && matches!(&fact.kind, PlaceKind::Local { name: place } if place == name)
        })
}

fn assignment_reads_binding(
    facts: &FixupFacts,
    binding: crate::fixups::facts::BindingId,
    path: &[PathSegment],
) -> bool {
    facts.def_use(binding).is_some_and(|fact| {
        fact.reads
            .iter()
            .any(|read| read.0.as_slice().starts_with(path))
    })
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
    use crate::rust_ast::{BinOp, Item, Program};

    fn fixed(params: Vec<crate::rust_ast::FnParam>, ret: Option<&str>, stmts: Vec<Stmt>) -> String {
        let mut program = Program {
            items: vec![Item::Fn(func(params, ret, stmts))],
        };
        loop {
            let analyzed = crate::fixups::facts::analyze(program.clone());
            let Item::Fn(f) = &mut program.items[0] else {
                unreachable!();
            };
            if !fixup(&mut f.body, FunctionId(0), &analyzed.facts) {
                break;
            }
        }
        program.emit()
    }

    #[test]
    fn fuses_zero_init_with_immediate_first_assignment() {
        let out = fixed(
            vec![],
            Some("i32"),
            vec![
                let_mut("c", "i32", int(0)),
                assign("c", bin(BinOp::Add, var("a"), var("b"))),
                Stmt::Return(Some(var("c"))),
            ],
        );

        assert_eq!(
            out,
            "\
fn f() -> i32 {
    let mut c: i32 = a + b;
    return c;
}
"
        );
    }

    #[test]
    fn does_not_fuse_when_first_assignment_reads_the_placeholder() {
        let stmts = vec![
            let_mut("c", "i32", int(0)),
            assign("c", bin(BinOp::Add, var("c"), int(1))),
            Stmt::Return(Some(var("c"))),
        ];
        let expected = emit(func(vec![], Some("i32"), stmts.clone()));

        assert_eq!(fixed(vec![], Some("i32"), stmts), expected);
    }

    #[test]
    fn does_not_fuse_when_assignment_is_not_immediate() {
        let stmts = vec![
            let_mut("c", "i32", int(0)),
            Stmt::If {
                cond: var("cond"),
                then_body: vec![IndentStmt {
                    depth: 2,
                    stmt: assign("c", int(1)),
                }],
                else_body: vec![],
            },
            Stmt::Return(Some(var("c"))),
        ];
        let expected = emit(func(
            vec![param("cond", "bool")],
            Some("i32"),
            stmts.clone(),
        ));

        assert_eq!(
            fixed(vec![param("cond", "bool")], Some("i32"), stmts),
            expected
        );
    }

    #[test]
    fn does_not_fuse_non_placeholder_initializers() {
        let stmts = vec![
            let_mut("c", "i32", int(7)),
            assign("c", int(1)),
            Stmt::Return(Some(var("c"))),
        ];
        let expected = emit(func(vec![], Some("i32"), stmts.clone()));

        assert_eq!(fixed(vec![], Some("i32"), stmts), expected);
    }
}
