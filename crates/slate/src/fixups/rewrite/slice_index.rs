use std::collections::{BTreeMap, BTreeSet};

use crate::fixups::facts::{AstPath, FixupFacts, FunctionId, PathSegment, PointerOffsetUnit};
use crate::fixups::idents::stmt_ident_count;
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLocation, TraceLogger, TraceSnippet, fact,
};
use crate::rust_ast::{Expr, Ident, IndentStmt, Item, Prim, Program, Stmt, Type, UnaryOp};

pub(in crate::fixups) fn fixup(program: &mut Program, facts: &FixupFacts) -> bool {
    let mut logger = crate::fixups::trace::NoopLogger;
    SliceIndex::new(&mut logger).fixup(program, facts)
}

pub(in crate::fixups) struct SliceIndex<'a> {
    logger: &'a mut dyn TraceLogger,
}

impl<'a> SliceIndex<'a> {
    pub(in crate::fixups) fn new(logger: &'a mut dyn TraceLogger) -> Self {
        Self { logger }
    }

    pub(in crate::fixups) fn fixup(&mut self, program: &mut Program, facts: &FixupFacts) -> bool {
        let before = self.logger.is_enabled().then(|| program.emit());
        let changed = fixup_impl(program, facts);
        if changed && let Some(before) = before {
            self.logger.rewrite(RewriteEvent {
                pass: TracePass::SliceIndex,
                kind: "rewrite_slice_pointer_indexes".into(),
                location: TraceLocation::default(),
                before: vec![TraceSnippet::new("program", before.trim_end())],
                after: vec![TraceSnippet::new("program", program.emit().trim_end())],
                facts: vec![fact(
                    "planned_indexes",
                    facts.slice_pointer_indexes.len().to_string(),
                )],
            });
        }
        changed
    }
}

fn fixup_impl(program: &mut Program, facts: &FixupFacts) -> bool {
    let plans = plans_by_function(facts);
    if plans.is_empty() {
        return false;
    }

    let mut changed = false;
    for (item_index, item) in program.items.iter_mut().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        let Some(function_plans) = plans.get(&function) else {
            continue;
        };
        changed |= rewrite_body(&mut f.body, function_plans, &mut Vec::new());
        changed |= prune_dead_fact_temps(&mut f.body, function_plans);
    }
    changed
}

#[derive(Clone)]
struct Plan {
    path: AstPath,
    pointer_name: String,
    offset_name: String,
    slice_name: String,
    index_name: String,
    mutable: bool,
}

fn plans_by_function(facts: &FixupFacts) -> BTreeMap<FunctionId, Vec<Plan>> {
    let mut by_function = BTreeMap::new();
    for fact in &facts.slice_pointer_indexes {
        if fact.unit != PointerOffsetUnit::Elements {
            continue;
        }
        let Some(view) = facts
            .slice_pointer_views
            .iter()
            .find(|view| view.site.function == fact.site.function && view.pointer == fact.pointer)
        else {
            continue;
        };
        let (Some(pointer_name), Some(offset_name), Some(slice_name), Some(index_name)) = (
            facts.binding_name(fact.pointer),
            facts.binding_name(fact.offset_index),
            facts.binding_name(fact.slice),
            facts.binding_name(fact.ranged_index),
        ) else {
            continue;
        };
        by_function
            .entry(fact.site.function)
            .or_insert_with(Vec::new)
            .push(Plan {
                path: fact.site.path.clone(),
                pointer_name: pointer_name.to_string(),
                offset_name: offset_name.to_string(),
                slice_name: slice_name.to_string(),
                index_name: index_name.to_string(),
                mutable: view.mutable,
            });
    }
    by_function
}

fn rewrite_body(body: &mut [IndentStmt], plans: &[Plan], path: &mut Vec<PathSegment>) -> bool {
    let mut changed = false;
    for (index, indent) in body.iter_mut().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            changed |= rewrite_stmt(&mut indent.stmt, plans, path);
            walk::nested_bodies_mut_with_path(&mut indent.stmt, path, &mut |body, path| {
                changed |= rewrite_body(body, plans, path);
            });
            changed |= unwrap_safe_singleton_unsafe(&mut indent.stmt);
        });
    }
    changed
}

