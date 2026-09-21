use std::collections::HashMap;

use crate::ast::{
    AlignAsOperand, ArrayDeclarator, ArraySize, Attribute, DeclarationSpecifiers, Declarator,
    EnumItemKind, FieldItemKind, FixedPointKind, FixedPointRank, FloatingType, IntegerRank,
    IntegerType, ParameterList, TagBody, TagDefinition, TagId, TagKind, TagSpecifier,
    TranslationUnit, TypeName, TypeOfOperand, TypeSpecifier,
};
use crate::compiler_args::CompilerFlavor;
use crate::ir::{
    Access, ArrayExtent, ArrayParameter, BindingId, BitFieldUnit, Enumerator, Field, Number,
    NumericType, RecordKind, RecordLayout, Type, TypeDefinition, TypeDefinitionKind, TypeId, Value,
    ValueKind,
};
use crate::target_info::{StorageLayout, TargetInfo};
use num_bigint::{BigInt, BigUint};

use super::ctype::{
    CTypeKind, CTypeMetadata, CTypes, Extent, FixedKind, FixedRank, FixedType, FloatKind, IntRank,
    QualType, Qualifiers,
};
use super::numeric::ResolveError;
use super::operand::Operand;
use crate::standard_features::StandardFeatures;

pub(super) struct ParameterShape {
    pub adjusted: QualType,
    pub ty: Type,
    pub qualifiers: Qualifiers,
    pub array: Option<ArrayParameter>,
}

pub struct TypeResolver {
    target: TargetInfo,
    pub(super) flavor: CompilerFlavor,
    pub features: StandardFeatures,
    pub ctypes: CTypes,
    tags: Vec<crate::ast::Span<TagDefinition>>,
    tag_ids: HashMap<TagId, TypeId>,
    ordinary: Vec<HashMap<String, Ordinary>>,
    tag_names: Vec<HashMap<(TagKind, String), TypeId>>,
    pub definitions: Vec<TypeDefinition>,
    pub(super) assertion_scope: bool,
    pub(super) extents: HashMap<crate::ast::NodeId, BindingId>,
    pub(super) references: HashMap<crate::ast::NodeId, BindingId>,
    pub(super) entities: super::entity::Entities,
    pub(super) typeof_operands: HashMap<crate::ast::NodeId, QualType>,
    pub(super) enumerators: HashMap<crate::ast::NodeId, Operand>,
    pub(super) record_fields: HashMap<TypeId, Vec<QualType>>,
    pub(super) pragmas: super::pragmas::Pragmas,
    prototype_scope: bool,
}

pub(super) enum Ordinary {
    Declared,
    Alias(QualType),
    Constant(Operand),
    Object(QualType),
}

impl TypeResolver {
    pub(super) fn target_info(&self) -> &TargetInfo {
        &self.target
    }

    pub fn new(target: TargetInfo) -> Self {
        Self {
            target,
            flavor: CompilerFlavor::Clang,
            features: StandardFeatures::default(),
            ctypes: CTypes::default(),
            tags: Vec::new(),
            tag_ids: HashMap::new(),
            ordinary: vec![HashMap::new()],
            tag_names: vec![HashMap::new()],
            definitions: Vec::new(),
            assertion_scope: false,
            extents: HashMap::new(),
            references: HashMap::new(),
            entities: super::entity::Entities::default(),
            typeof_operands: HashMap::new(),
            enumerators: HashMap::new(),
            record_fields: HashMap::new(),
            pragmas: super::pragmas::Pragmas::default(),
            prototype_scope: false,
        }
    }

    pub fn ir_type(&self, q: QualType) -> Type {
        self.ctypes.ir_type(q, &self.target)
    }

    pub fn layout(&self, q: QualType) -> Option<Type> {
        (!self.ctypes.is_void(q)).then(|| self.ir_type(q))
    }

    pub fn object_type(&self, q: QualType, reason: &'static str) -> Result<Type, ResolveError> {
        self.layout(q).ok_or(ResolveError::Unsupported(reason))
    }

    pub fn render(&self, q: QualType) -> CTypeMetadata {
        self.ctypes.render(q, &self.definitions)
    }

    pub(super) fn declaration_spelling(&self, q: QualType, name: &str) -> String {
        self.ctypes.declaration_spelling(q, name, &self.definitions)
    }

    pub(super) fn compiler_flavor(&self) -> CompilerFlavor {
        self.flavor
    }

    pub fn access_of(&self, q: QualType) -> Access {
        self.ctypes.access(q)
    }

