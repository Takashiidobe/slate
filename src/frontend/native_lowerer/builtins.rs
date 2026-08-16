use super::exprs::{next_tmp, node_type, truthy};
use super::types::CType;
use super::{Env, LResult};
use crate::backend::rust_ast::{
    BinOp, Block, Expr as RExpr, IndentStmt, Raw, RustValue, Stmt as RStmt, UnaryOp as RUnaryOp,
};
use crate::frontend::native_lowerer::exprs::lower_expr;
use crate::function_identity::CallBinding;
use crate::parse::clang_ast::{Clang, Node};
use clang_ast::Id;
use std::collections::HashMap;

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

/// x86 intrinsic header wrappers (`_mm_crc32_u8`, `_mm_aesenc_si128`, ...) whose
/// body is a single `return __builtin_ia32_*(args)` (or bare call, for `void`
/// wrappers) forward straight to the matching `core::arch::x86_64` function of
/// the *same name* -- Rust's std::arch intentionally mirrors Intel's public
/// intrinsic names, so this covers the whole `__builtin_ia32_*` surface (GCC's
/// `i386-builtin.def` alone has thousands of entries) without a hand-written
/// name table, unlike the bare compiler builtins in `KNOWN` below that user code
/// can call directly with no header wrapper in between (e.g. `__builtin_ia32_pause`).
fn thin_ia32_wrapper_target(node: &Node) -> bool {
    let Some(body) = node
        .inner
        .iter()
        .find(|c| matches!(c.kind, Clang::CompoundStmt(_)))
    else {
        return false;
    };
    let [stmt] = body.inner.as_slice() else {
        return false;
    };
    let call = match &stmt.kind {
        Clang::ReturnStmt(_) => stmt.inner.first().map(|n| {
            let mut n = n;
            while let Clang::ImplicitCastExpr(_) | Clang::CStyleCastExpr(_) | Clang::ParenExpr(_) =
                &n.kind
            {
                let Some(inner) = n.inner.first() else { break };
                n = inner;
            }
            n
        }),
        Clang::CallExpr(_) => Some(stmt),
        _ => None,
    };
    let Some(call) = call else { return false };
    let Clang::CallExpr(_) = &call.kind else {
        return false;
    };
    let Some(callee) = call.inner.first() else {
        return false;
    };
    builtin_callee_name(callee).is_some_and(|n| n.starts_with("__builtin_ia32_"))
}

pub(crate) fn collect_intrinsic_passthroughs(tu: &Node) -> HashMap<Id, String> {
    let mut out = HashMap::new();
    for node in &tu.inner {
        let Clang::FunctionDecl(d) = &node.kind else {
            continue;
        };
        if d.is_implicit || !thin_ia32_wrapper_target(node) {
            continue;
        }
        if let Some(name) = &d.name {
            out.insert(node.id, name.clone());
        }
    }
    out
}

pub(crate) fn intrinsic_passthrough_name<'a>(callee: &Node, env: Env<'a>) -> Option<&'a str> {
    let Clang::ImplicitCastExpr(_) = &callee.kind else {
        return None;
    };
    let inner = callee.inner.first()?;
    let Clang::DeclRefExpr(r) = &inner.kind else {
        return None;
    };
    env.intrinsic_passthroughs
        .get(&r.referenced_decl.id)
        .map(String::as_str)
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

/// (builtin name, libc symbol) for GNU/Clang implicit builtins that alias a
/// libc function Clang typechecks the call against, so call-site args already
/// carry the right implicit casts and can pass through via plain `lower_expr`
/// -- unlike `MATH_BUILTINS`, which needs a forced f64 cast.
const LIBC_PASSTHROUGH_BUILTINS: &[(&str, &str)] = &[
    ("__builtin_memchr", "memchr"),
    ("__builtin_bcopy", "bcopy"),
    ("__builtin_bzero", "bzero"),
];

pub(crate) fn collect_libc_passthrough_externs(
    tu: &Node,
) -> Vec<crate::backend::rust_ast::ExternFnDecl> {
    use crate::backend::rust_ast::{CLibType, ExternFnDecl, FnParam, Prim, Type};
    use crate::function_identity::FunctionIdentity;

    fn walk<'a>(node: &'a Node, used: &mut std::collections::BTreeSet<&'a str>) {
        if let Clang::DeclRefExpr(r) = &node.kind
            && let Some(name) = r.referenced_decl.name.as_deref()
            && let Some((builtin, ..)) = LIBC_PASSTHROUGH_BUILTINS.iter().find(|(b, ..)| *b == name)
        {
            used.insert(builtin);
        }
        for child in &node.inner {
            walk(child, used);
        }
    }

    let mut used = std::collections::BTreeSet::new();
    walk(tu, &mut used);

    let void_ptr = |mutable| Type::Ptr {
        mutable,
        inner: Box::new(Type::CLib(CLibType::VOID)),
    };

    LIBC_PASSTHROUGH_BUILTINS
        .iter()
        .filter(|(builtin, ..)| used.contains(builtin))
        .map(|(builtin, libc_name)| {
            let (params, ret): (Vec<Type>, Option<Type>) = match *builtin {
                "__builtin_memchr" => (
                    vec![
                        void_ptr(false),
                        Type::Prim(Prim::I32),
                        Type::Prim(Prim::Usize),
                    ],
                    Some(void_ptr(true)),
                ),
                "__builtin_bcopy" => (
                    vec![void_ptr(false), void_ptr(true), Type::Prim(Prim::Usize)],
                    None,
                ),
                "__builtin_bzero" => (vec![void_ptr(true), Type::Prim(Prim::Usize)], None),
                _ => unreachable!(),
            };
            ExternFnDecl {
                name: (*libc_name).into(),
                identity: FunctionIdentity::Unknown,
                declared_type: None,
                params: params
                    .into_iter()
                    .enumerate()
                    .map(|(i, ty)| FnParam {
                        name: format!("arg{i}"),
                        mutable: false,
                        ty,
                    })
                    .collect(),
                variadic: false,
                ret,
                safe: false,
            }
        })
        .collect()
}

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

