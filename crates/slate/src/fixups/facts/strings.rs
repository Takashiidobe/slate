use std::collections::{BTreeMap, BTreeSet};

use crate::fixups::facts::walk;
use crate::fixups::facts::{
    AsciiNumericSign, AsciiNumericStringFact, AstPath, BindingId, BindingKind, CallCallee,
    CallSignatureSource, ConstValue, FixupFacts, FunctionId, NulTermination, PathSegment, Site,
    StringBufferFact, StringBufferKind, StringBufferProvenance, StringBufferRejection,
    StringCopyRewrite, StringCopyRewriteFact, StringLibcFunction, StringLibcUseFact,
    StringLiftPlanFact, StringPointerViewFact, StringPointerViewKind, StringRecoveryCandidate,
    ValueSubject,
};
use crate::rust_ast::{
    Block, Expr, IndentStmt, Item, Pattern, Prim, Program, RustValue, Stmt, Type, UnaryOp,
};

pub(in crate::fixups) fn collect_facts(program: &Program, facts: &mut FixupFacts) {
    facts.string_buffers.clear();
    facts.ascii_numeric_strings.clear();
    facts.string_pointer_views.clear();
    facts.string_libc_uses.clear();

    let mut buffers = Vec::new();
    let mut pointer_views = Vec::new();
    let mut libc_uses = Vec::new();
    for (item_index, item) in program.items.iter().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        let mut collector = Collector::new(function, facts);
        collector.enter_root_scope();
        collector.body(&f.body, &mut Vec::new(), false);
        let collected = collector.finish();
        buffers.extend(collected.buffers);
        pointer_views.extend(collected.pointer_views);
        libc_uses.extend(collected.libc_uses);
    }

    facts.ascii_numeric_strings = collect_ascii_numeric_strings(&buffers);
    facts.string_buffers = buffers;
    facts.string_pointer_views = pointer_views;
    facts.string_libc_uses = libc_uses;
}

fn collect_ascii_numeric_strings(buffers: &[StringBufferFact]) -> Vec<AsciiNumericStringFact> {
    buffers
        .iter()
        .filter_map(|buffer| {
            let bytes = buffer.bytes.as_deref()?;
            let (sign, digits) = ascii_numeric_token(bytes)?;
            Some(AsciiNumericStringFact {
                site: buffer.site.clone(),
                binding: buffer.binding,
                sign,
                digits,
            })
        })
        .collect()
}

fn ascii_numeric_token(bytes: &[u8]) -> Option<(AsciiNumericSign, usize)> {
    let (sign, digits) = match bytes.first() {
        Some(b'+') => (AsciiNumericSign::Plus, &bytes[1..]),
        Some(b'-') => (AsciiNumericSign::Minus, &bytes[1..]),
        _ => (AsciiNumericSign::None, bytes),
    };
    (!digits.is_empty() && digits.iter().all(u8::is_ascii_digit)).then_some((sign, digits.len()))
}

pub(in crate::fixups) fn collect_rewrite_facts(program: &Program, facts: &mut FixupFacts) {
    facts.string_lift_plans.clear();
    facts.string_copy_rewrites.clear();

    for (item_index, item) in program.items.iter().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        let mut plans = Vec::new();
        let mut rewrites = Vec::new();
        let consts = const_usize_temps(function, facts);
        collect_body_rewrite_facts(
            function,
            &f.body,
            &mut Vec::new(),
            facts,
            &consts,
            &mut plans,
            &mut rewrites,
        );
        facts.string_lift_plans.extend(plans);
        facts.string_copy_rewrites.extend(rewrites);
    }
}

fn collect_body_rewrite_facts(
    function: FunctionId,
    body: &[IndentStmt],
    path: &mut Vec<PathSegment>,
    facts: &FixupFacts,
    consts: &BTreeMap<String, usize>,
    plans: &mut Vec<StringLiftPlanFact>,
    rewrites: &mut Vec<StringCopyRewriteFact>,
) {
    collect_nested_rewrite_facts(function, body, path, facts, consts, plans, rewrites);
    let borrowed = liftable_bindings(
        function,
        body,
        path,
        facts,
        StringRecoveryCandidate::BorrowedStr,
        consts,
    );
    let borrowed_cstr = liftable_bindings(
        function,
        body,
        path,
        facts,
        StringRecoveryCandidate::BorrowedCStr,
        consts,
    );
    let borrowed_bytes = liftable_bindings(
        function,
        body,
        path,
        facts,
        StringRecoveryCandidate::BorrowedBytes,
        consts,
    );
    let owned = liftable_bindings(
        function,
        body,
        path,
        facts,
        StringRecoveryCandidate::OwnedString,
        consts,
    );
    for binding in borrowed_cstr {
        if let Some(plan) = lift_plan_for_binding(
            function,
            body,
            path,
            facts,
            binding,
            StringRecoveryCandidate::BorrowedCStr,
        ) {
            plans.push(plan);
        }
    }
    for binding in borrowed {
        if let Some(plan) = lift_plan_for_binding(
            function,
            body,
            path,
            facts,
            binding,
            StringRecoveryCandidate::BorrowedStr,
        ) {
            plans.push(plan);
        }
    }
    for binding in borrowed_bytes {
        if let Some(plan) = lift_plan_for_binding(
            function,
            body,
            path,
            facts,
            binding,
            StringRecoveryCandidate::BorrowedBytes,
        ) {
            plans.push(plan);
        }
    }
    for binding in &owned {
        if let Some(plan) = lift_plan_for_binding(
            function,
            body,
            path,
            facts,
            *binding,
            StringRecoveryCandidate::OwnedString,
        ) {
            plans.push(plan);
        }
    }
    collect_copy_rewrites(function, body, path, facts, consts, &owned, rewrites);
}

fn collect_nested_rewrite_facts(
    function: FunctionId,
    body: &[IndentStmt],
    path: &mut Vec<PathSegment>,
    facts: &FixupFacts,
    consts: &BTreeMap<String, usize>,
    plans: &mut Vec<StringLiftPlanFact>,
    rewrites: &mut Vec<StringCopyRewriteFact>,
) {
    for (index, indent) in body.iter().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| match &indent.stmt {
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
                    collect_body_rewrite_facts(
                        function, then_body, path, facts, consts, plans, rewrites,
                    );
                });
                walk::with_path_segment(path, PathSegment::Else, |path| {
                    collect_body_rewrite_facts(
                        function, else_body, path, facts, consts, plans, rewrites,
                    );
                });
            }
            Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
                collect_body_rewrite_facts(function, body, path, facts, consts, plans, rewrites);
            }
            Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
                walk::with_path_segment(path, PathSegment::BlockBody, |path| {
                    collect_body_rewrite_facts(
                        function,
                        &body.stmts,
                        path,
                        facts,
                        consts,
                        plans,
                        rewrites,
                    );
                });
            }
            Stmt::Match { arms, .. } => {
                for (arm_index, arm) in arms.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::MatchArm(arm_index), |path| {
                        collect_body_rewrite_facts(
                            function, &arm.body, path, facts, consts, plans, rewrites,
                        );
                    });
                }
            }
            _ => {}
        });
    }
}

fn liftable_bindings(
    function: FunctionId,
    body: &[IndentStmt],
    path: &[PathSegment],
    facts: &FixupFacts,
    recovery: StringRecoveryCandidate,
    consts: &BTreeMap<String, usize>,
) -> BTreeSet<BindingId> {
    let candidates = (0..body.len())
        .filter_map(|index| {
            let stmt_path = stmt_path(path, index);
            let candidate = lift_candidate_at(function, body, facts, &stmt_path, recovery)?;
            Some((candidate.binding, index, candidate.remove_index))
        })
        .collect::<Vec<_>>();
    let mut liftable = candidates
        .iter()
        .map(|(binding, _, _)| *binding)
        .collect::<BTreeSet<_>>();
    loop {
        let before = liftable.clone();
        let ctx = LiftContext {
            function,
            facts,
            liftable: &before,
            consts,
        };
        liftable.retain(|binding| {
            let Some((_, index, remove_index)) = candidates
                .iter()
                .find(|(candidate, _, _)| candidate == binding)
                .copied()
            else {
                return false;
            };
            let scan_start = remove_index.map_or(index + 1, |index| index + 1);
            body[scan_start..]
                .iter()
                .enumerate()
                .all(|(offset, indent)| {
                    let stmt_path = stmt_path(path, scan_start + offset);
                    stmt_allows_lift(&ctx, &indent.stmt, &stmt_path, *binding, recovery)
                })
        });
        if liftable == before {
            return liftable;
        }
    }
}

