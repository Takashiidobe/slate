use crate::identity::{FunctionIdentity, Known};
use crate::program::{FunctionId, Program, ProgramBinding, UnitId};
use foldhash::{HashMap, HashMapExt, HashSet, HashSetExt};
use rayon::prelude::*;
use slate_parser::ast::{NodeId, Span};
use slate_parser::ir::{
    self, BindingId, Callee, CompareOp, ConversionKind, Evaluation, NumericType, Parameters, Place,
    PlaceKind, SehHandler, Statement, StorageDuration, Type, Value, ValueKind, Visitor,
};

const STACK_RED_ZONE: usize = 512 << 10;
const STACK_GROWTH: usize = 16 << 20;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct PointerCapability {
    pub write: bool,
    pub unique: bool,
    pub free: bool,
    pub offset: bool,
    pub escape: bool,
    pub nullable: bool,
    pub string: bool,
    pub raw: bool,
    pub byte_elements: bool,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum PointerRole {
    Parameter,
    Local,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct Borrow {
    pub mutable: bool,
    pub many: bool,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct PointerFact {
    pub role: PointerRole,
    pub capability: PointerCapability,
    pub borrow: Option<Borrow>,
    pub length: Option<Length>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct Length {
    pub parameter: usize,
    pub read: bool,
}

impl PointerCapability {
    fn borrow(&self) -> Option<Borrow> {
        let shared_write = self.write && !self.unique;
        (!self.raw && !self.escape && !self.nullable && !self.free && !self.string && !shared_write)
            .then_some(Borrow {
                mutable: self.write,
                many: self.offset,
            })
    }
}

pub(crate) struct PointerFacts {
    pub bindings: HashMap<ProgramBinding, PointerFact>,
    pub functions: HashMap<ProgramBinding, Vec<BindingId>>,
}

const WRITE: u8 = 1;
const OFFSET: u8 = 1 << 1;
const FREE: u8 = 1 << 2;
const ESCAPE: u8 = 1 << 3;
const NULLABLE: u8 = 1 << 4;
const STRING: u8 = 1 << 5;
const SHARED: u8 = 1 << 6;
const MACRO: u8 = 1 << 7;

#[derive(Clone, Copy, PartialEq, Eq)]
enum NodeKind {
    Parameter(usize),
    Local,
    Temporary,
    Return,
}

struct Node {
    binding: BindingId,
    kind: NodeKind,
    byte_elements: bool,
    opaque_pointee: bool,
    flags: u8,
    raw: bool,
    reads: u32,
}

#[derive(Clone)]
enum Shape {
    Array {
        length: u64,
        element: Type,
    },
    Allocation {
        pointer: BindingId,
        count: BindingId,
        element: Type,
    },
    Binding(BindingId),
    Constant(u64),
    Other,
}

struct CallSite {
    callee: FunctionId,
    arguments: Vec<(usize, u32)>,
    objects: Vec<(usize, BindingId)>,
    unproven: Vec<usize>,
    pinned: bool,
    shapes: Vec<Shape>,
}

struct Summary {
    parameters: Vec<BindingId>,
    lengths: Vec<bool>,
    pointees: Vec<Option<Type>>,
    written: HashSet<BindingId>,
    reads: HashMap<BindingId, u32>,
    nodes: Vec<Node>,
    unions: Vec<(u32, u32)>,
    copies: Vec<(u32, u32)>,
    calls: Vec<CallSite>,
    results: Vec<(u32, FunctionId)>,
    returned: Option<u32>,
    address_taken: Vec<FunctionId>,
}

#[derive(Default)]
struct Sources {
    roots: Vec<(u32, bool)>,
    objects: Vec<BindingId>,
    null: bool,
    opaque: bool,
    retyped: bool,
}

#[derive(Clone, Copy, PartialEq, Eq)]
enum Use {
    Read,
    Write,
    Address,
}

fn tracked_pointer(ty: &Type) -> Option<bool> {
    let Type::Pointer { pointee, .. } = ty else {
        return None;
    };
    if matches!(**pointee, Type::Function { .. }) {
        return None;
    }
    Some(matches!(
        **pointee,
        Type::Numeric(NumericType::Integer { width: 8, .. })
    ))
}

struct Walker<'a, 'm> {
    program: &'a Program<'m>,
    identities: &'a HashMap<ProgramBinding, FunctionIdentity>,
    inert: &'a HashSet<ProgramBinding>,
    unit: UnitId,
    nodes: Vec<Node>,
    index: HashMap<BindingId, u32>,
    unions: Vec<(u32, u32)>,
    copies: Vec<(u32, u32)>,
    calls: Vec<CallSite>,
    results: Vec<(u32, FunctionId)>,
    result_nodes: HashMap<NodeId, u32>,
    returned: Option<u32>,
    address_taken: Vec<FunctionId>,
    automatic: HashSet<BindingId>,
    macro_depth: u32,
    written: HashSet<BindingId>,
    reads: HashMap<BindingId, u32>,
    allocations: HashMap<BindingId, (BindingId, Type)>,
}

fn peel_argument(value: &Value) -> &Value {
    match &value.node.value {
        ValueKind::Copy { operand, .. } => peel_argument(operand),
        ValueKind::Convert {
            kind: ConversionKind::PointerCast,
            operand,
            ..
        } if same_pointee(&operand.ty, &value.ty) => peel_argument(operand),
        #[expect(
            slate::sensitive_wildcard,
            reason = "only copies and same-pointee casts keep an argument's identity"
        )]
        _ => value,
    }
}

fn peel_conversions(value: &Value) -> &Value {
    match &value.node.value {
        ValueKind::Copy { operand, .. } | ValueKind::Convert { operand, .. } => {
            peel_conversions(operand)
        }
        #[expect(
            slate::sensitive_wildcard,
            reason = "every other value ends the conversion chain"
        )]
        _ => value,
    }
}

fn read_binding(value: &Value) -> Option<BindingId> {
    match &value.node.value {
        ValueKind::Read {
            place:
                Place {
                    kind: PlaceKind::Binding(binding),
                    ..
                },
            ordering: None,
        } => Some(*binding),
        #[expect(
            slate::sensitive_wildcard,
            reason = "only a plain read names a binding's current value"
        )]
        _ => None,
    }
}

fn length_type(ty: &Type) -> bool {
    matches!(
        ty,
        Type::Numeric(NumericType::Integer {
            bit_precise: false,
            ..
        })
    )
}

fn pointee(ty: &Type) -> Option<&Type> {
    match ty {
        Type::Pointer { pointee, .. } => Some(pointee),
        #[expect(slate::sensitive_wildcard, reason = "only pointers have a pointee")]
        _ => None,
    }
}

fn same_pointee(from: &Type, to: &Type) -> bool {
    matches!((from, to), (Type::Pointer { pointee: left, .. }, Type::Pointer { pointee: right, .. }) if left == right)
}

impl Walker<'_, '_> {
    fn register(&mut self, binding: BindingId, ty: &Type, kind: NodeKind) -> Option<u32> {
        let node = self.anonymous(binding, ty, kind)?;
        self.index.insert(binding, node);
        Some(node)
    }

