use super::numeric::{Context, ResolveError};
use super::types::TypeResolver;
use crate::ast::{Expr, ExprKind, Initializer, NodeId, Span, StmtKind};
use crate::compiler_args::LanguageStandard;
use crate::const_expr::{AssignOp, BinaryOp, PostfixOp, UnaryOp};
use crate::diagnostics::{DiagnosticOptions, Warning};
use crate::ir::*;
use std::collections::HashMap;

pub(super) struct Lowerer {
    pub context: Context,
    pub types: TypeResolver,
    pub module: Module,
    pub names: NameResolution,
    pub c_types: HashMap<BindingId, super::types::CTypeMetadata>,
    pub function_declarations: HashMap<BindingId, super::function::FunctionDeclarations>,
    pub type_spans: HashMap<TypeId, Span<TypeDefinition>>,
    pub object_requests: HashMap<BindingId, super::module::ObjectRequest>,
    pub next_id: u32,
    pub break_targets: Vec<BindingId>,
    pub continue_targets: Vec<BindingId>,
    pub switches: Vec<(BindingId, Type)>,
    pub in_function: bool,
    pub return_type: Option<Type>,
    pub diagnostic_options: DiagnosticOptions,
    pub standard: LanguageStandard,
    pub diagnostics: Vec<super::SemaError>,
}

impl Lowerer {
    pub(super) fn warn<T>(&mut self, warning: Warning, message: &str, node: &Span<T>) {
        self.diagnostics.extend(warning.diagnose(
            message,
            &self.diagnostic_options,
            self.standard,
            node.provenance,
            node.expansion,
        ));
    }

    fn is_record(&self, ty: &Type) -> bool {
        match ty {
            Type::Defined(_) => match self.kind(ty) {
                Some(TypeDefinitionKind::Record { .. }) => true,
                Some(TypeDefinitionKind::Alias(inner)) => self.is_record(inner),
                _ => false,
            },
            _ => false,
        }
    }

    pub fn fresh(&mut self) -> BindingId {
        let id = BindingId(self.next_id);
        self.next_id += 1;
        id
    }

    pub fn declaration_id(&self, node: NodeId, name: &str) -> Result<BindingId, ResolveError> {
        if let Some(id) = self.names.declarations.get(&node) {
            return Ok(*id);
        }
        self.names
            .bindings
            .iter()
            .find(|b| b.id == node && b.name == name)
            .or_else(|| self.names.bindings.iter().find(|b| b.name == name))
            .map(|b| b.value.id)
            .ok_or(ResolveError::Unsupported("missing declaration binding"))
    }

    pub(super) fn reference(&self, e: &Expr) -> Result<BindingId, ResolveError> {
        self.types
            .references
            .get(&e.id)
            .copied()
            .ok_or(ResolveError::Unsupported("missing expression binding"))
    }

    pub fn kind(&self, ty: &Type) -> Option<&TypeDefinitionKind> {
        self.types.kind(ty)
    }

    pub fn pointer(&mut self, pointee: Type, is_const: bool) -> Type {
        self.qualified_pointer(pointee, is_const, Access::default())
    }

    pub fn qualified_pointer(&mut self, pointee: Type, is_const: bool, access: Access) -> Type {
        Type::Pointer {
            pointee: Box::new(pointee),
            is_const,
            access,
        }
    }

    pub(super) fn pointee(&self, ty: &Type) -> Result<Type, ResolveError> {
        match ty {
            Type::Pointer { pointee, .. } => Ok((**pointee).clone()),
            _ => Err(ResolveError::Unsupported("expected pointer")),
        }
    }

    pub(super) fn deref(&self, pointer: Value) -> Result<Place, ResolveError> {
        let Type::Pointer {
            pointee, access, ..
        } = &pointer.ty
        else {
            return Err(ResolveError::Unsupported("expected pointer"));
        };
        Ok(Place {
            ty: (**pointee).clone(),
            access: *access,
            kind: PlaceKind::Deref(Box::new(pointer)),
        })
    }

    pub fn value<T: Clone>(&self, e: &Span<T>, ty: Type, kind: ValueKind) -> Value {
        Value {
            ty,
            node: e.clone().with_value(kind),
        }
    }

    pub fn condition(
        &self,
        value: Value,
        reason: Option<ConversionReason>,
    ) -> Result<Value, ResolveError> {
        if self.enum_underlying(&value.ty).is_some() {
            return self.condition(self.enum_integer(value), reason);
        }
        let mut result = match &value.ty {
            Type::Bool => return Ok(value),
            Type::Numeric(_) | Type::Imaginary(_) => self.context.condition(value),
            Type::Complex(component) => {
                let component = *component;
                let zero = self.value(
                    &value.node,
                    Type::Numeric(component),
                    ValueKind::Constant(match component {
                        NumericType::Integer { .. } => Number::Integer(0u32.into()),
                        NumericType::Float(format) => Number::float_zero(format),
                    }),
                );
                let zero =
                    self.context
                        .convert(zero, value.ty.clone(), ConversionReason::UsualArith);
                let node = value.node.clone();
                self.value(
                    &node,
                    Type::Bool,
                    ValueKind::Compare {
                        op: CompareOp::Ne,
                        left: Box::new(value),
                        right: Box::new(zero),
                        exceptions: matches!(component, NumericType::Float(_))
                            .then_some(self.context.floating.exceptions),
                        reason,
                    },
                )
            }
            ty if self.pointee(ty).is_ok() => {
                let zero = self.value(&value.node, ty.clone(), ValueKind::Null);
                let node = value.node.clone();
                self.value(
                    &node,
                    Type::Bool,
                    ValueKind::Compare {
                        op: CompareOp::Ne,
                        left: Box::new(value),
                        right: Box::new(zero),
                        exceptions: None,
                        reason,
                    },
                )
            }
            _ => return Err(ResolveError::Unsupported("non-scalar condition")),
        };
        if let ValueKind::Compare { reason: why, .. } = &mut result.node.value {
            *why = reason;
        }
        Ok(result)
    }

    fn enum_underlying(&self, ty: &Type) -> Option<Type> {
        match self.kind(ty) {
            Some(TypeDefinitionKind::Enum { underlying, .. }) => underlying.clone(),
            Some(TypeDefinitionKind::Alias(inner)) => self.enum_underlying(inner),
            _ => None,
        }
    }

    pub fn enum_integer(&self, value: Value) -> Value {
        if let Some(ty) = self.enum_underlying(&value.ty) {
            let node = value.node.clone();
            return self.value(
                &node,
                ty,
                ValueKind::Convert {
                    kind: ConversionKind::EnumToInt,
                    operand: Box::new(value),
                    reason: ConversionReason::Promotion,
                    semantics: ConversionSema::Exact,
                },
            );
        }
        value
    }

