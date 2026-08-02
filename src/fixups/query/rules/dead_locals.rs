use crate::fixups::trace::Pass;
use crate::rust_ast::{Expr, Stmt};

use super::super::{EditSet, Field, Local, QueryRule, StatementSequence, Usage, Value};

pub(in crate::fixups) fn rewrite(pass: Pass) -> QueryRule<StatementSequence> {
    QueryRule::new(
        pass,
        "remove_dead_local",
        StatementSequence::new(1).starting_with(Local {
            value: Value {
                usage: Field::eq(Some(Usage {
                    reads: 0,
                    writes: 0,
                })),
                ..Default::default()
            },
            ..Default::default()
        }),
    )
    .case("dead_local", |case, matched| {
        let [stmt] = case.statements(matched)?;
        let Stmt::Let {
            init: Some(init), ..
        } = &stmt.stmt
        else {
            return Err(case.reject());
        };
        if !discardable_known_method(init) {
            let statement = matched.statement(0);
            let effects = case.fact(|query| query.statement_effects(&statement))?;
            case.require(effects.effects.is_empty())?;
        }
        Ok(EditSet::replace_statements(
            matched.target().clone(),
            Vec::new(),
        ))
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
