use std::cmp::Reverse;
use std::collections::{HashMap, HashSet};

use super::support::*;
use crate::effects::{
    AllocId, AtomicId, CallSummary, Effect, EffectTrace, FileId, IntWidth, Location, OptionValue,
    ParamSeed, Value, call_summary,
};
use crate::rust_ast::{
    AtomicOrdering, AtomicPlace, AtomicRmwOp, BinOp, Block, Expr, FnDef, IndentStmt, Item, Pattern,
    Program, Stmt, Type, UnaryOp,
};

pub fn interpret(f: &FnDef) -> EffectTrace {
    interpret_with_params(f, &[])
}

pub fn interpret_with_params(f: &FnDef, params: &[(&str, ParamSeed)]) -> EffectTrace {
    let mut interp = Interp::default();
    interp.seed_params(params);
    let _ = interp.run(&f.body);
    interp.trace
}

pub fn interpret_program_main(program: &Program) -> EffectTrace {
    let mut interp = Interp::default();
    interp.seed_program(program);
    let main = interp
        .funcs
        .get("main")
        .cloned()
        .expect("fixture must define `main`");
    if interp.run(&main.body) == Flow::Normal {
        interp.trace.push(Effect::Exit(0));
    }
    interp.trace
}

/// How a statement completed: either it ran through normally, or it hit a
/// `return`/`std::process::exit` that the enclosing `if`/`for` must
/// propagate past without running the rest of their body.
#[derive(Debug, Clone, Copy, PartialEq)]
enum Flow {
    Normal,
    Return(Value),
    Break,
    Continue,
}

struct VecBinding {
    alloc: AllocId,
    elem_width: IntWidth,
    elem_signed: bool,
    elem_size: u64,
    len: u64,
    owned: bool,
}

struct StructBinding {
    alloc: AllocId,
    field_offsets: HashMap<String, u64>,
}

struct OnceLockBinding {
    guard: Location,
    payload: Location,
}

#[derive(Default)]
struct Interp {
    vecs: HashMap<String, VecBinding>,
    structs: HashMap<String, StructBinding>,
    globals: HashMap<String, Location>,
    once_locks: HashMap<String, OnceLockBinding>,
    funcs: HashMap<String, FnDef>,
    scalars: HashMap<String, Value>,
    files: HashMap<String, FileId>,
    atomics: HashMap<String, AtomicId>,
    atomic_values: HashMap<AtomicId, Value>,
    heap: HashMap<Location, Value>,
    next_alloc: u32,
    next_file: u32,
    next_atomic: u32,
    trace: EffectTrace,
    freed: HashSet<AllocId>,
    call_depth: usize,
}

impl Interp {
    fn seed_program(&mut self, program: &Program) {
        for item in &program.items {
            match item {
                Item::Static { name, ty, init, .. } => self.seed_static(name, ty, init),
                Item::Fn(f) => {
                    self.funcs.insert(f.name.clone(), f.clone());
                }
                _ => {}
            }
        }
    }

    fn seed_static(&mut self, name: &str, ty: &Type, init: &Expr) {
        if is_once_lock_ty(ty) {
            let zero = Value::Int {
                width: IntWidth::W32,
                signed: true,
                value: 0,
            };
            let guard_alloc = AllocId(self.next_alloc);
            self.next_alloc += 1;
            let guard = Location {
                alloc: guard_alloc,
                byte_offset: 0,
            };
            self.heap.insert(guard, zero);
            self.trace.push(Effect::Alloc {
                alloc: guard_alloc,
                size: 4,
            });
            self.trace.push(Effect::Write {
                loc: guard,
                value: zero,
            });
            let payload_alloc = AllocId(self.next_alloc);
            self.next_alloc += 1;
            let payload = Location {
                alloc: payload_alloc,
                byte_offset: 0,
            };
            self.heap.insert(payload, zero);
            self.trace.push(Effect::Alloc {
                alloc: payload_alloc,
                size: 4,
            });
            self.trace.push(Effect::Write {
                loc: payload,
                value: zero,
            });
            self.once_locks
                .insert(name.to_string(), OnceLockBinding { guard, payload });
            return;
        }
        let Some((_, _, size)) = scalar_type_shape(ty) else {
            return;
        };
        let value = self.eval(init);
        let alloc = AllocId(self.next_alloc);
        self.next_alloc += 1;
        let loc = Location {
            alloc,
            byte_offset: 0,
        };
        self.globals.insert(name.to_string(), loc);
        self.heap.insert(loc, value);
        self.trace.push(Effect::Alloc { alloc, size });
        self.trace.push(Effect::Write { loc, value });
    }

    fn seed_params(&mut self, params: &[(&str, ParamSeed)]) {
        for (name, seed) in params {
            match seed {
                ParamSeed::Scalar(v) => {
                    self.scalars.insert(name.to_string(), *v);
                }
                ParamSeed::Buffer(elems) => self.seed_buffer(name, elems),
            }
        }
    }

    fn seed_buffer(&mut self, name: &str, elems: &[Value]) {
        let alloc = AllocId(self.next_alloc);
        self.next_alloc += 1;
        let (elem_width, elem_signed, elem_size) = elems
            .first()
            .map(|elem| match elem {
                Value::Int { width, signed, .. } => (*width, *signed, int_byte_size(elem)),
                other => {
                    panic!("effects::rust_ast: buffer element must be an integer, found {other:?}")
                }
            })
            .unwrap_or((IntWidth::W32, true, 4));
        for (index, elem) in elems.iter().enumerate() {
            let loc = Location {
                alloc,
                byte_offset: index as u64 * elem_size,
            };
            self.heap.insert(loc, *elem);
        }
        self.vecs.insert(
            name.to_string(),
            VecBinding {
                alloc,
                elem_width,
                elem_signed,
                elem_size,
                len: elems.len() as u64,
                owned: false,
            },
        );
    }

    fn run(&mut self, body: &[IndentStmt]) -> Flow {
        for stmt in body {
            match self.step(&stmt.stmt) {
                Flow::Normal => {}
                flow => return flow,
            }
        }
        Flow::Normal
    }

