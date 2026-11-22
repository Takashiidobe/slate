use super::rules::walk::child_exprs;
use crate::backend::rust_ast::{
    AsmOperand, AtomicPlace, Block, Expr, Item, Pointee, Program, Stmt, UnaryOp,
};

pub(super) fn needs_unsafe(expr: &Expr) -> bool {
    match expr {
        Expr::Unary {
            op: UnaryOp::Deref(Pointee::Raw),
            ..
        }
        | Expr::Transmute { .. }
        | Expr::CopyNonoverlapping { .. }
        | Expr::PtrCopy { .. }
        | Expr::WriteBytes { .. } => true,
        Expr::Var { unsafe_, .. }
        | Expr::Call { unsafe_, .. }
        | Expr::MethodCall { unsafe_, .. }
        | Expr::MethodCallGeneric { unsafe_, .. } => unsafe_.0,
        Expr::Field { union, .. } => union.0,
        Expr::AtomicRef { place, .. }
        | Expr::AtomicLoad { place, .. }
        | Expr::AtomicStore { place, .. }
        | Expr::AtomicFetch { place, .. }
        | Expr::AtomicSwap { place, .. }
        | Expr::AtomicCompareExchange { place, .. } => matches!(place, AtomicPlace::Ptr(_)),
        _ => false,
    }
}

enum Part<'a> {
    Expr(&'a Expr),
    Stmts(&'a [Stmt]),
    Block(&'a Block),
}

fn parts(stmt: &Stmt) -> Vec<Part<'_>> {
    match stmt {
        Stmt::Cfg { body, .. }
        | Stmt::Loop { body, .. }
        | Stmt::Scope { body }
        | Stmt::LabeledBlock { body, .. } => vec![Part::Stmts(body)],
        Stmt::Let {
            init: Some(expr), ..
        }
        | Stmt::Expr(expr)
        | Stmt::Return(Some(expr)) => vec![Part::Expr(expr)],
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            vec![Part::Expr(target), Part::Expr(value)]
        }
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => vec![
            Part::Expr(cond),
            Part::Stmts(then_body),
            Part::Expr(then_value),
            Part::Stmts(else_body),
            Part::Expr(else_value),
        ],
        Stmt::InlineAsm(asm) => asm
            .operands
            .iter()
            .flat_map(|operand| match operand {
                AsmOperand::In { value, .. }
                | AsmOperand::Out { value, .. }
                | AsmOperand::Const(value)
                | AsmOperand::Sym(value) => vec![Part::Expr(value)],
                AsmOperand::InOut { input, output, .. } => {
                    vec![Part::Expr(input), Part::Expr(output)]
                }
                AsmOperand::Label { state, value } => vec![Part::Expr(state), Part::Expr(value)],
            })
            .collect(),
        Stmt::Unsafe { body } | Stmt::Block(body) => vec![Part::Block(body)],
        Stmt::While { cond, body, .. } => vec![Part::Expr(cond), Part::Block(body)],
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => vec![
            Part::Expr(cond),
            Part::Stmts(then_body),
            Part::Stmts(else_body),
        ],
        Stmt::For { iter, body, .. } => vec![Part::Expr(iter), Part::Stmts(body)],
        Stmt::Match { expr, arms } => std::iter::once(Part::Expr(expr))
            .chain(arms.iter().map(|arm| Part::Stmts(&arm.body)))
            .collect(),
        Stmt::Let { init: None, .. }
        | Stmt::Return(None)
        | Stmt::Comment(_)
        | Stmt::Break(_)
        | Stmt::Continue(_) => Vec::new(),
    }
}

fn expr_marked(expr: &Expr) -> bool {
    needs_unsafe(expr)
        || match expr {
            Expr::Block(block) | Expr::Unsafe(block) => block_marked(block),
            other => child_exprs(other).into_iter().any(expr_marked),
        }
}

fn block_marked(block: &Block) -> bool {
    block.stmts.iter().any(stmt_marked) || block.tail.as_deref().is_some_and(expr_marked)
}

fn stmt_marked(stmt: &Stmt) -> bool {
    matches!(stmt, Stmt::InlineAsm(_))
        || parts(stmt).into_iter().any(|part| match part {
            Part::Expr(expr) => expr_marked(expr),
            Part::Stmts(stmts) => stmts.iter().any(stmt_marked),
            Part::Block(block) => block_marked(block),
        })
}

fn check_expr(expr: &Expr, function: &str) {
    match expr {
        Expr::Block(block) | Expr::Unsafe(block) => {
            if matches!(expr, Expr::Unsafe(_)) && !block_marked(block) {
                eprintln!("unmarked unsafe in {function}: {}", expr.render());
            }
            check_block(block, function);
        }
        other => {
            for child in child_exprs(other) {
                check_expr(child, function);
            }
        }
    }
}

fn check_block(block: &Block, function: &str) {
    for stmt in &block.stmts {
        check_stmt(stmt, function);
    }
    if let Some(tail) = &block.tail {
        check_expr(tail, function);
    }
}

fn check_stmt(stmt: &Stmt, function: &str) {
    if let Stmt::Unsafe { body } = stmt
        && !block_marked(body)
    {
        eprintln!("unmarked unsafe in {function}: {}", stmt.render());
    }
    for part in parts(stmt) {
        match part {
            Part::Expr(expr) => check_expr(expr, function),
            Part::Stmts(stmts) => {
                for stmt in stmts {
                    check_stmt(stmt, function);
                }
            }
            Part::Block(block) => check_block(block, function),
        }
    }
}

pub(super) fn check(program: &Program) {
    if std::env::var_os("SLATE_UNSAFE_MARK_CHECK").is_none() {
        return;
    }
    for item in &program.items {
        if let Item::Fn(func) = item {
            for stmt in &func.body {
                check_stmt(stmt, &func.name);
            }
        }
    }
}
