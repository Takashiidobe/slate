use std::collections::{BTreeMap, BTreeSet};

use crate::fixups::facts::{
    AstPath, BindingId, BindingKind, CallCallee, FixupFacts, FunctionId, PathSegment,
    StringBufferKind, StringLibcFunction, StringParamLiftFact,
};
use crate::rust_ast::{Block, Expr, IndentStmt, Item, Prim, Program, Stmt, Type, Visibility};

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
struct Key {
    function: FunctionId,
    param: BindingId,
}

#[derive(Debug, Clone)]
struct Candidate {
    key: Key,
    function_name: String,
    index: usize,
}

pub(in crate::fixups) fn collect_facts(program: &Program, facts: &mut FixupFacts) {
    facts.string_param_lifts.clear();

    let candidates = collect_candidates(program, facts);
    let mut active = candidates
        .iter()
        .map(|candidate| candidate.key)
        .collect::<BTreeSet<_>>();
    let by_function = candidates
        .iter()
        .map(|candidate| ((candidate.key.function, candidate.index), candidate.key))
        .collect::<BTreeMap<_, _>>();

    loop {
        let before = active.clone();
        active.retain(|key| {
            let Some(candidate) = candidates.iter().find(|candidate| candidate.key == *key) else {
                return false;
            };
            all_uses_allow_lift(candidate, facts, &by_function, &before)
                && all_callers_prove_arg(program, candidate, facts, &before)
        });
        if active == before {
            break;
        }
    }

    facts.string_param_lifts = candidates
        .into_iter()
        .filter(|candidate| active.contains(&candidate.key))
        .map(|candidate| StringParamLiftFact {
            callee: candidate.key.function,
            param: candidate.key.param,
            index: candidate.index,
        })
        .collect();
}

fn collect_candidates(program: &Program, facts: &FixupFacts) -> Vec<Candidate> {
    program
        .items
        .iter()
        .enumerate()
        .filter_map(|(item_index, item)| {
            let Item::Fn(f) = item else {
                return None;
            };
            if f.name == "main" || f.unsafe_ || f.extern_c || !matches!(f.vis, Visibility::Private)
            {
                return None;
            }
            let function = facts.function_by_item_index(item_index)?;
            Some(
                f.params
                    .iter()
                    .enumerate()
                    .filter_map(move |(index, param)| {
                        if !is_char_ptr(&param.ty) {
                            return None;
                        }
                        let binding = facts.binding_by_param_index(function, index)?;
                        Some(Candidate {
                            key: Key {
                                function,
                                param: binding,
                            },
                            function_name: f.name.clone(),
                            index,
                        })
                    }),
            )
        })
        .flatten()
        .collect()
}

fn all_uses_allow_lift(
    candidate: &Candidate,
    facts: &FixupFacts,
    by_function: &BTreeMap<(FunctionId, usize), Key>,
    active: &BTreeSet<Key>,
) -> bool {
    let mut aliases = BTreeSet::from([candidate.key.param]);
    loop {
        let before = aliases.clone();
        for binding in &before {
            let Some(def_use) = facts.def_use(*binding) else {
                return false;
            };
            if !def_use.writes.is_empty() {
                return false;
            }
            for read in &def_use.reads {
                if let Some(alias) = direct_alias_at(candidate.key.function, facts, read) {
                    aliases.insert(alias);
                }
            }
        }
        if aliases == before {
            break;
        }
    }

    aliases.iter().all(|binding| {
        let Some(def_use) = facts.def_use(*binding) else {
            return false;
        };
        def_use.reads.iter().all(|read| {
            direct_alias_at(candidate.key.function, facts, read).is_some()
                || libc_use_allows(candidate.key.function, &aliases, facts, read, active)
                || internal_call_allows(
                    candidate.key.function,
                    &aliases,
                    facts,
                    by_function,
                    active,
                    read,
                )
        })
    })
}

