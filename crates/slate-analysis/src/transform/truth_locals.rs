use foldhash::{HashMap, HashSet};
use slate_parser::ast::MacroExpansionId;
use slate_parser::ir::*;

pub(crate) fn transform(module: &mut Module) {
    let Module {
        functions,
        macro_values,
        ..
    } = module;
    for function in functions.iter_mut() {
        let Some(body) = &mut function.value.body else {
            continue;
        };
        let mut scan = Scan {
            macro_values,
            depth: 0,
            locals: HashMap::default(),
            tainted: HashSet::default(),
        };
        walk_body_with_macros(body, macro_values, &mut scan);
        let truth = scan.truth_locals();
        if !truth.is_empty() {
            walk_body(body, &mut Retype { truth });
        }
    }
}

#[derive(Default)]
struct Usage {
    places: usize,
    reads: usize,
    writes: usize,
    truth_uses: usize,
    copies: Vec<BindingId>,
    definitions: Vec<Option<Vec<BindingId>>>,
}

impl Usage {
    fn pays(&self, truth: &HashSet<BindingId>) -> bool {
        let truth_uses = self.truth_uses
            + self
                .copies
                .iter()
                .filter(|target| truth.contains(*target))
                .count();
        truth_uses * 2 > self.reads
    }
}

struct Scan<'a> {
    macro_values: &'a MacroValues,
    depth: usize,
    locals: HashMap<BindingId, Usage>,
    tainted: HashSet<BindingId>,
}

impl Scan<'_> {
    fn truth_locals(self) -> HashSet<BindingId> {
        let mut truth: HashSet<BindingId> = self
            .locals
            .iter()
            .filter(|(id, usage)| {
                !self.tainted.contains(*id)
                    && usage.places == usage.reads + usage.writes
                    && usage.definitions.iter().all(Option::is_some)
            })
            .map(|(id, _)| *id)
            .collect();
        loop {
            let dropped: Vec<BindingId> = truth
                .iter()
                .filter(|id| {
                    let usage = &self.locals[*id];
                    !usage.pays(&truth)
                        || usage
                            .definitions
                            .iter()
                            .flatten()
                            .flatten()
                            .any(|source| !truth.contains(source))
                })
                .copied()
                .collect();
            if dropped.is_empty() {
                break;
            }
            for id in dropped {
                truth.remove(&id);
            }
        }
        truth
    }

    fn define(&mut self, id: BindingId, definition: &Value) {
        if self.locals.contains_key(&id)
            && let Some(source) = read_binding(definition)
            && let Some(usage) = self.locals.get_mut(&source)
        {
            usage.copies.push(id);
        }
        if let Some(usage) = self.locals.get_mut(&id) {
            let mut sources = Vec::new();
            let proven = zero_or_one(definition, self.macro_values, &mut sources);
            usage.definitions.push(proven.then_some(sources));
        }
    }
}

fn zero_or_one(value: &Value, macro_values: &MacroValues, sources: &mut Vec<BindingId>) -> bool {
    if macro_values.contains_key(&value.node.id) {
        return false;
    }
    match &value.node.value {
        ValueKind::Constant(number) => constant(number).is_some_and(|n| n <= 1),
        ValueKind::Convert {
            kind: ConversionKind::FromBool,
            ..
        } => true,
        ValueKind::Read {
            place:
                Place {
                    kind: PlaceKind::Binding(source),
                    ..
                },
            ordering: None,
        } => {
            sources.push(*source);
            true
        }
        ValueKind::Conditional {
            then_value,
            else_value,
            ..
        } => {
            zero_or_one(then_value, macro_values, sources)
                && zero_or_one(else_value, macro_values, sources)
        }
        _ => false,
    }
}

fn constant(number: &Number) -> Option<u8> {
    match number {
        Number::Integer(n) => u8::try_from(n).ok(),
        Number::SignedInteger(n) => u8::try_from(n).ok(),
        _ => None,
    }
}

fn read_binding(value: &Value) -> Option<BindingId> {
    match &value.node.value {
        ValueKind::Read {
            place:
                Place {
                    kind: PlaceKind::Binding(id),
                    ..
                },
            ordering: None,
        } => Some(*id),
        _ => None,
    }
}

fn is_zero(value: &Value) -> bool {
    matches!(&value.node.value, ValueKind::Constant(number) if constant(number) == Some(0))
}

fn tested_against_zero<'v>(left: &'v Value, right: &'v Value) -> Option<&'v Value> {
    match (is_zero(left), is_zero(right)) {
        (false, true) => Some(left),
        (true, false) => Some(right),
        _ => None,
    }
}

fn nests(statement: &Statement) -> bool {
    matches!(
        statement,
        Statement::NestedFunction(_) | Statement::OmpParallel(_) | Statement::OmpFor(_)
    )
}

