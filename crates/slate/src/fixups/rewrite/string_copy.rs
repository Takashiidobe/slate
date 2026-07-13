use crate::fixups::facts::{
    AstPath, ConstValue, FixupFacts, FunctionId, PathSegment, StringBufferProvenance,
    StringRecoveryCandidate, ValueSubject,
};
use crate::rust_ast::{
    Block, Expr, ExternDecl, IndentStmt, Item, Prim, Program, RustValue, Stmt, Type,
};
use std::collections::{BTreeMap, BTreeSet};

pub(in crate::fixups) fn fixup(program: &mut Program, facts: &FixupFacts) {
    for (item_index, item) in program.items.iter_mut().enumerate() {
        if let Item::Fn(f) = item
            && let Some(function) = facts.function_by_item_index(item_index)
        {
            fixup_body(&mut f.body, function, facts);
        }
    }
    prune_unused_externs(program);
}

#[derive(Clone)]
struct Candidate {
    init: Expr,
    remove_index: Option<usize>,
}

#[derive(Clone)]
enum Source {
    Str(Expr),
    Owned(String),
}

fn fixup_body(body: &mut Vec<IndentStmt>, function: FunctionId, facts: &FixupFacts) {
    fixup_nested(body, function, facts);
    let consts = const_usize_temps(function, facts);
    let liftable = liftable_names(body, function, facts, &consts);
    let mut remove = Vec::new();
    for i in 0..body.len() {
        let Some(candidate) = candidate_at(body, function, facts, &stmt_path(&[], i)) else {
            continue;
        };
        let Stmt::Let { name, ty, init, .. } = &mut body[i].stmt else {
            continue;
        };
        if !liftable.contains(name) {
            continue;
        }
        *ty = Some(Type::Custom("String".into()));
        *init = Some(to_owned(candidate.init));
        if let Some(remove_index) = candidate.remove_index {
            remove.push(remove_index);
        }
    }
    for i in remove.into_iter().rev() {
        body.remove(i);
    }
    rewrite_body(body, &liftable, &consts);
}

fn liftable_names(
    body: &[IndentStmt],
    function: FunctionId,
    facts: &FixupFacts,
    consts: &BTreeMap<String, usize>,
) -> BTreeSet<String> {
    let candidates = (0..body.len())
        .filter_map(|i| {
            let candidate = candidate_at(body, function, facts, &stmt_path(&[], i))?;
            let name = let_name(&body[i].stmt)?;
            Some((name, i, candidate))
        })
        .collect::<Vec<_>>();
    let mut liftable = candidates
        .iter()
        .map(|(name, _, _)| name.clone())
        .collect::<BTreeSet<_>>();
    loop {
        let before = liftable.clone();
        liftable.retain(|name| {
            let Some((_, i, candidate)) = candidates
                .iter()
                .find(|(candidate_name, _, _)| candidate_name == name)
            else {
                return false;
            };
            let scan_start = candidate.remove_index.map_or(i + 1, |index| index + 1);
            body[scan_start..]
                .iter()
                .all(|indent| stmt_allows_lift(&indent.stmt, name, &before, consts))
        });
        if liftable == before {
            return liftable;
        }
    }
}

fn let_name(stmt: &Stmt) -> Option<String> {
    match stmt {
        Stmt::Let { name, .. } => Some(name.clone()),
        _ => None,
    }
}

fn candidate_at(
    body: &[IndentStmt],
    function: FunctionId,
    facts: &FixupFacts,
    path: &[PathSegment],
) -> Option<Candidate> {
    let buffer = facts.string_buffer_at(function, &AstPath(path.to_vec()))?;
    if !buffer
        .candidates
        .contains(&StringRecoveryCandidate::OwnedString)
    {
        return None;
    }
    let init = match &buffer.provenance {
        StringBufferProvenance::ZeroInitialized => String::new(),
        StringBufferProvenance::Literal => {
            if facts
                .def_use(buffer.binding)
                .is_some_and(|def_use| !def_use.writes.is_empty())
            {
                return None;
            }
            String::from_utf8(buffer.bytes.clone()?).ok()?
        }
        StringBufferProvenance::AssignedLiteral { .. } => {
            String::from_utf8(buffer.bytes.clone()?).ok()?
        }
        _ => return None,
    };
    let remove_index = match &buffer.provenance {
        StringBufferProvenance::AssignedLiteral { assignment } => {
            Some(assignment_index(path, &assignment.0)?)
        }
        _ => None,
    };
    if remove_index.is_some_and(|index| index >= body.len()) {
        return None;
    }
    Some(Candidate {
        init: Expr::Str(init),
        remove_index,
    })
}

