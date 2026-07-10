//! The Rust AST and its textual emitter — the pipeline's output side.
//!
//! Lowering builds this tree; [`Program::emit`] prints it. The emitter never
//! concatenates source ad hoc: every node knows how to print itself, indentation
//! is structural, and binary expressions are fully parenthesized so precedence is
//! never in question. V0 output is meant to be correct, not pretty; the
//! idiomatization passes (see docs/idiomatization.md) clean it up later.

use std::fmt::Write;

#[derive(Debug, Default, Clone)]
pub struct Program {
    pub items: Vec<Item>,
}

#[derive(Debug, Clone)]
pub enum Item {
    Func(Func),
    /// Escape hatch for things without a modeled node yet (e.g. `use` lines).
    Raw(String),
}

#[derive(Debug, Clone)]
pub struct Func {
    pub name: String,
    pub params: Vec<Param>,
    pub ret: Option<Type>,
    pub body: Block,
}

#[derive(Debug, Clone)]
pub struct Param {
    pub name: String,
    pub ty: Type,
}

#[derive(Debug, Default, Clone)]
pub struct Block {
    pub stmts: Vec<Stmt>,
}

#[derive(Debug, Clone)]
pub enum Stmt {
    Let {
        name: String,
        mutable: bool,
        ty: Option<Type>,
        init: Option<Expr>,
    },
    Assign {
        target: Expr,
        value: Expr,
    },
    Expr(Expr),
    Return(Option<Expr>),
    While {
        cond: Expr,
        body: Block,
    },
    Block(Block),
}

#[derive(Debug, Clone)]
pub enum Expr {
    /// A literal or identifier printed verbatim (numbers, `true`, names).
    Lit(String),
    Var(String),
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
        func: Box<Expr>,
        args: Vec<Expr>,
    },
    /// A macro invocation, e.g. `println!(...)`. `name` excludes the `!`.
    Macro {
        name: String,
        args: Vec<Expr>,
    },
    Unsafe(Box<Expr>),
    Cast {
        expr: Box<Expr>,
        ty: Type,
    },
    Ref {
        mutable: bool,
        expr: Box<Expr>,
    },
    /// Fully-formed Rust text spliced in as-is (e.g. `libc::printf`).
    Raw(String),
}

#[derive(Debug, Clone)]
pub enum Type {
    Named(String),
    Ptr { mutable: bool, inner: Box<Type> },
    Unit,
}

// ---------------------------------------------------------------------------
// Emitter
// ---------------------------------------------------------------------------

const INDENT: &str = "    ";

impl Program {
    pub fn emit(&self) -> String {
        let mut out = String::new();
        for (i, item) in self.items.iter().enumerate() {
            if i > 0 {
                out.push('\n');
            }
            item.emit(&mut out);
        }
        out
    }
}

impl Item {
    fn emit(&self, out: &mut String) {
        match self {
            Item::Func(f) => f.emit(out),
            Item::Raw(s) => {
                out.push_str(s);
                out.push('\n');
            }
        }
    }
}

impl Func {
    fn emit(&self, out: &mut String) {
        let _ = write!(out, "fn {}(", self.name);
        for (i, p) in self.params.iter().enumerate() {
            if i > 0 {
                out.push_str(", ");
            }
            let _ = write!(out, "{}: {}", p.name, p.ty.render());
        }
        out.push(')');
        if let Some(ret) = &self.ret {
            let _ = write!(out, " -> {}", ret.render());
        }
        out.push_str(" {\n");
        self.body.emit(out, 1);
        out.push_str("}\n");
    }
}

impl Block {
    fn emit(&self, out: &mut String, depth: usize) {
        for stmt in &self.stmts {
            stmt.emit(out, depth);
        }
    }
}

