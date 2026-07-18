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
        cir_len: usize,
        rust_ast_len: usize,
    },
    EffectMismatch {
        at: usize,
        cir: Effect,
        rust_ast: Effect,
    },
}

impl std::fmt::Display for Divergence {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Divergence::LengthMismatch {
                at,
                cir_len,
                rust_ast_len,
            } => write!(
                f,
                "effect traces diverge at #{at}: CIR trace has {cir_len} effect(s), rust_ast trace has {rust_ast_len}"
            ),
            Divergence::EffectMismatch { at, cir, rust_ast } => write!(
                f,
                "effect traces diverge at #{at}: CIR produced {cir:?}, rust_ast produced {rust_ast:?}"
            ),
        }
    }
}

pub fn compare(cir: &EffectTrace, rust_ast: &EffectTrace) -> Result<(), Box<Divergence>> {
    for (at, (cir_effect, rust_effect)) in
        cir.effects.iter().zip(rust_ast.effects.iter()).enumerate()
    {
        if cir_effect != rust_effect {
            return Err(Box::new(Divergence::EffectMismatch {
                at,
                cir: cir_effect.clone(),
                rust_ast: rust_effect.clone(),
            }));
        }
    }
    let (cir_len, rust_ast_len) = (cir.effects.len(), rust_ast.effects.len());
    if cir_len != rust_ast_len {
        return Err(Box::new(Divergence::LengthMismatch {
            at: cir_len.min(rust_ast_len),
            cir_len,
            rust_ast_len,
        }));
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::effects::{AllocId, IntWidth, Location, Value};

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
                cir: Effect::Write {
                    loc: Location {
                        alloc: AllocId(0),
                        byte_offset: 4,
                    },
                    value: int32(2),
                },
                rust_ast: Effect::Write {
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
                cir_len: 6,
                rust_ast_len: 5,
            }))
        );
    }

    #[test]
    fn divergence_display_names_the_index_and_both_sides() {
        let mismatch = Divergence::EffectMismatch {
            at: 2,
            cir: Effect::Exit(3),
            rust_ast: Effect::Exit(4),
        };
        let message = mismatch.to_string();
        assert!(message.contains("#2"));
        assert!(message.contains("CIR produced"));
        assert!(message.contains("rust_ast produced"));
    }
}
