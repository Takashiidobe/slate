//! Effect-trace comparator: decides whether two [`super::EffectTrace`]s
//! represent equivalent executions.
//!
//! On a mismatch this reports the first diverging effect rather than just
//! "not equal": which side produced what, and at what index into the trace —
//! a bare `assert_eq!` on two multi-effect vectors doesn't say which
//! allocation or index actually diverged.

use super::{Effect, EffectTrace};

/// Where and how two traces first diverge.
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
    for (at, (left_effect, right_effect)) in
        left.effects.iter().zip(right.effects.iter()).enumerate()
    {
        if left_effect != right_effect {
            return Err(Box::new(Divergence::EffectMismatch {
                at,
                left: left_effect.clone(),
                right: right_effect.clone(),
            }));
        }
    }
    let (left_len, right_len) = (left.effects.len(), right.effects.len());
    if left_len != right_len {
        return Err(Box::new(Divergence::LengthMismatch {
            at: left_len.min(right_len),
            left_len,
            right_len,
        }));
    }
    Ok(())
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

    /// The malloc/array fixture's effect sequence, shared by the CIR walker
    /// (slate-0hf.2) and the idiomatized Vec walker (slate-0hf.3):
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
                at: 2,
                left: Effect::Write {
                    loc: Location {
                        alloc: AllocId(0),
                        byte_offset: 4,
                    },
                    value: int32(2),
                },
                right: Effect::Write {
                    loc: Location {
                        alloc: AllocId(0),
                        byte_offset: 4,
                    },
                    value: int32(99),
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
                at: 5,
                left_len: 6,
                right_len: 5,
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
}