fn is_char_array(ty: &Type) -> bool {
    matches!(
        ty,
        Type::Array { elem, .. } if matches!(&**elem, Type::Prim(Prim::I8 | Prim::U8))
    )
}

fn is_zero_array(expr: &Expr) -> bool {
    matches!(
        expr,
        Expr::ArrayRepeat { elem, .. }
            if matches!(&**elem, Expr::Value(RustValue::I64(0) | RustValue::I128(0)))
    )
}

fn array_c_string(expr: &Expr) -> Option<String> {
    let Expr::ArrayLit(elems) = expr else {
        return None;
    };
    let bytes = elems.iter().map(byte_literal).collect::<Option<Vec<_>>>()?;
    let nul = bytes.iter().position(|byte| *byte == 0)?;
    if bytes[nul..].iter().any(|byte| *byte != 0) {
        return None;
    }
    String::from_utf8(bytes[..nul].to_vec()).ok()
}

fn byte_literal(expr: &Expr) -> Option<u8> {
    let n = match expr {
        Expr::Value(RustValue::I64(n)) => *n,
        Expr::Value(RustValue::I128(n)) => i64::try_from(*n).ok()?,
        Expr::Cast { expr, .. } => return byte_literal(expr),
        _ => return None,
    };
    u8::try_from(n).ok()
}

fn const_usize_temps(function: FunctionId, facts: &FixupFacts) -> BTreeMap<String, usize> {
    facts
        .values
        .iter()
        .filter(|fact| fact.function == function)
        .filter_map(|fact| match (&fact.subject, &fact.value) {
            (ValueSubject::Binding(binding), ConstValue::Usize(value)) => {
                Some((facts.binding_name(*binding)?.to_owned(), *value))
            }
            _ => None,
        })
        .collect()
}

fn stmt_path(path: &[PathSegment], index: usize) -> Vec<PathSegment> {
    let mut path = path.to_vec();
    path.push(PathSegment::Stmt(index));
    path
}

fn assignment_index(def_path: &[PathSegment], assignment_path: &[PathSegment]) -> Option<usize> {
    let parent = def_path.get(..def_path.len().checked_sub(1)?)?;
    let assignment_parent = assignment_path.get(..assignment_path.len().checked_sub(1)?)?;
    if assignment_parent != parent {
        return None;
    }
    match assignment_path.last()? {
        PathSegment::Stmt(index) => Some(*index),
        _ => None,
    }
}

fn stmt_mentions_var(stmt: &Stmt, name: &str) -> bool {
    !stmt_allows_lift(stmt, name, &BTreeSet::new(), &BTreeMap::new())
}

fn stmt_allows_lift(
    stmt: &Stmt,
    name: &str,
    liftable: &BTreeSet<String>,
    consts: &BTreeMap<String, usize>,
) -> bool {
    match stmt {
        Stmt::Expr(expr) if supported_copy_call(expr, liftable, consts).is_some() => true,
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => expr_allows_lift(expr, name, liftable),
        Stmt::Let { init, .. } => init
            .as_ref()
            .is_none_or(|expr| expr_allows_lift(expr, name, liftable)),
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            !expr_mentions_var(target, name) && expr_allows_lift(value, name, liftable)
        }
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            expr_allows_lift(cond, name, liftable)
                && body_allows_lift(then_body, name, liftable, consts)
                && body_allows_lift(else_body, name, liftable, consts)
        }
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            expr_allows_lift(cond, name, liftable)
                && body_allows_lift(then_body, name, liftable, consts)
                && expr_allows_lift(then_value, name, liftable)
                && body_allows_lift(else_body, name, liftable, consts)
                && expr_allows_lift(else_value, name, liftable)
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            body_allows_lift(body, name, liftable, consts)
        }
        Stmt::While { cond, body } => {
            expr_allows_lift(cond, name, liftable)
                && block_allows_lift(body, name, liftable, consts)
        }
        Stmt::Block(body) | Stmt::Unsafe { body } => {
            block_allows_lift(body, name, liftable, consts)
        }
        Stmt::Match { expr, arms } => {
            expr_allows_lift(expr, name, liftable)
                && arms
                    .iter()
                    .all(|arm| body_allows_lift(&arm.body, name, liftable, consts))
        }
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => true,
    }
}

