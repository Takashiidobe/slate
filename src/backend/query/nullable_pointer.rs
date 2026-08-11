use crate::backend::rust_ast::{BinOp, Expr, Ident, RustValue, Stmt, Type};

pub(super) struct NullableProducer {
    pub(super) option_expr: Expr,
    pub(super) base_ptr: Option<Expr>,
}

pub(super) fn nullable_pointer_option(expr: &Expr) -> Option<NullableProducer> {
    let Expr::MethodCall { recv, method, args } = expr else {
        return None;
    };
    if method != "map_or"
        || args.len() != 2
        || !is_null_expr(&args[0])
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
        Expr::Cast { ty, .. } => matches!(ty, Type::Ptr { .. }),
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
        Expr::Cast {
            expr,
            ty: Type::Ptr { .. },
        } => pointer_add_base(expr, index_param),
        Expr::MethodCall { recv, method, args } if method == "add" && args.len() == 1 => {
            matches!(&args[0], Expr::Var(name) if name.as_str() == index_param)
                .then(|| (**recv).clone())
        }
        _ => None,
    }
}

pub(super) fn transparent_alias_value(expr: &Expr, source_name: &str) -> bool {
    match expr {
        Expr::Var(name) => name.as_str() == source_name,
        Expr::Cast {
            expr,
            ty: Type::Ptr { .. },
        } => transparent_alias_value(expr, source_name),
        _ => false,
    }
}

pub(super) fn removable_alias_decl(stmt: &Stmt, alias_name: &str) -> bool {
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

pub(super) fn is_generated_temp(name: &str) -> bool {
    name.strip_prefix("_v")
        .is_some_and(|suffix| !suffix.is_empty() && suffix.chars().all(|c| c.is_ascii_digit()))
}

pub(super) fn preferred_option_name(producer_name: &str, alias_names: &[String]) -> String {
    alias_names
        .iter()
        .find(|name| !is_generated_temp(name))
        .cloned()
        .unwrap_or_else(|| producer_name.to_string())
}

pub(super) fn supported_observation(
    stmt: &Stmt,
    alias_name: &str,
    base_ptr: Option<&Expr>,
) -> bool {
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

pub(super) fn find_distance_observation(
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

pub(super) fn null_comparison(
    op: &BinOp,
    lhs: &Expr,
    rhs: &Expr,
    alias_name: &str,
) -> Option<&'static str> {
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

fn is_null_expr(expr: &Expr) -> bool {
    matches!(expr, Expr::Value(RustValue::NullPtr))
        || matches!(
            expr,
            Expr::Call { func, args, .. } if args.is_empty() && is_null_path(func)
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
