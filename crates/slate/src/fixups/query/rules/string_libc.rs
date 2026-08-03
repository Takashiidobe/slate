use crate::fixups::facts::{AsciiNumericSign, AstPath, PathSegment, StringLibcFunction};
use crate::fixups::runtime;
use crate::fixups::trace::Pass;
use crate::function_identity::Known;
use crate::rust_ast::{BinOp, Block, Expr, Ident, Path, Prim, RustValue, Stmt, Type, UnaryOp};

use super::super::{
    BindingRef, CallRecord, CallTarget, EditSet, ExprSite, ExpressionRef, FnCall, ItemCaseContext,
    QueryRule, Rejection, ValueSite,
};

pub(in crate::fixups) fn calls() -> QueryRule<FnCall> {
    QueryRule::new(
        Pass::StringLibc,
        "rewrite_string_libc_idioms",
        FnCall::default(),
    )
    .case("strlen", strlen_case)
    .case("strcmp", strcmp_case)
    .case("strncmp", strncmp_case)
    .case("memcmp", memcmp_case)
    .case("strchr", strchr_case)
    .case("strrchr", strrchr_case)
    .case("strstr", strstr_case)
    .case("strpbrk", strpbrk_case)
    .case("strspn", strspn_case)
    .case("strcspn", strcspn_case)
    .case("atoi", atoi_case)
    .case("atol", atol_case)
    .case("strtol", strtol_case)
    .case("strtoul", strtoul_case)
    .case("strtod", strtod_case)
}

#[derive(Clone, Copy, PartialEq, Eq)]
enum StringKind {
    Str,
    Bytes,
}

#[derive(Clone)]
struct Source {
    name: String,
    kind: StringKind,
    ascii_only: bool,
}

#[derive(Clone)]
struct Compare {
    lhs: Expr,
    rhs: Expr,
}

fn edit_target(call: &CallRecord) -> ExprSite {
    call.trivial_unsafe_site
        .clone()
        .unwrap_or_else(|| call.site.clone())
}

/// A `Let`'s initializer sits one `Expr(0)` segment below the statement's own
/// path; anything deeper means the site is nested inside a larger initializer,
/// not the direct root the retype/collapse rewrites require.
fn is_direct_statement_root(statement: &AstPath, site: &AstPath) -> bool {
    site.0.len() == statement.0.len() + 1 && site.0.last() == Some(&PathSegment::Expr(0))
}

/// `strlen`/`strspn`/`strcspn` lower as a C `size_t` result (`u64`), but their Rust
/// replacements return `usize`; when the call is the direct initializer of a `let`,
/// the declared type must move with it or the statement stops compiling.
fn usize_call_edit(
    case: &mut ItemCaseContext<'_, '_>,
    call: &CallRecord,
    replacement: Expr,
) -> Result<EditSet, Rejection> {
    let target = edit_target(call);
    let statement_ref = case.fact(|query| {
        query.enclosing_statement(&ExpressionRef {
            site: target.clone(),
        })
    });
    if let Ok(statement_ref) = statement_ref
        && is_direct_statement_root(&statement_ref.path, &target.path)
        && let Ok(stmt) = case.fact(|query| query.statement(&statement_ref))
        && let Stmt::Let {
            name,
            mutable,
            ty: Some(Type::Prim(Prim::U64)),
            init: Some(_),
        } = &stmt.stmt
    {
        let name = name.clone();
        let mutable = *mutable;
        let mut edits = EditSet::new();
        edits.push_replace_statement(
            statement_ref.item_index,
            statement_ref.path.clone(),
            Some(Stmt::Let {
                name,
                mutable,
                ty: Some(Type::Prim(Prim::Usize)),
                init: Some(replacement),
            }),
        );
        return Ok(edits);
    }
    Ok(EditSet::replace_expression(edit_target(call), replacement))
}

fn source_for_binding(
    case: &mut ItemCaseContext<'_, '_>,
    binding: &BindingRef,
) -> Result<Source, Rejection> {
    let buffer = case.fact(|query| query.string_buffer(&binding.value_site()))?;
    let kind = string_kind_for_buffer(buffer.kind).ok_or_else(|| case.reject())?;
    Ok(Source {
        name: binding.name.clone(),
        kind,
        ascii_only: buffer.ascii_only,
    })
}

fn string_kind_for_buffer(kind: crate::fixups::facts::StringBufferKind) -> Option<StringKind> {
    use crate::fixups::facts::StringBufferKind;
    match kind {
        StringBufferKind::BorrowedStr | StringBufferKind::OwnedString => Some(StringKind::Str),
        StringBufferKind::BorrowedBytes => Some(StringKind::Bytes),
        StringBufferKind::BorrowedCStr | StringBufferKind::CharArray => None,
    }
}

