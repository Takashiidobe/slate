use super::ctype::{CTypeKind, Extent, QualType};
use super::expression::Lowerer;
use super::numeric::ResolveError;
use crate::ast::{
    DeclarationSpecifiers, Declarator, Expr, ExprKind, FieldItemKind, TagBody, TagSpecifier,
    TypeName, TypeOfOperand, TypeSpecifier,
};
use crate::ir::{PlaceKind, Type};

impl Lowerer {
    pub(super) fn resolve_type(
        &mut self,
        specifiers: &DeclarationSpecifiers,
        declarator: &Declarator,
    ) -> Result<QualType, ResolveError> {
        self.prepare_typeof(specifiers, declarator)?;
        self.types.resolve(specifiers, declarator)
    }

    pub(super) fn resolve_parameter_type(
        &mut self,
        specifiers: &DeclarationSpecifiers,
        declarator: &Declarator,
    ) -> Result<QualType, ResolveError> {
        self.prepare_typeof(specifiers, declarator)?;
        self.types.resolve_parameter(specifiers, declarator)
    }

    pub(super) fn resolve_type_name(&mut self, ty: &TypeName) -> Result<QualType, ResolveError> {
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

    fn typeof_operand(&mut self, e: &Expr) -> Result<QualType, ResolveError> {
        let next_id = self.next_id;
        let globals = self.module.globals.len();
        let resolved = self.operand_type(e);
        self.next_id = next_id;
        self.module.globals.truncate(globals);
        self.types.entities.discard_after(next_id);
        resolved
    }

    fn operand_type(&mut self, e: &Expr) -> Result<QualType, ResolveError> {
        match &e.value {
            ExprKind::Paren(inner) => return self.operand_type(inner),
            ExprKind::Generic {
                controlling,
                associations,
            } => {
                let selected = self.generic_selected(controlling, associations)?;
                return self.operand_type(selected);
            }
            ExprKind::StringLiteral(literal) => return Ok(self.types.string_type(literal)),
            _ => {}
        }
        if let Ok(place) = self.place(e) {
            if matches!(place.kind, PlaceKind::Field { bits: Some(_), .. }) {
                return Err(ResolveError::Invalid("typeof applied to a bit-field"));
            }
            return Ok(place.c);
        }
        Ok(self.expr(e)?.c)
    }

    pub(super) fn with_length(&mut self, resolved: QualType, ty: &Type) -> QualType {
        let Type::Array {
            length: Some(length),
            ..
        } = ty
        else {
            return resolved;
        };
        match self.types.ctypes.element(resolved) {
            Some((element, Extent::Incomplete)) => self.types.ctypes.qual(CTypeKind::Array {
                element,
                extent: Extent::Fixed(*length),
            }),
            _ => resolved,
        }
    }
}
