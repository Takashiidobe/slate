use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{Arena, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::{Expr, Ident, Type, UnaryOp};

fn source_var(expr: &Expr) -> Option<Ident> {
    match expr {
        Expr::Var(v) => Some(*v),
        Expr::Cast { expr, .. } => source_var(expr),
        _ => None,
    }
}

fn expr_only_deref(expr: &Expr, name: Ident) -> bool {
    if let Expr::Unary {
        op: UnaryOp::Deref,
        expr: inner,
    } = expr
        && matches!(&**inner, Expr::Var(v) if *v == name)
    {
        return true;
    }
    match expr {
        Expr::Var(v) => *v != name,
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => expr_only_deref(expr, name),
        Expr::Binary { lhs, rhs, .. } => expr_only_deref(lhs, name) && expr_only_deref(rhs, name),
        Expr::Call { func, args, .. } => {
            expr_only_deref(func, name) && args.iter().all(|a| expr_only_deref(a, name))
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            expr_only_deref(recv, name) && args.iter().all(|a| expr_only_deref(a, name))
        }
        Expr::Index { base, index } => expr_only_deref(base, name) && expr_only_deref(index, name),
        Expr::Field { base, .. } | Expr::TupleField { base, .. } => expr_only_deref(base, name),
        Expr::Block(block) | Expr::Unsafe(block) => {
            block.stmts.iter().all(|s| !s.stmt.reads_var(name.as_str()))
                && block
                    .tail
                    .as_deref()
                    .is_none_or(|tail| expr_only_deref(tail, name))
        }
        _ => !expr.reads_var(name.as_str()),
    }
}

fn kind_only_deref_uses(kind: &NodeKind, name: Ident) -> bool {
    match kind {
        NodeKind::Let { init, .. } => init.as_ref().is_none_or(|e| expr_only_deref(e, name)),
        NodeKind::LetIf {
            cond,
            then_value,
            else_value,
            ..
        } => {
            expr_only_deref(cond, name)
                && expr_only_deref(then_value, name)
                && expr_only_deref(else_value, name)
        }
        NodeKind::Assign { target, value } | NodeKind::CompoundAssign { target, value, .. } => {
            expr_only_deref(target, name) && expr_only_deref(value, name)
        }
        NodeKind::InlineAsm(_) => false,
        NodeKind::Expr(expr) => expr_only_deref(expr, name),
        NodeKind::Return(expr) => expr.as_ref().is_none_or(|e| expr_only_deref(e, name)),
        NodeKind::Unsafe { tail, .. } | NodeKind::Block { tail, .. } => {
            tail.as_deref().is_none_or(|e| expr_only_deref(e, name))
        }
        NodeKind::While { cond, tail, .. } => {
            expr_only_deref(cond, name) && tail.as_deref().is_none_or(|e| expr_only_deref(e, name))
        }
        NodeKind::If { cond, .. } => expr_only_deref(cond, name),
        NodeKind::For { iter, .. } => expr_only_deref(iter, name),
        NodeKind::Loop { .. } | NodeKind::Scope { .. } | NodeKind::LabeledBlock { .. } => true,
        NodeKind::Match { expr, .. } => expr_only_deref(expr, name),
        NodeKind::Break(_) | NodeKind::Continue(_) => true,
    }
}

fn pure_rename_target(kind: &NodeKind, name: Ident) -> Option<Ident> {
    match kind {
        NodeKind::Let {
            name: temp,
            init: Some(Expr::Var(v)),
            ..
        } if *v == name => Some(*temp),
        _ => None,
    }
}

fn all_uses_deref_transitively(arena: &Arena, name: Ident, visited: &mut Vec<Ident>) -> bool {
    if visited.contains(&name) {
        return false;
    }
    visited.push(name);
    arena.def_use_neighbors(name).iter().all(|&neighbor| {
        let Some(kind) = arena.get(neighbor) else {
            return true;
        };
        kind_only_deref_uses(kind, name)
            || pure_rename_target(kind, name)
                .is_some_and(|temp| all_uses_deref_transitively(arena, temp, visited))
    })
}

fn function_root(arena: &Arena, mut id: NodeId) -> NodeId {
    while let Some(parent) = arena.parent(id) {
        id = parent;
    }
    id
}

fn node_substitute_var(arena: &mut Arena, id: NodeId, name: &str, replacement: &Expr) -> bool {
    let mut changed = arena
        .get_mut(id)
        .is_some_and(|kind| kind_own_substitute_var(kind, name, replacement));
    let children: Vec<NodeId> = arena
        .get(id)
        .map(|kind| kind.child_lists().into_iter().flatten().copied().collect())
        .unwrap_or_default();
    for child in children {
        changed |= node_substitute_var(arena, child, name, replacement);
    }
    changed
}

fn kind_own_substitute_var(kind: &mut NodeKind, name: &str, replacement: &Expr) -> bool {
    match kind {
        NodeKind::Let { init, .. } => init
            .as_mut()
            .is_some_and(|e| e.substitute_var(name, replacement)),
        NodeKind::LetIf {
            cond,
            then_value,
            else_value,
            ..
        } => {
            let a = cond.substitute_var(name, replacement);
            let b = then_value.substitute_var(name, replacement);
            let c = else_value.substitute_var(name, replacement);
            a || b || c
        }
        NodeKind::Assign { target, value } | NodeKind::CompoundAssign { target, value, .. } => {
            let a = target.substitute_var(name, replacement);
            let b = value.substitute_var(name, replacement);
            a || b
        }
        NodeKind::InlineAsm(_) => false,
        NodeKind::Expr(expr) => expr.substitute_var(name, replacement),
        NodeKind::Return(expr) => expr
            .as_mut()
            .is_some_and(|e| e.substitute_var(name, replacement)),
        NodeKind::Unsafe { tail, .. } | NodeKind::Block { tail, .. } => tail
            .as_mut()
            .is_some_and(|t| t.substitute_var(name, replacement)),
        NodeKind::While { cond, tail, .. } => {
            let a = cond.substitute_var(name, replacement);
            let b = tail
                .as_mut()
                .is_some_and(|t| t.substitute_var(name, replacement));
            a || b
        }
        NodeKind::If { cond, .. } => cond.substitute_var(name, replacement),
        NodeKind::For { iter, pat, .. } => {
            if pat.as_str() == name {
                false
            } else {
                iter.substitute_var(name, replacement)
            }
        }
        NodeKind::Loop { .. } | NodeKind::Scope { .. } | NodeKind::LabeledBlock { .. } => false,
        NodeKind::Match { expr, .. } => expr.substitute_var(name, replacement),
        NodeKind::Break(_) | NodeKind::Continue(_) => false,
    }
}

fn remove_from_parent(arena: &mut Arena, parent: NodeId, id: NodeId) {
    if let Some(kind) = arena.get_mut(parent) {
        for list in kind.child_lists_mut() {
            if let Some(pos) = list.iter().position(|&child| child == id) {
                list.remove(pos);
                return;
            }
        }
    }
}

pub(in crate::backend::engine) struct RawPtrAliasElide;

impl NodeRule for RawPtrAliasElide {
    fn name(&self) -> &'static str {
        "raw_ptr_alias::elide"
    }

    fn priority(&self) -> u32 {
        6
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::Let]
    }

    fn matches(&self, arena: &Arena, id: NodeId) -> bool {
        matches!(
            arena.get(id),
            Some(NodeKind::Let {
                mutable: true,
                ty: Some(Type::Ptr { .. }),
                init: Some(init),
                ..
            }) if source_var(init).is_some()
        )
    }

    fn apply(&self, arena: &mut Arena, id: NodeId) -> bool {
        let Some(NodeKind::Let {
            name,
            mutable: true,
            ty: Some(Type::Ptr { .. }),
            init: Some(init),
        }) = arena.get(id)
        else {
            return false;
        };
        let Some(source) = source_var(init) else {
            return false;
        };
        let name = *name;
        if name == source {
            return false;
        }
        let replacement = init.clone();

        if !all_uses_deref_transitively(arena, name, &mut Vec::new()) {
            return false;
        }

        let root = function_root(arena, id);
        if !node_substitute_var(arena, root, name.as_str(), &replacement) {
            return false;
        }
        arena.touch_subtree(root);

        let Some(parent) = arena.parent(id) else {
            return false;
        };
        let _ = arena.take(id);
        remove_from_parent(arena, parent, id);
        true
    }
}