fn libc_use_allows(
    function: FunctionId,
    aliases: &BTreeSet<BindingId>,
    facts: &FixupFacts,
    read: &AstPath,
    active: &BTreeSet<Key>,
) -> bool {
    facts.string_libc_uses.iter().any(|usage| {
        usage.function == function
            && path_starts_with(&read.0, &usage.path.0)
            && supported_string_callee(usage.callee)
            && usage.pointer_args.iter().all(|binding| {
                aliases.contains(binding) || binding_is_liftable_source(facts, *binding, active)
            })
    })
}

fn internal_call_allows(
    function: FunctionId,
    _aliases: &BTreeSet<BindingId>,
    facts: &FixupFacts,
    by_function: &BTreeMap<(FunctionId, usize), Key>,
    active: &BTreeSet<Key>,
    read: &AstPath,
) -> bool {
    facts.callsites.iter().any(|callsite| {
        callsite.function == function
            && matches!(callsite.callee, CallCallee::Direct { .. })
            && path_starts_with(&read.0, &callsite.path.0)
            && callsite.args.iter().any(|arg| {
                paths_overlap(&read.0, &arg.path.0)
                    && direct_callee_function(facts, callsite)
                        .and_then(|callee| by_function.get(&(callee, arg.slot)))
                        .is_some_and(|target| active.contains(target))
            })
    })
}

fn all_callers_prove_arg(
    program: &Program,
    candidate: &Candidate,
    facts: &FixupFacts,
    active: &BTreeSet<Key>,
) -> bool {
    let calls = facts
        .callsites
        .iter()
        .filter(|callsite| match &callsite.callee {
            CallCallee::Direct { name, .. } => name == &candidate.function_name,
            CallCallee::Indirect => false,
        })
        .collect::<Vec<_>>();
    !calls.is_empty()
        && calls.iter().all(|callsite| {
            let Some(arg) = callsite.args.iter().find(|arg| arg.slot == candidate.index) else {
                return false;
            };
            let expr = expr_at_path(program, callsite.function, &arg.path);

            expr.is_some_and(|expr| {
                expr_is_liftable_source(expr, callsite.function, &arg.path, facts, active)
            })
        })
}

fn direct_callee_function(
    facts: &FixupFacts,
    callsite: &crate::fixups::facts::CallsiteFact,
) -> Option<FunctionId> {
    let CallCallee::Direct {
        signature: Some(signature),
        ..
    } = callsite.callee
    else {
        return None;
    };
    match facts.call_signatures.get(signature.0)?.source {
        crate::fixups::facts::CallSignatureSource::Function(function) => Some(function),
        crate::fixups::facts::CallSignatureSource::Extern { .. } => None,
    }
}

fn expr_is_liftable_source(
    expr: &Expr,
    function: FunctionId,
    path: &AstPath,
    facts: &FixupFacts,
    active: &BTreeSet<Key>,
) -> bool {
    match peel_pointer_view(expr) {
        Expr::Var(name) => binding_read_under(facts, function, path)
            .or_else(|| binding_named(facts, function, name.as_str()))
            .is_some_and(|binding| binding_is_liftable_source(facts, binding, active)),
        Expr::CStr(bytes) => c_string_payload(bytes).is_some(),
        _ => false,
    }
}

fn binding_named(facts: &FixupFacts, function: FunctionId, name: &str) -> Option<BindingId> {
    facts
        .bindings
        .iter()
        .rev()
        .find(|binding| binding.function == function && binding.name == name)
        .map(|binding| binding.id)
}

fn binding_is_liftable_source(
    facts: &FixupFacts,
    binding: BindingId,
    active: &BTreeSet<Key>,
) -> bool {
    facts.string_buffer(binding).is_some_and(|buffer| {
        matches!(
            buffer.kind,
            StringBufferKind::BorrowedStr | StringBufferKind::OwnedString
        ) || (buffer.kind == StringBufferKind::CharArray
            && buffer
                .candidates
                .contains(&crate::fixups::facts::StringRecoveryCandidate::BorrowedStr))
    }) || facts.bindings.iter().any(|fact| {
        fact.id == binding
            && matches!(fact.kind, BindingKind::Param { .. })
            && active.contains(&Key {
                function: fact.function,
                param: binding,
            })
    }) || local_aliases_active_param(facts, binding, active)
}

