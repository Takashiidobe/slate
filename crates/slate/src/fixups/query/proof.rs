use super::{ByteExtent, ByteRepresentation, CallTarget, ExprSite, NulPosition, PointerMutability};

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
    ByteSource,
    ConstantU8,
    ConstantUsize,
    FullByteView,
    FirstNul,
    PrefixContains,
    MovablePure,
    ZeroUsers,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) enum EvidenceDetail {
    IndexedCall {
        target: CallTarget,
        arity: usize,
    },
    Binding {
        name: String,
    },
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
    UseDomain {
        name: String,
        users: usize,
        complete: bool,
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
