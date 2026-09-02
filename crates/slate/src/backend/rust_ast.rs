use crate::function_identity::{CallBinding, FunctionIdentity};
use ordered_float::OrderedFloat;
use std::collections::BTreeSet;

#[derive(Debug, Clone, Default, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct Program {
    pub items: Vec<Item>,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Item {
    Fn(FnDef),
    Comment(Comment),
    CrateAttrs(Vec<CrateAttr>),
    Mod {
        name: Ident,
    },
    InlineMod {
        vis: Visibility,
        name: Ident,
        items: Vec<Item>,
    },
    Use {
        path: Path,
    },
    Static {
        attrs: Vec<Attr>,
        vis: Visibility,
        mutable: bool,
        name: String,
        ty: Type,
        init: Expr,
    },
    Const {
        attrs: Vec<Attr>,
        name: String,
        ty: Type,
        init: Expr,
    },
    ExternBlock {
        abi: String,
        decls: Vec<ExternDecl>,
    },
    Enum(EnumDef),
    Record(RecordDef),
    Struct(StructDef),
    Impl(ImplBlock),
    Macro {
        name: String,
        args: Vec<Expr>,
    },
    Cfg {
        cfg: Cfg,
        item: Box<Item>,
    },
    SupportModule(SupportModule),
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct SupportModule {
    pub name: Ident,
    pub source: String,
    pub exports: Vec<Path>,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct Comment {
    pub lines: Vec<String>,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Cfg {
    Flag(String),
    Opt { key: String, value: String },
    Not(Box<Cfg>),
    Any(Vec<Cfg>),
    All(Vec<Cfg>),
}

impl Cfg {
    pub fn render(&self) -> String {
        crate::backend::codegen::cfg_to_string(self)
    }
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct StructDef {
    pub attrs: Vec<Attr>,
    pub vis: Visibility,
    pub field_vis: Visibility,
    pub generics: Vec<GenericParam>,
    pub name: String,
    pub fields: StructFields,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum StructFields {
    Tuple(Vec<Type>),
    Named(Vec<StructField>),
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct StructField {
    pub attrs: Vec<Attr>,
    pub name: String,
    pub ty: Type,
}

#[derive(Debug, Clone, Copy, Default, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Visibility {
    #[default]
    Private,
    Pub,
}

impl Visibility {
    pub fn keyword(self) -> Option<&'static str> {
        match self {
            Visibility::Private => None,
            Visibility::Pub => Some("pub"),
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum CrateAttr {
    Allow(Vec<Lint>),
    Deny(Vec<Lint>),
    Feature(Feature),
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Lint {
    DeadCode,
    Unused,
    NonCamelCaseTypes,
    NonSnakeCase,
    NonUpperCaseGlobals,
    ArithmeticOverflow,
    UnconditionalPanic,
    SuspiciousRuntimeSymbolDefinitions,
    UnpredictableFunctionPointerComparisons,
    UnusedComparisons,
}

impl Lint {
    pub fn spelling(&self) -> &'static str {
        match self {
            Lint::DeadCode => "dead_code",
            Lint::Unused => "unused",
            Lint::NonCamelCaseTypes => "non_camel_case_types",
            Lint::NonSnakeCase => "non_snake_case",
            Lint::NonUpperCaseGlobals => "non_upper_case_globals",
            Lint::ArithmeticOverflow => "arithmetic_overflow",
            Lint::UnconditionalPanic => "unconditional_panic",
            Lint::SuspiciousRuntimeSymbolDefinitions => "suspicious_runtime_symbol_definitions",
            Lint::UnpredictableFunctionPointerComparisons => {
                "unpredictable_function_pointer_comparisons"
            }
            Lint::UnusedComparisons => "unused_comparisons",
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Feature {
    AbiUnadjusted,
    AsmGotoWithOutputs,
    Breakpoint,
    CVariadic,
    F16,
    F128,
    Linkage,
    LinkLlvmIntrinsics,
    PortableSimd,
    SimdFfi,
    ThreadLocal,
    UsedWithArg,
}

impl Feature {
    pub fn spelling(self) -> &'static str {
        match self {
            Feature::AbiUnadjusted => "abi_unadjusted",
            Feature::AsmGotoWithOutputs => "asm_goto_with_outputs",
            Feature::Breakpoint => "breakpoint",
            Feature::CVariadic => "c_variadic",
            Feature::F16 => "f16",
            Feature::F128 => "f128",
            Feature::Linkage => "linkage",
            Feature::LinkLlvmIntrinsics => "link_llvm_intrinsics",
            Feature::PortableSimd => "portable_simd",
            Feature::SimdFfi => "simd_ffi",
            Feature::ThreadLocal => "thread_local",
            Feature::UsedWithArg => "used_with_arg",
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Attr {
    Call { path: Path, args: Vec<AttrArg> },
    Allow(Vec<Lint>),
    Repr(Vec<Repr>),
    Derive(Vec<Derive>),
    NoMangle,
    Naked,
    ExportName(String),
    LinkName(String),
    ThreadLocal,
    WeakLinkage,
    ExternWeakLinkage,
    LinkSection(String),
    Used(UsedKind),
    Deprecated(Option<String>),
    TargetFeature(String),
    Cold,
    MustUse,
    Inline(InlineHint),
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum InlineHint {
    Always,
    Never,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum AttrArg {
    Type(Type),
    UInt(u64),
    Bool(bool),
    Named(String, Box<AttrArg>),
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum UsedKind {
    Plain,
    Compiler,
    Linker,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Abi {
    C,
    CUnwind,
}

impl Abi {
    pub fn spelling(self) -> &'static str {
        match self {
            Abi::C => "C",
            Abi::CUnwind => "C-unwind",
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Repr {
    C,
    Align(u32),
    Packed(u32),
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Derive {
    Clone,
    Copy,
    PartialEq,
    PartialOrd,
    Eq,
    Debug,
    Hash,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct GenericParam {
    pub name: String,
    pub bounds: Vec<TraitBound>,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct TraitBound {
    pub trait_: StdTrait,
    pub assoc: Vec<(String, Type)>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum StdTrait {
    Add,
    Sub,
    Mul,
    Div,
    AddAssign,
    SubAssign,
    MulAssign,
    DivAssign,
    RemAssign,
    BitAnd,
    BitOr,
    BitXor,
    Shl,
    Shr,
    BitAndAssign,
    BitOrAssign,
    BitXorAssign,
    ShlAssign,
    ShrAssign,
    Rem,
    PartialEq,
    PartialOrd,
    Not,
    Neg,
    Deref,
    DerefMut,
}

impl StdTrait {
    pub fn path(self) -> &'static str {
        match self {
            StdTrait::Add => "core::ops::Add",
            StdTrait::Sub => "core::ops::Sub",
            StdTrait::Mul => "core::ops::Mul",
            StdTrait::Div => "core::ops::Div",
            StdTrait::AddAssign => "core::ops::AddAssign",
            StdTrait::SubAssign => "core::ops::SubAssign",
            StdTrait::MulAssign => "core::ops::MulAssign",
            StdTrait::DivAssign => "core::ops::DivAssign",
            StdTrait::RemAssign => "core::ops::RemAssign",
            StdTrait::BitAnd => "core::ops::BitAnd",
            StdTrait::BitOr => "core::ops::BitOr",
            StdTrait::BitXor => "core::ops::BitXor",
            StdTrait::Shl => "core::ops::Shl",
            StdTrait::Shr => "core::ops::Shr",
            StdTrait::BitAndAssign => "core::ops::BitAndAssign",
            StdTrait::BitOrAssign => "core::ops::BitOrAssign",
            StdTrait::BitXorAssign => "core::ops::BitXorAssign",
            StdTrait::ShlAssign => "core::ops::ShlAssign",
            StdTrait::ShrAssign => "core::ops::ShrAssign",
            StdTrait::Rem => "core::ops::Rem",
            StdTrait::PartialEq => "core::cmp::PartialEq",
            StdTrait::PartialOrd => "core::cmp::PartialOrd",
            StdTrait::Not => "core::ops::Not",
            StdTrait::Neg => "core::ops::Neg",
            StdTrait::Deref => "core::ops::Deref",
            StdTrait::DerefMut => "core::ops::DerefMut",
        }
    }

    pub fn method(self) -> &'static str {
        match self {
            StdTrait::Add => "add",
            StdTrait::Sub => "sub",
            StdTrait::Mul => "mul",
            StdTrait::Div => "div",
            StdTrait::AddAssign => "add_assign",
            StdTrait::SubAssign => "sub_assign",
            StdTrait::MulAssign => "mul_assign",
            StdTrait::DivAssign => "div_assign",
            StdTrait::RemAssign => "rem_assign",
            StdTrait::BitAnd => "bitand",
            StdTrait::BitOr => "bitor",
            StdTrait::BitXor => "bitxor",
            StdTrait::Shl => "shl",
            StdTrait::Shr => "shr",
            StdTrait::BitAndAssign => "bitand_assign",
            StdTrait::BitOrAssign => "bitor_assign",
            StdTrait::BitXorAssign => "bitxor_assign",
            StdTrait::ShlAssign => "shl_assign",
            StdTrait::ShrAssign => "shr_assign",
            StdTrait::Rem => "rem",
            StdTrait::PartialEq => "eq",
            StdTrait::PartialOrd => "partial_cmp",
            StdTrait::Not => "not",
            StdTrait::Neg => "neg",
            StdTrait::Deref => "deref",
            StdTrait::DerefMut => "deref_mut",
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct ImplBlock {
    pub generics: Vec<GenericParam>,
    pub trait_: Option<TraitRef>,
    pub self_ty: Type,
    pub items: Vec<ImplItem>,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum TraitRef {
    Std(StdTrait),
    From(Type),
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum ImplItem {
    AssocType { name: String, ty: Type },
    Method(Method),
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct Method {
    pub name: String,
    pub self_kind: SelfKind,
    pub params: Vec<FnParam>,
    pub ret: Option<Type>,
    pub body: Expr,
}

#[derive(Debug, Clone, Copy, Default, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum SelfKind {
    #[default]
    None,
    Value,
    Ref,
    RefMut,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct EnumConst {
    pub comments: Vec<Comment>,
    pub name: String,
    pub value: i64,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct EnumDef {
    pub comments: Vec<Comment>,
    pub attrs: Vec<Attr>,
    pub vis: Visibility,
    pub name: String,
    pub variants: Vec<EnumConst>,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct RecordField {
    pub comments: Vec<Comment>,
    pub name: Ident,
    pub ty: Type,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct RecordDef {
    pub comments: Vec<Comment>,
    pub vis: Visibility,
    pub field_vis: Visibility,
    pub is_union: bool,
    pub allow_non_camel_case: bool,
    pub name: String,
    pub fields: Vec<RecordField>,
    pub packed: Option<u32>,
    pub align: Option<u32>,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum ExternDecl {
    Fn(ExternFnDecl),
    Static {
        attrs: Vec<Attr>,
        mutable: bool,
        name: String,
        ty: Type,
    },
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct ExternFnDecl {
    pub attrs: Vec<Attr>,
    pub name: String,
    pub identity: FunctionIdentity,
    pub declared_type: Option<String>,
    pub trusted_headers: BTreeSet<String>,
    pub params: Vec<FnParam>,
    pub variadic: bool,
    pub ret: Option<Type>,
    pub safe: bool,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct FnDef {
    pub attrs: Vec<Attr>,
    pub vis: Visibility,
    pub unsafe_: bool,
    pub abi: Option<Abi>,
    pub name: String,
    pub params: Vec<FnParam>,
    pub ret: Option<Type>,
    pub body: Vec<IndentStmt>,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct FnParam {
    pub name: String,
    pub mutable: bool,
    pub ty: Type,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct IndentStmt {
    pub depth: usize,
    pub stmt: Stmt,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct MatchArm {
    pub pattern: Pattern,
    pub body: Vec<IndentStmt>,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct ExprMatchArm {
    pub pattern: Pattern,
    pub value: Expr,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Pattern {
    Wildcard,
    Binding(Ident),
    I64(i64),
    I128(i128),
    U128(u128),
    InclusiveRange { start: i128, end: i128 },
    InclusiveRangeU128 { start: u128, end: u128 },
    TupleStruct { name: Ident, fields: Vec<Pattern> },
    Guarded { bind: Ident, cond: Box<Expr> },
    Or(Vec<Pattern>),
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum AtomicPlace {
    Ptr(Box<Expr>),
    Local(Ident),
}

impl AtomicPlace {
    pub fn ptr_expr(&self) -> Option<&Expr> {
        match self {
            AtomicPlace::Ptr(ptr) => Some(ptr),
            AtomicPlace::Local(_) => None,
        }
    }

    pub fn ptr_expr_mut(&mut self) -> Option<&mut Expr> {
        match self {
            AtomicPlace::Ptr(ptr) => Some(ptr),
            AtomicPlace::Local(_) => None,
        }
    }

    pub fn local(&self) -> Option<&Ident> {
        match self {
            AtomicPlace::Ptr(_) => None,
            AtomicPlace::Local(name) => Some(name),
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum AtomicType {
    I8,
    U8,
    I16,
    U16,
    I32,
    U32,
    I64,
    U64,
    Isize,
    Usize,
    Bool,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum AtomicOrdering {
    Relaxed,
    Acquire,
    Release,
    AcqRel,
    SeqCst,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum AtomicRmwOp {
    Add,
    Sub,
    And,
    Xor,
    Or,
    Nand,
    Max,
    Min,
}

#[derive(Debug, Clone, Default, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct Block {
    pub stmts: Vec<IndentStmt>,
    pub tail: Option<Box<Expr>>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum AsmDialect {
    Att,
    Intel,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum AsmReg {
    Class(String),
    Explicit(String),
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum AsmOperand {
    In {
        reg: AsmReg,
        value: Expr,
    },
    Out {
        reg: AsmReg,
        late: bool,
        value: Expr,
    },
    InOut {
        reg: AsmReg,
        late: bool,
        input: Expr,
        output: Expr,
    },
    Const(Expr),
    Label {
        state: Expr,
        value: Expr,
        destination: Label,
    },
}

impl AsmOperand {
    pub fn visit_exprs(&self, f: &mut impl FnMut(&Expr)) {
        match self {
            Self::In { value, .. } | Self::Out { value, .. } | Self::Const(value) => f(value),
            Self::InOut { input, output, .. } => {
                f(input);
                f(output);
            }
            Self::Label { state, value, .. } => {
                f(state);
                f(value);
            }
        }
    }

    pub fn visit_exprs_mut(&mut self, f: &mut impl FnMut(&mut Expr)) {
        match self {
            Self::In { value, .. } | Self::Out { value, .. } | Self::Const(value) => f(value),
            Self::InOut { input, output, .. } => {
                f(input);
                f(output);
            }
            Self::Label { state, value, .. } => {
                f(state);
                f(value);
            }
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct InlineAsm {
    pub template: String,
    pub dialect: Option<AsmDialect>,
    pub operands: Vec<AsmOperand>,
    pub raw: bool,
}

#[expect(
    clippy::large_enum_variant,
    reason = "statement payloads vary by kind; boxed variants would add indirection on the hot lowering path"
)]
#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Stmt {
    Let {
        name: String,
        mutable: bool,
        ty: Option<Type>,
        init: Option<Expr>,
    },
    LetIf {
        name: String,
        mutable: bool,
        ty: Option<Type>,
        cond: Expr,
        then_body: Vec<IndentStmt>,
        then_value: Expr,
        else_body: Vec<IndentStmt>,
        else_value: Expr,
    },
    Assign {
        target: Expr,
        value: Expr,
    },
    CompoundAssign {
        target: Expr,
        op: BinOp,
        value: Expr,
    },
    InlineAsm(InlineAsm),
    Expr(Expr),
    Return(Option<Expr>),
    Unsafe {
        body: Block,
    },
    If {
        cond: Expr,
        then_body: Vec<IndentStmt>,
        else_body: Vec<IndentStmt>,
    },
    Loop {
        label: Option<Label>,
        body: Vec<IndentStmt>,
    },
    For {
        pat: String,
        iter: Expr,
        body: Vec<IndentStmt>,
    },
    Scope {
        body: Vec<IndentStmt>,
    },
    LabeledBlock {
        label: Label,
        body: Vec<IndentStmt>,
    },
    Match {
        expr: Expr,
        arms: Vec<MatchArm>,
    },
    Break(Option<Label>),
    Continue(Option<Label>),
    While {
        cond: Expr,
        body: Block,
    },
    Block(Block),
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum RustValue {
    I64(i64),
    Usize(usize),
    I128(i128),
    U128(u128),
    TypedInt(i128, Prim),
    TypedUInt(u128, Prim),
    Float(OrderedFloat<f64>),
    Bool(bool),
    None,
    NullPtr,
}

impl From<i64> for RustValue {
    fn from(value: i64) -> Self {
        Self::I64(value)
    }
}

impl From<usize> for RustValue {
    fn from(value: usize) -> Self {
        Self::Usize(value)
    }
}

impl From<i128> for RustValue {
    fn from(value: i128) -> Self {
        Self::I128(value)
    }
}

impl From<u128> for RustValue {
    fn from(value: u128) -> Self {
        Self::U128(value)
    }
}

impl From<f64> for RustValue {
    fn from(value: f64) -> Self {
        Self::Float(OrderedFloat::from(value))
    }
}

impl From<bool> for RustValue {
    fn from(value: bool) -> Self {
        Self::Bool(value)
    }
}

mod ident_intern {
    use std::collections::HashSet;
    use std::sync::{Mutex, OnceLock};

    #[derive(Debug, Clone, Copy, Eq)]
    pub(super) struct Symbol(&'static str);

    impl Symbol {
        pub(super) fn as_str(self) -> &'static str {
            self.0
        }
    }

    impl PartialEq for Symbol {
        fn eq(&self, other: &Self) -> bool {
            std::ptr::eq(self.0, other.0)
        }
    }

    impl std::hash::Hash for Symbol {
        fn hash<H: std::hash::Hasher>(&self, state: &mut H) {
            self.0.as_ptr().hash(state);
        }
    }

    fn table() -> &'static Mutex<HashSet<&'static str>> {
        static TABLE: OnceLock<Mutex<HashSet<&'static str>>> = OnceLock::new();
        TABLE.get_or_init(|| Mutex::new(HashSet::new()))
    }

    pub(super) fn intern(s: &str) -> Symbol {
        let mut t = table().lock().unwrap();
        if let Some(&existing) = t.get(s) {
            return Symbol(existing);
        }
        let leaked: &'static str = Box::leak(s.to_owned().into_boxed_str());
        t.insert(leaked);
        Symbol(leaked)
    }
}

#[derive(Debug, Clone, Copy, Eq)]
pub struct Ident(ident_intern::Symbol);

impl Ident {
    pub fn new(s: impl Into<String>) -> Self {
        Self(ident_intern::intern(&s.into()))
    }

    pub fn as_str(&self) -> &str {
        self.0.as_str()
    }

    pub fn into_string(self) -> String {
        self.as_str().to_string()
    }
}

impl PartialEq for Ident {
    fn eq(&self, other: &Self) -> bool {
        self.0 == other.0
    }
}

impl std::hash::Hash for Ident {
    fn hash<H: std::hash::Hasher>(&self, state: &mut H) {
        self.0.hash(state);
    }
}

impl PartialOrd for Ident {
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
        Some(self.cmp(other))
    }
}

impl Ord for Ident {
    fn cmp(&self, other: &Self) -> std::cmp::Ordering {
        self.as_str().cmp(other.as_str())
    }
}

impl From<String> for Ident {
    fn from(s: String) -> Self {
        Self::new(s)
    }
}

impl From<&str> for Ident {
    fn from(s: &str) -> Self {
        Self::new(s)
    }
}

impl std::fmt::Display for Ident {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.write_str(self.as_str())
    }
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct Path {
    pub segments: Vec<Ident>,
}

impl Path {
    pub fn new(segments: impl IntoIterator<Item = Ident>) -> Self {
        Self {
            segments: segments.into_iter().collect(),
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct Label(Ident);

impl Label {
    pub fn new(name: impl Into<Ident>) -> Self {
        Self(name.into())
    }

    pub fn as_str(&self) -> &str {
        self.0.as_str()
    }
}

impl From<String> for Label {
    fn from(s: String) -> Self {
        Self::new(s)
    }
}

impl From<&str> for Label {
    fn from(s: &str) -> Self {
        Self::new(s)
    }
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Expr {
    Value(RustValue),
    Str(String),
    HexFloat(String),
    ByteStr(Vec<u8>),
    CStr(Vec<u8>),
    Var(Ident),
    Unary {
        op: UnaryOp,
        expr: Box<Expr>,
    },
    Binary {
        op: BinOp,
        lhs: Box<Expr>,
        rhs: Box<Expr>,
    },
    Range {
        start: Box<Expr>,
        end: Box<Expr>,
    },
    Call {
        func: Box<Expr>,
        args: Vec<Expr>,
        binding: CallBinding,
    },
    MethodCall {
        recv: Box<Expr>,
        method: String,
        args: Vec<Expr>,
    },
    MethodCallGeneric {
        recv: Box<Expr>,
        method: String,
        type_args: Vec<Type>,
        args: Vec<Expr>,
    },
    Field {
        base: Box<Expr>,
        field: String,
    },
    TupleField {
        base: Box<Expr>,
        index: usize,
    },
    ArrayPtr {
        array: Box<Expr>,
        mutable: bool,
    },
    Index {
        base: Box<Expr>,
        index: Box<Expr>,
    },
    StructLit {
        name: String,
        fields: Vec<(String, Expr)>,
    },
    TupleStructLit {
        name: String,
        fields: Vec<Expr>,
    },
    ArrayLit(Vec<Expr>),
    ArrayRepeat {
        elem: Box<Expr>,
        len: usize,
    },
    VecLit(Vec<Expr>),
    VecRepeat {
        elem: Box<Expr>,
        len: Box<Expr>,
    },
    Macro {
        name: String,
        args: Vec<Expr>,
    },
    Closure {
        params: Vec<Ident>,
        body: Box<Expr>,
    },
    Match {
        expr: Box<Expr>,
        arms: Vec<ExprMatchArm>,
    },
    If {
        cond: Box<Expr>,
        then_expr: Box<Expr>,
        else_expr: Box<Expr>,
    },
    Block(Box<Block>),
    Unsafe(Box<Block>),
    ConstBlock(Box<Expr>),
    Cast {
        expr: Box<Expr>,
        ty: Type,
    },
    Ref {
        mutable: bool,
        expr: Box<Expr>,
    },
    AddrOf {
        mutable: bool,
        expr: Box<Expr>,
    },
    AtomicRef {
        ty: AtomicType,
        place: AtomicPlace,
    },
    AtomicLoad {
        ty: AtomicType,
        place: AtomicPlace,
        ordering: AtomicOrdering,
    },
    AtomicStore {
        ty: AtomicType,
        place: AtomicPlace,
        value: Box<Expr>,
        ordering: AtomicOrdering,
    },
    AtomicFetch {
        ty: AtomicType,
        op: AtomicRmwOp,
        place: AtomicPlace,
        value: Box<Expr>,
        ordering: AtomicOrdering,
    },
    AtomicSwap {
        ty: AtomicType,
        place: AtomicPlace,
        value: Box<Expr>,
        ordering: AtomicOrdering,
    },
    AtomicCompareExchange {
        ty: AtomicType,
        place: AtomicPlace,
        expected: Box<Expr>,
        desired: Box<Expr>,
        success: AtomicOrdering,
        failure: AtomicOrdering,
    },
    AtomicNew {
        ty: AtomicType,
        value: Box<Expr>,
    },
    AtomicFence {
        ordering: AtomicOrdering,
    },
    Transmute {
        from: Type,
        to: Type,
        expr: Box<Expr>,
    },
    CopyNonoverlapping {
        src: Box<Expr>,
        dst: Box<Expr>,
        count: usize,
    },
    PtrCopy {
        src: Box<Expr>,
        dst: Box<Expr>,
        count: Box<Expr>,
        overlapping: bool,
    },
    WriteBytes {
        dst: Box<Expr>,
        val: Box<Expr>,
        count: Box<Expr>,
    },
    Todo(String),
    Path(Path),
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Type {
    Prim(Prim),
    Custom(String),
    String,
    LongDouble,
    TyVar(Ident),
    CLib(CLibType),
    Complex(Box<Type>),
    Generic {
        name: String,
        args: Vec<Type>,
    },
    VaList,
    Str,
    Ref {
        mutable: bool,
        inner: Box<Type>,
    },
    Slice(Box<Type>),
    Ptr {
        mutable: bool,
        inner: Box<Type>,
    },
    Array {
        elem: Box<Type>,
        len: u64,
    },
    FnPtr {
        abi: Abi,
        params: Vec<Type>,
        ret: Box<Type>,
    },
    Unit,
    Variadic,
    Never,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum CLibInitializer {
    ScalarZero,
    Zeroed,
    Fields(&'static [&'static str]),
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct CLibType {
    c_name: &'static str,
    rust_path: &'static str,
    alignment: u32,
    initializer: CLibInitializer,
}

impl CLibType {
    pub const VOID: Self = Self::new("void", "core::ffi::c_void", 1, CLibInitializer::ScalarZero);
    pub const CHAR: Self = Self::new("char", "core::ffi::c_char", 1, CLibInitializer::ScalarZero);

    pub const fn new(
        c_name: &'static str,
        rust_path: &'static str,
        alignment: u32,
        initializer: CLibInitializer,
    ) -> Self {
        Self {
            c_name,
            rust_path,
            alignment,
            initializer,
        }
    }

    pub fn c_name(self) -> &'static str {
        self.c_name
    }

    pub fn path(self) -> &'static str {
        self.rust_path
    }

    pub fn alignment(self) -> u32 {
        self.alignment
    }

    pub fn initializer(self) -> CLibInitializer {
        self.initializer
    }
}

pub const CLIB_RECORD_TYPES: &[CLibType] = &[
    CLibType::new("FILE", "libc::FILE", 1, CLibInitializer::ScalarZero),
    CLibType::new(
        "pthread_attr_t",
        "libc::pthread_attr_t",
        1,
        CLibInitializer::ScalarZero,
    ),
    CLibType::new(
        "timespec",
        "libc::timespec",
        8,
        CLibInitializer::Fields(&["tv_sec", "tv_nsec"]),
    ),
];

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Prim {
    Bool,
    I8,
    I16,
    I32,
    I64,
    I128,
    Isize,
    U8,
    U16,
    U32,
    U64,
    U128,
    Usize,
    F16,
    F32,
    F64,
    F128,
}

impl Prim {
    pub fn spelling(self) -> &'static str {
        match self {
            Prim::Bool => "bool",
            Prim::I8 => "i8",
            Prim::I16 => "i16",
            Prim::I32 => "i32",
            Prim::I64 => "i64",
            Prim::I128 => "i128",
            Prim::Isize => "isize",
            Prim::U8 => "u8",
            Prim::U16 => "u16",
            Prim::U32 => "u32",
            Prim::U64 => "u64",
            Prim::U128 => "u128",
            Prim::Usize => "usize",
            Prim::F16 => "f16",
            Prim::F32 => "f32",
            Prim::F64 => "f64",
            Prim::F128 => "f128",
        }
    }

    pub fn parse(s: &str) -> Option<Prim> {
        Some(match s {
            "bool" => Prim::Bool,
            "i8" => Prim::I8,
            "i16" => Prim::I16,
            "i32" => Prim::I32,
            "i64" => Prim::I64,
            "i128" => Prim::I128,
            "isize" => Prim::Isize,
            "u8" => Prim::U8,
            "u16" => Prim::U16,
            "u32" => Prim::U32,
            "u64" => Prim::U64,
            "u128" => Prim::U128,
            "usize" => Prim::Usize,
            "f16" => Prim::F16,
            "f32" => Prim::F32,
            "f64" => Prim::F64,
            "f128" => Prim::F128,
            _ => return None,
        })
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum BinOp {
    Add,
    Sub,
    Mul,
    Div,
    Rem,
    BitAnd,
    BitOr,
    BitXor,
    Shl,
    Shr,
    Eq,
    Ne,
    Lt,
    Le,
    Gt,
    Ge,
    And,
    Or,
}

impl BinOp {
    pub fn spelling(self) -> &'static str {
        match self {
            BinOp::Add => "+",
            BinOp::Sub => "-",
            BinOp::Mul => "*",
            BinOp::Div => "/",
            BinOp::Rem => "%",
            BinOp::BitAnd => "&",
            BinOp::BitOr => "|",
            BinOp::BitXor => "^",
            BinOp::Shl => "<<",
            BinOp::Shr => ">>",
            BinOp::Eq => "==",
            BinOp::Ne => "!=",
            BinOp::Lt => "<",
            BinOp::Le => "<=",
            BinOp::Gt => ">",
            BinOp::Ge => ">=",
            BinOp::And => "&&",
            BinOp::Or => "||",
        }
    }

    // higher binds tighter; mirrors Rust's operator precedence.
    pub fn precedence(self) -> u8 {
        match self {
            BinOp::Or => 3,
            BinOp::And => 4,
            BinOp::Eq | BinOp::Ne | BinOp::Lt | BinOp::Gt | BinOp::Le | BinOp::Ge => 5,
            BinOp::BitOr => 6,
            BinOp::BitXor => 7,
            BinOp::BitAnd => 8,
            BinOp::Shl | BinOp::Shr => 9,
            BinOp::Add | BinOp::Sub => 10,
            BinOp::Mul | BinOp::Div | BinOp::Rem => 11,
        }
    }

    pub fn is_comparison(self) -> bool {
        self.precedence() == 5
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum UnaryOp {
    Neg,
    Not,
    Deref,
    Raw(Raw),
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Raw {
    Const,
    Mut,
}

impl UnaryOp {
    pub fn spelling(self) -> &'static str {
        match self {
            UnaryOp::Neg => "-",
            UnaryOp::Not => "!",
            UnaryOp::Deref => "*",
            UnaryOp::Raw(Raw::Const) => "&raw const ",
            UnaryOp::Raw(Raw::Mut) => "&raw mut ",
        }
    }
}

impl Program {
    pub fn emit(&self) -> String {
        crate::backend::codegen::program_to_string(self)
    }
}

impl Stmt {
    pub fn render(&self) -> String {
        crate::backend::codegen::stmt_to_string(self)
    }

    pub fn substitute_var(&mut self, name: &str, replacement: &Expr) -> bool {
        stmt_substitute_var(self, name, replacement)
    }

    pub fn child_bodies_mut(&mut self) -> Vec<&mut Vec<IndentStmt>> {
        match self {
            Stmt::LetIf {
                then_body,
                else_body,
                ..
            }
            | Stmt::If {
                then_body,
                else_body,
                ..
            } => vec![then_body, else_body],
            Stmt::Loop { body, .. }
            | Stmt::For { body, .. }
            | Stmt::Scope { body }
            | Stmt::LabeledBlock { body, .. } => vec![body],
            Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
                vec![&mut body.stmts]
            }
            Stmt::Match { arms, .. } => arms.iter_mut().map(|arm| &mut arm.body).collect(),
            Stmt::Let { .. }
            | Stmt::Assign { .. }
            | Stmt::CompoundAssign { .. }
            | Stmt::InlineAsm(_)
            | Stmt::Expr(_)
            | Stmt::Return(_)
            | Stmt::Break(_)
            | Stmt::Continue(_) => Vec::new(),
        }
    }

    pub fn declared_name_mut(&mut self) -> Option<&mut String> {
        match self {
            Stmt::Let { name, .. } | Stmt::LetIf { name, .. } => Some(name),
            Stmt::For { pat, .. } => Some(pat),
            _ => None,
        }
    }

    pub fn collect_vars(&self, out: &mut Vec<Ident>) {
        stmt_collect_vars(self, out)
    }

    pub fn collect_calls<'a>(&'a self, out: &mut Vec<(&'a Ident, &'a [Expr])>) {
        stmt_collect_calls(self, out)
    }

    pub fn collect_offset_calls<'a>(&'a self, out: &mut Vec<(&'a Ident, &'a str, &'a Expr)>) {
        stmt_collect_offset_calls(self, out)
    }

    pub fn reads_var(&self, name: &str) -> bool {
        stmt_reads_var(self, name)
    }
}

impl Expr {
    pub fn aligned_inner(&self) -> Option<&Expr> {
        match self {
            Expr::TupleStructLit { name, fields }
                if name == "aligned::Aligned" && fields.len() == 1 =>
            {
                fields.first()
            }
            _ => None,
        }
    }

    pub fn peel_aligned(&self) -> &Expr {
        self.aligned_inner().unwrap_or(self)
    }

    pub fn render(&self) -> String {
        crate::backend::codegen::expr_to_string(self)
    }

    pub fn reads_var(&self, name: &str) -> bool {
        match self {
            Expr::Var(v) => v.as_str() == name,
            Expr::Value(_)
            | Expr::Str(_)
            | Expr::HexFloat(_)
            | Expr::ByteStr(_)
            | Expr::ConstBlock(_)
            | Expr::CStr(_)
            | Expr::Path(_)
            | Expr::AtomicFence { .. }
            | Expr::Todo(_) => false,
            Expr::Unary { expr, .. }
            | Expr::Cast { expr, .. }
            | Expr::Ref { expr, .. }
            | Expr::AddrOf { expr, .. }
            | Expr::Transmute { expr, .. } => expr.reads_var(name),
            Expr::Block(block) | Expr::Unsafe(block) => {
                block
                    .stmts
                    .iter()
                    .any(|stmt| stmt_reads_var(&stmt.stmt, name))
                    || block.tail.as_ref().is_some_and(|tail| tail.reads_var(name))
            }
            Expr::CopyNonoverlapping { src, dst, .. } => src.reads_var(name) || dst.reads_var(name),
            Expr::PtrCopy {
                src, dst, count, ..
            } => src.reads_var(name) || dst.reads_var(name) || count.reads_var(name),
            Expr::WriteBytes { dst, val, count } => {
                dst.reads_var(name) || val.reads_var(name) || count.reads_var(name)
            }
            Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. } => {
                place.ptr_expr().is_some_and(|ptr| ptr.reads_var(name))
            }
            Expr::AtomicStore { place, value, .. }
            | Expr::AtomicFetch { place, value, .. }
            | Expr::AtomicSwap { place, value, .. } => {
                place.ptr_expr().is_some_and(|ptr| ptr.reads_var(name)) || value.reads_var(name)
            }
            Expr::AtomicNew { value, .. } => value.reads_var(name),
            Expr::AtomicCompareExchange {
                place,
                expected,
                desired,
                ..
            } => {
                place.ptr_expr().is_some_and(|ptr| ptr.reads_var(name))
                    || expected.reads_var(name)
                    || desired.reads_var(name)
            }
            Expr::Binary { lhs, rhs, .. } => lhs.reads_var(name) || rhs.reads_var(name),
            Expr::Range { start, end } => start.reads_var(name) || end.reads_var(name),
            Expr::Call { func, args, .. } => {
                func.reads_var(name) || args.iter().any(|arg| arg.reads_var(name))
            }
            Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
                recv.reads_var(name) || args.iter().any(|arg| arg.reads_var(name))
            }
            Expr::Field { base, .. } | Expr::TupleField { base, .. } => base.reads_var(name),
            Expr::ArrayPtr { array, .. } => array.reads_var(name),
            Expr::Index { base, index } => base.reads_var(name) || index.reads_var(name),
            Expr::StructLit { fields, .. } => fields.iter().any(|(_, value)| value.reads_var(name)),
            Expr::TupleStructLit { fields, .. } | Expr::ArrayLit(fields) | Expr::VecLit(fields) => {
                fields.iter().any(|elem| elem.reads_var(name))
            }
            Expr::ArrayRepeat { elem, .. } => elem.reads_var(name),
            Expr::VecRepeat { elem, len } => elem.reads_var(name) || len.reads_var(name),
            Expr::Closure { params, body } => {
                !params.iter().any(|p| p.as_str() == name) && body.reads_var(name)
            }
            Expr::Macro { args, .. } => args.iter().any(|arg| arg.reads_var(name)),
            Expr::Match { expr, arms } => {
                expr.reads_var(name) || arms.iter().any(|arm| arm.value.reads_var(name))
            }
            Expr::If {
                cond,
                then_expr,
                else_expr,
            } => cond.reads_var(name) || then_expr.reads_var(name) || else_expr.reads_var(name),
        }
    }

    pub fn collect_vars(&self, out: &mut Vec<Ident>) {
        match self {
            Expr::Var(v) => out.push(*v),
            Expr::Value(_)
            | Expr::Str(_)
            | Expr::HexFloat(_)
            | Expr::ByteStr(_)
            | Expr::ConstBlock(_)
            | Expr::CStr(_)
            | Expr::Path(_)
            | Expr::AtomicFence { .. }
            | Expr::Todo(_) => {}
            Expr::Unary { expr, .. }
            | Expr::Cast { expr, .. }
            | Expr::Ref { expr, .. }
            | Expr::AddrOf { expr, .. }
            | Expr::Transmute { expr, .. } => expr.collect_vars(out),
            Expr::Block(block) | Expr::Unsafe(block) => {
                for stmt in &block.stmts {
                    stmt_collect_vars(&stmt.stmt, out);
                }
                if let Some(tail) = &block.tail {
                    tail.collect_vars(out);
                }
            }
            Expr::CopyNonoverlapping { src, dst, .. } => {
                src.collect_vars(out);
                dst.collect_vars(out);
            }
            Expr::PtrCopy {
                src, dst, count, ..
            } => {
                src.collect_vars(out);
                dst.collect_vars(out);
                count.collect_vars(out);
            }
            Expr::WriteBytes { dst, val, count } => {
                dst.collect_vars(out);
                val.collect_vars(out);
                count.collect_vars(out);
            }
            Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. } => {
                if let Some(ptr) = place.ptr_expr() {
                    ptr.collect_vars(out);
                }
            }
            Expr::AtomicStore { place, value, .. }
            | Expr::AtomicFetch { place, value, .. }
            | Expr::AtomicSwap { place, value, .. } => {
                if let Some(ptr) = place.ptr_expr() {
                    ptr.collect_vars(out);
                }
                value.collect_vars(out);
            }
            Expr::AtomicNew { value, .. } => value.collect_vars(out),
            Expr::AtomicCompareExchange {
                place,
                expected,
                desired,
                ..
            } => {
                if let Some(ptr) = place.ptr_expr() {
                    ptr.collect_vars(out);
                }
                expected.collect_vars(out);
                desired.collect_vars(out);
            }
            Expr::Binary { lhs, rhs, .. } => {
                lhs.collect_vars(out);
                rhs.collect_vars(out);
            }
            Expr::Range { start, end } => {
                start.collect_vars(out);
                end.collect_vars(out);
            }
            Expr::Call { func, args, .. } => {
                func.collect_vars(out);
                for arg in args {
                    arg.collect_vars(out);
                }
            }
            Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
                recv.collect_vars(out);
                for arg in args {
                    arg.collect_vars(out);
                }
            }
            Expr::Field { base, .. } | Expr::TupleField { base, .. } => base.collect_vars(out),
            Expr::ArrayPtr { array, .. } => array.collect_vars(out),
            Expr::Index { base, index } => {
                base.collect_vars(out);
                index.collect_vars(out);
            }
            Expr::StructLit { fields, .. } => {
                for (_, value) in fields {
                    value.collect_vars(out);
                }
            }
            Expr::TupleStructLit { fields, .. } | Expr::ArrayLit(fields) | Expr::VecLit(fields) => {
                for elem in fields {
                    elem.collect_vars(out);
                }
            }
            Expr::ArrayRepeat { elem, .. } => elem.collect_vars(out),
            Expr::VecRepeat { elem, len } => {
                elem.collect_vars(out);
                len.collect_vars(out);
            }
            Expr::Closure { params, body } => {
                let mut inner = Vec::new();
                body.collect_vars(&mut inner);
                out.extend(
                    inner
                        .into_iter()
                        .filter(|v| !params.iter().any(|p| p.as_str() == v.as_str())),
                );
            }
            Expr::Macro { args, .. } => {
                for arg in args {
                    arg.collect_vars(out);
                }
            }
            Expr::Match { expr, arms } => {
                expr.collect_vars(out);
                for arm in arms {
                    arm.value.collect_vars(out);
                }
            }
            Expr::If {
                cond,
                then_expr,
                else_expr,
            } => {
                cond.collect_vars(out);
                then_expr.collect_vars(out);
                else_expr.collect_vars(out);
            }
        }
    }

    pub fn collect_offset_calls<'a>(&'a self, out: &mut Vec<(&'a Ident, &'a str, &'a Expr)>) {
        if let Expr::MethodCall { recv, method, args } = self
            && let Expr::Var(recv_name) = &**recv
            && matches!(method.as_str(), "offset" | "add" | "wrapping_add")
            && let [index] = args.as_slice()
        {
            out.push((recv_name, method.as_str(), index));
        }
        match self {
            Expr::Var(_)
            | Expr::Value(_)
            | Expr::Str(_)
            | Expr::HexFloat(_)
            | Expr::ByteStr(_)
            | Expr::ConstBlock(_)
            | Expr::CStr(_)
            | Expr::Path(_)
            | Expr::AtomicFence { .. }
            | Expr::Todo(_) => {}
            Expr::Unary { expr, .. }
            | Expr::Cast { expr, .. }
            | Expr::Ref { expr, .. }
            | Expr::AddrOf { expr, .. }
            | Expr::Transmute { expr, .. } => expr.collect_offset_calls(out),
            Expr::Block(block) | Expr::Unsafe(block) => {
                for stmt in &block.stmts {
                    stmt_collect_offset_calls(&stmt.stmt, out);
                }
                if let Some(tail) = &block.tail {
                    tail.collect_offset_calls(out);
                }
            }
            Expr::CopyNonoverlapping { src, dst, .. } => {
                src.collect_offset_calls(out);
                dst.collect_offset_calls(out);
            }
            Expr::PtrCopy {
                src, dst, count, ..
            } => {
                src.collect_offset_calls(out);
                dst.collect_offset_calls(out);
                count.collect_offset_calls(out);
            }
            Expr::WriteBytes { dst, val, count } => {
                dst.collect_offset_calls(out);
                val.collect_offset_calls(out);
                count.collect_offset_calls(out);
            }
            Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. } => {
                if let Some(ptr) = place.ptr_expr() {
                    ptr.collect_offset_calls(out);
                }
            }
            Expr::AtomicStore { place, value, .. }
            | Expr::AtomicFetch { place, value, .. }
            | Expr::AtomicSwap { place, value, .. } => {
                if let Some(ptr) = place.ptr_expr() {
                    ptr.collect_offset_calls(out);
                }
                value.collect_offset_calls(out);
            }
            Expr::AtomicNew { value, .. } => value.collect_offset_calls(out),
            Expr::AtomicCompareExchange {
                place,
                expected,
                desired,
                ..
            } => {
                if let Some(ptr) = place.ptr_expr() {
                    ptr.collect_offset_calls(out);
                }
                expected.collect_offset_calls(out);
                desired.collect_offset_calls(out);
            }
            Expr::Binary { lhs, rhs, .. } => {
                lhs.collect_offset_calls(out);
                rhs.collect_offset_calls(out);
            }
            Expr::Range { start, end } => {
                start.collect_offset_calls(out);
                end.collect_offset_calls(out);
            }
            Expr::Call { func, args, .. } => {
                func.collect_offset_calls(out);
                for arg in args {
                    arg.collect_offset_calls(out);
                }
            }
            Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
                recv.collect_offset_calls(out);
                for arg in args {
                    arg.collect_offset_calls(out);
                }
            }
            Expr::Field { base, .. } | Expr::TupleField { base, .. } => {
                base.collect_offset_calls(out)
            }
            Expr::ArrayPtr { array, .. } => array.collect_offset_calls(out),
            Expr::Index { base, index } => {
                base.collect_offset_calls(out);
                index.collect_offset_calls(out);
            }
            Expr::StructLit { fields, .. } => {
                for (_, value) in fields {
                    value.collect_offset_calls(out);
                }
            }
            Expr::TupleStructLit { fields, .. } | Expr::ArrayLit(fields) | Expr::VecLit(fields) => {
                for elem in fields {
                    elem.collect_offset_calls(out);
                }
            }
            Expr::ArrayRepeat { elem, .. } => elem.collect_offset_calls(out),
            Expr::VecRepeat { elem, len } => {
                elem.collect_offset_calls(out);
                len.collect_offset_calls(out);
            }
            Expr::Closure { body, .. } => body.collect_offset_calls(out),
            Expr::Macro { args, .. } => {
                for arg in args {
                    arg.collect_offset_calls(out);
                }
            }
            Expr::Match { expr, arms } => {
                expr.collect_offset_calls(out);
                for arm in arms {
                    arm.value.collect_offset_calls(out);
                }
            }
            Expr::If {
                cond,
                then_expr,
                else_expr,
            } => {
                cond.collect_offset_calls(out);
                then_expr.collect_offset_calls(out);
                else_expr.collect_offset_calls(out);
            }
        }
    }

    pub fn collect_calls<'a>(&'a self, out: &mut Vec<(&'a Ident, &'a [Expr])>) {
        if let Expr::Call { func, args, .. } = self
            && let Expr::Var(callee) = &**func
        {
            out.push((callee, args));
        }
        match self {
            Expr::Var(_)
            | Expr::Value(_)
            | Expr::Str(_)
            | Expr::HexFloat(_)
            | Expr::ByteStr(_)
            | Expr::ConstBlock(_)
            | Expr::CStr(_)
            | Expr::Path(_)
            | Expr::AtomicFence { .. }
            | Expr::Todo(_) => {}
            Expr::Unary { expr, .. }
            | Expr::Cast { expr, .. }
            | Expr::Ref { expr, .. }
            | Expr::AddrOf { expr, .. }
            | Expr::Transmute { expr, .. } => expr.collect_calls(out),
            Expr::Block(block) | Expr::Unsafe(block) => {
                for stmt in &block.stmts {
                    stmt_collect_calls(&stmt.stmt, out);
                }
                if let Some(tail) = &block.tail {
                    tail.collect_calls(out);
                }
            }
            Expr::CopyNonoverlapping { src, dst, .. } => {
                src.collect_calls(out);
                dst.collect_calls(out);
            }
            Expr::PtrCopy {
                src, dst, count, ..
            } => {
                src.collect_calls(out);
                dst.collect_calls(out);
                count.collect_calls(out);
            }
            Expr::WriteBytes { dst, val, count } => {
                dst.collect_calls(out);
                val.collect_calls(out);
                count.collect_calls(out);
            }
            Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. } => {
                if let Some(ptr) = place.ptr_expr() {
                    ptr.collect_calls(out);
                }
            }
            Expr::AtomicStore { place, value, .. }
            | Expr::AtomicFetch { place, value, .. }
            | Expr::AtomicSwap { place, value, .. } => {
                if let Some(ptr) = place.ptr_expr() {
                    ptr.collect_calls(out);
                }
                value.collect_calls(out);
            }
            Expr::AtomicNew { value, .. } => value.collect_calls(out),
            Expr::AtomicCompareExchange {
                place,
                expected,
                desired,
                ..
            } => {
                if let Some(ptr) = place.ptr_expr() {
                    ptr.collect_calls(out);
                }
                expected.collect_calls(out);
                desired.collect_calls(out);
            }
            Expr::Binary { lhs, rhs, .. } => {
                lhs.collect_calls(out);
                rhs.collect_calls(out);
            }
            Expr::Range { start, end } => {
                start.collect_calls(out);
                end.collect_calls(out);
            }
            Expr::Call { func, args, .. } => {
                func.collect_calls(out);
                for arg in args {
                    arg.collect_calls(out);
                }
            }
            Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
                recv.collect_calls(out);
                for arg in args {
                    arg.collect_calls(out);
                }
            }
            Expr::Field { base, .. } | Expr::TupleField { base, .. } => base.collect_calls(out),
            Expr::ArrayPtr { array, .. } => array.collect_calls(out),
            Expr::Index { base, index } => {
                base.collect_calls(out);
                index.collect_calls(out);
            }
            Expr::StructLit { fields, .. } => {
                for (_, value) in fields {
                    value.collect_calls(out);
                }
            }
            Expr::TupleStructLit { fields, .. } | Expr::ArrayLit(fields) | Expr::VecLit(fields) => {
                for elem in fields {
                    elem.collect_calls(out);
                }
            }
            Expr::ArrayRepeat { elem, .. } => elem.collect_calls(out),
            Expr::VecRepeat { elem, len } => {
                elem.collect_calls(out);
                len.collect_calls(out);
            }
            Expr::Closure { body, .. } => body.collect_calls(out),
            Expr::Macro { args, .. } => {
                for arg in args {
                    arg.collect_calls(out);
                }
            }
            Expr::Match { expr, arms } => {
                expr.collect_calls(out);
                for arm in arms {
                    arm.value.collect_calls(out);
                }
            }
            Expr::If {
                cond,
                then_expr,
                else_expr,
            } => {
                cond.collect_calls(out);
                then_expr.collect_calls(out);
                else_expr.collect_calls(out);
            }
        }
    }

    pub fn substitute_var(&mut self, name: &str, replacement: &Expr) -> bool {
        match self {
            Expr::Var(v) if v.as_str() == name => {
                *self = replacement.clone();
                true
            }
            Expr::Value(_)
            | Expr::Str(_)
            | Expr::HexFloat(_)
            | Expr::ByteStr(_)
            | Expr::ConstBlock(_)
            | Expr::CStr(_)
            | Expr::Var(_)
            | Expr::Path(_) => false,
            Expr::Unary { expr, .. }
            | Expr::Cast { expr, .. }
            | Expr::Ref { expr, .. }
            | Expr::AddrOf { expr, .. }
            | Expr::Transmute { expr, .. } => expr.substitute_var(name, replacement),
            Expr::Block(block) | Expr::Unsafe(block) => {
                let mut changed = false;
                for stmt in &mut block.stmts {
                    changed |= stmt_substitute_var(&mut stmt.stmt, name, replacement);
                }
                if let Some(tail) = &mut block.tail {
                    changed |= tail.substitute_var(name, replacement);
                }
                changed
            }
            Expr::CopyNonoverlapping { src, dst, .. } => {
                let s = src.substitute_var(name, replacement);
                let d = dst.substitute_var(name, replacement);
                s || d
            }
            Expr::PtrCopy {
                src, dst, count, ..
            } => {
                let s = src.substitute_var(name, replacement);
                let d = dst.substitute_var(name, replacement);
                let c = count.substitute_var(name, replacement);
                s || d || c
            }
            Expr::WriteBytes { dst, val, count } => {
                let d = dst.substitute_var(name, replacement);
                let v = val.substitute_var(name, replacement);
                let c = count.substitute_var(name, replacement);
                d || v || c
            }
            Expr::AtomicFence { .. } | Expr::Todo(_) => false,
            Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. } => place
                .ptr_expr_mut()
                .is_some_and(|ptr| ptr.substitute_var(name, replacement)),
            Expr::AtomicStore { place, value, .. }
            | Expr::AtomicFetch { place, value, .. }
            | Expr::AtomicSwap { place, value, .. } => {
                let p = place
                    .ptr_expr_mut()
                    .is_some_and(|ptr| ptr.substitute_var(name, replacement));
                let v = value.substitute_var(name, replacement);
                p || v
            }
            Expr::AtomicNew { value, .. } => value.substitute_var(name, replacement),
            Expr::AtomicCompareExchange {
                place,
                expected,
                desired,
                ..
            } => {
                let p = place
                    .ptr_expr_mut()
                    .is_some_and(|ptr| ptr.substitute_var(name, replacement));
                let e = expected.substitute_var(name, replacement);
                let d = desired.substitute_var(name, replacement);
                p || e || d
            }
            Expr::Binary { lhs, rhs, .. } => {
                let l = lhs.substitute_var(name, replacement);
                let r = rhs.substitute_var(name, replacement);
                l || r
            }
            Expr::Range { start, end } => {
                let s = start.substitute_var(name, replacement);
                let e = end.substitute_var(name, replacement);
                s || e
            }
            Expr::Call { func, args, .. } => {
                let mut changed = func.substitute_var(name, replacement);
                for arg in args {
                    changed |= arg.substitute_var(name, replacement);
                }
                changed
            }
            Expr::MethodCall { recv, args, .. } => {
                let mut changed = recv.substitute_var(name, replacement);
                for arg in args {
                    changed |= arg.substitute_var(name, replacement);
                }
                changed
            }
            Expr::MethodCallGeneric { recv, args, .. } => {
                let mut changed = recv.substitute_var(name, replacement);
                for arg in args {
                    changed |= arg.substitute_var(name, replacement);
                }
                changed
            }
            Expr::Field { base, .. } | Expr::TupleField { base, .. } => {
                base.substitute_var(name, replacement)
            }
            Expr::ArrayPtr { array, .. } => array.substitute_var(name, replacement),
            Expr::Index { base, index } => {
                let b = base.substitute_var(name, replacement);
                let i = index.substitute_var(name, replacement);
                b || i
            }
            Expr::StructLit { fields, .. } => {
                let mut changed = false;
                for (_, value) in fields {
                    changed |= value.substitute_var(name, replacement);
                }
                changed
            }
            Expr::TupleStructLit { fields, .. } => {
                let mut changed = false;
                for value in fields {
                    changed |= value.substitute_var(name, replacement);
                }
                changed
            }
            Expr::ArrayLit(elems) => {
                let mut changed = false;
                for elem in elems {
                    changed |= elem.substitute_var(name, replacement);
                }
                changed
            }
            Expr::ArrayRepeat { elem, .. } => elem.substitute_var(name, replacement),
            Expr::VecLit(elems) => {
                let mut changed = false;
                for elem in elems {
                    changed |= elem.substitute_var(name, replacement);
                }
                changed
            }
            Expr::VecRepeat { elem, len } => {
                let e = elem.substitute_var(name, replacement);
                let l = len.substitute_var(name, replacement);
                e || l
            }
            Expr::Closure { params, body } => {
                if params.iter().any(|p| p.as_str() == name) {
                    false
                } else {
                    body.substitute_var(name, replacement)
                }
            }
            Expr::Macro { args, .. } => {
                let mut changed = false;
                for arg in args {
                    changed |= arg.substitute_var(name, replacement);
                }
                changed
            }
            Expr::Match { expr, arms } => {
                let mut changed = expr.substitute_var(name, replacement);
                for arm in arms {
                    changed |= arm.value.substitute_var(name, replacement);
                }
                changed
            }
            Expr::If {
                cond,
                then_expr,
                else_expr,
            } => {
                let c = cond.substitute_var(name, replacement);
                let t = then_expr.substitute_var(name, replacement);
                let e = else_expr.substitute_var(name, replacement);
                c || t || e
            }
        }
    }
}

fn stmt_reads_var(stmt: &Stmt, name: &str) -> bool {
    match stmt {
        Stmt::Let {
            init: Some(expr), ..
        } => expr.reads_var(name),
        Stmt::Let { init: None, .. } => false,
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            cond.reads_var(name)
                || then_body
                    .iter()
                    .any(|stmt| stmt_reads_var(&stmt.stmt, name))
                || then_value.reads_var(name)
                || else_body
                    .iter()
                    .any(|stmt| stmt_reads_var(&stmt.stmt, name))
                || else_value.reads_var(name)
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            target.reads_var(name) || value.reads_var(name)
        }
        Stmt::InlineAsm(asm) => {
            let mut found = false;
            for operand in &asm.operands {
                operand.visit_exprs(&mut |expr| {
                    found |= expr.reads_var(name);
                });
            }
            found
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => expr.reads_var(name),
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => false,
        Stmt::For { iter, body, pat } => {
            (pat != name && iter.reads_var(name))
                || (pat != name && body.iter().any(|stmt| stmt_reads_var(&stmt.stmt, name)))
        }
        Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } | Stmt::Loop { body, .. } => {
            body.iter().any(|stmt| stmt_reads_var(&stmt.stmt, name))
        }
        Stmt::Unsafe { body } => {
            body.stmts
                .iter()
                .any(|stmt| stmt_reads_var(&stmt.stmt, name))
                || body.tail.as_ref().is_some_and(|tail| tail.reads_var(name))
        }
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            cond.reads_var(name)
                || then_body
                    .iter()
                    .chain(else_body.iter())
                    .any(|stmt| stmt_reads_var(&stmt.stmt, name))
        }
        Stmt::Match { expr, arms } => {
            expr.reads_var(name)
                || arms.iter().any(|arm| {
                    matches!(&arm.pattern, Pattern::Guarded { cond, .. } if cond.reads_var(name))
                        || arm.body.iter().any(|stmt| stmt_reads_var(&stmt.stmt, name))
                })
        }
        Stmt::While { cond, body } => {
            cond.reads_var(name)
                || body
                    .stmts
                    .iter()
                    .any(|stmt| stmt_reads_var(&stmt.stmt, name))
                || body.tail.as_ref().is_some_and(|tail| tail.reads_var(name))
        }
        Stmt::Block(body) => {
            body.stmts
                .iter()
                .any(|stmt| stmt_reads_var(&stmt.stmt, name))
                || body.tail.as_ref().is_some_and(|tail| tail.reads_var(name))
        }
    }
}

fn stmt_collect_offset_calls<'a>(stmt: &'a Stmt, out: &mut Vec<(&'a Ident, &'a str, &'a Expr)>) {
    match stmt {
        Stmt::Let {
            init: Some(expr), ..
        } => expr.collect_offset_calls(out),
        Stmt::Let { init: None, .. } => {}
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            cond.collect_offset_calls(out);
            for stmt in then_body {
                stmt_collect_offset_calls(&stmt.stmt, out);
            }
            then_value.collect_offset_calls(out);
            for stmt in else_body {
                stmt_collect_offset_calls(&stmt.stmt, out);
            }
            else_value.collect_offset_calls(out);
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            target.collect_offset_calls(out);
            value.collect_offset_calls(out);
        }
        Stmt::InlineAsm(_) => {}
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => expr.collect_offset_calls(out),
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
        Stmt::For { iter, body, .. } => {
            iter.collect_offset_calls(out);
            for stmt in body {
                stmt_collect_offset_calls(&stmt.stmt, out);
            }
        }
        Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } | Stmt::Loop { body, .. } => {
            for stmt in body {
                stmt_collect_offset_calls(&stmt.stmt, out);
            }
        }
        Stmt::Unsafe { body } => {
            for stmt in &body.stmts {
                stmt_collect_offset_calls(&stmt.stmt, out);
            }
            if let Some(tail) = &body.tail {
                tail.collect_offset_calls(out);
            }
        }
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            cond.collect_offset_calls(out);
            for stmt in then_body.iter().chain(else_body.iter()) {
                stmt_collect_offset_calls(&stmt.stmt, out);
            }
        }
        Stmt::Match { expr, arms } => {
            expr.collect_offset_calls(out);
            for arm in arms {
                if let Pattern::Guarded { cond, .. } = &arm.pattern {
                    cond.collect_offset_calls(out);
                }
                for stmt in &arm.body {
                    stmt_collect_offset_calls(&stmt.stmt, out);
                }
            }
        }
        Stmt::While { cond, body } => {
            cond.collect_offset_calls(out);
            for stmt in &body.stmts {
                stmt_collect_offset_calls(&stmt.stmt, out);
            }
            if let Some(tail) = &body.tail {
                tail.collect_offset_calls(out);
            }
        }
        Stmt::Block(body) => {
            for stmt in &body.stmts {
                stmt_collect_offset_calls(&stmt.stmt, out);
            }
            if let Some(tail) = &body.tail {
                tail.collect_offset_calls(out);
            }
        }
    }
}

fn stmt_collect_calls<'a>(stmt: &'a Stmt, out: &mut Vec<(&'a Ident, &'a [Expr])>) {
    match stmt {
        Stmt::Let {
            init: Some(expr), ..
        } => expr.collect_calls(out),
        Stmt::Let { init: None, .. } => {}
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            cond.collect_calls(out);
            for stmt in then_body {
                stmt_collect_calls(&stmt.stmt, out);
            }
            then_value.collect_calls(out);
            for stmt in else_body {
                stmt_collect_calls(&stmt.stmt, out);
            }
            else_value.collect_calls(out);
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            target.collect_calls(out);
            value.collect_calls(out);
        }
        Stmt::InlineAsm(_) => {}
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => expr.collect_calls(out),
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
        Stmt::For { iter, body, .. } => {
            iter.collect_calls(out);
            for stmt in body {
                stmt_collect_calls(&stmt.stmt, out);
            }
        }
        Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } | Stmt::Loop { body, .. } => {
            for stmt in body {
                stmt_collect_calls(&stmt.stmt, out);
            }
        }
        Stmt::Unsafe { body } => {
            for stmt in &body.stmts {
                stmt_collect_calls(&stmt.stmt, out);
            }
            if let Some(tail) = &body.tail {
                tail.collect_calls(out);
            }
        }
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            cond.collect_calls(out);
            for stmt in then_body.iter().chain(else_body.iter()) {
                stmt_collect_calls(&stmt.stmt, out);
            }
        }
        Stmt::Match { expr, arms } => {
            expr.collect_calls(out);
            for arm in arms {
                if let Pattern::Guarded { cond, .. } = &arm.pattern {
                    cond.collect_calls(out);
                }
                for stmt in &arm.body {
                    stmt_collect_calls(&stmt.stmt, out);
                }
            }
        }
        Stmt::While { cond, body } => {
            cond.collect_calls(out);
            for stmt in &body.stmts {
                stmt_collect_calls(&stmt.stmt, out);
            }
            if let Some(tail) = &body.tail {
                tail.collect_calls(out);
            }
        }
        Stmt::Block(body) => {
            for stmt in &body.stmts {
                stmt_collect_calls(&stmt.stmt, out);
            }
            if let Some(tail) = &body.tail {
                tail.collect_calls(out);
            }
        }
    }
}

fn stmt_collect_vars(stmt: &Stmt, out: &mut Vec<Ident>) {
    match stmt {
        Stmt::Let {
            init: Some(expr), ..
        } => expr.collect_vars(out),
        Stmt::Let { init: None, .. } => {}
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            cond.collect_vars(out);
            for stmt in then_body {
                stmt_collect_vars(&stmt.stmt, out);
            }
            then_value.collect_vars(out);
            for stmt in else_body {
                stmt_collect_vars(&stmt.stmt, out);
            }
            else_value.collect_vars(out);
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            target.collect_vars(out);
            value.collect_vars(out);
        }
        Stmt::InlineAsm(asm) => {
            for operand in &asm.operands {
                operand.visit_exprs(&mut |expr| expr.collect_vars(out));
            }
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => expr.collect_vars(out),
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
        Stmt::For { iter, body, pat } => {
            iter.collect_vars(out);
            let mut inner = Vec::new();
            for stmt in body {
                stmt_collect_vars(&stmt.stmt, &mut inner);
            }
            out.extend(inner.into_iter().filter(|v| v.as_str() != pat.as_str()));
        }
        Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } | Stmt::Loop { body, .. } => {
            for stmt in body {
                stmt_collect_vars(&stmt.stmt, out);
            }
        }
        Stmt::Unsafe { body } => {
            for stmt in &body.stmts {
                stmt_collect_vars(&stmt.stmt, out);
            }
            if let Some(tail) = &body.tail {
                tail.collect_vars(out);
            }
        }
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            cond.collect_vars(out);
            for stmt in then_body.iter().chain(else_body.iter()) {
                stmt_collect_vars(&stmt.stmt, out);
            }
        }
        Stmt::Match { expr, arms } => {
            expr.collect_vars(out);
            for arm in arms {
                if let Pattern::Guarded { cond, .. } = &arm.pattern {
                    cond.collect_vars(out);
                }
                for stmt in &arm.body {
                    stmt_collect_vars(&stmt.stmt, out);
                }
            }
        }
        Stmt::While { cond, body } => {
            cond.collect_vars(out);
            for stmt in &body.stmts {
                stmt_collect_vars(&stmt.stmt, out);
            }
            if let Some(tail) = &body.tail {
                tail.collect_vars(out);
            }
        }
        Stmt::Block(body) => {
            for stmt in &body.stmts {
                stmt_collect_vars(&stmt.stmt, out);
            }
            if let Some(tail) = &body.tail {
                tail.collect_vars(out);
            }
        }
    }
}

fn stmt_substitute_var(stmt: &mut Stmt, name: &str, replacement: &Expr) -> bool {
    match stmt {
        Stmt::Let {
            init: Some(expr), ..
        } => expr.substitute_var(name, replacement),
        Stmt::Let { init: None, .. } => false,
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            let mut changed = cond.substitute_var(name, replacement);
            for stmt in then_body.iter_mut().chain(else_body.iter_mut()) {
                changed |= stmt_substitute_var(&mut stmt.stmt, name, replacement);
            }
            changed |= then_value.substitute_var(name, replacement);
            changed |= else_value.substitute_var(name, replacement);
            changed
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            let t = target.substitute_var(name, replacement);
            let v = value.substitute_var(name, replacement);
            t || v
        }
        Stmt::InlineAsm(asm) => {
            let mut changed = false;
            for operand in &mut asm.operands {
                operand.visit_exprs_mut(&mut |expr| {
                    changed |= expr.substitute_var(name, replacement);
                });
            }
            changed
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => expr.substitute_var(name, replacement),
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => false,
        Stmt::For { iter, body, pat } => {
            let mut changed = if pat == name {
                false
            } else {
                iter.substitute_var(name, replacement)
            };
            if pat != name {
                for stmt in body {
                    changed |= stmt_substitute_var(&mut stmt.stmt, name, replacement);
                }
            }
            changed
        }
        Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            let mut changed = false;
            for stmt in body {
                changed |= stmt_substitute_var(&mut stmt.stmt, name, replacement);
            }
            changed
        }
        Stmt::Unsafe { body } => {
            let mut changed = false;
            for stmt in &mut body.stmts {
                changed |= stmt_substitute_var(&mut stmt.stmt, name, replacement);
            }
            if let Some(tail) = &mut body.tail {
                changed |= tail.substitute_var(name, replacement);
            }
            changed
        }
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            let mut changed = cond.substitute_var(name, replacement);
            for stmt in then_body.iter_mut().chain(else_body.iter_mut()) {
                changed |= stmt_substitute_var(&mut stmt.stmt, name, replacement);
            }
            changed
        }
        Stmt::Loop { body, .. } => {
            let mut changed = false;
            for stmt in body {
                changed |= stmt_substitute_var(&mut stmt.stmt, name, replacement);
            }
            changed
        }
        Stmt::Match { expr, arms } => {
            let mut changed = expr.substitute_var(name, replacement);
            for arm in arms {
                for stmt in &mut arm.body {
                    changed |= stmt_substitute_var(&mut stmt.stmt, name, replacement);
                }
            }
            changed
        }
        Stmt::While { cond, body } => {
            let mut changed = cond.substitute_var(name, replacement);
            for stmt in &mut body.stmts {
                changed |= stmt_substitute_var(&mut stmt.stmt, name, replacement);
            }
            if let Some(tail) = &mut body.tail {
                changed |= tail.substitute_var(name, replacement);
            }
            changed
        }
        Stmt::Block(body) => {
            let mut changed = false;
            for stmt in &mut body.stmts {
                changed |= stmt_substitute_var(&mut stmt.stmt, name, replacement);
            }
            if let Some(tail) = &mut body.tail {
                changed |= tail.substitute_var(name, replacement);
            }
            changed
        }
    }
}

impl Type {
    pub fn is_unit(&self) -> bool {
        matches!(self, Type::Unit)
    }

    pub fn aligned_inner(&self) -> Option<&Type> {
        match self {
            Type::Generic { name, args } if name == "aligned::Aligned" && args.len() == 2 => {
                args.get(1)
            }
            _ => None,
        }
    }

    pub fn peel_aligned(&self) -> &Type {
        self.aligned_inner().unwrap_or(self)
    }

    pub fn render(&self) -> String {
        crate::backend::codegen::type_to_string(self)
    }

    pub fn parse(s: &str) -> Type {
        let s = s.trim();
        if let Some(rest) = s.strip_prefix("*mut ") {
            return Type::Ptr {
                mutable: true,
                inner: Box::new(Type::parse(rest)),
            };
        }
        if let Some(rest) = s.strip_prefix("*const ") {
            return Type::Ptr {
                mutable: false,
                inner: Box::new(Type::parse(rest)),
            };
        }
        if let Some(rest) = s.strip_prefix("&mut ") {
            return Type::Ref {
                mutable: true,
                inner: Box::new(Type::parse(rest)),
            };
        }
        if let Some(rest) = s.strip_prefix('&') {
            return Type::Ref {
                mutable: false,
                inner: Box::new(Type::parse(rest)),
            };
        }
        if let Some(rest) = s.strip_prefix('[').and_then(|s| s.strip_suffix(']'))
            && let Some((elem, len)) = rest.rsplit_once(';')
            && let Ok(len) = len.trim().parse()
        {
            return Type::Array {
                elem: Box::new(Type::parse(elem.trim())),
                len,
            };
        }
        if let Some(rest) = s.strip_prefix('[').and_then(|s| s.strip_suffix(']')) {
            return Type::Slice(Box::new(Type::parse(rest)));
        }
        if s == "()" {
            return Type::Unit;
        }
        if s == "core::ffi::VaList<'_>" {
            return Type::VaList;
        }
        if s == "str" {
            return Type::Str;
        }
        match Prim::parse(s) {
            Some(p) => Type::Prim(p),
            None => Type::Custom(s.to_string()),
        }
    }
}
