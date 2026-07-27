//! Simplify casts whose typed context already preserves behavior.

use crate::fixups::facts::{AstPath, FixupFacts, FunctionId, PathSegment};
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, function_path_location, path_fact, stmt_snippet,
};
use crate::rust_ast::{BinOp, Expr, IndentStmt, Prim, Stmt, Type};

pub(in crate::fixups) fn fixup(
    body: &mut [IndentStmt],
    function: FunctionId,
    facts: &FixupFacts,
) -> bool {
    let mut logger = crate::fixups::trace::NoopLogger;
    UnnecessaryCasts::new(&mut logger).fixup(body, function, facts)
}

pub(in crate::fixups) struct UnnecessaryCasts<'a> {
    logger: &'a mut dyn TraceLogger,
}

impl<'a> UnnecessaryCasts<'a> {
    pub(in crate::fixups) fn new(logger: &'a mut dyn TraceLogger) -> Self {
        Self { logger }
    }

    pub(in crate::fixups) fn fixup(
        &mut self,
        body: &mut [IndentStmt],
        function: FunctionId,
        facts: &FixupFacts,
    ) -> bool {
        fixup_at(body, function, facts, &mut Vec::new(), self.logger)
    }
}

fn fixup_at(
    body: &mut [IndentStmt],
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
    logger: &mut dyn TraceLogger,
) -> bool {
    let mut changed = false;
    for (index, indent) in body.iter_mut().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            walk::nested_body_vecs_mut_with_path(&mut indent.stmt, path, &mut |body, path| {
                changed |= fixup_at(body, function, facts, path, logger);
            });
            let before = logger.is_enabled().then(|| indent.stmt.clone());
            if let Stmt::Assign { value, .. } = &mut indent.stmt {
                let mut expr_path = path.to_vec();
                expr_path.push(PathSegment::Expr(1));
                if simplify_assignment_value(value, function, facts, &expr_path) {
                    if let Some(before) = before {
                        logger.rewrite(RewriteEvent {
                            pass: TracePass::UnnecessaryCasts,
                            kind: "strip_unnecessary_assignment_cast".into(),
                            location: function_path_location(facts, function, &expr_path),
                            before: vec![stmt_snippet("stmt", &before)],
                            after: vec![stmt_snippet("stmt", &indent.stmt)],
                            facts: vec![path_fact("expr_path", &expr_path)],
                        });
                    }
                    changed = true;
                }
            }
        });
    }
    changed
}

fn simplify_assignment_value(
    value: &mut Expr,
    function: FunctionId,
    facts: &FixupFacts,
    path: &[PathSegment],
) -> bool {
    let Some(replacement) = simplified_narrow_binary(value, function, facts, path) else {
        return false;
    };
    *value = replacement;
    true
}

fn simplified_narrow_binary(
    value: &Expr,
    function: FunctionId,
    facts: &FixupFacts,
    path: &[PathSegment],
) -> Option<Expr> {
    let Expr::Cast { expr, ty: outer_ty } = value else {
        return None;
    };
    let Type::Prim(target) = outer_ty else {
        return None;
    };
    if !is_promoted_narrow_int(*target) {
        return None;
    }
    let Expr::Binary { op, lhs, rhs } = &**expr else {
        return None;
    };
    if !matches!(op, BinOp::Add) {
        return None;
    }
    let outer = facts.cast_at(function, &AstPath(path.to_vec()))?;
    if !type_is_prim(outer.from.as_ref(), Prim::I32) || !owned_type_is_prim(&outer.to, *target) {
        return None;
    }
    let lhs_path = binary_child_path(path, 0);
    let rhs_path = binary_child_path(path, 1);
    let lhs = stripped_operand(lhs, *target, function, facts, &lhs_path)?;
    let rhs = stripped_operand(rhs, *target, function, facts, &rhs_path)?;
    Some(Expr::Binary {
        op: *op,
        lhs: Box::new(lhs),
        rhs: Box::new(rhs),
    })
}

fn stripped_operand(
    operand: &Expr,
    target: Prim,
    function: FunctionId,
    facts: &FixupFacts,
    path: &[PathSegment],
) -> Option<Expr> {
    let Expr::Cast { expr, ty } = operand else {
        return None;
    };
    if !owned_type_is_prim(ty, Prim::I32) {
        return None;
    }
    let fact = facts.cast_at(function, &AstPath(path.to_vec()))?;
    if !type_is_prim(fact.from.as_ref(), target) || !owned_type_is_prim(&fact.to, Prim::I32) {
        return None;
    }
    Some((**expr).clone())
}

fn binary_child_path(path: &[PathSegment], child: usize) -> Vec<PathSegment> {
    let mut out = path.to_vec();
    out.push(PathSegment::Expr(0));
    out.push(PathSegment::Expr(child));
    out
}

fn type_is_prim(ty: Option<&Type>, expected: Prim) -> bool {
    matches!(ty, Some(Type::Prim(actual)) if *actual == expected)
}

fn owned_type_is_prim(ty: &Type, expected: Prim) -> bool {
    matches!(ty, Type::Prim(actual) if *actual == expected)
}

fn is_promoted_narrow_int(prim: Prim) -> bool {
    matches!(prim, Prim::I8 | Prim::U8 | Prim::I16 | Prim::U16)
}
