mod borrow;
mod c_string;
mod emit;
mod flow;
mod length;
mod ownership;
mod summary;

use crate::identity::FunctionIdentity;
use crate::program::{Program, ProgramBinding, UnitId};
use foldhash::{HashMap, HashSet};
use slate_parser::ast::NodeId;
use slate_parser::ir::BindingId;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
struct PointerCapability {
    write: bool,
    unique: bool,
    free: bool,
    offset: bool,
    escape: bool,
    nullable: bool,
    string: bool,
    raw: bool,
    byte_elements: bool,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum PointerRole {
    Parameter,
    Local,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct Borrow {
    pub mutable: bool,
    pub many: bool,
    pub nullable: bool,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Owner {
    Vec,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct PointerFact {
    pub role: PointerRole,
    pub borrow: Option<Borrow>,
    pub length: Option<Length>,
    pub c_string: bool,
    pub owner: Option<Owner>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct Length {
    pub parameter: usize,
    pub read: bool,
}

impl PointerCapability {
    fn borrow(&self) -> Option<Borrow> {
        let shared_write = self.write && !self.unique;
        (!self.raw && !self.escape && !self.free && !self.string && !shared_write).then_some(
            Borrow {
                mutable: self.write,
                many: self.offset,
                nullable: self.nullable,
            },
        )
    }
}

pub(crate) struct PointerFacts {
    pub bindings: HashMap<ProgramBinding, PointerFact>,
    pub returns: HashMap<ProgramBinding, Borrow>,
    pub elided_releases: HashSet<(UnitId, NodeId)>,
    pub heap_statements: HashMap<(UnitId, NodeId), HeapStatement>,
}

#[derive(Debug, Clone, Copy)]
pub struct HeapStatement {
    pub owner: BindingId,
    pub action: HeapAction,
}

#[derive(Debug, Clone, Copy)]
pub enum HeapAction {
    TryAllocate,
    AllocationFailed,
    TryResize,
    ResizeFailed(BindingId),
    RemoveWrite,
    ClearedCheck(bool),
}

const WRITE: u8 = 1;
const OFFSET: u8 = 1 << 1;
const FREE: u8 = 1 << 2;
const ESCAPE: u8 = 1 << 3;
const NULLABLE: u8 = 1 << 4;
const STRING: u8 = 1 << 5;
const SHARED: u8 = 1 << 6;
const MACRO: u8 = 1 << 7;

pub(crate) fn analyze(
    program: &Program<'_>,
    identities: &HashMap<ProgramBinding, FunctionIdentity>,
) -> PointerFacts {
    let timing = std::env::var_os("SLATE_INTERPROC_TIMING")
        .is_some()
        .then(std::time::Instant::now);
    let inputs = summary::Inputs::new(program, identities);
    let functions = summary::collect(&inputs);
    let summarized = timing.map(|start| (start.elapsed(), std::time::Instant::now()));

    let (summaries, leaked_statics) = summary::Summaries::new(program, functions);
    let flow = flow::solve(program, &summaries, leaked_statics);
    let owned = ownership::prove(program, &summaries, &flow);
    let (lengths, unproven) =
        length::prove(program, &summaries, &flow, &owned, &inputs.mutable_statics);
    let c_strings = c_string::parameters(&summaries, &flow);
    let facts = emit::facts(
        program, &summaries, &flow, owned, &lengths, &unproven, &c_strings,
    );

    if let Some((summarize, joined)) = summarized {
        for (phase, elapsed) in [("summarize", summarize), ("join", joined.elapsed())] {
            eprintln!(
                "interproc-timing pass=ir_pointer phase={phase} ms={:.3}",
                elapsed.as_secs_f64() * 1e3
            );
        }
        eprintln!(
            "interproc-count pass=ir_pointer name=nodes value={}",
            flow.node_count()
        );
    }
    facts
}
