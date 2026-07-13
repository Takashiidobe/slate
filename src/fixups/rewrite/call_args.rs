//! Inline single-use argument temps into type-safe call positions.
//!
//! Baseline lowering materializes every call argument as its own `let` temp:
//! `let _v1: i32 = 2; let _v3 = add(_v1, _v2); printf(fmt, _v3)`. The pure-temp
//! inliner ([`super::inline_temps`]) deliberately refuses call-argument slots,
//! because dropping a literal's type annotation into a vararg slot would change
//! its inferred type — `printf(_v0)` with `_v0: i64 = 9223372036854775807` must
//! not become `printf(9223372036854775807)`, where the literal defaults to `i32`.
//!
//! This pass inlines exactly the two positions where the argument's Rust type is
//! pinned by something other than the temp's annotation:
//!
//! - **(a)** a pure temp whose single use is a *declared* parameter slot of a
//!   non-variadic callee — the parameter type pins any literal.
//! - **(b)** a temp initialized by a call to a known function — its Rust type is
//!   fixed by the callee's return type, so inlining is safe even in a vararg
//!   slot; the between-statements guard keeps its side effect from being
//!   reordered.

use std::collections::HashMap;

use crate::fixups::facts::{
    AstPath, CallArgPinning, CallCallee, EffectSubject, FixupFacts, FunctionId, PathSegment, Purity,
};
use crate::fixups::support::walk;
use crate::rust_ast::{Expr, ExternDecl, IndentStmt, Item, Program, Stmt, Type};

pub(in crate::fixups) struct Signature {
    params: Vec<Type>,
    variadic: bool,
}

pub(in crate::fixups) type Signatures = HashMap<String, Signature>;

pub(in crate::fixups) fn collect_signatures(program: &Program) -> Signatures {
    let mut sigs = Signatures::new();
    let mut record = |name: &str, params: Vec<Type>, variadic: bool| {
        sigs.insert(name.to_string(), Signature { params, variadic });
    };
    for item in &program.items {
        match item {
            Item::Fn(f) => record(
                &f.name,
                f.params.iter().map(|p| p.ty.clone()).collect(),
                false,
            ),
            Item::Func(f) => record(
                &f.name,
                f.params.iter().map(|p| p.ty.clone()).collect(),
                false,
            ),
            Item::ExternBlock { decls, .. } => {
                for decl in decls {
                    if let ExternDecl::Fn(d) = decl {
                        record(
                            &d.name,
                            d.params.iter().map(|p| p.ty.clone()).collect(),
                            d.variadic,
                        );
                    }
                }
            }
            _ => {}
        }
    }
    sigs
}

pub(in crate::fixups) fn fixup(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
) -> bool {
    fixup_at(body, function, facts, &mut Vec::new())
}

fn fixup_at(
    body: &mut Vec<IndentStmt>,
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
) -> bool {
    if fixup_nested(body, function, facts, path) {
        return true;
    }
    for i in 0..body.len() {
        let def_path = stmt_path(path, i);
        let Stmt::Let {
            name,
            mutable: false,
            init: Some(init),
            ..
        } = &body[i].stmt
        else {
            continue;
        };
        if !is_temp_name(name) {
            continue;
        }
        let Some(binding) = facts.binding_by_local_path(function, name, &AstPath(def_path.clone()))
        else {
            continue;
        };
        let name = name.clone();
        let init = init.clone();
        let Some((use_index, slot)) = single_arg_use(body, i, binding, function, facts, path)
        else {
            continue;
        };
        let mut arg_path = stmt_path(path, use_index);
        arg_path.push(PathSegment::Expr(slot + 1));
        if !inlinable(function, facts, &def_path, &arg_path) {
            continue;
        }
        if body[use_index].stmt.substitute_var(&name, &init) {
            body.remove(i);
            return true;
        }
    }
    false
}

