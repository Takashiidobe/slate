use crate::fixups::support::walk;
use crate::fixups::trace::Pass;
use crate::rust_ast::{
    Attr, Block, Expr, FnDef, ImplItem, IndentStmt, Item, Program, Stmt, Type, UnaryOp,
};

use super::super::{
    Predicate, ProgramRule, Rejection, RejectionReason, UnusedParamPlan, rewrite_unused_param,
};

pub(in crate::fixups) fn program() -> ProgramRule {
    ProgramRule::unused_params(Pass::UnusedParams, "remove_unused_param").case(
        "unreachable_param",
        |case| {
            let program = case.snapshot_program();
            for param in case.all_params() {
                if !trivially_droppable(&param.param_ty) {
                    continue;
                }
                let usage = case.param_usage(&param);
                if usage.reads != 0 || usage.writes != 0 {
                    continue;
                }
                let Some(Item::Fn(f)) = program.items.get(param.function_item_index) else {
                    continue;
                };
                if !eligible_signature(f)
                    || program_has_unsafe_ref(program, &param.function_name)
                    || any_undroppable_call(
                        program,
                        &param.function_name,
                        param.param_index,
                        f.params.len(),
                    )
                {
                    continue;
                }
                let plan = UnusedParamPlan {
                    function_item_index: param.function_item_index,
                    function_name: param.function_name.clone(),
                    param_index: param.param_index,
                    param_name: param.param_name.clone(),
                    param_ty: param.param_ty.render(),
                };
                case.note_unused_param(&plan);
                return Ok(rewrite_unused_param(plan));
            }
            Err(Rejection::new(
                Predicate::UnusedParam,
                None,
                RejectionReason::MissingEvidence,
                Vec::new(),
            ))
        },
    )
}

fn eligible_signature(f: &FnDef) -> bool {
    f.name != "main" && f.abi.is_none() && !is_exported(&f.attrs)
}

fn is_exported(attrs: &[Attr]) -> bool {
    attrs
        .iter()
        .any(|attr| matches!(attr, Attr::Used(_) | Attr::NoMangle | Attr::WeakLinkage))
}

fn trivially_droppable(ty: &Type) -> bool {
    match ty {
        Type::Prim(_) | Type::Unit | Type::Ptr { .. } | Type::Ref { .. } => true,
        Type::Array { elem, .. } => trivially_droppable(elem),
        Type::Custom(_)
        | Type::LongDouble
        | Type::TyVar(_)
        | Type::CLib(_)
        | Type::Complex(_)
        | Type::Generic { .. }
        | Type::VaList
        | Type::Str
        | Type::Slice(_)
        | Type::FnPtr { .. }
        | Type::Variadic
        | Type::Never => false,
    }
}

fn is_pure_expr(expr: &Expr) -> bool {
    match expr {
        Expr::Value(_) | Expr::Str(_) | Expr::ByteStr(_) | Expr::CStr(_) | Expr::HexFloat(_) => {
            true
        }
        Expr::Var(_) | Expr::Path(_) => true,
        Expr::Unary { op, expr } => matches!(op, UnaryOp::Neg | UnaryOp::Not) && is_pure_expr(expr),
        Expr::Binary { lhs, rhs, .. }
        | Expr::Range {
            start: lhs,
            end: rhs,
        } => is_pure_expr(lhs) && is_pure_expr(rhs),
        Expr::Cast { expr, .. } => is_pure_expr(expr),
        Expr::Field { base, .. } | Expr::TupleField { base, .. } => is_pure_expr(base),
        Expr::Index { base, index } => is_pure_expr(base) && is_pure_expr(index),
        Expr::StructLit { fields, .. } => fields.iter().all(|(_, value)| is_pure_expr(value)),
        Expr::TupleStructLit { fields, .. } => fields.iter().all(is_pure_expr),
        Expr::ArrayLit(values) | Expr::VecLit(values) => values.iter().all(is_pure_expr),
        Expr::ArrayRepeat { elem, .. } => is_pure_expr(elem),
        _ => false,
    }
}

fn is_named_callee(expr: &Expr, name: &str) -> bool {
    match expr {
        Expr::Var(n) => n.as_str() == name,
        Expr::Path(path) => path.segments.len() == 1 && path.segments[0].as_str() == name,
        _ => false,
    }
}

