//! lower: combine the CIR Op-tree with the C AST oracle into Rust output.

use crate::c_ast::{RecordKind, Unit};
use crate::cir::ir::{Attr, Block, Module, Op, Region};
use crate::ctx::Ctx;
use crate::rust_ast::{Item, Program};
use std::collections::{BTreeMap, BTreeSet};

/// How a translation unit fits into a multi-file project: which symbols other
/// units define, which sibling modules the crate root must declare, and whether
/// definitions are emitted `pub` so siblings can import them.
#[derive(Default, Clone)]
pub struct ProjectInfo {
    /// function symbol → module (rust file stem) that defines it, for functions
    /// defined anywhere in the project. A body-less decl whose symbol is in here
    /// becomes `use crate::<module>::<sym>;` instead of an `extern "C"` decl.
    pub cross_module: BTreeMap<String, String>,
    /// global symbol → module that defines it. An `extern` global whose symbol is
    /// in here becomes `use crate::<module>::<sym>;` instead of an extern static.
    pub cross_module_globals: BTreeMap<String, String>,
    /// modules the crate root declares with `mod <name>;` (empty for non-root).
    pub child_modules: Vec<String>,
    /// emit function and global definitions as `pub` so other modules can import them.
    pub emit_pub: bool,
}

/// CIR encodes external linkage as `linkage = 0`; a C `static` is nonzero.
fn linkage_is_external(op: &Op) -> bool {
    attr_int(op, "linkage").unwrap_or(0) == 0
}

pub fn defined_functions(module: &Module) -> Vec<String> {
    let Some(module_op) = module.ops.iter().find(|op| op.name == "builtin.module") else {
        return Vec::new();
    };
    region_ops(module_op)
        .iter()
        .filter(|op| op.name == "cir.func" && !region_ops(op).is_empty() && linkage_is_external(op))
        .filter_map(|op| attr_str(op, "sym_name").map(str::to_string))
        .collect()
}

pub fn defined_globals(module: &Module) -> Vec<String> {
    let Some(module_op) = module.ops.iter().find(|op| op.name == "builtin.module") else {
        return Vec::new();
    };
    region_ops(module_op)
        .iter()
        .filter(|op| {
            op.name == "cir.global"
                && attr_str(op, "initial_value").is_some()
                && linkage_is_external(op)
        })
        .filter_map(|op| attr_str(op, "sym_name").map(sanitize_ident))
        .collect()
}

/// Lower a parsed CIR module (with the C AST as an oracle) to a Rust program.
pub fn lower(cir: &Module, c: &Unit, ctx: &mut Ctx) -> Program {
    lower_with_project(cir, c, ctx, &ProjectInfo::default())
}

/// Lower a translation unit that is part of a multi-file project.
pub fn lower_with_project(cir: &Module, c: &Unit, ctx: &mut Ctx, project: &ProjectInfo) -> Program {
    let mut lowerer = Lowerer {
        ctx,
        aliases: cir.aliases.clone(),
        records: c
            .records
            .iter()
            .map(|record| (sanitize_ident(&record.name), record.clone()))
            .collect(),
        globals: BTreeMap::new(),
        extern_globals: BTreeMap::new(),
        strings: BTreeMap::new(),
        const_arrays: BTreeMap::new(),
        const_aggregates: BTreeMap::new(),
        const_zero_globals: BTreeSet::new(),
        externs: BTreeMap::new(),
        extern_returns: BTreeMap::new(),
        uses_long_double: std::cell::Cell::new(false),
        uses_complex: std::cell::Cell::new(false),
        uses_c_variadic: std::cell::Cell::new(false),
        variadic_defs: BTreeSet::new(),
        project: project.clone(),
        cross_uses: Vec::new(),
    };
    lowerer.lower_module(cir, c)
}

struct Lowerer<'a> {
    ctx: &'a mut Ctx,
    aliases: BTreeMap<String, String>,
    records: BTreeMap<String, crate::c_ast::Record>,
    globals: BTreeMap<String, GlobalVar>,
    extern_globals: BTreeMap<String, String>,
    strings: BTreeMap<String, Vec<u8>>,
    /// numeric aggregate const globals (e.g. `int a[5]={..}`) → element literals,
    /// keyed by raw sym_name; consumed when a `cir.copy` initializes a local.
    const_arrays: BTreeMap<String, Vec<String>>,
    /// nested aggregate const globals (structs, unions, arrays of aggregates) →
    /// their raw `#cir.const_record`/`#cir.const_array` initializer, keyed by raw
    /// sym_name; rendered recursively against the destination type on `cir.copy`.
    const_aggregates: BTreeMap<String, String>,
    const_zero_globals: BTreeSet<String>,
    /// external (body-less) functions → rust types of their fixed params; the
    /// call site uses this to `as`-cast args and wrap the call in `unsafe`.
    externs: BTreeMap<String, Vec<String>>,
    extern_returns: BTreeMap<String, Option<String>>,
    uses_long_double: std::cell::Cell<bool>,
    uses_complex: std::cell::Cell<bool>,
    uses_c_variadic: std::cell::Cell<bool>,
    variadic_defs: BTreeSet<String>,
    project: ProjectInfo,
    /// `use crate::<mod>::<sym>;` lines for body-less decls resolved to a sibling.
    cross_uses: Vec<String>,
}

struct FunctionLowerer<'a, 'b> {
    parent: &'a mut Lowerer<'b>,
    values: BTreeMap<String, Val>,
    slots: BTreeMap<String, String>,
    slot_types: BTreeMap<String, String>,
    member_ptrs: BTreeMap<String, MemberPtr>,
    element_ptrs: BTreeMap<String, ElementPtr>,
    temp_counter: usize,
    indent: usize,
    out: String,
    is_main: bool,
    loop_stack: Vec<LoopFrame>,
    label_counter: usize,
    /// Set for functions with unstructured control flow (goto/multi-block); drives
    /// the state-machine dispatch loop. `None` for structured functions.
    dispatch: Option<DispatchCtx>,
    /// Alloca results hoisted above the dispatch loop so locals outlive block arms.
    hoisted: BTreeSet<String>,
    /// `va_list` SSA values (the `ap` alloca and its array-decay casts) → slot name.
    va_places: BTreeMap<String, String>,
    va_args_param: Option<String>,
}

/// Maps CIR jump targets to dispatch-loop states for a `goto`-bearing function.
struct DispatchCtx {
    loop_label: String,
    state_var: String,
    /// C label name (`cir.label`/`cir.goto`) -> block index.
    label_to_state: BTreeMap<String, usize>,
    /// Block label (`^bbN`, entry as `bb0`) -> block index, for `cir.br`.
    block_to_state: BTreeMap<String, usize>,
}

// How `cir.break`/`cir.continue` lower for the enclosing loop. A C `for` loop's
// step must still run on `continue`, but Rust's `continue` skips the loop tail,
// so the body is wrapped in a labeled block and `continue` becomes
// `break 'continue`. Rust then forbids an unlabeled `break` from diverging out
// through that block, so the loop is labeled too and `break` targets it.
struct LoopFrame {
    break_label: Option<String>,
    continue_label: Option<String>,
    is_loop: bool,
}

struct SwitchCase<'a> {
    values: Vec<i64>,
    is_default: bool,
    region: &'a Region,
}

#[derive(Debug, Clone)]
struct MemberPtr {
    base: String,
    field: String,
    unsafe_access: bool,
}

#[derive(Debug, Clone)]
struct ElementPtr {
    base: String,
    index: String,
    unsafe_access: bool,
}

#[derive(Debug, Clone)]
struct GlobalVar {
    name: String,
    ty: String,
    init: String,
    external: bool,
}

#[derive(Debug, Clone)]
enum Val {
    Expr(String),
    Global(String),
}

impl Val {
    fn render(&self, strings: &BTreeMap<String, Vec<u8>>) -> String {
        match self {
            Val::Expr(s) => s.clone(),
            Val::Global(name) => match strings.get(name) {
                Some(bytes) => {
                    // *mut so it fits *mut char slots; weakens to *const for printf/libc.
                    format!("{}.as_ptr() as *mut libc::c_char", rust_byte_string(bytes))
                }
                None => name.clone(),
            },
        }
    }
}

impl<'a> Lowerer<'a> {
    fn lower_module(&mut self, module: &Module, c: &Unit) -> Program {
        let mut items = vec![Item::Raw(
            "#![allow(dead_code, unused, non_snake_case, non_upper_case_globals, arithmetic_overflow)]".into(),
        )];

        for enm in &c.enums {
            if let Some(text) = self.lower_enum(enm) {
                items.push(Item::Raw(text));
            }
        }
        for record in &c.records {
            if let Some(text) = self.lower_record(record) {
                items.push(Item::Raw(text));
            }
        }
        for text in self.standard_record_defs() {
            items.push(Item::Raw(text));
        }

        let Some(module_op) = module.ops.iter().find(|op| op.name == "builtin.module") else {
            self.ctx
                .diagnostics
                .error("lower: no builtin.module op", None);
            return Program { items };
        };

        let ops = region_ops(module_op);
        for op in &ops {
            if op.name == "cir.global" {
                self.collect_global(op);
            }
        }
        for global in self.globals.values() {
            let global_vis = if self.project.emit_pub && global.external {
                "pub "
            } else {
                ""
            };
            items.push(Item::Raw(format!(
                "{global_vis}static mut {}: {} = {};\n",
                global.name, global.ty, global.init
            )));
        }

        let module_uses_long_double = ops.iter().any(|op| op_mentions_long_double(op));
        let mut extern_decls = Vec::new();
        for (name, ty) in &self.extern_globals {
            // an extern global defined in a sibling TU becomes a module import.
            if let Some(module) = self.project.cross_module_globals.get(name) {
                self.cross_uses
                    .push(format!("use crate::{module}::{name};"));
                continue;
            }
            extern_decls.push(format!("static mut {name}: {ty};"));
        }
        for op in &ops {
            if op.name != "cir.func" || !region_ops(op).is_empty() {
                continue;
            }
            let Some(name) = attr_str(op, "sym_name") else {
                continue;
            };
            // complex runtime routines are declared in the prelude.
            if is_complex_runtime_call(name) {
                continue;
            }
            // a prototype whose definition lives in a sibling TU becomes a module
            // import; the call then flows through the normal (non-extern) path.
            if let Some(module) = self.project.cross_module.get(name) {
                self.cross_uses
                    .push(format!("use crate::{module}::{name};"));
                continue;
            }
            let function_type = attr_str(op, "function_type").unwrap_or("");
            let (sig, params, ret) = self.extern_fn_signature(name, function_type);
            self.externs.insert(name.to_string(), params);
            self.extern_returns.insert(name.to_string(), ret.clone());
            if name == "strtold" && ret.as_deref() == Some(LONG_DOUBLE_TY) {
                extern_decls.push(
                    "fn __slate_strtold(_0: *mut i8, _1: *mut *mut i8, _2: *mut LongDouble);"
                        .to_string(),
                );
            } else {
                extern_decls.push(sig);
            }
            if name == "printf" && module_uses_long_double {
                extern_decls.push(
                    "fn __slate_printf_ld_i32(_0: *mut i8, _1: *const LongDouble, _2: i32) -> i32;"
                        .to_string(),
                );
            }
        }
        if !extern_decls.is_empty() {
            items.push(Item::Raw(format!(
                "unsafe extern \"C\" {{\n    {}\n}}\n",
                extern_decls.join("\n    ")
            )));
        }

        // collected before lowering so call sites wrap in `unsafe` regardless of order.
        for op in &ops {
            if op.name == "cir.func"
                && !region_ops(op).is_empty()
                && attr_str(op, "sym_name").is_some_and(|name| name != "main")
                && function_type_is_variadic(attr_str(op, "function_type").unwrap_or(""))
            {
                self.variadic_defs
                    .insert(attr_str(op, "sym_name").unwrap().to_string());
            }
        }

        for op in ops {
            if op.name != "cir.func" || region_ops(op).is_empty() {
                continue;
            }
            match self.lower_func(op) {
                Some(text) => items.push(Item::Raw(text)),
                None => self.ctx.diagnostics.warn(
                    format!("lower: skipped function {:?}", attr_str(op, "sym_name")),
                    op.loc.clone(),
                ),
            }
        }

        if self.uses_long_double.get() {
            items.insert(1, Item::Raw(LONG_DOUBLE_PRELUDE.to_string()));
        }
        if self.uses_complex.get() {
            items.insert(1, Item::Raw(COMPLEX_PRELUDE.to_string()));
        }

        // module wiring goes right after the crate-level `#![allow(..)]` attr.
        let mut wiring: Vec<String> = self
            .project
            .child_modules
            .iter()
            .map(|m| format!("mod {m};"))
            .collect();
        wiring.append(&mut self.cross_uses);
        for (offset, line) in wiring.into_iter().enumerate() {
            items.insert(1 + offset, Item::Raw(line));
        }

        // grouped with the crate-level `#![allow(..)]` so both stay at the top.
        if self.uses_c_variadic.get()
            && let Some(Item::Raw(first)) = items.first_mut()
        {
            *first = format!("#![feature(c_variadic)]\n{first}");
        }

        Program { items }
    }

