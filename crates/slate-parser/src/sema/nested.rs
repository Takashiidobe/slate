use super::expression::Lowerer;
use super::numeric::ResolveError;
use crate::ast::{Expr, ExprKind, FunctionDefinition, NodeId, Span, Stmt, StmtKind};
use crate::ir::*;
use crate::visit::{self, Visitor};
use foldhash::HashMap;

pub(super) struct NestedScope {
    id: BindingId,
    range: std::ops::Range<u32>,
    references: Vec<BindingId>,
}

struct Found {
    node: NodeId,
    name: String,
    references: Vec<BindingId>,
}

struct NestedScan<'a> {
    references: &'a HashMap<NodeId, BindingId>,
    open: Vec<usize>,
    found: Vec<Found>,
}

impl Visitor for NestedScan<'_> {
    type Error = std::convert::Infallible;

    fn visit_stmt(&mut self, stmt: &Stmt) -> Result<(), Self::Error> {
        let StmtKind::NestedFunction(function) = &stmt.value else {
            return visit::walk_stmt(self, stmt);
        };
        self.found.push(Found {
            node: stmt.id,
            name: function.declarator.name().unwrap_or_default().to_owned(),
            references: Vec::new(),
        });
        self.open.push(self.found.len() - 1);
        visit::walk_function(self, function)?;
        self.open.pop();
        Ok(())
    }

    fn visit_expr(&mut self, expr: &Expr) -> Result<(), Self::Error> {
        if matches!(expr.value, ExprKind::Identifier(_))
            && let Some(id) = self.references.get(&expr.id)
        {
            for &index in &self.open {
                let references = &mut self.found[index].references;
                if !references.contains(id) {
                    references.push(*id);
                }
            }
        }
        visit::walk_expr(self, expr)
    }
}

impl Lowerer {
    pub(super) fn analyze_nested_functions(&mut self, body: &[Stmt]) -> Result<(), ResolveError> {
        self.nested_functions.clear();
        let mut scan = NestedScan {
            references: &self.types.references,
            open: Vec::new(),
            found: Vec::new(),
        };
        let Ok(()) = visit::walk_stmts(&mut scan, body);
        for nested in scan.found {
            let id = self.declaration_id(nested.node, &nested.name)?;
            let range = self
                .names
                .nested_function_ranges
                .get(&nested.node)
                .cloned()
                .ok_or(ResolveError::Internal("nested function binding range"))?;
            self.nested_functions.push(NestedScope {
                id,
                range,
                references: nested.references,
            });
        }
        Ok(())
    }

    fn nested_captures(&self, id: BindingId) -> Vec<BindingId> {
        let functions = &self.nested_functions;
        let index: HashMap<BindingId, usize> = functions
            .iter()
            .enumerate()
            .map(|(index, scope)| (scope.id, index))
            .collect();
        let mut captures: Vec<Vec<BindingId>> = functions
            .iter()
            .map(|scope| {
                scope
                    .references
                    .iter()
                    .copied()
                    .filter(|id| !scope.range.contains(&id.0) && self.is_local_object(*id))
                    .collect()
            })
            .collect();
        let mut changed = true;
        while changed {
            changed = false;
            for (caller, scope) in functions.iter().enumerate() {
                for callee in scope.references.iter().filter_map(|id| index.get(id)) {
                    for capture in captures[*callee].clone() {
                        if !scope.range.contains(&capture.0) && !captures[caller].contains(&capture)
                        {
                            captures[caller].push(capture);
                            changed = true;
                        }
                    }
                }
            }
        }
        index
            .get(&id)
            .map(|index| std::mem::take(&mut captures[*index]))
            .unwrap_or_default()
    }

    pub(super) fn nested_function(
        &mut self,
        statement: &Stmt,
        function: &FunctionDefinition,
    ) -> Result<NestedFunction, ResolveError> {
        let enclosing = (
            std::mem::take(&mut self.break_targets),
            std::mem::take(&mut self.continue_targets),
            std::mem::take(&mut self.switches),
            std::mem::take(&mut self.seh_leave_targets),
            std::mem::take(&mut self.ms_asm_return),
            std::mem::take(&mut self.function_target),
            self.in_naked_function,
            self.types.function_names.take(),
            self.return_type,
            self.compound_start,
        );
        let site: Span<()> = statement.derive(());
        let lowered = self.function_definition(statement.id, &site, function, false, true);
        (
            self.break_targets,
            self.continue_targets,
            self.switches,
            self.seh_leave_targets,
            self.ms_asm_return,
            self.function_target,
            self.in_naked_function,
            self.types.function_names,
            self.return_type,
            self.compound_start,
        ) = enclosing;
        self.in_function = true;
        let function = lowered?.value;
        let captures = self
            .nested_captures(function.id)
            .into_iter()
            .map(|id| {
                let c = self
                    .types
                    .entities
                    .ty(&id)
                    .ok_or(ResolveError::Internal("captured variable has no type"))?;
                Ok(NestedCapture {
                    id,
                    ty: self.types.object_type(c, "captured variable type")?,
                })
            })
            .collect::<Result<_, ResolveError>>()?;
        Ok(NestedFunction { function, captures })
    }
}
