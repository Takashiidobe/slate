use super::expression::Lowerer;
use super::numeric::ResolveError;
use super::types::Ordinary;
use crate::ast::{
    self, MsAsmBinaryOp, MsAsmExpr, MsAsmOperator, MsAsmSegment, MsAsmSize, Register, Span,
};
use crate::ir::{BindingId, BindingKind, Place, Type, TypeDefinitionKind};

#[expect(dead_code, reason = "lowered by slate-parser-25m.6.3")]
#[derive(Debug)]
pub(super) struct MsAsmLine {
    pub label: Option<String>,
    pub prefixes: Vec<String>,
    pub mnemonic: Option<String>,
    pub operands: Vec<MsAsmOperand>,
}

#[expect(dead_code, reason = "lowered by slate-parser-25m.6.3")]
#[derive(Debug)]
pub(super) enum MsAsmOperand {
    Register(Register),
    Segment(MsAsmSegment),
    St(u8),
    Immediate(i64),
    Reference(MsAsmReference),
}

#[expect(dead_code, reason = "lowered by slate-parser-25m.6.3")]
#[derive(Debug)]
pub(super) struct MsAsmReference {
    pub symbol: Option<MsAsmSymbol>,
    pub displacement: i64,
    pub base: Option<Register>,
    pub index: Option<(Register, i64)>,
    pub size: Option<MsAsmSize>,
    pub segment: Option<MsAsmSegment>,
    pub offset: bool,
    pub short: bool,
}

#[expect(dead_code, reason = "lowered by slate-parser-25m.6.3")]
#[derive(Debug)]
pub(super) enum MsAsmSymbol {
    Object(Place),
    Function(BindingId),
    Label(String),
}

#[derive(Default)]
struct Value {
    symbol: Option<MsAsmSymbol>,
    ty: Option<Type>,
    constant: i64,
    registers: Vec<(Register, i64)>,
    bracketed: bool,
    size: Option<MsAsmSize>,
    segment: Option<MsAsmSegment>,
    offset: bool,
    short: bool,
}

impl Value {
    fn constant(constant: i64) -> Self {
        Self {
            constant,
            ..Self::default()
        }
    }

    fn as_constant(&self) -> Option<i64> {
        let plain = self.symbol.is_none() && self.registers.is_empty() && !self.bracketed;
        plain.then_some(self.constant)
    }

    fn add(mut self, other: Value) -> Result<Value, ResolveError> {
        if self.symbol.is_some() && other.symbol.is_some() {
            return Err(ResolveError::Invalid("two C names in one `__asm` operand"));
        }
        self.symbol = self.symbol.or(other.symbol);
        self.ty = self.ty.or(other.ty);
        self.constant = self.constant.wrapping_add(other.constant);
        self.registers.extend(other.registers);
        if self.registers.len() > 2 {
            return Err(ResolveError::Invalid(
                "more than two registers in one `__asm` operand",
            ));
        }
        self.bracketed |= other.bracketed;
        self.size = self.size.or(other.size);
        self.segment = self.segment.or(other.segment);
        self.offset |= other.offset;
        self.short |= other.short;
        Ok(self)
    }
}

impl Lowerer {
    pub(super) fn ms_asm(&mut self, asm: &ast::MsAsm) -> Result<Vec<MsAsmLine>, ResolveError> {
        let mut lines = Vec::with_capacity(asm.instructions.len());
        for instruction in &asm.instructions {
            let instruction = &instruction.value;
            let mut operands = Vec::with_capacity(instruction.operands.len());
            for operand in &instruction.operands {
                operands.push(match &operand.value {
                    MsAsmExpr::SegmentRegister(segment) => MsAsmOperand::Segment(*segment),
                    MsAsmExpr::St(index) => MsAsmOperand::St(*index),
                    _ => classify(self.ms_asm_value(operand)?)?,
                });
            }
            lines.push(MsAsmLine {
                label: instruction.label.as_ref().map(|label| label.value.clone()),
                prefixes: instruction
                    .prefixes
                    .iter()
                    .map(|prefix| prefix.value.clone())
                    .collect(),
                mnemonic: instruction
                    .mnemonic
                    .as_ref()
                    .map(|mnemonic| mnemonic.value.clone()),
                operands,
            });
        }
        Ok(lines)
    }

