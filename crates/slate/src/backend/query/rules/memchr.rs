use crate::backend::trace::Pass;
use crate::backend::{facts::Purity, rust_ast::ExprMatchArm};

use super::super::{
    CallTarget, Definition, DefinitionKind, EditSet, Field, FnCall, Predicate, QueryRule,
    StableExpr, byte_position, known_index, pointer_at_or_null,
};
use crate::backend::query::FunctionBodyRecipe;
use crate::backend::query::recipe::{
    call, cast, indent, let_stmt, method, null_mut, path, ptr, unsafe_expr, var, void_ptr,
};
use crate::backend::rust_ast::{BinOp, Expr, Ident, Pattern, Prim, Stmt, Type, UnaryOp};

pub(in crate::backend) fn calls() -> QueryRule<FnCall> {
    QueryRule::new(
        Pass::MemchrPreludeFixupCalls,
        "rewrite_memchr_call",
        FnCall {
            target: Field::eq(CallTarget::Generated("__slate_memchr".into())),
            arity: Field::eq(3),
            ..Default::default()
        },
    )
    .case("known_nul", |case, call| {
        let [source, needle, count] = case.call_args(call);
        let source = case.fact(|query| query.byte_source(&source))?;
        let needle_value = case.fact(|query| query.const_u8(&needle))?;
        case.require_at(needle_value == 0, Predicate::ConstantU8, &needle)?;
        let needle_ref = case.fact(|query| query.expression(&needle))?;
        let needle_effects = case.fact(|query| query.expression_effects(&needle_ref))?;
        case.require(needle_effects.purity == Purity::MovablePure)?;
        let nul = case.fact(|query| query.first_nul(&source))?;
        case.fact(|query| query.prefix_contains(&count, nul))?;
        let replacement =
            case.lower_expr(pointer_at_or_null(source, known_index(nul)), &call.site)?;
        Ok(EditSet::replace_expression(call.site.clone(), replacement))
    })
    .case("byte_position", |case, call| {
        let [source, needle, count] = case.call_args(call);
        let source = case.fact(|query| query.byte_source(&source))?;
        case.fact(|query| query.full_byte_view(&source, &count))?;
        let needle_ref = case.fact(|query| query.expression(&needle))?;
        let needle_effects = case.fact(|query| query.expression_effects(&needle_ref))?;
        case.require(needle_effects.purity == Purity::MovablePure)?;
        let needle = StableExpr {
            site: needle_ref.site,
        };
        let replacement = case.lower_expr(
            pointer_at_or_null(source, byte_position(needle)),
            &call.site,
        )?;
        Ok(EditSet::replace_expression(call.site.clone(), replacement))
    })
}

pub(in crate::backend) fn helper() -> QueryRule<Definition> {
    QueryRule::new(
        Pass::MemchrPrelude,
        "manage_memchr_helper",
        Definition {
            kind: Field::eq(DefinitionKind::Function),
            name: Field::eq("__slate_memchr".into()),
            ..Default::default()
        },
    )
    .case("unused", |case, definition| {
        let users = case.fact(|query| query.definition_users(definition))?;
        case.require_at(users.users == 0, Predicate::ZeroUsers, &users.site)?;
        Ok(EditSet::delete_definition(definition.clone()))
    })
    .case("retained", |case, definition| {
        let function = case.fact(|query| query.definition_function(definition))?;
        case.replace_function_body(function, memchr_fallback_body())
    })
}

pub(crate) fn memchr_fallback_body() -> FunctionBodyRecipe {
    FunctionBodyRecipe {
        body: vec![
            indent(let_stmt(
                "b",
                Some(Type::Prim(Prim::U8)),
                cast(var("c"), Type::Prim(Prim::U8)),
            )),
            indent(let_stmt(
                "bytes",
                Some(ptr(false, Type::Prim(Prim::U8))),
                cast(var("s"), ptr(false, Type::Prim(Prim::U8))),
            )),
            indent(let_stmt(
                "haystack",
                None,
                unsafe_expr(call(
                    path(["std", "slice", "from_raw_parts"]),
                    vec![var("bytes"), var("n")],
                )),
            )),
            indent(Stmt::Return(Some(Expr::Match {
                expr: Box::new(helper_position()),
                arms: vec![
                    ExprMatchArm {
                        pattern: Pattern::TupleStruct {
                            name: Ident::from("Some"),
                            fields: vec![Pattern::Binding(Ident::from("i"))],
                        },
                        value: unsafe_expr(cast(
                            method(var("bytes"), "add", vec![var("i")]),
                            void_ptr(true),
                        )),
                    },
                    ExprMatchArm {
                        pattern: Pattern::Binding(Ident::from("None")),
                        value: null_mut(),
                    },
                ],
            }))),
        ],
    }
}

fn helper_position() -> Expr {
    method(
        method(var("haystack"), "iter", Vec::new()),
        "position",
        vec![Expr::Closure {
            params: vec![Ident::from("x")],
            body: Box::new(Expr::Binary {
                op: BinOp::Eq,
                lhs: Box::new(Expr::Unary {
                    op: UnaryOp::Deref,
                    expr: Box::new(var("x")),
                }),
                rhs: Box::new(var("b")),
            }),
        }],
    )
}
