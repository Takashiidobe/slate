//! Inline single-use pure temps directly on the statement list. The temp's init
//! is spliced as an `Expr` subtree into its use site and precedence-aware
//! rendering elides redundant parens.

use crate::fixups::Fixup;
use crate::fixups::facts::{
    AstPath, EffectKind, EffectSubject, FixupFacts, FunctionId, PathSegment,
};
use crate::fixups::idents::expr_ident_count;
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, ast_path_fact, binding_facts, fact,
    function_path_location, path_fact, stmt_snippet,
};
use crate::rust_ast::{Expr, IndentStmt, Prim, RustValue, Stmt, Type};

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub(in crate::fixups) enum Phase {
    Early,
    Late,
}

pub(in crate::fixups) struct InlineTemps<'a> {
    phase: Phase,
    function: FunctionId,
    facts: &'a FixupFacts,
    logger: &'a mut dyn TraceLogger,
}

impl Fixup for InlineTemps<'_> {
    fn fixup(&mut self, body: &mut Vec<IndentStmt>) -> bool {
        self.fixup_at(body, self.function, self.facts, &mut Vec::new())
    }
}

impl<'a> InlineTemps<'a> {
    fn fixup_at(
        &mut self,
        body: &mut Vec<IndentStmt>,
        function: FunctionId,
        facts: &FixupFacts,
        path: &mut Vec<PathSegment>,
    ) -> bool {
        if self.inline_nested_temps(body, function, facts, path) {
            return true;
        }
        for i in 0..body.len() {
            let mut def_path = path.clone();
            def_path.push(PathSegment::Stmt(i));
            let Stmt::Let {
                name,
                mutable: false,
                init: Some(init),
                ty,
                ..
            } = &body[i].stmt
            else {
                continue;
            };
            if !is_temp_name(name) {
                continue;
            }
            let Some(binding) =
                facts.binding_by_local_path(function, name, &AstPath(def_path.clone()))
            else {
                continue;
            };
            let name = name.clone();
            let init = init.clone();
            let Some(use_index) = temp_chain_use_index(
                body,
                i,
                TempCandidate {
                    binding,
                    init: &init,
                    ty: ty.as_ref(),
                },
                InlineEnv {
                    function,
                    facts,
                    body_path: path,
                    phase: self.phase,
                },
            ) else {
                continue;
            };
            let trace_before = self
                .logger
                .is_enabled()
                .then(|| (body[i].stmt.clone(), body[use_index].stmt.clone()));
            if body[use_index].stmt.substitute_var(&name, &init) {
                if let Some((before_def, before_use)) = trace_before {
                    let after_use = body[use_index].stmt.clone();
                    self.log_inline_event(
                        &name,
                        binding,
                        function,
                        facts,
                        path,
                        i,
                        use_index,
                        &before_def,
                        &before_use,
                        &after_use,
                    );
                }
                body.remove(i);
                return true;
            }
        }
        false
    }

    fn inline_nested_temps(
        &mut self,
        body: &mut [IndentStmt],
        function: FunctionId,
        facts: &FixupFacts,
        path: &mut Vec<PathSegment>,
    ) -> bool {
        for (index, stmt) in body.iter_mut().enumerate() {
            let mut changed = false;
            walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
                walk::nested_body_vecs_mut_with_path(&mut stmt.stmt, path, &mut |body, path| {
                    if !changed && self.fixup_at(body, function, facts, path) {
                        changed = true;
                    }
                });
            });
            if changed {
                return true;
            }
        }
        false
    }

    #[allow(clippy::too_many_arguments)]
    fn log_inline_event(
        &mut self,
        name: &str,
        binding: crate::fixups::facts::BindingId,
        function: FunctionId,
        facts: &FixupFacts,
        body_path: &[PathSegment],
        def_index: usize,
        use_index: usize,
        before_def: &Stmt,
        before_use: &Stmt,
        after_use: &Stmt,
    ) {
        if !self.logger.is_enabled() {
            return;
        }
        let producer_path = stmt_path(body_path, def_index);
        let consumer_path = stmt_path(body_path, use_index);
        let mut event_facts = binding_facts(facts, binding);
        event_facts.extend([
            fact("temp", name),
            fact(
                "phase",
                match self.phase {
                    Phase::Early => "early",
                    Phase::Late => "late",
                },
            ),
            path_fact("producer_path", &producer_path),
            path_fact("consumer_path", &consumer_path),
        ]);
        if let Some(chain) = facts.temp_chains.iter().find(|fact| {
            fact.function == function
                && fact.binding == binding
                && fact.producer_path == AstPath(producer_path.clone())
        }) {
            event_facts.push(ast_path_fact("fact_consumer_path", &chain.consumer_path));
            event_facts.push(fact("dependencies", chain.dependencies.len().to_string()));
        }
        self.logger.rewrite(RewriteEvent {
            pass: match self.phase {
                Phase::Early => TracePass::EarlyInlineTemps,
                Phase::Late => TracePass::LateInlineTemps,
            },
            kind: "inline_temp".into(),
            location: function_path_location(facts, function, &consumer_path),
            before: vec![
                stmt_snippet("producer", before_def),
                stmt_snippet("consumer", before_use),
            ],
            after: vec![stmt_snippet("consumer", after_use)],
            facts: event_facts,
        });
    }

    pub(in crate::fixups) fn new(
        phase: Phase,
        function: FunctionId,
        facts: &'a FixupFacts,
        logger: &'a mut dyn TraceLogger,
    ) -> Self {
        Self {
            phase,
            function,
            facts,
            logger,
        }
    }
}

