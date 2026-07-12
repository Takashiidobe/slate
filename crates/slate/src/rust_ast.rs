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
    Struct(StructDef),
    Impl(ImplBlock),
    /// Escape hatch for things without a modeled node yet (runtime preludes).
    Raw(String),
}

/// A struct definition richer than [`RecordDef`]: attributes, generics, and a
/// tuple-or-named body, none of which `RecordDef` can express.
#[derive(Debug, Clone)]
pub struct StructDef {
    pub attrs: Vec<Attr>,
    pub generics: Vec<GenericParam>,
    pub name: String,
    pub fields: StructFields,
}

#[derive(Debug, Clone)]
pub enum StructFields {
    Tuple(Vec<Type>),
    Named(Vec<(String, Type)>),
}

#[derive(Debug, Clone)]
pub enum Attr {
    Repr(Vec<Repr>),
    Derive(Vec<Derive>),
}

#[derive(Debug, Clone, Copy)]
pub enum Repr {
    C,
    Align(u32),
}

#[derive(Debug, Clone, Copy)]
pub enum Derive {
    Clone,
    Copy,
}

#[derive(Debug, Clone)]
pub struct GenericParam {
    pub name: String,
    pub bounds: Vec<TraitBound>,
}

#[derive(Debug, Clone)]
pub struct TraitBound {
    pub trait_: StdTrait,
    pub assoc: Vec<(String, Type)>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum StdTrait {
    Add,
    Sub,
    Mul,
    Div,
    Neg,
}

impl StdTrait {
    pub fn path(self) -> &'static str {
        match self {
            StdTrait::Add => "core::ops::Add",
            StdTrait::Sub => "core::ops::Sub",
            StdTrait::Mul => "core::ops::Mul",
            StdTrait::Div => "core::ops::Div",
            StdTrait::Neg => "core::ops::Neg",
        }
    }

    pub fn method(self) -> &'static str {
        match self {
            StdTrait::Add => "add",
            StdTrait::Sub => "sub",
            StdTrait::Mul => "mul",
            StdTrait::Div => "div",
            StdTrait::Neg => "neg",
        }
    }
}

#[derive(Debug, Clone)]
pub struct ImplBlock {
    pub generics: Vec<GenericParam>,
    /// The trait being implemented; `None` for an inherent impl.
    pub trait_: Option<StdTrait>,
    pub self_ty: Type,
    pub items: Vec<ImplItem>,
}

#[derive(Debug, Clone)]
pub enum ImplItem {
    AssocType { name: String, ty: Type },
    Method(Method),
}

#[derive(Debug, Clone)]
pub struct Method {
    pub name: String,
    pub takes_self: bool,
    pub params: Vec<FnParam>,
    pub ret: Option<Type>,
    pub body: Expr,
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
    pub fields: Vec<(Ident, Type)>,
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
    pub pattern: Pattern,
    pub body: Vec<IndentStmt>,
}

#[derive(Debug, Clone)]
pub struct ExprMatchArm {
    pub pattern: Pattern,
    pub value: Expr,
}

#[derive(Debug, Clone)]
pub enum Pattern {
    Wildcard,
    Binding(Ident),
    I64(i64),
    I128(i128),
    TupleStruct { name: Ident, fields: Vec<Pattern> },
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
    pub stmts: Vec<IndentStmt>,
    pub tail: Option<Box<Expr>>,
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
        body: Block,
    },
    If {
        cond: Expr,
        then_body: Vec<IndentStmt>,
        else_body: Vec<IndentStmt>,
    },
    Loop {
        label: Option<Label>,
        body: Vec<IndentStmt>,
    },
    Scope {
        body: Vec<IndentStmt>,
    },
    LabeledBlock {
        label: Label,
        body: Vec<IndentStmt>,
    },
    Match {
        expr: Expr,
        arms: Vec<MatchArm>,
    },
    Break(Option<Label>),
    Continue(Option<Label>),
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
    I64(i64),
    I128(i128),
    Float(f64),
    Bool(bool),
    None,
    NullPtr,
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct Ident(String);

impl Ident {
    pub fn new(s: impl Into<String>) -> Self {
        Self(s.into())
    }

    pub fn as_str(&self) -> &str {
        &self.0
    }

    pub fn into_string(self) -> String {
        self.0
    }
}

impl From<String> for Ident {
    fn from(s: String) -> Self {
        Self::new(s)
    }
}

impl From<&str> for Ident {
    fn from(s: &str) -> Self {
        Self::new(s)
    }
}

impl std::fmt::Display for Ident {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.write_str(self.as_str())
    }
}

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct Label(Ident);

