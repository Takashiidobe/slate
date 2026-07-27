use crate::fixups::facts::{
    AstPath, FixupFacts, FunctionId, PathSegment, StringBufferProvenance, StringCopyRewrite,
    StringRecoveryCandidate,
};
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLocation, TraceLogger, TraceSnippet, fact,
};
use crate::rust_ast::{Block, Expr, IndentStmt, Item, Program, Stmt, Type};
use std::collections::BTreeSet;

pub(in crate::fixups) fn fixup(program: &mut Program, facts: &FixupFacts) {
    let mut logger = crate::fixups::trace::NoopLogger;
    StringCopy::new(&mut logger).fixup(program, facts);
}

pub(in crate::fixups) struct StringCopy<'a> {
    logger: &'a mut dyn TraceLogger,
}

impl<'a> StringCopy<'a> {
    pub(in crate::fixups) fn new(logger: &'a mut dyn TraceLogger) -> Self {
        Self { logger }
    }

    pub(in crate::fixups) fn fixup(&mut self, program: &mut Program, facts: &FixupFacts) {
        let before = self.logger.is_enabled().then(|| program.emit());
        fixup_impl(program, facts);
        if let Some(before) = before {
            let after = program.emit();
            if before != after {
                self.logger.rewrite(RewriteEvent {
                    pass: TracePass::StringCopy,
                    kind: "rewrite_string_copy_idioms".into(),
                    location: TraceLocation::default(),
                    before: vec![TraceSnippet::new("program", before.trim_end())],
                    after: vec![TraceSnippet::new("program", after.trim_end())],
                    facts: vec![fact(
                        "string_copy_rewrites",
                        facts.string_copy_rewrites.len().to_string(),
                    )],
                });
            }
        }
    }
}

fn fixup_impl(program: &mut Program, facts: &FixupFacts) {
    for (item_index, item) in program.items.iter_mut().enumerate() {
        if let Item::Fn(f) = item
            && let Some(function) = facts.function_by_item_index(item_index)
        {
            fixup_body(&mut f.body, function, facts);
        }
    }
}

#[derive(Clone)]
struct Candidate {
    init: Expr,
    remove_index: Option<usize>,
}

fn fixup_body(body: &mut Vec<IndentStmt>, function: FunctionId, facts: &FixupFacts) {
    fixup_body_at(body, function, facts, &mut Vec::new());
}

fn fixup_body_at(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
) {
    fixup_nested(body, function, facts, path);
    let liftable = liftable_names(body, function, facts, path);
    let mut remove = Vec::new();
    for i in 0..body.len() {
        let Some(candidate) = candidate_at(body, function, facts, &stmt_path(path, i)) else {
            continue;
        };
        let Stmt::Let { name, ty, init, .. } = &mut body[i].stmt else {
            continue;
        };
        if !liftable.contains(name) {
            continue;
        }
        *ty = Some(Type::Custom("String".into()));
        *init = Some(to_owned(candidate.init));
        if let Some(remove_index) = candidate.remove_index {
            remove.push(remove_index);
        }
    }
    rewrite_body(body, function, facts, path, &liftable);
    for i in remove.into_iter().rev() {
        body.remove(i);
    }
}

fn liftable_names(
    body: &[IndentStmt],
    function: FunctionId,
    facts: &FixupFacts,
    path: &[PathSegment],
) -> BTreeSet<String> {
    (0..body.len())
        .filter_map(|i| {
            let buffer = facts.string_buffer_at(function, &AstPath(stmt_path(path, i)))?;
            facts
                .string_lift_plans
                .iter()
                .any(|plan| {
                    plan.site.function == function
                        && plan.binding == buffer.binding
                        && plan.site.path == buffer.site.path
                        && plan.recovery == StringRecoveryCandidate::OwnedString
                })
                .then(|| facts.binding_name(buffer.binding).map(str::to_owned))
                .flatten()
        })
        .collect()
}

