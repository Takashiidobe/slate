use super::exprs::{next_tmp, node_type, truthy};
use super::{Env, LResult};
use crate::backend::rust_ast::{
    BinOp, Block, Expr as RExpr, IndentStmt, Raw, RustValue, Stmt as RStmt, UnaryOp as RUnaryOp,
};
use crate::frontend::native_lowerer::exprs::lower_expr;
use crate::function_identity::CallBinding;
use crate::parse::clang_ast::{Clang, Node};

/// GNU/Clang implicit compiler builtins that never appear as ordinary
/// `FunctionDecl`s (see `items::collect_top_level`'s `!is_implicit` guard),
/// so a call to one can't resolve through the normal `DeclRefExpr` -> `Ctx::vars`
/// path -- they need direct Rust-equivalent codegen instead.
pub(crate) fn builtin_callee_name(node: &Node) -> Option<&str> {
    let Clang::ImplicitCastExpr(_) = &node.kind else {
        return None;
    };
    let inner = node.inner.first()?;
    let Clang::DeclRefExpr(r) = &inner.kind else {
        return None;
    };
    let name = r.referenced_decl.name.as_deref()?;
    name.starts_with("__builtin").then_some(name)
}

/// (builtin name, libc symbol, arg count, returns i64 instead of f64)
const MATH_BUILTINS: &[(&str, &str, usize, bool)] = &[
    ("__builtin_sin", "sin", 1, false),
    ("__builtin_cos", "cos", 1, false),
    ("__builtin_tan", "tan", 1, false),
    ("__builtin_log", "log", 1, false),
    ("__builtin_log10", "log10", 1, false),
    ("__builtin_log2", "log2", 1, false),
    ("__builtin_exp", "exp", 1, false),
    ("__builtin_exp2", "exp2", 1, false),
    ("__builtin_sqrt", "sqrt", 1, false),
    ("__builtin_pow", "pow", 2, false),
    ("__builtin_fmod", "fmod", 2, false),
    ("__builtin_lround", "lround", 1, true),
    ("__builtin_llround", "llround", 1, true),
];

pub(crate) fn collect_math_builtin_externs(
    tu: &Node,
) -> Vec<crate::backend::rust_ast::ExternFnDecl> {
    use crate::backend::rust_ast::{ExternFnDecl, FnParam, Prim, Type};
    use crate::function_identity::FunctionIdentity;

    fn walk<'a>(node: &'a Node, used: &mut std::collections::BTreeSet<&'a str>) {
        if let Clang::DeclRefExpr(r) = &node.kind
            && let Some(name) = r.referenced_decl.name.as_deref()
            && let Some((builtin, ..)) = MATH_BUILTINS.iter().find(|(b, ..)| *b == name)
        {
            used.insert(builtin);
        }
        for child in &node.inner {
            walk(child, used);
        }
    }

    let mut used = std::collections::BTreeSet::new();
    walk(tu, &mut used);

    MATH_BUILTINS
        .iter()
        .filter(|(builtin, ..)| used.contains(builtin))
        .map(|(_, libc_name, argc, ret_i64)| ExternFnDecl {
            name: (*libc_name).into(),
            identity: FunctionIdentity::Unknown,
            declared_type: None,
            params: (0..*argc)
                .map(|i| FnParam {
                    name: format!("arg{i}"),
                    mutable: false,
                    ty: Type::Prim(Prim::F64),
                })
                .collect(),
            variadic: false,
            ret: Some(Type::Prim(if *ret_i64 { Prim::I64 } else { Prim::F64 })),
            safe: false,
        })
        .collect()
}