    pub fn with_tags(target: TargetInfo, unit: &TranslationUnit) -> Self {
        let mut resolver = Self::new(target);
        resolver.flavor = unit.flavor;
        resolver.features = StandardFeatures::new(unit.standard);
        resolver.tags = unit.tags.clone();
        resolver.pragmas = super::pragmas::collect(unit);
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

    pub(super) fn with_scope<T>(
        &mut self,
        resolve: impl FnOnce(&mut Self) -> Result<T, ResolveError>,
    ) -> Result<T, ResolveError> {
        self.push_scope();
        let result = resolve(self);
        self.pop_scope();
        result
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

    pub(super) fn constant_value(&mut self, e: &crate::ast::Expr) -> Result<Operand, ResolveError> {
        let context =
            super::numeric::Context::new(self.target.clone()).with_features(self.features);
        self.constant_value_with_context(&context, e)
    }

    pub(super) fn constant_value_with_context(
        &mut self,
        context: &super::numeric::Context,
        e: &crate::ast::Expr,
    ) -> Result<Operand, ResolveError> {
        use crate::ast::ExprKind;
        let (ty, kind) = match &e.value {
            ExprKind::Paren(inner) => return self.constant_value_with_context(context, inner),
            ExprKind::Identifier(name) => {
                return match self.lookup(name) {
                    Some(Ordinary::Constant(value)) => Ok(value.clone()),
                    _ => Err(ResolveError::Unsupported(
                        "nonconstant or unknown identifier",
                    )),
                };
            }
            ExprKind::CharLiteral(literal) => {
                let (ty, number) = self.character_constant(literal)?;
                (ty, ValueKind::Constant(number))
            }
            ExprKind::SizeOfExpr(operand) | ExprKind::AlignOfExpr(operand) => {
                let ty = self.assertion_operand_type(operand)?;
                let layout = self
                    .qualified_storage(self.ir_type(ty), self.ctypes.quals(ty).is_atomic)
                    .map_err(|error| match error {
                        ResolveError::Unsupported("incomplete field type")
                            if matches!(e.value, ExprKind::SizeOfExpr(_)) =>
                        {
                            ResolveError::Unsupported("sizeof of incomplete type")
                        }
                        error => error,
                    })?;
                let n = if matches!(e.value, ExprKind::SizeOfExpr(_)) {
                    layout.size_bytes
                } else {
                    self.object_alignment(operand, u64::from(layout.alignment_bytes))
                };
                (
                    self.ctypes.size_type(&self.target),
                    ValueKind::Constant(Number::Integer(n.into())),
                )
            }
            ExprKind::Conditional {
                condition,
                then_value,
                else_value,
            } => {
                let condition = self.constant_value_with_context(context, condition)?;
                let left = match then_value {
                    Some(left) => self.constant_value_with_context(context, left)?,
                    None => condition.clone(),
                };
                let right = self.constant_value_with_context(context, else_value)?;
                let (left, right) = self.arithmetic_operands(context, left, right)?;
                (
                    left.c,
                    ValueKind::Conditional {
                        condition: Box::new(context.condition(condition.value)),
                        then_value: Box::new(left.value),
                        else_value: Box::new(right.value),
                    },
                )
            }
            ExprKind::TypesCompatible { left_ty, right_ty } => (
                self.ctypes.int(),
                ValueKind::Constant(Number::SignedInteger(
                    u8::from(self.types_compatible(left_ty, right_ty)?.0).into(),
                )),
            ),
            ExprKind::Call { callee, arguments }
                if super::expression::choose_expr_operands(callee, arguments).is_some() =>
            {
                let (condition, when_true, when_false) =
                    super::expression::choose_expr_operands(callee, arguments)
                        .ok_or(ResolveError::Unsupported("__builtin_choose_expr"))?;
                let taken = self.constant_integer(condition)?;
                let chosen = if taken.sign() == num_bigint::Sign::NoSign {
                    when_false
                } else {
                    when_true
                };
                return self.constant_value(chosen);
            }
            ExprKind::Call { callee, arguments }
                if super::expression::constant_p_operand(callee, arguments).is_some() =>
            {
                let operand = super::expression::constant_p_operand(callee, arguments)
                    .ok_or(ResolveError::Unsupported("__builtin_constant_p"))?;
                (
                    self.ctypes.int(),
                    ValueKind::Constant(Number::SignedInteger(
                        u8::from(self.is_constant(operand)).into(),
                    )),
                )
            }
            ExprKind::SizeOfType { ty } | ExprKind::AlignOf { ty } => {
                let resolved = self.resolve(&ty.specifiers, &ty.declarator)?;
                let atomic = self.ctypes.quals(resolved).is_atomic;
                let ty = self.ir_type(resolved);
                let layout = self
                    .sizeof_storage(ty, atomic)
                    .map_err(|error| match error {
                        ResolveError::Unsupported("incomplete field type") => {
                            ResolveError::Unsupported("sizeof of incomplete type")
                        }
                        error => error,
                    })?;
                let n = if matches!(e.value, ExprKind::SizeOfType { .. }) {
                    layout.size_bytes
                } else {
                    u64::from(layout.alignment_bytes)
                };
                (
                    self.ctypes.size_type(&self.target),
                    ValueKind::Constant(Number::Integer(n.into())),
                )
            }
            ExprKind::OffsetOf { ty, member } => {
                let ty = self.resolve(&ty.specifiers, &ty.declarator)?;
                let ty = self.object_type(ty, "void offsetof")?;
                let (_, n) = self.offsetof_member(ty, member)?;
                (
                    self.ctypes.size_type(&self.target),
                    ValueKind::Constant(Number::Integer(n.into())),
                )
            }
            ExprKind::Binary { op, left, right } => {
                let left = self.constant_value_with_context(context, left)?;
                let right = self.constant_value_with_context(context, right)?;
                return self.binary_operand(context, e, *op, left, right);
            }
            ExprKind::Unary { op, operand } => {
                use crate::const_expr::UnaryOp;
                let operand = self.constant_value_with_context(context, operand)?;
                match op {
                    UnaryOp::Plus | UnaryOp::Minus | UnaryOp::BitNot => {
                        return self.unary_operand(context, e, *op, operand);
                    }
                    UnaryOp::Not => (
                        self.ctypes.int(),
                        ValueKind::Unary {
                            op: crate::ir::UnaryArithOp::Not,
                            operand: Box::new(context.condition(operand.value)),
                            semantics: crate::ir::ArithSema::Exact,
                        },
                    ),
                    UnaryOp::Real | UnaryOp::Imag if !self.ctypes.is_complex_domain(operand.c) => {
                        let operand = self.promote_operand(context, operand, None)?;
                        if *op == UnaryOp::Real {
                            return Ok(operand);
                        }
                        (operand.c, ValueKind::Constant(Number::Integer(0u8.into())))
                    }
                    _ => return Err(ResolveError::Unsupported("nonconstant unary expression")),
                }
            }
            ExprKind::Comma { left, right } => {
                let left = self.constant_value_with_context(context, left)?;
                let right = self.constant_value_with_context(context, right)?;
                (
                    right.c,
                    ValueKind::Sequence {
                        left: Box::new(left.value),
                        right: Box::new(right.value),
                    },
                )
            }
            ExprKind::Generic {
                controlling,
                associations,
            } => {
                let selected = self.generic_selection(controlling, associations)?;
                return self.constant_value_with_context(context, selected);
            }
            ExprKind::Cast { ty, value } => {
                let ty = self.resolve(&ty.specifiers, &ty.declarator)?;
                self.object_type(ty, "void constant cast")?;
                let value = self.constant_value_with_context(context, value)?;
                let mut operand = self.arithmetic_conversion(
                    context,
                    value,
                    ty,
                    crate::ir::ConversionReason::Explicit,
                )?;
                operand.value.node = e.derive(operand.value.node.value);
                return Ok(operand);
            }
            _ => return self.literal(context, e),
        };
        let truth = matches!(
            kind,
            ValueKind::Unary {
                op: crate::ir::UnaryArithOp::Not,
                ..
            }
        );
        let mut operand = self.operand(e, ty, kind);
        if truth {
            operand.value.ty = Type::Bool;
        }
        Ok(operand)
    }

    pub(super) fn object_alignment(&self, e: &crate::ast::Expr, natural: u64) -> u64 {
        let mut operand = e;
        while let crate::ast::ExprKind::Paren(inner) = &operand.value {
            operand = inner;
        }
        let Some(id) = self.references.get(&operand.id) else {
            return natural;
        };
        let Some(requested) = self.entities.request(id).alignment else {
            return natural;
        };
        self.declared_alignment(requested, natural)
    }

    fn object(&self, e: &crate::ast::Expr) -> Option<QualType> {
        let id = self.references.get(&e.id)?;
        self.entities.ty(id)
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
            GenericControl::Type { ty } => {
                let ty = self.resolve(&ty.specifiers, &ty.declarator)?;
                self.object_type(ty, "void generic controlling type")?;
                ty
            }
            GenericControl::Expr(expr) => self.assertion_operand_type(expr)?,
        };
        self.select_association(controlling, associations)
    }

    // The controlling operand is lvalue-converted, so array and function associations never match.
    pub(super) fn select_association<'e>(
        &mut self,
        controlling: QualType,
        associations: &'e [crate::ast::GenericAssociation],
    ) -> Result<&'e crate::ast::Expr, ResolveError> {
        use crate::ast::GenericAssociation;
        let controlling = self.ctypes.lvalue_conversion(controlling);
        let mut selected = None;
        let mut fallback = None;
        for association in associations {
            match association {
                GenericAssociation::Default(value) => fallback = Some(value),
                GenericAssociation::Type { ty, value } => {
                    let ty = self.resolve(&ty.specifiers, &ty.declarator)?;
                    self.object_type(ty, "void generic association type")?;
                    if self.ctypes.compatible(ty, controlling) {
                        if selected.is_some() {
                            return Err(ResolveError::Invalid("ambiguous generic selection"));
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
    ) -> Result<QualType, ResolveError> {
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
                ExprKind::Identifier(name) if builtin_result_type(name).is_some() => {
                    Ok(self.ctypes.qual(CTypeKind::Bool))
                }
                _ => Err(ResolveError::Unsupported("nonconstant call expression")),
            },
            ExprKind::Identifier(_) if self.object(e).is_some() => self
                .object(e)
                .ok_or(ResolveError::Unsupported("untyped binding")),
            ExprKind::Identifier(name) => match self.lookup(name) {
                Some(Ordinary::Object(q)) => Ok(*q),
                Some(Ordinary::Constant(value)) => Ok(value.c),
                _ => Err(ResolveError::Unsupported(
                    "unknown or unsupported sizeof operand type",
                )),
            },
            ExprKind::StringLiteral(literal) => Ok(self.string_type(literal)),
            ExprKind::CompoundLiteral { ty, initializer } => {
                let resolved = self.resolve(&ty.specifiers, &ty.declarator)?;
                if let Some((element, Extent::Incomplete)) = self.ctypes.element(resolved) {
                    let length = self.inferred_array_length(&self.ir_type(element), initializer)?;
                    Ok(self.ctypes.qual(CTypeKind::Array {
                        element,
                        extent: Extent::Fixed(length),
                    }))
                } else {
                    Ok(resolved)
                }
            }
            ExprKind::Unary {
                op: crate::const_expr::UnaryOp::Deref,
                operand,
            } => {
                let pointer = self.assertion_operand_type(operand)?;
                self.ctypes
                    .pointee(pointer)
                    .ok_or(ResolveError::Unsupported(
                        "sizeof dereference of nonpointer",
                    ))
            }
            ExprKind::Index { base, .. } => {
                let base = self.assertion_operand_type(base)?;
                self.ctypes
                    .element(base)
                    .map(|(element, _)| element)
                    .or_else(|| self.ctypes.pointee(base))
                    .ok_or(ResolveError::Unsupported("sizeof index of nonarray"))
            }
            ExprKind::Comma { right, .. } => {
                let right = self.assertion_operand_type(right)?;
                Ok(self.ctypes.lvalue_conversion(right))
            }
            ExprKind::Unary {
                op: crate::const_expr::UnaryOp::AddrOf,
                operand,
            } => {
                let pointee = self.assertion_operand_type(operand)?;
                Ok(self.ctypes.pointer(pointee))
            }
            ExprKind::Member { base, field, arrow } => {
                let base = self.assertion_operand_type(base)?;
                let base = if *arrow {
                    self.ctypes
                        .pointee(base)
                        .ok_or(ResolveError::Unsupported("sizeof member of nonpointer"))?
                } else {
                    base
                };
                self.field_of(base, &field.value)
                    .ok_or(ResolveError::Unsupported("sizeof of unknown member"))
            }
            ExprKind::Conditional {
                condition,
                then_value,
                ..
            } => {
                let left = match then_value {
                    Some(then_value) => self.assertion_operand_type(then_value)?,
                    None => self.assertion_operand_type(condition)?,
                };
                if self.ctypes.is_record(left) {
                    return Ok(left);
                }
                self.constant_value(e).map(|value| value.c)
            }
            _ => self.constant_value(e).map(|value| value.c),
        }
    }

    pub(super) fn merge_redeclaration(
        &mut self,
        id: BindingId,
        previous: Option<QualType>,
        declared: QualType,
    ) -> Result<Option<&'static str>, ResolveError> {
        let Some(previous) = previous else {
            return Ok(None);
        };
        if let Some(composite) = self.ctypes.composite(previous, declared) {
            self.entities.declare(id, composite, false);
            return Ok(None);
        }
        let message = self.conflict_message(previous, declared)?;
        self.entities.declare(id, previous, false);
        Ok(Some(message))
    }

    /// The conflict table in ir-spec.md: a return or object type may differ only
    /// where the layouts coincide, but a prototyped parameter list may always
    /// differ, because MSVC warns (C4028/C4030/C4031/C4052) rather than rejecting.
    fn conflict_message(
        &mut self,
        previous: QualType,
        declared: QualType,
    ) -> Result<&'static str, ResolveError> {
        let returns = self
            .ctypes
            .function_parts(previous)
            .map(|(ret, ..)| ret)
            .zip(self.ctypes.function_parts(declared).map(|(ret, ..)| ret));
        if let Some((previous_return, declared_return)) = returns {
            if self.ctypes.compatible(previous_return, declared_return) {
                return Ok("function redeclared with a different parameter list");
            }
            if same_layout(
                &self.ir_type(previous_return),
                &self.ir_type(declared_return),
            ) {
                return Ok(
                    "function redeclared with a different integer return type of the same size",
                );
            }
            return Err(ResolveError::Invalid(
                "conflicting types for function redeclaration",
            ));
        }
        if same_layout(&self.ir_type(previous), &self.ir_type(declared)) {
            return Ok("redeclaration with a different integer type of the same size");
        }
        Err(ResolveError::Invalid("conflicting types for redeclaration"))
    }