struct LiftCandidate {
    binding: BindingId,
    remove_index: Option<usize>,
    remove_assignment: Option<AstPath>,
}

struct LiftContext<'a> {
    function: FunctionId,
    facts: &'a FixupFacts,
    liftable: &'a BTreeSet<BindingId>,
    consts: &'a BTreeMap<String, usize>,
}

fn lift_plan_for_binding(
    function: FunctionId,
    body: &[IndentStmt],
    path: &[PathSegment],
    facts: &FixupFacts,
    binding: BindingId,
    recovery: StringRecoveryCandidate,
) -> Option<StringLiftPlanFact> {
    for index in 0..body.len() {
        let stmt_path = stmt_path(path, index);
        let Some(candidate) = lift_candidate_at(function, body, facts, &stmt_path, recovery) else {
            continue;
        };
        if candidate.binding == binding {
            return Some(StringLiftPlanFact {
                site: Site {
                    function,
                    path: AstPath(stmt_path),
                },
                binding,
                recovery,
                remove_assignment: candidate.remove_assignment,
            });
        }
    }
    None
}

fn lift_candidate_at(
    function: FunctionId,
    body: &[IndentStmt],
    facts: &FixupFacts,
    path: &[PathSegment],
    recovery: StringRecoveryCandidate,
) -> Option<LiftCandidate> {
    let buffer = facts.string_buffer_at(function, &AstPath(path.to_vec()))?;
    if !buffer.candidates.contains(&recovery) {
        return None;
    }
    let remove_assignment = match &buffer.provenance {
        StringBufferProvenance::Literal => {
            if facts
                .def_use(buffer.binding)
                .is_some_and(|def_use| !def_use.writes.is_empty())
            {
                return None;
            }
            None
        }
        StringBufferProvenance::ZeroInitialized => {
            if recovery != StringRecoveryCandidate::OwnedString {
                return None;
            }
            None
        }
        StringBufferProvenance::AssignedLiteral { assignment } => Some(assignment.clone()),
        _ => return None,
    };
    let remove_index = remove_assignment
        .as_ref()
        .and_then(|assignment| assignment_index(path, &assignment.0));
    if remove_index.is_some_and(|index| index >= body.len()) {
        return None;
    }
    Some(LiftCandidate {
        binding: buffer.binding,
        remove_index,
        remove_assignment,
    })
}

fn collect_copy_rewrites(
    function: FunctionId,
    body: &[IndentStmt],
    path: &[PathSegment],
    facts: &FixupFacts,
    consts: &BTreeMap<String, usize>,
    liftable: &BTreeSet<BindingId>,
    rewrites: &mut Vec<StringCopyRewriteFact>,
) {
    for (index, indent) in body.iter().enumerate() {
        let stmt_path = stmt_path(path, index);
        if let Stmt::Expr(expr) = &indent.stmt
            && let Some((dst, rewrite)) =
                copy_rewrite_for_expr(function, expr, &stmt_path, facts, liftable, consts)
        {
            rewrites.push(StringCopyRewriteFact {
                site: Site {
                    function,
                    path: AstPath(stmt_path),
                },
                dst,
                rewrite,
            });
        }
    }
}

fn stmt_allows_lift(
    ctx: &LiftContext<'_>,
    stmt: &Stmt,
    path: &[PathSegment],
    binding: BindingId,
    recovery: StringRecoveryCandidate,
) -> bool {
    if let Stmt::Expr(expr) = stmt
        && recovery == StringRecoveryCandidate::OwnedString
        && copy_rewrite_for_expr(
            ctx.function,
            expr,
            path,
            ctx.facts,
            ctx.liftable,
            ctx.consts,
        )
        .is_some()
    {
        return true;
    }
    let uses = binding_uses_under(ctx.facts, ctx.function, binding, path);
    uses.into_iter().all(|use_path| {
        use_allowed(
            ctx.function,
            &use_path,
            ctx.facts,
            binding,
            recovery,
            ctx.liftable,
        )
    })
}

fn body_allows_lift(
    ctx: &LiftContext<'_>,
    body: &[IndentStmt],
    path: &[PathSegment],
    binding: BindingId,
    recovery: StringRecoveryCandidate,
) -> bool {
    body.iter().enumerate().all(|(index, indent)| {
        stmt_allows_lift(
            ctx,
            &indent.stmt,
            &stmt_path(path, index),
            binding,
            recovery,
        )
    })
}

fn block_allows_lift(
    ctx: &LiftContext<'_>,
    block: &Block,
    path: &[PathSegment],
    binding: BindingId,
    recovery: StringRecoveryCandidate,
) -> bool {
    body_allows_lift(ctx, &block.stmts, path, binding, recovery)
        && block.tail.as_deref().is_none_or(|_tail| {
            let tail_path = child_path(path, PathSegment::BlockTail);
            binding_uses_under(ctx.facts, ctx.function, binding, &tail_path)
                .into_iter()
                .all(|use_path| {
                    use_allowed(
                        ctx.function,
                        &use_path,
                        ctx.facts,
                        binding,
                        recovery,
                        ctx.liftable,
                    )
                })
        })
}
fn binding_uses_under(
    facts: &FixupFacts,
    function: FunctionId,
    binding: BindingId,
    prefix: &[PathSegment],
) -> Vec<AstPath> {
    let mut uses = Vec::new();
    if let Some(def_use) = facts.def_use(binding) {
        uses.extend(
            def_use
                .reads
                .iter()
                .filter(|path| path_starts_with(&path.0, prefix))
                .cloned(),
        );
        uses.extend(
            def_use
                .writes
                .iter()
                .filter(|path| path_starts_with(&path.0, prefix))
                .cloned(),
        );
    }
    uses.extend(
        facts
            .string_pointer_views
            .iter()
            .filter(|view| {
                view.site.function == function
                    && view.source == binding
                    && path_starts_with(&view.site.path.0, prefix)
            })
            .map(|view| view.site.path.clone()),
    );
    uses
}

