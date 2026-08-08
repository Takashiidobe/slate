use crate::fixups::facts::walk;
use crate::fixups::facts::{
    self, AstPath, FunctionId, NulTermination, PathSegment, PrintfArgFact, PrintfCallFact, Site,
    StringBufferFact, StringBufferProvenance, StringBufferRejection, StringPointerViewFact,
};
use crate::function_identity::{Known, known_call};
use crate::rust_ast::{Block, Expr, FnDef, FnParam, IndentStmt, RustValue, Stmt, Type};
use ordered_float::OrderedFloat;
use std::collections::BTreeMap;
pub(in crate::fixups) fn collect_for_function(
    function: FunctionId,
    f: &FnDef,
    string_buffers: &[StringBufferFact],
    string_pointer_views: &[StringPointerViewFact],
) -> Vec<PrintfCallFact> {
    let mut calls = Vec::new();
    let mut env = PrintfEnv::from_params(&f.params);
    body(
        function,
        &f.body,
        &mut env,
        &mut Vec::new(),
        &mut calls,
        string_buffers,
        string_pointer_views,
    );
    calls
}

fn body(
    function: FunctionId,
    body: &[IndentStmt],
    env: &mut PrintfEnv,
    path: &mut Vec<PathSegment>,
    calls: &mut Vec<PrintfCallFact>,
    string_buffers: &[StringBufferFact],
    string_pointer_views: &[StringPointerViewFact],
) {
    for (index, indent) in body.iter().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            stmt(
                function,
                &indent.stmt,
                env,
                path,
                calls,
                string_buffers,
                string_pointer_views,
            );
        });
        env.update_after_stmt(&indent.stmt);
    }
}

fn visit_block(
    function: FunctionId,
    block: &Block,
    env: &PrintfEnv,
    path: &mut Vec<PathSegment>,
    calls: &mut Vec<PrintfCallFact>,
    string_buffers: &[StringBufferFact],
    string_pointer_views: &[StringPointerViewFact],
) {
    let mut block_env = env.clone();
    body(
        function,
        &block.stmts,
        &mut block_env,
        path,
        calls,
        string_buffers,
        string_pointer_views,
    );
    if let Some(tail) = &block.tail {
        walk::with_path_segment(path, PathSegment::BlockTail, |path| {
            visit_expr(
                function,
                tail,
                &block_env,
                path,
                calls,
                string_buffers,
                string_pointer_views,
            );
        });
    }
}

