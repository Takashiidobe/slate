//! lower: combine the CIR Op-tree with the C AST oracle into Rust output.

use crate::c_ast::Unit;
use crate::cir::ir::{Attr, Block, Module, Op, Region};
use crate::ctx::Ctx;
use crate::rust_ast::{Item, Program};
use std::collections::BTreeMap;

/// Lower a parsed CIR module (with the C AST as an oracle) to a Rust program.
pub fn lower(cir: &Module, _c: &Unit, ctx: &mut Ctx) -> Program {
    let mut lowerer = Lowerer {
        ctx,
        aliases: cir.aliases.clone(),
        strings: BTreeMap::new(),
    };
    lowerer.lower_module(cir)
}

struct Lowerer<'a> {
    ctx: &'a mut Ctx,
    aliases: BTreeMap<String, String>,
    strings: BTreeMap<String, Vec<u8>>,
}

struct FunctionLowerer<'a, 'b> {
    parent: &'a mut Lowerer<'b>,
    values: BTreeMap<String, Val>,
    slots: BTreeMap<String, String>,
    slot_types: BTreeMap<String, String>,
    temp_counter: usize,
    indent: usize,
    out: String,
    is_main: bool,
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
                    format!(
                        "{}.as_ptr() as *const libc::c_char",
                        rust_byte_string(bytes)
                    )
                }
                None => name.clone(),
            },
        }
    }
}

impl<'a> Lowerer<'a> {
    fn lower_module(&mut self, module: &Module) -> Program {
        let mut items = vec![Item::Raw(
            "#![allow(dead_code, unused, non_snake_case)]".into(),
        )];

        let Some(module_op) = module.ops.iter().find(|op| op.name == "builtin.module") else {
            self.ctx
                .diagnostics
                .error("lower: no builtin.module op", None);
            return Program { items };
        };

        let ops = region_ops(module_op);
        for op in &ops {
            if op.name == "cir.global" {
                self.collect_global_string(op);
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

        Program { items }
    }

    fn collect_global_string(&mut self, op: &Op) {
        let Some(name) = attr_str(op, "sym_name") else {
            return;
        };
        let Some(raw) = attr_str(op, "initial_value") else {
            return;
        };
        if let Some(mut bytes) = parse_cir_const_array(raw) {
            bytes.push(0);
            self.strings.insert(name.to_string(), bytes);
        }
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

    fn rust_type(&self, cir_ty: &str) -> String {
        rust_type_with_aliases(cir_ty, &self.aliases)
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
            "cir.load" => self.lower_load(op),
            "cir.const" => self.lower_const(op),
            "cir.add" => self.lower_binary(op, "+"),
            "cir.inc" => self.lower_inc(op),
            "cir.cmp" => self.lower_cmp(op),
            "cir.get_global" => self.lower_get_global(op),
            "cir.cast" => self.lower_cast(op),
            "cir.call" => self.lower_call(op),
            "cir.return" => self.lower_return(op),
            "cir.scope" => self.lower_scope(op),
            "cir.for" => self.lower_for(op),
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
        self.emit_line(&format!("let mut {name}: {ty} = {};", default_value(&ty)));
    }

    fn lower_store(&mut self, op: &Op) {
        if op.operands.len() < 2 {
            return;
        }
        let value = self.render_operand(&op.operands[0]);
        let ptr = &op.operands[1];
        if let Some(slot) = self.slots.get(ptr) {
            self.emit_line(&format!("{slot} = {value};"));
        } else {
            let ptr = self.render_operand(ptr);
            self.emit_line(&format!("unsafe {{ *{ptr} = {value}; }}"));
        }
    }

    fn lower_load(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(ptr) = op.operands.first() else {
            return;
        };
        let value = match self.slots.get(ptr) {
            Some(slot) => slot.clone(),
            None => format!("unsafe {{ *{} }}", self.render_operand(ptr)),
        };
        self.materialize(result, value, op_result_type(op));
    }

    fn lower_const(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let value = attr_str(op, "value")
            .and_then(parse_cir_int)
            .map(|n| n.to_string())
            .unwrap_or_else(|| "0".into());
        self.materialize(result, value, op_result_type(op));
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

    fn lower_inc(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(value) = op.operands.first() else {
            return;
        };
        let value = self.render_operand(value);
        self.materialize(result, format!("({value} + 1)"), op_result_type(op));
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

    fn lower_cast(&mut self, op: &Op) {
        let Some(result) = op.results.first() else {
            return;
        };
        let Some(src) = op.operands.first() else {
            return;
        };
        let value = match self.values.get(src).cloned() {
            Some(Val::Global(name)) => Val::Global(name),
            _ => Val::Expr(format!(
                "{} as {}",
                self.render_operand(src),
                self.parent.rust_type(op_result_type(op).unwrap_or(""))
            )),
        };
        self.values.insert(result.clone(), value);
    }

    fn lower_call(&mut self, op: &Op) {
        let callee = attr_str(op, "callee")
            .unwrap_or("")
            .trim_start_matches('@')
            .to_string();
        let args = op
            .operands
            .iter()
            .map(|operand| self.render_operand(operand))
            .collect::<Vec<_>>();
        let expr = if callee == "printf" {
            format!("unsafe {{ libc::printf({}) }}", args.join(", "))
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
            .unwrap_or_else(|| sanitize_ident(operand))
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

fn op_result_type(op: &Op) -> Option<&str> {
    op.ty
        .as_deref()
        .and_then(|ty| ty.rsplit_once("->"))
        .map(|(_, ret)| ret.trim())
}

fn parse_function_type(s: &str) -> (Vec<String>, Option<String>) {
    let Some(inner) = s
        .strip_prefix("!cir.func<")
        .and_then(|s| s.strip_suffix('>'))
    else {
        return (Vec::new(), None);
    };
    let Some((params, ret)) = inner.split_once("->") else {
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
    } else if let Some(inner) = ty
        .strip_prefix("!cir.ptr<")
        .and_then(|s| s.strip_suffix('>'))
    {
        format!("*mut {}", rust_type_with_aliases(inner, aliases))
    } else {
        "i32".into()
    }
}

fn op_type_return(ty: &str) -> Option<&str> {
    ty.rsplit_once("->").map(|(_, ret)| ret.trim())
}

fn default_value(ty: &str) -> &'static str {
    if ty == "bool" { "false" } else { "0" }
}

fn parse_cir_int(s: &str) -> Option<i64> {
    let start = s.find("#cir.int<")? + "#cir.int<".len();
    let rest = &s[start..];
    let end = rest.find('>')?;
    rest[..end].parse().ok()
}

fn parse_cir_const_array(s: &str) -> Option<Vec<u8>> {
    let start = s.find('"')? + 1;
    let rest = &s[start..];
    let end = rest.find('"')?;
    Some(decode_cir_string(&rest[..end]))
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
    if out.is_empty() { "_tmp".into() } else { out }
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
    }
}
