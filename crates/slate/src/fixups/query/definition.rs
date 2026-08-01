use std::collections::BTreeMap;

use crate::fixups::facts::StringRecoveryCandidate;
use crate::fixups::trace::{Pass, RewriteEvent, TraceLocation, TraceLogger, TraceSnippet, fact};
use crate::rust_ast::{ExternDecl, IndentStmt, Item, Program};

use super::plan::{
    EditTarget, Plan, PlanBuilder, PlanDiagnostic, PlanSite, PlannedEdit, TouchedItems,
};
use super::rewrite::{evidence_trace_fact, predicate_name, rejection_name};
use super::{
    CaseRejection, Definition, DefinitionKind, DefinitionLocation, DefinitionSite, Evidence, Field,
    FunctionBodyRecipe, HeapOwnershipPlanSet, QueryContext, Rejection, RuleCaseIdentity,
    RuleIdentity, StringLiftPlanSet,
};

type DefinitionCaseFn = for<'case, 'snapshot> fn(
    &mut DefinitionCaseContext<'case, 'snapshot>,
) -> Result<DefinitionRecipe, Rejection>;

struct DeclarativeDefinitionCase {
    name: String,
    apply: DefinitionCaseFn,
}

pub(in crate::fixups) struct DefinitionRule {
    identity: RuleIdentity,
    matcher: Definition,
    cases: Vec<DeclarativeDefinitionCase>,
}

impl DefinitionRule {
    pub(in crate::fixups) fn matches(
        pass: Pass,
        rule: impl Into<String>,
        matcher: Definition,
    ) -> Self {
        Self {
            identity: RuleIdentity::new(pass, rule),
            matcher,
            cases: Vec::new(),
        }
    }

    pub(in crate::fixups) fn case(
        mut self,
        name: impl Into<String>,
        apply: DefinitionCaseFn,
    ) -> Self {
        self.cases.push(DeclarativeDefinitionCase {
            name: name.into(),
            apply,
        });
        self
    }

    fn candidates(&self, query: &QueryContext<'_>) -> Vec<DefinitionSite> {
        query
            .all_definitions()
            .filter(|definition| {
                self.matcher.kind.matches(&definition.kind, &())
                    && self.matcher.name.matches(&definition.name, &())
                    && self.matcher.group.matches(&definition.group, &())
            })
            .cloned()
            .collect()
    }
}

pub(in crate::fixups) struct DefinitionCaseContext<'case, 'snapshot> {
    query: &'case QueryContext<'snapshot>,
    definition: &'case DefinitionSite,
    evidence: Vec<Evidence>,
}

impl DefinitionCaseContext<'_, '_> {
    pub(in crate::fixups) fn zero_users(&mut self) -> Result<(), Rejection> {
        self.prove(self.query.zero_users(self.definition)).map(drop)
    }

    pub(in crate::fixups) fn zero_group_users(&mut self) -> Result<(), Rejection> {
        let Some(group) = &self.definition.group else {
            return Err(Rejection::new(
                super::Predicate::ZeroGroupUsers,
                None,
                super::RejectionReason::MissingEvidence,
                self.evidence.clone(),
            ));
        };
        self.prove(self.query.zero_group_users(group)).map(drop)
    }

    pub(in crate::fixups) fn heap_ownership_plans(
        &mut self,
    ) -> Result<HeapOwnershipPlanSet, Rejection> {
        self.prove(self.query.heap_ownership_plans(self.definition))
    }

    pub(in crate::fixups) fn string_lift_plans(
        &mut self,
        recovery: &Field<StringRecoveryCandidate>,
    ) -> Result<StringLiftPlanSet, Rejection> {
        self.prove(self.query.string_lift_plans(self.definition, recovery))
    }

