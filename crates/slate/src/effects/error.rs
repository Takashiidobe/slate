use std::fmt;

use crate::effects::{AllocId, CallSummary, Location, Value};
use crate::rust_ast::{AtomicRmwOp, BinOp, Expr, Path, Type, UnaryOp};

/// The namespace an unresolved name was looked up in.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum BindingKind {
    Scalar,
    Global,
    Vec,
    Struct,
    Record,
    OnceLock,
    File,
    Atomic,
    Field,
    Function,
    Collection,
}

impl fmt::Display for BindingKind {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let s = match self {
            BindingKind::Scalar => "scalar",
            BindingKind::Global => "global",
            BindingKind::Vec => "Vec",
            BindingKind::Struct => "struct",
            BindingKind::Record => "record",
            BindingKind::OnceLock => "OnceLock",
            BindingKind::File => "file",
            BindingKind::Atomic => "atomic",
            BindingKind::Field => "field",
            BindingKind::Function => "function",
            BindingKind::Collection => "collection",
        };
        write!(f, "{s}")
    }
}

/// The operation or syntactic position that rejected its input.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Construct {
    AtomicReceiver,
    AddrOfMut,
    AtomicPointerPlace,
    AtomicOrdering,
    BufReaderNew,
    OpenOptionsOpen,
    OpenOptionsMethod,
    OpenOptionsChain,
    OpenOptionsMode,
    CStringExpr,
    ArrayPointerArg,
    SomeComparator,
    ComparatorArg,
    SomePayload,
    CastTargetType,
    CollectionExpr,
    PointerComparison,
    PointerNullComparison,
    NullComparison,
    FileNullComparison,
    FloatBinop,
    AtomicPointerRmw,
    AssignTarget,
    ArrayAssignment,
    CompoundAssignBase,
    CompoundAssignTarget,
    ForLoopIterator,
    IterSourceReceiver,
    VecLocalType,
    VecInitializer,
    ArrayLocalType,
    ArrayInitializer,
    RecordArrayElemType,
    PushReceiver,
    DropTarget,
    PrintMacro,
    FormatMacro,
    WriteAllCall,
    FileArgument,
    AssignTargetBase,
    StringFromLiteral,
    PushStr,
    PathExpr,
    FieldArrayIndex,
    DerefFieldBase,
    FieldBase,
    PointerFieldBase,
    TraceFreeEval,
    AggregateArrayFieldOffset,
    AggregateFieldOffset,
    AddrOfMacro,
    OffsetOfMacro,
    UnaryOperand,
    IndexBase,
    TupleFieldName,
    PointerOffsetMethod,
    OffsetFromMethod,
    OverflowingMethodArg,
    TupleFieldValue,
    LibcCall(CallSummary),
    ReadUntilArgs,
    ReadTakeArgs,
    ParseStringMethod,
    ParseTargetType,
    UnwrapOrArg,
    UnwrapOrValue,
    SortByArg,
    SortByClosureParams,
    BinarySearchByArg,
    BinarySearchByClosureParams,
    MapOrArgs,
    MapOrClosureParams,
    MapOrValue,
    CompareMethodArg,
    ReadComparable,
    IterPositionArgs,
    IterPositionClosureParams,
    IterReduceReceiver,
    IterReduceAdapter,
    IterReduceIterReceiver,
    IterReduceKind,
    FoldArgs,
    FoldClosure,
    FoldClosureParams,
    SortArrayCollection,
    StrtoEndPointer,
    UnsupportedExpr,
    AtomicMethodArgs,
    AtomicMethod,
    IntegerMethodReceiver,
    IntegerMethodArg,
    OnceLockReceiver,
    OnceLockInitArgs,
    OnceLockInitParams,
    AddrOfExpr,
    AssertUncheckedArgs,
    CallTarget,
    SomeArg,
    ReadVolatileArgs,
    WriteVolatileArgs,
    PrimAlignOf,
    PrimSizeOf,
}

/// The shape an argument list or literal was expected to have.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ArgShapeKind {
    NoArguments,
    OneArgument,
    TwoArguments,
    ThreeArguments,
    FourArguments,
    FiveArguments,
    BoolLiteral,
    StringLiteralPath,
    FormatString,
}

