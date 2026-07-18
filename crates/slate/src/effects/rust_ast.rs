use std::collections::HashMap;

use super::{AllocId, Effect, EffectTrace, IntWidth, Location, Value};
use crate::rust_ast::{BinOp, Expr, FnDef, IndentStmt, Path, RustValue, Stmt, Type, UnaryOp};

pub fn interpret(f: &FnDef) -> EffectTrace {
    let mut interp = Interp::default();
    let _ = interp.run(&f.body);
    interp.trace
}

/// How a statement completed: either it ran through normally, or it hit a
/// `return`/`std::process::exit` that the enclosing `if`/`for` must
/// propagate past without running the rest of their body.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum Flow {
    Normal,
    Return,
}

struct VecBinding {
    alloc: AllocId,
    elem_width: IntWidth,
    elem_signed: bool,
    elem_size: u64,
    len: u64,
}

#[derive(Default)]
struct Interp {
    vecs: HashMap<String, VecBinding>,
    scalars: HashMap<String, Value>,
    heap: HashMap<Location, Value>,
    next_alloc: u32,
    trace: EffectTrace,
}

impl Interp {
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
                if vec_elem_shape(ty).is_some() {
                    self.let_vec(name, ty, init);
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
                self.trace.push(Effect::Exit(code));
                Flow::Return
            }
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
                self.trace.push(Effect::Exit(code));
                Flow::Return
            }
            other => panic!("effects::rust_ast: unsupported stmt `{other:?}`"),
        }
    }

    fn assign(&mut self, target: &Expr, value: &Expr) -> Flow {
        match target {
            Expr::Var(ident) => {
                let v = self.eval(value);
                self.scalars.insert(ident.as_str().to_string(), v);
            }
            Expr::Index { base, index } => self.assign_index(base, index, value),
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
                Flow::Normal => {}
                Flow::Return => return Flow::Return,
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
            },
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

    fn assign_index(&mut self, base: &Expr, index: &Expr, value: &Expr) {
        let name = match base {
            Expr::Var(ident) => ident.as_str(),
            other => panic!("effects::rust_ast: unsupported assign target base `{other:?}`"),
        };
        let idx = value_as_u64(self.eval(index));
        let raw = value_as_i128(self.eval(value));
        let binding = self
            .vecs
            .get(name)
            .unwrap_or_else(|| panic!("effects::rust_ast: assign into unknown Vec `{name}`"));
        let value = Value::Int {
            width: binding.elem_width,
            signed: binding.elem_signed,
            value: raw,
        };
        let loc = Location {
            alloc: binding.alloc,
            byte_offset: idx * binding.elem_size,
        };
        self.heap.insert(loc, value);
        self.trace.push(Effect::Write { loc, value });
    }

    fn eval(&mut self, expr: &Expr) -> Value {
        match expr {
            Expr::Value(rv) => rust_value_to_value(rv),
            Expr::Var(ident) => *self.scalars.get(ident.as_str()).unwrap_or_else(|| {
                panic!(
                    "effects::rust_ast: read of unknown scalar `{}`",
                    ident.as_str()
                )
            }),
            // ignores the cast's target type, mirroring `cir.cast`'s own pass-through behavior.
            Expr::Cast { expr, .. } => self.eval(expr),
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
            other => panic!("effects::rust_ast: unsupported expr `{other:?}`"),
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
        _ => false,
    }
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
            let (a, b) = (value_as_i128(a), value_as_i128(b));
            Value::Bool(match op {
                BinOp::Eq => a == b,
                BinOp::Ne => a != b,
                BinOp::Lt => a < b,
                BinOp::Le => a <= b,
                BinOp::Gt => a > b,
                BinOp::Ge => a >= b,
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
    use crate::rust_ast::{Ident, IndentStmt, Prim, Visibility};

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
                Effect::Exit(3),
            ]
        );
    }
}
