use std::collections::BTreeMap;

use crate::fixups::facts::walk;
use crate::fixups::facts::{AstPath, FixupFacts, PathSegment};
use crate::fixups::support::walk as mut_walk;
use crate::fixups::trace::{
    Pass, RewriteEvent, TraceLogger, TraceSnippet, fact, function_path_location, path_location,
};
use crate::rust_ast::{IndentStmt, Item, Program};

use super::plan::{EditTarget, Plan, PlanBuilder, PlanDiagnostic, PlanSite, PlannedEdit};
use super::rewrite::{evidence_trace_fact, predicate_name, rejection_name};
use super::{
    CaseRejection, Evidence, Predicate, QueryContext, Rejection, RejectionReason, RuleCaseIdentity,
    RuleIdentity, StmtWindowSite,
};

type StmtWindowCaseFn = for<'case, 'snapshot> fn(
    &mut StmtWindowCaseContext<'case, 'snapshot>,
) -> Result<Vec<IndentStmt>, Rejection>;

struct DeclarativeStmtWindowCase {
    name: String,
    apply: StmtWindowCaseFn,
}

pub(in crate::fixups) struct StmtWindowRule {
    identity: RuleIdentity,
    width: usize,
    cases: Vec<DeclarativeStmtWindowCase>,
}

impl StmtWindowRule {
    pub(in crate::fixups) fn new(pass: Pass, rule: impl Into<String>, width: usize) -> Self {
        assert!(
            width > 0,
            "a statement window must cover at least one statement"
        );
        Self {
            identity: RuleIdentity::new(pass, rule),
            width,
            cases: Vec::new(),
        }
    }

    pub(in crate::fixups) fn case(
        mut self,
        name: impl Into<String>,
        apply: StmtWindowCaseFn,
    ) -> Self {
        self.cases.push(DeclarativeStmtWindowCase {
            name: name.into(),
            apply,
        });
        self
    }

    fn candidates<'snapshot>(
        &self,
        query: &QueryContext<'snapshot>,
    ) -> Vec<StmtWindowCandidate<'snapshot>> {
        let mut candidates = Vec::new();
        for (item_index, item) in query.snapshot_program().items.iter().enumerate() {
            if let Item::Fn(function) = item {
                collect_windows(
                    item_index,
                    &function.body,
                    &mut Vec::new(),
                    self.width,
                    &mut candidates,
                );
            }
        }
        candidates
    }
}

fn collect_windows<'snapshot>(
    item_index: usize,
    body: &'snapshot [IndentStmt],
    path: &mut Vec<PathSegment>,
    width: usize,
    out: &mut Vec<StmtWindowCandidate<'snapshot>>,
) {
    if let Some(window_count) = body.len().checked_sub(width - 1) {
        for start in 0..window_count {
            out.push(StmtWindowCandidate {
                site: StmtWindowSite {
                    item_index,
                    path: AstPath(path.clone()),
                    start,
                    end: start + width,
                },
                window: &body[start..start + width],
            });
        }
    }
    for (index, indent) in body.iter().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            walk::nested_body_vecs_with_path(&indent.stmt, path, &mut |nested, path| {
                collect_windows(item_index, nested, path, width, out);
            });
        });
    }
}

struct StmtWindowCandidate<'snapshot> {
    site: StmtWindowSite,
    window: &'snapshot [IndentStmt],
}

pub(in crate::fixups) struct StmtWindowCaseContext<'case, 'snapshot> {
    query: &'case QueryContext<'snapshot>,
    candidate: &'case StmtWindowCandidate<'snapshot>,
    evidence: Vec<Evidence>,
}

