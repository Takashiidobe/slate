use std::collections::BTreeSet;
use std::marker::PhantomData;

use crate::backend::facts::{
    AstPath, BindingId, ConstValue, CountedLoopBound, CountedLoopIndexUse, CountedLoopStart,
    CountedLoopStep, EffectKind, FileOpenMode, FileUseKind, HeapAllocationKind, HeapExtent,
    HeapInitKind, HeapReadSafety, HeapResizeKind, HeapUseKind, PlaceAccess, PlaceKind, Purity,
    SliceLoopAccess, StringLibcFunction,
};
use crate::backend::rust_ast::{AtomicType, Expr, IndentStmt, Pattern, Type};

use super::item::StatementRef;

#[derive(Debug, Clone, PartialEq, Eq, salsa::SalsaValue)]
pub(in crate::backend) struct AtomicPromotionSet {
    pub(super) locals: Vec<AtomicLocalPromotion>,
    pub(super) globals: Vec<AtomicGlobalPromotion>,
}

#[derive(Debug, Clone, PartialEq, Eq, salsa::SalsaValue)]
pub(super) struct AtomicLocalPromotion {
    pub(super) function_item_index: usize,
    pub(super) name: String,
    pub(super) ty: AtomicType,
}

#[derive(Debug, Clone, PartialEq, Eq, salsa::SalsaValue)]
pub(super) struct AtomicGlobalPromotion {
    pub(super) name: String,
    pub(super) ty: AtomicType,
}

#[derive(Debug, Clone)]
pub(in crate::backend) struct AtomicCompareExchangeChain {
    pub(super) compare_exchange: Expr,
    pub(super) expected_name: String,
    pub(super) final_name: String,
    pub(super) mutable: bool,
    pub(super) ty: Option<Type>,
    pub(super) needs_cast: bool,
    pub(super) depth: usize,
}

#[derive(Debug, Clone)]
pub(in crate::backend) struct DispatchRegion {
    pub(in crate::backend) state_declaration: StatementRef,
    pub(in crate::backend) dispatch_loop: StatementRef,
    pub(super) depth: usize,
    pub(super) dispatch: crate::backend::facts::goto::DispatchLoop,
}

#[derive(Debug, Clone)]
pub(in crate::backend) struct SwitchDispatch {
    pub(super) selector: Expr,
    pub(super) cases: Vec<SwitchCase>,
}

#[derive(Debug, Clone)]
pub(in crate::backend) struct VaListAlias {
    pub(super) param_index: usize,
    pub(super) local_name: String,
    pub(super) local_decl: AstPath,
    pub(super) clone_assign: AstPath,
}

#[derive(Debug, Clone)]
pub(in crate::backend) struct SwitchCase {
    pub(super) patterns: Vec<Pattern>,
    pub(super) is_default: bool,
    pub(super) body: Vec<IndentStmt>,
    pub(super) falls_through: bool,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(in crate::backend) struct Usage {
    pub(in crate::backend) reads: usize,
    pub(in crate::backend) writes: usize,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::backend) struct ResolvedValue {
    pub(in crate::backend) ty: Option<Type>,
    pub(in crate::backend) usage: Option<Usage>,
    pub(in crate::backend) purity: Option<Purity>,
}

#[derive(Debug, Clone, PartialEq, Eq, salsa::SalsaValue)]
pub(in crate::backend) struct BindingRef<'db> {
    pub(in crate::backend) item_index: usize,
    pub(in crate::backend) function_name: String,
    pub(in crate::backend) name: String,
    pub(in crate::backend) definition: AstPath,
    pub(in crate::backend) kind: BindingCategory,
    pub(in crate::backend) ty: Option<Type>,
    pub(super) id: BindingId<'db>,
}

