use crate::ast::{
    ArraySize, Decl, DeclKind, Declaration, Declarator, Designator, EnumItemKind, Expr, ExprKind,
    Initializer, InitializerItem, ParameterList, Span, Stmt, StmtKind, StorageClass, TagBody,
    TagId as AstTagId, TagSpecifier, TranslationUnit, TypeName, TypeOfOperand, TypeSpecifier,
};
use crate::ir::{Binding, BindingId, BindingKind, NameResolution, Reference};
use std::collections::{HashMap, HashSet};

#[derive(Debug, thiserror::Error)]
pub enum ResolveError {
    #[error("unresolved {namespace} name `{name}`")]
    Unresolved {
        namespace: &'static str,
        name: String,
    },
    #[error("duplicate {namespace} name `{name}`")]
    Duplicate {
        namespace: &'static str,
        name: String,
    },
}

#[derive(Clone)]
struct Entry {
    id: BindingId,
    kind: BindingKind,
    display_name: String,
}

pub fn resolve(unit: &TranslationUnit) -> Result<NameResolution, ResolveError> {
    Resolver::new(unit).translation_unit(unit)
}

struct Resolver {
    control_scopes: bool,
    resolution: NameResolution,
    ordinary: Vec<HashMap<String, Entry>>,
    tags: Vec<HashMap<String, Entry>>,
    labels: HashMap<String, Entry>,
    local_labels: Vec<HashMap<String, Entry>>,
    local_label_declarations: HashMap<crate::ast::NodeId, Entry>,
    defined_labels: HashSet<BindingId>,
    collecting_labels: bool,
    collected_tags: HashSet<AstTagId>,
    tag_ids: HashMap<AstTagId, Entry>,
    display_counts: HashMap<String, u32>,
    unit_tags: HashMap<AstTagId, Span<crate::ast::TagDefinition>>,
    next_id: u32,
}

impl Resolver {
    fn new(unit: &TranslationUnit) -> Self {
        Self {
            control_scopes: unit.standard.has_control_scopes(),
            resolution: NameResolution::default(),
            ordinary: vec![HashMap::new()],
            tags: vec![HashMap::new()],
            labels: HashMap::new(),
            local_labels: vec![HashMap::new()],
            local_label_declarations: HashMap::new(),
            defined_labels: HashSet::new(),
            collecting_labels: false,
            collected_tags: HashSet::new(),
            tag_ids: HashMap::new(),
            display_counts: HashMap::new(),
            unit_tags: unit
                .tags
                .iter()
                .map(|tag| (tag.value.id, tag.clone()))
                .collect(),
            next_id: 0,
        }
    }

    fn translation_unit(mut self, unit: &TranslationUnit) -> Result<NameResolution, ResolveError> {
        for declaration in &unit.decls {
            self.declaration_node(declaration)?;
        }
        Ok(self.resolution)
    }

    fn declaration_node(&mut self, declaration: &Decl) -> Result<(), ResolveError> {
        match &declaration.value {
            DeclKind::Comment(_) | DeclKind::Asm(_) | DeclKind::Pragma(_) => Ok(()),
            DeclKind::StaticAssert(assertion) => self.expr(&assertion.condition),
            DeclKind::Declaration(inner) => self.declaration(inner, declaration),
            DeclKind::Function(function) => {
                self.type_specifier(&function.specifiers.ty, declaration)?;
                let name = function.declarator.name().unwrap_or("<anonymous>");
                self.bind_ordinary(name, BindingKind::Function, declaration)?;
                let outer_labels = std::mem::take(&mut self.labels);
                let outer_local_labels =
                    std::mem::replace(&mut self.local_labels, vec![HashMap::new()]);
                self.collect_labels(&function.body)?;
                self.push_scope();
                if let Some(parameters) = function.declarator.function_parameters() {
                    for parameter in parameters.parameters() {
                        self.type_specifier(&parameter.specifiers.ty, parameter)?;
                        if let Some(name) = parameter.declarator.name() {
                            self.bind_ordinary(name, BindingKind::Parameter, parameter)?;
                        }
                    }
                }
                for statement in &function.body {
                    self.statement(statement)?;
                }
                self.pop_scope();
                self.labels = outer_labels;
                self.local_labels = outer_local_labels;
                Ok(())
            }
        }
    }

