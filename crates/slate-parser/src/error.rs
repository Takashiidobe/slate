use crate::pp::PPError;
use crate::sema::SemaErrors;
use miette::{Diagnostic, NamedSource, SourceSpan};
use thiserror::Error;

#[derive(Debug, Error, Diagnostic)]
#[error("{message}")]
pub struct ParseError {
    pub message: String,
    #[source_code]
    pub source_code: NamedSource<String>,
    #[label]
    pub span: SourceSpan,
}

#[derive(Debug, Error, Diagnostic)]
pub enum FrontendError {
    #[error("{0}")]
    #[diagnostic(transparent)]
    PP(#[source] PPError),
    #[error("{0}")]
    #[diagnostic(transparent)]
    Parse(#[source] ParseError),
    #[error("{0}")]
    #[diagnostic(transparent)]
    Sema(#[source] SemaErrors),
}

impl ParseError {
    pub fn new(
        name: impl Into<String>,
        source: impl Into<String>,
        offset: usize,
        length: usize,
        message: impl Into<String>,
    ) -> Self {
        let name = name.into();
        let source = source.into();
        let offset = offset.min(source.len());
        let length = length.min(source.len() - offset);
        Self {
            message: message.into(),
            source_code: NamedSource::new(name, source).with_language("C"),
            span: (offset, length).into(),
        }
    }
}
