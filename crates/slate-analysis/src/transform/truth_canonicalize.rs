use super::const_fold::constant_comparison;
use super::inline_temporaries::{children, evaluations};
use foldhash::HashSet;
use slate_parser::ast::{MacroExpansionId, Span};
use slate_parser::ir::*;

pub(crate) fn transform(module: &mut Module) {
    let Module {
        globals,
        functions,
        macro_values,
        ..
    } = module;
    let mut truth = Truth {
        macro_values,
        depth: 0,
        temporaries: HashSet::default(),
        written: HashSet::default(),
    };
    for global in globals {
        if let Some(initializer) = &mut global.value.variable.initializer {
            walk_value_with_macros(initializer, macro_values, &mut truth);
        }
    }
    for function in functions.iter_mut() {
        if let Some(body) = &mut function.value.body {
            walk_body_with_macros(body, macro_values, &mut truth);
        }
    }
    if truth.written.is_empty() {
        return;
    }
    for function in functions {
        if let Some(body) = &mut function.value.body {
            initialize(body, &truth.written);
        }
    }
}

fn initialize(body: &mut Vec<Span<Statement>>, written: &HashSet<BindingId>) {
    for statement in body.iter_mut() {
        for child in children(&mut statement.value) {
            initialize(child, written);
        }
        for evaluation in evaluations(&mut statement.value) {
            initialize(&mut evaluation.statements, written);
        }
    }
    let mut position = 0;
    while position + 1 < body.len() {
        let (head, tail) = body.split_at_mut(position + 1);
        if let Statement::Temporary {
            id,
            initializer: initializer @ None,
            ..
        } = &mut head[position].value
            && written.contains(id)
            && let Statement::Write {
                place:
                    Place {
                        kind: PlaceKind::Binding(target),
                        ..
                    },
                value,
                ordering: None,
                ..
            } = &mut tail[0].value
            && target == id
        {
            *initializer = Some(std::mem::replace(
                value,
                Value {
                    ty: Type::Void,
                    node: value.node.derive(ValueKind::Void),
                },
            ));
            body.remove(position + 1);
        }
        position += 1;
    }
}

pub fn negation(condition: &Value, macro_values: &MacroValues) -> Option<Value> {
    let truth = Truth {
        macro_values,
        depth: 0,
        temporaries: HashSet::default(),
        written: HashSet::default(),
    };
    let mut negated = truth.rewrite(&not(condition, condition.clone()))?;
    while let Some(rewritten) = truth.rewrite(&negated) {
        negated = rewritten;
    }
    if let Some(holds) = constant_comparison(&negated, macro_values) {
        negated.node.value = ValueKind::Constant(Number::Bool(holds));
    }
    Some(negated)
}

struct Truth<'a> {
    macro_values: &'a MacroValues,
    depth: usize,
    temporaries: HashSet<BindingId>,
    written: HashSet<BindingId>,
}

impl Visitor for Truth<'_> {
    fn enter_expansion(&mut self, _expansion: MacroExpansionId) {
        self.depth += 1;
    }

    fn leave_expansion(&mut self, _expansion: MacroExpansionId) {
        self.depth -= 1;
    }

    fn enter_value_hole(&mut self, _expansion: MacroExpansionId) {
        self.depth += 1;
    }

    fn leave_value_hole(&mut self, _expansion: MacroExpansionId) {
        self.depth -= 1;
    }

    fn rewrite_value(&mut self, value: &mut Value) {
        if self.depth > 0 {
            return;
        }
        while let Some(rewritten) = self.rewrite(value) {
            *value = rewritten;
        }
    }

    fn statement(&mut self, statement: &Statement) {
        if let Statement::Temporary {
            id, ty: Type::Bool, ..
        } = statement
        {
            self.temporaries.insert(*id);
        }
    }

    fn rewrite_statement(&mut self, statement: &mut Statement) {
        if self.depth > 0 {
            return;
        }
        let Statement::If {
            condition,
            then_body,
            else_body: Some(else_body),
            folded: None,
        } = statement
        else {
            return;
        };
        let (Some((then_place, then_value)), Some((else_place, else_value))) = (
            self.temporary_write(then_body),
            self.temporary_write(else_body),
        ) else {
            return;
        };
        if then_place != else_place {
            return;
        }
        let Some(folded) = self.fold(
            condition,
            condition,
            (self.arm(then_value, false), then_value),
            (self.arm(else_value, false), else_value),
            false,
        ) else {
            return;
        };
        self.written.insert(then_place);
        let Some(Span {
            value: Statement::Write { place, .. },
            ..
        }) = then_body.pop()
        else {
            return;
        };
        *statement = Statement::Write {
            place,
            value: folded,
            ordering: None,
            unsequenced: false,
        };
    }
}

