#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Severity {
    Warning,
    Error,
}

#[derive(Debug, Clone)]
pub struct Diagnostic {
    pub severity: Severity,
    pub message: String,
}

#[derive(Debug, Default)]
pub struct Diagnostics {
    pub items: Vec<Diagnostic>,
}

impl Diagnostics {
    pub fn warn(&mut self, message: impl Into<String>) {
        self.items.push(Diagnostic {
            severity: Severity::Warning,
            message: message.into(),
        });
    }

    pub fn error(&mut self, message: impl Into<String>) {
        self.items.push(Diagnostic {
            severity: Severity::Error,
            message: message.into(),
        });
    }

    pub fn has_errors(&self) -> bool {
        self.items.iter().any(|d| d.severity == Severity::Error)
    }
}

#[derive(Debug, Default)]
pub struct Ctx {
    pub diagnostics: Diagnostics,
}
