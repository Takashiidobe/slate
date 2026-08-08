use std::collections::{BTreeMap, BTreeSet};

use crate::fixups::facts::walk::Bodies;
use crate::fixups::facts::{
    self, AstPath, BindingFact, BindingId, BindingKind, BindingTypeFact, CallCallee, DefUseFact,
    FixupFacts, FunctionId, StringBufferFact, StringBufferKind, StringLibcFunction,
    StringLibcUseFact, StringParamLiftFact,
};
use crate::fixups::facts::{CallSignatureSource, CallsiteFact, walk};
use crate::rust_ast::{Expr, FnDef, Prim, Program, Type, Visibility};

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
struct Key {
    function: FunctionId,
    param: BindingId,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(in crate::fixups) struct Candidate {
    key: Key,
    function_name: String,
    index: usize,
}

pub(in crate::fixups) fn collect_facts(program: &Program, facts: &mut FixupFacts) {
    let bodies = walk::bodies_from_program(program, facts);
    let snapshot = Snapshot {
        bindings: &facts.bindings,
        binding_types: &facts.binding_types,
        def_use: &facts.def_use,
        callsites: &facts.callsites,
        string_buffers: &facts.string_buffers,
        string_libc_uses: &facts.string_libc_uses,
    };
    facts.string_param_lifts = compute(
        &bodies,
        &snapshot,
        collect_candidates(&bodies, &facts.bindings),
    );
}

pub(in crate::fixups) struct Snapshot<'a> {
    pub(in crate::fixups) bindings: &'a [BindingFact],
    pub(in crate::fixups) binding_types: &'a [BindingTypeFact],
    pub(in crate::fixups) def_use: &'a [DefUseFact],
    pub(in crate::fixups) callsites: &'a [CallsiteFact],
    pub(in crate::fixups) string_buffers: &'a [StringBufferFact],
    pub(in crate::fixups) string_libc_uses: &'a [StringLibcUseFact],
}

impl<'a> Snapshot<'a> {
    fn def_use(&self, binding: BindingId) -> Option<&DefUseFact> {
        facts::def_use_of(self.def_use, binding)
    }

    fn binding_named(&self, function: FunctionId, name: &str) -> Option<BindingId> {
        facts::binding_named(self.bindings, function, name)
    }

    fn binding_read_under(
        &self,
        function: FunctionId,
        name: &str,
        path: &AstPath,
    ) -> Option<BindingId> {
        facts::binding_read_under(self.def_use, self.bindings, function, name, path)
    }

    fn binding_type(&self, binding: BindingId) -> Option<&str> {
        facts::binding_type(self.binding_types, binding)
    }

    fn string_buffer(&self, binding: BindingId) -> Option<&StringBufferFact> {
        facts::string_buffer(self.string_buffers, binding)
    }

    fn local_binding_at(&self, function: FunctionId, path: &AstPath) -> Option<&BindingFact> {
        facts::local_binding_at(self.bindings, function, path)
    }
}

pub(in crate::fixups) fn compute(
    bodies: &Bodies,
    facts: &Snapshot,
    candidates: Vec<Candidate>,
) -> Vec<StringParamLiftFact> {
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
                && all_callers_prove_arg(bodies, candidate, facts, &before)
        });
        if active == before {
            break;
        }
    }

    candidates
        .into_iter()
        .filter(|candidate| active.contains(&candidate.key))
        .map(|candidate| StringParamLiftFact {
            callee: candidate.key.function,
            param: candidate.key.param,
            index: candidate.index,
        })
        .collect()
}

fn collect_candidates(bodies: &Bodies, bindings: &[BindingFact]) -> Vec<Candidate> {
    bodies
        .iter()
        .flat_map(|(&function, &f)| candidates_for_function(function, f, bindings))
        .collect()
}

pub(in crate::fixups) fn candidates_for_function(
    function: FunctionId,
    f: &FnDef,
    bindings: &[BindingFact],
) -> Vec<Candidate> {
    if f.name == "main" || f.unsafe_ || f.abi.is_some() || !matches!(f.vis, Visibility::Private) {
        return Vec::new();
    }
    f.params
        .iter()
        .enumerate()
        .filter_map(|(index, param)| {
            if !is_char_ptr(&param.ty) {
                return None;
            }
            let binding = facts::binding_by_param_index(bindings, function, index)?;
            Some(Candidate {
                key: Key {
                    function,
                    param: binding,
                },
                function_name: f.name.clone(),
                index,
            })
        })
        .collect()
}

fn all_uses_allow_lift(
    candidate: &Candidate,
    facts: &Snapshot,
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
    facts: &Snapshot,
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
    facts: &Snapshot,
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
                    && direct_callee_function(callsite)
                        .and_then(|callee| by_function.get(&(callee, arg.slot)))
                        .is_some_and(|target| active.contains(target))
            })
    })
}

fn all_callers_prove_arg(
    bodies: &Bodies,
    candidate: &Candidate,
    facts: &Snapshot,
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
            let expr = walk::expr_at_body_path(bodies, callsite.site.function, &arg.path);

            expr.is_some_and(|expr| {
                expr_is_liftable_source(expr, callsite.site.function, &arg.path, facts, active)
            })
        })
}

fn direct_callee_function(callsite: &CallsiteFact) -> Option<FunctionId> {
    let CallCallee::Direct {
        signature: Some(signature),
        ..
    } = &callsite.callee
    else {
        return None;
    };
    match signature.source {
        CallSignatureSource::Function(function) => Some(function),
        CallSignatureSource::Extern { .. } => None,
    }
}

fn expr_is_liftable_source(
    expr: &Expr,
    function: FunctionId,
    path: &AstPath,
    facts: &Snapshot,
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
    facts: &Snapshot,
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
    facts: &Snapshot,
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

fn direct_alias_at(function: FunctionId, facts: &Snapshot, path: &AstPath) -> Option<BindingId> {
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