enum Arm {
    True,
    False,
    Other,
}

impl Truth<'_> {
    fn plain(&self, value: &Value) -> bool {
        !self.macro_values.contains_key(&value.node.id)
    }

    fn rewrite(&self, value: &Value) -> Option<Value> {
        match &value.node.value {
            ValueKind::Compare {
                op: op @ (CompareOp::Eq | CompareOp::Ne),
                left,
                right,
                reason,
                ..
            } if *reason != Some(ConversionReason::Explicit) => {
                let tested = match (zero(left), zero(right)) {
                    (false, true) if self.plain(right) => left,
                    (true, false) if self.plain(left) => right,
                    _ => return None,
                };
                let truth = self.truth_of(tested)?;
                Some(match op {
                    CompareOp::Ne => truth,
                    _ => self.negate(value, truth),
                })
            }
            ValueKind::Unary {
                op: UnaryArithOp::Not,
                operand,
                ..
            } if value.ty == Type::Bool && self.plain(operand) => match &operand.node.value {
                ValueKind::Unary {
                    op: UnaryArithOp::Not,
                    operand: inner,
                    ..
                } if operand.ty == Type::Bool => Some((**inner).clone()),
                ValueKind::Constant(Number::Bool(constant)) => Some(Value {
                    ty: Type::Bool,
                    node: value
                        .node
                        .clone()
                        .with_value(ValueKind::Constant(Number::Bool(!constant))),
                }),
                ValueKind::Compare {
                    op,
                    left,
                    right,
                    exceptions,
                    reason,
                } if *reason != Some(ConversionReason::Explicit)
                    && (matches!(op, CompareOp::Eq | CompareOp::Ne)
                        || (exceptions.is_none() && ordered(&left.ty))) =>
                {
                    Some(Value {
                        ty: Type::Bool,
                        node: value.node.clone().with_value(ValueKind::Compare {
                            op: inverse(*op),
                            left: left.clone(),
                            right: right.clone(),
                            exceptions: *exceptions,
                            reason: None,
                        }),
                    })
                }
                _ => None,
            },
            ValueKind::Conditional {
                condition,
                then_value,
                else_value,
            } if self.plain(then_value) && self.plain(else_value) => {
                let integer = integer(&value.ty);
                if !integer && value.ty != Type::Bool {
                    return None;
                }
                let folded = self.fold(
                    value,
                    condition,
                    (self.arm(then_value, integer), then_value),
                    (self.arm(else_value, integer), else_value),
                    integer,
                )?;
                Some(if integer {
                    from_bool(value, folded)
                } else {
                    folded
                })
            }
            _ => None,
        }
    }

    fn temporary_write<'v>(&self, body: &'v [Span<Statement>]) -> Option<(BindingId, &'v Value)> {
        match body {
            [
                Span {
                    value:
                        Statement::Write {
                            place:
                                Place {
                                    kind: PlaceKind::Binding(id),
                                    ..
                                },
                            value,
                            ordering: None,
                            ..
                        },
                    ..
                },
            ] if self.temporaries.contains(id) && self.plain(value) => Some((*id, value)),
            _ => None,
        }
    }

    fn negate(&self, anchor: &Value, operand: Value) -> Value {
        let mut negated = not(anchor, operand);
        while let Some(rewritten) = self.rewrite(&negated) {
            negated = rewritten;
        }
        negated
    }

    fn truth_of(&self, tested: &Value) -> Option<Value> {
        match &tested.node.value {
            ValueKind::Convert {
                kind: ConversionKind::FromBool,
                operand,
                reason,
                ..
            } if *reason != ConversionReason::Explicit && self.plain(tested) => {
                Some((**operand).clone())
            }
            _ => None,
        }
    }

    fn arm(&self, arm: &Value, integer: bool) -> Arm {
        match &arm.node.value {
            ValueKind::Constant(Number::Bool(true)) if !integer => Arm::True,
            ValueKind::Constant(Number::Bool(false)) if !integer => Arm::False,
            ValueKind::Constant(Number::Integer(one)) if integer && *one == 1u32.into() => {
                Arm::True
            }
            ValueKind::Constant(Number::Integer(zero)) if integer && *zero == 0u32.into() => {
                Arm::False
            }
            _ => Arm::Other,
        }
    }

    fn fold(
        &self,
        whole: &Value,
        condition: &Value,
        then_arm: (Arm, &Value),
        else_arm: (Arm, &Value),
        integer: bool,
    ) -> Option<Value> {
        let condition = condition.clone();
        let as_bool = |arm: &Value| {
            if integer {
                self.truth_of(arm)
            } else {
                Some(arm.clone())
            }
        };
        match (then_arm, else_arm) {
            ((Arm::True, _), (Arm::False, _)) => Some(condition),
            ((Arm::False, _), (Arm::True, _)) => Some(self.negate(whole, condition)),
            ((Arm::True, _), (Arm::Other, other)) => {
                Some(logical(whole, LogicalOp::Or, condition, as_bool(other)?))
            }
            ((Arm::Other, other), (Arm::False, _)) => {
                Some(logical(whole, LogicalOp::And, condition, as_bool(other)?))
            }
            _ => None,
        }
    }
}

