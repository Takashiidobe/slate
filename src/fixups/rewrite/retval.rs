//! Collapse a return-value slot store into the final return or main exit when
//! the slot is used only for that round trip.

use crate::fixups::facts::effects::is_movable_pure_expr;
use crate::fixups::facts::{
    AstPath, BindingId, ControlFlowSubject, FixupFacts, FunctionId, PathSegment, PlaceAccess,
    PlaceKind,
};
use crate::fixups::idents::{expr_ident, expr_ident_count, stmt_ident_count};
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, ast_path_fact, fact, function_path_location,
    path_fact, stmt_snippet, stmts_snippet,
};
use crate::rust_ast::{Expr, FnDef, IndentStmt, Path, Prim, Stmt, Type};

pub(in crate::fixups) fn fixup(f: &mut FnDef, function: FunctionId, facts: &FixupFacts) {
    let mut logger = crate::fixups::trace::NoopLogger;
    Retval::new(&mut logger).fixup(f, function, facts);
}

pub(in crate::fixups) struct Retval<'a> {
    logger: &'a mut dyn TraceLogger,
}

impl<'a> Retval<'a> {
    pub(in crate::fixups) fn new(logger: &'a mut dyn TraceLogger) -> Self {
        Self { logger }
    }

    pub(in crate::fixups) fn fixup(
        &mut self,
        f: &mut FnDef,
        function: FunctionId,
        facts: &FixupFacts,
    ) {
        collapse_return_slots_logged(&mut f.body, function, facts, &mut Vec::new(), self.logger);

        if f.name == "main" {
            collapse_main_exit_slots_logged(
                &mut f.body,
                function,
                facts,
                &mut Vec::new(),
                self.logger,
            );
        }

        remove_unused_retval_artifacts_logged(
            &mut f.body,
            function,
            facts,
            &Vec::new(),
            self.logger,
        );
    }
}

fn collapse_return_slots(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
) {
    let mut logger = crate::fixups::trace::NoopLogger;
    collapse_return_slots_logged(body, function, facts, path, &mut logger);
}

fn collapse_return_slots_logged(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
    logger: &mut dyn TraceLogger,
) {
    for (index, stmt) in body.iter_mut().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            walk::nested_body_vecs_mut_with_path(&mut stmt.stmt, path, &mut |nested, path| {
                collapse_return_slots_logged(nested, function, facts, path, logger);
            });
        });
    }

    let mut collapses: Vec<_> = facts
        .retval_collapses
        .iter()
        .filter(|fact| fact.function == function)
        .filter_map(|fact| Some((direct_stmt_index(path, &fact.return_path)?, fact)))
        .collect();
    collapses.sort_by_key(|(ret_index, _)| *ret_index);
    for (ret_index, fact) in collapses.into_iter().rev() {
        collapse_return_slot(body, path, ret_index, fact, function, facts, logger);
    }
}

fn collapse_return_slot(
    body: &mut Vec<IndentStmt>,
    body_path: &[PathSegment],
    ret_index: usize,
    collapse_fact: &crate::fixups::facts::RetvalCollapseFact,
    function: FunctionId,
    facts: &FixupFacts,
    logger: &mut dyn TraceLogger,
) {
    let Some(value_index) = direct_stmt_index(body_path, &collapse_fact.value_path) else {
        return;
    };
    if ret_index >= body.len() || value_index >= body.len() {
        return;
    }
    let value = match &body[value_index].stmt {
        Stmt::Let {
            init: Some(init), ..
        } => init.clone(),
        Stmt::Assign { value, .. } => value.clone(),
        _ => return,
    };
    let mut remove = Vec::new();
    for path in &collapse_fact.remove_paths {
        let Some(index) = direct_stmt_index(body_path, path) else {
            return;
        };
        if index >= body.len() || index == ret_index {
            return;
        }
        remove.push(index);
    }
    remove.sort_unstable();
    remove.dedup();

    let Stmt::Return(Some(_)) = &body[ret_index].stmt else {
        return;
    };
    let before = logger.is_enabled().then(|| {
        let removed = remove
            .iter()
            .filter_map(|index| body.get(*index).map(|indent| indent.stmt.clone()))
            .collect::<Vec<_>>();
        (
            body[value_index].stmt.clone(),
            body[ret_index].stmt.clone(),
            removed,
        )
    });
    body[ret_index].stmt = Stmt::Return(Some(value));
    if let Some((before_value, before_return, removed)) = before {
        let return_path = stmt_path(body_path, ret_index);
        let value_path = stmt_path(body_path, value_index);
        let mut facts_out = vec![
            ast_path_fact("return_path", &collapse_fact.return_path),
            ast_path_fact("value_path", &collapse_fact.value_path),
            path_fact("resolved_return_path", &return_path),
            path_fact("resolved_value_path", &value_path),
            fact("remove_count", remove.len().to_string()),
        ];
        facts_out.extend(
            collapse_fact
                .remove_paths
                .iter()
                .enumerate()
                .map(|(index, path)| ast_path_fact(format!("remove_path[{index}]"), path)),
        );
        let mut before_snippets = vec![
            stmt_snippet("value", &before_value),
            stmt_snippet("return", &before_return),
        ];
        before_snippets.extend(
            removed
                .iter()
                .enumerate()
                .map(|(index, stmt)| stmt_snippet(format!("removed[{index}]"), stmt)),
        );
        logger.rewrite(RewriteEvent {
            pass: TracePass::Retval,
            kind: "collapse_return_slot".into(),
            location: function_path_location(facts, function, &return_path),
            before: before_snippets,
            after: vec![stmt_snippet("return", &body[ret_index].stmt)],
            facts: facts_out,
        });
    }
    for index in remove.into_iter().rev() {
        body.remove(index);
    }
}

