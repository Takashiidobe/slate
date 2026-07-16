use crate::fixups::idents::stmt_ident_count;
use crate::fixups::support::walk;
use crate::rust_ast::{Block, Expr, IndentStmt, Stmt};

pub(in crate::fixups) fn fixup(body: &mut Vec<IndentStmt>) -> bool {
    let mut changed = false;
    while fixup_once(body) {
        changed = true;
    }
    changed
}

fn fixup_once(body: &mut Vec<IndentStmt>) -> bool {
    if inline_nested_alias(body) {
        return true;
    }
    for def_index in 0..body.len() {
        let Some((alias, source)) = alias_def(&body[def_index].stmt) else {
            continue;
        };
        let Some(use_index) = single_later_use(body, def_index, &alias) else {
            continue;
        };
        if source_changes_between(body, def_index, use_index, &source)
            || stmt_declares_name(&body[use_index].stmt, &source)
        {
            continue;
        }
        if body[use_index]
            .stmt
            .substitute_var(&alias, &Expr::Var(source.into()))
        {
            body.remove(def_index);
            return true;
        }
    }
    false
}

fn inline_nested_alias(body: &mut [IndentStmt]) -> bool {
    for stmt in body {
        let mut changed = false;
        walk::nested_body_vecs_mut_with_path(&mut stmt.stmt, &mut Vec::new(), &mut |body, _| {
            if !changed && fixup_once(body) {
                changed = true;
            }
        });
        if changed {
            return true;
        }
    }
    false
}

fn alias_def(stmt: &Stmt) -> Option<(String, String)> {
    let Stmt::Let {
        name,
        mutable: false,
        init: Some(Expr::Var(source)),
        ..
    } = stmt
    else {
        return None;
    };
    if !is_temp_name(name) || is_temp_name(source.as_str()) {
        return None;
    }
    Some((name.clone(), source.as_str().to_string()))
}

fn single_later_use(body: &[IndentStmt], def_index: usize, alias: &str) -> Option<usize> {
    let mut use_index = None;
    let mut count = 0;
    for (index, indent) in body.iter().enumerate().skip(def_index + 1) {
        let stmt_count = stmt_ident_count(&indent.stmt, alias);
        if stmt_count == 0 {
            continue;
        }
        count += stmt_count;
        if use_index.is_none() {
            use_index = Some(index);
        }
    }
    (count == 1).then_some(use_index?)
}

fn source_changes_between(
    body: &[IndentStmt],
    def_index: usize,
    use_index: usize,
    source: &str,
) -> bool {
    body[def_index + 1..use_index].iter().any(|indent| {
        stmt_declares_name(&indent.stmt, source) || stmt_changes_name(&indent.stmt, source)
    })
}

fn stmt_declares_name(stmt: &Stmt, name: &str) -> bool {
    match stmt {
        Stmt::Let { name: binding, .. } | Stmt::LetIf { name: binding, .. } => binding == name,
        Stmt::For { pat, .. } => pat == name,
        Stmt::Match { arms, .. } => arms
            .iter()
            .any(|arm| pattern_declares_name(&arm.pattern, name)),
        _ => false,
    }
}

fn stmt_changes_name(stmt: &Stmt, name: &str) -> bool {
    match stmt {
        Stmt::Assign { target, .. } | Stmt::CompoundAssign { target, .. } => {
            expr_writes_name(target, name)
        }
        Stmt::Let { init, .. } => init
            .as_ref()
            .is_some_and(|expr| expr_mutably_borrows_name(expr, name)),
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            expr_mutably_borrows_name(cond, name)
                || body_changes_name(then_body, name)
                || expr_mutably_borrows_name(then_value, name)
                || body_changes_name(else_body, name)
                || expr_mutably_borrows_name(else_value, name)
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => expr_mutably_borrows_name(expr, name),
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            expr_mutably_borrows_name(cond, name)
                || body_changes_name(then_body, name)
                || body_changes_name(else_body, name)
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            body_changes_name(body, name)
        }
        Stmt::For { iter, body, .. } => {
            expr_mutably_borrows_name(iter, name) || body_changes_name(body, name)
        }
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
            block_changes_name(body, name)
        }
        Stmt::Match { expr, arms } => {
            expr_mutably_borrows_name(expr, name)
                || arms.iter().any(|arm| body_changes_name(&arm.body, name))
        }
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => false,
    }
}

