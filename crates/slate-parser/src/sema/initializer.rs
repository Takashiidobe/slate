use super::ctype::convert::ConversionContext;
use super::ctype::{CTypeKind, Extent, QualType};
use super::expression::Lowerer;
use super::numeric::ResolveError;
use super::operand::Operand;
use super::types::TypeResolver;
use crate::ast::{Designator, Expr, ExprKind, Initializer, InitializerItem, Span};
use crate::const_expr::{Encoding, StringLiteral};
use crate::ir::*;

#[derive(Clone)]
enum Shape {
    Scalar,
    Struct(Vec<Field>),
    Union(Vec<Field>),
    Array {
        element: Type,
        length: Option<u64>,
        vector: bool,
    },
}

#[derive(Clone)]
enum Entry {
    Leaf(Value),
    Sub(Builder),
}

#[derive(Clone)]
struct Builder {
    c: QualType,
    ty: Type,
    shape: Shape,
    members: Vec<(AggregateTarget, Entry)>,
    next: u64,
}

struct Cursor<'a> {
    items: &'a [InitializerItem],
    index: usize,
    pending: Option<Operand>,
}

type Step = (AggregateTarget, Type);

fn initializable(field: &Field) -> bool {
    field.name.is_some() || field.bit_width.is_none()
}

fn is_flexible(field: &Field) -> bool {
    matches!(field.ty, Type::Array { length: None, .. })
}

fn bounds(target: AggregateTarget) -> (u64, u64) {
    match target {
        AggregateTarget::Field(index) => (index as u64, index as u64),
        AggregateTarget::Index(index) => (index, index),
        AggregateTarget::Range { start, end } => (start, end),
    }
}

fn cover(start: u64, end: u64) -> AggregateTarget {
    if start == end {
        AggregateTarget::Index(start)
    } else {
        AggregateTarget::Range { start, end }
    }
}

fn last_position(target: AggregateTarget) -> AggregateTarget {
    match target {
        AggregateTarget::Range { end, .. } => AggregateTarget::Index(end),
        other => other,
    }
}

fn sized(ty: &Type) -> Result<Type, ResolveError> {
    match ty {
        Type::Array { length: None, .. } => Err(ResolveError::Unimplemented(
            "flexible array member initializer",
        )),
        _ => Ok(ty.clone()),
    }
}

fn shape_full(shape: &Shape, next: u64, members: usize) -> bool {
    match shape {
        Shape::Struct(fields) => !fields.iter().skip(next as usize).any(initializable),
        Shape::Union(fields) => members > 0 || !fields.iter().any(initializable),
        Shape::Array { length, .. } => length.is_some_and(|length| next >= length),
        Shape::Scalar => true,
    }
}

fn shape_next_target(shape: &Shape, next: u64) -> Result<Step, ResolveError> {
    match shape {
        Shape::Struct(fields) | Shape::Union(fields) => fields
            .iter()
            .enumerate()
            .skip(next as usize)
            .find(|(_, field)| initializable(field))
            .map(|(index, field)| (AggregateTarget::Field(index), field.ty.clone()))
            .ok_or(ResolveError::Rejected("excess elements in initializer")),
        Shape::Array { element, .. } => Ok((AggregateTarget::Index(next), sized(element)?)),
        Shape::Scalar => Err(ResolveError::Rejected("initializer list for scalar")),
    }
}

impl Builder {
    fn full(&self) -> bool {
        shape_full(&self.shape, self.next, self.members.len())
    }

    fn next_target(&self) -> Result<Step, ResolveError> {
        shape_next_target(&self.shape, self.next).map_err(ResolveError::checked)
    }

    fn advance(&mut self, target: AggregateTarget) {
        self.next = bounds(target).1 + 1;
    }

    fn split_out(&mut self, start: u64, end: u64) {
        let members = std::mem::take(&mut self.members);
        for (target, entry) in members {
            let (existing_start, existing_end) = bounds(target);
            let straddles = matches!(
                target,
                AggregateTarget::Index(_) | AggregateTarget::Range { .. }
            ) && existing_start <= end
                && start <= existing_end
                && !(start <= existing_start && existing_end <= end);
            if !straddles {
                self.members.push((target, entry));
                continue;
            }
            if existing_start < start {
                self.members
                    .push((cover(existing_start, start - 1), entry.clone()));
            }
            self.members.push((
                cover(existing_start.max(start), existing_end.min(end)),
                entry.clone(),
            ));
            if existing_end > end {
                self.members.push((cover(end + 1, existing_end), entry));
            }
        }
    }

