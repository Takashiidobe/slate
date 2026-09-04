use super::walk;
use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{FunctionOptimizer, NodeId, NodeKindTag};
use crate::backend::rust_ast::{CLibType, Expr, Prim, Type};

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

fn is_byte_ptr(ty: &Type) -> bool {
    let Type::Ptr { inner, .. } = ty else {
        return false;
    };
    matches!(
        **inner,
        Type::CLib(CLibType::CHAR) | Type::Prim(Prim::I8) | Type::Prim(Prim::U8)
    )
}

fn cstr_content(expr: &Expr) -> Option<Vec<u8>> {
    let Expr::Cast { expr: inner, ty } = expr else {
        return None;
    };
    if !is_byte_ptr(ty) {
        return None;
    }
    let mut inner = inner.as_ref();
    while let Expr::Cast { expr: next, ty } = inner {
        if !is_byte_ptr(ty) {
            return None;
        }
        inner = next.as_ref();
    }
    let Expr::MethodCall { recv, method, args } = inner else {
        return None;
    };
    if method.as_str() != "as_ptr" || !args.is_empty() {
        return None;
    }
    match recv.as_ref() {
        Expr::CStr(bytes) => Some(bytes.clone()),
        Expr::ByteStr(bytes) => {
            let (last, rest) = bytes.split_last()?;
            (*last == 0 && !rest.contains(&0)).then(|| rest.to_vec())
        }
        _ => None,
    }
}

fn rewrite_of(expr: &Expr) -> Option<Expr> {
    let content = cstr_content(expr)?;
    let Expr::Cast { ty, .. } = expr else {
        return None;
    };
    let ptr = Expr::MethodCall {
        recv: Box::new(Expr::CStr(content)),
        method: "as_ptr".into(),
        args: Vec::new(),
    };
    let rewritten = if is_const_c_char_ptr(ty) {
        ptr
    } else {
        Expr::Cast {
            expr: Box::new(ptr),
            ty: ty.clone(),
        }
    };
    (rewritten != *expr).then_some(rewritten)
}

fn rewrite_here(expr: &mut Expr) -> bool {
    let Some(rewritten) = rewrite_of(expr) else {
        return false;
    };
    *expr = rewritten;
    true
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

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        arena.get(id).is_some_and(|kind| {
            let mut found = false;
            walk::visit_kind_exprs(kind, |expr| {
                found |= walk::any_collapsible(expr, &|expr| rewrite_of(expr).is_some())
            });
            found
        })
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        let Some(kind) = arena.get_mut(id) else {
            return false;
        };
        let mut changed = false;
        walk::visit_kind_exprs_mut(kind, |expr| {
            changed |= walk::fold_bottom_up(expr, &mut rewrite_here)
        });
        if !changed {
            return false;
        }
        arena.touch(id);
        true
    }
}
