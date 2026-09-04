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

fn peel_ptr_view(expr: &Expr) -> &Expr {
    match expr {
        Expr::Cast { expr, .. } => peel_ptr_view(expr),
        Expr::MethodCall { recv, method, args }
            if args.is_empty() && (method == "as_ptr" || method == "as_mut_ptr") =>
        {
            peel_ptr_view(recv)
        }
        Expr::Unsafe(block) | Expr::Block(block)
            if block.stmts.is_empty() && block.tail.is_some() =>
        {
            peel_ptr_view(block.tail.as_deref().unwrap())
        }
        other => other,
    }
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

pub(super) fn is_str_or_slice(ty: &Type) -> bool {
    match ty {
        Type::Str | Type::Slice(_) => true,
        Type::Ref { inner, .. } => matches!(**inner, Type::Str | Type::Slice(_)),
        Type::Generic { name, .. } => name == "String" || name == "Vec",
        _ => false,
    }
}

fn is_u8(ty: &Type) -> bool {
    matches!(ty, Type::Prim(Prim::U8))
}

pub(super) fn is_byte_str_or_slice(ty: &Type) -> bool {
    match ty {
        Type::Str => true,
        Type::Slice(inner) => is_u8(inner),
        Type::Ref { inner, .. } => is_byte_str_or_slice(inner),
        Type::Generic { name, args } => {
            name == "String" || (name == "Vec" && args.first().is_some_and(is_u8))
        }
        _ => false,
    }
}

fn is_u8_slice(ty: &Type) -> bool {
    match ty {
        Type::Slice(inner) => is_u8(inner),
        Type::Ref { inner, .. } => is_u8_slice(inner),
        Type::Generic { name, args } => name == "Vec" && args.first().is_some_and(is_u8),
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

fn slice_to(expr: Expr, len: Expr) -> Expr {
    Expr::Index {
        base: Box::new(expr),
        index: Box::new(Expr::Range {
            start: Box::new(Expr::Value(RustValue::Usize(0))),
            end: Box::new(cast(len, Type::Prim(Prim::Usize))),
        }),
    }
}

fn lifted_bytes(ctx: &CallCtx, i: usize) -> Option<Expr> {
    ctx.lifted_arg(i, is_string)
        .map(|arg| method_call(arg, "as_bytes", Vec::new()))
        .or_else(|| ctx.lifted_arg(i, is_u8_slice))
}

fn c_string_len(bytes: Expr, bound: Option<Expr>) -> Expr {
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
    let lhs = slice_to(a.clone(), c_string_len(a, Some(bound.clone())));
    let rhs = Expr::Ref {
        mutable: false,
        expr: Box::new(slice_to(b.clone(), c_string_len(b, Some(bound)))),
    };
    Some(cast(
        method_call(lhs, "cmp", vec![rhs]),
        Type::Prim(Prim::I32),
    ))
}

fn str_n_len(ctx: &CallCtx) -> Option<Expr> {
    let bytes = lifted_bytes(ctx, 0)?;
    Some(c_string_len(bytes, Some(ctx.args().get(1)?.clone())))
}

fn str_span(ctx: &CallCtx, accepted: bool) -> Option<Expr> {
    let input = lifted_bytes(ctx, 0)?;
    let set = lifted_bytes(ctx, 1)?;
    let input = slice_to(input.clone(), c_string_len(input, None));
    let set = slice_to(set.clone(), c_string_len(set, None));
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
    Some(method_call(
        position,
        "unwrap_or",
        vec![method_call(input, "len", Vec::new())],
    ))
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

fn const_i8_ptr() -> Type {
    Type::Ptr {
        mutable: false,
        inner: Box::new(Type::Prim(Prim::I8)),
    }
}

fn ato_helper(ctx: &CallCtx, name: &str) -> Option<Expr> {
    let arg = ctx.args().first()?.clone();
    let arg = match &arg {
        Expr::Cast { ty, .. } if *ty == const_i8_ptr() => arg,
        _ => cast(arg, const_i8_ptr()),
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
    match expr {
        Expr::Cast { expr, .. } => peel_call_head(expr),
        Expr::Unsafe(block) | Expr::Block(block)
            if block.stmts.is_empty() && block.tail.is_some() =>
        {
            peel_call_head(block.tail.as_deref().unwrap())
        }
        other => other,
    }
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
        ]
    }

    fn call_anchor(&self) -> Option<Ident> {
        Some(Ident::new(self.known.symbol()))
    }

    fn matches(&self, arena: &FunctionOptimizer, id: NodeId) -> bool {
        let Some(kind) = arena.get(id) else {
            return false;
        };
        let Some(slot) = call_slot(kind) else {
            return false;
        };
        let Some(args) = known_call_args(slot, self.known) else {
            return false;
        };
        let ctx = CallCtx {
            arena,
            args,
            result_type: call_result_type(kind),
            discards_result: node_discards_result(arena, kind),
        };
        (self.build)(&ctx).is_some()
    }

    fn apply(&self, arena: &mut FunctionOptimizer, id: NodeId) -> bool {
        let Some(kind) = arena.get(id) else {
            return false;
        };
        let Some(slot) = call_slot(kind) else {
            return false;
        };
        let Some(args) = known_call_args(slot, self.known) else {
            return false;
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

pub(super) fn rules() -> Vec<Box<dyn NodeRule>> {
    vec![
        libc_call(Known::Exit, |ctx| {
            Some(free_call(&["std", "process", "exit"], ctx.args().to_vec()))
        }),
        libc_call(Known::Abort, |ctx| {
            Some(free_call(&["std", "process", "abort"], ctx.args().to_vec()))
        }),
        libc_call(Known::StrLen, |ctx| {
            let length = method_call(ctx.lifted_arg(0, is_str_or_slice)?, "len", Vec::new());
            match ctx.result_type() {
                Some(ty) if *ty == Type::Prim(Prim::Usize) => Some(length),
                Some(ty) => Some(cast(length, ty.clone())),
                None if ctx.discards_result() => Some(length),
                None => None,
            }
        }),
        libc_call(Known::StrCmp, |ctx| {
            let a = ctx.lifted_arg(0, is_byte_str_or_slice)?;
            let b = ctx.lifted_arg(1, is_byte_str_or_slice)?;
            Some(cast(method_call(a, "cmp", vec![b]), Type::Prim(Prim::I32)))
        }),
        libc_call(Known::StrNCmp, str_n_cmp),
        libc_call(Known::StrNLen, str_n_len),
        libc_call(Known::StrSpn, |ctx| str_span(ctx, true)),
        libc_call(Known::StrCSpn, |ctx| str_span(ctx, false)),
        libc_call(Known::Atoi, |ctx| {
            fold_atoi(ctx, Prim::I32).or_else(|| ato_helper(ctx, "__slate_atoi"))
        }),
        libc_call(Known::Atol, |ctx| {
            fold_atoi(ctx, Prim::I64).or_else(|| ato_helper(ctx, "__slate_atol"))
        }),
        libc_call(Known::Atoll, |ctx| {
            fold_atoi(ctx, Prim::I64).or_else(|| ato_helper(ctx, "__slate_atol"))
        }),
        libc_call(Known::Atof, fold_atof),
        libc_call(Known::MemCpy, |ctx| mem_copy(ctx, false)),
        libc_call(Known::MemMove, |ctx| mem_copy(ctx, true)),
        libc_call(Known::MemSet, mem_set),
        libc_call(Known::MemCmp, mem_cmp),
    ]
}
