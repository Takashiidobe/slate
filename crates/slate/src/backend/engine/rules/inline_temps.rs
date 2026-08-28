use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{Arena, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::{Expr, Ident, Prim, RustValue, Stmt, Type, UnaryOp};

fn is_temp_name(name: &str) -> bool {
    name.strip_prefix("_v")
        .is_some_and(|rest| !rest.is_empty() && rest.bytes().all(|byte| byte.is_ascii_digit()))
}

fn expr_ident(expr: &Expr) -> Option<Ident> {
    match expr {
        Expr::Var(name) => Some(*name),
        _ => None,
    }
}

fn is_pure_primitive_bit_method(method: &str) -> bool {
    matches!(
        method,
        "reverse_bits"
            | "swap_bytes"
            | "leading_zeros"
            | "trailing_zeros"
            | "count_ones"
            | "rotate_left"
            | "rotate_right"
    )
}

fn is_movable_pure_bit_operand(expr: &Expr) -> bool {
    match expr {
        Expr::Unary {
            op: UnaryOp::Not,
            expr,
        } => is_movable_pure_bit_operand(expr),
        Expr::Cast { expr, .. } => is_movable_pure_bit_operand(expr),
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            is_movable_pure_expr(cond)
                && is_movable_pure_bit_operand(then_expr)
                && is_movable_pure_bit_operand(else_expr)
        }
        _ => is_movable_pure_expr(expr),
    }
}

fn is_movable_pure_expr(expr: &Expr) -> bool {
    match expr {
        Expr::Value(_) | Expr::Var(_) => true,
        Expr::Cast { expr, .. } => is_movable_pure_expr(expr),
        Expr::Unary { op, expr } => !matches!(op, UnaryOp::Not) && is_movable_pure_expr(expr),
        Expr::Binary { lhs, rhs, .. } => is_movable_pure_expr(lhs) && is_movable_pure_expr(rhs),
        Expr::MethodCall { recv, method, args } if is_pure_primitive_bit_method(method) => {
            is_movable_pure_bit_operand(recv) && args.iter().all(is_movable_pure_expr)
        }
        Expr::Field { base, .. } | Expr::TupleField { base, .. } => is_movable_pure_expr(base),
        Expr::Index { base, index } => is_movable_pure_expr(base) && is_movable_pure_expr(index),
        Expr::StructLit { fields, .. } => {
            fields.iter().all(|(_, value)| is_movable_pure_expr(value))
        }
        Expr::TupleStructLit { fields, .. } => fields.iter().all(is_movable_pure_expr),
        Expr::ArrayLit(elems) => elems.iter().all(is_movable_pure_expr),
        Expr::ArrayRepeat { elem, .. } => is_movable_pure_expr(elem),
        Expr::Unsafe(block) if block.stmts.is_empty() => {
            block.tail.as_deref().is_some_and(is_movable_pure_expr)
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            is_movable_pure_expr(cond)
                && is_movable_pure_expr(then_expr)
                && is_movable_pure_expr(else_expr)
        }
        _ => false,
    }
}

#[derive(Default, Clone, Copy)]
struct Effects {
    unknown_call: bool,
    method_call: bool,
    macro_expansion: bool,
    volatile_read: bool,
    volatile_write: bool,
    atomic_read: bool,
    atomic_write: bool,
    memory_write: bool,
}

impl Effects {
    fn union(self, other: Effects) -> Effects {
        Effects {
            unknown_call: self.unknown_call || other.unknown_call,
            method_call: self.method_call || other.method_call,
            macro_expansion: self.macro_expansion || other.macro_expansion,
            volatile_read: self.volatile_read || other.volatile_read,
            volatile_write: self.volatile_write || other.volatile_write,
            atomic_read: self.atomic_read || other.atomic_read,
            atomic_write: self.atomic_write || other.atomic_write,
            memory_write: self.memory_write || other.memory_write,
        }
    }

    fn is_side_effect(self) -> bool {
        self.unknown_call
            || self.method_call
            || self.macro_expansion
            || self.volatile_write
            || self.atomic_write
            || self.memory_write
    }

    fn is_effectful(self) -> bool {
        self.is_side_effect() || self.volatile_read
    }

    fn has_call(self) -> bool {
        self.unknown_call || self.method_call || self.macro_expansion
    }
}

#[derive(PartialEq, Eq)]
enum Purity {
    MovablePure,
    ReadOnly,
    Effectful,
}

fn classify_purity(expr: &Expr, effects: Effects) -> Purity {
    if effects.is_side_effect() {
        Purity::Effectful
    } else if is_movable_pure_expr(expr) {
        Purity::MovablePure
    } else {
        Purity::ReadOnly
    }
}

fn call_effects(expr: &Expr) -> Effects {
    let Expr::Call { func, .. } = expr else {
        return Effects {
            unknown_call: true,
            ..Default::default()
        };
    };
    let Expr::Var(name) = &**func else {
        return Effects {
            unknown_call: true,
            ..Default::default()
        };
    };
    match name.as_str() {
        "std::ptr::read_volatile" | "core::ptr::read_volatile" => Effects {
            volatile_read: true,
            ..Default::default()
        },
        "std::ptr::write_volatile" | "core::ptr::write_volatile" => Effects {
            volatile_write: true,
            ..Default::default()
        },
        _ => Effects {
            unknown_call: true,
            ..Default::default()
        },
    }
}

