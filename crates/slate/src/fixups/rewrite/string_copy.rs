use crate::fixups::facts::{
    AstPath, CallCallee, FixupFacts, FunctionId, PathSegment, StringBufferProvenance,
    StringCopyRewrite, StringRecoveryCandidate,
};
use crate::rust_ast::{Block, Expr, ExternDecl, IndentStmt, Item, Program, Stmt, Type};
use std::collections::BTreeSet;

pub(in crate::fixups) fn fixup(program: &mut Program, facts: &FixupFacts) {
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
                    plan.function == function
                        && plan.binding == buffer.binding
                        && plan.path == buffer.path
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
        plan.function == function
            && plan.binding == buffer.binding
            && plan.path.0 == path
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
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
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
        Expr::Call { func, args } => {
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
        Expr::AtomicRef { ptr, .. } | Expr::AtomicLoad { ptr, .. } => {
            rewrite_expr_pointer_views(ptr, liftable);
        }
        Expr::AtomicStore { ptr, value, .. }
        | Expr::AtomicFetch { ptr, value, .. }
        | Expr::AtomicSwap { ptr, value, .. } => {
            rewrite_expr_pointer_views(ptr, liftable);
            rewrite_expr_pointer_views(value, liftable);
        }
        Expr::AtomicCompareExchange {
            ptr,
            expected,
            desired,
            ..
        } => {
            rewrite_expr_pointer_views(ptr, liftable);
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

fn is_copy_func(name: &str) -> bool {
    matches!(name, "strcpy" | "strncpy" | "strcat" | "strncat")
}

pub(in crate::fixups) fn prune_unused_externs(program: &mut Program, facts: &FixupFacts) {
    let used = direct_calls(facts);
    program.items.retain_mut(|item| match item {
        Item::ExternBlock { decls, .. } => {
            decls.retain(|decl| match decl {
                ExternDecl::Fn(f) if is_copy_func(&f.name) => used.contains(&f.name),
                _ => true,
            });
            !decls.is_empty()
        }
        _ => true,
    });
}

fn direct_calls(facts: &FixupFacts) -> Vec<String> {
    let mut calls = facts
        .callsites
        .iter()
        .filter_map(|callsite| match &callsite.callee {
            CallCallee::Direct { name, .. } => Some(name.clone()),
            CallCallee::Indirect => None,
        })
        .collect::<Vec<_>>();
    calls.sort();
    calls.dedup();
    calls
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
        | Expr::AtomicRef { ptr: expr, .. }
        | Expr::AtomicLoad { ptr: expr, .. } => pred(expr),
        Expr::Binary { lhs, rhs, .. }
        | Expr::Index {
            base: lhs,
            index: rhs,
        } => pred(lhs) || pred(rhs),
        Expr::Call { func, args } => pred(func) || args.iter().any(pred),
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            pred(recv) || args.iter().any(pred)
        }
        Expr::Field { base, .. }
        | Expr::TupleField { base, .. }
        | Expr::ArrayPtr { array: base, .. } => pred(base),
        Expr::StructLit { fields, .. } => fields.iter().any(|(_, value)| pred(value)),
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
        Expr::AtomicStore { ptr, value, .. }
        | Expr::AtomicFetch { ptr, value, .. }
        | Expr::AtomicSwap { ptr, value, .. } => pred(ptr) || pred(value),
        Expr::AtomicCompareExchange {
            ptr,
            expected,
            desired,
            ..
        } => pred(ptr) || pred(expected) || pred(desired),
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
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => false,
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::Prim;

    fn run(stmts: Vec<Stmt>) -> String {
        let mut program = Program {
            items: vec![Item::Fn(func(Vec::new(), None, stmts))],
        };
        let analyzed = crate::fixups::facts::analyze(program.clone());
        fixup(&mut program, &analyzed.facts);
        program.emit()
    }

    #[test]
    fn rewrites_strcpy_to_owned_string_assignment() {
        let out = run(vec![
            Stmt::Let {
                name: "dst".into(),
                mutable: true,
                ty: Some(Type::Array {
                    elem: Box::new(Type::Prim(Prim::I8)),
                    len: 8,
                }),
                init: Some(Expr::ArrayRepeat {
                    elem: Box::new(int(0)),
                    len: 8,
                }),
            },
            Stmt::Expr(Expr::Unsafe(Box::new(Block {
                stmts: vec![],
                tail: Some(Box::new(call(
                    "strcpy",
                    vec![as_mut_ptr("dst"), c_string("abc")],
                ))),
            }))),
        ]);
        assert!(out.contains("let mut dst: String = \"\".to_owned();"));
        assert!(out.contains("dst = \"abc\".to_owned();"));
        assert!(!out.contains("strcpy("));
    }

    #[test]
    fn rewrites_strcat_to_push_str() {
        let out = run(vec![
            Stmt::Let {
                name: "dst".into(),
                mutable: true,
                ty: Some(Type::Array {
                    elem: Box::new(Type::Prim(Prim::I8)),
                    len: 8,
                }),
                init: Some(Expr::ArrayLit(vec![int(104), int(105), int(0)])),
            },
            Stmt::Expr(Expr::Unsafe(Box::new(Block {
                stmts: vec![],
                tail: Some(Box::new(call(
                    "strcat",
                    vec![as_mut_ptr("dst"), c_string("!")],
                ))),
            }))),
        ]);
        assert!(out.contains("let mut dst: String = \"hi\".to_owned();"));
        assert!(out.contains("dst.push_str(\"!\");"));
        assert!(!out.contains("strcat("));
    }

    fn as_mut_ptr(name: &str) -> Expr {
        Expr::MethodCall {
            recv: Box::new(Expr::Var(name.into())),
            method: "as_mut_ptr".into(),
            args: Vec::new(),
        }
    }

    fn c_string(text: &str) -> Expr {
        let mut bytes = text.as_bytes().to_vec();
        bytes.push(0);
        Expr::MethodCall {
            recv: Box::new(Expr::ByteStr(bytes)),
            method: "as_ptr".into(),
            args: Vec::new(),
        }
    }
}