fn rewrite_stmt(stmt: &mut Stmt, plans: &[Plan], path: &[PathSegment]) -> bool {
    let mut changed = false;
    let stmt_plans = plans_for_path(plans, path);
    if stmt_plans.is_empty() {
        return false;
    }
    match stmt {
        Stmt::Let {
            init: Some(init), ..
        } => changed |= rewrite_expr(init, &stmt_plans, false),
        Stmt::Assign { target, value } => {
            changed |= rewrite_expr(target, &stmt_plans, true);
            changed |= rewrite_expr(value, &stmt_plans, false);
        }
        Stmt::CompoundAssign { target, value, .. } => {
            changed |= rewrite_expr(target, &stmt_plans, true);
            changed |= rewrite_expr(value, &stmt_plans, false);
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => {
            changed |= rewrite_expr(expr, &stmt_plans, false);
        }
        Stmt::If { cond, .. } | Stmt::While { cond, .. } => {
            changed |= rewrite_expr(cond, &stmt_plans, false);
        }
        Stmt::For { iter, .. } => {
            changed |= rewrite_expr(iter, &stmt_plans, false);
        }
        Stmt::LetIf {
            cond,
            then_value,
            else_value,
            ..
        } => {
            changed |= rewrite_expr(cond, &stmt_plans, false);
            changed |= rewrite_expr(then_value, &stmt_plans, false);
            changed |= rewrite_expr(else_value, &stmt_plans, false);
        }
        Stmt::Match { expr, .. } => changed |= rewrite_expr(expr, &stmt_plans, false),
        Stmt::Let { init: None, .. }
        | Stmt::Loop { .. }
        | Stmt::Scope { .. }
        | Stmt::LabeledBlock { .. }
        | Stmt::Unsafe { .. }
        | Stmt::Block(_)
        | Stmt::Return(None)
        | Stmt::Break(_)
        | Stmt::Continue(_) => {}
    }
    changed
}

fn plans_for_path<'a>(plans: &'a [Plan], path: &[PathSegment]) -> Vec<&'a Plan> {
    plans
        .iter()
        .filter(|plan| plan.path == AstPath(path.to_vec()))
        .collect()
}

fn rewrite_expr(expr: &mut Expr, plans: &[&Plan], write_target: bool) -> bool {
    let mut changed = false;
    walk::exprs_mut_with(expr, &mut |expr| {
        if let Some(replacement) = indexed_expr(expr, plans, write_target) {
            *expr = replacement;
            changed = true;
            return false;
        }
        true
    });
    changed
}

fn indexed_expr(expr: &Expr, plans: &[&Plan], write_target: bool) -> Option<Expr> {
    let offset = deref_offset(expr)?;
    let pointer_name = receiver_var(offset.recv)?;
    let offset_name = offset_arg_var(offset.arg)?;
    let plan = plans.iter().find(|plan| {
        plan.pointer_name == pointer_name
            && plan.offset_name == offset_name
            && (!write_target || plan.mutable)
    })?;
    Some(Expr::Index {
        base: Box::new(Expr::Var(Ident::from(plan.slice_name.as_str()))),
        index: Box::new(Expr::Cast {
            expr: Box::new(Expr::Var(Ident::from(plan.index_name.as_str()))),
            ty: Type::Prim(Prim::Usize),
        }),
    })
}

struct OffsetParts<'a> {
    recv: &'a Expr,
    arg: &'a Expr,
}

fn deref_offset(expr: &Expr) -> Option<OffsetParts<'_>> {
    match expr {
        Expr::Unsafe(block) => deref_offset(block.tail.as_deref()?),
        Expr::Unary {
            op: UnaryOp::Deref,
            expr,
        } => offset_parts(peel_unsafe(expr)),
        _ => None,
    }
}