pub(crate) fn try_lower_builtin_call(node: &Node, name: &str, env: Env) -> Option<LResult<RExpr>> {
    let args: Vec<&Node> = node.inner[1..].iter().collect();
    let arg = |i: usize| -> LResult<RExpr> { lower_expr(args[i], env) };
    let result_ty = || node_type(node).lower(env.records);

    if let Some((_, libc_name, argc, _)) = MATH_BUILTINS.iter().find(|(b, ..)| *b == name) {
        return Some((|| {
            let call_args = (0..*argc)
                .map(|i| {
                    Ok(RExpr::Cast {
                        expr: Box::new(arg(i)?),
                        ty: crate::backend::rust_ast::Type::Prim(
                            crate::backend::rust_ast::Prim::F64,
                        ),
                    })
                })
                .collect::<LResult<Vec<_>>>()?;
            Ok(RExpr::Call {
                binding: CallBinding::Generated,
                func: Box::new(RExpr::Var((*libc_name).into())),
                args: call_args,
            })
        })());
    }

    const KNOWN: &[&str] = &[
        "__builtin_addressof",
        "__builtin_debugtrap",
        "__builtin_trap",
        "__builtin_unreachable",
        "__builtin_expect",
        "__builtin_assume",
        "__builtin_return_address",
        "__builtin_frame_address",
        "__builtin___clear_cache",
        "__builtin_prefetch",
        "__builtin_bitreverse32",
        "__builtin_bswap32",
        "__builtin_clz",
        "__builtin_ctz",
        "__builtin_popcount",
        "__builtin_parity",
        "__builtin_ffs",
        "__builtin_clrsb",
        "__builtin_rotateleft32",
        "__builtin_rotateright32",
        "__builtin_add_overflow",
        "__builtin_sub_overflow",
        "__builtin_mul_overflow",
    ];
    if !KNOWN.contains(&name) {
        return None;
    }

    Some((|| -> LResult<RExpr> {
        match name {
            "__builtin_addressof" => Ok(RExpr::Unary {
                op: RUnaryOp::Raw(Raw::Mut),
                expr: Box::new(arg(0)?),
            }),
            "__builtin_debugtrap" | "__builtin_trap" => Ok(RExpr::Call {
                binding: CallBinding::Generated,
                func: Box::new(RExpr::Var("std::process::abort".into())),
                args: vec![],
            }),
            "__builtin_unreachable" => Ok(RExpr::Macro {
                name: "unreachable".into(),
                args: vec![],
            }),
            "__builtin_expect" => Ok(RExpr::Cast {
                expr: Box::new(arg(0)?),
                ty: result_ty(),
            }),
            "__builtin_assume" => Ok(RExpr::Call {
                binding: CallBinding::Generated,
                func: Box::new(RExpr::Var("core::hint::assert_unchecked".into())),
                args: vec![truthy(args[0], env)?],
            }),
            "__builtin_return_address" | "__builtin_frame_address" => Ok(RExpr::Cast {
                expr: Box::new(RExpr::Value(RustValue::Usize(1))),
                ty: result_ty(),
            }),
            "__builtin___clear_cache" | "__builtin_prefetch" => {
                let evaluated = (0..args.len()).map(arg).collect::<LResult<Vec<_>>>()?;
                Ok(RExpr::Block(Box::new(Block {
                    stmts: evaluated
                        .into_iter()
                        .map(|e| IndentStmt {
                            depth: 0,
                            stmt: RStmt::Expr(e),
                        })
                        .collect(),
                    tail: None,
                })))
            }
            "__builtin_bitreverse32" => Ok(RExpr::MethodCall {
                recv: Box::new(arg(0)?),
                method: "reverse_bits".into(),
                args: vec![],
            }),
            "__builtin_bswap32" => Ok(RExpr::MethodCall {
                recv: Box::new(arg(0)?),
                method: "swap_bytes".into(),
                args: vec![],
            }),
            "__builtin_clz" => Ok(RExpr::Cast {
                expr: Box::new(RExpr::MethodCall {
                    recv: Box::new(arg(0)?),
                    method: "leading_zeros".into(),
                    args: vec![],
                }),
                ty: result_ty(),
            }),
            "__builtin_ctz" => Ok(RExpr::Cast {
                expr: Box::new(RExpr::MethodCall {
                    recv: Box::new(arg(0)?),
                    method: "trailing_zeros".into(),
                    args: vec![],
                }),
                ty: result_ty(),
            }),
            "__builtin_popcount" => Ok(RExpr::Cast {
                expr: Box::new(RExpr::MethodCall {
                    recv: Box::new(arg(0)?),
                    method: "count_ones".into(),
                    args: vec![],
                }),
                ty: result_ty(),
            }),
            "__builtin_parity" => Ok(RExpr::Cast {
                expr: Box::new(RExpr::Binary {
                    op: BinOp::BitAnd,
                    lhs: Box::new(RExpr::MethodCall {
                        recv: Box::new(arg(0)?),
                        method: "count_ones".into(),
                        args: vec![],
                    }),
                    rhs: Box::new(RExpr::Value(RustValue::I64(1))),
                }),
                ty: result_ty(),
            }),
            "__builtin_ffs" => {
                let a = arg(0)?;
                let ty = result_ty();
                Ok(RExpr::If {
                    cond: Box::new(RExpr::Binary {
                        op: BinOp::Eq,
                        lhs: Box::new(a.clone()),
                        rhs: Box::new(RExpr::Value(RustValue::I64(0))),
                    }),
                    then_expr: Box::new(RExpr::Value(RustValue::I64(0))),
                    else_expr: Box::new(RExpr::Binary {
                        op: BinOp::Add,
                        lhs: Box::new(RExpr::Cast {
                            expr: Box::new(RExpr::MethodCall {
                                recv: Box::new(a),
                                method: "trailing_zeros".into(),
                                args: vec![],
                            }),
                            ty,
                        }),
                        rhs: Box::new(RExpr::Value(RustValue::I64(1))),
                    }),
                })
            }
            "__builtin_clrsb" => {
                let a = arg(0)?;
                let ty = result_ty();
                let sign_stripped = RExpr::If {
                    cond: Box::new(RExpr::Binary {
                        op: BinOp::Lt,
                        lhs: Box::new(a.clone()),
                        rhs: Box::new(RExpr::Value(RustValue::I64(0))),
                    }),
                    then_expr: Box::new(RExpr::Unary {
                        op: RUnaryOp::Not,
                        expr: Box::new(a.clone()),
                    }),
                    else_expr: Box::new(a),
                };
                Ok(RExpr::Binary {
                    op: BinOp::Sub,
                    lhs: Box::new(RExpr::Cast {
                        expr: Box::new(RExpr::MethodCall {
                            recv: Box::new(sign_stripped),
                            method: "leading_zeros".into(),
                            args: vec![],
                        }),
                        ty,
                    }),
                    rhs: Box::new(RExpr::Value(RustValue::I64(1))),
                })
            }
            "__builtin_rotateleft32" => Ok(RExpr::MethodCall {
                recv: Box::new(arg(0)?),
                method: "rotate_left".into(),
                args: vec![arg(1)?],
            }),
            "__builtin_rotateright32" => Ok(RExpr::MethodCall {
                recv: Box::new(arg(0)?),
                method: "rotate_right".into(),
                args: vec![arg(1)?],
            }),
            "__builtin_add_overflow" | "__builtin_sub_overflow" | "__builtin_mul_overflow" => {
                let lhs = arg(0)?;
                let rhs = arg(1)?;
                let out_ptr = arg(2)?;
                let method = match name {
                    "__builtin_add_overflow" => "overflowing_add",
                    "__builtin_sub_overflow" => "overflowing_sub",
                    _ => "overflowing_mul",
                };
                let tmp = next_tmp();
                Ok(RExpr::Block(Box::new(Block {
                    stmts: vec![
                        IndentStmt {
                            depth: 0,
                            stmt: RStmt::Let {
                                name: tmp.clone(),
                                mutable: false,
                                ty: None,
                                init: Some(RExpr::MethodCall {
                                    recv: Box::new(lhs),
                                    method: method.into(),
                                    args: vec![rhs],
                                }),
                            },
                        },
                        IndentStmt {
                            depth: 0,
                            stmt: RStmt::Assign {
                                target: RExpr::Unary {
                                    op: RUnaryOp::Deref,
                                    expr: Box::new(out_ptr),
                                },
                                value: RExpr::TupleField {
                                    base: Box::new(RExpr::Var(tmp.as_str().into())),
                                    index: 0,
                                },
                            },
                        },
                    ],
                    tail: Some(Box::new(RExpr::TupleField {
                        base: Box::new(RExpr::Var(tmp.as_str().into())),
                        index: 1,
                    })),
                })))
            }
            _ => unreachable!("filtered by KNOWN above"),
        }
    })())
}
