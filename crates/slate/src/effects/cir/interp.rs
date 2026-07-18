use std::collections::{BTreeMap, HashMap, HashSet};

use crate::cir::ir::{Attr, CirOpKind, Op, Region};
use crate::rust_ast::AtomicOrdering;

use super::support::*;
use crate::effects::{
    AllocId, AtomicId, CallSummary, Effect, EffectTrace, FileId, IntWidth, Location, ParamSeed,
    Value, call_summary,
};

const BLOCK_LABEL_ALLOC: AllocId = AllocId(u32::MAX);

pub fn interpret(ops: &[Op]) -> EffectTrace {
    interpret_with_params(ops, &[])
}

pub fn interpret_with_params(ops: &[Op], params: &[(&str, ParamSeed)]) -> EffectTrace {
    let mut interp = Interp::default();
    interp.seed_params(params);
    let _ = interp.run(ops);
    interp.trace
}

pub fn interpret_module_main(module: &crate::cir::ir::Module) -> EffectTrace {
    let builtin_module = &module.ops[0];
    let top_level = &builtin_module.regions[0].blocks[0].ops;
    let mut interp = Interp::default();
    interp.type_aliases = module.aliases.clone();
    interp.seed_module(top_level);
    let main = interp
        .funcs
        .get("main")
        .cloned()
        .expect("fixture must define `main`");
    let _ = interp.run_function_region(&main.regions[0]);
    interp.trace
}

/// How a statement/op completed: either it ran through normally, or it hit a
/// `cir.return` that the enclosing `cir.if`/`cir.for` must propagate past
/// without running the rest of their body.
#[derive(Debug, Clone, Copy, PartialEq)]
enum Flow {
    Normal,
    Return(Value),
    Break,
    Continue,
    Jump(usize),
}

#[derive(Default)]
struct Interp {
    env: HashMap<String, Value>,
    arrays: HashMap<String, Vec<Value>>,
    locals: HashSet<String>,
    local_allocs: HashMap<String, AllocId>,
    globals: HashMap<String, Location>,
    const_arrays: HashMap<String, Vec<Value>>,
    c_strings: HashMap<String, Vec<u8>>,
    hidden_c_strings: HashMap<String, Location>,
    funcs: HashMap<String, Op>,
    function_pointers: HashMap<String, String>,
    atomics: HashMap<String, AtomicId>,
    atomic_values: HashMap<AtomicId, Value>,
    heap: HashMap<Location, Value>,
    next_alloc: u32,
    next_atomic: u32,
    trace: EffectTrace,
    struct_allocs: HashMap<String, AllocId>,
    struct_alloc_slot: HashMap<AllocId, usize>,
    freed: HashSet<AllocId>,
    call_depth: usize,
    next_file: u32,
    block_to_index: HashMap<String, usize>,
    label_to_index: HashMap<String, usize>,
    pending_block_args: HashMap<usize, Vec<Value>>,
    type_aliases: BTreeMap<String, String>,
    aggregate_bindings: HashMap<String, AggregateBinding>,
    aggregate_allocs: HashMap<AllocId, AggregateBinding>,
    bitfield_offsets: HashMap<(AllocId, String), u64>,
    bitfield_next_offsets: HashMap<AllocId, u64>,
}

#[derive(Debug, Clone)]
struct AggregateBinding {
    alloc: AllocId,
    size: u64,
}

impl Interp {
    fn seed_module(&mut self, ops: &[Op]) {
        for op in ops {
            match op.kind() {
                CirOpKind::Global => self.seed_global(op),
                CirOpKind::Func => {
                    if !op
                        .regions
                        .first()
                        .is_none_or(|region| region.blocks.is_empty())
                        && let Some(name) = attr_str(op, "sym_name")
                    {
                        self.funcs.insert(name.to_string(), op.clone());
                    }
                }
                _ => {}
            }
        }
    }

    fn seed_global(&mut self, op: &Op) {
        let Some(name) = attr_str(op, "sym_name") else {
            return;
        };
        if let Some(values) = global_const_array_values(op) {
            if attr_str(op, "constant") != Some("false") {
                self.const_arrays.insert(name.to_string(), values.clone());
                if values.iter().all(|value| {
                    matches!(
                        value,
                        Value::Int {
                            width: IntWidth::W8,
                            ..
                        }
                    )
                }) {
                    self.c_strings
                        .insert(name.to_string(), values_to_bytes(&values));
                }
                return;
            }
            let is_block_label_table = values
                .iter()
                .all(|value| matches!(value, Value::BlockLabel(_)));
            let alloc = if is_block_label_table {
                BLOCK_LABEL_ALLOC
            } else {
                let alloc = AllocId(self.next_alloc);
                self.next_alloc += 1;
                alloc
            };
            let base = Location {
                alloc,
                byte_offset: 0,
            };
            self.globals.insert(name.to_string(), base);
            let size: u64 = values.iter().map(int_byte_size).sum();
            if !is_block_label_table {
                self.trace.push(Effect::Alloc { alloc, size });
            }
            let mut offset = 0;
            for value in &values {
                let loc = Location {
                    alloc,
                    byte_offset: offset,
                };
                self.heap.insert(loc, *value);
                if !is_block_label_table {
                    self.trace.push(Effect::Write { loc, value: *value });
                }
                offset += int_byte_size(value);
            }
            return;
        }
        if let Some(values) = global_const_aggregate_values(op, &self.type_aliases) {
            let Some(ty) = attr_str(op, "sym_type") else {
                return;
            };
            let size = cir_type_size(ty, &self.type_aliases).unwrap_or(0);
            let alloc = if attr_str(op, "constant") == Some("false") {
                let alloc = AllocId(self.next_alloc);
                self.next_alloc += 1;
                alloc
            } else {
                AllocId(u32::MAX - self.globals.len() as u32)
            };
            let base = Location {
                alloc,
                byte_offset: 0,
            };
            self.globals.insert(name.to_string(), base);
            self.aggregate_allocs
                .insert(alloc, AggregateBinding { alloc, size });
            for (offset, value) in values {
                self.heap.insert(
                    Location {
                        alloc,
                        byte_offset: offset,
                    },
                    value,
                );
            }
            if attr_str(op, "constant") == Some("false") {
                self.trace.push(Effect::Alloc { alloc, size });
            }
            return;
        }
        if attr_str(op, "constant") != Some("false") {
            return;
        }
        let Some(ty) = attr_str(op, "sym_type") else {
            return;
        };
        let Some((_, bits)) = int_type_width_signed(ty) else {
            return;
        };
        let raw = attr_str(op, "initial_value").unwrap_or_default();
        let value = int_const_value(raw, Some(ty));
        let alloc = AllocId(self.next_alloc);
        self.next_alloc += 1;
        let loc = Location {
            alloc,
            byte_offset: 0,
        };
        self.globals.insert(name.to_string(), loc);
        self.heap.insert(loc, value);
        self.trace.push(Effect::Alloc {
            alloc,
            size: (bits / 8) as u64,
        });
        self.trace.push(Effect::Write { loc, value });
    }

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

