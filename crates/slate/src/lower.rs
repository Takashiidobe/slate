//! lower: combine the CIR Op-tree with the C AST oracle into Rust output.

use crate::c_ast::{RecordKind, Unit};
use crate::cir::ir::{Attr, Block, Module, Op, Region};
use crate::ctx::Ctx;
use crate::rust_ast::{Item, Program};
use std::collections::BTreeMap;

/// Lower a parsed CIR module (with the C AST as an oracle) to a Rust program.
pub fn lower(cir: &Module, c: &Unit, ctx: &mut Ctx) -> Program {
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
        externs: BTreeMap::new(),
        uses_long_double: std::cell::Cell::new(false),
        uses_complex: std::cell::Cell::new(false),
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
    /// external (body-less) functions → rust types of their fixed params; the
    /// call site uses this to `as`-cast args and wrap the call in `unsafe`.
    externs: BTreeMap<String, Vec<String>>,
    uses_long_double: std::cell::Cell<bool>,
    uses_complex: std::cell::Cell<bool>,
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
}

#[derive(Debug, Clone)]
struct MemberPtr {
    base: String,
    field: String,
}

#[derive(Debug, Clone)]
struct ElementPtr {
    base: String,
    index: String,
}

#[derive(Debug, Clone)]
struct GlobalVar {
    name: String,
    ty: String,
    init: String,
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
            items.push(Item::Raw(format!(
                "static mut {}: {} = {};\n",
                global.name, global.ty, global.init
            )));
        }

        let mut extern_decls = Vec::new();
        for (name, ty) in &self.extern_globals {
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
            let function_type = attr_str(op, "function_type").unwrap_or("");
            let (sig, params) = self.extern_fn_signature(name, function_type);
            self.externs.insert(name.to_string(), params);
            extern_decls.push(sig);
        }
        if !extern_decls.is_empty() {
            items.push(Item::Raw(format!(
                "unsafe extern \"C\" {{\n    {}\n}}\n",
                extern_decls.join("\n    ")
            )));
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

        Program { items }
    }

    fn collect_global(&mut self, op: &Op) {
        let Some(name) = attr_str(op, "sym_name") else {
            return;
        };
        let Some(raw) = attr_str(op, "initial_value") else {
            let Some(sym_type) = attr_str(op, "sym_type") else {
                return;
            };
            let name = sanitize_ident(name);
            let ty = self.rust_type(sym_type);
            self.extern_globals.insert(name, ty);
            return;
        };
        if let Some(mut bytes) = parse_cir_const_array(raw) {
            bytes.push(0);
            self.strings.insert(name.to_string(), bytes);
        } else if let Some(elems) = parse_cir_const_array_elems(raw) {
            self.const_arrays.insert(name.to_string(), elems);
        } else if raw.trim_start().starts_with("#cir.zero")
            && parse_cir_array_type(attr_str(op, "sym_type").unwrap_or("")).is_some()
        {
            // zero-initialized array; render_array_literal zero-pads to length.
            self.const_arrays.insert(name.to_string(), Vec::new());
        } else if let Some(init) = parse_cir_int(raw)
            .map(|n| n.to_string())
            .or_else(|| parse_cir_fp(raw))
        {
            let ty = self.rust_type(attr_str(op, "sym_type").unwrap_or("!s32i"));
            let name = sanitize_ident(name);
            self.globals
                .insert(name.clone(), GlobalVar { name, ty, init });
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

    fn lower_func(&mut self, op: &Op) -> Option<String> {
        let name = attr_str(op, "sym_name")?;
        let function_type = attr_str(op, "function_type").unwrap_or("");
        let (param_types, ret_ty) = parse_function_type(function_type);
        let entry = op.regions.first()?.blocks.first()?;
        let is_main = name == "main";

        let mut text = String::new();
        let params = entry
            .args
            .iter()
            .enumerate()
            .map(|(i, (arg, ty))| {
                let ty = param_types.get(i).map(String::as_str).unwrap_or(ty);
                format!("{arg}: {}", self.rust_type(ty))
            })
            .collect::<Vec<_>>()
            .join(", ");

        if is_main {
            text.push_str("fn main() {\n");
        } else {
            text.push_str(&format!(
                "fn {name}({params}) -> {} {{\n",
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
        };

        for (arg, _) in &entry.args {
            f.values.insert(arg.clone(), Val::Expr(arg.clone()));
        }
        f.lower_block(entry);
        text.push_str(&f.out);
        text.push_str("}\n");
        Some(text)
    }

    /// Build a Rust `extern "C"` signature line for a body-less C declaration,
    /// returning `(line, fixed_param_rust_types)`. Trailing `...` becomes a Rust
    /// variadic; a missing return arrow means the C function returns `void`.
    fn extern_fn_signature(&self, name: &str, function_type: &str) -> (String, Vec<String>) {
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
        let ret = match ret {
            Some(ret) if ret != "()" => format!(" -> {}", self.rust_type(ret)),
            _ => String::new(),
        };
        let line = format!("fn {name}({}){ret};", parts.join(", "));
        (line, param_types)
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

    fn rust_c_type(&self, ty: &crate::c_ast::CType) -> String {
        let rust = c_type_to_rust(ty);
        if rust.contains(LONG_DOUBLE_TY) {
            self.uses_long_double.set(true);
        }
        rust
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
            "cir.abs" => self.lower_abs(op),
            "cir.ceil" => self.lower_unary_method(op, "ceil"),
            "cir.fabs" => self.lower_unary_method(op, "abs"),
            "cir.floor" => self.lower_unary_method(op, "floor"),
            "cir.round" => self.lower_unary_method(op, "round"),
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
            "cir.inc" => self.lower_inc(op),
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
            "cir.return" => self.lower_return(op),
            "cir.scope" => self.lower_scope(op),
            "cir.if" => self.lower_if(op),
            "cir.for" => self.lower_for(op),
            "cir.while" => self.lower_while(op),
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
        let name = sanitize_ident(attr_str(op, "name").unwrap_or(result));
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
            self.emit_line(&format!(
                "{}[({}) as usize] = {value};",
                element.base, element.index
            ));
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
            self.emit_line(&format!(
                "{}[({}) as usize] = {value};",
                element.base, element.index
            ));
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
                } else {
                    let elems = self.parent.const_arrays.get(name)?;
                    Some(render_array_literal(elems, dst_len.unwrap_or(elems.len())))
                }
            }
            _ => self
                .slots
                .contains_key(src)
                .then(|| self.render_operand(src)),
        }
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
            format!("{}[({}) as usize]", element.base, element.index)
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
            format!("({lhs} {rust_op} {rhs})"),
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
        self.materialize(result, format!("({lhs} {rust_op} {rhs})"), ty);
    }

    fn lower_inc(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(value) = op.operands.first() else {
            return;
        };
        let value = self.render_operand(value);
        let ty = op_result_type(op);
        self.materialize(result, format!("({value} + 1)"), ty);
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
        self.materialize(result, format!("(!{value})"), ty);
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
        self.materialize(result, format!("({lhs} {cmp} {rhs})"), Some("!cir.bool"));
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

    fn lower_get_member(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(base_ptr) = op.operands.first() else {
            return;
        };
        let Some(base) = self.slots.get(base_ptr).cloned() else {
            self.parent.ctx.diagnostics.warn(
                "lower: unsupported get_member base".to_string(),
                op.loc.clone(),
            );
            return;
        };
        let field = sanitize_ident(attr_str(op, "name").unwrap_or(result));
        self.member_ptrs
            .insert(result.clone(), MemberPtr { base, field });
    }

    fn lower_get_element(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        if op.operands.len() < 2 {
            return;
        }
        let base_ptr = &op.operands[0];
        let Some(base) = self.slots.get(base_ptr).cloned() else {
            self.parent.ctx.diagnostics.warn(
                "lower: unsupported get_element base".to_string(),
                op.loc.clone(),
            );
            return;
        };
        let index = self.render_operand(&op.operands[1]);
        self.element_ptrs
            .insert(result.clone(), ElementPtr { base, index });
    }

    fn lower_cast(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(src) = op.operands.first() else {
            return;
        };
        let result_ty = op_result_type(op).unwrap_or("");
        let operand_ty = op_operand_types(op.ty.as_deref().unwrap_or(""))
            .into_iter()
            .next()
            .unwrap_or("");
        let value = match self.values.get(src).cloned() {
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
            _ if result_ty == "!cir.bool" && operand_ty != "!cir.bool" => Val::Expr(format!(
                "({} != {})",
                self.render_operand(src),
                zero_for_cir_type(operand_ty)
            )),
            _ if result_ty == operand_ty => Val::Expr(self.render_operand(src)),
            _ => Val::Expr(format!(
                "{} as {}",
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
        } else {
            format!("{callee}({})", args.join(", "))
        };

        if let Some(result) = op.results.first() {
            self.materialize(result, expr, op_result_type(op));
        } else {
            self.emit_expr(expr);
        }
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

    fn lower_for(&mut self, op: &Op) {
        if op.regions.len() < 3 {
            self.emit_expr("todo!(\"cir.for\")".into());
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
        self.lower_region_ops(&op.regions[1]);
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
        self.lower_region_ops(&op.regions[1]);
        self.indent -= 1;
        self.emit_line("}");
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
        if let Some(record) = self.parent.records.get(ty) {
            match record.kind {
                RecordKind::Struct => {
                    let fields = record
                        .fields
                        .iter()
                        .map(|field| {
                            format!(
                                "{}: {}",
                                sanitize_ident(&field.name),
                                default_c_value(&field.ty)
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
                            default_c_value(&field.ty)
                        );
                    }
                }
            }
        }
        if is_long_double(ty) || ty == LONG_DOUBLE_TY {
            return format!("{LONG_DOUBLE_TY}(0.0)");
        }
        if let Some(inner) = ty
            .strip_prefix(COMPLEX_TY)
            .and_then(|s| s.strip_suffix('>'))
        {
            let d = default_value(inner);
            return format!("Complex {{ re: {d}, im: {d} }}");
        }
        if let Some((inner, len)) = parse_cir_array_type(ty) {
            return format!("[{}; {len}]", default_value(&inner));
        }
        if let Some((inner, len)) = parse_rust_array_type(ty) {
            return format!("[{}; {len}]", default_value(inner));
        }
        default_value(ty).into()
    }
}

fn region_ops(op: &Op) -> Vec<&Op> {
    op.regions
        .iter()
        .flat_map(|region| region.blocks.iter())
        .flat_map(|block| block.ops.iter())
        .collect()
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

fn op_result_type(op: &Op) -> Option<&str> {
    op.ty
        .as_deref()
        .and_then(split_top_level_arrow)
        .map(|(_, ret)| ret.trim())
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
    let (params, ret) = split_top_level_arrow(inner)?;
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

fn default_c_value(ty: &crate::c_ast::CType) -> &'static str {
    match ty {
        crate::c_ast::CType::Bool => "false",
        crate::c_ast::CType::Float { bits: 80 } => "LongDouble(0.0)",
        crate::c_ast::CType::Float { .. } => "0.0",
        crate::c_ast::CType::Record(_) => "Default::default()",
        crate::c_ast::CType::FuncPtr { .. } => "None",
        _ => "0",
    }
}

fn zero_for_cir_type(ty: &str) -> &'static str {
    match rust_type(ty).as_str() {
        "f32" | "f64" => "0.0",
        "bool" => "false",
        _ => "0",
    }
}

fn parse_cir_int(s: &str) -> Option<i64> {
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
    // rust has no hex-float literal syntax; leave those unsupported for now
    if text.starts_with("0x") || text.starts_with("0X") {
        return None;
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
    if !s.contains("#cir.const_array<[") {
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
            parse_cir_int(part)
                .map(|n| n.to_string())
                .or_else(|| parse_cir_fp(part))
        })
        .collect()
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
        assert_eq!(rust_type("!rec_Pair"), "Pair");
        assert_eq!(rust_type("!rec__IO_FILE"), "libc::FILE");
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
    fn maps_source_bool_type_to_rust_bool() {
        assert_eq!(c_type_to_rust(&crate::c_ast::CType::Bool), "bool");
        assert_eq!(default_c_value(&crate::c_ast::CType::Bool), "false");
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
        assert_eq!(
            default_c_value(&crate::c_ast::CType::Float { bits: 80 }),
            "LongDouble(0.0)"
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
        // hex-float literals have no rust syntax and stay unsupported
        assert_eq!(parse_cir_fp("#cir.fp<0x7FF0000000000000>"), None);
        assert_eq!(parse_cir_fp("#cir.int<0> : !s32i"), None);
    }

    #[test]
    fn floating_point_types_default_to_zero_point_zero() {
        assert_eq!(default_value("f32"), "0.0");
        assert_eq!(default_value("f64"), "0.0");
        assert_eq!(default_value("i32"), "0");
        assert_eq!(default_value("bool"), "false");
    }
}
