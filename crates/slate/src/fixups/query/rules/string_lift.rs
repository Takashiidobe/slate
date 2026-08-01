use crate::fixups::facts::{
    PathSegment, StringBufferFact, StringBufferProvenance, StringRecoveryCandidate,
};
use crate::fixups::trace::Pass;
use crate::rust_ast::{Expr, Prim, Stmt, Type};

use super::super::{Local, Rejection, ValueCaseContext, ValueEdit, ValueRule, same_container};

pub(in crate::fixups) fn rewrite() -> ValueRule {
    ValueRule::new(Pass::StringLift, "lift_string_buffer", Local::default())
        .case("borrowed_str", |case| {
            let buffer = case.string_buffer()?;
            let bytes = buffer.bytes.clone().ok_or_else(|| case.reject())?;
            let text = String::from_utf8(bytes).map_err(|_| case.reject())?;
            lift(
                case,
                &buffer,
                StringRecoveryCandidate::BorrowedStr,
                str_ref_type(),
                Expr::Str(text),
            )
        })
        .case("borrowed_bytes", |case| {
            let buffer = case.string_buffer()?;
            let bytes = buffer.bytes.clone().ok_or_else(|| case.reject())?;
            lift(
                case,
                &buffer,
                StringRecoveryCandidate::BorrowedBytes,
                byte_slice_ref_type(),
                Expr::ByteStr(bytes),
            )
        })
}

pub(in crate::fixups) fn rewrite_c_strings() -> ValueRule {
    ValueRule::new(
        Pass::StringLiftFixupCStrings,
        "lift_string_buffer",
        Local::default(),
    )
    .case("borrowed_cstr", |case| {
        let buffer = case.string_buffer()?;
        case.require(buffer.ascii_only && !buffer.interior_nul)?;
        let bytes = buffer.bytes.clone().ok_or_else(|| case.reject())?;
        lift(
            case,
            &buffer,
            StringRecoveryCandidate::BorrowedCStr,
            cstr_ref_type(),
            Expr::CStr(bytes),
        )
    })
}

fn lift(
    case: &mut ValueCaseContext<'_, '_>,
    buffer: &StringBufferFact,
    recovery: StringRecoveryCandidate,
    ty: Type,
    expr: Expr,
) -> Result<Vec<ValueEdit>, Rejection> {
    let def_path = case.site();
    let assignment_path = match &buffer.provenance {
        StringBufferProvenance::AssignedLiteral { assignment } => Some(assignment.clone()),
        _ => None,
    };
    let uses = case.uses()?;
    let pointer_views = case.string_pointer_view_sites()?;
    for site in uses.iter().chain(pointer_views.iter()) {
        if site.path == def_path || assignment_path.as_ref() == Some(&site.path) {
            continue;
        }
        let allowed = case.use_allows_string_lift(site, recovery)?;
        case.require(allowed)?;
    }
    let mut edits = vec![ValueEdit::replace_stmt(
        def_path.clone(),
        Stmt::Let {
            name: case.name().to_string(),
            mutable: false,
            ty: Some(ty),
            init: Some(expr),
        },
    )];
    if let Some(assignment) = &assignment_path
        && same_container(&def_path, assignment)
    {
        edits.push(ValueEdit::remove_stmt(assignment.clone()));
    }
    let mut skip_prefix: Option<Vec<PathSegment>> = None;
    for site in case.exprs()? {
        if skip_prefix
            .as_ref()
            .is_some_and(|prefix| site.path.0.starts_with(prefix.as_slice()))
        {
            continue;
        }
        if let Some(rewritten) = case
            .expr(&site)
            .and_then(|expr| rewrite_pointer_view_expr(expr, case.name()))
        {
            skip_prefix = Some(site.path.0.clone());
            edits.push(ValueEdit::replace_expr(site, rewritten));
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