    fn contains(&self, position: usize, start: u64, end: u64) -> bool {
        let (existing_start, existing_end) = bounds(self.members[position].0);
        start <= existing_start && existing_end <= end
    }

    fn write(&mut self, target: AggregateTarget, entry: Entry) -> Result<(), ResolveError> {
        if matches!(self.shape, Shape::Union(_)) {
            self.members.clear();
            self.members.push((target, entry));
            return Ok(());
        }
        let (start, end) = bounds(target);
        self.split_out(start, end);
        self.members.retain(|(existing, _)| {
            let (existing_start, existing_end) = bounds(*existing);
            !(start <= existing_start && existing_end <= end)
        });
        self.members.push((target, entry));
        Ok(())
    }

    fn partitions(
        &mut self,
        target: AggregateTarget,
        fresh: &Builder,
        split_end: bool,
    ) -> Result<Vec<usize>, ResolveError> {
        if matches!(self.shape, Shape::Union(_)) {
            if self.members.iter().any(|(existing, _)| *existing != target) {
                self.members.clear();
            }
            if self.members.is_empty() {
                self.members.push((target, Entry::Sub(fresh.clone())));
            }
        } else if matches!(target, AggregateTarget::Field(_)) {
            if !self.members.iter().any(|(existing, _)| *existing == target) {
                self.members.push((target, Entry::Sub(fresh.clone())));
            }
        } else {
            let (start, end) = bounds(target);
            self.split_out(start, end);
            let mut covered: Vec<(u64, u64)> = (0..self.members.len())
                .filter(|&position| self.contains(position, start, end))
                .map(|position| bounds(self.members[position].0))
                .collect();
            covered.sort_unstable();
            let mut open = start;
            let mut gaps = Vec::new();
            for (covered_start, covered_end) in covered {
                if open < covered_start {
                    gaps.push((open, covered_start - 1));
                }
                open = covered_end + 1;
            }
            if open <= end {
                gaps.push((open, end));
            }
            for (gap_start, gap_end) in gaps {
                self.members
                    .push((cover(gap_start, gap_end), Entry::Sub(fresh.clone())));
            }
            if split_end {
                self.split_out(end, end);
            }
        }
        let (start, end) = bounds(target);
        let mut positions: Vec<usize> = (0..self.members.len())
            .filter(|&position| self.contains(position, start, end))
            .collect();
        positions.sort_by_key(|&position| bounds(self.members[position].0).0);
        for &position in &positions {
            if matches!(self.members[position].1, Entry::Leaf(_)) {
                return Err(ResolveError::Unimplemented(
                    "designator into initialized scalar or copied aggregate",
                ));
            }
        }
        Ok(positions)
    }

    fn finish(mut self, anchor: &Span<()>) -> Result<Value, ResolveError> {
        self.members.sort_by_key(|(target, _)| bounds(*target).0);
        let covered: u64 = self
            .members
            .iter()
            .map(|(target, _)| {
                let (start, end) = bounds(*target);
                end - start + 1
            })
            .sum();
        let (ty, zero_fill) = match &self.shape {
            Shape::Struct(fields) => {
                let flexible = self
                    .members
                    .iter()
                    .filter(|(target, _)| {
                        matches!(target, AggregateTarget::Field(index) if is_flexible(&fields[*index]))
                    })
                    .count() as u64;
                let required = fields
                    .iter()
                    .filter(|field| initializable(field) && !is_flexible(field))
                    .count() as u64;
                (self.ty.clone(), covered - flexible < required)
            }
            Shape::Union(_) => (self.ty.clone(), false),
            Shape::Array {
                element,
                length,
                vector,
            } => {
                let length = match length {
                    Some(length) => *length,
                    None => self
                        .members
                        .last()
                        .map_or(0, |(target, _)| bounds(*target).1 + 1),
                };
                let ty = if *vector {
                    self.ty.clone()
                } else {
                    Type::Array {
                        element: Box::new(element.clone()),
                        length: Some(length),
                    }
                };
                (ty, covered < length)
            }
            Shape::Scalar => (self.ty.clone(), true),
        };
        let mut members = Vec::new();
        for (target, entry) in self.members {
            let value = match entry {
                Entry::Leaf(value) => value,
                Entry::Sub(builder) => builder.finish(anchor)?,
            };
            members.push(AggregateMember { target, value });
        }
        Ok(Value {
            ty,
            node: anchor
                .clone()
                .derive(ValueKind::Aggregate { members, zero_fill }),
        })
    }
}

