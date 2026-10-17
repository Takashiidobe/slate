use super::{inline_temps, walk};
use crate::backend::engine::NodeRule;
use crate::backend::engine::arena::{Arena, FunctionOptimizer, NodeId, NodeKind, NodeKindTag};
use crate::backend::rust_ast::{
    BinOp, Block, Expr, Ident, Path, Prim, RustValue, Stmt, Type, UnaryOp,
};
use crate::function_identity::{CallBinding, Known};

pub(super) struct CallCtx<'a> {
    arena: &'a FunctionOptimizer,
    args: &'a [Expr],
    result_type: Option<Type>,
    discards_result: bool,
}

impl CallCtx<'_> {
    pub(super) fn args(&self) -> &[Expr] {
        self.args
    }

    pub(super) fn discards_result(&self) -> bool {
        self.discards_result
    }

    pub(super) fn result_type(&self) -> Option<&Type> {
        self.result_type.as_ref()
    }

    pub(super) fn lifted_arg(&self, i: usize, want: fn(&Type) -> bool) -> Option<Expr> {
        self.resolve_lifted(self.args.get(i)?, want, 0)
    }

    pub(super) fn const_str_arg(&self, i: usize) -> Option<String> {
        self.resolve_const_str(self.args.get(i)?, 0)
    }

    fn resolve_const_str(&self, expr: &Expr, depth: usize) -> Option<String> {
        if depth > 8 {
            return None;
        }
        let peeled = peel_ptr_view(expr);
        if let Some(literal) = const_str_literal(peeled) {
            return Some(literal);
        }
        let Expr::Var(name) = peeled else {
            return None;
        };
        let ty = self.var_type(*name)?;
        if is_raw_ptr(&ty) && !self.is_reassigned(*name) {
            return self.resolve_const_str(&self.var_init(*name)?, depth + 1);
        }
        None
    }

    fn resolve_lifted(&self, expr: &Expr, want: fn(&Type) -> bool, depth: usize) -> Option<Expr> {
        if depth > 8 {
            return None;
        }
        let peeled = peel_ptr_view(expr);
        if let Some(literal) = const_str_literal(peeled)
            && want(&ref_str())
        {
            return Some(Expr::Str(literal));
        }
        let Expr::Var(name) = peeled else {
            return None;
        };
        let ty = self.var_type(*name)?;
        if want(&ty) {
            return Some(Expr::Var(*name));
        }
        if is_raw_ptr(&ty) && !self.is_reassigned(*name) {
            return self.resolve_lifted(&self.var_init(*name)?, want, depth + 1);
        }
        None
    }

    fn var_type(&self, name: Ident) -> Option<Type> {
        if let Some(ty) = self.arena.param_type(name) {
            return Some(ty.clone());
        }
        match self.arena.get(self.arena.definition(name)?)? {
            NodeKind::Let { ty: Some(ty), .. } => Some(ty.clone()),
            _ => None,
        }
    }

    fn resolve_local_array(&self, expr: &Expr, depth: usize) -> Option<(Ident, Type, u64)> {
        if depth > 8 {
            return None;
        }
        let peeled = peel_ptr_view(expr);
        let Expr::Var(name) = peeled else {
            return None;
        };
        let ty = self.var_type(*name)?;
        if let Type::Array { elem, len } = unwrap_aligned(&ty) {
            return is_byte_elem(elem).then(|| (*name, (**elem).clone(), *len));
        }
        if is_raw_ptr(&ty) && !self.is_reassigned(*name) {
            return self.resolve_local_array(&self.var_init(*name)?, depth + 1);
        }
        None
    }

    pub(super) fn lifted_local_array(&self, i: usize) -> Option<(Ident, Type, u64)> {
        self.resolve_local_array(self.args.get(i)?, 0)
    }

    fn var_init(&self, name: Ident) -> Option<Expr> {
        match self.arena.get(self.arena.definition(name)?)? {
            NodeKind::Let {
                init: Some(init), ..
            } => Some(init.clone()),
            _ => None,
        }
    }

    fn is_reassigned(&self, name: Ident) -> bool {
        self.arena
            .def_use_neighbors(name)
            .iter()
            .any(|&id| match self.arena.get(id) {
                Some(NodeKind::Assign { target, .. })
                | Some(NodeKind::CompoundAssign { target, .. }) => {
                    assign_root_var(target) == Some(name)
                }
                _ => false,
            })
    }
}

