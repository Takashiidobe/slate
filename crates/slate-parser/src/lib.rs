#![feature(register_tool)]
#![register_tool(slate)]
#![deny(
    clippy::unwrap_used,
    clippy::expect_used,
    clippy::panic,
    clippy::unreachable,
    clippy::todo,
    clippy::unimplemented,
    clippy::disallowed_macros
)]

pub mod ast;
pub mod attribute_support;
pub mod charset;
mod code_pages;
pub mod compiler_args;
pub mod compiler_headers;
pub mod compiler_options;
pub mod const_expr;
pub mod diagnostics;
pub mod dialect;
pub mod error;
pub mod files;
mod gnu_charsets;
pub mod ir;
pub mod lexer;
pub mod native_headers;
pub mod parser;
pub mod pp;
pub mod reachability;
pub mod render;
pub mod rules;
pub mod sanitizers;
pub mod sema;
pub mod standard_features;
pub mod sysroot;
pub mod target;
pub mod target_info;
pub mod target_registry;
pub mod visit;
