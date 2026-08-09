use std::collections::BTreeMap;

use crate::fixups::facts::walk;
use crate::fixups::facts::{AstPath, PathSegment};
use crate::fixups::salsa::SalsaFacts;
use crate::fixups::support::walk as mut_walk;
use crate::fixups::trace::{
    Pass, ProgramSummary, RewriteEvent, TraceLocation, TraceLogger, TraceSnippet, fact,
    function_path_location, path_location,
};
use crate::rust_ast::{Expr, ExternDecl, IndentStmt, Item, Program};

use super::plan::{EditTarget, Plan, PlanBuilder, PlanDiagnostic, PlanSite, PlannedEdit};
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
    EnumVariant,
    Expression,
    Field,
    Function,
    MatchArm,
    Parameter,
    Program,
    Statement,
    StatementContainer,
    TypeUse,
}

pub(in crate::fixups) enum QueryItem<'snapshot> {
    Binding(BindingRef<'snapshot>),
    Definition(&'snapshot DefinitionSite),
    #[expect(
        dead_code,
        reason = "query API surface not yet wired into a fixup rule"
    )]
    EnumVariant(super::EnumVariantRef),
    Expression(ExpressionRef),
    #[expect(
        dead_code,
        reason = "query API surface not yet wired into a fixup rule"
    )]
    Field(super::FieldRef),
    Function(FunctionRef<'snapshot>),
    #[expect(
        dead_code,
        reason = "query API surface not yet wired into a fixup rule"
    )]
    MatchArm(super::MatchArmRef),
    Parameter(super::ParameterRef<'snapshot>),
    Program(super::ProgramRef),
    Statement(StatementRef),
    #[expect(
        dead_code,
        reason = "query API surface not yet wired into a fixup rule"
    )]
    StatementContainer(super::StatementContainerRef),
    #[expect(
        dead_code,
        reason = "query API surface not yet wired into a fixup rule"
    )]
    TypeUse(super::TypeUseRef<'snapshot>),
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
    Items {
        index: usize,
        expected_len: usize,
    },
    Program {
        expected_len: usize,
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

    pub(in crate::fixups) fn container(&self) -> Option<super::StatementContainerRef> {
        let mut path = self.path.0.clone();
        matches!(path.pop(), Some(PathSegment::Stmt(_))).then_some(super::StatementContainerRef {
            item_index: self.item_index,
            path: AstPath(path),
        })
    }

    pub(in crate::fixups) fn index(&self) -> Option<usize> {
        match self.path.0.last()? {
            PathSegment::Stmt(index) => Some(*index),
            _ => None,
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
pub(in crate::fixups) struct StatementMatch<const N: usize> {
    target: StatementRange,
}

impl<const N: usize> StatementMatch<N> {
    pub(in crate::fixups) fn new(target: StatementRange) -> Self {
        Self { target }
    }

    pub(in crate::fixups) fn target(&self) -> &StatementRange {
        &self.target
    }

    pub(in crate::fixups) fn statement(&self, offset: usize) -> StatementRef {
        assert!(offset < N);
        let mut path = self.target.path.0.clone();
        path.push(PathSegment::Stmt(self.target.start + offset));
        StatementRef {
            item_index: self.target.item_index,
            path: AstPath(path),
        }
    }
}

pub(in crate::fixups) trait Matcher {
    type Capture<'db>: MatchCapture;

    fn domain(&self) -> QueryDomain;
    fn matches<'db>(
        &self,
        query: &QueryContext<'_>,
        item: &QueryItem<'db>,
    ) -> Option<Self::Capture<'db>>;
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

impl<'db> MatchCapture for FunctionRef<'db> {
    fn anchor(&self) -> Anchor {
        Anchor::Function {
            item_index: self.item_index,
            name: self.name.clone(),
        }
    }
}

impl MatchCapture for super::ProgramRef {
    fn anchor(&self) -> Anchor {
        Anchor::Program {
            expected_len: self.expected_len,
        }
    }
}

impl<'db> MatchCapture for BindingRef<'db> {
    fn anchor(&self) -> Anchor {
        Anchor::Binding {
            item_index: self.item_index,
            path: self.definition.clone(),
            name: self.name.clone(),
        }
    }
}

impl<'db> MatchCapture for super::ParameterRef<'db> {
    fn anchor(&self) -> Anchor {
        self.binding.anchor()
    }
}

impl MatchCapture for super::StatementContainerRef {
    fn anchor(&self) -> Anchor {
        Anchor::Statements(StatementRange {
            item_index: self.item_index,
            path: self.path.clone(),
            start: 0,
            end: 0,
        })
    }
}

impl MatchCapture for super::MatchArmRef {
    fn anchor(&self) -> Anchor {
        Anchor::Statements(StatementRange {
            item_index: self.statement.item_index,
            path: {
                let mut path = self.statement.path.0.clone();
                path.push(PathSegment::MatchArm(self.index));
                AstPath(path)
            },
            start: 0,
            end: 0,
        })
    }
}

impl MatchCapture for super::FieldRef {
    fn anchor(&self) -> Anchor {
        Anchor::Definition(DefinitionLocation::Item(self.item_index))
    }
}

impl MatchCapture for super::EnumVariantRef {
    fn anchor(&self) -> Anchor {
        Anchor::Definition(DefinitionLocation::Item(self.item_index))
    }
}

impl<'db> MatchCapture for super::TypeUseRef<'db> {
    fn anchor(&self) -> Anchor {
        match self {
            super::TypeUseRef::FunctionReturn(function) => function.anchor(),
            super::TypeUseRef::Parameter(parameter) => parameter.anchor(),
            super::TypeUseRef::Field(field) => field.anchor(),
        }
    }
}

impl<const N: usize> MatchCapture for StatementMatch<N> {
    fn anchor(&self) -> Anchor {
        Anchor::Statements(self.target.clone())
    }
}

type ItemCaseFn<M> = for<'case, 'snapshot> fn(
    &mut ItemCaseContext<'case, 'snapshot>,
    &<M as Matcher>::Capture<'snapshot>,
) -> Result<EditSet, Rejection>;

struct DeclarativeItemCase<M: Matcher> {
    name: String,
    apply: ItemCaseFn<M>,
}

pub(in crate::fixups) struct QueryRule<M: Matcher> {
    identity: RuleIdentity,
    matcher: M,
    cases: Vec<DeclarativeItemCase<M>>,
    ordered_non_overlapping: bool,
}

impl<M: Matcher> QueryRule<M> {
    pub(in crate::fixups) fn new(pass: Pass, rule: impl Into<String>, matcher: M) -> Self {
        Self {
            identity: RuleIdentity::new(pass, rule),
            matcher,
            cases: Vec::new(),
            ordered_non_overlapping: false,
        }
    }

    pub(in crate::fixups) fn case(mut self, name: impl Into<String>, apply: ItemCaseFn<M>) -> Self {
        self.cases.push(DeclarativeItemCase {
            name: name.into(),
            apply,
        });
        self
    }

    pub(in crate::fixups) fn ordered_non_overlapping(mut self) -> Self {
        self.ordered_non_overlapping = true;
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

    pub(in crate::fixups) fn is_bare_pointer_dereference(
        &self,
        expression: &ExpressionRef,
    ) -> bool {
        self.query.is_bare_pointer_dereference(expression)
    }

    pub(in crate::fixups) fn call_args<const N: usize>(&self, call: &CallRecord) -> [ExprSite; N] {
        assert_eq!(N, call.args.len());
        std::array::from_fn(|index| call.args[index].clone())
    }

    pub(in crate::fixups) fn statements<const N: usize>(
        &self,
        matched: &StatementMatch<N>,
    ) -> Result<[IndentStmt; N], Rejection> {
        let statements = statement_container(self.query.snapshot_program(), &matched.target)
            .and_then(|body| body.get(matched.target.start..matched.target.end))
            .ok_or_else(|| self.reject())?;
        Ok(std::array::from_fn(|index| statements[index].clone()))
    }

    pub(in crate::fixups) fn preceding_statements<const N: usize>(
        &self,
        matched: &StatementMatch<N>,
    ) -> Result<&'snapshot [IndentStmt], Rejection> {
        statement_container(self.query.snapshot_program(), &matched.target)
            .and_then(|body| body.get(..matched.target.start))
            .ok_or_else(|| self.reject())
    }

    pub(in crate::fixups) fn lower_expr(
        &mut self,
        recipe: ExprRecipe<'snapshot>,
        site: &ExprSite,
    ) -> Result<Expr, Rejection> {
        self.attempt(recipe.lower(self.query, site))
    }

    pub(in crate::fixups) fn replace_function_body(
        &self,
        target: FunctionRef<'_>,
        body: FunctionBodyRecipe,
    ) -> Result<EditSet, Rejection> {
        let mut replacement = self
            .query
            .function_def(&target)
            .cloned()
            .ok_or_else(|| self.reject())?;
        replacement.body = body.lower();
        Ok(EditSet::replace_function(target, replacement))
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

pub(in crate::fixups) enum AnchoredEdit {
    Program {
        target: super::ProgramRef,
        replacement: Program,
        removed: Vec<usize>,
    },
    Definition {
        target: DefinitionSite,
        replacement: Option<DefinitionReplacement>,
    },
    Function {
        target: FunctionEditTarget,
        replacement: crate::rust_ast::FnDef,
    },
    Expression {
        target: ExprSite,
        replacement: Expr,
    },
    Statement {
        target: StatementRange,
        replacement: Option<crate::rust_ast::Stmt>,
    },
    Statements {
        target: StatementRange,
        replacement: Vec<IndentStmt>,
    },
    #[expect(
        dead_code,
        reason = "query API surface not yet wired into a fixup rule"
    )]
    InsertItems {
        index: usize,
        expected_len: usize,
        items: Vec<Item>,
    },
}

#[derive(Clone)]
pub(in crate::fixups) enum DefinitionReplacement {
    #[expect(
        dead_code,
        reason = "query API surface not yet wired into a fixup rule"
    )]
    Item(Item),
    #[expect(
        dead_code,
        reason = "query API surface not yet wired into a fixup rule"
    )]
    ExternDecl(ExternDecl),
}

