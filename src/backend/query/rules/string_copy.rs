use crate::backend::facts::{PathSegment, StringBufferProvenance, StringRecoveryCandidate};
use crate::backend::rust_ast::{Expr, Stmt, Type};
use crate::backend::trace::Pass;

use super::super::{
    Binding, BindingCategory, BindingRef, EditSet, Field, ItemCaseContext, QueryRule, Rejection,
    StatementRef, StringCopyAction, same_statement_container,
};

pub(in crate::backend) fn rewrite() -> QueryRule<Binding> {
    QueryRule::new(
        Pass::StringCopy,
        "rewrite_string_copy_idioms",
        Binding {
            kind: Field::eq(BindingCategory::Local),
            ..Default::default()
        },
    )
    .case("owned_string", rewrite_case)
}

fn rewrite_case<'db>(
    case: &mut ItemCaseContext<'_, '_>,
    binding: &BindingRef<'db>,
) -> Result<EditSet, Rejection> {
    let buffer = case.fact(|query| query.string_buffer(&binding.value_site()))?;
    case.require(
        buffer
            .candidates
            .contains(&StringRecoveryCandidate::OwnedString),
    )?;
    let init = match &buffer.provenance {
        StringBufferProvenance::ZeroInitialized => String::new(),
        StringBufferProvenance::Literal | StringBufferProvenance::AssignedLiteral { .. } => {
            let bytes = buffer.bytes.clone().ok_or_else(|| case.reject())?;
            String::from_utf8(bytes).map_err(|_| case.reject())?
        }
        _ => return Err(case.reject()),
    };

    let def_path = binding.definition.clone();
    let def_statement = case.fact(|query| {
        query.statement(&StatementRef {
            item_index: binding.item_index,
            path: def_path.clone(),
        })
    })?;
    let Stmt::Let { mutable, .. } = &def_statement.stmt else {
        return Err(case.reject());
    };
    let mutable = *mutable;
    let assignment_path = match &buffer.provenance {
        StringBufferProvenance::AssignedLiteral { assignment } => Some(assignment.clone()),
        _ => None,
    };

    let copy_sites =
        case.fact(|query| query.string_copy_rewrite_sites(&binding.value_site(), &binding.name))?;
    let copy_paths = copy_sites
        .iter()
        .map(|site| site.statement.path.0.clone())
        .collect::<Vec<_>>();
    let within_copy_site =
        |path: &[PathSegment]| copy_paths.iter().any(|prefix| path.starts_with(prefix));

    let uses = case.fact(|query| query.value_uses(&binding.value_site(), &binding.name))?;
    let pointer_views =
        case.fact(|query| query.string_pointer_view_sites(&binding.value_site(), &binding.name))?;
    for site in uses.iter().chain(pointer_views.iter()) {
        if site.path == def_path
            || assignment_path.as_ref() == Some(&site.path)
            || within_copy_site(&site.path.0)
        {
            continue;
        }
        let allowed = case.fact(|query| {
            query.string_use_allows_lift(
                &binding.value_site(),
                &binding.name,
                site,
                StringRecoveryCandidate::OwnedString,
            )
        })?;
        case.require(allowed)?;
    }

    let mut edits = EditSet::new();
    edits.push_replace_statement(
        binding.item_index,
        def_path.clone(),
        Some(Stmt::Let {
            name: binding.name.clone(),
            mutable,
            ty: Some(Type::Custom("String".into())),
            init: Some(to_owned(Expr::Str(init))),
        }),
    );
    if let Some(assignment) = &assignment_path
        && same_statement_container(&def_path, assignment)
    {
        edits.push_replace_statement(binding.item_index, assignment.clone(), None);
    }
    for site in &copy_sites {
        edits.push_replace_statement(
            binding.item_index,
            site.statement.path.clone(),
            Some(copy_replacement_stmt(&binding.name, &site.action)),
        );
    }

    let mut skip_prefix: Option<Vec<PathSegment>> = None;
    for site in case.fact(|query| query.all_exprs(binding.item_index))? {
        if skip_prefix
            .as_ref()
            .is_some_and(|prefix| site.path.0.starts_with(prefix.as_slice()))
            || within_copy_site(&site.path.0)
        {
            continue;
        }
        if let Some(rewritten) = case
            .expr(&site)
            .and_then(|expr| rewrite_pointer_view_expr(expr, &binding.name))
        {
            skip_prefix = Some(site.path.0.clone());
            edits.push_replace_expression(site, rewritten);
        }
    }
    Ok(edits)
}

fn copy_replacement_stmt(dst: &str, action: &StringCopyAction) -> Stmt {
    match action {
        StringCopyAction::AssignLiteral(text) => Stmt::Assign {
            target: Expr::Var(dst.into()),
            value: to_owned(Expr::Str(text.clone())),
        },
        StringCopyAction::AssignOwned(source) => Stmt::Assign {
            target: Expr::Var(dst.into()),
            value: to_owned(as_str_call(&source.name)),
        },
        StringCopyAction::PushLiteral(text) => {
            Stmt::Expr(push_str_call(dst, Expr::Str(text.clone())))
        }
        StringCopyAction::PushOwned(source) => {
            Stmt::Expr(push_str_call(dst, as_str_call(&source.name)))
        }
    }
}

fn as_str_call(name: &str) -> Expr {
    Expr::MethodCall {
        recv: Box::new(Expr::Var(name.into())),
        method: "as_str".into(),
        args: Vec::new(),
    }
}

fn push_str_call(dst: &str, value: Expr) -> Expr {
    Expr::MethodCall {
        recv: Box::new(Expr::Var(dst.into())),
        method: "push_str".into(),
        args: vec![value],
    }
}

fn to_owned(expr: Expr) -> Expr {
    Expr::MethodCall {
        recv: Box::new(expr),
        method: "to_owned".into(),
        args: Vec::new(),
    }
}

fn rewrite_pointer_view_expr(expr: &Expr, name: &str) -> Option<Expr> {
    (pointer_view_source(expr)? == name).then(|| Expr::Var(name.into()))
}

fn pointer_view_source(expr: &Expr) -> Option<&str> {
    match expr {
        Expr::MethodCall { recv, method, args }
            if args.is_empty() && matches!(method.as_str(), "as_ptr" | "as_mut_ptr") =>
        {
            match &**recv {
                Expr::Var(v) => Some(v.as_str()),
                _ => None,
            }
        }
        Expr::ArrayPtr { array, .. } => match &**array {
            Expr::Var(v) => Some(v.as_str()),
            _ => None,
        },
        Expr::Cast { expr, .. }
        | Expr::Unary { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => pointer_view_source(expr),
        _ => None,
    }
}