    pub fn convert(
        &mut self,
        value: Value,
        to: Type,
        reason: ConversionReason,
    ) -> Result<Value, ResolveError> {
        if value.ty == to {
            if self.is_record(&to)
                && matches!(
                    reason,
                    ConversionReason::Assign | ConversionReason::Arg | ConversionReason::Return
                )
            {
                let node = value.node.clone();
                return Ok(self.value(
                    &node,
                    to,
                    ValueKind::Copy {
                        operand: Box::new(value),
                        reason,
                    },
                ));
            }
            return Ok(value);
        }
        if self.enum_underlying(&value.ty).is_some() {
            return self.convert(self.enum_integer(value), to, reason);
        }
        if let Some(underlying) = self.enum_underlying(&to) {
            let value = self.convert(value, underlying, reason)?;
            let node = value.node.clone();
            return Ok(self.value(
                &node,
                to,
                ValueKind::Convert {
                    kind: ConversionKind::IntToEnum,
                    operand: Box::new(value),
                    reason,
                    semantics: ConversionSema::Exact,
                },
            ));
        }
        if to == Type::Bool {
            return self.condition(value, Some(reason));
        }
        if matches!(to, Type::Vector { .. }) || matches!(value.ty, Type::Vector { .. }) {
            return self.context.vector_convert(value, to, reason);
        }
        if matches!(to, Type::Numeric(_) | Type::Complex(_) | Type::Imaginary(_))
            && matches!(
                value.ty,
                Type::Numeric(_) | Type::Complex(_) | Type::Imaginary(_) | Type::Bool
            )
        {
            return Ok(self.context.convert(value, to, reason));
        }
        if self.pointee(&to).is_ok() {
            if matches!(&value.node.value, ValueKind::Constant(Number::Integer(n)) if *n == num_bigint::BigUint::default())
                || matches!(value.node.value, ValueKind::Null)
            {
                return Ok(self.value(&value.node, to, ValueKind::Null));
            }
            if let (Ok(a), Ok(b)) = (self.pointee(&value.ty), self.pointee(&to))
                && (super::types::compatible(&a, &b)
                    || differ_only_in_sign(&a, &b)
                    || differ_only_in_nested_qualifiers(&a, &b)
                    || a == Type::Void
                    || b == Type::Void
                    || reason == ConversionReason::Explicit)
            {
                if matches!(
                    reason,
                    ConversionReason::Assign | ConversionReason::Arg | ConversionReason::Return
                ) && let Some((warning, message)) = pointer_conversion_warning(&value.ty, &to)
                {
                    self.warn(warning, message, &value.node);
                }
                let node = value.node.clone();
                return Ok(self.value(
                    &node,
                    to,
                    ValueKind::Convert {
                        kind: ConversionKind::PointerCast,
                        operand: Box::new(value),
                        reason,
                        semantics: ConversionSema::Exact,
                    },
                ));
            }
        }
        if self.pointee(&value.ty).is_ok() && matches!(to, Type::Numeric(_)) {
            let node = value.node.clone();
            return Ok(self.value(
                &node,
                to,
                ValueKind::Convert {
                    kind: ConversionKind::PtrToInt,
                    operand: Box::new(value),
                    reason,
                    semantics: ConversionSema::Exact,
                },
            ));
        }
        if matches!(value.ty, Type::Numeric(_)) && self.pointee(&to).is_ok() {
            let node = value.node.clone();
            return Ok(self.value(
                &node,
                to,
                ValueKind::Convert {
                    kind: ConversionKind::IntToPtr,
                    operand: Box::new(value),
                    reason,
                    semantics: ConversionSema::Exact,
                },
            ));
        }
        Err(ResolveError::Unsupported(
            "incompatible or unsupported conversion",
        ))
    }

    pub fn convert_expr(
        &mut self,
        expression: &Expr,
        value: Value,
        to: Type,
        reason: ConversionReason,
    ) -> Result<Value, ResolveError> {
        if self.pointee(&to).is_ok()
            && matches!(value.ty, Type::Numeric(NumericType::Integer { .. }))
            && crate::const_expr::Parser::evaluate_ast(expression).is_ok_and(|number| number == 0)
        {
            return Ok(self.value(expression, to, ValueKind::Null));
        }
        self.convert(value, to, reason)
    }

    pub(super) fn place(&mut self, e: &Expr) -> Result<Place, ResolveError> {
        match &e.value {
            ExprKind::Paren(inner) => self.place(inner),
            ExprKind::Generic {
                controlling,
                associations,
            } => {
                let selected = self.generic_selected(controlling, associations)?;
                self.place(selected)
            }
            ExprKind::Identifier(_) => {
                let id = self.reference(e)?;
                let ty = self
                    .types
                    .bindings
                    .get(&id)
                    .ok_or(ResolveError::Unsupported("untyped binding"))?
                    .clone();
                Ok(Place {
                    ty,
                    kind: PlaceKind::Binding(id),
                    access: self.types.access.get(&id).copied().unwrap_or_default(),
                })
            }
            ExprKind::CompoundLiteral { ty, initializer } => {
                let resolved = self.resolve_type_name(ty)?;
                let access = super::types::access(resolved.c.qualifiers);
                let declared = resolved
                    .ty
                    .ok_or(ResolveError::Unsupported("void compound literal"))?;
                let anchor = e.clone().with_value(());
                let value = self.initializer_value(
                    &declared,
                    &Initializer::List(initializer.clone()),
                    &anchor,
                )?;
                let object = self.fresh();
                let ty = value.ty.clone();
                self.types.bindings.insert(object, ty.clone());
                let storage = if self.in_function {
                    StorageDuration::Automatic
                } else {
                    StorageDuration::Static
                };
                Ok(Place {
                    ty,
                    kind: PlaceKind::CompoundLiteral {
                        object,
                        storage,
                        initializer: Box::new(value),
                    },
                    access,
                })
            }
            ExprKind::Unary {
                op: UnaryOp::Deref,
                operand,
            } => {
                let value = self.expr(operand)?;
                self.deref(value)
            }
            ExprKind::Unary {
                op: UnaryOp::Real | UnaryOp::Imag,
                operand,
            } => {
                let base = self.place(operand)?;
                let Type::Complex(component) = base.ty else {
                    return Err(ResolveError::Unsupported(
                        "complex component of non-complex place",
                    ));
                };
                Ok(Place {
                    ty: Type::Numeric(component),
                    access: base.access,
                    kind: PlaceKind::ComplexPart {
                        base: Box::new(base),
                        imaginary: matches!(
                            &e.value,
                            ExprKind::Unary {
                                op: UnaryOp::Imag,
                                ..
                            }
                        ),
                    },
                })
            }
            ExprKind::Index { base, index } => {
                let base = self.expr(base)?;
                let index = self.expr(index)?;
                let (pointer_ty, kind) = self.binary(BinaryOp::Add, base, index)?;
                self.deref(self.value(e, pointer_ty, kind))
            }
            ExprKind::Member { base, field, arrow } => {
                let base = if *arrow {
                    let value = self.expr(base)?;
                    self.deref(value)?
                } else {
                    self.place(base)?
                };
                self.project(base, field.value.as_str())?
                    .ok_or(ResolveError::Unsupported("unknown member"))
            }
            _ => Err(ResolveError::Unsupported(
                "expression is not a supported place",
            )),
        }
    }

