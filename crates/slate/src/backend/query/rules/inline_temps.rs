use crate::backend::facts::{EffectKind, Purity};
use crate::backend::idents::{expr_ident, expr_ident_count};
use crate::backend::rust_ast::{Expr, Prim, RustValue, Stmt, Type, UnaryOp};
use crate::backend::support::walk;
use crate::backend::trace::Pass;

use super::super::item::StatementRef;
use super::super::{
    Binding, BindingAccess, BindingCategory, BindingRef, EditSet, ExpressionEffects, Field,
    ItemCaseContext, Phase, Predicate, QueryRule, Rejection,
};

fn matcher() -> Binding {
    Binding {
        kind: Field::eq(BindingCategory::Local),
        name: Field::predicate(|name: &String, _| is_temp_name(name)),
        ..Default::default()
    }
}

pub(in crate::backend) fn early() -> QueryRule<Binding> {
    QueryRule::new(Pass::EarlyInlineTemps, "inline_temp", matcher())
        .case("early", early_case)
        .ordered_non_overlapping()
}

pub(in crate::backend) fn late() -> QueryRule<Binding> {
    QueryRule::new(Pass::LateInlineTemps, "inline_temp", matcher())
        .case("late", late_case)
        .ordered_non_overlapping()
}

fn early_case<'db>(
    case: &mut ItemCaseContext<'_, '_>,
    binding: &BindingRef<'db>,
) -> Result<EditSet, Rejection> {
    apply(case, binding, Phase::Early)
}

fn late_case<'db>(
    case: &mut ItemCaseContext<'_, '_>,
    binding: &BindingRef<'db>,
) -> Result<EditSet, Rejection> {
    apply(case, binding, Phase::Late)
}

fn apply<'db>(
    case: &mut ItemCaseContext<'_, '_>,
    binding: &BindingRef<'db>,
    phase: Phase,
) -> Result<EditSet, Rejection> {
    let initializer = case.fact(|query| query.binding_initializer(binding))?;
    let uses = case.fact(|query| query.binding_uses(binding))?;
    let reads = uses
        .uses
        .iter()
        .filter(|usage| usage.access == BindingAccess::Read)
        .collect::<Vec<_>>();
    case.require_at(
        reads.len() == 1 && uses.uses.len() == 1,
        Predicate::BindingUses,
        &initializer.site,
    )?;
    let use_expression = reads[0]
        .expression()
        .cloned()
        .ok_or_else(|| case.reject())?;
    let _ = case.fact(|query| query.enclosing_statement(&use_expression))?;

    let producer = StatementRef {
        item_index: binding.item_index,
        path: binding.definition.clone(),
    };
    let producer_statement = case.fact(|query| query.statement(&producer))?;
    case.require(matches!(
        producer_statement.stmt,
        Stmt::Let { mutable: false, .. }
    ))?;
    let producer_container = producer.container().ok_or_else(|| case.reject())?;
    let consumer =
        case.fact(|query| query.statement_in_container(&producer_container, &use_expression))?;
    let producer_index = producer.index().ok_or_else(|| case.reject())?;
    let consumer_index = consumer.index().ok_or_else(|| case.reject())?;
    case.require(consumer_index > producer_index)?;
    let adjacent = consumer_index == producer_index + 1;

    let producer_effects = case.fact(|query| query.expression_effects(&initializer))?;
    let consumer_stmt = case.fact(|query| query.statement(&consumer))?.stmt.clone();
    let consumer_effects = case.fact(|query| query.observed_statement_effects(&consumer))?;
    let binding_type = if binding.ty.is_some() {
        Some(case.fact(|query| query.binding_type(binding))?)
    } else {
        None
    };
    let init = case
        .expr(&initializer.site)
        .cloned()
        .ok_or_else(|| case.reject())?;

    if producer_effects.purity == Purity::MovablePure {
        for statement in case.fact(|query| query.statements_between(&producer, &consumer))? {
            let indent = case.fact(|query| query.statement(&statement))?;
            let Stmt::Let {
                name,
                mutable: false,
                init: Some(_),
                ..
            } = &indent.stmt
            else {
                return Err(case.reject());
            };
            case.require(is_temp_name(name))?;
            let intermediate = case.fact(|query| query.statement_binding(&statement))?;
            let initializer = case.fact(|query| query.binding_initializer(&intermediate))?;
            let effects = case.fact(|query| query.expression_effects(&initializer))?;
            case.require(effects.purity == Purity::MovablePure)?;
        }
    } else {
        case.require(
            adjacent
                && ((producer_effects.purity == Purity::Effectful
                    && immediate_effectful_consumer(&consumer_stmt, &binding.name))
                    || (is_atomic_result(&init, &producer_effects)
                        && immediate_atomic_result_consumer(&consumer_stmt, &binding.name))),
        )?;
    }

    if phase == Phase::Early && is_effectful(&producer_effects) {
        case.require(early_effectful_consumer(&consumer_stmt, &binding.name))?;
    }
    let allowed_receiver =
        is_option_receiver_use(&consumer_stmt, &binding.name, binding_type.as_ref(), &init);
    let allowed_argument = is_allowed_argument_use(
        &consumer_stmt,
        &binding.name,
        binding_type.as_ref(),
        &init,
        &producer_effects,
        adjacent,
        phase,
    );
    if consumer_effects.as_ref().is_some_and(has_call) {
        case.require(allowed_receiver || allowed_argument)?;
    }
    if is_receiver_use(&consumer_stmt, &binding.name) {
        case.require(allowed_receiver)?;
    }
    if let Some(root) = root_var(&init) {
        case.require(!macro_arg_alias_conflict(&consumer_stmt, root))?;
    }

    let mut edits = EditSet::new();
    edits.push_replace_expression(use_expression.site, init);
    edits.push_replace_statement(binding.item_index, binding.definition.clone(), None);
    Ok(edits)
}