    fn step(&mut self, stmt: &Stmt) -> Flow {
        match stmt {
            Stmt::Let {
                name,
                ty: Some(ty),
                init: Some(init),
                ..
            } => {
                match init {
                    Expr::StructLit { fields, .. } => self.let_struct(name, fields),
                    Expr::Call { func, args } if is_path(func, &["String", "from"]) => {
                        self.let_string(name, args)
                    }
                    _ if vec_elem_shape(ty).is_some() => self.let_vec(name, ty, init),
                    _ if array_elem_shape(ty).is_some() => self.let_array(name, ty, init),
                    Expr::CStr(bytes) if is_cstr_ref_ty(ty) => self.let_cstr(name, bytes),
                    _ => {
                        let value = self.eval(init);
                        self.scalars.insert(name.clone(), value);
                    }
                }
                Flow::Normal
            }
            Stmt::Let {
                name,
                ty: None,
                init: Some(init),
                ..
            } => {
                if let Some(file) = self.open_file(init) {
                    self.files.insert(name.clone(), file);
                    self.scalars.insert(name.clone(), Value::File(file));
                } else if let Expr::AtomicNew { value, .. } = init {
                    let value = self.eval(value);
                    let atomic = self.define_atomic(name, value);
                    self.scalars.insert(name.clone(), Value::Atomic(atomic));
                } else {
                    let value = self.eval(init);
                    self.scalars.insert(name.clone(), value);
                }
                Flow::Normal
            }
            Stmt::Expr(Expr::MethodCall { recv, method, args }) if method == "push" => {
                self.push(recv, args);
                Flow::Normal
            }
            Stmt::Expr(Expr::Call { func, args }) if is_path(func, &["std", "process", "exit"]) => {
                let code = value_as_i32(self.eval(&args[0]));
                self.drop_live_vecs();
                self.trace.push(Effect::Exit(code));
                Flow::Return(Value::Int {
                    width: IntWidth::W32,
                    signed: true,
                    value: code as i128,
                })
            }
            Stmt::Expr(Expr::Call { func, args }) if is_path(func, &["drop"]) => {
                self.drop_var(&args[0]);
                Flow::Normal
            }
            Stmt::Expr(Expr::MethodCall { recv, method, args })
                if method == "unwrap" && args.is_empty() && self.write_all_call(recv) =>
            {
                Flow::Normal
            }
            Stmt::Expr(Expr::Macro { name, args }) if name == "println" || name == "print" => {
                self.print(args);
                Flow::Normal
            }
            Stmt::Expr(expr) => {
                self.eval(expr);
                Flow::Normal
            }
            Stmt::Unsafe { body } => self.run_block(body),
            Stmt::Scope { body } => self.run(body),
            Stmt::Loop { body, .. } => self.run_loop(body),
            Stmt::Break(_) => Flow::Break,
            Stmt::Continue(_) => Flow::Continue,
            Stmt::Assign { target, value } => self.assign(target, value),
            Stmt::CompoundAssign { target, op, value } => self.compound_assign(target, *op, value),
            Stmt::If {
                cond,
                then_body,
                else_body,
            } => {
                if value_as_bool(self.eval(cond)) {
                    self.run(then_body)
                } else {
                    self.run(else_body)
                }
            }
            Stmt::For { pat, iter, body } => self.run_for(pat, iter, body),
            Stmt::Return(value) => {
                let code = value
                    .as_ref()
                    .map(|expr| value_as_i32(self.eval(expr)))
                    .unwrap_or(0);
                let value = Value::Int {
                    width: IntWidth::W32,
                    signed: true,
                    value: code as i128,
                };
                if self.call_depth == 0 {
                    self.drop_live_vecs();
                    self.trace.push(Effect::Exit(code));
                }
                Flow::Return(value)
            }
            other => panic!("effects::rust_ast: unsupported stmt `{other:?}`"),
        }
    }

    fn run_block(&mut self, block: &Block) -> Flow {
        match self.run(&block.stmts) {
            Flow::Normal => {
                if let Some(tail) = &block.tail {
                    let value = self.eval(tail);
                    Flow::Return(value)
                } else {
                    Flow::Normal
                }
            }
            flow => flow,
        }
    }

    fn run_loop(&mut self, body: &[IndentStmt]) -> Flow {
        loop {
            match self.run(body) {
                Flow::Normal | Flow::Continue => {}
                Flow::Break => return Flow::Normal,
                flow @ Flow::Return(_) => return flow,
            }
        }
    }

    fn assign(&mut self, target: &Expr, value: &Expr) -> Flow {
        match target {
            Expr::Var(ident) => {
                if self.vecs.contains_key(ident.as_str())
                    && matches!(value, Expr::ArrayLit(_) | Expr::ArrayRepeat { .. })
                {
                    self.assign_array(ident.as_str(), value);
                    return Flow::Normal;
                }
                let v = self.eval(value);
                if let Some(loc) = self.globals.get(ident.as_str()).copied() {
                    self.heap.insert(loc, v);
                    self.trace.push(Effect::Write { loc, value: v });
                } else {
                    self.scalars.insert(ident.as_str().to_string(), v);
                }
            }
            Expr::Index { base, index } => self.assign_index(base, index, value),
            Expr::Field { base, field } => self.assign_field(base, field, value),
            other => panic!("effects::rust_ast: unsupported assign target `{other:?}`"),
        }
        Flow::Normal
    }

    fn assign_array(&mut self, name: &str, value: &Expr) {
        let binding = self
            .vecs
            .get(name)
            .unwrap_or_else(|| panic!("effects::rust_ast: assign to unknown array `{name}`"));
        let (elem_width, elem_signed, elem_size, len, alloc) = (
            binding.elem_width,
            binding.elem_signed,
            binding.elem_size,
            binding.len,
            binding.alloc,
        );
        let values: Vec<Value> = match value {
            Expr::ArrayLit(elems) => elems
                .iter()
                .map(|elem| Value::Int {
                    width: elem_width,
                    signed: elem_signed,
                    value: value_as_i128(self.eval(elem)),
                })
                .collect(),
            Expr::ArrayRepeat { elem, len } => {
                let value = Value::Int {
                    width: elem_width,
                    signed: elem_signed,
                    value: value_as_i128(self.eval(elem)),
                };
                if value_as_i128(value) == 0 {
                    Vec::new()
                } else {
                    vec![value; *len]
                }
            }
            other => panic!("effects::rust_ast: unsupported array assignment `{other:?}`"),
        };
        if !values.is_empty() && values.len() as u64 != len {
            panic!("effects::rust_ast: array assignment length does not match binding");
        }
        for (index, value) in values.into_iter().enumerate() {
            let loc = Location {
                alloc,
                byte_offset: index as u64 * elem_size,
            };
            self.heap.insert(loc, value);
            self.trace.push(Effect::Write { loc, value });
        }
    }

    fn compound_assign(&mut self, target: &Expr, op: BinOp, value: &Expr) -> Flow {
        match target {
            Expr::Var(ident) => {
                let name = ident.as_str().to_string();
                let current = *self.scalars.get(&name).unwrap_or_else(|| {
                    panic!("effects::rust_ast: compound-assign to unknown scalar `{name}`")
                });
                let rhs = self.eval(value);
                self.scalars.insert(name, apply_binop(op, current, rhs));
            }
            Expr::Index { base, index } => {
                let name = match base.as_ref() {
                    Expr::Var(ident) => ident.as_str(),
                    other => {
                        panic!("effects::rust_ast: unsupported compound-assign base `{other:?}`")
                    }
                };
                let idx = value_as_u64(self.eval(index));
                let binding = self.vecs.get(name).unwrap_or_else(|| {
                    panic!("effects::rust_ast: compound-assign into unknown Vec `{name}`")
                });
                if self.freed.contains(&binding.alloc) {
                    panic!("effects::rust_ast: compound-assign to {name} after free");
                }
                let loc = Location {
                    alloc: binding.alloc,
                    byte_offset: idx * binding.elem_size,
                };
                let current = *self.heap.get(&loc).unwrap_or_else(|| {
                    panic!("effects::rust_ast: read from never-written {loc:?}")
                });
                self.trace.push(Effect::Read {
                    loc,
                    value: current,
                });
                let rhs = self.eval(value);
                let updated = apply_binop(op, current, rhs);
                self.heap.insert(loc, updated);
                self.trace.push(Effect::Write {
                    loc,
                    value: updated,
                });
            }
            other => panic!("effects::rust_ast: unsupported compound-assign target `{other:?}`"),
        }
        Flow::Normal
    }

    fn run_for(&mut self, pat: &str, iter: &Expr, body: &[IndentStmt]) -> Flow {
        let (start, end) = match iter {
            Expr::Range { start, end } => (
                value_as_i128(self.eval(start)),
                value_as_i128(self.eval(end)),
            ),
            other => panic!("effects::rust_ast: unsupported for-loop iterator `{other:?}`"),
        };
        let mut i = start;
        while i < end {
            self.scalars.insert(
                pat.to_string(),
                Value::Int {
                    width: IntWidth::W32,
                    signed: true,
                    value: i,
                },
            );
            match self.run(body) {
                Flow::Normal | Flow::Continue => {}
                Flow::Break => return Flow::Normal,
                flow @ Flow::Return(_) => return flow,
            }
            i += 1;
        }
        Flow::Normal
    }