    fn declaration<T>(
        &mut self,
        declaration: &Declaration,
        span: &Span<T>,
    ) -> Result<(), ResolveError> {
        self.type_specifier(&declaration.specifiers.ty, span)?;
        let base_kind = if declaration.specifiers.storage == StorageClass::Typedef {
            BindingKind::Typedef
        } else {
            BindingKind::Object
        };
        for declarator in &declaration.declarators {
            self.declarator(&declarator.declarator)?;
            let kind = if base_kind == BindingKind::Object
                && declarator.declarator.function_parameters().is_some()
            {
                BindingKind::Function
            } else {
                base_kind
            };
            if !self.collecting_labels
                && let Some(name) = declarator.declarator.name()
            {
                self.bind_ordinary(name, kind, declarator)?;
            }
            if let Some(initializer) = &declarator.initializer {
                self.initializer(initializer)?;
            }
        }
        Ok(())
    }

    fn statement(&mut self, statement: &Stmt) -> Result<(), ResolveError> {
        let scoped = self.control_scopes
            && matches!(
                statement.value,
                StmtKind::If { .. }
                    | StmtKind::While { .. }
                    | StmtKind::DoWhile { .. }
                    | StmtKind::For { .. }
                    | StmtKind::Switch { .. }
            );
        if scoped {
            self.push_scope();
        }
        let result = self.statement_contents(statement);
        if scoped {
            self.pop_scope();
        }
        result
    }

    fn control_body(&mut self, body: &Stmt) -> Result<(), ResolveError> {
        if let StmtKind::Attributed { body, .. } = &body.value {
            return self.control_body(body);
        }
        let scoped = self.control_scopes && !matches!(body.value, StmtKind::Block(_));
        if scoped {
            self.push_scope();
        }
        let result = self.statement(body);
        if scoped {
            self.pop_scope();
        }
        result
    }

    fn statement_contents(&mut self, statement: &Stmt) -> Result<(), ResolveError> {
        match &statement.value {
            StmtKind::Null
            | StmtKind::Comment(_)
            | StmtKind::ReturnVoid
            | StmtKind::Attribute(_)
            | StmtKind::Break
            | StmtKind::Continue
            | StmtKind::Pragma(_) => Ok(()),
            StmtKind::LocalLabelDecl(labels) => {
                for label in labels {
                    self.declare_local_label(label)?;
                }
                Ok(())
            }
            StmtKind::Attributed { body, .. } => self.statement(body),
            StmtKind::Return(value) | StmtKind::Expr(value) | StmtKind::ComputedGoto(value) => {
                self.expr(value)
            }
            StmtKind::Decl(declaration) => self.declaration(declaration, statement),
            StmtKind::StaticAssert(assertion) => self.expr(&assertion.condition),
            StmtKind::Block(body) => self.scoped_statements(body),
            StmtKind::If {
                condition,
                then_branch,
                else_branch,
            } => {
                self.expr(condition)?;
                self.control_body(then_branch)?;
                if let Some(branch) = else_branch {
                    self.control_body(branch)?;
                }
                Ok(())
            }
            StmtKind::While { condition, body } => {
                self.expr(condition)?;
                self.control_body(body)
            }
            StmtKind::DoWhile { body, condition } => {
                self.control_body(body)?;
                self.expr(condition)
            }
            StmtKind::For {
                init,
                condition,
                increment,
                body,
            } => {
                if let Some(init) = init {
                    self.statement(init)?;
                }
                if let Some(condition) = condition {
                    self.expr(condition)?;
                }
                if let Some(increment) = increment {
                    self.expr(increment)?;
                }
                self.control_body(body)?;
                Ok(())
            }
            StmtKind::Switch { discriminant, body } => {
                self.expr(discriminant)?;
                self.control_body(body)
            }
            StmtKind::Labeled { label, body } => {
                if self.collecting_labels {
                    self.bind_label(label)?;
                }
                self.statement(body)
            }
            StmtKind::SwitchLabel { label, body } => {
                match label {
                    crate::ast::SwitchLabel::Case(value) => self.expr(value)?,
                    crate::ast::SwitchLabel::CaseRange { start, end } => {
                        self.expr(start)?;
                        self.expr(end)?;
                    }
                    crate::ast::SwitchLabel::Default => {}
                }
                self.statement(body)
            }
            StmtKind::Asm(asm) => {
                if let Some(operands) = &asm.operands {
                    for operand in operands.outputs.iter().chain(&operands.inputs) {
                        self.expr(&operand.expr)?;
                    }
                    for label in &operands.labels {
                        self.reference_label(label)?;
                    }
                }
                Ok(())
            }
            StmtKind::Goto(label) => self.reference_label(label),
            StmtKind::NestedFunction(_) if self.collecting_labels => Ok(()),
            StmtKind::NestedFunction(function) => {
                let fake = statement
                    .clone()
                    .with_value(DeclKind::Function((**function).clone()));
                self.declaration_node(&fake)
            }
        }
    }