impl Visitor for Scan<'_> {
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

    fn enter_place_hole(&mut self, _expansion: MacroExpansionId, _place: &Place) {
        self.depth += 1;
    }

    fn leave_place_hole(&mut self, _expansion: MacroExpansionId) {
        self.depth -= 1;
    }

    fn statement(&mut self, statement: &Statement) {
        if nests(statement) {
            self.depth += 1;
        }
        match statement {
            Statement::Let(variable)
                if self.depth == 0
                    && variable.storage == StorageDuration::Automatic
                    && matches!(variable.ty, Type::Numeric(NumericType::Integer { .. }))
                    && !variable.access.volatile
                    && !variable.access.atomic
                    && variable.register.is_none()
                    && variable.cleanup.is_none()
                    && !variable.byref =>
            {
                let mut usage = Usage::default();
                if variable.initializer.is_none() {
                    usage
                        .definitions
                        .push((variable.auto_init == Some(AutoInit::Zero)).then(Vec::new));
                }
                self.locals.insert(variable.id, usage);
                if let Some(initializer) = &variable.initializer {
                    self.define(variable.id, initializer);
                }
            }
            Statement::Write {
                place:
                    Place {
                        kind: PlaceKind::Binding(id),
                        ..
                    },
                value,
                ordering: None,
                ..
            } => {
                if let Some(usage) = self.locals.get_mut(id) {
                    usage.writes += 1;
                }
                self.define(*id, value);
            }
            _ => {}
        }
    }

    fn rewrite_statement(&mut self, statement: &mut Statement) {
        if nests(statement) {
            self.depth -= 1;
        }
    }

    fn value_kind(&mut self, kind: &ValueKind) {
        match kind {
            ValueKind::Read {
                place:
                    Place {
                        kind: PlaceKind::Binding(id),
                        ..
                    },
                ordering: None,
            } => {
                if let Some(usage) = self.locals.get_mut(id) {
                    usage.reads += 1;
                }
            }
            ValueKind::Compare {
                op: CompareOp::Eq | CompareOp::Ne,
                left,
                right,
                ..
            } => {
                if let Some(id) = tested_against_zero(left, right).and_then(read_binding)
                    && let Some(usage) = self.locals.get_mut(&id)
                {
                    usage.truth_uses += 1;
                }
            }
            _ => {}
        }
    }

    fn binding(&mut self, binding: BindingId) {
        if let Some(usage) = self.locals.get_mut(&binding) {
            usage.places += 1;
        }
        if self.depth > 0 {
            self.tainted.insert(binding);
        }
    }
}

struct Retype {
    truth: HashSet<BindingId>,
}

impl Retype {
    fn bool_read(&self, value: &Value) -> Option<()> {
        match &value.node.value {
            ValueKind::Convert {
                kind: ConversionKind::FromBool,
                operand,
                ..
            } if operand.ty == Type::Bool => {
                read_binding(operand).filter(|id| self.truth.contains(id))?;
                Some(())
            }
            _ => None,
        }
    }
}

impl Visitor for Retype {
    fn rewrite_place(&mut self, place: &mut Place) {
        if let PlaceKind::Binding(id) = place.kind
            && self.truth.contains(&id)
        {
            place.ty = Type::Bool;
        }
    }

    fn rewrite_value(&mut self, value: &mut Value) {
        if value.ty != Type::Bool && read_binding(value).is_some_and(|id| self.truth.contains(&id))
        {
            let kind = std::mem::replace(&mut value.node.value, ValueKind::Void);
            let read = Value {
                ty: Type::Bool,
                node: value.node.derive(kind),
            };
            value.node.value = ValueKind::Convert {
                kind: ConversionKind::FromBool,
                operand: Box::new(read),
                reason: ConversionReason::Assign,
                semantics: ConversionSema::Exact,
            };
            return;
        }
        let ValueKind::Compare {
            op: op @ (CompareOp::Eq | CompareOp::Ne),
            left,
            right,
            ..
        } = &mut value.node.value
        else {
            return;
        };
        let op = *op;
        let tested = match (is_zero(left), is_zero(right)) {
            (false, true) => left,
            (true, false) => right,
            _ => return,
        };
        if self.bool_read(tested).is_none() {
            return;
        }
        let ValueKind::Convert { operand, .. } =
            std::mem::replace(&mut tested.node.value, ValueKind::Void)
        else {
            return;
        };
        let read = *operand;
        value.node.value = match op {
            CompareOp::Ne => read.node.value,
            _ => ValueKind::Unary {
                op: UnaryArithOp::Not,
                operand: Box::new(read),
                semantics: ArithSema::Exact,
            },
        };
    }

    fn rewrite_statement(&mut self, statement: &mut Statement) {
        match statement {
            Statement::Let(variable) if self.truth.contains(&variable.id) => {
                variable.ty = Type::Bool;
                if let Some(initializer) = &mut variable.initializer {
                    to_bool(initializer);
                }
            }
            Statement::Write {
                place:
                    Place {
                        kind: PlaceKind::Binding(id),
                        ..
                    },
                value,
                ..
            } if self.truth.contains(id) => to_bool(value),
            _ => {}
        }
    }
}

fn to_bool(value: &mut Value) {
    if matches!(
        value.node.value,
        ValueKind::Convert {
            kind: ConversionKind::FromBool,
            ..
        }
    ) {
        let ValueKind::Convert { operand, .. } =
            std::mem::replace(&mut value.node.value, ValueKind::Void)
        else {
            return;
        };
        *value = *operand;
        return;
    }
    value.ty = Type::Bool;
    match &mut value.node.value {
        ValueKind::Constant(number) => {
            let one = constant(number) == Some(1);
            *number = Number::Bool(one);
        }
        ValueKind::Conditional {
            then_value,
            else_value,
            ..
        } => {
            to_bool(then_value);
            to_bool(else_value);
        }
        _ => {}
    }
}