impl<'db> BindingRef<'db> {
    pub(in crate::backend) fn value_site(&self) -> ValueSite {
        ValueSite {
            item_index: self.item_index,
            path: self.definition.clone(),
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, salsa::SalsaValue)]
pub(in crate::backend) enum BindingCategory {
    Parameter { index: usize },
    Local,
}

#[derive(Debug, Clone, PartialEq, Eq, salsa::SalsaValue)]
pub(in crate::backend) struct FunctionRef<'db> {
    pub(in crate::backend) item_index: usize,
    pub(in crate::backend) name: String,
    pub(super) id: crate::backend::facts::FunctionId<'db>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::backend) struct ProgramRef {
    pub(in crate::backend) expected_len: usize,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::backend) struct ParameterRef<'db> {
    pub(in crate::backend) binding: BindingRef<'db>,
    pub(in crate::backend) index: usize,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::backend) struct FunctionReachability {
    pub(in crate::backend) externally_reachable: bool,
    pub(in crate::backend) address_exposed: bool,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::backend) struct FunctionCallDomain<'db> {
    pub(in crate::backend) function: FunctionRef<'db>,
    pub(in crate::backend) calls: Vec<super::CallRecord>,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(in crate::backend) struct StatementContainerRef {
    pub(in crate::backend) item_index: usize,
    pub(in crate::backend) path: AstPath,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(in crate::backend) struct MatchArmRef {
    pub(in crate::backend) statement: super::item::StatementRef,
    pub(in crate::backend) index: usize,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(in crate::backend) struct FieldRef {
    pub(in crate::backend) item_index: usize,
    pub(in crate::backend) index: usize,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(in crate::backend) struct EnumVariantRef {
    pub(in crate::backend) item_index: usize,
    pub(in crate::backend) index: usize,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::backend) enum TypeUseRef<'db> {
    FunctionReturn(FunctionRef<'db>),
    Parameter(ParameterRef<'db>),
    Field(FieldRef),
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[expect(
    dead_code,
    reason = "query API surface not yet wired into a fixup rule"
)]
pub(in crate::backend) enum TypeUseKind {
    FunctionReturn,
    Parameter,
    Field,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::backend) struct BindingDefUse<'db> {
    pub(in crate::backend) binding: BindingRef<'db>,
    pub(in crate::backend) reads: Vec<UseSiteRef>,
    pub(in crate::backend) writes: Vec<UseSiteRef>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::backend) enum UseSiteRef {
    Expression(ExpressionRef),
    Statement(super::item::StatementRef),
}

impl UseSiteRef {
    pub(in crate::backend) fn path(&self) -> &AstPath {
        match self {
            UseSiteRef::Expression(expression) => &expression.site.path,
            UseSiteRef::Statement(statement) => &statement.path,
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::backend) struct ExpressionRef {
    pub(in crate::backend) site: ExprSite,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub(in crate::backend) enum ExpressionRole {
    AssignmentTarget,
    AssignmentValue,
    Call,
    CallArgument(usize),
    CallCallee,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(in crate::backend) enum ExpressionKind {
    Call,
    Cast,
    Field,
    Index,
    Literal,
    Variable,
    Other,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::backend) struct ExpressionEffects {
    pub(in crate::backend) purity: Purity,
    pub(in crate::backend) effects: BTreeSet<EffectKind>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::backend) struct ExpressionPlace {
    pub(in crate::backend) access: PlaceAccess,
    pub(in crate::backend) kind: PlaceKind,
    pub(in crate::backend) readable: bool,
    pub(in crate::backend) assignable: bool,
    pub(in crate::backend) ordinary_slot: bool,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(in crate::backend) enum BindingAccess {
    Read,
    Write,
    ReadWrite,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::backend) struct BindingUse {
    pub(in crate::backend) site: UseSiteRef,
    pub(in crate::backend) access: BindingAccess,
}

impl BindingUse {
    pub(in crate::backend) fn expression(&self) -> Option<&ExpressionRef> {
        match &self.site {
            UseSiteRef::Expression(expression) => Some(expression),
            UseSiteRef::Statement(_) => None,
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::backend) struct BindingUses<'db> {
    pub(in crate::backend) binding: BindingRef<'db>,
    pub(in crate::backend) uses: Vec<BindingUse>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::backend) struct ExpressionValues {
    pub(in crate::backend) values: Vec<ConstValue>,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(in crate::backend) struct ExprSite {
    pub(in crate::backend) item_index: usize,
    pub(in crate::backend) path: AstPath,
    pub(super) fact_path: AstPath,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash, salsa::SalsaValue)]
pub(in crate::backend) enum DefinitionLocation {
    Item(usize),
    ExternDecl {
        item_index: usize,
        decl_index: usize,
    },
}

impl DefinitionLocation {
    pub(in crate::backend) fn item_index(&self) -> usize {
        match self {
            Self::Item(item_index) => *item_index,
            Self::ExternDecl { item_index, .. } => *item_index,
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(in crate::backend) enum DefinitionKind {
    Function,
    ExternFunction,
    ExternStatic,
    SupportModule,
    Struct,
    Record,
    Enum,
    Static,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(in crate::backend) enum DefinitionGroup {
    Header(String),
    SupportModule(String),
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(in crate::backend) struct DefinitionSelector {
    pub(in crate::backend) kind: DefinitionKind,
    pub(in crate::backend) name: String,
}

#[derive(Debug, Clone, PartialEq, Eq, Hash, salsa::SalsaValue)]
pub(in crate::backend) struct DefinitionSite {
    pub(in crate::backend) location: DefinitionLocation,
    pub(in crate::backend) kind: DefinitionKind,
    pub(in crate::backend) name: String,
    pub(in crate::backend) symbols: Vec<String>,
    pub(in crate::backend) group: Option<DefinitionGroup>,
    pub(super) externally_reachable: bool,
}

#[derive(Debug, Clone, PartialEq, Eq, salsa::SalsaValue)]
pub(in crate::backend) struct DefinitionUsers {
    pub(in crate::backend) definition: DefinitionSite,
    pub(in crate::backend) users: usize,
    pub(in crate::backend) site: ExprSite,
}

#[derive(Debug, Clone, PartialEq, Eq, salsa::SalsaValue)]
pub(in crate::backend) struct DefinitionGroupUsers {
    pub(in crate::backend) group: DefinitionGroup,
    pub(in crate::backend) users: usize,
    pub(in crate::backend) site: ExprSite,
}

#[derive(Debug, Clone, PartialEq, Eq, salsa::SalsaValue)]
pub(in crate::backend) struct ItemReferences {
    pub(in crate::backend) item_index: usize,
    pub(in crate::backend) symbols: BTreeSet<String>,
}

#[derive(Debug, Clone, PartialEq, Eq, salsa::SalsaValue)]
pub(in crate::backend) struct ReferenceDomain {
    pub(in crate::backend) definitions: Vec<DefinitionSite>,
    pub(in crate::backend) items: Vec<ItemReferences>,
}

#[derive(Debug, Clone, PartialEq, Eq, salsa::SalsaValue)]
pub(in crate::backend) struct AnonymousStructSet {
    pub(super) structs: Vec<AnonymousStructPlan>,
}

#[derive(Debug, Clone, PartialEq, Eq, salsa::SalsaValue)]
pub(super) struct AnonymousStructPlan {
    pub(super) item_index: usize,
    pub(super) original_name: String,
    pub(super) generated_name: String,
    pub(super) fields: Vec<AnonymousStructField>,
}

#[derive(Debug, Clone, PartialEq, Eq, salsa::SalsaValue)]
pub(super) struct AnonymousStructField {
    pub(super) name: String,
    pub(super) ty: Type,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(in crate::backend) enum ByteRepresentation {
    Collection,
    Bytes,
    CStr,
    Str,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(in crate::backend) enum PointerMutability {
    Const,
    Mut,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(in crate::backend) enum ByteExtent {
    Constant(usize),
    Dynamic,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub(in crate::backend) enum NulPosition {
    Constant(usize),
    ByteLength,
}

#[derive(Debug, Clone, PartialEq, Eq, salsa::SalsaValue)]
pub(in crate::backend) struct ByteSource<'snapshot> {
    pub(in crate::backend) site: ExprSite,
    pub(in crate::backend) name: String,
    pub(in crate::backend) representation: ByteRepresentation,
    pub(in crate::backend) mutability: PointerMutability,
    pub(in crate::backend) extent: ByteExtent,
    pub(super) binding: BindingId<'snapshot>,
    pub(super) snapshot: PhantomData<&'snapshot ()>,
}

#[derive(Debug, Clone, PartialEq, Eq, salsa::SalsaValue)]
pub(in crate::backend) struct ByteView<'snapshot> {
    pub(in crate::backend) source: ByteSource<'snapshot>,
    pub(in crate::backend) extent: ByteExtent,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::backend) struct StableExpr {
    pub(in crate::backend) site: ExprSite,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(in crate::backend) struct StatementRange {
    pub(in crate::backend) item_index: usize,
    pub(in crate::backend) path: AstPath,
    pub(in crate::backend) start: usize,
    pub(in crate::backend) end: usize,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(in crate::backend) struct ValueSite {
    pub(in crate::backend) item_index: usize,
    pub(in crate::backend) path: AstPath,
}

#[derive(Debug, Clone, PartialEq, Eq, salsa::SalsaValue)]
pub(in crate::backend) struct LazySingletonSet {
    pub(super) singletons: Vec<LazySingletonPlan>,
}

#[derive(Debug, Clone, PartialEq, Eq, salsa::SalsaValue)]
pub(super) struct LazySingletonPlan {
    pub(super) function_item_index: usize,
    pub(super) payload_item_index: usize,
    pub(super) payload_name: String,
    pub(super) payload_ty: Type,
    pub(super) init_expr: Expr,
    pub(super) flag_item_index: usize,
    pub(super) flag_name: String,
}

#[derive(Debug, Clone, PartialEq, Eq, salsa::SalsaValue)]
pub(in crate::backend) struct HeapOwnershipFacts<'db> {
    pub(in crate::backend) owners: Vec<HeapOwnership<'db>>,
}

#[derive(Debug, Clone)]
pub(in crate::backend) struct ArrayElementPointerOrigin {
    pub(in crate::backend) pointer_name: String,
    pub(in crate::backend) base_name: String,
    pub(in crate::backend) index: Expr,
}

#[derive(Debug, Clone, PartialEq, Eq, salsa::SalsaValue)]
pub(in crate::backend) struct HeapOwnership<'db> {
    pub(in crate::backend) pointer: BindingRef<'db>,
    pub(in crate::backend) allocation_temp: BindingRef<'db>,
    pub(in crate::backend) size_temp: Option<BindingRef<'db>>,
    pub(in crate::backend) free_temp: Option<BindingRef<'db>>,
    pub(in crate::backend) aliases: Vec<BindingRef<'db>>,
    pub(in crate::backend) pointer_statement: StatementRef,
    pub(in crate::backend) allocation_statement: StatementRef,
    pub(in crate::backend) assignment_statement: StatementRef,
    pub(in crate::backend) free_statement: StatementRef,
    pub(in crate::backend) elem_ty: Type,
    pub(in crate::backend) allocation: HeapAllocationKind,
    pub(in crate::backend) extent: HeapExtent,
    pub(in crate::backend) init: HeapInitKind,
    pub(in crate::backend) read_safety: HeapReadSafety,
    pub(in crate::backend) uses: Vec<HeapUse>,
    pub(in crate::backend) reallocations: Vec<HeapReallocation<'db>>,
}

#[derive(Debug, Clone, PartialEq, Eq, salsa::SalsaValue)]
pub(in crate::backend) struct HeapUse {
    pub(in crate::backend) statement: StatementRef,
    pub(in crate::backend) kind: HeapUseKind,
}

#[derive(Debug, Clone, PartialEq, Eq, salsa::SalsaValue)]
pub(in crate::backend) struct HeapReallocation<'db> {
    pub(in crate::backend) source_temp: Option<BindingRef<'db>>,
    pub(in crate::backend) allocation_temp: BindingRef<'db>,
    pub(in crate::backend) size_temp: Option<BindingRef<'db>>,
    pub(in crate::backend) allocation_statement: StatementRef,
    pub(in crate::backend) assignment_statement: StatementRef,
    pub(in crate::backend) new_extent: HeapExtent,
    pub(in crate::backend) init: HeapInitKind,
    pub(in crate::backend) resize: HeapResizeKind,
}

#[derive(Debug, Clone, PartialEq, Eq, salsa::SalsaValue)]
pub(in crate::backend) struct PtrLenPlanSet {
    pub(super) plans: Vec<PtrLenPlan>,
}

#[derive(Debug, Clone, PartialEq, Eq, salsa::SalsaValue)]
pub(super) struct PtrLenPlan {
    pub(super) item_index: usize,
    pub(super) function_name: String,
    pub(super) ptr_index: usize,
    pub(super) ptr_name: String,
    pub(super) mutable: bool,
    pub(super) elem: Type,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub(in crate::backend) enum Phase {
    Early,
    Late,
}

#[derive(Debug, Clone, PartialEq, Eq, salsa::SalsaValue)]
pub(in crate::backend) struct BufferPointerField<'db> {
    pub(in crate::backend) buffer: BindingRef<'db>,
    pub(in crate::backend) array: BindingRef<'db>,
    pub(in crate::backend) assignment: StatementRef,
    pub(in crate::backend) array_len: usize,
}

#[derive(Debug, Clone, PartialEq, Eq, salsa::SalsaValue)]
pub(in crate::backend) struct BufferPointerFields<'db> {
    pub(in crate::backend) fields: Vec<BufferPointerField<'db>>,
}

#[derive(Debug, Clone, PartialEq, Eq, salsa::SalsaValue)]
pub(in crate::backend) struct FileOwnershipFacts<'db> {
    pub(in crate::backend) owners: Vec<FileOwnership<'db>>,
}

#[derive(Debug, Clone, PartialEq, Eq, salsa::SalsaValue)]
pub(in crate::backend) struct FileOwnership<'db> {
    pub(in crate::backend) handle: BindingRef<'db>,
    pub(in crate::backend) handle_statement: StatementRef,
    pub(in crate::backend) open_statement: StatementRef,
    pub(in crate::backend) assign_statement: StatementRef,
    pub(in crate::backend) mode: Option<FileOpenMode>,
    pub(in crate::backend) uses: Vec<FileUse>,
}

#[derive(Debug, Clone, PartialEq, Eq, salsa::SalsaValue)]
pub(in crate::backend) struct FileUse {
    pub(in crate::backend) statement: StatementRef,
    pub(in crate::backend) kind: FileUseKind,
}

#[derive(Debug, Clone, PartialEq, Eq, salsa::SalsaValue)]
pub(in crate::backend) struct SliceLoopFact<'db> {
    pub(in crate::backend) index: BindingRef<'db>,
    pub(in crate::backend) slice: BindingRef<'db>,
    pub(in crate::backend) start: CountedLoopStart,
    pub(in crate::backend) bound: CountedLoopBound,
    pub(in crate::backend) step: CountedLoopStep,
    pub(in crate::backend) index_use: CountedLoopIndexUse,
    pub(in crate::backend) access: SliceLoopAccess,
}

#[derive(Debug, Clone)]
pub(in crate::backend) struct StringCopySite<'db> {
    pub(in crate::backend) statement: StatementRef,
    pub(in crate::backend) action: StringCopyAction<'db>,
}

#[derive(Debug, Clone)]
pub(in crate::backend) enum StringCopyAction<'db> {
    AssignLiteral(String),
    AssignOwned(BindingRef<'db>),
    PushLiteral(String),
    PushOwned(BindingRef<'db>),
}

#[derive(Debug, Clone)]
pub(in crate::backend) struct StringLibcUse<'db> {
    pub(in crate::backend) callee: StringLibcFunction,
    pub(in crate::backend) pointer_args: Vec<BindingRef<'db>>,
}
