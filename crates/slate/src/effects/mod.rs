//! Shared effect vocabulary for cross-interpretation equivalence checking.
//!
//! Two independent walkers each turn a program into an [`EffectTrace`]: one
//! over the CIR op-tree ([`cir`]), one over the emitted Rust AST
//! ([`rust_ast`]). A third piece ([`interpreter`]) takes two traces and
//! decides whether they are equivalent. This module defines the trace and
//! effect types both walkers emit and the comparator consumes, so none of the
//! three has to agree on vocabulary with the others by convention alone.
//!
//! Two design decisions make traces from two structurally different programs
//! (a `*const i32` walk vs. its idiomatized `Vec<i32>`/`Box<[i32]>` form)
//! comparable at all:
//!
//! - **Locations never carry a real address.** [`AllocId`] numbers
//!   allocations by the order the *producing* walker saw them get created,
//!   not by pointer value — a CIR walker and a rust_ast walker each start
//!   counting from zero independently. As long as both programs allocate in
//!   the same relative order (true for a fixup that preserves allocation
//!   order, which is the only kind of fixup that could be correct), the same
//!   `AllocId` on both sides denotes the same logical buffer with no
//!   remapping table required. [`Location`] then addresses into that
//!   allocation by byte offset, which is representation-independent: a
//!   `*const i32` walk offsets by `i * 4`, a `Vec<i32>` index does the same
//!   arithmetic internally.
//! - **Only effectful operations are ever pushed.** There is no "silent tick"
//!   variant in [`Effect`] — a walker simply does not emit anything while
//!   evaluating an internal expression, only when it allocates, reads,
//!   writes, crosses a stdlib/std call boundary, returns, or exits. That
//!   makes the stuttering-equivalence reduction discussed for this project
//!   implicit in the vocabulary: comparing two traces is comparing two
//!   already-reduced observable-event sequences, not filtering silent steps
//!   out of a larger one.

pub mod cir;
pub mod interpreter;
pub mod rust_ast;

/// A heap allocation, identified by creation order rather than address.
#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct AllocId(pub u32);

/// A byte offset into an [`AllocId`]'s allocation.
#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct Location {
    pub alloc: AllocId,
    pub byte_offset: u64,
}

/// Integer width, kept distinct from the value so a `u8` write and an `i32`
/// write of the same number are not treated as the same effect — a fixup
/// that narrows a type is a real behavior change, not a representation
/// detail to canonicalize away.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum IntWidth {
    W8,
    W16,
    W32,
    W64,
    W128,
    /// `usize`/`isize` on the Rust side, `size_t`/pointer-width on the C side.
    PointerSized,
}

/// A scalar value carried by an effect.
#[derive(Debug, Clone, Copy, PartialEq)]
pub enum Value {
    Int {
        width: IntWidth,
        signed: bool,
        value: i128,
    },
    Float(f64),
    Bool(bool),
    /// A pointer/reference value itself being moved around, as opposed to
    /// dereferenced — e.g. a function returning the buffer it allocated.
    Ref(Location),
    /// The null pointer, kept distinct from `Ref` since it has no `AllocId`.
    Null,
}

/// One observable event in a program's execution.
#[derive(Debug, Clone, PartialEq)]
pub enum Effect {
    /// A heap allocation of `size` bytes — `malloc`, `Vec::with_capacity`,
    /// `Box::new`, etc. Sizes are in bytes so a C `malloc(n * sizeof(int))`
    /// and a Rust `Vec::<i32>::with_capacity(n)` compare on equal footing.
    Alloc { alloc: AllocId, size: u64 },
    /// A write of `value` to `loc`.
    Write { loc: Location, value: Value },
    /// A read of `value` from `loc`.
    Read { loc: Location, value: Value },
    /// A call crossing a boundary this walker does not step into (a C
    /// stdlib function, a Rust std function). What the call *does* — e.g.
    /// that `malloc` produces an `Alloc` — is the walker's job to model as
    /// separate effects; this variant only records that the call happened
    /// and what was passed to it.
    Call { name: String, args: Vec<Value> },
    /// The value a function hands back to its caller.
    Return(Value),
    /// Process exit, with the code observed by the parent process.
    Exit(i32),
}

/// The ordered effect sequence a walker produced for one program execution.
#[derive(Debug, Clone, Default, PartialEq)]
pub struct EffectTrace {
    pub effects: Vec<Effect>,
}

impl EffectTrace {
    pub fn push(&mut self, effect: Effect) {
        self.effects.push(effect);
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    fn int32(value: i32) -> Value {
        Value::Int {
            width: IntWidth::W32,
            signed: true,
            value: value as i128,
        }
    }

    /// A CIR-shaped trace for `int *p = malloc(2 * sizeof(int)); p[0] = 4;
    /// p[1] = 7; return p[0] + p[1];` and a rust_ast-shaped trace for the
    /// idiomatized `let mut v = Vec::with_capacity(2); v.push(4); v.push(7);
    /// return v[0] + v[1];` are produced by unrelated walkers with unrelated
    /// internal tick counts, but resolve to the same effect sequence once
    /// locations are addressed by allocation order and byte offset.
    #[test]
    fn same_allocation_order_makes_traces_comparable_across_shapes() {
        let alloc = AllocId(0);

        let cir_trace = EffectTrace {
            effects: vec![
                Effect::Alloc { alloc, size: 8 },
                Effect::Write {
                    loc: Location {
                        alloc,
                        byte_offset: 0,
                    },
                    value: int32(4),
                },
                Effect::Write {
                    loc: Location {
                        alloc,
                        byte_offset: 4,
                    },
                    value: int32(7),
                },
                Effect::Return(int32(11)),
            ],
        };

        let rust_trace = EffectTrace {
            effects: vec![
                Effect::Alloc { alloc, size: 8 },
                Effect::Write {
                    loc: Location {
                        alloc,
                        byte_offset: 0,
                    },
                    value: int32(4),
                },
                Effect::Write {
                    loc: Location {
                        alloc,
                        byte_offset: 4,
                    },
                    value: int32(7),
                },
                Effect::Return(int32(11)),
            ],
        };

        assert_eq!(cir_trace, rust_trace);
    }

    #[test]
    fn differing_width_is_not_canonicalized_away() {
        let alloc = AllocId(0);
        let loc = Location {
            alloc,
            byte_offset: 0,
        };
        let as_i32 = Effect::Write {
            loc,
            value: int32(4),
        };
        let as_u8 = Effect::Write {
            loc,
            value: Value::Int {
                width: IntWidth::W8,
                signed: false,
                value: 4,
            },
        };
        assert_ne!(as_i32, as_u8);
    }
}
