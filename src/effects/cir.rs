use std::collections::{HashMap, HashSet};

use crate::cir::ir::{Attr, CirOpKind, Op, Region};

use super::{AllocId, Effect, EffectTrace, IntWidth, Location, ParamSeed, Value};

pub fn interpret(ops: &[Op]) -> EffectTrace {
    interpret_with_params(ops, &[])
}

pub fn interpret_with_params(ops: &[Op], params: &[(&str, ParamSeed)]) -> EffectTrace {
    let mut interp = Interp::default();
    interp.seed_params(params);
    let _ = interp.run(ops);
    interp.trace
}

/// How a statement/op completed: either it ran through normally, or it hit a
/// `cir.return` that the enclosing `cir.if`/`cir.for` must propagate past
/// without running the rest of their body.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum Flow {
    Normal,
    Return,
}

#[derive(Default)]
struct Interp {
    env: HashMap<String, Value>,
    locals: HashSet<String>,
    heap: HashMap<Location, Value>,
    next_alloc: u32,
    trace: EffectTrace,
    struct_allocs: HashMap<String, AllocId>,
    struct_alloc_slot: HashMap<AllocId, usize>,
}

impl Interp {
    fn seed_params(&mut self, params: &[(&str, ParamSeed)]) {
        for (name, seed) in params {
            let value = match seed {
                ParamSeed::Scalar(v) => *v,
                ParamSeed::Buffer(elems) => self.seed_buffer(elems),
            };
            self.env.insert(name.to_string(), value);
        }
    }

    fn seed_buffer(&mut self, elems: &[Value]) -> Value {
        let alloc = AllocId(self.next_alloc);
        self.next_alloc += 1;
        let mut offset = 0u64;
        for elem in elems {
            self.heap.insert(
                Location {
                    alloc,
                    byte_offset: offset,
                },
                *elem,
            );
            offset += int_byte_size(elem);
        }
        Value::Ref(Location {
            alloc,
            byte_offset: 0,
        })
    }

    fn run(&mut self, ops: &[Op]) -> Flow {
        for op in ops {
            match self.step(op) {
                Flow::Normal => {}
                flow => return flow,
            }
        }
        Flow::Normal
    }

    /// Runs every block of a region in sequence, propagating whichever
    /// `Flow` its last op produces. Structured CIR regions used by
    /// `cir.if`/`cir.for` are single-block in practice, but this doesn't
    /// assume that.
    fn run_region(&mut self, region: &Region) -> Flow {
        for block in &region.blocks {
            match self.run(&block.ops) {
                Flow::Normal => {}
                flow => return flow,
            }
        }
        Flow::Normal
    }

    fn step(&mut self, op: &Op) -> Flow {
        match op.kind() {
            CirOpKind::Alloca => {
                let result = first_result(op);
                self.locals.insert(result.to_string());
                Flow::Normal
            }
            CirOpKind::GetGlobal => {
                let result = first_result(op);
                self.env.insert(result.to_string(), Value::Null);
                Flow::Normal
            }
            CirOpKind::Const => {
                let result = first_result(op);
                let raw = attr_str(op, "value").unwrap_or_default();
                let value = int_const_value(raw, result_type(op));
                self.env.insert(result.to_string(), value);
                Flow::Normal
            }
            CirOpKind::Mul => self.binop(op, i128::wrapping_mul),
            CirOpKind::Add => self.binop(op, i128::wrapping_add),
            CirOpKind::Sub => self.binop(op, i128::wrapping_sub),
            CirOpKind::Div => self.binop(op, i128::wrapping_div),
            CirOpKind::Rem => self.binop(op, i128::wrapping_rem),
            CirOpKind::And => self.binop(op, |a, b| a & b),
            CirOpKind::Or => self.binop(op, |a, b| a | b),
            CirOpKind::Xor => self.binop(op, |a, b| a ^ b),
            CirOpKind::Shift => self.shift(op),
            CirOpKind::Not => self.unary(op, |a| !a),
            CirOpKind::Minus => self.unary(op, i128::wrapping_neg),
            CirOpKind::Inc => self.unary(op, |a| a.wrapping_add(1)),
            CirOpKind::Dec => self.unary(op, |a| a.wrapping_sub(1)),
            CirOpKind::Cmp => self.cmp(op),
            CirOpKind::Cast => {
                let result = first_result(op);
                let value = self.resolve(&op.operands[0]);
                self.env.insert(result.to_string(), value);
                Flow::Normal
            }
            CirOpKind::Call => self.call(op),
            CirOpKind::PtrStride => self.ptr_stride(op),
            CirOpKind::GetMember => self.get_member(op),
            CirOpKind::Store => self.store(op),
            CirOpKind::Load => self.load(op),
            CirOpKind::If => self.if_(op),
            CirOpKind::For => self.for_(op),
            CirOpKind::Scope => self.run_region(&op.regions[0]),
            CirOpKind::Yield => Flow::Normal,
            CirOpKind::Return => {
                let code = op
                    .operands
                    .first()
                    .map(|name| self.resolve(name))
                    .map(|value| match value {
                        Value::Int { value, .. } => value as i32,
                        other => {
                            panic!("effects::cir: expected an integer exit code, found {other:?}")
                        }
                    })
                    .unwrap_or(0);
                self.trace.push(Effect::Exit(code));
                Flow::Return
            }
            _ => panic!("effects::cir: unsupported op `{}`", op.name),
        }
    }

    fn binop(&mut self, op: &Op, f: impl FnOnce(i128, i128) -> i128) -> Flow {
        let result = first_result(op);
        let (a, width, signed) = self.resolve_int(&op.operands[0]);
        let (b, _, _) = self.resolve_int(&op.operands[1]);
        let value = Value::Int {
            width,
            signed,
            value: f(a, b),
        };
        self.env.insert(result.to_string(), value);
        Flow::Normal
    }

