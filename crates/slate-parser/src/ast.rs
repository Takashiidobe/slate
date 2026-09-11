use crate::const_expr::{BinaryOp, ConstExpr, UnaryOp};
use custom_debug::Debug as CustomDebug;

fn is_false(value: &bool) -> bool {
    !*value
}

#[derive(Debug, Clone, PartialEq)]
pub enum Condition {
    Defined(String),
    Constant(i64),
    Not(Box<Condition>),
    And(Box<Condition>, Box<Condition>),
    Or(Box<Condition>, Box<Condition>),
}

#[derive(Debug, Clone, PartialEq)]
pub struct Conditional<T> {
    pub branches: Vec<(Condition, T)>,
}

#[derive(Debug, Clone, PartialEq)]
pub enum Expr {
    Const(Box<ConstExpr>),
    IntLit(i64),
    StringLit(String),
    Identifier(String),
    Unary {
        op: UnaryOp,
        value: Box<Expr>,
    },
    Binary {
        op: BinaryOp,
        left: Box<Expr>,
        right: Box<Expr>,
    },
    SizeOf(Box<Expr>),
    Generic {
        controlling: Box<Expr>,
        associations: Vec<GenericAssociation>,
    },
    StatementExpression(Vec<Stmt>),
    Call {
        callee: String,
        argument: String,
    },
    Cast {
        ty: String,
        expression: String,
    },
}

impl std::fmt::Display for Expr {
    fn fmt(&self, formatter: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::Const(value) => write!(formatter, "{value}"),
            Self::IntLit(value) => write!(formatter, "{value}"),
            Self::StringLit(value) => write!(formatter, "\"{value}\""),
            Self::Identifier(value) => formatter.write_str(value),
            Self::Unary { op, value } => write!(formatter, "{}{}", <&str>::from(*op), value),
            Self::Binary { op, left, right } => {
                write!(formatter, "({left} {} {right})", <&str>::from(*op))
            }
            Self::SizeOf(value) => write!(formatter, "sizeof({value})"),
            Self::Generic {
                controlling,
                associations,
            } => write!(
                formatter,
                "_Generic({controlling}, {} )",
                associations.len()
            ),
            Self::StatementExpression(statements) => {
                write!(
                    formatter,
                    "statement_expression({} statements)",
                    statements.len()
                )
            }
            Self::Call { callee, argument } => write!(formatter, "{callee}({argument})"),
            Self::Cast { ty, expression } => write!(formatter, "({ty}){expression}"),
        }
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct GenericAssociation {
    pub type_name: Option<String>,
    pub expression: Expr,
}

#[derive(Debug, Clone, PartialEq)]
pub enum Designator {
    Array(i64),
    Field(String),
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
    Conditional(Conditional<Box<Initializer>>),
}

#[derive(Debug, Clone, PartialEq)]
pub enum Stmt {
    Return(Expr),
    Expr(Expr),
    Conditional(Conditional<Vec<Stmt>>),
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub struct FileId(pub u32);

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
}

#[derive(Debug, Clone, PartialEq, Eq)]
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
    Constructor,
    Destructor,
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
    pub body: Vec<Stmt>,
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
    BFloat16,
    Char,
    SignedChar,
    UnsignedChar,
    Short,
    UnsignedShort,
    Int,
    UnsignedInt,
    Long,
    UnsignedLong,
    LongLong,
    UnsignedLongLong,
    Float,
    Float16,
    Fp16,
    Float64x,
    Float128,
    Float128Ext,
    Double,
    LongDouble,
    Complex,
    DoubleComplex,
    LongDoubleComplex,
    Int128,
    UnsignedInt128,
    BitInt {
        width: ConstExpr,
        is_unsigned: bool,
    },
    Named(String),
    Tagged {
        kind: TagKind,
        name: Option<String>,
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

pub type Type = CType;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
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
    ThreadLocal,
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
            StorageClass::ThreadLocal => "_Thread_local",
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
    Expression(Box<Expr>),
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
    pub is_inline: bool,
    #[debug(skip_if = is_false)]
    pub is_noreturn: bool,
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
pub struct RecordDecl {
    pub kind: TagKind,
    pub name: Option<String>,
    #[debug(skip_if = Vec::is_empty)]
    pub fields: Vec<FieldDecl>,
    pub provenance: Provenance,
    #[debug(skip_if = Vec::is_empty)]
    pub attributes: Vec<Attribute>,
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
    pub value: Option<Expr>,
}

#[derive(CustomDebug, Clone, PartialEq)]
pub enum Decl {
    Function(FunctionDecl),
    Declaration {
        declaration: Declaration,
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
    Conditional(Conditional<Vec<Decl>>),
}

impl Decl {
    pub fn name(&self) -> Option<&str> {
        match self {
            Self::Function(function) => Some(&function.name),
            Self::Declaration { declaration, .. } => declaration.declarator.name(),
            Self::Typedef { name, .. } => Some(name),
            Self::Record(record) => record.name.as_deref(),
            Self::Enum(enumeration) => enumeration.name.as_deref(),
            Self::Conditional(_) => None,
        }
    }

    pub fn provenance(&self) -> Option<FileId> {
        match self {
            Self::Function(function) => Some(function.provenance.file),
            Self::Declaration { provenance, .. } | Self::Typedef { provenance, .. } => {
                Some(provenance.file)
            }
            Self::Record(record) => Some(record.provenance.file),
            Self::Enum(enumeration) => Some(enumeration.provenance.file),
            Self::Conditional(_) => None,
        }
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct TranslationUnit {
    pub decls: Vec<Decl>,
}

#[derive(Debug, Clone, PartialEq)]
pub enum ConcreteStmt {
    Return(Expr),
    Expr(Expr),
}

#[derive(CustomDebug, Clone, PartialEq)]
pub struct ConcreteFunctionDecl {
    pub ret_type: Type,
    pub name: String,
    #[debug(skip_if = Vec::is_empty)]
    pub parameters: Vec<Parameter>,
    #[debug(skip_if = is_false)]
    pub variadic: bool,
    #[debug(skip_if = Vec::is_empty)]
    pub body: Vec<ConcreteStmt>,
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
pub enum ConcreteDecl {
    Function(ConcreteFunctionDecl),
    Declaration {
        declaration: Declaration,
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

#[derive(Debug, Clone, PartialEq)]
pub struct ConcreteTranslationUnit {
    pub decls: Vec<ConcreteDecl>,
}