fn body_allows_lift(
    body: &[IndentStmt],
    name: &str,
    liftable: &BTreeSet<String>,
    consts: &BTreeMap<String, usize>,
) -> bool {
    body.iter()
        .all(|indent| stmt_allows_lift(&indent.stmt, name, liftable, consts))
}

fn block_allows_lift(
    block: &Block,
    name: &str,
    liftable: &BTreeSet<String>,
    consts: &BTreeMap<String, usize>,
) -> bool {
    body_allows_lift(&block.stmts, name, liftable, consts)
        && block
            .tail
            .as_deref()
            .is_none_or(|tail| expr_allows_lift(tail, name, liftable))
}

fn expr_allows_lift(expr: &Expr, name: &str, liftable: &BTreeSet<String>) -> bool {
    if expr_mentions_pointer_view(expr, name) {
        return expr_supported_pointer_view(expr, name, liftable);
    }
    !expr_mentions_var(expr, name)
}

fn expr_mentions_var(expr: &Expr, name: &str) -> bool {
    expr_any(
        expr,
        &mut |expr| matches!(expr, Expr::Var(v) if v.as_str() == name),
    )
}

fn expr_supported_pointer_view(expr: &Expr, name: &str, liftable: &BTreeSet<String>) -> bool {
    match expr {
        Expr::Call { func, args } if matches!(&**func, Expr::Var(callee) if matches!(callee.as_str(), "strlen" | "strcmp" | "strncmp")) => {
            args.iter().all(|arg| {
                pointer_view_source(arg)
                    .is_none_or(|source| source == name || liftable.contains(source))
                    || source_expr(arg, liftable).is_some()
            })
        }
        Expr::Call { func, args } if matches!(&**func, Expr::Var(callee) if callee.as_str() == "printf") => {
            printf_allows_lift(args, name)
        }
        Expr::Unsafe(block) if block.stmts.is_empty() => block
            .tail
            .as_deref()
            .is_some_and(|tail| expr_supported_pointer_view(tail, name, liftable)),
        Expr::Cast { expr, .. }
        | Expr::Unary { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => expr_supported_pointer_view(expr, name, liftable),
        _ => false,
    }
}

fn printf_allows_lift(args: &[Expr], name: &str) -> bool {
    let Some((fmt, rest)) = args.split_first() else {
        return false;
    };
    let Some(conversions) = printf_conversions(fmt) else {
        return false;
    };
    conversions.len() == rest.len()
        && conversions.iter().zip(rest).all(|(conversion, arg)| {
            *conversion == b's' && expr_mentions_pointer_view(arg, name)
                || !expr_mentions_pointer_view(arg, name)
        })
}

fn printf_conversions(expr: &Expr) -> Option<Vec<u8>> {
    let bytes = const_c_string(expr)?;
    let mut conversions = Vec::new();
    let mut i = 0;
    while i < bytes.len() {
        match bytes[i] {
            b'%' => {
                let conv = *bytes.get(i + 1)?;
                if conv == b'%' {
                    i += 2;
                    continue;
                }
                conversions.push(conv);
                i += 2;
            }
            _ => i += 1,
        }
    }
    Some(conversions)
}

fn expr_mentions_pointer_view(expr: &Expr, name: &str) -> bool {
    expr_any(expr, &mut |expr| {
        pointer_view_source(expr).is_some_and(|source| source == name)
    })
}

fn fixup_nested(body: &mut [IndentStmt], function: FunctionId, facts: &FixupFacts) {
    for indent in body {
        match &mut indent.stmt {
            Stmt::If {
                then_body,
                else_body,
                ..
            }
            | Stmt::LetIf {
                then_body,
                else_body,
                ..
            } => {
                fixup_body(then_body, function, facts);
                fixup_body(else_body, function, facts);
            }
            Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
                fixup_body(body, function, facts);
            }
            Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
                fixup_body(&mut body.stmts, function, facts);
            }
            Stmt::Match { arms, .. } => {
                for arm in arms {
                    fixup_body(&mut arm.body, function, facts);
                }
            }
            _ => {}
        }
    }
}

