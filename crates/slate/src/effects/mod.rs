//! Shared effect vocabulary for Rust-to-Rust equivalence checking.
//!
//! The Rust AST interpreter turns raw lowered Rust and fixuped Rust into
//! [`EffectTrace`]s. A comparator then decides whether the traces are
//! equivalent. This module defines the trace and effect types both extraction
//! and comparison use, so the effect vocabulary stays explicit.
//!
//! Two design decisions make traces from two structurally different programs
//! (a `*const i32` walk vs. its idiomatized `Vec<i32>`/`Box<[i32]>` form)
//! comparable at all:
//!
//! - **Locations never carry a real address.** [`AllocId`] numbers
//!   allocations by the order the Rust AST interpreter saw them get created,
//!   not by pointer value. Comparison later compacts allocation ids so raw and
//!   fixuped Rust can differ by dead or unobserved allocation details while
//!   still naming the same live logical buffers consistently.
//! - **Only effectful operations are ever pushed.** There is no "silent tick"
//!   variant in [`Effect`] — a walker simply does not emit anything while
//!   evaluating an internal expression, only when it allocates, reads,
//!   writes, crosses a stdlib/std call boundary, returns, or exits. That
//!   makes the stuttering-equivalence reduction discussed for this project
//!   implicit in the vocabulary: comparing two traces is comparing two
//!   already-reduced observable-event sequences, not filtering silent steps
//!   out of a larger one.

use crate::rust_ast::{AtomicOrdering, AtomicRmwOp};

pub mod interpreter;
pub mod rust_ast;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) enum CallSummary {
    Malloc,
    Free,
    Memcpy,
    Memmove,
    Memset,
    Memchr,
    Strlen,
    Printf,
    Fopen,
    Fputs,
    Fclose,
    Qsort,
    Bsearch,
}

pub(crate) fn call_summary(name: &str) -> Option<CallSummary> {
    match name {
        "malloc" => Some(CallSummary::Malloc),
        "free" => Some(CallSummary::Free),
        "memcpy" => Some(CallSummary::Memcpy),
        "memmove" => Some(CallSummary::Memmove),
        "memset" => Some(CallSummary::Memset),
        "memchr" => Some(CallSummary::Memchr),
        "strlen" => Some(CallSummary::Strlen),
        "printf" => Some(CallSummary::Printf),
        "fopen" => Some(CallSummary::Fopen),
        "fputs" => Some(CallSummary::Fputs),
        "fclose" => Some(CallSummary::Fclose),
        "qsort" => Some(CallSummary::Qsort),
        "bsearch" => Some(CallSummary::Bsearch),
        _ => None,
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct AllocId(pub u32);

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct FileId(pub u32);

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct AtomicId(pub u32);

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct Location {
    pub alloc: AllocId,
    pub byte_offset: u64,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum IntWidth {
    W8,
    W16,
    W32,
    W64,
    W128,
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
    Ref(Location),
    File(FileId),
    Atomic(AtomicId),
    AtomicResult {
        ok: bool,
        value: OptionValue,
    },
    BlockLabel(&'static str),
    Null,
    Option(Option<OptionValue>),
}

#[derive(Debug, Clone, Copy, PartialEq)]
pub enum OptionValue {
    Int {
        width: IntWidth,
        signed: bool,
        value: i128,
    },
    Bool(bool),
    Ref(Location),
}

#[derive(Debug, Clone, PartialEq)]
pub enum ParamSeed {
    Scalar(Value),
    Buffer(Vec<Value>),
}

#[derive(Debug, Clone, PartialEq)]
pub enum Effect {
    Alloc {
        alloc: AllocId,
        size: u64,
    },
    Dealloc {
        alloc: AllocId,
    },
    FileOpen {
        file: FileId,
        path: String,
        mode: String,
    },
    FileWrite {
        file: FileId,
        bytes: Vec<u8>,
    },
    FileClose {
        file: FileId,
    },
    AtomicLoad {
        atomic: AtomicId,
        ordering: AtomicOrdering,
        value: Value,
    },
    AtomicStore {
        atomic: AtomicId,
        ordering: AtomicOrdering,
        value: Value,
    },
    AtomicRmw {
        atomic: AtomicId,
        op: AtomicRmwOp,
        ordering: AtomicOrdering,
        operand: Value,
        old: Value,
        new: Value,
    },
    AtomicSwap {
        atomic: AtomicId,
        ordering: AtomicOrdering,
        old: Value,
        new: Value,
    },
    AtomicCompareExchange {
        atomic: AtomicId,
        success: AtomicOrdering,
        failure: AtomicOrdering,
        expected: Value,
        desired: Value,
        old: Value,
        exchanged: bool,
    },
    AtomicFence {
        ordering: AtomicOrdering,
    },
    Write {
        loc: Location,
        value: Value,
    },
    Read {
        loc: Location,
        value: Value,
    },
    Call {
        name: String,
        args: Vec<Value>,
    },
    Return(Value),
    Exit(i32),
}

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

    #[test]
    fn same_allocation_order_makes_traces_comparable_across_shapes() {
        let alloc = AllocId(0);

        let raw_trace = EffectTrace {
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
                Effect::Exit(11),
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
                Effect::Exit(11),
            ],
        };

        assert_eq!(raw_trace, rust_trace);
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

    #[test]
    fn option_values_are_distinct_from_raw_nulls() {
        let none = Value::Option(None);
        assert_ne!(none, Value::Null);

        let some_index = Value::Option(Some(OptionValue::Int {
            width: IntWidth::PointerSized,
            signed: false,
            value: 2,
        }));
        assert_eq!(some_index, some_index);
    }

    #[test]
    fn external_call_summaries_are_shared_by_frontends() {
        assert_eq!(call_summary("malloc"), Some(CallSummary::Malloc));
        assert_eq!(call_summary("printf"), Some(CallSummary::Printf));
        assert_eq!(call_summary("qsort"), Some(CallSummary::Qsort));
        assert_eq!(call_summary("bsearch"), Some(CallSummary::Bsearch));
        assert_eq!(call_summary("user_function"), None);
    }
}
