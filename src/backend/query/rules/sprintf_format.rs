use crate::backend::rust_ast::{Expr, Ident, IndentStmt, Path, Prim, RustValue, Stmt, Type};
use crate::backend::trace::Pass;
use crate::function_identity::{CallBinding, Known, known_call};

use super::super::{
    BindingAccess, BindingRef, CallRecord, CallTarget, EditSet, ExprSite, ExpressionRef, Field,
    FnCall, ItemCaseContext, Predicate, QueryRule, Rejection, RejectionReason, StatementRef,
    sprintf_format_expr, sprintf_worst_case_len,
};

use super::printf_format::replace_int_returning_call;

pub(in crate::backend) fn calls() -> QueryRule<FnCall> {
    QueryRule::new(
        Pass::SprintfFormat,
        "rewrite_sprintf_buffer_to_string",
        FnCall {
            target: Field::predicate(|target: &CallTarget, _| {
                matches!(target, CallTarget::Known(Known::SPrintf | Known::SNPrintf))
            }),
            ..Default::default()
        },
    )
    .case("constant_format", rewrite_case)
}

fn rewrite_case(
    case: &mut ItemCaseContext<'_, '_>,
    call: &CallRecord,
) -> Result<EditSet, Rejection> {
    let CallTarget::Known(known) = call.target else {
        return Err(case.reject());
    };
    let format_index = if known == Known::SNPrintf { 2 } else { 1 };

    let dst_site = call.args.first().ok_or_else(|| case.reject())?.clone();
    let buf_var_site = resolve_pointer_view_var(case, &dst_site)?;
    let buf_binding = case.fact(|query| {
        query.expression_binding(&ExpressionRef {
            site: buf_var_site.clone(),
        })
    })?;
    let (capacity, buf_mutable) = zero_buffer_declaration(case, &buf_binding)?;
    let size_bound = if known == Known::SNPrintf {
        let size_site = call.args.get(1).ok_or_else(|| case.reject())?.clone();
        resolve_const_u64(case, &size_site)?
    } else {
        capacity
    };

    let fill_target = call
        .trivial_unsafe_site
        .clone()
        .unwrap_or_else(|| call.site.clone());
    let fill_stmt = case.fact(|query| {
        query.enclosing_statement(&ExpressionRef {
            site: fill_target.clone(),
        })
    })?;
    let fill_stmt_root = case.fact(|query| query.statement_expression(&fill_stmt, 0))?;
    case.require(fill_stmt_root.site == fill_target)?;
    let nested_fill =
        require_no_buffer_use_before_fill(case, &buf_binding, &buf_var_site, &fill_stmt)?;

    let fact = case.fact(|query| query.printf_call_at(&call.site))?;
    let rest = call
        .args
        .get(format_index + 1..)
        .ok_or_else(|| case.reject())?
        .iter()
        .map(|site| case.expr(site).cloned())
        .collect::<Option<Vec<_>>>()
        .ok_or_else(|| case.reject())?;
    case.require_at(
        fact.arg_paths.len() == rest.len(),
        Predicate::PrintfCall,
        &call.site,
    )?;
    let format = fact.format.as_ref().ok_or_else(|| {
        case.reject_at(
            Predicate::PrintfCall,
            &call.site,
            RejectionReason::UnsupportedShape,
        )
    })?;
    let worst_case_len = sprintf_worst_case_len(format, &fact.arg_facts).ok_or_else(|| {
        case.reject_at(
            Predicate::PrintfCall,
            &call.site,
            RejectionReason::UnsupportedShape,
        )
    })?;
    case.require((worst_case_len as u64) < size_bound)?;
    let format_expr = sprintf_format_expr(format, &rest, &fact.arg_facts).ok_or_else(|| {
        case.reject_at(
            Predicate::PrintfCall,
            &call.site,
            RejectionReason::UnsupportedShape,
        )
    })?;

    let mut edits = EditSet::new();
    if nested_fill {
        replace_nested_fill(case, &buf_binding, &fill_stmt, format_expr, &mut edits)?;
    } else {
        edits.push_replace_statement(buf_binding.item_index, buf_binding.definition.clone(), None);
        replace_fill_call_with_declaration(
            case,
            &buf_binding,
            buf_mutable,
            &fill_stmt,
            format_expr,
            &mut edits,
        )?;
    }
    reconcile_other_uses(case, &buf_binding, &buf_var_site, &mut edits)?;
    Ok(edits)
}

