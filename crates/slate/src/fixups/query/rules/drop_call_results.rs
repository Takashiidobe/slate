use crate::fixups::facts::EffectKind;
use crate::fixups::trace::Pass;
use crate::rust_ast::Stmt;

use super::super::{
    Binding, BindingAccess, BindingCategory, BindingRef, EditSet, Field, ItemCaseContext,
    QueryRule, Rejection,
};

fn matcher() -> Binding {
    Binding {
        kind: Field::eq(BindingCategory::Local),
        name: Field::predicate(|name: &String, _| is_temp_name(name)),
        ..Default::default()
    }
}

pub(in crate::fixups) fn rewrite() -> QueryRule<Binding> {
    QueryRule::new(
        Pass::DropCallResults,
        "drop_unused_call_result_binding",
        matcher(),
    )
    .case("unused_call_result", drop_case)
}

fn drop_case(
    case: &mut ItemCaseContext<'_, '_>,
    binding: &BindingRef,
) -> Result<EditSet, Rejection> {
    let uses = case.fact(|query| query.binding_uses(binding))?;
    case.require(
        !uses
            .uses
            .iter()
            .any(|usage| matches!(usage.access, BindingAccess::Read | BindingAccess::ReadWrite)),
    )?;
    let initializer = case.fact(|query| query.binding_initializer(binding))?;
    let effects = case.fact(|query| query.expression_effects(&initializer))?;
    case.require(effects.effects.iter().any(|effect| {
        matches!(
            effect,
            EffectKind::ReadOnlyCall
                | EffectKind::UnknownCall
                | EffectKind::MethodCall
                | EffectKind::MacroExpansion
        )
    }))?;
    let init = case
        .expr(&initializer.site)
        .cloned()
        .ok_or_else(|| case.reject())?;
    let mut edits = EditSet::new();
    edits.push_replace_statement(
        binding.item_index,
        binding.definition.clone(),
        Some(Stmt::Expr(init)),
    );
    Ok(edits)
}

fn is_temp_name(name: &str) -> bool {
    name.strip_prefix("_v")
        .is_some_and(|rest| !rest.is_empty() && rest.bytes().all(|byte| byte.is_ascii_digit()))
}
