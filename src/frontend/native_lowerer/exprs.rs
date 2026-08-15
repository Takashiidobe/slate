use super::types::CType;
use super::Env;
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

pub(crate) fn lower_expr(node: &Node, env: Env) -> RExpr {
    match &node.kind {
        Clang::ParenExpr(_) => lower_expr(&node.inner[0], env),
        Clang::IntegerLiteral(l) => {
            let ty = node_type(node);
            RExpr::Value(ty.int_value(l.value.trim().parse::<u128>().unwrap_or(0) as i128))
        }
        Clang::FloatingLiteral(l) => {
            RExpr::Value(RustValue::Float(l.value.parse::<f64>().unwrap_or(0.0).into()))
        }
        Clang::CharacterLiteral(l) => {
            let ty = node_type(node);
            RExpr::Value(ty.int_value(l.value as i128))
        }
        Clang::StringLiteral(l) => RExpr::CStr(unescape_c_string(&l.value)),
        Clang::DeclRefExpr(r) => {
            let info = env
                .vars
                .get(&r.referenced_decl.id)
                .unwrap_or_else(|| panic!("unresolved decl reference {:?}", r.referenced_decl.id));
            RExpr::Var(info.name.as_str().into())
        }
        Clang::UnaryOperator(u) => unary_expr(node, u, env),
        Clang::BinaryOperator(b) => binary_expr(node, b, env),
        Clang::CompoundAssignOperator(b) => compound_assign_expr(node, b, env),
        Clang::CallExpr(_) => call_expr(node, env),
        Clang::MemberExpr(m) => member_expr(node, m, env),
        Clang::ArraySubscriptExpr(_) => subscript_expr(node, env),
        Clang::ImplicitCastExpr(c) | Clang::CStyleCastExpr(c) => cast_expr(node, c, env),
        Clang::ConditionalOperator(_) => {
            let ty = node_type(node);
            let _ = ty;
            RExpr::If {
                cond: Box::new(truthy(&node.inner[0], env)),
                then_expr: Box::new(lower_expr(&node.inner[1], env)),
                else_expr: Box::new(lower_expr(&node.inner[2], env)),
            }
        }
        Clang::InitListExpr(_) => super::globals::lower_init(node, &node_type(node), env),
        Clang::Other(o) if is_transparent_wrapper(o.kind.as_deref()) && node.inner.len() == 1 => {
            lower_expr(&node.inner[0], env)
        }
        other => todo!("native_lowerer: unsupported expr node: {other:?}"),
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

pub(crate) fn truthy(node: &Node, env: Env) -> RExpr {
    let ty = node_type(node);
    match &ty {
        CType::Bool => lower_expr(node, env),
        CType::Array { .. } => RExpr::Value(RustValue::Bool(true)),
        CType::Ptr(_) => {
            let lowered = lower_expr(node, env);
            RExpr::Unary {
                op: RUnaryOp::Not,
                expr: Box::new(RExpr::MethodCall {
                    recv: Box::new(lowered),
                    method: "is_null".into(),
                    args: Vec::new(),
                }),
            }
        }
        _ if ty.is_flonum() => {
            let lowered = lower_expr(node, env);
            RExpr::Binary {
                op: BinOp::Ne,
                lhs: Box::new(lowered),
                rhs: Box::new(RExpr::Value(RustValue::Float(0.0.into()))),
            }
        }
        _ => {
            let lowered = lower_expr(node, env);
            RExpr::Binary {
                op: BinOp::Ne,
                lhs: Box::new(lowered),
                rhs: Box::new(RExpr::Value(ty.int_value(0))),
            }
        }
    }
}

fn decay_to_ptr(node: &Node, env: Env) -> RExpr {
    let lowered = lower_expr(node, env);
    match node_type(node) {
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
    }
}

fn subscript_expr(node: &Node, env: Env) -> RExpr {
    let base = &node.inner[0];
    let index = &node.inner[1];
    let ptr = decay_to_ptr(base, env);
    let offset = RExpr::Cast {
        expr: Box::new(lower_expr(index, env)),
        ty: RType::Prim(Prim::Isize),
    };
    RExpr::Unary {
        op: RUnaryOp::Deref,
        expr: Box::new(RExpr::MethodCall {
            recv: Box::new(ptr),
            method: "wrapping_offset".into(),
            args: vec![offset],
        }),
    }
}

fn member_expr(node: &Node, m: &crate::parse::clang_ast::MemberExpr, env: Env) -> RExpr {
    let base_node = &node.inner[0];
    let base = if m.is_arrow {
        RExpr::Unary {
            op: RUnaryOp::Deref,
            expr: Box::new(lower_expr(base_node, env)),
        }
    } else {
        lower_expr(base_node, env)
    };
    RExpr::Field {
        base: Box::new(base),
        field: m.name.clone().unwrap_or_default(),
    }
}

fn call_expr(node: &Node, env: Env) -> RExpr {
    let callee = &node.inner[0];
    RExpr::Call {
        func: Box::new(lower_expr(callee, env)),
        args: node.inner[1..].iter().map(|a| lower_expr(a, env)).collect(),
        binding: CallBinding::Generated,
    }
}

fn cast_expr(node: &Node, c: &crate::parse::clang_ast::CastExpr, env: Env) -> RExpr {
    let inner = &node.inner[0];
    match c.cast_kind.as_str() {
        "LValueToRValue" | "NoOp" | "FunctionToPointerDecay" | "ToVoid" | "BuiltinFnToFnPtr" => {
            lower_expr(inner, env)
        }
        "ArrayToPointerDecay" => decay_to_ptr(inner, env),
        "IntegralToBoolean" | "FloatingToBoolean" | "PointerToBoolean" => truthy(inner, env),
        _ => {
            let target_ty = node_type(node);
            let target = target_ty.lower(env.records);
            let lowered = if matches!(target, RType::Ptr { .. }) {
                decay_to_ptr(inner, env)
            } else {
                lower_expr(inner, env)
            };
            match &target {
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
            }
        }
    }
}

fn unary_expr(node: &Node, u: &crate::parse::clang_ast::UnaryOperator, env: Env) -> RExpr {
    let inner = &node.inner[0];
    match u.opcode.as_str() {
        "-" => RExpr::Unary {
            op: RUnaryOp::Neg,
            expr: Box::new(lower_expr(inner, env)),
        },
        "~" => RExpr::Unary {
            op: RUnaryOp::Not,
            expr: Box::new(lower_expr(inner, env)),
        },
        "!" => RExpr::Unary {
            op: RUnaryOp::Not,
            expr: Box::new(truthy(inner, env)),
        },
        "+" => lower_expr(inner, env),
        "&" => RExpr::Unary {
            op: RUnaryOp::Raw(Raw::Mut),
            expr: Box::new(lower_expr(inner, env)),
        },
        "*" => RExpr::Unary {
            op: RUnaryOp::Deref,
            expr: Box::new(decay_to_ptr(inner, env)),
        },
        "++" | "--" => inc_dec_expr(u, inner, env),
        other => todo!("native_lowerer: unsupported unary opcode: {other}"),
    }
}

fn inc_dec_expr(u: &crate::parse::clang_ast::UnaryOperator, inner: &Node, env: Env) -> RExpr {
    let ty = node_type(inner);
    let target = lower_expr(inner, env);
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

    if u.is_postfix {
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
    }
}

fn assign_expr(lhs: &Node, rhs: &Node, env: Env) -> RExpr {
    let target = lower_expr(lhs, env);
    let value = lower_expr(rhs, env);
    RExpr::Block(Box::new(Block {
        stmts: vec![IndentStmt {
            depth: 0,
            stmt: RStmt::Assign {
                target: target.clone(),
                value,
            },
        }],
        tail: Some(Box::new(target)),
    }))
}

fn binary_expr(node: &Node, b: &crate::parse::clang_ast::BinaryOperator, env: Env) -> RExpr {
    let lhs_node = &node.inner[0];
    let rhs_node = &node.inner[1];
    match b.opcode.as_str() {
        "=" => assign_expr(lhs_node, rhs_node, env),
        "&&" => RExpr::Binary {
            op: BinOp::And,
            lhs: Box::new(truthy(lhs_node, env)),
            rhs: Box::new(truthy(rhs_node, env)),
        },
        "||" => RExpr::Binary {
            op: BinOp::Or,
            lhs: Box::new(truthy(lhs_node, env)),
            rhs: Box::new(truthy(rhs_node, env)),
        },
        "+" | "-" => {
            let lhs_ty = node_type(lhs_node);
            let rhs_ty = node_type(rhs_node);
            if lhs_ty.is_pointerish() || rhs_ty.is_pointerish() {
                pointer_arith(&b.opcode, lhs_node, rhs_node, &lhs_ty, &rhs_ty, env)
            } else {
                RExpr::Binary {
                    op: BinOp::from(COpcode(&b.opcode)),
                    lhs: Box::new(lower_expr(lhs_node, env)),
                    rhs: Box::new(lower_expr(rhs_node, env)),
                }
            }
        }
        op => RExpr::Binary {
            op: BinOp::from(COpcode(op)),
            lhs: Box::new(lower_expr(lhs_node, env)),
            rhs: Box::new(lower_expr(rhs_node, env)),
        },
    }
}

fn pointer_arith(
    op: &str,
    lhs: &Node,
    rhs: &Node,
    lhs_ty: &CType,
    rhs_ty: &CType,
    env: Env,
) -> RExpr {
    let lhs_is_ptr = lhs_ty.is_pointerish();
    let rhs_is_ptr = rhs_ty.is_pointerish();

    if lhs_is_ptr && rhs_is_ptr {
        return RExpr::MethodCall {
            recv: Box::new(decay_to_ptr(lhs, env)),
            method: "offset_from".into(),
            args: vec![decay_to_ptr(rhs, env)],
        };
    }

    let (ptr_node, offset_node) = if lhs_is_ptr { (lhs, rhs) } else { (rhs, lhs) };
    let ptr_expr = decay_to_ptr(ptr_node, env);
    let offset = RExpr::Cast {
        expr: Box::new(lower_expr(offset_node, env)),
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
    RExpr::MethodCall {
        recv: Box::new(ptr_expr),
        method: "wrapping_offset".into(),
        args: vec![delta],
    }
}

fn compound_assign_expr(node: &Node, b: &crate::parse::clang_ast::BinaryOperator, env: Env) -> RExpr {
    let lhs_node = &node.inner[0];
    let rhs_node = &node.inner[1];
    let base_op = b.opcode.trim_end_matches('=');
    let lhs_ty = node_type(lhs_node);
    let target = lower_expr(lhs_node, env);

    let new_value = if lhs_ty.is_pointerish() {
        pointer_arith(base_op, lhs_node, rhs_node, &lhs_ty, &node_type(rhs_node), env)
    } else {
        RExpr::Binary {
            op: BinOp::from(COpcode(base_op)),
            lhs: Box::new(target.clone()),
            rhs: Box::new(lower_expr(rhs_node, env)),
        }
    };

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
}

struct COpcode<'a>(&'a str);

impl From<COpcode<'_>> for BinOp {
    fn from(op: COpcode<'_>) -> BinOp {
        match op.0 {
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
            other => todo!("native_lowerer: unsupported binary opcode: {other}"),
        }
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
    out.push(0);
    out
}
