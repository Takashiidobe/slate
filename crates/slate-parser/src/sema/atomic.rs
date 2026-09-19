use super::expression::Lowerer;
use super::numeric::ResolveError;
use crate::ast::{Expr, ExprKind, Span};
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
    CompareExchange(CompareExchangeSource),
    Fetch {
        op: FetchOp,
        postfix: bool,
        byte_offsets: bool,
    },
    TestAndSet,
    Clear,
    Fence(FenceScope),
    Sync(SyncBuiltin),
    LockFree(LockFreeQuery),
}

#[derive(Clone, Copy)]
pub(super) enum CompareExchangeSource {
    C11 { weak: bool },
    Gnu { generic: bool },
}

// the legacy __sync builtins take no ordering argument and ignore any trailing ones
#[derive(Clone, Copy)]
pub(super) enum SyncBuiltin {
    Fetch { op: FetchOp, postfix: bool },
    CompareAndSwap(CompareExchangeForm),
    LockTestAndSet,
    LockRelease,
    Swap,
    Synchronize,
}

#[derive(Clone, Copy)]
pub(super) enum LockFreeQuery {
    C11,
    Always,
    Runtime,
}

#[derive(Clone, Copy)]
pub(super) enum FetchOp {
    Add,
    Sub,
    And,
    Or,
    Xor,
    Nand,
    Min { signed: Option<bool> },
    Max { signed: Option<bool> },
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
            "min" => Self::Min { signed: None },
            "max" => Self::Max { signed: None },
            _ => return None,
        })
    }
}