    fn anonymous(&mut self, binding: BindingId, ty: &Type, kind: NodeKind) -> Option<u32> {
        let byte_elements = tracked_pointer(ty)?;
        let node = self.nodes.len() as u32;
        self.nodes.push(Node {
            binding,
            kind,
            byte_elements,
            opaque_pointee: matches!(ty, Type::Pointer { pointee, .. } if **pointee == Type::Void),
            flags: 0,
            raw: false,
            reads: 0,
        });
        Some(node)
    }

    fn node(&self, binding: BindingId) -> Option<u32> {
        self.index.get(&binding).copied()
    }

    fn flag(&mut self, node: u32, flags: u8) {
        self.nodes[node as usize].flags |= flags;
    }

    fn sources(&self, value: &Value) -> Sources {
        let mut out = Sources::default();
        self.collect_sources(value, false, &mut out);
        out
    }

    fn mark(&mut self, value: &Value, flags: u8) {
        for (node, _) in self.sources(value).roots {
            self.flag(node, flags);
        }
    }

    fn offset(&mut self, pointer: &Value) {
        for (node, moved) in self.sources(pointer).roots {
            if !moved {
                self.flag(node, OFFSET);
            }
        }
    }

    fn atomic(&mut self, place: &Place) {
        let mut sources = Sources::default();
        self.collect_address_sources(place, false, &mut sources);
        for (node, _) in sources.roots {
            self.flag(node, ESCAPE | WRITE);
        }
    }

    fn collect_sources(&self, value: &Value, moved: bool, out: &mut Sources) {
        stacker::maybe_grow(STACK_RED_ZONE, STACK_GROWTH, || {
            self.collect_value_sources(value, moved, out)
        })
    }

    fn collect_value_sources(&self, value: &Value, moved: bool, out: &mut Sources) {
        match &value.node.value {
            ValueKind::Read { place, .. } => self.collect_read_sources(place, moved, out),
            ValueKind::Copy { operand, .. } => self.collect_sources(operand, moved, out),
            ValueKind::Convert { kind, operand, .. } => match kind {
                ConversionKind::PointerCast | ConversionKind::AddressSpaceCast => {
                    out.retyped |= !same_pointee(&operand.ty, &value.ty);
                    self.collect_sources(operand, moved, out)
                }
                _ => out.opaque = true,
            },
            ValueKind::PointerOffset { pointer, .. } => self.collect_sources(pointer, true, out),
            ValueKind::Conditional {
                then_value,
                else_value,
                ..
            } => {
                self.collect_sources(then_value, moved, out);
                self.collect_sources(else_value, moved, out);
            }
            ValueKind::Sequence { right, .. } => self.collect_sources(right, moved, out),
            ValueKind::MacroArg { value, .. } | ValueKind::Store { value, .. } => {
                self.collect_sources(value, moved, out)
            }
            ValueKind::MacroExpansion { evaluation, .. } => {
                self.collect_sources(&evaluation.value, moved, out)
            }
            ValueKind::StatementExpression(evaluation) => {
                self.collect_sources(&evaluation.value, moved, out)
            }
            ValueKind::AddressOf(place) | ValueKind::ArrayDecay { place, .. } => {
                self.collect_address_sources(place, moved, out)
            }
            ValueKind::Null | ValueKind::Constant(_) => out.null = true,
            ValueKind::Call { .. } => match self.result_nodes.get(&value.node.id) {
                Some(&node) => out.roots.push((node, moved)),
                None => out.opaque = true,
            },
            ValueKind::LayoutQuery { .. }
            | ValueKind::LabelAddress(_)
            | ValueKind::Seh(_)
            | ValueKind::Void
            | ValueKind::CodeUnits(_)
            | ValueKind::Aggregate { .. }
            | ValueKind::FunctionDecay { .. }
            | ValueKind::Update { .. }
            | ValueKind::CompareExchange { .. }
            | ValueKind::Fence { .. }
            | ValueKind::OldValue
            | ValueKind::PointerDifference { .. }
            | ValueKind::VaArg { .. }
            | ValueKind::Block(_)
            | ValueKind::Capture { .. }
            | ValueKind::VaStart { .. }
            | ValueKind::VaEnd { .. }
            | ValueKind::VaCopy { .. }
            | ValueKind::Arith { .. }
            | ValueKind::Overflow { .. }
            | ValueKind::Unary { .. }
            | ValueKind::Compare { .. }
            | ValueKind::Logical { .. }
            | ValueKind::FloatClass { .. }
            | ValueKind::Lane { .. }
            | ValueKind::Shuffle { .. }
            | ValueKind::Intrinsic { .. } => out.opaque = true,
        }
    }

    fn collect_read_sources(&self, place: &Place, moved: bool, out: &mut Sources) {
        match &place.kind {
            PlaceKind::Binding(binding) => match self.node(*binding) {
                Some(node) => out.roots.push((node, moved)),
                None => out.opaque = true,
            },
            PlaceKind::MacroArg { place, .. } => self.collect_read_sources(place, moved, out),
            PlaceKind::Deref(_)
            | PlaceKind::CompoundLiteral { .. }
            | PlaceKind::Temporary { .. }
            | PlaceKind::ComplexPart { .. }
            | PlaceKind::Field { .. }
            | PlaceKind::Index { .. }
            | PlaceKind::Lane { .. }
            | PlaceKind::Swizzle { .. } => out.opaque = true,
        }
    }

    fn collect_address_sources(&self, place: &Place, moved: bool, out: &mut Sources) {
        match &place.kind {
            PlaceKind::Deref(pointer) => self.collect_sources(pointer, moved, out),
            PlaceKind::Index { base, .. } => self.collect_sources(base, true, out),
            PlaceKind::ComplexPart { base, .. }
            | PlaceKind::Field { base, .. }
            | PlaceKind::Lane { base, .. }
            | PlaceKind::Swizzle { base, .. } => self.collect_address_sources(base, true, out),
            PlaceKind::MacroArg { place, .. } => self.collect_address_sources(place, moved, out),
            PlaceKind::Binding(binding) => {
                out.objects.push(*binding);
                out.opaque = true;
            }
            PlaceKind::CompoundLiteral { .. } | PlaceKind::Temporary { .. } => out.opaque = true,
        }
    }

    fn assign(&mut self, destination: u32, value: &Value) {
        let sources = self.sources(value);
        for (source, moved) in sources.roots {
            if source != destination {
                self.unions.push((destination, source));
                self.copies.push((source, destination));
            }
            if moved
                || matches!(
                    self.nodes[source as usize].kind,
                    NodeKind::Temporary | NodeKind::Return
                )
            {
                self.nodes[destination as usize].raw = true;
            }
        }
        if sources.opaque || sources.retyped {
            self.nodes[destination as usize].raw = true;
        }
        if sources.null {
            self.flag(destination, NULLABLE);
        }
    }

    fn macro_mentions(&mut self, named: Named) {
        for binding in named.0 {
            if let Some(node) = self.node(binding) {
                self.flag(node, MACRO);
            }
        }
    }

