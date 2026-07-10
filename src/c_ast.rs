//! Clang AST oracle for source-level facts that CIR may not preserve.

use serde_json::Value;
use std::path::Path;
use std::process::Command;

/// A parsed C translation unit.
#[derive(Debug, Default, Clone)]
pub struct Unit {
    pub functions: Vec<Function>,
}

#[derive(Debug, Clone)]
pub struct Function {
    pub name: String,
    pub params: Vec<Decl>,
    pub ret: CType,
    /// `None` for a prototype with no body.
    pub body: Option<Vec<Stmt>>,
    /// Source location `line:col` of the definition, for the CIR join.
    pub loc: Option<Loc>,
    /// Raw Clang JSON node for demand-driven facts the small AST has not modeled.
    pub raw: Option<Value>,
}

#[derive(Debug, Clone)]
pub struct Decl {
    pub name: String,
    pub ty: CType,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum CType {
    Void,
    Int { signed: bool, bits: u32 },
    Ptr(Box<CType>),
    Array(Box<CType>, Option<u64>),
}

#[derive(Debug, Clone)]
pub enum Stmt {
    Decl(Decl, Option<Expr>),
    Expr(Expr),
    Return(Option<Expr>),
    For {
        init: Box<Option<Stmt>>,
        cond: Option<Expr>,
        step: Option<Expr>,
        body: Vec<Stmt>,
    },
    While {
        cond: Expr,
        body: Vec<Stmt>,
    },
    If {
        cond: Expr,
        then: Vec<Stmt>,
        otherwise: Option<Vec<Stmt>>,
    },
}

#[derive(Debug, Clone)]
pub enum Expr {
    Int(i64),
    Str(String),
    Ident(String),
    Unary {
        op: String,
        expr: Box<Expr>,
    },
    Binary {
        op: String,
        lhs: Box<Expr>,
        rhs: Box<Expr>,
    },
    Call {
        name: String,
        args: Vec<Expr>,
    },
    Assign {
        target: Box<Expr>,
        value: Box<Expr>,
    },
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct Loc {
    pub line: u32,
    pub col: u32,
}

fn clang() -> String {
    std::env::var("SLATE_CLANG").unwrap_or_else(|_| {
        format!(
            "{}/llvm-project/build-cir/bin/clang",
            std::env::var("HOME").expect("HOME not set")
        )
    })
}

/// Load Clang's JSON AST for `src` and extract a compact source-level oracle.
pub fn parse_file(src: &Path) -> Result<Unit, String> {
    let out = Command::new(clang())
        .args(["-Xclang", "-ast-dump=json", "-fsyntax-only"])
        .args(crate::cir::emit::target_args())
        .arg(src)
        .output()
        .map_err(|e| format!("spawn {}: {e}", clang()))?;
    if !out.status.success() {
        return Err(format!(
            "clang -ast-dump=json failed:\n{}",
            String::from_utf8_lossy(&out.stderr)
        ));
    }
    parse_json(
        &String::from_utf8_lossy(&out.stdout),
        &src.to_string_lossy(),
    )
}

/// Parse a Clang JSON AST dump into a compact [`Unit`].
pub fn parse(src: &str) -> Result<Unit, String> {
    parse_json(src, "")
}

pub fn parse_json(json: &str, source_file: &str) -> Result<Unit, String> {
    let root: Value =
        serde_json::from_str(json).map_err(|e| format!("parse clang AST JSON: {e}"))?;
    let mut functions = Vec::new();
    collect_functions(&root, source_file, &mut functions);
    Ok(Unit { functions })
}

fn collect_functions(node: &Value, source_file: &str, out: &mut Vec<Function>) {
    if kind(node) == Some("FunctionDecl") && is_source_node(node, source_file) && has_body(node) {
        if let Some(function) = extract_function(node) {
            out.push(function);
        }
        return;
    }
    for child in children(node) {
        collect_functions(child, source_file, out);
    }
}

fn extract_function(node: &Value) -> Option<Function> {
    let name = node.get("name")?.as_str()?.to_string();
    let fn_qual_type = qual_type(node).unwrap_or("int ()");
    let (ret, _) = parse_function_qual_type(fn_qual_type);
    let params = children(node)
        .iter()
        .filter(|child| kind(child) == Some("ParmVarDecl"))
        .filter_map(|child| {
            Some(Decl {
                name: child.get("name")?.as_str()?.to_string(),
                ty: parse_c_type(qual_type(child).unwrap_or("int")),
            })
        })
        .collect();
    let body = children(node)
        .iter()
        .find(|child| kind(child) == Some("CompoundStmt"))
        .map(|child| parse_compound_stmt(child));

    Some(Function {
        name,
        params,
        ret,
        body,
        loc: loc(node),
        raw: Some(node.clone()),
    })
}

fn parse_compound_stmt(node: &Value) -> Vec<Stmt> {
    children(node)
        .iter()
        .filter_map(|child| parse_stmt(child))
        .collect()
}

fn parse_stmt(node: &Value) -> Option<Stmt> {
    match kind(node)? {
        "CompoundStmt" => Some(Stmt::Expr(Expr::Ident("{...}".into()))),
        "DeclStmt" => parse_decl_stmt(node),
        "ReturnStmt" => Some(Stmt::Return(
            children(node).first().and_then(|child| parse_expr(child)),
        )),
        "ForStmt" => parse_for_stmt(node),
        "WhileStmt" => {
            let kids = children(node);
            Some(Stmt::While {
                cond: kids.first().and_then(|child| parse_expr(child))?,
                body: kids
                    .get(1)
                    .map_or_else(Vec::new, |child| parse_stmt_body(child)),
            })
        }
        "IfStmt" => parse_if_stmt(node),
        _ => parse_expr(node).map(Stmt::Expr),
    }
}

fn parse_decl_stmt(node: &Value) -> Option<Stmt> {
    let kids = children(node);
    let decl = kids
        .into_iter()
        .find(|child| kind(child) == Some("VarDecl"))?;
    let name = decl.get("name")?.as_str()?.to_string();
    let ty = parse_c_type(qual_type(decl).unwrap_or("int"));
    let init = children(decl).first().and_then(|child| parse_expr(child));
    Some(Stmt::Decl(Decl { name, ty }, init))
}

fn parse_for_stmt(node: &Value) -> Option<Stmt> {
    let kids = children(node);
    let init = kids.first().and_then(|child| parse_stmt(child));
    let cond = kids.get(1).and_then(|child| parse_expr(child));
    let step = kids.get(2).and_then(|child| parse_expr(child));
    let body = kids
        .get(3)
        .map_or_else(Vec::new, |child| parse_stmt_body(child));
    Some(Stmt::For {
        init: Box::new(init),
        cond,
        step,
        body,
    })
}

fn parse_if_stmt(node: &Value) -> Option<Stmt> {
    let kids = children(node);
    let cond = kids.first().and_then(|child| parse_expr(child))?;
    let then = kids
        .get(1)
        .map_or_else(Vec::new, |child| parse_stmt_body(child));
    let otherwise = kids.get(2).map(|child| parse_stmt_body(child));
    Some(Stmt::If {
        cond,
        then,
        otherwise,
    })
}

fn parse_stmt_body(node: &Value) -> Vec<Stmt> {
    if kind(node) == Some("CompoundStmt") {
        parse_compound_stmt(node)
    } else {
        parse_stmt(node).into_iter().collect()
    }
}

fn parse_expr(node: &Value) -> Option<Expr> {
    match kind(node)? {
        "IntegerLiteral" => node
            .get("value")
            .and_then(Value::as_str)
            .and_then(|s| s.parse().ok())
            .map(Expr::Int),
        "StringLiteral" => node
            .get("value")
            .and_then(Value::as_str)
            .map(|s| Expr::Str(s.to_string())),
        "DeclRefExpr" => decl_ref_name(node).map(Expr::Ident),
        "ImplicitCastExpr" | "ParenExpr" | "ExprWithCleanups" => {
            children(node).first().and_then(|child| parse_expr(child))
        }
        "UnaryOperator" => {
            let op = node.get("opcode")?.as_str()?.to_string();
            let expr = children(node).first().and_then(|child| parse_expr(child))?;
            Some(Expr::Unary {
                op,
                expr: Box::new(expr),
            })
        }
        "BinaryOperator" | "CompoundAssignOperator" => {
            let kids = children(node);
            let op = node.get("opcode")?.as_str()?.to_string();
            let lhs = kids.first().and_then(|child| parse_expr(child))?;
            let rhs = kids.get(1).and_then(|child| parse_expr(child))?;
            if kind(node) == Some("CompoundAssignOperator") {
                Some(Expr::Assign {
                    target: Box::new(lhs.clone()),
                    value: Box::new(Expr::Binary {
                        op: op.trim_end_matches('=').to_string(),
                        lhs: Box::new(lhs),
                        rhs: Box::new(rhs),
                    }),
                })
            } else if op == "=" {
                Some(Expr::Assign {
                    target: Box::new(lhs),
                    value: Box::new(rhs),
                })
            } else {
                Some(Expr::Binary {
                    op,
                    lhs: Box::new(lhs),
                    rhs: Box::new(rhs),
                })
            }
        }
        "CallExpr" => {
            let kids = children(node);
            let callee = kids.first().and_then(|child| parse_expr(child))?;
            let Expr::Ident(name) = callee else {
                return None;
            };
            let args = kids
                .iter()
                .skip(1)
                .filter_map(|child| parse_expr(child))
                .collect();
            Some(Expr::Call { name, args })
        }
        _ => children(node).first().and_then(|child| parse_expr(child)),
    }
}

fn parse_function_qual_type(s: &str) -> (CType, Vec<CType>) {
    let Some((ret, params)) = s.split_once('(') else {
        return (parse_c_type(s), Vec::new());
    };
    let params = params.trim_end_matches(')').trim();
    let params = if params.is_empty() || params == "void" {
        Vec::new()
    } else {
        params
            .split(',')
            .map(|param| parse_c_type(param.trim()))
            .collect()
    };
    (parse_c_type(ret.trim()), params)
}

fn parse_c_type(s: &str) -> CType {
    let s = s.trim();
    if s == "void" {
        CType::Void
    } else if let Some(inner) = s.strip_suffix('*') {
        CType::Ptr(Box::new(parse_c_type(inner.trim())))
    } else if let Some((inner, size)) = s.split_once('[') {
        let size = size.trim_end_matches(']').parse().ok();
        CType::Array(Box::new(parse_c_type(inner.trim())), size)
    } else if s.contains("unsigned") {
        CType::Int {
            signed: false,
            bits: int_bits(s),
        }
    } else {
        CType::Int {
            signed: true,
            bits: int_bits(s),
        }
    }
}

fn int_bits(s: &str) -> u32 {
    if s.contains("char") {
        8
    } else if s.contains("short") {
        16
    } else if s.contains("long long") {
        64
    } else if s.contains("long") {
        64
    } else {
        32
    }
}

fn has_body(node: &Value) -> bool {
    children(node)
        .iter()
        .any(|child| kind(child) == Some("CompoundStmt"))
}

fn is_source_node(node: &Value, source_file: &str) -> bool {
    if source_file.is_empty() {
        return true;
    }
    let Some(file) = node
        .get("loc")
        .and_then(|loc| loc.get("file"))
        .and_then(Value::as_str)
    else {
        return false;
    };
    file == source_file || Path::new(file) == Path::new(source_file) || file.ends_with(source_file)
}

fn loc(node: &Value) -> Option<Loc> {
    let loc = node.get("loc")?;
    Some(Loc {
        line: loc.get("line")?.as_u64()? as u32,
        col: loc.get("col")?.as_u64()? as u32,
    })
}

fn qual_type(node: &Value) -> Option<&str> {
    node.get("type")?.get("qualType")?.as_str()
}

fn kind(node: &Value) -> Option<&str> {
    node.get("kind")?.as_str()
}

fn children(node: &Value) -> Vec<&Value> {
    node.get("inner")
        .and_then(Value::as_array)
        .map(|items| items.iter().collect())
        .unwrap_or_default()
}

fn decl_ref_name(node: &Value) -> Option<String> {
    node.get("referencedDecl")
        .and_then(|decl| decl.get("name"))
        .and_then(Value::as_str)
        .or_else(|| node.get("name").and_then(Value::as_str))
        .map(str::to_string)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn extracts_source_functions_from_clang_json() {
        let ast = r#"
{
  "kind": "TranslationUnitDecl",
  "inner": [
    {
      "kind": "FunctionDecl",
      "name": "printf",
      "loc": {"file": "/usr/include/stdio.h", "line": 1, "col": 5},
      "type": {"qualType": "int (const char *, ...)"}
    },
    {
      "kind": "FunctionDecl",
      "name": "add",
      "loc": {"file": "tests/fixtures/add.c", "line": 3, "col": 12},
      "type": {"qualType": "int (int, int)"},
      "inner": [
        {"kind": "ParmVarDecl", "name": "a", "type": {"qualType": "int"}},
        {"kind": "ParmVarDecl", "name": "b", "type": {"qualType": "int"}},
        {"kind": "CompoundStmt", "inner": [
          {"kind": "DeclStmt", "inner": [
            {"kind": "VarDecl", "name": "c", "type": {"qualType": "int"}, "inner": [
              {"kind": "BinaryOperator", "opcode": "+", "inner": [
                {"kind": "ImplicitCastExpr", "inner": [{"kind": "DeclRefExpr", "referencedDecl": {"name": "a"}}]},
                {"kind": "ImplicitCastExpr", "inner": [{"kind": "DeclRefExpr", "referencedDecl": {"name": "b"}}]}
              ]}
            ]}
          ]},
          {"kind": "ReturnStmt", "inner": [
            {"kind": "ImplicitCastExpr", "inner": [{"kind": "DeclRefExpr", "referencedDecl": {"name": "c"}}]}
          ]}
        ]}
      ]
    }
  ]
}
"#;

        let unit = parse_json(ast, "tests/fixtures/add.c").unwrap();
        assert_eq!(unit.functions.len(), 1);
        let add = &unit.functions[0];
        assert_eq!(add.name, "add");
        assert_eq!(add.loc, Some(Loc { line: 3, col: 12 }));
        assert_eq!(add.params.len(), 2);
        assert_eq!(add.params[0].name, "a");
        assert!(add.raw.is_some());
        let body = add.body.as_ref().unwrap();
        assert!(matches!(body[0], Stmt::Decl(_, Some(Expr::Binary { .. }))));
        assert!(matches!(body[1], Stmt::Return(Some(Expr::Ident(ref name))) if name == "c"));
    }

    #[test]
    fn extracts_for_loop_and_calls_from_clang_json() {
        let ast = r#"
{
  "kind": "TranslationUnitDecl",
  "inner": [
    {
      "kind": "FunctionDecl",
      "name": "sum_to",
      "loc": {"file": "loop_sum.c", "line": 3, "col": 12},
      "type": {"qualType": "int (int)"},
      "inner": [
        {"kind": "ParmVarDecl", "name": "n", "type": {"qualType": "int"}},
        {"kind": "CompoundStmt", "inner": [
          {"kind": "ForStmt", "inner": [
            {"kind": "DeclStmt", "inner": [{"kind": "VarDecl", "name": "i", "type": {"qualType": "int"}, "inner": [{"kind": "IntegerLiteral", "value": "1"}]}]},
            {"kind": "BinaryOperator", "opcode": "<=", "inner": [
              {"kind": "ImplicitCastExpr", "inner": [{"kind": "DeclRefExpr", "referencedDecl": {"name": "i"}}]},
              {"kind": "ImplicitCastExpr", "inner": [{"kind": "DeclRefExpr", "referencedDecl": {"name": "n"}}]}
            ]},
            {"kind": "UnaryOperator", "opcode": "++", "inner": [{"kind": "DeclRefExpr", "referencedDecl": {"name": "i"}}]},
            {"kind": "CompoundStmt", "inner": [
              {"kind": "CompoundAssignOperator", "opcode": "+=", "inner": [
                {"kind": "DeclRefExpr", "referencedDecl": {"name": "total"}},
                {"kind": "ImplicitCastExpr", "inner": [{"kind": "DeclRefExpr", "referencedDecl": {"name": "i"}}]}
              ]}
            ]}
          ]}
        ]}
      ]
    },
    {
      "kind": "FunctionDecl",
      "name": "main",
      "loc": {"file": "loop_sum.c", "line": 11, "col": 5},
      "type": {"qualType": "int (void)"},
      "inner": [
        {"kind": "CompoundStmt", "inner": [
          {"kind": "CallExpr", "inner": [
            {"kind": "ImplicitCastExpr", "inner": [{"kind": "DeclRefExpr", "referencedDecl": {"name": "printf"}}]},
            {"kind": "StringLiteral", "value": "\"%d\\n\""},
            {"kind": "CallExpr", "inner": [
              {"kind": "ImplicitCastExpr", "inner": [{"kind": "DeclRefExpr", "referencedDecl": {"name": "sum_to"}}]},
              {"kind": "IntegerLiteral", "value": "10"}
            ]}
          ]}
        ]}
      ]
    }
  ]
}
"#;

        let unit = parse_json(ast, "loop_sum.c").unwrap();
        assert_eq!(unit.functions.len(), 2);
        let body = unit.functions[0].body.as_ref().unwrap();
        assert!(matches!(body[0], Stmt::For { .. }));
        let main_body = unit.functions[1].body.as_ref().unwrap();
        assert!(
            matches!(main_body[0], Stmt::Expr(Expr::Call { ref name, .. }) if name == "printf")
        );
    }
}
