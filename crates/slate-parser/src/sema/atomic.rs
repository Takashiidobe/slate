use super::expression::Lowerer;
use super::numeric::ResolveError;
use crate::ast::{Expr, ExprKind};
use crate::ir::*;
use num_bigint::BigInt;

#[derive(Clone, Copy)]
pub(super) enum AtomicBuiltin {
    Init,
    Load {
        generic: bool,
    },
    Store {
        generic: bool,
    },
    Exchange {
        generic: bool,
    },
    CompareExchange(CompareExchangeForm),
    Fetch {
        op: FetchOp,
        postfix: bool,
        byte_offsets: bool,
    },
    TestAndSet,
    Clear,
    Fence(FenceScope),
}

#[derive(Clone, Copy)]
pub(super) enum CompareExchangeForm {
    C11 { weak: bool },
    Gnu { generic: bool },
}

#[derive(Clone, Copy)]
pub(super) enum FetchOp {
    Add,
    Sub,
    And,
    Or,
    Xor,
    Nand,
    Min,
    Max,
}

impl FetchOp {
    fn parse(name: &str) -> Option<Self> {
        Some(match name {
            "add" => Self::Add,
            "sub" => Self::Sub,
            "and" => Self::And,
            "or" => Self::Or,
            "xor" => Self::Xor,
            "nand" => Self::Nand,
            "min" => Self::Min,
            "max" => Self::Max,
            _ => return None,
        })
    }
}

pub(super) fn atomic_builtin(callee: &Expr) -> Option<AtomicBuiltin> {
    let ExprKind::Identifier(name) = &callee.value else {
        return None;
    };
    if let Some(operation) = name.strip_prefix("__c11_atomic_") {
        return Some(match operation {
            "init" => AtomicBuiltin::Init,
            "load" => AtomicBuiltin::Load { generic: false },
            "store" => AtomicBuiltin::Store { generic: false },
            "exchange" => AtomicBuiltin::Exchange { generic: false },
            "compare_exchange_strong" => {
                AtomicBuiltin::CompareExchange(CompareExchangeForm::C11 { weak: false })
            }
            "compare_exchange_weak" => {
                AtomicBuiltin::CompareExchange(CompareExchangeForm::C11 { weak: true })
            }
            "thread_fence" => AtomicBuiltin::Fence(FenceScope::Thread),
            "signal_fence" => AtomicBuiltin::Fence(FenceScope::Signal),
            _ => AtomicBuiltin::Fetch {
                op: FetchOp::parse(operation.strip_prefix("fetch_")?)?,
                postfix: true,
                byte_offsets: false,
            },
        });
    }
    let operation = name.strip_prefix("__atomic_")?;
    Some(match operation {
        "load_n" => AtomicBuiltin::Load { generic: false },
        "load" => AtomicBuiltin::Load { generic: true },
        "store_n" => AtomicBuiltin::Store { generic: false },
        "store" => AtomicBuiltin::Store { generic: true },
        "exchange_n" => AtomicBuiltin::Exchange { generic: false },
        "exchange" => AtomicBuiltin::Exchange { generic: true },
        "compare_exchange_n" => {
            AtomicBuiltin::CompareExchange(CompareExchangeForm::Gnu { generic: false })
        }
        "compare_exchange" => {
            AtomicBuiltin::CompareExchange(CompareExchangeForm::Gnu { generic: true })
        }
        "test_and_set" => AtomicBuiltin::TestAndSet,
        "clear" => AtomicBuiltin::Clear,
        "thread_fence" => AtomicBuiltin::Fence(FenceScope::Thread),
        "signal_fence" => AtomicBuiltin::Fence(FenceScope::Signal),
        _ => match operation.strip_prefix("fetch_") {
            Some(op) => AtomicBuiltin::Fetch {
                op: FetchOp::parse(op)?,
                postfix: true,
                byte_offsets: true,
            },
            None => AtomicBuiltin::Fetch {
                op: FetchOp::parse(operation.strip_suffix("_fetch")?)?,
                postfix: false,
                byte_offsets: true,
            },
        },
    })
}