    fn ms_asm_value(&mut self, expr: &Span<MsAsmExpr>) -> Result<Value, ResolveError> {
        Ok(match &expr.value {
            MsAsmExpr::Register(register) => Value {
                registers: vec![(register.clone(), 1)],
                ..Value::default()
            },
            MsAsmExpr::SegmentRegister(_) | MsAsmExpr::St(_) => {
                return Err(ResolveError::Invalid(
                    "segment or x87 register inside an `__asm` expression",
                ));
            }
            MsAsmExpr::Number(number) => Value::constant(*number as i64),
            MsAsmExpr::Name(name) => self.ms_asm_name(expr, name)?,
            MsAsmExpr::Member { base, field } => {
                let mut value = self.ms_asm_value(base)?;
                let ty = value.ty.take().ok_or(ResolveError::Invalid(
                    "`__asm` member of an untyped operand",
                ))?;
                let ty = self.unaliased(ty);
                let (ty, offset) = self.types.offsetof_field(ty, &field.value).map_err(|_| {
                    ResolveError::Invalid("no such struct or union member in `__asm`")
                })?;
                value.constant = value.constant.wrapping_add(offset as i64);
                value.ty = Some(ty);
                value
            }
            MsAsmExpr::Index { base, index } => {
                let base = self.ms_asm_value(base)?;
                let mut value = base.add(self.ms_asm_value(index)?)?;
                value.bracketed = true;
                value
            }
            MsAsmExpr::Bracket(operand) => {
                let mut value = self.ms_asm_value(operand)?;
                value.bracketed = true;
                value
            }
            MsAsmExpr::Binary { op, lhs, rhs } => {
                let lhs = self.ms_asm_value(lhs)?;
                let rhs = self.ms_asm_value(rhs)?;
                binary(*op, lhs, rhs)?
            }
            MsAsmExpr::Negate(operand) => {
                let value = self.ms_asm_value(operand)?;
                let constant = value
                    .as_constant()
                    .ok_or(ResolveError::Invalid("`__asm` negation of a non-constant"))?;
                Value::constant(constant.wrapping_neg())
            }
            MsAsmExpr::Ptr { size, operand } => {
                let mut value = self.ms_asm_value(operand)?;
                value.size = Some(*size);
                value
            }
            MsAsmExpr::Segment { segment, operand } => {
                let mut value = self.ms_asm_value(operand)?;
                value.segment = Some(*segment);
                value.bracketed = true;
                value
            }
            MsAsmExpr::Operator { operator, operand } => {
                let mut value = self.ms_asm_value(operand)?;
                match operator {
                    MsAsmOperator::Offset => {
                        if value.symbol.is_none() {
                            return Err(ResolveError::Invalid(
                                "`OFFSET` needs a C name or label in `__asm`",
                            ));
                        }
                        value.offset = true;
                        value.ty = None;
                        value
                    }
                    MsAsmOperator::Short => {
                        value.short = true;
                        value
                    }
                    MsAsmOperator::Type | MsAsmOperator::Length | MsAsmOperator::Size => {
                        let ty = value.ty.ok_or(ResolveError::Invalid(
                            "`TYPE`, `LENGTH` and `SIZE` need a C object or type in `__asm`",
                        ))?;
                        Value::constant(self.ms_asm_type_operator(*operator, ty)?)
                    }
                }
            }
        })
    }

    fn ms_asm_name(&mut self, expr: &Span<MsAsmExpr>, name: &str) -> Result<Value, ResolveError> {
        let Some(kind) = self
            .names
            .references
            .iter()
            .find(|reference| reference.id == expr.id)
            .map(|reference| reference.kind)
        else {
            return Ok(Value {
                symbol: Some(MsAsmSymbol::Label(name.to_owned())),
                ..Value::default()
            });
        };
        let identifier = Box::new(
            expr.clone()
                .with_value(ast::ExprKind::Identifier(name.to_owned())),
        );
        Ok(match kind {
            BindingKind::Object | BindingKind::Parameter => {
                let place = self.place(&identifier)?.place;
                Value {
                    ty: Some(place.ty.clone()),
                    symbol: Some(MsAsmSymbol::Object(place)),
                    ..Value::default()
                }
            }
            BindingKind::Function => Value {
                symbol: Some(MsAsmSymbol::Function(self.reference(&identifier)?)),
                ..Value::default()
            },
            BindingKind::Enumerator => {
                let constant = self.types.constant_integer(&identifier)?;
                Value::constant(
                    i64::try_from(constant)
                        .map_err(|_| ResolveError::Invalid("`__asm` enumerator out of range"))?,
                )
            }
            BindingKind::Typedef => {
                let Some(Ordinary::Alias(alias)) = self.types.lookup(name) else {
                    return Err(ResolveError::Unsupported("unknown typedef"));
                };
                let alias = *alias;
                Value {
                    ty: Some(self.types.ir_type(alias)),
                    ..Value::default()
                }
            }
            BindingKind::Tag | BindingKind::Label => {
                return Err(ResolveError::Invalid("name cannot be used in `__asm`"));
            }
        })
    }

