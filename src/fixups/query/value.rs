use std::collections::BTreeMap;

use crate::fixups::facts::walk;
use crate::fixups::facts::{
    AstPath, FixupFacts, PathSegment, StringBufferFact, StringRecoveryCandidate,
};
use crate::fixups::support::walk as mut_walk;
use crate::fixups::trace::{
    Pass, RewriteEvent, TraceLogger, TraceSnippet, fact, function_path_location, path_location,
};
use crate::rust_ast::{Expr, Item, Program, Stmt};

use super::plan::{
    EditTarget, Plan, PlanBuilder, PlanDiagnostic, PlanSite, PlannedEdit, TouchedItems,
};
use super::rewrite::{evidence_trace_fact, predicate_name, rejection_name};
use super::{
    ArrayElementPointerOrigin, CaseRejection, Evidence, ExprSite, Local, Predicate, QueryContext,
    Rejection, RejectionReason, ReplaceExpr, RuleCaseIdentity, RuleIdentity, ValueSite,
};

type ValueCaseFn = for<'case, 'snapshot> fn(
    &mut ValueCaseContext<'case, 'snapshot>,
) -> Result<Vec<ValueEdit>, Rejection>;

struct DeclarativeValueCase {
    name: String,
    apply: ValueCaseFn,
}

pub(in crate::fixups) struct ValueRule {
    identity: RuleIdentity,
    local: Local,
    cases: Vec<DeclarativeValueCase>,
}

impl ValueRule {
    pub(in crate::fixups) fn new(pass: Pass, rule: impl Into<String>, local: Local) -> Self {
        Self {
            identity: RuleIdentity::new(pass, rule),
            local,
            cases: Vec::new(),
        }
    }

    pub(in crate::fixups) fn case(mut self, name: impl Into<String>, apply: ValueCaseFn) -> Self {
        self.cases.push(DeclarativeValueCase {
            name: name.into(),
            apply,
        });
        self
    }

    fn candidates(&self, query: &QueryContext<'_>) -> Vec<ValueCandidate> {
        let mut candidates = Vec::new();
        for (item_index, item) in query.snapshot_program().items.iter().enumerate() {
            if let Item::Fn(function) = item {
                collect_lets(item_index, &function.body, &mut Vec::new(), &mut candidates);
            }
        }
        candidates
            .into_iter()
            .filter(|candidate| {
                self.local.name.matches(&candidate.name, &())
                    && self.local.mutable.matches(&candidate.mutable, &())
                    && self
                        .local
                        .value
                        .matches(&query.value_local(&candidate.site, &candidate.name), &())
            })
            .collect()
    }
}

struct ValueCandidate {
    site: ValueSite,
    name: String,
    mutable: bool,
}

fn collect_lets(
    item_index: usize,
    body: &[crate::rust_ast::IndentStmt],
    path: &mut Vec<PathSegment>,
    out: &mut Vec<ValueCandidate>,
) {
    for (index, indent) in body.iter().enumerate() {
        if let Stmt::Let { name, mutable, .. } = &indent.stmt {
            let mut let_path = path.clone();
            let_path.push(PathSegment::Stmt(index));
            out.push(ValueCandidate {
                site: ValueSite {
                    item_index,
                    path: AstPath(let_path),
                },
                name: name.clone(),
                mutable: *mutable,
            });
        }
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            walk::nested_body_vecs_with_path(&indent.stmt, path, &mut |nested, path| {
                collect_lets(item_index, nested, path, out);
            });
        });
    }
}

pub(in crate::fixups) struct ValueCaseContext<'case, 'snapshot> {
    query: &'case QueryContext<'snapshot>,
    candidate: &'case ValueCandidate,
    evidence: Vec<Evidence>,
}

impl ValueCaseContext<'_, '_> {
    pub(in crate::fixups) fn site(&self) -> AstPath {
        self.candidate.site.path.clone()
    }

    pub(in crate::fixups) fn name(&self) -> &str {
        &self.candidate.name
    }

    pub(in crate::fixups) fn expr(&self, site: &ExprSite) -> Option<&Expr> {
        self.query.expr(site)
    }

    pub(in crate::fixups) fn string_buffer(&mut self) -> Result<StringBufferFact, Rejection> {
        self.prove(self.query.string_buffer(&self.candidate.site))
    }

    pub(in crate::fixups) fn exprs(&mut self) -> Result<Vec<ExprSite>, Rejection> {
        self.prove(self.query.all_exprs(self.candidate.site.item_index))
    }

    pub(in crate::fixups) fn uses(&mut self) -> Result<Vec<ExprSite>, Rejection> {
        self.prove(
            self.query
                .value_uses(&self.candidate.site, &self.candidate.name),
        )
    }