    pub(super) fn require_modifiable_lvalue(&self, q: QualType) -> Result<(), ResolveError> {
        if self.ctypes.is_array(q) {
            return Err(ResolveError::Invalid("cannot assign to an array type"));
        }
        if self.ctypes.quals(q).is_const {
            return Err(ResolveError::Invalid(
                "cannot assign to a const-qualified lvalue",
            ));
        }
        if self.has_const_member(q, &mut Vec::new()) {
            return Err(ResolveError::Invalid(
                "cannot assign to a variable with a const-qualified member",
            ));
        }
        Ok(())
    }

    fn has_const_member(&self, q: QualType, seen: &mut Vec<TypeId>) -> bool {
        let CTypeKind::Record { id, .. } = self.ctypes.canonical_kind(q) else {
            return false;
        };
        let id = *id;
        if seen.contains(&id) {
            return false;
        }
        seen.push(id);
        self.record_fields.get(&id).is_some_and(|fields| {
            fields.iter().any(|field| {
                self.ctypes.quals(*field).is_const || self.has_const_member(*field, seen)
            })
        })
    }

    fn field_of(&self, q: QualType, name: &str) -> Option<QualType> {
        let CTypeKind::Record { id, .. } = self.ctypes.canonical_kind(q) else {
            return None;
        };
        let TypeDefinitionKind::Record {
            fields: Some(fields),
            ..
        } = &self.definitions[id.0 as usize].kind
        else {
            return None;
        };
        let types = self.record_fields.get(id)?;
        let quals = self.ctypes.quals(q);
        for (index, field) in fields.iter().enumerate() {
            let member = types.get(index).copied()?;
            if field.name.as_deref() == Some(name) {
                return Some(member.with(quals));
            }
            if field.name.is_none()
                && let Some(found) = self.field_of(member, name)
            {
                return Some(found.with(quals));
            }
        }
        None
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

    pub fn define_alias(&mut self, name: String, resolved: QualType) -> Result<(), ResolveError> {
        let ty = self.ir_type(resolved);
        let id = self.push(TypeDefinitionKind::Alias(ty));
        self.definitions[id.0 as usize].name = Some(name.clone());
        let alias = self.ctypes.qual(CTypeKind::Typedef {
            name: name.clone(),
            underlying: resolved,
        });
        self.declare(&name, Ordinary::Alias(alias));
        Ok(())
    }

    pub fn resolve(
        &mut self,
        specifiers: &DeclarationSpecifiers,
        declarator: &Declarator,
    ) -> Result<QualType, ResolveError> {
        let base = self
            .base(&specifiers.ty)?
            .with(specifiers.qualifiers.into());
        let resolved = self.derive(declarator, base)?;
        Ok(if specifiers.is_constexpr {
            resolved.with(Qualifiers::CONST)
        } else {
            resolved
        })
    }

    fn base(&mut self, specifier: &TypeSpecifier) -> Result<QualType, ResolveError> {
        let kind = match specifier {
            TypeSpecifier::Void => CTypeKind::Void,
            TypeSpecifier::Atomic(inner) => {
                let inner = self.resolve(&inner.specifiers, &inner.declarator)?;
                CTypeKind::AtomicSpecifier(inner)
            }
            TypeSpecifier::TypeOf(operand) | TypeSpecifier::TypeOfUnqual(operand) => {
                let unqualified = matches!(specifier, TypeSpecifier::TypeOfUnqual(_));
                let resolved = self.typeof_operand(operand)?;
                let spelling = match operand {
                    TypeOfOperand::Expression(expr) => expr.value.to_string(),
                    TypeOfOperand::Type(_) => self.ctypes.spelling(resolved, &self.definitions),
                };
                let keyword = if unqualified {
                    "typeof_unqual"
                } else {
                    "typeof"
                };
                CTypeKind::TypeOf {
                    spelling: format!("{keyword}({spelling})"),
                    underlying: if unqualified {
                        self.ctypes.unqualified(resolved)
                    } else {
                        resolved
                    },
                }
            }
            TypeSpecifier::Named(name) => {
                let Some(Ordinary::Alias(alias)) = self.lookup(name) else {
                    return Err(ResolveError::Unsupported("unknown typedef"));
                };
                return Ok(*alias);
            }
            TypeSpecifier::Tag(TagSpecifier::Definition(id)) => {
                let tag = self
                    .tags
                    .iter()
                    .find(|tag| tag.value.id == *id)
                    .cloned()
                    .ok_or(ResolveError::Unsupported("unknown tag definition"))?;
                let id = self.define_tag(&tag.value)?;
                tag_kind(tag.kind, id)
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
                    let id = self.push(incomplete_tag(*kind));
                    self.definitions[id.0 as usize].name = Some(name.clone());
                    self.declare_tag(key, id);
                    id
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
                    let underlying =
                        self.resolve(&fixed_type.specifiers, &fixed_type.declarator)?;
                    let underlying_ty =
                        self.object_type(underlying, "void enum underlying type")?;
                    let layout = self.storage(underlying_ty.clone())?;
                    self.ctypes.set_enum_underlying(id, underlying);
                    self.definitions[id.0 as usize].kind = TypeDefinitionKind::Enum {
                        underlying: Some(underlying_ty),
                        enumerators: None,
                        layout: Some(layout),
                    };
                }
                tag_kind(*kind, id)
            }
            TypeSpecifier::Bool => CTypeKind::Bool,
            TypeSpecifier::Complex(inner) => {
                let component = self.base(inner)?;
                let component = self.ctypes.canonical(component).ty;
                if !matches!(
                    self.ctypes.kind(component),
                    CTypeKind::Char
                        | CTypeKind::SChar
                        | CTypeKind::UChar
                        | CTypeKind::Int { .. }
                        | CTypeKind::BitInt { .. }
                        | CTypeKind::Float(_)
                ) {
                    return Err(ResolveError::Unsupported("complex component type"));
                }
                CTypeKind::Complex(component)
            }
            TypeSpecifier::Imaginary(inner) => {
                let component = self.base(inner)?;
                match self.ctypes.canonical_kind(component) {
                    CTypeKind::Float(kind) if !kind.is_decimal() => CTypeKind::Imaginary(*kind),
                    _ => {
                        return Err(ResolveError::Invalid(
                            "imaginary component must be a real floating type",
                        ));
                    }
                }
            }
            TypeSpecifier::Integer(IntegerType::Char { signed }) => match signed {
                None => CTypeKind::Char,
                Some(true) => CTypeKind::SChar,
                Some(false) => CTypeKind::UChar,
            },
            TypeSpecifier::Integer(IntegerType::Ranked { rank, signed }) => CTypeKind::Int {
                rank: match rank {
                    IntegerRank::Short => IntRank::Short,
                    IntegerRank::Int => IntRank::Int,
                    IntegerRank::Long => IntRank::Long,
                    IntegerRank::LongLong => IntRank::LongLong,
                    IntegerRank::Int128 => IntRank::Int128,
                },
                signed: *signed,
            },
            TypeSpecifier::Integer(IntegerType::BitInt { width, signed }) => {
                let width = u32::try_from(self.constant_integer(width)?)
                    .map_err(|_| ResolveError::Unsupported("invalid _BitInt width"))?;
                if width < if *signed { 2 } else { 1 } || width > super::validate::BIT_INT_MAX_WIDTH
                {
                    return Err(ResolveError::Unsupported("invalid _BitInt width"));
                }
                CTypeKind::BitInt {
                    width,
                    signed: *signed,
                }
            }
            TypeSpecifier::Floating(float) => CTypeKind::Float(match float {
                FloatingType::Float16 => FloatKind::Float16,
                FloatingType::Fp16 => FloatKind::Fp16,
                FloatingType::Float => FloatKind::Float,
                FloatingType::Double => FloatKind::Double,
                FloatingType::LongDouble => FloatKind::LongDouble,
                FloatingType::Float128 | FloatingType::Float128Ext => FloatKind::Float128,
                FloatingType::Decimal32 => FloatKind::Decimal32,
                FloatingType::Decimal64 => FloatKind::Decimal64,
                FloatingType::Decimal128 => FloatKind::Decimal128,
                _ => return Err(ResolveError::Unsupported("floating type")),
            }),
            TypeSpecifier::Vector(vector) => {
                let element = self.base(&vector.element)?;
                let valid = match self.ctypes.canonical_kind(element) {
                    CTypeKind::Char
                    | CTypeKind::SChar
                    | CTypeKind::UChar
                    | CTypeKind::Int { .. } => true,
                    CTypeKind::Float(kind) => !kind.is_decimal(),
                    _ => false,
                };
                if !valid {
                    return Err(ResolveError::Invalid(
                        "vector element must be an integer or real floating type",
                    ));
                }
                let element_bytes = self.storage(self.ir_type(element))?.size_bytes;
                let (bytes, lanes) = match &vector.size {
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
                CTypeKind::Vector {
                    element: self.ctypes.canonical(element).local_unqualified(),
                    lanes,
                    bytes,
                }
            }
            TypeSpecifier::FixedPoint(fixed) => CTypeKind::FixedPoint(FixedType {
                kind: match fixed.kind {
                    FixedPointKind::Fract => FixedKind::Fract,
                    FixedPointKind::Accum => FixedKind::Accum,
                },
                rank: match fixed.rank {
                    FixedPointRank::Short => FixedRank::Short,
                    FixedPointRank::Default => FixedRank::Default,
                    FixedPointRank::Long => FixedRank::Long,
                    FixedPointRank::LongLong => FixedRank::LongLong,
                },
                signed: fixed.signed,
                saturating: fixed.saturated,
            }),
            TypeSpecifier::TargetBuiltin(name) if name == "__builtin_va_list" => CTypeKind::VaList,
            TypeSpecifier::TargetBuiltin(_) => {
                return Err(ResolveError::Unsupported("target builtin type"));
            }
        };
        Ok(self.ctypes.qual(kind))
    }

    fn typeof_operand(&mut self, operand: &TypeOfOperand) -> Result<QualType, ResolveError> {
        match operand {
            TypeOfOperand::Type(ty) => self.resolve(&ty.specifiers, &ty.declarator),
            TypeOfOperand::Expression(expr) => {
                if let Some(resolved) = self.typeof_operands.get(&expr.id) {
                    return Ok(*resolved);
                }
                self.assertion_operand_type(expr)
            }
        }
    }

    fn derive(&mut self, declarator: &Declarator, q: QualType) -> Result<QualType, ResolveError> {
        match declarator {
            Declarator::Name(_) | Declarator::Abstract => Ok(q),
            Declarator::Grouped(inner) | Declarator::Attributed { inner, .. } => {
                self.derive(inner, q)
            }
            Declarator::Pointer {
                inner, qualifiers, ..
            } => {
                let q = self.ctypes.pointer(q).with((*qualifiers).into());
                self.derive(inner, q)
            }
            Declarator::Array { .. } => {
                let mut extents = Vec::new();
                let mut core = declarator;
                while let Declarator::Array { inner, size, .. } = core {
                    extents.push(match size {
                        ArraySize::Unspecified => Extent::Incomplete,
                        ArraySize::Star => Extent::Variable(None),
                        ArraySize::Expression(expr) => match self.extents.get(&expr.id) {
                            Some(extent) => Extent::Variable(Some(*extent)),
                            None => match self.constant_integer(expr) {
                                Ok(length) => {
                                    Extent::Fixed(u64::try_from(length).map_err(|_| {
                                        ResolveError::Unsupported("invalid array length")
                                    })?)
                                }
                                Err(_) if self.prototype_scope => Extent::Variable(None),
                                Err(error) => return Err(error),
                            },
                        },
                    });
                    core = inner;
                }
                let (core, mut q) = self.apply_pointers(core, q);
                for extent in extents {
                    if self.ctypes.is_void(q) {
                        return Err(ResolveError::Unsupported("void array element"));
                    }
                    q = self.ctypes.qual(CTypeKind::Array { element: q, extent });
                }
                self.derive(core, q)
            }
            Declarator::Function { inner, parameters } => {
                let params = self.with_scope(|this| {
                    let mut params = Vec::new();
                    for parameter in parameters.parameters() {
                        let resolved =
                            this.resolve_parameter(&parameter.specifiers, &parameter.declarator)?;
                        if this.ctypes.is_void(resolved) {
                            return Err(ResolveError::Unsupported("void parameter"));
                        }
                        params.push(resolved);
                    }
                    Ok(params)
                })?;
                let prototyped = self.features.empty_parens_are_prototype
                    || !matches!(parameters, ParameterList::Empty);
                let (core, ret) = self.apply_pointers(inner, q);
                let q = self.ctypes.qual(CTypeKind::Function {
                    ret,
                    params,
                    variadic: parameters.is_variadic(),
                    prototyped,
                });
                self.derive(core, q)
            }
        }
    }

    pub(super) fn resolve_parameter(
        &mut self,
        specifiers: &DeclarationSpecifiers,
        declarator: &Declarator,
    ) -> Result<QualType, ResolveError> {
        let enclosing = std::mem::replace(&mut self.prototype_scope, true);
        let resolved = self.resolve(specifiers, declarator);
        self.prototype_scope = enclosing;
        resolved
    }

    pub(super) fn parameter_shape(
        &mut self,
        resolved: QualType,
        declared_array: ArrayDeclarator,
    ) -> Result<ParameterShape, ResolveError> {
        let written = self.object_type(resolved, "void parameter")?;
        let array = match &written {
            Type::Array { length, .. } => Some(ArrayParameter {
                extent: length.map_or(ArrayExtent::Unspecified, ArrayExtent::Fixed),
                guaranteed: declared_array.is_static,
            }),
            Type::VariableArray { extent, .. } => Some(ArrayParameter {
                extent: ArrayExtent::Variable(*extent),
                guaranteed: declared_array.is_static,
            }),
            _ => None,
        };
        let qualifiers = match written {
            Type::Array { .. } | Type::VariableArray { .. } => declared_array.qualifiers.into(),
            _ => self.ctypes.quals(resolved),
        };
        let adjusted = self.adjusted_parameter(resolved, qualifiers);
        Ok(ParameterShape {
            ty: self.ir_type(adjusted),
            adjusted,
            qualifiers,
            array,
        })
    }

    pub(super) fn adjusted_parameter(&mut self, q: QualType, array: Qualifiers) -> QualType {
        if let Some((element, _)) = self.ctypes.element(q) {
            return self.ctypes.pointer(element).with(array);
        }
        if self.ctypes.is_function(q) {
            return self.ctypes.pointer(q);
        }
        q
    }

    fn apply_pointers<'d>(
        &mut self,
        mut core: &'d Declarator,
        mut q: QualType,
    ) -> (&'d Declarator, QualType) {
        while let Declarator::Pointer {
            inner, qualifiers, ..
        } = core
        {
            q = self.ctypes.pointer(q).with((*qualifiers).into());
            core = inner;
        }
        (core, q)
    }

