use std::cmp::Reverse;
use std::collections::{HashMap, HashSet};

use super::support::*;
use crate::effects::{
    AllocId, AtomicId, CallSummary, Effect, EffectTrace, FileId, IntWidth, Location, OptionValue,
    ParamSeed, Value, call_summary,
};
use crate::rust_ast::{
    AtomicOrdering, AtomicPlace, AtomicRmwOp, Attr, BinOp, Block, Expr, FnDef, IndentStmt, Item,
    Label, Path, Pattern, Prim, Program, Repr, Stmt, StructDef, StructFields, Type, UnaryOp,
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
        interp.drop_live_vecs();
        interp.trace.push(Effect::Exit(0));
    }
    interp.trace
}

/// How a statement completed: either it ran through normally, or it hit a
/// `return`/`std::process::exit` that the enclosing `if`/`for` must
/// propagate past without running the rest of their body.
#[derive(Debug, Clone, PartialEq)]
enum Flow {
    Normal,
    Return(Value),
    Break(Option<Label>),
    Continue(Option<Label>),
}

#[derive(Clone)]
struct VecBinding {
    alloc: AllocId,
    elem_width: IntWidth,
    elem_signed: bool,
    elem_size: u64,
    len: u64,
    owned: bool,
}

const LAZY_ARRAY_ALLOC: AllocId = AllocId(u32::MAX);

#[derive(Clone)]
struct StructBinding {
    alloc: AllocId,
    field_offsets: HashMap<String, u64>,
    array_fields: HashMap<String, (u64, u64)>,
    field_types: HashMap<String, Type>,
    size: u64,
}

struct OnceLockBinding {
    guard: Location,
    payload: Location,
}

#[derive(Default)]
struct Interp {
    vecs: HashMap<String, VecBinding>,
    array_elem_types: HashMap<String, Type>,
    structs: HashMap<String, StructBinding>,
    globals: HashMap<String, Location>,
    scalar_locs: HashMap<String, Location>,
    pointer_elem_sizes: HashMap<String, u64>,
    once_locks: HashMap<String, OnceLockBinding>,
    funcs: HashMap<String, FnDef>,
    records: HashMap<String, crate::rust_ast::RecordDef>,
    tuple_structs: HashMap<String, StructDef>,
    scalars: HashMap<String, Value>,
    files: HashMap<String, FileId>,
    file_paths: HashMap<FileId, String>,
    file_contents: HashMap<String, Vec<u8>>,
    file_offsets: HashMap<FileId, usize>,
    atomics: HashMap<String, AtomicId>,
    atomic_values: HashMap<AtomicId, Value>,
    heap: HashMap<Location, Value>,
    hidden_c_strings: HashMap<Vec<u8>, Location>,
    next_alloc: u32,
    next_file: u32,
    next_atomic: u32,
    trace: EffectTrace,
    freed: HashSet<AllocId>,
    call_depth: usize,
    next_struct_temp: u32,
}