    fn let_vec(&mut self, name: &str, ty: &Type, init: &Expr) {
        let (elem_width, elem_signed, elem_size) = vec_elem_shape(ty)
            .unwrap_or_else(|| panic!("effects::rust_ast: expected `Vec<T>` local, found {ty:?}"));
        let capacity = match init {
            Expr::Call { func, args } if is_path(func, &["Vec", "with_capacity"]) => {
                value_as_u64(self.eval(&args[0]))
            }
            Expr::VecRepeat { len, .. } => value_as_u64(self.eval(len)),
            other => panic!("effects::rust_ast: unsupported Vec initializer `{other:?}`"),
        };
        let alloc = AllocId(self.next_alloc);
        self.next_alloc += 1;
        self.trace.push(Effect::Alloc {
            alloc,
            size: capacity * elem_size,
        });
        self.vecs.insert(
            name.to_string(),
            VecBinding {
                alloc,
                elem_width,
                elem_signed,
                elem_size,
                len: 0,
                owned: true,
            },
        );
    }

    fn let_array(&mut self, name: &str, ty: &Type, init: &Expr) {
        let (elem_width, elem_signed, elem_size, len) = array_elem_shape(ty)
            .unwrap_or_else(|| panic!("effects::rust_ast: expected array local, found {ty:?}"));
        let values: Vec<Value> = match init {
            Expr::ArrayLit(elems) => elems
                .iter()
                .map(|elem| Value::Int {
                    width: elem_width,
                    signed: elem_signed,
                    value: value_as_i128(self.eval(elem)),
                })
                .collect(),
            Expr::ArrayRepeat { elem, len } => {
                let value = Value::Int {
                    width: elem_width,
                    signed: elem_signed,
                    value: value_as_i128(self.eval(elem)),
                };
                if value_as_i128(value) == 0 {
                    Vec::new()
                } else {
                    vec![value; *len]
                }
            }
            other => panic!("effects::rust_ast: unsupported array initializer `{other:?}`"),
        };
        if !values.is_empty() && values.len() as u64 != len {
            panic!("effects::rust_ast: array initializer length does not match type");
        }
        let alloc = AllocId(self.next_alloc);
        self.next_alloc += 1;
        self.trace.push(Effect::Alloc {
            alloc,
            size: len * elem_size,
        });
        for (index, value) in values.iter().enumerate() {
            let loc = Location {
                alloc,
                byte_offset: index as u64 * elem_size,
            };
            self.heap.insert(loc, *value);
            self.trace.push(Effect::Write { loc, value: *value });
        }
        self.vecs.insert(
            name.to_string(),
            VecBinding {
                alloc,
                elem_width,
                elem_signed,
                elem_size,
                len,
                owned: false,
            },
        );
    }

    fn let_cstr(&mut self, name: &str, bytes: &[u8]) {
        let alloc = AllocId(self.next_alloc);
        self.next_alloc += 1;
        let bytes: Vec<u8> = bytes.iter().copied().chain(std::iter::once(0)).collect();
        self.trace.push(Effect::Alloc {
            alloc,
            size: bytes.len() as u64,
        });
        for (index, byte) in bytes.iter().enumerate() {
            let loc = Location {
                alloc,
                byte_offset: index as u64,
            };
            let value = Value::Int {
                width: IntWidth::W8,
                signed: true,
                value: *byte as i128,
            };
            self.heap.insert(loc, value);
            self.trace.push(Effect::Write { loc, value });
        }
        self.vecs.insert(
            name.to_string(),
            VecBinding {
                alloc,
                elem_width: IntWidth::W8,
                elem_signed: true,
                elem_size: 1,
                len: bytes.len() as u64,
                owned: false,
            },
        );
        self.scalars.insert(
            name.to_string(),
            Value::Ref(Location {
                alloc,
                byte_offset: 0,
            }),
        );
    }

    fn push(&mut self, recv: &Expr, args: &[Expr]) {
        let name = match recv {
            Expr::Var(ident) => ident.as_str(),
            other => panic!("effects::rust_ast: unsupported push receiver `{other:?}`"),
        };
        let raw = value_as_i128(self.eval(&args[0]));
        let binding = self
            .vecs
            .get_mut(name)
            .unwrap_or_else(|| panic!("effects::rust_ast: push on unknown Vec `{name}`"));
        let value = Value::Int {
            width: binding.elem_width,
            signed: binding.elem_signed,
            value: raw,
        };
        let loc = Location {
            alloc: binding.alloc,
            byte_offset: binding.len * binding.elem_size,
        };
        binding.len += 1;
        self.heap.insert(loc, value);
        self.trace.push(Effect::Write { loc, value });
    }

    fn drop_live_vecs(&mut self) {
        let mut allocs: Vec<AllocId> = self
            .vecs
            .values()
            .filter(|binding| binding.owned)
            .map(|binding| binding.alloc)
            .collect();
        allocs.sort_by_key(|alloc| Reverse(alloc.0));
        for alloc in allocs {
            if self.freed.insert(alloc) {
                self.trace.push(Effect::Dealloc { alloc });
            }
        }
    }

    fn drop_var(&mut self, expr: &Expr) {
        let name = match expr {
            Expr::Var(ident) => ident.as_str(),
            other => panic!("effects::rust_ast: unsupported drop target `{other:?}`"),
        };
        if let Some(file) = self.files.remove(name) {
            self.trace.push(Effect::FileClose { file });
            self.scalars.remove(name);
            return;
        }
        let binding = self
            .vecs
            .get(name)
            .unwrap_or_else(|| panic!("effects::rust_ast: drop of unknown Vec `{name}`"));
        let alloc = binding.alloc;
        if !self.freed.insert(alloc) {
            panic!("effects::rust_ast: double free of {alloc:?}");
        }
        self.trace.push(Effect::Dealloc { alloc });
    }

    fn print(&mut self, args: &[Expr]) {
        let args = args.iter().skip(1).map(|expr| self.eval(expr)).collect();
        self.trace.push(Effect::Call {
            name: "printf".to_string(),
            args,
        });
    }

    fn open_file(&mut self, expr: &Expr) -> Option<FileId> {
        let OpenEffect { path, mode } = open_effect(expr)?;
        let file = FileId(self.next_file);
        self.next_file += 1;
        self.trace.push(Effect::FileOpen { file, path, mode });
        Some(file)
    }

    fn write_all_call(&mut self, expr: &Expr) -> bool {
        let Expr::Call { func, args } = expr else {
            return false;
        };
        if !is_path(func, &["std", "io", "Write", "write_all"]) {
            return false;
        }
        let [handle, bytes] = args.as_slice() else {
            panic!("effects::rust_ast: write_all expects handle and bytes");
        };
        let file = self.file_arg(handle);
        let bytes = match bytes {
            Expr::ByteStr(bytes) => bytes.clone(),
            other => panic!("effects::rust_ast: unsupported write_all bytes `{other:?}`"),
        };
        self.trace.push(Effect::FileWrite { file, bytes });
        true
    }

    fn file_arg(&self, expr: &Expr) -> FileId {
        match expr {
            Expr::Ref { expr, .. } => self.file_arg(expr),
            Expr::Var(ident) => *self
                .files
                .get(ident.as_str())
                .unwrap_or_else(|| panic!("effects::rust_ast: unknown file `{}`", ident.as_str())),
            other => panic!("effects::rust_ast: unsupported file argument `{other:?}`"),
        }
    }

    fn assign_index(&mut self, base: &Expr, index: &Expr, value: &Expr) {
        let name = match base {
            Expr::Var(ident) => ident.as_str(),
            other => panic!("effects::rust_ast: unsupported assign target base `{other:?}`"),
        };
        let idx = value_as_u64(self.eval(index));
        let raw = value_as_i128(self.eval(value));
        let binding = self
            .vecs
            .get_mut(name)
            .unwrap_or_else(|| panic!("effects::rust_ast: assign into unknown Vec `{name}`"));
        if self.freed.contains(&binding.alloc) {
            panic!("effects::rust_ast: write to {name} after free");
        }
        let value = Value::Int {
            width: binding.elem_width,
            signed: binding.elem_signed,
            value: raw,
        };
        let loc = Location {
            alloc: binding.alloc,
            byte_offset: idx * binding.elem_size,
        };
        binding.len = binding.len.max(idx + 1);
        self.heap.insert(loc, value);
        self.trace.push(Effect::Write { loc, value });
    }

