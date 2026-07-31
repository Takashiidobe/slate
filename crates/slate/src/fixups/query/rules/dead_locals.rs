use crate::fixups::trace::Pass;
use crate::rust_ast::{Expr, Stmt};

use super::super::StmtWindowRule;

pub(in crate::fixups) fn rewrite(pass: Pass) -> StmtWindowRule {
    StmtWindowRule::new(pass, "remove_dead_local", 1).case("dead_local", |case| {
        let [stmt] = case.stmts();
        let Stmt::Let {
            name,
            init: Some(init),
            ..
        } = &stmt.stmt
        else {
            return Err(case.reject());
        };
        case.dead_local(name)?;
        if !discardable_known_method(init) {
            case.no_effects()?;
        }
        Ok(Vec::new())
    })
}

fn discardable_known_method(expr: &Expr) -> bool {
    match expr {
        Expr::Cast { expr, .. } | Expr::Transmute { expr, .. } => discardable_known_method(expr),
        Expr::MethodCall { recv, method, args } if method == "len" && args.is_empty() => {
            discardable_receiver(recv)
        }
        _ => false,
    }
}

fn discardable_receiver(expr: &Expr) -> bool {
    match expr {
        Expr::Var(_) => true,
        Expr::MethodCall { recv, method, args }
            if matches!(method.as_str(), "as_slice" | "as_mut_slice") && args.is_empty() =>
        {
            discardable_receiver(recv)
        }
        _ => false,
    }
}
