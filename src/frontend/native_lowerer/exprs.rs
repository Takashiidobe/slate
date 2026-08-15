use super::types::{aggregate_type_name, lower_type, resolve_aggregate};
use crate::backend::rust_ast::{
    BinOp, Block, Expr as RExpr, IndentStmt, Prim, Raw, RustValue, Stmt as RStmt, Type as RType,
    UnaryOp as RUnaryOp,
};
use crate::function_identity::CallBinding;
use crate::parse::ast::{
    BinaryOp, Expr, ExprKind, GlobalKind, NodeId, Obj, Type as CType, UnaryOp as CUnaryOp,
};
use std::collections::HashMap;

pub(crate) struct FnCtx<'a> {
    pub locals: &'a [Obj],
    pub globals: &'a [Obj],
    pub types: &'a HashMap<NodeId, CType>,
    pub is_main: bool,
}

impl FnCtx<'_> {
    fn var(&self, idx: usize, is_local: bool) -> &Obj {
        if is_local {
            &self.locals[idx]
        } else {
            &self.globals[idx]
        }
    }
}

pub(crate) fn lower_expr(expr: &Expr, ctx: &FnCtx) -> RExpr {
    match &expr.kind {
        ExprKind::Null => RExpr::Value(RustValue::I64(0)),
        ExprKind::Num { value, fval } => num_expr(expr, *value, *fval),
        ExprKind::BigIntLiteral { raw } => bigint_expr(raw),
        ExprKind::LDoubleLiteral { value } => ldouble_expr(value),
        ExprKind::Memzero { idx, is_local } => {
            zero_value_expr(&ctx.var(*idx, *is_local).ty.clone(), ctx.types)
        }
        ExprKind::Unary { op, expr: inner } => unary_expr(*op, inner, expr, ctx),
        ExprKind::Call {
            callee,
            args,
            ret_buffer: _,
        } => RExpr::Call {
            func: Box::new(lower_expr(callee, ctx)),
            args: args.iter().map(|arg| lower_expr(arg, ctx)).collect(),
            binding: CallBinding::Generated,
        },
        ExprKind::Addr(inner) => RExpr::Unary {
            op: RUnaryOp::Raw(Raw::Mut),
            expr: Box::new(lower_expr(inner, ctx)),
        },
        ExprKind::Deref(inner) => RExpr::Unary {
            op: RUnaryOp::Deref,
            expr: Box::new(lower_expr(inner, ctx)),
        },
        ExprKind::Var { idx, is_local } => var_expr(*idx, *is_local, ctx),
        ExprKind::VlaPtr { idx, is_local } => {
            RExpr::Var(ctx.var(*idx, *is_local).name.as_str().into())
        }
        ExprKind::StmtExpr(stmts) => stmt_expr(stmts, ctx),
        ExprKind::Assign { lhs, rhs } => assign_expr(lhs, rhs, ctx),
        ExprKind::Cond { cond, then, els } => RExpr::If {
            cond: Box::new(lower_expr(cond, ctx)),
            then_expr: Box::new(lower_expr(then, ctx)),
            else_expr: Box::new(lower_expr(els, ctx)),
        },
        ExprKind::Comma { lhs, rhs } => RExpr::Block(Box::new(Block {
            stmts: vec![IndentStmt {
                depth: 0,
                stmt: RStmt::Expr(lower_expr(lhs, ctx)),
            }],
            tail: Some(Box::new(lower_expr(rhs, ctx))),
        })),
        ExprKind::Member { lhs, member } => RExpr::Field {
            base: Box::new(lower_expr(lhs, ctx)),
            field: member.name.clone(),
        },
        ExprKind::Cast { expr: inner, ty } => cast_expr(inner, ty, ctx),
        ExprKind::LabelVal { label } => RExpr::Var(label.as_str().into()),
        ExprKind::Binary { op, lhs, rhs } => binary_expr(*op, lhs, rhs, ctx),
        ExprKind::Cas { addr, old, new } => RExpr::Call {
            func: Box::new(RExpr::Var("__slate_cas_unsupported".into())),
            args: vec![
                lower_expr(addr, ctx),
                lower_expr(old, ctx),
                lower_expr(new, ctx),
            ],
            binding: CallBinding::Generated,
        },
        ExprKind::Exch { addr, val } => RExpr::Call {
            func: Box::new(RExpr::Var("__slate_exch_unsupported".into())),
            args: vec![lower_expr(addr, ctx), lower_expr(val, ctx)],
            binding: CallBinding::Generated,
        },
    }
}

fn var_expr(idx: usize, is_local: bool, ctx: &FnCtx) -> RExpr {
    let obj = ctx.var(idx, is_local);
    if !is_local
        && obj.global_kind == GlobalKind::Literal
        && let Some(bytes) = obj.init_data.clone()
    {
        return RExpr::ByteStr(bytes);
    }
    RExpr::Var(obj.name.as_str().into())
}

