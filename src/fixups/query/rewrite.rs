use std::collections::{BTreeMap, BTreeSet};

use crate::fixups::facts::{AstPath, FixupFacts};
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass, RewriteEvent, TraceFact, TraceLogger, TraceSnippet, fact, function_path_location,
    path_location,
};
use crate::rust_ast::{Expr, Item, Program};

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

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) enum PlanDiagnostic {
    CandidateRejected {
        rule: RuleIdentity,
        target: ExprSite,
        rejections: Vec<CaseRejection>,
    },
    TargetMismatch {
        contender: RuleCaseIdentity,
        candidate: ExprSite,
        replacement: ExprSite,
    },
    AmbiguousTarget {
        target: ExprSite,
        contenders: Vec<RuleCaseIdentity>,
    },
    OverlappingTargets {
        first: ExprSite,
        second: ExprSite,
    },
    MissingTarget {
        contender: RuleCaseIdentity,
        target: ExprSite,
    },
}

struct PlannedEdit {
    identity: RuleCaseIdentity,
    replacement: ReplaceExpr,
    rejected_cases: Vec<CaseRejection>,
}

pub(in crate::fixups) struct ExprPlanBuilder {
    proposals: Vec<PlannedEdit>,
    diagnostics: Vec<PlanDiagnostic>,
}

impl ExprPlanBuilder {
    pub(in crate::fixups) fn new() -> Self {
        Self {
            proposals: Vec::new(),
            diagnostics: Vec::new(),
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
                            self.diagnostics.push(PlanDiagnostic::TargetMismatch {
                                contender,
                                candidate: target.clone(),
                                replacement: replacement.target,
                            });
                            terminal_diagnostic = true;
                        } else {
                            selected = Some(PlannedEdit {
                                identity: contender,
                                replacement,
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
                self.proposals.push(selected);
            } else if !terminal_diagnostic && !rejected_cases.is_empty() {
                self.diagnostics.push(PlanDiagnostic::CandidateRejected {
                    rule: identity.clone(),
                    target,
                    rejections: rejected_cases,
                });
            }
        }
        self
    }

    pub(in crate::fixups) fn finish(self) -> ExprPlan {
        let mut diagnostics = self.diagnostics;
        let mut grouped = BTreeMap::<ExprSite, Vec<PlannedEdit>>::new();
        for proposal in self.proposals {
            grouped
                .entry(proposal.replacement.target.clone())
                .or_default()
                .push(proposal);
        }
        let mut edits = Vec::new();
        for (target, mut contenders) in grouped {
            if contenders.len() == 1 {
                edits.push(contenders.pop().unwrap());
            } else {
                diagnostics.push(PlanDiagnostic::AmbiguousTarget {
                    target,
                    contenders: contenders
                        .into_iter()
                        .map(|contender| contender.identity)
                        .collect(),
                });
            }
        }
        edits.sort_by(|left, right| left.replacement.target.cmp(&right.replacement.target));
        let mut overlapping = BTreeSet::new();
        for first in 0..edits.len() {
            for second in first + 1..edits.len() {
                let first_site = &edits[first].replacement.target;
                let second_site = &edits[second].replacement.target;
                if sites_overlap(first_site, second_site) {
                    overlapping.insert(first);
                    overlapping.insert(second);
                    diagnostics.push(PlanDiagnostic::OverlappingTargets {
                        first: first_site.clone(),
                        second: second_site.clone(),
                    });
                }
            }
        }
        let edits = edits
            .into_iter()
            .enumerate()
            .filter_map(|(index, edit)| (!overlapping.contains(&index)).then_some(edit))
            .collect();
        ExprPlan { edits, diagnostics }
    }
}

impl Default for ExprPlanBuilder {
    fn default() -> Self {
        Self::new()
    }
}

pub(in crate::fixups) struct ExprPlan {
    edits: Vec<PlannedEdit>,
    diagnostics: Vec<PlanDiagnostic>,
}

impl ExprPlan {
    pub(in crate::fixups) fn is_empty(&self) -> bool {
        self.edits.is_empty()
    }

    pub(in crate::fixups) fn diagnostics(&self) -> &[PlanDiagnostic] {
        &self.diagnostics
    }

