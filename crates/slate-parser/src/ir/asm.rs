use super::{BindingId, Place, Value};
use std::fmt;

#[derive(Debug, Clone)]
pub struct InlineAsm {
    pub template: String,
    pub volatile: bool,
    pub inline: bool,
    pub goto: bool,
    pub dialect: Option<AsmDialect>,
    pub pieces: Vec<AsmPiece>,
    pub operands: Vec<AsmOperand>,
    pub clobbers: Vec<AsmClobber>,
    pub labels: Vec<BindingId>,
    pub alternative: Option<usize>,
    pub rejected: Vec<AsmRejection>,
    pub options: Option<AsmOptions>,
}

impl InlineAsm {
    pub fn inputs_in_source_order(&self) -> Vec<usize> {
        let outputs = self
            .operands
            .iter()
            .filter(|operand| operand.direction() != AsmDirection::In)
            .count();
        let mut tied = self
            .operands
            .iter()
            .enumerate()
            .filter_map(|(index, operand)| match &operand.kind {
                AsmOperandKind::InOut {
                    input: Some(input), ..
                } => Some((input.operand, index)),
                _ => None,
            })
            .collect::<Vec<_>>();
        tied.sort_unstable();
        let mut tied = tied.into_iter().peekable();
        let mut untied = outputs..self.operands.len();
        let mut order = Vec::new();
        let mut source = outputs;
        loop {
            let next = tied
                .next_if(|(operand, _)| *operand == source)
                .map(|(_, index)| index)
                .or_else(|| untied.next())
                .or_else(|| tied.next().map(|(_, index)| index));
            let Some(index) = next else {
                return order;
            };
            order.push(index);
            source += 1;
        }
    }
}

// file-scope asm has none: `global_asm!` takes no options.
#[derive(Debug, Clone, Copy)]
pub struct AsmOptions {
    pub memory: AsmMemory,
    pub pure: bool,
    pub nostack: bool,
    pub preserves_flags: bool,
    pub may_unwind: bool,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub enum AsmMemory {
    None,
    ReadOnly,
    Any,
}

// `operand` is the source operand number, since a rejected tie may have been folded away.
#[derive(Debug, Clone)]
pub struct AsmRejection {
    pub alternative: usize,
    pub operand: usize,
    pub reason: AsmRejectReason,
}

#[derive(Debug, Clone)]
pub enum AsmRejectReason {
    Unresolved(String),
    ClobberOnly,
    Width,
    NotConstant,
    Matching,
    Missing,
}

#[derive(Debug, Clone)]
pub enum AsmPiece {
    Text(String),
    Operand {
        index: usize,
        modifier: Option<char>,
        view: Option<AsmRegisterView>,
    },
    Label(usize),
    Percent,
    UniqueId,
    // an MSVC `__asm` reference into a `Memory` operand's object.
    Address {
        operand: usize,
        displacement: i64,
        base: Option<String>,
        index: Option<(String, i64)>,
        size: Option<&'static str>,
    },
    // an MSVC `__asm` label, lowercased; rust `asm!` rejects named labels, so emission renumbers it.
    LocalLabel(String),
    EntryLabel {
        name: String,
        binding: BindingId,
    },
}

// the register slice a width modifier prints, which overrides the operand's own width.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum AsmRegisterView {
    Bits(u64),
    HighByte,
}

#[derive(Debug, Default, Clone, Copy, PartialEq, Eq)]
pub enum AsmDialect {
    #[default]
    Att,
    Intel,
}

impl std::str::FromStr for AsmDialect {
    type Err = String;

    fn from_str(value: &str) -> Result<Self, Self::Err> {
        match value {
            "att" => Ok(Self::Att),
            "intel" => Ok(Self::Intel),
            _ => Err(format!("unknown asm dialect: {value}")),
        }
    }
}

impl AsmDialect {
    pub fn as_str(self) -> &'static str {
        match self {
            Self::Att => "att",
            Self::Intel => "intel",
        }
    }
}

#[derive(Debug, Clone)]
pub struct AsmOperand {
    pub name: Option<String>,
    pub constraint: AsmConstraint,
    pub kind: AsmOperandKind,
    pub width: Option<u64>,
    pub selected: Option<AsmOperandClass>,
}