    fn shape(&self, value: &Value) -> Shape {
        let peeled = peel_argument(value);
        match &peeled.node.value {
            ValueKind::ArrayDecay {
                place:
                    Place {
                        kind: PlaceKind::Binding(_),
                        ..
                    },
                length: Some(length),
            } => match pointee(&peeled.ty) {
                Some(element) => Shape::Array {
                    length: *length,
                    element: element.clone(),
                },
                None => Shape::Other,
            },
            ValueKind::Constant(ir::Number::Integer(number)) => {
                u64::try_from(number).map_or(Shape::Other, Shape::Constant)
            }
            ValueKind::Constant(ir::Number::SignedInteger(number)) => {
                u64::try_from(number).map_or(Shape::Other, Shape::Constant)
            }
            #[expect(
                slate::sensitive_wildcard,
                reason = "arrays, constants and binding reads are the only provable length sources"
            )]
            _ => match read_binding(peeled) {
                Some(binding) => match self.allocations.get(&binding) {
                    Some((count, element)) => Shape::Allocation {
                        pointer: binding,
                        count: *count,
                        element: element.clone(),
                    },
                    None => Shape::Binding(binding),
                },
                None => Shape::Other,
            },
        }
    }

    fn allocation(&self, ty: &Type, initializer: &Value) -> Option<(BindingId, Type)> {
        let element = pointee(ty)?;
        let mut call = initializer;
        while let ValueKind::Copy { operand, .. }
        | ValueKind::Convert {
            kind: ConversionKind::PointerCast,
            operand,
            ..
        } = &call.node.value
        {
            call = operand;
        }
        let ValueKind::Call {
            callee: Callee::Direct(function),
            arguments,
            ..
        } = &call.node.value
        else {
            return None;
        };
        if !matches!(
            self.identities.get(&ProgramBinding {
                unit: self.unit,
                binding: *function,
            }),
            Some(FunctionIdentity::Known(Known::Malloc))
        ) {
            return None;
        }
        let [size] = arguments.as_slice() else {
            return None;
        };
        let ValueKind::Arith {
            op: ir::ArithOp::Mul,
            left,
            right,
            ..
        } = &peel_conversions(size).node.value
        else {
            return None;
        };
        let size_of = |value: &Value| {
            matches!(
                &peel_conversions(value).node.value,
                ValueKind::LayoutQuery {
                    query: ir::LayoutQuery::SizeOf { ty, .. },
                    ..
                } if ty == element
            )
        };
        let count = if size_of(right) {
            left
        } else if size_of(left) {
            right
        } else {
            return None;
        };
        Some((read_binding(peel_conversions(count))?, element.clone()))
    }

    fn proven(&self, value: &Value) -> bool {
        match &value.node.value {
            ValueKind::Read {
                place:
                    Place {
                        kind: PlaceKind::Binding(binding),
                        ..
                    },
                ordering: None,
            } => self.node(*binding).is_some(),
            ValueKind::Copy { operand, .. } => self.proven(operand),
            ValueKind::Convert {
                kind: ConversionKind::PointerCast,
                operand,
                ..
            } => same_pointee(&operand.ty, &value.ty) && self.proven(operand),
            ValueKind::Conditional {
                then_value,
                else_value,
                ..
            } => self.proven(then_value) && self.proven(else_value),
            ValueKind::AddressOf(Place {
                kind: PlaceKind::Binding(binding),
                ..
            })
            | ValueKind::ArrayDecay {
                place:
                    Place {
                        kind: PlaceKind::Binding(binding),
                        ..
                    },
                length: Some(_),
            } => self.automatic.contains(binding),
            #[expect(
                slate::sensitive_wildcard,
                reason = "only reads of tracked bindings and addresses of automatic objects lower to a borrow"
            )]
            _ => false,
        }
    }

    fn store(&mut self, place: &Place, value: &Value) {
        match &place.kind {
            PlaceKind::Binding(binding) => match self.node(*binding) {
                Some(node) => self.assign(node, value),
                None => self.mark(value, ESCAPE),
            },
            PlaceKind::MacroArg { place, .. } => self.store(place, value),
            PlaceKind::Deref(_)
            | PlaceKind::CompoundLiteral { .. }
            | PlaceKind::Temporary { .. }
            | PlaceKind::ComplexPart { .. }
            | PlaceKind::Field { .. }
            | PlaceKind::Index { .. }
            | PlaceKind::Lane { .. }
            | PlaceKind::Swizzle { .. } => self.mark(value, ESCAPE),
        }
    }

    fn rebase(&mut self, place: &Place) {
        match &place.kind {
            PlaceKind::Binding(binding) => {
                if let Some(node) = self.node(*binding) {
                    self.nodes[node as usize].raw = true;
                    self.flag(node, OFFSET);
                }
            }
            PlaceKind::MacroArg { place, .. } => self.rebase(place),
            PlaceKind::Deref(_)
            | PlaceKind::CompoundLiteral { .. }
            | PlaceKind::Temporary { .. }
            | PlaceKind::ComplexPart { .. }
            | PlaceKind::Field { .. }
            | PlaceKind::Index { .. }
            | PlaceKind::Lane { .. }
            | PlaceKind::Swizzle { .. } => {}
        }
    }

    fn place(&mut self, place: &Place, access: Use) {
        match &place.kind {
            PlaceKind::Binding(binding) => {
                if access == Use::Read {
                    *self.reads.entry(*binding).or_default() += 1;
                } else {
                    self.written.insert(*binding);
                }
                if let Some(node) = self.node(*binding) {
                    match access {
                        Use::Read => self.nodes[node as usize].reads += 1,
                        Use::Address => self.flag(node, ESCAPE | WRITE),
                        Use::Write => {}
                    }
                }
            }
            PlaceKind::Deref(pointer) => {
                self.value(pointer);
                if access == Use::Write {
                    self.mark(pointer, WRITE);
                }
            }
            PlaceKind::CompoundLiteral { initializer, .. }
            | PlaceKind::Temporary { initializer, .. } => {
                self.value(initializer);
                self.mark(initializer, ESCAPE);
            }
            PlaceKind::ComplexPart { base, .. }
            | PlaceKind::Field { base, .. }
            | PlaceKind::Swizzle { base, .. } => self.place(base, access),
            PlaceKind::Lane { base, index } => {
                self.place(base, access);
                self.value(index);
            }
            PlaceKind::Index { base, index } => {
                self.value(base);
                self.value(index);
                self.offset(base);
                if access == Use::Write {
                    self.mark(base, WRITE);
                }
            }
            PlaceKind::MacroArg { place, .. } => {
                self.place(place, access);
                let mut named = Named(Vec::new());
                place_bindings(place, &mut named);
                self.macro_mentions(named);
            }
        }
    }

    fn evaluation(&mut self, evaluation: &Evaluation) {
        self.statements(&evaluation.statements);
        self.value(&evaluation.value);
    }

    fn value(&mut self, value: &Value) {
        stacker::maybe_grow(STACK_RED_ZONE, STACK_GROWTH, || self.value_kind(value))
    }

    fn value_kind(&mut self, value: &Value) {
        match &value.node.value {
            ValueKind::Constant(_)
            | ValueKind::LayoutQuery { .. }
            | ValueKind::Null
            | ValueKind::LabelAddress(_)
            | ValueKind::Seh(_)
            | ValueKind::Void
            | ValueKind::CodeUnits(_)
            | ValueKind::Fence { .. }
            | ValueKind::OldValue => {}
            ValueKind::Copy { operand, .. } => self.value(operand),
            ValueKind::Aggregate { members, .. } => {
                for member in members {
                    self.value(&member.value);
                    self.mark(&member.value, ESCAPE);
                }
            }
            ValueKind::ArrayDecay { place, .. } | ValueKind::AddressOf(place) => {
                self.place(place, Use::Address)
            }
            ValueKind::FunctionDecay { place } => {
                if let PlaceKind::Binding(function) = place.kind
                    && let Some(id) = self.program.callee(self.unit, &Callee::Direct(function))
                {
                    self.address_taken.push(id);
                }
            }
            ValueKind::Store {
                place,
                value,
                ordering,
            } => {
                self.value(value);
                self.place(place, Use::Write);
                if ordering.is_some() || place.access.atomic {
                    self.atomic(place);
                }
                self.store(place, value);
            }
            ValueKind::Update {
                place,
                computation,
                ordering,
                ..
            } => {
                self.value(computation);
                self.place(place, Use::Write);
                if ordering.is_some() || place.access.atomic {
                    self.atomic(place);
                }
                self.rebase(place);
            }
            ValueKind::CompareExchange {
                place,
                expected,
                desired,
                ..
            } => {
                self.value(expected);
                self.value(desired);
                self.place(place, Use::Write);
                self.atomic(place);
                self.mark(expected, ESCAPE);
                self.mark(desired, ESCAPE);
            }
            ValueKind::PointerOffset {
                pointer, amount, ..
            } => {
                self.value(pointer);
                self.value(amount);
                self.offset(pointer);
            }
            ValueKind::PointerDifference { left, right, .. } => {
                self.value(left);
                self.value(right);
                self.offset(left);
                self.offset(right);
            }
            ValueKind::Conditional {
                condition,
                then_value,
                else_value,
            } => {
                self.value(condition);
                self.value(then_value);
                self.value(else_value);
            }
            ValueKind::Sequence { left, right } => {
                self.value(left);
                self.value(right);
            }
            ValueKind::Call {
                callee, arguments, ..
            } => self.call(value, callee, arguments),
            ValueKind::Read { place, ordering } => {
                self.place(place, Use::Read);
                if ordering.is_some() || place.access.atomic {
                    self.atomic(place);
                }
            }
            ValueKind::VaArg { list, .. }
            | ValueKind::VaStart { list, .. }
            | ValueKind::VaEnd { list, .. } => self.place(list, Use::Write),
            ValueKind::VaCopy {
                destination,
                source,
                ..
            } => {
                self.place(destination, Use::Write);
                self.place(source, Use::Read);
            }
            ValueKind::StatementExpression(evaluation) => self.evaluation(evaluation),
            ValueKind::MacroExpansion { evaluation, .. } => {
                self.macro_depth += 1;
                self.evaluation(evaluation);
                self.macro_depth -= 1;
            }
            ValueKind::MacroArg { value, .. } => {
                self.value(value);
                let mut named = Named(Vec::new());
                ir::walk_value(&mut (**value).clone(), &mut named);
                self.macro_mentions(named);
            }
            ValueKind::Block(literal) => {
                for capture in &literal.captures {
                    if let Some(node) = self.node(capture.id) {
                        self.flag(node, ESCAPE | WRITE);
                    }
                }
            }
            ValueKind::Capture { extent, value, .. } => {
                self.value(extent);
                self.value(value);
            }
            ValueKind::Convert { operand, .. } => {
                self.value(operand);
                if tracked_pointer(&operand.ty).is_some() && tracked_pointer(&value.ty).is_none() {
                    let flags = if value.ty == Type::Bool {
                        NULLABLE
                    } else {
                        ESCAPE
                    };
                    self.mark(operand, flags);
                }
            }
            ValueKind::Arith { left, right, .. } | ValueKind::Logical { left, right, .. } => {
                self.value(left);
                self.value(right);
            }
            ValueKind::Overflow {
                left,
                right,
                result,
                ..
            } => {
                self.value(left);
                self.value(right);
                self.place(result, Use::Write);
            }
            ValueKind::Unary { operand, .. } | ValueKind::FloatClass { operand, .. } => {
                self.value(operand)
            }
            ValueKind::Compare {
                op, left, right, ..
            } => {
                self.value(left);
                self.value(right);
                if matches!(op, CompareOp::Eq | CompareOp::Ne) {
                    if null_like(right) {
                        self.mark(left, NULLABLE);
                    }
                    if null_like(left) {
                        self.mark(right, NULLABLE);
                    }
                }
            }
            ValueKind::Lane { vector, index } => {
                self.value(vector);
                self.value(index);
            }
            ValueKind::Shuffle { left, right, .. } => {
                self.value(left);
                if let Some(right) = right {
                    self.value(right);
                }
            }
            ValueKind::Intrinsic { arguments, .. } => {
                for argument in arguments {
                    self.value(argument);
                    self.mark(argument, ESCAPE);
                }
            }
        }
    }

    fn call(&mut self, call: &Value, callee: &Callee, arguments: &[Value]) {
        if let Callee::Indirect(function) = callee {
            self.value(function);
        }
        for argument in arguments {
            self.value(argument);
        }
        if let Some(target) = self.program.callee(self.unit, callee) {
            if let Some(node) = self.anonymous(BindingId(u32::MAX), &call.ty, NodeKind::Temporary) {
                self.result_nodes.insert(call.node.id, node);
                self.results.push((node, target));
            }
            let mut site = CallSite {
                callee: target,
                arguments: Vec::new(),
                objects: Vec::new(),
                unproven: Vec::new(),
                pinned: self.macro_depth > 0,
                shapes: arguments
                    .iter()
                    .map(|argument| self.shape(argument))
                    .collect(),
            };
            for (index, argument) in arguments.iter().enumerate() {
                if tracked_pointer(&argument.ty).is_some() && !self.proven(argument) {
                    site.unproven.push(index);
                }
                let sources = self.sources(argument);
                for (node, _) in sources.roots {
                    site.arguments.push((index, node));
                }
                for object in sources.objects {
                    site.objects.push((index, object));
                }
            }
            self.calls.push(site);
            return;
        }
        if let Callee::Direct(binding) = callee
            && self.inert.contains(&ProgramBinding {
                unit: self.unit,
                binding: *binding,
            })
        {
            return;
        }
        let known = match callee {
            Callee::Direct(binding) => match self.identities.get(&ProgramBinding {
                unit: self.unit,
                binding: *binding,
            }) {
                Some(FunctionIdentity::Known(known)) => Some(*known),
                Some(FunctionIdentity::HeaderInline | FunctionIdentity::Unknown) | None => None,
            },
            Callee::Indirect(_) => None,
        };
        for (index, argument) in arguments.iter().enumerate() {
            let flags = known.map_or(ESCAPE, |known| known_argument(known, index));
            if flags != 0 {
                self.mark(argument, flags);
            }
        }
    }

    fn statements(&mut self, body: &[Span<Statement>]) {
        for statement in body {
            self.statement(statement);
        }
    }

    fn escape_all(&mut self, statement: &Span<Statement>) {
        let mut escaper = Escaper {
            index: &self.index,
            hits: Vec::new(),
        };
        ir::walk_body(&mut vec![statement.clone()], &mut escaper);
        for node in escaper.hits {
            self.flag(node, ESCAPE | WRITE);
        }
        let mut named = Named(Vec::new());
        ir::walk_body(&mut vec![statement.clone()], &mut named);
        self.written.extend(named.0);
    }

    fn statement(&mut self, statement: &Span<Statement>) {
        match &statement.value {
            Statement::Comment(_)
            | Statement::Null
            | Statement::Fence { .. }
            | Statement::Return(None)
            | Statement::Break(_)
            | Statement::Continue(_)
            | Statement::Goto(_)
            | Statement::SehLeave(_) => {}
            Statement::Temporary {
                id,
                ty,
                initializer,
                ..
            } => {
                let node = self.register(*id, ty, NodeKind::Temporary);
                if let Some(initializer) = initializer {
                    self.value(initializer);
                    match node {
                        Some(node) => self.assign(node, initializer),
                        None => self.mark(initializer, ESCAPE),
                    }
                }
            }
            Statement::Let(variable) => {
                let node = if matches!(variable.storage, StorageDuration::Automatic)
                    && !variable.name.is_empty()
                {
                    self.register(variable.id, &variable.ty, NodeKind::Local)
                } else {
                    None
                };
                if let Some(node) = node
                    && (variable.cleanup.is_some() || variable.byref)
                {
                    self.flag(node, ESCAPE | WRITE);
                }
                if let Some(handler) = variable.cleanup
                    && let Some(id) = self.program.callee(self.unit, &Callee::Direct(handler))
                {
                    self.address_taken.push(id);
                }
                if let Some(node) = node
                    && variable.initializer.is_none()
                {
                    self.nodes[node as usize].raw = true;
                }
                if matches!(variable.storage, StorageDuration::Automatic)
                    && !variable.byref
                    && variable.register.is_none()
                {
                    self.automatic.insert(variable.id);
                }
                if let Some(initializer) = &variable.initializer {
                    self.value(initializer);
                    if matches!(variable.storage, StorageDuration::Automatic)
                        && let Some(allocation) = self.allocation(&variable.ty, initializer)
                    {
                        self.allocations.insert(variable.id, allocation);
                    }
                    match node {
                        Some(node) => self.assign(node, initializer),
                        None => self.mark(initializer, ESCAPE),
                    }
                }
            }
            Statement::Write { place, value, .. } => {
                self.value(value);
                self.place(place, Use::Write);
                self.store(place, value);
            }
            Statement::Expression(value) | Statement::ComputedGoto(value) => self.value(value),
            Statement::Return(Some(value)) => {
                self.value(value);
                match self.returned {
                    Some(node) => self.assign(node, value),
                    None => self.mark(value, ESCAPE),
                }
            }
            Statement::Block(body)
            | Statement::Default { body, .. }
            | Statement::Label { body, .. } => self.statements(body),
            Statement::MacroExpansion { body, .. } => {
                self.macro_depth += 1;
                self.statements(body);
                self.macro_depth -= 1;
            }
            Statement::If {
                condition,
                then_body,
                else_body,
                ..
            } => {
                self.value(condition);
                self.statements(then_body);
                if let Some(else_body) = else_body {
                    self.statements(else_body);
                }
            }
            Statement::While {
                condition, body, ..
            }
            | Statement::DoWhile {
                condition, body, ..
            } => {
                self.evaluation(condition);
                self.statements(body);
            }
            Statement::For {
                init,
                condition,
                increment,
                body,
                counted,
                ..
            } => {
                self.statements(init);
                if let Some(condition) = condition {
                    self.evaluation(condition);
                }
                if let Some(increment) = increment {
                    self.evaluation(increment);
                }
                self.statements(body);
                if let Some(counted) = counted {
                    self.value(&counted.start);
                    self.value(&counted.end);
                }
            }
            Statement::Switch {
                discriminant, body, ..
            } => {
                self.value(discriminant);
                self.statements(body);
            }
            Statement::Case {
                start, end, body, ..
            } => {
                self.value(start);
                if let Some(end) = end {
                    self.value(end);
                }
                self.statements(body);
            }
            Statement::SehTry { body, handler, .. } => {
                self.statements(body);
                match handler {
                    SehHandler::Except { filter, body } => {
                        self.evaluation(filter);
                        self.statements(body);
                    }
                    SehHandler::Finally(body) => self.statements(body),
                }
            }
            Statement::Asm(_)
            | Statement::OmpParallel(_)
            | Statement::OmpFor(_)
            | Statement::NestedFunction(_) => self.escape_all(statement),
        }
    }
}