fn strlen_case(
    case: &mut ItemCaseContext<'_, '_>,
    call: &CallRecord,
) -> Result<EditSet, Rejection> {
    case.require(matches!(call.target, CallTarget::Known(Known::StrLen)))?;
    case.require(call.args.len() == 1)?;
    let usage = case.fact(|query| query.string_libc_use(&call.site))?;
    case.require(usage.callee == StringLibcFunction::StrLen)?;
    case.require(usage.pointer_args.len() == 1)?;
    let source = source_for_binding(case, &usage.pointer_args[0])?;
    usize_call_edit(case, call, strlen_replacement(source))
}

fn strcmp_case(
    case: &mut ItemCaseContext<'_, '_>,
    call: &CallRecord,
) -> Result<EditSet, Rejection> {
    case.require(matches!(call.target, CallTarget::Known(Known::StrCmp)))?;
    case.require(call.args.len() == 2)?;
    let usage = case.fact(|query| query.string_libc_use(&call.site))?;
    case.require(usage.callee == StringLibcFunction::StrCmp)?;
    case.require(usage.pointer_args.len() == 2)?;
    let lhs = source_for_binding(case, &usage.pointer_args[0])?;
    let rhs = source_for_binding(case, &usage.pointer_args[1])?;
    let (lhs, rhs) = comparable(lhs, rhs).ok_or_else(|| case.reject())?;
    compare_call_edit(case, call, Compare { lhs, rhs })
}

fn strncmp_case(
    case: &mut ItemCaseContext<'_, '_>,
    call: &CallRecord,
) -> Result<EditSet, Rejection> {
    case.require(matches!(call.target, CallTarget::Known(Known::StrNCmp)))?;
    case.require(call.args.len() == 3)?;
    let usage = case.fact(|query| query.string_libc_use(&call.site))?;
    case.require(usage.callee == StringLibcFunction::StrNCmp)?;
    case.require(usage.pointer_args.len() == 2)?;
    let lhs = source_for_binding(case, &usage.pointer_args[0])?;
    let rhs = source_for_binding(case, &usage.pointer_args[1])?;
    let count = case
        .expr(&call.args[2])
        .cloned()
        .ok_or_else(|| case.reject())?;
    let compare = Compare {
        lhs: prefix(lhs, count.clone()),
        rhs: prefix(rhs, count),
    };
    compare_call_edit(case, call, compare)
}

fn memcmp_case(
    case: &mut ItemCaseContext<'_, '_>,
    call: &CallRecord,
) -> Result<EditSet, Rejection> {
    case.require(matches!(call.target, CallTarget::Known(Known::MemCmp)))?;
    case.require(call.args.len() == 3)?;
    let usage = case.fact(|query| query.string_libc_use(&call.site))?;
    case.require(usage.callee == StringLibcFunction::MemCmp)?;
    case.require(usage.pointer_args.len() == 2)?;
    let lhs = source_for_binding(case, &usage.pointer_args[0])?;
    let rhs = source_for_binding(case, &usage.pointer_args[1])?;
    let count = case
        .expr(&call.args[2])
        .cloned()
        .ok_or_else(|| case.reject())?;
    let compare = Compare {
        lhs: prefix(lhs, count.clone()),
        rhs: prefix(rhs, count),
    };
    compare_call_edit(case, call, compare)
}

fn compare_call_edit(
    case: &mut ItemCaseContext<'_, '_>,
    call: &CallRecord,
    compare: Compare,
) -> Result<EditSet, Rejection> {
    if let Some(edits) = direct_zero_comparison_edit(case, call, &compare) {
        return Ok(edits);
    }
    if let Some(edits) = temp_zero_comparison_edit(case, call, &compare) {
        return Ok(edits);
    }
    Ok(EditSet::replace_expression(
        edit_target(call),
        cmp_to_i32(compare_expr(compare)),
    ))
}

/// `strcmp(a, b) == 0` (or any comparison op against a literal zero) directly
/// wrapping the call - the common case when the C source compares the result inline.
fn direct_zero_comparison_edit(
    case: &mut ItemCaseContext<'_, '_>,
    call: &CallRecord,
    compare: &Compare,
) -> Option<EditSet> {
    let parent = case
        .fact(|query| {
            query.parent_expression(&ExpressionRef {
                site: edit_target(call),
            })
        })
        .ok()?;
    let op = case.expr(&parent.site).and_then(zero_comparison_op)?;
    Some(EditSet::replace_expression(
        parent.site.clone(),
        compare_to_bool(compare, op),
    ))
}

