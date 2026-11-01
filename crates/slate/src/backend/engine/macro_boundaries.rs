use std::collections::HashSet;

use super::rules::walk::{child_exprs, child_exprs_mut, stmt_exprs, stmt_exprs_mut};
use crate::backend::rust_ast::{Expr, ImplItem, Item, Stmt};

pub fn reads_outside_arguments(stmts: &[Stmt], names: &HashSet<String>, level: u32) -> bool {
    stmts.iter().any(|stmt| {
        let mut exprs = Vec::new();
        stmt_exprs(stmt, &mut exprs);
        exprs.into_iter().any(|expr| expr_reads(expr, names, level))
    })
}

fn expr_reads(expr: &Expr, names: &HashSet<String>, level: u32) -> bool {
    match expr {
        Expr::MacroArg { level: found, .. } if *found == level => false,
        Expr::Var(name) => names.contains(name.as_str()),
        _ => child_exprs(expr)
            .into_iter()
            .any(|child| expr_reads(child, names, level)),
    }
}

pub fn strip_level(stmts: &mut [Stmt], level: u32) {
    for stmt in stmts {
        let mut exprs = Vec::new();
        stmt_exprs_mut(stmt, &mut exprs);
        for expr in exprs {
            strip_expr_level(expr, level);
        }
    }
}

fn strip_expr_level(expr: &mut Expr, level: u32) {
    while let Expr::MacroArg {
        level: found,
        expr: inner,
        ..
    } = expr
        && *found == level
    {
        let inner = std::mem::replace(&mut **inner, Expr::Str(String::new()));
        *expr = inner;
    }
    for child in child_exprs_mut(expr) {
        strip_expr_level(child, level);
    }
}

pub fn strip(stmts: &mut Vec<Stmt>) {
    for stmt in stmts.iter_mut() {
        let mut exprs = Vec::new();
        stmt_exprs_mut(stmt, &mut exprs);
        for expr in exprs {
            strip_expr(expr);
        }
    }
    flatten_regions(stmts);
}

pub(super) fn strip_item(item: &mut Item) {
    match item {
        Item::Fn(func) => strip(&mut func.body),
        Item::MacroRules { body, .. } => strip(body),
        Item::InlineMod { items, .. } => items.iter_mut().for_each(strip_item),
        Item::Impl(impl_block) => {
            for impl_item in &mut impl_block.items {
                if let ImplItem::Method(method) = impl_item
                    && let Expr::Block(block) = &mut method.body
                {
                    strip(&mut block.stmts);
                }
            }
        }
        _ => {}
    }
}

fn strip_expr(expr: &mut Expr) {
    while let Expr::MacroArg { expr: inner, .. } = expr {
        let inner = std::mem::replace(&mut **inner, Expr::Str(String::new()));
        *expr = inner;
    }
    if let Expr::Block(block) | Expr::Unsafe(block) = expr {
        flatten_regions(&mut block.stmts);
    }
    for child in child_exprs_mut(expr) {
        strip_expr(child);
    }
}

fn flatten_regions(stmts: &mut Vec<Stmt>) {
    let mut flattened = Vec::with_capacity(stmts.len());
    for mut stmt in std::mem::take(stmts) {
        for body in stmt.child_bodies_mut() {
            flatten_regions(body);
        }
        match stmt {
            Stmt::Scope {
                body,
                macro_region: Some(_),
            } => flattened.extend(body),
            other => flattened.push(other),
        }
    }
    *stmts = flattened;
}
