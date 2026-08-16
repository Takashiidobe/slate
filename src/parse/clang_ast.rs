pub use clang_ast::SourceLocation;
use clang_ast::{Id, SourceRange};
use serde::Deserialize;
use std::path::{Path, PathBuf};
use std::process::{Command, ExitStatus};
use thiserror::Error;

#[derive(Deserialize, Debug)]
pub struct QualType {
    #[serde(rename = "qualType")]
    pub qual_type: String,
    #[serde(default, rename = "desugaredQualType")]
    pub desugared_qual_type: Option<String>,
}

impl QualType {
    pub fn canonical(&self) -> &str {
        self.desugared_qual_type
            .as_deref()
            .unwrap_or(&self.qual_type)
    }
}

#[derive(Deserialize, Debug)]
pub struct BareDeclRef {
    pub id: Id,
    pub kind: String,
    pub name: Option<String>,
    #[serde(rename = "type")]
    pub qual_type: Option<QualType>,
}

#[derive(Deserialize, Debug, Default)]
pub struct Decl {
    pub loc: Option<SourceLocation>,
    pub range: Option<SourceRange>,
    pub name: Option<String>,
    #[serde(rename = "mangledName")]
    pub mangled_name: Option<String>,
    #[serde(rename = "type")]
    pub qual_type: Option<QualType>,
    #[serde(rename = "storageClass")]
    pub storage_class: Option<String>,
    #[serde(default)]
    pub variadic: bool,
    pub init: Option<String>,
    #[serde(default, rename = "isUsed")]
    pub is_used: bool,
    #[serde(default, rename = "isImplicit")]
    pub is_implicit: bool,
    #[serde(default, rename = "isReferenced")]
    pub is_referenced: bool,
}

#[derive(Deserialize, Debug, Default)]
pub struct Record {
    pub loc: Option<SourceLocation>,
    pub range: Option<SourceRange>,
    #[serde(rename = "tagUsed")]
    pub tag_used: Option<String>,
    pub name: Option<String>,
    #[serde(default, rename = "completeDefinition")]
    pub complete_definition: bool,
    #[serde(default, rename = "isImplicit")]
    pub is_implicit: bool,
}

#[derive(Deserialize, Debug, Default)]
pub struct Stmt {
    pub range: Option<SourceRange>,
}

#[derive(Deserialize, Debug, Default)]
pub struct Expr {
    pub range: Option<SourceRange>,
    #[serde(rename = "type")]
    pub qual_type: Option<QualType>,
    #[serde(rename = "valueCategory")]
    pub value_category: Option<String>,
}

#[derive(Deserialize, Debug)]
pub struct CastExpr {
    pub range: Option<SourceRange>,
    #[serde(rename = "type")]
    pub qual_type: Option<QualType>,
    #[serde(rename = "valueCategory")]
    pub value_category: Option<String>,
    #[serde(rename = "castKind")]
    pub cast_kind: String,
}

#[derive(Deserialize, Debug)]
pub struct DeclRefExpr {
    pub range: Option<SourceRange>,
    #[serde(rename = "type")]
    pub qual_type: Option<QualType>,
    #[serde(rename = "valueCategory")]
    pub value_category: Option<String>,
    #[serde(rename = "referencedDecl")]
    pub referenced_decl: BareDeclRef,
}

#[derive(Deserialize, Debug)]
pub struct BinaryOperator {
    pub range: Option<SourceRange>,
    #[serde(rename = "type")]
    pub qual_type: Option<QualType>,
    #[serde(rename = "valueCategory")]
    pub value_category: Option<String>,
    pub opcode: String,
}

#[derive(Deserialize, Debug)]
pub struct UnaryOperator {
    pub range: Option<SourceRange>,
    #[serde(rename = "type")]
    pub qual_type: Option<QualType>,
    #[serde(rename = "valueCategory")]
    pub value_category: Option<String>,
    pub opcode: String,
    #[serde(default, rename = "isPostfix")]
    pub is_postfix: bool,
}

