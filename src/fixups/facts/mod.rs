use std::collections::BTreeSet;

use crate::rust_ast::{AtomicType, Block, Expr, FnDef, IndentStmt, Item, Program, Stmt, Type};
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

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct AnalyzedProgram<'a> {
    pub(super) program: &'a Program,
    pub(super) facts: FixupFacts,
}

#[derive(Debug, Clone, Default, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(super) struct FixupFacts {
    pub(super) functions: Vec<FunctionFact>,
    pub(super) bindings: Vec<BindingFact>,
    pub(super) binding_types: Vec<BindingTypeFact>,
    pub(super) loops: Vec<LoopFact>,
    pub(super) borrow_alias: Vec<BorrowAliasFact>,
    pub(super) def_use: Vec<DefUseFact>,
    pub(super) effects: Vec<EffectFact>,
    pub(super) control_flow: Vec<ControlFlowFact>,
    pub(super) null_check_dominance: Vec<NullCheckDominanceFact>,
    pub(super) pointer_option_safety: Vec<PointerOptionSafetyFact>,
    pub(super) pointer_comparisons: Vec<PointerComparisonFact>,
    pub(super) struct_field_ownership: Vec<StructFieldOwnershipFact>,
    pub(super) option_box_locals: Vec<OptionBoxLocalCandidate>,
    pub(super) option_box_comparisons: Vec<OptionBoxComparison>,
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
    pub(super) callee_alloc_summaries: Vec<CalleeAllocSummaryFact>,
    pub(super) interprocedural_alloc_eligibility: Vec<InterproceduralAllocEligibilityFact>,
    pub(super) interprocedural_alloc_callers: Vec<InterproceduralAllocCallerFact>,
    pub(super) printf_calls: Vec<PrintfCallFact>,
    pub(super) ptr_len_slices: Vec<PtrLenSliceFact>,
    pub(super) array_element_pointer_origins: Vec<ArrayElementPointerOriginFact>,
    pub(super) buffer_pointer_fields: Vec<BufferPointerFieldFact>,
    pub(super) anonymous_structs: Vec<AnonymousStructFact>,
    pub(super) atomic_locals: Vec<AtomicLocalFact>,
    pub(super) atomic_globals: Vec<AtomicGlobalFact>,
    pub(super) lazy_init_singletons: Vec<LazyInitSingletonFact>,
    pub(super) counted_loops: Vec<CountedLoopFact>,
    pub(super) counted_slice_loops: Vec<CountedSliceLoopFact>,
}

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

