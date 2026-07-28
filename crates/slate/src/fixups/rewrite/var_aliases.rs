use crate::fixups::Fixup;
use crate::fixups::idents::stmt_ident_count;
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, fact, path_fact, path_location, stmt_snippet,
};
use crate::rust_ast::{Block, Expr, IndentStmt, Stmt};

pub(in crate::fixups) struct VarAliases<'a> {
    logger: &'a mut dyn TraceLogger,
}

impl Fixup for VarAliases<'_> {
    fn fixup(&mut self, body: &mut Vec<IndentStmt>) -> bool {
        self.fixup_once(body, &mut Vec::new())
    }
}

impl<'a> VarAliases<'a> {
    pub(in crate::fixups) fn new(logger: &'a mut dyn TraceLogger) -> Self {
        Self { logger }
    }

    fn fixup_once(&mut self, body: &mut Vec<IndentStmt>, path: &mut Vec<usize>) -> bool {
        if self.inline_nested_alias(body, path) {
            return true;
        }
        for def_index in 0..body.len() {
            let Some((alias, source)) = alias_def(&body[def_index].stmt) else {
                continue;
            };
            let Some(use_index) = single_later_use_stmt(body, def_index, &alias) else {
                continue;
            };
            if source_changes_between(body, def_index, use_index, &source)
                || stmt_declares_name(&body[use_index].stmt, &source)
            {
                continue;
            }
            let trace_before = self
                .logger
                .is_enabled()
                .then(|| (body[def_index].stmt.clone(), body[use_index].stmt.clone()));
            if body[use_index]
                .stmt
                .substitute_var(&alias, &Expr::Var(source.clone().into()))
            {
                if let Some((before_def, before_use)) = trace_before {
                    let after_use = body[use_index].stmt.clone();
                    self.log_alias_event(
                        &alias,
                        &source,
                        path,
                        def_index,
                        use_index,
                        &before_def,
                        &before_use,
                        &after_use,
                    );
                }
                body.remove(def_index);
                return true;
            }
        }
        false
    }

    fn inline_nested_alias(&mut self, body: &mut [IndentStmt], path: &mut Vec<usize>) -> bool {
        for (index, stmt) in body.iter_mut().enumerate() {
            let mut changed = false;
            path.push(index);
            walk::nested_body_vecs_mut_with_path(
                &mut stmt.stmt,
                &mut Vec::new(),
                &mut |body, _| {
                    if !changed && self.fixup_once(body, path) {
                        changed = true;
                    }
                },
            );
            path.pop();
            if changed {
                return true;
            }
        }
        false
    }

    #[allow(clippy::too_many_arguments)]
    fn log_alias_event(
        &mut self,
        alias: &str,
        source: &str,
        body_path: &[usize],
        def_index: usize,
        use_index: usize,
        before_def: &Stmt,
        before_use: &Stmt,
        after_use: &Stmt,
    ) {
        if !self.logger.is_enabled() {
            return;
        }
        let def_path = var_alias_path(body_path, def_index);
        let use_path = var_alias_path(body_path, use_index);
        self.logger.rewrite(RewriteEvent {
            pass: TracePass::VarAliases,
            kind: "inline_var_alias".into(),
            location: path_location(&use_path),
            before: vec![
                stmt_snippet("alias", before_def),
                stmt_snippet("consumer", before_use),
            ],
            after: vec![stmt_snippet("consumer", after_use)],
            facts: vec![
                fact("alias", alias),
                fact("source", source),
                path_fact("alias_path", &def_path),
                path_fact("consumer_path", &use_path),
                fact("source_changes_between", "false"),
                fact("consumer_declares_source", "false"),
            ],
        });
    }
}

fn var_alias_path(body_path: &[usize], index: usize) -> Vec<crate::fixups::facts::PathSegment> {
    let mut out: Vec<_> = body_path
        .iter()
        .copied()
        .map(crate::fixups::facts::PathSegment::Stmt)
        .collect();
    out.push(crate::fixups::facts::PathSegment::Stmt(index));
    out
}

fn alias_def(stmt: &Stmt) -> Option<(String, String)> {
    let Stmt::Let {
        name,
        mutable: false,
        init: Some(Expr::Var(source)),
        ..
    } = stmt
    else {
        return None;
    };
    if !is_temp_name(name) || is_temp_name(source.as_str()) {
        return None;
    }
    Some((name.clone(), source.as_str().to_string()))
}

