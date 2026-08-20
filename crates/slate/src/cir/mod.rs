pub mod emit;
pub mod flatten;

pub use clang_ir::ast::{
    Attribute as Attr, Block, Module as GenericModule, Operation as Op, Region, Type as CirType,
};
pub use clang_ir::model::Module;

pub use emit::{EmitError, TargetError, Tool, ToolOperation, emit_generic, emit_generic_with_args};
pub use flatten::{ModuleError, emit_module};
