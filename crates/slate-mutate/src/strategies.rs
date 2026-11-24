use std::collections::HashMap;
use std::fmt::Write;

use super::{Collector, Edit, Mutation, Rule, constant};
use slate_parser::ast::{Loc, NodeId, Span};
use slate_parser::ir::{
    self, ArithOp, ArithSema, BindingId, CompareOp, FloatType, Global, Module, Number, NumericType,
    Overflow, Place, PlaceKind, Statement, Type, Value, ValueKind,
};
use slate_parser::sema::ctype::{CTypeKind, Extent, IntRank};

impl Collector<'_> {
    pub(super) fn editable_location(&self, value: &Value) -> Option<Loc> {
        let span = &value.node;
        (span.spelling == span.expansion
            && span.macro_origin.is_none()
            && span.macro_nesting.is_none()
            && self.module.macro_provenance(span.id).is_empty())
        .then_some(span.spelling)
    }

    fn push(&mut self, rule: Rule, node: NodeId, location: Loc, edit: Edit, replacement: String) {
        self.mutations.push(Mutation {
            rule,
            location,
            function: self.function,
            node,
            edit,
            replacement,
        });
    }

    fn between(&self, value: &Value, left: &Value, right: &Value) -> Option<Loc> {
        let whole = self.editable_location(value)?;
        let left = self.editable_location(left)?;
        let right = self.editable_location(right)?;
        if whole.contains(left) && whole.contains(right) && left.end() < right.offset {
            Some(Loc::new(whole.file, left.end(), right.offset - left.end()))
        } else {
            None
        }
    }

    pub(super) fn binary_operator(
        &mut self,
        value: &Value,
        op: ArithOp,
        left: &Value,
        right: &Value,
        semantics: ArithSema,
    ) {
        if value.ty.integer_bits().is_none_or(|(_, signed)| signed)
            || semantics
                != (ArithSema::Integer {
                    overflow: Overflow::Wrap,
                })
            || !matches!(op, ArithOp::Add | ArithOp::Sub | ArithOp::Mul)
            || ![left, right].into_iter().all(simple_operand)
        {
            return;
        }
        let Some(location) = self.between(value, left, right) else {
            return;
        };
        for after in [ArithOp::Add, ArithOp::Sub, ArithOp::Mul] {
            if after != op {
                self.push(
                    Rule::BinaryOperator,
                    value.node.id,
                    location,
                    Edit::ReplaceBinaryOperator { before: op, after },
                    format!(" {} ", arith_symbol(after)),
                );
            }
        }
    }

    pub(super) fn negate_condition(&mut self, condition: &Value) {
        let condition = unwrap_value(condition);
        let ValueKind::Compare {
            op, left, right, ..
        } = &condition.node.value
        else {
            return;
        };
        if left.ty.integer_bits().is_none() || right.ty.integer_bits().is_none() {
            return;
        }
        let Some(location) = self.between(condition, left, right) else {
            return;
        };
        let after = match op {
            CompareOp::Eq => CompareOp::Ne,
            CompareOp::Ne => CompareOp::Eq,
            CompareOp::Lt => CompareOp::Ge,
            CompareOp::Le => CompareOp::Gt,
            CompareOp::Gt => CompareOp::Le,
            CompareOp::Ge => CompareOp::Lt,
        };
        self.push(
            Rule::NegateCondition,
            condition.node.id,
            location,
            Edit::NegateCondition { before: *op, after },
            format!(" {} ", compare_symbol(after)),
        );
    }

    pub(super) fn delete_statement(&mut self, statement: NodeId, call: &Value) {
        if let Some(location) = self.editable_location(call) {
            self.push(
                Rule::DeleteStatement,
                statement,
                location,
                Edit::DeleteStatement,
                "((void)0)".into(),
            );
        }
    }

    pub(super) fn printf<'a>(
        &mut self,
        format: &Value,
        args: &'a [Value],
        initializers: &HashMap<BindingId, &'a Value>,
    ) {
        let Some((_, format)) = literal_global(self.module, format) else {
            return;
        };
        let Some(specifiers) = printf_specifiers(format) else {
            return;
        };
        if specifiers.len() != args.len() {
            return;
        }
        for (specifier, value) in specifiers.into_iter().zip(args) {
            match specifier {
                b'd' if self.int_argument(value, true) => {
                    self.output(value, initializers);
                }
                b'u' if self.int_argument(value, false) => self.output(value, initializers),
                b'a' | b'A' | b'e' | b'E' | b'f' | b'F' | b'g' | b'G'
                    if float_format(&value.ty) == Some(FloatType::F64) =>
                {
                    self.output(value, initializers);
                }
                b'c' if self.int_argument(value, true) => self.output(value, initializers),
                b's' => self.string_argument(value),
                _ => {}
            }
        }
    }

    fn int_argument(&self, value: &Value, signed: bool) -> bool {
        self.module.c_types.value(value).is_some_and(|ty| {
            matches!(
                self.module.c_types.types.canonical_kind(ty),
                CTypeKind::Int { rank: IntRank::Int, signed: actual } if *actual == signed
            )
        })
    }

    pub(super) fn float_literal(&mut self, value: &Value) {
        let Some(location) = self.editable_location(value) else {
            return;
        };
        let ValueKind::Constant(before @ Number::FloatBits(_)) = &value.node.value else {
            return;
        };
        let values = match float_format(&value.ty) {
            Some(FloatType::F32) => [
                0.0,
                -0.0,
                1.0,
                -1.0,
                f32::from_bits(1),
                f32::MIN_POSITIVE,
                f32::MAX,
            ]
            .into_iter()
            .map(|number| (u128::from(number.to_bits()), format!("({number:e}f)")))
            .collect::<Vec<_>>(),
            Some(FloatType::F64) => [
                0.0,
                -0.0,
                1.0,
                -1.0,
                f64::from_bits(1),
                f64::MIN_POSITIVE,
                f64::MAX,
            ]
            .into_iter()
            .map(|number| (u128::from(number.to_bits()), format!("({number:e})")))
            .collect(),
            _ => return,
        };
        for (bits, replacement) in values {
            let after = Number::FloatBits(bits);
            if *before != after {
                self.push(
                    Rule::FloatBoundary,
                    value.node.id,
                    location,
                    Edit::ReplaceNumber {
                        before: before.clone(),
                        after,
                    },
                    replacement,
                );
            }
        }
        let (special_bits, suffix) = match float_format(&value.ty) {
            Some(FloatType::F32) => (
                [f32::INFINITY, f32::NEG_INFINITY, f32::NAN]
                    .map(|number| u128::from(number.to_bits())),
                "f",
            ),
            Some(FloatType::F64) => (
                [f64::INFINITY, f64::NEG_INFINITY, f64::NAN]
                    .map(|number| u128::from(number.to_bits())),
                "",
            ),
            _ => return,
        };
        let infinity = format!("__builtin_inf{suffix}()");
        for (bits, replacement) in special_bits.into_iter().zip([
            infinity.clone(),
            format!("(-{infinity})"),
            format!("__builtin_nan{suffix}(\"\")"),
        ]) {
            let after = Number::FloatBits(bits);
            if *before != after {
                self.push(
                    Rule::FloatSpecial,
                    value.node.id,
                    location,
                    Edit::ReplaceNumber {
                        before: before.clone(),
                        after,
                    },
                    replacement,
                );
            }
        }
    }

    pub(super) fn character_literal(&mut self, value: &Value) {
        let Some(location) = self.editable_location(value) else {
            return;
        };
        let ValueKind::Constant(before @ (Number::Integer(_) | Number::SignedInteger(_))) =
            &value.node.value
        else {
            return;
        };
        if !self.int_argument(value, true) {
            return;
        }
        for byte in [0u8, 0x7f, 0x80, 0xff] {
            let number = if self.module.target.char_signed {
                i128::from(byte as i8)
            } else {
                i128::from(byte)
            };
            if constant(value) != Some(number) {
                let after = if number < 0 {
                    Number::SignedInteger(number.into())
                } else {
                    Number::Integer(u128::from(byte).into())
                };
                self.push(
                    Rule::CharacterBoundary,
                    value.node.id,
                    location,
                    Edit::ReplaceNumber {
                        before: before.clone(),
                        after,
                    },
                    format!("'\\x{byte:02X}'"),
                );
            }
        }
    }

    pub(super) fn array_index(&mut self, place: &Place) {
        if let Some((id, length, amount)) = array_access(place)
            && self.arrays.contains(&id)
        {
            self.literal(
                amount,
                Rule::ArrayIndex,
                [0, 1, i128::from(length - 1)],
                |index| index < i128::from(length),
            );
        }
    }

    pub(super) fn null_pointer(&mut self, value: &Value) {
        if !matches!(value.ty, Type::Pointer { .. }) {
            return;
        }
        let value = unwrap_value(value);
        let place = match &value.node.value {
            ValueKind::AddressOf(place) | ValueKind::ArrayDecay { place, .. } => place,
            _ => return,
        };
        if !matches!(place.kind, PlaceKind::Binding(_)) || place.ty.is_void_or_function() {
            return;
        }
        let Some(location) = self.editable_location(value) else {
            return;
        };
        self.push(
            Rule::NullPointer,
            value.node.id,
            location,
            Edit::ReplacePointerWithNull,
            "((void *)0)".into(),
        );
    }

    pub(super) fn string_argument(&mut self, argument: &Value) {
        let decay = unwrap_value(argument);
        let Some(location) = self.editable_location(decay) else {
            return;
        };
        let Some((global, original)) = literal_global(self.module, decay) else {
            return;
        };
        let id = global.variable.id;
        if self.uses.get(&id) != Some(&1) {
            return;
        }
        let long = vec![b'A'; 4096];
        for bytes in [
            b"".as_slice(),
            b"ab\0cd",
            b"\xff",
            b"\xc3(",
            b"\xe2\x82\xac",
            &long,
        ] {
            let code_units: Vec<_> = bytes.iter().copied().map(u32::from).chain([0]).collect();
            if code_units == original {
                continue;
            }
            let replacement = if bytes == long {
                format!("\"{}\"", "A".repeat(4096))
            } else {
                let mut source = String::from("\"");
                for byte in bytes {
                    write!(source, "\\x{byte:02X}").unwrap();
                }
                source.push('"');
                source
            };
            self.push(
                Rule::StringBytes,
                decay.node.id,
                location,
                Edit::ReplaceString {
                    global: id,
                    bytes: code_units,
                },
                replacement,
            );
        }
    }
}