fn rewrite_body(
    body: &mut [IndentStmt],
    liftable: &BTreeSet<String>,
    consts: &BTreeMap<String, usize>,
) {
    for indent in body {
        rewrite_stmt(&mut indent.stmt, liftable, consts);
    }
}

fn rewrite_stmt(stmt: &mut Stmt, liftable: &BTreeSet<String>, consts: &BTreeMap<String, usize>) {
    if let Stmt::Expr(expr) = stmt
        && let Some(replacement) = supported_copy_call(expr, liftable, consts)
    {
        *stmt = replacement;
        return;
    }
    match stmt {
        Stmt::Let { init, .. } => {
            if let Some(init) = init {
                rewrite_expr_pointer_views(init, liftable);
            }
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => rewrite_expr_pointer_views(expr, liftable),
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            rewrite_expr_pointer_views(target, liftable);
            rewrite_expr_pointer_views(value, liftable);
        }
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            rewrite_expr_pointer_views(cond, liftable);
            rewrite_body(then_body, liftable, consts);
            rewrite_body(else_body, liftable, consts);
        }
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            rewrite_expr_pointer_views(cond, liftable);
            rewrite_body(then_body, liftable, consts);
            rewrite_expr_pointer_views(then_value, liftable);
            rewrite_body(else_body, liftable, consts);
            rewrite_expr_pointer_views(else_value, liftable);
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            rewrite_body(body, liftable, consts);
        }
        Stmt::While { cond, body } => {
            rewrite_expr_pointer_views(cond, liftable);
            rewrite_block_pointer_views(body, liftable, consts);
        }
        Stmt::Block(body) | Stmt::Unsafe { body } => {
            rewrite_block_pointer_views(body, liftable, consts)
        }
        Stmt::Match { expr, arms } => {
            rewrite_expr_pointer_views(expr, liftable);
            for arm in arms {
                rewrite_body(&mut arm.body, liftable, consts);
            }
        }
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
    }
}

fn rewrite_block_pointer_views(
    block: &mut Block,
    liftable: &BTreeSet<String>,
    consts: &BTreeMap<String, usize>,
) {
    rewrite_body(&mut block.stmts, liftable, consts);
    if let Some(tail) = &mut block.tail {
        rewrite_expr_pointer_views(tail, liftable);
    }
}

