use super::Parser;
use crate::dialect::Dialect;
use crate::standard_features::StandardFeatures;

#[derive(Clone, Copy)]
pub(crate) enum ParseContext<'a> {
    Parser(&'a Parser),
    Standalone(&'a Dialect),
}

impl<'a> ParseContext<'a> {
    pub(crate) fn dialect(self) -> &'a Dialect {
        match self {
            Self::Parser(parser) => parser.dialect(),
            Self::Standalone(dialect) => dialect,
        }
    }

    pub(crate) fn features(self) -> StandardFeatures {
        self.dialect().features()
    }

    pub(crate) fn parser(self) -> Option<&'a Parser> {
        match self {
            Self::Parser(parser) => Some(parser),
            Self::Standalone(_) => None,
        }
    }

    pub(crate) fn is_typedef(self, name: &str) -> bool {
        self.parser().is_some_and(|parser| parser.is_typedef(name))
    }
}

impl Parser {
    pub(crate) fn context(&self) -> ParseContext<'_> {
        ParseContext::Parser(self)
    }
}