pub(in crate::fixups) struct EditSet {
    edits: Vec<AnchoredEdit>,
    evidence: Vec<Evidence>,
}

#[derive(Clone)]
pub(in crate::fixups) struct FunctionEditTarget {
    item_index: usize,
    name: String,
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
            edits: vec![AnchoredEdit::Expression {
                target,
                replacement,
            }],
            evidence: Vec::new(),
        }
    }

    pub(in crate::fixups) fn replace_program(
        target: super::ProgramRef,
        replacement: Program,
        removed: Vec<usize>,
    ) -> Self {
        Self {
            edits: vec![AnchoredEdit::Program {
                target,
                replacement,
                removed,
            }],
            evidence: Vec::new(),
        }
    }

    pub(in crate::fixups) fn push_replace_expression(
        &mut self,
        target: ExprSite,
        replacement: Expr,
    ) {
        self.edits.push(AnchoredEdit::Expression {
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
        self.edits.push(AnchoredEdit::Statement {
            target: StatementRange {
                item_index,
                path: AstPath(container),
                start: index,
                end: index + 1,
            },
            replacement,
        });
    }

    pub(in crate::fixups) fn extend(&mut self, other: EditSet) {
        self.edits.extend(other.edits);
        self.evidence.extend(other.evidence);
    }

    pub(in crate::fixups) fn delete_definition(target: DefinitionSite) -> Self {
        Self::replace_definition(target, None)
    }

    pub(in crate::fixups) fn replace_definition(
        target: DefinitionSite,
        replacement: Option<DefinitionReplacement>,
    ) -> Self {
        Self {
            edits: vec![AnchoredEdit::Definition {
                target,
                replacement,
            }],
            evidence: Vec::new(),
        }
    }

    #[expect(
        dead_code,
        reason = "query API surface not yet wired into a fixup rule"
    )]
    pub(in crate::fixups) fn replace_item(target: DefinitionSite, replacement: Item) -> Self {
        Self::replace_definition(target, Some(DefinitionReplacement::Item(replacement)))
    }

    #[expect(
        dead_code,
        reason = "query API surface not yet wired into a fixup rule"
    )]
    pub(in crate::fixups) fn replace_extern_decl(
        target: DefinitionSite,
        replacement: ExternDecl,
    ) -> Self {
        Self::replace_definition(target, Some(DefinitionReplacement::ExternDecl(replacement)))
    }

    pub(in crate::fixups) fn replace_function(
        target: FunctionRef<'_>,
        replacement: crate::rust_ast::FnDef,
    ) -> Self {
        Self {
            edits: vec![AnchoredEdit::Function {
                target: FunctionEditTarget {
                    item_index: target.item_index,
                    name: target.name,
                },
                replacement,
            }],
            evidence: Vec::new(),
        }
    }

    pub(in crate::fixups) fn replace_statements(
        target: StatementRange,
        replacement: Vec<IndentStmt>,
    ) -> Self {
        Self {
            edits: vec![AnchoredEdit::Statements {
                target,
                replacement,
            }],
            evidence: Vec::new(),
        }
    }

    #[expect(
        dead_code,
        reason = "query API surface not yet wired into a fixup rule"
    )]
    pub(in crate::fixups) fn insert_items(
        index: usize,
        expected_len: usize,
        items: Vec<Item>,
    ) -> Self {
        Self {
            edits: vec![AnchoredEdit::InsertItems {
                index,
                expected_len,
                items,
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
                AnchoredEdit::Program { target, .. } => Anchor::Program {
                    expected_len: target.expected_len,
                },
                AnchoredEdit::Definition { target, .. } => {
                    Anchor::Definition(target.location.clone())
                }
                AnchoredEdit::Expression { target, .. } => Anchor::Expression(target.clone()),
                AnchoredEdit::Function { target, .. } => Anchor::Function {
                    item_index: target.item_index,
                    name: target.name.clone(),
                },
                AnchoredEdit::Statements { target, .. } => Anchor::Statements(target.clone()),
                AnchoredEdit::Statement { target, .. } => Anchor::Statements(target.clone()),
                AnchoredEdit::InsertItems {
                    index,
                    expected_len,
                    ..
                } => Anchor::Items {
                    index: *index,
                    expected_len: *expected_len,
                },
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
    if matches!(left, Anchor::Program { .. }) || matches!(right, Anchor::Program { .. }) {
        return true;
    }
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
        (Anchor::Items { index: left, .. }, Anchor::Items { index: right, .. }) => left == right,
        (Anchor::Items { .. }, _) | (_, Anchor::Items { .. }) => false,
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
        (Anchor::Program { .. }, _) | (_, Anchor::Program { .. }) => unreachable!(),
    }
}

fn anchor_item_index(anchor: &Anchor) -> Option<usize> {
    match anchor {
        Anchor::Binding { item_index, .. }
        | Anchor::Expression(ExprSite { item_index, .. })
        | Anchor::Function { item_index, .. }
        | Anchor::Statements(StatementRange { item_index, .. }) => Some(*item_index),
        Anchor::Definition(definition) => Some(definition.item_index()),
        Anchor::Items { .. } | Anchor::Program { .. } => None,
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

const MAX_EDITS_PER_ROUND_PER_STMT: usize = 5;
const SELECTED_SITES_WINDOW: usize = 64;

pub(in crate::fixups) struct ItemPlanBuilder {
    builder: PlanBuilder<EditSet>,
    max_edits: usize,
}

impl ItemPlanBuilder {
    pub(in crate::fixups) fn new() -> Self {
        Self {
            builder: PlanBuilder::new(),
            max_edits: usize::MAX,
        }
    }

    pub(in crate::fixups) fn add_rule<M: Matcher>(
        &mut self,
        query: &QueryContext<'_>,
        rule: &QueryRule<M>,
    ) -> &mut Self {
        let identity = rule.identity.clone();
        let stmts = ProgramSummary::from_program(query.snapshot_program()).stmts;
        self.max_edits = self.max_edits.min(
            stmts
                .saturating_mul(MAX_EDITS_PER_ROUND_PER_STMT)
                .max(MAX_EDITS_PER_ROUND_PER_STMT),
        );
        let mut selected_sites = Vec::<EditSetSite>::new();
        let timing = FixupTiming::new();
        let items_start = std::time::Instant::now();
        let items = query_items(query, rule.matcher.domain());
        timing.report(items_start.elapsed(), || {
            format!(
                "[fixup-timing] {:?} query_items rule={identity:?} count={}",
                items_start.elapsed(),
                items.len()
            )
        });
        for (item_position, item) in items.into_iter().enumerate() {
            if item_position % 200 == 0 {
                timing.log(|| {
                    format!(
                        "[fixup-timing] heartbeat rule={identity:?} item={item_position} elapsed={:?}",
                        items_start.elapsed()
                    )
                });
            }
            let candidate_start = std::time::Instant::now();
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
                let site = selected.edit.site();
                let window_start = selected_sites.len().saturating_sub(SELECTED_SITES_WINDOW);
                if rule.ordered_non_overlapping
                    && selected_sites[window_start..]
                        .iter()
                        .any(|selected| selected.overlaps(&site))
                {
                    timing.report(candidate_start.elapsed(), || {
                        format!(
                            "[fixup-timing] {:?} rule={identity:?} anchor={:?}",
                            candidate_start.elapsed(),
                            capture.anchor()
                        )
                    });
                    continue;
                }
                selected_sites.push(site);
                self.builder.propose(selected);
            } else if !rejected_cases.is_empty() {
                self.builder.diagnose(PlanDiagnostic::CandidateRejected {
                    rule: identity.clone(),
                    target: Some(EditSetSite(vec![capture.anchor()])),
                    rejections: rejected_cases,
                });
            }
            timing.report(candidate_start.elapsed(), || {
                format!(
                    "[fixup-timing] {:?} rule={identity:?} anchor={:?}",
                    candidate_start.elapsed(),
                    capture.anchor()
                )
            });
        }
        self
    }

    pub(in crate::fixups) fn finish(self) -> ItemPlan {
        ItemPlan {
            plan: self.builder.finish(self.max_edits),
        }
    }
}

struct FixupTiming {
    threshold: Option<std::time::Duration>,
}

impl FixupTiming {
    fn new() -> Self {
        let threshold = std::env::var("SLATE_FIXUP_TIMING")
            .ok()
            .and_then(|millis| millis.parse().ok())
            .map(std::time::Duration::from_millis);
        Self { threshold }
    }

    fn report(&self, elapsed: std::time::Duration, message: impl FnOnce() -> String) {
        if self.threshold.is_some_and(|threshold| elapsed >= threshold) {
            eprintln!("{}", message());
        }
    }

    fn log(&self, message: impl FnOnce() -> String) {
        if self.threshold.is_some() {
            eprintln!("{}", message());
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
    if domain == QueryDomain::Parameter {
        return query
            .all_parameters()
            .into_iter()
            .map(QueryItem::Parameter)
            .collect();
    }
    if domain == QueryDomain::Expression {
        return query
            .expression_sites()
            .into_iter()
            .map(|site| QueryItem::Expression(ExpressionRef { site }))
            .collect();
    }
    if domain == QueryDomain::Program {
        return vec![QueryItem::Program(super::ProgramRef {
            expected_len: query.snapshot_program().items.len(),
        })];
    }
    if domain == QueryDomain::Field {
        let mut fields = Vec::new();
        for (item_index, item) in query.snapshot_program().items.iter().enumerate() {
            let count = match unwrap_cfg(item) {
                Item::Record(record) => record.fields.len(),
                Item::Struct(definition) => match &definition.fields {
                    crate::rust_ast::StructFields::Named(fields) => fields.len(),
                    crate::rust_ast::StructFields::Tuple(fields) => fields.len(),
                },
                _ => 0,
            };
            fields.extend(
                (0..count).map(|index| QueryItem::Field(super::FieldRef { item_index, index })),
            );
        }
        return fields;
    }
    if domain == QueryDomain::EnumVariant {
        let mut variants = Vec::new();
        for (item_index, item) in query.snapshot_program().items.iter().enumerate() {
            let Item::Enum(definition) = unwrap_cfg(item) else {
                continue;
            };
            variants.extend(
                (0..definition.variants.len()).map(|index| {
                    QueryItem::EnumVariant(super::EnumVariantRef { item_index, index })
                }),
            );
        }
        return variants;
    }
    if domain == QueryDomain::TypeUse {
        let mut uses = query
            .all_functions()
            .into_iter()
            .filter(|function| {
                query
                    .function_def(function)
                    .is_some_and(|definition| definition.ret.is_some())
            })
            .map(|function| QueryItem::TypeUse(super::TypeUseRef::FunctionReturn(function)))
            .collect::<Vec<_>>();
        uses.extend(
            query
                .all_parameters()
                .into_iter()
                .map(|parameter| QueryItem::TypeUse(super::TypeUseRef::Parameter(parameter))),
        );
        for (item_index, item) in query.snapshot_program().items.iter().enumerate() {
            let count = match unwrap_cfg(item) {
                Item::Record(record) => record.fields.len(),
                Item::Struct(definition) => match &definition.fields {
                    crate::rust_ast::StructFields::Named(fields) => fields.len(),
                    crate::rust_ast::StructFields::Tuple(fields) => fields.len(),
                },
                _ => 0,
            };
            uses.extend((0..count).map(|index| {
                QueryItem::TypeUse(super::TypeUseRef::Field(super::FieldRef {
                    item_index,
                    index,
                }))
            }));
        }
        return uses;
    }
    let mut items = Vec::new();
    for (item_index, item) in query.snapshot_program().items.iter().enumerate() {
        let Item::Fn(function) = unwrap_cfg(item) else {
            continue;
        };
        if domain == QueryDomain::Statement {
            collect_statement_items(item_index, &function.body, &mut Vec::new(), &mut items);
        } else if domain == QueryDomain::StatementContainer {
            collect_statement_containers(item_index, &function.body, &mut Vec::new(), &mut items);
        } else if domain == QueryDomain::MatchArm {
            collect_match_arms(item_index, &function.body, &mut Vec::new(), &mut items);
        }
    }
    items
}

fn collect_statement_containers<'snapshot>(
    item_index: usize,
    body: &'snapshot [IndentStmt],
    path: &mut Vec<PathSegment>,
    out: &mut Vec<QueryItem<'snapshot>>,
) {
    out.push(QueryItem::StatementContainer(
        super::StatementContainerRef {
            item_index,
            path: AstPath(path.clone()),
        },
    ));
    for (index, indent) in body.iter().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            walk::nested_body_vecs_with_path(&indent.stmt, path, &mut |nested, path| {
                collect_statement_containers(item_index, nested, path, out);
            });
        });
    }
}

fn collect_match_arms<'snapshot>(
    item_index: usize,
    body: &'snapshot [IndentStmt],
    path: &mut Vec<PathSegment>,
    out: &mut Vec<QueryItem<'snapshot>>,
) {
    for (index, indent) in body.iter().enumerate() {
        let mut statement_path = path.clone();
        statement_path.push(PathSegment::Stmt(index));
        if let crate::rust_ast::Stmt::Match { arms, .. } = &indent.stmt {
            let statement = StatementRef {
                item_index,
                path: AstPath(statement_path),
            };
            out.extend((0..arms.len()).map(|index| {
                QueryItem::MatchArm(super::MatchArmRef {
                    statement: statement.clone(),
                    index,
                })
            }));
        }
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            walk::nested_body_vecs_with_path(&indent.stmt, path, &mut |nested, path| {
                collect_match_arms(item_index, nested, path, out);
            });
        });
    }
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
        out.push(QueryItem::Statement(StatementRef {
            item_index,
            path: AstPath(statement_path),
        }));
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
        salsa: &SalsaFacts,
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
        let mut expression_edits = BTreeMap::new();
        let mut function_edits = Vec::new();
        let mut definition_edits = Vec::new();
        let mut item_insertions = Vec::new();
        let mut program_edits = Vec::new();
        for planned_edit in accepted {
            let PlannedEdit {
                identity,
                edit,
                rejected_cases,
            } = planned_edit;
            for anchored in edit.edits {
                match anchored {
                    AnchoredEdit::Program {
                        target,
                        replacement,
                        removed,
                    } => program_edits.push(PlannedProgramEdit {
                        identity: identity.clone(),
                        target,
                        replacement,
                        removed,
                        evidence: edit.evidence.clone(),
                        rejected_cases: rejected_cases.clone(),
                    }),
                    AnchoredEdit::Definition {
                        target,
                        replacement,
                    } => {
                        definition_edits.push(PlannedDefinitionEdit {
                            identity: identity.clone(),
                            target,
                            replacement,
                            evidence: edit.evidence.clone(),
                            rejected_cases: rejected_cases.clone(),
                        });
                    }
                    AnchoredEdit::Function {
                        target,
                        replacement,
                    } => {
                        function_edits.push(PlannedFunctionEdit {
                            identity: identity.clone(),
                            target,
                            replacement,
                            evidence: edit.evidence.clone(),
                            rejected_cases: rejected_cases.clone(),
                        });
                    }
                    AnchoredEdit::Expression {
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
                    AnchoredEdit::Statements {
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
                    AnchoredEdit::Statement {
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
                    AnchoredEdit::InsertItems {
                        index,
                        expected_len,
                        items,
                    } => item_insertions.push(PlannedItemInsertion {
                        identity: identity.clone(),
                        index,
                        expected_len,
                        items,
                        evidence: edit.evidence.clone(),
                        rejected_cases: rejected_cases.clone(),
                    }),
                }
            }
        }
        let mut updated = program.clone();
        let mut applied_edits = 0;
        let mut removed = Vec::new();
        applied_edits += apply_program_edits(
            &mut updated,
            program_edits,
            logger,
            &mut diagnostics,
            &mut removed,
        );
        applied_edits += apply_expression_edits(
            &mut updated,
            expression_edits,
            salsa,
            logger,
            &mut diagnostics,
        );
        let (statement_applied, missing_statements) = {
            let mut state = ApplyState {
                edits: statement_edits,
                applied: 0,
                salsa,
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
            (state.applied, state.edits)
        };
        applied_edits += statement_applied;
        for (_, edit) in missing_statements {
            diagnostics.push(PlanDiagnostic::MissingTarget {
                contender: edit.identity,
                target: EditSetSite(vec![Anchor::Statements(edit.target)]),
            });
        }
        applied_edits +=
            apply_function_edits(&mut updated, function_edits, logger, &mut diagnostics);
        applied_edits += apply_definition_edits(
            &mut updated,
            definition_edits,
            logger,
            &mut diagnostics,
            &mut removed,
        );
        applied_edits += apply_item_insertions(
            &mut updated,
            item_insertions,
            logger,
            &mut diagnostics,
            &removed,
        );
        let applied = if applied_edits == expected_edits {
            *program = updated;
            accepted_sets
        } else {
            0
        };
        ItemApplyReport {
            changed: applied != 0,
            planned,
            applied,
            diagnostics,
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
        AnchoredEdit::Program { target, .. } => Anchor::Program {
            expected_len: target.expected_len,
        },
        AnchoredEdit::Definition { target, .. } => Anchor::Definition(target.location.clone()),
        AnchoredEdit::Expression { target, .. } => Anchor::Expression(target.clone()),
        AnchoredEdit::Statement { target, .. } | AnchoredEdit::Statements { target, .. } => {
            Anchor::Statements(target.clone())
        }
        AnchoredEdit::Function { target, .. } => Anchor::Function {
            item_index: target.item_index,
            name: target.name.clone(),
        },
        AnchoredEdit::InsertItems {
            index,
            expected_len,
            ..
        } => Anchor::Items {
            index: *index,
            expected_len: *expected_len,
        },
    }
}

fn anchored_edit_exists(program: &Program, edit: &AnchoredEdit) -> bool {
    match edit {
        AnchoredEdit::Program { target, .. } => target.expected_len == program.items.len(),
        AnchoredEdit::Definition {
            target,
            replacement,
        } => {
            definition_exists(program, target)
                && matches!(
                    (&target.location, replacement),
                    (DefinitionLocation::Item(_), None)
                        | (
                            DefinitionLocation::Item(_),
                            Some(DefinitionReplacement::Item(_))
                        )
                        | (DefinitionLocation::ExternDecl { .. }, None)
                        | (
                            DefinitionLocation::ExternDecl { .. },
                            Some(DefinitionReplacement::ExternDecl(_))
                        )
                )
        }
        AnchoredEdit::Expression { target, .. } => expression_matches(program, target, |_| true),
        AnchoredEdit::Statement { target, .. } | AnchoredEdit::Statements { target, .. } => {
            statement_range_exists(program, target)
        }
        AnchoredEdit::Function { target, .. } => {
            program.items.get(target.item_index).is_some_and(|item| {
                let Item::Fn(function) = unwrap_cfg(item) else {
                    return false;
                };
                function.name == target.name
            })
        }
        AnchoredEdit::InsertItems {
            index,
            expected_len,
            items,
        } => *expected_len == program.items.len() && *index <= *expected_len && !items.is_empty(),
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
    walk::target_expr_at_path(program, target.item_index, &target.path).is_some_and(predicate)
}

fn statement_range_exists(program: &Program, target: &StatementRange) -> bool {
    statement_container(program, target)
        .is_some_and(|body| target.start < target.end && target.end <= body.len())
}

fn statement_container<'program>(
    program: &'program Program,
    target: &StatementRange,
) -> Option<&'program [IndentStmt]> {
    let Item::Fn(function) = program.items.get(target.item_index)? else {
        return None;
    };
    body_container(&function.body, &mut Vec::new(), &target.path)
}

fn body_container<'body>(
    body: &'body [IndentStmt],
    path: &mut Vec<PathSegment>,
    target: &AstPath,
) -> Option<&'body [IndentStmt]> {
    if path.as_slice() == target.0 {
        return Some(body);
    }
    for (index, indent) in body.iter().enumerate() {
        let mut found = None;
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            walk::nested_body_vecs_with_path(&indent.stmt, path, &mut |nested, path| {
                if found.is_none() {
                    found = body_container(nested, path, target);
                }
            });
        });
        if found.is_some() {
            return found;
        }
    }
    None
}