fn use_allowed(
    function: FunctionId,
    use_path: &AstPath,
    facts: &FixupFacts,
    binding: BindingId,
    recovery: StringRecoveryCandidate,
    liftable: &BTreeSet<BindingId>,
) -> bool {
    if facts.printf_calls.iter().any(|printf| {
        printf.site.function == function
            && path_starts_with(&use_path.0, &printf.site.path.0)
            && printf_allows_lift(function, printf, facts, binding, liftable)
    }) {
        return true;
    }
    if recovery == StringRecoveryCandidate::BorrowedCStr {
        return facts.callsites.iter().any(|callsite| {
            callsite.site.function == function
                && path_starts_with(&use_path.0, &callsite.site.path.0)
                && matches!(
                    &callsite.callee,
                    CallCallee::Direct { name, .. } if name == "__slate_memchr"
                )
                && callsite
                    .args
                    .iter()
                    .any(|arg| paths_overlap(&use_path.0, &arg.path.0))
        });
    }
    if facts.callsites.iter().any(|callsite| {
        callsite.site.function == function
            && path_starts_with(&use_path.0, &callsite.site.path.0)
            && callsite.args.iter().any(|arg| {
                paths_overlap(&use_path.0, &arg.path.0)
                    && direct_callee_function(facts, callsite).is_some_and(|callee| {
                        facts.string_param_lifts.iter().any(|lift| {
                            lift.callee == callee && lift.index == arg.slot && lift.param != binding
                        })
                    })
            })
    }) {
        return true;
    }
    facts.string_libc_uses.iter().any(|libc| {
        libc.site.function == function
            && path_starts_with(&use_path.0, &libc.site.path.0)
            && matches!(
                libc.callee,
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
            && libc
                .pointer_args
                .iter()
                .all(|arg| *arg == binding || liftable.contains(arg))
    })
}

fn printf_allows_lift(
    function: FunctionId,
    printf: &crate::fixups::facts::PrintfCallFact,
    facts: &FixupFacts,
    binding: BindingId,
    liftable: &BTreeSet<BindingId>,
) -> bool {
    if !all_printf_string_args_allow_lift(function, facts, binding, liftable) {
        return false;
    }
    let Some(conversions) = printf.format.as_deref().and_then(simple_printf_conversions) else {
        return false;
    };
    conversions.len() == printf.arg_facts.len()
        && conversions
            .iter()
            .zip(&printf.arg_facts)
            .all(|(conversion, arg)| {
                facts
                    .string_pointer_view(function, &arg.path)
                    .is_none_or(|view| {
                        view.source != binding || (*conversion == b's' && arg.pointer)
                    })
            })
}

fn all_printf_string_args_allow_lift(
    function: FunctionId,
    facts: &FixupFacts,
    binding: BindingId,
    liftable: &BTreeSet<BindingId>,
) -> bool {
    facts
        .printf_calls
        .iter()
        .filter(|printf| printf.site.function == function)
        .all(|printf| {
            let Some(conversions) = printf.format.as_deref().and_then(simple_printf_conversions)
            else {
                return false;
            };
            conversions.len() == printf.arg_facts.len()
                && conversions
                    .iter()
                    .zip(&printf.arg_facts)
                    .all(|(conversion, arg)| match *conversion {
                        b's' => {
                            printf_string_arg_allows_lift(function, facts, binding, liftable, arg)
                        }
                        b'c' => arg.const_char.is_some(),
                        _ => true,
                    })
        })
}

fn printf_string_arg_allows_lift(
    function: FunctionId,
    facts: &FixupFacts,
    binding: BindingId,
    liftable: &BTreeSet<BindingId>,
    arg: &crate::fixups::facts::PrintfArgFact,
) -> bool {
    arg.const_string.is_some()
        || arg.rust_string
        || facts
            .string_pointer_view(function, &arg.path)
            .is_some_and(|view| view.source == binding || liftable.contains(&view.source))
}

fn copy_rewrite_for_expr(
    function: FunctionId,
    expr: &Expr,
    path: &[PathSegment],
    facts: &FixupFacts,
    liftable: &BTreeSet<BindingId>,
    consts: &BTreeMap<String, usize>,
) -> Option<(BindingId, StringCopyRewrite)> {
    let Expr::Call { args, .. } = peel_empty_unsafe(expr) else {
        return None;
    };
    let libc = facts.string_libc_use(function, &AstPath(path.to_vec()))?;
    match libc.callee {
        StringLibcFunction::StrCpy if args.len() == 2 => {
            let arg_path = call_arg_path(expr, path, 0);
            let dst = pointer_view_binding(function, &arg_path, facts)?;
            if !liftable.contains(&dst) {
                return None;
            }
            let rewrite = copy_source_rewrite(
                function,
                &args[1],
                &call_arg_path(expr, path, 1),
                facts,
                liftable,
                false,
            )?;
            Some((dst, rewrite))
        }
        StringLibcFunction::StrNCpy if args.len() == 3 => {
            let dst = pointer_view_binding(function, &call_arg_path(expr, path, 0), facts)?;
            if !liftable.contains(&dst) {
                return None;
            }
            let src = const_ascii_prefix(&args[1], &args[2], consts)?;
            Some((dst, StringCopyRewrite::AssignLiteral(src)))
        }
        StringLibcFunction::StrCat if args.len() == 2 => {
            let dst = pointer_view_binding(function, &call_arg_path(expr, path, 0), facts)?;
            if !liftable.contains(&dst) {
                return None;
            }
            let rewrite = copy_source_rewrite(
                function,
                &args[1],
                &call_arg_path(expr, path, 1),
                facts,
                liftable,
                true,
            )?;
            Some((dst, rewrite))
        }
        StringLibcFunction::StrNCat if args.len() == 3 => {
            let dst = pointer_view_binding(function, &call_arg_path(expr, path, 0), facts)?;
            if !liftable.contains(&dst) {
                return None;
            }
            let src = const_ascii_prefix(&args[1], &args[2], consts)?;
            Some((dst, StringCopyRewrite::PushLiteral(src)))
        }
        _ => None,
    }
}

fn copy_source_rewrite(
    function: FunctionId,
    expr: &Expr,
    path: &[PathSegment],
    facts: &FixupFacts,
    liftable: &BTreeSet<BindingId>,
    push: bool,
) -> Option<StringCopyRewrite> {
    if let Some(bytes) = const_c_string(expr) {
        let value = String::from_utf8(bytes).ok()?;
        return Some(if push {
            StringCopyRewrite::PushLiteral(value)
        } else {
            StringCopyRewrite::AssignLiteral(value)
        });
    }
    let source = pointer_view_binding(function, path, facts)?;
    if !liftable.contains(&source) {
        return None;
    }
    Some(if push {
        StringCopyRewrite::PushOwned(source)
    } else {
        StringCopyRewrite::AssignOwned(source)
    })
}

fn pointer_view_binding(
    function: FunctionId,
    path: &[PathSegment],
    facts: &FixupFacts,
) -> Option<BindingId> {
    facts
        .string_pointer_view(function, &AstPath(path.to_vec()))
        .map(|view| view.source)
}

fn binding_for_expr(
    function: FunctionId,
    expr: &Expr,
    path: &[PathSegment],
    facts: &FixupFacts,
) -> Option<BindingId> {
    let Expr::Var(name) = expr else {
        return None;
    };
    facts
        .def_use
        .iter()
        .find(|def_use| {
            def_use.function == function
                && facts.binding_name(def_use.binding) == Some(name.as_str())
                && def_use.reads.iter().any(|read| read.0 == path)
        })
        .map(|def_use| def_use.binding)
}

fn const_usize_temps(function: FunctionId, facts: &FixupFacts) -> BTreeMap<String, usize> {
    facts
        .values
        .iter()
        .filter(|fact| fact.site.function == function)
        .filter_map(|fact| match (&fact.subject, &fact.value) {
            (ValueSubject::Binding(binding), ConstValue::Usize(value)) => {
                Some((facts.binding_name(*binding)?.to_owned(), *value))
            }
            _ => None,
        })
        .collect()
}

fn const_ascii_prefix(
    src: &Expr,
    count: &Expr,
    consts: &BTreeMap<String, usize>,
) -> Option<String> {
    let bytes = const_c_string(src)?;
    if !bytes.is_ascii() {
        return None;
    }
    let n = const_usize(count, consts)?;
    String::from_utf8(bytes[..std::cmp::min(n, bytes.len())].to_vec()).ok()
}

fn const_usize(expr: &Expr, consts: &BTreeMap<String, usize>) -> Option<usize> {
    match expr {
        Expr::Var(name) => consts.get(name.as_str()).copied(),
        Expr::Value(RustValue::I64(n)) => usize::try_from(*n).ok(),
        Expr::Value(RustValue::I128(n)) => usize::try_from(*n).ok(),
        Expr::Cast { expr, .. } => const_usize(expr, consts),
        _ => None,
    }
}

fn const_c_string(expr: &Expr) -> Option<Vec<u8>> {
    match expr {
        Expr::Str(s) => Some(s.as_bytes().to_vec()),
        Expr::ByteStr(bytes) => Some(bytes.strip_suffix(&[0]).unwrap_or(bytes).to_vec()),
        Expr::CStr(bytes) => Some(bytes.clone()),
        Expr::Cast { expr, .. } => const_c_string(expr),
        Expr::MethodCall { recv, method, args } if method == "as_ptr" && args.is_empty() => {
            const_c_string(recv)
        }
        _ => None,
    }
}

fn call_arg_path(expr: &Expr, path: &[PathSegment], arg_index: usize) -> Vec<PathSegment> {
    let mut path = path.to_vec();
    if matches!(expr, Expr::Unsafe(block) if block.stmts.is_empty() && block.tail.is_some()) {
        path.push(PathSegment::UnsafeBody);
        path.push(PathSegment::BlockTail);
    }
    path.push(PathSegment::Expr(arg_index + 1));
    path
}

fn simple_printf_conversions(bytes: &[u8]) -> Option<Vec<u8>> {
    let mut conversions = Vec::new();
    let mut i = 0;
    while i < bytes.len() {
        match bytes[i] {
            b'%' => {
                let (next, conv) = simple_printf_conversion(bytes, i + 1)?;
                if conv == b'%' {
                    i += 2;
                    continue;
                }
                if !matches!(conv, b's' | b'c' | b'd' | b'i' | b'u' | b'x' | b'X' | b'o') {
                    return None;
                }
                conversions.push(conv);
                i = next;
            }
            0x20..=0x7e | b'\n' | b'\t' => i += 1,
            _ => return None,
        }
    }
    Some(conversions)
}

fn simple_printf_conversion(bytes: &[u8], mut i: usize) -> Option<(usize, u8)> {
    match bytes.get(i).copied()? {
        b'%' => return Some((i + 1, b'%')),
        b's' | b'c' => return Some((i + 1, bytes[i])),
        _ => {}
    }
    while let b'-' | b'+' | b'#' | b'0' | b' ' = bytes.get(i).copied()? {
        i += 1;
    }
    while bytes.get(i).is_some_and(u8::is_ascii_digit) {
        i += 1;
    }
    match bytes.get(i).copied()? {
        b'l' => {
            i += 1;
            if bytes.get(i).copied() == Some(b'l') {
                i += 1;
            }
        }
        b'z' => i += 1,
        _ => {}
    }
    let conv = bytes.get(i).copied()?;
    matches!(conv, b'd' | b'i' | b'u' | b'x' | b'X' | b'o').then_some((i + 1, conv))
}

fn stmt_path(path: &[PathSegment], index: usize) -> Vec<PathSegment> {
    let mut path = path.to_vec();
    path.push(PathSegment::Stmt(index));
    path
}

fn child_path(path: &[PathSegment], segment: PathSegment) -> Vec<PathSegment> {
    let mut path = path.to_vec();
    path.push(segment);
    path
}

fn path_starts_with(path: &[PathSegment], prefix: &[PathSegment]) -> bool {
    path.len() >= prefix.len() && &path[..prefix.len()] == prefix
}

fn paths_overlap(a: &[PathSegment], b: &[PathSegment]) -> bool {
    path_starts_with(a, b) || path_starts_with(b, a)
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
        CallSignatureSource::Function(function) => Some(function),
        CallSignatureSource::Extern { .. } => None,
    }
}