/// `int t = strcmp(a, b); ...; if (t == 0) ...` - the call result is stashed in a
/// temp first; only collapses when *every* read of that temp is itself a
/// zero-comparison, matching the legacy pass's all-or-nothing guard.
fn temp_zero_comparison_edit(
    case: &mut ItemCaseContext<'_, '_>,
    call: &CallRecord,
    compare: &Compare,
) -> Option<EditSet> {
    let target = edit_target(call);
    let statement_ref = case
        .fact(|query| {
            query.enclosing_statement(&ExpressionRef {
                site: target.clone(),
            })
        })
        .ok()?;
    if !is_direct_statement_root(&statement_ref.path, &target.path) {
        return None;
    }
    let stmt = case.fact(|query| query.statement(&statement_ref)).ok()?;
    let Stmt::Let {
        name,
        init: Some(_),
        ..
    } = &stmt.stmt
    else {
        return None;
    };
    let name = name.clone();
    let value_site = ValueSite {
        item_index: statement_ref.item_index,
        path: statement_ref.path.clone(),
    };
    // `value_uses` resolves to the *containing statement* of each read, not the
    // exact expression position, so each use is re-located to its precise
    // `Expr::Var` site (via `all_exprs`) before checking its immediate parent.
    let uses = case
        .fact(|query| query.value_uses(&value_site, &name))
        .ok()?;
    if uses.is_empty() {
        return None;
    }
    let all_exprs = case
        .fact(|query| query.all_exprs(value_site.item_index))
        .ok()?;
    let mut replacements = Vec::new();
    for use_stmt in &uses {
        let var_sites = all_exprs
            .iter()
            .filter(|site| site.path.0.starts_with(&use_stmt.path.0))
            .filter(|site| matches!(case.expr(site), Some(Expr::Var(v)) if v.as_str() == name))
            .cloned()
            .collect::<Vec<_>>();
        if var_sites.is_empty() {
            return None;
        }
        for var_site in var_sites {
            let parent = case
                .fact(|query| query.parent_expression(&ExpressionRef { site: var_site }))
                .ok()?;
            let op = case
                .expr(&parent.site)
                .and_then(|expr| zero_comparison_op_for_var(expr, &name))?;
            replacements.push((parent.site.clone(), op));
        }
    }
    let mut edits = EditSet::new();
    edits.push_replace_statement(statement_ref.item_index, statement_ref.path.clone(), None);
    for (site, op) in replacements {
        edits.push_replace_expression(site, compare_to_bool(compare, op));
    }
    Some(edits)
}

fn zero_comparison_op(expr: &Expr) -> Option<BinOp> {
    let Expr::Binary { op, lhs, rhs } = expr else {
        return None;
    };
    if !op.is_comparison() {
        return None;
    }
    if is_zero(rhs) {
        return Some(*op);
    }
    if is_zero(lhs) {
        return Some(flip_comparison(*op));
    }
    None
}

fn zero_comparison_op_for_var(expr: &Expr, name: &str) -> Option<BinOp> {
    let Expr::Binary { op, lhs, rhs } = expr else {
        return None;
    };
    if !op.is_comparison() {
        return None;
    }
    match (&**lhs, &**rhs) {
        (Expr::Var(v), rhs) if v.as_str() == name && is_zero(rhs) => Some(*op),
        (lhs, Expr::Var(v)) if v.as_str() == name && is_zero(lhs) => Some(flip_comparison(*op)),
        _ => None,
    }
}

fn strchr_case(
    case: &mut ItemCaseContext<'_, '_>,
    call: &CallRecord,
) -> Result<EditSet, Rejection> {
    case.require(matches!(call.target, CallTarget::Known(Known::StrChr)))?;
    case.require(call.args.len() == 2)?;
    let usage = case.fact(|query| query.string_libc_use(&call.site))?;
    case.require(usage.callee == StringLibcFunction::StrChr)?;
    case.require(usage.pointer_args.len() == 1)?;
    let source = source_for_binding(case, &usage.pointer_args[0])?;
    let needle_site = call.args[1].clone();
    let needle = case
        .expr(&needle_site)
        .cloned()
        .ok_or_else(|| case.reject())?;
    let zero = is_zero_arg(case, &needle_site);
    let ascii = ascii_byte_arg(case, &needle_site);
    let replacement = pointer_search(
        source.clone(),
        char_search_index(source, needle, false, zero, ascii),
    );
    Ok(EditSet::replace_expression(edit_target(call), replacement))
}

fn strrchr_case(
    case: &mut ItemCaseContext<'_, '_>,
    call: &CallRecord,
) -> Result<EditSet, Rejection> {
    case.require(matches!(call.target, CallTarget::Known(Known::StrRChr)))?;
    case.require(call.args.len() == 2)?;
    let usage = case.fact(|query| query.string_libc_use(&call.site))?;
    case.require(usage.callee == StringLibcFunction::StrRChr)?;
    case.require(usage.pointer_args.len() == 1)?;
    let source = source_for_binding(case, &usage.pointer_args[0])?;
    let needle_site = call.args[1].clone();
    let needle = case
        .expr(&needle_site)
        .cloned()
        .ok_or_else(|| case.reject())?;
    let zero = is_zero_arg(case, &needle_site);
    let ascii = ascii_byte_arg(case, &needle_site);
    let replacement = pointer_search(
        source.clone(),
        char_search_index(source, needle, true, zero, ascii),
    );
    Ok(EditSet::replace_expression(edit_target(call), replacement))
}

