use std::collections::BTreeMap;

use crate::fixups::facts::FunctionId;
use crate::fixups::facts::atomic_locals::place_local_target;
use crate::fixups::salsa::SalsaFacts;
use crate::fixups::support::walk;
use crate::function_identity::{CallBinding, FunctionIdentity, Known, known_call};
use crate::rust_ast::{
    AtomicPlace, AtomicType, Attr, Block, Derive, Expr, ExternDecl, ExternFnDecl, FnDef, FnParam,
    Ident, ImplItem, IndentStmt, Item, Program, Repr, Stmt, StructDef, StructFields, Type, UnaryOp,
};

use super::plan::TouchedItems;
use super::{
    AnonymousStructPlan, AnonymousStructSet, AtomicPromotionSet, Evidence, EvidenceDetail,
    ExprSite, LazySingletonPlan, LazySingletonSet, Predicate, Proof, PtrLenPlan, PtrLenPlanSet,
    QueryContext, QueryResult, Rejection, RejectionReason,
};

pub(in crate::fixups) struct ProgramReplacement {
    pub(in crate::fixups) replacement: Program,
    pub(in crate::fixups) touched: TouchedItems,
}

pub(in crate::fixups) fn rewrite_anonymous_structs(
    query: &QueryContext<'_>,
    structs: AnonymousStructSet,
) -> QueryResult<ProgramReplacement> {
    let site = structs.structs.first().map(|plan| ExprSite {
        item_index: plan.item_index,
        path: Default::default(),
        fact_path: Default::default(),
    });
    let mut replacement = query.snapshot_program().clone();
    if !apply_anonymous_structs(&mut replacement, query.snapshot_salsa(), structs.structs) {
        return Err(Rejection::new(
            Predicate::AnonymousStructDomain,
            site,
            RejectionReason::Contradicted,
            Vec::new(),
        ));
    }
    Ok(Proof::new(
        ProgramReplacement {
            replacement,
            touched: TouchedItems::unbounded(),
        },
        Vec::new(),
    ))
}

pub(in crate::fixups) fn rewrite_atomic_locals(
    query: &QueryContext<'_>,
    promotions: AtomicPromotionSet,
) -> QueryResult<ProgramReplacement> {
    let site = promotions.locals.first().map(|plan| ExprSite {
        item_index: plan.function_item_index,
        path: Default::default(),
        fact_path: Default::default(),
    });
    let mut replacement = query.snapshot_program().clone();
    if !apply_atomic_locals(&mut replacement, &promotions) {
        return Err(Rejection::new(
            Predicate::AtomicPromotionDomain,
            site,
            RejectionReason::Contradicted,
            Vec::new(),
        ));
    }
    Ok(Proof::new(
        ProgramReplacement {
            replacement,
            touched: TouchedItems::unbounded(),
        },
        Vec::new(),
    ))
}

fn apply_atomic_locals(program: &mut Program, promotions: &AtomicPromotionSet) -> bool {
    let mut changed = false;
    let promoted_globals: BTreeMap<String, AtomicType> = promotions
        .globals
        .iter()
        .map(|plan| (plan.name.clone(), plan.ty))
        .collect();
    rewrite_atomic_statics(&mut program.items, &promoted_globals, &mut changed);
    for (item_index, item) in program.items.iter_mut().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let mut promoted: BTreeMap<String, AtomicType> = promotions
            .locals
            .iter()
            .filter(|plan| plan.function_item_index == item_index)
            .map(|plan| (plan.name.clone(), plan.ty))
            .collect();
        for (name, ty) in &promoted_globals {
            promoted.entry(name.clone()).or_insert(*ty);
        }
        if promoted.is_empty() {
            continue;
        }
        rewrite_atomic_decls(&mut f.body, &promoted, &mut changed);
        walk::body_exprs_mut_with(&mut f.body, &mut |expr| {
            rewrite_atomic_expr(expr, &promoted, &mut changed);
            true
        });
    }
    changed
}

fn rewrite_atomic_statics(
    items: &mut [Item],
    promoted: &BTreeMap<String, AtomicType>,
    changed: &mut bool,
) {
    for item in items {
        match item {
            Item::Static {
                mutable,
                name,
                ty,
                init,
                ..
            } => {
                if let Some(atomic_ty) = promoted.get(name) {
                    *mutable = false;
                    *ty = Type::Custom(atomic_type_path(*atomic_ty).into());
                    *init = Expr::AtomicNew {
                        ty: *atomic_ty,
                        value: Box::new(init.clone()),
                    };
                    *changed = true;
                }
            }
            Item::Cfg { item, .. } => {
                rewrite_atomic_statics(std::slice::from_mut(item.as_mut()), promoted, changed);
            }
            _ => {}
        }
    }
}

