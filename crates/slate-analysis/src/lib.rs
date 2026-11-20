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

mod facts;
mod identity;
mod pointer;
mod program;

pub use facts::{BindingFacts, Facts, UnitFacts, ValueFacts, analyze};
pub use identity::{FunctionIdentity, Known};
pub use pointer::{PointerCapability, PointerFact, PointerRole};
pub use program::{FunctionEntry, FunctionId, Program, ProgramBinding, ProgramUnit, UnitId};
