//! The Rust AST and its textual emitter — the pipeline's output side.
//!
//! Lowering builds this tree; [`Program::emit`] prints it. The emitter never
//! concatenates source ad hoc: every node knows how to print itself, indentation
//! is structural, and binary expressions are fully parenthesized so precedence is
//! never in question. V0 output is meant to be correct, not pretty; the
//! idiomatization passes (see docs/idiomatization.md) clean it up later.
//!
//! The textual emitter lives in [`crate::codegen`]; this module is the data and
//! the AST-manipulation methods (parsing spellings, variable substitution).

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

/// A structured Rust literal value. Kept as typed data rather than pre-rendered
/// text so literals carry real shape through the AST (`RustValue::NullPtr`
/// instead of the string `"std::ptr::null_mut()"`), mirroring how atomics are
/// modeled. Use this over `Expr::Lit` whenever the value is known structurally.
#[derive(Debug, Clone)]
pub enum RustValue {
    Int(i64),
    NullPtr,
}

#[derive(Debug, Clone)]
pub enum Expr {
    /// A structured literal value (integer, null pointer).
    Value(RustValue),
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
// Emitter entry points — the printing logic lives in `crate::codegen`.
// ---------------------------------------------------------------------------

impl Program {
    pub fn emit(&self) -> String {
        crate::codegen::program_to_string(self)
    }
}

impl Stmt {
    // The single-line, un-indented text form, as fixups parse it. Only meaningful
    // for the flat statement forms the migrated body holds (Let/Assign/Expr/Return/Raw).
    pub fn render_line(&self) -> String {
        crate::codegen::stmt_line_to_string(self)
    }
}

impl Expr {
    pub fn render(&self) -> String {
        crate::codegen::expr_to_string(self)
    }

    // Render for splicing into arbitrary surrounding text (a `Stmt::Raw` line),
    // where the enclosing precedence is unknown. Anything that binds looser than a
    // call is wrapped so the splice can never change precedence.
    pub fn render_spliceable(&self) -> String {
        crate::codegen::expr_spliceable_to_string(self)
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
            Expr::Value(_) | Expr::Lit(_) | Expr::Var(_) | Expr::Raw(_) => false,
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
}

impl Type {
    pub fn render(&self) -> String {
        crate::codegen::type_to_string(self)
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
