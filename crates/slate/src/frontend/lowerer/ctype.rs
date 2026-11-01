use super::*;
use slate_parser::ir::{ArithOp, CompareOp};

const GLIBC_MASKS: &[(i128, &str)] = &[
    (256, "isupper"),
    (512, "islower"),
    (1024, "isalpha"),
    (2048, "isdigit"),
    (4096, "isxdigit"),
    (8192, "isspace"),
    (16384, "isprint"),
    (32768, "isgraph"),
    (1, "isblank"),
    (2, "iscntrl"),
    (4, "ispunct"),
    (8, "isalnum"),
];

const FREEBSD_MASKS: &[(i128, &str)] = &[
    (0x0040_0500, "isalnum"),
    (0x0000_0100, "isalpha"),
    (0x0000_0200, "iscntrl"),
    (0x0000_0400, "isdigit"),
    (0x0000_0800, "isgraph"),
    (0x0000_1000, "islower"),
    (0x0004_0000, "isprint"),
    (0x0000_2000, "ispunct"),
    (0x0000_4000, "isspace"),
    (0x0000_8000, "isupper"),
    (0x0001_0000, "isxdigit"),
    (0x0002_0000, "isblank"),
];

struct MuslRange {
    name: &'static str,
    folds_case: bool,
    start: i128,
    length: i128,
}

const MUSL_RANGES: &[MuslRange] = &[
    MuslRange {
        name: "isalpha",
        folds_case: true,
        start: 97,
        length: 26,
    },
    MuslRange {
        name: "isdigit",
        folds_case: false,
        start: 48,
        length: 10,
    },
    MuslRange {
        name: "isgraph",
        folds_case: false,
        start: 33,
        length: 94,
    },
    MuslRange {
        name: "islower",
        folds_case: false,
        start: 97,
        length: 26,
    },
    MuslRange {
        name: "isprint",
        folds_case: false,
        start: 32,
        length: 95,
    },
    MuslRange {
        name: "isupper",
        folds_case: false,
        start: 65,
        length: 26,
    },
];

fn peel_conversions(mut value: &ir::Value) -> &ir::Value {
    while let ValueKind::Copy { operand, .. } | ValueKind::Convert { operand, .. } =
        &value.node.value
    {
        value = operand;
    }
    value
}

