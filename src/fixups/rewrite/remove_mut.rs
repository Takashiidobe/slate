use crate::fixups::facts::{
    AstPath, BindingId, BorrowAliasReason, CallArgPinning, FixupFacts, FunctionId, PathSegment,
};
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, TraceSnippet, binding_facts, fact,
    function_path_location, path_fact, stmt_snippet,
};
use crate::rust_ast::{FnDef, IndentStmt, Stmt, Type};

pub(in crate::fixups) struct RemoveMut<'a> {
    function: FunctionId,
    facts: &'a FixupFacts,
    logger: &'a mut dyn TraceLogger,
}

impl<'a> RemoveMut<'a> {
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
        let mut changed = false;
        for (index, param) in f.params.iter_mut().enumerate() {
            if matches!(param.ty, Type::Variadic | Type::VaList) {
                continue;
            }
            let Some(binding) = self.facts.binding_by_param_index(self.function, index) else {
                continue;
            };
            if param_can_drop_mut(binding, self.facts) {
                let before = self
                    .logger
                    .is_enabled()
                    .then(|| format!("mut {}: {}", param.name, param.ty.render()));
                let param_name = param.name.clone();
                let param_ty = param.ty.render();
                changed |= param.mutable;
                param.mutable = false;
                if let Some(before) = before {
                    let mut event_facts = binding_facts(self.facts, binding);
                    event_facts.extend([
                        fact("param", param_name.clone()),
                        fact("param_index", index.to_string()),
                    ]);
                    self.logger.rewrite(RewriteEvent {
                        pass: TracePass::RemoveMut,
                        kind: "remove_param_mut".into(),
                        location: function_path_location(self.facts, self.function, &[]),
                        before: vec![TraceSnippet::new("param", before)],
                        after: vec![TraceSnippet::new(
                            "param",
                            format!("{param_name}: {param_ty}"),
                        )],
                        facts: event_facts,
                    });
                }
            }
        }
        changed |= self.remove_unneeded_mut(&mut f.body, &mut Vec::new());
        changed
    }
}

fn param_can_drop_mut(binding: BindingId, facts: &FixupFacts) -> bool {
    if !facts.binding_requires_mut(binding) {
        return true;
    }
    let Some(def_use) = facts.def_use(binding) else {
        return false;
    };
    if !def_use.writes.is_empty() {
        return false;
    }
    let Some(alias) = facts
        .borrow_alias
        .iter()
        .find(|fact| fact.binding == binding)
    else {
        return true;
    };
    if !alias.reasons.iter().all(|reason| {
        matches!(
            reason,
            BorrowAliasReason::Read | BorrowAliasReason::UnknownCallEscape
        )
    }) {
        return false;
    }
    def_use.reads.iter().all(|read| {
        facts
            .call_arg_at(alias.function, read)
            .is_none_or(|(_, arg)| {
                arg.pinning == CallArgPinning::DeclaredParam
                    && matches!(
                        arg.declared_ty.as_ref(),
                        Some(Type::Ref { mutable: false, .. })
                    )
            })
    })
}

impl<'a> RemoveMut<'a> {
    fn remove_unneeded_mut(
        &mut self,
        body: &mut [IndentStmt],
        path: &mut Vec<PathSegment>,
    ) -> bool {
        let mut changed = false;
        for (index, indent) in body.iter_mut().enumerate() {
            path.push(PathSegment::Stmt(index));
            changed |= self.remove_stmt_unneeded_mut(&mut indent.stmt, path);
            path.pop();
        }
        changed
    }

    fn remove_stmt_unneeded_mut(&mut self, stmt: &mut Stmt, path: &mut Vec<PathSegment>) -> bool {
        let before = self.logger.is_enabled().then(|| stmt.clone());
        let mut removed_name = None;
        let mut changed = false;
        match stmt {
            Stmt::Let { name, mutable, .. } | Stmt::LetIf { name, mutable, .. }
                if local_can_drop_mut(self.function, self.facts, name, path) =>
            {
                removed_name = Some(name.clone());
                changed |= *mutable;
                *mutable = false;
            }
            _ => {}
        }
        if let (Some(before), Some(name)) = (before, removed_name) {
            let mut event_facts = if let Some(binding) =
                self.facts
                    .binding_by_local_path(self.function, &name, &AstPath(path.to_vec()))
            {
                binding_facts(self.facts, binding)
            } else {
                vec![fact("binding_name", name)]
            };
            event_facts.push(path_fact("stmt_path", path));
            self.logger.rewrite(RewriteEvent {
                pass: TracePass::RemoveMut,
                kind: "remove_local_mut".into(),
                location: function_path_location(self.facts, self.function, path),
                before: vec![stmt_snippet("binding", &before)],
                after: vec![stmt_snippet("binding", stmt)],
                facts: event_facts,
            });
        }
        walk::nested_bodies_mut_with_path(stmt, path, &mut |body, path| {
            changed |= self.remove_unneeded_mut(body, path);
        });
        changed
    }
}

fn local_can_drop_mut(
    function: FunctionId,
    facts: &FixupFacts,
    name: &str,
    path: &[PathSegment],
) -> bool {
    facts
        .binding_by_local_path(function, name, &AstPath(path.to_vec()))
        .is_some_and(|binding| !facts.binding_requires_mut(binding))
}
