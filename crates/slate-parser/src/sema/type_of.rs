use super::expression::Lowerer;
use super::numeric::ResolveError;
use super::types::{CTypeMetadata, ResolvedType, TypeResolver};
use crate::ast::{
    DeclarationSpecifiers, Declarator, Expr, ExprKind, FieldItemKind, TagBody, TagSpecifier,
    TypeName, TypeOfOperand, TypeSpecifier,
};
use crate::const_expr::{BinaryOp, UnaryOp};
use crate::ir::{Place, PlaceKind, Type, TypeDefinitionKind, TypeId};

impl Lowerer {
    pub(super) fn resolve_type(
        &mut self,
        specifiers: &DeclarationSpecifiers,
        declarator: &Declarator,
    ) -> Result<ResolvedType, ResolveError> {
        self.prepare_typeof(specifiers, declarator)?;
        self.types.resolve(specifiers, declarator)
    }

    pub(super) fn resolve_parameter_type(
        &mut self,
        specifiers: &DeclarationSpecifiers,
        declarator: &Declarator,
    ) -> Result<ResolvedType, ResolveError> {
        self.prepare_typeof(specifiers, declarator)?;
        self.types.resolve_parameter(specifiers, declarator)
    }

    pub(super) fn resolve_type_name(
        &mut self,
        ty: &TypeName,
    ) -> Result<ResolvedType, ResolveError> {
        self.resolve_type(&ty.specifiers, &ty.declarator)
    }

    pub(super) fn prepare_typeof(
        &mut self,
        specifiers: &DeclarationSpecifiers,
        declarator: &Declarator,
    ) -> Result<(), ResolveError> {
        self.prepare_specifier(&specifiers.ty)?;
        self.prepare_declarator(declarator)
    }

    fn prepare_specifier(&mut self, specifier: &TypeSpecifier) -> Result<(), ResolveError> {
        match specifier {
            TypeSpecifier::TypeOf(TypeOfOperand::Expression(expr))
            | TypeSpecifier::TypeOfUnqual(TypeOfOperand::Expression(expr)) => {
                let resolved = self.typeof_operand(expr)?;
                self.types.typeof_operands.insert(expr.id, resolved);
                Ok(())
            }
            TypeSpecifier::TypeOf(TypeOfOperand::Type(ty))
            | TypeSpecifier::TypeOfUnqual(TypeOfOperand::Type(ty))
            | TypeSpecifier::Atomic(ty)
            | TypeSpecifier::Tag(TagSpecifier::Reference {
                fixed_type: Some(ty),
                ..
            }) => self.prepare_typeof(&ty.specifiers, &ty.declarator),
            TypeSpecifier::Complex(inner) | TypeSpecifier::Imaginary(inner) => {
                self.prepare_specifier(inner)
            }
            TypeSpecifier::Vector(vector) => self.prepare_specifier(&vector.element),
            TypeSpecifier::Tag(TagSpecifier::Definition(id)) => {
                let Some(tag) = self.types.tag_definition(*id) else {
                    return Ok(());
                };
                match &tag.body {
                    TagBody::Record(items) => {
                        for item in items {
                            let FieldItemKind::Field(declaration) = &item.value else {
                                continue;
                            };
                            self.prepare_specifier(&declaration.specifiers.ty)?;
                            for declarator in &declaration.declarators {
                                self.prepare_declarator(&declarator.declarator)?;
                            }
                        }
                        Ok(())
                    }
                    TagBody::Enum {
                        fixed_type: Some(ty),
                        ..
                    } => self.prepare_typeof(&ty.specifiers, &ty.declarator),
                    TagBody::Enum { .. } => Ok(()),
                }
            }
            TypeSpecifier::Void
            | TypeSpecifier::Bool
            | TypeSpecifier::Integer(_)
            | TypeSpecifier::Floating(_)
            | TypeSpecifier::FixedPoint(_)
            | TypeSpecifier::TargetBuiltin(_)
            | TypeSpecifier::Named(_)
            | TypeSpecifier::Tag(TagSpecifier::Reference { .. }) => Ok(()),
        }
    }

    fn prepare_declarator(&mut self, declarator: &Declarator) -> Result<(), ResolveError> {
        match declarator {
            Declarator::Abstract | Declarator::Name(_) => Ok(()),
            Declarator::Grouped(inner)
            | Declarator::Attributed { inner, .. }
            | Declarator::Pointer { inner, .. }
            | Declarator::Array { inner, .. } => self.prepare_declarator(inner),
            Declarator::Function { inner, parameters } => {
                for parameter in parameters.parameters() {
                    self.prepare_typeof(&parameter.specifiers, &parameter.declarator)?;
                }
                self.prepare_declarator(inner)
            }
        }
    }

