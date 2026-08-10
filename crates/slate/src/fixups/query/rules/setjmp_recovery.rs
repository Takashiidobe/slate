use std::collections::BTreeSet;

use crate::fixups::support::walk;
use crate::fixups::trace::Pass;
use crate::function_identity::CallBinding;
use crate::rust_ast::{
    BinOp, Block, Expr, ExprMatchArm, Ident, IndentStmt, Item, MatchArm, Path, Pattern, Prim,
    Program, RustValue, Stmt, StructDef, StructFields, Type, Visibility,
};

use super::super::{
    EditSet, Predicate, Proof, QueryContext, QueryResult, QueryRule, Rejection, RejectionReason,
    WholeProgram,
};

pub(in crate::fixups) fn program() -> QueryRule<WholeProgram> {
    QueryRule::new(
        Pass::SetjmpRecovery,
        "recover_setjmp_catch_unwind",
        WholeProgram::when(has_setjmp_recovery_candidates),
    )
    .case("setjmp_longjmp_idiom", |case, program| {
        let rewrite = case.fact(|query| rewrite_setjmp_recovery(query))?;
        Ok(EditSet::replace_program(
            program.clone(),
            rewrite.replacement,
            Vec::new(),
        ))
    })
}

fn has_setjmp_recovery_candidates(query: &QueryContext<'_>) -> bool {
    query
        .snapshot_program()
        .items
        .iter()
        .any(|item| match item {
            Item::Fn(f) => body_has_setjmp_guard(&f.body),
            _ => false,
        })
}

struct SetjmpRecoveryRewrite {
    replacement: Program,
}

fn rewrite_setjmp_recovery(query: &QueryContext<'_>) -> QueryResult<SetjmpRecoveryRewrite> {
    let mut replacement = query.snapshot_program().clone();
    let mut buffers = BTreeSet::new();
    let mut any_guard = false;
    for item in replacement.items.iter_mut() {
        if let Item::Fn(f) = item {
            any_guard |= rewrite_setjmp_guards_in_body(&mut f.body, true, &mut buffers);
        }
    }
    if !any_guard {
        return Err(Rejection::new(
            Predicate::SetjmpRecovery,
            None,
            RejectionReason::Contradicted,
            Vec::new(),
        ));
    }
    for item in replacement.items.iter_mut() {
        if let Item::Fn(f) = item {
            rewrite_longjmp_calls_in_body(&mut f.body, &buffers);
        }
    }
    for buffer in &buffers {
        replacement.items.push(payload_struct_item(buffer));
    }
    Ok(Proof::new(
        SetjmpRecoveryRewrite { replacement },
        Vec::new(),
    ))
}

fn body_has_setjmp_guard(body: &[IndentStmt]) -> bool {
    body.iter()
        .any(|indent| stmt_has_setjmp_guard(&indent.stmt))
}

fn stmt_has_setjmp_guard(stmt: &Stmt) -> bool {
    if let Stmt::Scope { body } = stmt
        && setjmp_guard_shape(body).is_some()
    {
        return true;
    }
    let mut found = false;
    walk::nested_bodies_with_path(stmt, &mut Vec::new(), &mut |nested, _| {
        found |= body_has_setjmp_guard(nested);
    });
    found
}

fn setjmp_guard_shape(body: &[IndentStmt]) -> Option<(String, String, Vec<IndentStmt>)> {
    let [let_indent, if_indent] = body else {
        return None;
    };
    let Stmt::Let {
        name: binding_name,
        init: Some(init),
        ..
    } = &let_indent.stmt
    else {
        return None;
    };
    let buffer = setjmp_call_buffer(init)?;
    let Stmt::If {
        cond,
        then_body,
        else_body,
    } = &if_indent.stmt
    else {
        return None;
    };
    if !else_body.is_empty() || !cond_matches_binding(cond, binding_name) {
        return None;
    }
    Some((buffer.to_string(), binding_name.clone(), then_body.clone()))
}

fn setjmp_call_buffer(expr: &Expr) -> Option<&str> {
    let Expr::Unsafe(block) = expr else {
        return None;
    };
    if !block.stmts.is_empty() {
        return None;
    }
    let Expr::Call { func, args, .. } = block.tail.as_deref()? else {
        return None;
    };
    if !matches!(func.as_ref(), Expr::Var(name) if name.as_str() == "setjmp") {
        return None;
    }
    let [buf_arg] = args.as_slice() else {
        return None;
    };
    resolve_address_root(buf_arg)
}

