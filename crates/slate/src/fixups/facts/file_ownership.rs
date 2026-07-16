use std::collections::BTreeSet;

use crate::fixups::facts::walk;
use crate::fixups::facts::{
    AstPath, BindingId, FileOpenMode, FileOwnershipFact, FileUseFact, FileUseKind, FixupFacts,
    FunctionId, PathSegment,
};
use crate::rust_ast::{Expr, IndentStmt, Item, Path, Program, RustValue, Stmt, Type};

pub(in crate::fixups) fn collect_facts(program: &Program, facts: &mut FixupFacts) {
    facts.file_ownership.clear();
    for (item_index, item) in program.items.iter().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        collect_body(function, &f.body, facts);
    }
}

fn collect_body(function: FunctionId, body: &[IndentStmt], facts: &mut FixupFacts) {
    for (index, pair) in body.windows(2).enumerate() {
        let Some(handle_name) = null_file_decl(&pair[0].stmt) else {
            continue;
        };
        let handle_path = AstPath(vec![PathSegment::Stmt(index)]);
        let Some(handle) = facts.binding_by_local_path(function, handle_name, &handle_path) else {
            continue;
        };
        let Some(open) = find_open(function, body, facts, index + 1, handle_name) else {
            continue;
        };
        facts.file_ownership.push(FileOwnershipFact {
            function,
            handle,
            open_temp: open.open_temp,
            close_temp: open.close_temp,
            handle_path,
            open_path: AstPath(vec![PathSegment::Stmt(open.open_index)]),
            assign_path: AstPath(vec![PathSegment::Stmt(open.assign_index)]),
            close_path: AstPath(vec![PathSegment::Stmt(open.close_index)]),
            path_arg: call_arg_path(open.open_index, 0),
            mode_arg: call_arg_path(open.open_index, 1),
            mode: open.mode,
            uses: open.uses,
        });
    }
}

struct OpenCandidate {
    open_index: usize,
    assign_index: usize,
    close_index: usize,
    open_temp: BindingId,
    close_temp: Option<BindingId>,
    mode: Option<FileOpenMode>,
    uses: Vec<FileUseFact>,
}

fn find_open(
    function: FunctionId,
    body: &[IndentStmt],
    facts: &FixupFacts,
    start: usize,
    handle_name: &str,
) -> Option<OpenCandidate> {
    for open_index in start..body.len() {
        let Some(open) = fopen_temp(&body[open_index].stmt) else {
            continue;
        };
        let open_temp = facts.binding_by_local_path(
            function,
            open.temp_name.as_str(),
            &AstPath(vec![PathSegment::Stmt(open_index)]),
        )?;
        for assign_index in open_index + 1..body.len() {
            if !assigns_opened_handle(
                &body[assign_index].stmt,
                handle_name,
                open.temp_name.as_str(),
            ) {
                continue;
            }
            let (close_index, close_temp, uses) =
                file_uses_are_owned(function, body, facts, handle_name, assign_index)?;
            return Some(OpenCandidate {
                open_index,
                assign_index,
                close_index,
                open_temp,
                close_temp,
                mode: open.mode,
                uses,
            });
        }
    }
    None
}

struct FOpenTemp {
    temp_name: String,
    mode: Option<FileOpenMode>,
}

fn null_file_decl(stmt: &Stmt) -> Option<&str> {
    let Stmt::Let {
        name,
        ty: Some(ty),
        init: Some(Expr::Value(RustValue::NullPtr)),
        ..
    } = stmt
    else {
        return None;
    };
    is_file_ptr(ty).then_some(name.as_str())
}

fn is_file_ptr(ty: &Type) -> bool {
    ty.render() == "*mut libc::FILE"
}

fn fopen_temp(stmt: &Stmt) -> Option<FOpenTemp> {
    let Stmt::Let {
        name,
        init: Some(init),
        ..
    } = stmt
    else {
        return None;
    };
    let call = unsafe_tail(init)?;
    let Expr::Call { func, args } = call else {
        return None;
    };
    if !matches!(&**func, Expr::Var(callee) if callee.as_str() == "fopen") || args.len() != 2 {
        return None;
    }
    Some(FOpenTemp {
        temp_name: name.clone(),
        mode: file_mode(&args[1]),
    })
}

fn assigns_opened_handle(stmt: &Stmt, handle_name: &str, open_temp: &str) -> bool {
    matches!(
        stmt,
        Stmt::Assign {
            target: Expr::Var(target),
            value: Expr::Var(value)
        } if target.as_str() == handle_name && value.as_str() == open_temp
    )
}

