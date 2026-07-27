//! Fold a parameter's stack spill directly into a mutable binding when the
//! parameter is spilled to a single local and never otherwise read.

use crate::fixups::facts::{
    AstPath, BindingId, FixupFacts, FunctionId, PathSegment, PlaceAccess, PlaceKind,
};
use crate::fixups::idents::expr_ident;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, fact, function_path_location, stmts_snippet,
};
use crate::rust_ast::{FnDef, IndentStmt, Stmt};

pub(in crate::fixups) fn fixup(f: &mut FnDef, function: FunctionId, facts: &FixupFacts) {
    let mut logger = crate::fixups::trace::NoopLogger;
    ParamSpills::new(&mut logger).fixup(f, function, facts);
}

pub(in crate::fixups) struct ParamSpills<'a> {
    logger: &'a mut dyn TraceLogger,
}

impl<'a> ParamSpills<'a> {
    pub(in crate::fixups) fn new(logger: &'a mut dyn TraceLogger) -> Self {
        Self { logger }
    }

    pub(in crate::fixups) fn fixup(
        &mut self,
        f: &mut FnDef,
        function: FunctionId,
        facts: &FixupFacts,
    ) {
        let before = self.logger.is_enabled().then(|| f.body.clone());
        fixup_impl(f, function, facts);
        if let Some(before) = before
            && body_code(&before) != body_code(&f.body)
        {
            self.logger.rewrite(RewriteEvent {
                pass: TracePass::ParamSpills,
                kind: "fold_param_spills".into(),
                location: function_path_location(facts, function, &[]),
                before: vec![stmts_snippet("body", &before)],
                after: vec![stmts_snippet("body", &f.body)],
                facts: vec![fact("params", f.params.len().to_string())],
            });
        }
    }
}

fn fixup_impl(f: &mut FnDef, function: FunctionId, facts: &FixupFacts) {
    let param_names: Vec<String> = f.params.iter().map(|p| p.name.clone()).collect();
    let mut claimed_locals: Vec<String> = Vec::new();
    let mut removed: Vec<usize> = Vec::new();

    for param_index in 0..f.params.len() {
        if f.params[param_index].mutable {
            continue;
        }
        let Some(param_binding) = facts.binding_by_param_index(function, param_index) else {
            continue;
        };
        let Some(param_def_use) = facts.def_use(param_binding) else {
            continue;
        };
        if param_def_use.reads.len() != 1 || !param_def_use.writes.is_empty() {
            continue;
        }

        let param_name = f.params[param_index].name.clone();
        let Some((store_index, local)) =
            f.body
                .iter()
                .enumerate()
                .find_map(|(index, stmt)| match &stmt.stmt {
                    Stmt::Assign { target, value }
                        if param_def_use.reads == [AstPath(stmt_path(index))] =>
                    {
                        let local = expr_ident(target)?;
                        (expr_ident(value) == Some(param_name.as_str()))
                            .then(|| (index, local.to_string()))
                    }
                    _ => None,
                })
        else {
            continue;
        };
        if param_names.iter().any(|name| name == &local)
            || claimed_locals.iter().any(|name| name == &local)
        {
            continue;
        }

        let store_path = AstPath(stmt_path(store_index));
        let Some(local_binding) = facts
            .bindings
            .iter()
            .find(|binding| binding.function == function && binding.name == local)
            .map(|binding| binding.id)
        else {
            continue;
        };
        if !store_writes_local(function, facts, &store_path, local_binding) {
            continue;
        }

        let Some(decl_index) =
            f.body
                .iter()
                .enumerate()
                .take(store_index)
                .find_map(|(index, stmt)| match &stmt.stmt {
                    Stmt::Let {
                        name,
                        mutable: true,
                        ..
                    } if name == &local => Some(index),
                    _ => None,
                })
        else {
            continue;
        };
        let decl_path = AstPath(stmt_path(decl_index));
        if facts
            .local_binding_at(function, &decl_path)
            .map(|fact| fact.id)
            != Some(local_binding)
        {
            continue;
        }
        if facts.binding_type(param_binding) != facts.binding_type(local_binding) {
            continue;
        }

        let Some(local_def_use) = facts.def_use(local_binding) else {
            continue;
        };
        if local_read_between_decl_and_store(&local_def_use.reads, decl_index, store_index) {
            continue;
        }

        f.params[param_index].name = local.clone();
        f.params[param_index].mutable = true;
        claimed_locals.push(local);
        removed.push(decl_index);
        removed.push(store_index);
    }

    removed.sort_unstable();
    removed.dedup();
    for index in removed.into_iter().rev() {
        f.body.remove(index);
    }
}

fn body_code(body: &[IndentStmt]) -> String {
    body.iter()
        .map(|stmt| stmt.stmt.render())
        .collect::<Vec<_>>()
        .join("")
}

fn store_writes_local(
    function: FunctionId,
    facts: &FixupFacts,
    path: &AstPath,
    binding: BindingId,
) -> bool {
    let Some(name) = facts.binding_name(binding) else {
        return false;
    };
    facts.place(function, path).is_some_and(|fact| {
        fact.access == PlaceAccess::Write
            && fact.ordinary_slot
            && matches!(&fact.kind, PlaceKind::Local { name: place } if place == name)
    })
}

fn local_read_between_decl_and_store(
    reads: &[AstPath],
    decl_index: usize,
    store_index: usize,
) -> bool {
    reads.iter().any(|read| {
        matches!(
            read.0.as_slice(),
            [PathSegment::Stmt(index), ..] if *index > decl_index && *index < store_index
        )
    })
}

fn stmt_path(index: usize) -> Vec<PathSegment> {
    vec![PathSegment::Stmt(index)]
}
