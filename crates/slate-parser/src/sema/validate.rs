use crate::ast::*;
use crate::compiler_args::{CompilerFlavor, LanguageStandard};
use crate::const_expr::{
    BinaryOp, CharLiteral, Encoding, IntegerLiteral, IntegerSizeSuffix, Radix, UnaryOp,
};
use crate::diagnostics::{DiagnosticContext, DiagnosticOptions, Warning};
use crate::files::{Files, decode_source_bytes, display_path};
use crate::ir::BindingId;
use crate::standard_features::{Availability, StandardFeatures};
use crate::target_info::TargetInfo;
use crate::visit::{self, Visitor};
use miette::{Diagnostic, LabeledSpan, NamedSource, Severity, SourceCode, SourceSpan};
use num_bigint::BigUint;
use std::collections::{HashMap, HashSet};
use thiserror::Error;

use super::names::{ItemResolution, ResolveError as NameError};

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

impl super::Sema<'_> {
    pub fn analyze(&mut self, files: &Files) -> Result<Vec<SemaError>, SemaErrors> {
        analyze(self.unit, &mut self.types, &self.names, &self.items, files)
    }
}

fn analyze(
    unit: &TranslationUnit,
    resolver: &mut super::types::TypeResolver,
    names: &crate::ir::NameResolution,
    items: &[ItemResolution],
    files: &Files,
) -> Result<Vec<SemaError>, SemaErrors> {
    let dialect = &unit.dialect;
    let flavor = dialect.flavor();
    let features = dialect.features();
    let literals = LiteralContext {
        target: dialect.target(),
        features,
        diagnostics: &dialect.options().diagnostics,
        standard: dialect.standard(),
        flavor,
    };
    let types = TypeContext {
        features,
        diagnostics: &dialect.options().diagnostics,
        standard: dialect.standard(),
        flavor,
    };
    let mut errors = super::assertion::validate(unit, resolver, names, items);
    for (decl, item) in unit.decls.iter().zip(items) {
        for unresolved in &item.errors {
            if let NameError::Unresolved {
                namespace: "typedef",
                name,
                loc,
            } = unresolved
            {
                errors.push(error(
                    decl.provenance,
                    *loc,
                    format!("unknown type name `{name}`"),
                ));
            }
        }
        match &decl.value {
            DeclKind::Comment(_) | DeclKind::Asm { .. } | DeclKind::Pragma(_) => {}
            DeclKind::Attribute(attributes) => check_attributes(attributes, &mut errors),
            DeclKind::StaticAssert { .. } => {
                visit_literals(decl, literals, &mut errors);
            }
            DeclKind::Function(function) => {
                let provenance = decl.provenance;
                check_attributes(&function.specifiers.attributes, &mut errors);
                check_attributes(&function.attributes, &mut errors);
                check_type(
                    &function.specifiers.ty,
                    types,
                    provenance,
                    decl.expansion,
                    &mut errors,
                );
                check_declarator(
                    &function.declarator,
                    types,
                    provenance,
                    decl.expansion,
                    &mut errors,
                );
                if matches!(flavor, CompilerFlavor::Clang | CompilerFlavor::Gcc) {
                    check_function_asm(
                        unit,
                        function,
                        &names.label_definitions,
                        &resolver.references,
                        flavor,
                        provenance,
                        &mut errors,
                    );
                }
                check_unnamed_parameters(function, types, &mut errors);
                visit_literals(function, literals, &mut errors);
                check_body_types(function, types, provenance, &mut errors);
            }
            DeclKind::Declaration(declaration) => {
                let provenance = decl.provenance;
                let specifiers = &declaration.specifiers;
                if let TypeSpecifier::Tag(TagSpecifier::Definition(id)) = &specifiers.ty
                    && let Some(tag) = unit.tag(*id)
                {
                    check_tag_definition(tag, types, decl.expansion, &mut errors);
                    visit_literals(tag, literals, &mut errors);
                }
                check_type(
                    &specifiers.ty,
                    types,
                    provenance,
                    decl.expansion,
                    &mut errors,
                );
                check_attributes(&specifiers.attributes, &mut errors);
                visit_literals(declaration, literals, &mut errors);
                for init_declarator in &declaration.declarators {
                    let declarator = &init_declarator.declarator;
                    if matches!(specifiers.ty, TypeSpecifier::Void)
                        && !matches!(
                            specifiers.storage,
                            StorageClass::Extern | StorageClass::Typedef
                        )
                        && declares_void_array(declarator)
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
                    check_attributes(&init_declarator.attributes, &mut errors);
                    if flavor.is_clang() {
                        check_register_variable(
                            unit,
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
    with_sources(errors, files)
}

pub fn with_sources(
    diagnostics: Vec<SemaError>,
    files: &Files,
) -> Result<Vec<SemaError>, SemaErrors> {
    let mut error_count = 0;
    let mut located = Vec::new();
    for diagnostic in diagnostics {
        if diagnostic.severity == Severity::Error {
            if error_count == ERROR_LIMIT {
                located.push(SemaError::unlocated(
                    "too many errors emitted, stopping now",
                ));
                break;
            }
            error_count += 1;
        }
        located.push(diagnostic.with_source(files));
    }
    if error_count > 0 {
        Err(SemaErrors { errors: located })
    } else {
        Ok(located)
    }
}

impl SemaError {
    pub(super) fn unlocated(message: impl Into<String>) -> Self {
        Self {
            message: message.into(),
            severity: Severity::Error,
            warning: None,
            provenance: None,
            loc: None,
            source_code: NamedSource::new("<unknown>", String::new()),
            span: SourceSpan::new(0.into(), 0),
        }
    }

    pub(super) fn resolve_error(error: &super::numeric::ResolveError, files: &Files) -> Self {
        let mut resolved = Self::unlocated(error.to_string());
        resolved.loc = error.loc();
        resolved.with_source(files)
    }

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

// an array of void never resolves, so the checker cannot see it as a void object
fn declares_void_array(declarator: &Declarator) -> bool {
    match declarator {
        Declarator::Grouped(inner) | Declarator::Attributed { inner, .. } => {
            declares_void_array(inner)
        }
        Declarator::Array { inner, .. } => !declarator_indirects(inner),
        _ => false,
    }
}

fn declarator_indirects(declarator: &Declarator) -> bool {
    match declarator {
        Declarator::Grouped(inner)
        | Declarator::Attributed { inner, .. }
        | Declarator::Array { inner, .. } => declarator_indirects(inner),
        Declarator::Pointer { .. } | Declarator::Function { .. } => true,
        Declarator::Abstract | Declarator::Name(_) => false,
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
        TypeSpecifier::Mode(mode) => extension_warning(&mode.base, features),
        TypeSpecifier::Integer(IntegerType::BitInt { .. })
            if features.bit_int_type != Availability::Standard =>
        {
            Some((
                Warning::BitIntExtension,
                "'_BitInt' is an extension before C23",
            ))
        }
        _ => None,
    }
}

fn check_unnamed_parameters(
    function: &FunctionDefinition,
    context: TypeContext<'_>,
    errors: &mut Vec<SemaError>,
) {
    if context.features.unnamed_definition_parameters == Availability::Standard {
        return;
    }
    let parameters = function
        .declarator
        .function_parameters()
        .map_or(&[][..], ParameterList::parameters);
    for parameter in parameters {
        if parameter.declarator.name().is_some() {
            continue;
        }
        errors.extend(Warning::C23Extensions.diagnose(
            "omitting the parameter name in a function definition is a C23 extension",
            context.diagnostics(),
            parameter.provenance,
            parameter.expansion,
        ));
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
    errors.extend(warning.diagnose(message, context.diagnostics(), provenance, loc));
}

#[derive(Clone, Copy)]
struct TypeContext<'a> {
    features: StandardFeatures,
    diagnostics: &'a DiagnosticOptions,
    standard: LanguageStandard,
    flavor: CompilerFlavor,
}

impl<'a> TypeContext<'a> {
    fn diagnostics(&self) -> DiagnosticContext<'a> {
        DiagnosticContext {
            options: self.diagnostics,
            standard: self.standard,
            flavor: self.flavor,
        }
    }
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
                check_attributes(&parameter.attributes, errors);
                check_declarator(&parameter.declarator, context, provenance, loc, errors);
            }
        }
        Declarator::Attributed { inner, attributes } => {
            check_attributes(attributes, errors);
            check_declarator(inner, context, provenance, loc, errors);
        }
        Declarator::Pointer {
            inner, attributes, ..
        } => {
            check_attributes(attributes, errors);
            check_declarator(inner, context, provenance, loc, errors);
        }
        Declarator::Grouped(inner) | Declarator::Array { inner, .. } => {
            check_declarator(inner, context, provenance, loc, errors)
        }
        Declarator::Abstract | Declarator::Name(_) => {}
    }
}

fn check_attributes(attributes: &[Span<Attribute>], errors: &mut Vec<SemaError>) {
    for attribute in attributes {
        if let Attribute::Invalid { name, .. } = &attribute.value {
            errors.push(error(
                attribute.provenance,
                attribute.expansion,
                format!("invalid arguments for attribute `{name}`"),
            ));
        }
        if let Attribute::AllocSize(expressions) = &attribute.value
            && !(1..=2).contains(&expressions.len())
        {
            errors.push(error(
                attribute.provenance,
                attribute.expansion,
                "alloc_size expects one or two arguments",
            ));
        }
    }
}

fn check_tag_definition(
    tag: &Span<TagDefinition>,
    context: TypeContext<'_>,
    loc: Loc,
    errors: &mut Vec<SemaError>,
) {
    let provenance = tag.provenance;
    check_attributes(&tag.attributes, errors);
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
                    check_attributes(&enumerator.attributes, errors);
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
        check_attributes(&attributes.cloned().collect::<Vec<_>>(), errors);
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
        TypeSpecifier::Tag(TagSpecifier::Reference {
            fixed_type: Some(fixed_type),
            ..
        }) => check_type_name(fixed_type, context, provenance, loc, errors),
        TypeSpecifier::Atomic(ty) => check_type_name(ty, context, provenance, loc, errors),
        TypeSpecifier::Vector(vector) => {
            check_type(&vector.element, context, provenance, loc, errors)
        }
        TypeSpecifier::Mode(mode) => check_type(&mode.base, context, provenance, loc, errors),
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
        | TypeSpecifier::Inferred
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
    pub(super) fn diagnose(
        self,
        message: impl Into<String>,
        diagnostics: DiagnosticContext<'_>,
        provenance: Provenance,
        loc: Loc,
    ) -> Option<SemaError> {
        let severity = diagnostics.severity(self)?;
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

pub(super) const BIT_INT_MAX_WIDTH: u32 = 65535;

pub(super) const ERROR_LIMIT: usize = 20;

pub(super) fn bit_int_literal_width(literal: &IntegerLiteral) -> Option<u32> {
    let signed = !literal.suffix.unsigned;
    let width = literal.value.bits() + u64::from(signed);
    let width = u32::try_from(width.max(1 + u64::from(signed))).ok()?;
    (width <= BIT_INT_MAX_WIDTH).then_some(width)
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
    if signed_only && !features.widest_integer_literal_fallback {
        candidates.push((if long_long { LongLong } else { Long }, false));
    }
    candidates
}

pub(super) struct IntegerLiteralSelection {
    pub rank: IntegerRank,
    pub signed: bool,
    pub value: crate::ir::Number,
    pub truncated: bool,
    pub widest_fallback: bool,
}

// gcc evaluates literals in intmax_t precision and warns; clang rejects, so the gcc value wins.
pub(super) fn select_integer_candidate(
    literal: &IntegerLiteral,
    target: &TargetInfo,
    features: StandardFeatures,
) -> Option<IntegerLiteralSelection> {
    let intmax_width = target.long_long_width;
    let truncated = literal.value.bits() > u64::from(intmax_width);
    let value = if truncated {
        &literal.value % (BigUint::from(1u32) << intmax_width)
    } else {
        literal.value.clone()
    };
    let candidate = integer_candidates(literal, features)
        .into_iter()
        .find(|(rank, signed)| {
            let width = integer_rank_width(*rank, target);
            width > 0 && fits_rank(&value, width, *signed)
        });
    let (rank, signed, value, widest_fallback) = match candidate {
        Some((rank, signed)) => (rank, signed, crate::ir::Number::Integer(value), false),
        None if features.widest_integer_literal_fallback => {
            let rank = if target.pointer_width >= 64 {
                IntegerRank::Int128
            } else {
                IntegerRank::LongLong
            };
            let signed = !literal.suffix.unsigned;
            let width = integer_rank_width(rank, target);
            let value = if !signed || fits_rank(&value, width, true) {
                crate::ir::Number::Integer(value)
            } else {
                crate::ir::Number::SignedInteger(
                    num_bigint::BigInt::from(value) - (num_bigint::BigInt::from(1u32) << width),
                )
            };
            (rank, signed, value, true)
        }
        None => return None,
    };
    Some(IntegerLiteralSelection {
        rank,
        signed,
        value,
        truncated,
        widest_fallback,
    })
}

fn integer_literal_warnings(
    literal: &IntegerLiteral,
    selection: &IntegerLiteralSelection,
    features: StandardFeatures,
) -> Vec<(Warning, String)> {
    let IntegerLiteralSelection {
        rank,
        signed,
        truncated,
        widest_fallback,
        ..
    } = *selection;
    let mut warnings = Vec::new();
    if truncated {
        warnings.push((
            Warning::IntegerLiteralTooLarge,
            "integer constant is too large for its type".to_string(),
        ));
    } else if widest_fallback {
        warnings.push((
            Warning::ImplicitlyUnsignedLiteral,
            "integer constant is so large that it is unsigned".to_string(),
        ));
    }
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
        return match bit_int_literal_width(literal) {
            Some(_) => Vec::new(),
            None => vec![(
                None,
                format!(
                    "integer literal `{}` is too large to be represented in any _BitInt type",
                    literal.spelling
                ),
            )],
        };
    }
    match select_integer_candidate(literal, context.target, context.features) {
        None => vec![(
            None,
            format!(
                "integer literal `{}` is too large to be represented in any integer type",
                literal.spelling
            ),
        )],
        Some(selection) => integer_literal_warnings(literal, &selection, context.features)
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
        if flavor.is_clang()
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

impl<'a> LiteralContext<'a> {
    fn diagnostics(&self) -> DiagnosticContext<'a> {
        DiagnosticContext {
            options: self.diagnostics,
            standard: self.standard,
            flavor: self.flavor,
        }
    }
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
            if literal.fixed_suffix.is_some() {
                return Vec::new();
            }
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
                context.diagnostics(),
                provenance,
                expr.expansion,
            )),
        }
    }
}

struct LiteralVisitor<'a, 'b> {
    context: LiteralContext<'a>,
    errors: &'b mut Vec<SemaError>,
}

impl Visitor for LiteralVisitor<'_, '_> {
    type Error = std::convert::Infallible;

    fn visit_expr(&mut self, expr: &Expr) -> Result<(), Self::Error> {
        push_literal_diagnostics(expr, self.context, expr.provenance, self.errors);
        visit::walk_expr(self, expr)
    }
}

trait LiteralVisitable {
    fn visit_literals(&self, visitor: &mut LiteralVisitor<'_, '_>);
}

impl LiteralVisitable for Decl {
    fn visit_literals(&self, visitor: &mut LiteralVisitor<'_, '_>) {
        visitor
            .visit_decl(self)
            .unwrap_or_else(|never| match never {});
    }
}

impl LiteralVisitable for FunctionDefinition {
    fn visit_literals(&self, visitor: &mut LiteralVisitor<'_, '_>) {
        visitor
            .visit_function(self)
            .unwrap_or_else(|never| match never {});
    }
}

impl LiteralVisitable for Declaration {
    fn visit_literals(&self, visitor: &mut LiteralVisitor<'_, '_>) {
        visitor
            .visit_declaration(self)
            .unwrap_or_else(|never| match never {});
    }
}

impl LiteralVisitable for Span<TagDefinition> {
    fn visit_literals(&self, visitor: &mut LiteralVisitor<'_, '_>) {
        visitor
            .visit_tag_definition(self)
            .unwrap_or_else(|never| match never {});
    }
}

fn visit_literals<T: LiteralVisitable + ?Sized>(
    node: &T,
    context: LiteralContext<'_>,
    errors: &mut Vec<SemaError>,
) {
    node.visit_literals(&mut LiteralVisitor { context, errors });
}

fn check_body_types(
    function: &FunctionDefinition,
    context: TypeContext<'_>,
    provenance: Provenance,
    errors: &mut Vec<SemaError>,
) {
    let mut visitor = BodyTypeVisitor {
        context,
        provenance,
        errors,
        loc: None,
    };
    visit::walk_stmts(&mut visitor, &function.body).unwrap_or_else(|never| match never {});
}

struct BodyTypeVisitor<'a, 'b> {
    context: TypeContext<'a>,
    provenance: Provenance,
    errors: &'b mut Vec<SemaError>,
    loc: Option<Loc>,
}

impl Visitor for BodyTypeVisitor<'_, '_> {
    type Error = std::convert::Infallible;

    fn visit_stmt(&mut self, stmt: &Stmt) -> Result<(), Self::Error> {
        let outer = self.loc.replace(stmt.expansion);
        let result = visit::walk_stmt(self, stmt);
        self.loc = outer;
        result
    }

    fn visit_type_specifier(&mut self, ty: &TypeSpecifier) -> Result<(), Self::Error> {
        if let Some(loc) = self.loc {
            check_extensions(ty, self.context, self.provenance, loc, self.errors);
        }
        visit::walk_type_specifier(self, ty)
    }
}

fn check_function_asm(
    unit: &TranslationUnit,
    function: &FunctionDefinition,
    label_definitions: &HashMap<NodeId, BindingId>,
    references: &HashMap<NodeId, BindingId>,
    flavor: CompilerFlavor,
    provenance: Provenance,
    errors: &mut Vec<SemaError>,
) {
    let mut labels = AsmLabelScopes {
        definitions: HashMap::new(),
        references,
    };
    let mut collector = AsmLabelVisitor::new(label_definitions, &mut labels.definitions);
    visit::walk_stmts(&mut collector, &function.body).unwrap_or_else(|never| match never {});
    let mut checker = AsmCheckVisitor::new(unit, &labels, flavor, provenance, errors);
    visit::walk_stmts(&mut checker, &function.body).unwrap_or_else(|never| match never {});
}

struct AsmLabelScopes<'a> {
    definitions: HashMap<BindingId, Vec<usize>>,
    references: &'a HashMap<NodeId, BindingId>,
}

