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
    /// Successor block labels from a `[^bbN, ...]` list (names without the `^`).
    pub successors: Vec<String>,
    /// `<{...}>` inherent attributes plus `{...}` discardable attributes, merged.
    pub attrs: BTreeMap<String, Attr>,
    /// Nested regions `({ ... })`.
    pub regions: Vec<Region>,
    /// Functional type signature `: (a, b) -> r`, kept as raw text for V0.
    pub ty: Option<String>,
    /// Trailing `loc(...)`, kept verbatim for the source-location join.
    pub loc: Option<String>,
}

impl Op {
    pub fn kind(&self) -> CirOpKind {
        CirOpKind::parse(&self.name)
    }
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

/// A classification of `Op::name` for typed dispatch, computed on demand
/// from the raw string CIR text actually printed (kept as-is on `Op` for
/// diagnostics/round-tripping). `Other` covers op names this enum doesn't
/// (yet) enumerate — CIR gains ops across Clang versions, so this can't be
/// a closed world.
#[derive(Debug, Clone, PartialEq, Eq, Hash)]
pub enum CirOpKind {
    Abs,
    Acos,
    Add,
    AddOverflow,
    Alloca,
    And,
    Asin,
    Asm,
    Assume,
    Atan,
    Atan2,
    AtomicCmpxchg,
    AtomicFence,
    AtomicFetch,
    AtomicXchg,
    Bitreverse,
    BlockAddress,
    Br,
    Break,
    ByteSwap,
    Call,
    CallLlvmIntrinsic,
    Case,
    Cast,
    Ceil,
    ClearCache,
    Clrsb,
    Clz,
    Cmp,
    ComplexAdd,
    ComplexConj,
    ComplexCreate,
    ComplexDiv,
    ComplexImag,
    ComplexImagPtr,
    ComplexMul,
    ComplexReal,
    ComplexRealPtr,
    ComplexSub,
    Condition,
    Const,
    Continue,
    Copy,
    Copysign,
    Cos,
    Ctz,
    Dec,
    Div,
    DivOverflow,
    Do,
    EhSetjmp,
    Exp,
    Exp2,
    Expect,
    ExtractMember,
    Fabs,
    Fadd,
    Fdiv,
    Ffs,
    Floor,
    Fmaximum,
    Fmaxnum,
    Fminimum,
    Fminnum,
    Fmod,
    Fmul,
    Fneg,
    For,
    FrameAddress,
    Fsub,
    Func,
    GetBitfield,
    GetElement,
    GetGlobal,
    GetMember,
    Global,
    Goto,
    If,
    Inc,
    IndirectBr,
    InsertMember,
    IsConstant,
    IsFpClass,
    Label,
    LibcMemchr,
    LibcMemcpy,
    LibcMemmove,
    LibcMemset,
    Llrint,
    Llround,
    Load,
    Log,
    Log10,
    Log2,
    Lrint,
    Lround,
    Minus,
    Modf,
    Mul,
    MulOverflow,
    Nearbyint,
    Not,
    Objsize,
    Or,
    Parity,
    Popcount,
    Pow,
    Prefetch,
    PtrDiff,
    PtrStride,
    Rem,
    RemOverflow,
    Return,
    Rint,
    Rotate,
    Round,
    Roundeven,
    Scope,
    Select,
    SetBitfield,
    Shift,
    Signbit,
    Sin,
    Sqrt,
    Stackrestore,
    Stacksave,
    Store,
    Sub,
    SubOverflow,
    Switch,
    Tan,
    Ternary,
    Trap,
    Triple,
    Trunc,
    Unreachable,
    VaArg,
    VaEnd,
    VaStart,
    VecExtract,
    VecInsert,
    VecShuffle,
    While,
    Xor,
    Yield,
    Other(String),
}

impl CirOpKind {
    pub fn parse(name: &str) -> CirOpKind {
        match name {
            "cir.abs" => CirOpKind::Abs,
            "cir.acos" => CirOpKind::Acos,
            "cir.add" => CirOpKind::Add,
            "cir.add.overflow" => CirOpKind::AddOverflow,
            "cir.alloca" => CirOpKind::Alloca,
            "cir.and" => CirOpKind::And,
            "cir.asin" => CirOpKind::Asin,
            "cir.asm" => CirOpKind::Asm,
            "cir.assume" => CirOpKind::Assume,
            "cir.atan" => CirOpKind::Atan,
            "cir.atan2" => CirOpKind::Atan2,
            "cir.atomic.cmpxchg" => CirOpKind::AtomicCmpxchg,
            "cir.atomic.fence" => CirOpKind::AtomicFence,
            "cir.atomic.fetch" => CirOpKind::AtomicFetch,
            "cir.atomic.xchg" => CirOpKind::AtomicXchg,
            "cir.bitreverse" => CirOpKind::Bitreverse,
            "cir.block_address" => CirOpKind::BlockAddress,
            "cir.br" => CirOpKind::Br,
            "cir.break" => CirOpKind::Break,
            "cir.byte_swap" => CirOpKind::ByteSwap,
            "cir.call" => CirOpKind::Call,
            "cir.call_llvm_intrinsic" => CirOpKind::CallLlvmIntrinsic,
            "cir.case" => CirOpKind::Case,
            "cir.cast" => CirOpKind::Cast,
            "cir.ceil" => CirOpKind::Ceil,
            "cir.clear_cache" => CirOpKind::ClearCache,
            "cir.clrsb" => CirOpKind::Clrsb,
            "cir.clz" => CirOpKind::Clz,
            "cir.cmp" => CirOpKind::Cmp,
            "cir.complex.add" => CirOpKind::ComplexAdd,
            "cir.complex.conj" => CirOpKind::ComplexConj,
            "cir.complex.create" => CirOpKind::ComplexCreate,
            "cir.complex.div" => CirOpKind::ComplexDiv,
            "cir.complex.imag" => CirOpKind::ComplexImag,
            "cir.complex.imag_ptr" => CirOpKind::ComplexImagPtr,
            "cir.complex.mul" => CirOpKind::ComplexMul,
            "cir.complex.real" => CirOpKind::ComplexReal,
            "cir.complex.real_ptr" => CirOpKind::ComplexRealPtr,
            "cir.complex.sub" => CirOpKind::ComplexSub,
            "cir.condition" => CirOpKind::Condition,
            "cir.const" => CirOpKind::Const,
            "cir.continue" => CirOpKind::Continue,
            "cir.copy" => CirOpKind::Copy,
            "cir.copysign" => CirOpKind::Copysign,
            "cir.cos" => CirOpKind::Cos,
            "cir.ctz" => CirOpKind::Ctz,
            "cir.dec" => CirOpKind::Dec,
            "cir.div" => CirOpKind::Div,
            "cir.div.overflow" => CirOpKind::DivOverflow,
            "cir.do" => CirOpKind::Do,
            "cir.eh.setjmp" => CirOpKind::EhSetjmp,
            "cir.exp" => CirOpKind::Exp,
            "cir.exp2" => CirOpKind::Exp2,
            "cir.expect" => CirOpKind::Expect,
            "cir.extract_member" => CirOpKind::ExtractMember,
            "cir.fabs" => CirOpKind::Fabs,
            "cir.fadd" => CirOpKind::Fadd,
            "cir.fdiv" => CirOpKind::Fdiv,
            "cir.ffs" => CirOpKind::Ffs,
            "cir.floor" => CirOpKind::Floor,
            "cir.fmaximum" => CirOpKind::Fmaximum,
            "cir.fmaxnum" => CirOpKind::Fmaxnum,
            "cir.fminimum" => CirOpKind::Fminimum,
            "cir.fminnum" => CirOpKind::Fminnum,
            "cir.fmod" => CirOpKind::Fmod,
            "cir.fmul" => CirOpKind::Fmul,
            "cir.fneg" => CirOpKind::Fneg,
            "cir.for" => CirOpKind::For,
            "cir.frame_address" => CirOpKind::FrameAddress,
            "cir.fsub" => CirOpKind::Fsub,
            "cir.func" => CirOpKind::Func,
            "cir.get_bitfield" => CirOpKind::GetBitfield,
            "cir.get_element" => CirOpKind::GetElement,
            "cir.get_global" => CirOpKind::GetGlobal,
            "cir.get_member" => CirOpKind::GetMember,
            "cir.global" => CirOpKind::Global,
            "cir.goto" => CirOpKind::Goto,
            "cir.if" => CirOpKind::If,
            "cir.inc" => CirOpKind::Inc,
            "cir.indirect_br" => CirOpKind::IndirectBr,
            "cir.insert_member" => CirOpKind::InsertMember,
            "cir.is_constant" => CirOpKind::IsConstant,
            "cir.is_fp_class" => CirOpKind::IsFpClass,
            "cir.label" => CirOpKind::Label,
            "cir.libc.memchr" => CirOpKind::LibcMemchr,
            "cir.libc.memcpy" => CirOpKind::LibcMemcpy,
            "cir.libc.memmove" => CirOpKind::LibcMemmove,
            "cir.libc.memset" => CirOpKind::LibcMemset,
            "cir.llrint" => CirOpKind::Llrint,
            "cir.llround" => CirOpKind::Llround,
            "cir.load" => CirOpKind::Load,
            "cir.log" => CirOpKind::Log,
            "cir.log10" => CirOpKind::Log10,
            "cir.log2" => CirOpKind::Log2,
            "cir.lrint" => CirOpKind::Lrint,
            "cir.lround" => CirOpKind::Lround,
            "cir.minus" => CirOpKind::Minus,
            "cir.modf" => CirOpKind::Modf,
            "cir.mul" => CirOpKind::Mul,
            "cir.mul.overflow" => CirOpKind::MulOverflow,
            "cir.nearbyint" => CirOpKind::Nearbyint,
            "cir.not" => CirOpKind::Not,
            "cir.objsize" => CirOpKind::Objsize,
            "cir.or" => CirOpKind::Or,
            "cir.parity" => CirOpKind::Parity,
            "cir.popcount" => CirOpKind::Popcount,
            "cir.pow" => CirOpKind::Pow,
            "cir.prefetch" => CirOpKind::Prefetch,
            "cir.ptr_diff" => CirOpKind::PtrDiff,
            "cir.ptr_stride" => CirOpKind::PtrStride,
            "cir.rem" => CirOpKind::Rem,
            "cir.rem.overflow" => CirOpKind::RemOverflow,
            "cir.return" => CirOpKind::Return,
            "cir.rint" => CirOpKind::Rint,
            "cir.rotate" => CirOpKind::Rotate,
            "cir.round" => CirOpKind::Round,
            "cir.roundeven" => CirOpKind::Roundeven,
            "cir.scope" => CirOpKind::Scope,
            "cir.select" => CirOpKind::Select,
            "cir.set_bitfield" => CirOpKind::SetBitfield,
            "cir.shift" => CirOpKind::Shift,
            "cir.signbit" => CirOpKind::Signbit,
            "cir.sin" => CirOpKind::Sin,
            "cir.sqrt" => CirOpKind::Sqrt,
            "cir.stackrestore" => CirOpKind::Stackrestore,
            "cir.stacksave" => CirOpKind::Stacksave,
            "cir.store" => CirOpKind::Store,
            "cir.sub" => CirOpKind::Sub,
            "cir.sub.overflow" => CirOpKind::SubOverflow,
            "cir.switch" => CirOpKind::Switch,
            "cir.tan" => CirOpKind::Tan,
            "cir.ternary" => CirOpKind::Ternary,
            "cir.trap" => CirOpKind::Trap,
            "cir.triple" => CirOpKind::Triple,
            "cir.trunc" => CirOpKind::Trunc,
            "cir.unreachable" => CirOpKind::Unreachable,
            "cir.va_arg" => CirOpKind::VaArg,
            "cir.va_end" => CirOpKind::VaEnd,
            "cir.va_start" => CirOpKind::VaStart,
            "cir.vec.extract" => CirOpKind::VecExtract,
            "cir.vec.insert" => CirOpKind::VecInsert,
            "cir.vec.shuffle" => CirOpKind::VecShuffle,
            "cir.while" => CirOpKind::While,
            "cir.xor" => CirOpKind::Xor,
            "cir.yield" => CirOpKind::Yield,
            other => CirOpKind::Other(other.to_string()),
        }
    }

