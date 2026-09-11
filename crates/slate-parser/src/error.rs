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

impl ParseError {
    pub fn new(
        name: impl Into<String>,
        source: impl Into<String>,
        offset: usize,
        length: usize,
        message: impl Into<String>,
    ) -> Self {
        let name = name.into();
        Self {
            message: message.into(),
            source_code: NamedSource::new(name, source.into()),
            span: (offset, length).into(),
        }
    }
}
