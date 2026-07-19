//! Effect-trace comparator: decides whether two [`super::EffectTrace`]s
//! represent equivalent executions.
//!
//! On a mismatch this reports the first diverging effect rather than just
//! "not equal": which side produced what, and at what index into the trace —
//! a bare `assert_eq!` on two multi-effect vectors doesn't say which
//! allocation or index actually diverged.

use std::collections::{BTreeMap, BTreeSet};

use super::{AllocId, AtomicId, Effect, EffectTrace, IntWidth, Location, OptionValue, Value};

/// Where and how two traces first diverge.
#[allow(clippy::large_enum_variant)]
#[derive(Debug, Clone, PartialEq)]
pub enum Divergence {
    LengthMismatch {
        at: usize,
        left_len: usize,
        right_len: usize,
    },
    EffectMismatch {
        at: usize,
        left: Effect,
        right: Effect,
    },
}

impl std::fmt::Display for Divergence {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Divergence::LengthMismatch {
                at,
                left_len,
                right_len,
            } => write!(
                f,
                "effect traces diverge at #{at}: left trace has {left_len} effect(s), right trace has {right_len}"
            ),
            Divergence::EffectMismatch { at, left, right } => write!(
                f,
                "effect traces diverge at #{at}: left produced {left:?}, right produced {right:?}"
            ),
        }
    }
}

pub fn compare(left: &EffectTrace, right: &EffectTrace) -> Result<(), Box<Divergence>> {
    let left = normalized_for_compare(left);
    let right = normalized_for_compare(right);
    let mut first_mismatch = None;
    for (at, (left_effect, right_effect)) in
        left.effects.iter().zip(right.effects.iter()).enumerate()
    {
        if left_effect != right_effect {
            first_mismatch = Some(Divergence::EffectMismatch {
                at,
                left: left_effect.clone(),
                right: right_effect.clone(),
            });
            break;
        }
    }
    let (left_len, right_len) = (left.effects.len(), right.effects.len());
    let divergence = first_mismatch.or_else(|| {
        (left_len != right_len).then_some(Divergence::LengthMismatch {
            at: left_len.min(right_len),
            left_len,
            right_len,
        })
    });
    if let Some(divergence) = divergence {
        if externally_observable_projection_matches(&left, &right) {
            return Ok(());
        }
        return Err(Box::new(divergence));
    }
    Ok(())
}

fn normalized_for_compare(trace: &EffectTrace) -> EffectTrace {
    let observed = observed_allocs(trace);
    let pruned = prune_dead_writes(&trace.effects);
    let effects: Vec<Effect> = pruned
        .effects
        .iter()
        .filter(|effect| !is_compare_filtered_effect(effect, &observed))
        .cloned()
        .collect();
    let alloc_map = compact_alloc_map(&effects);
    let atomic_map = compact_atomic_map(&effects);
    EffectTrace {
        effects: effects
            .into_iter()
            .map(|effect| remap_effect(effect, &alloc_map, &atomic_map))
            .collect(),
    }
}

fn externally_observable_projection_matches(left: &EffectTrace, right: &EffectTrace) -> bool {
    let left = externally_observable_projection(left);
    if !left.has_external {
        return false;
    }
    let right = externally_observable_projection(right);
    left.effects == right.effects
}

struct ExternalProjection {
    effects: Vec<Effect>,
    has_external: bool,
}

fn externally_observable_projection(trace: &EffectTrace) -> ExternalProjection {
    let effects = trace
        .effects
        .iter()
        .filter(|effect| {
            !matches!(
                effect,
                Effect::Alloc { .. } | Effect::Read { .. } | Effect::Write { .. }
            )
        })
        .cloned()
        .collect::<Vec<_>>();
    let has_external = effects
        .iter()
        .any(|effect| !matches!(effect, Effect::Exit(_)));
    ExternalProjection {
        effects,
        has_external,
    }
}

