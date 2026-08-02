use std::collections::BTreeMap;

use crate::fixups::facts::walk;
use crate::fixups::facts::{AstPath, FixupFacts, PathSegment};
use crate::fixups::support::walk as mut_walk;
use crate::fixups::trace::{
    Pass, RewriteEvent, TraceLogger, TraceSnippet, fact, function_path_location, path_location,
};
use crate::rust_ast::{IndentStmt, Item, Program};

use super::plan::{
    EditTarget, Plan, PlanBuilder, PlanDiagnostic, PlanSite, PlannedEdit, TouchedItems,
};
use super::rewrite::{evidence_trace_fact, predicate_name, rejection_name};
use super::{
    CaseRejection, Evidence, Predicate, QueryContext, Rejection, RejectionReason, RuleCaseIdentity,
    RuleIdentity, StatementRange,
};

pub(in crate::fixups) enum QueryItem<'snapshot> {
    Statement {
        site: StatementRef,
        tail: &'snapshot [IndentStmt],
    },
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(in crate::fixups) struct StatementRef {
    pub(in crate::fixups) item_index: usize,
    pub(in crate::fixups) path: AstPath,
}

impl StatementRef {
    pub(in crate::fixups) fn range(&self) -> StatementRange {
        let mut container = self.path.0.clone();
        let Some(PathSegment::Stmt(index)) = container.pop() else {
            unreachable!()
        };
        StatementRange {
            item_index: self.item_index,
            path: AstPath(container),
            start: index,
            end: index + 1,
        }
    }
}

#[derive(Clone)]
pub(in crate::fixups) struct StatementMatch {
    target: StatementRange,
    statements: Vec<IndentStmt>,
}

impl StatementMatch {
    pub(in crate::fixups) fn new(target: StatementRange, statements: Vec<IndentStmt>) -> Self {
        Self { target, statements }
    }

    pub(in crate::fixups) fn target(&self) -> &StatementRange {
        &self.target
    }

    pub(in crate::fixups) fn stmts<const N: usize>(&self) -> [&IndentStmt; N] {
        assert_eq!(N, self.statements.len());
        std::array::from_fn(|index| &self.statements[index])
    }

    pub(in crate::fixups) fn statement(&self, offset: usize) -> StatementRef {
        assert!(offset < self.statements.len());
        let mut path = self.target.path.0.clone();
        path.push(PathSegment::Stmt(self.target.start + offset));
        StatementRef {
            item_index: self.target.item_index,
            path: AstPath(path),
        }
    }
}

pub(in crate::fixups) trait Matcher {
    type Capture: Clone;

    fn matches(&self, query: &QueryContext<'_>, item: &QueryItem<'_>) -> Option<Self::Capture>;
}

type ItemCaseFn<C> = for<'case, 'snapshot> fn(
    &mut ItemCaseContext<'case, 'snapshot>,
    &C,
) -> Result<EditSet, Rejection>;

struct DeclarativeItemCase<C> {
    name: String,
    apply: ItemCaseFn<C>,
}

pub(in crate::fixups) struct QueryRule<M: Matcher> {
    identity: RuleIdentity,
    matcher: M,
    cases: Vec<DeclarativeItemCase<M::Capture>>,
}

impl<M: Matcher> QueryRule<M> {
    pub(in crate::fixups) fn new(pass: Pass, rule: impl Into<String>, matcher: M) -> Self {
        Self {
            identity: RuleIdentity::new(pass, rule),
            matcher,
            cases: Vec::new(),
        }
    }

    pub(in crate::fixups) fn case(
        mut self,
        name: impl Into<String>,
        apply: ItemCaseFn<M::Capture>,
    ) -> Self {
        self.cases.push(DeclarativeItemCase {
            name: name.into(),
            apply,
        });
        self
    }
}

pub(in crate::fixups) struct ItemCaseContext<'case, 'snapshot> {
    query: &'case QueryContext<'snapshot>,
    evidence: Vec<Evidence>,
}

