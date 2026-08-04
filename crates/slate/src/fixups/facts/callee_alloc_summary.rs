use crate::fixups::facts::heap_ownership::allocation_temp;
use crate::fixups::facts::walk;
use crate::fixups::facts::{AstPath, CalleeAllocSummaryFact, FixupFacts, FunctionId, PathSegment};
use crate::rust_ast::{Expr, FnDef, IndentStmt, Item, Program, Stmt, Type};

pub(in crate::fixups) fn collect_facts(program: &Program, facts: &mut FixupFacts) {
    facts.callee_alloc_summaries.clear();
    let mut all = Vec::new();
    for (item_index, item) in program.items.iter().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        if let Some(summary) = summarize_function(function, f) {
            all.push(summary);
        }
    }
    facts.callee_alloc_summaries = all;
}

fn summarize_function(function: FunctionId, f: &FnDef) -> Option<CalleeAllocSummaryFact> {
    let Some(Type::Ptr { inner: elem_ty, .. }) = &f.ret else {
        return None;
    };
    if count_returns(&f.body) != 1 {
        return None;
    }
    let (return_index, return_expr) = top_level_return(&f.body)?;
    let Expr::Var(temp_name) = peel_casts(return_expr) else {
        return None;
    };
    let alloc_index = find_temp_index(&f.body[..return_index], temp_name.as_str())?;
    let call = allocation_temp(&f.body[alloc_index].stmt, &f.body, alloc_index, elem_ty)?;
    if call.name != temp_name.as_str() {
        return None;
    }
    Some(CalleeAllocSummaryFact {
        function,
        elem_ty: (**elem_ty).clone(),
        allocation: call.kind,
        extent: call.extent,
        init: call.init,
        return_path: AstPath(vec![PathSegment::Stmt(return_index)]),
        alloc_source_path: AstPath(vec![PathSegment::Stmt(alloc_index)]),
    })
}

fn count_returns(body: &[IndentStmt]) -> usize {
    let mut count = 0;
    for indent in body {
        if matches!(&indent.stmt, Stmt::Return(Some(_))) {
            count += 1;
        }
        walk::nested_bodies_with_path(&indent.stmt, &mut Vec::new(), &mut |nested, _| {
            count += count_returns(nested);
        });
    }
    count
}

fn top_level_return(body: &[IndentStmt]) -> Option<(usize, &Expr)> {
    body.iter()
        .enumerate()
        .find_map(|(index, indent)| match &indent.stmt {
            Stmt::Return(Some(expr)) => Some((index, expr)),
            _ => None,
        })
}

fn find_temp_index(body: &[IndentStmt], name: &str) -> Option<usize> {
    body.iter().position(|indent| {
        matches!(&indent.stmt, Stmt::Let { name: let_name, .. } if let_name.as_str() == name)
    })
}

fn peel_casts(expr: &Expr) -> &Expr {
    match expr {
        Expr::Cast { expr, .. } => peel_casts(expr),
        _ => expr,
    }
}
