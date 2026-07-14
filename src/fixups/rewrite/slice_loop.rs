use crate::fixups::facts::{
    AstPath, CountedLoopBound, CountedLoopIndexUse, CountedLoopStart, CountedLoopStep,
    CountedSliceLoopFact, FixupFacts, FunctionId, PathSegment, SliceLoopAccess,
};
use crate::fixups::support::walk;
use crate::rust_ast::{Expr, Ident, IndentStmt, Item, Program, Stmt, UnaryOp};

const ITEM_NAME: &str = "__slate_item";

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
        fact.function == function
            && fact.loop_path == *loop_path
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
    rewrite_index_uses(&mut body, &slice_name, index_name.as_str());
    Some(IndentStmt {
        depth: pair[1].depth,
        stmt: Stmt::For {
            pat: ITEM_NAME.into(),
            iter: Expr::MethodCall {
                recv: Box::new(Expr::Var(Ident::new(slice_name))),
                method: method.into(),
                args: Vec::new(),
            },
            body,
        },
    })
}

fn rewrite_index_uses(body: &mut [IndentStmt], slice_name: &str, index_name: &str) {
    for indent in body {
        walk::stmt_exprs_mut_with(&mut indent.stmt, &mut |expr| {
            if is_slice_index(expr, slice_name, index_name) {
                *expr = item_deref();
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

fn item_deref() -> Expr {
    Expr::Unary {
        op: UnaryOp::Deref,
        expr: Box::new(Expr::Var(Ident::new(ITEM_NAME))),
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
    fn rewrites_canonical_read_only_slice_loop_to_for_iter() {
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

        assert!(fixup(&mut program, &facts));
        let out = program.emit();

        assert!(out.contains("for __slate_item in items.iter()"));
        assert!(out.contains("let _v7: i32 = *__slate_item;"));
        assert!(!out.contains("loop {"));
        assert!(!out.contains("items[(i as usize)]"));
    }
}
