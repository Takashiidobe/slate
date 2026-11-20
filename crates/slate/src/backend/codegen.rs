use crate::backend::printer::{INDENT, MARGIN, Printer};
use crate::backend::rust_ast::{
    Abi, AsmKind, AsmOperand, AsmReg, AtomicOrdering, AtomicPlace, AtomicRmwOp, AtomicType, Attr,
    Block, Cfg, Comment, CommentAttach, CrateAttr, Derive, Expr, ExternDecl, FnDef, GenericParam,
    ImplBlock, ImplItem, InlineAsm, InlineHint, Item, LayoutQuery, MacroParamShape, Method, Path,
    Pattern, Precedence, Prim, Program, RecordDef, RecordField, Repr, RustValue, SelfKind, Stmt,
    StructDef, StructFields, TraitBound, TraitRef, Type, UnaryOp,
};

fn expr_prec(expr: &Expr) -> Precedence {
    match expr {
        Expr::Range { .. } => Precedence::Range,
        Expr::Binary { op, .. } => op.precedence(),
        Expr::Cast { .. } => Precedence::Cast,
        Expr::Unary { .. } | Expr::Ref { .. } => Precedence::Prefix,
        Expr::Call { .. }
        | Expr::MethodCall { .. }
        | Expr::MethodCallGeneric { .. }
        | Expr::Field { .. }
        | Expr::TupleField { .. }
        | Expr::ArrayPtr { .. }
        | Expr::Index { .. } => Precedence::Call,
        _ => Precedence::Atom,
    }
}

fn starts_with_brace_expr(expr: &Expr) -> bool {
    match expr {
        Expr::Var(name) => name.as_str().starts_with('$'),
        _ => matches!(
            expr,
            Expr::Block(_)
                | Expr::Unsafe(_)
                | Expr::ConstBlock(_)
                | Expr::Transmute { .. }
                | Expr::CopyNonoverlapping { .. }
                | Expr::If { .. }
                | Expr::Match { .. }
        ),
    }
}

