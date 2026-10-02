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

fn call(name: &str, args: Vec<Expr>) -> Expr {
    Expr::Call {
        func: Box::new(Expr::Var(name.into())),
        args,
        binding: CallBinding::Generated,
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
        if !matches!(atomic.scope, ir::SyncScope::System) || !self.tables.atomic_scalar(&place.ty) {
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
            let wrapper = match &atomic_ty {
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
                _ => return Err(barrier()),
            };

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