fn def_use_query_path(path: &AstPath) -> AstPath {
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

    pub(super) fn function_name(&self, function: FunctionId) -> Option<&str> {
        self.functions
            .iter()
            .find(|fact| fact.id == function)
            .map(|fact| fact.name.as_str())
    }

    pub(super) fn splice_function(&mut self, program: &Program, function: FunctionId) {
        let Some(item_index) = self.function_item_index(function) else {
            return;
        };
        let Some(Item::Fn(f)) = program.items.get(item_index) else {
            return;
        };
        self.purge_function_facts(function);
        // Bindings/binding types/loops first - everything below resolves
        // bindings by name+path against `self.bindings`, so it must
        // already reflect this function's current body before they run.
        Collector::resume(self).function(function, f);
        self.borrow_alias.extend(borrow_alias::collect_for_function(
            function,
            f,
            &self.bindings,
        ));
        self.def_use
            .extend(def_use::collect_for_function(function, f, &self.bindings));
        self.effects
            .extend(effects::collect_for_function(function, f));
        self.values
            .extend(values::collect_for_function(function, f, &self.bindings));
        let collected =
            strings::collect_for_function(function, f, &self.bindings, &self.binding_types);
        self.string_buffers.extend(collected.buffers);
        self.string_pointer_views.extend(collected.pointer_views);
        self.string_libc_uses.extend(collected.libc_uses);
        let (counted, sliced) =
            counted_loop::collect_for_function(function, f, &self.bindings, &self.loops);
        self.counted_loops.extend(counted);
        self.counted_slice_loops.extend(sliced);
    }

    pub(super) fn remove_items(&mut self, item_indices: &[usize]) {
        let mut sorted = item_indices.to_vec();
        sorted.sort_unstable();
        sorted.dedup();
        for item_index in sorted.into_iter().rev() {
            self.remove_item(item_index);
        }
    }

    fn remove_item(&mut self, item_index: usize) {
        if let Some(function) = self.function_by_item_index(item_index) {
            self.purge_function_facts(function);
            self.functions.retain(|fact| fact.id != function);
        }
        for fact in &mut self.functions {
            if fact.item_index > item_index {
                fact.item_index -= 1;
            }
        }
    }

    fn purge_function_facts(&mut self, function: FunctionId) {
        let stale_bindings = self
            .bindings
            .iter()
            .filter(|binding| binding.function == function)
            .map(|binding| binding.id)
            .collect::<BTreeSet<_>>();
        self.bindings.retain(|binding| binding.function != function);
        self.binding_types
            .retain(|binding_type| !stale_bindings.contains(&binding_type.binding));
        self.loops
            .retain(|loop_fact| loop_fact.function != function);
        self.borrow_alias.retain(|fact| fact.function != function);
        self.def_use.retain(|fact| fact.function != function);
        self.effects
            .retain(|effect| effect.site.function != function);
        self.values.retain(|value| value.site.function != function);
        self.string_buffers
            .retain(|buffer| !stale_bindings.contains(&buffer.binding));
        self.string_pointer_views
            .retain(|view| view.site.function != function);
        self.string_libc_uses
            .retain(|use_fact| use_fact.site.function != function);
        self.counted_loops
            .retain(|loop_fact| loop_fact.site.function != function);
        self.counted_slice_loops
            .retain(|loop_fact| loop_fact.site.function != function);
    }

    pub(super) fn binding_by_local_path(
        &self,
        function: FunctionId,
        name: &str,
        path: &AstPath,
    ) -> Option<BindingId> {
        binding_by_local_path(&self.bindings, function, name, path)
    }

    pub(super) fn binding_name(&self, binding: BindingId) -> Option<&str> {
        self.bindings
            .iter()
            .find(|fact| fact.id == binding)
            .map(|fact| fact.name.as_str())
    }

    pub(super) fn bindings_read_under(
        &self,
        function: FunctionId,
        name: &str,
        path: &AstPath,
    ) -> Vec<BindingId> {
        let path = def_use_query_path(path);
        self.def_use
            .iter()
            .filter(|fact| {
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
            .collect()
    }

    pub(super) fn bindings_written_under(
        &self,
        function: FunctionId,
        name: &str,
        path: &AstPath,
    ) -> Vec<BindingId> {
        let path = def_use_query_path(path);
        self.def_use
            .iter()
            .filter(|fact| {
                fact.function == function
                    && self
                        .bindings
                        .iter()
                        .any(|binding| binding.id == fact.binding && binding.name == name)
                    && fact
                        .writes
                        .iter()
                        .any(|write| walk::paths_overlap(&write.0, &path.0))
            })
            .map(|fact| fact.binding)
            .collect()
    }

    pub(super) fn binding_type(&self, binding: BindingId) -> Option<&str> {
        binding_type(&self.binding_types, binding)
    }

    pub(super) fn binding_type_ast(&self, binding: BindingId) -> Option<&Type> {
        binding_type_ast(&self.binding_types, binding)
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
        def_use_of(&self.def_use, binding)
    }

    pub(super) fn effect(
        &self,
        function: FunctionId,
        subject: EffectSubject,
        path: &AstPath,
    ) -> Option<&EffectFact> {
        self.effects.iter().find(|fact| {
            fact.site.function == function && fact.subject == subject && &fact.site.path == path
        })
    }

    pub(super) fn control_flow(
        &self,
        function: FunctionId,
        subject: ControlFlowSubject,
        path: &AstPath,
    ) -> Option<&ControlFlowFact> {
        self.control_flow.iter().find(|fact| {
            fact.site.function == function && fact.subject == subject && &fact.site.path == path
        })
    }

    pub(super) fn place(&self, function: FunctionId, path: &AstPath) -> Option<&PlaceFact> {
        self.places
            .iter()
            .find(|fact| fact.site.function == function && &fact.site.path == path)
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
                fact.site.function == function && fact.subject == subject && &fact.site.path == path
            })
            .map(|fact| &fact.value)
    }

    pub(super) fn callsite(&self, function: FunctionId, path: &AstPath) -> Option<&CallsiteFact> {
        self.callsites
            .iter()
            .find(|fact| fact.site.function == function && &fact.site.path == path)
    }

    pub(super) fn call_arg_at(
        &self,
        function: FunctionId,
        path: &AstPath,
    ) -> Option<(&CallsiteFact, &CallArgFact)> {
        self.callsites
            .iter()
            .filter(|fact| fact.site.function == function)
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
            .find(|fact| fact.site.function == function && &fact.site.path == path)
    }

    pub(super) fn string_buffer(&self, binding: BindingId) -> Option<&StringBufferFact> {
        string_buffer(&self.string_buffers, binding)
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
            .find(|fact| fact.site.function == function && &fact.site.path == path)
    }

    pub(super) fn string_pointer_view(
        &self,
        function: FunctionId,
        path: &AstPath,
    ) -> Option<&StringPointerViewFact> {
        string_pointer_view(&self.string_pointer_views, function, path)
    }

    pub(super) fn string_use_allowed(
        &self,
        function: FunctionId,
        use_path: &AstPath,
        binding: BindingId,
        recovery: StringRecoveryCandidate,
        liftable: &BTreeSet<BindingId>,
    ) -> bool {
        strings::use_allowed(function, use_path, self, binding, recovery, liftable)
    }

    pub(super) fn liftable_string_bindings(
        &self,
        function: FunctionId,
        recovery: StringRecoveryCandidate,
    ) -> BTreeSet<BindingId> {
        self.string_lift_plans
            .iter()
            .filter(|plan| plan.site.function == function && plan.recovery == recovery)
            .map(|plan| plan.binding)
            .collect()
    }

    pub(super) fn string_libc_use(
        &self,
        function: FunctionId,
        path: &AstPath,
    ) -> Option<&StringLibcUseFact> {
        self.string_libc_uses
            .iter()
            .find(|fact| fact.site.function == function && &fact.site.path == path)
    }

    pub(super) fn null_check_dominance_at(
        &self,
        function: FunctionId,
        deref_path: &AstPath,
    ) -> Option<&NullCheckDominanceFact> {
        self.null_check_dominance.iter().find(|fact| {
            fact.deref_site.function == function
                && walk::paths_overlap(&fact.deref_site.path.0, &deref_path.0)
        })
    }

    pub(super) fn pointer_option_safety_of(
        &self,
        function: FunctionId,
        binding: BindingId,
    ) -> Option<&PointerOptionSafetyFact> {
        self.pointer_option_safety
            .iter()
            .find(|fact| fact.function == function && fact.binding == binding)
    }

    #[expect(
        dead_code,
        reason = "query API surface not yet wired into a fixup rule"
    )]
    pub(super) fn pointer_comparison_at(
        &self,
        function: FunctionId,
        path: &AstPath,
    ) -> Option<&PointerComparisonFact> {
        self.pointer_comparisons
            .iter()
            .find(|fact| fact.site.function == function && &fact.site.path == path)
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
            .find(|fact| fact.site.function == function && &fact.site.path == path)
    }
}