fn strstr_case(
    case: &mut ItemCaseContext<'_, '_>,
    call: &CallRecord,
) -> Result<EditSet, Rejection> {
    case.require(matches!(call.target, CallTarget::Known(Known::StrStr)))?;
    case.require(call.args.len() == 2)?;
    let usage = case.fact(|query| query.string_libc_use(&call.site))?;
    case.require(usage.callee == StringLibcFunction::StrStr)?;
    case.require(usage.pointer_args.len() == 2)?;
    let source = source_for_binding(case, &usage.pointer_args[0])?;
    let needle = source_for_binding(case, &usage.pointer_args[1])?;
    let index = substring_search_index(source.clone(), needle).ok_or_else(|| case.reject())?;
    Ok(EditSet::replace_expression(
        edit_target(call),
        pointer_search(source, index),
    ))
}

fn strpbrk_case(
    case: &mut ItemCaseContext<'_, '_>,
    call: &CallRecord,
) -> Result<EditSet, Rejection> {
    case.require(matches!(call.target, CallTarget::Known(Known::StrPBrk)))?;
    case.require(call.args.len() == 2)?;
    let usage = case.fact(|query| query.string_libc_use(&call.site))?;
    case.require(usage.callee == StringLibcFunction::StrPBrk)?;
    case.require(usage.pointer_args.len() == 2)?;
    let source = source_for_binding(case, &usage.pointer_args[0])?;
    let set = source_for_binding(case, &usage.pointer_args[1])?;
    let index = set_position(source.clone(), set, false).ok_or_else(|| case.reject())?;
    Ok(EditSet::replace_expression(
        edit_target(call),
        pointer_search(source, index),
    ))
}

fn strspn_case(
    case: &mut ItemCaseContext<'_, '_>,
    call: &CallRecord,
) -> Result<EditSet, Rejection> {
    case.require(matches!(call.target, CallTarget::Known(Known::StrSpn)))?;
    case.require(call.args.len() == 2)?;
    let usage = case.fact(|query| query.string_libc_use(&call.site))?;
    case.require(usage.callee == StringLibcFunction::StrSpn)?;
    case.require(usage.pointer_args.len() == 2)?;
    let source = source_for_binding(case, &usage.pointer_args[0])?;
    let set = source_for_binding(case, &usage.pointer_args[1])?;
    let replacement = span_index(source, set, true).ok_or_else(|| case.reject())?;
    usize_call_edit(case, call, replacement)
}

fn strcspn_case(
    case: &mut ItemCaseContext<'_, '_>,
    call: &CallRecord,
) -> Result<EditSet, Rejection> {
    case.require(matches!(call.target, CallTarget::Known(Known::StrCSpn)))?;
    case.require(call.args.len() == 2)?;
    let usage = case.fact(|query| query.string_libc_use(&call.site))?;
    case.require(usage.callee == StringLibcFunction::StrCSpn)?;
    case.require(usage.pointer_args.len() == 2)?;
    let source = source_for_binding(case, &usage.pointer_args[0])?;
    let set = source_for_binding(case, &usage.pointer_args[1])?;
    let replacement = span_index(source, set, false).ok_or_else(|| case.reject())?;
    usize_call_edit(case, call, replacement)
}

fn atoi_case(case: &mut ItemCaseContext<'_, '_>, call: &CallRecord) -> Result<EditSet, Rejection> {
    case.require(matches!(call.target, CallTarget::Known(Known::Atoi)))?;
    case.require(call.args.len() == 1)?;
    let usage = case.fact(|query| query.string_libc_use(&call.site))?;
    case.require(usage.callee == StringLibcFunction::Atoi)?;
    case.require(usage.pointer_args.len() == 1)?;
    numeric_parse_edit(
        case,
        call,
        &usage.pointer_args[0],
        "parse_i32",
        ParseTarget::I32,
    )
}

fn atol_case(case: &mut ItemCaseContext<'_, '_>, call: &CallRecord) -> Result<EditSet, Rejection> {
    case.require(matches!(call.target, CallTarget::Known(Known::Atol)))?;
    case.require(call.args.len() == 1)?;
    let usage = case.fact(|query| query.string_libc_use(&call.site))?;
    case.require(usage.callee == StringLibcFunction::Atol)?;
    case.require(usage.pointer_args.len() == 1)?;
    numeric_parse_edit(
        case,
        call,
        &usage.pointer_args[0],
        "parse_i64",
        ParseTarget::I64,
    )
}

fn strtol_case(
    case: &mut ItemCaseContext<'_, '_>,
    call: &CallRecord,
) -> Result<EditSet, Rejection> {
    case.require(matches!(call.target, CallTarget::Known(Known::StrTol)))?;
    case.require(call.args.len() == 3)?;
    let usage = case.fact(|query| query.string_libc_use(&call.site))?;
    case.require(usage.callee == StringLibcFunction::StrTol)?;
    case.require(usage.pointer_args.len() == 1)?;
    let arg1_null = is_null_arg(case, &call.args[1]);
    case.require(arg1_null)?;
    let arg2_ten = is_ten_arg(case, &call.args[2]);
    case.require(arg2_ten)?;
    numeric_parse_edit(
        case,
        call,
        &usage.pointer_args[0],
        "parse_i64",
        ParseTarget::I64,
    )
}

