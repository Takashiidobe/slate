use std::cmp::Reverse;
use std::collections::{HashMap, HashSet};

use super::{
    AllocId, AtomicId, Effect, EffectTrace, FileId, IntWidth, Location, OptionValue, ParamSeed,
    Value,
};
use crate::rust_ast::{
    AtomicOrdering, AtomicPlace, AtomicRmwOp, BinOp, Block, Expr, FnDef, IndentStmt, Item, Path,
    Pattern, Program, RustValue, Stmt, Type, UnaryOp,
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

struct OpenEffect {
    path: String,
    mode: String,
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
                vec![value; *len]
            }
            other => panic!("effects::rust_ast: unsupported array initializer `{other:?}`"),
        };
        if values.len() as u64 != len {
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

    fn eval_call(&mut self, func: &Expr, args: &[Expr]) -> Value {
        let Expr::Var(ident) = func else {
            panic!("effects::rust_ast: unsupported call target `{func:?}`");
        };
        let name = ident.as_str();
        if name == "Some" {
            let [arg] = args else {
                panic!("effects::rust_ast: Some expects one argument");
            };
            return Value::Option(Some(option_value(self.eval(arg))));
        }
        if name == "fopen" {
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
            return Value::File(file);
        }
        if name == "fputs" {
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
            return int32(0);
        }
        if name == "fclose" {
            let [file] = args else {
                panic!("effects::rust_ast: fclose expects file");
            };
            let file = match self.eval(file) {
                Value::File(file) => file,
                other => panic!("effects::rust_ast: fclose expected file handle, found {other:?}"),
            };
            self.trace.push(Effect::FileClose { file });
            return int32(0);
        }
        if name == "printf" {
            let values = args
                .iter()
                .skip(1)
                .map(|arg| self.eval(arg))
                .collect::<Vec<_>>();
            self.trace.push(Effect::Call {
                name: "printf".to_string(),
                args: values,
            });
            return int32(0);
        }
        let f = self
            .funcs
            .get(name)
            .cloned()
            .unwrap_or_else(|| panic!("effects::rust_ast: unsupported call target `{name}`"));
        if !args.is_empty() || !f.params.is_empty() {
            panic!("effects::rust_ast: user calls with arguments are unsupported");
        }
        self.call_depth += 1;
        let flow = self.run(&f.body);
        self.call_depth -= 1;
        let Flow::Return(value) = flow else {
            panic!("effects::rust_ast: user function `{name}` did not return");
        };
        value
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

fn is_path(expr: &Expr, segments: &[&str]) -> bool {
    match expr {
        Expr::Path(Path { segments: actual }) => {
            actual.len() == segments.len()
                && actual.iter().zip(segments).all(|(a, b)| a.as_str() == *b)
        }
        Expr::Var(ident) => ident.as_str() == segments.join("::"),
        _ => false,
    }
}

fn recv_name(expr: &Expr) -> &str {
    match expr {
        Expr::Var(ident) => ident.as_str(),
        other => panic!("effects::rust_ast: unsupported atomic receiver `{other:?}`"),
    }
}

fn addr_of_local(expr: &Expr) -> &str {
    match expr {
        Expr::AddrOf { expr, .. } => recv_name(expr),
        Expr::Macro { name, args } if name == "std::ptr::addr_of_mut" => {
            let [arg] = args.as_slice() else {
                panic!("effects::rust_ast: addr_of_mut! expects one argument");
            };
            recv_name(arg)
        }
        other => panic!("effects::rust_ast: unsupported atomic pointer place `{other:?}`"),
    }
}

fn ordering_expr(expr: &Expr) -> AtomicOrdering {
    match expr {
        Expr::Path(path) => ordering_from_name(path.segments.last().map(|s| s.as_str())),
        Expr::Var(ident) => ordering_from_name(ident.as_str().rsplit("::").next()),
        other => panic!("effects::rust_ast: unsupported atomic ordering `{other:?}`"),
    }
}

fn ordering_from_name(name: Option<&str>) -> AtomicOrdering {
    match name {
        Some("Relaxed") => AtomicOrdering::Relaxed,
        Some("Acquire") => AtomicOrdering::Acquire,
        Some("Release") => AtomicOrdering::Release,
        Some("AcqRel") => AtomicOrdering::AcqRel,
        Some("SeqCst") => AtomicOrdering::SeqCst,
        other => panic!("effects::rust_ast: unsupported atomic ordering `{other:?}`"),
    }
}

fn open_effect(expr: &Expr) -> Option<OpenEffect> {
    match expr {
        Expr::MethodCall { recv, method, args }
            if matches!(method.as_str(), "unwrap" | "unwrap_or_else") =>
        {
            open_effect(recv)
        }
        Expr::MethodCall { recv, method, args } if method == "open" => {
            let [Expr::Str(path)] = args.as_slice() else {
                panic!("effects::rust_ast: OpenOptions::open expects a string literal path");
            };
            Some(OpenEffect {
                path: path.clone(),
                mode: open_options_mode(recv),
            })
        }
        _ => None,
    }
}

fn open_options_mode(expr: &Expr) -> String {
    let mut read = false;
    let mut write = false;
    let mut append = false;
    let mut create = false;
    let mut truncate = false;
    let mut current = expr;
    loop {
        match current {
            Expr::Call { func, args }
                if args.is_empty() && is_path(func, &["std", "fs", "OpenOptions", "new"]) =>
            {
                break;
            }
            Expr::MethodCall { recv, method, args } => {
                let [Expr::Value(RustValue::Bool(value))] = args.as_slice() else {
                    panic!("effects::rust_ast: OpenOptions::{method} expects a bool literal");
                };
                match method.as_str() {
                    "read" => read = *value,
                    "write" => write = *value,
                    "append" => append = *value,
                    "create" => create = *value,
                    "truncate" => truncate = *value,
                    other => panic!("effects::rust_ast: unsupported OpenOptions method `{other}`"),
                }
                current = recv;
            }
            other => panic!("effects::rust_ast: unsupported OpenOptions chain `{other:?}`"),
        }
    }
    match (read, write, append, create, truncate) {
        (false, true, false, true, true) => "w",
        (true, false, false, false, false) => "r",
        (false, false, true, true, false) => "a",
        (true, true, false, false, false) => "r+",
        (true, true, false, true, true) => "w+",
        (true, false, true, true, false) => "a+",
        other => panic!("effects::rust_ast: unsupported OpenOptions mode {other:?}"),
    }
    .to_string()
}

fn c_string_expr(expr: &Expr) -> String {
    String::from_utf8_lossy(&c_string_expr_bytes(expr)).into_owned()
}

fn c_string_expr_bytes(expr: &Expr) -> Vec<u8> {
    let bytes = match expr {
        Expr::Cast { expr, .. } => return c_string_expr_bytes(expr),
        Expr::MethodCall { recv, method, args } if method == "as_ptr" && args.is_empty() => {
            c_string_expr_bytes(recv)
        }
        Expr::ByteStr(bytes) => bytes.clone(),
        Expr::CStr(bytes) => {
            let mut bytes = bytes.clone();
            bytes.push(0);
            bytes
        }
        other => panic!("effects::rust_ast: unsupported C string expression `{other:?}`"),
    };
    bytes.into_iter().take_while(|byte| *byte != 0).collect()
}

fn value_as_i32(value: Value) -> i32 {
    match value {
        Value::Int { value, .. } => value as i32,
        other => panic!("effects::rust_ast: expected an integer exit code, found {other:?}"),
    }
}

fn rust_value_to_value(rv: &RustValue) -> Value {
    match rv {
        RustValue::I64(v) => int32(*v as i128),
        RustValue::I128(v) => int32(*v),
        RustValue::Usize(v) => int32(*v as i128),
        RustValue::Bool(b) => Value::Bool(*b),
        RustValue::None => Value::Option(None),
        RustValue::NullPtr => Value::Null,
        other => panic!("effects::rust_ast: unsupported literal `{other:?}`"),
    }
}

// Literals never carry an `IntWidth` tag of their own in the emitted AST — see
// `src/effects/mod.rs` doc comment on why no trace comparison depends on an
// intermediate scalar's declared width, only on the destination it's written
// through.
fn int32(value: i128) -> Value {
    Value::Int {
        width: IntWidth::W32,
        signed: true,
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
        other => panic!("effects::rust_ast: buffer element must be an integer, found {other:?}"),
    }
}

fn option_value(value: Value) -> OptionValue {
    match value {
        Value::Int {
            width,
            signed,
            value,
        } => OptionValue::Int {
            width,
            signed,
            value,
        },
        Value::Bool(value) => OptionValue::Bool(value),
        Value::Ref(loc) => OptionValue::Ref(loc),
        other => panic!("effects::rust_ast: unsupported Some payload `{other:?}`"),
    }
}

fn option_value_to_value(value: OptionValue) -> Value {
    match value {
        OptionValue::Int {
            width,
            signed,
            value,
        } => Value::Int {
            width,
            signed,
            value,
        },
        OptionValue::Bool(value) => Value::Bool(value),
        OptionValue::Ref(loc) => Value::Ref(loc),
    }
}

fn option_unwrap(value: Value) -> Value {
    match value {
        Value::Option(Some(OptionValue::Int {
            width,
            signed,
            value,
        })) => Value::Int {
            width,
            signed,
            value,
        },
        Value::Option(Some(OptionValue::Bool(value))) => Value::Bool(value),
        Value::Option(Some(OptionValue::Ref(loc))) => Value::Ref(loc),
        Value::Option(None) => panic!("effects::rust_ast: unwrap on None"),
        other => panic!("effects::rust_ast: unwrap on non-option `{other:?}`"),
    }
}

fn option_is_none(value: Value) -> Value {
    match value {
        Value::Option(value) => Value::Bool(value.is_none()),
        other => panic!("effects::rust_ast: is_none on non-option `{other:?}`"),
    }
}

fn cast_value_to_type(value: Value, ty: &Type) -> Value {
    let Some((width, signed, _)) = scalar_type_shape(ty) else {
        return value;
    };
    match value {
        Value::Int { value, .. } => Value::Int {
            width,
            signed,
            value,
        },
        Value::Bool(value) => Value::Int {
            width,
            signed,
            value: i128::from(value),
        },
        other => other,
    }
}

fn scalar_type_shape(ty: &Type) -> Option<(IntWidth, bool, u64)> {
    let Type::Prim(prim) = ty else {
        return None;
    };
    use crate::rust_ast::Prim;
    Some(match prim {
        Prim::I8 => (IntWidth::W8, true, 1),
        Prim::U8 => (IntWidth::W8, false, 1),
        Prim::I16 => (IntWidth::W16, true, 2),
        Prim::U16 => (IntWidth::W16, false, 2),
        Prim::I32 => (IntWidth::W32, true, 4),
        Prim::U32 => (IntWidth::W32, false, 4),
        Prim::I64 => (IntWidth::W64, true, 8),
        Prim::U64 => (IntWidth::W64, false, 8),
        Prim::I128 => (IntWidth::W128, true, 16),
        Prim::U128 => (IntWidth::W128, false, 16),
        Prim::Isize => (IntWidth::PointerSized, true, 8),
        Prim::Usize => (IntWidth::PointerSized, false, 8),
        Prim::Bool => (IntWidth::W8, false, 1),
        Prim::F32 | Prim::F64 => return None,
    })
}

fn array_elem_shape(ty: &Type) -> Option<(IntWidth, bool, u64, u64)> {
    let Type::Array { elem, len } = ty else {
        return None;
    };
    let (width, signed, size) = scalar_type_shape(elem)?;
    Some((width, signed, size, *len))
}

fn is_cstr_ref_ty(ty: &Type) -> bool {
    matches!(
        ty,
        Type::Ref {
            inner,
            ..
        } if matches!(inner.as_ref(), Type::Custom(name) if name == "core::ffi::CStr")
    )
}

fn collection_name(expr: &Expr) -> &str {
    match expr {
        Expr::Var(ident) => ident.as_str(),
        Expr::MethodCall { recv, method, args }
            if args.is_empty()
                && matches!(method.as_str(), "as_slice" | "as_bytes" | "to_bytes") =>
        {
            collection_name(recv)
        }
        other => panic!("effects::rust_ast: unsupported collection expression `{other:?}`"),
    }
}

fn is_once_lock_ty(ty: &Type) -> bool {
    matches!(ty, Type::Generic { name, .. } if name == "std::sync::OnceLock")
}

fn value_as_i128(value: Value) -> i128 {
    match value {
        Value::Int { value, .. } => value,
        other => panic!("effects::rust_ast: expected an integer value, found {other:?}"),
    }
}

fn value_as_u64(value: Value) -> u64 {
    value_as_i128(value) as u64
}

fn value_as_bool(value: Value) -> bool {
    match value {
        Value::Bool(b) => b,
        other => panic!("effects::rust_ast: expected a bool value, found {other:?}"),
    }
}

fn apply_binop(op: BinOp, a: Value, b: Value) -> Value {
    match op {
        BinOp::Eq | BinOp::Ne | BinOp::Lt | BinOp::Le | BinOp::Gt | BinOp::Ge => {
            if matches!(
                (a, b),
                (Value::File(_), Value::Null) | (Value::Null, Value::File(_))
            ) {
                return Value::Bool(match op {
                    BinOp::Eq => false,
                    BinOp::Ne => true,
                    _ => panic!("effects::rust_ast: unsupported file/null comparison `{op:?}`"),
                });
            }
            let (a_int, b_int) = (value_as_i128(a), value_as_i128(b));
            Value::Bool(match op {
                BinOp::Eq => a_int == b_int,
                BinOp::Ne => a_int != b_int,
                BinOp::Lt => a_int < b_int,
                BinOp::Le => a_int <= b_int,
                BinOp::Gt => a_int > b_int,
                BinOp::Ge => a_int >= b_int,
                _ => unreachable!(),
            })
        }
        BinOp::And | BinOp::Or => {
            panic!("effects::rust_ast: {op:?} must short-circuit, not reach apply_binop")
        }
        _ => {
            let (width, signed) = match a {
                Value::Int { width, signed, .. } => (width, signed),
                other => panic!("effects::rust_ast: expected int operand, found {other:?}"),
            };
            let (a, b) = (value_as_i128(a), value_as_i128(b));
            let value = match op {
                BinOp::Add => a.wrapping_add(b),
                BinOp::Sub => a.wrapping_sub(b),
                BinOp::Mul => a.wrapping_mul(b),
                BinOp::Div => a.wrapping_div(b),
                BinOp::Rem => a.wrapping_rem(b),
                BinOp::BitAnd => a & b,
                BinOp::BitOr => a | b,
                BinOp::BitXor => a ^ b,
                BinOp::Shl => a.wrapping_shl(b as u32),
                BinOp::Shr => a.wrapping_shr(b as u32),
                _ => unreachable!(),
            };
            Value::Int {
                width,
                signed,
                value,
            }
        }
    }
}

fn atomic_rmw_value(op: AtomicRmwOp, old: Value, operand: Value) -> Value {
    let binop = match op {
        AtomicRmwOp::Add => BinOp::Add,
        AtomicRmwOp::Sub => BinOp::Sub,
        AtomicRmwOp::And => BinOp::BitAnd,
        AtomicRmwOp::Xor => BinOp::BitXor,
        AtomicRmwOp::Or => BinOp::BitOr,
        AtomicRmwOp::Nand => {
            let and = apply_binop(BinOp::BitAnd, old, operand);
            return match and {
                Value::Int {
                    width,
                    signed,
                    value,
                } => Value::Int {
                    width,
                    signed,
                    value: !value,
                },
                other => panic!("effects::rust_ast: atomic nand expected int, found {other:?}"),
            };
        }
        AtomicRmwOp::Max => {
            return int32(value_as_i128(old).max(value_as_i128(operand)));
        }
        AtomicRmwOp::Min => {
            return int32(value_as_i128(old).min(value_as_i128(operand)));
        }
    };
    apply_binop(binop, old, operand)
}

/// `Vec<T>`'s element width/signedness/byte-size, read off the local's
/// declared type — the emitted AST carries no width on integer literals
/// themselves, so the element type is the only source of truth for it.
fn vec_elem_shape(ty: &Type) -> Option<(IntWidth, bool, u64)> {
    let Type::Generic { name, args } = ty else {
        return None;
    };
    if name != "Vec" {
        return None;
    }
    let Type::Prim(prim) = args.first()? else {
        return None;
    };
    use crate::rust_ast::Prim;
    Some(match prim {
        Prim::I8 => (IntWidth::W8, true, 1),
        Prim::U8 => (IntWidth::W8, false, 1),
        Prim::I16 => (IntWidth::W16, true, 2),
        Prim::U16 => (IntWidth::W16, false, 2),
        Prim::I32 => (IntWidth::W32, true, 4),
        Prim::U32 => (IntWidth::W32, false, 4),
        Prim::I64 => (IntWidth::W64, true, 8),
        Prim::U64 => (IntWidth::W64, false, 8),
        Prim::I128 => (IntWidth::W128, true, 16),
        Prim::U128 => (IntWidth::W128, false, 16),
        Prim::Isize => (IntWidth::PointerSized, true, 8),
        Prim::Usize => (IntWidth::PointerSized, false, 8),
        Prim::Bool | Prim::F32 | Prim::F64 => return None,
    })
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::rust_ast::{FnParam, Ident, IndentStmt, Prim, Visibility};

    fn int32(value: i128) -> Value {
        Value::Int {
            width: IntWidth::W32,
            signed: true,
            value,
        }
    }

    fn stmt(s: Stmt) -> IndentStmt {
        IndentStmt { depth: 1, stmt: s }
    }

    /// Mirrors the CIR fixture's fixture-shape (minus the C-only cast/stride
    /// noise): `let mut p: Vec<i32> = Vec::with_capacity(2); p.push(1);
    /// p.push(2); return p[0] + p[1];`
    fn idiomatized_fixture() -> FnDef {
        FnDef {
            attrs: vec![],
            vis: Visibility::Private,
            unsafe_: false,
            abi: None,
            name: "main".to_string(),
            params: vec![],
            ret: Some(Type::Prim(Prim::I32)),
            body: vec![
                stmt(Stmt::Let {
                    name: "p".to_string(),
                    mutable: true,
                    ty: Some(Type::Generic {
                        name: "Vec".to_string(),
                        args: vec![Type::Prim(Prim::I32)],
                    }),
                    init: Some(Expr::Call {
                        func: Box::new(Expr::Path(Path::new([
                            Ident::new("Vec"),
                            Ident::new("with_capacity"),
                        ]))),
                        args: vec![Expr::Value(RustValue::I64(2))],
                    }),
                }),
                stmt(Stmt::Expr(Expr::MethodCall {
                    recv: Box::new(Expr::Var(Ident::new("p"))),
                    method: "push".to_string(),
                    args: vec![Expr::Value(RustValue::I64(1))],
                })),
                stmt(Stmt::Expr(Expr::MethodCall {
                    recv: Box::new(Expr::Var(Ident::new("p"))),
                    method: "push".to_string(),
                    args: vec![Expr::Value(RustValue::I64(2))],
                })),
                stmt(Stmt::Return(Some(Expr::Binary {
                    op: BinOp::Add,
                    lhs: Box::new(Expr::Index {
                        base: Box::new(Expr::Var(Ident::new("p"))),
                        index: Box::new(Expr::Value(RustValue::I64(0))),
                    }),
                    rhs: Box::new(Expr::Index {
                        base: Box::new(Expr::Var(Ident::new("p"))),
                        index: Box::new(Expr::Value(RustValue::I64(1))),
                    }),
                }))),
            ],
        }
    }

    fn read_after_drop_fixture() -> FnDef {
        FnDef {
            attrs: vec![],
            vis: Visibility::Private,
            unsafe_: false,
            abi: None,
            name: "main".to_string(),
            params: vec![],
            ret: Some(Type::Prim(Prim::I32)),
            body: vec![
                stmt(Stmt::Let {
                    name: "p".to_string(),
                    mutable: true,
                    ty: Some(Type::Generic {
                        name: "Vec".to_string(),
                        args: vec![Type::Prim(Prim::I32)],
                    }),
                    init: Some(Expr::Call {
                        func: Box::new(Expr::Path(Path::new([
                            Ident::new("Vec"),
                            Ident::new("with_capacity"),
                        ]))),
                        args: vec![Expr::Value(RustValue::I64(1))],
                    }),
                }),
                stmt(Stmt::Expr(Expr::MethodCall {
                    recv: Box::new(Expr::Var(Ident::new("p"))),
                    method: "push".to_string(),
                    args: vec![Expr::Value(RustValue::I64(1))],
                })),
                stmt(Stmt::Expr(Expr::Call {
                    func: Box::new(Expr::Path(Path::new([Ident::new("drop")]))),
                    args: vec![Expr::Var(Ident::new("p"))],
                })),
                stmt(Stmt::Return(Some(Expr::Index {
                    base: Box::new(Expr::Var(Ident::new("p"))),
                    index: Box::new(Expr::Value(RustValue::I64(0))),
                }))),
            ],
        }
    }

    #[test]
    #[should_panic(expected = "after free")]
    fn reading_after_drop_panics_instead_of_silently_succeeding() {
        interpret(&read_after_drop_fixture());
    }

    #[test]
    #[should_panic(expected = "double free")]
    fn dropping_twice_panics() {
        let mut f = read_after_drop_fixture();
        let drop_stmt = f.body[2].clone();
        f.body.insert(3, drop_stmt);
        interpret(&f);
    }

    #[test]
    fn println_macro_pushes_a_call_effect_with_only_the_substituted_args() {
        let body = vec![
            stmt(Stmt::Let {
                name: "sum".to_string(),
                mutable: false,
                ty: Some(Type::Prim(Prim::I32)),
                init: Some(Expr::Value(RustValue::I64(5))),
            }),
            stmt(Stmt::Expr(Expr::Macro {
                name: "println".to_string(),
                args: vec![Expr::Str("{}".to_string()), Expr::Var(Ident::new("sum"))],
            })),
            stmt(Stmt::Return(Some(Expr::Var(Ident::new("sum"))))),
        ];
        let f = FnDef {
            attrs: vec![],
            vis: Visibility::Private,
            unsafe_: false,
            abi: None,
            name: "main".to_string(),
            params: vec![],
            ret: Some(Type::Prim(Prim::I32)),
            body,
        };
        let trace = interpret(&f);
        assert_eq!(
            trace.effects,
            vec![
                Effect::Call {
                    name: "printf".to_string(),
                    args: vec![int32(5)],
                },
                Effect::Exit(5),
            ]
        );
    }

    #[test]
    fn vec_push_index_produces_expected_effects() {
        let trace = interpret(&idiomatized_fixture());
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
                Effect::Dealloc { alloc },
                Effect::Exit(3),
            ]
        );
    }

    /// The exact same effect sequence the CIR walker produces for
    /// `int *p = malloc(2 * sizeof(int)); p[0] = 1; p[1] = 2; return p[0] +
    /// p[1];` (see `cir::tests::malloc_write_read_produces_expected_effects`)
    /// — proving the shared `Effect` enum represents a raw-pointer walk and
    /// its idiomatized `Vec` counterpart without a special case.
    #[test]
    fn matches_cir_trace_shape() {
        let trace = interpret(&idiomatized_fixture());
        let alloc = AllocId(0);
        let cir_shaped_trace = EffectTrace {
            effects: vec![
                Effect::Alloc { alloc, size: 8 },
                Effect::Write {
                    loc: Location {
                        alloc,
                        byte_offset: 0,
                    },
                    value: int32(1),
                },
                Effect::Write {
                    loc: Location {
                        alloc,
                        byte_offset: 4,
                    },
                    value: int32(2),
                },
                Effect::Read {
                    loc: Location {
                        alloc,
                        byte_offset: 0,
                    },
                    value: int32(1),
                },
                Effect::Read {
                    loc: Location {
                        alloc,
                        byte_offset: 4,
                    },
                    value: int32(2),
                },
                Effect::Dealloc { alloc },
                Effect::Exit(3),
            ],
        };
        assert_eq!(trace, cir_shaped_trace);
    }

    /// The real `HeapOwnershipKind::VecBuffer` fixup shape (see
    /// `src/fixups/rewrite/heap_ownership.rs`) for the same fixture: direct
    /// indexed assignment instead of `.push()`, a scalar `let`, and
    /// `main_zero_exit`'s `std::process::exit(code)` rewrite of a non-zero
    /// `return` instead of a bare `return`.
    fn vec_repeat_fixture() -> FnDef {
        FnDef {
            attrs: vec![],
            vis: Visibility::Private,
            unsafe_: false,
            abi: None,
            name: "main".to_string(),
            params: vec![],
            ret: None,
            body: vec![
                stmt(Stmt::Let {
                    name: "p".to_string(),
                    mutable: true,
                    ty: Some(Type::Generic {
                        name: "Vec".to_string(),
                        args: vec![Type::Prim(Prim::I32)],
                    }),
                    init: Some(Expr::VecRepeat {
                        elem: Box::new(Expr::Value(RustValue::I64(0))),
                        len: Box::new(Expr::Value(RustValue::Usize(2))),
                    }),
                }),
                stmt(Stmt::Assign {
                    target: Expr::Index {
                        base: Box::new(Expr::Var(Ident::new("p"))),
                        index: Box::new(Expr::Value(RustValue::I64(0))),
                    },
                    value: Expr::Value(RustValue::I64(1)),
                }),
                stmt(Stmt::Assign {
                    target: Expr::Index {
                        base: Box::new(Expr::Var(Ident::new("p"))),
                        index: Box::new(Expr::Value(RustValue::I64(1))),
                    },
                    value: Expr::Value(RustValue::I64(2)),
                }),
                stmt(Stmt::Let {
                    name: "sum".to_string(),
                    mutable: true,
                    ty: Some(Type::Prim(Prim::I32)),
                    init: Some(Expr::Binary {
                        op: BinOp::Add,
                        lhs: Box::new(Expr::Index {
                            base: Box::new(Expr::Var(Ident::new("p"))),
                            index: Box::new(Expr::Value(RustValue::I64(0))),
                        }),
                        rhs: Box::new(Expr::Index {
                            base: Box::new(Expr::Var(Ident::new("p"))),
                            index: Box::new(Expr::Value(RustValue::I64(1))),
                        }),
                    }),
                }),
                stmt(Stmt::Expr(Expr::Call {
                    func: Box::new(Expr::Path(Path::new([
                        Ident::new("std"),
                        Ident::new("process"),
                        Ident::new("exit"),
                    ]))),
                    args: vec![Expr::Var(Ident::new("sum"))],
                })),
            ],
        }
    }

    /// Mirrors the idiomatized shape of:
    /// `let mut p: Vec<i32> = Vec::with_capacity(1);
    ///  if 5 > 3 { p[0] = 1; } else { p[0] = 2; }
    ///  return p[0];`
    fn if_else_fixture() -> FnDef {
        FnDef {
            attrs: vec![],
            vis: Visibility::Private,
            unsafe_: false,
            abi: None,
            name: "main".to_string(),
            params: vec![],
            ret: Some(Type::Prim(Prim::I32)),
            body: vec![
                stmt(Stmt::Let {
                    name: "p".to_string(),
                    mutable: true,
                    ty: Some(Type::Generic {
                        name: "Vec".to_string(),
                        args: vec![Type::Prim(Prim::I32)],
                    }),
                    init: Some(Expr::Call {
                        func: Box::new(Expr::Path(Path::new([
                            Ident::new("Vec"),
                            Ident::new("with_capacity"),
                        ]))),
                        args: vec![Expr::Value(RustValue::I64(1))],
                    }),
                }),
                stmt(Stmt::If {
                    cond: Expr::Binary {
                        op: BinOp::Gt,
                        lhs: Box::new(Expr::Value(RustValue::I64(5))),
                        rhs: Box::new(Expr::Value(RustValue::I64(3))),
                    },
                    then_body: vec![stmt(Stmt::Assign {
                        target: Expr::Index {
                            base: Box::new(Expr::Var(Ident::new("p"))),
                            index: Box::new(Expr::Value(RustValue::I64(0))),
                        },
                        value: Expr::Value(RustValue::I64(1)),
                    })],
                    else_body: vec![stmt(Stmt::Assign {
                        target: Expr::Index {
                            base: Box::new(Expr::Var(Ident::new("p"))),
                            index: Box::new(Expr::Value(RustValue::I64(0))),
                        },
                        value: Expr::Value(RustValue::I64(2)),
                    })],
                }),
                stmt(Stmt::Return(Some(Expr::Index {
                    base: Box::new(Expr::Var(Ident::new("p"))),
                    index: Box::new(Expr::Value(RustValue::I64(0))),
                }))),
            ],
        }
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
                Effect::Dealloc { alloc },
                Effect::Exit(1),
            ]
        );
    }

    /// Mirrors the idiomatized shape of:
    /// `let mut p: Vec<i32> = Vec::with_capacity(3);
    ///  for i in 0..3 { p[i] = i + 1; }
    ///  let mut sum: i32 = 0;
    ///  for i in 0..3 { sum += p[i]; }
    ///  return sum;`
    fn for_loop_fill_and_sum_fixture() -> FnDef {
        FnDef {
            attrs: vec![],
            vis: Visibility::Private,
            unsafe_: false,
            abi: None,
            name: "main".to_string(),
            params: vec![],
            ret: Some(Type::Prim(Prim::I32)),
            body: vec![
                stmt(Stmt::Let {
                    name: "p".to_string(),
                    mutable: true,
                    ty: Some(Type::Generic {
                        name: "Vec".to_string(),
                        args: vec![Type::Prim(Prim::I32)],
                    }),
                    init: Some(Expr::Call {
                        func: Box::new(Expr::Path(Path::new([
                            Ident::new("Vec"),
                            Ident::new("with_capacity"),
                        ]))),
                        args: vec![Expr::Value(RustValue::I64(3))],
                    }),
                }),
                stmt(Stmt::For {
                    pat: "i".to_string(),
                    iter: Expr::Range {
                        start: Box::new(Expr::Value(RustValue::I64(0))),
                        end: Box::new(Expr::Value(RustValue::I64(3))),
                    },
                    body: vec![stmt(Stmt::Assign {
                        target: Expr::Index {
                            base: Box::new(Expr::Var(Ident::new("p"))),
                            index: Box::new(Expr::Var(Ident::new("i"))),
                        },
                        value: Expr::Binary {
                            op: BinOp::Add,
                            lhs: Box::new(Expr::Var(Ident::new("i"))),
                            rhs: Box::new(Expr::Value(RustValue::I64(1))),
                        },
                    })],
                }),
                stmt(Stmt::Let {
                    name: "sum".to_string(),
                    mutable: true,
                    ty: Some(Type::Prim(Prim::I32)),
                    init: Some(Expr::Value(RustValue::I64(0))),
                }),
                stmt(Stmt::For {
                    pat: "i".to_string(),
                    iter: Expr::Range {
                        start: Box::new(Expr::Value(RustValue::I64(0))),
                        end: Box::new(Expr::Value(RustValue::I64(3))),
                    },
                    body: vec![stmt(Stmt::CompoundAssign {
                        target: Expr::Var(Ident::new("sum")),
                        op: BinOp::Add,
                        value: Expr::Index {
                            base: Box::new(Expr::Var(Ident::new("p"))),
                            index: Box::new(Expr::Var(Ident::new("i"))),
                        },
                    })],
                }),
                stmt(Stmt::Return(Some(Expr::Var(Ident::new("sum"))))),
            ],
        }
    }

    /// `for_loop_fill_and_sum_fixture` with its accumulator loop folded by
    /// `slice_reduce` into `let sum = p.iter().sum();`.
    fn slice_reduce_sum_fixture() -> FnDef {
        FnDef {
            attrs: vec![],
            vis: Visibility::Private,
            unsafe_: false,
            abi: None,
            name: "main".to_string(),
            params: vec![],
            ret: Some(Type::Prim(Prim::I32)),
            body: vec![
                stmt(Stmt::Let {
                    name: "p".to_string(),
                    mutable: true,
                    ty: Some(Type::Generic {
                        name: "Vec".to_string(),
                        args: vec![Type::Prim(Prim::I32)],
                    }),
                    init: Some(Expr::Call {
                        func: Box::new(Expr::Path(Path::new([
                            Ident::new("Vec"),
                            Ident::new("with_capacity"),
                        ]))),
                        args: vec![Expr::Value(RustValue::I64(3))],
                    }),
                }),
                stmt(Stmt::For {
                    pat: "i".to_string(),
                    iter: Expr::Range {
                        start: Box::new(Expr::Value(RustValue::I64(0))),
                        end: Box::new(Expr::Value(RustValue::I64(3))),
                    },
                    body: vec![stmt(Stmt::Assign {
                        target: Expr::Index {
                            base: Box::new(Expr::Var(Ident::new("p"))),
                            index: Box::new(Expr::Var(Ident::new("i"))),
                        },
                        value: Expr::Binary {
                            op: BinOp::Add,
                            lhs: Box::new(Expr::Var(Ident::new("i"))),
                            rhs: Box::new(Expr::Value(RustValue::I64(1))),
                        },
                    })],
                }),
                stmt(Stmt::Let {
                    name: "sum".to_string(),
                    mutable: false,
                    ty: Some(Type::Prim(Prim::I32)),
                    init: Some(Expr::MethodCall {
                        recv: Box::new(Expr::MethodCall {
                            recv: Box::new(Expr::Var(Ident::new("p"))),
                            method: "iter".to_string(),
                            args: vec![],
                        }),
                        method: "sum".to_string(),
                        args: vec![],
                    }),
                }),
                stmt(Stmt::Return(Some(Expr::Var(Ident::new("sum"))))),
            ],
        }
    }

    /// Mirrors the idiomatized shape of `bump`'s `&mut [i32]`/`i32` params
    /// (see `src/fixups/rewrite/ptr_len.rs`):
    /// `fn bump(items: &mut [i32], len: i32) -> i32 {
    ///    items[0] += 1; items[1] += 1;
    ///    return items[0] + items[1] + len;
    ///  }`
    fn bump_fixture() -> FnDef {
        FnDef {
            attrs: vec![],
            vis: Visibility::Private,
            unsafe_: false,
            abi: None,
            name: "bump".to_string(),
            params: vec![
                FnParam {
                    name: "items".to_string(),
                    mutable: true,
                    ty: Type::Ref {
                        mutable: true,
                        inner: Box::new(Type::Slice(Box::new(Type::Prim(Prim::I32)))),
                    },
                },
                FnParam {
                    name: "len".to_string(),
                    mutable: false,
                    ty: Type::Prim(Prim::I32),
                },
            ],
            ret: Some(Type::Prim(Prim::I32)),
            body: vec![
                stmt(Stmt::CompoundAssign {
                    target: Expr::Index {
                        base: Box::new(Expr::Var(Ident::new("items"))),
                        index: Box::new(Expr::Value(RustValue::I64(0))),
                    },
                    op: BinOp::Add,
                    value: Expr::Value(RustValue::I64(1)),
                }),
                stmt(Stmt::CompoundAssign {
                    target: Expr::Index {
                        base: Box::new(Expr::Var(Ident::new("items"))),
                        index: Box::new(Expr::Value(RustValue::I64(1))),
                    },
                    op: BinOp::Add,
                    value: Expr::Value(RustValue::I64(1)),
                }),
                stmt(Stmt::Return(Some(Expr::Binary {
                    op: BinOp::Add,
                    lhs: Box::new(Expr::Binary {
                        op: BinOp::Add,
                        lhs: Box::new(Expr::Index {
                            base: Box::new(Expr::Var(Ident::new("items"))),
                            index: Box::new(Expr::Value(RustValue::I64(0))),
                        }),
                        rhs: Box::new(Expr::Index {
                            base: Box::new(Expr::Var(Ident::new("items"))),
                            index: Box::new(Expr::Value(RustValue::I64(1))),
                        }),
                    }),
                    rhs: Box::new(Expr::Var(Ident::new("len"))),
                }))),
            ],
        }
    }

    #[test]
    fn function_parameters_seed_the_trace_and_match_cir_trace_shape() {
        let params: Vec<(&str, ParamSeed)> = vec![
            ("items", ParamSeed::Buffer(vec![int32(1), int32(2)])),
            ("len", ParamSeed::Scalar(int32(2))),
        ];
        let trace = interpret_with_params(&bump_fixture(), &params);
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

    /// The idiomatized field-literal shape of `struct_field_fixture` in
    /// `cir::tests`: `let p = Point { x: 3, y: 4 }; return p.x + p.y;`
    fn struct_field_fixture() -> FnDef {
        FnDef {
            attrs: vec![],
            vis: Visibility::Private,
            unsafe_: false,
            abi: None,
            name: "main".to_string(),
            params: vec![],
            ret: Some(Type::Prim(Prim::I32)),
            body: vec![
                stmt(Stmt::Let {
                    name: "p".to_string(),
                    mutable: false,
                    ty: Some(Type::Custom("Point".to_string())),
                    init: Some(Expr::StructLit {
                        name: "Point".to_string(),
                        fields: vec![
                            ("x".to_string(), Expr::Value(RustValue::I64(3))),
                            ("y".to_string(), Expr::Value(RustValue::I64(4))),
                        ],
                    }),
                }),
                stmt(Stmt::Return(Some(Expr::Binary {
                    op: BinOp::Add,
                    lhs: Box::new(Expr::Field {
                        base: Box::new(Expr::Var(Ident::new("p"))),
                        field: "x".to_string(),
                    }),
                    rhs: Box::new(Expr::Field {
                        base: Box::new(Expr::Var(Ident::new("p"))),
                        field: "y".to_string(),
                    }),
                }))),
            ],
        }
    }

    #[test]
    fn struct_field_literal_matches_cir_trace_shape() {
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
                Effect::Dealloc { alloc },
                Effect::Exit(6),
            ]
        );
    }

    #[test]
    fn slice_reduce_folded_sum_matches_the_raw_indexed_loops_trace() {
        let raw = interpret(&for_loop_fill_and_sum_fixture());
        let folded = interpret(&slice_reduce_sum_fixture());
        assert_eq!(folded.effects, raw.effects);
    }

    #[test]
    fn vec_repeat_indexed_assign_matches_cir_trace_shape() {
        let trace = interpret(&vec_repeat_fixture());
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
                Effect::Dealloc { alloc },
                Effect::Exit(3),
            ]
        );
    }

    /// The idiomatized `String`/`.len()` shape of `string_strlen_fixture` in
    /// `cir::tests`: `let s = String::from("abc"); return s.len() as i32;`
    fn string_strlen_fixture() -> FnDef {
        FnDef {
            attrs: vec![],
            vis: Visibility::Private,
            unsafe_: false,
            abi: None,
            name: "main".to_string(),
            params: vec![],
            ret: Some(Type::Prim(Prim::I32)),
            body: vec![
                stmt(Stmt::Let {
                    name: "s".to_string(),
                    mutable: false,
                    ty: Some(Type::Custom("String".to_string())),
                    init: Some(Expr::Call {
                        func: Box::new(Expr::Path(Path::new([
                            Ident::new("String"),
                            Ident::new("from"),
                        ]))),
                        args: vec![Expr::Str("abc".to_string())],
                    }),
                }),
                stmt(Stmt::Return(Some(Expr::Cast {
                    expr: Box::new(Expr::MethodCall {
                        recv: Box::new(Expr::Var(Ident::new("s"))),
                        method: "len".to_string(),
                        args: vec![],
                    }),
                    ty: Type::Prim(Prim::I32),
                }))),
            ],
        }
    }

    #[test]
    fn string_len_scans_a_string_buffer_and_pushes_a_call_effect() {
        let trace = interpret(&string_strlen_fixture());
        let alloc = AllocId(0);
        let byte = |value: i128| Value::Int {
            width: IntWidth::W8,
            signed: true,
            value,
        };
        assert_eq!(
            trace.effects,
            vec![
                Effect::Alloc { alloc, size: 4 },
                Effect::Write {
                    loc: Location {
                        alloc,
                        byte_offset: 0
                    },
                    value: byte(97),
                },
                Effect::Write {
                    loc: Location {
                        alloc,
                        byte_offset: 1
                    },
                    value: byte(98),
                },
                Effect::Write {
                    loc: Location {
                        alloc,
                        byte_offset: 2
                    },
                    value: byte(99),
                },
                Effect::Write {
                    loc: Location {
                        alloc,
                        byte_offset: 3
                    },
                    value: byte(0),
                },
                Effect::Call {
                    name: "strlen".to_string(),
                    args: vec![],
                },
                Effect::Exit(3),
            ]
        );
    }
}
