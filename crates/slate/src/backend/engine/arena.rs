use std::collections::HashMap;

use crate::backend::rust_ast::{Expr, Ident, IndentStmt, InlineAsm, Label, Pattern, Stmt};

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(in crate::backend) struct NodeId {
    index: u32,
    generation: u32,
}

impl NodeId {
    pub(in crate::backend) fn index(self) -> u32 {
        self.index
    }
}

pub(in crate::backend) struct MatchArmNode {
    pub(in crate::backend) pattern: Pattern,
    pub(in crate::backend) body: Vec<NodeId>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub(in crate::backend) enum NodeKindTag {
    Let,
    LetIf,
    Assign,
    CompoundAssign,
    InlineAsm,
    Expr,
    Return,
    Unsafe,
    If,
    Loop,
    For,
    Scope,
    LabeledBlock,
    Match,
    Break,
    Continue,
    While,
    Block,
}

impl NodeKindTag {
    pub(in crate::backend) const COUNT: usize = 18;
}

pub(in crate::backend) enum NodeKind {
    Let {
        name: Ident,
        mutable: bool,
        ty: Option<crate::backend::rust_ast::Type>,
        init: Option<Expr>,
    },
    LetIf {
        name: Ident,
        mutable: bool,
        ty: Option<crate::backend::rust_ast::Type>,
        cond: Expr,
        then_body: Vec<NodeId>,
        then_value: Expr,
        else_body: Vec<NodeId>,
        else_value: Expr,
    },
    Assign {
        target: Expr,
        value: Expr,
    },
    CompoundAssign {
        target: Expr,
        op: crate::backend::rust_ast::BinOp,
        value: Expr,
    },
    InlineAsm(InlineAsm),
    Expr(Expr),
    Return(Option<Expr>),
    Unsafe {
        stmts: Vec<NodeId>,
        tail: Option<Box<Expr>>,
    },
    If {
        cond: Expr,
        then_body: Vec<NodeId>,
        else_body: Vec<NodeId>,
    },
    Loop {
        label: Option<Label>,
        body: Vec<NodeId>,
    },
    For {
        pat: Ident,
        iter: Expr,
        body: Vec<NodeId>,
    },
    Scope {
        body: Vec<NodeId>,
    },
    LabeledBlock {
        label: Label,
        body: Vec<NodeId>,
    },
    Match {
        expr: Expr,
        arms: Vec<MatchArmNode>,
    },
    Break(Option<Label>),
    Continue(Option<Label>),
    While {
        cond: Expr,
        stmts: Vec<NodeId>,
        tail: Option<Box<Expr>>,
    },
    Block {
        stmts: Vec<NodeId>,
        tail: Option<Box<Expr>>,
    },
}

impl NodeKind {
    pub(in crate::backend) fn tag(&self) -> NodeKindTag {
        match self {
            NodeKind::Let { .. } => NodeKindTag::Let,
            NodeKind::LetIf { .. } => NodeKindTag::LetIf,
            NodeKind::Assign { .. } => NodeKindTag::Assign,
            NodeKind::CompoundAssign { .. } => NodeKindTag::CompoundAssign,
            NodeKind::InlineAsm(_) => NodeKindTag::InlineAsm,
            NodeKind::Expr(_) => NodeKindTag::Expr,
            NodeKind::Return(_) => NodeKindTag::Return,
            NodeKind::Unsafe { .. } => NodeKindTag::Unsafe,
            NodeKind::If { .. } => NodeKindTag::If,
            NodeKind::Loop { .. } => NodeKindTag::Loop,
            NodeKind::For { .. } => NodeKindTag::For,
            NodeKind::Scope { .. } => NodeKindTag::Scope,
            NodeKind::LabeledBlock { .. } => NodeKindTag::LabeledBlock,
            NodeKind::Match { .. } => NodeKindTag::Match,
            NodeKind::Break(_) => NodeKindTag::Break,
            NodeKind::Continue(_) => NodeKindTag::Continue,
            NodeKind::While { .. } => NodeKindTag::While,
            NodeKind::Block { .. } => NodeKindTag::Block,
        }
    }

