use std::collections::{BTreeMap, BTreeSet};

use crate::fixups::trace::{Pass, RewriteEvent, TraceLocation, TraceLogger, TraceSnippet, fact};
use crate::rust_ast::{ExternDecl, Item, Program};

use super::rewrite::evidence_trace_fact;
use super::{
    DefinitionKind, DefinitionLocation, DefinitionSelector, DefinitionSite, Evidence, QueryContext,
    Rejection, RuleIdentity,
};

pub(in crate::fixups) struct DefinitionRule {
    identity: RuleIdentity,
    selector: DefinitionSelector,
}

impl DefinitionRule {
    pub(in crate::fixups) fn function(
        pass: Pass,
        rule: impl Into<String>,
        name: impl Into<String>,
    ) -> Self {
        Self {
            identity: RuleIdentity::new(pass, rule),
            selector: DefinitionSelector {
                kind: DefinitionKind::Function,
                name: name.into(),
            },
        }
    }

    pub(in crate::fixups) fn extern_function(
        pass: Pass,
        rule: impl Into<String>,
        name: impl Into<String>,
    ) -> Self {
        Self {
            identity: RuleIdentity::new(pass, rule),
            selector: DefinitionSelector {
                kind: DefinitionKind::ExternFunction,
                name: name.into(),
            },
        }
    }
}

pub(in crate::fixups) struct DeleteDefinition {
    target: DefinitionSite,
    evidence: Vec<Evidence>,
}

struct PlannedDefinitionEdit {
    identity: RuleIdentity,
    deletion: DeleteDefinition,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) enum DefinitionPlanDiagnostic {
    CandidateRejected {
        rule: RuleIdentity,
        target: DefinitionSite,
        rejection: Rejection,
    },
    AmbiguousTarget {
        target: DefinitionLocation,
        contenders: Vec<RuleIdentity>,
    },
    OverlappingTargets {
        item_index: usize,
    },
    MissingTarget {
        rule: RuleIdentity,
        target: DefinitionSite,
    },
}

pub(in crate::fixups) struct DefinitionPlanBuilder {
    proposals: Vec<PlannedDefinitionEdit>,
    diagnostics: Vec<DefinitionPlanDiagnostic>,
}

impl DefinitionPlanBuilder {
    pub(in crate::fixups) fn new() -> Self {
        Self {
            proposals: Vec::new(),
            diagnostics: Vec::new(),
        }
    }

    pub(in crate::fixups) fn add_rule(
        &mut self,
        query: &QueryContext<'_>,
        rule: &DefinitionRule,
    ) -> &mut Self {
        for definition in query.definitions(&rule.selector) {
            match query.zero_users(definition) {
                Ok(proof) => self.proposals.push(PlannedDefinitionEdit {
                    identity: rule.identity.clone(),
                    deletion: DeleteDefinition {
                        target: proof.value.definition,
                        evidence: proof.evidence,
                    },
                }),
                Err(rejection) => {
                    self.diagnostics
                        .push(DefinitionPlanDiagnostic::CandidateRejected {
                            rule: rule.identity.clone(),
                            target: definition.clone(),
                            rejection,
                        });
                }
            }
        }
        self
    }

    pub(in crate::fixups) fn finish(self) -> DefinitionPlan {
        let mut diagnostics = self.diagnostics;
        let mut grouped = BTreeMap::<DefinitionLocation, Vec<PlannedDefinitionEdit>>::new();
        for proposal in self.proposals {
            grouped
                .entry(proposal.deletion.target.location.clone())
                .or_default()
                .push(proposal);
        }
        let mut edits = Vec::new();
        for (target, mut contenders) in grouped {
            if contenders.len() == 1 {
                edits.push(contenders.pop().unwrap());
            } else {
                diagnostics.push(DefinitionPlanDiagnostic::AmbiguousTarget {
                    target,
                    contenders: contenders
                        .into_iter()
                        .map(|contender| contender.identity)
                        .collect(),
                });
            }
        }
        let whole_items = edits
            .iter()
            .filter_map(|edit| match edit.deletion.target.location {
                DefinitionLocation::Item(item_index) => Some(item_index),
                DefinitionLocation::ExternDecl { .. } => None,
            })
            .collect::<BTreeSet<_>>();
        let overlapping = edits
            .iter()
            .filter_map(|edit| match edit.deletion.target.location {
                DefinitionLocation::ExternDecl { item_index, .. }
                    if whole_items.contains(&item_index) =>
                {
                    Some(item_index)
                }
                _ => None,
            })
            .collect::<BTreeSet<_>>();
        for item_index in &overlapping {
            diagnostics.push(DefinitionPlanDiagnostic::OverlappingTargets {
                item_index: *item_index,
            });
        }
        edits.retain(|edit| !overlapping.contains(&edit.deletion.target.location.item_index()));
        edits.sort_by(|left, right| {
            left.deletion
                .target
                .location
                .cmp(&right.deletion.target.location)
        });
        DefinitionPlan { edits, diagnostics }
    }
}

