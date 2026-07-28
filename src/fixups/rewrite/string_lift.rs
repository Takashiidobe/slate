use crate::fixups::Fixup;
use crate::fixups::facts::{
    AstPath, FixupFacts, FunctionId, PathSegment, StringBufferProvenance, StringRecoveryCandidate,
};
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, fact, function_path_location, stmts_snippet,
};
use crate::rust_ast::{Expr, IndentStmt, Prim, Stmt, Type};
use std::collections::BTreeSet;

pub(in crate::fixups) struct StringLift<'a> {
    pass: TracePass,
    function: FunctionId,
    facts: &'a FixupFacts,
    recoveries: Vec<StringRecoveryCandidate>,
    logger: &'a mut dyn TraceLogger,
}

impl Fixup for StringLift<'_> {
    fn fixup(&mut self, body: &mut Vec<IndentStmt>) -> bool {
        let before = self.logger.is_enabled().then(|| body.clone());
        let mut changed = fixup_nested(
            body,
            self.function,
            self.facts,
            &mut Vec::new(),
            &self.recoveries,
        );
        changed |= fixup_body(
            body,
            self.function,
            self.facts,
            &Vec::new(),
            &self.recoveries,
        );
        if changed && let Some(before) = before {
            self.logger.rewrite(RewriteEvent {
                pass: self.pass,
                kind: "lift_string_buffer".into(),
                location: function_path_location(self.facts, self.function, &[]),
                before: vec![stmts_snippet("body", &before)],
                after: vec![stmts_snippet("body", body)],
                facts: vec![
                    fact("recoveries", self.recoveries.len().to_string()),
                    fact(
                        "string_lift_plans",
                        self.facts.string_lift_plans.len().to_string(),
                    ),
                ],
            });
        }
        changed
    }
}

impl<'a> StringLift<'a> {
    pub(in crate::fixups) fn new(
        pass: TracePass,
        function: FunctionId,
        facts: &'a FixupFacts,
        recoveries: &[StringRecoveryCandidate],
        logger: &'a mut dyn TraceLogger,
    ) -> Self {
        Self {
            pass,
            function,
            facts,
            recoveries: recoveries.to_vec(),
            logger,
        }
    }
}

fn fixup_body(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
    path: &[PathSegment],
    recoveries: &[StringRecoveryCandidate],
) -> bool {
    let mut removals = BTreeSet::new();
    let mut changed = false;
    let mut i = 0;
    while i < body.len() {
        let stmt_path = stmt_path(path, i);
        let Some((name, lifted, remove_index)) =
            lift_candidate(body, function, facts, &stmt_path, recoveries)
        else {
            i += 1;
            continue;
        };
        changed = true;
        let Stmt::Let {
            mutable, ty, init, ..
        } = &mut body[i].stmt
        else {
            unreachable!();
        };
        *mutable = false;
        *ty = Some(lifted.ty);
        *init = Some(lifted.expr);
        if let Some(remove_index) = remove_index {
            removals.insert(remove_index);
        }
        for indent in body.iter_mut().skip(i + 1) {
            rewrite_stmt_pointer_views(&mut indent.stmt, &name);
        }
        i += 1;
    }
    for index in removals.into_iter().rev() {
        body.remove(index);
    }
    changed
}

fn fixup_nested(
    body: &mut [IndentStmt],
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
    recoveries: &[StringRecoveryCandidate],
) -> bool {
    let mut changed = false;
    for (index, indent) in body.iter_mut().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            match &mut indent.stmt {
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
                        changed |= fixup_nested(then_body, function, facts, path, recoveries);
                        changed |= fixup_body(then_body, function, facts, path, recoveries);
                    });
                    walk::with_path_segment(path, PathSegment::Else, |path| {
                        changed |= fixup_nested(else_body, function, facts, path, recoveries);
                        changed |= fixup_body(else_body, function, facts, path, recoveries);
                    });
                }
                Stmt::Loop { body, .. }
                | Stmt::Scope { body }
                | Stmt::LabeledBlock { body, .. } => {
                    changed |= fixup_nested(body, function, facts, path, recoveries);
                    changed |= fixup_body(body, function, facts, path, recoveries);
                }
                Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
                    changed |= fixup_nested(&mut body.stmts, function, facts, path, recoveries);
                    changed |= fixup_body(&mut body.stmts, function, facts, path, recoveries);
                }
                Stmt::Match { arms, .. } => {
                    for (arm_index, arm) in arms.iter_mut().enumerate() {
                        walk::with_path_segment(path, PathSegment::MatchArm(arm_index), |path| {
                            changed |=
                                fixup_nested(&mut arm.body, function, facts, path, recoveries);
                            changed |= fixup_body(&mut arm.body, function, facts, path, recoveries);
                        });
                    }
                }
                _ => {}
            }
        });
    }
    changed
}

struct Lifted {
    ty: Type,
    expr: Expr,
}

