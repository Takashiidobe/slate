use crate::const_expr::{BinaryOp, ConstExpr, UnaryOp};

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

#[derive(Debug, Clone, PartialEq, Eq)]
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
            Self::Call { callee, argument } => write!(formatter, "{callee}({argument})"),
            Self::Cast { ty, expression } => write!(formatter, "({ty}){expression}"),
        }
    }
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

#[derive(Debug, Clone, PartialEq)]
pub struct FunctionDecl {
    pub ret_type: Type,
    pub name: String,
    pub body: Vec<Stmt>,
    pub provenance: Provenance,
    pub qualifiers: Qualifiers,
    pub storage: StorageClass,
    pub is_inline: bool,
    pub attributes: Vec<Attribute>,
}

#[derive(Debug, Clone, PartialEq)]
pub enum CType {
    Void,
    Bool,
    Char,
    Int,
    Named(String),
    Tagged {
        kind: TagKind,
        name: Option<String>,
    },
    Qualified {
        qualifiers: Qualifiers,
        ty: Box<CType>,
    },
    Pointer {
        qualifiers: Qualifiers,
        pointee: Box<CType>,
    },
    Array {
        element: Box<CType>,
        size: ArraySize,
    },
    Function {
        return_type: Box<CType>,
        parameters: Vec<Parameter>,
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

#[derive(Debug, Clone, Copy, Default, PartialEq, Eq)]
pub struct Qualifiers {
    pub is_const: bool,
    pub is_volatile: bool,
    pub is_restrict: bool,
    pub is_atomic: bool,
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
    Expression(Box<Expr>),
}

#[derive(Debug, Clone, PartialEq)]
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
        parameters: Vec<Parameter>,
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

#[derive(Debug, Clone, PartialEq)]
pub struct Parameter {
    pub ty: CType,
    pub declarator: Option<Declarator>,
    pub attributes: Vec<Attribute>,
}

#[derive(Debug, Clone, PartialEq)]
pub struct DeclarationSpecifiers {
    pub ty: CType,
    pub qualifiers: Qualifiers,
    pub storage: StorageClass,
    pub is_inline: bool,
}

#[derive(Debug, Clone, PartialEq)]
pub struct Declaration {
    pub specifiers: DeclarationSpecifiers,
    pub declarator: Declarator,
    pub initializer: Option<Initializer>,
    pub attributes: Vec<Attribute>,
}

#[derive(Debug, Clone, PartialEq)]
pub struct RecordDecl {
    pub kind: TagKind,
    pub name: Option<String>,
    pub fields: Vec<FieldDecl>,
    pub provenance: Provenance,
    pub attributes: Vec<Attribute>,
}

#[derive(Debug, Clone, PartialEq)]
pub struct FieldDecl {
    pub declaration: Declaration,
    pub provenance: Provenance,
}

#[derive(Debug, Clone, PartialEq)]
pub struct EnumDecl {
    pub name: Option<String>,
    pub enumerators: Vec<Enumerator>,
    pub provenance: Provenance,
}

#[derive(Debug, Clone, PartialEq)]
pub struct Enumerator {
    pub name: String,
    pub value: Option<Expr>,
}

#[derive(Debug, Clone, PartialEq)]
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

#[derive(Debug, Clone, PartialEq)]
pub struct ConcreteFunctionDecl {
    pub ret_type: Type,
    pub name: String,
    pub body: Vec<ConcreteStmt>,
    pub provenance: Provenance,
    pub qualifiers: Qualifiers,
    pub storage: StorageClass,
    pub is_inline: bool,
    pub attributes: Vec<Attribute>,
}

#[derive(Debug, Clone, PartialEq)]
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
        attributes: Vec<Attribute>,
    },
    Record(RecordDecl),
    Enum(EnumDecl),
}

#[derive(Debug, Clone, PartialEq)]
pub struct ConcreteTranslationUnit {
    pub decls: Vec<ConcreteDecl>,
}
