use std::collections::BTreeMap;

use crate::fixups::trace::{Pass, RewriteEvent, TraceLocation, TraceLogger, TraceSnippet, fact};
use crate::rust_ast::{ExternDecl, IndentStmt, Item, Program};

use super::plan::{EditTarget, Plan, PlanBuilder, PlanDiagnostic, PlanSite, PlannedEdit};
use super::rewrite::{evidence_trace_fact, predicate_name, rejection_name};
use super::{
    CaseRejection, DefinitionGroup, DefinitionKind, DefinitionLocation, DefinitionSelector,
    DefinitionSite, Evidence, FunctionBodyRecipe, QueryContext, Rejection, RuleCaseIdentity,
    RuleIdentity,
};

enum DefinitionRuleSelector {
    Exact(DefinitionSelector),
    Group(DefinitionGroup),
    KnownExternFunctions,
}

type DefinitionCaseFn = for<'case, 'snapshot> fn(
    &mut DefinitionCaseContext<'case, 'snapshot>,
) -> Result<DefinitionRecipe, Rejection>;

struct DeclarativeDefinitionCase {
    name: String,
    apply: DefinitionCaseFn,
}

pub(in crate::fixups) struct DefinitionRule {
    identity: RuleIdentity,
    selector: DefinitionRuleSelector,
    cases: Vec<DeclarativeDefinitionCase>,
}

impl DefinitionRule {
    pub(in crate::fixups) fn function(
        pass: Pass,
        rule: impl Into<String>,
        name: impl Into<String>,
    ) -> Self {
        Self {
            identity: RuleIdentity::new(pass, rule),
            selector: DefinitionRuleSelector::Exact(DefinitionSelector {
                kind: DefinitionKind::Function,
                name: name.into(),
            }),
            cases: Vec::new(),
        }
    }

    pub(in crate::fixups) fn extern_function(
        pass: Pass,
        rule: impl Into<String>,
        name: impl Into<String>,
    ) -> Self {
        Self {
            identity: RuleIdentity::new(pass, rule),
            selector: DefinitionRuleSelector::Exact(DefinitionSelector {
                kind: DefinitionKind::ExternFunction,
                name: name.into(),
            }),
            cases: Vec::new(),
        }
    }

    pub(in crate::fixups) fn header(
        pass: Pass,
        rule: impl Into<String>,
        header: impl Into<String>,
    ) -> Self {
        Self {
            identity: RuleIdentity::new(pass, rule),
            selector: DefinitionRuleSelector::Group(DefinitionGroup::Header(header.into())),
            cases: Vec::new(),
        }
    }

    pub(in crate::fixups) fn known_extern_functions(pass: Pass, rule: impl Into<String>) -> Self {
        Self {
            identity: RuleIdentity::new(pass, rule),
            selector: DefinitionRuleSelector::KnownExternFunctions,
            cases: Vec::new(),
        }
    }

    pub(in crate::fixups) fn support_module(
        pass: Pass,
        rule: impl Into<String>,
        name: impl Into<String>,
    ) -> Self {
        Self {
            identity: RuleIdentity::new(pass, rule),
            selector: DefinitionRuleSelector::Exact(DefinitionSelector {
                kind: DefinitionKind::SupportModule,
                name: name.into(),
            }),
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
        match &self.selector {
            DefinitionRuleSelector::Exact(selector) => query.definitions(selector).to_vec(),
            DefinitionRuleSelector::Group(group) => query.definitions_in_group(group),
            DefinitionRuleSelector::KnownExternFunctions => query
                .definitions_of_kind(DefinitionKind::ExternFunction)
                .into_iter()
                .filter(|definition| definition.group.is_some())
                .collect(),
        }
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
                    }
                    DefinitionEditAction::ReplaceBody(body) => {
                        let Item::Fn(function) = &mut program.items[item_index] else {
                            unreachable!()
                        };
                        function.body = body.clone();
                        let after = item_snippet(&program.items[item_index]);
                        log_edit(logger, &edit, before, Some(after));
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
            }
            if decls.is_empty() {
                program.items.remove(item_index);
            }
        }
        DefinitionApplyReport {
            changed: applied != 0,
            planned,
            applied,
            diagnostics,
        }
    }
}

pub(in crate::fixups) struct DefinitionApplyReport {
    pub(in crate::fixups) changed: bool,
    pub(in crate::fixups) planned: usize,
    pub(in crate::fixups) applied: usize,
    #[allow(dead_code)]
    pub(super) diagnostics: Vec<PlanDiagnostic<DefinitionLocation>>,
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