fn rewrite_atomic_decls(
    body: &mut [IndentStmt],
    promoted: &BTreeMap<String, AtomicType>,
    changed: &mut bool,
) {
    for indent in body.iter_mut() {
        match &mut indent.stmt {
            Stmt::Let {
                name,
                mutable: mutable @ true,
                ty,
                init: Some(init),
            } => {
                if let Some(atomic_ty) = promoted.get(name) {
                    *mutable = false;
                    *ty = None;
                    *init = Expr::AtomicNew {
                        ty: *atomic_ty,
                        value: Box::new(init.clone()),
                    };
                    *changed = true;
                }
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
                rewrite_atomic_decls(then_body, promoted, changed);
                rewrite_atomic_decls(else_body, promoted, changed);
            }
            Stmt::Loop { body, .. }
            | Stmt::For { body, .. }
            | Stmt::Scope { body }
            | Stmt::LabeledBlock { body, .. } => rewrite_atomic_decls(body, promoted, changed),
            Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
                rewrite_atomic_decls(&mut body.stmts, promoted, changed)
            }
            Stmt::Match { arms, .. } => {
                for arm in arms {
                    rewrite_atomic_decls(&mut arm.body, promoted, changed);
                }
            }
            _ => {}
        }
    }
}

fn rewrite_atomic_expr(
    expr: &mut Expr,
    promoted: &BTreeMap<String, AtomicType>,
    changed: &mut bool,
) {
    if let Expr::Unsafe(block) = expr
        && block.stmts.is_empty()
        && let Some(tail) = &mut block.tail
        && promote_atomic_place(tail, promoted)
    {
        *expr = std::mem::replace(tail.as_mut(), Expr::Todo(String::new()));
        *changed = true;
        return;
    }
    if promote_atomic_place(expr, promoted) {
        *changed = true;
    }
}

fn promote_atomic_place(expr: &mut Expr, promoted: &BTreeMap<String, AtomicType>) -> bool {
    let (place, ty) = match expr {
        Expr::AtomicLoad { place, ty, .. }
        | Expr::AtomicStore { place, ty, .. }
        | Expr::AtomicFetch { place, ty, .. }
        | Expr::AtomicSwap { place, ty, .. }
        | Expr::AtomicCompareExchange { place, ty, .. } => (place, *ty),
        _ => return false,
    };
    let Some((local, local_ty)) = place_local_target(place, promoted) else {
        return false;
    };
    if local_ty != ty {
        return false;
    }
    *place = AtomicPlace::Local(local);
    true
}

fn atomic_type_path(ty: AtomicType) -> &'static str {
    match ty {
        AtomicType::I8 => "std::sync::atomic::AtomicI8",
        AtomicType::U8 => "std::sync::atomic::AtomicU8",
        AtomicType::I16 => "std::sync::atomic::AtomicI16",
        AtomicType::U16 => "std::sync::atomic::AtomicU16",
        AtomicType::I32 => "std::sync::atomic::AtomicI32",
        AtomicType::U32 => "std::sync::atomic::AtomicU32",
        AtomicType::I64 => "std::sync::atomic::AtomicI64",
        AtomicType::U64 => "std::sync::atomic::AtomicU64",
        AtomicType::Isize => "std::sync::atomic::AtomicIsize",
        AtomicType::Usize => "std::sync::atomic::AtomicUsize",
        AtomicType::Bool => "std::sync::atomic::AtomicBool",
    }
}

pub(in crate::fixups) fn rewrite_lazy_singletons(
    query: &QueryContext<'_>,
    set: LazySingletonSet,
) -> QueryResult<ProgramReplacement> {
    let site = set.singletons.first().map(|plan| ExprSite {
        item_index: plan.function_item_index,
        path: Default::default(),
        fact_path: Default::default(),
    });
    let mut replacement = query.snapshot_program().clone();
    if !apply_lazy_singletons(&mut replacement, &set.singletons) {
        return Err(Rejection::new(
            Predicate::LazySingletonDomain,
            site,
            RejectionReason::Contradicted,
            Vec::new(),
        ));
    }
    let touched = TouchedItems {
        in_place: set
            .singletons
            .iter()
            .flat_map(|plan| [plan.function_item_index, plan.payload_item_index])
            .collect(),
        removed: set
            .singletons
            .iter()
            .map(|plan| plan.flag_item_index)
            .collect(),
        unbounded: false,
    };
    Ok(Proof::new(
        ProgramReplacement {
            replacement,
            touched,
        },
        Vec::new(),
    ))
}

pub(in crate::fixups) fn rewrite_printf_fallback(
    query: &QueryContext<'_>,
) -> QueryResult<ProgramReplacement> {
    let mut replacement = query.snapshot_program().clone();
    if !apply_printf_fallback(&mut replacement) {
        return Err(Rejection::new(
            Predicate::PrintfCall,
            None,
            RejectionReason::Contradicted,
            Vec::new(),
        ));
    }
    Ok(Proof::new(
        ProgramReplacement {
            replacement,
            touched: TouchedItems::unbounded(),
        },
        Vec::new(),
    ))
}

fn apply_printf_fallback(program: &mut Program) -> bool {
    if program_has_printf_call(program) {
        let mut changed = ensure_stdout_and_fflush_externs(program);
        changed |= wrap_remaining_raw_printf_calls(program);
        changed
    } else {
        prune_printf_extern(program)
    }
}

