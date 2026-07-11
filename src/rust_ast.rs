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
    /// Migration target for lowered functions: a string header line plus a flat,
    /// depth-annotated statement list. Control flow stays as `Stmt::Raw` scaffolding
    /// at its nesting depth (no structural blocks yet); straight-line statements are
    /// real nodes so fixups can operate on them.
    Fn(FnDef),
    /// Escape hatch for things without a modeled node yet (e.g. `use` lines).
    Raw(String),
}

#[derive(Debug, Clone)]
pub struct FnDef {
    /// The opening line through the brace, e.g. `fn add(a: i32, b: i32) -> i32 {`.
    pub open: String,
    pub body: Vec<IndentStmt>,
}

#[derive(Debug, Clone)]
pub struct IndentStmt {
    pub depth: usize,
    pub stmt: Stmt,
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
    /// A fully-formed Rust statement line spliced in as-is, sans indentation and
    /// trailing newline. The migration bridge for control flow not yet modeled.
    Raw(String),
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
    /// A method call, e.g. `p.offset(3)`. `recv` is the receiver expression.
    MethodCall {
        recv: Box<Expr>,
        method: String,
        args: Vec<Expr>,
    },
    /// A field or tuple-index access, e.g. `pair.0` or `s.len`.
    Field {
        base: Box<Expr>,
        field: String,
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
            Item::Fn(f) => {
                out.push_str(&f.open);
                out.push('\n');
                for IndentStmt { depth, stmt } in &f.body {
                    stmt.emit(out, *depth);
                }
                out.push_str("}\n");
            }
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
    // The single-line, un-indented text form, as fixups parse it. Only meaningful
    // for the flat statement forms the migrated body holds (Let/Assign/Expr/Return/Raw).
    pub fn render_line(&self) -> String {
        let mut out = String::new();
        self.emit(&mut out, 0);
        out.trim_end().to_string()
    }

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
            Stmt::Raw(line) => {
                let _ = writeln!(out, "{pad}{line}");
            }
        }
    }
}

// Rust expression binding powers (higher binds tighter). Only the operators the
// lowerer emits are modeled; anything atomic or brace/paren-delimited renders at
// PREC_ATOM so it never needs wrapping.
const PREC_CAST: u8 = 12;
const PREC_PREFIX: u8 = 13;
const PREC_CALL: u8 = 14;
const PREC_ATOM: u8 = 15;

fn binop_prec(op: &str) -> u8 {
    match op {
        "||" => 3,
        "&&" => 4,
        "==" | "!=" | "<" | ">" | "<=" | ">=" => 5,
        "|" => 6,
        "^" => 7,
        "&" => 8,
        "<<" | ">>" => 9,
        "+" | "-" => 10,
        "*" | "/" | "%" => 11,
        _ => PREC_ATOM,
    }
}

fn is_comparison(op: &str) -> bool {
    binop_prec(op) == 5
}

impl Expr {
    pub fn render(&self) -> String {
        self.render_prec(0)
    }

    // Render for splicing into arbitrary surrounding text (a `Stmt::Raw` line),
    // where the enclosing precedence is unknown. Anything that binds looser than a
    // call is wrapped so the splice can never change precedence.
    pub fn render_spliceable(&self) -> String {
        if self.prec() < PREC_CALL {
            format!("({})", self.render())
        } else {
            self.render()
        }
    }

    // Replace every `Var(name)` node with a clone of `replacement`, returning
    // whether any substitution happened. Names baked into `Raw` text are not
    // reached — the inliner falls back to a textual splice for those.
    pub fn substitute_var(&mut self, name: &str, replacement: &Expr) -> bool {
        match self {
            Expr::Var(v) if v == name => {
                *self = replacement.clone();
                true
            }
            Expr::Lit(_) | Expr::Var(_) | Expr::Raw(_) => false,
            Expr::Unary { expr, .. }
            | Expr::Cast { expr, .. }
            | Expr::Ref { expr, .. }
            | Expr::Unsafe(expr) => expr.substitute_var(name, replacement),
            Expr::Binary { lhs, rhs, .. } => {
                let l = lhs.substitute_var(name, replacement);
                let r = rhs.substitute_var(name, replacement);
                l || r
            }
            Expr::Call { func, args } => {
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
            Expr::Field { base, .. } => base.substitute_var(name, replacement),
            Expr::Macro { args, .. } => {
                let mut changed = false;
                for arg in args {
                    changed |= arg.substitute_var(name, replacement);
                }
                changed
            }
        }
    }

    fn prec(&self) -> u8 {
        match self {
            Expr::Binary { op, .. } => binop_prec(op),
            Expr::Cast { .. } => PREC_CAST,
            Expr::Unary { .. } | Expr::Ref { .. } => PREC_PREFIX,
            Expr::Call { .. } | Expr::MethodCall { .. } | Expr::Field { .. } => PREC_CALL,
            _ => PREC_ATOM,
        }
    }

    // render, wrapping in parens when this expression binds looser than the
    // enclosing position requires. Extra parens are always safe, so the rule is
    // conservative: wrap on `<`, keep bare on `>=`.
    fn render_prec(&self, min: u8) -> String {
        let inner = self.render_raw();
        if self.prec() < min {
            format!("({inner})")
        } else {
            inner
        }
    }

    fn render_raw(&self) -> String {
        match self {
            Expr::Lit(s) | Expr::Var(s) | Expr::Raw(s) => s.clone(),
            Expr::Unary { op, expr } => format!("{op}{}", render_prefix_operand(expr)),
            Expr::Binary { op, lhs, rhs } => {
                let p = binop_prec(op);
                // left-assoc: the right operand must bind strictly tighter, so a
                // same-precedence right child (`a - (b - c)`) still needs parens.
                // comparisons are non-associative, so wrap same-precedence on both
                // sides to avoid an illegal `a < b < c` chain.
                let (lmin, rmin) = if is_comparison(op) {
                    (p + 1, p + 1)
                } else {
                    (p, p + 1)
                };
                format!("{} {op} {}", lhs.render_prec(lmin), rhs.render_prec(rmin))
            }
            Expr::Call { func, args } => {
                format!("{}({})", func.render_prec(PREC_CALL), render_args(args))
            }
            Expr::MethodCall { recv, method, args } => {
                format!(
                    "{}.{method}({})",
                    recv.render_prec(PREC_CALL),
                    render_args(args)
                )
            }
            Expr::Field { base, field } => {
                format!("{}.{field}", base.render_prec(PREC_CALL))
            }
            Expr::Macro { name, args } => {
                format!("{name}!({})", render_args(args))
            }
            Expr::Unsafe(e) => format!("unsafe {{ {} }}", e.render()),
            Expr::Cast { expr, ty } => {
                format!("{} as {}", expr.render_prec(PREC_CAST), ty.render())
            }
            Expr::Ref { mutable, expr } => {
                let kw = if *mutable { "&mut " } else { "&" };
                format!("{kw}{}", render_prefix_operand(expr))
            }
        }
    }
}

// A prefix operator over another prefix form (`- -a`, `&&x`) tokenizes as `--`
// or `&&` without a barrier, so parenthesize a nested prefix operand.
fn render_prefix_operand(expr: &Expr) -> String {
    if matches!(expr, Expr::Unary { .. } | Expr::Ref { .. }) {
        format!("({})", expr.render())
    } else {
        expr.render_prec(PREC_PREFIX)
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
    let mut c: i32 = a + b;
    return c;
}
";
        assert_eq!(prog.emit(), expected);
    }

