use super::expression::Lowerer;
use super::fold::integer_constant;
use super::numeric::ResolveError;
use super::operand::Lvalue;
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
            alternative: None,
            rejected: Vec::new(),
        };
        let Some(operands) = &asm.operands else {
            return Ok(lowered);
        };
        lower_pieces(&operands.pieces, dialect, family, &mut lowered.pieces);
        let mut sources = Vec::new();
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
            sources.push(self.source(output, constraint, Candidate::Output(kind)));
        }
        for input in &operands.inputs {
            let constraint = constraint(&input.constraint.value, family);
            let candidate = self.input_candidate(&input.expr, &constraint)?;
            sources.push(self.source(input, constraint, candidate));
        }
        let (alternative, rejected) = select(&sources, family);
        lowered.alternative = alternative;
        lowered.rejected = rejected;
        let mut renumbered: Vec<(usize, Option<AsmRegisterView>)> = Vec::new();
        for (index, source) in sources.into_iter().enumerate() {
            let choice =
                alternative.and_then(|alternative| choose(&source, alternative, family).ok());
            let tie = match choice {
                Some(Choice::Tie(output)) => Some(output),
                Some(Choice::Class(_)) => None,
                None => index
                    .checked_sub(operands.outputs.len())
                    .and_then(|input| operands.inputs.get(input))
                    .and_then(|input| input.constraint.value.tied_output()),
            };
            let selected = match choice {
                Some(Choice::Class(class)) => Some(class),
                Some(Choice::Tie(_)) | None => None,
            };
            let memory = match &selected {
                Some(class) => matches!(class, AsmOperandClass::Memory),
                None => source.constraint.allows_memory(),
            };
            let kind = match source.candidate {
                Candidate::Output(kind) => kind,
                Candidate::Lvalue {
                    lvalue,
                    addressable: true,
                    ..
                } if memory && tie.is_none() => AsmOperandKind::InPlace(lvalue.place),
                Candidate::Lvalue { expr, lvalue, .. } => {
                    AsmOperandKind::In(self.read(expr, lvalue)?.value)
                }
                Candidate::Value(value) => AsmOperandKind::In(value),
            };
            if let (Some(output), AsmOperandKind::In(value)) = (tie, &kind) {
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
                    input: Some(value.clone()),
                    early_clobber: *early_clobber,
                };
                renumbered.push((output, view));
                continue;
            }
            renumbered.push((lowered.operands.len(), None));
            let width = match &kind {
                AsmOperandKind::In(value) => self.width(&value.ty),
                AsmOperandKind::InPlace(place)
                | AsmOperandKind::Out { place, .. }
                | AsmOperandKind::InOut { place, .. } => self.width(&place.ty),
            };
            lowered.operands.push(AsmOperand {
                name: source.name,
                constraint: source.constraint,
                kind,
                width,
                selected,
            });
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
    fn source<'a>(
        &self,
        operand: &ast::AsmOperand,
        mut constraint: AsmConstraint,
        candidate: Candidate<'a>,
    ) -> Source<'a> {
        let ty = match &candidate {
            Candidate::Output(
                AsmOperandKind::InPlace(place)
                | AsmOperandKind::Out { place, .. }
                | AsmOperandKind::InOut { place, .. },
            ) => Some(&place.ty),
            Candidate::Output(AsmOperandKind::In(value)) | Candidate::Value(value) => {
                Some(&value.ty)
            }
            Candidate::Lvalue { lvalue, .. } => Some(&lvalue.place.ty),
        };
        let width = ty.and_then(|ty| self.width(ty));
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
        let constant = match &candidate {
            Candidate::Value(value) => {
                integer_constant(value, self.types.compiler_flavor()).is_some()
            }
            Candidate::Output(_) | Candidate::Lvalue { .. } => false,
        };
        let decays = ty.is_some_and(|ty| {
            matches!(
                ty,
                Type::Array { .. } | Type::VariableArray { .. } | Type::Function { .. }
            )
        }) && matches!(candidate, Candidate::Lvalue { .. });
        Source {
            name: operand.name.as_ref().map(|name| name.value.clone()),
            constraint,
            candidate,
            width: if decays { None } else { width },
            constant,
        }
    }

    fn input_candidate<'a>(
        &mut self,
        expr: &'a ast::Expr,
        constraint: &AsmConstraint,
    ) -> Result<Candidate<'a>, ResolveError> {
        if !constraint.allows_memory() {
            return Ok(Candidate::Value(self.expr(expr)?.value));
        }
        let memory_only = constraint.memory_only();
        let lvalue = match self.place(expr) {
            Ok(lvalue) => lvalue,
            Err(_) if memory_only => {
                return Err(ResolveError::Invalid(
                    "asm input with a memory-only constraint is not an lvalue",
                ));
            }
            Err(_) => return Ok(Candidate::Value(self.expr(expr)?.value)),
        };
        let addressable = self.memory_place(&lvalue.place, memory_only)?;
        Ok(Candidate::Lvalue {
            expr,
            lvalue,
            addressable,
        })
    }

    fn width(&self, ty: &Type) -> Option<u64> {
        self.types
            .storage(ty.clone())
            .ok()
            .map(|storage| storage.size_bytes * 8)
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

enum Candidate<'a> {
    Output(AsmOperandKind),
    Value(Value),
    Lvalue {
        expr: &'a ast::Expr,
        lvalue: Lvalue,
        addressable: bool,
    },
}

