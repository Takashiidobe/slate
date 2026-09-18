use crate::ast::*;
use crate::compiler_args::{CompilerFlavor, LanguageStandard};
use crate::const_expr::{CharLiteral, Encoding, IntegerLiteral, IntegerSizeSuffix, Radix, UnaryOp};
use crate::diagnostics::{DiagnosticOptions, Warning};
use crate::files::{Files, decode_source_bytes, display_path};
use crate::standard_features::{Availability, StandardFeatures};
use crate::target_info::TargetInfo;
use miette::{Diagnostic, LabeledSpan, NamedSource, Severity, SourceCode, SourceSpan};
use num_bigint::BigUint;
use std::collections::{HashMap, HashSet};
use thiserror::Error;

#[derive(Debug, Error, Clone)]
#[error("{message}")]
pub struct SemaError {
    pub message: String,
    pub severity: Severity,
    pub warning: Option<Warning>,
    pub provenance: Option<Provenance>,
    pub loc: Option<Loc>,
    pub source_code: NamedSource<String>,
    pub span: SourceSpan,
}

impl Diagnostic for SemaError {
    fn severity(&self) -> Option<Severity> {
        Some(self.severity)
    }

    fn code(&self) -> Option<Box<dyn std::fmt::Display + '_>> {
        self.warning
            .map(|warning| Box::new(format!("-W{warning}")) as Box<dyn std::fmt::Display>)
    }

    fn source_code(&self) -> Option<&dyn SourceCode> {
        Some(&self.source_code)
    }

    fn labels(&self) -> Option<Box<dyn Iterator<Item = LabeledSpan> + '_>> {
        Some(Box::new(std::iter::once(LabeledSpan::underline(self.span))))
    }
}

#[derive(Debug, Error, Diagnostic)]
#[error("semantic analysis failed")]
pub struct SemaErrors {
    #[related]
    pub errors: Vec<SemaError>,
}

impl TranslationUnit {
    pub fn analyze(&self, files: &Files) -> Result<Vec<SemaError>, SemaErrors> {
        let flavor = self.flavor;
        let features = StandardFeatures::new(self.standard);
        let literals = LiteralContext {
            target: &self.target,
            features,
            diagnostics: &self.options.diagnostics,
            standard: self.standard,
            flavor,
        };
        let typedefs = self
            .decls
            .iter()
            .filter_map(|decl| match &decl.value {
                DeclKind::Declaration(declaration)
                    if declaration.specifiers.storage == StorageClass::Typedef =>
                {
                    Some(declaration.names().map(str::to_string))
                }
                _ => None,
            })
            .flatten()
            .collect::<HashSet<_>>();
        let mut tags = HashSet::new();
        for tag in &self.tags {
            if let Some(name) = &tag.value.name {
                tags.insert(name.clone());
            }
            if let TagBody::Record(fields) = &tag.value.body {
                for field_item in fields {
                    if let FieldItemKind::Field(field) = &field_item.value {
                        collect_tag_names(&field.specifiers.ty, &mut tags);
                    }
                }
            }
        }
        for decl in &self.decls {
            match &decl.value {
                DeclKind::Function(function) => {
                    collect_tag_names(&function.specifiers.ty, &mut tags);
                    for parameter in function
                        .declarator
                        .function_parameters()
                        .map_or(&[][..], ParameterList::parameters)
                    {
                        collect_tag_names(&parameter.specifiers.ty, &mut tags);
                    }
                }
                DeclKind::Declaration(declaration) => {
                    collect_tag_names(&declaration.specifiers.ty, &mut tags);
                }
                DeclKind::Comment(_)
                | DeclKind::StaticAssert { .. }
                | DeclKind::Asm { .. }
                | DeclKind::Pragma(_) => {}
            }
        }

        let types = TypeContext {
            typedefs: &typedefs,
            tags: &tags,
            features,
            diagnostics: &self.options.diagnostics,
            standard: self.standard,
        };
        let mut errors = super::assertion::validate(self);
        for decl in &self.decls {
            match &decl.value {
                DeclKind::Comment(_)
                | DeclKind::StaticAssert { .. }
                | DeclKind::Asm { .. }
                | DeclKind::Pragma(_) => {}
                DeclKind::Function(function) => {
                    let provenance = decl.provenance;
                    check_attributes(
                        &function.specifiers.attributes,
                        provenance,
                        decl.expansion,
                        &mut errors,
                    );
                    check_attributes(
                        &function.attributes,
                        provenance,
                        decl.expansion,
                        &mut errors,
                    );
                    check_type(
                        &function.specifiers.ty,
                        types,
                        provenance,
                        decl.expansion,
                        &mut errors,
                    );
                    if flavor == CompilerFlavor::Clang {
                        check_function_asm(self, function, provenance, &mut errors);
                    }
                    check_literals(function, literals, provenance, &mut errors);
                    check_body_types(function, types, provenance, &mut errors);
                }
                DeclKind::Declaration(declaration) => {
                    let provenance = decl.provenance;
                    let specifiers = &declaration.specifiers;
                    if let TypeSpecifier::Tag(TagSpecifier::Definition(id)) = &specifiers.ty
                        && let Some(tag) = self.tag(*id)
                    {
                        check_tag_definition(tag, types, decl.expansion, &mut errors);
                    }
                    check_type(
                        &specifiers.ty,
                        types,
                        provenance,
                        decl.expansion,
                        &mut errors,
                    );
                    check_attributes(
                        &specifiers.attributes,
                        provenance,
                        decl.expansion,
                        &mut errors,
                    );
                    check_declaration_literals(declaration, literals, provenance, &mut errors);
                    for init_declarator in &declaration.declarators {
                        let declarator = &init_declarator.declarator;
                        if matches!(specifiers.ty, TypeSpecifier::Void)
                            && !matches!(
                                specifiers.storage,
                                StorageClass::Extern | StorageClass::Typedef
                            )
                            && declarator.name().is_some()
                            && !declarator_indirects_void(declarator)
                        {
                            errors.push(error(
                                provenance,
                                decl.expansion,
                                "object cannot have type void",
                            ));
                        }
                        check_declarator(
                            declarator,
                            types,
                            init_declarator.provenance,
                            decl.expansion,
                            &mut errors,
                        );
                        check_attributes(
                            &init_declarator.attributes,
                            init_declarator.provenance,
                            decl.expansion,
                            &mut errors,
                        );
                        if flavor == CompilerFlavor::Clang {
                            check_register_variable(
                                self,
                                specifiers,
                                init_declarator,
                                true,
                                init_declarator.provenance,
                                decl.expansion,
                                &mut errors,
                            );
                        }
                    }
                }
            }
        }
        let errors: Vec<SemaError> = errors
            .into_iter()
            .map(|error| error.with_source(files))
            .collect();
        if errors.iter().any(|error| error.severity == Severity::Error) {
            Err(SemaErrors { errors })
        } else {
            Ok(errors)
        }
    }
}

