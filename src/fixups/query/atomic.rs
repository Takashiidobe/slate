use crate::rust_ast::{Expr, IndentStmt, Pattern, Prim, Stmt, Type, UnaryOp};

use super::AtomicCompareExchangeChain;

pub(super) fn compare_exchange_chain(body: &[IndentStmt]) -> Option<AtomicCompareExchangeChain> {
    let [result, value, ok, not, failure, final_binding] = body else {
        return None;
    };
    let Stmt::Let {
        name: result_name,
        mutable: false,
        init: Some(compare_exchange),
        ..
    } = &result.stmt
    else {
        return None;
    };
    if !is_temp_name(result_name) || !matches!(compare_exchange, Expr::AtomicCompareExchange { .. })
    {
        return None;
    }
    let Expr::AtomicCompareExchange { expected, .. } = compare_exchange else {
        unreachable!()
    };
    let Expr::Var(expected_name) = &**expected else {
        return None;
    };

    let value_name = match_value_temp(&value.stmt, result_name)?;
    let ok_name = is_ok_temp(&ok.stmt, result_name)?;
    let not_name = not_temp(&not.stmt, ok_name)?;
    failure_write(&failure.stmt, not_name, expected_name.as_str(), value_name)?;
    let (final_name, mutable, ty, needs_cast) = final_bool_binding(&final_binding.stmt, ok_name)?;

    Some(AtomicCompareExchangeChain {
        compare_exchange: compare_exchange.clone(),
        expected_name: expected_name.to_string(),
        final_name: final_name.to_string(),
        mutable,
        ty: ty.cloned(),
        needs_cast,
        depth: final_binding.depth,
    })
}

fn match_value_temp<'a>(stmt: &'a Stmt, result_name: &str) -> Option<&'a str> {
    let Stmt::Let {
        name: value_name,
        mutable: false,
        init: Some(Expr::Match { expr, arms }),
        ..
    } = stmt
    else {
        return None;
    };
    if !is_temp_name(value_name)
        || !matches!(&**expr, Expr::Var(var) if var.as_str() == result_name)
    {
        return None;
    }
    let [ok, err] = arms.as_slice() else {
        return None;
    };
    if !matches!(
        &ok.pattern,
        Pattern::TupleStruct { name, fields }
            if name.as_str() == "Ok" && matches!(fields.as_slice(), [Pattern::Binding(v)] if expr_is_var(&ok.value, v.as_str()))
    ) || !matches!(
        &err.pattern,
        Pattern::TupleStruct { name, fields }
            if name.as_str() == "Err" && matches!(fields.as_slice(), [Pattern::Binding(v)] if expr_is_var(&err.value, v.as_str()))
    ) {
        return None;
    }
    Some(value_name)
}

fn is_ok_temp<'a>(stmt: &'a Stmt, result_name: &str) -> Option<&'a str> {
    let Stmt::Let {
        name,
        mutable: false,
        init: Some(Expr::MethodCall { recv, method, args }),
        ..
    } = stmt
    else {
        return None;
    };
    (is_temp_name(name)
        && method == "is_ok"
        && args.is_empty()
        && matches!(&**recv, Expr::Var(var) if var.as_str() == result_name))
    .then_some(name)
}

fn not_temp<'a>(stmt: &'a Stmt, ok_name: &str) -> Option<&'a str> {
    let Stmt::Let {
        name,
        mutable: false,
        init: Some(Expr::Unary {
            op: UnaryOp::Not,
            expr,
        }),
        ..
    } = stmt
    else {
        return None;
    };
    (is_temp_name(name) && matches!(&**expr, Expr::Var(var) if var.as_str() == ok_name))
        .then_some(name)
}

fn failure_write(stmt: &Stmt, not_name: &str, expected_name: &str, value_name: &str) -> Option<()> {
    let Stmt::If {
        cond: Expr::Var(cond),
        then_body,
        else_body,
    } = stmt
    else {
        return None;
    };
    let [
        IndentStmt {
            stmt: Stmt::Assign { target, value },
            ..
        },
    ] = then_body.as_slice()
    else {
        return None;
    };
    (cond.as_str() == not_name
        && else_body.is_empty()
        && matches!(target, Expr::Var(var) if var.as_str() == expected_name)
        && matches!(value, Expr::Var(var) if var.as_str() == value_name))
    .then_some(())
}

fn final_bool_binding<'a>(
    stmt: &'a Stmt,
    ok_name: &str,
) -> Option<(&'a str, bool, Option<&'a Type>, bool)> {
    let Stmt::Let {
        name,
        mutable,
        ty,
        init: Some(init),
    } = stmt
    else {
        return None;
    };
    match init {
        Expr::Cast { expr, ty } if matches!(ty, Type::Prim(Prim::I32)) => {
            matches!(&**expr, Expr::Var(var) if var.as_str() == ok_name).then_some((
                name.as_str(),
                *mutable,
                Some(ty),
                true,
            ))
        }
        Expr::Var(var) if var.as_str() == ok_name => {
            Some((name.as_str(), *mutable, ty.as_ref(), false))
        }
        _ => None,
    }
}

fn expr_is_var(expr: &Expr, name: &str) -> bool {
    matches!(expr, Expr::Var(var) if var.as_str() == name)
}

fn is_temp_name(name: &str) -> bool {
    name.strip_prefix("_v")
        .is_some_and(|rest| !rest.is_empty() && rest.bytes().all(|b| b.is_ascii_digit()))
}
