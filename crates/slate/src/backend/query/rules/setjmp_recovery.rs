use std::collections::{BTreeMap, BTreeSet};

use crate::backend::rust_ast::{
    Abi, BinOp, Block, Expr, ExprMatchArm, FnDef, Ident, IndentStmt, Item, Label, MatchArm, Path,
    Pattern, Prim, Program, RustValue, Stmt, StructDef, StructFields, Type, Visibility,
};
use crate::backend::salsa::SalsaFacts;
use crate::backend::support::walk;
use crate::backend::trace::Pass;
use crate::function_identity::CallBinding;

use super::super::{
    CallTarget, EditSet, Predicate, Proof, QueryContext, QueryResult, QueryRule, Rejection,
    RejectionReason, WholeProgram,
};

pub(in crate::backend) fn program() -> QueryRule<WholeProgram> {
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
    let mut origin_functions = BTreeSet::new();
    for item in replacement.items.iter_mut() {
        if let Item::Fn(f) = item
            && rewrite_longjmp_calls_in_body(&mut f.body, &buffers)
        {
            origin_functions.insert(f.name.clone());
        }
    }
    for buffer in &buffers {
        replacement.items.push(payload_struct_item(buffer));
    }

    flip_unwind_abi_single_program(&mut replacement, query, &origin_functions);

    Ok(Proof::new(
        SetjmpRecoveryRewrite { replacement },
        Vec::new(),
    ))
}

fn flip_unwind_abi_single_program(
    replacement: &mut Program,
    query: &QueryContext<'_>,
    origin_functions: &BTreeSet<String>,
) {
    if origin_functions.is_empty() {
        return;
    }
    let mut seeds = origin_functions.clone();
    let mut previous: BTreeSet<String> = BTreeSet::new();
    for _ in 0..8 {
        if seeds == previous {
            break;
        }
        previous = seeds.clone();
        flip_unwind_abi_fn_defs(replacement, query, &seeds);
        let single = std::slice::from_ref(&*replacement);
        let address_exposed = address_exposed_function_names(single);
        let flipped_names = cunwind_names(single);
        let flipped_shapes = cunwind_shapes(single);
        let siblings = address_exposed_c_abi_functions_matching_shapes(
            single,
            &flipped_shapes,
            &address_exposed,
            &flipped_names,
        );
        let indirect_callers = functions_with_cunwind_indirect_calls(single, &flipped_shapes);
        seeds = flipped_names
            .into_iter()
            .chain(siblings)
            .chain(indirect_callers)
            .collect();
    }
    let flipped_shapes = cunwind_shapes(std::slice::from_ref(&*replacement));
    rewrite_c_abi_fn_ptr_types_matching_shapes(replacement, &flipped_shapes);
}

pub(in crate::backend) fn propagate_unwind_abi_across_project(programs: &mut [Program]) {
    let mut previous: BTreeSet<String> = BTreeSet::new();
    for _ in 0..8 {
        let flipped_names = cunwind_names(programs);
        let flipped_shapes = cunwind_shapes(programs);
        let address_exposed = address_exposed_function_names(programs);
        let siblings = address_exposed_c_abi_functions_matching_shapes(
            programs,
            &flipped_shapes,
            &address_exposed,
            &flipped_names,
        );
        let indirect_callers = functions_with_cunwind_indirect_calls(programs, &flipped_shapes);
        let seeds: BTreeSet<String> = flipped_names
            .into_iter()
            .chain(siblings)
            .chain(indirect_callers)
            .collect();
        if seeds == previous {
            break;
        }
        previous = seeds.clone();
        for program in programs.iter_mut() {
            propagate_unwind_abi_with_seeds(program, &seeds);
        }
    }

    let flipped_shapes = cunwind_shapes(programs);
    for program in programs.iter_mut() {
        rewrite_c_abi_fn_ptr_types_matching_shapes(program, &flipped_shapes);
    }
}

fn cunwind_names(programs: &[Program]) -> BTreeSet<String> {
    programs
        .iter()
        .flat_map(|program| program.items.iter())
        .filter_map(|item| match item {
            Item::Fn(f) if f.abi == Some(Abi::CUnwind) => Some(f.name.clone()),
            _ => None,
        })
        .collect()
}

