use super::ctype::convert::CastKind;
use super::ctype::{CTypeKind, QualType};
use super::expression::Lowerer;
use super::numeric::ResolveError;
use super::operand::{Lvalue, Operand};
use super::typer::Slot;
use crate::ast::{Expr, ExprKind, Span};
use crate::compiler_args::CompilerFlavor;
use crate::ir::*;
use num_bigint::BigInt;

#[derive(Clone, Copy)]
pub(super) struct AtomicBuiltin {
    operation: AtomicOperation,
    scoped: bool,
}

#[derive(Clone, Copy)]
enum AtomicOperation {
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

struct FetchObject {
    place: Lvalue,
    declared: Option<QualType>,
}

struct Exchange {
    weak: Weakness,
    form: CompareExchangeForm,
    scope: SyncScope,
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
    FloatExtremum(ArithOp),
    UIncWrap,
    UDecWrap,
}

#[derive(Clone, Copy, PartialEq, Eq)]
enum FetchSpelling {
    Sync,
    C11,
    Gnu { fetch: bool },
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
            "fminimum" => Self::FloatExtremum(ArithOp::Minimum),
            "fmaximum" => Self::FloatExtremum(ArithOp::Maximum),
            "fminimum_num" => Self::FloatExtremum(ArithOp::MinimumNum),
            "fmaximum_num" => Self::FloatExtremum(ArithOp::MaximumNum),
            "uinc" => Self::UIncWrap,
            "udec" => Self::UDecWrap,
            _ => return None,
        })
    }

    fn spelled_as(self, spelling: FetchSpelling) -> Option<Self> {
        let spelled = match self {
            Self::Add | Self::Sub | Self::And | Self::Or | Self::Xor | Self::Nand => true,
            Self::Min { .. } | Self::Max { .. } => spelling != FetchSpelling::Sync,
            Self::FloatExtremum(_) | Self::UIncWrap | Self::UDecWrap => {
                spelling == FetchSpelling::Gnu { fetch: true }
            }
        };
        spelled.then_some(self)
    }
}

pub(super) fn atomic_builtin(callee: &Expr) -> Option<AtomicBuiltin> {
    let ExprKind::Identifier(name) = &callee.value else {
        return None;
    };
    if let Some(operation) = name.strip_prefix("__scoped_atomic_") {
        return Some(AtomicBuiltin {
            operation: scoped_operation(operation)?,
            scoped: true,
        });
    }
    Some(AtomicBuiltin {
        operation: unscoped_operation(name)?,
        scoped: false,
    })
}

fn scoped_operation(operation: &str) -> Option<AtomicOperation> {
    let operation = gnu_operation(operation)?;
    matches!(
        operation,
        AtomicOperation::Load { .. }
            | AtomicOperation::Store { .. }
            | AtomicOperation::Exchange { .. }
            | AtomicOperation::CompareExchange(_)
            | AtomicOperation::Fetch { .. }
            | AtomicOperation::Fence(FenceScope::Thread)
    )
    .then_some(operation)
}

fn unscoped_operation(name: &str) -> Option<AtomicOperation> {
    if let Some(operation) = name.strip_prefix("__sync_") {
        return sync_builtin(operation).map(AtomicOperation::Sync);
    }
    if let Some(operation) = name.strip_prefix("__c11_atomic_") {
        return Some(match operation {
            "init" => AtomicOperation::Init,
            "is_lock_free" => AtomicOperation::LockFree(LockFreeQuery::C11),
            "load" => AtomicOperation::Load { generic: false },
            "store" => AtomicOperation::Store { generic: false },
            "exchange" => AtomicOperation::Exchange { generic: false },
            "compare_exchange_strong" => {
                AtomicOperation::CompareExchange(CompareExchangeSource::C11 { weak: false })
            }
            "compare_exchange_weak" => {
                AtomicOperation::CompareExchange(CompareExchangeSource::C11 { weak: true })
            }
            "thread_fence" => AtomicOperation::Fence(FenceScope::Thread),
            "signal_fence" => AtomicOperation::Fence(FenceScope::Signal),
            _ => AtomicOperation::Fetch {
                op: FetchOp::parse(operation.strip_prefix("fetch_")?)?
                    .spelled_as(FetchSpelling::C11)?,
                postfix: true,
                byte_offsets: false,
            },
        });
    }
    gnu_operation(name.strip_prefix("__atomic_")?)
}