    /// C11 6.7.2.3p8: `struct S;` alone declares an incomplete tag in the
    /// current scope, hiding any outer one, rather than referring outward.
    pub(super) fn declare_incomplete_tag(&mut self, kind: TagKind, name: &str) -> TypeId {
        let key = (kind, name.to_owned());
        if let Some(id) = self.tag_names.last().and_then(|scope| scope.get(&key)) {
            return *id;
        }
        let id = self.push(incomplete_tag(kind));
        self.definitions[id.0 as usize].name = Some(name.to_owned());
        self.declare_tag(key, id);
        id
    }

    fn redeclare_tag_names(&mut self, tag: &TagDefinition, id: TypeId) {
        if let Some(name) = &tag.name {
            self.declare_tag((tag.kind, name.clone()), id);
        }
        let TagBody::Enum { enumerators, .. } = &tag.body else {
            return;
        };
        for item in enumerators {
            let EnumItemKind::Enumerator(enumerator) = &item.value else {
                continue;
            };
            let Some(operand) = self.enumerators.get(&item.id).cloned() else {
                continue;
            };
            self.declare(&enumerator.name, Ordinary::Constant(operand));
        }
    }

    fn define_tag(&mut self, tag: &TagDefinition) -> Result<TypeId, ResolveError> {
        if let Some(id) = self.tag_ids.get(&tag.id).copied() {
            self.redeclare_tag_names(tag, id);
            return Ok(id);
        }
        let previous = tag.name.as_ref().and_then(|name| {
            self.tag_names
                .last()
                .and_then(|scope| scope.get(&(tag.kind, name.clone())))
                .copied()
        });
        let redefines = previous.filter(|id| is_complete(&self.definitions[id.0 as usize].kind));
        let redefined_fields = redefines.and_then(|id| self.record_fields.get(&id).cloned());
        let id = previous.unwrap_or_else(|| self.push(incomplete_tag(tag.kind)));
        self.tag_ids.insert(tag.id, id);
        self.definitions[id.0 as usize].name = tag.name.clone();
        if let Some(name) = &tag.name {
            self.declare_tag((tag.kind, name.clone()), id);
        }
        let kind = match &tag.body {
            TagBody::Record(items) => {
                let mut fields = Vec::new();
                let mut field_types = Vec::new();
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
                        fields.push(item.derive(Field {
                            name: None,
                            ty: self.object_type(resolved, "void record field")?,
                            is_const: self.ctypes.quals(resolved).is_const,
                            access: self.access_of(resolved),
                            bit_width: None,
                        }));
                        field_types.push(resolved);
                        requests.push(field_request(
                            self,
                            &declaration.specifiers.attributes,
                            &[],
                        )?);
                    }
                    for declarator in &declaration.declarators {
                        let resolved =
                            self.resolve(&declaration.specifiers, &declarator.declarator)?;
                        let ty = self.object_type(resolved, "void record field")?;
                        let bit_width = declarator
                            .bit_width
                            .as_ref()
                            .map(|expr| {
                                let value = self.constant_integer(expr)?;
                                u32::try_from(value).map_err(|_| {
                                    ResolveError::Unsupported("invalid bit-field width")
                                })
                            })
                            .transpose()?;
                        fields.push(declarator.derive(Field {
                            name: declarator.declarator.name().map(str::to_owned),
                            ty,
                            is_const: self.ctypes.quals(resolved).is_const,
                            access: self.access_of(resolved),
                            bit_width,
                        }));
                        field_types.push(resolved);
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
                if self.pragmas.is_ms_struct(tag.id) {
                    return Err(ResolveError::Unsupported("ms_struct record layout"));
                }
                let max_field_alignment = self.pragmas.max_field_alignment(tag.id);
                let alignment = requested_alignment(self, &tag.attributes)?;
                let layout = self.layout_record(
                    tag.kind,
                    &fields,
                    &requests,
                    packed,
                    max_field_alignment,
                    alignment,
                )?;
                self.record_fields.insert(id, field_types);
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
                let fixed_underlying = fixed_type
                    .as_ref()
                    .map(|fixed_type| self.resolve(&fixed_type.specifiers, &fixed_type.declarator))
                    .transpose()?;
                let mut values = Vec::new();
                let mut previous = -1i64;
                let mut prior = HashMap::new();
                for item in enumerators {
                    let EnumItemKind::Enumerator(enumerator) = &item.value else {
                        continue;
                    };
                    let value = if let Some(expr) = &enumerator.value {
                        i64::try_from(self.constant_integer(expr)?).map_err(|_| {
                            ResolveError::Unsupported("enum value outside supported i64 range")
                        })?
                    } else {
                        previous
                            .checked_add(1)
                            .ok_or(ResolveError::Unsupported("enum value overflow"))?
                    };
                    previous = value;
                    prior.insert(enumerator.name.clone(), value);
                    let int_ty = self.ctypes.int();
                    let ty = self.ir_type(int_ty);
                    self.declare(
                        &enumerator.name,
                        Ordinary::Constant(Operand {
                            value: Value {
                                ty,
                                node: item.derive(ValueKind::Constant(Number::SignedInteger(
                                    BigInt::from(value),
                                ))),
                            },
                            c: int_ty,
                        }),
                    );
                    values.push((item, enumerator, value));
                }
                let is_fixed = fixed_underlying.is_some();
                let fits_int = values
                    .iter()
                    .all(|(_, _, value)| i32::try_from(*value).is_ok());
                let underlying_c = if let Some(fixed_underlying) = fixed_underlying {
                    fixed_underlying
                } else {
                    let (rank, signed) = if values.iter().any(|(_, _, value)| *value < 0) {
                        (
                            if fits_int {
                                IntRank::Int
                            } else {
                                IntRank::Long
                            },
                            true,
                        )
                    } else if values
                        .iter()
                        .all(|(_, _, value)| u32::try_from(*value).is_ok())
                    {
                        (IntRank::Int, false)
                    } else {
                        (IntRank::Long, false)
                    };
                    self.ctypes.qual(CTypeKind::Int { rank, signed })
                };
                let underlying = self.object_type(underlying_c, "void enum underlying type")?;
                self.ctypes.set_enum_underlying(id, underlying_c);
                let enumerator_c = if !is_fixed && fits_int {
                    self.ctypes.int()
                } else if self.features.enumerators_have_enum_type {
                    self.ctypes.qual(CTypeKind::Enum(id))
                } else {
                    underlying_c
                };
                let enumerator_type = self.ir_type(enumerator_c);
                let mut entries = Vec::new();
                for (item, enumerator, value) in values {
                    let value = Value {
                        ty: enumerator_type.clone(),
                        node: item.derive(ValueKind::Constant(if value < 0 {
                            Number::SignedInteger(BigInt::from(value))
                        } else {
                            Number::Integer(BigUint::from(value as u64))
                        })),
                    };
                    self.declare(
                        &enumerator.name,
                        Ordinary::Constant(Operand {
                            value: value.clone(),
                            c: enumerator_c,
                        }),
                    );
                    self.enumerators.insert(
                        item.id,
                        Operand {
                            value: value.clone(),
                            c: enumerator_c,
                        },
                    );
                    entries.push(item.derive(Enumerator {
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
        if redefines.is_some() {
            if self.features.compatible_tag_redefinitions
                && same_tag_content(&self.definitions[id.0 as usize].kind, &kind)
                && self.same_field_types(id, redefined_fields.as_deref(), &kind)
            {
                return Ok(id);
            }
            return Err(ResolveError::Invalid(
                "redefinition of struct, union, or enum tag",
            ));
        }
        self.definitions[id.0 as usize].kind = kind;
        Ok(id)
    }

    fn same_field_types(
        &self,
        id: TypeId,
        redefined: Option<&[QualType]>,
        kind: &TypeDefinitionKind,
    ) -> bool {
        if !matches!(kind, TypeDefinitionKind::Record { .. }) {
            return true;
        }
        let (Some(redefined), Some(current)) = (redefined, self.record_fields.get(&id)) else {
            return false;
        };
        redefined.len() == current.len()
            && redefined
                .iter()
                .zip(current)
                .all(|(a, b)| self.ctypes.compatible(*a, *b))
    }

    pub(super) fn types_compatible(
        &mut self,
        left: &TypeName,
        right: &TypeName,
    ) -> Result<(bool, String), ResolveError> {
        let left = self.compared_type(left)?;
        let right = self.compared_type(right)?;
        let compared = format!(
            "{}, {}",
            self.render(left).canonical,
            self.render(right).canonical
        );
        Ok((self.ctypes.compatible(left, right), compared))
    }

    fn compared_type(&mut self, name: &TypeName) -> Result<QualType, ResolveError> {
        let resolved = self.resolve(&name.specifiers, &name.declarator)?;
        let atomic = Qualifiers {
            is_atomic: self.ctypes.quals(resolved).is_atomic,
            ..Qualifiers::NONE
        };
        let unqualified = self.ctypes.unqualified(resolved);
        Ok(self.ctypes.canonical(unqualified).with(atomic))
    }

    pub(super) fn require_pointer_element(&self, ty: &Type) -> Result<(), ResolveError> {
        match ty {
            Type::VariableArray { element, .. } => self.require_pointer_element(element),
            Type::Void => Ok(()),
            ty => self.storage(ty.clone()).map(|_| ()),
        }
    }

    pub(super) fn storage(&self, ty: Type) -> Result<StorageLayout, ResolveError> {
        self.qualified_storage(ty, false)
    }

    pub(super) fn sizeof_storage(
        &self,
        ty: Type,
        atomic: bool,
    ) -> Result<StorageLayout, ResolveError> {
        if matches!(ty, Type::Void) {
            return Ok(StorageLayout {
                size_bytes: 1,
                alignment_bytes: 1,
            });
        }
        self.qualified_storage(ty, atomic)
    }

    fn promotes_atomic_layout(&self) -> bool {
        !matches!(self.flavor, CompilerFlavor::Gcc)
    }

    pub(super) fn effective_alignment(&self, requested: u64, natural: u64) -> u64 {
        if matches!(self.flavor, CompilerFlavor::Clang) {
            requested
        } else {
            requested.max(natural)
        }
    }

    pub(super) fn declared_alignment(&self, requested: u64, natural: u64) -> u64 {
        if matches!(self.flavor, CompilerFlavor::Msvc) {
            requested.max(natural)
        } else {
            requested
        }
    }

    pub(super) fn qualified_storage(
        &self,
        ty: Type,
        atomic: bool,
    ) -> Result<StorageLayout, ResolveError> {
        let promote = |layout| {
            if atomic && self.promotes_atomic_layout() {
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
        max_field_alignment: Option<u64>,
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
            let align = (if packed || field_packed { 1 } else { natural })
                .max(field_aligned.unwrap_or(1))
                .min(max_field_alignment.unwrap_or(u64::MAX));
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
                } else if packed || field_packed || max_field_alignment.is_some() {
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

fn is_complete(kind: &TypeDefinitionKind) -> bool {
    match kind {
        TypeDefinitionKind::Record { fields, .. } => fields.is_some(),
        TypeDefinitionKind::Enum { enumerators, .. } => enumerators.is_some(),
        TypeDefinitionKind::Alias(_) => true,
    }
}

fn same_tag_content(a: &TypeDefinitionKind, b: &TypeDefinitionKind) -> bool {
    match (a, b) {
        (
            TypeDefinitionKind::Record {
                fields: Some(a), ..
            },
            TypeDefinitionKind::Record {
                fields: Some(b), ..
            },
        ) => {
            a.len() == b.len()
                && a.iter().zip(b).all(|(a, b)| {
                    a.value.name == b.value.name
                        && a.value.ty == b.value.ty
                        && a.value.access == b.value.access
                        && a.value.bit_width == b.value.bit_width
                })
        }
        (
            TypeDefinitionKind::Enum {
                underlying: a_underlying,
                enumerators: Some(a),
                ..
            },
            TypeDefinitionKind::Enum {
                underlying: b_underlying,
                enumerators: Some(b),
                ..
            },
        ) => {
            a_underlying == b_underlying
                && a.len() == b.len()
                && a.iter().zip(b).all(|(a, b)| {
                    a.value.name == b.value.name
                        && matches!(
                            (&a.value.value.node.value, &b.value.value.node.value),
                            (ValueKind::Constant(a), ValueKind::Constant(b)) if a == b
                        )
                })
        }
        _ => false,
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

fn align_up(value: u64, alignment: u64) -> Result<u64, ResolveError> {
    if alignment == 0 || !alignment.is_power_of_two() {
        return Err(ResolveError::Unsupported("invalid alignment"));
    }
    value
        .checked_add(alignment - 1)
        .map(|sum| sum & !(alignment - 1))
        .ok_or(ResolveError::Unsupported("record size overflow"))
}

pub(super) fn requested_alignment<'a>(
    resolver: &mut TypeResolver,
    attributes: impl IntoIterator<Item = &'a Attribute>,
) -> Result<Option<u64>, ResolveError> {
    let mut requested: Option<u64> = None;
    for attribute in attributes {
        let value = match attribute {
            Attribute::Aligned(expr) | Attribute::AlignAs(AlignAsOperand::Expr(expr)) => {
                u64::try_from(resolver.constant_integer(expr)?)
                    .map_err(|_| ResolveError::Unsupported("invalid alignment"))?
            }
            Attribute::AlignAs(AlignAsOperand::Type { ty }) => {
                let resolved = resolver.resolve(&ty.specifiers, &ty.declarator)?;
                u64::from(
                    resolver
                        .storage(resolver.object_type(resolved, "void alignment type")?)?
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

/// Layout identity in MSVC's sense: the same storage shape, ignoring integer
/// signedness. C4142 ("benign redefinition") fires exactly here, C2371 otherwise.
pub(super) fn same_layout(a: &Type, b: &Type) -> bool {
    match (a, b) {
        (
            Type::Numeric(NumericType::Integer {
                width: a_width,
                bit_precise: a_bit_precise,
                ..
            }),
            Type::Numeric(NumericType::Integer {
                width: b_width,
                bit_precise: b_bit_precise,
                ..
            }),
        ) => a_width == b_width && a_bit_precise == b_bit_precise,
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
        ) => a_const == b_const && a_access == b_access && same_layout(a, b),
        (
            Type::Array {
                element: a,
                length: a_length,
            },
            Type::Array {
                element: b,
                length: b_length,
            },
        ) => a_length == b_length && same_layout(a, b),
        _ => a == b,
    }
}

fn tag_kind(kind: TagKind, id: TypeId) -> CTypeKind {
    match kind {
        TagKind::Enum => CTypeKind::Enum(id),
        TagKind::Struct | TagKind::Union => CTypeKind::Record {
            id,
            union: kind == TagKind::Union,
        },
    }
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
                    module.types.push(declaration.derive(definition.clone()));
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
                    let c_entries = resolver.render(resolved).entries();
                    resolver.define_alias(name, resolved)?;
                    for definition in &resolver.definitions[start..] {
                        let span = declarator.derive(definition.clone());
                        if matches!(definition.kind, TypeDefinitionKind::Alias(_)) {
                            module.annotate(&span, c_entries.clone());
                        }
                        module.types.push(span);
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
                let return_c = resolver.resolve(&function.specifiers, &Declarator::Abstract)?;
                let return_type = resolver.layout(return_c);
                let parameters =
                    resolve_parameters(&mut resolver, signature, &mut module, &mut next_binding)?;
                let parameter_operands = parameter_operands(&parameters);
                let result = return_type.clone().map(|ty| super::abi::AbiOperand {
                    ty,
                    atomic: resolver.ctypes.quals(return_c).is_atomic,
                });
                let abi = super::abi::AbiClassifier::new(&resolver, &module.target).from_parts(
                    result.as_ref(),
                    &parameter_operands,
                    matches!(
                        &parameters,
                        crate::ir::Parameters::Prototype { variadic: true, .. }
                    ),
                    parameter_operands.len(),
                )?;
                let lowered = declaration.derive(Function {
                    id: BindingId(next_binding),
                    name: name.into(),
                    parameters,
                    return_type,
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
                });
                module.annotate(&lowered, resolver.render(return_c).entries());
                module.functions.push(lowered);
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
                    let return_c = resolver.resolve(&item.specifiers, &Declarator::Abstract)?;
                    let return_type = resolver.layout(return_c);
                    let parameters = resolve_parameters(
                        &mut resolver,
                        signature,
                        &mut module,
                        &mut next_binding,
                    )?;
                    let parameter_operands = parameter_operands(&parameters);
                    let result = return_type.clone().map(|ty| super::abi::AbiOperand {
                        ty,
                        atomic: resolver.ctypes.quals(return_c).is_atomic,
                    });
                    let abi = super::abi::AbiClassifier::new(&resolver, &module.target)
                        .from_parts(
                            result.as_ref(),
                            &parameter_operands,
                            matches!(
                                &parameters,
                                crate::ir::Parameters::Prototype { variadic: true, .. }
                            ),
                            parameter_operands.len(),
                        )?;
                    let lowered = declarator.derive(Function {
                        id: BindingId(next_binding),
                        name: name.into(),
                        parameters,
                        return_type,
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
                    });
                    module.annotate(&lowered, resolver.render(return_c).entries());
                    module.functions.push(lowered);
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
                module.types.push(span.derive(definition.clone()));
            } else if let Some(declaration) = unit.decls.first() {
                module.types.push(declaration.derive(definition.clone()));
            }
        }
    }
    Ok(module)
}

fn parameter_operands(parameters: &crate::ir::Parameters) -> Vec<super::abi::AbiOperand> {
    match parameters {
        crate::ir::Parameters::Prototype { fixed, .. } => fixed
            .iter()
            .map(|parameter| super::abi::AbiOperand {
                ty: parameter.ty.clone(),
                atomic: parameter.access.atomic,
            })
            .collect(),
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
        let resolved = resolver.resolve_parameter(&parameter.specifiers, &parameter.declarator)?;
        let declared_array = parameter.declarator.array_parameter().unwrap_or_default();
        let shape = resolver.parameter_shape(resolved, declared_array)?;
        for definition in &resolver.definitions[start..] {
            module.types.push(parameter.derive(definition.clone()));
        }
        let lowered = parameter.derive(Parameter {
            id: BindingId(*next_binding),
            name: parameter.declarator.name().map(str::to_owned),
            ty: shape.ty,
            restrict: shape.qualifiers.is_restrict,
            is_const: shape.qualifiers.is_const,
            access: Access {
                volatile: shape.qualifiers.is_volatile,
                atomic: shape.qualifiers.is_atomic,
            },
            array: shape.array,
        });
        module.annotate(&lowered, resolver.render(resolved).entries());
        fixed.push(lowered);
        *next_binding += 1;
    }
    Ok(Parameters::Prototype {
        fixed,
        variadic: signature.is_variadic(),
    })
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