    fn collect_global(&mut self, op: &Op) {
        let Some(name) = attr_str(op, "sym_name") else {
            return;
        };
        let rust_name = sanitize_ident(name);
        let ty = attr_str(op, "sym_type").map(|ty| self.rust_type(ty));
        let is_c_global = !name.starts_with("__") && !name.starts_with(".str");
        let Some(raw) = attr_str(op, "initial_value") else {
            let Some(ty) = ty else {
                return;
            };
            self.extern_globals.insert(rust_name, ty);
            return;
        };
        if let Some(mut bytes) = parse_cir_const_array(raw) {
            if is_c_global && let Some(ty) = ty {
                let elems: Vec<String> = bytes.iter().map(|b| b.to_string()).collect();
                self.globals.insert(
                    rust_name.clone(),
                    GlobalVar {
                        name: rust_name,
                        ty,
                        init: render_array_literal(&elems, bytes.len()),
                        external: linkage_is_external(op),
                    },
                );
            } else {
                bytes.push(0);
                self.strings.insert(name.to_string(), bytes);
            }
        } else if let Some(elems) = parse_cir_const_array_elems(raw) {
            if is_c_global && let Some(ty) = ty {
                if let Some((_, len)) = parse_rust_array_type(&ty) {
                    self.globals.insert(
                        rust_name.clone(),
                        GlobalVar {
                            name: rust_name,
                            ty,
                            init: render_array_literal(&elems, len as usize),
                            external: linkage_is_external(op),
                        },
                    );
                }
            } else {
                self.const_arrays.insert(name.to_string(), elems);
            }
        } else if is_cir_aggregate_init(raw) {
            if is_c_global && let Some(ty) = ty {
                if let Some(init) = self.render_const_value(&ty, raw) {
                    self.globals.insert(
                        rust_name.clone(),
                        GlobalVar {
                            name: rust_name,
                            ty,
                            init,
                            external: linkage_is_external(op),
                        },
                    );
                }
            } else {
                self.const_aggregates
                    .insert(name.to_string(), raw.to_string());
            }
        } else if raw.trim_start().starts_with("#cir.zero")
            && let Some((elem, len)) = parse_cir_array_type(attr_str(op, "sym_type").unwrap_or(""))
        {
            if is_c_global && let Some(ty) = ty {
                self.globals.insert(
                    rust_name.clone(),
                    GlobalVar {
                        name: rust_name,
                        ty,
                        init: format!("[{}; {len}]", zero_for_cir_type(&elem)),
                        external: linkage_is_external(op),
                    },
                );
            } else if elem == "!s8i" && name.starts_with(".str") {
                self.strings.insert(name.to_string(), vec![0; len as usize]);
            } else {
                self.const_arrays.insert(name.to_string(), Vec::new());
            }
        } else if raw.trim_start().starts_with("#cir.zero") {
            if is_c_global && let Some(ty) = ty {
                self.globals.insert(
                    rust_name.clone(),
                    GlobalVar {
                        name: rust_name,
                        init: self.default_value(&ty),
                        ty,
                        external: linkage_is_external(op),
                    },
                );
            } else {
                self.const_zero_globals.insert(name.to_string());
            }
        } else if let Some(init) = parse_cir_int(raw)
            .map(|n| n.to_string())
            .or_else(|| parse_cir_fp(raw))
        {
            let ty = ty.unwrap_or_else(|| self.rust_type("!s32i"));
            let external = linkage_is_external(op);
            self.globals.insert(
                rust_name.clone(),
                GlobalVar {
                    name: rust_name,
                    ty,
                    init,
                    external,
                },
            );
        }
    }

    fn lower_enum(&mut self, enm: &crate::c_ast::Enum) -> Option<String> {
        if enm.variants.is_empty() {
            return None;
        }
        let mut text = String::new();
        for variant in &enm.variants {
            text.push_str(&format!(
                "const {}: i32 = {};\n",
                sanitize_ident(&variant.name),
                variant.value
            ));
        }
        Some(text)
    }

    fn lower_record(&mut self, record: &crate::c_ast::Record) -> Option<String> {
        if record.fields.is_empty() {
            return None;
        }
        let mut text = match record.kind {
            RecordKind::Struct => format!(
                "#[repr(C)]\n#[derive(Clone, Copy)]\nstruct {} {{\n",
                sanitize_ident(&record.name)
            ),
            RecordKind::Union => format!(
                "#[repr(C)]\n#[derive(Clone, Copy)]\nunion {} {{\n",
                sanitize_ident(&record.name)
            ),
        };
        for field in &record.fields {
            text.push_str(&format!(
                "    {}: {},\n",
                sanitize_ident(&field.name),
                self.rust_c_type(&field.ty)
            ));
        }
        text.push_str("}\n");
        Some(text)
    }

    fn standard_record_defs(&self) -> Vec<String> {
        let mut out = Vec::new();
        for name in ["div_t", "ldiv_t", "lldiv_t", "imaxdiv_t", "tm", "lconv"] {
            if self.records.contains_key(name) {
                continue;
            }
            if self
                .aliases
                .values()
                .any(|ty| cir_record_name(ty) == Some(name))
            {
                out.push(standard_record_def(name).to_string());
            }
        }
        out
    }

    fn lower_func(&mut self, op: &Op) -> Option<String> {
        let name = attr_str(op, "sym_name")?;
        let function_type = attr_str(op, "function_type").unwrap_or("");
        let (param_types, ret_ty) = parse_function_type(function_type);
        let entry = op.regions.first()?.blocks.first()?;
        let is_main = name == "main";
        let is_variadic = !is_main && function_type_is_variadic(function_type);

        let mut text = String::new();
        let mut params = entry
            .args
            .iter()
            .enumerate()
            .map(|(i, (arg, ty))| {
                let ty = param_types.get(i).map(String::as_str).unwrap_or(ty);
                format!("{arg}: {}", self.rust_type(ty))
            })
            .collect::<Vec<_>>();

        let va_args_param = if is_variadic {
            let param = "__slate_va_args".to_string();
            params.push(format!("mut {param}: ..."));
            Some(param)
        } else {
            None
        };
        let params = params.join(", ");

        if is_main {
            text.push_str("fn main() {\n");
            text.push_str(&self.main_arg_bindings(entry));
        } else {
            let vis = if self.project.emit_pub && linkage_is_external(op) {
                "pub "
            } else {
                ""
            };
            // Rust variadics must be `unsafe extern "C"`.
            let prefix = if is_variadic {
                self.uses_c_variadic.set(true);
                self.variadic_defs.insert(name.to_string());
                "unsafe extern \"C\" "
            } else {
                ""
            };
            text.push_str(&format!(
                "{vis}{prefix}fn {name}({params}) -> {} {{\n",
                self.rust_type(ret_ty.as_deref().unwrap_or("()"))
            ));
        }

        let mut f = FunctionLowerer {
            parent: self,
            values: BTreeMap::new(),
            slots: BTreeMap::new(),
            slot_types: BTreeMap::new(),
            member_ptrs: BTreeMap::new(),
            element_ptrs: BTreeMap::new(),
            temp_counter: 0,
            indent: 1,
            out: String::new(),
            is_main,
            loop_stack: Vec::new(),
            label_counter: 0,
            dispatch: None,
            hoisted: BTreeSet::new(),
            va_places: BTreeMap::new(),
            va_args_param,
        };

        for (arg, _) in &entry.args {
            f.values.insert(arg.clone(), Val::Expr(arg.clone()));
        }
        let body = op.regions.first().unwrap();
        if body.blocks.len() > 1 {
            f.lower_dispatch(body);
        } else {
            f.lower_block(entry);
        }
        text.push_str(&f.out);
        text.push_str("}\n");
        Some(text)
    }

    fn main_arg_bindings(&self, entry: &Block) -> String {
        if entry.args.is_empty() {
            return String::new();
        }
        let mut text = String::new();
        text.push_str("    let mut __slate_argv_storage: Vec<std::ffi::CString> = std::env::args().map(|arg| std::ffi::CString::new(arg).unwrap()).collect();\n");
        text.push_str("    let mut __slate_argv_ptrs: Vec<*mut i8> = __slate_argv_storage.iter().map(|arg| arg.as_ptr() as *mut i8).collect();\n");
        text.push_str("    __slate_argv_ptrs.push(std::ptr::null_mut());\n");

        for (i, (arg, ty)) in entry.args.iter().enumerate() {
            let name = sanitize_ident(arg);
            let rust_ty = self.rust_type(ty);
            let value = match i {
                0 => "__slate_argv_storage.len() as i32".to_string(),
                1 => "__slate_argv_ptrs.as_mut_ptr()".to_string(),
                _ => "std::ptr::null_mut()".to_string(),
            };
            text.push_str(&format!("    let {name}: {rust_ty} = {value};\n"));
        }
        text
    }

    /// Build a Rust `extern "C"` signature line for a body-less C declaration,
    /// returning `(line, fixed_param_rust_types, return_type)`. Trailing `...` becomes a Rust
    /// variadic; a missing return arrow means the C function returns `void`.
    fn extern_fn_signature(
        &self,
        name: &str,
        function_type: &str,
    ) -> (String, Vec<String>, Option<String>) {
        let inner = function_type
            .strip_prefix("!cir.func<")
            .and_then(|s| s.strip_suffix('>'))
            .unwrap_or("");
        let (params_str, ret) = match split_top_level_arrow(inner) {
            Some((params, ret)) => (params.trim(), Some(ret.trim())),
            None => (inner.trim(), None),
        };
        let params_str = params_str.trim_start_matches('(').trim_end_matches(')');

        let mut parts = Vec::new();
        let mut param_types = Vec::new();
        let mut variadic = false;
        for (i, raw) in split_top_level(params_str, ',')
            .into_iter()
            .map(str::trim)
            .filter(|s| !s.is_empty())
            .enumerate()
        {
            if raw == "..." {
                variadic = true;
            } else {
                let ty = self.rust_type(raw);
                parts.push(format!("_{i}: {ty}"));
                param_types.push(ty);
            }
        }
        if variadic {
            parts.push("...".to_string());
        }
        let ret_ty = match ret {
            Some(ret) if ret != "()" => Some(self.rust_type(ret)),
            _ => None,
        };
        let ret = ret_ty
            .as_ref()
            .map(|ty| format!(" -> {ty}"))
            .unwrap_or_default();
        let line = format!("fn {name}({}){ret};", parts.join(", "));
        (line, param_types, ret_ty)
    }

    fn rust_type(&self, cir_ty: &str) -> String {
        let ty = rust_type_with_aliases(cir_ty, &self.aliases);
        if ty.contains(LONG_DOUBLE_TY) {
            self.uses_long_double.set(true);
        }
        if ty.contains(COMPLEX_TY) {
            self.uses_complex.set(true);
        }
        ty
    }

    fn cir_type_is_union(&self, ty: &str) -> bool {
        let ty = self.expand_alias(ty);
        if ty.starts_with("!cir.union<") {
            return true;
        }
        cir_record_name(ty)
            .and_then(|name| self.records.get(&sanitize_ident(name)))
            .is_some_and(|record| record.kind == RecordKind::Union)
    }

    fn expand_alias<'b>(&'b self, ty: &'b str) -> &'b str {
        let mut ty = ty.trim();
        let mut seen = BTreeSet::new();
        while let Some(expanded) = self.aliases.get(ty) {
            if !seen.insert(ty.to_string()) {
                break;
            }
            ty = expanded.trim();
        }
        ty
    }

    fn rust_c_type(&self, ty: &crate::c_ast::CType) -> String {
        let rust = c_type_to_rust(ty);
        if rust.contains(LONG_DOUBLE_TY) {
            self.uses_long_double.set(true);
        }
        rust
    }

    fn default_value(&self, ty: &str) -> String {
        if let Some(record) = self.records.get(ty) {
            match record.kind {
                RecordKind::Struct => {
                    let fields = record
                        .fields
                        .iter()
                        .map(|field| {
                            format!(
                                "{}: {}",
                                sanitize_ident(&field.name),
                                self.default_value(&c_type_to_rust(&field.ty))
                            )
                        })
                        .collect::<Vec<_>>()
                        .join(", ");
                    return format!("{} {{ {fields} }}", sanitize_ident(&record.name));
                }
                RecordKind::Union => {
                    if let Some(field) = record.fields.first() {
                        return format!(
                            "{} {{ {}: {} }}",
                            sanitize_ident(&record.name),
                            sanitize_ident(&field.name),
                            self.default_value(&c_type_to_rust(&field.ty))
                        );
                    }
                }
            }
        }
        if is_long_double(ty) || ty == LONG_DOUBLE_TY {
            return format!("{LONG_DOUBLE_TY}(0.0)");
        }
        if let Some(value) = standard_record_default(ty) {
            return value.to_string();
        }
        if let Some(inner) = ty
            .strip_prefix(COMPLEX_TY)
            .and_then(|s| s.strip_suffix('>'))
        {
            let d = default_value(inner);
            return format!("Complex {{ re: {d}, im: {d} }}");
        }
        if let Some((inner, len)) = parse_cir_array_type(ty) {
            return format!("[{}; {len}]", self.default_value(&inner));
        }
        if let Some((inner, len)) = parse_rust_array_type(ty) {
            return format!("[{}; {len}]", self.default_value(inner));
        }
        default_value(ty).into()
    }

    fn render_const_value(&self, rust_ty: &str, raw: &str) -> Option<String> {
        let raw = raw.trim();
        if raw.starts_with("#cir.const_record<") {
            let record = self.records.get(rust_ty)?;
            let open = raw.find('{')?;
            let close = raw.rfind('}')?;
            let elems = split_top_level(&raw[open + 1..close], ',');
            match record.kind {
                RecordKind::Struct => {
                    let fields = record
                        .fields
                        .iter()
                        .enumerate()
                        .map(|(i, field)| {
                            let field_ty = c_type_to_rust(&field.ty);
                            let value = elems
                                .get(i)
                                .and_then(|e| self.render_const_value(&field_ty, e.trim()))
                                .unwrap_or_else(|| self.default_value(&field_ty));
                            format!("{}: {value}", sanitize_ident(&field.name))
                        })
                        .collect::<Vec<_>>()
                        .join(", ");
                    Some(format!("{} {{ {fields} }}", sanitize_ident(&record.name)))
                }
                RecordKind::Union => {
                    let field = record.fields.first()?;
                    let field_ty = c_type_to_rust(&field.ty);
                    let value = elems
                        .first()
                        .and_then(|e| self.render_const_value(&field_ty, e.trim()))
                        .unwrap_or_else(|| self.default_value(&field_ty));
                    Some(format!(
                        "{} {{ {}: {value} }}",
                        sanitize_ident(&record.name),
                        sanitize_ident(&field.name)
                    ))
                }
            }
        } else if raw.starts_with("#cir.const_array<[") {
            let (elem_ty, len) = parse_rust_array_type(rust_ty)?;
            let open = raw.find('[')?;
            let close = raw.rfind(']')?;
            let mut out: Vec<String> = split_top_level(&raw[open + 1..close], ',')
                .into_iter()
                .map(|e| e.trim().to_string())
                .filter(|e| !e.is_empty())
                .take(len as usize)
                .map(|e| {
                    self.render_const_value(elem_ty, &e)
                        .unwrap_or_else(|| self.default_value(elem_ty))
                })
                .collect();
            out.resize(len as usize, self.default_value(elem_ty));
            Some(format!("[{}]", out.join(", ")))
        } else if raw.starts_with("#cir.zero") {
            Some(self.default_value(rust_ty))
        } else {
            parse_cir_int(raw)
                .map(|n| n.to_string())
                .or_else(|| parse_cir_fp(raw))
        }
    }
}

