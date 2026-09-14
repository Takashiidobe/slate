//! Diagnostics accumulated while lowering C code into Rust.
//!
//! Lowering records recoverable warnings and fatal errors in [`Diagnostics`].

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
/// The severity assigned to a [`Diagnostic`].
pub enum Severity {
    /// A problem that does not prevent translation from continuing.
    Warning,
    /// A problem that makes the translated program invalid.
    Error,
}

#[derive(Debug, Clone)]
/// A warning or error produced during translation.
pub struct Diagnostic {
    /// Whether this diagnostic is a warning or an error.
    pub severity: Severity,
    /// A human-readable description of the problem.
    pub message: String,
}

#[derive(Debug, Default, Clone)]
/// Diagnostics accumulated during a translation stage.
///
/// Use [`warn`](Self::warn) and [`error`](Self::error) to record diagnostics,
/// then [`has_errors`](Self::has_errors) to decide whether translation can continue.
///
/// # Examples
///
/// ```
/// use slate::ctx::Diagnostics;
///
/// let mut diagnostics = Diagnostics::default();
/// diagnostics.warn("unsupported optimization");
/// diagnostics.error("could not lower expression");
/// assert!(diagnostics.has_errors());
/// ```
pub struct Diagnostics {
    /// The diagnostics in the order they were recorded.
    pub items: Vec<Diagnostic>,
}

impl std::fmt::Display for Diagnostics {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        for diagnostic in &self.items {
            write!(f, "\n{:?}: {}", diagnostic.severity, diagnostic.message)?;
        }
        Ok(())
    }
}

impl Diagnostics {
    /// Records a warning without preventing translation from continuing.
    pub fn warn(&mut self, message: impl Into<String>) {
        self.items.push(Diagnostic {
            severity: Severity::Warning,
            message: message.into(),
        });
    }

    /// Records an error that should prevent the current translation from succeeding.
    pub fn error(&mut self, message: impl Into<String>) {
        self.items.push(Diagnostic {
            severity: Severity::Error,
            message: message.into(),
        });
    }

    /// Returns whether any recorded diagnostic has [`Severity::Error`].
    pub fn has_errors(&self) -> bool {
        self.items.iter().any(|d| d.severity == Severity::Error)
    }
}

#[derive(Debug, Default)]
/// Context passed through lowering to collect [`Diagnostics`].
pub struct Ctx {
    /// Diagnostics recorded by the lowering pass.
    pub diagnostics: Diagnostics,
}