    pub(in crate::fixups) fn apply(
        self,
        program: &mut Program,
        facts: &FixupFacts,
        logger: &mut dyn TraceLogger,
    ) -> ApplyReport {
        let planned = self.edits.len();
        let mut diagnostics = self.diagnostics;
        let mut edits = self
            .edits
            .into_iter()
            .map(|edit| {
                (
                    (
                        edit.replacement.target.item_index,
                        edit.replacement.target.path.clone(),
                    ),
                    edit,
                )
            })
            .collect::<BTreeMap<_, _>>();
        let mut applied = 0;
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
                    let ReplaceExpr {
                        target,
                        replacement,
                        evidence,
                    } = edit.replacement;
                    let before = std::mem::replace(expr, replacement);
                    if logger.is_enabled() {
                        logger.rewrite(rewrite_event(
                            &edit.identity,
                            &target,
                            &evidence,
                            &edit.rejected_cases,
                            &before,
                            expr,
                            facts,
                        ));
                    }
                    applied += 1;
                    false
                },
            );
        }
        for (_, edit) in edits {
            diagnostics.push(PlanDiagnostic::MissingTarget {
                contender: edit.identity,
                target: edit.replacement.target,
            });
        }
        ApplyReport {
            changed: applied != 0,
            planned,
            applied,
            diagnostics,
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct ApplyReport {
    pub(in crate::fixups) changed: bool,
    pub(in crate::fixups) planned: usize,
    pub(in crate::fixups) applied: usize,
    pub(in crate::fixups) diagnostics: Vec<PlanDiagnostic>,
}

fn sites_overlap(first: &ExprSite, second: &ExprSite) -> bool {
    first.item_index == second.item_index
        && (path_starts_with(&first.path.0, &second.path.0)
            || path_starts_with(&second.path.0, &first.path.0))
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

fn evidence_trace_fact(evidence: &Evidence) -> TraceFact {
    fact(
        format!("evidence.{}", predicate_name(evidence.predicate)),
        evidence_detail(&evidence.detail),
    )
}

fn predicate_name(predicate: Predicate) -> &'static str {
    match predicate {
        Predicate::Call => "call",
        Predicate::ByteSource => "byte_source",
        Predicate::ConstantU8 => "constant_u8",
        Predicate::ConstantUsize => "constant_usize",
        Predicate::FullByteView => "full_byte_view",
        Predicate::FirstNul => "first_nul",
        Predicate::PrefixContains => "prefix_contains",
        Predicate::MovablePure => "movable_pure",
    }
}

fn rejection_name(rejection: RejectionReason) -> &'static str {
    match rejection {
        RejectionReason::MissingEvidence => "missing_evidence",
        RejectionReason::Contradicted => "contradicted",
        RejectionReason::UnsupportedShape => "unsupported_shape",
        RejectionReason::Ambiguous => "ambiguous",
        RejectionReason::OutOfRange => "out_of_range",
    }
}