impl AsmLabelScopes<'_> {
    fn scope_of(&self, label: NodeId) -> Option<&[usize]> {
        let binding = self.references.get(&label)?;
        self.definitions.get(binding).map(Vec::as_slice)
    }
}

struct AsmLabelVisitor<'a> {
    label_definitions: &'a HashMap<NodeId, BindingId>,
    labels: &'a mut HashMap<BindingId, Vec<usize>>,
    scope: Vec<usize>,
    next_scope: usize,
}

impl<'a> AsmLabelVisitor<'a> {
    fn new(
        label_definitions: &'a HashMap<NodeId, BindingId>,
        labels: &'a mut HashMap<BindingId, Vec<usize>>,
    ) -> Self {
        Self {
            label_definitions,
            labels,
            scope: Vec::new(),
            next_scope: 0,
        }
    }
}

impl<'a> Visitor for AsmLabelVisitor<'a> {
    type Error = std::convert::Infallible;

    fn visit_stmt(&mut self, stmt: &Stmt) -> Result<(), Self::Error> {
        if let StmtKind::Labeled { label, .. } = &stmt.value
            && let Some(&binding) = self.label_definitions.get(&label.id)
        {
            self.labels.insert(binding, self.scope.clone());
        }
        visit::walk_stmt(self, stmt)
    }

