use crate::fixups::facts::{AstPath, BindingId, FixupFacts, FunctionId, PathSegment};
use crate::rust_ast::{BinOp, Expr, Ident, IndentStmt, Item, Program, RustValue, Stmt};

pub(in crate::fixups) fn fixup(program: &mut Program, facts: &FixupFacts) -> bool {
    let mut changed = false;
    for (item_index, item) in program.items.iter_mut().enumerate() {
        if let Item::Fn(f) = item
            && let Some(function) = facts.function_by_item_index(item_index)
        {
            changed |= fixup_body(&mut f.body, function, facts, &mut Vec::new());
        }
    }
    changed
}

fn fixup_body(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
) -> bool {
    for index in 0..body.len() {
        if fixup_nested(&mut body[index].stmt, function, facts, path, index) {
            return true;
        }
    }
    for producer_index in 0..body.len() {
        let Some(plan) = plan_for_producer(body, producer_index, function, facts, path) else {
            continue;
        };
        apply_plan(body, &plan);
        return true;
    }
    false
}

fn fixup_nested(
    stmt: &mut Stmt,
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
    index: usize,
) -> bool {
    path.push(PathSegment::Stmt(index));
    let changed = match stmt {
        Stmt::If {
            then_body,
            else_body,
            ..
        }
        | Stmt::LetIf {
            then_body,
            else_body,
            ..
        } => {
            path.push(PathSegment::Then);
            let then_changed = fixup_body(then_body, function, facts, path);
            path.pop();
            path.push(PathSegment::Else);
            let else_changed = !then_changed && fixup_body(else_body, function, facts, path);
            path.pop();
            then_changed || else_changed
        }
        Stmt::Loop { body, .. } => {
            path.push(PathSegment::LoopBody);
            let changed = fixup_body(body, function, facts, path);
            path.pop();
            changed
        }
        Stmt::Scope { body } => {
            path.push(PathSegment::ScopeBody);
            let changed = fixup_body(body, function, facts, path);
            path.pop();
            changed
        }
        Stmt::LabeledBlock { body, .. } => {
            path.push(PathSegment::LabeledBody);
            let changed = fixup_body(body, function, facts, path);
            path.pop();
            changed
        }
        _ => false,
    };
    path.pop();
    changed
}

struct Plan {
    producer_index: usize,
    alias_decl_index: Option<usize>,
    alias_assign_index: Option<usize>,
    alias_name: String,
    option_name: String,
    option_expr: Expr,
}

fn plan_for_producer(
    body: &[IndentStmt],
    producer_index: usize,
    function: FunctionId,
    facts: &FixupFacts,
    body_path: &[PathSegment],
) -> Option<Plan> {
    let producer_path = stmt_path(body_path, producer_index);
    let Stmt::Let {
        name: option_name,
        mutable: false,
        init: Some(init),
        ..
    } = &body[producer_index].stmt
    else {
        return None;
    };
    let option_expr = nullable_pointer_option(init)?;
    let producer_binding =
        facts.binding_by_local_path(function, option_name, &AstPath(producer_path.clone()))?;
    let producer_reads = &facts.def_use(producer_binding)?.reads;
    if producer_reads.len() != 1 {
        return None;
    }
    let alias_assign_index = direct_stmt_index(body_path, &producer_reads[0])?;
    if alias_assign_index <= producer_index || alias_assign_index >= body.len() {
        return None;
    }
    let alias_name = alias_from_assignment(&body[alias_assign_index].stmt, option_name)?;
    let alias_binding =
        visible_local_binding(function, facts, body_path, alias_assign_index, &alias_name)?;
    let alias_def_use = facts.def_use(alias_binding)?;
    if !alias_def_use
        .writes
        .iter()
        .all(|write| direct_stmt_index(body_path, write) == Some(alias_assign_index))
    {
        return None;
    }
    if alias_def_use.reads.is_empty()
        || !alias_def_use
            .reads
            .iter()
            .all(|read| null_check_read(body, body_path, read, &alias_name).is_some())
    {
        return None;
    }
    let alias_decl_index = direct_stmt_index(body_path, &facts.def_use(alias_binding)?.definition)
        .filter(|index| *index < alias_assign_index);
    Some(Plan {
        producer_index,
        alias_decl_index,
        alias_assign_index: Some(alias_assign_index),
        alias_name,
        option_name: option_name.clone(),
        option_expr,
    })
}