fn mask_name(masks: &[(i128, &'static str)], mask: i128) -> Option<&'static str> {
    masks
        .iter()
        .find(|(known, _)| *known == mask)
        .map(|(_, name)| *name)
}

impl Tables<'_> {
    fn integer_width(&self, ty: &ir::Type) -> Option<u32> {
        match self.resolve_type(ty) {
            ir::Type::Numeric(ir::NumericType::Integer { width, .. }) => Some(*width),
            _ => None,
        }
    }

    fn keeps_low_bits(&self, mut value: &ir::Value, bits: u32) -> bool {
        loop {
            if self
                .integer_width(&value.ty)
                .is_none_or(|width| width < bits)
            {
                return false;
            }
            match &value.node.value {
                ValueKind::Copy { operand, .. } | ValueKind::Convert { operand, .. } => {
                    value = operand;
                }
                _ => return true,
            }
        }
    }

    fn constant_keeping_bits(&self, value: &ir::Value, bits: u32) -> Option<i128> {
        if !self.keeps_low_bits(value, bits) {
            return None;
        }
        match &peel_conversions(value).node.value {
            ValueKind::Constant(ir::Number::Integer(number)) => number.to_string().parse().ok(),
            ValueKind::Constant(ir::Number::SignedInteger(number)) => {
                number.to_string().parse().ok()
            }
            ValueKind::Arith {
                op: ArithOp::Or,
                left,
                right,
                ..
            } => Some(
                self.constant_keeping_bits(left, bits)?
                    | self.constant_keeping_bits(right, bits)?,
            ),
            _ => None,
        }
    }

    fn glibc_table_test<'v>(&self, value: &'v ir::Value) -> Option<(&'static str, &'v ir::Value)> {
        let table = self.stdio.glibc_ctype_table?;
        let ValueKind::Arith {
            op: ArithOp::And,
            left,
            right,
            ..
        } = &value.node.value
        else {
            return None;
        };
        let name = mask_name(GLIBC_MASKS, self.constant_keeping_bits(right, 16)?)?;
        if !self.keeps_low_bits(left, 16) {
            return None;
        }
        let ValueKind::Read { place, .. } = &peel_conversions(left).node.value else {
            return None;
        };
        let PlaceKind::Deref(entry) = &place.kind else {
            return None;
        };
        let ValueKind::PointerOffset {
            pointer,
            amount,
            subtract: false,
            ..
        } = &entry.node.value
        else {
            return None;
        };
        let ValueKind::Read { place, .. } = &pointer.node.value else {
            return None;
        };
        let PlaceKind::Deref(location) = &place.kind else {
            return None;
        };
        let ValueKind::Call {
            callee: ir::Callee::Direct(id),
            ..
        } = &location.node.value
        else {
            return None;
        };
        (*id == table && self.matches_int(&amount.ty)).then_some((name, &**amount))
    }

    fn matches_int(&self, ty: &ir::Type) -> bool {
        matches!(
            self.resolve_type(ty),
            ir::Type::Numeric(ir::NumericType::Integer {
                width: 32,
                signed: true,
                ..
            })
        )
    }

    fn freebsd_table_test<'v>(
        &self,
        value: &'v ir::Value,
    ) -> Option<(&'static str, &'v ir::Value)> {
        let ValueKind::Call {
            callee: ir::Callee::Direct(id),
            arguments,
            ..
        } = &value.node.value
        else {
            return None;
        };
        let [argument, mask] = arguments.as_slice() else {
            return None;
        };
        (Some(*id) == self.stdio.freebsd_ctype_test)
            .then(|| self.constant_keeping_bits(mask, 32))
            .flatten()
            .and_then(|mask| mask_name(FREEBSD_MASKS, mask))
            .map(|name| (name, argument))
    }

    fn is_musl_macro(&self, value: &ir::Value) -> bool {
        let ValueKind::Conditional {
            condition,
            then_value,
            else_value,
        } = &value.node.value
        else {
            return false;
        };
        if !self.target.triple.contains("musl") {
            return false;
        }
        let ValueKind::Compare {
            op: CompareOp::Ne,
            left,
            right,
            ..
        } = &condition.node.value
        else {
            return false;
        };
        if self.constant_keeping_bits(left, 32) != Some(0)
            || self.constant_keeping_bits(right, 32) != Some(0)
        {
            return false;
        }
        let ValueKind::Call {
            callee: ir::Callee::Direct(id),
            arguments,
            ..
        } = &then_value.node.value
        else {
            return false;
        };
        let (Some(name), [argument]) = (self.stdio.functions.get(id), arguments.as_slice()) else {
            return false;
        };
        let Some(range) = MUSL_RANGES.iter().find(|range| range.name == *name) else {
            return false;
        };
        let ValueKind::Compare {
            op: CompareOp::Lt,
            left: offset,
            right: length,
            ..
        } = &peel_conversions(else_value).node.value
        else {
            return false;
        };
        let ValueKind::Arith {
            op: ArithOp::Sub,
            left: operand,
            right: start,
            ..
        } = &offset.node.value
        else {
            return false;
        };
        if self.constant_keeping_bits(start, 32) != Some(range.start)
            || self.constant_keeping_bits(length, 32) != Some(range.length)
        {
            return false;
        }
        let operand = if range.folds_case {
            let ValueKind::Arith {
                op: ArithOp::Or,
                left,
                right,
                ..
            } = &operand.node.value
            else {
                return false;
            };
            if self.constant_keeping_bits(right, 32) != Some(32) {
                return false;
            }
            left
        } else {
            operand
        };
        let ValueKind::Convert { operand, .. } = &operand.node.value else {
            return false;
        };
        operand.to_string() == argument.to_string()
    }
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn lower_ctype_macro(&mut self, value: &ir::Value) -> Result<Option<Expr>> {
        let tables = self.tables;
        if tables.stdio.functions.is_empty() {
            return Ok(None);
        }
        if tables.is_musl_macro(value) {
            let ValueKind::Conditional { then_value, .. } = &value.node.value else {
                return Ok(None);
            };
            return self.lower_value_node(then_value).map(Some);
        }
        let Some((name, argument)) = tables
            .glibc_table_test(value)
            .or_else(|| tables.freebsd_table_test(value))
        else {
            return Ok(None);
        };
        let call = Expr::Call {
            func: Box::new(stdio::support_path(name)),
            args: vec![self.lower_value(argument)?],
            binding: CallBinding::Direct {
                identity: stdio::ctype_identity(name),
                canonical_type: None,
                trusted_declaration: None,
                trusted_headers: Default::default(),
            },
        };
        Ok(Some(Expr::Unsafe(Box::new(rust::Block {
            stmts: Vec::new(),
            tail: Some(Box::new(call)),
        }))))
    }
}
