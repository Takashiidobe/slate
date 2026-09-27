use crate::ast::{
    ArraySize, Decl, DeclKind, Declaration, Declarator, EnumItemKind, Expr, ExprKind, Initializer,
    InitializerItem, Span, Stmt, StmtKind, StorageClass, TagBody, TagId as AstTagId, TagSpecifier,
    TranslationUnit, TypeName, TypeOfOperand, TypeSpecifier,
};
use crate::compiler_args::CompilerFlavor;
use crate::ir::{Binding, BindingId, BindingKind, NameResolution, Reference};
use crate::standard_features::StandardFeatures;
use crate::visit::Visitor;
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
    let mut resolver = Resolver::new(unit);
    resolver.visit_translation_unit(unit)?;
    Ok(resolver.resolution)
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
    linked: HashMap<String, Entry>,
    implicit_builtin_calls: HashMap<&'static str, Vec<Span<()>>>,
    flavor: CompilerFlavor,
    next_id: u32,
}

impl Resolver {
    fn new(unit: &TranslationUnit) -> Self {
        Self {
            control_scopes: StandardFeatures::new(unit.standard).control_statement_scopes,
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
            linked: HashMap::new(),
            implicit_builtin_calls: HashMap::new(),
            flavor: unit.flavor,
            next_id: 0,
        }
    }

