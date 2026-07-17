use std::collections::{BTreeMap, BTreeSet};

use crate::fixups::facts::file_ownership::match_gets_loop;
use crate::fixups::facts::{
    AstPath, FileOpenMode, FileOwnershipFact, FileUseKind, FixupFacts, FunctionId, PathSegment,
};
use crate::rust_ast::{Block, Expr, Ident, IndentStmt, Item, Program, RustValue, Stmt, Type};

pub(in crate::fixups) fn fixup(program: &mut Program, facts: &FixupFacts) -> bool {
    let mut changed = false;
    for (item_index, item) in program.items.iter_mut().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        changed |= fixup_body(&mut f.body, function, facts);
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
            FileUseKind::Puts | FileUseKind::Close | FileUseKind::Gets
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
            FileUseKind::Read | FileUseKind::Write => return None,
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
            func: Box::new(Expr::Var("std::io::BufReader::new".into())),
            args: vec![open_expr],
        }
    } else {
        open_expr
    }
}

fn open_options(mode: FileOpenMode) -> Expr {
    let mut expr = Expr::Call {
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
                ty: Some(Type::parse("Vec<u8>")),
                init: Some(Expr::Call {
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
            func: Box::new(Expr::Var("std::io::Write::write_all".into())),
            args: vec![
                Expr::Ref {
                    mutable: true,
                    expr: Box::new(Expr::Call {
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
    let Expr::Call { func, args } = unsafe_tail(init)? else {
        return None;
    };
    if !matches!(&**func, Expr::Var(name) if name.as_str() == "fopen") || args.len() != 2 {
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
    let Expr::Call { func, args } = unsafe_tail(expr)? else {
        return None;
    };
    if !matches!(&**func, Expr::Var(name) if name.as_str() == "fputs") || args.len() != 2 {
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

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::facts;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{ExternDecl, ExternFnDecl, FnParam, Type};

    fn run(stmts: Vec<Stmt>) -> String {
        let mut program = Program {
            items: vec![
                extern_fn(
                    "fopen",
                    vec![("path", "*mut i8"), ("mode", "*mut i8")],
                    Some("*mut libc::FILE"),
                ),
                extern_fn(
                    "fputs",
                    vec![("s", "*mut i8"), ("stream", "*mut libc::FILE")],
                    Some("i32"),
                ),
                extern_fn("fclose", vec![("stream", "*mut libc::FILE")], Some("i32")),
                Item::Fn(func(vec![], None, stmts)),
            ],
        };
        let facts = facts::analyze(program.clone()).facts;
        fixup(&mut program, &facts);
        program.emit()
    }

    fn extern_fn(name: &str, params: Vec<(&str, &str)>, ret: Option<&str>) -> Item {
        Item::ExternBlock {
            abi: "C".into(),
            decls: vec![ExternDecl::Fn(ExternFnDecl {
                name: name.into(),
                params: params
                    .into_iter()
                    .map(|(name, ty)| FnParam {
                        name: name.into(),
                        mutable: false,
                        ty: Type::parse(ty),
                    })
                    .collect(),
                variadic: false,
                ret: ret.map(Type::parse),
            })],
        }
    }

    fn unsafe_call(name: &str, args: Vec<Expr>) -> Expr {
        Expr::Unsafe(Box::new(Block {
            stmts: Vec::new(),
            tail: Some(Box::new(call(name, args))),
        }))
    }

    #[test]
    fn rewrites_literal_fopen_fputs_fclose_owner() {
        let out = run(vec![
            Stmt::Let {
                name: "f".into(),
                mutable: true,
                ty: Some(Type::parse("*mut libc::FILE")),
                init: Some(Expr::Value(RustValue::NullPtr)),
            },
            temp(
                "_v0",
                "*mut libc::FILE",
                unsafe_call(
                    "fopen",
                    vec![Expr::CStr(b"out.txt".to_vec()), Expr::CStr(b"w".to_vec())],
                ),
            ),
            assign("f", var("_v0")),
            Stmt::If {
                cond: var("f"),
                then_body: vec![IndentStmt {
                    depth: 2,
                    stmt: Stmt::Expr(Expr::Call {
                        func: Box::new(Expr::Path(crate::rust_ast::Path::new(
                            ["std", "process", "exit"].into_iter().map(Ident::new),
                        ))),
                        args: vec![int(0)],
                    }),
                }],
                else_body: Vec::new(),
            },
            temp("_v1", "*mut libc::FILE", var("f")),
            Stmt::Expr(unsafe_call(
                "fputs",
                vec![Expr::CStr(b"owned\n".to_vec()), var("_v1")],
            )),
            temp("_v2", "*mut libc::FILE", var("f")),
            Stmt::Expr(unsafe_call("fclose", vec![var("_v2")])),
        ]);

        assert!(out.contains("let mut f = std::fs::OpenOptions::new().write(true).create(true).truncate(true).open(\"out.txt\").unwrap_or_else(|__slate_err|"));
        assert!(out.contains("std::io::Write::write_all(&mut f, b\"owned\\n\").unwrap();"));
        assert!(!out.contains("unsafe { fopen("));
        assert!(!out.contains("unsafe { fputs("));
        assert!(!out.contains("unsafe { fclose("));
    }

    fn cast(expr: Expr, ty: &str) -> Expr {
        Expr::Cast {
            expr: Box::new(expr),
            ty: Type::parse(ty),
        }
    }

    fn buf_ptr(name: &str) -> Expr {
        cast(
            Expr::ArrayPtr {
                array: Box::new(var(name)),
                mutable: true,
            },
            "*mut i8",
        )
    }

    fn unsafe_var(name: &str) -> Expr {
        Expr::Unsafe(Box::new(Block {
            stmts: Vec::new(),
            tail: Some(Box::new(var(name))),
        }))
    }

    fn null_break_if(result_name: &str) -> Stmt {
        Stmt::If {
            cond: Expr::Unary {
                op: crate::rust_ast::UnaryOp::Not,
                expr: Box::new(bin(
                    crate::rust_ast::BinOp::Ne,
                    var(result_name),
                    cast(Expr::Value(RustValue::NullPtr), "*mut i8"),
                )),
            },
            then_body: vec![IndentStmt {
                depth: 2,
                stmt: Stmt::Break(None),
            }],
            else_body: Vec::new(),
        }
    }

    fn gets_echo_loop(handle: &str, alias: &str, buf_name: &str) -> Stmt {
        Stmt::Loop {
            label: None,
            body: vec![
                IndentStmt {
                    depth: 2,
                    stmt: temp(alias, "*mut libc::FILE", var(handle)),
                },
                IndentStmt {
                    depth: 2,
                    stmt: temp(
                        "_vres",
                        "*mut i8",
                        unsafe_call(
                            "fgets",
                            vec![
                                buf_ptr(buf_name),
                                int(64),
                                cast(var(alias), "*mut libc::FILE"),
                            ],
                        ),
                    ),
                },
                IndentStmt {
                    depth: 2,
                    stmt: null_break_if("_vres"),
                },
                IndentStmt {
                    depth: 2,
                    stmt: Stmt::Scope {
                        body: vec![
                            IndentStmt {
                                depth: 3,
                                stmt: temp("_vout", "*mut libc::FILE", unsafe_var("stdout")),
                            },
                            IndentStmt {
                                depth: 3,
                                stmt: Stmt::Expr(unsafe_call(
                                    "fputs",
                                    vec![buf_ptr(buf_name), cast(var("_vout"), "*mut libc::FILE")],
                                )),
                            },
                        ],
                    },
                },
            ],
        }
    }

    #[test]
    fn rewrites_gets_loop_owner_into_buffered_read_until() {
        let out = run(vec![
            Stmt::Let {
                name: "f".into(),
                mutable: true,
                ty: Some(Type::parse("*mut libc::FILE")),
                init: Some(Expr::Value(RustValue::NullPtr)),
            },
            temp(
                "_v0",
                "*mut libc::FILE",
                unsafe_call(
                    "fopen",
                    vec![Expr::CStr(b"in.txt".to_vec()), Expr::CStr(b"r".to_vec())],
                ),
            ),
            assign("f", var("_v0")),
            Stmt::If {
                cond: var("f"),
                then_body: vec![IndentStmt {
                    depth: 2,
                    stmt: Stmt::Expr(Expr::Call {
                        func: Box::new(Expr::Path(crate::rust_ast::Path::new(
                            ["std", "process", "exit"].into_iter().map(Ident::new),
                        ))),
                        args: vec![int(0)],
                    }),
                }],
                else_body: Vec::new(),
            },
            Stmt::Let {
                name: "line".into(),
                mutable: true,
                ty: Some(Type::parse("[i8; 64]")),
                init: Some(Expr::Value(RustValue::I64(0))),
            },
            gets_echo_loop("f", "_v1", "line"),
            temp("_v2", "*mut libc::FILE", var("f")),
            Stmt::Expr(unsafe_call("fclose", vec![var("_v2")])),
        ]);

        assert!(out.contains("std::io::BufReader::new"), "{out}");
        assert!(
            out.contains("std::io::BufRead::read_until(&mut std::io::Read::take(&mut f, 63"),
            "{out}"
        );
        assert!(
            out.contains("std::io::Write::write_all(&mut std::io::stdout(), &line)"),
            "{out}"
        );
        assert!(!out.contains("unsafe { fgets("), "{out}");
        assert!(!out.contains("unsafe { fclose("));
    }

    #[test]
    fn rewrites_close_followed_by_more_statements_as_explicit_drop() {
        let out = run(vec![
            Stmt::Let {
                name: "f".into(),
                mutable: true,
                ty: Some(Type::parse("*mut libc::FILE")),
                init: Some(Expr::Value(RustValue::NullPtr)),
            },
            temp(
                "_v0",
                "*mut libc::FILE",
                unsafe_call(
                    "fopen",
                    vec![Expr::CStr(b"out.txt".to_vec()), Expr::CStr(b"w".to_vec())],
                ),
            ),
            assign("f", var("_v0")),
            Stmt::If {
                cond: var("f"),
                then_body: vec![IndentStmt {
                    depth: 2,
                    stmt: Stmt::Expr(Expr::Call {
                        func: Box::new(Expr::Path(crate::rust_ast::Path::new(
                            ["std", "process", "exit"].into_iter().map(Ident::new),
                        ))),
                        args: vec![int(0)],
                    }),
                }],
                else_body: Vec::new(),
            },
            temp("_v1", "*mut libc::FILE", var("f")),
            Stmt::Expr(unsafe_call(
                "fputs",
                vec![Expr::CStr(b"owned\n".to_vec()), var("_v1")],
            )),
            temp("_v2", "*mut libc::FILE", var("f")),
            Stmt::Expr(unsafe_call("fclose", vec![var("_v2")])),
            Stmt::Expr(call("remove", vec![Expr::CStr(b"out.txt".to_vec())])),
        ]);

        assert!(out.contains("drop(f);"), "{out}");
        assert!(!out.contains("unsafe { fclose("));
    }
}