#[derive(Deserialize, Debug)]
pub struct Literal {
    pub range: Option<SourceRange>,
    #[serde(rename = "type")]
    pub qual_type: Option<QualType>,
    pub value: String,
}

#[derive(Deserialize, Debug, Default)]
pub struct SizeOfAlignOfExpr {
    pub range: Option<SourceRange>,
    #[serde(rename = "type")]
    pub qual_type: Option<QualType>,
    pub name: Option<String>,
    #[serde(rename = "argType")]
    pub arg_type: Option<QualType>,
}

#[derive(Deserialize, Debug)]
pub struct CharLiteral {
    pub range: Option<SourceRange>,
    #[serde(rename = "type")]
    pub qual_type: Option<QualType>,
    pub value: i64,
}

#[derive(Deserialize, Debug, Default)]
pub struct MemberExpr {
    pub range: Option<SourceRange>,
    #[serde(rename = "type")]
    pub qual_type: Option<QualType>,
    #[serde(rename = "valueCategory")]
    pub value_category: Option<String>,
    pub name: Option<String>,
    #[serde(default, rename = "isArrow")]
    pub is_arrow: bool,
    #[serde(rename = "referencedMemberDecl")]
    pub referenced_member_decl: Option<String>,
}

#[derive(Deserialize, Debug, Default)]
pub struct Other {
    #[serde(default)]
    pub kind: Option<String>,
    pub name: Option<String>,
    #[serde(rename = "tagUsed")]
    pub tag_used: Option<String>,
    #[serde(rename = "type")]
    pub qual_type: Option<QualType>,
    pub loc: Option<SourceLocation>,
    pub value: Option<serde_json::Value>,
    #[serde(default, rename = "targetLabelDeclId")]
    pub target_label_decl_id: Option<Id>,
    #[serde(default, rename = "declId")]
    pub decl_id: Option<Id>,
}

#[derive(Deserialize, Debug)]
pub enum Clang {
    TranslationUnitDecl,
    FunctionDecl(Decl),
    ParmVarDecl(Decl),
    VarDecl(Decl),
    FieldDecl(Decl),
    RecordDecl(Record),
    TypedefDecl(Decl),
    EnumDecl(Decl),
    EnumConstantDecl(Decl),
    CompoundStmt(Stmt),
    DeclStmt(Stmt),
    ReturnStmt(Stmt),
    IfStmt(Stmt),
    ForStmt(Stmt),
    WhileStmt(Stmt),
    DoStmt(Stmt),
    BreakStmt(Stmt),
    ContinueStmt(Stmt),
    NullStmt(Stmt),
    SwitchStmt(Stmt),
    CaseStmt(Stmt),
    DefaultStmt(Stmt),
    ParenExpr(Expr),
    BinaryOperator(BinaryOperator),
    CompoundAssignOperator(BinaryOperator),
    UnaryOperator(UnaryOperator),
    DeclRefExpr(DeclRefExpr),
    ImplicitCastExpr(CastExpr),
    CStyleCastExpr(CastExpr),
    CallExpr(Expr),
    MemberExpr(MemberExpr),
    ArraySubscriptExpr(Expr),
    InitListExpr(Expr),
    ConditionalOperator(Expr),
    IntegerLiteral(Literal),
    FloatingLiteral(Literal),
    CharacterLiteral(CharLiteral),
    StringLiteral(Literal),
    UnaryExprOrTypeTraitExpr(SizeOfAlignOfExpr),
    Other(Other),
}

pub type Node = clang_ast::Node<Clang>;

