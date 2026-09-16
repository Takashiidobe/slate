use crate::ast::*;
use std::collections::{HashMap, HashSet};

pub fn filter_translation_unit(tu: &TranslationUnit, root_file: FileId) -> TranslationUnit {
    let mut reachability = Reachability::new(tu);
    reachability.mark_roots(root_file);
    TranslationUnit {
        standard: tu.standard,
        options: tu.options.clone(),
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
    nodes: &'a [Decl],
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
            let DeclKind::Declaration(declaration) = &decl.value else {
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
            if let TagBody::Enum { enumerators, .. } = &tag.value.body {
                for item in enumerators {
                    if let EnumItemKind::Enumerator(enumerator) = &item.value {
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
        let mut roots = self
            .nodes
            .iter()
            .enumerate()
            .filter_map(|(id, decl)| (decl.provenance.file == root_file).then_some(id))
            .collect::<Vec<_>>();
        roots.extend(
            self.nodes
                .iter()
                .enumerate()
                .filter_map(|(id, decl)| self.has_retention_attribute(decl).then_some(id)),
        );
        for id in roots {
            self.mark(id);
        }
    }

    fn mark(&mut self, id: usize) {
        if !self.reachable.insert(id) {
            return;
        }
        match &self.nodes[id].value {
            DeclKind::Comment(_)
            | DeclKind::StaticAssert { .. }
            | DeclKind::Asm { .. }
            | DeclKind::Pragma(_) => {}
            DeclKind::Function(function) => self.mark_function(function),
            DeclKind::Declaration(declaration) => self.mark_declaration(declaration),
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
        self.mark_attributes(&tag.attributes);
        match &tag.value.body {
            TagBody::Record(fields) => {
                for field in fields {
                    if let FieldItemKind::Field(field) = &field.value {
                        self.mark_attributes(&field.specifiers.attributes);
                        self.mark_type(&field.specifiers.ty);
                        for declarator in &field.declarators {
                            self.mark_attributes(&declarator.attributes);
                            self.mark_declarator(&declarator.declarator);
                        }
                    }
                }
            }
            TagBody::Enum {
                fixed_type,
                enumerators,
            } => {
                if let Some(fixed_type) = fixed_type {
                    self.mark_type_name(fixed_type);
                }
                for value in enumerators.iter().filter_map(|item| match &item.value {
                    EnumItemKind::Enumerator(enumerator) => enumerator.value.as_ref(),
                    EnumItemKind::Comment(_) => None,
                }) {
                    self.mark_expr(value);
                }
            }
        }
    }

    fn mark_function(&mut self, function: &FunctionDefinition) {
        self.mark_attributes(&function.attributes);
        self.mark_attributes(&function.specifiers.attributes);
        self.mark_type(&function.specifiers.ty);
        self.mark_declarator(&function.declarator);
        self.mark_stmts(&function.body);
    }

    fn mark_parameter(&mut self, parameter: &ParameterDeclaration) {
        self.mark_attributes(&parameter.specifiers.attributes);
        self.mark_attributes(&parameter.attributes);
        self.mark_type(&parameter.specifiers.ty);
        self.mark_declarator(&parameter.declarator);
    }

    fn mark_declaration(&mut self, declaration: &Declaration) {
        self.mark_attributes(&declaration.specifiers.attributes);
        self.mark_type(&declaration.specifiers.ty);
        for declarator in &declaration.declarators {
            self.mark_declarator(&declarator.declarator);
            self.mark_attributes(&declarator.attributes);
            if let Some(initializer) = &declarator.initializer {
                self.mark_initializer(initializer);
            }
        }
    }

    fn mark_type_name(&mut self, ty: &TypeName) {
        self.mark_attributes(&ty.specifiers.attributes);
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

    fn mark_stmts(&mut self, stmts: &[Stmt]) {
        for stmt in stmts {
            self.mark_stmt(stmt);
        }
    }

    fn mark_stmt(&mut self, stmt: &Stmt) {
        match &stmt.value {
            StmtKind::Return(expr) | StmtKind::Expr(expr) | StmtKind::ComputedGoto(expr) => {
                self.mark_expr(expr)
            }
            StmtKind::Labeled { body, .. } => self.mark_stmt(body),
            StmtKind::SwitchLabel { label, body } => {
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
            StmtKind::Decl(declaration) => self.mark_declaration(declaration),
            StmtKind::Block(body) => self.mark_stmts(body),
            StmtKind::If {
                condition,
                then_branch,
                else_branch,
            } => {
                self.mark_expr(condition);
                self.mark_stmt(then_branch);
                if let Some(else_branch) = else_branch {
                    self.mark_stmt(else_branch);
                }
            }
            StmtKind::While { condition, body }
            | StmtKind::DoWhile { body, condition }
            | StmtKind::Switch {
                discriminant: condition,
                body,
            } => {
                self.mark_expr(condition);
                self.mark_stmt(body);
            }
            StmtKind::For {
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
                self.mark_stmt(body);
            }
            StmtKind::NestedFunction(function) => self.mark_function(function),
            StmtKind::Attribute(attributes) => self.mark_attributes(attributes),
            StmtKind::Null
            | StmtKind::Comment(_)
            | StmtKind::ReturnVoid
            | StmtKind::StaticAssert(_)
            | StmtKind::LocalLabelDecl(_)
            | StmtKind::Asm(_)
            | StmtKind::Goto(_)
            | StmtKind::Break
            | StmtKind::Continue
            | StmtKind::Pragma(_) => {}
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
            TypeSpecifier::Tag(TagSpecifier::Reference {
                name, fixed_type, ..
            }) => {
                self.mark_name(name);
                if let Some(fixed_type) = fixed_type {
                    self.mark_type_name(fixed_type);
                }
            }
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
            Declarator::Grouped(inner) => self.mark_declarator(inner),
            Declarator::Attributed { inner, attributes } => {
                self.mark_attributes(attributes);
                self.mark_declarator(inner);
            }
            Declarator::Pointer {
                inner, attributes, ..
            } => {
                self.mark_attributes(attributes);
                self.mark_declarator(inner);
            }
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

    fn has_retention_attribute(&self, decl: &Decl) -> bool {
        match &decl.value {
            DeclKind::Function(function) => {
                self.attributes_retain(&function.specifiers.attributes)
                    || self.declarator_has_retention_attribute(&function.declarator)
            }
            DeclKind::Declaration(declaration) => {
                self.attributes_retain(&declaration.specifiers.attributes)
                    || declaration.declarators.iter().any(|declarator| {
                        self.attributes_retain(&declarator.attributes)
                            || self.declarator_has_retention_attribute(&declarator.declarator)
                    })
            }
            DeclKind::Comment(_)
            | DeclKind::StaticAssert(_)
            | DeclKind::Asm(_)
            | DeclKind::Pragma(_) => false,
        }
    }

    fn declarator_has_retention_attribute(&self, declarator: &Declarator) -> bool {
        match declarator {
            Declarator::Abstract | Declarator::Name(_) => false,
            Declarator::Grouped(inner) | Declarator::Function { inner, .. } => {
                self.declarator_has_retention_attribute(inner)
            }
            Declarator::Attributed { inner, attributes }
            | Declarator::Pointer {
                inner, attributes, ..
            } => {
                self.attributes_retain(attributes) || self.declarator_has_retention_attribute(inner)
            }
            Declarator::Array { inner, .. } => self.declarator_has_retention_attribute(inner),
        }
    }

    fn attributes_retain(&self, attributes: &[Attribute]) -> bool {
        attributes.iter().any(|attribute| {
            matches!(
                attribute,
                Attribute::Used
                    | Attribute::Retain
                    | Attribute::Constructor(_)
                    | Attribute::Destructor(_)
                    | Attribute::Alias(_)
                    | Attribute::WeakRef(_)
                    | Attribute::Ifunc(_)
            )
        })
    }

    fn mark_attributes(&mut self, attributes: &[Attribute]) {
        for attribute in attributes {
            match attribute {
                Attribute::Alias(name)
                | Attribute::WeakRef(name)
                | Attribute::Ifunc(name)
                | Attribute::Cleanup(name) => self.mark_name(name),
                Attribute::AddressSpace(value)
                | Attribute::PassObjectSize {
                    size_type: value, ..
                }
                | Attribute::Aligned(value)
                | Attribute::VectorSize(value)
                | Attribute::AllocAlign(value)
                | Attribute::ExtVectorType(value)
                | Attribute::CallingConvention(CallingConvention::RegParm(value))
                | Attribute::AlignAs(AlignAsOperand::Expr(value)) => self.mark_expr(value),
                Attribute::AlignAs(AlignAsOperand::Type { ty }) => self.mark_type_name(ty),
                Attribute::AssumeAligned(values) | Attribute::AllocSize(values) => {
                    for value in values {
                        self.mark_expr(value);
                    }
                }
                _ => {}
            }
        }
    }
}
