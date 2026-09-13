use crate::ast::*;
use std::collections::{HashMap, HashSet};

pub fn mark_unreachable(body: Vec<SpannedStmt>) -> Vec<SpannedStmt> {
    let mut result = Vec::with_capacity(body.len());
    let mut terminated = false;
    for stmt in body {
        if is_jump_target(&stmt) {
            terminated = false;
        }
        let stmt = mark_unreachable_in(stmt);
        if terminated {
            let loc = stmt.clone();
            result.push(loc.with_value(Stmt::Unreachable(Box::new(stmt))));
        } else {
            terminated = always_terminates(&stmt);
            result.push(stmt);
        }
    }
    result
}

fn is_jump_target(stmt: &SpannedStmt) -> bool {
    matches!(
        &stmt.value,
        Stmt::Labeled(_) | Stmt::Case(_) | Stmt::Default
    )
}

fn mark_unreachable_in(stmt: SpannedStmt) -> SpannedStmt {
    let Span {
        value,
        spelling,
        expansion,
    } = stmt;
    let value = match value {
        Stmt::Block(body) => Stmt::Block(mark_unreachable(body)),
        Stmt::If {
            condition,
            then_branch,
            else_branch,
        } => Stmt::If {
            condition,
            then_branch: mark_unreachable(then_branch),
            else_branch: else_branch.map(mark_unreachable),
        },
        Stmt::While { condition, body } => Stmt::While {
            condition,
            body: mark_unreachable(body),
        },
        Stmt::DoWhile { body, condition } => Stmt::DoWhile {
            body: mark_unreachable(body),
            condition,
        },
        Stmt::For {
            init,
            condition,
            increment,
            body,
        } => Stmt::For {
            init: init.map(|stmt| Box::new(mark_unreachable_in(*stmt))),
            condition,
            increment,
            body: mark_unreachable(body),
        },
        Stmt::Switch { discriminant, body } => Stmt::Switch {
            discriminant,
            body: mark_unreachable(body),
        },
        other => other,
    };
    Span::new(value, spelling, expansion)
}

fn always_terminates(stmt: &SpannedStmt) -> bool {
    match &stmt.value {
        Stmt::Return(_) | Stmt::ReturnVoid | Stmt::Break | Stmt::Continue | Stmt::Goto(_) => true,
        Stmt::Block(body) => block_terminates(body),
        Stmt::If {
            then_branch,
            else_branch: Some(else_branch),
            ..
        } => block_terminates(then_branch) && block_terminates(else_branch),
        Stmt::Unreachable(inner) => always_terminates(inner),
        _ => false,
    }
}

fn block_terminates(body: &[SpannedStmt]) -> bool {
    body.iter().any(always_terminates)
}

pub fn filter_translation_unit(tu: &TranslationUnit, root_file: FileId) -> TranslationUnit {
    let mut reachability = Reachability::new(tu);
    reachability.mark_roots(root_file);
    TranslationUnit {
        decls: tu
            .decls
            .iter()
            .enumerate()
            .filter(|(id, _)| reachability.reachable.contains(id))
            .map(|(_, decl)| decl.clone())
            .collect(),
    }
}

struct Reachability<'a> {
    nodes: &'a [SpannedDecl],
    symbols: HashMap<String, Vec<usize>>,
    reachable: HashSet<usize>,
}

impl<'a> Reachability<'a> {
    fn new(tu: &'a TranslationUnit) -> Self {
        let mut symbols: HashMap<String, Vec<usize>> = HashMap::new();
        for (id, decl) in tu.decls.iter().enumerate() {
            if let Some(name) = decl.name() {
                symbols.entry(name.to_string()).or_default().push(id);
            }
        }
        Self {
            nodes: &tu.decls,
            symbols,
            reachable: HashSet::new(),
        }
    }

    fn mark_roots(&mut self, root_file: FileId) {
        let roots = self
            .nodes
            .iter()
            .enumerate()
            .filter_map(|(id, decl)| (decl.provenance() == root_file).then_some(id))
            .collect::<Vec<_>>();
        for id in roots {
            self.mark(id);
        }
    }

    fn mark(&mut self, id: usize) {
        if !self.reachable.insert(id) {
            return;
        }
        match &self.nodes[id].value {
            Decl::Comment { .. } | Decl::StaticAssert { .. } | Decl::Asm { .. } => {}
            Decl::Function(function) => self.mark_type(&function.ret_type),
            Decl::Declaration { declaration, .. } => {
                self.mark_type(&declaration.specifiers.ty);
                self.mark_declarator(&declaration.declarator);
            }
            Decl::Typedef { ty, .. } => self.mark_type(ty),
            Decl::Record(record) => {
                for field in &record.fields {
                    if let FieldItem::Field(field) = &field.value {
                        self.mark_type(&field.declaration.specifiers.ty);
                    }
                }
            }
            Decl::Enum(_) => {}
        }
    }

    fn mark_name(&mut self, name: &str) {
        let ids = self.symbols.get(name).cloned().unwrap_or_default();
        for id in ids {
            self.mark(id);
        }
    }

    fn mark_type(&mut self, ty: &CType) {
        match ty {
            CType::Named(name) => self.mark_name(name),
            CType::Tagged { name, .. } => {
                if let Some(name) = name {
                    self.mark_name(name);
                }
            }
            CType::Qualified { ty, .. } | CType::Pointer { pointee: ty, .. } => self.mark_type(ty),
            CType::Atomic(ty) => self.mark_type(ty),
            CType::Vector(vector) => self.mark_type(&vector.element),
            CType::TypeOf(TypeOfOperand::Type(ty))
            | CType::TypeOfUnqual(TypeOfOperand::Type(ty)) => self.mark_type(ty),
            CType::Imaginary(ty) => self.mark_type(ty),
            CType::Array { element, .. } => self.mark_type(element),
            CType::Function {
                return_type,
                parameters,
                ..
            } => {
                self.mark_type(return_type);
                for parameter in parameters {
                    self.mark_type(&parameter.ty);
                }
            }
            CType::Void
            | CType::Bool
            | CType::Integer(_)
            | CType::Floating(_)
            | CType::Complex(_) => {}
            CType::FixedPoint(_) => {}
            CType::TypeOf(TypeOfOperand::Expression(_))
            | CType::TypeOfUnqual(TypeOfOperand::Expression(_)) => {}
            CType::TargetBuiltin(_) => {}
        }
    }

    fn mark_declarator(&mut self, declarator: &Declarator) {
        match declarator {
            Declarator::Abstract | Declarator::Name(_) => {}
            Declarator::Grouped(inner) | Declarator::Pointer { inner, .. } => {
                self.mark_declarator(inner)
            }
            Declarator::Array { inner, .. } => self.mark_declarator(inner),
            Declarator::Function {
                inner, parameters, ..
            } => {
                self.mark_declarator(inner);
                for parameter in parameters {
                    self.mark_type(&parameter.ty);
                }
            }
        }
    }
}