fn candidate_at(
    body: &[IndentStmt],
    function: FunctionId,
    facts: &FixupFacts,
    path: &[PathSegment],
) -> Option<Candidate> {
    let buffer = facts.string_buffer_at(function, &AstPath(path.to_vec()))?;
    if !buffer
        .candidates
        .contains(&StringRecoveryCandidate::OwnedString)
    {
        return None;
    }
    let plan = facts.string_lift_plans.iter().find(|plan| {
        plan.site.function == function
            && plan.binding == buffer.binding
            && plan.site.path.0 == path
            && plan.recovery == StringRecoveryCandidate::OwnedString
    })?;
    let init = match &buffer.provenance {
        StringBufferProvenance::ZeroInitialized => String::new(),
        StringBufferProvenance::Literal => String::from_utf8(buffer.bytes.clone()?).ok()?,
        StringBufferProvenance::AssignedLiteral { .. } => {
            String::from_utf8(buffer.bytes.clone()?).ok()?
        }
        _ => return None,
    };
    let remove_index = match &buffer.provenance {
        StringBufferProvenance::AssignedLiteral { .. } => plan
            .remove_assignment
            .as_ref()
            .and_then(|assignment| assignment_index(path, &assignment.0)),
        _ => None,
    };
    if remove_index.is_some_and(|index| index >= body.len()) {
        return None;
    }
    Some(Candidate {
        init: Expr::Str(init),
        remove_index,
    })
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

fn fixup_nested(
    body: &mut [IndentStmt],
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
) {
    for (index, indent) in body.iter_mut().enumerate() {
        crate::fixups::support::walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
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
                    crate::fixups::support::walk::with_path_segment(
                        path,
                        PathSegment::Then,
                        |path| {
                            fixup_body_at(then_body, function, facts, path);
                        },
                    );
                    crate::fixups::support::walk::with_path_segment(
                        path,
                        PathSegment::Else,
                        |path| {
                            fixup_body_at(else_body, function, facts, path);
                        },
                    );
                }
                Stmt::Loop { body, .. }
                | Stmt::Scope { body }
                | Stmt::LabeledBlock { body, .. } => {
                    fixup_body_at(body, function, facts, path);
                }
                Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
                    fixup_body_at(&mut body.stmts, function, facts, path);
                }
                Stmt::Match { arms, .. } => {
                    for (arm_index, arm) in arms.iter_mut().enumerate() {
                        crate::fixups::support::walk::with_path_segment(
                            path,
                            PathSegment::MatchArm(arm_index),
                            |path| fixup_body_at(&mut arm.body, function, facts, path),
                        );
                    }
                }
                _ => {}
            }
        });
    }
}

fn rewrite_body(
    body: &mut [IndentStmt],
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
    liftable: &BTreeSet<String>,
) {
    for (index, indent) in body.iter_mut().enumerate() {
        crate::fixups::support::walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            rewrite_stmt(&mut indent.stmt, function, facts, path, liftable);
        });
    }
}

fn rewrite_stmt(
    stmt: &mut Stmt,
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
    liftable: &BTreeSet<String>,
) {
    if let Stmt::Expr(_expr) = stmt
        && let Some(replacement) = copy_replacement(function, facts, &AstPath(path.to_vec()))
    {
        *stmt = replacement;
        return;
    }
    match stmt {
        Stmt::Let { init, .. } => {
            if let Some(init) = init {
                rewrite_expr_pointer_views(init, liftable);
            }
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => rewrite_expr_pointer_views(expr, liftable),
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            rewrite_expr_pointer_views(target, liftable);
            rewrite_expr_pointer_views(value, liftable);
        }
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            rewrite_expr_pointer_views(cond, liftable);
            crate::fixups::support::walk::with_path_segment(path, PathSegment::Then, |path| {
                rewrite_body(then_body, function, facts, path, liftable);
            });
            crate::fixups::support::walk::with_path_segment(path, PathSegment::Else, |path| {
                rewrite_body(else_body, function, facts, path, liftable);
            });
        }
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            rewrite_expr_pointer_views(cond, liftable);
            crate::fixups::support::walk::with_path_segment(path, PathSegment::Then, |path| {
                rewrite_body(then_body, function, facts, path, liftable);
            });
            rewrite_expr_pointer_views(then_value, liftable);
            crate::fixups::support::walk::with_path_segment(path, PathSegment::Else, |path| {
                rewrite_body(else_body, function, facts, path, liftable);
            });
            rewrite_expr_pointer_views(else_value, liftable);
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            rewrite_body(body, function, facts, path, liftable);
        }
        Stmt::For { iter, body, .. } => {
            rewrite_expr_pointer_views(iter, liftable);
            rewrite_body(body, function, facts, path, liftable);
        }
        Stmt::While { cond, body } => {
            rewrite_expr_pointer_views(cond, liftable);
            rewrite_block_pointer_views(body, function, facts, path, liftable);
        }
        Stmt::Block(body) | Stmt::Unsafe { body } => {
            rewrite_block_pointer_views(body, function, facts, path, liftable)
        }
        Stmt::Match { expr, arms } => {
            rewrite_expr_pointer_views(expr, liftable);
            for (arm_index, arm) in arms.iter_mut().enumerate() {
                crate::fixups::support::walk::with_path_segment(
                    path,
                    PathSegment::MatchArm(arm_index),
                    |path| rewrite_body(&mut arm.body, function, facts, path, liftable),
                );
            }
        }
        Stmt::InlineAsm(_) | Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
    }
}