fn cunwind_shapes(programs: &[Program]) -> BTreeSet<(Vec<Type>, Type)> {
    programs
        .iter()
        .flat_map(|program| program.items.iter())
        .filter_map(|item| match item {
            Item::Fn(f) if f.abi == Some(Abi::CUnwind) => Some(fn_shape(f)),
            _ => None,
        })
        .collect()
}

fn address_exposed_c_abi_functions_matching_shapes(
    programs: &[Program],
    shapes: &BTreeSet<(Vec<Type>, Type)>,
    address_exposed: &BTreeSet<String>,
    already: &BTreeSet<String>,
) -> BTreeSet<String> {
    if shapes.is_empty() {
        return BTreeSet::new();
    }
    programs
        .iter()
        .flat_map(|program| program.items.iter())
        .filter_map(|item| match item {
            Item::Fn(f)
                if f.abi == Some(Abi::C)
                    && !already.contains(&f.name)
                    && address_exposed.contains(&f.name)
                    && shapes.contains(&fn_shape(f)) =>
            {
                Some(f.name.clone())
            }
            _ => None,
        })
        .collect()
}

fn functions_with_cunwind_indirect_calls(
    programs: &[Program],
    shapes: &BTreeSet<(Vec<Type>, Type)>,
) -> BTreeSet<String> {
    if shapes.is_empty() {
        return BTreeSet::new();
    }
    let mut field_shapes: BTreeMap<String, (Vec<Type>, Type)> = BTreeMap::new();
    let mut global_shapes: BTreeMap<String, (Vec<Type>, Type)> = BTreeMap::new();
    for program in programs {
        for item in &program.items {
            match item {
                Item::Struct(s) => {
                    if let StructFields::Named(fields) = &s.fields {
                        for field in fields {
                            if let Some(shape) = fn_ptr_shape_in_type(&field.ty) {
                                field_shapes.insert(field.name.clone(), shape);
                            }
                        }
                    }
                }
                Item::Record(r) => {
                    for field in &r.fields {
                        if let Some(shape) = fn_ptr_shape_in_type(&field.ty) {
                            field_shapes.insert(field.name.as_str().to_string(), shape);
                        }
                    }
                }
                Item::Static { name, ty, .. } | Item::Const { name, ty, .. } => {
                    if let Some(shape) = fn_ptr_shape_in_type(ty) {
                        global_shapes.insert(name.clone(), shape);
                    }
                }
                _ => {}
            }
        }
    }

    let mut result = BTreeSet::new();
    for program in programs {
        for item in &program.items {
            let Item::Fn(f) = item else { continue };
            let mut local_shapes = BTreeMap::new();
            for param in &f.params {
                if let Some(shape) = fn_ptr_shape_in_type(&param.ty) {
                    local_shapes.insert(param.name.clone(), shape);
                }
            }
            collect_local_fn_ptr_shapes(&f.body, &mut local_shapes);
            let found = walk::body_expr_any(&f.body, &mut |expr| {
                let Expr::Call {
                    func,
                    binding: CallBinding::Indirect,
                    ..
                } = expr
                else {
                    return false;
                };
                resolve_indirect_callee_shape(func, &local_shapes, &global_shapes, &field_shapes)
                    .is_some_and(|shape| shapes.contains(&shape))
            });
            if found {
                result.insert(f.name.clone());
            }
        }
    }
    result
}

fn collect_local_fn_ptr_shapes(
    body: &[IndentStmt],
    shapes: &mut BTreeMap<String, (Vec<Type>, Type)>,
) {
    for indent in body {
        let named_ty = match &indent.stmt {
            Stmt::Let {
                name, ty: Some(ty), ..
            }
            | Stmt::LetIf {
                name, ty: Some(ty), ..
            } => Some((name.clone(), ty)),
            _ => None,
        };
        if let Some((name, ty)) = named_ty
            && let Some(shape) = fn_ptr_shape_in_type(ty)
        {
            shapes.insert(name, shape);
        }
        walk::nested_bodies_with_path(&indent.stmt, &mut Vec::new(), &mut |nested, _| {
            collect_local_fn_ptr_shapes(nested, shapes);
        });
    }
}

