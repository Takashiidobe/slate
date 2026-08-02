use crate::fixups::trace::Pass;
use crate::function_identity::Known;
use crate::rust_ast::Type;

use super::super::{CallTarget, EditSet, ExternFn, Field, FnCall, QueryRule, process_exit};

pub(in crate::fixups) fn calls() -> QueryRule<FnCall> {
    QueryRule::new(
        Pass::LibcExit,
        "rewrite_libc_exit",
        FnCall {
            target: Field::eq(CallTarget::Known(Known::Exit)),
            arity: Field::eq(1),
            ..Default::default()
        },
    )
    .case("never_returning_extern", |case, call| {
        case.fact(|query| {
            query.extern_fn(&ExternFn {
                name: Field::eq(Known::Exit.symbol().into()),
                arity: Field::eq(1),
                returns: Field::eq(Some(Type::Never)),
            })
        })?;
        let replacement = case.lower_expr(process_exit(), &call.site)?;
        let target = call
            .trivial_unsafe_site
            .clone()
            .unwrap_or_else(|| call.site.clone());
        Ok(EditSet::replace_expression(target, replacement))
    })
}
