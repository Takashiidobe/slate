use super::ctype::{CTypeKind, QualType};
use super::expression::Lowerer;
use super::numeric::ResolveError;
use super::operand::Operand;
use super::types::TypeResolver;
use crate::ast::{Designator, Expr, ExprKind, Initializer, InitializerItem, Span};
use crate::const_expr::{Encoding, Parser};
use crate::ir::*;

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

enum Entry {
    Leaf(Value),
    Sub(Builder),
}

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

fn sized(ty: &Type) -> Result<Type, ResolveError> {
    match ty {
        Type::Array { length: None, .. } => Err(ResolveError::Unsupported(
            "flexible array member initializer",
        )),
        _ => Ok(ty.clone()),
    }
}

impl Builder {
    fn full(&self) -> bool {
        match &self.shape {
            Shape::Struct(fields) => !fields.iter().skip(self.next as usize).any(initializable),
            Shape::Union(fields) => !self.members.is_empty() || !fields.iter().any(initializable),
            Shape::Array { length, .. } => length.is_some_and(|length| self.next >= length),
            Shape::Scalar => true,
        }
    }

    fn next_target(&self) -> Result<Step, ResolveError> {
        match &self.shape {
            Shape::Struct(fields) | Shape::Union(fields) => fields
                .iter()
                .enumerate()
                .skip(self.next as usize)
                .find(|(_, field)| initializable(field))
                .map(|(index, field)| (AggregateTarget::Field(index), field.ty.clone()))
                .ok_or(ResolveError::Unsupported("excess elements in initializer")),
            Shape::Array { element, .. } => {
                Ok((AggregateTarget::Index(self.next), sized(element)?))
            }
            Shape::Scalar => Err(ResolveError::Unsupported("initializer list for scalar")),
        }
    }

    fn advance(&mut self, target: AggregateTarget) {
        self.next = bounds(target).1 + 1;
    }

    fn make_room(&mut self, target: AggregateTarget) -> Result<(), ResolveError> {
        if matches!(self.shape, Shape::Union(_)) {
            if self
                .members
                .first()
                .is_some_and(|(existing, _)| *existing != target)
            {
                self.members.clear();
            }
            return Ok(());
        }
        let (start, end) = bounds(target);
        let overlapping = self.members.iter().any(|(existing, _)| {
            let (existing_start, existing_end) = bounds(*existing);
            *existing != target && existing_start <= end && start <= existing_end
        });
        if overlapping {
            return Err(ResolveError::Unsupported("overlapping designated range"));
        }
        Ok(())
    }

    fn insert(&mut self, target: AggregateTarget, entry: Entry) -> Result<(), ResolveError> {
        self.make_room(target)?;
        self.members.retain(|(existing, _)| *existing != target);
        self.members.push((target, entry));
        Ok(())
    }

