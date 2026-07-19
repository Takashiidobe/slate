//! Recover compound assignment (`a -= 5`) from the `store(binop(load a, rhs), a)`
//! shape that CIR lowers `a -= 5` into. CIR cannot distinguish `a -= 5` from
//! `a = a - 5`; when both spellings are equivalent this pass prefers the compound
//! form. Restricted to simple local slots (a plain variable target) with a pure
//! rhs, so it never reorders a side effect or touches a volatile/complex lvalue.

use crate::fixups::facts::{AstPath, EffectSubject, FixupFacts, FunctionId, PathSegment, Purity};
use crate::fixups::idents::expr_ident;
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, function_path_location, path_fact, stmt_snippet,
};
use crate::rust_ast::{BinOp, Expr, IndentStmt, Stmt};

pub(in crate::fixups) fn fixup(body: &mut [IndentStmt], function: FunctionId, facts: &FixupFacts) {
    let mut logger = crate::fixups::trace::NoopLogger;
    CompoundAssign::new(&mut logger).fixup(body, function, facts);
}

pub(in crate::fixups) struct CompoundAssign<'a> {
    logger: &'a mut dyn TraceLogger,
}

impl<'a> CompoundAssign<'a> {
    pub(in crate::fixups) fn new(logger: &'a mut dyn TraceLogger) -> Self {
        Self { logger }
    }

    pub(in crate::fixups) fn fixup(
        &mut self,
        body: &mut [IndentStmt],
        function: FunctionId,
        facts: &FixupFacts,
    ) {
        fixup_at(body, function, facts, &mut Vec::new(), self.logger);
    }
}

fn fixup_at(
    body: &mut [IndentStmt],
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
    logger: &mut dyn TraceLogger,
) {
    for index in 0..body.len() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            walk::nested_body_vecs_mut_with_path(&mut body[index].stmt, path, &mut |body, path| {
                fixup_at(body, function, facts, path, logger);
            });

            if recover_temp_backed_compound_assign(body, index, function, facts, path, logger) {
                return;
            }

            let before = logger.is_enabled().then(|| body[index].stmt.clone());
            if let Stmt::Assign { target, value } = &mut body[index].stmt
                && let Some((op, rhs)) = compound_parts(target, value, function, facts, path)
            {
                body[index].stmt = Stmt::CompoundAssign {
                    target: target.clone(),
                    op,
                    value: rhs,
                };
                if let Some(before) = before {
                    logger.rewrite(RewriteEvent {
                        pass: TracePass::CompoundAssign,
                        kind: "recover_compound_assign".into(),
                        location: function_path_location(facts, function, path),
                        before: vec![stmt_snippet("stmt", &before)],
                        after: vec![stmt_snippet("stmt", &body[index].stmt)],
                        facts: vec![path_fact("stmt_path", path)],
                    });
                }
            }
        });
    }
}

fn recover_temp_backed_compound_assign(
    body: &mut [IndentStmt],
    index: usize,
    function: FunctionId,
    facts: &FixupFacts,
    path: &[PathSegment],
    logger: &mut dyn TraceLogger,
) -> bool {
    recover_post_update(body, index, function, facts, path, logger)
        || recover_pre_update(body, index, function, facts, path, logger)
}

fn recover_post_update(
    body: &mut [IndentStmt],
    index: usize,
    function: FunctionId,
    facts: &FixupFacts,
    path: &[PathSegment],
    logger: &mut dyn TraceLogger,
) -> bool {
    let Some(previous) = index.checked_sub(1) else {
        return false;
    };
    let Some((temp, source)) = temp_copy(&body[previous].stmt) else {
        return false;
    };
    let Stmt::Assign { target, value } = &body[index].stmt else {
        return false;
    };
    if expr_ident(target) != Some(source) {
        return false;
    }
    let Some((op, rhs)) = temp_compound_value(value, temp, function, facts, path) else {
        return false;
    };

    let before = logger
        .is_enabled()
        .then(|| vec![body[previous].stmt.clone(), body[index].stmt.clone()]);
    body[index].stmt = Stmt::CompoundAssign {
        target: target.clone(),
        op,
        value: rhs,
    };
    if let Some(before) = before {
        logger.rewrite(RewriteEvent {
            pass: TracePass::CompoundAssign,
            kind: "recover_post_update_compound_assign".into(),
            location: function_path_location(facts, function, path),
            before: before
                .iter()
                .enumerate()
                .map(|(i, stmt)| stmt_snippet(format!("stmt{i}"), stmt))
                .collect(),
            after: vec![stmt_snippet("stmt", &body[index].stmt)],
            facts: vec![path_fact("stmt_path", path)],
        });
    }
    true
}