#[derive(Debug, Error)]
pub enum ClangAstError {
    #[error("configure target for clang AST dump of {path}: {source}")]
    Target {
        path: PathBuf,
        #[source]
        source: crate::cir::TargetError,
    },
    #[error("spawn {clang} for clang AST dump of {path}: {source}")]
    Spawn {
        clang: String,
        path: PathBuf,
        #[source]
        source: std::io::Error,
    },
    #[error("clang AST dump failed for {path} with {status}:\n{stderr}")]
    ClangFailed {
        path: PathBuf,
        status: ExitStatus,
        stderr: String,
    },
    #[error("parse clang AST JSON for {path}: {source}")]
    ParseJson {
        path: PathBuf,
        #[source]
        source: serde_json::Error,
    },
}

fn clang() -> String {
    std::env::var("SLATE_CC").unwrap_or_else(|_| "clang".to_string())
}

pub fn dump_json(src: &Path, extra_args: &[String]) -> Result<String, ClangAstError> {
    let clang = clang();
    let target_args = crate::cir::emit::target_args().map_err(|source| ClangAstError::Target {
        path: src.to_path_buf(),
        source,
    })?;
    let out = Command::new(&clang)
        .args(["-std=gnu23", "-Xclang", "-ast-dump=json", "-fsyntax-only"])
        .args(target_args)
        .args(extra_args)
        .arg(src)
        .output()
        .map_err(|source| ClangAstError::Spawn {
            clang: clang.clone(),
            path: src.to_path_buf(),
            source,
        })?;
    if !out.status.success() {
        return Err(ClangAstError::ClangFailed {
            path: src.to_path_buf(),
            status: out.status,
            stderr: String::from_utf8_lossy(&out.stderr).into_owned(),
        });
    }
    Ok(String::from_utf8_lossy(&out.stdout).into_owned())
}

pub fn parse_file(src: &Path, extra_args: &[String]) -> Result<Node, ClangAstError> {
    let json = dump_json(src, extra_args)?;
    let mut deserializer = serde_json::Deserializer::from_str(&json);
    deserializer.disable_recursion_limit();
    let deserializer = serde_stacker::Deserializer::new(&mut deserializer);
    Node::deserialize(deserializer).map_err(|source| ClangAstError::ParseJson {
        path: src.to_path_buf(),
        source,
    })
}

fn write_loc(out: &mut String, loc: &SourceLocation) {
    use std::fmt::Write;
    if let Some(spelling) = &loc.spelling_loc {
        write!(
            out,
            " @ {}:{}:{}",
            spelling.file, spelling.line, spelling.col
        )
        .unwrap();
    }
}

fn write_range(out: &mut String, range: &SourceRange) {
    write_loc(out, &range.begin);
}