impl<'a> Cursor<'a> {
    fn new(items: &'a [InitializerItem]) -> Self {
        Self {
            items,
            index: 0,
            pending: None,
        }
    }
}

pub(super) struct ElementError {
    pub(super) at: Option<Span<()>>,
    pub(super) error: ResolveError,
}

impl From<ResolveError> for ElementError {
    fn from(error: ResolveError) -> Self {
        Self { at: None, error }
    }
}

struct Walk {
    c: QualType,
    shape: Shape,
    next: u64,
    reach: u64,
    written: bool,
}

impl Walk {
    fn full(&self) -> bool {
        shape_full(&self.shape, self.next, usize::from(self.written))
    }

    fn advance(&mut self, target: AggregateTarget) {
        self.next = bounds(target).1 + 1;
        self.reach = self.reach.max(self.next);
        self.written = true;
    }
}

impl TypeResolver {
    pub(super) fn kind(&self, ty: &Type) -> Option<&TypeDefinitionKind> {
        match ty {
            Type::Defined(id) => self.definitions.get(id.0 as usize).map(|d| &d.kind),
            _ => None,
        }
    }

    fn initializes_whole(&self, value: QualType, target: QualType) -> bool {
        self.ctypes.compatible_unqualified(
            self.ctypes.unqualified_view(value),
            self.ctypes.unqualified_view(target),
        )
    }

    fn walk(&self, c: QualType) -> Result<Walk, ResolveError> {
        Ok(Walk {
            c,
            shape: self.shape(&self.ir_type(c))?,
            next: 0,
            reach: 0,
            written: false,
        })
    }

    fn subobject_type(
        &self,
        c: QualType,
        target: AggregateTarget,
    ) -> Result<QualType, ResolveError> {
        match (self.ctypes.canonical_kind(c), target) {
            (CTypeKind::Record { id, .. }, AggregateTarget::Field(index)) => self
                .record_fields
                .get(id)
                .and_then(|fields| fields.get(index))
                .copied()
                .ok_or(ResolveError::Internal("missing initializer field type")),
            (CTypeKind::Vector { element, .. }, _) => Ok(*element),
            (_, AggregateTarget::Index(_) | AggregateTarget::Range { .. }) => self
                .ctypes
                .element(c)
                .map(|(element, _)| element)
                .ok_or(ResolveError::Internal("missing initializer element type")),
            _ => Err(ResolveError::Internal("invalid initializer subobject")),
        }
    }

    fn unaliased(&self, ty: &Type) -> Type {
        let mut ty = ty.clone();
        while let Some(TypeDefinitionKind::Alias(inner)) = self.kind(&ty) {
            ty = inner.clone();
        }
        ty
    }

    fn shape(&self, ty: &Type) -> Result<Shape, ResolveError> {
        let ty = self.unaliased(ty);
        if let Type::Array { element, length } = &ty {
            return Ok(Shape::Array {
                element: (**element).clone(),
                length: *length,
                vector: false,
            });
        }
        if let Type::Vector { element, lanes } = &ty {
            return Ok(Shape::Array {
                element: Type::Numeric(*element),
                length: Some(u64::from(*lanes)),
                vector: true,
            });
        }
        match self.kind(&ty) {
            Some(TypeDefinitionKind::Record { kind, fields, .. }) => {
                let fields = fields
                    .as_ref()
                    .ok_or(ResolveError::Rejected("initializer for incomplete record"))?
                    .iter()
                    .map(|field| field.value.clone())
                    .collect();
                Ok(match kind {
                    RecordKind::Struct => Shape::Struct(fields),
                    RecordKind::Union => Shape::Union(fields),
                })
            }
            _ => Ok(Shape::Scalar),
        }
    }