    fn let_struct(&mut self, name: &str, fields: &[(String, Expr)]) {
        let values: Vec<(String, Value)> = fields
            .iter()
            .map(|(field, expr)| (field.clone(), self.eval(expr)))
            .collect();
        let alloc = AllocId(self.next_alloc);
        self.next_alloc += 1;
        let size: u64 = values.iter().map(|(_, v)| int_byte_size(v)).sum();
        self.trace.push(Effect::Alloc { alloc, size });
        let mut field_offsets = HashMap::new();
        let mut offset = 0u64;
        for (field, value) in values {
            let loc = Location {
                alloc,
                byte_offset: offset,
            };
            field_offsets.insert(field, offset);
            self.heap.insert(loc, value);
            self.trace.push(Effect::Write { loc, value });
            offset += int_byte_size(&value);
        }
        self.structs.insert(
            name.to_string(),
            StructBinding {
                alloc,
                field_offsets,
            },
        );
    }

    fn assign_field(&mut self, base: &Expr, field: &str, value: &Expr) {
        let name = match base {
            Expr::Var(ident) => ident.as_str(),
            other => panic!("effects::rust_ast: unsupported field-assign base `{other:?}`"),
        };
        let value = self.eval(value);
        let binding = self.structs.get(name).unwrap_or_else(|| {
            panic!("effects::rust_ast: field-assign on unknown struct `{name}`")
        });
        let offset = *binding.field_offsets.get(field).unwrap_or_else(|| {
            panic!("effects::rust_ast: unknown field `{field}` on struct `{name}`")
        });
        let loc = Location {
            alloc: binding.alloc,
            byte_offset: offset,
        };
        self.heap.insert(loc, value);
        self.trace.push(Effect::Write { loc, value });
    }

    fn let_string(&mut self, name: &str, args: &[Expr]) {
        let s = match &args[0] {
            Expr::Str(s) => s.clone(),
            other => panic!(
                "effects::rust_ast: `String::from` expects a string literal, found {other:?}"
            ),
        };
        let alloc = AllocId(self.next_alloc);
        self.next_alloc += 1;
        let bytes: Vec<u8> = s.bytes().chain(std::iter::once(0)).collect();
        self.trace.push(Effect::Alloc {
            alloc,
            size: bytes.len() as u64,
        });
        for (index, byte) in bytes.iter().enumerate() {
            let loc = Location {
                alloc,
                byte_offset: index as u64,
            };
            let value = Value::Int {
                width: IntWidth::W8,
                signed: true,
                value: *byte as i128,
            };
            self.heap.insert(loc, value);
            self.trace.push(Effect::Write { loc, value });
        }
        self.scalars.insert(
            name.to_string(),
            Value::Ref(Location {
                alloc,
                byte_offset: 0,
            }),
        );
    }

    fn string_len(&mut self, recv: &Expr) -> Value {
        let name = match recv {
            Expr::Var(ident) => ident.as_str(),
            other => panic!("effects::rust_ast: unsupported `.len()` receiver `{other:?}`"),
        };
        let base = match self.scalars.get(name) {
            Some(Value::Ref(loc)) => *loc,
            other => {
                panic!("effects::rust_ast: `.len()` on non-string scalar `{name}` ({other:?})")
            }
        };
        let mut len = 0u64;
        loop {
            let loc = Location {
                alloc: base.alloc,
                byte_offset: base.byte_offset + len,
            };
            match self.heap.get(&loc) {
                Some(Value::Int { value: 0, .. }) => break,
                Some(_) => len += 1,
                None => panic!("effects::rust_ast: read from never-written {loc:?}"),
            }
        }
        self.trace.push(Effect::Call {
            name: "strlen".to_string(),
            args: vec![],
        });
        Value::Int {
            width: IntWidth::W64,
            signed: false,
            value: len as i128,
        }
    }

    fn eval_field(&mut self, base: &Expr, field: &str) -> Value {
        let name = match base {
            Expr::Var(ident) => ident.as_str(),
            other => panic!("effects::rust_ast: unsupported field-read base `{other:?}`"),
        };
        let binding = self
            .structs
            .get(name)
            .unwrap_or_else(|| panic!("effects::rust_ast: field-read on unknown struct `{name}`"));
        let offset = *binding.field_offsets.get(field).unwrap_or_else(|| {
            panic!("effects::rust_ast: unknown field `{field}` on struct `{name}`")
        });
        let loc = Location {
            alloc: binding.alloc,
            byte_offset: offset,
        };
        let value = *self
            .heap
            .get(&loc)
            .unwrap_or_else(|| panic!("effects::rust_ast: read from never-written {loc:?}"));
        self.trace.push(Effect::Read { loc, value });
        value
    }