    pub(in crate::backend) fn declared_name(&self) -> Option<Ident> {
        match self {
            NodeKind::Let { name, .. } | NodeKind::LetIf { name, .. } => Some(*name),
            NodeKind::For { pat, .. } => Some(*pat),
            _ => None,
        }
    }

    pub(in crate::backend) fn call_anchor(&self) -> Option<Ident> {
        match self {
            NodeKind::Expr(expr)
            | NodeKind::Return(Some(expr))
            | NodeKind::Assign { value: expr, .. }
            | NodeKind::CompoundAssign { value: expr, .. } => expr_call_anchor(expr),
            NodeKind::Let {
                init: Some(expr), ..
            } => expr_call_anchor(expr),
            _ => None,
        }
    }

    pub(in crate::backend) fn child_lists_mut(&mut self) -> Vec<&mut Vec<NodeId>> {
        match self {
            NodeKind::LetIf {
                then_body,
                else_body,
                ..
            } => vec![then_body, else_body],
            NodeKind::Unsafe { stmts, .. }
            | NodeKind::While { stmts, .. }
            | NodeKind::Block { stmts, .. } => {
                vec![stmts]
            }
            NodeKind::If {
                then_body,
                else_body,
                ..
            } => vec![then_body, else_body],
            NodeKind::Loop { body, .. }
            | NodeKind::For { body, .. }
            | NodeKind::Scope { body }
            | NodeKind::LabeledBlock { body, .. } => vec![body],
            NodeKind::Match { arms, .. } => arms.iter_mut().map(|arm| &mut arm.body).collect(),
            NodeKind::Let { .. }
            | NodeKind::Assign { .. }
            | NodeKind::CompoundAssign { .. }
            | NodeKind::InlineAsm(_)
            | NodeKind::Expr(_)
            | NodeKind::Return(_)
            | NodeKind::Break(_)
            | NodeKind::Continue(_) => Vec::new(),
        }
    }

