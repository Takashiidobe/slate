use std::collections::{BTreeMap, BTreeSet};

use crate::fixups::facts::{FixupFacts, FunctionId};
use crate::fixups::idents::stmt_ident_count;
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, fact, function_path_location, stmts_snippet,
};
use crate::rust_ast::{BinOp, Expr, Ident, IndentStmt, Item, Program, Stmt};

pub(in crate::fixups) fn fixup(program: &mut Program, facts: &FixupFacts) -> bool {
    let mut logger = crate::fixups::trace::NoopLogger;
    ArrayElementPointerOrigin::new(&mut logger).fixup(program, facts)
}

pub(in crate::fixups) struct ArrayElementPointerOrigin<'a> {
    logger: &'a mut dyn TraceLogger,
}

impl<'a> ArrayElementPointerOrigin<'a> {
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
        let mut origins = origins_for_function(function, facts);
        if origins.is_empty() {
            continue;
        }
        collect_aliases(&f.body, &mut origins);
        if origins.is_empty() {
            continue;
        }
        let before = logger.is_enabled().then(|| f.body.clone());
        let function_changed = rewrite_body(&mut f.body, &origins);
        changed |= function_changed;
        if changed {
            let removable = origins.keys().cloned().collect();
            changed |= prune_dead_pointer_stmts(&mut f.body, &removable);
            if let Some(before) = before {
                if body_code(&before) != body_code(&f.body) {
                    logger.rewrite(RewriteEvent {
                        pass: TracePass::ArrayElementPointerOrigin,
                        kind: "rewrite_array_element_pointer_origins".into(),
                        location: function_path_location(facts, function, &[]),
                        before: vec![stmts_snippet("body", &before)],
                        after: vec![stmts_snippet("body", &f.body)],
                        facts: vec![fact("origins", origins.len().to_string())],
                    });
                }
            }
        }
    }
    changed
}

fn body_code(body: &[IndentStmt]) -> String {
    body.iter()
        .map(|stmt| stmt.stmt.render())
        .collect::<Vec<_>>()
        .join("")
}

#[derive(Clone)]
struct Origin {
    base: String,
    index: Expr,
}

fn origins_for_function(function: FunctionId, facts: &FixupFacts) -> BTreeMap<String, Origin> {
    facts
        .array_element_pointer_origins
        .iter()
        .filter(|fact| fact.site.function == function)
        .filter_map(|fact| {
            Some((
                facts.binding_name(fact.pointer)?.to_string(),
                Origin {
                    base: facts.binding_name(fact.base)?.to_string(),
                    index: fact.index.clone(),
                },
            ))
        })
        .collect()
}

fn collect_aliases(body: &[IndentStmt], origins: &mut BTreeMap<String, Origin>) {
    let mut changed = true;
    while changed {
        changed = false;
        collect_aliases_once(body, origins, &mut changed);
    }
}

fn collect_aliases_once(
    body: &[IndentStmt],
    origins: &mut BTreeMap<String, Origin>,
    changed: &mut bool,
) {
    for indent in body {
        match &indent.stmt {
            Stmt::Let {
                name,
                init: Some(init),
                ..
            } => {
                if let Some(source) = var_name(init)
                    && let Some(origin) = origins.get(source).cloned()
                    && !origins.contains_key(name)
                {
                    origins.insert(name.clone(), origin);
                    *changed = true;
                }
            }
            Stmt::Assign { target, value } => {
                if let (Some(name), Some(source)) = (var_name(target), var_name(value))
                    && let Some(origin) = origins.get(source).cloned()
                    && !origins.contains_key(name)
                {
                    origins.insert(name.to_string(), origin);
                    *changed = true;
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
                collect_aliases_once(then_body, origins, changed);
                collect_aliases_once(else_body, origins, changed);
            }
            Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
                collect_aliases_once(body, origins, changed);
            }
            Stmt::For { body, .. } => collect_aliases_once(body, origins, changed),
            Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
                collect_aliases_once(&body.stmts, origins, changed);
            }
            Stmt::Match { arms, .. } => {
                for arm in arms {
                    collect_aliases_once(&arm.body, origins, changed);
                }
            }
            Stmt::CompoundAssign { .. }
            | Stmt::Let { init: None, .. }
            | Stmt::Expr(_)
            | Stmt::Return(_)
            | Stmt::Break(_)
            | Stmt::Continue(_) => {}
        }
    }
}