type BindingCounts = HashMap<BindingId, usize>;

pub(super) fn binding_uses(module: &Module) -> (BindingCounts, BindingCounts) {
    #[derive(Default)]
    struct Uses {
        all: BindingCounts,
        projected_reads: BindingCounts,
    }

    impl ir::ReadVisitor for Uses {
        fn place(&mut self, place: &Place) {
            if let PlaceKind::Binding(id) = place.kind {
                *self.all.entry(id).or_default() += 1;
            }
        }

        fn value(&mut self, value: &Value) {
            if let ValueKind::Read {
                place,
                ordering: None,
            } = &value.node.value
                && let Some((id, _, _)) = array_access(place)
            {
                *self.projected_reads.entry(id).or_default() += 1;
            }
        }
    }

    let mut uses = Uses::default();
    ir::walk_module_ref(module, &mut uses);
    (uses.all, uses.projected_reads)
}

fn array_access(place: &Place) -> Option<(BindingId, u64, &Value)> {
    let PlaceKind::Deref(address) = &place.kind else {
        return None;
    };
    let ValueKind::PointerOffset {
        pointer,
        amount,
        subtract: false,
        ..
    } = &address.node.value
    else {
        return None;
    };
    let ValueKind::ArrayDecay {
        place: base,
        length: Some(length @ 1..),
    } = &pointer.node.value
    else {
        return None;
    };
    let PlaceKind::Binding(id) = base.kind else {
        return None;
    };
    Some((id, *length, amount))
}