fn single_later_use_stmt(body: &[IndentStmt], def_index: usize, alias: &str) -> Option<usize> {
    let mut use_index = None;
    for (index, indent) in body.iter().enumerate().skip(def_index + 1) {
        let stmt_count = stmt_ident_count(&indent.stmt, alias);
        if stmt_count == 0 {
            continue;
        }
        if use_index.replace(index).is_some() {
            return None;
        }
    }
    use_index
}

fn source_changes_between(
    body: &[IndentStmt],
    def_index: usize,
    use_index: usize,
    source: &str,
) -> bool {
    body[def_index + 1..use_index].iter().any(|indent| {
        stmt_declares_name(&indent.stmt, source) || stmt_changes_name(&indent.stmt, source)
    })
}

fn stmt_declares_name(stmt: &Stmt, name: &str) -> bool {
    match stmt {
        Stmt::Let { name: binding, .. } | Stmt::LetIf { name: binding, .. } => binding == name,
        Stmt::For { pat, .. } => pat == name,
        Stmt::Match { arms, .. } => arms
            .iter()
            .any(|arm| pattern_declares_name(&arm.pattern, name)),
        _ => false,
    }
}

fn stmt_changes_name(stmt: &Stmt, name: &str) -> bool {
    match stmt {
        Stmt::Assign { target, .. } | Stmt::CompoundAssign { target, .. } => {
            expr_writes_name(target, name)
        }
        Stmt::Let { init, .. } => init
            .as_ref()
            .is_some_and(|expr| expr_mutably_borrows_name(expr, name)),
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            expr_mutably_borrows_name(cond, name)
                || body_changes_name(then_body, name)
                || expr_mutably_borrows_name(then_value, name)
                || body_changes_name(else_body, name)
                || expr_mutably_borrows_name(else_value, name)
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => expr_mutably_borrows_name(expr, name),
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            expr_mutably_borrows_name(cond, name)
                || body_changes_name(then_body, name)
                || body_changes_name(else_body, name)
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            body_changes_name(body, name)
        }
        Stmt::For { iter, body, .. } => {
            expr_mutably_borrows_name(iter, name) || body_changes_name(body, name)
        }
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
            block_changes_name(body, name)
        }
        Stmt::Match { expr, arms } => {
            expr_mutably_borrows_name(expr, name)
                || arms.iter().any(|arm| body_changes_name(&arm.body, name))
        }
        Stmt::InlineAsm(_) => true,
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => false,
    }
}

fn body_changes_name(body: &[IndentStmt], name: &str) -> bool {
    body.iter()
        .any(|indent| stmt_changes_name(&indent.stmt, name))
}

fn block_changes_name(block: &Block, name: &str) -> bool {
    body_changes_name(&block.stmts, name)
        || block
            .tail
            .as_ref()
            .is_some_and(|tail| expr_mutably_borrows_name(tail, name))
}

fn expr_mutably_borrows_name(expr: &Expr, name: &str) -> bool {
    walk::expr_any(expr, &mut |expr| match expr {
        Expr::Ref {
            mutable: true,
            expr,
        }
        | Expr::AddrOf {
            mutable: true,
            expr,
        } => matches!(&**expr, Expr::Var(var) if var.as_str() == name),
        _ => false,
    })
}

fn expr_writes_name(expr: &Expr, name: &str) -> bool {
    match expr {
        Expr::Var(var) => var.as_str() == name,
        Expr::Unary { expr, .. }
        | Expr::Field { base: expr, .. }
        | Expr::TupleField { base: expr, .. }
        | Expr::ArrayPtr { array: expr, .. } => expr_writes_name(expr, name),
        Expr::Index { base, .. } => expr_writes_name(base, name),
        _ => false,
    }
}

fn pattern_declares_name(pattern: &crate::rust_ast::Pattern, name: &str) -> bool {
    match pattern {
        crate::rust_ast::Pattern::Binding(binding) => binding.as_str() == name,
        crate::rust_ast::Pattern::TupleStruct { fields, .. } => fields
            .iter()
            .any(|field| pattern_declares_name(field, name)),
        crate::rust_ast::Pattern::Wildcard
        | crate::rust_ast::Pattern::I64(_)
        | crate::rust_ast::Pattern::I128(_)
        | crate::rust_ast::Pattern::U128(_) => false,
    }
}

fn is_temp_name(name: &str) -> bool {
    name.strip_prefix("_v")
        .is_some_and(|rest| !rest.is_empty() && rest.bytes().all(|b| b.is_ascii_digit()))
}
