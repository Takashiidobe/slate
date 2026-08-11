use crate::backend::rust_ast::Expr;

use super::array_env::{CopyEnv, count_value, endpoint, slice_index};

pub(super) fn compare_operands(
    a: &Expr,
    b: &Expr,
    count: &Expr,
    env: &CopyEnv,
) -> Option<(Expr, Expr)> {
    let a_ep = endpoint(a)?;
    let b_ep = endpoint(b)?;
    let a_info = env.arrays.get(&a_ep.base)?;
    let b_info = env.arrays.get(&b_ep.base)?;
    if a_info.elem_size != b_info.elem_size || a_info.byte_signed != b_info.byte_signed {
        return None;
    }
    let count_bytes = count_value(count, env)?;
    let elem_size = a_info.elem_size;
    if count_bytes % elem_size != 0 {
        return None;
    }
    let len = count_bytes / elem_size;
    if len == 0 {
        return None;
    }
    if a_ep.start.checked_add(len)? > a_info.len || b_ep.start.checked_add(len)? > b_info.len {
        return None;
    }
    if a_ep.start == 0 && b_ep.start == 0 && len == a_info.len && len == b_info.len {
        return Some((
            Expr::Var(a_ep.base.clone().into()),
            Expr::Var(b_ep.base.clone().into()),
        ));
    }
    Some((
        slice_index(&a_ep.base, a_ep.start, a_ep.start + len),
        slice_index(&b_ep.base, b_ep.start, b_ep.start + len),
    ))
}
