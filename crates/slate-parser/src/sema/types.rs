use std::collections::HashMap;

use crate::ast::{
    ArraySize, DeclarationSpecifiers, Declarator, FloatingType, IntegerRank, IntegerType,
    ParameterList, Qualifiers, TypeSpecifier,
};
use crate::ir::{FloatType, NumericType, Type, TypeDefinition, TypeDefinitionKind, TypeId};
use crate::target_info::{LongDoubleFormat, TargetInfo};

use super::numeric::ResolveError;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct CTypeMetadata {
    pub spelling: String,
    pub canonical: String,
    pub typedef_chain: Vec<String>,
    pub qualifiers: Qualifiers,
}

impl CTypeMetadata {
    pub fn entries(&self) -> Vec<(String, String)> {
        let mut entries = vec![("c".into(), self.spelling.clone())];
        if self.canonical != self.spelling {
            entries.push(("c_canon".into(), self.canonical.clone()));
        }
        if !self.typedef_chain.is_empty() {
            entries.push(("typedef_chain".into(), self.typedef_chain.join(" -> ")));
        }
        if self.qualifiers.is_const {
            entries.push(("c_const".into(), "true".into()));
        }
        if self.qualifiers.is_volatile {
            entries.push(("c_volatile".into(), "true".into()));
        }
        if self.qualifiers.is_restrict {
            entries.push(("c_restrict".into(), "true".into()));
        }
        if self.qualifiers.is_atomic {
            entries.push(("c_atomic".into(), "true".into()));
        }
        entries
    }
}

#[derive(Debug, Clone)]
pub struct ResolvedType {
    pub ty: Option<Type>,
    pub c: CTypeMetadata,
}

pub struct TypeResolver {
    target: TargetInfo,
    aliases: HashMap<String, ResolvedType>,
    pub definitions: Vec<TypeDefinition>,
}

impl TypeResolver {
    pub fn new(target: TargetInfo) -> Self {
        Self {
            target,
            aliases: HashMap::new(),
            definitions: Vec::new(),
        }
    }

    pub fn define_alias(
        &mut self,
        name: String,
        resolved: ResolvedType,
    ) -> Result<(), ResolveError> {
        let ty = resolved
            .ty
            .ok_or(ResolveError::Unsupported("void typedef"))?;
        let id = self.push(TypeDefinitionKind::Alias(ty));
        self.definitions[id.0 as usize].name = Some(name.clone());
        self.aliases.insert(name, resolved);
        Ok(())
    }

    pub fn resolve(
        &mut self,
        specifiers: &DeclarationSpecifiers,
        declarator: &Declarator,
    ) -> Result<ResolvedType, ResolveError> {
        let (base, spelling, canonical, chain) = self.base(&specifiers.ty)?;
        let prefix = qualifier_spelling(specifiers.qualifiers);
        let mut resolved = ResolvedType {
            ty: base,
            c: CTypeMetadata {
                spelling: format!("{prefix}{spelling}"),
                canonical: format!("{prefix}{canonical}"),
                typedef_chain: chain,
                qualifiers: specifiers.qualifiers,
            },
        };
        self.derive(declarator, &mut resolved)?;
        Ok(resolved)
    }