fn peel_unsafe(expr: &Expr) -> &Expr {
    match expr {
        Expr::Unsafe(block) => block.tail.as_deref().map_or(expr, peel_unsafe),
        _ => expr,
    }
}

fn offset_parts(expr: &Expr) -> Option<OffsetParts<'_>> {
    let Expr::MethodCall { recv, method, args } = expr else {
        return None;
    };
    if method != "offset" || args.len() != 1 {
        return None;
    }
    Some(OffsetParts {
        recv,
        arg: &args[0],
    })
}

fn receiver_var(expr: &Expr) -> Option<&str> {
    let Expr::Var(name) = expr else {
        return None;
    };
    Some(name.as_str())
}

fn offset_arg_var(expr: &Expr) -> Option<&str> {
    match expr {
        Expr::Cast { expr, .. } => offset_arg_var(expr),
        Expr::Var(name) => Some(name.as_str()),
        _ => None,
    }
}

fn unwrap_safe_singleton_unsafe(stmt: &mut Stmt) -> bool {
    let Stmt::Unsafe { body } = stmt else {
        return false;
    };
    if body.tail.is_some() || body.stmts.len() != 1 {
        return false;
    }
    let inner = body.stmts[0].stmt.clone();
    if !is_safe_slice_index_stmt(&inner) {
        return false;
    }
    *stmt = inner;
    true
}

fn is_safe_slice_index_stmt(stmt: &Stmt) -> bool {
    match stmt {
        Stmt::Assign { target, value } => is_slice_index(target) && !contains_unsafe(value),
        Stmt::CompoundAssign { target, value, .. } => {
            is_slice_index(target) && !contains_unsafe(value)
        }
        _ => false,
    }
}

fn is_slice_index(expr: &Expr) -> bool {
    matches!(expr, Expr::Index { .. })
}

fn contains_unsafe(expr: &Expr) -> bool {
    let mut found = false;
    crate::fixups::support::walk::expr_any(expr, &mut |expr| {
        if matches!(
            expr,
            Expr::Unsafe(_)
                | Expr::Unary {
                    op: UnaryOp::Deref,
                    ..
                }
        ) {
            found = true;
        }
        found
    });
    found
}

fn prune_dead_fact_temps(body: &mut Vec<IndentStmt>, plans: &[Plan]) -> bool {
    let removable = removable_temp_names(plans);
    prune_dead_fact_temps_in_body(body, &removable)
}

fn prune_dead_fact_temps_in_body(body: &mut Vec<IndentStmt>, removable: &BTreeSet<String>) -> bool {
    let mut changed = false;
    for indent in body.iter_mut() {
        walk::nested_body_vecs_mut_with_path(&mut indent.stmt, &mut Vec::new(), &mut |body, _| {
            changed |= prune_dead_fact_temps_in_body(body, removable);
        });
    }
    let mut index = 0;
    while index < body.len() {
        let remove = match &body[index].stmt {
            Stmt::Let {
                name,
                init: Some(_),
                ..
            } if removable.contains(name) => body[index + 1..]
                .iter()
                .all(|indent| stmt_ident_count(&indent.stmt, name) == 0),
            _ => false,
        };
        if remove {
            body.remove(index);
            changed = true;
        } else {
            index += 1;
        }
    }
    changed
}

fn removable_temp_names(plans: &[Plan]) -> BTreeSet<String> {
    plans
        .iter()
        .flat_map(|plan| [&plan.pointer_name, &plan.offset_name])
        .filter(|name| is_temp_name(name))
        .cloned()
        .collect()
}