impl Interp {
    fn seed_program(&mut self, program: &Program) {
        for item in &program.items {
            match item {
                Item::Static { name, ty, init, .. } => self.seed_static(name, ty, init),
                Item::Fn(f) => {
                    self.funcs.insert(f.name.clone(), f.clone());
                }
                Item::Record(record) => {
                    self.records.insert(record.name.clone(), record.clone());
                }
                Item::Struct(def) => {
                    self.tuple_structs.insert(def.name.clone(), def.clone());
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
            self.heap.insert(guard, zero.clone());
            self.trace.push(Effect::Alloc {
                alloc: guard_alloc,
                size: 4,
            });
            self.trace.push(Effect::Write {
                loc: guard,
                value: zero.clone(),
            });
            let payload_alloc = AllocId(self.next_alloc);
            self.next_alloc += 1;
            let payload = Location {
                alloc: payload_alloc,
                byte_offset: 0,
            };
            self.heap.insert(payload, zero.clone());
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
        if let Type::Array { elem, len } = ty {
            let loc = self.bind_array_storage(name, elem, *len, init, false);
            self.globals.insert(name.to_string(), loc);
            return;
        }
        if matches!(ty, Type::Custom(_)) {
            let loc = self.bind_record_storage(name, ty, init);
            self.globals.insert(name.to_string(), loc);
            return;
        }
        let size = self.type_layout(ty).0;
        let value = cast_value_to_type(self.eval(init), ty);
        let alloc = AllocId(self.next_alloc);
        self.next_alloc += 1;
        let loc = Location {
            alloc,
            byte_offset: 0,
        };
        self.globals.insert(name.to_string(), loc);
        self.heap.insert(loc, value.clone());
        self.trace.push(Effect::Alloc { alloc, size });
        self.trace.push(Effect::Write { loc, value });
    }

    fn seed_params(&mut self, params: &[(&str, ParamSeed)]) {
        for (name, seed) in params {
            match seed {
                ParamSeed::Scalar(v) => {
                    self.scalars.insert(name.to_string(), v.clone());
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
            self.heap.insert(loc, elem.clone());
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
                self.record_decl_type(name, ty);
                match init {
                    Expr::StructLit {
                        name: record_name,
                        fields,
                    } => self.let_struct(name, record_name, fields),
                    Expr::Call { func, args } if is_path(func, &["String", "from"]) => {
                        self.let_string(name, args)
                    }
                    Expr::MethodCall { recv, method, args }
                        if matches!(ty, Type::Custom(s) if s == "String")
                            && method == "to_owned"
                            && args.is_empty() =>
                    {
                        let bytes = self.string_expr_bytes(recv);
                        self.let_string_bytes(name, &bytes);
                    }
                    Expr::Str(s) if is_str_ref_ty(ty) => self.let_string_bytes(name, s.as_bytes()),
                    _ if vec_elem_shape(ty).is_some() => self.let_vec(name, ty, init),
                    _ if matches!(ty, Type::Array { .. }) => self.let_array(name, ty, init),
                    Expr::CStr(bytes) if is_cstr_ref_ty(ty) => self.let_cstr(name, bytes),
                    _ if matches!(ty, Type::Custom(_)) => self.let_struct_value(name, init),
                    _ => {
                        let value = cast_value_to_type(self.eval(init), ty);
                        self.scalars.insert(name.clone(), value);
                    }
                }
                Flow::Normal
            }
            Stmt::LetIf {
                name,
                cond,
                then_body,
                then_value,
                else_body,
                else_value,
                ..
            } => {
                let (body, value) = if value_as_bool(self.eval(cond)) {
                    (then_body, then_value)
                } else {
                    (else_body, else_value)
                };
                match self.run(body) {
                    Flow::Normal => {
                        let value = self.eval(value);
                        self.scalars.insert(name.clone(), value);
                        Flow::Normal
                    }
                    flow => flow,
                }
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
            Stmt::Let {
                name,
                ty: Some(ty),
                init: None,
                ..
            } => {
                self.record_decl_type(name, ty);
                Flow::Normal
            }
            Stmt::Let { init: None, .. } => Flow::Normal,
            Stmt::Expr(Expr::MethodCall { recv, method, args }) if method == "push_str" => {
                self.push_str(recv, args);
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
            Stmt::Expr(Expr::Call { func, args })
                if is_path(func, &["std", "ptr", "write_volatile"]) =>
            {
                self.write_volatile(args);
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
            Stmt::Loop { label, body } => self.run_loop(label.as_ref(), body),
            Stmt::Break(label) => Flow::Break(label.clone()),
            Stmt::Continue(label) => Flow::Continue(label.clone()),
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
            Stmt::LabeledBlock { label, body } => match self.run(body) {
                Flow::Break(Some(target)) if target == *label => Flow::Normal,
                flow => flow,
            },
            Stmt::Match { expr, arms } => self.run_match(expr, arms),
            Stmt::While { cond, body } => self.run_while(cond, body),
            Stmt::Block(block) => self.run_block(block),
            Stmt::Return(value) => {
                let value = value
                    .as_ref()
                    .map(|expr| self.eval(expr))
                    .unwrap_or_else(|| Value::Int {
                        width: IntWidth::W32,
                        signed: true,
                        value: 0,
                    });
                if self.call_depth == 0 {
                    let code = value_as_i32(&value);
                    self.drop_live_vecs();
                    self.trace.push(Effect::Exit(code));
                }
                Flow::Return(value)
            }
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

    fn run_loop(&mut self, label: Option<&Label>, body: &[IndentStmt]) -> Flow {
        loop {
            match self.run(body) {
                Flow::Normal => {}
                Flow::Continue(None) => {}
                Flow::Continue(Some(target)) if label == Some(&target) => {}
                Flow::Break(None) => return Flow::Normal,
                Flow::Break(Some(target)) if label == Some(&target) => return Flow::Normal,
                flow @ Flow::Return(_) => return flow,
                flow @ (Flow::Break(_) | Flow::Continue(_)) => return flow,
            }
        }
    }

    fn assign(&mut self, target: &Expr, value: &Expr) -> Flow {
        match target {
            Expr::Var(ident) => {
                if self.scalars.contains_key(ident.as_str()) && self.string_owned_expr(value) {
                    let bytes = self.string_expr_bytes(value);
                    self.replace_string(ident.as_str(), &bytes);
                    return Flow::Normal;
                }
                if self.vecs.contains_key(ident.as_str())
                    && matches!(value, Expr::ArrayLit(_) | Expr::ArrayRepeat { .. })
                {
                    self.assign_array(ident.as_str(), value);
                    return Flow::Normal;
                }
                if self.structs.contains_key(ident.as_str())
                    && !self.pointer_elem_sizes.contains_key(ident.as_str())
                {
                    let v = self.eval(value);
                    let Value::Ref(src) = v else {
                        panic!("effects::rust_ast: assigning non-struct value to struct `{ident}`");
                    };
                    self.copy_struct_to_existing(ident.as_str(), src);
                    return Flow::Normal;
                }
                let v = self.eval(value);
                if let Some(loc) = self.globals.get(ident.as_str()).cloned() {
                    self.write_loc(loc, v);
                } else if let Some(loc) = self.scalar_locs.get(ident.as_str()).cloned() {
                    self.write_loc(loc, v);
                } else {
                    self.scalars.insert(ident.as_str().to_string(), v);
                }
            }
            Expr::Index { base, index } => self.assign_index(base, index, value),
            Expr::Field { base, field } => self.assign_field(base, field, value),
            Expr::Unary {
                op: UnaryOp::Deref,
                expr,
            } => {
                let loc = self.eval_ref(expr);
                let value = self.eval(value);
                self.write_loc(loc, value);
            }
            other => panic!("effects::rust_ast: unsupported assign target `{other:?}`"),
        }
        Flow::Normal
    }

    fn assign_array(&mut self, name: &str, value: &Expr) {
        if let Some(Type::Custom(record_name)) = self.array_elem_types.get(name).cloned() {
            self.write_record_array_values(name, &Type::Custom(record_name), value);
            return;
        }
        self.materialize_collection(name);
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
                if value_as_i128(&value) == 0 {
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
            self.heap.insert(loc, value.clone());
            self.trace.push(Effect::Write { loc, value });
        }
    }

    fn compound_assign(&mut self, target: &Expr, op: BinOp, value: &Expr) -> Flow {
        match target {
            Expr::Var(ident) => {
                let name = ident.as_str().to_string();
                let current = self
                    .scalars
                    .get(&name)
                    .unwrap_or_else(|| {
                        panic!("effects::rust_ast: compound-assign to unknown scalar `{name}`")
                    })
                    .clone();
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
                self.materialize_collection(name);
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
                let current = self
                    .heap
                    .get(&loc)
                    .unwrap_or_else(|| panic!("effects::rust_ast: read from never-written {loc:?}"))
                    .clone();
                self.trace.push(Effect::Read {
                    loc,
                    value: current.clone(),
                });
                let rhs = self.eval(value);
                let updated = apply_binop(op, current, rhs);
                self.heap.insert(loc, updated.clone());
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
        if let Expr::MethodCall { recv, method, args } = iter
            && method == "enumerate"
            && args.is_empty()
        {
            return self.run_for_enumerate(pat, recv, body);
        }
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
                Flow::Normal | Flow::Continue(None) => {}
                Flow::Break(None) => return Flow::Normal,
                flow @ Flow::Return(_) => return flow,
                flow @ (Flow::Break(_) | Flow::Continue(_)) => return flow,
            }
            i += 1;
        }
        Flow::Normal
    }

    fn run_for_enumerate(&mut self, pat: &str, iter: &Expr, body: &[IndentStmt]) -> Flow {
        let (index_param, item_param) = tuple_pat2(pat);
        let (alloc, elem_size, len) = self.iter_source(iter);
        for index in 0..len {
            self.scalars.insert(
                index_param.to_string(),
                Value::Int {
                    width: IntWidth::PointerSized,
                    signed: false,
                    value: index as i128,
                },
            );
            self.scalars.insert(
                item_param.to_string(),
                Value::Ref(Location {
                    alloc,
                    byte_offset: index * elem_size,
                }),
            );
            match self.run(body) {
                Flow::Normal | Flow::Continue(None) => {}
                Flow::Break(None) => return Flow::Normal,
                flow @ Flow::Return(_) => return flow,
                flow @ (Flow::Break(_) | Flow::Continue(_)) => return flow,
            }
        }
        self.scalars.remove(index_param);
        self.scalars.remove(item_param);
        Flow::Normal
    }

    fn run_match(&mut self, expr: &Expr, arms: &[crate::rust_ast::MatchArm]) -> Flow {
        let value = self.eval(expr);
        for arm in arms {
            if pattern_matches(&arm.pattern, value.clone()) {
                return self.run(&arm.body);
            }
        }
        panic!("effects::rust_ast: statement match had no matching arm for {value:?}");
    }

    fn run_while(&mut self, cond: &Expr, body: &Block) -> Flow {
        loop {
            if !value_as_bool(self.eval(cond)) {
                return Flow::Normal;
            }
            match self.run_block(body) {
                Flow::Normal | Flow::Continue(None) => {}
                Flow::Break(None) => return Flow::Normal,
                flow @ Flow::Return(_) => return flow,
                flow @ (Flow::Break(_) | Flow::Continue(_)) => return flow,
            }
        }
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
                len: if matches!(init, Expr::VecRepeat { .. }) {
                    capacity
                } else {
                    0
                },
                owned: true,
            },
        );
        if let Expr::VecRepeat { elem, .. } = init {
            let value = Value::Int {
                width: elem_width,
                signed: elem_signed,
                value: value_as_i128(self.eval(elem)),
            };
            for index in 0..capacity {
                self.heap.insert(
                    Location {
                        alloc,
                        byte_offset: index * elem_size,
                    },
                    value.clone(),
                );
            }
        }
    }

    fn let_array(&mut self, name: &str, ty: &Type, init: &Expr) {
        if let Type::Array { elem, len } = ty
            && matches!(elem.as_ref(), Type::Custom(_))
        {
            self.bind_array_storage(name, elem, *len, init, false);
            return;
        }
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
                if value_as_i128(&value) == 0 {
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
        let alloc = if values.is_empty() {
            LAZY_ARRAY_ALLOC
        } else {
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
                self.heap.insert(loc, value.clone());
                self.trace.push(Effect::Write {
                    loc,
                    value: value.clone(),
                });
            }
            alloc
        };
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

    fn bind_array_storage(
        &mut self,
        name: &str,
        elem_ty: &Type,
        len: u64,
        init: &Expr,
        owned: bool,
    ) -> Location {
        let (elem_size, _) = self.type_layout(elem_ty);
        let alloc = AllocId(self.next_alloc);
        self.next_alloc += 1;
        self.trace.push(Effect::Alloc {
            alloc,
            size: elem_size * len,
        });
        self.vecs.insert(
            name.to_string(),
            VecBinding {
                alloc,
                elem_width: IntWidth::W32,
                elem_signed: true,
                elem_size,
                len,
                owned,
            },
        );
        self.array_elem_types
            .insert(name.to_string(), elem_ty.clone());
        if matches!(elem_ty, Type::Custom(_)) {
            self.bind_record_array_metadata(name, elem_ty, alloc, elem_size, len);
            self.write_record_array_values(name, elem_ty, init);
        } else {
            self.write_scalar_array_values(name, elem_ty, init);
        }
        Location {
            alloc,
            byte_offset: 0,
        }
    }

    fn write_scalar_array_values(&mut self, name: &str, elem_ty: &Type, init: &Expr) {
        let binding = self
            .vecs
            .get(name)
            .unwrap_or_else(|| panic!("effects::rust_ast: write to unknown array `{name}`"))
            .clone();
        let elems = array_init_elems(init, binding.len);
        for (index, elem) in elems.into_iter().enumerate() {
            let value = cast_value_to_type(self.eval(elem), elem_ty);
            let loc = Location {
                alloc: binding.alloc,
                byte_offset: index as u64 * binding.elem_size,
            };
            self.heap.insert(loc, value.clone());
            self.trace.push(Effect::Write { loc, value });
        }
    }

    fn bind_record_array_metadata(
        &mut self,
        name: &str,
        elem_ty: &Type,
        alloc: AllocId,
        elem_size: u64,
        len: u64,
    ) {
        let Type::Custom(record_name) = elem_ty else {
            return;
        };
        let mut field_offsets = HashMap::new();
        let mut array_fields = HashMap::new();
        let mut field_types = HashMap::new();
        for index in 0..len {
            let prefix = index.to_string();
            self.record_field_metadata(
                &prefix,
                record_name,
                index * elem_size,
                &mut field_offsets,
                &mut array_fields,
                &mut field_types,
            );
        }
        self.structs.insert(
            name.to_string(),
            StructBinding {
                alloc,
                field_offsets,
                array_fields,
                field_types,
                size: elem_size * len,
            },
        );
    }

    fn write_record_array_values(&mut self, name: &str, elem_ty: &Type, init: &Expr) {
        let binding = self
            .vecs
            .get(name)
            .unwrap_or_else(|| panic!("effects::rust_ast: write to unknown array `{name}`"))
            .clone();
        let elems = array_init_elems(init, binding.len);
        for (index, elem) in elems.into_iter().enumerate() {
            self.write_record_array_elem(name, elem_ty, index as u64, &elem);
        }
    }

    fn write_record_array_elem(&mut self, name: &str, elem_ty: &Type, index: u64, expr: &Expr) {
        let Type::Custom(record_name) = elem_ty else {
            panic!("effects::rust_ast: expected record array element type, found {elem_ty:?}");
        };
        let binding = self
            .vecs
            .get(name)
            .unwrap_or_else(|| panic!("effects::rust_ast: write to unknown array `{name}`"))
            .clone();
        let base_offset = index * binding.elem_size;
        let mut field_offsets = HashMap::new();
        let mut array_fields = HashMap::new();
        let mut field_types = HashMap::new();
        match expr {
            Expr::StructLit { fields, .. } => {
                self.write_struct_fields(
                    binding.alloc,
                    base_offset,
                    &index.to_string(),
                    Some(record_name),
                    fields,
                    &mut field_offsets,
                    &mut array_fields,
                    &mut field_types,
                );
            }
            Expr::TupleStructLit { fields, .. } if fields.len() == 1 => {
                self.write_record_array_elem(name, elem_ty, index, &fields[0]);
            }
            other => {
                let src = self.eval(other);
                let Value::Ref(src) = src else {
                    panic!(
                        "effects::rust_ast: record array element must be aggregate, found {src:?}"
                    );
                };
                self.copy_struct_bytes(src, binding.alloc, base_offset);
            }
        }
    }

    fn let_cstr(&mut self, name: &str, bytes: &[u8]) {
        let alloc = AllocId(self.next_alloc);
        self.next_alloc += 1;
        let bytes: Vec<u8> = bytes.iter().cloned().chain(std::iter::once(0)).collect();
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
            self.heap.insert(loc, value.clone());
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
        self.heap.insert(loc, value.clone());
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
        let [fmt_expr, rest @ ..] = args else {
            panic!("effects::rust_ast: println!/print! expects a format string");
        };
        let Expr::Str(fmt) = fmt_expr else {
            panic!("effects::rust_ast: unsupported format string `{fmt_expr:?}`");
        };
        let specs: Vec<FormatSpec> = parse_format_string(fmt)
            .into_iter()
            .filter_map(|segment| match segment {
                FormatSegment::Placeholder(spec) => Some(spec),
                FormatSegment::Literal(_) => None,
            })
            .collect();
        let args = rest
            .iter()
            .enumerate()
            .map(|(index, expr)| {
                let value = self.eval(expr);
                match specs.get(index) {
                    Some(spec) if spec.ty == 'p' => value,
                    _ => self.resolve_string_arg(value),
                }
            })
            .collect();
        self.trace.push(Effect::Call {
            name: "printf".to_string(),
            args,
        });
    }

    fn resolve_string_arg(&self, value: Value) -> Value {
        match value {
            Value::Ref(loc) => Value::Bytes(self.read_c_string_silent(loc)),
            other => other,
        }
    }

    fn eval_format_macro(&mut self, args: &[Expr]) -> Value {
        let [fmt_expr, rest @ ..] = args else {
            panic!("effects::rust_ast: format! expects a format string");
        };
        let Expr::Str(fmt) = fmt_expr else {
            panic!("effects::rust_ast: unsupported format! format string `{fmt_expr:?}`");
        };
        let mut out = Vec::new();
        let mut arg_index = 0;
        for segment in parse_format_string(fmt) {
            match segment {
                FormatSegment::Literal(text) => out.extend_from_slice(text.as_bytes()),
                FormatSegment::Placeholder(spec) => {
                    let value = self.eval(&rest[arg_index]);
                    arg_index += 1;
                    out.extend(render_format_arg(&value, &spec));
                }
            }
        }
        Value::Bytes(out)
    }

    fn open_file(&mut self, expr: &Expr) -> Option<FileId> {
        let OpenEffect { path, mode } = open_effect(expr)?;
        let file = FileId(self.next_file);
        self.next_file += 1;
        self.file_paths.insert(file, path.clone());
        self.file_offsets.insert(file, 0);
        if mode.contains('w') {
            self.file_contents.insert(path.clone(), Vec::new());
        }
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
        self.append_file_bytes(file, &bytes);
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
        if matches!(base, Expr::Field { .. }) {
            let loc = self.field_array_element_location(base, index);
            let value = match self.field_array_element_type(base) {
                Some(elem_ty) => cast_value_to_type(self.eval(value), &elem_ty),
                None => {
                    let raw = value_as_i128(self.eval(value));
                    Value::Int {
                        width: IntWidth::W32,
                        signed: true,
                        value: raw,
                    }
                }
            };
            self.write_loc(loc, value);
            return;
        }
        let raw = value_as_i128(self.eval(value));
        let name = match base {
            Expr::Var(ident) => ident.as_str(),
            other => panic!("effects::rust_ast: unsupported assign target base `{other:?}`"),
        };
        let idx = value_as_u64(self.eval(index));
        self.materialize_collection(name);
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
        self.write_loc(loc, value);
    }

    fn let_struct(&mut self, name: &str, record_name: &str, fields: &[(String, Expr)]) {
        self.bind_struct_fields(name, Some(record_name), fields);
    }

    fn bind_struct_fields(
        &mut self,
        name: &str,
        record_name: Option<&str>,
        fields: &[(String, Expr)],
    ) -> Location {
        let alloc = AllocId(self.next_alloc);
        self.next_alloc += 1;
        let mut field_offsets = HashMap::new();
        let mut array_fields = HashMap::new();
        let mut field_types = HashMap::new();
        if let Some(record_name) = record_name
            && self.records.contains_key(record_name)
        {
            self.record_field_metadata(
                "",
                record_name,
                0,
                &mut field_offsets,
                &mut array_fields,
                &mut field_types,
            );
        }
        let alloc_size = record_name
            .filter(|record_name| self.records.contains_key(*record_name))
            .map(|record_name| self.size_of_named_type(record_name))
            .unwrap_or(0);
        let alloc_slot = self.trace.effects.len();
        self.trace.push(Effect::Alloc {
            alloc,
            size: alloc_size,
        });
        let size = self.write_struct_fields(
            alloc,
            0,
            "",
            record_name,
            fields,
            &mut field_offsets,
            &mut array_fields,
            &mut field_types,
        );
        let size = alloc_size.max(size);
        if let Effect::Alloc {
            size: alloc_size, ..
        } = &mut self.trace.effects[alloc_slot]
        {
            *alloc_size = size;
        }
        self.structs.insert(
            name.to_string(),
            StructBinding {
                alloc,
                field_offsets,
                array_fields,
                field_types,
                size,
            },
        );
        Location {
            alloc,
            byte_offset: 0,
        }
    }

    fn eval_struct_lit(&mut self, record_name: &str, fields: &[(String, Expr)]) -> Value {
        let name = format!("__struct_tmp{}", self.next_struct_temp);
        self.next_struct_temp += 1;
        Value::Ref(self.bind_struct_fields(&name, Some(record_name), fields))
    }

    fn eval_tuple_struct_lit(&mut self, _name: &str, fields: &[Expr]) -> Value {
        if fields.len() == 1 {
            return self.eval(&fields[0]);
        }
        Value::Tuple(fields.iter().map(|field| self.eval(field)).collect())
    }

    fn let_struct_value(&mut self, name: &str, init: &Expr) {
        if let Expr::TupleStructLit { fields, .. } = init
            && fields.len() == 1
            && matches!(&fields[0], Expr::StructLit { .. })
        {
            let value = self.eval(&fields[0]);
            let Value::Ref(src) = value else {
                unreachable!();
            };
            self.bind_struct_copy(name, src);
            return;
        }
        let value = self.eval(init);
        let Value::Ref(src) = value else {
            self.scalars.insert(name.to_string(), value);
            return;
        };
        self.bind_struct_copy(name, src);
    }

    fn bind_record_storage(&mut self, name: &str, _ty: &Type, init: &Expr) -> Location {
        match init {
            Expr::StructLit {
                name: record_name,
                fields,
            } => self.bind_struct_fields(name, Some(record_name), fields),
            Expr::TupleStructLit { fields, .. } if fields.len() == 1 => {
                self.bind_record_storage(name, _ty, &fields[0])
            }
            other => {
                let value = self.eval(other);
                let Value::Ref(src) = value else {
                    panic!(
                        "effects::rust_ast: record initializer must be aggregate, found {value:?}"
                    );
                };
                self.bind_struct_copy(name, src);
                Location {
                    alloc: self.structs[name].alloc,
                    byte_offset: 0,
                }
            }
        }
    }

    fn bind_struct_copy(&mut self, name: &str, src: Location) {
        let source = self
            .structs
            .values()
            .find(|binding| binding.alloc == src.alloc)
            .cloned()
            .unwrap_or_else(|| {
                panic!(
                    "effects::rust_ast: copy from unknown struct allocation {:?}",
                    src.alloc
                )
            });
        let alloc = AllocId(self.next_alloc);
        self.next_alloc += 1;
        self.trace.push(Effect::Alloc {
            alloc,
            size: source.size,
        });
        self.copy_struct_bytes(src, alloc, 0);
        self.structs.insert(
            name.to_string(),
            StructBinding {
                alloc,
                field_offsets: source.field_offsets,
                array_fields: source.array_fields,
                field_types: source.field_types,
                size: source.size,
            },
        );
    }

    fn copy_struct_to_existing(&mut self, name: &str, src: Location) {
        let alloc = self
            .structs
            .get(name)
            .unwrap_or_else(|| panic!("effects::rust_ast: copy into unknown struct `{name}`"))
            .alloc;
        self.copy_struct_bytes(src, alloc, 0);
    }

    fn copy_struct_bytes(&mut self, src: Location, dst_alloc: AllocId, dst_base_offset: u64) {
        let source = self
            .structs
            .values()
            .find(|binding| binding.alloc == src.alloc)
            .cloned()
            .unwrap_or_else(|| {
                panic!(
                    "effects::rust_ast: copy from unknown struct allocation {:?}",
                    src.alloc
                )
            });
        let mut offsets: Vec<u64> = self
            .heap
            .keys()
            .filter(|loc| loc.alloc == src.alloc && loc.byte_offset < src.byte_offset + source.size)
            .map(|loc| loc.byte_offset - src.byte_offset)
            .collect();
        offsets.sort_unstable();
        for offset in offsets {
            let src_loc = Location {
                alloc: src.alloc,
                byte_offset: src.byte_offset + offset,
            };
            let value = self.heap[&src_loc].clone();
            let loc = Location {
                alloc: dst_alloc,
                byte_offset: dst_base_offset + offset,
            };
            self.heap.insert(loc, value.clone());
            self.trace.push(Effect::Write { loc, value });
        }
    }

    fn write_struct_fields(
        &mut self,
        alloc: AllocId,
        base_offset: u64,
        prefix: &str,
        record_name: Option<&str>,
        fields: &[(String, Expr)],
        field_offsets: &mut HashMap<String, u64>,
        array_fields: &mut HashMap<String, (u64, u64)>,
        field_types: &mut HashMap<String, Type>,
    ) -> u64 {
        if let Some(record_name) = record_name
            && self
                .records
                .get(record_name)
                .is_some_and(|record| record.is_union)
        {
            let mut size = 0u64;
            for (field, expr) in fields {
                let path = if prefix.is_empty() {
                    field.clone()
                } else {
                    format!("{prefix}.{field}")
                };
                let field_ty = self.record_field_type(record_name, field);
                if let Some(field_ty) = &field_ty {
                    field_types.insert(path.clone(), field_ty.clone());
                }
                field_offsets.insert(path.clone(), base_offset);
                let field_size = self.write_struct_field_value(
                    alloc,
                    base_offset,
                    &path,
                    field_ty.as_ref(),
                    expr,
                    field_offsets,
                    array_fields,
                    field_types,
                );
                size = size.max(field_size);
            }
            return size;
        }
        let mut offset = 0u64;
        for (field, expr) in fields {
            let path = if prefix.is_empty() {
                field.clone()
            } else {
                format!("{prefix}.{field}")
            };
            let field_ty =
                record_name.and_then(|record_name| self.record_field_type(record_name, field));
            if let Some(field_ty) = &field_ty {
                field_types.insert(path.clone(), field_ty.clone());
            }
            field_offsets.insert(path.clone(), base_offset + offset);
            let size = self.write_struct_field_value(
                alloc,
                base_offset + offset,
                &path,
                field_ty.as_ref(),
                expr,
                field_offsets,
                array_fields,
                field_types,
            );
            offset += size;
        }
        offset
    }

    fn write_struct_field_value(
        &mut self,
        alloc: AllocId,
        offset: u64,
        path: &str,
        ty: Option<&Type>,
        expr: &Expr,
        field_offsets: &mut HashMap<String, u64>,
        array_fields: &mut HashMap<String, (u64, u64)>,
        field_types: &mut HashMap<String, Type>,
    ) -> u64 {
        match expr {
            Expr::StructLit { name, fields } => self.write_struct_fields(
                alloc,
                offset,
                path,
                Some(name),
                fields,
                field_offsets,
                array_fields,
                field_types,
            ),
            Expr::ArrayLit(elems) => {
                let elem_ty = match ty {
                    Some(Type::Array { elem, .. }) => Some(elem.as_ref()),
                    _ => None,
                };
                let (_, _, elem_size) =
                    elem_ty
                        .and_then(scalar_type_shape)
                        .unwrap_or((IntWidth::W32, true, 4));
                let mut elem_offset = 0u64;
                for elem in elems {
                    let value = elem_ty
                        .map(|ty| cast_value_to_type(self.eval(elem), ty))
                        .unwrap_or_else(|| self.eval(elem));
                    let loc = Location {
                        alloc,
                        byte_offset: offset + elem_offset,
                    };
                    self.heap.insert(loc, value.clone());
                    self.trace.push(Effect::Write { loc, value });
                    elem_offset += elem_size;
                }
                array_fields.insert(path.to_string(), (offset, elem_size));
                elem_offset
            }
            Expr::ArrayRepeat { elem, len } => {
                let elem_ty = match ty {
                    Some(Type::Array { elem, .. }) => Some(elem.as_ref()),
                    _ => None,
                };
                let value = elem_ty
                    .map(|ty| cast_value_to_type(self.eval(elem), ty))
                    .unwrap_or_else(|| self.eval(elem));
                let (_, _, elem_size) = elem_ty.and_then(scalar_type_shape).unwrap_or_else(|| {
                    let elem_size = int_byte_size(&value);
                    (IntWidth::W32, true, elem_size)
                });
                for index in 0..*len {
                    let loc = Location {
                        alloc,
                        byte_offset: offset + index as u64 * elem_size,
                    };
                    self.heap.insert(loc, value.clone());
                    self.trace.push(Effect::Write {
                        loc,
                        value: value.clone(),
                    });
                }
                array_fields.insert(path.to_string(), (offset, elem_size));
                *len as u64 * elem_size
            }
            _ => {
                let value = ty
                    .map(|ty| cast_value_to_type(self.eval(expr), ty))
                    .unwrap_or_else(|| self.eval(expr));
                let loc = Location {
                    alloc,
                    byte_offset: offset,
                };
                self.heap.insert(loc, value.clone());
                let size = ty
                    .map(|ty| self.type_layout(ty).0)
                    .unwrap_or_else(|| local_value_size(&value));
                self.trace.push(Effect::Write { loc, value });
                size
            }
        }
    }

    fn record_field_metadata(
        &self,
        prefix: &str,
        record_name: &str,
        base_offset: u64,
        field_offsets: &mut HashMap<String, u64>,
        array_fields: &mut HashMap<String, (u64, u64)>,
        field_types: &mut HashMap<String, Type>,
    ) {
        let record = self
            .records
            .get(record_name)
            .unwrap_or_else(|| panic!("effects::rust_ast: unknown record `{record_name}`"));
        let mut offset = 0u64;
        for field in &record.fields {
            let (field_size, field_align) = self.type_layout(&field.ty);
            if !record.is_union && !record.packed {
                offset = align_to(offset, field_align);
            }
            let field_offset = if record.is_union { 0 } else { offset };
            let path = if prefix.is_empty() {
                field.name.as_str().to_string()
            } else {
                format!("{prefix}.{}", field.name.as_str())
            };
            field_offsets.insert(path.clone(), base_offset + field_offset);
            field_types.insert(path.clone(), field.ty.clone());
            if let Type::Array { elem, .. } = &field.ty {
                array_fields.insert(
                    path.clone(),
                    (base_offset + field_offset, self.type_layout(elem).0),
                );
            }
            if let Type::Custom(child_name) = &field.ty
                && self.records.contains_key(child_name)
            {
                self.record_field_metadata(
                    &path,
                    child_name,
                    base_offset + field_offset,
                    field_offsets,
                    array_fields,
                    field_types,
                );
            }
            if !record.is_union {
                offset += field_size;
            }
        }
    }

    fn record_field_type(&self, record_name: &str, field: &str) -> Option<Type> {
        self.records
            .get(record_name)?
            .fields
            .iter()
            .find(|candidate| candidate.name.as_str() == field)
            .map(|field| field.ty.clone())
    }

    fn assign_field(&mut self, base: &Expr, field: &str, value: &Expr) {
        let field_ty = self.field_type(base, field);
        let value = match field_ty.as_ref() {
            Some(ty) => self
                .eval_bitfield_projection(value, ty)
                .unwrap_or_else(|| cast_value_to_type(self.eval(value), ty)),
            None => self.eval(value),
        };
        let loc = self.field_location(base, field);
        self.write_loc(loc, value);
    }

    fn let_string(&mut self, name: &str, args: &[Expr]) {
        let s = match &args[0] {
            Expr::Str(s) => s.clone(),
            other => panic!(
                "effects::rust_ast: `String::from` expects a string literal, found {other:?}"
            ),
        };
        self.let_string_bytes(name, s.as_bytes());
    }

    fn let_string_bytes(&mut self, name: &str, bytes: &[u8]) {
        let loc = self.alloc_string_bytes(bytes);
        self.scalars.insert(name.to_string(), Value::Ref(loc));
    }

    fn alloc_string_bytes(&mut self, bytes: &[u8]) -> Location {
        let alloc = AllocId(self.next_alloc);
        self.next_alloc += 1;
        let bytes: Vec<u8> = bytes.iter().cloned().chain(std::iter::once(0)).collect();
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
            self.heap.insert(loc, value.clone());
            self.trace.push(Effect::Write { loc, value });
        }
        Location {
            alloc,
            byte_offset: 0,
        }
    }

    fn replace_string(&mut self, name: &str, bytes: &[u8]) {
        let loc = self.alloc_string_bytes(bytes);
        self.scalars.insert(name.to_string(), Value::Ref(loc));
    }

    fn string_owned_expr(&self, expr: &Expr) -> bool {
        matches!(expr, Expr::MethodCall { method, args, .. } if method == "to_owned" && args.is_empty())
    }

    fn push_str(&mut self, recv: &Expr, args: &[Expr]) {
        let [arg] = args else {
            panic!("effects::rust_ast: push_str expects one argument");
        };
        let base = match self.eval(recv) {
            Value::Ref(loc) => loc,
            other => panic!("effects::rust_ast: push_str on non-string value {other:?}"),
        };
        let offset = self.c_string_len(base);
        let bytes = self.string_expr_bytes(arg);
        self.write_c_string(
            Location {
                alloc: base.alloc,
                byte_offset: base.byte_offset + offset,
            },
            &bytes,
        );
    }

    fn string_expr_bytes(&mut self, expr: &Expr) -> Vec<u8> {
        match expr {
            Expr::Cast { expr, .. } => self.string_expr_bytes(expr),
            Expr::MethodCall { recv, method, args } if method == "to_owned" && args.is_empty() => {
                self.string_expr_bytes(recv)
            }
            Expr::MethodCall { recv, method, args } if method == "as_ptr" && args.is_empty() => {
                self.string_expr_bytes(recv)
            }
            Expr::Str(s) => s.as_bytes().to_vec(),
            Expr::ByteStr(bytes) | Expr::CStr(bytes) => bytes
                .iter()
                .cloned()
                .take_while(|byte| *byte != 0)
                .collect(),
            _ => {
                let loc = self.eval_ref(expr);
                self.read_c_string(loc)
            }
        }
    }

    fn read_c_string(&mut self, base: Location) -> Vec<u8> {
        let len = self.c_string_len(base);
        self.read_bytes(base, len)
            .into_iter()
            .map(|value| value_as_i128(value) as u8)
            .collect()
    }

    fn c_string_len(&mut self, base: Location) -> u64 {
        let mut len = 0u64;
        loop {
            let loc = Location {
                alloc: base.alloc,
                byte_offset: base.byte_offset + len,
            };
            let value = self.read_loc(loc);
            if value_as_i128(value) as u8 == 0 {
                break len;
            }
            len += 1;
        }
    }

    fn c_string_len_silent(&self, base: Location) -> u64 {
        let mut len = 0u64;
        loop {
            let loc = Location {
                alloc: base.alloc,
                byte_offset: base.byte_offset + len,
            };
            match self.heap.get(&loc) {
                Some(Value::Int { value, .. }) if *value as u8 == 0 => break len,
                Some(_) => len += 1,
                None => panic!("effects::rust_ast: read from never-written {loc:?}"),
            }
        }
    }

    fn read_c_string_silent(&self, base: Location) -> Vec<u8> {
        let len = self.c_string_len_silent(base);
        (0..len)
            .map(|offset| {
                let loc = Location {
                    alloc: base.alloc,
                    byte_offset: base.byte_offset + offset,
                };
                match self.heap.get(&loc) {
                    Some(value) => value_as_i128(value) as u8,
                    None => panic!("effects::rust_ast: read from never-written {loc:?}"),
                }
            })
            .collect()
    }

    fn write_c_string(&mut self, dst: Location, bytes: &[u8]) {
        let values = bytes
            .iter()
            .cloned()
            .chain(std::iter::once(0))
            .map(|byte| Value::Int {
                width: IntWidth::W8,
                signed: true,
                value: byte as i128,
            })
            .collect::<Vec<_>>();
        self.write_bytes(dst, &values);
    }

    fn string_len(&mut self, recv: &Expr) -> Value {
        let base = self.eval_ref(recv);
        let len = self.c_string_len_silent(base);
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
        let loc = self.field_location(base, field);
        let value = self
            .heap
            .get(&loc)
            .unwrap_or_else(|| panic!("effects::rust_ast: read from never-written {loc:?}"))
            .clone();
        self.trace.push(Effect::Read {
            loc,
            value: value.clone(),
        });
        value
    }

    fn field_location(&self, base: &Expr, field: &str) -> Location {
        let (name, prefix) = self.field_path(base);
        let path = if prefix.is_empty() {
            field.to_string()
        } else {
            format!("{prefix}.{field}")
        };
        let binding = self
            .structs
            .get(name)
            .unwrap_or_else(|| panic!("effects::rust_ast: field-read on unknown struct `{name}`"));
        let offset = *binding.field_offsets.get(&path).unwrap_or_else(|| {
            panic!("effects::rust_ast: unknown field `{path}` on struct `{name}`")
        });
        Location {
            alloc: binding.alloc,
            byte_offset: offset,
        }
    }

    fn field_type(&self, base: &Expr, field: &str) -> Option<Type> {
        let (name, prefix) = self.field_path(base);
        let path = if prefix.is_empty() {
            field.to_string()
        } else {
            format!("{prefix}.{field}")
        };
        self.structs.get(name)?.field_types.get(&path).cloned()
    }

    fn eval_bitfield_projection(&mut self, expr: &Expr, ty: &Type) -> Option<Value> {
        let Expr::Binary {
            op: BinOp::Shr,
            lhs,
            rhs,
        } = expr
        else {
            return None;
        };
        let Expr::Binary {
            op: BinOp::Shl,
            lhs: inner,
            rhs: left_shift,
        } = lhs.as_ref()
        else {
            return None;
        };
        let left_shift = value_as_u64(self.eval(left_shift));
        let right_shift = value_as_u64(self.eval(rhs));
        if left_shift != right_shift {
            return None;
        }
        let (width, signed, _) = scalar_type_shape(ty)?;
        let bits = int_width_bits(width)?;
        let bit_width = bits.checked_sub(left_shift as u32)?;
        let value = value_as_i128(cast_value_to_type(self.eval(inner), ty));
        Some(Value::Int {
            width,
            signed,
            value: truncate_to_bits(value, bit_width, signed),
        })
    }

    fn eval_path(&self, path: &Path) -> Value {
        let segments = path
            .segments
            .iter()
            .map(|segment| segment.as_str())
            .collect::<Vec<_>>();
        match segments.as_slice() {
            ["u64", "MAX"] => Value::Int {
                width: IntWidth::W64,
                signed: false,
                value: u64::MAX as i128,
            },
            ["usize", "MAX"] => Value::Int {
                width: IntWidth::PointerSized,
                signed: false,
                value: u64::MAX as i128,
            },
            ["std", "cmp", "Ordering", "Less"] => int32(-1),
            ["std", "cmp", "Ordering", "Equal"] => int32(0),
            ["std", "cmp", "Ordering", "Greater"] => int32(1),
            [_, variant] if variant.starts_with("MODE_") => int32(0),
            _ => panic!("effects::rust_ast: unsupported path `{path:?}`"),
        }
    }

    fn field_array_element_location(&mut self, base: &Expr, index: &Expr) -> Location {
        let (name, path) = self.field_path(base);
        let name = name.to_string();
        let idx = value_as_u64(self.eval(index));
        let binding = self
            .structs
            .get(&name)
            .unwrap_or_else(|| panic!("effects::rust_ast: index field on unknown struct `{name}`"));
        let (offset, elem_size) = *binding.array_fields.get(&path).unwrap_or_else(|| {
            panic!("effects::rust_ast: field `{path}` on struct `{name}` is not an array")
        });
        Location {
            alloc: binding.alloc,
            byte_offset: offset + idx * elem_size,
        }
    }

    fn field_array_element_type(&self, base: &Expr) -> Option<Type> {
        let (name, path) = self.field_path(base);
        let Type::Array { elem, .. } = self.structs.get(name)?.field_types.get(&path)? else {
            return None;
        };
        Some((**elem).clone())
    }

    fn field_path<'a>(&'a self, expr: &'a Expr) -> (&'a str, String) {
        match expr {
            Expr::Var(ident) => {
                if self.structs.contains_key(ident.as_str()) {
                    return (ident.as_str(), String::new());
                }
                if let Some(Value::Ref(loc)) = self.scalars.get(ident.as_str())
                    && self.aggregate_allocs_contains(*loc)
                {
                    return self.struct_path_for_loc(*loc);
                }
                (ident.as_str(), String::new())
            }
            Expr::Unary {
                op: UnaryOp::Deref,
                expr,
            } => {
                let value = self.eval_ref_without_trace(expr);
                let Value::Ref(loc) = value else {
                    panic!(
                        "effects::rust_ast: dereferenced field base is not a pointer: {value:?}"
                    );
                };
                self.struct_path_for_loc(loc)
            }
            Expr::Field { base, field } => {
                let (name, prefix) = self.field_path(base);
                let path = if prefix.is_empty() {
                    field.clone()
                } else {
                    format!("{prefix}.{field}")
                };
                (name, path)
            }
            Expr::Index { base, index } => {
                let name = collection_name(base);
                let index = value_as_u64(self.eval_without_trace(index));
                (name, index.to_string())
            }
            other => panic!("effects::rust_ast: unsupported field base `{other:?}`"),
        }
    }

    fn eval_ref_without_trace(&self, expr: &Expr) -> Value {
        match expr {
            Expr::Var(ident) if self.globals.contains_key(ident.as_str()) => self
                .heap
                .get(&self.globals[ident.as_str()])
                .unwrap_or_else(|| {
                    panic!("effects::rust_ast: read from never-written global `{ident}`")
                })
                .clone(),
            Expr::Var(ident) => self
                .scalars
                .get(ident.as_str())
                .unwrap_or_else(|| panic!("effects::rust_ast: read of unknown scalar `{ident}`"))
                .clone(),
            _ => panic!("effects::rust_ast: unsupported pointer field base `{expr:?}`"),
        }
    }

    fn eval_without_trace(&self, expr: &Expr) -> Value {
        match expr {
            Expr::Value(rv) => rust_value_to_value(rv),
            Expr::Cast { expr, ty } => cast_value_to_type(self.eval_without_trace(expr), ty),
            Expr::Var(ident) if self.scalars.contains_key(ident.as_str()) => {
                self.scalars[ident.as_str()].clone()
            }
            other => panic!("effects::rust_ast: unsupported trace-free eval `{other:?}`"),
        }
    }

    fn struct_path_for_loc(&self, loc: Location) -> (&str, String) {
        let (name, binding) = self
            .structs
            .iter()
            .find(|(_, binding)| binding.alloc == loc.alloc)
            .unwrap_or_else(|| {
                panic!(
                    "effects::rust_ast: dereferenced field base points to unknown aggregate {:?}",
                    loc.alloc
                )
            });
        if self.array_elem_types.contains_key(name.as_str())
            && let Some(vec) = self.vecs.get(name)
        {
            let index = loc.byte_offset / vec.elem_size;
            let offset = loc.byte_offset % vec.elem_size;
            if offset == 0 {
                return (name.as_str(), index.to_string());
            }
            let prefix = index.to_string();
            let path = binding
                .field_offsets
                .iter()
                .find_map(|(path, field_offset)| {
                    (*field_offset == loc.byte_offset
                        && path
                            .strip_prefix(&prefix)
                            .is_some_and(|rest| rest.starts_with('.')))
                    .then(|| path.clone())
                })
                .unwrap_or_else(|| {
                    panic!(
                        "effects::rust_ast: no aggregate array field starts at offset {}",
                        loc.byte_offset
                    )
                });
            return (name.as_str(), path);
        }
        if loc.byte_offset == 0 {
            return (name.as_str(), String::new());
        }
        let path = binding
            .field_offsets
            .iter()
            .find_map(|(path, offset)| (*offset == loc.byte_offset).then(|| path.clone()))
            .unwrap_or_else(|| {
                panic!(
                    "effects::rust_ast: no aggregate field starts at offset {}",
                    loc.byte_offset
                )
            });
        (name.as_str(), path)
    }

    fn aggregate_allocs_contains(&self, loc: Location) -> bool {
        self.structs
            .values()
            .any(|binding| binding.alloc == loc.alloc)
    }

    fn eval(&mut self, expr: &Expr) -> Value {
        match expr {
            Expr::Value(rv) => rust_value_to_value(rv),
            Expr::HexFloat(s) => Value::Float(parse_hex_float(s)),
            Expr::Var(ident) if self.globals.contains_key(ident.as_str()) => {
                self.read_global(ident.as_str())
            }
            Expr::Var(ident) if self.scalar_locs.contains_key(ident.as_str()) => {
                self.read_loc(self.scalar_locs[ident.as_str()])
            }
            Expr::Var(ident) if self.scalars.contains_key(ident.as_str()) => {
                self.scalars[ident.as_str()].clone()
            }
            Expr::Var(ident) if self.structs.contains_key(ident.as_str()) => Value::Ref(Location {
                alloc: self.structs[ident.as_str()].alloc,
                byte_offset: 0,
            }),
            Expr::Var(ident) => self
                .scalars
                .get(ident.as_str())
                .unwrap_or_else(|| {
                    panic!(
                        "effects::rust_ast: read of unknown scalar `{}`",
                        ident.as_str()
                    )
                })
                .clone(),
            Expr::Path(path) => self.eval_path(path),
            Expr::Cast { expr, ty } => cast_value_to_type(self.eval(expr), ty),
            Expr::StructLit { name, fields } => self.eval_struct_lit(name, fields),
            Expr::TupleStructLit { name, fields } => self.eval_tuple_struct_lit(name, fields),
            Expr::Str(s) => Value::Ref(self.hidden_c_string(s.as_bytes())),
            Expr::ByteStr(bytes) | Expr::CStr(bytes) => Value::Ref(self.hidden_c_string(bytes)),
            Expr::ArrayPtr { array, .. } => Value::Ref(self.collection_base(array)),
            Expr::AddrOf { expr, .. } => Value::Ref(self.addr_of(expr)),
            Expr::Macro { name, args }
                if matches!(name.as_str(), "std::ptr::addr_of_mut" | "std::ptr::addr_of") =>
            {
                let [arg] = args.as_slice() else {
                    panic!("effects::rust_ast: {name}! expects one argument");
                };
                Value::Ref(self.addr_of(arg))
            }
            Expr::Macro { name, args } if name == "format" => self.eval_format_macro(args),
            Expr::Macro { name, args } if name == "std::mem::offset_of" => {
                let [Expr::Var(record), Expr::Var(field)] = args.as_slice() else {
                    panic!("effects::rust_ast: offset_of! expects type and field");
                };
                Value::Int {
                    width: IntWidth::PointerSized,
                    signed: false,
                    value: self.field_offset_named_type(record.as_str(), field.as_str()) as i128,
                }
            }
            Expr::Unary { op, expr } => {
                let value = self.eval(expr);
                match (op, value) {
                    (UnaryOp::Neg, Value::Float(value)) => Value::Float(-value),
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
            Expr::TupleField { base, index } => self.eval_tuple_field(base, *index),
            Expr::Index { base, index } => {
                if let Expr::ArrayLit(elems) = base.as_ref() {
                    let idx = value_as_u64(self.eval(index)) as usize;
                    return self.eval(&elems[idx]);
                }
                if matches!(base.as_ref(), Expr::Field { .. }) {
                    let loc = self.field_array_element_location(base, index);
                    let value = self
                        .heap
                        .get(&loc)
                        .unwrap_or_else(|| {
                            panic!("effects::rust_ast: read from never-written {loc:?}")
                        })
                        .clone();
                    self.trace.push(Effect::Read {
                        loc,
                        value: value.clone(),
                    });
                    return value;
                }
                let name = match base.as_ref() {
                    Expr::Var(ident) => ident.as_str(),
                    other => panic!("effects::rust_ast: unsupported index base `{other:?}`"),
                };
                let idx = value_as_u64(self.eval(index));
                self.materialize_collection(name);
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
                let value = self
                    .heap
                    .get(&loc)
                    .unwrap_or_else(|| panic!("effects::rust_ast: read from never-written {loc:?}"))
                    .clone();
                self.trace.push(Effect::Read {
                    loc,
                    value: value.clone(),
                });
                value
            }
            Expr::Field { base, field } if field.bytes().all(|byte| byte.is_ascii_digit()) => {
                let index = field
                    .parse::<usize>()
                    .unwrap_or_else(|_| panic!("effects::rust_ast: invalid tuple field `{field}`"));
                self.eval_tuple_field(base, index)
            }
            Expr::Field { base, field } => self.eval_field(base, field),
            Expr::MethodCall { recv, method, args } if method == "len" && args.is_empty() => {
                self.string_len(recv)
            }
            Expr::MethodCall { recv, method, args } if method == "to_owned" && args.is_empty() => {
                let bytes = self.string_expr_bytes(recv);
                Value::Ref(self.alloc_string_bytes(&bytes))
            }
            Expr::MethodCallGeneric {
                recv,
                method,
                type_args,
                args,
            } if method == "parse" && args.is_empty() => self.parse_string_method(recv, type_args),
            Expr::MethodCall { recv, method, args } if method == "unwrap_or" => {
                self.unwrap_or(recv, args)
            }
            Expr::MethodCall { recv, method, args }
                if method == "as_mut_ptr" && args.is_empty() =>
            {
                Value::Ref(self.collection_base(recv))
            }
            Expr::MethodCall { recv, method, args }
                if matches!(
                    method.as_str(),
                    "as_slice" | "as_mut_slice" | "as_bytes" | "to_bytes"
                ) && args.is_empty() =>
            {
                Value::Ref(self.collection_base(recv))
            }
            Expr::MethodCall { recv, method, args } if method == "as_ptr" && args.is_empty() => {
                match recv.as_ref() {
                    Expr::Var(ident) if self.vecs.contains_key(ident.as_str()) => {
                        Value::Ref(self.collection_base(recv))
                    }
                    _ => self.eval(recv),
                }
            }
            Expr::MethodCall { recv, method, args }
                if matches!(method.as_str(), "add" | "offset") =>
            {
                let [offset] = args.as_slice() else {
                    panic!("effects::rust_ast: pointer {method} expects one argument");
                };
                let base = self.eval_ref(recv);
                let elem_size = self.pointer_elem_size(recv, base);
                Value::Ref(Location {
                    alloc: base.alloc,
                    byte_offset: base.byte_offset
                        + value_as_u64(self.eval(offset)).wrapping_mul(elem_size),
                })
            }
            Expr::MethodCall { recv, method, args } if method == "offset_from" => {
                let [other] = args.as_slice() else {
                    panic!("effects::rust_ast: offset_from expects one argument");
                };
                let lhs = self.eval_ref(recv);
                let rhs = self.eval_ref(other);
                if lhs.alloc != rhs.alloc {
                    panic!("effects::rust_ast: offset_from across different allocations");
                }
                let elem_size = self.pointer_elem_size(recv, lhs);
                Value::Int {
                    width: IntWidth::PointerSized,
                    signed: true,
                    value: (lhs.byte_offset as i128 - rhs.byte_offset as i128) / elem_size as i128,
                }
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
            Expr::MethodCall { recv, method, args } if method == "is_some" && args.is_empty() => {
                option_is_some(self.eval(recv))
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
            Expr::MethodCall { recv, method, args } if self.is_integer_method(method) => {
                self.eval_integer_method(recv, method, args)
            }
            Expr::MethodCall { recv, method, args } => self.eval_atomic_method(recv, method, args),
            Expr::Match { expr, arms } => self.eval_match(expr, arms),
            Expr::If {
                cond,
                then_expr,
                else_expr,
            } => {
                if value_as_bool(self.eval(cond)) {
                    self.eval(then_expr)
                } else {
                    self.eval(else_expr)
                }
            }
            Expr::Call { func, args } => self.eval_call(func, args),
            Expr::Block(block) => match self.run_block(block) {
                Flow::Return(value) => value,
                Flow::Normal => panic!("effects::rust_ast: block expression has no tail value"),
                Flow::Break(_) | Flow::Continue(_) => {
                    panic!("effects::rust_ast: loop control escaped block expression")
                }
            },
            Expr::Unsafe(block) => match self.run_block(block) {
                Flow::Return(value) => value,
                Flow::Normal => panic!("effects::rust_ast: unsafe expression has no tail value"),
                Flow::Break(_) | Flow::Continue(_) => {
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
                self.atomic_store(place, *ordering, value.clone());
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
                    value.clone(),
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

    fn is_integer_method(&self, method: &str) -> bool {
        matches!(
            method,
            "wrapping_abs"
                | "reverse_bits"
                | "swap_bytes"
                | "leading_zeros"
                | "trailing_zeros"
                | "count_ones"
                | "rotate_left"
                | "rotate_right"
                | "overflowing_add"
                | "overflowing_sub"
                | "overflowing_mul"
                | "overflowing_div"
                | "overflowing_rem"
        )
    }

    fn eval_integer_method(&mut self, recv: &Expr, method: &str, args: &[Expr]) -> Value {
        let value = self.eval(recv);
        let Value::Int {
            width,
            signed,
            value,
        } = value
        else {
            panic!("effects::rust_ast: `{method}` expected integer receiver, found {value:?}");
        };
        let bits = int_width_bits(width).unwrap_or(64);
        let unsigned = truncate_to_bits(value, bits, false) as u128;
        match method {
            "wrapping_abs" => Value::Int {
                width,
                signed,
                value: truncate_to_bits(value.wrapping_abs(), bits, signed),
            },
            "reverse_bits" => Value::Int {
                width,
                signed,
                value: truncate_to_bits(reverse_bits(unsigned, bits), bits, signed),
            },
            "swap_bytes" => Value::Int {
                width,
                signed,
                value: truncate_to_bits(swap_bytes(unsigned, bits), bits, signed),
            },
            "leading_zeros" => int32(leading_zeros(unsigned, bits) as i128),
            "trailing_zeros" => int32(trailing_zeros(unsigned, bits) as i128),
            "count_ones" => int32(count_ones(unsigned, bits) as i128),
            "rotate_left" | "rotate_right" => {
                let [amount] = args else {
                    panic!("effects::rust_ast: `{method}` expects one argument");
                };
                let amount = value_as_u64(self.eval(amount)) as u32 % bits;
                let mask = bit_mask(bits);
                let rotated = if amount == 0 {
                    unsigned & mask
                } else if method == "rotate_left" {
                    ((unsigned << amount) | (unsigned >> (bits - amount))) & mask
                } else {
                    ((unsigned >> amount) | (unsigned << (bits - amount))) & mask
                };
                Value::Int {
                    width,
                    signed,
                    value: truncate_to_bits(rotated as i128, bits, signed),
                }
            }
            "overflowing_add" | "overflowing_sub" | "overflowing_mul" | "overflowing_div"
            | "overflowing_rem" => self.overflowing_method(width, signed, value, method, args),
            _ => unreachable!(),
        }
    }

    fn overflowing_method(
        &mut self,
        width: IntWidth,
        signed: bool,
        lhs: i128,
        method: &str,
        args: &[Expr],
    ) -> Value {
        let [rhs] = args else {
            panic!("effects::rust_ast: `{method}` expects one argument");
        };
        let rhs = value_as_i128(self.eval(rhs));
        let bits = int_width_bits(width).unwrap_or(64);
        let (value, overflowed) = overflowing_int_op(lhs, rhs, bits, signed, method);
        Value::Tuple(vec![
            Value::Int {
                width,
                signed,
                value,
            },
            Value::Bool(overflowed),
        ])
    }

    fn eval_tuple_field(&mut self, base: &Expr, index: usize) -> Value {
        match self.eval(base) {
            Value::Tuple(values) => values.get(index).cloned().unwrap_or_else(|| {
                panic!("effects::rust_ast: tuple field index {index} out of range")
            }),
            other => panic!("effects::rust_ast: tuple field on non-tuple `{other:?}`"),
        }
    }

    fn eval_match(&mut self, expr: &Expr, arms: &[crate::rust_ast::ExprMatchArm]) -> Value {
        let scrutinee = self.eval(expr);
        match scrutinee {
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
            value => {
                for arm in arms {
                    if pattern_matches(&arm.pattern, value.clone()) {
                        return self.eval(&arm.value);
                    }
                }
                panic!("effects::rust_ast: no match arm for {value:?}");
            }
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
        let value = self
            .scalars
            .get(name)
            .unwrap_or_else(|| panic!("effects::rust_ast: atomic access to unknown `{name}`"))
            .clone();
        let atomic = self.allocate_atomic(value.clone());
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
            value: value.clone(),
        });
        value
    }

    fn atomic_store(&mut self, place: &AtomicPlace, ordering: AtomicOrdering, value: Value) {
        let atomic = self.atomic_place(place);
        self.atomic_values.insert(atomic, value.clone());
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
        let new = atomic_rmw_value(op, old.clone(), operand.clone());
        self.atomic_values.insert(atomic, new.clone());
        self.trace.push(Effect::AtomicRmw {
            atomic,
            op,
            ordering,
            operand,
            old: old.clone(),
            new,
        });
        old
    }

    fn atomic_swap(&mut self, place: &AtomicPlace, ordering: AtomicOrdering, new: Value) -> Value {
        let atomic = self.atomic_place(place);
        let old = self.atomic_value(atomic);
        self.atomic_values.insert(atomic, new.clone());
        self.trace.push(Effect::AtomicSwap {
            atomic,
            ordering,
            old: old.clone(),
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
            self.atomic_values.insert(atomic, desired.clone());
        }
        self.trace.push(Effect::AtomicCompareExchange {
            atomic,
            success,
            failure,
            expected,
            desired,
            old: old.clone(),
            exchanged,
        });
        Value::AtomicResult {
            ok: exchanged,
            value: option_value(old),
        }
    }

    fn atomic_value(&self, atomic: AtomicId) -> Value {
        self.atomic_values
            .get(&atomic)
            .unwrap_or_else(|| panic!("effects::rust_ast: read from unknown atomic {atomic:?}"))
            .clone()
    }

    fn read_global(&mut self, name: &str) -> Value {
        let loc = *self
            .globals
            .get(name)
            .unwrap_or_else(|| panic!("effects::rust_ast: read of unknown global `{name}`"));
        let value = self
            .heap
            .get(&loc)
            .unwrap_or_else(|| panic!("effects::rust_ast: read from never-written {loc:?}"))
            .clone();
        self.trace.push(Effect::Read {
            loc,
            value: value.clone(),
        });
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
            self.heap.insert(payload, value.clone());
            self.trace.push(Effect::Write {
                loc: payload,
                value,
            });
            let one = Value::Int {
                width: IntWidth::W32,
                signed: true,
                value: 1,
            };
            self.heap.insert(guard, one.clone());
            self.trace.push(Effect::Write {
                loc: guard,
                value: one,
            });
        }
        self.read_loc(payload)
    }

    fn read_loc(&mut self, loc: Location) -> Value {
        let value = self
            .heap
            .get(&loc)
            .unwrap_or_else(|| panic!("effects::rust_ast: read from never-written {loc:?}"))
            .clone();
        self.trace.push(Effect::Read {
            loc,
            value: value.clone(),
        });
        value
    }

    fn write_loc(&mut self, loc: Location, value: Value) {
        self.heap.insert(loc, value.clone());
        for (name, scalar_loc) in &self.scalar_locs {
            if *scalar_loc == loc {
                self.scalars.insert(name.clone(), value.clone());
            }
        }
        self.trace.push(Effect::Write { loc, value });
    }

    fn addr_of(&mut self, expr: &Expr) -> Location {
        match expr {
            Expr::Var(ident) if self.globals.contains_key(ident.as_str()) => {
                self.globals[ident.as_str()]
            }
            Expr::Var(ident) if self.structs.contains_key(ident.as_str()) => Location {
                alloc: self.structs[ident.as_str()].alloc,
                byte_offset: 0,
            },
            Expr::Var(ident) => self.scalar_location(ident.as_str()),
            Expr::Index { base, index } => {
                if matches!(base.as_ref(), Expr::Field { .. }) {
                    return self.field_array_element_location(base, index);
                }
                let name = collection_name(base);
                let idx = value_as_u64(self.eval(index));
                self.materialize_collection(name);
                let binding = self.vecs.get(name).unwrap_or_else(|| {
                    panic!("effects::rust_ast: address of index into unknown Vec `{name}`")
                });
                Location {
                    alloc: binding.alloc,
                    byte_offset: idx * binding.elem_size,
                }
            }
            Expr::Field { base, field } => self.field_location(base, field),
            other => panic!("effects::rust_ast: unsupported address-of expression `{other:?}`"),
        }
    }

    fn scalar_location(&mut self, name: &str) -> Location {
        if let Some(loc) = self.scalar_locs.get(name).cloned() {
            return loc;
        }
        let value = self
            .scalars
            .get(name)
            .unwrap_or_else(|| panic!("effects::rust_ast: address of unknown scalar `{name}`"))
            .clone();
        let alloc = AllocId(self.next_alloc);
        self.next_alloc += 1;
        let loc = Location {
            alloc,
            byte_offset: 0,
        };
        self.scalar_locs.insert(name.to_string(), loc);
        self.heap.insert(loc, value.clone());
        self.trace.push(Effect::Alloc {
            alloc,
            size: local_value_size(&value),
        });
        self.trace.push(Effect::Write { loc, value });
        loc
    }

    fn eval_call(&mut self, func: &Expr, args: &[Expr]) -> Value {
        if args.is_empty() && is_path(func, &["std", "ptr", "null_mut"]) {
            return Value::Null;
        }
        if let Some(align) = self.generic_align_of(func, args) {
            return Value::Int {
                width: IntWidth::PointerSized,
                signed: false,
                value: align as i128,
            };
        }
        if let Some(size) = self.generic_size_of(func, args) {
            return Value::Int {
                width: IntWidth::PointerSized,
                signed: false,
                value: size as i128,
            };
        }
        if args.is_empty() && is_path(func, &["std", "mem", "size_of"]) {
            return int32(4);
        }
        if is_path(func, &["core", "hint", "assert_unchecked"]) {
            let [cond] = args else {
                panic!("effects::rust_ast: assert_unchecked expects one argument");
            };
            if !value_as_bool(self.eval(cond)) {
                panic!("effects::rust_ast: assert_unchecked(false)");
            }
            return int32(0);
        }
        if is_path(func, &["std", "ptr", "read_volatile"]) {
            return self.read_volatile(args);
        }
        if is_path(func, &["std", "ptr", "write_volatile"]) {
            self.write_volatile(args);
            return int32(0);
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
        match name.as_str() {
            "__slate_runtime::parse_i32" => return self.parse_runtime_i32(args),
            "__slate_runtime::parse_i64" => return self.parse_runtime_i64(args),
            "__slate_runtime::parse_u64" => return self.parse_runtime_u64(args),
            "__slate_runtime::parse_f64" => return self.parse_runtime_f64(args),
            _ => {}
        }
        let f = self
            .funcs
            .get(&name)
            .cloned()
            .unwrap_or_else(|| panic!("effects::rust_ast: unsupported call target `{name}`"));
        let values = args.iter().map(|arg| self.eval(arg)).collect::<Vec<_>>();
        self.call_user(&f, &values, Some(args))
    }

    fn eval_ref(&mut self, expr: &Expr) -> Location {
        match self.eval(expr) {
            Value::Ref(loc) => loc,
            other => panic!("effects::rust_ast: expected pointer value, found {other:?}"),
        }
    }

    fn collection_base(&mut self, expr: &Expr) -> Location {
        let name = collection_name(expr);
        self.materialize_collection(name);
        let binding = self
            .vecs
            .get(name)
            .unwrap_or_else(|| panic!("effects::rust_ast: pointer to unknown collection `{name}`"));
        Location {
            alloc: binding.alloc,
            byte_offset: 0,
        }
    }

    fn collection_len_from_arg(&self, expr: &Expr) -> Option<u64> {
        match expr {
            Expr::MethodCall { recv, method, args }
                if args.is_empty()
                    && matches!(
                        method.as_str(),
                        "as_slice" | "as_mut_slice" | "as_bytes" | "to_bytes"
                    ) =>
            {
                let name = collection_name(recv);
                self.vecs.get(name).map(|binding| binding.len)
            }
            Expr::Var(ident) => self.vecs.get(ident.as_str()).map(|binding| binding.len),
            _ => None,
        }
    }

    fn record_decl_type(&mut self, name: &str, ty: &Type) {
        if let Some(size) = pointer_elem_size_from_type(ty) {
            self.pointer_elem_sizes.insert(name.to_string(), size);
        }
    }

    fn pointer_elem_size(&self, expr: &Expr, loc: Location) -> u64 {
        match expr {
            Expr::Var(ident) => {
                if let Some(size) = self.pointer_elem_sizes.get(ident.as_str()) {
                    return *size;
                }
            }
            Expr::Cast { ty, .. } => {
                if let Some(size) = pointer_elem_size_from_type(ty) {
                    return size;
                }
            }
            Expr::Unsafe(block) => {
                if let Some(tail) = &block.tail {
                    return self.pointer_elem_size(tail, loc);
                }
            }
            Expr::MethodCall { recv, .. } => return self.pointer_elem_size(recv, loc),
            _ => {}
        }
        self.vecs
            .values()
            .find(|binding| binding.alloc == loc.alloc)
            .map(|binding| binding.elem_size)
            .unwrap_or(1)
    }

    fn materialize_collection(&mut self, name: &str) {
        let Some(binding) = self.vecs.get_mut(name) else {
            return;
        };
        if binding.alloc != LAZY_ARRAY_ALLOC {
            return;
        }
        let alloc = AllocId(self.next_alloc);
        self.next_alloc += 1;
        self.trace.push(Effect::Alloc {
            alloc,
            size: binding.len * binding.elem_size,
        });
        binding.alloc = alloc;
    }

    fn hidden_c_string(&mut self, bytes: &[u8]) -> Location {
        if let Some(&loc) = self.hidden_c_strings.get(bytes) {
            return loc;
        }
        let alloc = AllocId(self.next_alloc);
        self.next_alloc += 1;
        let base = Location {
            alloc,
            byte_offset: 0,
        };
        for (index, byte) in bytes.iter().cloned().chain(std::iter::once(0)).enumerate() {
            self.heap.insert(
                Location {
                    alloc,
                    byte_offset: index as u64,
                },
                Value::Int {
                    width: IntWidth::W8,
                    signed: true,
                    value: byte as i128,
                },
            );
        }
        self.hidden_c_strings.insert(bytes.to_vec(), base);
        base
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
            let loc = Location {
                alloc: base.alloc,
                byte_offset: base.byte_offset + index as u64,
            };
            self.heap.insert(loc, value.clone());
            self.trace.push(Effect::Write {
                loc,
                value: value.clone(),
            });
        }
    }

    fn read_volatile(&mut self, args: &[Expr]) -> Value {
        let [ptr] = args else {
            panic!("effects::rust_ast: read_volatile expects one argument");
        };
        let loc = self.eval_ref(ptr);
        self.read_loc(loc)
    }

    fn write_volatile(&mut self, args: &[Expr]) {
        let [ptr, value] = args else {
            panic!("effects::rust_ast: write_volatile expects two arguments");
        };
        let loc = self.eval_ref(ptr);
        let value = self.eval(value);
        self.write_loc(loc, value);
    }

    fn generic_size_of(&self, func: &Expr, args: &[Expr]) -> Option<u64> {
        if !args.is_empty() {
            return None;
        }
        let name = path_name(func)?;
        let ty_name = name
            .strip_prefix("std::mem::size_of::<")?
            .strip_suffix('>')?;
        Some(self.size_of_named_type(ty_name))
    }

    fn generic_align_of(&self, func: &Expr, args: &[Expr]) -> Option<u64> {
        if !args.is_empty() {
            return None;
        }
        let name = path_name(func)?;
        let ty_name = name
            .strip_prefix("std::mem::align_of::<")?
            .strip_suffix('>')?;
        Some(self.align_of_named_type(ty_name))
    }

    fn align_of_named_type(&self, name: &str) -> u64 {
        if let Some(prim) = Prim::parse(name) {
            let size = type_size(&Type::Prim(prim)).unwrap_or_else(|| {
                panic!("effects::rust_ast: unsupported primitive align `{name}`")
            });
            return size.min(8).max(1);
        }
        if let Some(record) = self.records.get(name) {
            return self.record_align(record);
        }
        if let Some(def) = self.tuple_structs.get(name) {
            return self.tuple_struct_align(def);
        }
        panic!("effects::rust_ast: unknown type `{name}` in align_of")
    }

    fn size_of_named_type(&self, name: &str) -> u64 {
        if let Some(prim) = Prim::parse(name) {
            return type_size(&Type::Prim(prim)).unwrap_or_else(|| {
                panic!("effects::rust_ast: unsupported primitive size `{name}`")
            });
        }
        let Some(record) = self.records.get(name) else {
            if let Some(def) = self.tuple_structs.get(name) {
                return self.tuple_struct_size(def);
            }
            panic!("effects::rust_ast: unknown type `{name}` in size_of");
        };
        if record.is_union {
            let size = record
                .fields
                .iter()
                .map(|field| self.type_layout(&field.ty).0)
                .max()
                .unwrap_or(0);
            return align_to(size, self.record_align(record));
        }
        let mut offset = 0u64;
        for field in &record.fields {
            let (size, align) = self.type_layout(&field.ty);
            if !record.packed {
                offset = align_to(offset, align);
            }
            offset += size;
        }
        align_to(offset, self.record_align(record))
    }

    fn type_layout(&self, ty: &Type) -> (u64, u64) {
        match ty {
            Type::Custom(name) => {
                let size = self.size_of_named_type(name);
                let align = if let Some(record) = self.records.get(name) {
                    self.record_align(record)
                } else if let Some(def) = self.tuple_structs.get(name) {
                    self.tuple_struct_align(def)
                } else {
                    1
                };
                (size, align)
            }
            Type::Array { elem, len } => {
                let (size, align) = self.type_layout(elem);
                (size * *len, align)
            }
            _ => {
                let size = type_size(ty).unwrap_or_else(|| {
                    panic!("effects::rust_ast: unsupported size_of type `{ty:?}`")
                });
                (size, size.min(8).max(1))
            }
        }
    }

    fn field_offset_named_type(&self, record_name: &str, field_name: &str) -> u64 {
        let record = self.records.get(record_name).unwrap_or_else(|| {
            panic!("effects::rust_ast: unknown type `{record_name}` in offset_of")
        });
        if record.is_union {
            return 0;
        }
        let mut offset = 0u64;
        for field in &record.fields {
            let (size, align) = self.type_layout(&field.ty);
            if !record.packed {
                offset = align_to(offset, align);
            }
            if field.name.as_str() == field_name {
                return offset;
            }
            offset += size;
        }
        panic!("effects::rust_ast: unknown field `{field_name}` in offset_of::<{record_name}>")
    }

    fn record_align(&self, record: &crate::rust_ast::RecordDef) -> u64 {
        if record.packed {
            return 1;
        }
        record.align.map(u64::from).unwrap_or_else(|| {
            record
                .fields
                .iter()
                .map(|field| self.type_layout(&field.ty).1)
                .max()
                .unwrap_or(1)
        })
    }

    fn tuple_struct_size(&self, def: &StructDef) -> u64 {
        let StructFields::Tuple(fields) = &def.fields else {
            return 0;
        };
        let mut offset = 0u64;
        for field in fields {
            let (size, align) = self.type_layout(field);
            offset = align_to(offset, align);
            offset += size;
        }
        align_to(offset, self.tuple_struct_align(def))
    }

    fn tuple_struct_align(&self, def: &StructDef) -> u64 {
        let field_align = match &def.fields {
            StructFields::Tuple(fields) => fields
                .iter()
                .map(|field| self.type_layout(field).1)
                .max()
                .unwrap_or(1),
            StructFields::Named(fields) => fields
                .iter()
                .map(|(_, field)| self.type_layout(field).1)
                .max()
                .unwrap_or(1),
        };
        def.attrs
            .iter()
            .filter_map(|attr| match attr {
                Attr::Repr(reprs) => Some(reprs),
                _ => None,
            })
            .flatten()
            .filter_map(|repr| match repr {
                Repr::Align(align) => Some(u64::from(*align)),
                _ => None,
            })
            .max()
            .unwrap_or(field_align)
            .max(field_align)
    }

    fn eval_call_summary(&mut self, summary: CallSummary, args: &[Expr]) -> Value {
        match summary {
            CallSummary::Malloc => self.call_malloc(args),
            CallSummary::Calloc => self.call_calloc(args),
            CallSummary::Free => self.call_free(args),
            CallSummary::Memcpy => self.call_memcpy(args),
            CallSummary::Memmove => self.call_memcpy(args),
            CallSummary::Memset => self.call_memset(args),
            CallSummary::Memchr => self.call_memchr(args),
            CallSummary::Strlen => self.call_strlen(args),
            CallSummary::Strcpy => self.call_strcpy(args),
            CallSummary::Strcat => self.call_strcat(args),
            CallSummary::Strncpy => self.call_strncpy(args),
            CallSummary::Strncat => self.call_strncat(args),
            CallSummary::Strcmp => self.call_strcmp(args, None),
            CallSummary::Strncmp => self.call_strncmp(args),
            CallSummary::Memcmp => self.call_memcmp(args),
            CallSummary::Strchr => self.call_strchr(args),
            CallSummary::Strrchr => self.call_strrchr(args),
            CallSummary::Strstr => self.call_strstr(args),
            CallSummary::Strpbrk => self.call_strpbrk(args),
            CallSummary::Strspn => self.call_strspn(args),
            CallSummary::Strcspn => self.call_strcspn(args),
            CallSummary::Atoi => self.call_atoi(args),
            CallSummary::Atol => self.call_atol(args),
            CallSummary::Strtol => self.call_strtol(args),
            CallSummary::Strtoul => self.call_strtoul(args),
            CallSummary::Strtod => self.call_strtod(args),
            CallSummary::Fopen => self.call_fopen(args),
            CallSummary::Fputs => self.call_fputs(args),
            CallSummary::Fgets => self.call_fgets(args),
            CallSummary::Fread => self.call_fread(args),
            CallSummary::Fwrite => self.call_fwrite(args),
            CallSummary::Fclose => self.call_fclose(args),
            CallSummary::Printf => self.call_printf(args),
            CallSummary::Puts => self.call_puts(args),
            CallSummary::Remove => self.call_remove(args),
            CallSummary::Toupper => self.call_toupper(args),
            CallSummary::Tolower => self.call_tolower(args),
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

    fn call_calloc(&mut self, args: &[Expr]) -> Value {
        let [count, elem_size] = args else {
            panic!("effects::rust_ast: calloc expects count and element size");
        };
        let size = value_as_u64(self.eval(count)).wrapping_mul(value_as_u64(self.eval(elem_size)));
        let alloc = AllocId(self.next_alloc);
        self.next_alloc += 1;
        self.trace.push(Effect::Alloc { alloc, size });
        let zero = Value::Int {
            width: IntWidth::W8,
            signed: true,
            value: 0,
        };
        for byte_offset in 0..size {
            self.heap
                .insert(Location { alloc, byte_offset }, zero.clone());
        }
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

    fn call_memcpy(&mut self, args: &[Expr]) -> Value {
        let [dst, src, len] = args else {
            panic!("effects::rust_ast: memcpy/memmove expects three arguments");
        };
        let dst = self.eval_ref(dst);
        let src = self.eval_ref(src);
        let len = value_as_u64(self.eval(len));
        let values = self.read_bytes(src, len);
        self.write_bytes(dst, &values);
        Value::Ref(dst)
    }

    fn call_memset(&mut self, args: &[Expr]) -> Value {
        let [dst, byte, len] = args else {
            panic!("effects::rust_ast: memset expects three arguments");
        };
        let dst = self.eval_ref(dst);
        let byte = value_as_i128(self.eval(byte)) as u8;
        let len = value_as_u64(self.eval(len));
        let value = Value::Int {
            width: IntWidth::W8,
            signed: true,
            value: byte as i128,
        };
        self.write_bytes(dst, &vec![value; len as usize]);
        Value::Ref(dst)
    }

    fn call_memchr(&mut self, args: &[Expr]) -> Value {
        let [base, needle, len] = args else {
            panic!("effects::rust_ast: memchr expects three arguments");
        };
        let base = self.eval_ref(base);
        let needle = value_as_i128(self.eval(needle)) as u8;
        let len = value_as_u64(self.eval(len));
        for index in 0..len {
            let loc = Location {
                alloc: base.alloc,
                byte_offset: base.byte_offset + index,
            };
            let value = self.read_loc(loc);
            if value_as_i128(value) as u8 == needle {
                return Value::Ref(loc);
            }
        }
        Value::Null
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

    fn call_strcpy(&mut self, args: &[Expr]) -> Value {
        let [dst, src] = args else {
            panic!("effects::rust_ast: strcpy expects dst and src");
        };
        let dst = self.eval_ref(dst);
        let bytes = self.string_expr_bytes(src);
        self.write_c_string(dst, &bytes);
        Value::Ref(dst)
    }

    fn call_strcat(&mut self, args: &[Expr]) -> Value {
        let [dst, src] = args else {
            panic!("effects::rust_ast: strcat expects dst and src");
        };
        let dst = self.eval_ref(dst);
        let offset = self.c_string_len(dst);
        let bytes = self.string_expr_bytes(src);
        self.write_c_string(
            Location {
                alloc: dst.alloc,
                byte_offset: dst.byte_offset + offset,
            },
            &bytes,
        );
        Value::Ref(dst)
    }

    fn call_strncpy(&mut self, args: &[Expr]) -> Value {
        let [dst, src, len] = args else {
            panic!("effects::rust_ast: strncpy expects dst, src, and len");
        };
        let dst = self.eval_ref(dst);
        let len = value_as_u64(self.eval(len)) as usize;
        let mut bytes = self.string_expr_bytes(src);
        bytes.truncate(len);
        let values = bytes
            .into_iter()
            .map(|byte| Value::Int {
                width: IntWidth::W8,
                signed: true,
                value: byte as i128,
            })
            .collect::<Vec<_>>();
        self.write_bytes(dst, &values);
        Value::Ref(dst)
    }

    fn call_strncat(&mut self, args: &[Expr]) -> Value {
        let [dst, src, len] = args else {
            panic!("effects::rust_ast: strncat expects dst, src, and len");
        };
        let dst = self.eval_ref(dst);
        let offset = self.c_string_len(dst);
        let len = value_as_u64(self.eval(len)) as usize;
        let mut bytes = self.string_expr_bytes(src);
        bytes.truncate(len);
        self.write_c_string(
            Location {
                alloc: dst.alloc,
                byte_offset: dst.byte_offset + offset,
            },
            &bytes,
        );
        Value::Ref(dst)
    }

    fn call_strcmp(&mut self, args: &[Expr], len: Option<usize>) -> Value {
        let [left, right] = args else {
            panic!("effects::rust_ast: strcmp expects two arguments");
        };
        let mut left = self.string_expr_bytes(left);
        let mut right = self.string_expr_bytes(right);
        if let Some(len) = len {
            left.truncate(len);
            right.truncate(len);
        }
        int32(compare_bytes(&left, &right) as i128)
    }

    fn call_strncmp(&mut self, args: &[Expr]) -> Value {
        let [left, right, len] = args else {
            panic!("effects::rust_ast: strncmp expects three arguments");
        };
        let len = value_as_u64(self.eval(len)) as usize;
        self.call_strcmp(&[left.clone(), right.clone()], Some(len))
    }

    fn call_memcmp(&mut self, args: &[Expr]) -> Value {
        let [left, right, len] = args else {
            panic!("effects::rust_ast: memcmp expects three arguments");
        };
        let left = self.eval_ref(left);
        let right = self.eval_ref(right);
        let len = value_as_u64(self.eval(len));
        let left = self
            .read_bytes(left, len)
            .into_iter()
            .map(|value| value_as_i128(value) as u8)
            .collect::<Vec<_>>();
        let right = self
            .read_bytes(right, len)
            .into_iter()
            .map(|value| value_as_i128(value) as u8)
            .collect::<Vec<_>>();
        int32(compare_bytes(&left, &right) as i128)
    }

    fn call_strchr(&mut self, args: &[Expr]) -> Value {
        let [base, needle] = args else {
            panic!("effects::rust_ast: strchr expects string and needle");
        };
        let base = self.eval_ref(base);
        let needle = value_as_i128(self.eval(needle)) as u8;
        let bytes = self.read_c_string(base);
        for (index, byte) in bytes.iter().cloned().chain(std::iter::once(0)).enumerate() {
            if byte == needle {
                return Value::Ref(Location {
                    alloc: base.alloc,
                    byte_offset: base.byte_offset + index as u64,
                });
            }
        }
        Value::Null
    }

    fn call_strrchr(&mut self, args: &[Expr]) -> Value {
        let [base, needle] = args else {
            panic!("effects::rust_ast: strrchr expects string and needle");
        };
        let base = self.eval_ref(base);
        let needle = value_as_i128(self.eval(needle)) as u8;
        let bytes = self.read_c_string(base);
        let mut found = if needle == 0 { Some(bytes.len()) } else { None };
        for (index, byte) in bytes.iter().cloned().enumerate() {
            if byte == needle {
                found = Some(index);
            }
        }
        found
            .map(|index| {
                Value::Ref(Location {
                    alloc: base.alloc,
                    byte_offset: base.byte_offset + index as u64,
                })
            })
            .unwrap_or(Value::Null)
    }

    fn call_strstr(&mut self, args: &[Expr]) -> Value {
        let [haystack, needle] = args else {
            panic!("effects::rust_ast: strstr expects haystack and needle");
        };
        let base = self.eval_ref(haystack);
        let haystack = self.read_c_string(base);
        let needle = self.string_expr_bytes(needle);
        let found = if needle.is_empty() {
            Some(0)
        } else {
            haystack
                .windows(needle.len())
                .position(|window| window == needle.as_slice())
        };
        found
            .map(|index| {
                Value::Ref(Location {
                    alloc: base.alloc,
                    byte_offset: base.byte_offset + index as u64,
                })
            })
            .unwrap_or(Value::Null)
    }

    fn call_strpbrk(&mut self, args: &[Expr]) -> Value {
        let [base, set] = args else {
            panic!("effects::rust_ast: strpbrk expects string and set");
        };
        let base = self.eval_ref(base);
        let bytes = self.read_c_string(base);
        let set = self.string_expr_bytes(set);
        bytes
            .iter()
            .position(|byte| set.contains(byte))
            .map(|index| {
                Value::Ref(Location {
                    alloc: base.alloc,
                    byte_offset: base.byte_offset + index as u64,
                })
            })
            .unwrap_or(Value::Null)
    }

    fn call_strspn(&mut self, args: &[Expr]) -> Value {
        let [base, set] = args else {
            panic!("effects::rust_ast: strspn expects string and set");
        };
        let bytes = self.string_expr_bytes(base);
        let set = self.string_expr_bytes(set);
        let len = bytes.iter().take_while(|byte| set.contains(byte)).count();
        Value::Int {
            width: IntWidth::PointerSized,
            signed: false,
            value: len as i128,
        }
    }

    fn call_strcspn(&mut self, args: &[Expr]) -> Value {
        let [base, reject] = args else {
            panic!("effects::rust_ast: strcspn expects string and reject set");
        };
        let bytes = self.string_expr_bytes(base);
        let reject = self.string_expr_bytes(reject);
        let len = bytes
            .iter()
            .take_while(|byte| !reject.contains(byte))
            .count();
        Value::Int {
            width: IntWidth::PointerSized,
            signed: false,
            value: len as i128,
        }
    }

    fn call_atoi(&mut self, args: &[Expr]) -> Value {
        let [base] = args else {
            panic!("effects::rust_ast: atoi expects string");
        };
        int32(parse_i128_prefix(&self.string_expr_bytes(base)) as i32 as i128)
    }

    fn call_atol(&mut self, args: &[Expr]) -> Value {
        let [base] = args else {
            panic!("effects::rust_ast: atol expects string");
        };
        Value::Int {
            width: IntWidth::W64,
            signed: true,
            value: parse_i128_prefix(&self.string_expr_bytes(base)) as i64 as i128,
        }
    }

    fn call_strtol(&mut self, args: &[Expr]) -> Value {
        let [base, end, _base_arg] = args else {
            panic!("effects::rust_ast: strtol expects string, end pointer, and base");
        };
        let loc = self.eval_ref(base);
        let bytes = self.read_c_string(loc);
        self.write_end_pointer(end, loc, decimal_prefix_len(&bytes));
        Value::Int {
            width: IntWidth::W64,
            signed: true,
            value: parse_i128_prefix(&bytes) as i64 as i128,
        }
    }

    fn call_strtoul(&mut self, args: &[Expr]) -> Value {
        let [base, end, _base_arg] = args else {
            panic!("effects::rust_ast: strtoul expects string, end pointer, and base");
        };
        let loc = self.eval_ref(base);
        let bytes = self.read_c_string(loc);
        self.write_end_pointer(end, loc, decimal_prefix_len(&bytes));
        Value::Int {
            width: IntWidth::W64,
            signed: false,
            value: parse_u128_prefix(&bytes) as u64 as i128,
        }
    }

    fn call_strtod(&mut self, args: &[Expr]) -> Value {
        let [base, end] = args else {
            panic!("effects::rust_ast: strtod expects string and end pointer");
        };
        let loc = self.eval_ref(base);
        let bytes = self.read_c_string(loc);
        self.write_end_pointer(end, loc, float_prefix_len(&bytes));
        Value::Float(parse_f64_prefix(&bytes))
    }

    fn write_end_pointer(&mut self, end: &Expr, base: Location, offset: usize) {
        match self.eval(end) {
            Value::Null => {}
            Value::Ref(loc) => self.write_loc(
                loc,
                Value::Ref(Location {
                    alloc: base.alloc,
                    byte_offset: base.byte_offset + offset as u64,
                }),
            ),
            other => panic!("effects::rust_ast: strto* end pointer was {other:?}"),
        }
    }

    fn call_fopen(&mut self, args: &[Expr]) -> Value {
        let [path, mode] = args else {
            panic!("effects::rust_ast: fopen expects path and mode");
        };
        let file = FileId(self.next_file);
        self.next_file += 1;
        let path = String::from_utf8_lossy(&self.string_expr_bytes(path)).into_owned();
        let mode = String::from_utf8_lossy(&self.string_expr_bytes(mode)).into_owned();
        self.file_paths.insert(file, path.clone());
        self.file_offsets.insert(file, 0);
        if mode.contains('w') {
            self.file_contents.insert(path.clone(), Vec::new());
        }
        self.trace.push(Effect::FileOpen { file, path, mode });
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
        let bytes = self.string_expr_bytes(bytes);
        self.append_file_bytes(file, &bytes);
        self.trace.push(Effect::FileWrite { file, bytes });
        int32(0)
    }

    fn call_fgets(&mut self, args: &[Expr]) -> Value {
        let [dst, len, file] = args else {
            panic!("effects::rust_ast: fgets expects buffer, size, and file");
        };
        let dst = self.eval_ref(dst);
        let len = value_as_u64(self.eval(len)) as usize;
        let file = self.eval_file(file, "fgets");
        if len == 0 {
            return Value::Null;
        }
        let bytes = self.read_file_bytes(file, len.saturating_sub(1), true);
        if bytes.is_empty() {
            return Value::Null;
        }
        self.write_c_string(dst, &bytes);
        Value::Ref(dst)
    }

    fn call_fread(&mut self, args: &[Expr]) -> Value {
        let [dst, size, count, file] = args else {
            panic!("effects::rust_ast: fread expects buffer, size, count, and file");
        };
        let dst = self.eval_ref(dst);
        let size = value_as_u64(self.eval(size));
        let count = value_as_u64(self.eval(count));
        let file = self.eval_file(file, "fread");
        let max = size.saturating_mul(count);
        let bytes = self.read_file_bytes(file, max as usize, false);
        let values = bytes
            .iter()
            .cloned()
            .map(|byte| Value::Int {
                width: IntWidth::W8,
                signed: true,
                value: byte as i128,
            })
            .collect::<Vec<_>>();
        self.write_bytes(dst, &values);
        Value::Int {
            width: IntWidth::PointerSized,
            signed: false,
            value: if size == 0 {
                0
            } else {
                (bytes.len() as u64 / size) as i128
            },
        }
    }

    fn call_fwrite(&mut self, args: &[Expr]) -> Value {
        let [src, size, count, file] = args else {
            panic!("effects::rust_ast: fwrite expects buffer, size, count, and file");
        };
        let src = self.eval_ref(src);
        let size = value_as_u64(self.eval(size));
        let count = value_as_u64(self.eval(count));
        let file = self.eval_file(file, "fwrite");
        let len = size.saturating_mul(count);
        let bytes = self
            .read_bytes(src, len)
            .into_iter()
            .map(|value| value_as_i128(value) as u8)
            .collect::<Vec<_>>();
        self.append_file_bytes(file, &bytes);
        self.trace.push(Effect::FileWrite { file, bytes });
        Value::Int {
            width: IntWidth::PointerSized,
            signed: false,
            value: count as i128,
        }
    }

    fn call_fclose(&mut self, args: &[Expr]) -> Value {
        let [file] = args else {
            panic!("effects::rust_ast: fclose expects file");
        };
        let file = self.eval_file(file, "fclose");
        self.trace.push(Effect::FileClose { file });
        int32(0)
    }

    fn eval_file(&mut self, expr: &Expr, callee: &str) -> FileId {
        match self.eval(expr) {
            Value::File(file) => file,
            other => panic!("effects::rust_ast: {callee} expected file handle, found {other:?}"),
        }
    }

    fn append_file_bytes(&mut self, file: FileId, bytes: &[u8]) {
        let Some(path) = self.file_paths.get(&file).cloned() else {
            return;
        };
        self.file_contents.entry(path).or_default().extend(bytes);
    }

    fn read_file_bytes(&mut self, file: FileId, max: usize, stop_at_newline: bool) -> Vec<u8> {
        let path = self
            .file_paths
            .get(&file)
            .cloned()
            .unwrap_or_else(|| panic!("effects::rust_ast: unknown file handle {file:?}"));
        let contents = self.file_contents.entry(path).or_default();
        let offset = self.file_offsets.entry(file).or_insert(0);
        let mut end = (*offset + max).min(contents.len());
        if stop_at_newline
            && let Some(newline) = contents[*offset..end]
                .iter()
                .position(|byte| *byte == b'\n')
        {
            end = *offset + newline + 1;
        }
        let bytes = contents[*offset..end].to_vec();
        *offset = end;
        bytes
    }

    fn call_printf(&mut self, args: &[Expr]) -> Value {
        let [fmt_expr, rest @ ..] = args else {
            panic!("effects::rust_ast: printf expects a format string");
        };
        let specs = c_format_specs(&c_format_bytes(fmt_expr));
        let values = rest
            .iter()
            .enumerate()
            .map(|(index, arg)| {
                let value = self.eval(arg);
                match specs.get(index) {
                    Some(CFormatSpec::Str) => self.resolve_string_arg(value),
                    Some(CFormatSpec::Char) => resolve_char_arg(value),
                    Some(CFormatSpec::Num {
                        conv,
                        alternate,
                        zero_pad,
                        left_align,
                        width,
                    }) => Value::Bytes(render_c_num_arg(
                        &value,
                        *conv,
                        *alternate,
                        *zero_pad,
                        *left_align,
                        *width,
                    )),
                    _ => value,
                }
            })
            .collect::<Vec<_>>();
        self.trace.push(Effect::Call {
            name: "printf".to_string(),
            args: values,
        });
        int32(0)
    }

    fn call_puts(&mut self, args: &[Expr]) -> Value {
        let [arg] = args else {
            panic!("effects::rust_ast: puts expects one argument");
        };
        let loc = self.eval_ref(arg);
        self.trace.push(Effect::Call {
            name: "puts".to_string(),
            args: vec![Value::Ref(loc)],
        });
        int32(0)
    }

    fn call_remove(&mut self, args: &[Expr]) -> Value {
        let [path] = args else {
            panic!("effects::rust_ast: remove expects path");
        };
        let loc = self.eval_ref(path);
        let path_bytes = self.read_c_string(loc);
        let path = String::from_utf8_lossy(&path_bytes).into_owned();
        self.file_contents.remove(&path);
        self.trace.push(Effect::Call {
            name: "remove".to_string(),
            args: vec![Value::Ref(loc)],
        });
        int32(0)
    }

    fn call_toupper(&mut self, args: &[Expr]) -> Value {
        let [arg] = args else {
            panic!("effects::rust_ast: toupper expects one argument");
        };
        let value = value_as_i128(self.eval(arg)) as u8;
        int32(value.to_ascii_uppercase() as i128)
    }

    fn call_tolower(&mut self, args: &[Expr]) -> Value {
        let [arg] = args else {
            panic!("effects::rust_ast: tolower expects one argument");
        };
        let value = value_as_i128(self.eval(arg)) as u8;
        int32(value.to_ascii_lowercase() as i128)
    }

    fn parse_string_method(&mut self, recv: &Expr, type_args: &[Type]) -> Value {
        let [ty] = type_args else {
            panic!("effects::rust_ast: parse expects one type argument");
        };
        let bytes = self.string_expr_bytes(recv);
        let value = match ty {
            Type::Prim(Prim::I32) => OptionValue::Int {
                width: IntWidth::W32,
                signed: true,
                value: parse_i128_prefix(&bytes) as i32 as i128,
            },
            Type::Prim(Prim::I64) => OptionValue::Int {
                width: IntWidth::W64,
                signed: true,
                value: parse_i128_prefix(&bytes) as i64 as i128,
            },
            Type::Prim(Prim::U64) => OptionValue::Int {
                width: IntWidth::W64,
                signed: false,
                value: parse_u128_prefix(&bytes) as u64 as i128,
            },
            other => panic!("effects::rust_ast: unsupported parse type `{other:?}`"),
        };
        Value::Option(Some(value))
    }

    fn unwrap_or(&mut self, recv: &Expr, args: &[Expr]) -> Value {
        let [default] = args else {
            panic!("effects::rust_ast: unwrap_or expects one argument");
        };
        match self.eval(recv) {
            Value::Option(Some(value)) => option_value_to_value(value),
            Value::Option(None) => self.eval(default),
            other => panic!("effects::rust_ast: unwrap_or on unsupported value {other:?}"),
        }
    }

    fn parse_runtime_i32(&mut self, args: &[Expr]) -> Value {
        let [arg] = args else {
            panic!("effects::rust_ast: parse_i32 expects one argument");
        };
        int32(parse_i128_prefix(&self.string_expr_bytes(arg)) as i32 as i128)
    }

    fn parse_runtime_i64(&mut self, args: &[Expr]) -> Value {
        let [arg] = args else {
            panic!("effects::rust_ast: parse_i64 expects one argument");
        };
        Value::Int {
            width: IntWidth::W64,
            signed: true,
            value: parse_i128_prefix(&self.string_expr_bytes(arg)) as i64 as i128,
        }
    }

    fn parse_runtime_u64(&mut self, args: &[Expr]) -> Value {
        let [arg] = args else {
            panic!("effects::rust_ast: parse_u64 expects one argument");
        };
        Value::Int {
            width: IntWidth::W64,
            signed: false,
            value: parse_u128_prefix(&self.string_expr_bytes(arg)) as u64 as i128,
        }
    }

    fn parse_runtime_f64(&mut self, args: &[Expr]) -> Value {
        let [arg] = args else {
            panic!("effects::rust_ast: parse_f64 expects one argument");
        };
        Value::Float(parse_f64_prefix(&self.string_expr_bytes(arg)))
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
            let value = self.call_user(&f, &[Value::Ref(key), Value::Ref(elem)], None);
            if value_as_i128(value) == 0 {
                return Value::Ref(elem);
            }
        }
        Value::Null
    }

    fn call_user(&mut self, f: &FnDef, args: &[Value], arg_exprs: Option<&[Expr]>) -> Value {
        if f.params.len() != args.len() {
            panic!(
                "effects::rust_ast: user function `{}` expected {} arg(s), got {}",
                f.name,
                f.params.len(),
                args.len()
            );
        }
        let saved_scalars = std::mem::take(&mut self.scalars);
        let saved_structs = self.structs.clone();
        let saved_scalar_locs = self.scalar_locs.clone();
        let saved_vecs = self.vecs.clone();
        let saved_pointer_elem_sizes = self.pointer_elem_sizes.clone();
        for (index, (param, value)) in f.params.iter().zip(args).enumerate() {
            self.record_decl_type(&param.name, &param.ty);
            if let Value::Ref(loc) = value
                && !matches!(param.ty, Type::Ptr { .. } | Type::Ref { .. })
                && self.aggregate_allocs_contains(*loc)
            {
                self.bind_struct_copy(&param.name, *loc);
            } else {
                self.scalars.insert(param.name.to_string(), value.clone());
            }
            if let Value::Ref(loc) = value
                && let Some((elem_width, elem_signed, elem_size)) = slice_elem_shape(&param.ty)
            {
                let len = arg_exprs
                    .and_then(|exprs| exprs.get(index))
                    .and_then(|expr| self.collection_len_from_arg(expr))
                    .unwrap_or(0);
                self.vecs.insert(
                    param.name.to_string(),
                    VecBinding {
                        alloc: loc.alloc,
                        elem_width,
                        elem_signed,
                        elem_size,
                        len,
                        owned: false,
                    },
                );
            }
        }
        self.call_depth += 1;
        let flow = self.run(&f.body);
        self.call_depth -= 1;
        let returned_struct = match &flow {
            Flow::Return(Value::Ref(loc)) => self
                .structs
                .values()
                .find(|binding| binding.alloc == loc.alloc)
                .cloned(),
            _ => None,
        };
        let mut restored_scalars = saved_scalars;
        for (name, loc) in &saved_scalar_locs {
            if let Some(value) = self.heap.get(loc).cloned() {
                restored_scalars.insert(name.clone(), value);
            }
        }
        self.scalars = restored_scalars;
        self.structs = saved_structs;
        self.scalar_locs = saved_scalar_locs;
        self.vecs = saved_vecs;
        self.pointer_elem_sizes = saved_pointer_elem_sizes;
        let Flow::Return(value) = flow else {
            panic!(
                "effects::rust_ast: user function `{}` did not return",
                f.name
            );
        };
        if let (Value::Ref(loc), Some(binding)) = (&value, returned_struct) {
            let name = format!("__struct_tmp{}", self.next_struct_temp);
            self.next_struct_temp += 1;
            self.structs.insert(name, binding);
            return Value::Ref(*loc);
        }
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
            let value = this.call_user(&f, &[Value::Ref(left), Value::Ref(right)], None);
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
                self.swap_elements(left, right, elem_size);
                j -= 1;
            }
        }
    }

    fn swap_elements(&mut self, left: Location, right: Location, elem_size: u64) {
        let mut offsets = self
            .heap
            .keys()
            .filter_map(|loc| {
                if loc.alloc != left.alloc {
                    return None;
                }
                if (left.byte_offset..left.byte_offset + elem_size).contains(&loc.byte_offset) {
                    return Some(loc.byte_offset - left.byte_offset);
                }
                if (right.byte_offset..right.byte_offset + elem_size).contains(&loc.byte_offset) {
                    return Some(loc.byte_offset - right.byte_offset);
                }
                None
            })
            .collect::<Vec<_>>();
        offsets.sort_unstable();
        offsets.dedup();
        for offset in offsets {
            let left_loc = Location {
                alloc: left.alloc,
                byte_offset: left.byte_offset + offset,
            };
            let right_loc = Location {
                alloc: right.alloc,
                byte_offset: right.byte_offset + offset,
            };
            let left_value = self.heap.get(&left_loc).cloned();
            let right_value = self.heap.get(&right_loc).cloned();
            if let Some(value) = right_value {
                self.heap.insert(left_loc, value.clone());
                self.trace.push(Effect::Write {
                    loc: left_loc,
                    value,
                });
            }
            if let Some(value) = left_value {
                self.heap.insert(right_loc, value.clone());
                self.trace.push(Effect::Write {
                    loc: right_loc,
                    value,
                });
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
            let value = self
                .heap
                .get(&loc)
                .unwrap_or_else(|| panic!("effects::rust_ast: read from never-written {loc:?}"))
                .clone();
            self.trace.push(Effect::Read {
                loc,
                value: value.clone(),
            });
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
            let value = this
                .heap
                .get(&loc)
                .unwrap_or_else(|| panic!("effects::rust_ast: read from never-written {loc:?}"))
                .clone();
            this.trace.push(Effect::Read {
                loc,
                value: value.clone(),
            });
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

fn pattern_matches(pattern: &Pattern, value: Value) -> bool {
    match (pattern, value) {
        (Pattern::Wildcard, _) => true,
        (Pattern::I64(expected), Value::Int { value, .. }) => *expected as i128 == value,
        (Pattern::I128(expected), Value::Int { value, .. }) => *expected == value,
        (Pattern::Binding(_), _) => true,
        _ => false,
    }
}

fn tuple_pat2(pat: &str) -> (&str, &str) {
    let Some(inner) = pat.strip_prefix('(').and_then(|pat| pat.strip_suffix(')')) else {
        panic!("effects::rust_ast: enumerate loop expects tuple pattern, found `{pat}`");
    };
    let Some((left, right)) = inner.split_once(',') else {
        panic!("effects::rust_ast: enumerate loop expects two pattern fields, found `{pat}`");
    };
    let left = left.trim();
    let right = right.trim();
    if left.is_empty() || right.is_empty() || right.contains(',') {
        panic!("effects::rust_ast: enumerate loop expects two bindings, found `{pat}`");
    }
    (left, right)
}

#[derive(Clone, Copy, PartialEq, Eq)]
enum CFormatSpec {
    Str,
    Char,
    Num {
        conv: u8,
        alternate: bool,
        zero_pad: bool,
        left_align: bool,
        width: Option<usize>,
    },
    Other,
}

fn c_format_specs(fmt: &[u8]) -> Vec<CFormatSpec> {
    let mut specs = Vec::new();
    let mut i = 0;
    while i < fmt.len() {
        if fmt[i] != b'%' {
            i += 1;
            continue;
        }
        i += 1;
        if i < fmt.len() && fmt[i] == b'%' {
            i += 1;
            continue;
        }
        let mut alternate = false;
        let mut zero_pad = false;
        let mut left_align = false;
        while i < fmt.len() && matches!(fmt[i], b'-' | b'+' | b'0' | b' ' | b'#') {
            match fmt[i] {
                b'#' => alternate = true,
                b'0' => zero_pad = true,
                b'-' => left_align = true,
                _ => {}
            }
            i += 1;
        }
        let width_start = i;
        while i < fmt.len() && fmt[i].is_ascii_digit() {
            i += 1;
        }
        let width = if i > width_start {
            std::str::from_utf8(&fmt[width_start..i])
                .ok()
                .and_then(|digits| digits.parse().ok())
        } else {
            None
        };
        if i < fmt.len() && fmt[i] == b'.' {
            i += 1;
            while i < fmt.len() && fmt[i].is_ascii_digit() {
                i += 1;
            }
        }
        while i < fmt.len() && matches!(fmt[i], b'h' | b'l' | b'L' | b'q' | b'j' | b'z' | b't') {
            i += 1;
        }
        if i >= fmt.len() {
            break;
        }
        let conv = fmt[i];
        i += 1;
        specs.push(match conv {
            b's' => CFormatSpec::Str,
            b'c' => CFormatSpec::Char,
            b'x' | b'X' | b'o' if alternate => CFormatSpec::Num {
                conv,
                alternate,
                zero_pad,
                left_align,
                width,
            },
            _ => CFormatSpec::Other,
        });
    }
    specs
}

fn c_format_bytes(expr: &Expr) -> Vec<u8> {
    match expr {
        Expr::Str(s) => s.as_bytes().to_vec(),
        Expr::ByteStr(bytes) | Expr::CStr(bytes) => bytes.clone(),
        Expr::Cast { expr, .. } => c_format_bytes(expr),
        Expr::MethodCall { recv, method, .. } if method == "as_ptr" => c_format_bytes(recv),
        other => panic!("effects::rust_ast: unsupported printf format string `{other:?}`"),
    }
}

fn resolve_char_arg(value: Value) -> Value {
    match value {
        Value::Int { value, .. } => Value::Bytes(vec![value as u8]),
        other => other,
    }
}

fn render_c_num_arg(
    value: &Value,
    conv: u8,
    alternate: bool,
    zero_pad: bool,
    left_align: bool,
    width: Option<usize>,
) -> Vec<u8> {
    let Value::Int {
        value,
        width: int_width,
        ..
    } = value
    else {
        panic!("effects::rust_ast: printf numeric conversion expects an integer, found {value:?}");
    };
    let mut core = match conv {
        b'x' => format_uint_radix(*value, *int_width, 16, false),
        b'X' => format_uint_radix(*value, *int_width, 16, true),
        b'o' => format_uint_radix(*value, *int_width, 8, false),
        _ => panic!(
            "effects::rust_ast: unsupported printf conversion `{}`",
            conv as char
        ),
    };
    let is_zero = *value == 0;
    let prefix = match conv {
        b'x' if alternate && !is_zero => "0x",
        b'X' if alternate && !is_zero => "0X",
        _ => "",
    };
    if conv == b'o' && alternate && !is_zero {
        core = format!("0{core}");
    }
    let body = format!("{prefix}{core}");
    let total_width = width.unwrap_or(0);
    let pad_len = total_width.saturating_sub(body.chars().count());
    if pad_len == 0 {
        return body.into_bytes();
    }
    if left_align {
        let mut out = body;
        out.extend(std::iter::repeat_n(' ', pad_len));
        return out.into_bytes();
    }
    if zero_pad {
        let mut out = String::with_capacity(total_width);
        out.push_str(prefix);
        out.extend(std::iter::repeat_n('0', pad_len));
        out.push_str(&core);
        return out.into_bytes();
    }
    let mut out: String = std::iter::repeat_n(' ', pad_len).collect();
    out.push_str(&body);
    out.into_bytes()
}

#[derive(Clone, Copy, PartialEq, Eq)]
enum Align {
    Left,
    Right,
    Center,
}

#[derive(Clone, Copy)]
struct FormatSpec {
    fill: char,
    align: Option<Align>,
    alternate: bool,
    zero_pad: bool,
    width: Option<usize>,
    ty: char,
}

enum FormatSegment<'a> {
    Literal(&'a str),
    Placeholder(FormatSpec),
}

fn parse_format_string(fmt: &str) -> Vec<FormatSegment<'_>> {
    let mut segments = Vec::new();
    let mut literal_start = 0;
    let mut i = 0;
    let bytes = fmt.as_bytes();
    while i < bytes.len() {
        match bytes[i] {
            b'{' if bytes.get(i + 1) == Some(&b'{') => {
                segments.push(FormatSegment::Literal(&fmt[literal_start..=i]));
                i += 2;
                literal_start = i;
            }
            b'}' if bytes.get(i + 1) == Some(&b'}') => {
                segments.push(FormatSegment::Literal(&fmt[literal_start..=i]));
                i += 2;
                literal_start = i;
            }
            b'{' => {
                if i > literal_start {
                    segments.push(FormatSegment::Literal(&fmt[literal_start..i]));
                }
                let start = i + 1;
                let Some(end_rel) = fmt[start..].find('}') else {
                    panic!("effects::rust_ast: unterminated format placeholder in `{fmt}`");
                };
                let inner = &fmt[start..start + end_rel];
                segments.push(FormatSegment::Placeholder(parse_format_spec(
                    inner.strip_prefix(':').unwrap_or(""),
                )));
                i = start + end_rel + 1;
                literal_start = i;
            }
            _ => i += 1,
        }
    }
    if literal_start < fmt.len() {
        segments.push(FormatSegment::Literal(&fmt[literal_start..]));
    }
    segments
}

fn parse_format_spec(spec: &str) -> FormatSpec {
    let chars: Vec<char> = spec.chars().collect();
    let mut i = 0;
    let mut fill = ' ';
    let mut align = None;
    if chars.len() >= 2 && matches!(chars[1], '<' | '>' | '^') {
        fill = chars[0];
        align = Some(match chars[1] {
            '<' => Align::Left,
            '>' => Align::Right,
            _ => Align::Center,
        });
        i = 2;
    } else if !chars.is_empty() && matches!(chars[0], '<' | '>' | '^') {
        align = Some(match chars[0] {
            '<' => Align::Left,
            '>' => Align::Right,
            _ => Align::Center,
        });
        i = 1;
    }
    let mut alternate = false;
    if i < chars.len() && chars[i] == '#' {
        alternate = true;
        i += 1;
    }
    let mut zero_pad = false;
    if i < chars.len() && chars[i] == '0' {
        zero_pad = true;
        i += 1;
    }
    let width_start = i;
    while i < chars.len() && chars[i].is_ascii_digit() {
        i += 1;
    }
    let width = if i > width_start {
        Some(
            chars[width_start..i]
                .iter()
                .collect::<String>()
                .parse()
                .unwrap(),
        )
    } else {
        None
    };
    if i < chars.len() && chars[i] == '.' {
        i += 1;
        while i < chars.len() && chars[i].is_ascii_digit() {
            i += 1;
        }
    }
    let ty = chars.get(i).copied().unwrap_or('\0');
    FormatSpec {
        fill,
        align,
        alternate,
        zero_pad,
        width,
        ty,
    }
}

fn format_uint_radix(value: i128, width: IntWidth, base: u32, upper: bool) -> String {
    let bits: u32 = match width {
        IntWidth::W8 => 8,
        IntWidth::W16 => 16,
        IntWidth::W32 => 32,
        IntWidth::W64 | IntWidth::PointerSized => 64,
        IntWidth::W128 => 128,
    };
    let mask: u128 = if bits >= 128 {
        u128::MAX
    } else {
        (1u128 << bits) - 1
    };
    let bits_value = (value as u128) & mask;
    match base {
        16 if upper => format!("{bits_value:X}"),
        16 => format!("{bits_value:x}"),
        8 => format!("{bits_value:o}"),
        2 => format!("{bits_value:b}"),
        _ => unreachable!("effects::rust_ast: unsupported format! radix {base}"),
    }
}

fn render_format_arg(value: &Value, spec: &FormatSpec) -> Vec<u8> {
    let (core, is_numeric) = match (value, spec.ty) {
        (Value::Int { value, width, .. }, 'x') => {
            (format_uint_radix(*value, *width, 16, false), true)
        }
        (Value::Int { value, width, .. }, 'X') => {
            (format_uint_radix(*value, *width, 16, true), true)
        }
        (Value::Int { value, width, .. }, 'o') => {
            (format_uint_radix(*value, *width, 8, false), true)
        }
        (Value::Int { value, width, .. }, 'b') => {
            (format_uint_radix(*value, *width, 2, false), true)
        }
        (Value::Int { value, .. }, _) => (value.to_string(), true),
        (Value::Bytes(bytes), _) => (String::from_utf8_lossy(bytes).into_owned(), false),
        (Value::Float(value), _) => (value.to_string(), false),
        (Value::Bool(value), _) => (value.to_string(), false),
        other => panic!("effects::rust_ast: unsupported format! argument `{other:?}`"),
    };
    let prefix = if is_numeric && spec.alternate {
        match spec.ty {
            'x' => "0x",
            'X' => "0X",
            'o' => "0o",
            'b' => "0b",
            _ => "",
        }
    } else {
        ""
    };
    let body = format!("{prefix}{core}");
    let width = spec.width.unwrap_or(0);
    let pad_len = width.saturating_sub(body.chars().count());
    if pad_len == 0 {
        return body.into_bytes();
    }
    if is_numeric && spec.zero_pad && spec.align.is_none() {
        let mut out = String::with_capacity(width);
        out.push_str(prefix);
        out.extend(std::iter::repeat_n('0', pad_len));
        out.push_str(&core);
        return out.into_bytes();
    }
    let align = spec.align.unwrap_or(if is_numeric {
        Align::Right
    } else {
        Align::Left
    });
    match align {
        Align::Left => {
            let mut out = body;
            out.extend(std::iter::repeat_n(spec.fill, pad_len));
            out.into_bytes()
        }
        Align::Right => {
            let mut out: String = std::iter::repeat_n(spec.fill, pad_len).collect();
            out.push_str(&body);
            out.into_bytes()
        }
        Align::Center => {
            let left = pad_len / 2;
            let right = pad_len - left;
            let mut out: String = std::iter::repeat_n(spec.fill, left).collect();
            out.push_str(&body);
            out.extend(std::iter::repeat_n(spec.fill, right));
            out.into_bytes()
        }
    }
}

fn array_init_elems(init: &Expr, len: u64) -> Vec<&Expr> {
    match init {
        Expr::ArrayLit(elems) => {
            if elems.len() as u64 != len {
                panic!("effects::rust_ast: array initializer length does not match type");
            }
            elems.iter().collect()
        }
        Expr::ArrayRepeat {
            elem,
            len: repeat_len,
        } => {
            if *repeat_len as u64 != len {
                panic!("effects::rust_ast: array repeat length does not match type");
            }
            std::iter::repeat(elem.as_ref()).take(*repeat_len).collect()
        }
        other => panic!("effects::rust_ast: unsupported array initializer `{other:?}`"),
    }
}

fn local_value_size(value: &Value) -> u64 {
    match value {
        Value::Int { .. } => int_byte_size(value),
        Value::Bool(_) => 1,
        Value::Float(_) => 8,
        Value::Ref(_) | Value::Null | Value::File(_) | Value::Atomic(_) => 8,
        Value::AtomicResult { .. }
        | Value::Tuple(_)
        | Value::BlockLabel(_)
        | Value::Option(_)
        | Value::Bytes(_) => {
            panic!("effects::rust_ast: cannot take address of transient value {value:?}")
        }
    }
}

fn compare_bytes(left: &[u8], right: &[u8]) -> i8 {
    for (a, b) in left.iter().cloned().zip(right.iter().cloned()) {
        if a != b {
            return if a < b { -1 } else { 1 };
        }
    }
    match left.len().cmp(&right.len()) {
        std::cmp::Ordering::Less => -1,
        std::cmp::Ordering::Equal => 0,
        std::cmp::Ordering::Greater => 1,
    }
}

fn parse_i128_prefix(bytes: &[u8]) -> i128 {
    let text = std::str::from_utf8(bytes).unwrap_or("");
    let prefix_len = decimal_prefix_len(bytes);
    if prefix_len == 0 {
        0
    } else {
        text[..prefix_len].parse::<i128>().unwrap_or_else(|_| {
            if text[..prefix_len].starts_with('-') {
                i128::MIN
            } else {
                i128::MAX
            }
        })
    }
}

fn parse_u128_prefix(bytes: &[u8]) -> u128 {
    let text = std::str::from_utf8(bytes).unwrap_or("");
    let prefix_len = decimal_prefix_len(bytes);
    if prefix_len == 0 {
        return 0;
    }
    let prefix = &text[..prefix_len];
    let (negative, digits) = match prefix.strip_prefix('-') {
        Some(rest) => (true, rest),
        None => (false, prefix.strip_prefix('+').unwrap_or(prefix)),
    };
    let value = digits.parse::<u128>().unwrap_or(u128::MAX);
    if negative {
        value.wrapping_neg()
    } else {
        value
    }
}

fn parse_f64_prefix(bytes: &[u8]) -> f64 {
    let text = std::str::from_utf8(bytes).unwrap_or("");
    let prefix_len = float_prefix_len(bytes);
    if prefix_len == 0 {
        0.0
    } else {
        text[..prefix_len].parse::<f64>().unwrap_or(0.0)
    }
}

fn decimal_prefix_len(bytes: &[u8]) -> usize {
    let mut index = bytes
        .iter()
        .position(|byte| !byte.is_ascii_whitespace())
        .unwrap_or(bytes.len());
    if matches!(bytes.get(index), Some(b'+' | b'-')) {
        index += 1;
    }
    let digits_start = index;
    while bytes.get(index).is_some_and(u8::is_ascii_digit) {
        index += 1;
    }
    if index == digits_start { 0 } else { index }
}

fn float_prefix_len(bytes: &[u8]) -> usize {
    let mut index = bytes
        .iter()
        .position(|byte| !byte.is_ascii_whitespace())
        .unwrap_or(bytes.len());
    if matches!(bytes.get(index), Some(b'+' | b'-')) {
        index += 1;
    }
    let mut digits = 0usize;
    while bytes.get(index).is_some_and(u8::is_ascii_digit) {
        index += 1;
        digits += 1;
    }
    if bytes.get(index) == Some(&b'.') {
        index += 1;
        while bytes.get(index).is_some_and(u8::is_ascii_digit) {
            index += 1;
            digits += 1;
        }
    }
    if digits == 0 {
        return 0;
    }
    if matches!(bytes.get(index), Some(b'e' | b'E')) {
        let exp = index;
        index += 1;
        if matches!(bytes.get(index), Some(b'+' | b'-')) {
            index += 1;
        }
        let exp_digits = index;
        while bytes.get(index).is_some_and(u8::is_ascii_digit) {
            index += 1;
        }
        if index == exp_digits {
            index = exp;
        }
    }
    index
}

fn int_width_bits(width: IntWidth) -> Option<u32> {
    Some(match width {
        IntWidth::W8 => 8,
        IntWidth::W16 => 16,
        IntWidth::W32 => 32,
        IntWidth::W64 => 64,
        IntWidth::W128 => 128,
        IntWidth::PointerSized => 64,
    })
}

fn truncate_to_bits(value: i128, bits: u32, signed: bool) -> i128 {
    if bits == 0 || bits >= 128 {
        return value;
    }
    let mask = (1i128 << bits) - 1;
    let truncated = value & mask;
    if signed && (truncated & (1i128 << (bits - 1))) != 0 {
        truncated - (1i128 << bits)
    } else {
        truncated
    }
}

fn align_to(value: u64, align: u64) -> u64 {
    if align <= 1 {
        value
    } else {
        value.div_ceil(align) * align
    }
}

fn bit_mask(bits: u32) -> u128 {
    if bits >= 128 {
        u128::MAX
    } else {
        (1u128 << bits) - 1
    }
}

fn reverse_bits(value: u128, bits: u32) -> i128 {
    (value.reverse_bits() >> (128 - bits)) as i128
}

fn swap_bytes(value: u128, bits: u32) -> i128 {
    let bytes = bits / 8;
    let mut out = 0u128;
    for index in 0..bytes {
        let byte = (value >> (index * 8)) & 0xff;
        out |= byte << ((bytes - index - 1) * 8);
    }
    out as i128
}

fn leading_zeros(value: u128, bits: u32) -> u32 {
    (value & bit_mask(bits)).leading_zeros() - (128 - bits)
}

fn trailing_zeros(value: u128, bits: u32) -> u32 {
    let value = value & bit_mask(bits);
    if value == 0 {
        bits
    } else {
        value.trailing_zeros()
    }
}

fn count_ones(value: u128, bits: u32) -> u32 {
    (value & bit_mask(bits)).count_ones()
}

fn overflowing_int_op(lhs: i128, rhs: i128, bits: u32, signed: bool, method: &str) -> (i128, bool) {
    if signed {
        if matches!(method, "overflowing_div" | "overflowing_rem") && rhs == 0 {
            return (0, true);
        }
        let raw = match method {
            "overflowing_add" => lhs.wrapping_add(rhs),
            "overflowing_sub" => lhs.wrapping_sub(rhs),
            "overflowing_mul" => lhs.wrapping_mul(rhs),
            "overflowing_div" => lhs.wrapping_div(rhs),
            "overflowing_rem" => lhs.wrapping_rem(rhs),
            _ => unreachable!(),
        };
        let value = truncate_to_bits(raw, bits, true);
        let min = -(1i128 << (bits - 1));
        let max = (1i128 << (bits - 1)) - 1;
        let overflowed = match method {
            "overflowing_add" => lhs.checked_add(rhs).is_none_or(|v| v < min || v > max),
            "overflowing_sub" => lhs.checked_sub(rhs).is_none_or(|v| v < min || v > max),
            "overflowing_mul" => lhs.checked_mul(rhs).is_none_or(|v| v < min || v > max),
            "overflowing_div" => rhs == 0 || (lhs == min && rhs == -1),
            "overflowing_rem" => rhs == 0 || (lhs == min && rhs == -1),
            _ => unreachable!(),
        };
        return (value, overflowed);
    }

    let mask = bit_mask(bits);
    let lhs = lhs as u128 & mask;
    let rhs = rhs as u128 & mask;
    let (raw, overflowed) = match method {
        "overflowing_add" => lhs.overflowing_add(rhs),
        "overflowing_sub" => lhs.overflowing_sub(rhs),
        "overflowing_mul" => lhs.overflowing_mul(rhs),
        "overflowing_div" if rhs == 0 => (0, true),
        "overflowing_div" => (lhs / rhs, false),
        "overflowing_rem" if rhs == 0 => (0, true),
        "overflowing_rem" => (lhs % rhs, false),
        _ => unreachable!(),
    };
    (
        truncate_to_bits((raw & mask) as i128, bits, false),
        overflowed || raw > mask,
    )
}

fn parse_hex_float(text: &str) -> f64 {
    let Some((mantissa, exponent)) = text
        .trim()
        .strip_prefix("0x")
        .or_else(|| text.trim().strip_prefix("0X"))
        .and_then(|rest| rest.split_once(['p', 'P']))
    else {
        return text.parse::<f64>().unwrap_or(0.0);
    };
    let exponent = exponent.parse::<i32>().unwrap_or(0);
    let (whole, frac) = mantissa.split_once('.').unwrap_or((mantissa, ""));
    let whole_value =
        u128::from_str_radix(if whole.is_empty() { "0" } else { whole }, 16).unwrap_or(0) as f64;
    let frac_value = frac
        .bytes()
        .enumerate()
        .filter_map(|(index, byte)| char::from(byte).to_digit(16).map(|digit| (index, digit)))
        .map(|(index, digit)| digit as f64 / 16f64.powi(index as i32 + 1))
        .sum::<f64>();
    (whole_value + frac_value) * 2f64.powi(exponent)
}

#[cfg(test)]
mod tests;