impl fmt::Display for ArgShapeKind {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let s = match self {
            ArgShapeKind::NoArguments => "no arguments",
            ArgShapeKind::OneArgument => "one argument",
            ArgShapeKind::TwoArguments => "two arguments",
            ArgShapeKind::ThreeArguments => "three arguments",
            ArgShapeKind::FourArguments => "four arguments",
            ArgShapeKind::FiveArguments => "five arguments",
            ArgShapeKind::BoolLiteral => "a bool literal",
            ArgShapeKind::StringLiteralPath => "a string literal path",
            ArgShapeKind::FormatString => "a format string",
        };
        write!(f, "{s}")
    }
}

/// The shape of value an operation required, mirroring `Value`'s variants.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ValueKind {
    Int,
    Float,
    Bool,
    Ref,
    File,
    Atomic,
    AtomicResult,
    Tuple,
    BlockLabel,
    Null,
    Option,
    Bytes,
}

impl fmt::Display for ValueKind {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let s = match self {
            ValueKind::Int => "integer",
            ValueKind::Float => "float",
            ValueKind::Bool => "bool",
            ValueKind::Ref => "reference",
            ValueKind::File => "file",
            ValueKind::Atomic => "atomic",
            ValueKind::AtomicResult => "atomic result",
            ValueKind::Tuple => "tuple",
            ValueKind::BlockLabel => "block label",
            ValueKind::Null => "null",
            ValueKind::Option => "option",
            ValueKind::Bytes => "bytes",
        };
        write!(f, "{s}")
    }
}

impl From<&Value> for ValueKind {
    fn from(value: &Value) -> Self {
        match value {
            Value::Int { .. } => ValueKind::Int,
            Value::Float(_) => ValueKind::Float,
            Value::Bool(_) => ValueKind::Bool,
            Value::Ref(_) => ValueKind::Ref,
            Value::File(_) => ValueKind::File,
            Value::Atomic(_) => ValueKind::Atomic,
            Value::AtomicResult { .. } => ValueKind::AtomicResult,
            Value::Tuple(_) => ValueKind::Tuple,
            Value::BlockLabel(_) => ValueKind::BlockLabel,
            Value::Null => ValueKind::Null,
            Value::Option(_) => ValueKind::Option,
            Value::Bytes(_) => ValueKind::Bytes,
        }
    }
}

/// A value the interpreter rejected, kept in its native type so callers can
/// pattern-match on it instead of re-parsing a rendered string.
#[derive(Debug, Clone)]
pub enum Found {
    Expr(Expr),
    Type(Type),
    Value(Value),
    Path(Path),
    BinOp(BinOp),
    UnaryOp(UnaryOp),
    AtomicRmwOp(AtomicRmwOp),
    Str(String),
    OptStr(Option<String>),
    OpenOptionsFlags {
        read: bool,
        write: bool,
        append: bool,
        create: bool,
        truncate: bool,
    },
    Offset(u64),
}

impl From<Expr> for Found {
    fn from(expr: Expr) -> Self {
        Found::Expr(expr)
    }
}

impl From<Type> for Found {
    fn from(ty: Type) -> Self {
        Found::Type(ty)
    }
}

impl From<Value> for Found {
    fn from(value: Value) -> Self {
        Found::Value(value)
    }
}

impl From<Path> for Found {
    fn from(path: Path) -> Self {
        Found::Path(path)
    }
}

impl From<BinOp> for Found {
    fn from(op: BinOp) -> Self {
        Found::BinOp(op)
    }
}

impl From<UnaryOp> for Found {
    fn from(op: UnaryOp) -> Self {
        Found::UnaryOp(op)
    }
}

impl From<AtomicRmwOp> for Found {
    fn from(op: AtomicRmwOp) -> Self {
        Found::AtomicRmwOp(op)
    }
}

impl From<String> for Found {
    fn from(s: String) -> Self {
        Found::Str(s)
    }
}

impl From<&str> for Found {
    fn from(s: &str) -> Self {
        Found::Str(s.to_string())
    }
}

impl From<Option<String>> for Found {
    fn from(s: Option<String>) -> Self {
        Found::OptStr(s)
    }
}