fn ensure_stdout_and_fflush_externs(program: &mut Program) -> bool {
    let has_stdout = program.items.iter().any(|item| {
        matches!(item, Item::ExternBlock { decls, .. } if decls.iter().any(|decl| matches!(decl, ExternDecl::Static { name, .. } if name == "stdout")))
    });
    let has_fflush = program.items.iter().any(|item| {
        matches!(item, Item::ExternBlock { decls, .. } if decls.iter().any(|decl| matches!(decl, ExternDecl::Fn(f) if f.name == "fflush")))
    });
    if has_stdout && has_fflush {
        return false;
    }
    let mut new_decls = Vec::new();
    if !has_stdout {
        new_decls.push(stdout_static_decl());
    }
    if !has_fflush {
        new_decls.push(fflush_fn_decl());
    }
    if let Some(Item::ExternBlock { decls, .. }) = program
        .items
        .iter_mut()
        .find(|item| matches!(item, Item::ExternBlock { abi, .. } if abi == "C"))
    {
        decls.extend(new_decls);
    } else {
        program.items.insert(
            0,
            Item::ExternBlock {
                abi: "C".into(),
                decls: new_decls,
            },
        );
    }
    true
}

fn stdout_static_decl() -> ExternDecl {
    ExternDecl::Static {
        attrs: Vec::new(),
        mutable: true,
        name: "stdout".into(),
        ty: Type::parse("*mut libc::FILE"),
    }
}

fn fflush_fn_decl() -> ExternDecl {
    ExternDecl::Fn(ExternFnDecl {
        identity: crate::function_identity::FunctionIdentity::Unknown,
        name: "fflush".into(),
        params: vec![FnParam {
            name: "_0".into(),
            mutable: false,
            ty: Type::parse("*mut libc::FILE"),
        }],
        variadic: false,
        ret: Some(Type::parse("i32")),
    })
}

fn wrap_remaining_raw_printf_calls(program: &mut Program) -> bool {
    let mut changed = false;
    for item in &mut program.items {
        if let Item::Fn(f) = item {
            changed |= wrap_raw_printf_in_body(&mut f.body);
        }
    }
    changed
}

fn wrap_raw_printf_in_body(body: &mut Vec<IndentStmt>) -> bool {
    let mut changed = false;
    let mut index = 0;
    while index < body.len() {
        let mut path = Vec::new();
        walk::nested_body_vecs_mut_with_path(&mut body[index].stmt, &mut path, &mut |body, _| {
            changed |= wrap_raw_printf_in_body(body);
        });
        if is_raw_printf_call_stmt(&body[index].stmt) {
            let depth = body[index].depth;
            body.insert(
                index + 1,
                IndentStmt {
                    depth,
                    stmt: fflush_after_stmt(),
                },
            );
            body.insert(
                index,
                IndentStmt {
                    depth,
                    stmt: flush_before_stmt(),
                },
            );
            changed = true;
            index += 3;
        } else {
            index += 1;
        }
    }
    changed
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

fn is_raw_printf_call_stmt(stmt: &Stmt) -> bool {
    let Stmt::Expr(expr) = stmt else {
        return false;
    };
    known_call(peel_empty_unsafe(expr)) == Some(Known::Printf)
}

fn flush_before_stmt() -> Stmt {
    Stmt::Expr(Expr::MethodCall {
        recv: Box::new(Expr::Call {
            binding: CallBinding::Generated,
            func: Box::new(Expr::Var("std::io::Write::flush".into())),
            args: vec![Expr::Ref {
                mutable: true,
                expr: Box::new(Expr::Call {
                    binding: CallBinding::Generated,
                    func: Box::new(Expr::Var("std::io::stdout".into())),
                    args: Vec::new(),
                }),
            }],
        }),
        method: "unwrap".into(),
        args: Vec::new(),
    })
}

fn fflush_after_stmt() -> Stmt {
    Stmt::Expr(Expr::Unsafe(Box::new(Block {
        stmts: Vec::new(),
        tail: Some(Box::new(Expr::Call {
            binding: CallBinding::Generated,
            func: Box::new(Expr::Var("fflush".into())),
            args: vec![Expr::Cast {
                expr: Box::new(Expr::Unsafe(Box::new(Block {
                    stmts: Vec::new(),
                    tail: Some(Box::new(Expr::Var("stdout".into()))),
                }))),
                ty: Type::parse("*mut libc::FILE"),
            }],
        })),
    })))
}

fn prune_printf_extern(program: &mut Program) -> bool {
    let before_items = program.items.len();
    let before_decls = program
        .items
        .iter()
        .filter_map(|item| match item {
            Item::ExternBlock { decls, .. } => Some(decls.len()),
            _ => None,
        })
        .sum::<usize>();
    program.items.retain_mut(|item| match item {
        Item::ExternBlock { decls, .. } => {
            decls.retain(|decl| {
                !matches!(decl, ExternDecl::Fn(f) if f.identity == FunctionIdentity::Known(Known::Printf))
            });
            !decls.is_empty()
        }
        _ => true,
    });
    let after_decls = program
        .items
        .iter()
        .filter_map(|item| match item {
            Item::ExternBlock { decls, .. } => Some(decls.len()),
            _ => None,
        })
        .sum::<usize>();
    program.items.len() != before_items || after_decls != before_decls
}

fn program_has_printf_call(program: &Program) -> bool {
    program.items.iter().any(|item| match item {
        Item::Fn(f) => body_has_printf_call(&f.body),
        _ => false,
    })
}

fn body_has_printf_call(body: &[IndentStmt]) -> bool {
    body.iter().any(|indent| stmt_has_printf_call(&indent.stmt))
}

fn block_has_printf_call(block: &Block) -> bool {
    body_has_printf_call(&block.stmts) || block.tail.as_deref().is_some_and(expr_has_printf_call)
}

fn stmt_has_printf_call(stmt: &Stmt) -> bool {
    match stmt {
        Stmt::Let { init, .. } => init.as_ref().is_some_and(expr_has_printf_call),
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            expr_has_printf_call(cond)
                || body_has_printf_call(then_body)
                || expr_has_printf_call(then_value)
                || body_has_printf_call(else_body)
                || expr_has_printf_call(else_value)
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            expr_has_printf_call(target) || expr_has_printf_call(value)
        }
        Stmt::Expr(expr) => expr_has_printf_call(expr),
        Stmt::Return(expr) => expr.as_ref().is_some_and(expr_has_printf_call),
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
            block_has_printf_call(body)
        }
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            expr_has_printf_call(cond)
                || body_has_printf_call(then_body)
                || body_has_printf_call(else_body)
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            body_has_printf_call(body)
        }
        Stmt::For { iter, body, .. } => expr_has_printf_call(iter) || body_has_printf_call(body),
        Stmt::Match { expr, arms } => {
            expr_has_printf_call(expr) || arms.iter().any(|arm| body_has_printf_call(&arm.body))
        }
        Stmt::InlineAsm(_) | Stmt::Break(_) | Stmt::Continue(_) => false,
    }
}

