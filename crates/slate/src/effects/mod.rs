use crate::function_identity::Known;
use crate::rust_ast::{AtomicOrdering, AtomicRmwOp};

pub mod error;
pub mod interp;
pub mod interpreter;
pub mod support;

pub use error::{ArgShapeKind, BindingKind, Construct, EffectError, Found, ValueKind};

pub(crate) type EResult<T> = Result<T, EffectError>;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum CallSummary {
    Malloc,
    Calloc,
    Realloc,
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
    FPrintf,
    Exit,
    Puts,
    Fopen,
    Fputs,
    Fgets,
    Fread,
    Fwrite,
    Fclose,
    Fflush,
    Remove,
    Perror,
    Toupper,
    Tolower,
    IsAlpha,
    IsDigit,
    IsUpper,
    IsLower,
    IsAlnum,
    IsXDigit,
    IsPunct,
    IsCntrl,
    IsGraph,
    IsPrint,
    IsSpace,
    Sin,
    Cos,
    Tan,
    Log,
    Log10,
    Log2,
    Pow,
    Sqrt,
    Exp,
    Exp2,
    Fmod,
    Lround,
    Llround,
    MulDc3,
    DivDc3,
    PthreadCreate,
    PthreadJoin,
    Qsort,
    Bsearch,
}

pub(crate) fn call_summary(known: Known) -> CallSummary {
    match known {
        Known::Malloc => CallSummary::Malloc,
        Known::Calloc => CallSummary::Calloc,
        Known::Realloc => CallSummary::Realloc,
        Known::Free => CallSummary::Free,
        Known::MemCpy => CallSummary::Memcpy,
        Known::MemMove => CallSummary::Memmove,
        Known::MemSet => CallSummary::Memset,
        Known::MemChr => CallSummary::Memchr,
        Known::StrLen => CallSummary::Strlen,
        Known::StrCpy => CallSummary::Strcpy,
        Known::StrCat => CallSummary::Strcat,
        Known::StrNCpy => CallSummary::Strncpy,
        Known::StrNCat => CallSummary::Strncat,
        Known::StrCmp => CallSummary::Strcmp,
        Known::StrNCmp => CallSummary::Strncmp,
        Known::MemCmp => CallSummary::Memcmp,
        Known::StrChr => CallSummary::Strchr,
        Known::StrRChr => CallSummary::Strrchr,
        Known::StrStr => CallSummary::Strstr,
        Known::StrPBrk => CallSummary::Strpbrk,
        Known::StrSpn => CallSummary::Strspn,
        Known::StrCSpn => CallSummary::Strcspn,
        Known::Atoi => CallSummary::Atoi,
        Known::Atol => CallSummary::Atol,
        Known::StrTol => CallSummary::Strtol,
        Known::StrToul => CallSummary::Strtoul,
        Known::StrTod => CallSummary::Strtod,
        Known::Printf => CallSummary::Printf,
        Known::FPrintf => CallSummary::FPrintf,
        Known::Exit => CallSummary::Exit,
        Known::Puts => CallSummary::Puts,
        Known::FOpen => CallSummary::Fopen,
        Known::FPuts => CallSummary::Fputs,
        Known::FGets => CallSummary::Fgets,
        Known::FRead => CallSummary::Fread,
        Known::FWrite => CallSummary::Fwrite,
        Known::FClose => CallSummary::Fclose,
        Known::FFlush => CallSummary::Fflush,
        Known::Remove => CallSummary::Remove,
        Known::Perror => CallSummary::Perror,
        Known::ToUpper => CallSummary::Toupper,
        Known::ToLower => CallSummary::Tolower,
        Known::IsAlpha => CallSummary::IsAlpha,
        Known::IsDigit => CallSummary::IsDigit,
        Known::IsUpper => CallSummary::IsUpper,
        Known::IsLower => CallSummary::IsLower,
        Known::IsAlnum => CallSummary::IsAlnum,
        Known::IsXDigit => CallSummary::IsXDigit,
        Known::IsPunct => CallSummary::IsPunct,
        Known::IsCntrl => CallSummary::IsCntrl,
        Known::IsGraph => CallSummary::IsGraph,
        Known::IsPrint => CallSummary::IsPrint,
        Known::IsSpace => CallSummary::IsSpace,
        Known::Sin => CallSummary::Sin,
        Known::Cos => CallSummary::Cos,
        Known::Tan => CallSummary::Tan,
        Known::Log => CallSummary::Log,
        Known::Log10 => CallSummary::Log10,
        Known::Log2 => CallSummary::Log2,
        Known::Pow => CallSummary::Pow,
        Known::Sqrt => CallSummary::Sqrt,
        Known::Exp => CallSummary::Exp,
        Known::Exp2 => CallSummary::Exp2,
        Known::Fmod => CallSummary::Fmod,
        Known::Lround => CallSummary::Lround,
        Known::Llround => CallSummary::Llround,
        Known::PthreadCreate => CallSummary::PthreadCreate,
        Known::PthreadJoin => CallSummary::PthreadJoin,
        Known::Qsort => CallSummary::Qsort,
        Known::Bsearch => CallSummary::Bsearch,
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
    Function(String),
    File(FileId),
    Atomic(AtomicId),
    AtomicResult {
        ok: bool,
        value: OptionValue,
    },
    Tuple(Vec<Value>),
    Null,
    Option(Option<OptionValue>),
    Bytes(Vec<u8>),
}

#[derive(Debug, Clone, PartialEq)]
pub enum OptionValue {
    Int {
        width: IntWidth,
        signed: bool,
        value: i128,
    },
    Bool(bool),
    Ref(Location),
    Function(String),
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