pub(super) fn analyze(program: &Program) -> AnalyzedProgram<'_> {
    let mut facts = FixupFacts::default();
    Collector::new(&mut facts).program(program);
    anonymous_structs::collect_facts(program, &mut facts);
    borrow_alias::collect_facts(program, &mut facts);
    def_use::collect_facts(program, &mut facts);
    effects::collect_facts(program, &mut facts);
    control_flow::collect_facts(program, &mut facts);
    null_check_dominance::collect_facts(program, &mut facts);
    pointer_option_safety::collect_facts(program, &mut facts);
    struct_field_ownership::collect_facts(program, &mut facts);
    places::collect_facts(program, &mut facts);
    values::collect_facts(program, &mut facts);
    calls::collect_facts(program, &mut facts);
    casts::collect_facts(program, &mut facts);
    strings::collect_facts(program, &mut facts);
    string_params::collect_facts(program, &mut facts);
    heap_ownership::collect_facts(program, &mut facts);
    callee_alloc_summary::collect_facts(program, &mut facts);
    interprocedural_alloc_eligibility::collect_facts(program, &mut facts);
    option_box_locals::collect_facts(program, &mut facts);
    printf::collect_facts(program, &mut facts);
    strings::collect_rewrite_facts(program, &mut facts);
    c_strings::collect_facts(program, &mut facts);
    file_ownership::collect_facts(program, &mut facts);
    ptr_len::collect_facts(program, &mut facts);
    array_element_pointer_origin::collect_facts(program, &mut facts);
    atomic_locals::collect_facts(program, &mut facts);
    lazy_singleton::collect_facts(program, &mut facts);
    buffer_cursor::collect_facts(program, &mut facts);
    counted_loop::collect_facts(program, &mut facts);
    AnalyzedProgram { program, facts }
}