    pub(in crate::fixups) fn string_pointer_view_sites(
        &mut self,
    ) -> Result<Vec<ExprSite>, Rejection> {
        self.prove(
            self.query
                .string_pointer_view_sites(&self.candidate.site, &self.candidate.name),
        )
    }

    pub(in crate::fixups) fn use_allows_string_lift(
        &mut self,
        site: &ExprSite,
        recovery: StringRecoveryCandidate,
    ) -> Result<bool, Rejection> {
        self.prove(self.query.string_use_allows_lift(
            &self.candidate.site,
            &self.candidate.name,
            site,
            recovery,
        ))
    }

    pub(in crate::fixups) fn pointer_origin(
        &mut self,
    ) -> Result<ArrayElementPointerOrigin, Rejection> {
        self.pointer_origin_named(&self.candidate.name.clone())
    }

    pub(in crate::fixups) fn pointer_origin_named(
        &mut self,
        name: &str,
    ) -> Result<ArrayElementPointerOrigin, Rejection> {
        self.prove(self.query.pointer_origin(&self.candidate.site, name))
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
            Predicate::ValueGuard,
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

pub(in crate::fixups) enum ValueEdit {
    Stmt {
        path: AstPath,
        replacement: Option<Box<Stmt>>,
    },
    Expr(ReplaceExpr),
}

impl ValueEdit {
    pub(in crate::fixups) fn replace_stmt(path: AstPath, stmt: Stmt) -> Self {
        Self::Stmt {
            path,
            replacement: Some(Box::new(stmt)),
        }
    }

    pub(in crate::fixups) fn remove_stmt(path: AstPath) -> Self {
        Self::Stmt {
            path,
            replacement: None,
        }
    }

    pub(in crate::fixups) fn replace_expr(site: ExprSite, expr: Expr) -> Self {
        Self::Expr(ReplaceExpr::new(site, expr))
    }

    fn edit_site(&self, item_index: usize) -> ValueEditSite {
        match self {
            Self::Stmt { path, .. } => ValueEditSite::Stmt {
                item_index,
                path: path.clone(),
            },
            Self::Expr(replace) => ValueEditSite::Expr(replace.target.clone()),
        }
    }
}

pub(in crate::fixups) fn same_container(declaration: &AstPath, other: &AstPath) -> bool {
    let Some((PathSegment::Stmt(_), decl_parent)) = declaration.0.split_last() else {
        return false;
    };
    let Some((PathSegment::Stmt(_), other_parent)) = other.0.split_last() else {
        return false;
    };
    decl_parent == other_parent
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord)]
enum ValueEditSite {
    Stmt { item_index: usize, path: AstPath },
    Expr(ExprSite),
}

impl PlanSite for ValueEditSite {
    fn overlaps(&self, other: &Self) -> bool {
        match (self, other) {
            (
                Self::Stmt {
                    item_index: a,
                    path: pa,
                },
                Self::Stmt {
                    item_index: b,
                    path: pb,
                },
            ) => a == b && pa == pb,
            (Self::Expr(a), Self::Expr(b)) => a.overlaps(b),
            (Self::Stmt { item_index, path }, Self::Expr(expr_site))
            | (Self::Expr(expr_site), Self::Stmt { item_index, path }) => {
                *item_index == expr_site.item_index && path_starts_with(&expr_site.path.0, &path.0)
            }
        }
    }
}

fn path_starts_with(path: &[PathSegment], prefix: &[PathSegment]) -> bool {
    path.len() >= prefix.len() && path[..prefix.len()] == *prefix
}

struct ValuePlanEdit {
    target: ValueEditSite,
    action: ValueEdit,
    evidence: Vec<Evidence>,
}

impl EditTarget for ValuePlanEdit {
    type Site = ValueEditSite;

    fn site(&self) -> ValueEditSite {
        self.target.clone()
    }
}

pub(in crate::fixups) struct ValuePlanBuilder {
    builder: PlanBuilder<ValuePlanEdit>,
}

impl ValuePlanBuilder {
    pub(in crate::fixups) fn new() -> Self {
        Self {
            builder: PlanBuilder::new(),
        }
    }

