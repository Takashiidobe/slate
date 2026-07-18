use std::collections::HashMap;

use super::{AllocId, Effect, EffectTrace, IntWidth, Location, Value};
use crate::rust_ast::{BinOp, Expr, FnDef, Path, RustValue, Stmt, Type};

pub fn interpret(f: &FnDef) -> EffectTrace {
    let mut interp = Interp::default();
    interp.run(&f.body);
    interp.trace
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
    fn run(&mut self, body: &[crate::rust_ast::IndentStmt]) {
        for stmt in body {
            self.step(&stmt.stmt);
        }
    }

    fn step(&mut self, stmt: &Stmt) {
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
            }
            Stmt::Expr(Expr::MethodCall { recv, method, args }) if method == "push" => {
                self.push(recv, args)
            }
            Stmt::Expr(Expr::Call { func, args }) if is_path(func, &["std", "process", "exit"]) => {
                let code = value_as_i32(self.eval(&args[0]));
                self.trace.push(Effect::Exit(code));
            }
            Stmt::Assign { target, value } => match target {
                Expr::Index { base, index } => self.assign_index(base, index, value),
                other => panic!("effects::rust_ast: unsupported assign target `{other:?}`"),
            },
            Stmt::Return(value) => {
                let code = value
                    .as_ref()
                    .map(|expr| value_as_i32(self.eval(expr)))
                    .unwrap_or(0);
                self.trace.push(Effect::Exit(code));
            }
            other => panic!("effects::rust_ast: unsupported stmt `{other:?}`"),
        }
    }

    fn let_vec(&mut self, name: &str, ty: &Type, init: &Expr) {
        let (elem_width, elem_signed, elem_size) = vec_elem_shape(ty)
            .unwrap_or_else(|| panic!("effects::rust_ast: expected `Vec<T>` local, found {ty:?}"));
        let capacity = match init {
            Expr::Call { func, args } if is_path(func, &["Vec", "with_capacity"]) => {
                as_i128(&args[0])
            }
            Expr::VecRepeat { len, .. } => as_i128(len),
            other => panic!("effects::rust_ast: unsupported Vec initializer `{other:?}`"),
        };
        let alloc = AllocId(self.next_alloc);
        self.next_alloc += 1;
        self.trace.push(Effect::Alloc {
            alloc,
            size: capacity as u64 * elem_size,
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
        let raw = as_i128(&args[0]);
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
        let idx = as_i128(index) as u64;
        let raw = as_i128(value);
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
            Expr::Var(ident) => *self.scalars.get(ident.as_str()).unwrap_or_else(|| {
                panic!(
                    "effects::rust_ast: read of unknown scalar `{}`",
                    ident.as_str()
                )
            }),
            Expr::Binary {
                op: BinOp::Add,
                lhs,
                rhs,
            } => {
                let a = self.eval(lhs);
                let b = self.eval(rhs);
                match (a, b) {
                    (
                        Value::Int {
                            width,
                            signed,
                            value: a,
                        },
                        Value::Int { value: b, .. },
                    ) => Value::Int {
                        width,
                        signed,
                        value: a.wrapping_add(b),
                    },
                    (a, b) => panic!("effects::rust_ast: cannot add {a:?} and {b:?}"),
                }
            }
            Expr::Index { base, index } => {
                let name = match base.as_ref() {
                    Expr::Var(ident) => ident.as_str(),
                    other => panic!("effects::rust_ast: unsupported index base `{other:?}`"),
                };
                let idx = as_i128(index) as u64;
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

fn as_i128(expr: &Expr) -> i128 {
    match expr {
        Expr::Value(RustValue::I64(v)) => *v as i128,
        Expr::Value(RustValue::I128(v)) => *v,
        Expr::Value(RustValue::Usize(v)) => *v as i128,
        other => panic!("effects::rust_ast: expected an integer literal, found {other:?}"),
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