fn expr_effects(expr: &Expr) -> Effects {
    match expr {
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::CStr(_)
        | Expr::Var(_)
        | Expr::Path(_)
        | Expr::Todo(_) => Effects::default(),
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => expr_effects(expr),
        Expr::Binary { lhs, rhs, .. }
        | Expr::Range {
            start: lhs,
            end: rhs,
        } => expr_effects(lhs).union(expr_effects(rhs)),
        Expr::Call { func, args, .. } => {
            let mut effects = call_effects(expr).union(expr_effects(func));
            for arg in args {
                effects = effects.union(expr_effects(arg));
            }
            effects
        }
        Expr::MethodCall { recv, method, args } => {
            let mut effects = expr_effects(recv);
            for arg in args {
                effects = effects.union(expr_effects(arg));
            }
            if !is_pure_primitive_bit_method(method) {
                effects.method_call = true;
            }
            effects
        }
        Expr::MethodCallGeneric { recv, args, .. } => {
            let mut effects = expr_effects(recv);
            for arg in args {
                effects = effects.union(expr_effects(arg));
            }
            effects.method_call = true;
            effects
        }
        Expr::Field { base, .. }
        | Expr::TupleField { base, .. }
        | Expr::ArrayPtr { array: base, .. } => expr_effects(base),
        Expr::Index { base, index } => expr_effects(base).union(expr_effects(index)),
        Expr::StructLit { fields, .. } => {
            fields.iter().fold(Effects::default(), |acc, (_, value)| {
                acc.union(expr_effects(value))
            })
        }
        Expr::TupleStructLit { fields, .. } => {
            fields.iter().fold(Effects::default(), |acc, value| {
                acc.union(expr_effects(value))
            })
        }
        Expr::ArrayLit(elems) | Expr::VecLit(elems) => {
            elems.iter().fold(Effects::default(), |acc, elem| {
                acc.union(expr_effects(elem))
            })
        }
        Expr::ArrayRepeat { elem, .. } | Expr::Closure { body: elem, .. } => expr_effects(elem),
        Expr::VecRepeat { elem, len } => expr_effects(elem).union(expr_effects(len)),
        Expr::Macro { args, .. } => {
            let mut effects = Effects {
                macro_expansion: true,
                ..Default::default()
            };
            for arg in args {
                effects = effects.union(expr_effects(arg));
            }
            effects
        }
        Expr::Match { expr, arms } => arms.iter().fold(expr_effects(expr), |acc, arm| {
            acc.union(expr_effects(&arm.value))
        }),
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => expr_effects(cond)
            .union(expr_effects(then_expr))
            .union(expr_effects(else_expr)),
        Expr::Block(block) | Expr::Unsafe(block) => {
            if block.stmts.is_empty() {
                block
                    .tail
                    .as_deref()
                    .map_or(Effects::default(), expr_effects)
            } else {
                Effects {
                    unknown_call: true,
                    ..Default::default()
                }
            }
        }
        Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. } => {
            let mut effects = Effects {
                atomic_read: true,
                ..Default::default()
            };
            if let Some(ptr) = place.ptr_expr() {
                effects = effects.union(expr_effects(ptr));
            }
            effects
        }
        Expr::AtomicStore { place, value, .. }
        | Expr::AtomicFetch { place, value, .. }
        | Expr::AtomicSwap { place, value, .. } => {
            let mut effects = Effects {
                atomic_write: true,
                memory_write: true,
                ..Default::default()
            };
            if let Some(ptr) = place.ptr_expr() {
                effects = effects.union(expr_effects(ptr));
            }
            effects.union(expr_effects(value))
        }
        Expr::AtomicNew { value, .. } => expr_effects(value),
        Expr::AtomicCompareExchange {
            place,
            expected,
            desired,
            ..
        } => {
            let mut effects = Effects {
                atomic_write: true,
                memory_write: true,
                ..Default::default()
            };
            if let Some(ptr) = place.ptr_expr() {
                effects = effects.union(expr_effects(ptr));
            }
            effects
                .union(expr_effects(expected))
                .union(expr_effects(desired))
        }
        Expr::AtomicFence { .. } => Effects {
            atomic_write: true,
            ..Default::default()
        },
        Expr::CopyNonoverlapping { src, dst, .. } => Effects {
            memory_write: true,
            ..Default::default()
        }
        .union(expr_effects(src))
        .union(expr_effects(dst)),
        Expr::PtrCopy {
            src, dst, count, ..
        } => Effects {
            memory_write: true,
            ..Default::default()
        }
        .union(expr_effects(src))
        .union(expr_effects(dst))
        .union(expr_effects(count)),
        Expr::WriteBytes { dst, val, count } => Effects {
            memory_write: true,
            ..Default::default()
        }
        .union(expr_effects(dst))
        .union(expr_effects(val))
        .union(expr_effects(count)),
    }
}

fn is_atomic_result(expr: &Expr, effects: Effects) -> bool {
    matches!(
        expr,
        Expr::AtomicLoad { .. }
            | Expr::AtomicFetch { .. }
            | Expr::AtomicSwap { .. }
            | Expr::AtomicCompareExchange { .. }
    ) && (effects.atomic_read || effects.atomic_write)
}

