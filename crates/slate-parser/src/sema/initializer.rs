use std::collections::HashMap;
use std::rc::Rc;

use super::ctype::convert::ConversionContext;
use super::ctype::{CTypeKind, Extent, QualType};
use super::expression::Lowerer;
use super::numeric::ResolveError;
use super::types::TypeResolver;
use crate::ast::{Designator, Expr, ExprKind, Initializer, InitializerItem, NodeId, Span};
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
    ty: Type,
    shape: Shape,
    members: Vec<(AggregateTarget, Entry)>,
}

type Step = (AggregateTarget, Type);

type Path = Vec<(AggregateTarget, QualType)>;

pub(super) type RecordedPlan = Result<Rc<InitializerPlan>, ResolveError>;

pub(super) struct InitializerPlan {
    c: QualType,
    writes: Vec<Write>,
}

#[derive(Clone, Copy)]
pub(super) enum InitializerSource<'e> {
    Initializer(&'e Initializer),
    CompoundLiteral(&'e [InitializerItem]),
}

struct Write {
    path: Path,
    value: Planned,
}

enum Planned {
    Element { expr: NodeId, to: QualType },
    CodeUnits { expr: NodeId, c: QualType },
    Braced(QualType),
    Open(QualType),
    Complex { c: QualType, parts: [NodeId; 2] },
}

struct Planner {
    writes: Vec<Write>,
    check: bool,
}

impl Planner {
    fn write(&mut self, path: &[(AggregateTarget, QualType)], value: Planned) {
        self.writes.push(Write {
            path: path.to_vec(),
            value,
        });
    }
}

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

    fn write(&mut self, target: AggregateTarget, entry: Entry) {
        if matches!(self.shape, Shape::Union(_)) {
            self.members.clear();
            self.members.push((target, entry));
            return;
        }
        let (start, end) = bounds(target);
        self.split_out(start, end);
        self.members.retain(|(existing, _)| {
            let (existing_start, existing_end) = bounds(*existing);
            !(start <= existing_start && existing_end <= end)
        });
        self.members.push((target, entry));
    }

    fn partitions(
        &mut self,
        target: AggregateTarget,
        fresh: &Builder,
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

    fn place(
        &mut self,
        path: &[(AggregateTarget, QualType)],
        entry: &Entry,
        open: bool,
        fresh: &impl Fn(QualType) -> Result<Builder, ResolveError>,
    ) -> Result<(), ResolveError> {
        let [(target, c), rest @ ..] = path else {
            return Err(ResolveError::Internal("empty initializer path"));
        };
        if rest.is_empty() && !open {
            self.write(*target, entry.clone());
            return Ok(());
        }
        if rest.is_empty() {
            self.partitions(*target, &fresh(*c)?)?;
            return Ok(());
        }
        for position in self.partitions(*target, &fresh(*c)?)? {
            let Entry::Sub(sub) = &mut self.members[position].1 else {
                return Err(ResolveError::Internal("initializer path through a leaf"));
            };
            sub.place(rest, entry, open, fresh)?;
        }
        Ok(())
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
    path: Path,
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

    fn child(&self, target: AggregateTarget, c: QualType) -> Path {
        let mut path = self.path.clone();
        path.push((target, c));
        path
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

    fn walk(&self, c: QualType, path: Path) -> Result<Walk, ResolveError> {
        Ok(Walk {
            c,
            path,
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

    pub(super) fn unaliased(&self, ty: &Type) -> Type {
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
            && let Some(literal) = string_literal(expr)
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
        let mut planner = Planner {
            writes: Vec::new(),
            check: false,
        };
        let mut walk = self.walk(c, Vec::new())?;
        self.walk_fill(&mut planner, &mut walk, items, &mut 0, true, false)
            .map_err(|error| error.error)?;
        Ok(walk.reach)
    }

    pub(super) fn record_initializer(
        &mut self,
        key: NodeId,
        c: QualType,
        source: InitializerSource<'_>,
    ) -> Result<(), ElementError> {
        let mut planner = Planner {
            writes: Vec::new(),
            check: true,
        };
        let result = match source {
            InitializerSource::Initializer(initializer) => {
                self.plan_initializer(&mut planner, &[], c, initializer)
            }
            InitializerSource::CompoundLiteral(items) => {
                self.plan_braced(&mut planner, &[], c, items)
            }
        };
        let plan = match &result {
            Ok(()) => Ok(Rc::new(InitializerPlan {
                c,
                writes: planner.writes,
            })),
            Err(error) => Err(error.error.clone()),
        };
        self.initializer_plans.insert(key, plan);
        result
    }

    fn plan_initializer(
        &mut self,
        planner: &mut Planner,
        path: &[(AggregateTarget, QualType)],
        c: QualType,
        initializer: &Initializer,
    ) -> Result<(), ElementError> {
        match initializer {
            Initializer::List(items) => self.plan_braced(planner, path, c, items),
            Initializer::Expr(expr) => {
                if let Some((target, items)) = self.array_compound_literal(c, expr) {
                    return self.plan_braced(planner, path, target, items);
                }
                let ty = self.ir_type(c);
                if matches!(self.shape(&ty)?, Shape::Array { .. })
                    && self.string_array(expr, &ty)?.is_some()
                {
                    planner.write(path, Planned::CodeUnits { expr: expr.id, c });
                    return Ok(());
                }
                self.plan_element(planner, path, expr, c)
            }
        }
    }

    fn array_compound_literal<'e>(
        &mut self,
        c: QualType,
        e: &'e Expr,
    ) -> Option<(QualType, &'e [InitializerItem])> {
        let ExprKind::CompoundLiteral { initializer, .. } = &e.value else {
            return None;
        };
        let literal = self.expression_type(e).ok()?;
        let literal = self.ctypes.unqualified(literal);
        let target = self.ctypes.unqualified(c);
        if !self.ctypes.is_array(target) || !self.ctypes.compatible(target, literal) {
            return None;
        }
        let complete = matches!(self.ctypes.element(target), Some((_, Extent::Fixed(_))));
        let target = if complete {
            c
        } else {
            literal.with(self.ctypes.quals(c))
        };
        Some((target, initializer))
    }

    fn plan_braced(
        &mut self,
        planner: &mut Planner,
        path: &[(AggregateTarget, QualType)],
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
            let parts = [
                self.complex_part(component, &real.value)?,
                self.complex_part(component, &imaginary.value)?,
            ];
            planner.write(path, Planned::Complex { c, parts });
            return Ok(());
        }
        if matches!(shape, Shape::Scalar) {
            return match items {
                [] => {
                    planner.write(path, Planned::Braced(c));
                    Ok(())
                }
                [first, ..] if first.designators.is_empty() => {
                    self.plan_initializer(planner, path, c, &first.value)
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
            planner.write(path, Planned::CodeUnits { expr: expr.id, c });
            return Ok(());
        }
        planner.write(path, Planned::Braced(c));
        let mut walk = self.walk(c, path.to_vec())?;
        self.walk_fill(planner, &mut walk, items, &mut 0, true, false)
    }

    fn complex_part(&mut self, c: QualType, value: &Initializer) -> Result<NodeId, ElementError> {
        match value {
            Initializer::Expr(expr) => {
                self.record_element(expr, c)?;
                Ok(expr.id)
            }
            Initializer::List(items) => match items.as_slice() {
                [first, ..] if first.designators.is_empty() => self.complex_part(c, &first.value),
                [] => Err(ResolveError::Unimplemented("empty braces for complex component").into()),
                _ => {
                    Err(ResolveError::Rejected("designator in initializer for scalar type").into())
                }
            },
        }
    }

    fn plan_element(
        &mut self,
        planner: &mut Planner,
        path: &[(AggregateTarget, QualType)],
        expr: &Expr,
        to: QualType,
    ) -> Result<(), ElementError> {
        if planner.check {
            self.record_element(expr, to)?;
        }
        planner.write(path, Planned::Element { expr: expr.id, to });
        Ok(())
    }

    fn record_element(&mut self, expr: &Expr, to: QualType) -> Result<(), ElementError> {
        let at = |error| ElementError {
            at: Some(expr.derive(())),
            error,
        };
        let from = self.operand_type(expr).map_err(at)?;
        self.record_conversion(expr, from, to, ConversionContext::Assign)
            .map_err(at)?;
        Ok(())
    }

    fn walk_fill(
        &mut self,
        planner: &mut Planner,
        walk: &mut Walk,
        items: &[InitializerItem],
        index: &mut usize,
        braced: bool,
        pending: bool,
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
                let designated = walk.path.clone();
                self.walk_place(planner, walk, designated, &steps, items, index)?;
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
            let path = walk.child(target, c);
            *index = self.walk_item(planner, &path, c, items, *index)?;
            pending = false;
            walk.advance(target);
        }
        Ok(())
    }

    fn walk_place(
        &mut self,
        planner: &mut Planner,
        walk: &mut Walk,
        mut designated: Path,
        steps: &[Step],
        items: &[InitializerItem],
        index: &mut usize,
    ) -> Result<(), ElementError> {
        let (target, _) = steps
            .first()
            .cloned()
            .ok_or(ResolveError::Internal("empty designator path"))?;
        let c = self.subobject_type(walk.c, target)?;
        designated.push((target, c));
        if steps.len() == 1 {
            *index = self.walk_item(planner, &designated, c, items, *index)?;
        } else {
            let mut sub = self.walk(c, walk.child(last_position(target), c))?;
            self.walk_place(planner, &mut sub, designated, &steps[1..], items, index)?;
            self.walk_fill(planner, &mut sub, items, index, false, false)?;
        }
        walk.advance(target);
        Ok(())
    }

    fn walk_item(
        &mut self,
        planner: &mut Planner,
        path: &[(AggregateTarget, QualType)],
        c: QualType,
        items: &[InitializerItem],
        index: usize,
    ) -> Result<usize, ElementError> {
        let expr = match &items[index].value {
            Initializer::List(inner) => {
                if planner.check {
                    self.plan_braced(planner, path, c, inner)?;
                }
                return Ok(index + 1);
            }
            Initializer::Expr(expr) => expr,
        };
        let ty = self.ir_type(c);
        let shape = self.shape(&ty)?;
        if matches!(shape, Shape::Array { .. }) && self.string_array(expr, &ty)?.is_some() {
            planner.write(path, Planned::CodeUnits { expr: expr.id, c });
            return Ok(index + 1);
        }
        let value = match self.operand_type(expr) {
            Ok(value) => Some(value),
            Err(error) if planner.check => {
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
            self.plan_element(planner, path, expr, c)?;
            return Ok(index + 1);
        }
        let mut sub = self.walk(c, path.to_vec())?;
        let reached = if sub.full() {
            planner.write(path, Planned::Open(c));
            index + 1
        } else {
            index
        };
        let mut next = index;
        self.walk_fill(planner, &mut sub, items, &mut next, false, true)?;
        Ok(reached.max(next))
    }

    fn string_array<'e>(
        &mut self,
        e: &'e Expr,
        ty: &Type,
    ) -> Result<Option<&'e StringLiteral>, ResolveError> {
        let Some(literal) = string_literal(e) else {
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

fn initializer_exprs<'e>(source: InitializerSource<'e>) -> HashMap<NodeId, &'e Expr> {
    fn collect<'e>(value: &'e Initializer, exprs: &mut HashMap<NodeId, &'e Expr>) {
        match value {
            Initializer::List(items) => {
                for item in items {
                    collect(&item.value, exprs);
                }
            }
            Initializer::Expr(expr) => {
                exprs.insert(expr.id, expr);
                if let ExprKind::CompoundLiteral { initializer, .. } = &expr.value {
                    for item in initializer {
                        collect(&item.value, exprs);
                    }
                }
            }
        }
    }
    let mut exprs = HashMap::new();
    match source {
        InitializerSource::Initializer(initializer) => collect(initializer, &mut exprs),
        InitializerSource::CompoundLiteral(items) => {
            for item in items {
                collect(&item.value, &mut exprs);
            }
        }
    }
    exprs
}

impl Lowerer {
    pub(super) fn initializer_value(
        &mut self,
        key: NodeId,
        c: QualType,
        source: InitializerSource<'_>,
        anchor: &Span<()>,
    ) -> Result<Value, ResolveError> {
        let plan = self.plan(key, c, source)?;
        let exprs = initializer_exprs(source);
        let mut root: Option<Entry> = None;
        for write in &plan.writes {
            let entry = self.planned_entry(&write.value, &exprs)?;
            match (&mut root, write.path.as_slice()) {
                (_, []) => root = Some(entry),
                (Some(Entry::Sub(builder)), path) => {
                    let open = matches!(write.value, Planned::Open(_));
                    builder.place(path, &entry, open, &|c| self.fresh_builder(c))?
                }
                _ => {
                    return Err(ResolveError::Internal(
                        "initializer path outside the object",
                    ));
                }
            }
        }
        match root {
            Some(Entry::Leaf(value)) => Ok(value),
            Some(Entry::Sub(builder)) => builder.finish(anchor),
            None => Err(ResolveError::Internal("empty initializer plan")),
        }
    }

    // the checker plans variably modified types with unbound extents, so lowering plans them again
    fn plan(
        &mut self,
        key: NodeId,
        c: QualType,
        source: InitializerSource<'_>,
    ) -> Result<Rc<InitializerPlan>, ResolveError> {
        let recorded = self
            .types
            .initializer_plans
            .get(&key)
            .ok_or(ResolveError::Internal(
                "initializer plan not recorded by the checker",
            ))?
            .clone()
            .map_err(ResolveError::checked)?;
        if !self.types.ctypes.has_unbound_extent(recorded.c) {
            return Ok(recorded);
        }
        self.types
            .record_initializer(key, c, source)
            .map_err(|error| error.error.checked())?;
        self.types
            .initializer_plans
            .get(&key)
            .cloned()
            .ok_or(ResolveError::Internal("initializer plan not recorded"))?
            .map_err(ResolveError::checked)
    }

    fn fresh_builder(&self, c: QualType) -> Result<Builder, ResolveError> {
        let ty = self.types.ir_type(c);
        Ok(Builder {
            shape: self.types.shape(&ty).map_err(ResolveError::checked)?,
            ty,
            members: Vec::new(),
        })
    }

    fn planned_entry(
        &mut self,
        planned: &Planned,
        exprs: &HashMap<NodeId, &Expr>,
    ) -> Result<Entry, ResolveError> {
        let expr = |id: &NodeId| {
            exprs.get(id).copied().ok_or(ResolveError::Internal(
                "initializer element outside the initializer",
            ))
        };
        Ok(match planned {
            Planned::Element { expr: id, to } => Entry::Leaf(self.element(expr(id)?, *to)?),
            Planned::CodeUnits { expr: id, c } => {
                Entry::Leaf(self.string_array_initializer(expr(id)?, *c)?)
            }
            Planned::Braced(c) | Planned::Open(c) => Entry::Sub(self.fresh_builder(*c)?),
            Planned::Complex { c, parts } => {
                let component = self.types.ctypes.arithmetic_component(*c);
                let mut members = Vec::new();
                for (index, id) in parts.iter().enumerate() {
                    members.push(AggregateMember {
                        target: AggregateTarget::Index(index as u64),
                        value: self.element(expr(id)?, component)?,
                    });
                }
                let node = members[0].value.node.clone();
                Entry::Leaf(Value {
                    ty: self.types.ir_type(*c),
                    node: node.derive(ValueKind::Aggregate {
                        members,
                        zero_fill: false,
                    }),
                })
            }
        })
    }

    fn element(&mut self, e: &Expr, to: QualType) -> Result<Value, ResolveError> {
        let value = self.expr(e)?;
        let to = self.types.ctypes.unqualified(to);
        Ok(self
            .convert_recorded(e, value, to, ConversionReason::Assign)?
            .value)
    }

    fn string_array_initializer(&mut self, e: &Expr, c: QualType) -> Result<Value, ResolveError> {
        let ty = self.types.ir_type(c);
        let Some(literal) = self
            .types
            .string_array(e, &ty)
            .map_err(ResolveError::checked)?
        else {
            return Err(ResolveError::Internal("string initializer is not a string"));
        };
        let Type::Array { element, length } = ty else {
            return Err(ResolveError::Internal("string initializer for non-array"));
        };
        let mut units = literal.execution_units(self.context.target.wchar_width);
        units.push(0);
        let length = length.unwrap_or(units.len() as u64);
        units.resize(length as usize, 0);
        let ty = Type::Array {
            element,
            length: Some(length),
        };
        Ok(self.value(e, ty, ValueKind::CodeUnits(units)))
    }
}

fn string_literal(e: &Expr) -> Option<&StringLiteral> {
    match &e.value {
        ExprKind::Paren(inner) => string_literal(inner),
        ExprKind::StringLiteral(literal) => Some(literal),
        _ => None,
    }
}
