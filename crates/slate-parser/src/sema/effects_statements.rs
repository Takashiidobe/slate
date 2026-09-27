use super::effects::Hoister;
use super::numeric::ResolveError;
use crate::ast::Span;
use crate::ir::*;
use std::collections::HashMap;

pub(super) fn normalize(
    module: &mut Module,
    next_id: u32,
    access: HashMap<BindingId, Access>,
) -> Result<(), ResolveError> {
    let effect_free = module
        .functions
        .iter()
        .filter(|function| function.semantics.memory.is_some())
        .map(|function| function.value.id)
        .collect();
    let mut hoister = Hoister::new(next_id, module.target.pointer_width, access, effect_free);
    for function in &mut module.functions {
        if let Some(body) = &mut function.value.body {
            *body = hoister.statements(std::mem::take(body))?;
        }
    }
    Ok(())
}

impl Hoister {
    fn evaluation(
        &mut self,
        evaluation: Evaluation,
        discarded: bool,
    ) -> Result<Evaluation, ResolveError> {
        let mut statements = self.statements(evaluation.statements)?;
        let value = if discarded {
            let node = evaluation.value.node.clone();
            self.discard(evaluation.value, None, &mut statements)?;
            Value {
                ty: Type::Void,
                node: node.derive(ValueKind::Void),
            }
        } else {
            self.value(evaluation.value, &mut statements)?
        };
        Ok(Evaluation { statements, value })
    }

    pub(super) fn statements(
        &mut self,
        body: Vec<Span<Statement>>,
    ) -> Result<Vec<Span<Statement>>, ResolveError> {
        let mut out = Vec::new();
        for statement in body {
            let span = statement.clone().with_value(());
            let statement = match statement.value {
                Statement::Expression(value) => {
                    self.discard(value, Some(span.clone()), &mut out)?;
                    continue;
                }
                Statement::Let(mut variable) => {
                    if let Some(initializer) = variable.initializer.take() {
                        let mut effects = Vec::new();
                        let value = self.value(initializer, &mut effects)?;
                        if effects.is_empty() {
                            variable.initializer = Some(value);
                        } else {
                            let place = Place {
                                ty: variable.ty.clone(),
                                kind: PlaceKind::Binding(variable.id),
                                access: self.access.get(&variable.id).copied().unwrap_or_default(),
                            };
                            out.push(span.clone().with_value(Statement::Let(variable)));
                            out.extend(effects);
                            let ordering = place.implicit_ordering();
                            out.push(span.derive(Statement::Write {
                                place,
                                value,
                                ordering,
                                unsequenced: false,
                            }));
                            continue;
                        }
                    }
                    Statement::Let(variable)
                }
                Statement::Temporary {
                    id,
                    ty,
                    initializer,
                    unsequenced,
                } => Statement::Temporary {
                    id,
                    ty,
                    initializer: initializer.map(|v| self.value(v, &mut out)).transpose()?,
                    unsequenced,
                },
                Statement::Write {
                    place,
                    value,
                    ordering,
                    unsequenced: _,
                } => {
                    let store = Value {
                        ty: place.ty.clone(),
                        node: span.with_value(ValueKind::Store {
                            place,
                            value: Box::new(value),
                            ordering,
                        }),
                    };
                    self.discard(store, None, &mut out)?;
                    continue;
                }
                Statement::Return(value) => {
                    Statement::Return(value.map(|v| self.value(v, &mut out)).transpose()?)
                }
                Statement::ComputedGoto(value) => {
                    Statement::ComputedGoto(self.value(value, &mut out)?)
                }
                Statement::Block(body) => Statement::Block(self.statements(body)?),
                Statement::If {
                    condition,
                    then_body,
                    else_body,
                } => Statement::If {
                    condition: self.value(condition, &mut out)?,
                    then_body: self.statements(then_body)?,
                    else_body: else_body.map(|b| self.statements(b)).transpose()?,
                },
                Statement::While {
                    id,
                    condition,
                    body,
                } => Statement::While {
                    id,
                    condition: self.evaluation(condition, false)?,
                    body: self.statements(body)?,
                },
                Statement::DoWhile {
                    id,
                    body,
                    condition,
                } => Statement::DoWhile {
                    id,
                    body: self.statements(body)?,
                    condition: self.evaluation(condition, false)?,
                },
                Statement::For {
                    id,
                    init,
                    condition,
                    increment,
                    body,
                } => Statement::For {
                    id,
                    init: self.statements(init)?,
                    condition: condition.map(|v| self.evaluation(v, false)).transpose()?,
                    increment: increment.map(|v| self.evaluation(v, true)).transpose()?,
                    body: self.statements(body)?,
                },
                Statement::Switch {
                    id,
                    discriminant,
                    body,
                } => Statement::Switch {
                    id,
                    discriminant: self.value(discriminant, &mut out)?,
                    body: self.statements(body)?,
                },
                Statement::Case {
                    switch,
                    start,
                    end,
                    body,
                } => Statement::Case {
                    switch,
                    start,
                    end,
                    body: self.statements(body)?,
                },
                Statement::Default { switch, body } => Statement::Default {
                    switch,
                    body: self.statements(body)?,
                },
                Statement::Asm(mut asm) => {
                    let mut placed = Vec::with_capacity(asm.operands.len());
                    for operand in std::mem::take(&mut asm.operands) {
                        let kind = match operand.kind {
                            AsmOperandKind::Out {
                                place,
                                early_clobber,
                            } => AsmOperandKind::Out {
                                place: self.place(place, &mut out)?,
                                early_clobber,
                            },
                            AsmOperandKind::InOut {
                                place,
                                input,
                                early_clobber,
                            } => AsmOperandKind::InOut {
                                place: self.place(place, &mut out)?,
                                input,
                                early_clobber,
                            },
                            kind @ (AsmOperandKind::In(_)
                            | AsmOperandKind::InPlace(_)
                            | AsmOperandKind::Symbol(_)) => kind,
                        };
                        placed.push(AsmOperand { kind, ..operand });
                    }
                    asm.operands = placed;
                    let order = asm.inputs_in_source_order();
                    let mut operands = std::mem::take(&mut asm.operands)
                        .into_iter()
                        .map(Some)
                        .collect::<Vec<_>>();
                    for index in order {
                        let Some(slot) = operands.get_mut(index) else {
                            continue;
                        };
                        let Some(operand) = slot.take() else {
                            continue;
                        };
                        let kind = match operand.kind {
                            AsmOperandKind::In(value) => {
                                AsmOperandKind::In(self.value(value, &mut out)?)
                            }
                            AsmOperandKind::InPlace(place) => {
                                AsmOperandKind::InPlace(self.place(place, &mut out)?)
                            }
                            AsmOperandKind::InOut {
                                place,
                                input: Some(input),
                                early_clobber,
                            } => AsmOperandKind::InOut {
                                place,
                                input: Some(AsmTiedInput {
                                    value: self.value(input.value, &mut out)?,
                                    operand: input.operand,
                                }),
                                early_clobber,
                            },
                            kind => kind,
                        };
                        *slot = Some(AsmOperand { kind, ..operand });
                    }
                    asm.operands = operands.into_iter().flatten().collect();
                    Statement::Asm(asm)
                }
                Statement::Label { id, name, body } => Statement::Label {
                    id,
                    name,
                    body: self.statements(body)?,
                },
                other @ (Statement::Null
                | Statement::Fence { .. }
                | Statement::Break(_)
                | Statement::Continue(_)
                | Statement::Goto(_)) => other,
            };
            out.push(span.with_value(statement));
        }
        Ok(out)
    }
}