fn expr_any(expr: &Expr, pred: &mut dyn FnMut(&Expr) -> bool) -> bool {
    if pred(expr) {
        return true;
    }
    match expr {
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::CStr(_)
        | Expr::Var(_)
        | Expr::Path(_)
        | Expr::Todo(_)
        | Expr::AtomicFence { .. } => false,
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => expr_any(expr, pred),
        Expr::Binary { lhs, rhs, .. }
        | Expr::Range {
            start: lhs,
            end: rhs,
        } => expr_any(lhs, pred) || expr_any(rhs, pred),
        Expr::Call { func, args, .. } => {
            expr_any(func, pred) || args.iter().any(|a| expr_any(a, pred))
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            expr_any(recv, pred) || args.iter().any(|a| expr_any(a, pred))
        }
        Expr::Field { base, .. }
        | Expr::TupleField { base, .. }
        | Expr::ArrayPtr { array: base, .. } => expr_any(base, pred),
        Expr::Index { base, index } => expr_any(base, pred) || expr_any(index, pred),
        Expr::StructLit { fields, .. } => fields.iter().any(|(_, v)| expr_any(v, pred)),
        Expr::TupleStructLit { fields, .. } | Expr::ArrayLit(fields) | Expr::VecLit(fields) => {
            fields.iter().any(|v| expr_any(v, pred))
        }
        Expr::ArrayRepeat { elem, .. } | Expr::Closure { body: elem, .. } => expr_any(elem, pred),
        Expr::VecRepeat { elem, len } => expr_any(elem, pred) || expr_any(len, pred),
        Expr::Macro { args, .. } => args.iter().any(|a| expr_any(a, pred)),
        Expr::Match { expr, arms } => {
            expr_any(expr, pred) || arms.iter().any(|arm| expr_any(&arm.value, pred))
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => expr_any(cond, pred) || expr_any(then_expr, pred) || expr_any(else_expr, pred),
        Expr::Block(block) | Expr::Unsafe(block) => {
            block
                .stmts
                .iter()
                .any(|stmt| stmt_any_expr(&stmt.stmt, pred))
                || block
                    .tail
                    .as_deref()
                    .is_some_and(|tail| expr_any(tail, pred))
        }
        Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. } => {
            place.ptr_expr().is_some_and(|ptr| expr_any(ptr, pred))
        }
        Expr::AtomicStore { place, value, .. }
        | Expr::AtomicFetch { place, value, .. }
        | Expr::AtomicSwap { place, value, .. } => {
            place.ptr_expr().is_some_and(|ptr| expr_any(ptr, pred)) || expr_any(value, pred)
        }
        Expr::AtomicNew { value, .. } => expr_any(value, pred),
        Expr::AtomicCompareExchange {
            place,
            expected,
            desired,
            ..
        } => {
            place.ptr_expr().is_some_and(|ptr| expr_any(ptr, pred))
                || expr_any(expected, pred)
                || expr_any(desired, pred)
        }
        Expr::CopyNonoverlapping { src, dst, .. } => expr_any(src, pred) || expr_any(dst, pred),
        Expr::PtrCopy {
            src, dst, count, ..
        } => expr_any(src, pred) || expr_any(dst, pred) || expr_any(count, pred),
        Expr::WriteBytes { dst, val, count } => {
            expr_any(dst, pred) || expr_any(val, pred) || expr_any(count, pred)
        }
    }
}

fn stmt_any_expr(stmt: &Stmt, pred: &mut dyn FnMut(&Expr) -> bool) -> bool {
    match stmt {
        Stmt::Let { init, .. } => init.as_ref().is_some_and(|e| expr_any(e, pred)),
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            expr_any(cond, pred)
                || then_body
                    .iter()
                    .chain(else_body)
                    .any(|s| stmt_any_expr(&s.stmt, pred))
                || expr_any(then_value, pred)
                || expr_any(else_value, pred)
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            expr_any(target, pred) || expr_any(value, pred)
        }
        Stmt::InlineAsm(asm) => {
            let mut found = false;
            for operand in &asm.operands {
                operand.visit_exprs(&mut |expr| found |= expr_any(expr, pred));
            }
            found
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => expr_any(expr, pred),
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => false,
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            expr_any(cond, pred)
                || then_body
                    .iter()
                    .chain(else_body)
                    .any(|s| stmt_any_expr(&s.stmt, pred))
        }
        Stmt::Loop { body, .. }
        | Stmt::For { body, .. }
        | Stmt::Scope { body }
        | Stmt::LabeledBlock { body, .. } => body.iter().any(|s| stmt_any_expr(&s.stmt, pred)),
        Stmt::Match { expr, arms } => {
            expr_any(expr, pred)
                || arms
                    .iter()
                    .any(|arm| arm.body.iter().any(|s| stmt_any_expr(&s.stmt, pred)))
        }
        Stmt::While { cond, body } => {
            expr_any(cond, pred)
                || body.stmts.iter().any(|s| stmt_any_expr(&s.stmt, pred))
                || body.tail.as_deref().is_some_and(|t| expr_any(t, pred))
        }
        Stmt::Unsafe { body } | Stmt::Block(body) => {
            body.stmts.iter().any(|s| stmt_any_expr(&s.stmt, pred))
                || body.tail.as_deref().is_some_and(|t| expr_any(t, pred))
        }
    }
}

fn stmt_e_ident_count(stmt: &Stmt, name: Ident) -> usize {
    match stmt {
        Stmt::Let { init, .. } => init.as_ref().map_or(0, |e| e_ident_count(e, name)),
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            e_ident_count(cond, name)
                + then_body
                    .iter()
                    .chain(else_body)
                    .map(|s| stmt_e_ident_count(&s.stmt, name))
                    .sum::<usize>()
                + e_ident_count(then_value, name)
                + e_ident_count(else_value, name)
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            e_ident_count(target, name) + e_ident_count(value, name)
        }
        Stmt::InlineAsm(asm) => {
            let mut count = 0;
            for operand in &asm.operands {
                operand.visit_exprs(&mut |expr| count += e_ident_count(expr, name));
            }
            count
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => e_ident_count(expr, name),
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => 0,
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            e_ident_count(cond, name)
                + then_body
                    .iter()
                    .chain(else_body)
                    .map(|s| stmt_e_ident_count(&s.stmt, name))
                    .sum::<usize>()
        }
        Stmt::Loop { body, .. }
        | Stmt::For { body, .. }
        | Stmt::Scope { body }
        | Stmt::LabeledBlock { body, .. } => {
            body.iter().map(|s| stmt_e_ident_count(&s.stmt, name)).sum()
        }
        Stmt::Match { expr, arms } => {
            e_ident_count(expr, name)
                + arms
                    .iter()
                    .map(|arm| {
                        arm.body
                            .iter()
                            .map(|s| stmt_e_ident_count(&s.stmt, name))
                            .sum::<usize>()
                    })
                    .sum::<usize>()
        }
        Stmt::While { cond, body } => {
            e_ident_count(cond, name)
                + body
                    .stmts
                    .iter()
                    .map(|s| stmt_e_ident_count(&s.stmt, name))
                    .sum::<usize>()
                + body.tail.as_deref().map_or(0, |t| e_ident_count(t, name))
        }
        Stmt::Unsafe { body } | Stmt::Block(body) => {
            body.stmts
                .iter()
                .map(|s| stmt_e_ident_count(&s.stmt, name))
                .sum::<usize>()
                + body.tail.as_deref().map_or(0, |t| e_ident_count(t, name))
        }
    }
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