#[derive(Clone, Copy)]
struct TempCandidate<'a> {
    binding: crate::fixups::facts::BindingId,
    init: &'a Expr,
    ty: Option<&'a Type>,
}

#[derive(Clone, Copy)]
struct InlineEnv<'a> {
    function: FunctionId,
    facts: &'a FixupFacts,
    body_path: &'a [PathSegment],
    phase: Phase,
}

fn temp_chain_use_index(
    body: &[IndentStmt],
    def_index: usize,
    temp: TempCandidate<'_>,
    env: InlineEnv<'_>,
) -> Option<usize> {
    let fact = env.facts.temp_chains.iter().find(|fact| {
        fact.function == env.function
            && fact.binding == temp.binding
            && fact.producer_path == AstPath(stmt_path(env.body_path, def_index))
    })?;
    let use_index = direct_stmt_index(env.body_path, &fact.consumer_path)?;
    if use_index <= def_index || use_index >= body.len() {
        return None;
    }
    let use_path = stmt_path(env.body_path, use_index);
    let name = binding_name(env.facts, temp.binding)?;
    let allowed_receiver = is_option_receiver_use(&body[use_index].stmt, name, temp.ty);
    let producer_path = stmt_path(env.body_path, def_index);
    if env.phase == Phase::Early
        && is_effectful_expr(env.function, env.facts, &producer_path)
        && !early_effectful_consumer(&body[use_index].stmt, name)
    {
        return None;
    }
    let allowed_arg = is_allowed_argument_use(ArgumentUse {
        stmt: &body[use_index].stmt,
        name,
        init: temp.init,
        ty: temp.ty,
        producer_path: &producer_path,
        adjacent: use_index == def_index + 1,
        env,
    });
    if (stmt_contains_call(env.function, env.facts, &use_path) && !allowed_receiver && !allowed_arg)
        || (is_receiver_use(&body[use_index].stmt, name) && !allowed_receiver)
    {
        return None;
    }
    Some(use_index)
}

fn stmt_contains_call(function: FunctionId, facts: &FixupFacts, path: &[PathSegment]) -> bool {
    facts
        .effect(function, EffectSubject::Stmt, &AstPath(path.to_vec()))
        .is_some_and(|fact| {
            fact.effects.contains(&EffectKind::ReadOnlyCall)
                || fact.effects.contains(&EffectKind::UnknownCall)
                || fact.effects.contains(&EffectKind::MethodCall)
                || fact.effects.contains(&EffectKind::MacroExpansion)
        })
}

fn is_receiver_use(stmt: &Stmt, name: &str) -> bool {
    walk::stmt_expr_any(stmt, &mut |expr| {
        let receiver = match expr {
            Expr::MethodCall { recv, .. } | Expr::MethodCallGeneric { recv, .. } => Some(&**recv),
            Expr::Field { base, .. } | Expr::TupleField { base, .. } => Some(&**base),
            _ => None,
        };
        matches!(receiver, Some(Expr::Var(v)) if v.as_str() == name)
    })
}