impl Default for DefinitionPlanBuilder {
    fn default() -> Self {
        Self::new()
    }
}

pub(in crate::fixups) struct DefinitionPlan {
    edits: Vec<PlannedDefinitionEdit>,
    diagnostics: Vec<DefinitionPlanDiagnostic>,
}

impl DefinitionPlan {
    pub(in crate::fixups) fn diagnostics(&self) -> &[DefinitionPlanDiagnostic] {
        &self.diagnostics
    }

    pub(in crate::fixups) fn apply(
        self,
        program: &mut Program,
        logger: &mut dyn TraceLogger,
    ) -> DefinitionApplyReport {
        let planned = self.edits.len();
        let mut diagnostics = self.diagnostics;
        let mut by_item = BTreeMap::<usize, Vec<PlannedDefinitionEdit>>::new();
        for edit in self.edits {
            by_item
                .entry(edit.deletion.target.location.item_index())
                .or_default()
                .push(edit);
        }
        let mut applied = 0;
        for (item_index, mut edits) in by_item.into_iter().rev() {
            edits.sort_by(|left, right| {
                right
                    .deletion
                    .target
                    .location
                    .cmp(&left.deletion.target.location)
            });
            if item_index >= program.items.len() {
                for edit in edits {
                    diagnostics.push(DefinitionPlanDiagnostic::MissingTarget {
                        rule: edit.identity,
                        target: edit.deletion.target,
                    });
                }
                continue;
            }
            if matches!(
                edits.first().map(|edit| &edit.deletion.target.location),
                Some(DefinitionLocation::Item(_))
            ) {
                let edit = edits.pop().unwrap();
                if definition_matches(&program.items[item_index], &edit.deletion.target) {
                    let before = item_snippet(&program.items[item_index]);
                    program.items.remove(item_index);
                    log_deletion(logger, &edit, before);
                    applied += 1;
                } else {
                    diagnostics.push(DefinitionPlanDiagnostic::MissingTarget {
                        rule: edit.identity,
                        target: edit.deletion.target,
                    });
                }
                continue;
            }
            let Item::ExternBlock { abi, decls } = &mut program.items[item_index] else {
                for edit in edits {
                    diagnostics.push(DefinitionPlanDiagnostic::MissingTarget {
                        rule: edit.identity,
                        target: edit.deletion.target,
                    });
                }
                continue;
            };
            for edit in edits {
                let DefinitionLocation::ExternDecl { decl_index, .. } =
                    edit.deletion.target.location
                else {
                    unreachable!()
                };
                if decls
                    .get(decl_index)
                    .is_some_and(|decl| extern_definition_matches(decl, &edit.deletion.target))
                {
                    let before = extern_decl_snippet(abi, &decls[decl_index]);
                    decls.remove(decl_index);
                    log_deletion(logger, &edit, before);
                    applied += 1;
                } else {
                    diagnostics.push(DefinitionPlanDiagnostic::MissingTarget {
                        rule: edit.identity,
                        target: edit.deletion.target,
                    });
                }
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

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct DefinitionApplyReport {
    pub(in crate::fixups) changed: bool,
    pub(in crate::fixups) planned: usize,
    pub(in crate::fixups) applied: usize,
    pub(in crate::fixups) diagnostics: Vec<DefinitionPlanDiagnostic>,
}

fn definition_matches(item: &Item, definition: &DefinitionSite) -> bool {
    matches!(
        (item, definition.kind),
        (Item::Fn(function), DefinitionKind::Function) if function.name == definition.name
    )
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

fn log_deletion(logger: &mut dyn TraceLogger, edit: &PlannedDefinitionEdit, before: String) {
    if !logger.is_enabled() {
        return;
    }
    let mut facts = vec![
        fact("query_rule", edit.identity.name.clone()),
        fact("query_case", "zero_users"),
    ];
    facts.extend(edit.deletion.evidence.iter().map(evidence_trace_fact));
    logger.rewrite(RewriteEvent {
        pass: edit.identity.pass,
        kind: edit.identity.name.clone(),
        location: TraceLocation {
            function: Some(edit.deletion.target.name.clone()),
            ..TraceLocation::default()
        },
        before: vec![TraceSnippet::new("definition", before)],
        after: Vec::new(),
        facts,
    });
}