struct Escaper<'a> {
    index: &'a HashMap<BindingId, u32>,
    hits: Vec<u32>,
}

impl Visitor for Escaper<'_> {
    fn binding(&mut self, binding: BindingId) {
        self.hits.extend(self.index.get(&binding).copied());
    }
}

struct Named(Vec<BindingId>);

fn place_bindings(place: &Place, named: &mut Named) {
    match &place.kind {
        PlaceKind::Binding(binding) => named.0.push(*binding),
        PlaceKind::Deref(value) => ir::walk_value(&mut value.clone(), named),
        PlaceKind::Index { base, index } => {
            ir::walk_value(&mut base.clone(), named);
            ir::walk_value(&mut index.clone(), named);
        }
        PlaceKind::Lane { base, index } => {
            place_bindings(base, named);
            ir::walk_value(&mut index.clone(), named);
        }
        PlaceKind::Field { base, .. }
        | PlaceKind::ComplexPart { base, .. }
        | PlaceKind::Swizzle { base, .. } => place_bindings(base, named),
        PlaceKind::MacroArg { place, .. } => place_bindings(place, named),
        PlaceKind::CompoundLiteral { initializer, .. }
        | PlaceKind::Temporary { initializer, .. } => {
            ir::walk_value(&mut initializer.clone(), named)
        }
    }
}