impl ItemCaseContext<'_, '_> {
    pub(in crate::fixups) fn local_binding(
        &mut self,
        statement: &StatementRef,
        name: &str,
    ) -> Result<super::BindingRef, Rejection> {
        self.prove(
            self.query
                .binding_at(statement.item_index, &statement.path, name),
        )
    }

    pub(in crate::fixups) fn def_use(
        &mut self,
        binding: &super::BindingRef,
    ) -> Result<super::BindingDefUse, Rejection> {
        self.prove(self.query.binding_def_use(binding))
    }

    pub(in crate::fixups) fn counted_loop(
        &mut self,
        statement: &StatementRef,
    ) -> Result<crate::fixups::facts::CountedLoopFact, Rejection> {
        self.prove(self.query.counted_loop(statement))
    }

    pub(in crate::fixups) fn no_effects(
        &mut self,
        statement: &StatementRef,
    ) -> Result<(), Rejection> {
        self.prove(self.query.no_effects(statement))
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
            Predicate::ItemGuard,
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

pub(in crate::fixups) enum AnchoredEdit {
    ReplaceStatements {
        target: StatementRange,
        replacement: Vec<IndentStmt>,
    },
}

pub(in crate::fixups) struct EditSet {
    edits: Vec<AnchoredEdit>,
    evidence: Vec<Evidence>,
}

impl EditSet {
    pub(in crate::fixups) fn replace_statements(
        target: StatementRange,
        replacement: Vec<IndentStmt>,
    ) -> Self {
        Self {
            edits: vec![AnchoredEdit::ReplaceStatements {
                target,
                replacement,
            }],
            evidence: Vec::new(),
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord)]
pub(in crate::fixups) struct EditSetSite(Vec<StatementRange>);

impl EditTarget for EditSet {
    type Site = EditSetSite;

    fn site(&self) -> Self::Site {
        let mut sites = self
            .edits
            .iter()
            .map(|edit| match edit {
                AnchoredEdit::ReplaceStatements { target, .. } => target.clone(),
            })
            .collect::<Vec<_>>();
        sites.sort();
        EditSetSite(sites)
    }
}

impl PlanSite for EditSetSite {
    fn overlaps(&self, other: &Self) -> bool {
        self.0
            .iter()
            .any(|left| other.0.iter().any(|right| left.overlaps(right)))
    }
}

impl StatementRange {
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

pub(in crate::fixups) struct ItemPlanBuilder {
    builder: PlanBuilder<EditSet>,
}

impl ItemPlanBuilder {
    pub(in crate::fixups) fn new() -> Self {
        Self {
            builder: PlanBuilder::new(),
        }
    }

    pub(in crate::fixups) fn add_rule<M: Matcher>(
        &mut self,
        query: &QueryContext<'_>,
        rule: &QueryRule<M>,
    ) -> &mut Self {
        let identity = rule.identity.clone();
        for item in query_items(query) {
            let Some(capture) = rule.matcher.matches(query, &item) else {
                continue;
            };
            let mut rejected_cases = Vec::new();
            let mut selected = None;
            for case in &rule.cases {
                let mut context = ItemCaseContext {
                    query,
                    evidence: Vec::new(),
                };
                match (case.apply)(&mut context, &capture) {
                    Ok(mut edit) => {
                        edit.evidence.extend(context.evidence);
                        selected = Some(PlannedEdit {
                            identity: RuleCaseIdentity {
                                rule: identity.clone(),
                                case: case.name.clone(),
                            },
                            edit,
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
                    target: None,
                    rejections: rejected_cases,
                });
            }
        }
        self
    }

    pub(in crate::fixups) fn finish(self) -> ItemPlan {
        ItemPlan {
            plan: self.builder.finish(),
        }
    }
}

fn query_items<'snapshot>(query: &QueryContext<'snapshot>) -> Vec<QueryItem<'snapshot>> {
    let mut items = Vec::new();
    for (item_index, item) in query.snapshot_program().items.iter().enumerate() {
        if let Item::Fn(function) = item {
            collect_statement_items(item_index, &function.body, &mut Vec::new(), &mut items);
        }
    }
    items
}

fn collect_statement_items<'snapshot>(
    item_index: usize,
    body: &'snapshot [IndentStmt],
    path: &mut Vec<PathSegment>,
    out: &mut Vec<QueryItem<'snapshot>>,
) {
    for start in 0..body.len() {
        let mut statement_path = path.clone();
        statement_path.push(PathSegment::Stmt(start));
        out.push(QueryItem::Statement {
            site: StatementRef {
                item_index,
                path: AstPath(statement_path),
            },
            tail: &body[start..],
        });
    }
    for (index, indent) in body.iter().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            walk::nested_body_vecs_with_path(&indent.stmt, path, &mut |nested, path| {
                collect_statement_items(item_index, nested, path, out);
            });
        });
    }
}

