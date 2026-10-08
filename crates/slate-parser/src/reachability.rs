use crate::ast::*;
use crate::compiler_options::InlineSemantics;
use crate::target_info::TargetEnvironment;
use std::collections::{HashMap, HashSet};

pub fn filter_translation_unit(
    tu: &TranslationUnit,
    root_file: FileId,
    forced_roots: &[FileId],
) -> TranslationUnit {
    let mut reachability = Reachability::new(tu);
    reachability.mark_roots(root_file, forced_roots);
    TranslationUnit {
        dialect: tu.dialect.clone(),
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
            .map(|mut tag| {
                reachability.retain_root_comments(&mut tag.value.body);
                tag
            })
            .collect(),
    }
}

// a tag first named inside a member declaration has file scope, like one named at top level
#[derive(Default)]
struct FileScopeNames<'a> {
    referenced_tags: Vec<&'a str>,
    defined: Vec<&'a str>,
}

impl<'a> FileScopeNames<'a> {
    fn collect(&mut self, tu: &'a TranslationUnit, ty: &'a TypeSpecifier) {
        match ty {
            TypeSpecifier::Tag(TagSpecifier::Reference { name, .. }) => {
                self.referenced_tags.push(&name.value)
            }
            TypeSpecifier::Tag(TagSpecifier::Definition(tag_id)) => {
                let Some(tag) = tu.tag(*tag_id) else {
                    return;
                };
                if let Some(name) = &tag.value.name {
                    self.defined.push(name);
                }
                match &tag.value.body {
                    TagBody::Enum { enumerators, .. } => {
                        for item in enumerators {
                            if let EnumItemKind::Enumerator(enumerator) = &item.value {
                                self.defined.push(&enumerator.name);
                            }
                        }
                    }
                    TagBody::Record(fields) => {
                        for field in fields {
                            if let FieldItemKind::Field(field) = &field.value {
                                self.collect(tu, &field.specifiers.ty);
                            }
                        }
                    }
                }
            }
            _ => {}
        }
    }
}

struct Reachability<'a> {
    tu: &'a TranslationUnit,
    nodes: &'a [Decl],
    symbols: HashMap<String, Vec<usize>>,
    reachable: HashSet<usize>,
    reachable_tags: HashSet<TagId>,
    root_files: Vec<FileId>,
}

impl<'a> Reachability<'a> {
    fn new(tu: &'a TranslationUnit) -> Self {
        let mut symbols: HashMap<String, Vec<usize>> = HashMap::new();
        let mut introduced_tags = HashSet::new();
        for (id, decl) in tu.decls.iter().enumerate() {
            for name in decl.names() {
                symbols.entry(name.to_string()).or_default().push(id);
            }
            let specifiers = match &decl.value {
                DeclKind::Declaration(declaration) => &declaration.specifiers.ty,
                DeclKind::Function(function) => &function.specifiers.ty,
                _ => continue,
            };
            let mut introduced = FileScopeNames::default();
            introduced.collect(tu, specifiers);
            for name in introduced.referenced_tags {
                if introduced_tags.insert(name) {
                    symbols.entry(name.to_string()).or_default().push(id);
                }
            }
            for name in introduced.defined {
                introduced_tags.insert(name);
                symbols.entry(name.to_string()).or_default().push(id);
            }
        }
        Self {
            tu,
            nodes: &tu.decls,
            symbols,
            reachable: HashSet::new(),
            reachable_tags: HashSet::new(),
            root_files: Vec::new(),
        }
    }

    fn mark_roots(&mut self, root_file: FileId, forced_roots: &[FileId]) {
        self.root_files = std::iter::once(root_file)
            .chain(forced_roots.iter().copied())
            .collect();
        let mut roots = self
            .nodes
            .iter()
            .enumerate()
            .filter_map(|(id, decl)| {
                (decl.provenance.file == root_file || forced_roots.contains(&decl.provenance.file))
                    .then_some(id)
            })
            .collect::<Vec<_>>();
        roots.extend(self.nodes.iter().enumerate().filter_map(|(id, decl)| {
            (matches!(decl.value, DeclKind::Pragma(_))
                || self.has_retention_attribute(decl)
                || self.defines_external_symbol(decl))
            .then_some(id)
        }));
        for id in roots {
            self.mark(id);
        }
    }