fn rewrite_block_pointer_views(
    block: &mut Block,
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
    liftable: &BTreeSet<String>,
) {
    rewrite_body(&mut block.stmts, function, facts, path, liftable);
    if let Some(tail) = &mut block.tail {
        rewrite_expr_pointer_views(tail, liftable);
    }
}

fn rewrite_expr_pointer_views(expr: &mut Expr, liftable: &BTreeSet<String>) {
    if let Some(source) = pointer_view_source(expr)
        && liftable.contains(source)
    {
        *expr = Expr::Var(source.into());
        return;
    }
    match expr {
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => rewrite_expr_pointer_views(expr, liftable),
        Expr::Binary { lhs, rhs, .. } => {
            rewrite_expr_pointer_views(lhs, liftable);
            rewrite_expr_pointer_views(rhs, liftable);
        }
        Expr::Range { start, end } => {
            rewrite_expr_pointer_views(start, liftable);
            rewrite_expr_pointer_views(end, liftable);
        }
        Expr::Call { func, args, .. } => {
            rewrite_expr_pointer_views(func, liftable);
            for arg in args {
                rewrite_expr_pointer_views(arg, liftable);
            }
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            rewrite_expr_pointer_views(recv, liftable);
            for arg in args {
                rewrite_expr_pointer_views(arg, liftable);
            }
        }
        Expr::Field { base, .. }
        | Expr::TupleField { base, .. }
        | Expr::ArrayPtr { array: base, .. } => rewrite_expr_pointer_views(base, liftable),
        Expr::Index { base, index } => {
            rewrite_expr_pointer_views(base, liftable);
            rewrite_expr_pointer_views(index, liftable);
        }
        Expr::StructLit { fields, .. } => {
            for (_, value) in fields {
                rewrite_expr_pointer_views(value, liftable);
            }
        }
        Expr::TupleStructLit { fields, .. } => {
            for value in fields {
                rewrite_expr_pointer_views(value, liftable);
            }
        }
        Expr::ArrayLit(elems) => {
            for elem in elems {
                rewrite_expr_pointer_views(elem, liftable);
            }
        }
        Expr::ArrayRepeat { elem, .. } => rewrite_expr_pointer_views(elem, liftable),
        Expr::VecLit(elems) => {
            for elem in elems {
                rewrite_expr_pointer_views(elem, liftable);
            }
        }
        Expr::VecRepeat { elem, len } => {
            rewrite_expr_pointer_views(elem, liftable);
            rewrite_expr_pointer_views(len, liftable);
        }
        Expr::Macro { args, .. } => {
            for arg in args {
                rewrite_expr_pointer_views(arg, liftable);
            }
        }
        Expr::Closure { body, .. } => rewrite_expr_pointer_views(body, liftable),
        Expr::Match { expr, arms } => {
            rewrite_expr_pointer_views(expr, liftable);
            for arm in arms {
                rewrite_expr_pointer_views(&mut arm.value, liftable);
            }
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            rewrite_expr_pointer_views(cond, liftable);
            rewrite_expr_pointer_views(then_expr, liftable);
            rewrite_expr_pointer_views(else_expr, liftable);
        }
        Expr::Block(block) | Expr::Unsafe(block) => {
            rewrite_block_expr_pointer_views(block, liftable)
        }
        Expr::CopyNonoverlapping { src, dst, .. } => {
            rewrite_expr_pointer_views(src, liftable);
            rewrite_expr_pointer_views(dst, liftable);
        }
        Expr::PtrCopy {
            src, dst, count, ..
        } => {
            rewrite_expr_pointer_views(src, liftable);
            rewrite_expr_pointer_views(dst, liftable);
            rewrite_expr_pointer_views(count, liftable);
        }
        Expr::WriteBytes { dst, val, count } => {
            rewrite_expr_pointer_views(dst, liftable);
            rewrite_expr_pointer_views(val, liftable);
            rewrite_expr_pointer_views(count, liftable);
        }
        Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. } => {
            if let Some(ptr) = place.ptr_expr_mut() {
                rewrite_expr_pointer_views(ptr, liftable);
            }
        }
        Expr::AtomicStore { place, value, .. }
        | Expr::AtomicFetch { place, value, .. }
        | Expr::AtomicSwap { place, value, .. } => {
            if let Some(ptr) = place.ptr_expr_mut() {
                rewrite_expr_pointer_views(ptr, liftable);
            }
            rewrite_expr_pointer_views(value, liftable);
        }
        Expr::AtomicNew { value, .. } => rewrite_expr_pointer_views(value, liftable),
        Expr::AtomicCompareExchange {
            place,
            expected,
            desired,
            ..
        } => {
            if let Some(ptr) = place.ptr_expr_mut() {
                rewrite_expr_pointer_views(ptr, liftable);
            }
            rewrite_expr_pointer_views(expected, liftable);
            rewrite_expr_pointer_views(desired, liftable);
        }
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::CStr(_)
        | Expr::Var(_)
        | Expr::Path(_)
        | Expr::AtomicFence { .. }
        | Expr::Todo(_) => {}
    }
}

