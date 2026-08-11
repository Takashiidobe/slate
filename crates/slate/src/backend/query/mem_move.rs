use crate::backend::rust_ast::{Expr, Stmt};

use super::array_env::{CopyEnv, count_value, endpoint};
use super::ptr_copy::{copy_within, ptr_copy_stmt};

pub(super) struct MemMovePlan {
    pub(super) stmt: Stmt,
}

pub(super) fn mem_move_plan(stmt: &Stmt, env: &CopyEnv) -> Option<MemMovePlan> {
    let copy = ptr_copy_stmt(stmt)?;
    plan_from_parts(copy.dst, copy.src, copy.count, env)
}

pub(super) fn plan_from_parts(
    dst: &Expr,
    src: &Expr,
    count: &Expr,
    env: &CopyEnv,
) -> Option<MemMovePlan> {
    let src_ep = endpoint(src)?;
    let dst_ep = endpoint(dst)?;
    if src_ep.base != dst_ep.base {
        return None;
    }
    let buf_info = env.arrays.get(&dst_ep.base)?;
    if !buf_info.mutable {
        return None;
    }
    let count_bytes = count_value(count, env)?;
    let elem_size = buf_info.elem_size;
    if count_bytes % elem_size != 0 {
        return None;
    }
    let len = count_bytes / elem_size;
    if len == 0 {
        return None;
    }
    if src_ep.start.checked_add(len)? > buf_info.len
        || dst_ep.start.checked_add(len)? > buf_info.len
    {
        return None;
    }
    Some(MemMovePlan {
        stmt: Stmt::Expr(copy_within(
            &dst_ep.base,
            src_ep.start,
            src_ep.start + len,
            dst_ep.start,
        )),
    })
}