fn simple_operand(value: &Value) -> bool {
    match &unwrap_value(value).node.value {
        ValueKind::Constant(_) => true,
        ValueKind::Read {
            place,
            ordering: None,
        } => matches!(place.kind, PlaceKind::Binding(_)),
        _ => false,
    }
}

pub(super) fn literal_global<'a>(
    module: &'a Module,
    argument: &Value,
) -> Option<(&'a Global, &'a [u32])> {
    let ValueKind::ArrayDecay { place, .. } = &unwrap_value(argument).node.value else {
        return None;
    };
    let PlaceKind::Binding(binding) = place.kind else {
        return None;
    };
    let global = module
        .globals
        .iter()
        .find(|global| global.variable.id == binding && global.variable.name.starts_with(".str"))?;
    let initializer = global.variable.initializer.as_ref()?;
    let ValueKind::CodeUnits(bytes) = &initializer.node.value else {
        return None;
    };
    let Type::Array { element, .. } = &initializer.ty else {
        return None;
    };
    (element.integer_bits().is_some_and(|(bits, _)| bits == 8) && bytes.last() == Some(&0))
        .then_some((&global.value, bytes.as_slice()))
}

pub(super) fn apply_string(
    module: &mut Module,
    function: NodeId,
    node: NodeId,
    binding: BindingId,
    bytes: &[u32],
) -> bool {
    struct Resize {
        node: NodeId,
        binding: BindingId,
        length: u64,
        place: Option<NodeId>,
    }

    impl ir::Visitor for Resize {
        fn rewrite_value(&mut self, value: &mut Value) {
            if value.node.id == self.node
                && let ValueKind::ArrayDecay { place, length } = &mut value.node.value
                && matches!(place.kind, PlaceKind::Binding(id) if id == self.binding)
                && let Type::Array { length: extent, .. } = &mut place.ty
            {
                *length = Some(self.length);
                *extent = Some(self.length);
                self.place = Some(place.id);
            }
        }
    }

    let Some(global) = module
        .globals
        .iter_mut()
        .find(|global| global.variable.id == binding)
    else {
        return false;
    };
    let variable = &mut global.value.variable;
    let Some(initializer) = &mut variable.initializer else {
        return false;
    };
    let (
        Type::Array {
            length: variable_length,
            ..
        },
        Type::Array {
            length: value_length,
            ..
        },
        ValueKind::CodeUnits(original),
    ) = (
        &mut variable.ty,
        &mut initializer.ty,
        &mut initializer.node.value,
    )
    else {
        return false;
    };
    if *original == bytes {
        return false;
    }
    let Some(body) = module
        .functions
        .iter_mut()
        .find(|entry| entry.id == function)
        .and_then(|entry| entry.value.body.as_mut())
    else {
        return false;
    };
    let length = bytes.len() as u64;
    let mut resize = Resize {
        node,
        binding,
        length,
        place: None,
    };
    ir::walk_function_body(body, &mut resize);
    let Some(place) = resize.place else {
        return false;
    };
    *variable_length = Some(length);
    *value_length = Some(length);
    *original = bytes.to_vec();
    for id in [global.id, initializer.node.id, place] {
        if let Some(ty) = module.c_types.nodes.get_mut(&id) {
            resize_c_type(&mut module.c_types.types, ty, length);
        }
    }
    if let Some(ty) = module.c_types.bindings.get_mut(&binding) {
        resize_c_type(&mut module.c_types.types, ty, length);
    }
    true
}