fn body_changes_name(body: &[IndentStmt], name: &str) -> bool {
    body.iter()
        .any(|indent| stmt_changes_name(&indent.stmt, name))
}

fn block_changes_name(block: &Block, name: &str) -> bool {
    body_changes_name(&block.stmts, name)
        || block
            .tail
            .as_ref()
            .is_some_and(|tail| expr_mutably_borrows_name(tail, name))
}

fn expr_mutably_borrows_name(expr: &Expr, name: &str) -> bool {
    walk::expr_any(expr, &mut |expr| match expr {
        Expr::Ref {
            mutable: true,
            expr,
        }
        | Expr::AddrOf {
            mutable: true,
            expr,
        } => matches!(&**expr, Expr::Var(var) if var.as_str() == name),
        _ => false,
    })
}

fn expr_writes_name(expr: &Expr, name: &str) -> bool {
    match expr {
        Expr::Var(var) => var.as_str() == name,
        Expr::Unary { expr, .. }
        | Expr::Field { base: expr, .. }
        | Expr::TupleField { base: expr, .. }
        | Expr::ArrayPtr { array: expr, .. } => expr_writes_name(expr, name),
        Expr::Index { base, .. } => expr_writes_name(base, name),
        _ => false,
    }
}

fn pattern_declares_name(pattern: &crate::rust_ast::Pattern, name: &str) -> bool {
    match pattern {
        crate::rust_ast::Pattern::Binding(binding) => binding.as_str() == name,
        crate::rust_ast::Pattern::TupleStruct { fields, .. } => fields
            .iter()
            .any(|field| pattern_declares_name(field, name)),
        crate::rust_ast::Pattern::Wildcard
        | crate::rust_ast::Pattern::I64(_)
        | crate::rust_ast::Pattern::I128(_) => false,
    }
}

fn is_temp_name(name: &str) -> bool {
    name.strip_prefix("_v")
        .is_some_and(|rest| !rest.is_empty() && rest.bytes().all(|b| b.is_ascii_digit()))
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Expr, RustValue, Stmt};

    fn run(stmts: Vec<Stmt>) -> String {
        after_body(
            |body| {
                fixup(body);
            },
            vec![],
            None,
            stmts,
        )
    }

    fn println_arg(expr: Expr) -> Stmt {
        Stmt::Expr(Expr::Macro {
            name: "println".into(),
            args: vec![Expr::Str("{}".into()), expr],
        })
    }

    #[test]
    fn inlines_single_use_var_alias_into_macro_arg() {
        let out = run(vec![
            temp("loaded", "i32", int(7)),
            temp("_v30", "i32", var("loaded")),
            println_arg(var("_v30")),
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let loaded: i32 = 7;
    println!(\"{}\", loaded);
}
"
        );
    }

    #[test]
    fn skips_alias_when_source_is_reassigned_before_use() {
        let out = run(vec![
            let_mut("expected", "i32", int(7)),
            temp("_v30", "i32", var("expected")),
            assign("expected", int(9)),
            println_arg(var("_v30")),
        ]);

        assert!(out.contains("let _v30: i32 = expected;"), "{out}");
    }

    #[test]
    fn skips_alias_when_source_is_reassigned_in_nested_body_before_use() {
        let out = run(vec![
            let_mut("expected", "i32", int(7)),
            temp("_v30", "i32", var("expected")),
            Stmt::If {
                cond: Expr::Value(RustValue::Bool(true)),
                then_body: vec![IndentStmt {
                    depth: 2,
                    stmt: assign("expected", int(9)),
                }],
                else_body: vec![],
            },
            println_arg(var("_v30")),
        ]);

        assert!(out.contains("let _v30: i32 = expected;"), "{out}");
    }

    #[test]
    fn skips_alias_when_source_is_mutably_borrowed_before_use() {
        let out = run(vec![
            let_mut("expected", "i32", int(7)),
            temp("_v30", "i32", var("expected")),
            Stmt::Expr(call(
                "touch",
                vec![Expr::Ref {
                    mutable: true,
                    expr: Box::new(var("expected")),
                }],
            )),
            println_arg(var("_v30")),
        ]);

        assert!(out.contains("let _v30: i32 = expected;"), "{out}");
    }

    #[test]
    fn skips_non_temp_aliases() {
        let out = run(vec![
            temp("loaded", "i32", int(7)),
            temp("alias", "i32", var("loaded")),
            println_arg(var("alias")),
        ]);

        assert!(out.contains("let alias: i32 = loaded;"), "{out}");
    }
}
