use std::collections::{HashMap, HashSet};

use crate::ast::{
    AlignAsOperand, ArrayDeclarator, ArraySize, Attribute, DeclarationSpecifiers, Declarator,
    EnumItemKind, FieldItemKind, FixedPointKind, FixedPointRank, FloatingType, IntegerRank,
    IntegerType, ParameterList, Span, TagBody, TagDefinition, TagId, TagKind, TagSpecifier,
    TranslationUnit, TypeName, TypeOfOperand, TypeSpecifier,
};
use crate::compiler_args::{CompilerFlavor, LanguageStandard};
use crate::diagnostics::{DiagnosticContext, DiagnosticOptions, Warning};
use crate::ir::{
    Access, ArrayExtent, ArrayParameter, BindingId, BitFieldUnit, CallConv, Enumerator, Field,
    Number, NumericType, PointerSpace, RecordKind, RecordLayout, Type, TypeDefinition,
    TypeDefinitionKind, TypeId, Value, ValueKind,
};
use crate::target_info::{
    LongDoubleFormat, StorageLayout, TargetEnvironment, TargetFamily, TargetInfo,
};
use num_bigint::{BigInt, BigUint};

use super::attributes::{Subject, Use};
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
    pub(super) inferred: Option<QualType>,
    pub(super) enumerators: HashMap<crate::ast::NodeId, Operand>,
    pub(super) record_fields: HashMap<TypeId, Vec<QualType>>,
    field_alignments: HashMap<TypeId, Vec<u64>>,
    pub(super) pragmas: super::pragmas::Pragmas,
    pub(super) diagnostics: Vec<super::SemaError>,
    diagnostic_options: DiagnosticOptions,
    standard: LanguageStandard,
    prototype_scope: bool,
}

