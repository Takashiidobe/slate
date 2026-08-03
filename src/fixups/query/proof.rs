use crate::fixups::facts::{
    AsciiNumericSign, CallArgPinning, CountedLoopIndexUse, CountedLoopStart, CountedLoopStep,
    PlaceAccess, Purity, SliceLoopAccess, StringLibcFunction,
};
use crate::rust_ast::Type;

use super::{
    ByteExtent, ByteRepresentation, CallTarget, DefinitionGroup, ExprSite, NulPosition,
    PointerMutability,
};

pub(in crate::fixups) type QueryResult<T> = Result<Proof<T>, Rejection>;

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct Proof<T> {
    pub(in crate::fixups) value: T,
    pub(in crate::fixups) evidence: Vec<Evidence>,
}

impl<T> Proof<T> {
    pub(super) fn new(value: T, evidence: Vec<Evidence>) -> Self {
        Self { value, evidence }
    }

    pub(in crate::fixups) fn into_parts(self) -> (T, Vec<Evidence>) {
        (self.value, self.evidence)
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct Evidence {
    pub(in crate::fixups) predicate: Predicate,
    pub(in crate::fixups) site: ExprSite,
    pub(in crate::fixups) detail: EvidenceDetail,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(in crate::fixups) enum Predicate {
    Call,
    Binding,
    BindingUses,
    DefUse,
    Expression,
    ExpressionDependencies,
    ExpressionEffects,
    ExpressionPlace,
    ExpressionType,
    ExpressionValues,
    Function,
    ParentExpression,
    ReferenceDomain,
    Statement,
    StatementRegion,
    StatementReachable,
    AnonymousStructDomain,
    SortSearchDomain,
    ByteSource,
    ConstantU8,
    ConstantUsize,
    FullByteView,
    FirstNul,
    PrefixContains,
    MovablePure,
    ExternFn,
    ZeroUsers,
    ZeroGroupUsers,
    CountedLoop,
    CountedSliceLoop,
    ItemGuard,
    LazySingletonDomain,
    AtomicPromotionDomain,
    CStringLiteral,
    ArgumentPosition,
    Callsite,
    CallArgumentPinning,
    PrintfCall,
    SwitchDispatch,
    VaListAlias,
    BindingRequiresMut,
    BorrowAliasReasons,
    ReadPath,
    ArrayElementPointerOrigin,
    BufferPointerFields,
    HeapOwnershipFacts,
    FileOwnershipFacts,
    PtrLenSlice,
    StringParamLift,
    ValueGuard,
    StringBuffer,
    StringUse,
    StringLibcUse,
    AsciiNumericSign,
    AllExprs,
    Cast,
    DirectCalls,
    FunctionReachability,
    FunctionCallDomain,
    CallArgument,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) enum EvidenceDetail {
    IndexedCall {
        target: CallTarget,
        arity: usize,
    },
    AnonymousStructDomain {
        records: usize,
        facts: usize,
        conflicts: usize,
        complete: bool,
    },
    SortSearchDomain {
        comparators: usize,
    },
    Binding {
        name: String,
    },
    BindingUses {
        reads: usize,
        writes: usize,
    },
    DefUse {
        reads: usize,
        writes: usize,
    },
    Expression,
    ExpressionDependencies {
        count: usize,
    },
    Statement,
    StatementRegion {
        statements: usize,
    },
    StatementReachable {
        reachable: bool,
    },
    ExpressionEffects {
        purity: Purity,
        effects: usize,
    },
    ExpressionPlace {
        access: PlaceAccess,
        ordinary_slot: bool,
    },
    ExpressionType {
        ty: Type,
    },
    ExpressionValues {
        count: usize,
    },
    Function {
        name: String,
    },
    ReferenceDomain {
        definitions: usize,
        items: usize,
    },
    ParentExpression,
    PointerView {
        representation: ByteRepresentation,
        mutability: PointerMutability,
    },
    Extent(ByteExtent),
    SourceLength,
    ConstantU8(u8),
    ConstantUsize(usize),
    NulPosition(NulPosition),
    PrefixContains {
        count: usize,
        nul: usize,
    },
    MovablePure,
    ValueGuard,
    ExternFnDeclaration {
        name: String,
        arity: usize,
        returns_never: bool,
    },
    UseDomain {
        name: String,
        users: usize,
        complete: bool,
    },
    GroupUseDomain {
        group: DefinitionGroup,
        definitions: usize,
        users: usize,
        complete: bool,
    },
    CountedLoop {
        start: CountedLoopStart,
        step: CountedLoopStep,
        index_use: CountedLoopIndexUse,
    },
    CountedSliceLoop {
        index_use: CountedLoopIndexUse,
        access: SliceLoopAccess,
    },
    LazySingletonDomain {
        singletons: usize,
    },
    AtomicPromotionDomain {
        locals: usize,
        globals: usize,
    },
    CStringLiteral {
        bytes: usize,
    },
    ArgumentPosition {
        slot: usize,
    },
    Callsite {
        direct: bool,
    },
    CallArgumentPinning {
        pinning: CallArgPinning,
        variadic: bool,
    },
    PrintfCall {
        args: usize,
        known_format: bool,
    },
    SwitchDispatch {
        cases: usize,
    },
    BindingRequiresMut {
        required: bool,
    },
    BorrowAliasReasons {
        tracked: bool,
        reasons: usize,
    },
    VaListAlias {
        param_index: usize,
    },
    ArrayElementPointerOrigin {
        origins: usize,
    },
    BufferPointerFields {
        fields: usize,
    },
    HeapOwnershipFacts {
        owners: usize,
    },
    FileOwnershipFacts {
        owners: usize,
    },
    PtrLenSlice {
        plans: usize,
    },
    StringParamLift,
    StringBuffer {
        bytes: usize,
    },
    StringUse {
        allowed: bool,
    },
    StringLibcUse {
        callee: StringLibcFunction,
    },
    AsciiNumericSign {
        sign: AsciiNumericSign,
    },
    AllExprs {
        count: usize,
    },
    Cast {
        to: Type,
    },
    DirectCalls {
        function: String,
        calls: usize,
        references: usize,
    },
    FunctionReachability {
        function: String,
        externally_reachable: bool,
        address_exposed: bool,
    },
    FunctionCallDomain {
        function: String,
        calls: usize,
    },
    CallArgument {
        index: usize,
    },
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct Rejection {
    pub(in crate::fixups) predicate: Predicate,
    pub(in crate::fixups) site: Option<ExprSite>,
    pub(in crate::fixups) reason: RejectionReason,
    pub(in crate::fixups) evidence: Vec<Evidence>,
}

impl Rejection {
    pub(in crate::fixups) fn new(
        predicate: Predicate,
        site: Option<ExprSite>,
        reason: RejectionReason,
        evidence: Vec<Evidence>,
    ) -> Self {
        Self {
            predicate,
            site,
            reason,
            evidence,
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(in crate::fixups) enum RejectionReason {
    MissingEvidence,
    Contradicted,
    UnsupportedShape,
    Ambiguous,
    OutOfRange,
    IncompleteDomain,
}
