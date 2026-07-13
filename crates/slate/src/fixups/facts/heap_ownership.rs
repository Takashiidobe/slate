use crate::fixups::facts::walk;
use crate::fixups::facts::{
    AstPath, BindingId, FixupFacts, FunctionId, HeapOwnershipFact, HeapOwnershipKind, PathSegment,
};
use crate::rust_ast::{Block, Expr, IndentStmt, Item, Program, RustValue, Stmt, Type};

pub(in crate::fixups) fn collect_facts(program: &Program, facts: &mut FixupFacts) {
    facts.heap_ownership.clear();
    for (item_index, item) in program.items.iter().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        collect_body(function, &f.body, facts);
    }
}

fn collect_body(function: FunctionId, body: &[IndentStmt], facts: &mut FixupFacts) {
    for (index, pair) in body.windows(2).enumerate() {
        let Some((pointer_name, elem_ty)) = null_pointer_decl(&pair[0].stmt) else {
            continue;
        };
        let pointer_path = AstPath(vec![PathSegment::Stmt(index)]);
        let Some(pointer) = facts.binding_by_local_path(function, pointer_name, &pointer_path)
        else {
            continue;
        };
        let Some(candidate) = find_allocation(function, body, facts, index + 1, pointer_name)
        else {
            continue;
        };
        if !heap_uses_are_owned(body, pointer_name, &candidate) {
            continue;
        }
        facts.heap_ownership.push(HeapOwnershipFact {
            function,
            pointer,
            allocation_temp: candidate.allocation_temp,
            size_temp: candidate.size_temp,
            free_temp: candidate.free_temp,
            pointer_path,
            allocation_path: AstPath(vec![PathSegment::Stmt(candidate.allocation_index)]),
            assign_path: AstPath(vec![PathSegment::Stmt(candidate.assign_index)]),
            free_path: AstPath(vec![PathSegment::Stmt(candidate.free_index)]),
            elem_ty,
            kind: HeapOwnershipKind::ScalarBox,
        });
    }
}

struct Candidate {
    allocation_index: usize,
    assign_index: usize,
    free_index: usize,
    allocation_temp: BindingId,
    size_temp: Option<BindingId>,
    free_temp: Option<BindingId>,
}

fn find_allocation(
    function: FunctionId,
    body: &[IndentStmt],
    facts: &FixupFacts,
    start: usize,
    pointer_name: &str,
) -> Option<Candidate> {
    for allocation_index in start..body.len() {
        let Some((allocation_name, size_name)) = malloc_temp(&body[allocation_index].stmt) else {
            continue;
        };
        let allocation_path = AstPath(vec![PathSegment::Stmt(allocation_index)]);
        let allocation_temp =
            facts.binding_by_local_path(function, allocation_name, &allocation_path)?;
        let size_temp = size_name.and_then(|name| {
            if allocation_index == 0 {
                return None;
            }
            facts.binding_by_local_path(
                function,
                name,
                &AstPath(vec![PathSegment::Stmt(allocation_index - 1)]),
            )
        });

        for assign_index in allocation_index + 1..body.len() {
            if !assigns_allocated_pointer(&body[assign_index].stmt, pointer_name, allocation_name) {
                continue;
            }
            let (free_index, free_temp) =
                single_free(function, body, facts, assign_index + 1, pointer_name)?;
            return Some(Candidate {
                allocation_index,
                assign_index,
                free_index,
                allocation_temp,
                size_temp,
                free_temp,
            });
        }
    }
    None
}

fn single_free(
    function: FunctionId,
    body: &[IndentStmt],
    facts: &FixupFacts,
    start: usize,
    pointer_name: &str,
) -> Option<(usize, Option<BindingId>)> {
    let mut found = None;
    for index in start..body.len() {
        if free_call_on_pointer(&body[index].stmt, pointer_name) {
            if found.is_some() {
                return None;
            }
            found = Some((index, None));
            continue;
        }
        if index + 1 < body.len()
            && let Some(temp) = pointer_alias_temp(&body[index].stmt, pointer_name)
            && free_call_on_pointer(&body[index + 1].stmt, temp)
        {
            if found.is_some() {
                return None;
            }
            let path = AstPath(vec![PathSegment::Stmt(index)]);
            let binding = facts.binding_by_local_path(function, temp, &path)?;
            found = Some((index + 1, Some(binding)));
        }
    }
    found
}

