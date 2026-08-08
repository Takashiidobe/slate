use core::fmt;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ParseError {
    InvalidArch,
    InvalidVendor,
    InvalidKernel,
    InvalidEnv,
    InvalidObj,
    UnexpectedEnd,
    ExpectedDash,
    TrailingCharacters,
}

impl fmt::Display for ParseError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            ParseError::InvalidArch => write!(f, "invalid architecture"),
            ParseError::InvalidVendor => write!(f, "invalid vendor"),
            ParseError::InvalidKernel => write!(f, "invalid kernel"),
            ParseError::InvalidEnv => write!(f, "invalid environment/ABI"),
            ParseError::InvalidObj => write!(f, "invalid object format"),
            ParseError::UnexpectedEnd => write!(f, "unexpected end of triple string"),
            ParseError::ExpectedDash => write!(f, "expected '-' separator"),
            ParseError::TrailingCharacters => write!(f, "trailing characters after triple"),
        }
    }
}

impl core::error::Error for ParseError {}
