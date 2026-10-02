use super::*;

fn ordering(order: &ir::MemoryOrder) -> Option<Expr> {
    let name = match order {
        ir::MemoryOrder::Relaxed => "Relaxed",
        ir::MemoryOrder::Consume | ir::MemoryOrder::Acquire => "Acquire",
        ir::MemoryOrder::Release => "Release",
        ir::MemoryOrder::AcqRel => "AcqRel",
        ir::MemoryOrder::SeqCst => "SeqCst",
        ir::MemoryOrder::Dynamic(_) => return None,
    };
    Some(Expr::Var(
        format!("std::sync::atomic::Ordering::{name}").into(),
    ))
}

fn load_ordering(order: &ir::MemoryOrder) -> Option<Expr> {
    match order {
        ir::MemoryOrder::Release => ordering(&ir::MemoryOrder::Relaxed),
        ir::MemoryOrder::AcqRel => ordering(&ir::MemoryOrder::Acquire),
        order => ordering(order),
    }
}

fn call(name: &str, args: Vec<Expr>) -> Expr {
    Expr::Call {
        func: Box::new(Expr::Var(name.into())),
        args,
        binding: CallBinding::Generated,
    }
}

fn method(recv: Expr, method: &str, args: Vec<Expr>) -> Expr {
    Expr::MethodCall {
        recv: Box::new(recv),
        method: method.into(),
        args,
    }
}

fn var(name: &str) -> Expr {
    Expr::Var(name.into())
}

fn let_stmt(name: &str, init: Expr) -> Stmt {
    Stmt::Let {
        name: name.into(),
        mutable: false,
        ty: None,
        init: Some(init),
    }
}

fn identity_closure(name: &str) -> Expr {
    Expr::Closure {
        params: vec![name.into()],
        body: Box::new(var(name)),
    }
}

fn unsafe_block(stmts: Vec<Stmt>, tail: Expr) -> Expr {
    Expr::Unsafe(Box::new(rust::Block {
        stmts,
        tail: Some(Box::new(tail)),
    }))
}

fn atomic_wrapper(ty: &rust::Type) -> Option<&'static str> {
    Some(match ty {
        rust::Type::Prim(Prim::Bool) => "AtomicBool",
        rust::Type::Prim(Prim::I8) => "AtomicI8",
        rust::Type::Prim(Prim::U8) => "AtomicU8",
        rust::Type::Prim(Prim::I16) => "AtomicI16",
        rust::Type::Prim(Prim::U16) => "AtomicU16",
        rust::Type::Prim(Prim::I32) => "AtomicI32",
        rust::Type::Prim(Prim::U32) => "AtomicU32",
        rust::Type::Prim(Prim::I64) => "AtomicI64",
        rust::Type::Prim(Prim::U64) => "AtomicU64",
        rust::Type::Ptr { .. } => "AtomicPtr",
        _ => return None,
    })
}

#[derive(Clone, Copy, PartialEq, Eq)]
enum AtomicRepr {
    Value,
    Pointer,
    Bits(&'static str),
}

struct AtomicTarget {
    ty: rust::Type,
    atomic_ty: rust::Type,
    repr: AtomicRepr,
    wrapper: &'static str,
}

impl AtomicTarget {
    fn to_atomic(&self, value: Expr) -> Expr {
        match self.repr {
            AtomicRepr::Value => value,
            AtomicRepr::Pointer => Expr::Cast {
                expr: Box::new(value),
                ty: self.atomic_ty.clone(),
            },
            AtomicRepr::Bits(_) => method(value, "to_bits", Vec::new()),
        }
    }

