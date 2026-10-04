use super::asm::register;
use super::expression::Lowerer;
use super::ms_asm_effects::{X87_STACK, clobbered, effects, union, writes};
use super::numeric::ResolveError;
use super::operand::Lvalue;
use crate::ast::{
    self, MsAsmBinaryOp, MsAsmExpr, MsAsmOperator, MsAsmSegment, MsAsmSize, Register, Span,
};
use crate::ir::{
    AsmAccess, AsmClobber, AsmConstraint, AsmDialect, AsmMemory, AsmOperand, AsmOperandKind,
    AsmOptions, AsmPiece, AsmSymbol, BindingId, BindingKind, InlineAsm, PlaceKind, Type,
    TypeDefinitionKind, ValueKind,
};
use crate::lexer::Keyword;
use crate::target::x86::decode_register;
use std::collections::{BTreeSet, HashMap};
use std::fmt::Write;

enum MsAsmOperand {
    Register(Register),
    Segment(MsAsmSegment),
    St(u8),
    Immediate(i64),
    Reference(MsAsmReference),
}

struct MsAsmReference {
    symbol: Option<MsAsmSymbol>,
    ty: Option<Type>,
    displacement: i64,
    base: Option<Register>,
    index: Option<(Register, i64)>,
    size: Option<MsAsmSize>,
    segment: Option<MsAsmSegment>,
    offset: bool,
    short: bool,
}

enum MsAsmSymbol {
    Object {
        name: String,
        lvalue: Lvalue,
        anchor: Span<()>,
    },
    Function(BindingId),
    Label(String),
    CLabel(BindingId),
}

#[derive(Default)]
struct Lowered {
    pieces: Vec<AsmPiece>,
    operands: Vec<AsmOperand>,
    objects: HashMap<BindingId, usize>,
    functions: HashMap<BindingId, usize>,
}

impl Lowered {
    fn text(&mut self, text: &str) {
        match self.pieces.last_mut() {
            Some(AsmPiece::Text(previous)) => previous.push_str(text),
            _ => self.pieces.push(AsmPiece::Text(text.to_owned())),
        }
    }