impl Visitor for Named {
    fn binding(&mut self, binding: BindingId) {
        self.0.push(binding);
    }
}

fn null_like(value: &Value) -> bool {
    match &value.node.value {
        ValueKind::Null => true,
        ValueKind::Constant(_) => matches!(value.ty, Type::Pointer { .. }),
        ValueKind::Copy { operand, .. } | ValueKind::Convert { operand, .. } => null_like(operand),
        #[expect(
            slate::sensitive_wildcard,
            reason = "only null and pointer-typed constants, through copies and conversions, are null pointer constants"
        )]
        _ => false,
    }
}

fn known_argument(known: Known, index: usize) -> u8 {
    match known {
        Known::Free => FREE,
        Known::MemCpy | Known::MemMove | Known::MemSet => {
            if index == 0 {
                WRITE
            } else {
                0
            }
        }
        Known::StrCpy | Known::StrCat | Known::StrNCpy | Known::StrNCat => {
            if index == 0 {
                STRING | WRITE
            } else {
                STRING
            }
        }
        Known::StrLen
        | Known::StrCmp
        | Known::StrNCmp
        | Known::StrNLen
        | Known::StrChr
        | Known::StrRChr
        | Known::StrStr
        | Known::StrPBrk
        | Known::StrSpn
        | Known::StrCSpn => STRING | OFFSET,
        Known::MemChr | Known::MemCmp => 0,
        _ => ESCAPE,
    }
}

