use std::collections::{BTreeMap, BTreeSet};

use crate::backend::facts::{
    AstPath, HeapOwnershipKind, HeapResizeKind, OptionBoxAssignKind, PathSegment,
};
use crate::backend::idents::{expr_ident_count, stmt_ident_count};
use crate::backend::query::{
    AtomicCompareExchangeChain, ByteRepresentation, ByteSource, NulPosition, PointerMutability,
    Predicate, QueryContext, Rejection, RejectionReason, StableExpr, default_value,
    find_distance_observation, null_comparison,
};
use crate::backend::rust_ast::{
    BinOp, Block, CLibType, Expr, ExprMatchArm, FnDef, Ident, IndentStmt, MatchArm, Pattern, Prim,
    RustValue, Stmt, Type, UnaryOp,
};
use crate::backend::support::walk;
use crate::function_identity::{Known, known_call};

pub(in crate::backend) struct ExprRecipe<'snapshot> {
    kind: ExprRecipeKind<'snapshot>,
}

enum ExprRecipeKind<'snapshot> {
    PointerAtOrNull {
        source: ByteSource<'snapshot>,
        index: SearchIndex,
    },
    ProcessExit,
}

pub(in crate::backend) enum SearchIndex {
    Known(NulPosition),
    Position(StableExpr),
}

pub(in crate::backend) fn recover_assert(cond: Expr, depth: usize) -> IndentStmt {
    IndentStmt {
        depth,
        stmt: Stmt::Expr(Expr::Macro {
            name: "assert".into(),
            args: vec![cond],
        }),
    }
}

pub(in crate::backend) fn preserve_assert_result(
    name: String,
    ty: Option<Type>,
    init: Expr,
    depth: usize,
) -> IndentStmt {
    IndentStmt {
        depth,
        stmt: Stmt::Let {
            name,
            mutable: false,
            ty,
            init: Some(init),
        },
    }
}

pub(in crate::backend) fn collapse_atomic_compare_exchange(
    chain: AtomicCompareExchangeChain,
) -> IndentStmt {
    let mut init = Expr::Match {
        expr: Box::new(chain.compare_exchange),
        arms: vec![
            ExprMatchArm {
                pattern: Pattern::TupleStruct {
                    name: "Ok".into(),
                    fields: vec![Pattern::Wildcard],
                },
                value: Expr::Value(RustValue::Bool(true)),
            },
            ExprMatchArm {
                pattern: Pattern::TupleStruct {
                    name: "Err".into(),
                    fields: vec![Pattern::Binding("v".into())],
                },
                value: Expr::Block(Box::new(Block {
                    stmts: vec![IndentStmt {
                        depth: 0,
                        stmt: Stmt::Assign {
                            target: Expr::Var(chain.expected_name.into()),
                            value: Expr::Var("v".into()),
                        },
                    }],
                    tail: Some(Box::new(Expr::Value(RustValue::Bool(false)))),
                })),
            },
        ],
    };
    if chain.needs_cast {
        init = Expr::Cast {
            expr: Box::new(init),
            ty: Type::Prim(Prim::I32),
        };
    }
    IndentStmt {
        depth: chain.depth,
        stmt: Stmt::Let {
            name: chain.final_name,
            mutable: chain.mutable,
            ty: chain.ty,
            init: Some(init),
        },
    }
}

pub(in crate::backend) fn pointer_at_or_null(
    source: ByteSource<'_>,
    index: SearchIndex,
) -> ExprRecipe<'_> {
    ExprRecipe {
        kind: ExprRecipeKind::PointerAtOrNull { source, index },
    }
}

pub(in crate::backend) fn process_exit() -> ExprRecipe<'static> {
    ExprRecipe {
        kind: ExprRecipeKind::ProcessExit,
    }
}

pub(in crate::backend) fn known_index(position: NulPosition) -> SearchIndex {
    SearchIndex::Known(position)
}

pub(in crate::backend) fn byte_position(needle: StableExpr) -> SearchIndex {
    SearchIndex::Position(needle)
}

impl FunctionBodyRecipe {
    pub(super) fn lower(self) -> Vec<IndentStmt> {
        self.body
    }
}

pub(in crate::backend) fn initialize_local(declaration: &Stmt, value: Expr) -> Option<Stmt> {
    let mut initialized = declaration.clone();
    let Stmt::Let { init, .. } = &mut initialized else {
        return None;
    };
    *init = Some(value);
    Some(initialized)
}

pub(in crate::backend) fn rewrite_heap_ownership(
    body: Vec<IndentStmt>,
    plans: Vec<HeapOwnershipPlan>,
) -> FunctionBodyRecipe {
    let mut body = body;
    rewrite_owned_body_stmts(&mut body, &plans);
    FunctionBodyRecipe { body }
}

pub(in crate::backend) fn rewrite_file_ownership(
    body: Vec<IndentStmt>,
    plans: Vec<super::stdio::Plan>,
) -> FunctionBodyRecipe {
    let mut replacements = BTreeMap::new();
    let mut remove = BTreeSet::new();
    for plan in plans {
        replacements.extend(plan.replacements);
        remove.extend(plan.remove);
    }
    let body = body
        .into_iter()
        .enumerate()
        .filter_map(|(index, indent)| {
            if remove.contains(&index) {
                return None;
            }
            match replacements.remove(&index) {
                Some(stmt) => Some(IndentStmt { stmt, ..indent }),
                None => Some(indent),
            }
        })
        .collect();
    FunctionBodyRecipe { body }
}

#[derive(Debug, Clone)]
pub(in crate::backend) struct HeapOwnershipPlan {
    pub(in crate::backend) pointer_name: String,
    pub(in crate::backend) kind: HeapOwnershipKind,
    pub(in crate::backend) pointer_stmt: Option<usize>,
    pub(in crate::backend) size_stmt: Option<usize>,
    pub(in crate::backend) allocation_stmt: Option<usize>,
    pub(in crate::backend) assign_stmt: Option<usize>,
    pub(in crate::backend) free_temp_stmt: Option<usize>,
    pub(in crate::backend) free_stmt: Option<usize>,
    pub(in crate::backend) reallocs: Vec<HeapOwnershipReallocPlan>,
    pub(in crate::backend) elem_ty: Type,
    pub(in crate::backend) init: Expr,
    pub(in crate::backend) count: Option<Expr>,
}

#[derive(Debug, Clone)]
pub(in crate::backend) struct HeapOwnershipReallocPlan {
    pub(in crate::backend) source_temp_stmt: Option<usize>,
    pub(in crate::backend) size_stmt: Option<usize>,
    pub(in crate::backend) allocation_stmt: Option<usize>,
    pub(in crate::backend) assign_stmt: Option<usize>,
    pub(in crate::backend) resize: HeapResizeKind,
    pub(in crate::backend) count: Expr,
}

