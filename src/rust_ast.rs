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
    /// Migration target for lowered functions. Some scaffolding still uses
    /// `Stmt::Raw`, but common control-flow and straight-line statements are
    /// structured so fixups can operate on them.
    Fn(FnDef),
    CrateAttrs(Vec<String>),
    Mod {
        name: String,
    },
    Use {
        path: String,
    },
    Static {
        vis: Option<String>,
        mutable: bool,
        name: String,
        ty: Type,
        init: Expr,
    },
    ExternBlock {
        abi: String,
        decls: Vec<ExternDecl>,
    },
    /// A C enum lowered as a group of `const NAME: i32 = value;` items.
    Enum(Vec<EnumConst>),
    Record(RecordDef),
    /// Escape hatch for things without a modeled node yet (runtime preludes).
    Raw(String),
}

#[derive(Debug, Clone)]
pub struct EnumConst {
    pub name: String,
    pub value: i64,
}

#[derive(Debug, Clone)]
pub struct RecordDef {
    pub is_union: bool,
    pub allow_non_camel_case: bool,
    pub name: String,
    pub fields: Vec<(String, Type)>,
}

#[derive(Debug, Clone)]
pub enum ExternDecl {
    Fn(ExternFnDecl),
    Static {
        mutable: bool,
        name: String,
        ty: Type,
    },
}

#[derive(Debug, Clone)]
pub struct ExternFnDecl {
    pub name: String,
    pub params: Vec<FnParam>,
    pub variadic: bool,
    pub ret: Option<Type>,
}

#[derive(Debug, Clone)]
pub struct FnDef {
    pub vis: Option<String>,
    pub unsafe_extern_c: bool,
    pub name: String,
    pub params: Vec<FnParam>,
    pub ret: Option<Type>,
    pub body: Vec<IndentStmt>,
}

#[derive(Debug, Clone)]
pub struct FnParam {
    pub name: String,
    pub mutable: bool,
    pub ty: Type,
}

#[derive(Debug, Clone)]
pub struct IndentStmt {
    pub depth: usize,
    pub stmt: Stmt,
}

#[derive(Debug, Clone)]
pub struct MatchArm {
    pub pattern: String,
    pub body: Vec<IndentStmt>,
}

#[derive(Debug, Clone)]
pub struct ExprMatchArm {
    pub pattern: String,
    pub value: Expr,
}

#[derive(Debug, Clone, Copy)]
pub enum AtomicType {
    I8,
    U8,
    I16,
    U16,
    I32,
    U32,
    I64,
    U64,
    Isize,
    Usize,
    Bool,
}

#[derive(Debug, Clone, Copy)]
pub enum AtomicOrdering {
    Relaxed,
    Acquire,
    Release,
    AcqRel,
    SeqCst,
}