fn peel_wrapper(expr: &Expr, through_ptr_methods: bool) -> &Expr {
    match expr {
        Expr::Cast { expr, .. } => peel_wrapper(expr, through_ptr_methods),
        Expr::MethodCall { recv, method, args }
            if through_ptr_methods
                && args.is_empty()
                && (method == "as_ptr" || method == "as_mut_ptr") =>
        {
            peel_wrapper(recv, through_ptr_methods)
        }
        Expr::Unsafe(block) | Expr::Block(block)
            if block.stmts.is_empty() && block.tail.is_some() =>
        {
            peel_wrapper(block.tail.as_deref().unwrap(), through_ptr_methods)
        }
        other => other,
    }
}

fn peel_ptr_view(expr: &Expr) -> &Expr {
    peel_wrapper(expr, true)
}

fn assign_root_var(expr: &Expr) -> Option<Ident> {
    match expr {
        Expr::Var(name) => Some(*name),
        Expr::Unary { expr, .. }
        | Expr::Field { base: expr, .. }
        | Expr::Index { base: expr, .. } => assign_root_var(expr),
        _ => None,
    }
}

fn is_raw_ptr(ty: &Type) -> bool {
    matches!(ty, Type::Ptr { .. })
}

fn unwrap_aligned(ty: &Type) -> &Type {
    match ty {
        Type::Generic { name, args } if name == "aligned::Aligned" && args.len() == 2 => &args[1],
        other => other,
    }
}

fn is_byte_elem(ty: &Type) -> bool {
    matches!(ty, Type::Prim(Prim::U8 | Prim::I8))
}

fn const_len(expr: &Expr) -> Option<u64> {
    match expr {
        Expr::Cast { expr, .. } => const_len(expr),
        Expr::Value(RustValue::I64(n)) => u64::try_from(*n).ok(),
        Expr::Value(RustValue::Usize(n)) => Some(*n as u64),
        Expr::Value(RustValue::I128(n)) => u64::try_from(*n).ok(),
        Expr::Value(RustValue::U128(n)) => u64::try_from(*n).ok(),
        Expr::Value(RustValue::TypedInt(n, _)) => u64::try_from(*n).ok(),
        Expr::Value(RustValue::TypedUInt(n, _)) => u64::try_from(*n).ok(),
        _ => None,
    }
}

fn ref_str() -> Type {
    Type::Ref {
        mutable: false,
        inner: Box::new(Type::Str),
    }
}

fn const_str_literal(expr: &Expr) -> Option<String> {
    match expr {
        Expr::Str(s) => Some(s.clone()),
        Expr::CStr(bytes) => String::from_utf8(bytes.clone()).ok(),
        Expr::ByteStr(bytes) => {
            String::from_utf8(bytes.strip_suffix(&[0]).unwrap_or(bytes).to_vec()).ok()
        }
        _ => None,
    }
}

fn is_u8(ty: &Type) -> bool {
    matches!(ty, Type::Prim(Prim::U8))
}

fn is_u8_slice(ty: &Type) -> bool {
    match ty {
        Type::Slice(inner) => is_u8(inner),
        Type::Ref { inner, .. } => is_u8_slice(inner),
        _ => false,
    }
}

fn is_string(ty: &Type) -> bool {
    match ty {
        Type::Str => true,
        Type::Ref { inner, .. } => matches!(**inner, Type::Str),
        Type::Generic { name, .. } => name == "String",
        _ => false,
    }
}

fn method_call(recv: Expr, method: &str, args: Vec<Expr>) -> Expr {
    Expr::MethodCall {
        recv: Box::new(recv),
        method: method.to_string(),
        args,
    }
}

fn cast(expr: Expr, ty: Type) -> Expr {
    Expr::Cast {
        expr: Box::new(expr),
        ty,
    }
}

fn byte_ptr(mutable: bool) -> Type {
    Type::Ptr {
        mutable,
        inner: Box::new(Type::Prim(Prim::U8)),
    }
}

fn block_then(effect: Expr, value: Expr) -> Expr {
    Expr::Block(Box::new(Block {
        stmts: vec![Stmt::Expr(effect)],
        tail: Some(Box::new(value)),
    }))
}

fn returning_dst(ctx: &CallCtx, effect: Expr, dst: &Expr) -> Expr {
    if ctx.discards_result() {
        effect
    } else {
        block_then(effect, dst.clone())
    }
}

