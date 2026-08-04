use crate::rust_ast::{Expr, Stmt};

use super::array_env::{CopyEnv, count_value, endpoint, range, slice_index, uint};

pub(super) struct CopyPlan {
    pub(super) expr: Expr,
}

pub(super) fn copy_plan(stmt: &Stmt, env: &CopyEnv) -> Option<CopyPlan> {
    let copy = ptr_copy_stmt(stmt)?;
    let src = endpoint(copy.src)?;
    let dst = endpoint(copy.dst)?;
    let src_info = env.arrays.get(&src.base)?;
    let dst_info = env.arrays.get(&dst.base)?;
    if !dst_info.mutable || src_info.elem_size != dst_info.elem_size {
        return None;
    }
    let count_bytes = count_value(copy.count, env)?;
    let elem_size = src_info.elem_size;
    if count_bytes % elem_size != 0 {
        return None;
    }
    let len = count_bytes / elem_size;
    if len == 0 {
        return None;
    }
    if src.start.checked_add(len)? > src_info.len || dst.start.checked_add(len)? > dst_info.len {
        return None;
    }
    if src.base == dst.base {
        return Some(CopyPlan {
            expr: copy_within(&dst.base, src.start, src.start + len, dst.start),
        });
    }
    Some(CopyPlan {
        expr: copy_from_slice(&dst.base, dst.start, len, &src.base, src.start),
    })
}

struct PtrCopyExpr<'a> {
    src: &'a Expr,
    dst: &'a Expr,
    count: &'a Expr,
}

fn ptr_copy_stmt(stmt: &Stmt) -> Option<PtrCopyExpr<'_>> {
    let Stmt::Expr(expr) = stmt else {
        return None;
    };
    match expr {
        Expr::Unsafe(block) if block.stmts.is_empty() => {
            let Expr::PtrCopy {
                src, dst, count, ..
            } = block.tail.as_deref()?
            else {
                return None;
            };
            Some(PtrCopyExpr { src, dst, count })
        }
        Expr::PtrCopy {
            src, dst, count, ..
        } => Some(PtrCopyExpr { src, dst, count }),
        _ => None,
    }
}

fn copy_within(base: &str, src_start: u64, src_end: u64, dst_start: u64) -> Expr {
    Expr::MethodCall {
        recv: Box::new(Expr::Var(base.into())),
        method: "copy_within".into(),
        args: vec![range(src_start, src_end), uint(dst_start)],
    }
}

fn copy_from_slice(dst: &str, dst_start: u64, len: u64, src: &str, src_start: u64) -> Expr {
    Expr::MethodCall {
        recv: Box::new(slice_index(dst, dst_start, dst_start + len)),
        method: "copy_from_slice".into(),
        args: vec![Expr::Ref {
            mutable: false,
            expr: Box::new(slice_index(src, src_start, src_start + len)),
        }],
    }
}
