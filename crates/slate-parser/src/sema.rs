use crate::ast::*;
use crate::const_expr::ConstExpr;
use crate::eval::Env;
use crate::files::{Files, display_path};
use miette::{Diagnostic, NamedSource, SourceSpan};
use std::collections::HashSet;
use thiserror::Error;

#[derive(Debug, Error, Diagnostic, Clone)]
#[error("{message}")]
pub struct SemaError {
    pub message: String,
    pub provenance: Option<Provenance>,
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
    pub fn analyze(&self, defines: &[String], files: &Files) -> Result<(), SemaErrors> {
        let mut env = Env::new();
        for define in defines {
            env = env.define(define.trim_start_matches("-D").split_once('=').map_or_else(
                || define.trim_start_matches("-D").to_string(),
                |(name, _)| name.to_string(),
            ));
        }
        let concrete = self.eval(&env);
        let typedefs = concrete
            .decls
            .iter()
            .filter_map(|decl| match decl {
                ConcreteDecl::Typedef { name, .. } => Some(name.clone()),
                _ => None,
            })
            .collect::<HashSet<_>>();
        let tags = concrete
            .decls
            .iter()
            .filter_map(|decl| match decl {
                ConcreteDecl::Record(record) => record.name.clone(),
                ConcreteDecl::Enum(enumeration) => enumeration.name.clone(),
                ConcreteDecl::Declaration { declaration, .. } => match &declaration.specifiers.ty {
                    CType::Tagged { name, .. } => name.clone(),
                    _ => None,
                },
                _ => None,
            })
            .collect::<HashSet<_>>();

        let mut errors = Vec::new();
        for decl in &concrete.decls {
            match decl {
                ConcreteDecl::Function(function) => {
                    check_attributes(&function.attributes, function.provenance, &mut errors);
                    check_type(
                        &function.ret_type,
                        &typedefs,
                        &tags,
                        function.provenance,
                        &mut errors,
                    );
                }
                ConcreteDecl::Declaration {
                    declaration,
                    provenance,
                } => {
                    if matches!(declaration.specifiers.ty, CType::Void)
                        && declaration.declarator.name().is_some()
                        && !matches!(declaration.declarator, Declarator::Function { .. })
                    {
                        errors.push(error(*provenance, "object cannot have type void"));
                    }
                    check_type(
                        &declaration.specifiers.ty,
                        &typedefs,
                        &tags,
                        *provenance,
                        &mut errors,
                    );
                    check_declarator(
                        &declaration.declarator,
                        &typedefs,
                        &tags,
                        *provenance,
                        &mut errors,
                    );
                    check_attributes(&declaration.attributes, *provenance, &mut errors);
                }
                ConcreteDecl::Typedef {
                    ty,
                    provenance,
                    attributes,
                    ..
                } => {
                    check_type(ty, &typedefs, &tags, *provenance, &mut errors);
                    check_attributes(attributes, *provenance, &mut errors);
                }
                ConcreteDecl::Record(record) => {
                    check_attributes(&record.attributes, record.provenance, &mut errors);
                    for field in &record.fields {
                        check_type(
                            &field.declaration.specifiers.ty,
                            &typedefs,
                            &tags,
                            field.provenance,
                            &mut errors,
                        );
                        check_attributes(
                            &field.declaration.attributes,
                            field.provenance,
                            &mut errors,
                        );
                    }
                }
                ConcreteDecl::Enum(_) => {}
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
        let Some(provenance) = self.provenance else {
            return self;
        };
        let path = files.path(provenance.file);
        let source = std::fs::read_to_string(path).unwrap_or_default();
        let offset: usize = source
            .lines()
            .take(provenance.line)
            .map(|line| line.len() + 1)
            .sum();
        self.source_code = NamedSource::new(display_path(path), source).with_language("C");
        self.span = SourceSpan::new(offset.into(), 1);
        self
    }
}

fn check_declarator(
    declarator: &Declarator,
    typedefs: &HashSet<String>,
    tags: &HashSet<String>,
    provenance: Provenance,
    errors: &mut Vec<SemaError>,
) {
    match declarator {
        Declarator::Function {
            parameters, inner, ..
        } => {
            check_declarator(inner, typedefs, tags, provenance, errors);
            for parameter in parameters {
                check_type(&parameter.ty, typedefs, tags, provenance, errors);
                check_attributes(&parameter.attributes, provenance, errors);
                if let Some(declarator) = &parameter.declarator {
                    check_declarator(declarator, typedefs, tags, provenance, errors);
                }
            }
        }
        Declarator::Grouped(inner)
        | Declarator::Pointer { inner, .. }
        | Declarator::Array { inner, .. } => {
            check_declarator(inner, typedefs, tags, provenance, errors)
        }
        Declarator::Abstract | Declarator::Name(_) => {}
    }
}

fn check_attributes(attributes: &[Attribute], provenance: Provenance, errors: &mut Vec<SemaError>) {
    for attribute in attributes {
        if let Attribute::Invalid { name, .. } = attribute {
            errors.push(error(
                provenance,
                format!("invalid arguments for attribute `{name}`"),
            ));
        }
        if let Attribute::Aligned(expression) | Attribute::VectorSize(expression) = attribute
            && !is_integer_constant_expression(expression)
        {
            errors.push(error(
                provenance,
                "layout attribute requires an integer constant expression",
            ));
        }
        if let Attribute::AllocSize(expressions) = attribute
            && !(1..=2).contains(&expressions.len())
        {
            errors.push(error(provenance, "alloc_size expects one or two arguments"));
        }
    }
}

fn is_integer_constant_expression(expression: &ConstExpr) -> bool {
    match expression {
        ConstExpr::Integer(_) | ConstExpr::SizeOf(_) => true,
        ConstExpr::Unary { value, .. } => is_integer_constant_expression(value),
        ConstExpr::Binary { left, right, .. } => {
            is_integer_constant_expression(left) && is_integer_constant_expression(right)
        }
        ConstExpr::Identifier(_) => false,
    }
}

fn check_type(
    ty: &CType,
    typedefs: &HashSet<String>,
    tags: &HashSet<String>,
    provenance: Provenance,
    errors: &mut Vec<SemaError>,
) {
    match ty {
        CType::Named(name) if !typedefs.contains(name) => {
            errors.push(error(provenance, format!("unknown type name `{name}`")))
        }
        CType::Tagged {
            name: Some(name), ..
        } if !tags.contains(name) => {
            errors.push(error(provenance, format!("unknown tag `{name}`")))
        }
        CType::Qualified { ty, .. } | CType::Pointer { pointee: ty, .. } => {
            check_type(ty, typedefs, tags, provenance, errors)
        }
        CType::Atomic(ty) => check_type(ty, typedefs, tags, provenance, errors),
        CType::Vector(vector) => check_type(&vector.element, typedefs, tags, provenance, errors),
        CType::TypeOf(TypeOfOperand::Type(ty)) => {
            check_type(ty, typedefs, tags, provenance, errors)
        }
        CType::Imaginary(ty) => check_type(ty, typedefs, tags, provenance, errors),
        CType::Array { element, .. } => check_type(element, typedefs, tags, provenance, errors),
        CType::Function {
            return_type,
            parameters,
            ..
        } => {
            check_type(return_type, typedefs, tags, provenance, errors);
            for parameter in parameters {
                check_type(&parameter.ty, typedefs, tags, provenance, errors);
            }
        }
        CType::Void
        | CType::Bool
        | CType::Integer(_)
        | CType::Floating(_)
        | CType::Complex(_)
        | CType::FixedPoint(_)
        | CType::TypeOf(TypeOfOperand::Expression(_))
        | CType::TargetBuiltin(_)
        | CType::Named(_)
        | CType::Tagged { .. } => {}
    }
}

fn error(provenance: Provenance, message: impl Into<String>) -> SemaError {
    SemaError {
        message: message.into(),
        provenance: Some(provenance),
        source_code: NamedSource::new("<unknown>", String::new()),
        span: SourceSpan::new(0.into(), 0),
    }
}