fn c_type_to_rust(ty: &crate::c_ast::CType) -> String {
    match ty {
        crate::c_ast::CType::Void => "()".into(),
        crate::c_ast::CType::Bool => "bool".into(),
        crate::c_ast::CType::Int {
            signed: true,
            bits: 8,
        } => "i8".into(),
        crate::c_ast::CType::Int {
            signed: false,
            bits: 8,
        } => "u8".into(),
        crate::c_ast::CType::Int {
            signed: true,
            bits: 16,
        } => "i16".into(),
        crate::c_ast::CType::Int {
            signed: false,
            bits: 16,
        } => "u16".into(),
        crate::c_ast::CType::Int {
            signed: true,
            bits: 32,
        } => "i32".into(),
        crate::c_ast::CType::Int {
            signed: false,
            bits: 32,
        } => "u32".into(),
        crate::c_ast::CType::Int {
            signed: true,
            bits: 64,
        } => "i64".into(),
        crate::c_ast::CType::Int {
            signed: false,
            bits: 64,
        } => "u64".into(),
        crate::c_ast::CType::Int { .. } => "i32".into(),
        crate::c_ast::CType::Float { bits: 32 } => "f32".into(),
        crate::c_ast::CType::Float { bits: 64 } => "f64".into(),
        crate::c_ast::CType::Float { bits: 80 } => LONG_DOUBLE_TY.into(),
        crate::c_ast::CType::Float { .. } => "f64".into(),
        crate::c_ast::CType::Ptr(inner) => format!("*mut {}", c_type_to_rust(inner)),
        crate::c_ast::CType::FuncPtr { ret, params } => {
            let params = params
                .iter()
                .map(c_type_to_rust)
                .collect::<Vec<_>>()
                .join(", ");
            format!("Option<fn({params}) -> {}>", c_type_to_rust(ret))
        }
        crate::c_ast::CType::Array(inner, Some(len)) => {
            format!("[{}; {len}]", c_type_to_rust(inner))
        }
        crate::c_ast::CType::Array(inner, None) => format!("*mut {}", c_type_to_rust(inner)),
        crate::c_ast::CType::Record(name) => sanitize_ident(name),
    }
}

impl<'a, 'b> FunctionLowerer<'a, 'b> {
    fn lower_block(&mut self, block: &Block) {
        for op in &block.ops {
            self.lower_op(op);
        }
    }

    fn lower_region_ops(&mut self, region: &Region) {
        for block in &region.blocks {
            self.lower_block(block);
        }
    }

    fn lower_op(&mut self, op: &Op) {
        match op.name.as_str() {
            "cir.alloca" => self.lower_alloca(op),
            "cir.store" => self.lower_store(op),
            "cir.copy" => self.lower_copy(op),
            "cir.load" => self.lower_load(op),
            "cir.const" => self.lower_const(op),
            "cir.add.overflow" => self.lower_overflow_arith(op, "overflowing_add"),
            "cir.sub.overflow" => self.lower_overflow_arith(op, "overflowing_sub"),
            "cir.mul.overflow" => self.lower_overflow_arith(op, "overflowing_mul"),
            "cir.div.overflow" => self.lower_overflow_arith(op, "overflowing_div"),
            "cir.rem.overflow" => self.lower_overflow_arith(op, "overflowing_rem"),
            "cir.add" => self.lower_int_arith(op, "+"),
            "cir.sub" => self.lower_int_arith(op, "-"),
            "cir.mul" => self.lower_int_arith(op, "*"),
            "cir.div" => self.lower_int_arith(op, "/"),
            "cir.rem" => self.lower_int_arith(op, "%"),
            "cir.and" => self.lower_int_arith(op, "&"),
            "cir.or" => self.lower_int_arith(op, "|"),
            "cir.xor" => self.lower_int_arith(op, "^"),
            "cir.shift" => self.lower_shift(op),
            "cir.not" => self.lower_not(op),
            "cir.minus" | "cir.fneg" => self.lower_neg(op),
            "cir.abs" => self.lower_abs(op),
            "cir.ceil" => self.lower_unary_method(op, "ceil"),
            "cir.copysign" => self.lower_binary_method(op, "copysign"),
            "cir.fabs" => self.lower_unary_method(op, "abs"),
            "cir.floor" => self.lower_unary_method(op, "floor"),
            "cir.fmaxnum" => self.lower_binary_method(op, "max"),
            "cir.fminnum" => self.lower_binary_method(op, "min"),
            "cir.is_fp_class" => self.lower_is_fp_class(op),
            "cir.modf" => self.lower_modf(op),
            "cir.nearbyint" => self.lower_unary_method(op, "round_ties_even"),
            "cir.rint" => self.lower_unary_method(op, "round_ties_even"),
            "cir.round" => self.lower_unary_method(op, "round"),
            "cir.signbit" => self.lower_signbit(op),
            "cir.trunc" => self.lower_unary_method(op, "trunc"),
            "cir.fadd" => self.lower_binary(op, "+"),
            "cir.fsub" => self.lower_binary(op, "-"),
            "cir.fmul" => self.lower_binary(op, "*"),
            "cir.fdiv" => self.lower_binary(op, "/"),
            "cir.complex.add" => self.lower_binary(op, "+"),
            "cir.complex.sub" => self.lower_binary(op, "-"),
            "cir.complex.create" => self.lower_complex_create(op),
            "cir.complex.real" => self.lower_complex_part(op, "re"),
            "cir.complex.imag" => self.lower_complex_part(op, "im"),
            "cir.inc" => self.lower_step(op, "+"),
            "cir.dec" => self.lower_step(op, "-"),
            "cir.cmp" => self.lower_cmp(op),
            "cir.select" => self.lower_select(op),
            "cir.ternary" => self.lower_ternary(op),
            "cir.get_global" => self.lower_get_global(op),
            "cir.get_member" => self.lower_get_member(op),
            "cir.get_element" => self.lower_get_element(op),
            "cir.cast" => self.lower_cast(op),
            "cir.ptr_stride" => self.lower_ptr_stride(op),
            "cir.ptr_diff" => self.lower_ptr_diff(op),
            "cir.call" => self.lower_call(op),
            "cir.va_start" => self.lower_va_start(op),
            "cir.va_arg" => self.lower_va_arg(op),
            // `VaList` drops on scope exit.
            "cir.va_end" => {}
            "cir.atomic.fetch" => self.lower_atomic_fetch(op),
            "cir.atomic.xchg" => self.lower_atomic_xchg(op),
            "cir.atomic.cmpxchg" => self.lower_atomic_cmpxchg(op),
            "cir.atomic.fence" => self.lower_atomic_fence(),
            "cir.return" => self.lower_return(op),
            "cir.scope" => self.lower_scope(op),
            "cir.if" => self.lower_if(op),
            "cir.switch" => self.lower_switch(op),
            "cir.for" => self.lower_for(op),
            "cir.while" => self.lower_while(op),
            "cir.do" => self.lower_do(op),
            "cir.break" => self.lower_break(),
            "cir.continue" => self.lower_continue(),
            "cir.goto" => self.lower_goto(op),
            "cir.br" => self.lower_br(op),
            "cir.label" => {}
            "cir.yield" | "cir.condition" => {}
            other => {
                self.parent
                    .ctx
                    .diagnostics
                    .warn(format!("lower: unsupported CIR op {other}"), op.loc.clone());
                self.emit_expr(format!("todo!({other:?})"));
            }
        }
    }

    fn lower_alloca(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        // hoisted allocas were already declared above the dispatch loop.
        if self.hoisted.contains(result) && self.dispatch.is_some() {
            return;
        }
        let name = sanitize_ident(attr_str(op, "name").unwrap_or(result));
        // a `va_list` local becomes a Rust `VaList`, assigned by `va_start`.
        if op
            .ty
            .as_deref()
            .is_some_and(|ty| ty.contains("__va_list_tag"))
        {
            self.slots.insert(result.clone(), name.clone());
            self.va_places.insert(result.clone(), name.clone());
            self.emit_line(&format!("let mut {name}: core::ffi::VaList<'_>;"));
            return;
        }
        let ty = self
            .pointee_type(op.ty.as_deref().unwrap_or(""))
            .unwrap_or_else(|| "i32".into());
        self.slots.insert(result.clone(), name.clone());
        self.slot_types.insert(result.clone(), ty.clone());
        self.emit_line(&format!(
            "let mut {name}: {ty} = {};",
            self.default_value(&ty)
        ));
    }

    fn lower_store(&mut self, op: &Op) {
        if op.operands.len() < 2 {
            return;
        }
        let operand_types = op_operand_types(op.ty.as_deref().unwrap_or(""));
        let value_ty = operand_types.first().copied();
        let value = if value_ty.is_some_and(is_cir_function_pointer_type) {
            self.render_function_pointer_operand(&op.operands[0])
        } else if value_ty.is_some_and(|ty| ty.starts_with("!cir.ptr<")) {
            self.render_pointer_operand(&op.operands[0])
        } else {
            self.render_operand(&op.operands[0])
        };
        let ptr = &op.operands[1];
        if attr_bool(op, "is_volatile") {
            let addr = self.store_address(ptr);
            self.emit_line(&format!(
                "unsafe {{ std::ptr::write_volatile({addr}, {value}); }}"
            ));
        } else if let Some(global) = self.global_name(ptr) {
            self.emit_line(&format!("unsafe {{ {global} = {value}; }}"));
        } else if let Some(member) = self.member_ptrs.get(ptr).cloned() {
            self.emit_line(&format!(
                "unsafe {{ {}.{} = {value}; }}",
                member.base, member.field
            ));
        } else if let Some(element) = self.element_ptrs.get(ptr).cloned() {
            let place = format!("{}[({}) as usize]", element.base, element.index);
            if element.unsafe_access {
                self.emit_line(&format!("unsafe {{ {place} = {value}; }}"));
            } else {
                self.emit_line(&format!("{place} = {value};"));
            }
        } else if let Some(slot) = self.slots.get(ptr) {
            self.emit_line(&format!("{slot} = {value};"));
        } else {
            let ptr = self.render_operand(ptr);
            self.emit_line(&format!("unsafe {{ *{ptr} = {value}; }}"));
        }
    }

    fn lower_copy(&mut self, op: &Op) {
        if op.operands.len() < 2 {
            return;
        }
        let dst = op.operands[0].clone();
        let src = op.operands[1].clone();
        let Some(value) = self.copy_source_value(&dst, &src) else {
            // opaque aggregate copy: fall back to a raw one-element memcpy.
            let d = self.render_pointer_operand(&dst);
            let s = self.render_pointer_operand(&src);
            self.emit_line(&format!(
                "unsafe {{ std::ptr::copy_nonoverlapping({s}, {d}, 1); }}"
            ));
            return;
        };
        if let Some(global) = self.global_name(&dst) {
            self.emit_line(&format!("unsafe {{ {global} = {value}; }}"));
        } else if let Some(member) = self.member_ptrs.get(&dst).cloned() {
            self.emit_line(&format!(
                "unsafe {{ {}.{} = {value}; }}",
                member.base, member.field
            ));
        } else if let Some(element) = self.element_ptrs.get(&dst).cloned() {
            let place = format!("{}[({}) as usize]", element.base, element.index);
            if element.unsafe_access {
                self.emit_line(&format!("unsafe {{ {place} = {value}; }}"));
            } else {
                self.emit_line(&format!("{place} = {value};"));
            }
        } else if let Some(slot) = self.slots.get(&dst).cloned() {
            self.emit_line(&format!("{slot} = {value};"));
        } else {
            let d = self.render_pointer_operand(&dst);
            self.emit_line(&format!("unsafe {{ *{d} = {value}; }}"));
        }
    }

    /// Resolve the by-value source of a `cir.copy`: a numeric/char const global
    /// renders to an array literal (padded to the destination length), while an
    /// aggregate local relies on the `Copy` derive of arrays and `#[repr(C)]`
    /// structs. Returns `None` when the source is opaque (raw pointer copy).
    fn copy_source_value(&self, dst: &str, src: &str) -> Option<String> {
        let dst_len = self
            .slot_types
            .get(dst)
            .and_then(|ty| parse_rust_array_type(ty))
            .map(|(_, len)| len as usize);
        match self.values.get(src) {
            Some(Val::Global(name)) => {
                if let Some(bytes) = self.parent.strings.get(name) {
                    let elems: Vec<String> = bytes.iter().map(|b| b.to_string()).collect();
                    Some(render_array_literal(&elems, dst_len.unwrap_or(elems.len())))
                } else if let Some(elems) = self.parent.const_arrays.get(name) {
                    Some(render_array_literal(elems, dst_len.unwrap_or(elems.len())))
                } else if let Some(raw) = self.parent.const_aggregates.get(name) {
                    let ty = self.slot_types.get(dst)?;
                    self.render_const_value(ty, raw)
                } else if self.parent.const_zero_globals.contains(name) {
                    self.slot_types.get(dst).map(|ty| self.default_value(ty))
                } else {
                    None
                }
            }
            _ => self
                .slots
                .contains_key(src)
                .then(|| self.render_operand(src)),
        }
    }

    fn render_const_value(&self, rust_ty: &str, raw: &str) -> Option<String> {
        self.parent.render_const_value(rust_ty, raw)
    }

    fn lower_load(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(ptr) = op.operands.first() else {
            return;
        };
        let value = if attr_bool(op, "is_volatile") {
            format!(
                "unsafe {{ std::ptr::read_volatile({}) }}",
                self.load_address(ptr)
            )
        } else if let Some(global) = self.global_name(ptr) {
            format!("unsafe {{ {global} }}")
        } else if let Some(member) = self.member_ptrs.get(ptr) {
            format!("unsafe {{ {}.{} }}", member.base, member.field)
        } else if let Some(element) = self.element_ptrs.get(ptr) {
            let place = format!("{}[({}) as usize]", element.base, element.index);
            if element.unsafe_access {
                format!("unsafe {{ {place} }}")
            } else {
                place
            }
        } else if let Some(slot) = self.slots.get(ptr) {
            slot.clone()
        } else {
            format!("unsafe {{ *({}) }}", self.render_operand(ptr))
        };
        self.materialize(result, value, op_result_type(op));
    }

    fn load_address(&self, ptr: &str) -> String {
        if let Some(member) = self.member_ptrs.get(ptr) {
            format!("std::ptr::addr_of!({}.{})", member.base, member.field)
        } else if let Some(element) = self.element_ptrs.get(ptr) {
            format!(
                "std::ptr::addr_of!({}[({}) as usize])",
                element.base, element.index
            )
        } else if let Some(slot) = self.slots.get(ptr) {
            format!("std::ptr::addr_of!({slot})")
        } else if let Some(global) = self.global_name(ptr) {
            format!("std::ptr::addr_of!({global})")
        } else {
            self.render_operand(ptr)
        }
    }

    fn store_address(&self, ptr: &str) -> String {
        if let Some(member) = self.member_ptrs.get(ptr) {
            format!("std::ptr::addr_of_mut!({}.{})", member.base, member.field)
        } else if let Some(element) = self.element_ptrs.get(ptr) {
            format!(
                "std::ptr::addr_of_mut!({}[({}) as usize])",
                element.base, element.index
            )
        } else if let Some(slot) = self.slots.get(ptr) {
            format!("std::ptr::addr_of_mut!({slot})")
        } else if let Some(global) = self.global_name(ptr) {
            format!("std::ptr::addr_of_mut!({global})")
        } else {
            self.render_operand(ptr)
        }
    }

    fn global_name(&self, ptr: &str) -> Option<String> {
        let Some(Val::Global(name)) = self.values.get(ptr) else {
            return None;
        };
        let name = sanitize_ident(name);
        (self.parent.globals.contains_key(&name) || self.parent.extern_globals.contains_key(&name))
            .then_some(name)
    }