    fn eval(&mut self, expr: &Expr) -> Value {
        match expr {
            Expr::Value(rv) => rust_value_to_value(rv),
            Expr::Var(ident) if self.globals.contains_key(ident.as_str()) => {
                self.read_global(ident.as_str())
            }
            Expr::Var(ident) => *self.scalars.get(ident.as_str()).unwrap_or_else(|| {
                panic!(
                    "effects::rust_ast: read of unknown scalar `{}`",
                    ident.as_str()
                )
            }),
            Expr::Cast { expr, ty } => cast_value_to_type(self.eval(expr), ty),
            Expr::AddrOf { expr, .. } => Value::Ref(self.addr_of(expr)),
            Expr::Unary { op, expr } => {
                let value = self.eval(expr);
                match (op, value) {
                    (
                        UnaryOp::Neg,
                        Value::Int {
                            width,
                            signed,
                            value,
                        },
                    ) => Value::Int {
                        width,
                        signed,
                        value: value.wrapping_neg(),
                    },
                    (UnaryOp::Not, Value::Bool(b)) => Value::Bool(!b),
                    (
                        UnaryOp::Not,
                        Value::Int {
                            width,
                            signed,
                            value,
                        },
                    ) => Value::Int {
                        width,
                        signed,
                        value: !value,
                    },
                    (UnaryOp::Deref, value @ Value::Int { .. }) => value,
                    (UnaryOp::Deref, Value::Ref(loc)) => self.read_loc(loc),
                    (op, other) => panic!("effects::rust_ast: cannot apply {op:?} to {other:?}"),
                }
            }
            Expr::Binary { op, lhs, rhs } => self.eval_binary(*op, lhs, rhs),
            Expr::Index { base, index } => {
                let name = match base.as_ref() {
                    Expr::Var(ident) => ident.as_str(),
                    other => panic!("effects::rust_ast: unsupported index base `{other:?}`"),
                };
                let idx = value_as_u64(self.eval(index));
                let binding = self.vecs.get(name).unwrap_or_else(|| {
                    panic!("effects::rust_ast: index into unknown Vec `{name}`")
                });
                if self.freed.contains(&binding.alloc) {
                    panic!("effects::rust_ast: read from {name} after free");
                }
                let loc = Location {
                    alloc: binding.alloc,
                    byte_offset: idx * binding.elem_size,
                };
                let value = *self.heap.get(&loc).unwrap_or_else(|| {
                    panic!("effects::rust_ast: read from never-written {loc:?}")
                });
                self.trace.push(Effect::Read { loc, value });
                value
            }
            Expr::Field { base, field } => self.eval_field(base, field),
            Expr::MethodCall { recv, method, args } if method == "len" && args.is_empty() => {
                self.string_len(recv)
            }
            Expr::MethodCall { recv, method, args } if method == "position" => {
                self.iter_position(recv, args)
            }
            Expr::MethodCall { recv, method, args } if method == "sort_by" => {
                self.sort_by(recv, args);
                int32(0)
            }
            Expr::MethodCall { recv, method, args } if method == "binary_search_by" => {
                self.binary_search_by(recv, args)
            }
            Expr::MethodCall { recv, method, args } if method == "map_or" => {
                self.map_or(recv, args)
            }
            Expr::MethodCall { recv, method, args } if method == "cmp" => {
                self.compare_method(recv, args)
            }
            Expr::MethodCall { recv, method, args } if method == "is_none" && args.is_empty() => {
                option_is_none(self.eval(recv))
            }
            Expr::MethodCall { recv, method, args } if method == "is_ok" && args.is_empty() => {
                match self.eval(recv) {
                    Value::AtomicResult { ok, .. } => Value::Bool(ok),
                    other => panic!("effects::rust_ast: `.is_ok()` on unsupported {other:?}"),
                }
            }
            Expr::MethodCall { recv, method, args } if method == "unwrap" && args.is_empty() => {
                option_unwrap(self.eval(recv))
            }
            Expr::MethodCall { recv, method, args } if method == "get_or_init" => {
                self.once_lock_get_or_init(recv, args)
            }
            Expr::MethodCall { recv, method, args }
                if matches!(method.as_str(), "sum" | "product" | "fold") =>
            {
                self.eval_iter_reduce(recv, method, args)
            }
            Expr::MethodCall { recv, method, args } => self.eval_atomic_method(recv, method, args),
            Expr::Match { expr, arms } => self.eval_match(expr, arms),
            Expr::Call { func, args } => self.eval_call(func, args),
            Expr::Block(block) => match self.run_block(block) {
                Flow::Return(value) => value,
                Flow::Normal => panic!("effects::rust_ast: block expression has no tail value"),
                Flow::Break | Flow::Continue => {
                    panic!("effects::rust_ast: loop control escaped block expression")
                }
            },
            Expr::Unsafe(block) => match self.run_block(block) {
                Flow::Return(value) => value,
                Flow::Normal => panic!("effects::rust_ast: unsafe expression has no tail value"),
                Flow::Break | Flow::Continue => {
                    panic!("effects::rust_ast: loop control escaped unsafe expression")
                }
            },
            Expr::AtomicRef { place, .. } => Value::Atomic(self.atomic_place(place)),
            Expr::AtomicLoad {
                place, ordering, ..
            } => self.atomic_load(place, *ordering),
            Expr::AtomicStore {
                place,
                value,
                ordering,
                ..
            } => {
                let value = self.eval(value);
                self.atomic_store(place, *ordering, value);
                value
            }
            Expr::AtomicFetch {
                op,
                place,
                value,
                ordering,
                ..
            } => {
                let value = self.eval(value);
                self.atomic_rmw(*op, place, *ordering, value)
            }
            Expr::AtomicSwap {
                place,
                value,
                ordering,
                ..
            } => {
                let value = self.eval(value);
                self.atomic_swap(place, *ordering, value)
            }
            Expr::AtomicCompareExchange {
                place,
                expected,
                desired,
                success,
                failure,
                ..
            } => {
                let expected = self.eval(expected);
                let desired = self.eval(desired);
                self.atomic_compare_exchange(place, *success, *failure, expected, desired)
            }
            Expr::AtomicNew { value, .. } => {
                let value = self.eval(value);
                let atomic = self.allocate_atomic(value);
                Value::Atomic(atomic)
            }
            Expr::AtomicFence { ordering } => {
                self.trace.push(Effect::AtomicFence {
                    ordering: *ordering,
                });
                int32(0)
            }
            other => panic!("effects::rust_ast: unsupported expr `{other:?}`"),
        }
    }

    fn eval_atomic_method(&mut self, recv: &Expr, method: &str, args: &[Expr]) -> Value {
        match method {
            "load" => {
                let [ordering] = args else {
                    panic!("effects::rust_ast: atomic load expects ordering");
                };
                self.atomic_load(
                    &AtomicPlace::Local(recv_name(recv).into()),
                    ordering_expr(ordering),
                )
            }
            "store" => {
                let [value, ordering] = args else {
                    panic!("effects::rust_ast: atomic store expects value and ordering");
                };
                let value = self.eval(value);
                self.atomic_store(
                    &AtomicPlace::Local(recv_name(recv).into()),
                    ordering_expr(ordering),
                    value,
                );
                value
            }
            "fetch_add" | "fetch_sub" | "fetch_and" | "fetch_xor" | "fetch_or" | "fetch_nand"
            | "fetch_max" | "fetch_min" => {
                let [value, ordering] = args else {
                    panic!("effects::rust_ast: atomic fetch expects value and ordering");
                };
                let op = match method {
                    "fetch_add" => AtomicRmwOp::Add,
                    "fetch_sub" => AtomicRmwOp::Sub,
                    "fetch_and" => AtomicRmwOp::And,
                    "fetch_xor" => AtomicRmwOp::Xor,
                    "fetch_or" => AtomicRmwOp::Or,
                    "fetch_nand" => AtomicRmwOp::Nand,
                    "fetch_max" => AtomicRmwOp::Max,
                    "fetch_min" => AtomicRmwOp::Min,
                    _ => unreachable!(),
                };
                let value = self.eval(value);
                self.atomic_rmw(
                    op,
                    &AtomicPlace::Local(recv_name(recv).into()),
                    ordering_expr(ordering),
                    value,
                )
            }
            "swap" => {
                let [value, ordering] = args else {
                    panic!("effects::rust_ast: atomic swap expects value and ordering");
                };
                let value = self.eval(value);
                self.atomic_swap(
                    &AtomicPlace::Local(recv_name(recv).into()),
                    ordering_expr(ordering),
                    value,
                )
            }
            "compare_exchange" => {
                let [expected, desired, success, failure] = args else {
                    panic!("effects::rust_ast: compare_exchange expects four arguments");
                };
                let expected = self.eval(expected);
                let desired = self.eval(desired);
                self.atomic_compare_exchange(
                    &AtomicPlace::Local(recv_name(recv).into()),
                    ordering_expr(success),
                    ordering_expr(failure),
                    expected,
                    desired,
                )
            }
            other => panic!("effects::rust_ast: unsupported method `{other}`"),
        }
    }

    fn eval_match(&mut self, expr: &Expr, arms: &[crate::rust_ast::ExprMatchArm]) -> Value {
        match self.eval(expr) {
            Value::AtomicResult { ok, value } => {
                let arm_name = if ok { "Ok" } else { "Err" };
                for arm in arms {
                    if let Pattern::TupleStruct { name, fields } = &arm.pattern
                        && name.as_str() == arm_name
                    {
                        if let Some(Pattern::Binding(binding)) = fields.first() {
                            let previous = self
                                .scalars
                                .insert(binding.to_string(), option_value_to_value(value));
                            let result = self.eval(&arm.value);
                            match previous {
                                Some(previous) => {
                                    self.scalars.insert(binding.to_string(), previous);
                                }
                                None => {
                                    self.scalars.remove(binding.as_str());
                                }
                            }
                            return result;
                        }
                        return self.eval(&arm.value);
                    }
                }
                panic!("effects::rust_ast: no match arm for atomic result `{arm_name}`");
            }
            other => panic!("effects::rust_ast: unsupported match scrutinee {other:?}"),
        }
    }

    fn define_atomic(&mut self, name: &str, value: Value) -> AtomicId {
        let atomic = self.allocate_atomic(value);
        self.atomics.insert(name.to_string(), atomic);
        atomic
    }

    fn allocate_atomic(&mut self, value: Value) -> AtomicId {
        let atomic = AtomicId(self.next_atomic);
        self.next_atomic += 1;
        self.atomic_values.insert(atomic, value);
        atomic
    }

    fn atomic_place(&mut self, place: &AtomicPlace) -> AtomicId {
        match place {
            AtomicPlace::Local(name) => self.atomic_for_name(name.as_str()),
            AtomicPlace::Ptr(expr) => self.atomic_for_name(addr_of_local(expr)),
        }
    }