fn copy_replacement(function: FunctionId, facts: &FixupFacts, path: &AstPath) -> Option<Stmt> {
    let fact = facts.string_copy_rewrite(function, path)?;
    let dst = facts.binding_name(fact.dst)?.to_owned();
    Some(match &fact.rewrite {
        StringCopyRewrite::AssignLiteral(value) => Stmt::Assign {
            target: Expr::Var(dst.into()),
            value: to_owned(Expr::Str(value.clone())),
        },
        StringCopyRewrite::AssignOwned(source) => Stmt::Assign {
            target: Expr::Var(dst.into()),
            value: to_owned(Expr::MethodCall {
                recv: Box::new(Expr::Var(facts.binding_name(*source)?.to_owned().into())),
                method: "as_str".into(),
                args: Vec::new(),
            }),
        },
        StringCopyRewrite::PushLiteral(value) => Stmt::Expr(Expr::MethodCall {
            recv: Box::new(Expr::Var(dst.into())),
            method: "push_str".into(),
            args: vec![Expr::Str(value.clone())],
        }),
        StringCopyRewrite::PushOwned(source) => Stmt::Expr(Expr::MethodCall {
            recv: Box::new(Expr::Var(dst.into())),
            method: "push_str".into(),
            args: vec![Expr::MethodCall {
                recv: Box::new(Expr::Var(facts.binding_name(*source)?.to_owned().into())),
                method: "as_str".into(),
                args: Vec::new(),
            }],
        }),
    })
}

fn rewrite_block_expr_pointer_views(block: &mut Block, liftable: &BTreeSet<String>) {
    crate::fixups::support::walk::block_exprs_mut_with(block, &mut |expr| {
        if let Some(source) = pointer_view_source(expr)
            && liftable.contains(source)
        {
            *expr = Expr::Var(source.into());
            return false;
        }
        true
    });
}

fn to_owned(expr: Expr) -> Expr {
    Expr::MethodCall {
        recv: Box::new(expr),
        method: "to_owned".into(),
        args: Vec::new(),
    }
}

