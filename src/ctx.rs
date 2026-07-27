use std::collections::BTreeMap;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Severity {
    Warning,
    Error,
}

#[derive(Debug, Clone)]
pub struct Diagnostic {
    pub severity: Severity,
    pub message: String,
    pub loc: Option<String>,
}

#[derive(Debug, Default)]
pub struct Diagnostics {
    pub items: Vec<Diagnostic>,
}

impl Diagnostics {
    pub fn warn(&mut self, message: impl Into<String>, loc: Option<String>) {
        self.items.push(Diagnostic {
            severity: Severity::Warning,
            message: message.into(),
            loc,
        });
    }

    pub fn error(&mut self, message: impl Into<String>, loc: Option<String>) {
        self.items.push(Diagnostic {
            severity: Severity::Error,
            message: message.into(),
            loc,
        });
    }

    pub fn has_errors(&self) -> bool {
        self.items.iter().any(|d| d.severity == Severity::Error)
    }
}

#[derive(Debug, Clone)]
pub enum Symbol {
    ConstStr(Vec<u8>),
    Func { ty: String },
}

#[derive(Debug, Default)]
pub struct SymbolTable {
    pub globals: BTreeMap<String, Symbol>,
}

#[derive(Debug, Default)]
pub struct Ctx {
    pub diagnostics: Diagnostics,
    pub symbols: SymbolTable,
}
