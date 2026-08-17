pub mod emit;
pub mod flatten;
pub mod ir;

pub use emit::{EmitError, TargetError, Tool, ToolOperation, emit_generic, emit_generic_with_args};
pub use flatten::{ModuleError, emit_module};