fn strtoul_case(
    case: &mut ItemCaseContext<'_, '_>,
    call: &CallRecord,
) -> Result<EditSet, Rejection> {
    case.require(matches!(call.target, CallTarget::Known(Known::StrToul)))?;
    case.require(call.args.len() == 3)?;
    let usage = case.fact(|query| query.string_libc_use(&call.site))?;
    case.require(usage.callee == StringLibcFunction::StrToul)?;
    case.require(usage.pointer_args.len() == 1)?;
    let arg1_null = is_null_arg(case, &call.args[1]);
    case.require(arg1_null)?;
    let arg2_ten = is_ten_arg(case, &call.args[2]);
    case.require(arg2_ten)?;
    numeric_parse_edit(
        case,
        call,
        &usage.pointer_args[0],
        "parse_u64",
        ParseTarget::U64,
    )
}

fn strtod_case(
    case: &mut ItemCaseContext<'_, '_>,
    call: &CallRecord,
) -> Result<EditSet, Rejection> {
    case.require(matches!(call.target, CallTarget::Known(Known::StrTod)))?;
    case.require(call.args.len() == 2)?;
    let usage = case.fact(|query| query.string_libc_use(&call.site))?;
    case.require(usage.callee == StringLibcFunction::StrTod)?;
    case.require(usage.pointer_args.len() == 1)?;
    let arg1_null = is_null_arg(case, &call.args[1]);
    case.require(arg1_null)?;
    numeric_parse_edit(
        case,
        call,
        &usage.pointer_args[0],
        "parse_f64",
        ParseTarget::F64,
    )
}

fn numeric_parse_edit(
    case: &mut ItemCaseContext<'_, '_>,
    call: &CallRecord,
    binding: &BindingRef,
    helper: &str,
    target: ParseTarget,
) -> Result<EditSet, Rejection> {
    let source = source_for_binding(case, binding)?;
    case.require(source.kind == StringKind::Str)?;
    if let Some(integer_target) = target.integer()
        && let Ok(replacement) = direct_numeric_parse(case, binding, &source.name, integer_target)
    {
        return Ok(EditSet::replace_expression(edit_target(call), replacement));
    }
    let replacement = Expr::Call {
        binding: crate::function_identity::CallBinding::Generated,
        func: Box::new(runtime::numeric_parse_path(helper)),
        args: vec![Expr::Var(source.name.into())],
    };
    Ok(EditSet::replace_expression(edit_target(call), replacement))
}

#[derive(Clone, Copy)]
enum ParseTarget {
    I32,
    I64,
    U64,
    F64,
}

impl ParseTarget {
    fn integer(self) -> Option<IntegerParseTarget> {
        match self {
            Self::I32 => Some(IntegerParseTarget::I32),
            Self::I64 => Some(IntegerParseTarget::I64),
            Self::U64 => Some(IntegerParseTarget::U64),
            Self::F64 => None,
        }
    }
}

#[derive(Clone, Copy)]
enum IntegerParseTarget {
    I32,
    I64,
    U64,
}

fn direct_numeric_parse(
    case: &mut ItemCaseContext<'_, '_>,
    binding: &BindingRef,
    source: &str,
    target: IntegerParseTarget,
) -> Result<Expr, Rejection> {
    let sign = case.fact(|query| query.ascii_numeric_sign(binding))?;
    let buffer = case.fact(|query| query.string_buffer(&binding.value_site()))?;
    let bytes = buffer.bytes.clone().ok_or_else(|| case.reject())?;
    let text = std::str::from_utf8(&bytes).map_err(|_| case.reject())?;
    case.require(target_accepts(text, sign, target))?;
    Ok(Expr::MethodCall {
        recv: Box::new(Expr::MethodCallGeneric {
            recv: Box::new(Expr::Var(source.into())),
            method: "parse".into(),
            type_args: vec![parse_target_type(target)],
            args: Vec::new(),
        }),
        method: "unwrap_or".into(),
        args: vec![Expr::Value(RustValue::I64(0))],
    })
}

fn target_accepts(text: &str, sign: AsciiNumericSign, target: IntegerParseTarget) -> bool {
    match target {
        IntegerParseTarget::I32 => text.parse::<i32>().is_ok(),
        IntegerParseTarget::I64 => text.parse::<i64>().is_ok(),
        IntegerParseTarget::U64 if sign == AsciiNumericSign::Minus => false,
        IntegerParseTarget::U64 => text.parse::<u64>().is_ok(),
    }
}