impl Label {
    pub fn new(name: impl Into<Ident>) -> Self {
        Self(name.into())
    }

    pub fn as_str(&self) -> &str {
        self.0.as_str()
    }
}

impl From<String> for Label {
    fn from(s: String) -> Self {
        Self::new(s)
    }
}

impl From<&str> for Label {
    fn from(s: &str) -> Self {
        Self::new(s)
    }
}

#[derive(Debug, Clone)]
pub enum Expr {
    /// A structured literal value (integer, null pointer).
    Value(RustValue),
    /// A literal or identifier printed verbatim (numbers, `true`, names).
    Lit(String),
    Var(Ident),
    Unary {
        op: UnaryOp,
        expr: Box<Expr>,
    },
    Binary {
        op: BinOp,
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
    MethodCallGeneric {
        recv: Box<Expr>,
        method: String,
        type_args: Vec<Type>,
        args: Vec<Expr>,
    },
    /// A field or tuple-index access, e.g. `pair.0` or `s.len`.
    Field {
        base: Box<Expr>,
        field: String,
    },
    TupleField {
        base: Box<Expr>,
        index: usize,
    },
    ArrayPtr {
        array: Box<Expr>,
        mutable: bool,
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
    Block(Box<Block>),
    Unsafe(Box<Block>),
    Cast {
        expr: Box<Expr>,
        ty: Type,
    },
    Ref {
        mutable: bool,
        expr: Box<Expr>,
    },
    AddrOf {
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
    Transmute {
        from: Type,
        to: Type,
        expr: Box<Expr>,
    },
    CopyNonoverlapping {
        src: Box<Expr>,
        dst: Box<Expr>,
        count: usize,
    },
    Todo(String),
    /// Fully-formed Rust text spliced in as-is (e.g. `libc::printf`).
    Raw(String),
}

#[derive(Debug, Clone)]
pub enum Type {
    Prim(Prim),
    /// Records, `LongDouble`, and other opaque spellings with no native Rust type.
    Named(String),
    /// A generic type parameter reference, e.g. the `T` in `Complex<T>`.
    TyVar(Ident),
    /// A C library type with no native Rust equivalent, spelled by its FFI path.
    CLib(CLibType),
    Complex(Box<Type>),
    Generic {
        name: String,
        args: Vec<Type>,
    },
    VaList,
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
    Variadic,
}

/// C library types with no native Rust equivalent, referenced through their FFI path.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum CLibType {
    Void,
    File,
}

impl CLibType {
    pub fn path(self) -> &'static str {
        match self {
            CLibType::Void => "core::ffi::c_void",
            CLibType::File => "libc::FILE",
        }
    }
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

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum BinOp {
    Add,
    Sub,
    Mul,
    Div,
    Rem,
    BitAnd,
    BitOr,
    BitXor,
    Shl,
    Shr,
    Eq,
    Ne,
    Lt,
    Le,
    Gt,
    Ge,
    And,
    Or,
}

impl BinOp {
    pub fn spelling(self) -> &'static str {
        match self {
            BinOp::Add => "+",
            BinOp::Sub => "-",
            BinOp::Mul => "*",
            BinOp::Div => "/",
            BinOp::Rem => "%",
            BinOp::BitAnd => "&",
            BinOp::BitOr => "|",
            BinOp::BitXor => "^",
            BinOp::Shl => "<<",
            BinOp::Shr => ">>",
            BinOp::Eq => "==",
            BinOp::Ne => "!=",
            BinOp::Lt => "<",
            BinOp::Le => "<=",
            BinOp::Gt => ">",
            BinOp::Ge => ">=",
            BinOp::And => "&&",
            BinOp::Or => "||",
        }
    }

    // higher binds tighter; mirrors Rust's operator precedence.
    pub fn precedence(self) -> u8 {
        match self {
            BinOp::Or => 3,
            BinOp::And => 4,
            BinOp::Eq | BinOp::Ne | BinOp::Lt | BinOp::Gt | BinOp::Le | BinOp::Ge => 5,
            BinOp::BitOr => 6,
            BinOp::BitXor => 7,
            BinOp::BitAnd => 8,
            BinOp::Shl | BinOp::Shr => 9,
            BinOp::Add | BinOp::Sub => 10,
            BinOp::Mul | BinOp::Div | BinOp::Rem => 11,
        }
    }

    pub fn is_comparison(self) -> bool {
        self.precedence() == 5
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum UnaryOp {
    Neg,
    Not,
    Deref,
}

impl UnaryOp {
    pub fn spelling(self) -> &'static str {
        match self {
            UnaryOp::Neg => "-",
            UnaryOp::Not => "!",
            UnaryOp::Deref => "*",
        }
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
            Expr::Var(v) if v.as_str() == name => {
                *self = replacement.clone();
                true
            }
            Expr::Value(_) | Expr::Lit(_) | Expr::Var(_) | Expr::Raw(_) => false,
            Expr::Unary { expr, .. }
            | Expr::Cast { expr, .. }
            | Expr::Ref { expr, .. }
            | Expr::AddrOf { expr, .. }
            | Expr::Transmute { expr, .. } => expr.substitute_var(name, replacement),
            Expr::Block(block) | Expr::Unsafe(block) => {
                let mut changed = false;
                for stmt in &mut block.stmts {
                    changed |= stmt_substitute_var(&mut stmt.stmt, name, replacement);
                }
                if let Some(tail) = &mut block.tail {
                    changed |= tail.substitute_var(name, replacement);
                }
                changed
            }
            Expr::CopyNonoverlapping { src, dst, .. } => {
                let s = src.substitute_var(name, replacement);
                let d = dst.substitute_var(name, replacement);
                s || d
            }
            Expr::AtomicFence { .. } | Expr::Todo(_) => false,
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
            Expr::MethodCallGeneric { recv, args, .. } => {
                let mut changed = recv.substitute_var(name, replacement);
                for arg in args {
                    changed |= arg.substitute_var(name, replacement);
                }
                changed
            }
            Expr::Field { base, .. } | Expr::TupleField { base, .. } => {
                base.substitute_var(name, replacement)
            }
            Expr::ArrayPtr { array, .. } => array.substitute_var(name, replacement),
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

fn stmt_substitute_var(stmt: &mut Stmt, name: &str, replacement: &Expr) -> bool {
    match stmt {
        Stmt::Let {
            init: Some(expr), ..
        } => expr.substitute_var(name, replacement),
        Stmt::Let { init: None, .. } => false,
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            let mut changed = cond.substitute_var(name, replacement);
            for stmt in then_body.iter_mut().chain(else_body.iter_mut()) {
                changed |= stmt_substitute_var(&mut stmt.stmt, name, replacement);
            }
            changed |= then_value.substitute_var(name, replacement);
            changed |= else_value.substitute_var(name, replacement);
            changed
        }
        Stmt::Assign { target, value } => {
            let t = target.substitute_var(name, replacement);
            let v = value.substitute_var(name, replacement);
            t || v
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => expr.substitute_var(name, replacement),
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) | Stmt::Raw(_) => false,
        Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            let mut changed = false;
            for stmt in body {
                changed |= stmt_substitute_var(&mut stmt.stmt, name, replacement);
            }
            changed
        }
        Stmt::Unsafe { body } => {
            let mut changed = false;
            for stmt in &mut body.stmts {
                changed |= stmt_substitute_var(&mut stmt.stmt, name, replacement);
            }
            if let Some(tail) = &mut body.tail {
                changed |= tail.substitute_var(name, replacement);
            }
            changed
        }
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            let mut changed = cond.substitute_var(name, replacement);
            for stmt in then_body.iter_mut().chain(else_body.iter_mut()) {
                changed |= stmt_substitute_var(&mut stmt.stmt, name, replacement);
            }
            changed
        }
        Stmt::Loop { body, .. } => {
            let mut changed = false;
            for stmt in body {
                changed |= stmt_substitute_var(&mut stmt.stmt, name, replacement);
            }
            changed
        }
        Stmt::Match { expr, arms } => {
            let mut changed = expr.substitute_var(name, replacement);
            for arm in arms {
                for stmt in &mut arm.body {
                    changed |= stmt_substitute_var(&mut stmt.stmt, name, replacement);
                }
            }
            changed
        }
        Stmt::While { cond, body } => {
            let mut changed = cond.substitute_var(name, replacement);
            for stmt in &mut body.stmts {
                changed |= stmt_substitute_var(&mut stmt.stmt, name, replacement);
            }
            if let Some(tail) = &mut body.tail {
                changed |= tail.substitute_var(name, replacement);
            }
            changed
        }
        Stmt::Block(body) => {
            let mut changed = false;
            for stmt in &mut body.stmts {
                changed |= stmt_substitute_var(&mut stmt.stmt, name, replacement);
            }
            if let Some(tail) = &mut body.tail {
                changed |= tail.substitute_var(name, replacement);
            }
            changed
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
        if s == "core::ffi::VaList<'_>" {
            return Type::VaList;
        }
        match Prim::parse(s) {
            Some(p) => Type::Prim(p),
            None => Type::Named(s.to_string()),
        }
    }
}