fn apply_plan(body: &mut Vec<IndentStmt>, plan: &Plan) {
    if let Stmt::Let { ty, init, .. } = &mut body[plan.producer_index].stmt {
        *ty = None;
        *init = Some(plan.option_expr.clone());
    }
    for stmt in body.iter_mut() {
        rewrite_null_checks(&mut stmt.stmt, &plan.alias_name, &plan.option_name);
    }
    let mut remove = Vec::new();
    if let Some(index) = plan.alias_assign_index {
        remove.push(index);
    }
    if let Some(index) = plan.alias_decl_index {
        remove.push(index);
    }
    remove.sort_unstable();
    remove.dedup();
    for index in remove.into_iter().rev() {
        if index != plan.producer_index {
            body.remove(index);
        }
    }
}

fn nullable_pointer_option(expr: &Expr) -> Option<Expr> {
    let Expr::MethodCall { recv, method, args } = expr else {
        return None;
    };
    if method != "map_or"
        || args.len() != 2
        || !is_null_mut(&args[0])
        || !is_pointer_closure(&args[1])
    {
        return None;
    }
    Some((**recv).clone())
}

fn is_pointer_closure(expr: &Expr) -> bool {
    matches!(expr, Expr::Closure { body, .. } if returns_pointer(body))
}

fn returns_pointer(expr: &Expr) -> bool {
    match expr {
        Expr::Unsafe(block) | Expr::Block(block) => {
            block.tail.as_deref().is_some_and(returns_pointer)
        }
        Expr::Cast { ty, .. } => matches!(ty, crate::rust_ast::Type::Ptr { .. }),
        _ => false,
    }
}

fn alias_from_assignment(stmt: &Stmt, option_name: &str) -> Option<String> {
    let Stmt::Assign { target, value } = stmt else {
        return None;
    };
    let Expr::Var(alias) = target else {
        return None;
    };
    let Expr::Cast { expr, .. } = value else {
        return None;
    };
    matches!(&**expr, Expr::Var(name) if name.as_str() == option_name).then(|| alias.to_string())
}

fn visible_local_binding(
    function: FunctionId,
    facts: &FixupFacts,
    body_path: &[PathSegment],
    before_index: usize,
    name: &str,
) -> Option<BindingId> {
    facts
        .bindings
        .iter()
        .filter(|binding| binding.function == function && binding.name == name)
        .filter_map(|binding| {
            let index = direct_stmt_index(body_path, &binding.path)?;
            (index < before_index).then_some((index, binding.id))
        })
        .max_by_key(|(index, _)| *index)
        .map(|(_, binding)| binding)
}

fn null_check_read<'a>(
    body: &'a [IndentStmt],
    body_path: &[PathSegment],
    read: &AstPath,
    alias_name: &str,
) -> Option<&'a Expr> {
    let index = direct_stmt_index(body_path, read)?;
    if index >= body.len() {
        return None;
    }
    null_check_expr(&body[index].stmt, alias_name)
}

fn null_check_expr<'a>(stmt: &'a Stmt, alias_name: &str) -> Option<&'a Expr> {
    match stmt {
        Stmt::Let {
            init: Some(expr), ..
        }
        | Stmt::Assign { value: expr, .. }
        | Stmt::Expr(expr)
        | Stmt::Return(Some(expr)) => find_null_check(expr, alias_name),
        _ => None,
    }
}

fn find_null_check<'a>(expr: &'a Expr, alias_name: &str) -> Option<&'a Expr> {
    match expr {
        Expr::Binary { op, lhs, rhs } if null_comparison(op, lhs, rhs, alias_name).is_some() => {
            Some(expr)
        }
        Expr::Cast { expr, .. } => find_null_check(expr, alias_name),
        _ => None,
    }
}

fn rewrite_null_checks(stmt: &mut Stmt, alias_name: &str, option_name: &str) {
    match stmt {
        Stmt::Let {
            init: Some(expr), ..
        }
        | Stmt::Assign { value: expr, .. }
        | Stmt::Expr(expr)
        | Stmt::Return(Some(expr)) => rewrite_null_check_expr(expr, alias_name, option_name),
        _ => {}
    }
}

fn rewrite_null_check_expr(expr: &mut Expr, alias_name: &str, option_name: &str) {
    match expr {
        Expr::Binary { op, lhs, rhs } => {
            if let Some(method) = null_comparison(op, lhs, rhs, alias_name) {
                *expr = method_call(var(option_name), method);
            }
        }
        Expr::Cast { expr, .. } => rewrite_null_check_expr(expr, alias_name, option_name),
        _ => {}
    }
}