fn num_expr(expr: &Expr, value: i64, fval: f64) -> RExpr {
    match expr.ty.as_ref() {
        Some(ty) if ty.is_flonum() => RExpr::Value(RustValue::Float(fval.into())),
        Some(CType::Bool) => RExpr::Value(RustValue::Bool(value != 0)),
        Some(ty) => int_literal(value, ty),
        None => RExpr::Value(RustValue::I64(value)),
    }
}

fn int_literal(value: i64, ty: &CType) -> RExpr {
    let prim = int_prim(ty);
    if ty.is_unsigned() {
        RExpr::Value(RustValue::TypedUInt(value as u64 as u128, prim))
    } else {
        RExpr::Value(RustValue::TypedInt(value as i128, prim))
    }
}

fn int_prim(ty: &CType) -> Prim {
    match ty.size() {
        1 => {
            if ty.is_unsigned() {
                Prim::U8
            } else {
                Prim::I8
            }
        }
        2 => {
            if ty.is_unsigned() {
                Prim::U16
            } else {
                Prim::I16
            }
        }
        4 => {
            if ty.is_unsigned() {
                Prim::U32
            } else {
                Prim::I32
            }
        }
        16 => {
            if ty.is_unsigned() {
                Prim::U128
            } else {
                Prim::I128
            }
        }
        _ => {
            if ty.is_unsigned() {
                Prim::U64
            } else {
                Prim::I64
            }
        }
    }
}

fn bigint_expr(raw: &str) -> RExpr {
    let digits = raw.trim_end_matches(['w', 'W', 'b', 'B', 'u', 'U']);
    let mut value: i128 = 0;
    for byte in digits.bytes() {
        if let Some(digit) = (byte as char).to_digit(10) {
            value = value.wrapping_mul(10).wrapping_add(digit as i128);
        }
    }
    RExpr::Value(RustValue::I128(value))
}

fn ldouble_expr(value: &rustc_apfloat::ieee::X87DoubleExtended) -> RExpr {
    use rustc_apfloat::{Float, FloatConvert};
    let mut lost_info = false;
    let converted: rustc_apfloat::ieee::Double = (*value).convert(&mut lost_info).value;
    let bits: u128 = converted.to_bits();
    RExpr::Value(RustValue::Float(f64::from_bits(bits as u64).into()))
}

fn zero_value_expr(ty: &CType, types: &HashMap<NodeId, CType>) -> RExpr {
    let resolved = resolve_aggregate(ty, types);
    match resolved {
        CType::Struct { members, .. } => RExpr::StructLit {
            name: aggregate_type_name(resolved),
            fields: members
                .iter()
                .map(|member| (member.name.clone(), zero_value_expr(&member.ty, types)))
                .collect(),
        },
        CType::Union { members, .. } => match members.first() {
            Some(member) => RExpr::StructLit {
                name: aggregate_type_name(resolved),
                fields: vec![(member.name.clone(), zero_value_expr(&member.ty, types))],
            },
            None => RExpr::Value(RustValue::I64(0)),
        },
        CType::Array { base, len } => RExpr::ArrayRepeat {
            elem: Box::new(zero_value_expr(base, types)),
            len: (*len).max(0) as usize,
        },
        _ if resolved.is_flonum() => RExpr::Value(RustValue::Float(0.0.into())),
        CType::Ptr(_) | CType::Func { .. } => RExpr::Value(RustValue::NullPtr),
        _ => int_literal(0, resolved),
    }
}

fn unary_expr(op: CUnaryOp, inner: &Expr, expr: &Expr, ctx: &FnCtx) -> RExpr {
    let lowered = lower_expr(inner, ctx);
    match op {
        CUnaryOp::Neg => RExpr::Unary {
            op: RUnaryOp::Neg,
            expr: Box::new(lowered),
        },
        CUnaryOp::BitNot => RExpr::Unary {
            op: RUnaryOp::Not,
            expr: Box::new(lowered),
        },
        CUnaryOp::Not => {
            let zero = match inner.ty.as_ref() {
                Some(ty) if ty.is_flonum() => RExpr::Value(RustValue::Float(0.0.into())),
                Some(ty) => int_literal(0, ty),
                None => RExpr::Value(RustValue::I64(0)),
            };
            let cmp = RExpr::Binary {
                op: BinOp::Eq,
                lhs: Box::new(lowered),
                rhs: Box::new(zero),
            };
            RExpr::Cast {
                expr: Box::new(cmp),
                ty: expr
                    .ty
                    .as_ref()
                    .map(|ty| lower_type(ty, ctx.types))
                    .unwrap_or(RType::Prim(Prim::I32)),
            }
        }
        CUnaryOp::Real | CUnaryOp::Imag => lowered,
    }
}

fn stmt_expr(stmts: &[crate::parse::ast::Stmt], ctx: &FnCtx) -> RExpr {
    let mut body: Vec<IndentStmt> = Vec::new();
    let mut tail = None;
    for (i, stmt) in stmts.iter().enumerate() {
        let is_last = i + 1 == stmts.len();
        if is_last && let crate::parse::ast::StmtKind::Expr(value) = &stmt.kind {
            tail = Some(Box::new(lower_expr(value, ctx)));
            continue;
        }
        super::stmts::lower_stmt(stmt, ctx, &mut body);
    }
    RExpr::Block(Box::new(Block { stmts: body, tail }))
}