    fn lower_const(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let raw = attr_str(op, "value").unwrap_or("");
        // MLIR may print a const as an attribute alias (e.g. `#false`); expand it.
        let raw = self.parent.aliases.get(raw).map_or(raw, String::as_str);
        let result_ty = op_result_type(op);
        if let Some((re, im)) = parse_cir_const_complex(raw) {
            self.materialize(
                result,
                format!("Complex {{ re: {re}, im: {im} }}"),
                result_ty,
            );
            return;
        }
        if let Some(b) = parse_cir_bool(raw) {
            self.materialize(result, b.to_string(), result_ty);
            return;
        }
        if raw.starts_with("#cir.ptr<null>") {
            self.materialize(result, "std::ptr::null_mut()".into(), result_ty);
            return;
        }
        let value = parse_cir_int(raw)
            .map(|n| n.to_string())
            .or_else(|| parse_cir_fp(raw))
            .unwrap_or_else(|| "0".into());
        let value = if result_ty.is_some_and(is_long_double) {
            format!("{LONG_DOUBLE_TY}({value})")
        } else {
            value
        };
        self.materialize(result, value, result_ty);
    }

    fn lower_complex_create(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        let re = self.render_operand(&op.operands[0]);
        let im = self.render_operand(&op.operands[1]);
        self.materialize(
            result,
            format!("Complex {{ re: {re}, im: {im} }}"),
            op_result_type(op),
        );
    }

