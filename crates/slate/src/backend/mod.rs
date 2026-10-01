//! Backend processing for generated Rust.
//!
//! The backend rewrites the baseline Rust AST produced by the frontend, then
//! emits formatted Rust source through [`crate::backend::format_rust`].

/// Rust source emission and code-generation helpers.
pub mod codegen;
mod engine;
mod format;
mod interproc;
/// The Rust AST transformed by the backend.
pub mod rust_ast;

use crate::backend::rust_ast::Program;

pub use format::{format_rust, pretty_rust, write_pretty_rust, write_rust};

/// Applies the backend rewrite pipeline to a generated Rust program.
pub fn apply(program: Program) -> Program {
    apply_with_target(program, &crate::target::active_info())
}

pub fn apply_with_target(
    program: Program,
    target: &slate_parser::target_info::TargetInfo,
) -> Program {
    if std::env::var_os("SLATE_RAW_LOWER").is_some()
        || std::env::var("NEXTEST_PROFILE").is_ok_and(|profile| profile == "lowering")
    {
        return program;
    }
    let mut program = program;
    engine::apply(&mut program, target);
    program
}

/// Propagates unwind ABI information across translated project programs.
pub fn propagate_unwind_abi_across_project(_programs: &mut [Program]) {}
