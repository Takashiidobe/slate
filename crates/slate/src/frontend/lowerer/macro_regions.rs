use std::collections::HashSet;

use slate_parser::ast::{
    MacroDefinition, MacroInvocationId, MacroInvocations, NestedExpansionId, Span,
};
use slate_parser::ir;

use super::control_flow;
use super::statements::collect_lets;
use super::*;
use crate::backend::engine::macro_boundaries;

#[derive(Clone, Copy, PartialEq, Eq)]
pub(super) enum MacroRegionId {
    Source(MacroInvocationId),
    Nested(NestedExpansionId),
}

impl MacroRegionId {
    fn definition(self, macros: &MacroInvocations) -> &MacroDefinition {
        match self {
            Self::Source(id) => macros.definition(macros.invocation(id)),
            Self::Nested(id) => macros.nested_definition(id),
        }
    }

    fn site_of<T>(self, macros: &MacroInvocations, span: &Span<T>) -> Option<Option<usize>> {
        match self {
            Self::Source(id) => macros
                .sites(span.expansion)
                .into_iter()
                .find(|site| site.invocation == id)
                .map(|site| site.argument),
            Self::Nested(id) => span
                .macro_nesting
                .as_ref()?
                .site_of(id)
                .map(|link| link.argument),
        }
    }

    fn touches(self, macros: &MacroInvocations, statement: Option<&Span<ir::Statement>>) -> bool {
        match self {
            Self::Source(id) => touches(macros, id, statement),
            Self::Nested(_) => {
                statement.is_some_and(|statement| self.site_of(macros, statement).is_some())
            }
        }
    }
}

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
    region: MacroRegionId,
    statement: &Span<ir::Statement>,
) -> bool {
    if matches!(statement.value, ir::Statement::Comment(_)) {
        return true;
    }
    let Some(argument) = region.site_of(macros, statement) else {
        return false;
    };
    if argument.is_some() {
        return matches!(
            statement.value,
            ir::Statement::Let(_) | ir::Statement::Temporary { .. }
        );
    }
    control_flow::children(&statement.value)
        .into_iter()
        .flatten()
        .all(|child| spelled_in_body(macros, region, child))
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
                level,
                index,
                expr: left_value,
            },
            Expr::MacroArg {
                level: right_level,
                index: right_index,
                expr: right_value,
            },
        ) if index == right_index && level == right_level => Expr::MacroArg {
            level,
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

    pub(super) fn macro_region_of(&self, statement: &Span<ir::Statement>) -> Option<MacroRegionId> {
        let open = self.macro_regions.len();
        if !self.tables.named_c_types || self.macro_argument_depth != open {
            return None;
        }
        let macros = self.tables.macros;
        let region = match self.macro_regions.last() {
            None => {
                let site = macros.sites(statement.expansion).pop()?;
                site.argument
                    .is_none()
                    .then_some(MacroRegionId::Source(site.invocation))?
            }
            Some(enclosing) => {
                let parent = match enclosing {
                    MacroRegionId::Source(_) => None,
                    MacroRegionId::Nested(id) => Some(*id),
                };
                let nesting = statement.macro_nesting.as_ref()?;
                let mut children = nesting
                    .iter()
                    .filter(|link| macros.nested[link.expansion.0 as usize].parent == parent);
                let child = children.next()?;
                if child.argument.is_some() || children.next().is_some() {
                    return None;
                }
                MacroRegionId::Nested(child.expansion)
            }
        };
        fits_region(&statement.value).then_some(region)
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
        region: MacroRegionId,
        statements: &[Span<ir::Statement>],
        before: Option<&Span<ir::Statement>>,
        after: Option<&Span<ir::Statement>>,
    ) -> Result<Vec<Stmt>> {
        let level = self.macro_regions.len() as u32;
        self.macro_regions.push(region);
        self.macro_argument_depth = self.macro_regions.len();
        let lowered = self.lower_statement_list(statements);
        self.macro_regions.pop();
        self.macro_argument_depth = self.macro_regions.len();
        let mut body = lowered?;
        let mut declared = Vec::new();
        collect_lets(statements, &mut declared, &mut HashSet::new());
        let mut caller_locals = self.function_locals.clone();
        for id in declared {
            caller_locals.remove(&self.binding_name(id));
        }
        if macro_boundaries::reads_outside_arguments(&body, &caller_locals, level) {
            macro_boundaries::strip_level(&mut body, level);
            return Ok(body);
        }
        let macros = self.tables.macros;
        let definition = region.definition(macros);
        let params = macro_params(definition);
        let foldable = params.is_some()
            && !region.touches(macros, before)
            && !region.touches(macros, after)
            && statements
                .iter()
                .all(|statement| spelled_in_body(macros, region, statement));
        Ok(vec![Stmt::Scope {
            body,
            macro_region: Some(Box::new(rust::MacroRegion {
                name: definition.name.to_string(),
                params: params.unwrap_or_default(),
                foldable,
                level,
            })),
        }])
    }

    pub(super) fn macro_argument_entered(&self, value: &ir::Value) -> Option<u32> {
        let region = self
            .macro_regions
            .get(self.macro_argument_depth.checked_sub(1)?)?;
        region
            .site_of(self.tables.macros, &value.node)??
            .try_into()
            .ok()
    }
}
