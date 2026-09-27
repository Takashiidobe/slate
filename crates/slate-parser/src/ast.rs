use crate::const_expr::{
    AssignOp, BinaryOp, CharLiteral, FloatLiteral, IntegerLiteral, PostfixOp, StringLiteral,
    UnaryOp,
};
use custom_debug::Debug as CustomDebug;
use std::cell::Cell;
use std::rc::Rc;
use std::sync::atomic::{AtomicU32, Ordering};

pub(crate) fn is_false(value: &bool) -> bool {
    !*value
}

fn encoding_prefix(encoding: crate::const_expr::Encoding, string: bool) -> &'static str {
    use crate::const_expr::Encoding;
    match (encoding, string) {
        (Encoding::Plain, _) => "",
        (Encoding::Utf8, _) => "u8",
        (Encoding::Utf16, _) => "u",
        (Encoding::Utf32, _) => "U",
        (Encoding::Wide, _) => "L",
    }
}

pub type Expr = Box<Span<ExprKind>>;
pub type Stmt = Span<StmtKind>;
pub type FieldItem = Span<FieldItemKind>;
pub type EnumItem = Span<EnumItemKind>;
pub type Decl = Span<DeclKind>;

#[derive(Debug, Clone, PartialEq)]
pub struct Pragma {
    pub kind: PragmaKind,
}