fn summarize(
    program: &Program<'_>,
    identities: &HashMap<ProgramBinding, FunctionIdentity>,
    inert: &HashSet<ProgramBinding>,
    function: FunctionId,
) -> Option<Summary> {
    let entry = program.function(function)?;
    let mut walker = Walker {
        program,
        identities,
        inert,
        unit: entry.unit,
        nodes: Vec::new(),
        index: HashMap::new(),
        unions: Vec::new(),
        copies: Vec::new(),
        calls: Vec::new(),
        results: Vec::new(),
        result_nodes: HashMap::new(),
        returned: None,
        address_taken: Vec::new(),
        automatic: HashSet::new(),
        macro_depth: 0,
        written: HashSet::new(),
        reads: HashMap::new(),
        allocations: HashMap::new(),
    };
    if let Some(return_type) = &entry.function.return_type {
        walker.returned = walker.anonymous(entry.function.value.id, return_type, NodeKind::Return);
    }
    let mut parameters = Vec::new();
    let mut lengths = Vec::new();
    let mut pointees = Vec::new();
    if let Parameters::Prototype { fixed, .. } = &entry.function.parameters {
        for (index, parameter) in fixed.iter().enumerate() {
            parameters.push(parameter.value.id);
            lengths.push(length_type(&parameter.value.ty));
            pointees.push(pointee(&parameter.value.ty).cloned());
            walker.automatic.insert(parameter.value.id);
            walker.register(
                parameter.value.id,
                &parameter.value.ty,
                NodeKind::Parameter(index),
            );
        }
    }
    if let Some(body) = &entry.function.body {
        walker.statements(body);
    }
    for &(source, destination) in &walker.copies {
        if walker.nodes[destination as usize].kind == NodeKind::Local
            && walker.nodes[source as usize].reads > 1
        {
            walker.nodes[source as usize].flags |= SHARED;
        }
    }
    Some(Summary {
        parameters,
        lengths,
        pointees,
        written: walker.written,
        reads: walker.reads,
        nodes: walker.nodes,
        unions: walker.unions,
        copies: walker.copies,
        calls: walker.calls,
        results: walker.results,
        returned: walker.returned,
        address_taken: walker.address_taken,
    })
}

struct UnionFind {
    parent: Vec<u32>,
    size: Vec<u32>,
}

impl UnionFind {
    fn new(count: usize) -> Self {
        Self {
            parent: (0..count as u32).collect(),
            size: vec![1; count],
        }
    }

    fn find(&mut self, mut node: u32) -> u32 {
        while self.parent[node as usize] != node {
            let grandparent = self.parent[self.parent[node as usize] as usize];
            self.parent[node as usize] = grandparent;
            node = grandparent;
        }
        node
    }

    fn union(&mut self, left: u32, right: u32) {
        let (mut left, mut right) = (self.find(left), self.find(right));
        if left == right {
            return;
        }
        if self.size[left as usize] < self.size[right as usize] {
            std::mem::swap(&mut left, &mut right);
        }
        self.parent[right as usize] = left;
        self.size[left as usize] += self.size[right as usize];
    }
}

fn reach(count: usize, edges: &mut [(u32, u32)], seeds: &mut [bool]) {
    edges.sort_unstable();
    let mut start = vec![0usize; count + 1];
    for &(from, _) in edges.iter() {
        start[from as usize + 1] += 1;
    }
    for index in 0..count {
        start[index + 1] += start[index];
    }
    let mut stack: Vec<u32> = (0..count as u32).filter(|&n| seeds[n as usize]).collect();
    while let Some(node) = stack.pop() {
        for &(_, to) in &edges[start[node as usize]..start[node as usize + 1]] {
            if !seeds[to as usize] {
                seeds[to as usize] = true;
                stack.push(to);
            }
        }
    }
}

fn propagate(flags: &mut [u8], flag: u8, edges: &mut [(u32, u32)]) {
    let mut seeds: Vec<bool> = flags.iter().map(|flags| flags & flag != 0).collect();
    reach(flags.len(), edges, &mut seeds);
    for (flags, seeded) in flags.iter_mut().zip(seeds) {
        if seeded {
            *flags |= flag;
        }
    }
}

fn capability(flags: u8, raw: bool, byte_elements: bool) -> PointerCapability {
    PointerCapability {
        write: flags & WRITE != 0,
        unique: flags & SHARED == 0,
        free: flags & FREE != 0,
        offset: flags & OFFSET != 0,
        escape: flags & ESCAPE != 0,
        nullable: flags & NULLABLE != 0,
        string: flags & STRING != 0 && byte_elements,
        raw,
        byte_elements,
    }
}

fn candidate(function: &ir::Function, exported: bool, address_taken: bool) -> bool {
    !address_taken
        && !exported
        && function.name != "main"
        && matches!(
            function.parameters,
            Parameters::Prototype {
                variadic: false,
                ..
            }
        )
        && !function.semantics.naked
        && !function.semantics.interrupt
}