    pub(in crate::backend) fn child_lists(&self) -> Vec<&Vec<NodeId>> {
        match self {
            NodeKind::LetIf {
                then_body,
                else_body,
                ..
            } => vec![then_body, else_body],
            NodeKind::Unsafe { stmts, .. }
            | NodeKind::While { stmts, .. }
            | NodeKind::Block { stmts, .. } => {
                vec![stmts]
            }
            NodeKind::If {
                then_body,
                else_body,
                ..
            } => vec![then_body, else_body],
            NodeKind::Loop { body, .. }
            | NodeKind::For { body, .. }
            | NodeKind::Scope { body }
            | NodeKind::LabeledBlock { body, .. } => vec![body],
            NodeKind::Match { arms, .. } => arms.iter().map(|arm| &arm.body).collect(),
            NodeKind::Let { .. }
            | NodeKind::Assign { .. }
            | NodeKind::CompoundAssign { .. }
            | NodeKind::InlineAsm(_)
            | NodeKind::Expr(_)
            | NodeKind::Return(_)
            | NodeKind::Break(_)
            | NodeKind::Continue(_) => Vec::new(),
        }
    }
}

fn expr_call_anchor(expr: &Expr) -> Option<Ident> {
    match expr {
        Expr::Call { func, .. } => match func.as_ref() {
            Expr::Var(name) => Some(*name),
            Expr::Path(path) => path.segments.last().copied(),
            _ => None,
        },
        Expr::Cast { expr, .. } => expr_call_anchor(expr),
        Expr::Unsafe(block) | Expr::Block(block) if block.stmts.is_empty() => {
            block.tail.as_deref().and_then(expr_call_anchor)
        }
        _ => None,
    }
}

fn own_reads(kind: &NodeKind) -> Vec<Ident> {
    let mut out = Vec::new();
    match kind {
        NodeKind::Let { init, .. } => {
            if let Some(init) = init {
                init.collect_vars(&mut out);
            }
        }
        NodeKind::LetIf {
            cond,
            then_value,
            else_value,
            ..
        } => {
            cond.collect_vars(&mut out);
            then_value.collect_vars(&mut out);
            else_value.collect_vars(&mut out);
        }
        NodeKind::Assign { target, value } | NodeKind::CompoundAssign { target, value, .. } => {
            target.collect_vars(&mut out);
            value.collect_vars(&mut out);
        }
        NodeKind::InlineAsm(asm) => {
            for operand in &asm.operands {
                operand.visit_exprs(&mut |expr| expr.collect_vars(&mut out));
            }
        }
        NodeKind::Expr(expr) => expr.collect_vars(&mut out),
        NodeKind::Return(expr) => {
            if let Some(expr) = expr {
                expr.collect_vars(&mut out);
            }
        }
        NodeKind::Unsafe { tail, .. } | NodeKind::Block { tail, .. } => {
            if let Some(tail) = tail.as_deref() {
                tail.collect_vars(&mut out);
            }
        }
        NodeKind::While { cond, tail, .. } => {
            cond.collect_vars(&mut out);
            if let Some(tail) = tail.as_deref() {
                tail.collect_vars(&mut out);
            }
        }
        NodeKind::If { cond, .. } => cond.collect_vars(&mut out),
        NodeKind::For { iter, .. } => iter.collect_vars(&mut out),
        NodeKind::Loop { .. } | NodeKind::Scope { .. } | NodeKind::LabeledBlock { .. } => {}
        NodeKind::Match { expr, .. } => expr.collect_vars(&mut out),
        NodeKind::Break(_) | NodeKind::Continue(_) => {}
    }
    out
}

struct Slot {
    generation: u32,
    parent: Option<NodeId>,
    kind: Option<NodeKind>,
    reads: Vec<Ident>,
}

pub(in crate::backend) struct Arena {
    slots: Vec<Slot>,
    free: Vec<u32>,
    def_uses: HashMap<Ident, Vec<NodeId>>,
    defs: HashMap<Ident, NodeId>,
    param_types: HashMap<Ident, crate::backend::rust_ast::Type>,
}

impl Arena {
    fn new() -> Self {
        Self {
            slots: Vec::new(),
            free: Vec::new(),
            def_uses: HashMap::new(),
            defs: HashMap::new(),
            param_types: HashMap::new(),
        }
    }

    pub(in crate::backend) fn definition(&self, name: Ident) -> Option<NodeId> {
        self.defs.get(&name).copied()
    }

    pub(in crate::backend) fn param_type(
        &self,
        name: Ident,
    ) -> Option<&crate::backend::rust_ast::Type> {
        self.param_types.get(&name)
    }

    fn reserve(&mut self, parent: Option<NodeId>) -> NodeId {
        if let Some(index) = self.free.pop() {
            let slot = &mut self.slots[index as usize];
            slot.parent = parent;
            slot.kind = None;
            debug_assert!(slot.reads.is_empty());
            NodeId {
                index,
                generation: slot.generation,
            }
        } else {
            let index = self.slots.len() as u32;
            self.slots.push(Slot {
                generation: 0,
                parent,
                kind: None,
                reads: Vec::new(),
            });
            NodeId {
                index,
                generation: 0,
            }
        }
    }

    fn fill(&mut self, id: NodeId, kind: NodeKind) {
        if let Some(name) = kind.declared_name() {
            self.defs.insert(name, id);
        }
        self.slots[id.index as usize].kind = Some(kind);
        self.touch(id);
    }

    pub(in crate::backend) fn set_kind(&mut self, id: NodeId, kind: NodeKind) {
        if let Some(name) = self.get(id).and_then(NodeKind::declared_name)
            && self.defs.get(&name) == Some(&id)
        {
            self.defs.remove(&name);
        }
        if let Some(name) = kind.declared_name() {
            self.defs.insert(name, id);
        }
        if let Some(slot) = self.slots.get_mut(id.index as usize)
            && slot.generation == id.generation
        {
            slot.kind = Some(kind);
        }
        self.touch(id);
    }

