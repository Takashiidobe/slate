mod interp;
mod support;

use crate::rust_ast::{FnDef, Program};

use super::{EffectTrace, ParamSeed};

pub fn interpret(f: &FnDef) -> EffectTrace {
    interp::interpret(f)
}

pub fn interpret_with_params(f: &FnDef, params: &[(&str, ParamSeed)]) -> EffectTrace {
    interp::interpret_with_params(f, params)
}

pub fn interpret_program_main(program: &Program) -> EffectTrace {
    interp::interpret_program_main(program)
}
