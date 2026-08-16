use super::types::CType;
use super::{Env, LResult, LowerError, NodeExt};
use crate::backend::rust_ast::{
    BinOp, Block, Expr as RExpr, IndentStmt, Prim, Raw, RustValue, Stmt as RStmt, Type as RType,
    UnaryOp as RUnaryOp,
};
use crate::function_identity::CallBinding;
use crate::parse::clang_ast::{Clang, Node, QualType};
use std::sync::atomic::{AtomicUsize, Ordering};

static TMP_COUNTER: AtomicUsize = AtomicUsize::new(0);

fn next_tmp() -> String {
    format!("__tmp{}", TMP_COUNTER.fetch_add(1, Ordering::Relaxed))
}

fn qual_type_of(node: &Node) -> Option<&QualType> {
    match &node.kind {
        Clang::ParenExpr(e)
        | Clang::CallExpr(e)
        | Clang::ArraySubscriptExpr(e)
        | Clang::InitListExpr(e)
        | Clang::ConditionalOperator(e) => e.qual_type.as_ref(),
        Clang::BinaryOperator(b) | Clang::CompoundAssignOperator(b) => b.qual_type.as_ref(),
        Clang::UnaryOperator(u) => u.qual_type.as_ref(),
        Clang::DeclRefExpr(r) => r.qual_type.as_ref(),
        Clang::ImplicitCastExpr(c) | Clang::CStyleCastExpr(c) => c.qual_type.as_ref(),
        Clang::MemberExpr(m) => m.qual_type.as_ref(),
        Clang::UnaryExprOrTypeTraitExpr(s) => s.qual_type.as_ref(),
        Clang::IntegerLiteral(l) | Clang::FloatingLiteral(l) | Clang::StringLiteral(l) => {
            l.qual_type.as_ref()
        }
        Clang::CharacterLiteral(l) => l.qual_type.as_ref(),
        _ => None,
    }
}

fn lowers_to_rust_bool(node: &Node) -> bool {
    match &node.kind {
        Clang::BinaryOperator(b) => matches!(
            b.opcode.as_str(),
            "==" | "!=" | "<" | "<=" | ">" | ">=" | "&&" | "||"
        ),
        Clang::UnaryOperator(u) => u.opcode == "!",
        _ => false,
    }
}

pub(crate) fn node_type(node: &Node) -> CType {
    if lowers_to_rust_bool(node) {
        return CType::Bool;
    }
    qual_type_of(node)
        .map(|t| CType::parse(t.canonical()))
        .unwrap_or(CType::Int)
}

