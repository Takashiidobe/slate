#![feature(register_tool)]
#![register_tool(slate)]
#![deny(
    clippy::unwrap_used,
    clippy::expect_used,
    clippy::panic,
    clippy::unreachable,
    clippy::todo,
    clippy::unimplemented
)]

mod byte_buffers;
mod c_string_arrays;
mod constant_strings;
mod counted_loops;
mod def_use;
mod effects;
mod facts;
mod guarded_heap;
mod identity;
mod pointer;
mod program;
pub mod transform;

pub use byte_buffers::ArrayAlias;
pub use constant_strings::ConstantString;
pub use counted_loops::CountedLoop;
pub use def_use::{BindingMutability, BorrowArgument};
pub use facts::{BindingFacts, Facts, UnitFacts, ValueFacts, analyze};
pub use guarded_heap::{GuardedHeapStatement, HeapElement, HeapIndex};
pub use identity::{FunctionIdentity, Known};
pub use pointer::{Borrow, HeapAction, Length, Owner, PointerFact, PointerRole};
pub use program::{FunctionEntry, FunctionId, Program, ProgramBinding, ProgramUnit, UnitId};
