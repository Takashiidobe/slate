use super::*;

pub(super) const COMPLEX_TY: &str = "num_complex::Complex";

fn fenv_param(name: &str, ty: Type) -> FnParam {
    FnParam {
        name: shim_param(name),
        mutable: false,
        ty,
    }
}

fn fenv_extern_decl(name: String, params: Vec<FnParam>, ret: Type) -> ExternFnDecl {
    ExternFnDecl {
        attrs: Vec::new(),
        identity: FunctionIdentity::Unknown,
        name,
        declared_type: None,
        trusted_headers: std::collections::BTreeSet::new(),
        params,
        variadic: false,
        ret: Some(ret),
        safe: true,
    }
}

pub(super) fn fenv_shim_decls() -> Vec<ExternFnDecl> {
    let float_ty = |bits: u32| {
        if bits == 32 {
            Type::Prim(Prim::F32)
        } else {
            Type::Prim(Prim::F64)
        }
    };
    let mut decls = Vec::new();
    for bits in [32u32, 64] {
        let f = float_ty(bits);
        for op in ["add", "sub", "mul", "div", "rem"] {
            decls.push(fenv_extern_decl(
                format!("__slate_fenv_{op}_f{bits}"),
                vec![fenv_param("a", f.clone()), fenv_param("b", f.clone())],
                f.clone(),
            ));
        }
        for cmp in ["lt", "le", "gt", "ge", "eq", "ne"] {
            decls.push(fenv_extern_decl(
                format!("__slate_fenv_{cmp}_f{bits}"),
                vec![fenv_param("a", f.clone()), fenv_param("b", f.clone())],
                Type::Prim(Prim::Bool),
            ));
        }
        for unary in [
            "sin",
            "cos",
            "exp",
            "exp2",
            "log",
            "log2",
            "log10",
            "ceil",
            "floor",
            "round",
            "rint",
            "nearbyint",
            "roundeven",
            "trunc",
            "sqrt",
            "fabs",
        ] {
            decls.push(fenv_extern_decl(
                format!("__slate_fenv_{unary}_f{bits}"),
                vec![fenv_param("a", f.clone())],
                f.clone(),
            ));
        }
        for binary in ["pow", "fmax", "fmin", "copysign"] {
            decls.push(fenv_extern_decl(
                format!("__slate_fenv_{binary}_f{bits}"),
                vec![fenv_param("a", f.clone()), fenv_param("b", f.clone())],
                f.clone(),
            ));
        }
        decls.push(fenv_extern_decl(
            format!("__slate_fenv_fma_f{bits}"),
            vec![
                fenv_param("a", f.clone()),
                fenv_param("b", f.clone()),
                fenv_param("c", f.clone()),
            ],
            f.clone(),
        ));
        decls.push(fenv_extern_decl(
            format!("__slate_fenv_i64_to_f{bits}"),
            vec![fenv_param("a", Type::Prim(Prim::I64))],
            f.clone(),
        ));
        decls.push(fenv_extern_decl(
            format!("__slate_fenv_u64_to_f{bits}"),
            vec![fenv_param("a", Type::Prim(Prim::U64))],
            f.clone(),
        ));
        decls.push(fenv_extern_decl(
            format!("__slate_fenv_f{bits}_to_i64"),
            vec![fenv_param("a", f.clone())],
            Type::Prim(Prim::I64),
        ));
        decls.push(fenv_extern_decl(
            format!("__slate_fenv_f{bits}_to_u64"),
            vec![fenv_param("a", f.clone())],
            Type::Prim(Prim::U64),
        ));
        decls.push(fenv_extern_decl(
            format!("__slate_fenv_f{bits}_to_bool"),
            vec![fenv_param("a", f.clone())],
            Type::Prim(Prim::Bool),
        ));
    }
    decls.push(fenv_extern_decl(
        "__slate_fenv_f32_to_f64".into(),
        vec![fenv_param("a", Type::Prim(Prim::F32))],
        Type::Prim(Prim::F64),
    ));
    decls.push(fenv_extern_decl(
        "__slate_fenv_f64_to_f32".into(),
        vec![fenv_param("a", Type::Prim(Prim::F64))],
        Type::Prim(Prim::F32),
    ));
    decls
}

pub(super) fn is_long_double(ty: &CirType) -> bool {
    matches!(ty, CirType::LongDouble { .. } | CirType::Fp80)
}

pub(super) fn is_quad_long_double(ty: &CirType) -> bool {
    matches!(ty, CirType::LongDouble { underlying } if matches!(underlying.as_ref(), CirType::Fp128))
}

pub(super) fn is_wrapped_long_double(ty: &CirType) -> bool {
    is_long_double(ty)
        && !is_quad_long_double(ty)
        && crate::frontend::toolchain::active_long_double_bits() != 64
}