pub(super) fn atomic_builtin(callee: &Expr) -> Option<AtomicBuiltin> {
    let ExprKind::Identifier(name) = &callee.value else {
        return None;
    };
    if let Some(operation) = name.strip_prefix("__sync_") {
        return sync_builtin(operation).map(AtomicBuiltin::Sync);
    }
    if let Some(operation) = name.strip_prefix("__c11_atomic_") {
        return Some(match operation {
            "init" => AtomicBuiltin::Init,
            "is_lock_free" => AtomicBuiltin::LockFree(LockFreeQuery::C11),
            "load" => AtomicBuiltin::Load { generic: false },
            "store" => AtomicBuiltin::Store { generic: false },
            "exchange" => AtomicBuiltin::Exchange { generic: false },
            "compare_exchange_strong" => {
                AtomicBuiltin::CompareExchange(CompareExchangeSource::C11 { weak: false })
            }
            "compare_exchange_weak" => {
                AtomicBuiltin::CompareExchange(CompareExchangeSource::C11 { weak: true })
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
            AtomicBuiltin::CompareExchange(CompareExchangeSource::Gnu { generic: false })
        }
        "compare_exchange" => {
            AtomicBuiltin::CompareExchange(CompareExchangeSource::Gnu { generic: true })
        }
        "always_lock_free" => AtomicBuiltin::LockFree(LockFreeQuery::Always),
        "is_lock_free" => AtomicBuiltin::LockFree(LockFreeQuery::Runtime),
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

fn sync_builtin(operation: &str) -> Option<SyncBuiltin> {
    Some(match operation {
        "bool_compare_and_swap" => SyncBuiltin::CompareAndSwap(CompareExchangeForm::Success),
        "val_compare_and_swap" => SyncBuiltin::CompareAndSwap(CompareExchangeForm::Old),
        "lock_test_and_set" => SyncBuiltin::LockTestAndSet,
        "lock_release" => SyncBuiltin::LockRelease,
        "swap" => SyncBuiltin::Swap,
        "synchronize" => SyncBuiltin::Synchronize,
        _ => match operation.strip_prefix("fetch_and_") {
            Some(op) => SyncBuiltin::Fetch {
                op: match op {
                    "min" => FetchOp::Min { signed: Some(true) },
                    "max" => FetchOp::Max { signed: Some(true) },
                    "umin" => FetchOp::Min {
                        signed: Some(false),
                    },
                    "umax" => FetchOp::Max {
                        signed: Some(false),
                    },
                    op => FetchOp::parse(op)
                        .filter(|op| !matches!(op, FetchOp::Min { .. } | FetchOp::Max { .. }))?,
                },
                postfix: true,
            },
            None => SyncBuiltin::Fetch {
                op: FetchOp::parse(operation.strip_suffix("_and_fetch")?)
                    .filter(|op| !matches!(op, FetchOp::Min { .. } | FetchOp::Max { .. }))?,
                postfix: false,
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
            let metadata = self.module.metadata.entry(e.id).or_default();
            metadata.push(("c_builtin".into(), name.clone()));
            if let Some(origin) = &callee.macro_origin {
                let mut innermost = origin.as_ref();
                while let Some(parent) = &innermost.parent {
                    innermost = parent;
                }
                metadata.push(("c_macro".into(), innermost.name.clone()));
            }
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
                AtomicBuiltin::CompareExchange(CompareExchangeSource::C11 { weak }),
                [object, expected, desired, success, failure],
            ) => {
                let place = self.atomic_object(object)?;
                let expected = self.expr(expected)?;
                self.pointee(&expected.ty)?;
                let desired = self.desired(desired, &place, false)?;
                let weak = if weak {
                    Weakness::Weak
                } else {
                    Weakness::Strong
                };
                self.compare_exchange(
                    e,
                    place,
                    [expected, desired],
                    [success, failure],
                    weak,
                    CompareExchangeForm::WriteBack,
                )
            }
            (
                AtomicBuiltin::CompareExchange(CompareExchangeSource::Gnu { generic }),
                [object, expected, desired, weak, success, failure],
            ) => {
                let place = self.atomic_object(object)?;
                let expected = self.expr(expected)?;
                self.pointee(&expected.ty)?;
                let desired = self.desired(desired, &place, generic)?;
                let weak = self.expr(weak)?;
                let weak = match constant_integer(&weak) {
                    Some(value) if value == BigInt::from(0) => Weakness::Strong,
                    Some(_) => Weakness::Weak,
                    None => Weakness::Dynamic(Box::new(weak)),
                };
                self.compare_exchange(
                    e,
                    place,
                    [expected, desired],
                    [success, failure],
                    weak,
                    CompareExchangeForm::WriteBack,
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
            (AtomicBuiltin::Sync(builtin), arguments) => self.sync_builtin(e, builtin, arguments),
            (AtomicBuiltin::LockFree(query), arguments) => self.lock_free(e, query, arguments),
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
        place: Place,
        [expected, desired]: [Value; 2],
        [success, failure]: [&Expr; 2],
        weak: Weakness,
        form: CompareExchangeForm,
    ) -> Result<Value, ResolveError> {
        let success = self.memory_order(success)?;
        let failure = self.memory_order(failure)?;
        Ok(self.exchange_node(
            e,
            place,
            [expected, desired],
            [success, failure],
            weak,
            form,
        ))
    }

    fn exchange_node(
        &self,
        e: &Expr,
        place: Place,
        [expected, desired]: [Value; 2],
        [success, failure]: [MemoryOrder; 2],
        weak: Weakness,
        form: CompareExchangeForm,
    ) -> Value {
        let ty = match form {
            CompareExchangeForm::WriteBack | CompareExchangeForm::Success => Type::Bool,
            CompareExchangeForm::Old => place.ty.clone(),
        };
        self.value(
            e,
            ty,
            ValueKind::CompareExchange {
                place,
                expected: Box::new(expected),
                desired: Box::new(desired),
                success,
                failure,
                weak,
                form,
            },
        )
    }

    fn sync_builtin(
        &mut self,
        e: &Expr,
        builtin: SyncBuiltin,
        arguments: &[Expr],
    ) -> Result<Value, ResolveError> {
        match (builtin, arguments) {
            (SyncBuiltin::Fetch { op, postfix }, [object, operand, ..]) => {
                let place = self.atomic_object(object)?;
                let computation = self.fetch_computation(&place, op, operand, true)?;
                Ok(self.atomic_update(e, place, computation, postfix, MemoryOrder::SeqCst))
            }
            (SyncBuiltin::CompareAndSwap(form), [object, expected, desired, ..]) => {
                let place = self.atomic_object(object)?;
                let expected = self.atomic_operand(expected, &place)?;
                let desired = self.atomic_operand(desired, &place)?;
                Ok(self.exchange_node(
                    e,
                    place,
                    [expected, desired],
                    [MemoryOrder::SeqCst, MemoryOrder::SeqCst],
                    Weakness::Strong,
                    form,
                ))
            }
            // gcc documents lock_test_and_set as an acquire barrier only; clang emits seq_cst
            (SyncBuiltin::LockTestAndSet | SyncBuiltin::Swap, [object, desired, ..]) => {
                let place = self.atomic_object(object)?;
                let value = self.atomic_operand(desired, &place)?;
                let ordering = if matches!(builtin, SyncBuiltin::LockTestAndSet) {
                    MemoryOrder::Acquire
                } else {
                    MemoryOrder::SeqCst
                };
                Ok(self.atomic_update(e, place, value, true, ordering))
            }
            (SyncBuiltin::LockRelease, [object, ..]) => {
                let place = self.atomic_object(object)?;
                let zero = self.flag_constant(object, place.ty.clone(), 0);
                let store = self.store(e, place, zero, Some(MemoryOrder::Release));
                Ok(self.discarded(e, store))
            }
            (SyncBuiltin::Synchronize, _) => Ok(self.value(
                e,
                Type::Void,
                ValueKind::Fence {
                    ordering: MemoryOrder::SeqCst,
                    scope: FenceScope::Thread,
                },
            )),
            _ => Err(ResolveError::Unsupported("atomic builtin argument count")),
        }
    }

    // mirrors clang's constant evaluator; undecidable queries fall back to libatomic
    fn lock_free(
        &mut self,
        e: &Expr,
        query: LockFreeQuery,
        arguments: &[Expr],
    ) -> Result<Value, ResolveError> {
        let (size, pointer) = match (query, arguments) {
            (LockFreeQuery::C11, [size]) => (size, None),
            (LockFreeQuery::Always | LockFreeQuery::Runtime, [size, pointer]) => {
                (size, Some(pointer))
            }
            _ => return Err(ResolveError::Unsupported("atomic builtin argument count")),
        };
        let size = self.expr(size)?;
        let pointer = pointer.map(|pointer| self.expr(pointer)).transpose()?;
        let known = constant_integer(&size)
            .and_then(|size| u64::try_from(size).ok())
            .filter(|size| {
                size.is_power_of_two() && *size <= self.context.target.max_atomic_inline_bytes()
            })
            .is_some_and(|size| {
                size == 1
                    || pointer
                        .as_ref()
                        .is_none_or(|pointer| self.lock_free_pointer(pointer, size))
            });
        if known || matches!(query, LockFreeQuery::Always) {
            return Ok(self.value(e, Type::Bool, ValueKind::Constant(Number::Bool(known))));
        }
        let size_type = Type::integer(self.context.target.pointer_width, false);
        let size = self.convert(size, size_type.clone(), ConversionReason::Arg)?;
        let void_pointer = self.qualified_pointer(
            Type::Void,
            true,
            Access {
                volatile: true,
                atomic: false,
            },
        );
        let pointer = match pointer {
            Some(pointer) => self.convert(pointer, void_pointer.clone(), ConversionReason::Arg)?,
            None => self.value(e, void_pointer.clone(), ValueKind::Null),
        };
        let signature = Type::Function {
            return_type: Some(Box::new(Type::Bool)),
            parameters: vec![size_type, void_pointer],
            variadic: false,
            prototyped: true,
        };
        let callee = self.libatomic_is_lock_free(e, &signature)?;
        let abi = self.abi_signature(&signature, None)?;
        Ok(self.value(
            e,
            Type::Bool,
            ValueKind::Call {
                callee: Callee::Direct(callee),
                signature,
                abi,
                arguments: vec![size, pointer],
            },
        ))
    }

    fn lock_free_pointer(&self, pointer: &Value, size: u64) -> bool {
        if matches!(pointer.node.value, ValueKind::Null)
            || constant_integer(pointer).is_some_and(|value| value == BigInt::from(0))
        {
            return true;
        }
        let Type::Pointer { pointee, .. } = &pointer.ty else {
            return false;
        };
        self.types
            .storage((**pointee).clone())
            .is_ok_and(|layout| u64::from(layout.alignment_bytes) >= size)
    }

    fn libatomic_is_lock_free(
        &mut self,
        e: &Expr,
        signature: &Type,
    ) -> Result<BindingId, ResolveError> {
        const NAME: &str = "__atomic_is_lock_free";
        if let Some(function) = self.module.functions.iter().find(|f| f.name == NAME) {
            return Ok(function.value.id);
        }
        let Type::Function { parameters, .. } = signature else {
            return Err(ResolveError::Unsupported("libatomic signature"));
        };
        let fixed = parameters
            .iter()
            .map(|ty| {
                let id = self.fresh();
                Span::new(
                    Parameter {
                        id,
                        name: None,
                        ty: ty.clone(),
                        restrict: false,
                        is_const: false,
                        array: None,
                    },
                    e.spelling,
                    e.expansion,
                )
                .with_provenance(e.provenance)
            })
            .collect();
        let id = self.fresh();
        let abi = self.abi_signature(signature, None)?;
        let function = Function {
            id,
            name: NAME.into(),
            parameters: Parameters::Prototype {
                fixed,
                variadic: false,
            },
            return_type: Some(Type::Bool),
            abi,
            linkage: Linkage::External,
            symbol: SymbolAttributes::default(),
            semantics: FunctionSemantics::default(),
            body: None,
            fallthrough: None,
        };
        self.module
            .functions
            .push(Span::new(function, e.spelling, e.expansion).with_provenance(e.provenance));
        Ok(id)
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
        let floating = match &place.ty {
            Type::Numeric(NumericType::Integer { .. }) => false,
            Type::Numeric(NumericType::Float(_)) if matches!(op, FetchOp::Add | FetchOp::Sub) => {
                true
            }
            _ => {
                return Err(ResolveError::Unsupported(
                    "atomic arithmetic on non-integer",
                ));
            }
        };
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
        let wrap = if floating {
            ArithSema::Floating(self.context.floating)
        } else {
            ArithSema::Integer {
                overflow: Overflow::Wrap,
            }
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
            FetchOp::Min { signed } | FetchOp::Max { signed } => {
                let (left, right) = match (signed, &place.ty) {
                    (
                        Some(signed),
                        Type::Numeric(NumericType::Integer {
                            width,
                            signed: declared,
                            ..
                        }),
                    ) if signed != *declared => {
                        let compared = Type::integer(*width, signed);
                        (
                            self.convert(old.clone(), compared.clone(), ConversionReason::Arg)?,
                            self.convert(operand.clone(), compared, ConversionReason::Arg)?,
                        )
                    }
                    _ => (old.clone(), operand.clone()),
                };
                let keep_old = self.value(
                    e,
                    Type::Bool,
                    ValueKind::Compare {
                        op: if matches!(op, FetchOp::Min { .. }) {
                            CompareOp::Lt
                        } else {
                            CompareOp::Gt
                        },
                        left: Box::new(left),
                        right: Box::new(right),
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