struct PlannedProgramEdit {
    identity: RuleCaseIdentity,
    target: super::ProgramRef,
    replacement: Program,
    removed: Vec<usize>,
    evidence: Vec<Evidence>,
    rejected_cases: Vec<CaseRejection>,
}

fn apply_program_edits(
    program: &mut Program,
    edits: Vec<PlannedProgramEdit>,
    logger: &mut dyn TraceLogger,
    diagnostics: &mut Vec<PlanDiagnostic<EditSetSite>>,
    removed: &mut Vec<usize>,
) -> usize {
    let mut applied = 0;
    for edit in edits {
        if edit.target.expected_len != program.items.len() {
            diagnostics.push(PlanDiagnostic::MissingTarget {
                contender: edit.identity,
                target: EditSetSite(vec![Anchor::Program {
                    expected_len: edit.target.expected_len,
                }]),
            });
            continue;
        }
        let before = program.emit();
        let after = edit.replacement.emit();
        log_item_edit(
            logger,
            &edit.identity,
            &edit.evidence,
            &edit.rejected_cases,
            TraceChange {
                location: TraceLocation {
                    function: Some("program".into()),
                    ast_path: Some("program".into()),
                    ..TraceLocation::default()
                },
                label: "program",
                before: before.trim_end().to_owned(),
                after: after.trim_end().to_owned(),
            },
        );
        *program = edit.replacement;
        removed.extend(edit.removed);
        applied += 1;
    }
    applied
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
    salsa: &SalsaFacts,
    logger: &mut dyn TraceLogger,
    diagnostics: &mut Vec<PlanDiagnostic<EditSetSite>>,
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
                        location: salsa
                            .function_by_item_index(item_index)
                            .map(|function| {
                                function_path_location(salsa, function, &edit.target.path.0)
                            })
                            .unwrap_or_else(|| path_location(&edit.target.path.0)),
                        label: "expr",
                        before: before.render(),
                        after: expr.render(),
                    },
                );
                applied += 1;
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