impl Lowerer {
    pub(super) fn atomic_builtin(
        &mut self,
        e: &Expr,
        callee: &Expr,
        builtin: AtomicBuiltin,
        arguments: &[Expr],
    ) -> Result<Value, ResolveError> {
        if let ExprKind::Identifier(name) = &callee.value {
            self.module
                .metadata
                .entry(e.id)
                .or_default()
                .push(("c_builtin".into(), name.clone()));
        }
        match (builtin, arguments) {
            (AtomicBuiltin::Init, [object, desired]) => {
                let place = self.atomic_object(object)?;
                let value = self.atomic_operand(desired, &place)?;
                let store = self.store(e, place, value, None);
                Ok(self.discarded(e, store))
            }
            (AtomicBuiltin::Load { generic: false }, [object, order]) => {
                let place = self.atomic_object(object)?;
                let ordering = self.memory_order(order)?;
                Ok(self.atomic_read(e, place, Some(ordering)))
            }
            (AtomicBuiltin::Load { generic: true }, [object, result, order]) => {
                let place = self.atomic_object(object)?;
                let ordering = self.memory_order(order)?;
                let value = self.atomic_read(e, place, Some(ordering));
                let target = self.atomic_object(result)?;
                let ordering = target.implicit_ordering();
                let store = self.store(result, target, value, ordering);
                Ok(self.discarded(e, store))
            }
            (AtomicBuiltin::Store { generic }, [object, desired, order]) => {
                let place = self.atomic_object(object)?;
                let value = self.desired(desired, &place, generic)?;
                let ordering = self.memory_order(order)?;
                let store = self.store(e, place, value, Some(ordering));
                Ok(self.discarded(e, store))
            }
            (AtomicBuiltin::Exchange { generic: false }, [object, desired, order]) => {
                let place = self.atomic_object(object)?;
                let value = self.atomic_operand(desired, &place)?;
                let ordering = self.memory_order(order)?;
                Ok(self.atomic_update(e, place, value, true, ordering))
            }
            (AtomicBuiltin::Exchange { generic: true }, [object, desired, result, order]) => {
                let place = self.atomic_object(object)?;
                let value = self.desired(desired, &place, true)?;
                let ordering = self.memory_order(order)?;
                let old = self.atomic_update(e, place, value, true, ordering);
                let target = self.atomic_object(result)?;
                let ordering = target.implicit_ordering();
                let store = self.store(result, target, old, ordering);
                Ok(self.discarded(e, store))
            }
            (
                AtomicBuiltin::CompareExchange(CompareExchangeForm::C11 { weak }),
                [object, expected, desired, success, failure],
            ) => self.compare_exchange(
                e,
                [object, expected, desired],
                [success, failure],
                false,
                weak,
            ),
            (
                AtomicBuiltin::CompareExchange(CompareExchangeForm::Gnu { generic }),
                [object, expected, desired, weak, success, failure],
            ) => {
                let weak = self.expr(weak)?;
                let weak = constant_integer(&weak).ok_or(ResolveError::Unsupported(
                    "non-constant compare-exchange weak flag",
                ))?;
                let weak = weak != BigInt::from(0);
                self.compare_exchange(
                    e,
                    [object, expected, desired],
                    [success, failure],
                    generic,
                    weak,
                )
            }
            (
                AtomicBuiltin::Fetch {
                    op,
                    postfix,
                    byte_offsets,
                },
                [object, operand, order],
            ) => {
                let place = self.atomic_object(object)?;
                let computation = self.fetch_computation(&place, op, operand, byte_offsets)?;
                let ordering = self.memory_order(order)?;
                Ok(self.atomic_update(e, place, computation, postfix, ordering))
            }
            (AtomicBuiltin::TestAndSet, [object, order]) => {
                let place = self.flag_object(object)?;
                let set = self.flag_constant(object, place.ty.clone(), 1);
                let ordering = self.memory_order(order)?;
                let old = self.atomic_update(e, place, set, true, ordering);
                if old.ty == Type::Bool {
                    return Ok(old);
                }
                let zero = self.flag_constant(object, old.ty.clone(), 0);
                Ok(self.value(
                    e,
                    Type::Bool,
                    ValueKind::Compare {
                        op: CompareOp::Ne,
                        left: Box::new(old),
                        right: Box::new(zero),
                        exceptions: None,
                        reason: None,
                    },
                ))
            }
            (AtomicBuiltin::Clear, [object, order]) => {
                let place = self.flag_object(object)?;
                let clear = self.flag_constant(object, place.ty.clone(), 0);
                let ordering = self.memory_order(order)?;
                let store = self.store(e, place, clear, Some(ordering));
                Ok(self.discarded(e, store))
            }
            (AtomicBuiltin::Fence(scope), [order]) => {
                let ordering = self.memory_order(order)?;
                Ok(self.value(e, Type::Void, ValueKind::Fence { ordering, scope }))
            }
            _ => Err(ResolveError::Unsupported("atomic builtin argument count")),
        }
    }

