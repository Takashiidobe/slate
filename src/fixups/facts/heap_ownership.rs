use crate::fixups::facts::walk;
use crate::fixups::facts::{
    self, AstPath, BindingFact, BindingId, FunctionId, HeapAllocationKind, HeapExtent,
    HeapInitKind, HeapOwnershipFact, HeapReadSafety, HeapReallocFact, HeapResizeKind, HeapUseFact,
    HeapUseKind, PathSegment,
};
use crate::function_identity::{Known, known_call};
use crate::rust_ast::{BinOp, Block, Expr, IndentStmt, Prim, RustValue, Stmt, Type, UnaryOp};
use std::collections::BTreeSet;

pub(super) type OwnedHeapUses = (
    Option<usize>,
    Option<BindingId>,
    Vec<BindingId>,
    Vec<HeapUseFact>,
    Vec<HeapReallocFact>,
    HeapReadSafety,
);
pub(in crate::fixups) fn collect_for_function(
    function: FunctionId,
    body: &[IndentStmt],
    bindings: &[BindingFact],
) -> Vec<HeapOwnershipFact> {
    let mut out = Vec::new();
    for (index, pair) in body.windows(2).enumerate() {
        let Some((pointer_name, elem_ty)) = null_pointer_decl(&pair[0].stmt) else {
            continue;
        };
        let pointer_path = AstPath(vec![PathSegment::Stmt(index)]);
        let Some(pointer) =
            facts::binding_by_local_path(bindings, function, pointer_name, &pointer_path)
        else {
            continue;
        };
        let Some(candidate) =
            find_allocation(function, body, bindings, index + 1, pointer_name, &elem_ty)
        else {
            continue;
        };
        out.push(HeapOwnershipFact {
            function,
            pointer,
            allocation_temp: candidate.allocation_temp,
            size_temp: candidate.size_temp,
            free_temp: candidate.free_temp,
            aliases: candidate.aliases,
            pointer_path,
            allocation_path: AstPath(vec![PathSegment::Stmt(candidate.allocation_index)]),
            assign_path: AstPath(vec![PathSegment::Stmt(candidate.assign_index)]),
            free_path: AstPath(vec![PathSegment::Stmt(candidate.free_index)]),
            elem_ty,
            allocation: candidate.allocation,
            extent: candidate.extent.clone(),
            init: candidate.init,
            read_safety: candidate.read_safety,
            uses: candidate.uses,
            reallocations: candidate.reallocations,
        });
    }
    out
}

pub(super) struct Candidate {
    pub(super) allocation_index: usize,
    pub(super) assign_index: usize,
    pub(super) free_index: usize,
    pub(super) allocation_temp: BindingId,
    pub(super) size_temp: Option<BindingId>,
    pub(super) free_temp: Option<BindingId>,
    pub(super) aliases: Vec<BindingId>,
    pub(super) allocation: HeapAllocationKind,
    pub(super) extent: HeapExtent,
    pub(super) init: HeapInitKind,
    pub(super) elem_ty: Type,
    pub(super) read_safety: HeapReadSafety,
    pub(super) uses: Vec<HeapUseFact>,
    pub(super) reallocations: Vec<HeapReallocFact>,
}

fn find_allocation(
    function: FunctionId,
    body: &[IndentStmt],
    bindings: &[BindingFact],
    start: usize,
    pointer_name: &str,
    elem_ty: &Type,
) -> Option<Candidate> {
    for allocation_index in start..body.len() {
        let Some(allocation_call) = allocation_temp(
            &body[allocation_index].stmt,
            body,
            allocation_index,
            elem_ty,
        ) else {
            continue;
        };
        let allocation_path = AstPath(vec![PathSegment::Stmt(allocation_index)]);
        let allocation_temp = facts::binding_by_local_path(
            bindings,
            function,
            &allocation_call.name,
            &allocation_path,
        )?;
        let size_temp = temp_binding_before(
            function,
            bindings,
            body,
            allocation_index,
            allocation_call.size_name.as_deref(),
        );

        for assign_index in allocation_index + 1..body.len() {
            if !assigns_allocated_pointer(
                &body[assign_index].stmt,
                pointer_name,
                &allocation_call.name,
            ) {
                continue;
            }
            let mut candidate = Candidate {
                allocation_index,
                assign_index,
                free_index: 0,
                allocation_temp,
                size_temp,
                free_temp: None,
                aliases: Vec::new(),
                allocation: allocation_call.kind,
                extent: allocation_call.extent.clone(),
                init: allocation_call.init,
                elem_ty: elem_ty.clone(),
                read_safety: HeapReadSafety::MayReadUninitialized,
                uses: Vec::new(),
                reallocations: Vec::new(),
            };
            let (free_index, free_temp, aliases, uses, reallocations, read_safety) =
                heap_uses_are_owned(function, body, bindings, pointer_name, &candidate)?;
            candidate.free_index = free_index?;
            candidate.free_temp = free_temp;
            candidate.aliases = aliases;
            candidate.uses = uses;
            candidate.reallocations = reallocations;
            candidate.read_safety = read_safety;
            return Some(Candidate { ..candidate });
        }
    }
    None
}

