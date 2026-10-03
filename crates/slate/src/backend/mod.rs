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

pub fn structure_control_flow(mut program: Program) -> Program {
    engine::apply_control_flow(&mut program);
    program
}

pub fn apply_with_target(
    program: Program,
    target: &slate_parser::target_info::TargetInfo,
) -> Program {
    let mut program = program;
    engine::apply(&mut program, target);
    program
}

pub fn place_in_distinct_sections(
    program: &mut Program,
    unit: &str,
    functions: &std::collections::BTreeSet<String>,
) {
    for item in &mut program.items {
        if let rust_ast::Item::Fn(function) = item
            && functions.contains(function.name.as_str())
        {
            function.attrs.push(rust_ast::Attr::LinkSection(format!(
                ".text.slate_distinct.{unit}.{}",
                function.name
            )));
        }
    }
}

pub fn propagate_unwind_abi_across_project(_programs: &mut [Program]) {}