fn evidence_detail(detail: &EvidenceDetail) -> String {
    match detail {
        EvidenceDetail::IndexedCall { target, arity } => {
            format!("{};arity={arity}", call_target_name(target))
        }
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
        EvidenceDetail::ConstantU8(value) => value.to_string(),
        EvidenceDetail::ConstantUsize(value) => value.to_string(),
        EvidenceDetail::NulPosition(position) => nul_position_name(*position),
        EvidenceDetail::PrefixContains { count, nul } => {
            format!("count={count};nul={nul}")
        }
        EvidenceDetail::MovablePure => "movable_pure".into(),
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

#[cfg(test)]
mod tests {
    use crate::fixups::facts;
    use crate::fixups::query::{
        CallRecord, CallTarget, ExprPlanBuilder, ExprRule, PlanDiagnostic, QueryContext, Rejection,
        RejectionReason, ReplaceExpr, RuleCase, RuleIdentity, RuleResult,
    };
    use crate::fixups::test_support::{call, func, int, temp};
    use crate::fixups::trace::{NoopLogger, Pass, ProgramSummary, RewriteEvent, TraceLogger};
    use crate::rust_ast::{Expr, Item, Program, RustValue};

    struct OrderedRule;

    impl ExprRule for OrderedRule {
        type Candidate = CallRecord;

        fn identity(&self) -> RuleIdentity {
            RuleIdentity::new(Pass::MemchrPreludeFixupCalls, "ordered_rewrite")
        }

        fn candidates(&self, query: &QueryContext<'_>) -> Vec<Self::Candidate> {
            query
                .calls(&CallTarget::Generated("__rewrite".into()), 1)
                .to_vec()
        }

        fn target(&self, candidate: &Self::Candidate) -> crate::fixups::query::ExprSite {
            candidate.site.clone()
        }

        fn cases(&self, query: &QueryContext<'_>, candidate: &Self::Candidate) -> Vec<RuleCase> {
            let specific = match query.const_u8(&candidate.args[0]) {
                Ok(proof) if proof.value == 0 => RuleResult::replace(
                    ReplaceExpr::new(candidate.site.clone(), int(10)).with_evidence(proof.evidence),
                ),
                Ok(_) => RuleResult::reject(Rejection::new(
                    crate::fixups::query::Predicate::ConstantU8,
                    Some(candidate.args[0].clone()),
                    RejectionReason::Contradicted,
                    Vec::new(),
                )),
                Err(rejection) => RuleResult::reject(rejection),
            };
            vec![
                RuleCase::new("zero", specific),
                RuleCase::new(
                    "fallback",
                    RuleResult::replace(ReplaceExpr::new(candidate.site.clone(), int(20))),
                ),
            ]
        }
    }

    struct FixedRule {
        name: &'static str,
        target: crate::fixups::query::ExprSite,
        replacement: Expr,
    }

    impl ExprRule for FixedRule {
        type Candidate = ();

        fn identity(&self) -> RuleIdentity {
            RuleIdentity::new(Pass::MemchrPreludeFixupCalls, self.name)
        }

        fn candidates(&self, _query: &QueryContext<'_>) -> Vec<Self::Candidate> {
            vec![()]
        }

        fn target(&self, _candidate: &Self::Candidate) -> crate::fixups::query::ExprSite {
            self.target.clone()
        }

        fn cases(&self, _query: &QueryContext<'_>, _candidate: &Self::Candidate) -> Vec<RuleCase> {
            vec![RuleCase::new(
                "only",
                RuleResult::replace(ReplaceExpr::new(
                    self.target.clone(),
                    self.replacement.clone(),
                )),
            )]
        }
    }

    #[derive(Default)]
    struct RecordingLogger {
        events: Vec<RewriteEvent>,
    }

    impl TraceLogger for RecordingLogger {
        fn is_enabled(&self) -> bool {
            true
        }

        fn begin_pass(&mut self, _pass: Pass, _before: ProgramSummary, _before_emit: String) {}

        fn end_pass(&mut self, _after: ProgramSummary, _after_emit: String) {}

        fn rewrite(&mut self, event: RewriteEvent) {
            self.events.push(event);
        }
    }

    fn program() -> Program {
        Program {
            items: vec![Item::Fn(func(
                Vec::new(),
                None,
                vec![
                    temp("first", "i32", call("__rewrite", vec![int(0)])),
                    temp("second", "i32", call("__rewrite", vec![int(1)])),
                ],
            ))],
        }
    }

    #[test]
    fn ordered_cases_plan_and_apply_owned_replacements() {
        let mut program = program();
        let analyzed = facts::analyze(&program);
        let query = QueryContext::new(analyzed.program, &analyzed.facts);
        let mut builder = ExprPlanBuilder::new();
        builder.add_rule(&query, &OrderedRule);
        let plan = builder.finish();

        assert!(!plan.is_empty());
        assert!(plan.diagnostics().is_empty());
        drop(query);
        let facts = analyzed.facts;
        let mut logger = RecordingLogger::default();
        let report = plan.apply(&mut program, &facts, &mut logger);

        assert!(report.changed);
        assert_eq!(report.planned, 2);
        assert_eq!(report.applied, 2);
        assert!(report.diagnostics.is_empty());
        assert_eq!(logger.events.len(), 2);
        assert_eq!(logger.events[0].kind, "ordered_rewrite");
        assert!(
            logger.events[0]
                .facts
                .iter()
                .any(|fact| fact.key == "evidence.constant_u8")
        );
        assert!(
            logger.events[1]
                .facts
                .iter()
                .any(|fact| fact.key == "rejected_case.zero")
        );
        let Item::Fn(function) = &program.items[0] else {
            panic!()
        };
        assert!(matches!(
            &function.body[0].stmt,
            crate::rust_ast::Stmt::Let {
                init: Some(Expr::Value(RustValue::I64(10))),
                ..
            }
        ));
        assert!(matches!(
            &function.body[1].stmt,
            crate::rust_ast::Stmt::Let {
                init: Some(Expr::Value(RustValue::I64(20))),
                ..
            }
        ));
    }

    #[test]
    fn same_target_results_are_ambiguous() {
        let mut program = program();
        let analyzed = facts::analyze(&program);
        let query = QueryContext::new(analyzed.program, &analyzed.facts);
        let target = query.calls(&CallTarget::Generated("__rewrite".into()), 1)[0]
            .site
            .clone();
        let mut builder = ExprPlanBuilder::new();
        builder
            .add_rule(
                &query,
                &FixedRule {
                    name: "first",
                    target: target.clone(),
                    replacement: int(1),
                },
            )
            .add_rule(
                &query,
                &FixedRule {
                    name: "second",
                    target,
                    replacement: int(2),
                },
            );
        let plan = builder.finish();

        assert!(plan.is_empty());
        assert!(matches!(
            plan.diagnostics(),
            [PlanDiagnostic::AmbiguousTarget { contenders, .. }] if contenders.len() == 2
        ));
        drop(query);
        let facts = analyzed.facts;
        let report = plan.apply(&mut program, &facts, &mut NoopLogger);
        assert!(!report.changed);
        assert_eq!(report.planned, 0);
        assert_eq!(report.applied, 0);
    }

    #[test]
    fn parent_and_child_results_are_rejected_together() {
        let program = program();
        let analyzed = facts::analyze(&program);
        let query = QueryContext::new(analyzed.program, &analyzed.facts);
        let call = &query.calls(&CallTarget::Generated("__rewrite".into()), 1)[0];
        let mut builder = ExprPlanBuilder::new();
        builder
            .add_rule(
                &query,
                &FixedRule {
                    name: "parent",
                    target: call.site.clone(),
                    replacement: int(1),
                },
            )
            .add_rule(
                &query,
                &FixedRule {
                    name: "child",
                    target: call.args[0].clone(),
                    replacement: int(2),
                },
            );
        let plan = builder.finish();

        assert!(plan.is_empty());
        assert!(matches!(
            plan.diagnostics(),
            [PlanDiagnostic::OverlappingTargets { .. }]
        ));
    }

    #[test]
    fn rejected_candidates_remain_diagnostic() {
        struct RejectingRule;

        impl ExprRule for RejectingRule {
            type Candidate = CallRecord;

            fn identity(&self) -> RuleIdentity {
                RuleIdentity::new(Pass::MemchrPreludeFixupCalls, "rejecting")
            }

            fn candidates(&self, query: &QueryContext<'_>) -> Vec<Self::Candidate> {
                query
                    .calls(&CallTarget::Generated("__rewrite".into()), 1)
                    .to_vec()
            }

            fn target(&self, candidate: &Self::Candidate) -> crate::fixups::query::ExprSite {
                candidate.site.clone()
            }

            fn cases(
                &self,
                query: &QueryContext<'_>,
                candidate: &Self::Candidate,
            ) -> Vec<RuleCase> {
                vec![RuleCase::new(
                    "constant",
                    match query.const_u8(&candidate.site) {
                        Ok(_) => unreachable!(),
                        Err(rejection) => RuleResult::reject(rejection),
                    },
                )]
            }
        }

        let program = program();
        let analyzed = facts::analyze(&program);
        let query = QueryContext::new(analyzed.program, &analyzed.facts);
        let mut builder = ExprPlanBuilder::new();
        builder.add_rule(&query, &RejectingRule);
        let plan = builder.finish();

        assert!(plan.is_empty());
        assert_eq!(plan.diagnostics().len(), 2);
        assert!(plan.diagnostics().iter().all(|diagnostic| matches!(
            diagnostic,
            PlanDiagnostic::CandidateRejected { rejections, .. }
                if rejections.len() == 1
        )));
    }
}
