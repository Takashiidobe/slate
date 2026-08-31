use std::collections::BTreeMap;
use std::path::Path;

use crate::frontend::toolchain::{
    EmitError, Tool, ToolOperation, emit_generic_with_args, emit_generic_with_args_cfg_flattened,
    emit_generic_with_args_flattened,
};
use clang_ir::Error as ParseError;
use clang_ir::ast::{Module as GenericModule, Operation as Op};
use clang_ir::model::Module;
use thiserror::Error;

#[derive(Debug, Error)]
pub enum ModuleError {
    #[error(transparent)]
    Emit(#[from] EmitError),
    #[error(transparent)]
    Parse(#[from] ParseError),
    #[error(transparent)]
    Model(#[from] clang_ir::model::ModelError),
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

fn contains_indirect_goto(op: &Op) -> bool {
    op.mnemonic() == "indirect_goto"
        || op.regions.iter().any(|region| {
            region
                .blocks
                .iter()
                .any(|block| block.ops.iter().any(contains_indirect_goto))
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

fn contains_asm_goto(op: &Op) -> bool {
    (op.mnemonic() == "asm"
        && op
            .attr("asm_string")
            .and_then(|value| value.as_str())
            .is_some_and(|template| template.contains(":l}")))
        || op.regions.iter().any(|region| {
            region
                .blocks
                .iter()
                .any(|block| block.ops.iter().any(contains_asm_goto))
        })
}

fn needs_goto_flattening(op: &Op) -> bool {
    op.mnemonic() == "func" && contains_goto(op)
}

fn needs_indirect_goto_flattening(op: &Op) -> bool {
    op.mnemonic() == "func" && contains_indirect_goto(op)
}

fn needs_asm_goto_flattening(op: &Op) -> bool {
    op.mnemonic() == "func"
        && op
            .regions
            .first()
            .is_some_and(|body| body.blocks.len() <= 1)
        && !contains_goto(op)
        && contains_nested_label(op)
        && contains_asm_goto(op)
}

fn module_op(module: &GenericModule) -> Option<&Op> {
    module.ops.iter().find(|op| op.name == "builtin.module")
}

fn module_needs_flattening(module: &GenericModule, needs_flattening: fn(&Op) -> bool) -> bool {
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

fn merge_flattened_functions(
    module: &mut GenericModule,
    flat_module: &GenericModule,
    needs_flattening: fn(&Op) -> bool,
) {
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
/// except that structured functions requiring dispatch are replaced with a
/// flattened form that `lower_dispatch` can turn into a state machine. Raw
/// `cir.goto` functions use `--cir-flatten-cfg --cir-goto-solver`; nested asm
/// goto functions use `--cir-flatten-cfg` alone so their label identities
/// survive. Other functions keep their structured form.
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
            )?)?);
        }
        Err(error) => return Err(error.into()),
    };
    let mut module = parse_module(&generic)?;
    if module_needs_flattening(&module, needs_indirect_goto_flattening) {
        let flat_module = parse_module(&emit_generic_with_args_flattened(src, extra_args)?)?;
        merge_flattened_functions(&mut module, &flat_module, needs_indirect_goto_flattening);
    }
    if module_needs_flattening(&module, needs_asm_goto_flattening) {
        let flat_module = parse_module(&emit_generic_with_args_cfg_flattened(src, extra_args)?)?;
        merge_flattened_functions(&mut module, &flat_module, needs_asm_goto_flattening);
    }
    if module_needs_flattening(&module, needs_goto_flattening) {
        let flat_module = parse_module(&emit_generic_with_args_flattened(src, extra_args)?)?;
        merge_flattened_functions(&mut module, &flat_module, needs_goto_flattening);
    }
    Ok(Module::from_generic(module)?)
}
