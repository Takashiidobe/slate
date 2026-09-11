use crate::ast::*;
use crate::eval::Env;
use std::collections::HashSet;
use std::fmt;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct SemanticError {
    pub message: String,
    pub provenance: Option<Provenance>,
}

impl fmt::Display for SemanticError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.message)
    }
}

impl TranslationUnit {
    pub fn analyze(&self, defines: &[String]) -> Vec<SemanticError> {
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
                }
                ConcreteDecl::Typedef { ty, provenance, .. } => {
                    check_type(ty, &typedefs, &tags, *provenance, &mut errors);
                }
                ConcreteDecl::Record(record) => {
                    for field in &record.fields {
                        check_type(
                            &field.declaration.specifiers.ty,
                            &typedefs,
                            &tags,
                            field.provenance,
                            &mut errors,
                        );
                    }
                }
                ConcreteDecl::Enum(_) => {}
            }
        }
        errors
    }
}

fn check_declarator(
    declarator: &Declarator,
    typedefs: &HashSet<String>,
    tags: &HashSet<String>,
    provenance: Provenance,
    errors: &mut Vec<SemanticError>,
) {
    match declarator {
        Declarator::Function {
            parameters, inner, ..
        } => {
            check_declarator(inner, typedefs, tags, provenance, errors);
            for parameter in parameters {
                check_type(&parameter.ty, typedefs, tags, provenance, errors);
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

fn check_type(
    ty: &CType,
    typedefs: &HashSet<String>,
    tags: &HashSet<String>,
    provenance: Provenance,
    errors: &mut Vec<SemanticError>,
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
        | CType::Char
        | CType::Int
        | CType::Named(_)
        | CType::Tagged { .. } => {}
    }
}

fn error(provenance: Provenance, message: impl Into<String>) -> SemanticError {
    SemanticError {
        message: message.into(),
        provenance: Some(provenance),
    }
}