pub(super) fn null_pointer_decl(stmt: &Stmt) -> Option<(&str, Type)> {
    let Stmt::Let {
        name,
        ty: Some(Type::Ptr {
            mutable: true,
            inner,
        }),
        init: Some(Expr::Value(RustValue::NullPtr)),
        ..
    } = stmt
    else {
        return None;
    };
    Some((name.as_str(), (**inner).clone()))
}

pub(super) struct AllocationCall {
    pub(super) name: String,
    size_name: Option<String>,
    pub(super) kind: HeapAllocationKind,
    pub(super) extent: HeapExtent,
    pub(super) init: HeapInitKind,
}

pub(super) fn allocation_temp(
    stmt: &Stmt,
    body: &[IndentStmt],
    index: usize,
    elem_ty: &Type,
) -> Option<AllocationCall> {
    let Stmt::Let {
        name,
        init: Some(init),
        ..
    } = stmt
    else {
        return None;
    };
    let Expr::Unsafe(block) = init else {
        return None;
    };
    let call = block.tail.as_deref()?;
    let Expr::Call { args, .. } = call else {
        return None;
    };
    match known_call(call) {
        Some(Known::Malloc) if args.len() == 1 => {
            let size_name = size_arg_name(&args[0]).map(str::to_owned);
            let size_expr = size_name
                .as_deref()
                .and_then(|name| temp_init_before(body, index, name))
                .cloned()
                .unwrap_or_else(|| strip_casts(&args[0]).clone());
            Some(AllocationCall {
                name: name.to_string(),
                size_name,
                kind: HeapAllocationKind::Malloc,
                extent: extent_from_malloc_size(&size_expr, elem_ty),
                init: HeapInitKind::Uninitialized,
            })
        }
        Some(Known::Calloc) if args.len() == 2 => {
            let count_name = size_arg_name(&args[0]).map(str::to_owned);
            let count = count_name
                .as_deref()
                .and_then(|name| temp_init_near_before(body, index, name))
                .cloned()
                .unwrap_or_else(|| strip_casts(&args[0]).clone());
            let size_name = size_arg_name(&args[1]).map(str::to_owned);
            let elem_size = size_name
                .as_deref()
                .and_then(|name| temp_init_before(body, index, name))
                .cloned()
                .unwrap_or_else(|| strip_casts(&args[1]).clone());
            Some(AllocationCall {
                name: name.to_string(),
                size_name,
                kind: HeapAllocationKind::Calloc,
                extent: extent_from_calloc_args(count, &elem_size, elem_ty),
                init: HeapInitKind::Zeroed,
            })
        }
        _ => None,
    }
}

fn size_arg_name(expr: &Expr) -> Option<&str> {
    match expr {
        Expr::Var(name) => Some(name.as_str()),
        Expr::Cast { expr, .. } => size_arg_name(expr),
        _ => None,
    }
}

fn temp_init_before<'a>(body: &'a [IndentStmt], index: usize, name: &str) -> Option<&'a Expr> {
    let stmt = &body.get(index.checked_sub(1)?)?.stmt;
    let Stmt::Let {
        name: binding,
        init: Some(init),
        ..
    } = stmt
    else {
        return None;
    };
    (binding == name).then_some(init)
}

