//! Inline single-use pure temps directly on the statement list. The temp's init
//! is spliced as an `Expr` subtree into its use site and precedence-aware
//! rendering elides redundant parens.

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

pub(in crate::fixups) fn fixup(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
    phase: Phase,
) -> bool {
    let mut logger = crate::fixups::trace::NoopLogger;
    InlineTemps::new(phase, &mut logger).fixup(body, function, facts)
}

pub(in crate::fixups) struct InlineTemps<'a> {
    phase: Phase,
    logger: &'a mut dyn TraceLogger,
}

impl<'a> InlineTemps<'a> {
    pub(in crate::fixups) fn new(phase: Phase, logger: &'a mut dyn TraceLogger) -> Self {
        Self { phase, logger }
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
        Expr::Binary { .. } => ty.is_some() && !contains_integer_literal(init),
        Expr::Index { .. } => ty.is_some(),
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

fn call_or_macro_arg_use_expr(expr: &Expr, name: &str) -> bool {
    match expr {
        Expr::Call { args, .. } | Expr::Macro { args, .. } => args
            .iter()
            .any(|arg| matches!(arg, Expr::Var(var) if var.as_str() == name)),
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

fn simple_macro_arg_use(stmt: &Stmt, name: &str) -> bool {
    match stmt {
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => simple_macro_arg_use_expr(expr, name),
        _ => false,
    }
}

fn simple_macro_arg_use_expr(expr: &Expr, name: &str) -> bool {
    match expr {
        Expr::Macro { args, .. } => {
            args.iter()
                .any(|arg| matches!(arg, Expr::Var(var) if var.as_str() == name))
                && args.iter().all(|arg| {
                    matches!(arg, Expr::Var(var) if var.as_str() == name)
                        || is_obviously_pure_expr(arg)
                })
        }
        Expr::Block(block) | Expr::Unsafe(block) => block
            .tail
            .as_deref()
            .is_some_and(|tail| simple_macro_arg_use_expr(tail, name)),
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

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::fixups::trace::{CollectingLogger, Pass, ProgramSummary, TraceLogger};
    use crate::rust_ast::{
        AtomicOrdering, AtomicPlace, AtomicType, BinOp, Block, Item, MatchArm, Pattern, Program,
        Type,
    };

    fn inlined(stmts: Vec<Stmt>) -> String {
        inlined_with_phase(stmts, Phase::Late)
    }

    fn early_inlined(stmts: Vec<Stmt>) -> String {
        inlined_with_phase(stmts, Phase::Early)
    }

    fn inlined_with_phase(stmts: Vec<Stmt>, phase: Phase) -> String {
        let mut program = Program {
            items: vec![Item::Fn(func(vec![], None, stmts))],
        };
        loop {
            let analyzed = crate::fixups::facts::analyze(program.clone());
            let facts = analyzed.facts;
            let Item::Fn(f) = &mut program.items[0] else {
                unreachable!();
            };
            if !fixup(&mut f.body, FunctionId(0), &facts, phase) {
                break;
            }
        }
        program.emit()
    }

    #[test]
    fn inlines_single_use_scalar_temps() {
        let out = inlined(vec![
            let_mut("a", "i32", int(0)),
            temp("_v0", "i32", int(20)),
            assign("a", var("_v0")),
            temp("_v1", "i32", int(5)),
            temp("_v2", "i32", var("a")),
            temp("_v3", "i32", bin(BinOp::Sub, var("_v2"), var("_v1"))),
            assign("a", var("_v3")),
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let mut a: i32 = 0;
    a = 20;
    a = a - 5;
}
"
        );
    }

    #[test]
    fn logs_temp_inline_rewrite() {
        let mut program = Program {
            items: vec![Item::Fn(func(
                vec![],
                None,
                vec![
                    let_mut("a", "i32", int(0)),
                    temp("_v0", "i32", int(20)),
                    assign("a", var("_v0")),
                ],
            ))],
        };
        let analyzed = crate::fixups::facts::analyze(program.clone());
        let mut logger = CollectingLogger::default();
        logger.begin_pass(
            Pass::LateInlineTemps,
            ProgramSummary::from_program(&program),
            program.emit(),
        );
        let Item::Fn(f) = &mut program.items[0] else {
            unreachable!();
        };
        assert!(InlineTemps::new(Phase::Late, &mut logger).fixup(
            &mut f.body,
            FunctionId(0),
            &analyzed.facts
        ));
        logger.end_pass(ProgramSummary::from_program(&program), program.emit());
        let log = logger.finish(ProgramSummary::from_program(&program));
        let event = &log.passes[0].events[0];

        assert_eq!(event.kind, "inline_temp");
        assert_eq!(event.before[0].code, "let _v0: i32 = 20;");
        assert_eq!(event.before[1].code, "a = _v0;");
        assert_eq!(event.after[0].code, "a = 20;");
        assert!(
            event
                .facts
                .iter()
                .any(|fact| fact.key == "phase" && fact.value == "late")
        );
    }

    #[test]
    fn inlines_temp_chains_from_facts() {
        let out = inlined(vec![
            temp("_v1", "i32", var("op")),
            temp("_v2", "i32", var("value")),
            temp("_v3", "i32", bin(BinOp::Add, var("_v1"), var("_v2"))),
            Stmt::Return(Some(var("_v3"))),
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    return op + value;
}
"
        );
    }

    #[test]
    fn inlines_member_access_temps_into_arithmetic() {
        let left = Expr::Field {
            base: Box::new(var("p")),
            field: "left".into(),
        };
        let right = Expr::Field {
            base: Box::new(var("p")),
            field: "right".into(),
        };
        let out = inlined(vec![
            temp("_v0", "i32", left),
            temp("_v1", "i32", bin(BinOp::Mul, var("_v0"), int(10))),
            temp("_v2", "i32", right),
            Stmt::Return(Some(bin(BinOp::Add, var("_v1"), var("_v2")))),
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    return p.left * 10 + p.right;
}
"
        );
    }

    #[test]
    fn keeps_temp_that_feeds_a_branch_value() {
        // a temp used once in the branch body (the store) and once as the branch's
        // yielded value must not be inlined away, or the tail reference dangles.
        let out = inlined(vec![Stmt::LetIf {
            name: "_v0".into(),
            mutable: false,
            ty: Some(Type::parse("i32")),
            cond: var("c"),
            then_body: vec![
                IndentStmt {
                    depth: 2,
                    stmt: temp("_v1", "i32", bin(BinOp::Add, var("a"), int(1))),
                },
                IndentStmt {
                    depth: 2,
                    stmt: assign("a", var("_v1")),
                },
            ],
            then_value: var("_v1"),
            else_body: vec![],
            else_value: int(0),
        }]);

        assert!(
            out.contains("let _v1: i32 = a + 1;"),
            "binding must survive, got:\n{out}"
        );
        assert_eq!(
            out.matches("_v1").count(),
            3,
            "def + store use + branch value"
        );
    }

    #[test]
    fn does_not_inline_call_results() {
        let out = inlined(vec![
            temp("_v0", "i32", call("g", vec![])),
            temp("_v1", "i32", var("_v0")),
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i32 = g();
    let _v1: i32 = _v0;
}
"
        );
    }

    #[test]
    fn inlines_immediate_effectful_temp_into_return_slot() {
        let out = inlined(vec![
            let_mut("__retval", "i32", int(0)),
            temp("_v1", "i32", call("op", vec![var("value")])),
            assign("__retval", var("_v1")),
            Stmt::Return(Some(var("__retval"))),
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let mut __retval: i32 = 0;
    __retval = op(value);
    return __retval;
}
"
        );
    }

    #[test]
    fn inlines_immediate_effectful_temp_into_return() {
        let out = inlined(vec![
            temp("_v1", "i32", call("op", vec![var("value")])),
            Stmt::Return(Some(var("_v1"))),
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    return op(value);
}
"
        );
    }

    #[test]
    fn inlines_immediate_effectful_temp_into_local_assignment() {
        let out = inlined(vec![
            let_mut("first", "i32", int(0)),
            temp("_v0", "i32", call("next_arg", vec![])),
            assign("first", var("_v0")),
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let mut first: i32 = 0;
    first = next_arg();
}
"
        );
    }

    #[test]
    fn early_pass_keeps_effectful_temp_into_local_assignment() {
        let out = early_inlined(vec![
            let_mut("first", "i32", int(0)),
            temp("_v0", "i32", call("next_arg", vec![])),
            assign("first", var("_v0")),
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let mut first: i32 = 0;
    let _v0: i32 = next_arg();
    first = _v0;
}
"
        );
    }

    #[test]
    fn inlines_immediate_effectful_temp_into_local_compound_assignment() {
        let out = inlined(vec![
            let_mut("total", "i32", int(0)),
            temp("_v0", "i32", call("next_arg", vec![])),
            Stmt::CompoundAssign {
                target: var("total"),
                op: BinOp::Add,
                value: var("_v0"),
            },
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let mut total: i32 = 0;
    total += next_arg();
}
"
        );
    }

    #[test]
    fn inlines_temps_inside_for_body() {
        let out = inlined(vec![Stmt::For {
            pat: "_".into(),
            iter: var("items"),
            body: vec![
                IndentStmt {
                    depth: 0,
                    stmt: temp("_v0", "i32", call("next_arg", vec![])),
                },
                IndentStmt {
                    depth: 0,
                    stmt: Stmt::CompoundAssign {
                        target: var("total"),
                        op: BinOp::Add,
                        value: var("_v0"),
                    },
                },
            ],
        }]);

        assert_eq!(
            out,
            "\
fn f() {
    for _ in items {
        total += next_arg();
    }
}
"
        );
    }

    #[test]
    fn inlines_temps_inside_match_arms() {
        let out = inlined(vec![Stmt::Match {
            expr: var("state"),
            arms: vec![MatchArm {
                pattern: Pattern::I64(0),
                body: vec![
                    IndentStmt {
                        depth: 0,
                        stmt: temp("_v0", "i32", var("sum")),
                    },
                    IndentStmt {
                        depth: 0,
                        stmt: temp("_v1", "i32", var("i")),
                    },
                    IndentStmt {
                        depth: 0,
                        stmt: temp("_v2", "i32", bin(BinOp::Add, var("_v0"), var("_v1"))),
                    },
                    IndentStmt {
                        depth: 0,
                        stmt: assign("sum", var("_v2")),
                    },
                ],
            }],
        }]);

        assert_eq!(
            out,
            "\
fn f() {
    match state {
        0 => {
            sum = sum + i;
        }
    }
}
"
        );
    }

    #[test]
    fn inlines_i32_literal_temps_into_unsafe_call_arguments() {
        let out = inlined(vec![
            temp("_v1", "i32", int(4)),
            temp("_v2", "i32", int(10)),
            Stmt::Expr(Expr::Unsafe(Box::new(Block {
                stmts: vec![],
                tail: Some(Box::new(call("sum", vec![var("_v1"), var("_v2")]))),
            }))),
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    unsafe { sum(4, 10) };
}
"
        );
    }

    #[test]
    fn early_pass_keeps_call_argument_temps() {
        let out = early_inlined(vec![
            temp("_v1", "i32", int(4)),
            temp("_v2", "i32", int(10)),
            Stmt::Expr(Expr::Unsafe(Box::new(Block {
                stmts: vec![],
                tail: Some(Box::new(call("sum", vec![var("_v1"), var("_v2")]))),
            }))),
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let _v1: i32 = 4;
    let _v2: i32 = 10;
    unsafe { sum(_v1, _v2) };
}
"
        );
    }

    #[test]
    fn inlines_immediate_effectful_temp_into_simple_macro_argument() {
        let out = inlined(vec![
            temp("_v0", "i32", call("sum", vec![])),
            Stmt::Expr(Expr::Macro {
                name: "println".into(),
                args: vec![Expr::Str("{}".into()), var("_v0")],
            }),
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    println!(\"{}\", sum());
}
"
        );
    }

    #[test]
    fn does_not_inline_non_adjacent_effectful_temp() {
        let out = inlined(vec![
            temp("_v1", "i32", call("op", vec![var("value")])),
            temp("_v2", "i32", int(1)),
            assign("__retval", var("_v1")),
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let _v1: i32 = op(value);
    let _v2: i32 = 1;
    __retval = _v1;
}
"
        );
    }

    #[test]
    fn does_not_cross_side_effecting_statement() {
        let out = inlined(vec![
            temp("_v0", "i32", var("a")),
            Stmt::Expr(call("printf", vec![var("_v1")])),
            assign("b", var("_v0")),
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i32 = a;
    printf(_v1);
    b = _v0;
}
"
        );
    }

    #[test]
    fn does_not_inline_impure_intrinsics() {
        let out = inlined(vec![
            temp(
                "_v0",
                "i32",
                call("std::ptr::read_volatile", vec![var("p")]),
            ),
            assign("b", var("_v0")),
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i32 = std::ptr::read_volatile(p);
    b = _v0;
}
"
        );
    }

    #[test]
    fn inlines_adjacent_atomic_result_temp_into_assignment() {
        let out = inlined(vec![
            temp(
                "_v0",
                "i32",
                Expr::AtomicLoad {
                    ty: AtomicType::I32,
                    place: AtomicPlace::Local("a".into()),
                    ordering: AtomicOrdering::SeqCst,
                },
            ),
            assign("loaded", var("_v0")),
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    loaded = a.load(std::sync::atomic::Ordering::SeqCst);
}
"
        );
    }

    #[test]
    fn does_not_inline_method_receivers_that_need_type_annotations() {
        let out = inlined(vec![
            temp("_v0", "i32", int(2147483647)),
            temp("_v1", "i32", int(1)),
            Stmt::Let {
                name: "_v2".into(),
                mutable: false,
                ty: None,
                init: Some(Expr::MethodCall {
                    recv: Box::new(var("_v0")),
                    method: "overflowing_add".into(),
                    args: vec![var("_v1")],
                }),
            },
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i32 = 2147483647;
    let _v1: i32 = 1;
    let _v2 = _v0.overflowing_add(_v1);
}
"
        );
    }

    #[test]
    fn inlines_option_presence_check_receivers() {
        let out = inlined(vec![
            temp("_v0", "Option<fn(i32) -> i32>", var("op")),
            Stmt::If {
                cond: Expr::MethodCall {
                    recv: Box::new(var("_v0")),
                    method: "is_some".into(),
                    args: vec![],
                },
                then_body: vec![IndentStmt {
                    depth: 0,
                    stmt: Stmt::Return(Some(var("value"))),
                }],
                else_body: vec![],
            },
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    if op.is_some() {
        return value;
    }
}
"
        );
    }

    #[test]
    fn does_not_inline_call_arguments_that_need_type_annotations() {
        let out = inlined(vec![
            temp("_v0", "i64", int(9223372036854775807)),
            Stmt::Let {
                name: "_v1".into(),
                mutable: false,
                ty: Some(Type::parse("i32")),
                init: Some(call("printf", vec![var("_v0")])),
            },
        ]);

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i64 = 9223372036854775807;
    let _v1: i32 = printf(_v0);
}
"
        );
    }
}
