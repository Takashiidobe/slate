use super::*;

pub(super) const LONG_DOUBLE_TY: &str = "LongDouble";

pub(super) fn long_double_zero_expr() -> Expr {
    Expr::TupleStructLit {
        name: LONG_DOUBLE_TY.into(),
        fields: vec![Expr::ArrayRepeat {
            elem: Box::new(Expr::Value(RustValue::I64(0))),
            len: 10,
        }],
    }
}

// x86-64 SysV long double is the 10-byte x87 payload in a 16-byte slot.
pub(super) fn long_double_prelude(vis: Visibility) -> Vec<Item> {
    vec![Item::Struct(StructDef {
        attrs: vec![
            RustAttr::Repr(vec![Repr::C, Repr::Align(16)]),
            RustAttr::Derive(vec![Derive::Clone, Derive::Copy]),
        ],
        vis,
        field_vis: vis,
        generics: vec![],
        name: LONG_DOUBLE_TY.into(),
        fields: StructFields::Tuple(vec![Type::Array {
            elem: Box::new(Type::Prim(Prim::U8)),
            len: 10,
        }]),
    })]
}

pub(super) fn is_long_double(ty: &str) -> bool {
    ty.starts_with("!cir.long_double")
}

pub(super) fn is_quad_long_double(ty: &str) -> bool {
    ty.starts_with("!cir.long_double<!cir.f128>")
}

pub(super) fn is_wrapped_long_double(ty: &str) -> bool {
    is_long_double(ty) && !is_quad_long_double(ty) && !crate::cir::emit::uses_f64_long_double_abi()
}

pub(super) fn is_format_string_arg(ty: &str) -> bool {
    matches!(ty, "!cir.ptr<!s8i>" | "!cir.ptr<!u8i>")
}

pub(super) fn long_double_shim_type_tag(ty: &Type) -> String {
    match ty {
        Type::Prim(Prim::I8) => "i8".into(),
        Type::Prim(Prim::U8) => "u8".into(),
        Type::Prim(Prim::I16) => "i16".into(),
        Type::Prim(Prim::U16) => "u16".into(),
        Type::Prim(Prim::I32) => "i32".into(),
        Type::Prim(Prim::U32) => "u32".into(),
        Type::Prim(Prim::I64) => "i64".into(),
        Type::Prim(Prim::U64) => "u64".into(),
        Type::Prim(Prim::I128) => "i128".into(),
        Type::Prim(Prim::U128) => "u128".into(),
        Type::Prim(Prim::Isize) => "isize".into(),
        Type::Prim(Prim::Usize) => "usize".into(),
        Type::Prim(Prim::F32) => "f32".into(),
        Type::Prim(Prim::F64) => "f64".into(),
        Type::Prim(Prim::Bool) => "bool".into(),
        Type::Ptr { inner, .. } => format!("p{}", long_double_shim_type_tag(inner)),
        _ => "x".into(),
    }
}

// clang lowers complex `*`/`/` to the libgcc runtime (__mul?c3/__div?c3), reached
// directly for `/` and via a NaN-recovery branch for `*`. We call the same symbols
// so results are bit-identical; #[repr(C)] {re, im} matches the return ABI.
pub(super) fn is_complex_runtime_call(name: &str) -> bool {
    matches!(name, "__muldc3" | "__divdc3" | "__mulsc3" | "__divsc3")
}

pub(super) fn complex_ty(inner: Type) -> Type {
    Type::Complex(Box::new(inner))
}

pub(super) fn cir_complex_inner(ty: &str) -> Option<&str> {
    ty.strip_prefix("!cir.complex<")?.strip_suffix('>')
}

pub(super) fn complex_binop_impl(trait_: StdTrait, op: BinOp) -> Item {
    let field = |base: &str, field: &str| Expr::Field {
        base: Box::new(Expr::Var(base.into())),
        field: field.into(),
    };
    let component = |name: &str| {
        (
            name.to_string(),
            Expr::Binary {
                op,
                lhs: Box::new(field("self", name)),
                rhs: Box::new(field("o", name)),
            },
        )
    };
    let method = Method {
        name: trait_.method().into(),
        self_kind: SelfKind::Value,
        params: vec![FnParam {
            name: "o".into(),
            mutable: false,
            ty: complex_ty(Type::TyVar("T".into())),
        }],
        ret: Some(complex_ty(Type::TyVar("T".into()))),
        body: Expr::StructLit {
            name: "Complex".into(),
            fields: vec![component("re"), component("im")],
        },
    };
    Item::Impl(ImplBlock {
        generics: vec![GenericParam {
            name: "T".into(),
            bounds: vec![TraitBound {
                trait_,
                assoc: vec![("Output".into(), Type::TyVar("T".into()))],
            }],
        }],
        trait_: Some(TraitRef::Std(trait_)),
        self_ty: complex_ty(Type::TyVar("T".into())),
        items: vec![
            ImplItem::AssocType {
                name: "Output".into(),
                ty: complex_ty(Type::TyVar("T".into())),
            },
            ImplItem::Method(method),
        ],
    })
}

pub(super) fn complex_runtime_decl(name: &str, prim: Prim) -> ExternDecl {
    let param = |n: &str| FnParam {
        name: n.into(),
        mutable: false,
        ty: Type::Prim(prim),
    };
    ExternDecl::Fn(ExternFnDecl {
        identity: crate::function_identity::FunctionIdentity::Unknown,
        name: name.into(),
        params: vec![param("a"), param("b"), param("c"), param("d")],
        variadic: false,
        ret: Some(complex_ty(Type::Prim(prim))),
        safe: false,
    })
}