fn lift_candidate(
    body: &[IndentStmt],
    function: FunctionId,
    facts: &FixupFacts,
    path: &[PathSegment],
    recoveries: &[StringRecoveryCandidate],
) -> Option<(String, Lifted, Option<usize>)> {
    let buffer = facts.string_buffer_at(function, &AstPath(path.to_vec()))?;
    let name = facts.binding_name(buffer.binding)?.to_owned();
    let plan = facts.string_lift_plans.iter().find(|plan| {
        plan.site.function == function
            && plan.binding == buffer.binding
            && plan.site.path.0 == path
            && recoveries.contains(&plan.recovery)
    })?;
    let lifted = lifted_buffer(buffer, plan.recovery)?;
    let remove_index = match &buffer.provenance {
        StringBufferProvenance::Literal => None,
        StringBufferProvenance::AssignedLiteral { .. } => plan
            .remove_assignment
            .as_ref()
            .and_then(|assignment| assignment_index(path, &assignment.0)),
        _ => return None,
    };
    if remove_index.is_some_and(|index| index >= body.len()) {
        return None;
    }
    Some((name, lifted, remove_index))
}

fn lifted_buffer(
    buffer: &crate::fixups::facts::StringBufferFact,
    recovery: StringRecoveryCandidate,
) -> Option<Lifted> {
    let bytes = buffer.bytes.clone()?;
    if recovery == StringRecoveryCandidate::BorrowedStr {
        let text = String::from_utf8(bytes).ok()?;
        return Some(Lifted {
            ty: str_ref_type(),
            expr: Expr::Str(text),
        });
    }
    if recovery == StringRecoveryCandidate::BorrowedBytes {
        return Some(Lifted {
            ty: byte_slice_ref_type(),
            expr: Expr::ByteStr(bytes),
        });
    }
    if recovery == StringRecoveryCandidate::BorrowedCStr {
        if !buffer.ascii_only || buffer.interior_nul {
            return None;
        }
        return Some(Lifted {
            ty: cstr_ref_type(),
            expr: Expr::CStr(bytes),
        });
    }
    None
}

fn str_ref_type() -> Type {
    Type::Ref {
        mutable: false,
        inner: Box::new(Type::Str),
    }
}

fn byte_slice_ref_type() -> Type {
    Type::Ref {
        mutable: false,
        inner: Box::new(Type::Slice(Box::new(Type::Prim(Prim::U8)))),
    }
}

fn cstr_ref_type() -> Type {
    Type::Ref {
        mutable: false,
        inner: Box::new(Type::Custom("core::ffi::CStr".into())),
    }
}

fn stmt_path(path: &[PathSegment], index: usize) -> Vec<PathSegment> {
    let mut path = path.to_vec();
    path.push(PathSegment::Stmt(index));
    path
}

fn assignment_index(def_path: &[PathSegment], assignment_path: &[PathSegment]) -> Option<usize> {
    let parent = def_path.get(..def_path.len().checked_sub(1)?)?;
    let assignment_parent = assignment_path.get(..assignment_path.len().checked_sub(1)?)?;
    if assignment_parent != parent {
        return None;
    }
    match assignment_path.last()? {
        PathSegment::Stmt(index) => Some(*index),
        _ => None,
    }
}

fn rewrite_stmt_pointer_views(stmt: &mut Stmt, name: &str) {
    walk::stmt_exprs_mut_with(stmt, &mut |expr| rewrite_pointer_view_expr(expr, name));
}

fn rewrite_expr_pointer_views(expr: &mut Expr, name: &str) {
    walk::exprs_mut_with(expr, &mut |expr| rewrite_pointer_view_expr(expr, name));
}

fn rewrite_pointer_view_expr(expr: &mut Expr, name: &str) -> bool {
    match expr {
        Expr::MethodCall { recv, method, args }
            if args.is_empty()
                && matches!(&**recv, Expr::Var(v) if v.as_str() == name)
                && matches!(method.as_str(), "as_ptr" | "as_mut_ptr") =>
        {
            *expr = Expr::Cast {
                expr: Box::new(Expr::MethodCall {
                    recv: Box::new(Expr::Var(name.into())),
                    method: "as_ptr".into(),
                    args: Vec::new(),
                }),
                ty: Type::Ptr {
                    mutable: true,
                    inner: Box::new(Type::Prim(Prim::I8)),
                },
            };
            false
        }
        Expr::ArrayPtr { array, .. } if matches!(&**array, Expr::Var(v) if v.as_str() == name) => {
            *expr = Expr::Cast {
                expr: Box::new(Expr::MethodCall {
                    recv: Box::new(Expr::Var(name.into())),
                    method: "as_ptr".into(),
                    args: Vec::new(),
                }),
                ty: Type::Ptr {
                    mutable: true,
                    inner: Box::new(Type::Prim(Prim::I8)),
                },
            };
            false
        }
        _ => true,
    }
}