impl SemaError {
    fn with_source(mut self, files: &Files) -> Self {
        let Some(loc) = self.loc else {
            return self;
        };
        let path = files.path(loc.file);
        let source = std::fs::read(path)
            .map(|bytes| decode_source_bytes(&bytes))
            .unwrap_or_default();
        self.source_code = NamedSource::new(display_path(path), source).with_language("C");
        self.span = SourceSpan::new(loc.offset.into(), loc.length.max(1));
        self
    }
}

fn declarator_indirects_void(declarator: &Declarator) -> bool {
    match declarator {
        Declarator::Grouped(inner)
        | Declarator::Attributed { inner, .. }
        | Declarator::Array { inner, .. } => declarator_indirects_void(inner),
        Declarator::Pointer { .. } | Declarator::Function { .. } => true,
        Declarator::Abstract | Declarator::Name(_) => false,
    }
}

fn walk_type(ty: &TypeSpecifier, visit: &mut impl FnMut(&TypeSpecifier)) {
    visit(ty);
    match ty {
        TypeSpecifier::Atomic(ty)
        | TypeSpecifier::TypeOf(TypeOfOperand::Type(ty))
        | TypeSpecifier::TypeOfUnqual(TypeOfOperand::Type(ty)) => {
            walk_type(&ty.specifiers.ty, visit)
        }
        TypeSpecifier::Vector(vector) => walk_type(&vector.element, visit),
        TypeSpecifier::Imaginary(ty) => walk_type(ty, visit),
        TypeSpecifier::Tag(TagSpecifier::Reference {
            fixed_type: Some(fixed_type),
            ..
        }) => walk_type(&fixed_type.specifiers.ty, visit),
        TypeSpecifier::Void
        | TypeSpecifier::Bool
        | TypeSpecifier::Integer(_)
        | TypeSpecifier::Floating(_)
        | TypeSpecifier::Complex(_)
        | TypeSpecifier::FixedPoint(_)
        | TypeSpecifier::TypeOf(TypeOfOperand::Expression(_))
        | TypeSpecifier::TypeOfUnqual(TypeOfOperand::Expression(_))
        | TypeSpecifier::TargetBuiltin(_)
        | TypeSpecifier::Named(_)
        | TypeSpecifier::Tag(_) => {}
    }
}

fn extension_warning(
    ty: &TypeSpecifier,
    features: StandardFeatures,
) -> Option<(Warning, &'static str)> {
    match ty {
        TypeSpecifier::Integer(IntegerType::Ranked {
            rank: IntegerRank::LongLong,
            ..
        }) if features.long_long_type != Availability::Standard => Some((
            Warning::LongLong,
            "'long long' is an extension when C99 mode is not enabled",
        )),
        _ => None,
    }
}

fn check_extensions(
    ty: &TypeSpecifier,
    context: TypeContext<'_>,
    provenance: Provenance,
    loc: Loc,
    errors: &mut Vec<SemaError>,
) {
    let Some((warning, message)) = extension_warning(ty, context.features) else {
        return;
    };
    errors.extend(warning.diagnose(
        message,
        context.diagnostics,
        context.standard,
        provenance,
        loc,
    ));
}

#[derive(Clone, Copy)]
struct TypeContext<'a> {
    typedefs: &'a HashSet<String>,
    tags: &'a HashSet<String>,
    features: StandardFeatures,
    diagnostics: &'a DiagnosticOptions,
    standard: LanguageStandard,
}