fn fixup_nested(
    body: &mut [IndentStmt],
    function: FunctionId,
    facts: &FixupFacts,
    path: &mut Vec<PathSegment>,
) -> bool {
    for (index, stmt) in body.iter_mut().enumerate() {
        if walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            match &mut stmt.stmt {
                Stmt::If {
                    then_body,
                    else_body,
                    ..
                }
                | Stmt::LetIf {
                    then_body,
                    else_body,
                    ..
                } => {
                    walk::with_path_segment(path, PathSegment::Then, |path| {
                        fixup_at(then_body, function, facts, path)
                    }) || walk::with_path_segment(path, PathSegment::Else, |path| {
                        fixup_at(else_body, function, facts, path)
                    })
                }
                Stmt::Loop { body, .. } => {
                    walk::with_path_segment(path, PathSegment::LoopBody, |path| {
                        fixup_at(body, function, facts, path)
                    })
                }
                Stmt::Scope { body } => {
                    walk::with_path_segment(path, PathSegment::ScopeBody, |path| {
                        fixup_at(body, function, facts, path)
                    })
                }
                Stmt::LabeledBlock { body, .. } => {
                    walk::with_path_segment(path, PathSegment::LabeledBody, |path| {
                        fixup_at(body, function, facts, path)
                    })
                }
                Stmt::Unsafe { body } => {
                    walk::with_path_segment(path, PathSegment::UnsafeBody, |path| {
                        fixup_at(&mut body.stmts, function, facts, path)
                    })
                }
                _ => false,
            }
        }) {
            return true;
        }
    }
    false
}

/// The single use of `name` after `def_index`, as `(use_index, callee, slot)`,
/// when that use is a top-level argument of a `name`-free call to a plain-ident
/// callee and every statement in between is a pure temp-let (so the definition
/// can move to the use site without crossing a side effect).
fn single_arg_use(
    body: &[IndentStmt],
    def_index: usize,
    binding: crate::fixups::facts::BindingId,
    function: FunctionId,
    facts: &FixupFacts,
    body_path: &[PathSegment],
) -> Option<(usize, usize)> {
    let reads = &facts.def_use(binding)?.reads;
    if reads.len() != 1 {
        return None;
    }
    let use_index = direct_stmt_index(body_path, &reads[0])?;
    if use_index <= def_index || use_index >= body.len() {
        return None;
    }
    let name = binding_name(facts, binding)?;
    let slot = find_arg_slot(&body[use_index].stmt, name)?;
    for index in def_index + 1..use_index {
        if !is_pure_temp_let(
            &body[index].stmt,
            function,
            facts,
            &stmt_path(body_path, index),
        ) {
            return None;
        }
    }
    Some((use_index, slot))
}

fn find_arg_slot(stmt: &Stmt, name: &str) -> Option<usize> {
    let mut result = None;
    walk::stmt_exprs(stmt, &mut |expr| {
        if result.is_some() {
            return;
        }
        if let Expr::Call { func, args } = expr
            && matches!(&**func, Expr::Var(_))
            && let Some(slot) = args
                .iter()
                .position(|arg| matches!(arg, Expr::Var(v) if v.as_str() == name))
        {
            result = Some(slot);
        }
    });
    result
}

fn inlinable(
    function: FunctionId,
    facts: &FixupFacts,
    def_path: &[PathSegment],
    arg_path: &[PathSegment],
) -> bool {
    if facts
        .callsite(function, &AstPath(def_path.to_vec()))
        .is_some_and(|callsite| {
            matches!(
                callsite.callee,
                CallCallee::Direct {
                    signature: Some(_),
                    ..
                }
            )
        })
    {
        return true;
    }
    facts
        .call_arg_at(function, &AstPath(arg_path.to_vec()))
        .is_some_and(|(_, arg)| {
            arg.pinning == CallArgPinning::DeclaredParam
                && !arg.variadic
                && is_pure_expr(function, facts, def_path)
        })
}

fn is_pure_temp_let(
    stmt: &Stmt,
    function: FunctionId,
    facts: &FixupFacts,
    path: &[PathSegment],
) -> bool {
    matches!(stmt, Stmt::Let { name, init: Some(_), .. } if is_temp_name(name))
        && facts.effects.iter().any(|fact| {
            fact.function == function
                && fact.path == AstPath(path.to_vec())
                && fact.subject == EffectSubject::Expr
                && fact.purity == Purity::MovablePure
        })
}

fn is_pure_expr(function: FunctionId, facts: &FixupFacts, path: &[PathSegment]) -> bool {
    facts
        .effect(function, EffectSubject::Expr, &AstPath(path.to_vec()))
        .is_some_and(|fact| fact.purity == Purity::MovablePure)
}

fn is_temp_name(name: &str) -> bool {
    name.strip_prefix("_v")
        .is_some_and(|rest| !rest.is_empty() && rest.bytes().all(|b| b.is_ascii_digit()))
}

