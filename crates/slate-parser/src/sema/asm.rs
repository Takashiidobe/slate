use super::expression::Lowerer;
use super::numeric::ResolveError;
use crate::ast;
use crate::compiler_args::CompilerFlavor;
use crate::ir::*;
use crate::target_info::TargetFamily;

impl Lowerer {
    pub(super) fn asm(&mut self, asm: &ast::GnuAsm) -> Result<InlineAsm, ResolveError> {
        let qualifier = |wanted: ast::AsmQualifier| {
            asm.qualifiers
                .iter()
                .any(|qualifier| qualifier.value == wanted)
        };
        let family = self.context.target.family;
        let dialect = match family {
            TargetFamily::X86 | TargetFamily::X86_64 => Some(AsmDialect::Att),
            TargetFamily::AArch64 | TargetFamily::Arm32 => None,
        };
        let mut lowered = InlineAsm {
            template: asm.template.value.clone(),
            volatile: qualifier(ast::AsmQualifier::Volatile),
            inline: qualifier(ast::AsmQualifier::Inline),
            goto: qualifier(ast::AsmQualifier::Goto),
            dialect,
            pieces: Vec::new(),
            operands: Vec::new(),
            clobbers: Vec::new(),
            labels: Vec::new(),
        };
        let Some(operands) = &asm.operands else {
            return Ok(lowered);
        };
        lower_pieces(&operands.pieces, dialect, family, &mut lowered.pieces);
        for output in &operands.outputs {
            let constraint = constraint(&output.constraint.value, family);
            let place = self.place(&output.expr)?.place;
            self.memory_place(&place, constraint.memory_only())?;
            let early_clobber = output.constraint.value.early_clobber();
            let kind = match output.constraint.value.write_modifier() {
                Some(ast::AsmConstraintModifier::ReadWrite) => AsmOperandKind::InOut {
                    place,
                    input: None,
                    early_clobber,
                },
                _ => AsmOperandKind::Out {
                    place,
                    early_clobber,
                },
            };
            let name = output.name.as_ref().map(|name| name.value.clone());
            lowered
                .operands
                .push(self.asm_operand(name, constraint, kind));
        }
        let mut renumbered: Vec<(usize, Option<AsmRegisterView>)> = (0..operands.outputs.len())
            .map(|index| (index, None))
            .collect();
        for input in &operands.inputs {
            if let Some(output) = input.constraint.value.tied_output() {
                let value = self.expr(&input.expr)?.value;
                let input_width = self.width(&value.ty);
                let Some(AsmOperand {
                    kind:
                        AsmOperandKind::Out {
                            place,
                            early_clobber,
                        },
                    width,
                    ..
                }) = lowered.operands.get(output)
                else {
                    return Err(ResolveError::Unsupported("asm input tied to a non-output"));
                };
                // x86 prints a tied input at its own width, not its output's.
                let view = match input_width {
                    Some(bits)
                        if matches!(family, TargetFamily::X86 | TargetFamily::X86_64)
                            && input_width != *width =>
                    {
                        Some(AsmRegisterView::Bits(bits))
                    }
                    _ => None,
                };
                lowered.operands[output].kind = AsmOperandKind::InOut {
                    place: place.clone(),
                    input: Some(value),
                    early_clobber: *early_clobber,
                };
                renumbered.push((output, view));
                continue;
            }
            let constraint = constraint(&input.constraint.value, family);
            let kind = if constraint.allows_memory() {
                self.memory_input(&input.expr, constraint.memory_only())?
            } else {
                AsmOperandKind::In(self.expr(&input.expr)?.value)
            };
            renumbered.push((lowered.operands.len(), None));
            let name = input.name.as_ref().map(|name| name.value.clone());
            lowered
                .operands
                .push(self.asm_operand(name, constraint, kind));
        }
        for piece in &mut lowered.pieces {
            if let AsmPiece::Operand { index, view, .. } = piece {
                let (target, tied) = renumbered[*index];
                *index = target;
                *view = view.or(tied);
            }
        }
        lowered.clobbers = operands
            .clobbers
            .iter()
            .map(|clobber| match &clobber.value {
                ast::AsmClobber::Memory => AsmClobber::Memory,
                ast::AsmClobber::Cc => AsmClobber::Cc,
                ast::AsmClobber::Unwind => AsmClobber::Unwind,
                ast::AsmClobber::Register(reg) => AsmClobber::Register(register(reg)),
            })
            .collect();
        for label in &operands.labels {
            lowered.labels.push(
                self.names
                    .references
                    .iter()
                    .find(|reference| reference.id == label.id)
                    .map(|reference| reference.binding)
                    .ok_or(ResolveError::Unsupported("missing asm label binding"))?,
            );
        }
        Ok(lowered)
    }
}