fn rewrite_expr_pointer_views(expr: &mut Expr, liftable: &BTreeSet<String>) {
    if let Some(source) = pointer_view_source(expr)
        && liftable.contains(source)
    {
        *expr = Expr::Var(source.into());
        return;
    }
    match expr {
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => rewrite_expr_pointer_views(expr, liftable),
        Expr::Binary { lhs, rhs, .. } => {
            rewrite_expr_pointer_views(lhs, liftable);
            rewrite_expr_pointer_views(rhs, liftable);
        }
        Expr::Call { func, args } => {
            rewrite_expr_pointer_views(func, liftable);
            for arg in args {
                rewrite_expr_pointer_views(arg, liftable);
            }
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            rewrite_expr_pointer_views(recv, liftable);
            for arg in args {
                rewrite_expr_pointer_views(arg, liftable);
            }
        }
        Expr::Field { base, .. }
        | Expr::TupleField { base, .. }
        | Expr::ArrayPtr { array: base, .. } => rewrite_expr_pointer_views(base, liftable),
        Expr::Index { base, index } => {
            rewrite_expr_pointer_views(base, liftable);
            rewrite_expr_pointer_views(index, liftable);
        }
        Expr::StructLit { fields, .. } => {
            for (_, value) in fields {
                rewrite_expr_pointer_views(value, liftable);
            }
        }
        Expr::ArrayLit(elems) => {
            for elem in elems {
                rewrite_expr_pointer_views(elem, liftable);
            }
        }
        Expr::ArrayRepeat { elem, .. } => rewrite_expr_pointer_views(elem, liftable),
        Expr::Macro { args, .. } => {
            for arg in args {
                rewrite_expr_pointer_views(arg, liftable);
            }
        }
        Expr::Closure { body, .. } => rewrite_expr_pointer_views(body, liftable),
        Expr::Match { expr, arms } => {
            rewrite_expr_pointer_views(expr, liftable);
            for arm in arms {
                rewrite_expr_pointer_views(&mut arm.value, liftable);
            }
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            rewrite_expr_pointer_views(cond, liftable);
            rewrite_expr_pointer_views(then_expr, liftable);
            rewrite_expr_pointer_views(else_expr, liftable);
        }
        Expr::Block(block) | Expr::Unsafe(block) => {
            rewrite_block_pointer_views(block, liftable, &BTreeMap::new())
        }
        Expr::CopyNonoverlapping { src, dst, .. } => {
            rewrite_expr_pointer_views(src, liftable);
            rewrite_expr_pointer_views(dst, liftable);
        }
        Expr::PtrCopy {
            src, dst, count, ..
        } => {
            rewrite_expr_pointer_views(src, liftable);
            rewrite_expr_pointer_views(dst, liftable);
            rewrite_expr_pointer_views(count, liftable);
        }
        Expr::WriteBytes { dst, val, count } => {
            rewrite_expr_pointer_views(dst, liftable);
            rewrite_expr_pointer_views(val, liftable);
            rewrite_expr_pointer_views(count, liftable);
        }
        Expr::AtomicRef { ptr, .. } | Expr::AtomicLoad { ptr, .. } => {
            rewrite_expr_pointer_views(ptr, liftable);
        }
        Expr::AtomicStore { ptr, value, .. }
        | Expr::AtomicFetch { ptr, value, .. }
        | Expr::AtomicSwap { ptr, value, .. } => {
            rewrite_expr_pointer_views(ptr, liftable);
            rewrite_expr_pointer_views(value, liftable);
        }
        Expr::AtomicCompareExchange {
            ptr,
            expected,
            desired,
            ..
        } => {
            rewrite_expr_pointer_views(ptr, liftable);
            rewrite_expr_pointer_views(expected, liftable);
            rewrite_expr_pointer_views(desired, liftable);
        }
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::Var(_)
        | Expr::Path(_)
        | Expr::AtomicFence { .. }
        | Expr::Todo(_) => {}
    }
}

fn supported_copy_call(
    expr: &Expr,
    liftable: &BTreeSet<String>,
    consts: &BTreeMap<String, usize>,
) -> Option<Stmt> {
    let Expr::Call { func, args } = peel_empty_unsafe(expr) else {
        return None;
    };
    let Expr::Var(name) = &**func else {
        return None;
    };
    match name.as_str() {
        "strcpy" if args.len() == 2 => {
            let dst = pointer_view_source(&args[0])?;
            if !liftable.contains(dst) {
                return None;
            }
            let src = source_expr(&args[1], liftable)?;
            Some(Stmt::Assign {
                target: Expr::Var(dst.into()),
                value: to_owned(source_as_str(src)),
            })
        }
        "strncpy" if args.len() == 3 => {
            let dst = pointer_view_source(&args[0])?;
            if !liftable.contains(dst) {
                return None;
            }
            let src = const_ascii_prefix(&args[1], &args[2], consts)?;
            Some(Stmt::Assign {
                target: Expr::Var(dst.into()),
                value: to_owned(Expr::Str(src)),
            })
        }
        "strcat" if args.len() == 2 => {
            let dst = pointer_view_source(&args[0])?;
            if !liftable.contains(dst) {
                return None;
            }
            let src = source_expr(&args[1], liftable)?;
            Some(Stmt::Expr(Expr::MethodCall {
                recv: Box::new(Expr::Var(dst.into())),
                method: "push_str".into(),
                args: vec![source_as_str(src)],
            }))
        }
        "strncat" if args.len() == 3 => {
            let dst = pointer_view_source(&args[0])?;
            if !liftable.contains(dst) {
                return None;
            }
            let src = const_ascii_prefix(&args[1], &args[2], consts)?;
            Some(Stmt::Expr(Expr::MethodCall {
                recv: Box::new(Expr::Var(dst.into())),
                method: "push_str".into(),
                args: vec![Expr::Str(src)],
            }))
        }
        _ => None,
    }
}

