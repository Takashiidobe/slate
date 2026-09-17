use super::numeric::{Context, ResolveError};
use super::types::TypeResolver;
use crate::ast::{Expr, ExprKind, NodeId, Span};
use crate::const_expr::{AssignOp, BinaryOp, Encoding, PostfixOp, UnaryOp};
use crate::ir::*;
use std::collections::HashMap;

pub(super) struct Lowerer {
    pub context: Context,
    pub types: TypeResolver,
    pub module: Module,
    pub names: NameResolution,
    pub bindings: HashMap<BindingId, Type>,
    pub type_spans: HashMap<TypeId, Span<TypeDefinition>>,
    pub next_id: u32,
}

impl Lowerer {
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
        self.names
            .bindings
            .iter()
            .find(|b| b.id == node && b.name == name)
            .or_else(|| self.names.bindings.iter().find(|b| b.name == name))
            .map(|b| b.value.id)
            .ok_or(ResolveError::Unsupported("missing declaration binding"))
    }

    fn reference(&self, e: &Expr) -> Result<BindingId, ResolveError> {
        self.names
            .references
            .iter()
            .find(|r| r.id == e.id)
            .map(|r| r.binding)
            .ok_or(ResolveError::Unsupported("missing expression binding"))
    }

    pub fn kind(&self, ty: &Type) -> Option<&TypeDefinitionKind> {
        match ty {
            Type::Defined(id) => self.types.definitions.get(id.0 as usize).map(|d| &d.kind),
            _ => None,
        }
    }

    pub fn pointer(&mut self, pointee: Type, is_const: bool) -> Type {
        Type::Pointer {
            pointee: Box::new(pointee),
            is_const,
        }
    }

    fn pointee(&self, ty: &Type) -> Result<Type, ResolveError> {
        match ty {
            Type::Pointer { pointee, .. } => Ok((**pointee).clone()),
            _ => Err(ResolveError::Unsupported("expected pointer")),
        }
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
        let mut result = match &value.ty {
            Type::Bool => return Ok(value),
            Type::Numeric(_) => self.context.condition(value),
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

    pub fn convert(
        &self,
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
        if to == Type::Bool {
            return self.condition(value, Some(reason));
        }
        if matches!(to, Type::Numeric(_)) && matches!(value.ty, Type::Numeric(_) | Type::Bool) {
            return Ok(self.context.convert(value, to, reason));
        }
        if self.pointee(&to).is_ok() {
            if matches!(&value.node.value, ValueKind::Constant(Number::Integer(n)) if *n == num_bigint::BigUint::default())
                || matches!(value.node.value, ValueKind::Null)
            {
                return Ok(self.value(&value.node, to, ValueKind::Null));
            }
            if let (Ok(a), Ok(b)) = (self.pointee(&value.ty), self.pointee(&to))
                && (a == b
                    || a == Type::Void
                    || b == Type::Void
                    || reason == ConversionReason::Explicit)
            {
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
        Err(ResolveError::Unsupported(
            "incompatible or unsupported conversion",
        ))
    }

    pub fn convert_expr(
        &self,
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

    fn place(&mut self, e: &Expr) -> Result<Place, ResolveError> {
        match &e.value {
            ExprKind::Paren(inner) => self.place(inner),
            ExprKind::Identifier(_) => {
                let id = self.reference(e)?;
                let ty = self
                    .bindings
                    .get(&id)
                    .ok_or(ResolveError::Unsupported("untyped binding"))?
                    .clone();
                Ok(Place {
                    ty,
                    kind: PlaceKind::Binding(id),
                })
            }
            ExprKind::Unary {
                op: UnaryOp::Deref,
                operand,
            } => {
                let value = self.expr(operand)?;
                Ok(Place {
                    ty: self.pointee(&value.ty)?,
                    kind: PlaceKind::Deref(Box::new(value)),
                })
            }
            ExprKind::Index { base, index } => {
                let base = self.expr(base)?;
                let index = self.expr(index)?;
                let (pointer_ty, kind) = self.binary(BinaryOp::Add, base, index)?;
                Ok(Place {
                    ty: self.pointee(&pointer_ty)?,
                    kind: PlaceKind::Deref(Box::new(self.value(e, pointer_ty, kind))),
                })
            }
            ExprKind::Member { base, field, arrow } => {
                let base = if *arrow {
                    let value = self.expr(base)?;
                    Place {
                        ty: self.pointee(&value.ty)?,
                        kind: PlaceKind::Deref(Box::new(value)),
                    }
                } else {
                    self.place(base)?
                };
                let Some(TypeDefinitionKind::Record {
                    fields: Some(fields),
                    ..
                }) = self.kind(&base.ty)
                else {
                    return Err(ResolveError::Unsupported(
                        "member of incomplete or non-record",
                    ));
                };
                let (index, field) = fields
                    .iter()
                    .enumerate()
                    .find(|(_, f)| f.name.as_deref() == Some(field.value.as_str()))
                    .ok_or(ResolveError::Unsupported("unknown or anonymous member"))?;
                if field.bit_width.is_some() {
                    return Err(ResolveError::Unsupported("bit-field access"));
                }
                Ok(Place {
                    ty: field.ty.clone(),
                    kind: PlaceKind::Field {
                        base: Box::new(base),
                        index,
                    },
                })
            }
            _ => Err(ResolveError::Unsupported(
                "expression is not a supported place",
            )),
        }
    }

    fn read(&mut self, e: &Expr, place: Place) -> Result<Value, ResolveError> {
        if let Type::Array { element, length } = &place.ty {
            let ty = self.pointer((**element).clone(), false);
            let length = *length;
            return Ok(self.value(e, ty, ValueKind::ArrayDecay { place, length }));
        }
        if matches!(place.ty, Type::Function { .. }) {
            return Err(ResolveError::Unsupported("function value or indirect call"));
        }
        Ok(self.value(e, place.ty.clone(), ValueKind::Read(place)))
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
                if element != other {
                    return Err(ResolveError::Unsupported(
                        "incompatible pointer subtraction",
                    ));
                }
                self.types.storage(element.clone())?;
                Ok((
                    Type::Numeric(NumericType::Integer {
                        width: self.context.target.pointer_width,
                        signed: true,
                    }),
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
        self.types.storage(element.clone())?;
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
                place,
                computation: Box::new(computation),
                postfix,
            },
        ))
    }

    fn unevaluated_type(&mut self, operand: &Expr) -> Result<Type, ResolveError> {
        if let ExprKind::Paren(inner) = &operand.value {
            return self.unevaluated_type(inner);
        }
        if let ExprKind::StringLiteral(lit) = &operand.value {
            if lit.encoding != Encoding::Plain {
                return Err(ResolveError::Unsupported("encoded string literal"));
            }
            return Ok(Type::Array {
                element: Box::new(Type::Numeric(NumericType::Integer {
                    width: 8,
                    signed: self.context.target.char_signed,
                })),
                length: Some(lit.code_units.len() as u64 + 1),
            });
        }
        let globals = self.module.globals.len();
        let next_id = self.next_id;
        let result = match self.place(operand) {
            Ok(place) => Ok(place.ty),
            Err(_) => self.expr(operand).map(|value| value.ty),
        };
        self.module.globals.truncate(globals);
        self.next_id = next_id;
        result
    }

    fn layout_constant(&mut self, e: &Expr, amount: u64, key: &str, detail: String) -> Value {
        self.module
            .metadata
            .entry(e.id)
            .or_default()
            .push((key.into(), detail));
        let ty = Type::Numeric(NumericType::Integer {
            width: self.context.target.pointer_width,
            signed: false,
        });
        self.value(e, ty, ValueKind::Constant(Number::Integer(amount.into())))
    }

    pub fn expr(&mut self, e: &Expr) -> Result<Value, ResolveError> {
        match &e.value {
            ExprKind::Paren(inner) => self.expr(inner),
            ExprKind::Identifier(_)
            | ExprKind::Member { .. }
            | ExprKind::Index { .. }
            | ExprKind::Unary {
                op: UnaryOp::Deref, ..
            } => {
                let place = self.place(e)?;
                self.read(e, place)
            }
            ExprKind::CharLiteral(lit) => {
                if lit.encoding != Encoding::Plain || lit.code_units.len() != 1 {
                    return Err(ResolveError::Unsupported("wide or multicharacter literal"));
                }
                Ok(self.value(
                    e,
                    self.context.int_type(),
                    ValueKind::Constant(Number::Integer(lit.code_units[0].into())),
                ))
            }
            ExprKind::NullPtrLiteral => {
                let ty = self.pointer(Type::Void, false);
                Ok(self.value(e, ty, ValueKind::Null))
            }
            ExprKind::StringLiteral(lit) => {
                if lit.encoding != Encoding::Plain {
                    return Err(ResolveError::Unsupported("encoded string literal"));
                }
                let mut bytes = lit
                    .code_units
                    .iter()
                    .map(|c| {
                        u8::try_from(*c).map_err(|_| ResolveError::Unsupported("string code unit"))
                    })
                    .collect::<Result<Vec<_>, _>>()?;
                bytes.push(0);
                let length = Some(bytes.len() as u64);
                let element = Type::Numeric(NumericType::Integer {
                    width: 8,
                    signed: self.context.target.char_signed,
                });
                let ty = Type::Array {
                    element: Box::new(element),
                    length,
                };
                let id = self.fresh();
                let initializer = self.value(e, ty.clone(), ValueKind::Bytes(bytes));
                self.module.globals.push(e.clone().with_value(Global {
                    variable: Variable {
                        id,
                        name: format!(".str{}", id.0),
                        ty: ty.clone(),
                        storage: StorageDuration::Static,
                        initializer: Some(initializer),
                    },
                    linkage: Linkage::Internal,
                    definition: true,
                }));
                self.read(
                    e,
                    Place {
                        ty,
                        kind: PlaceKind::Binding(id),
                    },
                )
            }
            ExprKind::Cast { ty, value } => {
                let to = self.types.resolve(&ty.specifiers, &ty.declarator)?.ty;
                let value = self.expr(value)?;
                if let Some(to) = to {
                    self.convert(value, to, ConversionReason::Explicit)
                } else {
                    let end = self.value(e, Type::Void, ValueKind::Void);
                    Ok(self.value(
                        e,
                        Type::Void,
                        ValueKind::Sequence {
                            left: Box::new(value),
                            right: Box::new(end),
                        },
                    ))
                }
            }
            ExprKind::Unary {
                op: UnaryOp::AddrOf,
                operand,
            } => {
                let place = self.place(operand)?;
                let ty = self.pointer(place.ty.clone(), false);
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
            ExprKind::Unary { op, operand } => {
                let value = self.expr(operand)?;
                match op {
                    UnaryOp::Plus => {
                        if !matches!(value.ty, Type::Bool | Type::Numeric(_)) {
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
                let right = self.expr(right)?;
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
                    return Ok(self.value(
                        e,
                        Type::Bool,
                        ValueKind::Compare {
                            op: if *op == BinaryOp::Equal {
                                CompareOp::Eq
                            } else {
                                CompareOp::Ne
                            },
                            left: Box::new(self.convert_expr(
                                left_expr,
                                left,
                                ty.clone(),
                                ConversionReason::UsualArith,
                            )?),
                            right: Box::new(self.convert_expr(
                                right_expr,
                                right,
                                ty,
                                ConversionReason::UsualArith,
                            )?),
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
                if matches!(left.ty, Type::Bool | Type::Numeric(_))
                    && matches!(right.ty, Type::Bool | Type::Numeric(_))
                {
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
            ExprKind::Call { callee, arguments } => {
                let mut callee = callee;
                while let ExprKind::Paren(inner) = &callee.value {
                    callee = inner;
                }
                if !matches!(callee.value, ExprKind::Identifier(_)) {
                    return Err(ResolveError::Unsupported("indirect call"));
                }
                let function = self.reference(callee)?;
                let ty = self
                    .bindings
                    .get(&function)
                    .ok_or(ResolveError::Unsupported("untyped callee"))?
                    .clone();
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
                            Type::Numeric(NumericType::Integer { width, .. })
                                if *width < self.context.target.int_width =>
                            {
                                self.context.int_type()
                            }
                            _ => value.ty.clone(),
                        };
                        self.convert(value, to, ConversionReason::Vararg)?
                    };
                    lowered.push(value);
                }
                Ok(self.value(
                    e,
                    return_type.as_ref().map_or(Type::Void, |ty| (**ty).clone()),
                    ValueKind::Call {
                        function,
                        arguments: lowered,
                    },
                ))
            }
            ExprKind::IntegerLiteral(_) | ExprKind::FloatLiteral(_) | ExprKind::BoolLiteral(_) => {
                self.context.resolve(e)
            }
            ExprKind::SizeOfType { ty } | ExprKind::AlignOf { ty } => {
                let ty = self
                    .types
                    .resolve(&ty.specifiers, &ty.declarator)?
                    .ty
                    .ok_or(ResolveError::Unsupported("void layout"))?;
                let layout = self.types.storage(ty.clone())?;
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
                let ty = self.unevaluated_type(operand)?;
                let layout = self.types.storage(ty.clone())?;
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
            ExprKind::OffsetOf { ty, member } => {
                let ty = self
                    .types
                    .resolve(&ty.specifiers, &ty.declarator)?
                    .ty
                    .ok_or(ResolveError::Unsupported("void offsetof"))?;
                let (_, offset) = self.types.offsetof_member(ty.clone(), member)?;
                Ok(self.layout_constant(e, offset, "offset_of", format!("{ty}.{member}")))
            }
            _ => Err(ResolveError::Unsupported("advanced expression")),
        }
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