pub fn dump_tree(node: &Node, depth: usize, out: &mut String) {
    use std::fmt::Write;

    let indent = "  ".repeat(depth);
    write!(out, "{indent}[{}] ", node.id).unwrap();
    match &node.kind {
        Clang::TranslationUnitDecl => write!(out, "TranslationUnitDecl").unwrap(),
        Clang::FunctionDecl(d) | Clang::ParmVarDecl(d) | Clang::VarDecl(d) => {
            let kind = match &node.kind {
                Clang::FunctionDecl(_) => "FunctionDecl",
                Clang::ParmVarDecl(_) => "ParmVarDecl",
                _ => "VarDecl",
            };
            write!(out, "{kind}").unwrap();
            if let Some(name) = &d.name {
                write!(out, " name={name:?}").unwrap();
            }
            if let Some(ty) = &d.qual_type {
                write!(out, " type={:?}", ty.qual_type).unwrap();
            }
            if let Some(sc) = &d.storage_class {
                write!(out, " storageClass={sc:?}").unwrap();
            }
            if d.variadic {
                write!(out, " variadic").unwrap();
            }
            if let Some(loc) = &d.loc {
                write_loc(out, loc);
            }
            if let Some(range) = &d.range {
                write_range(out, range);
            }
        }
        Clang::FieldDecl(d)
        | Clang::TypedefDecl(d)
        | Clang::EnumDecl(d)
        | Clang::EnumConstantDecl(d) => {
            let kind = match &node.kind {
                Clang::FieldDecl(_) => "FieldDecl",
                Clang::TypedefDecl(_) => "TypedefDecl",
                Clang::EnumDecl(_) => "EnumDecl",
                _ => "EnumConstantDecl",
            };
            write!(out, "{kind}").unwrap();
            if let Some(name) = &d.name {
                write!(out, " name={name:?}").unwrap();
            }
            if let Some(ty) = &d.qual_type {
                write!(out, " type={:?}", ty.qual_type).unwrap();
            }
            if let Some(loc) = &d.loc {
                write_loc(out, loc);
            }
            if let Some(range) = &d.range {
                write_range(out, range);
            }
        }
        Clang::RecordDecl(r) => {
            write!(out, "RecordDecl").unwrap();
            if let Some(tag) = &r.tag_used {
                write!(out, " tagUsed={tag:?}").unwrap();
            }
            if let Some(name) = &r.name {
                write!(out, " name={name:?}").unwrap();
            }
            if let Some(loc) = &r.loc {
                write_loc(out, loc);
            }
            if let Some(range) = &r.range {
                write_range(out, range);
            }
        }
        Clang::CompoundStmt(s)
        | Clang::DeclStmt(s)
        | Clang::ReturnStmt(s)
        | Clang::IfStmt(s)
        | Clang::ForStmt(s)
        | Clang::WhileStmt(s)
        | Clang::DoStmt(s)
        | Clang::BreakStmt(s)
        | Clang::ContinueStmt(s)
        | Clang::SwitchStmt(s)
        | Clang::CaseStmt(s)
        | Clang::DefaultStmt(s)
        | Clang::NullStmt(s) => {
            let kind = match &node.kind {
                Clang::CompoundStmt(_) => "CompoundStmt",
                Clang::DeclStmt(_) => "DeclStmt",
                Clang::ReturnStmt(_) => "ReturnStmt",
                Clang::IfStmt(_) => "IfStmt",
                Clang::ForStmt(_) => "ForStmt",
                Clang::WhileStmt(_) => "WhileStmt",
                Clang::DoStmt(_) => "DoStmt",
                Clang::BreakStmt(_) => "BreakStmt",
                Clang::ContinueStmt(_) => "ContinueStmt",
                Clang::SwitchStmt(_) => "SwitchStmt",
                Clang::CaseStmt(_) => "CaseStmt",
                Clang::DefaultStmt(_) => "DefaultStmt",
                _ => "NullStmt",
            };
            write!(out, "{kind}").unwrap();
            if let Some(range) = &s.range {
                write_range(out, range);
            }
        }
        Clang::ParenExpr(e)
        | Clang::CallExpr(e)
        | Clang::ArraySubscriptExpr(e)
        | Clang::InitListExpr(e)
        | Clang::ConditionalOperator(e) => {
            let kind = match &node.kind {
                Clang::ParenExpr(_) => "ParenExpr",
                Clang::CallExpr(_) => "CallExpr",
                Clang::ArraySubscriptExpr(_) => "ArraySubscriptExpr",
                Clang::InitListExpr(_) => "InitListExpr",
                _ => "ConditionalOperator",
            };
            write!(out, "{kind}").unwrap();
            if let Some(ty) = &e.qual_type {
                write!(out, " type={:?}", ty.qual_type).unwrap();
            }
            if let Some(range) = &e.range {
                write_range(out, range);
            }
        }
        Clang::BinaryOperator(b) | Clang::CompoundAssignOperator(b) => {
            let kind = match &node.kind {
                Clang::BinaryOperator(_) => "BinaryOperator",
                _ => "CompoundAssignOperator",
            };
            write!(out, "{kind} opcode={:?}", b.opcode).unwrap();
            if let Some(ty) = &b.qual_type {
                write!(out, " type={:?}", ty.qual_type).unwrap();
            }
            if let Some(range) = &b.range {
                write_range(out, range);
            }
        }
        Clang::UnaryOperator(u) => {
            write!(
                out,
                "UnaryOperator opcode={:?} isPostfix={}",
                u.opcode, u.is_postfix
            )
            .unwrap();
            if let Some(ty) = &u.qual_type {
                write!(out, " type={:?}", ty.qual_type).unwrap();
            }
            if let Some(range) = &u.range {
                write_range(out, range);
            }
        }
        Clang::DeclRefExpr(r) => {
            write!(
                out,
                "DeclRefExpr referencedDecl=[{}] {}",
                r.referenced_decl.id, r.referenced_decl.kind
            )
            .unwrap();
            if let Some(name) = &r.referenced_decl.name {
                write!(out, " name={name:?}").unwrap();
            }
            if let Some(ty) = &r.qual_type {
                write!(out, " type={:?}", ty.qual_type).unwrap();
            }
            if let Some(range) = &r.range {
                write_range(out, range);
            }
        }
        Clang::ImplicitCastExpr(c) | Clang::CStyleCastExpr(c) => {
            let kind = match &node.kind {
                Clang::ImplicitCastExpr(_) => "ImplicitCastExpr",
                _ => "CStyleCastExpr",
            };
            write!(out, "{kind} castKind={:?}", c.cast_kind).unwrap();
            if let Some(ty) = &c.qual_type {
                write!(out, " type={:?}", ty.qual_type).unwrap();
            }
            if let Some(range) = &c.range {
                write_range(out, range);
            }
        }
        Clang::MemberExpr(m) => {
            write!(out, "MemberExpr").unwrap();
            if let Some(name) = &m.name {
                write!(out, " name={name:?}").unwrap();
            }
            write!(out, " isArrow={}", m.is_arrow).unwrap();
            if let Some(ty) = &m.qual_type {
                write!(out, " type={:?}", ty.qual_type).unwrap();
            }
            if let Some(range) = &m.range {
                write_range(out, range);
            }
        }
        Clang::IntegerLiteral(l) | Clang::FloatingLiteral(l) | Clang::StringLiteral(l) => {
            let kind = match &node.kind {
                Clang::IntegerLiteral(_) => "IntegerLiteral",
                Clang::FloatingLiteral(_) => "FloatingLiteral",
                _ => "StringLiteral",
            };
            write!(out, "{kind} value={:?}", l.value).unwrap();
            if let Some(ty) = &l.qual_type {
                write!(out, " type={:?}", ty.qual_type).unwrap();
            }
            if let Some(range) = &l.range {
                write_range(out, range);
            }
        }
        Clang::CharacterLiteral(l) => {
            write!(out, "CharacterLiteral value={}", l.value).unwrap();
            if let Some(ty) = &l.qual_type {
                write!(out, " type={:?}", ty.qual_type).unwrap();
            }
            if let Some(range) = &l.range {
                write_range(out, range);
            }
        }
        Clang::UnaryExprOrTypeTraitExpr(s) => {
            write!(out, "UnaryExprOrTypeTraitExpr name={:?}", s.name).unwrap();
            if let Some(ty) = &s.arg_type {
                write!(out, " argType={:?}", ty.qual_type).unwrap();
            }
            if let Some(ty) = &s.qual_type {
                write!(out, " type={:?}", ty.qual_type).unwrap();
            }
            if let Some(range) = &s.range {
                write_range(out, range);
            }
        }
        Clang::Other(o) => {
            write!(out, "{}", o.kind.as_deref().unwrap_or("<unknown>")).unwrap();
            if let Some(name) = &o.name {
                write!(out, " name={name:?}").unwrap();
            }
            if let Some(tag_used) = &o.tag_used {
                write!(out, " tagUsed={tag_used:?}").unwrap();
            }
            if let Some(ty) = &o.qual_type {
                write!(out, " type={:?}", ty.qual_type).unwrap();
            }
            if let Some(loc) = &o.loc {
                write_loc(out, loc);
            }
        }
    }
    out.push('\n');
    for child in &node.inner {
        dump_tree(child, depth + 1, out);
    }
}