fn resolve_indirect_callee_shape(
    func: &Expr,
    local_shapes: &BTreeMap<String, (Vec<Type>, Type)>,
    global_shapes: &BTreeMap<String, (Vec<Type>, Type)>,
    field_shapes: &BTreeMap<String, (Vec<Type>, Type)>,
) -> Option<(Vec<Type>, Type)> {
    match func {
        Expr::Var(name) => local_shapes
            .get(name.as_str())
            .or_else(|| global_shapes.get(name.as_str()))
            .cloned(),
        Expr::Field { field, .. } => field_shapes.get(field.as_str()).cloned(),
        Expr::MethodCall { recv, .. } | Expr::MethodCallGeneric { recv, .. } => {
            resolve_indirect_callee_shape(recv, local_shapes, global_shapes, field_shapes)
        }
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. } => {
            resolve_indirect_callee_shape(expr, local_shapes, global_shapes, field_shapes)
        }
        Expr::Unsafe(block) | Expr::Block(block) => {
            let tail = block.tail.as_deref()?;
            resolve_indirect_callee_shape(tail, local_shapes, global_shapes, field_shapes)
        }
        _ => None,
    }
}

fn fn_ptr_shape_in_type(ty: &Type) -> Option<(Vec<Type>, Type)> {
    match ty {
        Type::FnPtr { params, ret, .. } => Some((params.clone(), (**ret).clone())),
        Type::Generic { args, .. } => args.iter().find_map(fn_ptr_shape_in_type),
        Type::Ref { inner, .. } | Type::Ptr { inner, .. } | Type::Complex(inner) => {
            fn_ptr_shape_in_type(inner)
        }
        _ => None,
    }
}

fn propagate_unwind_abi_with_seeds(program: &mut Program, seeds: &BTreeSet<String>) {
    if seeds.is_empty() {
        return;
    }
    let mut replacement = program.clone();
    {
        let mut facts = SalsaFacts::new_empty();
        facts.set_program(program);
        let query = QueryContext::new(program, &facts);
        flip_unwind_abi_fn_defs(&mut replacement, &query, seeds);
    }
    *program = replacement;
}

fn flip_unwind_abi_fn_defs(
    replacement: &mut Program,
    query: &QueryContext<'_>,
    seeds: &BTreeSet<String>,
) {
    if seeds.is_empty() {
        return;
    }
    let flip_targets = reachable_c_abi_functions(query, seeds);
    for item in replacement.items.iter_mut() {
        if let Item::Fn(f) = item
            && flip_targets.contains(&f.name)
            && f.abi == Some(Abi::C)
        {
            f.abi = Some(Abi::CUnwind);
        }
    }
}

fn fn_shape(f: &FnDef) -> (Vec<Type>, Type) {
    (
        f.params.iter().map(|param| param.ty.clone()).collect(),
        f.ret.clone().unwrap_or(Type::Unit),
    )
}

fn address_exposed_function_names(programs: &[Program]) -> BTreeSet<String> {
    let mut all_names: BTreeSet<String> = BTreeSet::new();
    for program in programs {
        let mut facts = SalsaFacts::new_empty();
        facts.set_program(program);
        let query = QueryContext::new(program, &facts);
        all_names.extend(
            query
                .all_functions()
                .iter()
                .map(|function| function.name.clone()),
        );
    }

    let mut exposed = BTreeSet::new();
    for program in programs {
        let mut facts = SalsaFacts::new_empty();
        facts.set_program(program);
        let query = QueryContext::new(program, &facts);
        for name in &all_names {
            if exposed.contains(name) {
                continue;
            }
            let direct_call_count = query
                .all_calls()
                .filter(|call| matches!(&call.target, CallTarget::Direct(target) if target == name))
                .count();
            if query.symbol_use_count(name) != direct_call_count {
                exposed.insert(name.clone());
            }
        }
    }
    exposed
}

