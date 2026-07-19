use std::collections::BTreeMap;

use crate::fixups::facts::PathSegment;
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, fact, named_path_location, path_fact,
    stmt_snippet,
};
use crate::rust_ast::{BinOp, Expr, IndentStmt, Prim, RustValue, Stmt, Type};

pub(in crate::fixups) fn fixup(body: &mut Vec<IndentStmt>) -> bool {
    let mut logger = crate::fixups::trace::NoopLogger;
    PtrCopy::new("<unknown>", &mut logger).fixup(body)
}

pub(in crate::fixups) struct PtrCopy<'a> {
    function_name: String,
    logger: &'a mut dyn TraceLogger,
}

impl<'a> PtrCopy<'a> {
    pub(in crate::fixups) fn new(
        function_name: impl Into<String>,
        logger: &'a mut dyn TraceLogger,
    ) -> Self {
        Self {
            function_name: function_name.into(),
            logger,
        }
    }

    pub(in crate::fixups) fn fixup(&mut self, body: &mut Vec<IndentStmt>) -> bool {
        self.fixup_at(body, &mut Vec::new())
    }

    fn fixup_at(&mut self, body: &mut Vec<IndentStmt>, path: &mut Vec<PathSegment>) -> bool {
        let env = CopyEnv::from_body(body);
        for index in 0..body.len() {
            let mut changed = false;
            walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
                walk::nested_body_vecs_mut_with_path(
                    &mut body[index].stmt,
                    path,
                    &mut |body, path| {
                        if !changed {
                            changed = self.fixup_at(body, path);
                        }
                    },
                );
            });
            if changed {
                return true;
            }

            let before = self.logger.is_enabled().then(|| body[index].stmt.clone());
            if let Some(plan) = copy_plan(&body[index].stmt, &env) {
                body[index].stmt = Stmt::Expr(plan.expr);
                if let Some(before) = before {
                    self.log_rewrite(path, index, before, &body[index].stmt, plan.kind);
                }
                return true;
            }
        }
        false
    }

    fn log_rewrite(
        &mut self,
        path: &[PathSegment],
        index: usize,
        before: Stmt,
        after: &Stmt,
        kind: CopyRewriteKind,
    ) {
        let mut stmt_path = path.to_vec();
        stmt_path.push(PathSegment::Stmt(index));
        self.logger.rewrite(RewriteEvent {
            pass: TracePass::PtrCopy,
            kind: "rewrite_pointer_copy".into(),
            location: named_path_location(self.function_name.clone(), &stmt_path),
            before: vec![stmt_snippet("copy", &before)],
            after: vec![stmt_snippet("copy", after)],
            facts: vec![
                path_fact("stmt_path", &stmt_path),
                fact(
                    "rewrite",
                    match kind {
                        CopyRewriteKind::CopyWithin => "copy_within",
                        CopyRewriteKind::CopyFromSlice => "copy_from_slice",
                    },
                ),
            ],
        });
    }
}

#[derive(Clone)]
struct CopyEnv {
    arrays: BTreeMap<String, ArrayInfo>,
    constants: BTreeMap<String, u64>,
}

#[derive(Clone)]
struct ArrayInfo {
    mutable: bool,
    len: u64,
    elem_size: u64,
}

#[derive(Clone)]
struct CopyPlan {
    expr: Expr,
    kind: CopyRewriteKind,
}

#[derive(Clone, Copy)]
enum CopyRewriteKind {
    CopyWithin,
    CopyFromSlice,
}

#[derive(Clone)]
struct CopyEndpoint {
    base: String,
    start: u64,
}

impl CopyEnv {
    fn from_body(body: &[IndentStmt]) -> Self {
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
            if let Some(Type::Array { elem, len }) = ty
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

fn copy_plan(stmt: &Stmt, env: &CopyEnv) -> Option<CopyPlan> {
    let copy = ptr_copy_stmt(stmt)?;
    let src = endpoint(&copy.src)?;
    let dst = endpoint(&copy.dst)?;
    let src_info = env.arrays.get(&src.base)?;
    let dst_info = env.arrays.get(&dst.base)?;
    if !dst_info.mutable || src_info.elem_size != dst_info.elem_size {
        return None;
    }
    let count_bytes = count_value(&copy.count, env)?;
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
            kind: CopyRewriteKind::CopyWithin,
        });
    }
    Some(CopyPlan {
        expr: copy_from_slice(&dst.base, dst.start, len, &src.base, src.start),
        kind: CopyRewriteKind::CopyFromSlice,
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

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Block, Item, Program};

    fn after(stmts: Vec<Stmt>) -> String {
        let mut program = Program {
            items: vec![Item::Fn(func(vec![], None, stmts))],
        };
        let Item::Fn(f) = &mut program.items[0] else {
            unreachable!();
        };
        while fixup(&mut f.body) {}
        program.emit()
    }

    fn array(name: &str, ty: &str, len: usize) -> Stmt {
        Stmt::Let {
            name: name.into(),
            mutable: true,
            ty: Some(Type::parse(ty)),
            init: Some(Expr::ArrayRepeat {
                elem: Box::new(int(0)),
                len,
            }),
        }
    }

    fn ptr_copy(src: Expr, dst: Expr, count: Expr) -> Stmt {
        Stmt::Expr(Expr::Unsafe(Box::new(Block {
            stmts: vec![],
            tail: Some(Box::new(Expr::PtrCopy {
                src: Box::new(src),
                dst: Box::new(dst),
                count: Box::new(count),
                overlapping: true,
            })),
        })))
    }

    fn as_mut_ptr(name: &str) -> Expr {
        Expr::MethodCall {
            recv: Box::new(var(name)),
            method: "as_mut_ptr".into(),
            args: vec![],
        }
    }

    fn add(ptr: Expr, offset: i64) -> Expr {
        Expr::MethodCall {
            recv: Box::new(ptr),
            method: "add".into(),
            args: vec![int(offset)],
        }
    }

    #[test]
    fn rewrites_same_buffer_copy_to_copy_within() {
        let out = after(vec![
            array("buf", "[i8; 8]", 8),
            temp("_len", "u64", int(4)),
            ptr_copy(as_mut_ptr("buf"), add(as_mut_ptr("buf"), 2), var("_len")),
        ]);

        assert!(out.contains("buf.copy_within(0..4, 2);"), "{out}");
        assert!(!out.contains("std::ptr::copy"), "{out}");
    }

    #[test]
    fn rewrites_distinct_buffers_to_copy_from_slice() {
        let out = after(vec![
            array("src", "[u16; 8]", 8),
            array("dst", "[u16; 8]", 8),
            ptr_copy(add(as_mut_ptr("src"), 1), add(as_mut_ptr("dst"), 2), int(6)),
        ]);

        assert!(
            out.contains("dst[(2..5)].copy_from_slice(&src[(1..4)]);"),
            "{out}"
        );
        assert!(!out.contains("std::ptr::copy"), "{out}");
    }

    #[test]
    fn keeps_unaligned_byte_count_as_pointer_copy() {
        let out = after(vec![
            array("src", "[u16; 8]", 8),
            array("dst", "[u16; 8]", 8),
            ptr_copy(as_mut_ptr("src"), as_mut_ptr("dst"), int(3)),
        ]);

        assert!(out.contains("std::ptr::copy"), "{out}");
        assert!(!out.contains("copy_from_slice"), "{out}");
    }
}
