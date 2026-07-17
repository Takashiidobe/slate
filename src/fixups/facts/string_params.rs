use std::collections::{BTreeMap, BTreeSet};

use crate::fixups::facts::walk;
use crate::fixups::facts::{
    AstPath, BindingId, BindingKind, CallCallee, FixupFacts, FunctionId, StringBufferKind,
    StringLibcFunction, StringParamLiftFact,
};
use crate::rust_ast::{Expr, Item, Prim, Program, Type, Visibility};

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
            if f.name == "main"
                || f.unsafe_
                || f.abi.is_some()
                || !matches!(f.vis, Visibility::Private)
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
        usage.site.function == function
            && walk::path_starts_with(&read.0, &usage.site.path.0)
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
        callsite.site.function == function
            && matches!(callsite.callee, CallCallee::Direct { .. })
            && walk::path_starts_with(&read.0, &callsite.site.path.0)
            && callsite.args.iter().any(|arg| {
                walk::paths_overlap(&read.0, &arg.path.0)
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
            let expr = walk::expr_at_path(facts, program, callsite.site.function, &arg.path);

            expr.is_some_and(|expr| {
                expr_is_liftable_source(expr, callsite.site.function, &arg.path, facts, active)
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
        Expr::Var(name) => facts
            .binding_read_under(function, name.as_str(), path)
            .or_else(|| facts.binding_named(function, name.as_str()))
            .is_some_and(|binding| binding_is_liftable_source(facts, binding, active)),
        Expr::CStr(bytes) => c_string_payload(bytes).is_some(),
        _ => false,
    }
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

fn c_string_payload(bytes: &[u8]) -> Option<&[u8]> {
    let payload = bytes.strip_suffix(&[0])?;
    (!payload.contains(&0) && std::str::from_utf8(payload).is_ok()).then_some(payload)
}
