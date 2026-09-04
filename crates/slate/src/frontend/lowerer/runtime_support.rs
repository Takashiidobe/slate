use super::*;

pub(super) const LONG_DOUBLE_TY: &str = "LongDouble";
pub(super) const COMPLEX_TY: &str = "num_complex::Complex";

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
    let mut items = vec![Item::Struct(StructDef {
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
    })];
    items.push(f80_binop_impl(StdTrait::Add, "__slate_f80_add"));
    items.push(f80_binop_impl(StdTrait::Sub, "__slate_f80_sub"));
    items.push(f80_binop_impl(StdTrait::Mul, "__slate_f80_mul"));
    items.push(f80_binop_impl(StdTrait::Div, "__slate_f80_div"));
    items.push(f80_assign_impl(StdTrait::AddAssign, "__slate_f80_add"));
    items.push(f80_assign_impl(StdTrait::SubAssign, "__slate_f80_sub"));
    items.push(f80_assign_impl(StdTrait::MulAssign, "__slate_f80_mul"));
    items.push(f80_assign_impl(StdTrait::DivAssign, "__slate_f80_div"));
    items.push(f80_neg_impl());
    items.push(f80_partial_eq_impl());
    items.push(f80_partial_ord_impl());
    items
}

fn f80_assign_impl(trait_: StdTrait, shim: &str) -> Item {
    let self_value = Expr::Unary {
        op: UnaryOp::Deref,
        expr: Box::new(Expr::Var("self".into())),
    };
    let method = Method {
        name: trait_.method().into(),
        self_kind: SelfKind::RefMut,
        params: vec![FnParam {
            name: "o".into(),
            mutable: false,
            ty: Type::LongDouble,
        }],
        ret: None,
        body: Expr::Block(Box::new(crate::backend::rust_ast::Block {
            stmts: vec![IndentStmt {
                depth: 0,
                stmt: Stmt::Assign {
                    target: self_value.clone(),
                    value: f80_call(shim, vec![self_value, Expr::Var("o".into())]),
                },
            }],
            tail: None,
        })),
    };
    Item::Impl(ImplBlock {
        generics: vec![],
        trait_: Some(TraitRef::Std(trait_)),
        self_ty: Type::LongDouble,
        items: vec![ImplItem::Method(method)],
    })
}

fn f80_binop_impl(trait_: StdTrait, shim: &str) -> Item {
    let method = Method {
        name: trait_.method().into(),
        self_kind: SelfKind::Value,
        params: vec![FnParam {
            name: "o".into(),
            mutable: false,
            ty: Type::LongDouble,
        }],
        ret: Some(Type::LongDouble),
        body: f80_call(shim, vec![Expr::Var("self".into()), Expr::Var("o".into())]),
    };
    Item::Impl(ImplBlock {
        generics: vec![],
        trait_: Some(TraitRef::Std(trait_)),
        self_ty: Type::LongDouble,
        items: vec![
            ImplItem::AssocType {
                name: "Output".into(),
                ty: Type::LongDouble,
            },
            ImplItem::Method(method),
        ],
    })
}

fn f80_neg_impl() -> Item {
    let method = Method {
        name: StdTrait::Neg.method().into(),
        self_kind: SelfKind::Value,
        params: vec![],
        ret: Some(Type::LongDouble),
        body: f80_call("__slate_f80_neg", vec![Expr::Var("self".into())]),
    };
    Item::Impl(ImplBlock {
        generics: vec![],
        trait_: Some(TraitRef::Std(StdTrait::Neg)),
        self_ty: Type::LongDouble,
        items: vec![
            ImplItem::AssocType {
                name: "Output".into(),
                ty: Type::LongDouble,
            },
            ImplItem::Method(method),
        ],
    })
}

fn f80_partial_eq_impl() -> Item {
    let method = Method {
        name: StdTrait::PartialEq.method().into(),
        self_kind: SelfKind::Ref,
        params: vec![FnParam {
            name: "other".into(),
            mutable: false,
            ty: Type::Ref {
                mutable: false,
                inner: Box::new(Type::LongDouble),
            },
        }],
        ret: Some(Type::Prim(Prim::Bool)),
        body: f80_call(
            "__slate_f80_eq",
            vec![
                Expr::Unary {
                    op: UnaryOp::Deref,
                    expr: Box::new(Expr::Var("self".into())),
                },
                Expr::Unary {
                    op: UnaryOp::Deref,
                    expr: Box::new(Expr::Var("other".into())),
                },
            ],
        ),
    };
    Item::Impl(ImplBlock {
        generics: vec![],
        trait_: Some(TraitRef::Std(StdTrait::PartialEq)),
        self_ty: Type::LongDouble,
        items: vec![ImplItem::Method(method)],
    })
}

