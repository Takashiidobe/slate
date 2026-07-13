use crate::rust_ast::{Block, Expr, IndentStmt, Prim, RustValue, Stmt, Type};
use std::collections::BTreeSet;

pub(super) fn fixup(body: &mut Vec<IndentStmt>) {
    fixup_nested(body);
    let liftable = liftable_names(body);
    let mut i = 0;
    while i < body.len() {
        let Some((name, lifted, remove_index)) = lift_candidate(body, i) else {
            i += 1;
            continue;
        };
        if !liftable.contains(&name) {
            i += 1;
            continue;
        }
        let scan_start = remove_index.map_or(i + 1, |index| index + 1);
        if body[scan_start..]
            .iter()
            .any(|indent| !stmt_allows_lift(&indent.stmt, &name, &liftable))
        {
            i += 1;
            continue;
        }
        let Stmt::Let {
            mutable, ty, init, ..
        } = &mut body[i].stmt
        else {
            unreachable!();
        };
        *mutable = false;
        *ty = Some(lifted.ty);
        *init = Some(lifted.expr);
        if let Some(remove_index) = remove_index {
            body.remove(remove_index);
        }
        for indent in body.iter_mut().skip(i + 1) {
            rewrite_stmt_pointer_views(&mut indent.stmt, &name);
        }
        i += 1;
    }
}

fn liftable_names(body: &[IndentStmt]) -> BTreeSet<String> {
    let candidates = (0..body.len())
        .filter_map(|i| {
            lift_candidate(body, i).map(|(name, _, remove_index)| (name, i, remove_index))
        })
        .collect::<Vec<_>>();
    let mut liftable = candidates
        .iter()
        .map(|(name, _, _)| name.clone())
        .collect::<BTreeSet<_>>();
    loop {
        let before = liftable.clone();
        liftable.retain(|name| {
            let Some((_, i, remove_index)) = candidates
                .iter()
                .find(|(candidate, _, _)| candidate == name)
                .cloned()
            else {
                return false;
            };
            let scan_start = remove_index.map_or(i + 1, |index| index + 1);
            body[scan_start..]
                .iter()
                .all(|indent| stmt_allows_lift(&indent.stmt, name, &before))
        });
        if liftable == before {
            return liftable;
        }
    }
}

fn fixup_nested(body: &mut [IndentStmt]) {
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
                fixup(then_body);
                fixup(else_body);
            }
            Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
                fixup(body);
            }
            Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
                fixup(&mut body.stmts);
            }
            Stmt::Match { arms, .. } => {
                for arm in arms {
                    fixup(&mut arm.body);
                }
            }
            _ => {}
        }
    }
}

struct Lifted {
    ty: Type,
    expr: Expr,
}

fn lift_candidate(body: &[IndentStmt], i: usize) -> Option<(String, Lifted, Option<usize>)> {
    let Stmt::Let {
        name,
        mutable: true,
        ty: Some(ty),
        init,
    } = &body.get(i)?.stmt
    else {
        return None;
    };
    if !is_char_array(ty) {
        return None;
    }
    if let Some(init) = init.as_ref().and_then(lift_array_literal) {
        return Some((name.clone(), init, None));
    }
    if !init.as_ref().is_some_and(is_zero_array) {
        return None;
    }
    for (index, indent) in body.iter().enumerate().skip(i + 1) {
        match &indent.stmt {
            Stmt::Assign {
                target: Expr::Var(target),
                value,
            } if target.as_str() == name => {
                return Some((name.clone(), lift_array_literal(value)?, Some(index)));
            }
            stmt if stmt_mentions_var(stmt, name) => return None,
            _ => {}
        }
    }
    None
}

fn is_char_array(ty: &Type) -> bool {
    matches!(
        ty,
        Type::Array { elem, .. }
            if matches!(&**elem, Type::Prim(Prim::I8 | Prim::U8))
    )
}

fn is_zero_array(expr: &Expr) -> bool {
    matches!(
        expr,
        Expr::ArrayRepeat { elem, .. }
            if matches!(&**elem, Expr::Value(RustValue::I64(0) | RustValue::I128(0)))
    )
}

