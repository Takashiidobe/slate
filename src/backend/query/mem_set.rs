use crate::backend::rust_ast::{Expr, Prim, RustValue, Stmt, Type};

use super::array_env::{CopyEnv, count_value, endpoint, slice_index};

pub(super) struct MemSetPlan {
    pub(super) expr: Expr,
}

pub(super) fn mem_set_plan(stmt: &Stmt, env: &CopyEnv) -> Option<MemSetPlan> {
    let write = write_bytes_stmt(stmt)?;
    plan_from_parts(write.dst, write.val, write.count, env)
}

pub(super) fn plan_from_parts(
    dst: &Expr,
    val: &Expr,
    count: &Expr,
    env: &CopyEnv,
) -> Option<MemSetPlan> {
    let dst = endpoint(dst)?;
    let dst_info = env.arrays.get(&dst.base)?;
    if !dst_info.mutable {
        return None;
    }
    let byte = count_value(val, env)? % 256;
    if byte == 0 {
        if !zero_fill_width(dst_info.elem_size) {
            return None;
        }
    } else if dst_info.byte_signed.is_none() {
        return None;
    }
    let count_bytes = count_value(count, env)?;
    if count_bytes % dst_info.elem_size != 0 {
        return None;
    }
    let len = count_bytes / dst_info.elem_size;
    if len == 0 || dst.start.checked_add(len)? > dst_info.len {
        return None;
    }
    let value = if byte == 0 {
        Expr::Value(RustValue::I64(0))
    } else {
        fill_byte_expr(byte, dst_info.byte_signed?)
    };
    Some(MemSetPlan {
        expr: fill_expr(&dst.base, dst.start, len, dst_info.len, value),
    })
}

fn zero_fill_width(elem_size: u64) -> bool {
    matches!(elem_size, 1 | 2 | 4 | 8 | 16)
}

fn fill_byte_expr(byte: u64, signed: bool) -> Expr {
    let literal = Expr::Value(RustValue::I64(byte as i64));
    if signed && byte > i8::MAX as u64 {
        Expr::Cast {
            expr: Box::new(literal),
            ty: Type::Prim(Prim::I8),
        }
    } else {
        literal
    }
}

fn fill_expr(base: &str, start: u64, len: u64, array_len: u64, value: Expr) -> Expr {
    let recv = if start == 0 && len == array_len {
        Expr::Var(base.into())
    } else {
        slice_index(base, start, start + len)
    };
    Expr::MethodCall {
        recv: Box::new(recv),
        method: "fill".into(),
        args: vec![value],
    }
}

struct WriteBytesExpr<'a> {
    dst: &'a Expr,
    val: &'a Expr,
    count: &'a Expr,
}

fn write_bytes_stmt(stmt: &Stmt) -> Option<WriteBytesExpr<'_>> {
    let Stmt::Expr(expr) = stmt else {
        return None;
    };
    match expr {
        Expr::Unsafe(block) if block.stmts.is_empty() => {
            let Expr::WriteBytes { dst, val, count } = block.tail.as_deref()? else {
                return None;
            };
            Some(WriteBytesExpr { dst, val, count })
        }
        Expr::WriteBytes { dst, val, count } => Some(WriteBytesExpr { dst, val, count }),
        _ => None,
    }
}