pub(crate) fn lower_expr(node: &Node, env: Env) -> LResult<RExpr> {
    match &node.kind {
        Clang::ParenExpr(_) => lower_expr(node.child(0)?, env),
        Clang::IntegerLiteral(l) => {
            let ty = node_type(node);
            Ok(RExpr::Value(ty.int_value(
                l.value.trim().parse::<u128>().unwrap_or(0) as i128,
            )))
        }
        Clang::FloatingLiteral(l) if matches!(node_type(node), CType::LDouble) => {
            // Clang's `value` text is already rounded to 80-bit precision, so
            // parse it directly rather than round-tripping through f64 (which
            // would silently truncate literals with more significant digits
            // than f64 can carry).
            Ok(
                crate::frontend::lowerer::constants::f80_literal_expr(&l.value).unwrap_or_else(
                    || RExpr::Call {
                        binding: CallBinding::Generated,
                        func: Box::new(RExpr::Var("__slate_f80_from_f64".into())),
                        args: vec![RExpr::Value(RustValue::Float(
                            l.value.parse::<f64>().unwrap_or(0.0).into(),
                        ))],
                    },
                ),
            )
        }
        Clang::FloatingLiteral(l) => Ok(RExpr::Value(RustValue::Float(
            l.value.parse::<f64>().unwrap_or(0.0).into(),
        ))),
        Clang::CharacterLiteral(l) => {
            let ty = node_type(node);
            Ok(RExpr::Value(ty.int_value(l.value as i128)))
        }
        Clang::StringLiteral(l) => Ok(RExpr::CStr(unescape_c_string(&l.value))),
        Clang::DeclRefExpr(r) => {
            if let Some(info) = env.vars.get(&r.referenced_decl.id) {
                return Ok(RExpr::Var(info.name.as_str().into()));
            }
            if let Some(value) = env.enum_values.get(&r.referenced_decl.id) {
                return Ok(RExpr::Value(node_type(node).int_value(*value)));
            }
            Err(LowerError::UnresolvedDecl(r.referenced_decl.id))
        }
        Clang::UnaryExprOrTypeTraitExpr(s) => {
            let operand_ty = match &s.arg_type {
                Some(qt) => CType::parse(qt.canonical()),
                None => node_type(node.child(0)?),
            };
            let rust_ty = operand_ty.lower(env.records).render();
            let op = match s.name.as_deref() {
                Some("alignof" | "_Alignof" | "__alignof") => "align_of",
                _ => "size_of",
            };
            let call = RExpr::Call {
                func: Box::new(RExpr::Var(format!("std::mem::{op}::<{rust_ty}>").into())),
                args: Vec::new(),
                binding: CallBinding::Generated,
            };
            Ok(RExpr::Cast {
                expr: Box::new(call),
                ty: node_type(node).lower(env.records),
            })
        }
        Clang::UnaryOperator(u) => unary_expr(node, u, env),
        Clang::BinaryOperator(b) => binary_expr(node, b, env),
        Clang::CompoundAssignOperator(b) => compound_assign_expr(node, b, env),
        Clang::CallExpr(_) => call_expr(node, env),
        Clang::MemberExpr(m) => member_expr(node, m, env),
        Clang::ArraySubscriptExpr(_) => subscript_expr(node, env),
        Clang::ImplicitCastExpr(c) | Clang::CStyleCastExpr(c) => cast_expr(node, c, env),
        Clang::ConditionalOperator(_) => Ok(RExpr::If {
            cond: Box::new(truthy(node.child(0)?, env)?),
            then_expr: Box::new(lower_expr(node.child(1)?, env)?),
            else_expr: Box::new(lower_expr(node.child(2)?, env)?),
        }),
        Clang::InitListExpr(_) => super::globals::lower_init(node, &node_type(node), env),
        Clang::Other(o) if is_transparent_wrapper(o.kind.as_deref()) && node.inner.len() == 1 => {
            lower_expr(node.child(0)?, env)
        }
        Clang::Other(o) => Err(LowerError::UnsupportedExpr(o.kind.clone())),
        _ => Err(LowerError::UnsupportedExpr(None)),
    }
}

fn is_transparent_wrapper(kind: Option<&str>) -> bool {
    matches!(
        kind,
        Some("ConstantExpr")
            | Some("ExprWithCleanups")
            | Some("MaterializeTemporaryExpr")
            | Some("CXXBindTemporaryExpr")
    )
}

pub(crate) fn truthy(node: &Node, env: Env) -> LResult<RExpr> {
    let ty = node_type(node);
    match &ty {
        CType::Bool => lower_expr(node, env),
        CType::Array { .. } => Ok(RExpr::Value(RustValue::Bool(true))),
        CType::Ptr(_) => {
            let lowered = lower_expr(node, env)?;
            Ok(RExpr::Unary {
                op: RUnaryOp::Not,
                expr: Box::new(RExpr::MethodCall {
                    recv: Box::new(lowered),
                    method: "is_null".into(),
                    args: Vec::new(),
                }),
            })
        }
        _ if ty.is_flonum() => {
            let lowered = lower_expr(node, env)?;
            Ok(RExpr::Binary {
                op: BinOp::Ne,
                lhs: Box::new(lowered),
                rhs: Box::new(RExpr::Value(RustValue::Float(0.0.into()))),
            })
        }
        _ => {
            let lowered = lower_expr(node, env)?;
            Ok(RExpr::Binary {
                op: BinOp::Ne,
                lhs: Box::new(lowered),
                rhs: Box::new(RExpr::Value(ty.int_value(0))),
            })
        }
    }
}

