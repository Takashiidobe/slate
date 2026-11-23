use super::*;
use crate::function_identity::Known;
use rust::RustValue;

struct Site<'c> {
    args: &'c [Expr],
    buffers: Vec<Option<Buffer>>,
    discarded: bool,
    result: rust::Type,
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn known_callee(&self, callee: &ir::Callee) -> Option<Known> {
        let ir::Callee::Direct(id) = callee else {
            return None;
        };
        let name = self.tables.names.get(id)?;
        let FunctionIdentity::Known(known) = name.identity else {
            return None;
        };
        (name.rust.trim_start_matches("__builtin_") == known.symbol()).then_some(known)
    }

    pub(super) fn call_result(&mut self, signature: &ir::Type) -> Result<rust::Type> {
        Ok(match self.tables.resolve_type(signature) {
            ir::Type::Function {
                return_type: Some(ret),
                ..
            } => self.lower_type(ret)?,
            _ => rust::Type::Unit,
        })
    }

    pub(super) fn lower_known_call(
        &mut self,
        known: Known,
        arguments: &[ir::Value],
        args: &[Expr],
        signature: &ir::Type,
        discarded: bool,
    ) -> Result<Option<Expr>> {
        if args.len() != arguments.len() {
            return Ok(None);
        }
        if matches!(known, Known::MemCpy | Known::MemMove | Known::MemSet)
            && !discarded
            && let Some(dst) = args.first()
            && !duplicable(dst)
        {
            let temp = self.next_temp();
            let mut bound = args.to_vec();
            bound[0] = Expr::var(temp.as_str().into());
            let Some(lowered) =
                self.lower_known_call(known, arguments, &bound, signature, discarded)?
            else {
                return Ok(None);
            };
            return Ok(Some(Expr::Block(Box::new(rust::Block {
                stmts: vec![Stmt::Let {
                    origin: rust::LocalOrigin::Synthetic,
                    name: temp,
                    mutable: false,
                    ty: None,
                    init: Some(dst.clone()),
                }],
                tail: Some(Box::new(lowered)),
            }))));
        }
        let result = self.call_result(signature)?;
        let mut buffers = Vec::new();
        if let Some((indices, arity)) = known.buffer_arguments()
            && arguments.len() == arity
        {
            for &index in indices {
                buffers.push(self.buffer(&arguments[index])?);
            }
        }
        let site = Site {
            args,
            buffers,
            discarded,
            result,
        };
        let char_prim = crate::backend::engine::char_prim(self.tables.target);
        Ok(match known {
            Known::Exit if self.tables.runtime.has_std() => {
                Some(free_call(&["std", "process", "exit"], args.to_vec()))
            }
            Known::Abort if self.tables.runtime.has_std() => {
                Some(free_call(&["std", "process", "abort"], args.to_vec()))
            }
            Known::Free => self.lower_release(arguments),
            Known::StrCpy | Known::StrCat if discarded => {
                self.lower_c_string_copy(known, arguments)?
            }
            Known::Atoi => Some(ato_helper(&site, "__slate_atoi", char_prim)),
            Known::Atol => Some(ato_helper(&site, "__slate_atol", char_prim)),
            Known::Atoll => Some(ato_helper(&site, "__slate_atoll", char_prim)),
            Known::MemCpy => mem_copy(&site, false),
            Known::MemMove => mem_copy(&site, true),
            Known::MemSet => mem_set(&site),
            Known::MemCmp => mem_cmp(&site),
            #[expect(
                slate::sensitive_wildcard,
                reason = "every other libc function lowers to a plain call"
            )]
            _ => None,
        })
    }

    pub(super) fn array_alias(&self, binding: BindingId) -> bool {
        !self.unborrowed.contains(&binding)
            && !self.switch_lets.contains(&binding)
            && self.tables.array_alias_of(binding).is_some()
    }

    fn buffer(&mut self, value: &ir::Value) -> Result<Option<Buffer>> {
        match &value.node.value {
            ValueKind::Copy { operand, .. }
            | ValueKind::Convert {
                kind: ir::ConversionKind::PointerCast,
                operand,
                ..
            } => self.buffer(operand),
            ValueKind::PointerOffset {
                pointer,
                amount,
                subtract: false,
                element,
                ..
            } if byte_integer(self.tables.resolve_type(element))
                && let Some(amount) = constant_amount(amount) =>
            {
                let Some(buffer) = self.buffer(pointer)? else {
                    return Ok(None);
                };
                let offset = buffer.offset + amount;
                Ok(buffer
                    .length
                    .is_none_or(|length| offset <= length)
                    .then_some(Buffer { offset, ..buffer }))
            }
            ValueKind::ArrayDecay {
                place:
                    place @ ir::Place {
                        kind: PlaceKind::Binding(id),
                        ..
                    },
                length: Some(length),
            } if !self.tables.statics.contains(id) && !self.tables.strings.contains_key(id) => {
                let ir::Type::Array { element, .. } = self.tables.resolve_type(&place.ty) else {
                    return Ok(None);
                };
                if !byte_integer(self.tables.resolve_type(element)) {
                    return Ok(None);
                }
                Ok(Some(Buffer {
                    root: *id,
                    base: self.lower_place(place)?,
                    element: self.lower_type(element)?,
                    offset: 0,
                    length: Some(*length),
                    mutable: true,
                }))
            }
            ValueKind::Read {
                place:
                    ir::Place {
                        kind: PlaceKind::Binding(id),
                        ..
                    },
                ordering: None,
            } => {
                if let Some(initializer) = self.array_aliases.get(id) {
                    let initializer = initializer.clone();
                    return self.buffer(&initializer);
                }
                let Some(borrow) = self
                    .borrow_of(*id)
                    .filter(|borrow| borrow.many && !borrow.nullable)
                else {
                    return Ok(None);
                };
                let ir::Type::Pointer { pointee, .. } = self.tables.resolve_type(&value.ty) else {
                    return Ok(None);
                };
                if !byte_integer(self.tables.resolve_type(pointee)) {
                    return Ok(None);
                }
                Ok(Some(Buffer {
                    root: *id,
                    base: Expr::var(self.binding_name(*id).as_str().into()),
                    element: self.lower_type(pointee)?,
                    offset: 0,
                    length: None,
                    mutable: borrow.mutable,
                }))
            }
            #[expect(
                slate::sensitive_wildcard,
                reason = "only local byte arrays, their aliases and offsets, and lifted byte slices are buffers"
            )]
            _ => Ok(None),
        }
    }
}

