use super::{
    ByteExtent, ByteRepresentation, CallTarget, Evidence, EvidenceDetail, NulPosition,
    PointerMutability, Predicate, Rejection, RejectionReason,
};
use crate::fixups::trace::{Pass, TraceFact, fact};

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct RuleIdentity {
    pub(in crate::fixups) pass: Pass,
    pub(in crate::fixups) name: String,
}

impl RuleIdentity {
    pub(in crate::fixups) fn new(pass: Pass, name: impl Into<String>) -> Self {
        Self {
            pass,
            name: name.into(),
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct RuleCaseIdentity {
    pub(in crate::fixups) rule: RuleIdentity,
    pub(in crate::fixups) case: String,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct CaseRejection {
    pub(in crate::fixups) case: String,
    pub(in crate::fixups) rejection: Rejection,
}

pub(super) fn evidence_trace_fact(evidence: &Evidence) -> TraceFact {
    fact(
        format!("evidence.{}", predicate_name(evidence.predicate)),
        evidence_detail(&evidence.detail),
    )
}

pub(super) fn predicate_name(predicate: Predicate) -> &'static str {
    match predicate {
        Predicate::Call => "call",
        Predicate::Binding => "binding",
        Predicate::BindingUses => "binding_uses",
        Predicate::DefUse => "def_use",
        Predicate::Expression => "expression",
        Predicate::ExpressionDependencies => "expression_dependencies",
        Predicate::ExpressionEffects => "expression_effects",
        Predicate::ExpressionPlace => "expression_place",
        Predicate::ExpressionType => "expression_type",
        Predicate::ExpressionValues => "expression_values",
        Predicate::Function => "function",
        Predicate::ParentExpression => "parent_expression",
        Predicate::AnonymousStructDomain => "anonymous_struct_domain",
        Predicate::ByteSource => "byte_source",
        Predicate::ConstantU8 => "constant_u8",
        Predicate::ConstantUsize => "constant_usize",
        Predicate::FullByteView => "full_byte_view",
        Predicate::FirstNul => "first_nul",
        Predicate::PrefixContains => "prefix_contains",
        Predicate::MovablePure => "movable_pure",
        Predicate::ExternFn => "extern_fn",
        Predicate::ZeroUsers => "zero_users",
        Predicate::ZeroGroupUsers => "zero_group_users",
        Predicate::CountedLoop => "counted_loop",
        Predicate::ItemGuard => "item_guard",
        Predicate::LazySingletonDomain => "lazy_singleton_domain",
        Predicate::AtomicPromotionDomain => "atomic_promotion_domain",
        Predicate::CStringLiteral => "c_string_literal",
        Predicate::ArgumentPosition => "argument_position",
        Predicate::Callsite => "callsite",
        Predicate::CallArgumentPinning => "call_argument_pinning",
        Predicate::PrintfCall => "printf_call",
        Predicate::SwitchDispatch => "switch_dispatch",
        Predicate::VaListAlias => "va_list_alias",
        Predicate::ReadPath => "read_path",
        Predicate::ArrayElementPointerOrigin => "array_element_pointer_origin",
        Predicate::BufferPointerFields => "buffer_pointer_fields",
        Predicate::HeapOwnershipFacts => "heap_ownership_facts",
        Predicate::PtrLenSlice => "ptr_len_slice",
        Predicate::ValueGuard => "value_guard",
        Predicate::StringBuffer => "string_buffer",
        Predicate::StringUse => "string_use",
        Predicate::AllExprs => "all_exprs",
        Predicate::Cast => "cast",
        Predicate::ReferenceDomain => "reference_domain",
        Predicate::Statement => "statement",
        Predicate::StatementRegion => "statement_region",
        Predicate::DirectCalls => "direct_calls",
        Predicate::FunctionReachability => "function_reachability",
        Predicate::FunctionCallDomain => "function_call_domain",
        Predicate::CallArgument => "call_argument",
    }
}

pub(super) fn rejection_name(rejection: RejectionReason) -> &'static str {
    match rejection {
        RejectionReason::MissingEvidence => "missing_evidence",
        RejectionReason::Contradicted => "contradicted",
        RejectionReason::UnsupportedShape => "unsupported_shape",
        RejectionReason::Ambiguous => "ambiguous",
        RejectionReason::OutOfRange => "out_of_range",
        RejectionReason::IncompleteDomain => "incomplete_domain",
    }
}

fn evidence_detail(detail: &EvidenceDetail) -> String {
    match detail {
        EvidenceDetail::IndexedCall { target, arity } => {
            format!("{};arity={arity}", call_target_name(target))
        }
        EvidenceDetail::AnonymousStructDomain {
            records,
            facts,
            conflicts,
            complete,
        } => format!("records={records};facts={facts};conflicts={conflicts};complete={complete}"),
        EvidenceDetail::Binding { name } => format!("binding={name}"),
        EvidenceDetail::BindingUses { reads, writes } => {
            format!("reads={reads},writes={writes}")
        }
        EvidenceDetail::DefUse { reads, writes } => {
            format!("reads={reads},writes={writes}")
        }
        EvidenceDetail::Expression => "resolved=true".into(),
        EvidenceDetail::ExpressionDependencies { count } => format!("count={count}"),
        EvidenceDetail::Statement => "resolved=true".into(),
        EvidenceDetail::ExpressionEffects { purity, effects } => {
            format!("purity={purity:?};effects={effects}")
        }
        EvidenceDetail::ExpressionPlace {
            access,
            ordinary_slot,
        } => format!("access={access:?};ordinary_slot={ordinary_slot}"),
        EvidenceDetail::ExpressionType { ty } => format!("type={ty:?}"),
        EvidenceDetail::ExpressionValues { count } => format!("count={count}"),
        EvidenceDetail::Function { name } => format!("function={name}"),
        EvidenceDetail::ParentExpression => "resolved=true".into(),
        EvidenceDetail::PointerView {
            representation,
            mutability,
        } => format!(
            "representation={};mutability={}",
            representation_name(*representation),
            mutability_name(*mutability)
        ),
        EvidenceDetail::Extent(extent) => extent_name(*extent),
        EvidenceDetail::SourceLength => "source_length".into(),
        EvidenceDetail::ConstantU8(value) => value.to_string(),
        EvidenceDetail::ConstantUsize(value) => value.to_string(),
        EvidenceDetail::NulPosition(position) => nul_position_name(*position),
        EvidenceDetail::PrefixContains { count, nul } => {
            format!("count={count};nul={nul}")
        }
        EvidenceDetail::MovablePure => "movable_pure".into(),
        EvidenceDetail::ExternFnDeclaration {
            name,
            arity,
            returns_never,
        } => format!("name={name};arity={arity};returns_never={returns_never}"),
        EvidenceDetail::UseDomain {
            name,
            users,
            complete,
        } => format!("name={name};users={users};complete={complete}"),
        EvidenceDetail::GroupUseDomain {
            group,
            definitions,
            users,
            complete,
        } => format!("group={group:?};definitions={definitions};users={users};complete={complete}"),
        EvidenceDetail::CountedLoop {
            start,
            step,
            index_use,
        } => format!("start={start:?};step={step:?};index_use={index_use:?}"),
        EvidenceDetail::LazySingletonDomain { singletons } => {
            format!("singletons={singletons}")
        }
        EvidenceDetail::AtomicPromotionDomain { locals, globals } => {
            format!("locals={locals};globals={globals}")
        }
        EvidenceDetail::CStringLiteral { bytes } => format!("bytes={bytes}"),
        EvidenceDetail::ArgumentPosition { slot } => format!("slot={slot}"),
        EvidenceDetail::Callsite { direct } => format!("direct={direct}"),
        EvidenceDetail::CallArgumentPinning { pinning, variadic } => {
            format!("pinning={pinning:?};variadic={variadic}")
        }
        EvidenceDetail::PrintfCall { args, known_format } => {
            format!("args={args};known_format={known_format}")
        }
        EvidenceDetail::SwitchDispatch { cases } => format!("cases={cases}"),
        EvidenceDetail::VaListAlias { param_index } => format!("param_index={param_index}"),
        EvidenceDetail::ArrayElementPointerOrigin { origins } => {
            format!("origins={origins}")
        }
        EvidenceDetail::BufferPointerFields { fields } => format!("fields={fields}"),
        EvidenceDetail::HeapOwnershipFacts { owners } => format!("owners={owners}"),
        EvidenceDetail::PtrLenSlice { plans } => format!("plans={plans}"),
        EvidenceDetail::StringBuffer { bytes } => format!("bytes={bytes}"),
        EvidenceDetail::StringUse { allowed } => format!("allowed={allowed}"),
        EvidenceDetail::AllExprs { count } => format!("count={count}"),
        EvidenceDetail::Cast { to } => format!("to={to:?}"),
        EvidenceDetail::ReferenceDomain { definitions, items } => {
            format!("definitions={definitions}, items={items}")
        }
        EvidenceDetail::StatementRegion { statements } => {
            format!("statements={statements}")
        }
        EvidenceDetail::DirectCalls {
            function,
            calls,
            references,
        } => format!("function={function};calls={calls};references={references}"),
        EvidenceDetail::FunctionReachability {
            function,
            externally_reachable,
            address_exposed,
        } => format!(
            "function={function};externally_reachable={externally_reachable};address_exposed={address_exposed}"
        ),
        EvidenceDetail::FunctionCallDomain { function, calls } => {
            format!("function={function};calls={calls}")
        }
        EvidenceDetail::CallArgument { index } => format!("index={index}"),
    }
}

fn call_target_name(target: &CallTarget) -> String {
    match target {
        CallTarget::Known(known) => format!("known:{}", known.symbol()),
        CallTarget::Generated(name) => format!("generated:{name}"),
        CallTarget::Direct(name) => format!("direct:{name}"),
        CallTarget::Indirect => "indirect".into(),
    }
}

fn representation_name(representation: ByteRepresentation) -> &'static str {
    match representation {
        ByteRepresentation::Collection => "collection",
        ByteRepresentation::Bytes => "bytes",
        ByteRepresentation::CStr => "c_str",
        ByteRepresentation::Str => "str",
    }
}

fn mutability_name(mutability: PointerMutability) -> &'static str {
    match mutability {
        PointerMutability::Const => "const",
        PointerMutability::Mut => "mut",
    }
}

fn extent_name(extent: ByteExtent) -> String {
    match extent {
        ByteExtent::Constant(extent) => extent.to_string(),
        ByteExtent::Dynamic => "dynamic".into(),
    }
}

fn nul_position_name(position: NulPosition) -> String {
    match position {
        NulPosition::Constant(position) => position.to_string(),
        NulPosition::ByteLength => "byte_length".into(),
    }
}