struct OwnedHeapPlan {
    kind: HeapOwnershipKind,
}

struct OwnedHeap {
    plans: BTreeMap<String, OwnedHeapPlan>,
    aliases: BTreeMap<String, String>,
}

impl OwnedHeap {
    fn new(plans: &[HeapOwnershipPlan]) -> Self {
        Self {
            plans: plans
                .iter()
                .map(|plan| (plan.pointer_name.clone(), OwnedHeapPlan { kind: plan.kind }))
                .collect(),
            aliases: BTreeMap::new(),
        }
    }

    fn from_owners(owners: impl IntoIterator<Item = (String, HeapOwnershipKind)>) -> Self {
        Self {
            plans: owners
                .into_iter()
                .map(|(name, kind)| (name, OwnedHeapPlan { kind }))
                .collect(),
            aliases: BTreeMap::new(),
        }
    }

    fn owner_for(&self, name: &str) -> Option<String> {
        if self.plans.contains_key(name) {
            return Some(name.to_string());
        }
        self.aliases.get(name).cloned()
    }

    fn kind_for_owner(&self, owner: &str) -> Option<HeapOwnershipKind> {
        self.plans.get(owner).map(|plan| plan.kind)
    }
}

fn rewrite_owned_body_stmts(body: &mut Vec<IndentStmt>, plans: &[HeapOwnershipPlan]) -> bool {
    let mut changed = false;
    let mut remove = BTreeSet::new();
    for plan in plans {
        let Some(pointer_stmt) = plan.pointer_stmt else {
            continue;
        };
        if let Some(indent) = body.get_mut(pointer_stmt) {
            changed |= rewrite_pointer_decl(&mut indent.stmt, plan);
        }
        for index in [
            plan.size_stmt,
            calloc_count_stmt(body, plan.allocation_stmt),
            plan.allocation_stmt,
            plan.assign_stmt,
            plan.free_temp_stmt,
            plan.free_stmt,
        ]
        .into_iter()
        .flatten()
        {
            remove.insert(index);
        }
        for realloc in &plan.reallocs {
            for index in [
                realloc.source_temp_stmt,
                realloc.size_stmt,
                realloc.allocation_stmt,
            ]
            .into_iter()
            .flatten()
            {
                remove.insert(index);
            }
            if let Some(assign_stmt) = realloc.assign_stmt
                && let Some(indent) = body.get_mut(assign_stmt)
            {
                indent.stmt = realloc_stmt(&plan.pointer_name, realloc, &plan.init);
                changed = true;
            }
        }
    }

    let mut owned = OwnedHeap::new(plans);
    for (index, indent) in body.iter_mut().enumerate() {
        if remove.contains(&index) {
            continue;
        }
        if let Some((alias, owner)) = owned_alias(&indent.stmt, &owned) {
            owned.aliases.insert(alias, owner);
            remove.insert(index);
            continue;
        }
        changed |= rewrite_owned_stmt(&mut indent.stmt, &owned);
    }
    changed |= fuse_scalar_box_initializers(body, plans, &mut remove);
    for index in remove.into_iter().rev() {
        if index < body.len() {
            body.remove(index);
            changed = true;
        }
    }
    changed
}

fn fuse_scalar_box_initializers(
    body: &mut [IndentStmt],
    plans: &[HeapOwnershipPlan],
    remove: &mut BTreeSet<usize>,
) -> bool {
    let mut changed = false;
    for plan in plans {
        if plan.kind != HeapOwnershipKind::ScalarBox {
            continue;
        }
        let Some(pointer_stmt) = plan.pointer_stmt else {
            continue;
        };
        let Some(store_index) =
            first_scalar_box_store(pointer_stmt, body, remove, &plan.pointer_name)
        else {
            continue;
        };
        let Some(value) = scalar_box_store_value(&body[store_index].stmt, &plan.pointer_name)
        else {
            continue;
        };
        if expr_ident_count(value, &plan.pointer_name) != 0 {
            continue;
        }
        let value = value.clone();
        if set_box_new_arg(&mut body[pointer_stmt].stmt, value) {
            remove.insert(store_index);
            changed = true;
        }
    }
    changed
}

fn first_scalar_box_store(
    start: usize,
    body: &[IndentStmt],
    remove: &BTreeSet<usize>,
    pointer_name: &str,
) -> Option<usize> {
    for (index, indent) in body.iter().enumerate().skip(start + 1) {
        if remove.contains(&index) {
            continue;
        }
        if scalar_box_store_value(&indent.stmt, pointer_name).is_some() {
            return Some(index);
        }
        if stmt_blocks_scalar_box_initializer_fold(&indent.stmt, pointer_name) {
            return None;
        }
    }
    None
}

fn stmt_blocks_scalar_box_initializer_fold(stmt: &Stmt, pointer_name: &str) -> bool {
    match stmt {
        Stmt::Let { name, init, .. } => {
            name == pointer_name
                || init
                    .as_ref()
                    .is_some_and(|init| expr_ident_count(init, pointer_name) != 0)
        }
        Stmt::Assign { .. }
        | Stmt::CompoundAssign { .. }
        | Stmt::Expr(_)
        | Stmt::Return(Some(_))
        | Stmt::Return(None) => stmt_ident_count(stmt, pointer_name) != 0,
        _ => true,
    }
}

fn scalar_box_store_value<'a>(stmt: &'a Stmt, pointer_name: &str) -> Option<&'a Expr> {
    let Stmt::Assign { target, value } = stmt else {
        return None;
    };
    let Expr::Unary {
        op: UnaryOp::Deref,
        expr,
    } = target
    else {
        return None;
    };
    matches!(&**expr, Expr::Var(name) if name.as_str() == pointer_name).then_some(value)
}

fn set_box_new_arg(stmt: &mut Stmt, value: Expr) -> bool {
    let Stmt::Let {
        init: Some(Expr::Call { args, .. }),
        ..
    } = stmt
    else {
        return false;
    };
    if args.len() != 1 {
        return false;
    }
    args[0] = value;
    true
}

fn rewrite_pointer_decl(stmt: &mut Stmt, plan: &HeapOwnershipPlan) -> bool {
    let Stmt::Let {
        name,
        mutable,
        ty,
        init,
    } = stmt
    else {
        return false;
    };
    if name != &plan.pointer_name {
        return false;
    }
    *mutable = true;
    match plan.kind {
        HeapOwnershipKind::ScalarBox => {
            *ty = Some(Type::Generic {
                name: "Box".into(),
                args: vec![plan.elem_ty.clone()],
            });
            *init = Some(box_new(&plan.elem_ty));
        }
        HeapOwnershipKind::VecBuffer => {
            *ty = Some(Type::Generic {
                name: "Vec".into(),
                args: vec![plan.elem_ty.clone()],
            });
            *init = Some(Expr::VecRepeat {
                elem: Box::new(plan.init.clone()),
                len: Box::new(usize_expr(
                    plan.count
                        .clone()
                        .unwrap_or(Expr::Value(RustValue::Usize(0))),
                )),
            });
        }
    }
    true
}