    fn atomic_object(&mut self, object: &Expr) -> Result<Place, ResolveError> {
        let pointer = self.expr(object)?;
        let place = self.deref(pointer)?;
        if matches!(place.ty, Type::Void | Type::Function { .. }) {
            return Err(ResolveError::Unsupported(
                "atomic builtin on non-object pointer",
            ));
        }
        Ok(place)
    }

    fn flag_object(&mut self, object: &Expr) -> Result<Place, ResolveError> {
        let pointer = self.expr(object)?;
        let mut place = self.deref(pointer)?;
        if place.ty == Type::Void {
            place.ty = Type::integer(8, false);
        }
        Ok(place)
    }

    fn flag_constant(&self, e: &Expr, ty: Type, value: u8) -> Value {
        let number = if ty == Type::Bool {
            Number::Bool(value != 0)
        } else {
            Number::Integer(value.into())
        };
        self.value(e, ty, ValueKind::Constant(number))
    }

    fn atomic_operand(&mut self, operand: &Expr, place: &Place) -> Result<Value, ResolveError> {
        let value = self.expr(operand)?;
        self.convert_expr(operand, value, place.ty.clone(), ConversionReason::Arg)
    }

    fn desired(
        &mut self,
        desired: &Expr,
        place: &Place,
        generic: bool,
    ) -> Result<Value, ResolveError> {
        if !generic {
            return self.atomic_operand(desired, place);
        }
        let source = self.atomic_object(desired)?;
        let ordering = source.implicit_ordering();
        Ok(self.atomic_read(desired, source, ordering))
    }

    fn atomic_read(&self, e: &Expr, place: Place, ordering: Option<MemoryOrder>) -> Value {
        self.value(e, place.ty.clone(), ValueKind::Read { place, ordering })
    }

    fn store(&self, e: &Expr, place: Place, value: Value, ordering: Option<MemoryOrder>) -> Value {
        self.value(
            e,
            place.ty.clone(),
            ValueKind::Store {
                place,
                value: Box::new(value),
                ordering,
            },
        )
    }

    fn discarded(&self, e: &Expr, value: Value) -> Value {
        let void = self.value(e, Type::Void, ValueKind::Void);
        self.value(
            e,
            Type::Void,
            ValueKind::Sequence {
                left: Box::new(value),
                right: Box::new(void),
            },
        )
    }

    fn atomic_update(
        &self,
        e: &Expr,
        place: Place,
        computation: Value,
        postfix: bool,
        ordering: MemoryOrder,
    ) -> Value {
        self.value(
            e,
            place.ty.clone(),
            ValueKind::Update {
                place,
                computation: Box::new(computation),
                postfix,
                ordering: Some(ordering),
            },
        )
    }

    fn compare_exchange(
        &mut self,
        e: &Expr,
        [object, expected, desired]: [&Expr; 3],
        [success, failure]: [&Expr; 2],
        generic: bool,
        weak: bool,
    ) -> Result<Value, ResolveError> {
        let place = self.atomic_object(object)?;
        let expected = self.expr(expected)?;
        self.pointee(&expected.ty)?;
        let desired = self.desired(desired, &place, generic)?;
        let success = self.memory_order(success)?;
        let failure = self.memory_order(failure)?;
        Ok(self.value(
            e,
            Type::Bool,
            ValueKind::CompareExchange {
                place,
                expected: Box::new(expected),
                desired: Box::new(desired),
                success,
                failure,
                weak,
            },
        ))
    }