fn pointer_view_source(expr: &Expr) -> Option<&str> {
    match expr {
        Expr::MethodCall { recv, method, args }
            if args.is_empty() && matches!(method.as_str(), "as_ptr" | "as_mut_ptr") =>
        {
            match &**recv {
                Expr::Var(v) => Some(v.as_str()),
                _ => None,
            }
        }
        Expr::ArrayPtr { array, .. } => match &**array {
            Expr::Var(v) => Some(v.as_str()),
            _ => None,
        },
        Expr::Cast { expr, .. }
        | Expr::Unary { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => pointer_view_source(expr),
        _ => None,
    }
}

fn peel_empty_unsafe(expr: &Expr) -> &Expr {
    if let Expr::Unsafe(block) = expr
        && block.stmts.is_empty()
        && let Some(tail) = &block.tail
    {
        return tail;
    }
    expr
}

fn expr_any(expr: &Expr, pred: &mut impl FnMut(&Expr) -> bool) -> bool {
    pred(expr) || expr_children_any(expr, &mut |expr| expr_any(expr, pred))
}

fn expr_children_any(expr: &Expr, pred: &mut impl FnMut(&Expr) -> bool) -> bool {
    match expr {
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. }
        | Expr::Closure { body: expr, .. }
        | Expr::AtomicNew { value: expr, .. } => pred(expr),
        Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. } => {
            place.ptr_expr().is_some_and(&mut *pred)
        }
        Expr::Binary { lhs, rhs, .. }
        | Expr::Range {
            start: lhs,
            end: rhs,
        }
        | Expr::Index {
            base: lhs,
            index: rhs,
        } => pred(lhs) || pred(rhs),
        Expr::Call { func, args, .. } => pred(func) || args.iter().any(pred),
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            pred(recv) || args.iter().any(pred)
        }
        Expr::Field { base, .. }
        | Expr::TupleField { base, .. }
        | Expr::ArrayPtr { array: base, .. } => pred(base),
        Expr::StructLit { fields, .. } => fields.iter().any(|(_, value)| pred(value)),
        Expr::TupleStructLit { fields, .. } => fields.iter().any(pred),
        Expr::ArrayLit(elems) => elems.iter().any(pred),
        Expr::ArrayRepeat { elem, .. } => pred(elem),
        Expr::VecLit(elems) => elems.iter().any(pred),
        Expr::VecRepeat { elem, len } => pred(elem) || pred(len),
        Expr::Macro { args, .. } => args.iter().any(pred),
        Expr::Match { expr, arms } => pred(expr) || arms.iter().any(|arm| pred(&arm.value)),
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => pred(cond) || pred(then_expr) || pred(else_expr),
        Expr::Block(block) | Expr::Unsafe(block) => {
            block
                .stmts
                .iter()
                .any(|indent| stmt_expr_any(&indent.stmt, pred))
                || block.tail.as_deref().is_some_and(pred)
        }
        Expr::AtomicStore { place, value, .. }
        | Expr::AtomicFetch { place, value, .. }
        | Expr::AtomicSwap { place, value, .. } => {
            place.ptr_expr().is_some_and(&mut *pred) || pred(value)
        }
        Expr::AtomicCompareExchange {
            place,
            expected,
            desired,
            ..
        } => place.ptr_expr().is_some_and(&mut *pred) || pred(expected) || pred(desired),
        Expr::CopyNonoverlapping { src, dst, .. } => pred(src) || pred(dst),
        Expr::PtrCopy {
            src, dst, count, ..
        } => pred(src) || pred(dst) || pred(count),
        Expr::WriteBytes { dst, val, count } => pred(dst) || pred(val) || pred(count),
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::CStr(_)
        | Expr::Var(_)
        | Expr::Path(_)
        | Expr::Todo(_)
        | Expr::AtomicFence { .. } => false,
    }
}

fn stmt_expr_any(stmt: &Stmt, pred: &mut impl FnMut(&Expr) -> bool) -> bool {
    match stmt {
        Stmt::Let { init, .. } => init.as_ref().is_some_and(pred),
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            pred(target) || pred(value)
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => pred(expr),
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            pred(cond)
                || then_body
                    .iter()
                    .any(|indent| stmt_expr_any(&indent.stmt, pred))
                || else_body
                    .iter()
                    .any(|indent| stmt_expr_any(&indent.stmt, pred))
        }
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            pred(cond)
                || then_body
                    .iter()
                    .any(|indent| stmt_expr_any(&indent.stmt, pred))
                || pred(then_value)
                || else_body
                    .iter()
                    .any(|indent| stmt_expr_any(&indent.stmt, pred))
                || pred(else_value)
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            body.iter().any(|indent| stmt_expr_any(&indent.stmt, pred))
        }
        Stmt::For { iter, body, .. } => {
            pred(iter) || body.iter().any(|indent| stmt_expr_any(&indent.stmt, pred))
        }
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
            body.stmts
                .iter()
                .any(|indent| stmt_expr_any(&indent.stmt, pred))
                || body.tail.as_deref().is_some_and(pred)
        }
        Stmt::Match { expr, arms } => {
            pred(expr)
                || arms.iter().any(|arm| {
                    arm.body
                        .iter()
                        .any(|indent| stmt_expr_any(&indent.stmt, pred))
                })
        }
        Stmt::InlineAsm(_) | Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => false,
    }
}
