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
mod heap_ir;
mod identity;
mod pointer;
mod printf;
mod program;
pub mod transform;

pub use byte_buffers::ArrayAlias;
pub use constant_strings::ConstantString;
pub use counted_loops::CountedLoop;
pub use counted_loops::integer_constant;
pub use def_use::single_read_initializers;
pub use def_use::{BindingMutability, BorrowArgument};
pub use facts::{BindingFacts, Facts, UnitFacts, ValueFacts, analyze};
pub use guarded_heap::{GuardedHeapStatement, HeapElement, HeapIndex};
pub use identity::{FunctionIdentity, Known};
pub use pointer::{
    Borrow, HeapAction, Length, Owner, PointerFact, PointerRole, fixed_array_position,
};
pub fn nonnegative_integer_constant(value: &slate_parser::ir::Value) -> Option<u64> {
    u64::try_from(counted_loops::integer_constant(value)?).ok()
}
pub use printf::PrintfFormat;
pub use program::{FunctionEntry, FunctionId, Program, ProgramBinding, ProgramUnit, UnitId};