    fn ms_asm_type_operator(&self, operator: MsAsmOperator, ty: Type) -> Result<i64, ResolveError> {
        let ty = self.unaliased(ty);
        let size = |ty: Type| {
            self.types
                .storage(ty)
                .map(|storage| storage.size_bytes as i64)
        };
        let (element, length) = match &ty {
            Type::Array {
                element,
                length: Some(length),
            } => ((**element).clone(), *length as i64),
            _ => (ty.clone(), 1),
        };
        match operator {
            MsAsmOperator::Type => size(element),
            MsAsmOperator::Length => Ok(length),
            _ => size(ty),
        }
    }

    fn unaliased(&self, mut ty: Type) -> Type {
        while let Some(TypeDefinitionKind::Alias(inner)) = self.kind(&ty) {
            ty = inner.clone();
        }
        ty
    }
}

fn binary(op: MsAsmBinaryOp, lhs: Value, rhs: Value) -> Result<Value, ResolveError> {
    match op {
        MsAsmBinaryOp::Add => lhs.add(rhs),
        MsAsmBinaryOp::Sub => {
            let constant = rhs.as_constant().ok_or(ResolveError::Invalid(
                "`__asm` subtraction needs a constant right operand",
            ))?;
            lhs.add(Value::constant(constant.wrapping_neg()))
        }
        MsAsmBinaryOp::Mul => match (lhs.as_constant(), rhs.as_constant()) {
            (Some(lhs), Some(rhs)) => Ok(Value::constant(lhs.wrapping_mul(rhs))),
            (Some(scale), None) => scaled(rhs, scale),
            (None, Some(scale)) => scaled(lhs, scale),
            (None, None) => Err(ResolveError::Invalid(
                "`__asm` multiplication needs a constant operand",
            )),
        },
        MsAsmBinaryOp::Div => match (lhs.as_constant(), rhs.as_constant()) {
            (Some(lhs), Some(rhs)) => lhs
                .checked_div(rhs)
                .map(Value::constant)
                .ok_or(ResolveError::Invalid("`__asm` division by zero")),
            _ => Err(ResolveError::Invalid(
                "`__asm` division needs constant operands",
            )),
        },
    }
}

fn scaled(mut value: Value, scale: i64) -> Result<Value, ResolveError> {
    match value.registers.as_mut_slice() {
        [(_, factor)] if value.symbol.is_none() && value.constant == 0 && *factor == 1 => {
            *factor = scale;
            Ok(value)
        }
        _ => Err(ResolveError::Invalid(
            "`__asm` can only scale a single register",
        )),
    }
}

fn classify(value: Value) -> Result<MsAsmOperand, ResolveError> {
    let bare = value.symbol.is_none()
        && !value.bracketed
        && value.segment.is_none()
        && value.ty.is_none()
        && value.size.is_none();
    if bare
        && value.constant == 0
        && let [(register, 1)] = value.registers.as_slice()
    {
        return Ok(MsAsmOperand::Register(register.clone()));
    }
    if bare && value.registers.is_empty() {
        return Ok(MsAsmOperand::Immediate(value.constant));
    }
    let mut registers = value.registers.into_iter();
    let (base, index) = match (registers.next(), registers.next()) {
        (None, _) => (None, None),
        (Some((base, 1)), None) => (Some(base), None),
        (Some(index), None) => (None, Some(index)),
        (Some((base, 1)), Some(index)) | (Some(index), Some((base, 1))) => {
            (Some(base), Some(index))
        }
        (Some(_), Some(_)) => {
            return Err(ResolveError::Invalid(
                "`__asm` operand scales two registers",
            ));
        }
    };
    Ok(MsAsmOperand::Reference(MsAsmReference {
        symbol: value.symbol,
        displacement: value.constant,
        base,
        index,
        size: value.size,
        segment: value.segment,
        offset: value.offset,
        short: value.short,
    }))
}