fn recover_pre_update(
    body: &mut [IndentStmt],
    index: usize,
    function: FunctionId,
    facts: &FixupFacts,
    path: &[PathSegment],
    logger: &mut dyn TraceLogger,
) -> bool {
    let Some(next) = index.checked_add(1).filter(|next| *next < body.len()) else {
        return false;
    };
    let Stmt::Let {
        name,
        mutable,
        ty,
        init: Some(value),
    } = &body[index].stmt
    else {
        return false;
    };
    let Stmt::Assign {
        target,
        value: assigned,
    } = &body[next].stmt
    else {
        return false;
    };
    if expr_ident(assigned) != Some(name.as_str()) {
        return false;
    }
    let Some((op, rhs)) = compound_parts(target, value, function, facts, path) else {
        return false;
    };

    let before = logger
        .is_enabled()
        .then(|| vec![body[index].stmt.clone(), body[next].stmt.clone()]);
    let temp_name = name.clone();
    let temp_mutable = *mutable;
    let temp_ty = ty.clone();
    let target = target.clone();
    body[index].stmt = Stmt::CompoundAssign {
        target: target.clone(),
        op,
        value: rhs,
    };
    body[next].stmt = Stmt::Let {
        name: temp_name,
        mutable: temp_mutable,
        ty: temp_ty,
        init: Some(target),
    };
    if let Some(before) = before {
        logger.rewrite(RewriteEvent {
            pass: TracePass::CompoundAssign,
            kind: "recover_pre_update_compound_assign".into(),
            location: function_path_location(facts, function, path),
            before: before
                .iter()
                .enumerate()
                .map(|(i, stmt)| stmt_snippet(format!("stmt{i}"), stmt))
                .collect(),
            after: vec![
                stmt_snippet("stmt0", &body[index].stmt),
                stmt_snippet("stmt1", &body[next].stmt),
            ],
            facts: vec![path_fact("stmt_path", path)],
        });
    }
    true
}

fn temp_copy(stmt: &Stmt) -> Option<(&str, &str)> {
    let Stmt::Let {
        name,
        init: Some(init),
        ..
    } = stmt
    else {
        return None;
    };
    if !is_temp_name(name) {
        return None;
    }
    Some((name.as_str(), expr_ident(init)?))
}

fn temp_compound_value(
    value: &Expr,
    temp: &str,
    function: FunctionId,
    facts: &FixupFacts,
    path: &[PathSegment],
) -> Option<(BinOp, Expr)> {
    let Expr::Binary { op, lhs, rhs } = value else {
        return None;
    };
    let mut rhs_path = path.to_vec();
    rhs_path.push(PathSegment::Expr(1));
    if !is_compound_op(*op)
        || expr_ident(lhs) != Some(temp)
        || !is_pure_expr(function, facts, &rhs_path)
    {
        return None;
    }
    Some((*op, (**rhs).clone()))
}

fn compound_parts(
    target: &Expr,
    value: &Expr,
    function: FunctionId,
    facts: &FixupFacts,
    path: &[PathSegment],
) -> Option<(BinOp, Expr)> {
    let name = expr_ident(target)?;
    let Expr::Binary { op, lhs, rhs } = value else {
        return None;
    };
    let mut rhs_path = path.to_vec();
    rhs_path.push(PathSegment::Expr(1));
    if !is_compound_op(*op)
        || expr_ident(lhs) != Some(name)
        || !is_pure_expr(function, facts, &rhs_path)
    {
        return None;
    }
    Some((*op, (**rhs).clone()))
}

fn is_temp_name(name: &str) -> bool {
    name.strip_prefix("_v")
        .is_some_and(|rest| rest.chars().all(|ch| ch.is_ascii_digit()))
}

fn is_compound_op(op: BinOp) -> bool {
    matches!(
        op,
        BinOp::Add
            | BinOp::Sub
            | BinOp::Mul
            | BinOp::Div
            | BinOp::Rem
            | BinOp::Shl
            | BinOp::Shr
            | BinOp::BitAnd
            | BinOp::BitOr
            | BinOp::BitXor
    )
}

