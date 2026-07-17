use std::collections::BTreeSet;

use crate::rust_ast::{Block, Expr, IndentStmt, Item, Program, Stmt, Type};

pub(super) mod anonymous_structs;
pub(super) mod array_element_pointer_origin;
pub(super) mod atomic_locals;
pub(super) mod borrow_alias;
pub(super) mod buffer_cursor;
pub(super) mod c_strings;
pub(super) mod calls;
pub(super) mod casts;
pub(super) mod control_flow;
pub(super) mod counted_loop;
pub(super) mod def_use;
pub(super) mod effects;
pub(super) mod file_ownership;
pub(crate) mod goto;
pub(super) mod heap_ownership;
pub(super) mod loop_shapes;
pub(super) mod places;
pub(super) mod printf;
pub(super) mod ptr_len;
pub(super) mod retval;
pub(super) mod slice_index;
pub(super) mod string_params;
pub(super) mod strings;
pub(super) mod temp_chains;
pub(super) mod va_list;
pub(super) mod values;
pub(super) mod walk;

#[derive(Debug, Clone)]
pub(super) struct AnalyzedProgram {
    pub(super) program: Program,
    pub(super) facts: FixupFacts,
}

#[derive(Debug, Default, Clone)]
pub(super) struct FixupFacts {
    pub(super) functions: Vec<FunctionFact>,
    pub(super) bindings: Vec<BindingFact>,
    pub(super) binding_types: Vec<BindingTypeFact>,
    pub(super) loops: Vec<LoopFact>,
    pub(super) borrow_alias: Vec<BorrowAliasFact>,
    pub(super) def_use: Vec<DefUseFact>,
    pub(super) effects: Vec<EffectFact>,
    pub(super) control_flow: Vec<ControlFlowFact>,
    pub(super) casts: Vec<CastFact>,
    pub(super) places: Vec<PlaceFact>,
    pub(super) values: Vec<ValueFact>,
    pub(super) call_signatures: Vec<CallSignatureFact>,
    pub(super) callsites: Vec<CallsiteFact>,
    pub(super) string_buffers: Vec<StringBufferFact>,
    pub(super) ascii_numeric_strings: Vec<AsciiNumericStringFact>,
    pub(super) string_pointer_views: Vec<StringPointerViewFact>,
    pub(super) string_libc_uses: Vec<StringLibcUseFact>,
    pub(super) string_lift_plans: Vec<StringLiftPlanFact>,
    pub(super) string_param_lifts: Vec<StringParamLiftFact>,
    pub(super) string_copy_rewrites: Vec<StringCopyRewriteFact>,
    pub(super) c_string_literals: Vec<CStringLiteralFact>,
    pub(super) file_ownership: Vec<FileOwnershipFact>,
    pub(super) heap_ownership: Vec<HeapOwnershipFact>,
    pub(super) printf_calls: Vec<PrintfCallFact>,
    pub(super) ptr_len_slices: Vec<PtrLenSliceFact>,
    pub(super) array_element_pointer_origins: Vec<ArrayElementPointerOriginFact>,
    pub(super) buffer_pointer_fields: Vec<BufferPointerFieldFact>,
    pub(super) anonymous_structs: Vec<AnonymousStructFact>,
    pub(super) atomic_locals: Vec<AtomicLocalFact>,
    pub(super) atomic_globals: Vec<AtomicGlobalFact>,
    pub(super) slice_pointer_views: Vec<SlicePointerViewFact>,
    pub(super) slice_index_ranges: Vec<SliceIndexRangeFact>,
    pub(super) slice_pointer_indexes: Vec<SlicePointerIndexFact>,
    pub(super) counted_loops: Vec<CountedLoopFact>,
    pub(super) counted_slice_loops: Vec<CountedSliceLoopFact>,
    pub(super) loop_shapes: Vec<LoopShapeFact>,
    pub(super) loop_shape_rejections: Vec<LoopShapeRejectionFact>,
    pub(super) retval_collapses: Vec<RetvalCollapseFact>,
    pub(super) temp_chains: Vec<TempChainFact>,
    pub(super) va_list_aliases: Vec<VaListAliasFact>,
    pub(super) relations: Vec<FactRelation>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct FunctionId(pub(super) usize);

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct BindingId(pub(super) usize);

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct LoopId(pub(super) usize);

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct SignatureId(pub(super) usize);

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct FunctionFact {
    pub(super) id: FunctionId,
    pub(super) name: String,
    pub(super) item_index: usize,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct BindingFact {
    pub(super) id: BindingId,
    pub(super) function: FunctionId,
    pub(super) name: String,
    pub(super) kind: BindingKind,
    pub(super) path: AstPath,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct BindingTypeFact {
    pub(super) binding: BindingId,
    pub(super) rendered: String,
}

/// A `let mut <name>: <int>` local whose every use is the pointer operand of
/// an atomic op of width `ty`; safe to re-declare as native `AtomicN` storage.
#[derive(Debug, Clone)]
pub(super) struct AtomicLocalFact {
    pub(super) function: FunctionId,
    pub(super) name: String,
    pub(super) ty: crate::rust_ast::AtomicType,
}

#[derive(Debug, Clone)]
pub(super) struct AtomicGlobalFact {
    pub(super) name: String,
    pub(super) ty: crate::rust_ast::AtomicType,
}

#[derive(Debug, Clone)]
pub(super) struct AnonymousStructFact {
    pub(super) original_name: String,
    pub(super) generated_name: String,
    pub(super) fields: Vec<AnonymousStructFieldFact>,
}

#[derive(Debug, Clone)]
pub(super) struct AnonymousStructFieldFact {
    pub(super) name: String,
    pub(super) ty: Type,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct BufferPointerFieldFact {
    pub(super) function: FunctionId,
    pub(super) buffer: BindingId,
    pub(super) array: BindingId,
    pub(super) field: String,
    pub(super) index: usize,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) enum BindingKind {
    Param { index: usize },
    Local,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct LoopFact {
    pub(super) id: LoopId,
    pub(super) function: FunctionId,
    pub(super) kind: LoopKind,
    pub(super) path: AstPath,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct BorrowAliasFact {
    pub(super) function: FunctionId,
    pub(super) binding: BindingId,
    pub(super) state: BorrowAliasState,
    pub(super) reasons: BTreeSet<BorrowAliasReason>,
    pub(super) uses: Vec<BorrowAliasUseFact>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct BorrowAliasUseFact {
    pub(super) kind: BorrowAliasUseKind,
    pub(super) path: AstPath,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct DefUseFact {
    pub(super) function: FunctionId,
    pub(super) binding: BindingId,
    pub(super) definition: AstPath,
    pub(super) reads: Vec<AstPath>,
    pub(super) writes: Vec<AstPath>,
    pub(super) last_use: Option<AstPath>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct RetvalCollapseFact {
    pub(super) function: FunctionId,
    pub(super) return_path: AstPath,
    pub(super) value_path: AstPath,
    pub(super) remove_paths: Vec<AstPath>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct TempChainFact {
    pub(super) function: FunctionId,
    pub(super) binding: BindingId,
    pub(super) producer_path: AstPath,
    pub(super) consumer_path: AstPath,
    pub(super) dependencies: Vec<BindingId>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct VaListAliasFact {
    pub(super) function: FunctionId,
    pub(super) param: BindingId,
    pub(super) local: BindingId,
    pub(super) local_decl_path: AstPath,
    pub(super) clone_assign_path: AstPath,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct EffectFact {
    pub(super) function: FunctionId,
    pub(super) subject: EffectSubject,
    pub(super) path: AstPath,
    pub(super) purity: Purity,
    pub(super) effects: BTreeSet<EffectKind>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct ControlFlowFact {
    pub(super) function: FunctionId,
    pub(super) subject: ControlFlowSubject,
    pub(super) path: AstPath,
    pub(super) reachable: bool,
    pub(super) falls_through: bool,
    pub(super) exits: BTreeSet<ControlFlowExit>,
    pub(super) single_exit: bool,
    pub(super) has_unreachable_tail: bool,
    pub(super) expression_eligible: bool,
}

#[derive(Debug, Clone)]
pub(super) struct CastFact {
    pub(super) function: FunctionId,
    pub(super) path: AstPath,
    pub(super) from: Option<Type>,
    pub(super) to: Type,
    pub(super) kind: CastKind,
    pub(super) required: bool,
    pub(super) reasons: BTreeSet<CastRequirement>,
    pub(super) removable_candidate: bool,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct PlaceFact {
    pub(super) function: FunctionId,
    pub(super) path: AstPath,
    pub(super) access: PlaceAccess,
    pub(super) kind: PlaceKind,
    pub(super) readable: bool,
    pub(super) assignable: bool,
    pub(super) ordinary_slot: bool,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct ValueFact {
    pub(super) function: FunctionId,
    pub(super) subject: ValueSubject,
    pub(super) path: AstPath,
    pub(super) value: ConstValue,
}

#[derive(Debug, Clone)]
pub(super) struct CallSignatureFact {
    pub(super) id: SignatureId,
    pub(super) name: String,
    pub(super) source: CallSignatureSource,
    pub(super) params: Vec<CallParamFact>,
    pub(super) variadic: bool,
    pub(super) ret: Option<Type>,
    pub(super) semantics: BTreeSet<LibcCallSemantic>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum CallSignatureSource {
    Function(FunctionId),
    Extern {
        item_index: usize,
        decl_index: usize,
    },
}

#[derive(Debug, Clone)]
pub(super) struct CallParamFact {
    pub(super) index: usize,
    pub(super) name: String,
    pub(super) ty: Type,
}

#[derive(Debug, Clone)]
pub(super) struct CallsiteFact {
    pub(super) function: FunctionId,
    pub(super) path: AstPath,
    pub(super) callee: CallCallee,
    pub(super) args: Vec<CallArgFact>,
    pub(super) variadic_boundary: Option<usize>,
    pub(super) ret: Option<Type>,
    pub(super) result_binding: Option<BindingId>,
    pub(super) semantics: BTreeSet<LibcCallSemantic>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) enum CallCallee {
    Direct {
        name: String,
        signature: Option<SignatureId>,
    },
    Indirect,
}

#[derive(Debug, Clone)]
pub(super) struct CallArgFact {
    pub(super) slot: usize,
    pub(super) path: AstPath,
    pub(super) declared_ty: Option<Type>,
    pub(super) variadic: bool,
    pub(super) pinning: CallArgPinning,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum CallArgPinning {
    DeclaredParam,
    VariadicUnpinned,
    UnknownCallee,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub(super) enum LibcCallSemantic {
    Printf,
    StrLen,
    StrCmp,
    StrNCmp,
    MemCmp,
    StrCpy,
    StrNCpy,
    StrCat,
    StrNCat,
    MemCpy,
    MemSet,
    FOpen,
    FRead,
    FWrite,
    FGets,
    FPuts,
    FClose,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum ValueSubject {
    Expr,
    Binding(BindingId),
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord)]
pub(super) enum ConstValue {
    Integer(i128),
    Usize(usize),
    Bool(bool),
    Bytes(Vec<u8>),
    CStringBytes(Vec<u8>),
    String(String),
    Zero,
    ArrayLength(usize),
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct StringBufferFact {
    pub(super) function: FunctionId,
    pub(super) binding: BindingId,
    pub(super) path: AstPath,
    pub(super) kind: StringBufferKind,
    pub(super) provenance: StringBufferProvenance,
    pub(super) bytes: Option<Vec<u8>>,
    pub(super) nul_termination: NulTermination,
    pub(super) interior_nul: bool,
    pub(super) ascii_only: bool,
    pub(super) candidates: BTreeSet<StringRecoveryCandidate>,
    pub(super) rejections: BTreeSet<StringBufferRejection>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct AsciiNumericStringFact {
    pub(super) function: FunctionId,
    pub(super) binding: BindingId,
    pub(super) path: AstPath,
    pub(super) sign: AsciiNumericSign,
    pub(super) digits: usize,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum AsciiNumericSign {
    None,
    Plus,
    Minus,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum StringBufferKind {
    CharArray,
    BorrowedStr,
    BorrowedCStr,
    BorrowedBytes,
    OwnedString,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) enum StringBufferProvenance {
    Literal,
    ZeroInitialized,
    AssignedLiteral { assignment: AstPath },
    Lifted,
    Unknown,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum NulTermination {
    NotApplicable,
    Unterminated,
    Terminated,
    AllZero,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub(super) enum StringRecoveryCandidate {
    BorrowedStr,
    BorrowedCStr,
    BorrowedBytes,
    OwnedString,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub(super) enum StringBufferRejection {
    Indexed,
    Mutated,
    UnsupportedInitializer,
    Unterminated,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct StringPointerViewFact {
    pub(super) function: FunctionId,
    pub(super) source: BindingId,
    pub(super) path: AstPath,
    pub(super) mutable: bool,
    pub(super) kind: StringPointerViewKind,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum StringPointerViewKind {
    As,
    AsMut,
    Array,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct StringLibcUseFact {
    pub(super) function: FunctionId,
    pub(super) callee: StringLibcFunction,
    pub(super) path: AstPath,
    pub(super) pointer_args: Vec<BindingId>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct StringLiftPlanFact {
    pub(super) function: FunctionId,
    pub(super) binding: BindingId,
    pub(super) path: AstPath,
    pub(super) recovery: StringRecoveryCandidate,
    pub(super) remove_assignment: Option<AstPath>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct StringParamLiftFact {
    pub(super) callee: FunctionId,
    pub(super) param: BindingId,
    pub(super) index: usize,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct StringCopyRewriteFact {
    pub(super) function: FunctionId,
    pub(super) path: AstPath,
    pub(super) dst: BindingId,
    pub(super) rewrite: StringCopyRewrite,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct CStringLiteralFact {
    pub(super) function: FunctionId,
    pub(super) receiver_path: AstPath,
    pub(super) bytes: Vec<u8>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct FileOwnershipFact {
    pub(super) function: FunctionId,
    pub(super) handle: BindingId,
    pub(super) open_temp: BindingId,
    pub(super) close_temp: Option<BindingId>,
    pub(super) handle_path: AstPath,
    pub(super) open_path: AstPath,
    pub(super) assign_path: AstPath,
    pub(super) close_path: AstPath,
    pub(super) path_arg: AstPath,
    pub(super) mode_arg: AstPath,
    pub(super) mode: Option<FileOpenMode>,
    pub(super) uses: Vec<FileUseFact>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum FileOpenMode {
    Read,
    Write,
    Append,
    ReadUpdate,
    WriteUpdate,
    AppendUpdate,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct FileUseFact {
    pub(super) path: AstPath,
    pub(super) kind: FileUseKind,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum FileUseKind {
    Read,
    Write,
    Gets,
    Puts,
    Close,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) enum StringCopyRewrite {
    AssignLiteral(String),
    AssignOwned(BindingId),
    PushLiteral(String),
    PushOwned(BindingId),
}

#[derive(Debug, Clone)]
pub(super) struct HeapOwnershipFact {
    pub(super) function: FunctionId,
    pub(super) pointer: BindingId,
    pub(super) allocation_temp: BindingId,
    pub(super) size_temp: Option<BindingId>,
    pub(super) free_temp: Option<BindingId>,
    pub(super) pointer_path: AstPath,
    pub(super) allocation_path: AstPath,
    pub(super) assign_path: AstPath,
    pub(super) free_path: AstPath,
    pub(super) elem_ty: Type,
    pub(super) allocation: HeapAllocationKind,
    pub(super) extent: HeapExtent,
    pub(super) init: HeapInitKind,
    pub(super) read_safety: HeapReadSafety,
    pub(super) uses: Vec<HeapUseFact>,
    pub(super) reallocations: Vec<HeapReallocFact>,
    pub(super) kind: HeapOwnershipKind,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum HeapOwnershipKind {
    ScalarBox,
    VecBuffer,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) enum HeapAllocationKind {
    Malloc,
    Calloc,
}

#[derive(Debug, Clone)]
pub(super) enum HeapExtent {
    Scalar,
    Elements { count: Expr },
    Unknown,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum HeapInitKind {
    Uninitialized,
    Zeroed,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum HeapReadSafety {
    ZeroInitialized,
    ReadsAfterWrites,
    MayReadUninitialized,
}

#[derive(Debug, Clone)]
pub(super) struct HeapUseFact {
    pub(super) path: AstPath,
    pub(super) kind: HeapUseKind,
}

#[derive(Debug, Clone)]
pub(super) enum HeapUseKind {
    ScalarRead,
    ScalarWrite,
    IndexedRead { index: Expr },
    IndexedWrite { index: Expr },
    Free,
}

#[derive(Debug, Clone)]
pub(super) struct HeapReallocFact {
    pub(super) source_temp: Option<BindingId>,
    pub(super) allocation_temp: BindingId,
    pub(super) size_temp: Option<BindingId>,
    pub(super) allocation_path: AstPath,
    pub(super) assign_path: AstPath,
    pub(super) new_extent: HeapExtent,
    pub(super) init: HeapInitKind,
    pub(super) resize: HeapResizeKind,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum HeapResizeKind {
    Grow,
    Shrink,
    SameOrUnknown,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct PrintfCallFact {
    pub(super) function: FunctionId,
    pub(super) path: AstPath,
    pub(super) format: Option<Vec<u8>>,
    pub(super) arg_paths: Vec<AstPath>,
    pub(super) arg_facts: Vec<PrintfArgFact>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct PrintfArgFact {
    pub(super) path: AstPath,
    pub(super) const_string: Option<String>,
    pub(super) const_char: Option<String>,
    pub(super) rust_string: bool,
    pub(super) pointer: bool,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum StringLibcFunction {
    StrLen,
    StrCmp,
    StrNCmp,
    MemCmp,
    StrChr,
    StrRChr,
    StrStr,
    StrPBrk,
    StrSpn,
    StrCSpn,
    StrCpy,
    StrNCpy,
    StrCat,
    StrNCat,
    Atoi,
    Atol,
    StrTol,
    StrToul,
    StrTod,
    Printf,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) enum PlaceKind {
    Local {
        name: String,
    },
    Projection {
        root: PlaceRoot,
        projections: Vec<PlaceProjection>,
    },
    Volatile {
        access: VolatileAccess,
    },
    Atomic {
        access: AtomicPlaceAccess,
    },
    Unsupported,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) enum PlaceRoot {
    Local { name: String },
    Unsupported,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) enum PlaceProjection {
    Deref,
    Field(String),
    TupleField(usize),
    Index,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum PlaceAccess {
    Read,
    Write,
    ReadWrite,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum VolatileAccess {
    Read,
    Write,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum AtomicPlaceAccess {
    Read,
    Write,
    ReadWrite,
}

#[derive(Debug, Clone)]
pub(super) struct PtrLenSliceFact {
    pub(super) caller: FunctionId,
    pub(super) callee: FunctionId,
    pub(super) ptr_param: BindingId,
    pub(super) len_param: Option<BindingId>,
    pub(super) mutable: bool,
    pub(super) elem_ty: Type,
}

#[derive(Debug, Clone)]
pub(super) struct ArrayElementPointerOriginFact {
    pub(super) function: FunctionId,
    pub(super) pointer: BindingId,
    pub(super) base: BindingId,
    pub(super) index: Expr,
    pub(super) mutable: bool,
    pub(super) path: AstPath,
}

#[derive(Debug, Clone)]
pub(super) struct SlicePointerViewFact {
    pub(super) function: FunctionId,
    pub(super) pointer: BindingId,
    pub(super) slice: BindingId,
    pub(super) mutable: bool,
    pub(super) elem_ty: Type,
    pub(super) path: AstPath,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct SliceIndexRangeFact {
    pub(super) function: FunctionId,
    pub(super) index: BindingId,
    pub(super) slice: BindingId,
    pub(super) lower: IndexLowerBound,
    pub(super) upper: IndexUpperBound,
    pub(super) loop_path: AstPath,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct SlicePointerIndexFact {
    pub(super) function: FunctionId,
    pub(super) pointer: BindingId,
    pub(super) slice: BindingId,
    pub(super) offset_index: BindingId,
    pub(super) ranged_index: BindingId,
    pub(super) unit: PointerOffsetUnit,
    pub(super) path: AstPath,
}

#[derive(Debug, Clone)]
pub(super) struct CountedLoopFact {
    pub(super) function: FunctionId,
    pub(super) loop_id: LoopId,
    pub(super) index: BindingId,
    pub(super) bound: Expr,
    pub(super) start: CountedLoopStart,
    pub(super) step: CountedLoopStep,
    pub(super) index_use: CountedLoopIndexUse,
    pub(super) loop_path: AstPath,
    pub(super) body_path: AstPath,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct CountedSliceLoopFact {
    pub(super) function: FunctionId,
    pub(super) loop_id: LoopId,
    pub(super) index: BindingId,
    pub(super) slice: BindingId,
    pub(super) start: CountedLoopStart,
    pub(super) bound: CountedLoopBound,
    pub(super) step: CountedLoopStep,
    pub(super) index_use: CountedLoopIndexUse,
    pub(super) access: SliceLoopAccess,
    pub(super) loop_path: AstPath,
    pub(super) body_path: AstPath,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct LoopShapeFact {
    pub(super) function: FunctionId,
    pub(super) loop_id: LoopId,
    pub(super) kind: LoopShapeKind,
    pub(super) induction: Option<BindingId>,
    pub(super) accumulators: Vec<BindingId>,
    pub(super) collections: Vec<BindingId>,
    pub(super) mutation_targets: Vec<BindingId>,
    pub(super) loop_path: AstPath,
    pub(super) body_path: AstPath,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) enum LoopShapeKind {
    Counted { access: SliceLoopAccess },
    Reduction { op: ReductionOp },
    Search { result: SearchResult },
    Copy,
    Fill,
    Sentinel { target: SentinelTarget },
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum ReductionOp {
    Add,
    Mul,
    BitAnd,
    BitOr,
    BitXor,
    And,
    Or,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum SearchResult {
    BreaksOnMatch,
    AssignsFlag,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum SentinelTarget {
    IndexedCollection,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct LoopShapeRejectionFact {
    pub(super) function: FunctionId,
    pub(super) loop_id: LoopId,
    pub(super) attempted: LoopShapeKindTag,
    pub(super) reason: LoopShapeRejection,
    pub(super) loop_path: AstPath,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum LoopShapeKindTag {
    Counted,
    Reduction,
    Search,
    Copy,
    Fill,
    Sentinel,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub(super) enum LoopShapeRejection {
    AmbiguousBody,
    MissingCollection,
    MissingInduction,
    MissingMutation,
    MultipleMutations,
    UnsupportedControlFlow,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum IndexLowerBound {
    Zero,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum IndexUpperBound {
    SliceLen,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum PointerOffsetUnit {
    Elements,
    Bytes,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum CountedLoopStart {
    Zero,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum CountedLoopBound {
    SliceLen,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum CountedLoopStep {
    One,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum CountedLoopIndexUse {
    Unused,
    Other,
    SliceIndexOnly,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum SliceLoopAccess {
    ReadOnly,
    Mutable,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum EffectSubject {
    Expr,
    Stmt,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum ControlFlowSubject {
    Body,
    Block,
    Stmt,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord)]
pub(super) enum ControlFlowExit {
    Return,
    Break(Option<String>),
    Continue(Option<String>),
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum CastKind {
    NoOp,
    IntegerSignChange,
    IntegerWidthChange,
    IntegerSignAndWidthChange,
    IntegerSameShape,
    FloatWidthChange,
    FloatInteger,
    PointerCast,
    ReferenceCoercion,
    SliceCoercion,
    LiteralInferenceGuard,
    Semantic,
    Unknown,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub(super) enum CastRequirement {
    Abi,
    Semantics,
    Inference,
    RustCoercion,
    UnknownSource,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum Purity {
    MovablePure,
    ReadOnly,
    Effectful,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub(super) enum EffectKind {
    ReadOnlyCall,
    UnknownCall,
    MethodCall,
    MacroExpansion,
    VolatileRead,
    VolatileWrite,
    AtomicRead,
    AtomicWrite,
    MemoryWrite,
    UnknownSideEffect,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub(super) enum BorrowAliasState {
    ReadOnly,
    UniqueMutation,
    Escaped,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub(super) enum BorrowAliasReason {
    Read,
    Assigned,
    MutatedProjection,
    SharedBorrow,
    MutableBorrow,
    AddressTaken,
    RawPtrDerived,
    UnknownCallEscape,
    AtomicAccess,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum BorrowAliasUseKind {
    Read,
    Assigned,
    MutatedProjection,
    SharedBorrow,
    MutableBorrow,
    AddressTaken,
    RawPtrDerived,
    UnknownCallEscape,
    AtomicAccess,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) enum LoopKind {
    Loop,
    While,
    For,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct FactRelation {
    pub(super) kind: RelationKind,
    pub(super) members: Vec<SemanticId>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) enum RelationKind {
    Supersedes,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum SemanticId {
    Function(FunctionId),
    Binding(BindingId),
    Loop(LoopId),
}

#[derive(Debug, Default, Clone, PartialEq, Eq)]
pub(super) struct AstPath(pub(super) Vec<PathSegment>);

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) enum PathSegment {
    Stmt(usize),
    Then,
    Else,
    LoopBody,
    ForBody,
    ScopeBody,
    LabeledBody,
    MatchArm(usize),
    UnsafeBody,
    WhileBody,
    BlockBody,
    BlockTail,
    Expr(usize),
}

impl FixupFacts {
    pub(super) fn function_by_item_index(&self, item_index: usize) -> Option<FunctionId> {
        self.functions
            .iter()
            .find(|function| function.item_index == item_index)
            .map(|function| function.id)
    }

    pub(super) fn function_item_index(&self, function: FunctionId) -> Option<usize> {
        self.functions
            .iter()
            .find(|fact| fact.id == function)
            .map(|fact| fact.item_index)
    }

    pub(super) fn binding_by_param_index(
        &self,
        function: FunctionId,
        index: usize,
    ) -> Option<BindingId> {
        self.bindings
            .iter()
            .find(|binding| {
                binding.function == function
                    && matches!(binding.kind, BindingKind::Param { index: i } if i == index)
            })
            .map(|binding| binding.id)
    }

    pub(super) fn binding_by_local_path(
        &self,
        function: FunctionId,
        name: &str,
        path: &AstPath,
    ) -> Option<BindingId> {
        self.bindings
            .iter()
            .find(|binding| {
                binding.function == function
                    && binding.name == name
                    && binding.kind == BindingKind::Local
                    && &binding.path == path
            })
            .map(|binding| binding.id)
    }

    pub(super) fn binding_name(&self, binding: BindingId) -> Option<&str> {
        self.bindings
            .iter()
            .find(|fact| fact.id == binding)
            .map(|fact| fact.name.as_str())
    }

    /// Most-recently-declared binding named `name` in `function`.
    pub(super) fn binding_named(&self, function: FunctionId, name: &str) -> Option<BindingId> {
        self.bindings
            .iter()
            .rev()
            .find(|binding| binding.function == function && binding.name == name)
            .map(|binding| binding.id)
    }

    /// The binding named `name` with a recorded read overlapping `path`, if
    /// any. `path` may be shallower than the recorded read (def_use records
    /// reads at whole-statement granularity) or deeper (e.g. a specific
    /// call-argument sub-expression), so both directions of containment
    /// count. The name filter matters because multiple differently-named
    /// bindings read within the same statement (e.g. two pointer arguments
    /// of one call) share that same coarse read path.
    pub(super) fn binding_read_under(
        &self,
        function: FunctionId,
        name: &str,
        path: &AstPath,
    ) -> Option<BindingId> {
        self.def_use
            .iter()
            .find(|fact| {
                fact.function == function
                    && self
                        .bindings
                        .iter()
                        .any(|binding| binding.id == fact.binding && binding.name == name)
                    && fact
                        .reads
                        .iter()
                        .any(|read| walk::paths_overlap(&read.0, &path.0))
            })
            .map(|fact| fact.binding)
    }

    pub(super) fn local_binding_at(
        &self,
        function: FunctionId,
        path: &AstPath,
    ) -> Option<&BindingFact> {
        self.bindings.iter().find(|binding| {
            binding.function == function
                && binding.kind == BindingKind::Local
                && &binding.path == path
        })
    }

    pub(super) fn binding_type(&self, binding: BindingId) -> Option<&str> {
        self.binding_types
            .iter()
            .find(|fact| fact.binding == binding)
            .map(|fact| fact.rendered.as_str())
    }

    pub(super) fn binding_requires_mut(&self, binding: BindingId) -> bool {
        if self
            .borrow_alias
            .iter()
            .find(|fact| fact.binding == binding)
            .is_some_and(|fact| fact.state != BorrowAliasState::ReadOnly)
        {
            return true;
        }
        let Some(binding) = self.bindings.iter().find(|fact| fact.id == binding) else {
            return false;
        };
        self.bindings
            .iter()
            .filter(|other| other.function == binding.function && other.name == binding.name)
            .any(|other| {
                self.borrow_alias
                    .iter()
                    .find(|fact| fact.binding == other.id)
                    .is_some_and(|fact| fact.state != BorrowAliasState::ReadOnly)
            })
    }

    pub(super) fn def_use(&self, binding: BindingId) -> Option<&DefUseFact> {
        self.def_use.iter().find(|fact| fact.binding == binding)
    }

    pub(super) fn effect(
        &self,
        function: FunctionId,
        subject: EffectSubject,
        path: &AstPath,
    ) -> Option<&EffectFact> {
        self.effects
            .iter()
            .find(|fact| fact.function == function && fact.subject == subject && &fact.path == path)
    }

    pub(super) fn control_flow(
        &self,
        function: FunctionId,
        subject: ControlFlowSubject,
        path: &AstPath,
    ) -> Option<&ControlFlowFact> {
        self.control_flow
            .iter()
            .find(|fact| fact.function == function && fact.subject == subject && &fact.path == path)
    }

    pub(super) fn place(&self, function: FunctionId, path: &AstPath) -> Option<&PlaceFact> {
        self.places
            .iter()
            .find(|fact| fact.function == function && &fact.path == path)
    }

    pub(super) fn value(
        &self,
        function: FunctionId,
        subject: ValueSubject,
        path: &AstPath,
    ) -> Option<&ValueFact> {
        self.values
            .iter()
            .find(|fact| fact.function == function && fact.subject == subject && &fact.path == path)
    }

    pub(super) fn has_value(
        &self,
        function: FunctionId,
        subject: ValueSubject,
        path: &AstPath,
        value: &ConstValue,
    ) -> bool {
        self.values.iter().any(|fact| {
            fact.function == function
                && fact.subject == subject
                && &fact.path == path
                && &fact.value == value
        })
    }

    pub(super) fn values_at(
        &self,
        function: FunctionId,
        subject: ValueSubject,
        path: &AstPath,
    ) -> impl Iterator<Item = &ConstValue> {
        self.values
            .iter()
            .filter(move |fact| {
                fact.function == function && fact.subject == subject && &fact.path == path
            })
            .map(|fact| &fact.value)
    }

    pub(super) fn callsite(&self, function: FunctionId, path: &AstPath) -> Option<&CallsiteFact> {
        self.callsites
            .iter()
            .find(|fact| fact.function == function && &fact.path == path)
    }

    pub(super) fn call_arg_at(
        &self,
        function: FunctionId,
        path: &AstPath,
    ) -> Option<(&CallsiteFact, &CallArgFact)> {
        self.callsites
            .iter()
            .filter(|fact| fact.function == function)
            .find_map(|callsite| {
                callsite
                    .args
                    .iter()
                    .find(|arg| &arg.path == path)
                    .map(|arg| (callsite, arg))
            })
    }

    pub(super) fn cast_at(&self, function: FunctionId, path: &AstPath) -> Option<&CastFact> {
        self.casts
            .iter()
            .find(|fact| fact.function == function && &fact.path == path)
    }

    pub(super) fn string_buffer(&self, binding: BindingId) -> Option<&StringBufferFact> {
        self.string_buffers
            .iter()
            .find(|fact| fact.binding == binding)
    }

    pub(super) fn ascii_numeric_string(
        &self,
        binding: BindingId,
    ) -> Option<&AsciiNumericStringFact> {
        self.ascii_numeric_strings
            .iter()
            .find(|fact| fact.binding == binding)
    }

    pub(super) fn string_buffer_at(
        &self,
        function: FunctionId,
        path: &AstPath,
    ) -> Option<&StringBufferFact> {
        self.string_buffers
            .iter()
            .find(|fact| fact.function == function && &fact.path == path)
    }

    pub(super) fn string_pointer_view(
        &self,
        function: FunctionId,
        path: &AstPath,
    ) -> Option<&StringPointerViewFact> {
        self.string_pointer_views
            .iter()
            .find(|fact| fact.function == function && &fact.path == path)
    }

    pub(super) fn string_libc_use(
        &self,
        function: FunctionId,
        path: &AstPath,
    ) -> Option<&StringLibcUseFact> {
        self.string_libc_uses
            .iter()
            .find(|fact| fact.function == function && &fact.path == path)
    }

    pub(super) fn string_lift_plan(
        &self,
        function: FunctionId,
        binding: BindingId,
    ) -> Option<&StringLiftPlanFact> {
        self.string_lift_plans
            .iter()
            .find(|fact| fact.function == function && fact.binding == binding)
    }

    pub(super) fn string_copy_rewrite(
        &self,
        function: FunctionId,
        path: &AstPath,
    ) -> Option<&StringCopyRewriteFact> {
        self.string_copy_rewrites
            .iter()
            .find(|fact| fact.function == function && &fact.path == path)
    }

    pub(super) fn c_string_literal(
        &self,
        function: FunctionId,
        receiver_path: &AstPath,
    ) -> Option<&CStringLiteralFact> {
        self.c_string_literals
            .iter()
            .find(|fact| fact.function == function && &fact.receiver_path == receiver_path)
    }

    pub(super) fn printf_call(
        &self,
        function: FunctionId,
        path: &AstPath,
    ) -> Option<&PrintfCallFact> {
        self.printf_calls
            .iter()
            .find(|fact| fact.function == function && &fact.path == path)
    }

    pub(super) fn file_ownership(&self, handle: BindingId) -> Option<&FileOwnershipFact> {
        self.file_ownership
            .iter()
            .find(|fact| fact.handle == handle)
    }
}

pub(super) fn analyze(program: Program) -> AnalyzedProgram {
    let mut collector = Collector::default();
    collector.program(&program);
    anonymous_structs::collect_facts(&program, &mut collector.facts);
    borrow_alias::collect_facts(&program, &mut collector.facts);
    def_use::collect_facts(&program, &mut collector.facts);
    effects::collect_facts(&program, &mut collector.facts);
    control_flow::collect_facts(&program, &mut collector.facts);
    places::collect_facts(&program, &mut collector.facts);
    retval::collect_facts(&program, &mut collector.facts);
    temp_chains::collect_facts(&program, &mut collector.facts);
    values::collect_facts(&program, &mut collector.facts);
    calls::collect_facts(&program, &mut collector.facts);
    casts::collect_facts(&program, &mut collector.facts);
    strings::collect_facts(&program, &mut collector.facts);
    string_params::collect_facts(&program, &mut collector.facts);
    heap_ownership::collect_facts(&program, &mut collector.facts);
    printf::collect_facts(&program, &mut collector.facts);
    strings::collect_rewrite_facts(&program, &mut collector.facts);
    c_strings::collect_facts(&program, &mut collector.facts);
    file_ownership::collect_facts(&program, &mut collector.facts);
    ptr_len::collect_facts(&program, &mut collector.facts);
    array_element_pointer_origin::collect_facts(&program, &mut collector.facts);
    atomic_locals::collect_facts(&program, &mut collector.facts);
    buffer_cursor::collect_facts(&program, &mut collector.facts);
    slice_index::collect_facts(&program, &mut collector.facts);
    counted_loop::collect_facts(&program, &mut collector.facts);
    loop_shapes::collect_facts(&program, &mut collector.facts);
    va_list::collect_facts(&program, &mut collector.facts);
    AnalyzedProgram {
        program,
        facts: collector.facts,
    }
}

#[derive(Default)]
struct Collector {
    facts: FixupFacts,
}

impl Collector {
    fn program(&mut self, program: &Program) {
        for (item_index, item) in program.items.iter().enumerate() {
            let Item::Fn(f) = item else {
                continue;
            };
            let function = self.push_function(f.name.clone(), item_index);
            for (index, param) in f.params.iter().enumerate() {
                self.push_binding(
                    function,
                    param.name.clone(),
                    BindingKind::Param { index },
                    AstPath::default(),
                    Some(param.ty.clone()),
                );
            }
            self.body(function, &f.body, &mut Vec::new());
        }
    }

    fn push_function(&mut self, name: String, item_index: usize) -> FunctionId {
        let id = FunctionId(self.facts.functions.len());
        self.facts.functions.push(FunctionFact {
            id,
            name,
            item_index,
        });
        id
    }

    fn push_binding(
        &mut self,
        function: FunctionId,
        name: String,
        kind: BindingKind,
        path: AstPath,
        ty: Option<Type>,
    ) -> BindingId {
        let id = BindingId(self.facts.bindings.len());
        self.facts.bindings.push(BindingFact {
            id,
            function,
            name,
            kind,
            path,
        });
        if let Some(ty) = ty {
            self.facts.binding_types.push(BindingTypeFact {
                binding: id,
                rendered: ty.render(),
            });
        }
        id
    }

    fn push_loop(&mut self, function: FunctionId, kind: LoopKind, path: AstPath) -> LoopId {
        let id = LoopId(self.facts.loops.len());
        self.facts.loops.push(LoopFact {
            id,
            function,
            kind,
            path,
        });
        id
    }

    fn body(&mut self, function: FunctionId, body: &[IndentStmt], path: &mut Vec<PathSegment>) {
        for (index, indent) in body.iter().enumerate() {
            path.push(PathSegment::Stmt(index));
            self.stmt(function, &indent.stmt, path);
            path.pop();
        }
    }

    fn block(&mut self, function: FunctionId, block: &Block, path: &mut Vec<PathSegment>) {
        self.body(function, &block.stmts, path);
    }

    fn stmt(&mut self, function: FunctionId, stmt: &Stmt, path: &mut Vec<PathSegment>) {
        match stmt {
            Stmt::Let { name, ty, .. } => {
                self.push_binding(
                    function,
                    name.clone(),
                    BindingKind::Local,
                    AstPath(path.clone()),
                    ty.clone(),
                );
            }
            Stmt::LetIf {
                name,
                ty,
                then_body,
                else_body,
                ..
            } => {
                self.push_binding(
                    function,
                    name.clone(),
                    BindingKind::Local,
                    AstPath(path.clone()),
                    ty.clone(),
                );
                path.push(PathSegment::Then);
                self.body(function, then_body, path);
                path.pop();
                path.push(PathSegment::Else);
                self.body(function, else_body, path);
                path.pop();
            }
            Stmt::If {
                then_body,
                else_body,
                ..
            } => {
                path.push(PathSegment::Then);
                self.body(function, then_body, path);
                path.pop();
                path.push(PathSegment::Else);
                self.body(function, else_body, path);
                path.pop();
            }
            Stmt::Loop { body, .. } => {
                self.push_loop(function, LoopKind::Loop, AstPath(path.clone()));
                path.push(PathSegment::LoopBody);
                self.body(function, body, path);
                path.pop();
            }
            Stmt::For { pat, body, .. } => {
                self.push_loop(function, LoopKind::For, AstPath(path.clone()));
                self.push_binding(
                    function,
                    pat.clone(),
                    BindingKind::Local,
                    AstPath(path.clone()),
                    None,
                );
                path.push(PathSegment::ForBody);
                self.body(function, body, path);
                path.pop();
            }
            Stmt::Scope { body } => {
                path.push(PathSegment::ScopeBody);
                self.body(function, body, path);
                path.pop();
            }
            Stmt::LabeledBlock { body, .. } => {
                path.push(PathSegment::LabeledBody);
                self.body(function, body, path);
                path.pop();
            }
            Stmt::Match { arms, .. } => {
                for (index, arm) in arms.iter().enumerate() {
                    path.push(PathSegment::MatchArm(index));
                    self.body(function, &arm.body, path);
                    path.pop();
                }
            }
            Stmt::Unsafe { body } => {
                path.push(PathSegment::UnsafeBody);
                self.block(function, body, path);
                path.pop();
            }
            Stmt::While { body, .. } => {
                self.push_loop(function, LoopKind::While, AstPath(path.clone()));
                path.push(PathSegment::WhileBody);
                self.block(function, body, path);
                path.pop();
            }
            Stmt::Block(body) => {
                path.push(PathSegment::BlockBody);
                self.block(function, body, path);
                path.pop();
            }
            Stmt::Assign { .. }
            | Stmt::CompoundAssign { .. }
            | Stmt::Expr(_)
            | Stmt::Return(_)
            | Stmt::Break(_)
            | Stmt::Continue(_) => {}
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Block, FnDef, Item, Pattern, Program, Stmt, Visibility};

    fn named(name: &str, stmts: Vec<Stmt>) -> FnDef {
        let mut f = func(vec![param("arg0", "i32")], None, stmts);
        f.name = name.into();
        f
    }

    #[test]
    fn assigns_deterministic_function_and_param_ids() {
        let program = Program {
            items: vec![
                Item::Fn(named("first", Vec::new())),
                Item::Fn(named("second", Vec::new())),
            ],
        };

        let analyzed = analyze(program);

        assert_eq!(
            analyzed
                .facts
                .functions
                .iter()
                .map(|f| (f.id, f.name.as_str(), f.item_index))
                .collect::<Vec<_>>(),
            vec![(FunctionId(0), "first", 0), (FunctionId(1), "second", 1)]
        );
        assert_eq!(
            analyzed
                .facts
                .bindings
                .iter()
                .map(|b| (b.id, b.function, b.name.as_str(), b.kind.clone()))
                .collect::<Vec<_>>(),
            vec![
                (
                    BindingId(0),
                    FunctionId(0),
                    "arg0",
                    BindingKind::Param { index: 0 }
                ),
                (
                    BindingId(1),
                    FunctionId(1),
                    "arg0",
                    BindingKind::Param { index: 0 }
                )
            ]
        );
    }

    #[test]
    fn records_nested_local_paths_and_loop_ids() {
        let program = Program {
            items: vec![Item::Fn(named(
                "f",
                vec![
                    let_mut("outer", "i32", int(0)),
                    Stmt::Scope {
                        body: vec![IndentStmt {
                            depth: 2,
                            stmt: Stmt::Loop {
                                label: None,
                                body: vec![IndentStmt {
                                    depth: 3,
                                    stmt: let_mut("inner", "i32", int(1)),
                                }],
                            },
                        }],
                    },
                    Stmt::While {
                        cond: var("outer"),
                        body: Block {
                            stmts: vec![IndentStmt {
                                depth: 2,
                                stmt: let_mut("while_local", "i32", int(2)),
                            }],
                            tail: None,
                        },
                    },
                ],
            ))],
        };

        let analyzed = analyze(program);

        assert_eq!(
            analyzed
                .facts
                .bindings
                .iter()
                .map(|b| (b.name.as_str(), b.path.clone()))
                .collect::<Vec<_>>(),
            vec![
                ("arg0", AstPath::default()),
                ("outer", AstPath(vec![PathSegment::Stmt(0)])),
                (
                    "inner",
                    AstPath(vec![
                        PathSegment::Stmt(1),
                        PathSegment::ScopeBody,
                        PathSegment::Stmt(0),
                        PathSegment::LoopBody,
                        PathSegment::Stmt(0)
                    ])
                ),
                (
                    "while_local",
                    AstPath(vec![
                        PathSegment::Stmt(2),
                        PathSegment::WhileBody,
                        PathSegment::Stmt(0)
                    ])
                )
            ]
        );
        assert_eq!(
            analyzed
                .facts
                .loops
                .iter()
                .map(|l| (l.id, l.kind.clone(), l.path.clone()))
                .collect::<Vec<_>>(),
            vec![
                (
                    LoopId(0),
                    LoopKind::Loop,
                    AstPath(vec![
                        PathSegment::Stmt(1),
                        PathSegment::ScopeBody,
                        PathSegment::Stmt(0)
                    ])
                ),
                (
                    LoopId(1),
                    LoopKind::While,
                    AstPath(vec![PathSegment::Stmt(2)])
                )
            ]
        );
    }

    #[test]
    fn records_branch_and_match_local_paths() {
        let program = Program {
            items: vec![Item::Fn(FnDef {
                attrs: Vec::new(),
                vis: Visibility::Private,
                unsafe_: false,
                abi: None,
                name: "f".into(),
                params: Vec::new(),
                ret: None,
                body: vec![IndentStmt {
                    depth: 1,
                    stmt: Stmt::Match {
                        expr: var("x"),
                        arms: vec![crate::rust_ast::MatchArm {
                            pattern: Pattern::Wildcard,
                            body: vec![IndentStmt {
                                depth: 2,
                                stmt: Stmt::LetIf {
                                    name: "choice".into(),
                                    mutable: false,
                                    ty: None,
                                    cond: var("x"),
                                    then_body: vec![IndentStmt {
                                        depth: 3,
                                        stmt: let_mut("then_local", "i32", int(1)),
                                    }],
                                    then_value: int(1),
                                    else_body: vec![IndentStmt {
                                        depth: 3,
                                        stmt: let_mut("else_local", "i32", int(2)),
                                    }],
                                    else_value: int(2),
                                },
                            }],
                        }],
                    },
                }],
            })],
        };

        let analyzed = analyze(program);

        assert_eq!(
            analyzed
                .facts
                .bindings
                .iter()
                .map(|b| (b.name.as_str(), b.path.clone()))
                .collect::<Vec<_>>(),
            vec![
                (
                    "choice",
                    AstPath(vec![
                        PathSegment::Stmt(0),
                        PathSegment::MatchArm(0),
                        PathSegment::Stmt(0)
                    ])
                ),
                (
                    "then_local",
                    AstPath(vec![
                        PathSegment::Stmt(0),
                        PathSegment::MatchArm(0),
                        PathSegment::Stmt(0),
                        PathSegment::Then,
                        PathSegment::Stmt(0)
                    ])
                ),
                (
                    "else_local",
                    AstPath(vec![
                        PathSegment::Stmt(0),
                        PathSegment::MatchArm(0),
                        PathSegment::Stmt(0),
                        PathSegment::Else,
                        PathSegment::Stmt(0)
                    ])
                )
            ]
        );
    }

    #[test]
    fn analysis_preserves_program_output() {
        let program = Program {
            items: vec![Item::Fn(named(
                "f",
                vec![let_mut("x", "i32", int(0)), Stmt::Return(Some(var("x")))],
            ))],
        };
        let before = program.emit();

        let analyzed = analyze(program);

        assert_eq!(analyzed.program.emit(), before);
        assert!(analyzed.facts.relations.is_empty());
    }
}