fn local_aliases_active_param(
    facts: &FixupFacts,
    binding: BindingId,
    active: &BTreeSet<Key>,
) -> bool {
    let Some(local) = facts.bindings.iter().find(|fact| fact.id == binding) else {
        return false;
    };
    if local.kind != BindingKind::Local {
        return false;
    }
    active.iter().any(|key| {
        key.function == local.function
            && facts
                .def_use(key.param)
                .is_some_and(|def_use| def_use.reads.iter().any(|read| read == &local.path))
    })
}

fn direct_alias_at(function: FunctionId, facts: &FixupFacts, path: &AstPath) -> Option<BindingId> {
    let alias = facts.local_binding_at(function, path)?;
    let ty = facts.binding_type(alias.id).map(Type::parse)?;
    is_char_ptr(&ty).then_some(alias.id)
}

fn binding_read_under(
    facts: &FixupFacts,
    function: FunctionId,
    path: &AstPath,
) -> Option<BindingId> {
    facts
        .def_use
        .iter()
        .find(|fact| {
            fact.function == function
                && fact
                    .reads
                    .iter()
                    .any(|read| path_starts_with(&read.0, &path.0))
        })
        .map(|fact| fact.binding)
}

fn peel_pointer_view(expr: &Expr) -> &Expr {
    match expr {
        Expr::Cast { expr, .. }
        | Expr::Unary { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => peel_pointer_view(expr),
        Expr::MethodCall { recv, method, args }
            if args.is_empty() && matches!(method.as_str(), "as_ptr" | "as_mut_ptr") =>
        {
            peel_pointer_view(recv)
        }
        Expr::ArrayPtr { array, .. } => peel_pointer_view(array),
        _ => expr,
    }
}

fn expr_at_path<'a>(
    program: &'a Program,
    function: FunctionId,
    path: &AstPath,
) -> Option<&'a Expr> {
    let item_index = facts_function(program, function)?;
    let Item::Fn(f) = &program.items[item_index] else {
        return None;
    };
    expr_in_body(&f.body, &path.0)
}

fn facts_function(program: &Program, function: FunctionId) -> Option<usize> {
    let mut seen = 0;
    for (index, item) in program.items.iter().enumerate() {
        if matches!(item, Item::Fn(_)) {
            if seen == function.0 {
                return Some(index);
            }
            seen += 1;
        }
    }
    None
}

fn expr_in_body<'a>(body: &'a [IndentStmt], path: &[PathSegment]) -> Option<&'a Expr> {
    let [PathSegment::Stmt(index), rest @ ..] = path else {
        return None;
    };
    stmt_expr_at(&body.get(*index)?.stmt, rest)
}

fn stmt_expr_at<'a>(stmt: &'a Stmt, path: &[PathSegment]) -> Option<&'a Expr> {
    match stmt {
        Stmt::Let {
            init: Some(init), ..
        } => expr_at(init, path),
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => match path {
            [PathSegment::Expr(0), rest @ ..] => expr_at(target, rest),
            [PathSegment::Expr(1), rest @ ..] => expr_at(value, rest),
            _ => None,
        },
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => expr_at(expr, path),
        Stmt::If {
            then_body,
            else_body,
            ..
        } => match path {
            [PathSegment::Then, rest @ ..] => expr_in_body(then_body, rest),
            [PathSegment::Else, rest @ ..] => expr_in_body(else_body, rest),
            _ => None,
        },
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            expr_in_body(body, path)
        }
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
            expr_in_block(body, path)
        }
        Stmt::Match { arms, .. } => match path {
            [PathSegment::MatchArm(index), rest @ ..] => {
                expr_in_body(&arms.get(*index)?.body, rest)
            }
            _ => None,
        },
        _ => None,
    }
}

fn expr_in_block<'a>(block: &'a Block, path: &[PathSegment]) -> Option<&'a Expr> {
    match path {
        [PathSegment::BlockTail] => block.tail.as_deref(),
        [PathSegment::BlockTail, rest @ ..] => expr_at(block.tail.as_deref()?, rest),
        _ => expr_in_body(&block.stmts, path),
    }
}