struct Collector<'a> {
    facts: &'a mut FixupFacts,
    next_binding: usize,
    next_loop: usize,
}

impl<'a> Collector<'a> {
    /// For a fresh whole-program walk, where `bindings`/`loops` only ever
    /// grow by appending - `Vec::len()` is a safe, O(1) source of the next
    /// id.
    fn new(facts: &'a mut FixupFacts) -> Self {
        let next_binding = facts.bindings.len();
        let next_loop = facts.loops.len();
        Self {
            facts,
            next_binding,
            next_loop,
        }
    }

    /// For re-deriving one function's bindings/loops after removing its
    /// stale entries from the middle of `bindings`/`loops` (incremental
    /// splicing - see `FixupFacts::splice_function`). `Vec::len()` is not
    /// safe there: it can be smaller than an id already held by some
    /// *other* function's surviving entry, since that entry's position
    /// shifted down when the removed entries were spliced out from
    /// earlier in the vec. Scans once for the true maximum instead - paid
    /// per splice, not per push.
    fn resume(facts: &'a mut FixupFacts) -> Self {
        let next_binding = facts.bindings.iter().map(|b| b.id.0 + 1).max().unwrap_or(0);
        let next_loop = facts.loops.iter().map(|l| l.id.0 + 1).max().unwrap_or(0);
        Self {
            facts,
            next_binding,
            next_loop,
        }
    }

    fn program(&mut self, program: &Program) {
        for (item_index, item) in program.items.iter().enumerate() {
            let Item::Fn(f) = item else {
                continue;
            };
            let function = self.push_function(f.name.clone(), item_index);
            self.function(function, f);
        }
    }

    /// Bindings, binding types, and loops for one function, given an
    /// already-assigned `FunctionId` - independent of any other function's
    /// facts. The entry point `slate-04q.75.56.8` (incremental facts)
    /// needs to re-derive one function's bindings/loops in place without a
    /// whole-program walk; it must not call `push_function`, which assigns
    /// a fresh `FunctionId` - only `program()` does that, for a function
    /// seen for the first time.
    fn function(&mut self, function: FunctionId, f: &FnDef) {
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
        let id = BindingId(self.next_binding);
        self.next_binding += 1;
        self.facts.bindings.push(BindingFact {
            id,
            function,
            name,
            kind,
            path,
        });
        if let Some(ty) = ty {
            let ty = ty.peel_aligned().clone();
            self.facts.binding_types.push(BindingTypeFact {
                binding: id,
                rendered: ty.render(),
                ty,
            });
        }
        id
    }

    fn push_loop(&mut self, function: FunctionId, kind: LoopKind, path: AstPath) -> LoopId {
        let id = LoopId(self.next_loop);
        self.next_loop += 1;
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
            | Stmt::InlineAsm(_)
            | Stmt::Expr(_)
            | Stmt::Return(_)
            | Stmt::Break(_)
            | Stmt::Continue(_) => {}
        }
    }
}