    fn lower_complex_part(&mut self, op: &Op, field: &str) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(src) = op.operands.first() else {
            return;
        };
        let value = format!("{}.{field}", self.render_operand(src));
        self.materialize(result, value, op_result_type(op));
    }

    // cir.select(cond, t, f) is a pure value pick; all three operands are already
    // materialized, so it collapses to a Rust `if` expression.
    fn lower_select(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 3 {
            return;
        }
        let cond = self.render_operand(&op.operands[0]);
        let t = self.render_operand(&op.operands[1]);
        let f = self.render_operand(&op.operands[2]);
        self.materialize(
            result,
            format!("if {cond} {{ {t} }} else {{ {f} }}"),
            op_result_type(op),
        );
    }

    // cir.ternary has two value-yielding regions; clang emits it for the NaN-recovery
    // arm of complex `*` (the taken branch calls __muldc3). Lower to an `if` whose
    // block bodies run each region's ops and tail-yield the region result.
    fn lower_ternary(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(cond) = op.operands.first() else {
            return;
        };
        if op.regions.len() < 2 {
            self.emit_expr("todo!(\"cir.ternary\")".into());
            return;
        }
        let cond = self.render_operand(cond);
        let name = self.next_temp();
        let ty = op_result_type(op)
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or_else(|| "i32".into());
        self.emit_line(&format!("let {name}: {ty} = if {cond} {{"));
        self.indent += 1;
        let t = self.lower_yield_region(&op.regions[0]);
        self.emit_line(&t);
        self.indent -= 1;
        self.emit_line("} else {");
        self.indent += 1;
        let f = self.lower_yield_region(&op.regions[1]);
        self.emit_line(&f);
        self.indent -= 1;
        self.emit_line("};");
        self.values.insert(result.to_string(), Val::Expr(name));
    }

    // Lower every op in a region, capturing the terminating cir.yield's operand as
    // the region's tail value instead of lowering the yield itself.
    fn lower_yield_region(&mut self, region: &Region) -> String {
        let mut yielded = String::new();
        for block in &region.blocks {
            for op in &block.ops {
                if op.name == "cir.yield" {
                    if let Some(operand) = op.operands.first() {
                        yielded = self.render_operand(operand);
                    }
                } else {
                    self.lower_op(op);
                }
            }
        }
        yielded
    }

    fn lower_binary(&mut self, op: &Op, rust_op: &str) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        let lhs = self.render_operand(&op.operands[0]);
        let rhs = self.render_operand(&op.operands[1]);
        self.materialize(
            result,
            format!("(({lhs}) {rust_op} ({rhs}))"),
            op_result_type(op),
        );
    }

    // The batch crate builds with `overflow-checks = false`, so plain `+`/`-`/`*`
    // wrap two's-complement just like clang's `-O0` C — no `wrapping_*` needed.
    // `/` and `%` still trap on div-by-zero and INT_MIN/-1 on both sides, so the
    // generator avoids those.
    fn lower_int_arith(&mut self, op: &Op, rust_op: &str) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        let lhs = self.render_operand(&op.operands[0]);
        let rhs = self.render_operand(&op.operands[1]);
        let ty = op_result_type(op);
        self.materialize(result, format!("(({lhs}) {rust_op} ({rhs}))"), ty);
    }

    fn lower_overflow_arith(&mut self, op: &Op, rust_method: &str) {
        if op.results.len() < 2 || op.operands.len() < 2 {
            return;
        }
        let lhs = self.render_operand(&op.operands[0]);
        let rhs = self.render_operand(&op.operands[1]);
        let result_types = op_result_types(op);
        let pair = self.next_temp();
        self.emit_line(&format!("let {pair} = ({lhs}).{rust_method}({rhs});"));
        self.materialize(
            &op.results[0],
            format!("{pair}.0"),
            result_types.first().copied(),
        );
        self.materialize(
            &op.results[1],
            format!("{pair}.1"),
            result_types.get(1).copied(),
        );
    }

    fn lower_step(&mut self, op: &Op, rust_op: &str) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(value) = op.operands.first() else {
            return;
        };
        let value = self.render_operand(value);
        let ty = op_result_type(op);
        self.materialize(result, format!("({value} {rust_op} 1)"), ty);
    }

    // cir.shift carries the isShiftleft unit attr for `<<`; its absence means `>>`.
    // Rust's `>>` is arithmetic on signed and logical on unsigned, matching C by type.
    fn lower_shift(&mut self, op: &Op) {
        let rust_op = if attr_bool(op, "isShiftleft") {
            "<<"
        } else {
            ">>"
        };
        self.lower_int_arith(op, rust_op);
    }

    // cir.not is C's unary `~`; Rust spells integer bitwise complement `!`.
    fn lower_not(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(value) = op.operands.first() else {
            return;
        };
        let value = self.render_operand(value);
        let ty = op_result_type(op);
        self.materialize(result, format!("(!({value}))"), ty);
    }

    fn lower_neg(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(value) = op.operands.first() else {
            return;
        };
        let value = self.render_operand(value);
        let result_ty = op_result_type(op);
        let operand_ty = op_operand_types(op.ty.as_deref().unwrap_or(""))
            .into_iter()
            .next()
            .unwrap_or("");
        let rust_ty = result_ty
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or_else(|| "i32".into());
        let expr = if rust_ty == LONG_DOUBLE_TY {
            format!("{LONG_DOUBLE_TY}(-({value}).0)")
        } else if operand_ty == "!cir.bool" {
            format!("-(({value}) as {rust_ty})")
        } else {
            format!("(-({value}))")
        };
        self.materialize(result, expr, result_ty);
    }

    fn lower_abs(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(value) = op.operands.first() else {
            return;
        };
        let value = self.render_operand(value);
        let result_ty = op_result_type(op);
        let rust_ty = result_ty
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or_else(|| "i32".into());
        let expr = if matches!(rust_ty.as_str(), "i8" | "i16" | "i32" | "i64") {
            format!("({value}).wrapping_abs()")
        } else if rust_ty == LONG_DOUBLE_TY {
            format!("{LONG_DOUBLE_TY}(({value}).0.abs())")
        } else {
            format!("({value}).abs()")
        };
        self.materialize(result, expr, result_ty);
    }

    fn lower_unary_method(&mut self, op: &Op, method: &str) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(value) = op.operands.first() else {
            return;
        };
        let value = self.render_operand(value);
        let result_ty = op_result_type(op);
        let rust_ty = result_ty
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or_else(|| "f64".into());
        let expr = if rust_ty == LONG_DOUBLE_TY {
            format!("{LONG_DOUBLE_TY}(({value}).0.{method}())")
        } else {
            format!("({value}).{method}()")
        };
        self.materialize(result, expr, result_ty);
    }

    fn lower_binary_method(&mut self, op: &Op, method: &str) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        let lhs = self.render_operand(&op.operands[0]);
        let rhs = self.render_operand(&op.operands[1]);
        let result_ty = op_result_type(op);
        let rust_ty = result_ty
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or_else(|| "f64".into());
        let expr = if rust_ty == LONG_DOUBLE_TY {
            format!("{LONG_DOUBLE_TY}(({lhs}).0.{method}(({rhs}).0))")
        } else {
            format!("({lhs}).{method}({rhs})")
        };
        self.materialize(result, expr, result_ty);
    }

    fn lower_signbit(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(value) = op.operands.first() else {
            return;
        };
        let operand_ty = op
            .ty
            .as_deref()
            .and_then(|ty| op_operand_types(ty).into_iter().next());
        let value = self.render_float_predicate_operand(value, operand_ty);
        self.materialize(
            result,
            format!("({value}).is_sign_negative()"),
            op_result_type(op),
        );
    }

    fn lower_is_fp_class(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(value) = op.operands.first() else {
            return;
        };
        let Some(flags) = attr_int(op, "flags") else {
            return;
        };
        let operand_ty = op
            .ty
            .as_deref()
            .and_then(|ty| op_operand_types(ty).into_iter().next());
        let value = self.render_float_predicate_operand(value, operand_ty);
        let mut parts = Vec::new();
        if flags & 0x3 != 0 {
            parts.push(format!("({value}).is_nan()"));
        }
        if flags & 0x4 != 0 {
            parts.push(format!("({value}) == f64::NEG_INFINITY"));
        }
        if flags & 0x8 != 0 {
            parts.push(format!(
                "({value}).is_normal() && ({value}).is_sign_negative()"
            ));
        }
        if flags & 0x10 != 0 {
            parts.push(format!(
                "({value}).is_subnormal() && ({value}).is_sign_negative()"
            ));
        }
        if flags & 0x20 != 0 {
            parts.push(format!("({value}) == 0.0 && ({value}).is_sign_negative()"));
        }
        if flags & 0x40 != 0 {
            parts.push(format!("({value}) == 0.0 && !({value}).is_sign_negative()"));
        }
        if flags & 0x80 != 0 {
            parts.push(format!(
                "({value}).is_subnormal() && !({value}).is_sign_negative()"
            ));
        }
        if flags & 0x100 != 0 {
            parts.push(format!(
                "({value}).is_normal() && !({value}).is_sign_negative()"
            ));
        }
        if flags & 0x200 != 0 {
            parts.push(format!("({value}) == f64::INFINITY"));
        }
        let expr = if parts.is_empty() {
            "false".into()
        } else {
            parts.join(" || ")
        };
        self.materialize(result, expr, op_result_type(op));
    }

    fn render_float_predicate_operand(&self, operand: &str, ty: Option<&str>) -> String {
        let value = self.render_operand(operand);
        match ty {
            Some(ty) if is_long_double(ty) => format!("({value}).0"),
            Some("!cir.float") => format!("({value}) as f64"),
            _ => value,
        }
    }

    fn lower_modf(&mut self, op: &Op) {
        if op.results.len() < 2 {
            return;
        }
        let Some(value) = op.operands.first() else {
            return;
        };
        let value = self.render_operand(value);
        let result_types = op_result_types(op);
        self.materialize(
            &op.results[0],
            format!("({value}).fract()"),
            result_types.first().copied(),
        );
        self.materialize(
            &op.results[1],
            format!("({value}).trunc()"),
            result_types.get(1).copied(),
        );
    }

    fn lower_cmp(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        let lhs = self.render_operand(&op.operands[0]);
        let rhs = self.render_operand(&op.operands[1]);
        let cmp = match attr_int(op, "kind") {
            Some(0) => "<",
            Some(1) => "<=",
            Some(2) => ">",
            Some(3) => ">=",
            Some(4) => "==",
            Some(5) => "!=",
            _ => "<=",
        };
        self.materialize(
            result,
            format!("(({lhs}) {cmp} ({rhs}))"),
            Some("!cir.bool"),
        );
    }

    fn lower_get_global(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let name = attr_str(op, "name")
            .unwrap_or("")
            .trim_start_matches('@')
            .trim_matches('"')
            .to_string();
        self.values.insert(result.clone(), Val::Global(name));
    }

    /// Render a pointer operand as a Rust place (lvalue) expression when it names
    /// a known place (slot, struct member, array element, or global). Composing
    /// this lets nested aggregates — struct-of-array, array-of-struct — chain into
    /// a single Rust path like `b.data[i]` or `ps[i].x`.
    fn place_expr(&self, ptr: &str) -> Option<String> {
        if let Some(member) = self.member_ptrs.get(ptr) {
            Some(format!("{}.{}", member.base, member.field))
        } else if let Some(element) = self.element_ptrs.get(ptr) {
            Some(format!("{}[({}) as usize]", element.base, element.index))
        } else if let Some(slot) = self.slots.get(ptr) {
            Some(slot.clone())
        } else {
            self.global_name(ptr)
        }
    }

    fn lower_get_member(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(base_ptr) = op.operands.first() else {
            return;
        };
        let base = self
            .place_expr(base_ptr)
            .unwrap_or_else(|| format!("(*{})", self.render_pointer_operand(base_ptr)));
        let field = sanitize_ident(attr_str(op, "name").unwrap_or(result));
        let unsafe_access = self.ptr_requires_unsafe(base_ptr) || self.op_base_is_union(op);
        self.member_ptrs.insert(
            result.clone(),
            MemberPtr {
                base,
                field,
                unsafe_access,
            },
        );
    }

    fn lower_get_element(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        let base_ptr = &op.operands[0];
        let base = self
            .place_expr(base_ptr)
            .unwrap_or_else(|| format!("(*{})", self.render_pointer_operand(base_ptr)));
        let index = self.render_operand(&op.operands[1]);
        let unsafe_access = self.ptr_requires_unsafe(base_ptr);
        self.element_ptrs.insert(
            result.clone(),
            ElementPtr {
                base,
                index,
                unsafe_access,
            },
        );
    }

    fn ptr_requires_unsafe(&self, ptr: &str) -> bool {
        self.global_name(ptr).is_some()
            || self
                .member_ptrs
                .get(ptr)
                .is_some_and(|member| member.unsafe_access)
            || self
                .element_ptrs
                .get(ptr)
                .is_some_and(|element| element.unsafe_access)
    }

    fn op_base_is_union(&self, op: &Op) -> bool {
        op.ty
            .as_deref()
            .and_then(|ty| op_operand_types(ty).into_iter().next())
            .and_then(cir_ptr_inner)
            .is_some_and(|ty| self.parent.cir_type_is_union(ty))
    }

    fn lower_cast(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(src) = op.operands.first() else {
            return;
        };
        // array-decay of a `va_list` local keeps referring to the same slot.
        if let Some(slot) = self.va_places.get(src).cloned() {
            self.va_places.insert(result.clone(), slot.clone());
            self.values.insert(result.clone(), Val::Expr(slot));
            return;
        }
        let result_ty = op_result_type(op).unwrap_or("");
        let operand_ty = op_operand_types(op.ty.as_deref().unwrap_or(""))
            .into_iter()
            .next()
            .unwrap_or("");
        let value = match self.values.get(src).cloned() {
            // array-to-pointer decay of a numeric/wide-string const global: render
            // the elements as a (statically promoted) array literal and take its
            // pointer, since the global has no Rust name of its own.
            Some(Val::Global(name))
                if result_ty.starts_with("!cir.ptr<")
                    && self.parent.const_arrays.contains_key(&name) =>
            {
                let elems = &self.parent.const_arrays[&name];
                let (elem_ty, len) = cir_ptr_inner(operand_ty)
                    .and_then(parse_cir_array_type)
                    .map_or(("i32".to_string(), elems.len()), |(elem, len)| {
                        (self.parent.rust_type(&elem), len as usize)
                    });
                // annotate one element so the array element type can't default wrong
                let mut typed: Vec<String> = elems.clone();
                if let Some(first) = typed.first_mut() {
                    *first = format!("{first} as {elem_ty}");
                }
                let arr = render_array_literal(&typed, len);
                let ptr_ty = self.parent.rust_type(result_ty);
                Val::Expr(format!("{arr}.as_ptr() as {ptr_ty}"))
            }
            Some(Val::Global(name)) => Val::Global(name),
            _ if self
                .slot_types
                .get(src)
                .is_some_and(|ty| parse_rust_array_type(ty).is_some()) =>
            {
                Val::Expr(format!("{}.as_mut_ptr()", self.render_operand(src)))
            }
            _ if is_long_double(result_ty) && !is_long_double(operand_ty) => Val::Expr(format!(
                "{LONG_DOUBLE_TY}({} as f64)",
                self.render_operand(src)
            )),
            _ if is_long_double(operand_ty) && result_ty == "!cir.bool" => {
                Val::Expr(format!("({}.0 != 0.0)", self.render_operand(src)))
            }
            _ if is_long_double(operand_ty) && !is_long_double(result_ty) => Val::Expr(format!(
                "({}.0 as {})",
                self.render_operand(src),
                self.parent.rust_type(result_ty)
            )),
            _ if result_ty.starts_with("!cir.ptr<") && operand_ty.starts_with("!cir.ptr<") => {
                Val::Expr(format!(
                    "{} as {}",
                    self.render_pointer_operand(src),
                    self.parent.rust_type(result_ty)
                ))
            }
            // integer sentinel (SIG_IGN/SIG_DFL/SIG_ERR = (void(*)(int))N) cast to a
            // fn pointer: `as` cannot target Option<fn(..)>, so reinterpret the bits.
            _ if result_ty.starts_with("!cir.ptr<!cir.func<") => {
                let ptr_ty = self.parent.rust_type(result_ty);
                Val::Expr(format!(
                    "unsafe {{ std::mem::transmute::<usize, {ptr_ty}>(({}) as usize) }}",
                    self.render_operand(src)
                ))
            }
            _ if result_ty == "!cir.bool" && operand_ty != "!cir.bool" => Val::Expr(format!(
                "({} != {})",
                self.render_operand(src),
                zero_for_cir_type(operand_ty)
            )),
            _ if result_ty == operand_ty => Val::Expr(self.render_operand(src)),
            _ => Val::Expr(format!(
                "({}) as {}",
                self.render_operand(src),
                self.parent.rust_type(result_ty)
            )),
        };
        self.values.insert(result.clone(), value);
    }

    fn lower_ptr_diff(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        let lhs = self.render_operand(&op.operands[0]);
        let rhs = self.render_operand(&op.operands[1]);
        let ty = op_result_type(op)
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or_else(|| "i64".into());
        self.materialize(
            result,
            format!("unsafe {{ {lhs}.offset_from({rhs}) as {ty} }}"),
            op_result_type(op),
        );
    }

    fn lower_ptr_stride(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        let base = self.render_operand(&op.operands[0]);
        let index = self.render_operand(&op.operands[1]);
        self.values.insert(
            result.clone(),
            Val::Expr(format!("unsafe {{ {base}.offset({index} as isize) }}")),
        );
    }

    fn lower_call(&mut self, op: &Op) {
        let operand_types = op_operand_types(op.ty.as_deref().unwrap_or(""));
        let direct_callee =
            attr_str(op, "callee").map(|callee| callee.trim_start_matches('@').to_string());
        let (callee, arg_operands, arg_types) = if let Some(callee) = direct_callee {
            (callee, op.operands.as_slice(), operand_types.as_slice())
        } else {
            let Some((callee_operand, arg_operands)) = op.operands.split_first() else {
                return;
            };
            let callee = self.render_operand(callee_operand);
            (
                format!("{callee}.unwrap()"),
                arg_operands,
                operand_types.get(1..).unwrap_or(&[]),
            )
        };
        let args = arg_operands
            .iter()
            .zip(arg_types.iter().copied())
            .map(|(operand, ty)| self.render_call_arg(operand, ty))
            .collect::<Vec<_>>();
        if callee == "strtold"
            && self
                .parent
                .extern_returns
                .get(&callee)
                .and_then(|ret| ret.as_deref())
                == Some(LONG_DOUBLE_TY)
        {
            if let Some(result) = op.results.first() {
                let name = self.next_temp();
                let a0 = args
                    .first()
                    .cloned()
                    .unwrap_or_else(|| "std::ptr::null_mut()".into());
                let a1 = args
                    .get(1)
                    .cloned()
                    .unwrap_or_else(|| "std::ptr::null_mut()".into());
                self.emit_line(&format!(
                    "let mut {name}: {LONG_DOUBLE_TY} = {LONG_DOUBLE_TY}(0.0);"
                ));
                self.emit_line(&format!(
                    "unsafe {{ __slate_strtold({a0} as *mut i8, {a1} as *mut *mut i8, std::ptr::addr_of_mut!({name})); }}"
                ));
                self.values.insert(result.to_string(), Val::Expr(name));
            }
            return;
        }
        if callee == "printf"
            && arg_types.iter().any(|ty| is_long_double(ty))
            && args.len() == 3
            && arg_types.get(1).is_some_and(|ty| is_long_double(ty))
            && arg_types.get(2).is_some_and(|ty| *ty == "!s32i")
        {
            let expr = format!(
                "unsafe {{ __slate_printf_ld_i32({} as *mut i8, std::ptr::addr_of!({}) as *const LongDouble, {} as i32) }}",
                args[0], args[1], args[2]
            );
            if let Some(result) = op.results.first() {
                self.materialize(result, expr, op_result_type(op));
            } else {
                self.emit_expr(expr);
            }
            return;
        }
        let expr = if is_complex_runtime_call(&callee) {
            self.parent.uses_complex.set(true);
            format!("unsafe {{ {callee}({}) }}", args.join(", "))
        } else if let Some(param_types) = self.parent.externs.get(&callee).cloned() {
            let args = args
                .iter()
                .enumerate()
                .map(|(i, arg)| match param_types.get(i) {
                    // function-pointer params (Option<fn..>) can't be `as`-cast;
                    // let fn-item -> fn-ptr coercion handle them.
                    Some(_)
                        if arg_types
                            .get(i)
                            .is_some_and(|t| is_cir_function_pointer_type(t)) =>
                    {
                        arg.clone()
                    }
                    Some(ty) => format!("{arg} as {ty}"),
                    None => arg.clone(),
                })
                .collect::<Vec<_>>();
            format!("unsafe {{ {callee}({}) }}", args.join(", "))
        } else if self.parent.variadic_defs.contains(&callee) {
            format!("unsafe {{ {callee}({}) }}", args.join(", "))
        } else {
            format!("{callee}({})", args.join(", "))
        };

        if let Some(result) = op.results.first() {
            self.materialize(result, expr, op_result_type(op));
        } else {
            self.emit_expr(expr);
        }
    }

    // Atomic RMW/fence ops lower to plain non-atomic read-modify-write through
    // `store_address` (a `*mut T`). Single-threaded differential testing makes
    // the atomic/ordering semantics unobservable; real atomics are a later fixup.
    fn atomic_rust_type(&self, op: &Op) -> String {
        op_result_type(op)
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or_else(|| "i32".into())
    }

    fn lower_atomic_fetch(&mut self, op: &Op) {
        let Some(result) = op.results.first().cloned() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        let addr = self.store_address(&op.operands[0]);
        let val = self.render_operand(&op.operands[1]);
        let ty = self.atomic_rust_type(op);
        let old = self.next_temp();
        self.emit_line(&format!("let {old}: {ty} = unsafe {{ *{addr} }};"));
        let combined = match attr_int(op, "binop").unwrap_or(0) {
            0 => format!("({old} + ({val}))"),
            1 => format!("({old} - ({val}))"),
            2 => format!("({old} & ({val}))"),
            3 => format!("({old} ^ ({val}))"),
            4 => format!("({old} | ({val}))"),
            5 => format!("(!({old} & ({val})))"),
            6 => format!("({old}).max({val})"),
            _ => format!("({old}).min({val})"),
        };
        let new = self.next_temp();
        self.emit_line(&format!("let {new}: {ty} = {combined};"));
        self.emit_line(&format!("unsafe {{ *{addr} = {new}; }}"));
        // `fetch_first` returns the pre-op value; its absence returns the new value.
        let bound = if attr_bool(op, "fetch_first") {
            old
        } else {
            new
        };
        self.values.insert(result, Val::Expr(bound));
    }

    fn lower_atomic_xchg(&mut self, op: &Op) {
        let Some(result) = op.results.first().cloned() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        let addr = self.store_address(&op.operands[0]);
        let val = self.render_operand(&op.operands[1]);
        let ty = self.atomic_rust_type(op);
        let old = self.next_temp();
        self.emit_line(&format!("let {old}: {ty} = unsafe {{ *{addr} }};"));
        self.emit_line(&format!("unsafe {{ *{addr} = {val}; }}"));
        self.values.insert(result, Val::Expr(old));
    }

    fn lower_atomic_cmpxchg(&mut self, op: &Op) {
        if op.operands.len() < 3 || op.results.len() < 2 {
            return;
        }
        let addr = self.store_address(&op.operands[0]);
        let expected = self.render_operand(&op.operands[1]);
        let desired = self.render_operand(&op.operands[2]);
        let ty = op_result_types(op)
            .first()
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or_else(|| "i32".into());
        let old = self.next_temp();
        let ok = self.next_temp();
        self.emit_line(&format!("let {old}: {ty} = unsafe {{ *{addr} }};"));
        self.emit_line(&format!("let {ok}: bool = {old} == ({expected});"));
        self.emit_line(&format!("if {ok} {{ unsafe {{ *{addr} = {desired}; }} }}"));
        self.values.insert(op.results[0].clone(), Val::Expr(old));
        self.values.insert(op.results[1].clone(), Val::Expr(ok));
    }

    fn lower_atomic_fence(&mut self) {
        self.emit_line("std::sync::atomic::fence(std::sync::atomic::Ordering::SeqCst);");
    }

    fn lower_va_start(&mut self, op: &Op) {
        let Some(ptr) = op.operands.first() else {
            return;
        };
        let Some(slot) = self.va_places.get(ptr).cloned() else {
            return;
        };
        let args = self
            .va_args_param
            .clone()
            .unwrap_or_else(|| "__slate_va_args".into());
        self.emit_line(&format!("{slot} = {args}.clone();"));
    }

    fn lower_va_arg(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(ptr) = op.operands.first() else {
            return;
        };
        let Some(slot) = self.va_places.get(ptr).cloned() else {
            return;
        };
        let ty = op_result_type(op)
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or_else(|| "i32".into());
        self.materialize(
            result,
            format!("unsafe {{ {slot}.next_arg::<{ty}>() }}"),
            op_result_type(op),
        );
    }

    fn lower_return(&mut self, op: &Op) {
        let value = op
            .operands
            .first()
            .map(|operand| self.render_operand(operand));
        if self.is_main {
            let code = value.unwrap_or_else(|| "0".into());
            self.emit_line(&format!("std::process::exit({code} as i32);"));
        } else if let Some(value) = value {
            self.emit_line(&format!("return {value};"));
        } else {
            self.emit_line("return;");
        }
    }

    fn lower_scope(&mut self, op: &Op) {
        self.emit_line("{");
        self.indent += 1;
        for region in &op.regions {
            self.lower_region_ops(region);
        }
        self.indent -= 1;
        self.emit_line("}");
    }

    fn lower_if(&mut self, op: &Op) {
        let Some(cond) = op.operands.first() else {
            self.emit_expr("todo!(\"cir.if\")".into());
            return;
        };
        let cond = self.render_operand(cond);
        self.emit_line(&format!("if {cond} {{"));
        self.indent += 1;
        if let Some(region) = op.regions.first() {
            self.lower_region_ops(region);
        }
        self.indent -= 1;
        let has_else = op
            .regions
            .get(1)
            .is_some_and(|region| region.blocks.iter().any(|block| !block.ops.is_empty()));
        if has_else {
            self.emit_line("} else {");
            self.indent += 1;
            self.lower_region_ops(&op.regions[1]);
            self.indent -= 1;
        }
        self.emit_line("}");
    }

    fn lower_switch(&mut self, op: &Op) {
        let Some(selector) = op.operands.first() else {
            self.emit_expr("todo!(\"cir.switch\")".into());
            return;
        };
        let Some(region) = op.regions.first() else {
            self.emit_expr("todo!(\"cir.switch\")".into());
            return;
        };
        let cases: Vec<_> = region
            .blocks
            .iter()
            .flat_map(|block| &block.ops)
            .filter(|op| op.name == "cir.case")
            .filter_map(switch_case)
            .collect();
        if cases.is_empty() {
            return;
        }

        let n = self.label_counter;
        self.label_counter += 1;
        let label = format!("'__switch{n}");
        let selector_name = format!("__switch_value{n}");
        let case_name = format!("__switch_case{n}");
        let default_index = cases.iter().position(|case| case.is_default);
        let fallback = default_index
            .map(|index| index.to_string())
            .unwrap_or_else(|| "-1".into());
        let selector = self.render_operand(selector);

        self.emit_line("{");
        self.indent += 1;
        self.emit_line(&format!("let {selector_name} = {selector};"));
        self.emit_line(&format!(
            "let mut {case_name}: i32 = match {selector_name} {{"
        ));
        self.indent += 1;
        for (index, case) in cases.iter().enumerate() {
            for value in &case.values {
                self.emit_line(&format!("{value} => {index},"));
            }
        }
        self.emit_line(&format!("_ => {fallback},"));
        self.indent -= 1;
        self.emit_line("};");
        self.emit_line(&format!("{label}: loop {{"));
        self.indent += 1;
        self.emit_line(&format!("match {case_name} {{"));
        self.indent += 1;
        self.loop_stack.push(LoopFrame {
            break_label: Some(label.clone()),
            continue_label: None,
            is_loop: false,
        });
        for (index, case) in cases.iter().enumerate() {
            self.emit_line(&format!("{index} => {{"));
            self.indent += 1;
            self.lower_region_ops(case.region);
            if !region_ends_control_flow(case.region) {
                if index + 1 < cases.len() {
                    self.emit_line(&format!("{case_name} = {};", index + 1));
                    self.emit_line(&format!("continue {label};"));
                } else {
                    self.emit_line(&format!("break {label};"));
                }
            }
            self.indent -= 1;
            self.emit_line("}");
        }
        self.loop_stack.pop();
        self.emit_line(&format!("_ => break {label},"));
        self.indent -= 1;
        self.emit_line("}");
        self.indent -= 1;
        self.emit_line("}");
        self.indent -= 1;
        self.emit_line("}");
    }

    fn lower_for(&mut self, op: &Op) {
        if op.regions.len() < 3 {
            self.emit_expr("todo!(\"cir.for\")".into());
            return;
        }
        // C `continue` must still run the step region, so wrap the body in a
        // labeled block (`continue` -> `break 'continue`). Rust then forbids an
        // unlabeled `break` from diverging through that block, so the loop is
        // labeled too and `break` targets it.
        let (break_label, continue_label) = if region_has_direct_continue(&op.regions[1]) {
            let n = self.label_counter;
            self.label_counter += 1;
            (Some(format!("'__loop{n}")), Some(format!("'__continue{n}")))
        } else {
            (None, None)
        };
        match &break_label {
            Some(label) => self.emit_line(&format!("{label}: loop {{")),
            None => self.emit_line("loop {"),
        }
        self.indent += 1;
        let cond = self.lower_condition_region(&op.regions[0]);
        self.emit_line(&format!("if !({cond}) {{"));
        self.indent += 1;
        self.emit_line("break;");
        self.indent -= 1;
        self.emit_line("}");
        if let Some(label) = &continue_label {
            self.emit_line(&format!("{label}: {{"));
            self.indent += 1;
        }
        self.loop_stack.push(LoopFrame {
            break_label,
            continue_label: continue_label.clone(),
            is_loop: true,
        });
        self.lower_region_ops(&op.regions[1]);
        self.loop_stack.pop();
        if continue_label.is_some() {
            self.indent -= 1;
            self.emit_line("}");
        }
        self.lower_region_ops(&op.regions[2]);
        self.indent -= 1;
        self.emit_line("}");
    }

    fn lower_while(&mut self, op: &Op) {
        if op.regions.len() < 2 {
            self.emit_expr("todo!(\"cir.while\")".into());
            return;
        }
        self.emit_line("loop {");
        self.indent += 1;
        let cond = self.lower_condition_region(&op.regions[0]);
        self.emit_line(&format!("if !({cond}) {{"));
        self.indent += 1;
        self.emit_line("break;");
        self.indent -= 1;
        self.emit_line("}");
        // A while loop has no step region, so plain Rust `break`/`continue` match C.
        self.loop_stack.push(LoopFrame {
            break_label: None,
            continue_label: None,
            is_loop: true,
        });
        self.lower_region_ops(&op.regions[1]);
        self.loop_stack.pop();
        self.indent -= 1;
        self.emit_line("}");
    }

    fn lower_do(&mut self, op: &Op) {
        if op.regions.len() < 2 {
            self.emit_expr("todo!(\"cir.do\")".into());
            return;
        }
        let (break_label, continue_label) = if region_has_direct_continue(&op.regions[0]) {
            let n = self.label_counter;
            self.label_counter += 1;
            (Some(format!("'__loop{n}")), Some(format!("'__continue{n}")))
        } else {
            (None, None)
        };
        match &break_label {
            Some(label) => self.emit_line(&format!("{label}: loop {{")),
            None => self.emit_line("loop {"),
        }
        self.indent += 1;
        if let Some(label) = &continue_label {
            self.emit_line(&format!("{label}: {{"));
            self.indent += 1;
        }
        self.loop_stack.push(LoopFrame {
            break_label: break_label.clone(),
            continue_label: continue_label.clone(),
            is_loop: true,
        });
        self.lower_region_ops(&op.regions[0]);
        self.loop_stack.pop();
        if continue_label.is_some() {
            self.indent -= 1;
            self.emit_line("}");
        }
        let cond = self.lower_condition_region(&op.regions[1]);
        self.emit_line(&format!("if !({cond}) {{"));
        self.indent += 1;
        match &break_label {
            Some(label) => self.emit_line(&format!("break {label};")),
            None => self.emit_line("break;"),
        }
        self.indent -= 1;
        self.emit_line("}");
        self.indent -= 1;
        self.emit_line("}");
    }

    fn lower_break(&mut self) {
        let label = self.loop_stack.last().and_then(|f| f.break_label.clone());
        match label {
            Some(label) => self.emit_line(&format!("break {label};")),
            None => self.emit_line("break;"),
        }
    }

    fn lower_continue(&mut self) {
        let label = self
            .loop_stack
            .iter()
            .rev()
            .find(|frame| frame.is_loop)
            .and_then(|f| f.continue_label.clone());
        match label {
            Some(label) => self.emit_line(&format!("break {label};")),
            None => self.emit_line("continue;"),
        }
    }

    /// Lower a function body with unstructured control flow into a state-machine
    /// dispatch loop. Each CIR block becomes a `match` arm keyed on a `__state`
    /// variable; `cir.br`/`cir.goto` set the next state and `continue` the loop.
    /// Allocas are hoisted above the loop so locals survive across block arms.
    fn lower_dispatch(&mut self, body: &Region) {
        let n = self.label_counter;
        self.label_counter += 1;
        let loop_label = format!("'__dispatch{n}");
        let state_var = format!("__state{n}");

        let mut label_to_state = BTreeMap::new();
        let mut block_to_state = BTreeMap::new();
        for (i, block) in body.blocks.iter().enumerate() {
            let key = block.label.clone().unwrap_or_else(|| format!("bb{i}"));
            block_to_state.insert(key, i);
            for op in &block.ops {
                if op.name == "cir.label" {
                    if let Some(label) = attr_str(op, "label") {
                        label_to_state.insert(label.to_string(), i);
                    }
                }
            }
        }
        self.dispatch = Some(DispatchCtx {
            loop_label: loop_label.clone(),
            state_var: state_var.clone(),
            label_to_state,
            block_to_state,
        });

        for block in &body.blocks {
            for op in &block.ops {
                if op.name == "cir.alloca" {
                    self.lower_alloca(op);
                    if let Some(result) = op.results.first() {
                        self.hoisted.insert(result.clone());
                    }
                }
            }
        }

        self.emit_line(&format!("let mut {state_var}: i32 = 0;"));
        self.emit_line(&format!("{loop_label}: loop {{"));
        self.indent += 1;
        self.emit_line(&format!("match {state_var} {{"));
        self.indent += 1;
        for (i, block) in body.blocks.iter().enumerate() {
            self.emit_line(&format!("{i} => {{"));
            self.indent += 1;
            self.lower_block(block);
            if !block_diverges(block) {
                self.emit_line(&format!("{state_var} = {};", i + 1));
                self.emit_line(&format!("continue {loop_label};"));
            }
            self.indent -= 1;
            self.emit_line("}");
        }
        self.emit_line(&format!("_ => break {loop_label},"));
        self.indent -= 1;
        self.emit_line("}");
        self.indent -= 1;
        self.emit_line("}");
        self.dispatch = None;
    }

    fn lower_goto(&mut self, op: &Op) {
        let Some(dispatch) = &self.dispatch else {
            return;
        };
        let target = attr_str(op, "label").and_then(|l| dispatch.label_to_state.get(l));
        match target {
            Some(&state) => {
                let stmt = format!(
                    "{} = {state};\ncontinue {};",
                    dispatch.state_var, dispatch.loop_label
                );
                self.emit_dispatch_jump(&stmt);
            }
            None => self.emit_expr("todo!(\"cir.goto: unknown label\")".into()),
        }
    }

    fn lower_br(&mut self, op: &Op) {
        let Some(dispatch) = &self.dispatch else {
            return;
        };
        let target = op
            .successors
            .first()
            .and_then(|bb| dispatch.block_to_state.get(bb));
        match target {
            Some(&state) => {
                let stmt = format!(
                    "{} = {state};\ncontinue {};",
                    dispatch.state_var, dispatch.loop_label
                );
                self.emit_dispatch_jump(&stmt);
            }
            None => self.emit_expr("todo!(\"cir.br: unknown successor\")".into()),
        }
    }

    fn emit_dispatch_jump(&mut self, stmt: &str) {
        for line in stmt.lines() {
            self.emit_line(line);
        }
    }

    fn lower_condition_region(&mut self, region: &Region) -> String {
        let mut condition = "true".to_string();
        for block in &region.blocks {
            for op in &block.ops {
                if op.name == "cir.condition" {
                    if let Some(operand) = op.operands.first() {
                        condition = self.render_operand(operand);
                    }
                } else {
                    self.lower_op(op);
                }
            }
        }
        condition
    }

    fn materialize(&mut self, result: &str, expr: String, cir_ty: Option<&str>) {
        let name = self.next_temp();
        let ty = cir_ty
            .map(|ty| self.parent.rust_type(ty))
            .unwrap_or_else(|| "i32".into());
        self.emit_line(&format!("let {name}: {ty} = {expr};"));
        self.values.insert(result.to_string(), Val::Expr(name));
    }

    fn render_operand(&self, operand: &str) -> String {
        self.values
            .get(operand)
            .map(|value| value.render(&self.parent.strings))
            .or_else(|| self.slots.get(operand).map(|slot| slot.clone()))
            .unwrap_or_else(|| sanitize_ident(operand))
    }

    fn render_pointer_operand(&self, operand: &str) -> String {
        if self.member_ptrs.contains_key(operand) || self.element_ptrs.contains_key(operand) {
            return self.store_address(operand);
        }
        if let Some(value) = self.values.get(operand) {
            return value.render(&self.parent.strings);
        }
        if let Some(slot) = self.slots.get(operand) {
            return if self
                .slot_types
                .get(operand)
                .is_some_and(|ty| parse_rust_array_type(ty).is_some())
            {
                format!("{slot}.as_mut_ptr()")
            } else {
                format!("std::ptr::addr_of_mut!({slot})")
            };
        }
        sanitize_ident(operand)
    }

    fn render_function_pointer_operand(&self, operand: &str) -> String {
        match self.values.get(operand) {
            Some(Val::Global(name)) if !self.parent.strings.contains_key(name) => {
                format!("Some({})", sanitize_ident(name))
            }
            Some(value) => value.render(&self.parent.strings),
            None => self.render_operand(operand),
        }
    }

    fn render_call_arg(&self, operand: &str, ty: &str) -> String {
        if is_cir_function_pointer_type(ty) {
            self.render_function_pointer_operand(operand)
        } else if ty.starts_with("!cir.ptr<") {
            self.render_pointer_operand(operand)
        } else {
            self.render_operand(operand)
        }
    }

    fn next_temp(&mut self) -> String {
        let name = format!("_v{}", self.temp_counter);
        self.temp_counter += 1;
        name
    }

    fn emit_expr(&mut self, expr: String) {
        self.emit_line(&format!("{expr};"));
    }

    fn emit_line(&mut self, line: &str) {
        self.out.push_str(&"    ".repeat(self.indent));
        self.out.push_str(line);
        self.out.push('\n');
    }

    fn pointee_type(&self, ty: &str) -> Option<String> {
        let ret = op_type_return(ty)?;
        ret.strip_prefix("!cir.ptr<")
            .and_then(|s| s.strip_suffix('>'))
            .map(|ty| self.parent.rust_type(ty))
    }

    fn default_value(&self, ty: &str) -> String {
        self.parent.default_value(ty)
    }
}