fn mem_copy(ctx: &CallCtx, overlapping: bool) -> Option<Expr> {
    let [dst, src, len] = ctx.args() else {
        return None;
    };
    let effect = Expr::PtrCopy {
        src: Box::new(cast(src.clone(), byte_ptr(false))),
        dst: Box::new(cast(dst.clone(), byte_ptr(true))),
        count: Box::new(cast(len.clone(), Type::Prim(Prim::Usize))),
        overlapping,
    };
    Some(returning_dst(ctx, effect, dst))
}

fn mem_set(ctx: &CallCtx) -> Option<Expr> {
    let [dst, val, len] = ctx.args() else {
        return None;
    };
    let effect = Expr::WriteBytes {
        dst: Box::new(cast(dst.clone(), byte_ptr(true))),
        val: Box::new(cast(val.clone(), Type::Prim(Prim::U8))),
        count: Box::new(cast(len.clone(), Type::Prim(Prim::Usize))),
    };
    Some(returning_dst(ctx, effect, dst))
}

fn usize_lit(n: u64) -> Expr {
    Expr::Value(RustValue::Usize(n as usize))
}

fn slice_upto(base: Expr, n: u64) -> Expr {
    Expr::Index {
        base: Box::new(base),
        index: Box::new(Expr::Range {
            start: Box::new(usize_lit(0)),
            end: Box::new(usize_lit(n)),
        }),
    }
}

fn range_upto(n: u64) -> Expr {
    Expr::Range {
        start: Box::new(usize_lit(0)),
        end: Box::new(usize_lit(n)),
    }
}

fn safe_mem_copy_like(ctx: &CallCtx, allow_self: bool) -> Option<Expr> {
    let [dst, _src, len] = ctx.args() else {
        return None;
    };
    let n = const_len(len)?;
    let (dst_name, dst_elem, dst_len) = ctx.lifted_local_array(0)?;
    let (src_name, src_elem, src_len) = ctx.lifted_local_array(1)?;
    if dst_elem != src_elem || n > dst_len || n > src_len {
        return None;
    }
    let effect = if dst_name == src_name {
        if !allow_self {
            return None;
        }
        method_call(
            Expr::Var(dst_name),
            "copy_within",
            vec![range_upto(n), usize_lit(0)],
        )
    } else {
        method_call(
            slice_upto(Expr::Var(dst_name), n),
            "copy_from_slice",
            vec![Expr::Ref {
                mutable: false,
                expr: Box::new(slice_upto(Expr::Var(src_name), n)),
            }],
        )
    };
    Some(returning_dst(ctx, effect, dst))
}

fn safe_mem_set(ctx: &CallCtx) -> Option<Expr> {
    let [dst, val, len] = ctx.args() else {
        return None;
    };
    let n = const_len(len)?;
    let (dst_name, dst_elem, dst_len) = ctx.lifted_local_array(0)?;
    if n > dst_len {
        return None;
    }
    let effect = method_call(
        slice_upto(Expr::Var(dst_name), n),
        "fill",
        vec![cast(val.clone(), dst_elem)],
    );
    Some(returning_dst(ctx, effect, dst))
}

fn slice_to(expr: Expr, len: Expr) -> Expr {
    Expr::Index {
        base: Box::new(expr),
        index: Box::new(Expr::Range {
            start: Box::new(Expr::Value(RustValue::Usize(0))),
            end: Box::new(match len {
                Expr::Value(RustValue::Usize(_)) => len,
                other => cast(other, Type::Prim(Prim::Usize)),
            }),
        }),
    }
}

fn lifted_bytes(ctx: &CallCtx, i: usize) -> Option<Expr> {
    ctx.lifted_arg(i, is_string)
        .map(|arg| method_call(arg, "as_bytes", Vec::new()))
        .or_else(|| ctx.lifted_arg(i, is_u8_slice))
}

fn literal_len(bytes: &Expr) -> Option<usize> {
    match bytes {
        Expr::MethodCall { recv, method, args }
            if method.as_str() == "as_bytes" && args.is_empty() =>
        {
            match recv.as_ref() {
                Expr::Str(text) if !text.contains('\0') => Some(text.len()),
                _ => None,
            }
        }
        _ => None,
    }
}

