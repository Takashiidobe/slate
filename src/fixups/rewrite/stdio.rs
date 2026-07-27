use std::collections::{BTreeMap, BTreeSet};

use crate::fixups::facts::file_ownership::{buf_ptr_var, match_gets_loop};
use crate::fixups::facts::{
    AstPath, FileOpenMode, FileOwnershipFact, FileUseKind, FixupFacts, FunctionId, PathSegment,
};
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, fact, function_path_location, stmts_snippet,
};
use crate::function_identity::{Known, known_call};
use crate::rust_ast::{
    BinOp, Block, Expr, Ident, IndentStmt, Item, Program, RustValue, Stmt, Type,
};

pub(in crate::fixups) fn fixup(program: &mut Program, facts: &FixupFacts) -> bool {
    let mut logger = crate::fixups::trace::NoopLogger;
    Stdio::new(&mut logger).fixup(program, facts)
}

pub(in crate::fixups) struct Stdio<'a> {
    logger: &'a mut dyn TraceLogger,
}

impl<'a> Stdio<'a> {
    pub(in crate::fixups) fn new(logger: &'a mut dyn TraceLogger) -> Self {
        Self { logger }
    }

    pub(in crate::fixups) fn fixup(&mut self, program: &mut Program, facts: &FixupFacts) -> bool {
        fixup_impl(program, facts, self.logger)
    }
}

fn fixup_impl(program: &mut Program, facts: &FixupFacts, logger: &mut dyn TraceLogger) -> bool {
    let mut changed = false;
    for (item_index, item) in program.items.iter_mut().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        let before = logger.is_enabled().then(|| f.body.clone());
        let function_changed = fixup_body(&mut f.body, function, facts);
        if function_changed && let Some(before) = before {
            let uses = facts
                .file_ownership
                .iter()
                .filter(|fact| fact.function == function)
                .map(|fact| fact.uses.len())
                .sum::<usize>();
            logger.rewrite(RewriteEvent {
                pass: TracePass::Stdio,
                kind: "rewrite_stdio_file_ownership".into(),
                location: function_path_location(facts, function, &[]),
                before: vec![stmts_snippet("body", &before)],
                after: vec![stmts_snippet("body", &f.body)],
                facts: vec![fact("file_uses", uses.to_string())],
            });
        }
        changed |= function_changed;
    }
    changed
}

fn fixup_body(body: &mut Vec<IndentStmt>, function: FunctionId, facts: &FixupFacts) -> bool {
    let plans = facts
        .file_ownership
        .iter()
        .filter(|fact| fact.function == function)
        .filter_map(|fact| plan_for_fact(body, facts, fact))
        .collect::<Vec<_>>();
    if plans.is_empty() {
        return false;
    }

    // Plans are computed against the original `body`'s indices; merging them into a
    // single pass (rather than mutating `body` once per plan) avoids one plan's removals
    // shifting the indices a later plan relies on.
    let mut replacements = BTreeMap::new();
    let mut remove = BTreeSet::new();
    for plan in plans {
        replacements.extend(plan.replacements);
        remove.extend(plan.remove);
    }

    let new_body = body
        .drain(..)
        .enumerate()
        .filter_map(|(index, indent)| {
            if remove.contains(&index) {
                return None;
            }
            match replacements.remove(&index) {
                Some(stmt) => Some(IndentStmt { stmt, ..indent }),
                None => Some(indent),
            }
        })
        .collect();
    *body = new_body;
    true
}

struct Plan {
    replacements: BTreeMap<usize, Stmt>,
    remove: BTreeSet<usize>,
}

