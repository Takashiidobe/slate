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

#[cfg(test)]
mod tests {
    use crate::fixups::facts::{self, AstPath, BindingId, FunctionId, PathSegment, TempChainFact};
    use crate::fixups::test_support::*;
    use crate::rust_ast::{
        AtomicOrdering, AtomicPlace, AtomicRmwOp, AtomicType, BinOp, Expr, Item, Program, Stmt,
        UnaryOp,
    };

    fn analyzed(stmts: Vec<Stmt>) -> facts::FixupFacts {
        facts::analyze(Program {
            items: vec![Item::Fn(func(
                vec![param("op", "i32"), param("value", "i32")],
                Some("i32"),
                stmts,
            ))],
        })
        .facts
    }

    fn atomic_load() -> Expr {
        Expr::AtomicLoad {
            ty: AtomicType::I32,
            place: AtomicPlace::Local("a".into()),
            ordering: AtomicOrdering::SeqCst,
        }
    }

    fn atomic_fetch() -> Expr {
        Expr::AtomicFetch {
            ty: AtomicType::I32,
            op: AtomicRmwOp::Add,
            place: AtomicPlace::Local("a".into()),
            value: Box::new(int(1)),
            ordering: AtomicOrdering::SeqCst,
        }
    }

    fn binding_for(facts: &facts::FixupFacts, name: &str, path: AstPath) -> BindingId {
        facts
            .binding_by_local_path(FunctionId(0), name, &path)
            .unwrap()
    }