    fn sub(
        &mut self,
        target: AggregateTarget,
        fresh: Builder,
    ) -> Result<&mut Builder, ResolveError> {
        self.make_room(target)?;
        let position = match self
            .members
            .iter()
            .position(|(existing, _)| *existing == target)
        {
            Some(position) => position,
            None => {
                self.members.push((target, Entry::Sub(fresh)));
                self.members.len() - 1
            }
        };
        match &mut self.members[position].1 {
            Entry::Sub(builder) => Ok(builder),
            Entry::Leaf(_) => Err(ResolveError::Unsupported(
                "designator into initialized scalar or copied aggregate",
            )),
        }
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
                        .map(|(target, _)| bounds(*target).1 + 1)
                        .ok_or(ResolveError::Unsupported(
                            "empty initializer for array of unknown length",
                        ))?,
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
            Shape::Scalar => return Err(ResolveError::Unsupported("initializer list for scalar")),
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

fn consume(cursor: &mut Option<&mut Cursor<'_>>) {
    if let Some(cursor) = cursor {
        cursor.index += 1;
    }
}

impl TypeResolver {
    pub(super) fn kind(&self, ty: &Type) -> Option<&TypeDefinitionKind> {
        match ty {
            Type::Defined(id) => self.definitions.get(id.0 as usize).map(|d| &d.kind),
            _ => None,
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
                    .ok_or(ResolveError::Unsupported(
                        "initializer for incomplete record",
                    ))?
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

    pub(super) fn inferred_array_length(
        &mut self,
        element: &Type,
        items: &[InitializerItem],
    ) -> Result<u64, ResolveError> {
        let (mut next, mut length, mut index) = (0, 0, 0);
        while index < items.len() {
            let item = &items[index];
            match item.designators.first() {
                None => {
                    index = self.consumed(element, items, index)?;
                    next += 1;
                }
                Some(Designator::Array(at)) => {
                    next = array_index(at, None)? + 1;
                    index += 1;
                }
                Some(Designator::ArrayRange { end, .. }) => {
                    next = array_index(end, None)? + 1;
                    index += 1;
                }
                Some(Designator::Field(_)) => {
                    return Err(ResolveError::Unsupported(
                        "designator does not match aggregate type",
                    ));
                }
            }
            length = length.max(next);
        }
        Ok(length)
    }

    fn consumed(
        &mut self,
        ty: &Type,
        items: &[InitializerItem],
        index: usize,
    ) -> Result<usize, ResolveError> {
        let Initializer::Expr(expr) = &items[index].value else {
            return Ok(index + 1);
        };
        let children: Vec<Type> = match self.shape(ty)? {
            Shape::Scalar => return Ok(index + 1),
            Shape::Array {
                element, length, ..
            } => {
                if matches!(expr.value, ExprKind::StringLiteral(_))
                    && matches!(element, Type::Numeric(NumericType::Integer { .. }))
                {
                    return Ok(index + 1);
                }
                let length = length.ok_or(ResolveError::Unsupported(
                    "flexible array member initializer",
                ))?;
                (0..length).map(|_| element.clone()).collect()
            }
            shape @ (Shape::Struct(_) | Shape::Union(_)) => {
                let (Shape::Struct(fields) | Shape::Union(fields)) = &shape else {
                    return Ok(index + 1);
                };
                let value = self.assertion_operand_type(expr)?;
                if self.unaliased(&self.ir_type(value)) == self.unaliased(ty) {
                    return Ok(index + 1);
                }
                let fields = fields.iter().filter(|field| initializable(field));
                let take = if matches!(shape, Shape::Union(_)) {
                    1
                } else {
                    usize::MAX
                };
                fields.take(take).map(|field| field.ty.clone()).collect()
            }
        };
        let mut index = index;
        for (position, child) in children.iter().enumerate() {
            if position > 0 && (index >= items.len() || !items[index].designators.is_empty()) {
                break;
            }
            index = self.consumed(child, items, index)?;
        }
        Ok(index)
    }
}

impl Lowerer {
    fn subobject_type(
        &self,
        c: QualType,
        target: AggregateTarget,
    ) -> Result<QualType, ResolveError> {
        match (self.types.ctypes.canonical_kind(c), target) {
            (CTypeKind::Record { id, .. }, AggregateTarget::Field(index)) => self
                .types
                .record_fields
                .get(id)
                .and_then(|fields| fields.get(index))
                .copied()
                .ok_or(ResolveError::Unsupported("missing initializer field type")),
            (CTypeKind::Vector { element, .. }, _) => Ok(*element),
            (_, AggregateTarget::Index(_) | AggregateTarget::Range { .. }) => self
                .types
                .ctypes
                .element(c)
                .map(|(element, _)| element)
                .ok_or(ResolveError::Unsupported(
                    "missing initializer element type",
                )),
            _ => Err(ResolveError::Unsupported("invalid initializer subobject")),
        }
    }

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

    fn builder(&self, c: QualType) -> Result<Builder, ResolveError> {
        let ty = self.types.ir_type(c);
        match self.types.shape(&ty)? {
            Shape::Scalar => Err(ResolveError::Unsupported(
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
            Initializer::Expr(expr) => self.init_expr(c, expr, None),
            Initializer::List(items) => self.braced(c, items),
        }
    }

    fn braced(&mut self, c: QualType, items: &[InitializerItem]) -> Result<Entry, ResolveError> {
        let ty = &self.types.ir_type(c);
        let shape = self.types.shape(ty)?;
        if let (Type::Complex(_component), [real, imaginary]) = (self.types.unaliased(ty), items)
            && real.designators.is_empty()
            && imaginary.designators.is_empty()
        {
            let component = self.types.ctypes.arithmetic_component(c);
            let mut members = Vec::new();
            for (index, item) in [real, imaginary].into_iter().enumerate() {
                let Entry::Leaf(value) = self.init_initializer(component, &item.value)? else {
                    return Err(ResolveError::Unsupported(
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
                [item] if item.designators.is_empty() => self.init_initializer(c, &item.value),
                _ => Err(ResolveError::Unsupported(
                    "scalar initializer list must hold exactly one element",
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
            && let Some((_, value)) = self.string_array_initializer(expr, ty)?
        {
            return Ok(Entry::Leaf(value));
        }
        let mut cursor = Cursor::new(items);
        Ok(Entry::Sub(self.fill(c, &mut cursor, true)?))
    }

    fn fill(
        &mut self,
        c: QualType,
        cursor: &mut Cursor<'_>,
        braced: bool,
    ) -> Result<Builder, ResolveError> {
        let mut builder = self.builder(c)?;
        let items = cursor.items;
        while cursor.index < items.len() {
            let item = &items[cursor.index];
            if !item.designators.is_empty() && cursor.pending.is_none() {
                if !braced {
                    break;
                }
                self.designated(&mut builder, &item.designators, cursor)?;
                continue;
            }
            if builder.full() {
                if braced {
                    return Err(ResolveError::Unsupported("excess elements in initializer"));
                }
                break;
            }
            let (target, _) = builder.next_target()?;
            let element = self.subobject_type(builder.c, target)?;
            let entry = self.init_subobject(element, cursor)?;
            builder.insert(target, entry)?;
            builder.advance(target);
        }
        Ok(builder)
    }

    fn init_subobject(
        &mut self,
        c: QualType,
        cursor: &mut Cursor<'_>,
    ) -> Result<Entry, ResolveError> {
        let items = cursor.items;
        match &items[cursor.index].value {
            Initializer::List(inner) => {
                cursor.index += 1;
                self.braced(c, inner)
            }
            Initializer::Expr(expr) => self.init_expr(c, expr, Some(cursor)),
        }
    }

    fn init_expr(
        &mut self,
        c: QualType,
        expr: &Expr,
        mut cursor: Option<&mut Cursor<'_>>,
    ) -> Result<Entry, ResolveError> {
        let ty = &self.types.ir_type(c);
        let shape = self.types.shape(ty)?;
        if matches!(shape, Shape::Array { .. })
            && let Some((_, value)) = self.string_array_initializer(expr, ty)?
        {
            consume(&mut cursor);
            return Ok(Entry::Leaf(value));
        }
        let value = match cursor
            .as_deref_mut()
            .and_then(|cursor| cursor.pending.take())
        {
            Some(value) => value,
            None => self.expr(expr)?,
        };
        let whole = match shape {
            Shape::Scalar => true,
            Shape::Struct(_) | Shape::Union(_) => {
                self.types.unaliased(&value.ty) == self.types.unaliased(ty)
            }
            Shape::Array { .. } => false,
        };
        if whole {
            consume(&mut cursor);
            let value = self.convert_expr(expr, value, c, ConversionReason::Assign)?;
            return Ok(Entry::Leaf(value.value));
        }
        let cursor = cursor.ok_or(ResolveError::Unsupported(
            "aggregate initialized without braces",
        ))?;
        cursor.pending = Some(value);
        Ok(Entry::Sub(self.fill(c, cursor, false)?))
    }

    fn designated(
        &mut self,
        builder: &mut Builder,
        designators: &[Designator],
        cursor: &mut Cursor<'_>,
    ) -> Result<(), ResolveError> {
        let (first, rest) = designators
            .split_first()
            .ok_or(ResolveError::Unsupported("empty designator list"))?;
        let steps = self.resolve(builder, first)?;
        self.apply(builder, &steps, rest, cursor)
    }

    fn apply(
        &mut self,
        builder: &mut Builder,
        steps: &[Step],
        rest: &[Designator],
        cursor: &mut Cursor<'_>,
    ) -> Result<(), ResolveError> {
        let Some(((target, _ty), tail)) = steps.split_first() else {
            return Err(ResolveError::Unsupported("empty designator path"));
        };
        if tail.is_empty() && rest.is_empty() {
            let c = self.subobject_type(builder.c, *target)?;
            let entry = self.init_subobject(c, cursor)?;
            builder.insert(*target, entry)?;
            builder.advance(*target);
            return Ok(());
        }
        let c = self.subobject_type(builder.c, *target)?;
        let fresh = self.builder(c)?;
        let sub = builder.sub(*target, fresh)?;
        if tail.is_empty() {
            self.designated(sub, rest, cursor)?;
        } else {
            self.apply(sub, tail, rest, cursor)?;
        }
        builder.advance(*target);
        Ok(())
    }

    fn resolve(
        &self,
        builder: &Builder,
        designator: &Designator,
    ) -> Result<Vec<Step>, ResolveError> {
        match (&builder.shape, designator) {
            (Shape::Struct(fields) | Shape::Union(fields), Designator::Field(name)) => self
                .field_path(fields, &name.value)
                .ok_or(ResolveError::Unsupported("unknown field designator")),
            (
                Shape::Array {
                    element, length, ..
                },
                Designator::Array(index),
            ) => {
                let index = array_index(index, *length)?;
                Ok(vec![(AggregateTarget::Index(index), sized(element)?)])
            }
            (
                Shape::Array {
                    element, length, ..
                },
                Designator::ArrayRange { start, end },
            ) => {
                let start = array_index(start, *length)?;
                let end = array_index(end, *length)?;
                if end < start {
                    return Err(ResolveError::Unsupported("empty designated range"));
                }
                let target = if start == end {
                    AggregateTarget::Index(start)
                } else {
                    AggregateTarget::Range { start, end }
                };
                Ok(vec![(target, sized(element)?)])
            }
            _ => Err(ResolveError::Unsupported(
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
                && let Ok(Shape::Struct(inner) | Shape::Union(inner)) = self.types.shape(&field.ty)
                && let Some(path) = self.field_path(&inner, name)
            {
                return Some([vec![step], path].concat());
            }
        }
        None
    }

    pub(super) fn string_array_initializer(
        &mut self,
        e: &Expr,
        ty: &Type,
    ) -> Result<Option<(Type, Value)>, ResolveError> {
        let ExprKind::StringLiteral(literal) = &e.value else {
            return Ok(None);
        };
        let Type::Array { element, length } = ty else {
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
        let literal_c = self.types.string_type(literal);
        let literal_ty = self.types.ir_type(literal_c);
        let Type::Array {
            element: literal_element,
            ..
        } = &literal_ty
        else {
            return Err(ResolveError::Unsupported("string literal type"));
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
            return Err(ResolveError::Unsupported(
                "string literal initializer for incompatible array element",
            ));
        }
        let mut units = literal.execution_units(self.context.target.wchar_width);
        units.push(0);
        let length = length.unwrap_or(units.len() as u64);
        units.resize(length as usize, 0);
        let ty = Type::Array {
            element: element.clone(),
            length: Some(length),
        };
        let value = self.value(e, ty.clone(), ValueKind::CodeUnits(units));
        Ok(Some((ty, value)))
    }
}

fn array_index(expr: &Expr, length: Option<u64>) -> Result<u64, ResolveError> {
    let index = Parser::evaluate_ast(expr)
        .ok()
        .and_then(|index| u64::try_from(index).ok())
        .ok_or(ResolveError::Unsupported(
            "non-constant or negative array designator",
        ))?;
    if length.is_some_and(|length| index >= length) {
        return Err(ResolveError::Unsupported("array designator out of range"));
    }
    Ok(index)
}
