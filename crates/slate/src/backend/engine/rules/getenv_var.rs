use super::inline_temps;
use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{
    Arena, FunctionOptimizer, MatchArmNode, NodeId, NodeKind, NodeKindTag,
};
use crate::backend::rust_ast::{BinOp, Expr, Ident, Path, Pattern, RustValue};
use crate::function_identity::{CallBinding, Known};

struct RecoveredGetenv {
    getenv_site_id: NodeId,
    decl_id: Option<NodeId>,
    bool_let_id: Option<NodeId>,
    ptr_var: Ident,
    name: String,
    ok_body: Vec<NodeId>,
    err_body: Vec<NodeId>,
}

fn owning_list(arena: &Arena, id: NodeId) -> Option<(NodeId, usize)> {
    let parent_id = arena.parent(id)?;
    let list_index = arena
        .get(parent_id)?
        .child_lists()
        .iter()
        .position(|list| list.contains(&id))?;
    Some((parent_id, list_index))
}

fn writes_var(kind: &NodeKind, name: Ident) -> bool {
    match kind {
        NodeKind::Let { name: decl, .. } => *decl == name,
        NodeKind::Assign {
            target: Expr::Var(target),
            ..
        } => *target == name,
        _ => false,
    }
}

fn nearest_preceding_write(arena: &Arena, id: NodeId, name: Ident) -> Option<NodeId> {
    let (parent_id, list_index) = owning_list(arena, id)?;
    let list = arena
        .get(parent_id)?
        .child_lists()
        .into_iter()
        .nth(list_index)?;
    let position = list.iter().position(|&x| x == id)?;
    list[..position]
        .iter()
        .rev()
        .find(|&&candidate| {
            arena
                .get(candidate)
                .is_some_and(|kind| writes_var(kind, name))
        })
        .copied()
}

fn remove_from_list(arena: &mut Arena, id: NodeId) {
    let Some((parent_id, list_index)) = owning_list(arena, id) else {
        return;
    };
    if let Some(parent_kind) = arena.get_mut(parent_id)
        && let Some(list) = parent_kind.child_lists_mut().get_mut(list_index)
    {
        list.retain(|&x| x != id);
    }
}

fn peel_casts(expr: &Expr) -> &Expr {
    match expr {
        Expr::Cast { expr, .. } => peel_casts(expr),
        _ => expr,
    }
}

fn is_null_like(arena: &Arena, expr: &Expr, depth: u32) -> bool {
    if depth > 4 {
        return false;
    }
    let peeled = peel_casts(expr);
    if matches!(peeled, Expr::Value(RustValue::NullPtr)) {
        return true;
    }
    let Expr::Var(name) = peeled else {
        return false;
    };
    let Some(def_id) = arena.definition(*name) else {
        return false;
    };
    let Some(NodeKind::Let {
        mutable: false,
        init: Some(init),
        ..
    }) = arena.get(def_id)
    else {
        return false;
    };
    is_null_like(arena, init, depth + 1)
}

fn extract_null_check(arena: &Arena, cond: &Expr) -> Option<(Ident, bool)> {
    let Expr::Binary { op, lhs, rhs } = cond else {
        return None;
    };
    let is_ne = match op {
        BinOp::Ne => true,
        BinOp::Eq => false,
        _ => return None,
    };
    if let Expr::Var(v) = lhs.as_ref()
        && is_null_like(arena, rhs, 0)
    {
        return Some((*v, is_ne));
    }
    if let Expr::Var(v) = rhs.as_ref()
        && is_null_like(arena, lhs, 0)
    {
        return Some((*v, is_ne));
    }
    None
}

fn peel_wrapper(expr: &Expr) -> &Expr {
    match expr {
        Expr::Cast { expr, .. } => peel_wrapper(expr),
        Expr::Unsafe(block) | Expr::Block(block)
            if block.stmts.is_empty() && block.tail.is_some() =>
        {
            peel_wrapper(block.tail.as_deref().unwrap())
        }
        _ => expr,
    }
}

fn const_c_str_arg(expr: &Expr) -> Option<String> {
    let Expr::MethodCall { recv, method, args } = peel_wrapper(expr) else {
        return None;
    };
    if method != "as_ptr" || !args.is_empty() {
        return None;
    }
    let Expr::CStr(bytes) = recv.as_ref() else {
        return None;
    };
    String::from_utf8(bytes.clone()).ok()
}

fn const_getenv_name(expr: &Expr) -> Option<String> {
    let Expr::Call { binding, args, .. } = peel_wrapper(expr) else {
        return None;
    };
    if binding.known() != Some(Known::Getenv) || args.len() != 1 {
        return None;
    }
    const_c_str_arg(&args[0])
}

