//! Textual code generation for the Rust AST — the pipeline's output side.
//!
//! Lowering builds [`crate::rust_ast`] nodes; [`Codegen`] prints them. The
//! emitter never concatenates source ad hoc: every node knows how to print
//! itself, indentation is structural, and binary expressions are parenthesized
//! only where precedence demands it. V0 output is meant to be correct, not
//! pretty; the idiomatization passes (see docs/idiomatization.md) clean it up.
//!
//! Emission is written against [`std::fmt::Write`], so a caller can stream into
//! any sink — a `String`, a formatter, or a test buffer — without forcing a
//! heap allocation per node.

use std::fmt::{self, Write};

use crate::rust_ast::{
    AtomicOrdering, AtomicRmwOp, AtomicType, Block, Expr, ExternDecl, FnDef, Func, IndentStmt,
    Item, Program, RecordDef, RustValue, Stmt, Type,
};

const INDENT: &str = "    ";

// Rust expression binding powers (higher binds tighter). Only the operators the
// lowerer emits are modeled; anything atomic or brace/paren-delimited renders at
// PREC_ATOM so it never needs wrapping.
const PREC_CAST: u8 = 4;
const PREC_CAST_OPERAND: u8 = 12;
const PREC_PREFIX: u8 = 13;
const PREC_CALL: u8 = 14;
const PREC_ATOM: u8 = 15;

fn expr_prec(expr: &Expr) -> u8 {
    match expr {
        Expr::Binary { op, .. } => op.precedence(),
        Expr::Cast { .. } => PREC_CAST,
        Expr::Unary { .. } | Expr::Ref { .. } => PREC_PREFIX,
        Expr::Call { .. } | Expr::MethodCall { .. } | Expr::Field { .. } | Expr::Index { .. } => {
            PREC_CALL
        }
        _ => PREC_ATOM,
    }
}

fn atomic_wrapper(ty: AtomicType) -> &'static str {
    match ty {
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

fn ordering_str(ordering: AtomicOrdering) -> &'static str {
    match ordering {
        AtomicOrdering::Relaxed => "std::sync::atomic::Ordering::Relaxed",
        AtomicOrdering::Acquire => "std::sync::atomic::Ordering::Acquire",
        AtomicOrdering::Release => "std::sync::atomic::Ordering::Release",
        AtomicOrdering::AcqRel => "std::sync::atomic::Ordering::AcqRel",
        AtomicOrdering::SeqCst => "std::sync::atomic::Ordering::SeqCst",
    }
}