fn is_pure_expr(function: FunctionId, facts: &FixupFacts, path: &[PathSegment]) -> bool {
    facts
        .effect(function, EffectSubject::Expr, &AstPath(path.to_vec()))
        .is_some_and(|fact| fact.purity == Purity::MovablePure)
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Item, MatchArm, Pattern, Program, Type};

    fn after_facts(
        params: Vec<crate::rust_ast::FnParam>,
        ret: Option<&str>,
        stmts: Vec<Stmt>,
    ) -> String {
        let mut program = Program {
            items: vec![Item::Fn(func(params, ret, stmts))],
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
    fn recovers_every_arithmetic_and_bitwise_form() {
        let out = after_facts(
            vec![],
            None,
            vec![
                let_mut("a", "i32", int(20)),
                assign("a", bin(BinOp::Sub, var("a"), int(5))),
                assign("a", bin(BinOp::Mul, var("a"), int(3))),
                assign("a", bin(BinOp::Div, var("a"), int(5))),
                assign("a", bin(BinOp::Rem, var("a"), int(7))),
                assign("a", bin(BinOp::Shl, var("a"), int(3))),
                assign("a", bin(BinOp::Shr, var("a"), int(2))),
                assign("a", bin(BinOp::BitAnd, var("a"), int(6))),
                assign("a", bin(BinOp::BitXor, var("a"), int(3))),
                assign("a", bin(BinOp::BitOr, var("a"), int(8))),
            ],
        );

        assert_eq!(
            out,
            "\
fn f() {
    let mut a: i32 = 20;
    a -= 5;
    a *= 3;
    a /= 5;
    a %= 7;
    a <<= 3;
    a >>= 2;
    a &= 6;
    a ^= 3;
    a |= 8;
}
"
        );
    }

    #[test]
    fn recovers_post_decrement_temp_assignment() {
        let out = after_facts(
            vec![],
            None,
            vec![
                let_mut("a", "i32", int(5)),
                Stmt::Let {
                    name: "_v1".into(),
                    mutable: false,
                    ty: Some(Type::parse("i32")),
                    init: Some(var("a")),
                },
                assign("a", bin(BinOp::Sub, var("_v1"), int(1))),
                Stmt::Let {
                    name: "post".into(),
                    mutable: false,
                    ty: Some(Type::parse("i32")),
                    init: Some(var("_v1")),
                },
            ],
        );

        assert!(out.contains("let _v1: i32 = a;"));
        assert!(out.contains("a -= 1;"));
        assert!(out.contains("let post: i32 = _v1;"));
        assert!(!out.contains("a = _v1 - 1;"));
    }

    #[test]
    fn recovers_pre_decrement_temp_assignment() {
        let out = after_facts(
            vec![],
            None,
            vec![
                let_mut("a", "i32", int(5)),
                Stmt::Let {
                    name: "_v1".into(),
                    mutable: false,
                    ty: Some(Type::parse("i32")),
                    init: Some(bin(BinOp::Sub, var("a"), int(1))),
                },
                assign("a", var("_v1")),
                Stmt::Let {
                    name: "pre".into(),
                    mutable: false,
                    ty: Some(Type::parse("i32")),
                    init: Some(var("_v1")),
                },
            ],
        );

        assert!(out.contains("a -= 1;"));
        assert!(out.contains("let _v1: i32 = a;"));
        assert!(out.contains("let pre: i32 = _v1;"));
        assert!(!out.contains("let _v1: i32 = a - 1;"));
        assert!(!out.contains("a = _v1;"));
    }

    #[test]
    fn keeps_assignment_when_lhs_is_not_the_target() {
        let out = after_facts(
            vec![],
            None,
            vec![
                let_mut("a", "i32", int(20)),
                assign("a", bin(BinOp::Sub, int(5), var("a"))),
            ],
        );

        assert_eq!(
            out,
            "\
fn f() {
    let mut a: i32 = 20;
    a = 5 - a;
}
"
        );
    }

    #[test]
    fn recovers_compound_assignment_inside_match_arms() {
        let out = after_facts(
            vec![],
            None,
            vec![Stmt::Match {
                expr: var("state"),
                arms: vec![MatchArm {
                    pattern: Pattern::I64(0),
                    body: vec![IndentStmt {
                        depth: 0,
                        stmt: assign("sum", bin(BinOp::Add, var("sum"), var("i"))),
                    }],
                }],
            }],
        );

        assert_eq!(
            out,
            "\
fn f() {
    match state {
        0 => {
            sum += i;
        }
    }
}
"
        );
    }

    #[test]
    fn keeps_assignment_when_rhs_is_impure() {
        let out = after_facts(
            vec![],
            None,
            vec![
                let_mut("a", "i32", int(20)),
                assign("a", bin(BinOp::Add, var("a"), call("g", vec![]))),
            ],
        );

        assert_eq!(
            out,
            "\
fn f() {
    let mut a: i32 = 20;
    a = a + g();
}
"
        );
    }

    #[test]
    fn keeps_comparison_assignment() {
        let out = after_facts(
            vec![],
            None,
            vec![
                Stmt::Let {
                    name: "b".into(),
                    mutable: true,
                    ty: Some(Type::parse("bool")),
                    init: Some(Expr::Value(crate::rust_ast::RustValue::Bool(false))),
                },
                assign("b", bin(BinOp::Lt, var("b"), int(1))),
            ],
        );

        assert!(out.contains("b = b < 1;"));
    }
}