fn replace_nested_fill<'db>(
    case: &mut ItemCaseContext<'_, '_>,
    buf_binding: &BindingRef<'db>,
    fill_stmt: &StatementRef,
    format_expr: Expr,
    edits: &mut EditSet,
) -> Result<(), Rejection> {
    edits.push_replace_statement(
        buf_binding.item_index,
        buf_binding.definition.clone(),
        Some(Stmt::Let {
            name: buf_binding.name.clone(),
            mutable: true,
            ty: Some(Type::Custom("String".into())),
            init: Some(Expr::Call {
                func: Box::new(Expr::Path(Path::new(["String", "new"].map(Ident::from)))),
                args: Vec::new(),
                binding: CallBinding::Generated,
            }),
        }),
    );
    let indent = case.fact(|query| query.statement(fill_stmt))?;
    if let Stmt::Let { .. } = &indent.stmt {
        let binding = case.fact(|query| query.statement_binding(fill_stmt))?;
        let uses = case.fact(|query| query.binding_uses(&binding))?;
        case.require(uses.uses.is_empty())?;
    } else {
        case.require(matches!(indent.stmt, Stmt::Expr(_)))?;
    }
    edits.push_replace_statement(
        fill_stmt.item_index,
        fill_stmt.path.clone(),
        Some(Stmt::Assign {
            target: Expr::Var(buf_binding.name.clone().into()),
            value: format_expr,
        }),
    );
    Ok(())
}

fn replace_fill_call_with_declaration<'db>(
    case: &mut ItemCaseContext<'_, '_>,
    buf_binding: &BindingRef<'db>,
    buf_mutable: bool,
    fill_stmt: &StatementRef,
    format_expr: Expr,
    edits: &mut EditSet,
) -> Result<(), Rejection> {
    let indent = case.fact(|query| query.statement(fill_stmt))?;
    let depth = indent.depth;
    let buf_let = Stmt::Let {
        name: buf_binding.name.clone(),
        mutable: buf_mutable,
        ty: Some(Type::Custom("String".into())),
        init: Some(format_expr),
    };

    match &indent.stmt {
        Stmt::Expr(_) => {
            edits.push_replace_statement(
                fill_stmt.item_index,
                fill_stmt.path.clone(),
                Some(buf_let),
            );
        }
        Stmt::Let {
            name, mutable, ty, ..
        } => {
            let binding = case.fact(|query| query.statement_binding(fill_stmt))?;
            let uses = case.fact(|query| query.binding_uses(&binding))?;
            let used = uses.uses.iter().any(|usage| {
                matches!(usage.access, BindingAccess::Read | BindingAccess::ReadWrite)
            });
            if used {
                let ret_let = Stmt::Let {
                    name: name.clone(),
                    mutable: *mutable,
                    ty: ty.clone(),
                    init: Some(Expr::Cast {
                        expr: Box::new(Expr::MethodCall {
                            recv: Box::new(Expr::Var(buf_binding.name.clone().into())),
                            method: "len".into(),
                            args: Vec::new(),
                        }),
                        ty: ty.clone().unwrap_or(Type::Prim(Prim::I32)),
                    }),
                };
                edits.extend(EditSet::replace_statements(
                    fill_stmt.range(),
                    vec![
                        IndentStmt {
                            depth,
                            stmt: buf_let,
                        },
                        IndentStmt {
                            depth,
                            stmt: ret_let,
                        },
                    ],
                ));
            } else {
                edits.push_replace_statement(
                    fill_stmt.item_index,
                    fill_stmt.path.clone(),
                    Some(buf_let),
                );
            }
        }
        _ => return Err(case.reject()),
    }
    Ok(())
}

