//! The parsed CIR, kept as a generic MLIR Op-tree rather than a typed enum. The
//! generic form `%res = "name"(%operands) <{attrs}> ({regions}) : (types) -> types`
//! is completely op-agnostic, so this model stays stable as op coverage grows;
//! lowering interprets `op.name` and reads attrs/operands through typed views.

use std::collections::BTreeMap;

/// A whole translation unit: the top-level ops (a `builtin.module` and its body).
#[derive(Debug, Default, Clone)]
pub struct Module {
    pub ops: Vec<Op>,
    pub aliases: BTreeMap<String, String>,
}

/// One MLIR operation in generic form.
#[derive(Debug, Clone)]
pub struct Op {
    /// SSA results this op defines, e.g. `%3`, `%c` (names without the `%`).
    pub results: Vec<String>,
    /// Dialect-qualified op name, e.g. `cir.const`, `cir.binop`, `cir.func`.
    pub name: String,
    /// SSA operands referenced (names without the `%`).
    pub operands: Vec<String>,
    /// `<{...}>` inherent attributes plus `{...}` discardable attributes, merged.
    pub attrs: BTreeMap<String, Attr>,
    /// Nested regions `({ ... })`.
    pub regions: Vec<Region>,
    /// Functional type signature `: (a, b) -> r`, kept as raw text for V0.
    pub ty: Option<String>,
    /// Trailing `loc(...)`, kept verbatim for the source-location join.
    pub loc: Option<String>,
}

/// A region is an ordered list of blocks.
#[derive(Debug, Default, Clone)]
pub struct Region {
    pub blocks: Vec<Block>,
}

/// A block: an optional label, block arguments, and its ops.
#[derive(Debug, Default, Clone)]
pub struct Block {
    /// Block label without the `^`, e.g. `bb1`. The entry block is often unlabeled.
    pub label: Option<String>,
    /// Block arguments as `(name, type-text)` pairs.
    pub args: Vec<(String, String)>,
    pub ops: Vec<Op>,
}

/// Attribute values. V0 keeps most as raw text and only distinguishes the shapes
/// lowering actually needs to branch on.
#[derive(Debug, Clone)]
pub enum Attr {
    /// Anything not yet given a richer shape — stored as it was printed.
    Raw(String),
    Int(i64),
    Str(String),
    Type(String),
    Array(Vec<Attr>),
    Dict(BTreeMap<String, Attr>),
}

impl Attr {
    pub fn as_int(&self) -> Option<i64> {
        match self {
            Attr::Int(n) => Some(*n),
            _ => None,
        }
    }

    pub fn as_str(&self) -> Option<&str> {
        match self {
            Attr::Str(s) | Attr::Raw(s) | Attr::Type(s) => Some(s),
            _ => None,
        }
    }
}
