use crate::rust_ast::{Attr, Expr, Type, UnaryOp};

use super::{
    BindingCategory, BindingRef, CallTarget, Evidence, EvidenceDetail, ExprSite, ParameterRemoval,
    Predicate, Proof, QueryContext, QueryResult, Rejection, RejectionReason,
};

pub(super) fn removable_parameter(
    query: &QueryContext<'_>,
    binding: &BindingRef,
) -> QueryResult<ParameterRemoval> {
    let predicate = Predicate::UnusedParam;
    let site = ExprSite {
        item_index: binding.item_index,
        path: Default::default(),
        fact_path: Default::default(),
    };
    let BindingCategory::Parameter { index } = binding.kind else {
        return Err(reject(predicate, site));
    };
    let Some(ty) = &binding.ty else {
        return Err(reject(predicate, site));
    };
    if !trivially_droppable(ty) {
        return Err(reject(predicate, site));
    }
    let (uses, mut evidence) = query.binding_def_use(binding)?.into_parts();
    if !uses.reads.is_empty() || !uses.writes.is_empty() {
        return Err(reject(predicate, site));
    }
    let Some(function) = query
        .all_functions()
        .into_iter()
        .find(|function| function.item_index == binding.item_index)
    else {
        return Err(reject(predicate, site));
    };
    if function.function.name == "main"
        || function.function.abi.is_some()
        || function.function.attrs.iter().any(is_exported)
        || index >= function.function.params.len()
    {
        return Err(reject(predicate, site));
    }
    let calls = query
        .all_calls()
        .filter(|call| {
            matches!(&call.target, CallTarget::Direct(name) if name == &function.function.name)
        })
        .collect::<Vec<_>>();
    if query.symbol_use_count(&function.function.name) != calls.len() {
        return Err(reject(predicate, site));
    }
    if query.all_bindings().into_iter().any(|other| {
        let BindingCategory::Parameter { index: other_index } = other.kind else {
            return false;
        };
        other.item_index == binding.item_index
            && other_index > index
            && other.ty.as_ref().is_some_and(trivially_droppable)
            && query
                .binding_def_use(&other)
                .is_ok_and(|proof| proof.value.reads.is_empty() && proof.value.writes.is_empty())
            && calls.iter().all(|call| {
                call.args
                    .get(other_index)
                    .and_then(|argument| query.expr(argument))
                    .is_some_and(is_pure_expr)
            })
    }) {
        return Err(reject(predicate, site));
    }
    let mut call_sites = Vec::new();
    for call in calls {
        if call.args.len() != function.function.params.len()
            || !call
                .args
                .get(index)
                .and_then(|argument| query.expr(argument))
                .is_some_and(is_pure_expr)
        {
            return Err(reject(predicate, site));
        }
        call_sites.push(call.site.clone());
    }
    evidence.push(Evidence {
        predicate,
        site,
        detail: EvidenceDetail::UnusedParam {
            function: binding.function_name.clone(),
            param: binding.name.clone(),
            param_index: index,
        },
    });
    Ok(Proof::new(
        ParameterRemoval {
            binding: binding.clone(),
            function,
            index,
            calls: call_sites,
        },
        evidence,
    ))
}

fn reject(predicate: Predicate, site: ExprSite) -> Rejection {
    Rejection::new(
        predicate,
        Some(site),
        RejectionReason::MissingEvidence,
        Vec::new(),
    )
}

fn is_exported(attr: &Attr) -> bool {
    matches!(attr, Attr::Used(_) | Attr::NoMangle | Attr::WeakLinkage)
}

fn trivially_droppable(ty: &Type) -> bool {
    match ty {
        Type::Prim(_) | Type::Unit | Type::Ptr { .. } | Type::Ref { .. } => true,
        Type::Array { elem, .. } => trivially_droppable(elem),
        Type::Custom(_)
        | Type::LongDouble
        | Type::TyVar(_)
        | Type::CLib(_)
        | Type::Complex(_)
        | Type::Generic { .. }
        | Type::VaList
        | Type::Str
        | Type::Slice(_)
        | Type::FnPtr { .. }
        | Type::Variadic
        | Type::Never => false,
    }
}

fn is_pure_expr(expr: &Expr) -> bool {
    match expr {
        Expr::Value(_) | Expr::Str(_) | Expr::ByteStr(_) | Expr::CStr(_) | Expr::HexFloat(_) => {
            true
        }
        Expr::Var(_) | Expr::Path(_) => true,
        Expr::Unary { op, expr } => matches!(op, UnaryOp::Neg | UnaryOp::Not) && is_pure_expr(expr),
        Expr::Binary { lhs, rhs, .. }
        | Expr::Range {
            start: lhs,
            end: rhs,
        } => is_pure_expr(lhs) && is_pure_expr(rhs),
        Expr::Cast { expr, .. } => is_pure_expr(expr),
        Expr::Field { base, .. } | Expr::TupleField { base, .. } => is_pure_expr(base),
        Expr::Index { base, index } => is_pure_expr(base) && is_pure_expr(index),
        Expr::StructLit { fields, .. } => fields.iter().all(|(_, value)| is_pure_expr(value)),
        Expr::TupleStructLit { fields, .. } => fields.iter().all(is_pure_expr),
        Expr::ArrayLit(values) | Expr::VecLit(values) => values.iter().all(is_pure_expr),
        Expr::ArrayRepeat { elem, .. } => is_pure_expr(elem),
        _ => false,
    }
}