fn gnu_operation(operation: &str) -> Option<AtomicOperation> {
    Some(match operation {
        "load_n" => AtomicOperation::Load { generic: false },
        "load" => AtomicOperation::Load { generic: true },
        "store_n" => AtomicOperation::Store { generic: false },
        "store" => AtomicOperation::Store { generic: true },
        "exchange_n" => AtomicOperation::Exchange { generic: false },
        "exchange" => AtomicOperation::Exchange { generic: true },
        "compare_exchange_n" => {
            AtomicOperation::CompareExchange(CompareExchangeSource::Gnu { generic: false })
        }
        "compare_exchange" => {
            AtomicOperation::CompareExchange(CompareExchangeSource::Gnu { generic: true })
        }
        "always_lock_free" => AtomicOperation::LockFree(LockFreeQuery::Always),
        "is_lock_free" => AtomicOperation::LockFree(LockFreeQuery::Runtime),
        "test_and_set" => AtomicOperation::TestAndSet,
        "clear" => AtomicOperation::Clear,
        "thread_fence" => AtomicOperation::Fence(FenceScope::Thread),
        "signal_fence" => AtomicOperation::Fence(FenceScope::Signal),
        _ => match operation.strip_prefix("fetch_") {
            Some(op) => AtomicOperation::Fetch {
                op: FetchOp::parse(op)?.spelled_as(FetchSpelling::Gnu { fetch: true })?,
                postfix: true,
                byte_offsets: true,
            },
            None => AtomicOperation::Fetch {
                op: FetchOp::parse(operation.strip_suffix("_fetch")?)?
                    .spelled_as(FetchSpelling::Gnu { fetch: false })?,
                postfix: false,
                byte_offsets: true,
            },
        },
    })
}

fn sync_builtin(operation: &str) -> Option<SyncBuiltin> {
    let (operation, sized) = match strip_size(operation) {
        Some(operation) => (operation, true),
        None => (operation, false),
    };
    let builtin = sync_operation(operation)?;
    (!sized || builtin.sized()).then_some(builtin)
}

// gcc's legacy spellings name the width they were resolved for; clang ignores
// the suffix and takes the width from the pointee
fn strip_size(operation: &str) -> Option<&str> {
    ["_1", "_2", "_4", "_8", "_16"]
        .iter()
        .find_map(|suffix| operation.strip_suffix(suffix))
}

fn sync_operation(operation: &str) -> Option<SyncBuiltin> {
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
                    op => FetchOp::parse(op)?.spelled_as(FetchSpelling::Sync)?,
                },
                postfix: true,
            },
            None => SyncBuiltin::Fetch {
                op: FetchOp::parse(operation.strip_suffix("_and_fetch")?)?
                    .spelled_as(FetchSpelling::Sync)?,
                postfix: false,
            },
        },
    })
}

#[derive(Default)]
pub(super) struct AtomicOperands<'e> {
    pub objects: Vec<&'e Expr>,
    pub pointers: Vec<&'e Expr>,
    pub values: Vec<(&'e Expr, &'e Expr)>,
    pub fetch: Option<(FetchOp, &'e Expr, &'e Expr)>,
    pub lock_free: Option<(&'e Expr, Option<&'e Expr>)>,
}

// the operand checks atomic lowering relies on: the loaded object's type and the fetch operand
pub(super) fn fetch_rule(
    object: &Type,
    op: FetchOp,
    operand: &Type,
    flavor: CompilerFlavor,
) -> Result<(), ResolveError> {
    if matches!(object, Type::Pointer { .. }) {
        let integer = matches!(operand, Type::Numeric(NumericType::Integer { .. }));
        return match op {
            FetchOp::Add | FetchOp::Sub
                if integer
                    || flavor == CompilerFlavor::Clang
                        && matches!(operand, Type::Numeric(NumericType::Float(_))) =>
            {
                Ok(())
            }
            FetchOp::Add | FetchOp::Sub => {
                Err(ResolveError::Rejected("noninteger atomic pointer offset"))
            }
            FetchOp::And | FetchOp::Or | FetchOp::Xor | FetchOp::Nand
                if integer && flavor == CompilerFlavor::Gcc =>
            {
                Ok(())
            }
            _ => Err(ResolveError::Rejected(
                "atomic bitwise operation on pointer",
            )),
        };
    }
    match (object, op) {
        (
            Type::Numeric(NumericType::Float(_)),
            FetchOp::Add
            | FetchOp::Sub
            | FetchOp::Min { signed: None }
            | FetchOp::Max { signed: None }
            | FetchOp::FloatExtremum(_),
        ) => Ok(()),
        (_, FetchOp::FloatExtremum(_)) => Err(ResolveError::Rejected(
            "atomic floating extremum on a non-floating object",
        )),
        (Type::Numeric(NumericType::Integer { .. }) | Type::Bool, _) => Ok(()),
        _ => Err(ResolveError::Rejected("atomic arithmetic on non-integer")),
    }
}