    pub fn as_str(&self) -> &str {
        match self {
            CirOpKind::Abs => "cir.abs",
            CirOpKind::Acos => "cir.acos",
            CirOpKind::Add => "cir.add",
            CirOpKind::AddOverflow => "cir.add.overflow",
            CirOpKind::Alloca => "cir.alloca",
            CirOpKind::And => "cir.and",
            CirOpKind::Asin => "cir.asin",
            CirOpKind::Asm => "cir.asm",
            CirOpKind::Assume => "cir.assume",
            CirOpKind::Atan => "cir.atan",
            CirOpKind::Atan2 => "cir.atan2",
            CirOpKind::AtomicCmpxchg => "cir.atomic.cmpxchg",
            CirOpKind::AtomicFence => "cir.atomic.fence",
            CirOpKind::AtomicFetch => "cir.atomic.fetch",
            CirOpKind::AtomicXchg => "cir.atomic.xchg",
            CirOpKind::Bitreverse => "cir.bitreverse",
            CirOpKind::BlockAddress => "cir.block_address",
            CirOpKind::Br => "cir.br",
            CirOpKind::Break => "cir.break",
            CirOpKind::ByteSwap => "cir.byte_swap",
            CirOpKind::Call => "cir.call",
            CirOpKind::CallLlvmIntrinsic => "cir.call_llvm_intrinsic",
            CirOpKind::Case => "cir.case",
            CirOpKind::Cast => "cir.cast",
            CirOpKind::Ceil => "cir.ceil",
            CirOpKind::ClearCache => "cir.clear_cache",
            CirOpKind::Clrsb => "cir.clrsb",
            CirOpKind::Clz => "cir.clz",
            CirOpKind::Cmp => "cir.cmp",
            CirOpKind::ComplexAdd => "cir.complex.add",
            CirOpKind::ComplexConj => "cir.complex.conj",
            CirOpKind::ComplexCreate => "cir.complex.create",
            CirOpKind::ComplexDiv => "cir.complex.div",
            CirOpKind::ComplexImag => "cir.complex.imag",
            CirOpKind::ComplexImagPtr => "cir.complex.imag_ptr",
            CirOpKind::ComplexMul => "cir.complex.mul",
            CirOpKind::ComplexReal => "cir.complex.real",
            CirOpKind::ComplexRealPtr => "cir.complex.real_ptr",
            CirOpKind::ComplexSub => "cir.complex.sub",
            CirOpKind::Condition => "cir.condition",
            CirOpKind::Const => "cir.const",
            CirOpKind::Continue => "cir.continue",
            CirOpKind::Copy => "cir.copy",
            CirOpKind::Copysign => "cir.copysign",
            CirOpKind::Cos => "cir.cos",
            CirOpKind::Ctz => "cir.ctz",
            CirOpKind::Dec => "cir.dec",
            CirOpKind::Div => "cir.div",
            CirOpKind::DivOverflow => "cir.div.overflow",
            CirOpKind::Do => "cir.do",
            CirOpKind::EhSetjmp => "cir.eh.setjmp",
            CirOpKind::Exp => "cir.exp",
            CirOpKind::Exp2 => "cir.exp2",
            CirOpKind::Expect => "cir.expect",
            CirOpKind::ExtractMember => "cir.extract_member",
            CirOpKind::Fabs => "cir.fabs",
            CirOpKind::Fadd => "cir.fadd",
            CirOpKind::Fdiv => "cir.fdiv",
            CirOpKind::Ffs => "cir.ffs",
            CirOpKind::Floor => "cir.floor",
            CirOpKind::Fmaximum => "cir.fmaximum",
            CirOpKind::Fmaxnum => "cir.fmaxnum",
            CirOpKind::Fminimum => "cir.fminimum",
            CirOpKind::Fminnum => "cir.fminnum",
            CirOpKind::Fmod => "cir.fmod",
            CirOpKind::Fmul => "cir.fmul",
            CirOpKind::Fneg => "cir.fneg",
            CirOpKind::For => "cir.for",
            CirOpKind::FrameAddress => "cir.frame_address",
            CirOpKind::Fsub => "cir.fsub",
            CirOpKind::Func => "cir.func",
            CirOpKind::GetBitfield => "cir.get_bitfield",
            CirOpKind::GetElement => "cir.get_element",
            CirOpKind::GetGlobal => "cir.get_global",
            CirOpKind::GetMember => "cir.get_member",
            CirOpKind::Global => "cir.global",
            CirOpKind::Goto => "cir.goto",
            CirOpKind::If => "cir.if",
            CirOpKind::Inc => "cir.inc",
            CirOpKind::IndirectBr => "cir.indirect_br",
            CirOpKind::InsertMember => "cir.insert_member",
            CirOpKind::IsConstant => "cir.is_constant",
            CirOpKind::IsFpClass => "cir.is_fp_class",
            CirOpKind::Label => "cir.label",
            CirOpKind::LibcMemchr => "cir.libc.memchr",
            CirOpKind::LibcMemcpy => "cir.libc.memcpy",
            CirOpKind::LibcMemmove => "cir.libc.memmove",
            CirOpKind::LibcMemset => "cir.libc.memset",
            CirOpKind::Llrint => "cir.llrint",
            CirOpKind::Llround => "cir.llround",
            CirOpKind::Load => "cir.load",
            CirOpKind::Log => "cir.log",
            CirOpKind::Log10 => "cir.log10",
            CirOpKind::Log2 => "cir.log2",
            CirOpKind::Lrint => "cir.lrint",
            CirOpKind::Lround => "cir.lround",
            CirOpKind::Minus => "cir.minus",
            CirOpKind::Modf => "cir.modf",
            CirOpKind::Mul => "cir.mul",
            CirOpKind::MulOverflow => "cir.mul.overflow",
            CirOpKind::Nearbyint => "cir.nearbyint",
            CirOpKind::Not => "cir.not",
            CirOpKind::Objsize => "cir.objsize",
            CirOpKind::Or => "cir.or",
            CirOpKind::Parity => "cir.parity",
            CirOpKind::Popcount => "cir.popcount",
            CirOpKind::Pow => "cir.pow",
            CirOpKind::Prefetch => "cir.prefetch",
            CirOpKind::PtrDiff => "cir.ptr_diff",
            CirOpKind::PtrStride => "cir.ptr_stride",
            CirOpKind::Rem => "cir.rem",
            CirOpKind::RemOverflow => "cir.rem.overflow",
            CirOpKind::Return => "cir.return",
            CirOpKind::Rint => "cir.rint",
            CirOpKind::Rotate => "cir.rotate",
            CirOpKind::Round => "cir.round",
            CirOpKind::Roundeven => "cir.roundeven",
            CirOpKind::Scope => "cir.scope",
            CirOpKind::Select => "cir.select",
            CirOpKind::SetBitfield => "cir.set_bitfield",
            CirOpKind::Shift => "cir.shift",
            CirOpKind::Signbit => "cir.signbit",
            CirOpKind::Sin => "cir.sin",
            CirOpKind::Sqrt => "cir.sqrt",
            CirOpKind::Stackrestore => "cir.stackrestore",
            CirOpKind::Stacksave => "cir.stacksave",
            CirOpKind::Store => "cir.store",
            CirOpKind::Sub => "cir.sub",
            CirOpKind::SubOverflow => "cir.sub.overflow",
            CirOpKind::Switch => "cir.switch",
            CirOpKind::Tan => "cir.tan",
            CirOpKind::Ternary => "cir.ternary",
            CirOpKind::Trap => "cir.trap",
            CirOpKind::Triple => "cir.triple",
            CirOpKind::Trunc => "cir.trunc",
            CirOpKind::Unreachable => "cir.unreachable",
            CirOpKind::VaArg => "cir.va_arg",
            CirOpKind::VaEnd => "cir.va_end",
            CirOpKind::VaStart => "cir.va_start",
            CirOpKind::VecExtract => "cir.vec.extract",
            CirOpKind::VecInsert => "cir.vec.insert",
            CirOpKind::VecShuffle => "cir.vec.shuffle",
            CirOpKind::While => "cir.while",
            CirOpKind::Xor => "cir.xor",
            CirOpKind::Yield => "cir.yield",
            CirOpKind::Other(name) => name,
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn parse_recognizes_dotted_and_underscored_names() {
        assert_eq!(CirOpKind::parse("cir.add"), CirOpKind::Add);
        assert_eq!(CirOpKind::parse("cir.add.overflow"), CirOpKind::AddOverflow);
        assert_eq!(CirOpKind::parse("cir.get_bitfield"), CirOpKind::GetBitfield);
        assert_eq!(CirOpKind::parse("cir.eh.setjmp"), CirOpKind::EhSetjmp);
    }

    #[test]
    fn parse_recognizes_names_with_digits() {
        assert_eq!(CirOpKind::parse("cir.atan2"), CirOpKind::Atan2);
        assert_eq!(CirOpKind::parse("cir.exp2"), CirOpKind::Exp2);
        assert_eq!(CirOpKind::parse("cir.log10"), CirOpKind::Log10);
        assert_eq!(CirOpKind::parse("cir.log2"), CirOpKind::Log2);
    }

    #[test]
    fn parse_falls_back_to_other_for_unknown_names() {
        assert_eq!(
            CirOpKind::parse("cir.not_a_real_op"),
            CirOpKind::Other("cir.not_a_real_op".to_string())
        );
    }

    #[test]
    fn as_str_round_trips_through_parse() {
        for name in [
            "cir.add",
            "cir.add.overflow",
            "cir.get_bitfield",
            "cir.eh.setjmp",
            "cir.ptr_stride",
            "cir.for",
            "cir.if",
        ] {
            assert_eq!(CirOpKind::parse(name).as_str(), name);
        }
    }

    #[test]
    fn op_kind_reads_off_the_op_name() {
        let op = Op {
            results: vec![],
            name: "cir.store".to_string(),
            operands: vec![],
            successors: vec![],
            attrs: BTreeMap::new(),
            regions: vec![],
            ty: None,
            loc: None,
        };
        assert_eq!(op.kind(), CirOpKind::Store);
    }
}