fn plan_for_fact(
    body: &[IndentStmt],
    facts: &FixupFacts,
    fact: &FileOwnershipFact,
) -> Option<Plan> {
    if !fact.uses.iter().all(|use_| {
        matches!(
            use_.kind,
            FileUseKind::Puts
                | FileUseKind::Close
                | FileUseKind::Gets
                | FileUseKind::Read
                | FileUseKind::Write
        )
    }) {
        return None;
    }
    let handle = facts.binding_name(fact.handle)?;
    let handle_index = stmt_index(&fact.handle_path)?;
    let open_index = stmt_index(&fact.open_path)?;
    let assign_index = stmt_index(&fact.assign_path)?;
    let guard_index = assign_index + 1;
    let path = fopen_path_literal(&body.get(open_index)?.stmt)?;
    let mode = fact.mode?;
    let guard_body = open_failure_body(&body.get(guard_index)?.stmt)?;
    let buffered = fact.uses.iter().any(|use_| use_.kind == FileUseKind::Gets);

    let mut replacements = BTreeMap::new();
    replacements.insert(
        open_index,
        Stmt::Let {
            name: handle.to_owned(),
            mutable: true,
            ty: None,
            init: Some(open_file_expr(path, mode, guard_body, buffered)),
        },
    );

    let mut remove = BTreeSet::from([handle_index, assign_index, guard_index]);
    let aliases = BTreeSet::from([handle.to_string()]);
    for use_ in &fact.uses {
        let use_index = stmt_index(&use_.path)?;
        match use_.kind {
            FileUseKind::Puts => {
                let bytes = fputs_literal(&body.get(use_index)?.stmt)?;
                replacements.insert(use_index, write_all_stmt(handle, bytes));
                if previous_aliases_handle(body, use_index, handle) {
                    remove.insert(use_index - 1);
                }
            }
            FileUseKind::Close => {
                if use_index == body.len() - 1 {
                    remove.insert(use_index);
                } else {
                    replacements.insert(use_index, drop_stmt(handle));
                }
                if previous_aliases_handle(body, use_index, handle) {
                    remove.insert(use_index - 1);
                }
            }
            FileUseKind::Gets => {
                let Stmt::Loop {
                    body: loop_body, ..
                } = &body.get(use_index)?.stmt
                else {
                    return None;
                };
                let gets = match_gets_loop(loop_body, &aliases)?;
                replacements.insert(use_index, gets_loop_stmt(handle, &gets));
            }
            FileUseKind::Read => {
                let (result, args) = read_write_call(&body.get(use_index)?.stmt, Known::FRead)?;
                let buf_name = buf_ptr_var(&args[0])?;
                let expr = fread_expr(handle, &buf_name, &args[1], &args[2]);
                replacements.insert(use_index, bind_result(result, expr));
                if previous_aliases_handle(body, use_index, handle) {
                    remove.insert(use_index - 1);
                }
            }
            FileUseKind::Write => {
                let (result, args) = read_write_call(&body.get(use_index)?.stmt, Known::FWrite)?;
                let buf_name = buf_ptr_var(&args[0])?;
                let expr = fwrite_expr(handle, &buf_name, &args[1], &args[2]);
                replacements.insert(use_index, bind_result(result, expr));
                if previous_aliases_handle(body, use_index, handle) {
                    remove.insert(use_index - 1);
                }
            }
        }
    }
    Some(Plan {
        replacements,
        remove,
    })
}

fn open_file_expr(
    path: String,
    mode: FileOpenMode,
    guard_body: Vec<IndentStmt>,
    buffered: bool,
) -> Expr {
    let open_expr = Expr::MethodCall {
        recv: Box::new(Expr::MethodCall {
            recv: Box::new(open_options(mode)),
            method: "open".into(),
            args: vec![Expr::Str(path)],
        }),
        method: "unwrap_or_else".into(),
        args: vec![Expr::Closure {
            params: vec![Ident::new("__slate_err")],
            body: Box::new(Expr::Block(Box::new(Block {
                stmts: guard_body,
                tail: Some(Box::new(Expr::Macro {
                    name: "unreachable".into(),
                    args: Vec::new(),
                })),
            }))),
        }],
    };
    if buffered {
        Expr::Call {
            binding: crate::function_identity::CallBinding::Generated,
            func: Box::new(Expr::Var("std::io::BufReader::new".into())),
            args: vec![open_expr],
        }
    } else {
        open_expr
    }
}

