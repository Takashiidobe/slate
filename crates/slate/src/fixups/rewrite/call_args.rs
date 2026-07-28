//! Inline single-use argument temps into type-safe call positions.
//!
//! Baseline lowering materializes every call argument as its own `let` temp:
//! `let _v1: i32 = 2; let _v3 = add(_v1, _v2); printf(fmt, _v3)`. The pure-temp
//! inliner ([`super::inline_temps`]) deliberately refuses call-argument slots,
//! because dropping a literal's type annotation into a vararg slot would change
//! its inferred type — `printf(_v0)` with `_v0: i64 = 9223372036854775807` must
//! not become `printf(9223372036854775807)`, where the literal defaults to `i32`.
//!
//! This pass inlines exactly the two positions where the argument's Rust type is
//! pinned by something other than the temp's annotation:
//!
//! - **(a)** a pure temp whose single use is a *declared* parameter slot of a
//!   non-variadic callee — the parameter type pins any literal.
//! - **(b)** a temp initialized by a call to a known function — its Rust type is
//!   fixed by the callee's return type, so inlining is safe even in a vararg
//!   slot; the between-statements guard keeps its side effect from being
//!   reordered.

use crate::fixups::Fixup;
use crate::fixups::facts::{
    AstPath, CallArgPinning, CallCallee, EffectSubject, FixupFacts, FunctionId, PathSegment, Purity,
};
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, binding_facts, fact, function_path_location,
    path_fact, stmt_snippet,
};
use crate::rust_ast::{Expr, IndentStmt, Stmt};

pub(in crate::fixups) struct CallArgs<'a> {
    function: FunctionId,
    facts: &'a FixupFacts,
    logger: &'a mut dyn TraceLogger,
}

impl Fixup for CallArgs<'_> {
    fn fixup(&mut self, body: &mut Vec<IndentStmt>) -> bool {
        self.fixup_at(body, self.function, self.facts, &mut Vec::new())
    }
}