    fn unary(&mut self, op: &Op, f: impl FnOnce(i128) -> i128) -> Flow {
        let result = first_result(op);
        let (a, width, signed) = self.resolve_int(&op.operands[0]);
        let value = Value::Int {
            width,
            signed,
            value: f(a),
        };
        self.env.insert(result.to_string(), value);
        Flow::Normal
    }

    // `isShiftleft` is a unit (presence-only) MLIR attr, same convention as
    // `src/lower.rs`'s `attr_bool`.
    fn shift(&mut self, op: &Op) -> Flow {
        if op.attrs.contains_key("isShiftleft") {
            self.binop(op, |a, b| a.wrapping_shl(b as u32))
        } else {
            self.binop(op, |a, b| a.wrapping_shr(b as u32))
        }
    }

    fn cmp(&mut self, op: &Op) -> Flow {
        let result = first_result(op);
        let (a, ..) = self.resolve_int(&op.operands[0]);
        let (b, ..) = self.resolve_int(&op.operands[1]);
        let value = match attr_int(op, "kind") {
            Some(0) => a < b,
            Some(1) => a <= b,
            Some(2) => a > b,
            Some(3) => a >= b,
            Some(4) => a == b,
            Some(5) => a != b,
            other => panic!("effects::cir: cir.cmp has unexpected `kind` {other:?}"),
        };
        self.env.insert(result.to_string(), Value::Bool(value));
        Flow::Normal
    }

    fn call(&mut self, op: &Op) -> Flow {
        let callee = attr_str(op, "callee")
            .map(|s| s.trim_start_matches('@'))
            .unwrap_or_default();
        match callee {
            "malloc" => {
                let (size, ..) = self.resolve_int(&op.operands[0]);
                let alloc = AllocId(self.next_alloc);
                self.next_alloc += 1;
                self.trace.push(Effect::Alloc {
                    alloc,
                    size: size as u64,
                });
                let result = first_result(op);
                self.env.insert(
                    result.to_string(),
                    Value::Ref(Location {
                        alloc,
                        byte_offset: 0,
                    }),
                );
            }
            "free" => {}
            "printf" => {
                let args = op.operands[1..]
                    .iter()
                    .map(|name| self.resolve(name))
                    .collect();
                self.trace.push(Effect::Call {
                    name: "printf".to_string(),
                    args,
                });
                if let Some(result) = op.results.first() {
                    self.env.insert(
                        result.clone(),
                        Value::Int {
                            width: IntWidth::W32,
                            signed: true,
                            value: 0,
                        },
                    );
                }
            }
            other => panic!("effects::cir: unsupported call target `{other}`"),
        }
        Flow::Normal
    }

    fn ptr_stride(&mut self, op: &Op) -> Flow {
        let result = first_result(op);
        let base = self.resolve_ref(&op.operands[0]);
        let (index, ..) = self.resolve_int(&op.operands[1]);
        let elem_size = pointee_byte_size(result_type(op)).expect("ptr_stride pointee size");
        let loc = Location {
            alloc: base.alloc,
            byte_offset: base
                .byte_offset
                .wrapping_add((index * elem_size as i128) as u64),
        };
        self.env.insert(result.to_string(), Value::Ref(loc));
        Flow::Normal
    }

    // field byte offset = index_attr * this field's own size (assumes homogeneous field sizes)
    fn get_member(&mut self, op: &Op) -> Flow {
        let result = first_result(op);
        let base_name = &op.operands[0];
        let index = attr_int(op, "index_attr").expect("cir.get_member: missing index_attr") as u64;
        let field_size =
            pointee_byte_size(result_type(op)).expect("cir.get_member: field pointee size");
        let base = if self.locals.contains(base_name) {
            self.struct_base(base_name, index, field_size)
        } else {
            self.resolve_ref(base_name)
        };
        let loc = Location {
            alloc: base.alloc,
            byte_offset: base.byte_offset + index * field_size,
        };
        self.env.insert(result.to_string(), Value::Ref(loc));
        Flow::Normal
    }

    fn struct_base(&mut self, name: &str, index: u64, field_size: u64) -> Location {
        let needed_size = (index + 1) * field_size;
        let alloc = match self.struct_allocs.get(name) {
            Some(&alloc) => {
                let slot = self.struct_alloc_slot[&alloc];
                if let Effect::Alloc { size, .. } = &mut self.trace.effects[slot] {
                    *size = (*size).max(needed_size);
                }
                alloc
            }
            None => {
                let alloc = AllocId(self.next_alloc);
                self.next_alloc += 1;
                self.struct_alloc_slot
                    .insert(alloc, self.trace.effects.len());
                self.trace.push(Effect::Alloc {
                    alloc,
                    size: needed_size,
                });
                self.struct_allocs.insert(name.to_string(), alloc);
                alloc
            }
        };
        Location {
            alloc,
            byte_offset: 0,
        }
    }

    fn store(&mut self, op: &Op) -> Flow {
        let value = self.resolve(&op.operands[0]);
        let place = &op.operands[1];
        if self.locals.contains(place) {
            self.env.insert(place.clone(), value);
        } else {
            let loc = self.resolve_ref(place);
            self.heap.insert(loc, value);
            self.trace.push(Effect::Write { loc, value });
        }
        Flow::Normal
    }

    fn load(&mut self, op: &Op) -> Flow {
        let result = first_result(op);
        let place = &op.operands[0];
        if self.locals.contains(place) {
            let value = self.env[place];
            self.env.insert(result.to_string(), value);
        } else {
            let loc = self.resolve_ref(place);
            let value = *self
                .heap
                .get(&loc)
                .unwrap_or_else(|| panic!("effects::cir: read from never-written {loc:?}"));
            self.env.insert(result.to_string(), value);
            self.trace.push(Effect::Read { loc, value });
        }
        Flow::Normal
    }