fn zero(value: &Value) -> bool {
    matches!(&value.node.value, ValueKind::Constant(Number::Integer(zero)) if *zero == 0u32.into())
}

fn integer(ty: &Type) -> bool {
    matches!(ty, Type::Numeric(NumericType::Integer { .. }))
}

fn ordered(ty: &Type) -> bool {
    matches!(
        ty,
        Type::Bool | Type::Numeric(NumericType::Integer { .. }) | Type::Pointer { .. }
    )
}

fn inverse(op: CompareOp) -> CompareOp {
    match op {
        CompareOp::Eq => CompareOp::Ne,
        CompareOp::Ne => CompareOp::Eq,
        CompareOp::Lt => CompareOp::Ge,
        CompareOp::Le => CompareOp::Gt,
        CompareOp::Gt => CompareOp::Le,
        CompareOp::Ge => CompareOp::Lt,
    }
}

fn not(anchor: &Value, operand: Value) -> Value {
    Value {
        ty: Type::Bool,
        node: anchor.node.derive(ValueKind::Unary {
            op: UnaryArithOp::Not,
            operand: Box::new(operand),
            semantics: ArithSema::Exact,
        }),
    }
}

fn logical(anchor: &Value, op: LogicalOp, left: Value, right: Value) -> Value {
    Value {
        ty: Type::Bool,
        node: anchor.node.derive(ValueKind::Logical {
            op,
            left: Box::new(left),
            right: Box::new(right),
        }),
    }
}

fn from_bool(anchor: &Value, operand: Value) -> Value {
    Value {
        ty: anchor.ty.clone(),
        node: anchor.node.clone().with_value(ValueKind::Convert {
            kind: ConversionKind::FromBool,
            operand: Box::new(operand),
            reason: ConversionReason::Assign,
            semantics: ConversionSema::Exact,
        }),
    }
}