fn expr_has_printf_call(expr: &Expr) -> bool {
    match expr {
        Expr::Call { func, args, .. } => {
            known_call(expr) == Some(Known::Printf)
                || expr_has_printf_call(func)
                || args.iter().any(expr_has_printf_call)
        }
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => expr_has_printf_call(expr),
        Expr::Binary { lhs, rhs, .. } => expr_has_printf_call(lhs) || expr_has_printf_call(rhs),
        Expr::Range { start, end } => expr_has_printf_call(start) || expr_has_printf_call(end),
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            expr_has_printf_call(recv) || args.iter().any(expr_has_printf_call)
        }
        Expr::Field { base, .. } | Expr::TupleField { base, .. } => expr_has_printf_call(base),
        Expr::ArrayPtr { array, .. } => expr_has_printf_call(array),
        Expr::Index { base, index } => expr_has_printf_call(base) || expr_has_printf_call(index),
        Expr::StructLit { fields, .. } => {
            fields.iter().any(|(_, value)| expr_has_printf_call(value))
        }
        Expr::TupleStructLit { fields, .. } => fields.iter().any(expr_has_printf_call),
        Expr::ArrayLit(elems) => elems.iter().any(expr_has_printf_call),
        Expr::ArrayRepeat { elem, .. } => expr_has_printf_call(elem),
        Expr::VecLit(elems) => elems.iter().any(expr_has_printf_call),
        Expr::VecRepeat { elem, len } => expr_has_printf_call(elem) || expr_has_printf_call(len),
        Expr::Macro { args, .. } => args.iter().any(expr_has_printf_call),
        Expr::Closure { body, .. } => expr_has_printf_call(body),
        Expr::Match { expr, arms } => {
            expr_has_printf_call(expr) || arms.iter().any(|arm| expr_has_printf_call(&arm.value))
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            expr_has_printf_call(cond)
                || expr_has_printf_call(then_expr)
                || expr_has_printf_call(else_expr)
        }
        Expr::Block(block) | Expr::Unsafe(block) => block_has_printf_call(block),
        Expr::CopyNonoverlapping { src, dst, .. } => {
            expr_has_printf_call(src) || expr_has_printf_call(dst)
        }
        Expr::PtrCopy {
            src, dst, count, ..
        } => expr_has_printf_call(src) || expr_has_printf_call(dst) || expr_has_printf_call(count),
        Expr::WriteBytes { dst, val, count } => {
            expr_has_printf_call(dst) || expr_has_printf_call(val) || expr_has_printf_call(count)
        }
        Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. } => {
            place.ptr_expr().is_some_and(expr_has_printf_call)
        }
        Expr::AtomicStore { place, value, .. }
        | Expr::AtomicFetch { place, value, .. }
        | Expr::AtomicSwap { place, value, .. } => {
            place.ptr_expr().is_some_and(expr_has_printf_call) || expr_has_printf_call(value)
        }
        Expr::AtomicNew { value, .. } => expr_has_printf_call(value),
        Expr::AtomicCompareExchange {
            place,
            expected,
            desired,
            ..
        } => {
            place.ptr_expr().is_some_and(expr_has_printf_call)
                || expr_has_printf_call(expected)
                || expr_has_printf_call(desired)
        }
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::CStr(_)
        | Expr::Var(_)
        | Expr::Path(_)
        | Expr::AtomicFence { .. }
        | Expr::Todo(_) => false,
    }
}

