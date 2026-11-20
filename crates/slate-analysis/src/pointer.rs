use crate::identity::{FunctionIdentity, Known};
use crate::program::{FunctionId, Program, ProgramBinding, UnitId};
use foldhash::{HashMap, HashMapExt};
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
pub struct PointerFact {
    pub role: PointerRole,
    pub capability: PointerCapability,
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
    flags: u8,
    raw: bool,
    reads: u32,
}

struct CallSite {
    callee: FunctionId,
    arguments: Vec<(usize, u32)>,
    objects: Vec<(usize, BindingId)>,
}

struct Summary {
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
            if moved {
                self.nodes[destination as usize].raw = true;
            }
        }
        if sources.opaque {
            self.nodes[destination as usize].raw = true;
        }
        if sources.null {
            self.flag(destination, NULLABLE);
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
            PlaceKind::MacroArg { place, .. } => self.place(place, access),
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
            ValueKind::StatementExpression(evaluation)
            | ValueKind::MacroExpansion { evaluation, .. } => self.evaluation(evaluation),
            ValueKind::MacroArg { value, .. } => self.value(value),
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
            };
            for (index, argument) in arguments.iter().enumerate() {
                let sources = self.sources(argument);
                for (node, _) in sources.roots {
                    site.arguments.push((index, node));
                }
                for object in sources.objects {
                    site.objects.push((index, object));
                }
            }
            if !site.arguments.is_empty() || site.objects.len() > 1 {
                self.calls.push(site);
            }
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
                if let Some(initializer) = &variable.initializer {
                    self.value(initializer);
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
            | Statement::Label { body, .. }
            | Statement::MacroExpansion { body, .. } => self.statements(body),
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
    function: FunctionId,
) -> Option<Summary> {
    let entry = program.function(function)?;
    let mut walker = Walker {
        program,
        identities,
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
    };
    if let Some(return_type) = &entry.function.return_type {
        walker.returned = walker.anonymous(entry.function.value.id, return_type, NodeKind::Return);
    }
    if let Parameters::Prototype { fixed, .. } = &entry.function.parameters {
        for (index, parameter) in fixed.iter().enumerate() {
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
    let summaries: Vec<Option<Summary>> = (0..count as u32)
        .into_par_iter()
        .map(|index| summarize(program, identities, FunctionId(index)))
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
            let flags = class_flags[classes.find(global) as usize];
            let capability = PointerCapability {
                write: flags & WRITE != 0,
                unique: flags & SHARED == 0,
                free: flags & FREE != 0,
                offset: flags & OFFSET != 0,
                escape: flags & ESCAPE != 0,
                nullable: flags & NULLABLE != 0,
                string: flags & STRING != 0 && item.byte_elements,
                raw: raw[global as usize],
                byte_elements: item.byte_elements,
            };
            facts.bindings.insert(
                ProgramBinding {
                    unit: entry.unit,
                    binding: item.binding,
                },
                PointerFact { role, capability },
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
