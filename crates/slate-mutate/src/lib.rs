use std::collections::{BTreeSet, HashMap, HashSet};

use slate_analysis::{
    FunctionIdentity, Known, UnitFacts, integer_constant, single_read_initializers,
};
use slate_parser::ast::{FileId, Loc, NodeId};
use slate_parser::ir::{
    self, ArithOp, ArithSema, BindingId, CompareOp, ConversionSema, Fits, Module, Number, Overflow,
    PlaceKind, Statement, StorageDuration, Type, Value, ValueKind,
};
use slate_parser::sema::ctype::{CTypeKind, IntRank};

mod strategies;
use strategies::{binding_uses, literal_global, source_token};

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Rule {
    IntegerBoundary,
    ArithmeticOperand,
    Divisor,
    ShiftAmount,
    ComparisonThreshold,
    CastBoundary,
    BinaryOperator,
    NegateCondition,
    DeleteStatement,
    FloatBoundary,
    FloatSpecial,
    StringBytes,
    ArrayIndex,
    CharacterBoundary,
    NullPointer,
}

impl Rule {
    pub fn strategy(self) -> &'static str {
        match self {
            Self::BinaryOperator => "operator",
            Self::NegateCondition => "condition",
            Self::DeleteStatement => "statement",
            Self::FloatBoundary | Self::FloatSpecial => "float",
            Self::StringBytes => "string",
            Self::ArrayIndex => "array",
            Self::CharacterBoundary => "char",
            Self::NullPointer => "pointer",
            _ => "integer",
        }
    }
}

#[derive(Debug, Clone)]
enum Edit {
    ReplaceNumber { before: Number, after: Number },
    ReplaceBinaryOperator { before: ArithOp, after: ArithOp },
    NegateCondition { before: CompareOp, after: CompareOp },
    DeleteStatement,
    ReplaceString { global: BindingId, bytes: Vec<u32> },
    ReplacePointerWithNull,
}

#[derive(Debug, Clone)]
pub struct Mutation {
    pub rule: Rule,
    pub location: Loc,
    function: NodeId,
    node: NodeId,
    edit: Edit,
    replacement: String,
}

impl Mutation {
    pub fn description(&self) -> String {
        match &self.edit {
            Edit::ReplaceString { bytes, .. } if bytes.len() > 80 => {
                format!("{} string bytes", bytes.len() - 1)
            }
            _ => self.replacement.clone(),
        }
    }

    /// Apply to the original module or to a clone that preserves its node IDs.
    pub fn apply(&self, module: &mut Module) -> bool {
        struct Replace<'a> {
            mutation: &'a Mutation,
            depth: usize,
            changed_depth: Option<usize>,
        }

        impl ir::Visitor for Replace<'_> {
            fn value_kind(&mut self, _: &ValueKind) {
                self.depth += 1;
            }

            fn rewrite_value(&mut self, value: &mut Value) {
                self.depth -= 1;
                if self.changed_depth.is_some_and(|depth| depth > self.depth) {
                    self.changed_depth = Some(self.depth);
                    if let ValueKind::Convert {
                        operand,
                        semantics: ConversionSema::Fits(fit),
                        ..
                    } = &mut value.node.value
                    {
                        let always = bounds(&operand.ty)
                            .is_some_and(|(min, max)| fits(&value.ty, min) && fits(&value.ty, max))
                            || constant(operand).is_some_and(|number| fits(&value.ty, number));
                        *fit = if always { Fits::Always } else { Fits::Unknown };
                    }
                }
                if value.node.id != self.mutation.node {
                    return;
                }
                match (&self.mutation.edit, &mut value.node.value) {
                    (Edit::ReplaceNumber { before, after }, ValueKind::Constant(number))
                        if before == number =>
                    {
                        *number = after.clone();
                    }
                    (
                        Edit::ReplaceBinaryOperator { before, after },
                        ValueKind::Arith { op, .. },
                    ) if before == op => {
                        *op = *after;
                    }
                    (Edit::NegateCondition { before, after }, ValueKind::Compare { op, .. })
                        if before == op =>
                    {
                        *op = *after;
                    }
                    (
                        Edit::ReplacePointerWithNull,
                        ValueKind::AddressOf(_) | ValueKind::ArrayDecay { .. },
                    ) => {
                        value.node.value = ValueKind::Null;
                    }
                    _ => return,
                }
                self.changed_depth = Some(self.depth);
            }
        }

        if let Edit::ReplaceString { global, bytes } = &self.edit {
            return strategies::apply_string(module, self.function, self.node, *global, bytes);
        }
        let Some(body) = module
            .functions
            .iter_mut()
            .find(|function| function.id == self.function)
            .and_then(|function| function.value.body.as_mut())
        else {
            return false;
        };
        if matches!(self.edit, Edit::DeleteStatement) {
            if let Some(index) = body.iter().position(|statement| statement.id == self.node) {
                body.remove(index);
                return true;
            }
            return false;
        }
        let mut replace = Replace {
            mutation: self,
            depth: 0,
            changed_depth: None,
        };
        ir::walk_function_body(body, &mut replace);
        replace.changed_depth.is_some()
    }

    /// Materialize the equivalent change for a C oracle using the original source.
    pub fn oracle_source(&self, file: FileId, source: &str) -> Option<String> {
        let location = self.location;
        if location.file != file {
            return None;
        }
        let range = location.offset..location.end();
        let token = source.get(range.clone())?;
        if !source_token(self.rule, &self.edit, token) {
            return None;
        }
        let mut output = source.to_owned();
        let line_breaks = token.bytes().filter(|&byte| byte == b'\n').count();
        let replacement = format!("{}{}", self.replacement, "\n".repeat(line_breaks));
        output.replace_range(range, &replacement);
        Some(output)
    }
}