    fn record_body(&self, ty: &Type) -> Option<(Vec<Span<Field>>, Option<RecordLayout>)> {
        match self.kind(ty)? {
            TypeDefinitionKind::Record {
                fields: Some(fields),
                layout,
                ..
            } => Some((fields.clone(), layout.clone())),
            TypeDefinitionKind::Alias(inner) => {
                let inner = inner.clone();
                self.record_body(&inner)
            }
            _ => None,
        }
    }

    fn field_place(
        &self,
        base: Place,
        index: usize,
        field: &Field,
        layout: Option<&RecordLayout>,
    ) -> Result<Place, ResolveError> {
        let bits = match field.bit_width {
            Some(width) if width != 0 => {
                let layout =
                    layout.ok_or(ResolveError::Unsupported("bit-field in unlaid-out record"))?;
                let unit = layout
                    .field_units
                    .get(index)
                    .copied()
                    .flatten()
                    .ok_or(ResolveError::Unsupported("bit-field without storage unit"))?;
                let position = layout
                    .bit_offsets
                    .get(index)
                    .copied()
                    .flatten()
                    .ok_or(ResolveError::Unsupported("bit-field without bit offset"))?;
                let unit = layout
                    .bit_units
                    .get(unit)
                    .map(|storage| (unit, storage))
                    .ok_or(ResolveError::Unsupported("bit-field without storage unit"))?;
                Some(BitFieldAccess {
                    unit: unit.0,
                    unit_offset: unit.1.offset,
                    unit_size: unit.1.size,
                    bit_offset: position - unit.1.offset * 8,
                    width,
                })
            }
            Some(_) => return Err(ResolveError::Unsupported("zero-width bit-field access")),
            None => None,
        };
        Ok(Place {
            ty: field.ty.clone(),
            access: base.access.union(field.access),
            kind: PlaceKind::Field {
                base: Box::new(base),
                index,
                bits,
            },
        })
    }

    fn project(&self, base: Place, name: &str) -> Result<Option<Place>, ResolveError> {
        let Some((fields, layout)) = self.record_body(&base.ty) else {
            return Err(ResolveError::Unsupported(
                "member of incomplete or non-record",
            ));
        };
        if let Some((index, field)) = fields
            .iter()
            .enumerate()
            .find(|(_, f)| f.name.as_deref() == Some(name))
        {
            return self
                .field_place(base, index, field, layout.as_ref())
                .map(Some);
        }
        for (index, field) in fields.iter().enumerate() {
            if field.name.is_some() || self.record_body(&field.ty).is_none() {
                continue;
            }
            let nested = self.field_place(base.clone(), index, field, layout.as_ref())?;
            if let Some(found) = self.project(nested, name)? {
                return Ok(Some(found));
            }
        }
        Ok(None)
    }

