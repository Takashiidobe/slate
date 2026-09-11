#[derive(Debug, Clone, PartialEq)]
pub enum Condition {
    Defined(String),
    Not(Box<Condition>),
}

#[derive(Debug, Clone, PartialEq)]
pub struct Conditional<T> {
    pub branches: Vec<(Condition, T)>,
}

#[derive(Debug, Clone, PartialEq)]
pub enum Expr {
    IntLit(i64),
}

#[derive(Debug, Clone, PartialEq)]
pub enum Stmt {
    Return(Expr),
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
}

#[derive(Debug, Clone, PartialEq)]
pub struct FunctionDecl {
    pub ret_type: Type,
    pub name: String,
    pub body: Vec<Stmt>,
    pub provenance: Provenance,
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
}

#[derive(Debug, Clone, PartialEq)]
pub struct DeclarationSpecifiers {
    pub ty: CType,
    pub qualifiers: Qualifiers,
    pub storage: StorageClass,
}

#[derive(Debug, Clone, PartialEq)]
pub struct Declaration {
    pub specifiers: DeclarationSpecifiers,
    pub declarator: Declarator,
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
    },
    Conditional(Conditional<Vec<Decl>>),
}

impl Decl {
    pub fn name(&self) -> Option<&str> {
        match self {
            Self::Function(function) => Some(&function.name),
            Self::Declaration { declaration, .. } => declaration.declarator.name(),
            Self::Typedef { name, .. } => Some(name),
            Self::Conditional(_) => None,
        }
    }

    pub fn provenance(&self) -> Option<FileId> {
        match self {
            Self::Function(function) => Some(function.provenance.file),
            Self::Declaration { provenance, .. } | Self::Typedef { provenance, .. } => {
                Some(provenance.file)
            }
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
}

#[derive(Debug, Clone, PartialEq)]
pub struct ConcreteFunctionDecl {
    pub ret_type: Type,
    pub name: String,
    pub body: Vec<ConcreteStmt>,
    pub provenance: Provenance,
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
    },
}

#[derive(Debug, Clone, PartialEq)]
pub struct ConcreteTranslationUnit {
    pub decls: Vec<ConcreteDecl>,
}
