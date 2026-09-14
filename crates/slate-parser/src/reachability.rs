use crate::ast::*;
use std::collections::{HashMap, HashSet};

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
        tags: tu
            .tags
            .iter()
            .filter(|tag| reachability.reachable_tags.contains(&tag.value.id))
            .cloned()
            .collect(),
        flavor: tu.flavor,
        target: tu.target,
    }
}

struct Reachability<'a> {
    tu: &'a TranslationUnit,
    nodes: &'a [SpannedDecl],
    symbols: HashMap<String, Vec<usize>>,
    reachable: HashSet<usize>,
    reachable_tags: HashSet<TagId>,
}

impl<'a> Reachability<'a> {
    fn new(tu: &'a TranslationUnit) -> Self {
        let mut symbols: HashMap<String, Vec<usize>> = HashMap::new();
        for (id, decl) in tu.decls.iter().enumerate() {
            for name in decl.names() {
                symbols.entry(name.to_string()).or_default().push(id);
            }
            let Decl::Declaration { declaration, .. } = &decl.value else {
                continue;
            };
            let TypeSpecifier::Tag(TagSpecifier::Definition(tag_id)) = &declaration.specifiers.ty
            else {
                continue;
            };
            let Some(tag) = tu.tag(*tag_id) else {
                continue;
            };
            if let Some(name) = &tag.value.name {
                symbols.entry(name.clone()).or_default().push(id);
            }
            if let TagBody::Enum(items) = &tag.value.body {
                for item in items {
                    if let EnumItem::Enumerator(enumerator) = &item.value {
                        symbols.entry(enumerator.name.clone()).or_default().push(id);
                    }
                }
            }
        }
        Self {
            tu,
            nodes: &tu.decls,
            symbols,
            reachable: HashSet::new(),
            reachable_tags: HashSet::new(),
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
            Decl::Comment(_) | Decl::StaticAssert { .. } | Decl::Asm { .. } => {}
            Decl::Function(function) => self.mark_function(function),
            Decl::Declaration { declaration, .. } => self.mark_declaration(declaration),
        }
    }

    fn mark_tag(&mut self, id: TagId) {
        if !self.reachable_tags.insert(id) {
            return;
        }
        let tu = self.tu;
        let Some(tag) = tu.tag(id) else {
            return;
        };
        match &tag.value.body {
            TagBody::Record(fields) => {
                for field in fields {
                    if let FieldItem::Field(field) = &field.value {
                        self.mark_type(&field.specifiers.ty);
                        for declarator in &field.declarators {
                            self.mark_declarator(&declarator.declarator);
                        }
                    }
                }
            }
            TagBody::Enum(items) => {
                for value in items.iter().filter_map(|item| match &item.value {
                    EnumItem::Enumerator(enumerator) => enumerator.value.as_ref(),
                    EnumItem::Comment(_) => None,
                }) {
                    self.mark_expr(value);
                }
            }
        }
    }

    fn mark_function(&mut self, function: &FunctionDefinition) {
        self.mark_type(&function.specifiers.ty);
        self.mark_declarator(&function.declarator);
        self.mark_stmts(&function.body);
    }

    fn mark_parameter(&mut self, parameter: &ParameterDeclaration) {
        self.mark_type(&parameter.specifiers.ty);
        self.mark_declarator(&parameter.declarator);
    }

    fn mark_declaration(&mut self, declaration: &Declaration) {
        self.mark_type(&declaration.specifiers.ty);
        for declarator in &declaration.declarators {
            self.mark_declarator(&declarator.declarator);
            if let Some(initializer) = &declarator.initializer {
                self.mark_initializer(initializer);
            }
        }
    }

    fn mark_type_name(&mut self, ty: &TypeName) {
        self.mark_type(&ty.specifiers.ty);
        self.mark_declarator(&ty.declarator);
    }

