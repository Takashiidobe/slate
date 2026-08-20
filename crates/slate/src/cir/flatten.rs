use std::collections::BTreeMap;
use std::path::Path;

use super::emit::{
    EmitError, Tool, ToolOperation, emit_generic_with_args, emit_generic_with_args_flattened,
};
use super::{GenericModule, Module, Op};
use clang_ir::Error as ParseError;
use thiserror::Error;

#[derive(Debug, Error)]
pub enum ModuleError {
    #[error(transparent)]
    Emit(#[from] EmitError),
    #[error(transparent)]
    Parse(#[from] ParseError),
}

fn parse_module(text: &str) -> Result<GenericModule, ParseError> {
    clang_ir::parse_generic_str(text)
}

fn sym_name(op: &Op) -> Option<&str> {
    op.attr("sym_name")?.as_str()
}

fn contains_goto(op: &Op) -> bool {
    op.regions.iter().any(|region| {
        region.blocks.iter().any(|block| {
            block
                .ops
                .iter()
                .any(|child| child.mnemonic() == "goto" || contains_goto(child))
        })
    })
}

fn contains_label(op: &Op) -> bool {
    op.mnemonic() == "label"
        || op.regions.iter().any(|region| {
            region
                .blocks
                .iter()
                .any(|block| block.ops.iter().any(contains_label))
        })
}

fn contains_nested_label(op: &Op) -> bool {
    op.regions.first().is_some_and(|body| {
        body.blocks.iter().any(|block| {
            block.ops.iter().any(|child| {
                child.regions.iter().any(|region| {
                    region
                        .blocks
                        .iter()
                        .any(|block| block.ops.iter().any(contains_label))
                })
            })
        })
    })
}

fn needs_flattening(op: &Op) -> bool {
    op.mnemonic() == "func"
        && op
            .regions
            .first()
            .is_some_and(|body| body.blocks.len() <= 1 || contains_nested_label(op))
        && contains_goto(op)
}

fn module_op(module: &GenericModule) -> Option<&Op> {
    module.ops.iter().find(|op| op.name == "builtin.module")
}

fn module_needs_flattening(module: &GenericModule) -> bool {
    let Some(module_op) = module_op(module) else {
        return false;
    };
    module_op
        .regions
        .iter()
        .flat_map(|region| region.blocks.iter())
        .flat_map(|block| block.ops.iter())
        .any(needs_flattening)
}

fn merge_flattened_functions(module: &mut GenericModule, flat_module: &GenericModule) {
    let Some(flat_module_op) = module_op(flat_module) else {
        return;
    };
    let flat_funcs: BTreeMap<String, Op> = flat_module_op
        .regions
        .iter()
        .flat_map(|region| region.blocks.iter())
        .flat_map(|block| block.ops.iter())
        .filter(|op| op.mnemonic() == "func")
        .filter_map(|op| Some((sym_name(op)?.to_string(), op.clone())))
        .collect();
    let Some(module_op) = module.ops.iter_mut().find(|op| op.name == "builtin.module") else {
        return;
    };
    for region in &mut module_op.regions {
        for block in &mut region.blocks {
            for op in &mut block.ops {
                if needs_flattening(op)
                    && let Some(name) = sym_name(op)
                    && let Some(flat_func) = flat_funcs.get(name)
                {
                    *op = flat_func.clone();
                }
            }
        }
    }
}

/// Parses `src` into a `Module` exactly like `parse_module(&emit_generic_with_args(..))`,
/// except that any function which is still structured (a single top-level
/// block) but contains a raw `cir.goto` — a shape structured lowering can't
/// bridge on its own (see slate-sy7c/slate-fex8) — is replaced with its
/// fully flattened form (`cir-opt --cir-flatten-cfg --cir-goto-solver`),
/// which `lower_dispatch` knows how to turn into a correct (if less
/// idiomatic) state machine. Functions that don't contain a goto, and
/// functions CIR already emits as multi-block on their own (computed goto,
/// asm goto, top-level-labeled functions), are left completely untouched —
/// the extra cir-opt invocation only happens at all when the first parse
/// finds at least one goto-bearing structured function.
pub fn emit_module(src: &Path, extra_args: &[String]) -> Result<Module, ModuleError> {
    let generic = match emit_generic_with_args(src, extra_args) {
        Ok(generic) => generic,
        Err(EmitError::ToolFailed {
            tool: Tool::Clang,
            operation: ToolOperation::EmitCir,
            stderr,
            ..
        }) if stderr.contains("does not dominate this use") => {
            return Ok(Module::from_generic(parse_module(
                &emit_generic_with_args_flattened(src, extra_args)?,
            )?));
        }
        Err(error) => return Err(error.into()),
    };
    let mut module = parse_module(&generic)?;
    if module_needs_flattening(&module) {
        let flat_module = parse_module(&emit_generic_with_args_flattened(src, extra_args)?)?;
        merge_flattened_functions(&mut module, &flat_module);
    }
    Ok(Module::from_generic(module))
}