#[derive(Debug, Clone)]
pub enum AsmOperandKind {
    In(Value),
    // a memory-capable input names its object: the asm may address it instead of a pre-asm load.
    InPlace(Place),
    Out {
        place: Place,
        early_clobber: bool,
    },
    // a link-time constant address, which Rust expresses as a `sym` operand.
    Symbol(AsmSymbol),
    // `input` is a tied `"0"` operand; without one the place itself is read.
    InOut {
        place: Place,
        input: Option<AsmTiedInput>,
        early_clobber: bool,
    },
    // MSVC `__asm` addresses the object itself, so the operand is its address.
    Memory {
        place: Place,
        access: AsmAccess,
    },
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum AsmAccess {
    Read,
    Write,
    ReadWrite,
}

impl AsmAccess {
    pub fn as_str(self) -> &'static str {
        match self {
            Self::Read => "read",
            Self::Write => "write",
            Self::ReadWrite => "readwrite",
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct AsmSymbol {
    pub binding: BindingId,
    pub offset: i64,
}

#[derive(Debug, Clone)]
pub struct AsmTiedInput {
    pub value: Value,
    // the source operand number, which folding into the output's slot otherwise loses.
    pub operand: usize,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum AsmDirection {
    In,
    Out,
    LateOut,
    InOut,
    InLateOut,
}

impl AsmOperand {
    // without `&` the allocator may reuse an input's register, which is Rust's late form.
    pub fn direction(&self) -> AsmDirection {
        match self.kind {
            AsmOperandKind::In(_)
            | AsmOperandKind::InPlace(_)
            | AsmOperandKind::Symbol(_)
            | AsmOperandKind::Memory { .. } => AsmDirection::In,
            AsmOperandKind::Out {
                early_clobber: true,
                ..
            } => AsmDirection::Out,
            AsmOperandKind::Out { .. } => AsmDirection::LateOut,
            AsmOperandKind::InOut {
                early_clobber: true,
                ..
            } => AsmDirection::InOut,
            AsmOperandKind::InOut { .. } => AsmDirection::InLateOut,
        }
    }
}

impl AsmDirection {
    pub fn as_str(self) -> &'static str {
        match self {
            Self::In => "in",
            Self::Out => "out",
            Self::LateOut => "lateout",
            Self::InOut => "inout",
            Self::InLateOut => "inlateout",
        }
    }
}

#[derive(Debug, Clone)]
pub struct AsmConstraint {
    pub alternatives: Vec<AsmConstraintAlternative>,
}

#[derive(Debug, Clone)]
pub struct AsmConstraintAlternative {
    pub modifiers: Vec<AsmConstraintModifier>,
    pub location: AsmConstraintLocation,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum AsmConstraintModifier {
    Commutative,
    Pic,
}

#[derive(Debug, Clone)]
pub enum AsmConstraintLocation {
    HardRegister(AsmRegister),
    Matching(usize),
    Letters {
        letters: String,
        classes: Vec<AsmOperandClass>,
    },
}

#[derive(Debug, Clone)]
pub enum AsmOperandClass {
    Register(AsmRegisterClass),
    Explicit(AsmRegister),
    Memory,
    Immediate,
    Symbol,
    Unresolved(String),
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum AsmRegisterClass {
    Reg,
    RegAbcd,
    // no rust class exists for these subsets; emission pins a free register from the set.
    RegLegacy,
    VRegLow8,
    XmmReg,
    YmmReg,
    ZmmReg,
    KReg,
    X87Reg,
    MmxReg,
    VReg,
    VRegLow16,
    SReg,
    DReg,
}

impl AsmRegisterClass {
    pub fn as_str(self) -> &'static str {
        match self {
            Self::Reg => "reg",
            Self::RegAbcd => "reg_abcd",
            Self::RegLegacy => "reg_legacy",
            Self::VRegLow8 => "vreg_low8",
            Self::XmmReg => "xmm_reg",
            Self::YmmReg => "ymm_reg",
            Self::ZmmReg => "zmm_reg",
            Self::KReg => "kreg",
            Self::X87Reg => "x87_reg",
            Self::MmxReg => "mmx_reg",
            Self::VReg => "vreg",
            Self::VRegLow16 => "vreg_low16",
            Self::SReg => "sreg",
            Self::DReg => "dreg",
        }
    }
}

#[derive(Debug, Clone)]
pub enum AsmClobber {
    Memory,
    Cc,
    Unwind,
    Register(AsmRegister),
}

#[derive(Debug, Clone)]
pub struct AsmRegister {
    pub spelling: String,
    pub canonical: Option<&'static str>,
}

impl AsmConstraint {
    pub fn allows_memory(&self) -> bool {
        self.alternatives.iter().any(|alternative| {
            alternative
                .classes()
                .any(|class| matches!(class, AsmOperandClass::Memory))
        })
    }

    pub fn memory_only(&self) -> bool {
        self.alternatives.iter().all(|alternative| {
            alternative.classes().next().is_some()
                && alternative
                    .classes()
                    .all(|class| matches!(class, AsmOperandClass::Memory))
        })
    }
}

impl AsmConstraintAlternative {
    fn classes(&self) -> impl Iterator<Item = &AsmOperandClass> {
        match &self.location {
            AsmConstraintLocation::Letters { classes, .. } => classes.iter(),
            AsmConstraintLocation::HardRegister(_) | AsmConstraintLocation::Matching(_) => {
                [].iter()
            }
        }
    }
}

impl InlineAsm {
    pub fn has_sections(&self) -> bool {
        !self.pieces.is_empty()
            || !self.operands.is_empty()
            || !self.clobbers.is_empty()
            || !self.labels.is_empty()
    }
}

impl fmt::Display for AsmPiece {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::Text(text) => write!(f, "{text:?}"),
            Self::Operand {
                index,
                modifier,
                view,
            } => {
                f.write_str("%")?;
                if let Some(modifier) = modifier {
                    write!(f, "{modifier}")?;
                }
                write!(f, "{index}")?;
                match view {
                    Some(AsmRegisterView::Bits(bits)) => write!(f, "({bits})"),
                    Some(AsmRegisterView::HighByte) => f.write_str("(high8)"),
                    None => Ok(()),
                }
            }
            Self::Label(index) => write!(f, "%l{index}"),
            Self::Percent => f.write_str("%%"),
            Self::UniqueId => f.write_str("%="),
            Self::Address {
                operand,
                displacement,
                base,
                index,
                size,
            } => {
                f.write_str("addr")?;
                if let Some(size) = size {
                    write!(f, "<{size}>")?;
                }
                write!(f, "(%{operand}")?;
                if let Some(base) = base {
                    write!(f, " + {base}")?;
                }
                if let Some((index, scale)) = index {
                    write!(f, " + {index}*{scale}")?;
                }
                match displacement {
                    0 => {}
                    displacement if *displacement < 0 => {
                        write!(f, " - {}", displacement.unsigned_abs())?
                    }
                    displacement => write!(f, " + {displacement}")?,
                }
                f.write_str(")")
            }
            Self::LocalLabel(name) => write!(f, "label({name})"),
            Self::EntryLabel { name, binding } => write!(f, "entry_label(%{}, {name})", binding.0),
        }
    }
}

impl AsmConstraintModifier {
    fn spelling(self) -> char {
        match self {
            Self::Commutative => '%',
            Self::Pic => '-',
        }
    }
}

impl fmt::Display for AsmConstraint {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let mut spelling = String::new();
        for (index, alternative) in self.alternatives.iter().enumerate() {
            if index > 0 {
                spelling.push(',');
            }
            for modifier in &alternative.modifiers {
                spelling.push(modifier.spelling());
            }
            match &alternative.location {
                AsmConstraintLocation::HardRegister(register) => {
                    spelling.push('{');
                    spelling.push_str(&register.spelling);
                    spelling.push('}');
                }
                AsmConstraintLocation::Matching(index) => {
                    spelling.push_str(&index.to_string());
                }
                AsmConstraintLocation::Letters { letters, .. } => spelling.push_str(letters),
            }
        }
        write!(f, "{spelling:?}")
    }
}

impl fmt::Display for AsmConstraintAlternative {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match &self.location {
            AsmConstraintLocation::HardRegister(register) => {
                write!(
                    f,
                    "{{{}}}",
                    register.canonical.unwrap_or(&register.spelling)
                )
            }
            AsmConstraintLocation::Matching(index) => write!(f, "{index}"),
            AsmConstraintLocation::Letters { classes, .. } => {
                for (index, class) in classes.iter().enumerate() {
                    if index > 0 {
                        f.write_str(" | ")?;
                    }
                    write!(f, "{class}")?;
                }
                Ok(())
            }
        }
    }
}