#[derive(Debug, Clone, Copy)]
pub enum AtomicRmwOp {
    Add,
    Sub,
    And,
    Xor,
    Or,
    Nand,
    Max,
    Min,
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
    LetIf {
        name: String,
        mutable: bool,
        ty: Option<Type>,
        cond: Expr,
        then_body: Vec<IndentStmt>,
        then_value: Expr,
        else_body: Vec<IndentStmt>,
        else_value: Expr,
    },
    Assign {
        target: Expr,
        value: Expr,
    },
    Expr(Expr),
    Return(Option<Expr>),
    Unsafe {
        body: Vec<IndentStmt>,
    },
    If {
        cond: Expr,
        then_body: Vec<IndentStmt>,
        else_body: Vec<IndentStmt>,
    },
    Loop {
        label: Option<String>,
        body: Vec<IndentStmt>,
    },
    Scope {
        body: Vec<IndentStmt>,
    },
    LabeledBlock {
        label: String,
        body: Vec<IndentStmt>,
    },
    Match {
        expr: Expr,
        arms: Vec<MatchArm>,
    },
    Break(Option<String>),
    Continue(Option<String>),
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
    Index {
        base: Box<Expr>,
        index: Box<Expr>,
    },
    StructLit {
        name: String,
        fields: Vec<(String, Expr)>,
    },
    ArrayLit(Vec<Expr>),
    ArrayRepeat {
        elem: Box<Expr>,
        len: usize,
    },
    /// A macro invocation, e.g. `println!(...)`. `name` excludes the `!`.
    Macro {
        name: String,
        args: Vec<Expr>,
    },
    Match {
        expr: Box<Expr>,
        arms: Vec<ExprMatchArm>,
    },
    If {
        cond: Box<Expr>,
        then_expr: Box<Expr>,
        else_expr: Box<Expr>,
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
    AtomicRef {
        ty: AtomicType,
        ptr: Box<Expr>,
    },
    AtomicLoad {
        ty: AtomicType,
        ptr: Box<Expr>,
        ordering: AtomicOrdering,
    },
    AtomicStore {
        ty: AtomicType,
        ptr: Box<Expr>,
        value: Box<Expr>,
        ordering: AtomicOrdering,
    },
    AtomicFetch {
        ty: AtomicType,
        op: AtomicRmwOp,
        ptr: Box<Expr>,
        value: Box<Expr>,
        ordering: AtomicOrdering,
    },
    AtomicSwap {
        ty: AtomicType,
        ptr: Box<Expr>,
        value: Box<Expr>,
        ordering: AtomicOrdering,
    },
    AtomicCompareExchange {
        ty: AtomicType,
        ptr: Box<Expr>,
        expected: Box<Expr>,
        desired: Box<Expr>,
        success: AtomicOrdering,
        failure: AtomicOrdering,
    },
    AtomicFence {
        ordering: AtomicOrdering,
    },
    /// Fully-formed Rust text spliced in as-is (e.g. `libc::printf`).
    Raw(String),
}

#[derive(Debug, Clone)]
pub enum Type {
    Prim(Prim),
    /// Records, `LongDouble`, `Complex<f64>`, `libc::FILE`, and other opaque spellings.
    Named(String),
    Ptr {
        mutable: bool,
        inner: Box<Type>,
    },
    Array {
        elem: Box<Type>,
        len: u64,
    },
    FnPtr {
        params: Vec<Type>,
        ret: Box<Type>,
    },
    Unit,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Prim {
    Bool,
    I8,
    I16,
    I32,
    I64,
    I128,
    Isize,
    U8,
    U16,
    U32,
    U64,
    U128,
    Usize,
    F32,
    F64,
}

impl Prim {
    pub fn spelling(self) -> &'static str {
        match self {
            Prim::Bool => "bool",
            Prim::I8 => "i8",
            Prim::I16 => "i16",
            Prim::I32 => "i32",
            Prim::I64 => "i64",
            Prim::I128 => "i128",
            Prim::Isize => "isize",
            Prim::U8 => "u8",
            Prim::U16 => "u16",
            Prim::U32 => "u32",
            Prim::U64 => "u64",
            Prim::U128 => "u128",
            Prim::Usize => "usize",
            Prim::F32 => "f32",
            Prim::F64 => "f64",
        }
    }