fn stmt(
    function: FunctionId,
    stmt: &Stmt,
    env: &PrintfEnv,
    path: &mut Vec<PathSegment>,
    calls: &mut Vec<PrintfCallFact>,
    string_buffers: &[StringBufferFact],
    string_pointer_views: &[StringPointerViewFact],
) {
    match stmt {
        Stmt::Let { init, .. } => {
            if let Some(init) = init {
                visit_expr(
                    function,
                    init,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                );
            }
        }
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            visit_expr(
                function,
                cond,
                env,
                path,
                calls,
                string_buffers,
                string_pointer_views,
            );
            let mut then_env = env.clone();
            let mut else_env = env.clone();
            walk::with_path_segment(path, PathSegment::Then, |path| {
                body(
                    function,
                    then_body,
                    &mut then_env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                );
                visit_expr(
                    function,
                    then_value,
                    &then_env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                );
            });
            walk::with_path_segment(path, PathSegment::Else, |path| {
                body(
                    function,
                    else_body,
                    &mut else_env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                );
                visit_expr(
                    function,
                    else_value,
                    &else_env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                );
            });
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            visit_expr(
                function,
                target,
                env,
                path,
                calls,
                string_buffers,
                string_pointer_views,
            );
            visit_expr(
                function,
                value,
                env,
                path,
                calls,
                string_buffers,
                string_pointer_views,
            );
        }
        Stmt::InlineAsm(_) => {}
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => visit_expr(
            function,
            expr,
            env,
            path,
            calls,
            string_buffers,
            string_pointer_views,
        ),
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            visit_expr(
                function,
                cond,
                env,
                path,
                calls,
                string_buffers,
                string_pointer_views,
            );
            let mut then_env = env.clone();
            let mut else_env = env.clone();
            walk::with_path_segment(path, PathSegment::Then, |path| {
                body(
                    function,
                    then_body,
                    &mut then_env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
            walk::with_path_segment(path, PathSegment::Else, |path| {
                body(
                    function,
                    else_body,
                    &mut else_env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
        }
        Stmt::Loop { body: nested, .. } => {
            let mut nested_env = env.clone();
            walk::with_path_segment(path, PathSegment::LoopBody, |path| {
                body(
                    function,
                    nested,
                    &mut nested_env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
        }
        Stmt::For {
            iter, body: nested, ..
        } => {
            visit_expr(
                function,
                iter,
                env,
                path,
                calls,
                string_buffers,
                string_pointer_views,
            );
            let mut nested_env = env.clone();
            walk::with_path_segment(path, PathSegment::ForBody, |path| {
                body(
                    function,
                    nested,
                    &mut nested_env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
        }
        Stmt::Scope { body: nested } => {
            let mut nested_env = env.clone();
            walk::with_path_segment(path, PathSegment::ScopeBody, |path| {
                body(
                    function,
                    nested,
                    &mut nested_env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
        }
        Stmt::LabeledBlock { body: nested, .. } => {
            let mut nested_env = env.clone();
            walk::with_path_segment(path, PathSegment::LabeledBody, |path| {
                body(
                    function,
                    nested,
                    &mut nested_env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
        }
        Stmt::Unsafe { body: nested } => {
            walk::with_path_segment(path, PathSegment::UnsafeBody, |path| {
                visit_block(
                    function,
                    nested,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
        }
        Stmt::While { cond, body: nested } => {
            visit_expr(
                function,
                cond,
                env,
                path,
                calls,
                string_buffers,
                string_pointer_views,
            );
            walk::with_path_segment(path, PathSegment::WhileBody, |path| {
                visit_block(
                    function,
                    nested,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
        }
        Stmt::Block(nested) => {
            walk::with_path_segment(path, PathSegment::BlockBody, |path| {
                visit_block(
                    function,
                    nested,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
        }
        Stmt::Match { expr: value, arms } => {
            visit_expr(
                function,
                value,
                env,
                path,
                calls,
                string_buffers,
                string_pointer_views,
            );
            for (index, arm) in arms.iter().enumerate() {
                let mut arm_env = env.clone();
                walk::with_path_segment(path, PathSegment::MatchArm(index), |path| {
                    body(
                        function,
                        &arm.body,
                        &mut arm_env,
                        path,
                        calls,
                        string_buffers,
                        string_pointer_views,
                    );
                });
            }
        }
    }
}

fn visit_expr(
    function: FunctionId,
    expr: &Expr,
    env: &PrintfEnv,
    path: &mut Vec<PathSegment>,
    calls: &mut Vec<PrintfCallFact>,
    string_buffers: &[StringBufferFact],
    string_pointer_views: &[StringPointerViewFact],
) {
    if let Expr::Call { args, .. } = expr
        && let Some(known @ (Known::Printf | Known::FPrintf | Known::SPrintf | Known::SNPrintf)) =
            known_call(expr)
    {
        let format_index = match known {
            Known::FPrintf | Known::SPrintf => 1,
            Known::SNPrintf => 2,
            _ => 0,
        };
        let arg_facts = args
            .iter()
            .enumerate()
            .skip(format_index + 1)
            .map(|(index, arg)| {
                let mut arg_path = path.to_vec();
                arg_path.push(PathSegment::Expr(index + 1));
                PrintfArgFact {
                    path: AstPath(arg_path),
                    const_string: const_c_string_arg(arg, env).or_else(|| {
                        let mut arg_path = path.to_vec();
                        arg_path.push(PathSegment::Expr(index + 1));
                        proven_local_c_string_arg(
                            function,
                            string_buffers,
                            string_pointer_views,
                            &arg_path,
                        )
                    }),
                    const_char: const_c_char_arg(arg, env),
                    const_float: const_float_arg(arg, env),
                    rust_string: is_rust_string_arg(arg, env),
                    pointer: is_printf_pointer_arg(arg, env),
                }
            })
            .collect();
        calls.push(PrintfCallFact {
            site: Site {
                function,
                path: AstPath(path.to_vec()),
            },
            format: args.get(format_index).and_then(const_c_string),
            arg_paths: (format_index + 1..args.len())
                .map(|index| {
                    let mut arg_path = path.to_vec();
                    arg_path.push(PathSegment::Expr(index + 1));
                    AstPath(arg_path)
                })
                .collect(),
            arg_facts,
        });
    }

    match expr {
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::CStr(_)
        | Expr::Var(_)
        | Expr::Path(_)
        | Expr::AtomicFence { .. }
        | Expr::Todo(_) => {}
        Expr::Unary { expr: inner, .. }
        | Expr::Cast { expr: inner, .. }
        | Expr::Ref { expr: inner, .. }
        | Expr::AddrOf { expr: inner, .. }
        | Expr::Transmute { expr: inner, .. } => {
            walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                visit_expr(
                    function,
                    inner,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
        }
        Expr::Binary { lhs, rhs, .. } => {
            walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                visit_expr(
                    function,
                    lhs,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
            walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                visit_expr(
                    function,
                    rhs,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
        }
        Expr::Range { start, end } => {
            walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                visit_expr(
                    function,
                    start,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
            walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                visit_expr(
                    function,
                    end,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
        }
        Expr::Call { func, args, .. } => {
            walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                visit_expr(
                    function,
                    func,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
            for (index, arg) in args.iter().enumerate() {
                walk::with_path_segment(path, PathSegment::Expr(index + 1), |path| {
                    visit_expr(
                        function,
                        arg,
                        env,
                        path,
                        calls,
                        string_buffers,
                        string_pointer_views,
                    )
                });
            }
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                visit_expr(
                    function,
                    recv,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
            for (index, arg) in args.iter().enumerate() {
                walk::with_path_segment(path, PathSegment::Expr(index + 1), |path| {
                    visit_expr(
                        function,
                        arg,
                        env,
                        path,
                        calls,
                        string_buffers,
                        string_pointer_views,
                    )
                });
            }
        }
        Expr::Field { base, .. }
        | Expr::TupleField { base, .. }
        | Expr::ArrayPtr { array: base, .. } => {
            walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                visit_expr(
                    function,
                    base,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
        }
        Expr::Index { base, index } => {
            walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                visit_expr(
                    function,
                    base,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
            walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                visit_expr(
                    function,
                    index,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
        }
        Expr::StructLit { fields, .. } => {
            for (index, (_, value)) in fields.iter().enumerate() {
                walk::with_path_segment(path, PathSegment::Expr(index), |path| {
                    visit_expr(
                        function,
                        value,
                        env,
                        path,
                        calls,
                        string_buffers,
                        string_pointer_views,
                    )
                });
            }
        }
        Expr::TupleStructLit { fields, .. } => {
            for (index, value) in fields.iter().enumerate() {
                walk::with_path_segment(path, PathSegment::Expr(index), |path| {
                    visit_expr(
                        function,
                        value,
                        env,
                        path,
                        calls,
                        string_buffers,
                        string_pointer_views,
                    )
                });
            }
        }
        Expr::ArrayLit(elems) | Expr::VecLit(elems) | Expr::Macro { args: elems, .. } => {
            for (index, elem) in elems.iter().enumerate() {
                walk::with_path_segment(path, PathSegment::Expr(index), |path| {
                    visit_expr(
                        function,
                        elem,
                        env,
                        path,
                        calls,
                        string_buffers,
                        string_pointer_views,
                    )
                });
            }
        }
        Expr::ArrayRepeat { elem, .. } | Expr::Closure { body: elem, .. } => {
            walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                visit_expr(
                    function,
                    elem,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
        }
        Expr::VecRepeat { elem, len } => {
            walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                visit_expr(
                    function,
                    elem,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
            walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                visit_expr(
                    function,
                    len,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
        }
        Expr::Match { expr: value, arms } => {
            walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                visit_expr(
                    function,
                    value,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
            for (index, arm) in arms.iter().enumerate() {
                walk::with_path_segment(path, PathSegment::Expr(index + 1), |path| {
                    visit_expr(
                        function,
                        &arm.value,
                        env,
                        path,
                        calls,
                        string_buffers,
                        string_pointer_views,
                    )
                });
            }
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                visit_expr(
                    function,
                    cond,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
            walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                visit_expr(
                    function,
                    then_expr,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
            walk::with_path_segment(path, PathSegment::Expr(2), |path| {
                visit_expr(
                    function,
                    else_expr,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
        }
        Expr::Block(block) => {
            walk::with_path_segment(path, PathSegment::BlockBody, |path| {
                visit_block(
                    function,
                    block,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
        }
        Expr::Unsafe(block) => {
            walk::with_path_segment(path, PathSegment::UnsafeBody, |path| {
                visit_block(
                    function,
                    block,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
        }
        Expr::CopyNonoverlapping { src, dst, .. } => {
            walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                visit_expr(
                    function,
                    src,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
            walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                visit_expr(
                    function,
                    dst,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
        }
        Expr::PtrCopy {
            src, dst, count, ..
        } => {
            walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                visit_expr(
                    function,
                    src,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
            walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                visit_expr(
                    function,
                    dst,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
            walk::with_path_segment(path, PathSegment::Expr(2), |path| {
                visit_expr(
                    function,
                    count,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
        }
        Expr::WriteBytes { dst, val, count } => {
            walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                visit_expr(
                    function,
                    dst,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
            walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                visit_expr(
                    function,
                    val,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
            walk::with_path_segment(path, PathSegment::Expr(2), |path| {
                visit_expr(
                    function,
                    count,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
        }
        Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. } => {
            if let Some(ptr) = place.ptr_expr() {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    visit_expr(
                        function,
                        ptr,
                        env,
                        path,
                        calls,
                        string_buffers,
                        string_pointer_views,
                    )
                });
            }
        }
        Expr::AtomicStore { place, value, .. }
        | Expr::AtomicFetch { place, value, .. }
        | Expr::AtomicSwap { place, value, .. } => {
            if let Some(ptr) = place.ptr_expr() {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    visit_expr(
                        function,
                        ptr,
                        env,
                        path,
                        calls,
                        string_buffers,
                        string_pointer_views,
                    )
                });
            }
            walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                visit_expr(
                    function,
                    value,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
        }
        Expr::AtomicNew { value, .. } => {
            walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                visit_expr(
                    function,
                    value,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
        }
        Expr::AtomicCompareExchange {
            place,
            expected,
            desired,
            ..
        } => {
            if let Some(ptr) = place.ptr_expr() {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    visit_expr(
                        function,
                        ptr,
                        env,
                        path,
                        calls,
                        string_buffers,
                        string_pointer_views,
                    )
                });
            }
            walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                visit_expr(
                    function,
                    expected,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
            walk::with_path_segment(path, PathSegment::Expr(2), |path| {
                visit_expr(
                    function,
                    desired,
                    env,
                    path,
                    calls,
                    string_buffers,
                    string_pointer_views,
                )
            });
        }
    }
}

fn const_c_string(expr: &Expr) -> Option<Vec<u8>> {
    match expr {
        Expr::Str(s) => Some(s.as_bytes().to_vec()),
        Expr::ByteStr(bytes) => Some(bytes.strip_suffix(&[0]).unwrap_or(bytes).to_vec()),
        Expr::CStr(bytes) => Some(bytes.clone()),
        Expr::Cast { expr, .. } => const_c_string(expr),
        Expr::MethodCall { recv, method, args } if method == "as_ptr" && args.is_empty() => {
            const_c_string(recv)
        }
        Expr::Value(RustValue::NullPtr) => None,
        _ => None,
    }
}

fn const_c_string_arg(arg: &Expr, env: &PrintfEnv) -> Option<String> {
    let bytes = pointer_view_source(arg)
        .and_then(|name| env.consts.get(name))
        .and_then(const_c_string)
        .or_else(|| const_c_string(env.resolve_const(arg)))?;
    if bytes.contains(&0) {
        return None;
    }
    String::from_utf8(bytes).ok()
}

fn proven_local_c_string_arg(
    function: FunctionId,
    string_buffers: &[StringBufferFact],
    string_pointer_views: &[StringPointerViewFact],
    path: &[PathSegment],
) -> Option<String> {
    let source =
        facts::string_pointer_view(string_pointer_views, function, &AstPath(path.to_vec()))
            .map(|view| view.source)?;
    let buffer = facts::string_buffer(string_buffers, source)?;
    if buffer
        .rejections
        .contains(&StringBufferRejection::EscapedToCall)
        || matches!(
            buffer.provenance,
            StringBufferProvenance::ZeroInitialized | StringBufferProvenance::Unknown
        )
    {
        return None;
    }
    if buffer.nul_termination != NulTermination::Terminated || buffer.interior_nul {
        return None;
    }
    String::from_utf8(buffer.bytes.clone()?).ok()
}

fn pointer_view_source(expr: &Expr) -> Option<&str> {
    match expr {
        Expr::Cast { expr, .. } => pointer_view_source(expr),
        Expr::MethodCall { recv, method, args } if method == "as_ptr" && args.is_empty() => {
            match &**recv {
                Expr::Var(name) => Some(name.as_str()),
                _ => None,
            }
        }
        _ => None,
    }
}

fn const_c_char_arg(arg: &Expr, env: &PrintfEnv) -> Option<String> {
    let value = const_integer(env.resolve_const(arg))?;
    let byte = u8::try_from(value).ok()?;
    if !byte.is_ascii() {
        return None;
    }
    Some(char::from(byte).to_string())
}

fn const_integer(expr: &Expr) -> Option<i64> {
    match expr {
        Expr::Value(RustValue::I64(n)) => Some(*n),
        Expr::Value(RustValue::I128(n)) => i64::try_from(*n).ok(),
        Expr::Cast { expr, .. } => const_integer(expr),
        _ => None,
    }
}

fn const_float_arg(arg: &Expr, env: &PrintfEnv) -> Option<OrderedFloat<f64>> {
    const_float(env.resolve_const(arg))
}

fn const_float(expr: &Expr) -> Option<OrderedFloat<f64>> {
    match expr {
        Expr::Value(RustValue::Float(n)) => Some(*n),
        Expr::Cast { expr, .. } => const_float(expr),
        _ => None,
    }
}

fn is_printf_const(expr: &Expr) -> bool {
    const_integer(expr).is_some() || const_c_string(expr).is_some() || const_float(expr).is_some()
}

fn is_rust_string_arg(arg: &Expr, env: &PrintfEnv) -> bool {
    match strip_pointer_view(arg) {
        Expr::Var(name) => env
            .types
            .get(name.as_str())
            .is_some_and(type_is_rust_string),
        _ => false,
    }
}

fn strip_pointer_view(expr: &Expr) -> &Expr {
    match expr {
        Expr::Cast { expr, .. } => strip_pointer_view(expr),
        Expr::MethodCall { recv, method, args }
            if args.is_empty() && matches!(method.as_str(), "as_ptr" | "as_mut_ptr") =>
        {
            strip_pointer_view(recv)
        }
        Expr::ArrayPtr { array, .. } => strip_pointer_view(array),
        _ => expr,
    }
}

fn type_is_rust_string(ty: &Type) -> bool {
    matches!(ty, Type::Custom(name) if name == "String")
        || matches!(
            ty,
            Type::Ref {
                mutable: false,
                inner,
            } if matches!(&**inner, Type::Str)
        )
}

fn is_printf_pointer_arg(arg: &Expr, env: &PrintfEnv) -> bool {
    match arg {
        Expr::Var(name) => env.types.get(name.as_str()).is_some_and(type_is_pointer),
        Expr::Cast { ty, expr } => type_is_pointer(ty) && !is_null_pointer_source(expr),
        Expr::AddrOf { .. } | Expr::Ref { .. } | Expr::ArrayPtr { .. } => true,
        Expr::MethodCall { method, args, .. } if args.is_empty() => {
            matches!(method.as_str(), "as_ptr" | "as_mut_ptr")
        }
        _ => false,
    }
}

fn type_is_pointer(ty: &Type) -> bool {
    matches!(ty, Type::Ptr { .. })
}

fn is_null_pointer_source(expr: &Expr) -> bool {
    match expr {
        Expr::Value(RustValue::I64(0) | RustValue::I128(0) | RustValue::NullPtr) => true,
        Expr::Cast { expr, .. } => is_null_pointer_source(expr),
        _ => false,
    }
}

#[derive(Clone)]
struct PrintfEnv {
    consts: BTreeMap<String, Expr>,
    types: BTreeMap<String, Type>,
}

impl PrintfEnv {
    fn new() -> Self {
        Self {
            consts: BTreeMap::new(),
            types: BTreeMap::new(),
        }
    }

    fn from_params(params: &[FnParam]) -> Self {
        let mut env = Self::new();
        for param in params {
            env.types.insert(param.name.clone(), param.ty.clone());
        }
        env
    }

    fn resolve_const<'a>(&'a self, expr: &'a Expr) -> &'a Expr {
        match expr {
            Expr::Var(name) => self.consts.get(name.as_str()).unwrap_or(expr),
            Expr::Cast { expr, .. } => self.resolve_const(expr),
            _ => expr,
        }
    }

    fn update_after_stmt(&mut self, stmt: &Stmt) {
        match stmt {
            Stmt::Let { name, ty, .. } => {
                if let Some(ty) = ty {
                    self.types.insert(name.clone(), ty.clone());
                } else {
                    self.types.remove(name.as_str());
                }
            }
            Stmt::Assign { target, .. } | Stmt::CompoundAssign { target, .. } => {
                if let Expr::Var(name) = target {
                    self.consts.remove(name.as_str());
                }
            }
            _ => {}
        }

        match stmt {
            Stmt::Let {
                name,
                mutable: false,
                init: Some(init),
                ..
            } if is_printf_const(init) => {
                self.consts.insert(name.clone(), init.clone());
            }
            Stmt::Let { name, .. } => {
                self.consts.remove(name.as_str());
            }
            _ => {}
        }
    }
}