fn const_bound(bound: &Expr) -> Option<usize> {
    usize::try_from(super::for_range::int_value(bound)?).ok()
}

fn c_string_bytes(bytes: Expr, bound: Option<Expr>) -> Expr {
    if literal_len(&bytes).is_some_and(|len| {
        bound
            .as_ref()
            .is_none_or(|bound| const_bound(bound).is_some_and(|k| k >= len))
    }) {
        return bytes;
    }
    slice_to(bytes.clone(), c_string_len(bytes, bound))
}

fn c_string_len(bytes: Expr, bound: Option<Expr>) -> Expr {
    if let Some(len) = literal_len(&bytes) {
        return match bound {
            None => Expr::Value(RustValue::Usize(len)),
            Some(bound) => match const_bound(&bound) {
                Some(k) => Expr::Value(RustValue::Usize(k.min(len))),
                None => method_call(
                    cast(bound, Type::Prim(Prim::Usize)),
                    "min",
                    vec![Expr::Value(RustValue::Usize(len))],
                ),
            },
        };
    }
    let byte = Ident::new("__slate_byte");
    let is_nul = Expr::Binary {
        op: BinOp::Eq,
        lhs: Box::new(Expr::Unary {
            op: UnaryOp::Deref,
            expr: Box::new(Expr::Var(byte)),
        }),
        rhs: Box::new(Expr::Value(RustValue::TypedUInt(0, Prim::U8))),
    };
    let position = method_call(
        method_call(bytes.clone(), "iter", Vec::new()),
        "position",
        vec![Expr::Closure {
            params: vec![byte],
            body: Box::new(is_nul),
        }],
    );
    let len = method_call(
        position,
        "unwrap_or",
        vec![method_call(bytes, "len", Vec::new())],
    );
    match bound {
        Some(bound) => method_call(len, "min", vec![cast(bound, Type::Prim(Prim::Usize))]),
        None => len,
    }
}

fn str_n_cmp(ctx: &CallCtx) -> Option<Expr> {
    let a = lifted_bytes(ctx, 0)?;
    let b = lifted_bytes(ctx, 1)?;
    let bound = ctx.args().get(2)?.clone();
    let lhs = c_string_bytes(a, Some(bound.clone()));
    let rhs = Expr::Ref {
        mutable: false,
        expr: Box::new(c_string_bytes(b, Some(bound))),
    };
    Some(cast(
        method_call(lhs, "cmp", vec![rhs]),
        Type::Prim(Prim::I32),
    ))
}

fn size_result(ctx: &CallCtx, length: Expr) -> Option<Expr> {
    match ctx.result_type() {
        Some(ty) if *ty == Type::Prim(Prim::Usize) => Some(length),
        Some(ty) => Some(cast(length, ty.clone())),
        None if ctx.discards_result() => Some(length),
        None => None,
    }
}

fn str_n_len(ctx: &CallCtx) -> Option<Expr> {
    let bytes = lifted_bytes(ctx, 0)?;
    size_result(ctx, c_string_len(bytes, Some(ctx.args().get(1)?.clone())))
}

fn str_span(ctx: &CallCtx, accepted: bool) -> Option<Expr> {
    let input = lifted_bytes(ctx, 0)?;
    let set = lifted_bytes(ctx, 1)?;
    let input = c_string_bytes(input, None);
    let set = c_string_bytes(set, None);
    let byte = Ident::new("__slate_byte");
    let contains = method_call(set, "contains", vec![Expr::Var(byte)]);
    let stop = if accepted {
        Expr::Unary {
            op: UnaryOp::Not,
            expr: Box::new(contains),
        }
    } else {
        contains
    };
    let position = method_call(
        method_call(input.clone(), "iter", Vec::new()),
        "position",
        vec![Expr::Closure {
            params: vec![byte],
            body: Box::new(stop),
        }],
    );
    size_result(
        ctx,
        method_call(
            position,
            "unwrap_or",
            vec![method_call(input, "len", Vec::new())],
        ),
    )
}

fn mem_cmp(ctx: &CallCtx) -> Option<Expr> {
    let a = ctx.lifted_arg(0, is_u8_slice)?;
    let b = ctx.lifted_arg(1, is_u8_slice)?;
    let len = ctx.args().get(2)?.clone();
    let lhs = slice_to(a, len.clone());
    let rhs = Expr::Ref {
        mutable: false,
        expr: Box::new(slice_to(b, len)),
    };
    Some(cast(
        method_call(lhs, "cmp", vec![rhs]),
        Type::Prim(Prim::I32),
    ))
}

