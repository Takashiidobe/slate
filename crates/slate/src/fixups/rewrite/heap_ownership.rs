use std::collections::{BTreeMap, BTreeSet};

use crate::fixups::facts::{
    AstPath, CallCallee, FixupFacts, FunctionId, HeapOwnershipKind, PathSegment,
};
use crate::fixups::support::walk;
use crate::rust_ast::{
    Block, Expr, ExternDecl, IndentStmt, Item, Prim, Program, RustValue, Stmt, Type, UnaryOp,
};

pub(in crate::fixups) fn fixup(program: &mut Program, facts: &FixupFacts) {
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
        rewrite_body(&mut f.body, function_plans);
    }
}

pub(in crate::fixups) fn prune_unused_externs(program: &mut Program, facts: &FixupFacts) {
    let used = direct_calls(facts);
    program.items.retain_mut(|item| match item {
        Item::ExternBlock { decls, .. } => {
            decls.retain(|decl| match decl {
                ExternDecl::Fn(f) if matches!(f.name.as_str(), "malloc" | "free") => {
                    used.contains(&f.name)
                }
                _ => true,
            });
            !decls.is_empty()
        }
        _ => true,
    });
}

fn plans_by_function(facts: &FixupFacts) -> BTreeMap<FunctionId, Vec<Plan>> {
    let mut by_function = BTreeMap::new();
    for fact in &facts.heap_ownership {
        if fact.kind != HeapOwnershipKind::ScalarBox {
            continue;
        }
        let Some(pointer_name) = facts.binding_name(fact.pointer) else {
            continue;
        };
        by_function
            .entry(fact.function)
            .or_insert_with(Vec::new)
            .push(Plan {
                pointer_name: pointer_name.to_string(),
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
                elem_ty: fact.elem_ty.clone(),
            });
    }
    by_function
}

#[derive(Clone)]
struct Plan {
    pointer_name: String,
    pointer_stmt: Option<usize>,
    size_stmt: Option<usize>,
    allocation_stmt: Option<usize>,
    assign_stmt: Option<usize>,
    free_temp_stmt: Option<usize>,
    free_stmt: Option<usize>,
    elem_ty: Type,
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
    }

    let owned: BTreeSet<_> = plans
        .iter()
        .map(|plan| plan.pointer_name.as_str())
        .collect();
    for (index, indent) in body.iter_mut().enumerate() {
        if remove.contains(&index) {
            continue;
        }
        rewrite_owned_stmt(&mut indent.stmt, &owned);
    }
    for index in remove.into_iter().rev() {
        if index < body.len() {
            body.remove(index);
        }
    }
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
    *ty = Some(Type::Generic {
        name: "Box".into(),
        args: vec![plan.elem_ty.clone()],
    });
    *init = Some(box_new(&plan.elem_ty));
}

fn rewrite_owned_stmt(stmt: &mut Stmt, owned: &BTreeSet<&str>) {
    match stmt {
        Stmt::Unsafe { body }
            if body.tail.is_none()
                && body.stmts.len() == 1
                && stmt_can_leave_unsafe(&body.stmts[0].stmt, owned) =>
        {
            let mut replacement = body.stmts[0].stmt.clone();
            rewrite_owned_stmt(&mut replacement, owned);
            *stmt = replacement;
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
        Stmt::Match { expr, arms } => {
            rewrite_owned_expr(expr, owned);
            for arm in arms {
                rewrite_owned_body(&mut arm.body, owned);
            }
        }
        Stmt::Let { init: None, .. } | Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
    }
}

fn rewrite_owned_body(body: &mut [IndentStmt], owned: &BTreeSet<&str>) {
    for indent in body {
        rewrite_owned_stmt(&mut indent.stmt, owned);
    }
}

fn rewrite_owned_block(block: &mut Block, owned: &BTreeSet<&str>) {
    rewrite_owned_body(&mut block.stmts, owned);
    if let Some(tail) = &mut block.tail {
        rewrite_owned_expr(tail, owned);
    }
}

fn rewrite_owned_expr(expr: &mut Expr, owned: &BTreeSet<&str>) {
    if let Some(replacement) = owned_unsafe_tail(expr, owned) {
        *expr = replacement;
        return;
    }
    walk::exprs_mut_with(expr, &mut |expr| {
        if let Some(replacement) = owned_unsafe_tail(expr, owned) {
            *expr = replacement;
            return false;
        }
        true
    });
}

fn stmt_can_leave_unsafe(stmt: &Stmt, owned: &BTreeSet<&str>) -> bool {
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

fn expr_can_leave_unsafe(expr: &Expr, owned: &BTreeSet<&str>) -> bool {
    let mut ok = true;
    crate::fixups::facts::walk::exprs(expr, &mut |expr| {
        if matches!(expr, Expr::Unsafe(_)) && owned_unsafe_tail(expr, owned).is_none() {
            ok = false;
        }
    });
    ok
}

fn owned_unsafe_tail(expr: &Expr, owned: &BTreeSet<&str>) -> Option<Expr> {
    let Expr::Unsafe(block) = expr else {
        return None;
    };
    if !block.stmts.is_empty() {
        return None;
    }
    let tail = block.tail.as_deref()?;
    if owned_deref(tail, owned) {
        Some(tail.clone())
    } else {
        None
    }
}

fn owned_deref(expr: &Expr, owned: &BTreeSet<&str>) -> bool {
    matches!(
        expr,
        Expr::Unary {
            op: UnaryOp::Deref,
            expr
        } if matches!(&**expr, Expr::Var(name) if owned.contains(name.as_str()))
    )
}

fn box_new(ty: &Type) -> Expr {
    Expr::Call {
        func: Box::new(Expr::Var(format!("Box::<{}>::new", ty.render()).into())),
        args: vec![default_value(ty)],
    }
}

fn default_value(ty: &Type) -> Expr {
    match ty {
        Type::Prim(Prim::Bool) => Expr::Value(RustValue::Bool(false)),
        Type::Prim(Prim::F32 | Prim::F64) => Expr::Value(RustValue::Float(0.0)),
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

fn direct_calls(facts: &FixupFacts) -> BTreeSet<String> {
    facts
        .callsites
        .iter()
        .filter_map(|callsite| match &callsite.callee {
            CallCallee::Direct { name, .. } => Some(name.clone()),
            CallCallee::Indirect => None,
        })
        .collect()
}
