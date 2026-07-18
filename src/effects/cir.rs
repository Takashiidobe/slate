mod interp;
mod support;

use crate::cir::ir::{Module, Op};

use super::{EffectTrace, ParamSeed};

pub fn interpret(ops: &[Op]) -> EffectTrace {
    interp::interpret(ops)
}

pub fn interpret_with_params(ops: &[Op], params: &[(&str, ParamSeed)]) -> EffectTrace {
    interp::interpret_with_params(ops, params)
}

pub fn interpret_module_main(module: &Module) -> EffectTrace {
    interp::interpret_module_main(module)
}