pub(super) enum AtomicResult<'e> {
    Void,
    Bool,
    Object(&'e Expr),
    Fetched(&'e Expr),
    Flag(&'e Expr),
}

impl AtomicBuiltin {
    pub(super) fn operands(self, arguments: &[Expr]) -> AtomicOperands<'_> {
        let arguments = match arguments.split_last() {
            Some((_, rest)) if self.scoped => rest,
            _ => arguments,
        };
        let mut operands = AtomicOperands::default();
        match (self.operation, arguments) {
            (AtomicOperation::Init, [object, desired])
            | (AtomicOperation::Exchange { generic: false }, [object, desired, _])
            | (AtomicOperation::Store { generic: false }, [object, desired, _])
            | (
                AtomicOperation::Sync(SyncBuiltin::LockTestAndSet | SyncBuiltin::Swap),
                [object, desired, ..],
            ) => {
                operands.objects.push(object);
                operands.values.push((object, desired));
            }
            (AtomicOperation::Load { generic: false }, [object, _])
            | (AtomicOperation::Sync(SyncBuiltin::LockRelease), [object, ..]) => {
                operands.objects.push(object);
            }
            (AtomicOperation::Load { generic: true }, [object, other, _])
            | (AtomicOperation::Store { generic: true }, [object, other, _]) => {
                operands.objects.extend([object, other]);
            }
            (AtomicOperation::Exchange { generic: true }, [object, desired, result, _]) => {
                operands.objects.extend([object, desired, result]);
            }
            (
                AtomicOperation::CompareExchange(CompareExchangeSource::C11 { .. }),
                [object, expected, desired, _, _],
            )
            | (
                AtomicOperation::CompareExchange(CompareExchangeSource::Gnu { generic: false }),
                [object, expected, desired, _, _, _],
            ) => {
                operands.objects.push(object);
                operands.pointers.push(expected);
                operands.values.push((object, desired));
            }
            (
                AtomicOperation::CompareExchange(CompareExchangeSource::Gnu { generic: true }),
                [object, expected, desired, _, _, _],
            ) => {
                operands.objects.extend([object, desired]);
                operands.pointers.push(expected);
            }
            (AtomicOperation::Fetch { op, .. }, [object, operand, _])
            | (AtomicOperation::Sync(SyncBuiltin::Fetch { op, .. }), [object, operand, ..]) => {
                operands.objects.push(object);
                operands.fetch = Some((op, object, operand));
            }
            (
                AtomicOperation::Sync(SyncBuiltin::CompareAndSwap(_)),
                [object, expected, desired, ..],
            ) => {
                operands.objects.push(object);
                operands
                    .values
                    .extend([(object, expected), (object, desired)]);
            }
            (AtomicOperation::TestAndSet | AtomicOperation::Clear, [object, _]) => {
                operands.pointers.push(object);
            }
            (AtomicOperation::LockFree(LockFreeQuery::C11), [size]) => {
                operands.lock_free = Some((size, None));
            }
            (
                AtomicOperation::LockFree(LockFreeQuery::Always | LockFreeQuery::Runtime),
                [size, pointer],
            ) => {
                operands.lock_free = Some((size, Some(pointer)));
            }
            _ => {}
        }
        operands
    }

    pub(super) fn result(self, arguments: &[Expr]) -> Option<AtomicResult<'_>> {
        let arguments = match arguments.split_last() {
            Some((_, rest)) if self.scoped => rest,
            _ => arguments,
        };
        Some(match (self.operation, arguments) {
            (AtomicOperation::Init, [_, _])
            | (AtomicOperation::Load { generic: true }, [_, _, _])
            | (AtomicOperation::Store { .. }, [_, _, _])
            | (AtomicOperation::Exchange { generic: true }, [_, _, _, _])
            | (AtomicOperation::Clear, [_, _])
            | (AtomicOperation::Fence(_), [_])
            | (AtomicOperation::Sync(SyncBuiltin::LockRelease), [_, ..])
            | (AtomicOperation::Sync(SyncBuiltin::Synchronize), _) => AtomicResult::Void,
            (AtomicOperation::Fetch { .. }, [object, _, _])
            | (AtomicOperation::Sync(SyncBuiltin::Fetch { .. }), [object, _, ..]) => {
                AtomicResult::Fetched(object)
            }
            (AtomicOperation::Load { generic: false }, [object, _])
            | (AtomicOperation::Exchange { generic: false }, [object, _, _])
            | (
                AtomicOperation::Sync(SyncBuiltin::LockTestAndSet | SyncBuiltin::Swap),
                [object, _, ..],
            )
            | (
                AtomicOperation::Sync(SyncBuiltin::CompareAndSwap(CompareExchangeForm::Old)),
                [object, _, _, ..],
            ) => AtomicResult::Object(object),
            (
                AtomicOperation::CompareExchange(CompareExchangeSource::C11 { .. }),
                [_, _, _, _, _],
            )
            | (
                AtomicOperation::CompareExchange(CompareExchangeSource::Gnu { .. }),
                [_, _, _, _, _, _],
            )
            | (AtomicOperation::Sync(SyncBuiltin::CompareAndSwap(_)), [_, _, _, ..])
            | (AtomicOperation::LockFree(LockFreeQuery::C11), [_])
            | (AtomicOperation::LockFree(LockFreeQuery::Always | LockFreeQuery::Runtime), [_, _]) => {
                AtomicResult::Bool
            }
            (AtomicOperation::TestAndSet, [object, _]) => AtomicResult::Flag(object),
            _ => return None,
        })
    }
}