fn assignment_index(def_path: &[PathSegment], assignment_path: &[PathSegment]) -> Option<usize> {
    let parent = def_path.get(..def_path.len().checked_sub(1)?)?;
    let assignment_parent = assignment_path.get(..assignment_path.len().checked_sub(1)?)?;
    if assignment_parent != parent {
        return None;
    }
    match assignment_path.last()? {
        PathSegment::Stmt(index) => Some(*index),
        _ => None,
    }
}

struct Collected {
    buffers: Vec<StringBufferFact>,
    pointer_views: Vec<StringPointerViewFact>,
    libc_uses: Vec<StringLibcUseFact>,
}

struct Collector<'a> {
    function: FunctionId,
    facts: &'a FixupFacts,
    scopes: Vec<BTreeMap<String, Option<BindingId>>>,
    summaries: BTreeMap<BindingId, BufferSummary>,
    pointer_views: Vec<StringPointerViewFact>,
    libc_uses: Vec<StringLibcUseFact>,
}

#[derive(Clone)]
struct BufferSummary {
    binding: BindingId,
    path: AstPath,
    kind: StringBufferKind,
    provenance: StringBufferProvenance,
    bytes: Option<Vec<u8>>,
    nul_termination: NulTermination,
    interior_nul: bool,
    ascii_only: bool,
    rejections: BTreeSet<StringBufferRejection>,
}

struct LiteralBytes {
    bytes: Vec<u8>,
    nul_termination: NulTermination,
    interior_nul: bool,
}

impl<'a> Collector<'a> {
    fn new(function: FunctionId, facts: &'a FixupFacts) -> Self {
        Self {
            function,
            facts,
            scopes: Vec::new(),
            summaries: BTreeMap::new(),
            pointer_views: Vec::new(),
            libc_uses: Vec::new(),
        }
    }

    fn enter_root_scope(&mut self) {
        self.scopes.push(BTreeMap::new());
        let params: Vec<_> = self
            .facts
            .bindings
            .iter()
            .filter(|binding| binding.function == self.function)
            .filter_map(|binding| match &binding.kind {
                BindingKind::Param { .. } => Some((binding.name.clone(), binding.id)),
                BindingKind::Local => None,
            })
            .collect();
        for (name, id) in params {
            self.bind(name, Some(id));
            if let Some(ty) = self
                .facts
                .binding_type(id)
                .map(Type::parse)
                .as_ref()
                .and_then(lifted_kind)
            {
                self.summaries
                    .insert(id, BufferSummary::new(id, AstPath::default(), ty));
            }
        }
    }

    fn finish(self) -> Collected {
        Collected {
            buffers: self
                .summaries
                .into_values()
                .map(|summary| summary.into_fact(self.function))
                .collect(),
            pointer_views: self.pointer_views,
            libc_uses: self.libc_uses,
        }
    }