struct PlannedFunctionEdit {
    identity: RuleCaseIdentity,
    target: FunctionEditTarget,
    replacement: crate::rust_ast::FnDef,
    evidence: Vec<Evidence>,
    rejected_cases: Vec<CaseRejection>,
}

fn apply_function_edits(
    program: &mut Program,
    edits: Vec<PlannedFunctionEdit>,
    logger: &mut dyn TraceLogger,
    diagnostics: &mut Vec<PlanDiagnostic<EditSetSite>>,
) -> usize {
    let mut applied = 0;
    for edit in edits {
        let Some(item) = program.items.get_mut(edit.target.item_index) else {
            diagnostics.push(missing_function(edit));
            continue;
        };
        let Item::Fn(function) = unwrap_cfg_mut(item) else {
            diagnostics.push(missing_function(edit));
            continue;
        };
        if function.name != edit.target.name {
            diagnostics.push(missing_function(edit));
            continue;
        }
        let before = item_snippet(&Item::Fn(function.clone()));
        *function = edit.replacement;
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
    }
    applied
}

fn missing_function(edit: PlannedFunctionEdit) -> PlanDiagnostic<EditSetSite> {
    PlanDiagnostic::MissingTarget {
        contender: edit.identity,
        target: EditSetSite(vec![Anchor::Function {
            item_index: edit.target.item_index,
            name: edit.target.name,
        }]),
    }
}

