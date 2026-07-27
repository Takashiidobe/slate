use crate::fixups::facts::walk;
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
        collect_body(function, &f.body, facts, &mut Vec::new(), &mut all);
    }
    facts.retval_collapses = all;
}

fn collect_body(
    function: FunctionId,
    body: &[IndentStmt],
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
    out: &mut Vec<RetvalCollapseFact>,
) {
    for (index, indent) in body.iter().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            walk::nested_bodies_with_path(&indent.stmt, path, &mut |nested, path| {
                collect_body(function, nested, facts, path, out);
            });
        });
    }
    for ret_index in 1..body.len() {
        let ret_path = stmt_path(path, ret_index);
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
        if let Some(fact) = initialized_decl_return(function, body, ret_index, binding, facts, path)
        {
            out.push(fact);
        } else if let Some(fact) =
            assigned_slot_return(function, body, ret_index, binding, facts, path)
        {
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
    parent_path: &[PathSegment],
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
    let decl_path = AstPath(stmt_path(parent_path, decl_index));
    let ret_path = AstPath(stmt_path(parent_path, ret_index));
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
    parent_path: &[PathSegment],
) -> Option<RetvalCollapseFact> {
    let store_index = ret_index.checked_sub(1)?;
    let Stmt::Assign { target, .. } = &body[store_index].stmt else {
        return None;
    };
    let store_path = AstPath(stmt_path(parent_path, store_index));
    if !store_writes_binding(function, facts, &store_path, binding, target) {
        return None;
    }
    let ret_path = AstPath(stmt_path(parent_path, ret_index));
    Some(RetvalCollapseFact {
        function,
        return_path: ret_path,
        value_path: store_path.clone(),
        remove_paths: vec![store_path],
    })
}

fn store_writes_binding(
    function: FunctionId,
    facts: &FixupFacts,
    path: &AstPath,
    binding: BindingId,
    target: &Expr,
) -> bool {
    let Some(name) = facts.binding_name(binding) else {
        return false;
    };
    if expr_ident(target) != Some(name) {
        return false;
    }
    facts.place(function, path).is_some_and(|fact| {
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

fn stmt_path(parent_path: &[PathSegment], index: usize) -> Vec<PathSegment> {
    let mut path = parent_path.to_vec();
    path.push(PathSegment::Stmt(index));
    path
}