fn simple_macro_arg_uses_name(expr: &Expr, name: Ident) -> bool {
    match expr {
        Expr::Var(var) => *var == name,
        Expr::Cast { expr, .. } | Expr::Unary { expr, .. } => {
            simple_macro_arg_uses_name(expr, name)
        }
        _ => false,
    }
}

fn simple_macro_arg_use_expr(expr: &Expr, name: Ident) -> bool {
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

fn is_option_like_type(ty: &Type) -> bool {
    match ty {
        Type::FnPtr { .. } => true,
        Type::Generic { name, .. } => name == "Option",
        Type::Custom(name) => name.starts_with("Option<"),
        _ => false,
    }
}

fn kind_own_ident_count(kind: &NodeKind, name: Ident) -> usize {
    match kind {
        NodeKind::Let { init, .. } => init.as_ref().map_or(0, |e| e_ident_count(e, name)),
        NodeKind::LetIf {
            cond,
            then_value,
            else_value,
            ..
        } => {
            e_ident_count(cond, name)
                + e_ident_count(then_value, name)
                + e_ident_count(else_value, name)
        }
        NodeKind::Assign { target, value } | NodeKind::CompoundAssign { target, value, .. } => {
            e_ident_count(target, name) + e_ident_count(value, name)
        }
        NodeKind::InlineAsm(asm) => {
            let mut count = 0;
            for operand in &asm.operands {
                operand.visit_exprs(&mut |expr| count += e_ident_count(expr, name));
            }
            count
        }
        NodeKind::Expr(expr) => e_ident_count(expr, name),
        NodeKind::Return(expr) => expr.as_ref().map_or(0, |e| e_ident_count(e, name)),
        NodeKind::Unsafe { tail, .. } | NodeKind::Block { tail, .. } => {
            tail.as_ref().map_or(0, |t| e_ident_count(t, name))
        }
        NodeKind::While { cond, tail, .. } => {
            e_ident_count(cond, name) + tail.as_ref().map_or(0, |t| e_ident_count(t, name))
        }
        NodeKind::If { cond, .. } => e_ident_count(cond, name),
        NodeKind::For { iter, .. } => e_ident_count(iter, name),
        NodeKind::Loop { .. } | NodeKind::Scope { .. } | NodeKind::LabeledBlock { .. } => 0,
        NodeKind::Match { expr, .. } => e_ident_count(expr, name),
        NodeKind::Break(_) | NodeKind::Continue(_) => 0,
    }
}

fn e_ident_count(expr: &Expr, name: Ident) -> usize {
    match expr {
        Expr::Var(v) => usize::from(*v == name),
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::CStr(_)
        | Expr::Path(_)
        | Expr::Todo(_)
        | Expr::AtomicFence { .. } => 0,
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => e_ident_count(expr, name),
        Expr::Binary { lhs, rhs, .. }
        | Expr::Range {
            start: lhs,
            end: rhs,
        } => e_ident_count(lhs, name) + e_ident_count(rhs, name),
        Expr::Call { func, args, .. } => {
            e_ident_count(func, name) + args.iter().map(|a| e_ident_count(a, name)).sum::<usize>()
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            e_ident_count(recv, name) + args.iter().map(|a| e_ident_count(a, name)).sum::<usize>()
        }
        Expr::Field { base, .. }
        | Expr::TupleField { base, .. }
        | Expr::ArrayPtr { array: base, .. } => e_ident_count(base, name),
        Expr::Index { base, index } => e_ident_count(base, name) + e_ident_count(index, name),
        Expr::StructLit { fields, .. } => fields.iter().map(|(_, v)| e_ident_count(v, name)).sum(),
        Expr::TupleStructLit { fields, .. } => fields.iter().map(|v| e_ident_count(v, name)).sum(),
        Expr::ArrayLit(elems) | Expr::VecLit(elems) => {
            elems.iter().map(|e| e_ident_count(e, name)).sum()
        }
        Expr::ArrayRepeat { elem, .. } | Expr::Closure { body: elem, .. } => {
            e_ident_count(elem, name)
        }
        Expr::VecRepeat { elem, len } => e_ident_count(elem, name) + e_ident_count(len, name),
        Expr::Macro { args, .. } => args.iter().map(|a| e_ident_count(a, name)).sum(),
        Expr::Match { expr, arms } => {
            e_ident_count(expr, name)
                + arms
                    .iter()
                    .map(|arm| e_ident_count(&arm.value, name))
                    .sum::<usize>()
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            e_ident_count(cond, name)
                + e_ident_count(then_expr, name)
                + e_ident_count(else_expr, name)
        }
        Expr::Block(block) | Expr::Unsafe(block) => {
            block
                .stmts
                .iter()
                .map(|stmt| stmt_e_ident_count(&stmt.stmt, name))
                .sum::<usize>()
                + block.tail.as_deref().map_or(0, |t| e_ident_count(t, name))
        }
        Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. } => {
            place.ptr_expr().map_or(0, |p| e_ident_count(p, name))
        }
        Expr::AtomicStore { place, value, .. }
        | Expr::AtomicFetch { place, value, .. }
        | Expr::AtomicSwap { place, value, .. } => {
            place.ptr_expr().map_or(0, |p| e_ident_count(p, name)) + e_ident_count(value, name)
        }
        Expr::AtomicNew { value, .. } => e_ident_count(value, name),
        Expr::AtomicCompareExchange {
            place,
            expected,
            desired,
            ..
        } => {
            place.ptr_expr().map_or(0, |p| e_ident_count(p, name))
                + e_ident_count(expected, name)
                + e_ident_count(desired, name)
        }
        Expr::CopyNonoverlapping { src, dst, .. } => {
            e_ident_count(src, name) + e_ident_count(dst, name)
        }
        Expr::PtrCopy {
            src, dst, count, ..
        } => e_ident_count(src, name) + e_ident_count(dst, name) + e_ident_count(count, name),
        Expr::WriteBytes { dst, val, count } => {
            e_ident_count(dst, name) + e_ident_count(val, name) + e_ident_count(count, name)
        }
    }
}

