//! Identifier-counting helpers shared across fixup passes.

use crate::backend::rust_ast::{AtomicPlace, Block, Expr, Pattern, Stmt};

pub(super) fn expr_ident(expr: &Expr) -> Option<&str> {
    match expr {
        Expr::Var(s) if is_ident(s.as_str()) => Some(s.as_str()),
        _ => None,
    }
}

pub(super) fn stmt_ident_count(stmt: &Stmt, name: &str) -> usize {
    match stmt {
        Stmt::Let { name: n, init, .. } => {
            usize::from(n == name) + init.as_ref().map_or(0, |expr| expr_ident_count(expr, name))
        }
        Stmt::LetIf {
            name: n,
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            usize::from(n == name)
                + expr_ident_count(cond, name)
                + then_body
                    .iter()
                    .chain(else_body)
                    .map(|stmt| stmt_ident_count(&stmt.stmt, name))
                    .sum::<usize>()
                + expr_ident_count(then_value, name)
                + expr_ident_count(else_value, name)
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            expr_ident_count(target, name) + expr_ident_count(value, name)
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => expr_ident_count(expr, name),
        Stmt::Return(None) => 0,
        Stmt::Break(label) | Stmt::Continue(label) => label
            .as_ref()
            .map_or(0, |label| usize::from(label.as_str() == name)),
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            expr_ident_count(cond, name)
                + then_body
                    .iter()
                    .chain(else_body)
                    .map(|stmt| stmt_ident_count(&stmt.stmt, name))
                    .sum::<usize>()
        }
        Stmt::Loop { label, body } => {
            label
                .as_ref()
                .map_or(0, |label| ident_count(label.as_str(), name))
                + body
                    .iter()
                    .map(|stmt| stmt_ident_count(&stmt.stmt, name))
                    .sum::<usize>()
        }
        Stmt::For { pat, iter, body } => {
            usize::from(pat == name)
                + expr_ident_count(iter, name)
                + body
                    .iter()
                    .map(|stmt| stmt_ident_count(&stmt.stmt, name))
                    .sum::<usize>()
        }
        Stmt::LabeledBlock { label, body } => {
            ident_count(label.as_str(), name)
                + body
                    .iter()
                    .map(|stmt| stmt_ident_count(&stmt.stmt, name))
                    .sum::<usize>()
        }
        Stmt::Scope { body } => body
            .iter()
            .map(|stmt| stmt_ident_count(&stmt.stmt, name))
            .sum(),
        Stmt::Unsafe { body } => block_ident_count(body, name),
        Stmt::Match { expr, arms } => {
            expr_ident_count(expr, name)
                + arms
                    .iter()
                    .map(|arm| {
                        pattern_ident_count(&arm.pattern, name)
                            + arm
                                .body
                                .iter()
                                .map(|stmt| stmt_ident_count(&stmt.stmt, name))
                                .sum::<usize>()
                    })
                    .sum::<usize>()
        }
        Stmt::While { cond, body } => expr_ident_count(cond, name) + block_ident_count(body, name),
        Stmt::Block(body) => block_ident_count(body, name),
        Stmt::InlineAsm(asm) => asm
            .operands
            .iter()
            .map(|operand| {
                let mut count = 0;
                operand.visit_exprs(&mut |expr| count += expr_ident_count(expr, name));
                count
            })
            .sum(),
    }
}

fn pattern_ident_count(pattern: &Pattern, name: &str) -> usize {
    match pattern {
        Pattern::Wildcard
        | Pattern::I64(_)
        | Pattern::I128(_)
        | Pattern::U128(_)
        | Pattern::InclusiveRange { .. } => 0,
        Pattern::Binding(binding) => usize::from(binding.as_str() == name),
        Pattern::TupleStruct { name: ctor, fields } => {
            usize::from(ctor.as_str() == name)
                + fields
                    .iter()
                    .map(|field| pattern_ident_count(field, name))
                    .sum::<usize>()
        }
    }
}

fn block_ident_count(block: &Block, name: &str) -> usize {
    block
        .stmts
        .iter()
        .map(|stmt| stmt_ident_count(&stmt.stmt, name))
        .sum::<usize>()
        + block
            .tail
            .as_ref()
            .map_or(0, |tail| expr_ident_count(tail, name))
}

fn atomic_place_ident_count(place: &AtomicPlace, name: &str) -> usize {
    match place {
        AtomicPlace::Ptr(ptr) => expr_ident_count(ptr, name),
        AtomicPlace::Local(local) => usize::from(local.as_str() == name),
    }
}