    fn visit_expr(&mut self, expr: &Expr) -> Result<(), Self::Error> {
        if let ExprKind::StatementExpression(body) = &expr.value {
            self.scope.push(self.next_scope);
            self.next_scope += 1;
            visit::walk_stmts(self, body)?;
            self.scope.pop();
            Ok(())
        } else {
            visit::walk_expr(self, expr)
        }
    }
}

struct AsmCheckVisitor<'a, 'b> {
    unit: &'a TranslationUnit,
    labels: &'a AsmLabelScopes<'a>,
    flavor: CompilerFlavor,
    provenance: Provenance,
    errors: &'b mut Vec<SemaError>,
    scope: Vec<usize>,
    next_scope: usize,
}

impl<'a, 'b> AsmCheckVisitor<'a, 'b> {
    fn new(
        unit: &'a TranslationUnit,
        labels: &'a AsmLabelScopes<'a>,
        flavor: CompilerFlavor,
        provenance: Provenance,
        errors: &'b mut Vec<SemaError>,
    ) -> Self {
        Self {
            unit,
            labels,
            flavor,
            provenance,
            errors,
            scope: Vec::new(),
            next_scope: 0,
        }
    }
}

impl Visitor for AsmCheckVisitor<'_, '_> {
    type Error = std::convert::Infallible;

