use std::collections::HashSet;

use slate_parser::ast::{MacroDefinition, MacroInvocationId, MacroInvocations, Span};
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

fn touches(
    macros: &MacroInvocations,
    invocation: MacroInvocationId,
    statement: Option<&Span<ir::Statement>>,
) -> bool {
    let site = macros.invocation(invocation).site;
    statement.is_some_and(|statement| {
        let loc = statement.expansion;
        loc.file == site.file
            && loc.offset < site.offset + site.length
            && site.offset < loc.offset + loc.length
    })
}

fn spelled_in_body(
    macros: &MacroInvocations,
    invocation: MacroInvocationId,
    statement: &Span<ir::Statement>,
) -> bool {
    if matches!(statement.value, ir::Statement::Comment(_)) {
        return true;
    }
    let sites = macros.sites(statement.expansion);
    let Some(site) = sites.iter().find(|site| site.invocation == invocation) else {
        return false;
    };
    if site.argument.is_some() {
        return matches!(
            statement.value,
            ir::Statement::Let(_) | ir::Statement::Temporary { .. }
        );
    }
    control_flow::children(&statement.value)
        .into_iter()
        .flatten()
        .all(|child| spelled_in_body(macros, invocation, child))
}

fn declares_local(statement: &ir::Statement) -> bool {
    matches!(statement, ir::Statement::Let(_))
        || control_flow::children(statement)
            .into_iter()
            .flatten()
            .any(|child| declares_local(&child.value))
}

pub(super) fn is_whole_macro_block(
    macros: &MacroInvocations,
    statement: &Span<ir::Statement>,
) -> bool {
    let Some(site) = macros.sites(statement.expansion).pop() else {
        return false;
    };
    let replacement = macros
        .definition(macros.invocation(site.invocation))
        .replacement
        .trim();
    site.argument.is_none()
        && matches!(statement.value, ir::Statement::Block(_))
        && replacement.starts_with('{')
        && replacement.ends_with('}')
        && fits_region(&statement.value)
        && !declares_local(&statement.value)
}

fn macro_params(definition: &MacroDefinition) -> Option<Vec<rust::MacroParam>> {
    let parameters = definition.parameters.as_ref()?;
    let replacement = &definition.replacement;
    if definition.variadic || replacement.contains('#') {
        return None;
    }
    let mut shapes = vec![rust::MacroParamShape::Unused; parameters.len()];
    let mut callee_before = false;
    let mut chars = replacement.char_indices().peekable();
    while let Some((start, c)) = chars.next() {
        if c == '"' || c == '\'' {
            let mut escaped = false;
            for (_, next) in chars.by_ref() {
                if !escaped && next == c {
                    break;
                }
                escaped = !escaped && next == '\\';
            }
            callee_before = false;
        } else if c.is_alphanumeric() || c == '_' {
            let mut end = start + c.len_utf8();
            while let Some(&(at, next)) = chars.peek() {
                if !next.is_alphanumeric() && next != '_' {
                    break;
                }
                end = at + next.len_utf8();
                chars.next();
            }
            let word = &replacement[start..end];
            if let Some(index) = parameters.iter().position(|name| name == word) {
                shapes[index] = if callee_before || shapes[index] == rust::MacroParamShape::List {
                    rust::MacroParamShape::List
                } else {
                    rust::MacroParamShape::Expr
                };
            }
            callee_before = !matches!(word, "return" | "else" | "do" | "case" | "sizeof" | "goto");
        } else if !c.is_whitespace() {
            callee_before = false;
        }
    }
    if parameters.iter().any(|name| name == "crate") {
        return None;
    }
    Some(
        parameters
            .iter()
            .zip(shapes)
            .map(|(name, shape)| rust::MacroParam {
                name: name.clone(),
                shape,
            })
            .collect(),
    )
}

pub(super) fn merge_operator_spelled_in_argument(value: &ir::Value, lowered: Expr) -> Expr {
    let ValueKind::Arith { left, right, .. } = &value.node.value else {
        return lowered;
    };
    let (left, right) = (left.node.expansion, right.node.expansion);
    if left.file != right.file || left.end() > right.offset {
        return lowered;
    }
    let Expr::Binary { op, lhs, rhs } = lowered else {
        return lowered;
    };
    match (*lhs, *rhs) {
        (
            Expr::MacroArg {
                index,
                expr: left_value,
            },
            Expr::MacroArg {
                index: right_index,
                expr: right_value,
            },
        ) if index == right_index => Expr::MacroArg {
            index,
            expr: Box::new(Expr::Binary {
                op,
                lhs: left_value,
                rhs: right_value,
            }),
        },
        (lhs, rhs) => Expr::Binary {
            op,
            lhs: Box::new(lhs),
            rhs: Box::new(rhs),
        },
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

    pub(super) fn lower_lone_statement(
        &mut self,
        statement: &Span<ir::Statement>,
        before: Option<&Span<ir::Statement>>,
        after: Option<&Span<ir::Statement>>,
    ) -> Result<Vec<Stmt>> {
        match self.macro_region_of(statement) {
            Some(invocation) => {
                self.lower_macro_region(invocation, std::slice::from_ref(statement), before, after)
            }
            None => self.lower_statement(statement),
        }
    }

    pub(super) fn lower_graph_statement(
        &mut self,
        statement: &Span<ir::Statement>,
    ) -> Result<Vec<Stmt>> {
        if is_whole_macro_block(self.tables.macros, statement) {
            self.lower_lone_statement(statement, None, None)
        } else {
            self.lower_statement(statement)
        }
    }

    pub(super) fn lower_macro_region(
        &mut self,
        invocation: MacroInvocationId,
        statements: &[Span<ir::Statement>],
        before: Option<&Span<ir::Statement>>,
        after: Option<&Span<ir::Statement>>,
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
        let definition = macros.definition(macros.invocation(invocation));
        let params = macro_params(definition);
        let foldable = params.is_some()
            && !touches(self.tables.macros, invocation, before)
            && !touches(self.tables.macros, invocation, after)
            && statements
                .iter()
                .all(|statement| spelled_in_body(self.tables.macros, invocation, statement));
        Ok(vec![Stmt::Scope {
            body,
            macro_region: Some(Box::new(rust::MacroRegion {
                name: definition.name.to_string(),
                params: params.unwrap_or_default(),
                foldable,
                mentions: definition
                    .replacement
                    .split(|c: char| !c.is_alphanumeric() && c != '_')
                    .filter(|word| !word.is_empty())
                    .map(str::to_string)
                    .collect(),
            })),
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