fn resize_c_type(
    types: &mut slate_parser::sema::ctype::CTypes,
    ty: &mut slate_parser::sema::ctype::QualType,
    length: u64,
) {
    if let CTypeKind::Array { element, .. } = *types.canonical_kind(*ty) {
        ty.ty = types.intern(CTypeKind::Array {
            element,
            extent: Extent::Fixed(length),
        });
    }
}

pub(super) fn unwrap_value(mut value: &Value) -> &Value {
    while let ValueKind::Convert { operand, .. } | ValueKind::Copy { operand, .. } =
        &value.node.value
    {
        value = operand;
    }
    value
}

pub(super) fn constant_return(body: &[Span<Statement>]) -> bool {
    matches!(body, [statement] if matches!(&statement.value,
        Statement::Return(Some(value)) if constant(value).is_some()))
}

pub(super) fn float_format(ty: &Type) -> Option<FloatType> {
    match ty {
        Type::Numeric(NumericType::Float(format @ (FloatType::F32 | FloatType::F64))) => {
            Some(*format)
        }
        _ => None,
    }
}

pub(super) fn preserves_float_range(to: &Type, from: &Type) -> bool {
    match (float_format(to), float_format(from)) {
        (Some(to), Some(from)) => to == from || (to == FloatType::F64 && from == FloatType::F32),
        _ => false,
    }
}

