use crate::ast::*;
use crate::const_expr::ConstExpr;
use crate::files::{Files, decode_source_bytes, display_path};
use miette::{Diagnostic, NamedSource, SourceSpan};
use std::collections::HashSet;
use thiserror::Error;

#[derive(Debug, Error, Diagnostic, Clone)]
#[error("{message}")]
pub struct SemaError {
    pub message: String,
    pub provenance: Option<Provenance>,
    pub loc: Option<Loc>,
    #[source_code]
    pub source_code: NamedSource<String>,
    #[label]
    pub span: SourceSpan,
}

#[derive(Debug, Error, Diagnostic)]
#[error("semantic analysis failed")]
pub struct SemaErrors {
    #[related]
    pub errors: Vec<SemaError>,
}

impl TranslationUnit {
    pub fn analyze(&self, files: &Files) -> Result<(), SemaErrors> {
        let typedefs = self
            .decls
            .iter()
            .filter_map(|decl| match &decl.value {
                Decl::Typedef { name, .. } => Some(name.clone()),
                _ => None,
            })
            .collect::<HashSet<_>>();
        let mut tags = HashSet::new();
        for decl in &self.decls {
            match &decl.value {
                Decl::Record(record) => {
                    if let Some(name) = &record.name {
                        tags.insert(name.clone());
                    }
                    for field_item in &record.fields {
                        if let FieldItem::Field(field) = &field_item.value {
                            collect_tag_names(&field.declaration.specifiers.ty, &mut tags);
                        }
                    }
                }
                Decl::Enum(enumeration) => {
                    if let Some(name) = &enumeration.name {
                        tags.insert(name.clone());
                    }
                }
                Decl::Function(function) => {
                    collect_tag_names(&function.ret_type, &mut tags);
                    for parameter in &function.parameters {
                        collect_tag_names(&parameter.ty, &mut tags);
                    }
                }
                Decl::Declaration { declaration, .. } => {
                    collect_tag_names(&declaration.specifiers.ty, &mut tags);
                }
                Decl::Typedef { ty, .. } => collect_tag_names(ty, &mut tags),
                Decl::Comment { .. } | Decl::StaticAssert { .. } => {}
            }
        }

        let mut errors = Vec::new();
        for decl in &self.decls {
            match &decl.value {
                Decl::Comment { .. } | Decl::StaticAssert { .. } => {}
                Decl::Function(function) => {
                    check_attributes(
                        &function.attributes,
                        function.provenance,
                        decl.expansion,
                        &mut errors,
                    );
                    check_type(
                        &function.ret_type,
                        &typedefs,
                        &tags,
                        function.provenance,
                        decl.expansion,
                        &mut errors,
                    );
                }
                Decl::Declaration {
                    declaration,
                    provenance,
                } => {
                    if matches!(declaration.specifiers.ty, CType::Void)
                        && declaration.declarator.name().is_some()
                        && !declarator_indirects_void(&declaration.declarator)
                    {
                        errors.push(error(
                            *provenance,
                            decl.expansion,
                            "object cannot have type void",
                        ));
                    }
                    check_type(
                        &declaration.specifiers.ty,
                        &typedefs,
                        &tags,
                        *provenance,
                        decl.expansion,
                        &mut errors,
                    );
                    check_declarator(
                        &declaration.declarator,
                        &typedefs,
                        &tags,
                        *provenance,
                        decl.expansion,
                        &mut errors,
                    );
                    check_attributes(
                        &declaration.attributes,
                        *provenance,
                        decl.expansion,
                        &mut errors,
                    );
                }
                Decl::Typedef {
                    ty,
                    provenance,
                    attributes,
                    ..
                } => {
                    check_type(
                        ty,
                        &typedefs,
                        &tags,
                        *provenance,
                        decl.expansion,
                        &mut errors,
                    );
                    check_attributes(attributes, *provenance, decl.expansion, &mut errors);
                }
                Decl::Record(record) => {
                    check_attributes(
                        &record.attributes,
                        record.provenance,
                        decl.expansion,
                        &mut errors,
                    );
                    for field_item in &record.fields {
                        let FieldItem::Field(field) = &field_item.value else {
                            continue;
                        };
                        check_type(
                            &field.declaration.specifiers.ty,
                            &typedefs,
                            &tags,
                            field.provenance,
                            field_item.expansion,
                            &mut errors,
                        );
                        check_attributes(
                            &field.declaration.attributes,
                            field.provenance,
                            field_item.expansion,
                            &mut errors,
                        );
                    }
                }
                Decl::Enum(_) => {}
            }
        }
        let errors: Vec<SemaError> = errors
            .into_iter()
            .map(|error| error.with_source(files))
            .collect();
        if errors.is_empty() {
            Ok(())
        } else {
            Err(SemaErrors { errors })
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
        Declarator::Grouped(inner) | Declarator::Array { inner, .. } => {
            declarator_indirects_void(inner)
        }
        Declarator::Pointer { .. } | Declarator::Function { .. } => true,
        Declarator::Abstract | Declarator::Name(_) => false,
    }
}

fn check_declarator(
    declarator: &Declarator,
    typedefs: &HashSet<String>,
    tags: &HashSet<String>,
    provenance: Provenance,
    loc: Loc,
    errors: &mut Vec<SemaError>,
) {
    match declarator {
        Declarator::Function {
            parameters, inner, ..
        } => {
            check_declarator(inner, typedefs, tags, provenance, loc, errors);
            for parameter in parameters {
                check_type(&parameter.ty, typedefs, tags, provenance, loc, errors);
                check_attributes(&parameter.attributes, provenance, loc, errors);
                if let Some(declarator) = &parameter.declarator {
                    check_declarator(declarator, typedefs, tags, provenance, loc, errors);
                }
            }
        }
        Declarator::Grouped(inner)
        | Declarator::Pointer { inner, .. }
        | Declarator::Array { inner, .. } => {
            check_declarator(inner, typedefs, tags, provenance, loc, errors)
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
        if let Attribute::Aligned(expression) | Attribute::VectorSize(expression) = attribute
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

fn is_integer_constant_expression(expression: &ConstExpr) -> bool {
    match expression {
        ConstExpr::Integer(_)
        | ConstExpr::WideInteger(_)
        | ConstExpr::SizeOf(_)
        | ConstExpr::SizeOfType { .. }
        | ConstExpr::AlignOf { .. } => true,
        ConstExpr::Unary { value, .. } | ConstExpr::Cast { value, .. } => {
            is_integer_constant_expression(value)
        }
        ConstExpr::Binary { left, right, .. } => {
            is_integer_constant_expression(left) && is_integer_constant_expression(right)
        }
        ConstExpr::Ternary {
            condition,
            then_value,
            else_value,
        } => {
            is_integer_constant_expression(condition)
                && is_integer_constant_expression(then_value)
                && is_integer_constant_expression(else_value)
        }
        ConstExpr::Identifier(_)
        | ConstExpr::StringLit(_)
        | ConstExpr::Utf8StringLit(_)
        | ConstExpr::Utf16StringLit(_)
        | ConstExpr::Utf32StringLit(_)
        | ConstExpr::WideStringLit(_)
        | ConstExpr::Generic { .. }
        | ConstExpr::Float(_)
        | ConstExpr::Call { .. }
        | ConstExpr::Assign { .. }
        | ConstExpr::Comma(..)
        | ConstExpr::Member { .. }
        | ConstExpr::Arrow { .. }
        | ConstExpr::Index { .. }
        | ConstExpr::OffsetOf { .. }
        | ConstExpr::TypesCompatible { .. }
        | ConstExpr::PostIncrement(_)
        | ConstExpr::PostDecrement(_)
        | ConstExpr::PreIncrement(_)
        | ConstExpr::PreDecrement(_)
        | ConstExpr::AddrOf(_)
        | ConstExpr::Deref(_)
        | ConstExpr::CompoundLiteral { .. }
        | ConstExpr::BitCast { .. }
        | ConstExpr::VaArg { .. }
        | ConstExpr::LabelAddr(_)
        | ConstExpr::StatementExpression(_) => false,
    }
}

fn collect_tag_names(ty: &CType, tags: &mut HashSet<String>) {
    match ty {
        CType::Tagged { name, body, .. } => {
            if let Some(name) = name {
                tags.insert(name.clone());
            }
            match body {
                Some(TagBody::Fields(fields)) => {
                    for field in fields {
                        collect_tag_names(&field.declaration.specifiers.ty, tags);
                    }
                }
                Some(TagBody::Enumerators(_)) | None => {}
            }
        }
        CType::Qualified { ty, .. } | CType::Pointer { pointee: ty, .. } => {
            collect_tag_names(ty, tags)
        }
        CType::Atomic(ty) | CType::Complex(ty) | CType::Imaginary(ty) => {
            collect_tag_names(ty, tags)
        }
        CType::Vector(vector) => collect_tag_names(&vector.element, tags),
        CType::TypeOf(TypeOfOperand::Type(ty)) | CType::TypeOfUnqual(TypeOfOperand::Type(ty)) => {
            collect_tag_names(ty, tags)
        }
        CType::Array { element, .. } => collect_tag_names(element, tags),
        CType::Function {
            return_type,
            parameters,
            ..
        } => {
            collect_tag_names(return_type, tags);
            for parameter in parameters {
                collect_tag_names(&parameter.ty, tags);
            }
        }
        CType::Void
        | CType::Bool
        | CType::Integer(_)
        | CType::Floating(_)
        | CType::FixedPoint(_)
        | CType::TypeOf(TypeOfOperand::Expression(_))
        | CType::TypeOfUnqual(TypeOfOperand::Expression(_))
        | CType::TargetBuiltin(_)
        | CType::Named(_) => {}
    }
}

fn check_type(
    ty: &CType,
    typedefs: &HashSet<String>,
    tags: &HashSet<String>,
    provenance: Provenance,
    loc: Loc,
    errors: &mut Vec<SemaError>,
) {
    match ty {
        CType::Named(name) if !typedefs.contains(name) => errors.push(error(
            provenance,
            loc,
            format!("unknown type name `{name}`"),
        )),
        CType::Tagged {
            name: Some(name), ..
        } if !tags.contains(name) => {
            errors.push(error(provenance, loc, format!("unknown tag `{name}`")))
        }
        CType::Qualified { ty, .. } | CType::Pointer { pointee: ty, .. } => {
            check_type(ty, typedefs, tags, provenance, loc, errors)
        }
        CType::Atomic(ty) => check_type(ty, typedefs, tags, provenance, loc, errors),
        CType::Vector(vector) => {
            check_type(&vector.element, typedefs, tags, provenance, loc, errors)
        }
        CType::TypeOf(TypeOfOperand::Type(ty)) | CType::TypeOfUnqual(TypeOfOperand::Type(ty)) => {
            check_type(ty, typedefs, tags, provenance, loc, errors)
        }
        CType::Imaginary(ty) => check_type(ty, typedefs, tags, provenance, loc, errors),
        CType::Array { element, .. } => {
            check_type(element, typedefs, tags, provenance, loc, errors)
        }
        CType::Function {
            return_type,
            parameters,
            ..
        } => {
            check_type(return_type, typedefs, tags, provenance, loc, errors);
            for parameter in parameters {
                check_type(&parameter.ty, typedefs, tags, provenance, loc, errors);
            }
        }
        CType::Void
        | CType::Bool
        | CType::Integer(_)
        | CType::Floating(_)
        | CType::Complex(_)
        | CType::FixedPoint(_)
        | CType::TypeOf(TypeOfOperand::Expression(_))
        | CType::TypeOfUnqual(TypeOfOperand::Expression(_))
        | CType::TargetBuiltin(_)
        | CType::Named(_)
        | CType::Tagged { .. } => {}
    }
}

fn error(provenance: Provenance, loc: Loc, message: impl Into<String>) -> SemaError {
    SemaError {
        message: message.into(),
        provenance: Some(provenance),
        loc: Some(loc),
        source_code: NamedSource::new("<unknown>", String::new()),
        span: SourceSpan::new(0.into(), 0),
    }
}
