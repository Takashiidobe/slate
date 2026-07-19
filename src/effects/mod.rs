use crate::rust_ast::{AtomicOrdering, AtomicRmwOp};

pub mod interp;
pub mod interpreter;
pub mod support;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) enum CallSummary {
    Malloc,
    Calloc,
    Free,
    Memcpy,
    Memmove,
    Memset,
    Memchr,
    Strlen,
    Strcpy,
    Strcat,
    Strncpy,
    Strncat,
    Strcmp,
    Strncmp,
    Memcmp,
    Strchr,
    Strrchr,
    Strstr,
    Strpbrk,
    Strspn,
    Strcspn,
    Atoi,
    Atol,
    Strtol,
    Strtoul,
    Strtod,
    Printf,
    Puts,
    Fopen,
    Fputs,
    Fgets,
    Fread,
    Fwrite,
    Fclose,
    Remove,
    Toupper,
    Tolower,
    Qsort,
    Bsearch,
}

pub(crate) fn call_summary(name: &str) -> Option<CallSummary> {
    match name {
        "malloc" => Some(CallSummary::Malloc),
        "calloc" => Some(CallSummary::Calloc),
        "free" => Some(CallSummary::Free),
        "memcpy" => Some(CallSummary::Memcpy),
        "memmove" => Some(CallSummary::Memmove),
        "memset" => Some(CallSummary::Memset),
        "memchr" => Some(CallSummary::Memchr),
        "strlen" => Some(CallSummary::Strlen),
        "strcpy" => Some(CallSummary::Strcpy),
        "strcat" => Some(CallSummary::Strcat),
        "strncpy" => Some(CallSummary::Strncpy),
        "strncat" => Some(CallSummary::Strncat),
        "strcmp" => Some(CallSummary::Strcmp),
        "strncmp" => Some(CallSummary::Strncmp),
        "memcmp" => Some(CallSummary::Memcmp),
        "strchr" => Some(CallSummary::Strchr),
        "strrchr" => Some(CallSummary::Strrchr),
        "strstr" => Some(CallSummary::Strstr),
        "strpbrk" => Some(CallSummary::Strpbrk),
        "strspn" => Some(CallSummary::Strspn),
        "strcspn" => Some(CallSummary::Strcspn),
        "atoi" => Some(CallSummary::Atoi),
        "atol" => Some(CallSummary::Atol),
        "strtol" => Some(CallSummary::Strtol),
        "strtoul" => Some(CallSummary::Strtoul),
        "strtod" => Some(CallSummary::Strtod),
        "printf" => Some(CallSummary::Printf),
        "puts" => Some(CallSummary::Puts),
        "fopen" => Some(CallSummary::Fopen),
        "fputs" => Some(CallSummary::Fputs),
        "fgets" => Some(CallSummary::Fgets),
        "fread" => Some(CallSummary::Fread),
        "fwrite" => Some(CallSummary::Fwrite),
        "fclose" => Some(CallSummary::Fclose),
        "remove" => Some(CallSummary::Remove),
        "toupper" => Some(CallSummary::Toupper),
        "tolower" => Some(CallSummary::Tolower),
        "qsort" => Some(CallSummary::Qsort),
        "bsearch" => Some(CallSummary::Bsearch),
        _ => None,
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct AllocId(pub u32);

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct FileId(pub u32);

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct AtomicId(pub u32);

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct Location {
    pub alloc: AllocId,
    pub byte_offset: u64,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum IntWidth {
    W8,
    W16,
    W32,
    W64,
    W128,
    PointerSized,
}

#[derive(Debug, Clone, PartialEq)]
pub enum Value {
    Int {
        width: IntWidth,
        signed: bool,
        value: i128,
    },
    Float(f64),
    Bool(bool),
    Ref(Location),
    File(FileId),
    Atomic(AtomicId),
    AtomicResult {
        ok: bool,
        value: OptionValue,
    },
    Tuple(Vec<Value>),
    BlockLabel(&'static str),
    Null,
    Option(Option<OptionValue>),
    Bytes(Vec<u8>),
}

#[derive(Debug, Clone, Copy, PartialEq)]
pub enum OptionValue {
    Int {
        width: IntWidth,
        signed: bool,
        value: i128,
    },
    Bool(bool),
    Ref(Location),
}

#[derive(Debug, Clone, PartialEq)]
pub enum ParamSeed {
    Scalar(Value),
    Buffer(Vec<Value>),
}

#[derive(Debug, Clone, PartialEq)]
pub enum Effect {
    Alloc {
        alloc: AllocId,
        size: u64,
    },
    Dealloc {
        alloc: AllocId,
    },
    FileOpen {
        file: FileId,
        path: String,
        mode: String,
    },
    FileWrite {
        file: FileId,
        bytes: Vec<u8>,
    },
    FileClose {
        file: FileId,
    },
    AtomicLoad {
        atomic: AtomicId,
        ordering: AtomicOrdering,
        value: Value,
    },
    AtomicStore {
        atomic: AtomicId,
        ordering: AtomicOrdering,
        value: Value,
    },
    AtomicRmw {
        atomic: AtomicId,
        op: AtomicRmwOp,
        ordering: AtomicOrdering,
        operand: Value,
        old: Value,
        new: Value,
    },
    AtomicSwap {
        atomic: AtomicId,
        ordering: AtomicOrdering,
        old: Value,
        new: Value,
    },
    AtomicCompareExchange {
        atomic: AtomicId,
        success: AtomicOrdering,
        failure: AtomicOrdering,
        expected: Value,
        desired: Value,
        old: Value,
        exchanged: bool,
    },
    AtomicFence {
        ordering: AtomicOrdering,
    },
    Write {
        loc: Location,
        value: Value,
    },
    Read {
        loc: Location,
        value: Value,
    },
    Call {
        name: String,
        args: Vec<Value>,
    },
    Return(Value),
    Exit(i32),
}

#[derive(Debug, Clone, Default, PartialEq)]
pub struct EffectTrace {
    pub effects: Vec<Effect>,
}

impl EffectTrace {
    pub fn push(&mut self, effect: Effect) {
        self.effects.push(effect);
    }
}
