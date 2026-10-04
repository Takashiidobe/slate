use super::builtins::{Expansion, Reduction};
use super::ctype::{CTypeKind, QualType};
use super::expression::Lowerer;
use super::numeric::ResolveError;
use super::operand::Operand;
use crate::ast::Expr;
use crate::ir::*;
use num_bigint::BigInt;

pub(super) type LoweredCall = (QualType, Type, Vec<Value>);

impl Lowerer {
    pub(super) fn expand_builtin(
        &mut self,
        e: &Expr,
        expansion: Expansion,
        signature: QualType,
        (returned, ty, arguments): LoweredCall,
    ) -> Result<Result<Operand, LoweredCall>, ResolveError> {
        let result_type = self.types.ir_type(returned);
        let kind = match expansion {
            Expansion::Reduce(reduction) => {
                let element = arguments
                    .first()
                    .and_then(|vector| lanewise_element(&vector.ty))
                    .ok_or(ResolveError::Internal("vector reduction operand"))?;
                intrinsic(reduction_intrinsic(reduction, element), arguments)
            }
            Expansion::Intrinsic(name) => intrinsic(name, arguments),
            Expansion::Popcount => intrinsic("llvm.ctpop", arguments),
            Expansion::Fma => intrinsic("llvm.fma", arguments),
            Expansion::Max | Expansion::Min => {
                let element = lanewise_element(&result_type)
                    .ok_or(ResolveError::Internal("elementwise operand"))?;
                let name = match (expansion == Expansion::Max, element) {
                    (true, NumericType::Integer { signed: true, .. }) => "llvm.smax",
                    (true, NumericType::Integer { signed: false, .. }) => "llvm.umax",
                    (true, NumericType::Float(_)) => "llvm.maxnum",
                    (false, NumericType::Integer { signed: true, .. }) => "llvm.smin",
                    (false, NumericType::Integer { signed: false, .. }) => "llvm.umin",
                    (false, NumericType::Float(_)) => "llvm.minnum",
                };
                intrinsic(name, arguments)
            }
            Expansion::Extract => {
                let (Some(source), Some(lanes)) = (
                    arguments
                        .first()
                        .and_then(|vector| vector_lanes(&vector.ty)),
                    vector_lanes(&result_type),
                ) else {
                    return Err(ResolveError::Internal("vector extract operand"));
                };
                let index = arguments.get(1).and_then(|index| self.constant(index));
                let unmasked = arguments
                    .get(3)
                    .is_none_or(|mask| self.all_lanes(mask, lanes));
                let (Some(index), true) = (index, unmasked) else {
                    return Ok(Err((returned, ty, arguments)));
                };
                let subvectors = BigInt::from(source / lanes);
                let first = u32::try_from((index % &subvectors + &subvectors) % &subvectors)
                    .map_err(|_| ResolveError::Internal("vector extract index"))?
                    * lanes;
                let mut arguments = arguments.into_iter();
                let vector = arguments
                    .next()
                    .ok_or(ResolveError::Internal("vector extract operand"))?;
                let extract = ValueKind::Shuffle {
                    left: Box::new(vector),
                    right: None,
                    mask: ShuffleMask::Lanes((first..first + lanes).map(Some).collect()),
                };
                match arguments.nth(1) {
                    Some(passthrough) if super::effects::has_effects(&passthrough) => {
                        ValueKind::Sequence {
                            left: Box::new(passthrough),
                            right: Box::new(self.value(e, result_type.clone(), extract)),
                        }
                    }
                    _ => extract,
                }
            }
            Expansion::TernaryLogic => {
                let shape = arguments.first().and_then(|vector| match &vector.ty {
                    Type::Vector {
                        element: NumericType::Integer { width, .. },
                        lanes,
                    } => Some((*width, *lanes)),
                    _ => None,
                });
                let Some((width, lanes)) = shape else {
                    return Err(ResolveError::Internal("ternary logic operand"));
                };
                let immediate = arguments.get(3).and_then(|imm| self.constant(imm));
                let unmasked = arguments
                    .get(4)
                    .is_some_and(|mask| self.all_lanes(mask, lanes));
                let (Some(immediate), true) = (immediate, unmasked) else {
                    return Ok(Err((returned, ty, arguments)));
                };
                let int = self.types.ctypes.int();
                let immediate = self.operand(
                    e,
                    int,
                    ValueKind::Constant(Number::Integer(
                        immediate.to_biguint().unwrap_or_default(),
                    )),
                );
                let mut operands: Vec<Value> = arguments.into_iter().take(3).collect();
                operands.push(immediate.value);
                let suffix = if width == 32 { 'd' } else { 'q' };
                ValueKind::Intrinsic {
                    name: format!("llvm.x86.avx512.pternlog.{suffix}.{}", width * lanes),
                    arguments: operands,
                }
            }
            Expansion::FloatReduce(op) => {
                let params = self
                    .types
                    .ctypes
                    .function_parts(signature)
                    .map(|(_, params, _, _)| params.to_vec())
                    .unwrap_or_default();
                let (Some(&vector_c), [start, vector]) = (params.get(1), arguments.as_slice())
                else {
                    return Err(ResolveError::Internal("float reduction operands"));
                };
                let value =
                    self.float_reduce_tree(e, op, start.clone(), vector_c, vector.clone())?;
                return Ok(Ok(Operand { c: returned, value }));
            }
        };
        Ok(Ok(self.operand(e, returned, kind)))
    }