    fn to_c(&self, value: Expr) -> Expr {
        match self.repr {
            AtomicRepr::Value => value,
            AtomicRepr::Pointer => Expr::Cast {
                expr: Box::new(value),
                ty: self.ty.clone(),
            },
            AtomicRepr::Bits(float) => call(&format!("{float}::from_bits"), vec![value]),
        }
    }
}

enum Combine {
    Operand,
    Wrapping(&'static str),
    Binary(BinOp),
    Nand,
}

fn update_children(value: &ir::Value) -> Option<Vec<&ir::Value>> {
    Some(match &value.node.value {
        ValueKind::Constant(_) | ValueKind::OldValue | ValueKind::Null => Vec::new(),
        ValueKind::Read {
            place,
            ordering: None,
        } if !place.access.volatile => Vec::new(),
        ValueKind::Convert { operand, .. }
        | ValueKind::Unary { operand, .. }
        | ValueKind::Copy { operand, .. } => vec![operand],
        ValueKind::Arith { left, right, .. } | ValueKind::Compare { left, right, .. } => {
            vec![left, right]
        }
        ValueKind::PointerOffset {
            pointer, amount, ..
        } => vec![pointer, amount],
        ValueKind::Conditional {
            condition,
            then_value,
            else_value,
        } => vec![condition, then_value, else_value],
        _ => return None,
    })
}

fn is_old(value: &ir::Value) -> bool {
    matches!(value.node.value, ValueKind::OldValue)
}

fn contains_old(value: &ir::Value) -> bool {
    is_old(value)
        || update_children(value)
            .unwrap_or_default()
            .into_iter()
            .any(contains_old)
}

fn pure_update(value: &ir::Value) -> bool {
    update_children(value).is_some_and(|children| children.into_iter().all(pure_update))
}

fn native_rmw(
    computation: &ir::Value,
    repr: AtomicRepr,
    is_bool: bool,
) -> Option<(&'static str, &ir::Value, Combine)> {
    if !contains_old(computation) {
        return Some(("swap", computation, Combine::Operand));
    }
    if repr != AtomicRepr::Value {
        return None;
    }
    match &computation.node.value {
        ValueKind::Arith {
            op,
            left,
            right,
            semantics,
        } => {
            let commutative = matches!(
                op,
                ir::ArithOp::Add | ir::ArithOp::And | ir::ArithOp::Or | ir::ArithOp::Xor
            );
            let operand = match (is_old(left), is_old(right)) {
                (true, false) if !contains_old(right) => right,
                (false, true) if commutative && !contains_old(left) => left,
                _ => return None,
            };
            let wraps = matches!(
                semantics,
                ir::ArithSema::Integer {
                    overflow: ir::Overflow::Wrap | ir::Overflow::Undefined
                }
            );
            Some(match op {
                ir::ArithOp::Add if wraps && !is_bool => {
                    ("fetch_add", operand, Combine::Wrapping("wrapping_add"))
                }
                ir::ArithOp::Sub if wraps && !is_bool => {
                    ("fetch_sub", operand, Combine::Wrapping("wrapping_sub"))
                }
                ir::ArithOp::And => ("fetch_and", operand, Combine::Binary(BinOp::BitAnd)),
                ir::ArithOp::Or => ("fetch_or", operand, Combine::Binary(BinOp::BitOr)),
                ir::ArithOp::Xor => ("fetch_xor", operand, Combine::Binary(BinOp::BitXor)),
                _ => return None,
            })
        }
        ValueKind::Unary {
            op: ir::UnaryArithOp::Not,
            operand,
            ..
        } => match &operand.node.value {
            ValueKind::Arith {
                op: ir::ArithOp::And,
                left,
                right,
                ..
            } if is_old(left) && !contains_old(right) => Some(("fetch_nand", right, Combine::Nand)),
            _ => None,
        },
        _ => None,
    }
}

impl Tables<'_> {
    pub(super) fn atomic_scalar(&self, ty: &ir::Type) -> bool {
        matches!(
            self.resolve_type(ty),
            ir::Type::Bool
                | ir::Type::Pointer { .. }
                | ir::Type::Numeric(ir::NumericType::Integer {
                    width: 8 | 16 | 32 | 64 | 128,
                    ..
                })
        )
    }
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn lower_atomic_access(
        &mut self,
        place: &ir::Place,
        atomic: &ir::Atomicity,
        value: Option<Expr>,
    ) -> Result<Expr> {
        let barrier = || {
            Failure::from(Construct::Place {
                ir: place.to_string(),
            })
        };
        if !matches!(atomic.scope, ir::SyncScope::System) {
            return Err(barrier());
        }
        let order = ordering(&atomic.order).ok_or_else(barrier)?;
        if matches!(atomic.order, ir::MemoryOrder::AcqRel)
            || (value.is_none() && matches!(atomic.order, ir::MemoryOrder::Release))
            || (value.is_some()
                && matches!(
                    atomic.order,
                    ir::MemoryOrder::Acquire | ir::MemoryOrder::Consume
                ))
        {
            return Err(barrier());
        }
        if !self.tables.atomic_scalar(&place.ty) {
            let target = self
                .atomic_target(&place.ty)?
                .filter(|target| {
                    matches!(target.repr, AtomicRepr::Bits(_)) && !place.access.volatile
                })
                .ok_or_else(barrier)?;
            let recv = self.atomic_receiver(place, &target, value.is_some())?;
            return Ok(unsafe_block(
                Vec::new(),
                match value {
                    Some(value) => method(recv, "store", vec![target.to_atomic(value), order]),
                    None => target.to_c(method(recv, "load", vec![order])),
                },
            ));
        }
        let ty = self.lower_type(&place.ty)?;
        let atomic_ty = match &ty {
            rust::Type::Ptr { inner, .. } => rust::Type::Ptr {
                mutable: true,
                inner: inner.clone(),
            },
            rust::Type::FnPtr { .. } => rust::Type::Ptr {
                mutable: true,
                inner: Box::new(rust::Type::Prim(Prim::U8)),
            },
            _ => ty.clone(),
        };
        let function_pointer = matches!(ty, rust::Type::FnPtr { .. });
        let is_store = value.is_some();
        let value = value.map(|value| {
            if function_pointer {
                Expr::Transmute {
                    from: ty.clone(),
                    to: atomic_ty.clone(),
                    expr: Box::new(value),
                }
            } else if matches!(ty, rust::Type::Ptr { .. }) {
                Expr::Cast {
                    expr: Box::new(value),
                    ty: atomic_ty.clone(),
                }
            } else {
                value
            }
        });
        let address = Expr::Cast {
            expr: Box::new(self.lower_address(place, value.is_some())?),
            ty: rust::Type::Ptr {
                mutable: true,
                inner: Box::new(atomic_ty.clone()),
            },
        };
        let lowered = if matches!(ty, rust::Type::Prim(Prim::I128 | Prim::U128)) {
            if !self.tables.target.triple.contains("linux") {
                return Err(barrier());
            }
            self.dependencies.atomic128 = true;
            let address = Expr::Cast {
                expr: Box::new(address),
                ty: rust::Type::Ptr {
                    mutable: true,
                    inner: Box::new(rust::Type::Prim(Prim::U128)),
                },
            };
            let order = Expr::Value(rust::RustValue::I64(match atomic.order {
                ir::MemoryOrder::Relaxed => 0,
                ir::MemoryOrder::Consume | ir::MemoryOrder::Acquire => 2,
                ir::MemoryOrder::Release => 3,
                ir::MemoryOrder::SeqCst => 5,
                _ => return Err(barrier()),
            }));
            match value {
                Some(value) => call(
                    "__slate_atomic128::__atomic_store_16",
                    vec![
                        address,
                        Expr::Cast {
                            expr: Box::new(value),
                            ty: rust::Type::Prim(Prim::U128),
                        },
                        order,
                    ],
                ),
                None => Expr::Cast {
                    expr: Box::new(call(
                        "__slate_atomic128::__atomic_load_16",
                        vec![address, order],
                    )),
                    ty: ty.clone(),
                },
            }
        } else {
            let wrapper = atomic_wrapper(&atomic_ty).ok_or_else(barrier)?;

            if place.access.volatile {
                self.dependencies.atomic_volatile = true;
                let ptr = call(
                    &format!("std::sync::atomic::{wrapper}::from_ptr_raw"),
                    vec![address],
                );
                let (method, args) = match value {
                    Some(value) => ("store_volatile", vec![ptr, value, order]),
                    None => ("load_volatile", vec![ptr, order]),
                };
                call(&format!("std::sync::atomic::{wrapper}::{method}"), args)
            } else {
                let recv = call(
                    &format!("std::sync::atomic::{wrapper}::from_ptr"),
                    vec![address],
                );
                let (method, args) = match value {
                    Some(value) => ("store", vec![value, order]),
                    None => ("load", vec![order]),
                };
                Expr::MethodCall {
                    recv: Box::new(recv),
                    method: method.into(),
                    args,
                }
            }
        };
        let lowered = if is_store {
            lowered
        } else if function_pointer {
            Expr::Transmute {
                from: atomic_ty,
                to: ty,
                expr: Box::new(lowered),
            }
        } else if matches!(ty, rust::Type::Ptr { .. }) {
            Expr::Cast {
                expr: Box::new(lowered),
                ty,
            }
        } else {
            lowered
        };
        Ok(Expr::Unsafe(Box::new(rust::Block {
            stmts: Vec::new(),
            tail: Some(Box::new(lowered)),
        })))
    }