fn file_uses_are_owned(
    function: FunctionId,
    body: &[IndentStmt],
    facts: &FixupFacts,
    handle_name: &str,
    assign_index: usize,
) -> Option<(usize, Option<BindingId>, Vec<FileUseFact>)> {
    let mut aliases = BTreeSet::from([handle_name.to_string()]);
    let mut close: Option<(usize, Option<BindingId>)> = None;
    let mut uses = Vec::new();
    let mut saw_null_guard = false;
    for (index, indent) in body.iter().enumerate() {
        if index <= assign_index {
            continue;
        }
        if let Some(alias) = handle_alias_temp(&indent.stmt, &aliases) {
            aliases.insert(alias.to_string());
            continue;
        }
        if !saw_null_guard && null_exit_guard(&indent.stmt, handle_name) {
            saw_null_guard = true;
            continue;
        }
        if let Some(kind) = file_use(&indent.stmt, &aliases) {
            if !saw_null_guard {
                return None;
            }
            if kind == FileUseKind::Close {
                if close.is_some() {
                    return None;
                }
                close = Some((
                    index,
                    close_temp_before(function, body, facts, index, handle_name),
                ));
            } else if close.is_some() {
                return None;
            }
            uses.push(FileUseFact {
                path: AstPath(vec![PathSegment::Stmt(index)]),
                kind,
            });
            continue;
        }
        if reassigns_handle(&indent.stmt, handle_name)
            || stmt_mentions_any_handle(&indent.stmt, &aliases)
        {
            return None;
        }
    }
    let (close_index, close_temp) = close?;
    Some((close_index, close_temp, uses))
}

fn handle_alias_temp<'a>(stmt: &'a Stmt, names: &BTreeSet<String>) -> Option<&'a str> {
    let Stmt::Let {
        name,
        init: Some(Expr::Var(source)),
        ..
    } = stmt
    else {
        return None;
    };
    names.contains(source.as_str()).then_some(name.as_str())
}

fn close_temp_before(
    function: FunctionId,
    body: &[IndentStmt],
    facts: &FixupFacts,
    index: usize,
    handle_name: &str,
) -> Option<BindingId> {
    let prev_index = index.checked_sub(1)?;
    let alias = direct_handle_alias(&body.get(prev_index)?.stmt, handle_name)?;
    facts.binding_by_local_path(
        function,
        alias,
        &AstPath(vec![PathSegment::Stmt(prev_index)]),
    )
}

fn direct_handle_alias<'a>(stmt: &'a Stmt, handle_name: &str) -> Option<&'a str> {
    let Stmt::Let {
        name,
        init: Some(Expr::Var(source)),
        ..
    } = stmt
    else {
        return None;
    };
    (source.as_str() == handle_name).then_some(name.as_str())
}

fn null_exit_guard(stmt: &Stmt, handle_name: &str) -> bool {
    match stmt {
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => cond_mentions(cond, handle_name) && body_exits(then_body) && else_body.is_empty(),
        Stmt::Scope { body } => scope_null_exit_guard(body, handle_name),
        _ => false,
    }
}

fn scope_null_exit_guard(body: &[IndentStmt], handle_name: &str) -> bool {
    if body.len() == 1 {
        return null_exit_guard(&body[0].stmt, handle_name);
    }
    if body.len() != 2 {
        return false;
    }
    let Stmt::Let {
        name,
        init: Some(init),
        ..
    } = &body[0].stmt
    else {
        return body
            .iter()
            .any(|indent| null_exit_guard(&indent.stmt, handle_name));
    };
    if !cond_mentions(init, handle_name) {
        return false;
    }
    matches!(
        &body[1].stmt,
        Stmt::If {
            cond: Expr::Var(cond),
            then_body,
            else_body,
        } if cond.as_str() == name.as_str() && body_exits(then_body) && else_body.is_empty()
    )
}

fn body_exits(body: &[IndentStmt]) -> bool {
    body.iter().any(|indent| match &indent.stmt {
        Stmt::Return(_) => true,
        Stmt::Expr(expr) => is_process_exit(expr),
        Stmt::If {
            then_body,
            else_body,
            ..
        } => body_exits(then_body) && body_exits(else_body),
        Stmt::Scope { body } => body_exits(body),
        _ => false,
    })
}