    fn array_index(&mut self, expr: &Expr, length: Option<u64>) -> Result<u64, ResolveError> {
        let index = self
            .constant_integer(expr)
            .ok()
            .and_then(|index| u64::try_from(index).ok())
            .ok_or(ResolveError::Rejected(
                "non-constant or negative array designator",
            ))?;
        if length.is_some_and(|length| index >= length) {
            return Err(ResolveError::Rejected("array designator out of range"));
        }
        Ok(index)
    }

    fn designator_step(
        &mut self,
        shape: &Shape,
        designator: &Designator,
    ) -> Result<Vec<Step>, ResolveError> {
        match (shape, designator) {
            (Shape::Struct(fields) | Shape::Union(fields), Designator::Field(name)) => self
                .field_path(fields, &name.value)
                .ok_or(ResolveError::Rejected("unknown field designator")),
            (
                Shape::Array {
                    element, length, ..
                },
                Designator::Array(index),
            ) => {
                let index = self.array_index(index, *length)?;
                Ok(vec![(AggregateTarget::Index(index), sized(element)?)])
            }
            (
                Shape::Array {
                    element, length, ..
                },
                Designator::ArrayRange { start, end },
            ) => {
                let start = self.array_index(start, *length)?;
                let end = self.array_index(end, *length)?;
                if end < start {
                    return Err(ResolveError::Rejected("empty designated range"));
                }
                Ok(vec![(cover(start, end), sized(element)?)])
            }
            _ => Err(ResolveError::Rejected(
                "designator does not match aggregate type",
            )),
        }
    }

    fn field_path(&self, fields: &[Field], name: &str) -> Option<Vec<Step>> {
        for (index, field) in fields.iter().enumerate() {
            let step = (AggregateTarget::Field(index), field.ty.clone());
            if field.name.as_deref() == Some(name) {
                return Some(vec![step]);
            }
            if field.name.is_none()
                && field.bit_width.is_none()
                && let Ok(Shape::Struct(inner) | Shape::Union(inner)) = self.shape(&field.ty)
                && let Some(path) = self.field_path(&inner, name)
            {
                return Some([vec![step], path].concat());
            }
        }
        None
    }

    fn designator_steps(
        &mut self,
        ty: &Type,
        designators: &[Designator],
    ) -> Result<Vec<Step>, ResolveError> {
        let mut steps: Vec<Step> = Vec::new();
        let mut current = ty.clone();
        for designator in designators {
            let shape = self.shape(&current)?;
            let resolved = self.designator_step(&shape, designator)?;
            current = resolved
                .last()
                .map(|(_, ty)| ty.clone())
                .ok_or(ResolveError::Internal("empty designator path"))?;
            steps.extend(resolved);
        }
        if steps.is_empty() {
            return Err(ResolveError::Internal("empty designator list"));
        }
        Ok(steps)
    }

    pub(super) fn inferred_array_length(
        &mut self,
        element: QualType,
        items: &[InitializerItem],
    ) -> Result<u64, ResolveError> {
        if let [item] = items
            && item.designators.is_empty()
            && let Initializer::Expr(expr) = &item.value
            && let ExprKind::StringLiteral(literal) = &expr.value
            && self.ctypes.is_integer(element)
            && let Some((_, Extent::Fixed(length))) = {
                let string = self.string_type(literal);
                self.ctypes.element(string)
            }
        {
            return Ok(length);
        }
        let c = self.ctypes.qual(CTypeKind::Array {
            element,
            extent: Extent::Incomplete,
        });
        let mut walk = self.walk(c)?;
        let mut index = 0;
        self.walk_fill(&mut walk, items, &mut index, true, false, false)
            .map_err(|error| error.error)?;
        Ok(walk.reach)
    }

    // lowering's current-object walk over types alone, recording each element's conversion
    pub(super) fn check_initializer(
        &mut self,
        c: QualType,
        initializer: &Initializer,
    ) -> Result<(), ElementError> {
        match initializer {
            Initializer::List(items) => self.check_braced(c, items),
            Initializer::Expr(expr) => {
                let ty = self.ir_type(c);
                if matches!(self.shape(&ty)?, Shape::Array { .. })
                    && self.string_array(expr, &ty)?.is_some()
                {
                    return Ok(());
                }
                self.record_element(expr, c)
            }
        }
    }

