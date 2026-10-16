use super::walk::child_exprs;
use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{FunctionOptimizer, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::{Expr, Ident, Stmt, Type};

fn place_root(expr: &Expr) -> Option<Ident> {
    match expr {
        Expr::Var(name) => Some(*name),
        Expr::Field { base, .. } | Expr::TupleField { base, .. } | Expr::Index { base, .. } => {
            place_root(base)
        }
        _ => None,
    }
}

fn by_value_receiver(ty: Option<&Type>) -> bool {
    matches!(
        ty,
        Some(Type::Prim(_) | Type::Ptr { .. } | Type::FnPtr { .. } | Type::Ref { .. })
    )
}

struct Binding<'a> {
    name: Ident,
    ty: Option<&'a Type>,
}

impl Binding<'_> {
    fn roots(&self, place: &Expr) -> bool {
        place_root(place) == Some(self.name)
    }

    fn expr_mutates(&self, expr: &Expr) -> bool {
        let here = match expr {
            Expr::Ref {
                mutable: true,
                expr,
            }
            | Expr::AddrOf {
                mutable: true,
                expr,
            }
            | Expr::ArrayPtr {
                mutable: true,
                array: expr,
            } => self.roots(expr),
            Expr::MethodCall { recv, .. } | Expr::MethodCallGeneric { recv, .. } => {
                self.roots(recv) && !(matches!(**recv, Expr::Var(_)) && by_value_receiver(self.ty))
            }
            Expr::Macro { args, .. } => args.iter().any(|arg| self.roots(arg)),
            Expr::AtomicRef { .. }
            | Expr::AtomicLoad { .. }
            | Expr::AtomicStore { .. }
            | Expr::AtomicFetch { .. }
            | Expr::AtomicSwap { .. }
            | Expr::AtomicCompareExchange { .. } => true,
            Expr::Block(block) | Expr::Unsafe(block) => {
                block.stmts.iter().any(|stmt| self.stmt_mutates(stmt))
            }
            _ => false,
        };
        here || child_exprs(expr)
            .into_iter()
            .any(|child| self.expr_mutates(child))
    }

    fn stmt_mutates(&self, stmt: &Stmt) -> bool {
        match stmt {
            Stmt::Comment(_) => false,
            Stmt::Let { name, init, .. } => {
                name.as_str() == self.name.as_str()
                    || init.as_ref().is_some_and(|init| self.expr_mutates(init))
            }
            Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
                self.roots(target) || self.expr_mutates(target) || self.expr_mutates(value)
            }
            Stmt::Expr(expr) | Stmt::Return(Some(expr)) => self.expr_mutates(expr),
            Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => false,
            other => other.reads_var(self.name.as_str()),
        }
    }

    fn node_mutates(&self, kind: &NodeKind) -> bool {
        match kind {
            NodeKind::Assign { target, value } | NodeKind::CompoundAssign { target, value, .. } => {
                self.roots(target) || self.expr_mutates(target) || self.expr_mutates(value)
            }
            NodeKind::Let { name, init, .. } => {
                *name == self.name || init.as_ref().is_some_and(|init| self.expr_mutates(init))
            }
            NodeKind::LetIf { .. } | NodeKind::InlineAsm(_) => true,
            NodeKind::For { pat, iter, .. } => *pat == self.name || self.expr_mutates(iter),
            other => {
                let mut mutates = false;
                super::walk::visit_kind_exprs(other, |expr| mutates |= self.expr_mutates(expr));
                mutates
            }
        }
    }
}

fn demotable(arena: &FunctionOptimizer, id: NodeId) -> bool {
    let Some(NodeKind::Let {
        name,
        mutable: true,
        ty,
        init: Some(_),
    }) = arena.get(id)
    else {
        return false;
    };
    if arena.param_type(*name).is_some() || arena.definition(*name) != Some(id) {
        return false;
    }
    let binding = Binding {
        name: *name,
        ty: ty.as_ref(),
    };
    arena
        .def_use_neighbors(*name)
        .iter()
        .filter(|&&neighbor| neighbor != id)
        .all(|&neighbor| {
            arena
                .get(neighbor)
                .is_some_and(|kind| !binding.node_mutates(kind))
        })
}

pub(in crate::backend::engine) fn demote_params(arena: &mut FunctionOptimizer) {
    let immutable: Vec<Ident> = arena
        .params()
        .iter()
        .filter(|param| param.mutable)
        .map(|param| Ident::new(&param.name))
        .filter(|&name| {
            let binding = Binding {
                name,
                ty: arena.param_type(name),
            };
            arena.definition(name).is_none()
                && arena.def_use_neighbors(name).iter().all(|&neighbor| {
                    arena
                        .get(neighbor)
                        .is_some_and(|kind| !binding.node_mutates(kind))
                })
        })
        .collect();
    for name in immutable {
        arena.demote_param(name);
    }
}

pub(in crate::backend::engine) struct RemoveMut;

impl NodeRule for RemoveMut {
    fn name(&self) -> &'static str {
        "remove_mut"
    }

    fn priority(&self) -> u32 {
        0
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::Let]
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        demotable(arena, id)
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        if !demotable(arena, id) {
            return false;
        }
        let Some(NodeKind::Let { mutable, .. }) = arena.get_mut(id) else {
            return false;
        };
        *mutable = false;
        true
    }
}