    pub(in crate::backend) fn touch(&mut self, id: NodeId) {
        let Some(slot) = self.slots.get(id.index as usize) else {
            return;
        };
        if slot.generation != id.generation {
            return;
        }
        let new_reads = slot.kind.as_ref().map(own_reads).unwrap_or_default();
        let slot = &mut self.slots[id.index as usize];
        let old_reads = std::mem::replace(&mut slot.reads, new_reads.clone());
        for name in &old_reads {
            if !new_reads.contains(name)
                && let Some(readers) = self.def_uses.get_mut(name)
            {
                readers.retain(|&reader| reader != id);
            }
        }
        for name in &new_reads {
            if !old_reads.contains(name) {
                self.def_uses.entry(*name).or_default().push(id);
            }
        }
    }

    pub(in crate::backend) fn touch_subtree(&mut self, id: NodeId) {
        self.touch(id);
        let children: Vec<NodeId> = self
            .get(id)
            .map(|kind| kind.child_lists().into_iter().flatten().copied().collect())
            .unwrap_or_default();
        for child in children {
            self.touch_subtree(child);
        }
    }

    pub(in crate::backend) fn reads(&self, id: NodeId) -> &[Ident] {
        self.slots
            .get(id.index as usize)
            .filter(|slot| slot.generation == id.generation)
            .map(|slot| slot.reads.as_slice())
            .unwrap_or(&[])
    }

    pub(in crate::backend) fn def_use_neighbors(&self, name: Ident) -> &[NodeId] {
        self.def_uses.get(&name).map(Vec::as_slice).unwrap_or(&[])
    }

    pub(in crate::backend) fn get(&self, id: NodeId) -> Option<&NodeKind> {
        self.slots
            .get(id.index as usize)
            .filter(|slot| slot.generation == id.generation)
            .and_then(|slot| slot.kind.as_ref())
    }

    pub(in crate::backend) fn get_mut(&mut self, id: NodeId) -> Option<&mut NodeKind> {
        self.slots
            .get_mut(id.index as usize)
            .filter(|slot| slot.generation == id.generation)
            .and_then(|slot| slot.kind.as_mut())
    }

    pub(in crate::backend) fn parent(&self, id: NodeId) -> Option<NodeId> {
        self.slots
            .get(id.index as usize)
            .filter(|slot| slot.generation == id.generation)
            .and_then(|slot| slot.parent)
    }

    pub(in crate::backend) fn set_parent(&mut self, id: NodeId, parent: Option<NodeId>) {
        if let Some(slot) = self.slots.get_mut(id.index as usize)
            && slot.generation == id.generation
        {
            slot.parent = parent;
        }
    }

    pub(in crate::backend) fn take(&mut self, id: NodeId) -> Option<NodeKind> {
        let slot = self.slots.get_mut(id.index as usize)?;
        if slot.generation != id.generation {
            return None;
        }
        let old_reads = std::mem::take(&mut slot.reads);
        for name in &old_reads {
            if let Some(readers) = self.def_uses.get_mut(name) {
                readers.retain(|&reader| reader != id);
            }
        }
        let slot = &mut self.slots[id.index as usize];
        let kind = slot.kind.take();
        slot.generation += 1;
        slot.parent = None;
        self.free.push(id.index);
        if let Some(name) = kind.as_ref().and_then(NodeKind::declared_name)
            && self.defs.get(&name) == Some(&id)
        {
            self.defs.remove(&name);
        }
        kind
    }

    pub(in crate::backend) fn reparent_children(
        &mut self,
        kind: &mut NodeKind,
        new_parent: Option<NodeId>,
    ) {
        for list in kind.child_lists_mut() {
            for &child in list.iter() {
                self.set_parent(child, new_parent);
            }
        }
    }

    pub(in crate::backend) fn live_ids(&self) -> Vec<NodeId> {
        self.slots
            .iter()
            .enumerate()
            .filter(|(_, slot)| slot.kind.is_some())
            .map(|(index, slot)| NodeId {
                index: index as u32,
                generation: slot.generation,
            })
            .collect()
    }

