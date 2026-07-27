use crate::fixups::facts::walk;
use crate::fixups::facts::{
    AstPath, BindingId, BindingKind, EffectKind, EffectSubject, FixupFacts, FunctionId,
    PathSegment, Purity, TempChainFact,
};
use crate::fixups::idents::expr_ident;
use crate::rust_ast::{Expr, IndentStmt, Item, Program, Stmt};

pub(in crate::fixups) fn collect_facts(program: &Program, facts: &mut FixupFacts) {
    facts.temp_chains.clear();
    let mut all = Vec::new();
    for (item_index, item) in program.items.iter().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        collect_body(function, &f.body, facts, &mut Vec::new(), &mut all);
    }
    facts.temp_chains = all;
}

fn collect_body(
    function: FunctionId,
    body: &[IndentStmt],
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
    out: &mut Vec<TempChainFact>,
) {
    for (index, indent) in body.iter().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            if let Some(fact) = temp_chain_at(function, body, index, facts, path) {
                out.push(fact);
            }
            walk::nested_bodies_with_path(&indent.stmt, path, &mut |nested, path| {
                collect_body(function, nested, facts, path, out);
            });
        });
    }
}

fn temp_chain_at(
    function: FunctionId,
    body: &[IndentStmt],
    def_index: usize,
    facts: &FixupFacts,
    def_path: &[PathSegment],
) -> Option<TempChainFact> {
    let Stmt::Let {
        name,
        mutable: false,
        init: Some(init),
        ..
    } = &body[def_index].stmt
    else {
        return None;
    };
    if !is_temp_name(name) {
        return None;
    }
    let is_movable_pure = is_movable_pure_expr(function, facts, def_path);
    let is_effectful = is_effectful_expr(function, facts, def_path);
    let is_atomic_result = is_atomic_result_expr(init, function, facts, def_path);
    let producer_path = AstPath(def_path.to_vec());
    let binding = facts.binding_by_local_path(function, name, &producer_path)?;
    let def_use = facts.def_use(binding)?;
    if !def_use.writes.is_empty() || def_use.reads.len() != 1 {
        return None;
    }
    let producer_prefix = &def_path[..def_path.len().checked_sub(1)?];
    let consumer_path = def_use.reads[0].clone();
    let consumer_index = consumer_stmt_index(producer_prefix, &consumer_path)?;
    if consumer_index <= def_index || consumer_index >= body.len() {
        return None;
    }
    if is_movable_pure {
        if !(def_index + 1..consumer_index).all(|index| {
            is_movable_pure_temp_let(function, facts, &body[index].stmt, producer_prefix, index)
        }) {
            return None;
        }
    } else if !(consumer_index == def_index + 1
        && ((is_effectful && immediate_effectful_consumer(&body[consumer_index].stmt, name))
            || (is_atomic_result
                && immediate_atomic_result_consumer(&body[consumer_index].stmt, name))))
    {
        return None;
    }
    Some(TempChainFact {
        function,
        binding,
        producer_path,
        consumer_path,
        dependencies: temp_dependencies(function, facts, &AstPath(def_path.to_vec()), binding),
    })
}

fn is_movable_pure_temp_let(
    function: FunctionId,
    facts: &FixupFacts,
    stmt: &Stmt,
    parent_path: &[PathSegment],
    index: usize,
) -> bool {
    let Stmt::Let {
        name,
        mutable: false,
        init: Some(_),
        ..
    } = stmt
    else {
        return false;
    };
    if !is_temp_name(name) {
        return false;
    }
    let mut path = parent_path.to_vec();
    path.push(PathSegment::Stmt(index));
    is_movable_pure_expr(function, facts, &path)
}

fn is_movable_pure_expr(function: FunctionId, facts: &FixupFacts, path: &[PathSegment]) -> bool {
    facts
        .effect(function, EffectSubject::Expr, &AstPath(path.to_vec()))
        .is_some_and(|fact| fact.purity == Purity::MovablePure)
}