fn remove_unused_retval_decl(body: &mut Vec<IndentStmt>) {
    let Some(index) = body.iter().position(|indent| {
        matches!(
            &indent.stmt,
            Stmt::Let {
                name,
                mutable: true,
                ..
            } if name == "__retval"
        )
    }) else {
        return;
    };
    let uses = body
        .iter()
        .enumerate()
        .filter(|(i, _)| *i != index)
        .map(|(_, indent)| stmt_ident_count(&indent.stmt, "__retval"))
        .sum::<usize>();
    if uses == 0 {
        body.remove(index);
    }
}

fn remove_unused_retval_artifacts(body: &mut Vec<IndentStmt>) {
    if retval_read_count(body) != 0 {
        return;
    }
    remove_unused_retval_writes(body);
    remove_unused_retval_decl(body);
}

fn remove_unused_retval_artifacts_logged(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
    path: &[PathSegment],
    logger: &mut dyn TraceLogger,
) {
    let before = logger.is_enabled().then(|| body.clone());
    remove_unused_retval_artifacts(body);
    let Some(before) = before else {
        return;
    };
    if stmts_code(&before) == stmts_code(body) {
        return;
    }
    logger.rewrite(RewriteEvent {
        pass: TracePass::Retval,
        kind: "remove_unused_retval_artifacts".into(),
        location: function_path_location(facts, function, path),
        before: vec![stmts_snippet("body", &before)],
        after: vec![stmts_snippet("body", body)],
        facts: vec![path_fact("body_path", path)],
    });
}

fn stmts_code(body: &[IndentStmt]) -> String {
    body.iter()
        .map(|stmt| stmt.stmt.render())
        .collect::<Vec<_>>()
        .join("")
}

fn retval_read_count(body: &[IndentStmt]) -> usize {
    body.iter()
        .map(|indent| stmt_retval_read_count(&indent.stmt))
        .sum()
}

fn stmt_retval_read_count(stmt: &Stmt) -> usize {
    match stmt {
        Stmt::Let { init, .. } => init
            .as_ref()
            .map_or(0, |expr| expr_ident_count(expr, "__retval")),
        Stmt::Assign { target, value } if expr_ident(target) == Some("__retval") => {
            expr_ident_count(value, "__retval")
        }
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            expr_ident_count(cond, "__retval")
                + retval_read_count(then_body)
                + expr_ident_count(then_value, "__retval")
                + retval_read_count(else_body)
                + expr_ident_count(else_value, "__retval")
        }
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            expr_ident_count(cond, "__retval")
                + retval_read_count(then_body)
                + retval_read_count(else_body)
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } => retval_read_count(body),
        Stmt::For { iter, body, .. } => {
            expr_ident_count(iter, "__retval") + retval_read_count(body)
        }
        Stmt::LabeledBlock { body, .. } => retval_read_count(body),
        Stmt::Match { expr, arms } => {
            expr_ident_count(expr, "__retval")
                + arms
                    .iter()
                    .map(|arm| retval_read_count(&arm.body))
                    .sum::<usize>()
        }
        Stmt::Unsafe { body } | Stmt::Block(body) => {
            retval_read_count(&body.stmts)
                + body
                    .tail
                    .as_ref()
                    .map_or(0, |tail| expr_ident_count(tail, "__retval"))
        }
        Stmt::While { cond, body } => {
            expr_ident_count(cond, "__retval")
                + retval_read_count(&body.stmts)
                + body
                    .tail
                    .as_ref()
                    .map_or(0, |tail| expr_ident_count(tail, "__retval"))
        }
        other => stmt_ident_count(other, "__retval"),
    }
}