fn node_ident_count(arena: &Arena, id: NodeId, name: Ident) -> usize {
    let Some(kind) = arena.get(id) else {
        return 0;
    };
    kind_own_ident_count(kind, name)
        + kind
            .child_lists()
            .iter()
            .flat_map(|list| list.iter())
            .map(|&child| node_ident_count(arena, child, name))
            .sum::<usize>()
}

fn kind_own_effects(kind: &NodeKind) -> Effects {
    match kind {
        NodeKind::Let { init, .. } => init.as_ref().map_or(Effects::default(), expr_effects),
        NodeKind::LetIf {
            cond,
            then_value,
            else_value,
            ..
        } => expr_effects(cond)
            .union(expr_effects(then_value))
            .union(expr_effects(else_value)),
        NodeKind::Assign { target, value } | NodeKind::CompoundAssign { target, value, .. } => {
            Effects {
                memory_write: true,
                ..Default::default()
            }
            .union(expr_effects(target))
            .union(expr_effects(value))
        }
        NodeKind::InlineAsm(asm) => {
            let mut effects = Effects {
                macro_expansion: true,
                ..Default::default()
            };
            for operand in &asm.operands {
                operand.visit_exprs(&mut |expr| effects = effects.union(expr_effects(expr)));
            }
            effects
        }
        NodeKind::Expr(expr) => expr_effects(expr),
        NodeKind::Return(expr) => expr.as_ref().map_or(Effects::default(), expr_effects),
        NodeKind::Unsafe { tail, .. } | NodeKind::Block { tail, .. } => tail
            .as_ref()
            .map_or(Effects::default(), |t| expr_effects(t)),
        NodeKind::While { cond, tail, .. } => expr_effects(cond).union(
            tail.as_ref()
                .map_or(Effects::default(), |t| expr_effects(t)),
        ),
        NodeKind::If { cond, .. } => expr_effects(cond),
        NodeKind::For { iter, .. } => expr_effects(iter),
        NodeKind::Loop { .. } | NodeKind::Scope { .. } | NodeKind::LabeledBlock { .. } => {
            Effects::default()
        }
        NodeKind::Match { expr, .. } => expr_effects(expr),
        NodeKind::Break(_) | NodeKind::Continue(_) => Effects::default(),
    }
}

fn node_effects(arena: &Arena, id: NodeId) -> Effects {
    let Some(kind) = arena.get(id) else {
        return Effects::default();
    };
    kind.child_lists()
        .iter()
        .flat_map(|list| list.iter())
        .fold(kind_own_effects(kind), |acc, &child| {
            acc.union(node_effects(arena, child))
        })
}

fn kind_own_expr_any(kind: &NodeKind, pred: &mut dyn FnMut(&Expr) -> bool) -> bool {
    match kind {
        NodeKind::Let { init, .. } => init.as_ref().is_some_and(|e| expr_any(e, pred)),
        NodeKind::LetIf {
            cond,
            then_value,
            else_value,
            ..
        } => expr_any(cond, pred) || expr_any(then_value, pred) || expr_any(else_value, pred),
        NodeKind::Assign { target, value } | NodeKind::CompoundAssign { target, value, .. } => {
            expr_any(target, pred) || expr_any(value, pred)
        }
        NodeKind::InlineAsm(asm) => {
            let mut found = false;
            for operand in &asm.operands {
                operand.visit_exprs(&mut |expr| found |= expr_any(expr, pred));
            }
            found
        }
        NodeKind::Expr(expr) => expr_any(expr, pred),
        NodeKind::Return(expr) => expr.as_ref().is_some_and(|e| expr_any(e, pred)),
        NodeKind::Unsafe { tail, .. } | NodeKind::Block { tail, .. } => {
            tail.as_ref().is_some_and(|t| expr_any(t, pred))
        }
        NodeKind::While { cond, tail, .. } => {
            expr_any(cond, pred) || tail.as_ref().is_some_and(|t| expr_any(t, pred))
        }
        NodeKind::If { cond, .. } => expr_any(cond, pred),
        NodeKind::For { iter, .. } => expr_any(iter, pred),
        NodeKind::Loop { .. } | NodeKind::Scope { .. } | NodeKind::LabeledBlock { .. } => false,
        NodeKind::Match { expr, .. } => expr_any(expr, pred),
        NodeKind::Break(_) | NodeKind::Continue(_) => false,
    }
}

fn node_expr_any(arena: &Arena, id: NodeId, pred: &mut dyn FnMut(&Expr) -> bool) -> bool {
    let Some(kind) = arena.get(id) else {
        return false;
    };
    kind_own_expr_any(kind, pred)
        || kind
            .child_lists()
            .iter()
            .flat_map(|list| list.iter())
            .any(|&child| node_expr_any(arena, child, pred))
}

fn is_receiver_use(arena: &Arena, id: NodeId, name: Ident) -> bool {
    node_expr_any(arena, id, &mut |expr| {
        let receiver = match expr {
            Expr::MethodCall { recv, .. } | Expr::MethodCallGeneric { recv, .. } => Some(&**recv),
            Expr::Field { base, .. } | Expr::TupleField { base, .. } => Some(&**base),
            _ => None,
        };
        matches!(receiver, Some(Expr::Var(value)) if *value == name)
    })
}

fn is_option_receiver_use(
    arena: &Arena,
    id: NodeId,
    name: Ident,
    ty: Option<&Type>,
    init: &Expr,
) -> bool {
    if !ty.is_some_and(is_option_like_type) || matches!(init, Expr::Value(RustValue::None)) {
        return false;
    }
    node_expr_any(arena, id, &mut |expr| {
        matches!(
            expr,
            Expr::MethodCall { recv, method, args }
                if matches!(method.as_str(), "is_some" | "is_none" | "unwrap")
                    && args.is_empty()
                    && matches!(&**recv, Expr::Var(value) if *value == name)
        )
    })
}

