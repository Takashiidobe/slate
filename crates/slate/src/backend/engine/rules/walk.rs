use crate::backend::engine::arena::{Arena, NodeId, NodeKind};
use crate::backend::rust_ast::{AsmOperand, Block, Expr, Stmt};

pub(super) fn child_exprs(expr: &Expr) -> Vec<&Expr> {
    match expr {
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. }
        | Expr::Field { base: expr, .. }
        | Expr::TupleField { base: expr, .. }
        | Expr::ArrayPtr { array: expr, .. }
        | Expr::ArrayRepeat { elem: expr, .. }
        | Expr::AtomicNew { value: expr, .. }
        | Expr::Closure { body: expr, .. } => vec![expr],
        Expr::Binary { lhs, rhs, .. }
        | Expr::Range {
            start: lhs,
            end: rhs,
        }
        | Expr::Index {
            base: lhs,
            index: rhs,
        }
        | Expr::VecRepeat {
            elem: lhs,
            len: rhs,
        } => vec![lhs, rhs],
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => vec![cond, then_expr, else_expr],
        Expr::Call { func, args, .. } => {
            let mut out = vec![func.as_ref()];
            out.extend(args.iter());
            out
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            let mut out = vec![recv.as_ref()];
            out.extend(args.iter());
            out
        }
        Expr::StructLit { fields, .. } => fields.iter().map(|(_, value)| value).collect(),
        Expr::TupleStructLit { fields, .. } => fields.iter().collect(),
        Expr::ArrayLit(elems) | Expr::VecLit(elems) => elems.iter().collect(),
        Expr::Macro { args, .. } => args.iter().collect(),
        Expr::Match { expr, arms } => {
            let mut out = vec![expr.as_ref()];
            out.extend(arms.iter().map(|arm| &arm.value));
            out
        }
        Expr::CopyNonoverlapping { src, dst, .. } => vec![src, dst],
        Expr::PtrCopy {
            src, dst, count, ..
        } => vec![src, dst, count],
        Expr::WriteBytes { dst, val, count } => vec![dst, val, count],
        Expr::AtomicStore { value, .. }
        | Expr::AtomicFetch { value, .. }
        | Expr::AtomicSwap { value, .. } => vec![value],
        Expr::AtomicCompareExchange {
            expected, desired, ..
        } => vec![expected, desired],
        Expr::Block(block) | Expr::Unsafe(block) => {
            let mut out: Vec<&Expr> = Vec::new();
            for stmt in &block.stmts {
                stmt_exprs(stmt, &mut out);
            }
            if let Some(tail) = &block.tail {
                out.push(tail);
            }
            out
        }
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::ConstBlock(_)
        | Expr::CStr(_)
        | Expr::Var(_)
        | Expr::Path(_)
        | Expr::GenericPath { .. }
        | Expr::Todo(_)
        | Expr::AtomicFence { .. }
        | Expr::AtomicRef { .. }
        | Expr::AtomicLoad { .. } => Vec::new(),
    }
}

