use std::collections::BTreeMap;

use crate::rust_ast::{BinOp, Expr, IndentStmt, Prim, RustValue, Stmt, Type};

#[derive(Clone)]
pub(super) struct CopyEnv {
    arrays: BTreeMap<String, ArrayInfo>,
    constants: BTreeMap<String, u64>,
}

#[derive(Clone)]
struct ArrayInfo {
    mutable: bool,
    len: u64,
    elem_size: u64,
}

pub(super) struct CopyPlan {
    pub(super) expr: Expr,
}

#[derive(Clone)]
struct CopyEndpoint {
    base: String,
    start: u64,
}

impl CopyEnv {
    pub(super) fn from_body(body: &[IndentStmt]) -> Self {
        let mut env = Self {
            arrays: BTreeMap::new(),
            constants: BTreeMap::new(),
        };
        for indent in body {
            let Stmt::Let {
                name,
                mutable,
                ty,
                init,
            } = &indent.stmt
            else {
                continue;
            };
            if let Some(Type::Array { elem, len }) = ty.as_ref().map(|ty| ty.peel_aligned())
                && let Some(elem_size) = type_size(elem)
            {
                env.arrays.insert(
                    name.clone(),
                    ArrayInfo {
                        mutable: *mutable,
                        len: *len,
                        elem_size,
                    },
                );
            }
            if let Some(value) = init.as_ref().and_then(int_value).and_then(nonnegative) {
                env.constants.insert(name.clone(), value);
            }
        }
        env
    }
}

pub(super) fn copy_plan(stmt: &Stmt, env: &CopyEnv) -> Option<CopyPlan> {
    if let Some(plan) = write_bytes_plan(stmt, env) {
        return Some(plan);
    }
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

fn write_bytes_plan(stmt: &Stmt, env: &CopyEnv) -> Option<CopyPlan> {
    let write = write_bytes_stmt(stmt)?;
    if count_value(write.val, env)? != 0 {
        return None;
    }
    let dst = endpoint(write.dst)?;
    let dst_info = env.arrays.get(&dst.base)?;
    if !dst_info.mutable || !zero_fill_type(dst_info.elem_size) {
        return None;
    }
    let count_bytes = count_value(write.count, env)?;
    if count_bytes % dst_info.elem_size != 0 {
        return None;
    }
    let len = count_bytes / dst_info.elem_size;
    if len == 0 || dst.start.checked_add(len)? > dst_info.len {
        return None;
    }
    Some(CopyPlan {
        expr: fill_zero(&dst.base, dst.start, len, dst_info.len),
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

fn endpoint(expr: &Expr) -> Option<CopyEndpoint> {
    match peel_casts(expr) {
        Expr::ArrayPtr { array, .. } => {
            let Expr::Var(base) = &**array else {
                return None;
            };
            Some(CopyEndpoint {
                base: base.to_string(),
                start: 0,
            })
        }
        Expr::MethodCall { recv, method, args } if method == "add" && args.len() == 1 => {
            let mut endpoint = endpoint(recv)?;
            endpoint.start = endpoint
                .start
                .checked_add(nonnegative(int_value(&args[0])?)?)?;
            Some(endpoint)
        }
        Expr::MethodCall { recv, method, args } if method == "as_mut_ptr" && args.is_empty() => {
            let Expr::Var(base) = &**recv else {
                return None;
            };
            Some(CopyEndpoint {
                base: base.to_string(),
                start: 0,
            })
        }
        Expr::MethodCall { recv, method, args } if method == "as_ptr" && args.is_empty() => {
            let Expr::Var(base) = &**recv else {
                return None;
            };
            Some(CopyEndpoint {
                base: base.to_string(),
                start: 0,
            })
        }
        _ => None,
    }
}

fn peel_casts(expr: &Expr) -> &Expr {
    match expr {
        Expr::Cast { expr, .. } => peel_casts(expr),
        _ => expr,
    }
}

fn count_value(expr: &Expr, env: &CopyEnv) -> Option<u64> {
    match peel_casts(expr) {
        Expr::Var(name) => env.constants.get(name.as_str()).copied(),
        expr => int_value(expr).and_then(nonnegative),
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

fn fill_zero(base: &str, start: u64, len: u64, array_len: u64) -> Expr {
    let recv = if start == 0 && len == array_len {
        Expr::Var(base.into())
    } else {
        slice_index(base, start, start + len)
    };
    Expr::MethodCall {
        recv: Box::new(recv),
        method: "fill".into(),
        args: vec![int_zero()],
    }
}

fn slice_index(base: &str, start: u64, end: u64) -> Expr {
    Expr::Index {
        base: Box::new(Expr::Var(base.into())),
        index: Box::new(range(start, end)),
    }
}

fn range(start: u64, end: u64) -> Expr {
    Expr::Range {
        start: Box::new(uint(start)),
        end: Box::new(uint(end)),
    }
}

fn uint(value: u64) -> Expr {
    Expr::Value(RustValue::I64(value as i64))
}

fn int_zero() -> Expr {
    Expr::Value(RustValue::I64(0))
}

fn int_value(expr: &Expr) -> Option<i128> {
    match expr {
        Expr::Value(RustValue::I64(value)) => Some(*value as i128),
        Expr::Value(RustValue::I128(value)) => Some(*value),
        Expr::Value(RustValue::Usize(value)) => Some(*value as i128),
        Expr::Cast { expr, .. } => int_value(expr),
        Expr::Binary { op, lhs, rhs } => {
            let lhs = int_value(lhs)?;
            let rhs = int_value(rhs)?;
            Some(match op {
                BinOp::Add => lhs.checked_add(rhs)?,
                BinOp::Sub => lhs.checked_sub(rhs)?,
                BinOp::Mul => lhs.checked_mul(rhs)?,
                _ => return None,
            })
        }
        _ => None,
    }
}

fn nonnegative(value: i128) -> Option<u64> {
    (value >= 0).then_some(value as u64)
}

fn type_size(ty: &Type) -> Option<u64> {
    match ty {
        Type::Prim(Prim::I8 | Prim::U8 | Prim::Bool) => Some(1),
        Type::Prim(Prim::I16 | Prim::U16) => Some(2),
        Type::Prim(Prim::I32 | Prim::U32) => Some(4),
        Type::Prim(Prim::I64 | Prim::U64) => Some(8),
        Type::Prim(Prim::I128 | Prim::U128) => Some(16),
        _ => None,
    }
}

fn zero_fill_type(elem_size: u64) -> bool {
    matches!(elem_size, 1 | 2 | 4 | 8 | 16)
}