/// Enumerate independent mutations for one module. Facts must describe that module.
pub fn candidates(module: &Module, facts: UnitFacts<'_>) -> Vec<Mutation> {
    let Some(main) = module
        .functions
        .iter()
        .find(|function| function.name == "main" && function.body.is_some())
    else {
        return Vec::new();
    };
    let Some(body) = &main.body else {
        return Vec::new();
    };
    let single_reads: HashSet<_> = single_read_initializers(body).collect();
    let mut initializers = HashMap::new();
    let (uses, projected_reads) = binding_uses(module);
    let mut collector = Collector {
        module,
        function: main.id,
        mutations: Vec::new(),
        uses,
        arrays: HashSet::new(),
    };
    for statement in body {
        match &statement.value {
            Statement::Let(variable)
                if variable.storage == StorageDuration::Automatic
                    && !variable.is_const
                    && !variable.constexpr
                    && variable.cleanup.is_none()
                    && !variable.byref
                    && single_reads.contains(&variable.id)
                    && let Some(initializer) = &variable.initializer =>
            {
                initializers.insert(variable.id, initializer);
            }
            Statement::Let(variable)
                if matches!(
                    variable.ty,
                    Type::Array {
                        length: Some(_),
                        ..
                    }
                ) && variable.initializer.is_some()
                    && collector
                        .uses
                        .get(&variable.id)
                        .is_some_and(|uses| projected_reads.get(&variable.id) == Some(uses)) =>
            {
                collector.arrays.insert(variable.id);
            }
            Statement::Return(Some(value)) => collector.output(value, &initializers),
            Statement::If {
                condition,
                then_body,
                else_body: Some(else_body),
                folded: None,
            } if strategies::constant_return(then_body)
                && strategies::constant_return(else_body) =>
            {
                collector.negate_condition(condition);
            }
            Statement::Expression(call)
                if let ValueKind::Call {
                    callee: ir::Callee::Direct(callee),
                    arguments,
                    ..
                } = &call.node.value =>
            {
                match facts.identity(*callee) {
                    FunctionIdentity::Known(Known::Puts)
                        if let [argument] = arguments.as_slice() =>
                    {
                        collector.string_argument(argument);
                        if literal_global(module, argument).is_some() {
                            collector.delete_statement(statement.id, call);
                        }
                    }
                    FunctionIdentity::Known(Known::Printf)
                        if let Some((format, args)) = arguments.split_first() =>
                    {
                        collector.printf(format, args, &initializers);
                    }
                    _ => {}
                }
            }
            _ => {}
        }
    }

    let mut seen = BTreeSet::new();
    collector.mutations.retain(|mutation| {
        seen.insert((
            mutation.location.file.0,
            mutation.location.offset,
            mutation.location.length,
            mutation.replacement.clone(),
        ))
    });
    let mut next_at_location = HashMap::new();
    collector.mutations.sort_by_cached_key(|mutation| {
        let ordinal = next_at_location.entry(mutation.location).or_insert(0);
        let key = *ordinal;
        *ordinal += 1;
        key
    });
    collector.mutations
}

