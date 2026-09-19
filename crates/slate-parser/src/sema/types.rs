use std::collections::HashMap;

use crate::ast::{
    AlignAsOperand, ArraySize, Attribute, DeclarationSpecifiers, Declarator, EnumItemKind,
    FieldItemKind, FloatingType, IntegerRank, IntegerType, ParameterList, Qualifiers, TagBody,
    TagDefinition, TagId, TagKind, TagSpecifier, TranslationUnit, TypeName, TypeOfOperand,
    TypeSpecifier,
};
use crate::const_expr::Encoding;
use crate::ir::{
    Access, BindingId, BitFieldUnit, Enumerator, Field, FloatType, Number, NumericType, RecordKind,
    RecordLayout, Type, TypeDefinition, TypeDefinitionKind, TypeId, Value, ValueKind,
    VariableExtent,
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
    pub derived_from: Option<Box<CTypeMetadata>>,
}

impl CTypeMetadata {
    fn plain(spelling: String) -> Self {
        Self {
            canonical: spelling.clone(),
            spelling,
            typedef_chain: Vec::new(),
            qualifiers: Qualifiers::default(),
            derived_from: None,
        }
    }

    pub(super) fn qualified(mut self, qualifiers: Qualifiers, ty: Option<&Type>) -> Self {
        let words = qualifier_spelling(Qualifiers {
            is_const: qualifiers.is_const && !self.qualifiers.is_const,
            is_volatile: qualifiers.is_volatile && !self.qualifiers.is_volatile,
            is_restrict: qualifiers.is_restrict && !self.qualifiers.is_restrict,
            is_atomic: qualifiers.is_atomic && !self.qualifiers.is_atomic,
        });
        if words.is_empty() {
            return self;
        }
        let pointer = matches!(ty, Some(Type::Pointer { .. }));
        for spelling in [&mut self.spelling, &mut self.canonical] {
            match top_pointer_star(spelling).filter(|_| pointer) {
                Some(star) => {
                    let words = if star + 1 == spelling.len() {
                        words.trim_end()
                    } else {
                        &words
                    };
                    spelling.insert_str(star + 1, words);
                }
                None => spelling.insert_str(0, &words),
            }
        }
        self.qualifiers = merge_qualifiers(self.qualifiers, qualifiers);
        self
    }

    pub(super) fn unqualified(&self, ty: Option<&Type>) -> Self {
        let pointer = matches!(ty, Some(Type::Pointer { .. }));
        let derived_from = match ty {
            Some(Type::Array { element, .. } | Type::VariableArray { element, .. }) => self
                .derived_from
                .as_ref()
                .map(|element_c| Box::new(element_c.unqualified(Some(element)))),
            _ => self.derived_from.clone(),
        };
        if self.qualifiers == Qualifiers::default() && derived_from == self.derived_from {
            return self.clone();
        }
        let canonical = strip_qualifiers(&self.canonical, pointer);
        let spelling = strip_qualifiers(&self.spelling, pointer);
        let sugared = spelling != self.spelling;
        Self {
            spelling: if sugared { spelling } else { canonical.clone() },
            canonical,
            typedef_chain: if sugared {
                self.typedef_chain.clone()
            } else {
                Vec::new()
            },
            qualifiers: Qualifiers::default(),
            derived_from,
        }
    }

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
    pub(super) extents: HashMap<crate::ast::NodeId, BindingId>,
    pub(super) references: HashMap<crate::ast::NodeId, BindingId>,
    pub(super) bindings: HashMap<BindingId, Type>,
    pub(super) access: HashMap<BindingId, Access>,
    pub(super) typeof_operands: HashMap<crate::ast::NodeId, ResolvedType>,
    pub(super) field_c: HashMap<TypeId, Vec<CTypeMetadata>>,
    prototype_scope: bool,
}

pub(super) enum Ordinary {
    Declared,
    Alias(ResolvedType),
    Constant(Value),
    Object(Type, Access),
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
            extents: HashMap::new(),
            references: HashMap::new(),
            bindings: HashMap::new(),
            access: HashMap::new(),
            typeof_operands: HashMap::new(),
            field_c: HashMap::new(),
            prototype_scope: false,
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

    pub(super) fn tag_definition(&self, id: TagId) -> Option<TagDefinition> {
        self.tags
            .iter()
            .find(|tag| tag.value.id == id)
            .map(|tag| tag.value.clone())
    }

