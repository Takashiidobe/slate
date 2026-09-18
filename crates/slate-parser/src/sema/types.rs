use std::collections::HashMap;

use crate::ast::{
    AlignAsOperand, ArraySize, Attribute, DeclarationSpecifiers, Declarator, EnumItemKind,
    FieldItemKind, FloatingType, IntegerRank, IntegerType, ParameterList, Qualifiers, TagBody,
    TagDefinition, TagId, TagKind, TagSpecifier, TranslationUnit, TypeSpecifier,
};
use crate::const_expr::Encoding;
use crate::ir::{
    BindingId, BitFieldUnit, Enumerator, Field, FloatType, Number, NumericType, RecordKind,
    RecordLayout, Type, TypeDefinition, TypeDefinitionKind, TypeId, Value, ValueKind,
};
use crate::target_info::{LongDoubleFormat, StorageLayout, TargetInfo};
use num_bigint::{BigInt, BigUint};

use super::numeric::ResolveError;
use crate::standard_features::StandardFeatures;

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
    pub features: StandardFeatures,
    tags: Vec<crate::ast::Span<TagDefinition>>,
    tag_ids: HashMap<TagId, TypeId>,
    ordinary: Vec<HashMap<String, Ordinary>>,
    tag_names: Vec<HashMap<(TagKind, String), TypeId>>,
    pub definitions: Vec<TypeDefinition>,
    pub(super) assertion_scope: bool,
}

pub(super) enum Ordinary {
    Declared,
    Alias(ResolvedType),
    Constant(Value),
    Object(Type),
}

impl TypeResolver {
    pub fn new(target: TargetInfo) -> Self {
        Self {
            target,
            features: StandardFeatures::default(),
            tags: Vec::new(),
            tag_ids: HashMap::new(),
            ordinary: vec![HashMap::new()],
            tag_names: vec![HashMap::new()],
            definitions: Vec::new(),
            assertion_scope: false,
        }
    }

    pub fn with_tags(target: TargetInfo, unit: &TranslationUnit) -> Self {
        let mut resolver = Self::new(target);
        resolver.features = StandardFeatures::new(unit.standard);
        resolver.tags = unit.tags.clone();
        resolver
    }