pub(super) fn child_exprs_mut(expr: &mut Expr) -> Vec<&mut Expr> {
    match expr {
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. }
        | Expr::Field { base: expr, .. }
        | Expr::TupleField { base: expr, .. }
        | Expr::ArrayPtr { array: expr, .. }
        | Expr::ArrayRepeat { elem: expr, .. }
        | Expr::AtomicNew { value: expr, .. }
        | Expr::Closure { body: expr, .. } => vec![expr],
        Expr::Binary { lhs, rhs, .. }
        | Expr::Range {
            start: lhs,
            end: rhs,
        }
        | Expr::Index {
            base: lhs,
            index: rhs,
        }
        | Expr::VecRepeat {
            elem: lhs,
            len: rhs,
        } => vec![lhs, rhs],
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => vec![cond, then_expr, else_expr],
        Expr::Call { func, args, .. } => {
            let mut out = vec![func.as_mut()];
            out.extend(args.iter_mut());
            out
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            let mut out = vec![recv.as_mut()];
            out.extend(args.iter_mut());
            out
        }
        Expr::StructLit { fields, .. } => fields.iter_mut().map(|(_, value)| value).collect(),
        Expr::TupleStructLit { fields, .. } => fields.iter_mut().collect(),
        Expr::ArrayLit(elems) | Expr::VecLit(elems) => elems.iter_mut().collect(),
        Expr::Macro { args, .. } => args.iter_mut().collect(),
        Expr::Match { expr, arms } => {
            let mut out = vec![expr.as_mut()];
            out.extend(arms.iter_mut().map(|arm| &mut arm.value));
            out
        }
        Expr::CopyNonoverlapping { src, dst, .. } => vec![src, dst],
        Expr::PtrCopy {
            src, dst, count, ..
        } => vec![src, dst, count],
        Expr::WriteBytes { dst, val, count } => vec![dst, val, count],
        Expr::AtomicStore { value, .. }
        | Expr::AtomicFetch { value, .. }
        | Expr::AtomicSwap { value, .. } => vec![value],
        Expr::AtomicCompareExchange {
            expected, desired, ..
        } => vec![expected, desired],
        Expr::Block(block) | Expr::Unsafe(block) => {
            let mut out: Vec<&mut Expr> = Vec::new();
            for stmt in &mut block.stmts {
                stmt_exprs_mut(stmt, &mut out);
            }
            if let Some(tail) = &mut block.tail {
                out.push(tail);
            }
            out
        }
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::ConstBlock(_)
        | Expr::CStr(_)
        | Expr::Var(_)
        | Expr::Path(_)
        | Expr::GenericPath { .. }
        | Expr::Todo(_)
        | Expr::AtomicFence { .. }
        | Expr::AtomicRef { .. }
        | Expr::AtomicLoad { .. } => Vec::new(),
    }
}

fn expr_has_stmts(expr: &Expr) -> bool {
    matches!(expr, Expr::Block(block) | Expr::Unsafe(block) if !block.stmts.is_empty())
        || child_exprs(expr).into_iter().any(expr_has_stmts)
}

pub(super) fn kind_has_stmt_exprs(kind: &NodeKind) -> bool {
    let mut found = false;
    visit_kind_exprs(kind, |expr| found |= expr_has_stmts(expr));
    found
}

pub(super) fn stmt_exprs<'a>(stmt: &'a Stmt, out: &mut Vec<&'a Expr>) {
    match stmt {
        Stmt::Cfg { .. } => unreachable!("only the project merge gates statements"),
        Stmt::Let {
            init: Some(expr), ..
        }
        | Stmt::Expr(expr)
        | Stmt::Return(Some(expr)) => out.push(expr),
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            out.push(target);
            out.push(value);
        }
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            out.push(cond);
            stmts_exprs(then_body, out);
            out.push(then_value);
            stmts_exprs(else_body, out);
            out.push(else_value);
        }
        Stmt::InlineAsm(asm) => {
            for operand in &asm.operands {
                match operand {
                    AsmOperand::In { value, .. }
                    | AsmOperand::Out { value, .. }
                    | AsmOperand::Const(value)
                    | AsmOperand::Sym(value) => out.push(value),
                    AsmOperand::InOut { input, output, .. } => {
                        out.push(input);
                        out.push(output);
                    }
                    AsmOperand::Label { state, value } => {
                        out.push(state);
                        out.push(value);
                    }
                }
            }
        }
        Stmt::Unsafe { body } | Stmt::Block(body) => block_exprs(body, out),
        Stmt::While { cond, body, .. } => {
            out.push(cond);
            block_exprs(body, out);
        }
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            out.push(cond);
            stmts_exprs(then_body, out);
            stmts_exprs(else_body, out);
        }
        Stmt::For { iter, body, .. } => {
            out.push(iter);
            stmts_exprs(body, out);
        }
        Stmt::Match { expr, arms } => {
            out.push(expr);
            for arm in arms {
                stmts_exprs(&arm.body, out);
            }
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            stmts_exprs(body, out)
        }
        Stmt::Let { init: None, .. }
        | Stmt::Return(None)
        | Stmt::Comment(_)
        | Stmt::Break(_)
        | Stmt::Continue(_) => {}
    }
}

