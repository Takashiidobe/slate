use std::collections::{BTreeMap, BTreeSet};

use crate::fixups::support::walk;
use crate::rust_ast::{Block, Expr, IndentStmt, Item, Program, Stmt, Type};

#[derive(Debug, Clone)]
pub(super) struct AnalyzedProgram {
    pub(super) program: Program,
    pub(super) facts: FixupFacts,
}

#[derive(Debug, Default, Clone)]
pub(super) struct FixupFacts {
    pub(super) functions: Vec<FunctionFact>,
    pub(super) bindings: Vec<BindingFact>,
    pub(super) loops: Vec<LoopFact>,
    pub(super) borrow_alias: Vec<BorrowAliasFact>,
    pub(super) def_use: Vec<DefUseFact>,
    pub(super) effects: Vec<EffectFact>,
    pub(super) places: Vec<PlaceFact>,
    pub(super) values: Vec<ValueFact>,
    pub(super) call_signatures: Vec<CallSignatureFact>,
    pub(super) callsites: Vec<CallsiteFact>,
    pub(super) string_buffers: Vec<StringBufferFact>,
    pub(super) string_pointer_views: Vec<StringPointerViewFact>,
    pub(super) string_libc_uses: Vec<StringLibcUseFact>,
    pub(super) mutability: Vec<BindingMutabilityFact>,
    pub(super) ptr_len_slices: Vec<PtrLenSliceFact>,
    pub(super) ptr_len_unsupported_callsites: Vec<PtrLenUnsupportedCallsiteFact>,
    pub(super) slice_pointer_views: Vec<SlicePointerViewFact>,
    pub(super) slice_index_ranges: Vec<SliceIndexRangeFact>,
    pub(super) slice_pointer_indexes: Vec<SlicePointerIndexFact>,
    pub(super) counted_slice_loops: Vec<CountedSliceLoopFact>,
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
pub(super) struct BindingMutabilityFact {
    pub(super) binding: BindingId,
    pub(super) required: bool,
    pub(super) reasons: BTreeSet<MutabilityReason>,
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
pub(super) struct EffectFact {
    pub(super) function: FunctionId,
    pub(super) subject: EffectSubject,
    pub(super) path: AstPath,
    pub(super) purity: Purity,
    pub(super) effects: BTreeSet<EffectKind>,
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
    FuncItem {
        item_index: usize,
    },
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
    pub(super) candidates: BTreeSet<StringRecoveryCandidate>,
    pub(super) rejections: BTreeSet<StringBufferRejection>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum StringBufferKind {
    CharArray,
    BorrowedStr,
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
    AsPtr,
    AsMutPtr,
    ArrayPtr,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct StringLibcUseFact {
    pub(super) function: FunctionId,
    pub(super) callee: StringLibcFunction,
    pub(super) path: AstPath,
    pub(super) pointer_args: Vec<BindingId>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum StringLibcFunction {
    StrLen,
    StrCmp,
    StrNCmp,
    MemCmp,
    StrCpy,
    StrNCpy,
    StrCat,
    StrNCat,
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
    pub(super) len_param: BindingId,
    pub(super) backing_array_len: u64,
    pub(super) mutable: bool,
    pub(super) elem_ty: Type,
    pub(super) len_ty: Type,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) struct PtrLenUnsupportedCallsiteFact {
    pub(super) caller: FunctionId,
    pub(super) callee: FunctionId,
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

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub(super) enum MutabilityReason {
    Assigned,
    AddressTaken,
    MutBorrowed,
    RawPtrDerived,
    MethodReceiver,
    AtomicAccess,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) enum LoopKind {
    Loop,
    While,
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

    pub(super) fn binding_requires_mut(&self, binding: BindingId) -> bool {
        self.mutability
            .iter()
            .find(|fact| fact.binding == binding)
            .is_some_and(|fact| fact.required)
    }
}

pub(super) fn analyze(program: Program) -> AnalyzedProgram {
    let mut collector = Collector::default();
    collector.program(&program);
    super::borrow_alias::collect_facts(&program, &mut collector.facts);
    super::def_use::collect_facts(&program, &mut collector.facts);
    super::effects::collect_facts(&program, &mut collector.facts);
    super::places::collect_facts(&program, &mut collector.facts);
    super::values::collect_facts(&program, &mut collector.facts);
    super::calls::collect_facts(&program, &mut collector.facts);
    super::strings::collect_facts(&program, &mut collector.facts);
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
                );
            }
            self.body(function, &f.body, &mut Vec::new());
            self.collect_mutability(function, f);
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
    ) -> BindingId {
        let id = BindingId(self.facts.bindings.len());
        self.facts.bindings.push(BindingFact {
            id,
            function,
            name,
            kind,
            path,
        });
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
            Stmt::Let { name, .. } => {
                self.push_binding(
                    function,
                    name.clone(),
                    BindingKind::Local,
                    AstPath(path.clone()),
                );
            }
            Stmt::LetIf {
                name,
                then_body,
                else_body,
                ..
            } => {
                self.push_binding(
                    function,
                    name.clone(),
                    BindingKind::Local,
                    AstPath(path.clone()),
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

    fn collect_mutability(&mut self, function: FunctionId, f: &crate::rust_ast::FnDef) {
        let mut required = BTreeMap::<String, BTreeSet<MutabilityReason>>::new();
        collect_required_mut(&f.body, &mut required);
        for binding in self
            .facts
            .bindings
            .iter()
            .filter(|binding| binding.function == function)
        {
            let reasons = required.get(&binding.name).cloned().unwrap_or_default();
            self.facts.mutability.push(BindingMutabilityFact {
                binding: binding.id,
                required: !reasons.is_empty(),
                reasons,
            });
        }
    }
}

fn collect_required_mut(
    body: &[IndentStmt],
    required: &mut BTreeMap<String, BTreeSet<MutabilityReason>>,
) {
    for stmt in body {
        collect_required_stmt(&stmt.stmt, required);
    }
}

fn collect_required_stmt(stmt: &Stmt, required: &mut BTreeMap<String, BTreeSet<MutabilityReason>>) {
    match stmt {
        Stmt::Let { init, .. } => {
            if let Some(init) = init {
                collect_expr_hazards(init, required);
            }
        }
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            collect_expr_hazards(cond, required);
            collect_required_mut(then_body, required);
            collect_expr_hazards(then_value, required);
            collect_required_mut(else_body, required);
            collect_expr_hazards(else_value, required);
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            collect_vars(target, MutabilityReason::Assigned, required);
            collect_expr_hazards(value, required);
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => collect_expr_hazards(expr, required),
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            collect_expr_hazards(cond, required);
            collect_required_mut(then_body, required);
            collect_required_mut(else_body, required);
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            collect_required_mut(body, required);
        }
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
            collect_block_required_mut(body, required);
        }
        Stmt::Match { expr, arms } => {
            collect_expr_hazards(expr, required);
            for arm in arms {
                collect_required_mut(&arm.body, required);
            }
        }
    }
}

fn collect_block_required_mut(
    block: &Block,
    required: &mut BTreeMap<String, BTreeSet<MutabilityReason>>,
) {
    collect_required_mut(&block.stmts, required);
    if let Some(tail) = &block.tail {
        collect_expr_hazards(tail, required);
    }
}

fn collect_expr_hazards(expr: &Expr, required: &mut BTreeMap<String, BTreeSet<MutabilityReason>>) {
    walk::exprs(expr, &mut |expr| match expr {
        Expr::AddrOf { expr, .. } => collect_vars(expr, MutabilityReason::AddressTaken, required),
        Expr::Ref {
            mutable: true,
            expr,
        } => collect_vars(expr, MutabilityReason::MutBorrowed, required),
        Expr::ArrayPtr {
            mutable: true,
            array,
        } => collect_vars(array, MutabilityReason::RawPtrDerived, required),
        Expr::MethodCall { recv, .. } | Expr::MethodCallGeneric { recv, .. } => {
            collect_vars(recv, MutabilityReason::MethodReceiver, required)
        }
        Expr::AtomicRef { ptr, .. }
        | Expr::AtomicLoad { ptr, .. }
        | Expr::AtomicStore { ptr, .. }
        | Expr::AtomicFetch { ptr, .. }
        | Expr::AtomicSwap { ptr, .. }
        | Expr::AtomicCompareExchange { ptr, .. } => {
            collect_vars(ptr, MutabilityReason::AtomicAccess, required)
        }
        _ => {}
    });
}

fn collect_vars(
    expr: &Expr,
    reason: MutabilityReason,
    required: &mut BTreeMap<String, BTreeSet<MutabilityReason>>,
) {
    walk::exprs(expr, &mut |expr| {
        if let Expr::Var(name) = expr {
            required
                .entry(name.as_str().to_string())
                .or_default()
                .insert(reason);
        }
    });
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
                vis: Visibility::Private,
                unsafe_extern_c: false,
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

    #[test]
    fn records_mutability_facts_for_assignments_and_alias_hazards() {
        let program = Program {
            items: vec![Item::Fn(named(
                "f",
                vec![
                    let_mut("assigned", "i32", int(0)),
                    assign("assigned", int(1)),
                    let_mut("addr", "i32", int(0)),
                    Stmt::Expr(Expr::AddrOf {
                        mutable: true,
                        expr: Box::new(var("addr")),
                    }),
                    let_mut("borrowed", "i32", int(0)),
                    Stmt::Expr(Expr::Ref {
                        mutable: true,
                        expr: Box::new(var("borrowed")),
                    }),
                    let_mut("raw", "[i32; 1]", Expr::ArrayLit(vec![int(0)])),
                    Stmt::Expr(Expr::ArrayPtr {
                        array: Box::new(var("raw")),
                        mutable: true,
                    }),
                    let_mut(
                        "receiver",
                        "Vec<i32>",
                        Expr::Call {
                            func: Box::new(Expr::Var("Vec::new".into())),
                            args: vec![],
                        },
                    ),
                    Stmt::Expr(Expr::MethodCall {
                        recv: Box::new(var("receiver")),
                        method: "push".into(),
                        args: vec![int(1)],
                    }),
                    let_mut("plain", "i32", int(0)),
                ],
            ))],
        };

        let analyzed = analyze(program);
        let function = FunctionId(0);
        let reason_for = |name: &str| {
            let binding = analyzed
                .facts
                .bindings
                .iter()
                .find(|binding| binding.function == function && binding.name == name)
                .unwrap();
            analyzed
                .facts
                .mutability
                .iter()
                .find(|fact| fact.binding == binding.id)
                .unwrap()
                .reasons
                .clone()
        };

        assert_eq!(
            reason_for("assigned"),
            BTreeSet::from([MutabilityReason::Assigned])
        );
        assert_eq!(
            reason_for("addr"),
            BTreeSet::from([MutabilityReason::AddressTaken])
        );
        assert_eq!(
            reason_for("borrowed"),
            BTreeSet::from([MutabilityReason::MutBorrowed])
        );
        assert_eq!(
            reason_for("raw"),
            BTreeSet::from([MutabilityReason::RawPtrDerived])
        );
        assert_eq!(
            reason_for("receiver"),
            BTreeSet::from([MutabilityReason::MethodReceiver])
        );
        assert!(reason_for("plain").is_empty());
    }
}
