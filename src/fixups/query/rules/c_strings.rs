use crate::fixups::trace::Pass;
use crate::rust_ast::Expr;

use super::super::{
    EditSet, ExprPattern, ExpressionKind, ExpressionRef, Field, ItemCaseContext, Predicate,
    QueryRule, Rejection, RejectionReason,
};

pub(in crate::fixups) fn rewrite() -> QueryRule<ExprPattern> {
    QueryRule::new(
        Pass::CStrings,
        "rewrite_c_string_literal",
        ExprPattern {
            kind: Field::eq(ExpressionKind::Literal),
            ..Default::default()
        },
    )
    .case("recognized_receiver", rewrite_literal)
}

fn rewrite_literal(
    case: &mut ItemCaseContext<'_, '_>,
    expression: &ExpressionRef,
) -> Result<EditSet, Rejection> {
    let site = &expression.site;
    if !matches!(case.expr(site), Some(Expr::ByteStr(_))) {
        return Err(case.reject_at(
            Predicate::CStringLiteral,
            site,
            RejectionReason::UnsupportedShape,
        ));
    }
    let bytes = case.fact(|query| query.c_string_literal(site))?;
    Ok(EditSet::replace_expression(site.clone(), Expr::CStr(bytes)))
}