    pub(super) fn check_braced(
        &mut self,
        c: QualType,
        items: &[InitializerItem],
    ) -> Result<(), ElementError> {
        let ty = self.ir_type(c);
        let shape = self.shape(&ty)?;
        if let (Type::Complex(_), [real, imaginary]) = (self.unaliased(&ty), items)
            && real.designators.is_empty()
            && imaginary.designators.is_empty()
        {
            let component = self.ctypes.arithmetic_component(c);
            self.check_initializer(component, &real.value)?;
            return self.check_initializer(component, &imaginary.value);
        }
        if matches!(shape, Shape::Scalar) {
            return match items {
                [] => Ok(()),
                [first, ..] if first.designators.is_empty() => {
                    self.check_initializer(c, &first.value)
                }
                _ => {
                    Err(ResolveError::Rejected("designator in initializer for scalar type").into())
                }
            };
        }
        if let (
            Shape::Array { .. },
            [
                InitializerItem {
                    designators,
                    value: Initializer::Expr(expr),
                },
            ],
        ) = (&shape, items)
            && designators.is_empty()
            && self.string_array(expr, &ty)?.is_some()
        {
            return Ok(());
        }
        let mut walk = self.walk(c)?;
        let mut index = 0;
        self.walk_fill(&mut walk, items, &mut index, true, false, true)
    }

    fn record_element(&mut self, expr: &Expr, to: QualType) -> Result<(), ElementError> {
        let at = |error| ElementError {
            at: Some(expr.derive(())),
            error,
        };
        let from = self.operand_type(expr).map_err(at)?;
        self.record_conversion(expr, from, to, ConversionContext::Assign)
            .map_err(at)?;
        let to = self.ctypes.unqualified(to);
        self.element_targets.insert(expr.id, to);
        Ok(())
    }

    fn walk_fill(
        &mut self,
        walk: &mut Walk,
        items: &[InitializerItem],
        index: &mut usize,
        braced: bool,
        pending: bool,
        check: bool,
    ) -> Result<(), ElementError> {
        let mut pending = pending;
        while *index < items.len() {
            let designators = &items[*index].designators;
            if !designators.is_empty() && !pending {
                if !braced {
                    break;
                }
                let ty = self.ir_type(walk.c);
                let steps = self.designator_steps(&ty, designators)?;
                self.walk_place(walk, &steps, items, index, check)?;
                continue;
            }
            if walk.full() {
                if !braced {
                    break;
                }
                *index += 1;
                pending = false;
                continue;
            }
            let (target, _) = shape_next_target(&walk.shape, walk.next)?;
            let c = self.subobject_type(walk.c, target)?;
            *index = self.walk_item(c, items, *index, check)?;
            pending = false;
            walk.advance(target);
        }
        Ok(())
    }

    fn walk_place(
        &mut self,
        walk: &mut Walk,
        steps: &[Step],
        items: &[InitializerItem],
        index: &mut usize,
        check: bool,
    ) -> Result<(), ElementError> {
        let (target, _) = steps
            .first()
            .cloned()
            .ok_or(ResolveError::Internal("empty designator path"))?;
        let c = self.subobject_type(walk.c, target)?;
        if steps.len() == 1 {
            *index = self.walk_item(c, items, *index, check)?;
        } else {
            let mut sub = self.walk(c)?;
            self.walk_place(&mut sub, &steps[1..], items, index, check)?;
            self.walk_fill(&mut sub, items, index, false, false, check)?;
        }
        walk.advance(target);
        Ok(())
    }