fn null_pointer_decl(stmt: &Stmt) -> Option<(&str, Type)> {
    let Stmt::Let {
        name,
        ty: Some(Type::Ptr {
            mutable: true,
            inner,
        }),
        init: Some(Expr::Value(RustValue::NullPtr)),
        ..
    } = stmt
    else {
        return None;
    };
    Some((name.as_str(), (**inner).clone()))
}

fn malloc_temp(stmt: &Stmt) -> Option<(&str, Option<&str>)> {
    let Stmt::Let {
        name,
        init: Some(init),
        ..
    } = stmt
    else {
        return None;
    };
    malloc_call_size_name(init).map(|size| (name.as_str(), size))
}

fn malloc_call_size_name(expr: &Expr) -> Option<Option<&str>> {
    let Expr::Unsafe(block) = expr else {
        return None;
    };
    let call = block.tail.as_deref()?;
    let Expr::Call { func, args } = call else {
        return None;
    };
    if !matches!(&**func, Expr::Var(name) if name.as_str() == "malloc") || args.len() != 1 {
        return None;
    }
    Some(size_arg_name(&args[0]))
}

fn size_arg_name(expr: &Expr) -> Option<&str> {
    match expr {
        Expr::Var(name) => Some(name.as_str()),
        Expr::Cast { expr, .. } => size_arg_name(expr),
        _ => None,
    }
}

fn assigns_allocated_pointer(stmt: &Stmt, pointer_name: &str, allocation_name: &str) -> bool {
    let Stmt::Assign { target, value } = stmt else {
        return false;
    };
    matches!(target, Expr::Var(name) if name.as_str() == pointer_name)
        && cast_source_var(value).is_some_and(|name| name == allocation_name)
}

fn cast_source_var(expr: &Expr) -> Option<&str> {
    match expr {
        Expr::Var(name) => Some(name.as_str()),
        Expr::Cast { expr, .. } => cast_source_var(expr),
        _ => None,
    }
}

fn pointer_alias_temp<'a>(stmt: &'a Stmt, pointer_name: &str) -> Option<&'a str> {
    let Stmt::Let {
        name,
        init: Some(Expr::Var(source)),
        ..
    } = stmt
    else {
        return None;
    };
    if source.as_str() == pointer_name {
        Some(name.as_str())
    } else {
        None
    }
}

fn free_call_on_pointer(stmt: &Stmt, pointer_name: &str) -> bool {
    let Stmt::Expr(expr) = stmt else {
        return false;
    };
    let Expr::Unsafe(block) = expr else {
        return false;
    };
    block_tail_free_arg(block).is_some_and(|arg| cast_source_var(arg) == Some(pointer_name))
}

fn block_tail_free_arg(block: &Block) -> Option<&Expr> {
    let Expr::Call { func, args } = block.tail.as_deref()? else {
        return None;
    };
    if matches!(&**func, Expr::Var(name) if name.as_str() == "free") && args.len() == 1 {
        args.first()
    } else {
        None
    }
}

fn heap_uses_are_owned(body: &[IndentStmt], pointer_name: &str, candidate: &Candidate) -> bool {
    let mut free_calls = 0;
    for (index, indent) in body.iter().enumerate() {
        if index == candidate.assign_index
            || index == candidate.allocation_index
            || index == candidate.free_index
            || Some(index) == candidate.size_temp.map(|_| candidate.allocation_index - 1)
        {
            continue;
        }
        if uses_pointer_as_free_alias(index, indent, body, pointer_name, candidate) {
            continue;
        }
        if pointer_alias_temp(&indent.stmt, pointer_name).is_some()
            || reassigns_pointer(&indent.stmt, pointer_name)
        {
            return false;
        }
        if free_call_on_pointer(&indent.stmt, pointer_name) {
            free_calls += 1;
            continue;
        }
        if stmt_escapes_pointer(&indent.stmt, pointer_name) {
            return false;
        }
    }
    free_calls == 0
}

fn uses_pointer_as_free_alias(
    index: usize,
    indent: &IndentStmt,
    body: &[IndentStmt],
    pointer_name: &str,
    candidate: &Candidate,
) -> bool {
    if candidate.free_temp.is_none() || index + 1 != candidate.free_index {
        return false;
    }
    let Some(alias) = pointer_alias_temp(&indent.stmt, pointer_name) else {
        return false;
    };
    body.get(candidate.free_index)
        .is_some_and(|stmt| free_call_on_pointer(&stmt.stmt, alias))
}