fn assign_expr(lhs: &Expr, rhs: &Expr, ctx: &FnCtx) -> RExpr {
    let target = lower_expr(lhs, ctx);
    let value = lower_expr(rhs, ctx);
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

fn cast_expr(inner: &Expr, ty: &CType, ctx: &FnCtx) -> RExpr {
    let target = lower_type(ty, ctx.types);
    let lowered = if matches!(target, RType::Ptr { .. }) {
        decay_to_ptr(inner, ctx)
    } else {
        lower_expr(inner, ctx)
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

fn binary_expr(op: BinaryOp, lhs: &Expr, rhs: &Expr, ctx: &FnCtx) -> RExpr {
    let lhs_is_ptr = innermost_ty(lhs).is_some_and(is_pointerish);
    let rhs_is_ptr = innermost_ty(rhs).is_some_and(is_pointerish);

    if matches!(op, BinaryOp::Add | BinaryOp::Sub) && (lhs_is_ptr || rhs_is_ptr) {
        return pointer_arith(op, lhs, rhs, lhs_is_ptr, rhs_is_ptr, ctx);
    }

    RExpr::Binary {
        op: map_binop(op),
        lhs: Box::new(lower_expr(lhs, ctx)),
        rhs: Box::new(lower_expr(rhs, ctx)),
    }
}

fn innermost_ty(expr: &Expr) -> Option<&CType> {
    match &expr.kind {
        ExprKind::Cast { expr: inner, .. } => innermost_ty(inner),
        _ => expr.ty.as_ref(),
    }
}

fn is_pointerish(ty: &CType) -> bool {
    matches!(ty, CType::Ptr(_) | CType::Array { .. } | CType::Vla { .. })
}

fn decay_to_ptr(expr: &Expr, ctx: &FnCtx) -> RExpr {
    let lowered = lower_expr(expr, ctx);
    match expr.ty.as_ref() {
        Some(CType::Array { .. }) | Some(CType::Vla { .. }) => RExpr::MethodCall {
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

fn pointer_arith(
    op: BinaryOp,
    lhs: &Expr,
    rhs: &Expr,
    lhs_is_ptr: bool,
    rhs_is_ptr: bool,
    ctx: &FnCtx,
) -> RExpr {
    if lhs_is_ptr && rhs_is_ptr {
        return RExpr::Binary {
            op: BinOp::Sub,
            lhs: Box::new(RExpr::Cast {
                expr: Box::new(decay_to_ptr(lhs, ctx)),
                ty: RType::Prim(Prim::Isize),
            }),
            rhs: Box::new(RExpr::Cast {
                expr: Box::new(decay_to_ptr(rhs, ctx)),
                ty: RType::Prim(Prim::Isize),
            }),
        };
    }
    let (ptr_expr, ptr_ty, byte_offset) = if lhs_is_ptr {
        (
            decay_to_ptr(lhs, ctx),
            lhs.ty.as_ref(),
            lower_expr(rhs, ctx),
        )
    } else {
        (
            decay_to_ptr(rhs, ctx),
            rhs.ty.as_ref(),
            lower_expr(lhs, ctx),
        )
    };
    let ptr_ty = ptr_ty
        .map(|ty| lower_type(ty, ctx.types))
        .unwrap_or(RType::Ptr {
            mutable: true,
            inner: Box::new(RType::Prim(Prim::U8)),
        });
    let addr = RExpr::Cast {
        expr: Box::new(ptr_expr),
        ty: RType::Prim(Prim::Usize),
    };
    let offset = RExpr::Cast {
        expr: Box::new(byte_offset),
        ty: RType::Prim(Prim::Usize),
    };
    RExpr::Cast {
        expr: Box::new(RExpr::Binary {
            op: map_binop(op),
            lhs: Box::new(addr),
            rhs: Box::new(offset),
        }),
        ty: ptr_ty,
    }
}

fn map_binop(op: BinaryOp) -> BinOp {
    match op {
        BinaryOp::Add => BinOp::Add,
        BinaryOp::Sub => BinOp::Sub,
        BinaryOp::Mul => BinOp::Mul,
        BinaryOp::Div => BinOp::Div,
        BinaryOp::Mod => BinOp::Rem,
        BinaryOp::BitAnd => BinOp::BitAnd,
        BinaryOp::BitOr => BinOp::BitOr,
        BinaryOp::BitXor => BinOp::BitXor,
        BinaryOp::Shl => BinOp::Shl,
        BinaryOp::Shr => BinOp::Shr,
        BinaryOp::LogAnd => BinOp::And,
        BinaryOp::LogOr => BinOp::Or,
        BinaryOp::Eq => BinOp::Eq,
        BinaryOp::Ne => BinOp::Ne,
        BinaryOp::Lt => BinOp::Lt,
        BinaryOp::Le => BinOp::Le,
    }
}