fn lift_array_literal(expr: &Expr) -> Option<Lifted> {
    let Expr::ArrayLit(elems) = expr else {
        return None;
    };
    let mut bytes = elems.iter().map(byte_literal).collect::<Option<Vec<_>>>()?;
    if bytes.pop() != Some(0) {
        return None;
    }
    if bytes.contains(&0) {
        return Some(Lifted {
            ty: byte_slice_ref_type(),
            expr: Expr::ByteStr(bytes),
        });
    }
    match String::from_utf8(bytes.clone()) {
        Ok(text) => Some(Lifted {
            ty: str_ref_type(),
            expr: Expr::Str(text),
        }),
        Err(_) => Some(Lifted {
            ty: byte_slice_ref_type(),
            expr: Expr::ByteStr(bytes),
        }),
    }
}

fn str_ref_type() -> Type {
    Type::Ref {
        mutable: false,
        inner: Box::new(Type::Str),
    }
}

fn byte_slice_ref_type() -> Type {
    Type::Ref {
        mutable: false,
        inner: Box::new(Type::Slice(Box::new(Type::Prim(Prim::U8)))),
    }
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

fn stmt_allows_lift(stmt: &Stmt, name: &str, liftable: &BTreeSet<String>) -> bool {
    match stmt {
        Stmt::Let { init, .. } => init
            .as_ref()
            .is_none_or(|expr| expr_allows_lift(expr, name, false, liftable)),
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => {
            expr_allows_lift(expr, name, false, liftable)
        }
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => true,
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            !expr_mentions_var(target, name) && expr_allows_lift(value, name, false, liftable)
        }
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            expr_allows_lift(cond, name, false, liftable)
                && body_allows_lift(then_body, name, liftable)
                && body_allows_lift(else_body, name, liftable)
        }
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            expr_allows_lift(cond, name, false, liftable)
                && body_allows_lift(then_body, name, liftable)
                && expr_allows_lift(then_value, name, false, liftable)
                && body_allows_lift(else_body, name, liftable)
                && expr_allows_lift(else_value, name, false, liftable)
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            body_allows_lift(body, name, liftable)
        }
        Stmt::While { body, cond } => {
            expr_allows_lift(cond, name, false, liftable) && block_allows_lift(body, name, liftable)
        }
        Stmt::Block(body) | Stmt::Unsafe { body } => block_allows_lift(body, name, liftable),
        Stmt::Match { expr, arms } => {
            expr_allows_lift(expr, name, false, liftable)
                && arms
                    .iter()
                    .all(|arm| body_allows_lift(&arm.body, name, liftable))
        }
    }
}

fn body_allows_lift(body: &[IndentStmt], name: &str, liftable: &BTreeSet<String>) -> bool {
    body.iter()
        .all(|indent| stmt_allows_lift(&indent.stmt, name, liftable))
}

fn block_allows_lift(block: &Block, name: &str, liftable: &BTreeSet<String>) -> bool {
    body_allows_lift(&block.stmts, name, liftable)
        && block
            .tail
            .as_deref()
            .is_none_or(|expr| expr_allows_lift(expr, name, false, liftable))
}

