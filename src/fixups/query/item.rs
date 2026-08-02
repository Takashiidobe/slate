use std::collections::BTreeMap;

use crate::fixups::facts::walk;
use crate::fixups::facts::{AstPath, FixupFacts, PathSegment};
use crate::fixups::support::walk as mut_walk;
use crate::fixups::trace::{
    Pass, RewriteEvent, TraceLocation, TraceLogger, TraceSnippet, fact, function_path_location,
    path_location,
};
use crate::rust_ast::{Expr, ExternDecl, IndentStmt, Item, Program};

use super::plan::{
    EditTarget, Plan, PlanBuilder, PlanDiagnostic, PlanSite, PlannedEdit, TouchedItems,
};
use super::rewrite::{evidence_trace_fact, predicate_name, rejection_name};
use super::{
    BindingRef, CallRecord, CaseRejection, DefinitionKind, DefinitionLocation, DefinitionSite,
    Evidence, ExprRecipe, ExprSite, ExpressionRef, FunctionBodyRecipe, FunctionRef, Predicate,
    QueryContext, Rejection, RejectionReason, RuleCaseIdentity, RuleIdentity, StatementRange,
};

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(in crate::fixups) enum QueryDomain {
    Binding,
    Definition,
    Expression,
    Function,
    Statement,
}

pub(in crate::fixups) enum QueryItem<'snapshot> {
    Binding(BindingRef),
    Definition(&'snapshot DefinitionSite),
    Expression(ExpressionRef),
    Function(FunctionRef),
    Statement {
        site: StatementRef,
        tail: &'snapshot [IndentStmt],
    },
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord)]
pub(in crate::fixups) enum Anchor {
    Binding {
        item_index: usize,
        path: AstPath,
        name: String,
    },
    Definition(DefinitionLocation),
    Expression(ExprSite),
    Function {
        item_index: usize,
        name: String,
    },
    Statements(StatementRange),
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

pub(in crate::fixups) fn same_statement_container(declaration: &AstPath, other: &AstPath) -> bool {
    let Some((PathSegment::Stmt(_), declaration_parent)) = declaration.0.split_last() else {
        return false;
    };
    let Some((PathSegment::Stmt(_), other_parent)) = other.0.split_last() else {
        return false;
    };
    declaration_parent == other_parent
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
    type Capture: MatchCapture;

    fn domain(&self) -> QueryDomain;
    fn matches(&self, query: &QueryContext<'_>, item: &QueryItem<'_>) -> Option<Self::Capture>;
}

pub(in crate::fixups) trait MatchCapture: Clone {
    fn anchor(&self) -> Anchor;

    fn evidence(&self) -> Vec<Evidence> {
        Vec::new()
    }
}

impl MatchCapture for DefinitionSite {
    fn anchor(&self) -> Anchor {
        Anchor::Definition(self.location.clone())
    }
}

impl MatchCapture for ExpressionRef {
    fn anchor(&self) -> Anchor {
        Anchor::Expression(self.site.clone())
    }
}

impl MatchCapture for CallRecord {
    fn anchor(&self) -> Anchor {
        Anchor::Expression(self.site.clone())
    }