fn const_ascii_prefix(
    src: &Expr,
    count: &Expr,
    consts: &BTreeMap<String, usize>,
) -> Option<String> {
    let bytes = const_c_string(src)?;
    if !bytes.is_ascii() {
        return None;
    }
    let n = const_usize(count, consts)?;
    String::from_utf8(bytes[..std::cmp::min(n, bytes.len())].to_vec()).ok()
}

fn source_expr(expr: &Expr, liftable: &BTreeSet<String>) -> Option<Source> {
    if let Some(bytes) = const_c_string(expr) {
        return String::from_utf8(bytes)
            .ok()
            .map(Expr::Str)
            .map(Source::Str);
    }
    let source = pointer_view_source(expr)?;
    if liftable.contains(source) {
        return Some(Source::Owned(source.into()));
    }
    None
}

fn source_as_str(source: Source) -> Expr {
    match source {
        Source::Str(expr) => expr,
        Source::Owned(name) => Expr::MethodCall {
            recv: Box::new(Expr::Var(name.into())),
            method: "as_str".into(),
            args: Vec::new(),
        },
    }
}

fn to_owned(expr: Expr) -> Expr {
    Expr::MethodCall {
        recv: Box::new(expr),
        method: "to_owned".into(),
        args: Vec::new(),
    }
}

fn const_usize(expr: &Expr, consts: &BTreeMap<String, usize>) -> Option<usize> {
    match expr {
        Expr::Var(name) => consts.get(name.as_str()).copied(),
        Expr::Value(RustValue::I64(n)) => usize::try_from(*n).ok(),
        Expr::Value(RustValue::I128(n)) => usize::try_from(*n).ok(),
        Expr::Cast { expr, .. } => const_usize(expr, consts),
        _ => None,
    }
}

fn const_c_string(expr: &Expr) -> Option<Vec<u8>> {
    match expr {
        Expr::Str(s) => Some(s.as_bytes().to_vec()),
        Expr::ByteStr(bytes) => Some(bytes.strip_suffix(&[0]).unwrap_or(bytes).to_vec()),
        Expr::Cast { expr, .. } => const_c_string(expr),
        Expr::MethodCall { recv, method, args } if method == "as_ptr" && args.is_empty() => {
            const_c_string(recv)
        }
        _ => None,
    }
}

fn pointer_view_source(expr: &Expr) -> Option<&str> {
    match expr {
        Expr::MethodCall { recv, method, args }
            if args.is_empty() && matches!(method.as_str(), "as_ptr" | "as_mut_ptr") =>
        {
            match &**recv {
                Expr::Var(v) => Some(v.as_str()),
                _ => None,
            }
        }
        Expr::ArrayPtr { array, .. } => match &**array {
            Expr::Var(v) => Some(v.as_str()),
            _ => None,
        },
        Expr::Cast { expr, .. }
        | Expr::Unary { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => pointer_view_source(expr),
        _ => None,
    }
}

fn peel_empty_unsafe(expr: &Expr) -> &Expr {
    if let Expr::Unsafe(block) = expr
        && block.stmts.is_empty()
        && let Some(tail) = &block.tail
    {
        return tail;
    }
    expr
}

fn is_copy_func(name: &str) -> bool {
    matches!(name, "strcpy" | "strncpy" | "strcat" | "strncat")
}

fn prune_unused_externs(program: &mut Program) {
    let used = copy_calls(program);
    program.items.retain_mut(|item| match item {
        Item::ExternBlock { decls, .. } => {
            decls.retain(|decl| match decl {
                ExternDecl::Fn(f) if is_copy_func(&f.name) => used.contains(&f.name),
                _ => true,
            });
            !decls.is_empty()
        }
        _ => true,
    });
}

fn copy_calls(program: &Program) -> Vec<String> {
    let mut calls = Vec::new();
    for item in &program.items {
        if let Item::Fn(f) = item {
            collect_copy_calls_body(&f.body, &mut calls);
        }
    }
    calls.sort();
    calls.dedup();
    calls
}

fn collect_copy_calls_body(body: &[IndentStmt], calls: &mut Vec<String>) {
    for indent in body {
        collect_copy_calls_stmt(&indent.stmt, calls);
    }
}

fn collect_copy_calls_block(block: &Block, calls: &mut Vec<String>) {
    collect_copy_calls_body(&block.stmts, calls);
    if let Some(tail) = &block.tail {
        collect_copy_calls_expr(tail, calls);
    }
}

fn collect_copy_calls_stmt(stmt: &Stmt, calls: &mut Vec<String>) {
    match stmt {
        Stmt::Let { init, .. } => {
            if let Some(expr) = init {
                collect_copy_calls_expr(expr, calls);
            }
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            collect_copy_calls_expr(target, calls);
            collect_copy_calls_expr(value, calls);
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => collect_copy_calls_expr(expr, calls),
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            collect_copy_calls_expr(cond, calls);
            collect_copy_calls_body(then_body, calls);
            collect_copy_calls_body(else_body, calls);
        }
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            collect_copy_calls_expr(cond, calls);
            collect_copy_calls_body(then_body, calls);
            collect_copy_calls_expr(then_value, calls);
            collect_copy_calls_body(else_body, calls);
            collect_copy_calls_expr(else_value, calls);
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            collect_copy_calls_body(body, calls);
        }
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
            collect_copy_calls_block(body, calls);
        }
        Stmt::Match { expr, arms } => {
            collect_copy_calls_expr(expr, calls);
            for arm in arms {
                collect_copy_calls_body(&arm.body, calls);
            }
        }
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
    }
}