pub(in crate::fixups) fn rewrite_ptr_len(
    query: &QueryContext<'_>,
    set: PtrLenPlanSet,
) -> QueryResult<ProgramReplacement> {
    let site = set.plans.first().map(|plan| ExprSite {
        item_index: plan.item_index,
        path: Default::default(),
        fact_path: Default::default(),
    });
    let mut grouped = BTreeMap::<String, Vec<PtrLenPlan>>::new();
    for plan in set.plans {
        grouped
            .entry(plan.function_name.clone())
            .or_default()
            .push(plan);
    }
    let mut replacement = query.snapshot_program().clone();
    if !apply_ptr_len(&mut replacement, &grouped) {
        return Err(Rejection::new(
            Predicate::PtrLenSlice,
            site,
            RejectionReason::Contradicted,
            Vec::new(),
        ));
    }
    Ok(Proof::new(
        ProgramReplacement {
            replacement,
            touched: TouchedItems::unbounded(),
        },
        Vec::new(),
    ))
}

pub(in crate::fixups) fn rewrite_sort_search(
    query: &QueryContext<'_>,
) -> QueryResult<ProgramReplacement> {
    let predicate = Predicate::SortSearchDomain;
    let mut replacement = query.snapshot_program().clone();
    let Some(comparators) = super::sort_search::rewrite_program(&mut replacement) else {
        return Err(Rejection::new(
            predicate,
            None,
            RejectionReason::Contradicted,
            Vec::new(),
        ));
    };
    let evidence = vec![Evidence {
        predicate,
        site: ExprSite {
            item_index: 0,
            path: Default::default(),
            fact_path: Default::default(),
        },
        detail: EvidenceDetail::SortSearchDomain { comparators },
    }];
    Ok(Proof::new(
        ProgramReplacement {
            replacement,
            touched: TouchedItems::unbounded(),
        },
        evidence,
    ))
}

fn apply_anonymous_structs(
    program: &mut Program,
    salsa: &SalsaFacts,
    structs: Vec<AnonymousStructPlan>,
) -> bool {
    let plans = structs
        .into_iter()
        .map(|plan| (plan.original_name.clone(), plan))
        .collect::<BTreeMap<_, _>>();
    let record_fields = record_field_types(program);
    let global_types = global_types(program);
    let mut changed = false;
    for item in &mut program.items {
        changed |= rewrite_item(item, &plans, &record_fields, &global_types, salsa);
    }
    changed
}

fn record_field_types(program: &Program) -> BTreeMap<String, BTreeMap<String, Type>> {
    program
        .items
        .iter()
        .filter_map(|item| match item {
            Item::Record(record) => Some((
                record.name.clone(),
                record
                    .fields
                    .iter()
                    .map(|field| (field.name.to_string(), field.ty.clone()))
                    .collect(),
            )),
            _ => None,
        })
        .collect()
}

fn global_types(program: &Program) -> BTreeMap<String, Type> {
    program
        .items
        .iter()
        .filter_map(|item| match item {
            Item::Static { name, ty, .. } => Some((name.clone(), ty.clone())),
            _ => None,
        })
        .collect()
}

fn rewrite_item(
    item: &mut Item,
    plans: &BTreeMap<String, AnonymousStructPlan>,
    record_fields: &BTreeMap<String, BTreeMap<String, Type>>,
    global_types: &BTreeMap<String, Type>,
    salsa: &SalsaFacts,
) -> bool {
    match item {
        Item::Record(record) => {
            let mut changed = false;
            for field in &mut record.fields {
                changed |= rewrite_type(&mut field.ty, plans);
            }
            let Some(mut plan) = plans.get(&record.name).cloned() else {
                return changed;
            };
            for field in &mut plan.fields {
                rewrite_type(&mut field.ty, plans);
            }
            *item = Item::Struct(StructDef {
                attrs: vec![
                    Attr::Repr(vec![Repr::C]),
                    Attr::Derive(vec![Derive::Clone, Derive::Copy]),
                ],
                vis: record.vis,
                generics: vec![],
                name: plan.generated_name,
                fields: StructFields::Tuple(
                    plan.fields.into_iter().map(|field| field.ty).collect(),
                ),
            });
            true
        }
        Item::Fn(function) => {
            let function_id = salsa.function_by_name(&function.name);
            rewrite_fn(
                function,
                function_id,
                plans,
                record_fields,
                global_types,
                salsa,
            )
        }
        Item::Static { ty, init, .. } | Item::Const { ty, init, .. } => {
            rewrite_type(ty, plans) | rewrite_expr(init, global_types, plans, record_fields)
        }
        Item::Struct(record) => rewrite_struct_def(record, plans),
        Item::Impl(block) => {
            let mut changed = rewrite_type(&mut block.self_ty, plans);
            for item in &mut block.items {
                match item {
                    ImplItem::AssocType { ty, .. } => changed |= rewrite_type(ty, plans),
                    ImplItem::Method(method) => {
                        for param in &mut method.params {
                            changed |= rewrite_type(&mut param.ty, plans);
                        }
                        if let Some(ret) = &mut method.ret {
                            changed |= rewrite_type(ret, plans);
                        }
                        changed |=
                            rewrite_expr(&mut method.body, global_types, plans, record_fields);
                    }
                }
            }
            changed
        }
        Item::Cfg { item, .. } => rewrite_item(item, plans, record_fields, global_types, salsa),
        Item::ExternBlock { decls, .. } => {
            let mut changed = false;
            for decl in decls {
                match decl {
                    crate::rust_ast::ExternDecl::Fn(function) => {
                        for param in &mut function.params {
                            changed |= rewrite_type(&mut param.ty, plans);
                        }
                        if let Some(ret) = &mut function.ret {
                            changed |= rewrite_type(ret, plans);
                        }
                    }
                    crate::rust_ast::ExternDecl::Static { ty, .. } => {
                        changed |= rewrite_type(ty, plans)
                    }
                }
            }
            changed
        }
        Item::Comment(_)
        | Item::Macro { .. }
        | Item::CrateAttrs(_)
        | Item::Mod { .. }
        | Item::Use { .. }
        | Item::Enum(_)
        | Item::SupportModule(_) => false,
    }
}