    fn atomic_target(&mut self, ty: &ir::Type) -> Result<Option<AtomicTarget>> {
        let lowered = self.lower_type(ty)?;
        let (atomic_ty, repr) = match &lowered {
            rust::Type::Ptr { inner, .. } => (
                rust::Type::Ptr {
                    mutable: true,
                    inner: inner.clone(),
                },
                AtomicRepr::Pointer,
            ),
            rust::Type::Prim(Prim::F32) => (rust::Type::Prim(Prim::U32), AtomicRepr::Bits("f32")),
            rust::Type::Prim(Prim::F64) => (rust::Type::Prim(Prim::U64), AtomicRepr::Bits("f64")),
            _ if self.tables.atomic_scalar(ty) => (lowered.clone(), AtomicRepr::Value),
            _ => return Ok(None),
        };
        Ok(atomic_wrapper(&atomic_ty).map(|wrapper| AtomicTarget {
            ty: lowered,
            atomic_ty,
            repr,
            wrapper,
        }))
    }

    fn atomic_receiver(
        &mut self,
        place: &ir::Place,
        target: &AtomicTarget,
        write: bool,
    ) -> Result<Expr> {
        let address = Expr::Cast {
            expr: Box::new(self.lower_address(place, write)?),
            ty: rust::Type::Ptr {
                mutable: true,
                inner: Box::new(target.atomic_ty.clone()),
            },
        };
        Ok(call(
            &format!("std::sync::atomic::{}::from_ptr", target.wrapper),
            vec![address],
        ))
    }

