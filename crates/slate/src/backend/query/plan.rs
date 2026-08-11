use std::collections::{BTreeMap, BTreeSet};

use super::{CaseRejection, RuleCaseIdentity, RuleIdentity};

pub(super) trait PlanSite: Clone + Ord {
    fn overlaps(&self, other: &Self) -> bool;

    fn internal_overlaps(&self) -> Vec<(Self, Self)> {
        Vec::new()
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
    #[expect(
        dead_code,
        reason = "query API surface not yet wired into a fixup rule"
    )]
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

    const OVERLAP_WINDOW: usize = 64;

    pub(super) fn finish(self, max_edits: usize) -> Plan<E> {
        let mut diagnostics = self.diagnostics;
        let mut grouped = BTreeMap::<E::Site, Vec<PlannedEdit<E>>>::new();
        for proposal in self.proposals {
            let site = proposal.edit.site();
            let internal_overlaps = site.internal_overlaps();
            if internal_overlaps.is_empty() {
                grouped.entry(site).or_default().push(proposal);
            } else {
                diagnostics.extend(
                    internal_overlaps.into_iter().map(|(first, second)| {
                        PlanDiagnostic::OverlappingTargets { first, second }
                    }),
                );
            }
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
        edits.truncate(max_edits);
        let mut overlapping = BTreeSet::new();
        for first in 0..edits.len() {
            let window_end = (first + 1 + Self::OVERLAP_WINDOW).min(edits.len());
            for second in first + 1..window_end {
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