fn is_option_receiver_use(stmt: &Stmt, name: &str, ty: Option<&Type>) -> bool {
    if !ty.is_some_and(is_option_like_type) {
        return false;
    }
    walk::stmt_expr_any(stmt, &mut |expr| {
        matches!(
            expr,
            Expr::MethodCall { recv, method, args }
                if matches!(method.as_str(), "is_some" | "is_none" | "unwrap")
                    && args.is_empty()
                    && matches!(&**recv, Expr::Var(v) if v.as_str() == name)
        )
    })
}

fn is_option_like_type(ty: &Type) -> bool {
    match ty {
        Type::FnPtr { .. } => true,
        Type::Generic { name, .. } => name == "Option",
        Type::Custom(name) => name.starts_with("Option<"),
        _ => false,
    }
}

fn early_effectful_consumer(stmt: &Stmt, name: &str) -> bool {
    match stmt {
        Stmt::Assign { target, value } => {
            matches!(target, Expr::Var(target) if target.as_str() == "__retval")
                && matches!(value, Expr::Var(value) if value.as_str() == name)
        }
        Stmt::Return(Some(expr)) => matches!(expr, Expr::Var(value) if value.as_str() == name),
        _ => false,
    }
}

struct ArgumentUse<'a> {
    stmt: &'a Stmt,
    name: &'a str,
    init: &'a Expr,
    ty: Option<&'a Type>,
    producer_path: &'a [PathSegment],
    adjacent: bool,
    env: InlineEnv<'a>,
}

fn is_allowed_argument_use(arg: ArgumentUse<'_>) -> bool {
    if arg.env.phase == Phase::Early {
        return false;
    }
    if is_effectful_expr(arg.env.function, arg.env.facts, arg.producer_path) {
        return arg.adjacent && simple_macro_arg_use(arg.stmt, arg.name);
    }
    if method_arg_use(arg.stmt, arg.name) && contains_integer_literal(arg.init) {
        return false;
    }
    type_stable_arg_init(arg.init, arg.ty) && call_or_macro_arg_use(arg.stmt, arg.name)
}

fn is_effectful_expr(function: FunctionId, facts: &FixupFacts, path: &[PathSegment]) -> bool {
    facts
        .effect(function, EffectSubject::Expr, &AstPath(path.to_vec()))
        .is_some_and(|fact| {
            fact.effects.iter().any(|effect| {
                matches!(
                    effect,
                    EffectKind::UnknownCall
                        | EffectKind::MethodCall
                        | EffectKind::MacroExpansion
                        | EffectKind::UnknownSideEffect
                        | EffectKind::VolatileRead
                        | EffectKind::VolatileWrite
                        | EffectKind::AtomicWrite
                        | EffectKind::MemoryWrite
                )
            })
        })
}

fn type_stable_arg_init(init: &Expr, ty: Option<&Type>) -> bool {
    match init {
        Expr::Var(_) | Expr::Cast { .. } => true,
        Expr::Unary { .. } => ty.is_some(),
        Expr::Binary { .. } => ty.is_some() && !contains_integer_literal(init),
        Expr::Index { .. } => ty.is_some(),
        Expr::Block(block) | Expr::Unsafe(block) if block.stmts.is_empty() => block
            .tail
            .as_deref()
            .is_some_and(|tail| type_stable_arg_init(tail, ty)),
        Expr::Value(RustValue::I64(_)) => matches!(ty, Some(Type::Prim(Prim::I32))),
        Expr::Value(RustValue::Bool(_)) => true,
        _ => false,
    }
}

fn contains_integer_literal(expr: &Expr) -> bool {
    match expr {
        Expr::Value(RustValue::I64(_) | RustValue::I128(_) | RustValue::Usize(_)) => true,
        Expr::Unary { expr, .. } | Expr::Cast { expr, .. } => contains_integer_literal(expr),
        Expr::Binary { lhs, rhs, .. } => {
            contains_integer_literal(lhs) || contains_integer_literal(rhs)
        }
        Expr::Index { base, .. } => contains_integer_literal(base),
        _ => false,
    }
}

fn call_or_macro_arg_use(stmt: &Stmt, name: &str) -> bool {
    match stmt {
        Stmt::Let { init, .. } => init
            .as_ref()
            .is_some_and(|expr| call_or_macro_arg_use_expr(expr, name)),
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            call_or_macro_arg_use_expr(target, name) || call_or_macro_arg_use_expr(value, name)
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => call_or_macro_arg_use_expr(expr, name),
        _ => false,
    }
}