impl Lowerer {
    fn asm_operand(
        &self,
        name: Option<String>,
        mut constraint: AsmConstraint,
        kind: AsmOperandKind,
    ) -> AsmOperand {
        let ty = match &kind {
            AsmOperandKind::In(value) => &value.ty,
            AsmOperandKind::InPlace(place)
            | AsmOperandKind::Out { place, .. }
            | AsmOperandKind::InOut { place, .. } => &place.ty,
        };
        let width = self.width(ty);
        let wide = match width {
            Some(256) => Some(AsmRegisterClass::YmmReg),
            Some(512) => Some(AsmRegisterClass::ZmmReg),
            _ => None,
        };
        for alternative in &mut constraint.alternatives {
            if let (Some(wide), AsmConstraintLocation::Letters { classes, .. }) =
                (wide, &mut alternative.location)
            {
                for class in classes {
                    if let AsmOperandClass::Register(register @ AsmRegisterClass::XmmReg) = class {
                        *register = wide;
                    }
                }
            }
        }
        AsmOperand {
            name,
            constraint,
            kind,
            width,
        }
    }

    fn width(&self, ty: &Type) -> Option<u64> {
        self.types
            .storage(ty.clone())
            .ok()
            .map(|storage| storage.size_bytes * 8)
    }

    fn memory_input(
        &mut self,
        expr: &ast::Expr,
        memory_only: bool,
    ) -> Result<AsmOperandKind, ResolveError> {
        let lvalue = match self.place(expr) {
            Ok(lvalue) => lvalue,
            Err(_) if memory_only => {
                return Err(ResolveError::Invalid(
                    "asm input with a memory-only constraint is not an lvalue",
                ));
            }
            Err(_) => return Ok(AsmOperandKind::In(self.expr(expr)?.value)),
        };
        if self.memory_place(&lvalue.place, memory_only)? {
            Ok(AsmOperandKind::InPlace(lvalue.place))
        } else {
            Ok(AsmOperandKind::In(self.read(expr, lvalue)?.value))
        }
    }

    // compilers spill an unaddressable object to a temporary, which an input cannot tell from a copy.
    fn memory_place(&self, place: &Place, memory_only: bool) -> Result<bool, ResolveError> {
        match &place.kind {
            PlaceKind::Field { bits: Some(_), .. } if memory_only => {
                Err(ResolveError::Invalid("address of a bit-field"))
            }
            PlaceKind::Binding(id)
                if memory_only
                    && self.types.compiler_flavor() == CompilerFlavor::Gcc
                    && self.types.entities.is_register(id) =>
            {
                Err(ResolveError::Invalid(
                    "address of register variable requested",
                ))
            }
            _ => Ok(self.addressable(place).is_ok()),
        }
    }
}