fn temp_init_near_before<'a>(body: &'a [IndentStmt], index: usize, name: &str) -> Option<&'a Expr> {
    for offset in 1..=2 {
        if let Some(stmt_index) = index.checked_sub(offset)
            && let Some(init) = temp_init_at(body, stmt_index, name)
        {
            return Some(init);
        }
    }
    None
}

fn temp_init_at<'a>(body: &'a [IndentStmt], index: usize, name: &str) -> Option<&'a Expr> {
    let stmt = &body.get(index)?.stmt;
    let Stmt::Let {
        name: binding,
        init: Some(init),
        ..
    } = stmt
    else {
        return None;
    };
    (binding == name).then_some(init)
}

fn temp_binding_before(
    function: FunctionId,
    bindings: &[BindingFact],
    body: &[IndentStmt],
    index: usize,
    name: Option<&str>,
) -> Option<BindingId> {
    let name = name?;
    if index == 0 {
        return None;
    }
    temp_init_before(body, index, name)?;
    facts::binding_by_local_path(
        bindings,
        function,
        name,
        &AstPath(vec![PathSegment::Stmt(index - 1)]),
    )
}

fn strip_casts(expr: &Expr) -> &Expr {
    match expr {
        Expr::Cast { expr, .. } => strip_casts(expr),
        _ => expr,
    }
}

fn extent_from_malloc_size(size: &Expr, elem_ty: &Type) -> HeapExtent {
    let Some(elem_size) = type_size_bytes(elem_ty) else {
        return HeapExtent::Unknown;
    };
    if int_value(size) == Some(elem_size) {
        return HeapExtent::Scalar;
    }
    if let Some(count) = mul_count_for_elem_size(size, elem_size) {
        return HeapExtent::Elements { count };
    }
    HeapExtent::Unknown
}

fn extent_from_calloc_args(count: Expr, elem_size: &Expr, elem_ty: &Type) -> HeapExtent {
    let Some(type_size) = type_size_bytes(elem_ty) else {
        return HeapExtent::Unknown;
    };
    if int_value(elem_size) != Some(type_size) {
        return HeapExtent::Unknown;
    }
    if int_value(&count) == Some(1) {
        HeapExtent::Scalar
    } else {
        HeapExtent::Elements { count }
    }
}

fn mul_count_for_elem_size(expr: &Expr, elem_size: i64) -> Option<Expr> {
    let Expr::Binary {
        op: BinOp::Mul,
        lhs,
        rhs,
    } = strip_casts(expr)
    else {
        return None;
    };
    if int_value(lhs) == Some(elem_size) {
        return Some((**rhs).clone());
    }
    if int_value(rhs) == Some(elem_size) {
        return Some((**lhs).clone());
    }
    None
}

fn int_value(expr: &Expr) -> Option<i64> {
    match strip_casts(expr) {
        Expr::Value(RustValue::I64(value)) => Some(*value),
        Expr::Value(RustValue::I128(value)) => i64::try_from(*value).ok(),
        _ => None,
    }
}

fn type_size_bytes(ty: &Type) -> Option<i64> {
    match ty {
        Type::Prim(Prim::Bool | Prim::I8 | Prim::U8) => Some(1),
        Type::Prim(Prim::I16 | Prim::U16) => Some(2),
        Type::Prim(Prim::I32 | Prim::U32 | Prim::F32) => Some(4),
        Type::Prim(Prim::I64 | Prim::U64 | Prim::F64 | Prim::Isize | Prim::Usize) => Some(8),
        Type::Prim(Prim::I128 | Prim::U128 | Prim::F128) => Some(16),
        Type::Ptr { .. } => Some(8),
        _ => None,
    }
}

pub(super) fn assigns_allocated_pointer(
    stmt: &Stmt,
    pointer_name: &str,
    allocation_name: &str,
) -> bool {
    let Stmt::Assign { target, value } = stmt else {
        return false;
    };
    matches!(target, Expr::Var(name) if name.as_str() == pointer_name)
        && cast_source_var(value).is_some_and(|name| name == allocation_name)
}

fn cast_source_var(expr: &Expr) -> Option<&str> {
    match expr {
        Expr::Var(name) => Some(name.as_str()),
        Expr::Cast { expr, .. } => cast_source_var(expr),
        _ => None,
    }
}