#[derive(CustomDebug, Clone, PartialEq)]
pub enum PragmaKind {
    Pack {
        action: PragmaStackAction,
        #[debug(skip_if = Option::is_none)]
        label: Option<String>,
        alignment: Option<Expr>,
    },
    Weak {
        name: String,
        alias: Option<String>,
    },
    Visibility {
        action: PragmaStackAction,
        visibility: Option<String>,
    },
    Stdc {
        option: StdcPragmaOption,
        value: StdcPragmaValue,
    },
    FloatControl(FloatControl),
    MsStruct {
        action: MsStructAction,
    },
    Opaque(String),
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum PragmaStackAction {
    Push,
    Pop,
    Show,
    Set,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum MsStructAction {
    On,
    Off,
    Reset,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum StdcPragmaOption {
    FenvAccess,
    FpContract,
    CxLimitedRange,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum StdcPragmaValue {
    On,
    Off,
    Default,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum FloatControlOption {
    Precise,
    Except,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum FloatControl {
    Set {
        option: FloatControlOption,
        enabled: bool,
        push: bool,
    },
    Push,
    Pop,
    Malformed,
}

#[derive(CustomDebug, Clone, PartialEq)]
pub enum ExprKind {
    Identifier(String),
    IntegerLiteral(IntegerLiteral),
    FloatLiteral(FloatLiteral),
    CharLiteral(CharLiteral),
    StringLiteral(StringLiteral),
    Paren(Expr),
    Unary {
        op: UnaryOp,
        operand: Expr,
    },
    Postfix {
        op: PostfixOp,
        operand: Expr,
    },
    Binary {
        op: BinaryOp,
        left: Expr,
        right: Expr,
    },
    Assign {
        op: AssignOp,
        target: Expr,
        value: Expr,
    },
    Conditional {
        condition: Expr,
        #[debug(skip_if = Option::is_none)]
        then_value: Option<Expr>,
        else_value: Expr,
    },
    Comma {
        left: Expr,
        right: Expr,
    },
    Call {
        callee: Expr,
        arguments: Vec<Expr>,
    },
    Member {
        base: Expr,
        field: Span<String>,
        #[debug(skip_if = is_false)]
        arrow: bool,
    },
    Index {
        base: Expr,
        index: Expr,
    },
    Cast {
        ty: Box<TypeName>,
        value: Expr,
    },
    CompoundLiteral {
        ty: Box<TypeName>,
        initializer: Vec<InitializerItem>,
    },
    SizeOfExpr(Expr),
    SizeOfType {
        ty: Box<TypeName>,
    },
    AlignOf {
        ty: Box<TypeName>,
    },
    AlignOfExpr(Expr),
    OffsetOf {
        ty: Box<TypeName>,
        member: Expr,
    },
    Generic {
        controlling: GenericControl,
        associations: Vec<GenericAssociation>,
    },
    VaArg {
        list: Expr,
        ty: Box<TypeName>,
    },
    TypesCompatible {
        left_ty: Box<TypeName>,
        right_ty: Box<TypeName>,
    },
    BitCast {
        ty: Box<TypeName>,
        value: Expr,
    },
    ConvertVector {
        ty: Box<TypeName>,
        value: Expr,
    },
    LabelAddress(Span<String>),
    StatementExpression(Vec<Stmt>),
    BoolLiteral(bool),
    NullPtrLiteral,
}

#[derive(Debug, Clone, PartialEq)]
pub enum GenericControl {
    Expr(Expr),
    Type { ty: Box<TypeName> },
}

#[derive(Debug, Clone, PartialEq)]
pub enum GenericAssociation {
    Type { ty: Box<TypeName>, value: Expr },
    Default(Expr),
}

impl std::fmt::Display for ExprKind {
    fn fmt(&self, formatter: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::Identifier(value) => formatter.write_str(value),
            Self::IntegerLiteral(value) => formatter.write_str(&value.spelling),
            Self::FloatLiteral(value) => formatter.write_str(&value.spelling),
            Self::CharLiteral(value) => write!(
                formatter,
                "{}'{}'",
                encoding_prefix(value.encoding, false),
                value.spelling
            ),
            Self::StringLiteral(value) => {
                write!(formatter, "{}\"", encoding_prefix(value.encoding, true))?;
                for piece in &value.pieces {
                    formatter.write_str(&piece.value)?;
                }
                formatter.write_str("\"")
            }
            Self::Paren(value) => write!(formatter, "({value})"),
            Self::Unary { op, operand } => write!(formatter, "{}{operand}", <&str>::from(*op)),
            Self::Postfix { op, operand } => write!(formatter, "{operand}{}", <&str>::from(*op)),
            Self::Binary { op, left, right } => {
                write!(formatter, "{left} {} {right}", <&str>::from(*op))
            }
            Self::Assign { op, target, value } => {
                write!(formatter, "{target} {} {value}", <&str>::from(*op))
            }
            Self::Conditional {
                condition,
                then_value: Some(then_value),
                else_value,
            } => write!(formatter, "{condition} ? {then_value} : {else_value}"),
            Self::Conditional {
                condition,
                then_value: None,
                else_value,
            } => write!(formatter, "{condition} ?: {else_value}"),
            Self::Comma { left, right } => write!(formatter, "{left}, {right}"),
            Self::Call { callee, arguments } => {
                write!(formatter, "{callee}(")?;
                for (index, argument) in arguments.iter().enumerate() {
                    if index > 0 {
                        write!(formatter, ", ")?;
                    }
                    write!(formatter, "{argument}")?;
                }
                write!(formatter, ")")
            }
            Self::Member { base, field, arrow } => {
                let access = if *arrow { "->" } else { "." };
                write!(formatter, "{base}{access}{field}")
            }
            Self::Index { base, index } => write!(formatter, "{base}[{index}]"),
            Self::Cast { value, .. } => write!(formatter, "(cast){value}"),
            Self::CompoundLiteral { .. } => write!(formatter, "(compound literal)"),
            Self::SizeOfExpr(value) => write!(formatter, "sizeof {value}"),
            Self::SizeOfType { .. } => write!(formatter, "sizeof(...)"),
            Self::AlignOf { .. } => write!(formatter, "_Alignof(...)"),
            Self::AlignOfExpr(value) => write!(formatter, "_Alignof {value}"),
            Self::OffsetOf { member, .. } => write!(formatter, "__builtin_offsetof(..., {member})"),
            Self::Generic { .. } => formatter.write_str("_Generic(...)"),
            Self::VaArg { list, .. } => write!(formatter, "__builtin_va_arg({list}, ...)"),
            Self::TypesCompatible { .. } => {
                formatter.write_str("__builtin_types_compatible_p(...)")
            }
            Self::BitCast { value, .. } => write!(formatter, "__builtin_bit_cast(..., {value})"),
            Self::ConvertVector { value, .. } => {
                write!(formatter, "__builtin_convertvector({value}, ...)")
            }
            Self::LabelAddress(label) => write!(formatter, "&&{label}"),
            Self::StatementExpression(_) => formatter.write_str("({ ... })"),
            Self::BoolLiteral(true) => formatter.write_str("true"),
            Self::BoolLiteral(false) => formatter.write_str("false"),
            Self::NullPtrLiteral => formatter.write_str("nullptr"),
        }
    }
}

#[derive(Debug, Clone, PartialEq)]
pub enum Designator {
    Array(Expr),
    ArrayRange { start: Expr, end: Expr },
    Field(Span<String>),
}

#[derive(Debug, Clone, PartialEq)]
pub struct InitializerItem {
    pub designators: Vec<Designator>,
    pub value: Initializer,
}

#[derive(Debug, Clone, PartialEq)]
pub enum Initializer {
    Expr(Expr),
    List(Vec<InitializerItem>),
}

#[derive(Debug, Clone, PartialEq)]
pub enum StmtKind {
    Comment(CommentGroup),
    Null,
    Return(Expr),
    ReturnVoid,
    Expr(Expr),
    Decl(Declaration),
    StaticAssert(StaticAssert),
    Attribute(Vec<Span<Attribute>>),
    Attributed {
        attributes: Vec<Span<Attribute>>,
        body: Box<Stmt>,
    },
    Block(Vec<Stmt>),
    If {
        condition: Expr,
        then_branch: Box<Stmt>,
        else_branch: Option<Box<Stmt>>,
    },
    While {
        condition: Expr,
        body: Box<Stmt>,
    },
    DoWhile {
        body: Box<Stmt>,
        condition: Expr,
    },
    For {
        init: Option<Box<Stmt>>,
        condition: Option<Expr>,
        increment: Option<Expr>,
        body: Box<Stmt>,
    },
    Switch {
        discriminant: Expr,
        body: Box<Stmt>,
    },
    Labeled {
        label: Span<String>,
        body: Box<Stmt>,
    },
    SwitchLabel {
        label: SwitchLabel,
        body: Box<Stmt>,
    },
    LocalLabelDecl(Vec<Span<String>>),
    Asm(GnuAsm),
    Goto(Span<String>),
    ComputedGoto(Expr),
    NestedFunction(Box<FunctionDefinition>),
    Pragma(Pragma),
    Break,
    Continue,
}

#[derive(Debug, Clone, PartialEq)]
pub enum SwitchLabel {
    Case(Expr),
    CaseRange { start: Expr, end: Expr },
    Default,
}

#[derive(Default, Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub struct FileId(pub u32);

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct Loc {
    pub file: FileId,
    pub offset: usize,
    pub length: usize,
}

impl Loc {
    pub fn new(file: FileId, offset: usize, length: usize) -> Self {
        Self {
            file,
            offset,
            length,
        }
    }

    pub fn through(self, end: Self) -> Self {
        if self.file != end.file {
            return self;
        }
        let end_offset = end.offset.saturating_add(end.length);
        Self::new(
            self.file,
            self.offset.min(end.offset),
            end_offset.saturating_sub(self.offset.min(end.offset)),
        )
    }
}

#[derive(Default, Debug, Clone, Copy, PartialEq, Eq, Hash, PartialOrd, Ord)]
pub struct NodeId(pub u32);

static NEXT_NODE_ID: AtomicU32 = AtomicU32::new(0);

impl NodeId {
    fn next() -> Self {
        Self(NEXT_NODE_ID.fetch_add(1, Ordering::Relaxed))
    }
}

thread_local! {
    static SHOW_NODE_IDS: Cell<bool> = const { Cell::new(false) };
}

pub fn set_show_node_ids(show: bool) {
    SHOW_NODE_IDS.with(|cell| cell.set(show));
}

#[derive(Clone, PartialEq, Eq)]
pub struct Span<T> {
    pub id: NodeId,
    pub value: T,
    pub spelling: Loc,
    pub expansion: Loc,
    pub provenance: Provenance,
    pub macro_origin: Option<Rc<MacroOrigin>>,
    pub leading_space: bool,
}

impl<T: std::fmt::Debug> std::fmt::Debug for Span<T> {
    fn fmt(&self, formatter: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        if SHOW_NODE_IDS.with(Cell::get) {
            write!(formatter, "#{} ", self.id.0)?;
        }
        if self.provenance.system_header_is_none() {
            self.value.fmt(formatter)
        } else {
            formatter
                .debug_struct("Spanned")
                .field("value", &self.value)
                .field("provenance", &self.provenance)
                .finish()
        }
    }
}

impl<T> Span<T> {
    pub fn new(value: T, spelling: Loc, expansion: Loc) -> Self {
        Self {
            id: NodeId::next(),
            value,
            spelling,
            expansion,
            provenance: Provenance::default(),
            macro_origin: None,
            leading_space: false,
        }
    }

    pub fn with_leading_space(mut self, leading_space: bool) -> Self {
        self.leading_space = leading_space;
        self
    }

    pub fn with_macro_origin(mut self, macro_origin: Option<Rc<MacroOrigin>>) -> Self {
        self.macro_origin = macro_origin;
        self
    }

    pub fn with_provenance(mut self, provenance: Provenance) -> Self {
        self.provenance = provenance;
        self
    }
}

impl<T> std::ops::Deref for Span<T> {
    type Target = T;

    fn deref(&self) -> &Self::Target {
        &self.value
    }
}

impl<T: std::fmt::Display> std::fmt::Display for Span<T> {
    fn fmt(&self, formatter: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        self.value.fmt(formatter)
    }
}

impl<T> Span<T> {
    pub fn with_value<U>(self, value: U) -> Span<U> {
        Span {
            id: self.id,
            value,
            spelling: self.spelling,
            expansion: self.expansion,
            provenance: self.provenance,
            macro_origin: self.macro_origin,
            leading_space: self.leading_space,
        }
    }

    pub fn derive<U>(&self, value: U) -> Span<U> {
        Span {
            id: NodeId::next(),
            value,
            spelling: self.spelling,
            expansion: self.expansion,
            provenance: self.provenance,
            macro_origin: self.macro_origin.clone(),
            leading_space: self.leading_space,
        }
    }

    pub fn map<U>(self, f: impl FnOnce(T) -> U) -> Span<U> {
        Span {
            id: self.id,
            value: f(self.value),
            spelling: self.spelling,
            expansion: self.expansion,
            provenance: self.provenance,
            macro_origin: self.macro_origin,
            leading_space: self.leading_space,
        }
    }

    pub fn cover<U>(value: T, spans: &[Span<U>]) -> Self {
        let Some(first) = spans.first() else {
            let loc = Loc::new(FileId(0), 0, 0);
            return Self::new(value, loc, loc);
        };
        let last = spans.last().unwrap();
        let macro_origin = spans
            .iter()
            .all(|span| span.macro_origin == first.macro_origin)
            .then(|| first.macro_origin.clone())
            .flatten();
        Self::new(
            value,
            first.spelling.through(last.spelling),
            first.expansion.through(last.expansion),
        )
        .with_macro_origin(macro_origin)
        .with_provenance(first.provenance)
    }

    pub(crate) fn cover_with_origin<U>(
        value: T,
        spans: &[Span<U>],
        macro_origin: Option<Rc<MacroOrigin>>,
    ) -> Self {
        let Some(first) = spans.first() else {
            let loc = Loc::new(FileId(0), 0, 0);
            return Self::new(value, loc, loc);
        };
        let last = spans.last().unwrap();
        Self::new(
            value,
            first.spelling.through(last.spelling),
            first.expansion.through(last.expansion),
        )
        .with_macro_origin(macro_origin)
        .with_provenance(first.provenance)
    }
}

#[derive(Default)]
pub(crate) struct SpanRangeIndex {
    origin_runs: Vec<usize>,
}

impl SpanRangeIndex {
    pub(crate) fn new<T>(spans: &[Span<T>]) -> Self {
        let mut origin_runs = Vec::with_capacity(spans.len());
        let mut run = 0;
        for (index, span) in spans.iter().enumerate() {
            if index > 0 && span.macro_origin != spans[index - 1].macro_origin {
                run += 1;
            }
            origin_runs.push(run);
        }
        Self { origin_runs }
    }

    pub(crate) fn cover<T, U>(
        &self,
        value: T,
        spans: &[Span<U>],
        start: usize,
        end: usize,
    ) -> Span<T> {
        if end > spans.len() || end > self.origin_runs.len() || start > end {
            return Span::cover(value, spans);
        }
        if start == end {
            return Span::cover(value, &[] as &[Span<U>]);
        }
        let macro_origin = (self.origin_runs[start] == self.origin_runs[end - 1])
            .then(|| spans[start].macro_origin.clone())
            .flatten();
        Span::cover_with_origin(value, &spans[start..end], macro_origin)
    }
}

#[derive(Default, Debug, Clone, Copy, PartialEq, Eq)]
pub enum HeaderKind {
    #[default]
    System,
    User,
}

#[derive(Default, Debug, Clone, Copy, PartialEq, Eq)]
pub struct Provenance {
    pub file: FileId,
    pub kind: HeaderKind,
    pub line: usize,
    pub system_header: Option<FileId>,
}

impl Provenance {
    fn system_header_is_none(&self) -> bool {
        self.system_header.is_none()
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct MacroOrigin {
    pub name: Rc<str>,
    pub definition: Provenance,
    pub inner: Option<Rc<MacroOriginLink>>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct MacroOriginLink {
    pub name: Rc<str>,
    pub definition: Provenance,
    pub parent: Option<Rc<MacroOriginLink>>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum AsmLabel {
    Symbol(String),
    Register(Register),
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum Register {
    X86(RegisterInfo<X86RegisterWidth>),
    Aarch64(RegisterInfo<AArch64RegisterWidth>),
    Other(String),
}

#[derive(CustomDebug, Clone, PartialEq)]
pub struct GnuAsm {
    #[debug(skip_if = Vec::is_empty)]
    pub qualifiers: Vec<Span<AsmQualifier>>,
    pub template: Span<String>,
    #[debug(skip_if = Option::is_none)]
    pub operands: Option<AsmOperands>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum AsmQualifier {
    Volatile,
    Inline,
    Goto,
}

#[derive(CustomDebug, Clone, PartialEq)]
pub struct AsmOperands {
    pub pieces: Vec<AsmTemplatePiece>,
    #[debug(skip_if = Vec::is_empty)]
    pub outputs: Vec<AsmOperand>,
    #[debug(skip_if = Vec::is_empty)]
    pub inputs: Vec<AsmOperand>,
    #[debug(skip_if = Vec::is_empty)]
    pub clobbers: Vec<Span<AsmClobber>>,
    #[debug(skip_if = Vec::is_empty)]
    pub labels: Vec<Span<String>>,
}

#[derive(CustomDebug, Clone, PartialEq, Eq)]
pub enum AsmTemplatePiece {
    Text(String),
    Operand {
        index: usize,
        #[debug(skip_if = Option::is_none)]
        modifier: Option<char>,
    },
    Label(usize),
    Percent,
    UniqueId,
    DialectAlternatives(Vec<Vec<AsmTemplatePiece>>),
}

#[derive(CustomDebug, Clone, PartialEq)]
pub struct AsmOperand {
    #[debug(skip_if = Option::is_none)]
    pub name: Option<Span<String>>,
    pub constraint: Span<AsmConstraint>,
    pub expr: Expr,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct AsmConstraint {
    pub alternatives: Vec<AsmConstraintAlternative>,
}

impl AsmConstraint {
    pub fn write_modifier(&self) -> Option<AsmConstraintModifier> {
        self.alternatives
            .first()?
            .modifiers
            .iter()
            .copied()
            .find(|modifier| {
                matches!(
                    modifier,
                    AsmConstraintModifier::Overwrite | AsmConstraintModifier::ReadWrite
                )
            })
    }

    pub fn early_clobber(&self) -> bool {
        self.alternatives.iter().any(|alternative| {
            alternative
                .modifiers
                .contains(&AsmConstraintModifier::EarlyClobber)
        })
    }

    pub fn tied_output(&self) -> Option<usize> {
        let mut tied = None;
        for alternative in &self.alternatives {
            let AsmConstraintLocation::Matching(index) = alternative.location else {
                return None;
            };
            if tied.is_some_and(|tied| tied != index) {
                return None;
            }
            tied = Some(index);
        }
        tied
    }
}

#[derive(CustomDebug, Clone, PartialEq, Eq)]
pub struct AsmConstraintAlternative {
    #[debug(skip_if = Vec::is_empty)]
    pub modifiers: Vec<AsmConstraintModifier>,
    pub location: AsmConstraintLocation,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum AsmConstraintModifier {
    Overwrite,
    ReadWrite,
    EarlyClobber,
    Commutative,
    Pic,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum AsmConstraintLocation {
    HardRegister(Register),
    Matching(usize),
    Letters(String),
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum AsmClobber {
    Memory,
    Cc,
    Unwind,
    Register(Register),
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct RegisterInfo<RW> {
    pub spelling: String,
    pub number: usize,
    pub canonical: &'static str,
    pub width: Option<RW>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum X86RegisterWidth {
    Low8,
    High8,
    Bits16,
    Bits32,
    Bits64,
}

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub enum AArch64RegisterWidth {
    Bits8,
    Bits16,
    Bits32,
    Bits64,
    Bits128,
    ScalableVector,
    ScalablePredicate,
}

#[derive(Debug, Clone, PartialEq)]
pub enum AlignAsOperand {
    Type { ty: Box<TypeName> },
    Expr(Expr),
}

#[derive(Debug, Clone, PartialEq)]
pub enum CallingConvention {
    Cdecl,
    Stdcall,
    Fastcall,
    Vectorcall,
    Thiscall,
    MsAbi,
    SysVAbi,
    PreserveMost,
    PreserveAll,
    PreserveNone,
    RegParm(Expr),
    Pcs(PcsConvention),
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum PcsConvention {
    Aapcs,
    AapcsVfp,
}

#[derive(Debug, Clone, PartialEq)]
pub enum Attribute {
    Packed,
    AddressSpace(Expr),
    PassObjectSize {
        size_type: Expr,
        dynamic: bool,
    },
    LifetimeBound,
    Overloadable,
    GnuInline,
    NoThrow,
    SelectAny,
    ThreadLocal,
    NoAlias,
    RestrictReturn,
    CodeSeg(String),
    OptimizeNone,
    Aligned(Expr),
    AlignAs(AlignAsOperand),
    VectorSize(Expr),
    Mode(String),
    Visibility(String),
    Section(String),
    Weak,
    Used,
    Retain,
    NoInline,
    AlwaysInline,
    NoReturn,
    Constructor(Option<i64>),
    Destructor(Option<i64>),
    NonNull(Vec<i64>),
    Annotate(String),
    Target(String),
    Alias(String),
    WeakRef(String),
    Malloc,
    AssumeAligned(Vec<Expr>),
    AllocSize(Vec<Expr>),
    AllocAlign(Expr),
    Cleanup(String),
    ReturnsNonNull,
    WarnUnusedResult,
    Sentinel(Option<i64>),
    Cold,
    Flatten,
    Hot,
    Leaf,
    NoIpa,
    NoClone,
    Optimize(Vec<String>),
    Naked,
    Interrupt,
    NoSplitStack,
    ReturnsTwice,
    CpuDispatch(Vec<String>),
    CpuSpecific(Vec<String>),
    TargetClones(Vec<String>),
    Ifunc(String),
    DllImport,
    DllExport,
    WeakImport,
    TlsModel(String),
    MsStruct,
    CallingConvention(CallingConvention),
    NoMips16,
    Availability(Vec<String>),
    ExtVectorType(Expr),
    ScalarStorageOrder(String),
    TransparentUnion,
    Format(Vec<String>),
    FormatArg(Vec<String>),
    GccStruct,
    Common,
    NoCommon,
    Pure,
    Const,
    MayAlias,
    Deprecated(Option<String>),
    NoDiscard(Option<String>),
    MaybeUnused,
    Fallthrough,
    Unknown {
        name: String,
        arguments: Vec<String>,
    },
    IgnoredDeclspec {
        name: String,
        arguments: Vec<String>,
    },
    Invalid {
        name: String,
        arguments: Vec<String>,
    },
}

#[derive(CustomDebug, Clone, PartialEq)]
pub struct FunctionDefinition {
    pub specifiers: DeclarationSpecifiers,
    pub declarator: Declarator,
    #[debug(skip_if = Vec::is_empty)]
    pub attributes: Vec<Span<Attribute>>,
    #[debug(skip_if = Vec::is_empty)]
    pub body: Vec<Stmt>,
}

#[derive(CustomDebug, Clone, PartialEq)]
pub enum TypeSpecifier {
    Void,
    Bool,
    Integer(IntegerType),
    Floating(FloatingType),
    Complex(Box<Self>),
    Atomic(Box<TypeName>),
    Vector(VectorType),
    Mode(ModeType),
    FixedPoint(FixedPointType),
    TypeOf(TypeOfOperand),
    TypeOfUnqual(TypeOfOperand),
    Imaginary(Box<Self>),
    TargetBuiltin(String),
    Inferred,
    Named(String),
    Tag(TagSpecifier),
}

#[derive(Debug, Clone, PartialEq)]
pub struct TypeName {
    pub specifiers: DeclarationSpecifiers,
    pub declarator: Declarator,
}

#[derive(Debug, Clone, PartialEq)]
pub enum IntegerType {
    Char { signed: Option<bool> },
    Ranked { rank: IntegerRank, signed: bool },
    BitInt { width: Expr, signed: bool },
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum IntegerRank {
    Short,
    Int,
    Long,
    LongLong,
    Int128,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum FloatingType {
    BFloat16,
    Float,
    Float16,
    Fp16,
    Float64x,
    Double,
    LongDouble,
    Float128,
    Float128Ext,
    Decimal32,
    Decimal64,
    Decimal128,
}

#[derive(Debug, Clone, PartialEq)]
pub struct VectorType {
    pub element: Box<TypeSpecifier>,
    pub size: VectorSize,
}

#[derive(Debug, Clone, PartialEq)]
pub struct ModeType {
    pub base: Box<TypeSpecifier>,
    pub mode: String,
}

#[derive(Debug, Clone, PartialEq)]
pub enum VectorSize {
    Bytes(Expr),
    Lanes(Expr),
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum FixedPointKind {
    Fract,
    Accum,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum FixedPointRank {
    Default,
    Short,
    Long,
    LongLong,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct FixedPointType {
    pub kind: FixedPointKind,
    pub rank: FixedPointRank,
    pub signed: bool,
    pub saturated: bool,
}

#[derive(Debug, Clone, PartialEq)]
pub enum TypeOfOperand {
    Expression(Expr),
    Type(Box<TypeName>),
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub enum TagKind {
    Struct,
    Union,
    Enum,
}

#[derive(CustomDebug, Clone, Copy, Default, PartialEq, Eq)]
pub struct Qualifiers {
    #[debug(skip_if = is_false)]
    pub is_const: bool,
    #[debug(skip_if = is_false)]
    pub is_volatile: bool,
    #[debug(skip_if = is_false)]
    pub is_restrict: bool,
    #[debug(skip_if = is_false)]
    pub is_atomic: bool,
    #[debug(skip_if = is_false)]
    pub is_unaligned: bool,
}

impl Qualifiers {
    fn is_default(value: &Self) -> bool {
        *value == Self::default()
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum StorageClass {
    None,
    Typedef,
    Extern,
    Static,
    Auto,
    Register,
}

impl StorageClass {
    pub fn as_str(self) -> &'static str {
        self.into()
    }

    fn is_none(value: &Self) -> bool {
        *value == Self::None
    }
}

impl From<StorageClass> for &'static str {
    fn from(storage: StorageClass) -> Self {
        match storage {
            StorageClass::None => "none",
            StorageClass::Typedef => "typedef",
            StorageClass::Extern => "extern",
            StorageClass::Static => "static",
            StorageClass::Auto => "auto",
            StorageClass::Register => "register",
        }
    }
}

impl TryFrom<&str> for StorageClass {
    type Error = ();

    fn try_from(value: &str) -> Result<Self, Self::Error> {
        match value {
            "typedef" => Ok(Self::Typedef),
            "extern" => Ok(Self::Extern),
            "static" => Ok(Self::Static),
            "auto" => Ok(Self::Auto),
            "register" => Ok(Self::Register),
            _ => Err(()),
        }
    }
}

#[derive(Debug, Clone, PartialEq)]
pub enum ArraySize {
    Unspecified,
    Expression(Expr),
    Star,
}

#[derive(CustomDebug, Clone, PartialEq)]
pub enum Declarator {
    Abstract,
    Name(String),
    Grouped(Box<Declarator>),
    Attributed {
        inner: Box<Declarator>,
        attributes: Vec<Span<Attribute>>,
    },
    Pointer {
        qualifiers: Qualifiers,
        #[debug(skip_if = Vec::is_empty)]
        attributes: Vec<Span<Attribute>>,
        inner: Box<Declarator>,
    },
    Array {
        inner: Box<Declarator>,
        size: ArraySize,
        #[debug(skip_if = Qualifiers::is_default)]
        qualifiers: Qualifiers,
        #[debug(skip_if = is_false)]
        is_static: bool,
    },
    Function {
        inner: Box<Declarator>,
        parameters: ParameterList,
    },
}

#[derive(Clone, Copy, Default)]
pub struct ArrayDeclarator {
    pub qualifiers: Qualifiers,
    pub is_static: bool,
}

#[derive(Clone, Copy)]
struct Derivation {
    depth: usize,
    grouped: bool,
    suffix: bool,
}

impl Attribute {
    pub fn is_type_attribute(&self) -> bool {
        matches!(
            self,
            Self::VectorSize(_) | Self::ExtVectorType(_) | Self::Mode(_)
        )
    }
}

impl TypeSpecifier {
    pub fn with_type_attributes<'a>(
        mut self,
        attributes: impl IntoIterator<Item = &'a Span<Attribute>>,
    ) -> Self {
        for attribute in attributes {
            let size = match &attribute.value {
                Attribute::VectorSize(size) => VectorSize::Bytes(size.clone()),
                Attribute::ExtVectorType(size) => VectorSize::Lanes(size.clone()),
                Attribute::Mode(mode) => {
                    self = Self::Mode(ModeType {
                        base: Box::new(self),
                        mode: mode.clone(),
                    });
                    continue;
                }
                _ => continue,
            };
            self = Self::Vector(VectorType {
                element: Box::new(self),
                size,
            });
        }
        self
    }
}

impl Declarator {
    pub fn name(&self) -> Option<&str> {
        match self {
            Self::Name(name) => Some(name),
            Self::Abstract => None,
            Self::Grouped(inner)
            | Self::Attributed { inner, .. }
            | Self::Pointer { inner, .. }
            | Self::Array { inner, .. }
            | Self::Function { inner, .. } => inner.name(),
        }
    }

    pub fn is_derived(&self) -> bool {
        self.outermost_derivation().is_some()
    }

    pub fn function_parameters(&self) -> Option<&ParameterList> {
        let mut layer = self;
        for _ in 0..self.outermost_derivation()?.depth {
            layer = layer.inner()?;
        }
        match layer {
            Self::Function { parameters, .. } => Some(parameters),
            _ => None,
        }
    }

    pub fn array_parameter(&self) -> Option<ArrayDeclarator> {
        let mut layer = self;
        for _ in 0..self.outermost_derivation()?.depth {
            layer = layer.inner()?;
        }
        match layer {
            Self::Array {
                qualifiers,
                is_static,
                ..
            } => Some(ArrayDeclarator {
                qualifiers: *qualifiers,
                is_static: *is_static,
            }),
            _ => None,
        }
    }

    pub fn function_parameters_mut(&mut self) -> Option<&mut ParameterList> {
        match self.outermost_layer_mut()? {
            Self::Function { parameters, .. } => Some(parameters),
            _ => None,
        }
    }

    pub fn pointer_qualifiers_mut(&mut self) -> Option<&mut Qualifiers> {
        match self.outermost_layer_mut()? {
            Self::Pointer { qualifiers, .. } => Some(qualifiers),
            _ => None,
        }
    }

    fn outermost_layer_mut(&mut self) -> Option<&mut Declarator> {
        let depth = self.outermost_derivation()?.depth;
        let mut layer = self;
        for _ in 0..depth {
            layer = match layer {
                Self::Grouped(inner)
                | Self::Attributed { inner, .. }
                | Self::Pointer { inner, .. }
                | Self::Array { inner, .. }
                | Self::Function { inner, .. } => &mut **inner,
                Self::Name(_) | Self::Abstract => return None,
            };
        }
        Some(layer)
    }

    pub fn grouped_attributes(&self) -> Vec<&Span<Attribute>> {
        let mut attributes = Vec::new();
        let mut layer = Some(self);
        while let Some(declarator) = layer {
            if let Self::Attributed {
                attributes: own, ..
            } = declarator
            {
                attributes.extend(own);
            }
            layer = declarator.inner();
        }
        attributes
    }

    fn inner(&self) -> Option<&Declarator> {
        match self {
            Self::Name(_) | Self::Abstract => None,
            Self::Grouped(inner)
            | Self::Attributed { inner, .. }
            | Self::Pointer { inner, .. }
            | Self::Array { inner, .. }
            | Self::Function { inner, .. } => Some(inner),
        }
    }

    fn outermost_derivation(&self) -> Option<Derivation> {
        let deeper = |inner: &Declarator, grouped: bool| {
            inner.outermost_derivation().map(|derivation| Derivation {
                depth: derivation.depth + 1,
                grouped: grouped || derivation.grouped,
                suffix: derivation.suffix,
            })
        };
        let this = |suffix| Derivation {
            depth: 0,
            grouped: false,
            suffix,
        };
        match self {
            Self::Name(_) | Self::Abstract => None,
            Self::Attributed { inner, .. } => deeper(inner, false),
            Self::Grouped(inner) => deeper(inner, true),
            Self::Pointer { inner, .. } => match deeper(inner, false) {
                Some(derivation) if derivation.grouped => Some(derivation),
                _ => Some(this(false)),
            },
            Self::Array { inner, .. } | Self::Function { inner, .. } => {
                match deeper(inner, false) {
                    Some(derivation) if derivation.grouped || derivation.suffix => Some(derivation),
                    _ => Some(this(true)),
                }
            }
        }
    }
}

#[derive(CustomDebug, Clone, PartialEq)]
pub enum ParameterList {
    Prototype {
        parameters: Vec<ParameterDeclaration>,
        #[debug(skip_if = is_false)]
        variadic: bool,
    },
    IdentifierList {
        parameters: Vec<ParameterDeclaration>,
    },
    Void,
    Empty,
}

impl ParameterList {
    pub fn parameters(&self) -> &[ParameterDeclaration] {
        match self {
            Self::Prototype { parameters, .. } | Self::IdentifierList { parameters } => parameters,
            Self::Void | Self::Empty => &[],
        }
    }

    pub fn is_variadic(&self) -> bool {
        matches!(self, Self::Prototype { variadic: true, .. })
    }
}

pub type ParameterDeclaration = Span<ParameterDeclarationKind>;

#[derive(CustomDebug, Clone, PartialEq)]
pub struct ParameterDeclarationKind {
    pub specifiers: DeclarationSpecifiers,
    pub declarator: Declarator,
    #[debug(skip_if = Vec::is_empty)]
    pub attributes: Vec<Span<Attribute>>,
}

#[derive(CustomDebug, Clone, PartialEq)]
pub struct DeclarationSpecifiers {
    pub ty: TypeSpecifier,
    #[debug(skip_if = Qualifiers::is_default)]
    pub qualifiers: Qualifiers,
    #[debug(skip_if = StorageClass::is_none)]
    pub storage: StorageClass,
    #[debug(skip_if = is_false)]
    pub is_thread_local: bool,
    #[debug(skip_if = is_false)]
    pub is_inline: bool,
    #[debug(skip_if = is_false)]
    pub is_noreturn: bool,
    #[debug(skip_if = is_false)]
    pub is_constexpr: bool,
    #[debug(skip_if = Vec::is_empty)]
    pub attributes: Vec<Span<Attribute>>,
}

#[derive(CustomDebug, Clone, PartialEq)]
pub struct Declaration {
    pub specifiers: DeclarationSpecifiers,
    #[debug(skip_if = Vec::is_empty)]
    pub declarators: Vec<InitDeclarator>,
}

impl Declaration {
    pub fn names(&self) -> impl Iterator<Item = &str> {
        self.declarators
            .iter()
            .filter_map(|declarator| declarator.declarator.name())
    }
}

pub type InitDeclarator = Span<InitDeclaratorKind>;

#[derive(CustomDebug, Clone, PartialEq)]
pub struct InitDeclaratorKind {
    pub declarator: Declarator,
    #[debug(skip_if = Option::is_none)]
    pub asm_label: Option<Span<AsmLabel>>,
    #[debug(skip_if = Vec::is_empty)]
    pub attributes: Vec<Span<Attribute>>,
    #[debug(skip_if = Option::is_none)]
    pub initializer: Option<Initializer>,
}

#[derive(CustomDebug, Clone, PartialEq)]
pub struct StaticAssert {
    pub condition: Expr,
    #[debug(skip_if = Option::is_none)]
    pub message: Option<String>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash, PartialOrd, Ord)]
pub struct TagId(pub usize);

#[derive(CustomDebug, Clone, PartialEq)]
pub enum TagSpecifier {
    Reference {
        kind: TagKind,
        name: String,
        #[debug(skip_if = Option::is_none)]
        fixed_type: Option<Box<TypeName>>,
    },
    Definition(TagId),
}

#[derive(CustomDebug, Clone, PartialEq)]
pub struct TagDefinition {
    pub id: TagId,
    pub kind: TagKind,
    pub name: Option<String>,
    #[debug(skip_if = Vec::is_empty)]
    pub attributes: Vec<Span<Attribute>>,
    pub body: TagBody,
}

#[derive(CustomDebug, Clone, PartialEq)]
pub enum TagBody {
    Record(Vec<FieldItem>),
    Enum {
        #[debug(skip_if = Option::is_none)]
        fixed_type: Option<TypeName>,
        enumerators: Vec<EnumItem>,
    },
}

#[derive(Debug, Clone, PartialEq)]
pub enum FieldItemKind {
    Comment(CommentGroup),
    Field(FieldDecl),
}

#[derive(Debug, Clone, PartialEq)]
pub enum EnumItemKind {
    Comment(CommentGroup),
    Enumerator(Enumerator),
}

#[derive(CustomDebug, Clone, PartialEq)]
pub struct CommentGroup {
    pub comment: Comment,
}

#[derive(Debug, Clone, PartialEq)]
pub struct Comment {
    pub text: Vec<String>,
    pub loc: Loc,
}

#[derive(CustomDebug, Clone, PartialEq)]
pub struct FieldDecl {
    pub specifiers: DeclarationSpecifiers,
    #[debug(skip_if = Vec::is_empty)]
    pub declarators: Vec<FieldDeclarator>,
}

pub type FieldDeclarator = Span<FieldDeclaratorKind>;

#[derive(CustomDebug, Clone, PartialEq)]
pub struct FieldDeclaratorKind {
    pub declarator: Declarator,
    #[debug(skip_if = Option::is_none)]
    pub bit_width: Option<Expr>,
    #[debug(skip_if = Vec::is_empty)]
    pub attributes: Vec<Span<Attribute>>,
}

#[derive(CustomDebug, Clone, PartialEq)]
pub struct Enumerator {
    pub name: String,
    #[debug(skip_if = Vec::is_empty)]
    pub attributes: Vec<Span<Attribute>>,
    pub value: Option<Expr>,
}

#[derive(CustomDebug, Clone, PartialEq)]
pub enum DeclKind {
    Comment(CommentGroup),
    Function(FunctionDefinition),
    Declaration(Declaration),
    StaticAssert(StaticAssert),
    Asm(GnuAsm),
    Pragma(Pragma),
}

impl DeclKind {
    pub fn names(&self) -> Vec<&str> {
        match self {
            Self::Comment(_) | Self::StaticAssert(_) | Self::Asm(_) | Self::Pragma(_) => Vec::new(),
            Self::Function(function) => function.declarator.name().into_iter().collect(),
            Self::Declaration(declaration) => declaration.names().collect(),
        }
    }
}

impl Span<DeclKind> {
    pub fn provenance_file(&self) -> FileId {
        self.provenance.file
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct TranslationUnit {
    pub standard: crate::compiler_args::LanguageStandard,
    pub options: crate::compiler_options::CompilerOptions,
    pub decls: Vec<Decl>,
    pub tags: Vec<Span<TagDefinition>>,
    pub flavor: crate::compiler_args::CompilerFlavor,
    pub target: crate::target_info::TargetInfo,
}

impl TranslationUnit {
    pub fn tag(&self, id: TagId) -> Option<&Span<TagDefinition>> {
        self.tags
            .binary_search_by_key(&id, |tag| tag.value.id)
            .ok()
            .map(|index| &self.tags[index])
    }
}