    fn base(
        &self,
        specifier: &TypeSpecifier,
    ) -> Result<(Option<Type>, String, String, Vec<String>), ResolveError> {
        let scalar = match specifier {
            TypeSpecifier::Void => return Ok((None, "void".into(), "void".into(), Vec::new())),
            TypeSpecifier::Named(name) => {
                let alias = self
                    .aliases
                    .get(name)
                    .ok_or(ResolveError::Unsupported("unknown typedef"))?;
                let mut chain = vec![name.clone()];
                chain.extend(alias.c.typedef_chain.iter().cloned());
                return Ok((alias.ty, name.clone(), alias.c.canonical.clone(), chain));
            }
            TypeSpecifier::Bool => (Type::Bool, "_Bool".into()),
            TypeSpecifier::Integer(IntegerType::Char { signed }) => {
                let spelling = match signed {
                    None => "char",
                    Some(true) => "signed char",
                    Some(false) => "unsigned char",
                };
                (
                    Type::Numeric(NumericType::Integer {
                        width: 8,
                        signed: signed.unwrap_or(self.target.char_signed),
                    }),
                    spelling.into(),
                )
            }
            TypeSpecifier::Integer(IntegerType::Ranked { rank, signed }) => {
                let (name, width) = match rank {
                    IntegerRank::Short => ("short", self.target.short_width),
                    IntegerRank::Int => ("int", self.target.int_width),
                    IntegerRank::Long => ("long", self.target.long_width),
                    IntegerRank::LongLong => ("long long", self.target.long_long_width),
                    IntegerRank::Int128 => ("__int128", 128),
                };
                let spelling = if *signed {
                    name.into()
                } else {
                    format!("unsigned {name}")
                };
                (
                    Type::Numeric(NumericType::Integer {
                        width,
                        signed: *signed,
                    }),
                    spelling,
                )
            }
            TypeSpecifier::Floating(float) => {
                let (kind, spelling) = match float {
                    FloatingType::Float16 | FloatingType::Fp16 => (FloatType::F16, "_Float16"),
                    FloatingType::Float => (FloatType::F32, "float"),
                    FloatingType::Double => (FloatType::F64, "double"),
                    FloatingType::LongDouble => (
                        match self.target.long_double {
                            LongDoubleFormat::Binary64 => FloatType::F64,
                            LongDoubleFormat::X87 => FloatType::F80,
                            LongDoubleFormat::Binary128 => FloatType::F128,
                        },
                        "long double",
                    ),
                    FloatingType::Float128 | FloatingType::Float128Ext => {
                        (FloatType::F128, "__float128")
                    }
                    _ => return Err(ResolveError::Unsupported("floating type")),
                };
                (Type::Numeric(NumericType::Float(kind)), spelling.into())
            }
            _ => return Err(ResolveError::Unsupported("type specifier")),
        };
        Ok((Some(scalar.0), scalar.1.clone(), scalar.1, Vec::new()))
    }