    fn float_reduce_tree(
        &mut self,
        e: &Expr,
        op: ArithOp,
        start: Value,
        vector_c: QualType,
        vector: Value,
    ) -> Result<Value, ResolveError> {
        let CTypeKind::Vector {
            element,
            mut lanes,
            mut bytes,
        } = *self.types.ctypes.canonical_kind(vector_c)
        else {
            return Err(ResolveError::Internal("float reduction operand"));
        };
        let semantics = self.context.floating_arith();
        let mut captures = Vec::new();
        let mut current = Operand {
            c: vector_c,
            value: vector,
        };
        while lanes > 1 {
            let bound = self.bind_once(current, &mut captures);
            let half = lanes / 2;
            bytes /= 2;
            let c = self.types.ctypes.qual(CTypeKind::Vector {
                element,
                lanes: half,
                bytes,
            });
            let ty = self.types.ir_type(c);
            let part = |range: std::ops::Range<u32>| {
                let read = self.value(
                    e,
                    bound.ty.clone(),
                    ValueKind::Read {
                        place: bound.clone(),
                        ordering: None,
                    },
                );
                self.value(
                    e,
                    ty.clone(),
                    ValueKind::Shuffle {
                        left: Box::new(read),
                        right: None,
                        mask: ShuffleMask::Lanes(range.map(Some).collect()),
                    },
                )
            };
            let low = part(0..half);
            let high = part(half..lanes);
            current = self.operand(
                e,
                c,
                ValueKind::Arith {
                    op,
                    left: Box::new(low),
                    right: Box::new(high),
                    semantics,
                },
            );
            lanes = half;
        }
        let int = self.types.ctypes.int();
        let zero = self.operand(e, int, ValueKind::Constant(Number::Integer(0u32.into())));
        let lane = self.operand(
            e,
            element,
            ValueKind::Lane {
                vector: Box::new(current.value),
                index: Box::new(zero.value),
            },
        );
        let reduced = self.operand(
            e,
            element,
            ValueKind::Arith {
                op,
                left: Box::new(start),
                right: Box::new(lane.value),
                semantics,
            },
        );
        Ok(self.captured(e, captures, reduced.value))
    }

    fn bind_once(&mut self, operand: Operand, captures: &mut Vec<(BindingId, Value)>) -> Place {
        let id = self.fresh();
        self.types.entities.declare(id, operand.c, false);
        let place = Place {
            ty: operand.value.ty.clone(),
            kind: PlaceKind::Binding(id),
            access: self.types.access_of(operand.c),
        };
        captures.push((id, operand.value));
        place
    }

    fn constant(&self, value: &Value) -> Option<BigInt> {
        super::fold::integer_constant(value, self.types.flavor())
    }

    fn all_lanes(&self, mask: &Value, lanes: u32) -> bool {
        let all = (BigInt::from(1) << lanes) - 1;
        self.constant(mask).is_some_and(|mask| mask & &all == all)
    }
}

fn intrinsic(name: &str, arguments: Vec<Value>) -> ValueKind {
    ValueKind::Intrinsic {
        name: name.into(),
        arguments,
    }
}

fn lanewise_element(ty: &Type) -> Option<NumericType> {
    match ty {
        Type::Vector { element, .. } | Type::Numeric(element) => Some(*element),
        _ => None,
    }
}

fn vector_lanes(ty: &Type) -> Option<u32> {
    match ty {
        Type::Vector { lanes, .. } => Some(*lanes),
        _ => None,
    }
}

fn reduction_intrinsic(reduction: Reduction, element: NumericType) -> &'static str {
    let float = matches!(element, NumericType::Float(_));
    let signed = matches!(element, NumericType::Integer { signed: true, .. });
    match reduction {
        Reduction::Add => "llvm.vector.reduce.add",
        Reduction::Mul => "llvm.vector.reduce.mul",
        Reduction::And => "llvm.vector.reduce.and",
        Reduction::Or => "llvm.vector.reduce.or",
        Reduction::Xor => "llvm.vector.reduce.xor",
        Reduction::Max if float => "llvm.vector.reduce.fmax",
        Reduction::Max if signed => "llvm.vector.reduce.smax",
        Reduction::Max => "llvm.vector.reduce.umax",
        Reduction::Min if float => "llvm.vector.reduce.fmin",
        Reduction::Min if signed => "llvm.vector.reduce.smin",
        Reduction::Min => "llvm.vector.reduce.umin",
        Reduction::Maximum => "llvm.vector.reduce.fmaximum",
        Reduction::Minimum => "llvm.vector.reduce.fminimum",
    }
}