fn var_name(expr: &Expr) -> Option<&str> {
    let Expr::Var(name) = expr else {
        return None;
    };
    Some(name.as_str())
}

fn rewrite_body(body: &mut [IndentStmt], origins: &BTreeMap<String, Origin>) -> bool {
    let mut changed = false;
    for indent in body {
        changed |= rewrite_stmt(&mut indent.stmt, origins);
        walk::nested_body_vecs_mut_with_path(&mut indent.stmt, &mut Vec::new(), &mut |body, _| {
            changed |= rewrite_body(body, origins);
        });
    }
    changed
}

fn rewrite_stmt(stmt: &mut Stmt, origins: &BTreeMap<String, Origin>) -> bool {
    let mut changed = false;
    walk::stmt_exprs_mut_with(stmt, &mut |expr| {
        if let Some(replacement) = safe_replacement(expr, origins) {
            *expr = replacement;
            changed = true;
            return false;
        }
        true
    });
    changed
}

fn safe_replacement(expr: &Expr, origins: &BTreeMap<String, Origin>) -> Option<Expr> {
    match expr {
        Expr::Unsafe(block) if block.stmts.is_empty() => {
            safe_replacement(block.tail.as_ref()?, origins)
        }
        Expr::Unary {
            op: crate::rust_ast::UnaryOp::Deref,
            expr,
        } => indexed_pointer(expr, origins),
        Expr::Cast { expr, ty } => pointer_diff(expr, origins).map(|expr| Expr::Cast {
            expr: Box::new(expr),
            ty: ty.clone(),
        }),
        _ => pointer_diff(expr, origins),
    }
}

fn indexed_pointer(expr: &Expr, origins: &BTreeMap<String, Origin>) -> Option<Expr> {
    let Expr::Var(name) = peel_unsafe(expr) else {
        return None;
    };
    let origin = origins.get(name.as_str())?;
    Some(Expr::Index {
        base: Box::new(Expr::Var(Ident::from(origin.base.as_str()))),
        index: Box::new(origin.index.clone()),
    })
}

fn pointer_diff(expr: &Expr, origins: &BTreeMap<String, Origin>) -> Option<Expr> {
    let Expr::MethodCall { recv, method, args } = peel_unsafe(expr) else {
        return None;
    };
    if method != "offset_from" || args.len() != 1 {
        return None;
    }
    let lhs = origin_for_pointer(recv, origins)?;
    let rhs = origin_for_pointer(&args[0], origins)?;
    if lhs.base != rhs.base {
        return None;
    }
    Some(Expr::Binary {
        op: BinOp::Sub,
        lhs: Box::new(lhs.index.clone()),
        rhs: Box::new(rhs.index.clone()),
    })
}

fn origin_for_pointer<'a>(
    expr: &Expr,
    origins: &'a BTreeMap<String, Origin>,
) -> Option<&'a Origin> {
    let Expr::Var(name) = peel_unsafe(expr) else {
        return None;
    };
    origins.get(name.as_str())
}

fn peel_unsafe(expr: &Expr) -> &Expr {
    match expr {
        Expr::Unsafe(block) if block.stmts.is_empty() => {
            block.tail.as_deref().map_or(expr, peel_unsafe)
        }
        _ => expr,
    }
}

fn prune_dead_pointer_stmts(body: &mut Vec<IndentStmt>, removable: &BTreeSet<String>) -> bool {
    let mut changed = false;
    for indent in body.iter_mut() {
        walk::nested_body_vecs_mut_with_path(&mut indent.stmt, &mut Vec::new(), &mut |body, _| {
            changed |= prune_dead_pointer_stmts(body, removable);
        });
    }
    while let Some(index) = dead_pointer_stmt_index(body, removable) {
        body.remove(index);
        changed = true;
    }
    changed
}