    pub fn parse(s: &str) -> Option<Prim> {
        Some(match s {
            "bool" => Prim::Bool,
            "i8" => Prim::I8,
            "i16" => Prim::I16,
            "i32" => Prim::I32,
            "i64" => Prim::I64,
            "i128" => Prim::I128,
            "isize" => Prim::Isize,
            "u8" => Prim::U8,
            "u16" => Prim::U16,
            "u32" => Prim::U32,
            "u64" => Prim::U64,
            "u128" => Prim::U128,
            "usize" => Prim::Usize,
            "f32" => Prim::F32,
            "f64" => Prim::F64,
            _ => return None,
        })
    }
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
                if let Some(vis) = &f.vis {
                    out.push_str(vis);
                    out.push(' ');
                }
                if f.unsafe_extern_c {
                    out.push_str("unsafe extern \"C\" ");
                }
                let _ = write!(out, "fn {}(", f.name);
                for (i, p) in f.params.iter().enumerate() {
                    if i > 0 {
                        out.push_str(", ");
                    }
                    if p.mutable {
                        out.push_str("mut ");
                    }
                    let _ = write!(out, "{}: {}", p.name, p.ty.render());
                }
                out.push(')');
                if let Some(ret) = &f.ret {
                    let _ = write!(out, " -> {}", ret.render());
                }
                out.push_str(" {");
                out.push('\n');
                for IndentStmt { depth, stmt } in &f.body {
                    stmt.emit(out, *depth);
                }
                out.push_str("}\n");
            }
            Item::CrateAttrs(attrs) => {
                for attr in attrs {
                    let _ = writeln!(out, "#![{attr}]");
                }
            }
            Item::Mod { name } => {
                let _ = writeln!(out, "mod {name};");
            }
            Item::Use { path } => {
                let _ = writeln!(out, "use {path};");
            }
            Item::Static {
                vis,
                mutable,
                name,
                ty,
                init,
            } => {
                if let Some(vis) = vis {
                    out.push_str(vis);
                    out.push(' ');
                }
                out.push_str("static ");
                if *mutable {
                    out.push_str("mut ");
                }
                let _ = writeln!(out, "{name}: {} = {};", ty.render(), init.render());
            }
            Item::ExternBlock { abi, decls } => {
                let _ = writeln!(out, "unsafe extern \"{abi}\" {{");
                for decl in decls {
                    out.push_str(INDENT);
                    decl.emit(out);
                }
                out.push_str("}\n");
            }
            Item::Enum(consts) => {
                for c in consts {
                    let _ = writeln!(out, "const {}: i32 = {};", c.name, c.value);
                }
                out.push('\n');
            }
            Item::Record(r) => {
                out.push_str("#[repr(C)]\n");
                if r.allow_non_camel_case {
                    out.push_str("#[allow(non_camel_case_types)]\n");
                }
                out.push_str("#[derive(Clone, Copy)]\n");
                let kw = if r.is_union { "union" } else { "struct" };
                let _ = writeln!(out, "{kw} {} {{", r.name);
                for (name, ty) in &r.fields {
                    let _ = writeln!(out, "    {name}: {},", ty.render());
                }
                out.push_str("}\n");
                out.push('\n');
            }
            Item::Raw(s) => {
                out.push_str(s);
                out.push('\n');
            }
        }
    }
}