    pub(in crate::fixups) fn add_rule(
        &mut self,
        query: &QueryContext<'_>,
        rule: &ValueRule,
    ) -> &mut Self {
        let identity = rule.identity.clone();
        for candidate in rule.candidates(query) {
            let mut rejected_cases = Vec::new();
            let mut selected: Option<Vec<PlannedEdit<ValuePlanEdit>>> = None;
            for case in &rule.cases {
                let mut context = ValueCaseContext {
                    query,
                    candidate: &candidate,
                    evidence: Vec::new(),
                };
                match (case.apply)(&mut context) {
                    Ok(edits) => {
                        let case_identity = RuleCaseIdentity {
                            rule: identity.clone(),
                            case: case.name.clone(),
                        };
                        let taken_rejections = std::mem::take(&mut rejected_cases);
                        selected = Some(
                            edits
                                .into_iter()
                                .map(|edit| {
                                    let target = edit.edit_site(candidate.site.item_index);
                                    PlannedEdit {
                                        identity: case_identity.clone(),
                                        edit: ValuePlanEdit {
                                            target,
                                            action: edit,
                                            evidence: context.evidence.clone(),
                                        },
                                        rejected_cases: taken_rejections.clone(),
                                    }
                                })
                                .collect(),
                        );
                        break;
                    }
                    Err(rejection) => rejected_cases.push(CaseRejection {
                        case: case.name.clone(),
                        rejection,
                    }),
                }
            }
            if let Some(edits) = selected {
                for edit in edits {
                    self.builder.propose(edit);
                }
            } else if !rejected_cases.is_empty() {
                self.builder.diagnose(PlanDiagnostic::CandidateRejected {
                    rule: identity.clone(),
                    target: Some(ValueEditSite::Stmt {
                        item_index: candidate.site.item_index,
                        path: candidate.site.path.clone(),
                    }),
                    rejections: rejected_cases,
                });
            }
        }
        self
    }

    pub(in crate::fixups) fn finish(self) -> ValuePlan {
        ValuePlan {
            plan: self.builder.finish(),
        }
    }
}

pub(in crate::fixups) struct ValuePlan {
    plan: Plan<ValuePlanEdit>,
}

impl ValuePlan {
    pub(in crate::fixups) fn apply(
        self,
        program: &mut Program,
        facts: &FixupFacts,
        logger: &mut dyn TraceLogger,
    ) -> ValueApplyReport {
        let planned = self.plan.edits.len();
        let mut diagnostics = self.plan.diagnostics;
        let mut expr_edits = BTreeMap::new();
        let mut stmt_edits = BTreeMap::new();
        for edit in self.plan.edits {
            match &edit.edit.target {
                ValueEditSite::Expr(site) => {
                    expr_edits.insert((site.item_index, site.path.clone()), edit);
                }
                ValueEditSite::Stmt { item_index, path } => {
                    stmt_edits.insert((*item_index, path.clone()), edit);
                }
            }
        }
        let mut state = ApplyState {
            expr_edits,
            stmt_edits,
            applied: 0,
            touched: TouchedItems::none(),
            facts,
            logger,
        };
        for (item_index, item) in program.items.iter_mut().enumerate() {
            let Item::Fn(function) = item else {
                continue;
            };
            apply_expr_edits(item_index, &mut function.body, &mut state);
        }
        for (item_index, item) in program.items.iter_mut().enumerate() {
            let Item::Fn(function) = item else {
                continue;
            };
            if !state
                .stmt_edits
                .keys()
                .any(|(target_item, _)| *target_item == item_index)
            {
                continue;
            }
            apply_stmt_edits(item_index, &mut function.body, &mut Vec::new(), &mut state);
        }
        for (_, edit) in state.expr_edits {
            diagnostics.push(PlanDiagnostic::MissingTarget {
                contender: edit.identity,
                target: edit.edit.target,
            });
        }
        for (_, edit) in state.stmt_edits {
            diagnostics.push(PlanDiagnostic::MissingTarget {
                contender: edit.identity,
                target: edit.edit.target,
            });
        }
        ValueApplyReport {
            changed: state.applied != 0,
            planned,
            applied: state.applied,
            diagnostics,
            touched: state.touched,
        }
    }
}

type ExprEditMap = BTreeMap<(usize, AstPath), PlannedEdit<ValuePlanEdit>>;
type StmtEditMap = BTreeMap<(usize, AstPath), PlannedEdit<ValuePlanEdit>>;

struct ApplyState<'a> {
    expr_edits: ExprEditMap,
    stmt_edits: StmtEditMap,
    applied: usize,
    touched: TouchedItems,
    facts: &'a FixupFacts,
    logger: &'a mut dyn TraceLogger,
}

