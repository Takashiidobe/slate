use crate::const_expr::{BinaryOp, ConstExpr, UnaryOp};
use custom_debug::Debug as CustomDebug;

pub(crate) fn is_false(value: &bool) -> bool {
    !*value
}

pub type SpannedExpr = Span<Expr>;
pub type SpannedStmt = Span<Stmt>;
pub type SpannedFieldItem = Span<FieldItem>;
pub type SpannedDecl = Span<Decl>;

#[derive(Debug, Clone, PartialEq)]
pub enum Expr {
    Const(Box<ConstExpr>),
    IntLit(i64),
    StringLit(String),
    Utf8StringLit(String),
    Utf16StringLit(String),
    Utf32StringLit(String),
    WideStringLit(String),
    Identifier(String),
    Unary {
        op: UnaryOp,
        value: Box<SpannedExpr>,
    },
    Binary {
        op: BinaryOp,
        left: Box<SpannedExpr>,
        right: Box<SpannedExpr>,
    },
    SizeOf(Box<SpannedExpr>),
    StatementExpression(Vec<SpannedStmt>),
}

impl std::fmt::Display for Expr {
    fn fmt(&self, formatter: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::Const(value) => write!(formatter, "{value}"),
            Self::IntLit(value) => write!(formatter, "{value}"),
            Self::StringLit(value) => write!(formatter, "\"{value}\""),
            Self::Utf8StringLit(value) => write!(formatter, "u8\"{value}\""),
            Self::Utf16StringLit(value) => write!(formatter, "u\"{value}\""),
            Self::Utf32StringLit(value) => write!(formatter, "U\"{value}\""),
            Self::WideStringLit(value) => write!(formatter, "L\"{value}\""),
            Self::Identifier(value) => formatter.write_str(value),
            Self::Unary { op, value } => write!(formatter, "{}{}", <&str>::from(*op), value),
            Self::Binary { op, left, right } => {
                write!(formatter, "({left} {} {right})", <&str>::from(*op))
            }
            Self::SizeOf(value) => write!(formatter, "sizeof({value})"),
            Self::StatementExpression(statements) => {
                write!(
                    formatter,
                    "statement_expression({} statements)",
                    statements.len()
                )
            }
        }
    }
}

#[derive(Debug, Clone, PartialEq)]
pub enum Designator {
    Array(i64),
    ArrayRange {
        start: IntegerValue,
        end: IntegerValue,
    },
    Field(String),
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum IntegerValue {
    I128(i128),
    Arbitrary(String),
}

#[derive(Debug, Clone, PartialEq)]
pub struct InitializerItem {
    pub designators: Vec<Designator>,
    pub value: Initializer,
}

#[derive(Debug, Clone, PartialEq)]
pub enum Initializer {
    Expr(SpannedExpr),
    List(Vec<InitializerItem>),
}

#[derive(Debug, Clone, PartialEq)]
pub enum Stmt {
    Comment {
        text: String,
        loc: Loc,
        provenance: Provenance,
    },
    Return(SpannedExpr),
    ReturnVoid,
    Expr(SpannedExpr),
    Decl(Declaration),
    StaticAssert(StaticAssert),
    Block(Vec<SpannedStmt>),
    If {
        condition: SpannedExpr,
        then_branch: Vec<SpannedStmt>,
        else_branch: Option<Vec<SpannedStmt>>,
    },
    While {
        condition: SpannedExpr,
        body: Vec<SpannedStmt>,
    },
    DoWhile {
        body: Vec<SpannedStmt>,
        condition: SpannedExpr,
    },
    For {
        init: Option<Box<SpannedStmt>>,
        condition: Option<SpannedExpr>,
        increment: Option<SpannedExpr>,
        body: Vec<SpannedStmt>,
    },
    Switch {
        discriminant: SpannedExpr,
        body: Vec<SpannedStmt>,
    },
    Case(SpannedExpr),
    CaseRange {
        start: SpannedExpr,
        end: SpannedExpr,
    },
    Default,
    Labeled(String),
    LocalLabelDecl(Vec<String>),
    Asm(String),
    Goto(String),
    ComputedGoto(SpannedExpr),
    NestedFunction(Box<FunctionDecl>),
    Break,
    Continue,
    Unreachable(Box<SpannedStmt>),
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
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

#[derive(Clone, PartialEq, Eq)]
pub struct Span<T> {
    pub value: T,
    pub spelling: Loc,
    pub expansion: Loc,
}

impl<T: std::fmt::Debug> std::fmt::Debug for Span<T> {
    fn fmt(&self, formatter: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        self.value.fmt(formatter)
    }
}

impl<T> Span<T> {
    pub fn new(value: T, spelling: Loc, expansion: Loc) -> Self {
        Self {
            value,
            spelling,
            expansion,
        }
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
        Span::new(value, self.spelling, self.expansion)
    }