    fn chain_for<'a>(facts: &'a facts::FixupFacts, name: &str) -> &'a TempChainFact {
        let binding = facts
            .bindings
            .iter()
            .find(|binding| binding.function == FunctionId(0) && binding.name == name)
            .unwrap()
            .id;
        facts
            .temp_chains
            .iter()
            .find(|fact| fact.binding == binding)
            .unwrap()
    }

    #[test]
    fn records_pure_single_use_temp_chains() {
        let facts = analyzed(vec![
            temp("_v1", "i32", var("op")),
            temp("_v2", "i32", var("value")),
            temp("_v3", "i32", bin(BinOp::Add, var("_v1"), var("_v2"))),
            Stmt::Return(Some(var("_v3"))),
        ]);

        let v1 = chain_for(&facts, "_v1");
        assert_eq!(v1.producer_path, AstPath(vec![PathSegment::Stmt(0)]));
        assert_eq!(v1.consumer_path, AstPath(vec![PathSegment::Stmt(2)]));
        assert!(v1.dependencies.is_empty());

        let v2 = chain_for(&facts, "_v2");
        assert_eq!(v2.producer_path, AstPath(vec![PathSegment::Stmt(1)]));
        assert_eq!(v2.consumer_path, AstPath(vec![PathSegment::Stmt(2)]));
        assert!(v2.dependencies.is_empty());

        let v3 = chain_for(&facts, "_v3");
        assert_eq!(v3.producer_path, AstPath(vec![PathSegment::Stmt(2)]));
        assert_eq!(v3.consumer_path, AstPath(vec![PathSegment::Stmt(3)]));
        assert_eq!(
            v3.dependencies,
            vec![
                binding_for(&facts, "_v1", AstPath(vec![PathSegment::Stmt(0)])),
                binding_for(&facts, "_v2", AstPath(vec![PathSegment::Stmt(1)])),
            ]
        );
    }

    #[test]
    fn rejects_temp_chains_crossing_unknown_call_barriers() {
        let facts = analyzed(vec![
            temp("_v1", "i32", var("op")),
            Stmt::Expr(call("mystery", vec![])),
            temp("_v2", "i32", bin(BinOp::Add, var("_v1"), int(1))),
        ]);

        assert!(
            !facts
                .temp_chains
                .iter()
                .any(|fact| { facts.binding_name(fact.binding) == Some("_v1") })
        );
    }

    #[test]
    fn rejects_temp_chains_crossing_memory_write_barriers() {
        let facts = analyzed(vec![
            temp("_v1", "i32", var("value")),
            assign("value", int(1)),
            temp("_v2", "i32", bin(BinOp::Add, var("_v1"), int(1))),
        ]);

        assert!(
            !facts
                .temp_chains
                .iter()
                .any(|fact| { facts.binding_name(fact.binding) == Some("_v1") })
        );
    }

    #[test]
    fn records_chains_with_nested_expression_reads() {
        let facts = analyzed(vec![
            temp("_v1", "*mut i32", var("slot")),
            Stmt::Assign {
                target: Expr::Unary {
                    op: UnaryOp::Deref,
                    expr: Box::new(var("_v1")),
                },
                value: int(5),
            },
        ]);

        let v1 = chain_for(&facts, "_v1");
        assert_eq!(v1.producer_path, AstPath(vec![PathSegment::Stmt(0)]));
        assert_eq!(v1.consumer_path, AstPath(vec![PathSegment::Stmt(1)]));
    }

    #[test]
    fn records_chains_into_unsafe_wrapper_statements() {
        let facts = analyzed(vec![
            temp("_v1", "*mut i32", var("slot")),
            Stmt::Unsafe {
                body: crate::rust_ast::Block {
                    stmts: vec![crate::rust_ast::IndentStmt {
                        depth: 2,
                        stmt: Stmt::Assign {
                            target: Expr::Unary {
                                op: UnaryOp::Deref,
                                expr: Box::new(var("_v1")),
                            },
                            value: int(5),
                        },
                    }],
                    tail: None,
                },
            },
        ]);

        let v1 = chain_for(&facts, "_v1");
        assert_eq!(v1.producer_path, AstPath(vec![PathSegment::Stmt(0)]));
        assert_eq!(
            v1.consumer_path,
            AstPath(vec![
                PathSegment::Stmt(1),
                PathSegment::UnsafeBody,
                PathSegment::Stmt(0),
            ])
        );
    }

    #[test]
    fn records_immediate_effectful_return_slot_temp() {
        let facts = analyzed(vec![
            let_mut("__retval", "i32", int(0)),
            temp("_v1", "i32", call("op", vec![var("value")])),
            assign("__retval", var("_v1")),
            Stmt::Return(Some(var("__retval"))),
        ]);

        let v1 = chain_for(&facts, "_v1");
        assert_eq!(v1.producer_path, AstPath(vec![PathSegment::Stmt(1)]));
        assert_eq!(v1.consumer_path, AstPath(vec![PathSegment::Stmt(2)]));
    }

    #[test]
    fn records_immediate_effectful_return_temp() {
        let facts = analyzed(vec![
            temp("_v1", "i32", call("op", vec![var("value")])),
            Stmt::Return(Some(var("_v1"))),
        ]);

        let v1 = chain_for(&facts, "_v1");
        assert_eq!(v1.producer_path, AstPath(vec![PathSegment::Stmt(0)]));
        assert_eq!(v1.consumer_path, AstPath(vec![PathSegment::Stmt(1)]));
    }

    #[test]
    fn rejects_non_adjacent_effectful_temp() {
        let facts = analyzed(vec![
            temp("_v1", "i32", call("op", vec![var("value")])),
            temp("_v2", "i32", int(1)),
            assign("__retval", var("_v1")),
        ]);

        assert!(
            !facts
                .temp_chains
                .iter()
                .any(|fact| { facts.binding_name(fact.binding) == Some("_v1") })
        );
    }

    #[test]
    fn records_immediate_effectful_local_assignment_temp() {
        let facts = analyzed(vec![
            temp("_v1", "i32", call("op", vec![var("value")])),
            assign("value", var("_v1")),
        ]);

        let v1 = chain_for(&facts, "_v1");
        assert_eq!(v1.producer_path, AstPath(vec![PathSegment::Stmt(0)]));
        assert_eq!(v1.consumer_path, AstPath(vec![PathSegment::Stmt(1)]));
    }

    #[test]
    fn records_immediate_atomic_result_assignment_temp() {
        let facts = analyzed(vec![
            temp("_v1", "i32", atomic_load()),
            assign("value", var("_v1")),
            temp("_v2", "i32", atomic_fetch()),
            assign("op", var("_v2")),
        ]);

        let v1 = chain_for(&facts, "_v1");
        assert_eq!(v1.producer_path, AstPath(vec![PathSegment::Stmt(0)]));
        assert_eq!(v1.consumer_path, AstPath(vec![PathSegment::Stmt(1)]));
        let v2 = chain_for(&facts, "_v2");
        assert_eq!(v2.producer_path, AstPath(vec![PathSegment::Stmt(2)]));
        assert_eq!(v2.consumer_path, AstPath(vec![PathSegment::Stmt(3)]));
    }

    #[test]
    fn rejects_non_adjacent_atomic_result_temp() {
        let facts = analyzed(vec![
            temp("_v1", "i32", atomic_load()),
            assign("op", int(1)),
            assign("value", var("_v1")),
        ]);

        assert!(
            !facts
                .temp_chains
                .iter()
                .any(|fact| { facts.binding_name(fact.binding) == Some("_v1") })
        );
    }
}