    fn walk_item(
        &mut self,
        c: QualType,
        items: &[InitializerItem],
        index: usize,
        check: bool,
    ) -> Result<usize, ElementError> {
        let expr = match &items[index].value {
            Initializer::List(inner) => {
                if check {
                    self.check_braced(c, inner)?;
                }
                return Ok(index + 1);
            }
            Initializer::Expr(expr) => expr,
        };
        let ty = self.ir_type(c);
        let shape = self.shape(&ty)?;
        if matches!(shape, Shape::Array { .. }) && self.string_array(expr, &ty)?.is_some() {
            return Ok(index + 1);
        }
        let value = match self.operand_type(expr) {
            Ok(value) => Some(value),
            Err(error) if check => {
                return Err(ElementError {
                    at: Some(expr.derive(())),
                    error,
                });
            }
            Err(_) => None,
        };
        let whole = match shape {
            Shape::Scalar => true,
            Shape::Struct(_) | Shape::Union(_) => {
                value.is_some_and(|value| self.initializes_whole(value, c))
            }
            Shape::Array { vector, .. } => {
                vector && value.is_some_and(|value| self.initializes_whole(value, c))
            }
        };
        if whole {
            if check {
                self.record_element(expr, c)?;
            }
            return Ok(index + 1);
        }
        let mut sub = self.walk(c)?;
        let reached = if sub.full() { index + 1 } else { index };
        let mut next = index;
        // the elided walk ignores the designator an outer walk already applied to this item
        self.walk_fill(&mut sub, items, &mut next, false, true, check)?;
        Ok(reached.max(next))
    }

    fn string_array<'e>(
        &mut self,
        e: &'e Expr,
        ty: &Type,
    ) -> Result<Option<&'e StringLiteral>, ResolveError> {
        let ExprKind::StringLiteral(literal) = &e.value else {
            return Ok(None);
        };
        let Type::Array { element, .. } = ty else {
            return Ok(None);
        };
        let Type::Numeric(NumericType::Integer {
            width,
            signed,
            bit_precise: false,
        }) = element.as_ref()
        else {
            return Ok(None);
        };
        let literal_c = self.string_type(literal);
        let Type::Array {
            element: literal_element,
            ..
        } = self.ir_type(literal_c)
        else {
            return Err(ResolveError::Internal("string literal type"));
        };
        let any_signedness = matches!(literal.encoding, Encoding::Plain | Encoding::Utf8);
        let compatible = matches!(
            literal_element.as_ref(),
            Type::Numeric(NumericType::Integer {
                width: literal_width,
                signed: literal_signed,
                ..
            }) if width == literal_width && (any_signedness || signed == literal_signed)
        );
        if !compatible {
            return Err(ResolveError::Rejected(
                "string literal initializer for incompatible array element",
            ));
        }
        Ok(Some(literal))
    }
}

impl Lowerer {
    pub(super) fn initializer_value(
        &mut self,
        c: QualType,
        initializer: &Initializer,
        anchor: &Span<()>,
    ) -> Result<Value, ResolveError> {
        match self.init_initializer(c, initializer)? {
            Entry::Leaf(value) => Ok(value),
            Entry::Sub(builder) => builder.finish(anchor),
        }
    }

    fn shape(&self, ty: &Type) -> Result<Shape, ResolveError> {
        self.types.shape(ty).map_err(ResolveError::checked)
    }

    fn builder(&self, c: QualType) -> Result<Builder, ResolveError> {
        let ty = self.types.ir_type(c);
        match self.shape(&ty)? {
            Shape::Scalar => Err(ResolveError::Internal(
                "braced initializer or designator for scalar",
            )),
            shape => Ok(Builder {
                c,
                ty: ty.clone(),
                shape,
                members: Vec::new(),
                next: 0,
            }),
        }
    }

    fn init_initializer(
        &mut self,
        c: QualType,
        value: &Initializer,
    ) -> Result<Entry, ResolveError> {
        match value {
            Initializer::List(items) => self.braced(c, items),
            Initializer::Expr(expr) => {
                let ty = self.types.ir_type(c);
                if matches!(self.shape(&ty)?, Shape::Array { .. })
                    && let Some(value) = self.string_array_initializer(expr, &ty)?
                {
                    return Ok(Entry::Leaf(value));
                }
                let value = self.expr(expr)?;
                let value = self.convert_element(expr, value, c)?;
                Ok(Entry::Leaf(value.value))
            }
        }
    }

