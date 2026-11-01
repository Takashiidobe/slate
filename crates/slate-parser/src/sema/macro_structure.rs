use crate::ast::*;
use crate::ir::NameResolution;
use crate::visit::{self, Visitor};
use std::collections::{HashMap, HashSet};
use std::convert::Infallible;

#[derive(Clone, Copy)]
pub(super) struct MacroRegion {
    pub expansion: MacroExpansionId,
    pub last: NodeId,
}

#[derive(Clone, Copy)]
pub(super) struct MacroHole {
    pub expansion: MacroExpansionId,
    pub argument: usize,
}

#[derive(Default)]
pub(super) struct MacroStructure {
    regions: HashMap<NodeId, Vec<MacroRegion>>,
    holes: HashMap<NodeId, Vec<MacroHole>>,
    values: HashMap<NodeId, Vec<MacroExpansionId>>,
    type_arguments: HashMap<MacroExpansionId, Vec<(usize, TypeName)>>,
}

struct BodyScan {
    expression_statements: HashMap<NodeId, NodeId>,
    runtime_expressions: HashSet<NodeId>,
    call_arguments: HashMap<NodeId, Vec<NodeId>>,
    in_runtime_code: bool,
    unfit: HashSet<MacroExpansionId>,
}

impl Visitor for BodyScan {
    type Error = Infallible;

    fn visit_function(&mut self, function: &FunctionDefinition) -> Result<(), Infallible> {
        self.in_runtime_code = true;
        let walked = visit::walk_function(self, function);
        self.in_runtime_code = false;
        walked
    }

    fn visit_stmt(&mut self, stmt: &Stmt) -> Result<(), Infallible> {
        match &stmt.value {
            StmtKind::Expr(expr) => {
                self.expression_statements.insert(expr.id, stmt.id);
            }
            StmtKind::Asm(_) | StmtKind::MsAsm(_) => {
                self.mark_unfit(stmt);
                return self.outside_runtime_code(|this| visit::walk_stmt(this, stmt));
            }
            StmtKind::SwitchLabel { label, body } => {
                self.mark_unfit(stmt);
                self.outside_runtime_code(|this| match label {
                    SwitchLabel::Case(value) => this.visit_expr(value),
                    SwitchLabel::CaseRange { start, end } => {
                        this.visit_expr(start)?;
                        this.visit_expr(end)
                    }
                    SwitchLabel::Default => Ok(()),
                })?;
                return self.visit_stmt(body);
            }
            StmtKind::Labeled { .. } | StmtKind::LocalLabelDecl(_) => self.mark_unfit(stmt),
            _ => {}
        }
        visit::walk_stmt(self, stmt)
    }

    fn visit_declaration(&mut self, declaration: &Declaration) -> Result<(), Infallible> {
        if matches!(
            declaration.specifiers.storage,
            StorageClass::Static | StorageClass::Extern
        ) {
            for declarator in &declaration.declarators {
                self.mark_unfit(declarator);
            }
            return self.outside_runtime_code(|this| visit::walk_declaration(this, declaration));
        }
        visit::walk_declaration(self, declaration)
    }

    fn visit_expr(&mut self, expr: &Expr) -> Result<(), Infallible> {
        if self.in_runtime_code {
            self.runtime_expressions.insert(expr.id);
        }
        if let ExprKind::Call { arguments, .. } = &expr.value {
            self.call_arguments.insert(
                expr.id,
                arguments.iter().map(|argument| argument.id).collect(),
            );
        }
        if matches!(
            expr.value,
            ExprKind::SizeOfExpr(_) | ExprKind::AlignOfExpr(..) | ExprKind::CountOfExpr(_)
        ) {
            return self.outside_runtime_code(|this| visit::walk_expr(this, expr));
        }
        visit::walk_expr(self, expr)
    }

    fn visit_type_name(&mut self, ty: &TypeName) -> Result<(), Infallible> {
        self.outside_runtime_code(|this| visit::walk_type_name(this, ty))
    }

    fn visit_declarator(&mut self, declarator: &Declarator) -> Result<(), Infallible> {
        self.outside_runtime_code(|this| visit::walk_declarator(this, declarator))
    }

    fn visit_type_specifier(&mut self, ty: &TypeSpecifier) -> Result<(), Infallible> {
        self.outside_runtime_code(|this| visit::walk_type_specifier(this, ty))
    }

    fn visit_attribute(&mut self, attribute: &Attribute) -> Result<(), Infallible> {
        self.outside_runtime_code(|this| visit::walk_attribute(this, attribute))
    }
}

impl BodyScan {
    fn outside_runtime_code(
        &mut self,
        visit: impl FnOnce(&mut Self) -> Result<(), Infallible>,
    ) -> Result<(), Infallible> {
        let outer = std::mem::replace(&mut self.in_runtime_code, false);
        let walked = visit(self);
        self.in_runtime_code = outer;
        walked
    }

    fn mark_unfit<T>(&mut self, span: &Span<T>) {
        for link in span.macro_nesting.iter().flat_map(|nesting| nesting.iter()) {
            self.unfit.insert(link.expansion);
        }
    }
}

