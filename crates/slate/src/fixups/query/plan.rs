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

/// Which `Program::items` positions an applied edit actually changed, for
/// `slate-04q.75.56.8` (incremental facts) to splice instead of triggering
/// a full `facts::analyze`. Two disjoint kinds of change, tracked
/// separately because they need different handling: `in_place` items kept
/// their position (only their content changed - a function body edit), so
/// only that item's own facts need re-deriving; `removed` items are gone
/// entirely, which shifts every later item's `item_index` and needs
/// renumbering, not just re-derivation. `unbounded` is an honest escape
/// hatch for edits whose blast radius isn't a small tracked set (e.g.
/// `anonymous_structs`, which can touch any function or type that
/// references the rewritten record) - callers must fall back to a full
/// reanalyze when it's set, ignoring the (empty) `in_place`/`removed`
/// lists.
#[derive(Debug, Clone, Default)]
pub(in crate::fixups) struct TouchedItems {
    pub(in crate::fixups) in_place: Vec<usize>,
    pub(in crate::fixups) removed: Vec<usize>,
    pub(in crate::fixups) unbounded: bool,
}

impl TouchedItems {
    pub(in crate::fixups) fn none() -> Self {
        Self::default()
    }

    pub(in crate::fixups) fn unbounded() -> Self {
        Self {
            unbounded: true,
            ..Self::default()
        }
    }

    #[allow(dead_code)]
    pub(in crate::fixups) fn merge(&mut self, other: TouchedItems) {
        self.unbounded |= other.unbounded;
        self.in_place.extend(other.in_place);
        self.removed.extend(other.removed);
    }
}