fn is_effectful_expr(function: FunctionId, facts: &FixupFacts, path: &[PathSegment]) -> bool {
    facts
        .effect(function, EffectSubject::Expr, &AstPath(path.to_vec()))
        .is_some_and(|fact| fact.purity == Purity::Effectful)
}

fn is_atomic_result_expr(
    expr: &Expr,
    function: FunctionId,
    facts: &FixupFacts,
    path: &[PathSegment],
) -> bool {
    if !matches!(
        expr,
        Expr::AtomicLoad { .. }
            | Expr::AtomicFetch { .. }
            | Expr::AtomicSwap { .. }
            | Expr::AtomicCompareExchange { .. }
    ) {
        return false;
    }
    facts
        .effect(function, EffectSubject::Expr, &AstPath(path.to_vec()))
        .is_some_and(|fact| {
            fact.effects.contains(&EffectKind::AtomicRead)
                || fact.effects.contains(&EffectKind::AtomicWrite)
        })
}

fn immediate_effectful_consumer(stmt: &Stmt, name: &str) -> bool {
    match stmt {
        Stmt::Assign { target, value } => {
            expr_ident(target).is_some() && expr_ident(value) == Some(name)
        }
        Stmt::CompoundAssign { target, value, .. } => {
            expr_ident(target).is_some() && expr_ident(value) == Some(name)
        }
        Stmt::Return(Some(expr)) => expr_ident(expr) == Some(name),
        Stmt::Unsafe { body } => {
            body.tail.is_none()
                && body.stmts.len() == 1
                && immediate_effectful_consumer(&body.stmts[0].stmt, name)
        }
        Stmt::Expr(expr) => simple_macro_arg_use(expr, name),
        _ => false,
    }
}

fn immediate_atomic_result_consumer(stmt: &Stmt, name: &str) -> bool {
    match stmt {
        Stmt::Let { init, .. } => init.as_ref().and_then(expr_ident) == Some(name),
        _ => immediate_effectful_consumer(stmt, name),
    }
}

fn simple_macro_arg_use(expr: &Expr, name: &str) -> bool {
    match expr {
        Expr::Macro { args, .. } => {
            args.iter().any(|arg| expr_ident(arg) == Some(name))
                && args
                    .iter()
                    .filter(|arg| expr_ident(arg) != Some(name))
                    .all(is_obviously_pure_expr)
        }
        Expr::Block(block) | Expr::Unsafe(block) => block
            .tail
            .as_deref()
            .is_some_and(|tail| simple_macro_arg_use(tail, name)),
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

fn temp_dependencies(
    function: FunctionId,
    facts: &FixupFacts,
    producer_path: &AstPath,
    binding: BindingId,
) -> Vec<BindingId> {
    facts
        .def_use
        .iter()
        .filter(|def_use| def_use.function == function && def_use.binding != binding)
        .filter(|def_use| def_use.reads.iter().any(|read| read == producer_path))
        .filter_map(|def_use| {
            let binding = facts
                .bindings
                .iter()
                .find(|binding| binding.id == def_use.binding)?;
            if binding.kind == BindingKind::Local && is_temp_name(&binding.name) {
                Some(binding.id)
            } else {
                None
            }
        })
        .collect()
}

fn consumer_stmt_index(parent_path: &[PathSegment], path: &AstPath) -> Option<usize> {
    let rest = path.0.strip_prefix(parent_path)?;
    match rest {
        [PathSegment::Stmt(index)]
        | [PathSegment::Stmt(index), PathSegment::Expr(_), ..]
        | [PathSegment::Stmt(index), PathSegment::UnsafeBody, ..] => Some(*index),
        _ => None,
    }
}

fn is_temp_name(name: &str) -> bool {
    name.strip_prefix("_v")
        .is_some_and(|suffix| !suffix.is_empty() && suffix.chars().all(|c| c.is_ascii_digit()))
}
