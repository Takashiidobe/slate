use crate::fixups::facts::{
    AstPath, CountedLoopBound, CountedLoopIndexUse, CountedLoopStart, CountedLoopStep,
    CountedSliceLoopFact, FixupFacts, FunctionId, PathSegment, SliceLoopAccess,
};
use crate::fixups::support::walk;
use crate::rust_ast::{Expr, Ident, IndentStmt, Item, Program, RustValue, Stmt, UnaryOp};

pub(in crate::fixups) fn fixup(program: &mut Program, facts: &FixupFacts) -> bool {
    let mut changed = false;
    for (item_index, item) in program.items.iter_mut().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        changed |= rewrite_body(&mut f.body, function, facts, &mut Vec::new());
    }
    changed
}

fn rewrite_body(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
) -> bool {
    let mut changed = false;
    for (index, indent) in body.iter_mut().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            walk::nested_body_vecs_mut_with_path(&mut indent.stmt, path, &mut |body, path| {
                changed |= rewrite_body(body, function, facts, path);
            });
        });
    }

    for index in (0..body.len().saturating_sub(1)).rev() {
        let mut loop_path = path.to_vec();
        loop_path.push(PathSegment::Stmt(index + 1));
        let Some(fact) = loop_fact(function, facts, &AstPath(loop_path)) else {
            continue;
        };
        let Some(replacement) = replacement_for_pair(&body[index..index + 2], fact, facts) else {
            continue;
        };
        body.splice(index..index + 2, [replacement]);
        changed = true;
    }
    changed
}

fn loop_fact<'a>(
    function: FunctionId,
    facts: &'a FixupFacts,
    loop_path: &AstPath,
) -> Option<&'a CountedSliceLoopFact> {
    facts.counted_slice_loops.iter().find(|fact| {
        fact.site.function == function
            && fact.site.loop_path == *loop_path
            && fact.start == CountedLoopStart::Zero
            && fact.bound == CountedLoopBound::SliceLen
            && fact.step == CountedLoopStep::One
            && fact.index_use == CountedLoopIndexUse::SliceIndexOnly
    })
}

fn replacement_for_pair(
    pair: &[IndentStmt],
    fact: &CountedSliceLoopFact,
    facts: &FixupFacts,
) -> Option<IndentStmt> {
    let Stmt::Let {
        name: index_name, ..
    } = &pair[0].stmt
    else {
        return None;
    };
    let Stmt::Loop {
        body: loop_body, ..
    } = &pair[1].stmt
    else {
        return None;
    };
    if loop_body.len() < 2 {
        return None;
    }
    let slice_name = facts.binding_name(fact.slice)?.to_string();
    let method = match fact.access {
        SliceLoopAccess::ReadOnly => "iter",
        SliceLoopAccess::Mutable => "iter_mut",
    };
    let mut body = loop_body[1..loop_body.len() - 1].to_vec();
    let item_name = extract_materialized_item(&mut body, &slice_name, index_name.as_str())?;
    rewrite_index_uses(&mut body, &slice_name, index_name.as_str(), &item_name);
    Some(IndentStmt {
        depth: pair[1].depth,
        stmt: Stmt::For {
            pat: item_name,
            iter: Expr::MethodCall {
                recv: Box::new(Expr::Var(Ident::new(slice_name))),
                method: method.into(),
                args: Vec::new(),
            },
            body,
        },
    })
}

fn extract_materialized_item(
    body: &mut Vec<IndentStmt>,
    slice_name: &str,
    index_name: &str,
) -> Option<String> {
    for index in 0..body.len() {
        if let Some(name) = extract_assigned_materialized_item(body, index, slice_name, index_name)
        {
            return Some(name);
        }
        if let Stmt::Let {
            name,
            init: Some(init),
            ..
        } = &body[index].stmt
            && !is_synthetic_temp(name)
            && is_slice_index(init, slice_name, index_name)
        {
            let name = name.clone();
            body.remove(index);
            substitute_remaining(body, index, &name);
            return Some(name);
        }
        match &mut body[index].stmt {
            Stmt::Scope { body }
            | Stmt::LabeledBlock { body, .. }
            | Stmt::Loop { body, .. }
            | Stmt::For { body, .. } => {
                if let Some(name) = extract_materialized_item(body, slice_name, index_name) {
                    return Some(name);
                }
            }
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
                if let Some(name) = extract_materialized_item(then_body, slice_name, index_name) {
                    return Some(name);
                }
                if let Some(name) = extract_materialized_item(else_body, slice_name, index_name) {
                    return Some(name);
                }
            }
            Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
                if let Some(name) =
                    extract_materialized_item(&mut body.stmts, slice_name, index_name)
                {
                    return Some(name);
                }
            }
            Stmt::Match { arms, .. } => {
                for arm in arms {
                    if let Some(name) =
                        extract_materialized_item(&mut arm.body, slice_name, index_name)
                    {
                        return Some(name);
                    }
                }
            }
            Stmt::Let { .. }
            | Stmt::Assign { .. }
            | Stmt::CompoundAssign { .. }
            | Stmt::Expr(_)
            | Stmt::Return(_)
            | Stmt::Break(_)
            | Stmt::Continue(_) => {}
        }
    }
    None
}