fn null_comparison(op: &BinOp, lhs: &Expr, rhs: &Expr, alias_name: &str) -> Option<&'static str> {
    let method = match (lhs, rhs) {
        (Expr::Var(name), null) if name.as_str() == alias_name && is_null_expr(null) => {
            Some("is_none")
        }
        (null, Expr::Var(name)) if name.as_str() == alias_name && is_null_expr(null) => {
            Some("is_none")
        }
        _ => None,
    }?;
    match op {
        BinOp::Eq => Some(method),
        BinOp::Ne => Some(if method == "is_none" {
            "is_some"
        } else {
            "is_none"
        }),
        _ => None,
    }
}

fn is_null_mut(expr: &Expr) -> bool {
    is_null_expr(expr)
}

fn is_null_expr(expr: &Expr) -> bool {
    matches!(expr, Expr::Value(RustValue::NullPtr))
        || matches!(
            expr,
            Expr::Call { func, args } if args.is_empty() && is_null_path(func)
        )
}

fn is_null_path(expr: &Expr) -> bool {
    let Expr::Path(path) = expr else {
        return false;
    };
    let segments = path.segments.iter().map(Ident::as_str).collect::<Vec<_>>();
    matches!(
        segments.as_slice(),
        ["std", "ptr", "null" | "null_mut"] | ["core", "ptr", "null" | "null_mut"]
    )
}

fn method_call(recv: Expr, method: &str) -> Expr {
    Expr::MethodCall {
        recv: Box::new(recv),
        method: method.into(),
        args: Vec::new(),
    }
}

fn var(name: &str) -> Expr {
    Expr::Var(Ident::from(name))
}

fn stmt_path(body_path: &[PathSegment], index: usize) -> Vec<PathSegment> {
    let mut path = body_path.to_vec();
    path.push(PathSegment::Stmt(index));
    path
}

