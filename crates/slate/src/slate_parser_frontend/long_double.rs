use crate::backend::rust_ast::{
    Attr as RustAttr, CLibType, Derive, Expr, ExternFnDecl, FnParam, Ident, ImplBlock, ImplItem,
    Item, Method, Path, Prim, Repr, RustValue, SelfKind, StdTrait, Stmt, StructDef, StructFields,
    TraitRef, Type, UnaryOp, Visibility,
};
use crate::function_identity::{CallBinding, FunctionIdentity};

pub(crate) const LONG_DOUBLE_TY: &str = "LongDouble";

pub(crate) fn long_double_zero_expr() -> Expr {
    Expr::TupleStructLit {
        name: LONG_DOUBLE_TY.into(),
        fields: vec![Expr::ArrayRepeat {
            elem: Box::new(Expr::Value(RustValue::I64(0))),
            len: 10,
        }],
    }
}

// x86-64 SysV long double is the 10-byte x87 payload in a 16-byte slot.
pub(crate) fn long_double_prelude(vis: Visibility) -> Vec<Item> {
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
            name: shim_param("o"),
            mutable: false,
            ty: Type::LongDouble,
        }],
        ret: None,
        body: Expr::Block(Box::new(crate::backend::rust_ast::Block {
            stmts: vec![Stmt::Assign {
                target: self_value.clone(),
                value: f80_call(shim, vec![self_value, Expr::Var(shim_param("o").into())]),
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
            name: shim_param("o"),
            mutable: false,
            ty: Type::LongDouble,
        }],
        ret: Some(Type::LongDouble),
        body: f80_call(
            shim,
            vec![Expr::Var("self".into()), Expr::Var(shim_param("o").into())],
        ),
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
            name: shim_param("other"),
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
                    expr: Box::new(Expr::Var(shim_param("other").into())),
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
                    expr: Box::new(Expr::Var(shim_param("other").into())),
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
            name: shim_param("other"),
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
        name: shim_param(name),
        mutable: false,
        ty,
    }
}

pub(crate) fn shim_param(name: &str) -> String {
    format!("__{name}")
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

pub(crate) fn f80_cast_from_name(ty: &Type) -> Option<&'static str> {
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

pub(crate) fn f80_cast_to_name(ty: &Type) -> Option<&'static str> {
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

pub(crate) fn f80_shim_decls() -> Vec<ExternFnDecl> {
    let f80 = || Type::LongDouble;
    let mut decls = vec![
        f80_binary_extern_decl("__slate_f80_add"),
        f80_binary_extern_decl("__slate_f80_sub"),
        f80_binary_extern_decl("__slate_f80_mul"),
        f80_binary_extern_decl("__slate_f80_div"),
        f80_binary_extern_decl("__slate_f80_copysign"),
        f80_binary_extern_decl("__slate_f80_fmax"),
        f80_binary_extern_decl("__slate_f80_fmin"),
        f80_extern_decl(
            "__slate_f80_powi",
            vec![f80_param("a", f80()), f80_param("n", Type::Prim(Prim::I32))],
            Some(f80()),
        ),
        f80_extern_decl(
            "__slate_f128_nexttoward",
            vec![
                f80_param("from", Type::Prim(Prim::F128)),
                f80_param("toward", Type::Prim(Prim::F128)),
            ],
            Some(Type::Prim(Prim::F128)),
        ),
        f80_extern_decl("__slate_f80_neg", vec![f80_param("a", f80())], Some(f80())),
        ExternFnDecl {
            safe: false,
            ..f80_extern_decl(
                "__slate_f80_va_arg",
                vec![f80_param(
                    "ap",
                    Type::Ptr {
                        mutable: true,
                        inner: Box::new(Type::VaList),
                    },
                )],
                Some(f80()),
            )
        },
    ];
    for shim in [
        "__slate_f80_abs",
        "__slate_f80_ceil",
        "__slate_f80_floor",
        "__slate_f80_sqrt",
        "__slate_f80_cbrt",
        "__slate_f80_exp",
        "__slate_f80_exp2",
        "__slate_f80_expm1",
        "__slate_f80_log",
        "__slate_f80_log2",
        "__slate_f80_log10",
        "__slate_f80_log1p",
        "__slate_f80_sin",
        "__slate_f80_cos",
        "__slate_f80_tan",
        "__slate_f80_asin",
        "__slate_f80_acos",
        "__slate_f80_atan",
        "__slate_f80_sinh",
        "__slate_f80_cosh",
        "__slate_f80_tanh",
        "__slate_f80_asinh",
        "__slate_f80_acosh",
        "__slate_f80_atanh",
        "__slate_f80_nearbyint",
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
    for shim in [
        "__slate_f80_fmod",
        "__slate_f80_remainder",
        "__slate_f80_pow",
        "__slate_f80_fdim",
        "__slate_f80_hypot",
    ] {
        decls.push(f80_binary_extern_decl(shim));
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

pub(crate) fn long_double_shim_type_tag(ty: &Type) -> String {
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