    fn mark_initializer(&mut self, initializer: &Initializer) {
        match initializer {
            Initializer::Expr(expr) => self.mark_expr(expr),
            Initializer::List(items) => {
                for item in items {
                    for designator in &item.designators {
                        match designator {
                            Designator::Array(index) => self.mark_expr(index),
                            Designator::ArrayRange { start, end } => {
                                self.mark_expr(start);
                                self.mark_expr(end);
                            }
                            Designator::Field(_) => {}
                        }
                    }
                    self.mark_initializer(&item.value);
                }
            }
        }
    }

    fn mark_stmts(&mut self, stmts: &[SpannedStmt]) {
        for stmt in stmts {
            self.mark_stmt(stmt);
        }
    }

    fn mark_stmt(&mut self, stmt: &SpannedStmt) {
        match &stmt.value {
            Stmt::Return(expr) | Stmt::Expr(expr) | Stmt::ComputedGoto(expr) => {
                self.mark_expr(expr)
            }
            Stmt::Labeled { body, .. } => self.mark_stmt(body),
            Stmt::SwitchLabel { label, body } => {
                match label {
                    SwitchLabel::Case(expr) => self.mark_expr(expr),
                    SwitchLabel::CaseRange { start, end } => {
                        self.mark_expr(start);
                        self.mark_expr(end);
                    }
                    SwitchLabel::Default => {}
                }
                self.mark_stmt(body);
            }
            Stmt::Decl(declaration) => self.mark_declaration(declaration),
            Stmt::Block(body) => self.mark_stmts(body),
            Stmt::If {
                condition,
                then_branch,
                else_branch,
            } => {
                self.mark_expr(condition);
                self.mark_stmts(then_branch);
                if let Some(else_branch) = else_branch {
                    self.mark_stmts(else_branch);
                }
            }
            Stmt::While { condition, body }
            | Stmt::DoWhile { body, condition }
            | Stmt::Switch {
                discriminant: condition,
                body,
            } => {
                self.mark_expr(condition);
                self.mark_stmts(body);
            }
            Stmt::For {
                init,
                condition,
                increment,
                body,
            } => {
                if let Some(init) = init {
                    self.mark_stmt(init);
                }
                for expr in condition.iter().chain(increment) {
                    self.mark_expr(expr);
                }
                self.mark_stmts(body);
            }
            Stmt::NestedFunction(function) => self.mark_function(function),
            Stmt::Comment(_)
            | Stmt::ReturnVoid
            | Stmt::StaticAssert(_)
            | Stmt::Attribute(_)
            | Stmt::LocalLabelDecl(_)
            | Stmt::Asm(_)
            | Stmt::Goto(_)
            | Stmt::Break
            | Stmt::Continue => {}
        }
    }