fn open_options(mode: FileOpenMode) -> Expr {
    let mut expr = Expr::Call {
        binding: crate::function_identity::CallBinding::Generated,
        func: Box::new(Expr::Var("std::fs::OpenOptions::new".into())),
        args: Vec::new(),
    };
    for (method, value) in mode_options(mode) {
        expr = Expr::MethodCall {
            recv: Box::new(expr),
            method: (*method).into(),
            args: vec![Expr::Value(RustValue::Bool(*value))],
        };
    }
    expr
}

fn mode_options(mode: FileOpenMode) -> &'static [(&'static str, bool)] {
    match mode {
        FileOpenMode::Read => &[("read", true)],
        FileOpenMode::Write => &[("write", true), ("create", true), ("truncate", true)],
        FileOpenMode::Append => &[("append", true), ("create", true)],
        FileOpenMode::ReadUpdate => &[("read", true), ("write", true)],
        FileOpenMode::WriteUpdate => &[
            ("read", true),
            ("write", true),
            ("create", true),
            ("truncate", true),
        ],
        FileOpenMode::AppendUpdate => &[("read", true), ("append", true), ("create", true)],
    }
}

fn write_all_stmt(handle: &str, bytes: Vec<u8>) -> Stmt {
    Stmt::Expr(Expr::MethodCall {
        recv: Box::new(Expr::Call {
            binding: crate::function_identity::CallBinding::Generated,
            func: Box::new(Expr::Var("std::io::Write::write_all".into())),
            args: vec![
                Expr::Ref {
                    mutable: true,
                    expr: Box::new(Expr::Var(handle.into())),
                },
                Expr::ByteStr(bytes),
            ],
        }),
        method: "unwrap".into(),
        args: Vec::new(),
    })
}

fn drop_stmt(handle: &str) -> Stmt {
    Stmt::Expr(Expr::Call {
        binding: crate::function_identity::CallBinding::Generated,
        func: Box::new(Expr::Var("drop".into())),
        args: vec![Expr::Var(handle.into())],
    })
}

fn gets_loop_stmt(
    handle: &str,
    gets: &crate::fixups::facts::file_ownership::GetsLoopMatch,
) -> Stmt {
    let buf_name = gets.buf_name.as_str();
    let read_len = Stmt::Let {
        name: "_n".to_string(),
        mutable: false,
        ty: None,
        init: Some(read_until_call(handle, buf_name, gets.buf_len)),
    };
    let body = vec![
        IndentStmt {
            depth: 0,
            stmt: Stmt::Let {
                name: buf_name.to_string(),
                mutable: true,
                ty: Some(Type::Generic {
                    name: "Vec".into(),
                    args: vec![Type::Prim(crate::rust_ast::Prim::U8)],
                }),
                init: Some(Expr::Call {
                    binding: crate::function_identity::CallBinding::Generated,
                    func: Box::new(Expr::Var("Vec::new".into())),
                    args: Vec::new(),
                }),
            },
        },
        IndentStmt {
            depth: 0,
            stmt: read_len,
        },
        IndentStmt {
            depth: 0,
            stmt: Stmt::If {
                cond: Expr::Binary {
                    op: crate::rust_ast::BinOp::Eq,
                    lhs: Box::new(Expr::Var("_n".into())),
                    rhs: Box::new(Expr::Value(RustValue::Usize(0))),
                },
                then_body: vec![IndentStmt {
                    depth: 0,
                    stmt: Stmt::Break(None),
                }],
                else_body: Vec::new(),
            },
        },
        IndentStmt {
            depth: 0,
            stmt: write_stdout_stmt(buf_name),
        },
    ];
    Stmt::Loop { label: None, body }
}

