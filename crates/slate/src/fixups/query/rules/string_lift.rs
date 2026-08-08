use crate::fixups::facts::{
    PathSegment, StringBufferFact, StringBufferProvenance, StringRecoveryCandidate,
};
use crate::fixups::trace::Pass;
use crate::rust_ast::{Expr, Prim, Stmt, Type};

use super::super::{
    Binding, BindingCategory, BindingRef, EditSet, Field, ItemCaseContext, QueryRule, Rejection,
    same_statement_container,
};

pub(in crate::fixups) fn rewrite() -> QueryRule<Binding> {
    QueryRule::new(
        Pass::StringLift,
        "lift_string_buffer",
        Binding {
            kind: Field::eq(BindingCategory::Local),
            ..Default::default()
        },
    )
    .case("borrowed_str", |case, binding| {
        let buffer = case.fact(|query| query.string_buffer(&binding.value_site()))?;
        let bytes = buffer.bytes.clone().ok_or_else(|| case.reject())?;
        let text = String::from_utf8(bytes).map_err(|_| case.reject())?;
        lift(
            case,
            binding,
            &buffer,
            StringRecoveryCandidate::BorrowedStr,
            str_ref_type(),
            Expr::Str(text),
        )
    })
    .case("borrowed_bytes", |case, binding| {
        let buffer = case.fact(|query| query.string_buffer(&binding.value_site()))?;
        let bytes = buffer.bytes.clone().ok_or_else(|| case.reject())?;
        lift(
            case,
            binding,
            &buffer,
            StringRecoveryCandidate::BorrowedBytes,
            byte_slice_ref_type(),
            Expr::ByteStr(bytes),
        )
    })
}

pub(in crate::fixups) fn rewrite_c_strings() -> QueryRule<Binding> {
    QueryRule::new(
        Pass::StringLiftFixupCStrings,
        "lift_string_buffer",
        Binding {
            kind: Field::eq(BindingCategory::Local),
            ..Default::default()
        },
    )
    .case("borrowed_cstr", |case, binding| {
        let buffer = case.fact(|query| query.string_buffer(&binding.value_site()))?;
        case.require(buffer.ascii_only && !buffer.interior_nul)?;
        let bytes = buffer.bytes.clone().ok_or_else(|| case.reject())?;
        lift(
            case,
            binding,
            &buffer,
            StringRecoveryCandidate::BorrowedCStr,
            cstr_ref_type(),
            Expr::CStr(bytes),
        )
    })
}

fn lift<'db>(
    case: &mut ItemCaseContext<'_, '_>,
    binding: &BindingRef<'db>,
    buffer: &StringBufferFact<'db>,
    recovery: StringRecoveryCandidate,
    ty: Type,
    expr: Expr,
) -> Result<EditSet, Rejection> {
    let def_path = binding.definition.clone();
    let assignment_path = match &buffer.provenance {
        StringBufferProvenance::AssignedLiteral { assignment } => Some(assignment.clone()),
        _ => None,
    };
    let uses = case.fact(|query| query.value_uses(&binding.value_site(), &binding.name))?;
    let pointer_views =
        case.fact(|query| query.string_pointer_view_sites(&binding.value_site(), &binding.name))?;
    for site in uses.iter().chain(pointer_views.iter()) {
        if site.path == def_path || assignment_path.as_ref() == Some(&site.path) {
            continue;
        }
        let allowed = case.fact(|query| {
            query.string_use_allows_lift(&binding.value_site(), &binding.name, site, recovery)
        })?;
        case.require(allowed)?;
    }
    let mut edits = EditSet::new();
    edits.push_replace_statement(
        binding.item_index,
        def_path.clone(),
        Some(Stmt::Let {
            name: binding.name.clone(),
            mutable: false,
            ty: Some(ty),
            init: Some(expr),
        }),
    );
    if let Some(assignment) = &assignment_path
        && same_statement_container(&def_path, assignment)
    {
        edits.push_replace_statement(binding.item_index, assignment.clone(), None);
    }
    let mut skip_prefix: Option<Vec<PathSegment>> = None;
    for site in case.fact(|query| query.all_exprs(binding.item_index))? {
        if skip_prefix
            .as_ref()
            .is_some_and(|prefix| site.path.0.starts_with(prefix.as_slice()))
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

fn rewrite_pointer_view_expr(expr: &Expr, name: &str) -> Option<Expr> {
    match expr {
        Expr::MethodCall { recv, method, args }
            if args.is_empty()
                && matches!(&**recv, Expr::Var(v) if v.as_str() == name)
                && matches!(method.as_str(), "as_ptr" | "as_mut_ptr") =>
        {
            Some(pointer_view_cast(name))
        }
        Expr::ArrayPtr { array, .. } if matches!(&**array, Expr::Var(v) if v.as_str() == name) => {
            Some(pointer_view_cast(name))
        }
        _ => None,
    }
}

fn pointer_view_cast(name: &str) -> Expr {
    Expr::Cast {
        expr: Box::new(Expr::MethodCall {
            recv: Box::new(Expr::Var(name.into())),
            method: "as_ptr".into(),
            args: Vec::new(),
        }),
        ty: Type::Ptr {
            mutable: true,
            inner: Box::new(Type::Prim(Prim::I8)),
        },
    }
}

fn str_ref_type() -> Type {
    Type::Ref {
        mutable: false,
        inner: Box::new(Type::Str),
    }
}

fn byte_slice_ref_type() -> Type {
    Type::Ref {
        mutable: false,
        inner: Box::new(Type::Slice(Box::new(Type::Prim(Prim::U8)))),
    }
}

fn cstr_ref_type() -> Type {
    Type::Ref {
        mutable: false,
        inner: Box::new(Type::Custom("core::ffi::CStr".into())),
    }
}