#[derive(Clone)]
struct Buffer {
    root: BindingId,
    base: Expr,
    element: rust::Type,
    offset: u64,
    length: Option<u64>,
    mutable: bool,
}

impl Buffer {
    fn holds(&self, n: Option<u64>) -> bool {
        match (n, self.length) {
            (Some(n), Some(length)) => self.offset.checked_add(n).is_some_and(|end| end <= length),
            _ => true,
        }
    }

    fn range(&self, len: &Length) -> Expr {
        Expr::Range {
            start: Box::new(usize_lit(self.offset)),
            end: Box::new(self.end(len)),
        }
    }

    fn end(&self, len: &Length) -> Expr {
        match len {
            Length::Constant(n) => usize_lit(self.offset + n),
            Length::Runtime(len) if self.offset == 0 => len.clone(),
            Length::Runtime(len) => Expr::Binary {
                op: rust::BinOp::Add,
                lhs: Box::new(usize_lit(self.offset)),
                rhs: Box::new(len.clone()),
            },
        }
    }

    fn span(&self, len: &Length) -> Expr {
        Expr::Index {
            base: Box::new(self.base.clone()),
            index: Box::new(self.range(len)),
        }
    }
}

enum Length {
    Constant(u64),
    Runtime(Expr),
}

impl Length {
    fn of(len: &Expr, uses: usize) -> Option<Self> {
        if let Some(n) = const_len(len) {
            return Some(Self::Constant(n));
        }
        (uses == 1 || duplicable(len))
            .then(|| Self::Runtime(cast(len.clone(), rust::Type::Prim(Prim::Usize))))
    }