impl fmt::Display for AsmOperandClass {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::Register(class) => f.write_str(class.as_str()),
            Self::Explicit(register) => write!(f, "{{{}}}", register.spelling),
            Self::Memory => f.write_str("mem"),
            Self::Immediate => f.write_str("imm"),
            Self::Symbol => f.write_str("sym"),
            Self::Unresolved(letters) => write!(f, "unresolved({letters:?})"),
        }
    }
}

impl fmt::Display for AsmOptions {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let memory = match self.memory {
            AsmMemory::Any => None,
            AsmMemory::ReadOnly => Some("readonly"),
            AsmMemory::None => Some("nomem"),
        };
        let names = [
            self.pure.then_some("pure"),
            memory,
            self.nostack.then_some("nostack"),
            self.preserves_flags.then_some("preserves_flags"),
            self.may_unwind.then_some("may_unwind"),
        ];
        for (index, name) in names.into_iter().flatten().enumerate() {
            if index > 0 {
                f.write_str(",")?;
            }
            f.write_str(name)?;
        }
        Ok(())
    }
}

impl fmt::Display for AsmRejection {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(
            f,
            "{} (operand {}: {})",
            self.alternative, self.operand, self.reason
        )
    }
}

impl fmt::Display for AsmRejectReason {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::Unresolved(letters) => write!(f, "unresolved({letters:?})"),
            Self::ClobberOnly => f.write_str("clobber-only"),
            Self::Width => f.write_str("width"),
            Self::NotConstant => f.write_str("not-constant"),
            Self::Matching => f.write_str("matching"),
            Self::Missing => f.write_str("missing"),
        }
    }
}

impl fmt::Display for AsmRegister {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self.canonical {
            Some(canonical) => write!(f, "{:?} as {canonical}", self.spelling),
            None => write!(f, "{:?}", self.spelling),
        }
    }
}

impl fmt::Display for AsmClobber {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::Memory => f.write_str("memory"),
            Self::Cc => f.write_str("cc"),
            Self::Unwind => f.write_str("unwind"),
            Self::Register(register) => write!(f, "{register}"),
        }
    }
}