    fn atomic_for_name(&mut self, name: &str) -> AtomicId {
        if let Some(&atomic) = self.atomics.get(name) {
            return atomic;
        }
        let value = *self
            .scalars
            .get(name)
            .unwrap_or_else(|| panic!("effects::rust_ast: atomic access to unknown `{name}`"));
        let atomic = self.allocate_atomic(value);
        self.atomics.insert(name.to_string(), atomic);
        self.scalars.insert(name.to_string(), Value::Atomic(atomic));
        atomic
    }

    fn atomic_load(&mut self, place: &AtomicPlace, ordering: AtomicOrdering) -> Value {
        let atomic = self.atomic_place(place);
        let value = self.atomic_value(atomic);
        self.trace.push(Effect::AtomicLoad {
            atomic,
            ordering,
            value,
        });
        value
    }

    fn atomic_store(&mut self, place: &AtomicPlace, ordering: AtomicOrdering, value: Value) {
        let atomic = self.atomic_place(place);
        self.atomic_values.insert(atomic, value);
        self.trace.push(Effect::AtomicStore {
            atomic,
            ordering,
            value,
        });
    }

    fn atomic_rmw(
        &mut self,
        op: AtomicRmwOp,
        place: &AtomicPlace,
        ordering: AtomicOrdering,
        operand: Value,
    ) -> Value {
        let atomic = self.atomic_place(place);
        let old = self.atomic_value(atomic);
        let new = atomic_rmw_value(op, old, operand);
        self.atomic_values.insert(atomic, new);
        self.trace.push(Effect::AtomicRmw {
            atomic,
            op,
            ordering,
            operand,
            old,
            new,
        });
        old
    }

    fn atomic_swap(&mut self, place: &AtomicPlace, ordering: AtomicOrdering, new: Value) -> Value {
        let atomic = self.atomic_place(place);
        let old = self.atomic_value(atomic);
        self.atomic_values.insert(atomic, new);
        self.trace.push(Effect::AtomicSwap {
            atomic,
            ordering,
            old,
            new,
        });
        old
    }

    fn atomic_compare_exchange(
        &mut self,
        place: &AtomicPlace,
        success: AtomicOrdering,
        failure: AtomicOrdering,
        expected: Value,
        desired: Value,
    ) -> Value {
        let atomic = self.atomic_place(place);
        let old = self.atomic_value(atomic);
        let exchanged = old == expected;
        if exchanged {
            self.atomic_values.insert(atomic, desired);
        }
        self.trace.push(Effect::AtomicCompareExchange {
            atomic,
            success,
            failure,
            expected,
            desired,
            old,
            exchanged,
        });
        Value::AtomicResult {
            ok: exchanged,
            value: option_value(old),
        }
    }

    fn atomic_value(&self, atomic: AtomicId) -> Value {
        *self
            .atomic_values
            .get(&atomic)
            .unwrap_or_else(|| panic!("effects::rust_ast: read from unknown atomic {atomic:?}"))
    }

    fn read_global(&mut self, name: &str) -> Value {
        let loc = *self
            .globals
            .get(name)
            .unwrap_or_else(|| panic!("effects::rust_ast: read of unknown global `{name}`"));
        let value = *self
            .heap
            .get(&loc)
            .unwrap_or_else(|| panic!("effects::rust_ast: read from never-written {loc:?}"));
        self.trace.push(Effect::Read { loc, value });
        value
    }

    fn once_lock_get_or_init(&mut self, recv: &Expr, args: &[Expr]) -> Value {
        let Expr::Var(ident) = recv else {
            panic!("effects::rust_ast: unsupported OnceLock receiver `{recv:?}`");
        };
        let (guard, payload) = self
            .once_locks
            .get(ident.as_str())
            .map(|binding| (binding.guard, binding.payload))
            .unwrap_or_else(|| panic!("effects::rust_ast: unknown OnceLock `{}`", ident.as_str()));
        let initialized = self.read_loc(guard);
        if value_as_i128(initialized) == 0 {
            let [Expr::Closure { params, body }] = args else {
                panic!("effects::rust_ast: OnceLock::get_or_init expects one closure");
            };
            if !params.is_empty() {
                panic!("effects::rust_ast: OnceLock initializer closure cannot take params");
            }
            let value = self.eval(body);
            self.heap.insert(payload, value);
            self.trace.push(Effect::Write {
                loc: payload,
                value,
            });
            let one = Value::Int {
                width: IntWidth::W32,
                signed: true,
                value: 1,
            };
            self.heap.insert(guard, one);
            self.trace.push(Effect::Write {
                loc: guard,
                value: one,
            });
        }
        self.read_loc(payload)
    }

    fn read_loc(&mut self, loc: Location) -> Value {
        let value = *self
            .heap
            .get(&loc)
            .unwrap_or_else(|| panic!("effects::rust_ast: read from never-written {loc:?}"));
        self.trace.push(Effect::Read { loc, value });
        value
    }

    fn addr_of(&mut self, expr: &Expr) -> Location {
        match expr {
            Expr::Index { base, index } => {
                let name = collection_name(base);
                let idx = value_as_u64(self.eval(index));
                let binding = self.vecs.get(name).unwrap_or_else(|| {
                    panic!("effects::rust_ast: address of index into unknown Vec `{name}`")
                });
                Location {
                    alloc: binding.alloc,
                    byte_offset: idx * binding.elem_size,
                }
            }
            other => panic!("effects::rust_ast: unsupported address-of expression `{other:?}`"),
        }
    }

    fn eval_call(&mut self, func: &Expr, args: &[Expr]) -> Value {
        if args.is_empty() && is_path(func, &["std", "ptr", "null_mut"]) {
            return Value::Null;
        }
        if args.is_empty() && is_path(func, &["std", "mem", "size_of"]) {
            return int32(4);
        }
        let Some(name) = path_name(func) else {
            panic!("effects::rust_ast: unsupported call target `{func:?}`");
        };
        if name == "Some" {
            let [arg] = args else {
                panic!("effects::rust_ast: Some expects one argument");
            };
            return Value::Option(Some(option_value(self.eval(arg))));
        }
        if let Some(summary) = call_summary(&name) {
            return self.eval_call_summary(summary, args);
        }
        let f = self
            .funcs
            .get(&name)
            .cloned()
            .unwrap_or_else(|| panic!("effects::rust_ast: unsupported call target `{name}`"));
        let values = args.iter().map(|arg| self.eval(arg)).collect::<Vec<_>>();
        self.call_user(&f, &values)
    }

    fn eval_call_summary(&mut self, summary: CallSummary, args: &[Expr]) -> Value {
        match summary {
            CallSummary::Malloc => self.call_malloc(args),
            CallSummary::Free => self.call_free(args),
            CallSummary::Strlen => self.call_strlen(args),
            CallSummary::Fopen => self.call_fopen(args),
            CallSummary::Fputs => self.call_fputs(args),
            CallSummary::Fclose => self.call_fclose(args),
            CallSummary::Printf => self.call_printf(args),
            CallSummary::Qsort => {
                self.qsort(args);
                int32(0)
            }
            CallSummary::Bsearch => self.bsearch(args),
        }
    }

    fn call_malloc(&mut self, args: &[Expr]) -> Value {
        let [size] = args else {
            panic!("effects::rust_ast: malloc expects size");
        };
        let size = value_as_u64(self.eval(size));
        let alloc = AllocId(self.next_alloc);
        self.next_alloc += 1;
        self.trace.push(Effect::Alloc { alloc, size });
        Value::Ref(Location {
            alloc,
            byte_offset: 0,
        })
    }

    fn call_free(&mut self, args: &[Expr]) -> Value {
        let [base] = args else {
            panic!("effects::rust_ast: free expects pointer");
        };
        let base = match self.eval(base) {
            Value::Ref(loc) => loc,
            other => panic!("effects::rust_ast: free expected pointer, found {other:?}"),
        };
        if !self.freed.insert(base.alloc) {
            panic!("effects::rust_ast: double free of {:?}", base.alloc);
        }
        self.trace.push(Effect::Dealloc { alloc: base.alloc });
        int32(0)
    }

