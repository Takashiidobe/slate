use slate_parser::ir::{self, BindingId, FloatType, NumericType, Type, TypeDefinitionKind, TypeId};
use slate_parser::target_info::TargetOs;
use std::collections::{BTreeSet, HashMap, HashSet};

pub struct Unit<'a> {
    pub module: &'a ir::Module,
    pub address_taken: &'a HashSet<BindingId>,
}

#[derive(Clone, Copy, PartialEq, Eq, Hash)]
enum Scalar {
    Void,
    Pointer,
    Integer(u32),
    Float(u32),
}

#[derive(PartialEq, Eq, Hash)]
struct ExactShape {
    variadic: bool,
    result: Scalar,
    parameters: Vec<Scalar>,
}

enum Shape {
    Exact(ExactShape),
    Aggregate {
        variadic: bool,
        arity: std::ops::RangeInclusive<usize>,
    },
}

impl Shape {
    fn variadic(&self) -> bool {
        match self {
            Shape::Exact(shape) => shape.variadic,
            Shape::Aggregate { variadic, .. } => *variadic,
        }
    }

    fn arity(&self) -> std::ops::RangeInclusive<usize> {
        match self {
            Shape::Exact(shape) => shape.parameters.len()..=shape.parameters.len(),
            Shape::Aggregate { arity, .. } => arity.clone(),
        }
    }
}

struct Definition<'a> {
    unit: usize,
    function: &'a ir::Function,
    shape: Shape,
}

pub fn mergeable_address_taken(units: &[Unit]) -> Vec<BTreeSet<String>> {
    let mut protected = vec![BTreeSet::new(); units.len()];
    let mut definitions = Vec::new();
    let mut taken_definitions = HashSet::new();
    let mut taken_symbols = HashSet::new();
    for (index, unit) in units.iter().enumerate() {
        if !matches!(
            unit.module.target.os,
            TargetOs::Linux | TargetOs::Android | TargetOs::FreeBsd
        ) {
            return protected;
        }
        let types: HashMap<TypeId, &TypeDefinitionKind> = unit
            .module
            .types
            .iter()
            .map(|definition| (definition.value.id, &definition.value.kind))
            .collect();
        for function in &unit.module.functions {
            let function = &function.value;
            let taken = unit.address_taken.contains(&function.id);
            if function.body.is_none() {
                if taken && matches!(function.linkage, ir::Linkage::External) {
                    taken_symbols.insert(symbol(function));
                }
                continue;
            }
            if taken {
                taken_definitions.insert((index, function.id));
            }
            definitions.push(Definition {
                unit: index,
                function,
                shape: shape(function, &types),
            });
        }
    }
    let mut exact: HashMap<&ExactShape, usize> = HashMap::new();
    let mut by_arity: HashMap<(bool, usize), usize> = HashMap::new();
    let mut aggregate_by_arity: HashMap<(bool, usize), usize> = HashMap::new();
    for definition in &definitions {
        let variadic = definition.shape.variadic();
        match &definition.shape {
            Shape::Exact(shape) => {
                *exact.entry(shape).or_default() += 1;
                *by_arity
                    .entry((variadic, shape.parameters.len()))
                    .or_default() += 1;
            }
            Shape::Aggregate { arity, .. } => {
                for count in arity.clone() {
                    *aggregate_by_arity.entry((variadic, count)).or_default() += 1;
                }
            }
        }
    }
    for definition in &definitions {
        let function = definition.function;
        let taken = taken_definitions.contains(&(definition.unit, function.id))
            || (matches!(function.linkage, ir::Linkage::External)
                && taken_symbols.contains(symbol(function)));
        if !taken || function.symbol.section.is_some() {
            continue;
        }
        let variadic = definition.shape.variadic();
        let count = |table: &HashMap<(bool, usize), usize>, arity: usize| {
            table.get(&(variadic, arity)).copied().unwrap_or(0)
        };
        let mergeable = match &definition.shape {
            Shape::Exact(shape) => {
                exact[shape] > 1 || count(&aggregate_by_arity, shape.parameters.len()) > 0
            }
            Shape::Aggregate { .. } => definition
                .shape
                .arity()
                .any(|arity| count(&by_arity, arity) + count(&aggregate_by_arity, arity) > 1),
        };
        if mergeable {
            protected[definition.unit].insert(super::lowerer::function_rust_name(function));
        }
    }
    protected
}

fn symbol(function: &ir::Function) -> &str {
    function
        .symbol
        .asm_name
        .as_deref()
        .unwrap_or(&function.name)
}

fn shape(function: &ir::Function, types: &HashMap<TypeId, &TypeDefinitionKind>) -> Shape {
    let (parameters, variadic) = match &function.parameters {
        ir::Parameters::Prototype { fixed, variadic } => (
            fixed
                .iter()
                .map(|parameter| &parameter.value.ty)
                .collect::<Vec<_>>(),
            *variadic,
        ),
        ir::Parameters::Unprototyped => (Vec::new(), false),
    };
    let result = function
        .return_type
        .as_ref()
        .map_or(Some(Scalar::Void), |ty| scalar(ty, types));
    let scalars: Vec<Option<Scalar>> = parameters.iter().map(|ty| scalar(ty, types)).collect();
    let aggregates = scalars.iter().filter(|scalar| scalar.is_none()).count();
    match result {
        Some(result) if aggregates == 0 => Shape::Exact(ExactShape {
            variadic,
            result,
            parameters: scalars.into_iter().flatten().collect(),
        }),
        _ => Shape::Aggregate {
            variadic,
            arity: parameters.len() - aggregates..=parameters.len() + aggregates + 1,
        },
    }
}

fn scalar(ty: &Type, types: &HashMap<TypeId, &TypeDefinitionKind>) -> Option<Scalar> {
    match ty {
        Type::Void => Some(Scalar::Void),
        Type::Bool => Some(Scalar::Integer(1)),
        Type::Numeric(NumericType::Integer {
            width,
            bit_precise: false,
            ..
        }) => Some(Scalar::Integer(*width)),
        Type::Numeric(NumericType::Float(FloatType::F16 | FloatType::BF16)) => {
            Some(Scalar::Float(16))
        }
        Type::Numeric(NumericType::Float(FloatType::F32)) => Some(Scalar::Float(32)),
        Type::Numeric(NumericType::Float(FloatType::F64)) => Some(Scalar::Float(64)),
        Type::Pointer { .. } => Some(Scalar::Pointer),
        Type::Defined(id) => match types.get(id)? {
            TypeDefinitionKind::Alias(aliased) => scalar(aliased, types),
            TypeDefinitionKind::Enum {
                underlying: Some(underlying),
                ..
            } => scalar(underlying, types),
            TypeDefinitionKind::Enum { .. } | TypeDefinitionKind::Record { .. } => None,
        },
        _ => None,
    }
}