fn f80_partial_ord_impl() -> Item {
    let ordering = || Type::Generic {
        name: "Option".into(),
        args: vec![Type::Custom("std::cmp::Ordering".into())],
    };
    let some = |ordering: &str| Expr::Call {
        binding: CallBinding::Generated,
        func: Box::new(Expr::Path(Path::new([Ident::from("Some")]))),
        args: vec![Expr::Path(Path::new(
            ["std", "cmp", "Ordering", ordering].map(Ident::from),
        ))],
    };
    let cmp = |shim: &str| {
        f80_call(
            shim,
            vec![
                Expr::Unary {
                    op: UnaryOp::Deref,
                    expr: Box::new(Expr::Var("self".into())),
                },
                Expr::Unary {
                    op: UnaryOp::Deref,
                    expr: Box::new(Expr::Var("other".into())),
                },
            ],
        )
    };
    let body = Expr::If {
        cond: Box::new(cmp("__slate_f80_lt")),
        then_expr: Box::new(some("Less")),
        else_expr: Box::new(Expr::If {
            cond: Box::new(cmp("__slate_f80_gt")),
            then_expr: Box::new(some("Greater")),
            else_expr: Box::new(Expr::If {
                cond: Box::new(cmp("__slate_f80_eq")),
                then_expr: Box::new(some("Equal")),
                else_expr: Box::new(Expr::Value(RustValue::None)),
            }),
        }),
    };
    let method = Method {
        name: StdTrait::PartialOrd.method().into(),
        self_kind: SelfKind::Ref,
        params: vec![FnParam {
            name: "other".into(),
            mutable: false,
            ty: Type::Ref {
                mutable: false,
                inner: Box::new(Type::LongDouble),
            },
        }],
        ret: Some(ordering()),
        body,
    };
    Item::Impl(ImplBlock {
        generics: vec![],
        trait_: Some(TraitRef::Std(StdTrait::PartialOrd)),
        self_ty: Type::LongDouble,
        items: vec![ImplItem::Method(method)],
    })
}

fn f80_call(name: &str, args: Vec<Expr>) -> Expr {
    Expr::Call {
        binding: CallBinding::Generated,
        func: Box::new(Expr::Var(name.into())),
        args,
    }
}

fn f80_param(name: &str, ty: Type) -> FnParam {
    FnParam {
        name: name.into(),
        mutable: false,
        ty,
    }
}

fn f80_extern_decl(name: &str, params: Vec<FnParam>, ret: Option<Type>) -> ExternFnDecl {
    ExternFnDecl {
        attrs: Vec::new(),
        identity: FunctionIdentity::Unknown,
        name: name.into(),
        declared_type: None,
        trusted_headers: std::collections::BTreeSet::new(),
        params,
        variadic: false,
        ret,
        safe: true,
    }
}

pub(super) fn f80_cast_from_name(ty: &Type) -> Option<&'static str> {
    let tag = match ty {
        Type::Prim(Prim::I8) => "i8",
        Type::Prim(Prim::U8) => "u8",
        Type::Prim(Prim::I16) => "i16",
        Type::Prim(Prim::U16) => "u16",
        Type::Prim(Prim::I32) => "i32",
        Type::Prim(Prim::U32) => "u32",
        Type::Prim(Prim::I64) => "i64",
        Type::Prim(Prim::U64) => "u64",
        Type::Prim(Prim::I128) => "i128",
        Type::Prim(Prim::U128) => "u128",
        Type::Prim(Prim::F32) => "f32",
        Type::Prim(Prim::F64) => "f64",
        Type::Prim(Prim::Bool) => "bool",
        _ => return None,
    };
    Some(match tag {
        "i8" => "__slate_f80_from_i8",
        "u8" => "__slate_f80_from_u8",
        "i16" => "__slate_f80_from_i16",
        "u16" => "__slate_f80_from_u16",
        "i32" => "__slate_f80_from_i32",
        "u32" => "__slate_f80_from_u32",
        "i64" => "__slate_f80_from_i64",
        "u64" => "__slate_f80_from_u64",
        "i128" => "__slate_f80_from_i128",
        "u128" => "__slate_f80_from_u128",
        "f32" => "__slate_f80_from_f32",
        "f64" => "__slate_f80_from_f64",
        "bool" => "__slate_f80_from_bool",
        _ => unreachable!(),
    })
}