    fn run_function_region(&mut self, region: &Region) -> Flow {
        let saved_blocks = std::mem::take(&mut self.block_to_index);
        let saved_labels = std::mem::take(&mut self.label_to_index);
        let saved_pending = std::mem::take(&mut self.pending_block_args);
        for (index, block) in region.blocks.iter().enumerate() {
            let key = block.label.clone().unwrap_or_else(|| format!("bb{index}"));
            self.block_to_index.insert(key, index);
            for op in &block.ops {
                if op.kind() == CirOpKind::Label
                    && let Some(label) = attr_str(op, "label")
                {
                    self.label_to_index.insert(label.to_string(), index);
                }
            }
        }

        let mut index = 0usize;
        let flow = loop {
            let Some(block) = region.blocks.get(index) else {
                break Flow::Normal;
            };
            if let Some(values) = self.pending_block_args.remove(&index) {
                if values.len() != block.args.len() {
                    panic!(
                        "effects::cir: block expected {} arg(s), got {}",
                        block.args.len(),
                        values.len()
                    );
                }
                for ((arg, _), value) in block.args.iter().zip(values) {
                    self.env.insert(arg.clone(), value);
                }
            }
            match self.run(&block.ops) {
                Flow::Normal => index += 1,
                Flow::Jump(next) => index = next,
                flow @ Flow::Return(_) => break flow,
                Flow::Break | Flow::Continue => break Flow::Normal,
            }
            if index >= region.blocks.len() {
                break Flow::Normal;
            }
        };
        self.block_to_index = saved_blocks;
        self.label_to_index = saved_labels;
        self.pending_block_args = saved_pending;
        flow
    }

