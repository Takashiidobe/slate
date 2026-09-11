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

#[derive(Debug, Clone, PartialEq)]
pub enum Type {
    Int,
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
    Int,
    Named(String),
}

#[derive(Debug, Clone, PartialEq)]
pub enum Decl {
    Function(FunctionDecl),
    Typedef {
        name: String,
        ty: CType,
        provenance: Provenance,
    },
    Conditional(Conditional<Vec<Decl>>),
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