pub(super) fn f80_cast_to_name(ty: &Type) -> Option<&'static str> {
    let tag = match ty {
        Type::Prim(Prim::I8) => "i8",
        Type::Prim(Prim::U8) => "u8",
        Type::Prim(Prim::I16) => "i16",
        Type::Prim(Prim::U16) => "u16",
        Type::Prim(Prim::I32) => "i32",
        Type::Prim(Prim::U32) => "u32",
        Type::Prim(Prim::I64) => "i64",
        Type::Prim(Prim::U64) => "u64",
        Type::Prim(Prim::I128) => "i128",
        Type::Prim(Prim::U128) => "u128",
        Type::Prim(Prim::F32) => "f32",
        Type::Prim(Prim::F64) => "f64",
        Type::Prim(Prim::Bool) => "bool",
        _ => return None,
    };
    Some(match tag {
        "i8" => "__slate_f80_to_i8",
        "u8" => "__slate_f80_to_u8",
        "i16" => "__slate_f80_to_i16",
        "u16" => "__slate_f80_to_u16",
        "i32" => "__slate_f80_to_i32",
        "u32" => "__slate_f80_to_u32",
        "i64" => "__slate_f80_to_i64",
        "u64" => "__slate_f80_to_u64",
        "i128" => "__slate_f80_to_i128",
        "u128" => "__slate_f80_to_u128",
        "f32" => "__slate_f80_to_f32",
        "f64" => "__slate_f80_to_f64",
        "bool" => "__slate_f80_to_bool",
        _ => unreachable!(),
    })
}

fn f80_binary_extern_decl(name: &str) -> ExternFnDecl {
    let f80 = || Type::LongDouble;
    f80_extern_decl(
        name,
        vec![f80_param("a", f80()), f80_param("b", f80())],
        Some(f80()),
    )
}