fn reachable_c_abi_functions(
    query: &QueryContext<'_>,
    seeds: &BTreeSet<String>,
) -> BTreeSet<String> {
    let callers_by_item_index: BTreeMap<usize, String> = query
        .all_functions()
        .iter()
        .map(|function| (function.item_index, function.name.clone()))
        .collect();

    let mut callers_of: BTreeMap<&str, BTreeSet<&str>> = BTreeMap::new();
    for call in query.all_calls() {
        let target = match &call.target {
            CallTarget::Direct(target) | CallTarget::Generated(target) => target,
            CallTarget::Known(_) | CallTarget::Indirect => continue,
        };
        let Some(caller) = callers_by_item_index.get(&call.site.item_index) else {
            continue;
        };
        callers_of
            .entry(target.as_str())
            .or_default()
            .insert(caller.as_str());
    }

    let mut to_flip: BTreeSet<String> = seeds.clone();
    let mut visited: BTreeSet<String> = BTreeSet::new();
    let mut frontier: Vec<String> = seeds.iter().cloned().collect();
    while let Some(name) = frontier.pop() {
        if !visited.insert(name.clone()) {
            continue;
        }
        let Some(callers) = callers_of.get(name.as_str()) else {
            continue;
        };
        for caller in callers {
            if to_flip.insert(caller.to_string()) {
                frontier.push(caller.to_string());
            }
        }
    }
    to_flip
}

fn rewrite_c_abi_fn_ptr_types_matching_shapes(
    program: &mut Program,
    shapes: &BTreeSet<(Vec<Type>, Type)>,
) {
    if shapes.is_empty() {
        return;
    }
    for item in program.items.iter_mut() {
        match item {
            Item::Fn(f) => {
                for param in f.params.iter_mut() {
                    rewrite_c_abi_type_matching(&mut param.ty, shapes);
                }
                if let Some(ret) = f.ret.as_mut() {
                    rewrite_c_abi_type_matching(ret, shapes);
                }
                rewrite_c_abi_let_types_matching(&mut f.body, shapes);
                walk::body_exprs_mut_with(&mut f.body, &mut |expr| {
                    rewrite_c_abi_types_in_expr_node_matching(expr, shapes);
                    true
                });
            }
            Item::Struct(s) => rewrite_c_abi_types_in_struct_fields_matching(&mut s.fields, shapes),
            Item::Record(r) => {
                for field in r.fields.iter_mut() {
                    rewrite_c_abi_type_matching(&mut field.ty, shapes);
                }
            }
            Item::Static { ty, init, .. } | Item::Const { ty, init, .. } => {
                rewrite_c_abi_type_matching(ty, shapes);
                walk::exprs_mut_with(init, &mut |expr| {
                    rewrite_c_abi_types_in_expr_node_matching(expr, shapes);
                    true
                });
            }
            _ => {}
        }
    }
}

fn rewrite_c_abi_types_in_struct_fields_matching(
    fields: &mut StructFields,
    shapes: &BTreeSet<(Vec<Type>, Type)>,
) {
    match fields {
        StructFields::Named(named) => {
            for field in named.iter_mut() {
                rewrite_c_abi_type_matching(&mut field.ty, shapes);
            }
        }
        StructFields::Tuple(tys) => {
            for ty in tys.iter_mut() {
                rewrite_c_abi_type_matching(ty, shapes);
            }
        }
    }
}

fn rewrite_c_abi_let_types_matching(body: &mut [IndentStmt], shapes: &BTreeSet<(Vec<Type>, Type)>) {
    for indent in body.iter_mut() {
        match &mut indent.stmt {
            Stmt::Let { ty: Some(ty), .. } | Stmt::LetIf { ty: Some(ty), .. } => {
                rewrite_c_abi_type_matching(ty, shapes);
            }
            _ => {}
        }
        let mut path = Vec::new();
        walk::nested_body_vecs_mut_with_path(&mut indent.stmt, &mut path, &mut |nested, _| {
            rewrite_c_abi_let_types_matching(nested, shapes);
        });
        walk::stmt_exprs_mut_with(&mut indent.stmt, &mut |expr| {
            if let Expr::Block(block) = expr {
                rewrite_c_abi_let_types_matching(&mut block.stmts, shapes);
            }
            true
        });
    }
}