    pub fn tag_span<'a>(
        &self,
        id: TypeId,
        unit: &'a TranslationUnit,
    ) -> Option<&'a crate::ast::Span<TagDefinition>> {
        unit.tags
            .iter()
            .find(|tag| self.tag_ids.get(&tag.value.id) == Some(&id))
    }

    pub(super) fn constant_integer(
        &mut self,
        e: &crate::ast::Expr,
    ) -> Result<BigInt, ResolveError> {
        let value = self.constant_value(e)?;
        super::fold::integer(&value).ok_or(ResolveError::Unsupported(
            "nonconstant or undefined integer expression",
        ))
    }

    pub(super) fn push_scope(&mut self) {
        self.ordinary.push(HashMap::new());
        self.tag_names.push(HashMap::new());
    }

    pub(super) fn pop_scope(&mut self) {
        if self.ordinary.len() > 1 {
            self.ordinary.pop();
            self.tag_names.pop();
        }
    }

    pub(super) fn declare(&mut self, name: &str, entry: Ordinary) {
        if let Some(scope) = self.ordinary.last_mut() {
            scope.insert(name.to_owned(), entry);
        }
    }

    fn lookup(&self, name: &str) -> Option<&Ordinary> {
        self.ordinary.iter().rev().find_map(|scope| scope.get(name))
    }

    fn declare_tag(&mut self, key: (TagKind, String), id: TypeId) {
        if let Some(scope) = self.tag_names.last_mut() {
            scope.insert(key, id);
        }
    }

    fn lookup_tag(&self, key: &(TagKind, String)) -> Option<TypeId> {
        self.tag_names
            .iter()
            .rev()
            .find_map(|scope| scope.get(key))
            .copied()
    }

    pub(super) fn constant_value(&mut self, e: &crate::ast::Expr) -> Result<Value, ResolveError> {
        use crate::ast::ExprKind;
        let context =
            super::numeric::Context::new(self.target.clone()).with_features(self.features);
        let (ty, kind) = match &e.value {
            ExprKind::Paren(inner) => return self.constant_value(inner),
            ExprKind::Identifier(name) => {
                return match self.lookup(name) {
                    Some(Ordinary::Constant(value)) => Ok(value.clone()),
                    _ => Err(ResolveError::Unsupported(
                        "nonconstant or unknown identifier",
                    )),
                };
            }
            ExprKind::CharLiteral(literal) => {
                let (ty, number) = character_constant(literal, &self.target)?;
                (ty, ValueKind::Constant(number))
            }
            ExprKind::SizeOfExpr(operand) | ExprKind::AlignOfExpr(operand) => {
                let ty = self.assertion_operand_type(operand)?;
                let layout = self.storage(ty)?;
                let n = if matches!(e.value, ExprKind::SizeOfExpr(_)) {
                    layout.size_bytes
                } else {
                    u64::from(layout.alignment_bytes)
                };
                (
                    Type::integer(self.target.pointer_width, false),
                    ValueKind::Constant(Number::Integer(n.into())),
                )
            }
            ExprKind::Conditional {
                condition,
                then_value,
                else_value,
            } => {
                let condition = self.constant_value(condition)?;
                let left = match then_value {
                    Some(left) => self.constant_value(left)?,
                    None => condition.clone(),
                };
                let right = self.constant_value(else_value)?;
                let (left, right) =
                    context.usual_arithmetic(context.promote(left), context.promote(right));
                (
                    left.ty.clone(),
                    ValueKind::Conditional {
                        condition: Box::new(context.condition(condition)),
                        then_value: Box::new(left),
                        else_value: Box::new(right),
                    },
                )
            }
            ExprKind::SizeOfType { ty } | ExprKind::AlignOf { ty } => {
                let ty = self
                    .resolve(&ty.specifiers, &ty.declarator)?
                    .ty
                    .ok_or(ResolveError::Unsupported("void layout"))?;
                let layout = self.storage(ty)?;
                let n = if matches!(e.value, ExprKind::SizeOfType { .. }) {
                    layout.size_bytes
                } else {
                    u64::from(layout.alignment_bytes)
                };
                (
                    Type::integer(self.target.pointer_width, false),
                    ValueKind::Constant(Number::Integer(n.into())),
                )
            }
            ExprKind::OffsetOf { ty, member } => {
                let ty = self
                    .resolve(&ty.specifiers, &ty.declarator)?
                    .ty
                    .ok_or(ResolveError::Unsupported("void offsetof"))?;
                let (_, n) = self.offsetof_member(ty, member)?;
                (
                    Type::integer(self.target.pointer_width, false),
                    ValueKind::Constant(Number::Integer(n.into())),
                )
            }
            ExprKind::Binary { op, left, right } => {
                let left = self.constant_value(left)?;
                let right = self.constant_value(right)?;
                context.resolve_binary(*op, left, right)?
            }
            ExprKind::Unary { op, operand } => {
                use crate::const_expr::UnaryOp;
                let operand = self.constant_value(operand)?;
                match op {
                    UnaryOp::Plus => return Ok(context.promote(operand)),
                    UnaryOp::Minus | UnaryOp::BitNot => {
                        context.resolve_unary_arith(*op, operand)?
                    }
                    UnaryOp::Not => (
                        Type::Bool,
                        ValueKind::Unary {
                            op: crate::ir::UnaryArithOp::Not,
                            operand: Box::new(context.condition(operand)),
                            semantics: crate::ir::ArithSema::Exact,
                        },
                    ),
                    _ => return Err(ResolveError::Unsupported("nonconstant unary expression")),
                }
            }
            ExprKind::Comma { left, right } => {
                let left = self.constant_value(left)?;
                let right = self.constant_value(right)?;
                (
                    right.ty.clone(),
                    ValueKind::Sequence {
                        left: Box::new(left),
                        right: Box::new(right),
                    },
                )
            }
            ExprKind::Generic {
                controlling,
                associations,
            } => {
                let selected = self.generic_selection(controlling, associations)?;
                return self.constant_value(selected);
            }
            ExprKind::Cast { ty, value } => {
                let ty = self
                    .resolve(&ty.specifiers, &ty.declarator)?
                    .ty
                    .ok_or(ResolveError::Unsupported("void constant cast"))?;
                let value = context.convert(
                    self.constant_value(value)?,
                    ty,
                    crate::ir::ConversionReason::Explicit,
                );
                (value.ty, value.node.value)
            }
            _ => return context.resolve(e),
        };
        Ok(Value {
            ty,
            node: e.clone().with_value(kind),
        })
    }

    pub(super) fn generic_selection<'e>(
        &mut self,
        controlling: &'e crate::ast::GenericControl,
        associations: &'e [crate::ast::GenericAssociation],
    ) -> Result<&'e crate::ast::Expr, ResolveError> {
        use crate::ast::GenericControl;
        let controlling = match controlling {
            GenericControl::Type { ty } => self
                .resolve(&ty.specifiers, &ty.declarator)?
                .ty
                .ok_or(ResolveError::Unsupported("void generic controlling type"))?,
            GenericControl::Expr(expr) => self.assertion_operand_type(expr)?,
        };
        self.select_association(controlling, associations)
    }

    // The controlling operand is lvalue-converted, so array and function associations never match.
    pub(super) fn select_association<'e>(
        &mut self,
        controlling: Type,
        associations: &'e [crate::ast::GenericAssociation],
    ) -> Result<&'e crate::ast::Expr, ResolveError> {
        use crate::ast::GenericAssociation;
        let controlling = match controlling {
            Type::Array { element, .. } => Type::Pointer {
                pointee: element,
                is_const: false,
            },
            ty @ Type::Function { .. } => Type::Pointer {
                pointee: Box::new(ty),
                is_const: false,
            },
            ty => ty,
        };
        let mut selected = None;
        let mut fallback = None;
        for association in associations {
            match association {
                GenericAssociation::Default(value) => fallback = Some(value),
                GenericAssociation::Type { ty, value } => {
                    let ty = self
                        .resolve(&ty.specifiers, &ty.declarator)?
                        .ty
                        .ok_or(ResolveError::Unsupported("void generic association type"))?;
                    if ty == controlling {
                        if selected.is_some() {
                            return Err(ResolveError::Unsupported("ambiguous generic selection"));
                        }
                        selected = Some(value);
                    }
                }
            }
        }
        selected
            .or(fallback)
            .ok_or(ResolveError::Unsupported("unselected generic association"))
    }

    pub(super) fn assertion_operand_type(
        &mut self,
        e: &crate::ast::Expr,
    ) -> Result<Type, ResolveError> {
        use crate::ast::ExprKind;
        match &e.value {
            ExprKind::Paren(inner) => self.assertion_operand_type(inner),
            ExprKind::Generic {
                controlling,
                associations,
            } => {
                let selected = self.generic_selection(controlling, associations)?;
                self.assertion_operand_type(selected)
            }
            ExprKind::Call { callee, .. } => match &callee.value {
                ExprKind::Identifier(name) => builtin_result_type(name),
                _ => None,
            }
            .ok_or(ResolveError::Unsupported("nonconstant call expression")),
            ExprKind::Identifier(name) => match self.lookup(name) {
                Some(Ordinary::Object(ty)) => Ok(ty.clone()),
                Some(Ordinary::Constant(value)) => Ok(value.ty.clone()),
                _ => Err(ResolveError::Unsupported(
                    "unknown or unsupported sizeof operand type",
                )),
            },
            ExprKind::StringLiteral(literal) => {
                Ok(string_literal_type(literal, &self.target, self.features))
            }
            ExprKind::Unary {
                op: crate::const_expr::UnaryOp::Deref,
                operand,
            } => match self.assertion_operand_type(operand)? {
                Type::Pointer { pointee, .. } => Ok(*pointee),
                _ => Err(ResolveError::Unsupported(
                    "sizeof dereference of nonpointer",
                )),
            },
            ExprKind::Index { base, .. } => match self.assertion_operand_type(base)? {
                Type::Array { element, .. } => Ok(*element),
                Type::Pointer { pointee, .. } => Ok(*pointee),
                _ => Err(ResolveError::Unsupported("sizeof index of nonarray")),
            },
            _ => self.constant_value(e).map(|value| value.ty),
        }
    }

    pub(super) fn offsetof_member(
        &mut self,
        root: Type,
        member: &crate::ast::Expr,
    ) -> Result<(Type, u64), ResolveError> {
        use crate::ast::ExprKind;
        match &member.value {
            ExprKind::Identifier(name) => self.offsetof_field(root, name),
            ExprKind::Member {
                base,
                field,
                arrow: false,
            } => {
                let (ty, offset) = self.offsetof_member(root, base)?;
                let (ty, field_offset) = self.offsetof_field(ty, &field.value)?;
                Ok((
                    ty,
                    offset
                        .checked_add(field_offset)
                        .ok_or(ResolveError::Unsupported("offsetof overflow"))?,
                ))
            }
            ExprKind::Index { base, index } => {
                let (ty, offset) = self.offsetof_member(root, base)?;
                let Type::Array { element, .. } = ty else {
                    return Err(ResolveError::Unsupported("offsetof index of non-array"));
                };
                let index = u64::try_from(self.constant_integer(index)?)
                    .map_err(|_| ResolveError::Unsupported("invalid offsetof index"))?;
                let size = self.storage((*element).clone())?.size_bytes;
                let offset = size
                    .checked_mul(index)
                    .and_then(|n| offset.checked_add(n))
                    .ok_or(ResolveError::Unsupported("offsetof overflow"))?;
                Ok((*element, offset))
            }
            _ => Err(ResolveError::Unsupported("offsetof member path")),
        }
    }

    fn offsetof_field(&self, ty: Type, name: &str) -> Result<(Type, u64), ResolveError> {
        let Type::Defined(id) = ty else {
            return Err(ResolveError::Unsupported("offsetof field of non-record"));
        };
        let Some(TypeDefinition {
            kind:
                TypeDefinitionKind::Record {
                    fields: Some(fields),
                    layout: Some(layout),
                    ..
                },
            ..
        }) = self.definitions.get(id.0 as usize)
        else {
            return Err(ResolveError::Unsupported(
                "offsetof incomplete or non-record",
            ));
        };
        let (index, field) = fields
            .iter()
            .enumerate()
            .find(|(_, f)| f.name.as_deref() == Some(name))
            .ok_or(ResolveError::Unsupported("unknown offsetof member"))?;
        if field.bit_width.is_some() {
            return Err(ResolveError::Unsupported("offsetof bit-field"));
        }
        let offset = *layout
            .offsets
            .get(index)
            .ok_or(ResolveError::Unsupported("missing field offset"))?;
        Ok((field.ty.clone(), offset))
    }

    pub fn define_alias(
        &mut self,
        name: String,
        resolved: ResolvedType,
    ) -> Result<(), ResolveError> {
        let ty = resolved
            .ty
            .clone()
            .ok_or(ResolveError::Unsupported("void typedef"))?;
        let id = self.push(TypeDefinitionKind::Alias(ty));
        self.definitions[id.0 as usize].name = Some(name.clone());
        self.declare(&name, Ordinary::Alias(resolved));
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
        &mut self,
        specifier: &TypeSpecifier,
    ) -> Result<(Option<Type>, String, String, Vec<String>), ResolveError> {
        let scalar = match specifier {
            TypeSpecifier::Void => return Ok((None, "void".into(), "void".into(), Vec::new())),
            TypeSpecifier::Named(name) => {
                let Some(Ordinary::Alias(alias)) = self.lookup(name) else {
                    return Err(ResolveError::Unsupported("unknown typedef"));
                };
                let mut chain = vec![name.clone()];
                chain.extend(alias.c.typedef_chain.iter().cloned());
                return Ok((
                    alias.ty.clone(),
                    name.clone(),
                    alias.c.canonical.clone(),
                    chain,
                ));
            }
            TypeSpecifier::Tag(TagSpecifier::Definition(id)) => {
                let tag = self
                    .tags
                    .iter()
                    .find(|tag| tag.value.id == *id)
                    .cloned()
                    .ok_or(ResolveError::Unsupported("unknown tag definition"))?;
                let ty = Type::Defined(self.define_tag(&tag.value)?);
                let spelling = tag_spelling(tag.kind, tag.name.as_deref());
                return Ok((Some(ty), spelling.clone(), spelling, Vec::new()));
            }
            TypeSpecifier::Tag(TagSpecifier::Reference {
                kind,
                name,
                fixed_type,
            }) => {
                let key = (*kind, name.clone());
                let id = if let Some(id) = self.lookup_tag(&key) {
                    id
                } else {
                    let tag = self
                        .tags
                        .iter()
                        .find(|tag| {
                            !self.assertion_scope
                                && tag.kind == *kind
                                && tag.name.as_deref() == Some(name)
                        })
                        .cloned();
                    if let Some(tag) = tag {
                        self.define_tag(&tag.value)?
                    } else {
                        let id = self.push(incomplete_tag(*kind));
                        self.definitions[id.0 as usize].name = Some(name.clone());
                        self.declare_tag(key, id);
                        id
                    }
                };
                if let Some(fixed_type) = fixed_type
                    && matches!(
                        self.definitions[id.0 as usize].kind,
                        TypeDefinitionKind::Enum {
                            underlying: None,
                            ..
                        }
                    )
                {
                    let underlying = self
                        .resolve(&fixed_type.specifiers, &fixed_type.declarator)?
                        .ty
                        .ok_or(ResolveError::Unsupported("void enum underlying type"))?;
                    let layout = self.storage(underlying.clone())?;
                    self.definitions[id.0 as usize].kind = TypeDefinitionKind::Enum {
                        underlying: Some(underlying),
                        enumerators: None,
                        layout: Some(layout),
                    };
                }
                let spelling = tag_spelling(*kind, Some(name));
                return Ok((
                    Some(Type::Defined(id)),
                    spelling.clone(),
                    spelling,
                    Vec::new(),
                ));
            }
            TypeSpecifier::Bool => (Type::Bool, "_Bool".into()),
            TypeSpecifier::Complex(inner) => {
                let (component, spelling, canonical, chain) = self.base(inner)?;
                let Some(Type::Numeric(component)) = component else {
                    return Err(ResolveError::Unsupported("complex component type"));
                };
                return Ok((
                    Some(Type::Complex(component)),
                    format!("_Complex {spelling}"),
                    format!("_Complex {canonical}"),
                    chain,
                ));
            }
            TypeSpecifier::Integer(IntegerType::Char { signed }) => {
                let spelling = match signed {
                    None => "char",
                    Some(true) => "signed char",
                    Some(false) => "unsigned char",
                };
                (
                    Type::integer(8, signed.unwrap_or(self.target.char_signed)),
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
                (Type::integer(width, *signed), spelling)
            }
            TypeSpecifier::Integer(IntegerType::BitInt { width, signed }) => {
                let width = u32::try_from(self.constant_integer(width)?)
                    .map_err(|_| ResolveError::Unsupported("invalid _BitInt width"))?;
                if width < if *signed { 2 } else { 1 } || width > 65535 {
                    return Err(ResolveError::Unsupported("invalid _BitInt width"));
                }
                (
                    Type::bit_precise(width, *signed),
                    format!("{}_BitInt({width})", if *signed { "" } else { "unsigned " }),
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
                    FloatingType::Decimal32 => (FloatType::D32, "_Decimal32"),
                    FloatingType::Decimal64 => (FloatType::D64, "_Decimal64"),
                    FloatingType::Decimal128 => (FloatType::D128, "_Decimal128"),
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
                let pointee = resolved.ty.take().unwrap_or(Type::Void);
                let grouped = matches!(pointee, Type::Function { .. } | Type::Array { .. });
                resolved.ty = Some(Type::Pointer {
                    pointee: Box::new(pointee),
                    is_const: resolved.c.qualifiers.is_const,
                });
                resolved.c.spelling = pointer_spelling(&resolved.c.spelling, *qualifiers, grouped);
                resolved.c.canonical =
                    pointer_spelling(&resolved.c.canonical, *qualifiers, grouped);
                resolved.c.qualifiers = *qualifiers;
                self.derive(inner, resolved)
            }
            Declarator::Array { inner, size, .. } => {
                let length = match size {
                    ArraySize::Unspecified => None,
                    ArraySize::Star => {
                        return Err(ResolveError::Unsupported("variable length array"));
                    }
                    ArraySize::Expression(expr) => Some(
                        u64::try_from(self.constant_integer(expr)?)
                            .map_err(|_| ResolveError::Unsupported("invalid array length"))?,
                    ),
                };
                let suffix = length.map_or("[]".to_owned(), |length| format!("[{length}]"));
                if let Declarator::Grouped(core) = inner.as_ref() {
                    let element = resolved
                        .ty
                        .take()
                        .ok_or(ResolveError::Unsupported("void array element"))?;
                    resolved.ty = Some(Type::Array {
                        element: Box::new(element),
                        length,
                    });
                    resolved.c.spelling.push_str(&suffix);
                    resolved.c.canonical.push_str(&suffix);
                    self.derive(core, resolved)
                } else {
                    self.derive(inner, resolved)?;
                    let element = resolved
                        .ty
                        .take()
                        .ok_or(ResolveError::Unsupported("void array element"))?;
                    resolved.ty = Some(Type::Array {
                        element: Box::new(element),
                        length,
                    });
                    resolved.c.spelling.push_str(&suffix);
                    resolved.c.canonical.push_str(&suffix);
                    Ok(())
                }
            }
            Declarator::Function { inner, parameters } => {
                let mut types = Vec::new();
                let mut c_parameters = Vec::new();
                for parameter in parameters.parameters() {
                    let parameter_type =
                        self.resolve(&parameter.specifiers, &parameter.declarator)?;
                    let mut ty = parameter_type
                        .ty
                        .ok_or(ResolveError::Unsupported("void parameter"))?;
                    ty = match ty {
                        Type::Array { element, .. } => Type::Pointer {
                            pointee: element,
                            is_const: false,
                        },
                        function @ Type::Function { .. } => Type::Pointer {
                            pointee: Box::new(function),
                            is_const: false,
                        },
                        other => other,
                    };
                    types.push(ty);
                    c_parameters.push(parameter_type.c.spelling);
                }
                let variadic = parameters.is_variadic();
                let prototyped = self.features.empty_parens_are_prototype
                    || !matches!(parameters, ParameterList::Empty);
                let suffix = if matches!(parameters, ParameterList::Empty) {
                    "()".to_owned()
                } else if matches!(parameters, ParameterList::Void) {
                    "(void)".to_owned()
                } else {
                    if variadic {
                        c_parameters.push("...".into());
                    }
                    format!("({})", c_parameters.join(", "))
                };
                if let Declarator::Grouped(core) = inner.as_ref() {
                    resolved.ty = Some(Type::Function {
                        return_type: resolved.ty.take().map(Box::new),
                        parameters: types,
                        variadic,
                        prototyped,
                    });
                    resolved.c.spelling.push_str(&suffix);
                    resolved.c.canonical.push_str(&suffix);
                    self.derive(core, resolved)
                } else {
                    self.derive(inner, resolved)?;
                    resolved.ty = Some(Type::Function {
                        return_type: resolved.ty.take().map(Box::new),
                        parameters: types,
                        variadic,
                        prototyped,
                    });
                    resolved.c.spelling.push_str(&suffix);
                    resolved.c.canonical.push_str(&suffix);
                    Ok(())
                }
            }
        }
    }

    fn define_tag(&mut self, tag: &TagDefinition) -> Result<TypeId, ResolveError> {
        if let Some(id) = self.tag_ids.get(&tag.id) {
            return Ok(*id);
        }
        let id = if let Some(name) = &tag.name {
            self.tag_names
                .last()
                .and_then(|scope| scope.get(&(tag.kind, name.clone())))
                .copied()
        } else {
            None
        }
        .unwrap_or_else(|| self.push(incomplete_tag(tag.kind)));
        self.tag_ids.insert(tag.id, id);
        self.definitions[id.0 as usize].name = tag.name.clone();
        if let Some(name) = &tag.name {
            self.declare_tag((tag.kind, name.clone()), id);
        }
        let kind = match &tag.body {
            TagBody::Record(items) => {
                let mut fields = Vec::new();
                let mut requests = Vec::new();
                for item in items {
                    let FieldItemKind::Field(declaration) = &item.value else {
                        continue;
                    };
                    if declaration.declarators.is_empty() {
                        // a tagged struct/union body with no declarator declares the tag, not a member
                        let anonymous = match &declaration.specifiers.ty {
                            TypeSpecifier::Tag(TagSpecifier::Definition(id)) => self
                                .tags
                                .iter()
                                .find(|tag| tag.value.id == *id)
                                .is_some_and(|tag| tag.name.is_none() && tag.kind != TagKind::Enum),
                            _ => false,
                        };
                        if !anonymous {
                            self.resolve(&declaration.specifiers, &Declarator::Abstract)?;
                            continue;
                        }
                        let resolved =
                            self.resolve(&declaration.specifiers, &Declarator::Abstract)?;
                        let ty = resolved
                            .ty
                            .ok_or(ResolveError::Unsupported("void record field"))?;
                        fields.push(item.clone().with_value(Field {
                            name: None,
                            ty,
                            bit_width: None,
                        }));
                        requests.push(field_request(
                            self,
                            &declaration.specifiers.attributes,
                            &[],
                        )?);
                    }
                    for declarator in &declaration.declarators {
                        let resolved =
                            self.resolve(&declaration.specifiers, &declarator.declarator)?;
                        let ty = resolved
                            .ty
                            .ok_or(ResolveError::Unsupported("void record field"))?;
                        let bit_width = declarator
                            .bit_width
                            .as_ref()
                            .map(|expr| {
                                let value = crate::const_expr::Parser::evaluate_ast(expr)?;
                                u32::try_from(value).map_err(|_| {
                                    ResolveError::Unsupported("invalid bit-field width")
                                })
                            })
                            .transpose()?;
                        fields.push(declarator.clone().with_value(Field {
                            name: declarator.declarator.name().map(str::to_owned),
                            ty,
                            bit_width,
                        }));
                        requests.push(field_request(
                            self,
                            &declaration.specifiers.attributes,
                            &declarator.attributes,
                        )?);
                    }
                }
                let packed = tag
                    .attributes
                    .iter()
                    .any(|attribute| matches!(attribute, Attribute::Packed));
                let alignment = requested_alignment(self, &tag.attributes)?;
                let layout = self.layout_record(tag.kind, &fields, &requests, packed, alignment)?;
                TypeDefinitionKind::Record {
                    kind: match tag.kind {
                        TagKind::Struct => RecordKind::Struct,
                        TagKind::Union => RecordKind::Union,
                        TagKind::Enum => return Err(ResolveError::Unsupported("enum record body")),
                    },
                    fields: Some(fields),
                    layout: Some(layout),
                }
            }
            TagBody::Enum {
                fixed_type,
                enumerators,
            } => {
                let fixed_underlying = if let Some(fixed_type) = fixed_type {
                    self.resolve(&fixed_type.specifiers, &fixed_type.declarator)?
                        .ty
                        .ok_or(ResolveError::Unsupported("void enum underlying type"))?
                } else {
                    Type::Void
                };
                let mut values = Vec::new();
                let mut previous = -1i64;
                let mut prior = HashMap::new();
                for item in enumerators {
                    let EnumItemKind::Enumerator(enumerator) = &item.value else {
                        continue;
                    };
                    let value = if let Some(expr) = &enumerator.value {
                        i64::try_from(self.constant_integer(&substitute_enumerators(expr, &prior))?)
                            .map_err(|_| {
                                ResolveError::Unsupported("enum value outside supported i64 range")
                            })?
                    } else {
                        previous
                            .checked_add(1)
                            .ok_or(ResolveError::Unsupported("enum value overflow"))?
                    };
                    previous = value;
                    prior.insert(enumerator.name.clone(), value);
                    values.push((item, enumerator, value));
                }
                let underlying = if fixed_underlying != Type::Void {
                    fixed_underlying
                } else if values
                    .iter()
                    .all(|(_, _, value)| i32::try_from(*value).is_ok())
                {
                    Type::integer(self.target.int_width, true)
                } else if values
                    .iter()
                    .all(|(_, _, value)| u32::try_from(*value).is_ok())
                {
                    Type::integer(self.target.int_width, false)
                } else {
                    Type::integer(self.target.long_width, true)
                };
                let mut entries = Vec::new();
                for (item, enumerator, value) in values {
                    entries.push(item.clone().with_value(Enumerator {
                        id: BindingId(entries.len() as u32),
                        name: enumerator.name.clone(),
                        value: Value {
                            ty: underlying.clone(),
                            node: item.clone().with_value(ValueKind::Constant(if value < 0 {
                                Number::SignedInteger(BigInt::from(value))
                            } else {
                                Number::Integer(BigUint::from(value as u64))
                            })),
                        },
                    }));
                }
                TypeDefinitionKind::Enum {
                    underlying: Some(underlying.clone()),
                    enumerators: Some(entries),
                    layout: Some({
                        let mut layout = self.storage(underlying)?;
                        if let Some(alignment) = requested_alignment(self, &tag.attributes)? {
                            layout.alignment_bytes =
                                u32::try_from(u64::from(layout.alignment_bytes).max(alignment))
                                    .map_err(|_| {
                                        ResolveError::Unsupported("enum alignment overflow")
                                    })?;
                        }
                        layout
                    }),
                }
            }
        };
        self.definitions[id.0 as usize].kind = kind;
        Ok(id)
    }

    pub(super) fn storage(&self, ty: Type) -> Result<StorageLayout, ResolveError> {
        match ty {
            Type::Defined(id) => match &self.definitions[id.0 as usize].kind {
                TypeDefinitionKind::Alias(inner) => self.storage(inner.clone()),
                TypeDefinitionKind::Record {
                    layout: Some(layout),
                    ..
                } => Ok(StorageLayout {
                    size_bytes: layout.size,
                    alignment_bytes: u32::try_from(layout.align)
                        .map_err(|_| ResolveError::Unsupported("record alignment overflow"))?,
                }),
                TypeDefinitionKind::Enum {
                    layout: Some(layout),
                    ..
                } => Ok(*layout),
                TypeDefinitionKind::Enum {
                    underlying: Some(underlying),
                    ..
                } => self.storage(underlying.clone()),
                _ => Err(ResolveError::Unsupported("incomplete field type")),
            },
            Type::Pointer { .. } => Ok(self.target.pointer),
            Type::Array {
                element,
                length: Some(length),
            } => {
                let element = self.storage(*element)?;
                Ok(StorageLayout {
                    size_bytes: align_up(element.size_bytes, u64::from(element.alignment_bytes))?
                        .checked_mul(length)
                        .ok_or(ResolveError::Unsupported("array size overflow"))?,
                    alignment_bytes: element.alignment_bytes,
                })
            }
            Type::Array { length: None, .. } | Type::Function { .. } => {
                Err(ResolveError::Unsupported("incomplete field type"))
            }
            _ => Ok(self.target.storage_of(ty)?),
        }
    }

    fn layout_record(
        &self,
        kind: TagKind,
        fields: &[crate::ast::Span<Field>],
        requests: &[(bool, Option<u64>)],
        packed: bool,
        requested: Option<u64>,
    ) -> Result<RecordLayout, ResolveError> {
        let mut end_bits = 0u64;
        let mut aggregate_align = requested.unwrap_or(1);
        let mut offsets = Vec::new();
        let mut bit_offsets = Vec::new();
        for (field, &(field_packed, field_aligned)) in fields.iter().zip(requests) {
            let storage = self.storage(field.ty.clone())?;
            let natural = u64::from(storage.alignment_bytes);
            let align =
                (if packed || field_packed { 1 } else { natural }).max(field_aligned.unwrap_or(1));
            if field.bit_width == Some(0) && self.target.abi.zero_width_bitfield_aligns_record {
                aggregate_align = aggregate_align.max(natural);
            } else if field.bit_width != Some(0) {
                aggregate_align = aggregate_align.max(align);
            }
            if let Some(width) = field.bit_width {
                let unit_bits = storage
                    .size_bytes
                    .checked_mul(8)
                    .ok_or(ResolveError::Unsupported("bit-field unit overflow"))?;
                if u64::from(width) > unit_bits {
                    return Err(ResolveError::Unsupported("bit-field wider than its type"));
                }
                if width == 0 {
                    if field.name.is_some() {
                        return Err(ResolveError::Unsupported("named zero-width bit-field"));
                    }
                    let position = if kind == TagKind::Union {
                        0
                    } else {
                        align_up(
                            end_bits,
                            natural
                                .checked_mul(8)
                                .ok_or(ResolveError::Unsupported("field alignment overflow"))?,
                        )?
                    };
                    end_bits = end_bits.max(position);
                    offsets.push(position / 8);
                    bit_offsets.push(Some(position));
                    continue;
                }
                let position = if kind == TagKind::Union {
                    0
                } else if packed || field_packed {
                    end_bits
                } else {
                    let boundary = align_up(
                        end_bits,
                        align
                            .checked_mul(8)
                            .ok_or(ResolveError::Unsupported("field alignment overflow"))?,
                    )?;
                    let last_bit = end_bits
                        .checked_add(u64::from(width) - 1)
                        .ok_or(ResolveError::Unsupported("record size overflow"))?;
                    if end_bits == 0 || end_bits / unit_bits != last_bit / unit_bits {
                        boundary
                    } else {
                        end_bits
                    }
                };
                let used = position
                    .checked_add(u64::from(width))
                    .ok_or(ResolveError::Unsupported("record size overflow"))?;
                end_bits = end_bits.max(if kind == TagKind::Union && !(packed || field_packed) {
                    unit_bits
                } else {
                    used
                });
                offsets.push(position / 8);
                bit_offsets.push(Some(position));
            } else {
                let position = if kind == TagKind::Union {
                    0
                } else {
                    align_up(end_bits.div_ceil(8), align)?
                };
                let end = position
                    .checked_add(storage.size_bytes)
                    .and_then(|bytes| bytes.checked_mul(8))
                    .ok_or(ResolveError::Unsupported("record size overflow"))?;
                end_bits = end_bits.max(end);
                offsets.push(position);
                bit_offsets.push(None);
            }
        }
        let size = align_up(end_bits.div_ceil(8), aggregate_align)?;
        let mut bit_units: Vec<BitFieldUnit> = Vec::new();
        let mut field_units = Vec::new();
        let mut prior_end = None;
        for (field, bit_offset) in fields.iter().zip(&bit_offsets) {
            let (Some(width), Some(position)) =
                (field.bit_width.filter(|width| *width != 0), bit_offset)
            else {
                field_units.push(None);
                prior_end = None;
                continue;
            };
            let end = position + u64::from(width);
            let unit = if kind == TagKind::Struct && prior_end == Some(*position) {
                let index = bit_units.len() - 1;
                bit_units[index].size = end.div_ceil(8) - bit_units[index].offset;
                index
            } else {
                let index = bit_units.len();
                bit_units.push(BitFieldUnit {
                    offset: position / 8,
                    size: end.div_ceil(8) - position / 8,
                });
                index
            };
            field_units.push(Some(unit));
            prior_end = Some(end);
        }
        Ok(RecordLayout {
            size,
            align: aggregate_align,
            offsets,
            bit_offsets,
            bit_units,
            field_units,
        })
    }

    pub(super) fn push(&mut self, kind: TypeDefinitionKind) -> TypeId {
        let id = TypeId(self.definitions.len() as u32);
        self.definitions.push(TypeDefinition {
            id,
            name: None,
            kind,
        });
        id
    }
}

fn incomplete_tag(kind: TagKind) -> TypeDefinitionKind {
    match kind {
        TagKind::Struct | TagKind::Union => TypeDefinitionKind::Record {
            kind: if kind == TagKind::Struct {
                RecordKind::Struct
            } else {
                RecordKind::Union
            },
            fields: None,
            layout: None,
        },
        TagKind::Enum => TypeDefinitionKind::Enum {
            underlying: None,
            enumerators: None,
            layout: None,
        },
    }
}

fn tag_spelling(kind: TagKind, name: Option<&str>) -> String {
    let prefix = match kind {
        TagKind::Struct => "struct",
        TagKind::Union => "union",
        TagKind::Enum => "enum",
    };
    name.map_or_else(|| prefix.to_owned(), |name| format!("{prefix} {name}"))
}

fn align_up(value: u64, alignment: u64) -> Result<u64, ResolveError> {
    if alignment == 0 || !alignment.is_power_of_two() {
        return Err(ResolveError::Unsupported("invalid alignment"));
    }
    value
        .checked_add(alignment - 1)
        .map(|sum| sum & !(alignment - 1))
        .ok_or(ResolveError::Unsupported("record size overflow"))
}

fn requested_alignment(
    resolver: &mut TypeResolver,
    attributes: &[Attribute],
) -> Result<Option<u64>, ResolveError> {
    let mut requested: Option<u64> = None;
    for attribute in attributes {
        let value = match attribute {
            Attribute::Aligned(expr) | Attribute::AlignAs(AlignAsOperand::Expr(expr)) => {
                u64::try_from(crate::const_expr::Parser::evaluate_ast(expr)?)
                    .map_err(|_| ResolveError::Unsupported("invalid alignment"))?
            }
            Attribute::AlignAs(AlignAsOperand::Type { ty }) => {
                let resolved = resolver.resolve(&ty.specifiers, &ty.declarator)?;
                u64::from(
                    resolver
                        .storage(
                            resolved
                                .ty
                                .ok_or(ResolveError::Unsupported("void alignment type"))?,
                        )?
                        .alignment_bytes,
                )
            }
            _ => continue,
        };
        if value != 0 {
            align_up(0, value)?;
            requested = Some(requested.unwrap_or(1).max(value));
        }
    }
    Ok(requested)
}

fn field_request(
    resolver: &mut TypeResolver,
    declaration: &[Attribute],
    field: &[Attribute],
) -> Result<(bool, Option<u64>), ResolveError> {
    let packed = declaration
        .iter()
        .chain(field)
        .any(|attribute| matches!(attribute, Attribute::Packed));
    let first = requested_alignment(resolver, declaration)?;
    let second = requested_alignment(resolver, field)?;
    Ok((packed, first.into_iter().chain(second).max()))
}

fn substitute_enumerators(
    expression: &crate::ast::Expr,
    values: &HashMap<String, i64>,
) -> crate::ast::Expr {
    use crate::ast::ExprKind;
    let mut result = expression.clone();
    result.value = match &expression.value {
        ExprKind::Identifier(name) if values.contains_key(name) => {
            let value = values[name];
            let literal = ExprKind::IntegerLiteral(crate::const_expr::IntegerLiteral::decimal(
                value.saturating_abs(),
            ));
            if value < 0 {
                ExprKind::Unary {
                    op: crate::const_expr::UnaryOp::Minus,
                    operand: Box::new(expression.as_ref().clone().with_value(literal)),
                }
            } else {
                literal
            }
        }
        ExprKind::Paren(inner) => ExprKind::Paren(substitute_enumerators(inner, values)),
        ExprKind::Unary { op, operand } => ExprKind::Unary {
            op: *op,
            operand: substitute_enumerators(operand, values),
        },
        ExprKind::Binary { op, left, right } => ExprKind::Binary {
            op: *op,
            left: substitute_enumerators(left, values),
            right: substitute_enumerators(right, values),
        },
        ExprKind::Conditional {
            condition,
            then_value,
            else_value,
        } => ExprKind::Conditional {
            condition: substitute_enumerators(condition, values),
            then_value: then_value
                .as_ref()
                .map(|value| substitute_enumerators(value, values)),
            else_value: substitute_enumerators(else_value, values),
        },
        ExprKind::Cast { ty, value } => ExprKind::Cast {
            ty: ty.clone(),
            value: substitute_enumerators(value, values),
        },
        _ => expression.value.clone(),
    };
    result
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
    let mut resolver = TypeResolver::with_tags(target, unit);
    let mut next_binding = 0u32;
    for declaration in &unit.decls {
        match &declaration.value {
            DeclKind::Comment(_) | DeclKind::Pragma(_) | DeclKind::StaticAssert(_) => continue,
            DeclKind::Declaration(item) if item.declarators.is_empty() => {
                let start = resolver.definitions.len();
                resolver.resolve(&item.specifiers, &Declarator::Abstract)?;
                for definition in &resolver.definitions[start..] {
                    module
                        .types
                        .push(declaration.clone().with_value(definition.clone()));
                }
            }
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
                let parameter_types = parameter_types(&parameters);
                let abi = super::abi::AbiClassifier::new(&resolver, &module.target).from_parts(
                    return_type.ty.as_ref(),
                    &parameter_types,
                    matches!(
                        &parameters,
                        crate::ir::Parameters::Prototype { variadic: true, .. }
                    ),
                    parameter_types.len(),
                )?;
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
                        abi,
                        linkage: if function.specifiers.storage == StorageClass::Static {
                            Linkage::Internal
                        } else {
                            Linkage::External
                        },
                        body: None,
                        fallthrough: None,
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
                    let parameter_types = parameter_types(&parameters);
                    let abi = super::abi::AbiClassifier::new(&resolver, &module.target)
                        .from_parts(
                            return_type.ty.as_ref(),
                            &parameter_types,
                            matches!(
                                &parameters,
                                crate::ir::Parameters::Prototype { variadic: true, .. }
                            ),
                            parameter_types.len(),
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
                            abi,
                            linkage: if item.specifiers.storage == StorageClass::Static {
                                Linkage::Internal
                            } else {
                                Linkage::External
                            },
                            body: None,
                            fallthrough: None,
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
            let span = resolver.tag_span(definition.id, unit);
            if let Some(span) = span {
                module
                    .types
                    .push(span.clone().with_value(definition.clone()));
            } else if let Some(declaration) = unit.decls.first() {
                module
                    .types
                    .push(declaration.clone().with_value(definition.clone()));
            }
        }
    }
    Ok(module)
}

fn parameter_types(parameters: &crate::ir::Parameters) -> Vec<Type> {
    match parameters {
        crate::ir::Parameters::Prototype { fixed, .. } => {
            fixed.iter().map(|parameter| parameter.ty.clone()).collect()
        }
        crate::ir::Parameters::Unprototyped => Vec::new(),
    }
}

fn resolve_parameters(
    resolver: &mut TypeResolver,
    signature: &ParameterList,
    module: &mut crate::ir::Module,
    next_binding: &mut u32,
) -> Result<crate::ir::Parameters, ResolveError> {
    use crate::ir::{BindingId, Parameter, Parameters};
    if matches!(signature, ParameterList::Empty) && !resolver.features.empty_parens_are_prototype {
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

pub(super) fn character_constant(
    literal: &crate::const_expr::CharLiteral,
    target: &TargetInfo,
) -> Result<(Type, Number), ResolveError> {
    let value = literal.value(target)?;
    let ty = if literal.encoding == Encoding::Plain {
        Type::integer(target.int_width, true)
    } else {
        Type::integer(
            literal.encoding.unit_width(target.wchar_width),
            literal.char_type_is_signed(target),
        )
    };
    let number = match u64::try_from(value) {
        Ok(value) => Number::Integer(BigUint::from(value)),
        Err(_) => Number::SignedInteger(BigInt::from(value)),
    };
    Ok((ty, number))
}

pub(super) fn string_literal_type(
    literal: &crate::const_expr::StringLiteral,
    target: &TargetInfo,
    features: StandardFeatures,
) -> Type {
    let width = literal.unit_width(target.wchar_width);
    let signed = match literal.encoding {
        Encoding::Plain => target.char_signed,
        Encoding::Wide => target.wchar_signed,
        Encoding::Utf8 => !features.u8_literals_are_unsigned && target.char_signed,
        Encoding::Utf16 | Encoding::Utf32 => false,
    };
    Type::Array {
        element: Box::new(Type::integer(width, signed)),
        length: Some(literal.execution_units(target.wchar_width).len() as u64 + 1),
    }
}

fn builtin_result_type(name: &str) -> Option<Type> {
    matches!(
        name,
        "__builtin_add_overflow" | "__builtin_sub_overflow" | "__builtin_mul_overflow"
    )
    .then_some(Type::Bool)
}