    pub(in crate::fixups) fn function_body(&self) -> Vec<IndentStmt> {
        match &self.query.snapshot_program().items[self.definition.location.item_index()] {
            Item::Fn(function) => function.body.clone(),
            _ => Vec::new(),
        }
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

pub(in crate::fixups) enum DefinitionRecipe {
    Delete,
    ReplaceBody(FunctionBodyRecipe),
}

pub(in crate::fixups) fn delete_definition() -> DefinitionRecipe {
    DefinitionRecipe::Delete
}

pub(in crate::fixups) fn replace_body(body: FunctionBodyRecipe) -> DefinitionRecipe {
    DefinitionRecipe::ReplaceBody(body)
}

enum DefinitionEditAction {
    Delete,
    ReplaceBody(Vec<IndentStmt>),
}

struct DefinitionEdit {
    target: DefinitionSite,
    action: DefinitionEditAction,
    evidence: Vec<Evidence>,
}

impl EditTarget for DefinitionEdit {
    type Site = DefinitionLocation;

    fn site(&self) -> DefinitionLocation {
        self.target.location.clone()
    }
}

impl PlanSite for DefinitionLocation {
    fn overlaps(&self, other: &Self) -> bool {
        match (self, other) {
            (DefinitionLocation::Item(item), DefinitionLocation::ExternDecl { item_index, .. })
            | (DefinitionLocation::ExternDecl { item_index, .. }, DefinitionLocation::Item(item)) => {
                item == item_index
            }
            _ => false,
        }
    }
}

#[derive(Default)]
pub(in crate::fixups) struct DefinitionPlanBuilder {
    builder: PlanBuilder<DefinitionEdit>,
}

impl DefinitionPlanBuilder {
    pub(in crate::fixups) fn new() -> Self {
        Self::default()
    }

    pub(in crate::fixups) fn add_rule(
        &mut self,
        query: &QueryContext<'_>,
        rule: &DefinitionRule,
    ) -> &mut Self {
        for definition in rule.candidates(query) {
            let mut rejected_cases = Vec::new();
            let mut selected = None;
            for case in &rule.cases {
                let mut context = DefinitionCaseContext {
                    query,
                    definition: &definition,
                    evidence: Vec::new(),
                };
                match (case.apply)(&mut context) {
                    Ok(recipe) => {
                        let action = match recipe {
                            DefinitionRecipe::Delete => DefinitionEditAction::Delete,
                            DefinitionRecipe::ReplaceBody(body) => {
                                DefinitionEditAction::ReplaceBody(body.lower())
                            }
                        };
                        selected = Some(PlannedEdit {
                            identity: RuleCaseIdentity {
                                rule: rule.identity.clone(),
                                case: case.name.clone(),
                            },
                            edit: DefinitionEdit {
                                target: definition.clone(),
                                action,
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
                    rule: rule.identity.clone(),
                    target: Some(definition.location.clone()),
                    rejections: rejected_cases,
                });
            }
        }
        self
    }

    pub(in crate::fixups) fn finish(self) -> DefinitionPlan {
        DefinitionPlan {
            plan: self.builder.finish(),
        }
    }
}

pub(in crate::fixups) struct DefinitionPlan {
    plan: Plan<DefinitionEdit>,
}

impl DefinitionPlan {
    pub(in crate::fixups) fn apply(
        self,
        program: &mut Program,
        logger: &mut dyn TraceLogger,
    ) -> DefinitionApplyReport {
        let planned = self.plan.edits.len();
        let mut diagnostics = self.plan.diagnostics;
        let mut by_item = BTreeMap::<usize, Vec<PlannedEdit<DefinitionEdit>>>::new();
        for edit in self.plan.edits {
            by_item
                .entry(edit.edit.target.location.item_index())
                .or_default()
                .push(edit);
        }
        let mut applied = 0;
        let mut touched = TouchedItems::none();
        for (item_index, mut edits) in by_item.into_iter().rev() {
            edits.sort_by(|left, right| right.edit.target.location.cmp(&left.edit.target.location));
            if item_index >= program.items.len() {
                for edit in edits {
                    diagnostics.push(missing_target(edit));
                }
                continue;
            }
            if matches!(
                edits.first().map(|edit| &edit.edit.target.location),
                Some(DefinitionLocation::Item(_))
            ) {
                let edit = edits.pop().unwrap();
                if !definition_matches(&program.items[item_index], &edit.edit.target) {
                    diagnostics.push(missing_target(edit));
                    continue;
                }
                let before = item_snippet(&program.items[item_index]);
                match &edit.edit.action {
                    DefinitionEditAction::Delete => {
                        program.items.remove(item_index);
                        log_edit(logger, &edit, before, None);
                        touched.removed.push(item_index);
                    }
                    DefinitionEditAction::ReplaceBody(body) => {
                        let Item::Fn(function) = &mut program.items[item_index] else {
                            unreachable!()
                        };
                        function.body = body.clone();
                        let after = item_snippet(&program.items[item_index]);
                        log_edit(logger, &edit, before, Some(after));
                        touched.in_place.push(item_index);
                    }
                }
                applied += 1;
                continue;
            }
            let Item::ExternBlock { abi, decls } = &mut program.items[item_index] else {
                for edit in edits {
                    diagnostics.push(missing_target(edit));
                }
                continue;
            };
            let mut any_removed = false;
            for edit in edits {
                let DefinitionLocation::ExternDecl { decl_index, .. } = edit.edit.target.location
                else {
                    unreachable!()
                };
                if !matches!(&edit.edit.action, DefinitionEditAction::Delete)
                    || !decls
                        .get(decl_index)
                        .is_some_and(|decl| extern_definition_matches(decl, &edit.edit.target))
                {
                    diagnostics.push(missing_target(edit));
                    continue;
                }
                let before = extern_decl_snippet(abi, &decls[decl_index]);
                decls.remove(decl_index);
                log_edit(logger, &edit, before, None);
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
        DefinitionApplyReport {
            changed: applied != 0,
            planned,
            applied,
            diagnostics,
            touched,
        }
    }
}

pub(in crate::fixups) struct DefinitionApplyReport {
    pub(in crate::fixups) changed: bool,
    pub(in crate::fixups) planned: usize,
    pub(in crate::fixups) applied: usize,
    #[allow(dead_code)]
    pub(super) diagnostics: Vec<PlanDiagnostic<DefinitionLocation>>,
    pub(in crate::fixups) touched: TouchedItems,
}

fn missing_target(edit: PlannedEdit<DefinitionEdit>) -> PlanDiagnostic<DefinitionLocation> {
    PlanDiagnostic::MissingTarget {
        contender: edit.identity,
        target: edit.edit.target.location,
    }
}

fn definition_matches(item: &Item, definition: &DefinitionSite) -> bool {
    match (item, definition.kind) {
        (Item::Fn(function), DefinitionKind::Function) => function.name == definition.name,
        (Item::SupportModule(module), DefinitionKind::SupportModule) => {
            module.name.as_str() == definition.name
        }
        _ => false,
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

fn log_edit(
    logger: &mut dyn TraceLogger,
    edit: &PlannedEdit<DefinitionEdit>,
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
    facts.extend(edit.edit.evidence.iter().map(evidence_trace_fact));
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
            function: Some(edit.edit.target.name.clone()),
            ..TraceLocation::default()
        },
        before: vec![TraceSnippet::new("definition", before)],
        after: after
            .map(|after| vec![TraceSnippet::new("definition", after)])
            .unwrap_or_default(),
        facts,
    });
}