    fn fetch_computation(
        &mut self,
        place: &Place,
        op: FetchOp,
        e: &Expr,
        byte_offsets: bool,
    ) -> Result<Value, ResolveError> {
        let operand = self.expr(e)?;
        let old = self.value(e, place.ty.clone(), ValueKind::OldValue);
        if let Type::Pointer { pointee, .. } = &place.ty {
            let subtract = match op {
                FetchOp::Add => false,
                FetchOp::Sub => true,
                _ => {
                    return Err(ResolveError::Unsupported(
                        "atomic bitwise operation on pointer",
                    ));
                }
            };
            if !matches!(operand.ty, Type::Numeric(NumericType::Integer { .. })) {
                return Err(ResolveError::Unsupported(
                    "noninteger atomic pointer offset",
                ));
            }
            let element = if byte_offsets {
                Type::integer(8, false)
            } else {
                self.types.storage((**pointee).clone())?;
                (**pointee).clone()
            };
            return Ok(self.value(
                e,
                place.ty.clone(),
                ValueKind::PointerOffset {
                    pointer: Box::new(old),
                    amount: Box::new(operand),
                    subtract,
                    element,
                    overflow: Overflow::Wrap,
                },
            ));
        }
        if !matches!(place.ty, Type::Numeric(NumericType::Integer { .. })) {
            return Err(ResolveError::Unsupported(
                "atomic arithmetic on non-integer",
            ));
        }
        let operand = self.convert(operand, place.ty.clone(), ConversionReason::Arg)?;
        let arith = |this: &Self, op, left, right, semantics| {
            this.value(
                e,
                place.ty.clone(),
                ValueKind::Arith {
                    op,
                    left: Box::new(left),
                    right: Box::new(right),
                    semantics,
                },
            )
        };
        let wrap = ArithSema::Integer {
            overflow: Overflow::Wrap,
        };
        Ok(match op {
            FetchOp::Add => arith(self, ArithOp::Add, old, operand, wrap),
            FetchOp::Sub => arith(self, ArithOp::Sub, old, operand, wrap),
            FetchOp::And => arith(self, ArithOp::And, old, operand, ArithSema::Exact),
            FetchOp::Or => arith(self, ArithOp::Or, old, operand, ArithSema::Exact),
            FetchOp::Xor => arith(self, ArithOp::Xor, old, operand, ArithSema::Exact),
            FetchOp::Nand => {
                let and = arith(self, ArithOp::And, old, operand, ArithSema::Exact);
                self.value(
                    e,
                    place.ty.clone(),
                    ValueKind::Unary {
                        op: UnaryArithOp::Not,
                        operand: Box::new(and),
                        semantics: ArithSema::Exact,
                    },
                )
            }
            FetchOp::Min | FetchOp::Max => {
                let keep_old = self.value(
                    e,
                    Type::Bool,
                    ValueKind::Compare {
                        op: if matches!(op, FetchOp::Min) {
                            CompareOp::Lt
                        } else {
                            CompareOp::Gt
                        },
                        left: Box::new(old.clone()),
                        right: Box::new(operand.clone()),
                        exceptions: None,
                        reason: None,
                    },
                );
                self.value(
                    e,
                    place.ty.clone(),
                    ValueKind::Conditional {
                        condition: Box::new(keep_old),
                        then_value: Box::new(old),
                        else_value: Box::new(operand),
                    },
                )
            }
        })
    }

    fn memory_order(&mut self, order: &Expr) -> Result<MemoryOrder, ResolveError> {
        let value = self.expr(order)?;
        if let Some(ordering) = constant_integer(&value)
            .and_then(|value| u64::try_from(value).ok())
            .and_then(MemoryOrder::from_c)
        {
            return Ok(ordering);
        }
        Ok(MemoryOrder::Dynamic(Box::new(self.enum_integer(value))))
    }
}

fn constant_integer(value: &Value) -> Option<BigInt> {
    match &value.node.value {
        ValueKind::Constant(Number::Integer(value)) => Some(value.clone().into()),
        ValueKind::Constant(Number::SignedInteger(value)) => Some(value.clone()),
        ValueKind::Constant(Number::Bool(value)) => Some(u8::from(*value).into()),
        _ => super::fold::integer(value),
    }
}