fn strip_casts(node: &Node) -> &Node {
    match &node.kind {
        Clang::ImplicitCastExpr(_) | Clang::CStyleCastExpr(_) | Clang::ParenExpr(_) => {
            node.inner.first().map(strip_casts).unwrap_or(node)
        }
        _ => node,
    }
}

fn is_const_expr(node: &Node) -> bool {
    let node = strip_casts(node);
    match &node.kind {
        Clang::IntegerLiteral(_) | Clang::FloatingLiteral(_) | Clang::CharacterLiteral(_) => true,
        Clang::UnaryOperator(_) => node.inner.first().is_some_and(is_const_expr),
        Clang::BinaryOperator(_) => node.inner.len() == 2 && node.inner.iter().all(is_const_expr),
        _ => false,
    }
}

fn int_literal_value(node: &Node) -> Option<i64> {
    match &strip_casts(node).kind {
        Clang::IntegerLiteral(l) => l.value.trim().parse::<i64>().ok(),
        _ => None,
    }
}

/// (element size, element count, offset in elements) for a pointer expression that
/// statically refers to a known local/global array, either directly or via `&arr[N]`.
fn array_size_info(node: &Node) -> Option<(i64, i64, i64)> {
    let node = strip_casts(node);
    match &node.kind {
        Clang::DeclRefExpr(_) => {
            let CType::Array { base, len } = node_type(node) else {
                return None;
            };
            Some((base.size(), len.unwrap_or(0), 0))
        }
        Clang::UnaryOperator(u) if u.opcode == "&" => {
            let inner = strip_casts(node.inner.first()?);
            let Clang::ArraySubscriptExpr(_) = &inner.kind else {
                return None;
            };
            let base = strip_casts(inner.inner.first()?);
            let Clang::DeclRefExpr(_) = &base.kind else {
                return None;
            };
            let CType::Array { base: elem_ty, len } = node_type(base) else {
                return None;
            };
            let index = int_literal_value(inner.inner.get(1)?)?;
            Some((elem_ty.size(), len.unwrap_or(0), index))
        }
        _ => None,
    }
}

pub(crate) fn try_lower_builtin_call(node: &Node, name: &str, env: Env) -> Option<LResult<RExpr>> {
    let args: Vec<&Node> = node.inner[1..].iter().collect();
    let arg = |i: usize| -> LResult<RExpr> { lower_expr(args[i], env) };
    let result_ty = || node_type(node).lower(env.records);

    if let Some((_, libc_name)) = LIBC_PASSTHROUGH_BUILTINS.iter().find(|(b, ..)| *b == name) {
        return Some((|| {
            let last = args.len() - 1;
            let call_args = (0..args.len())
                .map(|i| {
                    let e = arg(i)?;
                    Ok(if i == last {
                        RExpr::Cast {
                            expr: Box::new(e),
                            ty: crate::backend::rust_ast::Type::Prim(
                                crate::backend::rust_ast::Prim::Usize,
                            ),
                        }
                    } else {
                        e
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
        "__builtin_ia32_pause",
        "__builtin_ia32_lfence",
        "__builtin_ia32_mfence",
        "__builtin_ia32_sfence",
        "__builtin_ia32_rdtsc",
        "__builtin_ia32_rdtscp",
        "__builtin_constant_p",
        "__builtin_object_size",
        "__builtin_complex",
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
            "__builtin_ia32_pause"
            | "__builtin_ia32_lfence"
            | "__builtin_ia32_mfence"
            | "__builtin_ia32_sfence"
            | "__builtin_ia32_rdtsc" => {
                let func = match name {
                    "__builtin_ia32_pause" => "core::arch::x86_64::_mm_pause",
                    "__builtin_ia32_lfence" => "core::arch::x86_64::_mm_lfence",
                    "__builtin_ia32_mfence" => "core::arch::x86_64::_mm_mfence",
                    "__builtin_ia32_sfence" => "core::arch::x86_64::_mm_sfence",
                    _ => "core::arch::x86_64::_rdtsc",
                };
                Ok(RExpr::Call {
                    binding: CallBinding::Generated,
                    func: Box::new(RExpr::Var(func.into())),
                    args: vec![],
                })
            }
            "__builtin_ia32_rdtscp" => Ok(RExpr::Call {
                binding: CallBinding::Generated,
                func: Box::new(RExpr::Var("core::arch::x86_64::__rdtscp".into())),
                args: vec![arg(0)?],
            }),
            "__builtin_constant_p" => Ok(RExpr::Value(
                node_type(node).int_value(is_const_expr(args[0]) as i128),
            )),
            "__builtin_object_size" => {
                let kind = int_literal_value(args[1]).unwrap_or(0);
                let value = match array_size_info(args[0]) {
                    Some((elem_size, len, offset)) => {
                        (elem_size * len - elem_size * offset).max(0) as i128
                    }
                    None if kind <= 1 => i128::from(u64::MAX),
                    None => 0,
                };
                Ok(RExpr::Value(node_type(node).int_value(value)))
            }
            "__builtin_complex" => Ok(RExpr::StructLit {
                name: "num_complex::Complex".into(),
                fields: vec![("re".into(), arg(0)?), ("im".into(), arg(1)?)],
            }),
            _ => unreachable!("filtered by KNOWN above"),
        }
    })())
}