fn macro_arg_alias_conflict(arena: &Arena, id: NodeId, root: &str) -> bool {
    node_expr_any(arena, id, &mut |expr| {
        let Expr::Macro { args, .. } = expr else {
            return false;
        };
        args.iter().any(|arg| {
            expr_any(arg, &mut |sub| {
                matches!(
                    sub,
                    Expr::AddrOf { mutable: true, expr }
                        if root_var(expr) == Some(root)
                )
            })
        })
    })
}

fn immediate_effectful_consumer(arena: &Arena, id: NodeId, name: Ident) -> bool {
    match arena.get(id) {
        Some(NodeKind::Assign { target, value })
        | Some(NodeKind::CompoundAssign { target, value, .. }) => {
            expr_ident(target).is_some() && expr_ident(value) == Some(name)
        }
        Some(NodeKind::Return(Some(expr))) => expr_ident(expr) == Some(name),
        Some(NodeKind::Unsafe { stmts, tail }) if tail.is_none() && stmts.len() == 1 => {
            immediate_effectful_consumer(arena, stmts[0], name)
        }
        Some(NodeKind::Expr(expr)) => simple_macro_arg_use_expr(expr, name),
        _ => false,
    }
}

fn immediate_atomic_result_consumer(arena: &Arena, id: NodeId, name: Ident) -> bool {
    match arena.get(id) {
        Some(NodeKind::Let {
            init: Some(init), ..
        }) => expr_ident(init) == Some(name),
        _ => immediate_effectful_consumer(arena, id, name),
    }
}

fn early_effectful_consumer(arena: &Arena, id: NodeId, name: Ident) -> bool {
    match arena.get(id) {
        Some(NodeKind::Assign { target, value }) => {
            matches!(target, Expr::Var(t) if t.as_str() == "__retval")
                && matches!(value, Expr::Var(v) if *v == name)
        }
        Some(NodeKind::Return(Some(expr))) => matches!(expr, Expr::Var(v) if *v == name),
        _ => false,
    }
}

fn expr_is_type_anchored(expr: &Expr) -> bool {
    match expr {
        Expr::Value(value) => !matches!(
            value,
            RustValue::I64(_)
                | RustValue::I128(_)
                | RustValue::U128(_)
                | RustValue::Float(_)
                | RustValue::NullPtr
                | RustValue::None
        ),
        Expr::Var(_) => true,
        Expr::Cast { expr, .. } | Expr::Unary { expr, .. } => expr_is_type_anchored(expr),
        Expr::Binary { lhs, rhs, .. } => expr_is_type_anchored(lhs) && expr_is_type_anchored(rhs),
        _ => true,
    }
}

fn is_top_level_use(arena: &Arena, id: NodeId, name: Ident) -> bool {
    match arena.get(id) {
        Some(NodeKind::Let {
            init: Some(init), ..
        }) => expr_ident(init) == Some(name),
        Some(NodeKind::Assign { value, .. }) | Some(NodeKind::CompoundAssign { value, .. }) => {
            expr_ident(value) == Some(name)
        }
        Some(NodeKind::Return(Some(expr))) => expr_ident(expr) == Some(name),
        Some(NodeKind::Unsafe { stmts, tail }) if tail.is_none() && stmts.len() == 1 => {
            is_top_level_use(arena, stmts[0], name)
        }
        _ => false,
    }
}

struct Found {
    parent: NodeId,
    list_index: usize,
    decl_pos: usize,
    consumer_pos: usize,
    consumer_id: NodeId,
}

fn locate_consumer(arena: &Arena, decl_id: NodeId, name: Ident) -> Option<Found> {
    let parent = arena.parent(decl_id)?;
    let lists = arena.get(parent)?.child_lists();
    for (list_index, list) in lists.iter().enumerate() {
        let Some(decl_pos) = list.iter().position(|&child| child == decl_id) else {
            continue;
        };
        for (offset, &sibling) in list.iter().enumerate().skip(decl_pos + 1) {
            if node_ident_count(arena, sibling, name) > 0 {
                return Some(Found {
                    parent,
                    list_index,
                    decl_pos,
                    consumer_pos: offset,
                    consumer_id: sibling,
                });
            }
        }
        return None;
    }
    None
}

fn function_root(arena: &Arena, mut id: NodeId) -> NodeId {
    while let Some(parent) = arena.parent(id) {
        id = parent;
    }
    id
}

fn is_movable_pure_temp(arena: &Arena, id: NodeId) -> bool {
    match arena.get(id) {
        Some(NodeKind::Let {
            name,
            mutable: false,
            init: Some(init),
            ..
        }) => {
            is_temp_name(name.as_str())
                && classify_purity(init, expr_effects(init)) == Purity::MovablePure
        }
        _ => false,
    }
}

fn node_substitute_var(arena: &mut Arena, id: NodeId, name: &str, replacement: &Expr) -> bool {
    let mut changed = arena
        .get_mut(id)
        .is_some_and(|kind| kind_own_substitute_var(kind, name, replacement));
    let children: Vec<NodeId> = arena
        .get(id)
        .map(|kind| kind.child_lists().into_iter().flatten().copied().collect())
        .unwrap_or_default();
    for child in children {
        changed |= node_substitute_var(arena, child, name, replacement);
    }
    changed
}

