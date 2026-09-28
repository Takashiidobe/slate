use super::asm::register;
use super::expression::Lowerer;
use super::numeric::ResolveError;
use crate::ast::Span;
use crate::const_expr::BinaryOp;
use crate::ir::*;
use crate::target::x86::decode_register;
use crate::target_info::TargetFamily;

impl Lowerer {
    pub(super) fn ms_asm_return_operands(&mut self) -> Result<Vec<AsmOperand>, ResolveError> {
        if self.in_naked_function || self.context.target.family != TargetFamily::X86 {
            return Ok(Vec::new());
        }
        let Some(returned) = self.return_type else {
            return Ok(Vec::new());
        };
        let ty = self.types.ir_type(returned);
        let scalar = self.ms_asm_return_scalar(&ty);
        if !matches!(
            scalar,
            Type::Bool | Type::Numeric(NumericType::Integer { .. }) | Type::Pointer { .. }
        ) {
            return Ok(Vec::new());
        }
        let bytes = self.types.storage(ty)?.size_bytes;
        if bytes == 0 || bytes > 8 {
            return Ok(Vec::new());
        }
        let count = if bytes > 4 { 2 } else { 1 };
        if self.ms_asm_return.is_empty() {
            for _ in 0..count {
                let id = self.fresh();
                self.ms_asm_return.push(id);
            }
        }
        Ok(self
            .ms_asm_return
            .iter()
            .zip(["eax", "edx"])
            .map(|(id, name)| {
                let register = register(&decode_register(name));
                AsmOperand {
                    name: None,
                    constraint: AsmConstraint {
                        alternatives: vec![AsmConstraintAlternative {
                            modifiers: Vec::new(),
                            location: AsmConstraintLocation::HardRegister(register.clone()),
                        }],
                    },
                    kind: AsmOperandKind::Out {
                        place: return_place(*id),
                        early_clobber: false,
                    },
                    width: Some(32),
                    selected: Some(AsmOperandClass::Explicit(register)),
                }
            })
            .collect())
    }

    fn ms_asm_return_scalar(&self, ty: &Type) -> Type {
        match self.kind(ty) {
            Some(TypeDefinitionKind::Alias(inner))
            | Some(TypeDefinitionKind::Enum {
                underlying: Some(inner),
                ..
            }) => self.ms_asm_return_scalar(inner),
            _ => ty.clone(),
        }
    }

    pub(super) fn finish_ms_asm_return(
        &mut self,
        anchor: &Span<()>,
        returned: Option<&Type>,
        body: &mut Vec<Span<Statement>>,
        main_returns_zero: bool,
    ) -> Result<Option<Value>, ResolveError> {
        let registers = std::mem::take(&mut self.ms_asm_return);
        let Some(low) = registers.first() else {
            return Ok(None);
        };
        let returned = returned.ok_or(ResolveError::Rejected(
            "MS asm return without a return type",
        ))?;
        let declarations = registers.iter().map(|id| {
            anchor.derive(Statement::Temporary {
                id: *id,
                ty: Type::integer(32, false),
                initializer: main_returns_zero.then(|| {
                    self.value(
                        anchor,
                        Type::integer(32, false),
                        ValueKind::Constant(Number::Integer(0u32.into())),
                    )
                }),
                unsequenced: false,
            })
        });
        body.splice(0..0, declarations);
        let read = |id| {
            self.value(
                anchor,
                Type::integer(32, false),
                ValueKind::Read {
                    place: return_place(id),
                    ordering: None,
                },
            )
        };
        let mut value = read(*low);
        if let Some(high) = registers.get(1) {
            let wide = |value| {
                self.context.emit_arithmetic_conversion(
                    value,
                    Type::integer(64, false),
                    ConversionReason::Return,
                )
            };
            let shift = self.value(
                anchor,
                Type::integer(32, false),
                ValueKind::Constant(Number::Integer(32u32.into())),
            );
            let (ty, kind) =
                self.context
                    .emit_binary(BinaryOp::ShiftLeft, wide(read(*high))?, shift)?;
            let high = self.value(anchor, ty, kind);
            let (ty, kind) = self
                .context
                .emit_binary(BinaryOp::BitOr, wide(value)?, high)?;
            value = self.value(anchor, ty, kind);
        }
        let scalar = self.ms_asm_return_scalar(returned);
        value = match &scalar {
            Type::Pointer { .. } => self.value(
                anchor,
                scalar.clone(),
                ValueKind::Convert {
                    kind: ConversionKind::IntToPtr,
                    operand: Box::new(value),
                    reason: ConversionReason::Return,
                    semantics: ConversionSema::Exact,
                },
            ),
            Type::Bool => {
                let bit = self.context.emit_arithmetic_conversion(
                    value,
                    Type::integer(1, false),
                    ConversionReason::Return,
                )?;
                self.context.emit_arithmetic_conversion(
                    bit,
                    Type::Bool,
                    ConversionReason::Return,
                )?
            }
            _ => {
                self.context
                    .emit_arithmetic_conversion(value, scalar, ConversionReason::Return)?
            }
        };
        if value.ty != *returned {
            value = self.value(
                anchor,
                returned.clone(),
                ValueKind::Convert {
                    kind: ConversionKind::IntToEnum,
                    operand: Box::new(value),
                    reason: ConversionReason::Return,
                    semantics: ConversionSema::Exact,
                },
            );
        }
        Ok(Some(value))
    }
}

fn return_place(id: BindingId) -> Place {
    Place {
        ty: Type::integer(32, false),
        kind: PlaceKind::Binding(id),
        access: Access::default(),
    }
}