fn collect_copy_calls_expr(expr: &Expr, calls: &mut Vec<String>) {
    if let Expr::Call { func, .. } = expr
        && let Expr::Var(name) = &**func
        && is_copy_func(name.as_str())
    {
        calls.push(name.as_str().into());
    }
    expr_children_any(expr, &mut |expr| {
        collect_copy_calls_expr(expr, calls);
        false
    });
}

fn expr_any(expr: &Expr, pred: &mut impl FnMut(&Expr) -> bool) -> bool {
    pred(expr) || expr_children_any(expr, &mut |expr| expr_any(expr, pred))
}

fn expr_children_any(expr: &Expr, pred: &mut impl FnMut(&Expr) -> bool) -> bool {
    match expr {
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. }
        | Expr::Closure { body: expr, .. }
        | Expr::AtomicRef { ptr: expr, .. }
        | Expr::AtomicLoad { ptr: expr, .. } => pred(expr),
        Expr::Binary { lhs, rhs, .. }
        | Expr::Index {
            base: lhs,
            index: rhs,
        } => pred(lhs) || pred(rhs),
        Expr::Call { func, args } => pred(func) || args.iter().any(pred),
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            pred(recv) || args.iter().any(pred)
        }
        Expr::Field { base, .. }
        | Expr::TupleField { base, .. }
        | Expr::ArrayPtr { array: base, .. } => pred(base),
        Expr::StructLit { fields, .. } => fields.iter().any(|(_, value)| pred(value)),
        Expr::ArrayLit(elems) => elems.iter().any(pred),
        Expr::ArrayRepeat { elem, .. } => pred(elem),
        Expr::Macro { args, .. } => args.iter().any(pred),
        Expr::Match { expr, arms } => pred(expr) || arms.iter().any(|arm| pred(&arm.value)),
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => pred(cond) || pred(then_expr) || pred(else_expr),
        Expr::Block(block) | Expr::Unsafe(block) => {
            block
                .stmts
                .iter()
                .any(|indent| stmt_expr_any(&indent.stmt, pred))
                || block.tail.as_deref().is_some_and(pred)
        }
        Expr::AtomicStore { ptr, value, .. }
        | Expr::AtomicFetch { ptr, value, .. }
        | Expr::AtomicSwap { ptr, value, .. } => pred(ptr) || pred(value),
        Expr::AtomicCompareExchange {
            ptr,
            expected,
            desired,
            ..
        } => pred(ptr) || pred(expected) || pred(desired),
        Expr::CopyNonoverlapping { src, dst, .. } => pred(src) || pred(dst),
        Expr::PtrCopy {
            src, dst, count, ..
        } => pred(src) || pred(dst) || pred(count),
        Expr::WriteBytes { dst, val, count } => pred(dst) || pred(val) || pred(count),
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::Var(_)
        | Expr::Path(_)
        | Expr::Todo(_)
        | Expr::AtomicFence { .. } => false,
    }
}