fn kind_own_substitute_var(kind: &mut NodeKind, name: &str, replacement: &Expr) -> bool {
    match kind {
        NodeKind::Let { init, .. } => init
            .as_mut()
            .is_some_and(|e| e.substitute_var(name, replacement)),
        NodeKind::LetIf {
            cond,
            then_value,
            else_value,
            ..
        } => {
            let a = cond.substitute_var(name, replacement);
            let b = then_value.substitute_var(name, replacement);
            let c = else_value.substitute_var(name, replacement);
            a || b || c
        }
        NodeKind::Assign { target, value } | NodeKind::CompoundAssign { target, value, .. } => {
            let a = target.substitute_var(name, replacement);
            let b = value.substitute_var(name, replacement);
            a || b
        }
        NodeKind::InlineAsm(asm) => {
            let mut changed = false;
            for operand in &mut asm.operands {
                operand
                    .visit_exprs_mut(&mut |expr| changed |= expr.substitute_var(name, replacement));
            }
            changed
        }
        NodeKind::Expr(expr) => expr.substitute_var(name, replacement),
        NodeKind::Return(expr) => expr
            .as_mut()
            .is_some_and(|e| e.substitute_var(name, replacement)),
        NodeKind::Unsafe { tail, .. } | NodeKind::Block { tail, .. } => tail
            .as_mut()
            .is_some_and(|t| t.substitute_var(name, replacement)),
        NodeKind::While { cond, tail, .. } => {
            let a = cond.substitute_var(name, replacement);
            let b = tail
                .as_mut()
                .is_some_and(|t| t.substitute_var(name, replacement));
            a || b
        }
        NodeKind::If { cond, .. } => cond.substitute_var(name, replacement),
        NodeKind::For { iter, pat, .. } => {
            if pat.as_str() == name {
                false
            } else {
                iter.substitute_var(name, replacement)
            }
        }
        NodeKind::Loop { .. } | NodeKind::Scope { .. } | NodeKind::LabeledBlock { .. } => false,
        NodeKind::Match { expr, .. } => expr.substitute_var(name, replacement),
        NodeKind::Break(_) | NodeKind::Continue(_) => false,
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

fn call_arg_uses_name(expr: &Expr, name: Ident) -> bool {
    match expr {
        Expr::Var(var) => *var == name,
        Expr::Cast { expr, .. } => call_arg_uses_name(expr, name),
        Expr::Block(block) | Expr::Unsafe(block) if block.stmts.is_empty() => block
            .tail
            .as_deref()
            .is_some_and(|tail| call_arg_uses_name(tail, name)),
        _ => false,
    }
}

fn call_or_macro_arg_use_expr(expr: &Expr, name: Ident) -> bool {
    match expr {
        Expr::Call { args, .. } | Expr::Macro { args, .. } => {
            args.iter().any(|arg| call_arg_uses_name(arg, name))
        }
        Expr::MethodCall { args, .. } | Expr::MethodCallGeneric { args, .. } => {
            args.iter().any(|arg| e_ident_count(arg, name) > 0)
        }
        Expr::Block(block) | Expr::Unsafe(block) => block
            .tail
            .as_deref()
            .is_some_and(|tail| call_or_macro_arg_use_expr(tail, name)),
        Expr::Cast { expr, .. } => call_or_macro_arg_use_expr(expr, name),
        _ => false,
    }
}

fn kind_call_or_macro_arg_use(kind: &NodeKind, name: Ident) -> bool {
    match kind {
        NodeKind::Let {
            init: Some(init), ..
        } => call_or_macro_arg_use_expr(init, name),
        NodeKind::Assign { target, value } | NodeKind::CompoundAssign { target, value, .. } => {
            call_or_macro_arg_use_expr(target, name) || call_or_macro_arg_use_expr(value, name)
        }
        NodeKind::Expr(expr) => call_or_macro_arg_use_expr(expr, name),
        NodeKind::Return(Some(expr)) => call_or_macro_arg_use_expr(expr, name),
        _ => false,
    }
}

fn method_arg_use_expr(expr: &Expr, name: Ident) -> bool {
    match expr {
        Expr::MethodCall { args, .. } | Expr::MethodCallGeneric { args, .. } => {
            args.iter().any(|arg| e_ident_count(arg, name) > 0)
        }
        Expr::Block(block) | Expr::Unsafe(block) => block
            .tail
            .as_deref()
            .is_some_and(|tail| method_arg_use_expr(tail, name)),
        Expr::Cast { expr, .. } => method_arg_use_expr(expr, name),
        _ => false,
    }
}

fn kind_method_arg_use(kind: &NodeKind, name: Ident) -> bool {
    match kind {
        NodeKind::Let {
            init: Some(init), ..
        } => method_arg_use_expr(init, name),
        NodeKind::Assign { target, value } | NodeKind::CompoundAssign { target, value, .. } => {
            method_arg_use_expr(target, name) || method_arg_use_expr(value, name)
        }
        NodeKind::Expr(expr) => method_arg_use_expr(expr, name),
        NodeKind::Return(Some(expr)) => method_arg_use_expr(expr, name),
        _ => false,
    }
}

fn kind_simple_macro_arg_use(kind: &NodeKind, name: Ident) -> bool {
    match kind {
        NodeKind::Expr(expr) => simple_macro_arg_use_expr(expr, name),
        NodeKind::Return(Some(expr)) => simple_macro_arg_use_expr(expr, name),
        _ => false,
    }
}

fn is_allowed_argument_use(
    kind: &NodeKind,
    name: Ident,
    ty: Option<&Type>,
    init: &Expr,
    effects: Effects,
    adjacent: bool,
) -> bool {
    if effects.is_effectful() {
        return adjacent && kind_simple_macro_arg_use(kind, name);
    }
    if kind_method_arg_use(kind, name) && contains_integer_literal(init) {
        return false;
    }
    type_stable_arg_init(init, ty) && kind_call_or_macro_arg_use(kind, name)
}

pub(in crate::backend::engine) struct EarlyInlineTemps;

impl NodeRule for EarlyInlineTemps {
    fn name(&self) -> &'static str {
        "inline_temps::early"
    }

    fn priority(&self) -> u32 {
        3
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::Let]
    }

    fn matches(&self, arena: &Arena, id: NodeId) -> bool {
        matches!(
            arena.get(id),
            Some(NodeKind::Let {
                name,
                mutable: false,
                init: Some(_),
                ..
            }) if is_temp_name(name.as_str())
        )
    }

    fn apply(&self, arena: &mut Arena, id: NodeId) -> bool {
        let Some(NodeKind::Let {
            name,
            mutable: false,
            init: Some(init),
            ty,
        }) = arena.get(id)
        else {
            return false;
        };
        if !is_temp_name(name.as_str()) {
            return false;
        }
        let name = *name;
        let ty = ty.clone();
        let init = init.clone();

        let root = function_root(arena, id);
        if node_ident_count(arena, root, name) != 1 {
            return false;
        }

        let Some(found) = locate_consumer(arena, id, name) else {
            return false;
        };

        let producer_effects = expr_effects(&init);
        let producer_purity = classify_purity(&init, producer_effects);
        let adjacent = found.consumer_pos == found.decl_pos + 1;

        if producer_purity == Purity::MovablePure {
            let Some(parent_kind) = arena.get(found.parent) else {
                return false;
            };
            let lists = parent_kind.child_lists();
            let Some(list) = lists.get(found.list_index) else {
                return false;
            };
            let intervening = &list[found.decl_pos + 1..found.consumer_pos];
            if !intervening
                .iter()
                .all(|&sibling| is_movable_pure_temp(arena, sibling))
            {
                return false;
            }
        } else {
            let effectful_branch = producer_purity == Purity::Effectful
                && immediate_effectful_consumer(arena, found.consumer_id, name);
            let atomic_branch = is_atomic_result(&init, producer_effects)
                && immediate_atomic_result_consumer(arena, found.consumer_id, name);
            if !(adjacent && (effectful_branch || atomic_branch)) {
                return false;
            }
        }

        if producer_effects.is_effectful()
            && !early_effectful_consumer(arena, found.consumer_id, name)
        {
            return false;
        }

        let allowed_receiver =
            is_option_receiver_use(arena, found.consumer_id, name, ty.as_ref(), &init);
        let consumer_effects = node_effects(arena, found.consumer_id);
        if consumer_effects.has_call() && !allowed_receiver {
            return false;
        }
        if is_receiver_use(arena, found.consumer_id, name) && !allowed_receiver {
            return false;
        }
        if let Some(root) = root_var(&init)
            && macro_arg_alias_conflict(arena, found.consumer_id, root)
        {
            return false;
        }

        if !expr_is_type_anchored(&init) && !is_top_level_use(arena, found.consumer_id, name) {
            return false;
        }

        if !node_substitute_var(arena, found.consumer_id, name.as_str(), &init) {
            return false;
        }
        arena.touch_subtree(found.consumer_id);

        let _ = arena.take(id);
        if let Some(parent_kind) = arena.get_mut(found.parent)
            && let Some(list) = parent_kind.child_lists_mut().get_mut(found.list_index)
        {
            list.remove(found.decl_pos);
        }
        true
    }
}