fn owned_alias(stmt: &Stmt, owned: &OwnedHeap) -> Option<(String, String)> {
    let Stmt::Let {
        name,
        init: Some(Expr::Var(source)),
        ..
    } = stmt
    else {
        return None;
    };
    let owner = owned.owner_for(source.as_str())?;
    Some((name.clone(), owner))
}

fn calloc_count_stmt(body: &[IndentStmt], allocation_stmt: Option<usize>) -> Option<usize> {
    let allocation_stmt = allocation_stmt?;
    let count_stmt = allocation_stmt.checked_sub(2)?;
    let Stmt::Let {
        name: count_name,
        init: Some(_),
        ..
    } = &body.get(count_stmt)?.stmt
    else {
        return None;
    };
    let Stmt::Let {
        init: Some(Expr::Unsafe(block)),
        ..
    } = &body.get(allocation_stmt)?.stmt
    else {
        return None;
    };
    let Some(call @ Expr::Call { args, .. }) = block.tail.as_deref() else {
        return None;
    };
    if known_call(call) != Some(Known::Calloc) || args.len() != 2 {
        return None;
    }
    (size_arg_name(&args[0]) == Some(count_name.as_str())).then_some(count_stmt)
}

fn size_arg_name(expr: &Expr) -> Option<&str> {
    match expr {
        Expr::Var(name) => Some(name.as_str()),
        Expr::Cast { expr, .. } => size_arg_name(expr),
        _ => None,
    }
}

fn realloc_stmt(pointer_name: &str, realloc: &HeapOwnershipReallocPlan, init: &Expr) -> Stmt {
    match realloc.resize {
        HeapResizeKind::Shrink => Stmt::Expr(Expr::MethodCall {
            recv: Box::new(Expr::Var(pointer_name.to_string().into())),
            method: "truncate".into(),
            args: vec![usize_expr(realloc.count.clone())],
        }),
        HeapResizeKind::Grow | HeapResizeKind::SameOrUnknown => Stmt::Expr(Expr::MethodCall {
            recv: Box::new(Expr::Var(pointer_name.to_string().into())),
            method: "resize".into(),
            args: vec![usize_expr(realloc.count.clone()), init.clone()],
        }),
    }
}

fn rewrite_owned_stmt(stmt: &mut Stmt, owned: &OwnedHeap) -> bool {
    let mut changed = false;
    match stmt {
        Stmt::Unsafe { body } if body.tail.is_none() && body.stmts.len() == 1 => {
            let mut replacement = body.stmts[0].stmt.clone();
            changed |= rewrite_owned_stmt(&mut replacement, owned);
            if stmt_can_leave_unsafe(&replacement, owned) {
                *stmt = replacement;
                changed = true;
            } else if let Stmt::Unsafe { body } = stmt {
                body.stmts[0].stmt = replacement;
            }
        }
        Stmt::Unsafe { body } => changed |= rewrite_owned_block(body, owned),
        Stmt::Block(body) | Stmt::While { body, .. } => changed |= rewrite_owned_block(body, owned),
        Stmt::Let {
            init: Some(init), ..
        } => changed |= rewrite_owned_expr(init, owned),
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            changed |= rewrite_owned_expr(cond, owned);
            changed |= rewrite_owned_body(then_body, owned);
            changed |= rewrite_owned_expr(then_value, owned);
            changed |= rewrite_owned_body(else_body, owned);
            changed |= rewrite_owned_expr(else_value, owned);
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            changed |= rewrite_owned_expr(target, owned);
            changed |= rewrite_owned_expr(value, owned);
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => changed |= rewrite_owned_expr(expr, owned),
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            changed |= rewrite_owned_expr(cond, owned);
            changed |= rewrite_owned_body(then_body, owned);
            changed |= rewrite_owned_body(else_body, owned);
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            changed |= rewrite_owned_body(body, owned);
        }
        Stmt::For { iter, body, .. } => {
            changed |= rewrite_owned_expr(iter, owned);
            changed |= rewrite_owned_body(body, owned);
        }
        Stmt::Match { expr, arms } => {
            changed |= rewrite_owned_expr(expr, owned);
            for arm in arms {
                changed |= rewrite_owned_body(&mut arm.body, owned);
            }
        }
        Stmt::Let { init: None, .. }
        | Stmt::InlineAsm(_)
        | Stmt::Return(None)
        | Stmt::Break(_)
        | Stmt::Continue(_) => {}
    }
    changed
}

fn rewrite_owned_body(body: &mut [IndentStmt], owned: &OwnedHeap) -> bool {
    let mut changed = false;
    for indent in body {
        changed |= rewrite_owned_stmt(&mut indent.stmt, owned);
    }
    changed
}

fn rewrite_owned_block(block: &mut Block, owned: &OwnedHeap) -> bool {
    let mut changed = rewrite_owned_body(&mut block.stmts, owned);
    if let Some(tail) = &mut block.tail {
        changed |= rewrite_owned_expr(tail, owned);
    }
    changed
}

fn rewrite_owned_expr(expr: &mut Expr, owned: &OwnedHeap) -> bool {
    if let Some(replacement) = owned_heap_access(expr, owned) {
        *expr = replacement;
        return true;
    }
    let mut changed = false;
    walk::exprs_mut_with(expr, &mut |expr| {
        if let Some(replacement) = owned_heap_access(expr, owned) {
            *expr = replacement;
            changed = true;
            return false;
        }
        true
    });
    changed
}

