use std::collections::BTreeSet;
use std::marker::PhantomData;

use crate::fixups::facts::{
    AstPath, BindingId, ConstValue, EffectKind, HeapOwnershipKind, HeapResizeKind, PlaceAccess,
    PlaceKind, Purity,
};
use crate::rust_ast::{Expr, FnDef, Type};

use super::item::StatementRef;

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

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct BindingRef {
    pub(in crate::fixups) item_index: usize,
    pub(in crate::fixups) function_name: String,
    pub(in crate::fixups) name: String,
    pub(in crate::fixups) definition: AstPath,
    pub(in crate::fixups) kind: BindingCategory,
    pub(in crate::fixups) ty: Option<Type>,
    pub(super) id: BindingId,
}

impl BindingRef {
    pub(in crate::fixups) fn value_site(&self) -> ValueSite {
        ValueSite {
            item_index: self.item_index,
            path: self.definition.clone(),
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(in crate::fixups) enum BindingCategory {
    Parameter { index: usize },
    Local,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct FunctionRef {
    pub(in crate::fixups) item_index: usize,
    pub(in crate::fixups) name: String,
    pub(super) id: crate::fixups::facts::FunctionId,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct ProgramRef {
    pub(in crate::fixups) expected_len: usize,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct ParameterRef {
    pub(in crate::fixups) binding: BindingRef,
    pub(in crate::fixups) index: usize,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(in crate::fixups) struct StatementContainerRef {
    pub(in crate::fixups) item_index: usize,
    pub(in crate::fixups) path: AstPath,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(in crate::fixups) struct MatchArmRef {
    pub(in crate::fixups) statement: super::item::StatementRef,
    pub(in crate::fixups) index: usize,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(in crate::fixups) struct FieldRef {
    pub(in crate::fixups) item_index: usize,
    pub(in crate::fixups) index: usize,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(in crate::fixups) struct EnumVariantRef {
    pub(in crate::fixups) item_index: usize,
    pub(in crate::fixups) index: usize,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) enum TypeUseRef {
    FunctionReturn(FunctionRef),
    Parameter(ParameterRef),
    Field(FieldRef),
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(in crate::fixups) enum TypeUseKind {
    FunctionReturn,
    Parameter,
    Field,
}

#[derive(Debug, Clone)]
pub(in crate::fixups) struct ParameterRemoval {
    pub(in crate::fixups) binding: BindingRef,
    pub(in crate::fixups) function: FunctionRef,
    pub(in crate::fixups) replacement: FnDef,
    pub(in crate::fixups) index: usize,
    pub(in crate::fixups) calls: Vec<(ExprSite, Expr)>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct BindingDefUse {
    pub(in crate::fixups) binding: BindingRef,
    pub(in crate::fixups) reads: Vec<UseSiteRef>,
    pub(in crate::fixups) writes: Vec<UseSiteRef>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) enum UseSiteRef {
    Expression(ExpressionRef),
    Statement(super::item::StatementRef),
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct ExpressionRef {
    pub(in crate::fixups) site: ExprSite,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub(in crate::fixups) enum ExpressionRole {
    AssignmentTarget,
    AssignmentValue,
    Call,
    CallArgument(usize),
    CallCallee,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(in crate::fixups) enum ExpressionKind {
    Call,
    Cast,
    Field,
    Index,
    Literal,
    Variable,
    Other,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct ExpressionEffects {
    pub(in crate::fixups) purity: Purity,
    pub(in crate::fixups) effects: BTreeSet<EffectKind>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct ExpressionPlace {
    pub(in crate::fixups) access: PlaceAccess,
    pub(in crate::fixups) kind: PlaceKind,
    pub(in crate::fixups) readable: bool,
    pub(in crate::fixups) assignable: bool,
    pub(in crate::fixups) ordinary_slot: bool,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(in crate::fixups) enum BindingAccess {
    Read,
    Write,
    ReadWrite,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct BindingUse {
    pub(in crate::fixups) site: UseSiteRef,
    pub(in crate::fixups) access: BindingAccess,
}

impl BindingUse {
    pub(in crate::fixups) fn expression(&self) -> Option<&ExpressionRef> {
        match &self.site {
            UseSiteRef::Expression(expression) => Some(expression),
            UseSiteRef::Statement(_) => None,
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct BindingUses {
    pub(in crate::fixups) binding: BindingRef,
    pub(in crate::fixups) uses: Vec<BindingUse>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct ExpressionValues {
    pub(in crate::fixups) values: Vec<ConstValue>,
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
    Struct,
    Record,
    Enum,
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
pub(in crate::fixups) struct DefinitionUsers {
    pub(in crate::fixups) definition: DefinitionSite,
    pub(in crate::fixups) users: usize,
    pub(in crate::fixups) site: ExprSite,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct DefinitionGroupUsers {
    pub(in crate::fixups) group: DefinitionGroup,
    pub(in crate::fixups) users: usize,
    pub(in crate::fixups) site: ExprSite,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct ItemReferences {
    pub(in crate::fixups) item_index: usize,
    pub(in crate::fixups) symbols: BTreeSet<String>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct ReferenceDomain {
    pub(in crate::fixups) definitions: Vec<DefinitionSite>,
    pub(in crate::fixups) items: Vec<ItemReferences>,
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

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(in crate::fixups) struct StatementRange {
    pub(in crate::fixups) item_index: usize,
    pub(in crate::fixups) path: AstPath,
    pub(in crate::fixups) start: usize,
    pub(in crate::fixups) end: usize,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(in crate::fixups) struct ValueSite {
    pub(in crate::fixups) item_index: usize,
    pub(in crate::fixups) path: AstPath,
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
pub(in crate::fixups) struct ArrayElementPointerOrigin {
    pub(in crate::fixups) pointer_name: String,
    pub(in crate::fixups) base_name: String,
    pub(in crate::fixups) index: Expr,
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
pub(in crate::fixups) struct PtrLenPlanSet {
    pub(super) plans: Vec<PtrLenPlan>,
}

#[derive(Debug, Clone)]
pub(super) struct PtrLenPlan {
    pub(super) item_index: usize,
    pub(super) function_name: String,
    pub(super) ptr_index: usize,
    pub(super) ptr_name: String,
    pub(super) mutable: bool,
    pub(super) elem: Type,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub(in crate::fixups) enum Phase {
    Early,
    Late,
}

#[derive(Debug, Clone)]
pub(in crate::fixups) struct BufferPointerField {
    pub(in crate::fixups) buffer: BindingRef,
    pub(in crate::fixups) array: BindingRef,
    pub(in crate::fixups) assignment: StatementRef,
    pub(in crate::fixups) field: String,
    pub(in crate::fixups) index: usize,
    pub(in crate::fixups) array_len: usize,
}

#[derive(Debug, Clone)]
pub(in crate::fixups) struct BufferPointerFields {
    pub(in crate::fixups) fields: Vec<BufferPointerField>,
}