impl MacroStructure {
    pub(super) fn of(unit: &TranslationUnit, names: &NameResolution) -> Self {
        let macros = &unit.macros;
        let mut scan = BodyScan {
            expression_statements: HashMap::new(),
            runtime_expressions: HashSet::new(),
            call_arguments: HashMap::new(),
            in_runtime_code: false,
            unfit: HashSet::new(),
        };
        let Ok(()) = scan.visit_translation_unit(unit);
        let mut unfit = scan.unfit;
        unfit.extend(body_bindings_read_by_arguments(names));

        let depth = |id: MacroExpansionId| {
            std::iter::successors(macros.expansion(id).parent, |parent| {
                macros.expansion(*parent).parent
            })
            .count()
        };
        let mut structure = Self::default();
        for argument in &unit.macro_type_arguments {
            structure
                .type_arguments
                .entry(argument.expansion)
                .or_default()
                .push((argument.argument, argument.ty.clone()));
        }
        for (index, expansion) in macros.expansions.iter().enumerate() {
            let id = MacroExpansionId(index as u32);
            let liftable_arguments = expansion.argument_uses.iter().all(|argument| {
                !matches!(
                    argument.cover,
                    MacroCover::Statements(_) | MacroCover::Decl(_) | MacroCover::Split
                )
            });
            if unfit.contains(&id) || !liftable_arguments {
                continue;
            }
            let (first, last) = match &expansion.cover {
                Some(MacroCover::Statements(statements)) => {
                    let (Some(first), Some(last)) = (statements.first(), statements.last()) else {
                        continue;
                    };
                    (*first, *last)
                }
                Some(MacroCover::Expr(expr)) => match scan.expression_statements.get(expr) {
                    Some(statement) => (*statement, *statement),
                    None => {
                        let is_its_own_argument = expansion
                            .argument_uses
                            .iter()
                            .any(|argument| argument.cover == MacroCover::Expr(*expr));
                        if is_its_own_argument || !scan.runtime_expressions.contains(expr) {
                            continue;
                        }
                        structure.values.entry(*expr).or_default().push(id);
                        structure.add_holes(id, expansion, &scan.call_arguments);
                        continue;
                    }
                },
                _ => continue,
            };
            structure
                .regions
                .entry(first)
                .or_default()
                .push(MacroRegion {
                    expansion: id,
                    last,
                });
            structure.add_holes(id, expansion, &scan.call_arguments);
        }
        for regions in structure.regions.values_mut() {
            regions.sort_by_key(|region| depth(region.expansion));
        }
        for holes in structure.holes.values_mut() {
            holes.sort_by_key(|hole| depth(hole.expansion));
        }
        for values in structure.values.values_mut() {
            values.sort_by_key(|expansion| depth(*expansion));
        }
        structure
    }

    fn add_holes(
        &mut self,
        id: MacroExpansionId,
        expansion: &MacroExpansion,
        call_arguments: &HashMap<NodeId, Vec<NodeId>>,
    ) {
        for argument in &expansion.argument_uses {
            let nodes = match &argument.cover {
                MacroCover::Expr(node) | MacroCover::Callee(node) => std::slice::from_ref(node),
                MacroCover::CallArguments(call) => {
                    call_arguments.get(call).map_or(&[][..], Vec::as_slice)
                }
                _ => &[],
            };
            for node in nodes {
                self.holes.entry(*node).or_default().push(MacroHole {
                    expansion: id,
                    argument: argument.argument,
                });
            }
        }
    }

    pub(super) fn type_arguments(&self, expansion: MacroExpansionId) -> &[(usize, TypeName)] {
        self.type_arguments
            .get(&expansion)
            .map_or(&[], Vec::as_slice)
    }

    pub(super) fn value_expansions(&self, node: NodeId) -> &[MacroExpansionId] {
        self.values.get(&node).map_or(&[], Vec::as_slice)
    }

    pub(super) fn region(&self, first: NodeId, level: usize) -> Option<MacroRegion> {
        self.regions.get(&first)?.get(level).copied()
    }

    pub(super) fn holes(&self, node: NodeId) -> &[MacroHole] {
        self.holes.get(&node).map_or(&[], Vec::as_slice)
    }
}

fn body_bindings_read_by_arguments(names: &NameResolution) -> HashSet<MacroExpansionId> {
    let mut declared: HashMap<MacroExpansionId, HashSet<crate::ir::BindingId>> = HashMap::new();
    for binding in &names.bindings {
        for link in binding
            .macro_nesting
            .iter()
            .flat_map(|nesting| nesting.iter())
        {
            if link.argument.is_none() {
                declared
                    .entry(link.expansion)
                    .or_default()
                    .insert(binding.value.id);
            }
        }
    }
    let mut unfit = HashSet::new();
    for reference in &names.references {
        for link in reference
            .macro_nesting
            .iter()
            .flat_map(|nesting| nesting.iter())
        {
            if link.argument.is_some()
                && declared
                    .get(&link.expansion)
                    .is_some_and(|bindings| bindings.contains(&reference.value.binding))
            {
                unfit.insert(link.expansion);
            }
        }
    }
    unfit
}