    fn step(&mut self, op: &Op) -> Flow {
        match op.kind() {
            CirOpKind::Alloca => {
                let result = first_result(op);
                self.locals.insert(result.to_string());
                let pointee =
                    result_type(op).and_then(|ty| cir_ptr_pointee(ty, &self.type_aliases));
                if let Some(ty) = pointee
                    && cir_is_aggregate_type(ty, &self.type_aliases)
                    && let Some(size) = cir_type_size(ty, &self.type_aliases)
                {
                    self.aggregate_bindings.insert(
                        result.to_string(),
                        AggregateBinding {
                            alloc: AllocId(u32::MAX),
                            size,
                        },
                    );
                }
                Flow::Normal
            }
            CirOpKind::GetGlobal => {
                let result = first_result(op);
                let name = attr_str(op, "name")
                    .unwrap_or_default()
                    .trim_start_matches('@')
                    .trim_matches('"');
                if let Some(values) = self.const_arrays.get(name) {
                    self.arrays.insert(result.to_string(), values.clone());
                    self.c_strings
                        .insert(result.to_string(), values_to_bytes(values));
                }
                if self.funcs.contains_key(name) {
                    self.function_pointers
                        .insert(result.to_string(), name.to_string());
                }
                let value = self
                    .globals
                    .get(name)
                    .map_or(Value::Null, |loc| Value::Ref(*loc));
                self.env.insert(result.to_string(), value);
                Flow::Normal
            }
            CirOpKind::Const => {
                let result = first_result(op);
                let raw = attr_str(op, "value").unwrap_or_default();
                let value = const_value(raw, result_type(op));
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
            CirOpKind::Not => self.not(op),
            CirOpKind::Minus => self.unary(op, i128::wrapping_neg),
            CirOpKind::Inc => self.unary(op, |a| a.wrapping_add(1)),
            CirOpKind::Dec => self.unary(op, |a| a.wrapping_sub(1)),
            CirOpKind::Cmp => self.cmp(op),
            CirOpKind::Cast => {
                let result = first_result(op);
                if let Some(bytes) = self.c_strings.get(&op.operands[0]).cloned() {
                    self.c_strings.insert(result.to_string(), bytes);
                }
                let value = if self.locals.contains(&op.operands[0])
                    && result_type_for_operand(op.ty.as_deref(), 0)
                        .is_some_and(|ty| ty.trim().starts_with("!cir.ptr<!cir.array<"))
                    && result_type(op).is_some_and(|ty| ty.trim().starts_with("!cir.ptr<"))
                    && !matches!(self.env.get(&op.operands[0]), Some(Value::Ref(_)))
                {
                    Value::Ref(self.ensure_local_array_address(
                        &op.operands[0],
                        result_type_for_operand(op.ty.as_deref(), 0),
                    ))
                } else if self.locals.contains(&op.operands[0])
                    && result_type(op).is_some_and(|ty| ty.trim().starts_with("!cir.ptr<"))
                    && !matches!(self.env.get(&op.operands[0]), Some(Value::Ref(_)))
                {
                    Value::Ref(self.ensure_local_scalar_address(&op.operands[0], result_type(op)))
                } else if result_type(op).is_some_and(|ty| ty.trim() == "!cir.bool") {
                    match self.resolve(&op.operands[0]) {
                        Value::Int { value, .. } => Value::Bool(value != 0),
                        Value::Bool(value) => Value::Bool(value),
                        Value::Ref(_) => Value::Bool(true),
                        Value::File(_) => Value::Bool(true),
                        Value::Null => Value::Bool(false),
                        other => panic!("effects::cir: cannot cast {other:?} to bool"),
                    }
                } else if let Some((signed, bits)) = result_type(op).and_then(int_type_width_signed)
                {
                    match self.resolve(&op.operands[0]) {
                        Value::Bool(value) => Value::Int {
                            width: int_width(bits),
                            signed,
                            value: i128::from(value),
                        },
                        Value::Int { value, .. } => Value::Int {
                            width: int_width(bits),
                            signed,
                            value,
                        },
                        other => other,
                    }
                } else {
                    self.resolve(&op.operands[0])
                };
                self.env.insert(result.to_string(), value);
                Flow::Normal
            }
            CirOpKind::Call => self.call(op),
            CirOpKind::PtrStride => self.ptr_stride(op),
            CirOpKind::GetElement => self.get_element(op),
            CirOpKind::PtrDiff => self.ptr_diff(op),
            CirOpKind::GetMember => self.get_member(op),
            CirOpKind::GetBitfield => self.get_bitfield(op),
            CirOpKind::SetBitfield => self.set_bitfield(op),
            CirOpKind::Copy => self.copy(op),
            CirOpKind::LibcMemchr => self.libc_memchr(op),
            CirOpKind::Store => self.store(op),
            CirOpKind::Load => self.load(op),
            CirOpKind::AtomicFetch => self.atomic_fetch(op),
            CirOpKind::AtomicXchg => self.atomic_xchg(op),
            CirOpKind::AtomicCmpxchg => self.atomic_cmpxchg(op),
            CirOpKind::AtomicFence => self.atomic_fence(op),
            CirOpKind::If => self.if_(op),
            CirOpKind::For => self.for_(op),
            CirOpKind::While => self.while_(op),
            CirOpKind::Do => self.do_(op),
            CirOpKind::Switch => self.switch_(op),
            CirOpKind::Select => self.select(op),
            CirOpKind::Ternary => self.ternary(op),
            CirOpKind::Scope => self.run_region(&op.regions[0]),
            CirOpKind::Yield => Flow::Normal,
            CirOpKind::Break => Flow::Break,
            CirOpKind::Continue => Flow::Continue,
            CirOpKind::Goto => self.goto(op),
            CirOpKind::Br => self.br(op),
            CirOpKind::IndirectBr => self.indirect_br(op),
            CirOpKind::Label => Flow::Normal,
            CirOpKind::Return => {
                let value = op
                    .operands
                    .first()
                    .map(|name| self.resolve(name))
                    .unwrap_or(Value::Int {
                        width: IntWidth::W32,
                        signed: true,
                        value: 0,
                    });
                if self.call_depth == 0 {
                    let Value::Int { value: code, .. } = value else {
                        panic!("effects::cir: expected an integer exit code, found {value:?}")
                    };
                    let code = code as i32;
                    self.trace.push(Effect::Exit(code));
                }
                Flow::Return(value)
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

    fn not(&mut self, op: &Op) -> Flow {
        let result = first_result(op);
        let value = match self.resolve(&op.operands[0]) {
            Value::Bool(value) => Value::Bool(!value),
            Value::Int {
                width,
                signed,
                value,
            } => Value::Int {
                width,
                signed,
                value: !value,
            },
            other => panic!("effects::cir: cannot apply not to {other:?}"),
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
        let lhs = self.resolve(&op.operands[0]);
        let rhs = self.resolve(&op.operands[1]);
        let value = match (lhs, rhs) {
            (Value::Int { value: a, .. }, Value::Int { value: b, .. }) => {
                match attr_int(op, "kind") {
                    Some(0) => a < b,
                    Some(1) => a <= b,
                    Some(2) => a > b,
                    Some(3) => a >= b,
                    Some(4) => a == b,
                    Some(5) => a != b,
                    other => panic!("effects::cir: cir.cmp has unexpected `kind` {other:?}"),
                }
            }
            (Value::Ref(a), Value::Ref(b)) => match attr_int(op, "kind") {
                Some(4) => a == b,
                Some(5) => a != b,
                other => panic!("effects::cir: pointer cmp has unexpected `kind` {other:?}"),
            },
            (Value::Null, Value::Null) => match attr_int(op, "kind") {
                Some(4) => true,
                Some(5) => false,
                other => panic!("effects::cir: null cmp has unexpected `kind` {other:?}"),
            },
            (Value::Ref(_), Value::Null) | (Value::Null, Value::Ref(_)) => {
                match attr_int(op, "kind") {
                    Some(4) => false,
                    Some(5) => true,
                    other => {
                        panic!("effects::cir: pointer/null cmp has unexpected `kind` {other:?}")
                    }
                }
            }
            (Value::File(_), Value::Null) | (Value::Null, Value::File(_)) => {
                match attr_int(op, "kind") {
                    Some(4) => false,
                    Some(5) => true,
                    other => {
                        panic!("effects::cir: file/null cmp has unexpected `kind` {other:?}")
                    }
                }
            }
            other => panic!("effects::cir: cir.cmp has unexpected `kind` {other:?}"),
        };
        self.env.insert(result.to_string(), Value::Bool(value));
        Flow::Normal
    }

    fn call(&mut self, op: &Op) -> Flow {
        let callee = attr_str(op, "callee")
            .map(|s| s.trim_start_matches('@'))
            .unwrap_or_default();
        if let Some(summary) = call_summary(callee) {
            self.call_summary(op, summary);
        } else if self.funcs.contains_key(callee) {
            self.call_user_target(op, callee);
        } else {
            panic!("effects::cir: unsupported call target `{callee}`");
        }
        Flow::Normal
    }

    fn call_summary(&mut self, op: &Op, summary: CallSummary) {
        match summary {
            CallSummary::Malloc => self.call_malloc(op),
            CallSummary::Free => self.call_free(op),
            CallSummary::Memcpy | CallSummary::Memmove => self.call_memcpy(op),
            CallSummary::Memset => self.call_memset(op),
            CallSummary::Memchr => self.call_memchr(op),
            CallSummary::Strlen => self.call_strlen(op),
            CallSummary::Printf => self.call_printf(op),
            CallSummary::Fopen => self.call_fopen(op),
            CallSummary::Fputs => self.call_fputs(op),
            CallSummary::Fclose => self.call_fclose(op),
            CallSummary::Qsort => self.qsort(op),
            CallSummary::Bsearch => self.bsearch(op),
        }
    }

    fn call_malloc(&mut self, op: &Op) {
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

    fn call_free(&mut self, op: &Op) {
        let base = self.resolve_ref(&op.operands[0]);
        if !self.freed.insert(base.alloc) {
            panic!("effects::cir: double free of {:?}", base.alloc);
        }
        self.trace.push(Effect::Dealloc { alloc: base.alloc });
    }

    fn call_memcpy(&mut self, op: &Op) {
        let [dst, src, len] = op.operands.as_slice() else {
            panic!("effects::cir: memcpy/memmove expects three arguments");
        };
        let dst = self.resolve_ref(dst);
        let src = self.resolve_ref(src);
        let len = value_as_u64(self.resolve(len));
        let values = self.read_bytes(src, len);
        self.write_bytes(dst, &values);
        if let Some(result) = op.results.first() {
            self.env.insert(result.clone(), Value::Ref(dst));
        }
    }

    fn call_memset(&mut self, op: &Op) {
        let [dst, byte, len] = op.operands.as_slice() else {
            panic!("effects::cir: memset expects three arguments");
        };
        let dst = self.resolve_ref(dst);
        let byte = self.resolve_int(byte).0 as u8;
        let len = value_as_u64(self.resolve(len));
        let value = Value::Int {
            width: IntWidth::W8,
            signed: true,
            value: byte as i128,
        };
        self.write_bytes(dst, &vec![value; len as usize]);
        if let Some(result) = op.results.first() {
            self.env.insert(result.clone(), Value::Ref(dst));
        }
    }

    fn call_memchr(&mut self, op: &Op) {
        let [base, needle, len] = op.operands.as_slice() else {
            panic!("effects::cir: memchr expects three arguments");
        };
        let result = first_result(op);
        let base = self.resolve_ref(base);
        let needle = self.resolve_int(needle).0 as u8;
        let len = value_as_u64(self.resolve(len));
        let found = self.memchr(base, needle, len);
        self.env.insert(result.to_string(), found);
    }

    fn call_strlen(&mut self, op: &Op) {
        let base = self.resolve_ref(&op.operands[0]);
        let mut len = 0u64;
        loop {
            let loc = Location {
                alloc: base.alloc,
                byte_offset: base.byte_offset + len,
            };
            match self.heap.get(&loc) {
                Some(Value::Int { value: 0, .. }) => break,
                Some(_) => len += 1,
                None => panic!("effects::cir: strlen scanned past never-written {loc:?}"),
            }
        }
        self.trace.push(Effect::Call {
            name: "strlen".to_string(),
            args: vec![],
        });
        let result = first_result(op);
        let (signed, bits) = result_type(op)
            .and_then(int_type_width_signed)
            .unwrap_or((false, 64));
        self.env.insert(
            result.to_string(),
            Value::Int {
                width: int_width(bits),
                signed,
                value: len as i128,
            },
        );
    }

    fn call_printf(&mut self, op: &Op) {
        let args = op.operands[1..]
            .iter()
            .map(|name| self.resolve(name))
            .collect();
        self.trace.push(Effect::Call {
            name: "printf".to_string(),
            args,
        });
        self.store_i32_result(op, 0);
    }

    fn call_fopen(&mut self, op: &Op) {
        let path = self.c_string_operand(&op.operands[0]);
        let mode = self.c_string_operand(&op.operands[1]);
        let file = FileId(self.next_file);
        self.next_file += 1;
        self.trace.push(Effect::FileOpen { file, path, mode });
        let result = first_result(op);
        self.env.insert(result.to_string(), Value::File(file));
    }

    fn call_fputs(&mut self, op: &Op) {
        let bytes = self.c_string_operand_bytes(&op.operands[0]);
        let file = self.resolve_file(&op.operands[1]);
        self.trace.push(Effect::FileWrite { file, bytes });
        self.store_i32_result(op, 0);
    }

    fn call_fclose(&mut self, op: &Op) {
        let file = self.resolve_file(&op.operands[0]);
        self.trace.push(Effect::FileClose { file });
        self.store_i32_result(op, 0);
    }

    fn store_i32_result(&mut self, op: &Op, value: i32) {
        if let Some(result) = op.results.first() {
            self.env.insert(
                result.clone(),
                Value::Int {
                    width: IntWidth::W32,
                    signed: true,
                    value: value as i128,
                },
            );
        }
    }

    fn call_user_target(&mut self, op: &Op, name: &str) {
        let args = op
            .operands
            .iter()
            .map(|operand| self.resolve(operand))
            .collect::<Vec<_>>();
        let value = self.call_user(name, &args);
        let result = first_result(op);
        if !result.is_empty() {
            self.env.insert(result.to_string(), value);
        }
    }

    fn call_user(&mut self, name: &str, args: &[Value]) -> Value {
        let f = self.funcs[name].clone();
        let entry = &f.regions[0].blocks[0];
        if entry.args.len() != args.len() {
            panic!(
                "effects::cir: user function `{name}` expected {} arg(s), got {}",
                entry.args.len(),
                args.len()
            );
        }
        let saved_env = std::mem::take(&mut self.env);
        let saved_aggregate_bindings = self.aggregate_bindings.clone();
        for ((arg_name, _), value) in entry.args.iter().zip(args) {
            self.env.insert(arg_name.clone(), *value);
        }
        self.call_depth += 1;
        let flow = self.run_function_region(&f.regions[0]);
        self.call_depth -= 1;
        self.env = saved_env;
        self.aggregate_bindings = saved_aggregate_bindings;
        let Flow::Return(value) = flow else {
            panic!("effects::cir: user function `{name}` did not return");
        };
        value
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

    fn get_element(&mut self, op: &Op) -> Flow {
        let result = first_result(op);
        let base = self.resolve_ref(&op.operands[0]);
        let (index, ..) = self.resolve_int(&op.operands[1]);
        let elem_size = pointee_byte_size(result_type(op)).expect("get_element pointee size");
        let loc = Location {
            alloc: base.alloc,
            byte_offset: base.byte_offset + index as u64 * elem_size,
        };
        self.env.insert(result.to_string(), Value::Ref(loc));
        Flow::Normal
    }

    fn qsort(&mut self, op: &Op) {
        let [base, count, size, comparator] = op.operands.as_slice() else {
            panic!("effects::cir: qsort expects four arguments");
        };
        let base = self.resolve_ref(base);
        let len = value_as_u64(self.resolve(count));
        let elem_size = value_as_u64(self.resolve(size));
        let comparator = self
            .function_pointers
            .get(comparator)
            .unwrap_or_else(|| panic!("effects::cir: qsort comparator is unknown"))
            .clone();
        self.sort_by_callback(base, len, elem_size, &comparator);
    }

    fn bsearch(&mut self, op: &Op) {
        let [key, base, count, size, comparator] = op.operands.as_slice() else {
            panic!("effects::cir: bsearch expects five arguments");
        };
        let key = self.resolve_ref(key);
        let base = self.resolve_ref(base);
        let len = value_as_u64(self.resolve(count));
        let elem_size = value_as_u64(self.resolve(size));
        let comparator = self
            .function_pointers
            .get(comparator)
            .unwrap_or_else(|| panic!("effects::cir: bsearch comparator is unknown"))
            .clone();
        let result = first_result(op).to_string();
        for index in 0..len {
            let elem = Location {
                alloc: base.alloc,
                byte_offset: base.byte_offset + index * elem_size,
            };
            let cmp = self.call_user(&comparator, &[Value::Ref(key), Value::Ref(elem)]);
            if value_as_i128(cmp) == 0 {
                self.env.insert(result, Value::Ref(elem));
                return;
            }
        }
        self.env.insert(result, Value::Null);
    }

    fn sort_by_callback(&mut self, base: Location, len: u64, elem_size: u64, comparator: &str) {
        for i in 1..len {
            let mut j = i;
            while j > 0 {
                let left = Location {
                    alloc: base.alloc,
                    byte_offset: base.byte_offset + (j - 1) * elem_size,
                };
                let right = Location {
                    alloc: base.alloc,
                    byte_offset: base.byte_offset + j * elem_size,
                };
                let cmp = self.call_user(comparator, &[Value::Ref(left), Value::Ref(right)]);
                let cmp = value_as_i128(cmp);
                if cmp <= 0 {
                    break;
                }
                let left_value = *self
                    .heap
                    .get(&left)
                    .unwrap_or_else(|| panic!("effects::cir: read from never-written {left:?}"));
                let right_value = *self
                    .heap
                    .get(&right)
                    .unwrap_or_else(|| panic!("effects::cir: read from never-written {right:?}"));
                self.write_loc(left, right_value);
                self.write_loc(right, left_value);
                j -= 1;
            }
        }
    }

    fn ptr_diff(&mut self, op: &Op) -> Flow {
        let result = first_result(op);
        let lhs = self.resolve_ref(&op.operands[0]);
        let rhs = self.resolve_ref(&op.operands[1]);
        if lhs.alloc != rhs.alloc {
            panic!("effects::cir: ptr_diff across different allocations");
        }
        let elem_size = pointee_byte_size(result_type_for_operand(op.ty.as_deref(), 0))
            .expect("ptr_diff pointee size");
        let value = ((lhs.byte_offset as i128) - (rhs.byte_offset as i128)) / elem_size as i128;
        self.env.insert(
            result.to_string(),
            Value::Int {
                width: IntWidth::W64,
                signed: true,
                value,
            },
        );
        Flow::Normal
    }

    fn get_member(&mut self, op: &Op) -> Flow {
        let result = first_result(op);
        let base_name = &op.operands[0];
        let index =
            attr_int(op, "index_attr").expect("cir.get_member: missing index_attr") as usize;
        let base_ptr_ty = result_type_for_operand(op.ty.as_deref(), 0);
        let record_ty = base_ptr_ty
            .and_then(|ty| cir_ptr_pointee(ty, &self.type_aliases))
            .expect("cir.get_member: record pointer type");
        let field_offset = cir_struct_field_offset(record_ty, index, &self.type_aliases)
            .or_else(|| {
                result_type(op)
                    .and_then(|ty| cir_ptr_pointee(ty, &self.type_aliases))
                    .and_then(|ty| cir_type_size(ty, &self.type_aliases))
                    .map(|size| size * index as u64)
            })
            .unwrap_or(0);
        let base = if self.locals.contains(base_name) {
            self.aggregate_base(base_name)
        } else {
            self.resolve_ref(base_name)
        };
        let loc = Location {
            alloc: base.alloc,
            byte_offset: base.byte_offset + field_offset,
        };
        self.env.insert(result.to_string(), Value::Ref(loc));
        Flow::Normal
    }

    fn aggregate_base(&mut self, name: &str) -> Location {
        if let Some(binding) = self.aggregate_bindings.get(name)
            && binding.alloc != AllocId(u32::MAX)
        {
            return Location {
                alloc: binding.alloc,
                byte_offset: 0,
            };
        }
        let size = self
            .aggregate_bindings
            .get(name)
            .map(|binding| binding.size)
            .unwrap_or(0);
        let alloc = AllocId(self.next_alloc);
        self.next_alloc += 1;
        self.trace.push(Effect::Alloc { alloc, size });
        let binding = AggregateBinding { alloc, size };
        self.aggregate_bindings
            .insert(name.to_string(), binding.clone());
        self.aggregate_allocs.insert(alloc, binding);
        self.struct_alloc_slot
            .insert(alloc, self.trace.effects.len() - 1);
        let loc = Location {
            alloc,
            byte_offset: 0,
        };
        self.env.insert(name.to_string(), Value::Ref(loc));
        loc
    }

    fn store(&mut self, op: &Op) -> Flow {
        let value = if matches!(self.env.get(&op.operands[0]), Some(Value::Null))
            && self.c_strings.contains_key(&op.operands[0])
        {
            Value::Ref(self.hidden_c_string(&op.operands[0]).unwrap())
        } else {
            self.resolve(&op.operands[0])
        };
        let place = &op.operands[1];
        if self.locals.contains(place)
            && let Some(ordering) = attr_int(op, "mem_order")
        {
            self.atomic_store(place, store_ordering(ordering), value);
        } else if self.aggregate_bindings.contains_key(place) && matches!(value, Value::Ref(_)) {
            let dst = self.aggregate_base(place);
            let Value::Ref(src) = value else {
                unreachable!();
            };
            self.copy_aggregate(dst, src);
        } else if self.locals.contains(place) {
            self.env.insert(place.clone(), value);
        } else if matches!(value, Value::Ref(_)) && {
            let dst = self.resolve_ref(place);
            self.aggregate_allocs.contains_key(&dst.alloc)
        } {
            let dst = self.resolve_ref(place);
            let Value::Ref(src) = value else {
                unreachable!();
            };
            self.copy_aggregate(dst, src);
        } else {
            let loc = self.resolve_ref(place);
            if self.freed.contains(&loc.alloc) {
                panic!("effects::cir: write to {loc:?} after free");
            }
            self.heap.insert(loc, value);
            self.trace.push(Effect::Write { loc, value });
        }
        Flow::Normal
    }

    fn copy(&mut self, op: &Op) -> Flow {
        let dst = &op.operands[0];
        let src = &op.operands[1];
        if let Some(values) = self.arrays.get(src).cloned() {
            let alloc = self.ensure_local_array(dst, &values);
            let mut offset = 0;
            for value in &values {
                let loc = Location {
                    alloc,
                    byte_offset: offset,
                };
                self.heap.insert(loc, *value);
                self.trace.push(Effect::Write { loc, value: *value });
                offset += int_byte_size(value);
            }
            self.arrays.insert(dst.to_string(), values);
            return Flow::Normal;
        }
        let src = self.resolve_ref(src);
        let dst = if self.aggregate_bindings.contains_key(dst) {
            self.aggregate_base(dst)
        } else {
            self.resolve_ref(dst)
        };
        self.copy_aggregate(dst, src);
        Flow::Normal
    }

    fn libc_memchr(&mut self, op: &Op) -> Flow {
        let result = first_result(op);
        let base = self.resolve_ref(&op.operands[0]);
        let (needle, ..) = self.resolve_int(&op.operands[1]);
        let (len, ..) = self.resolve_int(&op.operands[2]);
        let found = self.memchr(base, needle as u8, len as u64);
        self.env.insert(result.to_string(), found);
        Flow::Normal
    }

    fn memchr(&mut self, base: Location, needle: u8, len: u64) -> Value {
        for index in 0..len {
            let loc = Location {
                alloc: base.alloc,
                byte_offset: base.byte_offset + index,
            };
            let value = *self
                .heap
                .get(&loc)
                .unwrap_or_else(|| panic!("effects::cir: memchr read from never-written {loc:?}"));
            self.trace.push(Effect::Read { loc, value });
            if value_as_u8(value) == needle {
                return Value::Ref(loc);
            }
        }
        Value::Null
    }

    fn read_bytes(&mut self, base: Location, len: u64) -> Vec<Value> {
        (0..len)
            .map(|index| {
                self.read_loc(Location {
                    alloc: base.alloc,
                    byte_offset: base.byte_offset + index,
                })
            })
            .collect()
    }

    fn write_bytes(&mut self, base: Location, values: &[Value]) {
        for (index, value) in values.iter().enumerate() {
            self.write_loc(
                Location {
                    alloc: base.alloc,
                    byte_offset: base.byte_offset + index as u64,
                },
                *value,
            );
        }
    }

    fn copy_aggregate(&mut self, dst: Location, src: Location) {
        let size = self
            .aggregate_allocs
            .get(&src.alloc)
            .or_else(|| self.aggregate_allocs.get(&dst.alloc))
            .map(|binding| binding.size)
            .unwrap_or(0);
        let mut offsets: Vec<u64> = self
            .heap
            .keys()
            .filter(|loc| loc.alloc == src.alloc && loc.byte_offset < src.byte_offset + size)
            .map(|loc| loc.byte_offset - src.byte_offset)
            .collect();
        offsets.sort_unstable();
        for offset in offsets {
            let src_loc = Location {
                alloc: src.alloc,
                byte_offset: src.byte_offset + offset,
            };
            let value = self.heap[&src_loc];
            let dst_loc = Location {
                alloc: dst.alloc,
                byte_offset: dst.byte_offset + offset,
            };
            self.heap.insert(dst_loc, value);
            self.trace.push(Effect::Write {
                loc: dst_loc,
                value,
            });
        }
    }

    fn get_bitfield(&mut self, op: &Op) -> Flow {
        let result = first_result(op);
        let loc = self.bitfield_location(op);
        let info = self.op_bitfield_info(op);
        let storage = self.heap.get(&loc).copied().unwrap_or(Value::Int {
            width: IntWidth::W32,
            signed: false,
            value: 0,
        });
        let raw = value_as_i128(storage);
        let value = truncate_bitfield(raw, info.size, info.signed, result_type(op));
        self.env.insert(result.to_string(), value);
        self.trace.push(Effect::Read { loc, value });
        Flow::Normal
    }

    fn set_bitfield(&mut self, op: &Op) -> Flow {
        let result = first_result(op);
        let loc = self.bitfield_location(op);
        let info = self.op_bitfield_info(op);
        let input = self.resolve(&op.operands[1]);
        let truncated = truncate_bitfield(
            value_as_i128(input),
            info.size,
            info.signed,
            result_type(op),
        );
        self.heap.insert(loc, truncated);
        self.env.insert(result.to_string(), truncated);
        self.trace.push(Effect::Write {
            loc,
            value: truncated,
        });
        Flow::Normal
    }

    fn op_bitfield_info(&self, op: &Op) -> BitfieldInfo {
        let raw = attr_str(op, "bitfield_info").expect("cir bitfield op missing bitfield_info");
        bitfield_info(raw, &self.type_aliases)
    }

    fn bitfield_location(&mut self, op: &Op) -> Location {
        let storage = self.resolve_ref(&op.operands[0]);
        let info = self.op_bitfield_info(op);
        let key = (storage.alloc, info.name);
        let field_size = bitfield_slot_size(op);
        let byte_offset = match self.bitfield_offsets.get(&key).copied() {
            Some(offset) => offset,
            None => {
                let offset = self
                    .bitfield_next_offsets
                    .get(&storage.alloc)
                    .copied()
                    .unwrap_or(0);
                self.bitfield_offsets.insert(key, offset);
                self.bitfield_next_offsets
                    .insert(storage.alloc, offset + field_size);
                offset
            }
        };
        let needed_size = byte_offset + field_size;
        if let Some(slot) = self.struct_alloc_slot.get(&storage.alloc).copied()
            && let Effect::Alloc { size, .. } = &mut self.trace.effects[slot]
        {
            *size = (*size).max(needed_size);
        }
        if let Some(binding) = self.aggregate_allocs.get_mut(&storage.alloc) {
            binding.size = binding.size.max(needed_size);
        }
        Location {
            alloc: storage.alloc,
            byte_offset,
        }
    }

    fn ensure_local_array(&mut self, name: &str, values: &[Value]) -> AllocId {
        if let Some(&alloc) = self.local_allocs.get(name) {
            return alloc;
        }
        let size = values.iter().map(int_byte_size).sum();
        let alloc = AllocId(self.next_alloc);
        self.next_alloc += 1;
        self.local_allocs.insert(name.to_string(), alloc);
        self.env.insert(
            name.to_string(),
            Value::Ref(Location {
                alloc,
                byte_offset: 0,
            }),
        );
        self.trace.push(Effect::Alloc { alloc, size });
        alloc
    }

    fn ensure_local_scalar_address(&mut self, name: &str, ty: Option<&str>) -> Location {
        let value = *self
            .env
            .get(name)
            .unwrap_or_else(|| panic!("effects::cir: address of uninitialized local `{name}`"));
        let alloc = match self.local_allocs.get(name).copied() {
            Some(alloc) => alloc,
            None => {
                let alloc = AllocId(self.next_alloc);
                self.next_alloc += 1;
                let size = pointee_byte_size(ty).unwrap_or_else(|| int_byte_size(&value));
                self.local_allocs.insert(name.to_string(), alloc);
                self.trace.push(Effect::Alloc { alloc, size });
                alloc
            }
        };
        let loc = Location {
            alloc,
            byte_offset: 0,
        };
        self.heap.insert(loc, value);
        self.trace.push(Effect::Write { loc, value });
        loc
    }

    fn ensure_local_array_address(&mut self, name: &str, ty: Option<&str>) -> Location {
        if let Some(&alloc) = self.local_allocs.get(name) {
            return Location {
                alloc,
                byte_offset: 0,
            };
        }
        let (elem_ty, len) = ty
            .and_then(|ty| ty.trim().strip_prefix("!cir.ptr<")?.strip_suffix('>'))
            .and_then(cir_array_ty)
            .unwrap_or_else(|| panic!("effects::cir: expected array pointer type for `{name}`"));
        let (_, bits) = int_type_width_signed(elem_ty)
            .unwrap_or_else(|| panic!("effects::cir: expected integer array element for `{name}`"));
        let alloc = AllocId(self.next_alloc);
        self.next_alloc += 1;
        self.local_allocs.insert(name.to_string(), alloc);
        self.trace.push(Effect::Alloc {
            alloc,
            size: len as u64 * (bits / 8) as u64,
        });
        let loc = Location {
            alloc,
            byte_offset: 0,
        };
        self.env.insert(name.to_string(), Value::Ref(loc));
        loc
    }

    fn hidden_c_string(&mut self, name: &str) -> Option<Location> {
        if let Some(&loc) = self.hidden_c_strings.get(name) {
            return Some(loc);
        }
        let bytes = self.c_strings.get(name)?;
        let alloc = AllocId(self.next_alloc);
        self.next_alloc += 1;
        let base = Location {
            alloc,
            byte_offset: 0,
        };
        for (index, byte) in bytes.iter().enumerate() {
            self.heap.insert(
                Location {
                    alloc,
                    byte_offset: index as u64,
                },
                Value::Int {
                    width: IntWidth::W8,
                    signed: true,
                    value: *byte as i128,
                },
            );
        }
        self.hidden_c_strings.insert(name.to_string(), base);
        Some(base)
    }

    fn load(&mut self, op: &Op) -> Flow {
        let result = first_result(op);
        let place = &op.operands[0];
        if self.locals.contains(place)
            && let Some(ordering) = attr_int(op, "mem_order")
        {
            let value = self.atomic_load(place, load_ordering(ordering));
            self.env.insert(result.to_string(), value);
        } else if self.aggregate_bindings.contains_key(place) {
            let loc = self.aggregate_base(place);
            self.env.insert(result.to_string(), Value::Ref(loc));
        } else if self.locals.contains(place) {
            let value = *self
                .env
                .get(place)
                .unwrap_or_else(|| panic!("effects::cir: load from uninitialized local `{place}`"));
            self.env.insert(result.to_string(), value);
        } else if self.load_result_is_aggregate(op) && {
            let loc = self.resolve_ref(place);
            self.aggregate_allocs.contains_key(&loc.alloc)
        } {
            let loc = self.resolve_ref(place);
            self.env.insert(result.to_string(), Value::Ref(loc));
        } else {
            let loc = self.resolve_ref(place);
            if self.freed.contains(&loc.alloc) {
                panic!("effects::cir: read from {loc:?} after free");
            }
            let value = *self
                .heap
                .get(&loc)
                .unwrap_or_else(|| panic!("effects::cir: read from never-written {loc:?}"));
            self.env.insert(result.to_string(), value);
            if !matches!(value, Value::BlockLabel(_)) {
                self.trace.push(Effect::Read { loc, value });
            }
        }
        Flow::Normal
    }

    fn read_loc(&mut self, loc: Location) -> Value {
        let value = *self
            .heap
            .get(&loc)
            .unwrap_or_else(|| panic!("effects::cir: read from never-written {loc:?}"));
        if !matches!(value, Value::BlockLabel(_)) {
            self.trace.push(Effect::Read { loc, value });
        }
        value
    }

    fn load_result_is_aggregate(&self, op: &Op) -> bool {
        result_type(op).is_some_and(|ty| {
            let ty = expand_type_alias(ty, &self.type_aliases).trim();
            ty.starts_with("!cir.struct<") || ty.starts_with("!cir.array<")
        })
    }

    fn write_loc(&mut self, loc: Location, value: Value) {
        if self.freed.contains(&loc.alloc) {
            panic!("effects::cir: write to {loc:?} after free");
        }
        self.heap.insert(loc, value);
        self.trace.push(Effect::Write { loc, value });
    }

    fn atomic_fetch(&mut self, op: &Op) -> Flow {
        let result = first_result(op);
        let place = &op.operands[0];
        let operand = self.resolve(&op.operands[1]);
        let atomic = self.atomic_for_name(place);
        let old = self.atomic_value(atomic);
        let op_kind = atomic_rmw_op(attr_int(op, "binop").unwrap_or(0));
        let new = atomic_rmw_value(op_kind, old, operand);
        self.atomic_values.insert(atomic, new);
        self.trace.push(Effect::AtomicRmw {
            atomic,
            op: op_kind,
            ordering: rust_ordering(attr_int(op, "mem_order").unwrap_or(5)),
            operand,
            old,
            new,
        });
        let value = if attr_str(op, "fetch_first") == Some("false") {
            new
        } else {
            old
        };
        self.env.insert(result.to_string(), value);
        Flow::Normal
    }

    fn atomic_xchg(&mut self, op: &Op) -> Flow {
        let result = first_result(op);
        let place = &op.operands[0];
        let new = self.resolve(&op.operands[1]);
        let atomic = self.atomic_for_name(place);
        let old = self.atomic_value(atomic);
        self.atomic_values.insert(atomic, new);
        self.trace.push(Effect::AtomicSwap {
            atomic,
            ordering: rust_ordering(attr_int(op, "mem_order").unwrap_or(5)),
            old,
            new,
        });
        self.env.insert(result.to_string(), old);
        Flow::Normal
    }

    fn atomic_cmpxchg(&mut self, op: &Op) -> Flow {
        let place = &op.operands[0];
        let expected = self.resolve(&op.operands[1]);
        let desired = self.resolve(&op.operands[2]);
        let atomic = self.atomic_for_name(place);
        let old = self.atomic_value(atomic);
        let exchanged = old == expected;
        if exchanged {
            self.atomic_values.insert(atomic, desired);
        }
        self.trace.push(Effect::AtomicCompareExchange {
            atomic,
            success: rust_ordering(attr_int(op, "succ_order").unwrap_or(5)),
            failure: load_ordering(attr_int(op, "fail_order").unwrap_or(5)),
            expected,
            desired,
            old,
            exchanged,
        });
        if let Some(result) = op.results.first() {
            self.env.insert(result.clone(), old);
        }
        if let Some(result) = op.results.get(1) {
            self.env.insert(result.clone(), Value::Bool(exchanged));
        }
        Flow::Normal
    }

    fn atomic_fence(&mut self, op: &Op) -> Flow {
        let ordering = rust_ordering(attr_int(op, "ordering").unwrap_or(5));
        self.trace.push(Effect::AtomicFence { ordering });
        Flow::Normal
    }

    fn atomic_store(&mut self, name: &str, ordering: AtomicOrdering, value: Value) {
        let atomic = self.atomic_for_name(name);
        self.env.insert(name.to_string(), value);
        self.atomic_values.insert(atomic, value);
        self.trace.push(Effect::AtomicStore {
            atomic,
            ordering,
            value,
        });
    }

    fn atomic_load(&mut self, name: &str, ordering: AtomicOrdering) -> Value {
        let atomic = self.atomic_for_name(name);
        let value = self.atomic_value(atomic);
        self.trace.push(Effect::AtomicLoad {
            atomic,
            ordering,
            value,
        });
        value
    }

    fn atomic_for_name(&mut self, name: &str) -> AtomicId {
        if let Some(&atomic) = self.atomics.get(name) {
            return atomic;
        }
        let value = *self
            .env
            .get(name)
            .unwrap_or_else(|| panic!("effects::cir: atomic access to unknown `{name}`"));
        let atomic = AtomicId(self.next_atomic);
        self.next_atomic += 1;
        self.atomics.insert(name.to_string(), atomic);
        self.atomic_values.insert(atomic, value);
        atomic
    }

    fn atomic_value(&self, atomic: AtomicId) -> Value {
        *self
            .atomic_values
            .get(&atomic)
            .unwrap_or_else(|| panic!("effects::cir: read from unknown atomic {atomic:?}"))
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
                Flow::Break => return Flow::Normal,
                Flow::Continue => {}
                flow @ Flow::Jump(_) => return flow,
                flow @ Flow::Return(_) => return flow,
            }
            match self.run_region(&op.regions[2]) {
                Flow::Normal | Flow::Continue => {}
                Flow::Break => return Flow::Normal,
                flow @ Flow::Jump(_) => return flow,
                flow @ Flow::Return(_) => return flow,
            }
        }
    }

    fn while_(&mut self, op: &Op) -> Flow {
        loop {
            if !self.eval_condition_region(&op.regions[0]) {
                return Flow::Normal;
            }
            match self.run_region(&op.regions[1]) {
                Flow::Normal | Flow::Continue => {}
                Flow::Break => return Flow::Normal,
                flow @ Flow::Jump(_) => return flow,
                flow @ Flow::Return(_) => return flow,
            }
        }
    }

    fn do_(&mut self, op: &Op) -> Flow {
        loop {
            match self.run_region(&op.regions[0]) {
                Flow::Normal | Flow::Continue => {}
                Flow::Break => return Flow::Normal,
                flow @ Flow::Return(_) => return flow,
                flow @ Flow::Jump(_) => return flow,
            }
            if !self.eval_condition_region(&op.regions[1]) {
                return Flow::Normal;
            }
        }
    }

    fn switch_(&mut self, op: &Op) -> Flow {
        let selector = value_as_i128(self.resolve(&op.operands[0]));
        let cases: Vec<_> = op
            .regions
            .first()
            .into_iter()
            .flat_map(|region| region.blocks.iter())
            .flat_map(|block| block.ops.iter())
            .filter(|op| op.kind() == CirOpKind::Case)
            .collect();
        let Some(mut index) = self.switch_start_case(selector, &cases) else {
            return Flow::Normal;
        };
        while let Some(case) = cases.get(index) {
            match self.run_region(&case.regions[0]) {
                Flow::Normal => index += 1,
                Flow::Break => return Flow::Normal,
                flow @ (Flow::Continue | Flow::Return(_) | Flow::Jump(_)) => {
                    return flow;
                }
            }
        }
        Flow::Normal
    }

    fn switch_start_case(&self, selector: i128, cases: &[&Op]) -> Option<usize> {
        let mut default = None;
        for (index, case) in cases.iter().enumerate() {
            if attr_int(case, "kind") == Some(0) {
                default = Some(index);
                continue;
            }
            if self.switch_case_values(case).contains(&selector) {
                return Some(index);
            }
        }
        default
    }

    fn switch_case_values(&self, op: &Op) -> Vec<i128> {
        match op.attrs.get("value") {
            Some(Attr::Array(values)) => values.iter().filter_map(attr_int_literal).collect(),
            _ => Vec::new(),
        }
    }

    fn select(&mut self, op: &Op) -> Flow {
        let result = first_result(op);
        let value = if self.resolve_bool(&op.operands[0]) {
            self.resolve(&op.operands[1])
        } else {
            self.resolve(&op.operands[2])
        };
        self.env.insert(result.to_string(), value);
        Flow::Normal
    }

    fn ternary(&mut self, op: &Op) -> Flow {
        let result = first_result(op);
        let region = if self.resolve_bool(&op.operands[0]) {
            &op.regions[0]
        } else {
            &op.regions[1]
        };
        match self.eval_yield_region(region) {
            Ok(value) => {
                self.env.insert(result.to_string(), value);
                Flow::Normal
            }
            Err(flow) => flow,
        }
    }

    fn goto(&mut self, op: &Op) -> Flow {
        let label = attr_str(op, "label").unwrap_or_default();
        let target = *self
            .label_to_index
            .get(label)
            .unwrap_or_else(|| panic!("effects::cir: unknown goto label `{label}`"));
        Flow::Jump(target)
    }

    fn br(&mut self, op: &Op) -> Flow {
        let target = if let Some(successor) = op.successors.first() {
            *self
                .block_to_index
                .get(successor)
                .unwrap_or_else(|| panic!("effects::cir: unknown successor block `{successor}`"))
        } else if let Some(operand) = op.operands.first() {
            self.block_label_target(self.resolve(operand))
        } else {
            panic!("effects::cir: cir.br has no successor")
        };
        if !op.operands.is_empty() && !op.successors.is_empty() {
            let values = op
                .operands
                .iter()
                .map(|operand| self.resolve(operand))
                .collect();
            self.pending_block_args.insert(target, values);
        }
        Flow::Jump(target)
    }

    fn indirect_br(&mut self, op: &Op) -> Flow {
        let target = self.block_label_target(self.resolve(&op.operands[0]));
        Flow::Jump(target)
    }

    fn block_label_target(&self, value: Value) -> usize {
        let Value::BlockLabel(label) = value else {
            panic!("effects::cir: indirect branch target was {value:?}");
        };
        *self
            .label_to_index
            .get(label)
            .unwrap_or_else(|| panic!("effects::cir: unknown indirect branch label `{label}`"))
    }

    fn eval_condition_region(&mut self, region: &Region) -> bool {
        for block in &region.blocks {
            for op in &block.ops {
                if matches!(op.kind(), CirOpKind::Condition | CirOpKind::Yield) {
                    return self.resolve_bool(&op.operands[0]);
                }
                if let flow @ (Flow::Return(_) | Flow::Break | Flow::Continue | Flow::Jump(_)) =
                    self.step(op)
                {
                    panic!("effects::cir: control flow escaped condition region: {flow:?}");
                }
            }
        }
        panic!("effects::cir: loop condition region has no cir.condition/cir.yield terminator")
    }

    fn eval_yield_region(&mut self, region: &Region) -> Result<Value, Flow> {
        for block in &region.blocks {
            for op in &block.ops {
                if op.kind() == CirOpKind::Yield {
                    let operand = op
                        .operands
                        .first()
                        .unwrap_or_else(|| panic!("effects::cir: value yield has no operand"));
                    return Ok(self.resolve(operand));
                }
                match self.step(op) {
                    Flow::Normal => {}
                    flow => return Err(flow),
                }
            }
        }
        panic!("effects::cir: value region has no cir.yield terminator")
    }

    fn resolve(&self, name: &str) -> Value {
        *self
            .env
            .get(name)
            .unwrap_or_else(|| panic!("effects::cir: use of unknown value `{name}`"))
    }

    fn resolve_int(&self, name: &str) -> (i128, IntWidth, bool) {
        match self.resolve(name) {
            Value::Int {
                width,
                signed,
                value,
            } => (value, width, signed),
            other => panic!("effects::cir: expected int value for {name}, found {other:?}"),
        }
    }

    fn resolve_ref(&mut self, name: &str) -> Location {
        match self.resolve(name) {
            Value::Ref(loc) => loc,
            Value::Null => self.hidden_c_string(name).unwrap_or_else(|| {
                panic!("effects::cir: expected pointer value for {name}, found Null")
            }),
            other => panic!("effects::cir: expected pointer value for {name}, found {other:?}"),
        }
    }

    fn resolve_file(&self, name: &str) -> FileId {
        match self.resolve(name) {
            Value::File(file) => file,
            other => panic!("effects::cir: expected file handle for {name}, found {other:?}"),
        }
    }

    fn resolve_bool(&self, name: &str) -> bool {
        match self.resolve(name) {
            Value::Bool(value) => value,
            other => panic!("effects::cir: expected bool value for {name}, found {other:?}"),
        }
    }

    fn c_string_operand(&self, name: &str) -> String {
        String::from_utf8_lossy(&self.c_string_operand_bytes(name)).into_owned()
    }

    fn c_string_operand_bytes(&self, name: &str) -> Vec<u8> {
        let bytes = self
            .c_strings
            .get(name)
            .unwrap_or_else(|| panic!("effects::cir: expected `{name}` to be a C string literal"));
        bytes
            .iter()
            .copied()
            .take_while(|byte| *byte != 0)
            .collect()
    }
}

fn attr_int_literal(attr: &Attr) -> Option<i128> {
    match attr {
        Attr::Int(n) => Some(*n as i128),
        Attr::Raw(raw) => {
            let start = raw.find("#cir.int<")? + "#cir.int<".len();
            let rest = &raw[start..];
            let end = rest.find('>')?;
            rest[..end].parse().ok()
        }
        _ => None,
    }
}

#[cfg(test)]
mod tests;