    fn call_strlen(&mut self, args: &[Expr]) -> Value {
        let [base] = args else {
            panic!("effects::rust_ast: strlen expects pointer");
        };
        let base = match self.eval(base) {
            Value::Ref(loc) => loc,
            other => panic!("effects::rust_ast: strlen expected pointer, found {other:?}"),
        };
        let mut len = 0u64;
        loop {
            let loc = Location {
                alloc: base.alloc,
                byte_offset: base.byte_offset + len,
            };
            match self.heap.get(&loc) {
                Some(Value::Int { value: 0, .. }) => break,
                Some(_) => len += 1,
                None => panic!("effects::rust_ast: strlen scanned past never-written {loc:?}"),
            }
        }
        self.trace.push(Effect::Call {
            name: "strlen".to_string(),
            args: vec![],
        });
        Value::Int {
            width: IntWidth::PointerSized,
            signed: false,
            value: len as i128,
        }
    }

    fn call_fopen(&mut self, args: &[Expr]) -> Value {
        let [path, mode] = args else {
            panic!("effects::rust_ast: fopen expects path and mode");
        };
        let file = FileId(self.next_file);
        self.next_file += 1;
        self.trace.push(Effect::FileOpen {
            file,
            path: c_string_expr(path),
            mode: c_string_expr(mode),
        });
        Value::File(file)
    }

    fn call_fputs(&mut self, args: &[Expr]) -> Value {
        let [bytes, file] = args else {
            panic!("effects::rust_ast: fputs expects bytes and file");
        };
        let file = match self.eval(file) {
            Value::File(file) => file,
            other => panic!("effects::rust_ast: fputs expected file handle, found {other:?}"),
        };
        self.trace.push(Effect::FileWrite {
            file,
            bytes: c_string_expr_bytes(bytes),
        });
        int32(0)
    }

    fn call_fclose(&mut self, args: &[Expr]) -> Value {
        let [file] = args else {
            panic!("effects::rust_ast: fclose expects file");
        };
        let file = match self.eval(file) {
            Value::File(file) => file,
            other => panic!("effects::rust_ast: fclose expected file handle, found {other:?}"),
        };
        self.trace.push(Effect::FileClose { file });
        int32(0)
    }

    fn call_printf(&mut self, args: &[Expr]) -> Value {
        let values = args
            .iter()
            .skip(1)
            .map(|arg| self.eval(arg))
            .collect::<Vec<_>>();
        self.trace.push(Effect::Call {
            name: "printf".to_string(),
            args: values,
        });
        int32(0)
    }

    fn bsearch(&mut self, args: &[Expr]) -> Value {
        let [key, base, count, size, comparator] = args else {
            panic!("effects::rust_ast: bsearch expects five arguments");
        };
        let key = match self.eval(key) {
            Value::Ref(loc) => loc,
            other => panic!("effects::rust_ast: bsearch expected key pointer, found {other:?}"),
        };
        let base = match self.eval(base) {
            Value::Ref(loc) => loc,
            other => panic!("effects::rust_ast: bsearch expected base pointer, found {other:?}"),
        };
        let len = value_as_u64(self.eval(count));
        let elem_size = value_as_u64(self.eval(size));
        let comparator = comparator_name(comparator);
        let f = self.funcs.get(comparator).cloned().unwrap_or_else(|| {
            panic!("effects::rust_ast: unknown bsearch comparator `{comparator}`")
        });
        for index in 0..len {
            let elem = Location {
                alloc: base.alloc,
                byte_offset: base.byte_offset + index * elem_size,
            };
            let value = self.call_user(&f, &[Value::Ref(key), Value::Ref(elem)]);
            if value_as_i128(value) == 0 {
                return Value::Ref(elem);
            }
        }
        Value::Null
    }

    fn call_user(&mut self, f: &FnDef, args: &[Value]) -> Value {
        if f.params.len() != args.len() {
            panic!(
                "effects::rust_ast: user function `{}` expected {} arg(s), got {}",
                f.name,
                f.params.len(),
                args.len()
            );
        }
        let saved_scalars = self.scalars.clone();
        for (param, value) in f.params.iter().zip(args) {
            self.scalars.insert(param.name.to_string(), *value);
        }
        self.call_depth += 1;
        let flow = self.run(&f.body);
        self.call_depth -= 1;
        self.scalars = saved_scalars;
        let Flow::Return(value) = flow else {
            panic!(
                "effects::rust_ast: user function `{}` did not return",
                f.name
            );
        };
        value
    }

    fn qsort(&mut self, args: &[Expr]) {
        let [base, count, size, comparator] = args else {
            panic!("effects::rust_ast: qsort expects four arguments");
        };
        let name = array_pointer_name(base);
        let len = value_as_u64(self.eval(count));
        let elem_size = value_as_u64(self.eval(size));
        let comparator = comparator_name(comparator);
        let f = self.funcs.get(comparator).cloned().unwrap_or_else(|| {
            panic!("effects::rust_ast: unknown qsort comparator `{comparator}`")
        });
        self.sort_array_by(name, len, elem_size, |this, left, right| {
            let value = this.call_user(&f, &[Value::Ref(left), Value::Ref(right)]);
            value_as_i128(value)
        });
    }

    fn sort_by(&mut self, recv: &Expr, args: &[Expr]) {
        let [Expr::Closure { params, body }] = args else {
            panic!("effects::rust_ast: sort_by expects one closure");
        };
        let [left_param, right_param] = params.as_slice() else {
            panic!("effects::rust_ast: sort_by closure expects two params");
        };
        let name = collection_name(recv).to_string();
        let binding = self
            .vecs
            .get(&name)
            .unwrap_or_else(|| panic!("effects::rust_ast: sort_by on unknown collection `{name}`"));
        let (len, elem_size) = (binding.len, binding.elem_size);
        let left_param = left_param.as_str().to_string();
        let right_param = right_param.as_str().to_string();
        let body = body.as_ref().clone();
        self.sort_array_by(&name, len, elem_size, |this, left, right| {
            let left_saved = this.scalars.insert(left_param.clone(), Value::Ref(left));
            let right_saved = this.scalars.insert(right_param.clone(), Value::Ref(right));
            let value = this.eval(&body);
            restore_scalar(&mut this.scalars, &left_param, left_saved);
            restore_scalar(&mut this.scalars, &right_param, right_saved);
            value_as_i128(value)
        });
    }

    fn binary_search_by(&mut self, recv: &Expr, args: &[Expr]) -> Value {
        let [Expr::Closure { params, body }] = args else {
            panic!("effects::rust_ast: binary_search_by expects one closure");
        };
        let [param] = params.as_slice() else {
            panic!("effects::rust_ast: binary_search_by closure expects one param");
        };
        let name = collection_name(recv).to_string();
        let binding = self.vecs.get(&name).unwrap_or_else(|| {
            panic!("effects::rust_ast: binary_search_by on unknown collection `{name}`")
        });
        let (alloc, len, elem_size) = (binding.alloc, binding.len, binding.elem_size);
        let param = param.as_str().to_string();
        let body = body.as_ref().clone();
        for index in 0..len {
            let loc = Location {
                alloc,
                byte_offset: index * elem_size,
            };
            let saved = self.scalars.insert(param.clone(), Value::Ref(loc));
            let value = self.eval(&body);
            restore_scalar(&mut self.scalars, &param, saved);
            if value_as_i128(value) == 0 {
                return Value::Option(Some(OptionValue::Int {
                    width: IntWidth::PointerSized,
                    signed: false,
                    value: index as i128,
                }));
            }
        }
        Value::Option(None)
    }

