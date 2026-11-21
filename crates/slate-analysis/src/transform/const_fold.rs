use num_bigint::{BigInt, Sign};
use slate_parser::ast::MacroExpansionId;
use slate_parser::ir::*;

pub(crate) fn transform(module: &mut Module) {
    let Module {
        globals,
        functions,
        macro_values,
        ..
    } = module;
    let mut fold = Fold {
        macro_values,
        depth: 0,
    };
    for global in globals {
        if let Some(initializer) = &mut global.value.variable.initializer {
            walk_value_with_macros(initializer, macro_values, &mut fold);
        }
    }
    for function in functions {
        if let Some(body) = &mut function.value.body {
            walk_body_with_macros(body, macro_values, &mut fold);
        }
    }
}

struct Fold<'a> {
    macro_values: &'a MacroValues,
    depth: usize,
}

impl Visitor for Fold<'_> {
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
        if let Some(holds) = constant_comparison(value, self.macro_values) {
            value.node.value = ValueKind::Constant(Number::Bool(holds));
            return;
        }
        let Some((width, signed)) = integer(&value.ty) else {
            return;
        };
        let folded = match &value.node.value {
            ValueKind::Convert {
                kind: ConversionKind::Widen | ConversionKind::Truncate | ConversionKind::Reinterpret,
                operand,
                reason,
                ..
            } if *reason != ConversionReason::Explicit
                && integer(&operand.ty).is_some()
                && !self.macro_values.contains_key(&operand.node.id) =>
            {
                constant(operand).map(|operand| wrap(operand, width, signed))
            }
            ValueKind::Unary {
                op: UnaryArithOp::Neg,
                operand,
                ..
            } if signed && !self.macro_values.contains_key(&operand.node.id) => constant(operand)
                .map(|operand| -operand)
                .filter(|negated| *negated >= -(BigInt::from(1) << (width - 1))),
            _ => None,
        };
        if let Some(folded) = folded {
            value.node.value = ValueKind::Constant(number(folded));
        }
    }
}

pub(super) fn constant_comparison(value: &Value, macro_values: &MacroValues) -> Option<bool> {
    let ValueKind::Compare {
        op,
        left,
        right,
        exceptions: None,
        reason,
    } = &value.node.value
    else {
        return None;
    };
    if *reason == Some(ConversionReason::Explicit)
        || integer(&left.ty).is_none()
        || macro_values.contains_key(&left.node.id)
        || macro_values.contains_key(&right.node.id)
    {
        return None;
    }
    let (left, right) = (constant(left)?, constant(right)?);
    Some(match op {
        CompareOp::Eq => left == right,
        CompareOp::Ne => left != right,
        CompareOp::Lt => left < right,
        CompareOp::Le => left <= right,
        CompareOp::Gt => left > right,
        CompareOp::Ge => left >= right,
    })
}

fn integer(ty: &Type) -> Option<(u32, bool)> {
    match ty {
        Type::Numeric(NumericType::Integer { width, signed, .. }) if *width <= 128 => {
            Some((*width, *signed))
        }
        _ => None,
    }
}

fn constant(value: &Value) -> Option<BigInt> {
    match &value.node.value {
        ValueKind::Constant(Number::Integer(magnitude)) => Some(BigInt::from(magnitude.clone())),
        ValueKind::Constant(Number::SignedInteger(signed)) => Some(signed.clone()),
        _ => None,
    }
}

fn wrap(value: BigInt, width: u32, signed: bool) -> BigInt {
    let modulus = BigInt::from(1) << width;
    let mut wrapped = ((value % &modulus) + &modulus) % &modulus;
    if signed && wrapped >= (BigInt::from(1) << (width - 1)) {
        wrapped -= modulus;
    }
    wrapped
}

fn number(value: BigInt) -> Number {
    match value.sign() {
        Sign::Minus => Number::SignedInteger(value),
        _ => Number::Integer(value.magnitude().clone()),
    }
}