    fn derive(
        &mut self,
        declarator: &Declarator,
        resolved: &mut ResolvedType,
    ) -> Result<(), ResolveError> {
        match declarator {
            Declarator::Name(_) | Declarator::Abstract => Ok(()),
            Declarator::Grouped(inner) | Declarator::Attributed { inner, .. } => {
                self.derive(inner, resolved)
            }
            Declarator::Pointer {
                inner, qualifiers, ..
            } => {
                let pointee = resolved.ty.unwrap_or(Type::Void);
                let grouped = matches!(pointee, Type::Defined(id) if matches!(self.definitions[id.0 as usize].kind, TypeDefinitionKind::Function { .. } | TypeDefinitionKind::Array { .. }));
                let id = self.push(TypeDefinitionKind::Pointer {
                    pointee,
                    is_const: resolved.c.qualifiers.is_const,
                });
                resolved.ty = Some(Type::Defined(id));
                resolved.c.spelling = pointer_spelling(&resolved.c.spelling, *qualifiers, grouped);
                resolved.c.canonical =
                    pointer_spelling(&resolved.c.canonical, *qualifiers, grouped);
                resolved.c.qualifiers = *qualifiers;
                self.derive(inner, resolved)
            }
            Declarator::Array { inner, size, .. } => {
                let element = resolved
                    .ty
                    .ok_or(ResolveError::Unsupported("void array element"))?;
                let length = match size {
                    ArraySize::Unspecified => None,
                    ArraySize::Star => {
                        return Err(ResolveError::Unsupported("variable length array"));
                    }
                    ArraySize::Expression(expr) => match &expr.value {
                        crate::ast::ExprKind::IntegerLiteral(literal) => Some(
                            u64::try_from(literal.value.clone())
                                .map_err(|_| ResolveError::Unsupported("array length overflow"))?,
                        ),
                        _ => return Err(ResolveError::Unsupported("nonconstant array length")),
                    },
                };
                let id = self.push(TypeDefinitionKind::Array { element, length });
                resolved.ty = Some(Type::Defined(id));
                let suffix = length.map_or("[]".to_owned(), |length| format!("[{length}]"));
                resolved.c.spelling.push_str(&suffix);
                resolved.c.canonical.push_str(&suffix);
                self.derive(inner, resolved)
            }
            Declarator::Function { inner, parameters } => {
                let mut types = Vec::new();
                let mut c_parameters = Vec::new();
                for parameter in parameters.parameters() {
                    let parameter_type =
                        self.resolve(&parameter.specifiers, &parameter.declarator)?;
                    types.push(
                        parameter_type
                            .ty
                            .ok_or(ResolveError::Unsupported("void parameter"))?,
                    );
                    c_parameters.push(parameter_type.c.spelling);
                }
                let id = self.push(TypeDefinitionKind::Function {
                    return_type: resolved.ty,
                    parameters: types,
                    variadic: parameters.is_variadic(),
                    prototyped: !matches!(parameters, ParameterList::Empty),
                });
                resolved.ty = Some(Type::Defined(id));
                let suffix = if matches!(parameters, ParameterList::Empty) {
                    "()".to_owned()
                } else if matches!(parameters, ParameterList::Void) {
                    "(void)".to_owned()
                } else {
                    if parameters.is_variadic() {
                        c_parameters.push("...".into());
                    }
                    format!("({})", c_parameters.join(", "))
                };
                resolved.c.spelling.push_str(&suffix);
                resolved.c.canonical.push_str(&suffix);
                self.derive(inner, resolved)
            }
        }
    }

    fn push(&mut self, kind: TypeDefinitionKind) -> TypeId {
        let id = TypeId(self.definitions.len() as u32);
        self.definitions.push(TypeDefinition {
            id,
            name: None,
            kind,
        });
        id
    }
}

fn qualifier_spelling(qualifiers: Qualifiers) -> String {
    let mut words = Vec::new();
    if qualifiers.is_const {
        words.push("const");
    }
    if qualifiers.is_volatile {
        words.push("volatile");
    }
    if qualifiers.is_restrict {
        words.push("restrict");
    }
    if qualifiers.is_atomic {
        words.push("_Atomic");
    }
    if words.is_empty() {
        String::new()
    } else {
        format!("{} ", words.join(" "))
    }
}

fn pointer_spelling(base: &str, qualifiers: Qualifiers, grouped: bool) -> String {
    let marker = format!("*{}", qualifier_spelling(qualifiers).trim_end());
    if grouped && let Some(offset) = base.find(['(', '[']) {
        return format!("{} ({marker}){}", &base[..offset], &base[offset..]);
    }
    format!("{base} {marker}")
}