fn check_declarator(
    declarator: &Declarator,
    context: TypeContext<'_>,
    provenance: Provenance,
    loc: Loc,
    errors: &mut Vec<SemaError>,
) {
    match declarator {
        Declarator::Function {
            parameters, inner, ..
        } => {
            check_declarator(inner, context, provenance, loc, errors);
            for parameter in parameters.parameters() {
                check_type(&parameter.specifiers.ty, context, provenance, loc, errors);
                check_attributes(&parameter.attributes, provenance, loc, errors);
                check_declarator(&parameter.declarator, context, provenance, loc, errors);
            }
        }
        Declarator::Attributed { inner, attributes } => {
            check_attributes(attributes, provenance, loc, errors);
            check_declarator(inner, context, provenance, loc, errors);
        }
        Declarator::Pointer {
            inner, attributes, ..
        } => {
            check_attributes(attributes, provenance, loc, errors);
            check_declarator(inner, context, provenance, loc, errors);
        }
        Declarator::Grouped(inner) | Declarator::Array { inner, .. } => {
            check_declarator(inner, context, provenance, loc, errors)
        }
        Declarator::Abstract | Declarator::Name(_) => {}
    }
}

fn check_attributes(
    attributes: &[Attribute],
    provenance: Provenance,
    loc: Loc,
    errors: &mut Vec<SemaError>,
) {
    for attribute in attributes {
        if let Attribute::Invalid { name, .. } = attribute {
            errors.push(error(
                provenance,
                loc,
                format!("invalid arguments for attribute `{name}`"),
            ));
        }
        if let Attribute::Aligned(expression)
        | Attribute::VectorSize(expression)
        | Attribute::AlignAs(AlignAsOperand::Expr(expression)) = attribute
            && !is_integer_constant_expression(expression)
        {
            errors.push(error(
                provenance,
                loc,
                "layout attribute requires an integer constant expression",
            ));
        }
        if let Attribute::AllocSize(expressions) = attribute
            && !(1..=2).contains(&expressions.len())
        {
            errors.push(error(
                provenance,
                loc,
                "alloc_size expects one or two arguments",
            ));
        }
    }
}

fn is_integer_constant_expression(expression: &Expr) -> bool {
    match &expression.value {
        ExprKind::IntegerLiteral(_)
        | ExprKind::CharLiteral(_)
        | ExprKind::SizeOfExpr(_)
        | ExprKind::SizeOfType { .. }
        | ExprKind::AlignOf { .. }
        | ExprKind::AlignOfExpr(_)
        | ExprKind::BoolLiteral(_) => true,
        ExprKind::Unary { op, operand } => {
            matches!(
                op,
                UnaryOp::Plus
                    | UnaryOp::Minus
                    | UnaryOp::BitNot
                    | UnaryOp::Not
                    | UnaryOp::Real
                    | UnaryOp::Imag
            ) && is_integer_constant_expression(operand)
        }
        ExprKind::Paren(value) | ExprKind::Cast { value, .. } => {
            is_integer_constant_expression(value)
        }
        ExprKind::Binary { left, right, .. } => {
            is_integer_constant_expression(left) && is_integer_constant_expression(right)
        }
        ExprKind::Conditional {
            condition,
            then_value,
            else_value,
        } => {
            is_integer_constant_expression(condition)
                && then_value
                    .as_ref()
                    .is_none_or(is_integer_constant_expression)
                && is_integer_constant_expression(else_value)
        }
        ExprKind::Identifier(_)
        | ExprKind::StringLiteral(_)
        | ExprKind::Generic { .. }
        | ExprKind::FloatLiteral(_)
        | ExprKind::Call { .. }
        | ExprKind::Assign { .. }
        | ExprKind::Comma { .. }
        | ExprKind::Member { .. }
        | ExprKind::Index { .. }
        | ExprKind::OffsetOf { .. }
        | ExprKind::TypesCompatible { .. }
        | ExprKind::Postfix { .. }
        | ExprKind::CompoundLiteral { .. }
        | ExprKind::BitCast { .. }
        | ExprKind::VaArg { .. }
        | ExprKind::LabelAddress(_)
        | ExprKind::StatementExpression(_)
        | ExprKind::NullPtrLiteral => false,
    }
}

fn check_tag_definition(
    tag: &Span<TagDefinition>,
    context: TypeContext<'_>,
    loc: Loc,
    errors: &mut Vec<SemaError>,
) {
    let provenance = tag.provenance;
    check_attributes(&tag.attributes, provenance, loc, errors);
    let fields = match &tag.body {
        TagBody::Record(fields) => fields,
        TagBody::Enum {
            fixed_type,
            enumerators,
        } => {
            if let Some(fixed_type) = fixed_type {
                check_type_name(fixed_type, context, provenance, loc, errors);
            }
            for item in enumerators {
                if let EnumItemKind::Enumerator(enumerator) = &item.value {
                    check_attributes(
                        &enumerator.attributes,
                        item.provenance,
                        item.expansion,
                        errors,
                    );
                }
            }
            return;
        }
    };
    for field_item in fields {
        let FieldItemKind::Field(field) = &field_item.value else {
            continue;
        };
        check_type(
            &field.specifiers.ty,
            context,
            field_item.provenance,
            field_item.expansion,
            errors,
        );
        let attributes = field.specifiers.attributes.iter().chain(
            field
                .declarators
                .iter()
                .flat_map(|declarator| &declarator.attributes),
        );
        check_attributes(
            &attributes.cloned().collect::<Vec<_>>(),
            field_item.provenance,
            field_item.expansion,
            errors,
        );
    }
}