fn extract_assigned_materialized_item(
    body: &mut Vec<IndentStmt>,
    index: usize,
    slice_name: &str,
    index_name: &str,
) -> Option<String> {
    let [
        IndentStmt {
            stmt:
                Stmt::Let {
                    name: item_name,
                    init: Some(item_init),
                    ..
                },
            ..
        },
        IndentStmt {
            stmt:
                Stmt::Let {
                    name: temp_name,
                    init: Some(temp_init),
                    ..
                },
            ..
        },
        IndentStmt {
            stmt:
                Stmt::Assign {
                    target,
                    value: assign_value,
                },
            ..
        },
    ] = body.get(index..index + 3)?
    else {
        return None;
    };
    if is_synthetic_temp(item_name)
        || !is_zero(item_init)
        || !is_synthetic_temp(temp_name)
        || !is_slice_index(temp_init, slice_name, index_name)
        || !matches!(target, Expr::Var(name) if name.as_str() == item_name)
        || !matches!(assign_value, Expr::Var(name) if name.as_str() == temp_name)
    {
        return None;
    }
    let item_name = item_name.clone();
    body.splice(index..index + 3, []);
    substitute_remaining(body, index, &item_name);
    Some(item_name)
}

fn substitute_remaining(body: &mut [IndentStmt], start: usize, name: &str) {
    let replacement = item_deref(name);
    for indent in &mut body[start..] {
        indent.stmt.substitute_var(name, &replacement);
    }
}

fn is_synthetic_temp(name: &str) -> bool {
    name.strip_prefix("_v")
        .is_some_and(|suffix| !suffix.is_empty() && suffix.chars().all(|ch| ch.is_ascii_digit()))
}

fn is_zero(expr: &Expr) -> bool {
    matches!(
        expr,
        Expr::Value(RustValue::I64(0) | RustValue::Usize(0) | RustValue::I128(0))
    )
}

fn rewrite_index_uses(
    body: &mut [IndentStmt],
    slice_name: &str,
    index_name: &str,
    item_name: &str,
) {
    for indent in body {
        walk::stmt_exprs_mut_with(&mut indent.stmt, &mut |expr| {
            if is_slice_index(expr, slice_name, index_name) {
                *expr = item_deref(item_name);
                return false;
            }
            true
        });
    }
}

fn is_slice_index(expr: &Expr, slice_name: &str, index_name: &str) -> bool {
    let Expr::Index { base, index } = expr else {
        return false;
    };
    matches!(&**base, Expr::Var(name) if name.as_str() == slice_name)
        && is_index_expr(index, index_name)
}

fn is_index_expr(expr: &Expr, index_name: &str) -> bool {
    match expr {
        Expr::Var(name) => name.as_str() == index_name,
        Expr::Cast { expr, .. } => is_index_expr(expr, index_name),
        _ => false,
    }
}