fn require_no_buffer_use_before_fill<'db>(
    case: &mut ItemCaseContext<'_, '_>,
    buf_binding: &BindingRef<'db>,
    fill_var_site: &ExprSite,
    fill_stmt: &StatementRef,
) -> Result<bool, Rejection> {
    let decl_stmt = StatementRef {
        item_index: buf_binding.item_index,
        path: buf_binding.definition.clone(),
    };
    let nested_fill = decl_stmt.container() != fill_stmt.container();
    let between = if nested_fill {
        Vec::new()
    } else {
        case.fact(|query| query.statements_between(&decl_stmt, fill_stmt))?
    };
    let uses = case.fact(|query| query.binding_uses(buf_binding))?;
    for usage in &uses.uses {
        let Some(expr_ref) = usage.expression() else {
            return Err(case.reject());
        };
        if expr_ref.site == *fill_var_site {
            continue;
        }
        let usage_stmt = case.fact(|query| query.enclosing_statement(expr_ref))?;
        if nested_fill {
            case.require(usage_stmt.container() == fill_stmt.container())?;
            case.require(usage_stmt.index() > fill_stmt.index())?;
        } else {
            case.require(!between.iter().any(|stmt| stmt.path == usage_stmt.path))?;
        }
    }
    Ok(nested_fill)
}

fn reconcile_other_uses<'db>(
    case: &mut ItemCaseContext<'_, '_>,
    buf_binding: &BindingRef<'db>,
    fill_var_site: &ExprSite,
    edits: &mut EditSet,
) -> Result<(), Rejection> {
    let uses = case.fact(|query| query.binding_uses(buf_binding))?;
    for usage in &uses.uses {
        let Some(expr_ref) = usage.expression() else {
            return Err(case.reject());
        };
        if expr_ref.site == *fill_var_site {
            continue;
        }
        case.require(usage.access == BindingAccess::Read)?;
        let (call_site, slot) = climb_to_call_argument(case, &expr_ref.site)?;
        let called = case
            .expr(&call_site)
            .and_then(known_call)
            .ok_or_else(|| case.reject())?;
        match called {
            Known::Puts if slot == 0 => {
                let puts_call = case.fact(|query| {
                    query.expression_call(&ExpressionRef {
                        site: call_site.clone(),
                    })
                })?;
                let puts_target = puts_call
                    .trivial_unsafe_site
                    .clone()
                    .unwrap_or_else(|| puts_call.site.clone());
                let println_edits = replace_int_returning_call(
                    case,
                    puts_target,
                    println_buf_call(&buf_binding.name),
                )?;
                edits.extend(println_edits);
            }
            Known::Printf | Known::FPrintf => {}
            _ => return Err(case.reject()),
        }
    }
    Ok(())
}

fn println_buf_call(name: &str) -> Expr {
    Expr::Macro {
        name: "println".into(),
        args: vec![Expr::Str("{}".into()), Expr::Var(name.into())],
    }
}

fn climb_to_call_argument(
    case: &mut ItemCaseContext<'_, '_>,
    site: &ExprSite,
) -> Result<(ExprSite, usize), Rejection> {
    let mut current = ExpressionRef { site: site.clone() };
    for _ in 0..6 {
        if let Ok(position) = case.fact(|query| query.argument_position(&current)) {
            let (call_ref, slot) = position;
            return Ok((call_ref.site, slot));
        }
        let parent = case.fact(|query| query.parent_expression(&current))?;
        let advances = match case.expr(&parent.site) {
            Some(Expr::Cast { .. } | Expr::ArrayPtr { .. }) => true,
            Some(Expr::MethodCall { method, args, .. }) => {
                args.is_empty() && matches!(method.as_str(), "as_ptr" | "as_mut_ptr")
            }
            _ => false,
        };
        if !advances {
            return Err(case.reject());
        }
        current = parent;
    }
    Err(case.reject())
}