    fn body(&mut self, body: &[IndentStmt], path: &mut Vec<PathSegment>, scoped: bool) {
        if scoped {
            self.scopes.push(BTreeMap::new());
        }
        for (index, indent) in body.iter().enumerate() {
            walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
                self.stmt(&indent.stmt, path);
            });
        }
        if scoped {
            self.scopes.pop();
        }
    }

    fn block(&mut self, block: &Block, path: &mut Vec<PathSegment>) {
        self.scopes.push(BTreeMap::new());
        self.body(&block.stmts, path, false);
        if let Some(tail) = &block.tail {
            walk::with_path_segment(path, PathSegment::BlockTail, |path| {
                self.expr(tail, path);
            });
        }
        self.scopes.pop();
    }

    fn stmt(&mut self, stmt: &Stmt, path: &mut Vec<PathSegment>) {
        match stmt {
            Stmt::Let { name, ty, init, .. } => {
                if let Some(init) = init {
                    self.expr(init, path);
                }
                self.define_local(name, ty.as_ref(), init.as_ref(), path);
            }
            Stmt::LetIf {
                name,
                ty,
                cond,
                then_body,
                then_value,
                else_body,
                else_value,
                ..
            } => {
                self.expr(cond, path);
                walk::with_path_segment(path, PathSegment::Then, |path| {
                    self.scopes.push(BTreeMap::new());
                    self.body(then_body, path, false);
                    self.expr(then_value, path);
                    self.scopes.pop();
                });
                walk::with_path_segment(path, PathSegment::Else, |path| {
                    self.scopes.push(BTreeMap::new());
                    self.body(else_body, path, false);
                    self.expr(else_value, path);
                    self.scopes.pop();
                });
                self.define_local(name, ty.as_ref(), None, path);
            }
            Stmt::Assign { target, value } => {
                self.expr(value, path);
                self.assign(target, value, path);
            }
            Stmt::CompoundAssign { target, value, .. } => {
                self.expr(value, path);
                self.mutate_target(target, path);
            }
            Stmt::Expr(expr) | Stmt::Return(Some(expr)) => self.expr(expr, path),
            Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
            Stmt::If {
                cond,
                then_body,
                else_body,
            } => {
                self.expr(cond, path);
                walk::with_path_segment(path, PathSegment::Then, |path| {
                    self.body(then_body, path, true)
                });
                walk::with_path_segment(path, PathSegment::Else, |path| {
                    self.body(else_body, path, true)
                });
            }
            Stmt::Loop { body, .. } => {
                walk::with_path_segment(path, PathSegment::LoopBody, |path| {
                    self.body(body, path, true)
                });
            }
            Stmt::For { pat, iter, body } => {
                self.expr(iter, path);
                let binding = self.local_binding(pat, path);
                walk::with_path_segment(path, PathSegment::ForBody, |path| {
                    self.scopes.push(BTreeMap::new());
                    self.bind(pat.to_string(), binding);
                    self.body(body, path, false);
                    self.scopes.pop();
                });
            }
            Stmt::Scope { body } => {
                walk::with_path_segment(path, PathSegment::ScopeBody, |path| {
                    self.body(body, path, true)
                });
            }
            Stmt::LabeledBlock { body, .. } => {
                walk::with_path_segment(path, PathSegment::LabeledBody, |path| {
                    self.body(body, path, true)
                });
            }
            Stmt::Unsafe { body } => {
                walk::with_path_segment(path, PathSegment::UnsafeBody, |path| {
                    self.block(body, path)
                });
            }
            Stmt::While { cond, body } => {
                self.expr(cond, path);
                walk::with_path_segment(path, PathSegment::WhileBody, |path| {
                    self.block(body, path)
                });
            }
            Stmt::Block(body) => {
                walk::with_path_segment(path, PathSegment::BlockBody, |path| {
                    self.block(body, path)
                });
            }
            Stmt::Match { expr, arms } => {
                self.expr(expr, path);
                for (index, arm) in arms.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::MatchArm(index), |path| {
                        self.scopes.push(BTreeMap::new());
                        self.shadow_pattern(&arm.pattern);
                        self.body(&arm.body, path, false);
                        self.scopes.pop();
                    });
                }
            }
        }
    }

    fn expr(&mut self, expr: &Expr, path: &mut Vec<PathSegment>) {
        if let Some((source, kind, mutable)) = self.pointer_view(expr)
            && let Some(binding) = self.binding_for_name(source)
        {
            self.pointer_views.push(StringPointerViewFact {
                site: Site {
                    function: self.function,
                    path: AstPath(path.to_vec()),
                },
                source: binding,
                mutable,
                kind,
            });
        }
        if let Some(callee) = libc_function(expr) {
            let pointer_args = libc_pointer_args(expr)
                .into_iter()
                .filter_map(|source| self.binding_for_name(source))
                .collect();
            self.libc_uses.push(StringLibcUseFact {
                site: Site {
                    function: self.function,
                    path: AstPath(path.to_vec()),
                },
                callee,
                pointer_args,
            });
        }

        match expr {
            Expr::Value(_)
            | Expr::Str(_)
            | Expr::HexFloat(_)
            | Expr::ByteStr(_)
            | Expr::CStr(_)
            | Expr::Var(_)
            | Expr::Path(_)
            | Expr::Todo(_)
            | Expr::AtomicFence { .. } => {}
            Expr::Unary {
                op: UnaryOp::Deref,
                expr,
            } => walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(expr, path)),
            Expr::Unary { expr, .. }
            | Expr::Cast { expr, .. }
            | Expr::Ref { expr, .. }
            | Expr::AddrOf { expr, .. }
            | Expr::Transmute { expr, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(expr, path));
            }
            Expr::Binary { lhs, rhs, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(lhs, path));
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(rhs, path));
            }
            Expr::Range { start, end } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(start, path));
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(end, path));
            }
            Expr::Call { func, args } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(func, path));
                for (index, arg) in args.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index + 1), |path| {
                        self.expr(arg, path)
                    });
                }
            }
            Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(recv, path));
                for (index, arg) in args.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index + 1), |path| {
                        self.expr(arg, path)
                    });
                }
            }
            Expr::Field { base, .. } | Expr::TupleField { base, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(base, path));
            }
            Expr::ArrayPtr { array, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(array, path));
            }
            Expr::Index { base, index } => {
                if let Expr::Var(name) = &**base
                    && let Some(binding) = self.binding_for_name(name.as_str())
                    && let Some(summary) = self.summaries.get_mut(&binding)
                {
                    summary.rejections.insert(StringBufferRejection::Indexed);
                }
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(base, path));
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(index, path));
            }
            Expr::StructLit { fields, .. } => {
                for (index, (_, value)) in fields.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index), |path| {
                        self.expr(value, path)
                    });
                }
            }
            Expr::TupleStructLit { fields, .. } => {
                for (index, value) in fields.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index), |path| {
                        self.expr(value, path)
                    });
                }
            }
            Expr::ArrayLit(elems) | Expr::VecLit(elems) | Expr::Macro { args: elems, .. } => {
                for (index, elem) in elems.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index), |path| {
                        self.expr(elem, path)
                    });
                }
            }
            Expr::ArrayRepeat { elem, .. } | Expr::Closure { body: elem, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(elem, path));
            }
            Expr::VecRepeat { elem, len } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(elem, path));
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(len, path));
            }
            Expr::Match { expr, arms } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(expr, path));
                for (index, arm) in arms.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index + 1), |path| {
                        self.expr(&arm.value, path)
                    });
                }
            }
            Expr::If {
                cond,
                then_expr,
                else_expr,
            } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(cond, path));
                walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                    self.expr(then_expr, path)
                });
                walk::with_path_segment(path, PathSegment::Expr(2), |path| {
                    self.expr(else_expr, path)
                });
            }
            Expr::Block(block) => {
                walk::with_path_segment(path, PathSegment::BlockBody, |path| {
                    self.block(block, path)
                });
            }
            Expr::Unsafe(block) => {
                walk::with_path_segment(path, PathSegment::UnsafeBody, |path| {
                    self.block(block, path)
                });
            }
            Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. } => {
                if let Some(ptr) = place.ptr_expr() {
                    walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                        self.expr(ptr, path)
                    });
                }
            }
            Expr::AtomicStore { place, value, .. }
            | Expr::AtomicFetch { place, value, .. }
            | Expr::AtomicSwap { place, value, .. } => {
                if let Some(ptr) = place.ptr_expr() {
                    walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                        self.expr(ptr, path)
                    });
                }
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(value, path));
            }
            Expr::AtomicNew { value, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(value, path));
            }
            Expr::AtomicCompareExchange {
                place,
                expected,
                desired,
                ..
            } => {
                if let Some(ptr) = place.ptr_expr() {
                    walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                        self.expr(ptr, path)
                    });
                }
                walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                    self.expr(expected, path)
                });
                walk::with_path_segment(path, PathSegment::Expr(2), |path| {
                    self.expr(desired, path)
                });
            }
            Expr::CopyNonoverlapping { src, dst, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(src, path));
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(dst, path));
            }
            Expr::PtrCopy {
                src, dst, count, ..
            } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(src, path));
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(dst, path));
                walk::with_path_segment(path, PathSegment::Expr(2), |path| self.expr(count, path));
            }
            Expr::WriteBytes { dst, val, count } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(dst, path));
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(val, path));
                walk::with_path_segment(path, PathSegment::Expr(2), |path| self.expr(count, path));
            }
        }
    }

    fn define_local(
        &mut self,
        name: &str,
        ty: Option<&Type>,
        init: Option<&Expr>,
        path: &[PathSegment],
    ) {
        let binding = self.local_binding(name, path);
        self.bind(name.to_string(), binding);
        let Some(binding) = binding else {
            return;
        };
        let Some(ty) = ty else {
            return;
        };
        let Some(summary) = self.summary_for_binding(binding, ty, init, AstPath(path.to_vec()))
        else {
            return;
        };
        self.summaries.insert(binding, summary);
    }

    fn local_binding(&self, name: &str, path: &[PathSegment]) -> Option<BindingId> {
        self.facts
            .binding_by_local_path(self.function, name, &AstPath(path.to_vec()))
    }

    fn summary_for_binding(
        &self,
        binding: BindingId,
        ty: &Type,
        init: Option<&Expr>,
        path: AstPath,
    ) -> Option<BufferSummary> {
        if is_char_array(ty) {
            let mut summary = BufferSummary::new(binding, path, StringBufferKind::CharArray);
            match init {
                Some(expr) if is_zero_array(expr) => {
                    summary.provenance = StringBufferProvenance::ZeroInitialized;
                    summary.nul_termination = NulTermination::AllZero;
                    summary.bytes = Some(Vec::new());
                    summary.ascii_only = true;
                }
                Some(expr) => match literal_bytes(expr) {
                    Some(literal) => {
                        summary.apply_literal(literal, StringBufferProvenance::Literal);
                    }
                    None => {
                        summary
                            .rejections
                            .insert(StringBufferRejection::UnsupportedInitializer);
                    }
                },
                None => {
                    summary.provenance = StringBufferProvenance::Unknown;
                }
            }
            return Some(summary);
        }

        let kind = lifted_kind(ty)?;
        let mut summary = BufferSummary::new(binding, path, kind);
        summary.provenance = StringBufferProvenance::Lifted;
        summary.nul_termination = NulTermination::NotApplicable;
        if let Some(init) = init {
            match init {
                Expr::Str(s) => {
                    summary.bytes = Some(s.as_bytes().to_vec());
                    summary.interior_nul = s.as_bytes().contains(&0);
                    summary.ascii_only = s.is_ascii();
                }
                Expr::ByteStr(bytes) => {
                    summary.bytes = Some(bytes.clone());
                    summary.interior_nul = bytes.contains(&0);
                    summary.ascii_only = bytes.is_ascii();
                }
                Expr::CStr(bytes) => {
                    summary.bytes = Some(bytes.clone());
                    summary.nul_termination = NulTermination::Terminated;
                    summary.interior_nul = false;
                    summary.ascii_only = bytes.is_ascii();
                }
                _ => {}
            }
        }
        Some(summary)
    }

    fn assign(&mut self, target: &Expr, value: &Expr, path: &mut Vec<PathSegment>) {
        match target {
            Expr::Var(name) => {
                let Some(binding) = self.binding_for_name(name.as_str()) else {
                    return;
                };
                let Some(summary) = self.summaries.get_mut(&binding) else {
                    return;
                };
                if summary.kind != StringBufferKind::CharArray {
                    summary.rejections.insert(StringBufferRejection::Mutated);
                    return;
                }
                if !matches!(
                    summary.provenance,
                    StringBufferProvenance::ZeroInitialized | StringBufferProvenance::Unknown
                ) {
                    summary.rejections.insert(StringBufferRejection::Mutated);
                    return;
                }
                match literal_bytes(value) {
                    Some(literal) => {
                        summary.apply_literal(
                            literal,
                            StringBufferProvenance::AssignedLiteral {
                                assignment: AstPath(path.to_vec()),
                            },
                        );
                    }
                    None => {
                        summary.rejections.insert(StringBufferRejection::Mutated);
                    }
                }
            }
            Expr::Field { base, .. } | Expr::TupleField { base, .. } => {
                self.assign(base, value, path)
            }
            Expr::Index { base, index } => {
                self.mutate_target(base, path);
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(index, path));
            }
            Expr::Unary {
                op: UnaryOp::Deref,
                expr,
            } => walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(expr, path)),
            Expr::Cast { expr, .. } => self.assign(expr, value, path),
            _ => self.expr(target, path),
        }
    }

    fn mutate_target(&mut self, target: &Expr, path: &mut Vec<PathSegment>) {
        match target {
            Expr::Var(name) => {
                if let Some(binding) = self.binding_for_name(name.as_str())
                    && let Some(summary) = self.summaries.get_mut(&binding)
                {
                    summary.rejections.insert(StringBufferRejection::Mutated);
                }
            }
            Expr::Field { base, .. } | Expr::TupleField { base, .. } => {
                self.mutate_target(base, path)
            }
            Expr::Index { base, index } => {
                self.mutate_target(base, path);
                walk::with_path_segment(path, PathSegment::Expr(1), |path| self.expr(index, path));
            }
            Expr::Unary {
                op: UnaryOp::Deref,
                expr,
            } => walk::with_path_segment(path, PathSegment::Expr(0), |path| self.expr(expr, path)),
            Expr::Cast { expr, .. } => self.mutate_target(expr, path),
            _ => self.expr(target, path),
        }
    }

    fn pointer_view<'b>(&self, expr: &'b Expr) -> Option<(&'b str, StringPointerViewKind, bool)> {
        match expr {
            Expr::MethodCall { recv, method, args } if args.is_empty() => {
                let source = var_name(recv)?;
                match method.as_str() {
                    "as_ptr" => Some((source, StringPointerViewKind::As, false)),
                    "as_mut_ptr" => Some((source, StringPointerViewKind::AsMut, true)),
                    _ => None,
                }
            }
            Expr::ArrayPtr { array, mutable } => {
                Some((var_name(array)?, StringPointerViewKind::Array, *mutable))
            }
            Expr::Cast { expr, .. }
            | Expr::Unary { expr, .. }
            | Expr::Ref { expr, .. }
            | Expr::AddrOf { expr, .. }
            | Expr::Transmute { expr, .. } => self.pointer_view(expr),
            _ => None,
        }
    }

    fn shadow_pattern(&mut self, pattern: &Pattern) {
        match pattern {
            Pattern::Binding(name) => self.bind(name.to_string(), None),
            Pattern::TupleStruct { fields, .. } => {
                for field in fields {
                    self.shadow_pattern(field);
                }
            }
            Pattern::Wildcard | Pattern::I64(_) | Pattern::I128(_) | Pattern::U128(_) => {}
        }
    }

    fn bind(&mut self, name: String, binding: Option<BindingId>) {
        if let Some(scope) = self.scopes.last_mut() {
            scope.insert(name, binding);
        }
    }

    fn binding_for_name(&self, name: &str) -> Option<BindingId> {
        self.scopes
            .iter()
            .rev()
            .find_map(|scope| scope.get(name).copied())
            .flatten()
    }
}