fn parse_target_type(target: IntegerParseTarget) -> Type {
    match target {
        IntegerParseTarget::I32 => Type::Prim(Prim::I32),
        IntegerParseTarget::I64 => Type::Prim(Prim::I64),
        IntegerParseTarget::U64 => Type::Prim(Prim::U64),
    }
}

fn strlen_replacement(source: Source) -> Expr {
    Expr::MethodCall {
        recv: Box::new(Expr::Var(source.name.into())),
        method: "len".into(),
        args: Vec::new(),
    }
}

fn comparable(lhs: Source, rhs: Source) -> Option<(Expr, Expr)> {
    if lhs.kind != rhs.kind {
        return None;
    }
    Some((Expr::Var(lhs.name.into()), Expr::Var(rhs.name.into())))
}

fn pointer_search(source: Source, index: Expr) -> Expr {
    let name = source.name.clone();
    Expr::MethodCall {
        recv: Box::new(index),
        method: "map_or".into(),
        args: vec![null_mut(), index_to_ptr(&name)],
    }
}

fn char_search_index(
    source: Source,
    needle: Expr,
    reverse: bool,
    zero: bool,
    ascii_needle: bool,
) -> Expr {
    if zero {
        return some(strlen_replacement(source));
    }
    if source.kind == StringKind::Str && source.ascii_only && ascii_needle {
        return Expr::MethodCall {
            recv: Box::new(Expr::Var(source.name.into())),
            method: if reverse { "rfind" } else { "find" }.into(),
            args: vec![char_expr(needle)],
        };
    }
    byte_position(byte_source_expr(source), byte_expr(needle), reverse)
}

fn byte_position(source: Expr, needle: Expr, reverse: bool) -> Expr {
    Expr::MethodCall {
        recv: Box::new(Expr::MethodCall {
            recv: Box::new(source),
            method: "iter".into(),
            args: Vec::new(),
        }),
        method: if reverse { "rposition" } else { "position" }.into(),
        args: vec![Expr::Closure {
            params: vec![Ident::new("__slate_byte")],
            body: Box::new(Expr::Binary {
                op: BinOp::Eq,
                lhs: Box::new(Expr::Unary {
                    op: UnaryOp::Deref,
                    expr: Box::new(Expr::Var("__slate_byte".into())),
                }),
                rhs: Box::new(byte_expr(needle)),
            }),
        }],
    }
}

fn substring_search_index(source: Source, needle: Source) -> Option<Expr> {
    if source.kind != needle.kind {
        return None;
    }
    if source.kind == StringKind::Str && source.ascii_only && needle.ascii_only {
        return Some(Expr::MethodCall {
            recv: Box::new(Expr::Var(source.name.into())),
            method: "find".into(),
            args: vec![Expr::Var(needle.name.into())],
        });
    }
    Some(Expr::If {
        cond: Box::new(Expr::MethodCall {
            recv: Box::new(byte_source_expr(needle.clone())),
            method: "is_empty".into(),
            args: Vec::new(),
        }),
        then_expr: Box::new(some(Expr::Value(RustValue::I64(0)))),
        else_expr: Box::new(Expr::MethodCall {
            recv: Box::new(Expr::MethodCall {
                recv: Box::new(byte_source_expr(source)),
                method: "windows".into(),
                args: vec![byte_len(needle.clone())],
            }),
            method: "position".into(),
            args: vec![Expr::Closure {
                params: vec![Ident::new("__slate_window")],
                body: Box::new(Expr::Binary {
                    op: BinOp::Eq,
                    lhs: Box::new(Expr::Var("__slate_window".into())),
                    rhs: Box::new(byte_source_expr(needle)),
                }),
            }],
        }),
    })
}

fn span_index(source: Source, set: Source, span_members: bool) -> Option<Expr> {
    Some(Expr::MethodCall {
        recv: Box::new(set_position(source.clone(), set, span_members)?),
        method: "unwrap_or".into(),
        args: vec![strlen_replacement(source)],
    })
}

fn set_position(source: Source, set: Source, invert: bool) -> Option<Expr> {
    if source.kind != set.kind {
        return None;
    }
    let use_str = source.kind == StringKind::Str && source.ascii_only && set.ascii_only;
    let contains = if use_str {
        Expr::MethodCall {
            recv: Box::new(Expr::Var(set.name.into())),
            method: "contains".into(),
            args: vec![Expr::Var("__slate_ch".into())],
        }
    } else {
        Expr::MethodCall {
            recv: Box::new(byte_source_expr(set)),
            method: "contains".into(),
            args: vec![Expr::Var("__slate_byte".into())],
        }
    };
    let body = if invert {
        Expr::Unary {
            op: UnaryOp::Not,
            expr: Box::new(contains),
        }
    } else {
        contains
    };
    let (recv, method, param) = if use_str {
        (Expr::Var(source.name.into()), "find", "__slate_ch")
    } else {
        (
            Expr::MethodCall {
                recv: Box::new(byte_source_expr(source)),
                method: "iter".into(),
                args: Vec::new(),
            },
            "position",
            "__slate_byte",
        )
    };
    Some(Expr::MethodCall {
        recv: Box::new(recv),
        method: method.into(),
        args: vec![Expr::Closure {
            params: vec![Ident::new(param)],
            body: Box::new(body),
        }],
    })
}