pub(super) fn is_complex_runtime_call(name: &str) -> bool {
    matches!(
        name,
        "__muldc3" | "__divdc3" | "__mulsc3" | "__divsc3" | "__multc3" | "__divtc3"
    )
}

pub(super) fn complex_ty(inner: Type) -> Type {
    Type::Complex(Box::new(inner))
}

pub(super) fn complex_runtime_decl(name: &str, prim: Prim) -> ExternDecl {
    let param = |n: &str| FnParam {
        name: shim_param(n),
        mutable: false,
        ty: Type::Prim(prim),
    };
    ExternDecl::Fn(ExternFnDecl {
        attrs: Vec::new(),
        identity: crate::function_identity::FunctionIdentity::Unknown,
        name: name.into(),
        declared_type: None,
        trusted_headers: std::collections::BTreeSet::new(),
        params: vec![param("a"), param("b"), param("c"), param("d")],
        variadic: false,
        ret: Some(complex_ty(Type::Prim(prim))),
        safe: false,
    })
}

// C `_Complex` is `num_complex::Complex`; the extern runtime routines keep
// float/double `*`/`/` bit-identical to clang's libgcc lowering.
pub(super) fn complex_prelude(uses_f128: bool) -> Vec<Item> {
    let mut decls = vec![
        complex_runtime_decl("__muldc3", Prim::F64),
        complex_runtime_decl("__divdc3", Prim::F64),
        complex_runtime_decl("__mulsc3", Prim::F32),
        complex_runtime_decl("__divsc3", Prim::F32),
    ];
    if uses_f128 {
        decls.push(complex_runtime_decl("__multc3", Prim::F128));
        decls.push(complex_runtime_decl("__divtc3", Prim::F128));
    }
    vec![Item::ExternBlock {
        abi: "C".into(),
        decls,
    }]
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
    let var = |name: &str| Expr::Var(shim_param(name).into());
    let local = |name: &str| Expr::Var(name.into());
    let byte_at = || Expr::MethodCall {
        recv: Box::new(local("bytes")),
        method: "add".into(),
        args: vec![local("i")],
    };

    let hit = Stmt::If {
        cond: Expr::Binary {
            op: BinOp::Eq,
            lhs: Box::new(FunctionLowerer::unsafe_expr(Expr::Unary {
                op: UnaryOp::Deref,
                expr: Box::new(byte_at()),
            })),
            rhs: Box::new(local("b")),
        },
        then_body: vec![Stmt::Return(Some(Expr::Cast {
            expr: Box::new(FunctionLowerer::unsafe_expr(byte_at())),
            ty: void_ptr(true),
        }))],
        else_body: Vec::new(),
    };
    let step = Stmt::CompoundAssign {
        target: local("i"),
        op: BinOp::Add,
        value: Expr::Value(RustValue::I64(1)),
    };
    let scan = Stmt::While {
        label: None,
        cond: Expr::Binary {
            op: BinOp::Lt,
            lhs: Box::new(local("i")),
            rhs: Box::new(var("n")),
        },
        body: crate::backend::rust_ast::Block {
            stmts: vec![hit, step],
            tail: None,
        },
    };

    let body = vec![
        Stmt::Let {
            name: "b".into(),
            mutable: false,
            ty: Some(Type::Prim(Prim::U8)),
            init: Some(Expr::Cast {
                expr: Box::new(var("c")),
                ty: Type::Prim(Prim::U8),
            }),
        },
        Stmt::Let {
            name: "bytes".into(),
            mutable: false,
            ty: Some(u8_const_ptr.clone()),
            init: Some(Expr::Cast {
                expr: Box::new(var("s")),
                ty: u8_const_ptr,
            }),
        },
        Stmt::Let {
            name: "i".into(),
            mutable: true,
            ty: Some(Type::Prim(Prim::Usize)),
            init: Some(Expr::Value(RustValue::I64(0))),
        },
        scan,
        Stmt::Return(Some(Expr::Value(RustValue::NullPtr))),
    ];

    Item::Fn(FnDef {
        attrs: Vec::new(),
        vis: Visibility::Private,
        unsafe_: false,
        abi: None,
        name: "__slate_memchr".into(),
        params: vec![
            FnParam {
                name: shim_param("s"),
                mutable: false,
                ty: void_ptr(false),
            },
            FnParam {
                name: shim_param("c"),
                mutable: false,
                ty: Type::Prim(Prim::I32),
            },
            FnParam {
                name: shim_param("n"),
                mutable: false,
                ty: Type::Prim(Prim::Usize),
            },
        ],
        ret: Some(void_ptr(true)),
        body,
    })
}
