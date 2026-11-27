mod borrow;
mod c_string;
mod emit;
mod flow;
mod length;
mod ownership;
mod reasons;
mod summary;

pub use summary::fixed_array_position;

use crate::counted_loops::CountedLoop;
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

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash, PartialOrd, Ord)]
enum Reason {
    StoreUntracked,
    StoreField,
    StoreMemory,
    StoreElement,
    CompoundLiteral,
    AddressOfPointer,
    Atomic,
    Aggregate,
    BlockCapture,
    NestedCapture,
    PointerToInteger,
    Intrinsic,
    OpenMp,
    AsmOperand,
    CleanupOrByref,
    InitializeUntracked,
    ReturnUntracked,
    OpaqueCall,
    LibraryCall,
    NonCandidateCallee,
    NonCandidateReturn,
    AliasedWrittenArguments,
    Unlabeled,
    NonCandidateParameter,
    ReturnMoved,
    ReturnTemporary,
    ReturnOpaque,
    ReturnRetyped,
    Moved,
    TemporarySource,
    OpaqueValue,
    Retyped,
    Uninitialized,
    Rebased,
    AsmOutput,
    HeapWrite,
    CalleeNoReturn,
    ReturnNoLifetime,
    ReturnLifetimeRejected,
    CalleeReturnUnlifted,
    NullableProjection,
    MacroArgument,
    NoBorrow,
    LeakedAutomatic,
    RawReturn,
    MacroCallSite,
    UnprovenArgument,
    PartOffset,
    RawArgument,
}

impl Reason {
    fn name(self) -> &'static str {
        match self {
            Reason::StoreUntracked => "escape.store_untracked_binding",
            Reason::StoreField => "escape.store_field",
            Reason::StoreMemory => "escape.store_memory",
            Reason::StoreElement => "escape.store_other_place",
            Reason::CompoundLiteral => "escape.compound_literal",
            Reason::AddressOfPointer => "escape.address_of_pointer",
            Reason::Atomic => "escape.atomic",
            Reason::Aggregate => "escape.aggregate",
            Reason::BlockCapture => "escape.block_capture",
            Reason::NestedCapture => "escape.nested_capture",
            Reason::PointerToInteger => "escape.pointer_to_integer",
            Reason::Intrinsic => "escape.intrinsic",
            Reason::OpenMp => "escape.openmp",
            Reason::AsmOperand => "escape.asm_operand",
            Reason::CleanupOrByref => "escape.cleanup_or_byref",
            Reason::InitializeUntracked => "escape.initialize_untracked",
            Reason::ReturnUntracked => "escape.return_untracked",
            Reason::OpaqueCall => "escape.opaque_call",
            Reason::LibraryCall => "escape.library_call",
            Reason::NonCandidateCallee => "escape.non_candidate_callee",
            Reason::NonCandidateReturn => "escape.non_candidate_return",
            Reason::AliasedWrittenArguments => "escape.aliased_written_arguments",
            Reason::Unlabeled => "escape.unlabeled",
            Reason::NonCandidateParameter => "raw.non_candidate_parameter",
            Reason::ReturnMoved => "raw.return_moved",
            Reason::ReturnTemporary => "raw.return_temporary",
            Reason::ReturnOpaque => "raw.return_opaque",
            Reason::ReturnRetyped => "raw.return_retyped",
            Reason::Moved => "raw.moved",
            Reason::TemporarySource => "raw.temporary_source",
            Reason::OpaqueValue => "raw.opaque_value",
            Reason::Retyped => "raw.retyped",
            Reason::Uninitialized => "raw.uninitialized",
            Reason::Rebased => "raw.rebased",
            Reason::AsmOutput => "raw.asm_output",
            Reason::HeapWrite => "raw.heap_write",
            Reason::CalleeNoReturn => "raw.callee_no_return",
            Reason::ReturnNoLifetime => "raw.return_no_parameter_lifetime",
            Reason::ReturnLifetimeRejected => "raw.return_lifetime_rejected",
            Reason::CalleeReturnUnlifted => "raw.callee_return_unlifted",
            Reason::NullableProjection => "raw.nullable_projection",
            Reason::MacroArgument => "unproven.macro_argument",
            Reason::NoBorrow => "unproven.no_borrow",
            Reason::LeakedAutomatic => "unproven.leaked_automatic",
            Reason::RawReturn => "unproven.raw_return",
            Reason::MacroCallSite => "unproven.macro_call_site",
            Reason::UnprovenArgument => "unproven.argument",
            Reason::PartOffset => "unproven.part_offset",
            Reason::RawArgument => "unproven.raw_argument",
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash, PartialOrd, Ord)]
struct Origin {
    reason: Reason,
    propagated: bool,
}

impl Origin {
    fn local(reason: Reason) -> Self {
        Origin {
            reason,
            propagated: false,
        }
    }
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
    counted_loops: &[HashMap<BindingId, CountedLoop>],
) -> PointerFacts {
    let timing = std::env::var_os("SLATE_INTERPROC_TIMING")
        .is_some()
        .then(std::time::Instant::now);
    let inputs = summary::Inputs::new(program, identities);
    let functions = summary::collect(&inputs);
    let summarized = timing.map(|start| (start.elapsed(), std::time::Instant::now()));

    let (mut summaries, leaked_statics) = summary::Summaries::new(program, functions);
    let reasons = std::env::var_os("SLATE_LIFT_REASONS")
        .map(|mode| (mode == "sites", leaked_statics.clone()));
    let flow = flow::solve(program, &summaries, leaked_statics);
    let owned = ownership::prove(program, &summaries, &flow);
    let (lengths, unproven) = length::prove(
        program,
        &summaries,
        &flow,
        &owned,
        &inputs.mutable_statics,
        counted_loops,
    );
    let c_strings = c_string::parameters(&summaries, &flow);
    let facts = emit::facts(
        program, &summaries, &flow, owned, &lengths, &unproven, &c_strings,
    );
    if let Some((sites, leaked_statics)) = reasons {
        reasons::report(program, &summaries, &flow, &unproven, &facts, sites);
        if summaries.assume_external_callers(program) {
            let flow = flow::solve(program, &summaries, leaked_statics);
            let owned = ownership::prove(program, &summaries, &flow);
            let (lengths, unproven) = length::prove(
                program,
                &summaries,
                &flow,
                &owned,
                &inputs.mutable_statics,
                counted_loops,
            );
            let c_strings = c_string::parameters(&summaries, &flow);
            let shimmed = emit::facts(
                program, &summaries, &flow, owned, &lengths, &unproven, &c_strings,
            );
            reasons::report_shims(
                program,
                &summaries,
                &flow,
                &unproven,
                &facts,
                &shimmed,
                counted_loops,
                sites,
            );
        }
    }

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
