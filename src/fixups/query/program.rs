use crate::fixups::trace::{Pass, RewriteEvent, TraceLocation, TraceLogger, TraceSnippet, fact};
use crate::rust_ast::{Item, Program};

use super::plan::{EditTarget, Plan, PlanBuilder, PlanDiagnostic, PlannedEdit};
use super::program_recipe::PreparedProgram;
use super::rewrite::{evidence_trace_fact, predicate_name, rejection_name};
use super::{
    AnonymousStructSet, CaseRejection, Evidence, ProgramRecipe, QueryContext, Rejection,
    RuleCaseIdentity, RuleIdentity,
};

enum ProgramRuleSelector {
    AnonymousStructs,
}

type ProgramCaseFn = for<'case, 'snapshot> fn(
    &mut ProgramCaseContext<'case, 'snapshot>,
) -> Result<ProgramRecipe, Rejection>;

struct DeclarativeProgramCase {
    name: String,
    apply: ProgramCaseFn,
}

pub(in crate::fixups) struct ProgramRule {
    identity: RuleIdentity,
    selector: ProgramRuleSelector,
    cases: Vec<DeclarativeProgramCase>,
}

impl ProgramRule {
    pub(in crate::fixups) fn anonymous_structs(pass: Pass, rule: impl Into<String>) -> Self {
        Self {
            identity: RuleIdentity::new(pass, rule),
            selector: ProgramRuleSelector::AnonymousStructs,
            cases: Vec::new(),
        }
    }

    pub(in crate::fixups) fn case(mut self, name: impl Into<String>, apply: ProgramCaseFn) -> Self {
        self.cases.push(DeclarativeProgramCase {
            name: name.into(),
            apply,
        });
        self
    }

    fn has_candidates(&self, query: &QueryContext<'_>) -> bool {
        match self.selector {
            ProgramRuleSelector::AnonymousStructs => query.has_anonymous_structs(),
        }
    }
}

pub(in crate::fixups) struct ProgramCaseContext<'case, 'snapshot> {
    query: &'case QueryContext<'snapshot>,
    evidence: Vec<Evidence>,
}

impl ProgramCaseContext<'_, '_> {
    pub(in crate::fixups) fn anonymous_structs(&mut self) -> Result<AnonymousStructSet, Rejection> {
        self.prove(self.query.anonymous_structs())
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

struct ProgramEdit {
    prepared: PreparedProgram,
    evidence: Vec<Evidence>,
}

impl EditTarget for ProgramEdit {
    type Site = ();

    fn site(&self) {}
}

#[derive(Default)]
pub(in crate::fixups) struct ProgramPlanBuilder {
    builder: PlanBuilder<ProgramEdit>,
}

impl ProgramPlanBuilder {
    pub(in crate::fixups) fn new() -> Self {
        Self::default()
    }

    pub(in crate::fixups) fn add_rule(
        &mut self,
        query: &QueryContext<'_>,
        rule: &ProgramRule,
    ) -> &mut Self {
        if !rule.has_candidates(query) {
            return self;
        }
        let mut rejected_cases = Vec::new();
        let mut selected = None;
        for case in &rule.cases {
            let mut context = ProgramCaseContext {
                query,
                evidence: Vec::new(),
            };
            let result = (case.apply)(&mut context).and_then(|recipe| {
                recipe.lower(query).map_err(|mut rejection| {
                    let mut evidence = context.evidence.clone();
                    evidence.append(&mut rejection.evidence);
                    rejection.evidence = evidence;
                    rejection
                })
            });
            match result {
                Ok(prepared) => {
                    selected = Some(PlannedEdit {
                        identity: RuleCaseIdentity {
                            rule: rule.identity.clone(),
                            case: case.name.clone(),
                        },
                        edit: ProgramEdit {
                            prepared,
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
                target: None,
                rejections: rejected_cases,
            });
        }
        self
    }

    pub(in crate::fixups) fn finish(self) -> ProgramPlan {
        ProgramPlan {
            plan: self.builder.finish(),
        }
    }
}

pub(in crate::fixups) struct ProgramPlan {
    plan: Plan<ProgramEdit>,
}

impl ProgramPlan {
    pub(in crate::fixups) fn apply(
        self,
        program: &mut Program,
        logger: &mut dyn TraceLogger,
    ) -> ProgramApplyReport {
        let planned = self.plan.edits.len();
        let mut diagnostics = self.plan.diagnostics;
        if logger.is_enabled() {
            for diagnostic in &diagnostics {
                log_diagnostic(logger, diagnostic);
            }
        }
        let Some(edit) = self.plan.edits.into_iter().next() else {
            return ProgramApplyReport {
                changed: false,
                planned,
                applied: 0,
                diagnostics,
            };
        };
        if !anchors_match(program, &edit.edit.prepared.anchors) {
            diagnostics.push(PlanDiagnostic::MissingTarget {
                contender: edit.identity,
                target: (),
            });
            return ProgramApplyReport {
                changed: false,
                planned,
                applied: 0,
                diagnostics,
            };
        }
        let before = logger.is_enabled().then(|| program.emit());
        let after = before
            .as_ref()
            .map(|_| edit.edit.prepared.replacement.emit());
        if let (Some(before), Some(after)) = (before, after) {
            log_edit(logger, &edit, before, after);
        }
        *program = edit.edit.prepared.replacement;
        ProgramApplyReport {
            changed: true,
            planned,
            applied: 1,
            diagnostics,
        }
    }
}

pub(in crate::fixups) struct ProgramApplyReport {
    pub(in crate::fixups) changed: bool,
    pub(in crate::fixups) planned: usize,
    pub(in crate::fixups) applied: usize,
    #[allow(dead_code)]
    pub(super) diagnostics: Vec<PlanDiagnostic<()>>,
}

fn anchors_match(program: &Program, anchors: &[(usize, String)]) -> bool {
    anchors.iter().all(|(item_index, name)| {
        matches!(
            program.items.get(*item_index),
            Some(Item::Record(record)) if record.name == *name
        )
    })
}

fn log_edit(
    logger: &mut dyn TraceLogger,
    edit: &PlannedEdit<ProgramEdit>,
    before: String,
    after: String,
) {
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
            function: Some("program".into()),
            ast_path: Some("program".into()),
            ..TraceLocation::default()
        },
        before: vec![TraceSnippet::new("program", before.trim_end())],
        after: vec![TraceSnippet::new("program", after.trim_end())],
        facts,
    });
}

fn log_diagnostic(logger: &mut dyn TraceLogger, diagnostic: &PlanDiagnostic<()>) {
    let PlanDiagnostic::CandidateRejected {
        rule, rejections, ..
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
            function: Some("program".into()),
            ast_path: Some("program".into()),
            ..TraceLocation::default()
        },
        before: Vec::new(),
        after: Vec::new(),
        facts,
    });
}