    fn lower_with_old(&mut self, computation: &ir::Value, old: Expr) -> Result<Expr> {
        let saved = self.old_value.replace(old);
        let lowered = self.lower_value(computation);
        self.old_value = saved;
        lowered
    }

    pub(super) fn lower_atomic_rmw(&mut self, value: &ir::Value) -> Result<Option<Expr>> {
        match &value.node.value {
            ValueKind::Update {
                place,
                computation,
                postfix,
                ordering: Some(atomic),
            } => self.lower_atomic_update(place, computation, *postfix, atomic),
            ValueKind::CompareExchange {
                place,
                expected,
                desired,
                success,
                failure,
                sync_scope,
                weak,
                form,
            } => {
                let method = match weak {
                    ir::Weakness::Strong => "compare_exchange",
                    ir::Weakness::Weak => "compare_exchange_weak",
                    ir::Weakness::Dynamic(_) => return Ok(None),
                };
                if !matches!(sync_scope, ir::SyncScope::System)
                    || matches!(failure, ir::MemoryOrder::Release | ir::MemoryOrder::AcqRel)
                    || (*form != ir::CompareExchangeForm::Old
                        && !matches!(self.tables.resolve_type(&value.ty), ir::Type::Bool))
                {
                    return Ok(None);
                }
                let (Some(success), Some(failure)) = (ordering(success), ordering(failure)) else {
                    return Ok(None);
                };
                self.lower_compare_exchange(
                    place,
                    expected,
                    desired,
                    method,
                    [success, failure],
                    *form,
                )
            }
            _ => Ok(None),
        }
    }

    fn lower_atomic_update(
        &mut self,
        place: &ir::Place,
        computation: &ir::Value,
        postfix: bool,
        atomic: &ir::Atomicity,
    ) -> Result<Option<Expr>> {
        if !matches!(atomic.scope, ir::SyncScope::System) || place.access.volatile {
            return Ok(None);
        }
        let (Some(order), Some(fetch_order)) =
            (ordering(&atomic.order), load_ordering(&atomic.order))
        else {
            return Ok(None);
        };
        let Some(target) = self.atomic_target(&place.ty)? else {
            return Ok(None);
        };
        let is_bool = matches!(target.ty, rust::Type::Prim(Prim::Bool));
        let old = self.next_temp();
        if let Some((rmw, operand, combine)) = native_rmw(computation, target.repr, is_bool) {
            let operand_name = self.next_temp();
            let lowered_operand = self.lower_value(operand)?;
            let recv = self.atomic_receiver(place, &target, true)?;
            let previous = method(recv, rmw, vec![target.to_atomic(var(&operand_name)), order]);
            let result = if postfix {
                target.to_c(var(&old))
            } else {
                match combine {
                    Combine::Operand => var(&operand_name),
                    Combine::Wrapping(name) => method(var(&old), name, vec![var(&operand_name)]),
                    Combine::Binary(op) => Expr::Binary {
                        op,
                        lhs: Box::new(var(&old)),
                        rhs: Box::new(var(&operand_name)),
                    },
                    Combine::Nand => Expr::Unary {
                        op: rust::UnaryOp::Not,
                        expr: Box::new(Expr::Binary {
                            op: BinOp::BitAnd,
                            lhs: Box::new(var(&old)),
                            rhs: Box::new(var(&operand_name)),
                        }),
                    },
                }
            };
            return Ok(Some(unsafe_block(
                vec![
                    let_stmt(&operand_name, lowered_operand),
                    let_stmt(&old, previous),
                ],
                result,
            )));
        }
        if !pure_update(computation) {
            return Ok(None);
        }
        let current = self.next_temp();
        let next = self.lower_with_old(computation, target.to_c(var(&current)))?;
        let closure = Expr::Closure {
            params: vec![current.as_str().into()],
            body: Box::new(call("Some", vec![target.to_atomic(next)])),
        };
        let recv = self.atomic_receiver(place, &target, true)?;
        let previous = method(
            method(recv, "fetch_update", vec![order, fetch_order, closure]),
            "unwrap_or_else",
            vec![identity_closure(&current)],
        );
        let result = if postfix {
            target.to_c(var(&old))
        } else {
            self.lower_with_old(computation, target.to_c(var(&old)))?
        };
        Ok(Some(unsafe_block(vec![let_stmt(&old, previous)], result)))
    }

