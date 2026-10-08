use super::expression::Lowerer;
use super::fold::{integer_number, integer_with_objects, read_with_objects};
use super::numeric::ResolveError;
use super::operand::Lvalue;
use super::types::TypeResolver;
use crate::ast;
use crate::compiler_args::CompilerFlavor;
use crate::ir::*;
use crate::target_info::TargetFamily;
use num_bigint::BigInt;

impl Lowerer {
    pub(super) fn asm(&mut self, asm: &ast::GnuAsm) -> Result<InlineAsm, ResolveError> {
        let qualifier = |wanted: ast::AsmQualifier| {
            asm.qualifiers
                .iter()
                .any(|qualifier| qualifier.value == wanted)
        };
        let family = self.context.target.family;
        let dialect = match family {
            TargetFamily::X86 | TargetFamily::X86_64 => Some(self.context.asm_dialect),
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
            options: None,
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
            let source_width = source.width;
            let mut kind = match source.candidate {
                Candidate::Output(kind) => kind,
                Candidate::Value(_)
                    if let (Some(AsmOperandClass::Symbol), Some(symbol)) =
                        (&selected, source.symbol) =>
                {
                    AsmOperandKind::Symbol(symbol)
                }
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
            if let (Some(AsmOperandClass::Immediate), Some(constant), AsmOperandKind::In(value)) =
                (&selected, source.constant, &mut kind)
            {
                value.node.value = ValueKind::Constant(integer_number(&value.ty, constant));
            }
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
                    return Err(ResolveError::Internal("asm input tied to a non-output"));
                };
                // x86 prints a tied input at its own width, not its output's.
                let view = match input_width {
                    Some(bits) if family.is_x86() && input_width != *width => {
                        Some(AsmRegisterView::Bits(bits))
                    }
                    _ => None,
                };
                lowered.operands[output].kind = AsmOperandKind::InOut {
                    place: place.clone(),
                    input: Some(AsmTiedInput {
                        value: value.clone(),
                        operand: index,
                    }),
                    early_clobber: *early_clobber,
                };
                renumbered.push((output, view));
                continue;
            }
            renumbered.push((lowered.operands.len(), None));
            let width = match &kind {
                AsmOperandKind::In(value) => self.width(&value.ty),
                AsmOperandKind::Symbol(_) => source_width,
                AsmOperandKind::InPlace(place)
                | AsmOperandKind::Out { place, .. }
                | AsmOperandKind::InOut { place, .. }
                | AsmOperandKind::Memory { place, .. } => self.width(&place.ty),
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
                    .ok_or(ResolveError::Internal("missing asm label binding"))?,
            );
        }
        Ok(lowered)
    }
}