fn is_process_exit(expr: &Expr) -> bool {
    let Expr::Call { func, .. } = expr else {
        return false;
    };
    let Expr::Path(Path { segments }) = &**func else {
        return false;
    };
    let segments = segments
        .iter()
        .map(|segment| segment.as_str())
        .collect::<Vec<_>>();
    matches!(segments.as_slice(), ["std", "process", "exit"])
}

fn cond_mentions(expr: &Expr, handle_name: &str) -> bool {
    walk::exprs_any(
        expr,
        &mut |expr| matches!(expr, Expr::Var(name) if name.as_str() == handle_name),
    )
}

fn file_use(stmt: &Stmt, aliases: &BTreeSet<String>) -> Option<FileUseKind> {
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
    let Expr::Var(callee) = &**func else {
        return None;
    };
    match callee.as_str() {
        "fread" if args.len() == 4 && arg_is_handle(&args[3], aliases) => Some(FileUseKind::Read),
        "fwrite" if args.len() == 4 && arg_is_handle(&args[3], aliases) => Some(FileUseKind::Write),
        "fgets" if args.len() == 3 && arg_is_handle(&args[2], aliases) => Some(FileUseKind::Gets),
        "fputs" if args.len() == 2 && arg_is_handle(&args[1], aliases) => Some(FileUseKind::Puts),
        "fclose" if args.len() == 1 && arg_is_handle(&args[0], aliases) => Some(FileUseKind::Close),
        _ => None,
    }
}

fn arg_is_handle(expr: &Expr, aliases: &BTreeSet<String>) -> bool {
    match expr {
        Expr::Var(name) => aliases.contains(name.as_str()),
        Expr::Cast { expr, .. } => arg_is_handle(expr, aliases),
        _ => false,
    }
}

fn unsafe_tail(expr: &Expr) -> Option<&Expr> {
    match expr {
        Expr::Unsafe(block) if block.stmts.is_empty() => block.tail.as_deref(),
        _ => Some(expr),
    }
}

fn reassigns_handle(stmt: &Stmt, handle_name: &str) -> bool {
    matches!(
        stmt,
        Stmt::Assign {
            target: Expr::Var(name),
            ..
        } if name.as_str() == handle_name
    )
}

fn stmt_mentions_any_handle(stmt: &Stmt, aliases: &BTreeSet<String>) -> bool {
    let mut escaped = false;
    walk::stmt_exprs(stmt, &mut |expr| {
        if escaped {
            return;
        }
        match expr {
            Expr::Call { func, args } => {
                if matches!(&**func, Expr::Var(callee) if supported_stdio_callee(callee.as_str())) {
                    return;
                }
                if args.iter().any(|arg| expr_mentions_any(arg, aliases)) {
                    escaped = true;
                }
            }
            Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
                if expr_mentions_any(recv, aliases)
                    || args.iter().any(|arg| expr_mentions_any(arg, aliases))
                {
                    escaped = true;
                }
            }
            Expr::AddrOf { expr, .. }
            | Expr::Ref { expr, .. }
            | Expr::ArrayPtr { array: expr, .. }
                if expr_mentions_any(expr, aliases) =>
            {
                escaped = true;
            }
            _ => {}
        }
    });
    escaped
}

fn expr_mentions_any(expr: &Expr, aliases: &BTreeSet<String>) -> bool {
    walk::exprs_any(
        expr,
        &mut |expr| matches!(expr, Expr::Var(name) if aliases.contains(name.as_str())),
    )
}

fn supported_stdio_callee(name: &str) -> bool {
    matches!(
        name,
        "fopen" | "fread" | "fwrite" | "fgets" | "fputs" | "fclose"
    )
}

fn file_mode(expr: &Expr) -> Option<FileOpenMode> {
    match expr {
        Expr::CStr(bytes) => mode_from_bytes(bytes),
        Expr::ByteStr(bytes) => mode_from_bytes(bytes.strip_suffix(&[0]).unwrap_or(bytes)),
        Expr::Str(s) => mode_from_bytes(s.as_bytes()),
        Expr::Cast { expr, .. } => file_mode(expr),
        Expr::MethodCall { recv, method, args } if method == "as_ptr" && args.is_empty() => {
            file_mode(recv)
        }
        _ => None,
    }
}

