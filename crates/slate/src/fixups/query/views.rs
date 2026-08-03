use std::collections::BTreeSet;
use std::marker::PhantomData;

use crate::fixups::facts::{
    AstPath, BindingId, ConstValue, CountedLoopBound, CountedLoopIndexUse, CountedLoopStart,
    CountedLoopStep, EffectKind, FileOpenMode, FileUseKind, HeapAllocationKind, HeapExtent,
    HeapInitKind, HeapReadSafety, HeapResizeKind, HeapUseKind, PlaceAccess, PlaceKind, Purity,
    SliceLoopAccess,
};
use crate::rust_ast::{AtomicType, Expr, IndentStmt, Pattern, Type};

use super::item::StatementRef;

#[derive(Debug, Clone)]
pub(in crate::fixups) struct AtomicPromotionSet {
    pub(super) locals: Vec<AtomicLocalPromotion>,
    pub(super) globals: Vec<AtomicGlobalPromotion>,
}

#[derive(Debug, Clone)]
pub(super) struct AtomicLocalPromotion {
    pub(super) function_item_index: usize,
    pub(super) name: String,
    pub(super) ty: AtomicType,
}

#[derive(Debug, Clone)]
pub(super) struct AtomicGlobalPromotion {
    pub(super) name: String,
    pub(super) ty: AtomicType,
}

#[derive(Debug, Clone)]
pub(in crate::fixups) struct AtomicCompareExchangeChain {
    pub(super) compare_exchange: Expr,
    pub(super) expected_name: String,
    pub(super) final_name: String,
    pub(super) mutable: bool,
    pub(super) ty: Option<Type>,
    pub(super) needs_cast: bool,
    pub(super) depth: usize,
}

#[derive(Debug, Clone)]
pub(in crate::fixups) struct DispatchRegion {
    pub(in crate::fixups) state_declaration: StatementRef,
    pub(in crate::fixups) dispatch_loop: StatementRef,
    pub(super) depth: usize,
    pub(super) dispatch: crate::fixups::facts::goto::DispatchLoop,
}

#[derive(Debug, Clone)]
pub(in crate::fixups) struct SwitchDispatch {
    pub(super) selector: Expr,
    pub(super) switch_label: String,
    pub(super) cases: Vec<SwitchCase>,
}

#[derive(Debug, Clone)]
pub(in crate::fixups) struct VaListAlias {
    pub(super) param_index: usize,
    pub(super) local_name: String,
    pub(super) local_decl: AstPath,
    pub(super) clone_assign: AstPath,
}

