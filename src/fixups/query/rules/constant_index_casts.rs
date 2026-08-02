use crate::fixups::trace::Pass;
use crate::rust_ast::{Expr, Prim, RustValue, Type};

use super::super::{
    EditSet, ExprPattern, ExpressionKind, ExpressionRef, Field, ItemCaseContext, Predicate,
    QueryRule, Rejection, RejectionReason,
};

pub(in crate::fixups) fn rewrite() -> QueryRule<ExprPattern> {
    QueryRule::new(
        Pass::ConstantIndexCasts,
        "strip_constant_index_cast",
        ExprPattern {
            kind: Field::eq(ExpressionKind::Index),
            ..Default::default()
        },
    )
    .case("nonnegative_integer_literal", rewrite_index)
}

fn rewrite_index(
    case: &mut ItemCaseContext<'_, '_>,
    expression: &ExpressionRef,
) -> Result<EditSet, Rejection> {
    let Some(Expr::Index { base, index }) = case.expr(&expression.site).cloned() else {
        return Err(case.reject_at(
            Predicate::Expression,
            &expression.site,
            RejectionReason::UnsupportedShape,
        ));
    };
    let Expr::Cast { expr: inner, ty } = *index else {
        return Err(case.reject_at(
            Predicate::Cast,
            &expression.site,
            RejectionReason::UnsupportedShape,
        ));
    };
    if !matches!(ty, Type::Prim(Prim::Usize)) || !is_nonnegative_integer_literal(&inner) {
        return Err(case.reject_at(
            Predicate::Cast,
            &expression.site,
            RejectionReason::Contradicted,
        ));
    }
    Ok(EditSet::replace_expression(
        expression.site.clone(),
        Expr::Index { base, index: inner },
    ))
}

fn is_nonnegative_integer_literal(expression: &Expr) -> bool {
    match expression {
        Expr::Value(RustValue::I64(value)) => *value >= 0,
        Expr::Value(RustValue::I128(value)) => *value >= 0,
        _ => false,
    }
}