fn expr_allows_lift(expr: &Expr, name: &str, in_printf: bool, liftable: &BTreeSet<String>) -> bool {
    match expr {
        Expr::Var(v) if v.as_str() == name => false,
        Expr::MethodCall { recv, method, args }
            if args.is_empty()
                && matches!(&**recv, Expr::Var(v) if v.as_str() == name)
                && matches!(method.as_str(), "as_ptr" | "as_mut_ptr") =>
        {
            in_printf
        }
        Expr::ArrayPtr { array, .. } if matches!(&**array, Expr::Var(v) if v.as_str() == name) => {
            in_printf
        }
        Expr::Call { func, args } if matches!(&**func, Expr::Var(callee) if callee.as_str() == "printf") => {
            if args.iter().any(|arg| expr_has_pointer_view(arg, name)) {
                printf_call_allows_lift(args, name)
            } else {
                args.iter()
                    .all(|arg| expr_allows_lift(arg, name, false, liftable))
            }
        }
        Expr::Call { func, args } if matches!(&**func, Expr::Var(callee) if matches!(callee.as_str(), "strlen" | "strcmp" | "strncmp" | "memcmp")) => {
            libc_string_call_allows_lift(args, name, liftable)
        }
        Expr::Call { func, args } => {
            expr_allows_lift(func, name, false, liftable)
                && args
                    .iter()
                    .all(|arg| expr_allows_lift(arg, name, false, liftable))
        }
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => expr_allows_lift(expr, name, in_printf, liftable),
        Expr::Binary { lhs, rhs, .. } => {
            expr_allows_lift(lhs, name, in_printf, liftable)
                && expr_allows_lift(rhs, name, in_printf, liftable)
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            expr_allows_lift(recv, name, in_printf, liftable)
                && args
                    .iter()
                    .all(|arg| expr_allows_lift(arg, name, in_printf, liftable))
        }
        Expr::Field { base, .. }
        | Expr::TupleField { base, .. }
        | Expr::ArrayPtr { array: base, .. } => expr_allows_lift(base, name, in_printf, liftable),
        Expr::Index { base, index } => {
            expr_allows_lift(base, name, in_printf, liftable)
                && expr_allows_lift(index, name, in_printf, liftable)
        }
        Expr::StructLit { fields, .. } => fields
            .iter()
            .all(|(_, value)| expr_allows_lift(value, name, in_printf, liftable)),
        Expr::ArrayLit(elems) => elems
            .iter()
            .all(|elem| expr_allows_lift(elem, name, in_printf, liftable)),
        Expr::ArrayRepeat { elem, .. } => expr_allows_lift(elem, name, in_printf, liftable),
        Expr::Macro { args, .. } => args
            .iter()
            .all(|arg| expr_allows_lift(arg, name, in_printf, liftable)),
        Expr::Closure { body, .. } => expr_allows_lift(body, name, in_printf, liftable),
        Expr::Match { expr, arms } => {
            expr_allows_lift(expr, name, in_printf, liftable)
                && arms
                    .iter()
                    .all(|arm| expr_allows_lift(&arm.value, name, in_printf, liftable))
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            expr_allows_lift(cond, name, in_printf, liftable)
                && expr_allows_lift(then_expr, name, in_printf, liftable)
                && expr_allows_lift(else_expr, name, in_printf, liftable)
        }
        Expr::Block(block) | Expr::Unsafe(block) => block_allows_lift(block, name, liftable),
        Expr::CopyNonoverlapping { src, dst, .. } => {
            expr_allows_lift(src, name, in_printf, liftable)
                && expr_allows_lift(dst, name, in_printf, liftable)
        }
        Expr::PtrCopy {
            src, dst, count, ..
        } => {
            expr_allows_lift(src, name, in_printf, liftable)
                && expr_allows_lift(dst, name, in_printf, liftable)
                && expr_allows_lift(count, name, in_printf, liftable)
        }
        Expr::WriteBytes { dst, val, count } => {
            expr_allows_lift(dst, name, in_printf, liftable)
                && expr_allows_lift(val, name, in_printf, liftable)
                && expr_allows_lift(count, name, in_printf, liftable)
        }
        Expr::AtomicRef { ptr, .. } | Expr::AtomicLoad { ptr, .. } => {
            expr_allows_lift(ptr, name, in_printf, liftable)
        }
        Expr::AtomicStore { ptr, value, .. }
        | Expr::AtomicFetch { ptr, value, .. }
        | Expr::AtomicSwap { ptr, value, .. } => {
            expr_allows_lift(ptr, name, in_printf, liftable)
                && expr_allows_lift(value, name, in_printf, liftable)
        }
        Expr::AtomicCompareExchange {
            ptr,
            expected,
            desired,
            ..
        } => {
            expr_allows_lift(ptr, name, in_printf, liftable)
                && expr_allows_lift(expected, name, in_printf, liftable)
                && expr_allows_lift(desired, name, in_printf, liftable)
        }
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::Path(_)
        | Expr::AtomicFence { .. }
        | Expr::Todo(_)
        | Expr::Var(_) => true,
    }
}

fn expr_mentions_var(expr: &Expr, name: &str) -> bool {
    !expr_allows_lift(expr, name, false, &BTreeSet::new())
}

fn stmt_mentions_var(stmt: &Stmt, name: &str) -> bool {
    !stmt_allows_lift(stmt, name, &BTreeSet::new())
}

fn expr_has_pointer_view(expr: &Expr, name: &str) -> bool {
    match expr {
        Expr::MethodCall { recv, method, args }
            if args.is_empty()
                && matches!(&**recv, Expr::Var(v) if v.as_str() == name)
                && matches!(method.as_str(), "as_ptr" | "as_mut_ptr") =>
        {
            true
        }
        Expr::ArrayPtr { array, .. } if matches!(&**array, Expr::Var(v) if v.as_str() == name) => {
            true
        }
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => expr_has_pointer_view(expr, name),
        _ => false,
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
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => pointer_view_source(expr),
        _ => None,
    }
}