impl ExternDecl {
    fn emit(&self, out: &mut String) {
        match self {
            ExternDecl::Fn(f) => {
                let _ = write!(out, "fn {}(", f.name);
                for (i, p) in f.params.iter().enumerate() {
                    if i > 0 {
                        out.push_str(", ");
                    }
                    let _ = write!(out, "{}: {}", p.name, p.ty.render());
                }
                if f.variadic {
                    if !f.params.is_empty() {
                        out.push_str(", ");
                    }
                    out.push_str("...");
                }
                out.push(')');
                if let Some(ret) = &f.ret {
                    let _ = write!(out, " -> {}", ret.render());
                }
                out.push_str(";\n");
            }
            ExternDecl::Static { mutable, name, ty } => {
                out.push_str("static ");
                if *mutable {
                    out.push_str("mut ");
                }
                let _ = writeln!(out, "{name}: {};", ty.render());
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
            Stmt::LetIf {
                name,
                mutable,
                ty,
                cond,
                then_body,
                then_value,
                else_body,
                else_value,
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
                let _ = writeln!(out, " = if {} {{", cond.render_spliceable());
                emit_indent_stmts(out, then_body, depth + 1);
                let _ = writeln!(out, "{}{}{}", pad, INDENT, then_value.render());
                let _ = writeln!(out, "{pad}}} else {{");
                emit_indent_stmts(out, else_body, depth + 1);
                let _ = writeln!(out, "{}{}{}", pad, INDENT, else_value.render());
                let _ = writeln!(out, "{pad}}};");
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
            Stmt::Unsafe { body } => {
                let _ = writeln!(out, "{pad}unsafe {{");
                emit_indent_stmts(out, body, depth + 1);
                let _ = writeln!(out, "{pad}}}");
            }
            Stmt::If {
                cond,
                then_body,
                else_body,
            } => {
                let _ = writeln!(out, "{pad}if {} {{", cond.render_spliceable());
                emit_indent_stmts(out, then_body, depth + 1);
                if else_body.is_empty() {
                    let _ = writeln!(out, "{pad}}}");
                } else {
                    let _ = writeln!(out, "{pad}}} else {{");
                    emit_indent_stmts(out, else_body, depth + 1);
                    let _ = writeln!(out, "{pad}}}");
                }
            }
            Stmt::Loop { label, body } => {
                match label {
                    Some(label) => {
                        let _ = writeln!(out, "{pad}{label}: loop {{");
                    }
                    None => {
                        let _ = writeln!(out, "{pad}loop {{");
                    }
                }
                emit_indent_stmts(out, body, depth + 1);
                let _ = writeln!(out, "{pad}}}");
            }
            Stmt::Scope { body } => {
                let _ = writeln!(out, "{pad}{{");
                emit_indent_stmts(out, body, depth + 1);
                let _ = writeln!(out, "{pad}}}");
            }
            Stmt::LabeledBlock { label, body } => {
                let _ = writeln!(out, "{pad}{label}: {{");
                emit_indent_stmts(out, body, depth + 1);
                let _ = writeln!(out, "{pad}}}");
            }
            Stmt::Match { expr, arms } => {
                let _ = writeln!(out, "{pad}match {} {{", expr.render_spliceable());
                for arm in arms {
                    let _ = writeln!(out, "{pad}{INDENT}{} => {{", arm.pattern);
                    emit_indent_stmts(out, &arm.body, depth + 2);
                    let _ = writeln!(out, "{pad}{INDENT}}}");
                }
                let _ = writeln!(out, "{pad}}}");
            }
            Stmt::Break(label) => match label {
                Some(label) => {
                    let _ = writeln!(out, "{pad}break {label};");
                }
                None => {
                    let _ = writeln!(out, "{pad}break;");
                }
            },
            Stmt::Continue(label) => match label {
                Some(label) => {
                    let _ = writeln!(out, "{pad}continue {label};");
                }
                None => {
                    let _ = writeln!(out, "{pad}continue;");
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

fn emit_indent_stmts(out: &mut String, body: &[IndentStmt], depth: usize) {
    for IndentStmt { depth: extra, stmt } in body {
        stmt.emit(out, depth + extra);
    }
}

// Rust expression binding powers (higher binds tighter). Only the operators the
// lowerer emits are modeled; anything atomic or brace/paren-delimited renders at
// PREC_ATOM so it never needs wrapping.
const PREC_CAST: u8 = 4;
const PREC_CAST_OPERAND: u8 = 12;
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
            Expr::AtomicFence { .. } => false,
            Expr::AtomicRef { ptr, .. } | Expr::AtomicLoad { ptr, .. } => {
                ptr.substitute_var(name, replacement)
            }
            Expr::AtomicStore { ptr, value, .. }
            | Expr::AtomicFetch { ptr, value, .. }
            | Expr::AtomicSwap { ptr, value, .. } => {
                let p = ptr.substitute_var(name, replacement);
                let v = value.substitute_var(name, replacement);
                p || v
            }
            Expr::AtomicCompareExchange {
                ptr,
                expected,
                desired,
                ..
            } => {
                let p = ptr.substitute_var(name, replacement);
                let e = expected.substitute_var(name, replacement);
                let d = desired.substitute_var(name, replacement);
                p || e || d
            }
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
            Expr::Index { base, index } => {
                let b = base.substitute_var(name, replacement);
                let i = index.substitute_var(name, replacement);
                b || i
            }
            Expr::StructLit { fields, .. } => {
                let mut changed = false;
                for (_, value) in fields {
                    changed |= value.substitute_var(name, replacement);
                }
                changed
            }
            Expr::ArrayLit(elems) => {
                let mut changed = false;
                for elem in elems {
                    changed |= elem.substitute_var(name, replacement);
                }
                changed
            }
            Expr::ArrayRepeat { elem, .. } => elem.substitute_var(name, replacement),
            Expr::Macro { args, .. } => {
                let mut changed = false;
                for arg in args {
                    changed |= arg.substitute_var(name, replacement);
                }
                changed
            }
            Expr::Match { expr, arms } => {
                let mut changed = expr.substitute_var(name, replacement);
                for arm in arms {
                    changed |= arm.value.substitute_var(name, replacement);
                }
                changed
            }
            Expr::If {
                cond,
                then_expr,
                else_expr,
            } => {
                let c = cond.substitute_var(name, replacement);
                let t = then_expr.substitute_var(name, replacement);
                let e = else_expr.substitute_var(name, replacement);
                c || t || e
            }
        }
    }

    fn prec(&self) -> u8 {
        match self {
            Expr::Binary { op, .. } => binop_prec(op),
            Expr::Cast { .. } => PREC_CAST,
            Expr::Unary { .. } | Expr::Ref { .. } => PREC_PREFIX,
            Expr::Call { .. }
            | Expr::MethodCall { .. }
            | Expr::Field { .. }
            | Expr::Index { .. } => PREC_CALL,
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
            Expr::Index { base, index } => {
                format!(
                    "{}[{}]",
                    base.render_prec(PREC_CALL),
                    index.render_spliceable()
                )
            }
            Expr::StructLit { name, fields } => {
                let fields = fields
                    .iter()
                    .map(|(name, value)| format!("{name}: {}", value.render()))
                    .collect::<Vec<_>>()
                    .join(", ");
                format!("{name} {{ {fields} }}")
            }
            Expr::ArrayLit(elems) => {
                format!("[{}]", render_args(elems))
            }
            Expr::ArrayRepeat { elem, len } => {
                format!("[{}; {len}]", elem.render())
            }
            Expr::Macro { name, args } => {
                format!("{name}!({})", render_args(args))
            }
            Expr::Match { expr, arms } => {
                let arms = arms
                    .iter()
                    .map(|arm| format!("{} => {}", arm.pattern, arm.value.render()))
                    .collect::<Vec<_>>()
                    .join(", ");
                format!("match {} {{ {arms} }}", expr.render_spliceable())
            }
            Expr::If {
                cond,
                then_expr,
                else_expr,
            } => {
                format!(
                    "if {} {{ {} }} else {{ {} }}",
                    cond.render_spliceable(),
                    then_expr.render(),
                    else_expr.render()
                )
            }
            Expr::Unsafe(e) => format!("unsafe {{ {} }}", e.render()),
            Expr::Cast { expr, ty } => {
                format!("{} as {}", expr.render_prec(PREC_CAST_OPERAND), ty.render())
            }
            Expr::Ref { mutable, expr } => {
                let kw = if *mutable { "&mut " } else { "&" };
                format!("{kw}{}", render_prefix_operand(expr))
            }
            Expr::AtomicRef { ty, ptr } => {
                format!(
                    "std::sync::atomic::{}::from_ptr({})",
                    ty.render_wrapper(),
                    ptr.render()
                )
            }
            Expr::AtomicLoad { ty, ptr, ordering } => {
                format!(
                    "unsafe {{ {}.load({}) }}",
                    render_atomic_ref(*ty, ptr),
                    ordering.render()
                )
            }
            Expr::AtomicStore {
                ty,
                ptr,
                value,
                ordering,
            } => {
                format!(
                    "unsafe {{ {}.store({}, {}) }}",
                    render_atomic_ref(*ty, ptr),
                    value.render(),
                    ordering.render()
                )
            }
            Expr::AtomicFetch {
                ty,
                op,
                ptr,
                value,
                ordering,
            } => {
                format!(
                    "unsafe {{ {}.{}({}, {}) }}",
                    render_atomic_ref(*ty, ptr),
                    op.render_method(),
                    value.render(),
                    ordering.render()
                )
            }
            Expr::AtomicSwap {
                ty,
                ptr,
                value,
                ordering,
            } => {
                format!(
                    "unsafe {{ {}.swap({}, {}) }}",
                    render_atomic_ref(*ty, ptr),
                    value.render(),
                    ordering.render()
                )
            }
            Expr::AtomicCompareExchange {
                ty,
                ptr,
                expected,
                desired,
                success,
                failure,
            } => {
                format!(
                    "unsafe {{ {}.compare_exchange({}, {}, {}, {}) }}",
                    render_atomic_ref(*ty, ptr),
                    expected.render(),
                    desired.render(),
                    success.render(),
                    failure.render()
                )
            }
            Expr::AtomicFence { ordering } => {
                format!("std::sync::atomic::fence({})", ordering.render())
            }
        }
    }
}

fn render_atomic_ref(ty: AtomicType, ptr: &Expr) -> String {
    Expr::AtomicRef {
        ty,
        ptr: Box::new(ptr.clone()),
    }
    .render()
}

impl AtomicType {
    fn render_wrapper(self) -> &'static str {
        match self {
            AtomicType::I8 => "AtomicI8",
            AtomicType::U8 => "AtomicU8",
            AtomicType::I16 => "AtomicI16",
            AtomicType::U16 => "AtomicU16",
            AtomicType::I32 => "AtomicI32",
            AtomicType::U32 => "AtomicU32",
            AtomicType::I64 => "AtomicI64",
            AtomicType::U64 => "AtomicU64",
            AtomicType::Isize => "AtomicIsize",
            AtomicType::Usize => "AtomicUsize",
            AtomicType::Bool => "AtomicBool",
        }
    }
}

impl AtomicOrdering {
    fn render(self) -> &'static str {
        match self {
            AtomicOrdering::Relaxed => "std::sync::atomic::Ordering::Relaxed",
            AtomicOrdering::Acquire => "std::sync::atomic::Ordering::Acquire",
            AtomicOrdering::Release => "std::sync::atomic::Ordering::Release",
            AtomicOrdering::AcqRel => "std::sync::atomic::Ordering::AcqRel",
            AtomicOrdering::SeqCst => "std::sync::atomic::Ordering::SeqCst",
        }
    }
}

impl AtomicRmwOp {
    fn render_method(self) -> &'static str {
        match self {
            AtomicRmwOp::Add => "fetch_add",
            AtomicRmwOp::Sub => "fetch_sub",
            AtomicRmwOp::And => "fetch_and",
            AtomicRmwOp::Xor => "fetch_xor",
            AtomicRmwOp::Or => "fetch_or",
            AtomicRmwOp::Nand => "fetch_nand",
            AtomicRmwOp::Max => "fetch_max",
            AtomicRmwOp::Min => "fetch_min",
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
            Type::Prim(p) => p.spelling().to_string(),
            Type::Named(n) => n.clone(),
            Type::Ptr { mutable, inner } => {
                let kw = if *mutable { "*mut " } else { "*const " };
                format!("{kw}{}", inner.render())
            }
            Type::Array { elem, len } => format!("[{}; {len}]", elem.render()),
            Type::FnPtr { params, ret } => {
                let params = params
                    .iter()
                    .map(Type::render)
                    .collect::<Vec<_>>()
                    .join(", ");
                format!("Option<fn({params}) -> {}>", ret.render())
            }
            Type::Unit => "()".to_string(),
        }
    }

    pub fn parse(s: &str) -> Type {
        let s = s.trim();
        if let Some(rest) = s.strip_prefix("*mut ") {
            return Type::Ptr {
                mutable: true,
                inner: Box::new(Type::parse(rest)),
            };
        }
        if let Some(rest) = s.strip_prefix("*const ") {
            return Type::Ptr {
                mutable: false,
                inner: Box::new(Type::parse(rest)),
            };
        }
        if s == "()" {
            return Type::Unit;
        }
        match Prim::parse(s) {
            Some(p) => Type::Prim(p),
            None => Type::Named(s.to_string()),
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
        assert_eq!(
            Expr::Binary {
                op: "<".into(),
                lhs: Box::new(Expr::Cast {
                    expr: var("a"),
                    ty: Type::Named("i32".into()),
                }),
                rhs: var("b"),
            }
            .render(),
            "(a as i32) < b"
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

    #[test]
    fn emits_unsafe_statement_body() {
        let prog = Program {
            items: vec![Item::Fn(FnDef {
                vis: None,
                unsafe_extern_c: false,
                name: "f".into(),
                params: vec![],
                ret: None,
                body: vec![IndentStmt {
                    depth: 1,
                    stmt: Stmt::Unsafe {
                        body: vec![IndentStmt {
                            depth: 0,
                            stmt: Stmt::Assign {
                                target: Expr::Raw("*p".into()),
                                value: Expr::Var("x".into()),
                            },
                        }],
                    },
                }],
            })],
        };

        assert_eq!(
            prog.emit(),
            "\
fn f() {
    unsafe {
        *p = x;
    }
}
"
        );
    }

    #[test]
    fn renders_atomic_expression_nodes() {
        let ptr = || {
            Box::new(Expr::Macro {
                name: "std::ptr::addr_of_mut".into(),
                args: vec![Expr::Var("a".into())],
            })
        };

        assert_eq!(
            Expr::AtomicFetch {
                ty: AtomicType::I32,
                op: AtomicRmwOp::Add,
                ptr: ptr(),
                value: Box::new(Expr::Var("x".into())),
                ordering: AtomicOrdering::SeqCst,
            }
            .render(),
            "unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a)).fetch_add(x, std::sync::atomic::Ordering::SeqCst) }"
        );
        assert_eq!(
            Expr::AtomicFence {
                ordering: AtomicOrdering::Acquire,
            }
            .render(),
            "std::sync::atomic::fence(std::sync::atomic::Ordering::Acquire)"
        );
    }
}
