use super::Preprocessor;
use crate::ast::Loc;
use crate::files::display_path;
use miette::{Diagnostic, LabeledSpan, NamedSource, Severity, SourceCode, SourceSpan};
use thiserror::Error;

#[derive(Debug, Error, Diagnostic, Clone)]
#[error("{message}")]
pub struct PPError {
    pub message: String,
    #[source_code]
    pub source_code: NamedSource<String>,
    #[label]
    pub span: Option<SourceSpan>,
}

#[derive(Debug, Clone, Error)]
#[error("{}", .error.message)]
pub struct DirectiveDiagnostic {
    pub severity: Severity,
    pub text: String,
    pub error: PPError,
}

impl Diagnostic for DirectiveDiagnostic {
    fn severity(&self) -> Option<Severity> {
        Some(self.severity)
    }

    fn source_code(&self) -> Option<&dyn SourceCode> {
        self.error.source_code()
    }

    fn labels(&self) -> Option<Box<dyn Iterator<Item = LabeledSpan> + '_>> {
        self.error.labels()
    }
}

#[derive(Debug, Error, Diagnostic)]
#[error("preprocessing failed")]
pub struct DirectiveErrors {
    #[related]
    pub errors: Vec<DirectiveDiagnostic>,
}

#[derive(Debug, Clone, PartialEq, Eq, Error)]
pub(super) enum PPErrorKind {
    #[error("unexpected conditional directive")]
    UnexpectedConditional,
    #[error("unterminated conditional directive")]
    UnterminatedConditional,
    #[error("multiple #else directives")]
    MultipleElse,
    #[error("{0} after #else")]
    ElifAfterElse(&'static str),
    #[error("invalid {directive} expression: {message}")]
    InvalidExpression {
        directive: &'static str,
        message: String,
    },
    #[error("expected macro name after {0}")]
    ExpectedMacroName(&'static str),
    #[error("expected `)` after macro parameters")]
    ExpectedParametersClose,
    #[error("expected \"FILENAME\" or <FILENAME>")]
    ExpectedHeaderName,
    #[error("expected resource name after #embed")]
    ExpectedEmbedResource,
    #[error("invalid #embed parameter")]
    InvalidEmbedParameter,
    #[error("invalid #line directive, expected a digit sequence")]
    InvalidLineDirective,
    #[error("{0}")]
    Directive(String),
    #[error("unsupported preprocessor directive")]
    UnsupportedDirective,
    #[error("{0}")]
    Assertion(&'static str),
    #[error("header not found in search path: {0}")]
    HeaderNotFound(String),
    #[error("#include nested too deeply")]
    IncludeTooDeep,
    #[error("failed to read {path}: {message}")]
    ReadFailed { path: String, message: String },
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct PPFailure {
    loc: Option<Loc>,
    kind: PPErrorKind,
}

impl PPFailure {
    pub(super) fn at(loc: Loc, kind: PPErrorKind) -> Self {
        Self {
            loc: Some(loc),
            kind,
        }
    }

    pub(super) fn unlocated(kind: PPErrorKind) -> Self {
        Self { loc: None, kind }
    }
}

impl Preprocessor<'_> {
    pub(super) fn render_error(&self, failure: PPFailure) -> PPError {
        let message = failure.kind.to_string();
        let Some(loc) = failure.loc else {
            return PPError {
                message,
                source_code: NamedSource::new("", String::new()),
                span: None,
            };
        };
        let source = self.sources.get(&loc.file).cloned().unwrap_or_default();
        let offset = loc.offset.min(source.len());
        let length = loc.length.min(source.len() - offset);
        PPError {
            message,
            source_code: NamedSource::new(display_path(self.files.path(loc.file)), source)
                .with_language("C"),
            span: Some((offset, length).into()),
        }
    }
}