fn collect_tag_names(ty: &TypeSpecifier, tags: &mut HashSet<String>) {
    match ty {
        TypeSpecifier::Tag(TagSpecifier::Reference { name, .. }) => {
            tags.insert(name.clone());
        }
        TypeSpecifier::Complex(ty) | TypeSpecifier::Imaginary(ty) => collect_tag_names(ty, tags),
        TypeSpecifier::Atomic(ty) => collect_tag_names(&ty.specifiers.ty, tags),
        TypeSpecifier::Vector(vector) => collect_tag_names(&vector.element, tags),
        TypeSpecifier::TypeOf(TypeOfOperand::Type(ty))
        | TypeSpecifier::TypeOfUnqual(TypeOfOperand::Type(ty)) => {
            collect_tag_names(&ty.specifiers.ty, tags)
        }
        TypeSpecifier::Void
        | TypeSpecifier::Bool
        | TypeSpecifier::Integer(_)
        | TypeSpecifier::Floating(_)
        | TypeSpecifier::FixedPoint(_)
        | TypeSpecifier::TypeOf(TypeOfOperand::Expression(_))
        | TypeSpecifier::TypeOfUnqual(TypeOfOperand::Expression(_))
        | TypeSpecifier::TargetBuiltin(_)
        | TypeSpecifier::Named(_)
        | TypeSpecifier::Tag(TagSpecifier::Definition(_)) => {}
    }
}

fn check_type(
    ty: &TypeSpecifier,
    context: TypeContext<'_>,
    provenance: Provenance,
    loc: Loc,
    errors: &mut Vec<SemaError>,
) {
    check_extensions(ty, context, provenance, loc, errors);
    match ty {
        TypeSpecifier::Named(name) if !context.typedefs.contains(name) => errors.push(error(
            provenance,
            loc,
            format!("unknown type name `{name}`"),
        )),
        TypeSpecifier::Tag(TagSpecifier::Reference {
            name, fixed_type, ..
        }) => {
            if !context.tags.contains(name) {
                errors.push(error(provenance, loc, format!("unknown tag `{name}`")));
            }
            if let Some(fixed_type) = fixed_type {
                check_type_name(fixed_type, context, provenance, loc, errors);
            }
        }
        TypeSpecifier::Atomic(ty) => check_type_name(ty, context, provenance, loc, errors),
        TypeSpecifier::Vector(vector) => {
            check_type(&vector.element, context, provenance, loc, errors)
        }
        TypeSpecifier::TypeOf(TypeOfOperand::Type(ty))
        | TypeSpecifier::TypeOfUnqual(TypeOfOperand::Type(ty)) => {
            check_type_name(ty, context, provenance, loc, errors)
        }
        TypeSpecifier::Imaginary(ty) => check_type(ty, context, provenance, loc, errors),
        TypeSpecifier::Void
        | TypeSpecifier::Bool
        | TypeSpecifier::Integer(_)
        | TypeSpecifier::Floating(_)
        | TypeSpecifier::Complex(_)
        | TypeSpecifier::FixedPoint(_)
        | TypeSpecifier::TypeOf(TypeOfOperand::Expression(_))
        | TypeSpecifier::TypeOfUnqual(TypeOfOperand::Expression(_))
        | TypeSpecifier::TargetBuiltin(_)
        | TypeSpecifier::Named(_)
        | TypeSpecifier::Tag(_) => {}
    }
}

fn check_type_name(
    type_name: &TypeName,
    context: TypeContext<'_>,
    provenance: Provenance,
    loc: Loc,
    errors: &mut Vec<SemaError>,
) {
    check_type(&type_name.specifiers.ty, context, provenance, loc, errors);
    check_declarator(&type_name.declarator, context, provenance, loc, errors);
}

impl Warning {
    fn diagnose(
        self,
        message: impl Into<String>,
        diagnostics: &DiagnosticOptions,
        standard: LanguageStandard,
        provenance: Provenance,
        loc: Loc,
    ) -> Option<SemaError> {
        let severity = diagnostics.severity(self, standard)?;
        let mut diagnostic = error(provenance, loc, message);
        diagnostic.severity = severity;
        diagnostic.warning = Some(self);
        Some(diagnostic)
    }
}

pub(super) fn error(provenance: Provenance, loc: Loc, message: impl Into<String>) -> SemaError {
    SemaError {
        message: message.into(),
        severity: Severity::Error,
        warning: None,
        provenance: Some(provenance),
        loc: Some(loc),
        source_code: NamedSource::new("<unknown>", String::new()),
        span: SourceSpan::new(0.into(), 0),
    }
}