fn rewrite_c_abi_types_in_expr_node_matching(
    expr: &mut Expr,
    shapes: &BTreeSet<(Vec<Type>, Type)>,
) {
    match expr {
        Expr::Cast { ty, .. } => rewrite_c_abi_type_matching(ty, shapes),
        Expr::Transmute { from, to, .. } => {
            rewrite_c_abi_type_matching(from, shapes);
            rewrite_c_abi_type_matching(to, shapes);
        }
        Expr::MethodCallGeneric { type_args, .. } => {
            for ty in type_args.iter_mut() {
                rewrite_c_abi_type_matching(ty, shapes);
            }
        }
        _ => {}
    }
}

fn rewrite_c_abi_type_matching(ty: &mut Type, shapes: &BTreeSet<(Vec<Type>, Type)>) {
    if let Type::FnPtr { abi, params, ret } = ty
        && *abi == Abi::C
        && shapes.contains(&(params.clone(), (**ret).clone()))
    {
        *abi = Abi::CUnwind;
    }
    match ty {
        Type::FnPtr { params, ret, .. } => {
            for param in params.iter_mut() {
                rewrite_c_abi_type_matching(param, shapes);
            }
            rewrite_c_abi_type_matching(ret, shapes);
        }
        Type::Ref { inner, .. }
        | Type::Slice(inner)
        | Type::Ptr { inner, .. }
        | Type::Complex(inner) => rewrite_c_abi_type_matching(inner, shapes),
        Type::Array { elem, .. } => rewrite_c_abi_type_matching(elem, shapes),
        Type::Generic { args, .. } => {
            for arg in args.iter_mut() {
                rewrite_c_abi_type_matching(arg, shapes);
            }
        }
        _ => {}
    }
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

/// Rewrites `break`/`continue` statements in `body` that target a label not
/// opened by a loop within `body` itself into `return <code>;`, so the
/// closure can carry the jump out as a signal for the caller to perform
/// after `catch_unwind` returns (a `break`/`continue` inside a closure
/// can't reach a label defined outside it). Returns the escaping statements
/// in first-encountered order (index + 1 is the signal code used for each),
/// or `None` if `body` contains other disallowed control flow (`return`, or
/// a `break`/`continue` this rewrite can't classify).
fn extract_escaping_control_flow(body: &mut [IndentStmt]) -> Option<Vec<Stmt>> {
    let mut escapes = Vec::new();
    let mut open_labels = Vec::new();
    rewrite_escaping_control_flow(body, &mut open_labels, &mut escapes).then_some(escapes)
}

fn rewrite_escaping_control_flow(
    body: &mut [IndentStmt],
    open_labels: &mut Vec<Option<Label>>,
    escapes: &mut Vec<Stmt>,
) -> bool {
    body.iter_mut()
        .all(|indent| rewrite_escaping_control_flow_in_stmt(&mut indent.stmt, open_labels, escapes))
}

fn rewrite_escaping_control_flow_in_stmt(
    stmt: &mut Stmt,
    open_labels: &mut Vec<Option<Label>>,
    escapes: &mut Vec<Stmt>,
) -> bool {
    match stmt {
        Stmt::Return(_) => false,
        Stmt::Break(target) => {
            if target
                .as_ref()
                .is_some_and(|label| open_labels.iter().flatten().any(|open| open == label))
                || (target.is_none() && !open_labels.is_empty())
            {
                return true;
            }
            let signal = Stmt::Break(target.clone());
            *stmt = Stmt::Return(Some(Expr::Value(RustValue::I64(escape_code(
                escapes, signal,
            )))));
            true
        }
        Stmt::Continue(target) => {
            if target
                .as_ref()
                .is_some_and(|label| open_labels.iter().flatten().any(|open| open == label))
                || (target.is_none() && !open_labels.is_empty())
            {
                return true;
            }
            let signal = Stmt::Continue(target.clone());
            *stmt = Stmt::Return(Some(Expr::Value(RustValue::I64(escape_code(
                escapes, signal,
            )))));
            true
        }
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
            rewrite_escaping_control_flow(then_body, open_labels, escapes)
                && rewrite_escaping_control_flow(else_body, open_labels, escapes)
        }
        Stmt::Loop { label, body } => {
            open_labels.push(label.clone());
            let ok = rewrite_escaping_control_flow(body, open_labels, escapes);
            open_labels.pop();
            ok
        }
        Stmt::For { body, .. } => {
            open_labels.push(None);
            let ok = rewrite_escaping_control_flow(body, open_labels, escapes);
            open_labels.pop();
            ok
        }
        Stmt::While { body, .. } => {
            open_labels.push(None);
            let ok = rewrite_escaping_control_flow(&mut body.stmts, open_labels, escapes);
            open_labels.pop();
            ok
        }
        Stmt::Scope { body } => rewrite_escaping_control_flow(body, open_labels, escapes),
        Stmt::LabeledBlock { label, body } => {
            open_labels.push(Some(label.clone()));
            let ok = rewrite_escaping_control_flow(body, open_labels, escapes);
            open_labels.pop();
            ok
        }
        Stmt::Unsafe { body } | Stmt::Block(body) => {
            rewrite_escaping_control_flow(&mut body.stmts, open_labels, escapes)
        }
        Stmt::Match { arms, .. } => arms
            .iter_mut()
            .all(|arm| rewrite_escaping_control_flow(&mut arm.body, open_labels, escapes)),
        Stmt::Let { .. }
        | Stmt::Assign { .. }
        | Stmt::CompoundAssign { .. }
        | Stmt::InlineAsm(_)
        | Stmt::Expr(_) => true,
    }
}

