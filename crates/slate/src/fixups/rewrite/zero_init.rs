//! Fuse a zero-initialized declaration with the assignment that immediately
//! overwrites it, when the assignment does not read the placeholder.

use crate::fixups::facts::{
    AstPath, ConstValue, EffectSubject, FixupFacts, FunctionId, PathSegment, PlaceAccess,
    PlaceKind, Purity, ValueSubject,
};
use crate::fixups::idents::expr_ident;
use crate::fixups::support::walk;
use crate::rust_ast::{Expr, IndentStmt, Stmt, UnaryOp};

pub(in crate::fixups) fn fixup(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
    cross_effects: bool,
) -> bool {
    fixup_at(body, function, facts, cross_effects, &mut Vec::new())
}

fn fixup_at(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
    cross_effects: bool,
    path: &mut Vec<PathSegment>,
) -> bool {
    if fixup_nested(body, function, facts, cross_effects, path) {
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
        let Some(assign_index) = first_overwriting_assignment(
            body,
            i,
            name,
            function,
            facts,
            binding,
            cross_effects,
            path,
        ) else {
            continue;
        };
        let Stmt::Assign { value, .. } = &body[assign_index].stmt else {
            unreachable!();
        };
        let value = value.clone();
        if let Stmt::Let { init, .. } = &mut body[i].stmt {
            *init = Some(value);
        }
        body.remove(assign_index);
        return true;
    }
    false
}

fn first_overwriting_assignment(
    body: &[IndentStmt],
    decl_index: usize,
    name: &str,
    function: FunctionId,
    facts: &FixupFacts,
    binding: crate::fixups::facts::BindingId,
    cross_effects: bool,
    body_path: &[PathSegment],
) -> Option<usize> {
    let (assign_index, value) =
        body.iter()
            .enumerate()
            .skip(decl_index + 1)
            .find_map(|(index, indent)| match &indent.stmt {
                Stmt::Assign { target, value } if expr_ident(target) == Some(name) => {
                    Some((index, value))
                }
                _ => None,
            })?;
    let assign_path = stmt_path(body_path, assign_index);
    if !assignment_writes_binding(function, facts, binding, &assign_path)
        || assignment_reads_binding(facts, binding, &assign_path)
        || assignment_reads_intervening_binding(
            body,
            decl_index,
            assign_index,
            function,
            facts,
            body_path,
            &assign_path,
        )
    {
        return None;
    }
    let value_reads_nothing = reads_nothing(value);
    for index in (decl_index + 1)..assign_index {
        let path = stmt_path(body_path, index);
        if can_cross_intervening_stmt(function, facts, binding, &path, &body[index].stmt) {
            continue;
        }
        // a constant store reads nothing, so removing it is safe past any effect
        // as long as the crossed statement neither reads nor writes the binding.
        if cross_effects
            && value_reads_nothing
            && !assignment_reads_binding(facts, binding, &path)
            && !binding_written_under(facts, binding, &path)
        {
            continue;
        }
        return None;
    }
    Some(assign_index)
}

fn reads_nothing(expr: &Expr) -> bool {
    match expr {
        Expr::Value(_) | Expr::Str(_) | Expr::ByteStr(_) | Expr::CStr(_) | Expr::HexFloat(_) => {
            true
        }
        Expr::Cast { expr, .. } => reads_nothing(expr),
        Expr::Unary { op, expr } => {
            matches!(op, UnaryOp::Neg | UnaryOp::Not) && reads_nothing(expr)
        }
        Expr::Binary { lhs, rhs, .. } => reads_nothing(lhs) && reads_nothing(rhs),
        _ => false,
    }
}

fn binding_written_under(
    facts: &FixupFacts,
    binding: crate::fixups::facts::BindingId,
    path: &[PathSegment],
) -> bool {
    facts.def_use(binding).is_some_and(|fact| {
        fact.writes
            .iter()
            .any(|write| write.0.as_slice().starts_with(path))
    })
}