pub(super) fn expr_ident_count(expr: &Expr, name: &str) -> usize {
    match expr {
        Expr::Value(_) => 0,
        Expr::Str(_) | Expr::HexFloat(_) | Expr::ByteStr(_) | Expr::CStr(_) => 0,
        Expr::Var(s) => usize::from(s.as_str() == name),
        Expr::Path(_) => 0,
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => expr_ident_count(expr, name),
        Expr::Block(block) | Expr::Unsafe(block) => block_ident_count(block, name),
        Expr::CopyNonoverlapping { src, dst, .. } => {
            expr_ident_count(src, name) + expr_ident_count(dst, name)
        }
        Expr::PtrCopy {
            src, dst, count, ..
        } => {
            expr_ident_count(src, name)
                + expr_ident_count(dst, name)
                + expr_ident_count(count, name)
        }
        Expr::WriteBytes { dst, val, count } => {
            expr_ident_count(dst, name)
                + expr_ident_count(val, name)
                + expr_ident_count(count, name)
        }
        Expr::AtomicFence { .. } | Expr::Todo(_) => 0,
        Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. } => {
            atomic_place_ident_count(place, name)
        }
        Expr::AtomicStore { place, value, .. }
        | Expr::AtomicFetch { place, value, .. }
        | Expr::AtomicSwap { place, value, .. } => {
            atomic_place_ident_count(place, name) + expr_ident_count(value, name)
        }
        Expr::AtomicNew { value, .. } => expr_ident_count(value, name),
        Expr::AtomicCompareExchange {
            place,
            expected,
            desired,
            ..
        } => {
            atomic_place_ident_count(place, name)
                + expr_ident_count(expected, name)
                + expr_ident_count(desired, name)
        }
        Expr::Binary { lhs, rhs, .. } => expr_ident_count(lhs, name) + expr_ident_count(rhs, name),
        Expr::Range { start, end } => expr_ident_count(start, name) + expr_ident_count(end, name),
        Expr::Call { func, args, .. } => {
            expr_ident_count(func, name)
                + args
                    .iter()
                    .map(|arg| expr_ident_count(arg, name))
                    .sum::<usize>()
        }
        Expr::MethodCall { recv, args, .. } => {
            expr_ident_count(recv, name)
                + args
                    .iter()
                    .map(|arg| expr_ident_count(arg, name))
                    .sum::<usize>()
        }
        Expr::MethodCallGeneric { recv, args, .. } => {
            expr_ident_count(recv, name)
                + args
                    .iter()
                    .map(|arg| expr_ident_count(arg, name))
                    .sum::<usize>()
        }
        Expr::Field { base, field } => expr_ident_count(base, name) + ident_count(field, name),
        Expr::TupleField { base, .. } => expr_ident_count(base, name),
        Expr::ArrayPtr { array, .. } => expr_ident_count(array, name),
        Expr::Index { base, index } => expr_ident_count(base, name) + expr_ident_count(index, name),
        Expr::StructLit {
            name: type_name,
            fields,
        } => {
            ident_count(type_name, name)
                + fields
                    .iter()
                    .map(|(field, value)| ident_count(field, name) + expr_ident_count(value, name))
                    .sum::<usize>()
        }
        Expr::TupleStructLit {
            name: type_name,
            fields,
        } => {
            ident_count(type_name, name)
                + fields
                    .iter()
                    .map(|value| expr_ident_count(value, name))
                    .sum::<usize>()
        }
        Expr::ArrayLit(elems) => elems.iter().map(|elem| expr_ident_count(elem, name)).sum(),
        Expr::ArrayRepeat { elem, .. } => expr_ident_count(elem, name),
        Expr::VecLit(elems) => elems.iter().map(|elem| expr_ident_count(elem, name)).sum(),
        Expr::VecRepeat { elem, len } => expr_ident_count(elem, name) + expr_ident_count(len, name),
        Expr::Closure { params, body } => {
            if params.iter().any(|p| p.as_str() == name) {
                0
            } else {
                expr_ident_count(body, name)
            }
        }
        Expr::Macro {
            name: macro_name,
            args,
        } => {
            ident_count(macro_name, name)
                + args
                    .iter()
                    .map(|arg| expr_ident_count(arg, name))
                    .sum::<usize>()
        }
        Expr::Match { expr, arms } => {
            expr_ident_count(expr, name)
                + arms
                    .iter()
                    .map(|arm| {
                        pattern_ident_count(&arm.pattern, name) + expr_ident_count(&arm.value, name)
                    })
                    .sum::<usize>()
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            expr_ident_count(cond, name)
                + expr_ident_count(then_expr, name)
                + expr_ident_count(else_expr, name)
        }
    }
}

fn is_ident(s: &str) -> bool {
    let bytes = s.as_bytes();
    !bytes.is_empty() && is_ident_start(bytes[0]) && bytes.iter().all(|&b| is_ident_continue(b))
}

fn ident_count(line: &str, name: &str) -> usize {
    let bytes = line.as_bytes();
    let name_bytes = name.as_bytes();
    let mut count = 0;
    let mut i = 0;
    while i + name_bytes.len() <= bytes.len() {
        if &bytes[i..i + name_bytes.len()] == name_bytes
            && (i == 0 || !is_ident_continue(bytes[i - 1]))
            && (i + name_bytes.len() == bytes.len()
                || !is_ident_continue(bytes[i + name_bytes.len()]))
        {
            count += 1;
            i += name_bytes.len();
        } else {
            i += 1;
        }
    }
    count
}

fn is_ident_start(b: u8) -> bool {
    b == b'_' || b.is_ascii_alphabetic()
}

fn is_ident_continue(b: u8) -> bool {
    is_ident_start(b) || b.is_ascii_digit()
}