fn c_atoi_prefix(s: &str) -> i128 {
    let t = s.trim_start();
    let b = t.as_bytes();
    let mut end = 0;
    if end < b.len() && matches!(b[end], b'+' | b'-') {
        end += 1;
    }
    let digits_start = end;
    while end < b.len() && b[end].is_ascii_digit() {
        end += 1;
    }
    if end == digits_start {
        return 0;
    }
    t[..end].parse::<i128>().unwrap_or(0)
}

fn saturate_int(v: i128, prim: Prim) -> i128 {
    match prim {
        Prim::I32 => v.clamp(i32::MIN as i128, i32::MAX as i128),
        Prim::I64 => v.clamp(i64::MIN as i128, i64::MAX as i128),
        _ => v,
    }
}

fn fold_atoi(ctx: &CallCtx, prim: Prim) -> Option<Expr> {
    let lit = ctx.const_str_arg(0)?;
    let value = saturate_int(c_atoi_prefix(&lit), prim);
    Some(Expr::Value(RustValue::TypedInt(value, prim)))
}

fn fold_atof(ctx: &CallCtx) -> Option<Expr> {
    let lit = ctx.const_str_arg(0)?;
    let value = lit.trim().parse::<f64>().ok().filter(|v| v.is_finite())?;
    Some(Expr::Value(RustValue::from(value)))
}

fn const_char_ptr(char_prim: Prim) -> Type {
    Type::Ptr {
        mutable: false,
        inner: Box::new(Type::Prim(char_prim)),
    }
}

fn ato_helper(ctx: &CallCtx, name: &str, char_prim: Prim) -> Option<Expr> {
    let arg = ctx.args().first()?.clone();
    let arg = match &arg {
        Expr::Cast { ty, .. } if *ty == const_char_ptr(char_prim) => arg,
        _ => cast(arg, const_char_ptr(char_prim)),
    };
    Some(Expr::Call {
        binding: CallBinding::Generated,
        func: Box::new(Expr::Var(name.into())),
        args: vec![arg],
    })
}

fn free_call(path: &[&str], args: Vec<Expr>) -> Expr {
    Expr::Call {
        binding: CallBinding::Generated,
        func: Box::new(Expr::Path(Path::new(path.iter().copied().map(Ident::from)))),
        args,
    }
}

type Build = Box<dyn Fn(&CallCtx) -> Option<Expr>>;

pub(super) struct LibcCall {
    known: Known,
    build: Build,
}

pub(super) fn libc_call(
    known: Known,
    build: impl Fn(&CallCtx) -> Option<Expr> + 'static,
) -> Box<dyn NodeRule> {
    Box::new(LibcCall {
        known,
        build: Box::new(build),
    })
}

fn call_slot(kind: &NodeKind) -> Option<&Expr> {
    match kind {
        NodeKind::Expr(expr)
        | NodeKind::Return(Some(expr))
        | NodeKind::Assign { value: expr, .. }
        | NodeKind::CompoundAssign { value: expr, .. } => Some(expr),
        NodeKind::Let {
            init: Some(expr), ..
        } => Some(expr),
        _ => None,
    }
}

fn call_slot_mut(kind: &mut NodeKind) -> Option<&mut Expr> {
    match kind {
        NodeKind::Expr(expr)
        | NodeKind::Return(Some(expr))
        | NodeKind::Assign { value: expr, .. }
        | NodeKind::CompoundAssign { value: expr, .. } => Some(expr),
        NodeKind::Let {
            init: Some(expr), ..
        } => Some(expr),
        _ => None,
    }
}

fn peel_call_head(expr: &Expr) -> &Expr {
    peel_wrapper(expr, false)
}

enum PeelStep {
    Cast,
    Wrap,
    Stop,
}

fn peel_call_head_mut(expr: &mut Expr) -> &mut Expr {
    let step = match &*expr {
        Expr::Cast { .. } => PeelStep::Cast,
        Expr::Unsafe(block) | Expr::Block(block)
            if block.stmts.is_empty() && block.tail.is_some() =>
        {
            PeelStep::Wrap
        }
        _ => PeelStep::Stop,
    };
    match step {
        PeelStep::Cast => {
            let Expr::Cast { expr, .. } = expr else {
                unreachable!()
            };
            peel_call_head_mut(expr)
        }
        PeelStep::Wrap => {
            let (Expr::Unsafe(block) | Expr::Block(block)) = expr else {
                unreachable!()
            };
            peel_call_head_mut(block.tail.as_deref_mut().unwrap())
        }
        PeelStep::Stop => expr,
    }
}