    fn if_(&mut self, op: &Op) -> Flow {
        let cond = self.resolve_bool(&op.operands[0]);
        if cond {
            self.run_region(&op.regions[0])
        } else if let Some(region) = op.regions.get(1) {
            self.run_region(region)
        } else {
            Flow::Normal
        }
    }

    // `cir.for`'s three regions are cond (terminated by `cir.condition`),
    // body, and step, matching src/lower.rs's `lower_for_loop_body`.
    fn for_(&mut self, op: &Op) -> Flow {
        loop {
            if !self.eval_condition_region(&op.regions[0]) {
                return Flow::Normal;
            }
            match self.run_region(&op.regions[1]) {
                Flow::Normal => {}
                Flow::Return => return Flow::Return,
            }
            match self.run_region(&op.regions[2]) {
                Flow::Normal => {}
                Flow::Return => return Flow::Return,
            }
        }
    }

    fn eval_condition_region(&mut self, region: &Region) -> bool {
        for block in &region.blocks {
            for op in &block.ops {
                if matches!(op.kind(), CirOpKind::Condition | CirOpKind::Yield) {
                    return self.resolve_bool(&op.operands[0]);
                }
                self.step(op);
            }
        }
        panic!("effects::cir: loop condition region has no cir.condition/cir.yield terminator")
    }

    fn resolve(&self, name: &str) -> Value {
        self.env[name]
    }

    fn resolve_int(&self, name: &str) -> (i128, IntWidth, bool) {
        match self.env[name] {
            Value::Int {
                width,
                signed,
                value,
            } => (value, width, signed),
            other => panic!("effects::cir: expected int value for {name}, found {other:?}"),
        }
    }

    fn resolve_ref(&self, name: &str) -> Location {
        match self.env[name] {
            Value::Ref(loc) => loc,
            other => panic!("effects::cir: expected pointer value for {name}, found {other:?}"),
        }
    }

    fn resolve_bool(&self, name: &str) -> bool {
        match self.env[name] {
            Value::Bool(value) => value,
            other => panic!("effects::cir: expected bool value for {name}, found {other:?}"),
        }
    }
}

fn first_result(op: &Op) -> &str {
    op.results.first().map(String::as_str).unwrap_or_default()
}

fn attr_str<'a>(op: &'a Op, key: &str) -> Option<&'a str> {
    op.attrs.get(key).and_then(Attr::as_str)
}

fn attr_int(op: &Op, key: &str) -> Option<i64> {
    op.attrs.get(key).and_then(Attr::as_int)
}

fn result_type(op: &Op) -> Option<&str> {
    let ty = op.ty.as_deref()?;
    let idx = ty.rfind("->")?;
    Some(ty[idx + 2..].trim())
}

fn int_type_width_signed(ty: &str) -> Option<(bool, u32)> {
    let rest = ty.trim().strip_prefix('!')?;
    let signed = match rest.as_bytes().first()? {
        b's' => true,
        b'u' => false,
        _ => return None,
    };
    let bits: u32 = rest[1..].strip_suffix('i')?.parse().ok()?;
    Some((signed, bits))
}

fn int_width(bits: u32) -> IntWidth {
    match bits {
        8 => IntWidth::W8,
        16 => IntWidth::W16,
        32 => IntWidth::W32,
        64 => IntWidth::W64,
        128 => IntWidth::W128,
        _ => IntWidth::PointerSized,
    }
}

fn int_const_value(raw: &str, ty: Option<&str>) -> Value {
    let start = raw
        .find("#cir.int<")
        .expect("cir.const: expected #cir.int<>");
    let rest = &raw[start + "#cir.int<".len()..];
    let end = rest.find('>').expect("cir.const: unterminated #cir.int<>");
    let value: i128 = rest[..end].parse().expect("cir.const: non-integer literal");
    let (signed, bits) = ty.and_then(int_type_width_signed).unwrap_or((true, 32));
    Value::Int {
        width: int_width(bits),
        signed,
        value,
    }
}

fn int_byte_size(value: &Value) -> u64 {
    match value {
        Value::Int { width, .. } => match width {
            IntWidth::W8 => 1,
            IntWidth::W16 => 2,
            IntWidth::W32 => 4,
            IntWidth::W64 | IntWidth::PointerSized => 8,
            IntWidth::W128 => 16,
        },
        other => panic!("effects::cir: buffer element must be an integer, found {other:?}"),
    }
}