fn stmt_can_leave_unsafe(stmt: &Stmt, owned: &OwnedHeap) -> bool {
    match stmt {
        Stmt::Assign { target, value } => {
            expr_can_leave_unsafe(target, owned) && expr_can_leave_unsafe(value, owned)
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => expr_can_leave_unsafe(expr, owned),
        Stmt::Let {
            init: Some(init), ..
        } => expr_can_leave_unsafe(init, owned),
        _ => false,
    }
}

fn expr_can_leave_unsafe(expr: &Expr, owned: &OwnedHeap) -> bool {
    let mut ok = true;
    crate::backend::facts::walk::exprs(expr, &mut |expr| {
        if matches!(expr, Expr::Unsafe(_)) && owned_heap_access(expr, owned).is_none() {
            ok = false;
        }
    });
    ok
}

fn owned_heap_access(expr: &Expr, owned: &OwnedHeap) -> Option<Expr> {
    match expr {
        Expr::Unsafe(block) if block.stmts.is_empty() => {
            heap_deref_replacement(block.tail.as_deref()?, owned)
        }
        Expr::Unary {
            op: UnaryOp::Deref,
            expr,
        } => pointer_replacement(expr, owned),
        _ => None,
    }
}

fn heap_deref_replacement(expr: &Expr, owned: &OwnedHeap) -> Option<Expr> {
    let Expr::Unary {
        op: UnaryOp::Deref,
        expr,
    } = expr
    else {
        return None;
    };
    pointer_replacement(expr, owned)
}

fn pointer_replacement(expr: &Expr, owned: &OwnedHeap) -> Option<Expr> {
    match expr {
        Expr::Var(name) => {
            let owner = owned.owner_for(name.as_str())?;
            if owned.kind_for_owner(&owner)? != HeapOwnershipKind::ScalarBox {
                return None;
            }
            Some(Expr::Unary {
                op: UnaryOp::Deref,
                expr: Box::new(Expr::Var(owner.into())),
            })
        }
        Expr::Unsafe(block) if block.stmts.is_empty() => {
            pointer_replacement(block.tail.as_deref()?, owned)
        }
        Expr::MethodCall { recv, method, args } if method == "add" && args.len() == 1 => {
            let Expr::Var(base) = recv.as_ref() else {
                return None;
            };
            let owner = owned.owner_for(base.as_str())?;
            if owned.kind_for_owner(&owner)? != HeapOwnershipKind::VecBuffer {
                return None;
            }
            Some(Expr::Index {
                base: Box::new(Expr::Var(owner.into())),
                index: Box::new(args[0].clone()),
            })
        }
        _ => None,
    }
}

fn usize_expr(expr: Expr) -> Expr {
    match expr {
        Expr::Value(RustValue::Usize(_)) => expr,
        Expr::Value(RustValue::I64(n)) => match usize::try_from(n) {
            Ok(n) => Expr::Value(RustValue::Usize(n)),
            Err(_) => Expr::Cast {
                expr: Box::new(Expr::Value(RustValue::I64(n))),
                ty: Type::Prim(Prim::Usize),
            },
        },
        Expr::Value(RustValue::I128(n)) => match usize::try_from(n) {
            Ok(n) => Expr::Value(RustValue::Usize(n)),
            Err(_) => Expr::Cast {
                expr: Box::new(Expr::Value(RustValue::I128(n))),
                ty: Type::Prim(Prim::Usize),
            },
        },
        Expr::Cast { expr, ty } if matches!(ty, Type::Prim(Prim::Usize)) => Expr::Cast { expr, ty },
        expr => Expr::Cast {
            expr: Box::new(expr),
            ty: Type::Prim(Prim::Usize),
        },
    }
}

fn box_new(ty: &Type) -> Expr {
    Expr::Call {
        binding: crate::function_identity::CallBinding::Generated,
        func: Box::new(Expr::Var(format!("Box::<{}>::new", ty.render()).into())),
        args: vec![default_value(ty)],
    }
}

impl ExprRecipe<'_> {
    pub(super) fn lower(
        self,
        query: &QueryContext<'_>,
        call_site: &crate::backend::query::ExprSite,
    ) -> Result<Expr, Rejection> {
        match self.kind {
            ExprRecipeKind::PointerAtOrNull { source, index } => {
                let index = match index {
                    SearchIndex::Known(position) => some(nul_index_expr(source.clone(), position)),
                    SearchIndex::Position(needle) => {
                        let value = query.expr(&needle.site).cloned().ok_or_else(|| {
                            Rejection::new(
                                Predicate::MovablePure,
                                Some(needle.site),
                                RejectionReason::MissingEvidence,
                                Vec::new(),
                            )
                        })?;
                        position(byte_source_expr(source.clone()), byte_expr(value))
                    }
                };
                Ok(pointer_search(source, index))
            }
            ExprRecipeKind::ProcessExit => {
                let Some(Expr::Call { args, .. }) = query.expr(call_site) else {
                    return Err(Rejection::new(
                        Predicate::Call,
                        Some(call_site.clone()),
                        RejectionReason::UnsupportedShape,
                        Vec::new(),
                    ));
                };
                let [status] = args.as_slice() else {
                    return Err(Rejection::new(
                        Predicate::Call,
                        Some(call_site.clone()),
                        RejectionReason::UnsupportedShape,
                        Vec::new(),
                    ));
                };
                Ok(call(path(["std", "process", "exit"]), vec![status.clone()]))
            }
        }
    }
}

fn pointer_search(source: ByteSource<'_>, index: Expr) -> Expr {
    method(index, "map_or", vec![null_mut(), index_to_ptr(source)])
}

fn byte_source_expr(source: ByteSource<'_>) -> Expr {
    match source.representation {
        ByteRepresentation::Collection => method(var(&source.name), "as_slice", Vec::new()),
        ByteRepresentation::Bytes => var(&source.name),
        ByteRepresentation::CStr => method(var(&source.name), "to_bytes", Vec::new()),
        ByteRepresentation::Str => method(var(&source.name), "as_bytes", Vec::new()),
    }
}

fn source_len(source: ByteSource<'_>) -> Expr {
    method(byte_source_expr(source), "len", Vec::new())
}

fn nul_index_expr(source: ByteSource<'_>, position: NulPosition) -> Expr {
    match position {
        NulPosition::ByteLength => source_len(source),
        NulPosition::Constant(position) => Expr::Value(RustValue::I64(position as i64)),
    }
}

fn position(source: Expr, needle: Expr) -> Expr {
    method(
        method(source, "iter", Vec::new()),
        "position",
        vec![Expr::Closure {
            params: vec![Ident::from("__slate_byte")],
            body: Box::new(Expr::Binary {
                op: BinOp::Eq,
                lhs: Box::new(byte_expr(Expr::Unary {
                    op: UnaryOp::Deref,
                    expr: Box::new(var("__slate_byte")),
                })),
                rhs: Box::new(needle),
            }),
        }],
    )
}

fn byte_expr(expr: Expr) -> Expr {
    cast(expr, Type::Prim(Prim::U8))
}

fn index_to_ptr(source: ByteSource<'_>) -> Expr {
    Expr::Closure {
        params: vec![Ident::from("__slate_index")],
        body: Box::new(unsafe_expr(cast(
            method(source_ptr(source), "add", vec![var("__slate_index")]),
            void_ptr(true),
        ))),
    }
}

