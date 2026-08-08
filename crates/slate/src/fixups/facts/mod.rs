use std::collections::BTreeSet;

use crate::rust_ast::{AtomicType, Expr, Type};
use ordered_float::OrderedFloat;

pub(super) mod anonymous_structs;
pub(super) mod array_element_pointer_origin;
pub(super) mod atomic_locals;
pub(super) mod borrow_alias;
pub(super) mod buffer_cursor;
pub(super) mod c_strings;
pub(super) mod callee_alloc_summary;
pub(super) mod calls;
pub(super) mod casts;
pub(super) mod control_flow;
pub(super) mod counted_loop;
pub(super) mod def_use;
pub(super) mod effects;
pub(super) mod file_ownership;
pub(crate) mod goto;
pub(super) mod heap_ownership;
pub(super) mod interprocedural_alloc_eligibility;
pub(super) mod lazy_singleton;
pub(super) mod null_check_dominance;
pub(super) mod option_box_locals;
pub(super) mod places;
pub(super) mod pointer_option_safety;
pub(super) mod printf;
pub(super) mod ptr_len;
pub(super) mod string_params;
pub(super) mod strings;
pub(super) mod struct_field_ownership;
pub(super) mod values;
pub(super) mod walk;

pub(super) const GENERATED_C_STRING_READ_CALLEE: &str = "std::ffi::CStr::from_ptr";

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct FunctionId(pub(super) usize);

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct BindingId(pub(super) usize);

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct LoopId(pub(super) usize);

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct SignatureId(pub(super) usize);

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct Site {
    pub(super) function: FunctionId,
    pub(super) path: AstPath,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct FunctionFact {
    pub(super) id: FunctionId,
    pub(super) name: String,
    pub(super) item_index: usize,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct BindingFact {
    pub(super) id: BindingId,
    pub(super) function: FunctionId,
    pub(super) name: String,
    pub(super) kind: BindingKind,
    pub(super) path: AstPath,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct BindingTypeFact {
    pub(super) binding: BindingId,
    pub(super) ty: Type,
    pub(super) rendered: String,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct AtomicLocalFact {
    pub(super) function: FunctionId,
    pub(super) name: String,
    pub(super) ty: AtomicType,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct AtomicGlobalFact {
    pub(super) name: String,
    pub(super) ty: AtomicType,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct LazyInitSingletonFact {
    pub(super) function: FunctionId,
    pub(super) flag_name: String,
    pub(super) payload_name: String,
    pub(super) payload_ty: Type,
    pub(super) init_expr: Expr,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct StaticDeclFact {
    pub(super) name: String,
    pub(super) mutable: bool,
    pub(super) ty: Type,
    pub(super) init: Expr,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct AnonymousStructFact {
    pub(super) original_name: String,
    pub(super) generated_name: String,
    pub(super) fields: Vec<AnonymousStructFieldFact>,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct AnonymousStructFieldFact {
    pub(super) name: String,
    pub(super) ty: Type,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct BufferPointerFieldFact {
    pub(super) site: Site,
    pub(super) buffer: BindingId,
    pub(super) array: BindingId,
    pub(super) field: String,
    pub(super) index: usize,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum BindingKind {
    Param { index: usize },
    Local,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct LoopFact {
    pub(super) id: LoopId,
    pub(super) function: FunctionId,
    pub(super) kind: LoopKind,
    pub(super) path: AstPath,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct BorrowAliasFact {
    pub(super) function: FunctionId,
    pub(super) binding: BindingId,
    pub(super) state: BorrowAliasState,
    pub(super) reasons: BTreeSet<BorrowAliasReason>,
    pub(super) uses: Vec<BorrowAliasUseFact>,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct BorrowAliasUseFact {
    pub(super) kind: BorrowAliasUseKind,
    pub(super) path: AstPath,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct DefUseFact {
    pub(super) function: FunctionId,
    pub(super) binding: BindingId,
    pub(super) definition: AstPath,
    pub(super) reads: Vec<AstPath>,
    pub(super) writes: Vec<AstPath>,
    pub(super) last_use: Option<AstPath>,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct EffectFact {
    pub(super) site: Site,
    pub(super) subject: EffectSubject,
    pub(super) purity: Purity,
    pub(super) effects: BTreeSet<EffectKind>,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct ControlFlowFact {
    pub(super) site: Site,
    pub(super) subject: ControlFlowSubject,
    pub(super) reachable: bool,
    pub(super) falls_through: bool,
    pub(super) exits: BTreeSet<ControlFlowExit>,
    pub(super) single_exit: bool,
    pub(super) has_unreachable_tail: bool,
    pub(super) expression_eligible: bool,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct CastFact {
    pub(super) site: Site,
    pub(super) from: Option<Type>,
    pub(super) to: Type,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct NullCheckDominanceFact {
    pub(super) function: FunctionId,
    pub(super) binding: BindingId,
    pub(super) deref_site: Site,
    pub(super) guard_site: Option<Site>,
    pub(super) proof: NullCheckProof,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum NullCheckProof {
    StructuredGuard,
    GuardClauseExit,
    ConstructionNonNull,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct PointerOptionSafetyFact {
    pub(super) function: FunctionId,
    pub(super) binding: BindingId,
    pub(super) eligible: bool,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct PointerComparisonFact {
    pub(super) site: Site,
    pub(super) kind: PointerComparisonKind,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum PointerComparisonKind {
    NullCompare,
    IdentityCompare,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct StructFieldOwnershipFact {
    pub(super) record_name: String,
    pub(super) field_name: String,
    pub(super) tree_eligible: bool,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct OptionBoxLocalCandidate {
    pub(super) function: FunctionId,
    pub(super) binding: BindingId,
    pub(super) name: String,
    pub(super) elem_ty: Type,
    pub(super) decl_path: AstPath,
    pub(super) assignments: Vec<OptionBoxAssignment>,
    pub(super) deref_paths: Vec<AstPath>,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct OptionBoxAssignment {
    pub(super) path: AstPath,
    pub(super) kind: OptionBoxAssignKind,
    pub(super) alloc_source: Option<AstPath>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum OptionBoxAssignKind {
    Null,
    Alloc,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct OptionBoxComparison {
    pub(super) function: FunctionId,
    pub(super) if_stmt_path: AstPath,
    pub(super) lhs: String,
    pub(super) rhs: String,
    pub(super) negate: bool,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct PlaceFact {
    pub(super) site: Site,
    pub(super) access: PlaceAccess,
    pub(super) kind: PlaceKind,
    pub(super) readable: bool,
    pub(super) assignable: bool,
    pub(super) ordinary_slot: bool,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct ValueFact {
    pub(super) site: Site,
    pub(super) subject: ValueSubject,
    pub(super) value: ConstValue,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct CallSignatureFact {
    pub(super) id: SignatureId,
    pub(super) name: String,
    pub(super) source: CallSignatureSource,
    pub(super) params: Vec<CallParamFact>,
    pub(super) variadic: bool,
    pub(super) ret: Option<Type>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum CallSignatureSource {
    Function(FunctionId),
    Extern {
        item_index: usize,
        decl_index: usize,
    },
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct CallParamFact {
    pub(super) ty: Type,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct CallsiteFact {
    pub(super) site: Site,
    pub(super) callee: CallCallee,
    pub(super) args: Vec<CallArgFact>,
    pub(super) ret: Option<Type>,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum CallCallee {
    Direct {
        name: String,
        signature: Option<CallSignatureFact>,
        identity: crate::function_identity::FunctionIdentity,
    },
    Indirect,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct CallArgFact {
    pub(super) slot: usize,
    pub(super) path: AstPath,
    pub(super) declared_ty: Option<Type>,
    pub(super) variadic: bool,
    pub(super) pinning: CallArgPinning,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum CallArgPinning {
    DeclaredParam,
    VariadicUnpinned,
    UnknownCallee,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum ValueSubject {
    Expr,
    Binding(BindingId),
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
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

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct StringBufferFact {
    pub(super) site: Site,
    pub(super) binding: BindingId,
    pub(super) kind: StringBufferKind,
    pub(super) provenance: StringBufferProvenance,
    pub(super) bytes: Option<Vec<u8>>,
    pub(super) nul_termination: NulTermination,
    pub(super) interior_nul: bool,
    pub(super) ascii_only: bool,
    pub(super) candidates: BTreeSet<StringRecoveryCandidate>,
    pub(super) rejections: BTreeSet<StringBufferRejection>,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct AsciiNumericStringFact {
    pub(super) site: Site,
    pub(super) binding: BindingId,
    pub(super) sign: AsciiNumericSign,
    pub(super) digits: usize,
}

#[derive(Debug, Clone, Copy, Default, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum AsciiNumericSign {
    #[default]
    None,
    Plus,
    Minus,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum StringBufferKind {
    CharArray,
    BorrowedStr,
    BorrowedCStr,
    BorrowedBytes,
    OwnedString,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum StringBufferProvenance {
    Literal,
    ZeroInitialized,
    AssignedLiteral { assignment: AstPath },
    Lifted,
    Unknown,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum NulTermination {
    NotApplicable,
    Unterminated,
    Terminated,
    AllZero,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum StringRecoveryCandidate {
    BorrowedStr,
    BorrowedCStr,
    BorrowedBytes,
    OwnedString,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum StringBufferRejection {
    Indexed,
    Mutated,
    UnsupportedInitializer,
    Unterminated,
    EscapedToCall,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct StringPointerViewFact {
    pub(super) site: Site,
    pub(super) source: BindingId,
    pub(super) mutable: bool,
    pub(super) kind: StringPointerViewKind,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum StringPointerViewKind {
    As,
    AsMut,
    Array,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct StringLibcUseFact {
    pub(super) site: Site,
    pub(super) callee: StringLibcFunction,
    pub(super) pointer_args: Vec<BindingId>,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct StringLiftPlanFact {
    pub(super) site: Site,
    pub(super) binding: BindingId,
    pub(super) recovery: StringRecoveryCandidate,
    pub(super) remove_assignment: Option<AstPath>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct StringParamLiftFact {
    pub(super) callee: FunctionId,
    pub(super) param: BindingId,
    pub(super) index: usize,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct StringCopyRewriteFact {
    pub(super) site: Site,
    pub(super) dst: BindingId,
    pub(super) rewrite: StringCopyRewrite,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct CStringLiteralFact {
    pub(super) function: FunctionId,
    pub(super) receiver_path: AstPath,
    pub(super) bytes: Vec<u8>,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
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

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum FileOpenMode {
    Read,
    Write,
    Append,
    ReadUpdate,
    WriteUpdate,
    AppendUpdate,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct FileUseFact {
    pub(super) path: AstPath,
    pub(super) kind: FileUseKind,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum FileUseKind {
    Read,
    Write,
    Gets,
    Puts,
    Close,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum StringCopyRewrite {
    AssignLiteral(String),
    AssignOwned(BindingId),
    PushLiteral(String),
    PushOwned(BindingId),
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct HeapOwnershipFact {
    pub(super) function: FunctionId,
    pub(super) pointer: BindingId,
    pub(super) allocation_temp: BindingId,
    pub(super) size_temp: Option<BindingId>,
    pub(super) free_temp: Option<BindingId>,
    pub(super) aliases: Vec<BindingId>,
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
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum HeapOwnershipKind {
    ScalarBox,
    VecBuffer,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum AllocProvenance {
    Direct {
        elem_ty: Type,
        allocation: HeapAllocationKind,
        extent: HeapExtent,
        init: HeapInitKind,
        return_path: AstPath,
        alloc_source_path: AstPath,
    },
    PassThrough {
        callees: Vec<String>,
    },
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct CalleeAllocSummaryFact {
    pub(super) function: FunctionId,
    pub(super) provenance: AllocProvenance,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct InterproceduralAllocEligibilityFact {
    pub(super) function: FunctionId,
    pub(super) elem_ty: Type,
    pub(super) allocation: HeapAllocationKind,
    pub(super) extent: HeapExtent,
    pub(super) init: HeapInitKind,
    pub(super) eligible: bool,
    pub(super) chain: Vec<FunctionId>,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct InterproceduralAllocCallerFact {
    pub(super) callee: FunctionId,
    pub(super) caller: FunctionId,
    pub(super) pointer_name: String,
    pub(super) decl_path: AstPath,
    pub(super) call_temp_path: AstPath,
    pub(super) free_path: Option<AstPath>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum HeapAllocationKind {
    Malloc,
    Calloc,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum HeapExtent {
    Scalar,
    Elements { count: Expr },
    Unknown,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum HeapInitKind {
    Uninitialized,
    Zeroed,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum HeapReadSafety {
    ZeroInitialized,
    ReadsAfterWrites,
    MayReadUninitialized,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct HeapUseFact {
    pub(super) path: AstPath,
    pub(super) kind: HeapUseKind,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum HeapUseKind {
    ScalarRead,
    ScalarWrite,
    IndexedRead { index: Expr },
    IndexedWrite { index: Expr },
    Free,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
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

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum HeapResizeKind {
    Grow,
    Shrink,
    SameOrUnknown,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct PrintfCallFact {
    pub(super) site: Site,
    pub(super) format: Option<Vec<u8>>,
    pub(super) arg_paths: Vec<AstPath>,
    pub(super) arg_facts: Vec<PrintfArgFact>,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct PrintfArgFact {
    pub(super) path: AstPath,
    pub(super) const_string: Option<String>,
    pub(super) const_char: Option<String>,
    pub(super) const_float: Option<OrderedFloat<f64>>,
    pub(super) rust_string: bool,
    pub(super) pointer: bool,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
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

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
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

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum PlaceRoot {
    Local { name: String },
    Unsupported,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum PlaceProjection {
    Deref,
    Field(String),
    TupleField(usize),
    Index,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum PlaceAccess {
    Read,
    Write,
    ReadWrite,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum VolatileAccess {
    Read,
    Write,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum AtomicPlaceAccess {
    Read,
    Write,
    ReadWrite,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct PtrLenSliceFact {
    pub(super) caller: FunctionId,
    pub(super) callee: FunctionId,
    pub(super) ptr_param: BindingId,
    pub(super) len_param: Option<BindingId>,
    pub(super) mutable: bool,
    pub(super) elem_ty: Type,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct ArrayElementPointerOriginFact {
    pub(super) site: Site,
    pub(super) pointer: BindingId,
    pub(super) base: BindingId,
    pub(super) index: Expr,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct LoopSite {
    pub(super) function: FunctionId,
    pub(super) loop_id: LoopId,
    pub(super) loop_path: AstPath,
    pub(super) body_path: AstPath,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct CountedLoopFact {
    pub(super) site: LoopSite,
    pub(super) bound: Expr,
    pub(super) start: CountedLoopStart,
    pub(super) step: CountedLoopStep,
    pub(super) index_use: CountedLoopIndexUse,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct CountedSliceLoopFact {
    pub(super) site: LoopSite,
    pub(super) index: BindingId,
    pub(super) slice: BindingId,
    pub(super) start: CountedLoopStart,
    pub(super) bound: CountedLoopBound,
    pub(super) step: CountedLoopStep,
    pub(super) index_use: CountedLoopIndexUse,
    pub(super) access: SliceLoopAccess,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum CountedLoopStart {
    Zero,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum CountedLoopBound {
    SliceLen,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum CountedLoopStep {
    One,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum CountedLoopIndexUse {
    Unused,
    Other,
    SliceIndexOnly,
    SliceIndexAndValue,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum SliceLoopAccess {
    ReadOnly,
    Mutable,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum EffectSubject {
    Expr,
    Stmt,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum ControlFlowSubject {
    Body,
    Block,
    Stmt,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum ControlFlowExit {
    Return,
    Break(Option<String>),
    Continue(Option<String>),
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum Purity {
    MovablePure,
    ReadOnly,
    Effectful,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
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

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum BorrowAliasState {
    ReadOnly,
    UniqueMutation,
    Escaped,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
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

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
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

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) enum LoopKind {
    Loop,
    While,
    For,
}

#[derive(Debug, Default, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct AstPath(pub(super) Vec<PathSegment>);

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
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

pub(in crate::fixups) fn def_use_query_path(path: &AstPath) -> AstPath {
    AstPath(
        path.0
            .iter()
            .filter(|segment| !matches!(segment, PathSegment::Expr(_)))
            .cloned()
            .collect(),
    )
}

pub(in crate::fixups) fn binding_by_param_index(
    bindings: &[BindingFact],
    function: FunctionId,
    index: usize,
) -> Option<BindingId> {
    bindings
        .iter()
        .find(|binding| {
            binding.function == function
                && matches!(binding.kind, BindingKind::Param { index: i } if i == index)
        })
        .map(|binding| binding.id)
}

pub(in crate::fixups) fn binding_by_local_path(
    bindings: &[BindingFact],
    function: FunctionId,
    name: &str,
    path: &AstPath,
) -> Option<BindingId> {
    bindings
        .iter()
        .find(|binding| {
            binding.function == function
                && binding.name == name
                && binding.kind == BindingKind::Local
                && &binding.path == path
        })
        .map(|binding| binding.id)
}

pub(in crate::fixups) fn binding_named(
    bindings: &[BindingFact],
    function: FunctionId,
    name: &str,
) -> Option<BindingId> {
    bindings
        .iter()
        .rev()
        .find(|binding| binding.function == function && binding.name == name)
        .map(|binding| binding.id)
}

pub(in crate::fixups) fn binding_type(
    binding_types: &[BindingTypeFact],
    binding: BindingId,
) -> Option<&str> {
    binding_types
        .iter()
        .find(|fact| fact.binding == binding)
        .map(|fact| fact.rendered.as_str())
}

pub(in crate::fixups) fn binding_type_ast(
    binding_types: &[BindingTypeFact],
    binding: BindingId,
) -> Option<&Type> {
    binding_types
        .iter()
        .find(|fact| fact.binding == binding)
        .map(|fact| &fact.ty)
}

pub(in crate::fixups) fn def_use_of(
    def_use: &[DefUseFact],
    binding: BindingId,
) -> Option<&DefUseFact> {
    def_use.iter().find(|fact| fact.binding == binding)
}

pub(in crate::fixups) fn string_buffer(
    string_buffers: &[StringBufferFact],
    binding: BindingId,
) -> Option<&StringBufferFact> {
    string_buffers.iter().find(|fact| fact.binding == binding)
}

pub(in crate::fixups) fn string_pointer_view<'a>(
    string_pointer_views: &'a [StringPointerViewFact],
    function: FunctionId,
    path: &AstPath,
) -> Option<&'a StringPointerViewFact> {
    string_pointer_views
        .iter()
        .find(|fact| fact.site.function == function && &fact.site.path == path)
}

/// The binding named `name` with a recorded read overlapping `path`, if
/// any. `path` may be shallower than the recorded read (def_use records
/// reads at whole-statement granularity) or deeper (e.g. a specific
/// call-argument sub-expression), so both directions of containment
/// count. The name filter matters because multiple differently-named
/// bindings read within the same statement (e.g. two pointer arguments
/// of one call) share that same coarse read path.
pub(in crate::fixups) fn binding_read_under(
    def_use: &[DefUseFact],
    bindings: &[BindingFact],
    function: FunctionId,
    name: &str,
    path: &AstPath,
) -> Option<BindingId> {
    let path = def_use_query_path(path);
    def_use
        .iter()
        .find(|fact| {
            fact.function == function
                && bindings
                    .iter()
                    .any(|binding| binding.id == fact.binding && binding.name == name)
                && fact
                    .reads
                    .iter()
                    .any(|read| walk::paths_overlap(&read.0, &path.0))
        })
        .map(|fact| fact.binding)
}

pub(in crate::fixups) fn local_binding_at<'a>(
    bindings: &'a [BindingFact],
    function: FunctionId,
    path: &AstPath,
) -> Option<&'a BindingFact> {
    bindings.iter().find(|binding| {
        binding.function == function && binding.kind == BindingKind::Local && &binding.path == path
    })
}

pub(in crate::fixups) fn binding_name(
    bindings: &[BindingFact],
    binding: BindingId,
) -> Option<&str> {
    bindings
        .iter()
        .find(|fact| fact.id == binding)
        .map(|fact| fact.name.as_str())
}

pub(in crate::fixups) fn string_buffer_at<'a>(
    string_buffers: &'a [StringBufferFact],
    function: FunctionId,
    path: &AstPath,
) -> Option<&'a StringBufferFact> {
    string_buffers
        .iter()
        .find(|fact| fact.site.function == function && &fact.site.path == path)
}

pub(in crate::fixups) fn string_libc_use<'a>(
    string_libc_uses: &'a [StringLibcUseFact],
    function: FunctionId,
    path: &AstPath,
) -> Option<&'a StringLibcUseFact> {
    string_libc_uses
        .iter()
        .find(|fact| fact.site.function == function && &fact.site.path == path)
}
