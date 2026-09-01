use super::walk::{child_exprs, child_exprs_mut};
use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{Arena, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::{CLibType, Expr, Type};

const KINDS: &[NodeKindTag] = &[
    NodeKindTag::Let,
    NodeKindTag::LetIf,
    NodeKindTag::Assign,
    NodeKindTag::CompoundAssign,
    NodeKindTag::Expr,
    NodeKindTag::Return,
    NodeKindTag::Unsafe,
    NodeKindTag::If,
    NodeKindTag::For,
    NodeKindTag::Match,
    NodeKindTag::While,
    NodeKindTag::Block,
];

fn is_const_c_char_ptr(ty: &Type) -> bool {
    matches!(ty, Type::Ptr { mutable: false, inner } if **inner == Type::CLib(CLibType::CHAR))
}

fn cstr_content(expr: &Expr) -> Option<Vec<u8>> {
    let Expr::Cast { expr: inner, ty } = expr else {
        return None;
    };
    if !is_const_c_char_ptr(ty) {
        return None;
    }
    let Expr::MethodCall { recv, method, args } = inner.as_ref() else {
        return None;
    };
    if method.as_str() != "as_ptr" || !args.is_empty() {
        return None;
    }
    let Expr::ByteStr(bytes) = recv.as_ref() else {
        return None;
    };
    let (last, rest) = bytes.split_last()?;
    if *last != 0 || rest.contains(&0) {
        return None;
    }
    Some(rest.to_vec())
}

fn rewrite_here(expr: &mut Expr) -> bool {
    let Some(content) = cstr_content(expr) else {
        return false;
    };
    *expr = Expr::MethodCall {
        recv: Box::new(Expr::CStr(content)),
        method: "as_ptr".into(),
        args: Vec::new(),
    };
    true
}

fn rewrite_expr(expr: &mut Expr) -> bool {
    let mut changed = false;
    for child in child_exprs_mut(expr) {
        changed |= rewrite_expr(child);
    }
    changed | rewrite_here(expr)
}

fn has_rewrite(expr: &Expr) -> bool {
    cstr_content(expr).is_some() || child_exprs(expr).iter().any(|child| has_rewrite(child))
}

fn rewrite_kind(kind: &mut NodeKind) -> bool {
    kind_exprs_mut(kind)
        .into_iter()
        .fold(false, |changed, expr| changed | rewrite_expr(expr))
}

fn kind_has_rewrite(kind: &NodeKind) -> bool {
    kind_exprs(kind).iter().any(|expr| has_rewrite(expr))
}

fn kind_exprs_mut(kind: &mut NodeKind) -> Vec<&mut Expr> {
    let mut out: Vec<&mut Expr> = Vec::new();
    match kind {
        NodeKind::Let { init, .. } => out.extend(init.as_mut()),
        NodeKind::LetIf {
            cond,
            then_value,
            else_value,
            ..
        } => out.extend([cond, then_value, else_value]),
        NodeKind::Assign { target, value } | NodeKind::CompoundAssign { target, value, .. } => {
            out.extend([target, value])
        }
        NodeKind::Expr(expr) => out.push(expr),
        NodeKind::Return(expr) => out.extend(expr.as_mut()),
        NodeKind::Unsafe { tail, .. } | NodeKind::Block { tail, .. } => {
            out.extend(tail.as_deref_mut())
        }
        NodeKind::While { cond, tail, .. } => {
            out.push(cond);
            out.extend(tail.as_deref_mut());
        }
        NodeKind::If { cond, .. } => out.push(cond),
        NodeKind::For { iter, .. } => out.push(iter),
        NodeKind::Match { expr, .. } => out.push(expr),
        NodeKind::InlineAsm(_)
        | NodeKind::Loop { .. }
        | NodeKind::Scope { .. }
        | NodeKind::LabeledBlock { .. }
        | NodeKind::Break(_)
        | NodeKind::Continue(_) => {}
    }
    out
}

fn kind_exprs(kind: &NodeKind) -> Vec<&Expr> {
    let mut out: Vec<&Expr> = Vec::new();
    match kind {
        NodeKind::Let { init, .. } => out.extend(init.as_ref()),
        NodeKind::LetIf {
            cond,
            then_value,
            else_value,
            ..
        } => out.extend([cond, then_value, else_value]),
        NodeKind::Assign { target, value } | NodeKind::CompoundAssign { target, value, .. } => {
            out.extend([target, value])
        }
        NodeKind::Expr(expr) => out.push(expr),
        NodeKind::Return(expr) => out.extend(expr.as_ref()),
        NodeKind::Unsafe { tail, .. } | NodeKind::Block { tail, .. } => out.extend(tail.as_deref()),
        NodeKind::While { cond, tail, .. } => {
            out.push(cond);
            out.extend(tail.as_deref());
        }
        NodeKind::If { cond, .. } => out.push(cond),
        NodeKind::For { iter, .. } => out.push(iter),
        NodeKind::Match { expr, .. } => out.push(expr),
        NodeKind::InlineAsm(_)
        | NodeKind::Loop { .. }
        | NodeKind::Scope { .. }
        | NodeKind::LabeledBlock { .. }
        | NodeKind::Break(_)
        | NodeKind::Continue(_) => {}
    }
    out
}

pub(in crate::backend::engine) struct CStrLiteral;

impl NodeRule for CStrLiteral {
    fn name(&self) -> &'static str {
        "cstr_literal"
    }

    fn priority(&self) -> u32 {
        46
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        KINDS
    }

    fn matches(&self, arena: &Arena, id: NodeId) -> bool {
        arena.get(id).is_some_and(kind_has_rewrite)
    }

    fn apply(&self, arena: &mut Arena, id: NodeId) -> bool {
        let Some(kind) = arena.get_mut(id) else {
            return false;
        };
        if !rewrite_kind(kind) {
            return false;
        }
        arena.touch(id);
        true
    }
}