fn region_ops(op: &Op) -> Vec<&Op> {
    op.regions
        .iter()
        .flat_map(|region| region.blocks.iter())
        .flat_map(|block| block.ops.iter())
        .collect()
}

fn op_mentions_long_double(op: &Op) -> bool {
    op.ty
        .as_deref()
        .is_some_and(|ty| ty.contains("!cir.long_double"))
        || op
            .attrs
            .values()
            .filter_map(Attr::as_str)
            .any(|value| value.contains("!cir.long_double"))
        || op.regions.iter().any(|region| {
            region.blocks.iter().any(|block| {
                block
                    .args
                    .iter()
                    .any(|(_, ty)| ty.contains("!cir.long_double"))
                    || block.ops.iter().any(op_mentions_long_double)
            })
        })
}

fn attr_str<'a>(op: &'a Op, key: &str) -> Option<&'a str> {
    op.attrs.get(key).and_then(Attr::as_str)
}

fn attr_int(op: &Op, key: &str) -> Option<i64> {
    op.attrs.get(key).and_then(Attr::as_int)
}

fn attr_bool(op: &Op, key: &str) -> bool {
    op.attrs.contains_key(key)
}

fn switch_case(op: &Op) -> Option<SwitchCase<'_>> {
    let is_default = attr_int(op, "kind") == Some(0);
    let values = match op.attrs.get("value") {
        Some(Attr::Array(values)) => values
            .iter()
            .filter_map(|value| match value {
                Attr::Int(n) => Some(*n),
                Attr::Raw(raw) => parse_cir_int(raw).map(|n| n as i64),
                _ => None,
            })
            .collect(),
        _ => Vec::new(),
    };
    let region = op.regions.first()?;
    Some(SwitchCase {
        values,
        is_default,
        region,
    })
}

/// Whether a dispatch block ends in its own control transfer (so the dispatch
/// loop must not append a fall-through to the next state).
fn block_diverges(block: &Block) -> bool {
    block
        .ops
        .last()
        .is_some_and(|op| matches!(op.name.as_str(), "cir.return" | "cir.br" | "cir.goto"))
}

fn region_ends_control_flow(region: &Region) -> bool {
    region
        .blocks
        .iter()
        .rev()
        .flat_map(|block| block.ops.iter().rev())
        .find(|op| op.name != "cir.yield")
        .is_some_and(|op| {
            matches!(
                op.name.as_str(),
                "cir.break" | "cir.continue" | "cir.return"
            )
        })
}

fn op_result_type(op: &Op) -> Option<&str> {
    op.ty
        .as_deref()
        .and_then(split_top_level_arrow)
        .map(|(_, ret)| ret.trim())
}

