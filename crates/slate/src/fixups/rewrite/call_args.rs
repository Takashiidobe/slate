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

use crate::fixups::facts::{
    AstPath, CallArgPinning, CallCallee, EffectSubject, FixupFacts, FunctionId, PathSegment, Purity,
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
    CallArgs::new(&mut logger).fixup(body, function, facts)
}

pub(in crate::fixups) struct CallArgs<'a> {
    logger: &'a mut dyn TraceLogger,
}

impl<'a> CallArgs<'a> {
    pub(in crate::fixups) fn new(logger: &'a mut dyn TraceLogger) -> Self {
        Self { logger }
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

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{
        ExternDecl, ExternFnDecl, FnParam, Item, MatchArm, Pattern, Program, Stmt, Type,
    };

    struct Signature {
        params: Vec<Type>,
        variadic: bool,
    }

    fn sig(params: &[&str], variadic: bool) -> Signature {
        Signature {
            params: params.iter().map(|t| Type::parse(t)).collect(),
            variadic,
        }
    }

    fn run(sigs: Vec<(&str, Signature)>, stmts: Vec<Stmt>) -> String {
        let mut items: Vec<Item> = sigs
            .into_iter()
            .map(|(name, sig)| Item::ExternBlock {
                abi: "C".into(),
                decls: vec![ExternDecl::Fn(ExternFnDecl {
                    identity: crate::function_identity::FunctionIdentity::Unknown,
                    name: name.into(),
                    params: sig
                        .params
                        .into_iter()
                        .enumerate()
                        .map(|(index, ty)| FnParam {
                            name: format!("arg{index}"),
                            mutable: false,
                            ty,
                            nonnull: false,
                        })
                        .collect(),
                    variadic: sig.variadic,
                    ret: Some(Type::parse("i32")),
                    returns_nonnull: false,
                })],
            })
            .collect();
        items.push(Item::Fn(func(vec![], None, stmts)));
        let mut program = Program { items };
        let item_index = program.items.len() - 1;
        loop {
            let analyzed = crate::fixups::facts::analyze(program.clone());
            let facts = analyzed.facts;
            let Item::Fn(f) = &mut program.items[item_index] else {
                unreachable!();
            };
            if !fixup(&mut f.body, FunctionId(0), &facts) {
                break;
            }
        }
        let Item::Fn(f) = &program.items[item_index] else {
            unreachable!();
        };
        emit(f.clone())
    }

    #[test]
    fn inlines_literals_into_params_and_call_result_into_vararg() {
        let out = run(
            vec![
                ("add", sig(&["i32", "i32"], false)),
                ("printf", sig(&["*mut i8"], true)),
            ],
            vec![
                temp("_v1", "i32", int(2)),
                temp("_v2", "i32", int(3)),
                temp("_v3", "i32", call("add", vec![var("_v1"), var("_v2")])),
                Stmt::Expr(call("printf", vec![var("fmt"), var("_v3")])),
            ],
        );

        assert_eq!(
            out,
            "\
fn f() {
    printf(fmt, add(2, 3));
}
"
        );
    }

    #[test]
    fn does_not_inline_bare_literal_into_vararg_slot() {
        let out = run(
            vec![("printf", sig(&["*mut i8"], true))],
            vec![
                temp("_v0", "i64", int(9223372036854775807)),
                Stmt::Expr(call("printf", vec![var("fmt"), var("_v0")])),
            ],
        );

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i64 = 9223372036854775807;
    printf(fmt, _v0);
}
"
        );
    }

    #[test]
    fn does_not_inline_into_unknown_callee() {
        let out = run(
            vec![],
            vec![
                temp("_v0", "i32", int(5)),
                Stmt::Expr(call("mystery", vec![var("_v0")])),
            ],
        );

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i32 = 5;
    mystery(_v0);
}
"
        );
    }

    #[test]
    fn inlines_pure_args_into_function_pointer_call() {
        let out = run(
            vec![],
            vec![
                temp("_v0", "i32", var("value")),
                Stmt::Return(Some(Expr::Call {
                    binding: crate::function_identity::CallBinding::Generated,
                    func: Box::new(Expr::MethodCall {
                        recv: Box::new(var("op")),
                        method: "unwrap".into(),
                        args: vec![],
                    }),
                    args: vec![var("_v0")],
                })),
            ],
        );

        assert_eq!(
            out,
            "\
fn f() {
    return op.unwrap()(value);
}
"
        );
    }

    #[test]
    fn inlines_call_args_inside_match_arm() {
        let out = run(
            vec![("add", sig(&["i32", "i32"], false))],
            vec![Stmt::Match {
                expr: var("state"),
                arms: vec![MatchArm {
                    pattern: Pattern::I64(0),
                    body: vec![
                        IndentStmt {
                            depth: 0,
                            stmt: temp("_v1", "i32", int(2)),
                        },
                        IndentStmt {
                            depth: 0,
                            stmt: temp("_v2", "i32", int(3)),
                        },
                        IndentStmt {
                            depth: 0,
                            stmt: Stmt::Expr(call("add", vec![var("_v1"), var("_v2")])),
                        },
                    ],
                }],
            }],
        );

        assert_eq!(
            out,
            "\
fn f() {
    match state {
        0 => {
            add(2, 3);
        }
    }
}
"
        );
    }

    #[test]
    fn does_not_reorder_call_result_across_side_effect() {
        let out = run(
            vec![("g", sig(&[], false)), ("printf", sig(&["*mut i8"], true))],
            vec![
                temp("_v0", "i32", call("g", vec![])),
                Stmt::Expr(call("side_effect", vec![])),
                Stmt::Expr(call("printf", vec![var("fmt"), var("_v0")])),
            ],
        );

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i32 = g();
    side_effect();
    printf(fmt, _v0);
}
"
        );
    }
}