fn libc_string_call_allows_lift(args: &[Expr], name: &str, liftable: &BTreeSet<String>) -> bool {
    let (pointer_args, other_args): (&[Expr], &[Expr]) = match args.len() {
        1 => (&args[..1], &[]),
        2 => (&args[..2], &[]),
        3 => (&args[..2], &args[2..]),
        _ => return false,
    };
    pointer_args.iter().all(|arg| {
        pointer_view_source(arg).is_some_and(|source| source == name || liftable.contains(source))
    }) && other_args
        .iter()
        .all(|arg| expr_allows_lift(arg, name, false, liftable))
}

fn printf_call_allows_lift(args: &[Expr], name: &str) -> bool {
    let Some((fmt, rest)) = args.split_first() else {
        return false;
    };
    let Some(conversions) = simple_printf_conversions(fmt) else {
        return false;
    };
    conversions.len() == rest.len()
        && conversions
            .iter()
            .zip(rest)
            .all(|(conversion, arg)| match conversion {
                b's' => expr_has_pointer_view(arg, name) || const_c_string(arg).is_some(),
                _ => expr_allows_lift(arg, name, false, &BTreeSet::new()),
            })
}

fn simple_printf_conversions(expr: &Expr) -> Option<Vec<u8>> {
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
                if !matches!(conv, b's' | b'c' | b'd' | b'i' | b'u' | b'x' | b'X' | b'o') {
                    return None;
                }
                conversions.push(conv);
                i += 2;
            }
            0x20..=0x7e | b'\n' | b'\t' => i += 1,
            _ => return None,
        }
    }
    Some(conversions)
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

fn rewrite_stmt_pointer_views(stmt: &mut Stmt, name: &str) {
    match stmt {
        Stmt::Let { init, .. } => {
            if let Some(init) = init {
                rewrite_expr_pointer_views(init, name);
            }
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => rewrite_expr_pointer_views(expr, name),
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            rewrite_expr_pointer_views(target, name);
            rewrite_expr_pointer_views(value, name);
        }
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            rewrite_expr_pointer_views(cond, name);
            rewrite_body_pointer_views(then_body, name);
            rewrite_body_pointer_views(else_body, name);
        }
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            rewrite_expr_pointer_views(cond, name);
            rewrite_body_pointer_views(then_body, name);
            rewrite_expr_pointer_views(then_value, name);
            rewrite_body_pointer_views(else_body, name);
            rewrite_expr_pointer_views(else_value, name);
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            rewrite_body_pointer_views(body, name);
        }
        Stmt::While { cond, body } => {
            rewrite_expr_pointer_views(cond, name);
            rewrite_block_pointer_views(body, name);
        }
        Stmt::Block(body) | Stmt::Unsafe { body } => rewrite_block_pointer_views(body, name),
        Stmt::Match { expr, arms } => {
            rewrite_expr_pointer_views(expr, name);
            for arm in arms {
                rewrite_body_pointer_views(&mut arm.body, name);
            }
        }
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
    }
}

fn rewrite_body_pointer_views(body: &mut [IndentStmt], name: &str) {
    for indent in body {
        rewrite_stmt_pointer_views(&mut indent.stmt, name);
    }
}

fn rewrite_block_pointer_views(block: &mut Block, name: &str) {
    rewrite_body_pointer_views(&mut block.stmts, name);
    if let Some(tail) = &mut block.tail {
        rewrite_expr_pointer_views(tail, name);
    }
}