fn index_to_ptr(name: &str) -> Expr {
    Expr::Closure {
        params: vec![Ident::new("__slate_index")],
        body: Box::new(Expr::Unsafe(Box::new(Block {
            stmts: Vec::new(),
            tail: Some(Box::new(Expr::Cast {
                expr: Box::new(Expr::MethodCall {
                    recv: Box::new(Expr::MethodCall {
                        recv: Box::new(Expr::Var(name.into())),
                        method: "as_ptr".into(),
                        args: Vec::new(),
                    }),
                    method: "add".into(),
                    args: vec![Expr::Var("__slate_index".into())],
                }),
                ty: Type::Ptr {
                    mutable: true,
                    inner: Box::new(Type::Prim(Prim::I8)),
                },
            })),
        }))),
    }
}

fn some(expr: Expr) -> Expr {
    Expr::Call {
        binding: crate::function_identity::CallBinding::Generated,
        func: Box::new(Expr::Var("Some".into())),
        args: vec![expr],
    }
}

fn null_mut() -> Expr {
    Expr::Call {
        binding: crate::function_identity::CallBinding::Generated,
        func: Box::new(path_expr(["std", "ptr", "null_mut"])),
        args: Vec::new(),
    }
}

fn char_expr(expr: Expr) -> Expr {
    Expr::Call {
        binding: crate::function_identity::CallBinding::Generated,
        func: Box::new(path_expr(["char", "from"])),
        args: vec![byte_expr(expr)],
    }
}

fn byte_expr(expr: Expr) -> Expr {
    Expr::Cast {
        expr: Box::new(expr),
        ty: Type::Prim(Prim::U8),
    }
}

fn byte_source_expr(source: Source) -> Expr {
    match source.kind {
        StringKind::Str => Expr::MethodCall {
            recv: Box::new(Expr::Var(source.name.into())),
            method: "as_bytes".into(),
            args: Vec::new(),
        },
        StringKind::Bytes => Expr::Var(source.name.into()),
    }
}

fn byte_len(source: Source) -> Expr {
    Expr::MethodCall {
        recv: Box::new(byte_source_expr(source)),
        method: "len".into(),
        args: Vec::new(),
    }
}

fn prefix(source: Source, count: Expr) -> Expr {
    let base = match source.kind {
        StringKind::Str => Expr::MethodCall {
            recv: Box::new(Expr::Var(source.name.clone().into())),
            method: "as_bytes".into(),
            args: Vec::new(),
        },
        StringKind::Bytes => Expr::Var(source.name.clone().into()),
    };
    let len = Expr::MethodCall {
        recv: Box::new(Expr::Var(source.name.into())),
        method: "len".into(),
        args: Vec::new(),
    };
    let n = Expr::Call {
        binding: crate::function_identity::CallBinding::Generated,
        func: Box::new(path_expr(["std", "cmp", "min"])),
        args: vec![
            Expr::Cast {
                expr: Box::new(count),
                ty: Type::Prim(Prim::Usize),
            },
            len,
        ],
    };
    Expr::TupleField {
        base: Box::new(Expr::MethodCall {
            recv: Box::new(base),
            method: "split_at".into(),
            args: vec![n],
        }),
        index: 0,
    }
}

fn compare_expr(compare: Compare) -> Expr {
    Expr::MethodCall {
        recv: Box::new(compare.lhs),
        method: "cmp".into(),
        args: vec![compare.rhs],
    }
}

fn compare_to_bool(compare: &Compare, op: BinOp) -> Expr {
    match op {
        BinOp::Eq => Expr::Binary {
            op: BinOp::Eq,
            lhs: Box::new(compare.lhs.clone()),
            rhs: Box::new(compare.rhs.clone()),
        },
        BinOp::Ne => Expr::Binary {
            op: BinOp::Ne,
            lhs: Box::new(compare.lhs.clone()),
            rhs: Box::new(compare.rhs.clone()),
        },
        BinOp::Lt => ordering_compare(compare.clone(), BinOp::Eq, "Less"),
        BinOp::Le => ordering_compare(compare.clone(), BinOp::Ne, "Greater"),
        BinOp::Gt => ordering_compare(compare.clone(), BinOp::Eq, "Greater"),
        BinOp::Ge => ordering_compare(compare.clone(), BinOp::Ne, "Less"),
        _ => unreachable!(),
    }
}

fn ordering_compare(compare: Compare, op: BinOp, variant: &str) -> Expr {
    Expr::Binary {
        op,
        lhs: Box::new(compare_expr(compare)),
        rhs: Box::new(ordering_variant(variant)),
    }
}