fn remove_unused_retval_writes(body: &mut Vec<IndentStmt>) {
    for indent in body.iter_mut() {
        let mut path = Vec::new();
        walk::nested_body_vecs_mut_with_path(&mut indent.stmt, &mut path, &mut |body, _| {
            remove_unused_retval_writes(body);
        });
    }
    body.retain(|indent| !is_removable_retval_write(&indent.stmt));
}

fn is_removable_retval_write(stmt: &Stmt) -> bool {
    match stmt {
        Stmt::Assign { target, value } if expr_ident(target) == Some("__retval") => {
            is_movable_pure_expr(value)
        }
        _ => false,
    }
}

fn collapse_main_exit_slots(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
) {
    let mut logger = crate::fixups::trace::NoopLogger;
    collapse_main_exit_slots_logged(body, function, facts, path, &mut logger);
}

fn collapse_main_exit_slots_logged(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
    logger: &mut dyn TraceLogger,
) {
    for (index, stmt) in body.iter_mut().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            walk::nested_body_vecs_mut_with_path(&mut stmt.stmt, path, &mut |nested, path| {
                collapse_main_exit_slots_logged(nested, function, facts, path, logger);
            });
        });
    }
    collapse_main_exit_slot(body, function, facts, path, logger);
}

fn collapse_main_exit_slot(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
    body_path: &[PathSegment],
    logger: &mut dyn TraceLogger,
) {
    let Some((exit_index, temp_name)) =
        body.iter()
            .enumerate()
            .find_map(|(index, stmt)| match &stmt.stmt {
                Stmt::Expr(expr)
                    if reachable_stmt(function, facts, &stmt_path(body_path, index)) =>
                {
                    main_exit_arg_temp(expr).map(|name| (index, name.to_string()))
                }
                _ => None,
            })
    else {
        return;
    };
    if exit_index < 2 {
        return;
    }

    let temp_index = exit_index - 1;
    let retval_name = match &body[temp_index].stmt {
        Stmt::Let {
            name,
            mutable: false,
            init: Some(init),
            ..
        } if name == &temp_name => expr_ident(init).map(str::to_string),
        _ => None,
    };
    let Some(retval_name) = retval_name else {
        return;
    };
    let temp_path = AstPath(stmt_path(body_path, temp_index));
    let Some(temp_binding) = facts.binding_by_local_path(function, &temp_name, &temp_path) else {
        return;
    };
    let Some(retval_binding) = facts
        .bindings
        .iter()
        .find(|binding| binding.function == function && binding.name == retval_name)
        .map(|binding| binding.id)
    else {
        return;
    };

    let store_index = temp_index - 1;
    let value = match &body[store_index].stmt {
        Stmt::Assign { target, value }
            if store_writes_binding(
                function,
                facts,
                &stmt_path(body_path, store_index),
                retval_binding,
                target,
            ) =>
        {
            value.clone()
        }
        _ => return,
    };

    let Some(retval_def_use) = facts.def_use(retval_binding) else {
        return;
    };
    let Some(temp_def_use) = facts.def_use(temp_binding) else {
        return;
    };
    let store_path = AstPath(stmt_path(body_path, store_index));
    if retval_def_use.reads != [AstPath(stmt_path(body_path, temp_index))]
        || !retval_def_use.writes.contains(&store_path)
        || temp_def_use.reads != [AstPath(stmt_path(body_path, exit_index))]
        || !temp_def_use.writes.is_empty()
    {
        return;
    }

    let mut remove = vec![temp_index, store_index];
    for write in &retval_def_use.writes {
        if let Some(index) = direct_stmt_index(body_path, write) {
            if index == store_index {
                continue;
            }
            if index > store_index
                || !pure_retval_write(function, facts, index, retval_binding, body)
            {
                return;
            }
            remove.push(index);
        }
    }

    let before = logger.is_enabled().then(|| {
        let removed = remove
            .iter()
            .filter_map(|index| body.get(*index).map(|indent| indent.stmt.clone()))
            .collect::<Vec<_>>();
        (
            body[store_index].stmt.clone(),
            body[exit_index].stmt.clone(),
            removed,
        )
    });
    let Stmt::Expr(expr) = &mut body[exit_index].stmt else {
        unreachable!();
    };
    replace_main_exit_arg(expr, value);
    remove.sort_unstable();
    remove.dedup();
    if let Some((before_store, before_exit, removed)) = before {
        let exit_path = stmt_path(body_path, exit_index);
        let store_path = stmt_path(body_path, store_index);
        let temp_path = stmt_path(body_path, temp_index);
        let mut event_facts = vec![
            path_fact("exit_path", &exit_path),
            path_fact("store_path", &store_path),
            path_fact("temp_path", &temp_path),
            fact("temp", temp_name),
            fact("retval", retval_name),
            fact("remove_count", remove.len().to_string()),
        ];
        event_facts.extend(remove.iter().enumerate().map(|(index, remove_index)| {
            path_fact(
                format!("remove_path[{index}]"),
                &stmt_path(body_path, *remove_index),
            )
        }));
        let mut before_snippets = vec![
            stmt_snippet("store", &before_store),
            stmt_snippet("exit", &before_exit),
        ];
        before_snippets.extend(
            removed
                .iter()
                .enumerate()
                .map(|(index, stmt)| stmt_snippet(format!("removed[{index}]"), stmt)),
        );
        logger.rewrite(RewriteEvent {
            pass: TracePass::Retval,
            kind: "collapse_main_exit_slot".into(),
            location: function_path_location(facts, function, &exit_path),
            before: before_snippets,
            after: vec![stmt_snippet("exit", &body[exit_index].stmt)],
            facts: event_facts,
        });
    }
    for index in remove.into_iter().rev() {
        body.remove(index);
    }
}