    fn var(name: &str) -> Box<Expr> {
        Box::new(Expr::Var(name.into()))
    }

    fn bin(op: &str, lhs: Box<Expr>, rhs: Box<Expr>) -> Box<Expr> {
        Box::new(Expr::Binary {
            op: op.into(),
            lhs,
            rhs,
        })
    }

    #[test]
    fn elides_parens_by_precedence() {
        // a + b * c : mul binds tighter, no parens.
        assert_eq!(
            bin("+", var("a"), bin("*", var("b"), var("c"))).render(),
            "a + b * c"
        );
        // (a + b) * c : add under mul must be wrapped.
        assert_eq!(
            bin("*", bin("+", var("a"), var("b")), var("c")).render(),
            "(a + b) * c"
        );
    }

    #[test]
    fn keeps_parens_for_left_assoc_right_child() {
        // a - (b - c) : subtraction is left-assoc, right child needs parens.
        assert_eq!(
            bin("-", var("a"), bin("-", var("b"), var("c"))).render(),
            "a - (b - c)"
        );
        // a - b - c : left nesting is the default parse, no parens.
        assert_eq!(
            bin("-", bin("-", var("a"), var("b")), var("c")).render(),
            "a - b - c"
        );
    }

    #[test]
    fn wraps_non_associative_comparison_chain() {
        assert_eq!(
            bin("<", bin("<", var("a"), var("b")), var("c")).render(),
            "(a < b) < c"
        );
    }

    #[test]
    fn shift_and_bitwise_precedence() {
        // add binds tighter than shift: no parens.
        assert_eq!(
            bin("<<", bin("+", var("a"), var("b")), var("c")).render(),
            "a + b << c"
        );
        // bitand binds tighter than comparison: no parens.
        assert_eq!(
            bin("==", bin("&", var("a"), var("b")), var("c")).render(),
            "a & b == c"
        );
    }

    #[test]
    fn wraps_looser_receiver_of_postfix() {
        // a binary receiver of a method call or field access must be wrapped.
        assert_eq!(
            Expr::MethodCall {
                recv: bin("+", var("a"), var("b")),
                method: "offset".into(),
                args: vec![Expr::Var("c".into())],
            }
            .render(),
            "(a + b).offset(c)"
        );
        assert_eq!(
            Expr::Field {
                base: Box::new(Expr::Cast {
                    expr: var("x"),
                    ty: Type::Named("i32".into()),
                }),
                field: "0".into(),
            }
            .render(),
            "(x as i32).0"
        );
        // a call/field receiver already binds tightly: no parens, chains flat.
        assert_eq!(
            Expr::Field {
                base: Box::new(Expr::MethodCall {
                    recv: var("p"),
                    method: "get".into(),
                    args: vec![],
                }),
                field: "0".into(),
            }
            .render(),
            "p.get().0"
        );
    }

    #[test]
    fn wraps_binary_under_prefix_and_cast() {
        assert_eq!(
            Expr::Unary {
                op: "-".into(),
                expr: bin("+", var("a"), var("b")),
            }
            .render(),
            "-(a + b)"
        );
        assert_eq!(
            Expr::Cast {
                expr: bin("+", var("a"), var("b")),
                ty: Type::Named("i64".into()),
            }
            .render(),
            "(a + b) as i64"
        );
    }

    #[test]
    fn separates_nested_prefix_operators() {
        assert_eq!(
            Expr::Unary {
                op: "-".into(),
                expr: Box::new(Expr::Unary {
                    op: "-".into(),
                    expr: var("a"),
                }),
            }
            .render(),
            "-(-a)"
        );
    }
}