pub(crate) fn analyze(
    program: &Program<'_>,
    identities: &HashMap<ProgramBinding, FunctionIdentity>,
) -> PointerFacts {
    let timing = std::env::var_os("SLATE_INTERPROC_TIMING")
        .is_some()
        .then(std::time::Instant::now);
    let count = program.functions().len();
    let mut inert = HashSet::new();
    for (unit, entry) in program.units() {
        for function in &entry.module.functions {
            if function.value.semantics.memory == Some(ir::MemoryEffects::None) {
                inert.insert(ProgramBinding {
                    unit,
                    binding: function.value.id,
                });
            }
        }
    }
    let summaries: Vec<Option<Summary>> = (0..count as u32)
        .into_par_iter()
        .map(|index| summarize(program, identities, &inert, FunctionId(index)))
        .collect();
    let summarized = timing.map(|start| (start.elapsed(), std::time::Instant::now()));

    let mut base = Vec::with_capacity(count + 1);
    base.push(0u32);
    for summary in &summaries {
        let nodes = summary.as_ref().map_or(0, |summary| summary.nodes.len());
        base.push(base[base.len() - 1] + nodes as u32);
    }
    let total = base[count] as usize;

    let mut address_taken = vec![false; count];
    for summary in summaries.iter().flatten() {
        for function in &summary.address_taken {
            address_taken[function.0 as usize] = true;
        }
    }
    for (unit, entry) in program.units() {
        let mut named = Named(Vec::new());
        for global in &entry.module.globals {
            if let Some(initializer) = &global.variable.initializer {
                ir::walk_value(&mut initializer.clone(), &mut named);
            }
        }
        for binding in named.0 {
            if let Some(function) = program.callee(unit, &Callee::Direct(binding)) {
                address_taken[function.0 as usize] = true;
            }
        }
    }
    let candidates: Vec<bool> = program
        .functions()
        .map(|(id, entry)| {
            candidate(
                &entry.function.value,
                program.exported(id),
                address_taken[id.0 as usize],
            )
        })
        .collect();
    let parameters: Vec<HashMap<usize, u32>> = summaries
        .iter()
        .zip(&base)
        .map(|(summary, &offset)| {
            let mut parameters = HashMap::new();
            for (node, entry) in summary
                .iter()
                .flat_map(|summary| summary.nodes.iter().enumerate())
            {
                if let NodeKind::Parameter(index) = entry.kind {
                    parameters.insert(index, offset + node as u32);
                }
            }
            parameters
        })
        .collect();

    let mut classes = UnionFind::new(total);
    let mut raw = vec![false; total];
    let mut seeds = vec![0u8; total];
    let mut copies = Vec::new();
    for (function, summary) in summaries.iter().enumerate() {
        let Some(summary) = summary else { continue };
        let offset = base[function];
        for &(left, right) in &summary.unions {
            classes.union(offset + left, offset + right);
        }
        for &(source, destination) in &summary.copies {
            copies.push((offset + source, offset + destination));
        }
        for (node, entry) in summary.nodes.iter().enumerate() {
            let global = (offset + node as u32) as usize;
            seeds[global] = entry.flags;
            raw[global] = entry.raw
                || (matches!(entry.kind, NodeKind::Parameter(_)) && !candidates[function]);
            if entry.kind == NodeKind::Return && !candidates[function] {
                seeds[global] |= ESCAPE;
            }
        }
    }
    let mut flows = Vec::new();
    let mut arguments: Vec<(u32, u32, usize, usize)> = Vec::new();
    let mut returns: Vec<(u32, u32)> = Vec::new();
    for (function, summary) in summaries.iter().enumerate() {
        let Some(summary) = summary else { continue };
        for &(node, callee) in &summary.results {
            let result = base[function] + node;
            let returned = summaries
                .get(callee.0 as usize)
                .and_then(Option::as_ref)
                .and_then(|callee| callee.returned);
            match returned {
                Some(returned) => {
                    let returned = base[callee.0 as usize] + returned;
                    flows.push((returned, result));
                    returns.push((returned, result));
                    copies.push((returned, result));
                }
                None => raw[result as usize] = true,
            }
        }
    }
    let mut class_flags = vec![0u8; total];
    for node in 0..total as u32 {
        let class = classes.find(node);
        class_flags[class as usize] |= seeds[node as usize];
    }

    let mut aliased = Vec::new();
    for (function, summary) in summaries.iter().enumerate() {
        let Some(summary) = summary else { continue };
        let offset = base[function];
        for site in &summary.calls {
            let callee = site.callee.0 as usize;
            let mut objects = Vec::new();
            for &(index, node) in &site.arguments {
                let caller = classes.find(offset + node);
                match parameters[callee].get(&index) {
                    Some(&parameter) if candidates[callee] => {
                        flows.push((caller, parameter));
                        arguments.push((caller, parameter, callee, index));
                        objects.push(((false, caller), index));
                    }
                    Some(_) | None => class_flags[caller as usize] |= ESCAPE,
                }
            }
            if !candidates[callee] {
                continue;
            }
            for &(index, object) in &site.objects {
                objects.push(((true, object.0), index));
            }
            objects.sort_unstable();
            objects.dedup();
            for pair in objects.windows(2) {
                if pair[0].0 == pair[1].0
                    && let Some(&left) = parameters[callee].get(&pair[0].1)
                    && let Some(&right) = parameters[callee].get(&pair[1].1)
                {
                    aliased.push((classes.find(left), classes.find(right)));
                }
            }
        }
    }

    let mut forward: Vec<(u32, u32)> = flows
        .iter()
        .map(|&(from, to)| (classes.find(from), classes.find(to)))
        .filter(|(from, to)| from != to)
        .collect();
    let mut backward: Vec<(u32, u32)> = forward.iter().map(|&(from, to)| (to, from)).collect();
    for &(left, right) in &aliased {
        backward.push((left, right));
        backward.push((right, left));
    }
    for flag in [WRITE, OFFSET, FREE, STRING, SHARED] {
        propagate(&mut class_flags, flag, &mut backward);
    }
    for &(left, right) in &aliased {
        if (class_flags[left as usize] | class_flags[right as usize]) & WRITE != 0 {
            class_flags[left as usize] |= ESCAPE;
            class_flags[right as usize] |= ESCAPE;
        }
    }
    propagate(&mut class_flags, ESCAPE, &mut backward);
    propagate(&mut class_flags, NULLABLE, &mut forward);
    reach(total, &mut copies, &mut raw);

    let class_of: Vec<u32> = (0..total as u32).map(|node| classes.find(node)).collect();
    let units: Vec<Option<UnitId>> = (0..count as u32)
        .map(|function| {
            program
                .function(FunctionId(function))
                .map(|entry| entry.unit)
        })
        .collect();
    let class_borrow = |flags: u8| capability(flags, false, false).borrow();
    let seed_unproven = |covered: &HashSet<(usize, usize)>| {
        let mut unproven = vec![false; total];
        for &class in &class_of {
            let class = class as usize;
            if class_flags[class] & MACRO != 0 || class_borrow(class_flags[class]).is_none() {
                unproven[class] = true;
            }
        }
        for (function, summary) in summaries.iter().enumerate() {
            let Some(summary) = summary else { continue };
            let offset = base[function];
            for site in &summary.calls {
                let callee = site.callee.0 as usize;
                if !candidates[callee] {
                    continue;
                }
                let foreign = site.pinned || units[callee] != units[function];
                for (index, &parameter) in &parameters[callee] {
                    if foreign
                        || (site.unproven.contains(index) && !covered.contains(&(callee, *index)))
                    {
                        unproven[class_of[parameter as usize] as usize] = true;
                    }
                }
                for &(index, node) in &site.arguments {
                    if covered.contains(&(callee, index)) {
                        continue;
                    }
                    let global = offset + node;
                    let item = &summary.nodes[node as usize];
                    let class = class_of[global as usize] as usize;
                    let lowered_raw = raw[global as usize]
                        || item.opaque_pointee
                        || matches!(item.kind, NodeKind::Temporary | NodeKind::Return)
                        || (item.kind == NodeKind::Local
                            && class_borrow(class_flags[class])
                                .is_none_or(|borrow| borrow.mutable));
                    if lowered_raw && let Some(&parameter) = parameters[callee].get(&index) {
                        unproven[class_of[parameter as usize] as usize] = true;
                    }
                }
            }
        }
        let mut edges: Vec<(u32, u32)> = returns
            .iter()
            .copied()
            .chain(
                arguments
                    .iter()
                    .filter(|&&(_, _, callee, index)| !covered.contains(&(callee, index)))
                    .map(|&(from, to, _, _)| (from, to)),
            )
            .map(|(from, to)| (class_of[from as usize], class_of[to as usize]))
            .filter(|(from, to)| from != to)
            .collect();
        reach(total, &mut edges, &mut unproven);
        unproven
    };

    let mut incoming: Vec<Vec<(usize, usize)>> = vec![Vec::new(); count];
    for (function, summary) in summaries.iter().enumerate() {
        let Some(summary) = summary else { continue };
        for (index, site) in summary.calls.iter().enumerate() {
            incoming[site.callee.0 as usize].push((function, index));
        }
    }
    let mut active: HashSet<(usize, usize, usize)> = HashSet::new();
    for (function, summary) in summaries.iter().enumerate() {
        let Some(summary) = summary else { continue };
        if !candidates[function] || incoming[function].is_empty() {
            continue;
        }
        let unwritten = |index: usize| !summary.written.contains(&summary.parameters[index]);
        for pointer in 0..summary.parameters.len() {
            if !parameters[function].contains_key(&pointer) || !unwritten(pointer) {
                continue;
            }
            for (length, &integer) in summary.lengths.iter().enumerate() {
                if integer && unwritten(length) {
                    active.insert((function, pointer, length));
                }
            }
        }
    }
    let proves = |active: &HashSet<(usize, usize, usize)>,
                  (function, pointer, length): (usize, usize, usize)| {
        let Some(element) = summaries[function]
            .as_ref()
            .and_then(|summary| summary.pointees[pointer].as_ref())
        else {
            return false;
        };
        incoming[function].iter().all(|&(caller, index)| {
            let Some(summary) = &summaries[caller] else {
                return false;
            };
            let site = &summary.calls[index];
            if site.pinned || units[caller] != units[function] {
                return false;
            }
            let caller_parameter = |binding: &BindingId| {
                (!summary.written.contains(binding))
                    .then(|| summary.parameters.iter().position(|id| id == binding))
                    .flatten()
            };
            match (site.shapes.get(pointer), site.shapes.get(length)) {
                (
                    Some(Shape::Array {
                        length: extent,
                        element: source,
                    }),
                    Some(Shape::Constant(count)),
                ) => extent == count && source == element,
                (
                    Some(Shape::Allocation {
                        pointer: binding,
                        count,
                        element: source,
                    }),
                    Some(Shape::Binding(passed)),
                ) => {
                    count == passed
                        && source == element
                        && !summary.written.contains(binding)
                        && !summary.written.contains(count)
                }
                (Some(Shape::Binding(source)), Some(Shape::Binding(passed))) => {
                    match (caller_parameter(source), caller_parameter(passed)) {
                        (Some(source), Some(passed)) => active.contains(&(caller, source, passed)),
                        _ => false,
                    }
                }
                _ => false,
            }
        })
    };
    let mut unproven;
    loop {
        let mut next: HashSet<(usize, usize, usize)> = active
            .iter()
            .copied()
            .filter(|&pair| proves(&active, pair))
            .collect();
        let mut uses: HashMap<(usize, bool, usize), usize> = HashMap::new();
        for &(function, pointer, length) in &next {
            *uses.entry((function, false, pointer)).or_default() += 1;
            *uses.entry((function, true, length)).or_default() += 1;
        }
        next.retain(|&(function, pointer, length)| {
            uses[&(function, false, pointer)] == 1 && uses[&(function, true, length)] == 1
        });
        let covered: HashSet<(usize, usize)> = next
            .iter()
            .map(|&(function, pointer, _)| (function, pointer))
            .collect();
        unproven = seed_unproven(&covered);
        next.retain(|&(function, pointer, _)| {
            let node = parameters[function][&pointer] as usize;
            let class = class_of[node] as usize;
            let item = summaries[function]
                .as_ref()
                .map(|summary| &summary.nodes[node - base[function] as usize]);
            !unproven[class]
                && item.is_some_and(|item| {
                    !item.opaque_pointee
                        && capability(class_flags[class], raw[node], item.byte_elements)
                            .borrow()
                            .is_some()
                })
        });
        if next == active {
            break;
        }
        active = next;
    }
    let lengths: HashMap<(usize, usize), usize> = active
        .iter()
        .map(|&(function, pointer, length)| ((function, pointer), length))
        .collect();

    let mut facts = PointerFacts {
        bindings: HashMap::new(),
        functions: HashMap::new(),
    };
    for (function, summary) in summaries.iter().enumerate() {
        let Some(summary) = summary else { continue };
        let Some(entry) = program.function(FunctionId(function as u32)) else {
            continue;
        };
        let mut reported = Vec::new();
        for (node, item) in summary.nodes.iter().enumerate() {
            let role = match item.kind {
                NodeKind::Parameter(_) if candidates[function] => PointerRole::Parameter,
                NodeKind::Local => PointerRole::Local,
                NodeKind::Parameter(_) | NodeKind::Temporary | NodeKind::Return => continue,
            };
            let global = base[function] + node as u32;
            let class = class_of[global as usize] as usize;
            let capability =
                capability(class_flags[class], raw[global as usize], item.byte_elements);
            let length = match item.kind {
                NodeKind::Parameter(index) => {
                    let lengths = &lengths;
                    lengths.get(&(function, index)).map(|&parameter| {
                        let binding = summary.parameters[parameter];
                        let forwarded =
                            summary
                                .calls
                                .iter()
                                .flat_map(|site| {
                                    let callee = site.callee.0 as usize;
                                    site.shapes.iter().enumerate().filter(move |&(index, shape)| {
                                    matches!(shape, Shape::Binding(read) if *read == binding)
                                        && lengths.iter().any(|(&(function, _), &length)| {
                                            function == callee && length == index
                                        })
                                })
                                })
                                .count() as u32;
                        Length {
                            parameter,
                            read: summary.reads.get(&binding).copied().unwrap_or(0) > forwarded,
                        }
                    })
                }
                NodeKind::Local | NodeKind::Temporary | NodeKind::Return => None,
            };
            let borrow = capability
                .borrow()
                .filter(|borrow| {
                    !item.opaque_pointee
                        && !unproven[class]
                        && (role == PointerRole::Parameter || !borrow.mutable)
                })
                .map(|borrow| Borrow {
                    many: borrow.many || length.is_some(),
                    ..borrow
                });
            facts.bindings.insert(
                ProgramBinding {
                    unit: entry.unit,
                    binding: item.binding,
                },
                PointerFact {
                    role,
                    capability,
                    borrow,
                    length,
                },
            );
            reported.push(item.binding);
        }
        if !reported.is_empty() {
            facts.functions.insert(
                ProgramBinding {
                    unit: entry.unit,
                    binding: entry.function.value.id,
                },
                reported,
            );
        }
    }
    if let Some((summarize, joined)) = summarized {
        for (phase, elapsed) in [("summarize", summarize), ("join", joined.elapsed())] {
            eprintln!(
                "interproc-timing pass=ir_pointer phase={phase} ms={:.3}",
                elapsed.as_secs_f64() * 1e3
            );
        }
        eprintln!("interproc-count pass=ir_pointer name=nodes value={total}");
    }
    facts
}
