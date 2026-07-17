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

use crate::fixups::facts::{FixupFacts, LazyInitSingletonFact};
use crate::fixups::idents::stmt_ident_count;
use crate::rust_ast::{
    BinOp, Expr, IndentStmt, Item, Prim, Program, RustValue, Stmt, Type, UnaryOp,
};

pub(in crate::fixups) fn collect_facts(program: &Program, facts: &mut FixupFacts) {
    facts.lazy_init_singletons.clear();
    let mut all = Vec::new();
    for (item_index, item) in program.items.iter().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        let Some((flag_name, payload_name, init_expr)) = match_body(&f.body) else {
            continue;
        };
        let Some(payload_ty) = static_item_type(program, &payload_name) else {
            continue;
        };
        if static_item_type(program, &flag_name).is_none() {
            continue;
        }
        if program_ident_count(program, &flag_name) != 2
            || program_ident_count(program, &payload_name) != 2
        {
            continue;
        }
        all.push(LazyInitSingletonFact {
            function,
            flag_name,
            payload_name,
            payload_ty,
            init_expr,
        });
    }
    facts.lazy_init_singletons = all;
}

/// Matches when `body` is exactly `[guard, return]`, where `guard` is either
/// a `Stmt::Scope` wrapping the three-statement guard-check shape or that
/// shape unwrapped at the top level, and `return` reads the payload the
/// guard just initialized.
fn match_body(body: &[IndentStmt]) -> Option<(String, String, Expr)> {
    let (guard, rest) = split_guard(body)?;
    let [check, cond, branch] = guard else {
        return None;
    };
    let (flag_temp, flag_name) = flag_read(&check.stmt)?;
    let cond_temp = cond_let_name(&cond.stmt, flag_temp)?;
    let then_body = if_on_var(&branch.stmt, cond_temp)?;
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
        && inner.len() == 3
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

fn static_item_type(program: &Program, name: &str) -> Option<Type> {
    program.items.iter().find_map(|item| match item {
        Item::Static {
            name: static_name,
            mutable: true,
            ty,
            ..
        } if static_name == name => Some(ty.clone()),
        _ => None,
    })
}

fn program_ident_count(program: &Program, name: &str) -> usize {
    program
        .items
        .iter()
        .map(|item| item_ident_count(item, name))
        .sum()
}

fn item_ident_count(item: &Item, name: &str) -> usize {
    match item {
        Item::Fn(f) => f
            .body
            .iter()
            .map(|indent| stmt_ident_count(&indent.stmt, name))
            .sum(),
        Item::Static {
            name: static_name,
            init,
            ..
        } => {
            if static_name == name {
                0
            } else {
                crate::fixups::idents::expr_ident_count(init, name)
            }
        }
        Item::Cfg { item, .. } => item_ident_count(item, name),
        _ => 0,
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::facts::{self, FunctionId};
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Item, Visibility};

    fn static_item(name: &str, ty: &str, init: Expr) -> Item {
        Item::Static {
            attrs: Vec::new(),
            vis: Visibility::Private,
            mutable: true,
            name: name.into(),
            ty: Type::parse(ty),
            init,
        }
    }

    fn qualifying_program() -> Program {
        Program {
            items: vec![
                static_item("cached_value", "i32", int(0)),
                static_item("computed", "i32", int(0)),
                Item::Fn({
                    let mut f = func(vec![], Some("i32"), Vec::new());
                    f.name = "get_value".into();
                    f.body = vec![
                        IndentStmt {
                            depth: 1,
                            stmt: Stmt::Scope {
                                body: vec![
                                    IndentStmt {
                                        depth: 2,
                                        stmt: temp_stmt(
                                            "_v0",
                                            "i32",
                                            Expr::Unsafe(Box::new(crate::rust_ast::Block {
                                                stmts: Vec::new(),
                                                tail: Some(Box::new(var("computed"))),
                                            })),
                                        ),
                                    },
                                    IndentStmt {
                                        depth: 2,
                                        stmt: temp_stmt(
                                            "_v1",
                                            "bool",
                                            Expr::Unary {
                                                op: UnaryOp::Not,
                                                expr: Box::new(bin(BinOp::Ne, var("_v0"), int(0))),
                                            },
                                        ),
                                    },
                                    IndentStmt {
                                        depth: 2,
                                        stmt: Stmt::If {
                                            cond: var("_v1"),
                                            then_body: vec![
                                                IndentStmt {
                                                    depth: 3,
                                                    stmt: temp_stmt(
                                                        "_v2",
                                                        "i32",
                                                        call("compute_value", vec![]),
                                                    ),
                                                },
                                                IndentStmt {
                                                    depth: 3,
                                                    stmt: unsafe_assign_stmt(
                                                        "cached_value",
                                                        var("_v2"),
                                                    ),
                                                },
                                                IndentStmt {
                                                    depth: 3,
                                                    stmt: unsafe_assign_stmt("computed", int(1)),
                                                },
                                            ],
                                            else_body: Vec::new(),
                                        },
                                    },
                                ],
                            },
                        },
                        IndentStmt {
                            depth: 1,
                            stmt: Stmt::Return(Some(Expr::Unsafe(Box::new(
                                crate::rust_ast::Block {
                                    stmts: Vec::new(),
                                    tail: Some(Box::new(var("cached_value"))),
                                },
                            )))),
                        },
                    ];
                    f
                }),
            ],
        }
    }

    fn temp_stmt(name: &str, ty: &str, init: Expr) -> Stmt {
        Stmt::Let {
            name: name.to_string(),
            mutable: false,
            ty: Some(Type::parse(ty)),
            init: Some(init),
        }
    }

    fn unsafe_assign_stmt(name: &str, value: Expr) -> Stmt {
        Stmt::Unsafe {
            body: crate::rust_ast::Block {
                stmts: vec![IndentStmt {
                    depth: 0,
                    stmt: Stmt::Assign {
                        target: var(name),
                        value,
                    },
                }],
                tail: None,
            },
        }
    }

    #[test]
    fn proves_the_guard_flag_and_payload_shape() {
        let program = qualifying_program();
        let analyzed = facts::analyze(program);
        assert_eq!(analyzed.facts.lazy_init_singletons.len(), 1);
        let fact = &analyzed.facts.lazy_init_singletons[0];
        assert_eq!(fact.function, FunctionId(0));
        assert_eq!(fact.flag_name, "computed");
        assert_eq!(fact.payload_name, "cached_value");
    }

    #[test]
    fn declines_when_payload_is_read_elsewhere() {
        let mut program = qualifying_program();
        program.items.push(Item::Fn({
            let mut f = func(
                vec![],
                Some("i32"),
                vec![Stmt::Return(Some(Expr::Unsafe(Box::new(
                    crate::rust_ast::Block {
                        stmts: Vec::new(),
                        tail: Some(Box::new(var("cached_value"))),
                    },
                ))))],
            );
            f.name = "peek_value".into();
            f
        }));

        let analyzed = facts::analyze(program);
        assert!(analyzed.facts.lazy_init_singletons.is_empty());
    }
}
