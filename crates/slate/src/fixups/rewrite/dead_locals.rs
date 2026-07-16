use crate::fixups::facts::{
    AstPath, EffectKind, EffectSubject, FixupFacts, FunctionId, PathSegment,
};
use crate::fixups::support::walk;
use crate::rust_ast::{Expr, IndentStmt, Stmt};

pub(in crate::fixups) fn fixup(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
) -> bool {
    fixup_at(body, function, facts, &mut Vec::new())
}

fn fixup_at(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
) -> bool {
    for index in 0..body.len() {
        let mut changed = false;
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            walk::nested_body_vecs_mut_with_path(&mut body[index].stmt, path, &mut |body, path| {
                if !changed {
                    changed = fixup_at(body, function, facts, path);
                }
            });
        });
        if changed {
            return true;
        }
    }

    for index in 0..body.len() {
        let stmt_path = stmt_path(path, index);
        if removable_dead_local(&body[index].stmt, function, facts, &stmt_path) {
            body.remove(index);
            return true;
        }
    }
    false
}

fn removable_dead_local(
    stmt: &Stmt,
    function: FunctionId,
    facts: &FixupFacts,
    path: &[PathSegment],
) -> bool {
    let Stmt::Let {
        name,
        init: Some(init),
        ..
    } = stmt
    else {
        return false;
    };
    let Some(binding) = facts.binding_by_local_path(function, name, &AstPath(path.to_vec())) else {
        return false;
    };
    facts
        .def_use(binding)
        .is_some_and(|def_use| def_use.reads.is_empty() && def_use.writes.is_empty())
        && discardable_init(init, function, facts, path)
}

fn discardable_init(
    expr: &Expr,
    function: FunctionId,
    facts: &FixupFacts,
    path: &[PathSegment],
) -> bool {
    if discardable_known_method(expr) {
        return true;
    }
    let Some(effect) = facts.effect(function, EffectSubject::Expr, &AstPath(path.to_vec())) else {
        return false;
    };
    effect.effects.iter().all(discardable_effect)
}

fn discardable_effect(effect: &EffectKind) -> bool {
    !matches!(
        effect,
        EffectKind::ReadOnlyCall
            | EffectKind::UnknownCall
            | EffectKind::MethodCall
            | EffectKind::MacroExpansion
            | EffectKind::VolatileRead
            | EffectKind::VolatileWrite
            | EffectKind::AtomicRead
            | EffectKind::AtomicWrite
            | EffectKind::MemoryWrite
            | EffectKind::UnknownSideEffect
    )
}

fn discardable_known_method(expr: &Expr) -> bool {
    match expr {
        Expr::Cast { expr, .. } | Expr::Transmute { expr, .. } => discardable_known_method(expr),
        Expr::MethodCall { recv, method, args } if method == "len" && args.is_empty() => {
            discardable_receiver(recv)
        }
        _ => false,
    }
}

fn discardable_receiver(expr: &Expr) -> bool {
    match expr {
        Expr::Var(_) => true,
        Expr::MethodCall { recv, method, args }
            if matches!(method.as_str(), "as_slice" | "as_mut_slice") && args.is_empty() =>
        {
            discardable_receiver(recv)
        }
        _ => false,
    }
}

fn stmt_path(body_path: &[PathSegment], index: usize) -> Vec<PathSegment> {
    let mut path = body_path.to_vec();
    path.push(PathSegment::Stmt(index));
    path
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::facts;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{BinOp, Item, Program, Type};

    fn after(stmts: Vec<Stmt>) -> String {
        let mut program = Program {
            items: vec![Item::Fn(func(vec![param("items", "&[i32]")], None, stmts))],
        };
        loop {
            let analyzed = facts::analyze(program.clone());
            let Item::Fn(f) = &mut program.items[0] else {
                unreachable!();
            };
            if !fixup(&mut f.body, facts::FunctionId(0), &analyzed.facts) {
                break;
            }
        }
        program.emit()
    }

    fn len_call() -> Expr {
        Expr::Cast {
            expr: Box::new(Expr::MethodCall {
                recv: Box::new(var("items")),
                method: "len".into(),
                args: vec![],
            }),
            ty: Type::parse("i32"),
        }
    }

    #[test]
    fn removes_unused_local_with_discardable_len_init() {
        let out = after(vec![
            let_mut("len", "i32", len_call()),
            let_mut("total", "i32", int(0)),
            Stmt::Return(Some(var("total"))),
        ]);

        assert!(!out.contains("len"));
        assert!(out.contains("let mut total: i32 = 0;"));
        assert!(out.contains("return total;"));
    }

    #[test]
    fn removes_unused_local_with_pure_value_init() {
        let out = after(vec![
            let_mut("literal", "i32", int(0)),
            let_mut("copy", "i32", var("items_len")),
            let_mut("math", "i32", bin(BinOp::Add, var("items_len"), int(1))),
            let_mut("total", "i32", int(0)),
            Stmt::Return(Some(var("total"))),
        ]);

        assert!(!out.contains("literal"));
        assert!(!out.contains("copy"));
        assert!(!out.contains("math"));
        assert!(out.contains("let mut total: i32 = 0;"));
    }

    #[test]
    fn keeps_unused_local_with_effectful_init() {
        let out = after(vec![
            let_mut("len", "i32", call("next_len", vec![])),
            let_mut("total", "i32", int(0)),
        ]);

        assert!(out.contains("let mut len: i32 = next_len();"));
    }

    #[test]
    fn keeps_unused_local_with_volatile_read_init() {
        let out = after(vec![
            let_mut(
                "read",
                "i32",
                Expr::Call {
                    func: Box::new(Expr::Path(crate::rust_ast::Path::new([
                        "std".into(),
                        "ptr".into(),
                        "read_volatile".into(),
                    ]))),
                    args: vec![Expr::AddrOf {
                        mutable: false,
                        expr: Box::new(var("items_len")),
                    }],
                },
            ),
            let_mut("total", "i32", int(0)),
            Stmt::Return(Some(var("total"))),
        ]);

        assert!(out.contains("let mut read: i32 = std::ptr::read_volatile"));
    }

    #[test]
    fn keeps_read_local() {
        let out = after(vec![
            let_mut("len", "i32", len_call()),
            let_mut("total", "i32", var("len")),
            Stmt::Return(Some(var("total"))),
        ]);

        assert!(out.contains("let mut len: i32 = items.len() as i32;"));
    }
}