enum BodyNode<'a> {
    Stmt(&'a Stmt),
    Expr(&'a Expr),
    EnterJumpScope,
    ExitJumpScope,
}

fn walk_stmts<'a>(stmts: &'a [Stmt], visit: &mut impl FnMut(BodyNode<'a>)) {
    for stmt in stmts {
        walk_stmt(stmt, visit);
    }
}

fn walk_stmt<'a>(stmt: &'a Stmt, visit: &mut impl FnMut(BodyNode<'a>)) {
    visit(BodyNode::Stmt(stmt));
    match &stmt.value {
        StmtKind::Return(expr) | StmtKind::Expr(expr) | StmtKind::ComputedGoto(expr) => {
            walk_expr(expr, visit)
        }
        StmtKind::Labeled { body, .. } | StmtKind::Attributed { body, .. } => {
            walk_stmt(body, visit)
        }
        StmtKind::SwitchLabel { label, body } => {
            match label {
                SwitchLabel::Case(expr) => walk_expr(expr, visit),
                SwitchLabel::CaseRange { start, end } => {
                    walk_expr(start, visit);
                    walk_expr(end, visit);
                }
                SwitchLabel::Default => {}
            }
            walk_stmt(body, visit);
        }
        StmtKind::Decl(declaration) => {
            for initializer in declaration
                .declarators
                .iter()
                .filter_map(|declarator| declarator.initializer.as_ref())
            {
                walk_initializer(initializer, visit);
            }
        }
        StmtKind::Block(body) => walk_stmts(body, visit),
        StmtKind::DoWhile { body, condition } => {
            walk_stmt(body, visit);
            walk_expr(condition, visit);
        }
        StmtKind::While { condition, body }
        | StmtKind::Switch {
            discriminant: condition,
            body,
        } => {
            walk_expr(condition, visit);
            walk_stmt(body, visit);
        }
        StmtKind::If {
            condition,
            then_branch,
            else_branch,
        } => {
            walk_expr(condition, visit);
            walk_stmt(then_branch, visit);
            if let Some(else_branch) = else_branch {
                walk_stmt(else_branch, visit);
            }
        }
        StmtKind::For {
            init,
            condition,
            increment,
            body,
        } => {
            if let Some(init) = init {
                walk_stmt(init, visit);
            }
            for expr in condition.iter().chain(increment) {
                walk_expr(expr, visit);
            }
            walk_stmt(body, visit);
        }
        StmtKind::Null
        | StmtKind::NestedFunction(_)
        | StmtKind::Comment(_)
        | StmtKind::ReturnVoid
        | StmtKind::StaticAssert(_)
        | StmtKind::Attribute(_)
        | StmtKind::LocalLabelDecl(_)
        | StmtKind::Asm(_)
        | StmtKind::Goto(_)
        | StmtKind::Break
        | StmtKind::Continue
        | StmtKind::Pragma(_) => {}
    }
}

fn walk_initializer<'a>(initializer: &'a Initializer, visit: &mut impl FnMut(BodyNode<'a>)) {
    match initializer {
        Initializer::Expr(expr) => walk_expr(expr, visit),
        Initializer::List(items) => {
            for item in items {
                walk_initializer(&item.value, visit);
            }
        }
    }
}

fn walk_expr<'a>(expr: &'a Expr, visit: &mut impl FnMut(BodyNode<'a>)) {
    visit(BodyNode::Expr(expr));
    match &expr.value {
        ExprKind::StatementExpression(body) => {
            visit(BodyNode::EnterJumpScope);
            walk_stmts(body, visit);
            visit(BodyNode::ExitJumpScope);
        }
        ExprKind::Generic {
            controlling,
            associations,
        } => {
            if let GenericControl::Expr(controlling) = controlling {
                walk_expr(controlling, visit);
            }
            for association in associations {
                let (GenericAssociation::Type { value, .. } | GenericAssociation::Default(value)) =
                    association;
                walk_expr(value, visit);
            }
        }
        ExprKind::Paren(value)
        | ExprKind::SizeOfExpr(value)
        | ExprKind::AlignOfExpr(value)
        | ExprKind::Unary { operand: value, .. }
        | ExprKind::Postfix { operand: value, .. }
        | ExprKind::Cast { value, .. }
        | ExprKind::BitCast { value, .. }
        | ExprKind::Member { base: value, .. }
        | ExprKind::VaArg { list: value, .. } => walk_expr(value, visit),
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
            walk_expr(left, visit);
            walk_expr(right, visit);
        }
        ExprKind::Conditional {
            condition,
            then_value,
            else_value,
        } => {
            walk_expr(condition, visit);
            if let Some(then_value) = then_value {
                walk_expr(then_value, visit);
            }
            walk_expr(else_value, visit);
        }
        ExprKind::Call { callee, arguments } => {
            walk_expr(callee, visit);
            for argument in arguments {
                walk_expr(argument, visit);
            }
        }
        ExprKind::CompoundLiteral { initializer, .. } => {
            for item in initializer {
                walk_initializer(&item.value, visit);
            }
        }
        ExprKind::IntegerLiteral(_)
        | ExprKind::FloatLiteral(_)
        | ExprKind::CharLiteral(_)
        | ExprKind::Identifier(_)
        | ExprKind::StringLiteral(_)
        | ExprKind::SizeOfType { .. }
        | ExprKind::AlignOf { .. }
        | ExprKind::OffsetOf { .. }
        | ExprKind::TypesCompatible { .. }
        | ExprKind::LabelAddress(_)
        | ExprKind::BoolLiteral(_)
        | ExprKind::NullPtrLiteral => {}
    }
}

pub(super) fn integer_rank_width(rank: IntegerRank, target: &TargetInfo) -> u32 {
    match rank {
        IntegerRank::Short => target.short_width,
        IntegerRank::Int => target.int_width,
        IntegerRank::Long => target.long_width,
        IntegerRank::LongLong => target.long_long_width,
        IntegerRank::Int128 => 128,
    }
}