    pub(super) fn generic_selected<'e>(
        &mut self,
        controlling: &crate::ast::GenericControl,
        associations: &'e [crate::ast::GenericAssociation],
    ) -> Result<&'e Expr, ResolveError> {
        let controlling = match controlling {
            crate::ast::GenericControl::Type { ty } => self
                .resolve_type_name(ty)?
                .ty
                .ok_or(ResolveError::Unsupported("void generic controlling type"))?,
            crate::ast::GenericControl::Expr(expr) => self.unevaluated(expr)?,
        };
        for association in associations {
            if let crate::ast::GenericAssociation::Type { ty, .. } = association {
                self.prepare_typeof(&ty.specifiers, &ty.declarator)?;
            }
        }
        self.types.select_association(controlling, associations)
    }

    // _Generic's controlling operand is never evaluated, so keep only the type it lowered to.
    fn unevaluated(&mut self, e: &Expr) -> Result<Type, ResolveError> {
        let next_id = self.next_id;
        let globals = self.module.globals.len();
        // lvalue conversion keeps a bit-field's declared type; integer promotion does not apply here
        let ty = match self.place(e) {
            Ok(
                place @ Place {
                    kind: PlaceKind::Field { bits: Some(_), .. },
                    ..
                },
            ) => Ok(place.ty),
            _ => self.expr(e).map(|value| value.ty),
        };
        self.next_id = next_id;
        self.module.globals.truncate(globals);
        ty
    }

    // A function designator decays to a pointer, so both call forms arrive here as one.
    fn callee(&mut self, e: &Expr) -> Result<(Callee, Type), ResolveError> {
        let value = self.expr(e)?;
        let signature = self.pointee(&value.ty)?;
        if !matches!(signature, Type::Function { .. }) {
            return Err(ResolveError::Unsupported("non-function callee"));
        }
        if let ValueKind::FunctionDecay {
            place:
                Place {
                    kind: PlaceKind::Binding(id),
                    ..
                },
        } = &value.node.value
        {
            return Ok((Callee::Direct(*id), signature));
        }
        Ok((Callee::Indirect(Box::new(value)), signature))
    }

    fn read(&mut self, e: &Expr, place: Place) -> Result<Value, ResolveError> {
        if let Type::Array { element, length } = &place.ty {
            let ty = self.qualified_pointer((**element).clone(), false, place.access);
            let length = *length;
            return Ok(self.value(e, ty, ValueKind::ArrayDecay { place, length }));
        }
        if let Type::VariableArray { element, .. } = &place.ty {
            let ty = self.qualified_pointer((**element).clone(), false, place.access);
            return Ok(self.value(
                e,
                ty,
                ValueKind::ArrayDecay {
                    place,
                    length: None,
                },
            ));
        }
        if matches!(place.ty, Type::Function { .. }) {
            // C makes *f on a function designator the same designator, so both spell one value.
            if let PlaceKind::Deref(pointer) = place.kind {
                return Ok(self.value(e, pointer.ty.clone(), pointer.node.value));
            }
            let ty = self.pointer(place.ty.clone(), false);
            return Ok(self.value(e, ty, ValueKind::FunctionDecay { place }));
        }
        let promote = self.promotes_by_width(&place);
        let ordering = place.implicit_ordering();
        let value = self.value(e, place.ty.clone(), ValueKind::Read { place, ordering });
        Ok(if promote {
            self.context
                .convert(value, self.context.int_type(), ConversionReason::Promotion)
        } else {
            value
        })
    }

    // a bit-field rvalue promotes by its declared width, not by its storage type
    fn promotes_by_width(&self, place: &Place) -> bool {
        matches!(
            &place.kind,
            PlaceKind::Field { bits: Some(bits), .. } if bits.width < self.context.target.int_width
        )
    }

    fn binary(
        &self,
        op: BinaryOp,
        left: Value,
        right: Value,
    ) -> Result<(Type, ValueKind), ResolveError> {
        let left_element = self.pointee(&left.ty).ok();
        let right_element = self.pointee(&right.ty).ok();
        match (op, left_element, right_element) {
            (BinaryOp::Sub, Some(element), Some(other)) => {
                if !super::types::compatible(&element, &other) {
                    return Err(ResolveError::Unsupported(
                        "incompatible pointer subtraction",
                    ));
                }
                self.types.require_complete(&element)?;
                Ok((
                    Type::integer(self.context.target.pointer_width, true),
                    ValueKind::PointerDifference {
                        left: Box::new(left),
                        right: Box::new(right),
                        element,
                    },
                ))
            }
            (BinaryOp::Add | BinaryOp::Sub, Some(element), None) => {
                self.pointer_offset(left, right, element, op == BinaryOp::Sub)
            }
            (BinaryOp::Add, None, Some(element)) => {
                self.pointer_offset(right, left, element, false)
            }
            _ => self.context.resolve_binary(op, left, right),
        }
    }

    fn pointer_offset(
        &self,
        pointer: Value,
        amount: Value,
        element: Type,
        subtract: bool,
    ) -> Result<(Type, ValueKind), ResolveError> {
        self.types.require_complete(&element)?;
        let amount = self.context.promote(amount);
        if !matches!(amount.ty, Type::Numeric(NumericType::Integer { .. })) {
            return Err(ResolveError::Unsupported("noninteger pointer offset"));
        }
        Ok((
            pointer.ty.clone(),
            ValueKind::PointerOffset {
                pointer: Box::new(pointer),
                amount: Box::new(amount),
                subtract,
                element,
                overflow: if self.context.pointer_wrap {
                    Overflow::Wrap
                } else {
                    Overflow::Undefined
                },
            },
        ))
    }

    fn update(
        &mut self,
        e: &Expr,
        target: &Expr,
        op: BinaryOp,
        rhs: Value,
        postfix: bool,
    ) -> Result<Value, ResolveError> {
        let place = self.place(target)?;
        let old = self.value(target, place.ty.clone(), ValueKind::OldValue);
        let old = if self.promotes_by_width(&place) {
            self.context
                .convert(old, self.context.int_type(), ConversionReason::Promotion)
        } else {
            old
        };
        let (ty, kind) = self.binary(op, old, rhs)?;
        let computation = self.convert(
            self.value(e, ty, kind),
            place.ty.clone(),
            ConversionReason::Assign,
        )?;
        Ok(self.value(
            e,
            place.ty.clone(),
            ValueKind::Update {
                ordering: place.implicit_ordering(),
                place,
                computation: Box::new(computation),
                postfix,
            },
        ))
    }

    fn unevaluated_type(&mut self, operand: &Expr) -> Result<(Type, Access), ResolveError> {
        if let ExprKind::Paren(inner) = &operand.value {
            return self.unevaluated_type(inner);
        }
        if let ExprKind::StringLiteral(lit) = &operand.value {
            return Ok((
                super::types::string_literal_type(lit, &self.context.target, self.types.features),
                Access::default(),
            ));
        }
        let globals = self.module.globals.len();
        let next_id = self.next_id;
        let result = match self.place(operand) {
            Ok(Place {
                kind: PlaceKind::Field { bits: Some(_), .. },
                ..
            }) => Err(ResolveError::Invalid(
                "application of sizeof or alignof to a bit-field",
            )),
            Ok(place) => Ok((place.ty, place.access)),
            Err(_) => self
                .expr(operand)
                .map(|value| (value.ty, Access::default())),
        };
        self.module.globals.truncate(globals);
        self.next_id = next_id;
        result
    }

    fn type_name_extents(
        &mut self,
        ty: &crate::ast::TypeName,
    ) -> Result<Vec<(BindingId, Value)>, ResolveError> {
        let mut extents = Vec::new();
        if self.in_function {
            self.extents(&ty.declarator, &mut extents)?;
        }
        Ok(extents)
    }

    fn with_extents(&mut self, e: &Expr, extents: Vec<(BindingId, Value)>, value: Value) -> Value {
        extents
            .into_iter()
            .rev()
            .fold(value, |value, (id, extent)| {
                self.value(
                    e,
                    value.ty.clone(),
                    ValueKind::Capture {
                        id,
                        extent: Box::new(extent),
                        value: Box::new(value),
                    },
                )
            })
    }

    fn runtime_size(&mut self, e: &Expr, ty: &Type) -> Result<Value, ResolveError> {
        let size_type = Type::integer(self.context.target.pointer_width, false);
        let Type::VariableArray { element, extent } = ty else {
            let size = self.types.storage(ty.clone())?.size_bytes;
            return Ok(self.value(
                e,
                size_type,
                ValueKind::Constant(Number::Integer(size.into())),
            ));
        };
        let VariableExtent::Captured(extent) = *extent else {
            return Err(ResolveError::Invalid(
                "size of an unspecified variable length array",
            ));
        };
        let place = Place {
            ty: size_type.clone(),
            kind: PlaceKind::Binding(extent),
            access: Access::default(),
        };
        let count = self.value(
            e,
            size_type,
            ValueKind::Read {
                place,
                ordering: None,
            },
        );
        let element = self.runtime_size(e, element)?;
        let (ty, kind) = self.binary(BinaryOp::Mul, count, element)?;
        Ok(self.value(e, ty, kind))
    }

    fn layout_constant(&mut self, e: &Expr, amount: u64, key: &str, detail: String) -> Value {
        self.module
            .metadata
            .entry(e.id)
            .or_default()
            .push((key.into(), detail));
        let ty = Type::integer(self.context.target.pointer_width, false);
        self.value(e, ty, ValueKind::Constant(Number::Integer(amount.into())))
    }

    pub fn expr(&mut self, e: &Expr) -> Result<Value, ResolveError> {
        match &e.value {
            ExprKind::Paren(inner) => self.expr(inner),
            ExprKind::Identifier(_)
                if self
                    .names
                    .references
                    .iter()
                    .any(|r| r.id == e.id && r.kind == BindingKind::Enumerator) =>
            {
                let binding = self.reference(e)?;
                let node = self
                    .names
                    .bindings
                    .iter()
                    .find(|b| b.value.id == binding)
                    .ok_or(ResolveError::Unsupported("missing enumerator binding"))?
                    .id;
                let value = self
                    .types
                    .definitions
                    .iter()
                    .find_map(|definition| {
                        if let TypeDefinitionKind::Enum {
                            enumerators: Some(entries),
                            ..
                        } = &definition.kind
                        {
                            entries
                                .iter()
                                .find(|entry| entry.id == node)
                                .map(|entry| entry.value.value.clone())
                        } else {
                            None
                        }
                    })
                    .ok_or(ResolveError::Unsupported("unresolved enumerator constant"))?;
                Ok(self.value(e, value.ty, value.node.value))
            }
            ExprKind::Identifier(_)
            | ExprKind::Member { .. }
            | ExprKind::Index { .. }
            | ExprKind::CompoundLiteral { .. }
            | ExprKind::Unary {
                op: UnaryOp::Deref, ..
            } => {
                let place = self.place(e)?;
                self.read(e, place)
            }
            ExprKind::CharLiteral(lit) => {
                let (ty, number) = super::types::character_constant(lit, &self.context.target)?;
                Ok(self.value(e, ty, ValueKind::Constant(number)))
            }
            ExprKind::LabelAddress(label) => {
                let id = self
                    .names
                    .references
                    .iter()
                    .find(|r| r.id == label.id)
                    .map(|r| r.binding)
                    .ok_or(ResolveError::Unsupported("missing label address binding"))?;
                let ty = self.pointer(Type::Void, false);
                Ok(self.value(e, ty, ValueKind::LabelAddress(id)))
            }
            ExprKind::NullPtrLiteral => {
                let ty = self.pointer(Type::Void, false);
                Ok(self.value(e, ty, ValueKind::Null))
            }
            ExprKind::StringLiteral(lit) => {
                let mut units = lit.execution_units(self.context.target.wchar_width);
                units.push(0);
                let ty = super::types::string_literal_type(
                    lit,
                    &self.context.target,
                    self.types.features,
                );
                let id = self.fresh();
                let initializer = self.value(e, ty.clone(), ValueKind::CodeUnits(units));
                self.module.globals.push(e.clone().with_value(Global {
                    variable: Variable {
                        id,
                        name: format!(".str{}", id.0),
                        ty: ty.clone(),
                        storage: StorageDuration::Static,
                        restrict: false,
                        is_const: false,
                        constexpr: false,
                        initializer: Some(initializer),
                    },
                    linkage: Linkage::Internal,
                    symbol: SymbolAttributes::default(),
                    definition: true,
                    alignment: None,
                    common: false,
                }));
                self.read(
                    e,
                    Place {
                        ty,
                        kind: PlaceKind::Binding(id),
                        access: Access::default(),
                    },
                )
            }
            ExprKind::Cast { ty, value } => {
                let extents = self.type_name_extents(ty)?;
                let to = self.resolve_type_name(ty)?.ty;
                let value = self.expr(value)?;
                let cast = if let Some(to) = to {
                    self.convert(value, to, ConversionReason::Explicit)?
                } else {
                    let end = self.value(e, Type::Void, ValueKind::Void);
                    self.value(
                        e,
                        Type::Void,
                        ValueKind::Sequence {
                            left: Box::new(value),
                            right: Box::new(end),
                        },
                    )
                };
                Ok(self.with_extents(e, extents, cast))
            }
            ExprKind::Unary {
                op: UnaryOp::AddrOf,
                operand,
            } => {
                let place = self.place(operand)?;
                if matches!(place.kind, PlaceKind::Field { bits: Some(_), .. }) {
                    return Err(ResolveError::Invalid("address of a bit-field"));
                }
                let ty = self.qualified_pointer(place.ty.clone(), false, place.access);
                Ok(self.value(e, ty, ValueKind::AddressOf(place)))
            }
            ExprKind::Unary {
                op: UnaryOp::PreIncrement | UnaryOp::PreDecrement,
                operand,
            }
            | ExprKind::Postfix { operand, .. } => {
                let decrement = matches!(
                    e.value,
                    ExprKind::Unary {
                        op: UnaryOp::PreDecrement,
                        ..
                    } | ExprKind::Postfix {
                        op: PostfixOp::Decrement,
                        ..
                    }
                );
                let rhs = self.value(
                    e,
                    self.context.int_type(),
                    ValueKind::Constant(Number::Integer(1u32.into())),
                );
                self.update(
                    e,
                    operand,
                    if decrement {
                        BinaryOp::Sub
                    } else {
                        BinaryOp::Add
                    },
                    rhs,
                    matches!(e.value, ExprKind::Postfix { .. }),
                )
            }
            ExprKind::Unary {
                op: UnaryOp::Real | UnaryOp::Imag,
                operand,
            } => {
                if let Ok(place) = self.place(e) {
                    return self.read(e, place);
                }
                let value = self.expr(operand)?;
                let Type::Complex(component) = value.ty else {
                    return Err(ResolveError::Unsupported(
                        "complex component of non-complex value",
                    ));
                };
                Ok(self.value(
                    e,
                    Type::Numeric(component),
                    ValueKind::Convert {
                        kind: if matches!(
                            &e.value,
                            ExprKind::Unary {
                                op: UnaryOp::Real,
                                ..
                            }
                        ) {
                            ConversionKind::ComplexToReal
                        } else {
                            ConversionKind::ComplexToImag
                        },
                        operand: Box::new(value),
                        reason: ConversionReason::Explicit,
                        semantics: ConversionSema::Exact,
                    },
                ))
            }
            ExprKind::Unary { op, operand } => {
                let value = self.expr(operand)?;
                let value = self.enum_integer(value);
                match op {
                    UnaryOp::Plus => {
                        if !matches!(
                            value.ty,
                            Type::Bool | Type::Numeric(_) | Type::Complex(_) | Type::Imaginary(_)
                        ) {
                            return Err(ResolveError::Unsupported("non-numeric unary plus"));
                        }
                        Ok(self.context.promote(value))
                    }
                    UnaryOp::Not => {
                        let value = self.condition(value, None)?;
                        Ok(self.value(
                            e,
                            Type::Bool,
                            ValueKind::Unary {
                                op: UnaryArithOp::Not,
                                operand: Box::new(value),
                                semantics: ArithSema::Exact,
                            },
                        ))
                    }
                    UnaryOp::Minus | UnaryOp::BitNot => {
                        let (ty, kind) = self.context.resolve_unary_arith(*op, value)?;
                        Ok(self.value(e, ty, kind))
                    }
                    _ => Err(ResolveError::Unsupported("advanced unary operator")),
                }
            }
            ExprKind::Binary { op, left, right } => {
                let left_expr = left;
                let right_expr = right;
                let left = self.expr(left)?;
                let left = self.enum_integer(left);
                let right = self.expr(right)?;
                let right = self.enum_integer(right);
                if matches!(op, BinaryOp::And | BinaryOp::Or) {
                    return Ok(self.value(
                        e,
                        Type::Bool,
                        ValueKind::Logical {
                            op: if *op == BinaryOp::And {
                                LogicalOp::And
                            } else {
                                LogicalOp::Or
                            },
                            left: Box::new(self.condition(left, None)?),
                            right: Box::new(self.condition(right, None)?),
                        },
                    ));
                }
                if matches!(op, BinaryOp::Equal | BinaryOp::NotEqual)
                    && (self.pointee(&left.ty).is_ok() || self.pointee(&right.ty).is_ok())
                {
                    let ty = if self.pointee(&left.ty).is_ok() {
                        left.ty.clone()
                    } else {
                        right.ty.clone()
                    };
                    let left = self.convert_expr(
                        left_expr,
                        left,
                        ty.clone(),
                        ConversionReason::UsualArith,
                    )?;
                    let right =
                        self.convert_expr(right_expr, right, ty, ConversionReason::UsualArith)?;
                    return Ok(self.value(
                        e,
                        Type::Bool,
                        ValueKind::Compare {
                            op: if *op == BinaryOp::Equal {
                                CompareOp::Eq
                            } else {
                                CompareOp::Ne
                            },
                            left: Box::new(left),
                            right: Box::new(right),
                            exceptions: None,
                            reason: None,
                        },
                    ));
                }
                if matches!(
                    op,
                    BinaryOp::Less
                        | BinaryOp::LessEqual
                        | BinaryOp::Greater
                        | BinaryOp::GreaterEqual
                ) && (self.pointee(&left.ty).is_ok() && self.pointee(&right.ty).is_ok())
                {
                    let ty = left.ty.clone();
                    let left = self.convert_expr(
                        left_expr,
                        left,
                        ty.clone(),
                        ConversionReason::UsualArith,
                    )?;
                    let right =
                        self.convert_expr(right_expr, right, ty, ConversionReason::UsualArith)?;
                    return Ok(self.value(
                        e,
                        Type::Bool,
                        ValueKind::Compare {
                            op: match op {
                                BinaryOp::Less => CompareOp::Lt,
                                BinaryOp::LessEqual => CompareOp::Le,
                                BinaryOp::Greater => CompareOp::Gt,
                                _ => CompareOp::Ge,
                            },
                            left: Box::new(left),
                            right: Box::new(right),
                            exceptions: None,
                            reason: None,
                        },
                    ));
                }
                let (ty, kind) = self.binary(*op, left, right)?;
                Ok(self.value(e, ty, kind))
            }
            ExprKind::Assign { op, target, value } => {
                let value_expr = value;
                let value = self.expr(value)?;
                if *op == AssignOp::Assign {
                    let place = self.place(target)?;
                    let value = self.convert_expr(
                        value_expr,
                        value,
                        place.ty.clone(),
                        ConversionReason::Assign,
                    )?;
                    return Ok(self.value(
                        e,
                        place.ty.clone(),
                        ValueKind::Store {
                            ordering: place.implicit_ordering(),
                            place,
                            value: Box::new(value),
                        },
                    ));
                }
                self.update(e, target, assignment_operator(*op)?, value, false)
            }
            ExprKind::Comma { left, right } => {
                let left = self.expr(left)?;
                let right = self.expr(right)?;
                Ok(self.value(
                    e,
                    right.ty.clone(),
                    ValueKind::Sequence {
                        left: Box::new(left),
                        right: Box::new(right),
                    },
                ))
            }
            ExprKind::Conditional {
                condition,
                then_value,
                else_value,
            } => {
                let Some(then_value) = then_value else {
                    return Err(ResolveError::Unsupported("GNU omitted conditional operand"));
                };
                let condition = self.expr(condition)?;
                let condition = self.condition(condition, None)?;
                let mut left = self.expr(then_value)?;
                let mut right = self.expr(else_value)?;
                if matches!(
                    left.ty,
                    Type::Bool | Type::Numeric(_) | Type::Complex(_) | Type::Imaginary(_)
                ) && matches!(
                    right.ty,
                    Type::Bool | Type::Numeric(_) | Type::Complex(_) | Type::Imaginary(_)
                ) {
                    (left, right) = self
                        .context
                        .usual_arithmetic(self.context.promote(left), self.context.promote(right));
                } else if self.pointee(&left.ty).is_ok() {
                    right = self.convert_expr(
                        else_value,
                        right,
                        left.ty.clone(),
                        ConversionReason::UsualArith,
                    )?;
                } else if self.pointee(&right.ty).is_ok() {
                    left = self.convert_expr(
                        then_value,
                        left,
                        right.ty.clone(),
                        ConversionReason::UsualArith,
                    )?;
                }
                if left.ty != right.ty {
                    return Err(ResolveError::Unsupported(
                        "incompatible conditional operands",
                    ));
                }
                Ok(self.value(
                    e,
                    left.ty.clone(),
                    ValueKind::Conditional {
                        condition: Box::new(condition),
                        then_value: Box::new(left),
                        else_value: Box::new(right),
                    },
                ))
            }
            ExprKind::Generic {
                controlling,
                associations,
            } => {
                let selected = self.generic_selected(controlling, associations)?;
                self.expr(selected)
            }
            ExprKind::Call {
                callee: builtin,
                arguments,
            } if super::atomic::atomic_builtin(builtin).is_some() => {
                let atomic = super::atomic::atomic_builtin(builtin)
                    .ok_or(ResolveError::Unsupported("atomic builtin"))?;
                self.atomic_builtin(e, builtin, atomic, arguments)
            }
            ExprKind::Call { callee, arguments }
                if constant_p_operand(callee, arguments).is_some() =>
            {
                let operand = constant_p_operand(callee, arguments)
                    .ok_or(ResolveError::Unsupported("__builtin_constant_p"))?;
                let constant = super::types::is_folded(&self.expr(operand)?);
                self.module
                    .metadata
                    .entry(e.id)
                    .or_default()
                    .push(("c_builtin".into(), "__builtin_constant_p".into()));
                Ok(self.value(
                    e,
                    self.context.int_type(),
                    ValueKind::Constant(Number::SignedInteger(u8::from(constant).into())),
                ))
            }
            ExprKind::Call { callee, arguments } if va_builtin(callee).is_some() => {
                let builtin = va_builtin(callee).ok_or(ResolveError::Unsupported("va builtin"))?;
                self.va_builtin(e, builtin, arguments)
            }
            ExprKind::Call { callee, arguments } => {
                let (callee, ty) = self.callee(callee)?;
                let Type::Function {
                    return_type,
                    parameters,
                    variadic,
                    prototyped,
                } = &ty
                else {
                    return Err(ResolveError::Unsupported("non-function callee"));
                };
                if *prototyped
                    && (arguments.len() < parameters.len()
                        || (!*variadic && arguments.len() != parameters.len()))
                {
                    return Err(ResolveError::Unsupported("call argument count"));
                }
                let mut lowered = Vec::new();
                for (index, argument) in arguments.iter().enumerate() {
                    let value = self.expr(argument)?;
                    let value = if let Some(to) = parameters.get(index).filter(|_| *prototyped) {
                        self.convert_expr(argument, value, to.clone(), ConversionReason::Arg)?
                    } else {
                        let to = match &value.ty {
                            Type::Numeric(NumericType::Float(FloatType::F32)) => {
                                Type::Numeric(NumericType::Float(FloatType::F64))
                            }
                            Type::Bool => self.context.int_type(),
                            Type::Numeric(NumericType::Integer {
                                width,
                                bit_precise: false,
                                ..
                            }) if *width < self.context.target.int_width => self.context.int_type(),
                            _ => value.ty.clone(),
                        };
                        self.convert(value, to, ConversionReason::Vararg)?
                    };
                    lowered.push(value);
                }
                let abi = self.abi_signature(&ty, Some(&lowered))?;
                Ok(self.value(
                    e,
                    return_type.as_ref().map_or(Type::Void, |ty| (**ty).clone()),
                    ValueKind::Call {
                        callee,
                        signature: ty,
                        abi,
                        arguments: lowered,
                    },
                ))
            }
            ExprKind::IntegerLiteral(_) | ExprKind::FloatLiteral(_) | ExprKind::BoolLiteral(_) => {
                self.context.resolve(e)
            }
            ExprKind::SizeOfType { ty } | ExprKind::AlignOf { ty } => {
                let extents = self.type_name_extents(ty)?;
                let resolved = self.resolve_type_name(ty)?;
                let atomic = resolved.c.qualifiers.is_atomic;
                let ty = resolved
                    .ty
                    .ok_or(ResolveError::Unsupported("void layout"))?;
                if matches!(e.value, ExprKind::SizeOfType { .. })
                    && matches!(ty, Type::VariableArray { .. })
                {
                    let size = self.runtime_size(e, &ty)?;
                    return Ok(self.with_extents(e, extents, size));
                }
                let layout = self
                    .types
                    .qualified_storage(fixed_element(&ty).clone(), atomic)?;
                let value = if matches!(e.value, ExprKind::SizeOfType { .. }) {
                    layout.size_bytes
                } else {
                    u64::from(layout.alignment_bytes)
                };
                let key = if matches!(e.value, ExprKind::SizeOfType { .. }) {
                    "size_of"
                } else {
                    "align_of"
                };
                Ok(self.layout_constant(e, value, key, ty.to_string()))
            }
            ExprKind::SizeOfExpr(operand) | ExprKind::AlignOfExpr(operand) => {
                let (ty, access) = self.unevaluated_type(operand)?;
                if matches!(e.value, ExprKind::SizeOfExpr(_))
                    && matches!(ty, Type::VariableArray { .. })
                {
                    return self.runtime_size(e, &ty);
                }
                let layout = self
                    .types
                    .qualified_storage(fixed_element(&ty).clone(), access.atomic)?;
                let amount = if matches!(e.value, ExprKind::SizeOfExpr(_)) {
                    layout.size_bytes
                } else {
                    u64::from(layout.alignment_bytes)
                };
                let key = if matches!(e.value, ExprKind::SizeOfExpr(_)) {
                    "size_of"
                } else {
                    "align_of"
                };
                Ok(self.layout_constant(e, amount, key, ty.to_string()))
            }
            ExprKind::TypesCompatible { left_ty, right_ty } => {
                let (compatible, compared) = self.types.types_compatible(left_ty, right_ty)?;
                self.module
                    .metadata
                    .entry(e.id)
                    .or_default()
                    .push(("types_compatible".into(), compared));
                Ok(self.value(
                    e,
                    self.context.int_type(),
                    ValueKind::Constant(Number::SignedInteger(u8::from(compatible).into())),
                ))
            }
            ExprKind::OffsetOf { ty, member } => {
                let ty = self
                    .resolve_type_name(ty)?
                    .ty
                    .ok_or(ResolveError::Unsupported("void offsetof"))?;
                let (_, offset) = self.types.offsetof_member(ty.clone(), member)?;
                Ok(self.layout_constant(e, offset, "offset_of", format!("{ty}.{member}")))
            }
            ExprKind::BitCast { ty, value } => {
                let ty = self
                    .resolve_type_name(ty)?
                    .ty
                    .ok_or(ResolveError::Invalid("bit cast to void"))?;
                if matches!(ty, Type::Array { .. } | Type::VariableArray { .. }) {
                    return Err(ResolveError::Invalid("bit cast to an array type"));
                }
                let value = self.expr(value)?;
                if self.types.storage(ty.clone())?.size_bytes
                    != self.types.storage(value.ty.clone())?.size_bytes
                {
                    return Err(ResolveError::Invalid(
                        "bit cast between types of different sizes",
                    ));
                }
                Ok(self.value(
                    e,
                    ty,
                    ValueKind::Convert {
                        kind: ConversionKind::BitCast,
                        operand: Box::new(value),
                        reason: ConversionReason::Explicit,
                        semantics: ConversionSema::Exact,
                    },
                ))
            }
            ExprKind::StatementExpression(body) => {
                if !self.in_function {
                    return Err(ResolveError::Invalid(
                        "statement expression outside a function",
                    ));
                }
                let last = body
                    .iter()
                    .rposition(|statement| !matches!(statement.value, StmtKind::Comment(_)));
                let (leading, result) = match last {
                    Some(index) => match &body[index].value {
                        StmtKind::Expr(result) => (&body[..index], Some(result)),
                        _ => (&body[..], None),
                    },
                    None => (&body[..], None),
                };
                let (statements, value) = self.scoped(|lower| {
                    let statements = lower.statements(leading, lower.return_type.clone())?;
                    let value = match result {
                        Some(result) => lower.expr(result)?,
                        None => lower.value(e, Type::Void, ValueKind::Void),
                    };
                    Ok((statements, value))
                })?;
                Ok(self.value(
                    e,
                    value.ty.clone(),
                    ValueKind::StatementExpression(Box::new(Evaluation { statements, value })),
                ))
            }
            ExprKind::VaArg { list, ty } => {
                let list = self.place(list)?;
                if list.ty != Type::VaList {
                    return Err(ResolveError::Unsupported("va_arg of non-va_list"));
                }
                let ty = self
                    .resolve_type_name(ty)?
                    .ty
                    .ok_or(ResolveError::Unsupported("va_arg of void"))?;
                self.types.storage(ty.clone())?;
                Ok(self.value(e, ty, ValueKind::VaArg { list }))
            }
        }
    }
}