pub(super) fn f80_shim_decls() -> Vec<ExternFnDecl> {
    let f80 = || Type::LongDouble;
    let mut decls = vec![
        f80_binary_extern_decl("__slate_f80_add"),
        f80_binary_extern_decl("__slate_f80_sub"),
        f80_binary_extern_decl("__slate_f80_mul"),
        f80_binary_extern_decl("__slate_f80_div"),
        f80_binary_extern_decl("__slate_f80_copysign"),
        f80_binary_extern_decl("__slate_f80_fmax"),
        f80_binary_extern_decl("__slate_f80_fmin"),
        f80_extern_decl("__slate_f80_neg", vec![f80_param("a", f80())], Some(f80())),
    ];
    for shim in [
        "__slate_f80_abs",
        "__slate_f80_ceil",
        "__slate_f80_floor",
        "__slate_f80_fract",
        "__slate_f80_round",
        "__slate_f80_trunc",
        "__slate_f80_rint",
    ] {
        decls.push(f80_extern_decl(
            shim,
            vec![f80_param("a", f80())],
            Some(f80()),
        ));
    }
    decls.push(f80_extern_decl(
        "__slate_f80_signbit",
        vec![f80_param("a", f80())],
        Some(Type::Prim(Prim::Bool)),
    ));
    decls.push(f80_extern_decl(
        "__slate_f80_is_fp_class",
        vec![
            f80_param("a", f80()),
            f80_param("flags", Type::Prim(Prim::I32)),
        ],
        Some(Type::Prim(Prim::Bool)),
    ));
    decls.push(f80_extern_decl(
        "__slate_cf80_mul",
        vec![
            f80_param("a", Type::Complex(Box::new(f80()))),
            f80_param("b", Type::Complex(Box::new(f80()))),
        ],
        Some(Type::Complex(Box::new(f80()))),
    ));
    decls.push(f80_extern_decl(
        "__slate_cf80_div",
        vec![
            f80_param("a", Type::Complex(Box::new(f80()))),
            f80_param("b", Type::Complex(Box::new(f80()))),
        ],
        Some(Type::Complex(Box::new(f80()))),
    ));
    decls.push(f80_extern_decl(
        "__slate_f80_fma",
        vec![
            f80_param("a", f80()),
            f80_param("b", f80()),
            f80_param("c", f80()),
        ],
        Some(f80()),
    ));
    for (shim, ty) in [
        ("__slate_f80_lt", Type::Prim(Prim::Bool)),
        ("__slate_f80_le", Type::Prim(Prim::Bool)),
        ("__slate_f80_gt", Type::Prim(Prim::Bool)),
        ("__slate_f80_ge", Type::Prim(Prim::Bool)),
        ("__slate_f80_eq", Type::Prim(Prim::Bool)),
        ("__slate_f80_ne", Type::Prim(Prim::Bool)),
    ] {
        decls.push(f80_extern_decl(
            shim,
            vec![f80_param("a", f80()), f80_param("b", f80())],
            Some(ty),
        ));
    }
    for (shim, ty) in [
        ("__slate_f80_from_i8", Type::Prim(Prim::I8)),
        ("__slate_f80_from_u8", Type::Prim(Prim::U8)),
        ("__slate_f80_from_i16", Type::Prim(Prim::I16)),
        ("__slate_f80_from_u16", Type::Prim(Prim::U16)),
        ("__slate_f80_from_i32", Type::Prim(Prim::I32)),
        ("__slate_f80_from_u32", Type::Prim(Prim::U32)),
        ("__slate_f80_from_i64", Type::Prim(Prim::I64)),
        ("__slate_f80_from_u64", Type::Prim(Prim::U64)),
        ("__slate_f80_from_i128", Type::Prim(Prim::I128)),
        ("__slate_f80_from_u128", Type::Prim(Prim::U128)),
        ("__slate_f80_from_f32", Type::Prim(Prim::F32)),
        ("__slate_f80_from_f64", Type::Prim(Prim::F64)),
        ("__slate_f80_from_bool", Type::Prim(Prim::Bool)),
    ] {
        decls.push(f80_extern_decl(shim, vec![f80_param("a", ty)], Some(f80())));
    }
    for (shim, ty) in [
        ("__slate_f80_to_i8", Type::Prim(Prim::I8)),
        ("__slate_f80_to_u8", Type::Prim(Prim::U8)),
        ("__slate_f80_to_i16", Type::Prim(Prim::I16)),
        ("__slate_f80_to_u16", Type::Prim(Prim::U16)),
        ("__slate_f80_to_i32", Type::Prim(Prim::I32)),
        ("__slate_f80_to_u32", Type::Prim(Prim::U32)),
        ("__slate_f80_to_i64", Type::Prim(Prim::I64)),
        ("__slate_f80_to_u64", Type::Prim(Prim::U64)),
        ("__slate_f80_to_i128", Type::Prim(Prim::I128)),
        ("__slate_f80_to_u128", Type::Prim(Prim::U128)),
        ("__slate_f80_to_f32", Type::Prim(Prim::F32)),
        ("__slate_f80_to_f64", Type::Prim(Prim::F64)),
        ("__slate_f80_to_bool", Type::Prim(Prim::Bool)),
    ] {
        decls.push(f80_extern_decl(shim, vec![f80_param("a", f80())], Some(ty)));
    }
    decls
}

fn fenv_param(name: &str, ty: Type) -> FnParam {
    FnParam {
        name: name.into(),
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
        && !crate::frontend::toolchain::uses_f64_long_double_abi()
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
        Type::Prim(Prim::F128) => "lq".into(),
        Type::Prim(Prim::Bool) => "bool".into(),
        Type::CLib(clib) if *clib == CLibType::CHAR => "c".into(),
        Type::LongDouble => "f80".into(),
        Type::Complex(inner) if matches!(inner.as_ref(), Type::LongDouble) => "cf80".into(),
        Type::Unit => "v".into(),
        Type::Ptr { inner, .. } => format!("p{}", long_double_shim_type_tag(inner)),
        _ => "x".into(),
    }
}

pub(super) fn is_complex_runtime_call(name: &str) -> bool {
    matches!(name, "__muldc3" | "__divdc3" | "__mulsc3" | "__divsc3")
}

pub(super) fn complex_ty(inner: Type) -> Type {
    Type::Complex(Box::new(inner))
}

pub(super) fn complex_runtime_decl(name: &str, prim: Prim) -> ExternDecl {
    let param = |n: &str| FnParam {
        name: n.into(),
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
pub(super) fn complex_prelude() -> Vec<Item> {
    vec![Item::ExternBlock {
        abi: "C".into(),
        decls: vec![
            complex_runtime_decl("__muldc3", Prim::F64),
            complex_runtime_decl("__divdc3", Prim::F64),
            complex_runtime_decl("__mulsc3", Prim::F32),
            complex_runtime_decl("__divsc3", Prim::F32),
        ],
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
        label: None,
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
