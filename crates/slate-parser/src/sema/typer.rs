use super::ctype::{CTypeKind, QualType};
use super::numeric::ResolveError;
use super::types::TypeResolver;
use crate::ast::{Expr, ExprKind, TypeName, TypeOfOperand, TypeSpecifier};
use crate::const_expr::{AssignOp, BinaryOp, UnaryOp};
use crate::ir::{NumericType, Type, TypeDefinitionKind};

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) struct Typed {
    pub c: QualType,
    pub lvalue: bool,
    pub bits: Option<u32>,
}

impl Typed {
    fn rvalue(c: QualType) -> Self {
        Self {
            c,
            lvalue: false,
            bits: None,
        }
    }

    fn lvalue(c: QualType) -> Self {
        Self {
            c,
            lvalue: true,
            bits: None,
        }
    }
}

const UNTYPED: ResolveError = ResolveError::Unimplemented("expression without a typing rule");

impl TypeResolver {
    pub(super) fn typed(&mut self, e: &Expr) -> Result<Typed, ResolveError> {
        if let Some(typed) = self.expression_types.get(&e.id) {
            return typed.ok_or(UNTYPED);
        }
        let typed = self.type_expression(e);
        self.expression_types
            .insert(e.id, typed.as_ref().ok().copied());
        typed
    }

    pub(super) fn rvalue_type(&mut self, typed: Typed) -> QualType {
        let decays = self.ctypes.element(typed.c).is_some() || self.ctypes.is_function(typed.c);
        if !typed.lvalue && !decays {
            return typed.c;
        }
        let c = self.ctypes.lvalue_conversion(typed.c);
        match typed.bits {
            Some(bits) => {
                let c = self.ctypes.enum_underlying(c).unwrap_or(c);
                let target = self.target_info().clone();
                self.ctypes.integer_promotion(c, Some(bits), &target)
            }
            None => c,
        }
    }

    fn operand_type(&mut self, e: &Expr) -> Result<QualType, ResolveError> {
        let typed = self.typed(e)?;
        Ok(self.rvalue_type(typed))
    }

    fn promoted(&mut self, c: QualType) -> QualType {
        let c = self.ctypes.enum_underlying(c).unwrap_or(c);
        let target = self.target_info().clone();
        self.ctypes.integer_promotion(c, None, &target)
    }

    fn type_name(&mut self, ty: &TypeName) -> Result<QualType, ResolveError> {
        if let TypeSpecifier::TypeOf(TypeOfOperand::Expression(expr))
        | TypeSpecifier::TypeOfUnqual(TypeOfOperand::Expression(expr)) = &ty.specifiers.ty
            && !self.typeof_operands.contains_key(&expr.id)
        {
            return Err(UNTYPED);
        }
        self.resolve(&ty.specifiers, &ty.declarator)
    }

    fn is_decimal(&self, c: QualType) -> Option<bool> {
        match self.ir_type(c) {
            Type::Numeric(NumericType::Float(format))
            | Type::Complex(NumericType::Float(format)) => Some(format.is_decimal()),
            _ => None,
        }
    }

