use super::{
    AbiSignature, Atomicity, BindingId, CTypeTable, Comments, CompareOp, FenceScope, Global,
    InlineAsm, Parameters, Place, Type, TypeDefinition, Value, Variable,
};
use crate::ast::{
    CommentAttach, CommentGroup, MacroExpansionId, MacroInvocations, NodeId, SmeAttribute, Span,
};
pub use crate::ast::{OmpScheduleKind, OmpScheduleModifier};
use crate::target_info::TargetInfo;
use std::collections::HashMap;

pub type Metadata = HashMap<NodeId, Vec<(String, String)>>;

#[derive(Debug, Clone)]
pub struct Module {
    pub target: TargetInfo,
    pub types: Vec<Span<TypeDefinition>>,
    pub asm: Vec<Span<InlineAsm>>,
    pub linker_options: Vec<String>,
    pub globals: Vec<Span<Global>>,
    pub functions: Vec<Span<Function>>,
    pub metadata: Metadata,
    pub comments: Comments,
    pub c_types: CTypeTable,
    pub macros: MacroInvocations,
    pub macro_rejections: HashMap<MacroExpansionId, &'static str>,
    pub macro_hidden_arguments: HashMap<MacroExpansionId, Vec<BindingId>>,
    pub zero_init_padding_bits: crate::compiler_options::ZeroInitPaddingBits,
    pub alloca: AllocaLayout,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct AllocaLayout {
    pub padding: u64,
    pub alignment: u64,
}

impl AllocaLayout {
    pub fn for_dialect(dialect: &crate::dialect::Dialect) -> Self {
        use crate::compiler_args::CompilerFlavor;
        use crate::target_info::TargetFamily;
        let target = dialect.target();
        let gcc = dialect.flavor() == CompilerFlavor::Gcc;
        let alignment = match target.family {
            TargetFamily::Arm32 => 8,
            TargetFamily::X86 | TargetFamily::X86_64 if gcc => target.isa.vector_register_bytes(),
            _ => 16,
        };
        let padding = match target.family {
            TargetFamily::X86_64 if gcc => 8,
            TargetFamily::X86 if gcc => 12,
            _ => 0,
        };
        Self { padding, alignment }
    }
}

#[derive(Debug, Clone)]
pub struct Comment {
    pub text: Vec<String>,
    pub attach: CommentAttach,
    pub doc: bool,
}

impl From<&CommentGroup> for Comment {
    fn from(group: &CommentGroup) -> Self {
        Self {
            text: group.comment.text.clone(),
            attach: group.attach,
            doc: group.doc,
        }
    }
}

const _: fn() = || {
    fn assert_send_sync<T: Send + Sync>() {}
    assert_send_sync::<Module>();
};

#[derive(Debug, Clone, Copy)]
pub enum Linkage {
    Internal,
    External,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Visibility {
    Default,
    Hidden,
    Protected,
    Internal,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum TlsModel {
    GlobalDynamic,
    LocalDynamic,
    InitialExec,
    LocalExec,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum DllStorage {
    Import,
    Export,
}

#[derive(Debug, Clone, Default, PartialEq, Eq)]
pub struct SymbolAttributes {
    pub asm_name: Option<String>,
    pub visibility: Option<Visibility>,
    pub weak: bool,
    pub alias: Option<String>,
    pub weakref: Option<String>,
    pub ifunc: Option<String>,
    pub section: Option<String>,
    pub used: bool,
    pub retain: bool,
    pub tls_model: Option<TlsModel>,
    pub dll_storage: Option<DllStorage>,
    pub selectany: bool,
    pub fallback: Option<String>,
    pub include: bool,
}

impl SymbolAttributes {
    pub fn merge(&mut self, later: Self) {
        self.asm_name = self.asm_name.take().or(later.asm_name);
        self.visibility = self.visibility.or(later.visibility);
        self.weak |= later.weak;
        self.alias = self.alias.take().or(later.alias);
        self.weakref = self.weakref.take().or(later.weakref);
        self.ifunc = self.ifunc.take().or(later.ifunc);
        self.section = self.section.take().or(later.section);
        self.used |= later.used;
        self.retain |= later.retain;
        self.tls_model = self.tls_model.or(later.tls_model);
        self.dll_storage = self.dll_storage.or(later.dll_storage);
        self.selectany |= later.selectany;
        self.fallback = self.fallback.take().or(later.fallback);
        self.include |= later.include;
    }
}

#[derive(Debug, Clone)]
pub struct Function {
    pub id: BindingId,
    pub name: String,
    pub parameters: Parameters,
    pub return_type: Option<Type>,
    pub abi: Option<AbiSignature>,
    pub linkage: Linkage,
    pub symbol: SymbolAttributes,
    pub semantics: FunctionSemantics,
    pub body: Option<Vec<Span<Statement>>>,
    pub fallthrough: Option<Fallthrough>,
}

#[derive(Debug, Clone, Default)]
pub struct FunctionSemantics {
    pub inlining: Option<Inlining>,
    pub inline_only: bool,
    pub discardable: bool,
    pub noreturn: bool,
    pub naked: bool,
    pub interrupt: bool,
    pub constructor: Option<u32>,
    pub destructor: Option<u32>,
    pub overloadable: bool,
    pub memory: Option<MemoryEffects>,
    pub deallocators: Vec<Deallocator>,
    pub target: Vec<TargetFeature>,
    pub sme: Vec<SmeAttribute>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum TargetFeature {
    Enable(String),
    Disable(String),
    Arch(String),
    Tune(String),
    BranchProtection(String),
}

/// `malloc(function, argument)`: `argument` is the 0-based parameter taking the pointer.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct Deallocator {
    pub function: BindingId,
    pub argument: u32,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub enum MemoryEffects {
    None,
    Read,
}

#[derive(Debug, Clone, Copy)]
pub enum Inlining {
    Hint,
    Always,
    Never,
}

#[derive(Debug, Clone)]
pub enum Fallthrough {
    Return(Box<Value>),
    Undefined,
    ReturnZero,
    ReturnVoid,
    UndefinedIfUsed,
}

#[derive(Debug, Clone)]
pub struct Evaluation {
    pub statements: Vec<Span<Statement>>,
    pub value: Value,
}

impl From<Value> for Evaluation {
    fn from(value: Value) -> Self {
        Self {
            statements: Vec::new(),
            value,
        }
    }
}

#[derive(Debug, Clone)]
pub enum Statement {
    Comment(Comment),
    Temporary {
        id: BindingId,
        ty: Type,
        initializer: Option<Value>,
        unsequenced: bool,
    },
    Let(Variable),
    Write {
        place: Place,
        value: Value,
        ordering: Option<Atomicity>,
        unsequenced: bool,
    },
    Fence {
        ordering: Atomicity,
        scope: FenceScope,
    },
    Expression(Value),
    Return(Option<Value>),
    Block(Vec<Span<Statement>>),
    Null,
    If {
        condition: Value,
        then_body: Vec<Span<Statement>>,
        else_body: Option<Vec<Span<Statement>>>,
    },
    While {
        id: BindingId,
        condition: Evaluation,
        body: Vec<Span<Statement>>,
    },
    DoWhile {
        id: BindingId,
        body: Vec<Span<Statement>>,
        condition: Evaluation,
    },
    For {
        id: BindingId,
        init: Vec<Span<Statement>>,
        condition: Option<Evaluation>,
        increment: Option<Evaluation>,
        body: Vec<Span<Statement>>,
        counted: Option<Box<CountedLoop>>,
    },
    Switch {
        id: BindingId,
        discriminant: Value,
        body: Vec<Span<Statement>>,
    },
    Case {
        switch: BindingId,
        start: Value,
        end: Option<Value>,
        body: Vec<Span<Statement>>,
    },
    Default {
        switch: BindingId,
        body: Vec<Span<Statement>>,
    },
    Break(BindingId),
    Continue(BindingId),
    Goto(BindingId),
    ComputedGoto(Value),
    Asm(Box<InlineAsm>),
    Label {
        id: BindingId,
        name: String,
        body: Vec<Span<Statement>>,
    },
    SehTry {
        id: BindingId,
        body: Vec<Span<Statement>>,
        handler: SehHandler,
    },
    SehLeave(BindingId),
    OmpParallel(Box<OmpParallel>),
    OmpFor(Box<OmpFor>),
    MacroExpansion {
        expansion: MacroExpansionId,
        body: Vec<Span<Statement>>,
    },
    NestedFunction(Box<NestedFunction>),
}

#[derive(Debug, Clone)]
pub struct NestedFunction {
    pub function: Function,
    pub captures: Vec<NestedCapture>,
}

#[derive(Debug, Clone)]
pub struct NestedCapture {
    pub id: BindingId,
    pub ty: Type,
}

#[derive(Debug, Clone)]
pub struct CountedLoop {
    pub index: BindingId,
    pub start: Value,
    pub end: Value,
    pub inclusive: bool,
}

#[derive(Debug, Clone)]
pub struct OmpParallel {
    pub num_threads: Option<Value>,
    pub condition: Option<Value>,
    pub shared: Vec<OmpShared>,
    pub private: Vec<OmpPrivate>,
    pub body: Vec<Span<Statement>>,
}

#[derive(Debug, Clone)]
pub struct OmpShared {
    pub binding: BindingId,
    pub ty: Type,
}

#[derive(Debug, Clone)]
pub struct OmpPrivate {
    pub original: BindingId,
    pub copy: Variable,
    pub firstprivate: bool,
}

#[derive(Debug, Clone)]
pub struct OmpFor {
    pub id: BindingId,
    pub schedule: Option<OmpSchedule>,
    pub nowait: bool,
    pub private: Vec<OmpPrivate>,
    pub loops: Vec<OmpLoop>,
    pub body: Vec<Span<Statement>>,
}

#[derive(Debug, Clone)]
pub struct OmpSchedule {
    pub kind: OmpScheduleKind,
    pub modifier: Option<OmpScheduleModifier>,
    pub chunk: Option<Value>,
}

#[derive(Debug, Clone)]
pub struct OmpLoop {
    pub variable: Variable,
    pub lower: Value,
    pub compare: CompareOp,
    pub upper: Value,
    pub step: Value,
    pub decrement: bool,
}

#[derive(Debug, Clone)]
pub enum SehHandler {
    Except {
        filter: Evaluation,
        body: Vec<Span<Statement>>,
    },
    Finally(Vec<Span<Statement>>),
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum SehIntrinsic {
    ExceptionCode,
    ExceptionInfo,
    AbnormalTermination,
}

impl std::fmt::Display for SehIntrinsic {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.write_str(match self {
            Self::ExceptionCode => "seh_exception_code",
            Self::ExceptionInfo => "seh_exception_info",
            Self::AbnormalTermination => "seh_abnormal_termination",
        })
    }
}

impl Module {
    pub fn new(target: TargetInfo) -> Self {
        Self {
            target,
            types: Vec::new(),
            asm: Vec::new(),
            linker_options: Vec::new(),
            globals: Vec::new(),
            functions: Vec::new(),
            metadata: Metadata::new(),
            comments: Comments::default(),
            c_types: CTypeTable::default(),
            macros: MacroInvocations::default(),
            macro_rejections: HashMap::new(),
            macro_hidden_arguments: HashMap::new(),
            zero_init_padding_bits: crate::compiler_options::ZeroInitPaddingBits::Standard,
            alloca: AllocaLayout {
                padding: 0,
                alignment: 16,
            },
        }
    }

    pub fn annotate<T>(
        &mut self,
        node: &Span<T>,
        entries: impl IntoIterator<Item = (String, String)>,
    ) {
        self.metadata.entry(node.id).or_default().extend(entries);
    }
}