fn known_call_args(expr: &Expr, known: Known) -> Option<&[Expr]> {
    match peel_call_head(expr) {
        Expr::Call { binding, args, .. } if binding.known() == Some(known) => Some(args),
        _ => None,
    }
}

fn nth_expr_mut<'a>(
    expr: &'a mut Expr,
    target: usize,
    counter: &mut usize,
) -> Option<&'a mut Expr> {
    if *counter == target {
        return Some(expr);
    }
    *counter += 1;
    for child in walk::child_exprs_mut(expr) {
        if let Some(found) = nth_expr_mut(child, target, counter) {
            return Some(found);
        }
    }
    None
}

impl LibcCall {
    fn site_replacement(&self, arena: &FunctionOptimizer, expr: &Expr) -> Option<Expr> {
        let args = known_call_args(expr, self.known)?;
        if args
            .iter()
            .any(|arg| inline_temps::expr_effects(arg).is_side_effect())
        {
            return None;
        }
        let result_type = match expr {
            Expr::Cast { ty, .. } => Some(ty.clone()),
            _ => None,
        };
        (self.build)(&CallCtx {
            arena,
            args,
            result_type,
            discards_result: false,
        })
    }

    fn search(
        &self,
        arena: &FunctionOptimizer,
        expr: &Expr,
        skip: Option<&Expr>,
        counter: &mut usize,
    ) -> Option<(usize, Expr)> {
        let ordinal = *counter;
        *counter += 1;
        if !skip.is_some_and(|skip| std::ptr::eq(skip, expr))
            && let Some(replacement) = self.site_replacement(arena, expr)
        {
            return Some((ordinal, replacement));
        }
        walk::child_exprs(expr)
            .into_iter()
            .find_map(|child| self.search(arena, child, skip, counter))
    }

    fn nested_site(&self, arena: &FunctionOptimizer, kind: &NodeKind) -> Option<(usize, Expr)> {
        let skip = call_slot(kind);
        let mut counter = 0;
        let mut found = None;
        walk::visit_kind_exprs(kind, |expr| {
            if found.is_none() {
                found = self.search(arena, expr, skip, &mut counter);
            }
        });
        found
    }
}

impl NodeRule for LibcCall {
    fn name(&self) -> &'static str {
        self.known.symbol()
    }

    fn priority(&self) -> u32 {
        59
    }

    fn kinds(&self) -> &'static [NodeKindTag] {
        &[
            NodeKindTag::Expr,
            NodeKindTag::Let,
            NodeKindTag::Return,
            NodeKindTag::Assign,
            NodeKindTag::CompoundAssign,
            NodeKindTag::If,
            NodeKindTag::While,
            NodeKindTag::Match,
        ]
    }

    fn call_anchor(&self) -> Option<Ident> {
        Some(Ident::new(self.known.symbol()))
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        let Some(kind) = arena.get(id) else {
            return false;
        };
        if let Some(slot) = call_slot(kind)
            && let Some(args) = known_call_args(slot, self.known)
        {
            let ctx = CallCtx {
                arena,
                args,
                result_type: call_result_type(kind),
                discards_result: node_discards_result(arena, kind),
            };
            return (self.build)(&ctx).is_some();
        }
        self.nested_site(arena, kind).is_some()
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        let Some(kind) = arena.get(id) else {
            return false;
        };
        let slot_args = call_slot(kind).and_then(|slot| known_call_args(slot, self.known));
        let Some(args) = slot_args else {
            let Some((ordinal, replacement)) = self.nested_site(arena, kind) else {
                return false;
            };
            let Some(kind) = arena.get_mut(id) else {
                return false;
            };
            let mut counter = 0;
            let mut replacement = Some(replacement);
            walk::visit_kind_exprs_mut(kind, |expr| {
                if replacement.is_some()
                    && let Some(site) = nth_expr_mut(expr, ordinal, &mut counter)
                    && let Some(replacement) = replacement.take()
                {
                    *peel_call_head_mut(site) = replacement;
                }
            });
            if replacement.is_some() {
                return false;
            }
            arena.touch(id);
            return true;
        };
        let args = args.to_vec();
        let result_type = call_result_type(kind);
        let discards_result = node_discards_result(arena, kind);
        let ctx = CallCtx {
            arena,
            args: &args,
            result_type,
            discards_result,
        };
        let Some(replacement) = (self.build)(&ctx) else {
            return false;
        };
        let Some(kind) = arena.get_mut(id) else {
            return false;
        };
        let Some(slot) = call_slot_mut(kind) else {
            return false;
        };
        *peel_call_head_mut(slot) = replacement;
        if let NodeKind::Let {
            init: Some(init), ..
        } = kind
            && discards_result
        {
            let init = std::mem::replace(init, Expr::Value(RustValue::I64(0)));
            arena.set_kind(id, NodeKind::Expr(init));
        }
        true
    }
}