fn mode_from_bytes(bytes: &[u8]) -> Option<FileOpenMode> {
    match bytes {
        b"r" | b"rb" => Some(FileOpenMode::Read),
        b"w" | b"wb" => Some(FileOpenMode::Write),
        b"a" | b"ab" => Some(FileOpenMode::Append),
        b"r+" | b"r+b" | b"rb+" => Some(FileOpenMode::ReadUpdate),
        b"w+" | b"w+b" | b"wb+" => Some(FileOpenMode::WriteUpdate),
        b"a+" | b"a+b" | b"ab+" => Some(FileOpenMode::AppendUpdate),
        _ => None,
    }
}

fn call_arg_path(stmt_index: usize, arg_index: usize) -> AstPath {
    AstPath(vec![
        PathSegment::Stmt(stmt_index),
        PathSegment::UnsafeBody,
        PathSegment::BlockTail,
        PathSegment::Expr(arg_index + 1),
    ])
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::facts;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{
        Block, Expr, ExternDecl, ExternFnDecl, Ident, Item, Path, Program, Stmt, Type,
    };

    fn analyzed(stmts: Vec<Stmt>) -> facts::FixupFacts {
        facts::analyze(Program {
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
                extern_fn(
                    "fread",
                    vec![
                        ("ptr", "*mut core::ffi::c_void"),
                        ("size", "usize"),
                        ("nmemb", "usize"),
                        ("stream", "*mut libc::FILE"),
                    ],
                    Some("usize"),
                ),
                extern_fn(
                    "fwrite",
                    vec![
                        ("ptr", "*mut core::ffi::c_void"),
                        ("size", "usize"),
                        ("nmemb", "usize"),
                        ("stream", "*mut libc::FILE"),
                    ],
                    Some("usize"),
                ),
                extern_fn(
                    "fgets",
                    vec![
                        ("s", "*mut i8"),
                        ("size", "i32"),
                        ("stream", "*mut libc::FILE"),
                    ],
                    Some("*mut i8"),
                ),
                extern_fn("fclose", vec![("stream", "*mut libc::FILE")], Some("i32")),
                Item::Fn(func(vec![], None, stmts)),
            ],
        })
        .facts
    }

    fn extern_fn(name: &str, params: Vec<(&str, &str)>, ret: Option<&str>) -> Item {
        Item::ExternBlock {
            abi: "C".into(),
            decls: vec![ExternDecl::Fn(ExternFnDecl {
                name: name.into(),
                params: params
                    .into_iter()
                    .map(|(name, ty)| crate::rust_ast::FnParam {
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

    fn exit_call() -> Expr {
        Expr::Call {
            func: Box::new(Expr::Path(Path::new(
                ["std", "process", "exit"].into_iter().map(Ident::new),
            ))),
            args: vec![int(0)],
        }
    }

    fn guarded_body(extra: Vec<Stmt>) -> Vec<Stmt> {
        let mut stmts = vec![
            Stmt::Let {
                name: "f".into(),
                mutable: true,
                ty: Some(Type::parse("*mut libc::FILE")),
                init: Some(Expr::Value(RustValue::NullPtr)),
            },
            temp(
                "_v0",
                "*mut libc::FILE",
                unsafe_call("fopen", vec![var("path"), Expr::CStr(b"w+".to_vec())]),
            ),
            assign("f", var("_v0")),
            Stmt::If {
                cond: var("f"),
                then_body: vec![IndentStmt {
                    depth: 2,
                    stmt: Stmt::Expr(exit_call()),
                }],
                else_body: Vec::new(),
            },
        ];
        stmts.extend(extra);
        stmts
    }

    fn lowered_guarded_body(extra: Vec<Stmt>) -> Vec<Stmt> {
        let mut stmts = vec![
            Stmt::Let {
                name: "f".into(),
                mutable: true,
                ty: Some(Type::parse("*mut libc::FILE")),
                init: Some(Expr::Value(RustValue::NullPtr)),
            },
            temp(
                "_v0",
                "*mut libc::FILE",
                unsafe_call("fopen", vec![var("path"), Expr::CStr(b"w+".to_vec())]),
            ),
            assign("f", var("_v0")),
            Stmt::Scope {
                body: vec![
                    IndentStmt {
                        depth: 2,
                        stmt: temp("_v_guard", "bool", var("f")),
                    },
                    IndentStmt {
                        depth: 2,
                        stmt: Stmt::If {
                            cond: var("_v_guard"),
                            then_body: vec![IndentStmt {
                                depth: 3,
                                stmt: Stmt::Expr(exit_call()),
                            }],
                            else_body: Vec::new(),
                        },
                    },
                ],
            },
        ];
        stmts.extend(extra);
        stmts
    }

    #[test]
    fn records_single_owner_fopen_fputs_fclose_scope() {
        let facts = analyzed(guarded_body(vec![
            temp("_v1", "*mut libc::FILE", var("f")),
            Stmt::Expr(unsafe_call(
                "fputs",
                vec![Expr::CStr(b"x".to_vec()), var("_v1")],
            )),
            temp("_v2", "*mut libc::FILE", var("f")),
            Stmt::Expr(unsafe_call(
                "fclose",
                vec![Expr::Cast {
                    expr: Box::new(var("_v2")),
                    ty: Type::parse("*mut libc::FILE"),
                }],
            )),
        ]));

        assert_eq!(facts.file_ownership.len(), 1);
        let owner = &facts.file_ownership[0];
        assert_eq!(owner.mode, Some(FileOpenMode::WriteUpdate));
        assert_eq!(
            owner.uses.iter().map(|use_| use_.kind).collect::<Vec<_>>(),
            vec![FileUseKind::Puts, FileUseKind::Close]
        );
        assert!(facts.file_ownership(owner.handle).is_some());
        assert_eq!(owner.path_arg, call_arg_path(1, 0));
        assert_eq!(owner.mode_arg, call_arg_path(1, 1));
    }

    #[test]
    fn rejects_leaked_or_unguarded_file_handles() {
        let leaked = analyzed(guarded_body(vec![
            temp("_v1", "*mut libc::FILE", var("f")),
            Stmt::Expr(unsafe_call(
                "fputs",
                vec![Expr::CStr(b"x".to_vec()), var("_v1")],
            )),
        ]));
        assert!(leaked.file_ownership.is_empty());

        let unguarded = analyzed(vec![
            Stmt::Let {
                name: "f".into(),
                mutable: true,
                ty: Some(Type::parse("*mut libc::FILE")),
                init: Some(Expr::Value(RustValue::NullPtr)),
            },
            temp(
                "_v0",
                "*mut libc::FILE",
                unsafe_call("fopen", vec![var("path"), Expr::CStr(b"w".to_vec())]),
            ),
            assign("f", var("_v0")),
            temp("_v1", "*mut libc::FILE", var("f")),
            Stmt::Expr(unsafe_call("fclose", vec![var("_v1")])),
        ]);
        assert!(unguarded.file_ownership.is_empty());
    }

    #[test]
    fn rejects_escaped_file_handles() {
        let facts = analyzed(guarded_body(vec![
            Stmt::Expr(call("unknown", vec![var("f")])),
            temp("_v1", "*mut libc::FILE", var("f")),
            Stmt::Expr(unsafe_call("fclose", vec![var("_v1")])),
        ]));

        assert!(facts.file_ownership.is_empty());
    }

    #[test]
    fn accepts_lowered_temp_null_exit_guard() {
        let facts = analyzed(lowered_guarded_body(vec![
            temp("_v1", "*mut libc::FILE", var("f")),
            Stmt::Expr(unsafe_call("fclose", vec![var("_v1")])),
        ]));

        assert_eq!(facts.file_ownership.len(), 1);
    }

    #[test]
    fn records_requested_stdio_file_use_kinds() {
        let facts = analyzed(guarded_body(vec![
            temp("_v1", "*mut libc::FILE", var("f")),
            temp(
                "_nread",
                "usize",
                unsafe_call("fread", vec![var("buf"), int(1), int(4), var("_v1")]),
            ),
            temp("_v2", "*mut libc::FILE", var("f")),
            temp(
                "_nwrite",
                "usize",
                unsafe_call("fwrite", vec![var("buf"), int(1), int(4), var("_v2")]),
            ),
            temp("_v3", "*mut libc::FILE", var("f")),
            Stmt::Expr(unsafe_call("fgets", vec![var("buf"), int(16), var("_v3")])),
            temp("_v4", "*mut libc::FILE", var("f")),
            Stmt::Expr(unsafe_call(
                "fputs",
                vec![Expr::CStr(b"x".to_vec()), var("_v4")],
            )),
            temp("_v5", "*mut libc::FILE", var("f")),
            Stmt::Expr(unsafe_call("fclose", vec![var("_v5")])),
        ]));

        assert_eq!(
            facts.file_ownership[0]
                .uses
                .iter()
                .map(|use_| use_.kind)
                .collect::<Vec<_>>(),
            vec![
                FileUseKind::Read,
                FileUseKind::Write,
                FileUseKind::Gets,
                FileUseKind::Puts,
                FileUseKind::Close,
            ]
        );
    }
}