fn lower_pieces(
    pieces: &[ast::AsmTemplatePiece],
    dialect: Option<AsmDialect>,
    family: TargetFamily,
    lowered: &mut Vec<AsmPiece>,
) {
    for piece in pieces {
        let piece = match piece {
            ast::AsmTemplatePiece::Text(text) => {
                if let Some(AsmPiece::Text(previous)) = lowered.last_mut() {
                    previous.push_str(text);
                    continue;
                }
                AsmPiece::Text(text.clone())
            }
            ast::AsmTemplatePiece::Operand { index, modifier } => AsmPiece::Operand {
                index: *index,
                modifier: *modifier,
                view: modifier.and_then(|modifier| view(modifier, family)),
            },
            ast::AsmTemplatePiece::Label(index) => AsmPiece::Label(*index),
            ast::AsmTemplatePiece::Percent => AsmPiece::Percent,
            ast::AsmTemplatePiece::UniqueId => AsmPiece::UniqueId,
            ast::AsmTemplatePiece::DialectAlternatives(alternatives) => {
                let selected = match dialect {
                    Some(AsmDialect::Intel) => 1,
                    Some(AsmDialect::Att) | None => 0,
                };
                if let Some(alternative) = alternatives.get(selected) {
                    lower_pieces(alternative, dialect, family, lowered);
                }
                continue;
            }
        };
        lowered.push(piece);
    }
}

fn view(modifier: char, family: TargetFamily) -> Option<AsmRegisterView> {
    let bits = match (family, modifier) {
        (TargetFamily::X86 | TargetFamily::X86_64, 'h') => {
            return Some(AsmRegisterView::HighByte);
        }
        (TargetFamily::X86 | TargetFamily::X86_64, 'b') => 8,
        (TargetFamily::X86 | TargetFamily::X86_64, 'w') => 16,
        (TargetFamily::X86 | TargetFamily::X86_64, 'k') => 32,
        (TargetFamily::X86 | TargetFamily::X86_64, 'q') => 64,
        (TargetFamily::X86 | TargetFamily::X86_64, 'x') => 128,
        (TargetFamily::X86 | TargetFamily::X86_64, 't') => 256,
        (TargetFamily::X86 | TargetFamily::X86_64, 'g') => 512,
        (TargetFamily::AArch64, 'b') => 8,
        (TargetFamily::AArch64, 'h') => 16,
        (TargetFamily::AArch64, 'w' | 's') => 32,
        (TargetFamily::AArch64, 'x' | 'd') => 64,
        (TargetFamily::AArch64, 'q') => 128,
        _ => return None,
    };
    Some(AsmRegisterView::Bits(bits))
}

fn constraint(constraint: &ast::AsmConstraint, family: TargetFamily) -> AsmConstraint {
    AsmConstraint {
        alternatives: constraint
            .alternatives
            .iter()
            .map(|alternative| AsmConstraintAlternative {
                modifiers: alternative.modifiers.iter().filter_map(modifier).collect(),
                location: match &alternative.location {
                    ast::AsmConstraintLocation::HardRegister(reg) => {
                        AsmConstraintLocation::HardRegister(register(reg))
                    }
                    ast::AsmConstraintLocation::Matching(index) => {
                        AsmConstraintLocation::Matching(*index)
                    }
                    ast::AsmConstraintLocation::Letters(letters) => {
                        AsmConstraintLocation::Letters {
                            letters: letters.clone(),
                            classes: classes(letters, family),
                        }
                    }
                },
            })
            .collect(),
    }
}