fn escape_code(escapes: &mut Vec<Stmt>, signal: Stmt) -> i64 {
    if let Some(index) = escapes.iter().position(|existing| existing == &signal) {
        (index + 1) as i64
    } else {
        escapes.push(signal);
        escapes.len() as i64
    }
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
        fields: StructFields::Named(vec![crate::backend::rust_ast::StructField {
            attrs: Vec::new(),
            name: "value".to_string(),
            ty: Type::Prim(Prim::I32),
        }]),
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
        let Some(escapes) = extract_escaping_control_flow(&mut closure_body[..checked_len]) else {
            break;
        };
        if strip_trailing_return {
            closure_body.pop();
        }

        body.truncate(guard_index);
        let payload_name = payload_type_name(&buffer);
        buffers.insert(buffer);

        let result_name = format!("__sj_{binding_name}");
        let payload_var = format!("__sj_payload_{binding_name}");

        let closure_tail = (!escapes.is_empty()).then(|| Box::new(Expr::Value(RustValue::I64(0))));
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
                            tail: closure_tail,
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

        let ok_arm = if escapes.is_empty() {
            MatchArm {
                pattern: Pattern::TupleStruct {
                    name: "Ok".into(),
                    fields: vec![Pattern::Wildcard],
                },
                body: Vec::new(),
            }
        } else {
            let signal_var = format!("__sj_signal_{result_name}");
            let dispatch_arms = escapes
                .iter()
                .enumerate()
                .map(|(index, escape)| MatchArm {
                    pattern: Pattern::I64((index + 1) as i64),
                    body: vec![stmt_indent(escape.clone())],
                })
                .chain(std::iter::once(MatchArm {
                    pattern: Pattern::Wildcard,
                    body: Vec::new(),
                }))
                .collect();
            MatchArm {
                pattern: Pattern::TupleStruct {
                    name: "Ok".into(),
                    fields: vec![Pattern::Binding(signal_var.clone().into())],
                },
                body: vec![stmt_indent(Stmt::Match {
                    expr: Expr::Var(signal_var.into()),
                    arms: dispatch_arms,
                })],
            }
        };

        let match_stmt = stmt_indent(Stmt::Match {
            expr: Expr::Var(result_name.into()),
            arms: vec![
                ok_arm,
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