fn rmw_method(op: AtomicRmwOp) -> &'static str {
    match op {
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

/// Streams the Rust AST into a [`Write`] sink. Holds the sink so emission is a
/// series of `&mut self` steps; carrying it here also leaves room for emitter
/// state (config, symbol tables) without rethreading every method signature.
pub struct Codegen<W: Write> {
    out: W,
}

impl<W: Write> Codegen<W> {
    pub fn new(out: W) -> Self {
        Self { out }
    }

    pub fn into_inner(self) -> W {
        self.out
    }

    pub fn program(&mut self, program: &Program) -> fmt::Result {
        for (i, item) in program.items.iter().enumerate() {
            if i > 0 {
                self.out.write_char('\n')?;
            }
            self.item(item)?;
        }
        Ok(())
    }

    fn item(&mut self, item: &Item) -> fmt::Result {
        match item {
            Item::Func(f) => self.func(f)?,
            Item::Fn(f) => self.fn_def(f)?,
            Item::CrateAttrs(attrs) => {
                for attr in attrs {
                    writeln!(self.out, "#![{attr}]")?;
                }
            }
            Item::Mod { name } => writeln!(self.out, "mod {name};")?,
            Item::Use { path } => writeln!(self.out, "use {path};")?,
            Item::Static {
                vis,
                mutable,
                name,
                ty,
                init,
            } => {
                if let Some(vis) = vis {
                    write!(self.out, "{vis} ")?;
                }
                self.out.write_str("static ")?;
                if *mutable {
                    self.out.write_str("mut ")?;
                }
                write!(self.out, "{name}: ")?;
                self.ty(ty)?;
                self.out.write_str(" = ")?;
                self.expr(init)?;
                self.out.write_str(";\n")?;
            }
            Item::ExternBlock { abi, decls } => {
                writeln!(self.out, "unsafe extern \"{abi}\" {{")?;
                for decl in decls {
                    self.out.write_str(INDENT)?;
                    self.extern_decl(decl)?;
                }
                self.out.write_str("}\n")?;
            }
            Item::Enum(consts) => {
                for c in consts {
                    writeln!(self.out, "const {}: i32 = {};", c.name, c.value)?;
                }
                self.out.write_char('\n')?;
            }
            Item::Record(r) => self.record(r)?,
            Item::Raw(s) => writeln!(self.out, "{s}")?,
        }
        Ok(())
    }

    fn fn_def(&mut self, f: &FnDef) -> fmt::Result {
        if let Some(vis) = &f.vis {
            write!(self.out, "{vis} ")?;
        }
        if f.unsafe_extern_c {
            self.out.write_str("unsafe extern \"C\" ")?;
        }
        write!(self.out, "fn {}(", f.name)?;
        for (i, p) in f.params.iter().enumerate() {
            if i > 0 {
                self.out.write_str(", ")?;
            }
            if p.mutable {
                self.out.write_str("mut ")?;
            }
            write!(self.out, "{}: ", p.name)?;
            self.ty(&p.ty)?;
        }
        self.out.write_char(')')?;
        if let Some(ret) = &f.ret {
            self.out.write_str(" -> ")?;
            self.ty(ret)?;
        }
        self.out.write_str(" {\n")?;
        for IndentStmt { depth, stmt } in &f.body {
            self.stmt(stmt, *depth)?;
        }
        self.out.write_str("}\n")
    }

    fn record(&mut self, r: &RecordDef) -> fmt::Result {
        self.out.write_str("#[repr(C)]\n")?;
        if r.allow_non_camel_case {
            self.out.write_str("#[allow(non_camel_case_types)]\n")?;
        }
        self.out.write_str("#[derive(Clone, Copy)]\n")?;
        let kw = if r.is_union { "union" } else { "struct" };
        writeln!(self.out, "{kw} {} {{", r.name)?;
        for (name, ty) in &r.fields {
            write!(self.out, "    {name}: ")?;
            self.ty(ty)?;
            self.out.write_str(",\n")?;
        }
        self.out.write_str("}\n\n")
    }

    fn extern_decl(&mut self, decl: &ExternDecl) -> fmt::Result {
        match decl {
            ExternDecl::Fn(f) => {
                write!(self.out, "fn {}(", f.name)?;
                for (i, p) in f.params.iter().enumerate() {
                    if i > 0 {
                        self.out.write_str(", ")?;
                    }
                    write!(self.out, "{}: ", p.name)?;
                    self.ty(&p.ty)?;
                }
                if f.variadic {
                    if !f.params.is_empty() {
                        self.out.write_str(", ")?;
                    }
                    self.out.write_str("...")?;
                }
                self.out.write_char(')')?;
                if let Some(ret) = &f.ret {
                    self.out.write_str(" -> ")?;
                    self.ty(ret)?;
                }
                self.out.write_str(";\n")
            }
            ExternDecl::Static { mutable, name, ty } => {
                self.out.write_str("static ")?;
                if *mutable {
                    self.out.write_str("mut ")?;
                }
                write!(self.out, "{name}: ")?;
                self.ty(ty)?;
                self.out.write_str(";\n")
            }
        }
    }

    fn func(&mut self, f: &Func) -> fmt::Result {
        write!(self.out, "fn {}(", f.name)?;
        for (i, p) in f.params.iter().enumerate() {
            if i > 0 {
                self.out.write_str(", ")?;
            }
            write!(self.out, "{}: ", p.name)?;
            self.ty(&p.ty)?;
        }
        self.out.write_char(')')?;
        if let Some(ret) = &f.ret {
            self.out.write_str(" -> ")?;
            self.ty(ret)?;
        }
        self.out.write_str(" {\n")?;
        self.block(&f.body, 1)?;
        self.out.write_str("}\n")
    }

    fn block(&mut self, block: &Block, depth: usize) -> fmt::Result {
        for stmt in &block.stmts {
            self.stmt(stmt, depth)?;
        }
        Ok(())
    }

    fn indent_stmts(&mut self, body: &[IndentStmt], depth: usize) -> fmt::Result {
        for IndentStmt { depth: extra, stmt } in body {
            self.stmt(stmt, depth + extra)?;
        }
        Ok(())
    }

    pub fn stmt(&mut self, stmt: &Stmt, depth: usize) -> fmt::Result {
        let pad = INDENT.repeat(depth);
        match stmt {
            Stmt::Let {
                name,
                mutable,
                ty,
                init,
            } => {
                self.out.write_str(&pad)?;
                self.out.write_str("let ")?;
                if *mutable {
                    self.out.write_str("mut ")?;
                }
                self.out.write_str(name)?;
                if let Some(ty) = ty {
                    self.out.write_str(": ")?;
                    self.ty(ty)?;
                }
                if let Some(init) = init {
                    self.out.write_str(" = ")?;
                    self.expr(init)?;
                }
                self.out.write_str(";\n")
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
                self.out.write_str(&pad)?;
                self.out.write_str("let ")?;
                if *mutable {
                    self.out.write_str("mut ")?;
                }
                self.out.write_str(name)?;
                if let Some(ty) = ty {
                    self.out.write_str(": ")?;
                    self.ty(ty)?;
                }
                self.out.write_str(" = if ")?;
                self.expr_spliceable(cond)?;
                self.out.write_str(" {\n")?;
                self.indent_stmts(then_body, depth + 1)?;
                write!(self.out, "{pad}{INDENT}")?;
                self.expr(then_value)?;
                write!(self.out, "\n{pad}}} else {{\n")?;
                self.indent_stmts(else_body, depth + 1)?;
                write!(self.out, "{pad}{INDENT}")?;
                self.expr(else_value)?;
                write!(self.out, "\n{pad}}};\n")
            }
            Stmt::Assign { target, value } => {
                self.out.write_str(&pad)?;
                self.expr(target)?;
                self.out.write_str(" = ")?;
                self.expr(value)?;
                self.out.write_str(";\n")
            }
            Stmt::Expr(e) => {
                self.out.write_str(&pad)?;
                self.expr(e)?;
                self.out.write_str(";\n")
            }
            Stmt::Return(e) => match e {
                Some(e) => {
                    write!(self.out, "{pad}return ")?;
                    self.expr(e)?;
                    self.out.write_str(";\n")
                }
                None => writeln!(self.out, "{pad}return;"),
            },
            Stmt::Unsafe { body } => {
                writeln!(self.out, "{pad}unsafe {{")?;
                self.indent_stmts(body, depth + 1)?;
                writeln!(self.out, "{pad}}}")
            }
            Stmt::If {
                cond,
                then_body,
                else_body,
            } => {
                write!(self.out, "{pad}if ")?;
                self.expr_spliceable(cond)?;
                self.out.write_str(" {\n")?;
                self.indent_stmts(then_body, depth + 1)?;
                if else_body.is_empty() {
                    writeln!(self.out, "{pad}}}")
                } else {
                    writeln!(self.out, "{pad}}} else {{")?;
                    self.indent_stmts(else_body, depth + 1)?;
                    writeln!(self.out, "{pad}}}")
                }
            }
            Stmt::Loop { label, body } => {
                match label {
                    Some(label) => writeln!(self.out, "{pad}{label}: loop {{")?,
                    None => writeln!(self.out, "{pad}loop {{")?,
                }
                self.indent_stmts(body, depth + 1)?;
                writeln!(self.out, "{pad}}}")
            }
            Stmt::Scope { body } => {
                writeln!(self.out, "{pad}{{")?;
                self.indent_stmts(body, depth + 1)?;
                writeln!(self.out, "{pad}}}")
            }
            Stmt::LabeledBlock { label, body } => {
                writeln!(self.out, "{pad}{label}: {{")?;
                self.indent_stmts(body, depth + 1)?;
                writeln!(self.out, "{pad}}}")
            }
            Stmt::Match { expr, arms } => {
                write!(self.out, "{pad}match ")?;
                self.expr_spliceable(expr)?;
                self.out.write_str(" {\n")?;
                for arm in arms {
                    writeln!(self.out, "{pad}{INDENT}{} => {{", arm.pattern)?;
                    self.indent_stmts(&arm.body, depth + 2)?;
                    writeln!(self.out, "{pad}{INDENT}}}")?;
                }
                writeln!(self.out, "{pad}}}")
            }
            Stmt::Break(label) => match label {
                Some(label) => writeln!(self.out, "{pad}break {label};"),
                None => writeln!(self.out, "{pad}break;"),
            },
            Stmt::Continue(label) => match label {
                Some(label) => writeln!(self.out, "{pad}continue {label};"),
                None => writeln!(self.out, "{pad}continue;"),
            },
            Stmt::While { cond, body } => {
                write!(self.out, "{pad}while ")?;
                self.expr(cond)?;
                self.out.write_str(" {\n")?;
                self.block(body, depth + 1)?;
                writeln!(self.out, "{pad}}}")
            }
            Stmt::Block(b) => {
                writeln!(self.out, "{pad}{{")?;
                self.block(b, depth + 1)?;
                writeln!(self.out, "{pad}}}")
            }
            Stmt::Raw(line) => writeln!(self.out, "{pad}{line}"),
        }
    }

    pub fn expr(&mut self, expr: &Expr) -> fmt::Result {
        self.expr_prec(expr, 0)
    }

    // Render for splicing into arbitrary surrounding text (a `Stmt::Raw` line),
    // where the enclosing precedence is unknown. Anything that binds looser than
    // a call is wrapped so the splice can never change precedence.
    fn expr_spliceable(&mut self, expr: &Expr) -> fmt::Result {
        if expr_prec(expr) < PREC_CALL {
            self.parenthesized(expr)
        } else {
            self.expr(expr)
        }
    }

    // Render, wrapping in parens when this expression binds looser than the
    // enclosing position requires. Extra parens are always safe, so the rule is
    // conservative: wrap on `<`, keep bare on `>=`.
    fn expr_prec(&mut self, expr: &Expr, min: u8) -> fmt::Result {
        if expr_prec(expr) < min {
            self.parenthesized(expr)
        } else {
            self.expr_raw(expr)
        }
    }

    fn parenthesized(&mut self, expr: &Expr) -> fmt::Result {
        self.out.write_char('(')?;
        self.expr_raw(expr)?;
        self.out.write_char(')')
    }

    fn expr_raw(&mut self, expr: &Expr) -> fmt::Result {
        match expr {
            Expr::Value(v) => self.value(v),
            Expr::Lit(s) | Expr::Var(s) | Expr::Raw(s) => self.out.write_str(s),
            Expr::Unary { op, expr } => {
                self.out.write_str(op.spelling())?;
                self.prefix_operand(expr)
            }
            Expr::Binary { op, lhs, rhs } => {
                let p = op.precedence();
                // left-assoc: the right operand must bind strictly tighter, so a
                // same-precedence right child (`a - (b - c)`) still needs parens.
                // comparisons are non-associative, so wrap same-precedence on both
                // sides to avoid an illegal `a < b < c` chain.
                let (lmin, rmin) = if op.is_comparison() {
                    (p + 1, p + 1)
                } else {
                    (p, p + 1)
                };
                self.expr_prec(lhs, lmin)?;
                write!(self.out, " {} ", op.spelling())?;
                self.expr_prec(rhs, rmin)
            }
            Expr::Call { func, args } => {
                self.expr_prec(func, PREC_CALL)?;
                self.out.write_char('(')?;
                self.args(args)?;
                self.out.write_char(')')
            }
            Expr::MethodCall { recv, method, args } => {
                self.expr_prec(recv, PREC_CALL)?;
                write!(self.out, ".{method}(")?;
                self.args(args)?;
                self.out.write_char(')')
            }
            Expr::Field { base, field } => {
                self.expr_prec(base, PREC_CALL)?;
                write!(self.out, ".{field}")
            }
            Expr::Index { base, index } => {
                self.expr_prec(base, PREC_CALL)?;
                self.out.write_char('[')?;
                self.expr_spliceable(index)?;
                self.out.write_char(']')
            }
            Expr::StructLit { name, fields } => {
                write!(self.out, "{name} {{ ")?;
                for (i, (field, value)) in fields.iter().enumerate() {
                    if i > 0 {
                        self.out.write_str(", ")?;
                    }
                    write!(self.out, "{field}: ")?;
                    self.expr(value)?;
                }
                self.out.write_str(" }")
            }
            Expr::ArrayLit(elems) => {
                self.out.write_char('[')?;
                self.args(elems)?;
                self.out.write_char(']')
            }
            Expr::ArrayRepeat { elem, len } => {
                self.out.write_char('[')?;
                self.expr(elem)?;
                write!(self.out, "; {len}]")
            }
            Expr::Macro { name, args } => {
                write!(self.out, "{name}!(")?;
                self.args(args)?;
                self.out.write_char(')')
            }
            Expr::Match { expr, arms } => {
                self.out.write_str("match ")?;
                self.expr_spliceable(expr)?;
                self.out.write_str(" { ")?;
                for (i, arm) in arms.iter().enumerate() {
                    if i > 0 {
                        self.out.write_str(", ")?;
                    }
                    write!(self.out, "{} => ", arm.pattern)?;
                    self.expr(&arm.value)?;
                }
                self.out.write_str(" }")
            }
            Expr::If {
                cond,
                then_expr,
                else_expr,
            } => {
                self.out.write_str("if ")?;
                self.expr_spliceable(cond)?;
                self.out.write_str(" { ")?;
                self.expr(then_expr)?;
                self.out.write_str(" } else { ")?;
                self.expr(else_expr)?;
                self.out.write_str(" }")
            }
            Expr::Unsafe(e) => {
                self.out.write_str("unsafe { ")?;
                self.expr(e)?;
                self.out.write_str(" }")
            }
            Expr::Cast { expr, ty } => {
                self.expr_prec(expr, PREC_CAST_OPERAND)?;
                self.out.write_str(" as ")?;
                self.ty(ty)
            }
            Expr::Ref { mutable, expr } => {
                self.out.write_str(if *mutable { "&mut " } else { "&" })?;
                self.prefix_operand(expr)
            }
            Expr::AddrOf { mutable, expr } => {
                self.out.write_str(if *mutable {
                    "std::ptr::addr_of_mut!("
                } else {
                    "std::ptr::addr_of!("
                })?;
                self.expr(expr)?;
                self.out.write_char(')')
            }
            Expr::AtomicRef { ty, ptr } => self.atomic_ref(*ty, ptr),
            Expr::AtomicLoad { ty, ptr, ordering } => {
                self.out.write_str("unsafe { ")?;
                self.atomic_ref(*ty, ptr)?;
                write!(self.out, ".load({}) }}", ordering_str(*ordering))
            }
            Expr::AtomicStore {
                ty,
                ptr,
                value,
                ordering,
            } => {
                self.out.write_str("unsafe { ")?;
                self.atomic_ref(*ty, ptr)?;
                self.out.write_str(".store(")?;
                self.expr(value)?;
                write!(self.out, ", {}) }}", ordering_str(*ordering))
            }
            Expr::AtomicFetch {
                ty,
                op,
                ptr,
                value,
                ordering,
            } => {
                self.out.write_str("unsafe { ")?;
                self.atomic_ref(*ty, ptr)?;
                write!(self.out, ".{}(", rmw_method(*op))?;
                self.expr(value)?;
                write!(self.out, ", {}) }}", ordering_str(*ordering))
            }
            Expr::AtomicSwap {
                ty,
                ptr,
                value,
                ordering,
            } => {
                self.out.write_str("unsafe { ")?;
                self.atomic_ref(*ty, ptr)?;
                self.out.write_str(".swap(")?;
                self.expr(value)?;
                write!(self.out, ", {}) }}", ordering_str(*ordering))
            }
            Expr::AtomicCompareExchange {
                ty,
                ptr,
                expected,
                desired,
                success,
                failure,
            } => {
                self.out.write_str("unsafe { ")?;
                self.atomic_ref(*ty, ptr)?;
                self.out.write_str(".compare_exchange(")?;
                self.expr(expected)?;
                self.out.write_str(", ")?;
                self.expr(desired)?;
                write!(
                    self.out,
                    ", {}, {}) }}",
                    ordering_str(*success),
                    ordering_str(*failure)
                )
            }
            Expr::AtomicFence { ordering } => {
                write!(
                    self.out,
                    "std::sync::atomic::fence({})",
                    ordering_str(*ordering)
                )
            }
            Expr::Transmute { from, to, expr } => {
                self.out.write_str("unsafe { std::mem::transmute::<")?;
                self.ty(from)?;
                self.out.write_str(", ")?;
                self.ty(to)?;
                self.out.write_str(">(")?;
                self.expr(expr)?;
                self.out.write_str(") }")
            }
            Expr::CopyNonoverlapping { src, dst, count } => {
                self.out
                    .write_str("unsafe { std::ptr::copy_nonoverlapping(")?;
                self.expr(src)?;
                self.out.write_str(", ")?;
                self.expr(dst)?;
                write!(self.out, ", {count}) }}")
            }
            Expr::Todo(note) => write!(self.out, "todo!({note:?})"),
        }
    }

    fn atomic_ref(&mut self, ty: AtomicType, ptr: &Expr) -> fmt::Result {
        write!(
            self.out,
            "std::sync::atomic::{}::from_ptr(",
            atomic_wrapper(ty)
        )?;
        self.expr(ptr)?;
        self.out.write_char(')')
    }

    // A prefix operator over another prefix form (`- -a`, `&&x`) tokenizes as
    // `--` or `&&` without a barrier, so parenthesize a nested prefix operand.
    fn prefix_operand(&mut self, expr: &Expr) -> fmt::Result {
        if matches!(expr, Expr::Unary { .. } | Expr::Ref { .. }) {
            self.parenthesized(expr)
        } else {
            self.expr_prec(expr, PREC_PREFIX)
        }
    }

    fn args(&mut self, args: &[Expr]) -> fmt::Result {
        for (i, arg) in args.iter().enumerate() {
            if i > 0 {
                self.out.write_str(", ")?;
            }
            self.expr(arg)?;
        }
        Ok(())
    }

    fn value(&mut self, value: &RustValue) -> fmt::Result {
        match value {
            RustValue::Int(n) => write!(self.out, "{n}"),
            RustValue::NullPtr => self.out.write_str("std::ptr::null_mut()"),
        }
    }

    fn ty(&mut self, ty: &Type) -> fmt::Result {
        match ty {
            Type::Prim(p) => self.out.write_str(p.spelling()),
            Type::Named(n) => self.out.write_str(n),
            Type::Ptr { mutable, inner } => {
                self.out
                    .write_str(if *mutable { "*mut " } else { "*const " })?;
                self.ty(inner)
            }
            Type::Array { elem, len } => {
                self.out.write_char('[')?;
                self.ty(elem)?;
                write!(self.out, "; {len}]")
            }
            Type::FnPtr { params, ret } => {
                self.out.write_str("Option<fn(")?;
                for (i, p) in params.iter().enumerate() {
                    if i > 0 {
                        self.out.write_str(", ")?;
                    }
                    self.ty(p)?;
                }
                self.out.write_str(") -> ")?;
                self.ty(ret)?;
                self.out.write_char('>')
            }
            Type::Unit => self.out.write_str("()"),
        }
    }
}

/// Emits `program` to a fresh `String`. Convenience over [`Codegen`] for the
/// common whole-program case.
pub fn program_to_string(program: &Program) -> String {
    let mut cg = Codegen::new(String::new());
    cg.program(program)
        .expect("writing to a String never fails");
    cg.into_inner()
}

pub fn expr_to_string(expr: &Expr) -> String {
    let mut cg = Codegen::new(String::new());
    cg.expr(expr).expect("writing to a String never fails");
    cg.into_inner()
}

pub fn expr_spliceable_to_string(expr: &Expr) -> String {
    let mut cg = Codegen::new(String::new());
    cg.expr_spliceable(expr)
        .expect("writing to a String never fails");
    cg.into_inner()
}

pub fn type_to_string(ty: &Type) -> String {
    let mut cg = Codegen::new(String::new());
    cg.ty(ty).expect("writing to a String never fails");
    cg.into_inner()
}

pub fn stmt_line_to_string(stmt: &Stmt) -> String {
    let mut cg = Codegen::new(String::new());
    cg.stmt(stmt, 0).expect("writing to a String never fails");
    cg.into_inner().trim_end().to_string()
}