    fn declaration_node(&mut self, declaration: &Decl) -> Result<(), ResolveError> {
        match &declaration.value {
            DeclKind::Comment(_) | DeclKind::Pragma(_) => Ok(()),
            DeclKind::Asm(asm) => {
                let Some(operands) = &asm.operands else {
                    return Ok(());
                };
                for operand in operands.outputs.iter().chain(&operands.inputs) {
                    self.visit_expr(&operand.expr)?;
                }
                Ok(())
            }
            DeclKind::StaticAssert(assertion) => self.visit_expr(&assertion.condition),
            DeclKind::Declaration(inner) => self.declaration(inner, declaration),
            DeclKind::Function(function) => {
                self.type_specifier(&function.specifiers.ty, declaration)?;
                crate::visit::walk_attributes(self, &function.specifiers.attributes)?;
                let name = function.declarator.name().unwrap_or("<anonymous>");
                self.bind_ordinary(name, BindingKind::Function, true, declaration)?;
                let outer_labels = std::mem::take(&mut self.labels);
                let outer_local_labels =
                    std::mem::replace(&mut self.local_labels, vec![HashMap::new()]);
                self.collect_labels(&function.body)?;
                self.push_scope();
                if let Some(parameters) = function.declarator.function_parameters() {
                    for parameter in parameters.parameters() {
                        self.type_specifier(&parameter.specifiers.ty, parameter)?;
                        self.visit_declarator(&parameter.declarator)?;
                        if let Some(name) = parameter.declarator.name() {
                            self.bind_ordinary(name, BindingKind::Parameter, false, parameter)?;
                        }
                    }
                }
                for statement in &function.body {
                    self.visit_stmt(statement)?;
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
        if declaration.declarators.is_empty()
            && let TypeSpecifier::Tag(TagSpecifier::Reference {
                name, fixed_type, ..
            }) = &declaration.specifiers.ty
        {
            self.declare_incomplete_tag(name, span)?;
            if let Some(fixed_type) = fixed_type {
                self.type_name(fixed_type, span)?;
            }
            return Ok(());
        }
        self.type_specifier(&declaration.specifiers.ty, span)?;
        crate::visit::walk_attributes(self, &declaration.specifiers.attributes)?;
        let base_kind = if declaration.specifiers.storage == StorageClass::Typedef {
            BindingKind::Typedef
        } else {
            BindingKind::Object
        };
        for declarator in &declaration.declarators {
            self.visit_declarator(&declarator.declarator)?;
            crate::visit::walk_attributes(self, &declarator.attributes)?;
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
                let linked = kind != BindingKind::Typedef
                    && (self.ordinary.len() == 1
                        || declaration.specifiers.storage == StorageClass::Extern
                        || (kind == BindingKind::Function
                            && declaration.specifiers.storage == StorageClass::None));
                self.bind_ordinary(name, kind, linked, declarator)?;
            }
            if let Some(initializer) = &declarator.initializer {
                self.visit_initializer(initializer)?;
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
        let result = self.visit_stmt(body);
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
            StmtKind::Attributed { body, .. } => self.visit_stmt(body),
            StmtKind::Return(value) | StmtKind::Expr(value) | StmtKind::ComputedGoto(value) => {
                self.visit_expr(value)
            }
            StmtKind::Decl(declaration) => self.declaration(declaration, statement),
            StmtKind::StaticAssert(assertion) => self.visit_expr(&assertion.condition),
            StmtKind::Block(body) => self.scoped_statements(body),
            StmtKind::If {
                condition,
                then_branch,
                else_branch,
            } => {
                self.visit_expr(condition)?;
                self.control_body(then_branch)?;
                if let Some(branch) = else_branch {
                    self.control_body(branch)?;
                }
                Ok(())
            }
            StmtKind::While { condition, body } => {
                self.visit_expr(condition)?;
                self.control_body(body)
            }
            StmtKind::DoWhile { body, condition } => {
                self.control_body(body)?;
                self.visit_expr(condition)
            }
            StmtKind::For {
                init,
                condition,
                increment,
                body,
            } => {
                if let Some(init) = init {
                    self.visit_stmt(init)?;
                }
                if let Some(condition) = condition {
                    self.visit_expr(condition)?;
                }
                if let Some(increment) = increment {
                    self.visit_expr(increment)?;
                }
                self.control_body(body)?;
                Ok(())
            }
            StmtKind::Switch { discriminant, body } => {
                self.visit_expr(discriminant)?;
                self.control_body(body)
            }
            StmtKind::Labeled { label, body } => {
                if self.collecting_labels {
                    let entry = self.bind_label(label)?;
                    self.resolution.label_definitions.insert(label.id, entry.id);
                }
                self.visit_stmt(body)
            }
            StmtKind::SwitchLabel { label, body } => {
                match label {
                    crate::ast::SwitchLabel::Case(value) => self.visit_expr(value)?,
                    crate::ast::SwitchLabel::CaseRange { start, end } => {
                        self.visit_expr(start)?;
                        self.visit_expr(end)?;
                    }
                    crate::ast::SwitchLabel::Default => {}
                }
                self.visit_stmt(body)
            }
            StmtKind::Asm(asm) => {
                if let Some(operands) = &asm.operands {
                    for operand in operands.outputs.iter().chain(&operands.inputs) {
                        self.visit_expr(&operand.expr)?;
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
            self.visit_stmt(statement)?;
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
            ExprKind::Cast { ty, value }
            | ExprKind::BitCast { ty, value }
            | ExprKind::ConvertVector { ty, value }
            | ExprKind::VaArg { list: value, ty } => {
                self.type_name(ty, expr)?;
                self.visit_expr(value)
            }
            ExprKind::Call { callee, arguments } => {
                let implicit_builtin = match &callee.value {
                    ExprKind::Identifier(name) if self.lookup_ordinary(name).is_none() => {
                        super::builtins::clang_builtin(name, self.flavor)
                    }
                    _ => None,
                };
                // GCC's `__builtin_exit` calls whatever `exit` is declared
                // in scope, so bind the prefixed spelling to it.
                if let (Some(builtin), ExprKind::Identifier(name)) =
                    (implicit_builtin, &callee.value)
                {
                    let visible = (builtin.name != name)
                        .then(|| self.lookup_ordinary(builtin.name))
                        .flatten();
                    match visible
                        .or_else(|| self.linked.get(builtin.name))
                        .filter(|entry| entry.kind == BindingKind::Function)
                        .cloned()
                    {
                        Some(entry) => self.push_reference(builtin.name, entry, callee),
                        None => self
                            .implicit_builtin_calls
                            .entry(builtin.name)
                            .or_default()
                            .push(copy_span(callee, ())),
                    }
                }
                if implicit_builtin.is_none()
                    && !super::expression::specially_lowered(callee, arguments)
                {
                    self.visit_expr(callee)?;
                }
                for argument in arguments {
                    self.visit_expr(argument)?;
                }
                Ok(())
            }
            ExprKind::CompoundLiteral { ty, initializer } => {
                self.type_name(ty, expr)?;
                for item in initializer {
                    self.visit_initializer_item(item)?;
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
                    crate::ast::GenericControl::Expr(value) => self.visit_expr(value)?,
                    crate::ast::GenericControl::Type { ty } => self.type_name(ty, expr)?,
                }
                for association in associations {
                    match association {
                        crate::ast::GenericAssociation::Type { ty, value } => {
                            self.type_name(ty, expr)?;
                            self.visit_expr(value)?;
                        }
                        crate::ast::GenericAssociation::Default(value) => self.visit_expr(value)?,
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
            _ => crate::visit::walk_expr(self, expr),
        }
    }

    fn offsetof_member(&mut self, member: &Expr) -> Result<(), ResolveError> {
        match &member.value {
            ExprKind::Identifier(_) => Ok(()),
            ExprKind::Member { base, .. } => self.offsetof_member(base),
            ExprKind::Index { base, index } => {
                self.offsetof_member(base)?;
                self.visit_expr(index)
            }
            _ => self.visit_expr(member),
        }
    }

    fn type_name<T>(&mut self, ty: &TypeName, span: &Span<T>) -> Result<(), ResolveError> {
        self.type_specifier(&ty.specifiers.ty, span)?;
        self.visit_declarator(&ty.declarator)
    }

    fn declarator(&mut self, declarator: &Declarator) -> Result<(), ResolveError> {
        match declarator {
            Declarator::Abstract | Declarator::Name(_) => Ok(()),
            Declarator::Grouped(inner) => self.visit_declarator(inner),
            Declarator::Attributed { inner, attributes }
            | Declarator::Pointer {
                inner, attributes, ..
            } => {
                self.visit_declarator(inner)?;
                crate::visit::walk_attributes(self, attributes)
            }
            Declarator::Array { inner, size, .. } => {
                self.visit_declarator(inner)?;
                if let ArraySize::Expression(value) = size {
                    self.visit_expr(value)?;
                }
                Ok(())
            }
            Declarator::Function { inner, parameters } => {
                self.visit_declarator(inner)?;
                let parameters = parameters.parameters();
                if !parameters.is_empty() {
                    self.push_scope();
                    let result = parameters.iter().try_for_each(|parameter| {
                        self.type_specifier(&parameter.specifiers.ty, parameter)?;
                        if self.collecting_labels {
                            self.visit_declarator(&parameter.declarator)?;
                        }
                        Ok(())
                    });
                    self.pop_scope();
                    result?;
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
            | TypeSpecifier::TypeOfUnqual(TypeOfOperand::Expression(value)) => {
                self.visit_expr(value)
            }
            TypeSpecifier::TypeOf(TypeOfOperand::Type(ty))
            | TypeSpecifier::TypeOfUnqual(TypeOfOperand::Type(ty)) => self.type_name(ty, span),
            TypeSpecifier::Integer(crate::ast::IntegerType::BitInt { width, .. }) => {
                self.visit_expr(width)
            }
            TypeSpecifier::Mode(mode) => self.type_specifier(&mode.base, span),
            TypeSpecifier::Vector(vector) => {
                self.type_specifier(&vector.element, span)?;
                match &vector.size {
                    crate::ast::VectorSize::Bytes(value) | crate::ast::VectorSize::Lanes(value) => {
                        self.visit_expr(value)
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
            let declared = tag
                .name
                .is_some()
                .then(|| self.tags.last().and_then(|scope| scope.get(&name)).cloned())
                .flatten();
            let entry = match declared {
                Some(entry) => entry,
                None => {
                    let entry = self.new_entry(&name, BindingKind::Tag, span);
                    if tag.name.is_some() {
                        self.tags
                            .last_mut()
                            .unwrap()
                            .insert(name.clone(), entry.clone());
                    }
                    entry
                }
            };
            self.tag_ids.insert(id, entry);
        }
        match &tag.body {
            TagBody::Enum { enumerators, .. } => {
                for item in enumerators {
                    if let EnumItemKind::Enumerator(enumerator) = &item.value {
                        if let Some(value) = &enumerator.value {
                            self.visit_expr(value)?;
                        }
                        if !self.collecting_labels {
                            self.bind_ordinary(
                                &enumerator.name,
                                BindingKind::Enumerator,
                                false,
                                item,
                            )?;
                        }
                    }
                }
            }
            TagBody::Record(fields) => {
                for field in fields {
                    if let crate::ast::FieldItemKind::Field(field) = &field.value {
                        self.type_specifier(&field.specifiers.ty, &tag)?;
                        for declarator in &field.declarators {
                            self.visit_declarator(&declarator.declarator)?;
                            if let Some(width) = &declarator.bit_width {
                                self.visit_expr(width)?;
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
        linked: bool,
        span: &Span<T>,
    ) -> Result<Entry, ResolveError> {
        if let Some(existing) = self.ordinary.last().unwrap().get(name).cloned() {
            if (self.ordinary.len() == 1 || linked) && redeclares(existing.kind, kind) {
                self.resolution.declarations.insert(span.id, existing.id);
                return Ok(existing);
            }
            return Err(ResolveError::Duplicate {
                namespace: "ordinary",
                name: name.into(),
            });
        }
        let entry = match self.linked.get(name) {
            Some(entry) if linked && redeclares(entry.kind, kind) => entry.clone(),
            _ => self.new_entry(name, kind, span),
        };
        if linked {
            self.linked.insert(name.into(), entry.clone());
            if kind == BindingKind::Function
                && let Some(calls) = self.implicit_builtin_calls.remove(name)
            {
                for call in calls {
                    self.push_reference(name, entry.clone(), &call);
                }
            }
        }
        self.ordinary
            .last_mut()
            .unwrap()
            .insert(name.into(), entry.clone());
        self.resolution.declarations.insert(span.id, entry.id);
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
        if super::expression::predefined_function_name(name) && self.lookup_ordinary(name).is_none()
        {
            return Ok(());
        }
        let entry =
            self.lookup_ordinary(name)
                .cloned()
                .ok_or_else(|| ResolveError::Unresolved {
                    namespace: "ordinary",
                    name: name.into(),
                })?;
        self.push_reference(name, entry, span);
        Ok(())
    }

    fn lookup_ordinary(&self, name: &str) -> Option<&Entry> {
        self.ordinary.iter().rev().find_map(|scope| scope.get(name))
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

    fn declare_incomplete_tag<T>(
        &mut self,
        name: &str,
        span: &Span<T>,
    ) -> Result<(), ResolveError> {
        if self.collecting_labels {
            return Ok(());
        }
        if self
            .tags
            .last()
            .is_some_and(|scope| scope.contains_key(name))
        {
            return self.reference_tag(name, span);
        }
        self.declare_tag_in_scope(name, span);
        Ok(())
    }

    fn reference_tag<T>(&mut self, name: &str, span: &Span<T>) -> Result<(), ResolveError> {
        if self.collecting_labels {
            return Ok(());
        }
        let visible = self
            .tags
            .iter()
            .rev()
            .find_map(|scope| scope.get(name))
            .cloned();
        let entry = match visible {
            Some(entry) => entry,
            None => self.declare_tag_in_scope(name, span),
        };
        self.push_reference(name, entry, span);
        Ok(())
    }

    fn declare_tag_in_scope<T>(&mut self, name: &str, span: &Span<T>) -> Entry {
        let entry = self.new_entry(name, BindingKind::Tag, span);
        if let Some(scope) = self.tags.last_mut() {
            scope.insert(name.to_owned(), entry.clone());
        }
        entry
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

impl Visitor for Resolver {
    type Error = ResolveError;

    fn visit_translation_unit(&mut self, unit: &TranslationUnit) -> Result<(), Self::Error> {
        for declaration in &unit.decls {
            self.visit_decl(declaration)?;
        }
        Ok(())
    }

    fn visit_decl(&mut self, decl: &Decl) -> Result<(), Self::Error> {
        self.declaration_node(decl)
    }

    fn visit_stmt(&mut self, stmt: &Stmt) -> Result<(), Self::Error> {
        self.statement(stmt)
    }

    fn visit_expr(&mut self, expr: &Expr) -> Result<(), Self::Error> {
        self.expr(expr)
    }

    fn visit_initializer(&mut self, initializer: &Initializer) -> Result<(), Self::Error> {
        crate::visit::walk_initializer(self, initializer)
    }

    fn visit_initializer_item(&mut self, item: &InitializerItem) -> Result<(), Self::Error> {
        crate::visit::walk_initializer_item(self, item)
    }

    fn visit_declarator(&mut self, declarator: &Declarator) -> Result<(), Self::Error> {
        self.declarator(declarator)
    }
}

fn redeclares(existing: BindingKind, declared: BindingKind) -> bool {
    let has_linkage = |kind| matches!(kind, BindingKind::Object | BindingKind::Function);
    existing == declared || (has_linkage(existing) && has_linkage(declared))
}

fn copy_span<T, U>(span: &Span<T>, value: U) -> Span<U> {
    Span {
        id: span.id,
        value,
        spelling: span.spelling,
        expansion: span.expansion,
        provenance: span.provenance,
        macro_origin: span.macro_origin.clone(),
        leading_space: span.leading_space,
    }
}
