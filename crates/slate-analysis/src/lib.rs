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

mod constant_strings;
mod counted_loops;
mod def_use;
mod effects;
mod facts;
mod identity;
mod pointer;
mod program;
pub mod transform;

pub use counted_loops::CountedLoop;
pub use facts::{BindingFacts, Facts, UnitFacts, ValueFacts, analyze};
pub use identity::{FunctionIdentity, Known};
pub use pointer::{Borrow, Length, PointerFact, PointerRole};
pub use program::{FunctionEntry, FunctionId, Program, ProgramBinding, ProgramUnit, UnitId};
