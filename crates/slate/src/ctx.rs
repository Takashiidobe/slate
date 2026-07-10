//! Shared translation context: diagnostics and the symbol table threaded through
//! the passes. The three input sources join here by source location.

use std::collections::BTreeMap;

/// Severity of a diagnostic. Lowering emits `Warning` for a `todo!()` fallback so
/// output still compiles-and-fails loudly rather than miscompiling silently.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Severity {
    Warning,
    Error,
}

#[derive(Debug, Clone)]
pub struct Diagnostic {
    pub severity: Severity,
    pub message: String,
    /// Verbatim CIR `loc(...)` if the offending op carried one.
    pub loc: Option<String>,
}

/// Collected non-fatal diagnostics.
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

/// A global symbol collected in build-symbols: function signatures and the
/// constant strings that `printf`-style calls reference.
#[derive(Debug, Clone)]
pub enum Symbol {
    /// A global constant string literal, decoded to its bytes (no NUL).
    ConstStr(Vec<u8>),
    /// A function signature, kept as raw CIR type text for V0.
    Func { ty: String },
}

#[derive(Debug, Default)]
pub struct SymbolTable {
    pub globals: BTreeMap<String, Symbol>,
}

/// The mutable state threaded through the pipeline.
#[derive(Debug, Default)]
pub struct Ctx {
    pub diagnostics: Diagnostics,
    pub symbols: SymbolTable,
}
