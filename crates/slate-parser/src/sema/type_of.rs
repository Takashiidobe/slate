use super::ctype::{CTypeKind, Extent, QualType, Qualifiers};
use super::expression::Lowerer;
use super::numeric::ResolveError;
use super::types::TypeResolver;
use crate::ast::{
    ArraySize, Attribute, DeclarationSpecifiers, Declarator, Expr, ExprKind, FieldItemKind,
    Initializer, NodeId, Span, StorageClass, TagBody, TagSpecifier, TypeName, TypeOfOperand,
    TypeSpecifier,
};
use crate::compiler_args::CompilerFlavor;
use crate::ir::BindingId;
use crate::ir::{Type, Value};
use crate::visit::{self, Visitor};
use std::collections::HashMap;

impl Lowerer {
    pub(super) fn resolve_type(
        &mut self,
        specifiers: &DeclarationSpecifiers,
        declarator: &Declarator,
    ) -> Result<QualType, ResolveError> {
        self.prepare_typeof(specifiers, declarator)?;
        self.types.resolve(specifiers, declarator)
    }

    pub(super) fn resolve_declarator_type(
        &mut self,
        specifiers: &DeclarationSpecifiers,
        declarator: &Declarator,
        attributes: &[Span<Attribute>],
    ) -> Result<QualType, ResolveError> {
        self.prepare_typeof(specifiers, declarator)?;
        self.types
            .resolve_declarator(specifiers, declarator, attributes)
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
        self.prepare_typeof(&ty.specifiers, &ty.declarator)?;
        self.types.resolve_type_name(ty)
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
            TypeSpecifier::Mode(mode) => self.prepare_specifier(&mode.base),
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
            | TypeSpecifier::Inferred
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

    pub(super) fn typeof_evaluations(
        &mut self,
        specifier: &TypeSpecifier,
        out: &mut Vec<(BindingId, Value)>,
    ) -> Result<(), ResolveError> {
        match specifier {
            TypeSpecifier::TypeOf(TypeOfOperand::Expression(expr))
            | TypeSpecifier::TypeOfUnqual(TypeOfOperand::Expression(expr)) => {
                let typed = self.types.typed(expr)?;
                if !self.types.ctypes.is_variably_modified(typed.c) {
                    return Ok(());
                }
                let value = self.expr(expr)?;
                if !super::effects::has_effects(&value.value) {
                    return Ok(());
                }
                let id = self.fresh();
                self.types.entities.declare(id, value.c, false);
                out.push((id, value.value));
                Ok(())
            }
            TypeSpecifier::TypeOf(TypeOfOperand::Type(ty))
            | TypeSpecifier::TypeOfUnqual(TypeOfOperand::Type(ty)) => {
                self.typeof_evaluations(&ty.specifiers.ty, out)?;
                self.extents(&ty.declarator, out)
            }
            _ => Ok(()),
        }
    }

    fn reserve_extents(&mut self, e: &Expr) {
        let unbound = self
            .types
            .typed(e)
            .is_ok_and(|typed| self.types.ctypes.has_unbound_extent(typed.c));
        if !self.in_function || !unbound {
            return;
        }
        let mut sizes = VariableSizes {
            types: &mut self.types,
            nodes: Vec::new(),
        };
        let Ok(()) = sizes.visit_expr(e);
        for node in sizes.nodes {
            let id = self.fresh();
            self.types.extents.insert(node, id);
            self.reserved_extents.insert(node, id);
        }
    }

    pub(super) fn infer_type(
        &mut self,
        specifiers: &DeclarationSpecifiers,
        declarator: &Declarator,
        initializer: Option<&Initializer>,
        binding: BindingId,
    ) -> Result<(QualType, QualType), ResolveError> {
        let expr = self
            .types
            .deduced_initializer(specifiers, declarator, initializer, binding)
            .map_err(ResolveError::checked)?;
        self.reserve_extents(expr);
        let value = self.types.typed(expr)?.c;
        self.types
            .inferred_base(declarator, value)
            .map_err(ResolveError::checked)
    }

    pub(super) fn typeof_operand(&mut self, e: &Expr) -> Result<QualType, ResolveError> {
        self.types
            .typeof_expression(e)
            .map_err(ResolveError::checked)
    }

    pub(super) fn operand_type(&mut self, e: &Expr) -> Result<(QualType, bool), ResolveError> {
        let typed = self.types.typed(e)?;
        Ok((typed.c, typed.bits.is_some()))
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

impl TypeResolver {
    pub(super) fn deduced_initializer<'e>(
        &mut self,
        specifiers: &DeclarationSpecifiers,
        declarator: &Declarator,
        initializer: Option<&'e Initializer>,
        binding: BindingId,
    ) -> Result<&'e Expr, ResolveError> {
        if specifiers.storage == StorageClass::Typedef {
            return Err(ResolveError::Rejected("'auto' not allowed in typedef"));
        }
        let expr = match initializer {
            Some(Initializer::Expr(expr)) => expr,
            Some(Initializer::List(_)) => {
                return Err(ResolveError::Rejected(
                    "cannot use 'auto' with an initializer list",
                ));
            }
            None => {
                return Err(ResolveError::Rejected(
                    "declaration with deduced type requires an initializer",
                ));
            }
        };
        if self.compiler_flavor() == CompilerFlavor::Gcc && !plain_identifier(declarator) {
            return Err(ResolveError::Rejected(
                "'auto' requires a plain identifier as declarator",
            ));
        }
        let mut own = OwnReference {
            references: &self.references,
            binding,
        };
        if own.visit_expr(expr).is_err() {
            return Err(ResolveError::Rejected(
                "variable declared with deduced type cannot appear in its own initializer",
            ));
        }
        if self.typed(expr)?.bits.is_some() {
            return Err(if self.compiler_flavor() == CompilerFlavor::Gcc {
                ResolveError::Unimplemented("deduced type of a bit-field initializer")
            } else {
                ResolveError::Rejected("cannot use a bit-field as a deduced-type initializer")
            });
        }
        Ok(expr)
    }

    pub(super) fn check_inferred(
        &self,
        declared: QualType,
        value: QualType,
    ) -> Result<(), ResolveError> {
        let ctypes = &self.ctypes;
        let declared = ctypes.canonical(declared).local_unqualified();
        let value = ctypes.canonical(value).local_unqualified();
        let added_pointee_quals = match (ctypes.pointee(declared), ctypes.pointee(value)) {
            (Some(to), Some(from)) => {
                let (to, from) = (ctypes.canonical(to), ctypes.canonical(from));
                to.local_unqualified() == from.local_unqualified()
                    && from.quals.without(to.quals).is_empty()
            }
            _ => false,
        };
        if declared == value || added_pointee_quals {
            Ok(())
        } else {
            Err(ResolveError::Rejected(
                "initializer does not match the deduced declarator",
            ))
        }
    }

    pub(super) fn inferred_base(
        &mut self,
        declarator: &Declarator,
        value: QualType,
    ) -> Result<(QualType, QualType), ResolveError> {
        let atomic = self.compiler_flavor() != CompilerFlavor::Gcc
            && self.ctypes.element(value).is_none()
            && self.ctypes.quals(value).is_atomic;
        let converted = self.ctypes.lvalue_conversion(value);
        let value = if atomic {
            converted.with(Qualifiers::ATOMIC)
        } else {
            converted
        };
        Ok((self.pattern_base(declarator, value)?, value))
    }

    fn pattern_base(
        &self,
        declarator: &Declarator,
        target: QualType,
    ) -> Result<QualType, ResolveError> {
        let mismatch = ResolveError::Rejected("initializer does not match the deduced declarator");
        match declarator {
            Declarator::Name(_) | Declarator::Abstract => Ok(target),
            Declarator::Grouped(inner) | Declarator::Attributed { inner, .. } => {
                self.pattern_base(inner, target)
            }
            Declarator::Pointer { inner, .. } => {
                let outer = self.pattern_base(inner, target)?;
                self.ctypes.pointee(outer).ok_or(mismatch)
            }
            Declarator::Array { inner, .. } => {
                let outer = self.pattern_base(inner, target)?;
                self.ctypes
                    .element(outer)
                    .map(|(element, _)| element)
                    .ok_or(mismatch)
            }
            Declarator::Function { inner, .. } => {
                let outer = self.pattern_base(inner, target)?;
                self.ctypes
                    .function_parts(outer)
                    .map(|(ret, ..)| ret)
                    .ok_or(mismatch)
            }
        }
    }
}

fn plain_identifier(declarator: &Declarator) -> bool {
    match declarator {
        Declarator::Name(_) => true,
        Declarator::Grouped(inner) | Declarator::Attributed { inner, .. } => {
            plain_identifier(inner)
        }
        _ => false,
    }
}

struct VariableSizes<'a> {
    types: &'a mut TypeResolver,
    nodes: Vec<NodeId>,
}

impl Visitor for VariableSizes<'_> {
    type Error = std::convert::Infallible;

    fn visit_declarator(&mut self, declarator: &Declarator) -> Result<(), Self::Error> {
        match declarator {
            Declarator::Array { inner, size, .. } => {
                self.visit_declarator(inner)?;
                if let ArraySize::Expression(expr) = size {
                    self.visit_expr(expr)?;
                    if !self.types.extents.contains_key(&expr.id)
                        && self.types.constant_integer(expr).is_err()
                    {
                        self.nodes.push(expr.id);
                    }
                }
                Ok(())
            }
            Declarator::Function { inner, .. } => self.visit_declarator(inner),
            _ => visit::walk_declarator(self, declarator),
        }
    }
}

struct OwnReference<'a> {
    references: &'a HashMap<NodeId, BindingId>,
    binding: BindingId,
}

impl Visitor for OwnReference<'_> {
    type Error = ();

    fn visit_expr(&mut self, expr: &Expr) -> Result<(), ()> {
        if matches!(expr.value, ExprKind::Identifier(_))
            && self.references.get(&expr.id) == Some(&self.binding)
        {
            return Err(());
        }
        visit::walk_expr(self, expr)
    }
}