    fn operand(&mut self, name: Option<String>, kind: AsmOperandKind) -> usize {
        self.operands.push(AsmOperand {
            name,
            constraint: AsmConstraint {
                alternatives: Vec::new(),
            },
            kind,
            width: None,
            selected: None,
        });
        self.operands.len() - 1
    }
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
            return Err(ResolveError::Rejected("two C names in one `__asm` operand"));
        }
        self.symbol = self.symbol.or(other.symbol);
        self.ty = self.ty.or(other.ty);
        self.constant = self.constant.wrapping_add(other.constant);
        self.registers.extend(other.registers);
        if self.registers.len() > 2 {
            return Err(ResolveError::Rejected(
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
    pub(super) fn ms_asm(&mut self, asm: &ast::MsAsm) -> Result<InlineAsm, ResolveError> {
        let mut lowered = Lowered {
            operands: self.ms_asm_return_operands()?,
            ..Lowered::default()
        };
        let mut clobbers = BTreeSet::new();
        let mut labels = Vec::new();
        for (line, instruction) in asm.instructions.iter().enumerate() {
            let instruction = &instruction.value;
            if line > 0 {
                lowered.text("\n");
            }
            let mut operands = Vec::with_capacity(instruction.operands.len());
            for operand in &instruction.operands {
                operands.push(match &operand.value {
                    MsAsmExpr::SegmentRegister(segment) => MsAsmOperand::Segment(*segment),
                    MsAsmExpr::St(index) => MsAsmOperand::St(*index),
                    _ => classify(self.ms_asm_value(operand)?)?,
                });
            }
            if let Some(label) = &instruction.label {
                if let Some(binding) = self.names.label_definitions.get(&label.id) {
                    lowered.pieces.push(AsmPiece::EntryLabel {
                        name: label.value.to_lowercase(),
                        binding: *binding,
                    });
                } else {
                    lowered
                        .pieces
                        .push(AsmPiece::LocalLabel(label.value.to_lowercase()));
                }
                lowered.text(":");
                if instruction.mnemonic.is_some() {
                    lowered.text(" ");
                }
            }
            for prefix in &instruction.prefixes {
                lowered.text(&prefix.value);
                lowered.text(" ");
            }
            let Some(mnemonic) = &instruction.mnemonic else {
                continue;
            };
            let lower = mnemonic.value.to_lowercase();
            lowered.text(if lower == "_emit" {
                ".byte"
            } else {
                &mnemonic.value
            });
            let sized = needs_size(&lower, &operands);
            let effects = effects(&lower, &instruction.prefixes, operands.len());
            clobbers.extend(effects.implicit);
            if effects.x87 {
                clobbers.extend(X87_STACK);
            }
            for (index, operand) in operands.into_iter().enumerate() {
                let access = if index == 0 {
                    effects.first
                } else {
                    effects.rest
                };
                if writes(access)
                    && let MsAsmOperand::Register(register) = &operand
                {
                    clobbers.extend(clobbered(register));
                }
                lowered.text(if index == 0 { " " } else { ", " });
                self.ms_asm_operand(operand, sized, access, &mut lowered, &mut labels)?;
            }
        }
        for operand in &mut lowered.operands {
            if let AsmOperandKind::Out { early_clobber, .. } = &mut operand.kind
                && let Some(crate::ir::AsmOperandClass::Explicit(register)) = &operand.selected
            {
                *early_clobber = clobbers.remove(register.spelling.as_str());
            }
        }
        let options = (!self.in_naked_function).then_some(AsmOptions {
            memory: AsmMemory::Any,
            pure: false,
            nostack: false,
            preserves_flags: false,
            may_unwind: false,
        });
        Ok(InlineAsm {
            template: template(asm),
            volatile: true,
            inline: false,
            goto: !labels.is_empty(),
            dialect: Some(AsmDialect::Intel),
            pieces: lowered.pieces,
            operands: lowered.operands,
            clobbers: clobbers
                .into_iter()
                .map(|name| AsmClobber::Register(register(&decode_register(name))))
                .collect(),
            labels,
            alternative: None,
            rejected: Vec::new(),
            options,
        })
    }

    fn ms_asm_operand(
        &mut self,
        operand: MsAsmOperand,
        sized: bool,
        access: AsmAccess,
        lowered: &mut Lowered,
        labels: &mut Vec<BindingId>,
    ) -> Result<(), ResolveError> {
        let reference = match operand {
            MsAsmOperand::Register(register) => {
                lowered.text(register_spelling(&register));
                return Ok(());
            }
            MsAsmOperand::Segment(segment) => {
                lowered.text(segment_spelling(segment));
                return Ok(());
            }
            MsAsmOperand::St(index) => {
                lowered.text(&format!("st({index})"));
                return Ok(());
            }
            MsAsmOperand::Immediate(value) => {
                lowered.text(&value.to_string());
                return Ok(());
            }
            MsAsmOperand::Reference(reference) => reference,
        };
        let registers = reference.base.is_some() || reference.index.is_some();
        let size = match reference.size {
            Some(size) => Some(size_keyword(size)),
            None if sized => reference.ty.and_then(|ty| self.natural_size(ty)),
            None => None,
        };
        if reference.short {
            lowered.text("short ");
        }
        match reference.symbol {
            Some(MsAsmSymbol::Label(name)) => {
                if registers || reference.displacement != 0 {
                    return Err(ResolveError::Unimplemented("`__asm` label with an offset"));
                }
                if reference.offset {
                    lowered.text("offset ");
                }
                lowered
                    .pieces
                    .push(AsmPiece::LocalLabel(name.to_lowercase()));
            }
            Some(MsAsmSymbol::CLabel(binding)) => {
                if registers || reference.displacement != 0 || reference.offset {
                    return Err(ResolveError::Unimplemented(
                        "`__asm` C label with an offset",
                    ));
                }
                let index = match labels.iter().position(|label| *label == binding) {
                    Some(index) => index,
                    None => {
                        labels.push(binding);
                        labels.len() - 1
                    }
                };
                lowered.pieces.push(AsmPiece::Label(index));
            }
            Some(MsAsmSymbol::Function(binding)) => {
                if registers || reference.displacement != 0 {
                    return Err(ResolveError::Unimplemented(
                        "`__asm` function with an offset",
                    ));
                }
                let index = match lowered.functions.get(&binding) {
                    Some(index) => *index,
                    None => {
                        let index = lowered.operand(
                            None,
                            AsmOperandKind::Symbol(AsmSymbol { binding, offset: 0 }),
                        );
                        lowered.functions.insert(binding, index);
                        index
                    }
                };
                lowered.pieces.push(AsmPiece::Operand {
                    index,
                    modifier: None,
                    view: None,
                });
            }
            Some(MsAsmSymbol::Object {
                name,
                lvalue,
                anchor,
            }) if reference.offset => {
                if registers {
                    return Err(ResolveError::Unimplemented(
                        "`__asm` `OFFSET` with a register",
                    ));
                }
                let pointer = self.types.ctypes.pointer(lvalue.c);
                let address = self.operand(&anchor, pointer, ValueKind::AddressOf(lvalue.place));
                let index = lowered.operand(Some(name), AsmOperandKind::In(address.value));
                lowered.pieces.push(AsmPiece::Operand {
                    index,
                    modifier: None,
                    view: None,
                });
                write_displacement(lowered, reference.displacement);
            }
            Some(MsAsmSymbol::Object { name, lvalue, .. }) => {
                let place = lvalue.place;
                let binding = match place.kind {
                    PlaceKind::Binding(binding) => binding,
                    _ => return Err(ResolveError::Unimplemented("`__asm` non-variable object")),
                };
                let operand = match lowered.objects.get(&binding) {
                    Some(index) => {
                        if let AsmOperandKind::Memory {
                            access: previous, ..
                        } = &mut lowered.operands[*index].kind
                        {
                            *previous = union(*previous, access);
                        }
                        *index
                    }
                    None => {
                        let index =
                            lowered.operand(Some(name), AsmOperandKind::Memory { place, access });
                        lowered.objects.insert(binding, index);
                        index
                    }
                };
                if let Some(segment) = reference.segment {
                    lowered.text(segment_spelling(segment));
                    lowered.text(":");
                }
                lowered.pieces.push(AsmPiece::Address {
                    operand,
                    displacement: reference.displacement,
                    base: reference
                        .base
                        .as_ref()
                        .map(|base| register_spelling(base).to_owned()),
                    index: reference
                        .index
                        .as_ref()
                        .map(|(index, scale)| (register_spelling(index).to_owned(), *scale)),
                    size,
                });
            }
            None => {
                let mut text = String::new();
                if let Some(size) = size {
                    let _ = write!(text, "{size} ptr ");
                }
                if let Some(segment) = reference.segment {
                    let _ = write!(text, "{}:", segment_spelling(segment));
                }
                text.push('[');
                let mut terms = Vec::new();
                if let Some(base) = &reference.base {
                    terms.push(register_spelling(base).to_owned());
                }
                if let Some((index, scale)) = &reference.index {
                    terms.push(format!("{}*{scale}", register_spelling(index)));
                }
                text.push_str(&terms.join(" + "));
                match reference.displacement {
                    0 if !terms.is_empty() => {}
                    displacement if terms.is_empty() => {
                        let _ = write!(text, "{displacement}");
                    }
                    displacement if displacement < 0 => {
                        let _ = write!(text, " - {}", displacement.unsigned_abs());
                    }
                    displacement => {
                        let _ = write!(text, " + {displacement}");
                    }
                }
                text.push(']');
                lowered.text(&text);
            }
        }
        Ok(())
    }

    fn natural_size(&self, ty: Type) -> Option<&'static str> {
        let mut ty = self.unaliased(ty);
        while let Type::Array { element, .. } = ty {
            ty = self.unaliased(*element);
        }
        match self.types.storage(ty).ok()?.size_bytes {
            1 => Some("byte"),
            2 => Some("word"),
            4 => Some("dword"),
            6 => Some("fword"),
            8 => Some("qword"),
            10 => Some("tbyte"),
            16 => Some("xmmword"),
            32 => Some("ymmword"),
            64 => Some("zmmword"),
            _ => None,
        }
    }

    fn ms_asm_value(&mut self, expr: &Span<MsAsmExpr>) -> Result<Value, ResolveError> {
        Ok(match &expr.value {
            MsAsmExpr::Register(register) => Value {
                registers: vec![(register.clone(), 1)],
                ..Value::default()
            },
            MsAsmExpr::SegmentRegister(_) | MsAsmExpr::St(_) => {
                return Err(ResolveError::Rejected(
                    "segment or x87 register inside an `__asm` expression",
                ));
            }
            MsAsmExpr::Number(number) => Value::constant(*number as i64),
            MsAsmExpr::Name(name) => self.ms_asm_name(expr, name)?,
            MsAsmExpr::Member { base, field } => {
                let mut value = self.ms_asm_value(base)?;
                let no_such_member =
                    ResolveError::Rejected("no such struct or union member in `__asm`");
                let (ty, offset) = match value.ty.take().map(|ty| self.unaliased(ty)) {
                    Some(ty)
                        if matches!(self.kind(&ty), Some(TypeDefinitionKind::Record { .. })) =>
                    {
                        self.types
                            .offsetof_field(ty, &field.value)
                            .map_err(|_| no_such_member)?
                    }
                    _ if self.types.flavor().is_msvc() => {
                        let declared_before = *self
                            .names
                            .ms_asm_members
                            .get(&field.id)
                            .ok_or(ResolveError::Internal("unresolved `__asm` member"))?;
                        self.types.ms_asm_field(&field.value, declared_before)?
                    }
                    Some(_) => return Err(no_such_member),
                    None => {
                        return Err(ResolveError::Rejected(
                            "`__asm` member of an untyped operand",
                        ));
                    }
                };
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
                    .ok_or(ResolveError::Rejected("`__asm` negation of a non-constant"))?;
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
            MsAsmExpr::Operator {
                operator: MsAsmOperator::Type,
                operand,
            } if let MsAsmExpr::TypeKeyword(keyword) = operand.value => {
                Value::constant(self.ms_asm_keyword_size(keyword)?)
            }
            MsAsmExpr::TypeKeyword(_) => {
                return Err(ResolveError::Rejected(
                    "C type keyword outside `TYPE` in `__asm`",
                ));
            }
            MsAsmExpr::Operator { operator, operand } => {
                let mut value = self.ms_asm_value(operand)?;
                match operator {
                    MsAsmOperator::Offset => {
                        if value.symbol.is_none() {
                            return Err(ResolveError::Rejected(
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
                        let ty = value.ty.ok_or(ResolveError::Rejected(
                            "`TYPE`, `LENGTH` and `SIZE` need a C object or type in `__asm`",
                        ))?;
                        Value::constant(self.ms_asm_type_operator(*operator, ty)?)
                    }
                }
            }
        })
    }

    fn ms_asm_name(&mut self, expr: &Span<MsAsmExpr>, name: &str) -> Result<Value, ResolveError> {
        let Some(reference) = self
            .names
            .references
            .iter()
            .find(|reference| reference.id == expr.id)
        else {
            return Ok(Value {
                symbol: Some(MsAsmSymbol::Label(name.to_owned())),
                ..Value::default()
            });
        };
        if reference.kind == BindingKind::Label {
            return Ok(Value {
                symbol: Some(MsAsmSymbol::CLabel(reference.binding)),
                ..Value::default()
            });
        }
        let (kind, binding) = (reference.kind, reference.binding);
        let identifier = Box::new(
            expr.clone()
                .with_value(ast::ExprKind::Identifier(name.to_owned())),
        );
        Ok(match kind {
            BindingKind::Object | BindingKind::Parameter => {
                let lvalue = self.place(&identifier)?;
                Value {
                    ty: Some(lvalue.place.ty.clone()),
                    symbol: Some(MsAsmSymbol::Object {
                        name: name.to_owned(),
                        lvalue,
                        anchor: expr.derive(()),
                    }),
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
                        .map_err(|_| ResolveError::Rejected("`__asm` enumerator out of range"))?,
                )
            }
            BindingKind::Typedef => {
                let alias = self
                    .types
                    .alias(binding)
                    .ok_or(ResolveError::Internal("unknown typedef"))?;
                Value {
                    ty: Some(self.types.ir_type(alias)),
                    ..Value::default()
                }
            }
            BindingKind::Tag | BindingKind::Label => {
                return Err(ResolveError::Rejected("name cannot be used in `__asm`"));
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

    fn ms_asm_keyword_size(&self, keyword: Keyword) -> Result<i64, ResolveError> {
        let target = self.types.target_info();
        let bits = match keyword {
            Keyword::Char | Keyword::Signed | Keyword::Unsigned => 8,
            Keyword::Short => target.short_width,
            Keyword::Int => target.int_width,
            Keyword::Long => target.long_width,
            Keyword::Float => 32,
            Keyword::Double | Keyword::Int64 => 64,
            _ => {
                return Err(ResolveError::Rejected(
                    "`TYPE` of an unsupported C type keyword in `__asm`",
                ));
            }
        };
        Ok(i64::from(bits / 8))
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
            let constant = rhs.as_constant().ok_or(ResolveError::Rejected(
                "`__asm` subtraction needs a constant right operand",
            ))?;
            lhs.add(Value::constant(constant.wrapping_neg()))
        }
        MsAsmBinaryOp::Mul => match (lhs.as_constant(), rhs.as_constant()) {
            (Some(lhs), Some(rhs)) => Ok(Value::constant(lhs.wrapping_mul(rhs))),
            (Some(scale), None) => scaled(rhs, scale),
            (None, Some(scale)) => scaled(lhs, scale),
            (None, None) => Err(ResolveError::Rejected(
                "`__asm` multiplication needs a constant operand",
            )),
        },
        MsAsmBinaryOp::Div => match (lhs.as_constant(), rhs.as_constant()) {
            (Some(lhs), Some(rhs)) => lhs
                .checked_div(rhs)
                .map(Value::constant)
                .ok_or(ResolveError::Rejected("`__asm` division by zero")),
            _ => Err(ResolveError::Rejected(
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
        _ => Err(ResolveError::Rejected(
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
            return Err(ResolveError::Rejected(
                "`__asm` operand scales two registers",
            ));
        }
    };
    Ok(MsAsmOperand::Reference(MsAsmReference {
        symbol: value.symbol,
        ty: value.ty,
        displacement: value.constant,
        base,
        index,
        size: value.size,
        segment: value.segment,
        offset: value.offset,
        short: value.short,
    }))
}

// masm leaves a memory operand's size to the register beside it, except where that register does not fix it.
fn needs_size(mnemonic: &str, operands: &[MsAsmOperand]) -> bool {
    let register = operands.iter().any(|operand| {
        matches!(
            operand,
            MsAsmOperand::Register(_) | MsAsmOperand::Segment(_)
        )
    });
    !register
        || matches!(
            mnemonic,
            "movzx"
                | "movsx"
                | "movsxd"
                | "shl"
                | "shr"
                | "sal"
                | "sar"
                | "rol"
                | "ror"
                | "rcl"
                | "rcr"
                | "shld"
                | "shrd"
        )
}

fn write_displacement(lowered: &mut Lowered, displacement: i64) {
    match displacement {
        0 => {}
        displacement if displacement < 0 => {
            lowered.text(&format!(" - {}", displacement.unsigned_abs()));
        }
        displacement => lowered.text(&format!(" + {displacement}")),
    }
}

fn register_spelling(register: &Register) -> &str {
    match register {
        Register::X86(info) => &info.spelling,
        Register::Aarch64(info) => &info.spelling,
        Register::Other(spelling) => spelling,
    }
}

fn segment_spelling(segment: MsAsmSegment) -> &'static str {
    match segment {
        MsAsmSegment::Es => "es",
        MsAsmSegment::Cs => "cs",
        MsAsmSegment::Ss => "ss",
        MsAsmSegment::Ds => "ds",
        MsAsmSegment::Fs => "fs",
        MsAsmSegment::Gs => "gs",
    }
}

// llvm's intel parser knows no `realN` or `oword`, so they print as their plain sizes.
fn size_keyword(size: MsAsmSize) -> &'static str {
    match size {
        MsAsmSize::Byte => "byte",
        MsAsmSize::Word => "word",
        MsAsmSize::Dword | MsAsmSize::Real4 => "dword",
        MsAsmSize::Fword => "fword",
        MsAsmSize::Qword | MsAsmSize::Real8 => "qword",
        MsAsmSize::Tbyte | MsAsmSize::Real10 => "tbyte",
        MsAsmSize::Mmword => "mmword",
        MsAsmSize::Xmmword | MsAsmSize::Oword => "xmmword",
        MsAsmSize::Ymmword => "ymmword",
        MsAsmSize::Zmmword => "zmmword",
    }
}

fn template(asm: &ast::MsAsm) -> String {
    let mut text = String::new();
    for (line, instruction) in asm.instructions.iter().enumerate() {
        let instruction = &instruction.value;
        if line > 0 {
            text.push('\n');
        }
        let mut words = Vec::new();
        if let Some(label) = &instruction.label {
            words.push(format!("{}:", label.value));
        }
        words.extend(
            instruction
                .prefixes
                .iter()
                .map(|prefix| prefix.value.clone()),
        );
        if let Some(mnemonic) = &instruction.mnemonic {
            let operands = instruction
                .operands
                .iter()
                .map(|operand| render(&operand.value))
                .collect::<Vec<_>>()
                .join(", ");
            words.push(if operands.is_empty() {
                mnemonic.value.clone()
            } else {
                format!("{} {operands}", mnemonic.value)
            });
        }
        text.push_str(&words.join(" "));
    }
    text
}

fn render(expr: &MsAsmExpr) -> String {
    match expr {
        MsAsmExpr::Register(register) => register_spelling(register).to_owned(),
        MsAsmExpr::SegmentRegister(segment) => segment_spelling(*segment).to_owned(),
        MsAsmExpr::St(index) => format!("st({index})"),
        MsAsmExpr::Number(number) => number.to_string(),
        MsAsmExpr::TypeKeyword(keyword) => <&str>::from(*keyword).to_owned(),
        MsAsmExpr::Name(name) => name.clone(),
        MsAsmExpr::Member { base, field } => format!("{}.{}", render(&base.value), field.value),
        MsAsmExpr::Index { base, index } => {
            format!("{}[{}]", render(&base.value), render(&index.value))
        }
        MsAsmExpr::Bracket(operand) => format!("[{}]", render(&operand.value)),
        MsAsmExpr::Binary { op, lhs, rhs } => {
            let (symbol, tight) = match op {
                MsAsmBinaryOp::Add => ("+", false),
                MsAsmBinaryOp::Sub => ("-", false),
                MsAsmBinaryOp::Mul => ("*", true),
                MsAsmBinaryOp::Div => ("/", true),
            };
            let side = |operand: &MsAsmExpr| match operand {
                MsAsmExpr::Binary {
                    op: MsAsmBinaryOp::Add | MsAsmBinaryOp::Sub,
                    ..
                } if tight => format!("({})", render(operand)),
                _ => render(operand),
            };
            let right = match &rhs.value {
                MsAsmExpr::Binary { .. } if !tight && matches!(op, MsAsmBinaryOp::Sub) => {
                    format!("({})", render(&rhs.value))
                }
                operand => side(operand),
            };
            format!("{} {symbol} {right}", side(&lhs.value))
        }
        MsAsmExpr::Negate(operand) => format!("-{}", render(&operand.value)),
        MsAsmExpr::Ptr { size, operand } => {
            format!("{} ptr {}", size_keyword(*size), render(&operand.value))
        }
        MsAsmExpr::Segment { segment, operand } => {
            format!("{}:{}", segment_spelling(*segment), render(&operand.value))
        }
        MsAsmExpr::Operator { operator, operand } => {
            let keyword = match operator {
                MsAsmOperator::Offset => "offset",
                MsAsmOperator::Type => "type",
                MsAsmOperator::Length => "length",
                MsAsmOperator::Size => "size",
                MsAsmOperator::Short => "short",
            };
            format!("{keyword} {}", render(&operand.value))
        }
    }
}
