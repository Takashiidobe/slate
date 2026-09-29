use super::numeric::ResolveError;
use crate::ast::Span;
use crate::ir::*;
use std::collections::{HashMap, HashSet};

pub(super) fn has_effects(value: &Value) -> bool {
    Hoister::new(0, 0, HashMap::new(), HashSet::new()).effects(value)
}

pub(super) struct Hoister {
    next_id: u32,
    old: Vec<Value>,
    unsequenced: bool,
    pub(super) access: HashMap<BindingId, Access>,
    effect_free: HashSet<BindingId>,
}

impl Hoister {
    pub(super) fn new(
        next_id: u32,
        _pointer_width: u32,
        access: HashMap<BindingId, Access>,
        effect_free: HashSet<BindingId>,
    ) -> Self {
        Self {
            next_id,
            old: Vec::new(),
            unsequenced: false,
            access,
            effect_free,
        }
    }

    /// C11 6.5p2 leaves the relative order of these operands unspecified, so the
    /// order hoisting commits to is arbitrary. Slate follows Clang; see
    /// `wiki/concepts/ir/control-flow.md`.
    fn grouped<T>(&mut self, unsequenced: bool, hoist: impl FnOnce(&mut Self) -> T) -> T {
        let outer = self.unsequenced;
        self.unsequenced = outer || unsequenced;
        let result = hoist(self);
        self.unsequenced = outer;
        result
    }

    fn temporary(&mut self, value: Value, out: &mut Vec<Span<Statement>>) -> Value {
        let place = self.declare(&value, Some(value.clone()), out);
        Value {
            ty: value.ty,
            node: value.node.derive(ValueKind::Read {
                place,
                ordering: None,
            }),
        }
    }

    fn declare(
        &mut self,
        source: &Value,
        initializer: Option<Value>,
        out: &mut Vec<Span<Statement>>,
    ) -> Place {
        let id = BindingId(self.next_id);
        self.next_id += 1;
        out.push(source.node.derive(Statement::Temporary {
            id,
            ty: source.ty.clone(),
            initializer,
            unsequenced: self.unsequenced,
        }));
        Place {
            ty: source.ty.clone(),
            kind: PlaceKind::Binding(id),
            access: Access::default(),
        }
    }

    pub(super) fn discard(
        &mut self,
        value: Value,
        span: Option<Span<()>>,
        out: &mut Vec<Span<Statement>>,
    ) -> Result<(), ResolveError> {
        let had_effects = self.effects(&value);
        let value = self.value(value, out)?;
        if !(had_effects && !self.effects(&value)) {
            let span = span.unwrap_or_else(|| value.node.derive(()));
            out.push(span.with_value(Statement::Expression(value)));
        }
        Ok(())
    }

    pub(super) fn place(
        &mut self,
        place: Place,
        out: &mut Vec<Span<Statement>>,
    ) -> Result<Place, ResolveError> {
        let kind = match place.kind {
            PlaceKind::Binding(id) => PlaceKind::Binding(id),
            PlaceKind::Deref(value) => PlaceKind::Deref(Box::new(self.value(*value, out)?)),
            PlaceKind::CompoundLiteral {
                object,
                storage,
                alignment,
                initializer,
            } => PlaceKind::CompoundLiteral {
                object,
                storage,
                alignment,
                initializer: Box::new(self.value(*initializer, out)?),
            },
            PlaceKind::Temporary {
                object,
                initializer,
            } => PlaceKind::Temporary {
                object,
                initializer: Box::new(self.value(*initializer, out)?),
            },
            PlaceKind::ComplexPart { base, imaginary } => PlaceKind::ComplexPart {
                base: Box::new(self.place(*base, out)?),
                imaginary,
            },
            PlaceKind::Field { base, index, bits } => PlaceKind::Field {
                base: Box::new(self.place(*base, out)?),
                index,
                bits,
            },
            PlaceKind::Index { base, index } => PlaceKind::Index {
                base: Box::new(self.value(*base, out)?),
                index: Box::new(self.value(*index, out)?),
            },
            PlaceKind::Lane { base, index } => PlaceKind::Lane {
                base: Box::new(self.place(*base, out)?),
                index: Box::new(self.value(*index, out)?),
            },
            PlaceKind::Swizzle { base, lanes } => PlaceKind::Swizzle {
                base: Box::new(self.place(*base, out)?),
                lanes,
            },
        };
        Ok(Place {
            ty: place.ty,
            kind,
            access: place.access,
        })
    }