    fn evidence(&self) -> Vec<Evidence> {
        self.evidence.clone()
    }
}

impl MatchCapture for FunctionRef {
    fn anchor(&self) -> Anchor {
        Anchor::Function {
            item_index: self.item_index,
            name: self.function.name.clone(),
        }
    }
}

impl MatchCapture for BindingRef {
    fn anchor(&self) -> Anchor {
        Anchor::Binding {
            item_index: self.item_index,
            path: self.definition.clone(),
            name: self.name.clone(),
        }
    }
}

impl MatchCapture for StatementMatch {
    fn anchor(&self) -> Anchor {
        Anchor::Statements(self.target.clone())
    }
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

impl<'snapshot> ItemCaseContext<'_, 'snapshot> {
    pub(in crate::fixups) fn fact<T>(
        &mut self,
        query: impl FnOnce(&QueryContext<'snapshot>) -> super::QueryResult<T>,
    ) -> Result<T, Rejection> {
        self.prove(query(self.query))
    }

    pub(in crate::fixups) fn attempt<T>(
        &mut self,
        result: Result<T, Rejection>,
    ) -> Result<T, Rejection> {
        match result {
            Ok(value) => Ok(value),
            Err(mut rejection) => {
                let mut evidence = self.evidence.clone();
                evidence.append(&mut rejection.evidence);
                rejection.evidence = evidence;
                Err(rejection)
            }
        }
    }

    pub(in crate::fixups) fn expr(&self, site: &ExprSite) -> Option<&Expr> {
        self.query.expr(site)
    }

    pub(in crate::fixups) fn call_args<const N: usize>(&self, call: &CallRecord) -> [ExprSite; N] {
        assert_eq!(N, call.args.len());
        std::array::from_fn(|index| call.args[index].clone())
    }

    pub(in crate::fixups) fn lower_expr(
        &mut self,
        recipe: ExprRecipe<'snapshot>,
        site: &ExprSite,
    ) -> Result<Expr, Rejection> {
        self.attempt(recipe.lower(self.query, site))
    }

    pub(in crate::fixups) fn function_body(&self, definition: &DefinitionSite) -> Vec<IndentStmt> {
        match &self.query.snapshot_program().items[definition.location.item_index()] {
            Item::Fn(function) => function.body.clone(),
            _ => Vec::new(),
        }
    }

    pub(in crate::fixups) fn require(&self, condition: bool) -> Result<(), Rejection> {
        if condition {
            Ok(())
        } else {
            Err(self.reject())
        }
    }

    pub(in crate::fixups) fn require_at(
        &self,
        condition: bool,
        predicate: Predicate,
        site: &ExprSite,
    ) -> Result<(), Rejection> {
        if condition {
            Ok(())
        } else {
            Err(self.reject_at(predicate, site, RejectionReason::Contradicted))
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

    pub(in crate::fixups) fn reject_at(
        &self,
        predicate: Predicate,
        site: &ExprSite,
        reason: RejectionReason,
    ) -> Rejection {
        Rejection::new(predicate, Some(site.clone()), reason, self.evidence.clone())
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

#[allow(clippy::large_enum_variant)]
pub(in crate::fixups) enum AnchoredEdit {
    DeleteDefinition {
        target: DefinitionSite,
    },
    ReplaceFunctionBody {
        target: DefinitionSite,
        body: Vec<IndentStmt>,
    },
    ReplaceExpression {
        target: ExprSite,
        replacement: Expr,
    },
    ReplaceStatement {
        target: StatementRange,
        replacement: Option<crate::rust_ast::Stmt>,
    },
    RemoveCallArgument {
        target: ExprSite,
        index: usize,
        expected_arity: usize,
    },
    RemoveFunctionParameter {
        target: FunctionRef,
        index: usize,
        name: String,
    },
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
    pub(in crate::fixups) fn new() -> Self {
        Self {
            edits: Vec::new(),
            evidence: Vec::new(),
        }
    }

    pub(in crate::fixups) fn replace_expression(target: ExprSite, replacement: Expr) -> Self {
        Self {
            edits: vec![AnchoredEdit::ReplaceExpression {
                target,
                replacement,
            }],
            evidence: Vec::new(),
        }
    }

    pub(in crate::fixups) fn push_replace_expression(
        &mut self,
        target: ExprSite,
        replacement: Expr,
    ) {
        self.edits.push(AnchoredEdit::ReplaceExpression {
            target,
            replacement,
        });
    }

    pub(in crate::fixups) fn push_replace_statement(
        &mut self,
        item_index: usize,
        path: AstPath,
        replacement: Option<crate::rust_ast::Stmt>,
    ) {
        let mut container = path.0;
        let Some(PathSegment::Stmt(index)) = container.pop() else {
            unreachable!()
        };
        self.edits.push(AnchoredEdit::ReplaceStatement {
            target: StatementRange {
                item_index,
                path: AstPath(container),
                start: index,
                end: index + 1,
            },
            replacement,
        });
    }

    pub(in crate::fixups) fn remove_parameter(removal: super::ParameterRemoval) -> Self {
        let expected_arity = removal.function.function.params.len();
        let mut edits = removal
            .calls
            .into_iter()
            .map(|target| AnchoredEdit::RemoveCallArgument {
                target,
                index: removal.index,
                expected_arity,
            })
            .collect::<Vec<_>>();
        edits.push(AnchoredEdit::RemoveFunctionParameter {
            target: removal.function,
            index: removal.index,
            name: removal.binding.name,
        });
        Self {
            edits,
            evidence: Vec::new(),
        }
    }

    pub(in crate::fixups) fn delete_definition(target: DefinitionSite) -> Self {
        Self {
            edits: vec![AnchoredEdit::DeleteDefinition { target }],
            evidence: Vec::new(),
        }
    }

    pub(in crate::fixups) fn replace_function_body(
        target: DefinitionSite,
        body: FunctionBodyRecipe,
    ) -> Self {
        Self {
            edits: vec![AnchoredEdit::ReplaceFunctionBody {
                target,
                body: body.lower(),
            }],
            evidence: Vec::new(),
        }
    }

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
pub(in crate::fixups) struct EditSetSite(Vec<Anchor>);

impl EditTarget for EditSet {
    type Site = EditSetSite;

    fn site(&self) -> Self::Site {
        let mut sites = self
            .edits
            .iter()
            .map(|edit| match edit {
                AnchoredEdit::DeleteDefinition { target }
                | AnchoredEdit::ReplaceFunctionBody { target, .. } => {
                    Anchor::Definition(target.location.clone())
                }
                AnchoredEdit::RemoveCallArgument { target, .. } => {
                    Anchor::Expression(target.clone())
                }
                AnchoredEdit::ReplaceExpression { target, .. } => {
                    Anchor::Expression(target.clone())
                }
                AnchoredEdit::RemoveFunctionParameter { target, .. } => Anchor::Function {
                    item_index: target.item_index,
                    name: target.function.name.clone(),
                },
                AnchoredEdit::ReplaceStatements { target, .. } => {
                    Anchor::Statements(target.clone())
                }
                AnchoredEdit::ReplaceStatement { target, .. } => Anchor::Statements(target.clone()),
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
            .any(|left| other.0.iter().any(|right| anchors_overlap(left, right)))
    }

    fn internal_overlaps(&self) -> Vec<(Self, Self)> {
        let mut overlaps = Vec::new();
        for first in 0..self.0.len() {
            for second in first + 1..self.0.len() {
                if anchors_overlap(&self.0[first], &self.0[second]) {
                    overlaps.push((
                        Self(vec![self.0[first].clone()]),
                        Self(vec![self.0[second].clone()]),
                    ));
                }
            }
        }
        overlaps
    }
}

fn anchors_overlap(left: &Anchor, right: &Anchor) -> bool {
    match (left, right) {
        (Anchor::Expression(left), Anchor::Expression(right)) => expression_overlap(left, right),
        (Anchor::Statements(left), Anchor::Statements(right)) => left.overlaps(right),
        (Anchor::Definition(left), Anchor::Definition(right)) => definitions_overlap(left, right),
        (Anchor::Definition(definition), Anchor::Statements(statements))
        | (Anchor::Statements(statements), Anchor::Definition(definition)) => {
            definition.item_index() == statements.item_index
        }
        (Anchor::Function { item_index, .. }, Anchor::Definition(definition))
        | (Anchor::Definition(definition), Anchor::Function { item_index, .. }) => {
            *item_index == definition.item_index()
        }
        (Anchor::Function { item_index, .. }, Anchor::Statements(statements))
        | (Anchor::Statements(statements), Anchor::Function { item_index, .. }) => {
            *item_index == statements.item_index
        }
        (Anchor::Expression(expression), Anchor::Statements(statements))
        | (Anchor::Statements(statements), Anchor::Expression(expression)) => {
            expression.item_index == statements.item_index
                && expression_within_statements(expression, statements)
        }
        (Anchor::Expression(expression), Anchor::Definition(definition))
        | (Anchor::Definition(definition), Anchor::Expression(expression)) => {
            expression.item_index == definition.item_index()
        }
        (Anchor::Expression(expression), Anchor::Function { item_index, .. })
        | (Anchor::Function { item_index, .. }, Anchor::Expression(expression)) => {
            expression.item_index == *item_index
        }
        (Anchor::Binding { item_index, .. }, other)
        | (other, Anchor::Binding { item_index, .. }) => {
            anchor_item_index(other) == Some(*item_index)
        }
        (
            Anchor::Function {
                item_index: left, ..
            },
            Anchor::Function {
                item_index: right, ..
            },
        ) => left == right,
    }
}

fn anchor_item_index(anchor: &Anchor) -> Option<usize> {
    match anchor {
        Anchor::Binding { item_index, .. }
        | Anchor::Expression(ExprSite { item_index, .. })
        | Anchor::Function { item_index, .. }
        | Anchor::Statements(StatementRange { item_index, .. }) => Some(*item_index),
        Anchor::Definition(definition) => Some(definition.item_index()),
    }
}

fn expression_overlap(left: &ExprSite, right: &ExprSite) -> bool {
    left.item_index == right.item_index
        && (left.path.0.starts_with(&right.path.0) || right.path.0.starts_with(&left.path.0))
}

fn expression_within_statements(expression: &ExprSite, statements: &StatementRange) -> bool {
    expression.path.0.starts_with(&statements.path.0)
        && matches!(expression.path.0.get(statements.path.0.len()), Some(PathSegment::Stmt(index)) if (statements.start..statements.end).contains(index))
}

fn definitions_overlap(left: &DefinitionLocation, right: &DefinitionLocation) -> bool {
    match (left, right) {
        (DefinitionLocation::Item(item), DefinitionLocation::ExternDecl { item_index, .. })
        | (DefinitionLocation::ExternDecl { item_index, .. }, DefinitionLocation::Item(item)) => {
            item == item_index
        }
        _ => left == right,
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
        for item in query_items(query, rule.matcher.domain()) {
            let Some(capture) = rule.matcher.matches(query, &item) else {
                continue;
            };
            let mut rejected_cases = Vec::new();
            let mut selected = None;
            for case in &rule.cases {
                let mut context = ItemCaseContext {
                    query,
                    evidence: capture.evidence(),
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
                    target: Some(EditSetSite(vec![capture.anchor()])),
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

fn query_items<'query>(
    query: &'query QueryContext<'_>,
    domain: QueryDomain,
) -> Vec<QueryItem<'query>> {
    if domain == QueryDomain::Definition {
        return query.all_definitions().map(QueryItem::Definition).collect();
    }
    if domain == QueryDomain::Function {
        return query
            .all_functions()
            .into_iter()
            .map(QueryItem::Function)
            .collect();
    }
    if domain == QueryDomain::Binding {
        return query
            .all_bindings()
            .into_iter()
            .map(QueryItem::Binding)
            .collect();
    }
    if domain == QueryDomain::Expression {
        return query
            .expression_sites()
            .into_iter()
            .map(|site| QueryItem::Expression(ExpressionRef { site }))
            .collect();
    }
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
        if logger.is_enabled() {
            for diagnostic in &diagnostics {
                log_diagnostic(logger, diagnostic);
            }
        }
        let mut accepted = Vec::new();
        for planned_edit in self.plan.edits {
            let missing = missing_targets(program, &planned_edit.edit);
            if missing.is_empty() && !planned_edit.edit.edits.is_empty() {
                accepted.push(planned_edit);
            } else {
                diagnostics.extend(missing.into_iter().map(|target| {
                    PlanDiagnostic::MissingTarget {
                        contender: planned_edit.identity.clone(),
                        target: EditSetSite(vec![target]),
                    }
                }));
            }
        }
        let accepted_sets = accepted.len();
        let expected_edits: usize = accepted.iter().map(|edit| edit.edit.edits.len()).sum();
        let mut statement_edits = BTreeMap::new();
        let mut call_edits = BTreeMap::new();
        let mut expression_edits = BTreeMap::new();
        let mut parameter_edits = Vec::new();
        let mut definition_edits = Vec::new();
        for planned_edit in accepted {
            let PlannedEdit {
                identity,
                edit,
                rejected_cases,
            } = planned_edit;
            for anchored in edit.edits {
                match anchored {
                    AnchoredEdit::DeleteDefinition { target } => {
                        definition_edits.push(PlannedDefinitionEdit {
                            identity: identity.clone(),
                            target,
                            action: DefinitionAction::Delete,
                            evidence: edit.evidence.clone(),
                            rejected_cases: rejected_cases.clone(),
                        });
                    }
                    AnchoredEdit::ReplaceFunctionBody { target, body } => {
                        definition_edits.push(PlannedDefinitionEdit {
                            identity: identity.clone(),
                            target,
                            action: DefinitionAction::ReplaceBody(body),
                            evidence: edit.evidence.clone(),
                            rejected_cases: rejected_cases.clone(),
                        });
                    }
                    AnchoredEdit::RemoveCallArgument {
                        target,
                        index,
                        expected_arity,
                    } => {
                        call_edits.insert(
                            (target.item_index, target.path.clone()),
                            PlannedCallEdit {
                                identity: identity.clone(),
                                target,
                                index,
                                expected_arity,
                                evidence: edit.evidence.clone(),
                                rejected_cases: rejected_cases.clone(),
                            },
                        );
                    }
                    AnchoredEdit::ReplaceExpression {
                        target,
                        replacement,
                    } => {
                        expression_edits.insert(
                            (target.item_index, target.path.clone()),
                            PlannedExpressionEdit {
                                identity: identity.clone(),
                                target,
                                replacement,
                                evidence: edit.evidence.clone(),
                                rejected_cases: rejected_cases.clone(),
                            },
                        );
                    }
                    AnchoredEdit::RemoveFunctionParameter {
                        target,
                        index,
                        name,
                    } => {
                        parameter_edits.push(PlannedParameterEdit {
                            identity: identity.clone(),
                            target,
                            index,
                            name,
                            evidence: edit.evidence.clone(),
                            rejected_cases: rejected_cases.clone(),
                        });
                    }
                    AnchoredEdit::ReplaceStatements {
                        target,
                        replacement,
                    } => {
                        statement_edits.insert(
                            target.clone(),
                            PlannedStatementEdit {
                                identity: identity.clone(),
                                target,
                                replacement,
                                preserve_depth: false,
                                evidence: edit.evidence.clone(),
                                rejected_cases: rejected_cases.clone(),
                            },
                        );
                    }
                    AnchoredEdit::ReplaceStatement {
                        target,
                        replacement,
                    } => {
                        statement_edits.insert(
                            target.clone(),
                            PlannedStatementEdit {
                                identity: identity.clone(),
                                target,
                                replacement: replacement
                                    .map(|stmt| vec![IndentStmt { depth: 0, stmt }])
                                    .unwrap_or_default(),
                                preserve_depth: true,
                                evidence: edit.evidence.clone(),
                                rejected_cases: rejected_cases.clone(),
                            },
                        );
                    }
                }
            }
        }
        let mut updated = program.clone();
        let mut applied_edits = 0;
        let mut touched = TouchedItems::none();
        applied_edits += apply_call_edits(
            &mut updated,
            call_edits,
            facts,
            logger,
            &mut diagnostics,
            &mut touched,
        );
        applied_edits += apply_expression_edits(
            &mut updated,
            expression_edits,
            facts,
            logger,
            &mut diagnostics,
            &mut touched,
        );
        let (statement_applied, statement_touched, missing_statements) = {
            let mut state = ApplyState {
                edits: statement_edits,
                applied: 0,
                touched: TouchedItems::none(),
                facts,
                logger,
            };
            for (item_index, item) in updated.items.iter_mut().enumerate() {
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
            (state.applied, state.touched, state.edits)
        };
        applied_edits += statement_applied;
        touched.in_place.extend(statement_touched.in_place);
        touched.removed.extend(statement_touched.removed);
        touched.unbounded |= statement_touched.unbounded;
        for (_, edit) in missing_statements {
            diagnostics.push(PlanDiagnostic::MissingTarget {
                contender: edit.identity,
                target: EditSetSite(vec![Anchor::Statements(edit.target)]),
            });
        }
        applied_edits += apply_parameter_edits(
            &mut updated,
            parameter_edits,
            logger,
            &mut diagnostics,
            &mut touched,
        );
        applied_edits += apply_definition_edits(
            &mut updated,
            definition_edits,
            logger,
            &mut diagnostics,
            &mut touched,
        );
        touched.in_place.sort_unstable();
        touched.in_place.dedup();
        touched.removed.sort_unstable();
        touched.removed.dedup();
        let applied = if applied_edits == expected_edits {
            *program = updated;
            accepted_sets
        } else {
            touched = TouchedItems::none();
            0
        };
        ItemApplyReport {
            changed: applied != 0,
            planned,
            applied,
            diagnostics,
            touched,
        }
    }
}

fn missing_targets(program: &Program, edit_set: &EditSet) -> Vec<Anchor> {
    edit_set
        .edits
        .iter()
        .filter(|edit| !anchored_edit_exists(program, edit))
        .map(edit_anchor)
        .collect()
}

fn edit_anchor(edit: &AnchoredEdit) -> Anchor {
    match edit {
        AnchoredEdit::DeleteDefinition { target }
        | AnchoredEdit::ReplaceFunctionBody { target, .. } => {
            Anchor::Definition(target.location.clone())
        }
        AnchoredEdit::ReplaceExpression { target, .. }
        | AnchoredEdit::RemoveCallArgument { target, .. } => Anchor::Expression(target.clone()),
        AnchoredEdit::ReplaceStatement { target, .. }
        | AnchoredEdit::ReplaceStatements { target, .. } => Anchor::Statements(target.clone()),
        AnchoredEdit::RemoveFunctionParameter { target, .. } => Anchor::Function {
            item_index: target.item_index,
            name: target.function.name.clone(),
        },
    }
}

fn anchored_edit_exists(program: &Program, edit: &AnchoredEdit) -> bool {
    match edit {
        AnchoredEdit::DeleteDefinition { target }
        | AnchoredEdit::ReplaceFunctionBody { target, .. } => definition_exists(program, target),
        AnchoredEdit::ReplaceExpression { target, .. } => {
            expression_matches(program, target, |_| true)
        }
        AnchoredEdit::RemoveCallArgument {
            target,
            index,
            expected_arity,
        } => expression_matches(
            program,
            target,
            |expr| matches!(expr, Expr::Call { args, .. } if args.len() == *expected_arity && *index < args.len()),
        ),
        AnchoredEdit::ReplaceStatement { target, .. }
        | AnchoredEdit::ReplaceStatements { target, .. } => statement_range_exists(program, target),
        AnchoredEdit::RemoveFunctionParameter {
            target,
            index,
            name,
        } => program.items.get(target.item_index).is_some_and(|item| {
            let Item::Fn(function) = unwrap_cfg(item) else {
                return false;
            };
            function.name == target.function.name
                && function
                    .params
                    .get(*index)
                    .is_some_and(|parameter| parameter.name == *name)
        }),
    }
}

fn definition_exists(program: &Program, definition: &DefinitionSite) -> bool {
    let Some(item) = program.items.get(definition.location.item_index()) else {
        return false;
    };
    match definition.location {
        DefinitionLocation::Item(_) => definition_matches(item, definition),
        DefinitionLocation::ExternDecl { decl_index, .. } => {
            let Item::ExternBlock { decls, .. } = unwrap_cfg(item) else {
                return false;
            };
            decls
                .get(decl_index)
                .is_some_and(|decl| extern_definition_matches(decl, definition))
        }
    }
}

fn expression_matches(
    program: &Program,
    target: &ExprSite,
    predicate: impl Fn(&Expr) -> bool,
) -> bool {
    let Some(Item::Fn(function)) = program.items.get(target.item_index) else {
        return false;
    };
    let mut matched = false;
    walk::body_exprs_with_path(&function.body, &mut Vec::new(), &mut |expr, path| {
        if path.as_slice() == target.path.0 && predicate(expr) {
            matched = true;
        }
    });
    matched
}

fn statement_range_exists(program: &Program, target: &StatementRange) -> bool {
    let Some(Item::Fn(function)) = program.items.get(target.item_index) else {
        return false;
    };
    body_range_exists(&function.body, &mut Vec::new(), target)
}

fn body_range_exists(
    body: &[IndentStmt],
    path: &mut Vec<PathSegment>,
    target: &StatementRange,
) -> bool {
    if path.as_slice() == target.path.0 {
        return target.start < target.end && target.end <= body.len();
    }
    let mut found = false;
    for (index, indent) in body.iter().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            walk::nested_body_vecs_with_path(&indent.stmt, path, &mut |nested, path| {
                found |= body_range_exists(nested, path, target);
            });
        });
    }
    found
}

struct PlannedCallEdit {
    identity: RuleCaseIdentity,
    target: ExprSite,
    index: usize,
    expected_arity: usize,
    evidence: Vec<Evidence>,
    rejected_cases: Vec<CaseRejection>,
}

struct PlannedExpressionEdit {
    identity: RuleCaseIdentity,
    target: ExprSite,
    replacement: Expr,
    evidence: Vec<Evidence>,
    rejected_cases: Vec<CaseRejection>,
}

fn apply_expression_edits(
    program: &mut Program,
    mut edits: BTreeMap<(usize, AstPath), PlannedExpressionEdit>,
    facts: &FixupFacts,
    logger: &mut dyn TraceLogger,
    diagnostics: &mut Vec<PlanDiagnostic<EditSetSite>>,
    touched: &mut TouchedItems,
) -> usize {
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
        mut_walk::body_exprs_mut_with_path(
            &mut function.body,
            &mut Vec::new(),
            &mut |expr, path| {
                let key = (item_index, AstPath(path.to_vec()));
                let Some(edit) = edits.remove(&key) else {
                    return true;
                };
                let before = std::mem::replace(expr, edit.replacement);
                log_item_edit(
                    logger,
                    &edit.identity,
                    &edit.evidence,
                    &edit.rejected_cases,
                    TraceChange {
                        location: facts
                            .function_by_item_index(item_index)
                            .map(|function| {
                                function_path_location(facts, function, &edit.target.path.0)
                            })
                            .unwrap_or_else(|| path_location(&edit.target.path.0)),
                        label: "expr",
                        before: before.render(),
                        after: expr.render(),
                    },
                );
                applied += 1;
                touched.in_place.push(item_index);
                false
            },
        );
    }
    diagnostics.extend(
        edits
            .into_values()
            .map(|edit| PlanDiagnostic::MissingTarget {
                contender: edit.identity,
                target: EditSetSite(vec![Anchor::Expression(edit.target)]),
            }),
    );
    applied
}

struct PlannedParameterEdit {
    identity: RuleCaseIdentity,
    target: FunctionRef,
    index: usize,
    name: String,
    evidence: Vec<Evidence>,
    rejected_cases: Vec<CaseRejection>,
}

fn apply_call_edits(
    program: &mut Program,
    mut edits: BTreeMap<(usize, AstPath), PlannedCallEdit>,
    facts: &FixupFacts,
    logger: &mut dyn TraceLogger,
    diagnostics: &mut Vec<PlanDiagnostic<EditSetSite>>,
    touched: &mut TouchedItems,
) -> usize {
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
        mut_walk::body_exprs_mut_with_path(
            &mut function.body,
            &mut Vec::new(),
            &mut |expr, path| {
                let site = ExprSite {
                    item_index,
                    path: AstPath(path.to_vec()),
                    fact_path: AstPath(path.to_vec()),
                };
                let Some(edit) = edits.remove(&(item_index, site.path.clone())) else {
                    return true;
                };
                let before = expr.render();
                let Expr::Call { args, .. } = expr else {
                    diagnostics.push(missing_call(edit));
                    return true;
                };
                if args.len() != edit.expected_arity || edit.index >= args.len() {
                    diagnostics.push(missing_call(edit));
                    return true;
                }
                args.remove(edit.index);
                let after = expr.render();
                log_item_edit(
                    logger,
                    &edit.identity,
                    &edit.evidence,
                    &edit.rejected_cases,
                    TraceChange {
                        location: facts
                            .function_by_item_index(item_index)
                            .map(|function| function_path_location(facts, function, &site.path.0))
                            .unwrap_or_else(|| path_location(&site.path.0)),
                        label: "expr",
                        before,
                        after,
                    },
                );
                applied += 1;
                touched.in_place.push(item_index);
                true
            },
        );
    }
    diagnostics.extend(edits.into_values().map(missing_call));
    applied
}

fn missing_call(edit: PlannedCallEdit) -> PlanDiagnostic<EditSetSite> {
    PlanDiagnostic::MissingTarget {
        contender: edit.identity,
        target: EditSetSite(vec![Anchor::Expression(edit.target)]),
    }
}

fn apply_parameter_edits(
    program: &mut Program,
    edits: Vec<PlannedParameterEdit>,
    logger: &mut dyn TraceLogger,
    diagnostics: &mut Vec<PlanDiagnostic<EditSetSite>>,
    touched: &mut TouchedItems,
) -> usize {
    let mut applied = 0;
    for edit in edits {
        let Some(item) = program.items.get_mut(edit.target.item_index) else {
            diagnostics.push(missing_parameter(edit));
            continue;
        };
        let Item::Fn(function) = unwrap_cfg_mut(item) else {
            diagnostics.push(missing_parameter(edit));
            continue;
        };
        if function.name != edit.target.function.name
            || !function
                .params
                .get(edit.index)
                .is_some_and(|parameter| parameter.name == edit.name)
        {
            diagnostics.push(missing_parameter(edit));
            continue;
        }
        let before = item_snippet(&Item::Fn(function.clone()));
        function.params.remove(edit.index);
        let after = item_snippet(&Item::Fn(function.clone()));
        log_item_edit(
            logger,
            &edit.identity,
            &edit.evidence,
            &edit.rejected_cases,
            TraceChange {
                location: TraceLocation {
                    function: Some(function.name.clone()),
                    ..TraceLocation::default()
                },
                label: "function",
                before,
                after,
            },
        );
        applied += 1;
        touched.in_place.push(edit.target.item_index);
    }
    applied
}

fn missing_parameter(edit: PlannedParameterEdit) -> PlanDiagnostic<EditSetSite> {
    PlanDiagnostic::MissingTarget {
        contender: edit.identity,
        target: EditSetSite(vec![Anchor::Function {
            item_index: edit.target.item_index,
            name: edit.target.function.name,
        }]),
    }
}

fn unwrap_cfg_mut(item: &mut Item) -> &mut Item {
    match item {
        Item::Cfg { item, .. } => unwrap_cfg_mut(item),
        _ => item,
    }
}

enum DefinitionAction {
    Delete,
    ReplaceBody(Vec<IndentStmt>),
}

struct PlannedDefinitionEdit {
    identity: RuleCaseIdentity,
    target: DefinitionSite,
    action: DefinitionAction,
    evidence: Vec<Evidence>,
    rejected_cases: Vec<CaseRejection>,
}

fn apply_definition_edits(
    program: &mut Program,
    edits: Vec<PlannedDefinitionEdit>,
    logger: &mut dyn TraceLogger,
    diagnostics: &mut Vec<PlanDiagnostic<EditSetSite>>,
    touched: &mut TouchedItems,
) -> usize {
    let mut by_item = BTreeMap::<usize, Vec<PlannedDefinitionEdit>>::new();
    for edit in edits {
        by_item
            .entry(edit.target.location.item_index())
            .or_default()
            .push(edit);
    }
    let mut applied = 0;
    for (item_index, mut edits) in by_item.into_iter().rev() {
        edits.sort_by(|left, right| right.target.location.cmp(&left.target.location));
        if item_index >= program.items.len() {
            diagnostics.extend(edits.into_iter().map(missing_definition));
            continue;
        }
        if matches!(
            edits.first().map(|edit| &edit.target.location),
            Some(DefinitionLocation::Item(_))
        ) {
            let edit = edits.pop().unwrap();
            if !definition_matches(&program.items[item_index], &edit.target) {
                diagnostics.push(missing_definition(edit));
                continue;
            }
            let before = item_snippet(&program.items[item_index]);
            match &edit.action {
                DefinitionAction::Delete => {
                    program.items.remove(item_index);
                    log_definition_edit(logger, &edit, before, None);
                    touched.removed.push(item_index);
                }
                DefinitionAction::ReplaceBody(body) => {
                    let Item::Fn(function) = &mut program.items[item_index] else {
                        unreachable!()
                    };
                    function.body = body.clone();
                    let after = item_snippet(&program.items[item_index]);
                    log_definition_edit(logger, &edit, before, Some(after));
                    touched.in_place.push(item_index);
                }
            }
            applied += 1;
            continue;
        }
        let Item::ExternBlock { abi, decls } = &mut program.items[item_index] else {
            diagnostics.extend(edits.into_iter().map(missing_definition));
            continue;
        };
        let mut any_removed = false;
        for edit in edits {
            let DefinitionLocation::ExternDecl { decl_index, .. } = edit.target.location else {
                unreachable!()
            };
            if !matches!(&edit.action, DefinitionAction::Delete)
                || !decls
                    .get(decl_index)
                    .is_some_and(|decl| extern_definition_matches(decl, &edit.target))
            {
                diagnostics.push(missing_definition(edit));
                continue;
            }
            let before = extern_decl_snippet(abi, &decls[decl_index]);
            decls.remove(decl_index);
            log_definition_edit(logger, &edit, before, None);
            applied += 1;
            any_removed = true;
        }
        if decls.is_empty() {
            program.items.remove(item_index);
            touched.removed.push(item_index);
        } else if any_removed {
            touched.in_place.push(item_index);
        }
    }
    applied
}

fn missing_definition(edit: PlannedDefinitionEdit) -> PlanDiagnostic<EditSetSite> {
    PlanDiagnostic::MissingTarget {
        contender: edit.identity,
        target: EditSetSite(vec![Anchor::Definition(edit.target.location)]),
    }
}

struct PlannedStatementEdit {
    identity: RuleCaseIdentity,
    target: StatementRange,
    replacement: Vec<IndentStmt>,
    preserve_depth: bool,
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
        let mut replacement = edit.replacement;
        if edit.preserve_depth && replacement.len() == 1 {
            replacement[0].depth = before[0].depth;
        }
        if state.logger.is_enabled() {
            state.logger.rewrite(rewrite_event(
                &edit.identity,
                &edit.target,
                &edit.evidence,
                &edit.rejected_cases,
                &before,
                &replacement,
                state.facts,
            ));
        }
        body.splice(edit.target.start..edit.target.end, replacement);
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

fn definition_matches(item: &Item, definition: &DefinitionSite) -> bool {
    let item = unwrap_cfg(item);
    match (item, definition.kind) {
        (Item::Fn(function), DefinitionKind::Function) => function.name == definition.name,
        (Item::SupportModule(module), DefinitionKind::SupportModule) => {
            module.name.as_str() == definition.name
        }
        (Item::Struct(definition_item), DefinitionKind::Struct) => {
            definition_item.name == definition.name
        }
        (Item::Record(definition_item), DefinitionKind::Record) => {
            definition_item.name == definition.name
        }
        (Item::Enum(definition_item), DefinitionKind::Enum) => {
            definition_item.name == definition.name
        }
        _ => false,
    }
}

fn unwrap_cfg(item: &Item) -> &Item {
    match item {
        Item::Cfg { item, .. } => unwrap_cfg(item),
        _ => item,
    }
}

fn extern_definition_matches(decl: &ExternDecl, definition: &DefinitionSite) -> bool {
    match (decl, definition.kind) {
        (ExternDecl::Fn(function), DefinitionKind::ExternFunction) => {
            function.name == definition.name
        }
        (ExternDecl::Static { name, .. }, DefinitionKind::ExternStatic) => name == &definition.name,
        _ => false,
    }
}

fn item_snippet(item: &Item) -> String {
    Program {
        items: vec![item.clone()],
    }
    .emit()
    .trim_end()
    .to_owned()
}

fn extern_decl_snippet(abi: &str, decl: &ExternDecl) -> String {
    item_snippet(&Item::ExternBlock {
        abi: abi.to_owned(),
        decls: vec![decl.clone()],
    })
}

fn log_definition_edit(
    logger: &mut dyn TraceLogger,
    edit: &PlannedDefinitionEdit,
    before: String,
    after: Option<String>,
) {
    if !logger.is_enabled() {
        return;
    }
    let mut facts = vec![
        fact("query_rule", edit.identity.rule.name.clone()),
        fact("query_case", edit.identity.case.clone()),
    ];
    facts.extend(edit.evidence.iter().map(evidence_trace_fact));
    facts.extend(edit.rejected_cases.iter().map(|rejected| {
        fact(
            format!("rejected_case.{}", rejected.case),
            format!(
                "{}:{}",
                predicate_name(rejected.rejection.predicate),
                rejection_name(rejected.rejection.reason)
            ),
        )
    }));
    logger.rewrite(RewriteEvent {
        pass: edit.identity.rule.pass,
        kind: edit.identity.rule.name.clone(),
        location: TraceLocation {
            function: Some(edit.target.name.clone()),
            ..TraceLocation::default()
        },
        before: vec![TraceSnippet::new("definition", before)],
        after: after
            .map(|after| vec![TraceSnippet::new("definition", after)])
            .unwrap_or_default(),
        facts,
    });
}

struct TraceChange {
    location: TraceLocation,
    label: &'static str,
    before: String,
    after: String,
}

fn log_item_edit(
    logger: &mut dyn TraceLogger,
    identity: &RuleCaseIdentity,
    evidence: &[Evidence],
    rejected_cases: &[CaseRejection],
    change: TraceChange,
) {
    if !logger.is_enabled() {
        return;
    }
    let mut facts = vec![
        fact("query_rule", identity.rule.name.clone()),
        fact("query_case", identity.case.clone()),
    ];
    facts.extend(evidence.iter().map(evidence_trace_fact));
    facts.extend(rejected_cases.iter().map(|rejected| {
        fact(
            format!("rejected_case.{}", rejected.case),
            format!(
                "{}:{}",
                predicate_name(rejected.rejection.predicate),
                rejection_name(rejected.rejection.reason)
            ),
        )
    }));
    logger.rewrite(RewriteEvent {
        pass: identity.rule.pass,
        kind: identity.rule.name.clone(),
        location: change.location,
        before: vec![TraceSnippet::new(change.label, change.before)],
        after: vec![TraceSnippet::new(change.label, change.after)],
        facts,
    });
}

fn log_diagnostic(logger: &mut dyn TraceLogger, diagnostic: &PlanDiagnostic<EditSetSite>) {
    let PlanDiagnostic::CandidateRejected {
        rule,
        target,
        rejections,
    } = diagnostic
    else {
        return;
    };
    let mut facts = vec![fact("query_rule", rule.name.clone())];
    for rejected in rejections {
        facts.push(fact(
            format!("rejected_case.{}", rejected.case),
            format!(
                "{}:{}",
                predicate_name(rejected.rejection.predicate),
                rejection_name(rejected.rejection.reason)
            ),
        ));
        facts.extend(rejected.rejection.evidence.iter().map(evidence_trace_fact));
    }
    logger.rewrite(RewriteEvent {
        pass: rule.pass,
        kind: rule.name.clone(),
        location: TraceLocation {
            function: Some("item".into()),
            ast_path: target.as_ref().map(|target| format!("{target:?}")),
            ..TraceLocation::default()
        },
        before: Vec::new(),
        after: Vec::new(),
        facts,
    });
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::facts;
    use crate::fixups::trace::NoopLogger;
    use crate::rust_ast::{FnDef, FunctionMetadata, RustValue, Stmt, Visibility};

    fn function(body: Vec<IndentStmt>) -> Item {
        Item::Fn(FnDef {
            attrs: Vec::new(),
            vis: Visibility::Private,
            unsafe_: false,
            abi: None,
            name: "test".into(),
            params: Vec::new(),
            ret: None,
            body,
            metadata: FunctionMetadata::default(),
        })
    }

    fn statement(value: i64) -> IndentStmt {
        IndentStmt {
            depth: 1,
            stmt: Stmt::Expr(Expr::Value(RustValue::I64(value))),
        }
    }

    fn expression_site(statement: usize) -> ExprSite {
        let path = AstPath(vec![PathSegment::Stmt(statement), PathSegment::Expr(0)]);
        ExprSite {
            item_index: 0,
            path: path.clone(),
            fact_path: path,
        }
    }

    fn identity() -> RuleCaseIdentity {
        RuleCaseIdentity {
            rule: RuleIdentity::new(Pass::StringLift, "test_rule"),
            case: "test_case".into(),
        }
    }

    fn item_plan(edit: EditSet) -> ItemPlan {
        item_plan_from(vec![edit])
    }

    fn item_plan_from(edits: Vec<EditSet>) -> ItemPlan {
        let mut builder = PlanBuilder::new();
        for edit in edits {
            builder.propose(PlannedEdit {
                identity: identity(),
                edit,
                rejected_cases: Vec::new(),
            });
        }
        ItemPlan {
            plan: builder.finish(),
        }
    }

    #[test]
    fn internal_duplicate_targets_are_rejected() {
        let target = expression_site(0);
        let mut edit = EditSet::replace_expression(target.clone(), Expr::Value(RustValue::I64(1)));
        edit.push_replace_expression(target, Expr::Value(RustValue::I64(2)));

        let plan = item_plan(edit);

        assert!(plan.plan.edits.is_empty());
        assert!(matches!(
            plan.plan.diagnostics.as_slice(),
            [PlanDiagnostic::OverlappingTargets { .. }]
        ));
    }

    #[test]
    fn internal_overlapping_targets_are_rejected() {
        let mut edit = EditSet::new();
        edit.push_replace_statement(0, AstPath(vec![PathSegment::Stmt(0)]), None);
        edit.push_replace_expression(expression_site(0), Expr::Value(RustValue::I64(2)));

        let plan = item_plan(edit);

        assert!(plan.plan.edits.is_empty());
        assert!(matches!(
            plan.plan.diagnostics.as_slice(),
            [PlanDiagnostic::OverlappingTargets { .. }]
        ));
    }

    #[test]
    fn missing_target_rejects_the_complete_edit_set() {
        let mut program = Program {
            items: vec![function(vec![statement(1), statement(2)])],
        };
        let before = program.emit();
        let mut edit =
            EditSet::replace_expression(expression_site(0), Expr::Value(RustValue::I64(2)));
        edit.push_replace_statement(0, AstPath(vec![PathSegment::Stmt(4)]), None);
        let valid = EditSet::replace_expression(expression_site(1), Expr::Value(RustValue::I64(3)));
        let plan = item_plan_from(vec![edit, valid]);
        let facts = facts::analyze(&program).facts;

        let report = plan.apply(&mut program, &facts, &mut NoopLogger);

        assert_ne!(program.emit(), before);
        assert_eq!(program.emit(), "fn test() {\n    1;\n    3;\n}\n");
        assert!(report.changed);
        assert_eq!(report.applied, 1);
        assert_eq!(report.touched.in_place, vec![0]);
        assert!(matches!(
            report.diagnostics.last(),
            Some(PlanDiagnostic::MissingTarget { .. })
        ));
    }

    #[test]
    fn mixed_edits_use_snapshot_paths_and_report_one_atomic_application() {
        let mut program = Program {
            items: vec![function(vec![statement(1), statement(2), statement(3)])],
        };
        let mut edit = EditSet::new();
        edit.push_replace_statement(0, AstPath(vec![PathSegment::Stmt(0)]), None);
        edit.push_replace_expression(expression_site(2), Expr::Value(RustValue::I64(4)));
        let plan = item_plan(edit);
        let facts = facts::analyze(&program).facts;

        let report = plan.apply(&mut program, &facts, &mut NoopLogger);

        assert_eq!(program.emit(), "fn test() {\n    2;\n    4;\n}\n");
        assert_eq!(report.applied, 1);
        assert_eq!(report.touched.in_place, vec![0]);
    }
}