fn assignment_reads_intervening_binding(
    body: &[IndentStmt],
    decl_index: usize,
    assign_index: usize,
    function: FunctionId,
    facts: &FixupFacts,
    body_path: &[PathSegment],
    assign_path: &[PathSegment],
) -> bool {
    body.iter()
        .enumerate()
        .take(assign_index)
        .skip(decl_index + 1)
        .any(|(index, indent)| {
            let Stmt::Let { name, .. } = &indent.stmt else {
                return false;
            };
            let path = stmt_path(body_path, index);
            facts
                .binding_by_local_path(function, name, &AstPath(path))
                .and_then(|binding| facts.def_use(binding))
                .is_some_and(|def_use| {
                    def_use
                        .reads
                        .iter()
                        .any(|read| read.0.as_slice().starts_with(assign_path))
                })
        })
}

fn can_cross_intervening_stmt(
    function: FunctionId,
    facts: &FixupFacts,
    binding: crate::fixups::facts::BindingId,
    path: &[PathSegment],
    stmt: &Stmt,
) -> bool {
    let Stmt::Let { init, .. } = stmt else {
        return false;
    };
    if assignment_reads_binding(facts, binding, path) {
        return false;
    }
    init.is_none()
        || facts
            .effect(function, EffectSubject::Expr, &AstPath(path.to_vec()))
            .is_some_and(|fact| fact.purity == Purity::MovablePure)
}