    pub fn cover<U>(value: T, spans: &[Span<U>]) -> Self {
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
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum HeaderKind {
    System,
    User,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct Provenance {
    pub file: FileId,
    pub kind: HeaderKind,
    pub line: usize,
    pub header: Option<FileId>,
}

impl Default for Provenance {
    fn default() -> Self {
        Self {
            file: FileId(0),
            kind: HeaderKind::System,
            line: 0,
            header: None,
        }
    }
}

#[derive(Debug, Clone, PartialEq)]
pub enum Attribute {
    Packed,
    Aligned(ConstExpr),
    VectorSize(ConstExpr),
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
    AssumeAligned(Vec<ConstExpr>),
    AllocSize(Vec<ConstExpr>),
    AllocAlign(ConstExpr),
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
    WeakImport,
    TlsModel(String),
    MsStruct,
    Stdcall,
    NoMips16,
    Availability(Vec<String>),
    ExtVectorType(ConstExpr),
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
    Invalid {
        name: String,
        arguments: Vec<String>,
    },
}

#[derive(CustomDebug, Clone, PartialEq)]
pub struct FunctionDecl {
    pub ret_type: Type,
    pub name: String,
    #[debug(skip_if = Vec::is_empty)]
    pub parameters: Vec<Parameter>,
    #[debug(skip_if = is_false)]
    pub variadic: bool,
    #[debug(skip_if = Vec::is_empty)]
    pub body: Vec<SpannedStmt>,
    pub provenance: Provenance,
    #[debug(skip_if = Qualifiers::is_default)]
    pub qualifiers: Qualifiers,
    #[debug(skip_if = StorageClass::is_none)]
    pub storage: StorageClass,
    #[debug(skip_if = is_false)]
    pub is_inline: bool,
    #[debug(skip_if = is_false)]
    pub is_noreturn: bool,
    #[debug(skip_if = Vec::is_empty)]
    pub attributes: Vec<Attribute>,
}

#[derive(CustomDebug, Clone, PartialEq)]
pub enum CType {
    Void,
    Bool,
    Integer(IntegerType),
    Floating(FloatingType),
    Complex(Box<Self>),
    Atomic(Box<Self>),
    Vector(VectorType),
    FixedPoint(FixedPointType),
    TypeOf(TypeOfOperand),
    TypeOfUnqual(TypeOfOperand),
    Imaginary(Box<Self>),
    TargetBuiltin(String),
    Named(String),
    Tagged {
        kind: TagKind,
        name: Option<String>,
        #[debug(skip_if = Option::is_none)]
        body: Option<TagBody>,
    },
    Qualified {
        #[debug(skip_if = Qualifiers::is_default)]
        qualifiers: Qualifiers,
        ty: Box<CType>,
    },
    Pointer {
        #[debug(skip_if = Qualifiers::is_default)]
        qualifiers: Qualifiers,
        pointee: Box<CType>,
    },
    Array {
        element: Box<CType>,
        size: ArraySize,
    },
    Function {
        return_type: Box<CType>,
        #[debug(skip_if = Vec::is_empty)]
        parameters: Vec<Parameter>,
        #[debug(skip_if = is_false)]
        variadic: bool,
    },
}

#[derive(Debug, Clone, PartialEq)]
pub enum IntegerType {
    Char { signed: Option<bool> },
    Ranked { rank: IntegerRank, signed: bool },
    BitInt { width: ConstExpr, signed: bool },
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
}

#[derive(Debug, Clone, PartialEq)]
pub struct VectorType {
    pub element: Box<CType>,
    pub size: VectorSize,
}

#[derive(Debug, Clone, PartialEq)]
pub enum VectorSize {
    Bytes(ConstExpr),
    Lanes(ConstExpr),
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
    pub saturated: bool,
}

#[derive(Debug, Clone, PartialEq)]
pub enum TypeOfOperand {
    Expression(Box<SpannedExpr>),
    Type(Box<CType>),
}

pub type Type = CType;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum TagKind {
    Struct,
    Union,
    Enum,
}

#[derive(Debug, Clone, PartialEq)]
pub enum TagBody {
    Fields(Vec<FieldDecl>),
    Enumerators(Vec<Enumerator>),
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
    Expression(Box<SpannedExpr>),
    Star,
}

#[derive(CustomDebug, Clone, PartialEq)]
pub enum Declarator {
    Abstract,
    Name(String),
    Grouped(Box<Declarator>),
    Pointer {
        qualifiers: Qualifiers,
        inner: Box<Declarator>,
    },
    Array {
        inner: Box<Declarator>,
        size: ArraySize,
    },
    Function {
        inner: Box<Declarator>,
        #[debug(skip_if = Vec::is_empty)]
        parameters: Vec<Parameter>,
        #[debug(skip_if = is_false)]
        variadic: bool,
    },
}

impl Declarator {
    pub fn name(&self) -> Option<&str> {
        match self {
            Self::Name(name) => Some(name),
            Self::Abstract => None,
            Self::Grouped(inner)
            | Self::Pointer { inner, .. }
            | Self::Array { inner, .. }
            | Self::Function { inner, .. } => inner.name(),
        }
    }
}

#[derive(CustomDebug, Clone, PartialEq)]
pub struct Parameter {
    pub ty: CType,
    #[debug(skip_if = Option::is_none)]
    pub declarator: Option<Declarator>,
    #[debug(skip_if = Vec::is_empty)]
    pub attributes: Vec<Attribute>,
}

#[derive(CustomDebug, Clone, PartialEq)]
pub struct DeclarationSpecifiers {
    pub ty: CType,
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
}

#[derive(CustomDebug, Clone, PartialEq)]
pub struct Declaration {
    pub specifiers: DeclarationSpecifiers,
    pub declarator: Declarator,
    #[debug(skip_if = Option::is_none)]
    pub initializer: Option<Initializer>,
    #[debug(skip_if = Vec::is_empty)]
    pub attributes: Vec<Attribute>,
}

#[derive(CustomDebug, Clone, PartialEq)]
pub struct StaticAssert {
    pub condition: SpannedExpr,
    #[debug(skip_if = Option::is_none)]
    pub message: Option<String>,
}

#[derive(CustomDebug, Clone, PartialEq)]
pub struct RecordDecl {
    pub kind: TagKind,
    pub name: Option<String>,
    #[debug(skip_if = Vec::is_empty)]
    pub fields: Vec<SpannedFieldItem>,
    pub provenance: Provenance,
    #[debug(skip_if = Vec::is_empty)]
    pub attributes: Vec<Attribute>,
}

#[derive(Debug, Clone, PartialEq)]
pub enum FieldItem {
    Comment {
        text: String,
        loc: Loc,
        provenance: Provenance,
    },
    Field(FieldDecl),
}

#[derive(Debug, Clone, PartialEq)]
pub struct FieldDecl {
    pub declaration: Declaration,
    pub provenance: Provenance,
}

#[derive(CustomDebug, Clone, PartialEq)]
pub struct EnumDecl {
    pub name: Option<String>,
    #[debug(skip_if = Vec::is_empty)]
    pub enumerators: Vec<Enumerator>,
    pub provenance: Provenance,
}

#[derive(Debug, Clone, PartialEq)]
pub struct Enumerator {
    pub name: String,
    pub value: Option<SpannedExpr>,
}

#[derive(CustomDebug, Clone, PartialEq)]
pub enum Decl {
    Comment {
        text: String,
        loc: Loc,
        provenance: Provenance,
    },
    Function(FunctionDecl),
    Declaration {
        declaration: Declaration,
        provenance: Provenance,
    },
    StaticAssert {
        assertion: StaticAssert,
        provenance: Provenance,
    },
    Typedef {
        name: String,
        ty: CType,
        provenance: Provenance,
        #[debug(skip_if = Vec::is_empty)]
        attributes: Vec<Attribute>,
    },
    Record(RecordDecl),
    Enum(EnumDecl),
}

impl Decl {
    pub fn name(&self) -> Option<&str> {
        match self {
            Self::Comment { .. } => None,
            Self::Function(function) => Some(&function.name),
            Self::Declaration { declaration, .. } => declaration.declarator.name(),
            Self::StaticAssert { .. } => None,
            Self::Typedef { name, .. } => Some(name),
            Self::Record(record) => record.name.as_deref(),
            Self::Enum(enumeration) => enumeration.name.as_deref(),
        }
    }

    pub fn provenance(&self) -> FileId {
        match self {
            Self::Comment { provenance, .. } => provenance.file,
            Self::Function(function) => function.provenance.file,
            Self::Declaration { provenance, .. }
            | Self::StaticAssert { provenance, .. }
            | Self::Typedef { provenance, .. } => provenance.file,
            Self::Record(record) => record.provenance.file,
            Self::Enum(enumeration) => enumeration.provenance.file,
        }
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct TranslationUnit {
    pub decls: Vec<SpannedDecl>,
}