impl<'snapshot> StmtWindowCaseContext<'_, 'snapshot> {
    pub(in crate::fixups) fn stmts<const N: usize>(&self) -> [&'snapshot IndentStmt; N] {
        assert_eq!(N, self.candidate.window.len());
        std::array::from_fn(|index| &self.candidate.window[index])
    }

    pub(in crate::fixups) fn counted_loop(
        &mut self,
    ) -> Result<crate::fixups::facts::CountedLoopFact, Rejection> {
        self.prove(self.query.counted_loop(&self.candidate.site))
    }

    /// Rejects the case when `condition` is false, carrying forward whatever
    /// evidence earlier proofs in this case already accumulated.
    pub(in crate::fixups) fn require(&self, condition: bool) -> Result<(), Rejection> {
        if condition {
            Ok(())
        } else {
            Err(self.reject())
        }
    }

    /// Builds a rejection for a structural shape that doesn't fit this case,
    /// carrying forward whatever evidence earlier proofs already
    /// accumulated. Statement-window shapes are arbitrary `Stmt` patterns,
    /// not one fixed shape like a call's argument list, so unlike
    /// `CallCaseContext` there is no single named predicate for "the
    /// window doesn't look like this case wants" - rule bodies match the
    /// shape themselves and reject through this when it doesn't fit.
    pub(in crate::fixups) fn reject(&self) -> Rejection {
        Rejection::new(
            Predicate::StmtWindowGuard,
            None,
            RejectionReason::UnsupportedShape,
            self.evidence.clone(),
        )
    }

    fn prove<T>(&mut self, result: super::QueryResult<T>) -> Result<T, Rejection> {
        match result {
            Ok(proof) => {
                self.evidence.extend(proof.evidence);
                Ok(proof.value)
            }
            Err(mut rejection) => {
                let mut evidence = self.evidence.clone();
                evidence.append(&mut rejection.evidence);
                rejection.evidence = evidence;
                Err(rejection)
            }
        }
    }
}

struct StmtWindowEdit {
    target: StmtWindowSite,
    replacement: Vec<IndentStmt>,
    evidence: Vec<Evidence>,
}

impl EditTarget for StmtWindowEdit {
    type Site = StmtWindowSite;

    fn site(&self) -> StmtWindowSite {
        self.target.clone()
    }
}

impl PlanSite for StmtWindowSite {
    fn overlaps(&self, other: &Self) -> bool {
        if self.item_index != other.item_index {
            return false;
        }
        if self.path == other.path {
            return self.start < other.end && other.start < self.end;
        }
        if let Some(rest) = strip_prefix(&other.path.0, &self.path.0) {
            return nested_within(rest, self.start, self.end);
        }
        if let Some(rest) = strip_prefix(&self.path.0, &other.path.0) {
            return nested_within(rest, other.start, other.end);
        }
        false
    }
}

fn strip_prefix<'a>(path: &'a [PathSegment], prefix: &[PathSegment]) -> Option<&'a [PathSegment]> {
    (path.len() > prefix.len() && path[..prefix.len()] == *prefix).then(|| &path[prefix.len()..])
}

fn nested_within(rest: &[PathSegment], start: usize, end: usize) -> bool {
    matches!(rest.first(), Some(PathSegment::Stmt(index)) if (start..end).contains(index))
}

pub(in crate::fixups) struct StmtWindowPlanBuilder {
    builder: PlanBuilder<StmtWindowEdit>,
}

impl StmtWindowPlanBuilder {
    pub(in crate::fixups) fn new() -> Self {
        Self {
            builder: PlanBuilder::new(),
        }
    }

    pub(in crate::fixups) fn add_rule(
        &mut self,
        query: &QueryContext<'_>,
        rule: &StmtWindowRule,
    ) -> &mut Self {
        let identity = rule.identity.clone();
        for candidate in rule.candidates(query) {
            let mut rejected_cases = Vec::new();
            let mut selected = None;
            for case in &rule.cases {
                let mut context = StmtWindowCaseContext {
                    query,
                    candidate: &candidate,
                    evidence: Vec::new(),
                };
                match (case.apply)(&mut context) {
                    Ok(replacement) => {
                        selected = Some(PlannedEdit {
                            identity: RuleCaseIdentity {
                                rule: identity.clone(),
                                case: case.name.clone(),
                            },
                            edit: StmtWindowEdit {
                                target: candidate.site.clone(),
                                replacement,
                                evidence: context.evidence,
                            },
                            rejected_cases: std::mem::take(&mut rejected_cases),
                        });
                        break;
                    }
                    Err(rejection) => rejected_cases.push(CaseRejection {
                        case: case.name.clone(),
                        rejection,
                    }),
                }
            }
            if let Some(selected) = selected {
                self.builder.propose(selected);
            } else if !rejected_cases.is_empty() {
                self.builder.diagnose(PlanDiagnostic::CandidateRejected {
                    rule: identity.clone(),
                    target: Some(candidate.site.clone()),
                    rejections: rejected_cases,
                });
            }
        }
        self
    }