fn cmp_to_i32(cmp: Expr) -> Expr {
    Expr::If {
        cond: Box::new(Expr::Binary {
            op: BinOp::Eq,
            lhs: Box::new(cmp.clone()),
            rhs: Box::new(ordering_variant("Less")),
        }),
        then_expr: Box::new(Expr::Value(RustValue::I64(-1))),
        else_expr: Box::new(Expr::If {
            cond: Box::new(Expr::Binary {
                op: BinOp::Eq,
                lhs: Box::new(cmp),
                rhs: Box::new(ordering_variant("Equal")),
            }),
            then_expr: Box::new(Expr::Value(RustValue::I64(0))),
            else_expr: Box::new(Expr::Value(RustValue::I64(1))),
        }),
    }
}

/// Casts (e.g. a pointer temp coerced with `as *mut *mut i8`) sit one `Expr(0)`
/// segment below their own site; peeling them finds the site actually worth
/// resolving to a binding for a per-binding constant check.
fn peel_cast_site(case: &ItemCaseContext<'_, '_>, site: &ExprSite) -> ExprSite {
    let mut site = site.clone();
    while matches!(case.expr(&site), Some(Expr::Cast { .. })) {
        site.path.0.push(PathSegment::Expr(0));
        site.fact_path.0.push(PathSegment::Expr(0));
    }
    site
}

fn is_zero_arg(case: &mut ItemCaseContext<'_, '_>, site: &ExprSite) -> bool {
    if case.expr(site).is_some_and(is_zero) {
        return true;
    }
    if case
        .fact(|query| query.const_usize(site))
        .is_ok_and(|value| value == 0)
    {
        return true;
    }
    // A temp-extracted `0`/`NULL` argument (e.g. `let _v = std::ptr::null_mut(); f(_v)`)
    // isn't a literal at this site, so fall back to the binding's own tracked value.
    let inner = peel_cast_site(case, site);
    if !matches!(case.expr(&inner), Some(Expr::Var(_))) {
        return false;
    }
    let Ok(binding) = case.fact(|query| {
        query.expression_binding(&ExpressionRef {
            site: inner.clone(),
        })
    }) else {
        return false;
    };
    case.fact(|query| query.binding_constant_zero(&binding))
        .is_ok()
}

fn is_null_arg(case: &mut ItemCaseContext<'_, '_>, site: &ExprSite) -> bool {
    if is_zero_arg(case, site) {
        return true;
    }
    let inner = peel_cast_site(case, site);
    case.expr(&inner).is_some_and(|expr| {
        matches!(
            expr,
            Expr::Call { func, args, .. }
                if args.is_empty() && matches_null_path(func)
        )
    })
}

fn is_ten_arg(case: &mut ItemCaseContext<'_, '_>, site: &ExprSite) -> bool {
    if case
        .expr(site)
        .is_some_and(|expr| matches!(expr, Expr::Value(RustValue::I64(10) | RustValue::I128(10))))
    {
        return true;
    }
    case.fact(|query| query.const_usize(site))
        .is_ok_and(|value| value == 10)
}

fn ascii_byte_arg(case: &mut ItemCaseContext<'_, '_>, site: &ExprSite) -> bool {
    byte_value_arg(case, site).is_some_and(|value| value < 128)
}

fn byte_value_arg(case: &mut ItemCaseContext<'_, '_>, site: &ExprSite) -> Option<u8> {
    if let Some(value) = case.expr(site).and_then(literal_int) {
        return u8::try_from(value).ok();
    }
    case.fact(|query| query.const_u8(site)).ok()
}

fn literal_int(expr: &Expr) -> Option<i128> {
    match expr {
        Expr::Value(RustValue::I64(n)) => Some(i128::from(*n)),
        Expr::Value(RustValue::I128(n)) => Some(*n),
        Expr::Cast { expr, .. } => literal_int(expr),
        _ => None,
    }
}

fn matches_null_path(expr: &Expr) -> bool {
    let Expr::Path(path) = expr else {
        return false;
    };
    let segments = path.segments.iter().map(Ident::as_str).collect::<Vec<_>>();
    matches!(
        segments.as_slice(),
        ["std", "ptr", "null" | "null_mut"] | ["core", "ptr", "null" | "null_mut"]
    )
}

fn is_zero(expr: &Expr) -> bool {
    match expr {
        Expr::Value(RustValue::I64(0) | RustValue::I128(0)) => true,
        Expr::Cast { expr, .. } => is_zero(expr),
        _ => false,
    }
}

fn flip_comparison(op: BinOp) -> BinOp {
    match op {
        BinOp::Lt => BinOp::Gt,
        BinOp::Le => BinOp::Ge,
        BinOp::Gt => BinOp::Lt,
        BinOp::Ge => BinOp::Le,
        op => op,
    }
}

fn ordering_variant(variant: &str) -> Expr {
    path_expr(["std", "cmp", "Ordering", variant])
}

fn path_expr<const N: usize>(segments: [&str; N]) -> Expr {
    Expr::Path(Path::new(segments.into_iter().map(Ident::new)))
}
