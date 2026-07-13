use crate::fixups::facts::{
    AstPath, FixupFacts, FunctionId, PathSegment, StringBufferProvenance, StringRecoveryCandidate,
};
use crate::fixups::support::walk;
use crate::rust_ast::{Expr, IndentStmt, Prim, RustValue, Stmt, Type};
use std::collections::BTreeSet;

pub(in crate::fixups) fn fixup(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
) {
    fixup_nested(body, function, facts, &mut Vec::new());
    fixup_body(body, function, facts, &mut Vec::new());
}

fn fixup_body(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
) {
    let liftable = liftable_names(body, function, facts, path);
    let mut i = 0;
    while i < body.len() {
        let stmt_path = stmt_path(path, i);
        let Some((name, lifted, remove_index)) = lift_candidate(body, function, facts, &stmt_path)
        else {
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

fn liftable_names(
    body: &[IndentStmt],
    function: FunctionId,
    facts: &FixupFacts,
    path: &[PathSegment],
) -> BTreeSet<String> {
    let candidates = (0..body.len())
        .filter_map(|i| {
            lift_candidate(body, function, facts, &stmt_path(path, i))
                .map(|(name, _, remove_index)| (name, i, remove_index))
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

fn fixup_nested(
    body: &mut [IndentStmt],
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
) {
    for (index, indent) in body.iter_mut().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
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
                    walk::with_path_segment(path, PathSegment::Then, |path| {
                        fixup_nested(then_body, function, facts, path);
                        fixup_body(then_body, function, facts, path);
                    });
                    walk::with_path_segment(path, PathSegment::Else, |path| {
                        fixup_nested(else_body, function, facts, path);
                        fixup_body(else_body, function, facts, path);
                    });
                }
                Stmt::Loop { body, .. }
                | Stmt::Scope { body }
                | Stmt::LabeledBlock { body, .. } => {
                    fixup_nested(body, function, facts, path);
                    fixup_body(body, function, facts, path);
                }
                Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
                    fixup_nested(&mut body.stmts, function, facts, path);
                    fixup_body(&mut body.stmts, function, facts, path);
                }
                Stmt::Match { arms, .. } => {
                    for (arm_index, arm) in arms.iter_mut().enumerate() {
                        walk::with_path_segment(path, PathSegment::MatchArm(arm_index), |path| {
                            fixup_nested(&mut arm.body, function, facts, path);
                            fixup_body(&mut arm.body, function, facts, path);
                        });
                    }
                }
                _ => {}
            }
        });
    }
}

struct Lifted {
    ty: Type,
    expr: Expr,
}

fn lift_candidate(
    body: &[IndentStmt],
    function: FunctionId,
    facts: &FixupFacts,
    path: &[PathSegment],
) -> Option<(String, Lifted, Option<usize>)> {
    let buffer = facts.string_buffer_at(function, &AstPath(path.to_vec()))?;
    let name = facts.binding_name(buffer.binding)?.to_owned();
    let lifted = lifted_buffer(buffer)?;
    let remove_index = match &buffer.provenance {
        StringBufferProvenance::Literal => {
            if facts
                .def_use(buffer.binding)
                .is_some_and(|def_use| !def_use.writes.is_empty())
            {
                return None;
            }
            None
        }
        StringBufferProvenance::AssignedLiteral { assignment } => {
            Some(assignment_index(path, &assignment.0)?)
        }
        _ => return None,
    };
    if remove_index.is_some_and(|index| index >= body.len()) {
        return None;
    }
    Some((name, lifted, remove_index))
}

fn lifted_buffer(buffer: &crate::fixups::facts::StringBufferFact) -> Option<Lifted> {
    let bytes = buffer.bytes.clone()?;
    if buffer
        .candidates
        .contains(&StringRecoveryCandidate::BorrowedStr)
    {
        let text = String::from_utf8(bytes).ok()?;
        return Some(Lifted {
            ty: str_ref_type(),
            expr: Expr::Str(text),
        });
    }
    if buffer
        .candidates
        .contains(&StringRecoveryCandidate::BorrowedBytes)
    {
        return Some(Lifted {
            ty: byte_slice_ref_type(),
            expr: Expr::ByteStr(bytes),
        });
    }
    None
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

fn stmt_allows_lift(stmt: &Stmt, name: &str, liftable: &BTreeSet<String>) -> bool {
    walk::stmt_exprs_all_with(
        stmt,
        &mut |stmt| match stmt {
            Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
                Some(!expr_mentions_var(target, name) && expr_allows_lift(value, name, liftable))
            }
            _ => None,
        },
        &mut |expr| expr_allows_lift_override(expr, name, liftable),
    )
}

fn expr_allows_lift(expr: &Expr, name: &str, liftable: &BTreeSet<String>) -> bool {
    walk::exprs_all_with(expr, &mut |expr| {
        expr_allows_lift_override(expr, name, liftable)
    })
}

fn expr_allows_lift_override(expr: &Expr, name: &str, liftable: &BTreeSet<String>) -> Option<bool> {
    match expr {
        Expr::Var(v) if v.as_str() == name => false,
        Expr::MethodCall { recv, method, args }
            if args.is_empty()
                && matches!(&**recv, Expr::Var(v) if v.as_str() == name)
                && matches!(method.as_str(), "as_ptr" | "as_mut_ptr") =>
        {
            false
        }
        Expr::ArrayPtr { array, .. } if matches!(&**array, Expr::Var(v) if v.as_str() == name) => {
            false
        }
        Expr::Call { func, args } if matches!(&**func, Expr::Var(callee) if callee.as_str() == "printf") => {
            if args.iter().any(|arg| expr_has_pointer_view(arg, name)) {
                printf_call_allows_lift(args, name)
            } else {
                args.iter().all(|arg| expr_allows_lift(arg, name, liftable))
            }
        }
        Expr::Call { func, args } if matches!(&**func, Expr::Var(callee) if matches!(callee.as_str(), "strlen" | "strcmp" | "strncmp" | "memcmp")) => {
            libc_string_call_allows_lift(args, name, liftable)
        }
        _ => return None,
    }
    .into()
}

fn expr_mentions_var(expr: &Expr, name: &str) -> bool {
    !expr_allows_lift(expr, name, &BTreeSet::new())
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
        .all(|arg| expr_allows_lift(arg, name, liftable))
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
                _ => expr_allows_lift(arg, name, &BTreeSet::new()),
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
    walk::stmt_exprs_mut_with(stmt, &mut |expr| rewrite_pointer_view_expr(expr, name));
}

fn rewrite_expr_pointer_views(expr: &mut Expr, name: &str) {
    walk::exprs_mut_with(expr, &mut |expr| rewrite_pointer_view_expr(expr, name));
}

fn rewrite_pointer_view_expr(expr: &mut Expr, name: &str) -> bool {
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
            false
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
            false
        }
        _ => true,
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Block, Expr, Item, Program, Stmt, Type};

    fn fixed(params: Vec<crate::rust_ast::FnParam>, ret: Option<&str>, stmts: Vec<Stmt>) -> String {
        let mut program = Program {
            items: vec![Item::Fn(func(params, ret, stmts))],
        };
        let analyzed = crate::fixups::facts::analyze(program.clone());
        let facts = analyzed.facts;
        let Item::Fn(f) = &mut program.items[0] else {
            unreachable!();
        };
        fixup(&mut f.body, FunctionId(0), &facts);
        program.emit()
    }

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
        let out = fixed(
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
        let out = fixed(
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
        let indexed = fixed(
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

        let mutated = fixed(
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
        let out = fixed(
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
        let out = fixed(
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
        let out = fixed(
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