fn rewrite_fn(
    function: &mut FnDef,
    function_id: Option<FunctionId>,
    plans: &BTreeMap<String, AnonymousStructPlan>,
    record_fields: &BTreeMap<String, BTreeMap<String, Type>>,
    global_types: &BTreeMap<String, Type>,
    salsa: &SalsaFacts,
) -> bool {
    let mut local_types = global_types.clone();
    if let Some(function_id) = function_id {
        local_types.extend(local_anonymous_types(function_id, salsa));
    }
    let mut changed = false;
    for param in &mut function.params {
        changed |= rewrite_type(&mut param.ty, plans);
    }
    if let Some(ret) = &mut function.ret {
        changed |= rewrite_type(ret, plans);
    }
    changed |= rewrite_body(&mut function.body, &local_types, plans, record_fields);
    changed
}

fn local_anonymous_types(function: FunctionId, salsa: &SalsaFacts) -> BTreeMap<String, Type> {
    salsa
        .binding_names_and_types(function)
        .into_iter()
        .map(|(name, rendered)| (name, Type::parse(&rendered)))
        .collect()
}

fn rewrite_body(
    body: &mut Vec<IndentStmt>,
    local_types: &BTreeMap<String, Type>,
    plans: &BTreeMap<String, AnonymousStructPlan>,
    record_fields: &BTreeMap<String, BTreeMap<String, Type>>,
) -> bool {
    let mut changed = false;
    for indent in body {
        changed |= rewrite_stmt(&mut indent.stmt, local_types, plans, record_fields);
        walk::nested_body_vecs_mut_with_path(&mut indent.stmt, &mut Vec::new(), &mut |body, _| {
            changed |= rewrite_body(body, local_types, plans, record_fields);
        });
    }
    changed
}

fn rewrite_stmt(
    stmt: &mut Stmt,
    local_types: &BTreeMap<String, Type>,
    plans: &BTreeMap<String, AnonymousStructPlan>,
    record_fields: &BTreeMap<String, BTreeMap<String, Type>>,
) -> bool {
    let mut changed = false;
    match stmt {
        Stmt::Let { ty, .. } | Stmt::LetIf { ty, .. } => {
            if let Some(ty) = ty {
                changed |= rewrite_type(ty, plans);
            }
        }
        _ => {}
    }
    walk::stmt_exprs_mut_with(stmt, &mut |expr| {
        changed |= rewrite_expr(expr, local_types, plans, record_fields);
        true
    });
    changed
}

fn rewrite_expr(
    expr: &mut Expr,
    local_types: &BTreeMap<String, Type>,
    plans: &BTreeMap<String, AnonymousStructPlan>,
    record_fields: &BTreeMap<String, BTreeMap<String, Type>>,
) -> bool {
    match expr {
        Expr::StructLit { name, fields } => {
            let Some(plan) = plans.get(name).cloned() else {
                return false;
            };
            let Some(values) = positional_fields(fields, &plan) else {
                return false;
            };
            *expr = Expr::TupleStructLit {
                name: plan.generated_name,
                fields: values,
            };
            true
        }
        Expr::Field { base, field } => {
            let Some(original) = base_original_type(base, local_types, plans, record_fields) else {
                return false;
            };
            let Some(plan) = plans.get(&original) else {
                return false;
            };
            let Some(index) = plan
                .fields
                .iter()
                .position(|candidate| candidate.name == *field)
            else {
                return false;
            };
            *expr = Expr::TupleField {
                base: Box::new((**base).clone()),
                index,
            };
            true
        }
        Expr::Cast { ty, .. } => rewrite_type(ty, plans),
        Expr::Transmute { from, to, .. } => rewrite_type(from, plans) | rewrite_type(to, plans),
        Expr::MethodCallGeneric { type_args, .. } => rewrite_types(type_args, plans),
        Expr::TupleStructLit { name, .. } => {
            if let Some(plan) = plans.get(name) {
                *name = plan.generated_name.clone();
                true
            } else {
                false
            }
        }
        _ => false,
    }
}

fn positional_fields(fields: &[(String, Expr)], plan: &AnonymousStructPlan) -> Option<Vec<Expr>> {
    if fields.len() != plan.fields.len() {
        return None;
    }
    plan.fields
        .iter()
        .map(|field| {
            fields
                .iter()
                .find_map(|(name, value)| (name == &field.name).then(|| value.clone()))
        })
        .collect()
}

