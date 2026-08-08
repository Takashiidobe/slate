//! Prove a function's entire body is the "static local guarded by an
//! initialized flag" lazy-singleton idiom:
//!
//! ```c
//! static int cached_value = 0;
//! static int computed = 0;
//! int get_value(void) {
//!     if (!computed) { cached_value = compute(); computed = 1; }
//!     return cached_value;
//! }
//! ```
//!
//! Baseline lowering keeps `computed`/`cached_value` as `static mut` with
//! unsafe reads/writes. This fact fires only when `computed` (the flag) is
//! written exactly once, to a nonzero constant, inside the guard, and read
//! exactly once, as the guard condition; and `cached_value` (the payload) is
//! written only inside that same guard and read only by the function's
//! trailing return. Both counts are checked program-wide, so any other use of
//! either static anywhere leaves the function on the baseline path.

use std::collections::BTreeMap;

use crate::fixups::facts::walk::Bodies;
use crate::fixups::facts::{FunctionId, LazyInitSingletonFact, StaticDeclFact};
use crate::fixups::idents::stmt_ident_count;
use crate::rust_ast::{BinOp, Expr, IndentStmt, Prim, RustValue, Stmt, Type, UnaryOp};
pub(in crate::fixups) fn compute<'db>(
    shapes: &BTreeMap<FunctionId<'db>, (String, String, Expr)>,
    bodies: &Bodies,
    statics: &[StaticDeclFact],
) -> Vec<LazyInitSingletonFact<'db>> {
    let mut all = Vec::new();
    for (&function, (flag_name, payload_name, init_expr)) in shapes {
        let Some(payload_ty) = static_item_type(statics, payload_name) else {
            continue;
        };
        if static_item_type(statics, flag_name).is_none() {
            continue;
        }
        if program_ident_count(bodies, statics, flag_name) != 2
            || program_ident_count(bodies, statics, payload_name) != 2
        {
            continue;
        }
        all.push(LazyInitSingletonFact {
            function,
            flag_name: flag_name.clone(),
            payload_name: payload_name.clone(),
            payload_ty,
            init_expr: init_expr.clone(),
        });
    }
    all
}

/// Matches when `body` is exactly `[guard, return]`, where `guard` is either
/// a `Stmt::Scope` wrapping the three-statement guard-check shape or that
/// shape unwrapped at the top level, and `return` reads the payload the
/// guard just initialized.
pub(in crate::fixups) fn match_body(body: &[IndentStmt]) -> Option<(String, String, Expr)> {
    let (guard, rest) = split_guard(body)?;
    let (flag_name, then_body) = match guard {
        [check, cond, branch] => {
            let (flag_temp, flag_name) = flag_read(&check.stmt)?;
            let cond_temp = cond_let_name(&cond.stmt, flag_temp)?;
            (flag_name, if_on_var(&branch.stmt, cond_temp)?)
        }
        [cond, branch] => {
            let (cond_temp, flag_name) = cond_let_name_with_flag_read(&cond.stmt)?;
            (flag_name, if_on_var(&branch.stmt, cond_temp)?)
        }
        _ => return None,
    };
    let (payload_name, init_expr) = then_body_shape(then_body, &flag_name)?;
    let [ret] = rest else {
        return None;
    };
    if !return_reads(&ret.stmt, &payload_name) {
        return None;
    }
    Some((flag_name, payload_name, init_expr))
}

fn split_guard(body: &[IndentStmt]) -> Option<(&[IndentStmt], &[IndentStmt])> {
    if let [first, rest @ ..] = body
        && let Stmt::Scope { body: inner } = &first.stmt
        && matches!(inner.len(), 2 | 3)
    {
        return Some((inner.as_slice(), rest));
    }
    if body.len() >= 4 {
        return Some((&body[..3], &body[3..]));
    }
    None
}

/// `let <temp>: <int> = unsafe { <flag> };`
fn flag_read(stmt: &Stmt) -> Option<(&str, String)> {
    let Stmt::Let {
        name: temp,
        ty: Some(Type::Prim(_)),
        init: Some(init),
        ..
    } = stmt
    else {
        return None;
    };
    let Expr::Unsafe(block) = init else {
        return None;
    };
    if !block.stmts.is_empty() {
        return None;
    }
    let Some(tail) = &block.tail else {
        return None;
    };
    let Expr::Var(flag) = tail.as_ref() else {
        return None;
    };
    Some((temp.as_str(), flag.as_str().to_string()))
}

/// `let <temp>: bool = !(<flag_temp> != 0);` or `<flag_temp> == 0`.
fn cond_let_name<'a>(stmt: &'a Stmt, flag_temp: &str) -> Option<&'a str> {
    let Stmt::Let {
        name: temp,
        ty: Some(Type::Prim(Prim::Bool)),
        init: Some(cond),
        ..
    } = stmt
    else {
        return None;
    };
    zero_test_matches(cond, flag_temp).then_some(temp.as_str())
}

fn cond_let_name_with_flag_read(stmt: &Stmt) -> Option<(&str, String)> {
    let Stmt::Let {
        name: temp,
        ty: Some(Type::Prim(Prim::Bool)),
        init: Some(cond),
        ..
    } = stmt
    else {
        return None;
    };
    zero_test_flag_read(cond).map(|flag| (temp.as_str(), flag))
}

fn zero_test_flag_read(cond: &Expr) -> Option<String> {
    match cond {
        Expr::Unary {
            op: UnaryOp::Not,
            expr,
        } => match expr.as_ref() {
            Expr::Binary {
                op: BinOp::Ne,
                lhs,
                rhs,
            } => unsafe_static_read(lhs).filter(|_| is_zero(rhs)),
            _ => None,
        },
        Expr::Binary {
            op: BinOp::Eq,
            lhs,
            rhs,
        } => unsafe_static_read(lhs).filter(|_| is_zero(rhs)),
        _ => None,
    }
}