fn unwrap_cfg_mut(item: &mut Item) -> &mut Item {
    match item {
        Item::Cfg { item, .. } => unwrap_cfg_mut(item),
        _ => item,
    }
}

struct PlannedDefinitionEdit {
    identity: RuleCaseIdentity,
    target: DefinitionSite,
    replacement: Option<DefinitionReplacement>,
    evidence: Vec<Evidence>,
    rejected_cases: Vec<CaseRejection>,
}

fn apply_definition_edits(
    program: &mut Program,
    edits: Vec<PlannedDefinitionEdit>,
    logger: &mut dyn TraceLogger,
    diagnostics: &mut Vec<PlanDiagnostic<EditSetSite>>,
    removed: &mut Vec<usize>,
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
            match &edit.replacement {
                None => {
                    program.items.remove(item_index);
                    log_definition_edit(logger, &edit, before, None);
                    removed.push(item_index);
                }
                Some(DefinitionReplacement::Item(replacement)) => {
                    program.items[item_index] = replacement.clone();
                    let after = item_snippet(&program.items[item_index]);
                    log_definition_edit(logger, &edit, before, Some(after));
                }
                Some(DefinitionReplacement::ExternDecl(_)) => unreachable!(),
            }
            applied += 1;
            continue;
        }
        let Item::ExternBlock { abi, decls } = &mut program.items[item_index] else {
            diagnostics.extend(edits.into_iter().map(missing_definition));
            continue;
        };
        for edit in edits {
            let DefinitionLocation::ExternDecl { decl_index, .. } = edit.target.location else {
                unreachable!()
            };
            if !decls
                .get(decl_index)
                .is_some_and(|decl| extern_definition_matches(decl, &edit.target))
            {
                diagnostics.push(missing_definition(edit));
                continue;
            }
            let before = extern_decl_snippet(abi, &decls[decl_index]);
            let after = match &edit.replacement {
                None => {
                    decls.remove(decl_index);
                    None
                }
                Some(DefinitionReplacement::ExternDecl(replacement)) => {
                    decls[decl_index] = replacement.clone();
                    Some(extern_decl_snippet(abi, &decls[decl_index]))
                }
                Some(DefinitionReplacement::Item(_)) => unreachable!(),
            };
            log_definition_edit(logger, &edit, before, after);
            applied += 1;
        }
        if decls.is_empty() {
            program.items.remove(item_index);
            removed.push(item_index);
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
    salsa: &'a SalsaFacts,
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
                state.salsa,
            ));
        }
        body.splice(edit.target.start..edit.target.end, replacement);
        state.applied += 1;
    }
}

