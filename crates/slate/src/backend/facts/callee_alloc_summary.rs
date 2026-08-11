use crate::backend::facts::heap_ownership::allocation_temp;
use crate::backend::facts::walk;
use crate::backend::facts::{
    AllocProvenance, AstPath, CalleeAllocSummaryFact, FunctionId, PathSegment,
};
use crate::backend::rust_ast::{Expr, FnDef, IndentStmt, Stmt, Type};
pub(in crate::backend) fn collect_for_function<'db>(
    function: FunctionId<'db>,
    f: &FnDef,
) -> Option<CalleeAllocSummaryFact<'db>> {
    let Some(Type::Ptr { inner: elem_ty, .. }) = &f.ret else {
        return None;
    };
    let mut returns = Vec::new();
    collect_returns(&f.body, &mut Vec::new(), &mut returns);
    if returns.is_empty() {
        return None;
    }

    // A single, top-level return tracing (through at most one hoisted temp) to a malloc/calloc
    // call: this function is itself the root allocator.
    if let [(return_path, return_expr)] = returns.as_slice()
        && let [PathSegment::Stmt(_)] = return_path.0.as_slice()
        && let Some((alloc_index, call)) = resolve_direct_alloc(&f.body, return_expr, elem_ty)
    {
        return Some(CalleeAllocSummaryFact {
            function,
            provenance: AllocProvenance::Direct {
                elem_ty: (**elem_ty).clone(),
                allocation: call.kind,
                extent: call.extent,
                init: call.init,
                return_path: return_path.clone(),
                alloc_source_path: AstPath(vec![PathSegment::Stmt(alloc_index)]),
            },
        });
    }

    // Otherwise, every reachable return (however many, however nested, through at most one
    // hoisted temp local to its own enclosing scope) must be a direct call to another
    // function -- whether they all ultimately agree on one allocation shape is an
    // interprocedural question the caller-eligibility join resolves, not this collector.
    let mut callees = Vec::new();
    for (return_path, return_expr) in &returns {
        let target = resolve_through_temp(&f.body, return_path, return_expr)?;
        let Expr::Call { func, .. } = target else {
            return None;
        };
        let Expr::Var(callee_name) = func.as_ref() else {
            return None;
        };
        callees.push(callee_name.as_str().to_string());
    }
    Some(CalleeAllocSummaryFact {
        function,
        provenance: AllocProvenance::PassThrough { callees },
    })
}

/// Resolves `return_expr` (a top-level return's own expression) through at most one hoisted
/// temp defined earlier in `f_body`, then checks whether the result is a malloc/calloc call.
fn resolve_direct_alloc(
    f_body: &[IndentStmt],
    return_expr: &Expr,
    elem_ty: &Type,
) -> Option<(usize, super::heap_ownership::AllocationCall)> {
    let Expr::Var(temp_name) = peel_casts(return_expr) else {
        return None;
    };
    let alloc_index = find_temp_index(f_body, temp_name.as_str())?;
    let call = allocation_temp(&f_body[alloc_index].stmt, f_body, alloc_index, elem_ty)?;
    (call.name == temp_name.as_str()).then_some((alloc_index, call))
}

/// Resolves a return expression through at most one hoisted temp defined in the return's own
/// enclosing statement list (which may be nested inside if/else/scope bodies, not just the
/// function's top level), returning the expression the temp was initialized with.
fn resolve_through_temp<'a>(
    f_body: &'a [IndentStmt],
    return_path: &AstPath,
    return_expr: &'a Expr,
) -> Option<&'a Expr> {
    let peeled = peel_casts(return_expr);
    let Expr::Var(temp_name) = peeled else {
        return Some(peeled);
    };
    let (enclosing, return_index) = enclosing_body(f_body, &return_path.0)?;
    let temp_index = find_temp_index(&enclosing[..return_index], temp_name.as_str())?;
    let Stmt::Let {
        name,
        init: Some(init),
        ..
    } = &enclosing[temp_index].stmt
    else {
        return None;
    };
    (name.as_str() == temp_name.as_str()).then(|| peel_casts(init))
}

/// Given a return statement's own full path, navigates to the statement list it's a direct
/// member of (its "enclosing scope"), and its own index within that list. Unlike a plain
/// `Stmt::Let` lookup, this walks through nested If/Scope/etc. path segments rather than
/// assuming the return lives at the function's top level.
fn enclosing_body<'a>(
    f_body: &'a [IndentStmt],
    path: &[PathSegment],
) -> Option<(&'a [IndentStmt], usize)> {
    let (last, prefix) = path.split_last()?;
    let PathSegment::Stmt(final_index) = last else {
        return None;
    };
    let mut current = f_body;
    let mut remaining = prefix;
    while let Some((segment, rest)) = remaining.split_first() {
        let PathSegment::Stmt(index) = segment else {
            return None;
        };
        let indent = current.get(*index)?;
        let (body_segment, deeper) = rest.split_first()?;
        current = nested_body_slice(&indent.stmt, body_segment)?;
        remaining = deeper;
    }
    Some((current, *final_index))
}

fn nested_body_slice<'a>(stmt: &'a Stmt, segment: &PathSegment) -> Option<&'a [IndentStmt]> {
    match (stmt, segment) {
        (Stmt::Scope { body }, PathSegment::ScopeBody) => Some(body),
        (Stmt::If { then_body, .. }, PathSegment::Then) => Some(then_body),
        (Stmt::If { else_body, .. }, PathSegment::Else) => Some(else_body),
        (Stmt::Loop { body, .. }, PathSegment::LoopBody) => Some(body),
        (Stmt::For { body, .. }, PathSegment::ForBody) => Some(body),
        (Stmt::LabeledBlock { body, .. }, PathSegment::LabeledBody) => Some(body),
        (Stmt::Unsafe { body }, PathSegment::UnsafeBody) => Some(&body.stmts),
        (Stmt::While { body, .. }, PathSegment::WhileBody) => Some(&body.stmts),
        (Stmt::Block(body), PathSegment::BlockBody) => Some(&body.stmts),
        _ => None,
    }
}

/// Collects every return statement in `body`, at any nesting depth, with its full path.
/// Does not attempt to prove any of them unreachable (e.g. a return following an if/else
/// where both branches already return) -- that's dead-code elimination's job, not this
/// collector's; a stray unresolvable return from dead code simply makes the whole function
/// fail to classify here, which is conservative and correct, just not maximally precise
/// until such a pass exists upstream of this one.
fn collect_returns(
    body: &[IndentStmt],
    path: &mut Vec<PathSegment>,
    out: &mut Vec<(AstPath, Expr)>,
) {
    for (index, indent) in body.iter().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            if let Stmt::Return(Some(expr)) = &indent.stmt {
                out.push((AstPath(path.clone()), expr.clone()));
            }
            walk::nested_bodies_with_path(&indent.stmt, path, &mut |nested, path| {
                collect_returns(nested, path, out);
            });
        });
    }
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