fn expr_has_unsafe_ref(expr: &Expr, name: &str) -> bool {
    match expr {
        Expr::Var(_) | Expr::Path(_) => is_named_callee(expr, name),
        Expr::Call { func, args, .. } => {
            let func_bad = if is_named_callee(func, name) {
                false
            } else {
                expr_has_unsafe_ref(func, name)
            };
            func_bad || args.iter().any(|arg| expr_has_unsafe_ref(arg, name))
        }
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. } => expr_has_unsafe_ref(expr, name),
        Expr::Binary { lhs, rhs, .. }
        | Expr::Range {
            start: lhs,
            end: rhs,
        }
        | Expr::Index {
            base: lhs,
            index: rhs,
        } => expr_has_unsafe_ref(lhs, name) || expr_has_unsafe_ref(rhs, name),
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            expr_has_unsafe_ref(recv, name) || args.iter().any(|arg| expr_has_unsafe_ref(arg, name))
        }
        Expr::Field { base, .. }
        | Expr::TupleField { base, .. }
        | Expr::ArrayPtr { array: base, .. } => expr_has_unsafe_ref(base, name),
        Expr::StructLit { fields, .. } => fields
            .iter()
            .any(|(_, value)| expr_has_unsafe_ref(value, name)),
        Expr::TupleStructLit { fields, .. } => {
            fields.iter().any(|value| expr_has_unsafe_ref(value, name))
        }
        Expr::ArrayLit(elems) | Expr::VecLit(elems) => {
            elems.iter().any(|elem| expr_has_unsafe_ref(elem, name))
        }
        Expr::ArrayRepeat { elem, .. } => expr_has_unsafe_ref(elem, name),
        Expr::VecRepeat { elem, len } => {
            expr_has_unsafe_ref(elem, name) || expr_has_unsafe_ref(len, name)
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            expr_has_unsafe_ref(cond, name)
                || expr_has_unsafe_ref(then_expr, name)
                || expr_has_unsafe_ref(else_expr, name)
        }
        Expr::Match { expr, arms } => {
            expr_has_unsafe_ref(expr, name)
                || arms.iter().any(|arm| expr_has_unsafe_ref(&arm.value, name))
        }
        Expr::Block(block) | Expr::Unsafe(block) => block_has_unsafe_ref(block, name),
        Expr::Closure { body, .. } => expr_has_unsafe_ref(body, name),
        Expr::Macro { args, .. } => args.iter().any(|arg| expr_has_unsafe_ref(arg, name)),
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::CStr(_)
        | Expr::Todo(_) => false,
        other => walk::expr_any(other, &mut |e| is_named_callee(e, name)),
    }
}

fn block_has_unsafe_ref(block: &Block, name: &str) -> bool {
    body_has_unsafe_ref(&block.stmts, name)
        || block
            .tail
            .as_deref()
            .is_some_and(|tail| expr_has_unsafe_ref(tail, name))
}

fn body_has_unsafe_ref(body: &[IndentStmt], name: &str) -> bool {
    body.iter()
        .any(|stmt| stmt_has_unsafe_ref(&stmt.stmt, name))
}

fn stmt_has_unsafe_ref(stmt: &Stmt, name: &str) -> bool {
    match stmt {
        Stmt::Let { init, .. } => init
            .as_ref()
            .is_some_and(|expr| expr_has_unsafe_ref(expr, name)),
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            expr_has_unsafe_ref(cond, name)
                || body_has_unsafe_ref(then_body, name)
                || expr_has_unsafe_ref(then_value, name)
                || body_has_unsafe_ref(else_body, name)
                || expr_has_unsafe_ref(else_value, name)
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            expr_has_unsafe_ref(target, name) || expr_has_unsafe_ref(value, name)
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => expr_has_unsafe_ref(expr, name),
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => false,
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            expr_has_unsafe_ref(cond, name)
                || body_has_unsafe_ref(then_body, name)
                || body_has_unsafe_ref(else_body, name)
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            body_has_unsafe_ref(body, name)
        }
        Stmt::For { iter, body, .. } => {
            expr_has_unsafe_ref(iter, name) || body_has_unsafe_ref(body, name)
        }
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
            block_has_unsafe_ref(body, name)
        }
        Stmt::Match { expr, arms } => {
            expr_has_unsafe_ref(expr, name)
                || arms.iter().any(|arm| body_has_unsafe_ref(&arm.body, name))
        }
        Stmt::InlineAsm(asm) => asm.operands.iter().any(|operand| {
            let mut found = false;
            operand.visit_exprs(&mut |expr| found |= expr_has_unsafe_ref(expr, name));
            found
        }),
    }
}

fn program_has_unsafe_ref(program: &Program, name: &str) -> bool {
    let mut found = false;
    each_item(&program.items, &mut |item| {
        if found {
            return;
        }
        found = item_has_unsafe_ref(item, name);
    });
    found
}

fn item_has_unsafe_ref(item: &Item, name: &str) -> bool {
    match item {
        Item::Fn(f) => body_has_unsafe_ref(&f.body, name),
        Item::Static { init, .. } => expr_has_unsafe_ref(init, name),
        Item::Impl(block) => block.items.iter().any(|it| match it {
            ImplItem::Method(m) => expr_has_unsafe_ref(&m.body, name),
            ImplItem::AssocType { .. } => false,
        }),
        _ => false,
    }
}

fn any_undroppable_call(
    program: &Program,
    name: &str,
    param_index: usize,
    expected_len: usize,
) -> bool {
    let mut found = false;
    each_item(&program.items, &mut |item| {
        if found {
            return;
        }
        let pred = &mut |e: &Expr| is_undroppable_call(e, name, param_index, expected_len);
        found = match item {
            Item::Fn(f) => walk::body_expr_any(&f.body, pred),
            Item::Static { init, .. } => walk::expr_any(init, pred),
            Item::Impl(block) => block.items.iter().any(|it| match it {
                ImplItem::Method(m) => walk::expr_any(&m.body, pred),
                ImplItem::AssocType { .. } => false,
            }),
            _ => false,
        };
    });
    found
}

fn is_undroppable_call(expr: &Expr, name: &str, param_index: usize, expected_len: usize) -> bool {
    matches!(expr, Expr::Call { func, args, .. }
        if is_named_callee(func, name)
            && (args.len() != expected_len || !is_pure_expr(&args[param_index])))
}

fn each_item<'a>(items: &'a [Item], f: &mut impl FnMut(&'a Item)) {
    for item in items {
        match item {
            Item::Cfg { item, .. } => each_item(std::slice::from_ref(item.as_ref()), f),
            other => f(other),
        }
    }
}