fn source_ptr(source: ByteSource<'_>) -> Expr {
    let method_name = if source.mutability == PointerMutability::Mut {
        "as_mut_ptr"
    } else {
        "as_ptr"
    };
    match source.representation {
        ByteRepresentation::Collection | ByteRepresentation::Bytes => {
            method(var(&source.name), method_name, Vec::new())
        }
        ByteRepresentation::CStr | ByteRepresentation::Str => {
            method(var(&source.name), "as_ptr", Vec::new())
        }
    }
}

fn some(expr: Expr) -> Expr {
    call(var("Some"), vec![expr])
}

pub(crate) fn var(name: &str) -> Expr {
    Expr::Var(Ident::from(name))
}

pub(crate) fn path<const N: usize>(parts: [&str; N]) -> Expr {
    Expr::Path(crate::backend::rust_ast::Path::new(parts.map(Ident::from)))
}

pub(crate) fn call(func: Expr, args: Vec<Expr>) -> Expr {
    Expr::Call {
        binding: crate::function_identity::CallBinding::Generated,
        func: Box::new(func),
        args,
    }
}

pub(crate) fn method(recv: Expr, method: &str, args: Vec<Expr>) -> Expr {
    Expr::MethodCall {
        recv: Box::new(recv),
        method: method.into(),
        args,
    }
}

pub(crate) fn cast(expr: Expr, ty: Type) -> Expr {
    Expr::Cast {
        expr: Box::new(expr),
        ty,
    }
}

pub(crate) fn unsafe_expr(value: Expr) -> Expr {
    Expr::Unsafe(Box::new(Block {
        stmts: Vec::new(),
        tail: Some(Box::new(value)),
    }))
}

pub(crate) fn ptr(mutable: bool, inner: Type) -> Type {
    Type::Ptr {
        mutable,
        inner: Box::new(inner),
    }
}

pub(crate) fn void_ptr(mutable: bool) -> Type {
    ptr(mutable, Type::CLib(CLibType::VOID))
}

pub(crate) fn null_mut() -> Expr {
    call(path(["std", "ptr", "null_mut"]), Vec::new())
}

pub(crate) fn indent(stmt: Stmt) -> IndentStmt {
    IndentStmt { depth: 1, stmt }
}

pub(crate) fn let_stmt(name: &str, ty: Option<Type>, init: Expr) -> Stmt {
    Stmt::Let {
        name: name.into(),
        mutable: false,
        ty,
        init: Some(init),
    }
}

#[derive(Clone)]
pub(in crate::backend) struct NullablePointerPlan {
    pub(in crate::backend) container: Vec<PathSegment>,
    pub(in crate::backend) producer_index: usize,
    pub(in crate::backend) producer_name: String,
    pub(in crate::backend) option_name: String,
    pub(in crate::backend) option_expr: Expr,
    pub(in crate::backend) base_ptr: Option<Expr>,
    pub(in crate::backend) aliases: Vec<NullablePointerAlias>,
}

pub(crate) struct FunctionBodyRecipe {
    pub(crate) body: Vec<IndentStmt>,
}

#[derive(Clone)]
pub(in crate::backend) struct NullablePointerAlias {
    pub(in crate::backend) name: String,
    pub(in crate::backend) remove_indices: Vec<usize>,
}

pub(in crate::backend) fn rewrite_nullable_pointer(
    mut body: Vec<IndentStmt>,
    plan: NullablePointerPlan,
) -> FunctionBodyRecipe {
    if let Some(container) = nullable_pointer_container_mut(&mut body, &plan.container) {
        apply_nullable_pointer_plan(container, &plan);
    }
    FunctionBodyRecipe { body }
}

fn nullable_pointer_container_mut<'a>(
    body: &'a mut Vec<IndentStmt>,
    path: &[PathSegment],
) -> Option<&'a mut Vec<IndentStmt>> {
    let [PathSegment::Stmt(index), rest @ ..] = path else {
        return path.is_empty().then_some(body);
    };
    match (&mut body.get_mut(*index)?.stmt, rest) {
        (
            Stmt::If { then_body, .. } | Stmt::LetIf { then_body, .. },
            [PathSegment::Then, rest @ ..],
        ) => nullable_pointer_container_mut(then_body, rest),
        (
            Stmt::If { else_body, .. } | Stmt::LetIf { else_body, .. },
            [PathSegment::Else, rest @ ..],
        ) => nullable_pointer_container_mut(else_body, rest),
        (Stmt::Loop { body, .. }, [PathSegment::LoopBody, rest @ ..]) => {
            nullable_pointer_container_mut(body, rest)
        }
        (Stmt::For { body, .. }, [PathSegment::ForBody, rest @ ..]) => {
            nullable_pointer_container_mut(body, rest)
        }
        (Stmt::Scope { body }, [PathSegment::ScopeBody, rest @ ..]) => {
            nullable_pointer_container_mut(body, rest)
        }
        (Stmt::LabeledBlock { body, .. }, [PathSegment::LabeledBody, rest @ ..]) => {
            nullable_pointer_container_mut(body, rest)
        }
        (Stmt::Unsafe { body }, [PathSegment::UnsafeBody, rest @ ..]) => {
            nullable_pointer_container_mut(&mut body.stmts, rest)
        }
        (Stmt::While { body, .. }, [PathSegment::WhileBody, rest @ ..]) => {
            nullable_pointer_container_mut(&mut body.stmts, rest)
        }
        (Stmt::Block(body), [PathSegment::BlockBody, rest @ ..]) => {
            nullable_pointer_container_mut(&mut body.stmts, rest)
        }
        (Stmt::Match { arms, .. }, [PathSegment::MatchArm(index), rest @ ..]) => {
            nullable_pointer_container_mut(&mut arms.get_mut(*index)?.body, rest)
        }
        _ => None,
    }
}

fn apply_nullable_pointer_plan(body: &mut Vec<IndentStmt>, plan: &NullablePointerPlan) {
    if let Stmt::Let { name, ty, init, .. } = &mut body[plan.producer_index].stmt {
        *name = plan.option_name.as_str().into();
        *ty = None;
        *init = Some(plan.option_expr.clone());
    }
    for stmt in body.iter_mut() {
        rewrite_nullable_pointer_observations(&mut stmt.stmt, plan);
    }
    let mut remove = Vec::new();
    for alias in &plan.aliases {
        remove.extend(alias.remove_indices.iter().copied());
    }
    remove.sort_unstable();
    remove.dedup();
    for index in remove.into_iter().rev() {
        if index != plan.producer_index {
            body.remove(index);
        }
    }
}

fn rewrite_nullable_pointer_observations(stmt: &mut Stmt, plan: &NullablePointerPlan) {
    match stmt {
        Stmt::Let {
            init: Some(expr), ..
        }
        | Stmt::Assign { value: expr, .. }
        | Stmt::Expr(expr)
        | Stmt::Return(Some(expr)) => rewrite_nullable_pointer_expr(expr, plan),
        _ => {}
    }
}

