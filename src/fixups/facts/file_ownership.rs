use std::collections::BTreeSet;

use crate::fixups::facts::walk;
use crate::fixups::facts::{
    AstPath, BindingId, FileOpenMode, FileOwnershipFact, FileUseFact, FileUseKind, FixupFacts,
    FunctionId, PathSegment,
};
use crate::function_identity::{Known, known_call};
use crate::rust_ast::{
    BinOp, Expr, IndentStmt, Item, Path, Program, RustValue, Stmt, Type, UnaryOp,
};

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
    let Expr::Call { args, .. } = call else {
        return None;
    };
    if known_call(call) != Some(Known::FOpen) || args.len() != 2 {
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
        if let Stmt::Loop {
            label: None,
            body: loop_body,
        } = &indent.stmt
        {
            if match_gets_loop(loop_body, &aliases).is_some() {
                if !saw_null_guard || close.is_some() {
                    return None;
                }
                uses.push(FileUseFact {
                    path: AstPath(vec![PathSegment::Stmt(index)]),
                    kind: FileUseKind::Gets,
                });
                continue;
            }
            if loop_mentions_any_handle(loop_body, &aliases) {
                return None;
            }
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
    let call = unsafe_tail(expr)?;
    let Expr::Call { args, .. } = call else {
        return None;
    };
    match known_call(call) {
        Some(Known::FRead) if args.len() == 4 && arg_is_handle(&args[3], aliases) => {
            Some(FileUseKind::Read)
        }
        Some(Known::FWrite) if args.len() == 4 && arg_is_handle(&args[3], aliases) => {
            Some(FileUseKind::Write)
        }
        Some(Known::FGets) if args.len() == 3 && arg_is_handle(&args[2], aliases) => {
            Some(FileUseKind::Gets)
        }
        Some(Known::FPuts) if args.len() == 2 && arg_is_handle(&args[1], aliases) => {
            Some(FileUseKind::Puts)
        }
        Some(Known::FClose) if args.len() == 1 && arg_is_handle(&args[0], aliases) => {
            Some(FileUseKind::Close)
        }
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
            Expr::Call { args, .. } => {
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

pub(in crate::fixups) struct GetsLoopMatch {
    pub buf_name: String,
    pub buf_len: i64,
}

/// Recognizes exactly `while (fgets(buf, n, handle) != NULL) { fputs(buf, stdout); }`,
/// lowered as `Loop { [..size/other temps, fgets-as-Let, null-check-break, Scope{echo}] }`.
pub(in crate::fixups) fn match_gets_loop(
    loop_body: &[IndentStmt],
    aliases: &BTreeSet<String>,
) -> Option<GetsLoopMatch> {
    let mut local_aliases = aliases.clone();
    let mut fgets_index = None;
    for (index, indent) in loop_body.iter().enumerate() {
        if let Some(alias) = handle_alias_temp(&indent.stmt, &local_aliases) {
            local_aliases.insert(alias.to_string());
            continue;
        }
        if matches!(
            file_use(&indent.stmt, &local_aliases),
            Some(FileUseKind::Gets)
        ) {
            fgets_index = Some(index);
            break;
        }
    }
    let fgets_index = fgets_index?;
    let Stmt::Let {
        name: result_name,
        init: Some(init),
        ..
    } = &loop_body[fgets_index].stmt
    else {
        return None;
    };
    let Expr::Call { args, .. } = unsafe_tail(init)? else {
        return None;
    };
    if args.len() != 3 {
        return None;
    }
    let buf_name = buf_ptr_var(&args[0])?;
    let buf_len = resolve_static_len(loop_body, fgets_index, &args[1])?;

    let break_stmt = &loop_body.get(fgets_index + 1)?.stmt;
    if !is_null_break(break_stmt, result_name) {
        return None;
    }

    let scope_index = fgets_index + 2;
    if scope_index != loop_body.len() - 1 {
        return None;
    }
    let Stmt::Scope { body: scope_body } = &loop_body[scope_index].stmt else {
        return None;
    };
    if !scope_echoes_buf_to_stdout(scope_body, &buf_name) {
        return None;
    }

    Some(GetsLoopMatch { buf_name, buf_len })
}

pub(in crate::fixups) fn match_gets_call(
    body: &[IndentStmt],
    use_index: usize,
) -> Option<GetsLoopMatch> {
    let Stmt::Expr(expr) = &body[use_index].stmt else {
        return None;
    };
    let Expr::Call { args, .. } = unsafe_tail(expr)? else {
        return None;
    };
    if args.len() != 3 {
        return None;
    }
    let buf_name = buf_ptr_var(&args[0])?;
    let buf_len = resolve_static_len(body, use_index, &args[1])?;
    Some(GetsLoopMatch { buf_name, buf_len })
}

fn strip_cast(expr: &Expr) -> &Expr {
    match expr {
        Expr::Cast { expr: inner, .. } => strip_cast(inner),
        _ => expr,
    }
}

pub(in crate::fixups) fn buf_ptr_var(expr: &Expr) -> Option<String> {
    let Expr::ArrayPtr { array, .. } = strip_cast(expr) else {
        return None;
    };
    let Expr::Var(name) = &**array else {
        return None;
    };
    Some(name.as_str().to_string())
}

fn resolve_static_len(
    loop_body: &[IndentStmt],
    fgets_index: usize,
    size_expr: &Expr,
) -> Option<i64> {
    if let Some(n) = literal_int(size_expr) {
        return Some(n);
    }
    let Expr::Var(name) = strip_cast(size_expr) else {
        return None;
    };
    loop_body[..fgets_index].iter().rev().find_map(|indent| {
        let Stmt::Let {
            name: let_name,
            init: Some(init),
            ..
        } = &indent.stmt
        else {
            return None;
        };
        (let_name.as_str() == name.as_str())
            .then(|| literal_int(init))
            .flatten()
    })
}

fn literal_int(expr: &Expr) -> Option<i64> {
    match strip_cast(expr) {
        Expr::Value(RustValue::I64(n)) => Some(*n),
        Expr::Value(RustValue::Usize(n)) => Some(*n as i64),
        Expr::Value(RustValue::I128(n)) => Some(*n as i64),
        _ => None,
    }
}

fn is_null_break(stmt: &Stmt, result_name: &str) -> bool {
    let Stmt::If {
        cond,
        then_body,
        else_body,
    } = stmt
    else {
        return false;
    };
    if !else_body.is_empty() {
        return false;
    }
    if !matches!(
        then_body.as_slice(),
        [IndentStmt {
            stmt: Stmt::Break(None),
            ..
        }]
    ) {
        return false;
    }
    is_not_null_check(cond, result_name)
}

fn is_not_null_check(expr: &Expr, result_name: &str) -> bool {
    let Expr::Unary {
        op: UnaryOp::Not,
        expr: inner,
    } = expr
    else {
        return false;
    };
    let Expr::Binary {
        op: BinOp::Ne,
        lhs,
        rhs,
    } = &**inner
    else {
        return false;
    };
    matches!(&**lhs, Expr::Var(name) if name.as_str() == result_name) && is_null_expr(rhs)
}

fn is_null_expr(expr: &Expr) -> bool {
    matches!(strip_cast(expr), Expr::Value(RustValue::NullPtr))
}

fn scope_echoes_buf_to_stdout(body: &[IndentStmt], buf_name: &str) -> bool {
    let [first, second] = body else {
        return false;
    };
    let Stmt::Let {
        name: stdout_alias,
        init: Some(init),
        ..
    } = &first.stmt
    else {
        return false;
    };
    if !is_stdout_expr(init) {
        return false;
    }
    let Stmt::Expr(expr) = &second.stmt else {
        return false;
    };
    let call = unsafe_tail(expr).unwrap_or(expr);
    let Expr::Call { args, .. } = call else {
        return false;
    };
    if known_call(call) != Some(Known::FPuts) || args.len() != 2 {
        return false;
    }
    buf_ptr_var(&args[0]).as_deref() == Some(buf_name)
        && matches!(strip_cast(&args[1]), Expr::Var(name) if name.as_str() == stdout_alias.as_str())
}

fn is_stdout_expr(expr: &Expr) -> bool {
    matches!(unsafe_tail(expr), Some(Expr::Var(name)) if name.as_str() == "stdout")
}

fn loop_mentions_any_handle(loop_body: &[IndentStmt], aliases: &BTreeSet<String>) -> bool {
    let mut local_aliases = aliases.clone();
    for indent in loop_body {
        if let Some(alias) = handle_alias_temp(&indent.stmt, &local_aliases) {
            local_aliases.insert(alias.to_string());
            continue;
        }
        if stmt_mentions_any_handle(&indent.stmt, &local_aliases) {
            return true;
        }
    }
    false
}

fn call_arg_path(stmt_index: usize, arg_index: usize) -> AstPath {
    AstPath(vec![
        PathSegment::Stmt(stmt_index),
        PathSegment::UnsafeBody,
        PathSegment::BlockTail,
        PathSegment::Expr(arg_index + 1),
    ])
}