pub(in crate::backend::engine) struct LateInlineTemps;

impl NodeRule for LateInlineTemps {
    fn name(&self) -> &'static str {
        "inline_temps::late"
    }

    fn priority(&self) -> u32 {
        41
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[NodeKindTag::Let]
    }

    fn matches(&self, arena: &Arena, id: NodeId) -> bool {
        matches!(
            arena.get(id),
            Some(NodeKind::Let {
                name,
                mutable: false,
                init: Some(_),
                ..
            }) if is_temp_name(name.as_str())
        )
    }

    fn apply(&self, arena: &mut Arena, id: NodeId) -> bool {
        let Some(NodeKind::Let {
            name,
            mutable: false,
            init: Some(init),
            ty,
        }) = arena.get(id)
        else {
            return false;
        };
        if !is_temp_name(name.as_str()) {
            return false;
        }
        let name = *name;
        let ty = ty.clone();
        let init = init.clone();

        let root = function_root(arena, id);
        if node_ident_count(arena, root, name) != 1 {
            return false;
        }

        let Some(found) = locate_consumer(arena, id, name) else {
            return false;
        };

        let producer_effects = expr_effects(&init);
        let producer_purity = classify_purity(&init, producer_effects);
        let adjacent = found.consumer_pos == found.decl_pos + 1;

        if producer_purity == Purity::MovablePure {
            let Some(parent_kind) = arena.get(found.parent) else {
                return false;
            };
            let lists = parent_kind.child_lists();
            let Some(list) = lists.get(found.list_index) else {
                return false;
            };
            let intervening = &list[found.decl_pos + 1..found.consumer_pos];
            if !intervening
                .iter()
                .all(|&sibling| is_movable_pure_temp(arena, sibling))
            {
                return false;
            }
        } else {
            let effectful_branch = producer_purity == Purity::Effectful
                && immediate_effectful_consumer(arena, found.consumer_id, name);
            let atomic_branch = is_atomic_result(&init, producer_effects)
                && immediate_atomic_result_consumer(arena, found.consumer_id, name);
            if !(adjacent && (effectful_branch || atomic_branch)) {
                return false;
            }
        }

        let allowed_receiver =
            is_option_receiver_use(arena, found.consumer_id, name, ty.as_ref(), &init);
        let Some(consumer_kind) = arena.get(found.consumer_id) else {
            return false;
        };
        let allowed_argument = is_allowed_argument_use(
            consumer_kind,
            name,
            ty.as_ref(),
            &init,
            producer_effects,
            adjacent,
        );
        let consumer_effects = node_effects(arena, found.consumer_id);
        if consumer_effects.has_call() && !(allowed_receiver || allowed_argument) {
            return false;
        }
        if is_receiver_use(arena, found.consumer_id, name) && !allowed_receiver {
            return false;
        }
        if let Some(root) = root_var(&init)
            && macro_arg_alias_conflict(arena, found.consumer_id, root)
        {
            return false;
        }

        if !expr_is_type_anchored(&init) && !is_top_level_use(arena, found.consumer_id, name) {
            return false;
        }

        if !node_substitute_var(arena, found.consumer_id, name.as_str(), &init) {
            return false;
        }
        arena.touch_subtree(found.consumer_id);

        let _ = arena.take(id);
        if let Some(parent_kind) = arena.get_mut(found.parent)
            && let Some(list) = parent_kind.child_lists_mut().get_mut(found.list_index)
        {
            list.remove(found.decl_pos);
        }
        true
    }
}
