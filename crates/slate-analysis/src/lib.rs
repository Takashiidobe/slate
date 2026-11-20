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
mod program;

pub use facts::{BindingFacts, Facts, UnitFacts, ValueFacts, analyze};
pub use program::{FunctionEntry, FunctionId, Program, ProgramBinding, UnitId};