fn fixup_nested(
    body: &mut [IndentStmt],
    function: FunctionId,
    facts: &FixupFacts,
    cross_effects: bool,
    path: &mut Vec<PathSegment>,
) -> bool {
    for (index, stmt) in body.iter_mut().enumerate() {
        let mut changed = false;
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            walk::nested_body_vecs_mut_with_path(&mut stmt.stmt, path, &mut |body, path| {
                if !changed && fixup_at(body, function, facts, cross_effects, path) {
                    changed = true;
                }
            });
        });
        if changed {
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
        && facts.def_use(binding).is_some_and(|fact| {
            fact.writes
                .iter()
                .any(|write| write.0.as_slice().starts_with(path))
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
    use crate::rust_ast::{BinOp, Block, Expr, Item, MatchArm, Pattern, Program};

    fn fixed(params: Vec<crate::rust_ast::FnParam>, ret: Option<&str>, stmts: Vec<Stmt>) -> String {
        fixed_with(params, ret, stmts, false)
    }

    fn fixed_crossing(
        params: Vec<crate::rust_ast::FnParam>,
        ret: Option<&str>,
        stmts: Vec<Stmt>,
    ) -> String {
        fixed_with(params, ret, stmts, true)
    }

    fn fixed_with(
        params: Vec<crate::rust_ast::FnParam>,
        ret: Option<&str>,
        stmts: Vec<Stmt>,
        cross_effects: bool,
    ) -> String {
        let mut program = Program {
            items: vec![Item::Fn(func(params, ret, stmts))],
        };
        loop {
            let analyzed = crate::fixups::facts::analyze(program.clone());
            let Item::Fn(f) = &mut program.items[0] else {
                unreachable!();
            };
            if !fixup(&mut f.body, FunctionId(0), &analyzed.facts, cross_effects) {
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
    fn fuses_repeated_zero_array_with_immediate_literal_assignment() {
        let out = fixed(
            vec![],
            None,
            vec![
                let_mut(
                    "a",
                    "[i32; 3]",
                    Expr::ArrayRepeat {
                        elem: Box::new(int(0)),
                        len: 3,
                    },
                ),
                Stmt::Assign {
                    target: var("a"),
                    value: Expr::ArrayLit(vec![int(1), int(2), int(3)]),
                },
            ],
        );

        assert_eq!(
            out,
            "\
fn f() {
    let mut a: [i32; 3] = [1, 2, 3];
}
"
        );
    }

    #[test]
    fn fuses_zero_init_with_immediate_effectful_assignment() {
        let out = fixed(
            vec![],
            Some("i32"),
            vec![
                let_mut("c", "i32", int(0)),
                assign("c", call("next_arg", vec![])),
                Stmt::Return(Some(var("c"))),
            ],
        );

        assert_eq!(
            out,
            "\
fn f() -> i32 {
    let mut c: i32 = next_arg();
    return c;
}
"
        );
    }

    #[test]
    fn fuses_zero_init_with_immediate_unsafe_effectful_assignment() {
        let out = fixed(
            vec![],
            Some("i32"),
            vec![
                let_mut("c", "i32", int(0)),
                assign(
                    "c",
                    crate::rust_ast::Expr::Unsafe(Box::new(Block {
                        stmts: Vec::new(),
                        tail: Some(Box::new(call("next_arg", vec![]))),
                    })),
                ),
                Stmt::Return(Some(var("c"))),
            ],
        );

        assert_eq!(
            out,
            "\
fn f() -> i32 {
    let mut c: i32 = unsafe { next_arg() };
    return c;
}
"
        );
    }

    #[test]
    fn fuses_zero_init_inside_match_arm() {
        let out = fixed(
            vec![],
            None,
            vec![Stmt::Match {
                expr: var("state"),
                arms: vec![MatchArm {
                    pattern: Pattern::I64(0),
                    body: vec![
                        IndentStmt {
                            depth: 0,
                            stmt: let_mut("c", "i32", int(0)),
                        },
                        IndentStmt {
                            depth: 0,
                            stmt: assign("c", bin(BinOp::Add, var("a"), var("b"))),
                        },
                    ],
                }],
            }],
        );

        assert_eq!(
            out,
            "\
fn f() {
    match state {
        0 => {
            let mut c: i32 = a + b;
        }
    }
}
"
        );
    }

    #[test]
    fn fuses_zero_init_across_pure_placeholder_declarations() {
        let out = fixed(
            vec![],
            Some("i32"),
            vec![
                let_mut("first", "i32", int(0)),
                let_mut("second", "i32", int(0)),
                assign("first", call("next_arg", vec![])),
                assign("second", call("next_arg", vec![])),
                Stmt::Return(Some(bin(BinOp::Add, var("first"), var("second")))),
            ],
        );

        assert_eq!(
            out,
            "\
fn f() -> i32 {
    let mut first: i32 = next_arg();
    let mut second: i32 = next_arg();
    return first + second;
}
"
        );
    }

    #[test]
    fn does_not_cross_declaration_read_by_assignment() {
        let stmts = vec![
            let_mut("__retval", "i32", int(0)),
            temp("_v1", "i32", int(1)),
            assign("__retval", var("_v1")),
            Stmt::Return(Some(var("__retval"))),
        ];
        let expected = emit(func(vec![], Some("i32"), stmts.clone()));

        assert_eq!(fixed(vec![], Some("i32"), stmts), expected);
    }

    #[test]
    fn folds_shadowed_name_assignment_into_matching_binding_only() {
        let out = fixed(
            vec![],
            Some("i32"),
            vec![
                let_mut("_atomictmp", "i32", int(0)),
                let_mut("_atomictmp", "i32", int(0)),
                assign("_atomictmp", int(100)),
                Stmt::Return(Some(var("_atomictmp"))),
            ],
        );

        assert_eq!(
            out,
            "\
fn f() -> i32 {
    let mut _atomictmp: i32 = 0;
    let mut _atomictmp: i32 = 100;
    return _atomictmp;
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

    #[test]
    fn crossing_mode_drops_constant_store_past_effectful_statement() {
        let out = fixed_crossing(
            vec![],
            None,
            vec![
                let_mut("x", "i32", int(0)),
                Stmt::Expr(call("side_effect", vec![])),
                assign("x", int(0)),
            ],
        );

        assert_eq!(
            out,
            "\
fn f() {
    let mut x: i32 = 0;
    side_effect();
}
"
        );
    }

    #[test]
    fn conservative_mode_keeps_constant_store_past_effectful_statement() {
        let stmts = vec![
            let_mut("x", "i32", int(0)),
            Stmt::Expr(call("side_effect", vec![])),
            assign("x", int(0)),
        ];
        let expected = emit(func(vec![], None, stmts.clone()));

        assert_eq!(fixed(vec![], None, stmts), expected);
    }

    #[test]
    fn crossing_mode_keeps_reading_store_past_effectful_statement() {
        let stmts = vec![
            let_mut("x", "i32", int(0)),
            Stmt::Expr(call("side_effect", vec![])),
            assign("x", var("y")),
        ];
        let expected = emit(func(vec![], None, stmts.clone()));

        assert_eq!(fixed_crossing(vec![], None, stmts), expected);
    }
}