// C `_Complex` has no native Rust type; a #[repr(C)] pair matches its two-scalar
// layout, and the extern runtime routines back `*`/`/`.
pub(super) fn complex_prelude() -> Vec<Item> {
    vec![
        Item::Struct(StructDef {
            attrs: vec![
                RustAttr::Repr(vec![Repr::C]),
                RustAttr::Derive(vec![Derive::Clone, Derive::Copy, Derive::PartialEq]),
            ],
            vis: Visibility::Private,
            field_vis: Visibility::Private,
            generics: vec![GenericParam {
                name: "T".into(),
                bounds: vec![],
            }],
            name: "Complex".into(),
            fields: StructFields::Named(vec![
                crate::backend::rust_ast::StructField {
                    attrs: Vec::new(),
                    name: "re".into(),
                    ty: Type::TyVar("T".into()),
                },
                crate::backend::rust_ast::StructField {
                    attrs: Vec::new(),
                    name: "im".into(),
                    ty: Type::TyVar("T".into()),
                },
            ]),
        }),
        complex_binop_impl(StdTrait::Add, BinOp::Add),
        complex_binop_impl(StdTrait::Sub, BinOp::Sub),
        Item::ExternBlock {
            abi: "C".into(),
            decls: vec![
                complex_runtime_decl("__muldc3", Prim::F64),
                complex_runtime_decl("__divdc3", Prim::F64),
                complex_runtime_decl("__mulsc3", Prim::F32),
                complex_runtime_decl("__divsc3", Prim::F32),
            ],
        },
    ]
}

// C `memchr` has no direct std equivalent; this byte scan matches its
// `(unsigned char)c` comparison and returns a raw pointer to the first hit.
pub(super) fn memchr_prelude() -> Item {
    let void_ptr = |mutable| Type::Ptr {
        mutable,
        inner: Box::new(Type::CLib(CLibType::VOID)),
    };
    let u8_const_ptr = Type::Ptr {
        mutable: false,
        inner: Box::new(Type::Prim(Prim::U8)),
    };
    let var = |name: &str| Expr::Var(name.into());
    let byte_at = || Expr::MethodCall {
        recv: Box::new(var("bytes")),
        method: "add".into(),
        args: vec![var("i")],
    };

    let hit = Stmt::If {
        cond: Expr::Binary {
            op: BinOp::Eq,
            lhs: Box::new(FunctionLowerer::unsafe_expr(Expr::Unary {
                op: UnaryOp::Deref,
                expr: Box::new(byte_at()),
            })),
            rhs: Box::new(var("b")),
        },
        then_body: vec![IndentStmt {
            depth: 0,
            stmt: Stmt::Return(Some(Expr::Cast {
                expr: Box::new(FunctionLowerer::unsafe_expr(byte_at())),
                ty: void_ptr(true),
            })),
        }],
        else_body: Vec::new(),
    };
    let step = Stmt::CompoundAssign {
        target: var("i"),
        op: BinOp::Add,
        value: Expr::Value(RustValue::I64(1)),
    };
    let scan = Stmt::While {
        cond: Expr::Binary {
            op: BinOp::Lt,
            lhs: Box::new(var("i")),
            rhs: Box::new(var("n")),
        },
        body: crate::backend::rust_ast::Block {
            stmts: vec![
                IndentStmt {
                    depth: 0,
                    stmt: hit,
                },
                IndentStmt {
                    depth: 0,
                    stmt: step,
                },
            ],
            tail: None,
        },
    };

    let body = vec![
        IndentStmt {
            depth: 1,
            stmt: Stmt::Let {
                name: "b".into(),
                mutable: false,
                ty: Some(Type::Prim(Prim::U8)),
                init: Some(Expr::Cast {
                    expr: Box::new(var("c")),
                    ty: Type::Prim(Prim::U8),
                }),
            },
        },
        IndentStmt {
            depth: 1,
            stmt: Stmt::Let {
                name: "bytes".into(),
                mutable: false,
                ty: Some(u8_const_ptr.clone()),
                init: Some(Expr::Cast {
                    expr: Box::new(var("s")),
                    ty: u8_const_ptr,
                }),
            },
        },
        IndentStmt {
            depth: 1,
            stmt: Stmt::Let {
                name: "i".into(),
                mutable: true,
                ty: Some(Type::Prim(Prim::Usize)),
                init: Some(Expr::Value(RustValue::I64(0))),
            },
        },
        IndentStmt {
            depth: 1,
            stmt: scan,
        },
        IndentStmt {
            depth: 1,
            stmt: Stmt::Return(Some(Expr::Value(RustValue::NullPtr))),
        },
    ];

    Item::Fn(FnDef {
        attrs: Vec::new(),
        vis: Visibility::Private,
        unsafe_: false,
        abi: None,
        name: "__slate_memchr".into(),
        params: vec![
            FnParam {
                name: "s".into(),
                mutable: false,
                ty: void_ptr(false),
            },
            FnParam {
                name: "c".into(),
                mutable: false,
                ty: Type::Prim(Prim::I32),
            },
            FnParam {
                name: "n".into(),
                mutable: false,
                ty: Type::Prim(Prim::Usize),
            },
        ],
        ret: Some(void_ptr(true)),
        body,
    })
}