    fn braced(&mut self, c: QualType, items: &[InitializerItem]) -> Result<Entry, ResolveError> {
        let ty = &self.types.ir_type(c);
        let shape = self.shape(ty)?;
        if let (Type::Complex(_component), [real, imaginary]) = (self.types.unaliased(ty), items)
            && real.designators.is_empty()
            && imaginary.designators.is_empty()
        {
            let component = self.types.ctypes.arithmetic_component(c);
            let mut members = Vec::new();
            for (index, item) in [real, imaginary].into_iter().enumerate() {
                let Entry::Leaf(value) = self.init_initializer(component, &item.value)? else {
                    return Err(ResolveError::Internal(
                        "braced initializer for complex component",
                    ));
                };
                members.push(AggregateMember {
                    target: AggregateTarget::Index(index as u64),
                    value,
                });
            }
            let node = members[0].value.node.clone();
            return Ok(Entry::Leaf(Value {
                ty: ty.clone(),
                node: node.derive(ValueKind::Aggregate {
                    members,
                    zero_fill: false,
                }),
            }));
        }
        if matches!(shape, Shape::Scalar) {
            return match items {
                [] => Ok(Entry::Sub(Builder {
                    c,
                    ty: ty.clone(),
                    shape,
                    members: Vec::new(),
                    next: 0,
                })),
                [first, ..] if first.designators.is_empty() => {
                    self.init_initializer(c, &first.value)
                }
                _ => Err(ResolveError::Internal(
                    "designator in initializer for scalar type",
                )),
            };
        }
        if let (
            Shape::Array { .. },
            [
                InitializerItem {
                    designators,
                    value: Initializer::Expr(expr),
                },
            ],
        ) = (&shape, items)
            && designators.is_empty()
            && let Some(value) = self.string_array_initializer(expr, ty)?
        {
            return Ok(Entry::Leaf(value));
        }
        let mut cursor = Cursor::new(items);
        let mut builder = self.builder(c)?;
        self.fill(&mut builder, &mut cursor, true)?;
        Ok(Entry::Sub(builder))
    }

    fn fill(
        &mut self,
        builder: &mut Builder,
        cursor: &mut Cursor<'_>,
        braced: bool,
    ) -> Result<(), ResolveError> {
        let items = cursor.items;
        while cursor.index < items.len() {
            let designators = &items[cursor.index].designators;
            if !designators.is_empty() && cursor.pending.is_none() {
                if !braced {
                    break;
                }
                let steps = self
                    .types
                    .designator_steps(&builder.ty, designators)
                    .map_err(ResolveError::checked)?;
                let start = cursor.index;
                self.designate(builder, &steps, cursor)?;
                self.resume(builder, &steps, cursor)?;
                if cursor.index == start {
                    return Err(ResolveError::Internal(
                        "designated initializer consumed nothing",
                    ));
                }
                continue;
            }
            if builder.full() {
                if !braced {
                    break;
                }
                cursor.index += 1;
                cursor.pending = None;
                continue;
            }
            let (target, _) = builder.next_target()?;
            self.init_into(builder, target, cursor)?;
            builder.advance(target);
        }
        Ok(())
    }

    fn init_into(
        &mut self,
        builder: &mut Builder,
        target: AggregateTarget,
        cursor: &mut Cursor<'_>,
    ) -> Result<(), ResolveError> {
        let c = self.types.subobject_type(builder.c, target)?;
        if let Some(entry) = self.item_entry(c, cursor)? {
            return builder.write(target, entry);
        }
        let pending = cursor.pending.take();
        let saved = cursor.index;
        let mut reached = saved;
        let fresh = self.builder(c)?;
        if fresh.full() {
            reached += 1;
        }
        for position in builder.partitions(target, &fresh, false)? {
            cursor.index = saved;
            cursor.pending = pending.clone();
            let Entry::Sub(sub) = &mut builder.members[position].1 else {
                return Err(ResolveError::Unimplemented(
                    "designator into initialized scalar or copied aggregate",
                ));
            };
            sub.next = 0;
            self.fill(sub, cursor, false)?;
            reached = reached.max(cursor.index);
        }
        cursor.index = reached;
        cursor.pending = None;
        Ok(())
    }