fn rewrite_nullable_pointer_expr(expr: &mut Expr, plan: &NullablePointerPlan) {
    if let Some(replacement) =
        nullable_pointer_observation_replacement(expr, &plan.producer_name, plan)
    {
        *expr = replacement;
        return;
    }
    if let Some(replacement) =
        nullable_pointer_observation_replacement(expr, &plan.option_name, plan)
    {
        *expr = replacement;
        return;
    }
    for alias in &plan.aliases {
        if let Some(replacement) = nullable_pointer_observation_replacement(expr, &alias.name, plan)
        {
            *expr = replacement;
            return;
        }
    }
    match expr {
        Expr::Binary { lhs, rhs, .. } => {
            rewrite_nullable_pointer_expr(lhs, plan);
            rewrite_nullable_pointer_expr(rhs, plan);
        }
        Expr::Cast { expr, .. }
        | Expr::Unary { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. } => rewrite_nullable_pointer_expr(expr, plan),
        Expr::Call { func, args, .. } => {
            rewrite_nullable_pointer_expr(func, plan);
            for arg in args {
                rewrite_nullable_pointer_expr(arg, plan);
            }
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            rewrite_nullable_pointer_expr(recv, plan);
            for arg in args {
                rewrite_nullable_pointer_expr(arg, plan);
            }
        }
        Expr::Block(block) | Expr::Unsafe(block) => {
            if block.stmts.is_empty()
                && let Some(tail) = &mut block.tail
            {
                rewrite_nullable_pointer_expr(tail, plan);
            }
        }
        _ => {}
    }
}

fn nullable_pointer_observation_replacement(
    expr: &Expr,
    alias_name: &str,
    plan: &NullablePointerPlan,
) -> Option<Expr> {
    if let Expr::Binary { op, lhs, rhs } = expr
        && let Some(method_name) = null_comparison(op, lhs, rhs, alias_name)
    {
        return Some(method(var(&plan.option_name), method_name, Vec::new()));
    }
    let base_ptr = plan.base_ptr.as_ref()?;
    let target_ty = find_distance_observation(expr, alias_name, base_ptr)?;
    Some(match target_ty {
        Some(ty) => Expr::Cast {
            expr: Box::new(method(var(&plan.option_name), "unwrap", Vec::new())),
            ty,
        },
        None => method(var(&plan.option_name), "unwrap", Vec::new()),
    })
}

#[derive(Debug, Clone)]
pub(in crate::backend) struct OptionBoxLocalPlan {
    pub(in crate::backend) name: String,
    pub(in crate::backend) elem_ty: Type,
    pub(in crate::backend) decl_path: AstPath,
    pub(in crate::backend) assignments: Vec<(AstPath, OptionBoxAssignKind, Option<AstPath>)>,
    pub(in crate::backend) guard_path: AstPath,
}

pub(in crate::backend) struct OptionBoxComparisonPlan {
    pub(in crate::backend) if_stmt_path: AstPath,
    pub(in crate::backend) lhs: String,
    pub(in crate::backend) rhs: String,
    pub(in crate::backend) negate: bool,
    pub(in crate::backend) elem_ty: Type,
}

pub(in crate::backend) fn rewrite_option_box_locals(
    body: Vec<IndentStmt>,
    plans: Vec<OptionBoxLocalPlan>,
    comparisons: Vec<OptionBoxComparisonPlan>,
) -> FunctionBodyRecipe {
    let mut body = body;
    let mut alloc_sources = Vec::new();
    for plan in &plans {
        rewrite_option_box_decl(&mut body, plan);
        for (path, kind, alloc_source) in &plan.assignments {
            rewrite_option_box_assignment(&mut body, path, *kind, &plan.elem_ty);
            alloc_sources.extend(alloc_source.clone());
        }
        rewrite_option_box_guard(&mut body, plan);
    }
    for comparison in &comparisons {
        rewrite_option_box_comparison(&mut body, comparison);
    }
    alloc_sources.sort_by(|a: &AstPath, b: &AstPath| b.cmp(a));
    for source in &alloc_sources {
        remove_stmt_at_path(&mut body, &source.0);
    }
    FunctionBodyRecipe { body }
}

fn rewrite_option_box_comparison(body: &mut [IndentStmt], plan: &OptionBoxComparisonPlan) {
    let Some(stmt) = with_stmt_at_path(body, &plan.if_stmt_path.0) else {
        return;
    };
    let Stmt::If { cond, .. } = stmt else {
        return;
    };
    *cond = option_box_ptr_eq(&plan.elem_ty, &plan.lhs, &plan.rhs, plan.negate);
}

fn option_box_ptr_eq(elem_ty: &Type, lhs_name: &str, rhs_name: &str, negate: bool) -> Expr {
    let eq_call = call(
        path(["std", "ptr", "eq"]),
        vec![
            option_as_const_ptr(elem_ty, lhs_name),
            option_as_const_ptr(elem_ty, rhs_name),
        ],
    );
    if negate {
        Expr::Unary {
            op: UnaryOp::Not,
            expr: Box::new(eq_call),
        }
    } else {
        eq_call
    }
}

fn option_as_const_ptr(elem_ty: &Type, name: &str) -> Expr {
    method(
        method(var(name), "as_deref", Vec::new()),
        "map_or",
        vec![
            call(path(["std", "ptr", "null"]), Vec::new()),
            Expr::Closure {
                params: vec![Ident::from("__slate_ref")],
                body: Box::new(cast(var("__slate_ref"), ptr(false, elem_ty.clone()))),
            },
        ],
    )
}

fn option_box_type(elem_ty: &Type) -> Type {
    Type::Generic {
        name: "Option".into(),
        args: vec![Type::Generic {
            name: "Box".into(),
            args: vec![elem_ty.clone()],
        }],
    }
}

fn rewrite_option_box_decl(body: &mut [IndentStmt], plan: &OptionBoxLocalPlan) {
    let Some(stmt) = with_stmt_at_path(body, &plan.decl_path.0) else {
        return;
    };
    let Stmt::Let {
        mutable, ty, init, ..
    } = stmt
    else {
        return;
    };
    *mutable = true;
    *ty = Some(option_box_type(&plan.elem_ty));
    *init = Some(Expr::Value(RustValue::None));
}

