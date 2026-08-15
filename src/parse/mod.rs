pub mod ast;
pub mod error;
pub mod lexer;
pub mod parser;
pub mod preprocessor;

use ast::Program;
use error::CompileResult;
use std::path::Path;

pub fn parse_file(path: &Path) -> CompileResult<Program> {
    let tokens = lexer::tokenize_file(path)?;
    let tokens = preprocessor::preprocess(tokens, Vec::new(), Vec::new())?;
    parser::parse(&tokens)
}