impl SyncBuiltin {
    fn sized(self) -> bool {
        match self {
            Self::Fetch { op, .. } => matches!(
                op,
                FetchOp::Add
                    | FetchOp::Sub
                    | FetchOp::And
                    | FetchOp::Or
                    | FetchOp::Xor
                    | FetchOp::Nand
            ),
            Self::CompareAndSwap(_) | Self::LockTestAndSet | Self::LockRelease | Self::Swap => true,
            Self::Synchronize => false,
        }
    }
}

impl Lowerer {
    pub(super) fn atomic_builtin(
        &mut self,
        e: &Expr,
        callee: &Expr,
        builtin: AtomicBuiltin,
        arguments: &[Expr],
    ) -> Result<Operand, ResolveError> {
        let mut metadata = Vec::new();
        if let ExprKind::Identifier(name) = &callee.value {
            metadata.push(("c_builtin".into(), name.clone()));
            if let Some(origin) = &callee.macro_origin {
                let name = origin
                    .inner
                    .as_ref()
                    .map_or(&origin.name, |inner| &inner.name);
                metadata.push(("c_macro".into(), name.to_string()));
            }
        }
        let value = self.atomic_operation(e, builtin, arguments)?;
        let operation = match &value.node.value {
            ValueKind::Sequence { left, .. } => &left.node,
            _ => &value.node,
        };
        self.module.annotate(operation, metadata);
        Ok(value)
    }