fn rewrite_expr_pointer_views(expr: &mut Expr, name: &str) {
    match expr {
        Expr::MethodCall { recv, method, args }
            if args.is_empty()
                && matches!(&**recv, Expr::Var(v) if v.as_str() == name)
                && matches!(method.as_str(), "as_ptr" | "as_mut_ptr") =>
        {
            *expr = Expr::Cast {
                expr: Box::new(Expr::MethodCall {
                    recv: Box::new(Expr::Var(name.into())),
                    method: "as_ptr".into(),
                    args: Vec::new(),
                }),
                ty: Type::Ptr {
                    mutable: true,
                    inner: Box::new(Type::Prim(Prim::I8)),
                },
            };
        }
        Expr::ArrayPtr { array, .. } if matches!(&**array, Expr::Var(v) if v.as_str() == name) => {
            *expr = Expr::Cast {
                expr: Box::new(Expr::MethodCall {
                    recv: Box::new(Expr::Var(name.into())),
                    method: "as_ptr".into(),
                    args: Vec::new(),
                }),
                ty: Type::Ptr {
                    mutable: true,
                    inner: Box::new(Type::Prim(Prim::I8)),
                },
            };
        }
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => rewrite_expr_pointer_views(expr, name),
        Expr::Binary { lhs, rhs, .. } => {
            rewrite_expr_pointer_views(lhs, name);
            rewrite_expr_pointer_views(rhs, name);
        }
        Expr::Call { func, args } => {
            rewrite_expr_pointer_views(func, name);
            for arg in args {
                rewrite_expr_pointer_views(arg, name);
            }
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            rewrite_expr_pointer_views(recv, name);
            for arg in args {
                rewrite_expr_pointer_views(arg, name);
            }
        }
        Expr::Field { base, .. }
        | Expr::TupleField { base, .. }
        | Expr::ArrayPtr { array: base, .. } => {
            rewrite_expr_pointer_views(base, name);
        }
        Expr::Index { base, index } => {
            rewrite_expr_pointer_views(base, name);
            rewrite_expr_pointer_views(index, name);
        }
        Expr::StructLit { fields, .. } => {
            for (_, value) in fields {
                rewrite_expr_pointer_views(value, name);
            }
        }
        Expr::ArrayLit(elems) => {
            for elem in elems {
                rewrite_expr_pointer_views(elem, name);
            }
        }
        Expr::ArrayRepeat { elem, .. } => rewrite_expr_pointer_views(elem, name),
        Expr::Macro { args, .. } => {
            for arg in args {
                rewrite_expr_pointer_views(arg, name);
            }
        }
        Expr::Closure { body, .. } => rewrite_expr_pointer_views(body, name),
        Expr::Match { expr, arms } => {
            rewrite_expr_pointer_views(expr, name);
            for arm in arms {
                rewrite_expr_pointer_views(&mut arm.value, name);
            }
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            rewrite_expr_pointer_views(cond, name);
            rewrite_expr_pointer_views(then_expr, name);
            rewrite_expr_pointer_views(else_expr, name);
        }
        Expr::Block(block) | Expr::Unsafe(block) => rewrite_block_pointer_views(block, name),
        Expr::CopyNonoverlapping { src, dst, .. } => {
            rewrite_expr_pointer_views(src, name);
            rewrite_expr_pointer_views(dst, name);
        }
        Expr::PtrCopy {
            src, dst, count, ..
        } => {
            rewrite_expr_pointer_views(src, name);
            rewrite_expr_pointer_views(dst, name);
            rewrite_expr_pointer_views(count, name);
        }
        Expr::WriteBytes { dst, val, count } => {
            rewrite_expr_pointer_views(dst, name);
            rewrite_expr_pointer_views(val, name);
            rewrite_expr_pointer_views(count, name);
        }
        Expr::AtomicRef { ptr, .. } | Expr::AtomicLoad { ptr, .. } => {
            rewrite_expr_pointer_views(ptr, name);
        }
        Expr::AtomicStore { ptr, value, .. }
        | Expr::AtomicFetch { ptr, value, .. }
        | Expr::AtomicSwap { ptr, value, .. } => {
            rewrite_expr_pointer_views(ptr, name);
            rewrite_expr_pointer_views(value, name);
        }
        Expr::AtomicCompareExchange {
            ptr,
            expected,
            desired,
            ..
        } => {
            rewrite_expr_pointer_views(ptr, name);
            rewrite_expr_pointer_views(expected, name);
            rewrite_expr_pointer_views(desired, name);
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

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Block, Expr, Stmt, Type};

    fn bytes(values: &[i64]) -> Expr {
        Expr::ArrayLit(values.iter().copied().map(int).collect())
    }

    fn printf_arg(name: &str) -> Stmt {
        Stmt::Expr(Expr::Unsafe(Box::new(Block {
            stmts: vec![],
            tail: Some(Box::new(call(
                "printf",
                vec![
                    Expr::ByteStr(b"%s\n\0".to_vec()),
                    Expr::MethodCall {
                        recv: Box::new(var(name)),
                        method: "as_mut_ptr".into(),
                        args: vec![],
                    },
                ],
            ))),
        })))
    }

    #[test]
    fn lifts_utf8_nul_terminated_char_array_used_by_printf() {
        let out = after_body(
            fixup,
            vec![],
            None,
            vec![
                let_mut(
                    "s",
                    "[i8; 3]",
                    Expr::ArrayRepeat {
                        elem: Box::new(int(0)),
                        len: 3,
                    },
                ),
                assign("s", bytes(&[104, 105, 0])),
                printf_arg("s"),
            ],
        );

        assert_eq!(
            out,
            "\
fn f() {
    let s: &str = \"hi\";
    unsafe { printf(b\"%s\\n\\0\", s.as_ptr() as *mut i8) };
}
"
        );
    }

    #[test]
    fn lifts_non_utf8_bytes_to_slice() {
        let out = after_body(
            fixup,
            vec![],
            None,
            vec![
                let_mut(
                    "s",
                    "[i8; 3]",
                    Expr::ArrayRepeat {
                        elem: Box::new(int(0)),
                        len: 3,
                    },
                ),
                assign("s", bytes(&[255, 65, 0])),
                printf_arg("s"),
            ],
        );

        assert!(out.contains("let s: &[u8] = b\"\\xffA\";"));
    }

    #[test]
    fn leaves_indexed_or_mutated_buffers_raw() {
        let indexed = after_body(
            fixup,
            vec![],
            None,
            vec![
                let_mut(
                    "s",
                    "[i8; 3]",
                    Expr::ArrayRepeat {
                        elem: Box::new(int(0)),
                        len: 3,
                    },
                ),
                assign("s", bytes(&[104, 105, 0])),
                Stmt::Expr(Expr::Index {
                    base: Box::new(var("s")),
                    index: Box::new(int(0)),
                }),
            ],
        );
        assert!(indexed.contains("let mut s: [i8; 3] = [0; 3];"));

        let mutated = after_body(
            fixup,
            vec![],
            None,
            vec![
                let_mut(
                    "s",
                    "[i8; 3]",
                    Expr::ArrayRepeat {
                        elem: Box::new(int(0)),
                        len: 3,
                    },
                ),
                assign("s", bytes(&[104, 105, 0])),
                assign("s", bytes(&[98, 121, 0])),
            ],
        );
        assert!(mutated.contains("let mut s: [i8; 3] = [0; 3];"));
    }

    #[test]
    fn leaves_unliftable_initializer_raw() {
        let out = after_body(
            fixup,
            vec![],
            None,
            vec![Stmt::Let {
                name: "s".into(),
                mutable: true,
                ty: Some(Type::parse("[i8; 2]")),
                init: Some(bytes(&[104, 105])),
            }],
        );

        assert!(out.contains("let mut s: [i8; 2] = [104, 105];"));
    }

    #[test]
    fn leaves_buffers_raw_when_printf_would_remain_raw() {
        let out = after_body(
            fixup,
            vec![],
            None,
            vec![
                let_mut(
                    "s",
                    "[i8; 3]",
                    Expr::ArrayRepeat {
                        elem: Box::new(int(0)),
                        len: 3,
                    },
                ),
                assign("s", bytes(&[104, 105, 0])),
                Stmt::Expr(Expr::Unsafe(Box::new(Block {
                    stmts: vec![],
                    tail: Some(Box::new(call(
                        "printf",
                        vec![
                            Expr::ByteStr(b"%-4s\n\0".to_vec()),
                            Expr::ArrayPtr {
                                array: Box::new(var("s")),
                                mutable: true,
                            },
                        ],
                    ))),
                }))),
            ],
        );

        assert!(out.contains("let mut s: [i8; 3] = [0; 3];"));
        assert!(out.contains("s = [104, 105, 0];"));
    }

    #[test]
    fn lifts_hoisted_declaration_with_later_initializer() {
        let out = after_body(
            fixup,
            vec![],
            None,
            vec![
                let_mut(
                    "s",
                    "[i8; 3]",
                    Expr::ArrayRepeat {
                        elem: Box::new(int(0)),
                        len: 3,
                    },
                ),
                let_mut("other", "i32", int(0)),
                assign("other", int(1)),
                assign("s", bytes(&[104, 105, 0])),
                printf_arg("s"),
            ],
        );

        assert!(out.contains("let s: &str = \"hi\";"));
        assert!(!out.contains("s = [104, 105, 0];"));
    }
}