fn fixed_element(ty: &Type) -> &Type {
    match ty {
        Type::VariableArray { element, .. } => fixed_element(element),
        _ => ty,
    }
}

#[derive(Clone, Copy)]
pub(super) enum VaBuiltin {
    Start,
    End,
    Copy,
}

fn pointer_conversion_warning(from: &Type, to: &Type) -> Option<(Warning, &'static str)> {
    let (
        Type::Pointer {
            pointee: from_pointee,
            is_const: from_const,
            access: from_access,
        },
        Type::Pointer {
            pointee: to_pointee,
            is_const: to_const,
            access: to_access,
        },
    ) = (from, to)
    else {
        return None;
    };
    if differ_only_in_sign(from_pointee, to_pointee) {
        return Some((
            Warning::PointerSign,
            "conversion between pointers to integer types with different sign",
        ));
    }
    if (*from_const && !to_const) || (from_access.volatile && !to_access.volatile) {
        return Some((
            Warning::IncompatiblePointerTypesDiscardsQualifiers,
            "pointer conversion discards qualifiers",
        ));
    }
    if differ_only_in_nested_qualifiers(from_pointee, to_pointee) {
        return Some((
            Warning::IncompatiblePointerTypesDiscardsQualifiers,
            "pointer conversion discards qualifiers in nested pointer types",
        ));
    }
    None
}