pub(super) fn fits_rank(value: &BigUint, width: u32, signed: bool) -> bool {
    let limit = BigUint::from(1u32) << (width - u32::from(signed));
    *value < limit
}

pub(super) fn integer_candidates(
    literal: &IntegerLiteral,
    features: StandardFeatures,
) -> Vec<(IntegerRank, bool)> {
    use IntegerRank::{Int, Long, LongLong};
    let size = literal.suffix.size;
    if size == IntegerSizeSuffix::BitInt {
        return Vec::new();
    }
    let unsigned_only = literal.suffix.unsigned;
    let signed_only = !unsigned_only && literal.radix == Radix::Decimal;
    let push = |candidates: &mut Vec<(IntegerRank, bool)>, rank| {
        if !unsigned_only {
            candidates.push((rank, true));
        }
        if !signed_only {
            candidates.push((rank, false));
        }
    };
    let long_long = features.long_long_type.is_accepted() || size == IntegerSizeSuffix::LongLong;
    let mut candidates = Vec::new();
    if size == IntegerSizeSuffix::None {
        push(&mut candidates, Int);
    }
    if size != IntegerSizeSuffix::LongLong {
        push(&mut candidates, Long);
        if signed_only && long_long && features.long_long_type != Availability::Standard {
            candidates.push((Long, false));
        }
    }
    if long_long {
        push(&mut candidates, LongLong);
    }
    if signed_only {
        candidates.push((if long_long { LongLong } else { Long }, false));
    }
    candidates
}

pub(super) fn select_integer_candidate(
    literal: &IntegerLiteral,
    target: &TargetInfo,
    features: StandardFeatures,
) -> Option<(IntegerRank, bool)> {
    integer_candidates(literal, features)
        .into_iter()
        .find(|(rank, signed)| {
            let width = integer_rank_width(*rank, target);
            width > 0 && fits_rank(&literal.value, width, *signed)
        })
}

fn integer_literal_warnings(
    literal: &IntegerLiteral,
    rank: IntegerRank,
    signed: bool,
    features: StandardFeatures,
) -> Vec<(Warning, String)> {
    let mut warnings = Vec::new();
    if rank == IntegerRank::LongLong && features.long_long_type != Availability::Standard {
        warnings.push((
            Warning::LongLong,
            "'long long' is an extension when C99 mode is not enabled".to_string(),
        ));
    }
    if !literal.suffix.unsigned && literal.radix == Radix::Decimal && !signed {
        warnings.push(if rank == IntegerRank::Long {
            (
                Warning::C99Compat,
                "integer literal is too large to be represented in type 'long', interpreting as \
                 'unsigned long' per C89; this literal will have type 'long long' in C99 onwards"
                    .to_string(),
            )
        } else {
            (
                Warning::ImplicitlyUnsignedLiteral,
                "integer literal is too large to be represented in a signed integer type, \
                 interpreting as unsigned"
                    .to_string(),
            )
        });
    }
    warnings
}

fn integer_literal_diagnostics(
    literal: &IntegerLiteral,
    context: LiteralContext<'_>,
) -> Vec<(Option<Warning>, String)> {
    if literal.suffix.size == IntegerSizeSuffix::BitInt {
        return Vec::new();
    }
    match select_integer_candidate(literal, context.target, context.features) {
        None => vec![(
            None,
            format!(
                "integer literal `{}` is too large to be represented in any integer type",
                literal.spelling
            ),
        )],
        Some((rank, signed)) => integer_literal_warnings(literal, rank, signed, context.features)
            .into_iter()
            .map(|(warning, message)| (Some(warning), message))
            .collect(),
    }
}

fn char_literal_max(encoding: Encoding, target: &TargetInfo) -> u32 {
    match encoding {
        Encoding::Plain | Encoding::Utf8 => 0xFF,
        Encoding::Utf16 => 0xFFFF,
        Encoding::Utf32 => u32::MAX,
        Encoding::Wide => {
            if target.wchar_width >= 32 {
                u32::MAX
            } else {
                0xFFFF
            }
        }
    }
}

fn resolve_char_literal(
    literal: &CharLiteral,
    target: &TargetInfo,
    flavor: CompilerFlavor,
) -> Result<(), String> {
    if literal.encoding == Encoding::Plain {
        if flavor == CompilerFlavor::Clang
            && literal.execution_units(target.wchar_width).len() > literal.code_units.len()
        {
            return Err("character too large for enclosing character literal type".to_string());
        }
        return Ok(());
    }
    if literal.code_units.len() > 1 {
        return Err(match literal.encoding {
            Encoding::Wide => "wide character literals may not contain multiple characters",
            _ => "Unicode character literals may not contain multiple characters",
        }
        .to_string());
    }
    if let [unit] = literal.code_units.as_slice()
        && *unit > char_literal_max(literal.encoding, target)
    {
        return Err("character too large for enclosing character literal type".to_string());
    }
    Ok(())
}

#[derive(Clone, Copy)]
struct LiteralContext<'a> {
    target: &'a TargetInfo,
    features: StandardFeatures,
    diagnostics: &'a DiagnosticOptions,
    standard: LanguageStandard,
    flavor: CompilerFlavor,
}