fn is_temp_name(name: &str) -> bool {
    name.strip_prefix("_v")
        .is_some_and(|rest| !rest.is_empty() && rest.bytes().all(|b| b.is_ascii_digit()))
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::facts;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{BinOp, Block, FnParam, Item, Program};

    fn apply(mut program: Program) -> String {
        let analyzed = facts::analyze(program.clone());
        fixup(&mut program, &analyzed.facts);
        program.emit()
    }

    fn slice_param() -> FnParam {
        FnParam {
            name: "items".into(),
            mutable: true,
            ty: Type::parse("&mut [i32]"),
        }
    }

    fn cast(expr: Expr, ty: &str) -> Expr {
        Expr::Cast {
            expr: Box::new(expr),
            ty: Type::parse(ty),
        }
    }

    fn method(recv: Expr, name: &str, args: Vec<Expr>) -> Expr {
        Expr::MethodCall {
            recv: Box::new(recv),
            method: name.into(),
            args,
        }
    }

    fn unsafe_tail(expr: Expr) -> Expr {
        Expr::Unsafe(Box::new(Block {
            stmts: Vec::new(),
            tail: Some(Box::new(expr)),
        }))
    }

    fn offset(method_name: &str) -> Expr {
        method(
            var("_v6"),
            method_name,
            vec![cast(cast(var("_v5"), "i64"), "isize")],
        )
    }

    fn deref(expr: Expr) -> Expr {
        Expr::Unary {
            op: UnaryOp::Deref,
            expr: Box::new(expr),
        }
    }

    fn loop_body(offset_method: &str) -> Stmt {
        Stmt::Scope {
            body: vec![
                IndentStmt {
                    depth: 2,
                    stmt: let_mut("i", "i32", int(0)),
                },
                IndentStmt {
                    depth: 2,
                    stmt: Stmt::Loop {
                        label: None,
                        body: vec![
                            IndentStmt {
                                depth: 3,
                                stmt: Stmt::If {
                                    cond: Expr::Unary {
                                        op: UnaryOp::Not,
                                        expr: Box::new(bin(BinOp::Lt, var("i"), var("len"))),
                                    },
                                    then_body: vec![IndentStmt {
                                        depth: 4,
                                        stmt: Stmt::Break(None),
                                    }],
                                    else_body: Vec::new(),
                                },
                            },
                            IndentStmt {
                                depth: 3,
                                stmt: temp("_v5", "i32", var("i")),
                            },
                            IndentStmt {
                                depth: 3,
                                stmt: temp(
                                    "_v6",
                                    "*mut i32",
                                    method(var("items"), "as_mut_ptr", Vec::new()),
                                ),
                            },
                            IndentStmt {
                                depth: 3,
                                stmt: temp(
                                    "_value",
                                    "i32",
                                    unsafe_tail(deref(offset(offset_method))),
                                ),
                            },
                            IndentStmt {
                                depth: 3,
                                stmt: Stmt::Unsafe {
                                    body: Block {
                                        stmts: vec![IndentStmt {
                                            depth: 4,
                                            stmt: Stmt::Assign {
                                                target: deref(unsafe_tail(offset(offset_method))),
                                                value: var("_value"),
                                            },
                                        }],
                                        tail: None,
                                    },
                                },
                            },
                            IndentStmt {
                                depth: 3,
                                stmt: Stmt::CompoundAssign {
                                    target: var("i"),
                                    op: BinOp::Add,
                                    value: int(1),
                                },
                            },
                        ],
                    },
                },
            ],
        }
    }

    fn program_with_offset(offset_method: &str) -> Program {
        Program {
            items: vec![Item::Fn(func(
                vec![slice_param()],
                None,
                vec![
                    temp(
                        "len",
                        "i32",
                        cast(method(var("items"), "len", Vec::new()), "i32"),
                    ),
                    loop_body(offset_method),
                ],
            ))],
        }
    }

    #[test]
    fn rewrites_proven_element_offsets_to_slice_indexes() {
        let out = apply(program_with_offset("offset"));

        assert!(out.contains("let _value: i32 = items[(i as usize)];"));
        assert!(out.contains("items[(i as usize)] = _value;"));
        assert!(!out.contains(".offset("));
        assert!(!out.contains("as_mut_ptr()"));
    }

    #[test]
    fn leaves_byte_offsets_raw() {
        let out = apply(program_with_offset("byte_offset"));

        assert!(out.contains(".byte_offset("));
        assert!(out.contains("as_mut_ptr()"));
        assert!(!out.contains("items[(i as usize)]"));
    }
}