pub fn resolve_type_module(
    unit: &crate::ast::TranslationUnit,
) -> Result<crate::ir::Module, ResolveError> {
    use crate::ast::{DeclKind, StorageClass};
    use crate::ir::{BindingId, Function, Linkage, Module};

    let target = unit.options.effective_target(unit.target.clone());
    let mut module = Module::new(target.clone());
    let mut resolver = TypeResolver::new(target);
    let mut next_binding = 0u32;
    for declaration in &unit.decls {
        match &declaration.value {
            DeclKind::Comment(_) | DeclKind::Pragma(_) | DeclKind::StaticAssert(_) => continue,
            DeclKind::Declaration(item) if item.specifiers.storage == StorageClass::Typedef => {
                for declarator in &item.declarators {
                    let start = resolver.definitions.len();
                    let name = declarator
                        .declarator
                        .name()
                        .ok_or(ResolveError::Unsupported("anonymous typedef"))?
                        .to_owned();
                    let resolved = resolver.resolve(&item.specifiers, &declarator.declarator)?;
                    module.metadata.insert(declarator.id, resolved.c.entries());
                    resolver.define_alias(name, resolved)?;
                    for definition in &resolver.definitions[start..] {
                        module
                            .types
                            .push(declarator.clone().with_value(definition.clone()));
                    }
                }
            }
            DeclKind::Function(function) => {
                let signature = function
                    .declarator
                    .function_parameters()
                    .ok_or(ResolveError::Unsupported("function declarator"))?;
                let name = function
                    .declarator
                    .name()
                    .ok_or(ResolveError::Unsupported("function name"))?;
                let return_type = resolver.resolve(&function.specifiers, &Declarator::Abstract)?;
                let parameters =
                    resolve_parameters(&mut resolver, signature, &mut module, &mut next_binding)?;
                module
                    .metadata
                    .insert(declaration.id, return_type.c.entries());
                module
                    .functions
                    .push(declaration.clone().with_value(Function {
                        id: BindingId(next_binding),
                        name: name.into(),
                        parameters,
                        return_type: return_type.ty,
                        linkage: if function.specifiers.storage == StorageClass::Static {
                            Linkage::Internal
                        } else {
                            Linkage::External
                        },
                        body: None,
                    }));
                next_binding += 1;
            }
            DeclKind::Declaration(item) => {
                for declarator in &item.declarators {
                    let signature = declarator.declarator.function_parameters().ok_or(
                        ResolveError::Unsupported("non-function declaration in type view"),
                    )?;
                    let name = declarator
                        .declarator
                        .name()
                        .ok_or(ResolveError::Unsupported("function name"))?;
                    let return_type = resolver.resolve(&item.specifiers, &Declarator::Abstract)?;
                    let parameters = resolve_parameters(
                        &mut resolver,
                        signature,
                        &mut module,
                        &mut next_binding,
                    )?;
                    module
                        .metadata
                        .insert(declarator.id, return_type.c.entries());
                    module
                        .functions
                        .push(declarator.clone().with_value(Function {
                            id: BindingId(next_binding),
                            name: name.into(),
                            parameters,
                            return_type: return_type.ty,
                            linkage: if item.specifiers.storage == StorageClass::Static {
                                Linkage::Internal
                            } else {
                                Linkage::External
                            },
                            body: None,
                        }));
                    next_binding += 1;
                }
            }
            _ => return Err(ResolveError::Unsupported("declaration in type view")),
        }
    }
    for definition in &resolver.definitions {
        if !module
            .types
            .iter()
            .any(|entry| entry.value.id == definition.id)
        {
            return Err(ResolveError::Unsupported("unattached derived type"));
        }
    }
    Ok(module)
}

fn resolve_parameters(
    resolver: &mut TypeResolver,
    signature: &ParameterList,
    module: &mut crate::ir::Module,
    next_binding: &mut u32,
) -> Result<crate::ir::Parameters, ResolveError> {
    use crate::ir::{BindingId, Parameter, Parameters};
    if matches!(signature, ParameterList::Empty) {
        return Ok(Parameters::Unprototyped);
    }
    let mut fixed = Vec::new();
    for parameter in signature.parameters() {
        let start = resolver.definitions.len();
        let resolved = resolver.resolve(&parameter.specifiers, &parameter.declarator)?;
        let ty = resolved
            .ty
            .ok_or(ResolveError::Unsupported("void parameter"))?;
        module.metadata.insert(parameter.id, resolved.c.entries());
        for definition in &resolver.definitions[start..] {
            module
                .types
                .push(parameter.clone().with_value(definition.clone()));
        }
        fixed.push(parameter.clone().with_value(Parameter {
            id: BindingId(*next_binding),
            name: parameter.declarator.name().map(str::to_owned),
            ty,
        }));
        *next_binding += 1;
    }
    Ok(Parameters::Prototype {
        fixed,
        variadic: signature.is_variadic(),
    })
}