fn differ_only_in_nested_qualifiers(a: &Type, b: &Type) -> bool {
    matches!((a, b), (Type::Pointer { .. }, Type::Pointer { .. }))
        && !super::types::compatible(a, b)
        && compatible_ignoring_qualifiers(a, b)
}

fn compatible_ignoring_qualifiers(a: &Type, b: &Type) -> bool {
    match (a, b) {
        (Type::Pointer { pointee: a, .. }, Type::Pointer { pointee: b, .. }) => {
            compatible_ignoring_qualifiers(a, b)
        }
        _ => super::types::compatible(a, b),
    }
}

fn differ_only_in_sign(a: &Type, b: &Type) -> bool {
    match (a, b) {
        (
            Type::Numeric(NumericType::Integer {
                width: a_width,
                signed: a_signed,
                bit_precise: a_bit_precise,
            }),
            Type::Numeric(NumericType::Integer {
                width: b_width,
                signed: b_signed,
                bit_precise: b_bit_precise,
            }),
        ) => a_width == b_width && a_bit_precise == b_bit_precise && a_signed != b_signed,
        _ => false,
    }
}

pub(super) fn constant_p_operand<'e>(callee: &Expr, arguments: &'e [Expr]) -> Option<&'e Expr> {
    match (&callee.value, arguments) {
        (ExprKind::Identifier(name), [operand]) if name == "__builtin_constant_p" => Some(operand),
        _ => None,
    }
}

