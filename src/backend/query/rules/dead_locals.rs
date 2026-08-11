use crate::backend::facts::EffectKind;
use crate::backend::rust_ast::{Expr, Stmt};
use crate::backend::trace::Pass;

use super::super::{EditSet, Field, Local, QueryRule, StatementSequence, Usage, Value};

pub(in crate::backend) fn rewrite(pass: Pass) -> QueryRule<StatementSequence<1>> {
    QueryRule::new(
        pass,
        "remove_dead_local",
        StatementSequence::new().starting_with(Local {
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
            case.require(
                effects
                    .effects
                    .iter()
                    .all(|effect| matches!(effect, EffectKind::ReadOnlyCall)),
            )?;
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
