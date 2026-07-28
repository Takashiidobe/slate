use crate::fixups::facts::{AstPath, BindingKind, FixupFacts, FunctionId, PathSegment};
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, function_path_location, stmts_snippet,
};
use crate::rust_ast::{FnDef, IndentStmt};

pub(in crate::fixups) struct VaList<'a> {
    function: FunctionId,
    facts: &'a FixupFacts,
    logger: &'a mut dyn TraceLogger,
}

impl<'a> VaList<'a> {
    pub(in crate::fixups) fn new(
        function: FunctionId,
        facts: &'a FixupFacts,
        logger: &'a mut dyn TraceLogger,
    ) -> Self {
        Self {
            function,
            facts,
            logger,
        }
    }

    pub(in crate::fixups) fn fixup(&mut self, f: &mut FnDef) -> bool {
        let before = self.logger.is_enabled().then(|| f.body.clone());
        let changed = fixup_fn(f, self.function, self.facts);
        if changed && let Some(before) = before {
            self.logger.rewrite(RewriteEvent {
                pass: TracePass::VaList,
                kind: "remove_va_list_clone_alias".into(),
                location: function_path_location(self.facts, self.function, &[]),
                before: vec![stmts_snippet("body", &before)],
                after: vec![stmts_snippet("body", &f.body)],
                facts: Vec::new(),
            });
        }
        changed
    }
}

fn fixup_fn(f: &mut FnDef, function: FunctionId, facts: &FixupFacts) -> bool {
    let aliases = facts
        .va_list_aliases
        .iter()
        .filter(|alias| alias.function == function)
        .collect::<Vec<_>>();
    let [alias] = aliases.as_slice() else {
        return false;
    };
    let Some(param_index) = facts.bindings.iter().find_map(|binding| {
        (binding.id == alias.param)
            .then_some(binding.kind.clone())
            .and_then(|kind| match kind {
                BindingKind::Param { index } => Some(index),
                BindingKind::Local => None,
            })
    }) else {
        return false;
    };
    let Some(local_name) = facts.binding_name(alias.local) else {
        return false;
    };
    let Some(param) = f.params.get_mut(param_index) else {
        return false;
    };

    if remove_alias_stmts(
        &mut f.body,
        &alias.local_decl_path,
        &alias.clone_assign_path,
        &mut Vec::new(),
    ) != 2
    {
        return false;
    }
    param.name = local_name.to_string();
    true
}

fn remove_alias_stmts(
    body: &mut Vec<IndentStmt>,
    local_decl_path: &AstPath,
    clone_assign_path: &AstPath,
    path: &mut Vec<PathSegment>,
) -> usize {
    let mut removed = 0;
    for index in (0..body.len()).rev() {
        let mut stmt_path = path.to_vec();
        stmt_path.push(PathSegment::Stmt(index));
        let stmt_path = AstPath(stmt_path);
        if local_decl_path == &stmt_path || clone_assign_path == &stmt_path {
            body.remove(index);
            removed += 1;
        }
    }

    if removed > 0 {
        return removed;
    }

    for (index, indent) in body.iter_mut().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            walk::nested_body_vecs_mut_with_path(&mut indent.stmt, path, &mut |body, path| {
                removed += remove_alias_stmts(body, local_decl_path, clone_assign_path, path);
            });
        });
        if removed > 0 {
            break;
        }
    }
    removed
}