fn getenv_site_init(arena: &Arena, id: NodeId, ptr_var: Ident) -> Option<&Expr> {
    match arena.get(id)? {
        NodeKind::Let {
            name,
            init: Some(init),
            ..
        } if *name == ptr_var => Some(init),
        NodeKind::Assign {
            target: Expr::Var(name),
            value,
        } if *name == ptr_var => Some(value),
        _ => None,
    }
}

fn plan(arena: &Arena, if_id: NodeId) -> Option<RecoveredGetenv> {
    let Some(NodeKind::If {
        cond,
        then_body,
        else_body,
    }) = arena.get(if_id)
    else {
        return None;
    };

    let (ptr_var, is_ne, bool_let_id, getenv_site_id) = if let Expr::Var(flag) = cond
        && let Some(bool_let_id) = arena.definition(*flag)
        && arena.def_use_neighbors(*flag) == [if_id]
        && let Some(NodeKind::Let {
            mutable: false,
            init: Some(flag_init),
            ..
        }) = arena.get(bool_let_id)
        && let Some((ptr_var, is_ne)) = extract_null_check(arena, flag_init)
    {
        (
            ptr_var,
            is_ne,
            Some(bool_let_id),
            nearest_preceding_write(arena, bool_let_id, ptr_var)?,
        )
    } else {
        let (ptr_var, is_ne) = extract_null_check(arena, cond)?;
        (
            ptr_var,
            is_ne,
            None,
            nearest_preceding_write(arena, if_id, ptr_var)?,
        )
    };

    let init = getenv_site_init(arena, getenv_site_id, ptr_var)?;
    let name = const_getenv_name(init)?;

    let mut allowed = vec![getenv_site_id, bool_let_id.unwrap_or(if_id)];
    let decl_id = if matches!(arena.get(getenv_site_id), Some(NodeKind::Assign { .. })) {
        let decl_id = arena.definition(ptr_var)?;
        if decl_id == getenv_site_id {
            return None;
        }
        let Some(NodeKind::Let {
            name: decl_name,
            init: Some(decl_init),
            ..
        }) = arena.get(decl_id)
        else {
            return None;
        };
        if *decl_name != ptr_var || inline_temps::expr_effects(decl_init).is_side_effect() {
            return None;
        }
        allowed.push(decl_id);
        Some(decl_id)
    } else {
        None
    };

    if !arena
        .def_use_neighbors(ptr_var)
        .iter()
        .all(|n| allowed.contains(n))
    {
        return None;
    }

    let (ok_body, err_body) = if is_ne {
        (then_body.clone(), else_body.clone())
    } else {
        (else_body.clone(), then_body.clone())
    };

    Some(RecoveredGetenv {
        getenv_site_id,
        decl_id,
        bool_let_id,
        ptr_var,
        name,
        ok_body,
        err_body,
    })
}

pub(in crate::backend::engine) struct GetenvVar;

impl NodeRule for GetenvVar {
    fn name(&self) -> &'static str {
        "getenv_var::recover"
    }

    fn priority(&self) -> u32 {
        55
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::If]
    }

    fn requeues_producers(&self) -> bool {
        true
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        plan(arena, id).is_some()
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        let Some(plan) = plan(arena, id) else {
            return false;
        };

        remove_from_list(arena, plan.getenv_site_id);
        arena.take(plan.getenv_site_id);
        if let Some(decl_id) = plan.decl_id {
            remove_from_list(arena, decl_id);
            arena.take(decl_id);
        }
        if let Some(bool_let_id) = plan.bool_let_id {
            remove_from_list(arena, bool_let_id);
            arena.take(bool_let_id);
        }

        let var_call = Expr::Call {
            binding: CallBinding::Generated,
            func: Box::new(Expr::Path(Path::new(
                ["std", "env", "var"].into_iter().map(Ident::from),
            ))),
            args: vec![Expr::Str(plan.name)],
        };
        arena.set_kind(
            id,
            NodeKind::Match {
                expr: var_call,
                arms: vec![
                    MatchArmNode {
                        pattern: Pattern::TupleStruct {
                            name: Ident::new("Ok"),
                            fields: vec![Pattern::Binding(plan.ptr_var)],
                        },
                        body: plan.ok_body,
                    },
                    MatchArmNode {
                        pattern: Pattern::TupleStruct {
                            name: Ident::new("Err"),
                            fields: vec![Pattern::Wildcard],
                        },
                        body: plan.err_body,
                    },
                ],
            },
        );
        true
    }
}