fn expr_contains_bare_struct_lit(expr: &Expr) -> bool {
    match expr {
        Expr::StructLit { .. } => true,
        Expr::Binary { lhs, rhs, .. } => {
            expr_contains_bare_struct_lit(lhs) || expr_contains_bare_struct_lit(rhs)
        }
        Expr::Range { start, end } => {
            expr_contains_bare_struct_lit(start) || expr_contains_bare_struct_lit(end)
        }
        Expr::Cast { expr, .. }
        | Expr::Unary { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. } => expr_contains_bare_struct_lit(expr),
        Expr::Field { base, .. } | Expr::TupleField { base, .. } | Expr::Index { base, .. } => {
            expr_contains_bare_struct_lit(base)
        }
        Expr::ArrayPtr { array, .. } => expr_contains_bare_struct_lit(array),
        Expr::Call { func, .. } => expr_contains_bare_struct_lit(func),
        Expr::MethodCall { recv, .. } | Expr::MethodCallGeneric { recv, .. } => {
            expr_contains_bare_struct_lit(recv)
        }
        _ => false,
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
        AtomicOrdering::Relaxed => "core::sync::atomic::Ordering::Relaxed",
        AtomicOrdering::Acquire => "core::sync::atomic::Ordering::Acquire",
        AtomicOrdering::Release => "core::sync::atomic::Ordering::Release",
        AtomicOrdering::AcqRel => "core::sync::atomic::Ordering::AcqRel",
        AtomicOrdering::SeqCst => "core::sync::atomic::Ordering::SeqCst",
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
pub const MACRO_LIST_HOLE: &str = "__slate_macro_list_";

const FLAT_MARGIN: isize = 0x8000;

fn macro_list_hole(expr: &Expr) -> Option<&str> {
    match expr {
        Expr::Var(name) => name.as_str().strip_prefix(MACRO_LIST_HOLE),
        _ => None,
    }
}

fn is_simple_literal(expr: &Expr) -> bool {
    match expr {
        Expr::Value(_) | Expr::Str(_) | Expr::HexFloat(_) => true,
        Expr::Unary {
            op: UnaryOp::Neg,
            expr,
        } => matches!(**expr, Expr::Value(_) | Expr::HexFloat(_)),
        _ => false,
    }
}

enum Piece<'a> {
    Expr(&'a Expr),
    Text(String),
    Asm(&'a AsmOperand),
}

/// Writes Rust source for backend AST nodes.
pub struct Codegen {
    p: Printer,
    macro_params: Vec<String>,
    no_unwind: bool,
}

impl Codegen {
    fn new(margin: isize) -> Self {
        Self {
            p: Printer::new(margin),
            macro_params: Vec::new(),
            no_unwind: false,
        }
    }

    fn finish(self) -> String {
        self.p.eof()
    }

    fn word(&mut self, text: impl Into<std::borrow::Cow<'static, str>>) {
        self.p.word(text);
    }

    fn ident_text(&self, name: &str) -> String {
        let is_hole = name.strip_prefix('$').is_some_and(|rest| {
            let rest = rest.strip_prefix('*').unwrap_or(rest);
            self.macro_params.iter().any(|param| param == rest)
        });
        if name.contains('$') && !is_hole {
            return escape_ident(&name.replace('$', "_dollar_"));
        }
        escape_ident(name)
    }

    fn ident(&mut self, name: &str) {
        let text = self.ident_text(name);
        self.word(text);
    }

    fn ident_path(&mut self, path: &str) {
        let text = path
            .split("::")
            .map(|segment| self.ident_text(segment))
            .collect::<Vec<_>>()
            .join("::");
        self.word(text);
    }

    fn open_block(&mut self) {
        self.p.cbox(INDENT);
    }

    fn close_block(&mut self) {
        self.p.offset(-INDENT);
        self.p.end();
        self.word("}");
    }

    fn braced_stmts(&mut self, body: &[Stmt]) {
        self.word("{");
        self.p.hardbreak_if_nonempty();
        self.stmts(body);
        self.close_block();
    }

    fn braced_block(&mut self, block: &Block) {
        self.word("{");
        self.p.hardbreak_if_nonempty();
        self.block(block);
        self.close_block();
    }

    fn small_block(&mut self, f: impl FnOnce(&mut Self)) {
        self.p.cbox(INDENT);
        self.word("{");
        self.p.space();
        self.p.ibox(0);
        f(self);
        self.p.end();
        self.p.space();
        self.p.offset(-INDENT);
        self.word("}");
        self.p.end();
    }

    /// Emits a complete Rust program.
    pub fn program(&mut self, program: &Program) {
        self.no_unwind = program.no_unwind;
        for (i, item) in program.items.iter().enumerate() {
            if i > 0
                && !matches!(program.items[i - 1], Item::Comment(_))
                && !same_item_group(&program.items[i - 1], item)
            {
                self.p.hardbreak();
            }
            self.item(item);
        }
    }

    fn item(&mut self, item: &Item) {
        match item {
            Item::Fn(f) => self.fn_def(f),
            Item::Comment(comment) => self.plain_comment(comment),
            Item::CrateAttrs(attrs) => {
                for attr in attrs {
                    self.word("#![");
                    self.crate_attr(attr);
                    self.word("]");
                    self.p.hardbreak();
                }
            }
            Item::Mod { name, path } => {
                if let Some(path) = path {
                    self.word(format!("#[path = {path:?}]"));
                    self.p.hardbreak();
                }
                self.word("mod ");
                self.ident(name.as_str());
                self.word(";");
                self.p.hardbreak();
            }
            Item::InlineMod { vis, name, items } => {
                self.open_block();
                if let Some(kw) = vis.keyword() {
                    self.word(format!("{kw} "));
                }
                self.word("mod ");
                self.ident(name.as_str());
                self.word(" {");
                self.p.hardbreak_if_nonempty();
                for item in items {
                    self.item(item);
                }
                self.close_block();
                self.p.hardbreak();
            }
            Item::Use { vis, path, alias } => {
                if let Some(vis) = vis.keyword() {
                    self.word(format!("{vis} "));
                }
                self.word("use ");
                self.path(path);
                if let Some(alias) = alias {
                    self.word(" as ");
                    self.ident(alias.as_str());
                }
                self.word(";");
                self.p.hardbreak();
            }
            Item::Static {
                comments,
                attrs,
                vis,
                mutable,
                name,
                ty,
                init,
            } => {
                for comment in comments {
                    self.comment(comment);
                }
                self.attrs(attrs);
                self.p.ibox(0);
                if let Some(kw) = vis.keyword() {
                    self.word(format!("{kw} "));
                }
                self.word("static ");
                if *mutable {
                    self.word("mut ");
                }
                self.ident(name);
                self.word(": ");
                self.ty(ty);
                self.word(" = ");
                self.p.neverbreak();
                self.expr(init);
                self.p.end();
                self.word(";");
                self.p.hardbreak();
            }
            Item::Const {
                attrs,
                name,
                ty,
                init,
            } => {
                self.attrs(attrs);
                self.p.ibox(0);
                self.word("const ");
                self.ident(name);
                self.word(": ");
                self.ty(ty);
                self.word(" = ");
                self.p.neverbreak();
                self.expr(init);
                self.p.end();
                self.word(";");
                self.p.hardbreak();
            }
            Item::TypeAlias { name, ty } => {
                self.word("type ");
                self.ident(name);
                self.word(" = ");
                self.ty(ty);
                self.word(";");
                self.p.hardbreak();
            }
            Item::ExternBlock { abi, decls } => {
                self.open_block();
                self.word(format!("unsafe extern \"{abi}\" {{"));
                self.p.hardbreak_if_nonempty();
                for decl in decls {
                    self.extern_decl(decl);
                }
                self.close_block();
                self.p.hardbreak();
            }
            Item::Enum(e) => self.enum_def(e),
            Item::Record(r) => self.record(r),
            Item::Struct(s) => self.struct_def(s),
            Item::Impl(im) => self.impl_block(im),
            Item::Macro { name, args } => {
                self.ident_path(name);
                self.word("!(");
                self.macro_args(args);
                self.word(");");
                self.p.hardbreak();
            }
            Item::MacroRules {
                comments,
                name,
                params,
                body,
                value: is_value,
                returns,
            } => {
                for comment in comments {
                    self.comment(comment);
                }
                self.macro_params = params.iter().map(|param| param.name.to_string()).collect();
                let mut matcher = String::new();
                for (index, param) in params.iter().enumerate() {
                    let separator = if index == 0 { "" } else { ", " };
                    let name = &param.name;
                    let piece = match param.shape {
                        MacroParamShape::Ident => format!("{separator}${name}:ident"),
                        MacroParamShape::Type => format!("{separator}${name}:ty"),
                        MacroParamShape::Expr => format!("{separator}${name}:expr"),
                        MacroParamShape::Repeat if index == 0 => format!("$(${name}:expr),*"),
                        MacroParamShape::Repeat => format!(" $(, ${name}:expr)*"),
                    };
                    matcher.push_str(&piece);
                }
                self.open_block();
                self.word(format!("macro_rules! {name} {{"));
                self.p.hardbreak();
                self.open_block();
                self.word(format!("({matcher}) => {{"));
                self.p.hardbreak_if_nonempty();
                match body.split_last() {
                    Some((Stmt::Return(Some(value)), [])) if *is_value => {
                        self.expr(value);
                        self.p.hardbreak();
                    }
                    Some((Stmt::Return(Some(value)), statements)) if *is_value => {
                        self.open_block();
                        self.word("{");
                        self.p.hardbreak();
                        self.stmts(statements);
                        self.expr(value);
                        self.p.hardbreak();
                        self.close_block();
                        self.p.hardbreak();
                    }
                    _ if *returns => {
                        self.open_block();
                        self.braced_stmts(body);
                        self.p.hardbreak();
                    }
                    _ => self.stmts(body),
                }
                self.close_block();
                self.word(";");
                self.p.hardbreak();
                self.close_block();
                self.p.hardbreak();
                self.macro_params.clear();
            }
            Item::MacroUseMod { name } => {
                self.word("#[macro_use]");
                self.p.hardbreak();
                self.word(format!("mod {};", name.as_str()));
                self.p.hardbreak();
            }
            Item::Cfg { cfg, item } => {
                self.cfg_attr_line(cfg);
                self.item(item);
            }
            Item::SupportModule(module) => {
                for line in module.source.lines() {
                    if !line.is_empty() {
                        self.word(line.to_string());
                    }
                    self.p.hardbreak();
                }
            }
            Item::GlobalAsm(asm) | Item::Comdat { asm, .. } => {
                self.inline_asm(asm);
                self.word(";");
                self.p.hardbreak();
            }
        }
    }

    fn cfg_attr_line(&mut self, cfg: &Cfg) {
        self.word("#[cfg(");
        self.cfg(cfg);
        self.word(")]");
        self.p.hardbreak();
    }

    fn comment(&mut self, comment: &Comment) {
        let marker = match (comment.attach, comment.doc) {
            (CommentAttach::Prologue, _) => "//!",
            (_, true) => "///",
            (_, false) => "//",
        };
        self.comment_lines(&comment.lines, marker);
    }

    fn plain_comment(&mut self, comment: &Comment) {
        let marker = match comment.attach {
            CommentAttach::Prologue => "//!",
            _ => "//",
        };
        self.comment_lines(&comment.lines, marker);
    }

    fn comment_lines(&mut self, lines: &[String], marker: &'static str) {
        for line in lines {
            if line.is_empty() {
                self.word(marker);
            } else {
                self.word(format!("{marker} {line}"));
            }
            self.p.hardbreak();
        }
    }

    fn fn_comments(&mut self, f: &FnDef) {
        for comment in &f.comments {
            self.comment(comment);
        }
        let mut notes = Vec::new();
        let mut arguments = Vec::new();
        for param in &f.params {
            let (documented, plain): (Vec<_>, Vec<_>) =
                param.comments.iter().partition(|comment| comment.doc);
            for comment in plain {
                let mut lines = comment.lines.clone();
                if let Some(first) = lines.first_mut() {
                    *first = format!("{}: {first}", param.name);
                }
                notes.push(lines);
            }
            let lines: Vec<&String> = documented
                .iter()
                .flat_map(|comment| &comment.lines)
                .collect();
            if !lines.is_empty() {
                arguments.push((&param.name, lines));
            }
        }
        for lines in notes {
            self.comment_lines(&lines, "//");
        }
        if arguments.is_empty() {
            return;
        }
        if f.comments.iter().any(|comment| comment.doc) {
            self.comment_lines(&[String::new()], "///");
        }
        self.comment_lines(&["# Arguments".into(), String::new()], "///");
        for (name, lines) in arguments {
            for (index, line) in lines.into_iter().enumerate() {
                let line = match (index, line.is_empty()) {
                    (0, _) => format!("* `{name}` - {line}"),
                    (_, true) => String::new(),
                    (_, false) => format!("  {line}"),
                };
                self.comment_lines(&[line], "///");
            }
        }
    }

    fn cfg(&mut self, cfg: &Cfg) {
        match cfg {
            Cfg::Flag(name) => self.word(name.to_string()),
            Cfg::Opt { key, value } => self.word(format!("{key} = \"{value}\"")),
            Cfg::Not(inner) => {
                self.word("not(");
                self.cfg(inner);
                self.word(")");
            }
            Cfg::Any(items) => self.cfg_list("any", items),
            Cfg::All(items) => self.cfg_list("all", items),
        }
    }

    fn cfg_list(&mut self, keyword: &'static str, items: &[Cfg]) {
        self.word(keyword);
        self.word("(");
        for (i, item) in items.iter().enumerate() {
            if i > 0 {
                self.word(", ");
            }
            self.cfg(item);
        }
        self.word(")");
    }

    fn params<T>(&mut self, params: &[T], variadic: bool, mut each: impl FnMut(&mut Self, &T)) {
        self.word("(");
        self.p.neverbreak();
        self.p.cbox(0);
        self.p.zerobreak();
        for (i, param) in params.iter().enumerate() {
            each(self, param);
            self.p.trailing_comma(i + 1 == params.len() && !variadic);
        }
        if variadic {
            self.word("...");
            self.p.zerobreak();
        }
        self.p.offset(-INDENT);
        self.p.end();
        self.word(")");
    }

    fn ret(&mut self, ret: Option<&Type>) {
        if let Some(ret) = ret
            && !ret.is_unit()
        {
            self.word(" -> ");
            self.ty(ret);
        }
    }

    fn fn_def(&mut self, f: &FnDef) {
        self.fn_comments(f);
        self.attrs(&f.attrs);
        self.open_block();
        if let Some(kw) = f.vis.keyword() {
            self.word(format!("{kw} "));
        }
        if f.unsafe_ {
            self.word("unsafe ");
        }
        if let Some(abi) = f.abi {
            self.abi(abi);
            self.word(" ");
        }
        self.word("fn ");
        self.ident(&f.name);
        self.params(&f.params, false, |this, p| {
            if p.mutable {
                this.word("mut ");
            }
            this.ident(&p.name);
            this.word(": ");
            this.ty(&p.ty);
        });
        self.ret(f.ret.as_ref());
        self.word(" {");
        self.p.hardbreak_if_nonempty();
        match f.body.split_last() {
            Some((Stmt::Expr(tail), init)) if f.ret.is_some() => {
                self.stmts(init);
                self.expr(tail);
                self.p.hardbreak();
            }
            _ => self.stmts(&f.body),
        }
        self.close_block();
        self.p.hardbreak();
    }

    fn record(&mut self, r: &RecordDef) {
        for comment in &r.comments {
            self.comment(comment);
        }
        let mut repr = vec![Repr::C];
        if let Some(n) = r.packed {
            repr.push(Repr::Packed(n));
        }
        if let Some(n) = r.align {
            repr.push(Repr::Align(n));
        }
        self.attrs(&[Attr::Repr(repr)]);
        if record_fields_are_copy(&r.fields) {
            self.word("#[derive(Clone, Copy)]");
        } else {
            self.word("#[derive(Clone)]");
        }
        self.p.hardbreak();
        self.open_block();
        if let Some(vis) = r.vis.keyword() {
            self.word(format!("{vis} "));
        }
        self.word(if r.is_union { "union " } else { "struct " });
        self.ident(&r.name);
        self.word(" {");
        self.p.hardbreak_if_nonempty();
        for field in &r.fields {
            for comment in &field.comments {
                self.comment(comment);
            }
            if let Some(cfg) = &field.cfg {
                self.cfg_attr_line(cfg);
            }
            if let Some(vis) = r.field_vis.keyword() {
                self.word(format!("{vis} "));
            }
            self.ident(field.name.as_str());
            self.word(": ");
            self.ty(&field.ty);
            self.word(",");
            self.p.hardbreak();
        }
        for comment in &r.trailing_comments {
            self.plain_comment(comment);
        }
        self.close_block();
        self.p.hardbreak();
    }

    fn enum_def(&mut self, e: &crate::backend::rust_ast::EnumDef) {
        for comment in &e.comments {
            self.comment(comment);
        }
        self.attrs(&e.attrs);
        self.open_block();
        if let Some(vis) = e.vis.keyword() {
            self.word(format!("{vis} "));
        }
        self.word("enum ");
        self.ident(&e.name);
        self.word(" {");
        self.p.hardbreak_if_nonempty();
        for variant in &e.variants {
            for comment in &variant.comments {
                self.comment(comment);
            }
            self.ident(&variant.name);
            self.word(format!(" = {},", variant.value));
            self.p.hardbreak();
        }
        self.close_block();
        self.p.hardbreak();
    }

    fn struct_def(&mut self, s: &StructDef) {
        self.attrs(&s.attrs);
        self.open_block();
        if let Some(vis) = s.vis.keyword() {
            self.word(format!("{vis} "));
        }
        self.word("struct ");
        self.ident(&s.name);
        self.generics(&s.generics);
        match &s.fields {
            StructFields::Tuple(tys) => {
                self.word("(");
                for (i, ty) in tys.iter().enumerate() {
                    if i > 0 {
                        self.word(", ");
                    }
                    if let Some(vis) = s.field_vis.keyword() {
                        self.word(format!("{vis} "));
                    }
                    self.ty(ty);
                }
                self.word(");");
                self.p.end();
            }
            StructFields::Named(fields) => {
                self.word(" {");
                self.p.hardbreak_if_nonempty();
                for field in fields {
                    self.attrs(&field.attrs);
                    if let Some(vis) = s.field_vis.keyword() {
                        self.word(format!("{vis} "));
                    }
                    self.ident(&field.name);
                    self.word(": ");
                    self.ty(&field.ty);
                    self.word(",");
                    self.p.hardbreak();
                }
                self.close_block();
            }
        }
        self.p.hardbreak();
    }

    fn attrs(&mut self, attrs: &[Attr]) {
        for attr in attrs {
            self.word("#[");
            self.attr(attr);
            self.word("]");
            self.p.hardbreak();
        }
    }

    fn crate_attr(&mut self, attr: &CrateAttr) {
        let (kind, lints) = match attr {
            CrateAttr::Allow(lints) => ("allow", lints),
            CrateAttr::Deny(lints) => ("deny", lints),
            CrateAttr::Feature(feature) => {
                return self.word(format!("feature({})", feature.spelling()));
            }
            CrateAttr::NoMain => return self.word("no_main"),
            CrateAttr::NoStd => return self.word("no_std"),
            CrateAttr::NoBuiltins => return self.word("no_builtins"),
        };
        let lints = lints
            .iter()
            .map(|lint| lint.spelling())
            .collect::<Vec<_>>()
            .join(", ");
        self.word(format!("{kind}({lints})"));
    }

    fn attr(&mut self, attr: &Attr) {
        match attr {
            Attr::Call { path, args } => {
                self.path(path);
                self.word("(");
                for (index, arg) in args.iter().enumerate() {
                    if index > 0 {
                        self.word(", ");
                    }
                    self.attr_arg(arg);
                }
                self.word(")");
            }
            Attr::Allow(items) => {
                let items = items
                    .iter()
                    .map(|item| item.spelling())
                    .collect::<Vec<_>>()
                    .join(", ");
                self.word(format!("allow({items})"));
            }
            Attr::Repr(items) => {
                let items = items
                    .iter()
                    .map(|r| match r {
                        Repr::C => "C".to_string(),
                        Repr::Transparent => "transparent".to_string(),
                        Repr::Align(n) => format!("align({n})"),
                        Repr::Packed(1) => "packed".to_string(),
                        Repr::Packed(n) => format!("packed({n})"),
                    })
                    .collect::<Vec<_>>()
                    .join(", ");
                self.word(format!("repr({items})"));
            }
            Attr::Derive(items) => {
                let items = items
                    .iter()
                    .map(|d| match d {
                        Derive::Clone => "Clone",
                        Derive::Copy => "Copy",
                        Derive::PartialEq => "PartialEq",
                        Derive::PartialOrd => "PartialOrd",
                        Derive::Eq => "Eq",
                        Derive::Debug => "Debug",
                        Derive::Hash => "Hash",
                    })
                    .collect::<Vec<_>>()
                    .join(", ");
                self.word(format!("derive({items})"));
            }
            Attr::NoMangle => self.word("unsafe(no_mangle)"),
            Attr::Naked => self.word("unsafe(naked)"),
            Attr::ExportName(name) => {
                self.word(format!("unsafe(export_name = {})", string_literal(name)));
            }
            Attr::LinkName(name) => self.word(format!("link_name = {}", string_literal(name))),
            Attr::ThreadLocal => self.word("thread_local"),
            Attr::WeakLinkage => self.word("linkage = \"weak\""),
            Attr::ExternWeakLinkage => self.word("linkage = \"extern_weak\""),
            Attr::CfgAttr(cfg, attr) => {
                self.word("cfg_attr(");
                self.cfg(cfg);
                self.word(", ");
                self.attr(attr);
                self.word(")");
            }
            Attr::LinkSection(section) => {
                self.word(format!(
                    "unsafe(link_section = {})",
                    string_literal(section)
                ));
            }
            Attr::Used(kind) => self.word(match kind {
                crate::backend::rust_ast::UsedKind::Compiler => "used(compiler)",
                crate::backend::rust_ast::UsedKind::Linker => "used(linker)",
            }),
            Attr::Deprecated(None) => self.word("deprecated"),
            Attr::Deprecated(Some(note)) => {
                self.word(format!("deprecated(note = {})", string_literal(note)));
            }
            Attr::TargetFeature(features) => {
                self.word(format!(
                    "target_feature(enable = {})",
                    string_literal(features)
                ));
            }
            Attr::Cold => self.word("cold"),
            Attr::MustUse => self.word("must_use"),
            Attr::Inline(InlineHint::Always) => self.word("inline(always)"),
            Attr::Inline(InlineHint::Never) => self.word("inline(never)"),
        }
    }

    fn attr_arg(&mut self, arg: &crate::backend::rust_ast::AttrArg) {
        use crate::backend::rust_ast::AttrArg;
        match arg {
            AttrArg::Type(ty) => self.ty(ty),
            AttrArg::UInt(value) => self.word(value.to_string()),
            AttrArg::Bool(value) => self.word(value.to_string()),
            AttrArg::Named(name, value) => {
                self.ident(name);
                self.word(" = ");
                self.attr_arg(value);
            }
        }
    }

    fn abi(&mut self, abi: Abi) {
        let spelling = if self.no_unwind && abi != Abi::Rust {
            abi.foreign_block()
        } else {
            abi.spelling()
        };
        self.word(format!("extern \"{spelling}\""));
    }

    fn generics(&mut self, generics: &[GenericParam]) {
        if generics.is_empty() {
            return;
        }
        self.word("<");
        for (i, g) in generics.iter().enumerate() {
            if i > 0 {
                self.word(", ");
            }
            self.ident(&g.name);
            for (j, bound) in g.bounds.iter().enumerate() {
                self.word(if j == 0 { ": " } else { " + " });
                self.trait_bound(bound);
            }
        }
        self.word(">");
    }

    fn trait_bound(&mut self, bound: &TraitBound) {
        self.word(bound.trait_.path());
        if !bound.assoc.is_empty() {
            self.word("<");
            for (i, (name, ty)) in bound.assoc.iter().enumerate() {
                if i > 0 {
                    self.word(", ");
                }
                self.ident(name);
                self.word(" = ");
                self.ty(ty);
            }
            self.word(">");
        }
    }

    fn impl_block(&mut self, im: &ImplBlock) {
        self.open_block();
        if matches!(im.trait_, Some(TraitRef::Sync)) {
            self.word("unsafe ");
        }
        self.word("impl");
        self.generics(&im.generics);
        self.word(" ");
        match &im.trait_ {
            Some(TraitRef::Std(tr)) => self.word(format!("{} for ", tr.path())),
            Some(TraitRef::From(from_ty)) => {
                self.word("From<");
                self.ty(from_ty);
                self.word("> for ");
            }
            Some(TraitRef::Sync) => self.word("Sync for "),
            None => {}
        }
        self.ty(&im.self_ty);
        self.word(" {");
        self.p.hardbreak_if_nonempty();
        for item in &im.items {
            match item {
                ImplItem::AssocType { name, ty } => {
                    self.word("type ");
                    self.ident(name);
                    self.word(" = ");
                    self.ty(ty);
                    self.word(";");
                    self.p.hardbreak();
                }
                ImplItem::Method(m) => self.method(m),
            }
        }
        self.close_block();
        self.p.hardbreak();
    }

    fn method(&mut self, m: &Method) {
        self.open_block();
        if m.is_const {
            self.word("const ");
        }
        self.word("fn ");
        self.ident(&m.name);
        let self_kw = match m.self_kind {
            SelfKind::None => None,
            SelfKind::Value => Some("self"),
            SelfKind::Ref => Some("&self"),
            SelfKind::RefMut => Some("&mut self"),
        };
        let mut params: Vec<Option<&crate::backend::rust_ast::FnParam>> =
            self_kw.iter().map(|_| None).collect();
        params.extend(m.params.iter().map(Some));
        self.params(&params, false, |this, p| match p {
            None => this.word(self_kw.unwrap_or_default()),
            Some(p) => {
                this.ident(&p.name);
                this.word(": ");
                this.ty(&p.ty);
            }
        });
        self.ret(m.ret.as_ref());
        self.word(" {");
        self.p.hardbreak();
        self.expr(&m.body);
        self.p.hardbreak();
        self.close_block();
        self.p.hardbreak();
    }

    fn extern_decl(&mut self, decl: &ExternDecl) {
        match decl {
            ExternDecl::Fn(f) => {
                self.attrs(&f.attrs);
                self.open_block();
                if f.safe {
                    self.word("safe ");
                }
                self.word("fn ");
                self.ident(&f.name);
                self.params(&f.params, f.variadic, |this, p| {
                    this.ident(&p.name);
                    this.word(": ");
                    this.ty(&p.ty);
                });
                self.ret(f.ret.as_ref());
                self.word(";");
                self.p.end();
                self.p.hardbreak();
            }
            ExternDecl::Static {
                attrs,
                mutable,
                name,
                ty,
            } => {
                self.attrs(attrs);
                self.word("static ");
                if *mutable {
                    self.word("mut ");
                }
                self.ident(name);
                self.word(": ");
                self.ty(ty);
                self.word(";");
                self.p.hardbreak();
            }
        }
    }

    fn block(&mut self, block: &Block) {
        self.stmts(&block.stmts);
        if let Some(tail) = &block.tail {
            self.expr(tail);
            self.p.hardbreak();
        }
    }

    fn stmts(&mut self, body: &[Stmt]) {
        let mut index = 0;
        while index < body.len() {
            let stmt = &body[index];
            if !matches!(stmt, Stmt::Comment(_))
                && let Some(Stmt::Comment(comment)) = body.get(index + 1)
                && comment.attach == CommentAttach::Trailing
                && let [line] = comment.lines.as_slice()
            {
                self.stmt_inner(stmt);
                self.word(format!(" // {line}"));
                self.p.hardbreak();
                index += 2;
                continue;
            }
            self.stmt(stmt);
            index += 1;
        }
    }

    pub fn stmt(&mut self, stmt: &Stmt) {
        if let Stmt::Comment(comment) = stmt {
            self.plain_comment(comment);
        } else {
            self.stmt_inner(stmt);
            self.p.hardbreak();
        }
    }

    fn let_head(&mut self, name: &str, mutable: bool, ty: Option<&Type>) {
        self.word("let ");
        if mutable {
            self.word("mut ");
        }
        self.ident(name);
        if let Some(ty) = ty {
            self.word(": ");
            self.ty(ty);
        }
    }

    fn assign_like(&mut self, target: &Expr, op: String, value: &Expr) {
        self.p.ibox(0);
        self.expr(target);
        self.word(op);
        self.p.neverbreak();
        self.expr(value);
        self.p.end();
        self.word(";");
    }

    fn condition(&mut self, cond: &Expr) {
        self.p.cbox(-INDENT);
        self.cond_expr(cond);
        self.p.end();
    }

    fn label(&mut self, label: Option<&crate::backend::rust_ast::Label>) {
        if let Some(label) = label {
            self.word(format!("'{}: ", label.as_str()));
        }
    }

    fn stmt_inner(&mut self, stmt: &Stmt) {
        match stmt {
            Stmt::Comment(comment) => self.plain_comment(comment),
            Stmt::Cfg { cfg, body } => {
                self.cfg_attr_line(cfg);
                match body.as_slice() {
                    [stmt @ (Stmt::Let { .. } | Stmt::LetIf { .. } | Stmt::Block(_))] => {
                        self.stmt_inner(stmt);
                    }
                    _ => {
                        self.open_block();
                        self.braced_stmts(body);
                    }
                }
            }
            Stmt::Let {
                name,
                mutable,
                ty,
                init,
            } => {
                self.p.ibox(0);
                self.let_head(name, *mutable, ty.as_ref());
                if let Some(init) = init {
                    self.word(" = ");
                    self.p.neverbreak();
                    self.expr(init);
                }
                self.p.end();
                self.word(";");
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
                self.open_block();
                self.let_head(name, *mutable, ty.as_ref());
                self.word(" = if ");
                self.condition(cond);
                self.word(" {");
                self.p.hardbreak();
                self.stmts(then_body);
                self.expr(then_value);
                self.p.hardbreak();
                self.close_block();
                self.word(" else ");
                self.open_block();
                self.word("{");
                self.p.hardbreak();
                self.stmts(else_body);
                self.expr(else_value);
                self.p.hardbreak();
                self.close_block();
                self.word(";");
            }
            Stmt::Assign { target, value } => self.assign_like(target, " = ".into(), value),
            Stmt::CompoundAssign { target, op, value } => {
                self.assign_like(target, format!(" {}= ", op.spelling()), value);
            }
            Stmt::InlineAsm(asm) => {
                self.inline_asm(asm);
                self.word(";");
            }
            Stmt::Expr(e) => {
                self.p.ibox(0);
                self.expr(e);
                self.word(";");
                self.p.end();
            }
            Stmt::Return(e) => match e {
                Some(e) => {
                    self.p.ibox(0);
                    self.word("return ");
                    self.expr(e);
                    self.word(";");
                    self.p.end();
                }
                None => self.word("return;"),
            },
            Stmt::Unsafe { body } => {
                self.open_block();
                self.word("unsafe ");
                self.braced_block(body);
            }
            Stmt::If {
                cond,
                then_body,
                else_body,
            } => {
                self.open_block();
                self.word("if ");
                self.condition(cond);
                self.word(" ");
                self.braced_stmts(then_body);
                if !else_body.is_empty() {
                    self.word(" else ");
                    self.open_block();
                    self.braced_stmts(else_body);
                }
            }
            Stmt::Loop { label, body } => {
                self.open_block();
                self.label(label.as_ref());
                self.word("loop ");
                self.braced_stmts(body);
            }
            Stmt::For {
                label,
                pat,
                iter,
                body,
            } => {
                self.open_block();
                self.label(label.as_ref());
                self.word("for ");
                self.ident(pat);
                self.word(" in ");
                self.condition(iter);
                self.word(" ");
                self.braced_stmts(body);
            }
            Stmt::Scope { body, .. } => {
                self.open_block();
                self.braced_stmts(body);
            }
            Stmt::LabeledBlock { label, body } => {
                self.open_block();
                self.label(Some(label));
                self.braced_stmts(body);
            }
            Stmt::Match { expr, arms } => {
                self.open_block();
                self.word("match ");
                self.condition(expr);
                self.word(" {");
                self.p.hardbreak_if_nonempty();
                for arm in arms {
                    self.open_block();
                    self.pattern(&arm.pattern);
                    self.word(" => ");
                    self.braced_stmts(&arm.body);
                    self.p.hardbreak();
                }
                self.close_block();
            }
            Stmt::Break(label) => match label {
                Some(label) => self.word(format!("break '{};", label.as_str())),
                None => self.word("break;"),
            },
            Stmt::Continue(label) => match label {
                Some(label) => self.word(format!("continue '{};", label.as_str())),
                None => self.word("continue;"),
            },
            Stmt::While { label, cond, body } => {
                self.open_block();
                self.label(label.as_ref());
                self.word("while ");
                self.condition(cond);
                self.word(" ");
                self.braced_block(body);
            }
            Stmt::Block(b) => {
                self.open_block();
                self.braced_block(b);
            }
        }
    }

    fn pieces(&mut self, pieces: &[Piece]) {
        self.p.cbox(INDENT);
        self.p.zerobreak();
        for (i, piece) in pieces.iter().enumerate() {
            match piece {
                Piece::Expr(expr) => self.expr(expr),
                Piece::Text(text) => self.word(text.clone()),
                Piece::Asm(operand) => self.asm_operand(operand),
            }
            self.p.trailing_comma(i + 1 == pieces.len());
        }
        self.p.offset(-INDENT);
        self.p.end();
    }

    fn inline_asm(&mut self, asm: &InlineAsm) {
        self.word(match asm.kind {
            AsmKind::Statement => "core::arch::asm!(",
            AsmKind::Naked => "core::arch::naked_asm!(",
            AsmKind::Global => "core::arch::global_asm!(",
        });
        let mut pieces = vec![Piece::Text(string_literal(&asm.template.to_string()))];
        pieces.extend(asm.operands.iter().map(Piece::Asm));
        if !asm.options.is_empty() {
            let options = asm
                .options
                .iter()
                .map(|option| option.spelling())
                .collect::<Vec<_>>()
                .join(", ");
            pieces.push(Piece::Text(format!("options({options})")));
        }
        self.pieces(&pieces);
        self.word(")");
    }

    fn asm_operand(&mut self, operand: &AsmOperand) {
        match operand {
            AsmOperand::In { reg, value } => {
                self.word(format!("in({}) ", asm_reg(reg)));
                self.expr(value);
            }
            AsmOperand::Out { reg, late, value } => {
                let kw = if *late { "lateout" } else { "out" };
                self.word(format!("{kw}({}) ", asm_reg(reg)));
                self.expr(value);
            }
            AsmOperand::InOut {
                reg,
                late,
                input,
                output,
            } => {
                let kw = if *late { "inlateout" } else { "inout" };
                self.word(format!("{kw}({}) ", asm_reg(reg)));
                self.expr(input);
                self.word(" => ");
                self.expr(output);
            }
            AsmOperand::Const(value) => {
                self.word("const ");
                self.expr(value);
            }
            AsmOperand::Sym(value) => {
                self.word("sym ");
                self.expr(value);
            }
            AsmOperand::Label { state, value } => {
                self.open_block();
                self.word("label {");
                self.p.hardbreak();
                self.assign_like(state, " = ".into(), value);
                self.p.hardbreak();
                self.close_block();
            }
        }
    }

    pub fn expr(&mut self, expr: &Expr) {
        self.expr_prec(expr, 0);
    }

    fn cond_expr(&mut self, expr: &Expr) {
        if expr_contains_bare_struct_lit(expr) {
            self.parenthesized(expr);
        } else {
            self.expr(expr);
        }
    }

    fn expr_prec(&mut self, expr: &Expr, min: u8) {
        if expr_prec(expr).level() < min {
            self.parenthesized(expr);
        } else {
            self.expr_raw(expr);
        }
    }

    fn parenthesized(&mut self, expr: &Expr) {
        self.word("(");
        self.expr_raw(expr);
        self.word(")");
    }

    fn type_args(&mut self, type_args: &[Type]) {
        self.word("::<");
        for (i, ty) in type_args.iter().enumerate() {
            if i > 0 {
                self.word(", ");
            }
            self.ty(ty);
        }
        self.word(">");
    }

    fn call_tail(&mut self, args: &[Expr]) {
        self.word("(");
        self.args(args);
        self.word(")");
    }

    fn method_chain(&mut self, expr: &Expr) {
        self.p.cbox(INDENT);
        self.chain_segment(expr);
        self.p.end();
    }

    fn chain_segment(&mut self, expr: &Expr) {
        let (recv, method, type_args, args) = match expr {
            Expr::MethodCall { recv, method, args } => (recv, method, None, args),
            Expr::MethodCallGeneric {
                recv,
                method,
                type_args,
                args,
            } => (recv, method, Some(type_args), args),
            _ => unreachable!(),
        };
        if matches!(
            **recv,
            Expr::MethodCall { .. } | Expr::MethodCallGeneric { .. }
        ) {
            self.chain_segment(recv);
        } else {
            self.expr_prec(recv, Precedence::Call.level());
        }
        self.p.zerobreak();
        self.word(".");
        self.ident(method);
        if let Some(type_args) = type_args {
            self.type_args(type_args);
        }
        self.p.cbox(0);
        self.call_tail(args);
        self.p.end();
    }

    fn binary_like(
        &mut self,
        lhs: impl FnOnce(&mut Self),
        op: String,
        rhs: impl FnOnce(&mut Self),
    ) {
        self.p.ibox(INDENT);
        self.p.ibox(-INDENT);
        lhs(self);
        self.p.end();
        self.p.space();
        self.word(op);
        rhs(self);
        self.p.end();
    }

    fn expr_raw(&mut self, expr: &Expr) {
        match expr {
            Expr::Value(v) => self.value(v),
            Expr::Str(s) => self.word(string_literal(s)),
            Expr::HexFloat(s) => self.word(s.to_string()),
            Expr::ByteStr(bytes) => self.word(byte_string_literal(bytes)),
            Expr::CStr(bytes) => self.word(c_string_literal(bytes)),
            Expr::Path(p) => self.path(p),
            Expr::GenericPath { path, type_args } => {
                self.path(path);
                self.type_args(type_args);
            }
            Expr::LayoutQuery {
                query: LayoutQuery::SizeOf(ty),
                ..
            } => {
                self.word("core::mem::size_of::<");
                self.ty(ty);
                self.word(">()");
            }
            Expr::LayoutQuery {
                query: LayoutQuery::AlignOf(ty),
                ..
            } => {
                self.word("core::mem::align_of::<");
                self.ty(ty);
                self.word(">()");
            }
            Expr::LayoutQuery {
                query: LayoutQuery::OffsetOf { ty, fields },
                ..
            } => {
                self.word("core::mem::offset_of!(");
                self.ty(ty);
                self.word(", ");
                let fields = fields
                    .iter()
                    .map(|field| self.ident_text(field))
                    .collect::<Vec<_>>()
                    .join(".");
                self.word(fields);
                self.word(")");
            }
            Expr::Var(s) => match macro_list_hole(expr) {
                Some(name) => self.word(format!("$(${name}),*")),
                None => self.ident(s.as_str()),
            },
            Expr::Unary { op, expr } => {
                self.word(op.spelling());
                self.prefix_operand(expr);
            }
            Expr::Binary { op, lhs, rhs } => {
                let p = op.precedence().level();
                let (lmin, rmin) = if op.is_comparison() {
                    (p + 1, p + 1)
                } else {
                    (p, p + 1)
                };
                self.binary_like(
                    |this| {
                        if starts_with_brace_expr(lhs) {
                            this.parenthesized(lhs);
                        } else {
                            this.expr_prec(lhs, lmin);
                        }
                    },
                    format!("{} ", op.spelling()),
                    |this| this.expr_prec(rhs, rmin),
                );
            }
            Expr::Range { start, end } => {
                self.range_endpoint(start);
                self.word("..");
                self.range_endpoint(end);
            }
            Expr::Call { func, args, .. } => {
                self.expr_prec(func, Precedence::Call.level());
                self.call_tail(args);
            }
            Expr::MethodCall { .. } | Expr::MethodCallGeneric { .. } => self.method_chain(expr),
            Expr::Field { base, field } => {
                self.expr_prec(base, Precedence::Call.level());
                self.word(".");
                self.ident(field);
            }
            Expr::TupleField { base, index } => {
                self.expr_prec(base, Precedence::Call.level());
                self.word(format!(".{index}"));
            }
            Expr::ArrayPtr { array, mutable } => {
                self.expr_prec(array, Precedence::Call.level());
                self.word(if *mutable {
                    ".as_mut_ptr()"
                } else {
                    ".as_ptr()"
                });
            }
            Expr::Index { base, index } => {
                self.expr_prec(base, Precedence::Call.level());
                self.word("[");
                self.expr(index);
                self.word("]");
            }
            Expr::StructLit { name, fields } => {
                self.p.cbox(INDENT);
                self.p.ibox(-INDENT);
                self.ident_path(name);
                self.p.end();
                self.word(" {");
                self.p.space_if_nonempty();
                for (i, (field, value)) in fields.iter().enumerate() {
                    self.ident(field);
                    self.word(": ");
                    self.expr(value);
                    self.p.trailing_comma_or_space(i + 1 == fields.len());
                }
                self.p.offset(-INDENT);
                self.p.end_with_max_width(34);
                self.word("}");
            }
            Expr::TupleStructLit { name, fields } => {
                self.ident_path(name);
                self.call_tail(fields);
            }
            Expr::ArrayLit(elems) => {
                self.word("[");
                self.args(elems);
                self.word("]");
            }
            Expr::ArrayRepeat { elem, len } => {
                self.word("[");
                self.expr(elem);
                self.word(format!("; {len}]"));
            }
            Expr::VecLit(elems) => {
                self.word("vec![");
                self.macro_args(elems);
                self.word("]");
            }
            Expr::VecRepeat { elem, len } => {
                self.word("vec![");
                self.expr(elem);
                self.word("; ");
                self.expr(len);
                self.word("]");
            }
            Expr::Macro { name, args } => {
                self.ident_path(name);
                self.word("!(");
                self.macro_args(args);
                self.word(")");
            }
            Expr::Closure { params, body } => {
                let params = params
                    .iter()
                    .map(|param| self.ident_text(param.as_str()))
                    .collect::<Vec<_>>()
                    .join(", ");
                self.word(format!("|{params}| "));
                self.expr(body);
            }
            Expr::Match { expr, arms } => {
                self.p.ibox(0);
                self.word("match ");
                self.cond_expr(expr);
                self.word(" {");
                self.p.neverbreak();
                self.p.cbox(INDENT);
                self.p.hardbreak_if_nonempty();
                for arm in arms {
                    self.p.ibox(0);
                    self.pattern(&arm.pattern);
                    self.word(" => ");
                    self.expr(&arm.value);
                    self.word(",");
                    self.p.end();
                    self.p.hardbreak();
                }
                self.p.offset(-INDENT);
                self.p.end();
                self.word("}");
                self.p.end();
            }
            Expr::If { .. } => self.expr_if(expr),
            Expr::Block(block) => self.expr_block(block),
            Expr::Unsafe(block) => {
                self.word("unsafe ");
                self.expr_block(block);
            }
            Expr::MacroName(name) => self.ident(name),
            Expr::MacroType(ty) => self.ty(ty),
            Expr::ConstBlock(expr) => {
                self.word("const ");
                self.small_block(|this| this.expr(expr));
            }
            Expr::Cast { expr, ty } => self.binary_like(
                |this| {
                    if starts_with_brace_expr(expr) {
                        this.parenthesized(expr);
                    } else {
                        this.expr_prec(expr, Precedence::CastOperand.level());
                    }
                },
                "as ".into(),
                |this| this.ty(ty),
            ),
            Expr::Ref { mutable, expr } => {
                self.word(if *mutable { "&mut " } else { "&" });
                self.prefix_operand(expr);
            }
            Expr::AddrOf { mutable, expr } => {
                self.word(if *mutable {
                    "core::ptr::addr_of_mut!("
                } else {
                    "core::ptr::addr_of!("
                });
                self.expr(expr);
                self.word(")");
            }
            Expr::AtomicRef { ty, place } => self.atomic_ref(*ty, place),
            Expr::AtomicLoad {
                ty,
                place,
                ordering,
            } => {
                self.atomic_ref(*ty, place);
                self.word(format!(".load({})", ordering_str(*ordering)));
            }
            Expr::AtomicStore {
                ty,
                place,
                value,
                ordering,
            } => {
                self.atomic_ref(*ty, place);
                self.word(".store(");
                self.pieces(&[
                    Piece::Expr(value),
                    Piece::Text(ordering_str(*ordering).into()),
                ]);
                self.word(")");
            }
            Expr::AtomicFetch {
                ty,
                op,
                place,
                value,
                ordering,
            } => {
                self.atomic_ref(*ty, place);
                self.word(format!(".{}(", rmw_method(*op)));
                self.pieces(&[
                    Piece::Expr(value),
                    Piece::Text(ordering_str(*ordering).into()),
                ]);
                self.word(")");
            }
            Expr::AtomicSwap {
                ty,
                place,
                value,
                ordering,
            } => {
                self.atomic_ref(*ty, place);
                self.word(".swap(");
                self.pieces(&[
                    Piece::Expr(value),
                    Piece::Text(ordering_str(*ordering).into()),
                ]);
                self.word(")");
            }
            Expr::AtomicCompareExchange {
                ty,
                place,
                expected,
                desired,
                success,
                failure,
            } => {
                self.atomic_ref(*ty, place);
                self.word(".compare_exchange(");
                self.pieces(&[
                    Piece::Expr(expected),
                    Piece::Expr(desired),
                    Piece::Text(ordering_str(*success).into()),
                    Piece::Text(ordering_str(*failure).into()),
                ]);
                self.word(")");
            }
            Expr::AtomicNew { ty, value } => {
                self.word(format!("core::sync::atomic::{}::new(", atomic_wrapper(*ty)));
                self.args(std::slice::from_ref(value));
                self.word(")");
            }
            Expr::AtomicFence { ordering } => self.word(format!(
                "core::sync::atomic::fence({})",
                ordering_str(*ordering)
            )),
            Expr::Transmute { from, to, expr } => {
                self.word("unsafe ");
                self.small_block(|this| {
                    this.word("core::mem::transmute::<");
                    this.ty(from);
                    this.word(", ");
                    this.ty(to);
                    this.word(">");
                    this.call_tail(std::slice::from_ref(expr));
                });
            }
            Expr::CopyNonoverlapping { src, dst, count } => {
                self.word("unsafe ");
                self.small_block(|this| {
                    this.word("core::ptr::copy_nonoverlapping(");
                    this.pieces(&[
                        Piece::Expr(src),
                        Piece::Expr(dst),
                        Piece::Text(count.to_string()),
                    ]);
                    this.word(")");
                });
            }
            Expr::PtrCopy {
                src,
                dst,
                count,
                overlapping,
            } => {
                let f = if *overlapping {
                    "copy"
                } else {
                    "copy_nonoverlapping"
                };
                self.word(format!("core::ptr::{f}("));
                self.pieces(&[Piece::Expr(src), Piece::Expr(dst), Piece::Expr(count)]);
                self.word(")");
            }
            Expr::WriteBytes { dst, val, count } => {
                self.word("core::ptr::write_bytes(");
                self.pieces(&[Piece::Expr(dst), Piece::Expr(val), Piece::Expr(count)]);
                self.word(")");
            }
            Expr::Todo(note) => self.word(format!("todo!({note:?})")),
        }
    }

    fn expr_if(&mut self, expr: &Expr) {
        self.p.cbox(INDENT);
        let mut current = expr;
        loop {
            let Expr::If {
                cond,
                then_expr,
                else_expr,
            } = current
            else {
                unreachable!()
            };
            self.word("if ");
            self.condition(cond);
            self.word(" ");
            self.branch_block(then_expr);
            self.word(" else ");
            if matches!(**else_expr, Expr::If { .. }) {
                current = else_expr;
                continue;
            }
            self.branch_block(else_expr);
            break;
        }
        self.p.end();
    }

    fn branch_block(&mut self, expr: &Expr) {
        self.word("{");
        self.p.space();
        self.p.ibox(0);
        self.expr(expr);
        self.p.end();
        self.p.space();
        self.p.offset(-INDENT);
        self.word("}");
    }

    fn range_endpoint(&mut self, expr: &Expr) {
        if matches!(expr, Expr::Binary { .. }) {
            self.parenthesized(expr);
        } else {
            self.expr_prec(expr, Precedence::Range.level() + 1);
        }
    }

    fn atomic_ref(&mut self, ty: AtomicType, place: &AtomicPlace) {
        match place {
            AtomicPlace::Ptr(ptr) => {
                self.word(format!(
                    "core::sync::atomic::{}::from_ptr(",
                    atomic_wrapper(ty)
                ));
                self.args(std::slice::from_ref(ptr));
                self.word(")");
            }
            AtomicPlace::Local(name) => self.ident(name.as_str()),
        }
    }

    fn prefix_operand(&mut self, expr: &Expr) {
        if matches!(expr, Expr::Unary { .. } | Expr::Ref { .. }) {
            self.parenthesized(expr);
        } else {
            self.expr_prec(expr, Precedence::Prefix.level());
        }
    }

    fn expr_block(&mut self, block: &Block) {
        if block.stmts.is_empty() {
            match &block.tail {
                Some(tail) => self.small_block(|this| this.expr(tail)),
                None => self.word("{}"),
            }
        } else {
            self.open_block();
            self.braced_block(block);
        }
    }

    fn args(&mut self, args: &[Expr]) {
        self.list(args, true);
    }

    fn macro_args(&mut self, args: &[Expr]) {
        self.list(args, false);
    }

    fn list(&mut self, args: &[Expr], trailing_comma: bool) {
        if args.len() > 1 && args.iter().all(is_simple_literal) {
            self.p.cbox(INDENT);
            self.p.zerobreak();
            self.p.ibox(0);
            for (i, arg) in args.iter().enumerate() {
                self.expr(arg);
                if i + 1 < args.len() {
                    self.word(",");
                    self.p.space();
                }
            }
            self.p.end();
            self.end_list(trailing_comma);
            return;
        }
        self.p.cbox(INDENT);
        self.p.zerobreak();
        for (i, arg) in args.iter().enumerate() {
            let last = i + 1 == args.len();
            match macro_list_hole(arg) {
                Some(name) if last && !trailing_comma => {
                    self.word(format!("$(${name}),*"));
                    self.p.zerobreak();
                }
                Some(name) => {
                    self.word(format!("$(${name},)*"));
                    if last {
                        self.p.zerobreak();
                    } else {
                        self.p.space();
                    }
                }
                None if last => {
                    self.expr(arg);
                    self.end_list(trailing_comma);
                    return;
                }
                None => {
                    self.expr(arg);
                    self.p.trailing_comma(false);
                }
            }
        }
        self.p.offset(-INDENT);
        self.p.end();
    }

    fn end_list(&mut self, trailing_comma: bool) {
        if trailing_comma {
            self.p.trailing_comma(true);
        } else {
            self.p.zerobreak();
        }
        self.p.offset(-INDENT);
        self.p.end();
    }

    fn value(&mut self, value: &RustValue) {
        let text = match value {
            RustValue::I64(n) => n.to_string(),
            RustValue::Usize(n) => format!("{n}usize"),
            RustValue::I128(n) => n.to_string(),
            RustValue::U128(n) => n.to_string(),
            RustValue::TypedInt(n, Prim::Named(named)) => format!("({n} as {})", named.name),
            RustValue::TypedUInt(n, Prim::Named(named)) => format!("({n} as {})", named.name),
            RustValue::TypedInt(n, ty) => format!("{n}{}", ty.spelling()),
            RustValue::TypedUInt(n, ty) => format!("{n}{}", ty.spelling()),
            RustValue::Float(n) if n.fract() == 0.0 => format!("{n:.1}"),
            RustValue::Float(n) => n.to_string(),
            RustValue::Bool(b) => b.to_string(),
            RustValue::None => "None".into(),
            RustValue::NullPtr => "core::ptr::null_mut()".into(),
        };
        self.word(text);
    }

    fn pattern(&mut self, pattern: &Pattern) {
        match pattern {
            Pattern::Wildcard => self.word("_"),
            Pattern::Binding(name) => self.ident(name.as_str()),
            Pattern::I64(n) => self.word(n.to_string()),
            Pattern::I128(n) => self.word(n.to_string()),
            Pattern::U128(n) => self.word(n.to_string()),
            Pattern::InclusiveRange { start, end } => self.word(format!("{start}..={end}")),
            Pattern::InclusiveRangeU128 { start, end } => self.word(format!("{start}..={end}")),
            Pattern::Guarded { bind, cond } => {
                self.ident(bind.as_str());
                self.word(" if ");
                self.expr(cond);
            }
            Pattern::Or(alts) => {
                self.p.ibox(0);
                for (i, alt) in alts.iter().enumerate() {
                    if i > 0 {
                        self.p.space();
                        self.word("| ");
                    }
                    self.pattern(alt);
                }
                self.p.end();
            }
            Pattern::TupleStruct { name, fields } => {
                self.ident_path(name.as_str());
                self.word("(");
                for (i, field) in fields.iter().enumerate() {
                    if i > 0 {
                        self.word(", ");
                    }
                    self.pattern(field);
                }
                self.word(")");
            }
        }
    }

    fn path(&mut self, path: &Path) {
        let text = path
            .segments
            .iter()
            .map(|segment| self.ident_text(segment.as_str()))
            .collect::<Vec<_>>()
            .join("::");
        self.word(text);
    }

    fn ty(&mut self, ty: &Type) {
        match ty {
            Type::Prim(p) => self.word(p.spelling()),
            Type::Custom(n) => self.ident_path(n),
            Type::String => self.word("String"),
            Type::LongDouble => self.word("LongDouble"),
            Type::TyVar(name) => self.ident(name.as_str()),
            Type::CLib(c) => self.word(c.path()),
            Type::Complex(inner) => {
                self.word("num_complex::Complex<");
                self.ty(inner);
                self.word(">");
            }
            Type::Generic { name, args } => {
                self.ident_path(name);
                self.word("<");
                for (i, arg) in args.iter().enumerate() {
                    if i > 0 {
                        self.word(", ");
                    }
                    self.ty(arg);
                }
                self.word(">");
            }
            Type::VaList => self.word("core::ffi::VaList<'static>"),
            Type::Str => self.word("str"),
            Type::Ref { mutable, inner } => {
                self.word(if *mutable { "&mut " } else { "&" });
                self.ty(inner);
            }
            Type::Slice(elem) => {
                self.word("[");
                self.ty(elem);
                self.word("]");
            }
            Type::Ptr { mutable, inner } => {
                self.word(if *mutable { "*mut " } else { "*const " });
                self.ty(inner);
            }
            Type::Array { elem, len } => {
                self.word("[");
                self.ty(elem);
                self.word(format!("; {len}]"));
            }
            Type::FnPtr { abi, params, ret } => {
                self.word("Option<unsafe ");
                if *abi != Abi::Rust {
                    self.abi(*abi);
                    self.word(" ");
                }
                self.word("fn(");
                for (i, p) in params.iter().enumerate() {
                    if i > 0 {
                        self.word(", ");
                    }
                    self.ty(p);
                }
                self.word(")");
                if !ret.is_unit() {
                    self.word(" -> ");
                    self.ty(ret);
                }
                self.word(">");
            }
            Type::Unit => self.word("()"),
            Type::Variadic => self.word("..."),
            Type::Never => self.word("!"),
        }
    }
}

fn asm_reg(reg: &AsmReg) -> String {
    match reg {
        AsmReg::Class(class) => class.to_string(),
        AsmReg::Explicit(reg) => format!("\"{reg}\""),
    }
}

fn record_fields_are_copy(fields: &[RecordField]) -> bool {
    fields.iter().all(|field| type_is_copy(&field.ty))
}

fn type_is_copy(ty: &Type) -> bool {
    match ty {
        Type::Custom(name) => name != "__SlateVaArgs",
        Type::VaList => false,
        Type::Array { elem, .. } => type_is_copy(elem),
        _ => true,
    }
}

#[derive(Clone, Copy, PartialEq, Eq)]
enum ItemGroup {
    Module,
    Use,
}

fn item_group(item: &Item) -> Option<ItemGroup> {
    match item {
        Item::Mod { .. } => Some(ItemGroup::Module),
        Item::Use { .. } => Some(ItemGroup::Use),
        Item::Cfg { item, .. } => item_group(item),
        _ => None,
    }
}

fn same_item_group(left: &Item, right: &Item) -> bool {
    item_group(left).is_some_and(|group| item_group(right) == Some(group))
}

fn render(margin: isize, f: impl FnOnce(&mut Codegen)) -> String {
    let mut cg = Codegen::new(margin);
    f(&mut cg);
    cg.finish()
}

/// Emits a complete [`Program`] as formatted Rust source.
pub fn program_to_string(program: &Program) -> String {
    render(MARGIN, |cg| cg.program(program))
}

/// Emits an [`Item`] as formatted Rust source.
pub fn item_to_string(item: &Item) -> String {
    render(MARGIN, |cg| cg.item(item))
}

/// Emits an [`Expr`] as a `String`.
pub fn expr_to_string(expr: &Expr) -> String {
    render(FLAT_MARGIN, |cg| cg.expr(expr))
}

/// Emits a [`Stmt`] as a `String`.
pub fn stmt_to_string(stmt: &Stmt) -> String {
    render(FLAT_MARGIN, |cg| cg.stmt(stmt))
}

/// Emits a [`Type`] as a `String`.
pub fn type_to_string(ty: &Type) -> String {
    render(FLAT_MARGIN, |cg| cg.ty(ty))
}

/// Emits a [`Cfg`] expression as a `String`.
pub fn cfg_to_string(cfg: &Cfg) -> String {
    render(FLAT_MARGIN, |cg| cg.cfg(cfg))
}

/// Escapes a C-derived name so it can be used as a Rust identifier.
///
/// Rust keywords receive an `r#` prefix, while boolean literals receive a
/// trailing underscore because raw identifiers cannot represent them.
pub fn escape_ident(name: &str) -> String {
    if name.starts_with("r#") || matches!(name, "crate" | "self" | "Self" | "super") {
        return name.to_string();
    }
    if matches!(name, "true" | "false") {
        return format!("{name}_");
    }
    if is_rust_keyword(name) {
        return format!("r#{name}");
    }
    name.to_string()
}

/// Returns whether `name` is reserved by Rust's grammar.
pub fn is_rust_keyword(name: &str) -> bool {
    matches!(
        name,
        "as" | "break"
            | "const"
            | "continue"
            | "crate"
            | "else"
            | "enum"
            | "extern"
            | "false"
            | "fn"
            | "for"
            | "if"
            | "impl"
            | "in"
            | "let"
            | "loop"
            | "match"
            | "mod"
            | "move"
            | "mut"
            | "pub"
            | "ref"
            | "return"
            | "self"
            | "Self"
            | "static"
            | "struct"
            | "super"
            | "trait"
            | "true"
            | "type"
            | "unsafe"
            | "use"
            | "where"
            | "while"
            | "async"
            | "await"
            | "dyn"
            | "abstract"
            | "become"
            | "box"
            | "do"
            | "final"
            | "macro"
            | "override"
            | "priv"
            | "typeof"
            | "unsized"
            | "virtual"
            | "yield"
            | "try"
            | "gen"
    )
}

fn byte_string_literal(bytes: &[u8]) -> String {
    let mut out = String::from("b\"");
    for b in bytes {
        match *b {
            b'\n' => out.push_str("\\n"),
            b'\r' => out.push_str("\\r"),
            b'\t' => out.push_str("\\t"),
            b'\\' => out.push_str("\\\\"),
            b'"' => out.push_str("\\\""),
            0 => out.push_str("\\0"),
            0x20..=0x7e => out.push(*b as char),
            _ => out.push_str(&format!("\\x{b:02x}")),
        }
    }
    out.push('"');
    out
}

fn c_string_literal(bytes: &[u8]) -> String {
    let mut out = String::from("c\"");
    for b in bytes {
        match *b {
            b'\n' => out.push_str("\\n"),
            b'\r' => out.push_str("\\r"),
            b'\t' => out.push_str("\\t"),
            b'\\' => out.push_str("\\\\"),
            b'"' => out.push_str("\\\""),
            0x20..=0x7e => out.push(*b as char),
            _ => out.push_str(&format!("\\x{b:02x}")),
        }
    }
    out.push('"');
    out
}

fn string_literal(s: &str) -> String {
    let mut out = String::from("\"");
    for ch in s.chars() {
        match ch {
            '\n' => out.push_str("\\n"),
            '\r' => out.push_str("\\r"),
            '\t' => out.push_str("\\t"),
            '\\' => out.push_str("\\\\"),
            '"' => out.push_str("\\\""),
            ch if ch.is_ascii_graphic() || ch == ' ' => out.push(ch),
            ch => out.push_str(&format!("\\u{{{:x}}}", ch as u32)),
        }
    }
    out.push('"');
    out
}
