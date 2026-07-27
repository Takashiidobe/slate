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