pub(super) fn va_builtin(callee: &Expr) -> Option<VaBuiltin> {
    let ExprKind::Identifier(name) = &callee.value else {
        return None;
    };
    match name.as_str() {
        "__builtin_va_start" | "__builtin_c23_va_start" => Some(VaBuiltin::Start),
        "__builtin_va_end" => Some(VaBuiltin::End),
        "__builtin_va_copy" => Some(VaBuiltin::Copy),
        _ => None,
    }
}

impl Lowerer {
    fn va_list_place(&mut self, argument: &Expr) -> Result<Place, ResolveError> {
        let place = self.place(argument)?;
        if place.ty != Type::VaList {
            return Err(ResolveError::Unsupported("va builtin on non-va_list"));
        }
        Ok(place)
    }

    fn va_builtin(
        &mut self,
        e: &Expr,
        builtin: VaBuiltin,
        arguments: &[Expr],
    ) -> Result<Value, ResolveError> {
        let kind = match (builtin, arguments) {
            (VaBuiltin::Start, [list] | [list, _]) => ValueKind::VaStart {
                list: self.va_list_place(list)?,
            },
            (VaBuiltin::End, [list]) => ValueKind::VaEnd {
                list: self.va_list_place(list)?,
            },
            (VaBuiltin::Copy, [destination, source]) => ValueKind::VaCopy {
                destination: self.va_list_place(destination)?,
                source: self.va_list_place(source)?,
            },
            _ => return Err(ResolveError::Unsupported("va builtin argument count")),
        };
        Ok(self.value(e, Type::Void, kind))
    }
}

fn assignment_operator(op: AssignOp) -> Result<BinaryOp, ResolveError> {
    Ok(match op {
        AssignOp::AddAssign => BinaryOp::Add,
        AssignOp::SubAssign => BinaryOp::Sub,
        AssignOp::MulAssign => BinaryOp::Mul,
        AssignOp::DivAssign => BinaryOp::Div,
        AssignOp::RemAssign => BinaryOp::Rem,
        AssignOp::BitAndAssign => BinaryOp::BitAnd,
        AssignOp::BitOrAssign => BinaryOp::BitOr,
        AssignOp::BitXorAssign => BinaryOp::BitXor,
        AssignOp::ShiftLeftAssign => BinaryOp::ShiftLeft,
        AssignOp::ShiftRightAssign => BinaryOp::ShiftRight,
        AssignOp::Assign => return Err(ResolveError::Unsupported("non-compound assignment")),
    })
}