fn unsafe_static_read(expr: &Expr) -> Option<String> {
    let Expr::Unsafe(block) = expr else {
        return None;
    };
    if !block.stmts.is_empty() {
        return None;
    }
    let Some(tail) = &block.tail else {
        return None;
    };
    let Expr::Var(name) = tail.as_ref() else {
        return None;
    };
    Some(name.as_str().to_string())
}

fn zero_test_matches(cond: &Expr, flag_temp: &str) -> bool {
    match cond {
        Expr::Unary {
            op: UnaryOp::Not,
            expr,
        } => match expr.as_ref() {
            Expr::Binary {
                op: BinOp::Ne,
                lhs,
                rhs,
            } => is_var(lhs, flag_temp) && is_zero(rhs),
            _ => false,
        },
        Expr::Binary {
            op: BinOp::Eq,
            lhs,
            rhs,
        } => is_var(lhs, flag_temp) && is_zero(rhs),
        _ => false,
    }
}

/// `if <temp> { <then_body> }` with no `else`.
fn if_on_var<'a>(stmt: &'a Stmt, temp: &str) -> Option<&'a [IndentStmt]> {
    let Stmt::If {
        cond,
        then_body,
        else_body,
    } = stmt
    else {
        return None;
    };
    if !is_var(cond, temp) || !else_body.is_empty() {
        return None;
    }
    Some(then_body.as_slice())
}

/// Either `unsafe { payload = <expr>; } unsafe { flag = 1; }` directly, or
/// with the payload's init hoisted to its own temp first (the common shape
/// when the init is a call result):
/// `let t = <expr>; unsafe { payload = t; } unsafe { flag = 1; }`.
fn then_body_shape(then_body: &[IndentStmt], flag: &str) -> Option<(String, Expr)> {
    match then_body {
        [payload_write, flag_write] => {
            let (payload, value) = unsafe_assign(&payload_write.stmt)?;
            let (written_flag, one) = unsafe_assign(&flag_write.stmt)?;
            if written_flag != flag
                || !is_nonzero_const(one)
                || expr_references(value, payload)
                || expr_references(value, flag)
            {
                return None;
            }
            Some((payload.to_string(), value.clone()))
        }
        [let_stmt, payload_write, flag_write] => {
            let Stmt::Let {
                name: temp,
                init: Some(init_expr),
                ..
            } = &let_stmt.stmt
            else {
                return None;
            };
            let (payload, value) = unsafe_assign(&payload_write.stmt)?;
            if !is_var(value, temp) {
                return None;
            }
            let (written_flag, one) = unsafe_assign(&flag_write.stmt)?;
            if written_flag != flag
                || !is_nonzero_const(one)
                || expr_references(init_expr, payload)
                || expr_references(init_expr, flag)
            {
                return None;
            }
            Some((payload.to_string(), init_expr.clone()))
        }
        _ => None,
    }
}

/// `unsafe { <name> = <value>; }` with nothing else in the block.
fn unsafe_assign(stmt: &Stmt) -> Option<(&str, &Expr)> {
    let Stmt::Unsafe { body } = stmt else {
        return None;
    };
    if body.tail.is_some() {
        return None;
    }
    let [inner] = body.stmts.as_slice() else {
        return None;
    };
    let Stmt::Assign {
        target: Expr::Var(name),
        value,
    } = &inner.stmt
    else {
        return None;
    };
    Some((name.as_str(), value))
}

/// `return unsafe { <payload> };`
fn return_reads(stmt: &Stmt, payload: &str) -> bool {
    let Stmt::Return(Some(Expr::Unsafe(block))) = stmt else {
        return false;
    };
    if !block.stmts.is_empty() {
        return false;
    }
    let Some(tail) = &block.tail else {
        return false;
    };
    matches!(tail.as_ref(), Expr::Var(name) if name.as_str() == payload)
}

fn is_var(expr: &Expr, name: &str) -> bool {
    matches!(expr, Expr::Var(n) if n.as_str() == name)
}

fn is_zero(expr: &Expr) -> bool {
    matches!(
        expr,
        Expr::Value(RustValue::I64(0) | RustValue::I128(0) | RustValue::Usize(0))
    )
}

fn is_nonzero_const(expr: &Expr) -> bool {
    match expr {
        Expr::Value(RustValue::I64(n)) => *n != 0,
        Expr::Value(RustValue::I128(n)) => *n != 0,
        Expr::Value(RustValue::Usize(n)) => *n != 0,
        Expr::Value(RustValue::Bool(b)) => *b,
        _ => false,
    }
}

fn expr_references(expr: &Expr, name: &str) -> bool {
    crate::fixups::idents::expr_ident_count(expr, name) > 0
}

fn static_item_type(statics: &[StaticDeclFact], name: &str) -> Option<Type> {
    statics
        .iter()
        .find(|decl| decl.mutable && decl.name == name)
        .map(|decl| decl.ty.clone())
}

fn program_ident_count(bodies: &Bodies, statics: &[StaticDeclFact], name: &str) -> usize {
    let function_count: usize = bodies
        .values()
        .map(|f| {
            f.body
                .iter()
                .map(|indent| stmt_ident_count(&indent.stmt, name))
                .sum::<usize>()
        })
        .sum();
    let static_count: usize = statics
        .iter()
        .map(|decl| {
            if decl.name == name {
                0
            } else {
                crate::fixups::idents::expr_ident_count(&decl.init, name)
            }
        })
        .sum();
    function_count + static_count
}
