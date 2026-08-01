use std::marker::PhantomData;

use crate::fixups::facts::{AstPath, BindingId, HeapOwnershipKind, HeapResizeKind, Purity};
use crate::rust_ast::{Expr, Type};

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(in crate::fixups) struct Usage {
    pub(in crate::fixups) reads: usize,
    pub(in crate::fixups) writes: usize,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct ResolvedValue {
    pub(in crate::fixups) ty: Option<Type>,
    pub(in crate::fixups) usage: Option<Usage>,
    pub(in crate::fixups) purity: Option<Purity>,
}

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

#[derive(Debug, Clone)]
pub(in crate::fixups) struct AnonymousStructSet {
    pub(super) structs: Vec<AnonymousStructPlan>,
}

#[derive(Debug, Clone)]
pub(super) struct AnonymousStructPlan {
    pub(super) item_index: usize,
    pub(super) original_name: String,
    pub(super) generated_name: String,
    pub(super) fields: Vec<AnonymousStructField>,
}

#[derive(Debug, Clone)]
pub(super) struct AnonymousStructField {
    pub(super) name: String,
    pub(super) ty: Type,
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

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
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

/// A run of `[start, end)` adjacent statements inside the `Vec<IndentStmt>`
/// reached by `path` (the container itself, not any one statement in it).
#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(in crate::fixups) struct StmtWindowSite {
    pub(in crate::fixups) item_index: usize,
    pub(in crate::fixups) path: AstPath,
    pub(in crate::fixups) start: usize,
    pub(in crate::fixups) end: usize,
}

#[derive(Debug, Clone)]
pub(in crate::fixups) struct LazySingletonSet {
    pub(super) singletons: Vec<LazySingletonPlan>,
}

#[derive(Debug, Clone)]
pub(super) struct LazySingletonPlan {
    pub(super) function_item_index: usize,
    pub(super) function_name: String,
    pub(super) payload_item_index: usize,
    pub(super) payload_name: String,
    pub(super) payload_ty: Type,
    pub(super) init_expr: Expr,
    pub(super) flag_item_index: usize,
    pub(super) flag_name: String,
}

#[derive(Debug, Clone)]
pub(in crate::fixups) struct HeapOwnershipPlanSet {
    pub(super) plans: Vec<HeapOwnershipPlan>,
}

#[derive(Debug, Clone)]
pub(super) struct HeapOwnershipPlan {
    pub(super) pointer_name: String,
    pub(super) kind: HeapOwnershipKind,
    pub(super) pointer_stmt: Option<usize>,
    pub(super) size_stmt: Option<usize>,
    pub(super) allocation_stmt: Option<usize>,
    pub(super) assign_stmt: Option<usize>,
    pub(super) free_temp_stmt: Option<usize>,
    pub(super) free_stmt: Option<usize>,
    pub(super) reallocs: Vec<HeapOwnershipReallocPlan>,
    pub(super) elem_ty: Type,
    pub(super) init: Expr,
    pub(super) count: Option<Expr>,
}

#[derive(Debug, Clone)]
pub(super) struct HeapOwnershipReallocPlan {
    pub(super) source_temp_stmt: Option<usize>,
    pub(super) size_stmt: Option<usize>,
    pub(super) allocation_stmt: Option<usize>,
    pub(super) assign_stmt: Option<usize>,
    pub(super) resize: HeapResizeKind,
    pub(super) count: Expr,
}

#[derive(Debug, Clone)]
pub(in crate::fixups) struct StringLiftPlanSet {
    pub(super) plans: Vec<StringLiftPlan>,
}

#[derive(Debug, Clone)]
pub(super) struct StringLiftPlan {
    pub(super) path: AstPath,
    pub(super) name: String,
    pub(super) ty: Type,
    pub(super) expr: Expr,
    pub(super) remove_path: Option<AstPath>,
}
