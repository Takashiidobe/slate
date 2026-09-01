use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{Arena, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::{Expr, Prim, RustValue, Stmt, Type};

const KINDS: &[NodeKindTag] = &[
    NodeKindTag::Let,
    NodeKindTag::LetIf,
    NodeKindTag::Assign,
    NodeKindTag::CompoundAssign,
    NodeKindTag::InlineAsm,
    NodeKindTag::Expr,
    NodeKindTag::Return,
    NodeKindTag::Unsafe,
    NodeKindTag::If,
    NodeKindTag::For,
    NodeKindTag::Match,
    NodeKindTag::While,
    NodeKindTag::Block,
];

fn is_sized_pointee(ty: &Type) -> bool {
    !matches!(
        ty,
        Type::Slice(_) | Type::Str | Type::VaList | Type::Variadic
    )
}

fn is_thin_raw_ptr(ty: &Type) -> bool {
    matches!(ty, Type::Ptr { inner, .. } if is_sized_pointee(inner))
}

fn is_int_prim(prim: Prim) -> bool {
    !matches!(
        prim,
        Prim::F16 | Prim::F32 | Prim::F64 | Prim::F128 | Prim::Bool
    )
}

fn yields_ptr_or_int(expr: &Expr) -> bool {
    match expr {
        Expr::Cast { ty, .. } => {
            is_thin_raw_ptr(ty) || matches!(ty, Type::Prim(p) if is_int_prim(*p))
        }
        Expr::MethodCall { method, .. } => matches!(method.as_str(), "as_ptr" | "as_mut_ptr"),
        Expr::ArrayPtr { .. } => true,
        Expr::Value(
            RustValue::NullPtr
            | RustValue::I64(_)
            | RustValue::I128(_)
            | RustValue::U128(_)
            | RustValue::Usize(_),
        ) => true,
        _ => false,
    }
}

fn collapsible_pair(expr: &Expr) -> bool {
    let Expr::Cast {
        expr: inner,
        ty: outer_ty,
    } = expr
    else {
        return false;
    };
    let Expr::Cast {
        ty: inner_ty,
        expr: innermost,
    } = inner.as_ref()
    else {
        return false;
    };
    if inner_ty == outer_ty {
        return true;
    }
    is_thin_raw_ptr(inner_ty) && is_thin_raw_ptr(outer_ty) && yields_ptr_or_int(innermost)
}

fn collapse_here(expr: &mut Expr) -> bool {
    let mut changed = false;
    while collapsible_pair(expr) {
        let Expr::Cast { expr: inner, .. } = expr else {
            break;
        };
        let Expr::Cast {
            expr: innermost, ..
        } = inner.as_mut()
        else {
            break;
        };
        let innermost = std::mem::replace(innermost.as_mut(), Expr::Todo(String::new()));
        **inner = innermost;
        changed = true;
    }
    changed
}

fn fold_expr(expr: &mut Expr) -> bool {
    let mut changed = false;
    for child in child_exprs_mut(expr) {
        changed |= fold_expr(child);
    }
    changed | collapse_here(expr)
}

fn has_collapsible(expr: &Expr) -> bool {
    collapsible_pair(expr) || child_exprs(expr).iter().any(|child| has_collapsible(child))
}

fn child_exprs(expr: &Expr) -> Vec<&Expr> {
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
                stmt_exprs(&stmt.stmt, &mut out);
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
        | Expr::CStr(_)
        | Expr::Var(_)
        | Expr::Path(_)
        | Expr::Todo(_)
        | Expr::AtomicFence { .. }
        | Expr::AtomicRef { .. }
        | Expr::AtomicLoad { .. } => Vec::new(),
    }
}

fn child_exprs_mut(expr: &mut Expr) -> Vec<&mut Expr> {
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
                stmt_exprs_mut(&mut stmt.stmt, &mut out);
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
        | Expr::CStr(_)
        | Expr::Var(_)
        | Expr::Path(_)
        | Expr::Todo(_)
        | Expr::AtomicFence { .. }
        | Expr::AtomicRef { .. }
        | Expr::AtomicLoad { .. } => Vec::new(),
    }
}

fn stmt_exprs<'a>(stmt: &'a Stmt, out: &mut Vec<&'a Expr>) {
    match stmt {
        Stmt::Let {
            init: Some(expr), ..
        }
        | Stmt::Expr(expr)
        | Stmt::Return(Some(expr)) => out.push(expr),
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            out.push(target);
            out.push(value);
        }
        _ => {}
    }
}