    fn statements(&mut self, statements: &[Stmt]) -> Result<(), ResolveError> {
        for statement in statements {
            self.statement(statement)?;
        }
        Ok(())
    }

    fn scoped_statements(&mut self, statements: &[Stmt]) -> Result<(), ResolveError> {
        self.push_scope();
        let result = self.statements(statements);
        self.pop_scope();
        result
    }

    fn expr(&mut self, expr: &Expr) -> Result<(), ResolveError> {
        match &expr.value {
            ExprKind::Identifier(name) => self.reference_ordinary(name, expr),
            ExprKind::Paren(value)
            | ExprKind::SizeOfExpr(value)
            | ExprKind::AlignOfExpr(value)
            | ExprKind::Unary { operand: value, .. }
            | ExprKind::Postfix { operand: value, .. }
            | ExprKind::Member { base: value, .. } => self.expr(value),
            ExprKind::Cast { ty, value }
            | ExprKind::BitCast { ty, value }
            | ExprKind::VaArg { list: value, ty } => {
                self.type_name(ty, expr)?;
                self.expr(value)
            }
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
                self.expr(left)?;
                self.expr(right)
            }
            ExprKind::Conditional {
                condition,
                then_value,
                else_value,
            } => {
                self.expr(condition)?;
                if let Some(value) = then_value {
                    self.expr(value)?;
                }
                self.expr(else_value)
            }
            ExprKind::Call { callee, arguments } => {
                self.expr(callee)?;
                for argument in arguments {
                    self.expr(argument)?;
                }
                Ok(())
            }
            ExprKind::CompoundLiteral { ty, initializer } => {
                self.type_name(ty, expr)?;
                for item in initializer {
                    self.initializer_item(item)?;
                }
                Ok(())
            }
            ExprKind::SizeOfType { ty } | ExprKind::AlignOf { ty } => self.type_name(ty, expr),
            ExprKind::OffsetOf { ty, member } => {
                self.type_name(ty, expr)?;
                self.offsetof_member(member)
            }
            ExprKind::Generic {
                controlling,
                associations,
            } => {
                match controlling {
                    crate::ast::GenericControl::Expr(value) => self.expr(value)?,
                    crate::ast::GenericControl::Type { ty } => self.type_name(ty, expr)?,
                }
                for association in associations {
                    match association {
                        crate::ast::GenericAssociation::Type { ty, value } => {
                            self.type_name(ty, expr)?;
                            self.expr(value)?;
                        }
                        crate::ast::GenericAssociation::Default(value) => self.expr(value)?,
                    }
                }
                Ok(())
            }
            ExprKind::TypesCompatible { left_ty, right_ty } => {
                self.type_name(left_ty, expr)?;
                self.type_name(right_ty, expr)
            }
            ExprKind::LabelAddress(label) => self.reference_label(label),
            ExprKind::StatementExpression(body) => self.scoped_statements(body),
            ExprKind::IntegerLiteral(_)
            | ExprKind::FloatLiteral(_)
            | ExprKind::CharLiteral(_)
            | ExprKind::StringLiteral(_)
            | ExprKind::BoolLiteral(_)
            | ExprKind::NullPtrLiteral => Ok(()),
        }
    }

    fn offsetof_member(&mut self, member: &Expr) -> Result<(), ResolveError> {
        match &member.value {
            ExprKind::Identifier(_) => Ok(()),
            ExprKind::Member { base, .. } => self.offsetof_member(base),
            ExprKind::Index { base, index } => {
                self.offsetof_member(base)?;
                self.expr(index)
            }
            _ => self.expr(member),
        }
    }

    fn initializer(&mut self, initializer: &Initializer) -> Result<(), ResolveError> {
        match initializer {
            Initializer::Expr(value) => self.expr(value),
            Initializer::List(items) => {
                for item in items {
                    self.initializer_item(item)?;
                }
                Ok(())
            }
        }
    }

    fn initializer_item(&mut self, item: &InitializerItem) -> Result<(), ResolveError> {
        for designator in &item.designators {
            match designator {
                Designator::Array(value) => self.expr(value)?,
                Designator::ArrayRange { start, end } => {
                    self.expr(start)?;
                    self.expr(end)?;
                }
                Designator::Field(_) => {}
            }
        }
        self.initializer(&item.value)
    }

    fn type_name<T>(&mut self, ty: &TypeName, span: &Span<T>) -> Result<(), ResolveError> {
        self.type_specifier(&ty.specifiers.ty, span)?;
        self.declarator(&ty.declarator)
    }

    fn declarator(&mut self, declarator: &Declarator) -> Result<(), ResolveError> {
        match declarator {
            Declarator::Abstract | Declarator::Name(_) => Ok(()),
            Declarator::Grouped(inner)
            | Declarator::Attributed { inner, .. }
            | Declarator::Pointer { inner, .. } => self.declarator(inner),
            Declarator::Array { inner, size, .. } => {
                self.declarator(inner)?;
                if let ArraySize::Expression(value) = size {
                    self.expr(value)?;
                }
                Ok(())
            }
            Declarator::Function { inner, parameters } => {
                self.declarator(inner)?;
                if let ParameterList::Prototype { parameters, .. } = parameters {
                    for parameter in parameters {
                        self.type_specifier(&parameter.specifiers.ty, parameter)?;
                        if self.collecting_labels {
                            self.declarator(&parameter.declarator)?;
                        }
                    }
                }
                Ok(())
            }
        }
    }

    fn type_specifier<T>(
        &mut self,
        ty: &TypeSpecifier,
        span: &Span<T>,
    ) -> Result<(), ResolveError> {
        match ty {
            TypeSpecifier::Named(name) => self.reference_typedef(name, span),
            TypeSpecifier::Tag(TagSpecifier::Reference {
                name, fixed_type, ..
            }) => {
                self.reference_tag(name, span)?;
                if let Some(fixed_type) = fixed_type {
                    self.type_name(fixed_type, span)?;
                }
                Ok(())
            }
            TypeSpecifier::Tag(TagSpecifier::Definition(id)) => self.define_tag(*id, span),
            TypeSpecifier::Atomic(ty) => self.type_name(ty, span),
            TypeSpecifier::Complex(inner) | TypeSpecifier::Imaginary(inner) => {
                self.type_specifier(inner, span)
            }
            TypeSpecifier::TypeOf(TypeOfOperand::Expression(value))
            | TypeSpecifier::TypeOfUnqual(TypeOfOperand::Expression(value)) => self.expr(value),
            TypeSpecifier::TypeOf(TypeOfOperand::Type(ty))
            | TypeSpecifier::TypeOfUnqual(TypeOfOperand::Type(ty)) => self.type_name(ty, span),
            TypeSpecifier::Integer(crate::ast::IntegerType::BitInt { width, .. }) => {
                self.expr(width)
            }
            TypeSpecifier::Vector(vector) => {
                self.type_specifier(&vector.element, span)?;
                match &vector.size {
                    crate::ast::VectorSize::Bytes(value) | crate::ast::VectorSize::Lanes(value) => {
                        self.expr(value)
                    }
                }
            }
            _ => Ok(()),
        }
    }

    fn define_tag<T>(&mut self, id: AstTagId, span: &Span<T>) -> Result<(), ResolveError> {
        if self.tag_ids.contains_key(&id)
            || (self.collecting_labels && !self.collected_tags.insert(id))
        {
            return Ok(());
        }
        let Some(tag) = self.unit_tags.get(&id).cloned() else {
            return Ok(());
        };
        let name = tag
            .name
            .clone()
            .unwrap_or_else(|| format!("<anonymous:{}>", id.0));
        if !self.collecting_labels {
            let entry = self.new_entry(&name, BindingKind::Tag, span);
            if tag.name.is_some() {
                self.tags
                    .last_mut()
                    .unwrap()
                    .insert(name.clone(), entry.clone());
            }
            self.tag_ids.insert(id, entry);
        }
        match &tag.body {
            TagBody::Enum { enumerators, .. } => {
                for item in enumerators {
                    if let EnumItemKind::Enumerator(enumerator) = &item.value {
                        if let Some(value) = &enumerator.value {
                            self.expr(value)?;
                        }
                        if !self.collecting_labels {
                            self.bind_ordinary(&enumerator.name, BindingKind::Enumerator, item)?;
                        }
                    }
                }
            }
            TagBody::Record(fields) => {
                for field in fields {
                    if let crate::ast::FieldItemKind::Field(field) = &field.value {
                        self.type_specifier(&field.specifiers.ty, &tag)?;
                        for declarator in &field.declarators {
                            self.declarator(&declarator.declarator)?;
                            if let Some(width) = &declarator.bit_width {
                                self.expr(width)?;
                            }
                        }
                    }
                }
            }
        }
        Ok(())
    }

    fn collect_labels(&mut self, statements: &[Stmt]) -> Result<(), ResolveError> {
        let first_binding = self.next_id;
        self.collecting_labels = true;
        let result = self.scoped_statements(statements);
        self.collecting_labels = false;
        result?;
        for binding in &self.resolution.bindings {
            if binding.value.id.0 >= first_binding
                && binding.kind == BindingKind::Label
                && !self.defined_labels.contains(&binding.value.id)
            {
                return Err(ResolveError::Unresolved {
                    namespace: "label",
                    name: binding.name.clone(),
                });
            }
        }
        Ok(())
    }

    fn declare_local_label(&mut self, label: &Span<String>) -> Result<(), ResolveError> {
        let entry = if self.collecting_labels {
            if self.local_labels.last().unwrap().contains_key(&label.value) {
                return Err(ResolveError::Duplicate {
                    namespace: "label",
                    name: label.value.clone(),
                });
            }
            let entry = self.new_entry(&label.value, BindingKind::Label, label);
            self.local_label_declarations
                .insert(label.id, entry.clone());
            entry
        } else {
            self.local_label_declarations
                .get(&label.id)
                .cloned()
                .ok_or_else(|| ResolveError::Unresolved {
                    namespace: "label",
                    name: label.value.clone(),
                })?
        };
        self.local_labels
            .last_mut()
            .unwrap()
            .insert(label.value.clone(), entry);
        Ok(())
    }

    fn bind_ordinary<T>(
        &mut self,
        name: &str,
        kind: BindingKind,
        span: &Span<T>,
    ) -> Result<Entry, ResolveError> {
        if let Some(existing) = self.ordinary.last().unwrap().get(name).cloned() {
            if self.ordinary.len() == 1 && existing.kind == kind {
                return Ok(existing);
            }
            return Err(ResolveError::Duplicate {
                namespace: "ordinary",
                name: name.into(),
            });
        }
        let entry = self.new_entry(name, kind, span);
        self.ordinary
            .last_mut()
            .unwrap()
            .insert(name.into(), entry.clone());
        Ok(entry)
    }

    fn bind_label(&mut self, label: &Span<String>) -> Result<Entry, ResolveError> {
        if let Some(entry) = self
            .local_labels
            .iter()
            .rev()
            .find_map(|scope| scope.get(&label.value))
            .cloned()
        {
            if !self.defined_labels.insert(entry.id) {
                return Err(ResolveError::Duplicate {
                    namespace: "label",
                    name: label.value.clone(),
                });
            }
            return Ok(entry);
        }
        if self.labels.contains_key(&label.value) {
            return Err(ResolveError::Duplicate {
                namespace: "label",
                name: label.value.clone(),
            });
        }
        let entry = self.new_entry(&label.value, BindingKind::Label, label);
        self.defined_labels.insert(entry.id);
        self.labels.insert(label.value.clone(), entry.clone());
        Ok(entry)
    }

    fn new_entry<T>(&mut self, name: &str, kind: BindingKind, span: &Span<T>) -> Entry {
        let id = BindingId(self.next_id);
        self.next_id += 1;
        let count = self.display_counts.entry(name.into()).or_default();
        let display_name = if *count == 0 {
            name.into()
        } else {
            format!("{name}#{count}")
        };
        *count += 1;
        let entry = Entry {
            id,
            kind,
            display_name: display_name.clone(),
        };
        self.resolution.bindings.push(copy_span(
            span,
            Binding {
                id,
                kind,
                name: name.into(),
                display_name,
            },
        ));
        entry
    }

    fn reference_ordinary<T>(&mut self, name: &str, span: &Span<T>) -> Result<(), ResolveError> {
        if self.collecting_labels {
            return Ok(());
        }
        let entry = self
            .ordinary
            .iter()
            .rev()
            .find_map(|scope| scope.get(name))
            .cloned()
            .ok_or_else(|| ResolveError::Unresolved {
                namespace: "ordinary",
                name: name.into(),
            })?;
        self.push_reference(name, entry, span);
        Ok(())
    }

    fn reference_typedef<T>(&mut self, name: &str, span: &Span<T>) -> Result<(), ResolveError> {
        if self.collecting_labels {
            return Ok(());
        }
        let entry = self
            .ordinary
            .iter()
            .rev()
            .find_map(|scope| scope.get(name))
            .filter(|entry| entry.kind == BindingKind::Typedef)
            .cloned()
            .ok_or_else(|| ResolveError::Unresolved {
                namespace: "typedef",
                name: name.into(),
            })?;
        self.push_reference(name, entry, span);
        Ok(())
    }

    fn reference_tag<T>(&mut self, name: &str, span: &Span<T>) -> Result<(), ResolveError> {
        if self.collecting_labels {
            return Ok(());
        }
        let entry = self
            .tags
            .iter()
            .rev()
            .find_map(|scope| scope.get(name))
            .cloned()
            .ok_or_else(|| ResolveError::Unresolved {
                namespace: "tag",
                name: name.into(),
            })?;
        self.push_reference(name, entry, span);
        Ok(())
    }

    fn reference_label<T>(&mut self, label: &Span<T>) -> Result<(), ResolveError>
    where
        T: AsRef<str>,
    {
        if self.collecting_labels {
            return Ok(());
        }
        let name = label.value.as_ref();
        let entry = self
            .local_labels
            .iter()
            .rev()
            .find_map(|scope| scope.get(name))
            .or_else(|| self.labels.get(name))
            .cloned()
            .ok_or_else(|| ResolveError::Unresolved {
                namespace: "label",
                name: name.into(),
            })?;
        self.push_reference(name, entry, label);
        Ok(())
    }

    fn push_reference<T>(&mut self, name: &str, entry: Entry, span: &Span<T>) {
        self.resolution.references.push(copy_span(
            span,
            Reference {
                binding: entry.id,
                kind: entry.kind,
                name: name.into(),
                display_name: entry.display_name,
            },
        ));
    }

    fn push_scope(&mut self) {
        self.ordinary.push(HashMap::new());
        self.tags.push(HashMap::new());
        self.local_labels.push(HashMap::new());
    }
    fn pop_scope(&mut self) {
        self.ordinary.pop();
        self.tags.pop();
        self.local_labels.pop();
    }
}

fn copy_span<T, U>(span: &Span<T>, value: U) -> Span<U> {
    Span {
        id: span.id,
        value,
        spelling: span.spelling,
        expansion: span.expansion,
        provenance: span.provenance,
        macro_origin: span.macro_origin.clone(),
    }
}