fn read_until_call(handle: &str, buf_name: &str, buf_len: i64) -> Expr {
    let take_expr = Expr::Call {
        binding: crate::function_identity::CallBinding::Generated,
        func: Box::new(Expr::Var("std::io::Read::take".into())),
        args: vec![
            Expr::Ref {
                mutable: true,
                expr: Box::new(Expr::Var(handle.into())),
            },
            Expr::Cast {
                expr: Box::new(Expr::Value(RustValue::I64(buf_len - 1))),
                ty: Type::parse("u64"),
            },
        ],
    };
    Expr::MethodCall {
        recv: Box::new(Expr::Call {
            binding: crate::function_identity::CallBinding::Generated,
            func: Box::new(Expr::Var("std::io::BufRead::read_until".into())),
            args: vec![
                Expr::Ref {
                    mutable: true,
                    expr: Box::new(take_expr),
                },
                newline_byte(),
                Expr::Ref {
                    mutable: true,
                    expr: Box::new(Expr::Var(buf_name.into())),
                },
            ],
        }),
        method: "unwrap".into(),
        args: Vec::new(),
    }
}

fn newline_byte() -> Expr {
    Expr::Cast {
        expr: Box::new(Expr::Value(RustValue::I64(10))),
        ty: Type::parse("u8"),
    }
}

fn write_stdout_stmt(buf_name: &str) -> Stmt {
    Stmt::Expr(Expr::MethodCall {
        recv: Box::new(Expr::Call {
            binding: crate::function_identity::CallBinding::Generated,
            func: Box::new(Expr::Var("std::io::Write::write_all".into())),
            args: vec![
                Expr::Ref {
                    mutable: true,
                    expr: Box::new(Expr::Call {
                        binding: crate::function_identity::CallBinding::Generated,
                        func: Box::new(Expr::Var("std::io::stdout".into())),
                        args: Vec::new(),
                    }),
                },
                Expr::Ref {
                    mutable: false,
                    expr: Box::new(Expr::Var(buf_name.into())),
                },
            ],
        }),
        method: "unwrap".into(),
        args: Vec::new(),
    })
}

type ReadWriteResult = Option<(String, bool, Option<Type>)>;

fn read_write_call(stmt: &Stmt, callee: Known) -> Option<(ReadWriteResult, &[Expr])> {
    match stmt {
        Stmt::Expr(expr) => Some((None, call_args(expr, callee)?)),
        Stmt::Let {
            name,
            mutable,
            ty,
            init: Some(expr),
        } => Some((
            Some((name.clone(), *mutable, ty.clone())),
            call_args(expr, callee)?,
        )),
        _ => None,
    }
}

fn call_args(expr: &Expr, callee: Known) -> Option<&[Expr]> {
    let call = unsafe_tail(expr)?;
    let Expr::Call { args, .. } = call else {
        return None;
    };
    if known_call(call) != Some(callee) || args.len() != 4 {
        return None;
    }
    Some(args)
}

fn bind_result(result: ReadWriteResult, expr: Expr) -> Stmt {
    match result {
        Some((name, mutable, ty)) => Stmt::Let {
            name,
            mutable,
            ty,
            init: Some(expr),
        },
        None => Stmt::Expr(expr),
    }
}

fn read_write_cap(size: &Expr, nmemb: &Expr) -> Expr {
    Expr::Binary {
        op: BinOp::Mul,
        lhs: Box::new(Expr::Cast {
            expr: Box::new(size.clone()),
            ty: Type::parse("usize"),
        }),
        rhs: Box::new(Expr::Cast {
            expr: Box::new(nmemb.clone()),
            ty: Type::parse("usize"),
        }),
    }
}

