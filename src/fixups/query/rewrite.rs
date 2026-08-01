use std::collections::BTreeMap;

use crate::fixups::facts::{AstPath, FixupFacts};
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass, RewriteEvent, TraceFact, TraceLogger, TraceSnippet, fact, function_path_location,
    path_location,
};
use crate::rust_ast::{Expr, Item, Program};

use super::plan::{
    EditTarget, Plan, PlanBuilder, PlanDiagnostic, PlanSite, PlannedEdit, TouchedItems,
};
use super::{
    ByteExtent, ByteRepresentation, CallTarget, Evidence, EvidenceDetail, ExprSite, NulPosition,
    PointerMutability, Predicate, QueryContext, Rejection, RejectionReason,
};

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

pub(in crate::fixups) struct ReplaceExpr {
    pub(in crate::fixups) target: ExprSite,
    pub(in crate::fixups) replacement: Expr,
    pub(in crate::fixups) evidence: Vec<Evidence>,
}

impl ReplaceExpr {
    pub(in crate::fixups) fn new(target: ExprSite, replacement: Expr) -> Self {
        Self {
            target,
            replacement,
            evidence: Vec::new(),
        }
    }

    pub(in crate::fixups) fn with_evidence(
        mut self,
        evidence: impl IntoIterator<Item = Evidence>,
    ) -> Self {
        self.evidence.extend(evidence);
        self
    }
}

impl EditTarget for ReplaceExpr {
    type Site = ExprSite;

    fn site(&self) -> ExprSite {
        self.target.clone()
    }
}

impl PlanSite for ExprSite {
    fn overlaps(&self, other: &Self) -> bool {
        self.item_index == other.item_index
            && (path_starts_with(&self.path.0, &other.path.0)
                || path_starts_with(&other.path.0, &self.path.0))
    }
}

pub(in crate::fixups) enum RuleResult {
    Replace(ReplaceExpr),
    Reject(Rejection),
}

impl RuleResult {
    pub(in crate::fixups) fn replace(replacement: ReplaceExpr) -> Self {
        Self::Replace(replacement)
    }

    pub(in crate::fixups) fn reject(rejection: Rejection) -> Self {
        Self::Reject(rejection)
    }
}

impl From<Result<ReplaceExpr, Rejection>> for RuleResult {
    fn from(result: Result<ReplaceExpr, Rejection>) -> Self {
        result.map_or_else(Self::Reject, Self::Replace)
    }
}

pub(in crate::fixups) struct RuleCase {
    pub(in crate::fixups) name: String,
    pub(in crate::fixups) result: RuleResult,
}

impl RuleCase {
    pub(in crate::fixups) fn new(name: impl Into<String>, result: RuleResult) -> Self {
        Self {
            name: name.into(),
            result,
        }
    }
}

pub(in crate::fixups) trait ExprRule {
    type Candidate;