    fn typeof_operand(&mut self, e: &Expr) -> Result<ResolvedType, ResolveError> {
        let next_id = self.next_id;
        let globals = self.module.globals.len();
        let resolved = self.operand_type(e);
        self.next_id = next_id;
        self.module.globals.truncate(globals);
        self.types.bindings.retain(|id, _| id.0 < next_id);
        self.types.access.retain(|id, _| id.0 < next_id);
        self.c_types.retain(|id, _| id.0 < next_id);
        resolved
    }

    fn operand_type(&mut self, e: &Expr) -> Result<ResolvedType, ResolveError> {
        match &e.value {
            ExprKind::Paren(inner) => return self.operand_type(inner),
            ExprKind::Generic {
                controlling,
                associations,
            } => {
                let selected = self.generic_selected(controlling, associations)?;
                return self.operand_type(selected);
            }
            ExprKind::StringLiteral(literal) => {
                let ty = super::types::string_literal_type(
                    literal,
                    &self.context.target,
                    self.types.features,
                );
                return Ok(ResolvedType {
                    c: self.types.c_type(Some(&ty))?,
                    ty: Some(ty),
                });
            }
            _ => {}
        }
        if let Ok(place) = self.place(e) {
            if matches!(place.kind, PlaceKind::Field { bits: Some(_), .. }) {
                return Err(ResolveError::Invalid("typeof applied to a bit-field"));
            }
            let c = self.place_c(e, &place)?;
            return Ok(ResolvedType {
                ty: Some(place.ty),
                c,
            });
        }
        let ty = self.value_type(e)?;
        let c = self.value_c(e)?;
        Ok(ResolvedType { ty, c })
    }

    fn value_type(&mut self, e: &Expr) -> Result<Option<Type>, ResolveError> {
        let ty = self.expr(e)?.ty;
        Ok((ty != Type::Void).then_some(ty))
    }

    fn place_c(&mut self, e: &Expr, place: &Place) -> Result<CTypeMetadata, ResolveError> {
        let c = match &e.value {
            ExprKind::Paren(inner) => return self.place_c(inner, place),
            ExprKind::Generic {
                controlling,
                associations,
            } => {
                let selected = self.generic_selected(controlling, associations)?;
                return self.place_c(selected, place);
            }
            ExprKind::Identifier(_) => self
                .reference(e)
                .ok()
                .and_then(|id| self.c_types.get(&id).cloned()),
            ExprKind::CompoundLiteral { ty, .. } => {
                Some(with_length(self.resolve_type_name(ty)?.c, &place.ty))
            }
            ExprKind::Unary {
                op: UnaryOp::Deref,
                operand,
            } => self.value_c(operand)?.derived_from.map(|c| *c),
            ExprKind::Index { base, index } => {
                let pointer = if matches!(self.value_type(base)?, Some(Type::Pointer { .. })) {
                    base
                } else {
                    index
                };
                self.value_c(pointer)?.derived_from.map(|c| *c)
            }
            ExprKind::Member { base, arrow, .. } => self.member_c(base, *arrow, place)?,
            _ => None,
        };
        match c {
            Some(c) => Ok(c),
            None => self.types.c_type(Some(&place.ty)),
        }
    }

    fn member_c(
        &mut self,
        base: &Expr,
        arrow: bool,
        place: &Place,
    ) -> Result<Option<CTypeMetadata>, ResolveError> {
        let PlaceKind::Field {
            base: record,
            index,
            ..
        } = &place.kind
        else {
            return Ok(None);
        };
        let Some(field) = self
            .record_id(&record.ty)
            .and_then(|id| self.types.field_c.get(&id))
            .and_then(|fields| fields.get(*index))
            .cloned()
        else {
            return Ok(None);
        };
        let object = if arrow {
            self.value_c(base)?.derived_from.map(|c| *c)
        } else {
            Some(self.place_c(base, record)?)
        };
        let qualifiers = object.map(|c| c.qualifiers).unwrap_or_default();
        Ok(Some(field.qualified(qualifiers, Some(&place.ty))))
    }

    fn record_id(&self, ty: &Type) -> Option<TypeId> {
        let Type::Defined(id) = ty else {
            return None;
        };
        match self.kind(ty)? {
            TypeDefinitionKind::Record { .. } => Some(*id),
            TypeDefinitionKind::Alias(inner) => self.record_id(inner),
            TypeDefinitionKind::Enum { .. } => None,
        }
    }