/// fwrite's source buffer is `[i8; N]`; `Write::write_all` needs `&[u8]`, so we build an
/// unsafe byte-slice view over the same bytes rather than copying them.
fn byte_slice_expr(buf_name: &str, cap: Expr) -> Expr {
    Expr::Unsafe(Box::new(Block {
        stmts: Vec::new(),
        tail: Some(Box::new(Expr::Call {
            binding: crate::function_identity::CallBinding::Generated,
            func: Box::new(Expr::Var("std::slice::from_raw_parts".into())),
            args: vec![
                Expr::Cast {
                    expr: Box::new(Expr::MethodCall {
                        recv: Box::new(Expr::Var(buf_name.into())),
                        method: "as_ptr".into(),
                        args: Vec::new(),
                    }),
                    ty: Type::parse("*const u8"),
                },
                cap,
            ],
        })),
    }))
}

fn fwrite_expr(handle: &str, buf_name: &str, size: &Expr, nmemb: &Expr) -> Expr {
    let cap = read_write_cap(size, nmemb);
    let slice = byte_slice_expr(buf_name, cap);
    let write_call = Stmt::Expr(Expr::MethodCall {
        recv: Box::new(Expr::Call {
            binding: crate::function_identity::CallBinding::Generated,
            func: Box::new(Expr::Var("std::io::Write::write_all".into())),
            args: vec![
                Expr::Ref {
                    mutable: true,
                    expr: Box::new(Expr::Var(handle.into())),
                },
                slice,
            ],
        }),
        method: "unwrap".into(),
        args: Vec::new(),
    });
    Expr::Block(Box::new(Block {
        stmts: vec![IndentStmt {
            depth: 0,
            stmt: write_call,
        }],
        tail: Some(Box::new(Expr::Cast {
            expr: Box::new(nmemb.clone()),
            ty: Type::parse("u64"),
        })),
    }))
}

/// fread's item-count return only counts whole items; a short read still deposits its
/// partial trailing bytes into the destination buffer, so we read into a `Vec` (which
/// tolerates short reads via `read_to_end`) and copy back only the bytes actually read.
fn fread_expr(handle: &str, buf_name: &str, size: &Expr, nmemb: &Expr) -> Expr {
    let cap = read_write_cap(size, nmemb);
    let read_buf_let = Stmt::Let {
        name: "_read_buf".into(),
        mutable: true,
        ty: Some(Type::parse("Vec<u8>")),
        init: Some(Expr::Call {
            binding: crate::function_identity::CallBinding::Generated,
            func: Box::new(Expr::Var("Vec::new".into())),
            args: Vec::new(),
        }),
    };
    let take_expr = Expr::Call {
        binding: crate::function_identity::CallBinding::Generated,
        func: Box::new(Expr::Var("std::io::Read::take".into())),
        args: vec![
            Expr::Ref {
                mutable: true,
                expr: Box::new(Expr::Var(handle.into())),
            },
            Expr::Cast {
                expr: Box::new(cap),
                ty: Type::parse("u64"),
            },
        ],
    };
    let read_to_end_call = Expr::MethodCall {
        recv: Box::new(Expr::Call {
            binding: crate::function_identity::CallBinding::Generated,
            func: Box::new(Expr::Var("std::io::Read::read_to_end".into())),
            args: vec![
                Expr::Ref {
                    mutable: true,
                    expr: Box::new(take_expr),
                },
                Expr::Ref {
                    mutable: true,
                    expr: Box::new(Expr::Var("_read_buf".into())),
                },
            ],
        }),
        method: "unwrap".into(),
        args: Vec::new(),
    };
    let n_let = Stmt::Let {
        name: "_n".into(),
        mutable: false,
        ty: None,
        init: Some(read_to_end_call),
    };
    let copy_stmt = Stmt::Expr(Expr::Unsafe(Box::new(Block {
        stmts: Vec::new(),
        tail: Some(Box::new(Expr::Call {
            binding: crate::function_identity::CallBinding::Generated,
            func: Box::new(Expr::Var("std::ptr::copy_nonoverlapping".into())),
            args: vec![
                Expr::MethodCall {
                    recv: Box::new(Expr::Var("_read_buf".into())),
                    method: "as_ptr".into(),
                    args: Vec::new(),
                },
                Expr::Cast {
                    expr: Box::new(Expr::MethodCall {
                        recv: Box::new(Expr::Var(buf_name.into())),
                        method: "as_mut_ptr".into(),
                        args: Vec::new(),
                    }),
                    ty: Type::parse("*mut u8"),
                },
                Expr::Var("_n".into()),
            ],
        })),
    })));
    let item_count = Expr::Cast {
        expr: Box::new(Expr::Binary {
            op: BinOp::Div,
            lhs: Box::new(Expr::Var("_n".into())),
            rhs: Box::new(Expr::Cast {
                expr: Box::new(size.clone()),
                ty: Type::parse("usize"),
            }),
        }),
        ty: Type::parse("u64"),
    };
    Expr::Block(Box::new(Block {
        stmts: vec![
            IndentStmt {
                depth: 0,
                stmt: read_buf_let,
            },
            IndentStmt {
                depth: 0,
                stmt: n_let,
            },
            IndentStmt {
                depth: 0,
                stmt: copy_stmt,
            },
        ],
        tail: Some(Box::new(item_count)),
    }))
}

