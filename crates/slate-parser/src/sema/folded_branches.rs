use super::asm_constants::JumpTargets;
use super::expression::Lowerer;
use crate::ir::*;
use num_bigint::Sign;

struct FoldedBranches<'a> {
    lower: &'a Lowerer,
}

impl MacroStructureVisitor for FoldedBranches<'_> {
    fn rewrite_statement(&mut self, statement: &mut Statement) {
        match statement {
            Statement::If {
                condition,
                then_body,
                else_body,
                folded,
            } => {
                let Some(taken) = self.lower.folded_condition(condition) else {
                    return;
                };
                let enterable = if taken {
                    else_body.as_mut().is_some_and(JumpTargets::of_statements)
                } else {
                    JumpTargets::of_statements(then_body)
                };
                if !enterable {
                    *folded = Some(taken);
                }
            }
            Statement::Switch {
                id,
                discriminant,
                body,
                folded,
            } => {
                let Some(value) = self.lower.folded_integer(discriminant) else {
                    return;
                };
                if JumpTargets::of_switch(*id, body) {
                    return;
                }
                *folded = Some(match value.sign() {
                    Sign::Minus => Number::SignedInteger(value),
                    Sign::NoSign | Sign::Plus => Number::Integer(value.magnitude().clone()),
                });
            }
            _ => {}
        }
    }
}

impl Lowerer {
    pub(super) fn mark_folded_branches(&mut self) {
        for index in 0..self.module.functions.len() {
            let Some(mut body) = self.module.functions[index].value.body.take() else {
                continue;
            };
            walk_macro_structure(&mut body, &mut FoldedBranches { lower: self });
            self.module.functions[index].value.body = Some(body);
        }
    }
}
