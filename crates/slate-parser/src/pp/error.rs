use super::Preprocessor;
use miette::{Diagnostic, NamedSource, SourceSpan};
use thiserror::Error;

#[derive(Debug, Error, Diagnostic, Clone)]
#[error("{message}")]
pub struct PPError {
    pub message: String,
    #[source_code]
    pub source_code: NamedSource<String>,
    #[label]
    pub span: SourceSpan,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct PPFailure {
    line: usize,
    message: String,
}

impl Preprocessor<'_> {
    pub(super) fn error(&self, line: usize, message: impl Into<String>) -> PPFailure {
        PPFailure {
            line,
            message: message.into(),
        }
    }

    pub(super) fn with_source(&self, error: PPFailure, name: &str, source: &str) -> PPError {
        let offset: usize = source
            .lines()
            .take(error.line)
            .map(|line| line.len() + 1)
            .sum();
        PPError {
            message: error.message,
            source_code: NamedSource::new(name, source.to_string()).with_language("C"),
            span: SourceSpan::new(offset.into(), 1),
        }
    }
}
