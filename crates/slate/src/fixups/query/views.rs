use std::marker::PhantomData;

use crate::fixups::facts::{AstPath, BindingId};

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(in crate::fixups) struct ExprSite {
    pub(in crate::fixups) item_index: usize,
    pub(in crate::fixups) path: AstPath,
    pub(super) fact_path: AstPath,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(in crate::fixups) enum DefinitionLocation {
    Item(usize),
    ExternDecl {
        item_index: usize,
        decl_index: usize,
    },
}

impl DefinitionLocation {
    pub(in crate::fixups) fn item_index(&self) -> usize {
        match self {
            Self::Item(item_index) => *item_index,
            Self::ExternDecl { item_index, .. } => *item_index,
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(in crate::fixups) enum DefinitionKind {
    Function,
    ExternFunction,
    ExternStatic,
    SupportModule,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(in crate::fixups) enum DefinitionGroup {
    Header(String),
    SupportModule(String),
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(in crate::fixups) struct DefinitionSelector {
    pub(in crate::fixups) kind: DefinitionKind,
    pub(in crate::fixups) name: String,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct DefinitionSite {
    pub(in crate::fixups) location: DefinitionLocation,
    pub(in crate::fixups) kind: DefinitionKind,
    pub(in crate::fixups) name: String,
    pub(in crate::fixups) symbols: Vec<String>,
    pub(in crate::fixups) group: Option<DefinitionGroup>,
    pub(super) externally_reachable: bool,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct ZeroUsers {
    pub(in crate::fixups) definition: DefinitionSite,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct ZeroGroupUsers {
    pub(in crate::fixups) group: DefinitionGroup,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(in crate::fixups) enum ByteRepresentation {
    Collection,
    Bytes,
    CStr,
    Str,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(in crate::fixups) enum PointerMutability {
    Const,
    Mut,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(in crate::fixups) enum ByteExtent {
    Constant(usize),
    Dynamic,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(in crate::fixups) enum NulPosition {
    Constant(usize),
    ByteLength,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct ByteSource<'snapshot> {
    pub(in crate::fixups) site: ExprSite,
    pub(in crate::fixups) name: String,
    pub(in crate::fixups) representation: ByteRepresentation,
    pub(in crate::fixups) mutability: PointerMutability,
    pub(in crate::fixups) extent: ByteExtent,
    pub(super) binding: BindingId,
    pub(super) snapshot: PhantomData<&'snapshot ()>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct ByteView<'snapshot> {
    pub(in crate::fixups) source: ByteSource<'snapshot>,
    pub(in crate::fixups) extent: ByteExtent,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct StableExpr {
    pub(in crate::fixups) site: ExprSite,
}