fn stmt_expr_any(stmt: &Stmt, pred: &mut impl FnMut(&Expr) -> bool) -> bool {
    match stmt {
        Stmt::Let { init, .. } => init.as_ref().is_some_and(pred),
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            pred(target) || pred(value)
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => pred(expr),
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            pred(cond)
                || then_body
                    .iter()
                    .any(|indent| stmt_expr_any(&indent.stmt, pred))
                || else_body
                    .iter()
                    .any(|indent| stmt_expr_any(&indent.stmt, pred))
        }
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            pred(cond)
                || then_body
                    .iter()
                    .any(|indent| stmt_expr_any(&indent.stmt, pred))
                || pred(then_value)
                || else_body
                    .iter()
                    .any(|indent| stmt_expr_any(&indent.stmt, pred))
                || pred(else_value)
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            body.iter().any(|indent| stmt_expr_any(&indent.stmt, pred))
        }
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
            body.stmts
                .iter()
                .any(|indent| stmt_expr_any(&indent.stmt, pred))
                || body.tail.as_deref().is_some_and(pred)
        }
        Stmt::Match { expr, arms } => {
            pred(expr)
                || arms.iter().any(|arm| {
                    arm.body
                        .iter()
                        .any(|indent| stmt_expr_any(&indent.stmt, pred))
                })
        }
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => false,
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;

    fn run(stmts: Vec<Stmt>) -> String {
        let mut program = Program {
            items: vec![Item::Fn(func(Vec::new(), None, stmts))],
        };
        let analyzed = crate::fixups::facts::analyze(program.clone());
        fixup(&mut program, &analyzed.facts);
        program.emit()
    }

    #[test]
    fn rewrites_strcpy_to_owned_string_assignment() {
        let out = run(vec![
            Stmt::Let {
                name: "dst".into(),
                mutable: true,
                ty: Some(Type::Array {
                    elem: Box::new(Type::Prim(Prim::I8)),
                    len: 8,
                }),
                init: Some(Expr::ArrayRepeat {
                    elem: Box::new(int(0)),
                    len: 8,
                }),
            },
            Stmt::Expr(Expr::Unsafe(Box::new(Block {
                stmts: vec![],
                tail: Some(Box::new(call(
                    "strcpy",
                    vec![as_mut_ptr("dst"), c_string("abc")],
                ))),
            }))),
        ]);
        assert!(out.contains("let mut dst: String = \"\".to_owned();"));
        assert!(out.contains("dst = \"abc\".to_owned();"));
        assert!(!out.contains("strcpy("));
    }

    #[test]
    fn rewrites_strcat_to_push_str() {
        let out = run(vec![
            Stmt::Let {
                name: "dst".into(),
                mutable: true,
                ty: Some(Type::Array {
                    elem: Box::new(Type::Prim(Prim::I8)),
                    len: 8,
                }),
                init: Some(Expr::ArrayLit(vec![int(104), int(105), int(0)])),
            },
            Stmt::Expr(Expr::Unsafe(Box::new(Block {
                stmts: vec![],
                tail: Some(Box::new(call(
                    "strcat",
                    vec![as_mut_ptr("dst"), c_string("!")],
                ))),
            }))),
        ]);
        assert!(out.contains("let mut dst: String = \"hi\".to_owned();"));
        assert!(out.contains("dst.push_str(\"!\");"));
        assert!(!out.contains("strcat("));
    }

    fn as_mut_ptr(name: &str) -> Expr {
        Expr::MethodCall {
            recv: Box::new(Expr::Var(name.into())),
            method: "as_mut_ptr".into(),
            args: Vec::new(),
        }
    }

    fn c_string(text: &str) -> Expr {
        let mut bytes = text.as_bytes().to_vec();
        bytes.push(0);
        Expr::MethodCall {
            recv: Box::new(Expr::ByteStr(bytes)),
            method: "as_ptr".into(),
            args: Vec::new(),
        }
    }
}