    fn retain_root_comments(&self, body: &mut TagBody) {
        match body {
            TagBody::Record(fields) => fields.retain(|item| {
                !matches!(item.value, FieldItemKind::Comment(_))
                    || self.root_files.contains(&item.provenance.file)
            }),
            TagBody::Enum { enumerators, .. } => enumerators.retain(|item| {
                !matches!(item.value, EnumItemKind::Comment(_))
                    || self.root_files.contains(&item.provenance.file)
            }),
        }
    }

    fn mark(&mut self, id: usize) {
        if !self.reachable.insert(id) {
            return;
        }
        let mut previous = id;
        while previous > 0 {
            previous -= 1;
            if self.nodes[previous].expansion.file != self.nodes[id].expansion.file
                || !self
                    .root_files
                    .contains(&self.nodes[previous].provenance.file)
                || !matches!(self.nodes[previous].value, DeclKind::Comment(_))
            {
                break;
            }
            self.reachable.insert(previous);
        }
        match &self.nodes[id].value {
            DeclKind::Comment(_) | DeclKind::Pragma(_) => {}
            DeclKind::Attribute(attributes) => self.mark_attributes(attributes),
            DeclKind::Asm(asm) => self.mark_gnu_asm(asm),
            DeclKind::StaticAssert(assertion) => self.mark_expr(&assertion.condition),
            DeclKind::Function(function) => self.mark_function(function),
            DeclKind::Declaration(declaration) => self.mark_declaration(declaration),
        }
        let nodes = self.nodes;
        for name in nodes[id].names() {
            self.mark_name(name);
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
                    if let FieldItemKind::StaticAssert(assertion) = &field.value {
                        self.mark_expr(&assertion.condition);
                    }
                    if let FieldItemKind::Field(field) = &field.value {
                        self.mark_attributes(&field.specifiers.attributes);
                        self.mark_type(&field.specifiers.ty);
                        for declarator in &field.declarators {
                            self.mark_attributes(&declarator.attributes);
                            self.mark_declarator(&declarator.declarator);
                            if let Some(width) = &declarator.bit_width {
                                self.mark_expr(width);
                            }
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
            StmtKind::IfDeclaration {
                declaration,
                condition,
                then_branch,
                else_branch,
            } => {
                self.mark_declaration(declaration);
                if let Some(condition) = condition {
                    self.mark_expr(condition);
                }
                self.mark_stmt(then_branch);
                if let Some(branch) = else_branch {
                    self.mark_stmt(branch);
                }
            }
            StmtKind::SwitchDeclaration {
                declaration,
                discriminant,
                body,
            } => {
                self.mark_declaration(declaration);
                if let Some(discriminant) = discriminant {
                    self.mark_expr(discriminant);
                }
                self.mark_stmt(body);
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
            StmtKind::MsAsm(asm) => {
                for instruction in &asm.instructions {
                    for operand in &instruction.value.operands {
                        self.mark_ms_asm_expr(operand);
                    }
                }
            }
            StmtKind::Attribute(attributes) => self.mark_attributes(attributes),
            StmtKind::Attributed { attributes, body } => {
                self.mark_attributes(attributes);
                self.mark_stmt(body);
            }
            StmtKind::StaticAssert(assertion) => self.mark_expr(&assertion.condition),
            StmtKind::Asm(asm) => self.mark_gnu_asm(asm),
            StmtKind::Null
            | StmtKind::Comment(_)
            | StmtKind::ReturnVoid
            | StmtKind::LocalLabelDecl(_)
            | StmtKind::Goto(_)
            | StmtKind::NamedBreak(_)
            | StmtKind::NamedContinue(_)
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
            ExprKind::SizeOfType { ty }
            | ExprKind::AlignOf { ty }
            | ExprKind::CountOfType { ty }
            | ExprKind::MaxOf { ty }
            | ExprKind::MinOf { ty } => self.mark_type_name(ty),
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
            | ExprKind::ConvertVector { ty, value }
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
            | ExprKind::CountOfExpr(value)
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

    fn mark_gnu_asm(&mut self, asm: &GnuAsm) {
        let Some(operands) = &asm.operands else {
            return;
        };
        for operand in operands.outputs.iter().chain(&operands.inputs) {
            self.mark_expr(&operand.expr);
        }
    }

    fn mark_ms_asm_expr(&mut self, expr: &Span<MsAsmExpr>) {
        match &expr.value {
            MsAsmExpr::Name(name) => self.mark_name(name),
            MsAsmExpr::Register(_)
            | MsAsmExpr::SegmentRegister(_)
            | MsAsmExpr::St(_)
            | MsAsmExpr::Number(_)
            | MsAsmExpr::TypeKeyword(_) => {}
            MsAsmExpr::Member { base, .. } => self.mark_ms_asm_expr(base),
            MsAsmExpr::Index {
                base: lhs,
                index: rhs,
            }
            | MsAsmExpr::Binary { lhs, rhs, .. } => {
                self.mark_ms_asm_expr(lhs);
                self.mark_ms_asm_expr(rhs);
            }
            MsAsmExpr::Bracket(operand)
            | MsAsmExpr::Negate(operand)
            | MsAsmExpr::Ptr { operand, .. }
            | MsAsmExpr::Segment { operand, .. }
            | MsAsmExpr::Operator { operand, .. } => self.mark_ms_asm_expr(operand),
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
            TypeSpecifier::Mode(mode) => self.mark_type(&mode.base),
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
            TypeSpecifier::TargetBuiltin(_) | TypeSpecifier::Inferred => {}
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
            | DeclKind::Pragma(_)
            | DeclKind::Attribute(_) => false,
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

    fn attributes_retain(&self, attributes: &[Span<Attribute>]) -> bool {
        attributes.iter().any(|attribute| {
            matches!(
                &attribute.value,
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

    fn mark_attributes(&mut self, attributes: &[Span<Attribute>]) {
        for attribute in attributes {
            match &attribute.value {
                Attribute::Alias(name)
                | Attribute::WeakRef(Some(name))
                | Attribute::Ifunc(name) => self.mark_name(name),
                Attribute::AddressSpace(value)
                | Attribute::PassObjectSize {
                    size_type: value, ..
                }
                | Attribute::Aligned(value)
                | Attribute::VectorSize(value)
                | Attribute::AllocAlign(value)
                | Attribute::Cleanup(value)
                | Attribute::ExtVectorType(value)
                | Attribute::CallingConvention(CallingConvention::RegParm(value))
                | Attribute::AlignAs(AlignAsOperand::Expr(value)) => self.mark_expr(value),
                Attribute::AlignAs(AlignAsOperand::Type { ty }) => self.mark_type_name(ty),
                Attribute::AssumeAligned(values) | Attribute::AllocSize(values) => {
                    for value in values {
                        self.mark_expr(value);
                    }
                }
                Attribute::Malloc {
                    deallocator,
                    argument,
                } => {
                    for value in deallocator.iter().chain(argument) {
                        self.mark_expr(value);
                    }
                }
                _ => {}
            }
        }
    }

    // clang emits these whatever file they come from, so an included .c must keep them.
    fn defines_external_symbol(&self, decl: &Decl) -> bool {
        match &decl.value {
            DeclKind::Function(function) => {
                function.specifiers.storage != StorageClass::Static
                    && (!function.specifiers.is_inline || self.emits_inline_definition(function))
            }
            DeclKind::Declaration(declaration) => match declaration.specifiers.storage {
                StorageClass::None => declaration.declarators.iter().any(|declarator| {
                    !self.declares_function(&declaration.specifiers.ty, &declarator.declarator)
                }),
                StorageClass::Extern => declaration
                    .declarators
                    .iter()
                    .any(|declarator| declarator.initializer.is_some()),
                _ => false,
            },
            DeclKind::Comment(_)
            | DeclKind::StaticAssert(_)
            | DeclKind::Asm(_)
            | DeclKind::Pragma(_)
            | DeclKind::Attribute(_) => false,
        }
    }

    // mirrors clang's GVA linkage for inline functions (basicGVALinkageForFunction).
    fn emits_inline_definition(&self, definition: &FunctionDefinition) -> bool {
        let Some(name) = definition.declarator.name() else {
            return false;
        };
        let redeclarations = self.function_redeclarations(name);
        let has_attribute = |wanted: fn(&Attribute) -> bool| {
            redeclarations
                .iter()
                .flat_map(|(_, attributes)| attributes)
                .any(|attribute| wanted(&attribute.value))
        };
        let semantics = if has_attribute(|attribute| matches!(attribute, Attribute::GnuInline)) {
            InlineSemantics::SupressDef
        } else if self.tu.dialect.target().environment == TargetEnvironment::Msvc {
            return has_attribute(|attribute| matches!(attribute, Attribute::DllExport))
                || redeclarations
                    .iter()
                    .any(|(specifiers, _)| specifiers.storage == StorageClass::Extern);
        } else {
            self.tu.dialect.inline_semantics()
        };
        let is_extern = definition.specifiers.storage == StorageClass::Extern;
        match semantics {
            InlineSemantics::SupressDef => !is_extern,
            InlineSemantics::ProvideDef => {
                is_extern
                    || redeclarations.iter().any(|(specifiers, _)| {
                        !specifiers.is_inline || specifiers.storage == StorageClass::Extern
                    })
            }
        }
    }

    fn function_redeclarations(
        &self,
        name: &str,
    ) -> Vec<(&'a DeclarationSpecifiers, Vec<&'a Span<Attribute>>)> {
        let nodes = self.nodes;
        let mut redeclarations = Vec::new();
        for id in self.symbols.get(name).into_iter().flatten() {
            match &nodes[*id].value {
                DeclKind::Function(function) if function.declarator.name() == Some(name) => {
                    redeclarations.push((
                        &function.specifiers,
                        function
                            .specifiers
                            .attributes_with(&function.declarator, &function.attributes)
                            .collect(),
                    ))
                }
                DeclKind::Declaration(declaration)
                    if declaration.specifiers.storage != StorageClass::Typedef =>
                {
                    for declarator in &declaration.declarators {
                        if declarator.declarator.name() == Some(name) {
                            redeclarations.push((
                                &declaration.specifiers,
                                declaration
                                    .specifiers
                                    .attributes_with(&declarator.declarator, &declarator.attributes)
                                    .collect(),
                            ));
                        }
                    }
                }
                _ => {}
            }
        }
        redeclarations
    }

    fn declares_function(&self, ty: &TypeSpecifier, declarator: &Declarator) -> bool {
        if declarator.function_parameters().is_some() {
            return true;
        }
        if declarator.is_derived() {
            return false;
        }
        let TypeSpecifier::Named(name) = ty else {
            return false;
        };
        let name = name.value.as_str();
        self.symbols
            .get(name)
            .into_iter()
            .flatten()
            .filter_map(|id| match &self.nodes[*id].value {
                DeclKind::Declaration(typedef)
                    if typedef.specifiers.storage == StorageClass::Typedef
                        && typedef.specifiers.ty != *ty =>
                {
                    Some(typedef)
                }
                _ => None,
            })
            .flat_map(|typedef| {
                typedef
                    .declarators
                    .iter()
                    .map(move |declarator| (&typedef.specifiers.ty, &declarator.declarator))
            })
            .any(|(ty, declarator)| {
                declarator.name() == Some(name) && self.declares_function(ty, declarator)
            })
    }
}