impl BufferSummary {
    fn new(binding: BindingId, path: AstPath, kind: StringBufferKind) -> Self {
        Self {
            binding,
            path,
            kind,
            provenance: StringBufferProvenance::Unknown,
            bytes: None,
            nul_termination: NulTermination::NotApplicable,
            interior_nul: false,
            ascii_only: false,
            rejections: BTreeSet::new(),
        }
    }

    fn apply_literal(&mut self, literal: LiteralBytes, provenance: StringBufferProvenance) {
        self.provenance = provenance;
        self.bytes = Some(literal.bytes);
        self.nul_termination = literal.nul_termination;
        self.interior_nul = literal.interior_nul;
        self.ascii_only = self.bytes.as_ref().is_some_and(|bytes| bytes.is_ascii());
        if self.nul_termination == NulTermination::Unterminated {
            self.rejections.insert(StringBufferRejection::Unterminated);
        }
    }

    fn into_fact(self, function: FunctionId) -> StringBufferFact {
        let candidates = self.candidates();
        StringBufferFact {
            site: Site {
                function,
                path: self.path,
            },
            binding: self.binding,
            kind: self.kind,
            provenance: self.provenance,
            bytes: self.bytes,
            nul_termination: self.nul_termination,
            interior_nul: self.interior_nul,
            ascii_only: self.ascii_only,
            candidates,
            rejections: self.rejections,
        }
    }

    fn candidates(&self) -> BTreeSet<StringRecoveryCandidate> {
        if !self.rejections.is_empty() {
            return BTreeSet::new();
        }
        match self.kind {
            StringBufferKind::BorrowedStr => BTreeSet::from([StringRecoveryCandidate::BorrowedStr]),
            StringBufferKind::BorrowedCStr => {
                BTreeSet::from([StringRecoveryCandidate::BorrowedCStr])
            }
            StringBufferKind::BorrowedBytes => {
                BTreeSet::from([StringRecoveryCandidate::BorrowedBytes])
            }
            StringBufferKind::OwnedString => BTreeSet::from([StringRecoveryCandidate::OwnedString]),
            StringBufferKind::CharArray => match self.provenance {
                StringBufferProvenance::ZeroInitialized => {
                    BTreeSet::from([StringRecoveryCandidate::OwnedString])
                }
                StringBufferProvenance::Literal
                | StringBufferProvenance::AssignedLiteral { .. }
                    if matches!(
                        self.nul_termination,
                        NulTermination::Terminated | NulTermination::AllZero
                    ) =>
                {
                    match &self.bytes {
                        Some(bytes) if !self.interior_nul && std::str::from_utf8(bytes).is_ok() => {
                            let mut candidates = BTreeSet::from([
                                StringRecoveryCandidate::BorrowedStr,
                                StringRecoveryCandidate::OwnedString,
                            ]);
                            if self.ascii_only {
                                candidates.insert(StringRecoveryCandidate::BorrowedCStr);
                            }
                            candidates
                        }
                        Some(_) => BTreeSet::from([StringRecoveryCandidate::BorrowedBytes]),
                        None => BTreeSet::new(),
                    }
                }
                _ => BTreeSet::new(),
            },
        }
    }
}