fn method_arg_use(stmt: &Stmt, name: &str) -> bool {
    match stmt {
        Stmt::Let { init, .. } => init
            .as_ref()
            .is_some_and(|expr| method_arg_use_expr(expr, name)),
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            method_arg_use_expr(target, name) || method_arg_use_expr(value, name)
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => method_arg_use_expr(expr, name),
        _ => false,
    }
}

fn method_arg_use_expr(expr: &Expr, name: &str) -> bool {
    match expr {
        Expr::MethodCall { args, .. } | Expr::MethodCallGeneric { args, .. } => {
            args.iter().any(|arg| expr_ident_count(arg, name) > 0)
        }
        Expr::Block(block) | Expr::Unsafe(block) => block
            .tail
            .as_deref()
            .is_some_and(|tail| method_arg_use_expr(tail, name)),
        Expr::Cast { expr, .. } => method_arg_use_expr(expr, name),
        _ => false,
    }
}

fn call_or_macro_arg_use_expr(expr: &Expr, name: &str) -> bool {
    match expr {
        Expr::Call { args, .. } | Expr::Macro { args, .. } => {
            args.iter().any(|arg| call_arg_uses_name(arg, name))
        }
        Expr::MethodCall { args, .. } | Expr::MethodCallGeneric { args, .. } => {
            args.iter().any(|arg| expr_ident_count(arg, name) > 0)
        }
        Expr::Block(block) | Expr::Unsafe(block) => block
            .tail
            .as_deref()
            .is_some_and(|tail| call_or_macro_arg_use_expr(tail, name)),
        Expr::Cast { expr, .. } => call_or_macro_arg_use_expr(expr, name),
        _ => false,
    }
}

fn call_arg_uses_name(expr: &Expr, name: &str) -> bool {
    match expr {
        Expr::Var(var) => var.as_str() == name,
        Expr::Cast { expr, .. } => call_arg_uses_name(expr, name),
        Expr::Block(block) | Expr::Unsafe(block) if block.stmts.is_empty() => block
            .tail
            .as_deref()
            .is_some_and(|tail| call_arg_uses_name(tail, name)),
        _ => false,
    }
}

fn simple_macro_arg_use(stmt: &Stmt, name: &str) -> bool {
    match stmt {
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => simple_macro_arg_use_expr(expr, name),
        _ => false,
    }
}

fn simple_macro_arg_use_expr(expr: &Expr, name: &str) -> bool {
    match expr {
        Expr::Macro { args, .. } => {
            args.iter().any(|arg| simple_macro_arg_uses_name(arg, name))
                && args
                    .iter()
                    .all(|arg| simple_macro_arg_uses_name(arg, name) || is_obviously_pure_expr(arg))
        }
        Expr::Block(block) | Expr::Unsafe(block) => block
            .tail
            .as_deref()
            .is_some_and(|tail| simple_macro_arg_use_expr(tail, name)),
        _ => false,
    }
}

fn simple_macro_arg_uses_name(expr: &Expr, name: &str) -> bool {
    match expr {
        Expr::Var(var) => var.as_str() == name,
        Expr::Cast { expr, .. } | Expr::Unary { expr, .. } => {
            simple_macro_arg_uses_name(expr, name)
        }
        _ => false,
    }
}

fn is_obviously_pure_expr(expr: &Expr) -> bool {
    match expr {
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::ByteStr(_)
        | Expr::CStr(_)
        | Expr::HexFloat(_)
        | Expr::Var(_)
        | Expr::Path(_) => true,
        Expr::Cast { expr, .. } | Expr::Unary { expr, .. } => is_obviously_pure_expr(expr),
        _ => false,
    }
}

fn is_temp_name(name: &str) -> bool {
    name.strip_prefix("_v")
        .is_some_and(|rest| !rest.is_empty() && rest.bytes().all(|b| b.is_ascii_digit()))
}

fn stmt_path(body_path: &[PathSegment], index: usize) -> Vec<PathSegment> {
    let mut path = body_path.to_vec();
    path.push(PathSegment::Stmt(index));
    path
}

fn direct_stmt_index(body_path: &[PathSegment], read: &AstPath) -> Option<usize> {
    let rest = read.0.strip_prefix(body_path)?;
    match rest {
        [PathSegment::Stmt(index), ..] => Some(*index),
        _ => None,
    }
}

fn binding_name(facts: &FixupFacts, binding: crate::fixups::facts::BindingId) -> Option<&str> {
    facts
        .bindings
        .iter()
        .find(|fact| fact.id == binding)
        .map(|fact| fact.name.as_str())
}
