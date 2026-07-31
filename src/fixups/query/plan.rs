use std::collections::{BTreeMap, BTreeSet};

use super::{CaseRejection, RuleCaseIdentity, RuleIdentity};

pub(super) trait PlanSite: Clone + Ord {
    fn overlaps(&self, other: &Self) -> bool;
}

impl PlanSite for () {
    fn overlaps(&self, _other: &Self) -> bool {
        true
    }
}

pub(super) trait EditTarget {
    type Site: PlanSite;

    fn site(&self) -> Self::Site;
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) enum PlanDiagnostic<S> {
    CandidateRejected {
        rule: RuleIdentity,
        target: Option<S>,
        rejections: Vec<CaseRejection>,
    },
    TargetMismatch {
        contender: RuleCaseIdentity,
        candidate: S,
        replacement: S,
    },
    AmbiguousTarget {
        target: S,
        contenders: Vec<RuleCaseIdentity>,
    },
    OverlappingTargets {
        first: S,
        second: S,
    },
    MissingTarget {
        contender: RuleCaseIdentity,
        target: S,
    },
}

pub(super) struct PlannedEdit<E> {
    pub(super) identity: RuleCaseIdentity,
    pub(super) edit: E,
    pub(super) rejected_cases: Vec<CaseRejection>,
}

pub(super) struct PlanBuilder<E: EditTarget> {
    proposals: Vec<PlannedEdit<E>>,
    diagnostics: Vec<PlanDiagnostic<E::Site>>,
}

impl<E: EditTarget> PlanBuilder<E> {
    pub(super) fn new() -> Self {
        Self {
            proposals: Vec::new(),
            diagnostics: Vec::new(),
        }
    }

    pub(super) fn propose(&mut self, edit: PlannedEdit<E>) {
        self.proposals.push(edit);
    }

    pub(super) fn diagnose(&mut self, diagnostic: PlanDiagnostic<E::Site>) {
        self.diagnostics.push(diagnostic);
    }

    /// Groups proposals by target site, drops sites with more than one
    /// contender (`AmbiguousTarget`), then drops any pair of surviving edits
    /// whose sites overlap (`OverlappingTargets`). Both checks emit a
    /// diagnostic for the edits they drop.
    pub(super) fn finish(self) -> Plan<E> {
        let mut diagnostics = self.diagnostics;
        let mut grouped = BTreeMap::<E::Site, Vec<PlannedEdit<E>>>::new();
        for proposal in self.proposals {
            grouped
                .entry(proposal.edit.site())
                .or_default()
                .push(proposal);
        }
        let mut edits = Vec::new();
        for (target, mut contenders) in grouped {
            if contenders.len() == 1 {
                edits.push(contenders.pop().unwrap());
            } else {
                diagnostics.push(PlanDiagnostic::AmbiguousTarget {
                    target,
                    contenders: contenders
                        .into_iter()
                        .map(|contender| contender.identity)
                        .collect(),
                });
            }
        }
        edits.sort_by_key(|edit| edit.edit.site());
        let mut overlapping = BTreeSet::new();
        for first in 0..edits.len() {
            for second in first + 1..edits.len() {
                let first_site = edits[first].edit.site();
                let second_site = edits[second].edit.site();
                if first_site.overlaps(&second_site) {
                    overlapping.insert(first);
                    overlapping.insert(second);
                    diagnostics.push(PlanDiagnostic::OverlappingTargets {
                        first: first_site,
                        second: second_site,
                    });
                }
            }
        }
        let edits = edits
            .into_iter()
            .enumerate()
            .filter_map(|(index, edit)| (!overlapping.contains(&index)).then_some(edit))
            .collect();
        Plan { edits, diagnostics }
    }
}

impl<E: EditTarget> Default for PlanBuilder<E> {
    fn default() -> Self {
        Self::new()
    }
}

pub(super) struct Plan<E: EditTarget> {
    pub(super) edits: Vec<PlannedEdit<E>>,
    pub(super) diagnostics: Vec<PlanDiagnostic<E::Site>>,
}
