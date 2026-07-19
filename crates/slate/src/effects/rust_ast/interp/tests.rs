use super::*;
use crate::rust_ast::{FnParam, Ident, IndentStmt, Prim, Visibility};
use crate::rust_ast::{Path, RustValue};

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

/// The expected effect sequence for `int *p = malloc(2 * sizeof(int));
/// p[0] = 1; p[1] = 2; return p[0] + p[1];`, proving the shared `Effect`
/// enum represents a raw-pointer walk and its idiomatized `Vec` counterpart
/// without a special case.
#[test]
fn matches_raw_pointer_trace_shape() {
    let trace = interpret(&idiomatized_fixture());
    let alloc = AllocId(0);
    let raw_pointer_trace = EffectTrace {
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
    assert_eq!(trace, raw_pointer_trace);
}

#[test]
fn pointer_add_uses_pointee_size_for_deref() {
    let f = FnDef {
        attrs: vec![],
        vis: Visibility::Private,
        unsafe_: false,
        abi: None,
        name: "main".to_string(),
        params: vec![],
        ret: Some(Type::Prim(Prim::I32)),
        body: vec![
            stmt(Stmt::Let {
                name: "values".to_string(),
                mutable: true,
                ty: Some(Type::Array {
                    elem: Box::new(Type::Prim(Prim::I32)),
                    len: 2,
                }),
                init: Some(Expr::ArrayLit(vec![
                    Expr::Value(RustValue::I64(10)),
                    Expr::Value(RustValue::I64(20)),
                ])),
            }),
            stmt(Stmt::Let {
                name: "p".to_string(),
                mutable: false,
                ty: Some(Type::Ptr {
                    mutable: true,
                    inner: Box::new(Type::Prim(Prim::I32)),
                }),
                init: Some(Expr::MethodCall {
                    recv: Box::new(Expr::Var(Ident::new("values"))),
                    method: "as_mut_ptr".to_string(),
                    args: vec![],
                }),
            }),
            stmt(Stmt::Return(Some(Expr::Unary {
                op: UnaryOp::Deref,
                expr: Box::new(Expr::MethodCall {
                    recv: Box::new(Expr::Var(Ident::new("p"))),
                    method: "add".to_string(),
                    args: vec![Expr::Value(RustValue::I64(1))],
                }),
            }))),
        ],
    };

    let trace = interpret(&f);
    assert!(trace.effects.contains(&Effect::Read {
        loc: Location {
            alloc: AllocId(0),
            byte_offset: 4
        },
        value: int32(20),
    }));
    assert_eq!(trace.effects.last(), Some(&Effect::Exit(20)));
}

#[test]
fn addressed_scalar_reads_back_deref_assignment() {
    let f = FnDef {
        attrs: vec![],
        vis: Visibility::Private,
        unsafe_: false,
        abi: None,
        name: "main".to_string(),
        params: vec![],
        ret: Some(Type::Prim(Prim::I32)),
        body: vec![
            stmt(Stmt::Let {
                name: "local".to_string(),
                mutable: true,
                ty: Some(Type::Prim(Prim::I32)),
                init: Some(Expr::Value(RustValue::I64(1))),
            }),
            stmt(Stmt::Let {
                name: "p".to_string(),
                mutable: false,
                ty: Some(Type::Ptr {
                    mutable: true,
                    inner: Box::new(Type::Prim(Prim::I32)),
                }),
                init: Some(Expr::Macro {
                    name: "std::ptr::addr_of_mut".to_string(),
                    args: vec![Expr::Var(Ident::new("local"))],
                }),
            }),
            stmt(Stmt::Assign {
                target: Expr::Unary {
                    op: UnaryOp::Deref,
                    expr: Box::new(Expr::Var(Ident::new("p"))),
                },
                value: Expr::Value(RustValue::I64(9)),
            }),
            stmt(Stmt::Return(Some(Expr::Var(Ident::new("local"))))),
        ],
    };

    let trace = interpret(&f);
    assert_eq!(trace.effects.last(), Some(&Effect::Exit(9)));
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
fn function_parameters_seed_the_trace_and_match_raw_pointer_shape() {
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
fn struct_field_literal_matches_raw_aggregate_shape() {
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
fn vec_repeat_indexed_assign_matches_raw_collection_shape() {
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