fn rewrite_option_box_assignment(
    body: &mut [IndentStmt],
    path: &AstPath,
    kind: OptionBoxAssignKind,
    elem_ty: &Type,
) {
    let Some(stmt) = with_stmt_at_path(body, &path.0) else {
        return;
    };
    let Stmt::Assign { value, .. } = stmt else {
        return;
    };
    *value = match kind {
        OptionBoxAssignKind::Null => Expr::Value(RustValue::None),
        OptionBoxAssignKind::Alloc => some(box_new(elem_ty)),
    };
}

fn rewrite_option_box_guard(body: &mut [IndentStmt], plan: &OptionBoxLocalPlan) {
    let Some(stmt) = with_stmt_at_path(body, &plan.guard_path.0) else {
        return;
    };
    let Stmt::If {
        then_body,
        else_body,
        ..
    } = stmt
    else {
        return;
    };
    let mut then_body = std::mem::take(then_body);
    strip_trailing_free(&mut then_body, &plan.name);
    strip_redundant_unsafe(&mut then_body, &plan.name);
    let mut rebind_depth = then_body.first().map_or(1, |indent| indent.depth);
    if rebind_depth == 0 {
        rebind_depth = 1;
    }
    let mut new_then_body = vec![IndentStmt {
        depth: rebind_depth,
        stmt: Stmt::Let {
            name: plan.name.clone(),
            mutable: true,
            ty: None,
            init: Some(Expr::Var(plan.name.clone().into())),
        },
    }];
    new_then_body.extend(then_body);
    let else_body = std::mem::take(else_body);
    *stmt = Stmt::Match {
        expr: method(var(&plan.name), "take", Vec::new()),
        arms: vec![
            MatchArm {
                pattern: Pattern::TupleStruct {
                    name: "Some".into(),
                    fields: vec![Pattern::Binding(plan.name.clone().into())],
                },
                body: new_then_body,
            },
            MatchArm {
                pattern: Pattern::Wildcard,
                body: else_body,
            },
        ],
    };
}

pub(in crate::backend) fn strip_redundant_unsafe(body: &mut Vec<IndentStmt>, name: &str) {
    let mut index = 0;
    while index < body.len() {
        if let Stmt::Unsafe { body: block } = &body[index].stmt
            && block_is_safe_once_converted(block, name)
        {
            let depth = body[index].depth;
            let removed = body.remove(index);
            let Stmt::Unsafe { body: block } = removed.stmt else {
                unreachable!()
            };
            let mut inner = block.stmts;
            if let Some(tail) = block.tail {
                inner.push(IndentStmt {
                    depth,
                    stmt: Stmt::Expr(*tail),
                });
            }
            for item in &mut inner {
                item.depth = depth;
            }
            let inserted = inner.len();
            body.splice(index..index, inner);
            index += inserted;
            continue;
        }
        strip_redundant_unsafe_exprs(&mut body[index].stmt, name);
        walk::nested_body_vecs_mut_with_path(
            &mut body[index].stmt,
            &mut Vec::new(),
            &mut |nested, _| {
                strip_redundant_unsafe(nested, name);
            },
        );
        index += 1;
    }
}

fn strip_redundant_unsafe_exprs(stmt: &mut Stmt, name: &str) {
    walk::stmt_exprs_mut_with(stmt, &mut |expr| {
        if let Expr::Unsafe(block) = expr
            && block_is_safe_once_converted(block, name)
        {
            let mut block = std::mem::replace(
                &mut **block,
                Block {
                    stmts: Vec::new(),
                    tail: None,
                },
            );
            *expr = if block.stmts.is_empty() && block.tail.is_some() {
                *block.tail.take().unwrap()
            } else {
                Expr::Block(Box::new(block))
            };
        }
        true
    });
}

fn block_is_safe_once_converted(block: &Block, name: &str) -> bool {
    let has_unsafe_construct =
        walk::body_expr_any(&block.stmts, &mut |expr| still_needs_unsafe(expr, name))
            || block.tail.as_deref().is_some_and(|tail| {
                walk::expr_any(tail, &mut |expr| still_needs_unsafe(expr, name))
            });
    !has_unsafe_construct
}

fn still_needs_unsafe(expr: &Expr, name: &str) -> bool {
    match expr {
        Expr::Unary {
            op: UnaryOp::Deref,
            expr: inner,
        } => !matches!(peel_wrappers(inner), Expr::Var(n) if n.as_str() == name),
        Expr::Call { .. } | Expr::MethodCall { .. } | Expr::Unsafe(_) => true,
        _ => false,
    }
}

fn strip_trailing_free(body: &mut Vec<IndentStmt>, name: &str) -> bool {
    let Some(last) = body.last() else {
        return false;
    };
    let aliases = local_pointer_aliases(body);
    let is_free = matches!(&last.stmt, Stmt::Expr(expr) if is_free_call(expr, name, &aliases));
    if is_free {
        body.pop();
    }
    is_free
}

fn is_free_call(expr: &Expr, name: &str, aliases: &BTreeMap<String, String>) -> bool {
    let Expr::Call { args, .. } = peel_wrappers(expr) else {
        return false;
    };
    if known_call(peel_wrappers(expr)) != Some(Known::Free) || args.len() != 1 {
        return false;
    }
    matches!(peel_wrappers(&args[0]), Expr::Var(arg_name) if resolve_local_alias(aliases, arg_name.as_str(), 4) == name)
}

fn local_pointer_aliases(body: &[IndentStmt]) -> BTreeMap<String, String> {
    let mut aliases = BTreeMap::new();
    for indent in body {
        if let Stmt::Let {
            name,
            init: Some(init),
            ..
        } = &indent.stmt
            && let Expr::Var(target) = peel_wrappers(init)
        {
            aliases.insert(name.clone(), target.as_str().to_string());
        }
    }
    aliases
}

fn resolve_local_alias<'a>(
    aliases: &'a BTreeMap<String, String>,
    name: &'a str,
    depth: u32,
) -> &'a str {
    if depth == 0 {
        return name;
    }
    match aliases.get(name) {
        Some(next) => resolve_local_alias(aliases, next.as_str(), depth - 1),
        None => name,
    }
}

fn peel_wrappers(expr: &Expr) -> &Expr {
    match expr {
        Expr::Cast { expr, .. } => peel_wrappers(expr),
        Expr::Unsafe(block) | Expr::Block(block) if block.stmts.is_empty() => {
            match block.tail.as_deref() {
                Some(inner) => peel_wrappers(inner),
                None => expr,
            }
        }
        _ => expr,
    }
}

fn with_stmt_at_path<'a>(body: &'a mut [IndentStmt], path: &[PathSegment]) -> Option<&'a mut Stmt> {
    let [PathSegment::Stmt(index), rest @ ..] = path else {
        return None;
    };
    let indent = body.get_mut(*index)?;
    if rest.is_empty() {
        return Some(&mut indent.stmt);
    }
    let (segment, deeper) = rest.split_first()?;
    let nested = nested_body_vec_mut(&mut indent.stmt, segment)?;
    with_stmt_at_path(nested, deeper)
}