    fn lower_compare_exchange(
        &mut self,
        place: &ir::Place,
        expected: &ir::Value,
        desired: &ir::Value,
        exchange: &str,
        [success, failure]: [Expr; 2],
        form: ir::CompareExchangeForm,
    ) -> Result<Option<Expr>> {
        if place.access.volatile {
            return Ok(None);
        }
        let Some(target) = self.atomic_target(&place.ty)? else {
            return Ok(None);
        };
        let expected_name = self.next_temp();
        let desired_name = self.next_temp();
        let result_name = self.next_temp();
        let mut stmts = vec![
            let_stmt(&expected_name, self.lower_value(expected)?),
            let_stmt(&desired_name, self.lower_value(desired)?),
        ];
        let expected_value = if form == ir::CompareExchangeForm::WriteBack {
            Expr::Unary {
                op: rust::UnaryOp::Deref,
                expr: Box::new(var(&expected_name)),
            }
        } else {
            var(&expected_name)
        };
        let recv = self.atomic_receiver(place, &target, true)?;
        stmts.push(let_stmt(
            &result_name,
            method(
                recv,
                exchange,
                vec![
                    target.to_atomic(expected_value.clone()),
                    target.to_atomic(var(&desired_name)),
                    success,
                    failure,
                ],
            ),
        ));
        let observed = self.next_temp();
        let tail = match form {
            ir::CompareExchangeForm::Old => target.to_c(method(
                var(&result_name),
                "unwrap_or_else",
                vec![identity_closure(&observed)],
            )),
            ir::CompareExchangeForm::Success => method(var(&result_name), "is_ok", Vec::new()),
            ir::CompareExchangeForm::WriteBack => Expr::Match {
                expr: Box::new(var(&result_name)),
                arms: vec![
                    rust::ExprMatchArm {
                        pattern: rust::Pattern::TupleStruct {
                            name: "Ok".into(),
                            fields: vec![rust::Pattern::Wildcard],
                        },
                        value: Expr::Value(rust::RustValue::Bool(true)),
                    },
                    rust::ExprMatchArm {
                        pattern: rust::Pattern::TupleStruct {
                            name: "Err".into(),
                            fields: vec![rust::Pattern::Binding(observed.as_str().into())],
                        },
                        value: Expr::Block(Box::new(rust::Block {
                            stmts: vec![Stmt::Assign {
                                target: expected_value,
                                value: target.to_c(var(&observed)),
                            }],
                            tail: Some(Box::new(Expr::Value(rust::RustValue::Bool(false)))),
                        })),
                    },
                ],
            },
        };
        Ok(Some(unsafe_block(stmts, tail)))
    }

    pub(super) fn lower_fence(
        &mut self,
        scope: ir::FenceScope,
        atomic: &ir::Atomicity,
    ) -> Option<Expr> {
        if !matches!(atomic.scope, ir::SyncScope::System) {
            return None;
        }
        if matches!(atomic.order, ir::MemoryOrder::Relaxed) {
            return Some(Expr::Block(Box::default()));
        }
        let order = ordering(&atomic.order)?;
        Some(call(
            match scope {
                ir::FenceScope::Thread => "std::sync::atomic::fence",
                ir::FenceScope::Signal => "std::sync::atomic::compiler_fence",
            },
            vec![order],
        ))
    }
}