#[derive(Debug, Clone)]
pub(in crate::fixups) struct SwitchCase {
    pub(super) patterns: Vec<Pattern>,
    pub(super) is_default: bool,
    pub(super) body: Vec<IndentStmt>,
    pub(super) falls_through: bool,
}

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

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct FunctionReachability {
    pub(in crate::fixups) externally_reachable: bool,
    pub(in crate::fixups) address_exposed: bool,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct FunctionCallDomain {
    pub(in crate::fixups) function: FunctionRef,
    pub(in crate::fixups) calls: Vec<super::CallRecord>,
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

impl UseSiteRef {
    pub(in crate::fixups) fn path(&self) -> &AstPath {
        match self {
            UseSiteRef::Expression(expression) => &expression.site.path,
            UseSiteRef::Statement(statement) => &statement.path,
        }
    }
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
pub(in crate::fixups) struct HeapOwnershipFacts {
    pub(in crate::fixups) owners: Vec<HeapOwnership>,
}

#[derive(Debug, Clone)]
pub(in crate::fixups) struct ArrayElementPointerOrigin {
    pub(in crate::fixups) pointer_name: String,
    pub(in crate::fixups) base_name: String,
    pub(in crate::fixups) index: Expr,
}

#[derive(Debug, Clone)]
pub(in crate::fixups) struct HeapOwnership {
    pub(in crate::fixups) pointer: BindingRef,
    pub(in crate::fixups) allocation_temp: BindingRef,
    pub(in crate::fixups) size_temp: Option<BindingRef>,
    pub(in crate::fixups) free_temp: Option<BindingRef>,
    pub(in crate::fixups) aliases: Vec<BindingRef>,
    pub(in crate::fixups) pointer_statement: StatementRef,
    pub(in crate::fixups) allocation_statement: StatementRef,
    pub(in crate::fixups) assignment_statement: StatementRef,
    pub(in crate::fixups) free_statement: StatementRef,
    pub(in crate::fixups) elem_ty: Type,
    pub(in crate::fixups) allocation: HeapAllocationKind,
    pub(in crate::fixups) extent: HeapExtent,
    pub(in crate::fixups) init: HeapInitKind,
    pub(in crate::fixups) read_safety: HeapReadSafety,
    pub(in crate::fixups) uses: Vec<HeapUse>,
    pub(in crate::fixups) reallocations: Vec<HeapReallocation>,
}

#[derive(Debug, Clone)]
pub(in crate::fixups) struct HeapUse {
    pub(in crate::fixups) statement: StatementRef,
    pub(in crate::fixups) kind: HeapUseKind,
}

#[derive(Debug, Clone)]
pub(in crate::fixups) struct HeapReallocation {
    pub(in crate::fixups) source_temp: Option<BindingRef>,
    pub(in crate::fixups) allocation_temp: BindingRef,
    pub(in crate::fixups) size_temp: Option<BindingRef>,
    pub(in crate::fixups) allocation_statement: StatementRef,
    pub(in crate::fixups) assignment_statement: StatementRef,
    pub(in crate::fixups) new_extent: HeapExtent,
    pub(in crate::fixups) init: HeapInitKind,
    pub(in crate::fixups) resize: HeapResizeKind,
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

#[derive(Debug, Clone)]
pub(in crate::fixups) struct FileOwnershipFacts {
    pub(in crate::fixups) owners: Vec<FileOwnership>,
}

#[derive(Debug, Clone)]
pub(in crate::fixups) struct FileOwnership {
    pub(in crate::fixups) handle: BindingRef,
    pub(in crate::fixups) open_temp: BindingRef,
    pub(in crate::fixups) close_temp: Option<BindingRef>,
    pub(in crate::fixups) handle_statement: StatementRef,
    pub(in crate::fixups) open_statement: StatementRef,
    pub(in crate::fixups) assign_statement: StatementRef,
    pub(in crate::fixups) close_statement: StatementRef,
    pub(in crate::fixups) mode: Option<FileOpenMode>,
    pub(in crate::fixups) uses: Vec<FileUse>,
}

#[derive(Debug, Clone)]
pub(in crate::fixups) struct FileUse {
    pub(in crate::fixups) statement: StatementRef,
    pub(in crate::fixups) kind: FileUseKind,
}

#[derive(Debug, Clone)]
pub(in crate::fixups) struct SliceLoopFact {
    pub(in crate::fixups) index: BindingRef,
    pub(in crate::fixups) slice: BindingRef,
    pub(in crate::fixups) start: CountedLoopStart,
    pub(in crate::fixups) bound: CountedLoopBound,
    pub(in crate::fixups) step: CountedLoopStep,
    pub(in crate::fixups) index_use: CountedLoopIndexUse,
    pub(in crate::fixups) access: SliceLoopAccess,
}

#[derive(Debug, Clone)]
pub(in crate::fixups) struct StringCopySite {
    pub(in crate::fixups) statement: StatementRef,
    pub(in crate::fixups) action: StringCopyAction,
}

#[derive(Debug, Clone)]
pub(in crate::fixups) enum StringCopyAction {
    AssignLiteral(String),
    AssignOwned(BindingRef),
    PushLiteral(String),
    PushOwned(BindingRef),
}
