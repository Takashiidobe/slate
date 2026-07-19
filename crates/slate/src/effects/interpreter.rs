use std::collections::{BTreeMap, BTreeSet};

use super::{AllocId, AtomicId, Effect, EffectTrace, IntWidth, Location, OptionValue, Value};

#[allow(clippy::large_enum_variant)]
#[derive(Debug, Clone, PartialEq)]
pub enum Divergence {
    Internal(String),
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
            Divergence::Internal(message) => write!(f, "effect comparison failed: {message}"),
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
    let left = normalized_for_compare(left)?;
    let right = normalized_for_compare(right)?;
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

fn normalized_for_compare(trace: &EffectTrace) -> Result<EffectTrace, Box<Divergence>> {
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
    Ok(EffectTrace {
        effects: effects
            .into_iter()
            .map(|effect| remap_effect(effect, &alloc_map, &atomic_map))
            .collect::<Result<Vec<_>, _>>()?,
    })
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
                Effect::Alloc { .. }
                    | Effect::Dealloc { .. }
                    | Effect::Read { .. }
                    | Effect::Write { .. }
            ) && !matches!(effect, Effect::Call { name, .. } if is_internal_model_call(name))
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

fn is_internal_model_call(name: &str) -> bool {
    matches!(
        name,
        "strlen"
            | "strcmp"
            | "strncmp"
            | "memcmp"
            | "strchr"
            | "strrchr"
            | "strstr"
            | "strpbrk"
            | "strspn"
            | "strcspn"
            | "memchr"
            | "atoi"
            | "atol"
            | "strtol"
            | "strtoul"
            | "strtod"
    )
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
                keep[idx] = needed_locs.remove(loc);
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
        if let Some(atomic) = effect_atomic(effect)
            && !map.contains_key(&atomic)
        {
            map.insert(atomic, AtomicId(map.len() as u32));
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

fn remap_atomic(
    atomic: AtomicId,
    atomic_map: &BTreeMap<AtomicId, AtomicId>,
) -> Result<AtomicId, Box<Divergence>> {
    atomic_map
        .get(&atomic)
        .copied()
        .ok_or_else(|| Box::new(Divergence::Internal(format!("unmapped atomic {atomic:?}"))))
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
) -> Result<Effect, Box<Divergence>> {
    match effect {
        Effect::Alloc { alloc, size } => Ok(Effect::Alloc {
            alloc: remap_alloc(alloc, alloc_map),
            size,
        }),
        Effect::Dealloc { alloc } => Ok(Effect::Dealloc {
            alloc: remap_alloc(alloc, alloc_map),
        }),
        Effect::Write { loc, value } => Ok(Effect::Write {
            loc: remap_loc(loc, alloc_map),
            value: remap_value(value, alloc_map),
        }),
        Effect::Read { loc, value } => Ok(Effect::Read {
            loc: remap_loc(loc, alloc_map),
            value: remap_value(value, alloc_map),
        }),
        Effect::Call { name, args } => Ok(Effect::Call {
            name,
            args: args
                .into_iter()
                .map(|value| remap_value(value, alloc_map))
                .collect(),
        }),
        Effect::Return(value) => Ok(Effect::Return(remap_value(value, alloc_map))),
        Effect::AtomicLoad {
            atomic,
            ordering,
            value,
        } => Ok(Effect::AtomicLoad {
            atomic: remap_atomic(atomic, atomic_map)?,
            ordering,
            value: remap_value(value, alloc_map),
        }),
        Effect::AtomicStore {
            atomic,
            ordering,
            value,
        } => Ok(Effect::AtomicStore {
            atomic: remap_atomic(atomic, atomic_map)?,
            ordering,
            value: remap_value(value, alloc_map),
        }),
        Effect::AtomicRmw {
            atomic,
            op,
            ordering,
            operand,
            old,
            new,
        } => Ok(Effect::AtomicRmw {
            atomic: remap_atomic(atomic, atomic_map)?,
            op,
            ordering,
            operand: remap_value(operand, alloc_map),
            old: remap_value(old, alloc_map),
            new: remap_value(new, alloc_map),
        }),
        Effect::AtomicSwap {
            atomic,
            ordering,
            old,
            new,
        } => Ok(Effect::AtomicSwap {
            atomic: remap_atomic(atomic, atomic_map)?,
            ordering,
            old: remap_value(old, alloc_map),
            new: remap_value(new, alloc_map),
        }),
        Effect::AtomicCompareExchange {
            atomic,
            success,
            failure,
            expected,
            desired,
            old,
            exchanged,
        } => Ok(Effect::AtomicCompareExchange {
            atomic: remap_atomic(atomic, atomic_map)?,
            success,
            failure,
            expected: remap_value(expected, alloc_map),
            desired: remap_value(desired, alloc_map),
            old: remap_value(old, alloc_map),
            exchanged,
        }),
        Effect::FileOpen { file, path, mode } => Ok(Effect::FileOpen { file, path, mode }),
        Effect::FileWrite { file, bytes } => Ok(Effect::FileWrite { file, bytes }),
        Effect::FileClose { file } => Ok(Effect::FileClose { file }),
        Effect::AtomicFence { ordering } => Ok(Effect::AtomicFence { ordering }),
        Effect::Exit(code) => Ok(Effect::Exit(code)),
    }
}

fn remap_value(value: Value, alloc_map: &BTreeMap<AllocId, AllocId>) -> Value {
    match value {
        Value::Ref(loc) => Value::Ref(remap_loc(loc, alloc_map)),
        Value::Int { width, value, .. } => Value::Int {
            width: IntWidth::PointerSized,
            signed: false,
            value: canonical_bits(value, width),
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
        OptionValue::Int { width, value, .. } => OptionValue::Int {
            width: IntWidth::PointerSized,
            signed: false,
            value: canonical_bits(value, width),
        },
        other => other,
    }
}

fn canonical_bits(value: i128, width: IntWidth) -> i128 {
    let bits = match width {
        IntWidth::W8 => 8,
        IntWidth::W16 => 16,
        IntWidth::W32 => 32,
        IntWidth::W64 | IntWidth::PointerSized => 64,
        IntWidth::W128 => return value,
    };
    value & ((1i128 << bits) - 1)
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