fn stmts_exprs<'a>(stmts: &'a [Stmt], out: &mut Vec<&'a Expr>) {
    for stmt in stmts {
        stmt_exprs(stmt, out);
    }
}

fn block_exprs<'a>(block: &'a Block, out: &mut Vec<&'a Expr>) {
    stmts_exprs(&block.stmts, out);
    out.extend(block.tail.as_deref());
}

pub(super) fn stmt_exprs_mut<'a>(stmt: &'a mut Stmt, out: &mut Vec<&'a mut Expr>) {
    match stmt {
        Stmt::Cfg { .. } => unreachable!("only the project merge gates statements"),
        Stmt::Let {
            init: Some(expr), ..
        }
        | Stmt::Expr(expr)
        | Stmt::Return(Some(expr)) => out.push(expr),
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            out.push(target);
            out.push(value);
        }
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            out.push(cond);
            stmts_exprs_mut(then_body, out);
            out.push(then_value);
            stmts_exprs_mut(else_body, out);
            out.push(else_value);
        }
        Stmt::InlineAsm(asm) => {
            for operand in &mut asm.operands {
                match operand {
                    AsmOperand::In { value, .. }
                    | AsmOperand::Out { value, .. }
                    | AsmOperand::Const(value)
                    | AsmOperand::Sym(value) => out.push(value),
                    AsmOperand::InOut { input, output, .. } => {
                        out.push(input);
                        out.push(output);
                    }
                    AsmOperand::Label { state, value } => {
                        out.push(state);
                        out.push(value);
                    }
                }
            }
        }
        Stmt::Unsafe { body } | Stmt::Block(body) => block_exprs_mut(body, out),
        Stmt::While { cond, body, .. } => {
            out.push(cond);
            block_exprs_mut(body, out);
        }
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            out.push(cond);
            stmts_exprs_mut(then_body, out);
            stmts_exprs_mut(else_body, out);
        }
        Stmt::For { iter, body, .. } => {
            out.push(iter);
            stmts_exprs_mut(body, out);
        }
        Stmt::Match { expr, arms } => {
            out.push(expr);
            for arm in arms {
                stmts_exprs_mut(&mut arm.body, out);
            }
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            stmts_exprs_mut(body, out)
        }
        Stmt::Let { init: None, .. }
        | Stmt::Return(None)
        | Stmt::Comment(_)
        | Stmt::Break(_)
        | Stmt::Continue(_) => {}
    }
}

fn stmts_exprs_mut<'a>(stmts: &'a mut [Stmt], out: &mut Vec<&'a mut Expr>) {
    for stmt in stmts {
        stmt_exprs_mut(stmt, out);
    }
}

fn block_exprs_mut<'a>(block: &'a mut Block, out: &mut Vec<&'a mut Expr>) {
    stmts_exprs_mut(&mut block.stmts, out);
    out.extend(block.tail.as_deref_mut());
}