fn prune_dead_writes(effects: &[Effect]) -> EffectTrace {
    let mut needed_locs = BTreeSet::new();
    let mut keep = vec![true; effects.len()];
    for (idx, effect) in effects.iter().enumerate().rev() {
        match effect {
            Effect::Read { loc, .. } => {
                needed_locs.insert(*loc);
            }
            Effect::Write { loc, .. } => {
                if needed_locs.remove(loc) {
                    keep[idx] = true;
                } else {
                    keep[idx] = false;
                }
            }
            _ => {}
        }
    }
    EffectTrace {
        effects: effects
            .iter()
            .zip(keep)
            .filter_map(|(effect, keep)| keep.then_some(effect.clone()))
            .collect(),
    }
}

fn observed_allocs(trace: &EffectTrace) -> BTreeSet<AllocId> {
    let mut observed = BTreeSet::new();
    for effect in &trace.effects {
        match effect {
            Effect::Dealloc { alloc } => {
                observed.insert(*alloc);
            }
            Effect::Read { loc, value } => {
                observed.insert(loc.alloc);
                observe_value(value.clone(), &mut observed);
            }
            Effect::Call { args, .. } => {
                for value in args {
                    observe_value(value.clone(), &mut observed);
                }
            }
            Effect::Return(value) => observe_value(value.clone(), &mut observed),
            Effect::AtomicLoad { value, .. } | Effect::AtomicStore { value, .. } => {
                observe_value(value.clone(), &mut observed);
            }
            Effect::AtomicRmw {
                operand, old, new, ..
            } => {
                observe_value(operand.clone(), &mut observed);
                observe_value(old.clone(), &mut observed);
                observe_value(new.clone(), &mut observed);
            }
            Effect::AtomicSwap { old, new, .. } => {
                observe_value(old.clone(), &mut observed);
                observe_value(new.clone(), &mut observed);
            }
            Effect::AtomicCompareExchange {
                expected,
                desired,
                old,
                ..
            } => {
                observe_value(expected.clone(), &mut observed);
                observe_value(desired.clone(), &mut observed);
                observe_value(old.clone(), &mut observed);
            }
            Effect::Alloc { .. }
            | Effect::FileOpen { .. }
            | Effect::FileWrite { .. }
            | Effect::FileClose { .. }
            | Effect::AtomicFence { .. }
            | Effect::Write { .. }
            | Effect::Exit(_) => {}
        }
    }
    observed
}

fn observe_value(value: Value, observed: &mut BTreeSet<AllocId>) {
    match value {
        Value::Ref(loc) => {
            observed.insert(loc.alloc);
        }
        Value::AtomicResult { value, .. } => observe_option_value(value, observed),
        Value::Tuple(values) => {
            for value in values {
                observe_value(value, observed);
            }
        }
        Value::Option(Some(value)) => observe_option_value(value, observed),
        Value::Int { .. }
        | Value::Float(_)
        | Value::Bool(_)
        | Value::File(_)
        | Value::Atomic(_)
        | Value::BlockLabel(_)
        | Value::Null
        | Value::Bytes(_)
        | Value::Option(None) => {}
    }
}

fn observe_option_value(value: OptionValue, observed: &mut BTreeSet<AllocId>) {
    if let OptionValue::Ref(loc) = value {
        observed.insert(loc.alloc);
    }
}

fn is_compare_filtered_effect(effect: &Effect, observed: &BTreeSet<AllocId>) -> bool {
    match effect {
        Effect::Alloc { .. } => true,
        Effect::Write { loc, .. } => !observed.contains(&loc.alloc),
        _ => false,
    }
}

fn compact_alloc_map(effects: &[Effect]) -> BTreeMap<AllocId, AllocId> {
    let mut map = BTreeMap::new();
    for effect in effects {
        for alloc in effect_allocs(effect) {
            if !map.contains_key(&alloc) {
                map.insert(alloc, AllocId(map.len() as u32));
            }
        }
    }
    map
}

fn compact_atomic_map(effects: &[Effect]) -> BTreeMap<AtomicId, AtomicId> {
    let mut map = BTreeMap::new();
    for effect in effects {
        if let Some(atomic) = effect_atomic(effect) {
            if !map.contains_key(&atomic) {
                map.insert(atomic, AtomicId(map.len() as u32));
            }
        }
    }
    map
}

