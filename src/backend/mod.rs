pub mod codegen;
mod engine;
mod interproc;
pub mod rust_ast;
pub mod trace;

use crate::backend::rust_ast::Program;

pub use trace::Pass;

#[derive(Debug, Clone, Default)]
pub struct SkipSet(std::collections::HashSet<Pass>);

impl SkipSet {
    pub fn none() -> Self {
        Self::default()
    }

    pub fn skip(pass: Pass) -> Self {
        let mut set = Self::default();
        set.0.insert(pass);
        set
    }
}

pub fn apply(program: Program) -> Program {
    apply_with(program, &SkipSet::none())
}

pub fn apply_with(program: Program, _skip: &SkipSet) -> Program {
    if std::env::var_os("SLATE_RAW_LOWER").is_some()
        || std::env::var("NEXTEST_PROFILE").is_ok_and(|profile| profile == "lowering")
    {
        return program;
    }
    let mut program = program;
    engine::apply(&mut program);
    program
}

pub fn debug_with(program: Program, _options: DebugOptions) -> String {
    apply(program).emit()
}

#[derive(Debug, Clone, Copy, Default)]
pub struct DebugOptions {
    pub up_to_pass: Option<Pass>,
    pub only_pass: Option<Pass>,
    pub debug_only_pass: Option<Pass>,
}

pub fn propagate_unwind_abi_across_project(_programs: &mut [Program]) {}

pub fn valid_pass_names() -> String {
    Pass::ALL
        .iter()
        .map(|pass| pass.name())
        .collect::<Vec<_>>()
        .join(", ")
}