struct Collector<'a> {
    module: &'a Module,
    function: NodeId,
    mutations: Vec<Mutation>,
    uses: HashMap<BindingId, usize>,
    arrays: HashSet<BindingId>,
}

impl Collector<'_> {
    fn output<'a>(&mut self, mut value: &'a Value, initializers: &HashMap<BindingId, &'a Value>) {
        let mut cast = None;
        loop {
            match &value.node.value {
                ValueKind::Read {
                    place,
                    ordering: None,
                } if let PlaceKind::Binding(id) = place.kind
                    && let Some(&initializer) = initializers.get(&id) =>
                {
                    value = initializer;
                }
                ValueKind::Copy { operand, .. } | ValueKind::Convert { operand, .. }
                    if (value.ty.integer_bits().is_some()
                        && operand.ty.integer_bits().is_some())
                        || strategies::preserves_float_range(&value.ty, &operand.ty) =>
                {
                    if matches!(
                        value.node.value,
                        ValueKind::Convert {
                            reason: ir::ConversionReason::Explicit,
                            ..
                        }
                    ) {
                        cast = cast.or_else(|| bounds(&value.ty));
                    }
                    value = operand;
                }
                _ => break,
            }
        }

        match &value.node.value {
            ValueKind::Constant(Number::FloatBits(_)) => self.float_literal(value),
            ValueKind::Constant(_) => {
                self.character_literal(value);
                if let Some((min, max)) = cast {
                    self.literal(
                        value,
                        Rule::CastBoundary,
                        [min - 1, min, min + 1, max - 1, max, max + 1],
                        |_| true,
                    );
                }
                self.literal(value, Rule::IntegerBoundary, boundaries(&value.ty), |_| {
                    true
                });
            }
            ValueKind::Compare {
                op, left, right, ..
            } => {
                self.negate_condition(value);
                if matches!(op, CompareOp::Eq | CompareOp::Ne) {
                    self.null_pointer(left);
                    self.null_pointer(right);
                }
                self.float_literal(strategies::unwrap_float(left));
                self.float_literal(strategies::unwrap_float(right));
                for (operand, other) in [(left, right), (right, left)] {
                    if let Some(threshold) = constant(other) {
                        self.literal(
                            operand,
                            Rule::ComparisonThreshold,
                            [threshold - 1, threshold, threshold + 1],
                            |candidate| fits(&operand.ty, candidate),
                        );
                    }
                }
            }
            ValueKind::Arith {
                op,
                left,
                right,
                semantics,
            } => {
                self.binary_operator(value, *op, left, right, *semantics);
                self.arithmetic(*op, left, right, *semantics, &value.ty);
            }
            ValueKind::Read {
                place,
                ordering: None,
            } => self.array_index(place),
            _ => {}
        }
    }

    fn arithmetic(
        &mut self,
        op: ArithOp,
        left: &Value,
        right: &Value,
        semantics: ArithSema,
        result: &Type,
    ) {
        let Some((width, signed)) = result
            .integer_bits()
            .filter(|&(width, _)| (2..=64).contains(&width))
        else {
            return;
        };
        match op {
            ArithOp::Add | ArithOp::Sub | ArithOp::Mul => {
                let wraps = !signed
                    || semantics
                        == (ArithSema::Integer {
                            overflow: Overflow::Wrap,
                        });
                for (operand, other, swap) in [(left, right, false), (right, left, true)] {
                    let other = constant(other);
                    self.literal(
                        operand,
                        Rule::ArithmeticOperand,
                        boundaries(&operand.ty),
                        |candidate| {
                            fits(&operand.ty, candidate)
                                && (wraps
                                    || other.map_or_else(
                                        || match op {
                                            ArithOp::Add => candidate == 0,
                                            ArithOp::Sub => swap && candidate == 0,
                                            ArithOp::Mul => matches!(candidate, 0 | 1),
                                            _ => unreachable!(),
                                        },
                                        |other| {
                                            let (left, right) = if swap {
                                                (other, candidate)
                                            } else {
                                                (candidate, other)
                                            };
                                            let computed = match op {
                                                ArithOp::Add => left.checked_add(right),
                                                ArithOp::Sub => left.checked_sub(right),
                                                _ => left.checked_mul(right),
                                            };
                                            computed.is_some_and(|value| fits(result, value))
                                        },
                                    ))
                        },
                    );
                }
            }
            ArithOp::Div | ArithOp::Rem => {
                self.literal(right, Rule::Divisor, [1, -1, 2], |candidate| {
                    fits(&right.ty, candidate)
                        && (candidate != -1
                            || constant(left).is_some_and(|left| fits(result, -left)))
                });
            }
            ArithOp::Shl | ArithOp::Shr => {
                self.literal(
                    right,
                    Rule::ShiftAmount,
                    [0, 1, i128::from(width - 1)],
                    |candidate| {
                        fits(&right.ty, candidate)
                            && (!signed
                                || constant(left).is_some_and(|left| {
                                    left >= 0
                                        && (op == ArithOp::Shr || fits(result, left << candidate))
                                }))
                    },
                );
            }
            _ => {}
        }
    }

    fn literal(
        &mut self,
        mut value: &Value,
        rule: Rule,
        candidates: impl IntoIterator<Item = i128>,
        allowed: impl Fn(i128) -> bool,
    ) {
        let mut conversions = Vec::new();
        while let ValueKind::Copy { operand, .. } | ValueKind::Convert { operand, .. } =
            &value.node.value
        {
            if bounds(&value.ty).is_none() {
                return;
            }
            conversions.push(&value.ty);
            value = operand;
        }
        let ValueKind::Constant(before @ (Number::Integer(_) | Number::SignedInteger(_))) =
            &value.node.value
        else {
            return;
        };
        let Some(location) = self.editable_location(value) else {
            return;
        };
        let Some(original) = constant(value) else {
            return;
        };
        let Some(ctype) = self.module.c_types.value(value) else {
            return;
        };
        let CTypeKind::Int { rank, signed } = *self.module.c_types.types.canonical_kind(ctype)
        else {
            return;
        };
        let rank_suffix = match rank {
            IntRank::Int => "",
            IntRank::Long => "L",
            IntRank::LongLong => "LL",
            _ => return,
        };
        let suffix = if signed {
            rank_suffix.to_owned()
        } else {
            format!("U{rank_suffix}")
        };

        for candidate in candidates {
            if candidate == original
                || !fits(&value.ty, candidate)
                || conversions.iter().any(|ty| !fits(ty, candidate))
                || !allowed(candidate)
            {
                continue;
            }
            let replacement =
                if candidate < 0 && bounds(&value.ty).is_some_and(|(min, _)| candidate == min) {
                    format!("(-{}{suffix} - 1{suffix})", -candidate - 1)
                } else if candidate < 0 {
                    format!("({candidate}{suffix})")
                } else {
                    format!("{candidate}{suffix}")
                };
            let after = if candidate < 0 {
                Number::SignedInteger(candidate.into())
            } else {
                Number::Integer((candidate as u128).into())
            };
            self.mutations.push(Mutation {
                rule,
                location,
                function: self.function,
                node: value.node.id,
                edit: Edit::ReplaceNumber {
                    before: before.clone(),
                    after,
                },
                replacement,
            });
        }
    }
}

fn constant(value: &Value) -> Option<i128> {
    i128::try_from(integer_constant(value)?).ok()
}

fn bounds(ty: &Type) -> Option<(i128, i128)> {
    let (width, signed) = ty.integer_bits()?;
    if !(2..=64).contains(&width) {
        return None;
    }
    let limit = 1i128 << (width - u32::from(signed));
    Some((if signed { -limit } else { 0 }, limit - 1))
}

fn fits(ty: &Type, value: i128) -> bool {
    bounds(ty).is_some_and(|(min, max)| (min..=max).contains(&value))
}

fn boundaries(ty: &Type) -> impl Iterator<Item = i128> {
    bounds(ty).into_iter().flat_map(|(min, max)| {
        [0, 1, -1, min, min + 1, max - 1, max]
            .into_iter()
            .chain(
                [8, 16, 24, 31, 32, 48, 63]
                    .into_iter()
                    .filter(move |_| min == 0)
                    .flat_map(|exponent| {
                        let power = 1i128 << exponent;
                        [power - 1, power, power + 1]
                    }),
            )
            .filter(move |value| (min..=max).contains(value))
    })
}
