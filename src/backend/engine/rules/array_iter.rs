use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{Arena, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::{Expr, Ident, RustValue, Type};

fn int_value(expr: &Expr) -> Option<i128> {
    match expr {
        Expr::Value(RustValue::I64(n)) => Some(*n as i128),
        Expr::Value(RustValue::I128(n)) => Some(*n),
        Expr::Value(RustValue::TypedInt(n, _)) => Some(*n),
        _ => None,
    }
}

fn is_var(expr: &Expr, name: Ident) -> bool {
    matches!(expr, Expr::Var(v) if *v == name)
}

fn is_ind_var_chain(expr: &Expr, ind_var: Ident) -> bool {
    match expr {
        Expr::Cast { expr, .. } => is_ind_var_chain(expr, ind_var),
        other => is_var(other, ind_var),
    }
}

fn direct_index_match(expr: &Expr, ind_var: Ident) -> Option<Ident> {
    match expr {
        Expr::Index { base, index } if is_ind_var_chain(index, ind_var) => match base.as_ref() {
            Expr::Var(arr) => Some(*arr),
            _ => None,
        },
        _ => None,
    }
}

fn array_index_target(expr: &Expr, ind_var: Ident) -> Option<Ident> {
    if let Some(arr) = direct_index_match(expr, ind_var) {
        return Some(arr);
    }
    match expr {
        Expr::Binary { lhs, rhs, .. } => {
            array_index_target(lhs, ind_var).or_else(|| array_index_target(rhs, ind_var))
        }
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => array_index_target(expr, ind_var),
        Expr::Call { func, args, .. } => array_index_target(func, ind_var)
            .or_else(|| args.iter().find_map(|arg| array_index_target(arg, ind_var))),
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            array_index_target(recv, ind_var)
                .or_else(|| args.iter().find_map(|arg| array_index_target(arg, ind_var)))
        }
        Expr::Field { base, .. } | Expr::TupleField { base, .. } => {
            array_index_target(base, ind_var)
        }
        Expr::Index { base, index } => {
            array_index_target(base, ind_var).or_else(|| array_index_target(index, ind_var))
        }
        _ => None,
    }
}

fn count_var(arena: &Arena, id: NodeId, name: Ident) -> usize {
    let own = arena.reads(id).iter().filter(|&&v| v == name).count();
    let Some(kind) = arena.get(id) else {
        return own;
    };
    own + kind
        .child_lists()
        .into_iter()
        .flatten()
        .map(|&child| count_var(arena, child, name))
        .sum::<usize>()
}

fn array_len(arena: &Arena, name: Ident) -> Option<u64> {
    let ty = arena.var_type(name)?.peel_aligned();
    match ty {
        Type::Array { elem, len } if matches!(elem.as_ref(), Type::Prim(_)) => Some(*len),
        _ => None,
    }
}

fn replace_index_use(expr: &mut Expr, arr: Ident, ind_var: Ident) -> bool {
    if direct_index_match(expr, ind_var) == Some(arr) {
        *expr = Expr::Var(ind_var);
        return true;
    }
    match expr {
        Expr::Binary { lhs, rhs, .. } => {
            replace_index_use(lhs, arr, ind_var) || replace_index_use(rhs, arr, ind_var)
        }
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => replace_index_use(expr, arr, ind_var),
        Expr::Call { func, args, .. } => {
            replace_index_use(func, arr, ind_var)
                || args
                    .iter_mut()
                    .any(|arg| replace_index_use(arg, arr, ind_var))
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            replace_index_use(recv, arr, ind_var)
                || args
                    .iter_mut()
                    .any(|arg| replace_index_use(arg, arr, ind_var))
        }
        Expr::Field { base, .. } | Expr::TupleField { base, .. } => {
            replace_index_use(base, arr, ind_var)
        }
        Expr::Index { base, index } => {
            replace_index_use(base, arr, ind_var) || replace_index_use(index, arr, ind_var)
        }
        _ => false,
    }
}

