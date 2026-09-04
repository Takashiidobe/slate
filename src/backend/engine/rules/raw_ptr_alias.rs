use super::walk;
use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{Arena, FunctionOptimizer, NodeId, NodeKind, NodeKindTag};
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
            block.stmts.iter().all(|s| !s.reads_var(name.as_str()))
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

fn bare_replacement(arena: &FunctionOptimizer, source: Ident, declared_ty: &Type) -> Option<Expr> {
    let Type::Ptr {
        inner: ptr_inner, ..
    } = declared_ty
    else {
        return None;
    };
    let Some(Type::Ref {
        mutable: true,
        inner: ref_inner,
    }) = arena.param_type(source)
    else {
        return None;
    };
    (ref_inner.as_ref() == ptr_inner.as_ref()).then_some(Expr::Var(source))
}

fn is_bare_rebind_of(kind: &NodeKind, name: Ident) -> bool {
    matches!(
        kind,
        NodeKind::Assign { target: Expr::Var(v), .. }
        | NodeKind::CompoundAssign { target: Expr::Var(v), .. }
        if *v == name
    )
}

fn is_single_assignment(arena: &Arena, name: Ident, except: NodeId) -> bool {
    arena.def_use_neighbors(name).iter().all(|&neighbor| {
        neighbor == except
            || !arena
                .get(neighbor)
                .is_some_and(|kind| is_bare_rebind_of(kind, name))
    })
}

fn pure_rename_target(arena: &Arena, kind: &NodeKind, id: NodeId, name: Ident) -> Option<Ident> {
    match kind {
        NodeKind::Let {
            name: temp,
            init: Some(Expr::Var(v)),
            ..
        } if *v == name => Some(*temp),
        NodeKind::Assign {
            target: Expr::Var(temp),
            value: Expr::Var(v),
        } if *v == name && is_single_assignment(arena, *temp, id) => Some(*temp),
        _ => None,
    }
}

fn all_uses_deref_transitively(
    arena: &Arena,
    name: Ident,
    except: Option<NodeId>,
    visited: &mut Vec<Ident>,
) -> bool {
    if visited.contains(&name) {
        return false;
    }
    visited.push(name);
    arena.def_use_neighbors(name).iter().all(|&neighbor| {
        if Some(neighbor) == except {
            return true;
        }
        let Some(kind) = arena.get(neighbor) else {
            return true;
        };
        kind_only_deref_uses(kind, name)
            || pure_rename_target(arena, kind, neighbor, name).is_some_and(|temp| {
                all_uses_deref_transitively(arena, temp, Some(neighbor), visited)
            })
    })
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

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
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

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        let Some(NodeKind::Let {
            name,
            mutable: true,
            ty: Some(ty @ Type::Ptr { .. }),
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
        let replacement = bare_replacement(arena, source, ty).unwrap_or_else(|| init.clone());

        if !all_uses_deref_transitively(arena, name, None, &mut Vec::new()) {
            return false;
        }

        let root = walk::function_root(arena, id);
        if !walk::substitute_var_in_subtree(
            arena,
            root,
            name.as_str(),
            &replacement,
            &kind_own_substitute_var,
        ) {
            return false;
        }
        arena.touch_subtree(root);

        let Some(parent) = arena.parent(id) else {
            return false;
        };
        let _ = arena.take(id);
        let _ = walk::remove_from_parent(arena, parent, id);
        true
    }
}