fn effect_atomic(effect: &Effect) -> Option<AtomicId> {
    match effect {
        Effect::AtomicLoad { atomic, .. }
        | Effect::AtomicStore { atomic, .. }
        | Effect::AtomicRmw { atomic, .. }
        | Effect::AtomicSwap { atomic, .. }
        | Effect::AtomicCompareExchange { atomic, .. } => Some(*atomic),
        _ => None,
    }
}

fn remap_atomic(atomic: AtomicId, atomic_map: &BTreeMap<AtomicId, AtomicId>) -> AtomicId {
    *atomic_map
        .get(&atomic)
        .unwrap_or_else(|| panic!("effects::interpreter: unmapped atomic {atomic:?}"))
}

fn effect_allocs(effect: &Effect) -> Vec<AllocId> {
    let mut allocs = Vec::new();
    match effect {
        Effect::Alloc { alloc, .. } | Effect::Dealloc { alloc } => allocs.push(*alloc),
        Effect::Write { loc, value } | Effect::Read { loc, value } => {
            allocs.push(loc.alloc);
            value_allocs(value.clone(), &mut allocs);
        }
        Effect::Call { args, .. } => {
            for value in args {
                value_allocs(value.clone(), &mut allocs);
            }
        }
        Effect::Return(value)
        | Effect::AtomicLoad { value, .. }
        | Effect::AtomicStore { value, .. } => value_allocs(value.clone(), &mut allocs),
        Effect::AtomicRmw {
            operand, old, new, ..
        } => {
            value_allocs(operand.clone(), &mut allocs);
            value_allocs(old.clone(), &mut allocs);
            value_allocs(new.clone(), &mut allocs);
        }
        Effect::AtomicSwap { old, new, .. } => {
            value_allocs(old.clone(), &mut allocs);
            value_allocs(new.clone(), &mut allocs);
        }
        Effect::AtomicCompareExchange {
            expected,
            desired,
            old,
            ..
        } => {
            value_allocs(expected.clone(), &mut allocs);
            value_allocs(desired.clone(), &mut allocs);
            value_allocs(old.clone(), &mut allocs);
        }
        Effect::FileOpen { .. }
        | Effect::FileWrite { .. }
        | Effect::FileClose { .. }
        | Effect::AtomicFence { .. }
        | Effect::Exit(_) => {}
    }
    allocs
}

fn value_allocs(value: Value, allocs: &mut Vec<AllocId>) {
    match value {
        Value::Ref(loc) => allocs.push(loc.alloc),
        Value::AtomicResult { value, .. } => option_value_allocs(value, allocs),
        Value::Tuple(values) => {
            for value in values {
                value_allocs(value, allocs);
            }
        }
        Value::Option(Some(value)) => option_value_allocs(value, allocs),
        Value::Int { .. }
        | Value::Float(_)
        | Value::Bool(_)
        | Value::File(_)
        | Value::Atomic(_)
        | Value::BlockLabel(_)
        | Value::Null
        | Value::Bytes(_)
        | Value::Option(None) => {}
    }
}

fn option_value_allocs(value: OptionValue, allocs: &mut Vec<AllocId>) {
    if let OptionValue::Ref(loc) = value {
        allocs.push(loc.alloc);
    }
}