    fn map_or(&mut self, recv: &Expr, args: &[Expr]) -> Value {
        let [default, Expr::Closure { params, body }] = args else {
            panic!("effects::rust_ast: map_or expects default and closure");
        };
        let [param] = params.as_slice() else {
            panic!("effects::rust_ast: map_or closure expects one param");
        };
        match self.eval(recv) {
            Value::Option(Some(value)) => {
                let param = param.as_str().to_string();
                let saved = self
                    .scalars
                    .insert(param.clone(), option_value_to_value(value));
                let result = self.eval(body);
                restore_scalar(&mut self.scalars, &param, saved);
                result
            }
            Value::Option(None) => self.eval(default),
            other => panic!("effects::rust_ast: map_or on non-option `{other:?}`"),
        }
    }

    fn sort_array_by(
        &mut self,
        name: &str,
        len: u64,
        elem_size: u64,
        mut compare: impl FnMut(&mut Self, Location, Location) -> i128,
    ) {
        let alloc = self
            .vecs
            .get(name)
            .unwrap_or_else(|| panic!("effects::rust_ast: sort on unknown collection `{name}`"))
            .alloc;
        for i in 1..len {
            let mut j = i;
            while j > 0 {
                let left = Location {
                    alloc,
                    byte_offset: (j - 1) * elem_size,
                };
                let right = Location {
                    alloc,
                    byte_offset: j * elem_size,
                };
                if compare(self, left, right) <= 0 {
                    break;
                }
                let left_value = *self.heap.get(&left).unwrap_or_else(|| {
                    panic!("effects::rust_ast: read from never-written {left:?}")
                });
                let right_value = *self.heap.get(&right).unwrap_or_else(|| {
                    panic!("effects::rust_ast: read from never-written {right:?}")
                });
                self.heap.insert(left, right_value);
                self.trace.push(Effect::Write {
                    loc: left,
                    value: right_value,
                });
                self.heap.insert(right, left_value);
                self.trace.push(Effect::Write {
                    loc: right,
                    value: left_value,
                });
                j -= 1;
            }
        }
    }

    fn compare_method(&mut self, recv: &Expr, args: &[Expr]) -> Value {
        let [arg] = args else {
            panic!("effects::rust_ast: cmp expects one argument");
        };
        let left = self.read_comparable(recv);
        let right = match arg {
            Expr::Ref { expr, .. } => self.read_comparable(expr),
            other => self.read_comparable(other),
        };
        int32((value_as_i128(left) - value_as_i128(right)).signum())
    }

    fn read_comparable(&mut self, expr: &Expr) -> Value {
        match self.eval(expr) {
            Value::Ref(loc) => self.read_loc(loc),
            value @ Value::Int { .. } => value,
            other => panic!("effects::rust_ast: cmp on unsupported value {other:?}"),
        }
    }

    fn iter_position(&mut self, recv: &Expr, args: &[Expr]) -> Value {
        let [Expr::Closure { params, body }] = args else {
            panic!("effects::rust_ast: `.position()` requires one closure");
        };
        let [param] = params.as_slice() else {
            panic!("effects::rust_ast: `.position()` closure must take one param");
        };
        let (alloc, elem_size, len) = self.iter_source(recv);
        for index in 0..len {
            let loc = Location {
                alloc,
                byte_offset: index * elem_size,
            };
            let value = *self
                .heap
                .get(&loc)
                .unwrap_or_else(|| panic!("effects::rust_ast: read from never-written {loc:?}"));
            self.trace.push(Effect::Read { loc, value });
            self.scalars.insert(param.as_str().to_string(), value);
            if value_as_bool(self.eval(body)) {
                self.scalars.remove(param.as_str());
                return Value::Option(Some(OptionValue::Int {
                    width: IntWidth::PointerSized,
                    signed: false,
                    value: index as i128,
                }));
            }
        }
        self.scalars.remove(param.as_str());
        Value::Option(None)
    }

    fn iter_source(&self, expr: &Expr) -> (AllocId, u64, u64) {
        let Expr::MethodCall { recv, method, args } = expr else {
            panic!("effects::rust_ast: `.position()` receiver must be `.iter()`, found `{expr:?}`");
        };
        if method != "iter" || !args.is_empty() {
            panic!("effects::rust_ast: unsupported iterator source `.{method}()`");
        }
        let name = collection_name(recv);
        let binding = self.vecs.get(name).unwrap_or_else(|| {
            panic!("effects::rust_ast: iterator over unknown collection `{name}`")
        });
        (binding.alloc, binding.elem_size, binding.len)
    }

    fn eval_iter_reduce(&mut self, recv: &Expr, method: &str, args: &[Expr]) -> Value {
        let Expr::MethodCall {
            recv: base,
            method: iter_method,
            args: iter_args,
        } = recv
        else {
            panic!("effects::rust_ast: `.{method}()` receiver must be `.iter()`, found `{recv:?}`");
        };
        if iter_method != "iter" || !iter_args.is_empty() {
            panic!(
                "effects::rust_ast: unsupported iterator adapter `.{iter_method}()` before `.{method}()`"
            );
        }
        let Expr::Var(ident) = &**base else {
            panic!("effects::rust_ast: unsupported `.iter()` receiver `{base:?}`");
        };
        let name = ident.as_str().to_string();
        let binding = self
            .vecs
            .get(&name)
            .unwrap_or_else(|| panic!("effects::rust_ast: `.iter()` over unknown Vec `{name}`"));
        let (alloc, elem_size, elem_width, elem_signed, len) = (
            binding.alloc,
            binding.elem_size,
            binding.elem_width,
            binding.elem_signed,
            binding.len,
        );

        let read_elem = |this: &mut Self, index: u64| -> Value {
            let loc = Location {
                alloc,
                byte_offset: index * elem_size,
            };
            let value = *this
                .heap
                .get(&loc)
                .unwrap_or_else(|| panic!("effects::rust_ast: read from never-written {loc:?}"));
            this.trace.push(Effect::Read { loc, value });
            value
        };

        match method {
            "sum" | "product" => {
                let op = if method == "sum" {
                    BinOp::Add
                } else {
                    BinOp::Mul
                };
                let mut acc = Value::Int {
                    width: elem_width,
                    signed: elem_signed,
                    value: if method == "sum" { 0 } else { 1 },
                };
                for i in 0..len {
                    acc = apply_binop(op, acc, read_elem(self, i));
                }
                acc
            }
            "fold" => {
                let [init, closure] = args else {
                    panic!("effects::rust_ast: `.fold()` requires an init and a closure");
                };
                let Expr::Closure { params, body } = closure else {
                    panic!("effects::rust_ast: `.fold()`'s second argument must be a closure");
                };
                let [acc_param, item_param] = params.as_slice() else {
                    panic!("effects::rust_ast: `.fold()` closure must take exactly two params");
                };
                let (acc_param, item_param) = (
                    acc_param.as_str().to_string(),
                    item_param.as_str().to_string(),
                );
                let mut acc = self.eval(init);
                for i in 0..len {
                    let item = read_elem(self, i);
                    self.scalars.insert(acc_param.clone(), acc);
                    self.scalars.insert(item_param.clone(), item);
                    acc = self.eval(body);
                }
                self.scalars.remove(&acc_param);
                self.scalars.remove(&item_param);
                acc
            }
            other => panic!("effects::rust_ast: unsupported iterator reduction `.{other}()`"),
        }
    }

    fn eval_binary(&mut self, op: BinOp, lhs: &Expr, rhs: &Expr) -> Value {
        match op {
            BinOp::And => {
                if !value_as_bool(self.eval(lhs)) {
                    return Value::Bool(false);
                }
                Value::Bool(value_as_bool(self.eval(rhs)))
            }
            BinOp::Or => {
                if value_as_bool(self.eval(lhs)) {
                    return Value::Bool(true);
                }
                Value::Bool(value_as_bool(self.eval(rhs)))
            }
            _ => {
                let a = self.eval(lhs);
                let b = self.eval(rhs);
                apply_binop(op, a, b)
            }
        }
    }
}

#[cfg(test)]
mod tests;
