use std::fmt;

#[derive(Debug, Clone, Copy, PartialEq, Eq, Default)]
pub struct SourceLocation {
    pub line: usize,
    pub column: usize,
    pub byte: usize,
    pub file_no: usize,
}

#[derive(Debug, Clone)]
pub struct CompileError {
    message: String,
    location: Option<SourceLocation>,
}

impl CompileError {
    pub fn new(message: impl Into<String>) -> Self {
        Self {
            message: message.into(),
            location: None,
        }
    }

    pub fn at(message: impl Into<String>, location: SourceLocation) -> Self {
        Self {
            message: message.into(),
            location: Some(location),
        }
    }

    pub fn location(&self) -> Option<SourceLocation> {
        self.location
    }

    pub fn message(&self) -> &str {
        &self.message
    }
}

impl fmt::Display for CompileError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.message)
    }
}

impl std::error::Error for CompileError {}

pub type CompileResult<T> = Result<T, CompileError>;
