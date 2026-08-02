use crate::fixups::facts::{
    CountedLoopIndexUse, CountedLoopStart, CountedLoopStep, PlaceAccess, Purity,
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
    AnonymousStructDomain,
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
    ItemGuard,
    LazySingletonDomain,
    ReadPath,
    ArrayElementPointerOrigin,
    BufferCursor,
    HeapOwnershipPlan,
    PtrLenSlice,
    ValueGuard,
    StringBuffer,
    StringUse,
    AllExprs,
    Cast,
    UnusedParam,
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
    LazySingletonDomain {
        singletons: usize,
    },
    ArrayElementPointerOrigin {
        origins: usize,
    },
    BufferCursor {
        arrays: usize,
        buffers: usize,
    },
    HeapOwnershipPlan {
        plans: usize,
    },
    PtrLenSlice {
        plans: usize,
    },
    StringBuffer {
        bytes: usize,
    },
    StringUse {
        allowed: bool,
    },
    AllExprs {
        count: usize,
    },
    Cast {
        to: Type,
    },
    UnusedParam {
        function: String,
        param: String,
        param_index: usize,
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