    fn value_c(&mut self, e: &Expr) -> Result<CTypeMetadata, ResolveError> {
        let ty = self.value_type(e)?;
        let c = match &e.value {
            ExprKind::Paren(inner) => return self.value_c(inner),
            ExprKind::Generic {
                controlling,
                associations,
            } => {
                let selected = self.generic_selected(controlling, associations)?;
                return self.value_c(selected);
            }
            ExprKind::Comma { right, .. } => return self.value_c(right),
            ExprKind::Cast { ty: name, .. } => {
                Some(self.resolve_type_name(name)?.c.unqualified(ty.as_ref()))
            }
            ExprKind::Assign { target, .. }
            | ExprKind::Postfix {
                operand: target, ..
            }
            | ExprKind::Unary {
                op: UnaryOp::PreIncrement | UnaryOp::PreDecrement,
                operand: target,
            } => {
                let place = self.place(target)?;
                Some(self.place_c(target, &place)?.unqualified(Some(&place.ty)))
            }
            ExprKind::Unary {
                op: UnaryOp::AddrOf,
                operand,
            } => {
                let place = self.place(operand)?;
                let mut resolved = ResolvedType {
                    c: self.place_c(operand, &place)?,
                    ty: Some(place.ty),
                };
                TypeResolver::apply_pointer(Default::default(), &mut resolved);
                Some(resolved.c)
            }
            ExprKind::Unary {
                op: UnaryOp::Plus | UnaryOp::Minus | UnaryOp::BitNot,
                operand,
            } => self.matching_c(ty.as_ref(), &[operand])?,
            ExprKind::Binary {
                op:
                    BinaryOp::Less
                    | BinaryOp::LessEqual
                    | BinaryOp::Greater
                    | BinaryOp::GreaterEqual
                    | BinaryOp::Equal
                    | BinaryOp::NotEqual
                    | BinaryOp::And
                    | BinaryOp::Or,
                ..
            } => None,
            ExprKind::Binary {
                op: BinaryOp::ShiftLeft | BinaryOp::ShiftRight,
                left,
                ..
            } => self.matching_c(ty.as_ref(), &[left])?,
            ExprKind::Binary { left, right, .. } => self.matching_c(ty.as_ref(), &[left, right])?,
            ExprKind::Conditional {
                condition,
                then_value,
                else_value,
            } => self.matching_c(
                ty.as_ref(),
                &[then_value.as_ref().unwrap_or(condition), else_value],
            )?,
            ExprKind::Call { callee, .. } => self
                .value_c(callee)?
                .derived_from
                .and_then(|function| function.derived_from)
                .map(|returned| returned.unqualified(ty.as_ref())),
            _ => match self.place(e) {
                Ok(place) => {
                    let c = self.place_c(e, &place)?;
                    Some(converted(c, place.ty))
                }
                Err(_) => None,
            },
        };
        match c {
            Some(c) => Ok(c),
            None => self.types.c_type(ty.as_ref()),
        }
    }

    fn matching_c(
        &mut self,
        ty: Option<&Type>,
        operands: &[&Expr],
    ) -> Result<Option<CTypeMetadata>, ResolveError> {
        for operand in operands {
            if self.value_type(operand)?.as_ref() == ty {
                return self.value_c(operand).map(Some);
            }
        }
        Ok(None)
    }
}

fn converted(c: CTypeMetadata, ty: Type) -> CTypeMetadata {
    let mut resolved = match ty {
        Type::Array { element, .. } | Type::VariableArray { element, .. } => ResolvedType {
            c: c.derived_from
                .clone()
                .map_or_else(|| c.clone(), |element_c| *element_c),
            ty: Some(*element),
        },
        function @ Type::Function { .. } => ResolvedType {
            c,
            ty: Some(function),
        },
        ty => return c.unqualified(Some(&ty)),
    };
    TypeResolver::apply_pointer(Default::default(), &mut resolved);
    resolved.c
}

pub(super) fn with_length(mut c: CTypeMetadata, ty: &Type) -> CTypeMetadata {
    if let Type::Array {
        length: Some(length),
        ..
    } = ty
    {
        let extent = format!("[{length}]");
        c.spelling = c.spelling.replacen("[]", &extent, 1);
        c.canonical = c.canonical.replacen("[]", &extent, 1);
    }
    c
}
