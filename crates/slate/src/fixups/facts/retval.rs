use crate::fixups::facts::{
    AstPath, BindingId, ControlFlowSubject, FixupFacts, FunctionId, PathSegment, PlaceAccess,
    PlaceKind, RetvalCollapseFact,
};
use crate::fixups::idents::expr_ident;
use crate::rust_ast::{Expr, IndentStmt, Item, Program, Stmt};

pub(in crate::fixups) fn collect_facts(program: &Program, facts: &mut FixupFacts) {
    facts.retval_collapses.clear();
    let mut all = Vec::new();
    for (item_index, item) in program.items.iter().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        collect_body(function, &f.body, facts, &mut all);
    }
    facts.retval_collapses = all;
}

fn collect_body(
    function: FunctionId,
    body: &[IndentStmt],
    facts: &FixupFacts,
    out: &mut Vec<RetvalCollapseFact>,
) {
    for ret_index in 1..body.len() {
        let ret_path = stmt_path(ret_index);
        if !reachable_stmt(function, facts, &ret_path) {
            continue;
        }
        let Stmt::Return(Some(Expr::Var(name))) = &body[ret_index].stmt else {
            continue;
        };
        let Some(binding) = facts
            .bindings
            .iter()
            .find(|binding| binding.function == function && binding.name == name.as_str())
            .map(|binding| binding.id)
        else {
            continue;
        };
        if let Some(fact) = initialized_decl_return(function, body, ret_index, binding, facts) {
            out.push(fact);
        } else if let Some(fact) = assigned_slot_return(function, body, ret_index, binding, facts) {
            out.push(fact);
        }
    }
}

fn initialized_decl_return(
    function: FunctionId,
    body: &[IndentStmt],
    ret_index: usize,
    binding: BindingId,
    facts: &FixupFacts,
) -> Option<RetvalCollapseFact> {
    let decl_index = ret_index.checked_sub(1)?;
    let Stmt::Let {
        name,
        init: Some(_),
        ..
    } = &body[decl_index].stmt
    else {
        return None;
    };
    if facts.binding_name(binding) != Some(name.as_str()) {
        return None;
    }
    let def_use = facts.def_use(binding)?;
    let decl_path = AstPath(stmt_path(decl_index));
    let ret_path = AstPath(stmt_path(ret_index));
    if def_use.definition != decl_path
        || !def_use.writes.is_empty()
        || def_use.reads != [ret_path.clone()]
    {
        return None;
    }
    Some(RetvalCollapseFact {
        function,
        return_path: ret_path,
        value_path: decl_path.clone(),
        remove_paths: vec![decl_path],
    })
}

fn assigned_slot_return(
    function: FunctionId,
    body: &[IndentStmt],
    ret_index: usize,
    binding: BindingId,
    facts: &FixupFacts,
) -> Option<RetvalCollapseFact> {
    let store_index = ret_index.checked_sub(1)?;
    let Stmt::Assign { target, .. } = &body[store_index].stmt else {
        return None;
    };
    if !store_writes_binding(function, facts, store_index, binding, target) {
        return None;
    }
    let def_use = facts.def_use(binding)?;
    let ret_path = AstPath(stmt_path(ret_index));
    let store_path = AstPath(stmt_path(store_index));
    if def_use.definition.0.len() != 1
        || def_use.reads != [ret_path.clone()]
        || def_use.writes != [store_path.clone()]
    {
        return None;
    }
    let [PathSegment::Stmt(decl_index)] = def_use.definition.0.as_slice() else {
        return None;
    };
    Some(RetvalCollapseFact {
        function,
        return_path: ret_path,
        value_path: store_path.clone(),
        remove_paths: vec![store_path, AstPath(stmt_path(*decl_index))],
    })
}

fn store_writes_binding(
    function: FunctionId,
    facts: &FixupFacts,
    index: usize,
    binding: BindingId,
    target: &Expr,
) -> bool {
    let Some(name) = facts.binding_name(binding) else {
        return false;
    };
    if expr_ident(target) != Some(name) {
        return false;
    }
    facts
        .place(function, &AstPath(stmt_path(index)))
        .is_some_and(|fact| {
            fact.access == PlaceAccess::Write
                && fact.ordinary_slot
                && matches!(&fact.kind, PlaceKind::Local { name: place } if place == name)
        })
}

fn reachable_stmt(function: FunctionId, facts: &FixupFacts, path: &[PathSegment]) -> bool {
    facts
        .control_flow(function, ControlFlowSubject::Stmt, &AstPath(path.to_vec()))
        .is_some_and(|fact| fact.reachable)
}

fn stmt_path(index: usize) -> Vec<PathSegment> {
    vec![PathSegment::Stmt(index)]
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{BinOp, Item, Program};

    fn analyzed(stmts: Vec<Stmt>) -> FixupFacts {
        let program = Program {
            items: vec![Item::Fn(func(vec![], Some("i32"), stmts))],
        };
        crate::fixups::facts::analyze(program).facts
    }

    #[test]
    fn records_declaration_initialized_retval_collapse() {
        let facts = analyzed(vec![
            let_mut("__retval", "i32", bin(BinOp::BitAnd, var("a"), var("b"))),
            Stmt::Return(Some(var("__retval"))),
        ]);

        assert_eq!(facts.retval_collapses.len(), 1);
        let fact = &facts.retval_collapses[0];
        assert_eq!(fact.return_path, AstPath(vec![PathSegment::Stmt(1)]));
        assert_eq!(fact.value_path, AstPath(vec![PathSegment::Stmt(0)]));
        assert_eq!(fact.remove_paths, vec![AstPath(vec![PathSegment::Stmt(0)])]);
    }

    #[test]
    fn rejects_declaration_initialized_retval_read_before_return() {
        let facts = analyzed(vec![
            let_mut("__retval", "i32", bin(BinOp::BitAnd, var("a"), var("b"))),
            let_mut("x", "i32", var("__retval")),
            Stmt::Return(Some(var("__retval"))),
        ]);

        assert!(facts.retval_collapses.is_empty());
    }
}