impl Lowerer {
    pub(super) fn asm_statement(&mut self, asm: &ast::GnuAsm) -> Result<InlineAsm, ResolveError> {
        let mut lowered = self.asm(asm)?;
        if self.in_naked_function {
            return Ok(lowered);
        }
        lowered.options = Some(options(
            &lowered,
            asm.operands.is_none(),
            self.context.target.family,
            self.types.flavor(),
        ));
        Ok(lowered)
    }

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
                | AsmOperandKind::InOut { place, .. }
                | AsmOperandKind::Memory { place, .. },
            ) => Some(&place.ty),
            Candidate::Output(AsmOperandKind::In(value)) | Candidate::Value(value) => {
                Some(&value.ty)
            }
            Candidate::Lvalue { lvalue, .. } => Some(&lvalue.place.ty),
            Candidate::Output(AsmOperandKind::Symbol(_)) => None,
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
        let target = self.types.fold_target();
        let objects = self.types.entities.constants();
        let (constant, symbol) = match &candidate {
            Candidate::Value(value) => (
                integer_with_objects(value, target, objects),
                self.symbol(value),
            ),
            Candidate::Lvalue { lvalue, .. } => {
                (read_with_objects(&lvalue.place, target, objects), None)
            }
            Candidate::Output(_) => (None, None),
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
            symbol,
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
                return Err(ResolveError::Internal(
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

    fn symbol(&self, value: &Value) -> Option<AsmSymbol> {
        match &value.node.value {
            ValueKind::AddressOf(place)
            | ValueKind::ArrayDecay { place, .. }
            | ValueKind::FunctionDecay { place } => self.symbol_place(place),
            ValueKind::Convert {
                kind:
                    ConversionKind::PointerCast
                    | ConversionKind::PtrToInt
                    | ConversionKind::IntToPtr
                    | ConversionKind::Reinterpret
                    | ConversionKind::BitCast
                    | ConversionKind::Widen
                    | ConversionKind::Truncate,
                operand,
                ..
            } if self.width(&value.ty) == Some(u64::from(self.context.target.pointer_width)) => {
                self.symbol(operand)
            }
            ValueKind::PointerOffset {
                pointer,
                amount,
                subtract,
                element,
                ..
            } => {
                let symbol = self.symbol(pointer)?;
                let bytes = self.scaled(amount, element)?;
                let offset = if *subtract {
                    symbol.offset.checked_sub(bytes)
                } else {
                    symbol.offset.checked_add(bytes)
                }?;
                Some(AsmSymbol { offset, ..symbol })
            }
            _ => None,
        }
    }

    fn symbol_place(&self, place: &Place) -> Option<AsmSymbol> {
        match &place.kind {
            PlaceKind::Binding(binding) => {
                let storage = self.types.entities.storage(*binding).or_else(|| {
                    self.module
                        .globals
                        .iter()
                        .find(|global| global.value.variable.id == *binding)
                        .map(|global| global.value.variable.storage)
                });
                let linked = match storage {
                    Some(StorageDuration::Static) => true,
                    // gcc prints a thread-local's symbol; clang rejects it as not a link-time address.
                    Some(StorageDuration::Thread) => self.types.flavor().is_gcc(),
                    Some(StorageDuration::Automatic) => false,
                    None => matches!(place.ty, Type::Function { .. }),
                };
                linked.then_some(AsmSymbol {
                    binding: *binding,
                    offset: 0,
                })
            }
            PlaceKind::Deref(pointer) => self.symbol(pointer),
            PlaceKind::Field {
                base,
                index,
                bits: None,
            } => {
                let symbol = self.symbol_place(base)?;
                let field = self.types.field_offset(&base.ty, *index)?;
                let offset = symbol.offset.checked_add(i64::try_from(field).ok()?)?;
                Some(AsmSymbol { offset, ..symbol })
            }
            PlaceKind::Index { base, index } => {
                let symbol = self.symbol(base)?;
                let offset = symbol.offset.checked_add(self.scaled(index, &place.ty)?)?;
                Some(AsmSymbol { offset, ..symbol })
            }
            _ => None,
        }
    }

    fn scaled(&self, amount: &Value, element: &Type) -> Option<i64> {
        let amount = integer_with_objects(
            amount,
            self.types.fold_target(),
            self.types.entities.constants(),
        )?;
        let amount = i64::try_from(amount).ok()?;
        let size = i64::try_from(self.types.storage(element.clone()).ok()?.size_bytes).ok()?;
        amount.checked_mul(size)
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
                Err(ResolveError::Internal("address of a bit-field"))
            }
            PlaceKind::Binding(id)
                if memory_only
                    && self.types.flavor().is_gcc()
                    && self.types.entities.is_register(id) =>
            {
                Err(ResolveError::Internal(
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
    constant: Option<BigInt>,
    symbol: Option<AsmSymbol>,
}

enum Choice {
    Class(AsmOperandClass),
    Tie(usize),
}

fn options(
    asm: &InlineAsm,
    basic: bool,
    family: TargetFamily,
    flavor: CompilerFlavor,
) -> AsmOptions {
    let outputs = asm
        .operands
        .iter()
        .any(|operand| operand.direction() != AsmDirection::In);
    let side_effects = asm.volatile || asm.goto || !outputs;
    let clobbers = |wanted: fn(&AsmClobber) -> bool| asm.clobbers.iter().any(wanted);
    let mut memory = AsmMemory::None;
    for operand in &asm.operands {
        let addresses = match &operand.selected {
            Some(class) => matches!(class, AsmOperandClass::Memory),
            None => operand.constraint.allows_memory(),
        };
        let access = match (&operand.kind, addresses) {
            (AsmOperandKind::InPlace(_), _) | (AsmOperandKind::In(_), true) => AsmMemory::ReadOnly,
            (AsmOperandKind::Out { .. } | AsmOperandKind::InOut { .. }, true) => AsmMemory::Any,
            _ => AsmMemory::None,
        };
        memory = memory.max(access);
    }
    if basic
        || clobbers(|clobber| matches!(clobber, AsmClobber::Memory))
        || (side_effects && !flavor.is_gcc())
    {
        memory = AsmMemory::Any;
    }
    AsmOptions {
        memory,
        pure: !side_effects && memory != AsmMemory::Any,
        nostack: true,
        preserves_flags: !family.is_x86() && !clobbers(|clobber| matches!(clobber, AsmClobber::Cc)),
        may_unwind: clobbers(|clobber| matches!(clobber, AsmClobber::Unwind)),
    }
}

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
        AsmOperandClass::Immediate if !output && source.constant.is_some() => Ok(0),
        AsmOperandClass::Symbol if !output && source.symbol.is_some() => Ok(0),
        AsmOperandClass::Immediate | AsmOperandClass::Symbol => Err(AsmRejectReason::NotConstant),
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

impl TypeResolver {
    pub(super) fn asm_operand_rule(
        &mut self,
        operand: &ast::AsmOperand,
        output: bool,
    ) -> Result<(), ResolveError> {
        let constraint = constraint(&operand.constraint.value, self.target_info().family);
        if !output && !constraint.allows_memory() {
            return Ok(());
        }
        let memory_only = constraint.memory_only();
        let typed = self.typed(&operand.expr)?;
        if !output && !typed.lvalue {
            return if memory_only {
                Err(ResolveError::Rejected(
                    "asm input with a memory-only constraint is not an lvalue",
                ))
            } else {
                Ok(())
            };
        }
        if memory_only && typed.bits.is_some() {
            return Err(ResolveError::Rejected("address of a bit-field"));
        }
        let mut expr = &operand.expr;
        while let ast::ExprKind::Paren(inner) = &expr.value {
            expr = inner;
        }
        if memory_only
            && self.flavor().is_gcc()
            && matches!(expr.value, ast::ExprKind::Identifier(_))
            && self
                .references
                .get(&expr.id)
                .is_some_and(|id| self.entities.is_register(id))
        {
            return Err(ResolveError::Rejected(
                "address of register variable requested",
            ));
        }
        Ok(())
    }
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
    let x86 = family.is_x86();
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
            (_, "n") => AsmOperandClass::Immediate,
            (_, "s") => AsmOperandClass::Symbol,
            (_, "Ws") if x86 => AsmOperandClass::Symbol,
            (_, "i") => {
                classes.extend([AsmOperandClass::Immediate, AsmOperandClass::Symbol]);
                continue;
            }
            (_, "g") => {
                classes.extend([
                    AsmOperandClass::Register(AsmRegisterClass::Reg),
                    AsmOperandClass::Memory,
                    AsmOperandClass::Immediate,
                    AsmOperandClass::Symbol,
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