fn decay_to_ptr(node: &Node, env: Env) -> LResult<RExpr> {
    let lowered = lower_expr(node, env)?;
    Ok(match node_type(node) {
        CType::Array { .. } => RExpr::MethodCall {
            recv: Box::new(RExpr::MethodCall {
                recv: Box::new(lowered),
                method: "as_ptr".into(),
                args: Vec::new(),
            }),
            method: "cast_mut".into(),
            args: Vec::new(),
        },
        _ => lowered,
    })
}

fn subscript_expr(node: &Node, env: Env) -> LResult<RExpr> {
    let base = node.child(0)?;
    let index = node.child(1)?;
    let ptr = decay_to_ptr(base, env)?;
    let offset = RExpr::Cast {
        expr: Box::new(lower_expr(index, env)?),
        ty: RType::Prim(Prim::Isize),
    };
    Ok(RExpr::Unary {
        op: RUnaryOp::Deref,
        expr: Box::new(RExpr::MethodCall {
            recv: Box::new(ptr),
            method: "wrapping_offset".into(),
            args: vec![offset],
        }),
    })
}

fn member_expr(node: &Node, m: &crate::parse::clang_ast::MemberExpr, env: Env) -> LResult<RExpr> {
    let base_node = node.child(0)?;
    let base = if m.is_arrow {
        RExpr::Unary {
            op: RUnaryOp::Deref,
            expr: Box::new(lower_expr(base_node, env)?),
        }
    } else {
        lower_expr(base_node, env)?
    };
    Ok(RExpr::Field {
        base: Box::new(base),
        field: m.name.clone().unwrap_or_default(),
    })
}

fn call_expr(node: &Node, env: Env) -> LResult<RExpr> {
    let callee = node.child(0)?;
    let callee_ty = node_type(callee);
    let fn_ty = match &callee_ty {
        CType::Func { .. } => Some(&callee_ty),
        CType::Ptr(inner) => Some(inner.as_ref()),
        _ => None,
    };
    let fixed_params = match fn_ty {
        Some(CType::Func {
            params,
            is_variadic: true,
            ..
        }) => Some(params.len()),
        _ => None,
    };
    let args = node.inner[1..]
        .iter()
        .enumerate()
        .map(|(i, a)| {
            let lowered = lower_expr(a, env)?;
            // C's variadic calling convention promotes int-typed args (which
            // include comparison/logical results in C, but lower to Rust
            // `bool` here) -- Rust rejects passing a bare `bool` to a
            // variadic function, so promote it explicitly.
            if fixed_params.is_some_and(|n| i >= n) && matches!(node_type(a), CType::Bool) {
                Ok(RExpr::Cast {
                    expr: Box::new(lowered),
                    ty: RType::Prim(Prim::I32),
                })
            } else {
                Ok(lowered)
            }
        })
        .collect::<LResult<Vec<_>>>()?;
    Ok(RExpr::Call {
        func: Box::new(lower_callee(callee, env)?),
        args,
        binding: CallBinding::Generated,
    })
}

/// A direct call's callee is a `FunctionToPointerDecay` cast wrapping the
/// called function's own name -- unlike every other use of that cast (which
/// needs `Some(..)` to satisfy the `Option<extern fn>` representation of a
/// function-pointer *value*, see `cast_expr`), calling it directly needs the
/// bare, callable function item, so that one cast is bypassed here. Calling
/// through an already-pointer-typed value (a variable, field, etc.) has no
/// such cast and unwraps the `Option` instead.
fn lower_callee(node: &Node, env: Env) -> LResult<RExpr> {
    if let Clang::ImplicitCastExpr(c) = &node.kind
        && c.cast_kind == "FunctionToPointerDecay"
    {
        return lower_expr(node.child(0)?, env);
    }
    Ok(RExpr::MethodCall {
        recv: Box::new(lower_expr(node, env)?),
        method: "unwrap".into(),
        args: Vec::new(),
    })
}