pub(super) enum Ordinary {
    Declared,
    Alias(QualType),
    Constant(Operand),
    Object {
        ty: QualType,
        alignment: Option<u64>,
    },
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
            inferred: None,
            enumerators: HashMap::new(),
            record_fields: HashMap::new(),
            field_alignments: HashMap::new(),
            pragmas: super::pragmas::Pragmas::default(),
            diagnostics: Vec::new(),
            diagnostic_options: DiagnosticOptions::default(),
            standard: LanguageStandard::default(),
            prototype_scope: false,
        }
    }

    pub fn ir_type(&self, q: QualType) -> Type {
        self.ctypes.ir_type(q, &self.target)
    }

    pub fn is_va_list(&mut self, ty: &Type) -> bool {
        let va_list = self.ctypes.va_list_type(&self.target);
        self.ir_type(va_list) == *ty
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
        resolver.ctypes.ptr32_extension_is_qualifier = unit.flavor == CompilerFlavor::Msvc;
        resolver.features =
            StandardFeatures::for_compiler(unit.standard, unit.flavor, &unit.target);
        resolver.tags = unit.tags.clone();
        resolver.pragmas = super::pragmas::collect(unit);
        resolver.diagnostic_options = unit.options.diagnostics.clone();
        resolver.standard = unit.standard;
        resolver
    }

    pub(super) fn warn<T>(&mut self, warning: Warning, message: &str, node: &Span<T>) {
        let diagnostics = DiagnosticContext {
            options: &self.diagnostic_options,
            standard: self.standard,
            flavor: self.flavor,
        };
        self.diagnostics.extend(warning.diagnose(
            message,
            diagnostics,
            node.provenance,
            node.expansion,
        ));
    }

    pub(super) fn check_attributes<'a>(
        &mut self,
        attributes: impl IntoIterator<Item = &'a Span<Attribute>>,
        subject: Subject,
    ) -> Result<(), ResolveError> {
        for attribute in attributes {
            match super::attributes::declaration_use(&attribute.value, subject) {
                Use::Unsupported(reason) => return Err(ResolveError::Unsupported(reason)),
                Use::Invalid(reason) => return Err(ResolveError::Invalid(reason)),
                Use::Inapplicable {
                    spelling,
                    applies_to,
                } => {
                    let message = match applies_to {
                        Some(subjects) => {
                            format!("'{spelling}' attribute ignored; it applies only to {subjects}")
                        }
                        None => format!("'{spelling}' attribute ignored"),
                    };
                    self.warn(Warning::IgnoredAttributes, &message, attribute);
                }
                Use::Unknown => {
                    if let Attribute::Unknown { name, .. } = &attribute.value
                        && !crate::attribute_support::spelling_registered(
                            name,
                            self.flavor,
                            &self.target,
                        )
                    {
                        let message = format!("unknown attribute '{name}' ignored");
                        self.warn(Warning::UnknownAttributes, &message, attribute);
                    }
                }
                Use::UnsupportedDeclspec => {
                    if let Attribute::IgnoredDeclspec { name, .. } = &attribute.value {
                        let message = format!("__declspec attribute '{name}' is not supported");
                        self.warn(Warning::IgnoredAttributes, &message, attribute);
                    }
                }
                Use::Symbol | Use::Layout | Use::Ignored => {}
            }
        }
        Ok(())
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

    fn machine_mode(&mut self, base: QualType, mode: &str) -> Result<QualType, ResolveError> {
        let name = mode
            .strip_prefix("__")
            .and_then(|name| name.strip_suffix("__"))
            .unwrap_or(mode);
        let mismatch =
            ResolveError::Invalid("type of machine mode does not match type of base type");
        let kind = match self.ctypes.canonical_kind(base).clone() {
            CTypeKind::Vector { element, bytes, .. } => {
                let element = self.machine_mode(element, mode)?;
                let element_bytes = self.storage(self.ir_type(element))?.size_bytes;
                if bytes % element_bytes != 0 {
                    return Err(ResolveError::Invalid(
                        "vector size is not a multiple of the machine mode size",
                    ));
                }
                let lanes = u32::try_from(bytes / element_bytes)
                    .map_err(|_| ResolveError::Invalid("vector lane count is too large"))?;
                CTypeKind::Vector {
                    element,
                    lanes,
                    bytes,
                }
            }
            CTypeKind::Float(_) => {
                let long_double = self.target.long_double;
                CTypeKind::Float(match name {
                    "SF" => FloatKind::Float,
                    "DF" => FloatKind::Double,
                    "XF" if long_double == LongDoubleFormat::X87 => FloatKind::LongDouble,
                    "TF" if long_double == LongDoubleFormat::Binary128 => FloatKind::LongDouble,
                    "TF" if self.target.has_float128() => FloatKind::Float128,
                    "XF" | "TF" => {
                        return Err(ResolveError::Invalid("unsupported machine mode"));
                    }
                    "QI" | "HI" | "SI" | "DI" | "TI" | "byte" | "word" | "pointer" => {
                        return Err(mismatch);
                    }
                    _ => return Err(ResolveError::Unsupported("machine mode")),
                })
            }
            kind => {
                let signed = match kind {
                    CTypeKind::Bool | CTypeKind::UChar => false,
                    CTypeKind::SChar => true,
                    CTypeKind::Char => self.target.char_signed,
                    CTypeKind::Int { signed, .. } => signed,
                    CTypeKind::Complex(_) => {
                        return Err(ResolveError::Unsupported("complex machine mode"));
                    }
                    CTypeKind::Enum(_) => {
                        let underlying = self
                            .ctypes
                            .enum_underlying(base)
                            .ok_or(ResolveError::Invalid("incomplete enum type"))?;
                        return self.machine_mode(underlying.with(base.quals), mode);
                    }
                    _ => {
                        return Err(ResolveError::Invalid(
                            "mode attribute only supported for integer and floating-point types",
                        ));
                    }
                };
                let width = match name {
                    "QI" | "byte" => 8,
                    "HI" => 16,
                    "SI" => 32,
                    "DI" => 64,
                    "TI" => 128,
                    "word" | "pointer" => self.target.pointer_width,
                    "SF" | "DF" | "XF" | "TF" => return Err(mismatch),
                    _ => return Err(ResolveError::Unsupported("machine mode")),
                };
                self.integer_of_width(width, signed)?
            }
        };
        Ok(self.ctypes.qual(kind).with(base.quals))
    }

    // clang's getIntTypeByWidth order: the first standard type of that width wins
    fn integer_of_width(&self, width: u32, signed: bool) -> Result<CTypeKind, ResolveError> {
        let target = &self.target;
        let rank = if width == 8 {
            return Ok(if signed {
                CTypeKind::SChar
            } else {
                CTypeKind::UChar
            });
        } else if width == target.short_width {
            IntRank::Short
        } else if width == target.int_width {
            IntRank::Int
        } else if width == target.long_width {
            IntRank::Long
        } else if width == target.long_long_width {
            IntRank::LongLong
        } else if width == 128 {
            IntRank::Int128
        } else {
            return Err(ResolveError::Invalid(
                "no integer type of the machine mode's width",
            ));
        };
        Ok(CTypeKind::Int { rank, signed })
    }

    pub(super) fn constant_integer(
        &mut self,
        e: &crate::ast::Expr,
    ) -> Result<BigInt, ResolveError> {
        let value = self.constant_value(e)?;
        super::fold::integer_constant(&value, self.flavor).ok_or(ResolveError::Unsupported(
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

    pub(super) fn lookup(&self, name: &str) -> Option<&Ordinary> {
        self.ordinary.iter().rev().find_map(|scope| scope.get(name))
    }

    pub(super) fn lookup_local(&self, name: &str) -> Option<&Ordinary> {
        self.ordinary.last().and_then(|scope| scope.get(name))
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
                if matches!(e.value, ExprKind::SizeOfExpr(_)) && self.ctypes.is_function(ty) {
                    let c = self.ctypes.size_type(&self.target);
                    return Ok(Operand {
                        c,
                        value: Value {
                            ty: self.ir_type(c),
                            node: e.derive(ValueKind::Constant(Number::Integer(1u8.into()))),
                        },
                    });
                }
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
                let layout = self.declared_storage(ty, layout)?;
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
                let layout = self.declared_storage(resolved, layout)?;
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

    pub(super) fn object_alignment(&mut self, e: &crate::ast::Expr, natural: u64) -> u64 {
        let mut operand = e;
        while let crate::ast::ExprKind::Paren(inner) = &operand.value {
            operand = inner;
        }
        if let crate::ast::ExprKind::Member { base, field, arrow } = &operand.value {
            let record = self.assertion_operand_type(base).ok().and_then(|base| {
                if *arrow {
                    self.ctypes.pointee(base)
                } else {
                    Some(base)
                }
            });
            return record
                .and_then(|record| self.field_alignment(record, &field.value))
                .unwrap_or(natural);
        }
        let requested = match (self.references.get(&operand.id), &operand.value) {
            (Some(id), _) => self.entities.request(id).alignment,
            (None, crate::ast::ExprKind::Identifier(name)) => match self.lookup(name) {
                Some(Ordinary::Object { alignment, .. }) => *alignment,
                _ => None,
            },
            (None, _) => None,
        };
        let Some(requested) = requested else {
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
            GenericControl::Type { ty } => self.resolve(&ty.specifiers, &ty.declarator)?,
            GenericControl::Expr(expr) => {
                let ty = self.assertion_operand_type(expr)?;
                self.ctypes.lvalue_conversion(ty)
            }
        };
        self.select_association(controlling, associations)
    }

    // c2y matches a type-name operand as written; only an expression operand is lvalue-converted.
    pub(super) fn select_association<'e>(
        &mut self,
        controlling: QualType,
        associations: &'e [crate::ast::GenericAssociation],
    ) -> Result<&'e crate::ast::Expr, ResolveError> {
        use crate::ast::GenericAssociation;
        let mut selected = None;
        let mut fallback = None;
        for association in associations {
            match association {
                GenericAssociation::Default(value) => fallback = Some(value),
                GenericAssociation::Type { ty, value } => {
                    let ty = self.resolve(&ty.specifiers, &ty.declarator)?;
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
                Some(Ordinary::Object { ty, .. }) => Ok(*ty),
                Some(Ordinary::Constant(value)) => Ok(value.c),
                _ => Err(ResolveError::Unsupported(
                    "unknown or unsupported sizeof operand type",
                )),
            },
            ExprKind::StringLiteral(literal) => Ok(self.string_type(literal)),
            ExprKind::NullPtrLiteral => Ok(self.ctypes.qual(CTypeKind::NullPtr)),
            ExprKind::CompoundLiteral { ty, initializer } => {
                let resolved = self.resolve(&ty.specifiers, &ty.declarator)?;
                if let Some((element, Extent::Incomplete)) = self.ctypes.element(resolved) {
                    let length = self.inferred_array_length(element, initializer)?;
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
        if self.ctypes.is_function(previous)
            && self.ctypes.function_convention(previous)
                != self.ctypes.function_convention(declared)
        {
            return Err(ResolveError::Invalid(
                "function redeclared with a different calling convention",
            ));
        }
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

    fn field_alignment(&self, q: QualType, name: &str) -> Option<u64> {
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
        let alignments = self.field_alignments.get(id)?;
        for (index, field) in fields.iter().enumerate() {
            if field.name.as_deref() == Some(name) {
                return alignments.get(index).copied();
            }
            if field.name.is_none()
                && let Some(found) = self.field_alignment(*types.get(index)?, name)
            {
                return Some(found);
            }
        }
        None
    }

    pub(super) fn union_cast_member(
        &self,
        to: QualType,
        from: QualType,
    ) -> Result<Option<usize>, ResolveError> {
        let CTypeKind::Record { id, union: true } = self.ctypes.canonical_kind(to) else {
            return Ok(None);
        };
        if self.ctypes.compatible_unqualified(from, to) {
            return Ok(None);
        }
        let TypeDefinitionKind::Record {
            fields: Some(fields),
            ..
        } = &self.definitions[id.0 as usize].kind
        else {
            return Err(ResolveError::Invalid("cast to incomplete union type"));
        };
        let from = self.ctypes.unqualified_view(from);
        fields
            .iter()
            .zip(self.record_fields.get(id).into_iter().flatten())
            .position(|(field, member)| {
                field.name.is_some() && self.ctypes.unqualified_view(*member) == from
            })
            .map(Some)
            .ok_or(ResolveError::Invalid(
                "cast to union type from type not present in union",
            ))
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

    pub(super) fn field_offset(&self, ty: &Type, index: usize) -> Option<u64> {
        let Type::Defined(id) = ty else {
            return None;
        };
        let Some(TypeDefinition {
            kind:
                TypeDefinitionKind::Record {
                    layout: Some(layout),
                    ..
                },
            ..
        }) = self.definitions.get(id.0 as usize)
        else {
            return None;
        };
        layout.offsets.get(index).copied()
    }

    pub(super) fn offsetof_field(&self, ty: Type, name: &str) -> Result<(Type, u64), ResolveError> {
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

    pub(super) fn ms_asm_field(&self, name: &str) -> Result<(Type, u64), ResolveError> {
        let mut shadowed = HashSet::new();
        let mut records = Vec::new();
        for scope in self.tag_names.iter().rev() {
            for ((kind, tag), id) in scope {
                if *kind != TagKind::Enum && shadowed.insert(tag.as_str()) {
                    self.with_anonymous_records(*id, &mut records);
                }
            }
        }
        let mut hits = records
            .into_iter()
            .filter_map(|id| self.flattened_field(id, name));
        match (hits.next(), hits.next()) {
            (Some(hit), None) => Ok(hit),
            (None, _) => Err(ResolveError::Invalid(
                "illegal struct/union member in `__asm`",
            )),
            (Some(_), Some(_)) => Err(ResolveError::Invalid("ambiguous member name in `__asm`")),
        }
    }

    fn record_fields(&self, id: TypeId) -> Option<(&[Span<Field>], &[u64])> {
        match self.definitions.get(id.0 as usize) {
            Some(TypeDefinition {
                kind:
                    TypeDefinitionKind::Record {
                        fields: Some(fields),
                        layout: Some(layout),
                        ..
                    },
                ..
            }) => Some((fields, &layout.offsets)),
            _ => None,
        }
    }

    fn with_anonymous_records(&self, id: TypeId, records: &mut Vec<TypeId>) {
        if records.contains(&id) {
            return;
        }
        records.push(id);
        for field in self.record_fields(id).map_or(&[][..], |(fields, _)| fields) {
            if let (None, Type::Defined(inner)) = (&field.name, &field.ty) {
                self.with_anonymous_records(*inner, records);
            }
        }
    }

    fn flattened_field(&self, id: TypeId, name: &str) -> Option<(Type, u64)> {
        let (fields, offsets) = self.record_fields(id)?;
        fields
            .iter()
            .zip(offsets)
            .find_map(|(field, offset)| match (&field.name, &field.ty) {
                (Some(field_name), ty) if field_name == name && field.bit_width.is_none() => {
                    Some((ty.clone(), *offset))
                }
                (None, Type::Defined(inner)) => self
                    .flattened_field(*inner, name)
                    .map(|(ty, inner_offset)| (ty, offset + inner_offset)),
                _ => None,
            })
    }

    pub fn define_alias<'a>(
        &mut self,
        name: String,
        resolved: QualType,
        attributes: impl IntoIterator<Item = &'a Span<Attribute>>,
    ) -> Result<(), ResolveError> {
        let previous = match self.lookup_local(&name) {
            Some(Ordinary::Alias(previous)) => self.ctypes.typedef_alignment(*previous),
            _ => None,
        };
        let alignment = requested_alignment(self, attributes)?.max(previous);
        let ty = self.ir_type(resolved);
        let id = self.push(TypeDefinitionKind::Alias(ty));
        self.definitions[id.0 as usize].name = Some(name.clone());
        let alias = self.ctypes.qual(CTypeKind::Typedef {
            name: name.clone(),
            underlying: resolved,
            alignment,
        });
        self.declare(&name, Ordinary::Alias(alias));
        Ok(())
    }

    // declarator-position type attributes change only their own declarator's base type
    pub fn resolve_declarator(
        &mut self,
        specifiers: &DeclarationSpecifiers,
        declarator: &Declarator,
        attributes: &[Span<Attribute>],
    ) -> Result<QualType, ResolveError> {
        let own = declarator
            .grouped_attributes()
            .into_iter()
            .chain(attributes)
            .collect::<Vec<_>>();
        let resolved = if !own
            .iter()
            .any(|attribute| attribute.value.is_type_attribute())
        {
            self.resolve(specifiers, declarator)?
        } else {
            let specifiers = DeclarationSpecifiers {
                ty: specifiers.ty.clone().with_type_attributes(own),
                ..specifiers.clone()
            };
            self.resolve(&specifiers, declarator)?
        };
        Ok(self.apply_convention(resolved, attributes))
    }

    pub(super) fn calling_convention<'a>(
        &self,
        attributes: impl IntoIterator<Item = &'a Span<Attribute>>,
    ) -> Option<CallConv> {
        let x86 = match self.target.family {
            TargetFamily::X86 => true,
            TargetFamily::X86_64 => false,
            TargetFamily::AArch64 | TargetFamily::Arm32 => return None,
        };
        attributes
            .into_iter()
            .filter_map(|attribute| match &attribute.value {
                Attribute::CallingConvention(convention) => match convention {
                    crate::ast::CallingConvention::Cdecl => Some(CallConv::C),
                    crate::ast::CallingConvention::Vectorcall => Some(CallConv::X86Vectorcall),
                    crate::ast::CallingConvention::Stdcall if x86 => Some(CallConv::X86Stdcall),
                    crate::ast::CallingConvention::Fastcall if x86 => Some(CallConv::X86Fastcall),
                    crate::ast::CallingConvention::Thiscall if x86 => Some(CallConv::X86Thiscall),
                    _ => None,
                },
                _ => None,
            })
            .last()
    }

    pub(super) fn inherit_convention(&mut self, id: BindingId, declared: QualType) -> QualType {
        let Some(previous) = self.entities.ty(&id) else {
            return declared;
        };
        let convention = self.ctypes.function_convention(previous);
        if convention == CallConv::C || self.ctypes.function_convention(declared) != CallConv::C {
            return declared;
        }
        self.ctypes
            .with_convention(declared, convention)
            .unwrap_or(declared)
    }

    pub(super) fn apply_convention<'a>(
        &mut self,
        q: QualType,
        attributes: impl IntoIterator<Item = &'a Span<Attribute>>,
    ) -> QualType {
        match self.calling_convention(attributes) {
            Some(convention) => self.ctypes.with_convention(q, convention).unwrap_or(q),
            None => q,
        }
    }

    pub fn resolve(
        &mut self,
        specifiers: &DeclarationSpecifiers,
        declarator: &Declarator,
    ) -> Result<QualType, ResolveError> {
        // the parser folds `mode` into the specifier, but clang applies it to the declared type
        if declarator.is_derived() && has_machine_mode(&specifiers.ty) {
            return Err(ResolveError::Invalid(
                "mode attribute only supported for integer and floating-point types",
            ));
        }
        let base = self.base(&specifiers.ty)?;
        let base = self.qualify(base, &specifiers.qualifiers)?;
        let convention = self.calling_convention(&specifiers.attributes);
        let resolved = match convention {
            Some(convention) if has_function_declarator(declarator) => {
                self.derive(declarator, base, Some(convention))?
            }
            Some(convention) => {
                let base = self
                    .ctypes
                    .with_convention(base, convention)
                    .unwrap_or(base);
                self.derive(declarator, base, None)?
            }
            None => self.derive(declarator, base, None)?,
        };
        Ok(if specifiers.is_constexpr {
            resolved.with(Qualifiers::CONST)
        } else {
            resolved
        })
    }

    fn base(&mut self, specifier: &TypeSpecifier) -> Result<QualType, ResolveError> {
        let kind = match specifier {
            TypeSpecifier::Void => CTypeKind::Void,
            TypeSpecifier::Inferred => {
                return self.inferred.take().ok_or(ResolveError::Invalid(
                    "'auto' type inference is not allowed here",
                ));
            }
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
                FloatingType::BFloat16 => FloatKind::BFloat16,
                FloatingType::Float16 => FloatKind::Float16,
                FloatingType::Fp16 => FloatKind::Fp16,
                FloatingType::Float => FloatKind::Float,
                FloatingType::Double => FloatKind::Double,
                FloatingType::LongDouble => FloatKind::LongDouble,
                FloatingType::Float32 => FloatKind::Float32,
                FloatingType::Float64 => FloatKind::Float64,
                FloatingType::Float32x => FloatKind::Float32x,
                FloatingType::Float64x if self.target.float64x_format().is_some() => {
                    FloatKind::Float64x
                }
                FloatingType::Float128 if self.target.has_float128() => FloatKind::Float128,
                FloatingType::Float128Ext => FloatKind::Float128,
                FloatingType::Float80 if self.target.long_double == LongDoubleFormat::X87 => {
                    FloatKind::LongDouble
                }
                FloatingType::Decimal32 => FloatKind::Decimal32,
                FloatingType::Decimal64 => FloatKind::Decimal64,
                FloatingType::Decimal128 => FloatKind::Decimal128,
                FloatingType::Float64x | FloatingType::Float128 | FloatingType::Float80 => {
                    return Err(ResolveError::Invalid(
                        "floating type is not supported on this target",
                    ));
                }
            }),
            TypeSpecifier::Mode(mode) => {
                let base = self.base(&mode.base)?;
                return self.machine_mode(base, &mode.mode);
            }
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
            TypeSpecifier::TargetBuiltin(name) if name == "__builtin_va_list" => {
                return Ok(self.ctypes.va_list_type(&self.target));
            }
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

    fn derive(
        &mut self,
        declarator: &Declarator,
        q: QualType,
        innermost: Option<CallConv>,
    ) -> Result<QualType, ResolveError> {
        match declarator {
            Declarator::Name(_) | Declarator::Abstract => Ok(q),
            Declarator::Grouped(inner) => self.derive(inner, q, innermost),
            Declarator::Attributed { inner, attributes } => {
                let q = self.apply_convention(q, attributes);
                self.derive(inner, q, innermost)
            }
            Declarator::Pointer {
                inner,
                qualifiers,
                attributes,
            } => {
                let q = self.apply_convention(q, attributes);
                let pointer = self.ctypes.pointer(q);
                let q = self.qualify(pointer, qualifiers)?;
                self.derive(inner, q, innermost)
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
                let (core, mut q) = self.apply_pointers(core, q)?;
                for extent in extents {
                    if self.ctypes.is_void(q) {
                        return Err(ResolveError::Unsupported("void array element"));
                    }
                    if self.ctypes.typedef_alignment(q).is_some()
                        && let Ok(natural) = self.storage(self.ir_type(q))
                    {
                        let storage = self.typedef_storage(q, natural)?;
                        if storage.size_bytes % u64::from(storage.alignment_bytes) != 0 {
                            return Err(ResolveError::Invalid(
                                "array element size is not a multiple of its alignment",
                            ));
                        }
                    }
                    q = self.ctypes.qual(CTypeKind::Array { element: q, extent });
                }
                self.derive(core, q, innermost)
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
                        params.push(match parameters {
                            ParameterList::IdentifierList { .. } => {
                                this.promoted_parameter(resolved)
                            }
                            _ => resolved,
                        });
                    }
                    Ok(params)
                })?;
                let prototyped = match parameters {
                    ParameterList::IdentifierList { .. } => false,
                    ParameterList::Empty => self.features.empty_parens_are_prototype,
                    ParameterList::Prototype { .. } | ParameterList::Void => true,
                };
                let (core, ret) = self.apply_pointers(inner, q)?;
                let q = self.ctypes.qual(CTypeKind::Function {
                    ret,
                    params,
                    variadic: parameters.is_variadic(),
                    prototyped,
                    convention: CallConv::C,
                });
                let q = match innermost {
                    Some(convention) if !has_function_declarator(core) => {
                        self.ctypes.with_convention(q, convention).unwrap_or(q)
                    }
                    _ => q,
                };
                self.derive(core, q, innermost)
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

    pub(super) fn promoted_parameter(&mut self, declared: QualType) -> QualType {
        let adjusted = self.ctypes.adjust_parameter(declared);
        let promoted = self.ctypes.default_promotion(adjusted, &self.target);
        self.ctypes.unqualified(promoted)
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
    ) -> Result<(&'d Declarator, QualType), ResolveError> {
        while let Declarator::Pointer {
            inner,
            qualifiers,
            attributes,
        } = core
        {
            q = self.apply_convention(q, attributes);
            let pointer = self.ctypes.pointer(q);
            q = self.qualify(pointer, qualifiers)?;
            core = inner;
        }
        Ok((core, q))
    }

    fn qualify(
        &mut self,
        q: QualType,
        qualifiers: &crate::ast::Qualifiers,
    ) -> Result<QualType, ResolveError> {
        let q = q.with((*qualifiers).into());
        let Some(space) = self.pointer_space(qualifiers)? else {
            return Ok(q);
        };
        if space == PointerSpace::Default && self.ctypes.is_pointer(q) {
            return Ok(q);
        }
        self.ctypes
            .pointer_in(q, space)
            .ok_or(ResolveError::Invalid(
                "__ptr32, __ptr64, __sptr and __uptr only apply to pointers",
            ))
    }

    fn pointer_space(
        &self,
        qualifiers: &crate::ast::Qualifiers,
    ) -> Result<Option<PointerSpace>, ResolveError> {
        let crate::ast::Qualifiers {
            is_ptr32,
            is_ptr64,
            is_sptr,
            is_uptr,
            ..
        } = *qualifiers;
        if !(is_ptr32 || is_ptr64 || is_sptr || is_uptr) {
            return Ok(None);
        }
        if is_ptr32 && is_ptr64 {
            return Err(ResolveError::Invalid(
                "'__ptr32' and '__ptr64' attributes are not compatible",
            ));
        }
        if is_sptr && is_uptr {
            return Err(ResolveError::Invalid(
                "'__sptr' and '__uptr' attributes are not compatible",
            ));
        }
        Ok(Some(match self.target.pointer_width {
            32 if is_ptr64 => PointerSpace::Ptr64,
            32 if is_uptr => PointerSpace::Ptr32Uptr,
            64 if is_ptr32 && is_uptr => PointerSpace::Ptr32Uptr,
            64 if is_ptr32 => PointerSpace::Ptr32Sptr,
            _ => PointerSpace::Default,
        }))
    }

    /// C11 6.7.2.3p8: `struct S;` alone declares an incomplete tag in the
    /// current scope, hiding any outer one, rather than referring outward.
    pub(super) fn declare_forward_tag(&mut self, specifiers: &DeclarationSpecifiers) -> bool {
        let TypeSpecifier::Tag(TagSpecifier::Reference {
            kind,
            name,
            fixed_type: None,
        }) = &specifiers.ty
        else {
            return false;
        };
        let key = (*kind, name.to_owned());
        if self
            .tag_names
            .last()
            .is_some_and(|scope| scope.contains_key(&key))
        {
            return true;
        }
        let id = self.push(incomplete_tag(*kind));
        self.definitions[id.0 as usize].name = Some(name.to_owned());
        self.declare_tag(key, id);
        true
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
                let union = tag.kind == TagKind::Union;
                self.check_attributes(&tag.attributes, Subject::Record { union })?;
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
                        self.check_attributes(&declaration.specifiers.attributes, Subject::Field)?;
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
                        self.check_attributes(
                            declaration
                                .specifiers
                                .attributes
                                .iter()
                                .chain(&declarator.attributes),
                            Subject::Field,
                        )?;
                        let resolved = self.resolve_declarator(
                            &declaration.specifiers,
                            &declarator.declarator,
                            &declarator.attributes,
                        )?;
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
                    .any(|attribute| matches!(&attribute.value, Attribute::Packed));
                let has = |wanted: &Attribute| {
                    tag.attributes
                        .iter()
                        .any(|attribute| &attribute.value == wanted)
                };
                let ms_struct = has(&Attribute::MsStruct)
                    || (self.pragmas.is_ms_struct(tag.id) && !has(&Attribute::GccStruct));
                let max_field_alignment = self.pragmas.max_field_alignment(tag.id);
                let alignment = requested_alignment(self, &tag.attributes)?;
                let (layout, field_alignments) = self.layout_record(
                    tag.kind,
                    &fields,
                    &field_types,
                    &requests,
                    RecordRules {
                        packed,
                        ms_struct,
                        max_field_alignment,
                    },
                    alignment,
                )?;
                self.record_fields.insert(id, field_types);
                self.field_alignments.insert(id, field_alignments);
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
            let same_member = if self.flavor == CompilerFlavor::Clang {
                CTypes::same_or_enum_underlying
            } else {
                CTypes::same
            };
            if self.features.compatible_tag_redefinitions
                && same_tag_shape(&self.definitions[id.0 as usize].kind, &kind)
                && self.same_field_types(id, redefined_fields.as_deref(), &kind, same_member)
            {
                return Ok(id);
            }
            return Err(ResolveError::Invalid(
                "redefinition of struct, union, or enum tag",
            ));
        }
        self.definitions[id.0 as usize].kind = kind;
        if self.features.compatible_tag_redefinitions {
            self.join_compatible_tag(id);
        }
        Ok(id)
    }

    fn join_compatible_tag(&mut self, id: TypeId) {
        let Some(name) = self.definitions[id.0 as usize].name.clone() else {
            return;
        };
        let candidates: Vec<TypeId> = self
            .definitions
            .iter()
            .filter(|definition| definition.id != id && definition.name.as_ref() == Some(&name))
            .map(|definition| definition.id)
            .collect();
        for candidate in candidates {
            self.ctypes.join_tag_class(id, candidate);
            let (earlier, current) = (
                &self.definitions[candidate.0 as usize].kind,
                &self.definitions[id.0 as usize].kind,
            );
            if same_tag_shape(earlier, current)
                && self.same_field_types(
                    candidate,
                    self.record_fields.get(&id).map(Vec::as_slice),
                    current,
                    CTypes::compatible,
                )
            {
                return;
            }
            self.ctypes.leave_tag_class(id);
        }
    }

    fn same_field_types(
        &self,
        id: TypeId,
        redefined: Option<&[QualType]>,
        kind: &TypeDefinitionKind,
        same_member: fn(&CTypes, QualType, QualType) -> bool,
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
                .all(|(a, b)| same_member(&self.ctypes, *a, *b))
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
            Type::Void | Type::Function { .. } => Ok(()),
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

    fn atomic_layout(&self, layout: StorageLayout) -> StorageLayout {
        match self.flavor {
            CompilerFlavor::Gcc => layout,
            CompilerFlavor::Clang => self.target.atomic_storage(layout),
            CompilerFlavor::Msvc => self.target.msvc_atomic_storage(layout),
        }
    }

    pub(super) fn effective_alignment(&self, requested: u64, natural: u64) -> u64 {
        if matches!(self.flavor, CompilerFlavor::Clang) {
            requested
        } else {
            requested.max(natural)
        }
    }

    pub(super) fn object_alignment_override(
        &self,
        ty: &Type,
        declared: Option<QualType>,
        requested: Option<u64>,
    ) -> Result<Option<u64>, ResolveError> {
        let large_array = (declared.is_some()
            && matches!(
                ty,
                Type::Array {
                    length: Some(_),
                    ..
                }
            ))
        .then(|| self.target.large_array_alignment())
        .flatten();
        let typedef_aligned = declared.is_some_and(|q| self.ctypes.typedef_alignment(q).is_some());
        let unaligned = declared.is_some_and(|q| self.is_unaligned(q));
        if requested.is_none() && large_array.is_none() && !typedef_aligned && !unaligned {
            return Ok(None);
        }
        let storage = self.storage(ty.clone())?;
        let natural = u64::from(storage.alignment_bytes);
        let typed = match declared {
            Some(q) => u64::from(self.declared_storage(q, storage)?.alignment_bytes),
            None => natural,
        };
        let effective = match (requested, large_array) {
            (Some(requested), _) => self.effective_alignment(requested, typed),
            (None, Some(large)) if storage.size_bytes >= large => typed.max(large),
            (None, _) => typed,
        };
        Ok((effective != natural).then_some(effective))
    }

    pub(super) fn declared_alignment(&self, requested: u64, natural: u64) -> u64 {
        if matches!(self.flavor, CompilerFlavor::Msvc) {
            requested.max(natural)
        } else {
            requested
        }
    }

    pub(super) fn typedef_storage(
        &self,
        q: QualType,
        layout: StorageLayout,
    ) -> Result<StorageLayout, ResolveError> {
        let Some(requested) = self.ctypes.typedef_alignment(q) else {
            return Ok(layout);
        };
        let alignment = self.declared_alignment(requested, u64::from(layout.alignment_bytes));
        Ok(StorageLayout {
            alignment_bytes: u32::try_from(alignment)
                .map_err(|_| ResolveError::Unsupported("typedef alignment overflow"))?,
            ..layout
        })
    }

    // unlike an aligned typedef, __unaligned leaves record member layout alone
    pub(super) fn declared_storage(
        &self,
        q: QualType,
        layout: StorageLayout,
    ) -> Result<StorageLayout, ResolveError> {
        let layout = self.typedef_storage(q, layout)?;
        if self.is_unaligned(q) {
            return Ok(StorageLayout {
                alignment_bytes: 1,
                ..layout
            });
        }
        Ok(layout)
    }

    // cl.exe only honours __unaligned on pointer types
    fn is_unaligned(&self, q: QualType) -> bool {
        self.ctypes.quals(q).is_unaligned
            && (self.flavor != CompilerFlavor::Msvc || self.ctypes.is_pointer(q))
    }

    pub(super) fn qualified_storage(
        &self,
        ty: Type,
        atomic: bool,
    ) -> Result<StorageLayout, ResolveError> {
        let promote = |layout| {
            if atomic {
                self.atomic_layout(layout)
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
            Type::Pointer { space, .. } => Ok(promote(self.target.pointer_storage(space))),
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

    fn ms_builtin_size(&self, ty: &Type) -> Result<Option<u64>, ResolveError> {
        match ty {
            Type::Array { element, .. } => self.ms_builtin_size(element),
            Type::Defined(id) => match &self.definitions[id.0 as usize].kind {
                TypeDefinitionKind::Alias(inner) => self.ms_builtin_size(inner),
                _ => Ok(None),
            },
            Type::Bool
            | Type::FixedPoint(_)
            | Type::Numeric(NumericType::Float(_))
            | Type::Numeric(NumericType::Integer {
                bit_precise: false, ..
            }) => {
                let size = self.target.storage_of(ty.clone())?.size_bytes;
                if size.is_power_of_two() {
                    Ok(Some(size))
                } else {
                    Err(ResolveError::Invalid(
                        "ms_struct layout of a fundamental type whose size is not a power of two",
                    ))
                }
            }
            _ => Ok(None),
        }
    }

    fn layout_record(
        &self,
        kind: TagKind,
        fields: &[crate::ast::Span<Field>],
        field_types: &[QualType],
        requests: &[(bool, Option<u64>)],
        rules: RecordRules,
        requested: Option<u64>,
    ) -> Result<(RecordLayout, Vec<u64>), ResolveError> {
        if self.target.environment == TargetEnvironment::Msvc {
            return self.layout_microsoft_record(
                kind,
                fields,
                field_types,
                requests,
                rules,
                requested,
            );
        }
        let RecordRules {
            packed,
            ms_struct,
            max_field_alignment,
        } = rules;
        let mut end_bits = 0u64;
        let mut aggregate_align = requested.unwrap_or(1);
        let mut required_align = requested;
        let mut offsets = Vec::new();
        let mut bit_offsets = Vec::new();
        let mut unit_sizes = Vec::new();
        let mut field_alignments = Vec::new();
        let mut ms_unit = MsUnit::default();
        for (position, ((field, &c), &(field_packed, field_aligned))) in
            fields.iter().zip(field_types).zip(requests).enumerate()
        {
            let storage = self.member_storage(kind, fields.len(), position, field)?;
            let storage = self.typedef_storage(c, storage)?;
            required_align = required_align
                .into_iter()
                .chain(field_aligned)
                .chain(self.required_alignment(&field.ty))
                .max();
            unit_sizes.push(storage.size_bytes);
            let unit_bits = checked_bit_width(field, storage)?;
            let mut natural = u64::from(storage.alignment_bytes);
            if ms_struct && !field.access.atomic {
                natural = natural.max(self.ms_builtin_size(&field.ty)?.unwrap_or(1));
            }
            if ms_struct && let Some(width) = field.bit_width {
                let (position, align) = ms_unit.place(
                    kind,
                    &mut end_bits,
                    u64::from(width),
                    unit_bits,
                    field_aligned,
                    max_field_alignment,
                )?;
                aggregate_align = aggregate_align.max(align);
                field_alignments.push(align);
                offsets.push(position / 8);
                bit_offsets.push(Some(position));
                continue;
            }
            ms_unit = MsUnit::default();
            let align = (if packed || field_packed { 1 } else { natural })
                .max(field_aligned.unwrap_or(1))
                .min(max_field_alignment.unwrap_or(u64::MAX));
            field_alignments.push(align);
            if field.bit_width == Some(0) && self.target.abi.zero_width_bitfield_aligns_record {
                aggregate_align = aggregate_align.max(natural);
            } else if field.bit_width != Some(0) {
                aggregate_align = aggregate_align.max(align);
            }
            if let Some(width) = field.bit_width {
                if width == 0 {
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
        let (bit_units, field_units) =
            bit_field_units(kind, fields, &bit_offsets, &unit_sizes, ms_struct);
        Ok((
            RecordLayout {
                size,
                align: aggregate_align,
                required_align,
                offsets,
                bit_offsets,
                bit_units,
                field_units,
            },
            field_alignments,
        ))
    }

    fn member_storage(
        &self,
        kind: TagKind,
        count: usize,
        position: usize,
        field: &Field,
    ) -> Result<StorageLayout, ResolveError> {
        match &field.ty {
            Type::Array {
                element,
                length: None,
            } if kind == TagKind::Union || position + 1 == count => Ok(StorageLayout {
                size_bytes: 0,
                alignment_bytes: self
                    .qualified_storage((**element).clone(), field.access.atomic)?
                    .alignment_bytes,
            }),
            ty => self.qualified_storage(ty.clone(), field.access.atomic),
        }
    }

    // clang's MicrosoftRecordLayoutBuilder, which clang uses for every MSVC-environment target, ms_struct or not
    fn layout_microsoft_record(
        &self,
        kind: TagKind,
        fields: &[crate::ast::Span<Field>],
        field_types: &[QualType],
        requests: &[(bool, Option<u64>)],
        rules: RecordRules,
        requested: Option<u64>,
    ) -> Result<(RecordLayout, Vec<u64>), ResolveError> {
        let union = kind == TagKind::Union;
        let max_field_alignment = if rules.packed {
            Some(1)
        } else {
            rules
                .max_field_alignment
                .filter(|pack| pack * 8 <= u64::from(self.target.pointer_width))
        };
        let mut size = 0u64;
        let mut alignment = 1u64;
        let mut required: Option<u64> = None;
        let mut after_bit_field = false;
        let mut unit_size = 0u64;
        let mut remaining_bits = 0u64;
        let mut offsets = Vec::new();
        let mut bit_offsets = Vec::new();
        let mut unit_sizes = Vec::new();
        let mut field_alignments = Vec::new();
        for (position, ((field, &c), &(field_packed, field_aligned))) in
            fields.iter().zip(field_types).zip(requests).enumerate()
        {
            let storage = self.member_storage(kind, fields.len(), position, field)?;
            let unit_bits = checked_bit_width(field, storage)?;
            unit_sizes.push(storage.size_bytes);
            let typedef_alignment = match self.ctypes.typedef_alignment(c) {
                Some(_) => Some(u64::from(self.typedef_storage(c, storage)?.alignment_bytes)),
                None => None,
            };
            let mut field_required = field_aligned.into_iter().chain(typedef_alignment).max();
            let mut field_alignment = u64::from(storage.alignment_bytes);
            if field.bit_width.is_some() {
                field_alignment = field_alignment.max(field_required.unwrap_or(1));
            } else {
                field_required = field_required
                    .into_iter()
                    .chain(self.required_alignment(&field.ty))
                    .max();
                required = required.into_iter().chain(field_required).max();
            }
            if let Some(max) = max_field_alignment {
                field_alignment = field_alignment.min(max);
            }
            if field_packed {
                field_alignment = 1;
            }
            let field_alignment = field_alignment.max(field_required.unwrap_or(1));
            field_alignments.push(field_alignment);
            let bit_offset = match field.bit_width {
                None => {
                    after_bit_field = false;
                    alignment = alignment.max(field_alignment);
                    let offset = if union {
                        0
                    } else {
                        align_up(size, field_alignment)?
                    };
                    size = size.max(
                        offset
                            .checked_add(storage.size_bytes)
                            .ok_or(ResolveError::Unsupported("record size overflow"))?,
                    );
                    offsets.push(offset);
                    bit_offsets.push(None);
                    continue;
                }
                Some(0) if !after_bit_field => {
                    if union {
                        0
                    } else {
                        size * 8
                    }
                }
                Some(0) => {
                    after_bit_field = false;
                    if union {
                        size = size.max(storage.size_bytes);
                        0
                    } else {
                        size = align_up(size, field_alignment)?;
                        alignment = alignment.max(field_alignment);
                        size * 8
                    }
                }
                Some(width)
                    if !union
                        && after_bit_field
                        && unit_size == storage.size_bytes
                        && u64::from(width) <= remaining_bits =>
                {
                    let bit_offset = size * 8 - remaining_bits;
                    remaining_bits -= u64::from(width);
                    bit_offset
                }
                Some(width) => {
                    after_bit_field = true;
                    unit_size = storage.size_bytes;
                    if union {
                        size = size.max(storage.size_bytes);
                        0
                    } else {
                        let offset = align_up(size, field_alignment)?;
                        size = offset
                            .checked_add(storage.size_bytes)
                            .ok_or(ResolveError::Unsupported("record size overflow"))?;
                        alignment = alignment.max(field_alignment);
                        remaining_bits = unit_bits - u64::from(width);
                        offset * 8
                    }
                }
            };
            offsets.push(bit_offset / 8);
            bit_offsets.push(Some(bit_offset));
        }
        size = align_up(size, alignment)?;
        let required = required.into_iter().chain(requested).max();
        let rounding = required.or((self.target.pointer_width == 64).then_some(1));
        if let Some(rounding) = rounding {
            alignment = alignment.max(rounding);
            size = align_up(size, alignment)?;
        }
        if size == 0 {
            size = if rounding.is_some_and(|rounding| rounding >= 4) {
                alignment
            } else {
                4
            };
        }
        let (bit_units, field_units) =
            bit_field_units(kind, fields, &bit_offsets, &unit_sizes, true);
        Ok((
            RecordLayout {
                size,
                align: alignment,
                required_align: required,
                offsets,
                bit_offsets,
                bit_units,
                field_units,
            },
            field_alignments,
        ))
    }

    pub(super) fn required_alignment(&self, ty: &Type) -> Option<u64> {
        match ty {
            Type::Array { element, .. } => self.required_alignment(element),
            Type::Defined(id) => match &self.definitions[id.0 as usize].kind {
                TypeDefinitionKind::Alias(inner) => self.required_alignment(inner),
                TypeDefinitionKind::Record {
                    layout: Some(layout),
                    ..
                } => layout.required_align,
                _ => None,
            },
            _ => None,
        }
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

#[derive(Debug, Clone, Copy)]
struct RecordRules {
    packed: bool,
    ms_struct: bool,
    max_field_alignment: Option<u64>,
}

#[derive(Debug, Default)]
struct MsUnit {
    bits: u64,
    unfilled: u64,
}

impl MsUnit {
    fn place(
        &mut self,
        kind: TagKind,
        end_bits: &mut u64,
        width: u64,
        unit_bits: u64,
        aligned: Option<u64>,
        max_field_alignment: Option<u64>,
    ) -> Result<(u64, u64), ResolveError> {
        let mut align = unit_bits / 8;
        if self.bits != unit_bits || self.unfilled < width {
            if self.bits == 0 && width == 0 {
                align = 1;
            }
            *self = Self::default();
        }
        align = align.max(aligned.unwrap_or(1));
        if width != 0 {
            align = align.min(max_field_alignment.unwrap_or(u64::MAX));
        }
        if kind == TagKind::Union {
            *end_bits = (*end_bits).max(if width == 0 { 8 } else { unit_bits });
            return Ok((0, 1));
        }
        let mut position = *end_bits - self.unfilled;
        if width == 0 || width > self.unfilled {
            position = align_up(
                position,
                align
                    .checked_mul(8)
                    .ok_or(ResolveError::Unsupported("field alignment overflow"))?,
            )?;
            self.unfilled = 0;
        }
        if width == 0 {
            *end_bits = position;
            *self = Self::default();
        } else {
            if self.unfilled == 0 {
                *end_bits = position
                    .checked_add(unit_bits)
                    .ok_or(ResolveError::Unsupported("record size overflow"))?;
                self.unfilled = unit_bits;
            }
            self.unfilled -= width;
            self.bits = unit_bits;
        }
        Ok((position, align))
    }
}

fn is_complete(kind: &TypeDefinitionKind) -> bool {
    match kind {
        TypeDefinitionKind::Record { fields, .. } => fields.is_some(),
        TypeDefinitionKind::Enum { enumerators, .. } => enumerators.is_some(),
        TypeDefinitionKind::Alias(_) => true,
    }
}

fn same_tag_shape(a: &TypeDefinitionKind, b: &TypeDefinitionKind) -> bool {
    match (a, b) {
        (
            TypeDefinitionKind::Record {
                kind: a_kind,
                fields: Some(a),
                ..
            },
            TypeDefinitionKind::Record {
                kind: b_kind,
                fields: Some(b),
                ..
            },
        ) => {
            matches!(
                (a_kind, b_kind),
                (RecordKind::Struct, RecordKind::Struct) | (RecordKind::Union, RecordKind::Union)
            ) && a.len() == b.len()
                && a.iter().zip(b).all(|(a, b)| {
                    a.value.name == b.value.name
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

fn checked_bit_width(field: &Field, storage: StorageLayout) -> Result<u64, ResolveError> {
    let unit_bits = storage
        .size_bytes
        .checked_mul(8)
        .ok_or(ResolveError::Unsupported("bit-field unit overflow"))?;
    if let Some(width) = field.bit_width {
        if u64::from(width) > unit_bits {
            return Err(ResolveError::Unsupported("bit-field wider than its type"));
        }
        if width == 0 && field.name.is_some() {
            return Err(ResolveError::Unsupported("named zero-width bit-field"));
        }
    }
    Ok(unit_bits)
}

fn bit_field_units(
    kind: TagKind,
    fields: &[crate::ast::Span<Field>],
    bit_offsets: &[Option<u64>],
    unit_sizes: &[u64],
    discrete: bool,
) -> (Vec<BitFieldUnit>, Vec<Option<usize>>) {
    let mut bit_units: Vec<BitFieldUnit> = Vec::new();
    let mut field_units = Vec::new();
    let mut prior_end = None;
    for ((field, bit_offset), unit_size) in fields.iter().zip(bit_offsets).zip(unit_sizes) {
        let (Some(width), Some(position)) =
            (field.bit_width.filter(|width| *width != 0), bit_offset)
        else {
            field_units.push(None);
            prior_end = None;
            continue;
        };
        if discrete {
            if kind == TagKind::Union || prior_end.is_none_or(|tail| *position >= tail) {
                bit_units.push(BitFieldUnit {
                    offset: position / 8,
                    size: *unit_size,
                });
                prior_end = Some(position + unit_size * 8);
            }
            field_units.push(Some(bit_units.len() - 1));
            continue;
        }
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
    (bit_units, field_units)
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
    attributes: impl IntoIterator<Item = &'a Span<Attribute>>,
) -> Result<Option<u64>, ResolveError> {
    let mut requested: Option<u64> = None;
    for attribute in attributes {
        let value = match &attribute.value {
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
    declaration: &[Span<Attribute>],
    field: &[Span<Attribute>],
) -> Result<(bool, Option<u64>), ResolveError> {
    let packed = declaration
        .iter()
        .chain(field)
        .any(|attribute| matches!(&attribute.value, Attribute::Packed));
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
                space: a_space,
            },
            Type::Pointer {
                pointee: b,
                is_const: b_const,
                access: b_access,
                space: b_space,
            },
        ) => a_const == b_const && a_access == b_access && a_space == b_space && same_layout(a, b),
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

fn without_conventions(specifiers: &DeclarationSpecifiers) -> DeclarationSpecifiers {
    DeclarationSpecifiers {
        attributes: specifiers
            .attributes
            .iter()
            .filter(|attribute| !matches!(attribute.value, Attribute::CallingConvention(_)))
            .cloned()
            .collect(),
        ..specifiers.clone()
    }
}

fn view_convention<'a>(
    resolver: &TypeResolver,
    parameters: &crate::ir::Parameters,
    attributes: impl IntoIterator<Item = &'a Span<Attribute>>,
) -> CallConv {
    if matches!(
        parameters,
        crate::ir::Parameters::Prototype { variadic: true, .. }
    ) {
        return CallConv::C;
    }
    resolver.calling_convention(attributes).unwrap_or_default()
}

fn has_function_declarator(declarator: &Declarator) -> bool {
    match declarator {
        Declarator::Name(_) | Declarator::Abstract => false,
        Declarator::Function { .. } => true,
        Declarator::Grouped(inner)
        | Declarator::Attributed { inner, .. }
        | Declarator::Pointer { inner, .. }
        | Declarator::Array { inner, .. } => has_function_declarator(inner),
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
                    let resolved = resolver.resolve_declarator(
                        &item.specifiers,
                        &declarator.declarator,
                        &declarator.attributes,
                    )?;
                    let c_entries = resolver.render(resolved).entries();
                    resolver.define_alias(
                        name,
                        resolved,
                        item.specifiers
                            .attributes
                            .iter()
                            .chain(&declarator.attributes),
                    )?;
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
                let return_c = resolver.resolve(
                    &without_conventions(&function.specifiers),
                    &Declarator::Abstract,
                )?;
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
                    view_convention(
                        &resolver,
                        &parameters,
                        function
                            .specifiers
                            .attributes
                            .iter()
                            .chain(function.declarator.grouped_attributes())
                            .chain(&function.attributes),
                    ),
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
                    let return_c = resolver.resolve(
                        &without_conventions(&item.specifiers),
                        &Declarator::Abstract,
                    )?;
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
                            view_convention(
                                &resolver,
                                &parameters,
                                item.specifiers
                                    .attributes
                                    .iter()
                                    .chain(declarator.declarator.grouped_attributes())
                                    .chain(&declarator.attributes),
                            ),
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
        let resolved = match signature {
            ParameterList::IdentifierList { .. } => resolver.promoted_parameter(resolved),
            _ => resolved,
        };
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

fn has_machine_mode(specifier: &TypeSpecifier) -> bool {
    match specifier {
        TypeSpecifier::Mode(_) => true,
        TypeSpecifier::Vector(vector) => has_machine_mode(&vector.element),
        _ => false,
    }
}