impl Stmt {
    fn emit(&self, out: &mut String, depth: usize) {
        let pad = INDENT.repeat(depth);
        match self {
            Stmt::Let {
                name,
                mutable,
                ty,
                init,
            } => {
                out.push_str(&pad);
                out.push_str("let ");
                if *mutable {
                    out.push_str("mut ");
                }
                out.push_str(name);
                if let Some(ty) = ty {
                    let _ = write!(out, ": {}", ty.render());
                }
                if let Some(init) = init {
                    let _ = write!(out, " = {}", init.render());
                }
                out.push_str(";\n");
            }
            Stmt::Assign { target, value } => {
                let _ = writeln!(out, "{pad}{} = {};", target.render(), value.render());
            }
            Stmt::Expr(e) => {
                let _ = writeln!(out, "{pad}{};", e.render());
            }
            Stmt::Return(e) => match e {
                Some(e) => {
                    let _ = writeln!(out, "{pad}return {};", e.render());
                }
                None => {
                    let _ = writeln!(out, "{pad}return;");
                }
            },
            Stmt::While { cond, body } => {
                let _ = writeln!(out, "{pad}while {} {{", cond.render());
                body.emit(out, depth + 1);
                let _ = writeln!(out, "{pad}}}");
            }
            Stmt::Block(b) => {
                let _ = writeln!(out, "{pad}{{");
                b.emit(out, depth + 1);
                let _ = writeln!(out, "{pad}}}");
            }
        }
    }
}

impl Expr {
    pub fn render(&self) -> String {
        match self {
            Expr::Lit(s) | Expr::Var(s) | Expr::Raw(s) => s.clone(),
            Expr::Unary { op, expr } => format!("{op}{}", expr.render()),
            Expr::Binary { op, lhs, rhs } => {
                format!("({} {op} {})", lhs.render(), rhs.render())
            }
            Expr::Call { func, args } => {
                format!("{}({})", func.render(), render_args(args))
            }
            Expr::Macro { name, args } => {
                format!("{name}!({})", render_args(args))
            }
            Expr::Unsafe(e) => format!("unsafe {{ {} }}", e.render()),
            Expr::Cast { expr, ty } => format!("{} as {}", expr.render(), ty.render()),
            Expr::Ref { mutable, expr } => {
                let kw = if *mutable { "&mut " } else { "&" };
                format!("{kw}{}", expr.render())
            }
        }
    }
}

fn render_args(args: &[Expr]) -> String {
    args.iter().map(Expr::render).collect::<Vec<_>>().join(", ")
}

impl Type {
    pub fn render(&self) -> String {
        match self {
            Type::Named(n) => n.clone(),
            Type::Ptr { mutable, inner } => {
                let kw = if *mutable { "*mut " } else { "*const " };
                format!("{kw}{}", inner.render())
            }
            Type::Unit => "()".to_string(),
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn emits_add_function() {
        // fn add(a: i32, b: i32) -> i32 { let mut c: i32 = (a + b); return c; }
        let prog = Program {
            items: vec![Item::Func(Func {
                name: "add".into(),
                params: vec![
                    Param {
                        name: "a".into(),
                        ty: Type::Named("i32".into()),
                    },
                    Param {
                        name: "b".into(),
                        ty: Type::Named("i32".into()),
                    },
                ],
                ret: Some(Type::Named("i32".into())),
                body: Block {
                    stmts: vec![
                        Stmt::Let {
                            name: "c".into(),
                            mutable: true,
                            ty: Some(Type::Named("i32".into())),
                            init: Some(Expr::Binary {
                                op: "+".into(),
                                lhs: Box::new(Expr::Var("a".into())),
                                rhs: Box::new(Expr::Var("b".into())),
                            }),
                        },
                        Stmt::Return(Some(Expr::Var("c".into()))),
                    ],
                },
            })],
        };

        let expected = "\
fn add(a: i32, b: i32) -> i32 {
    let mut c: i32 = (a + b);
    return c;
}
";
        assert_eq!(prog.emit(), expected);
    }
}