fn stmt_path(body_path: &[PathSegment], index: usize) -> Vec<PathSegment> {
    let mut path = body_path.to_vec();
    path.push(PathSegment::Stmt(index));
    path
}

fn direct_stmt_index(body_path: &[PathSegment], read: &AstPath) -> Option<usize> {
    let rest = read.0.strip_prefix(body_path)?;
    match rest {
        [PathSegment::Stmt(index), ..] => Some(*index),
        _ => None,
    }
}

fn binding_name(facts: &FixupFacts, binding: crate::fixups::facts::BindingId) -> Option<&str> {
    facts
        .bindings
        .iter()
        .find(|fact| fact.id == binding)
        .map(|fact| fact.name.as_str())
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{ExternDecl, ExternFnDecl, FnParam, Item, Program, Stmt};

    fn sig(params: &[&str], variadic: bool) -> Signature {
        Signature {
            params: params.iter().map(|t| Type::parse(t)).collect(),
            variadic,
        }
    }

    fn run(sigs: Vec<(&str, Signature)>, stmts: Vec<Stmt>) -> String {
        let mut items: Vec<Item> = sigs
            .into_iter()
            .map(|(name, sig)| Item::ExternBlock {
                abi: "C".into(),
                decls: vec![ExternDecl::Fn(ExternFnDecl {
                    name: name.into(),
                    params: sig
                        .params
                        .into_iter()
                        .enumerate()
                        .map(|(index, ty)| FnParam {
                            name: format!("arg{index}").into(),
                            mutable: false,
                            ty,
                        })
                        .collect(),
                    variadic: sig.variadic,
                    ret: Some(Type::parse("i32")),
                })],
            })
            .collect();
        items.push(Item::Fn(func(vec![], None, stmts)));
        let mut program = Program { items };
        let item_index = program.items.len() - 1;
        loop {
            let analyzed = crate::fixups::facts::analyze(program.clone());
            let facts = analyzed.facts;
            let Item::Fn(f) = &mut program.items[item_index] else {
                unreachable!();
            };
            if !fixup(&mut f.body, FunctionId(0), &facts) {
                break;
            }
        }
        let Item::Fn(f) = &program.items[item_index] else {
            unreachable!();
        };
        emit(f.clone())
    }

    #[test]
    fn inlines_literals_into_params_and_call_result_into_vararg() {
        let out = run(
            vec![
                ("add", sig(&["i32", "i32"], false)),
                ("printf", sig(&["*mut i8"], true)),
            ],
            vec![
                temp("_v1", "i32", int(2)),
                temp("_v2", "i32", int(3)),
                temp("_v3", "i32", call("add", vec![var("_v1"), var("_v2")])),
                Stmt::Expr(call("printf", vec![var("fmt"), var("_v3")])),
            ],
        );

        assert_eq!(
            out,
            "\
fn f() {
    printf(fmt, add(2, 3));
}
"
        );
    }

    #[test]
    fn does_not_inline_bare_literal_into_vararg_slot() {
        let out = run(
            vec![("printf", sig(&["*mut i8"], true))],
            vec![
                temp("_v0", "i64", int(9223372036854775807)),
                Stmt::Expr(call("printf", vec![var("fmt"), var("_v0")])),
            ],
        );

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i64 = 9223372036854775807;
    printf(fmt, _v0);
}
"
        );
    }

    #[test]
    fn does_not_inline_into_unknown_callee() {
        let out = run(
            vec![],
            vec![
                temp("_v0", "i32", int(5)),
                Stmt::Expr(call("mystery", vec![var("_v0")])),
            ],
        );

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i32 = 5;
    mystery(_v0);
}
"
        );
    }

    #[test]
    fn does_not_reorder_call_result_across_side_effect() {
        let out = run(
            vec![("g", sig(&[], false)), ("printf", sig(&["*mut i8"], true))],
            vec![
                temp("_v0", "i32", call("g", vec![])),
                Stmt::Expr(call("side_effect", vec![])),
                Stmt::Expr(call("printf", vec![var("fmt"), var("_v0")])),
            ],
        );

        assert_eq!(
            out,
            "\
fn f() {
    let _v0: i32 = g();
    side_effect();
    printf(fmt, _v0);
}
"
        );
    }
}
