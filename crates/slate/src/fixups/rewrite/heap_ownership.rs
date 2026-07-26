use std::collections::{BTreeMap, BTreeSet};

use crate::fixups::facts::{
    AstPath, FixupFacts, FunctionId, HeapExtent, HeapOwnershipFact, HeapOwnershipKind,
    HeapReadSafety, HeapResizeKind, PathSegment,
};
use crate::fixups::idents::{expr_ident_count, stmt_ident_count};
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, fact, function_path_location, stmts_snippet,
};
use crate::function_identity::{Known, known_call};
use crate::rust_ast::{
    Block, Expr, IndentStmt, Item, Prim, Program, RustValue, Stmt, Type, UnaryOp,
};

pub(in crate::fixups) fn fixup(program: &mut Program, facts: &FixupFacts) {
    let mut logger = crate::fixups::trace::NoopLogger;
    HeapOwnership::new(&mut logger).fixup(program, facts);
}

pub(in crate::fixups) struct HeapOwnership<'a> {
    logger: &'a mut dyn TraceLogger,
}

impl<'a> HeapOwnership<'a> {
    pub(in crate::fixups) fn new(logger: &'a mut dyn TraceLogger) -> Self {
        Self { logger }
    }

    pub(in crate::fixups) fn fixup(&mut self, program: &mut Program, facts: &FixupFacts) {
        fixup_impl(program, facts, self.logger);
    }
}

fn fixup_impl(program: &mut Program, facts: &FixupFacts, logger: &mut dyn TraceLogger) {
    let plans = plans_by_function(facts);
    if plans.is_empty() {
        return;
    }
    for (item_index, item) in program.items.iter_mut().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        let Some(function_plans) = plans.get(&function) else {
            continue;
        };
        let before = logger.is_enabled().then(|| f.body.clone());
        rewrite_body(&mut f.body, function_plans);
        if let Some(before) = before {
            logger.rewrite(RewriteEvent {
                pass: TracePass::HeapOwnership,
                kind: "rewrite_heap_ownership".into(),
                location: function_path_location(facts, function, &[]),
                before: vec![stmts_snippet("body", &before)],
                after: vec![stmts_snippet("body", &f.body)],
                facts: vec![fact("plans", function_plans.len().to_string())],
            });
        }
    }
}

fn plans_by_function(facts: &FixupFacts) -> BTreeMap<FunctionId, Vec<Plan>> {
    let mut by_function = BTreeMap::new();
    for fact in &facts.heap_ownership {
        let kind = fact.kind;
        if kind == HeapOwnershipKind::VecBuffer
            && fact.read_safety == HeapReadSafety::MayReadUninitialized
        {
            continue;
        }
        let Some(pointer_name) = facts.binding_name(fact.pointer) else {
            continue;
        };
        let Some(init) = init_for_fact(fact) else {
            continue;
        };
        let count = count_for_extent(&fact.extent);
        if kind == HeapOwnershipKind::VecBuffer && count.is_none() {
            continue;
        }
        let reallocs = fact
            .reallocations
            .iter()
            .map(|realloc| {
                Some(ReallocPlan {
                    source_temp_stmt: previous_stmt_index(&realloc.allocation_path)
                        .and_then(|index| index.checked_sub(1)),
                    size_stmt: previous_stmt_index(&realloc.allocation_path),
                    allocation_stmt: stmt_index(&realloc.allocation_path),
                    assign_stmt: stmt_index(&realloc.assign_path),
                    resize: realloc.resize,
                    count: count_for_extent(&realloc.new_extent)?,
                })
            })
            .collect::<Option<Vec<_>>>();
        let Some(reallocs) = reallocs else {
            continue;
        };
        by_function
            .entry(fact.function)
            .or_insert_with(Vec::new)
            .push(Plan {
                pointer_name: pointer_name.to_string(),
                kind,
                pointer_stmt: stmt_index(&fact.pointer_path),
                size_stmt: fact
                    .size_temp
                    .and_then(|_| previous_stmt_index(&fact.allocation_path)),
                allocation_stmt: stmt_index(&fact.allocation_path),
                assign_stmt: stmt_index(&fact.assign_path),
                free_temp_stmt: fact
                    .free_temp
                    .and_then(|_| previous_stmt_index(&fact.free_path)),
                free_stmt: stmt_index(&fact.free_path),
                reallocs,
                elem_ty: fact.elem_ty.clone(),
                init,
                count,
            });
    }
    by_function
}

#[derive(Clone)]
struct Plan {
    pointer_name: String,
    kind: HeapOwnershipKind,
    pointer_stmt: Option<usize>,
    size_stmt: Option<usize>,
    allocation_stmt: Option<usize>,
    assign_stmt: Option<usize>,
    free_temp_stmt: Option<usize>,
    free_stmt: Option<usize>,
    reallocs: Vec<ReallocPlan>,
    elem_ty: Type,
    init: Expr,
    count: Option<Expr>,
}

#[derive(Clone)]
struct ReallocPlan {
    source_temp_stmt: Option<usize>,
    size_stmt: Option<usize>,
    allocation_stmt: Option<usize>,
    assign_stmt: Option<usize>,
    resize: HeapResizeKind,
    count: Expr,
}

#[derive(Clone)]
struct OwnedPlan {
    kind: HeapOwnershipKind,
}

struct OwnedHeap {
    plans: BTreeMap<String, OwnedPlan>,
    aliases: BTreeMap<String, String>,
}