    fn visit_stmt(&mut self, stmt: &Stmt) -> Result<(), Self::Error> {
        match &stmt.value {
            StmtKind::Asm(asm) => check_asm_operands(
                asm,
                self.labels,
                &self.scope,
                self.flavor,
                self.provenance,
                stmt.expansion,
                self.errors,
            ),
            StmtKind::Decl(declaration) => {
                for declarator in &declaration.declarators {
                    check_register_variable(
                        self.unit,
                        &declaration.specifiers,
                        declarator,
                        false,
                        self.provenance,
                        stmt.expansion,
                        self.errors,
                    );
                }
            }
            _ => {}
        }
        visit::walk_stmt(self, stmt)
    }

    fn visit_expr(&mut self, expr: &Expr) -> Result<(), Self::Error> {
        if let ExprKind::StatementExpression(body) = &expr.value {
            self.scope.push(self.next_scope);
            self.next_scope += 1;
            visit::walk_stmts(self, body)?;
            self.scope.pop();
            Ok(())
        } else {
            visit::walk_expr(self, expr)
        }
    }
}

fn check_asm_operands(
    asm: &GnuAsm,
    labels: &AsmLabelScopes,
    scope: &[usize],
    flavor: CompilerFlavor,
    provenance: Provenance,
    asm_loc: Loc,
    errors: &mut Vec<SemaError>,
) {
    let Some(operands) = &asm.operands else {
        return;
    };
    if let Some((loc, message)) = asm_operand_error(operands, flavor) {
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
        match labels.scope_of(label.id) {
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

fn asm_operand_error(operands: &AsmOperands, flavor: CompilerFlavor) -> Option<(Loc, String)> {
    for output in &operands.outputs {
        if flavor.is_gcc() {
            if !is_gcc_output_lvalue(&output.expr) {
                return Some((
                    output.expr.expansion,
                    "lvalue required in 'asm' statement".into(),
                ));
            }
            continue;
        }
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
                let message = if flavor.is_gcc() {
                    "operand constraints for 'asm' differ in number of alternatives".into()
                } else {
                    format!(
                        "asm constraint has an unexpected number of alternatives: {expected} vs {count}"
                    )
                };
                return Some((operand.expr.expansion, message));
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

// gcc strips same-mode casts and folds before its lvalue check, which needs types this pass lacks.
fn is_gcc_output_lvalue(expr: &Expr) -> bool {
    match &expr.value {
        ExprKind::Identifier(_)
        | ExprKind::Unary {
            op: UnaryOp::Deref, ..
        }
        | ExprKind::Index { .. }
        | ExprKind::Member { arrow: true, .. }
        | ExprKind::CompoundLiteral { .. }
        | ExprKind::Generic { .. }
        | ExprKind::StatementExpression(_) => true,
        ExprKind::Paren(inner)
        | ExprKind::Member { base: inner, .. }
        | ExprKind::Cast { value: inner, .. }
        | ExprKind::BitCast { value: inner, .. }
        | ExprKind::Comma { right: inner, .. } => is_gcc_output_lvalue(inner),
        ExprKind::Conditional {
            condition,
            then_value,
            else_value,
        } => {
            is_gcc_output_lvalue(then_value.as_ref().unwrap_or(condition))
                && is_gcc_output_lvalue(else_value)
        }
        ExprKind::Binary { op, left, right } => match (
            identity_operand(*op, right, true),
            identity_operand(*op, left, false),
        ) {
            (true, _) => is_gcc_output_lvalue(left),
            (_, true) => is_gcc_output_lvalue(right),
            _ => false,
        },
        _ => false,
    }
}

fn identity_operand(op: BinaryOp, operand: &Expr, on_right: bool) -> bool {
    let ExprKind::IntegerLiteral(literal) = &operand.value else {
        return false;
    };
    let identity = match op {
        BinaryOp::Add | BinaryOp::BitOr | BinaryOp::BitXor => 0u32,
        BinaryOp::Sub | BinaryOp::ShiftLeft | BinaryOp::ShiftRight if on_right => 0,
        BinaryOp::Mul => 1,
        BinaryOp::Div if on_right => 1,
        _ => return false,
    };
    literal.value == BigUint::from(identity)
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
        Register::Other(_) => {}
        Register::X86(x86)
            if file_scope && !matches!(x86.spelling.as_str(), "rsp" | "rbp" | "esp" | "ebp") =>
        {
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