fn replace_index_use_stmt(arena: &mut Arena, id: NodeId, arr: Ident, ind_var: Ident) -> bool {
    let Some(kind) = arena.get_mut(id) else {
        return false;
    };
    let replaced = match kind {
        NodeKind::Let { init: Some(e), .. } => replace_index_use(e, arr, ind_var),
        NodeKind::Assign { target, value } | NodeKind::CompoundAssign { target, value, .. } => {
            replace_index_use(target, arr, ind_var) || replace_index_use(value, arr, ind_var)
        }
        NodeKind::Expr(e) => replace_index_use(e, arr, ind_var),
        NodeKind::Return(Some(e)) => replace_index_use(e, arr, ind_var),
        NodeKind::If { cond, .. } => replace_index_use(cond, arr, ind_var),
        NodeKind::Match { expr, .. } => replace_index_use(expr, arr, ind_var),
        NodeKind::For { iter, .. } => replace_index_use(iter, arr, ind_var),
        _ => false,
    };
    if replaced {
        arena.touch(id);
        return true;
    }
    let children: Vec<NodeId> = arena
        .get(id)
        .map(|kind| kind.child_lists().into_iter().flatten().copied().collect())
        .unwrap_or_default();
    children
        .into_iter()
        .any(|child| replace_index_use_stmt(arena, child, arr, ind_var))
}

fn find_target(
    arena: &Arena,
    ind_var: Ident,
    start: &Expr,
    end: &Expr,
    body: &[NodeId],
) -> Option<Ident> {
    if int_value(start) != Some(0) {
        return None;
    }
    let end_value = int_value(end)?;

    let mut arr = None;
    for &id in body {
        let Some(kind) = arena.get(id) else { continue };
        let found = match kind {
            NodeKind::Let { init: Some(e), .. } => array_index_target(e, ind_var),
            NodeKind::Assign { target, value } | NodeKind::CompoundAssign { target, value, .. } => {
                array_index_target(target, ind_var).or_else(|| array_index_target(value, ind_var))
            }
            NodeKind::Expr(e) => array_index_target(e, ind_var),
            NodeKind::Return(Some(e)) => array_index_target(e, ind_var),
            NodeKind::If { cond, .. } => array_index_target(cond, ind_var),
            NodeKind::Match { expr, .. } => array_index_target(expr, ind_var),
            _ => None,
        };
        if let Some(found) = found {
            if arr.is_some() && arr != Some(found) {
                return None;
            }
            arr = Some(found);
        }
    }
    let arr = arr?;
    if array_len(arena, arr)? != end_value as u64 {
        return None;
    }

    let ind_var_uses: usize = body.iter().map(|&id| count_var(arena, id, ind_var)).sum();
    if ind_var_uses != 1 {
        return None;
    }
    let arr_uses: usize = body.iter().map(|&id| count_var(arena, id, arr)).sum();
    if arr_uses != 1 {
        return None;
    }

    Some(arr)
}

pub(in crate::backend::engine) struct ForArrayIterRecover;

impl NodeRule for ForArrayIterRecover {
    fn name(&self) -> &'static str {
        "array_iter::recover"
    }

    fn priority(&self) -> u32 {
        14
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::For]
    }

    fn matches(&self, arena: &Arena, id: NodeId) -> bool {
        let Some(NodeKind::For { pat, iter, body }) = arena.get(id) else {
            return false;
        };
        let Expr::Range { start, end } = iter else {
            return false;
        };
        find_target(arena, *pat, start, end, body).is_some()
    }

    fn apply(&self, arena: &mut Arena, id: NodeId) -> bool {
        let Some(NodeKind::For { pat, iter, body }) = arena.get(id) else {
            return false;
        };
        let Expr::Range { start, end } = iter else {
            return false;
        };
        let ind_var = *pat;
        let Some(arr) = find_target(arena, ind_var, start, end, body) else {
            return false;
        };
        let body = body.clone();

        if !body
            .iter()
            .any(|&stmt| replace_index_use_stmt(arena, stmt, arr, ind_var))
        {
            return false;
        }

        let Some(NodeKind::For { iter, .. }) = arena.get_mut(id) else {
            return false;
        };
        *iter = Expr::MethodCall {
            recv: Box::new(Expr::MethodCall {
                recv: Box::new(Expr::Var(arr)),
                method: "iter".to_string(),
                args: Vec::new(),
            }),
            method: "copied".to_string(),
            args: Vec::new(),
        };
        arena.touch(id);
        true
    }
}
