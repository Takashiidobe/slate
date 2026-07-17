use crate::fixups::facts::{CallCallee, FixupFacts};
use crate::rust_ast::{ExternDecl, Item, Program};

pub(in crate::fixups) fn fixup(program: &mut Program, facts: &FixupFacts) {
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