pub(in crate::fixups) struct ItemApplyReport {
    pub(in crate::fixups) changed: bool,
    #[expect(
        dead_code,
        reason = "query API surface not yet wired into a fixup rule"
    )]
    pub(in crate::fixups) planned: usize,
    #[cfg_attr(not(test), expect(dead_code, reason = "read only by tests"))]
    pub(in crate::fixups) applied: usize,
    #[cfg_attr(not(test), expect(dead_code, reason = "read only by tests"))]
    pub(super) diagnostics: Vec<PlanDiagnostic<EditSetSite>>,
}

fn rewrite_event(
    identity: &RuleCaseIdentity,
    target: &StatementRange,
    evidence: &[Evidence],
    rejected_cases: &[CaseRejection],
    before: &[IndentStmt],
    after: &[IndentStmt],
    salsa: &SalsaFacts,
) -> RewriteEvent {
    let location = salsa
        .function_by_item_index(target.item_index)
        .map(|function| function_path_location(salsa, function, &target.path.0))
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
        (Item::Static { name, .. }, DefinitionKind::Static) => *name == definition.name,
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
        ..Program::default()
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

struct PlannedItemInsertion {
    identity: RuleCaseIdentity,
    index: usize,
    expected_len: usize,
    items: Vec<Item>,
    evidence: Vec<Evidence>,
    rejected_cases: Vec<CaseRejection>,
}

fn apply_item_insertions(
    program: &mut Program,
    mut edits: Vec<PlannedItemInsertion>,
    logger: &mut dyn TraceLogger,
    diagnostics: &mut Vec<PlanDiagnostic<EditSetSite>>,
    removed: &[usize],
) -> usize {
    edits.sort_by_key(|edit| std::cmp::Reverse(edit.index));
    let mut applied = 0;
    let mut inserted = 0;
    for edit in edits {
        if edit.expected_len < removed.len()
            || program.items.len() != edit.expected_len - removed.len() + inserted
        {
            diagnostics.push(missing_item_insertion(edit));
            continue;
        }
        let adjusted = edit.index - removed.iter().filter(|index| **index < edit.index).count();
        if adjusted > program.items.len() {
            diagnostics.push(missing_item_insertion(edit));
            continue;
        }
        let after = edit
            .items
            .iter()
            .map(item_snippet)
            .collect::<Vec<_>>()
            .join("\n");
        let inserted_now = edit.items.len();
        program.items.splice(adjusted..adjusted, edit.items);
        log_item_edit(
            logger,
            &edit.identity,
            &edit.evidence,
            &edit.rejected_cases,
            TraceChange {
                location: TraceLocation::default(),
                label: "items",
                before: String::new(),
                after,
            },
        );
        applied += 1;
        inserted += inserted_now;
    }
    applied
}

fn missing_item_insertion(edit: PlannedItemInsertion) -> PlanDiagnostic<EditSetSite> {
    PlanDiagnostic::MissingTarget {
        contender: edit.identity,
        target: EditSetSite(vec![Anchor::Items {
            index: edit.index,
            expected_len: edit.expected_len,
        }]),
    }
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
    use crate::fixups::salsa::SalsaFacts;
    use crate::fixups::trace::NoopLogger;
    use crate::rust_ast::{FnDef, RustValue, Stmt, Visibility};

    fn test_salsa(program: &Program) -> SalsaFacts {
        let mut salsa = SalsaFacts::new_empty();
        salsa.set_program(program);
        salsa
    }

    fn function(body: Vec<IndentStmt>) -> Item {
        named_function("test", body)
    }

    fn named_function(name: &str, body: Vec<IndentStmt>) -> Item {
        Item::Fn(FnDef {
            attrs: Vec::new(),
            vis: Visibility::Private,
            unsafe_: false,
            abi: None,
            name: name.into(),
            params: Vec::new(),
            ret: None,
            body,
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
            plan: builder.finish(usize::MAX),
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
            ..Program::default()
        };
        let before = program.emit();
        let mut edit =
            EditSet::replace_expression(expression_site(0), Expr::Value(RustValue::I64(2)));
        edit.push_replace_statement(0, AstPath(vec![PathSegment::Stmt(4)]), None);
        let valid = EditSet::replace_expression(expression_site(1), Expr::Value(RustValue::I64(3)));
        let plan = item_plan_from(vec![edit, valid]);
        let salsa = test_salsa(&program);

        let report = plan.apply(&mut program, &salsa, &mut NoopLogger);

        assert_ne!(program.emit(), before);
        assert_eq!(program.emit(), "fn test() {\n    1;\n    3;\n}\n");
        assert!(report.changed);
        assert_eq!(report.applied, 1);
        assert!(matches!(
            report.diagnostics.last(),
            Some(PlanDiagnostic::MissingTarget { .. })
        ));
    }

    #[test]
    fn mixed_edits_use_snapshot_paths_and_report_one_atomic_application() {
        let mut program = Program {
            items: vec![function(vec![statement(1), statement(2), statement(3)])],
            ..Program::default()
        };
        let mut edit = EditSet::new();
        edit.push_replace_statement(0, AstPath(vec![PathSegment::Stmt(0)]), None);
        edit.push_replace_expression(expression_site(2), Expr::Value(RustValue::I64(4)));
        let plan = item_plan(edit);
        let salsa = test_salsa(&program);

        let report = plan.apply(&mut program, &salsa, &mut NoopLogger);

        assert_eq!(program.emit(), "fn test() {\n    2;\n    4;\n}\n");
        assert_eq!(report.applied, 1);
    }

    fn function_ref<'db>(
        salsa: &'db SalsaFacts,
        item_index: usize,
        name: &str,
    ) -> FunctionRef<'db> {
        FunctionRef {
            item_index,
            name: name.into(),
            id: salsa.function_by_item_index(item_index).unwrap(),
        }
    }

    #[test]
    fn function_edit_and_statement_edit_in_a_different_item_apply_atomically() {
        let mut program = Program {
            items: vec![
                named_function("caller", vec![statement(1)]),
                named_function("callee", vec![statement(2)]),
            ],
            ..Program::default()
        };
        let salsa = test_salsa(&program);
        let target = function_ref(&salsa, 1, "callee");
        let replacement = FnDef {
            attrs: Vec::new(),
            vis: Visibility::Private,
            unsafe_: false,
            abi: None,
            name: "callee".into(),
            params: Vec::new(),
            ret: None,
            body: vec![statement(3)],
        };
        let mut edit = EditSet::replace_function(target, replacement);
        edit.push_replace_statement(
            0,
            AstPath(vec![PathSegment::Stmt(0)]),
            Some(statement(4).stmt),
        );
        let plan = item_plan(edit);

        let report = plan.apply(&mut program, &salsa, &mut NoopLogger);

        assert_eq!(
            program.emit(),
            "fn caller() {\n    4;\n}\n\nfn callee() {\n    3;\n}\n"
        );
        assert_eq!(report.applied, 1);
    }

    #[test]
    fn stale_anchor_in_one_item_rejects_the_whole_multi_item_set() {
        let mut program = Program {
            items: vec![
                named_function("caller", vec![statement(1)]),
                named_function("callee", vec![statement(2)]),
            ],
            ..Program::default()
        };
        let before = program.emit();
        let salsa = test_salsa(&program);
        let target = function_ref(&salsa, 1, "callee");
        let replacement = FnDef {
            attrs: Vec::new(),
            vis: Visibility::Private,
            unsafe_: false,
            abi: None,
            name: "callee".into(),
            params: Vec::new(),
            ret: None,
            body: vec![statement(3)],
        };
        let mut edit = EditSet::replace_function(target, replacement);
        // Stale: item 0 only has one statement (index 0), so index 4 can't exist.
        edit.push_replace_statement(
            0,
            AstPath(vec![PathSegment::Stmt(4)]),
            Some(statement(4).stmt),
        );
        let plan = item_plan(edit);

        let report = plan.apply(&mut program, &salsa, &mut NoopLogger);

        assert_eq!(program.emit(), before);
        assert_eq!(report.applied, 0);
        assert!(!report.changed);
        assert!(matches!(
            report.diagnostics.last(),
            Some(PlanDiagnostic::MissingTarget { .. })
        ));
    }

    #[test]
    fn whole_program_edit_uses_shared_atomic_application() {
        let mut program = Program {
            items: vec![function(vec![statement(1)])],
            ..Program::default()
        };
        let replacement = Program {
            items: vec![function(vec![statement(2)])],
            ..Program::default()
        };
        let edit = EditSet::replace_program(
            super::super::ProgramRef { expected_len: 1 },
            replacement,
            Vec::new(),
        );
        let plan = item_plan(edit);
        let salsa = test_salsa(&program);

        let report = plan.apply(&mut program, &salsa, &mut NoopLogger);

        assert_eq!(program.emit(), "fn test() {\n    2;\n}\n");
        assert_eq!(report.applied, 1);
    }

    #[test]
    fn whole_program_edit_overlaps_nested_edits() {
        let mut edit = EditSet::replace_program(
            super::super::ProgramRef { expected_len: 1 },
            Program {
                items: vec![function(vec![statement(2)])],
                ..Program::default()
            },
            Vec::new(),
        );
        edit.push_replace_expression(expression_site(0), Expr::Value(RustValue::I64(3)));

        let plan = item_plan(edit);

        assert!(plan.plan.edits.is_empty());
        assert!(matches!(
            plan.plan.diagnostics.as_slice(),
            [PlanDiagnostic::OverlappingTargets { .. }]
        ));
    }
}