fn op_result_types(op: &Op) -> Vec<&str> {
    let Some(ret) = op_result_type(op) else {
        return Vec::new();
    };
    let ret = ret.trim();
    if ret.starts_with('(') && ret.ends_with(')') {
        split_top_level(&ret[1..ret.len() - 1], ',')
            .into_iter()
            .map(str::trim)
            .filter(|ty| !ty.is_empty())
            .collect()
    } else {
        vec![ret]
    }
}

fn op_operand_types(ty: &str) -> Vec<&str> {
    let Some((params, _)) = split_top_level_arrow(ty) else {
        return Vec::new();
    };
    let params = params.trim().trim_start_matches('(').trim_end_matches(')');
    split_top_level(params, ',')
        .into_iter()
        .map(str::trim)
        .filter(|ty| !ty.is_empty())
        .collect()
}

fn cir_ptr_inner(ty: &str) -> Option<&str> {
    ty.trim()
        .strip_prefix("!cir.ptr<")
        .and_then(|ty| ty.strip_suffix('>'))
        .map(str::trim)
}

fn parse_function_type(s: &str) -> (Vec<String>, Option<String>) {
    let Some(inner) = s
        .strip_prefix("!cir.func<")
        .and_then(|s| s.strip_suffix('>'))
    else {
        return (Vec::new(), None);
    };
    let Some((params, ret)) = split_top_level_arrow(inner) else {
        return (Vec::new(), None);
    };
    let params = params.trim().trim_start_matches('(').trim_end_matches(')');
    let params = split_top_level(params, ',')
        .into_iter()
        .map(str::trim)
        .filter(|s| !s.is_empty() && *s != "...")
        .map(str::to_string)
        .collect();
    (params, Some(ret.trim().to_string()))
}

/// Whether a `!cir.func<..>` type ends its parameter list with `...`.
fn function_type_is_variadic(s: &str) -> bool {
    let Some(inner) = s
        .strip_prefix("!cir.func<")
        .and_then(|s| s.strip_suffix('>'))
    else {
        return false;
    };
    let params = split_top_level_arrow(inner).map_or(inner, |(params, _)| params);
    let params = params.trim().trim_start_matches('(').trim_end_matches(')');
    split_top_level(params, ',')
        .into_iter()
        .any(|s| s.trim() == "...")
}

const LONG_DOUBLE_TY: &str = "LongDouble";

// x86-64 SysV wants size 16 / align 16 for long double; align(16) on an f64
// newtype gives that layout while arithmetic stays f64-precision (tier 1).
const LONG_DOUBLE_PRELUDE: &str = "\
#[repr(C, align(16))]
#[derive(Clone, Copy)]
struct LongDouble(f64);
impl core::ops::Add for LongDouble { type Output = LongDouble; fn add(self, o: LongDouble) -> LongDouble { LongDouble(self.0 + o.0) } }
impl core::ops::Sub for LongDouble { type Output = LongDouble; fn sub(self, o: LongDouble) -> LongDouble { LongDouble(self.0 - o.0) } }
impl core::ops::Mul for LongDouble { type Output = LongDouble; fn mul(self, o: LongDouble) -> LongDouble { LongDouble(self.0 * o.0) } }
impl core::ops::Div for LongDouble { type Output = LongDouble; fn div(self, o: LongDouble) -> LongDouble { LongDouble(self.0 / o.0) } }
impl core::ops::Neg for LongDouble { type Output = LongDouble; fn neg(self) -> LongDouble { LongDouble(-self.0) } }
";

fn is_long_double(ty: &str) -> bool {
    ty.starts_with("!cir.long_double")
}

const COMPLEX_TY: &str = "Complex<";

// clang lowers complex `*`/`/` to the libgcc runtime (__mul?c3/__div?c3), reached
// directly for `/` and via a NaN-recovery branch for `*`. We call the same symbols
// so results are bit-identical; #[repr(C)] {re, im} matches the return ABI.
fn is_complex_runtime_call(name: &str) -> bool {
    matches!(name, "__muldc3" | "__divdc3" | "__mulsc3" | "__divsc3")
}

// C `_Complex` has no native Rust type; a #[repr(C)] pair matches its two-scalar
// layout, and the extern runtime routines back `*`/`/`.
const COMPLEX_PRELUDE: &str = "\
#[repr(C)]
#[derive(Clone, Copy)]
struct Complex<T> { re: T, im: T }
impl<T: core::ops::Add<Output = T>> core::ops::Add for Complex<T> {
    type Output = Complex<T>;
    fn add(self, o: Complex<T>) -> Complex<T> { Complex { re: self.re + o.re, im: self.im + o.im } }
}
impl<T: core::ops::Sub<Output = T>> core::ops::Sub for Complex<T> {
    type Output = Complex<T>;
    fn sub(self, o: Complex<T>) -> Complex<T> { Complex { re: self.re - o.re, im: self.im - o.im } }
}
unsafe extern \"C\" {
    fn __muldc3(a: f64, b: f64, c: f64, d: f64) -> Complex<f64>;
    fn __divdc3(a: f64, b: f64, c: f64, d: f64) -> Complex<f64>;
    fn __mulsc3(a: f32, b: f32, c: f32, d: f32) -> Complex<f32>;
    fn __divsc3(a: f32, b: f32, c: f32, d: f32) -> Complex<f32>;
}
";

fn rust_type(cir_ty: &str) -> String {
    rust_type_with_aliases(cir_ty, &BTreeMap::new())
}

// True if the region contains a `cir.continue` that targets the enclosing loop,
// i.e. one not swallowed by a nested loop. `cir.if`/`cir.scope`/`cir.switch`
// bodies are transparent, so we recurse through them.
fn region_has_direct_continue(region: &Region) -> bool {
    region
        .blocks
        .iter()
        .any(|block| ops_have_direct_continue(&block.ops))
}

fn ops_have_direct_continue(ops: &[Op]) -> bool {
    ops.iter().any(|op| match op.name.as_str() {
        "cir.continue" => true,
        "cir.for" | "cir.while" | "cir.do" => false,
        _ => op.regions.iter().any(region_has_direct_continue),
    })
}

fn rust_type_with_aliases(cir_ty: &str, aliases: &BTreeMap<String, String>) -> String {
    let ty = cir_ty.trim();
    if let Some(expanded) = aliases.get(ty) {
        return rust_type_with_aliases(expanded, aliases);
    }
    if ty == "()" || ty.is_empty() {
        "()".into()
    } else if ty == "!void" || ty == "!cir.void" {
        "core::ffi::c_void".into()
    } else if ty == "!cir.bool" {
        "bool".into()
    } else if ty == "!s32i" || ty == "!cir.int<s, 32>" {
        "i32".into()
    } else if ty == "!u32i" || ty == "!cir.int<u, 32>" {
        "u32".into()
    } else if ty == "!s16i" || ty == "!cir.int<s, 16>" {
        "i16".into()
    } else if ty == "!u16i" || ty == "!cir.int<u, 16>" {
        "u16".into()
    } else if ty == "!s8i" || ty == "!cir.int<s, 8>" {
        "i8".into()
    } else if ty == "!u8i" || ty == "!cir.int<u, 8>" {
        "u8".into()
    } else if ty == "!s64i" || ty == "!cir.int<s, 64>" {
        "i64".into()
    } else if ty == "!u64i" || ty == "!cir.int<u, 64>" {
        "u64".into()
    } else if ty == "!cir.float" {
        "f32".into()
    } else if ty == "!cir.double" {
        "f64".into()
    } else if is_long_double(ty) {
        LONG_DOUBLE_TY.into()
    } else if let Some(inner) = ty
        .strip_prefix("!cir.complex<")
        .and_then(|s| s.strip_suffix('>'))
    {
        format!("Complex<{}>", rust_type_with_aliases(inner, aliases))
    } else if let Some(inner) = ty
        .strip_prefix("!cir.ptr<")
        .and_then(|s| s.strip_suffix('>'))
    {
        if let Some(fn_ty) = cir_fn_type_to_rust(inner, aliases) {
            format!("Option<{fn_ty}>")
        } else {
            format!("*mut {}", rust_type_with_aliases(inner, aliases))
        }
    } else if let Some((inner, len)) = parse_cir_array_type(ty) {
        format!("[{}; {len}]", rust_type_with_aliases(&inner, aliases))
    } else if let Some(name) = cir_record_name(ty) {
        if name == "_IO_FILE" {
            "libc::FILE".into()
        } else {
            sanitize_ident(name)
        }
    } else {
        "i32".into()
    }
}

fn cir_fn_type_to_rust(ty: &str, aliases: &BTreeMap<String, String>) -> Option<String> {
    let inner = ty
        .trim()
        .strip_prefix("!cir.func<")
        .and_then(|s| s.strip_suffix('>'))?;
    let (params, ret) = split_top_level_arrow(inner).unwrap_or((inner, "()"));
    let params = params.trim().trim_start_matches('(').trim_end_matches(')');
    let params = split_top_level(params, ',')
        .into_iter()
        .map(str::trim)
        .filter(|s| !s.is_empty() && *s != "...")
        .map(|param| rust_type_with_aliases(param, aliases))
        .collect::<Vec<_>>()
        .join(", ");
    let ret = rust_type_with_aliases(ret.trim(), aliases);
    Some(format!("fn({params}) -> {ret}"))
}

fn is_cir_function_pointer_type(ty: &str) -> bool {
    ty.trim()
        .strip_prefix("!cir.ptr<")
        .and_then(|s| s.strip_suffix('>'))
        .is_some_and(|inner| inner.trim().starts_with("!cir.func<"))
}

fn parse_cir_array_type(ty: &str) -> Option<(String, u64)> {
    let inner = ty
        .trim()
        .strip_prefix("!cir.array<")
        .and_then(|s| s.strip_suffix('>'))?;
    let (element, len) = inner.rsplit_once(" x ")?;
    Some((element.trim().to_string(), len.trim().parse().ok()?))
}

fn parse_rust_array_type(ty: &str) -> Option<(&str, u64)> {
    let inner = ty
        .trim()
        .strip_prefix('[')
        .and_then(|s| s.strip_suffix(']'))?;
    let (element, len) = inner.rsplit_once(';')?;
    Some((element.trim(), len.trim().parse().ok()?))
}

fn cir_record_name(ty: &str) -> Option<&str> {
    if let Some(name) = ty.strip_prefix("!rec_") {
        return Some(name);
    }
    let rest = ty
        .strip_prefix("!cir.union<\"")
        .or_else(|| ty.strip_prefix("!cir.struct<\""))?;
    rest.split_once('"').map(|(name, _)| name)
}

fn op_type_return(ty: &str) -> Option<&str> {
    split_top_level_arrow(ty).map(|(_, ret)| ret.trim())
}

fn default_value(ty: &str) -> &'static str {
    match ty {
        "bool" => "false",
        "f32" | "f64" => "0.0",
        ty if ty.starts_with("*mut ") => "std::ptr::null_mut()",
        ty if ty.starts_with("Option<fn(") => "None",
        _ => "0",
    }
}

fn standard_record_def(name: &str) -> &'static str {
    match name {
        "div_t" => {
            "#[repr(C)]\n#[allow(non_camel_case_types)]\n#[derive(Clone, Copy)]\nstruct div_t { quot: i32, rem: i32 }\n"
        }
        "ldiv_t" => {
            "#[repr(C)]\n#[allow(non_camel_case_types)]\n#[derive(Clone, Copy)]\nstruct ldiv_t { quot: i64, rem: i64 }\n"
        }
        "lldiv_t" => {
            "#[repr(C)]\n#[allow(non_camel_case_types)]\n#[derive(Clone, Copy)]\nstruct lldiv_t { quot: i64, rem: i64 }\n"
        }
        "imaxdiv_t" => {
            "#[repr(C)]\n#[allow(non_camel_case_types)]\n#[derive(Clone, Copy)]\nstruct imaxdiv_t { quot: i64, rem: i64 }\n"
        }
        "tm" => {
            "#[repr(C)]\n#[allow(non_camel_case_types)]\n#[derive(Clone, Copy)]\nstruct tm { tm_sec: i32, tm_min: i32, tm_hour: i32, tm_mday: i32, tm_mon: i32, tm_year: i32, tm_wday: i32, tm_yday: i32, tm_isdst: i32, tm_gmtoff: i64, tm_zone: *mut i8 }\n"
        }
        "lconv" => {
            "#[repr(C)]\n#[allow(non_camel_case_types)]\n#[derive(Clone, Copy)]\nstruct lconv { decimal_point: *mut i8, thousands_sep: *mut i8, grouping: *mut i8, int_curr_symbol: *mut i8, currency_symbol: *mut i8, mon_decimal_point: *mut i8, mon_thousands_sep: *mut i8, mon_grouping: *mut i8, positive_sign: *mut i8, negative_sign: *mut i8, int_frac_digits: i8, frac_digits: i8, p_cs_precedes: i8, p_sep_by_space: i8, n_cs_precedes: i8, n_sep_by_space: i8, p_sign_posn: i8, n_sign_posn: i8, int_p_cs_precedes: i8, int_p_sep_by_space: i8, int_n_cs_precedes: i8, int_n_sep_by_space: i8, int_p_sign_posn: i8, int_n_sign_posn: i8 }\n"
        }
        _ => "",
    }
}

fn standard_record_default(ty: &str) -> Option<&'static str> {
    match ty {
        "div_t" => Some("div_t { quot: 0, rem: 0 }"),
        "ldiv_t" => Some("ldiv_t { quot: 0, rem: 0 }"),
        "lldiv_t" => Some("lldiv_t { quot: 0, rem: 0 }"),
        "imaxdiv_t" => Some("imaxdiv_t { quot: 0, rem: 0 }"),
        "tm" => Some(
            "tm { tm_sec: 0, tm_min: 0, tm_hour: 0, tm_mday: 0, tm_mon: 0, tm_year: 0, tm_wday: 0, tm_yday: 0, tm_isdst: 0, tm_gmtoff: 0, tm_zone: std::ptr::null_mut() }",
        ),
        "lconv" => Some(
            "lconv { decimal_point: std::ptr::null_mut(), thousands_sep: std::ptr::null_mut(), grouping: std::ptr::null_mut(), int_curr_symbol: std::ptr::null_mut(), currency_symbol: std::ptr::null_mut(), mon_decimal_point: std::ptr::null_mut(), mon_thousands_sep: std::ptr::null_mut(), mon_grouping: std::ptr::null_mut(), positive_sign: std::ptr::null_mut(), negative_sign: std::ptr::null_mut(), int_frac_digits: 0, frac_digits: 0, p_cs_precedes: 0, p_sep_by_space: 0, n_cs_precedes: 0, n_sep_by_space: 0, p_sign_posn: 0, n_sign_posn: 0, int_p_cs_precedes: 0, int_p_sep_by_space: 0, int_n_cs_precedes: 0, int_n_sep_by_space: 0, int_p_sign_posn: 0, int_n_sign_posn: 0 }",
        ),
        _ => None,
    }
}