    pub(in crate::backend) fn resolve(&self, index: u32) -> Option<NodeId> {
        let slot = self.slots.get(index as usize)?;
        slot.kind.is_some().then_some(NodeId {
            index,
            generation: slot.generation,
        })
    }
}

pub(in crate::backend) struct FunctionArena {
    pub(in crate::backend) arena: Arena,
    pub(in crate::backend) root: NodeId,
}

pub(in crate::backend) fn build(
    body: Vec<IndentStmt>,
    params: &[crate::backend::rust_ast::FnParam],
) -> FunctionArena {
    let mut arena = Arena::new();
    for param in params {
        arena
            .param_types
            .insert(Ident::new(&param.name), param.ty.clone());
    }
    let root = arena.reserve(None);
    let stmts = build_stmts(&mut arena, Some(root), body);
    arena.fill(root, NodeKind::Block { stmts, tail: None });
    FunctionArena { arena, root }
}

fn build_stmts(arena: &mut Arena, parent: Option<NodeId>, stmts: Vec<IndentStmt>) -> Vec<NodeId> {
    stmts
        .into_iter()
        .map(|stmt| build_stmt(arena, parent, stmt.stmt))
        .collect()
}

fn build_stmt(arena: &mut Arena, parent: Option<NodeId>, stmt: Stmt) -> NodeId {
    let id = arena.reserve(parent);
    let kind = match stmt {
        Stmt::Let {
            name,
            mutable,
            ty,
            init,
        } => NodeKind::Let {
            name: name.into(),
            mutable,
            ty,
            init,
        },
        Stmt::LetIf {
            name,
            mutable,
            ty,
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
        } => NodeKind::LetIf {
            name: name.into(),
            mutable,
            ty,
            cond,
            then_body: build_stmts(arena, Some(id), then_body),
            then_value,
            else_body: build_stmts(arena, Some(id), else_body),
            else_value,
        },
        Stmt::Assign { target, value } => NodeKind::Assign { target, value },
        Stmt::CompoundAssign { target, op, value } => {
            NodeKind::CompoundAssign { target, op, value }
        }
        Stmt::InlineAsm(asm) => NodeKind::InlineAsm(asm),
        Stmt::Expr(expr) => NodeKind::Expr(expr),
        Stmt::Return(expr) => NodeKind::Return(expr),
        Stmt::Unsafe { body } => NodeKind::Unsafe {
            stmts: build_stmts(arena, Some(id), body.stmts),
            tail: body.tail,
        },
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => NodeKind::If {
            cond,
            then_body: build_stmts(arena, Some(id), then_body),
            else_body: build_stmts(arena, Some(id), else_body),
        },
        Stmt::Loop { label, body } => NodeKind::Loop {
            label,
            body: build_stmts(arena, Some(id), body),
        },
        Stmt::For { pat, iter, body } => NodeKind::For {
            pat: pat.into(),
            iter,
            body: build_stmts(arena, Some(id), body),
        },
        Stmt::Scope { body } => NodeKind::Scope {
            body: build_stmts(arena, Some(id), body),
        },
        Stmt::LabeledBlock { label, body } => NodeKind::LabeledBlock {
            label,
            body: build_stmts(arena, Some(id), body),
        },
        Stmt::Match { expr, arms } => NodeKind::Match {
            expr,
            arms: arms
                .into_iter()
                .map(|arm| MatchArmNode {
                    pattern: arm.pattern,
                    body: build_stmts(arena, Some(id), arm.body),
                })
                .collect(),
        },
        Stmt::Break(label) => NodeKind::Break(label),
        Stmt::Continue(label) => NodeKind::Continue(label),
        Stmt::While { cond, body } => NodeKind::While {
            cond,
            stmts: build_stmts(arena, Some(id), body.stmts),
            tail: body.tail,
        },
        Stmt::Block(body) => NodeKind::Block {
            stmts: build_stmts(arena, Some(id), body.stmts),
            tail: body.tail,
        },
    };
    arena.fill(id, kind);
    id
}

pub(in crate::backend) fn reify(arena: &Arena, root: NodeId) -> Vec<IndentStmt> {
    let Some(NodeKind::Block { stmts, .. }) = arena.get(root) else {
        return Vec::new();
    };
    let mut out = Vec::new();
    reify_stmts(arena, stmts, 0, &mut out);
    out
}

fn reify_stmts(arena: &Arena, ids: &[NodeId], depth: usize, out: &mut Vec<IndentStmt>) {
    for &id in ids {
        reify_stmt(arena, id, depth, out);
    }
}

fn reify_nested(arena: &Arena, ids: &[NodeId], depth: usize) -> Vec<IndentStmt> {
    let mut out = Vec::new();
    reify_stmts(arena, ids, depth, &mut out);
    out
}

fn reify_stmt(arena: &Arena, id: NodeId, depth: usize, out: &mut Vec<IndentStmt>) {
    let Some(kind) = arena.get(id) else {
        return;
    };
    let stmt = match kind {
        NodeKind::Let {
            name,
            mutable,
            ty,
            init,
        } => Stmt::Let {
            name: name.as_str().to_string(),
            mutable: *mutable,
            ty: ty.clone(),
            init: init.clone(),
        },
        NodeKind::LetIf {
            name,
            mutable,
            ty,
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
        } => Stmt::LetIf {
            name: name.as_str().to_string(),
            mutable: *mutable,
            ty: ty.clone(),
            cond: cond.clone(),
            then_body: reify_nested(arena, then_body, depth + 1),
            then_value: then_value.clone(),
            else_body: reify_nested(arena, else_body, depth + 1),
            else_value: else_value.clone(),
        },
        NodeKind::Assign { target, value } => Stmt::Assign {
            target: target.clone(),
            value: value.clone(),
        },
        NodeKind::CompoundAssign { target, op, value } => Stmt::CompoundAssign {
            target: target.clone(),
            op: *op,
            value: value.clone(),
        },
        NodeKind::InlineAsm(asm) => Stmt::InlineAsm(asm.clone()),
        NodeKind::Expr(expr) => Stmt::Expr(expr.clone()),
        NodeKind::Return(expr) => Stmt::Return(expr.clone()),
        NodeKind::Unsafe { stmts, tail } => Stmt::Unsafe {
            body: crate::backend::rust_ast::Block {
                stmts: reify_nested(arena, stmts, depth + 1),
                tail: tail.clone(),
            },
        },
        NodeKind::If {
            cond,
            then_body,
            else_body,
        } => Stmt::If {
            cond: cond.clone(),
            then_body: reify_nested(arena, then_body, depth + 1),
            else_body: reify_nested(arena, else_body, depth + 1),
        },
        NodeKind::Loop { label, body } => Stmt::Loop {
            label: label.clone(),
            body: reify_nested(arena, body, depth + 1),
        },
        NodeKind::For { pat, iter, body } => Stmt::For {
            pat: pat.as_str().to_string(),
            iter: iter.clone(),
            body: reify_nested(arena, body, depth + 1),
        },
        NodeKind::Scope { body } => Stmt::Scope {
            body: reify_nested(arena, body, depth + 1),
        },
        NodeKind::LabeledBlock { label, body } => Stmt::LabeledBlock {
            label: label.clone(),
            body: reify_nested(arena, body, depth + 1),
        },
        NodeKind::Match { expr, arms } => Stmt::Match {
            expr: expr.clone(),
            arms: arms
                .iter()
                .map(|arm| crate::backend::rust_ast::MatchArm {
                    pattern: arm.pattern.clone(),
                    body: reify_nested(arena, &arm.body, depth + 1),
                })
                .collect(),
        },
        NodeKind::Break(label) => Stmt::Break(label.clone()),
        NodeKind::Continue(label) => Stmt::Continue(label.clone()),
        NodeKind::While { cond, stmts, tail } => Stmt::While {
            cond: cond.clone(),
            body: crate::backend::rust_ast::Block {
                stmts: reify_nested(arena, stmts, depth + 1),
                tail: tail.clone(),
            },
        },
        NodeKind::Block { stmts, tail } => Stmt::Block(crate::backend::rust_ast::Block {
            stmts: reify_nested(arena, stmts, depth + 1),
            tail: tail.clone(),
        }),
    };
    out.push(IndentStmt { depth, stmt });
}
