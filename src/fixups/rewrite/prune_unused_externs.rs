use crate::fixups::facts::{CallCallee, FixupFacts};
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLocation, TraceLogger, TraceSnippet, fact,
};
use crate::function_identity::known_declaration;
use crate::rust_ast::{ExternDecl, Item, Program};

pub(in crate::fixups) struct PruneUnusedExterns<'a> {
    facts: &'a FixupFacts,
    logger: &'a mut dyn TraceLogger,
}

impl<'a> PruneUnusedExterns<'a> {
    pub(in crate::fixups) fn new(facts: &'a FixupFacts, logger: &'a mut dyn TraceLogger) -> Self {
        Self { facts, logger }
    }

    pub(in crate::fixups) fn fixup(&mut self, program: &mut Program) -> bool {
        let before = self.logger.is_enabled().then(|| program.emit());
        let changed = fixup_impl(program, self.facts);
        if changed && let Some(before) = before {
            self.logger.rewrite(RewriteEvent {
                pass: TracePass::PruneUnusedExterns,
                kind: "prune_unused_extern_decls".into(),
                location: TraceLocation::default(),
                before: vec![TraceSnippet::new("program", before.trim_end())],
                after: vec![TraceSnippet::new("program", program.emit().trim_end())],
                facts: vec![fact(
                    "direct_calls",
                    direct_calls(self.facts).len().to_string(),
                )],
            });
        }
        changed
    }
}

fn fixup_impl(program: &mut Program, facts: &FixupFacts) -> bool {
    let used = direct_calls(facts);
    let mut changed = false;
    program.items.retain_mut(|item| match item {
        Item::ExternBlock { decls, .. } => {
            let before = decls.len();
            decls.retain(|decl| match decl {
                ExternDecl::Fn(f) if known_declaration(f.identity, &f.name).is_some() => {
                    used.contains(&f.name)
                }
                _ => true,
            });
            changed |= decls.len() != before;
            changed |= decls.is_empty();
            !decls.is_empty()
        }
        _ => true,
    });
    changed
}

fn direct_calls(facts: &FixupFacts) -> Vec<String> {
    let mut calls = facts
        .callsites
        .iter()
        .filter_map(|callsite| match &callsite.callee {
            CallCallee::Direct { name, .. } => Some(name.clone()),
            CallCallee::Indirect => None,
        })
        .collect::<Vec<_>>();
    calls.sort();
    calls.dedup();
    calls
}