    fn stable_place(
        &mut self,
        place: Place,
        out: &mut Vec<Span<Statement>>,
    ) -> Result<Place, ResolveError> {
        let kind = match place.kind {
            PlaceKind::Binding(id) => PlaceKind::Binding(id),
            PlaceKind::Deref(value) => {
                let value = self.value(*value, out)?;
                PlaceKind::Deref(Box::new(self.temporary(value, out)))
            }
            PlaceKind::CompoundLiteral {
                object,
                storage,
                alignment,
                initializer,
            } => PlaceKind::CompoundLiteral {
                object,
                storage,
                alignment,
                initializer: Box::new(self.value(*initializer, out)?),
            },
            PlaceKind::Temporary {
                object,
                initializer,
            } => PlaceKind::Temporary {
                object,
                initializer: Box::new(self.value(*initializer, out)?),
            },
            PlaceKind::ComplexPart { base, imaginary } => PlaceKind::ComplexPart {
                base: Box::new(self.stable_place(*base, out)?),
                imaginary,
            },
            PlaceKind::Field { base, index, bits } => PlaceKind::Field {
                base: Box::new(self.stable_place(*base, out)?),
                index,
                bits,
            },
            PlaceKind::Index { base, index } => {
                let base = self.value(*base, out)?;
                let base = self.temporary(base, out);
                let index = self.value(*index, out)?;
                let index = self.temporary(index, out);
                PlaceKind::Index {
                    base: Box::new(base),
                    index: Box::new(index),
                }
            }
            PlaceKind::Swizzle { base, lanes } => PlaceKind::Swizzle {
                base: Box::new(self.stable_place(*base, out)?),
                lanes,
            },
            PlaceKind::Lane { base, index } => {
                let base = self.stable_place(*base, out)?;
                let index = self.value(*index, out)?;
                let index = self.temporary(index, out);
                PlaceKind::Lane {
                    base: Box::new(base),
                    index: Box::new(index),
                }
            }
        };
        Ok(Place {
            ty: place.ty,
            kind,
            access: place.access,
        })
    }

    fn memory_order(
        &mut self,
        order: MemoryOrder,
        out: &mut Vec<Span<Statement>>,
    ) -> Result<MemoryOrder, ResolveError> {
        Ok(match order {
            MemoryOrder::Dynamic(value) => MemoryOrder::Dynamic(Box::new(self.value(*value, out)?)),
            fixed => fixed,
        })
    }

    fn sync_scope(
        &mut self,
        scope: SyncScope,
        out: &mut Vec<Span<Statement>>,
    ) -> Result<SyncScope, ResolveError> {
        Ok(match scope {
            SyncScope::Dynamic(value) => SyncScope::Dynamic(Box::new(self.value(*value, out)?)),
            fixed => fixed,
        })
    }

    fn order(
        &mut self,
        ordering: Atomicity,
        out: &mut Vec<Span<Statement>>,
    ) -> Result<Atomicity, ResolveError> {
        Ok(Atomicity {
            order: self.memory_order(ordering.order, out)?,
            scope: self.sync_scope(ordering.scope, out)?,
        })
    }

    fn ordering(
        &mut self,
        ordering: Option<Atomicity>,
        out: &mut Vec<Span<Statement>>,
    ) -> Result<Option<Atomicity>, ResolveError> {
        ordering
            .map(|ordering| self.order(ordering, out))
            .transpose()
    }

