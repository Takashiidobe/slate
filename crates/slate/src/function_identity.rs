use std::collections::BTreeSet;

pub use slate_analysis::{FunctionIdentity, Known};

#[derive(Debug, Clone, Default, PartialEq, Eq, PartialOrd, Ord, Hash)]
/// How a call expression is bound to a function declaration.
pub enum CallBinding {
    /// A direct call with declaration and identity metadata.
    Direct {
        /// The recognized identity of the called function.
        identity: FunctionIdentity,
        /// The canonical Clang AST function type emitted by the macro-dump plugin, when available.
        canonical_type: Option<String>,
        /// The trusted declaration spelling, when available.
        trusted_declaration: Option<String>,
        /// Headers that establish trusted provenance for the declaration.
        trusted_headers: BTreeSet<String>,
    },
    /// An indirect call whose target is not statically known.
    Indirect,
    #[default]
    /// A call generated internally by the translator.
    Generated,
}

impl CallBinding {
    /// Returns the known libc identity, if this binding has one.
    pub fn known(&self) -> Option<Known> {
        match self {
            Self::Direct {
                identity: FunctionIdentity::Known(known),
                ..
            } => Some(*known),
            Self::Direct { .. } | Self::Indirect | Self::Generated => None,
        }
    }

    pub fn is_header_inline(&self) -> bool {
        matches!(
            self,
            Self::Direct {
                identity: FunctionIdentity::HeaderInline,
                ..
            }
        )
    }

    pub fn trusted_declaration(&self) -> Option<&str> {
        match self {
            Self::Direct {
                trusted_declaration,
                ..
            } => trusted_declaration.as_deref(),
            Self::Indirect | Self::Generated => None,
        }
    }

    /// Iterates over headers that establish trusted declaration provenance.
    pub fn trusted_headers(&self) -> Box<dyn Iterator<Item = &str> + '_> {
        match self {
            Self::Direct {
                trusted_headers, ..
            } => Box::new(trusted_headers.iter().map(String::as_str)),
            Self::Indirect | Self::Generated => Box::new([].into_iter()),
        }
    }
}