fn root_var(expr: &Expr) -> Option<&str> {
    match expr {
        Expr::Var(name) => Some(name.as_str()),
        Expr::Field { base, .. } | Expr::TupleField { base, .. } | Expr::Index { base, .. } => {
            root_var(base)
        }
        Expr::Unary {
            op: UnaryOp::Deref,
            expr,
        }
        | Expr::Cast { expr, .. } => root_var(expr),
        Expr::Block(block) | Expr::Unsafe(block) if block.stmts.is_empty() => {
            block.tail.as_deref().and_then(root_var)
        }
        _ => None,
    }
}

fn macro_arg_alias_conflict(stmt: &Stmt, root: &str) -> bool {
    walk::stmt_expr_any(stmt, &mut |expr| {
        let Expr::Macro { args, .. } = expr else {
            return false;
        };
        args.iter().any(|arg| {
            walk::expr_any(arg, &mut |sub| {
                matches!(
                    sub,
                    Expr::AddrOf { mutable: true, expr }
                        if root_var(expr) == Some(root)
                )
            })
        })
    })
}

fn is_atomic_result(expr: &Expr, effects: &ExpressionEffects) -> bool {
    matches!(
        expr,
        Expr::AtomicLoad { .. }
            | Expr::AtomicFetch { .. }
            | Expr::AtomicSwap { .. }
            | Expr::AtomicCompareExchange { .. }
    ) && (effects.effects.contains(&EffectKind::AtomicRead)
        || effects.effects.contains(&EffectKind::AtomicWrite))
}

fn immediate_effectful_consumer(stmt: &Stmt, name: &str) -> bool {
    match stmt {
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            expr_ident(target).is_some() && expr_ident(value) == Some(name)
        }
        Stmt::Return(Some(expr)) => expr_ident(expr) == Some(name),
        Stmt::Unsafe { body } => {
            body.tail.is_none()
                && body.stmts.len() == 1
                && immediate_effectful_consumer(&body.stmts[0].stmt, name)
        }
        Stmt::Expr(expr) => simple_macro_arg_use_expr(expr, name),
        _ => false,
    }
}

fn immediate_atomic_result_consumer(stmt: &Stmt, name: &str) -> bool {
    match stmt {
        Stmt::Let { init, .. } => init.as_ref().and_then(expr_ident) == Some(name),
        _ => immediate_effectful_consumer(stmt, name),
    }
}

fn has_call(effects: &ExpressionEffects) -> bool {
    effects.effects.iter().any(|effect| {
        matches!(
            effect,
            EffectKind::ReadOnlyCall
                | EffectKind::UnknownCall
                | EffectKind::MethodCall
                | EffectKind::MacroExpansion
        )
    })
}