fn pointee_byte_size(ptr_ty: Option<&str>) -> Option<u64> {
    let inner = ptr_ty?
        .trim()
        .strip_prefix("!cir.ptr<")?
        .strip_suffix('>')?;
    let (_, bits) = int_type_width_signed(inner)?;
    Some((bits / 8) as u64)
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::cir::ir::Block;
    use std::collections::BTreeMap;

    fn op(name: &str, results: &[&str], operands: &[&str], ty: &str) -> Op {
        Op {
            results: results.iter().map(|s| s.to_string()).collect(),
            name: name.to_string(),
            operands: operands.iter().map(|s| s.to_string()).collect(),
            successors: Vec::new(),
            attrs: BTreeMap::new(),
            regions: Vec::new(),
            ty: Some(ty.to_string()),
            loc: None,
        }
    }

    fn const_op(result: &str, value: i64, ty: &str) -> Op {
        let mut o = op("cir.const", &[result], &[], &format!("() -> {ty}"));
        o.attrs.insert(
            "value".to_string(),
            Attr::Raw(format!("#cir.int<{value}> : {ty}")),
        );
        o
    }

    fn call_malloc(result: &str, size_operand: &str) -> Op {
        let mut o = op(
            "cir.call",
            &[result],
            &[size_operand],
            "(!u64i) -> !cir.ptr<!void>",
        );
        o.attrs
            .insert("callee".to_string(), Attr::Raw("@malloc".to_string()));
        o
    }

    fn call_free(ptr_operand: &str) -> Op {
        let mut o = op("cir.call", &[], &[ptr_operand], "(!cir.ptr<!s32i>) -> ()");
        o.attrs
            .insert("callee".to_string(), Attr::Raw("@free".to_string()));
        o
    }

    fn call_printf(result: &str, fmt_operand: &str, arg_operands: &[&str]) -> Op {
        let mut operands = vec![fmt_operand];
        operands.extend_from_slice(arg_operands);
        let mut o = op(
            "cir.call",
            &[result],
            &operands,
            "(!cir.ptr<!s8i>, ...) -> !s32i",
        );
        o.attrs
            .insert("callee".to_string(), Attr::Raw("@printf".to_string()));
        o
    }

    #[test]
    fn printf_call_pushes_a_call_effect_with_only_the_substituted_args() {
        let ops = vec![
            op("cir.get_global", &["fmt"], &[], "() -> !cir.ptr<!s8i>"),
            const_op("v", 5, "!s32i"),
            call_printf("r", "fmt", &["v"]),
            op("cir.return", &[], &[], "() -> ()"),
        ];
        let trace = interpret(&ops);
        assert_eq!(
            trace.effects,
            vec![
                Effect::Call {
                    name: "printf".to_string(),
                    args: vec![int32(5)],
                },
                Effect::Exit(0),
            ]
        );
    }

    /// Mirrors (minus printf) the CIR clang actually emits for:
    /// `int *p = malloc(2 * sizeof(int)); p[0] = 1; p[1] = 2;
    ///  free(p); return p[0] + p[1];`
    fn malloc_array_fixture() -> Vec<Op> {
        vec![
            op("cir.alloca", &["p"], &[], "() -> !cir.ptr<!cir.ptr<!s32i>>"),
            const_op("c4", 4, "!u64i"),
            const_op("c2", 2, "!u64i"),
            op(
                "cir.mul",
                &["size"],
                &["c4", "c2"],
                "(!u64i, !u64i) -> !u64i",
            ),
            call_malloc("raw", "size"),
            op(
                "cir.cast",
                &["buf"],
                &["raw"],
                "(!cir.ptr<!void>) -> !cir.ptr<!s32i>",
            ),
            op(
                "cir.store",
                &[],
                &["buf", "p"],
                "(!cir.ptr<!s32i>, !cir.ptr<!cir.ptr<!s32i>>) -> ()",
            ),
            const_op("v0", 1, "!s32i"),
            const_op("i0", 0, "!s64i"),
            op(
                "cir.load",
                &["p0"],
                &["p"],
                "(!cir.ptr<!cir.ptr<!s32i>>) -> !cir.ptr<!s32i>",
            ),
            op(
                "cir.ptr_stride",
                &["loc0"],
                &["p0", "i0"],
                "(!cir.ptr<!s32i>, !s64i) -> !cir.ptr<!s32i>",
            ),
            op(
                "cir.store",
                &[],
                &["v0", "loc0"],
                "(!s32i, !cir.ptr<!s32i>) -> ()",
            ),
            const_op("v1", 2, "!s32i"),
            const_op("i1", 1, "!s64i"),
            op(
                "cir.load",
                &["p1"],
                &["p"],
                "(!cir.ptr<!cir.ptr<!s32i>>) -> !cir.ptr<!s32i>",
            ),
            op(
                "cir.ptr_stride",
                &["loc1"],
                &["p1", "i1"],
                "(!cir.ptr<!s32i>, !s64i) -> !cir.ptr<!s32i>",
            ),
            op(
                "cir.store",
                &[],
                &["v1", "loc1"],
                "(!s32i, !cir.ptr<!s32i>) -> ()",
            ),
            const_op("i0b", 0, "!s64i"),
            op(
                "cir.load",
                &["p0b"],
                &["p"],
                "(!cir.ptr<!cir.ptr<!s32i>>) -> !cir.ptr<!s32i>",
            ),
            op(
                "cir.ptr_stride",
                &["loc0b"],
                &["p0b", "i0b"],
                "(!cir.ptr<!s32i>, !s64i) -> !cir.ptr<!s32i>",
            ),
            op(
                "cir.load",
                &["r0"],
                &["loc0b"],
                "(!cir.ptr<!s32i>) -> !s32i",
            ),
            const_op("i1b", 1, "!s64i"),
            op(
                "cir.load",
                &["p1b"],
                &["p"],
                "(!cir.ptr<!cir.ptr<!s32i>>) -> !cir.ptr<!s32i>",
            ),
            op(
                "cir.ptr_stride",
                &["loc1b"],
                &["p1b", "i1b"],
                "(!cir.ptr<!s32i>, !s64i) -> !cir.ptr<!s32i>",
            ),
            op(
                "cir.load",
                &["r1"],
                &["loc1b"],
                "(!cir.ptr<!s32i>) -> !s32i",
            ),
            op(
                "cir.add",
                &["sum"],
                &["r0", "r1"],
                "(!s32i, !s32i) -> !s32i",
            ),
            call_free("buf"),
            op("cir.return", &[], &["sum"], "(!s32i) -> ()"),
        ]
    }

    fn int32(value: i128) -> Value {
        Value::Int {
            width: IntWidth::W32,
            signed: true,
            value,
        }
    }

    #[test]
    fn malloc_write_read_produces_expected_effects() {
        let trace = interpret(&malloc_array_fixture());
        let alloc = AllocId(0);
        assert_eq!(
            trace.effects,
            vec![
                Effect::Alloc { alloc, size: 8 },
                Effect::Write {
                    loc: Location {
                        alloc,
                        byte_offset: 0
                    },
                    value: int32(1),
                },
                Effect::Write {
                    loc: Location {
                        alloc,
                        byte_offset: 4
                    },
                    value: int32(2),
                },
                Effect::Read {
                    loc: Location {
                        alloc,
                        byte_offset: 0
                    },
                    value: int32(1),
                },
                Effect::Read {
                    loc: Location {
                        alloc,
                        byte_offset: 4
                    },
                    value: int32(2),
                },
                Effect::Exit(3),
            ]
        );
    }

    #[test]
    fn local_pointer_variable_never_appears_as_an_effect() {
        let trace = interpret(&malloc_array_fixture());
        for effect in &trace.effects {
            if let Effect::Write { value, .. } | Effect::Read { value, .. } = effect {
                assert!(!matches!(value, Value::Ref(_)));
            }
        }
    }

    fn region(ops: Vec<Op>) -> Region {
        Region {
            blocks: vec![Block {
                label: None,
                args: Vec::new(),
                ops,
            }],
        }
    }

    /// Mirrors the CIR shape for:
    /// `int *p = malloc(sizeof(int));
    ///  if (5 > 3) { p[0] = 1; } else { p[0] = 2; }
    ///  return p[0];`
    fn if_else_fixture() -> Vec<Op> {
        vec![
            op("cir.alloca", &["p"], &[], "() -> !cir.ptr<!cir.ptr<!s32i>>"),
            const_op("c4", 4, "!u64i"),
            call_malloc("raw", "c4"),
            op(
                "cir.cast",
                &["buf"],
                &["raw"],
                "(!cir.ptr<!void>) -> !cir.ptr<!s32i>",
            ),
            op(
                "cir.store",
                &[],
                &["buf", "p"],
                "(!cir.ptr<!s32i>, !cir.ptr<!cir.ptr<!s32i>>) -> ()",
            ),
            const_op("c5", 5, "!s32i"),
            const_op("c3", 3, "!s32i"),
            {
                let mut cmp = op(
                    "cir.cmp",
                    &["cond"],
                    &["c5", "c3"],
                    "(!s32i, !s32i) -> !cir.bool",
                );
                cmp.attrs.insert("kind".to_string(), Attr::Int(2));
                cmp
            },
            {
                let mut if_op = op("cir.if", &[], &["cond"], "(!cir.bool) -> ()");
                if_op.regions = vec![
                    region(vec![
                        const_op("v1", 1, "!s32i"),
                        const_op("i0", 0, "!s64i"),
                        op(
                            "cir.load",
                            &["pld"],
                            &["p"],
                            "(!cir.ptr<!cir.ptr<!s32i>>) -> !cir.ptr<!s32i>",
                        ),
                        op(
                            "cir.ptr_stride",
                            &["loc0"],
                            &["pld", "i0"],
                            "(!cir.ptr<!s32i>, !s64i) -> !cir.ptr<!s32i>",
                        ),
                        op(
                            "cir.store",
                            &[],
                            &["v1", "loc0"],
                            "(!s32i, !cir.ptr<!s32i>) -> ()",
                        ),
                    ]),
                    region(vec![
                        const_op("v2", 2, "!s32i"),
                        const_op("i0b", 0, "!s64i"),
                        op(
                            "cir.load",
                            &["pldb"],
                            &["p"],
                            "(!cir.ptr<!cir.ptr<!s32i>>) -> !cir.ptr<!s32i>",
                        ),
                        op(
                            "cir.ptr_stride",
                            &["loc0b"],
                            &["pldb", "i0b"],
                            "(!cir.ptr<!s32i>, !s64i) -> !cir.ptr<!s32i>",
                        ),
                        op(
                            "cir.store",
                            &[],
                            &["v2", "loc0b"],
                            "(!s32i, !cir.ptr<!s32i>) -> ()",
                        ),
                    ]),
                ];
                if_op
            },
            const_op("i0c", 0, "!s64i"),
            op(
                "cir.load",
                &["pldc"],
                &["p"],
                "(!cir.ptr<!cir.ptr<!s32i>>) -> !cir.ptr<!s32i>",
            ),
            op(
                "cir.ptr_stride",
                &["loc0c"],
                &["pldc", "i0c"],
                "(!cir.ptr<!s32i>, !s64i) -> !cir.ptr<!s32i>",
            ),
            op("cir.load", &["r"], &["loc0c"], "(!cir.ptr<!s32i>) -> !s32i"),
            op("cir.return", &[], &["r"], "(!s32i) -> ()"),
        ]
    }

    #[test]
    fn if_takes_true_branch_and_skips_false_branch_effects() {
        let trace = interpret(&if_else_fixture());
        let alloc = AllocId(0);
        assert_eq!(
            trace.effects,
            vec![
                Effect::Alloc { alloc, size: 4 },
                Effect::Write {
                    loc: Location {
                        alloc,
                        byte_offset: 0
                    },
                    value: int32(1),
                },
                Effect::Read {
                    loc: Location {
                        alloc,
                        byte_offset: 0
                    },
                    value: int32(1),
                },
                Effect::Exit(1),
            ]
        );
    }

    /// Mirrors the CIR shape for:
    /// `int *p = malloc(3 * sizeof(int));
    ///  int i;
    ///  for (i = 0; i < 3; i = i + 1) { p[i] = i + 1; }
    ///  int sum = 0;
    ///  for (i = 0; i < 3; i = i + 1) { sum = sum + p[i]; }
    ///  return sum;`
    fn for_loop_fill_and_sum_fixture() -> Vec<Op> {
        let cond_region = |limit: i64| {
            region(vec![
                op("cir.load", &["iv"], &["i"], "(!cir.ptr<!s32i>) -> !s32i"),
                const_op("limit", limit, "!s32i"),
                {
                    let mut cmp = op(
                        "cir.cmp",
                        &["cond"],
                        &["iv", "limit"],
                        "(!s32i, !s32i) -> !cir.bool",
                    );
                    cmp.attrs.insert("kind".to_string(), Attr::Int(0));
                    cmp
                },
                op("cir.condition", &[], &["cond"], "(!cir.bool) -> ()"),
            ])
        };
        let step_region = || {
            region(vec![
                op("cir.load", &["ivs"], &["i"], "(!cir.ptr<!s32i>) -> !s32i"),
                const_op("one_s", 1, "!s32i"),
                op(
                    "cir.add",
                    &["inc"],
                    &["ivs", "one_s"],
                    "(!s32i, !s32i) -> !s32i",
                ),
                op(
                    "cir.store",
                    &[],
                    &["inc", "i"],
                    "(!s32i, !cir.ptr<!s32i>) -> ()",
                ),
            ])
        };

        vec![
            op("cir.alloca", &["p"], &[], "() -> !cir.ptr<!cir.ptr<!s32i>>"),
            op("cir.alloca", &["i"], &[], "() -> !cir.ptr<!s32i>"),
            op("cir.alloca", &["sum"], &[], "() -> !cir.ptr<!s32i>"),
            const_op("c4", 4, "!u64i"),
            const_op("c3", 3, "!u64i"),
            op(
                "cir.mul",
                &["size"],
                &["c4", "c3"],
                "(!u64i, !u64i) -> !u64i",
            ),
            call_malloc("raw", "size"),
            op(
                "cir.cast",
                &["buf"],
                &["raw"],
                "(!cir.ptr<!void>) -> !cir.ptr<!s32i>",
            ),
            op(
                "cir.store",
                &[],
                &["buf", "p"],
                "(!cir.ptr<!s32i>, !cir.ptr<!cir.ptr<!s32i>>) -> ()",
            ),
            const_op("zero0", 0, "!s32i"),
            op(
                "cir.store",
                &[],
                &["zero0", "i"],
                "(!s32i, !cir.ptr<!s32i>) -> ()",
            ),
            {
                let mut for_op = op("cir.for", &[], &[], "() -> ()");
                for_op.regions = vec![
                    cond_region(3),
                    region(vec![
                        op("cir.load", &["ivb"], &["i"], "(!cir.ptr<!s32i>) -> !s32i"),
                        const_op("one_b", 1, "!s32i"),
                        op(
                            "cir.add",
                            &["val"],
                            &["ivb", "one_b"],
                            "(!s32i, !s32i) -> !s32i",
                        ),
                        op(
                            "cir.load",
                            &["idx_s32"],
                            &["i"],
                            "(!cir.ptr<!s32i>) -> !s32i",
                        ),
                        op("cir.cast", &["idx"], &["idx_s32"], "(!s32i) -> !s64i"),
                        op(
                            "cir.load",
                            &["pld"],
                            &["p"],
                            "(!cir.ptr<!cir.ptr<!s32i>>) -> !cir.ptr<!s32i>",
                        ),
                        op(
                            "cir.ptr_stride",
                            &["loc"],
                            &["pld", "idx"],
                            "(!cir.ptr<!s32i>, !s64i) -> !cir.ptr<!s32i>",
                        ),
                        op(
                            "cir.store",
                            &[],
                            &["val", "loc"],
                            "(!s32i, !cir.ptr<!s32i>) -> ()",
                        ),
                    ]),
                    step_region(),
                ];
                for_op
            },
            const_op("zero1", 0, "!s32i"),
            op(
                "cir.store",
                &[],
                &["zero1", "i"],
                "(!s32i, !cir.ptr<!s32i>) -> ()",
            ),
            const_op("zero_sum", 0, "!s32i"),
            op(
                "cir.store",
                &[],
                &["zero_sum", "sum"],
                "(!s32i, !cir.ptr<!s32i>) -> ()",
            ),
            {
                let mut for_op = op("cir.for", &[], &[], "() -> ()");
                for_op.regions = vec![
                    cond_region(3),
                    region(vec![
                        op(
                            "cir.load",
                            &["idxr_s32"],
                            &["i"],
                            "(!cir.ptr<!s32i>) -> !s32i",
                        ),
                        op("cir.cast", &["idxr"], &["idxr_s32"], "(!s32i) -> !s64i"),
                        op(
                            "cir.load",
                            &["pldr"],
                            &["p"],
                            "(!cir.ptr<!cir.ptr<!s32i>>) -> !cir.ptr<!s32i>",
                        ),
                        op(
                            "cir.ptr_stride",
                            &["locr"],
                            &["pldr", "idxr"],
                            "(!cir.ptr<!s32i>, !s64i) -> !cir.ptr<!s32i>",
                        ),
                        op(
                            "cir.load",
                            &["elem"],
                            &["locr"],
                            "(!cir.ptr<!s32i>) -> !s32i",
                        ),
                        op(
                            "cir.load",
                            &["sum_old"],
                            &["sum"],
                            "(!cir.ptr<!s32i>) -> !s32i",
                        ),
                        op(
                            "cir.add",
                            &["sum_new"],
                            &["sum_old", "elem"],
                            "(!s32i, !s32i) -> !s32i",
                        ),
                        op(
                            "cir.store",
                            &[],
                            &["sum_new", "sum"],
                            "(!s32i, !cir.ptr<!s32i>) -> ()",
                        ),
                    ]),
                    step_region(),
                ];
                for_op
            },
            op(
                "cir.load",
                &["result"],
                &["sum"],
                "(!cir.ptr<!s32i>) -> !s32i",
            ),
            op("cir.return", &[], &["result"], "(!s32i) -> ()"),
        ]
    }

    /// Mirrors the CIR clang emits for a pointer+length pair parameter:
    /// `void bump(int *items, int len) {
    ///    items[0] += 1; items[1] += 1;
    ///    return items[0] + items[1] + len;
    ///  }`
    fn bump_len_fixture() -> Vec<Op> {
        vec![
            op(
                "cir.alloca",
                &["items"],
                &[],
                "() -> !cir.ptr<!cir.ptr<!s32i>>",
            ),
            op("cir.alloca", &["len"], &[], "() -> !cir.ptr<!s32i>"),
            op(
                "cir.store",
                &[],
                &["arg0", "items"],
                "(!cir.ptr<!s32i>, !cir.ptr<!cir.ptr<!s32i>>) -> ()",
            ),
            op(
                "cir.store",
                &[],
                &["arg1", "len"],
                "(!s32i, !cir.ptr<!s32i>) -> ()",
            ),
            const_op("v0", 1, "!s32i"),
            const_op("i0", 0, "!s64i"),
            op(
                "cir.load",
                &["p0"],
                &["items"],
                "(!cir.ptr<!cir.ptr<!s32i>>) -> !cir.ptr<!s32i>",
            ),
            op(
                "cir.ptr_stride",
                &["loc0"],
                &["p0", "i0"],
                "(!cir.ptr<!s32i>, !s64i) -> !cir.ptr<!s32i>",
            ),
            op("cir.load", &["r0"], &["loc0"], "(!cir.ptr<!s32i>) -> !s32i"),
            op(
                "cir.add",
                &["r0b"],
                &["r0", "v0"],
                "(!s32i, !s32i) -> !s32i",
            ),
            op(
                "cir.store",
                &[],
                &["r0b", "loc0"],
                "(!s32i, !cir.ptr<!s32i>) -> ()",
            ),
            const_op("v1", 1, "!s32i"),
            const_op("i1", 1, "!s64i"),
            op(
                "cir.load",
                &["p1"],
                &["items"],
                "(!cir.ptr<!cir.ptr<!s32i>>) -> !cir.ptr<!s32i>",
            ),
            op(
                "cir.ptr_stride",
                &["loc1"],
                &["p1", "i1"],
                "(!cir.ptr<!s32i>, !s64i) -> !cir.ptr<!s32i>",
            ),
            op("cir.load", &["r1"], &["loc1"], "(!cir.ptr<!s32i>) -> !s32i"),
            op(
                "cir.add",
                &["r1b"],
                &["r1", "v1"],
                "(!s32i, !s32i) -> !s32i",
            ),
            op(
                "cir.store",
                &[],
                &["r1b", "loc1"],
                "(!s32i, !cir.ptr<!s32i>) -> ()",
            ),
            const_op("i0c", 0, "!s64i"),
            op(
                "cir.load",
                &["p0c"],
                &["items"],
                "(!cir.ptr<!cir.ptr<!s32i>>) -> !cir.ptr<!s32i>",
            ),
            op(
                "cir.ptr_stride",
                &["loc0c"],
                &["p0c", "i0c"],
                "(!cir.ptr<!s32i>, !s64i) -> !cir.ptr<!s32i>",
            ),
            op(
                "cir.load",
                &["r0c"],
                &["loc0c"],
                "(!cir.ptr<!s32i>) -> !s32i",
            ),
            const_op("i1c", 1, "!s64i"),
            op(
                "cir.load",
                &["p1c"],
                &["items"],
                "(!cir.ptr<!cir.ptr<!s32i>>) -> !cir.ptr<!s32i>",
            ),
            op(
                "cir.ptr_stride",
                &["loc1c"],
                &["p1c", "i1c"],
                "(!cir.ptr<!s32i>, !s64i) -> !cir.ptr<!s32i>",
            ),
            op(
                "cir.load",
                &["r1c"],
                &["loc1c"],
                "(!cir.ptr<!s32i>) -> !s32i",
            ),
            op(
                "cir.add",
                &["sum1"],
                &["r0c", "r1c"],
                "(!s32i, !s32i) -> !s32i",
            ),
            op(
                "cir.load",
                &["lenv"],
                &["len"],
                "(!cir.ptr<!s32i>) -> !s32i",
            ),
            op(
                "cir.add",
                &["sum"],
                &["sum1", "lenv"],
                "(!s32i, !s32i) -> !s32i",
            ),
            op("cir.return", &[], &["sum"], "(!s32i) -> ()"),
        ]
    }

    #[test]
    fn function_parameters_seed_the_trace_as_a_buffer_and_a_scalar() {
        let params: Vec<(&str, ParamSeed)> = vec![
            ("arg0", ParamSeed::Buffer(vec![int32(1), int32(2)])),
            ("arg1", ParamSeed::Scalar(int32(2))),
        ];
        let trace = interpret_with_params(&bump_len_fixture(), &params);
        let alloc = AllocId(0);
        assert_eq!(
            trace.effects,
            vec![
                Effect::Read {
                    loc: Location {
                        alloc,
                        byte_offset: 0
                    },
                    value: int32(1),
                },
                Effect::Write {
                    loc: Location {
                        alloc,
                        byte_offset: 0
                    },
                    value: int32(2),
                },
                Effect::Read {
                    loc: Location {
                        alloc,
                        byte_offset: 4
                    },
                    value: int32(2),
                },
                Effect::Write {
                    loc: Location {
                        alloc,
                        byte_offset: 4
                    },
                    value: int32(3),
                },
                Effect::Read {
                    loc: Location {
                        alloc,
                        byte_offset: 0
                    },
                    value: int32(2),
                },
                Effect::Read {
                    loc: Location {
                        alloc,
                        byte_offset: 4
                    },
                    value: int32(3),
                },
                Effect::Exit(7),
            ]
        );
    }

    /// Mirrors the CIR clang emits for:
    /// `struct point { int x; int y; };
    ///  struct point *p = malloc(sizeof(struct point));
    ///  p->x = 3; p->y = 4;
    ///  int sum = p->x + p->y;
    ///  free(p);
    ///  return sum;`
    fn struct_field_fixture() -> Vec<Op> {
        let get_member = |result: &str, base: &str, index: i64, name: &str| {
            let mut o = op(
                "cir.get_member",
                &[result],
                &[base],
                "(!cir.ptr<!rec_point>) -> !cir.ptr<!s32i>",
            );
            o.attrs.insert("index_attr".to_string(), Attr::Int(index));
            o.attrs
                .insert("name".to_string(), Attr::Str(name.to_string()));
            o
        };
        vec![
            op(
                "cir.alloca",
                &["p"],
                &[],
                "() -> !cir.ptr<!cir.ptr<!rec_point>>",
            ),
            const_op("c8", 8, "!u64i"),
            call_malloc("raw", "c8"),
            op(
                "cir.cast",
                &["buf"],
                &["raw"],
                "(!cir.ptr<!void>) -> !cir.ptr<!rec_point>",
            ),
            op(
                "cir.store",
                &[],
                &["buf", "p"],
                "(!cir.ptr<!rec_point>, !cir.ptr<!cir.ptr<!rec_point>>) -> ()",
            ),
            const_op("v3", 3, "!s32i"),
            op(
                "cir.load",
                &["p0"],
                &["p"],
                "(!cir.ptr<!cir.ptr<!rec_point>>) -> !cir.ptr<!rec_point>",
            ),
            get_member("locx", "p0", 0, "x"),
            op(
                "cir.store",
                &[],
                &["v3", "locx"],
                "(!s32i, !cir.ptr<!s32i>) -> ()",
            ),
            const_op("v4", 4, "!s32i"),
            op(
                "cir.load",
                &["p1"],
                &["p"],
                "(!cir.ptr<!cir.ptr<!rec_point>>) -> !cir.ptr<!rec_point>",
            ),
            get_member("locy", "p1", 1, "y"),
            op(
                "cir.store",
                &[],
                &["v4", "locy"],
                "(!s32i, !cir.ptr<!s32i>) -> ()",
            ),
            op(
                "cir.load",
                &["p2"],
                &["p"],
                "(!cir.ptr<!cir.ptr<!rec_point>>) -> !cir.ptr<!rec_point>",
            ),
            get_member("locxr", "p2", 0, "x"),
            op(
                "cir.load",
                &["rx"],
                &["locxr"],
                "(!cir.ptr<!s32i>) -> !s32i",
            ),
            op(
                "cir.load",
                &["p3"],
                &["p"],
                "(!cir.ptr<!cir.ptr<!rec_point>>) -> !cir.ptr<!rec_point>",
            ),
            get_member("locyr", "p3", 1, "y"),
            op(
                "cir.load",
                &["ry"],
                &["locyr"],
                "(!cir.ptr<!s32i>) -> !s32i",
            ),
            op(
                "cir.add",
                &["sum"],
                &["rx", "ry"],
                "(!s32i, !s32i) -> !s32i",
            ),
            call_free("buf"),
            op("cir.return", &[], &["sum"], "(!s32i) -> ()"),
        ]
    }

    #[test]
    fn struct_field_read_write_produces_expected_effects() {
        let trace = interpret(&struct_field_fixture());
        let alloc = AllocId(0);
        assert_eq!(
            trace.effects,
            vec![
                Effect::Alloc { alloc, size: 8 },
                Effect::Write {
                    loc: Location {
                        alloc,
                        byte_offset: 0
                    },
                    value: int32(3),
                },
                Effect::Write {
                    loc: Location {
                        alloc,
                        byte_offset: 4
                    },
                    value: int32(4),
                },
                Effect::Read {
                    loc: Location {
                        alloc,
                        byte_offset: 0
                    },
                    value: int32(3),
                },
                Effect::Read {
                    loc: Location {
                        alloc,
                        byte_offset: 4
                    },
                    value: int32(4),
                },
                Effect::Exit(7),
            ]
        );
    }

    #[test]
    fn for_loop_fills_array_then_sums_it() {
        let trace = interpret(&for_loop_fill_and_sum_fixture());
        let alloc = AllocId(0);
        assert_eq!(
            trace.effects,
            vec![
                Effect::Alloc { alloc, size: 12 },
                Effect::Write {
                    loc: Location {
                        alloc,
                        byte_offset: 0
                    },
                    value: int32(1),
                },
                Effect::Write {
                    loc: Location {
                        alloc,
                        byte_offset: 4
                    },
                    value: int32(2),
                },
                Effect::Write {
                    loc: Location {
                        alloc,
                        byte_offset: 8
                    },
                    value: int32(3),
                },
                Effect::Read {
                    loc: Location {
                        alloc,
                        byte_offset: 0
                    },
                    value: int32(1),
                },
                Effect::Read {
                    loc: Location {
                        alloc,
                        byte_offset: 4
                    },
                    value: int32(2),
                },
                Effect::Read {
                    loc: Location {
                        alloc,
                        byte_offset: 8
                    },
                    value: int32(3),
                },
                Effect::Exit(6),
            ]
        );
    }
}
