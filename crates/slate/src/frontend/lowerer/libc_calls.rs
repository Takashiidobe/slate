use super::*;
use crate::function_identity::Known;
use rust::RustValue;

struct Site<'c> {
    args: &'c [Expr],
    slices: Vec<Option<Expr>>,
    arrays: Vec<Option<(Expr, rust::Type, u64)>>,
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
        let mut arrays = Vec::new();
        if matches!(known, Known::MemCpy | Known::MemMove | Known::MemSet) && arguments.len() == 3 {
            for argument in &arguments[..2] {
                arrays.push(self.local_byte_array(argument)?);
            }
        }
        let site = Site {
            args,
            slices: arguments
                .iter()
                .map(|argument| self.byte_slice(argument))
                .collect(),
            arrays,
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

    fn byte_slice(&self, value: &ir::Value) -> Option<Expr> {
        match &value.node.value {
            ValueKind::Copy { operand, .. }
            | ValueKind::Convert {
                kind: ir::ConversionKind::PointerCast,
                operand,
                ..
            } => self.byte_slice(operand),
            ValueKind::Read {
                place:
                    ir::Place {
                        kind: PlaceKind::Binding(id),
                        ..
                    },
                ordering: None,
            } if self.borrow_of(*id).is_some_and(|borrow| borrow.many)
                && matches!(
                    self.tables.resolve_type(&value.ty),
                    ir::Type::Pointer { pointee, .. }
                        if matches!(**pointee, ir::Type::Numeric(ir::NumericType::Integer { width: 8, signed: false, .. }))
                ) =>
            {
                Some(Expr::var(self.binding_name(*id).as_str().into()))
            }
            #[expect(
                slate::sensitive_wildcard,
                reason = "only borrowed unsigned byte pointers lower to byte slices"
            )]
            _ => None,
        }
    }

    fn local_byte_array(&mut self, value: &ir::Value) -> Result<Option<(Expr, rust::Type, u64)>> {
        match &value.node.value {
            ValueKind::Copy { operand, .. }
            | ValueKind::Convert {
                kind: ir::ConversionKind::PointerCast,
                operand,
                ..
            } => self.local_byte_array(operand),
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
                if !matches!(
                    self.tables.resolve_type(element),
                    ir::Type::Numeric(ir::NumericType::Integer { width: 8, .. })
                ) {
                    return Ok(None);
                }
                let element = self.lower_type(element)?;
                Ok(Some((self.lower_place(place)?, element, *length)))
            }
            #[expect(
                slate::sensitive_wildcard,
                reason = "only a decayed local byte array has a known extent"
            )]
            _ => Ok(None),
        }
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

fn upto(base: Expr, end: Expr) -> Expr {
    Expr::Index {
        base: Box::new(base),
        index: Box::new(Expr::Range {
            start: Box::new(usize_lit(0)),
            end: Box::new(end),
        }),
    }
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
    let a = site.slices.first()?.clone()?;
    let b = site.slices.get(1)?.clone()?;
    let len = site.args.get(2)?;
    if !duplicable(len) {
        return None;
    }
    let len = cast(len.clone(), rust::Type::Prim(Prim::Usize));
    Some(cast(
        method_call(upto(a, len.clone()), "cmp", vec![borrowed(upto(b, len))]),
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
    if let Some(n) = const_len(len)
        && let (Some((to, to_elem, to_len)), Some((from, from_elem, from_len))) =
            (site.arrays[0].clone(), site.arrays[1].clone())
        && to_elem == from_elem
        && n <= to_len
        && n <= from_len
    {
        let effect = if to == from {
            overlapping.then(|| {
                method_call(
                    to,
                    "copy_within",
                    vec![
                        Expr::Range {
                            start: Box::new(usize_lit(0)),
                            end: Box::new(usize_lit(n)),
                        },
                        usize_lit(0),
                    ],
                )
            })
        } else {
            Some(method_call(
                upto(to, usize_lit(n)),
                "copy_from_slice",
                vec![borrowed(upto(from, usize_lit(n)))],
            ))
        };
        if let Some(effect) = effect {
            return returning_dst(site, effect, dst);
        }
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
    if let Some(n) = const_len(len)
        && let Some((to, element, to_len)) = site.arrays[0].clone()
        && n <= to_len
    {
        let effect = method_call(
            upto(to, usize_lit(n)),
            "fill",
            vec![cast(value.clone(), element)],
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