struct Source<'a> {
    name: Option<String>,
    constraint: AsmConstraint,
    candidate: Candidate<'a>,
    width: Option<u64>,
    constant: bool,
}

enum Choice {
    Class(AsmOperandClass),
    Tie(usize),
}

// rust takes one class per operand, so the first alternative every operand can use wins.
fn select(sources: &[Source<'_>], family: TargetFamily) -> (Option<usize>, Vec<AsmRejection>) {
    let count = sources
        .first()
        .map_or(0, |source| source.constraint.alternatives.len());
    let mut rejected = Vec::new();
    for alternative in 0..count {
        let failure = sources.iter().enumerate().find_map(|(operand, source)| {
            choose(source, alternative, family)
                .err()
                .map(|reason| AsmRejection {
                    alternative,
                    operand,
                    reason,
                })
        });
        match failure {
            Some(rejection) => rejected.push(rejection),
            None => return (Some(alternative), rejected),
        }
    }
    (None, rejected)
}

fn choose(
    source: &Source<'_>,
    alternative: usize,
    family: TargetFamily,
) -> Result<Choice, AsmRejectReason> {
    let output = matches!(source.candidate, Candidate::Output(_));
    let location = source
        .constraint
        .alternatives
        .get(alternative)
        .map(|alternative| &alternative.location)
        .ok_or(AsmRejectReason::Missing)?;
    match location {
        AsmConstraintLocation::HardRegister(register) if clobber_only(register) => {
            Err(AsmRejectReason::ClobberOnly)
        }
        AsmConstraintLocation::HardRegister(register) => {
            Ok(Choice::Class(AsmOperandClass::Explicit(register.clone())))
        }
        AsmConstraintLocation::Matching(_) if output => Err(AsmRejectReason::Matching),
        AsmConstraintLocation::Matching(index) => Ok(Choice::Tie(*index)),
        AsmConstraintLocation::Letters { letters, classes } => {
            let mut first = None;
            let mut best: Option<(u8, &AsmOperandClass)> = None;
            for class in classes {
                match rank(source, class, output, family) {
                    Ok(rank) if best.is_none_or(|(best, _)| rank < best) => {
                        best = Some((rank, class));
                    }
                    Ok(_) => {}
                    Err(reason) => {
                        first.get_or_insert(reason);
                    }
                }
            }
            match best {
                Some((_, class)) => Ok(Choice::Class(class.clone())),
                None => Err(first.unwrap_or_else(|| AsmRejectReason::Unresolved(letters.clone()))),
            }
        }
    }
}

// a constant prefers the immediate and anything else prefers a register, as both compilers do.
fn rank(
    source: &Source<'_>,
    class: &AsmOperandClass,
    output: bool,
    family: TargetFamily,
) -> Result<u8, AsmRejectReason> {
    match class {
        AsmOperandClass::Immediate if !output && source.constant => Ok(0),
        AsmOperandClass::Immediate => Err(AsmRejectReason::NotConstant),
        AsmOperandClass::Register(AsmRegisterClass::X87Reg | AsmRegisterClass::MmxReg) => {
            Err(AsmRejectReason::ClobberOnly)
        }
        AsmOperandClass::Register(register) if fits(*register, source.width, family) => Ok(1),
        AsmOperandClass::Register(_) => Err(AsmRejectReason::Width),
        AsmOperandClass::Explicit(register) if clobber_only(register) => {
            Err(AsmRejectReason::ClobberOnly)
        }
        AsmOperandClass::Explicit(_) => Ok(2),
        AsmOperandClass::Memory => Ok(3),
        AsmOperandClass::Unresolved(letters) => Err(AsmRejectReason::Unresolved(letters.clone())),
    }
}

fn fits(register: AsmRegisterClass, width: Option<u64>, family: TargetFamily) -> bool {
    let Some(width) = width else {
        return false;
    };
    match register {
        AsmRegisterClass::Reg | AsmRegisterClass::RegAbcd | AsmRegisterClass::RegLegacy => {
            match family {
                TargetFamily::X86_64 | TargetFamily::AArch64 => matches!(width, 8 | 16 | 32 | 64),
                TargetFamily::X86 | TargetFamily::Arm32 => matches!(width, 8 | 16 | 32),
            }
        }
        AsmRegisterClass::XmmReg => matches!(width, 16 | 32 | 64 | 128),
        AsmRegisterClass::YmmReg => matches!(width, 16 | 32 | 64 | 128 | 256),
        AsmRegisterClass::ZmmReg => matches!(width, 16 | 32 | 64 | 128 | 256 | 512),
        AsmRegisterClass::KReg => matches!(width, 8 | 16 | 32 | 64),
        AsmRegisterClass::VReg | AsmRegisterClass::VRegLow16 | AsmRegisterClass::VRegLow8 => {
            matches!(width, 8 | 16 | 32 | 64 | 128)
        }
        AsmRegisterClass::SReg => width == 32,
        AsmRegisterClass::DReg => width == 64,
        AsmRegisterClass::X87Reg | AsmRegisterClass::MmxReg => false,
    }
}

fn clobber_only(register: &AsmRegister) -> bool {
    let name = register.canonical.unwrap_or(&register.spelling);
    ["st", "mm", "tmm"]
        .iter()
        .any(|prefix| name.starts_with(prefix))
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