fn main_exit_arg_temp(expr: &Expr) -> Option<&str> {
    let Expr::Call { func, args, .. } = expr else {
        return None;
    };
    if !is_std_process_exit(func) || args.len() != 1 {
        return None;
    }
    match &args[0] {
        Expr::Cast {
            expr,
            ty: Type::Prim(Prim::I32),
        } => expr_ident(expr),
        arg => expr_ident(arg),
    }
}

fn replace_main_exit_arg(expr: &mut Expr, replacement: Expr) {
    let Expr::Call { args, .. } = expr else {
        return;
    };
    args[0] = replacement;
}

fn is_std_process_exit(expr: &Expr) -> bool {
    let Expr::Path(Path { segments }) = expr else {
        return false;
    };
    let expected = ["std", "process", "exit"];
    segments.len() == expected.len()
        && segments
            .iter()
            .zip(expected)
            .all(|(segment, expected)| segment.as_str() == expected)
}

fn store_writes_binding(
    function: FunctionId,
    facts: &FixupFacts,
    path: &[PathSegment],
    binding: BindingId,
    target: &Expr,
) -> bool {
    let Some(name) = facts.binding_name(binding) else {
        return false;
    };
    if expr_ident(target) != Some(name) {
        return false;
    }
    facts
        .place(function, &AstPath(path.to_vec()))
        .is_some_and(|fact| {
            fact.access == PlaceAccess::Write
                && fact.ordinary_slot
                && matches!(&fact.kind, PlaceKind::Local { name: place } if place == name)
        })
}

fn pure_retval_write(
    function: FunctionId,
    facts: &FixupFacts,
    index: usize,
    binding: BindingId,
    body: &[IndentStmt],
) -> bool {
    let Some(stmt) = body.get(index).map(|indent| &indent.stmt) else {
        return false;
    };
    let Stmt::Assign { target, value } = stmt else {
        return false;
    };
    store_writes_binding(function, facts, &stmt_path(&[], index), binding, target)
        && is_movable_pure_expr(value)
}

fn reachable_stmt(function: FunctionId, facts: &FixupFacts, path: &[PathSegment]) -> bool {
    facts
        .control_flow(function, ControlFlowSubject::Stmt, &AstPath(path.to_vec()))
        .is_some_and(|fact| fact.reachable)
}

fn stmt_path(body_path: &[PathSegment], index: usize) -> Vec<PathSegment> {
    let mut path = body_path.to_vec();
    path.push(PathSegment::Stmt(index));
    path
}

fn direct_stmt_index(body_path: &[PathSegment], path: &AstPath) -> Option<usize> {
    let rest = path.0.strip_prefix(body_path)?;
    match rest {
        [PathSegment::Stmt(index)] => Some(*index),
        _ => None,
    }
}
