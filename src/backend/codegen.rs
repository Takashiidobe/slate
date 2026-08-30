use std::fmt::{self, Write};

use crate::backend::rust_ast::{
    Abi, AsmDialect, AsmOperand, AsmReg, AtomicOrdering, AtomicPlace, AtomicRmwOp, AtomicType,
    Attr, Block, Cfg, Comment, CrateAttr, Derive, Expr, ExternDecl, FnDef, GenericParam, ImplBlock,
    ImplItem, IndentStmt, InlineHint, Item, Method, Path, Program, RecordDef, RecordField, Repr,
    RustValue, SelfKind, Stmt, StructDef, StructFields, TraitBound, TraitRef, Type,
};

const INDENT: &str = "    ";

const PREC_CAST: u8 = 4;
const PREC_RANGE: u8 = 2;
const PREC_CAST_OPERAND: u8 = 12;
const PREC_PREFIX: u8 = 13;
const PREC_CALL: u8 = 14;
const PREC_ATOM: u8 = 15;

fn expr_prec(expr: &Expr) -> u8 {
    match expr {
        Expr::Range { .. } => PREC_RANGE,
        Expr::Binary { op, .. } => op.precedence(),
        Expr::Cast { .. } => PREC_CAST,
        Expr::Unary { .. } | Expr::Ref { .. } => PREC_PREFIX,
        Expr::Call { .. }
        | Expr::MethodCall { .. }
        | Expr::MethodCallGeneric { .. }
        | Expr::Field { .. }
        | Expr::TupleField { .. }
        | Expr::ArrayPtr { .. }
        | Expr::Index { .. } => PREC_CALL,
        _ => PREC_ATOM,
    }
}