    fn constant(&self) -> Option<u64> {
        match self {
            Self::Constant(n) => Some(*n),
            Self::Runtime(_) => None,
        }
    }
}

fn byte_integer(ty: &ir::Type) -> bool {
    matches!(
        ty,
        ir::Type::Numeric(ir::NumericType::Integer { width: 8, .. })
    )
}

fn constant_amount(value: &ir::Value) -> Option<u64> {
    match &value.node.value {
        ValueKind::Copy { operand, .. } | ValueKind::Convert { operand, .. } => {
            constant_amount(operand)
        }
        ValueKind::Constant(ir::Number::Integer(n)) => u64::try_from(n).ok(),
        ValueKind::Constant(ir::Number::SignedInteger(n)) => u64::try_from(n).ok(),
        #[expect(
            slate::sensitive_wildcard,
            reason = "only integer constants are constant offsets"
        )]
        _ => None,
    }
}

pub(super) fn c_content(bytes: &[u8]) -> &[u8] {
    let end = bytes
        .iter()
        .position(|&byte| byte == 0)
        .unwrap_or(bytes.len());
    &bytes[..end]
}

pub(super) fn const_len(expr: &Expr) -> Option<u64> {
    match expr {
        Expr::Cast { expr, .. } => const_len(expr),
        Expr::Value(RustValue::I64(n)) => u64::try_from(*n).ok(),
        Expr::Value(RustValue::Usize(n)) => Some(*n as u64),
        Expr::Value(RustValue::I128(n) | RustValue::TypedInt(n, _)) => u64::try_from(*n).ok(),
        Expr::Value(RustValue::U128(n) | RustValue::TypedUInt(n, _)) => u64::try_from(*n).ok(),
        Expr::LayoutQuery { amount, .. } => Some(amount.0),
        #[expect(
            slate::sensitive_wildcard,
            reason = "only literals and layout queries are constant lengths"
        )]
        _ => None,
    }
}

pub(super) fn duplicable(expr: &Expr) -> bool {
    match expr {
        Expr::Var { .. } | Expr::Value(_) | Expr::LayoutQuery { .. } => true,
        Expr::Cast { expr, .. } => duplicable(expr),
        Expr::Field { base, .. } => duplicable(base),
        Expr::MethodCall {
            recv, method, args, ..
        } => {
            matches!(method.as_str(), "as_ptr" | "as_mut_ptr" | "add" | "offset")
                && duplicable(recv)
                && args.iter().all(duplicable)
        }
        #[expect(
            slate::sensitive_wildcard,
            reason = "anything else may have effects or be costly to evaluate twice"
        )]
        _ => false,
    }
}

pub(super) fn method_call(recv: Expr, method: &str, args: Vec<Expr>) -> Expr {
    Expr::MethodCall {
        recv: Box::new(recv),
        method: method.to_string(),
        args,
        unsafe_: rust::Unsafe(false),
    }
}

pub(super) fn cast(expr: Expr, ty: rust::Type) -> Expr {
    Expr::Cast {
        expr: Box::new(expr),
        ty,
    }
}

pub(super) fn usize_lit(n: u64) -> Expr {
    Expr::Value(RustValue::Usize(n as usize))
}

fn borrowed(expr: Expr) -> Expr {
    Expr::Ref {
        mutable: false,
        expr: Box::new(expr),
    }
}

fn byte_ptr(mutable: bool) -> rust::Type {
    rust::Type::Ptr {
        mutable,
        inner: Box::new(rust::Type::Prim(Prim::U8)),
    }
}

fn free_call(path: &[&str], args: Vec<Expr>) -> Expr {
    Expr::Call {
        binding: CallBinding::Generated,
        func: Box::new(Expr::Path(rust::Path::new(
            path.iter().copied().map(rust::Ident::from),
        ))),
        args,
        unsafe_: rust::Unsafe(false),
    }
}

