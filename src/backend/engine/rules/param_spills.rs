use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{Arena, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::Expr;

fn remove_from_parent(arena: &mut Arena, parent: NodeId, id: NodeId) -> bool {
    let Some(kind) = arena.get_mut(parent) else {
        return false;
    };
    for list in kind.child_lists_mut() {
        if let Some(pos) = list.iter().position(|&child| child == id) {
            list.remove(pos);
            return true;
        }
    }
    false
}

pub(in crate::backend::engine) struct ParamSpillFold;

impl NodeRule for ParamSpillFold {
    fn name(&self) -> &'static str {
        "param_spills::fold"
    }

    fn priority(&self) -> u32 {
        6
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::Let]
    }

    fn matches(&self, arena: &Arena, id: NodeId) -> bool {
        let Some(NodeKind::Let {
            name,
            ty: Some(ty),
            init: Some(Expr::Var(source)),
            ..
        }) = arena.get(id)
        else {
            return false;
        };
        name != source && arena.param_type(*source) == Some(ty)
    }

    fn apply(&self, arena: &mut Arena, id: NodeId) -> bool {
        let Some(NodeKind::Let {
            name,
            mutable,
            ty: Some(ty),
            init: Some(Expr::Var(source)),
        }) = arena.get(id)
        else {
            return false;
        };
        let (name, source, mutable, ty) = (*name, *source, *mutable, ty.clone());
        if name == source
            || arena.param_type(source) != Some(&ty)
            || arena.def_use_neighbors(source) != [id]
        {
            return false;
        }
        let Some(parent) = arena.parent(id) else {
            return false;
        };
        if arena.parent(parent).is_some()
            || !matches!(arena.get(parent), Some(NodeKind::Block { .. }))
        {
            return false;
        }
        if !arena.rename_param(source, name, mutable) {
            return false;
        }
        if arena.take(id).is_none() {
            return false;
        }
        remove_from_parent(arena, parent, id)
    }
}