fn expr_at<'a>(expr: &'a Expr, path: &[PathSegment]) -> Option<&'a Expr> {
    if path.is_empty() {
        return Some(expr);
    }
    match (expr, path) {
        (Expr::Call { func, .. }, [PathSegment::Expr(0), rest @ ..]) => expr_at(func, rest),
        (Expr::Call { args, .. }, [PathSegment::Expr(index), rest @ ..]) if *index > 0 => {
            expr_at(args.get(index - 1)?, rest)
        }
        (Expr::Cast { expr, .. }, [PathSegment::Expr(0), rest @ ..])
        | (Expr::Unary { expr, .. }, [PathSegment::Expr(0), rest @ ..])
        | (Expr::Ref { expr, .. }, [PathSegment::Expr(0), rest @ ..])
        | (Expr::AddrOf { expr, .. }, [PathSegment::Expr(0), rest @ ..])
        | (Expr::Transmute { expr, .. }, [PathSegment::Expr(0), rest @ ..])
        | (Expr::MethodCall { recv: expr, .. }, [PathSegment::Expr(0), rest @ ..])
        | (Expr::ArrayPtr { array: expr, .. }, [PathSegment::Expr(0), rest @ ..]) => {
            expr_at(expr, rest)
        }
        (Expr::Binary { lhs, .. }, [PathSegment::Expr(0), rest @ ..])
        | (Expr::Index { base: lhs, .. }, [PathSegment::Expr(0), rest @ ..]) => expr_at(lhs, rest),
        (Expr::Binary { rhs, .. }, [PathSegment::Expr(1), rest @ ..])
        | (Expr::Index { index: rhs, .. }, [PathSegment::Expr(1), rest @ ..]) => expr_at(rhs, rest),
        (Expr::MethodCall { args, .. }, [PathSegment::Expr(index), rest @ ..]) if *index > 0 => {
            expr_at(args.get(index - 1)?, rest)
        }
        (Expr::Macro { args, .. }, [PathSegment::Expr(index), rest @ ..])
        | (Expr::ArrayLit(args), [PathSegment::Expr(index), rest @ ..]) => {
            expr_at(args.get(*index)?, rest)
        }
        _ => None,
    }
}

fn is_char_ptr(ty: &Type) -> bool {
    let Type::Ptr { inner, .. } = ty else {
        return false;
    };
    match &**inner {
        Type::Prim(Prim::I8 | Prim::U8) => true,
        Type::Custom(name) => matches!(
            name.as_str(),
            "libc::c_char" | "std::ffi::c_char" | "core::ffi::c_char"
        ),
        _ => false,
    }
}

fn supported_string_callee(callee: StringLibcFunction) -> bool {
    matches!(
        callee,
        StringLibcFunction::StrLen
            | StringLibcFunction::StrCmp
            | StringLibcFunction::StrNCmp
            | StringLibcFunction::MemCmp
            | StringLibcFunction::StrChr
            | StringLibcFunction::StrRChr
            | StringLibcFunction::StrStr
            | StringLibcFunction::StrPBrk
            | StringLibcFunction::StrSpn
            | StringLibcFunction::StrCSpn
            | StringLibcFunction::Atoi
            | StringLibcFunction::Atol
            | StringLibcFunction::StrTol
            | StringLibcFunction::StrToul
            | StringLibcFunction::StrTod
    )
}

fn path_starts_with(path: &[PathSegment], prefix: &[PathSegment]) -> bool {
    path.len() >= prefix.len() && &path[..prefix.len()] == prefix
}

fn paths_overlap(a: &[PathSegment], b: &[PathSegment]) -> bool {
    path_starts_with(a, b) || path_starts_with(b, a)
}

fn c_string_payload(bytes: &[u8]) -> Option<&[u8]> {
    let payload = bytes.strip_suffix(&[0])?;
    (!payload.contains(&0) && std::str::from_utf8(payload).is_ok()).then_some(payload)
}