fn pointer_alias_temp<'a>(stmt: &'a Stmt, pointer_name: &str) -> Option<&'a str> {
    let Stmt::Let {
        name,
        init: Some(Expr::Var(source)),
        ..
    } = stmt
    else {
        return None;
    };
    if source.as_str() == pointer_name {
        Some(name.as_str())
    } else {
        None
    }
}

fn free_call_on_any(stmt: &Stmt, names: &BTreeSet<String>) -> bool {
    let Stmt::Expr(expr) = stmt else {
        return false;
    };
    let Expr::Unsafe(block) = expr else {
        return false;
    };
    block_tail_free_arg(block)
        .and_then(cast_source_var)
        .is_some_and(|name| names.contains(name))
}

fn block_tail_free_arg(block: &Block) -> Option<&Expr> {
    let call = block.tail.as_deref()?;
    let Expr::Call { args, .. } = call else {
        return None;
    };
    if known_call(call) == Some(Known::Free) && args.len() == 1 {
        args.first()
    } else {
        None
    }
}

pub(super) fn heap_uses_are_owned(
    function: FunctionId,
    body: &[IndentStmt],
    bindings: &[BindingFact],
    pointer_name: &str,
    candidate: &Candidate,
) -> Option<OwnedHeapUses> {
    let mut aliases = BTreeSet::from([pointer_name.to_string()]);
    let mut alias_bindings = Vec::new();
    let mut free: Option<(usize, Option<BindingId>)> = None;
    let mut uses = Vec::new();
    let mut reallocations = Vec::new();
    let mut written = BTreeSet::new();
    let mut may_read_uninit = false;
    let mut index = 0;
    while index < body.len() {
        let indent = &body[index];
        if index == candidate.assign_index
            || index == candidate.allocation_index
            || Some(index) == candidate.size_temp.map(|_| candidate.allocation_index - 1)
        {
            index += 1;
            continue;
        }
        if let Some(realloc) = realloc_at(
            function,
            body,
            bindings,
            index,
            pointer_name,
            &aliases,
            candidate,
        ) {
            if free.is_some() {
                return None;
            }
            reallocations.push(realloc.fact);
            index = realloc.next_index;
            continue;
        }
        if let Some(alias) = pointer_alias_temp_from_any(&indent.stmt, &aliases) {
            if free.is_some() {
                return None;
            }
            let binding = facts::binding_by_local_path(
                bindings,
                function,
                alias,
                &AstPath(vec![PathSegment::Stmt(index)]),
            )?;
            aliases.insert(alias.to_string());
            alias_bindings.push(binding);
            index += 1;
            continue;
        }
        if free_call_on_any(&indent.stmt, &aliases) {
            if free.is_some() {
                return None;
            }
            let free_temp = free_temp_before(function, body, bindings, index, pointer_name);
            uses.push(HeapUseFact {
                path: AstPath(vec![PathSegment::Stmt(index)]),
                kind: HeapUseKind::Free,
            });
            free = Some((index, free_temp));
            index += 1;
            continue;
        }
        if let Some(use_kind) = heap_use(&indent.stmt, &aliases) {
            if free.is_some() {
                return None;
            }
            match &use_kind {
                HeapUseKind::ScalarWrite => {
                    written.insert(None);
                }
                HeapUseKind::IndexedWrite { index } => {
                    if let Some(value) = int_value(index) {
                        written.insert(Some(value));
                    }
                }
                HeapUseKind::ScalarRead => {
                    if candidate.init == HeapInitKind::Uninitialized && !written.contains(&None) {
                        may_read_uninit = true;
                    }
                }
                HeapUseKind::IndexedRead { index } => {
                    if candidate.init == HeapInitKind::Uninitialized
                        && int_value(index).is_none_or(|value| !written.contains(&Some(value)))
                    {
                        may_read_uninit = true;
                    }
                }
                HeapUseKind::Free => {}
            }
            uses.push(HeapUseFact {
                path: AstPath(vec![PathSegment::Stmt(index)]),
                kind: use_kind,
            });
            index += 1;
            continue;
        }
        if reassigns_pointer(&indent.stmt, pointer_name)
            || stmt_mentions_any_pointer(&indent.stmt, &aliases)
        {
            return None;
        }
        index += 1;
    }
    free.as_ref()?;
    let (free_index, free_temp) = match free {
        Some((index, temp)) => (Some(index), temp),
        None => (None, None),
    };
    let read_safety = if candidate.init == HeapInitKind::Zeroed {
        HeapReadSafety::ZeroInitialized
    } else if may_read_uninit {
        HeapReadSafety::MayReadUninitialized
    } else {
        HeapReadSafety::ReadsAfterWrites
    };
    Some((
        free_index,
        free_temp,
        alias_bindings,
        uses,
        reallocations,
        read_safety,
    ))
}

