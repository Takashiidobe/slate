use super::atomic::{AtomicBuiltin, AtomicResult, atomic_builtin};
use super::builtins::{CustomBuiltin, DerivedSignature};
use super::ctype::{CTypeKind, QualType};
use super::expression::{
    SourceLocationBuiltin, choose_expr_operands, constant_p_operand, source_location_builtin,
    va_builtin,
};
use super::numeric::ResolveError;
use super::types::TypeResolver;
use crate::ast::{Expr, ExprKind, TypeName};
use crate::const_expr::{AssignOp, BinaryOp, UnaryOp};
use crate::ir::{Number, NumericType, Type, TypeDefinitionKind, ValueKind};

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
            return Ok(*typed);
        }
        let typed = self.type_expression(e)?;
        self.expression_types.insert(e.id, typed);
        Ok(typed)
    }

    pub(super) fn expression_type(&mut self, e: &Expr) -> Result<QualType, ResolveError> {
        Ok(self.typed(e)?.c)
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
                if *op == AssignOp::Assign {
                    let typed = self.typed(target)?;
                    if !typed.lvalue {
                        return Err(UNTYPED);
                    }
                    self.require_modifiable_lvalue(typed.c)?;
                    Typed::rvalue(self.ctypes.unqualified(typed.c))
                } else if self.ctypes.is_arithmetic(value) {
                    self.updated(target)?
                } else {
                    return Err(UNTYPED);
                }
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
                let tested = self.operand_type(condition)?;
                if !self.ctypes.is_scalar(tested) {
                    return Err(UNTYPED);
                }
                let left = match then_value {
                    Some(then_value) => self.operand_type(then_value)?,
                    None => tested,
                };
                let right = self.operand_type(else_value)?;
                let lp = self.ctypes.is_pointer(left);
                let rp = self.ctypes.is_pointer(right);
                if self.ctypes.is_arithmetic(left) && self.ctypes.is_arithmetic(right) {
                    let left = self.promoted(left);
                    let right = self.promoted(right);
                    Typed::rvalue(self.arithmetic_type(left, right)?)
                } else if lp && rp {
                    let then_value: &Expr = match then_value {
                        Some(then_value) => then_value,
                        None => condition,
                    };
                    let else_null = self.null_pointer(else_value)?.ok_or(UNTYPED)?;
                    let then_null = self.null_pointer(then_value)?.ok_or(UNTYPED)?;
                    Typed::rvalue(if else_null {
                        left
                    } else if then_null {
                        right
                    } else {
                        let rules = self.features().conditional_pointers;
                        self.ctypes
                            .merge_pointer(left, right, rules)
                            .ok_or(UNTYPED)?
                    })
                } else if lp {
                    Typed::rvalue(left)
                } else if rp {
                    Typed::rvalue(right)
                } else if self.ctypes.compatible_unqualified(left, right) {
                    Typed::rvalue(left)
                } else {
                    return Err(UNTYPED);
                }
            }
            ExprKind::Call { callee, arguments } => self.call_type(callee, arguments)?,
            ExprKind::SizeOfType { .. }
            | ExprKind::AlignOf { .. }
            | ExprKind::SizeOfExpr(_)
            | ExprKind::AlignOfExpr(_)
            | ExprKind::OffsetOf { .. } => {
                let target = self.target_info().clone();
                Typed::rvalue(self.ctypes.size_type(&target))
            }
            ExprKind::TypesCompatible { .. } => Typed::rvalue(self.ctypes.int()),
            ExprKind::LabelAddress(_) => {
                let void = self.ctypes.qual(CTypeKind::Void);
                Typed::rvalue(self.ctypes.pointer(void))
            }
            ExprKind::BitCast { ty, value } | ExprKind::ConvertVector { ty, value } => {
                self.typed(value)?;
                Typed::rvalue(self.type_name(ty)?)
            }
            ExprKind::VaArg { list, ty } => {
                if !self.typed(list)?.lvalue {
                    return Err(UNTYPED);
                }
                Typed::rvalue(self.type_name(ty)?)
            }
            ExprKind::Unary {
                op: UnaryOp::Real | UnaryOp::Imag,
                operand,
            } => {
                let typed = self.typed(operand)?;
                let complex = matches!(self.ctypes.canonical_kind(typed.c), CTypeKind::Complex(_));
                if typed.lvalue && complex {
                    let quals = self.ctypes.quals(typed.c);
                    Typed::lvalue(self.ctypes.arithmetic_component(typed.c).with(quals))
                } else {
                    let c = self.rvalue_type(typed);
                    if complex {
                        Typed::rvalue(self.ctypes.arithmetic_component(c))
                    } else if self.ctypes.is_arithmetic(c) {
                        Typed::rvalue(self.promoted(c))
                    } else {
                        return Err(UNTYPED);
                    }
                }
            }
            ExprKind::StatementExpression(body) => {
                match super::expression::statement_expression_parts(body).2 {
                    Some(result) => Typed::rvalue(self.operand_type(result)?),
                    None => Typed::rvalue(self.ctypes.qual(CTypeKind::Void)),
                }
            }
        })
    }

    pub(super) fn chosen_expr<'e>(
        &mut self,
        callee: &Expr,
        arguments: &'e [Expr],
    ) -> Result<&'e Expr, ResolveError> {
        let (condition, when_true, when_false) = choose_expr_operands(callee, arguments)
            .ok_or(ResolveError::Internal("__builtin_choose_expr"))?;
        let taken = self.constant_integer(condition)?;
        Ok(if taken.sign() == num_bigint::Sign::NoSign {
            when_false
        } else {
            when_true
        })
    }

    fn call_type(&mut self, callee: &Expr, arguments: &[Expr]) -> Result<Typed, ResolveError> {
        if let Some(builtin) = atomic_builtin(callee) {
            return self.atomic_type(builtin, arguments);
        }
        if let Some(operand) = constant_p_operand(callee, arguments) {
            self.typed(operand)?;
            return Ok(Typed::rvalue(self.ctypes.int()));
        }
        if arguments.is_empty()
            && let Some(builtin) = source_location_builtin(callee)
        {
            return Ok(Typed::rvalue(match builtin {
                SourceLocationBuiltin::Line | SourceLocationBuiltin::Column => self.ctypes.int(),
                _ => {
                    let char_type = self.ctypes.qual(CTypeKind::Char);
                    self.ctypes.pointer(char_type)
                }
            }));
        }
        if choose_expr_operands(callee, arguments).is_some() {
            let chosen = self.chosen_expr(callee, arguments)?;
            return self.typed(chosen);
        }
        if va_builtin(callee).is_some() {
            return Ok(Typed::rvalue(self.ctypes.qual(CTypeKind::Void)));
        }
        let signature = match self.builtin_callee(callee, arguments) {
            Some((builtin, _)) => {
                if let Some(custom) = super::builtins::custom_builtin(builtin) {
                    return self.custom_builtin_type(custom, arguments);
                }
                match super::builtins::derived_signature(builtin) {
                    Some(derived) => {
                        let first = match (derived, arguments.first()) {
                            (DerivedSignature::Declared, _) | (_, None) => None,
                            (_, Some(argument)) => {
                                let typed = self.typed(argument)?;
                                Some(self.ctypes.lvalue_conversion(typed.c))
                            }
                        };
                        self.derived_signature(builtin, derived, arguments.len(), first)?
                    }
                    None => self.builtin_signature(builtin).ok_or(UNTYPED)?,
                }
            }
            None => {
                let pointer = self.operand_type(callee)?;
                self.ctypes.pointee(pointer).ok_or(UNTYPED)?
            }
        };
        let (returned, ..) = self.ctypes.function_parts(signature).ok_or(UNTYPED)?;
        Ok(Typed::rvalue(returned))
    }

    fn atomic_type(
        &mut self,
        builtin: AtomicBuiltin,
        arguments: &[Expr],
    ) -> Result<Typed, ResolveError> {
        let boolean = self.ctypes.qual(CTypeKind::Bool);
        let result = builtin.result(arguments).ok_or(UNTYPED)?;
        Ok(Typed::rvalue(match result {
            AtomicResult::Void => self.ctypes.qual(CTypeKind::Void),
            AtomicResult::Bool => boolean,
            AtomicResult::Object(object) | AtomicResult::Fetched(object) => {
                let pointer = self.operand_type(object)?;
                let pointee = self.ctypes.pointee(pointer).ok_or(UNTYPED)?;
                match self.ir_type(pointee) {
                    Type::Void | Type::Function { .. } => return Err(UNTYPED),
                    Type::Bool if matches!(result, AtomicResult::Fetched(_)) => {
                        self.ctypes.unqualified(pointee)
                    }
                    _ => pointee,
                }
            }
            AtomicResult::Flag(object) => {
                let pointer = self.operand_type(object)?;
                let pointee = self.ctypes.pointee(pointer).ok_or(UNTYPED)?;
                if self.ir_type(pointee) == Type::Bool {
                    pointee
                } else {
                    boolean
                }
            }
        }))
    }

    fn custom_builtin_type(
        &mut self,
        custom: CustomBuiltin,
        arguments: &[Expr],
    ) -> Result<Typed, ResolveError> {
        Ok(Typed::rvalue(match custom {
            CustomBuiltin::Overflow(_) => self.ctypes.qual(CTypeKind::Bool),
            CustomBuiltin::FloatClass(_)
            | CustomBuiltin::QuietCompare(_)
            | CustomBuiltin::Unordered
            | CustomBuiltin::LessGreater
            | CustomBuiltin::InfSign
            | CustomBuiltin::FloatClassify
            | CustomBuiltin::ClassifyType => self.ctypes.int(),
            CustomBuiltin::AddressOf => {
                let [operand] = arguments else {
                    return Err(UNTYPED);
                };
                let typed = self.typed(operand)?;
                if !typed.lvalue || typed.bits.is_some() {
                    return Err(UNTYPED);
                }
                self.ctypes.pointer(typed.c)
            }
            CustomBuiltin::Complex => {
                let [real, imaginary] = arguments else {
                    return Err(UNTYPED);
                };
                let real = self.operand_type(real)?;
                let real = self.real_floating_component(real)?;
                let imaginary = self.operand_type(imaginary)?;
                let imaginary = self.real_floating_component(imaginary)?;
                let target = self.target_info().clone();
                let common = self.ctypes.usual_real_type(real, imaginary, &target)?;
                self.complex_of(common)
            }
            CustomBuiltin::Shuffle => return Err(UNTYPED),
        }))
    }

    fn null_pointer(&mut self, e: &Expr) -> Result<Option<bool>, ResolveError> {
        Ok(match &e.value {
            ExprKind::Paren(inner) => self.null_pointer(inner)?,
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
                self.null_pointer(selected)?
            }
            ExprKind::Call { callee, arguments }
                if choose_expr_operands(callee, arguments).is_some() =>
            {
                let chosen = self.chosen_expr(callee, arguments)?;
                self.null_pointer(chosen)?
            }
            ExprKind::NullPtrLiteral => Some(true),
            ExprKind::Cast { value, .. } => {
                let from = self.operand_type(value)?;
                if self.ctypes.is_integer(from) {
                    self.integer_zero(value)
                } else if self.ctypes.is_pointer(from) {
                    self.null_pointer(value)?
                } else {
                    None
                }
            }
            _ => Some(false),
        })
    }

    fn integer_zero(&self, e: &Expr) -> Option<bool> {
        match &e.value {
            ExprKind::Paren(inner) => self.integer_zero(inner),
            ExprKind::IntegerLiteral(literal) if !literal.imaginary => {
                Some(crate::const_expr::Parser::evaluate_ast(e).is_ok_and(|number| number == 0))
            }
            ExprKind::Identifier(_) if self.object(e).is_some() => Some(false),
            ExprKind::Identifier(_) => Some(matches!(
                &self.constant(e)?.value.node.value,
                ValueKind::Constant(Number::Integer(n)) if *n == num_bigint::BigUint::default()
            )),
            _ => None,
        }
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