    fn vector_count(
        &mut self,
        expression: &crate::ast::Expr,
        invalid: &'static str,
    ) -> Result<u64, ResolveError> {
        let value = self.constant_integer(expression)?;
        match u64::try_from(value) {
            Ok(count) if count != 0 => Ok(count),
            _ => Err(ResolveError::Invalid(invalid)),
        }
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
                let (ty, access) = self.assertion_operand_type(operand)?;
                let layout = self.qualified_storage(ty, access.atomic)?;
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
            ExprKind::TypesCompatible { left_ty, right_ty } => (
                Type::integer(self.target.int_width, true),
                ValueKind::Constant(Number::SignedInteger(
                    u8::from(self.types_compatible(left_ty, right_ty)?.0).into(),
                )),
            ),
            ExprKind::Call { callee, arguments }
                if super::expression::constant_p_operand(callee, arguments).is_some() =>
            {
                let operand = super::expression::constant_p_operand(callee, arguments)
                    .ok_or(ResolveError::Unsupported("__builtin_constant_p"))?;
                (
                    Type::integer(self.target.int_width, true),
                    ValueKind::Constant(Number::SignedInteger(
                        u8::from(self.is_constant(operand)).into(),
                    )),
                )
            }
            ExprKind::SizeOfType { ty } | ExprKind::AlignOf { ty } => {
                let resolved = self.resolve(&ty.specifiers, &ty.declarator)?;
                let atomic = resolved.c.qualifiers.is_atomic;
                let ty = resolved
                    .ty
                    .ok_or(ResolveError::Unsupported("void layout"))?;
                let layout = self.qualified_storage(ty, atomic)?;
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

    fn object(&self, e: &crate::ast::Expr) -> Option<(Type, Access)> {
        let id = self.references.get(&e.id)?;
        let ty = self.bindings.get(id)?.clone();
        Some((ty, self.access.get(id).copied().unwrap_or_default()))
    }

    pub(super) fn is_constant(&mut self, e: &crate::ast::Expr) -> bool {
        self.constant_value(e).is_ok_and(|value| is_folded(&value))
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
            GenericControl::Expr(expr) => self.assertion_operand_type(expr)?.0,
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
                access: Access::default(),
            },
            ty @ Type::Function { .. } => Type::Pointer {
                pointee: Box::new(ty),
                is_const: false,
                access: Access::default(),
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
    ) -> Result<(Type, Access), ResolveError> {
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
            .ok_or(ResolveError::Unsupported("nonconstant call expression"))
            .map(|ty| (ty, Access::default())),
            ExprKind::Identifier(_) if self.object(e).is_some() => self
                .object(e)
                .ok_or(ResolveError::Unsupported("untyped binding")),
            ExprKind::Identifier(name) => match self.lookup(name) {
                Some(Ordinary::Object(ty, access)) => Ok((ty.clone(), *access)),
                Some(Ordinary::Constant(value)) => Ok((value.ty.clone(), Access::default())),
                _ => Err(ResolveError::Unsupported(
                    "unknown or unsupported sizeof operand type",
                )),
            },
            ExprKind::StringLiteral(literal) => Ok((
                string_literal_type(literal, &self.target, self.features),
                Access::default(),
            )),
            ExprKind::CompoundLiteral { ty, initializer } => {
                let resolved = self.resolve(&ty.specifiers, &ty.declarator)?;
                let literal_access = access(resolved.c.qualifiers);
                let declared = resolved
                    .ty
                    .ok_or(ResolveError::Unsupported("void compound literal"))?;
                match declared {
                    Type::Array {
                        element,
                        length: None,
                    } => {
                        let length = self.inferred_array_length(&element, initializer)?;
                        Ok((
                            Type::Array {
                                element,
                                length: Some(length),
                            },
                            literal_access,
                        ))
                    }
                    declared => Ok((declared, literal_access)),
                }
            }
            ExprKind::Unary {
                op: crate::const_expr::UnaryOp::Deref,
                operand,
            } => match self.assertion_operand_type(operand)? {
                (
                    Type::Pointer {
                        pointee, access, ..
                    },
                    _,
                ) => Ok((*pointee, access)),
                _ => Err(ResolveError::Unsupported(
                    "sizeof dereference of nonpointer",
                )),
            },
            ExprKind::Index { base, .. } => match self.assertion_operand_type(base)? {
                (Type::Array { element, .. }, access) => Ok((*element, access)),
                (
                    Type::Pointer {
                        pointee, access, ..
                    },
                    _,
                ) => Ok((*pointee, access)),
                _ => Err(ResolveError::Unsupported("sizeof index of nonarray")),
            },
            _ => self
                .constant_value(e)
                .map(|value| (value.ty, Access::default())),
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
        let mut resolved = self.base(&specifiers.ty)?;
        let prefix = qualifier_spelling(specifiers.qualifiers);
        resolved.c.spelling.insert_str(0, &prefix);
        resolved.c.canonical.insert_str(0, &prefix);
        resolved.c.qualifiers = merge_qualifiers(resolved.c.qualifiers, specifiers.qualifiers);
        self.derive(declarator, &mut resolved)?;
        if specifiers.is_constexpr {
            resolved.c = resolved.c.qualified(
                Qualifiers {
                    is_const: true,
                    ..Qualifiers::default()
                },
                resolved.ty.as_ref(),
            );
        }
        Ok(resolved)
    }

    fn base(&mut self, specifier: &TypeSpecifier) -> Result<ResolvedType, ResolveError> {
        let scalar = match specifier {
            TypeSpecifier::Void => {
                return Ok(ResolvedType {
                    ty: None,
                    c: CTypeMetadata::plain("void".into()),
                });
            }
            TypeSpecifier::Atomic(inner) => {
                let inner = self.resolve(&inner.specifiers, &inner.declarator)?;
                return Ok(ResolvedType {
                    ty: inner.ty,
                    c: CTypeMetadata {
                        spelling: format!("_Atomic({})", inner.c.spelling),
                        canonical: format!("_Atomic({})", inner.c.canonical),
                        typedef_chain: Vec::new(),
                        qualifiers: Qualifiers {
                            is_atomic: true,
                            ..Qualifiers::default()
                        },
                        derived_from: inner.c.derived_from,
                    },
                });
            }
            TypeSpecifier::TypeOf(operand) | TypeSpecifier::TypeOfUnqual(operand) => {
                let unqualified = matches!(specifier, TypeSpecifier::TypeOfUnqual(_));
                let resolved = self.typeof_operand(operand)?;
                let spelling = match operand {
                    TypeOfOperand::Expression(expr) => expr.value.to_string(),
                    TypeOfOperand::Type(_) => resolved.c.spelling.clone(),
                };
                let mut c = if unqualified {
                    resolved.c.unqualified(resolved.ty.as_ref())
                } else {
                    resolved.c
                };
                let keyword = if unqualified {
                    "typeof_unqual"
                } else {
                    "typeof"
                };
                c.spelling = format!("{keyword}({spelling})");
                return Ok(ResolvedType { ty: resolved.ty, c });
            }
            TypeSpecifier::Named(name) => {
                let Some(Ordinary::Alias(alias)) = self.lookup(name) else {
                    return Err(ResolveError::Unsupported("unknown typedef"));
                };
                let mut resolved = alias.clone();
                resolved.c.spelling = name.clone();
                resolved.c.typedef_chain.insert(0, name.clone());
                return Ok(resolved);
            }
            TypeSpecifier::Tag(TagSpecifier::Definition(id)) => {
                let tag = self
                    .tags
                    .iter()
                    .find(|tag| tag.value.id == *id)
                    .cloned()
                    .ok_or(ResolveError::Unsupported("unknown tag definition"))?;
                let ty = Type::Defined(self.define_tag(&tag.value)?);
                return Ok(ResolvedType {
                    ty: Some(ty),
                    c: CTypeMetadata::plain(tag_spelling(tag.kind, tag.name.as_deref())),
                });
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
                return Ok(ResolvedType {
                    ty: Some(Type::Defined(id)),
                    c: CTypeMetadata::plain(tag_spelling(*kind, Some(name))),
                });
            }
            TypeSpecifier::Bool => (Type::Bool, "_Bool".into()),
            TypeSpecifier::Complex(inner) => {
                let ResolvedType { ty: component, c } = self.base(inner)?;
                let Some(Type::Numeric(component)) = component else {
                    return Err(ResolveError::Unsupported("complex component type"));
                };
                return Ok(ResolvedType {
                    ty: Some(Type::Complex(component)),
                    c: CTypeMetadata {
                        spelling: format!("_Complex {}", c.spelling),
                        canonical: format!("_Complex {}", c.canonical),
                        ..c
                    },
                });
            }
            TypeSpecifier::Imaginary(inner) => {
                let ResolvedType { ty: component, c } = self.base(inner)?;
                let Some(Type::Numeric(NumericType::Float(format))) = component else {
                    return Err(ResolveError::Invalid(
                        "imaginary component must be a real floating type",
                    ));
                };
                if format.is_decimal() {
                    return Err(ResolveError::Invalid(
                        "imaginary component must be a real floating type",
                    ));
                }
                return Ok(ResolvedType {
                    ty: Some(Type::Imaginary(format)),
                    c: CTypeMetadata {
                        spelling: format!("_Imaginary {}", c.spelling),
                        canonical: format!("_Imaginary {}", c.canonical),
                        ..c
                    },
                });
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
            TypeSpecifier::Vector(vector) => {
                let ResolvedType { ty: element, c } = self.base(&vector.element)?;
                let element = match element {
                    Some(Type::Numeric(
                        element @ NumericType::Integer {
                            bit_precise: false, ..
                        },
                    )) => element,
                    Some(Type::Numeric(element @ NumericType::Float(format)))
                        if !format.is_decimal() =>
                    {
                        element
                    }
                    _ => {
                        return Err(ResolveError::Invalid(
                            "vector element must be an integer or real floating type",
                        ));
                    }
                };
                let element_bytes = self.storage(Type::Numeric(element))?.size_bytes;
                let (requested, lanes) = match &vector.size {
                    crate::ast::VectorSize::Bytes(expression) => {
                        let bytes = self
                            .vector_count(expression, "vector_size must be a positive constant")?;
                        if bytes % element_bytes != 0 {
                            return Err(ResolveError::Invalid(
                                "vector_size must be a multiple of the element size",
                            ));
                        }
                        (bytes, bytes / element_bytes)
                    }
                    crate::ast::VectorSize::Lanes(expression) => {
                        let lanes = self.vector_count(
                            expression,
                            "ext_vector_type must be a positive constant",
                        )?;
                        (lanes * element_bytes, lanes)
                    }
                };
                let lanes = u32::try_from(lanes)
                    .map_err(|_| ResolveError::Invalid("vector lane count is too large"))?;
                return Ok(ResolvedType {
                    ty: Some(Type::Vector { element, lanes }),
                    c: CTypeMetadata::plain(vector_spelling(&c.spelling, requested)),
                });
            }
            TypeSpecifier::TargetBuiltin(name) if name == "__builtin_va_list" => {
                (Type::VaList, "__builtin_va_list".into())
            }
            _ => return Err(ResolveError::Unsupported("type specifier")),
        };
        Ok(ResolvedType {
            ty: Some(scalar.0),
            c: CTypeMetadata::plain(scalar.1),
        })
    }

    fn typeof_operand(&mut self, operand: &TypeOfOperand) -> Result<ResolvedType, ResolveError> {
        match operand {
            TypeOfOperand::Type(ty) => self.resolve(&ty.specifiers, &ty.declarator),
            TypeOfOperand::Expression(expr) => {
                if let Some(resolved) = self.typeof_operands.get(&expr.id) {
                    return Ok(resolved.clone());
                }
                let (ty, _) = self.assertion_operand_type(expr)?;
                Ok(ResolvedType {
                    c: self.c_type(Some(&ty))?,
                    ty: Some(ty),
                })
            }
        }
    }

    pub(super) fn c_type(&self, ty: Option<&Type>) -> Result<CTypeMetadata, ResolveError> {
        let Some(ty) = ty else {
            return Ok(CTypeMetadata::plain("void".into()));
        };
        Ok(match ty {
            Type::Void => CTypeMetadata::plain("void".into()),
            Type::Bool => CTypeMetadata::plain("_Bool".into()),
            Type::VaList => CTypeMetadata::plain("__builtin_va_list".into()),
            Type::Numeric(numeric) => CTypeMetadata::plain(self.numeric_spelling(*numeric)?),
            Type::Complex(numeric) => {
                CTypeMetadata::plain(format!("_Complex {}", self.numeric_spelling(*numeric)?))
            }
            Type::Imaginary(format) => CTypeMetadata::plain(format!(
                "_Imaginary {}",
                self.numeric_spelling(NumericType::Float(*format))?
            )),
            Type::Vector { element, lanes } => CTypeMetadata::plain(vector_spelling(
                &self.numeric_spelling(*element)?,
                self.storage(Type::Numeric(*element))?.size_bytes * u64::from(*lanes),
            )),
            Type::Defined(id) => {
                let definition = self
                    .definitions
                    .get(id.0 as usize)
                    .ok_or(ResolveError::Unsupported("unknown type definition"))?;
                let name = definition.name.as_deref();
                match &definition.kind {
                    TypeDefinitionKind::Alias(inner) => {
                        let name = name.ok_or(ResolveError::Unsupported("unnamed typedef"))?;
                        let mut c = self.c_type(Some(inner))?;
                        c.spelling = name.to_owned();
                        c.typedef_chain.insert(0, name.to_owned());
                        c
                    }
                    TypeDefinitionKind::Record { kind, .. } => {
                        let kind = match kind {
                            RecordKind::Struct => TagKind::Struct,
                            RecordKind::Union => TagKind::Union,
                        };
                        CTypeMetadata::plain(tag_spelling(kind, name))
                    }
                    TypeDefinitionKind::Enum { .. } => {
                        CTypeMetadata::plain(tag_spelling(TagKind::Enum, name))
                    }
                }
            }
            Type::Pointer {
                pointee,
                is_const,
                access,
            } => {
                let qualifiers = Qualifiers {
                    is_const: *is_const,
                    is_volatile: access.volatile,
                    is_atomic: access.atomic,
                    is_restrict: false,
                };
                let mut resolved = ResolvedType {
                    c: self
                        .c_type(Some(pointee))?
                        .qualified(qualifiers, Some(pointee)),
                    ty: Some((**pointee).clone()),
                };
                Self::apply_pointer(Qualifiers::default(), &mut resolved);
                resolved.c
            }
            Type::Array { .. } | Type::VariableArray { .. } => {
                let mut extents = Vec::new();
                let mut core = ty;
                loop {
                    match core {
                        Type::Array { element, length } => {
                            extents.push(length.map_or_else(|| "[]".into(), |n| format!("[{n}]")));
                            core = element;
                        }
                        Type::VariableArray { element, .. } => {
                            extents.push("[*]".to_owned());
                            core = element;
                        }
                        _ => break,
                    }
                }
                let base = self.c_type(Some(core))?;
                let mut c = base.clone();
                let mut suffix = String::new();
                for extent in extents.iter().rev() {
                    suffix.insert_str(0, extent);
                    let element_c = std::mem::replace(
                        &mut c,
                        CTypeMetadata {
                            spelling: format!("{}{suffix}", base.spelling),
                            canonical: format!("{}{suffix}", base.canonical),
                            typedef_chain: base.typedef_chain.clone(),
                            qualifiers: base.qualifiers,
                            derived_from: None,
                        },
                    );
                    c.derived_from = Some(Box::new(element_c));
                }
                c
            }
            Type::Function {
                return_type,
                parameters,
                variadic,
                prototyped,
            } => {
                let return_c = self.c_type(return_type.as_deref())?;
                let mut spellings = parameters
                    .iter()
                    .map(|parameter| self.c_type(Some(parameter)).map(|c| c.spelling))
                    .collect::<Result<Vec<_>, _>>()?;
                let suffix = if !prototyped {
                    "()".to_owned()
                } else if spellings.is_empty() && !variadic {
                    "(void)".to_owned()
                } else {
                    if *variadic {
                        spellings.push("...".into());
                    }
                    format!("({})", spellings.join(", "))
                };
                CTypeMetadata {
                    spelling: format!("{}{suffix}", return_c.spelling),
                    canonical: format!("{}{suffix}", return_c.canonical),
                    typedef_chain: return_c.typedef_chain.clone(),
                    qualifiers: return_c.qualifiers,
                    derived_from: Some(Box::new(return_c)),
                }
            }
        })
    }

    fn numeric_spelling(&self, numeric: NumericType) -> Result<String, ResolveError> {
        let (width, signed) = match numeric {
            NumericType::Integer {
                width,
                signed,
                bit_precise: true,
            } => {
                let sign = if signed { "" } else { "unsigned " };
                return Ok(format!("{sign}_BitInt({width})"));
            }
            NumericType::Integer { width, signed, .. } => (width, signed),
            NumericType::Float(format) => {
                return Ok(match format {
                    FloatType::F16 => "_Float16",
                    FloatType::F32 => "float",
                    FloatType::F64 => "double",
                    FloatType::F80 => "long double",
                    FloatType::F128 if self.target.long_double == LongDoubleFormat::Binary128 => {
                        "long double"
                    }
                    FloatType::F128 => "__float128",
                    FloatType::D32 => "_Decimal32",
                    FloatType::D64 => "_Decimal64",
                    FloatType::D128 => "_Decimal128",
                }
                .into());
            }
        };
        if width == 8 {
            return Ok(match signed {
                _ if signed == self.target.char_signed => "char",
                true => "signed char",
                false => "unsigned char",
            }
            .into());
        }
        let name = [
            (self.target.int_width, "int"),
            (self.target.long_width, "long"),
            (self.target.long_long_width, "long long"),
            (self.target.short_width, "short"),
            (128, "__int128"),
        ]
        .into_iter()
        .find_map(|(candidate, name)| (candidate == width).then_some(name))
        .ok_or(ResolveError::Unsupported("C spelling of integer width"))?;
        Ok(if signed {
            name.into()
        } else {
            format!("unsigned {name}")
        })
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
                Self::apply_pointer(*qualifiers, resolved);
                self.derive(inner, resolved)
            }
            Declarator::Array { .. } => {
                enum Extent {
                    Unspecified,
                    Fixed(u64),
                    Variable(VariableExtent),
                }
                let mut lengths = Vec::new();
                let mut core = declarator;
                while let Declarator::Array { inner, size, .. } = core {
                    lengths.push(match size {
                        ArraySize::Unspecified => Extent::Unspecified,
                        ArraySize::Star => Extent::Variable(VariableExtent::Unspecified),
                        ArraySize::Expression(expr) => match self.extents.get(&expr.id) {
                            Some(extent) => Extent::Variable(VariableExtent::Captured(*extent)),
                            None => match self.constant_integer(expr) {
                                Ok(length) => {
                                    Extent::Fixed(u64::try_from(length).map_err(|_| {
                                        ResolveError::Unsupported("invalid array length")
                                    })?)
                                }
                                Err(_) if self.prototype_scope => {
                                    Extent::Variable(VariableExtent::Unspecified)
                                }
                                Err(error) => return Err(error),
                            },
                        },
                    });
                    core = inner;
                }
                let core = Self::apply_pointers(core, resolved);
                let base = resolved.c.clone();
                let mut suffix = String::new();
                for length in lengths {
                    let element = Box::new(
                        resolved
                            .ty
                            .take()
                            .ok_or(ResolveError::Unsupported("void array element"))?,
                    );
                    suffix.insert_str(
                        0,
                        &match length {
                            Extent::Unspecified => "[]".to_owned(),
                            Extent::Fixed(length) => format!("[{length}]"),
                            Extent::Variable(_) => "[*]".to_owned(),
                        },
                    );
                    let element_c = std::mem::replace(
                        &mut resolved.c,
                        CTypeMetadata {
                            spelling: format!("{}{suffix}", base.spelling),
                            canonical: format!("{}{suffix}", base.canonical),
                            typedef_chain: base.typedef_chain.clone(),
                            qualifiers: base.qualifiers,
                            derived_from: None,
                        },
                    );
                    resolved.c.derived_from = Some(Box::new(element_c));
                    resolved.ty = Some(match length {
                        Extent::Unspecified => Type::Array {
                            element,
                            length: None,
                        },
                        Extent::Fixed(length) => Type::Array {
                            element,
                            length: Some(length),
                        },
                        Extent::Variable(extent) => Type::VariableArray { element, extent },
                    });
                }
                self.derive(core, resolved)
            }
            Declarator::Function { inner, parameters } => {
                let mut types = Vec::new();
                let mut c_parameters = Vec::new();
                for parameter in parameters.parameters() {
                    let parameter_type =
                        self.resolve_parameter(&parameter.specifiers, &parameter.declarator)?;
                    let mut ty = parameter_type
                        .ty
                        .ok_or(ResolveError::Unsupported("void parameter"))?;
                    ty = match ty {
                        Type::Array { element, .. } | Type::VariableArray { element, .. } => {
                            Type::Pointer {
                                pointee: element,
                                is_const: false,
                                access: access(parameter_type.c.qualifiers),
                            }
                        }
                        function @ Type::Function { .. } => Type::Pointer {
                            pointee: Box::new(function),
                            is_const: false,
                            access: Access::default(),
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
                let core = Self::apply_pointers(inner, resolved);
                resolved.ty = Some(Type::Function {
                    return_type: resolved.ty.take().map(Box::new),
                    parameters: types,
                    variadic,
                    prototyped,
                });
                let return_c = resolved.c.clone();
                resolved.c.spelling.push_str(&suffix);
                resolved.c.canonical.push_str(&suffix);
                resolved.c.derived_from = Some(Box::new(return_c));
                self.derive(core, resolved)
            }
        }
    }

    pub(super) fn resolve_parameter(
        &mut self,
        specifiers: &DeclarationSpecifiers,
        declarator: &Declarator,
    ) -> Result<ResolvedType, ResolveError> {
        let enclosing = std::mem::replace(&mut self.prototype_scope, true);
        let resolved = self.resolve(specifiers, declarator);
        self.prototype_scope = enclosing;
        resolved
    }

    pub(super) fn apply_pointer(qualifiers: Qualifiers, resolved: &mut ResolvedType) {
        let pointee = resolved.ty.take().unwrap_or(Type::Void);
        let grouped = matches!(
            pointee,
            Type::Function { .. } | Type::Array { .. } | Type::VariableArray { .. }
        );
        resolved.ty = Some(Type::Pointer {
            pointee: Box::new(pointee),
            is_const: resolved.c.qualifiers.is_const,
            access: access(resolved.c.qualifiers),
        });
        let pointee_c = resolved.c.clone();
        resolved.c.spelling = pointer_spelling(&resolved.c.spelling, qualifiers, grouped);
        resolved.c.canonical = pointer_spelling(&resolved.c.canonical, qualifiers, grouped);
        resolved.c.qualifiers = qualifiers;
        resolved.c.derived_from = Some(Box::new(pointee_c));
    }

    pub(super) fn parameter_c(resolved: &ResolvedType, array: Qualifiers) -> CTypeMetadata {
        let mut adjusted = match (&resolved.ty, &resolved.c.derived_from) {
            (
                Some(Type::Array { element, .. } | Type::VariableArray { element, .. }),
                Some(element_c),
            ) => ResolvedType {
                ty: Some((**element).clone()),
                c: (**element_c).clone(),
            },
            (Some(Type::Function { .. }), _) => resolved.clone(),
            _ => return resolved.c.clone(),
        };
        let qualifiers = if matches!(resolved.ty, Some(Type::Function { .. })) {
            Qualifiers::default()
        } else {
            array
        };
        Self::apply_pointer(qualifiers, &mut adjusted);
        adjusted.c
    }

    // pointers under an array or function node bind to its element or return type
    fn apply_pointers<'d>(mut core: &'d Declarator, resolved: &mut ResolvedType) -> &'d Declarator {
        while let Declarator::Pointer {
            inner, qualifiers, ..
        } = core
        {
            Self::apply_pointer(*qualifiers, resolved);
            core = inner;
        }
        core
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
                let mut field_c = Vec::new();
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
                            .clone()
                            .ok_or(ResolveError::Unsupported("void record field"))?;
                        fields.push(item.clone().with_value(Field {
                            name: None,
                            ty,
                            access: access(resolved.c.qualifiers),
                            bit_width: None,
                        }));
                        field_c.push(resolved.c);
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
                            .clone()
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
                            access: access(resolved.c.qualifiers),
                            bit_width,
                        }));
                        field_c.push(resolved.c);
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
                self.field_c.insert(id, field_c);
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
                let is_fixed = fixed_underlying != Type::Void;
                let fits_int = values
                    .iter()
                    .all(|(_, _, value)| i32::try_from(*value).is_ok());
                let underlying = if is_fixed {
                    fixed_underlying
                } else if values.iter().any(|(_, _, value)| *value < 0) {
                    Type::integer(
                        if fits_int {
                            self.target.int_width
                        } else {
                            self.target.long_width
                        },
                        true,
                    )
                } else if values
                    .iter()
                    .all(|(_, _, value)| u32::try_from(*value).is_ok())
                {
                    Type::integer(self.target.int_width, false)
                } else {
                    Type::integer(self.target.long_width, false)
                };
                let enumerator_type = if !is_fixed && fits_int {
                    Type::integer(self.target.int_width, true)
                } else if self.features.enumerators_have_enum_type {
                    Type::Defined(id)
                } else {
                    underlying.clone()
                };
                let mut entries = Vec::new();
                for (item, enumerator, value) in values {
                    let value = Value {
                        ty: enumerator_type.clone(),
                        node: item.clone().with_value(ValueKind::Constant(if value < 0 {
                            Number::SignedInteger(BigInt::from(value))
                        } else {
                            Number::Integer(BigUint::from(value as u64))
                        })),
                    };
                    self.declare(&enumerator.name, Ordinary::Constant(value.clone()));
                    entries.push(item.clone().with_value(Enumerator {
                        id: BindingId(entries.len() as u32),
                        name: enumerator.name.clone(),
                        value,
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

    pub(super) fn types_compatible(
        &mut self,
        left: &TypeName,
        right: &TypeName,
    ) -> Result<(bool, String), ResolveError> {
        let (left_ty, left_spelling) = self.unqualified(left)?;
        let (right_ty, right_spelling) = self.unqualified(right)?;
        let compared = format!("{left_spelling}, {right_spelling}");
        if left_ty == right_ty && left_spelling == right_spelling {
            return Ok((true, compared));
        }
        let compatible = match (&left_ty, &right_ty) {
            (Some(Type::Defined(id)), Some(other @ Type::Numeric(_)))
            | (Some(other @ Type::Numeric(_)), Some(Type::Defined(id))) => matches!(
                &self.definitions[id.0 as usize].kind,
                TypeDefinitionKind::Enum { underlying: Some(underlying), .. } if underlying == other
            ),
            _ => false,
        };
        Ok((compatible, compared))
    }

    fn unqualified(&mut self, name: &TypeName) -> Result<(Option<Type>, String), ResolveError> {
        let mut name = name.clone();
        let qualifiers = match name.declarator.pointer_qualifiers_mut() {
            Some(qualifiers) => qualifiers,
            None => &mut name.specifiers.qualifiers,
        };
        *qualifiers = Qualifiers {
            is_atomic: qualifiers.is_atomic,
            ..Qualifiers::default()
        };
        let resolved = self.resolve(&name.specifiers, &name.declarator)?;
        let mut spelling = resolved.c.canonical.as_str();
        if !matches!(resolved.ty, Some(Type::Pointer { .. })) {
            while let Some(rest) = ["const ", "volatile ", "restrict "]
                .iter()
                .find_map(|word| spelling.strip_prefix(word))
            {
                spelling = rest;
            }
        }
        Ok((resolved.ty, spelling.to_owned()))
    }

    pub(super) fn require_complete(&self, ty: &Type) -> Result<(), ResolveError> {
        match ty {
            Type::VariableArray { element, .. } => self.require_complete(element),
            ty => self.storage(ty.clone()).map(|_| ()),
        }
    }

    pub(super) fn storage(&self, ty: Type) -> Result<StorageLayout, ResolveError> {
        self.qualified_storage(ty, false)
    }

    // _Atomic qualifies the element, never the array, so the flag rides
    // through array layers down to the value type clang would promote
    pub(super) fn qualified_storage(
        &self,
        ty: Type,
        atomic: bool,
    ) -> Result<StorageLayout, ResolveError> {
        let promote = |layout| {
            if atomic {
                self.target.atomic_storage(layout)
            } else {
                layout
            }
        };
        match ty {
            Type::Defined(id) => match &self.definitions[id.0 as usize].kind {
                TypeDefinitionKind::Alias(inner) => self.qualified_storage(inner.clone(), atomic),
                TypeDefinitionKind::Record {
                    layout: Some(layout),
                    ..
                } => Ok(promote(StorageLayout {
                    size_bytes: layout.size,
                    alignment_bytes: u32::try_from(layout.align)
                        .map_err(|_| ResolveError::Unsupported("record alignment overflow"))?,
                })),
                TypeDefinitionKind::Enum {
                    layout: Some(layout),
                    ..
                } => Ok(promote(*layout)),
                TypeDefinitionKind::Enum {
                    underlying: Some(underlying),
                    ..
                } => self.qualified_storage(underlying.clone(), atomic),
                _ => Err(ResolveError::Unsupported("incomplete field type")),
            },
            Type::Pointer { .. } => Ok(promote(self.target.pointer)),
            Type::Array {
                element,
                length: Some(length),
            } => {
                let element = self.qualified_storage(*element, atomic)?;
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
            _ => Ok(promote(self.target.storage_of(ty)?)),
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
        for (position, (field, &(field_packed, field_aligned))) in
            fields.iter().zip(requests).enumerate()
        {
            let storage = match &field.ty {
                Type::Array {
                    element,
                    length: None,
                } if kind == TagKind::Struct && position + 1 == fields.len() => StorageLayout {
                    size_bytes: 0,
                    alignment_bytes: self
                        .qualified_storage((**element).clone(), field.access.atomic)?
                        .alignment_bytes,
                },
                ty => self.qualified_storage(ty.clone(), field.access.atomic)?,
            };
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

fn vector_spelling(element: &str, bytes: u64) -> String {
    format!("{element} __attribute__((vector_size({bytes})))")
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

pub(super) fn compatible(a: &Type, b: &Type) -> bool {
    match (a, b) {
        (
            Type::VariableArray { element: a, .. },
            Type::VariableArray { element: b, .. } | Type::Array { element: b, .. },
        )
        | (Type::Array { element: a, .. }, Type::VariableArray { element: b, .. }) => {
            compatible(a, b)
        }
        (
            Type::Array {
                element: a,
                length: a_length,
            },
            Type::Array {
                element: b,
                length: b_length,
            },
        ) => a_length == b_length && compatible(a, b),
        (
            Type::Pointer {
                pointee: a,
                is_const: a_const,
                access: a_access,
            },
            Type::Pointer {
                pointee: b,
                is_const: b_const,
                access: b_access,
            },
        ) => a_const == b_const && a_access == b_access && compatible(a, b),
        _ => a == b,
    }
}

pub(super) fn access(qualifiers: Qualifiers) -> Access {
    Access {
        volatile: qualifiers.is_volatile,
        atomic: qualifiers.is_atomic,
    }
}

fn merge_qualifiers(a: Qualifiers, b: Qualifiers) -> Qualifiers {
    Qualifiers {
        is_const: a.is_const || b.is_const,
        is_volatile: a.is_volatile || b.is_volatile,
        is_restrict: a.is_restrict || b.is_restrict,
        is_atomic: a.is_atomic || b.is_atomic,
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

fn skip_qualifier_words(mut spelling: &str) -> &str {
    loop {
        let trimmed = spelling.trim_start();
        let rest = ["const", "volatile", "restrict", "_Atomic"]
            .iter()
            .find_map(|word| {
                trimmed
                    .strip_prefix(word)
                    .filter(|rest| rest.is_empty() || rest.starts_with([' ', ')']))
            });
        match rest {
            Some(rest) => spelling = rest,
            None => return trimmed,
        }
    }
}

fn top_pointer_star(spelling: &str) -> Option<usize> {
    let last = spelling.rfind('*')?;
    if skip_qualifier_words(&spelling[last + 1..]).is_empty() {
        return Some(last);
    }
    spelling.find("(*").map(|open| open + 1)
}

fn strip_qualifiers(spelling: &str, pointer: bool) -> String {
    if let Some(inner) = skip_qualifier_words(spelling)
        .strip_prefix("_Atomic(")
        .and_then(|s| s.strip_suffix(')'))
    {
        return inner.to_owned();
    }
    if pointer {
        return match top_pointer_star(spelling) {
            Some(star) => format!(
                "{}{}",
                &spelling[..=star],
                skip_qualifier_words(&spelling[star + 1..])
            ),
            None => spelling.to_owned(),
        };
    }
    skip_qualifier_words(spelling).to_owned()
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
                        symbol: Default::default(),
                        semantics: Default::default(),
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
                            symbol: Default::default(),
                            semantics: Default::default(),
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
    use crate::ir::{ArrayExtent, ArrayParameter, BindingId, Parameter, Parameters};
    if matches!(signature, ParameterList::Empty) && !resolver.features.empty_parens_are_prototype {
        return Ok(Parameters::Unprototyped);
    }
    let mut fixed = Vec::new();
    for parameter in signature.parameters() {
        let start = resolver.definitions.len();
        let resolved = resolver.resolve_parameter(&parameter.specifiers, &parameter.declarator)?;
        let declared_array = parameter.declarator.array_parameter();
        let qualifiers = match declared_array {
            Some(array) => array.qualifiers,
            None => resolved.c.qualifiers,
        };
        let ty = resolved
            .ty
            .ok_or(ResolveError::Unsupported("void parameter"))?;
        let array = match &ty {
            Type::Array { length, .. } => Some(ArrayParameter {
                extent: length.map_or(ArrayExtent::Unspecified, ArrayExtent::Fixed),
                guaranteed: declared_array.is_some_and(|array| array.is_static),
            }),
            Type::VariableArray { extent, .. } => Some(ArrayParameter {
                extent: ArrayExtent::Variable(*extent),
                guaranteed: declared_array.is_some_and(|array| array.is_static),
            }),
            _ => None,
        };
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
            restrict: qualifiers.is_restrict,
            is_const: qualifiers.is_const,
            array,
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

pub(super) fn is_folded(value: &Value) -> bool {
    match value.ty {
        Type::Bool | Type::Numeric(NumericType::Integer { .. }) => {
            super::fold::integer(value).is_some()
        }
        _ => matches!(value.node.value, ValueKind::Constant(_)),
    }
}

fn builtin_result_type(name: &str) -> Option<Type> {
    matches!(
        name,
        "__builtin_add_overflow" | "__builtin_sub_overflow" | "__builtin_mul_overflow"
    )
    .then_some(Type::Bool)
}