fn reassigns_pointer(stmt: &Stmt, pointer_name: &str) -> bool {
    matches!(
        stmt,
        Stmt::Assign {
            target: Expr::Var(name),
            ..
        } if name.as_str() == pointer_name
    )
}

fn stmt_escapes_pointer(stmt: &Stmt, pointer_name: &str) -> bool {
    let mut escaped = false;
    walk::stmt_exprs(stmt, &mut |expr| {
        if escaped {
            return;
        }
        match expr {
            Expr::Call { func, args } => {
                if matches!(&**func, Expr::Var(name) if name.as_str() == "free") {
                    return;
                }
                if args.iter().any(|arg| expr_mentions_var(arg, pointer_name)) {
                    escaped = true;
                }
            }
            Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
                if expr_mentions_var(recv, pointer_name)
                    || args.iter().any(|arg| expr_mentions_var(arg, pointer_name))
                {
                    escaped = true;
                }
            }
            Expr::AddrOf { expr, .. }
            | Expr::Ref { expr, .. }
            | Expr::ArrayPtr { array: expr, .. }
                if expr_mentions_var(expr, pointer_name) =>
            {
                escaped = true;
            }
            _ => {}
        }
    });
    escaped
}

fn expr_mentions_var(expr: &Expr, needle: &str) -> bool {
    walk::exprs_any(
        expr,
        &mut |expr| matches!(expr, Expr::Var(name) if name.as_str() == needle),
    )
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::facts;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Block, FnDef, Item, Program, Visibility};

    fn analyzed(stmts: Vec<Stmt>) -> facts::FixupFacts {
        let program = Program {
            items: vec![Item::Fn(FnDef {
                vis: Visibility::Private,
                unsafe_extern_c: false,
                name: "main".into(),
                params: Vec::new(),
                ret: None,
                body: stmts
                    .into_iter()
                    .map(|stmt| IndentStmt { depth: 1, stmt })
                    .collect(),
            })],
        };
        facts::analyze(program).facts
    }

    fn malloc_size(size: &str) -> Expr {
        Expr::Unsafe(Box::new(Block {
            stmts: Vec::new(),
            tail: Some(Box::new(Expr::Call {
                func: Box::new(var("malloc")),
                args: vec![Expr::Cast {
                    expr: Box::new(var(size)),
                    ty: Type::Prim(crate::rust_ast::Prim::U64),
                }],
            })),
        }))
    }

    fn free_ptr(ptr: &str) -> Stmt {
        Stmt::Expr(Expr::Unsafe(Box::new(Block {
            stmts: Vec::new(),
            tail: Some(Box::new(Expr::Call {
                func: Box::new(var("free")),
                args: vec![Expr::Cast {
                    expr: Box::new(var(ptr)),
                    ty: Type::parse("*mut core::ffi::c_void"),
                }],
            })),
        })))
    }

    #[test]
    fn records_scalar_malloc_free_owner() {
        let facts = analyzed(vec![
            let_mut("p", "*mut i32", Expr::Value(RustValue::NullPtr)),
            temp("size", "u64", int(4)),
            temp("raw", "*mut core::ffi::c_void", malloc_size("size")),
            assign(
                "p",
                Expr::Cast {
                    expr: Box::new(var("raw")),
                    ty: Type::parse("*mut i32"),
                },
            ),
            Stmt::Assign {
                target: Expr::Unary {
                    op: crate::rust_ast::UnaryOp::Deref,
                    expr: Box::new(var("p")),
                },
                value: int(1),
            },
            free_ptr("p"),
        ]);

        assert_eq!(facts.heap_ownership.len(), 1);
        let pointer = facts.binding_name(facts.heap_ownership[0].pointer).unwrap();
        assert_eq!(pointer, "p");
        assert_eq!(facts.heap_ownership[0].elem_ty.render(), "i32");
    }

    #[test]
    fn rejects_rebased_owner_pointer() {
        let facts = analyzed(vec![
            let_mut("p", "*mut i32", Expr::Value(RustValue::NullPtr)),
            temp("size", "u64", int(4)),
            temp("raw", "*mut core::ffi::c_void", malloc_size("size")),
            assign(
                "p",
                Expr::Cast {
                    expr: Box::new(var("raw")),
                    ty: Type::parse("*mut i32"),
                },
            ),
            assign(
                "p",
                Expr::Call {
                    func: Box::new(Expr::Var("p.offset".into())),
                    args: vec![int(1)],
                },
            ),
            free_ptr("p"),
        ]);

        assert!(facts.heap_ownership.is_empty());
    }
}