fn long_double_conversion_shim(
    target_ty: &CType,
    source_ty: &CType,
    records: &super::types::RecordRegistry,
) -> Option<&'static str> {
    use crate::frontend::lowerer::runtime_support::{f80_cast_from_name, f80_cast_to_name};
    match (target_ty, source_ty) {
        (CType::LDouble, CType::LDouble) => None,
        (CType::LDouble, _) => f80_cast_from_name(&source_ty.lower(records)),
        (_, CType::LDouble) => f80_cast_to_name(&target_ty.lower(records)),
        _ => None,
    }
}

fn cast_expr(node: &Node, c: &crate::parse::clang_ast::CastExpr, env: Env) -> LResult<RExpr> {
    let inner = node.child(0)?;
    match c.cast_kind.as_str() {
        "LValueToRValue" | "NoOp" | "ToVoid" | "BuiltinFnToFnPtr" => lower_expr(inner, env),
        "FunctionToPointerDecay" => Ok(RExpr::Call {
            binding: CallBinding::Generated,
            func: Box::new(RExpr::Var("Some".into())),
            args: vec![lower_expr(inner, env)?],
        }),
        "ArrayToPointerDecay" => decay_to_ptr(inner, env),
        "IntegralToBoolean" | "FloatingToBoolean" | "PointerToBoolean" => truthy(inner, env),
        _ => {
            let target_ty = node_type(node);
            let source_ty = node_type(inner);
            if let Some(shim) = long_double_conversion_shim(&target_ty, &source_ty, env.records) {
                return Ok(RExpr::Call {
                    binding: CallBinding::Generated,
                    func: Box::new(RExpr::Var(shim.into())),
                    args: vec![lower_expr(inner, env)?],
                });
            }
            let target = target_ty.lower(env.records);
            let lowered = if matches!(target, RType::Ptr { .. }) {
                decay_to_ptr(inner, env)?
            } else {
                lower_expr(inner, env)?
            };
            Ok(match &target {
                RType::Unit => RExpr::Block(Box::new(Block {
                    stmts: vec![IndentStmt {
                        depth: 0,
                        stmt: RStmt::Expr(lowered),
                    }],
                    tail: None,
                })),
                _ => RExpr::Cast {
                    expr: Box::new(lowered),
                    ty: target,
                },
            })
        }
    }
}

fn unary_expr(node: &Node, u: &crate::parse::clang_ast::UnaryOperator, env: Env) -> LResult<RExpr> {
    let inner = node.child(0)?;
    match u.opcode.as_str() {
        "-" => Ok(RExpr::Unary {
            op: RUnaryOp::Neg,
            expr: Box::new(lower_expr(inner, env)?),
        }),
        "~" => Ok(RExpr::Unary {
            op: RUnaryOp::Not,
            expr: Box::new(lower_expr(inner, env)?),
        }),
        "!" => Ok(RExpr::Unary {
            op: RUnaryOp::Not,
            expr: Box::new(truthy(inner, env)?),
        }),
        "+" => lower_expr(inner, env),
        "&" => Ok(RExpr::Unary {
            op: RUnaryOp::Raw(Raw::Mut),
            expr: Box::new(lower_expr(inner, env)?),
        }),
        "*" => Ok(RExpr::Unary {
            op: RUnaryOp::Deref,
            expr: Box::new(decay_to_ptr(inner, env)?),
        }),
        "++" | "--" => inc_dec_expr(u, inner, env),
        other => Err(LowerError::UnsupportedUnaryOp(other.to_string())),
    }
}