fn starts_with_brace_expr(expr: &Expr) -> bool {
    matches!(
        expr,
        Expr::Block(_) | Expr::Unsafe(_) | Expr::If { .. } | Expr::Match { .. }
    )
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

    fn ident(&mut self, name: &str) -> fmt::Result {
        self.out.write_str(&escape_ident(name))
    }

    fn ident_path(&mut self, path: &str) -> fmt::Result {
        for (index, segment) in path.split("::").enumerate() {
            if index > 0 {
                self.out.write_str("::")?;
            }
            self.ident(segment)?;
        }
        Ok(())
    }

    pub fn program(&mut self, program: &Program) -> fmt::Result {
        for (i, item) in program.items.iter().enumerate() {
            if i > 0
                && !matches!(program.items[i - 1], Item::Comment(_))
                && !same_item_group(&program.items[i - 1], item)
            {
                self.out.write_char('\n')?;
            }
            self.item(item)?;
        }
        Ok(())
    }

    fn item(&mut self, item: &Item) -> fmt::Result {
        match item {
            Item::Fn(f) => self.fn_def(f)?,
            Item::Comment(comment) => self.comment(comment, 0)?,
            Item::CrateAttrs(attrs) => {
                for attr in attrs {
                    self.out.write_str("#![")?;
                    self.crate_attr(attr)?;
                    self.out.write_str("]\n")?;
                }
            }
            Item::Mod { name } => {
                self.out.write_str("mod ")?;
                self.ident(name.as_str())?;
                self.out.write_str(";\n")?;
            }
            Item::InlineMod { vis, name, items } => {
                if let Some(kw) = vis.keyword() {
                    write!(self.out, "{kw} ")?;
                }
                self.out.write_str("mod ")?;
                self.ident(name.as_str())?;
                self.out.write_str(" {\n")?;
                let mut nested = Codegen::new(String::new());
                for item in items {
                    nested.item(item)?;
                }
                for line in nested.into_inner().lines() {
                    if !line.is_empty() {
                        self.out.write_str(INDENT)?;
                        self.out.write_str(line)?;
                    }
                    self.out.write_char('\n')?;
                }
                self.out.write_str("}\n")?;
            }
            Item::Use { path } => {
                self.out.write_str("use ")?;
                self.path(path)?;
                self.out.write_str(";\n")?;
            }
            Item::Static {
                attrs,
                vis,
                mutable,
                name,
                ty,
                init,
            } => {
                self.attrs(attrs)?;
                if let Some(kw) = vis.keyword() {
                    write!(self.out, "{kw} ")?;
                }
                self.out.write_str("static ")?;
                if *mutable {
                    self.out.write_str("mut ")?;
                }
                self.ident(name)?;
                self.out.write_str(": ")?;
                self.ty(ty)?;
                self.out.write_str(" = ")?;
                self.expr(init)?;
                self.out.write_str(";\n")?;
            }
            Item::Const {
                attrs,
                name,
                ty,
                init,
            } => {
                self.attrs(attrs)?;
                self.out.write_str("const ")?;
                self.ident(name)?;
                self.out.write_str(": ")?;
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
            Item::Enum(e) => self.enum_def(e)?,
            Item::Record(r) => self.record(r)?,
            Item::Struct(s) => self.struct_def(s)?,
            Item::Impl(im) => self.impl_block(im)?,
            Item::Macro { name, args } => {
                self.ident_path(name)?;
                self.out.write_str("!(")?;
                self.args(args)?;
                self.out.write_str(");\n")?;
            }
            Item::Cfg { cfg, item } => {
                self.out.write_str("#[cfg(")?;
                self.cfg(cfg)?;
                self.out.write_str(")]\n")?;
                self.item(item)?;
            }
            Item::SupportModule(module) => writeln!(self.out, "{}", module.source)?,
        }
        Ok(())
    }

    fn comment(&mut self, comment: &Comment, depth: usize) -> fmt::Result {
        for line in &comment.lines {
            for _ in 0..depth {
                self.out.write_str(INDENT)?;
            }
            if line.is_empty() {
                self.out.write_str("///\n")?;
            } else {
                writeln!(self.out, "/// {line}")?;
            }
        }
        Ok(())
    }

    fn cfg(&mut self, cfg: &Cfg) -> fmt::Result {
        match cfg {
            Cfg::Flag(name) => self.out.write_str(name),
            Cfg::Opt { key, value } => write!(self.out, "{key} = \"{value}\""),
            Cfg::Not(inner) => {
                self.out.write_str("not(")?;
                self.cfg(inner)?;
                self.out.write_str(")")
            }
            Cfg::Any(items) => self.cfg_list("any", items),
            Cfg::All(items) => self.cfg_list("all", items),
        }
    }

    fn cfg_list(&mut self, keyword: &str, items: &[Cfg]) -> fmt::Result {
        write!(self.out, "{keyword}(")?;
        for (i, item) in items.iter().enumerate() {
            if i > 0 {
                self.out.write_str(", ")?;
            }
            self.cfg(item)?;
        }
        self.out.write_str(")")
    }

    fn fn_def(&mut self, f: &FnDef) -> fmt::Result {
        self.attrs(&f.attrs)?;
        if let Some(kw) = f.vis.keyword() {
            write!(self.out, "{kw} ")?;
        }
        if f.unsafe_ {
            self.out.write_str("unsafe ")?;
        }
        if let Some(abi) = f.abi {
            self.abi(abi)?;
            self.out.write_char(' ')?;
        }
        self.out.write_str("fn ")?;
        self.ident(&f.name)?;
        self.out.write_char('(')?;
        for (i, p) in f.params.iter().enumerate() {
            if i > 0 {
                self.out.write_str(", ")?;
            }
            if p.mutable {
                self.out.write_str("mut ")?;
            }
            self.ident(&p.name)?;
            self.out.write_str(": ")?;
            self.ty(&p.ty)?;
        }
        self.out.write_char(')')?;
        if let Some(ret) = &f.ret
            && !ret.is_unit()
        {
            self.out.write_str(" -> ")?;
            self.ty(ret)?;
        }
        self.out.write_str(" {\n")?;
        for (index, IndentStmt { depth, stmt }) in f.body.iter().enumerate() {
            if f.ret.is_some()
                && index + 1 == f.body.len()
                && let Stmt::Expr(expr) = stmt
            {
                self.out.write_str(&INDENT.repeat(*depth))?;
                self.expr(expr)?;
                self.out.write_char('\n')?;
            } else {
                self.stmt(stmt, *depth)?;
            }
        }
        self.out.write_str("}\n")
    }

    fn record(&mut self, r: &RecordDef) -> fmt::Result {
        for comment in &r.comments {
            self.comment(comment, 0)?;
        }
        let mut repr = vec![Repr::C];
        if let Some(n) = r.packed {
            repr.push(Repr::Packed(n));
        }
        if let Some(n) = r.align {
            repr.push(Repr::Align(n));
        }
        self.attrs(&[Attr::Repr(repr)])?;
        if r.allow_non_camel_case {
            self.out.write_str("#[expect(non_camel_case_types)]\n")?;
        }
        if record_fields_are_copy(&r.fields) {
            self.out.write_str("#[derive(Clone, Copy)]\n")?;
        } else {
            self.out.write_str("#[derive(Clone)]\n")?;
        }
        let kw = if r.is_union { "union" } else { "struct" };
        if let Some(vis) = r.vis.keyword() {
            write!(self.out, "{vis} ")?;
        }
        write!(self.out, "{kw} ")?;
        self.ident(&r.name)?;
        self.out.write_str(" {\n")?;
        for field in &r.fields {
            for comment in &field.comments {
                self.comment(comment, 1)?;
            }
            self.out.write_str("    ")?;
            if let Some(vis) = r.field_vis.keyword() {
                write!(self.out, "{vis} ")?;
            }
            self.ident(field.name.as_str())?;
            self.out.write_str(": ")?;
            self.ty(&field.ty)?;
            self.out.write_str(",\n")?;
        }
        self.out.write_str("}\n\n")
    }

    fn enum_def(&mut self, e: &crate::backend::rust_ast::EnumDef) -> fmt::Result {
        for comment in &e.comments {
            self.comment(comment, 0)?;
        }
        self.attrs(&e.attrs)?;
        if let Some(vis) = e.vis.keyword() {
            write!(self.out, "{vis} ")?;
        }
        self.out.write_str("enum ")?;
        self.ident(&e.name)?;
        self.out.write_str(" {\n")?;
        for variant in &e.variants {
            for comment in &variant.comments {
                self.comment(comment, 1)?;
            }
            self.out.write_str("    ")?;
            self.ident(&variant.name)?;
            writeln!(self.out, " = {},", variant.value)?;
        }
        self.out.write_str("}\n\n")
    }

    fn struct_def(&mut self, s: &StructDef) -> fmt::Result {
        self.attrs(&s.attrs)?;
        if let Some(vis) = s.vis.keyword() {
            write!(self.out, "{vis} ")?;
        }
        self.out.write_str("struct ")?;
        self.ident(&s.name)?;
        self.generics(&s.generics)?;
        match &s.fields {
            StructFields::Tuple(tys) => {
                self.out.write_char('(')?;
                for (i, ty) in tys.iter().enumerate() {
                    if i > 0 {
                        self.out.write_str(", ")?;
                    }
                    if let Some(vis) = s.field_vis.keyword() {
                        write!(self.out, "{vis} ")?;
                    }
                    self.ty(ty)?;
                }
                self.out.write_str(");\n")
            }
            StructFields::Named(fields) => {
                self.out.write_str(" {\n")?;
                for field in fields {
                    for attr in &field.attrs {
                        self.out.write_str(INDENT)?;
                        self.out.write_str("#[")?;
                        self.attr(attr)?;
                        self.out.write_str("]\n")?;
                    }
                    self.out.write_str(INDENT)?;
                    if let Some(vis) = s.field_vis.keyword() {
                        write!(self.out, "{vis} ")?;
                    }
                    self.ident(&field.name)?;
                    self.out.write_str(": ")?;
                    self.ty(&field.ty)?;
                    self.out.write_str(",\n")?;
                }
                self.out.write_str("}\n")
            }
        }
    }

    fn attrs(&mut self, attrs: &[Attr]) -> fmt::Result {
        for attr in attrs {
            self.out.write_str("#[")?;
            self.attr(attr)?;
            self.out.write_str("]\n")?;
        }
        Ok(())
    }

    fn crate_attr(&mut self, attr: &CrateAttr) -> fmt::Result {
        let (kind, lints) = match attr {
            CrateAttr::Allow(lints) => ("allow", lints),
            CrateAttr::Deny(lints) => ("deny", lints),
            CrateAttr::Feature(feature) => {
                return write!(self.out, "feature({})", feature.spelling());
            }
        };
        write!(self.out, "{kind}(")?;
        for (i, lint) in lints.iter().enumerate() {
            if i > 0 {
                self.out.write_str(", ")?;
            }
            self.out.write_str(lint.spelling())?;
        }
        self.out.write_char(')')
    }

    fn attr(&mut self, attr: &Attr) -> fmt::Result {
        match attr {
            Attr::Call { path, args } => {
                self.path(path)?;
                self.out.write_char('(')?;
                for (index, arg) in args.iter().enumerate() {
                    if index > 0 {
                        self.out.write_str(", ")?;
                    }
                    self.attr_arg(arg)?;
                }
                self.out.write_char(')')
            }
            Attr::Allow(items) => {
                self.out.write_str("allow(")?;
                for (i, item) in items.iter().enumerate() {
                    if i > 0 {
                        self.out.write_str(", ")?;
                    }
                    self.out.write_str(item.spelling())?;
                }
                self.out.write_char(')')
            }
            Attr::Repr(items) => {
                self.out.write_str("repr(")?;
                for (i, r) in items.iter().enumerate() {
                    if i > 0 {
                        self.out.write_str(", ")?;
                    }
                    match r {
                        Repr::C => self.out.write_str("C")?,
                        Repr::Align(n) => write!(self.out, "align({n})")?,
                        Repr::Packed(1) => self.out.write_str("packed")?,
                        Repr::Packed(n) => write!(self.out, "packed({n})")?,
                    }
                }
                self.out.write_char(')')
            }
            Attr::Derive(items) => {
                self.out.write_str("derive(")?;
                for (i, d) in items.iter().enumerate() {
                    if i > 0 {
                        self.out.write_str(", ")?;
                    }
                    self.out.write_str(match d {
                        Derive::Clone => "Clone",
                        Derive::Copy => "Copy",
                        Derive::PartialEq => "PartialEq",
                        Derive::PartialOrd => "PartialOrd",
                        Derive::Eq => "Eq",
                        Derive::Debug => "Debug",
                        Derive::Hash => "Hash",
                    })?;
                }
                self.out.write_char(')')
            }
            Attr::NoMangle => self.out.write_str("unsafe(no_mangle)"),
            Attr::Naked => self.out.write_str("unsafe(naked)"),
            Attr::ExportName(name) => {
                write!(self.out, "unsafe(export_name = {})", string_literal(name))
            }
            Attr::LinkName(name) => write!(self.out, "link_name = {}", string_literal(name)),
            Attr::ThreadLocal => self.out.write_str("thread_local"),
            Attr::WeakLinkage => self.out.write_str("linkage = \"weak\""),
            Attr::ExternWeakLinkage => self.out.write_str("linkage = \"extern_weak\""),
            Attr::LinkSection(section) => {
                write!(
                    self.out,
                    "unsafe(link_section = {})",
                    string_literal(section)
                )
            }
            Attr::Used(kind) => match kind {
                crate::backend::rust_ast::UsedKind::Plain => self.out.write_str("used"),
                crate::backend::rust_ast::UsedKind::Compiler => {
                    self.out.write_str("used(compiler)")
                }
                crate::backend::rust_ast::UsedKind::Linker => self.out.write_str("used(linker)"),
            },
            Attr::Deprecated(None) => self.out.write_str("deprecated"),
            Attr::Deprecated(Some(note)) => {
                write!(self.out, "deprecated(note = {})", string_literal(note))
            }
            Attr::TargetFeature(features) => {
                write!(
                    self.out,
                    "target_feature(enable = {})",
                    string_literal(features)
                )
            }
            Attr::Cold => self.out.write_str("cold"),
            Attr::MustUse => self.out.write_str("must_use"),
            Attr::Inline(InlineHint::Always) => self.out.write_str("inline(always)"),
            Attr::Inline(InlineHint::Never) => self.out.write_str("inline(never)"),
        }
    }

    fn attr_arg(&mut self, arg: &crate::backend::rust_ast::AttrArg) -> fmt::Result {
        use crate::backend::rust_ast::AttrArg;
        match arg {
            AttrArg::Type(ty) => self.ty(ty),
            AttrArg::UInt(value) => write!(self.out, "{value}"),
            AttrArg::Bool(value) => write!(self.out, "{value}"),
            AttrArg::Named(name, value) => {
                self.ident(name)?;
                self.out.write_str(" = ")?;
                self.attr_arg(value)
            }
        }
    }

    fn abi(&mut self, abi: Abi) -> fmt::Result {
        write!(self.out, "extern \"{}\"", abi.spelling())
    }

    fn generics(&mut self, generics: &[GenericParam]) -> fmt::Result {
        if generics.is_empty() {
            return Ok(());
        }
        self.out.write_char('<')?;
        for (i, g) in generics.iter().enumerate() {
            if i > 0 {
                self.out.write_str(", ")?;
            }
            self.ident(&g.name)?;
            for (j, bound) in g.bounds.iter().enumerate() {
                self.out.write_str(if j == 0 { ": " } else { " + " })?;
                self.trait_bound(bound)?;
            }
        }
        self.out.write_char('>')
    }

    fn trait_bound(&mut self, bound: &TraitBound) -> fmt::Result {
        self.out.write_str(bound.trait_.path())?;
        if !bound.assoc.is_empty() {
            self.out.write_char('<')?;
            for (i, (name, ty)) in bound.assoc.iter().enumerate() {
                if i > 0 {
                    self.out.write_str(", ")?;
                }
                self.ident(name)?;
                self.out.write_str(" = ")?;
                self.ty(ty)?;
            }
            self.out.write_char('>')?;
        }
        Ok(())
    }

    fn impl_block(&mut self, im: &ImplBlock) -> fmt::Result {
        self.out.write_str("impl")?;
        self.generics(&im.generics)?;
        self.out.write_char(' ')?;
        match &im.trait_ {
            Some(TraitRef::Std(tr)) => write!(self.out, "{} for ", tr.path())?,
            Some(TraitRef::From(from_ty)) => {
                self.out.write_str("From<")?;
                self.ty(from_ty)?;
                self.out.write_str("> for ")?;
            }
            None => {}
        }
        self.ty(&im.self_ty)?;
        self.out.write_str(" {\n")?;
        for item in &im.items {
            match item {
                ImplItem::AssocType { name, ty } => {
                    write!(self.out, "{INDENT}type ")?;
                    self.ident(name)?;
                    self.out.write_str(" = ")?;
                    self.ty(ty)?;
                    self.out.write_str(";\n")?;
                }
                ImplItem::Method(m) => self.method(m)?,
            }
        }
        self.out.write_str("}\n")
    }

    fn method(&mut self, m: &Method) -> fmt::Result {
        write!(self.out, "{INDENT}fn ")?;
        self.ident(&m.name)?;
        self.out.write_char('(')?;
        let mut first = true;
        let self_kw = match m.self_kind {
            SelfKind::None => None,
            SelfKind::Value => Some("self"),
            SelfKind::Ref => Some("&self"),
            SelfKind::RefMut => Some("&mut self"),
        };
        if let Some(kw) = self_kw {
            self.out.write_str(kw)?;
            first = false;
        }
        for p in &m.params {
            if !first {
                self.out.write_str(", ")?;
            }
            first = false;
            self.ident(&p.name)?;
            self.out.write_str(": ")?;
            self.ty(&p.ty)?;
        }
        self.out.write_char(')')?;
        if let Some(ret) = &m.ret
            && !ret.is_unit()
        {
            self.out.write_str(" -> ")?;
            self.ty(ret)?;
        }
        self.out.write_str(" { ")?;
        self.expr(&m.body)?;
        self.out.write_str(" }\n")
    }

    fn extern_decl(&mut self, decl: &ExternDecl) -> fmt::Result {
        match decl {
            ExternDecl::Fn(f) => {
                for attr in &f.attrs {
                    self.out.write_str("#[")?;
                    self.attr(attr)?;
                    self.out.write_str("]\n")?;
                    self.out.write_str(INDENT)?;
                }
                if f.safe {
                    self.out.write_str("safe ")?;
                }
                self.out.write_str("fn ")?;
                self.ident(&f.name)?;
                self.out.write_char('(')?;
                for (i, p) in f.params.iter().enumerate() {
                    if i > 0 {
                        self.out.write_str(", ")?;
                    }
                    self.ident(&p.name)?;
                    self.out.write_str(": ")?;
                    self.ty(&p.ty)?;
                }
                if f.variadic {
                    if !f.params.is_empty() {
                        self.out.write_str(", ")?;
                    }
                    self.out.write_str("...")?;
                }
                self.out.write_char(')')?;
                if let Some(ret) = &f.ret
                    && !ret.is_unit()
                {
                    self.out.write_str(" -> ")?;
                    self.ty(ret)?;
                }
                self.out.write_str(";\n")
            }
            ExternDecl::Static {
                attrs,
                mutable,
                name,
                ty,
            } => {
                for attr in attrs {
                    self.out.write_str("#[")?;
                    self.attr(attr)?;
                    self.out.write_str("]\n")?;
                    self.out.write_str(INDENT)?;
                }
                self.out.write_str("static ")?;
                if *mutable {
                    self.out.write_str("mut ")?;
                }
                self.ident(name)?;
                self.out.write_str(": ")?;
                self.ty(ty)?;
                self.out.write_str(";\n")
            }
        }
    }

    fn block(&mut self, block: &Block, depth: usize) -> fmt::Result {
        self.indent_stmts(&block.stmts, depth)?;
        if let Some(tail) = &block.tail {
            self.out.write_str(&INDENT.repeat(depth))?;
            self.expr(tail)?;
            self.out.write_char('\n')?;
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
                self.ident(name)?;
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
                self.ident(name)?;
                if let Some(ty) = ty {
                    self.out.write_str(": ")?;
                    self.ty(ty)?;
                }
                self.out.write_str(" = if ")?;
                self.expr(cond)?;
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
            Stmt::CompoundAssign { target, op, value } => {
                self.out.write_str(&pad)?;
                self.expr(target)?;
                write!(self.out, " {}= ", op.spelling())?;
                self.expr(value)?;
                self.out.write_str(";\n")
            }
            Stmt::InlineAsm(asm) => {
                self.out.write_str(&pad)?;
                self.out.write_str("core::arch::asm!(")?;
                self.expr(&Expr::Str(asm.template.clone()))?;
                for operand in &asm.operands {
                    self.out.write_str(", ")?;
                    match operand {
                        AsmOperand::In { reg, value } => {
                            self.out.write_str("in(")?;
                            self.asm_reg(reg)?;
                            self.out.write_str(") ")?;
                            self.expr(value)?;
                        }
                        AsmOperand::Out { reg, late, value } => {
                            if *late {
                                self.out.write_str("lateout(")?;
                            } else {
                                self.out.write_str("out(")?;
                            }
                            self.asm_reg(reg)?;
                            self.out.write_str(") ")?;
                            self.expr(value)?;
                        }
                        AsmOperand::InOut {
                            reg,
                            late,
                            input,
                            output,
                        } => {
                            if *late {
                                self.out.write_str("inlateout(")?;
                            } else {
                                self.out.write_str("inout(")?;
                            }
                            self.asm_reg(reg)?;
                            self.out.write_str(") ")?;
                            self.expr(input)?;
                            self.out.write_str(" => ")?;
                            self.expr(output)?;
                        }
                        AsmOperand::Const(value) => {
                            self.out.write_str("const ")?;
                            self.expr(value)?;
                        }
                        AsmOperand::Label {
                            state,
                            value,
                            destination,
                        } => {
                            self.out.write_str("label {\n")?;
                            write!(self.out, "{pad}{INDENT}")?;
                            self.expr(state)?;
                            self.out.write_str(" = ")?;
                            self.expr(value)?;
                            self.out.write_str(";\n")?;
                            writeln!(self.out, "{pad}{INDENT}continue '{};", destination.as_str())?;
                            self.out.write_str(&pad)?;
                            self.out.write_char('}')?;
                        }
                    }
                }
                let mut options = Vec::new();
                if matches!(asm.dialect, Some(AsmDialect::Att)) {
                    options.push("att_syntax");
                }
                if asm.raw {
                    options.push("raw");
                }
                if !options.is_empty() {
                    self.out.write_str(", options(")?;
                    self.out.write_str(&options.join(", "))?;
                    self.out.write_char(')')?;
                }
                self.out.write_str(");\n")
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
                self.block(body, depth + 1)?;
                writeln!(self.out, "{pad}}}")
            }
            Stmt::If {
                cond,
                then_body,
                else_body,
            } => {
                write!(self.out, "{pad}if ")?;
                self.expr(cond)?;
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
                    Some(label) => writeln!(self.out, "{pad}'{}: loop {{", label.as_str())?,
                    None => writeln!(self.out, "{pad}loop {{")?,
                }
                self.indent_stmts(body, depth + 1)?;
                writeln!(self.out, "{pad}}}")
            }
            Stmt::For { pat, iter, body } => {
                write!(self.out, "{pad}for {pat} in ")?;
                self.expr(iter)?;
                self.out.write_str(" {\n")?;
                self.indent_stmts(body, depth + 1)?;
                writeln!(self.out, "{pad}}}")
            }
            Stmt::Scope { body } => {
                writeln!(self.out, "{pad}{{")?;
                self.indent_stmts(body, depth + 1)?;
                writeln!(self.out, "{pad}}}")
            }
            Stmt::LabeledBlock { label, body } => {
                writeln!(self.out, "{pad}'{}: {{", label.as_str())?;
                self.indent_stmts(body, depth + 1)?;
                writeln!(self.out, "{pad}}}")
            }
            Stmt::Match { expr, arms } => {
                write!(self.out, "{pad}match ")?;
                self.expr(expr)?;
                self.out.write_str(" {\n")?;
                for arm in arms {
                    write!(self.out, "{pad}{INDENT}")?;
                    self.pattern(&arm.pattern)?;
                    self.out.write_str(" => {\n")?;
                    self.indent_stmts(&arm.body, depth + 2)?;
                    writeln!(self.out, "{pad}{INDENT}}}")?;
                }
                writeln!(self.out, "{pad}}}")
            }
            Stmt::Break(label) => match label {
                Some(label) => writeln!(self.out, "{pad}break '{};", label.as_str()),
                None => writeln!(self.out, "{pad}break;"),
            },
            Stmt::Continue(label) => match label {
                Some(label) => writeln!(self.out, "{pad}continue '{};", label.as_str()),
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
        }
    }

    fn asm_reg(&mut self, reg: &AsmReg) -> fmt::Result {
        match reg {
            AsmReg::Class(class) => self.out.write_str(class),
            AsmReg::Explicit(reg) => {
                self.out.write_char('"')?;
                self.out.write_str(reg)?;
                self.out.write_char('"')
            }
        }
    }

    pub fn expr(&mut self, expr: &Expr) -> fmt::Result {
        self.expr_prec(expr, 0)
    }

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
            Expr::Str(s) => self.out.write_str(&string_literal(s)),
            Expr::HexFloat(s) => self.out.write_str(s),
            Expr::ByteStr(bytes) => self.out.write_str(&byte_string_literal(bytes)),
            Expr::CStr(bytes) => self.out.write_str(&c_string_literal(bytes)),
            Expr::Path(p) => self.path(p),
            Expr::Var(s) => self.ident(s.as_str()),
            Expr::Unary { op, expr } => {
                self.out.write_str(op.spelling())?;
                self.prefix_operand(expr)
            }
            Expr::Binary { op, lhs, rhs } => {
                let p = op.precedence();
                let (lmin, rmin) = if op.is_comparison() {
                    (p + 1, p + 1)
                } else {
                    (p, p + 1)
                };
                if starts_with_brace_expr(lhs) {
                    self.parenthesized(lhs)?;
                } else {
                    self.expr_prec(lhs, lmin)?;
                }
                write!(self.out, " {} ", op.spelling())?;
                self.expr_prec(rhs, rmin)
            }
            Expr::Range { start, end } => {
                self.range_endpoint(start)?;
                self.out.write_str("..")?;
                self.range_endpoint(end)
            }
            Expr::Call { func, args, .. } => {
                self.expr_prec(func, PREC_CALL)?;
                self.out.write_char('(')?;
                self.args(args)?;
                self.out.write_char(')')
            }
            Expr::MethodCall { recv, method, args } => {
                self.expr_prec(recv, PREC_CALL)?;
                self.out.write_char('.')?;
                self.ident(method)?;
                self.out.write_char('(')?;
                self.args(args)?;
                self.out.write_char(')')
            }
            Expr::MethodCallGeneric {
                recv,
                method,
                type_args,
                args,
            } => {
                self.expr_prec(recv, PREC_CALL)?;
                self.out.write_char('.')?;
                self.ident(method)?;
                self.out.write_str("::<")?;
                for (i, ty) in type_args.iter().enumerate() {
                    if i > 0 {
                        self.out.write_str(", ")?;
                    }
                    self.ty(ty)?;
                }
                self.out.write_str(">(")?;
                self.args(args)?;
                self.out.write_char(')')
            }
            Expr::Field { base, field } => {
                self.expr_prec(base, PREC_CALL)?;
                self.out.write_char('.')?;
                self.ident(field)
            }
            Expr::TupleField { base, index } => {
                self.expr_prec(base, PREC_CALL)?;
                write!(self.out, ".{index}")
            }
            Expr::ArrayPtr { array, mutable } => {
                self.expr_prec(array, PREC_CALL)?;
                self.out.write_str(if *mutable {
                    ".as_mut_ptr()"
                } else {
                    ".as_ptr()"
                })
            }
            Expr::Index { base, index } => {
                self.expr_prec(base, PREC_CALL)?;
                self.out.write_char('[')?;
                self.expr_prec(index, PREC_CALL)?;
                self.out.write_char(']')
            }
            Expr::StructLit { name, fields } => {
                self.ident_path(name)?;
                self.out.write_str(" { ")?;
                for (i, (field, value)) in fields.iter().enumerate() {
                    if i > 0 {
                        self.out.write_str(", ")?;
                    }
                    self.ident(field)?;
                    self.out.write_str(": ")?;
                    self.expr(value)?;
                }
                self.out.write_str(" }")
            }
            Expr::TupleStructLit { name, fields } => {
                self.ident_path(name)?;
                self.out.write_char('(')?;
                self.args(fields)?;
                self.out.write_char(')')
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
            Expr::VecLit(elems) => {
                self.out.write_str("vec![")?;
                self.args(elems)?;
                self.out.write_char(']')
            }
            Expr::VecRepeat { elem, len } => {
                self.out.write_str("vec![")?;
                self.expr(elem)?;
                self.out.write_str("; ")?;
                self.expr(len)?;
                self.out.write_char(']')
            }
            Expr::Macro { name, args } => {
                self.ident_path(name)?;
                self.out.write_str("!(")?;
                self.args(args)?;
                self.out.write_char(')')
            }
            Expr::Closure { params, body } => {
                self.out.write_char('|')?;
                for (i, param) in params.iter().enumerate() {
                    if i > 0 {
                        self.out.write_str(", ")?;
                    }
                    self.ident(param.as_str())?;
                }
                self.out.write_str("| ")?;
                self.expr(body)
            }
            Expr::Match { expr, arms } => {
                self.out.write_str("match ")?;
                self.expr(expr)?;
                self.out.write_str(" { ")?;
                for (i, arm) in arms.iter().enumerate() {
                    if i > 0 {
                        self.out.write_str(", ")?;
                    }
                    self.pattern(&arm.pattern)?;
                    self.out.write_str(" => ")?;
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
                self.expr(cond)?;
                self.out.write_str(" { ")?;
                self.expr(then_expr)?;
                self.out.write_str(" } else { ")?;
                self.expr(else_expr)?;
                self.out.write_str(" }")
            }
            Expr::Block(block) => self.expr_block(block),
            Expr::Unsafe(block) => {
                self.out.write_str("unsafe ")?;
                self.expr_block(block)
            }
            Expr::Cast { expr, ty } => {
                if starts_with_brace_expr(expr) {
                    self.parenthesized(expr)?;
                } else {
                    self.expr_prec(expr, PREC_CAST_OPERAND)?;
                }
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
            Expr::AtomicRef { ty, place } => self.atomic_ref(*ty, place),
            Expr::AtomicLoad {
                ty,
                place,
                ordering,
            } => {
                self.atomic_ref(*ty, place)?;
                write!(self.out, ".load({})", ordering_str(*ordering))
            }
            Expr::AtomicStore {
                ty,
                place,
                value,
                ordering,
            } => {
                self.atomic_ref(*ty, place)?;
                self.out.write_str(".store(")?;
                self.expr(value)?;
                write!(self.out, ", {})", ordering_str(*ordering))
            }
            Expr::AtomicFetch {
                ty,
                op,
                place,
                value,
                ordering,
            } => {
                self.atomic_ref(*ty, place)?;
                write!(self.out, ".{}(", rmw_method(*op))?;
                self.expr(value)?;
                write!(self.out, ", {})", ordering_str(*ordering))
            }
            Expr::AtomicSwap {
                ty,
                place,
                value,
                ordering,
            } => {
                self.atomic_ref(*ty, place)?;
                self.out.write_str(".swap(")?;
                self.expr(value)?;
                write!(self.out, ", {})", ordering_str(*ordering))
            }
            Expr::AtomicCompareExchange {
                ty,
                place,
                expected,
                desired,
                success,
                failure,
            } => {
                self.atomic_ref(*ty, place)?;
                self.out.write_str(".compare_exchange(")?;
                self.expr(expected)?;
                self.out.write_str(", ")?;
                self.expr(desired)?;
                write!(
                    self.out,
                    ", {}, {})",
                    ordering_str(*success),
                    ordering_str(*failure)
                )
            }
            Expr::AtomicNew { ty, value } => {
                write!(self.out, "std::sync::atomic::{}::new(", atomic_wrapper(*ty))?;
                self.expr(value)?;
                self.out.write_char(')')
            }
            Expr::AtomicFence { ordering } => self.out.write_fmt(format_args!(
                "std::sync::atomic::fence({})",
                ordering_str(*ordering)
            )),
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
                write!(self.out, "std::ptr::{f}(")?;
                self.expr(src)?;
                self.out.write_str(", ")?;
                self.expr(dst)?;
                self.out.write_str(", ")?;
                self.expr(count)?;
                self.out.write_char(')')
            }
            Expr::WriteBytes { dst, val, count } => {
                self.out.write_str("std::ptr::write_bytes(")?;
                self.expr(dst)?;
                self.out.write_str(", ")?;
                self.expr(val)?;
                self.out.write_str(", ")?;
                self.expr(count)?;
                self.out.write_char(')')
            }
            Expr::Todo(note) => write!(self.out, "todo!({note:?})"),
        }
    }

    fn range_endpoint(&mut self, expr: &Expr) -> fmt::Result {
        if matches!(expr, Expr::Binary { .. }) {
            self.parenthesized(expr)
        } else {
            self.expr_prec(expr, PREC_RANGE + 1)
        }
    }

    fn atomic_ref(&mut self, ty: AtomicType, place: &AtomicPlace) -> fmt::Result {
        match place {
            AtomicPlace::Ptr(ptr) => {
                write!(
                    self.out,
                    "std::sync::atomic::{}::from_ptr(",
                    atomic_wrapper(ty)
                )?;
                self.expr(ptr)?;
                self.out.write_char(')')
            }
            AtomicPlace::Local(name) => self.ident(name.as_str()),
        }
    }

    fn prefix_operand(&mut self, expr: &Expr) -> fmt::Result {
        if matches!(expr, Expr::Unary { .. } | Expr::Ref { .. }) {
            self.parenthesized(expr)
        } else {
            self.expr_prec(expr, PREC_PREFIX)
        }
    }

    fn expr_block(&mut self, block: &Block) -> fmt::Result {
        if block.stmts.is_empty() {
            self.out.write_str("{ ")?;
            if let Some(tail) = &block.tail {
                self.expr(tail)?;
            }
            self.out.write_str(" }")
        } else {
            self.out.write_str("{\n")?;
            self.block(block, 1)?;
            self.out.write_char('}')
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
            RustValue::I64(n) => write!(self.out, "{n}"),
            RustValue::Usize(n) => write!(self.out, "{n}usize"),
            RustValue::I128(n) => write!(self.out, "{n}"),
            RustValue::U128(n) => write!(self.out, "{n}"),
            RustValue::TypedInt(n, ty) => write!(self.out, "{n}{}", ty.spelling()),
            RustValue::TypedUInt(n, ty) => write!(self.out, "{n}{}", ty.spelling()),
            RustValue::Float(n) => {
                if n.fract() == 0.0 {
                    write!(self.out, "{n:.1}")
                } else {
                    write!(self.out, "{n}")
                }
            }
            RustValue::Bool(b) => write!(self.out, "{b}"),
            RustValue::None => self.out.write_str("None"),
            RustValue::NullPtr => self.out.write_str("std::ptr::null_mut()"),
        }
    }

    fn pattern(&mut self, pattern: &crate::backend::rust_ast::Pattern) -> fmt::Result {
        match pattern {
            crate::backend::rust_ast::Pattern::Wildcard => self.out.write_char('_'),
            crate::backend::rust_ast::Pattern::Binding(name) => self.ident(name.as_str()),
            crate::backend::rust_ast::Pattern::I64(n) => write!(self.out, "{n}"),
            crate::backend::rust_ast::Pattern::I128(n) => write!(self.out, "{n}"),
            crate::backend::rust_ast::Pattern::U128(n) => write!(self.out, "{n}"),
            crate::backend::rust_ast::Pattern::InclusiveRange { start, end } => {
                write!(self.out, "{start}..={end}")
            }
            crate::backend::rust_ast::Pattern::Guarded { bind, cond } => {
                self.ident(bind.as_str())?;
                self.out.write_str(" if ")?;
                self.expr(cond)
            }
            crate::backend::rust_ast::Pattern::TupleStruct { name, fields } => {
                self.ident_path(name.as_str())?;
                self.out.write_char('(')?;
                for (i, field) in fields.iter().enumerate() {
                    if i > 0 {
                        self.out.write_str(", ")?;
                    }
                    self.pattern(field)?;
                }
                self.out.write_char(')')
            }
        }
    }

    fn path(&mut self, path: &Path) -> fmt::Result {
        for (i, segment) in path.segments.iter().enumerate() {
            if i > 0 {
                self.out.write_str("::")?;
            }
            self.ident(segment.as_str())?;
        }
        Ok(())
    }

    fn ty(&mut self, ty: &Type) -> fmt::Result {
        match ty {
            Type::Prim(p) => self.out.write_str(p.spelling()),
            Type::Custom(n) => self.ident_path(n),
            Type::LongDouble => self.out.write_str("LongDouble"),
            Type::TyVar(name) => self.ident(name.as_str()),
            Type::CLib(c) => self.out.write_str(c.path()),
            Type::Complex(inner) => {
                self.out.write_str("num_complex::Complex<")?;
                self.ty(inner)?;
                self.out.write_char('>')
            }
            Type::Generic { name, args } => {
                self.ident_path(name)?;
                self.out.write_char('<')?;
                for (i, arg) in args.iter().enumerate() {
                    if i > 0 {
                        self.out.write_str(", ")?;
                    }
                    self.ty(arg)?;
                }
                self.out.write_char('>')
            }
            Type::VaList => self.out.write_str("core::ffi::VaList<'_>"),
            Type::Str => self.out.write_str("str"),
            Type::Ref { mutable, inner } => {
                self.out.write_char('&')?;
                if *mutable {
                    self.out.write_str("mut ")?;
                }
                self.ty(inner)
            }
            Type::Slice(elem) => {
                self.out.write_char('[')?;
                self.ty(elem)?;
                self.out.write_char(']')
            }
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
            Type::FnPtr { abi, params, ret } => {
                self.out.write_str("Option<unsafe ")?;
                self.abi(*abi)?;
                self.out.write_str(" fn(")?;
                for (i, p) in params.iter().enumerate() {
                    if i > 0 {
                        self.out.write_str(", ")?;
                    }
                    self.ty(p)?;
                }
                self.out.write_char(')')?;
                if !ret.is_unit() {
                    self.out.write_str(" -> ")?;
                    self.ty(ret)?;
                }
                self.out.write_char('>')
            }
            Type::Unit => self.out.write_str("()"),
            Type::Variadic => self.out.write_str("..."),
            Type::Never => self.out.write_char('!'),
        }
    }
}

fn record_fields_are_copy(fields: &[RecordField]) -> bool {
    fields.iter().all(|field| type_is_copy(&field.ty))
}

fn type_is_copy(ty: &Type) -> bool {
    match ty {
        Type::Custom(name) => name != "__SlateVaArgs",
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

pub fn program_to_string(program: &Program) -> String {
    let mut cg = Codegen::new(String::new());
    cg.program(program)
        .expect("writing to a String never fails");
    cg.into_inner()
}

pub fn item_to_string(item: &Item) -> String {
    let mut cg = Codegen::new(String::new());
    cg.item(item).expect("writing to a String never fails");
    cg.into_inner()
}

pub fn expr_to_string(expr: &Expr) -> String {
    let mut cg = Codegen::new(String::new());
    cg.expr(expr).expect("writing to a String never fails");
    cg.into_inner()
}

pub fn stmt_to_string(stmt: &Stmt) -> String {
    let mut cg = Codegen::new(String::new());
    cg.stmt(stmt, 0).expect("writing to a String never fails");
    cg.into_inner()
}

pub fn type_to_string(ty: &Type) -> String {
    let mut cg = Codegen::new(String::new());
    cg.ty(ty).expect("writing to a String never fails");
    cg.into_inner()
}

pub fn cfg_to_string(cfg: &Cfg) -> String {
    let mut cg = Codegen::new(String::new());
    cg.cfg(cfg).expect("writing to a String never fails");
    cg.into_inner()
}

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