fn pointer_alias_temp_from_any<'a>(stmt: &'a Stmt, names: &BTreeSet<String>) -> Option<&'a str> {
    let Stmt::Let {
        name,
        init: Some(Expr::Var(source)),
        ..
    } = stmt
    else {
        return None;
    };
    names.contains(source.as_str()).then_some(name.as_str())
}

fn free_temp_before(
    function: FunctionId,
    body: &[IndentStmt],
    bindings: &[BindingFact],
    index: usize,
    pointer_name: &str,
) -> Option<BindingId> {
    let prev_index = index.checked_sub(1)?;
    let alias = pointer_alias_temp(&body.get(prev_index)?.stmt, pointer_name)?;
    facts::binding_by_local_path(
        bindings,
        function,
        alias,
        &AstPath(vec![PathSegment::Stmt(prev_index)]),
    )
}

struct ReallocAt {
    fact: HeapReallocFact,
    next_index: usize,
}

fn realloc_at(
    function: FunctionId,
    body: &[IndentStmt],
    bindings: &[BindingFact],
    index: usize,
    pointer_name: &str,
    aliases: &BTreeSet<String>,
    candidate: &Candidate,
) -> Option<ReallocAt> {
    let source_name = pointer_alias_temp_from_any(&body.get(index)?.stmt, aliases)?;
    let size_index = index + 1;
    let realloc_index = index + 2;
    let assign_index = index + 3;
    let size_stmt = &body.get(size_index)?.stmt;
    let Stmt::Let {
        name: size_name,
        init: Some(size_expr),
        ..
    } = size_stmt
    else {
        return None;
    };
    let Stmt::Let {
        name: allocation_name,
        init: Some(realloc_expr),
        ..
    } = &body.get(realloc_index)?.stmt
    else {
        return None;
    };
    if !realloc_call_on_source(realloc_expr, source_name, size_name) {
        return None;
    }
    if !assigns_allocated_pointer(&body.get(assign_index)?.stmt, pointer_name, allocation_name) {
        return None;
    }
    let source_temp = facts::binding_by_local_path(
        bindings,
        function,
        source_name,
        &AstPath(vec![PathSegment::Stmt(index)]),
    );
    let allocation_temp = facts::binding_by_local_path(
        bindings,
        function,
        allocation_name,
        &AstPath(vec![PathSegment::Stmt(realloc_index)]),
    )?;
    let size_temp = facts::binding_by_local_path(
        bindings,
        function,
        size_name,
        &AstPath(vec![PathSegment::Stmt(size_index)]),
    );
    let new_extent = extent_from_malloc_size(size_expr, &candidate.elem_ty);
    let resize = resize_kind(&candidate.extent, &new_extent);
    Some(ReallocAt {
        fact: HeapReallocFact {
            source_temp,
            allocation_temp,
            size_temp,
            allocation_path: AstPath(vec![PathSegment::Stmt(realloc_index)]),
            assign_path: AstPath(vec![PathSegment::Stmt(assign_index)]),
            new_extent,
            init: HeapInitKind::Uninitialized,
            resize,
        },
        next_index: assign_index + 1,
    })
}

fn resize_kind(old: &HeapExtent, new: &HeapExtent) -> HeapResizeKind {
    match (extent_count(old), extent_count(new)) {
        (Some(old), Some(new)) if new > old => HeapResizeKind::Grow,
        (Some(old), Some(new)) if new < old => HeapResizeKind::Shrink,
        _ => HeapResizeKind::SameOrUnknown,
    }
}

fn extent_count(extent: &HeapExtent) -> Option<i64> {
    match extent {
        HeapExtent::Scalar => Some(1),
        HeapExtent::Elements { count } => int_value(count),
        HeapExtent::Unknown => None,
    }
}

