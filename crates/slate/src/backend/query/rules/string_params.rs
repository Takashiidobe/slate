use std::collections::HashMap;

use crate::backend::rust_ast::{Expr, Ident, IndentStmt, Prim, Stmt, Type};
use crate::backend::support::walk;
use crate::backend::trace::Pass;

use super::super::{
    CallTarget, EditSet, Function, FunctionRef, ItemCaseContext, QueryRule, Rejection,
};

pub(in crate::backend) fn rewrite() -> QueryRule<Function> {
    QueryRule::new(
        Pass::StringParams,
        "rewrite_string_params",
        Function::default(),
    )
    .case("string_param_lift", rewrite_case)
}

fn rewrite_case<'db>(
    case: &mut ItemCaseContext<'_, '_>,
    function: &FunctionRef<'db>,
) -> Result<EditSet, Rejection> {
    let mut replacement = case
        .fact(|query| query.function_snapshot(function))?
        .clone();
    let mut changed = false;

    if let Ok(own_indices) = case.fact(|query| query.string_param_lift_indices(function)) {
        for index in own_indices {
            if let Some(param) = replacement.params.get_mut(index) {
                let name = param.name.clone();
                param.ty = str_ref_type();
                rewrite_param_aliases(&mut replacement.body, &name);
                changed = true;
            }
        }
    }

    let calls = case.fact(|query| query.calls_in(function))?;
    let mut callee_indices = HashMap::<String, Vec<usize>>::new();
    for call in &calls {
        let CallTarget::Direct(name) = &call.target else {
            continue;
        };
        if callee_indices.contains_key(name.as_str()) {
            continue;
        }
        let Ok(callee) = case.fact(|query| query.function_by_name(name)) else {
            continue;
        };
        if let Ok(indices) = case.fact(|query| query.string_param_lift_indices(&callee)) {
            callee_indices.insert(name.clone(), indices);
        }
    }
    if !callee_indices.is_empty() {
        changed |= rewrite_call_args(&mut replacement.body, &callee_indices);
    }

    case.require(changed)?;
    Ok(EditSet::replace_function(function.clone(), replacement))
}

fn rewrite_call_args(
    body: &mut [IndentStmt],
    callee_indices: &HashMap<String, Vec<usize>>,
) -> bool {
    let mut changed = false;
    walk::body_exprs_mut_with(body, &mut |expr| {
        let Expr::Call { func, args, .. } = expr else {
            return true;
        };
        let Expr::Var(name) = &**func else {
            return true;
        };
        let Some(indices) = callee_indices.get(name.as_str()) else {
            return true;
        };
        for &index in indices {
            let Some(arg) = args.get_mut(index) else {
                continue;
            };
            if let Some(replacement) = lifted_arg(arg) {
                *arg = replacement;
                changed = true;
            }
        }
        true
    });
    changed
}

fn rewrite_param_aliases(body: &mut [IndentStmt], param_name: &str) -> bool {
    let mut changed = false;
    for indent in body {
        if let Stmt::Let {
            ty: Some(ty),
            init: Some(Expr::Var(source)),
            ..
        } = &mut indent.stmt
            && is_char_ptr(ty)
            && source.as_str() == param_name
        {
            *ty = str_ref_type();
            changed = true;
        }
        walk::nested_body_vecs_mut_with_path(
            &mut indent.stmt,
            &mut Vec::new(),
            &mut |nested, _path| {
                changed |= rewrite_param_aliases(nested, param_name);
            },
        );
    }
    changed
}

fn lifted_arg(expr: &Expr) -> Option<Expr> {
    match expr {
        Expr::Cast { expr, .. }
        | Expr::Unary { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => lifted_arg(expr),
        Expr::MethodCall { recv, method, args }
            if args.is_empty() && matches!(method.as_str(), "as_ptr" | "as_mut_ptr") =>
        {
            lifted_arg(recv)
        }
        Expr::ArrayPtr { array, .. } => lifted_arg(array),
        Expr::Var(name) => Some(Expr::Var(Ident::new(name.as_str()))),
        Expr::CStr(bytes) => {
            let payload = c_string_payload(bytes)?;
            Some(Expr::Str(std::str::from_utf8(payload).ok()?.to_string()))
        }
        _ => None,
    }
}

fn str_ref_type() -> Type {
    Type::Ref {
        mutable: false,
        inner: Box::new(Type::Str),
    }
}

fn c_string_payload(bytes: &[u8]) -> Option<&[u8]> {
    let payload = bytes.strip_suffix(&[0])?;
    (!payload.contains(&0) && std::str::from_utf8(payload).is_ok()).then_some(payload)
}

fn is_char_ptr(ty: &Type) -> bool {
    let Type::Ptr { inner, .. } = ty else {
        return false;
    };
    matches!(&**inner, Type::Prim(Prim::I8 | Prim::U8))
}