fn check_literal_expr(expr: &Expr, context: LiteralContext<'_>) -> Vec<(Option<Warning>, String)> {
    match &expr.value {
        ExprKind::IntegerLiteral(literal) => integer_literal_diagnostics(literal, context),
        ExprKind::CharLiteral(literal) => {
            resolve_char_literal(literal, context.target, context.flavor)
                .err()
                .map(|message| (None, message))
                .into_iter()
                .collect()
        }
        ExprKind::FloatLiteral(literal) => {
            super::numeric::resolve_float_literal(literal, context.target)
                .err()
                .map(|error| (None, error.to_string()))
                .into_iter()
                .collect()
        }
        _ => Vec::new(),
    }
}

fn push_literal_diagnostics(
    expr: &Expr,
    context: LiteralContext<'_>,
    provenance: Provenance,
    errors: &mut Vec<SemaError>,
) {
    for (warning, message) in check_literal_expr(expr, context) {
        match warning {
            None => errors.push(error(provenance, expr.expansion, message)),
            Some(warning) => errors.extend(warning.diagnose(
                message,
                context.diagnostics,
                context.standard,
                provenance,
                expr.expansion,
            )),
        }
    }
}

fn check_literals(
    function: &FunctionDefinition,
    context: LiteralContext<'_>,
    provenance: Provenance,
    errors: &mut Vec<SemaError>,
) {
    walk_stmts(&function.body, &mut |node| {
        let BodyNode::Expr(expr) = node else {
            return;
        };
        push_literal_diagnostics(expr, context, provenance, errors);
    });
}

fn check_body_types(
    function: &FunctionDefinition,
    context: TypeContext<'_>,
    provenance: Provenance,
    errors: &mut Vec<SemaError>,
) {
    walk_stmts(&function.body, &mut |node| {
        let BodyNode::Stmt(stmt) = node else {
            return;
        };
        let StmtKind::Decl(declaration) = &stmt.value else {
            return;
        };
        walk_type(&declaration.specifiers.ty, &mut |ty| {
            check_extensions(ty, context, provenance, stmt.expansion, errors)
        });
    });
}

fn check_declaration_literals(
    declaration: &Declaration,
    context: LiteralContext<'_>,
    provenance: Provenance,
    errors: &mut Vec<SemaError>,
) {
    for init_declarator in &declaration.declarators {
        let Some(initializer) = &init_declarator.initializer else {
            continue;
        };
        walk_initializer(initializer, &mut |node| {
            let BodyNode::Expr(expr) = node else {
                return;
            };
            push_literal_diagnostics(expr, context, provenance, errors);
        });
    }
}

fn check_function_asm(
    unit: &TranslationUnit,
    function: &FunctionDefinition,
    provenance: Provenance,
    errors: &mut Vec<SemaError>,
) {
    let mut labels = HashMap::new();
    let mut scope = Vec::new();
    let mut next_scope = 0;
    walk_stmts(&function.body, &mut |node| match node {
        BodyNode::Stmt(stmt) => {
            if let StmtKind::Labeled { label: name, .. } = &stmt.value {
                labels.insert(name.as_str(), scope.clone());
            }
        }
        BodyNode::Expr(_) => {}
        BodyNode::EnterJumpScope => {
            scope.push(next_scope);
            next_scope += 1;
        }
        BodyNode::ExitJumpScope => {
            scope.pop();
        }
    });
    scope.clear();
    next_scope = 0;
    walk_stmts(&function.body, &mut |node| match node {
        BodyNode::Stmt(stmt) => match &stmt.value {
            StmtKind::Asm(asm) => {
                check_asm_operands(asm, &labels, &scope, provenance, stmt.expansion, errors)
            }
            StmtKind::Decl(declaration) => {
                for declarator in &declaration.declarators {
                    check_register_variable(
                        unit,
                        &declaration.specifiers,
                        declarator,
                        false,
                        provenance,
                        stmt.expansion,
                        errors,
                    );
                }
            }
            _ => {}
        },
        BodyNode::Expr(_) => {}
        BodyNode::EnterJumpScope => {
            scope.push(next_scope);
            next_scope += 1;
        }
        BodyNode::ExitJumpScope => {
            scope.pop();
        }
    });
}

fn check_asm_operands(
    asm: &GnuAsm,
    labels: &HashMap<&str, Vec<usize>>,
    scope: &[usize],
    provenance: Provenance,
    asm_loc: Loc,
    errors: &mut Vec<SemaError>,
) {
    let Some(operands) = &asm.operands else {
        return;
    };
    if let Some((loc, message)) = asm_operand_error(operands) {
        errors.push(error(provenance, loc, message));
    }
    let mut operand_names = HashSet::new();
    for label in &operands.labels {
        if !operand_names.insert(label.value.as_str()) {
            errors.push(error(
                provenance,
                label.expansion,
                format!("duplicate use of asm operand name \"{}\"", label.value),
            ));
        }
    }
    let mut invalid_jump_scope = false;
    for label in &operands.labels {
        match labels.get(label.value.as_str()) {
            None => errors.push(error(
                provenance,
                label.expansion,
                format!("use of undeclared label '{}'", label.value),
            )),
            Some(label_scope) if !scope.starts_with(label_scope) => invalid_jump_scope = true,
            Some(_) => {}
        }
    }
    if invalid_jump_scope {
        errors.push(error(
            provenance,
            asm_loc,
            "cannot jump from this asm goto statement to one of its possible targets",
        ));
    }
}