fn item_deref(item_name: &str) -> Expr {
    Expr::Unary {
        op: UnaryOp::Deref,
        expr: Box::new(Expr::Var(Ident::new(item_name))),
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::facts;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{BinOp, FnParam, Prim, Type};

    fn stmt(stmt: Stmt) -> IndentStmt {
        IndentStmt { depth: 2, stmt }
    }

    fn cast_usize(expr: Expr) -> Expr {
        Expr::Cast {
            expr: Box::new(expr),
            ty: Type::Prim(Prim::Usize),
        }
    }

    fn indexed_item() -> Expr {
        Expr::Index {
            base: Box::new(var("items")),
            index: Box::new(cast_usize(var("i"))),
        }
    }

    #[test]
    fn rewrites_materialized_item_temp_to_for_iter() {
        let mut program = Program {
            items: vec![Item::Fn(func(
                vec![FnParam {
                    name: "items".into(),
                    mutable: true,
                    ty: Type::parse("&mut [i32]"),
                }],
                Some("i32"),
                vec![
                    Stmt::Let {
                        name: "len".into(),
                        mutable: false,
                        ty: Some(Type::Prim(Prim::I32)),
                        init: Some(Expr::Cast {
                            expr: Box::new(Expr::MethodCall {
                                recv: Box::new(var("items")),
                                method: "len".into(),
                                args: vec![],
                            }),
                            ty: Type::Prim(Prim::I32),
                        }),
                    },
                    let_mut("total", "i32", int(0)),
                    Stmt::Scope {
                        body: vec![
                            stmt(let_mut("i", "i32", int(0))),
                            stmt(Stmt::Loop {
                                label: None,
                                body: vec![
                                    stmt(Stmt::If {
                                        cond: Expr::Unary {
                                            op: UnaryOp::Not,
                                            expr: Box::new(bin(BinOp::Lt, var("i"), var("len"))),
                                        },
                                        then_body: vec![stmt(Stmt::Break(None))],
                                        else_body: vec![],
                                    }),
                                    stmt(Stmt::Scope {
                                        body: vec![
                                            stmt(temp("item", "i32", indexed_item())),
                                            stmt(Stmt::CompoundAssign {
                                                target: var("total"),
                                                op: BinOp::Add,
                                                value: var("item"),
                                            }),
                                        ],
                                    }),
                                    stmt(Stmt::CompoundAssign {
                                        target: var("i"),
                                        op: BinOp::Add,
                                        value: int(1),
                                    }),
                                ],
                            }),
                        ],
                    },
                    Stmt::Return(Some(var("total"))),
                ],
            ))],
        };
        let facts = facts::analyze(program.clone()).facts;

        assert!(fixup(&mut program, &facts));
        let out = program.emit();

        assert!(out.contains("for item in items.iter()"));
        assert!(out.contains("total += *item;"));
        assert!(!out.contains("let item: i32 ="));
        assert!(!out.contains("loop {"));
        assert!(!out.contains("items[(i as usize)]"));
    }

    #[test]
    fn leaves_synthetic_element_temp_as_counted_loop() {
        let mut program = Program {
            items: vec![Item::Fn(func(
                vec![FnParam {
                    name: "items".into(),
                    mutable: true,
                    ty: Type::parse("&mut [i32]"),
                }],
                Some("i32"),
                vec![
                    Stmt::Let {
                        name: "len".into(),
                        mutable: false,
                        ty: Some(Type::Prim(Prim::I32)),
                        init: Some(Expr::Cast {
                            expr: Box::new(Expr::MethodCall {
                                recv: Box::new(var("items")),
                                method: "len".into(),
                                args: vec![],
                            }),
                            ty: Type::Prim(Prim::I32),
                        }),
                    },
                    let_mut("total", "i32", int(0)),
                    Stmt::Scope {
                        body: vec![
                            stmt(let_mut("i", "i32", int(0))),
                            stmt(Stmt::Loop {
                                label: None,
                                body: vec![
                                    stmt(Stmt::If {
                                        cond: Expr::Unary {
                                            op: UnaryOp::Not,
                                            expr: Box::new(bin(BinOp::Lt, var("i"), var("len"))),
                                        },
                                        then_body: vec![stmt(Stmt::Break(None))],
                                        else_body: vec![],
                                    }),
                                    stmt(Stmt::Scope {
                                        body: vec![
                                            stmt(temp("_v7", "i32", indexed_item())),
                                            stmt(Stmt::CompoundAssign {
                                                target: var("total"),
                                                op: BinOp::Add,
                                                value: var("_v7"),
                                            }),
                                        ],
                                    }),
                                    stmt(Stmt::CompoundAssign {
                                        target: var("i"),
                                        op: BinOp::Add,
                                        value: int(1),
                                    }),
                                ],
                            }),
                        ],
                    },
                    Stmt::Return(Some(var("total"))),
                ],
            ))],
        };
        let facts = facts::analyze(program.clone()).facts;

        assert!(!fixup(&mut program, &facts));
        let out = program.emit();

        assert!(out.contains("loop {"));
        assert!(out.contains("let _v7: i32 = items[(i as usize)];"));
        assert!(!out.contains("for item in items.iter()"));
    }
}