impl From<u64> for Found {
    fn from(offset: u64) -> Self {
        Found::Offset(offset)
    }
}

#[derive(Debug, thiserror::Error)]
pub enum EffectError {
    /// An AST shape the interpreter does not (yet) handle.
    #[error("unsupported {construct:?}: {found:?}")]
    Unsupported { construct: Construct, found: Found },

    /// A call/macro/method was applied to the wrong number or shape of arguments.
    #[error("{construct:?} expects {expected}")]
    ArgShape {
        construct: Construct,
        expected: ArgShapeKind,
    },

    /// A name was not found in the namespace it was looked up in.
    #[error("unknown {kind} `{name}`")]
    UnknownBinding { kind: BindingKind, name: String },

    /// A freed allocation was read, written, or otherwise touched.
    #[error("use of `{name}` after free")]
    UseAfterFree { name: String },

    /// The same allocation was freed twice.
    #[error("double free of {alloc:?}")]
    DoubleFree { alloc: AllocId },

    /// An allocation was referenced with no struct binding registered for it.
    #[error("unknown struct allocation {alloc:?}")]
    UnknownAlloc { alloc: AllocId },

    /// A pointer-difference operation compared pointers into different allocations.
    #[error("offset_from across different allocations: {lhs:?}, {rhs:?}")]
    CrossAllocationOffset { lhs: AllocId, rhs: AllocId },

    /// A heap location was read before any write ever reached it.
    #[error("read from never-written {loc:?}")]
    UninitializedRead { loc: Location },

    /// A value did not have the shape an operation required.
    #[error("expected a {expected} value, found {found:?}")]
    TypeMismatch { expected: ValueKind, found: Found },

    /// A `match` over interpreter values had no arm for the value produced.
    #[error("no match arm for {value:?}")]
    NoMatchArm { value: Value },

    /// Two related sizes (e.g. an array literal and its declared length) disagreed.
    #[error("length mismatch in {construct:?}: expected {expected}, found {found}")]
    LengthMismatch {
        construct: Construct,
        expected: usize,
        found: usize,
    },

    /// An index fell outside the bounds of the collection it indexed into.
    #[error("index {index} out of range (len {len})")]
    IndexOutOfRange { index: usize, len: usize },

    /// A condition the interpreter's own invariants should have ruled out.
    #[error("internal error: {0}")]
    Internal(String),
}

impl EffectError {
    pub fn unsupported(construct: Construct, found: impl Into<Found>) -> Self {
        EffectError::Unsupported {
            construct,
            found: found.into(),
        }
    }

    pub fn arg_shape(construct: Construct, expected: ArgShapeKind) -> Self {
        EffectError::ArgShape {
            construct,
            expected,
        }
    }

    pub fn unknown(kind: BindingKind, name: impl Into<String>) -> Self {
        EffectError::UnknownBinding {
            kind,
            name: name.into(),
        }
    }

    pub fn use_after_free(name: impl Into<String>) -> Self {
        EffectError::UseAfterFree { name: name.into() }
    }

    pub fn double_free(alloc: AllocId) -> Self {
        EffectError::DoubleFree { alloc }
    }

    pub fn unknown_alloc(alloc: AllocId) -> Self {
        EffectError::UnknownAlloc { alloc }
    }

    pub fn cross_allocation_offset(lhs: AllocId, rhs: AllocId) -> Self {
        EffectError::CrossAllocationOffset { lhs, rhs }
    }

    pub fn uninitialized_read(loc: Location) -> Self {
        EffectError::UninitializedRead { loc }
    }

    pub fn type_mismatch(expected: ValueKind, found: impl Into<Found>) -> Self {
        EffectError::TypeMismatch {
            expected,
            found: found.into(),
        }
    }

    pub fn no_match_arm(value: Value) -> Self {
        EffectError::NoMatchArm { value }
    }

    pub fn length_mismatch(construct: Construct, expected: usize, found: usize) -> Self {
        EffectError::LengthMismatch {
            construct,
            expected,
            found,
        }
    }

    pub fn index_out_of_range(index: usize, len: usize) -> Self {
        EffectError::IndexOutOfRange { index, len }
    }

    pub fn internal(message: impl Into<String>) -> Self {
        EffectError::Internal(message.into())
    }
}