fn call_result_type(kind: &NodeKind) -> Option<Type> {
    match kind {
        NodeKind::Let { ty, .. } => ty.clone(),
        _ => None,
    }
}

fn node_discards_result(arena: &Arena, kind: &NodeKind) -> bool {
    match kind {
        NodeKind::Expr(_) => true,
        NodeKind::Let { name, .. } => arena.def_use_neighbors(*name).is_empty(),
        _ => false,
    }
}

pub(super) fn rules(target: &slate_parser::target_info::TargetInfo) -> Vec<Box<dyn NodeRule>> {
    let char_prim = crate::backend::engine::prelude::char_prim(target);
    let long_prim = crate::backend::engine::prelude::long_prim(target);
    vec![
        libc_call(Known::Exit, |ctx| {
            Some(free_call(&["std", "process", "exit"], ctx.args().to_vec()))
        }),
        libc_call(Known::Abort, |ctx| {
            Some(free_call(&["std", "process", "abort"], ctx.args().to_vec()))
        }),
        libc_call(Known::StrLen, |ctx| {
            let length = match ctx.lifted_arg(0, is_string) {
                Some(text) => method_call(text, "len", Vec::new()),
                None => c_string_len(ctx.lifted_arg(0, is_u8_slice)?, None),
            };
            size_result(ctx, length)
        }),
        libc_call(Known::StrCmp, |ctx| {
            if let (Some(a), Some(b)) = (ctx.lifted_arg(0, is_string), ctx.lifted_arg(1, is_string))
            {
                return Some(cast(method_call(a, "cmp", vec![b]), Type::Prim(Prim::I32)));
            }
            let a = lifted_bytes(ctx, 0)?;
            let b = lifted_bytes(ctx, 1)?;
            let lhs = c_string_bytes(a, None);
            let rhs = Expr::Ref {
                mutable: false,
                expr: Box::new(c_string_bytes(b, None)),
            };
            Some(cast(
                method_call(lhs, "cmp", vec![rhs]),
                Type::Prim(Prim::I32),
            ))
        }),
        libc_call(Known::StrNCmp, str_n_cmp),
        libc_call(Known::StrNLen, str_n_len),
        libc_call(Known::StrSpn, |ctx| str_span(ctx, true)),
        libc_call(Known::StrCSpn, |ctx| str_span(ctx, false)),
        libc_call(Known::Atoi, move |ctx| {
            fold_atoi(ctx, Prim::I32).or_else(|| ato_helper(ctx, "__slate_atoi", char_prim))
        }),
        libc_call(Known::Atol, move |ctx| {
            fold_atoi(ctx, long_prim).or_else(|| ato_helper(ctx, "__slate_atol", char_prim))
        }),
        libc_call(Known::Atoll, move |ctx| {
            fold_atoi(ctx, Prim::I64).or_else(|| ato_helper(ctx, "__slate_atoll", char_prim))
        }),
        libc_call(Known::Atof, fold_atof),
        libc_call(Known::MemCpy, |ctx| {
            safe_mem_copy_like(ctx, false).or_else(|| mem_copy(ctx, false))
        }),
        libc_call(Known::MemMove, |ctx| {
            safe_mem_copy_like(ctx, true).or_else(|| mem_copy(ctx, true))
        }),
        libc_call(Known::MemSet, |ctx| {
            safe_mem_set(ctx).or_else(|| mem_set(ctx))
        }),
        libc_call(Known::MemCmp, mem_cmp),
    ]
}