pub(super) fn unwrap_float(mut value: &Value) -> &Value {
    while let ValueKind::Copy { operand, .. } | ValueKind::Convert { operand, .. } =
        &value.node.value
        && preserves_float_range(&value.ty, &operand.ty)
    {
        value = operand;
    }
    value
}

fn printf_specifiers(bytes: &[u32]) -> Option<Vec<u8>> {
    let mut chars = bytes.split_last()?.1.iter().copied();
    let mut result = Vec::new();
    while let Some(byte) = chars.next() {
        match byte {
            0 => return None,
            37 => {
                let mut specifier = u8::try_from(chars.next()?).ok()?;
                while specifier.is_ascii_digit() || b" -+#.".contains(&specifier) {
                    specifier = u8::try_from(chars.next()?).ok()?;
                }
                match specifier {
                    b'd' | b'u' | b'a' | b'A' | b'e' | b'E' | b'f' | b'F' | b'g' | b'G' | b'c'
                    | b's' => result.push(specifier),
                    b'%' => {}
                    _ => return None,
                }
            }
            _ => {}
        }
    }
    Some(result)
}

pub(super) fn source_token(rule: Rule, edit: &Edit, token: &str) -> bool {
    match edit {
        Edit::ReplaceBinaryOperator { before, .. } => token.trim() == arith_symbol(*before),
        Edit::NegateCondition { before, .. } => token.trim() == compare_symbol(*before),
        Edit::DeleteStatement => token
            .strip_prefix("puts")
            .is_some_and(|tail| tail.trim_start().starts_with('(') && tail.ends_with(')')),
        Edit::ReplaceString { .. } => token.starts_with('"') && token.ends_with('"'),
        Edit::ReplacePointerWithNull => {
            let token = token.strip_prefix('&').unwrap_or(token).trim();
            token.starts_with(|c: char| c.is_ascii_alphabetic() || c == '_')
                && token
                    .bytes()
                    .all(|byte| byte.is_ascii_alphanumeric() || byte == b'_')
        }
        Edit::ReplaceNumber { .. } if rule == Rule::CharacterBoundary => {
            token.starts_with('\'') && token.ends_with('\'')
        }
        Edit::ReplaceNumber { .. } if matches!(rule, Rule::FloatBoundary | Rule::FloatSpecial) => {
            float_token(token)
        }
        Edit::ReplaceNumber { .. } => {
            token.starts_with(|c: char| c.is_ascii_digit())
                && token
                    .bytes()
                    .all(|byte| byte.is_ascii_alphanumeric() || byte == b'\'')
        }
    }
}

fn float_token(token: &str) -> bool {
    let token = token.trim_end_matches(['f', 'F']).replace('\'', "");
    if let Some(token) = token
        .strip_prefix("0x")
        .or_else(|| token.strip_prefix("0X"))
    {
        let Some((significand, exponent)) = token.split_once(['p', 'P']) else {
            return false;
        };
        return exponent.parse::<i32>().is_ok()
            && significand.bytes().any(|byte| byte.is_ascii_hexdigit())
            && significand
                .bytes()
                .all(|byte| byte == b'.' || byte.is_ascii_hexdigit())
            && significand.bytes().filter(|&byte| byte == b'.').count() <= 1;
    }
    token.parse::<f64>().is_ok()
        && token.starts_with(|c: char| c.is_ascii_digit() || c == '.')
        && token.bytes().any(|byte| matches!(byte, b'.' | b'e' | b'E'))
}

fn arith_symbol(op: ArithOp) -> &'static str {
    match op {
        ArithOp::Add => "+",
        ArithOp::Sub => "-",
        ArithOp::Mul => "*",
        _ => unreachable!(),
    }
}

fn compare_symbol(op: CompareOp) -> &'static str {
    match op {
        CompareOp::Eq => "==",
        CompareOp::Ne => "!=",
        CompareOp::Lt => "<",
        CompareOp::Le => "<=",
        CompareOp::Gt => ">",
        CompareOp::Ge => ">=",
    }
}