fn zero_for_cir_type(ty: &str) -> &'static str {
    match rust_type(ty).as_str() {
        "f32" | "f64" => "0.0",
        "bool" => "false",
        ty if ty.starts_with("*mut ") => "std::ptr::null_mut()",
        _ => "0",
    }
}

// i128 so a full-range `!u64i` value (e.g. SIG_ERR = (void(*)(int))-1, which CIR
// prints as the unsigned bit pattern 18446744073709551615) survives as a valid
// unsigned literal rather than overflowing i64 and collapsing to 0.
fn parse_cir_int(s: &str) -> Option<i128> {
    let start = s.find("#cir.int<")? + "#cir.int<".len();
    let rest = &s[start..];
    let end = rest.find('>')?;
    rest[..end].parse().ok()
}

fn parse_cir_bool(s: &str) -> Option<bool> {
    let start = s.find("#cir.bool<")? + "#cir.bool<".len();
    let rest = &s[start..];
    let end = rest.find('>')?;
    match rest[..end].trim() {
        "true" => Some(true),
        "false" => Some(false),
        _ => None,
    }
}

fn parse_cir_fp(s: &str) -> Option<String> {
    let start = s.find("#cir.fp<")? + "#cir.fp<".len();
    let rest = &s[start..];
    let end = rest.find('>')?;
    let text = rest[..end].trim();
    if text.starts_with("0x") || text.starts_with("0X") {
        let bits = u64::from_str_radix(&text[2..], 16).ok()?;
        return match text.len() - 2 {
            8 => Some(format!("f32::from_bits(0x{bits:08x})")),
            16 => Some(format!("f64::from_bits(0x{bits:016x})")),
            _ => None,
        };
    }
    Some(text.to_string())
}

// `#cir.const_complex<#cir.fp<re> : ty, #cir.fp<im> : ty>` -> (re, im) literals.
fn parse_cir_const_complex(s: &str) -> Option<(String, String)> {
    let start = s.find("#cir.const_complex<")? + "#cir.const_complex<".len();
    let inner = &s[start..];
    let re = parse_cir_fp(inner)?;
    let comma = inner.find(',')?;
    let im = parse_cir_fp(&inner[comma..])?;
    Some((re, im))
}

fn parse_cir_const_array(s: &str) -> Option<Vec<u8>> {
    let start = s.find('"')? + 1;
    let rest = &s[start..];
    let end = rest.find('"')?;
    Some(decode_cir_string(&rest[..end]))
}

/// Parse the numeric form `#cir.const_array<[#cir.int<1> : !s32i, ...]>` into
/// per-element Rust literals. Returns `None` for the string form (handled by
/// [`parse_cir_const_array`]) or any element we cannot render.
fn parse_cir_const_array_elems(s: &str) -> Option<Vec<String>> {
    let s = s.trim_start();
    if !s.starts_with("#cir.const_array<[") {
        return None;
    }
    let open = s.find('[')?;
    let close = s.rfind(']')?;
    let inner = &s[open + 1..close];
    split_top_level(inner, ',')
        .into_iter()
        .map(str::trim)
        .filter(|part| !part.is_empty())
        .map(|part| {
            if is_cir_aggregate_init(part) {
                return None; // array of aggregates → render_const_value handles it
            }
            parse_cir_int(part)
                .map(|n| n.to_string())
                .or_else(|| parse_cir_fp(part))
        })
        .collect()
}

/// A `cir.global` initializer that is a struct/union or nested-aggregate array,
/// rendered on demand by [`FunctionLowerer::render_const_value`].
fn is_cir_aggregate_init(raw: &str) -> bool {
    let raw = raw.trim_start();
    raw.starts_with("#cir.const_record<") || raw.starts_with("#cir.const_array<[")
}

/// Render `elems` as a Rust array literal, truncated or zero-padded to `len`.
fn render_array_literal(elems: &[String], len: usize) -> String {
    let mut out: Vec<String> = elems.iter().take(len).cloned().collect();
    out.resize(len, "0".to_string());
    format!("[{}]", out.join(", "))
}

fn decode_cir_string(s: &str) -> Vec<u8> {
    let mut bytes = Vec::new();
    let mut chars = s.chars().peekable();
    while let Some(c) = chars.next() {
        if c != '\\' {
            bytes.push(c as u8);
            continue;
        }
        let mut hex = String::new();
        while hex.len() < 2 && chars.peek().is_some_and(|c| c.is_ascii_hexdigit()) {
            hex.push(chars.next().unwrap());
        }
        if hex.is_empty() {
            bytes.push(b'\\');
        } else if let Ok(value) = u8::from_str_radix(&hex, 16) {
            bytes.push(value);
        }
    }
    bytes
}

fn rust_byte_string(bytes: &[u8]) -> String {
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

fn sanitize_ident(s: &str) -> String {
    let mut out = String::new();
    for (i, c) in s.chars().enumerate() {
        if (i == 0 && (c.is_ascii_alphabetic() || c == '_'))
            || (i > 0 && (c.is_ascii_alphanumeric() || c == '_'))
        {
            out.push(c);
        } else {
            out.push('_');
        }
    }
    if out.is_empty() {
        return "_tmp".into();
    }
    // `crate`/`self`/`Self`/`super` can't be raw identifiers, so mangle them instead.
    if matches!(out.as_str(), "crate" | "self" | "Self" | "super") {
        out.push('_');
    } else if is_rust_keyword(&out) {
        out = format!("r#{out}");
    }
    out
}

fn is_rust_keyword(s: &str) -> bool {
    matches!(
        s,
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

fn split_top_level_arrow(s: &str) -> Option<(&str, &str)> {
    let mut angle = 0usize;
    let mut paren = 0usize;
    let bytes = s.as_bytes();
    let mut i = 0usize;
    while i + 1 < bytes.len() {
        match bytes[i] as char {
            '<' => angle += 1,
            '>' => angle = angle.saturating_sub(1),
            '(' => paren += 1,
            ')' => paren = paren.saturating_sub(1),
            '-' if bytes[i + 1] == b'>' && angle == 0 && paren == 0 => {
                return Some((&s[..i], &s[i + 2..]));
            }
            _ => {}
        }
        i += 1;
    }
    None
}

fn split_top_level(s: &str, delimiter: char) -> Vec<&str> {
    let mut parts = Vec::new();
    let mut start = 0usize;
    let mut angle = 0usize;
    let mut paren = 0usize;
    for (i, c) in s.char_indices() {
        match c {
            '<' => angle += 1,
            '>' => angle = angle.saturating_sub(1),
            '(' => paren += 1,
            ')' => paren = paren.saturating_sub(1),
            c if c == delimiter && angle == 0 && paren == 0 => {
                parts.push(&s[start..i]);
                start = i + c.len_utf8();
            }
            _ => {}
        }
    }
    parts.push(&s[start..]);
    parts
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn maps_cir_integer_types_to_rust_primitives() {
        assert_eq!(rust_type("!s32i"), "i32");
        assert_eq!(rust_type("!cir.int<s, 32>"), "i32");
        assert_eq!(rust_type("!u32i"), "u32");
        assert_eq!(rust_type("!s16i"), "i16");
        assert_eq!(rust_type("!u8i"), "u8");
        assert_eq!(rust_type("!s64i"), "i64");
        assert_eq!(rust_type("!cir.float"), "f32");
        assert_eq!(rust_type("!cir.double"), "f64");
        assert_eq!(rust_type("!cir.long_double<!cir.f80>"), "LongDouble");
        assert_eq!(rust_type("!cir.ptr<!cir.double>"), "*mut f64");
        assert_eq!(
            rust_type("!cir.ptr<!cir.func<(!s32i, !s32i) -> !s32i>>"),
            "Option<fn(i32, i32) -> i32>"
        );
        assert_eq!(rust_type("!cir.ptr<!cir.func<()>>"), "Option<fn() -> ()>");
        assert_eq!(rust_type("!rec_Pair"), "Pair");
        assert_eq!(rust_type("!rec__IO_FILE"), "libc::FILE");
        assert_eq!(rust_type("!rec_div_t"), "div_t");
        assert_eq!(rust_type("!cir.union<\"Pair\" {!s32i, !s32i}>"), "Pair");
        assert_eq!(rust_type("!cir.array<!s32i x 3>"), "[i32; 3]");
    }

    #[test]
    fn parses_numeric_const_array_and_renders_literal() {
        let raw = "#cir.const_array<[#cir.int<1> : !s32i, #cir.int<2> : !s32i, \
                   #cir.int<3> : !s32i]> : !cir.array<!s32i x 5>";
        let elems = parse_cir_const_array_elems(raw).expect("numeric const array");
        assert_eq!(elems, vec!["1", "2", "3"]);
        // zero-padded up to the destination length.
        assert_eq!(render_array_literal(&elems, 5), "[1, 2, 3, 0, 0]");
        // truncated when the destination is shorter.
        assert_eq!(render_array_literal(&elems, 2), "[1, 2]");
        // the string form is not a numeric const array.
        assert_eq!(
            parse_cir_const_array_elems("#cir.const_array<\"hi\">"),
            None
        );
    }

    #[test]
    fn treats_nested_aggregate_arrays_as_aggregates_not_flat() {
        // an array of structs must not be flattened to its leading scalars; it is
        // rendered recursively by render_const_value instead.
        let raw = "#cir.const_array<[#cir.const_record<{#cir.int<1> : !s32i, \
                   #cir.int<2> : !s32i}> : !rec_P]> : !cir.array<!rec_P x 1>";
        assert_eq!(parse_cir_const_array_elems(raw), None);
        assert!(is_cir_aggregate_init(raw));
        assert!(is_cir_aggregate_init(
            "#cir.const_record<{#cir.int<3> : !s32i}> : !rec_P"
        ));
        assert!(!is_cir_aggregate_init("#cir.int<3> : !s32i"));
    }

    #[test]
    fn renders_zero_bytes_in_byte_strings() {
        assert_eq!(rust_byte_string(&[0]), "b\"\\0\"");
        assert_eq!(rust_byte_string(&[b'a', 0, b'b']), "b\"a\\0b\"");
    }

    #[test]
    fn maps_source_bool_type_to_rust_bool() {
        assert_eq!(c_type_to_rust(&crate::c_ast::CType::Bool), "bool");
    }

    #[test]
    fn maps_source_array_types_to_rust_arrays() {
        assert_eq!(
            c_type_to_rust(&crate::c_ast::CType::Array(
                Box::new(crate::c_ast::CType::Float { bits: 64 }),
                Some(3)
            )),
            "[f64; 3]"
        );
        assert_eq!(
            c_type_to_rust(&crate::c_ast::CType::Array(
                Box::new(crate::c_ast::CType::Int {
                    signed: false,
                    bits: 8
                }),
                None
            )),
            "*mut u8"
        );
    }

    #[test]
    fn maps_source_function_pointer_types_to_rust_options() {
        assert_eq!(
            c_type_to_rust(&crate::c_ast::CType::FuncPtr {
                ret: Box::new(crate::c_ast::CType::Int {
                    signed: true,
                    bits: 32
                }),
                params: vec![
                    crate::c_ast::CType::Int {
                        signed: true,
                        bits: 32
                    },
                    crate::c_ast::CType::Int {
                        signed: true,
                        bits: 32
                    }
                ],
            }),
            "Option<fn(i32, i32) -> i32>"
        );
    }

    #[test]
    fn maps_source_long_double_type_to_rust_long_double() {
        assert_eq!(
            c_type_to_rust(&crate::c_ast::CType::Float { bits: 80 }),
            "LongDouble"
        );
    }

    #[test]
    fn escapes_rust_keyword_identifiers() {
        assert_eq!(sanitize_ident("box"), "r#box");
        assert_eq!(sanitize_ident("match"), "r#match");
        assert_eq!(sanitize_ident("type"), "r#type");
        // these four cannot be raw identifiers, so they mangle with a trailing underscore
        assert_eq!(sanitize_ident("crate"), "crate_");
        assert_eq!(sanitize_ident("self"), "self_");
        assert_eq!(sanitize_ident("Self"), "Self_");
        assert_eq!(sanitize_ident("super"), "super_");
        // non-keywords and contextual keywords stay untouched
        assert_eq!(sanitize_ident("value"), "value");
        assert_eq!(sanitize_ident("union"), "union");
    }

    #[test]
    fn pointer_values_default_to_null_mut() {
        assert_eq!(default_value("*mut i32"), "std::ptr::null_mut()");
        assert_eq!(default_value("Option<fn(i32, i32) -> i32>"), "None");
    }

    #[test]
    fn parses_nested_op_operand_types() {
        assert_eq!(
            op_operand_types("(!cir.ptr<!s32i>, !cir.ptr<!cir.ptr<!s32i>>) -> ()"),
            vec!["!cir.ptr<!s32i>", "!cir.ptr<!cir.ptr<!s32i>>"]
        );
        assert_eq!(
            op_operand_types(
                "(!cir.ptr<!cir.func<(!s32i, !s32i) -> !s32i>>, !s32i, !s32i) -> !s32i"
            ),
            vec![
                "!cir.ptr<!cir.func<(!s32i, !s32i) -> !s32i>>",
                "!s32i",
                "!s32i"
            ]
        );
        assert_eq!(
            parse_function_type(
                "!cir.func<(!cir.ptr<!cir.func<(!s32i, !s32i) -> !s32i>>, !s32i, !s32i) -> !s32i>"
            ),
            (
                vec![
                    "!cir.ptr<!cir.func<(!s32i, !s32i) -> !s32i>>".to_string(),
                    "!s32i".to_string(),
                    "!s32i".to_string()
                ],
                Some("!s32i".to_string())
            )
        );
    }

    #[test]
    fn parses_floating_point_constants() {
        assert_eq!(
            parse_cir_fp("#cir.fp<1.500000e+00> : !cir.float").as_deref(),
            Some("1.500000e+00")
        );
        assert_eq!(
            parse_cir_fp("#cir.fp<2.250000e+00> : !cir.double").as_deref(),
            Some("2.250000e+00")
        );
        assert_eq!(
            parse_cir_fp("#cir.fp<0x7F800000>").as_deref(),
            Some("f32::from_bits(0x7f800000)")
        );
        assert_eq!(
            parse_cir_fp("#cir.fp<0x7FF0000000000000>").as_deref(),
            Some("f64::from_bits(0x7ff0000000000000)")
        );
        assert_eq!(parse_cir_fp("#cir.int<0> : !s32i"), None);
    }

    #[test]
    fn floating_point_types_default_to_zero_point_zero() {
        assert_eq!(default_value("f32"), "0.0");
        assert_eq!(default_value("f64"), "0.0");
        assert_eq!(default_value("i32"), "0");
        assert_eq!(default_value("bool"), "false");
        assert_eq!(
            standard_record_default("div_t"),
            Some("div_t { quot: 0, rem: 0 }")
        );
    }
}
