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
    pub outputs: Vec<AsmOutput>,
    pub inputs: Vec<AsmInput>,
    pub clobbers: Vec<AsmClobber>,
    pub labels: Vec<BindingId>,
}

#[derive(Debug, Clone)]
pub enum AsmPiece {
    Text(String),
    Operand {
        index: usize,
        modifier: Option<char>,
    },
    Label(usize),
    Percent,
    UniqueId,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum AsmDialect {
    Att,
    Intel,
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
pub struct AsmOutput {
    pub name: Option<String>,
    pub constraint: AsmConstraint,
    pub place: Place,
}

#[derive(Debug, Clone)]
pub struct AsmInput {
    pub name: Option<String>,
    pub constraint: AsmConstraint,
    pub value: Value,
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
    Overwrite,
    ReadWrite,
    EarlyClobber,
    Commutative,
    Pic,
}

#[derive(Debug, Clone)]
pub enum AsmConstraintLocation {
    HardRegister(AsmRegister),
    Matching(usize),
    Letters(String),
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

impl InlineAsm {
    pub fn has_sections(&self) -> bool {
        !self.pieces.is_empty()
            || !self.outputs.is_empty()
            || !self.inputs.is_empty()
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
                modifier: None,
            } => write!(f, "%{index}"),
            Self::Operand {
                index,
                modifier: Some(modifier),
            } => write!(f, "%{modifier}{index}"),
            Self::Label(index) => write!(f, "%l{index}"),
            Self::Percent => f.write_str("%%"),
            Self::UniqueId => f.write_str("%="),
        }
    }
}

impl AsmConstraintModifier {
    fn spelling(self) -> char {
        match self {
            Self::Overwrite => '=',
            Self::ReadWrite => '+',
            Self::EarlyClobber => '&',
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
                AsmConstraintLocation::Letters(letters) => spelling.push_str(letters),
            }
        }
        write!(f, "{spelling:?}")
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