fn stmt_exprs_mut<'a>(stmt: &'a mut Stmt, out: &mut Vec<&'a mut Expr>) {
    match stmt {
        Stmt::Let {
            init: Some(expr), ..
        }
        | Stmt::Expr(expr)
        | Stmt::Return(Some(expr)) => out.push(expr),
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            out.push(target);
            out.push(value);
        }
        _ => {}
    }
}

fn fold_kind(kind: &mut NodeKind) -> bool {
    let mut changed = false;
    let mut fold = |expr: &mut Expr| changed |= fold_expr(expr);
    match kind {
        NodeKind::Let { init, .. } => init.as_mut().into_iter().for_each(&mut fold),
        NodeKind::LetIf {
            cond,
            then_value,
            else_value,
            ..
        } => [cond, then_value, else_value]
            .into_iter()
            .for_each(&mut fold),
        NodeKind::Assign { target, value } | NodeKind::CompoundAssign { target, value, .. } => {
            [target, value].into_iter().for_each(&mut fold)
        }
        NodeKind::InlineAsm(asm) => {
            for operand in &mut asm.operands {
                operand.visit_exprs_mut(&mut fold);
            }
        }
        NodeKind::Expr(expr) => fold(expr),
        NodeKind::Return(expr) => expr.as_mut().into_iter().for_each(&mut fold),
        NodeKind::Unsafe { tail, .. } | NodeKind::Block { tail, .. } => {
            tail.as_deref_mut().into_iter().for_each(&mut fold)
        }
        NodeKind::While { cond, tail, .. } => {
            fold(cond);
            tail.as_deref_mut().into_iter().for_each(&mut fold);
        }
        NodeKind::If { cond, .. } => fold(cond),
        NodeKind::For { iter, .. } => fold(iter),
        NodeKind::Match { expr, .. } => fold(expr),
        NodeKind::Loop { .. }
        | NodeKind::Scope { .. }
        | NodeKind::LabeledBlock { .. }
        | NodeKind::Break(_)
        | NodeKind::Continue(_) => {}
    }
    changed
}

fn kind_has_collapsible(kind: &NodeKind) -> bool {
    let mut found = false;
    let mut scan = |expr: &Expr| found |= has_collapsible(expr);
    match kind {
        NodeKind::Let { init, .. } => init.as_ref().into_iter().for_each(&mut scan),
        NodeKind::LetIf {
            cond,
            then_value,
            else_value,
            ..
        } => [cond, then_value, else_value]
            .into_iter()
            .for_each(&mut scan),
        NodeKind::Assign { target, value } | NodeKind::CompoundAssign { target, value, .. } => {
            [target, value].into_iter().for_each(&mut scan)
        }
        NodeKind::InlineAsm(asm) => {
            for operand in &asm.operands {
                operand.visit_exprs(&mut scan);
            }
        }
        NodeKind::Expr(expr) => scan(expr),
        NodeKind::Return(expr) => expr.as_ref().into_iter().for_each(&mut scan),
        NodeKind::Unsafe { tail, .. } | NodeKind::Block { tail, .. } => {
            tail.as_deref().into_iter().for_each(&mut scan)
        }
        NodeKind::While { cond, tail, .. } => {
            scan(cond);
            tail.as_deref().into_iter().for_each(&mut scan);
        }
        NodeKind::If { cond, .. } => scan(cond),
        NodeKind::For { iter, .. } => scan(iter),
        NodeKind::Match { expr, .. } => scan(expr),
        NodeKind::Loop { .. }
        | NodeKind::Scope { .. }
        | NodeKind::LabeledBlock { .. }
        | NodeKind::Break(_)
        | NodeKind::Continue(_) => {}
    }
    found
}

pub(in crate::backend::engine) struct PeelCasts;

impl NodeRule for PeelCasts {
    fn name(&self) -> &'static str {
        "peel_casts"
    }

    fn priority(&self) -> u32 {
        45
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        KINDS
    }

    fn matches(&self, arena: &Arena, id: NodeId) -> bool {
        arena.get(id).is_some_and(kind_has_collapsible)
    }

    fn apply(&self, arena: &mut Arena, id: NodeId) -> bool {
        let Some(kind) = arena.get_mut(id) else {
            return false;
        };
        if !fold_kind(kind) {
            return false;
        }
        arena.touch(id);
        true
    }
}
