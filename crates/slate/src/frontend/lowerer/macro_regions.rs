use std::collections::HashSet;

use slate_parser::ast::{MacroInvocationId, Span};
use slate_parser::ir;

use super::control_flow;
use super::statements::collect_lets;
use super::*;
use crate::backend::engine::macro_boundaries;

fn fits_region(statement: &ir::Statement) -> bool {
    !matches!(
        statement,
        ir::Statement::Comment(_) | ir::Statement::Temporary { .. } | ir::Statement::Let(_)
    ) && transfers_stay_inside(statement, false)
}

fn transfers_stay_inside(statement: &ir::Statement, in_loop: bool) -> bool {
    let in_loop = in_loop
        || matches!(
            statement,
            ir::Statement::While { .. }
                | ir::Statement::DoWhile { .. }
                | ir::Statement::For { .. }
                | ir::Statement::Switch { .. }
        );
    match statement {
        ir::Statement::Return(_)
        | ir::Statement::Goto(_)
        | ir::Statement::ComputedGoto(_)
        | ir::Statement::Asm(_)
        | ir::Statement::Label { .. }
        | ir::Statement::Case { .. }
        | ir::Statement::Default { .. } => false,
        ir::Statement::Break(_) | ir::Statement::Continue(_) => in_loop,
        _ => control_flow::children(statement)
            .into_iter()
            .flatten()
            .all(|child| transfers_stay_inside(&child.value, in_loop)),
    }
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn function_locals(
        &self,
        params: &[FnParam],
        body: &[Span<ir::Statement>],
    ) -> HashSet<String> {
        let mut lets = Vec::new();
        collect_lets(body, &mut lets, &mut HashSet::new());
        lets.into_iter()
            .map(|id| self.binding_name(id))
            .chain(params.iter().map(|param| param.name.clone()))
            .collect()
    }

    pub(super) fn macro_region_of(
        &self,
        statement: &Span<ir::Statement>,
    ) -> Option<MacroInvocationId> {
        if !self.tables.named_c_types || self.macro_region.is_some() || self.in_macro_argument {
            return None;
        }
        let site = self.tables.macros.sites(statement.expansion).pop()?;
        (site.argument.is_none() && fits_region(&statement.value)).then_some(site.invocation)
    }

    pub(super) fn lower_macro_region(
        &mut self,
        invocation: MacroInvocationId,
        statements: &[Span<ir::Statement>],
    ) -> Result<Vec<Stmt>> {
        self.macro_region = Some(invocation);
        let lowered = self.lower_statement_list(statements);
        self.macro_region = None;
        let mut body = lowered?;
        let mut declared = Vec::new();
        collect_lets(statements, &mut declared, &mut HashSet::new());
        let mut caller_locals = self.function_locals.clone();
        for id in declared {
            caller_locals.remove(&self.binding_name(id));
        }
        if macro_boundaries::reads_outside_arguments(&body, &caller_locals) {
            macro_boundaries::strip(&mut body);
            return Ok(body);
        }
        let macros = self.tables.macros;
        let name = macros
            .definition(macros.invocation(invocation))
            .name
            .to_string();
        Ok(vec![Stmt::Scope {
            body,
            macro_region: Some(name),
        }])
    }

    pub(super) fn macro_argument_entered(&self, value: &ir::Value) -> Option<u32> {
        let region = self.macro_region.filter(|_| !self.in_macro_argument)?;
        self.tables
            .macros
            .sites(value.node.expansion)
            .into_iter()
            .find(|site| site.invocation == region)?
            .argument
            .map(|argument| argument as u32)
    }
}
