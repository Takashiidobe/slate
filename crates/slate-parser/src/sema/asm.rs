use super::expression::Lowerer;
use super::numeric::ResolveError;
use crate::ast;
use crate::ir::*;
use crate::target_info::TargetFamily;

impl Lowerer {
    pub(super) fn asm(&mut self, asm: &ast::GnuAsm) -> Result<InlineAsm, ResolveError> {
        let qualifier = |wanted: ast::AsmQualifier| {
            asm.qualifiers
                .iter()
                .any(|qualifier| qualifier.value == wanted)
        };
        let dialect = match self.context.target.family {
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
            outputs: Vec::new(),
            inputs: Vec::new(),
            clobbers: Vec::new(),
            labels: Vec::new(),
        };
        let Some(operands) = &asm.operands else {
            return Ok(lowered);
        };
        lower_pieces(&operands.pieces, dialect, &mut lowered.pieces);
        for output in &operands.outputs {
            let place = self.place(&output.expr)?;
            lowered.outputs.push(AsmOutput {
                name: output.name.as_ref().map(|name| name.value.clone()),
                constraint: constraint(&output.constraint.value),
                place: place.place,
            });
        }
        for input in &operands.inputs {
            let value = self.expr(&input.expr)?;
            lowered.inputs.push(AsmInput {
                name: input.name.as_ref().map(|name| name.value.clone()),
                constraint: constraint(&input.constraint.value),
                value: value.value,
            });
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

fn lower_pieces(
    pieces: &[ast::AsmTemplatePiece],
    dialect: Option<AsmDialect>,
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
                    lower_pieces(alternative, dialect, lowered);
                }
                continue;
            }
        };
        lowered.push(piece);
    }
}

fn constraint(constraint: &ast::AsmConstraint) -> AsmConstraint {
    AsmConstraint {
        alternatives: constraint
            .alternatives
            .iter()
            .map(|alternative| AsmConstraintAlternative {
                modifiers: alternative.modifiers.iter().map(modifier).collect(),
                location: match &alternative.location {
                    ast::AsmConstraintLocation::HardRegister(reg) => {
                        AsmConstraintLocation::HardRegister(register(reg))
                    }
                    ast::AsmConstraintLocation::Matching(index) => {
                        AsmConstraintLocation::Matching(*index)
                    }
                    ast::AsmConstraintLocation::Letters(letters) => {
                        AsmConstraintLocation::Letters(letters.clone())
                    }
                },
            })
            .collect(),
    }
}

fn modifier(modifier: &ast::AsmConstraintModifier) -> AsmConstraintModifier {
    match modifier {
        ast::AsmConstraintModifier::Overwrite => AsmConstraintModifier::Overwrite,
        ast::AsmConstraintModifier::ReadWrite => AsmConstraintModifier::ReadWrite,
        ast::AsmConstraintModifier::EarlyClobber => AsmConstraintModifier::EarlyClobber,
        ast::AsmConstraintModifier::Commutative => AsmConstraintModifier::Commutative,
        ast::AsmConstraintModifier::Pic => AsmConstraintModifier::Pic,
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