fn remap_effect(
    effect: Effect,
    alloc_map: &BTreeMap<AllocId, AllocId>,
    atomic_map: &BTreeMap<AtomicId, AtomicId>,
) -> Effect {
    match effect {
        Effect::Alloc { alloc, size } => Effect::Alloc {
            alloc: remap_alloc(alloc, alloc_map),
            size,
        },
        Effect::Dealloc { alloc } => Effect::Dealloc {
            alloc: remap_alloc(alloc, alloc_map),
        },
        Effect::Write { loc, value } => Effect::Write {
            loc: remap_loc(loc, alloc_map),
            value: remap_value(value, alloc_map),
        },
        Effect::Read { loc, value } => Effect::Read {
            loc: remap_loc(loc, alloc_map),
            value: remap_value(value, alloc_map),
        },
        Effect::Call { name, args } => Effect::Call {
            name,
            args: args
                .into_iter()
                .map(|value| remap_value(value, alloc_map))
                .collect(),
        },
        Effect::Return(value) => Effect::Return(remap_value(value, alloc_map)),
        Effect::AtomicLoad {
            atomic,
            ordering,
            value,
        } => Effect::AtomicLoad {
            atomic: remap_atomic(atomic, atomic_map),
            ordering,
            value: remap_value(value, alloc_map),
        },
        Effect::AtomicStore {
            atomic,
            ordering,
            value,
        } => Effect::AtomicStore {
            atomic: remap_atomic(atomic, atomic_map),
            ordering,
            value: remap_value(value, alloc_map),
        },
        Effect::AtomicRmw {
            atomic,
            op,
            ordering,
            operand,
            old,
            new,
        } => Effect::AtomicRmw {
            atomic: remap_atomic(atomic, atomic_map),
            op,
            ordering,
            operand: remap_value(operand, alloc_map),
            old: remap_value(old, alloc_map),
            new: remap_value(new, alloc_map),
        },
        Effect::AtomicSwap {
            atomic,
            ordering,
            old,
            new,
        } => Effect::AtomicSwap {
            atomic: remap_atomic(atomic, atomic_map),
            ordering,
            old: remap_value(old, alloc_map),
            new: remap_value(new, alloc_map),
        },
        Effect::AtomicCompareExchange {
            atomic,
            success,
            failure,
            expected,
            desired,
            old,
            exchanged,
        } => Effect::AtomicCompareExchange {
            atomic: remap_atomic(atomic, atomic_map),
            success,
            failure,
            expected: remap_value(expected, alloc_map),
            desired: remap_value(desired, alloc_map),
            old: remap_value(old, alloc_map),
            exchanged,
        },
        Effect::FileOpen { file, path, mode } => Effect::FileOpen { file, path, mode },
        Effect::FileWrite { file, bytes } => Effect::FileWrite { file, bytes },
        Effect::FileClose { file } => Effect::FileClose { file },
        Effect::AtomicFence { ordering } => Effect::AtomicFence { ordering },
        Effect::Exit(code) => Effect::Exit(code),
    }
}

fn remap_value(value: Value, alloc_map: &BTreeMap<AllocId, AllocId>) -> Value {
    match value {
        Value::Ref(loc) => Value::Ref(remap_loc(loc, alloc_map)),
        Value::Int { signed, value, .. } => Value::Int {
            width: IntWidth::PointerSized,
            signed,
            value,
        },
        Value::AtomicResult { ok, value } => Value::AtomicResult {
            ok,
            value: remap_option_value(value, alloc_map),
        },
        Value::Tuple(values) => Value::Tuple(
            values
                .into_iter()
                .map(|value| remap_value(value, alloc_map))
                .collect(),
        ),
        Value::Option(Some(value)) => Value::Option(Some(remap_option_value(value, alloc_map))),
        other => other,
    }
}

fn remap_option_value(value: OptionValue, alloc_map: &BTreeMap<AllocId, AllocId>) -> OptionValue {
    match value {
        OptionValue::Ref(loc) => OptionValue::Ref(remap_loc(loc, alloc_map)),
        other => other,
    }
}

fn remap_loc(loc: Location, alloc_map: &BTreeMap<AllocId, AllocId>) -> Location {
    Location {
        alloc: remap_alloc(loc.alloc, alloc_map),
        byte_offset: loc.byte_offset,
    }
}