fn apply_expr_edits(
    item_index: usize,
    body: &mut [crate::rust_ast::IndentStmt],
    state: &mut ApplyState<'_>,
) {
    if !state
        .expr_edits
        .keys()
        .any(|(target_item, _)| *target_item == item_index)
    {
        return;
    }
    mut_walk::body_exprs_mut_with_path(body, &mut Vec::new(), &mut |expr, path| {
        let key = (item_index, AstPath(path.to_vec()));
        let Some(edit) = state.expr_edits.remove(&key) else {
            return true;
        };
        let PlannedEdit {
            identity,
            edit: ValuePlanEdit {
                action, evidence, ..
            },
            rejected_cases,
        } = edit;
        let ValueEdit::Expr(ReplaceExpr {
            target: expr_target,
            replacement,
            ..
        }) = action
        else {
            unreachable!()
        };
        let before = std::mem::replace(expr, replacement);
        if state.logger.is_enabled() {
            state.logger.rewrite(expr_rewrite_event(
                &identity,
                &expr_target,
                &evidence,
                &rejected_cases,
                &before,
                expr,
                state.facts,
            ));
        }
        state.applied += 1;
        state.touched.in_place.push(item_index);
        false
    });
}

fn apply_stmt_edits(
    item_index: usize,
    body: &mut Vec<crate::rust_ast::IndentStmt>,
    path: &mut Vec<PathSegment>,
    state: &mut ApplyState<'_>,
) {
    for (index, indent) in body.iter_mut().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            mut_walk::nested_body_vecs_mut_with_path(
                &mut indent.stmt,
                path,
                &mut |nested, path| {
                    apply_stmt_edits(item_index, nested, path, state);
                },
            );
        });
    }
    let mut here_indices = (0..body.len())
        .filter(|&index| {
            let mut stmt_path = path.clone();
            stmt_path.push(PathSegment::Stmt(index));
            state
                .stmt_edits
                .contains_key(&(item_index, AstPath(stmt_path)))
        })
        .collect::<Vec<_>>();
    here_indices.sort_unstable_by(|a, b| b.cmp(a));
    for index in here_indices {
        let mut stmt_path = path.clone();
        stmt_path.push(PathSegment::Stmt(index));
        let key = (item_index, AstPath(stmt_path));
        let Some(edit) = state.stmt_edits.remove(&key) else {
            continue;
        };
        let PlannedEdit {
            identity,
            edit: ValuePlanEdit {
                action, evidence, ..
            },
            rejected_cases,
        } = edit;
        let ValueEdit::Stmt {
            path: stmt_path,
            replacement,
        } = action
        else {
            unreachable!()
        };
        let before = body[index].stmt.clone();
        let after = replacement.as_deref().cloned();
        match replacement {
            Some(stmt) => {
                body[index].stmt = *stmt;
            }
            None => {
                body.remove(index);
            }
        }
        if state.logger.is_enabled() {
            let site = ValueSite {
                item_index,
                path: stmt_path,
            };
            state.logger.rewrite(stmt_rewrite_event(
                &identity,
                &site,
                &evidence,
                &rejected_cases,
                &before,
                after.as_ref(),
                state.facts,
            ));
        }
        state.applied += 1;
        state.touched.in_place.push(item_index);
    }
}

pub(in crate::fixups) struct ValueApplyReport {
    pub(in crate::fixups) changed: bool,
    pub(in crate::fixups) planned: usize,
    pub(in crate::fixups) applied: usize,
    #[allow(dead_code)]
    diagnostics: Vec<PlanDiagnostic<ValueEditSite>>,
    pub(in crate::fixups) touched: TouchedItems,
}

fn expr_rewrite_event(
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
    RewriteEvent {
        pass: identity.rule.pass,
        kind: identity.rule.name.clone(),
        location,
        before: vec![TraceSnippet::new("expr", before.render().trim_end())],
        after: vec![TraceSnippet::new("expr", after.render().trim_end())],
        facts: trace_facts(identity, evidence, rejected_cases),
    }
}

fn stmt_rewrite_event(
    identity: &RuleCaseIdentity,
    site: &ValueSite,
    evidence: &[Evidence],
    rejected_cases: &[CaseRejection],
    before: &Stmt,
    after: Option<&Stmt>,
    facts: &FixupFacts,
) -> RewriteEvent {
    let location = facts
        .function_by_item_index(site.item_index)
        .map(|function| function_path_location(facts, function, &site.path.0))
        .unwrap_or_else(|| path_location(&site.path.0));
    RewriteEvent {
        pass: identity.rule.pass,
        kind: identity.rule.name.clone(),
        location,
        before: vec![TraceSnippet::new("stmt", before.render().trim_end())],
        after: after
            .map(|stmt| vec![TraceSnippet::new("stmt", stmt.render().trim_end())])
            .unwrap_or_default(),
        facts: trace_facts(identity, evidence, rejected_cases),
    }
}

fn trace_facts(
    identity: &RuleCaseIdentity,
    evidence: &[Evidence],
    rejected_cases: &[CaseRejection],
) -> Vec<crate::fixups::trace::TraceFact> {
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
    trace_facts
}