pub(super) fn visit_kind_exprs(kind: &NodeKind, mut f: impl FnMut(&Expr)) {
    match kind {
        NodeKind::Comment(_) => {}
        NodeKind::Let { init, .. } => init.as_ref().into_iter().for_each(&mut f),
        NodeKind::LetIf {
            cond,
            then_value,
            else_value,
            ..
        } => [cond, then_value, else_value].into_iter().for_each(&mut f),
        NodeKind::Assign { target, value } | NodeKind::CompoundAssign { target, value, .. } => {
            [target, value].into_iter().for_each(&mut f)
        }
        NodeKind::InlineAsm(asm) => {
            for operand in &asm.operands {
                operand.visit_exprs(&mut f);
            }
        }
        NodeKind::Expr(expr) => f(expr),
        NodeKind::Return(expr) => expr.as_ref().into_iter().for_each(&mut f),
        NodeKind::Unsafe { tail, .. } | NodeKind::Block { tail, .. } => {
            tail.as_deref().into_iter().for_each(&mut f)
        }
        NodeKind::While { cond, tail, .. } => {
            f(cond);
            tail.as_deref().into_iter().for_each(&mut f);
        }
        NodeKind::If { cond, .. } => f(cond),
        NodeKind::For { iter, .. } => f(iter),
        NodeKind::Match { expr, .. } => f(expr),
        NodeKind::Loop { .. }
        | NodeKind::Scope { .. }
        | NodeKind::LabeledBlock { .. }
        | NodeKind::Break(_)
        | NodeKind::Continue(_) => {}
    }
}

pub(super) fn visit_kind_exprs_mut(kind: &mut NodeKind, mut f: impl FnMut(&mut Expr)) {
    match kind {
        NodeKind::Comment(_) => {}
        NodeKind::Let { init, .. } => init.as_mut().into_iter().for_each(&mut f),
        NodeKind::LetIf {
            cond,
            then_value,
            else_value,
            ..
        } => [cond, then_value, else_value].into_iter().for_each(&mut f),
        NodeKind::Assign { target, value } | NodeKind::CompoundAssign { target, value, .. } => {
            [target, value].into_iter().for_each(&mut f)
        }
        NodeKind::InlineAsm(asm) => {
            for operand in &mut asm.operands {
                operand.visit_exprs_mut(&mut f);
            }
        }
        NodeKind::Expr(expr) => f(expr),
        NodeKind::Return(expr) => expr.as_mut().into_iter().for_each(&mut f),
        NodeKind::Unsafe { tail, .. } | NodeKind::Block { tail, .. } => {
            tail.as_deref_mut().into_iter().for_each(&mut f)
        }
        NodeKind::While { cond, tail, .. } => {
            f(cond);
            tail.as_deref_mut().into_iter().for_each(&mut f);
        }
        NodeKind::If { cond, .. } => f(cond),
        NodeKind::For { iter, .. } => f(iter),
        NodeKind::Match { expr, .. } => f(expr),
        NodeKind::Loop { .. }
        | NodeKind::Scope { .. }
        | NodeKind::LabeledBlock { .. }
        | NodeKind::Break(_)
        | NodeKind::Continue(_) => {}
    }
}

pub(super) fn fold_bottom_up(
    expr: &mut Expr,
    rewrite_here: &mut impl FnMut(&mut Expr) -> bool,
) -> bool {
    let mut changed = false;
    for child in child_exprs_mut(expr) {
        changed |= fold_bottom_up(child, rewrite_here);
    }
    changed | rewrite_here(expr)
}

pub(super) fn any_collapsible(expr: &Expr, collapsible_here: &impl Fn(&Expr) -> bool) -> bool {
    collapsible_here(expr)
        || child_exprs(expr)
            .iter()
            .any(|child| any_collapsible(child, collapsible_here))
}

pub(super) fn function_root(arena: &Arena, mut id: NodeId) -> NodeId {
    while let Some(parent) = arena.parent(id) {
        id = parent;
    }
    id
}

pub(super) fn substitute_var_in_subtree(
    arena: &mut Arena,
    id: NodeId,
    name: &str,
    replacement: &Expr,
    kind_own_substitute_var: &impl Fn(&mut NodeKind, &str, &Expr) -> bool,
) -> bool {
    let mut changed = arena
        .get_mut(id)
        .is_some_and(|kind| kind_own_substitute_var(kind, name, replacement));
    let children: Vec<NodeId> = arena
        .get(id)
        .map(|kind| kind.child_lists().into_iter().flatten().copied().collect())
        .unwrap_or_default();
    for child in children {
        changed |=
            substitute_var_in_subtree(arena, child, name, replacement, kind_own_substitute_var);
    }
    changed
}