fn classes(letters: &str, family: TargetFamily) -> Vec<AsmOperandClass> {
    let x86 = matches!(family, TargetFamily::X86 | TargetFamily::X86_64);
    let mut classes = Vec::new();
    let mut rest = letters;
    while let Some(first) = rest.chars().next() {
        let length = match first {
            'Y' | 'W' | 'j' | 'B' if x86 => 2,
            'U' if family == TargetFamily::AArch64 => 3,
            'U' if family == TargetFamily::Arm32 => 2,
            _ => 1,
        };
        let end = rest
            .char_indices()
            .nth(length)
            .map_or(rest.len(), |(index, _)| index);
        let (letter, tail) = rest.split_at(end);
        rest = tail;
        let class = match (family, letter) {
            (_, "#") => break,
            (_, "?" | "!" | "*" | "^" | "$") => continue,
            (_, "r") => AsmOperandClass::Register(AsmRegisterClass::Reg),
            (_, "m" | "o") => AsmOperandClass::Memory,
            (_, "i" | "n") => AsmOperandClass::Immediate,
            (_, "g") => {
                classes.extend([
                    AsmOperandClass::Register(AsmRegisterClass::Reg),
                    AsmOperandClass::Memory,
                    AsmOperandClass::Immediate,
                ]);
                continue;
            }
            (TargetFamily::X86, "q") => AsmOperandClass::Register(AsmRegisterClass::RegAbcd),
            (TargetFamily::X86, "R") => AsmOperandClass::Register(AsmRegisterClass::Reg),
            (TargetFamily::X86_64, "q") => AsmOperandClass::Register(AsmRegisterClass::Reg),
            (TargetFamily::X86_64, "R") => AsmOperandClass::Register(AsmRegisterClass::RegLegacy),
            (_, "Q") if x86 => AsmOperandClass::Register(AsmRegisterClass::RegAbcd),
            (_, "a") if x86 => explicit("ax"),
            (_, "b") if x86 => explicit("bx"),
            (_, "c") if x86 => explicit("cx"),
            (_, "d") if x86 => explicit("dx"),
            (_, "S") if x86 => explicit("si"),
            (_, "D") if x86 => explicit("di"),
            (_, "t") if x86 => explicit("st"),
            (_, "u") if x86 => explicit("st(1)"),
            (_, "Yz") if x86 => explicit("xmm0"),
            (_, "x") if x86 => AsmOperandClass::Register(AsmRegisterClass::XmmReg),
            (_, "v") if x86 => AsmOperandClass::Register(AsmRegisterClass::ZmmReg),
            (_, "k") if x86 => AsmOperandClass::Register(AsmRegisterClass::KReg),
            (_, "f") if x86 => AsmOperandClass::Register(AsmRegisterClass::X87Reg),
            (_, "y") if x86 => AsmOperandClass::Register(AsmRegisterClass::MmxReg),
            (_, "I" | "J" | "K" | "L" | "M" | "N" | "O" | "e" | "Z") if x86 => {
                AsmOperandClass::Immediate
            }
            (TargetFamily::AArch64, "w") => AsmOperandClass::Register(AsmRegisterClass::VReg),
            (TargetFamily::AArch64, "y") => AsmOperandClass::Register(AsmRegisterClass::VRegLow8),
            (TargetFamily::AArch64, "x") => AsmOperandClass::Register(AsmRegisterClass::VRegLow16),
            (TargetFamily::AArch64, "I" | "J" | "K" | "L" | "M" | "N" | "Z") => {
                AsmOperandClass::Immediate
            }
            (TargetFamily::Arm32, "t") => AsmOperandClass::Register(AsmRegisterClass::SReg),
            (TargetFamily::Arm32, "w") => AsmOperandClass::Register(AsmRegisterClass::DReg),
            (TargetFamily::Arm32, "I" | "J" | "K" | "L" | "M") => AsmOperandClass::Immediate,
            (TargetFamily::AArch64 | TargetFamily::Arm32, "Q") => AsmOperandClass::Memory,
            _ => AsmOperandClass::Unresolved(letter.to_string()),
        };
        classes.push(class);
    }
    classes
}

fn explicit(name: &'static str) -> AsmOperandClass {
    AsmOperandClass::Explicit(AsmRegister {
        spelling: name.to_string(),
        canonical: Some(name),
    })
}

fn modifier(modifier: &ast::AsmConstraintModifier) -> Option<AsmConstraintModifier> {
    match modifier {
        ast::AsmConstraintModifier::Overwrite
        | ast::AsmConstraintModifier::ReadWrite
        | ast::AsmConstraintModifier::EarlyClobber => None,
        ast::AsmConstraintModifier::Commutative => Some(AsmConstraintModifier::Commutative),
        ast::AsmConstraintModifier::Pic => Some(AsmConstraintModifier::Pic),
    }
}

pub(super) fn register(register: &ast::Register) -> AsmRegister {
    match register {
        ast::Register::X86(info) => AsmRegister {
            spelling: info.spelling.clone(),
            canonical: Some(info.canonical),
        },
        ast::Register::Aarch64(info) => AsmRegister {
            spelling: info.spelling.clone(),
            canonical: Some(info.canonical),
        },
        ast::Register::Other(spelling) => AsmRegister {
            spelling: spelling.clone(),
            canonical: None,
        },
    }
}
