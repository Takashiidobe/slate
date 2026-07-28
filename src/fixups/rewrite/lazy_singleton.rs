//! Rewrite a proven lazy-init singleton (see `facts/lazy_singleton.rs`) into
//! a safe `OnceLock::get_or_init` call: the payload static becomes
//! `std::sync::OnceLock<T>`, the function body collapses to a single
//! `return`, and the guard flag static — provably dead once its only reader
//! and writer are gone — is dropped outright (`unused_items` only prunes
//! unused types, not statics).

use crate::fixups::facts::FixupFacts;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLocation, TraceLogger, TraceSnippet, fact,
};
use crate::rust_ast::{Expr, IndentStmt, Item, Program, Stmt, Type, UnaryOp};
use std::collections::BTreeSet;

pub(in crate::fixups) struct LazySingleton<'a> {
    facts: &'a FixupFacts,
    logger: &'a mut dyn TraceLogger,
}

impl<'a> LazySingleton<'a> {
    pub(in crate::fixups) fn new(facts: &'a FixupFacts, logger: &'a mut dyn TraceLogger) -> Self {
        Self { facts, logger }
    }

    pub(in crate::fixups) fn fixup(&mut self, program: &mut Program) -> bool {
        let before = self.logger.is_enabled().then(|| program.emit());
        let changed = fixup_impl(program, self.facts);
        if changed && let Some(before) = before {
            self.logger.rewrite(RewriteEvent {
                pass: TracePass::LazySingleton,
                kind: "rewrite_lazy_singletons".into(),
                location: TraceLocation::default(),
                before: vec![TraceSnippet::new("program", before.trim_end())],
                after: vec![TraceSnippet::new("program", program.emit().trim_end())],
                facts: vec![fact(
                    "lazy_singletons",
                    self.facts.lazy_init_singletons.len().to_string(),
                )],
            });
        }
        changed
    }
}

fn fixup_impl(program: &mut Program, facts: &FixupFacts) -> bool {
    let mut changed = false;
    let flag_names: BTreeSet<&str> = facts
        .lazy_init_singletons
        .iter()
        .map(|fact| fact.flag_name.as_str())
        .collect();
    for fact in &facts.lazy_init_singletons {
        for item in program.items.iter_mut() {
            if let Item::Static {
                mutable,
                name,
                ty,
                init,
                ..
            } = item
                && *name == fact.payload_name
            {
                *mutable = false;
                *ty = once_lock_type(fact.payload_ty.clone());
                *init = once_lock_new();
                changed = true;
            }
        }
        let Some(item_index) = facts.function_item_index(fact.function) else {
            continue;
        };
        if let Some(Item::Fn(f)) = program.items.get_mut(item_index) {
            f.body = vec![IndentStmt {
                depth: 1,
                stmt: Stmt::Return(Some(get_or_init_deref(
                    &fact.payload_name,
                    fact.init_expr.clone(),
                ))),
            }];
            changed = true;
        }
    }
    if !flag_names.is_empty() {
        let before = program.items.len();
        program.items.retain(
            |item| !matches!(item, Item::Static { name, .. } if flag_names.contains(name.as_str())),
        );
        changed |= program.items.len() != before;
    }
    changed
}

fn once_lock_type(payload_ty: Type) -> Type {
    Type::Generic {
        name: "std::sync::OnceLock".to_string(),
        args: vec![payload_ty],
    }
}

fn once_lock_new() -> Expr {
    Expr::Call {
        binding: crate::function_identity::CallBinding::Generated,
        func: Box::new(Expr::Var("std::sync::OnceLock::new".into())),
        args: Vec::new(),
    }
}

fn get_or_init_deref(payload_name: &str, init_expr: Expr) -> Expr {
    Expr::Unary {
        op: UnaryOp::Deref,
        expr: Box::new(Expr::MethodCall {
            recv: Box::new(Expr::Var(payload_name.into())),
            method: "get_or_init".to_string(),
            args: vec![Expr::Closure {
                params: Vec::new(),
                body: Box::new(init_expr),
            }],
        }),
    }
}
