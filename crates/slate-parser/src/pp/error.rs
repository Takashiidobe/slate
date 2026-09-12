use super::Preprocessor;
use crate::ast::Loc;
use crate::files::display_path;
use miette::{Diagnostic, NamedSource, SourceSpan};
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

#[derive(Debug, Clone, PartialEq, Eq, Error)]
pub(super) enum PPErrorKind {
    #[error("unexpected conditional directive")]
    UnexpectedConditional,
    #[error("unterminated conditional directive")]
    UnterminatedConditional,
    #[error("multiple #else directives")]
    MultipleElse,
    #[error("#elif after #else")]
    ElifAfterElse,
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
    #[error("unsupported preprocessor directive")]
    UnsupportedDirective,
    #[error("header not found in search path: {0}")]
    HeaderNotFound(String),
    #[error("include cycle detected: {0}")]
    IncludeCycle(String),
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