    fn item_entry(
        &mut self,
        c: QualType,
        cursor: &mut Cursor<'_>,
    ) -> Result<Option<Entry>, ResolveError> {
        let items = cursor.items;
        let expr = match &items[cursor.index].value {
            Initializer::List(inner) => {
                cursor.index += 1;
                return self.braced(c, inner).map(Some);
            }
            Initializer::Expr(expr) => expr,
        };
        let ty = self.types.ir_type(c);
        let shape = self.shape(&ty)?;
        if matches!(shape, Shape::Array { .. })
            && let Some(value) = self.string_array_initializer(expr, &ty)?
        {
            cursor.index += 1;
            return Ok(Some(Entry::Leaf(value)));
        }
        let value = match cursor.pending.take() {
            Some(value) => value,
            None => self.expr(expr)?,
        };
        let whole = match shape {
            Shape::Scalar => true,
            Shape::Struct(_) | Shape::Union(_) => self.types.initializes_whole(value.c, c),
            Shape::Array { vector, .. } => vector && self.types.initializes_whole(value.c, c),
        };
        if whole {
            cursor.index += 1;
            let value = self.convert_element(expr, value, c)?;
            return Ok(Some(Entry::Leaf(value.value)));
        }
        cursor.pending = Some(value);
        Ok(None)
    }

    fn designate(
        &mut self,
        builder: &mut Builder,
        steps: &[Step],
        cursor: &mut Cursor<'_>,
    ) -> Result<(), ResolveError> {
        let (target, _) = *steps
            .first()
            .ok_or(ResolveError::Internal("empty designator path"))?;
        if steps.len() == 1 {
            return self.init_into(builder, target, cursor);
        }
        let c = self.types.subobject_type(builder.c, target)?;
        let fresh = self.builder(c)?;
        let saved = cursor.index;
        let mut reached = saved;
        for position in builder.partitions(target, &fresh, true)? {
            cursor.index = saved;
            let Entry::Sub(sub) = &mut builder.members[position].1 else {
                return Err(ResolveError::Unimplemented(
                    "designator into initialized scalar or copied aggregate",
                ));
            };
            self.designate(sub, &steps[1..], cursor)?;
            reached = cursor.index;
        }
        cursor.index = reached;
        Ok(())
    }

    fn resume(
        &mut self,
        builder: &mut Builder,
        steps: &[Step],
        cursor: &mut Cursor<'_>,
    ) -> Result<(), ResolveError> {
        let (target, _) = *steps
            .first()
            .ok_or(ResolveError::Internal("empty designator path"))?;
        if steps.len() > 1 {
            let last = last_position(target);
            let c = self.types.subobject_type(builder.c, last)?;
            let fresh = self.builder(c)?;
            let positions = builder.partitions(last, &fresh, false)?;
            let position = *positions
                .last()
                .ok_or(ResolveError::Internal("empty designator path"))?;
            let Entry::Sub(sub) = &mut builder.members[position].1 else {
                return Err(ResolveError::Unimplemented(
                    "designator into initialized scalar or copied aggregate",
                ));
            };
            self.resume(sub, &steps[1..], cursor)?;
            self.fill(sub, cursor, false)?;
        }
        builder.advance(target);
        Ok(())
    }

    fn string_array_initializer(
        &mut self,
        e: &Expr,
        ty: &Type,
    ) -> Result<Option<Value>, ResolveError> {
        let Some(literal) = self
            .types
            .string_array(e, ty)
            .map_err(ResolveError::checked)?
        else {
            return Ok(None);
        };
        let Type::Array { element, length } = ty else {
            return Err(ResolveError::Internal("string initializer for non-array"));
        };
        let mut units = literal.execution_units(self.context.target.wchar_width);
        units.push(0);
        let length = length.unwrap_or(units.len() as u64);
        units.resize(length as usize, 0);
        let ty = Type::Array {
            element: element.clone(),
            length: Some(length),
        };
        Ok(Some(self.value(e, ty, ValueKind::CodeUnits(units))))
    }

    fn convert_element(
        &mut self,
        e: &Expr,
        value: Operand,
        to: QualType,
    ) -> Result<Operand, ResolveError> {
        let to = self.types.ctypes.unqualified(to);
        if !self
            .types
            .element_targets
            .get(&e.id)
            .is_some_and(|&checked| self.types.initializes_whole(checked, to))
        {
            return Err(ResolveError::Internal(
                "initializer element target differs from the checker",
            ));
        }
        self.convert_recorded(e, value, to, ConversionReason::Assign)
    }
}