fn base_original_type(
    expr: &Expr,
    local_types: &BTreeMap<String, Type>,
    plans: &BTreeMap<String, AnonymousStructPlan>,
    record_fields: &BTreeMap<String, BTreeMap<String, Type>>,
) -> Option<String> {
    let ty = expr_type(expr, local_types, plans, record_fields)?;
    let Type::Custom(name) = ty.peel_aligned() else {
        return None;
    };
    Some(name.clone())
}

fn expr_type(
    expr: &Expr,
    local_types: &BTreeMap<String, Type>,
    plans: &BTreeMap<String, AnonymousStructPlan>,
    record_fields: &BTreeMap<String, BTreeMap<String, Type>>,
) -> Option<Type> {
    match expr {
        Expr::Var(name) => local_types
            .get(name.as_str())
            .map(|ty| ty.peel_aligned().clone()),
        Expr::Unary {
            op: crate::rust_ast::UnaryOp::Deref,
            expr,
        } => expr_type(expr, local_types, plans, record_fields),
        Expr::Index { base, .. } => {
            let base_ty = expr_type(base, local_types, plans, record_fields)?;
            match base_ty.peel_aligned() {
                Type::Array { elem, .. } | Type::Slice(elem) => Some((**elem).clone()),
                _ => None,
            }
        }
        Expr::Field { base, field } => {
            let base_ty = base_original_type(base, local_types, plans, record_fields)?;
            record_fields.get(&base_ty)?.get(field).cloned()
        }
        Expr::TupleField { base, index } => {
            let base_ty = base_original_type(base, local_types, plans, record_fields)?;
            Some(plans.get(&base_ty)?.fields.get(*index)?.ty.clone())
        }
        _ => None,
    }
}

fn rewrite_struct_def(
    record: &mut StructDef,
    plans: &BTreeMap<String, AnonymousStructPlan>,
) -> bool {
    match &mut record.fields {
        StructFields::Tuple(fields) => rewrite_types(fields, plans),
        StructFields::Named(fields) => {
            let mut changed = false;
            for (_, ty) in fields {
                changed |= rewrite_type(ty, plans);
            }
            changed
        }
    }
}

fn rewrite_type(ty: &mut Type, plans: &BTreeMap<String, AnonymousStructPlan>) -> bool {
    match ty {
        Type::Custom(name) => {
            let Some(plan) = plans.get(name) else {
                return false;
            };
            *name = plan.generated_name.clone();
            true
        }
        Type::Complex(inner)
        | Type::Ref { inner, .. }
        | Type::Slice(inner)
        | Type::Ptr { inner, .. } => rewrite_type(inner, plans),
        Type::Generic { args, .. } => rewrite_types(args, plans),
        Type::Array { elem, .. } => rewrite_type(elem, plans),
        Type::FnPtr { params, ret, .. } => rewrite_types(params, plans) | rewrite_type(ret, plans),
        Type::Prim(_)
        | Type::LongDouble
        | Type::TyVar(_)
        | Type::CLib(_)
        | Type::VaList
        | Type::Str
        | Type::Unit
        | Type::Variadic
        | Type::Never => false,
    }
}

fn rewrite_types(types: &mut [Type], plans: &BTreeMap<String, AnonymousStructPlan>) -> bool {
    let mut changed = false;
    for ty in types {
        changed |= rewrite_type(ty, plans);
    }
    changed
}

/// Converts each proven lazy-init singleton's payload static to
/// `OnceLock`, collapses its accessor function to one `get_or_init` return,
/// and drops the now-dead guard-flag static. Each plan's proof already
/// established (in `QueryContext::lazy_singletons`) that the flag and
/// payload are used nowhere else in the program, so no further use-count
/// check is needed here - only that the expected items are still in place.
fn apply_lazy_singletons(program: &mut Program, singletons: &[LazySingletonPlan]) -> bool {
    if singletons.is_empty() {
        return false;
    }
    for plan in singletons {
        let Some(Item::Static {
            mutable, ty, init, ..
        }) = program.items.get_mut(plan.payload_item_index)
        else {
            return false;
        };
        *mutable = false;
        *ty = once_lock_type(plan.payload_ty.clone());
        *init = once_lock_new();

        let Some(Item::Fn(f)) = program.items.get_mut(plan.function_item_index) else {
            return false;
        };
        f.body = vec![IndentStmt {
            depth: 1,
            stmt: Stmt::Return(Some(get_or_init_deref(
                &plan.payload_name,
                plan.init_expr.clone(),
            ))),
        }];
    }
    let mut changed = true;
    let flag_names = singletons
        .iter()
        .map(|plan| plan.flag_name.as_str())
        .collect::<std::collections::BTreeSet<_>>();
    let before = program.items.len();
    program.items.retain(
        |item| !matches!(item, Item::Static { name, .. } if flag_names.contains(name.as_str())),
    );
    changed |= program.items.len() != before;
    changed
}

fn once_lock_type(payload_ty: Type) -> Type {
    Type::Generic {
        name: "std::sync::OnceLock".to_string(),
        args: vec![payload_ty],
    }
}

fn once_lock_new() -> Expr {
    Expr::Call {
        binding: crate::function_identity::CallBinding::Generated,
        func: Box::new(Expr::Var(Ident::from("std::sync::OnceLock::new"))),
        args: Vec::new(),
    }
}

