pub mod c_ast;
pub mod c_shim;
pub mod cir_input;
pub mod directive_translate;
mod function_abi;
mod lowerer;
pub mod macros;
pub mod preprocess;
/// The [toolchain] module carries functions that turns a C file into ClangIR.
pub mod toolchain;

pub use lowerer::*;
