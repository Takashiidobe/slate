use crate::fixups::facts::{CallCallee, FixupFacts};
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLocation, TraceLogger, TraceSnippet, fact,
};
use crate::rust_ast::{ExternDecl, Item, Program};

pub(in crate::fixups) fn fixup(program: &mut Program, facts: &FixupFacts) {
    let mut logger = crate::fixups::trace::NoopLogger;
    PruneUnusedExterns::new(&mut logger).fixup(program, facts);
}

pub(in crate::fixups) struct PruneUnusedExterns<'a> {
    logger: &'a mut dyn TraceLogger,
}

impl<'a> PruneUnusedExterns<'a> {
    pub(in crate::fixups) fn new(logger: &'a mut dyn TraceLogger) -> Self {
        Self { logger }
    }

    pub(in crate::fixups) fn fixup(&mut self, program: &mut Program, facts: &FixupFacts) {
        let before = self.logger.is_enabled().then(|| program.emit());
        fixup_impl(program, facts);
        if let Some(before) = before {
            let after = program.emit();
            if before != after {
                self.logger.rewrite(RewriteEvent {
                    pass: TracePass::PruneUnusedExterns,
                    kind: "prune_unused_extern_decls".into(),
                    location: TraceLocation::default(),
                    before: vec![TraceSnippet::new("program", before.trim_end())],
                    after: vec![TraceSnippet::new("program", after.trim_end())],
                    facts: vec![fact("direct_calls", direct_calls(facts).len().to_string())],
                });
            }
        }
    }
}

fn fixup_impl(program: &mut Program, facts: &FixupFacts) {
    let used = direct_calls(facts);
    program.items.retain_mut(|item| match item {
        Item::ExternBlock { decls, .. } => {
            decls.retain(|decl| match decl {
                ExternDecl::Fn(f) if is_prunable(&f.name) => used.contains(&f.name),
                _ => true,
            });
            !decls.is_empty()
        }
        _ => true,
    });
}

fn is_prunable(name: &str) -> bool {
    matches!(
        name,
        "strlen"
            | "strcmp"
            | "strncmp"
            | "memcmp"
            | "strchr"
            | "strrchr"
            | "strstr"
            | "strpbrk"
            | "strspn"
            | "strcspn"
            | "atoi"
            | "atol"
            | "strtol"
            | "strtoul"
            | "strtod"
            | "strcpy"
            | "strncpy"
            | "strcat"
            | "strncat"
            | "qsort"
            | "bsearch"
            | "exit"
            | "malloc"
            | "calloc"
            | "realloc"
            | "free"
    )
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