fn get_or_init_deref(payload_name: &str, init_expr: Expr) -> Expr {
    Expr::Unary {
        op: UnaryOp::Deref,
        expr: Box::new(Expr::MethodCall {
            recv: Box::new(Expr::Var(Ident::from(payload_name))),
            method: "get_or_init".to_string(),
            args: vec![Expr::Closure {
                params: Vec::new(),
                body: Box::new(init_expr),
            }],
        }),
    }
}

fn apply_ptr_len(program: &mut Program, plans: &BTreeMap<String, Vec<PtrLenPlan>>) -> bool {
    let mut changed = false;
    for item in &mut program.items {
        let Item::Fn(f) = item else { continue };
        if let Some(fn_plans) = plans.get(&f.name) {
            changed |= rewrite_ptr_len_function(f, fn_plans);
        }
        changed |= rewrite_ptr_len_calls_in_body(&mut f.body, plans);
    }
    changed
}

fn rewrite_ptr_len_function(f: &mut FnDef, plans: &[PtrLenPlan]) -> bool {
    if plans.iter().any(|plan| f.params.len() <= plan.ptr_index) {
        return false;
    }
    let mut changed = false;
    for plan in plans {
        f.params[plan.ptr_index].ty = Type::Ref {
            mutable: plan.mutable,
            inner: Box::new(Type::Slice(Box::new(plan.elem.clone()))),
        };
        changed = true;
        changed |= rewrite_ptr_len_body_pointer_param(&mut f.body, &plan.ptr_name, plan.mutable);
    }
    changed
}

fn rewrite_ptr_len_body_pointer_param(body: &mut [IndentStmt], name: &str, mutable: bool) -> bool {
    let mut changed = false;
    walk::body_exprs_mut_with(body, &mut |expr| {
        if matches!(expr, Expr::Var(var) if var.as_str() == name) {
            changed = true;
            *expr = Expr::MethodCall {
                recv: Box::new(Expr::Var(name.into())),
                method: if mutable { "as_mut_ptr" } else { "as_ptr" }.into(),
                args: Vec::new(),
            };
            return false;
        }
        true
    });
    changed |= fix_ptr_len_let_pointer_mutability(body, name, mutable);
    changed
}

fn fix_ptr_len_let_pointer_mutability(body: &mut [IndentStmt], name: &str, mutable: bool) -> bool {
    let mut changed = false;
    let expected_method = if mutable { "as_mut_ptr" } else { "as_ptr" };
    for indent in body.iter_mut() {
        if let Stmt::Let {
            ty:
                Some(Type::Ptr {
                    mutable: ty_mutable,
                    ..
                }),
            init: Some(Expr::MethodCall { recv, method, args }),
            ..
        } = &mut indent.stmt
            && args.is_empty()
            && method.as_str() == expected_method
            && matches!(&**recv, Expr::Var(var) if var.as_str() == name)
            && *ty_mutable != mutable
        {
            *ty_mutable = mutable;
            changed = true;
        }
        walk::nested_body_vecs_mut_with_path(
            &mut indent.stmt,
            &mut Vec::new(),
            &mut |nested, _| {
                changed |= fix_ptr_len_let_pointer_mutability(nested, name, mutable);
            },
        );
    }
    changed
}

fn rewrite_ptr_len_calls_in_body(
    body: &mut [IndentStmt],
    plans: &BTreeMap<String, Vec<PtrLenPlan>>,
) -> bool {
    let mut changed = false;
    walk::body_exprs_mut_with(body, &mut |expr| {
        let Expr::Call { func, args, .. } = expr else {
            return true;
        };
        let Expr::Var(name) = &**func else {
            return true;
        };
        let Some(fn_plans) = plans.get(name.as_str()) else {
            return true;
        };
        if fn_plans.iter().any(|plan| args.len() <= plan.ptr_index) {
            return true;
        }
        let Some(array_names) = fn_plans
            .iter()
            .map(|plan| args.get(plan.ptr_index).and_then(ptr_len_array_pointer_arg))
            .collect::<Option<Vec<_>>>()
        else {
            return true;
        };

        for (plan, array_name) in fn_plans.iter().zip(array_names) {
            args[plan.ptr_index] = Expr::MethodCall {
                recv: Box::new(Expr::Var(Ident::from(array_name))),
                method: if plan.mutable {
                    "as_mut_slice"
                } else {
                    "as_slice"
                }
                .into(),
                args: Vec::new(),
            };
        }

        changed = true;
        false
    });
    changed
}

fn ptr_len_array_pointer_arg(expr: &Expr) -> Option<String> {
    match peel_ptr_len_pointer_view(expr) {
        Expr::Var(name) => Some(name.as_str().into()),
        _ => None,
    }
}

fn peel_ptr_len_pointer_view(expr: &Expr) -> &Expr {
    match expr {
        Expr::Cast { expr, .. }
        | Expr::Unary { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => peel_ptr_len_pointer_view(expr),
        Expr::MethodCall { recv, method, args }
            if args.is_empty() && matches!(method.as_str(), "as_ptr" | "as_mut_ptr") =>
        {
            peel_ptr_len_pointer_view(recv)
        }
        Expr::ArrayPtr { array, .. } => peel_ptr_len_pointer_view(array),
        _ => expr,
    }
}