fn is_effectful(effects: &ExpressionEffects) -> bool {
    effects.effects.iter().any(|effect| {
        matches!(
            effect,
            EffectKind::UnknownCall
                | EffectKind::MethodCall
                | EffectKind::MacroExpansion
                | EffectKind::UnknownSideEffect
                | EffectKind::VolatileRead
                | EffectKind::VolatileWrite
                | EffectKind::AtomicWrite
                | EffectKind::MemoryWrite
        )
    })
}

fn early_effectful_consumer(stmt: &Stmt, name: &str) -> bool {
    match stmt {
        Stmt::Assign { target, value } => {
            matches!(target, Expr::Var(target) if target.as_str() == "__retval")
                && matches!(value, Expr::Var(value) if value.as_str() == name)
        }
        Stmt::Return(Some(expr)) => matches!(expr, Expr::Var(value) if value.as_str() == name),
        _ => false,
    }
}

fn is_receiver_use(stmt: &Stmt, name: &str) -> bool {
    walk::stmt_expr_any(stmt, &mut |expr| {
        let receiver = match expr {
            Expr::MethodCall { recv, .. } | Expr::MethodCallGeneric { recv, .. } => Some(&**recv),
            Expr::Field { base, .. } | Expr::TupleField { base, .. } => Some(&**base),
            _ => None,
        };
        matches!(receiver, Some(Expr::Var(value)) if value.as_str() == name)
    })
}

fn is_option_receiver_use(stmt: &Stmt, name: &str, ty: Option<&Type>, init: &Expr) -> bool {
    if !ty.is_some_and(is_option_like_type) || matches!(init, Expr::Value(RustValue::None)) {
        return false;
    }
    walk::stmt_expr_any(stmt, &mut |expr| {
        matches!(
            expr,
            Expr::MethodCall { recv, method, args }
                if matches!(method.as_str(), "is_some" | "is_none" | "unwrap")
                    && args.is_empty()
                    && matches!(&**recv, Expr::Var(value) if value.as_str() == name)
        )
    })
}

fn is_option_like_type(ty: &Type) -> bool {
    match ty {
        Type::FnPtr { .. } => true,
        Type::Generic { name, .. } => name == "Option",
        Type::Custom(name) => name.starts_with("Option<"),
        _ => false,
    }
}

fn is_allowed_argument_use(
    stmt: &Stmt,
    name: &str,
    ty: Option<&Type>,
    init: &Expr,
    effects: &ExpressionEffects,
    adjacent: bool,
    phase: Phase,
) -> bool {
    if phase == Phase::Early {
        return false;
    }
    if is_effectful(effects) {
        return adjacent && simple_macro_arg_use(stmt, name);
    }
    if method_arg_use(stmt, name) && contains_integer_literal(init) {
        return false;
    }
    type_stable_arg_init(init, ty) && call_or_macro_arg_use(stmt, name)
}

fn type_stable_arg_init(init: &Expr, ty: Option<&Type>) -> bool {
    match init {
        Expr::Var(_) | Expr::Cast { .. } => true,
        Expr::Unary { .. } => ty.is_some(),
        Expr::Binary { .. } => ty.is_some() && !contains_integer_literal(init),
        Expr::Index { .. } => ty.is_some(),
        Expr::Block(block) | Expr::Unsafe(block) if block.stmts.is_empty() => block
            .tail
            .as_deref()
            .is_some_and(|tail| type_stable_arg_init(tail, ty)),
        Expr::Value(RustValue::I64(_)) => matches!(ty, Some(Type::Prim(Prim::I32))),
        Expr::Value(RustValue::Bool(_)) => true,
        _ => false,
    }
}

fn contains_integer_literal(expr: &Expr) -> bool {
    match expr {
        Expr::Value(RustValue::I64(_) | RustValue::I128(_) | RustValue::Usize(_)) => true,
        Expr::Unary { expr, .. } | Expr::Cast { expr, .. } => contains_integer_literal(expr),
        Expr::Binary { lhs, rhs, .. } => {
            contains_integer_literal(lhs) || contains_integer_literal(rhs)
        }
        Expr::Index { base, .. } => contains_integer_literal(base),
        _ => false,
    }
}

