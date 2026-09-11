use crate::ast::*;
use std::collections::{HashMap, HashSet};

pub fn filter_translation_unit(tu: &TranslationUnit, root_file: FileId) -> TranslationUnit {
    let mut reachability = Reachability::new(tu);
    reachability.mark_roots(root_file);
    TranslationUnit {
        decls: reachability.filter_decls(&tu.decls),
    }
}

struct Reachability<'a> {
    nodes: Vec<&'a Decl>,
    symbols: HashMap<String, Vec<usize>>,
    reachable: HashSet<usize>,
    next_id: usize,
}

impl<'a> Reachability<'a> {
    fn new(tu: &'a TranslationUnit) -> Self {
        let mut reachability = Self {
            nodes: Vec::new(),
            symbols: HashMap::new(),
            reachable: HashSet::new(),
            next_id: 0,
        };
        reachability.index_decls(&tu.decls);
        reachability
    }

    fn index_decls(&mut self, decls: &'a [Decl]) {
        for decl in decls {
            let id = self.nodes.len();
            self.nodes.push(decl);
            if let Some(name) = decl.name() {
                self.symbols.entry(name.to_string()).or_default().push(id);
            }
            if let Decl::Conditional(conditional) = decl {
                for (_, branch) in &conditional.branches {
                    self.index_decls(branch);
                }
            }
        }
    }

    fn mark_roots(&mut self, root_file: FileId) {
        let roots = self
            .nodes
            .iter()
            .enumerate()
            .filter_map(|(id, decl)| (decl.provenance() == Some(root_file)).then_some(id))
            .collect::<Vec<_>>();
        for id in roots {
            self.mark(id);
        }
    }

    fn mark(&mut self, id: usize) {
        if !self.reachable.insert(id) {
            return;
        }
        match self.nodes[id] {
            Decl::Function(function) => self.mark_type(&function.ret_type),
            Decl::Declaration { declaration, .. } => {
                self.mark_type(&declaration.specifiers.ty);
                self.mark_declarator(&declaration.declarator);
            }
            Decl::Typedef { ty, .. } => self.mark_type(ty),
            Decl::Record(record) => {
                for field in &record.fields {
                    self.mark_type(&field.declaration.specifiers.ty);
                }
            }
            Decl::Enum(_) => {}
            Decl::Conditional(_) => {}
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
            | CType::BFloat16
            | CType::Char
            | CType::SignedChar
            | CType::UnsignedChar
            | CType::Short
            | CType::UnsignedShort
            | CType::Int
            | CType::UnsignedInt
            | CType::Long
            | CType::UnsignedLong
            | CType::LongLong
            | CType::UnsignedLongLong
            | CType::Float
            | CType::Float16
            | CType::Fp16
            | CType::Float64x
            | CType::Float128
            | CType::Float128Ext
            | CType::Double
            | CType::LongDouble
            | CType::Complex
            | CType::DoubleComplex
            | CType::LongDoubleComplex
            | CType::Int128
            | CType::UnsignedInt128 => {}
            CType::BitInt { .. } => {}
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

    fn filter_decls(&mut self, decls: &[Decl]) -> Vec<Decl> {
        let mut filtered = Vec::new();
        for decl in decls {
            let id = self.next_id;
            self.next_id += 1;
            match decl {
                Decl::Conditional(conditional) => {
                    let branches = conditional
                        .branches
                        .iter()
                        .map(|(condition, branch)| (condition.clone(), self.filter_decls(branch)))
                        .filter(|(_, branch)| !branch.is_empty())
                        .collect::<Vec<_>>();
                    if !branches.is_empty() {
                        filtered.push(Decl::Conditional(Conditional { branches }));
                    }
                }
                _ if self.reachable.contains(&id) => filtered.push(decl.clone()),
                _ => {}
            }
        }
        filtered
    }
}