    fn mark_expr(&mut self, expr: &Expr) {
        match &expr.value {
            ExprKind::Identifier(name) => self.mark_name(name),
            ExprKind::Generic {
                controlling,
                associations,
            } => {
                match controlling {
                    GenericControl::Expr(controlling) => self.mark_expr(controlling),
                    GenericControl::Type { ty } => self.mark_type_name(ty),
                }
                for association in associations {
                    match association {
                        GenericAssociation::Type { ty, value } => {
                            self.mark_type_name(ty);
                            self.mark_expr(value);
                        }
                        GenericAssociation::Default(value) => self.mark_expr(value),
                    }
                }
            }
            ExprKind::SizeOfType { ty } | ExprKind::AlignOf { ty } => self.mark_type_name(ty),
            ExprKind::OffsetOf { ty, member } => {
                self.mark_type_name(ty);
                self.mark_expr(member);
            }
            ExprKind::TypesCompatible { left_ty, right_ty } => {
                self.mark_type_name(left_ty);
                self.mark_type_name(right_ty);
            }
            ExprKind::Cast { ty, value }
            | ExprKind::BitCast { ty, value }
            | ExprKind::VaArg { list: value, ty } => {
                self.mark_type_name(ty);
                self.mark_expr(value);
            }
            ExprKind::CompoundLiteral { ty, initializer } => {
                self.mark_type_name(ty);
                for item in initializer {
                    self.mark_initializer(&item.value);
                }
            }
            ExprKind::Paren(value)
            | ExprKind::SizeOfExpr(value)
            | ExprKind::AlignOfExpr(value)
            | ExprKind::Unary { operand: value, .. }
            | ExprKind::Postfix { operand: value, .. }
            | ExprKind::Member { base: value, .. } => self.mark_expr(value),
            ExprKind::Binary { left, right, .. }
            | ExprKind::Assign {
                target: left,
                value: right,
                ..
            }
            | ExprKind::Comma { left, right }
            | ExprKind::Index {
                base: left,
                index: right,
            } => {
                self.mark_expr(left);
                self.mark_expr(right);
            }
            ExprKind::Conditional {
                condition,
                then_value,
                else_value,
            } => {
                self.mark_expr(condition);
                if let Some(then_value) = then_value {
                    self.mark_expr(then_value);
                }
                self.mark_expr(else_value);
            }
            ExprKind::Call { callee, arguments } => {
                self.mark_expr(callee);
                for argument in arguments {
                    self.mark_expr(argument);
                }
            }
            ExprKind::StatementExpression(body) => self.mark_stmts(body),
            ExprKind::IntegerLiteral(_)
            | ExprKind::FloatLiteral(_)
            | ExprKind::CharLiteral(_)
            | ExprKind::StringLiteral(_)
            | ExprKind::LabelAddress(_)
            | ExprKind::BoolLiteral(_)
            | ExprKind::NullPtrLiteral => {}
        }
    }

    fn mark_name(&mut self, name: &str) {
        let ids = self.symbols.get(name).cloned().unwrap_or_default();
        for id in ids {
            self.mark(id);
        }
    }

    fn mark_type(&mut self, ty: &TypeSpecifier) {
        match ty {
            TypeSpecifier::Named(name) => self.mark_name(name),
            TypeSpecifier::Tag(TagSpecifier::Reference { name, .. }) => self.mark_name(name),
            TypeSpecifier::Tag(TagSpecifier::Definition(id)) => self.mark_tag(*id),
            TypeSpecifier::Atomic(ty) => self.mark_type_name(ty),
            TypeSpecifier::Vector(vector) => self.mark_type(&vector.element),
            TypeSpecifier::TypeOf(TypeOfOperand::Type(ty))
            | TypeSpecifier::TypeOfUnqual(TypeOfOperand::Type(ty)) => self.mark_type_name(ty),
            TypeSpecifier::Imaginary(ty) => self.mark_type(ty),
            TypeSpecifier::Void
            | TypeSpecifier::Bool
            | TypeSpecifier::Integer(_)
            | TypeSpecifier::Floating(_)
            | TypeSpecifier::Complex(_) => {}
            TypeSpecifier::FixedPoint(_) => {}
            TypeSpecifier::TypeOf(TypeOfOperand::Expression(expr))
            | TypeSpecifier::TypeOfUnqual(TypeOfOperand::Expression(expr)) => self.mark_expr(expr),
            TypeSpecifier::TargetBuiltin(_) => {}
        }
    }

    fn mark_declarator(&mut self, declarator: &Declarator) {
        match declarator {
            Declarator::Abstract | Declarator::Name(_) => {}
            Declarator::Grouped(inner)
            | Declarator::Attributed { inner, .. }
            | Declarator::Pointer { inner, .. } => self.mark_declarator(inner),
            Declarator::Array { inner, size, .. } => {
                self.mark_declarator(inner);
                if let ArraySize::Expression(size) = size {
                    self.mark_expr(size);
                }
            }
            Declarator::Function {
                inner, parameters, ..
            } => {
                self.mark_declarator(inner);
                for parameter in parameters.parameters() {
                    self.mark_parameter(parameter);
                }
            }
        }
    }
}