fn mem_cmp(site: &Site) -> Option<Expr> {
    let [Some(a), Some(b)] = site.buffers.as_slice() else {
        return None;
    };
    let len = Length::of(site.args.get(2)?, 2)?;
    if a.element != rust::Type::Prim(Prim::U8)
        || b.element != a.element
        || !a.holds(len.constant())
        || !b.holds(len.constant())
    {
        return None;
    }
    Some(cast(
        method_call(a.span(&len), "cmp", vec![borrowed(b.span(&len))]),
        rust::Type::Prim(Prim::I32),
    ))
}

fn ato_helper(site: &Site, name: &str, char_prim: Prim) -> Expr {
    let pointer = rust::Type::Ptr {
        mutable: false,
        inner: Box::new(rust::Type::Prim(char_prim)),
    };
    let arg = match site.args.first() {
        Some(arg @ Expr::Cast { ty, .. }) if *ty == pointer => arg.clone(),
        Some(arg) => cast(arg.clone(), pointer),
        None => Expr::Value(RustValue::NullPtr),
    };
    support_call(name, vec![arg], true)
}

pub(super) fn support_call(name: &str, args: Vec<Expr>, unsafe_: bool) -> Expr {
    Expr::Call {
        binding: CallBinding::Generated,
        func: Box::new(Expr::Path(rust::Path::new(
            ["slate_support", name].map(rust::Ident::from),
        ))),
        args,
        unsafe_: rust::Unsafe(unsafe_),
    }
}

fn returning_dst(site: &Site, effect: Expr, dst: &Expr) -> Option<Expr> {
    if site.discarded {
        return Some(effect);
    }
    if !duplicable(dst) {
        return None;
    }
    Some(Expr::Block(Box::new(rust::Block {
        stmts: vec![Stmt::Expr(effect)],
        tail: Some(Box::new(cast(dst.clone(), site.result.clone()))),
    })))
}

fn mem_copy(site: &Site, overlapping: bool) -> Option<Expr> {
    let [dst, src, len] = site.args else {
        return None;
    };
    if let [Some(to), Some(from)] = site.buffers.as_slice()
        && to.mutable
        && to.element == from.element
        && let Some(n) = Length::of(len, 2)
        && to.holds(n.constant())
        && from.holds(n.constant())
    {
        let effect = if to.root == from.root {
            method_call(
                to.base.clone(),
                "copy_within",
                vec![from.range(&n), usize_lit(to.offset)],
            )
        } else {
            method_call(
                to.span(&n),
                "copy_from_slice",
                vec![borrowed(from.span(&n))],
            )
        };
        return returning_dst(site, effect, dst);
    }
    let effect = Expr::PtrCopy {
        src: Box::new(cast(src.clone(), byte_ptr(false))),
        dst: Box::new(cast(dst.clone(), byte_ptr(true))),
        count: Box::new(cast(len.clone(), rust::Type::Prim(Prim::Usize))),
        overlapping,
    };
    returning_dst(site, effect, dst)
}

fn mem_set(site: &Site) -> Option<Expr> {
    let [dst, value, len] = site.args else {
        return None;
    };
    if let Some(Some(to)) = site.buffers.first()
        && to.mutable
        && let Some(n) = Length::of(len, 1)
        && to.holds(n.constant())
    {
        let effect = method_call(
            to.span(&n),
            "fill",
            vec![cast(value.clone(), to.element.clone())],
        );
        return returning_dst(site, effect, dst);
    }
    let effect = Expr::WriteBytes {
        dst: Box::new(cast(dst.clone(), byte_ptr(true))),
        val: Box::new(cast(value.clone(), rust::Type::Prim(Prim::U8))),
        count: Box::new(cast(len.clone(), rust::Type::Prim(Prim::Usize))),
    };
    returning_dst(site, effect, dst)
}