fn dead_pointer_stmt_index(body: &[IndentStmt], removable: &BTreeSet<String>) -> Option<usize> {
    body.iter().enumerate().position(|(index, indent)| {
        removable_stmt_name(&indent.stmt, removable).is_some_and(|name| {
            body[index + 1..]
                .iter()
                .all(|later| stmt_ident_count(&later.stmt, name) == 0)
        })
    })
}

fn removable_stmt_name<'a>(stmt: &'a Stmt, removable: &BTreeSet<String>) -> Option<&'a str> {
    match stmt {
        Stmt::Let { name, .. } if removable.contains(name) => Some(name.as_str()),
        Stmt::Assign { target, .. } => {
            let name = var_name(target)?;
            removable.contains(name).then_some(name)
        }
        _ => None,
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::facts;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Block, Item, Program, Type, UnaryOp};

    fn apply(mut program: Program) -> String {
        let analyzed = facts::analyze(program.clone());
        fixup(&mut program, &analyzed.facts);
        program.emit()
    }

    fn array(name: &str) -> Stmt {
        Stmt::Let {
            name: name.to_string(),
            mutable: true,
            ty: Some(Type::parse("[i32; 4]")),
            init: Some(Expr::ArrayRepeat {
                elem: Box::new(int(0)),
                len: 4,
            }),
        }
    }

    fn addr_of_index(array: &str, index: Expr) -> Expr {
        Expr::AddrOf {
            mutable: true,
            expr: Box::new(Expr::Index {
                base: Box::new(var(array)),
                index: Box::new(index),
            }),
        }
    }

    fn unsafe_tail(expr: Expr) -> Expr {
        Expr::Unsafe(Box::new(Block {
            stmts: Vec::new(),
            tail: Some(Box::new(expr)),
        }))
    }

    fn deref(expr: Expr) -> Expr {
        Expr::Unary {
            op: UnaryOp::Deref,
            expr: Box::new(expr),
        }
    }

    fn offset_from(lhs: Expr, rhs: Expr) -> Expr {
        Expr::MethodCall {
            recv: Box::new(lhs),
            method: "offset_from".into(),
            args: vec![rhs],
        }
    }

    fn program(stmts: Vec<Stmt>) -> Program {
        Program {
            items: vec![Item::Fn(func(vec![], None, stmts))],
        }
    }

    #[test]
    fn rewrites_proven_pointer_derefs_to_array_indexes() {
        let out = apply(program(vec![
            array("values"),
            temp("p", "*mut i32", addr_of_index("values", int(1))),
            temp("_v", "i32", unsafe_tail(deref(var("p")))),
        ]));

        assert!(out.contains("let _v: i32 = values[1];"));
        assert!(!out.contains("unsafe"));
        assert!(!out.contains("*p"));
    }

    #[test]
    fn rewrites_same_base_pointer_diff_through_aliases() {
        let out = apply(program(vec![
            array("values"),
            temp("p", "*mut i32", addr_of_index("values", int(1))),
            temp("q", "*mut i32", addr_of_index("values", int(3))),
            temp("_lhs", "*mut i32", var("q")),
            temp("_rhs", "*mut i32", var("p")),
            temp(
                "_distance",
                "i64",
                unsafe_tail(Expr::Cast {
                    expr: Box::new(offset_from(var("_lhs"), var("_rhs"))),
                    ty: Type::parse("i64"),
                }),
            ),
        ]));

        assert!(out.contains("let _distance: i64 = (3 - 1) as i64;"));
        assert!(!out.contains("offset_from"));
    }

    #[test]
    fn leaves_unrelated_base_pointer_diff_raw() {
        let out = apply(program(vec![
            array("left"),
            array("right"),
            temp("p", "*mut i32", addr_of_index("left", int(1))),
            temp("q", "*mut i32", addr_of_index("right", int(3))),
            temp(
                "_distance",
                "i64",
                unsafe_tail(Expr::Cast {
                    expr: Box::new(offset_from(var("q"), var("p"))),
                    ty: Type::parse("i64"),
                }),
            ),
        ]));

        assert!(out.contains("offset_from"));
        assert!(!out.contains("3 - 1"));
    }
}