fn inc_dec_expr(
    u: &crate::parse::clang_ast::UnaryOperator,
    inner: &Node,
    env: Env,
) -> LResult<RExpr> {
    let ty = node_type(inner);
    let target = lower_expr(inner, env)?;
    let is_incr = u.opcode == "++";
    let new_value = if ty.is_pointerish() {
        let delta = if is_incr { 1i128 } else { -1 };
        RExpr::MethodCall {
            recv: Box::new(target.clone()),
            method: "wrapping_offset".into(),
            args: vec![RExpr::Value(RustValue::TypedInt(delta, Prim::Isize))],
        }
    } else if ty.is_flonum() {
        let one = RExpr::Value(RustValue::Float(1.0.into()));
        RExpr::Binary {
            op: if is_incr { BinOp::Add } else { BinOp::Sub },
            lhs: Box::new(target.clone()),
            rhs: Box::new(one),
        }
    } else {
        RExpr::Binary {
            op: if is_incr { BinOp::Add } else { BinOp::Sub },
            lhs: Box::new(target.clone()),
            rhs: Box::new(RExpr::Value(ty.int_value(1))),
        }
    };

    Ok(if u.is_postfix {
        let tmp = next_tmp();
        RExpr::Block(Box::new(Block {
            stmts: vec![
                IndentStmt {
                    depth: 0,
                    stmt: RStmt::Let {
                        name: tmp.clone(),
                        mutable: false,
                        ty: None,
                        init: Some(target.clone()),
                    },
                },
                IndentStmt {
                    depth: 0,
                    stmt: RStmt::Assign {
                        target,
                        value: new_value,
                    },
                },
            ],
            tail: Some(Box::new(RExpr::Var(tmp.as_str().into()))),
        }))
    } else {
        RExpr::Block(Box::new(Block {
            stmts: vec![IndentStmt {
                depth: 0,
                stmt: RStmt::Assign {
                    target: target.clone(),
                    value: new_value,
                },
            }],
            tail: Some(Box::new(target)),
        }))
    })
}

fn assign_expr(lhs: &Node, rhs: &Node, env: Env) -> LResult<RExpr> {
    let target = lower_expr(lhs, env)?;
    let value = lower_expr(rhs, env)?;
    Ok(RExpr::Block(Box::new(Block {
        stmts: vec![IndentStmt {
            depth: 0,
            stmt: RStmt::Assign {
                target: target.clone(),
                value,
            },
        }],
        tail: Some(Box::new(target)),
    })))
}

fn binary_expr(
    node: &Node,
    b: &crate::parse::clang_ast::BinaryOperator,
    env: Env,
) -> LResult<RExpr> {
    let lhs_node = node.child(0)?;
    let rhs_node = node.child(1)?;
    match b.opcode.as_str() {
        "=" => assign_expr(lhs_node, rhs_node, env),
        "&&" => Ok(RExpr::Binary {
            op: BinOp::And,
            lhs: Box::new(truthy(lhs_node, env)?),
            rhs: Box::new(truthy(rhs_node, env)?),
        }),
        "||" => Ok(RExpr::Binary {
            op: BinOp::Or,
            lhs: Box::new(truthy(lhs_node, env)?),
            rhs: Box::new(truthy(rhs_node, env)?),
        }),
        "+" | "-" => {
            let lhs_ty = node_type(lhs_node);
            let rhs_ty = node_type(rhs_node);
            if lhs_ty.is_pointerish() || rhs_ty.is_pointerish() {
                pointer_arith(&b.opcode, lhs_node, rhs_node, &lhs_ty, &rhs_ty, env)
            } else {
                Ok(RExpr::Binary {
                    op: BinOp::try_from(COpcode(&b.opcode))?,
                    lhs: Box::new(lower_expr(lhs_node, env)?),
                    rhs: Box::new(lower_expr(rhs_node, env)?),
                })
            }
        }
        op => Ok(RExpr::Binary {
            op: BinOp::try_from(COpcode(op))?,
            lhs: Box::new(lower_expr(lhs_node, env)?),
            rhs: Box::new(lower_expr(rhs_node, env)?),
        }),
    }
}

fn pointer_arith(
    op: &str,
    lhs: &Node,
    rhs: &Node,
    lhs_ty: &CType,
    rhs_ty: &CType,
    env: Env,
) -> LResult<RExpr> {
    let lhs_is_ptr = lhs_ty.is_pointerish();
    let rhs_is_ptr = rhs_ty.is_pointerish();

    if lhs_is_ptr && rhs_is_ptr {
        return Ok(RExpr::MethodCall {
            recv: Box::new(decay_to_ptr(lhs, env)?),
            method: "offset_from".into(),
            args: vec![decay_to_ptr(rhs, env)?],
        });
    }

    let (ptr_node, offset_node) = if lhs_is_ptr { (lhs, rhs) } else { (rhs, lhs) };
    let ptr_expr = decay_to_ptr(ptr_node, env)?;
    let offset = RExpr::Cast {
        expr: Box::new(lower_expr(offset_node, env)?),
        ty: RType::Prim(Prim::Isize),
    };
    let delta = if op == "-" {
        RExpr::Unary {
            op: RUnaryOp::Neg,
            expr: Box::new(offset),
        }
    } else {
        offset
    };
    Ok(RExpr::MethodCall {
        recv: Box::new(ptr_expr),
        method: "wrapping_offset".into(),
        args: vec![delta],
    })
}