fn direct_stmt_index(body_path: &[PathSegment], path: &AstPath) -> Option<usize> {
    let rest = path.0.strip_prefix(body_path)?;
    match rest {
        [PathSegment::Stmt(index)] | [PathSegment::Stmt(index), ..] => Some(*index),
        _ => None,
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::facts;
    use crate::fixups::test_support::{emit, func};
    use crate::rust_ast::{Block, CLibType, Prim, RustValue, Type};

    #[test]
    fn rewrites_null_only_nullable_pointer_chain_to_option() {
        let mut program = Program {
            items: vec![Item::Fn(func(
                Vec::new(),
                None,
                vec![
                    Stmt::Let {
                        name: "miss".into(),
                        mutable: true,
                        ty: Some(Type::Ptr {
                            mutable: true,
                            inner: Box::new(Type::Prim(Prim::U8)),
                        }),
                        init: Some(null_mut()),
                    },
                    Stmt::Let {
                        name: "_v6".into(),
                        mutable: false,
                        ty: Some(Type::Ptr {
                            mutable: true,
                            inner: Box::new(Type::CLib(CLibType::Void)),
                        }),
                        init: Some(Expr::MethodCall {
                            recv: Box::new(Expr::Call {
                                func: Box::new(Expr::Var("Some".into())),
                                args: vec![Expr::Value(RustValue::I64(1))],
                            }),
                            method: "map_or".into(),
                            args: vec![
                                null_mut(),
                                Expr::Closure {
                                    params: vec![Ident::from("i")],
                                    body: Box::new(Expr::Unsafe(Box::new(Block {
                                        stmts: Vec::new(),
                                        tail: Some(Box::new(Expr::Cast {
                                            expr: Box::new(Expr::Var("i".into())),
                                            ty: Type::Ptr {
                                                mutable: true,
                                                inner: Box::new(Type::CLib(CLibType::Void)),
                                            },
                                        })),
                                    }))),
                                },
                            ],
                        }),
                    },
                    Stmt::Assign {
                        target: Expr::Var("miss".into()),
                        value: Expr::Cast {
                            expr: Box::new(Expr::Var("_v6".into())),
                            ty: Type::Ptr {
                                mutable: true,
                                inner: Box::new(Type::Prim(Prim::U8)),
                            },
                        },
                    },
                    Stmt::Let {
                        name: "is_miss".into(),
                        mutable: false,
                        ty: Some(Type::Prim(Prim::Bool)),
                        init: Some(Expr::Binary {
                            op: BinOp::Eq,
                            lhs: Box::new(Expr::Var("miss".into())),
                            rhs: Box::new(null_mut()),
                        }),
                    },
                ],
            ))],
        };
        let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());

        assert!(fixup(&mut program, &facts));
        let out = emit(match program.items.remove(0) {
            Item::Fn(f) => f,
            _ => unreachable!(),
        });
        assert!(out.contains("let _v6 = Some(1);"));
        assert!(out.contains("let is_miss: bool = _v6.is_none();"));
        assert!(!out.contains("let mut miss"));
        assert!(!out.contains("map_or"));
    }

    #[test]
    fn skips_pointer_identity_chain_then_rewrites_later_null_only_chain() {
        let mut program = Program {
            items: vec![Item::Fn(func(
                Vec::new(),
                None,
                vec![
                    ptr_decl("hit"),
                    ptr_decl("miss"),
                    option_pointer_temp("_v3"),
                    Stmt::Assign {
                        target: Expr::Var("hit".into()),
                        value: cast_var("_v3", u8_ptr()),
                    },
                    Stmt::Let {
                        name: "distance".into(),
                        mutable: false,
                        ty: Some(Type::Prim(Prim::I64)),
                        init: Some(Expr::MethodCall {
                            recv: Box::new(Expr::Var("hit".into())),
                            method: "offset_from".into(),
                            args: vec![Expr::Var("base".into())],
                        }),
                    },
                    option_pointer_temp("_v6"),
                    Stmt::Assign {
                        target: Expr::Var("miss".into()),
                        value: cast_var("_v6", u8_ptr()),
                    },
                    Stmt::Let {
                        name: "is_miss".into(),
                        mutable: false,
                        ty: Some(Type::Prim(Prim::Bool)),
                        init: Some(Expr::Binary {
                            op: BinOp::Eq,
                            lhs: Box::new(Expr::Var("miss".into())),
                            rhs: Box::new(null_mut()),
                        }),
                    },
                ],
            ))],
        };
        let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());

        assert!(fixup(&mut program, &facts));
        let out = emit(match program.items.remove(0) {
            Item::Fn(f) => f,
            _ => unreachable!(),
        });
        assert!(out.contains("let _v3: *mut core::ffi::c_void = Some(1).map_or"));
        assert!(out.contains("let _v6 = Some(1);"));
        assert!(out.contains("let is_miss: bool = _v6.is_none();"));
        assert!(out.contains("let mut hit"));
        assert!(!out.contains("let mut miss"));
    }

    fn null_mut() -> Expr {
        Expr::Call {
            func: Box::new(Expr::Path(crate::rust_ast::Path::new([
                Ident::from("std"),
                Ident::from("ptr"),
                Ident::from("null_mut"),
            ]))),
            args: Vec::new(),
        }
    }

    fn ptr_decl(name: &str) -> Stmt {
        Stmt::Let {
            name: name.into(),
            mutable: true,
            ty: Some(u8_ptr()),
            init: Some(null_mut()),
        }
    }

    fn option_pointer_temp(name: &str) -> Stmt {
        Stmt::Let {
            name: name.into(),
            mutable: false,
            ty: Some(Type::Ptr {
                mutable: true,
                inner: Box::new(Type::CLib(CLibType::Void)),
            }),
            init: Some(Expr::MethodCall {
                recv: Box::new(Expr::Call {
                    func: Box::new(Expr::Var("Some".into())),
                    args: vec![Expr::Value(RustValue::I64(1))],
                }),
                method: "map_or".into(),
                args: vec![
                    null_mut(),
                    Expr::Closure {
                        params: vec![Ident::from("i")],
                        body: Box::new(Expr::Unsafe(Box::new(Block {
                            stmts: Vec::new(),
                            tail: Some(Box::new(Expr::Cast {
                                expr: Box::new(Expr::Var("i".into())),
                                ty: Type::Ptr {
                                    mutable: true,
                                    inner: Box::new(Type::CLib(CLibType::Void)),
                                },
                            })),
                        }))),
                    },
                ],
            }),
        }
    }

    fn cast_var(name: &str, ty: Type) -> Expr {
        Expr::Cast {
            expr: Box::new(Expr::Var(name.into())),
            ty,
        }
    }

    fn u8_ptr() -> Type {
        Type::Ptr {
            mutable: true,
            inner: Box::new(Type::Prim(Prim::U8)),
        }
    }
}