fn remove_stmt_at_path(body: &mut Vec<IndentStmt>, path: &[PathSegment]) -> bool {
    let [PathSegment::Stmt(index), rest @ ..] = path else {
        return false;
    };
    if rest.is_empty() {
        if *index < body.len() {
            body.remove(*index);
            return true;
        }
        return false;
    }
    let Some(indent) = body.get_mut(*index) else {
        return false;
    };
    let Some((segment, deeper)) = rest.split_first() else {
        return false;
    };
    let Some(nested) = nested_body_vec_mut(&mut indent.stmt, segment) else {
        return false;
    };
    remove_stmt_at_path(nested, deeper)
}

fn nested_body_vec_mut<'a>(
    stmt: &'a mut Stmt,
    segment: &PathSegment,
) -> Option<&'a mut Vec<IndentStmt>> {
    match (stmt, segment) {
        (Stmt::Scope { body }, PathSegment::ScopeBody) => Some(body),
        (Stmt::If { then_body, .. }, PathSegment::Then) => Some(then_body),
        (Stmt::If { else_body, .. }, PathSegment::Else) => Some(else_body),
        (Stmt::Loop { body, .. }, PathSegment::LoopBody) => Some(body),
        (Stmt::For { body, .. }, PathSegment::ForBody) => Some(body),
        (Stmt::LabeledBlock { body, .. }, PathSegment::LabeledBody) => Some(body),
        (Stmt::Unsafe { body }, PathSegment::UnsafeBody) => Some(&mut body.stmts),
        (Stmt::While { body, .. }, PathSegment::WhileBody) => Some(&mut body.stmts),
        (Stmt::Block(body), PathSegment::BlockBody) => Some(&mut body.stmts),
        _ => None,
    }
}

pub(in crate::backend) struct InterproceduralAllocCalleePlan {
    pub(in crate::backend) elem_ty: Type,
    pub(in crate::backend) kind: HeapOwnershipKind,
    pub(in crate::backend) count: Option<Expr>,
    pub(in crate::backend) return_path: AstPath,
    pub(in crate::backend) alloc_source_path: AstPath,
}

pub(in crate::backend) struct InterproceduralAllocCallerPlan {
    pub(in crate::backend) pointer_name: String,
    pub(in crate::backend) kind: HeapOwnershipKind,
    pub(in crate::backend) elem_ty: Type,
    pub(in crate::backend) decl_path: AstPath,
    pub(in crate::backend) call_temp_path: AstPath,
    pub(in crate::backend) free_path: Option<AstPath>,
}

pub(in crate::backend) fn rewrite_interprocedural_alloc_callee(
    mut fndef: FnDef,
    plan: &InterproceduralAllocCalleePlan,
) -> FnDef {
    fndef.ret = Some(owned_type(plan.kind, &plan.elem_ty));
    if let Some(Stmt::Return(Some(expr))) = with_stmt_at_path(&mut fndef.body, &plan.return_path.0)
    {
        *expr = owned_init_expr(plan.kind, &plan.elem_ty, plan.count.as_ref());
    }
    remove_stmt_at_path(&mut fndef.body, &plan.alloc_source_path.0);
    fndef
}

pub(in crate::backend) fn rewrite_interprocedural_alloc_wrapper(
    mut fndef: FnDef,
    kind: HeapOwnershipKind,
    elem_ty: &Type,
) -> FnDef {
    fndef.ret = Some(owned_type(kind, elem_ty));
    fndef
}

pub(in crate::backend) fn rewrite_interprocedural_alloc_caller(
    body: Vec<IndentStmt>,
    plans: &[InterproceduralAllocCallerPlan],
) -> FunctionBodyRecipe {
    let mut body = body;
    for plan in plans {
        if let Some(stmt) = with_stmt_at_path(&mut body, &plan.decl_path.0) {
            retype_placeholder_decl(stmt, plan.kind, &plan.elem_ty);
        }
        if let Some(stmt) = with_stmt_at_path(&mut body, &plan.call_temp_path.0) {
            retype_call_temp_decl(stmt, plan.kind, &plan.elem_ty);
        }
    }

    let mut owned = OwnedHeap::from_owners(
        plans
            .iter()
            .map(|plan| (plan.pointer_name.clone(), plan.kind)),
    );
    let mut remove = BTreeSet::new();
    for (index, indent) in body.iter_mut().enumerate() {
        if let Some((alias, owner)) = owned_alias(&indent.stmt, &owned) {
            owned.aliases.insert(alias, owner);
            remove.insert(index);
            continue;
        }
        rewrite_owned_stmt(&mut indent.stmt, &owned);
    }
    for plan in plans {
        if let Some(free_path) = &plan.free_path
            && let [PathSegment::Stmt(index)] = free_path.0.as_slice()
        {
            remove.insert(*index);
        }
    }
    for index in remove.into_iter().rev() {
        if index < body.len() {
            body.remove(index);
        }
    }
    FunctionBodyRecipe { body }
}

fn owned_init_expr(kind: HeapOwnershipKind, elem_ty: &Type, count: Option<&Expr>) -> Expr {
    match kind {
        HeapOwnershipKind::ScalarBox => box_new(elem_ty),
        HeapOwnershipKind::VecBuffer => Expr::VecRepeat {
            elem: Box::new(default_value(elem_ty)),
            len: Box::new(usize_expr(
                count.cloned().unwrap_or(Expr::Value(RustValue::Usize(0))),
            )),
        },
    }
}

fn owned_type(kind: HeapOwnershipKind, elem_ty: &Type) -> Type {
    let name = match kind {
        HeapOwnershipKind::ScalarBox => "Box",
        HeapOwnershipKind::VecBuffer => "Vec",
    };
    Type::Generic {
        name: name.into(),
        args: vec![elem_ty.clone()],
    }
}

fn retype_placeholder_decl(stmt: &mut Stmt, kind: HeapOwnershipKind, elem_ty: &Type) {
    let Stmt::Let {
        mutable, ty, init, ..
    } = stmt
    else {
        return;
    };
    *mutable = true;
    *ty = Some(owned_type(kind, elem_ty));
    *init = Some(match kind {
        HeapOwnershipKind::ScalarBox => box_new(elem_ty),
        HeapOwnershipKind::VecBuffer => Expr::VecLit(Vec::new()),
    });
}

fn retype_call_temp_decl(stmt: &mut Stmt, kind: HeapOwnershipKind, elem_ty: &Type) {
    let Stmt::Let { ty, .. } = stmt else {
        return;
    };
    *ty = Some(owned_type(kind, elem_ty));
}