fn asm_operand_error(operands: &AsmOperands) -> Option<(Loc, String)> {
    for output in &operands.outputs {
        match output_lvalue(&output.expr) {
            OutputLvalue::Valid => {}
            OutputLvalue::Cast => {
                return Some((
                    output.expr.expansion,
                    "invalid use of a cast in an inline asm context requiring an lvalue".into(),
                ));
            }
            OutputLvalue::Invalid => {
                return Some((output.expr.expansion, "invalid lvalue in asm output".into()));
            }
        }
    }
    let mut expected = None;
    for operand in operands.outputs.iter().chain(&operands.inputs) {
        let count = operand.constraint.alternatives.len();
        match expected {
            None => expected = Some(count),
            Some(expected) if expected != count => {
                return Some((
                    operand.expr.expansion,
                    format!(
                        "asm constraint has an unexpected number of alternatives: {expected} vs {count}"
                    ),
                ));
            }
            Some(_) => {}
        }
    }
    None
}

enum OutputLvalue {
    Valid,
    Cast,
    Invalid,
}

fn output_lvalue(expr: &Expr) -> OutputLvalue {
    match &expr.value {
        ExprKind::Identifier(_)
        | ExprKind::StringLiteral(_)
        | ExprKind::Unary {
            op: UnaryOp::Deref, ..
        }
        | ExprKind::Index { .. }
        | ExprKind::Member { arrow: true, .. }
        | ExprKind::CompoundLiteral { .. }
        | ExprKind::Generic { .. } => OutputLvalue::Valid,
        ExprKind::Paren(inner) => output_lvalue(inner),
        ExprKind::Member { base, .. } => match output_lvalue(base) {
            OutputLvalue::Valid => OutputLvalue::Valid,
            OutputLvalue::Cast | OutputLvalue::Invalid => OutputLvalue::Invalid,
        },
        ExprKind::Cast { value, .. } | ExprKind::BitCast { value, .. } => {
            match output_lvalue(value) {
                OutputLvalue::Invalid => OutputLvalue::Invalid,
                OutputLvalue::Valid | OutputLvalue::Cast => OutputLvalue::Cast,
            }
        }
        _ => OutputLvalue::Invalid,
    }
}

fn check_register_variable(
    unit: &TranslationUnit,
    specifiers: &DeclarationSpecifiers,
    declarator: &InitDeclarator,
    file_scope: bool,
    provenance: Provenance,
    loc: Loc,
    errors: &mut Vec<SemaError>,
) {
    if !matches!(specifiers.storage, StorageClass::Register) {
        return;
    }
    let Some(label) = &declarator.asm_label else {
        if file_scope {
            errors.push(error(
                provenance,
                loc,
                "illegal storage class on file-scoped variable",
            ));
        }
        return;
    };
    let AsmLabel::Register(register) = &label.value else {
        return;
    };
    match register {
        Register::Other(name) => {
            if name.is_empty() {
                errors.push(error(
                    provenance,
                    label.expansion,
                    "cannot use an empty string literal in 'asm'",
                ));
            }
            errors.push(error(
                provenance,
                label.expansion,
                format!("unknown register name '{name}' in asm"),
            ));
        }
        Register::X86(x86) if file_scope && !matches!(x86.spelling.as_str(), "rsp" | "rbp") => {
            errors.push(error(
                provenance,
                label.expansion,
                format!(
                    "register '{}' unsuitable for global register variables on this target",
                    x86.spelling
                ),
            ));
        }
        Register::X86(_) | Register::Aarch64(_) => {}
    }
    if file_scope && !is_register_variable_type(unit, specifiers, &declarator.declarator) {
        errors.push(error(
            provenance,
            loc,
            "unsupported type for named register variable",
        ));
    }
}

fn is_register_variable_type(
    unit: &TranslationUnit,
    specifiers: &DeclarationSpecifiers,
    declarator: &Declarator,
) -> bool {
    let mut declarator = declarator;
    while let Declarator::Grouped(inner) | Declarator::Attributed { inner, .. } = declarator {
        declarator = inner;
    }
    match declarator {
        Declarator::Pointer { .. } => true,
        Declarator::Array { .. } | Declarator::Function { .. } => false,
        Declarator::Name(_) | Declarator::Abstract => is_register_scalar_type(&specifiers.ty, unit),
        Declarator::Grouped(_) | Declarator::Attributed { .. } => unreachable!(),
    }
}

fn is_register_scalar_type(ty: &TypeSpecifier, unit: &TranslationUnit) -> bool {
    match ty {
        TypeSpecifier::Atomic(ty) => {
            is_register_variable_type(unit, &ty.specifiers, &ty.declarator)
        }
        TypeSpecifier::Void
        | TypeSpecifier::Floating(_)
        | TypeSpecifier::Complex(_)
        | TypeSpecifier::Imaginary(_)
        | TypeSpecifier::FixedPoint(_)
        | TypeSpecifier::Vector(_)
        | TypeSpecifier::Tag(TagSpecifier::Reference {
            kind: TagKind::Struct | TagKind::Union,
            ..
        }) => false,
        TypeSpecifier::Tag(TagSpecifier::Definition(id)) => unit
            .tag(*id)
            .is_none_or(|tag| tag.value.kind == TagKind::Enum),
        _ => true,
    }
}