fn cond_matches_binding(cond: &Expr, binding: &str) -> bool {
    matches!(
        cond,
        Expr::Binary { op: BinOp::Ne, lhs, rhs }
            if matches!(lhs.as_ref(), Expr::Var(name) if name.as_str() == binding)
                && matches!(rhs.as_ref(), Expr::Value(RustValue::I64(0)))
    )
}

fn resolve_address_root(expr: &Expr) -> Option<&str> {
    match expr {
        Expr::Var(name) => Some(name.as_str()),
        Expr::Cast { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::Unary { expr, .. } => resolve_address_root(expr),
        Expr::MethodCall { recv, .. } | Expr::MethodCallGeneric { recv, .. } => {
            resolve_address_root(recv)
        }
        _ => None,
    }
}

fn body_has_disallowed_control_flow(body: &[IndentStmt]) -> bool {
    body.iter()
        .any(|indent| stmt_has_disallowed_control_flow(&indent.stmt))
}

fn stmt_has_disallowed_control_flow(stmt: &Stmt) -> bool {
    if matches!(stmt, Stmt::Return(_) | Stmt::Break(_) | Stmt::Continue(_)) {
        return true;
    }
    let mut found = false;
    walk::nested_bodies_with_path(stmt, &mut Vec::new(), &mut |nested, _| {
        found |= body_has_disallowed_control_flow(nested);
    });
    found
}

fn payload_type_name(buffer: &str) -> String {
    format!("__SlateJmpPayload_{buffer}")
}

fn payload_struct_item(buffer: &str) -> Item {
    Item::Struct(StructDef {
        attrs: Vec::new(),
        vis: Visibility::Private,
        field_vis: Visibility::Private,
        generics: Vec::new(),
        name: payload_type_name(buffer),
        fields: StructFields::Named(vec![("value".to_string(), Type::Prim(Prim::I32))]),
    })
}

fn std_path(segments: &[&str]) -> Expr {
    Expr::Path(Path::new(segments.iter().map(|s| Ident::from(*s))))
}

fn stmt_indent(stmt: Stmt) -> IndentStmt {
    IndentStmt { depth: 1, stmt }
}

fn rewrite_setjmp_guards_in_body(
    body: &mut Vec<IndentStmt>,
    is_function_top_level: bool,
    buffers: &mut BTreeSet<String>,
) -> bool {
    let mut changed = false;
    for indent in body.iter_mut() {
        let mut path = Vec::new();
        walk::nested_body_vecs_mut_with_path(&mut indent.stmt, &mut path, &mut |nested, _| {
            changed |= rewrite_setjmp_guards_in_body(nested, false, buffers);
        });
    }

    while let Some(guard_index) = body.iter().position(
        |indent| matches!(&indent.stmt, Stmt::Scope { body } if setjmp_guard_shape(body).is_some()),
    ) {
        let is_last = guard_index == body.len() - 1;
        if is_last && !is_function_top_level {
            break;
        }
        let Stmt::Scope { body: guard_body } = &body[guard_index].stmt else {
            unreachable!()
        };
        let Some((buffer, binding_name, recovery_body)) = setjmp_guard_shape(guard_body) else {
            unreachable!()
        };

        let mut closure_body = body[guard_index + 1..].to_vec();
        let mut strip_trailing_return = false;
        if is_function_top_level
            && matches!(
                closure_body.last().map(|indent| &indent.stmt),
                Some(Stmt::Return(None))
            )
        {
            strip_trailing_return = true;
        }
        let checked_len = closure_body.len() - usize::from(strip_trailing_return);
        if body_has_disallowed_control_flow(&closure_body[..checked_len]) {
            break;
        }
        if strip_trailing_return {
            closure_body.pop();
        }

        body.truncate(guard_index);
        let payload_name = payload_type_name(&buffer);
        buffers.insert(buffer);

        let result_name = format!("__sj_{binding_name}");
        let payload_var = format!("__sj_payload_{binding_name}");

        let catch_unwind_stmt = stmt_indent(Stmt::Let {
            name: result_name.clone(),
            mutable: false,
            ty: None,
            init: Some(Expr::Call {
                func: Box::new(std_path(&["std", "panic", "catch_unwind"])),
                args: vec![Expr::Call {
                    func: Box::new(std_path(&["std", "panic", "AssertUnwindSafe"])),
                    args: vec![Expr::Closure {
                        params: Vec::new(),
                        body: Box::new(Expr::Block(Box::new(Block {
                            stmts: closure_body,
                            tail: None,
                        }))),
                    }],
                    binding: CallBinding::Generated,
                }],
                binding: CallBinding::Generated,
            }),
        });

        let downcast_match = Expr::Match {
            expr: Box::new(Expr::MethodCallGeneric {
                recv: Box::new(Expr::Var(payload_var.clone().into())),
                method: "downcast".into(),
                type_args: vec![Type::Custom(payload_name)],
                args: Vec::new(),
            }),
            arms: vec![
                ExprMatchArm {
                    pattern: Pattern::TupleStruct {
                        name: "Ok".into(),
                        fields: vec![Pattern::Binding("__sj_p".into())],
                    },
                    value: Expr::Field {
                        base: Box::new(Expr::Var("__sj_p".into())),
                        field: "value".into(),
                    },
                },
                ExprMatchArm {
                    pattern: Pattern::TupleStruct {
                        name: "Err".into(),
                        fields: vec![Pattern::Binding("__sj_other".into())],
                    },
                    value: Expr::Call {
                        func: Box::new(std_path(&["std", "panic", "resume_unwind"])),
                        args: vec![Expr::Var("__sj_other".into())],
                        binding: CallBinding::Generated,
                    },
                },
            ],
        };

        let mut err_arm_body = vec![stmt_indent(Stmt::Let {
            name: binding_name,
            mutable: false,
            ty: Some(Type::Prim(Prim::I32)),
            init: Some(downcast_match),
        })];
        err_arm_body.extend(recovery_body);

        let match_stmt = stmt_indent(Stmt::Match {
            expr: Expr::Var(result_name.into()),
            arms: vec![
                MatchArm {
                    pattern: Pattern::TupleStruct {
                        name: "Ok".into(),
                        fields: vec![Pattern::Wildcard],
                    },
                    body: Vec::new(),
                },
                MatchArm {
                    pattern: Pattern::TupleStruct {
                        name: "Err".into(),
                        fields: vec![Pattern::Binding(payload_var.into())],
                    },
                    body: err_arm_body,
                },
            ],
        });

        body.push(catch_unwind_stmt);
        body.push(match_stmt);
        changed = true;
    }
    changed
}

fn rewrite_longjmp_calls_in_body(body: &mut [IndentStmt], buffers: &BTreeSet<String>) -> bool {
    let mut changed = false;
    for indent in body.iter_mut() {
        if let Some(new_stmt) = longjmp_rewrite(&indent.stmt, buffers) {
            indent.stmt = new_stmt;
            changed = true;
        }
        let mut path = Vec::new();
        walk::nested_body_vecs_mut_with_path(&mut indent.stmt, &mut path, &mut |nested, _| {
            changed |= rewrite_longjmp_calls_in_body(nested, buffers);
        });
    }
    changed
}

fn longjmp_rewrite(stmt: &Stmt, buffers: &BTreeSet<String>) -> Option<Stmt> {
    let Stmt::Expr(Expr::Unsafe(block)) = stmt else {
        return None;
    };
    if !block.stmts.is_empty() {
        return None;
    }
    let Expr::Call { func, args, .. } = block.tail.as_deref()? else {
        return None;
    };
    if !matches!(func.as_ref(), Expr::Var(name) if name.as_str() == "longjmp") {
        return None;
    }
    let [buf_arg, val_arg] = args.as_slice() else {
        return None;
    };
    let buffer = resolve_address_root(buf_arg)?;
    if !buffers.contains(buffer) {
        return None;
    }
    Some(Stmt::Expr(Expr::Call {
        func: Box::new(std_path(&["std", "panic", "panic_any"])),
        args: vec![Expr::StructLit {
            name: payload_type_name(buffer),
            fields: vec![("value".to_string(), val_arg.clone())],
        }],
        binding: CallBinding::Generated,
    }))
}