fn open_failure_body(stmt: &Stmt) -> Option<Vec<IndentStmt>> {
    match stmt {
        Stmt::Scope { body } if body.len() == 2 => {
            let Stmt::If {
                then_body,
                else_body,
                ..
            } = &body[1].stmt
            else {
                return None;
            };
            else_body.is_empty().then_some(then_body.clone())
        }
        Stmt::If {
            then_body,
            else_body,
            ..
        } if else_body.is_empty() => Some(then_body.clone()),
        _ => None,
    }
}

fn fopen_path_literal(stmt: &Stmt) -> Option<String> {
    let Stmt::Let {
        init: Some(init), ..
    } = stmt
    else {
        return None;
    };
    let call = unsafe_tail(init)?;
    let Expr::Call { args, .. } = call else {
        return None;
    };
    if known_call(call) != Some(Known::FOpen) || args.len() != 2 {
        return None;
    }
    let bytes = c_string_arg(&args[0])?;
    String::from_utf8(bytes).ok()
}

fn fputs_literal(stmt: &Stmt) -> Option<Vec<u8>> {
    let expr = match stmt {
        Stmt::Expr(expr) => expr,
        Stmt::Let {
            init: Some(expr), ..
        } => expr,
        _ => return None,
    };
    let call = unsafe_tail(expr)?;
    let Expr::Call { args, .. } = call else {
        return None;
    };
    if known_call(call) != Some(Known::FPuts) || args.len() != 2 {
        return None;
    }
    c_string_arg(&args[0])
}

fn c_string_arg(expr: &Expr) -> Option<Vec<u8>> {
    match expr {
        Expr::CStr(bytes) => Some(bytes.clone()),
        Expr::ByteStr(bytes) => Some(bytes.strip_suffix(&[0]).unwrap_or(bytes).to_vec()),
        Expr::Str(s) => Some(s.as_bytes().to_vec()),
        Expr::Cast { expr, .. } => c_string_arg(expr),
        Expr::MethodCall { recv, method, args } if method == "as_ptr" && args.is_empty() => {
            c_string_arg(recv)
        }
        _ => None,
    }
}

fn unsafe_tail(expr: &Expr) -> Option<&Expr> {
    match expr {
        Expr::Unsafe(block) if block.stmts.is_empty() => block.tail.as_deref(),
        _ => Some(expr),
    }
}

fn previous_aliases_handle(body: &[IndentStmt], index: usize, handle: &str) -> bool {
    let Some(prev_index) = index.checked_sub(1) else {
        return false;
    };
    matches!(
        &body.get(prev_index).map(|indent| &indent.stmt),
        Some(Stmt::Let {
            init: Some(Expr::Var(source)),
            ..
        }) if source.as_str() == handle
    )
}

fn stmt_index(path: &AstPath) -> Option<usize> {
    match path.0.as_slice() {
        [PathSegment::Stmt(index)] => Some(*index),
        _ => None,
    }
}
