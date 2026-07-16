use crate::fixups::facts::{AstPath, BindingId, FixupFacts, FunctionId, PathSegment};
use crate::fixups::idents::stmt_ident_count;
use crate::fixups::support::walk;
use crate::rust_ast::{BinOp, Expr, Ident, IndentStmt, Item, Program, RustValue, Stmt, Type};

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
        let mut changed = false;
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            walk::nested_body_vecs_mut_with_path(&mut body[index].stmt, path, &mut |body, path| {
                if !changed && fixup_body(body, function, facts, path) {
                    changed = true;
                }
            });
        });
        if changed {
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

struct Plan {
    producer_index: usize,
    option_name: String,
    option_expr: Expr,
    base_ptr: Option<Expr>,
    aliases: Vec<AliasPlan>,
}

struct AliasPlan {
    name: String,
    remove_indices: Vec<usize>,
}

struct NullableProducer {
    option_expr: Expr,
    base_ptr: Option<Expr>,
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
    let producer = nullable_pointer_option(init)?;
    let producer_binding =
        facts.binding_by_local_path(function, option_name, &AstPath(producer_path.clone()))?;
    if facts.def_use(producer_binding)?.reads.is_empty() {
        return None;
    }
    let aliases = collect_alias_chain(
        body,
        function,
        facts,
        body_path,
        producer_index,
        AliasSource {
            name: option_name.clone(),
            binding: producer_binding,
            remove_indices: Vec::new(),
        },
        producer.base_ptr.as_ref(),
    )?;
    Some(Plan {
        producer_index,
        option_name: option_name.clone(),
        option_expr: producer.option_expr,
        base_ptr: producer.base_ptr,
        aliases,
    })
}

fn apply_plan(body: &mut Vec<IndentStmt>, plan: &Plan) {
    if let Stmt::Let { ty, init, .. } = &mut body[plan.producer_index].stmt {
        *ty = None;
        *init = Some(plan.option_expr.clone());
    }
    for stmt in body.iter_mut() {
        rewrite_observations(&mut stmt.stmt, plan);
    }
    let mut remove = Vec::new();
    for alias in &plan.aliases {
        remove.extend(alias.remove_indices.iter().copied());
    }
    remove.sort_unstable();
    remove.dedup();
    for index in remove.into_iter().rev() {
        if index != plan.producer_index {
            body.remove(index);
        }
    }
}

struct AliasSource {
    name: String,
    binding: BindingId,
    remove_indices: Vec<usize>,
}

fn collect_alias_chain(
    body: &[IndentStmt],
    function: FunctionId,
    facts: &FixupFacts,
    body_path: &[PathSegment],
    producer_index: usize,
    producer: AliasSource,
    base_ptr: Option<&Expr>,
) -> Option<Vec<AliasPlan>> {
    let mut sources = vec![producer];
    let mut aliases = Vec::new();
    let mut cursor = 0;
    while cursor < sources.len() {
        let source_name = sources[cursor].name.clone();
        let source_binding = sources[cursor].binding;
        let def_use = facts.def_use(source_binding)?;
        for read in &def_use.reads {
            let read_index = direct_stmt_index(body_path, read)?;
            if read_index <= producer_index || read_index >= body.len() {
                return None;
            }
            let stmt = &body[read_index].stmt;
            if stmt_ident_count(stmt, &source_name) != 1 {
                return None;
            }
            if let Some(alias) =
                alias_edge(body, function, facts, body_path, read_index, &source_name)?
            {
                if !sources.iter().any(|source| source.name == alias.name) {
                    aliases.push(AliasPlan {
                        name: alias.name.clone(),
                        remove_indices: alias.remove_indices.clone(),
                    });
                    sources.push(alias);
                }
                continue;
            }
            if !supported_observation(stmt, &source_name, base_ptr) {
                return None;
            }
        }
        cursor += 1;
    }
    Some(aliases)
}

fn alias_edge(
    body: &[IndentStmt],
    function: FunctionId,
    facts: &FixupFacts,
    body_path: &[PathSegment],
    stmt_index: usize,
    source_name: &str,
) -> Option<Option<AliasSource>> {
    match &body[stmt_index].stmt {
        Stmt::Assign { target, value } => {
            let Expr::Var(alias_name) = target else {
                return Some(None);
            };
            if !transparent_alias_value(value, source_name) {
                return Some(None);
            }
            let binding =
                visible_local_binding(function, facts, body_path, stmt_index, alias_name.as_str())?;
            let def_use = facts.def_use(binding)?;
            if !def_use
                .writes
                .iter()
                .all(|write| direct_stmt_index(body_path, write) == Some(stmt_index))
            {
                return None;
            }
            let mut remove_indices = vec![stmt_index];
            if let Some(def_index) = direct_stmt_index(body_path, &def_use.definition)
                .filter(|index| *index < stmt_index)
            {
                if !removable_alias_decl(&body[def_index].stmt, alias_name.as_str()) {
                    return None;
                }
                remove_indices.push(def_index);
            }
            Some(Some(AliasSource {
                name: alias_name.to_string(),
                binding,
                remove_indices,
            }))
        }
        Stmt::Let {
            name,
            init: Some(init),
            ..
        } => {
            if !transparent_alias_value(init, source_name) {
                return Some(None);
            }
            let binding = facts.binding_by_local_path(
                function,
                name,
                &AstPath(stmt_path(body_path, stmt_index)),
            )?;
            let def_use = facts.def_use(binding)?;
            if !def_use
                .writes
                .iter()
                .all(|write| direct_stmt_index(body_path, write) == Some(stmt_index))
            {
                return None;
            }
            Some(Some(AliasSource {
                name: name.clone(),
                binding,
                remove_indices: vec![stmt_index],
            }))
        }
        _ => Some(None),
    }
}

fn transparent_alias_value(expr: &Expr, source_name: &str) -> bool {
    match expr {
        Expr::Var(name) => name.as_str() == source_name,
        Expr::Cast { expr, ty } if matches!(ty, Type::Ptr { .. }) => {
            transparent_alias_value(expr, source_name)
        }
        _ => false,
    }
}

fn removable_alias_decl(stmt: &Stmt, alias_name: &str) -> bool {
    matches!(
        stmt,
        Stmt::Let {
            name,
            mutable: true,
            init: Some(init),
            ..
        } if name == alias_name && is_null_expr(init)
    )
}

fn nullable_pointer_option(expr: &Expr) -> Option<NullableProducer> {
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
    Some(NullableProducer {
        option_expr: (**recv).clone(),
        base_ptr: pointer_closure_base(&args[1]),
    })
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

fn pointer_closure_base(expr: &Expr) -> Option<Expr> {
    let Expr::Closure { params, body } = expr else {
        return None;
    };
    let [index_param] = params.as_slice() else {
        return None;
    };
    pointer_add_base(body, index_param.as_str())
}

fn pointer_add_base(expr: &Expr, index_param: &str) -> Option<Expr> {
    match expr {
        Expr::Unsafe(block) | Expr::Block(block) if block.stmts.is_empty() => {
            pointer_add_base(block.tail.as_deref()?, index_param)
        }
        Expr::Cast { expr, ty } if matches!(ty, Type::Ptr { .. }) => {
            pointer_add_base(expr, index_param)
        }
        Expr::MethodCall { recv, method, args } if method == "add" && args.len() == 1 => {
            matches!(&args[0], Expr::Var(name) if name.as_str() == index_param)
                .then(|| (**recv).clone())
        }
        _ => None,
    }
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

fn supported_observation(stmt: &Stmt, alias_name: &str, base_ptr: Option<&Expr>) -> bool {
    match stmt {
        Stmt::Let {
            init: Some(expr), ..
        }
        | Stmt::Assign { value: expr, .. }
        | Stmt::Expr(expr)
        | Stmt::Return(Some(expr)) => {
            find_null_check(expr, alias_name).is_some()
                || base_ptr.is_some_and(|base_ptr| {
                    find_distance_observation(expr, alias_name, base_ptr).is_some()
                })
        }
        _ => false,
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

fn rewrite_observations(stmt: &mut Stmt, plan: &Plan) {
    match stmt {
        Stmt::Let {
            init: Some(expr), ..
        }
        | Stmt::Assign { value: expr, .. }
        | Stmt::Expr(expr)
        | Stmt::Return(Some(expr)) => rewrite_observation_expr(expr, plan),
        _ => {}
    }
}

fn rewrite_observation_expr(expr: &mut Expr, plan: &Plan) {
    if let Some(replacement) = observation_replacement(expr, &plan.option_name, plan) {
        *expr = replacement;
        return;
    }
    for alias in &plan.aliases {
        if let Some(replacement) = observation_replacement(expr, &alias.name, plan) {
            *expr = replacement;
            return;
        }
    }
    match expr {
        Expr::Binary { lhs, rhs, .. } => {
            rewrite_observation_expr(lhs, plan);
            rewrite_observation_expr(rhs, plan);
        }
        Expr::Cast { expr, .. }
        | Expr::Unary { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. } => rewrite_observation_expr(expr, plan),
        Expr::Call { func, args } => {
            rewrite_observation_expr(func, plan);
            for arg in args {
                rewrite_observation_expr(arg, plan);
            }
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            rewrite_observation_expr(recv, plan);
            for arg in args {
                rewrite_observation_expr(arg, plan);
            }
        }
        Expr::Block(block) | Expr::Unsafe(block) => {
            if block.stmts.is_empty()
                && let Some(tail) = &mut block.tail
            {
                rewrite_observation_expr(tail, plan);
            }
        }
        _ => {}
    }
}

fn observation_replacement(expr: &Expr, alias_name: &str, plan: &Plan) -> Option<Expr> {
    if let Expr::Binary { op, lhs, rhs } = expr
        && let Some(method) = null_comparison(op, lhs, rhs, alias_name)
    {
        return Some(method_call(var(&plan.option_name), method));
    }
    let base_ptr = plan.base_ptr.as_ref()?;
    let target_ty = find_distance_observation(expr, alias_name, base_ptr)?;
    Some(match target_ty {
        Some(ty) => Expr::Cast {
            expr: Box::new(method_call(var(&plan.option_name), "unwrap")),
            ty,
        },
        None => method_call(var(&plan.option_name), "unwrap"),
    })
}

fn find_distance_observation(
    expr: &Expr,
    alias_name: &str,
    base_ptr: &Expr,
) -> Option<Option<Type>> {
    match expr {
        Expr::Unsafe(block) | Expr::Block(block) if block.stmts.is_empty() => {
            find_distance_observation(block.tail.as_deref()?, alias_name, base_ptr)
        }
        Expr::Cast { expr, ty } => {
            let _ = distance_call(expr, alias_name, base_ptr)?;
            Some(Some(ty.clone()))
        }
        expr if distance_call(expr, alias_name, base_ptr).is_some() => Some(None),
        _ => None,
    }
}

fn distance_call<'a>(expr: &'a Expr, alias_name: &str, base_ptr: &Expr) -> Option<&'a Expr> {
    let Expr::MethodCall { recv, method, args } = expr else {
        return None;
    };
    if method == "offset_from"
        && args.len() == 1
        && matches!(&**recv, Expr::Var(name) if name.as_str() == alias_name)
        && same_expr(&args[0], base_ptr)
    {
        return Some(expr);
    }
    None
}

fn same_expr(lhs: &Expr, rhs: &Expr) -> bool {
    match (lhs, rhs) {
        (Expr::Var(lhs), Expr::Var(rhs)) => lhs == rhs,
        (
            Expr::MethodCall {
                recv: lhs_recv,
                method: lhs_method,
                args: lhs_args,
            },
            Expr::MethodCall {
                recv: rhs_recv,
                method: rhs_method,
                args: rhs_args,
            },
        ) => {
            lhs_method == rhs_method
                && lhs_args.is_empty()
                && rhs_args.is_empty()
                && same_expr(lhs_recv, rhs_recv)
        }
        (
            Expr::ArrayPtr {
                array: lhs_array,
                mutable: lhs_mutable,
            },
            Expr::ArrayPtr {
                array: rhs_array,
                mutable: rhs_mutable,
            },
        ) => lhs_mutable == rhs_mutable && same_expr(lhs_array, rhs_array),
        (Expr::ArrayPtr { array, mutable }, Expr::MethodCall { recv, method, args })
        | (Expr::MethodCall { recv, method, args }, Expr::ArrayPtr { array, mutable }) => {
            args.is_empty()
                && ((*mutable && method == "as_mut_ptr") || (!*mutable && method == "as_ptr"))
                && same_expr(array, recv)
        }
        (Expr::Cast { expr: lhs, .. }, rhs) => same_expr(lhs, rhs),
        (lhs, Expr::Cast { expr: rhs, .. }) => same_expr(lhs, rhs),
        _ => false,
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
    use crate::rust_ast::{Block, CLibType, MatchArm, Pattern, Prim, RustValue, Type};

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
    fn rewrites_direct_nullable_pointer_observation() {
        let mut program = Program {
            items: vec![Item::Fn(func(
                Vec::new(),
                None,
                vec![
                    option_pointer_temp("_v6"),
                    Stmt::Let {
                        name: "is_miss".into(),
                        mutable: false,
                        ty: Some(Type::Prim(Prim::Bool)),
                        init: Some(Expr::Binary {
                            op: BinOp::Eq,
                            lhs: Box::new(Expr::Var("_v6".into())),
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
        assert!(!out.contains("map_or"));
    }

    #[test]
    fn rewrites_nullable_pointer_observation_inside_match_arm() {
        let mut program = Program {
            items: vec![Item::Fn(func(
                Vec::new(),
                None,
                vec![Stmt::Match {
                    expr: Expr::Var("state".into()),
                    arms: vec![MatchArm {
                        pattern: Pattern::I64(0),
                        body: vec![
                            IndentStmt {
                                depth: 0,
                                stmt: option_pointer_temp("_v6"),
                            },
                            IndentStmt {
                                depth: 0,
                                stmt: Stmt::Let {
                                    name: "is_miss".into(),
                                    mutable: false,
                                    ty: Some(Type::Prim(Prim::Bool)),
                                    init: Some(Expr::Binary {
                                        op: BinOp::Eq,
                                        lhs: Box::new(Expr::Var("_v6".into())),
                                        rhs: Box::new(null_mut()),
                                    }),
                                },
                            },
                        ],
                    }],
                }],
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

    #[test]
    fn rewrites_same_base_pointer_distance_through_alias_chain() {
        let mut program = Program {
            items: vec![Item::Fn(func(
                Vec::new(),
                None,
                vec![
                    ptr_decl("hit"),
                    option_pointer_temp_from_base("_v3", "buf"),
                    Stmt::Assign {
                        target: Expr::Var("hit".into()),
                        value: cast_var("_v3", u8_ptr()),
                    },
                    Stmt::Let {
                        name: "_v10".into(),
                        mutable: false,
                        ty: Some(u8_ptr()),
                        init: Some(Expr::Var("hit".into())),
                    },
                    Stmt::Let {
                        name: "distance".into(),
                        mutable: false,
                        ty: Some(Type::Prim(Prim::I64)),
                        init: Some(Expr::Unsafe(Box::new(Block {
                            stmts: Vec::new(),
                            tail: Some(Box::new(Expr::Cast {
                                expr: Box::new(Expr::MethodCall {
                                    recv: Box::new(Expr::Var("_v10".into())),
                                    method: "offset_from".into(),
                                    args: vec![as_mut_ptr("buf")],
                                }),
                                ty: Type::Prim(Prim::I64),
                            })),
                        }))),
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
        assert!(out.contains("let _v3 = Some(1);"));
        assert!(out.contains("let distance: i64 = _v3.unwrap() as i64;"));
        assert!(!out.contains("let mut hit"));
        assert!(!out.contains("let _v10"));
        assert!(!out.contains("map_or"));
        assert!(!out.contains("offset_from"));
    }

    #[test]
    fn keeps_pointer_distance_from_unrelated_base_raw() {
        let mut program = Program {
            items: vec![Item::Fn(func(
                Vec::new(),
                None,
                vec![
                    ptr_decl("hit"),
                    option_pointer_temp_from_base("_v3", "buf"),
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
                            args: vec![as_mut_ptr("other")],
                        }),
                    },
                ],
            ))],
        };
        let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());

        assert!(!fixup(&mut program, &facts));
        let out = emit(match program.items.remove(0) {
            Item::Fn(f) => f,
            _ => unreachable!(),
        });
        assert!(out.contains("let _v3: *mut core::ffi::c_void = Some(1).map_or"));
        assert!(out.contains("hit.offset_from(other.as_mut_ptr())"));
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

    fn option_pointer_temp_from_base(name: &str, base: &str) -> Stmt {
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
                                expr: Box::new(Expr::MethodCall {
                                    recv: Box::new(as_mut_ptr(base)),
                                    method: "add".into(),
                                    args: vec![Expr::Var("i".into())],
                                }),
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

    fn as_mut_ptr(name: &str) -> Expr {
        Expr::MethodCall {
            recv: Box::new(Expr::Var(name.into())),
            method: "as_mut_ptr".into(),
            args: Vec::new(),
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