fn compound_assign_expr(
    node: &Node,
    b: &crate::parse::clang_ast::BinaryOperator,
    env: Env,
) -> LResult<RExpr> {
    let lhs_node = node.child(0)?;
    let rhs_node = node.child(1)?;
    let base_op = b.opcode.trim_end_matches('=');
    let lhs_ty = node_type(lhs_node);
    let target = lower_expr(lhs_node, env)?;

    let new_value = if lhs_ty.is_pointerish() {
        pointer_arith(
            base_op,
            lhs_node,
            rhs_node,
            &lhs_ty,
            &node_type(rhs_node),
            env,
        )?
    } else {
        RExpr::Binary {
            op: BinOp::try_from(COpcode(base_op))?,
            lhs: Box::new(target.clone()),
            rhs: Box::new(lower_expr(rhs_node, env)?),
        }
    };

    Ok(RExpr::Block(Box::new(Block {
        stmts: vec![IndentStmt {
            depth: 0,
            stmt: RStmt::Assign {
                target: target.clone(),
                value: new_value,
            },
        }],
        tail: Some(Box::new(target)),
    })))
}

struct COpcode<'a>(&'a str);

impl TryFrom<COpcode<'_>> for BinOp {
    type Error = LowerError;

    fn try_from(op: COpcode<'_>) -> LResult<BinOp> {
        Ok(match op.0 {
            "+" => BinOp::Add,
            "-" => BinOp::Sub,
            "*" => BinOp::Mul,
            "/" => BinOp::Div,
            "%" => BinOp::Rem,
            "&" => BinOp::BitAnd,
            "|" => BinOp::BitOr,
            "^" => BinOp::BitXor,
            "<<" => BinOp::Shl,
            ">>" => BinOp::Shr,
            "==" => BinOp::Eq,
            "!=" => BinOp::Ne,
            "<" => BinOp::Lt,
            "<=" => BinOp::Le,
            ">" => BinOp::Gt,
            ">=" => BinOp::Ge,
            "&&" => BinOp::And,
            "||" => BinOp::Or,
            other => return Err(LowerError::UnsupportedBinaryOp(other.to_string())),
        })
    }
}

fn unescape_c_string(spelling: &str) -> Vec<u8> {
    let inner = spelling
        .strip_prefix('"')
        .and_then(|s| s.strip_suffix('"'))
        .unwrap_or(spelling);
    let mut out = Vec::new();
    let mut chars = inner.chars().peekable();
    while let Some(c) = chars.next() {
        if c != '\\' {
            let mut buf = [0u8; 4];
            out.extend_from_slice(c.encode_utf8(&mut buf).as_bytes());
            continue;
        }
        match chars.next() {
            Some('n') => out.push(b'\n'),
            Some('t') => out.push(b'\t'),
            Some('r') => out.push(b'\r'),
            Some('0') => out.push(0),
            Some('\\') => out.push(b'\\'),
            Some('"') => out.push(b'"'),
            Some('\'') => out.push(b'\''),
            Some('a') => out.push(0x07),
            Some('b') => out.push(0x08),
            Some('f') => out.push(0x0c),
            Some('v') => out.push(0x0b),
            Some('x') => {
                let mut value: u32 = 0;
                while let Some(d) = chars.peek().and_then(|c| c.to_digit(16)) {
                    value = value * 16 + d;
                    chars.next();
                }
                out.push(value as u8);
            }
            Some(other) => out.push(other as u8),
            None => {}
        }
    }
    out
}