fn is_char_array(ty: &Type) -> bool {
    matches!(
        ty,
        Type::Array { elem, .. } if matches!(&**elem, Type::Prim(Prim::I8 | Prim::U8))
    )
}

fn lifted_kind(ty: &Type) -> Option<StringBufferKind> {
    match ty {
        Type::Custom(name) if name == "String" => Some(StringBufferKind::OwnedString),
        Type::Ref {
            mutable: false,
            inner,
        } => match &**inner {
            Type::Str => Some(StringBufferKind::BorrowedStr),
            Type::Custom(name) if name == "core::ffi::CStr" => Some(StringBufferKind::BorrowedCStr),
            Type::Slice(elem) if matches!(&**elem, Type::Prim(Prim::U8)) => {
                Some(StringBufferKind::BorrowedBytes)
            }
            _ => None,
        },
        _ => None,
    }
}

fn is_zero_array(expr: &Expr) -> bool {
    matches!(
        expr,
        Expr::ArrayRepeat { elem, .. }
            if matches!(&**elem, Expr::Value(RustValue::I64(0) | RustValue::I128(0)))
    )
}

fn literal_bytes(expr: &Expr) -> Option<LiteralBytes> {
    match expr {
        Expr::ArrayLit(elems) => {
            let bytes = elems.iter().map(byte_literal).collect::<Option<Vec<_>>>()?;
            Some(classify_bytes(bytes))
        }
        Expr::ByteStr(bytes) => Some(classify_bytes(bytes.clone())),
        Expr::Str(s) => Some(LiteralBytes {
            bytes: s.as_bytes().to_vec(),
            nul_termination: NulTermination::NotApplicable,
            interior_nul: s.as_bytes().contains(&0),
        }),
        Expr::Cast { expr, .. } => literal_bytes(expr),
        _ => None,
    }
}

fn classify_bytes(mut bytes: Vec<u8>) -> LiteralBytes {
    let Some(nul) = bytes.iter().position(|byte| *byte == 0) else {
        return LiteralBytes {
            bytes,
            nul_termination: NulTermination::Unterminated,
            interior_nul: false,
        };
    };
    let all_after_nul = bytes[nul..].iter().all(|byte| *byte == 0);
    let all_zero = bytes.iter().all(|byte| *byte == 0);
    let interior_nul = !all_after_nul;
    let payload = if all_after_nul {
        bytes.truncate(nul);
        bytes
    } else {
        bytes
    };
    LiteralBytes {
        bytes: payload,
        nul_termination: if all_zero {
            NulTermination::AllZero
        } else {
            NulTermination::Terminated
        },
        interior_nul,
    }
}

fn byte_literal(expr: &Expr) -> Option<u8> {
    let n = match expr {
        Expr::Value(RustValue::I64(n)) => *n,
        Expr::Value(RustValue::I128(n)) => i64::try_from(*n).ok()?,
        Expr::Cast { expr, .. } => return byte_literal(expr),
        _ => return None,
    };
    if (-128..=255).contains(&n) {
        Some(n as u8)
    } else {
        None
    }
}

