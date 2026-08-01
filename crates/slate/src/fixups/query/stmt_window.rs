use std::collections::BTreeMap;

use crate::fixups::facts::walk;
use crate::fixups::facts::{AstPath, FixupFacts, PathSegment};
use crate::fixups::support::walk as mut_walk;
use crate::fixups::trace::{
    Pass, RewriteEvent, TraceLogger, TraceSnippet, fact, function_path_location, path_location,
};
use crate::rust_ast::{IndentStmt, Item, Program, Stmt};

use super::plan::{
    EditTarget, Plan, PlanBuilder, PlanDiagnostic, PlanSite, PlannedEdit, TouchedItems,
};
use super::rewrite::{evidence_trace_fact, predicate_name, rejection_name};
use super::{
    CaseRejection, Evidence, Local, Predicate, QueryContext, Rejection, RejectionReason,
    RuleCaseIdentity, RuleIdentity, StmtWindowSite,
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
    local: Option<Local>,
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
            local: None,
            cases: Vec::new(),
        }
    }

    pub(in crate::fixups) fn matching_local(mut self, matcher: Local) -> Self {
        self.local = Some(matcher);
        self
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
        let Some(local) = &self.local else {
            return candidates;
        };
        candidates
            .into_iter()
            .filter(|candidate| {
                let Stmt::Let { name, mutable, .. } = &candidate.window[0].stmt else {
                    return false;
                };
                local.name.matches(name, &())
                    && local.mutable.matches(mutable, &())
                    && local
                        .value
                        .matches(&query.local_value(&candidate.site, name), &())
            })
            .collect()
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

    pub(in crate::fixups) fn stmt_path(&self, offset: usize) -> AstPath {
        let mut path = self.candidate.site.path.0.clone();
        path.push(PathSegment::Stmt(self.candidate.site.start + offset));
        AstPath(path)
    }

    pub(in crate::fixups) fn counted_loop(
        &mut self,
    ) -> Result<crate::fixups::facts::CountedLoopFact, Rejection> {
        self.prove(self.query.counted_loop(&self.candidate.site))
    }

    pub(in crate::fixups) fn read_path(
        &mut self,
        name: &str,
    ) -> Result<crate::fixups::facts::AstPath, Rejection> {
        self.prove(self.query.read_path(&self.candidate.site, name))
    }

    pub(in crate::fixups) fn no_effects(&mut self) -> Result<(), Rejection> {
        self.prove(self.query.no_effects(&self.candidate.site))
    }

    pub(in crate::fixups) fn require(&self, condition: bool) -> Result<(), Rejection> {
        if condition {
            Ok(())
        } else {
            Err(self.reject())
        }
    }

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
        let edits = self
            .plan
            .edits
            .into_iter()
            .map(|edit| (edit.edit.target.clone(), edit))
            .collect::<BTreeMap<_, _>>();
        let mut state = ApplyState {
            edits,
            applied: 0,
            touched: TouchedItems::none(),
            facts,
            logger,
        };
        for (item_index, item) in program.items.iter_mut().enumerate() {
            let Item::Fn(function) = item else {
                continue;
            };
            if !state
                .edits
                .keys()
                .any(|target| target.item_index == item_index)
            {
                continue;
            }
            apply_body(item_index, &mut function.body, &mut Vec::new(), &mut state);
        }
        for (_, edit) in state.edits {
            diagnostics.push(PlanDiagnostic::MissingTarget {
                contender: edit.identity,
                target: edit.edit.target,
            });
        }
        StmtWindowApplyReport {
            changed: state.applied != 0,
            planned,
            applied: state.applied,
            diagnostics,
            touched: state.touched,
        }
    }
}

/// The parts of `apply`'s state that stay the same across the whole
/// recursive descent - bundled so `apply_body` doesn't need eight separate
/// parameters just to thread them through.
struct ApplyState<'a> {
    edits: BTreeMap<StmtWindowSite, PlannedEdit<StmtWindowEdit>>,
    applied: usize,
    touched: TouchedItems,
    facts: &'a FixupFacts,
    logger: &'a mut dyn TraceLogger,
}

fn apply_body(
    item_index: usize,
    body: &mut Vec<IndentStmt>,
    path: &mut Vec<PathSegment>,
    state: &mut ApplyState<'_>,
) {
    for (index, indent) in body.iter_mut().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            mut_walk::nested_body_vecs_mut_with_path(
                &mut indent.stmt,
                path,
                &mut |nested, path| {
                    apply_body(item_index, nested, path, state);
                },
            );
        });
    }
    let here = AstPath(path.clone());
    let mut here_sites = state
        .edits
        .keys()
        .filter(|site| site.item_index == item_index && site.path == here)
        .cloned()
        .collect::<Vec<_>>();
    here_sites.sort_by_key(|site| std::cmp::Reverse(site.start));
    for site in here_sites {
        let Some(edit) = state.edits.remove(&site) else {
            continue;
        };
        if site.end > body.len() {
            state.edits.insert(site.clone(), edit);
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
        if state.logger.is_enabled() {
            state.logger.rewrite(rewrite_event(
                &identity,
                &target,
                &evidence,
                &rejected_cases,
                &before,
                &replacement,
                state.facts,
            ));
        }
        body.splice(target.start..target.end, replacement);
        state.applied += 1;
        state.touched.in_place.push(item_index);
    }
}

pub(in crate::fixups) struct StmtWindowApplyReport {
    pub(in crate::fixups) changed: bool,
    pub(in crate::fixups) planned: usize,
    pub(in crate::fixups) applied: usize,
    #[allow(dead_code)]
    pub(super) diagnostics: Vec<PlanDiagnostic<StmtWindowSite>>,
    pub(in crate::fixups) touched: TouchedItems,
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