fn remap_alloc(alloc: AllocId, alloc_map: &BTreeMap<AllocId, AllocId>) -> AllocId {
    alloc_map.get(&alloc).copied().unwrap_or(alloc)
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::effects::{AllocId, IntWidth, Location, OptionValue, Value};

    fn int32(value: i32) -> Value {
        Value::Int {
            width: IntWidth::W32,
            signed: true,
            value: value as i128,
        }
    }

    fn normalized_int(value: i32) -> Value {
        Value::Int {
            width: IntWidth::PointerSized,
            signed: true,
            value: value as i128,
        }
    }

    /// The malloc/array fixture's normalized effect sequence:
    /// `p = malloc(2 * sizeof(int)); p[0] = 1; p[1] = 2; return p[0] + p[1];`
    fn malloc_array_trace() -> EffectTrace {
        let alloc = AllocId(0);
        EffectTrace {
            effects: vec![
                Effect::Alloc { alloc, size: 8 },
                Effect::Write {
                    loc: Location {
                        alloc,
                        byte_offset: 0,
                    },
                    value: int32(1),
                },
                Effect::Write {
                    loc: Location {
                        alloc,
                        byte_offset: 4,
                    },
                    value: int32(2),
                },
                Effect::Read {
                    loc: Location {
                        alloc,
                        byte_offset: 0,
                    },
                    value: int32(1),
                },
                Effect::Read {
                    loc: Location {
                        alloc,
                        byte_offset: 4,
                    },
                    value: int32(2),
                },
                Effect::Exit(3),
            ],
        }
    }

    #[test]
    fn identical_traces_from_the_two_walkers_are_equivalent() {
        let cir = malloc_array_trace();
        let rust_ast = malloc_array_trace();
        assert_eq!(compare(&cir, &rust_ast), Ok(()));
    }

    #[test]
    fn a_wrong_written_value_is_reported_at_its_exact_index() {
        let cir = malloc_array_trace();
        let mut rust_ast = malloc_array_trace();
        rust_ast.effects[2] = Effect::Write {
            loc: Location {
                alloc: AllocId(0),
                byte_offset: 4,
            },
            value: int32(99),
        };

        let divergence = compare(&cir, &rust_ast).unwrap_err();
        assert_eq!(
            *divergence,
            Divergence::EffectMismatch {
                at: 1,
                left: Effect::Write {
                    loc: Location {
                        alloc: AllocId(0),
                        byte_offset: 4,
                    },
                    value: normalized_int(2),
                },
                right: Effect::Write {
                    loc: Location {
                        alloc: AllocId(0),
                        byte_offset: 4,
                    },
                    value: normalized_int(99),
                },
            }
        );
    }

    #[test]
    fn a_missing_trailing_effect_is_reported_as_a_length_mismatch() {
        let cir = malloc_array_trace();
        let mut rust_ast = malloc_array_trace();
        rust_ast.effects.pop();

        assert_eq!(
            compare(&cir, &rust_ast),
            Err(Box::new(Divergence::LengthMismatch {
                at: 4,
                left_len: 5,
                right_len: 4,
            }))
        );
    }

    #[test]
    fn divergence_display_names_the_index_and_both_sides() {
        let mismatch = Divergence::EffectMismatch {
            at: 2,
            left: Effect::Exit(3),
            right: Effect::Exit(4),
        };
        let message = mismatch.to_string();
        assert!(message.contains("#2"));
        assert!(message.contains("left produced"));
        assert!(message.contains("right produced"));
    }

    #[test]
    fn rust_to_rust_option_traces_compare_with_the_same_generic_comparator() {
        let left = EffectTrace {
            effects: vec![Effect::Call {
                name: "debug".to_string(),
                args: vec![Value::Option(Some(OptionValue::Int {
                    width: IntWidth::PointerSized,
                    signed: false,
                    value: 2,
                }))],
            }],
        };
        let right = left.clone();
        assert_eq!(compare(&left, &right), Ok(()));
    }

    #[test]
    fn copy_only_allocations_are_ignored_in_comparison_view() {
        let left = EffectTrace {
            effects: vec![
                Effect::Alloc {
                    alloc: AllocId(0),
                    size: 8,
                },
                Effect::Write {
                    loc: Location {
                        alloc: AllocId(0),
                        byte_offset: 0,
                    },
                    value: int32(1),
                },
                Effect::Write {
                    loc: Location {
                        alloc: AllocId(0),
                        byte_offset: 4,
                    },
                    value: int32(2),
                },
                Effect::Alloc {
                    alloc: AllocId(1),
                    size: 8,
                },
                Effect::Write {
                    loc: Location {
                        alloc: AllocId(1),
                        byte_offset: 0,
                    },
                    value: int32(1),
                },
                Effect::Write {
                    loc: Location {
                        alloc: AllocId(1),
                        byte_offset: 4,
                    },
                    value: int32(2),
                },
                Effect::Read {
                    loc: Location {
                        alloc: AllocId(1),
                        byte_offset: 0,
                    },
                    value: int32(1),
                },
                Effect::Exit(1),
            ],
        };
        let right = EffectTrace {
            effects: vec![
                Effect::Alloc {
                    alloc: AllocId(0),
                    size: 8,
                },
                Effect::Write {
                    loc: Location {
                        alloc: AllocId(0),
                        byte_offset: 0,
                    },
                    value: int32(1),
                },
                Effect::Write {
                    loc: Location {
                        alloc: AllocId(0),
                        byte_offset: 4,
                    },
                    value: int32(2),
                },
                Effect::Read {
                    loc: Location {
                        alloc: AllocId(0),
                        byte_offset: 0,
                    },
                    value: int32(1),
                },
                Effect::Exit(1),
            ],
        };

        assert_eq!(compare(&left, &right), Ok(()));
    }

    #[test]
    fn stack_allocation_timing_is_ignored_in_comparison_view() {
        let left = EffectTrace {
            effects: vec![
                Effect::Alloc {
                    alloc: AllocId(0),
                    size: 4,
                },
                Effect::Write {
                    loc: Location {
                        alloc: AllocId(0),
                        byte_offset: 0,
                    },
                    value: int32(1),
                },
                Effect::Read {
                    loc: Location {
                        alloc: AllocId(0),
                        byte_offset: 0,
                    },
                    value: int32(1),
                },
                Effect::Alloc {
                    alloc: AllocId(1),
                    size: 4,
                },
                Effect::Write {
                    loc: Location {
                        alloc: AllocId(1),
                        byte_offset: 0,
                    },
                    value: int32(2),
                },
                Effect::Read {
                    loc: Location {
                        alloc: AllocId(1),
                        byte_offset: 0,
                    },
                    value: int32(2),
                },
                Effect::Exit(0),
            ],
        };
        let right = EffectTrace {
            effects: vec![
                Effect::Alloc {
                    alloc: AllocId(0),
                    size: 4,
                },
                Effect::Alloc {
                    alloc: AllocId(1),
                    size: 4,
                },
                Effect::Write {
                    loc: Location {
                        alloc: AllocId(0),
                        byte_offset: 0,
                    },
                    value: int32(1),
                },
                Effect::Read {
                    loc: Location {
                        alloc: AllocId(0),
                        byte_offset: 0,
                    },
                    value: int32(1),
                },
                Effect::Write {
                    loc: Location {
                        alloc: AllocId(1),
                        byte_offset: 0,
                    },
                    value: int32(2),
                },
                Effect::Read {
                    loc: Location {
                        alloc: AllocId(1),
                        byte_offset: 0,
                    },
                    value: int32(2),
                },
                Effect::Exit(0),
            ],
        };

        assert_eq!(compare(&left, &right), Ok(()));
    }

    #[test]
    fn external_projection_allows_benign_memory_schedule_differences() {
        let left = EffectTrace {
            effects: vec![
                Effect::Alloc {
                    alloc: AllocId(0),
                    size: 8,
                },
                Effect::Write {
                    loc: Location {
                        alloc: AllocId(0),
                        byte_offset: 0,
                    },
                    value: int32(2),
                },
                Effect::Read {
                    loc: Location {
                        alloc: AllocId(0),
                        byte_offset: 0,
                    },
                    value: int32(2),
                },
                Effect::Call {
                    name: "printf".to_string(),
                    args: vec![int32(2)],
                },
                Effect::Exit(0),
            ],
        };
        let right = EffectTrace {
            effects: vec![
                Effect::Call {
                    name: "printf".to_string(),
                    args: vec![int32(2)],
                },
                Effect::Exit(0),
            ],
        };

        assert_eq!(compare(&left, &right), Ok(()));
    }
}