fn realloc_call_on_source(expr: &Expr, source_name: &str, size_name: &str) -> bool {
    let Expr::Unsafe(block) = expr else {
        return false;
    };
    let Some(call @ Expr::Call { args, .. }) = block.tail.as_deref() else {
        return false;
    };
    known_call(call) == Some(Known::Realloc)
        && args.len() == 2
        && cast_source_var(&args[0]) == Some(source_name)
        && size_arg_name(&args[1]) == Some(size_name)
}

fn heap_use(stmt: &Stmt, names: &BTreeSet<String>) -> Option<HeapUseKind> {
    match stmt {
        Stmt::Unsafe { body } if body.tail.is_none() && body.stmts.len() == 1 => {
            heap_use(&body.stmts[0].stmt, names)
        }
        Stmt::Assign { target, .. } => place_heap_use(target, names).map(|index| match index {
            Some(index) => HeapUseKind::IndexedWrite { index },
            None => HeapUseKind::ScalarWrite,
        }),
        Stmt::Let {
            init: Some(init), ..
        } => value_heap_use(init, names).map(|index| match index {
            Some(index) => HeapUseKind::IndexedRead { index },
            None => HeapUseKind::ScalarRead,
        }),
        _ => None,
    }
}

fn place_heap_use(expr: &Expr, names: &BTreeSet<String>) -> Option<Option<Expr>> {
    let Expr::Unary {
        op: UnaryOp::Deref,
        expr,
    } = expr
    else {
        return None;
    };
    pointer_expr_index(expr, names)
}

fn value_heap_use(expr: &Expr, names: &BTreeSet<String>) -> Option<Option<Expr>> {
    let Expr::Unsafe(block) = expr else {
        return None;
    };
    let Expr::Unary {
        op: UnaryOp::Deref,
        expr,
    } = block.tail.as_deref()?
    else {
        return None;
    };
    pointer_expr_index(expr, names)
}

fn pointer_expr_index(expr: &Expr, names: &BTreeSet<String>) -> Option<Option<Expr>> {
    match strip_unsafe(expr) {
        Expr::Var(name) if names.contains(name.as_str()) => Some(None),
        Expr::MethodCall { recv, method, args }
            if method == "add" && args.len() == 1 && base_is_owned(recv, names) =>
        {
            Some(Some(strip_casts(&args[0]).clone()))
        }
        _ => None,
    }
}

fn strip_unsafe(expr: &Expr) -> &Expr {
    match expr {
        Expr::Unsafe(block) if block.stmts.is_empty() => {
            block.tail.as_deref().map(strip_unsafe).unwrap_or(expr)
        }
        _ => expr,
    }
}

fn base_is_owned(expr: &Expr, names: &BTreeSet<String>) -> bool {
    matches!(strip_unsafe(expr), Expr::Var(name) if names.contains(name.as_str()))
}

fn reassigns_pointer(stmt: &Stmt, pointer_name: &str) -> bool {
    matches!(
        stmt,
        Stmt::Assign {
            target: Expr::Var(name),
            ..
        } if name.as_str() == pointer_name
    )
}

fn stmt_mentions_any_pointer(stmt: &Stmt, names: &BTreeSet<String>) -> bool {
    let mut escaped = false;
    walk::stmt_exprs(stmt, &mut |expr| {
        if escaped {
            return;
        }
        match expr {
            Expr::Call { args, .. } => {
                if known_call(expr) == Some(Known::Free) {
                    return;
                }
                if args.iter().any(|arg| expr_mentions_any(arg, names)) {
                    escaped = true;
                }
            }
            Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
                if expr_mentions_any(recv, names)
                    || args.iter().any(|arg| expr_mentions_any(arg, names))
                {
                    escaped = true;
                }
            }
            Expr::AddrOf { expr, .. }
            | Expr::Ref { expr, .. }
            | Expr::ArrayPtr { array: expr, .. }
                if expr_mentions_any(expr, names) =>
            {
                escaped = true;
            }
            _ => {}
        }
    });
    escaped
}

fn expr_mentions_any(expr: &Expr, names: &BTreeSet<String>) -> bool {
    walk::exprs_any(
        expr,
        &mut |expr| matches!(expr, Expr::Var(name) if names.contains(name.as_str())),
    )
}