pub(in crate::fixups) struct ItemPlan {
    plan: Plan<EditSet>,
}

impl ItemPlan {
    pub(in crate::fixups) fn apply(
        self,
        program: &mut Program,
        facts: &FixupFacts,
        logger: &mut dyn TraceLogger,
    ) -> ItemApplyReport {
        let planned = self.plan.edits.len();
        let mut diagnostics = self.plan.diagnostics;
        let mut edits = BTreeMap::new();
        for planned_edit in self.plan.edits {
            let PlannedEdit {
                identity,
                edit,
                rejected_cases,
            } = planned_edit;
            for anchored in edit.edits {
                let AnchoredEdit::ReplaceStatements {
                    target,
                    replacement,
                } = anchored;
                edits.insert(
                    target.clone(),
                    PlannedStatementEdit {
                        identity: identity.clone(),
                        target,
                        replacement,
                        evidence: edit.evidence.clone(),
                        rejected_cases: rejected_cases.clone(),
                    },
                );
            }
        }
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
            if state
                .edits
                .keys()
                .any(|target| target.item_index == item_index)
            {
                apply_body(item_index, &mut function.body, &mut Vec::new(), &mut state);
            }
        }
        for (_, edit) in state.edits {
            diagnostics.push(PlanDiagnostic::MissingTarget {
                contender: edit.identity,
                target: EditSetSite(vec![edit.target]),
            });
        }
        ItemApplyReport {
            changed: state.applied != 0,
            planned,
            applied: state.applied,
            diagnostics,
            touched: state.touched,
        }
    }
}

struct PlannedStatementEdit {
    identity: RuleCaseIdentity,
    target: StatementRange,
    replacement: Vec<IndentStmt>,
    evidence: Vec<Evidence>,
    rejected_cases: Vec<CaseRejection>,
}

struct ApplyState<'a> {
    edits: BTreeMap<StatementRange, PlannedStatementEdit>,
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
            state.edits.insert(site, edit);
            continue;
        }
        let before = body[edit.target.start..edit.target.end].to_vec();
        if state.logger.is_enabled() {
            state.logger.rewrite(rewrite_event(
                &edit.identity,
                &edit.target,
                &edit.evidence,
                &edit.rejected_cases,
                &before,
                &edit.replacement,
                state.facts,
            ));
        }
        body.splice(edit.target.start..edit.target.end, edit.replacement);
        state.applied += 1;
        state.touched.in_place.push(item_index);
    }
}

pub(in crate::fixups) struct ItemApplyReport {
    pub(in crate::fixups) changed: bool,
    pub(in crate::fixups) planned: usize,
    pub(in crate::fixups) applied: usize,
    #[allow(dead_code)]
    pub(super) diagnostics: Vec<PlanDiagnostic<EditSetSite>>,
    pub(in crate::fixups) touched: TouchedItems,
}

fn rewrite_event(
    identity: &RuleCaseIdentity,
    target: &StatementRange,
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