    fn branch(
        &mut self,
        source: &Value,
        condition: Value,
        left: Value,
        right: Value,
        out: &mut Vec<Span<Statement>>,
    ) -> Result<Value, ResolveError> {
        let result = if source.ty == Type::Void {
            None
        } else {
            Some(self.declare(source, None, out))
        };
        let mut branches = Vec::new();
        for value in [left, right] {
            let mut body = Vec::new();
            if let Some(place) = &result {
                let value = self.value(value, &mut body)?;
                body.push(value.node.derive(()).with_value(Statement::Write {
                    place: place.clone(),
                    value,
                    ordering: None,
                    unsequenced: false,
                }));
            } else {
                self.discard(value, None, &mut body)?;
            }
            branches.push(body);
        }
        let else_body = branches.pop();
        let then_body = branches.pop().unwrap_or_default();
        out.push(source.node.derive(Statement::If {
            condition,
            then_body,
            else_body,
        }));
        Ok(Value {
            ty: source.ty.clone(),
            node: source.node.clone().with_value(match result {
                Some(place) => ValueKind::Read {
                    place,
                    ordering: None,
                },
                None => ValueKind::Void,
            }),
        })
    }

    pub(super) fn value(
        &mut self,
        value: Value,
        out: &mut Vec<Span<Statement>>,
    ) -> Result<Value, ResolveError> {
        let source = value.node.clone().with_value(());
        let ty = value.ty.clone();
        let template = Value {
            ty: ty.clone(),
            node: source.derive(ValueKind::Void),
        };
        let kind = match value.node.value {
            ValueKind::Store {
                place,
                value,
                ordering,
            } => {
                let conflict = super::sequencing::assignment(&place, &value);
                let (value, place, ordering) = self.grouped(conflict, |this| {
                    Ok::<_, ResolveError>((
                        this.value(*value, out)?,
                        this.place(place, out)?,
                        this.ordering(ordering, out)?,
                    ))
                })?;
                out.push(source.with_value(Statement::Write {
                    place,
                    value: value.clone(),
                    ordering,
                    unsequenced: conflict || self.unsequenced,
                }));
                return Ok(value);
            }
            ValueKind::Update {
                place,
                computation,
                postfix,
                ordering: Some(ordering),
            } => {
                let place = self.place(place, out)?;
                self.old.push(Value {
                    ty: place.ty.clone(),
                    node: source.derive(ValueKind::OldValue),
                });
                let computation = self.value(*computation, out);
                self.old.pop();
                let ordering = self.order(ordering, out)?;
                let update = Value {
                    ty,
                    node: source.with_value(ValueKind::Update {
                        place,
                        computation: Box::new(computation?),
                        postfix,
                        ordering: Some(ordering),
                    }),
                };
                return Ok(self.temporary(update, out));
            }
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
                let place = self.place(place, out)?;
                let expected = self.value(*expected, out)?;
                let desired = self.value(*desired, out)?;
                let success = self.memory_order(success, out)?;
                let failure = self.memory_order(failure, out)?;
                let sync_scope = self.sync_scope(sync_scope, out)?;
                let weak = match weak {
                    Weakness::Dynamic(value) => {
                        Weakness::Dynamic(Box::new(self.value(*value, out)?))
                    }
                    fixed => fixed,
                };
                let exchange = Value {
                    ty,
                    node: source.with_value(ValueKind::CompareExchange {
                        place,
                        expected: Box::new(expected),
                        desired: Box::new(desired),
                        success,
                        failure,
                        sync_scope,
                        weak,
                        form,
                    }),
                };
                return Ok(self.temporary(exchange, out));
            }
            ValueKind::Fence { ordering, scope } => {
                let ordering = self.order(ordering, out)?;
                out.push(
                    source
                        .clone()
                        .with_value(Statement::Fence { ordering, scope }),
                );
                return Ok(Value {
                    ty,
                    node: source.derive(ValueKind::Void),
                });
            }
            ValueKind::Update {
                place,
                computation,
                postfix,
                ordering: None,
            } => {
                let conflict = super::sequencing::assignment(&place, &computation);
                let place = self.stable_place(place, out)?;
                let old = Value {
                    ty: place.ty.clone(),
                    node: source.derive(ValueKind::Read {
                        place: place.clone(),
                        ordering: None,
                    }),
                };
                let (old, result) = self.grouped(conflict, |this| {
                    let old = this.temporary(old, out);
                    this.old.push(old.clone());
                    let result = this.value(*computation, out);
                    this.old.pop();
                    Ok::<_, ResolveError>((old, this.temporary(result?, out)))
                })?;
                out.push(source.with_value(Statement::Write {
                    place,
                    value: result.clone(),
                    ordering: None,
                    unsequenced: conflict || self.unsequenced,
                }));
                return Ok(if postfix { old } else { result });
            }
            ValueKind::StatementExpression(evaluation) => {
                let result = (ty != Type::Void).then(|| self.declare(&template, None, out));
                let mut body = self.statements(evaluation.statements)?;
                match &result {
                    Some(place) => {
                        let value = self.value(evaluation.value, &mut body)?;
                        body.push(value.node.derive(()).with_value(Statement::Write {
                            place: place.clone(),
                            value,
                            ordering: None,
                            unsequenced: false,
                        }));
                    }
                    None if matches!(evaluation.value.node.value, ValueKind::Void) => {}
                    None => self.discard(evaluation.value, None, &mut body)?,
                }
                out.push(source.derive(Statement::Block(body)));
                return Ok(Value {
                    ty,
                    node: source.with_value(match result {
                        Some(place) => ValueKind::Read {
                            place,
                            ordering: None,
                        },
                        None => ValueKind::Void,
                    }),
                });
            }
            ValueKind::OldValue => {
                return self
                    .old
                    .last()
                    .cloned()
                    .ok_or(ResolveError::Internal("old value outside update"));
            }
            ValueKind::Sequence { left, right } => {
                self.discard(*left, None, out)?;
                return self.value(*right, out);
            }
            ValueKind::Capture { id, extent, value } => {
                let extent = self.value(*extent, out)?;
                out.push(source.derive(Statement::Temporary {
                    id,
                    ty: extent.ty.clone(),
                    initializer: Some(extent),
                    unsequenced: false,
                }));
                return self.value(*value, out);
            }
            ValueKind::Logical { op, left, right } => {
                let left = self.value(*left, out)?;
                if self.effects(&right) {
                    let constant = Value {
                        ty: Type::Bool,
                        node: source.derive(ValueKind::Constant(Number::Bool(matches!(
                            op,
                            LogicalOp::Or
                        )))),
                    };
                    return match op {
                        LogicalOp::And => self.branch(&template, left, *right, constant, out),
                        LogicalOp::Or => self.branch(&template, left, constant, *right, out),
                    };
                }
                ValueKind::Logical {
                    op,
                    left: Box::new(left),
                    right: Box::new(self.value(*right, out)?),
                }
            }
            ValueKind::Conditional {
                condition,
                then_value,
                else_value,
            } => {
                let condition = self.value(*condition, out)?;
                if self.effects(&then_value) || self.effects(&else_value) {
                    return self.branch(&template, condition, *then_value, *else_value, out);
                }
                ValueKind::Conditional {
                    condition: Box::new(condition),
                    then_value: Box::new(self.value(*then_value, out)?),
                    else_value: Box::new(self.value(*else_value, out)?),
                }
            }
            ValueKind::Call {
                callee,
                signature,
                abi,
                arguments,
            } => {
                let mut group: Vec<&Value> = Vec::new();
                if let Callee::Indirect(value) = &callee {
                    group.push(value);
                }
                group.extend(arguments.iter());
                let conflict = super::sequencing::unsequenced(&group);
                drop(group);
                let (callee, lowered) = self.grouped(conflict, |this| {
                    let callee = match callee {
                        Callee::Direct(id) => Callee::Direct(id),
                        Callee::Indirect(value) => {
                            Callee::Indirect(Box::new(this.value(*value, out)?))
                        }
                    };
                    let lowered = arguments
                        .into_iter()
                        .map(|argument| this.value(argument, out))
                        .collect::<Result<Vec<_>, _>>()?;
                    Ok::<_, ResolveError>((callee, lowered))
                })?;
                ValueKind::Call {
                    callee,
                    signature,
                    abi,
                    arguments: lowered,
                }
            }
            ValueKind::Arith {
                op,
                left,
                right,
                semantics,
            } => {
                let conflict = super::sequencing::unsequenced(&[&left, &right]);
                let (left, right) = self.grouped(conflict, |this| {
                    Ok::<_, ResolveError>((this.value(*left, out)?, this.value(*right, out)?))
                })?;
                ValueKind::Arith {
                    op,
                    left: Box::new(left),
                    right: Box::new(right),
                    semantics,
                }
            }
            ValueKind::Overflow {
                op,
                left,
                right,
                result,
            } => ValueKind::Overflow {
                op,
                left: Box::new(self.value(*left, out)?),
                right: Box::new(self.value(*right, out)?),
                result: self.place(result, out)?,
            },
            ValueKind::Compare {
                op,
                left,
                right,
                exceptions,
                reason,
            } => {
                let left = self.value(*left, out)?;
                let right = self.value(*right, out)?;
                ValueKind::Compare {
                    op,
                    left: Box::new(left),
                    right: Box::new(right),
                    exceptions,
                    reason,
                }
            }
            ValueKind::PointerOffset {
                pointer,
                amount,
                subtract,
                element,
                overflow,
            } => {
                let pointer = self.value(*pointer, out)?;
                let amount = self.value(*amount, out)?;
                ValueKind::PointerOffset {
                    pointer: Box::new(pointer),
                    amount: Box::new(amount),
                    subtract,
                    element,
                    overflow,
                }
            }
            ValueKind::PointerDifference {
                left,
                right,
                element,
            } => {
                let left = self.value(*left, out)?;
                let right = self.value(*right, out)?;
                ValueKind::PointerDifference {
                    left: Box::new(left),
                    right: Box::new(right),
                    element,
                }
            }
            ValueKind::Unary {
                op,
                operand,
                semantics,
            } => ValueKind::Unary {
                op,
                operand: Box::new(self.value(*operand, out)?),
                semantics,
            },
            ValueKind::FloatClass { test, operand } => ValueKind::FloatClass {
                test,
                operand: Box::new(self.value(*operand, out)?),
            },
            ValueKind::Lane { vector, index } => ValueKind::Lane {
                vector: Box::new(self.value(*vector, out)?),
                index: Box::new(self.value(*index, out)?),
            },
            ValueKind::Shuffle { left, right, mask } => ValueKind::Shuffle {
                left: Box::new(self.value(*left, out)?),
                right: match right {
                    Some(right) => Some(Box::new(self.value(*right, out)?)),
                    None => None,
                },
                mask: match mask {
                    ShuffleMask::Dynamic(mask) => {
                        ShuffleMask::Dynamic(Box::new(self.value(*mask, out)?))
                    }
                    lanes => lanes,
                },
            },
            ValueKind::Convert {
                kind,
                operand,
                reason,
                semantics,
            } => ValueKind::Convert {
                kind,
                operand: Box::new(self.value(*operand, out)?),
                reason,
                semantics,
            },
            ValueKind::Copy { operand, reason } => ValueKind::Copy {
                operand: Box::new(self.value(*operand, out)?),
                reason,
            },
            ValueKind::Aggregate { members, zero_fill } => {
                let mut lowered = Vec::new();
                for member in members {
                    lowered.push(AggregateMember {
                        target: member.target,
                        value: self.value(member.value, out)?,
                    });
                }
                ValueKind::Aggregate {
                    members: lowered,
                    zero_fill,
                }
            }
            ValueKind::Read { place, ordering } => ValueKind::Read {
                place: self.place(place, out)?,
                ordering: self.ordering(ordering, out)?,
            },
            ValueKind::AddressOf(place) => ValueKind::AddressOf(self.place(place, out)?),
            ValueKind::VaArg { list } => ValueKind::VaArg {
                list: self.place(list, out)?,
            },
            ValueKind::VaStart { list } => ValueKind::VaStart {
                list: self.place(list, out)?,
            },
            ValueKind::VaEnd { list } => ValueKind::VaEnd {
                list: self.place(list, out)?,
            },
            ValueKind::VaCopy {
                destination,
                source,
            } => ValueKind::VaCopy {
                destination: self.place(destination, out)?,
                source: self.place(source, out)?,
            },
            ValueKind::ArrayDecay { place, length } => ValueKind::ArrayDecay {
                place: self.place(place, out)?,
                length,
            },
            ValueKind::FunctionDecay { place } => ValueKind::FunctionDecay {
                place: self.place(place, out)?,
            },
            leaf @ (ValueKind::Constant(_)
            | ValueKind::Null
            | ValueKind::LabelAddress(_)
            | ValueKind::Void
            | ValueKind::CodeUnits(_)) => leaf,
        };
        Ok(Value {
            ty,
            node: source.with_value(kind),
        })
    }

    fn place_effects(&self, place: &Place) -> bool {
        match &place.kind {
            PlaceKind::Binding(_) => false,
            PlaceKind::Deref(value) => self.effects(value),
            PlaceKind::CompoundLiteral { initializer, .. } => self.effects(initializer),
            PlaceKind::Temporary { initializer, .. } => self.effects(initializer),
            PlaceKind::ComplexPart { base, .. } => self.place_effects(base),
            PlaceKind::Field { base, .. } => self.place_effects(base),
            PlaceKind::Index { base, index } => self.effects(base) || self.effects(index),
            PlaceKind::Lane { base, index } => self.place_effects(base) || self.effects(index),
            PlaceKind::Swizzle { base, .. } => self.place_effects(base),
        }
    }

    fn atomicity_effects(&self, ordering: &Atomicity) -> bool {
        matches!(&ordering.order, MemoryOrder::Dynamic(value) if self.effects(value))
            || matches!(&ordering.scope, SyncScope::Dynamic(value) if self.effects(value))
    }

    fn effects(&self, value: &Value) -> bool {
        match &value.node.value {
            ValueKind::Call {
                callee: Callee::Direct(id),
                arguments,
                ..
            } if self.effect_free.contains(id) => {
                arguments.iter().any(|argument| self.effects(argument))
            }
            ValueKind::Store { .. }
            | ValueKind::Update { .. }
            | ValueKind::CompareExchange { .. }
            | ValueKind::Overflow { .. }
            | ValueKind::Fence { .. }
            | ValueKind::Call { .. }
            | ValueKind::VaArg { .. }
            | ValueKind::StatementExpression(_)
            | ValueKind::Capture { .. }
            | ValueKind::VaStart { .. }
            | ValueKind::VaEnd { .. }
            | ValueKind::VaCopy { .. }
            | ValueKind::Sequence { .. } => true,
            ValueKind::Arith { left, right, .. }
            | ValueKind::Compare { left, right, .. }
            | ValueKind::Logical { left, right, .. }
            | ValueKind::PointerDifference { left, right, .. } => {
                self.effects(left) || self.effects(right)
            }
            ValueKind::PointerOffset {
                pointer, amount, ..
            } => self.effects(pointer) || self.effects(amount),
            ValueKind::Conditional {
                condition,
                then_value,
                else_value,
            } => self.effects(condition) || self.effects(then_value) || self.effects(else_value),
            ValueKind::Aggregate { members, .. } => {
                members.iter().any(|member| self.effects(&member.value))
            }
            ValueKind::Lane { vector, index } => self.effects(vector) || self.effects(index),
            ValueKind::Shuffle { left, right, mask } => {
                self.effects(left)
                    || right.as_deref().is_some_and(|right| self.effects(right))
                    || match mask {
                        ShuffleMask::Lanes(_) => false,
                        ShuffleMask::Dynamic(mask) => self.effects(mask),
                    }
            }
            ValueKind::Copy { operand, .. }
            | ValueKind::Unary { operand, .. }
            | ValueKind::FloatClass { operand, .. }
            | ValueKind::Convert { operand, .. } => self.effects(operand),
            ValueKind::Read {
                place,
                ordering: Some(ordering),
            } => self.place_effects(place) || self.atomicity_effects(ordering),
            ValueKind::Read { place, .. }
            | ValueKind::AddressOf(place)
            | ValueKind::ArrayDecay { place, .. }
            | ValueKind::FunctionDecay { place } => self.place_effects(place),
            ValueKind::OldValue
            | ValueKind::Constant(_)
            | ValueKind::Null
            | ValueKind::LabelAddress(_)
            | ValueKind::Void
            | ValueKind::CodeUnits(_) => false,
        }
    }
}