    fn member_type(&self, record: QualType, name: &str) -> Option<(QualType, Option<u32>)> {
        let CTypeKind::Record { id, .. } = self.ctypes.canonical_kind(record) else {
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
        let quals = self.ctypes.quals(record);
        for (field, member) in fields.iter().zip(types) {
            if field.name.as_deref() == Some(name) {
                return Some((member.with(quals), field.bit_width));
            }
            if field.name.is_none()
                && let Some((found, bits)) = self.member_type(*member, name)
            {
                return Some((found.with(quals), bits));
            }
        }
        None
    }

    fn type_expression(&mut self, e: &Expr) -> Result<Typed, ResolveError> {
        Ok(match &e.value {
            ExprKind::Paren(inner) => self.typed(inner)?,
            ExprKind::Identifier(_) => match self.object(e) {
                Some(c) => Typed::lvalue(c),
                None => Typed::rvalue(self.constant(e).ok_or(UNTYPED)?.c),
            },
            ExprKind::IntegerLiteral(_) | ExprKind::FloatLiteral(_) | ExprKind::BoolLiteral(_) => {
                let target = self.target_info().clone();
                let features = self.features();
                Typed::rvalue(self.literal_type(&target, features, e)?)
            }
            ExprKind::CharLiteral(literal) => Typed::rvalue(self.character_constant(literal)?.0),
            ExprKind::StringLiteral(literal) => Typed::lvalue(self.string_type(literal)),
            ExprKind::NullPtrLiteral => Typed::rvalue(self.ctypes.qual(CTypeKind::NullPtr)),
            ExprKind::Generic {
                controlling,
                associations,
            } => {
                let controlling = match controlling {
                    crate::ast::GenericControl::Type { ty } => self.type_name(ty)?,
                    crate::ast::GenericControl::Expr(expr) => {
                        let typed = self.typed(expr)?;
                        self.ctypes.lvalue_conversion(typed.c)
                    }
                };
                let selected = self.select_association(controlling, associations)?;
                self.typed(selected)?
            }
            ExprKind::Unary {
                op: UnaryOp::Deref,
                operand,
            } => {
                let pointer = self.operand_type(operand)?;
                Typed::lvalue(self.ctypes.pointee(pointer).ok_or(UNTYPED)?)
            }
            ExprKind::Unary {
                op: UnaryOp::AddrOf,
                operand,
            } => {
                let typed = self.typed(operand)?;
                if !typed.lvalue || typed.bits.is_some() {
                    return Err(UNTYPED);
                }
                Typed::rvalue(self.ctypes.pointer(typed.c))
            }
            ExprKind::Unary {
                op: op @ (UnaryOp::Plus | UnaryOp::Minus | UnaryOp::BitNot),
                operand,
            } => {
                let c = self.operand_type(operand)?;
                let accepted = if *op == UnaryOp::BitNot {
                    self.ctypes.is_integer(c)
                } else {
                    self.ctypes.is_arithmetic(c)
                };
                if !accepted {
                    return Err(UNTYPED);
                }
                Typed::rvalue(self.promoted(c))
            }
            ExprKind::Unary {
                op: UnaryOp::Not,
                operand,
            } => {
                let c = self.operand_type(operand)?;
                if !self.ctypes.is_scalar(c) {
                    return Err(UNTYPED);
                }
                Typed::rvalue(self.ctypes.int())
            }
            ExprKind::Unary {
                op: UnaryOp::PreIncrement | UnaryOp::PreDecrement,
                operand,
            }
            | ExprKind::Postfix { operand, .. } => self.updated(operand)?,
            ExprKind::Assign { op, target, value } => {
                let value = self.operand_type(value)?;
                if *op != AssignOp::Assign && !self.ctypes.is_arithmetic(value) {
                    return Err(UNTYPED);
                }
                self.updated(target)?
            }
            ExprKind::Comma { left, right } => {
                self.typed(left)?;
                Typed::rvalue(self.operand_type(right)?)
            }
            ExprKind::Cast { ty, value } => {
                self.typed(value)?;
                let to = self.type_name(ty)?;
                Typed::rvalue(self.ctypes.unqualified(to))
            }
            ExprKind::CompoundLiteral { ty, initializer } => {
                let resolved = self.type_name(ty)?;
                let c = match self.ctypes.element(resolved) {
                    Some((element, super::ctype::Extent::Incomplete)) => {
                        let length = self.inferred_array_length(element, initializer)?;
                        self.ctypes.qual(CTypeKind::Array {
                            element,
                            extent: super::ctype::Extent::Fixed(length),
                        })
                    }
                    _ => resolved,
                };
                Typed::lvalue(c)
            }
            ExprKind::Index { base, index } => {
                let base = self.operand_type(base)?;
                let index = self.operand_type(index)?;
                let (pointer, index) = if self.ctypes.is_pointer(base) {
                    (base, index)
                } else {
                    (index, base)
                };
                if !self.ctypes.is_integer(index) {
                    return Err(UNTYPED);
                }
                Typed::lvalue(self.ctypes.pointee(pointer).ok_or(UNTYPED)?)
            }
            ExprKind::Member { base, field, arrow } => {
                let (record, lvalue) = if *arrow {
                    let pointer = self.operand_type(base)?;
                    (self.ctypes.pointee(pointer).ok_or(UNTYPED)?, true)
                } else {
                    let typed = self.typed(base)?;
                    (typed.c, typed.lvalue)
                };
                let (c, bits) = self.member_type(record, &field.value).ok_or(UNTYPED)?;
                Typed { c, lvalue, bits }
            }
            ExprKind::Binary { op, left, right } => {
                let left = self.operand_type(left)?;
                let right = self.operand_type(right)?;
                Typed::rvalue(self.binary_type(*op, left, right)?)
            }
            ExprKind::Conditional {
                condition,
                then_value,
                else_value,
            } => {
                let condition = self.operand_type(condition)?;
                if !self.ctypes.is_scalar(condition) {
                    return Err(UNTYPED);
                }
                let left = match then_value {
                    Some(then_value) => self.operand_type(then_value)?,
                    None => condition,
                };
                let right = self.operand_type(else_value)?;
                if self.ctypes.is_arithmetic(left) && self.ctypes.is_arithmetic(right) {
                    let left = self.promoted(left);
                    let right = self.promoted(right);
                    Typed::rvalue(self.arithmetic_type(left, right)?)
                } else if self.ctypes.is_pointer(left) || self.ctypes.is_pointer(right) {
                    return Err(UNTYPED);
                } else if self.ctypes.compatible_unqualified(left, right) {
                    Typed::rvalue(left)
                } else {
                    return Err(UNTYPED);
                }
            }
            _ => return Err(UNTYPED),
        })
    }

    fn updated(&mut self, target: &Expr) -> Result<Typed, ResolveError> {
        let typed = self.typed(target)?;
        if !typed.lvalue || !self.ctypes.is_scalar(typed.c) {
            return Err(UNTYPED);
        }
        self.require_modifiable_lvalue(typed.c)?;
        Ok(Typed::rvalue(self.ctypes.unqualified(typed.c)))
    }

    fn binary_type(
        &mut self,
        op: BinaryOp,
        left: QualType,
        right: QualType,
    ) -> Result<QualType, ResolveError> {
        let lp = self.ctypes.is_pointer(left) || self.ctypes.is_nullptr(left);
        let rp = self.ctypes.is_pointer(right) || self.ctypes.is_nullptr(right);
        match op {
            BinaryOp::And | BinaryOp::Or => {
                if !self.ctypes.is_scalar(left) || !self.ctypes.is_scalar(right) {
                    return Err(UNTYPED);
                }
                return Ok(self.ctypes.int());
            }
            BinaryOp::Equal | BinaryOp::NotEqual if lp || rp => return Ok(self.ctypes.int()),
            BinaryOp::Less | BinaryOp::LessEqual | BinaryOp::Greater | BinaryOp::GreaterEqual
                if lp && rp =>
            {
                return Ok(self.ctypes.int());
            }
            BinaryOp::Sub if lp && rp => {
                let (Some(a), Some(b)) = (self.ctypes.pointee(left), self.ctypes.pointee(right))
                else {
                    return Err(UNTYPED);
                };
                let a = self.ctypes.canonical(a).local_unqualified();
                let b = self.ctypes.canonical(b).local_unqualified();
                if !self.ctypes.compatible(a, b) {
                    return Err(UNTYPED);
                }
                let target = self.target_info().clone();
                return Ok(self.ctypes.ptrdiff_type(&target));
            }
            BinaryOp::Add | BinaryOp::Sub if lp && self.ctypes.is_integer(right) => {
                return Ok(left);
            }
            BinaryOp::Add if rp && self.ctypes.is_integer(left) => return Ok(right),
            _ if lp || rp => return Err(UNTYPED),
            _ => {}
        }
        if let (Some(a), Some(b)) = (self.is_decimal(left), self.is_decimal(right))
            && a != b
        {
            return Err(UNTYPED);
        }
        let left = self.promoted(left);
        let right = self.promoted(right);
        Ok(self.binary_types(op, left, right)?.result)
    }
}