impl OwnedHeap {
    fn new(plans: &[Plan]) -> Self {
        Self {
            plans: plans
                .iter()
                .map(|plan| (plan.pointer_name.clone(), OwnedPlan { kind: plan.kind }))
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

fn rewrite_body(body: &mut Vec<IndentStmt>, plans: &[Plan]) {
    let mut remove = BTreeSet::new();
    for plan in plans {
        let Some(pointer_stmt) = plan.pointer_stmt else {
            continue;
        };
        if let Some(indent) = body.get_mut(pointer_stmt) {
            rewrite_pointer_decl(&mut indent.stmt, plan);
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
        rewrite_owned_stmt(&mut indent.stmt, &owned);
    }
    fuse_scalar_box_initializers(body, plans, &mut remove);
    for index in remove.into_iter().rev() {
        if index < body.len() {
            body.remove(index);
        }
    }
}

fn fuse_scalar_box_initializers(
    body: &mut [IndentStmt],
    plans: &[Plan],
    remove: &mut BTreeSet<usize>,
) {
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
        }
    }
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

fn rewrite_pointer_decl(stmt: &mut Stmt, plan: &Plan) {
    let Stmt::Let {
        name,
        mutable,
        ty,
        init,
    } = stmt
    else {
        return;
    };
    if name != &plan.pointer_name {
        return;
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

fn realloc_stmt(pointer_name: &str, realloc: &ReallocPlan, init: &Expr) -> Stmt {
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

fn rewrite_owned_stmt(stmt: &mut Stmt, owned: &OwnedHeap) {
    match stmt {
        Stmt::Unsafe { body } if body.tail.is_none() && body.stmts.len() == 1 => {
            let mut replacement = body.stmts[0].stmt.clone();
            rewrite_owned_stmt(&mut replacement, owned);
            if stmt_can_leave_unsafe(&replacement, owned) {
                *stmt = replacement;
            } else if let Stmt::Unsafe { body } = stmt {
                body.stmts[0].stmt = replacement;
            }
        }
        Stmt::Unsafe { body } => rewrite_owned_block(body, owned),
        Stmt::Block(body) | Stmt::While { body, .. } => rewrite_owned_block(body, owned),
        Stmt::Let {
            init: Some(init), ..
        } => rewrite_owned_expr(init, owned),
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            rewrite_owned_expr(cond, owned);
            rewrite_owned_body(then_body, owned);
            rewrite_owned_expr(then_value, owned);
            rewrite_owned_body(else_body, owned);
            rewrite_owned_expr(else_value, owned);
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            rewrite_owned_expr(target, owned);
            rewrite_owned_expr(value, owned);
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => rewrite_owned_expr(expr, owned),
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            rewrite_owned_expr(cond, owned);
            rewrite_owned_body(then_body, owned);
            rewrite_owned_body(else_body, owned);
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            rewrite_owned_body(body, owned);
        }
        Stmt::For { iter, body, .. } => {
            rewrite_owned_expr(iter, owned);
            rewrite_owned_body(body, owned);
        }
        Stmt::Match { expr, arms } => {
            rewrite_owned_expr(expr, owned);
            for arm in arms {
                rewrite_owned_body(&mut arm.body, owned);
            }
        }
        Stmt::Let { init: None, .. }
        | Stmt::InlineAsm(_)
        | Stmt::Return(None)
        | Stmt::Break(_)
        | Stmt::Continue(_) => {}
    }
}

fn rewrite_owned_body(body: &mut [IndentStmt], owned: &OwnedHeap) {
    for indent in body {
        rewrite_owned_stmt(&mut indent.stmt, owned);
    }
}

fn rewrite_owned_block(block: &mut Block, owned: &OwnedHeap) {
    rewrite_owned_body(&mut block.stmts, owned);
    if let Some(tail) = &mut block.tail {
        rewrite_owned_expr(tail, owned);
    }
}

fn rewrite_owned_expr(expr: &mut Expr, owned: &OwnedHeap) {
    if let Some(replacement) = owned_heap_access(expr, owned) {
        *expr = replacement;
        return;
    }
    walk::exprs_mut_with(expr, &mut |expr| {
        if let Some(replacement) = owned_heap_access(expr, owned) {
            *expr = replacement;
            return false;
        }
        true
    });
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
    crate::fixups::facts::walk::exprs(expr, &mut |expr| {
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

fn init_for_fact(fact: &HeapOwnershipFact) -> Option<Expr> {
    match fact.kind {
        HeapOwnershipKind::ScalarBox => Some(default_value(&fact.elem_ty)),
        HeapOwnershipKind::VecBuffer => match fact.read_safety {
            HeapReadSafety::ZeroInitialized | HeapReadSafety::ReadsAfterWrites => {
                Some(default_value(&fact.elem_ty))
            }
            HeapReadSafety::MayReadUninitialized => None,
        },
    }
}

fn count_for_extent(extent: &HeapExtent) -> Option<Expr> {
    match extent {
        HeapExtent::Scalar => Some(Expr::Value(RustValue::I64(1))),
        HeapExtent::Elements { count } => Some(count.clone()),
        HeapExtent::Unknown => None,
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

fn default_value(ty: &Type) -> Expr {
    match ty {
        Type::Prim(Prim::Bool) => Expr::Value(RustValue::Bool(false)),
        Type::Prim(Prim::F32 | Prim::F64) => Expr::Value(RustValue::Float(0.0)),
        Type::Prim(Prim::F128) => Expr::HexFloat("0.0f128".into()),
        Type::Ptr { .. } => Expr::Value(RustValue::NullPtr),
        _ => Expr::Value(RustValue::I64(0)),
    }
}

fn stmt_index(path: &AstPath) -> Option<usize> {
    match path.0.as_slice() {
        [PathSegment::Stmt(index)] => Some(*index),
        _ => None,
    }
}

fn previous_stmt_index(path: &AstPath) -> Option<usize> {
    stmt_index(path).and_then(|index| index.checked_sub(1))
}
