use crate::fixups::facts::{
    AstPath, EffectKind, EffectSubject, FixupFacts, FunctionId, PathSegment,
};
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, binding_facts, fact, function_path_location,
    path_fact, stmt_snippet,
};
use crate::rust_ast::{Expr, IndentStmt, Stmt};

pub(in crate::fixups) fn fixup(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
) -> bool {
    let mut logger = crate::fixups::trace::NoopLogger;
    DeadLocals::new(TracePass::DeadLocals, &mut logger).fixup(body, function, facts)
}

pub(in crate::fixups) struct DeadLocals<'a> {
    pass: TracePass,
    logger: &'a mut dyn TraceLogger,
}

impl<'a> DeadLocals<'a> {
    pub(in crate::fixups) fn new(pass: TracePass, logger: &'a mut dyn TraceLogger) -> Self {
        Self { pass, logger }
    }

    pub(in crate::fixups) fn fixup(
        &mut self,
        body: &mut Vec<IndentStmt>,
        function: FunctionId,
        facts: &FixupFacts,
    ) -> bool {
        self.fixup_at(body, function, facts, &mut Vec::new())
    }

    fn fixup_at(
        &mut self,
        body: &mut Vec<IndentStmt>,
        function: FunctionId,
        facts: &FixupFacts,
        path: &mut Vec<PathSegment>,
    ) -> bool {
        for index in 0..body.len() {
            let mut changed = false;
            walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
                walk::nested_body_vecs_mut_with_path(
                    &mut body[index].stmt,
                    path,
                    &mut |body, path| {
                        if !changed {
                            changed = self.fixup_at(body, function, facts, path);
                        }
                    },
                );
            });
            if changed {
                return true;
            }
        }

        for index in 0..body.len() {
            path.push(PathSegment::Stmt(index));
            if removable_dead_local(&body[index].stmt, function, facts, path) {
                self.log_dead_local_event(&body[index].stmt, function, facts, path);
                body.remove(index);
                return true;
            }
            path.pop();
        }
        false
    }

    fn log_dead_local_event(
        &mut self,
        stmt: &Stmt,
        function: FunctionId,
        facts: &FixupFacts,
        path: &[PathSegment],
    ) {
        if !self.logger.is_enabled() {
            return;
        }
        let Stmt::Let { name, .. } = stmt else {
            return;
        };
        let Some(binding) = facts.binding_by_local_path(function, name, &AstPath(path.to_vec()))
        else {
            return;
        };
        let mut event_facts = binding_facts(facts, binding);
        event_facts.extend([
            fact("local", name),
            path_fact("decl_path", path),
            fact("discardable_init", "true"),
        ]);
        if let Some(effect) = facts.effect(function, EffectSubject::Expr, &AstPath(path.to_vec())) {
            event_facts.push(fact("purity", format!("{:?}", effect.purity)));
            event_facts.push(fact("effects", format!("{:?}", effect.effects)));
        }
        self.logger.rewrite(RewriteEvent {
            pass: self.pass,
            kind: "remove_dead_local".into(),
            location: function_path_location(facts, function, path),
            before: vec![stmt_snippet("declaration", stmt)],
            after: Vec::new(),
            facts: event_facts,
        });
    }
}

fn removable_dead_local(
    stmt: &Stmt,
    function: FunctionId,
    facts: &FixupFacts,
    path: &[PathSegment],
) -> bool {
    let Stmt::Let {
        name,
        init: Some(init),
        ..
    } = stmt
    else {
        return false;
    };
    let Some(binding) = facts.binding_by_local_path(function, name, &AstPath(path.to_vec())) else {
        return false;
    };
    facts
        .def_use(binding)
        .is_some_and(|def_use| def_use.reads.is_empty() && def_use.writes.is_empty())
        && discardable_init(init, function, facts, path)
}

fn discardable_init(
    expr: &Expr,
    function: FunctionId,
    facts: &FixupFacts,
    path: &[PathSegment],
) -> bool {
    if discardable_known_method(expr) {
        return true;
    }
    let Some(effect) = facts.effect(function, EffectSubject::Expr, &AstPath(path.to_vec())) else {
        return false;
    };
    effect.effects.iter().all(discardable_effect)
}

fn discardable_effect(effect: &EffectKind) -> bool {
    !matches!(
        effect,
        EffectKind::ReadOnlyCall
            | EffectKind::UnknownCall
            | EffectKind::MethodCall
            | EffectKind::MacroExpansion
            | EffectKind::VolatileRead
            | EffectKind::VolatileWrite
            | EffectKind::AtomicRead
            | EffectKind::AtomicWrite
            | EffectKind::MemoryWrite
            | EffectKind::UnknownSideEffect
    )
}

fn discardable_known_method(expr: &Expr) -> bool {
    match expr {
        Expr::Cast { expr, .. } | Expr::Transmute { expr, .. } => discardable_known_method(expr),
        Expr::MethodCall { recv, method, args } if method == "len" && args.is_empty() => {
            discardable_receiver(recv)
        }
        _ => false,
    }
}

fn discardable_receiver(expr: &Expr) -> bool {
    match expr {
        Expr::Var(_) => true,
        Expr::MethodCall { recv, method, args }
            if matches!(method.as_str(), "as_slice" | "as_mut_slice") && args.is_empty() =>
        {
            discardable_receiver(recv)
        }
        _ => false,
    }
}
