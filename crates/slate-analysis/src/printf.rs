use foldhash::HashMap;
use slate_parser::ast::NodeId;
use slate_parser::ir::{self, Callee, ConversionKind, Place, PlaceKind, Value, ValueKind};

use crate::identity::Known;
use crate::program::{Program, UnitId};

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct PrintfFormat {
    pub literal: String,
    pub decimals: Vec<usize>,
}

pub(crate) fn analyze(
    program: &Program<'_>,
    unit: UnitId,
    module: &ir::Module,
    callees: &HashMap<ir::BindingId, Known>,
    literals: &HashMap<ir::BindingId, Vec<u8>>,
) -> HashMap<NodeId, PrintfFormat> {
    struct Calls<'a, 'm> {
        program: &'a Program<'m>,
        unit: UnitId,
        callees: &'a HashMap<ir::BindingId, Known>,
        literals: &'a HashMap<ir::BindingId, Vec<u8>>,
        formats: HashMap<NodeId, PrintfFormat>,
    }

    impl ir::ReadVisitor for Calls<'_, '_> {
        fn statement(&mut self, statement: &ir::Statement) {
            let ir::Statement::Expression(call) = statement else {
                return;
            };
            let ValueKind::Call {
                callee: Callee::Direct(callee),
                arguments,
                ..
            } = &call.node.value
            else {
                return;
            };
            if self.callees.get(callee) != Some(&Known::Printf) {
                return;
            }
            let Some((format, args)) = arguments.split_first() else {
                return;
            };
            let Some(plan) = literal_binding(format)
                .and_then(|id| self.literals.get(&id))
                .and_then(|literal| decimal_format(literal, args.len()))
            else {
                return;
            };
            if !args
                .iter()
                .any(|arg| self.program.reads_unaligned_place(self.unit, arg))
            {
                self.formats.insert(call.node.id, plan);
            }
        }
    }

    if !callees.values().any(|known| *known == Known::Printf) {
        return HashMap::default();
    }
    let mut calls = Calls {
        program,
        unit,
        callees,
        literals,
        formats: HashMap::default(),
    };
    ir::walk_module_ref(module, &mut calls);
    calls.formats
}

fn literal_binding(value: &Value) -> Option<ir::BindingId> {
    match &value.node.value {
        ValueKind::ArrayDecay { place, .. } => match place.kind {
            PlaceKind::Binding(id) => Some(id),
            _ => None,
        },
        ValueKind::Copy { operand, .. }
        | ValueKind::Convert {
            kind: ConversionKind::PointerCast,
            operand,
            ..
        } => literal_binding(operand),
        #[expect(
            slate::sensitive_wildcard,
            reason = "only direct string literals are formats"
        )]
        _ => None,
    }
}

fn decimal_format(bytes: &[u8], arguments: usize) -> Option<PrintfFormat> {
    if bytes.contains(&0) {
        return None;
    }
    let literal = std::str::from_utf8(bytes).ok()?;
    let mut decimals = Vec::new();
    let mut chars = literal.char_indices();
    while let Some((index, c)) = chars.next() {
        if c == '%' {
            if !matches!(chars.next(), Some((_, 'd'))) {
                return None;
            }
            decimals.push(index);
        }
    }
    (decimals.len() == arguments).then(|| PrintfFormat {
        literal: literal.to_owned(),
        decimals,
    })
}

impl Program<'_> {
    fn reads_unaligned_place(&self, unit: UnitId, value: &Value) -> bool {
        match &value.node.value {
            ValueKind::Read { place, .. } => self.place_may_be_unaligned(unit, place),
            ValueKind::Copy { operand, .. } | ValueKind::Convert { operand, .. } => {
                self.reads_unaligned_place(unit, operand)
            }
            #[expect(
                slate::sensitive_wildcard,
                reason = "other values are computed by value"
            )]
            _ => false,
        }
    }

    fn place_may_be_unaligned(&self, unit: UnitId, place: &Place) -> bool {
        place.access.align.is_some()
            || match &place.kind {
                PlaceKind::Field {
                    base,
                    index,
                    bits: None,
                } => {
                    !self.field_aligned(unit, &base.ty, *index)
                        || self.place_may_be_unaligned(unit, base)
                }
                PlaceKind::ComplexPart { base, .. } => self.place_may_be_unaligned(unit, base),
                PlaceKind::Index { base, .. } | PlaceKind::Deref(base) => {
                    self.points_into_unaligned_place(unit, base)
                }
                PlaceKind::Binding(_)
                | PlaceKind::Field { bits: Some(_), .. }
                | PlaceKind::CompoundLiteral { .. }
                | PlaceKind::Temporary { .. }
                | PlaceKind::Lane { .. }
                | PlaceKind::Swizzle { .. } => false,
            }
    }

    fn points_into_unaligned_place(&self, unit: UnitId, pointer: &Value) -> bool {
        match &pointer.node.value {
            ValueKind::ArrayDecay { place, .. } | ValueKind::AddressOf(place) => {
                self.place_may_be_unaligned(unit, place)
            }
            ValueKind::Copy { operand, .. }
            | ValueKind::Convert { operand, .. }
            | ValueKind::PointerOffset {
                pointer: operand, ..
            } => self.points_into_unaligned_place(unit, operand),
            #[expect(
                slate::sensitive_wildcard,
                reason = "only follow direct place address expressions"
            )]
            _ => false,
        }
    }
}