fn resolve_pointer_view_var(
    case: &mut ItemCaseContext<'_, '_>,
    site: &ExprSite,
) -> Result<ExprSite, Rejection> {
    let mut current = site.clone();
    for _ in 0..6 {
        match case.expr(&current) {
            Some(Expr::Var(_)) => return Ok(current),
            Some(Expr::Cast { .. } | Expr::ArrayPtr { .. }) => {
                current = case
                    .fact(|query| query.expression(&query.child(&current, 0)))?
                    .site;
            }
            Some(Expr::MethodCall { method, args, .. })
                if args.is_empty() && matches!(method.as_str(), "as_ptr" | "as_mut_ptr") =>
            {
                current = case
                    .fact(|query| query.expression(&query.child(&current, 0)))?
                    .site;
            }
            _ => return Err(case.reject()),
        }
    }
    Err(case.reject())
}

fn resolve_const_u64(
    case: &mut ItemCaseContext<'_, '_>,
    site: &ExprSite,
) -> Result<u64, Rejection> {
    let mut current = site.clone();
    for _ in 0..8 {
        if let Some(value) = case.expr(&current).and_then(const_u64) {
            return Ok(value);
        }
        match case.expr(&current) {
            Some(Expr::Cast { .. }) => {
                current = case
                    .fact(|query| query.expression(&query.child(&current, 0)))?
                    .site;
            }
            Some(Expr::Var(_)) => {
                let binding = case.fact(|query| {
                    query.expression_binding(&ExpressionRef {
                        site: current.clone(),
                    })
                })?;
                let initializer = case.fact(|query| query.binding_initializer(&binding))?;
                current = initializer.site;
            }
            _ => return Err(case.reject()),
        }
    }
    Err(case.reject())
}

fn zero_buffer_declaration<'db>(
    case: &mut ItemCaseContext<'_, '_>,
    buf_binding: &BindingRef<'db>,
) -> Result<(u64, bool), Rejection> {
    let def_stmt = StatementRef {
        item_index: buf_binding.item_index,
        path: buf_binding.definition.clone(),
    };
    let indent = case.fact(|query| query.statement(&def_stmt))?;
    let Stmt::Let {
        mutable,
        ty: Some(ty),
        init: Some(init),
        ..
    } = &indent.stmt
    else {
        return Err(case.reject());
    };
    case.require(is_zero_buffer_init(init))?;
    let capacity = buffer_capacity(ty).ok_or_else(|| case.reject())?;
    Ok((capacity, *mutable))
}

fn buffer_capacity(ty: &Type) -> Option<u64> {
    match ty {
        Type::Array { len, .. } => Some(*len),
        Type::Generic { args, .. } => args.iter().find_map(buffer_capacity),
        _ => None,
    }
}

fn const_u64(expr: &Expr) -> Option<u64> {
    match expr {
        Expr::Value(RustValue::I64(n)) => u64::try_from(*n).ok(),
        Expr::Value(RustValue::I128(n)) => u64::try_from(*n).ok(),
        Expr::Value(RustValue::U128(n)) => u64::try_from(*n).ok(),
        Expr::Value(RustValue::Usize(n)) => Some(*n as u64),
        Expr::Cast { expr, .. } => const_u64(expr),
        _ => None,
    }
}

fn is_zero_buffer_init(expr: &Expr) -> bool {
    match expr {
        Expr::ArrayRepeat { elem, .. } => is_zero_value(elem),
        Expr::TupleStructLit { fields, .. } if fields.len() == 1 => is_zero_buffer_init(&fields[0]),
        _ => false,
    }
}

fn is_zero_value(expr: &Expr) -> bool {
    matches!(
        expr,
        Expr::Value(
            RustValue::I64(0) | RustValue::I128(0) | RustValue::U128(0) | RustValue::Usize(0)
        )
    )
}