    fn atomic_operation(
        &mut self,
        e: &Expr,
        builtin: AtomicBuiltin,
        arguments: &[Expr],
    ) -> Result<Operand, ResolveError> {
        let (arguments, scope) = match arguments.split_last() {
            Some((scope, rest)) if builtin.scoped => (rest, self.sync_scope(scope)?),
            _ => (arguments, SyncScope::System),
        };
        match (builtin.operation, arguments) {
            (AtomicOperation::Init, [object, desired]) => {
                let place = self.atomic_object(object)?;
                let value = self.atomic_operand(desired, &place)?;
                let store = self.store(e, place, value, None);
                Ok(self.discarded(e, store))
            }
            (AtomicOperation::Load { generic: false }, [object, order]) => {
                let place = self.atomic_object(object)?;
                let ordering = self.atomicity(order, &scope)?;
                Ok(self.atomic_read(e, place, Some(ordering)))
            }
            (AtomicOperation::Load { generic: true }, [object, result, order]) => {
                let place = self.atomic_object(object)?;
                let ordering = self.atomicity(order, &scope)?;
                let value = self.atomic_read(e, place, Some(ordering));
                let target = self.atomic_object(result)?;
                let ordering = target.implicit_ordering();
                let store = self.store(result, target, value, ordering);
                Ok(self.discarded(e, store))
            }
            (AtomicOperation::Store { generic }, [object, desired, order]) => {
                let place = self.atomic_object(object)?;
                let value = self.desired(desired, &place, generic)?;
                let ordering = self.atomicity(order, &scope)?;
                let store = self.store(e, place, value, Some(ordering));
                Ok(self.discarded(e, store))
            }
            (AtomicOperation::Exchange { generic: false }, [object, desired, order]) => {
                let place = self.atomic_object(object)?;
                let value = self.atomic_operand(desired, &place)?;
                let ordering = self.atomicity(order, &scope)?;
                Ok(self.atomic_update(e, place, value, true, ordering))
            }
            (AtomicOperation::Exchange { generic: true }, [object, desired, result, order]) => {
                let place = self.atomic_object(object)?;
                let value = self.desired(desired, &place, true)?;
                let ordering = self.atomicity(order, &scope)?;
                let old = self.atomic_update(e, place, value, true, ordering);
                let target = self.atomic_object(result)?;
                let ordering = target.implicit_ordering();
                let store = self.store(result, target, old, ordering);
                Ok(self.discarded(e, store))
            }
            (
                AtomicOperation::CompareExchange(CompareExchangeSource::C11 { weak }),
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
                    Exchange {
                        weak,
                        form: CompareExchangeForm::WriteBack,
                        scope,
                    },
                )
            }
            (
                AtomicOperation::CompareExchange(CompareExchangeSource::Gnu { generic }),
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
                    None => Weakness::Dynamic(Box::new(weak.value)),
                };
                self.compare_exchange(
                    e,
                    place,
                    [expected, desired],
                    [success, failure],
                    Exchange {
                        weak,
                        form: CompareExchangeForm::WriteBack,
                        scope,
                    },
                )
            }
            (
                AtomicOperation::Fetch {
                    op,
                    postfix,
                    byte_offsets,
                },
                [object, operand, order],
            ) => {
                let object = self.arithmetic_object(object)?;
                let computation = self.fetch_computation(e, &object, op, operand, byte_offsets)?;
                let ordering = self.atomicity(order, &scope)?;
                let old = self.atomic_update(e, object.place, computation, postfix, ordering);
                self.fetch_result(old, object.declared)
            }
            (AtomicOperation::Sync(builtin), arguments) => self.sync_builtin(e, builtin, arguments),
            (AtomicOperation::LockFree(query), arguments) => self.lock_free(e, query, arguments),
            (AtomicOperation::TestAndSet, [object, order]) => {
                let place = self.flag_object(object)?;
                let set = self.flag_constant(object, place.c, 1);
                let ordering = self.atomicity(order, &scope)?;
                let old = self.atomic_update(e, place, set, true, ordering);
                if old.ty == Type::Bool {
                    return Ok(old);
                }
                let zero = self.flag_constant(object, old.c, 0);
                Ok(self.builtin_operand(
                    e,
                    CTypeKind::Bool,
                    ValueKind::Compare {
                        op: CompareOp::Ne,
                        left: Box::new(old.value),
                        right: Box::new(zero.value),
                        exceptions: None,
                        reason: None,
                    },
                ))
            }
            (AtomicOperation::Clear, [object, order]) => {
                let place = self.flag_object(object)?;
                let clear = self.flag_constant(object, place.c, 0);
                let ordering = self.atomicity(order, &scope)?;
                let store = self.store(e, place, clear, Some(ordering));
                Ok(self.discarded(e, store))
            }
            (AtomicOperation::Fence(fence), [order]) => {
                let ordering = self.atomicity(order, &scope)?;
                Ok(self.builtin_operand(
                    e,
                    CTypeKind::Void,
                    ValueKind::Fence {
                        ordering,
                        scope: fence,
                    },
                ))
            }
            _ => Err(ResolveError::Internal("atomic builtin argument count")),
        }
    }

    fn atomic_object(&mut self, object: &Expr) -> Result<Lvalue, ResolveError> {
        let pointer = self.expr(object)?;
        let place = self.deref(pointer)?;
        if matches!(place.ty, Type::Void | Type::Function { .. }) {
            return Err(ResolveError::Internal(
                "atomic builtin on non-object pointer",
            ));
        }
        Ok(place)
    }

    // clang computes a `_Bool` fetch in the object's storage byte (`atomicrmw
    // add ptr, i8`), which is the only way a byte outside {0, 1} can be stored
    fn arithmetic_object(&mut self, object: &Expr) -> Result<FetchObject, ResolveError> {
        let mut place = self.atomic_object(object)?;
        if place.ty != Type::Bool {
            return Ok(FetchObject {
                place,
                declared: None,
            });
        }
        let declared = place.c;
        place.c = self.types.ctypes.qual(CTypeKind::UChar);
        place.place.ty = self.types.ir_type(place.c);
        Ok(FetchObject {
            place,
            declared: Some(declared),
        })
    }

    fn fetch_result(
        &mut self,
        old: Operand,
        declared: Option<QualType>,
    ) -> Result<Operand, ResolveError> {
        match declared {
            Some(c) => self.apply_conversion(CastKind::Arithmetic, old, c, ConversionReason::Arg),
            None => Ok(old),
        }
    }

    fn flag_object(&mut self, object: &Expr) -> Result<Lvalue, ResolveError> {
        let pointer = self.expr(object)?;
        let mut place = self.deref(pointer)?;
        if place.ty == Type::Void {
            place.c = self.types.ctypes.qual(CTypeKind::UChar);
            place.place.ty = self.types.ir_type(place.c);
        }
        Ok(place)
    }

    fn flag_constant(&mut self, e: &Expr, ty: QualType, value: u8) -> Operand {
        let number = if matches!(self.types.ctypes.canonical_kind(ty), CTypeKind::Bool) {
            Number::Bool(value != 0)
        } else {
            Number::Integer(value.into())
        };
        self.operand(e, ty, ValueKind::Constant(number))
    }

    fn atomic_operand(&mut self, operand: &Expr, place: &Lvalue) -> Result<Operand, ResolveError> {
        let value = self.expr(operand)?;
        self.convert_recorded(operand, value, place.c, ConversionReason::Arg)
    }

    fn desired(
        &mut self,
        desired: &Expr,
        place: &Lvalue,
        generic: bool,
    ) -> Result<Operand, ResolveError> {
        if !generic {
            return self.atomic_operand(desired, place);
        }
        let source = self.atomic_object(desired)?;
        let ordering = source.implicit_ordering();
        Ok(self.atomic_read(desired, source, ordering))
    }

    fn atomic_read(&mut self, e: &Expr, place: Lvalue, ordering: Option<Atomicity>) -> Operand {
        self.operand(
            e,
            place.c,
            ValueKind::Read {
                place: place.place,
                ordering,
            },
        )
    }

    fn store(
        &mut self,
        e: &Expr,
        place: Lvalue,
        value: Operand,
        ordering: Option<Atomicity>,
    ) -> Operand {
        self.operand(
            e,
            place.c,
            ValueKind::Store {
                place: place.place,
                value: Box::new(value.value),
                ordering,
            },
        )
    }

    fn discarded(&mut self, e: &Expr, value: Operand) -> Operand {
        let void = self.builtin_operand(e, CTypeKind::Void, ValueKind::Void);
        self.builtin_operand(
            e,
            CTypeKind::Void,
            ValueKind::Sequence {
                left: Box::new(value.value),
                right: Box::new(void.value),
            },
        )
    }

    fn atomic_update(
        &mut self,
        e: &Expr,
        place: Lvalue,
        computation: Operand,
        postfix: bool,
        ordering: Atomicity,
    ) -> Operand {
        self.operand(
            e,
            place.c,
            ValueKind::Update {
                place: place.place,
                computation: Box::new(computation.value),
                postfix,
                ordering: Some(ordering),
            },
        )
    }

    fn compare_exchange(
        &mut self,
        e: &Expr,
        place: Lvalue,
        [expected, desired]: [Operand; 2],
        [success, failure]: [&Expr; 2],
        exchange: Exchange,
    ) -> Result<Operand, ResolveError> {
        let success = self.memory_order(success)?;
        let failure = self.memory_order(failure)?;
        Ok(self.exchange_node(e, place, [expected, desired], [success, failure], exchange))
    }

    fn exchange_node(
        &mut self,
        e: &Expr,
        place: Lvalue,
        [expected, desired]: [Operand; 2],
        [success, failure]: [MemoryOrder; 2],
        Exchange { weak, form, scope }: Exchange,
    ) -> Operand {
        let ty = match form {
            CompareExchangeForm::WriteBack | CompareExchangeForm::Success => {
                self.types.ctypes.qual(CTypeKind::Bool)
            }
            CompareExchangeForm::Old => place.c,
        };
        self.operand(
            e,
            ty,
            ValueKind::CompareExchange {
                place: place.place,
                expected: Box::new(expected.value),
                desired: Box::new(desired.value),
                success,
                failure,
                sync_scope: scope,
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
    ) -> Result<Operand, ResolveError> {
        match (builtin, arguments) {
            (SyncBuiltin::Fetch { op, postfix }, [object, operand, ..]) => {
                let object = self.arithmetic_object(object)?;
                let computation = self.fetch_computation(e, &object, op, operand, true)?;
                let old = self.atomic_update(
                    e,
                    object.place,
                    computation,
                    postfix,
                    MemoryOrder::SeqCst.into(),
                );
                self.fetch_result(old, object.declared)
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
                    Exchange {
                        weak: Weakness::Strong,
                        form,
                        scope: SyncScope::System,
                    },
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
                Ok(self.atomic_update(e, place, value, true, ordering.into()))
            }
            (SyncBuiltin::LockRelease, [object, ..]) => {
                let place = self.atomic_object(object)?;
                let zero = self.flag_constant(object, place.c, 0);
                let store = self.store(e, place, zero, Some(MemoryOrder::Release.into()));
                Ok(self.discarded(e, store))
            }
            (SyncBuiltin::Synchronize, _) => Ok(self.builtin_operand(
                e,
                CTypeKind::Void,
                ValueKind::Fence {
                    ordering: MemoryOrder::SeqCst.into(),
                    scope: FenceScope::Thread,
                },
            )),
            _ => Err(ResolveError::Internal("atomic builtin argument count")),
        }
    }

    // mirrors clang's constant evaluator; undecidable queries fall back to libatomic
    fn lock_free(
        &mut self,
        e: &Expr,
        query: LockFreeQuery,
        arguments: &[Expr],
    ) -> Result<Operand, ResolveError> {
        let (size, pointer) = match (query, arguments) {
            (LockFreeQuery::C11, [size]) => (size, None),
            (LockFreeQuery::Always | LockFreeQuery::Runtime, [size, pointer]) => {
                (size, Some(pointer))
            }
            _ => return Err(ResolveError::Internal("atomic builtin argument count")),
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
            return Ok(self.builtin_operand(
                e,
                CTypeKind::Bool,
                ValueKind::Constant(Number::Bool(known)),
            ));
        }
        let size = self.converted(e, Slot::Argument(0), size)?;
        let void_pointer = self.types.lock_free_pointer();
        let pointer = match pointer {
            Some(pointer) => self.converted(e, Slot::Argument(1), pointer)?,
            None => self.operand(e, void_pointer, ValueKind::Null),
        };
        let signature = Type::Function {
            return_type: Some(Box::new(Type::Bool)),
            parameters: vec![size.ty.clone(), self.types.ir_type(void_pointer)],
            variadic: false,
            prototyped: true,
            convention: CallConv::C,
        };
        let callee = self.libatomic_is_lock_free(e, &signature)?;
        let abi = self.abi_signature(&signature, None)?;
        Ok(self.builtin_operand(
            e,
            CTypeKind::Bool,
            ValueKind::Call {
                callee: Callee::Direct(callee),
                signature,
                abi,
                arguments: vec![size.value, pointer.value],
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
            return Err(ResolveError::Internal("libatomic signature"));
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
                        access: Access::default(),
                        array: None,
                    },
                    e.spelling,
                    e.expansion,
                )
                .with_provenance(e.provenance)
            })
            .collect();
        let id = self.fresh();
        let abi = Some(self.abi_signature(signature, None)?);
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
        call: &Expr,
        FetchObject { place, .. }: &FetchObject,
        op: FetchOp,
        e: &Expr,
        byte_offsets: bool,
    ) -> Result<Operand, ResolveError> {
        let operand = self.expr(e)?;
        fetch_rule(&place.ty, op, &operand.ty, self.types.flavor())
            .map_err(ResolveError::checked)?;
        let old = self.operand(e, place.c, ValueKind::OldValue);
        if matches!(place.ty, Type::Pointer { .. }) && !matches!(op, FetchOp::Add | FetchOp::Sub) {
            let operand = self.converted(call, Slot::Argument(1), operand)?;
            let address = operand.c;
            let explicit = ConversionReason::Explicit;
            let old = self.apply_conversion(CastKind::PtrToInt, old, address, explicit)?;
            let (op, not) = match op {
                FetchOp::And => (ArithOp::And, false),
                FetchOp::Or => (ArithOp::Or, false),
                FetchOp::Xor => (ArithOp::Xor, false),
                FetchOp::Nand => (ArithOp::And, true),
                _ => {
                    return Err(ResolveError::Internal(
                        "atomic bitwise operation on pointer",
                    ));
                }
            };
            let mut computed = self.operand(
                e,
                address,
                ValueKind::Arith {
                    op,
                    left: Box::new(old.value),
                    right: Box::new(operand.value),
                    semantics: ArithSema::Exact,
                },
            );
            if not {
                computed = self.operand(
                    e,
                    address,
                    ValueKind::Unary {
                        op: UnaryArithOp::Not,
                        operand: Box::new(computed.value),
                        semantics: ArithSema::Exact,
                    },
                );
            }
            return self.apply_conversion(CastKind::IntToPtr, computed, place.c, explicit);
        }
        if let Type::Pointer { pointee, .. } = &place.ty {
            let operand = self.converted(call, Slot::Argument(1), operand)?;
            let subtract = matches!(op, FetchOp::Sub);
            let element = if byte_offsets {
                Type::integer(8, false)
            } else {
                self.types.storage((**pointee).clone())?;
                (**pointee).clone()
            };
            return Ok(self.operand(
                e,
                place.c,
                ValueKind::PointerOffset {
                    pointer: Box::new(old.value),
                    amount: Box::new(operand.value),
                    subtract,
                    element,
                    overflow: Overflow::Wrap,
                },
            ));
        }
        let floating = match (&place.ty, op) {
            (
                Type::Numeric(NumericType::Float(_)),
                FetchOp::Add
                | FetchOp::Sub
                | FetchOp::Min { signed: None }
                | FetchOp::Max { signed: None }
                | FetchOp::FloatExtremum(_),
            ) => true,
            (Type::Numeric(NumericType::Integer { .. }), _) => false,
            _ => return Err(ResolveError::Internal("atomic arithmetic on non-integer")),
        };
        let operand = self.converted(call, Slot::Argument(1), operand)?;
        let arith = |this: &mut Self, op, left: Operand, right: Operand, semantics| {
            this.operand(
                e,
                place.c,
                ValueKind::Arith {
                    op,
                    left: Box::new(left.value),
                    right: Box::new(right.value),
                    semantics,
                },
            )
        };
        let wrap = if floating {
            self.context.floating_arith()
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
                self.operand(
                    e,
                    place.c,
                    ValueKind::Unary {
                        op: UnaryArithOp::Not,
                        operand: Box::new(and.value),
                        semantics: ArithSema::Exact,
                    },
                )
            }
            FetchOp::FloatExtremum(op) => arith(self, op, old, operand, wrap),
            FetchOp::Min { .. } | FetchOp::Max { .. } if floating => {
                let op = if matches!(op, FetchOp::Min { .. }) {
                    ArithOp::MinNum
                } else {
                    ArithOp::MaxNum
                };
                arith(self, op, old, operand, wrap)
            }
            FetchOp::UIncWrap | FetchOp::UDecWrap => {
                let (left, right) = self.compared_as(place, false, &old, &operand)?;
                let increment = matches!(op, FetchOp::UIncWrap);
                let one =
                    self.operand(e, place.c, ValueKind::Constant(Number::Integer(1u8.into())));
                let stepped = arith(
                    self,
                    if increment {
                        ArithOp::Add
                    } else {
                        ArithOp::Sub
                    },
                    old,
                    one,
                    wrap,
                );
                let (wrapped, condition) = if increment {
                    let zero =
                        self.operand(e, place.c, ValueKind::Constant(Number::Integer(0u8.into())));
                    let exhausted = self.compare(e, CompareOp::Ge, left, right);
                    (zero, exhausted)
                } else {
                    let zero =
                        self.operand(e, left.c, ValueKind::Constant(Number::Integer(0u8.into())));
                    let empty = self.compare(e, CompareOp::Eq, left.clone(), zero);
                    let above = self.compare(e, CompareOp::Gt, left, right);
                    let restart = self.builtin_operand(
                        e,
                        CTypeKind::Bool,
                        ValueKind::Logical {
                            op: LogicalOp::Or,
                            left: Box::new(empty.value),
                            right: Box::new(above.value),
                        },
                    );
                    (operand, restart)
                };
                self.operand(
                    e,
                    place.c,
                    ValueKind::Conditional {
                        condition: Box::new(condition.value),
                        then_value: Box::new(wrapped.value),
                        else_value: Box::new(stepped.value),
                    },
                )
            }
            FetchOp::Min { signed } | FetchOp::Max { signed } => {
                let (left, right) = match signed {
                    Some(signed) => self.compared_as(place, signed, &old, &operand)?,
                    None => (old.clone(), operand.clone()),
                };
                let op = if matches!(op, FetchOp::Min { .. }) {
                    CompareOp::Lt
                } else {
                    CompareOp::Gt
                };
                let keep_old = self.compare(e, op, left, right);
                self.operand(
                    e,
                    place.c,
                    ValueKind::Conditional {
                        condition: Box::new(keep_old.value),
                        then_value: Box::new(old.value),
                        else_value: Box::new(operand.value),
                    },
                )
            }
        })
    }

    fn compared_as(
        &mut self,
        place: &Lvalue,
        signed: bool,
        old: &Operand,
        operand: &Operand,
    ) -> Result<(Operand, Operand), ResolveError> {
        let Type::Numeric(NumericType::Integer {
            signed: declared, ..
        }) = &place.ty
        else {
            return Ok((old.clone(), operand.clone()));
        };
        if signed == *declared {
            return Ok((old.clone(), operand.clone()));
        }
        let compared = self.types.ctypes.integer_signedness(place.c, signed)?;
        let (kind, reason) = (CastKind::Arithmetic, ConversionReason::Arg);
        Ok((
            self.apply_conversion(kind, old.clone(), compared, reason)?,
            self.apply_conversion(kind, operand.clone(), compared, reason)?,
        ))
    }

    fn compare(&mut self, e: &Expr, op: CompareOp, left: Operand, right: Operand) -> Operand {
        self.builtin_operand(
            e,
            CTypeKind::Bool,
            ValueKind::Compare {
                op,
                left: Box::new(left.value),
                right: Box::new(right.value),
                exceptions: None,
                reason: None,
            },
        )
    }

    fn atomicity(&mut self, order: &Expr, scope: &SyncScope) -> Result<Atomicity, ResolveError> {
        Ok(Atomicity {
            order: self.memory_order(order)?,
            scope: scope.clone(),
        })
    }

    fn sync_scope(&mut self, scope: &Expr) -> Result<SyncScope, ResolveError> {
        let value = self.expr(scope)?;
        if let Some(scope) = constant_integer(&value)
            .and_then(|value| u64::try_from(value).ok())
            .and_then(SyncScope::from_c)
        {
            return Ok(scope);
        }
        Ok(SyncScope::Dynamic(Box::new(self.enum_integer(value.value))))
    }

    fn memory_order(&mut self, order: &Expr) -> Result<MemoryOrder, ResolveError> {
        let value = self.expr(order)?;
        if let Some(ordering) = constant_integer(&value)
            .and_then(|value| u64::try_from(value).ok())
            .and_then(MemoryOrder::from_c)
        {
            return Ok(ordering);
        }
        Ok(MemoryOrder::Dynamic(Box::new(
            self.enum_integer(value.value),
        )))
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