    fn identity(&self) -> RuleIdentity;
    fn candidates(&self, query: &QueryContext<'_>) -> Vec<Self::Candidate>;
    fn target(&self, candidate: &Self::Candidate) -> ExprSite;
    fn cases(&self, query: &QueryContext<'_>, candidate: &Self::Candidate) -> Vec<RuleCase>;
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct CaseRejection {
    pub(in crate::fixups) case: String,
    pub(in crate::fixups) rejection: Rejection,
}

pub(in crate::fixups) struct ExprPlanBuilder {
    builder: PlanBuilder<ReplaceExpr>,
}

impl ExprPlanBuilder {
    pub(in crate::fixups) fn new() -> Self {
        Self {
            builder: PlanBuilder::new(),
        }
    }

    pub(in crate::fixups) fn add_rule<R: ExprRule>(
        &mut self,
        query: &QueryContext<'_>,
        rule: &R,
    ) -> &mut Self {
        let identity = rule.identity();
        for candidate in rule.candidates(query) {
            let target = rule.target(&candidate);
            let mut rejected_cases = Vec::new();
            let mut selected = None;
            let mut terminal_diagnostic = false;
            for case in rule.cases(query, &candidate) {
                match case.result {
                    RuleResult::Replace(replacement) => {
                        let contender = RuleCaseIdentity {
                            rule: identity.clone(),
                            case: case.name,
                        };
                        if replacement.target != target {
                            self.builder.diagnose(PlanDiagnostic::TargetMismatch {
                                contender,
                                candidate: target.clone(),
                                replacement: replacement.target,
                            });
                            terminal_diagnostic = true;
                        } else {
                            selected = Some(PlannedEdit {
                                identity: contender,
                                edit: replacement,
                                rejected_cases: std::mem::take(&mut rejected_cases),
                            });
                        }
                        break;
                    }
                    RuleResult::Reject(rejection) => {
                        rejected_cases.push(CaseRejection {
                            case: case.name,
                            rejection,
                        });
                    }
                }
            }
            if let Some(selected) = selected {
                self.builder.propose(selected);
            } else if !terminal_diagnostic && !rejected_cases.is_empty() {
                self.builder.diagnose(PlanDiagnostic::CandidateRejected {
                    rule: identity.clone(),
                    target: Some(target),
                    rejections: rejected_cases,
                });
            }
        }
        self
    }

    pub(in crate::fixups) fn finish(self) -> ExprPlan {
        ExprPlan {
            plan: self.builder.finish(),
        }
    }
}

impl Default for ExprPlanBuilder {
    fn default() -> Self {
        Self::new()
    }
}

pub(in crate::fixups) struct ExprPlan {
    plan: Plan<ReplaceExpr>,
}

impl ExprPlan {
    pub(in crate::fixups) fn is_empty(&self) -> bool {
        self.plan.edits.is_empty()
    }

    pub(in crate::fixups) fn apply(
        self,
        program: &mut Program,
        facts: &FixupFacts,
        logger: &mut dyn TraceLogger,
    ) -> ApplyReport {
        let planned = self.plan.edits.len();
        let mut diagnostics = self.plan.diagnostics;
        let mut edits = self
            .plan
            .edits
            .into_iter()
            .map(|edit| {
                (
                    (edit.edit.target.item_index, edit.edit.target.path.clone()),
                    edit,
                )
            })
            .collect::<BTreeMap<_, _>>();
        let mut applied = 0;
        let mut touched = TouchedItems::none();
        for (item_index, item) in program.items.iter_mut().enumerate() {
            let Item::Fn(function) = item else {
                continue;
            };
            if !edits
                .keys()
                .any(|(target_item, _)| *target_item == item_index)
            {
                continue;
            }
            walk::body_exprs_mut_with_path(
                &mut function.body,
                &mut Vec::new(),
                &mut |expr, path| {
                    let key = (item_index, AstPath(path.to_vec()));
                    let Some(edit) = edits.remove(&key) else {
                        return true;
                    };
                    let PlannedEdit {
                        identity,
                        edit:
                            ReplaceExpr {
                                target,
                                replacement,
                                evidence,
                            },
                        rejected_cases,
                    } = edit;
                    let before = std::mem::replace(expr, replacement);
                    if logger.is_enabled() {
                        logger.rewrite(rewrite_event(
                            &identity,
                            &target,
                            &evidence,
                            &rejected_cases,
                            &before,
                            expr,
                            facts,
                        ));
                    }
                    applied += 1;
                    touched.in_place.push(item_index);
                    false
                },
            );
        }
        for (_, edit) in edits {
            diagnostics.push(PlanDiagnostic::MissingTarget {
                contender: edit.identity,
                target: edit.edit.target,
            });
        }
        ApplyReport {
            changed: applied != 0,
            planned,
            applied,
            diagnostics,
            touched,
        }
    }
}

pub(in crate::fixups) struct ApplyReport {
    pub(in crate::fixups) changed: bool,
    pub(in crate::fixups) planned: usize,
    pub(in crate::fixups) applied: usize,
    #[allow(dead_code)]
    pub(super) diagnostics: Vec<PlanDiagnostic<ExprSite>>,
    pub(in crate::fixups) touched: TouchedItems,
}

fn path_starts_with(
    path: &[crate::fixups::facts::PathSegment],
    prefix: &[crate::fixups::facts::PathSegment],
) -> bool {
    path.len() >= prefix.len() && path[..prefix.len()] == *prefix
}

fn rewrite_event(
    identity: &RuleCaseIdentity,
    target: &ExprSite,
    evidence: &[Evidence],
    rejected_cases: &[CaseRejection],
    before: &Expr,
    after: &Expr,
    facts: &FixupFacts,
) -> RewriteEvent {
    let location = facts
        .function_by_item_index(target.item_index)
        .map(|function| function_path_location(facts, function, &target.path.0))
        .unwrap_or_else(|| path_location(&target.path.0));
    let mut trace_facts = vec![
        fact("query_rule", identity.rule.name.clone()),
        fact("query_case", identity.case.clone()),
    ];
    trace_facts.extend(evidence.iter().map(evidence_trace_fact));
    trace_facts.extend(rejected_cases.iter().map(|rejected| {
        fact(
            format!("rejected_case.{}", rejected.case),
            format!(
                "{}:{}",
                predicate_name(rejected.rejection.predicate),
                rejection_name(rejected.rejection.reason)
            ),
        )
    }));
    RewriteEvent {
        pass: identity.rule.pass,
        kind: identity.rule.name.clone(),
        location,
        before: vec![TraceSnippet::new("expr", before.render().trim_end())],
        after: vec![TraceSnippet::new("expr", after.render().trim_end())],
        facts: trace_facts,
    }
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
        Predicate::StmtWindowGuard => "stmt_window_guard",
        Predicate::LazySingletonDomain => "lazy_singleton_domain",
        Predicate::ReadPath => "read_path",
        Predicate::NoEffects => "no_effects",
        Predicate::HeapOwnershipPlan => "heap_ownership_plan",
        Predicate::StringLiftPlan => "string_lift_plan",
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
        EvidenceDetail::NoEffects => "no_effects".into(),
        EvidenceDetail::HeapOwnershipPlan { plans } => format!("plans={plans}"),
        EvidenceDetail::StringLiftPlan { plans } => format!("plans={plans}"),
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
