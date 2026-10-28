use super::{
    AbiSignature, Atomicity, BindingId, Comments, CompareOp, FenceScope, Global, InlineAsm,
    Parameters, Place, Type, TypeDefinition, Value, Variable,
};
use crate::ast::{CommentAttach, CommentGroup, NodeId, Span};
pub use crate::ast::{OmpScheduleKind, OmpScheduleModifier};
use crate::target_info::TargetInfo;
use std::collections::HashMap;

pub type Metadata = HashMap<NodeId, Vec<(String, String)>>;

#[derive(Debug, Clone)]
pub struct Module {
    pub target: TargetInfo,
    pub types: Vec<Span<TypeDefinition>>,
    pub asm: Vec<Span<InlineAsm>>,
    pub globals: Vec<Span<Global>>,
    pub functions: Vec<Span<Function>>,
    pub metadata: Metadata,
    pub comments: Comments,
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
    pub noreturn: bool,
    pub naked: bool,
    pub interrupt: bool,
    pub constructor: Option<u32>,
    pub destructor: Option<u32>,
    pub overloadable: bool,
    pub memory: Option<MemoryEffects>,
    pub deallocators: Vec<Deallocator>,
    pub target: Vec<TargetFeature>,
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
            globals: Vec::new(),
            functions: Vec::new(),
            metadata: Metadata::new(),
            comments: Comments::default(),
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