    pub(in crate::fixups) fn finish(self) -> StmtWindowPlan {
        StmtWindowPlan {
            plan: self.builder.finish(),
        }
    }
}

pub(in crate::fixups) struct StmtWindowPlan {
    plan: Plan<StmtWindowEdit>,
}

impl StmtWindowPlan {
    pub(in crate::fixups) fn apply(
        self,
        program: &mut Program,
        facts: &FixupFacts,
        logger: &mut dyn TraceLogger,
    ) -> StmtWindowApplyReport {
        let planned = self.plan.edits.len();
        let mut diagnostics = self.plan.diagnostics;
        let mut edits = self
            .plan
            .edits
            .into_iter()
            .map(|edit| (edit.edit.target.clone(), edit))
            .collect::<BTreeMap<_, _>>();
        let mut applied = 0;
        for (item_index, item) in program.items.iter_mut().enumerate() {
            let Item::Fn(function) = item else {
                continue;
            };
            if !edits.keys().any(|target| target.item_index == item_index) {
                continue;
            }
            apply_body(
                item_index,
                &mut function.body,
                &mut Vec::new(),
                &mut edits,
                &mut applied,
                facts,
                logger,
            );
        }
        for (_, edit) in edits {
            diagnostics.push(PlanDiagnostic::MissingTarget {
                contender: edit.identity,
                target: edit.edit.target,
            });
        }
        StmtWindowApplyReport {
            changed: applied != 0,
            planned,
            applied,
            diagnostics,
        }
    }
}

fn apply_body(
    item_index: usize,
    body: &mut Vec<IndentStmt>,
    path: &mut Vec<PathSegment>,
    edits: &mut BTreeMap<StmtWindowSite, PlannedEdit<StmtWindowEdit>>,
    applied: &mut usize,
    facts: &FixupFacts,
    logger: &mut dyn TraceLogger,
) {
    for (index, indent) in body.iter_mut().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            mut_walk::nested_body_vecs_mut_with_path(
                &mut indent.stmt,
                path,
                &mut |nested, path| {
                    apply_body(item_index, nested, path, edits, applied, facts, logger);
                },
            );
        });
    }
    let here = AstPath(path.clone());
    let mut here_sites = edits
        .keys()
        .filter(|site| site.item_index == item_index && site.path == here)
        .cloned()
        .collect::<Vec<_>>();
    here_sites.sort_by_key(|site| std::cmp::Reverse(site.start));
    for site in here_sites {
        let Some(edit) = edits.remove(&site) else {
            continue;
        };
        if site.end > body.len() {
            edits.insert(site.clone(), edit);
            continue;
        }
        let PlannedEdit {
            identity,
            edit:
                StmtWindowEdit {
                    target,
                    replacement,
                    evidence,
                },
            rejected_cases,
        } = edit;
        let before = body[target.start..target.end].to_vec();
        if logger.is_enabled() {
            logger.rewrite(rewrite_event(
                &identity,
                &target,
                &evidence,
                &rejected_cases,
                &before,
                &replacement,
                facts,
            ));
        }
        body.splice(target.start..target.end, replacement);
        *applied += 1;
    }
}

pub(in crate::fixups) struct StmtWindowApplyReport {
    pub(in crate::fixups) changed: bool,
    pub(in crate::fixups) planned: usize,
    pub(in crate::fixups) applied: usize,
    #[allow(dead_code)]
    pub(super) diagnostics: Vec<PlanDiagnostic<StmtWindowSite>>,
}

fn rewrite_event(
    identity: &RuleCaseIdentity,
    target: &StmtWindowSite,
    evidence: &[Evidence],
    rejected_cases: &[CaseRejection],
    before: &[IndentStmt],
    after: &[IndentStmt],
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
    let render = |stmts: &[IndentStmt]| {
        stmts
            .iter()
            .map(|indent| indent.stmt.render())
            .collect::<Vec<_>>()
            .join("\n")
    };
    RewriteEvent {
        pass: identity.rule.pass,
        kind: identity.rule.name.clone(),
        location,
        before: vec![TraceSnippet::new("stmts", render(before).trim_end())],
        after: vec![TraceSnippet::new("stmts", render(after).trim_end())],
        facts: trace_facts,
    }
}