impl<'a> CallArgs<'a> {
    fn fixup_at(
        &mut self,
        body: &mut Vec<IndentStmt>,
        function: FunctionId,
        facts: &FixupFacts,
        path: &mut Vec<PathSegment>,
    ) -> bool {
        if self.fixup_nested(body, function, facts, path) {
            return true;
        }
        for i in 0..body.len() {
            let def_path = stmt_path(path, i);
            let Stmt::Let {
                name,
                mutable: false,
                init: Some(init),
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
            let Some((use_index, arg_use)) =
                single_arg_use(body, i, binding, function, facts, path)
            else {
                continue;
            };
            let slot = arg_use.slot();
            let mut arg_path = stmt_path(path, use_index);
            arg_path.push(PathSegment::Expr(slot + 1));
            if !inlinable(function, facts, &def_path, &arg_path, arg_use) {
                continue;
            }
            let trace_before = self
                .logger
                .is_enabled()
                .then(|| (body[i].stmt.clone(), body[use_index].stmt.clone()));
            if body[use_index].stmt.substitute_var(&name, &init) {
                if let Some((before_def, before_use)) = trace_before {
                    let after_use = body[use_index].stmt.clone();
                    self.log_inline_arg(
                        function,
                        facts,
                        binding,
                        &name,
                        arg_use,
                        &def_path,
                        &arg_path,
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

    fn fixup_nested(
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
    fn log_inline_arg(
        &mut self,
        function: FunctionId,
        facts: &FixupFacts,
        binding: crate::fixups::facts::BindingId,
        temp: &str,
        arg_use: ArgUse,
        def_path: &[PathSegment],
        arg_path: &[PathSegment],
        before_def: &Stmt,
        before_use: &Stmt,
        after_use: &Stmt,
    ) {
        if !self.logger.is_enabled() {
            return;
        }
        let mut event_facts = binding_facts(facts, binding);
        event_facts.extend([
            fact("temp", temp),
            fact("arg_slot", arg_use.slot().to_string()),
            fact(
                "arg_use",
                match arg_use {
                    ArgUse::DirectCall { .. } => "direct_call",
                    ArgUse::FunctionPointerCall { .. } => "function_pointer_call",
                },
            ),
            path_fact("producer_path", def_path),
            path_fact("arg_path", arg_path),
        ]);
        self.logger.rewrite(RewriteEvent {
            pass: TracePass::CallArgs,
            kind: "inline_call_arg_temp".into(),
            location: function_path_location(facts, function, arg_path),
            before: vec![
                stmt_snippet("producer", before_def),
                stmt_snippet("consumer", before_use),
            ],
            after: vec![stmt_snippet("consumer", after_use)],
            facts: event_facts,
        });
    }

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
}

/// The single use of `name` after `def_index`, as `(use_index, arg_use)`,
/// when that use is a top-level argument of a pinned call and every statement in
/// between is a pure temp-let.
fn single_arg_use(
    body: &[IndentStmt],
    def_index: usize,
    binding: crate::fixups::facts::BindingId,
    function: FunctionId,
    facts: &FixupFacts,
    body_path: &[PathSegment],
) -> Option<(usize, ArgUse)> {
    let reads = &facts.def_use(binding)?.reads;
    if reads.len() != 1 {
        return None;
    }
    let use_index = direct_stmt_index(body_path, &reads[0])?;
    if use_index <= def_index || use_index >= body.len() {
        return None;
    }
    let name = binding_name(facts, binding)?;
    let arg_use = find_arg_use(&body[use_index].stmt, name)?;
    for (index, indent) in body.iter().enumerate().take(use_index).skip(def_index + 1) {
        if !is_pure_temp_let(&indent.stmt, function, facts, &stmt_path(body_path, index)) {
            return None;
        }
    }
    Some((use_index, arg_use))
}

#[derive(Clone, Copy)]
enum ArgUse {
    DirectCall { slot: usize },
    FunctionPointerCall { slot: usize },
}

impl ArgUse {
    fn slot(self) -> usize {
        match self {
            ArgUse::DirectCall { slot } | ArgUse::FunctionPointerCall { slot } => slot,
        }
    }
}

fn find_arg_use(stmt: &Stmt, name: &str) -> Option<ArgUse> {
    match stmt {
        Stmt::Let { init, .. } => init.as_ref().and_then(|expr| find_arg_use_expr(expr, name)),
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            find_arg_use_expr(target, name).or_else(|| find_arg_use_expr(value, name))
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => find_arg_use_expr(expr, name),
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => find_arg_use_expr(cond, name)
            .or_else(|| find_arg_use_body(then_body, name))
            .or_else(|| find_arg_use_body(else_body, name)),
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => find_arg_use_expr(cond, name)
            .or_else(|| find_arg_use_body(then_body, name))
            .or_else(|| find_arg_use_expr(then_value, name))
            .or_else(|| find_arg_use_body(else_body, name))
            .or_else(|| find_arg_use_expr(else_value, name)),
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            find_arg_use_body(body, name)
        }
        Stmt::For { iter, body, .. } => {
            find_arg_use_expr(iter, name).or_else(|| find_arg_use_body(body, name))
        }
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
            find_arg_use_body(&body.stmts, name).or_else(|| {
                body.tail
                    .as_deref()
                    .and_then(|tail| find_arg_use_expr(tail, name))
            })
        }
        Stmt::Match { expr, arms } => find_arg_use_expr(expr, name).or_else(|| {
            arms.iter()
                .find_map(|arm| find_arg_use_body(&arm.body, name))
        }),
        Stmt::InlineAsm(_) | Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => None,
    }
}

fn find_arg_use_body(body: &[IndentStmt], name: &str) -> Option<ArgUse> {
    body.iter()
        .find_map(|indent| find_arg_use(&indent.stmt, name))
}

fn find_arg_use_expr(expr: &Expr, name: &str) -> Option<ArgUse> {
    if let Expr::Call { func, args, .. } = expr
        && let Some(slot) = args
            .iter()
            .position(|arg| matches!(arg, Expr::Var(v) if v.as_str() == name))
    {
        if matches!(&**func, Expr::Var(_)) {
            return Some(ArgUse::DirectCall { slot });
        }
        if is_option_unwrap_callee(func) {
            return Some(ArgUse::FunctionPointerCall { slot });
        }
    }
    match expr {
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. }
        | Expr::Closure { body: expr, .. }
        | Expr::AtomicNew { value: expr, .. } => find_arg_use_expr(expr, name),
        Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. } => place
            .ptr_expr()
            .and_then(|ptr| find_arg_use_expr(ptr, name)),
        Expr::Binary { lhs, rhs, .. }
        | Expr::Range {
            start: lhs,
            end: rhs,
        }
        | Expr::Index {
            base: lhs,
            index: rhs,
        } => find_arg_use_expr(lhs, name).or_else(|| find_arg_use_expr(rhs, name)),
        Expr::Call { func, args, .. } => find_arg_use_expr(func, name)
            .or_else(|| args.iter().find_map(|arg| find_arg_use_expr(arg, name))),
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            find_arg_use_expr(recv, name)
                .or_else(|| args.iter().find_map(|arg| find_arg_use_expr(arg, name)))
        }
        Expr::Field { base, .. }
        | Expr::TupleField { base, .. }
        | Expr::ArrayPtr { array: base, .. } => find_arg_use_expr(base, name),
        Expr::StructLit { fields, .. } => fields
            .iter()
            .find_map(|(_, value)| find_arg_use_expr(value, name)),
        Expr::TupleStructLit { fields, .. } => fields
            .iter()
            .find_map(|value| find_arg_use_expr(value, name)),
        Expr::ArrayLit(elems) => elems.iter().find_map(|elem| find_arg_use_expr(elem, name)),
        Expr::ArrayRepeat { elem, .. } => find_arg_use_expr(elem, name),
        Expr::VecLit(elems) => elems.iter().find_map(|elem| find_arg_use_expr(elem, name)),
        Expr::VecRepeat { elem, len } => {
            find_arg_use_expr(elem, name).or_else(|| find_arg_use_expr(len, name))
        }
        Expr::Macro { args, .. } => args.iter().find_map(|arg| find_arg_use_expr(arg, name)),
        Expr::Match { expr, arms } => find_arg_use_expr(expr, name).or_else(|| {
            arms.iter()
                .find_map(|arm| find_arg_use_expr(&arm.value, name))
        }),
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => find_arg_use_expr(cond, name)
            .or_else(|| find_arg_use_expr(then_expr, name))
            .or_else(|| find_arg_use_expr(else_expr, name)),
        Expr::Block(block) | Expr::Unsafe(block) => {
            find_arg_use_body(&block.stmts, name).or_else(|| {
                block
                    .tail
                    .as_deref()
                    .and_then(|tail| find_arg_use_expr(tail, name))
            })
        }
        Expr::AtomicStore { place, value, .. }
        | Expr::AtomicFetch { place, value, .. }
        | Expr::AtomicSwap { place, value, .. } => place
            .ptr_expr()
            .and_then(|ptr| find_arg_use_expr(ptr, name))
            .or_else(|| find_arg_use_expr(value, name)),
        Expr::AtomicCompareExchange {
            place,
            expected,
            desired,
            ..
        } => place
            .ptr_expr()
            .and_then(|ptr| find_arg_use_expr(ptr, name))
            .or_else(|| find_arg_use_expr(expected, name))
            .or_else(|| find_arg_use_expr(desired, name)),
        Expr::CopyNonoverlapping { src, dst, .. } => {
            find_arg_use_expr(src, name).or_else(|| find_arg_use_expr(dst, name))
        }
        Expr::PtrCopy {
            src, dst, count, ..
        } => find_arg_use_expr(src, name)
            .or_else(|| find_arg_use_expr(dst, name))
            .or_else(|| find_arg_use_expr(count, name)),
        Expr::WriteBytes { dst, val, count } => find_arg_use_expr(dst, name)
            .or_else(|| find_arg_use_expr(val, name))
            .or_else(|| find_arg_use_expr(count, name)),
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::CStr(_)
        | Expr::Var(_)
        | Expr::Path(_)
        | Expr::Todo(_)
        | Expr::AtomicFence { .. } => None,
    }
}

fn is_option_unwrap_callee(expr: &Expr) -> bool {
    matches!(
        expr,
        Expr::MethodCall { method, args, .. } if method == "unwrap" && args.is_empty()
    )
}

fn inlinable(
    function: FunctionId,
    facts: &FixupFacts,
    def_path: &[PathSegment],
    arg_path: &[PathSegment],
    arg_use: ArgUse,
) -> bool {
    if matches!(arg_use, ArgUse::FunctionPointerCall { .. }) {
        return is_pure_expr(function, facts, def_path);
    }
    if facts
        .callsite(function, &AstPath(def_path.to_vec()))
        .is_some_and(|callsite| {
            matches!(
                callsite.callee,
                CallCallee::Direct {
                    signature: Some(_),
                    ..
                }
            )
        })
    {
        return true;
    }
    facts
        .call_arg_at(function, &AstPath(arg_path.to_vec()))
        .is_some_and(|(_, arg)| {
            arg.pinning == CallArgPinning::DeclaredParam
                && !arg.variadic
                && is_pure_expr(function, facts, def_path)
        })
}

fn is_pure_temp_let(
    stmt: &Stmt,
    function: FunctionId,
    facts: &FixupFacts,
    path: &[PathSegment],
) -> bool {
    matches!(stmt, Stmt::Let { name, init: Some(_), .. } if is_temp_name(name))
        && facts.effects.iter().any(|fact| {
            fact.site.function == function
                && fact.site.path == AstPath(path.to_vec())
                && fact.subject == EffectSubject::Expr
                && fact.purity == Purity::MovablePure
        })
}

fn is_pure_expr(function: FunctionId, facts: &FixupFacts, path: &[PathSegment]) -> bool {
    facts
        .effect(function, EffectSubject::Expr, &AstPath(path.to_vec()))
        .is_some_and(|fact| fact.purity == Purity::MovablePure)
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