fn var_name(expr: &Expr) -> Option<&str> {
    match expr {
        Expr::Var(name) => Some(name.as_str()),
        Expr::Cast { expr, .. }
        | Expr::Unary { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => var_name(expr),
        _ => None,
    }
}

fn libc_function(expr: &Expr) -> Option<StringLibcFunction> {
    let Expr::Call { func, .. } = peel_empty_unsafe(expr) else {
        return None;
    };
    let Expr::Var(name) = &**func else {
        return None;
    };
    Some(match name.as_str() {
        "strlen" => StringLibcFunction::StrLen,
        "strcmp" => StringLibcFunction::StrCmp,
        "strncmp" => StringLibcFunction::StrNCmp,
        "memcmp" => StringLibcFunction::MemCmp,
        "strchr" => StringLibcFunction::StrChr,
        "strrchr" => StringLibcFunction::StrRChr,
        "strstr" => StringLibcFunction::StrStr,
        "strpbrk" => StringLibcFunction::StrPBrk,
        "strspn" => StringLibcFunction::StrSpn,
        "strcspn" => StringLibcFunction::StrCSpn,
        "strcpy" => StringLibcFunction::StrCpy,
        "strncpy" => StringLibcFunction::StrNCpy,
        "strcat" => StringLibcFunction::StrCat,
        "strncat" => StringLibcFunction::StrNCat,
        "atoi" => StringLibcFunction::Atoi,
        "atol" => StringLibcFunction::Atol,
        "strtol" => StringLibcFunction::StrTol,
        "strtoul" => StringLibcFunction::StrToul,
        "strtod" => StringLibcFunction::StrTod,
        "printf" => StringLibcFunction::Printf,
        _ => return None,
    })
}

fn libc_pointer_args(expr: &Expr) -> Vec<&str> {
    let Some(callee) = libc_function(expr) else {
        return Vec::new();
    };
    let Expr::Call { args, .. } = peel_empty_unsafe(expr) else {
        return Vec::new();
    };
    let pointer_args = match callee {
        StringLibcFunction::StrLen
        | StringLibcFunction::StrChr
        | StringLibcFunction::StrRChr
        | StringLibcFunction::Atoi
        | StringLibcFunction::Atol
        | StringLibcFunction::StrTol
        | StringLibcFunction::StrToul
        | StringLibcFunction::StrTod => &args[..args.len().min(1)],
        StringLibcFunction::StrCmp
        | StringLibcFunction::StrNCmp
        | StringLibcFunction::MemCmp
        | StringLibcFunction::StrStr
        | StringLibcFunction::StrPBrk
        | StringLibcFunction::StrSpn
        | StringLibcFunction::StrCSpn
        | StringLibcFunction::StrCpy
        | StringLibcFunction::StrNCpy
        | StringLibcFunction::StrCat
        | StringLibcFunction::StrNCat => &args[..args.len().min(2)],
        StringLibcFunction::Printf => args,
    };
    pointer_args.iter().filter_map(pointer_source).collect()
}

fn peel_empty_unsafe(expr: &Expr) -> &Expr {
    if let Expr::Unsafe(block) = expr
        && block.stmts.is_empty()
        && let Some(tail) = &block.tail
    {
        return tail;
    }
    expr
}

fn pointer_source(expr: &Expr) -> Option<&str> {
    match expr {
        Expr::Var(name) => Some(name.as_str()),
        Expr::MethodCall { recv, method, args }
            if args.is_empty() && matches!(method.as_str(), "as_ptr" | "as_mut_ptr") =>
        {
            pointer_source(recv)
        }
        Expr::ArrayPtr { array, .. } => pointer_source(array),
        Expr::Cast { expr, .. }
        | Expr::Unary { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => pointer_source(expr),
        _ => None,
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::facts;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Block, Expr, Item, Program, Stmt};

    fn analyzed(stmts: Vec<Stmt>) -> facts::FixupFacts {
        facts::analyze(Program {
            items: vec![Item::Fn(func(vec![], None, stmts))],
        })
        .facts
    }

    fn bytes(values: &[i64]) -> Expr {
        Expr::ArrayLit(values.iter().copied().map(int).collect())
    }

    fn binding_for(facts: &facts::FixupFacts, name: &str, path: AstPath) -> BindingId {
        facts
            .bindings
            .iter()
            .find(|binding| binding.name == name && binding.path == path)
            .unwrap()
            .id
    }

    fn buffer_for(facts: &facts::FixupFacts, binding: BindingId) -> &StringBufferFact {
        facts
            .string_buffers
            .iter()
            .find(|fact| fact.binding == binding)
            .unwrap()
    }

    fn numeric_for(
        facts: &facts::FixupFacts,
        binding: BindingId,
    ) -> Option<&AsciiNumericStringFact> {
        facts
            .ascii_numeric_strings
            .iter()
            .find(|fact| fact.binding == binding)
    }

    #[test]
    fn records_literal_and_zero_initialized_char_buffers() {
        let facts = analyzed(vec![
            let_mut("literal", "[i8; 3]", bytes(&[104, 105, 0])),
            let_mut(
                "zero",
                "[i8; 4]",
                Expr::ArrayRepeat {
                    elem: Box::new(int(0)),
                    len: 4,
                },
            ),
        ]);
        let literal = binding_for(&facts, "literal", AstPath(vec![PathSegment::Stmt(0)]));
        let zero = binding_for(&facts, "zero", AstPath(vec![PathSegment::Stmt(1)]));

        let literal = buffer_for(&facts, literal);
        assert_eq!(literal.provenance, StringBufferProvenance::Literal);
        assert_eq!(literal.bytes, Some(b"hi".to_vec()));
        assert_eq!(literal.nul_termination, NulTermination::Terminated);
        assert!(
            literal
                .candidates
                .contains(&StringRecoveryCandidate::BorrowedStr)
        );
        assert!(
            literal
                .candidates
                .contains(&StringRecoveryCandidate::OwnedString)
        );

        let zero = buffer_for(&facts, zero);
        assert_eq!(zero.provenance, StringBufferProvenance::ZeroInitialized);
        assert_eq!(zero.nul_termination, NulTermination::AllZero);
        assert!(
            zero.candidates
                .contains(&StringRecoveryCandidate::OwnedString)
        );
    }

    #[test]
    fn records_signed_i8_string_literal_bytes() {
        let facts = analyzed(vec![let_mut("utf8", "[i8; 4]", bytes(&[104, -61, -87, 0]))]);
        let utf8 = buffer_for(
            &facts,
            binding_for(&facts, "utf8", AstPath(vec![PathSegment::Stmt(0)])),
        );

        assert_eq!(utf8.bytes, Some("hé".as_bytes().to_vec()));
    }

    #[test]
    fn records_ascii_numeric_whole_tokens_only() {
        let facts = analyzed(vec![
            let_mut("positive", "[i8; 4]", bytes(&[52, 50, 48, 0])),
            let_mut("negative", "[i8; 3]", bytes(&[45, 55, 0])),
            let_mut("prefixed", "[i8; 4]", bytes(&[32, 52, 50, 0])),
            let_mut("partial", "[i8; 5]", bytes(&[52, 50, 120, 0, 0])),
            let_mut("sign_only", "[i8; 2]", bytes(&[45, 0])),
        ]);
        let positive = binding_for(&facts, "positive", AstPath(vec![PathSegment::Stmt(0)]));
        let negative = binding_for(&facts, "negative", AstPath(vec![PathSegment::Stmt(1)]));
        let prefixed = binding_for(&facts, "prefixed", AstPath(vec![PathSegment::Stmt(2)]));
        let partial = binding_for(&facts, "partial", AstPath(vec![PathSegment::Stmt(3)]));
        let sign_only = binding_for(&facts, "sign_only", AstPath(vec![PathSegment::Stmt(4)]));

        let positive = numeric_for(&facts, positive).unwrap();
        assert_eq!(positive.sign, AsciiNumericSign::None);
        assert_eq!(positive.digits, 3);
        assert_eq!(
            numeric_for(&facts, negative).unwrap().sign,
            AsciiNumericSign::Minus
        );
        assert!(numeric_for(&facts, prefixed).is_none());
        assert!(numeric_for(&facts, partial).is_none());
        assert!(numeric_for(&facts, sign_only).is_none());
    }

    #[test]
    fn records_assigned_c_strings_and_interior_nul_bytes() {
        let facts = analyzed(vec![
            let_mut(
                "s",
                "[i8; 5]",
                Expr::ArrayRepeat {
                    elem: Box::new(int(0)),
                    len: 5,
                },
            ),
            assign("s", bytes(&[97, 0, 98, 0, 0])),
        ]);
        let s = binding_for(&facts, "s", AstPath(vec![PathSegment::Stmt(0)]));
        let fact = buffer_for(&facts, s);

        assert_eq!(
            fact.provenance,
            StringBufferProvenance::AssignedLiteral {
                assignment: AstPath(vec![PathSegment::Stmt(1)])
            }
        );
        assert!(fact.interior_nul);
        assert!(
            fact.candidates
                .contains(&StringRecoveryCandidate::BorrowedBytes)
        );
        assert!(
            !fact
                .candidates
                .contains(&StringRecoveryCandidate::BorrowedStr)
        );
    }

    #[test]
    fn records_lifted_borrowed_and_owned_string_values() {
        let facts = analyzed(vec![
            temp("s", "&str", Expr::Str("hi".into())),
            temp("b", "&[u8]", Expr::ByteStr(b"hi".to_vec())),
            temp("owned", "String", Expr::Str("hi".into())),
        ]);

        let s = buffer_for(
            &facts,
            binding_for(&facts, "s", AstPath(vec![PathSegment::Stmt(0)])),
        );
        assert_eq!(s.kind, StringBufferKind::BorrowedStr);
        assert!(s.candidates.contains(&StringRecoveryCandidate::BorrowedStr));

        let b = buffer_for(
            &facts,
            binding_for(&facts, "b", AstPath(vec![PathSegment::Stmt(1)])),
        );
        assert_eq!(b.kind, StringBufferKind::BorrowedBytes);
        assert!(
            b.candidates
                .contains(&StringRecoveryCandidate::BorrowedBytes)
        );

        let owned = buffer_for(
            &facts,
            binding_for(&facts, "owned", AstPath(vec![PathSegment::Stmt(2)])),
        );
        assert_eq!(owned.kind, StringBufferKind::OwnedString);
        assert!(
            owned
                .candidates
                .contains(&StringRecoveryCandidate::OwnedString)
        );
    }

    #[test]
    fn records_pointer_views_and_libc_use_paths() {
        let facts = analyzed(vec![
            let_mut("s", "[i8; 3]", bytes(&[104, 105, 0])),
            Stmt::Expr(Expr::Unsafe(Box::new(Block {
                stmts: vec![],
                tail: Some(Box::new(call(
                    "printf",
                    vec![
                        Expr::ByteStr(b"%s\n\0".to_vec()),
                        Expr::ArrayPtr {
                            array: Box::new(var("s")),
                            mutable: true,
                        },
                    ],
                ))),
            }))),
        ]);
        let s = binding_for(&facts, "s", AstPath(vec![PathSegment::Stmt(0)]));

        let pointer = facts
            .string_pointer_views
            .iter()
            .find(|fact| fact.source == s)
            .unwrap();
        assert_eq!(pointer.kind, StringPointerViewKind::Array);
        assert!(pointer.mutable);
        assert_eq!(
            pointer.site.path,
            AstPath(vec![
                PathSegment::Stmt(1),
                PathSegment::UnsafeBody,
                PathSegment::BlockTail,
                PathSegment::Expr(2)
            ])
        );

        let libc = facts
            .string_libc_uses
            .iter()
            .find(|fact| fact.callee == StringLibcFunction::Printf)
            .unwrap();
        assert_eq!(libc.site.path, AstPath(vec![PathSegment::Stmt(1)]));
        assert_eq!(libc.pointer_args, vec![s]);
    }

    #[test]
    fn rejects_indexed_mutated_and_unterminated_buffers() {
        let facts = analyzed(vec![
            let_mut("indexed", "[i8; 3]", bytes(&[104, 105, 0])),
            Stmt::Expr(Expr::Index {
                base: Box::new(var("indexed")),
                index: Box::new(int(0)),
            }),
            let_mut("mutated", "[i8; 3]", bytes(&[104, 105, 0])),
            assign("mutated", bytes(&[98, 121, 0])),
            let_mut("unterminated", "[i8; 2]", bytes(&[104, 105])),
        ]);

        let indexed = buffer_for(
            &facts,
            binding_for(&facts, "indexed", AstPath(vec![PathSegment::Stmt(0)])),
        );
        assert!(indexed.rejections.contains(&StringBufferRejection::Indexed));
        assert!(indexed.candidates.is_empty());

        let mutated = buffer_for(
            &facts,
            binding_for(&facts, "mutated", AstPath(vec![PathSegment::Stmt(2)])),
        );
        assert!(mutated.rejections.contains(&StringBufferRejection::Mutated));
        assert!(mutated.candidates.is_empty());

        let unterminated = buffer_for(
            &facts,
            binding_for(&facts, "unterminated", AstPath(vec![PathSegment::Stmt(4)])),
        );
        assert_eq!(unterminated.nul_termination, NulTermination::Unterminated);
        assert!(
            unterminated
                .rejections
                .contains(&StringBufferRejection::Unterminated)
        );
    }
}