fn call_or_macro_arg_use(stmt: &Stmt, name: &str) -> bool {
    match stmt {
        Stmt::Let { init, .. } => init
            .as_ref()
            .is_some_and(|expr| call_or_macro_arg_use_expr(expr, name)),
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            call_or_macro_arg_use_expr(target, name) || call_or_macro_arg_use_expr(value, name)
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => call_or_macro_arg_use_expr(expr, name),
        _ => false,
    }
}

fn method_arg_use(stmt: &Stmt, name: &str) -> bool {
    match stmt {
        Stmt::Let { init, .. } => init
            .as_ref()
            .is_some_and(|expr| method_arg_use_expr(expr, name)),
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            method_arg_use_expr(target, name) || method_arg_use_expr(value, name)
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => method_arg_use_expr(expr, name),
        _ => false,
    }
}

fn method_arg_use_expr(expr: &Expr, name: &str) -> bool {
    match expr {
        Expr::MethodCall { args, .. } | Expr::MethodCallGeneric { args, .. } => {
            args.iter().any(|arg| expr_ident_count(arg, name) > 0)
        }
        Expr::Block(block) | Expr::Unsafe(block) => block
            .tail
            .as_deref()
            .is_some_and(|tail| method_arg_use_expr(tail, name)),
        Expr::Cast { expr, .. } => method_arg_use_expr(expr, name),
        _ => false,
    }
}

fn call_or_macro_arg_use_expr(expr: &Expr, name: &str) -> bool {
    match expr {
        Expr::Call { args, .. } | Expr::Macro { args, .. } => {
            args.iter().any(|arg| call_arg_uses_name(arg, name))
        }
        Expr::MethodCall { args, .. } | Expr::MethodCallGeneric { args, .. } => {
            args.iter().any(|arg| expr_ident_count(arg, name) > 0)
        }
        Expr::Block(block) | Expr::Unsafe(block) => block
            .tail
            .as_deref()
            .is_some_and(|tail| call_or_macro_arg_use_expr(tail, name)),
        Expr::Cast { expr, .. } => call_or_macro_arg_use_expr(expr, name),
        _ => false,
    }
}

fn call_arg_uses_name(expr: &Expr, name: &str) -> bool {
    match expr {
        Expr::Var(var) => var.as_str() == name,
        Expr::Cast { expr, .. } => call_arg_uses_name(expr, name),
        Expr::Block(block) | Expr::Unsafe(block) if block.stmts.is_empty() => block
            .tail
            .as_deref()
            .is_some_and(|tail| call_arg_uses_name(tail, name)),
        _ => false,
    }
}

fn simple_macro_arg_use(stmt: &Stmt, name: &str) -> bool {
    match stmt {
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => simple_macro_arg_use_expr(expr, name),
        _ => false,
    }
}

fn simple_macro_arg_use_expr(expr: &Expr, name: &str) -> bool {
    match expr {
        Expr::Macro { args, .. } => {
            args.iter().any(|arg| simple_macro_arg_uses_name(arg, name))
                && args
                    .iter()
                    .all(|arg| simple_macro_arg_uses_name(arg, name) || is_obviously_pure_expr(arg))
        }
        Expr::Block(block) | Expr::Unsafe(block) => block
            .tail
            .as_deref()
            .is_some_and(|tail| simple_macro_arg_use_expr(tail, name)),
        _ => false,
    }
}

fn simple_macro_arg_uses_name(expr: &Expr, name: &str) -> bool {
    match expr {
        Expr::Var(var) => var.as_str() == name,
        Expr::Cast { expr, .. } | Expr::Unary { expr, .. } => {
            simple_macro_arg_uses_name(expr, name)
        }
        _ => false,
    }
}

fn is_obviously_pure_expr(expr: &Expr) -> bool {
    match expr {
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::ByteStr(_)
        | Expr::CStr(_)
        | Expr::HexFloat(_)
        | Expr::Var(_)
        | Expr::Path(_) => true,
        Expr::Cast { expr, .. } | Expr::Unary { expr, .. } => is_obviously_pure_expr(expr),
        _ => false,
    }
}

fn is_temp_name(name: &str) -> bool {
    name.strip_prefix("_v")
        .is_some_and(|rest| !rest.is_empty() && rest.bytes().all(|byte| byte.is_ascii_digit()))
}
