use super::names::ItemResolution;
use crate::ast::*;
use crate::const_expr::{AssignOp, BinaryOp, UnaryOp};
use crate::ir::{
    BindingId, Linkage, NameResolution, Number, NumericType, SymbolAttributes, Type, Value,
    ValueKind,
};
use crate::visit::{self, Visitor};
use num_bigint::{BigInt, Sign};

use super::ctype::QualType;
use super::ctype::convert::ConversionContext;
use super::entity::ObjectRequest;
use super::numeric::ResolveError;
use super::types::TypeResolver;
use super::validate::{SemaError, error};

pub(super) fn validate(
    unit: &TranslationUnit,
    types: &mut TypeResolver,
    names: &NameResolution,
    items: &[ItemResolution],
) -> Vec<SemaError> {
    let mut checker = Checker {
        unit,
        types,
        errors: Vec::new(),
        return_type: None,
    };
    for (declaration, item) in unit.decls.iter().zip(items) {
        for id in item.declared.clone().map(BindingId) {
            if names.implicit_functions.contains(&id) {
                let ty = checker.types.ctypes.implicit_function();
                checker.types.entities.declare(id, ty, false);
                checker.types.entities.merge_declaration(
                    id,
                    Linkage::External,
                    None,
                    false,
                    SymbolAttributes::default(),
                );
            }
        }
        match &declaration.value {
            DeclKind::StaticAssert(assertion) => checker.assertion(assertion),
            DeclKind::Declaration(declaration) => checker.declaration(declaration, true),
            DeclKind::Function(function) => {
                checker.function(declaration.id, Some(declaration.derive(())), function)
            }
            _ => {}
        }
        let diagnostics = std::mem::take(&mut checker.types.diagnostics);
        if !diagnostics.is_empty() {
            checker
                .types
                .item_diagnostics
                .insert(declaration.id, diagnostics);
        }
    }
    checker.errors
}

struct Checker<'a> {
    unit: &'a TranslationUnit,
    types: &'a mut TypeResolver,
    errors: Vec<SemaError>,
    return_type: Option<QualType>,
}

impl Checker<'_> {
    fn assertion(&mut self, assertion: &StaticAssert) {
        let condition = &assertion.condition;
        match ice_shape(self.types, condition) {
            Shape::Constant => {}
            Shape::Skip => return,
            Shape::NotConstant(call) => {
                self.errors.push(error(
                    call.provenance,
                    call.expansion,
                    "static assertion requires an integer constant expression: call to a function that cannot be constant folded",
                ));
                return;
            }
        }
        let result = self
            .types
            .constant_value(condition)
            .and_then(|value| match value.ty {
                Type::Bool | Type::Numeric(NumericType::Integer { .. }) => {
                    super::fold::integer_constant(&value, self.unit.dialect.flavor()).ok_or(
                        ResolveError::Rejected("nonconstant or undefined integer expression"),
                    )
                }
                _ => Err(ResolveError::Rejected("non-integer constant expression")),
            })
            .map_err(|error| error.to_string());
        let message = match result {
            Ok(value) if value.sign() != Sign::NoSign => return,
            Ok(_) => assertion.message.as_ref().map_or_else(
                || "static assertion failed".to_owned(),
                |message| format!("static assertion failed: {message}"),
            ),
            Err(reason) => {
                format!("static assertion requires an integer constant expression: {reason}")
            }
        };
        self.errors
            .push(error(condition.provenance, condition.expansion, message));
    }

    fn declare_object(
        &mut self,
        node: NodeId,
        ty: QualType,
        alignment: Option<u64>,
        linkage: Option<Linkage>,
    ) {
        let Some(&id) = self.types.declarations.get(&node) else {
            return;
        };
        let ty = match self.types.entities.ty(&id) {
            Some(previous) => self
                .types
                .ctypes
                .composite(previous, ty)
                .unwrap_or(previous),
            None => ty,
        };
        self.types.entities.declare(id, ty, false);
        if let Some(linkage) = linkage {
            self.types.entities.merge_declaration(
                id,
                linkage,
                None,
                false,
                SymbolAttributes::default(),
            );
        }
        let _ = self.types.entities.merge_request(
            id,
            ObjectRequest {
                alignment,
                common: None,
            },
        );
    }

    fn function(&mut self, node: NodeId, owner: Option<Span<()>>, function: &FunctionDefinition) {
        let owner_linkage = owner.is_some();
        let mut names = None;
        let mut return_type = None;
        let owner = std::mem::replace(&mut self.types.owner, owner);
        let resolved = self
            .types
            .resolve(&function.specifiers, &function.declarator);
        self.types.owner = owner;
        if let Ok(ty) = resolved {
            let linkage = owner_linkage.then(|| linkage(function.specifiers.storage));
            self.declare_object(node, ty, None, linkage.flatten());
            names = function
                .declarator
                .name()
                .map(|name| self.types.function_names(ty, name));
            return_type = self
                .types
                .ctypes
                .function_parts(ty)
                .map(|(returned, ..)| returned)
                .filter(|&returned| !self.types.ctypes.is_void(returned));
        }
        let enclosing_return = std::mem::replace(&mut self.return_type, return_type);
        let enclosing = std::mem::replace(&mut self.types.function_names, names);
        for parameter in function
            .declarator
            .function_parameters()
            .map_or(&[][..], ParameterList::parameters)
        {
            self.specifier(&parameter.specifiers.ty);
            self.declarator(&parameter.declarator);
            let owner = self.types.owner.replace(parameter.derive(()));
            self.tag(&parameter.specifiers.ty);
            let provisional = std::mem::replace(&mut self.types.provisional_extents, true);
            let resolved = self
                .types
                .resolve(&parameter.specifiers, &parameter.declarator);
            self.types.provisional_extents = provisional;
            self.types.owner = owner;
            if let Ok(resolved) = resolved
                && !self.types.ctypes.is_void(resolved)
            {
                let declared_array = parameter.declarator.array_parameter().unwrap_or_default();
                let adjusted = self
                    .types
                    .adjusted_parameter(resolved, declared_array.qualifiers.into());
                self.declare_object(parameter.id, adjusted, None, None);
            }
        }
        for stmt in &function.body {
            self.statement(stmt);
        }
        self.types.function_names = enclosing;
        self.return_type = enclosing_return;
    }

    fn declaration(&mut self, declaration: &Declaration, global: bool) {
        if declaration.declarators.is_empty() {
            if !self.types.declare_forward_tag(&declaration.specifiers) {
                self.tag(&declaration.specifiers.ty);
                let _ = self
                    .types
                    .resolve(&declaration.specifiers, &Declarator::Abstract);
            }
            return;
        }
        let first = declaration.declarators.first();
        let owner = std::mem::replace(
            &mut self.types.owner,
            first.map(|declarator| declarator.derive(())),
        );
        self.specifier(&declaration.specifiers.ty);
        self.tag(&declaration.specifiers.ty);
        self.types.owner = owner;
        for declarator in &declaration.declarators {
            self.declarator(&declarator.declarator);
            let Some(name) = declarator.declarator.name() else {
                continue;
            };
            let owner = self.types.owner.replace(declarator.derive(()));
            let mut initialized = None;
            let provisional = std::mem::replace(&mut self.types.provisional_extents, true);
            let resolved = self
                .types
                .declarator_type(&declaration.specifiers, declarator);
            self.types.provisional_extents = provisional;
            if let Ok(resolved) = resolved
                && declaration.specifiers.storage == StorageClass::Typedef
            {
                let attributes = declaration
                    .specifiers
                    .attributes
                    .iter()
                    .chain(&declarator.attributes);
                if self.types.ctypes.is_variably_modified(resolved) {
                    self.types
                        .declare_provisional_alias(declarator.id, name.to_owned(), resolved);
                } else {
                    let _ = self.types.define_alias(
                        declarator.id,
                        name.to_owned(),
                        resolved,
                        attributes,
                    );
                }
            }
            self.types.owner = owner;
            if let Ok(resolved) = resolved
                && declaration.specifiers.storage != StorageClass::Typedef
                && (!self.types.ctypes.is_void(resolved)
                    || declaration.specifiers.storage == StorageClass::Extern)
            {
                let completed = self
                    .types
                    .completed_array(resolved, declarator.initializer.as_ref());
                let attributes = declaration
                    .specifiers
                    .attributes
                    .iter()
                    .chain(&declarator.attributes);
                let requested = super::types::requested_alignment(self.types, attributes)
                    .ok()
                    .flatten();
                let storage = declaration.specifiers.storage;
                let linkage = if global
                    || storage == StorageClass::Extern
                    || self.types.ctypes.is_function(completed)
                {
                    linkage(storage)
                } else {
                    None
                };
                self.declare_object(declarator.id, completed, requested, linkage);
                if !self.types.ctypes.is_array(completed)
                    && !self.types.ctypes.is_function(completed)
                {
                    initialized = Some(completed);
                }
                if declaration.specifiers.is_constexpr
                    && let Some(Initializer::Expr(expr)) = &declarator.initializer
                    && ice_shape(self.types, expr).is_constant()
                    && let Ok(value) = self.types.constant_value(expr)
                {
                    self.types.declare_constant(declarator.id, value);
                }
            }
            if let Some(initializer) = &declarator.initializer {
                if (global || declaration.specifiers.storage == StorageClass::Static)
                    && let Err((expr, reason)) = (InvalidConstantArithmetic {
                        types: self.types,
                        flavor: self.unit.dialect.flavor(),
                    })
                    .visit_initializer(initializer)
                {
                    self.errors.push(error(
                        expr.provenance,
                        expr.expansion,
                        format!("initializer element is not a compile-time constant: {reason}"),
                    ));
                }
                self.initializer(initializer);
                if let (Some(to), Initializer::Expr(expr)) = (initialized, initializer) {
                    self.convert(expr, to, ConversionContext::Assign);
                }
            }
        }
    }

    fn tag(&mut self, ty: &TypeSpecifier) {
        let TypeSpecifier::Tag(TagSpecifier::Definition(id)) = ty else {
            return;
        };
        let Some(tag) = self.unit.tag(*id) else {
            return;
        };
        if let TagBody::Enum {
            enumerators,
            fixed_type,
        } = &tag.body
        {
            let int_ty = self.types.ctypes.int();
            let fixed = fixed_type
                .as_ref()
                .and_then(|ty| self.types.resolve(&ty.specifiers, &ty.declarator).ok());
            let mut previous: Option<BigInt> = Some((-1).into());
            for item in enumerators {
                let EnumItemKind::Enumerator(enumerator) = &item.value else {
                    continue;
                };
                let value = match &enumerator.value {
                    Some(expr) if ice_shape(self.types, expr).is_constant() => {
                        self.types.constant_integer(expr).ok()
                    }
                    Some(_) => None,
                    None => previous.as_ref().map(|value| value + 1),
                };
                previous = value.clone();
                if let Some(value) = value {
                    let c = fixed.unwrap_or(int_ty);
                    let ty = self.types.ir_type(c);
                    let Type::Numeric(NumericType::Integer { width, signed, .. }) = ty else {
                        continue;
                    };
                    let limit = BigInt::from(1u8) << (width - u32::from(signed));
                    let min = if signed { -&limit } else { BigInt::from(0u8) };
                    if value < min || value >= limit {
                        continue;
                    }
                    self.types.declare_constant(
                        item.id,
                        super::operand::Operand {
                            c,
                            value: Value {
                                ty,
                                node: item
                                    .clone()
                                    .derive(ValueKind::Constant(Number::SignedInteger(value))),
                            },
                        },
                    );
                }
            }
        }
        // Failed dependencies remain unavailable; only a consuming assertion diagnoses them.
        let specifiers = DeclarationSpecifiers {
            ty: ty.clone(),
            qualifiers: Qualifiers::default(),
            storage: StorageClass::None,
            is_thread_local: false,
            is_inline: false,
            is_noreturn: false,
            is_constexpr: false,
            attributes: Vec::new(),
        };
        let _ = self.types.resolve(&specifiers, &Declarator::Abstract);
    }

    fn statement(&mut self, stmt: &Stmt) {
        match &stmt.value {
            StmtKind::StaticAssert(assertion) => self.assertion(assertion),
            StmtKind::Decl(declaration) => self.declaration(declaration, false),
            StmtKind::Block(body) => {
                for stmt in body {
                    self.statement(stmt);
                }
            }
            StmtKind::NestedFunction(function) => self.function(stmt.id, None, function),
            StmtKind::If {
                condition,
                then_branch,
                else_branch,
            } => {
                self.expression(condition);
                self.statement(then_branch);
                if let Some(branch) = else_branch {
                    self.statement(branch);
                }
            }
            StmtKind::DoWhile { condition, body } => {
                self.statement(body);
                self.expression(condition);
            }
            StmtKind::While { condition, body }
            | StmtKind::Switch {
                discriminant: condition,
                body,
            } => {
                self.expression(condition);
                self.statement(body);
            }
            StmtKind::For {
                init,
                condition,
                increment,
                body,
            } => {
                if let Some(init) = init {
                    self.statement(init);
                }
                for expr in condition.iter().chain(increment) {
                    self.expression(expr);
                }
                self.statement(body);
            }
            StmtKind::SwitchLabel { label, body } => {
                match label {
                    SwitchLabel::Case(value) => self.expression(value),
                    SwitchLabel::CaseRange { start, end } => {
                        self.expression(start);
                        self.expression(end);
                    }
                    SwitchLabel::Default => {}
                }
                self.statement(body);
            }
            StmtKind::Asm(asm) => {
                for operand in asm
                    .operands
                    .iter()
                    .flat_map(|operands| operands.outputs.iter().chain(&operands.inputs))
                {
                    self.expression(&operand.expr);
                }
            }
            StmtKind::Labeled { body, .. } | StmtKind::Attributed { body, .. } => {
                self.statement(body)
            }
            StmtKind::Return(expr) => {
                self.expression(expr);
                if let Some(to) = self.return_type {
                    self.convert(expr, to, ConversionContext::Return);
                }
            }
            StmtKind::Expr(expr) | StmtKind::ComputedGoto(expr) => self.expression(expr),
            _ => {}
        }
    }

    fn initializer(&mut self, initializer: &Initializer) {
        match initializer {
            Initializer::Expr(expr) => self.expression(expr),
            Initializer::List(items) => {
                for item in items {
                    self.initializer(&item.value);
                }
            }
        }
    }

    fn expression(&mut self, expr: &Expr) {
        match &expr.value {
            ExprKind::StatementExpression(body) => {
                for stmt in body {
                    self.statement(stmt);
                }
            }
            ExprKind::Cast { ty, value } => {
                self.type_name(ty);
                self.expression(value);
                self.cast(expr, ty, value);
            }
            ExprKind::BitCast { ty, value } | ExprKind::ConvertVector { ty, value } => {
                self.type_name(ty);
                self.expression(value);
            }
            ExprKind::VaArg { list, ty } => {
                self.expression(list);
                self.type_name(ty);
            }
            ExprKind::SizeOfType { ty } | ExprKind::AlignOf { ty } => self.type_name(ty),
            ExprKind::OffsetOf { ty, member } => {
                self.type_name(ty);
                self.expression(member);
            }
            ExprKind::TypesCompatible { left_ty, right_ty } => {
                self.type_name(left_ty);
                self.type_name(right_ty);
            }
            ExprKind::Paren(expr)
            | ExprKind::Unary { operand: expr, .. }
            | ExprKind::Postfix { operand: expr, .. }
            | ExprKind::SizeOfExpr(expr)
            | ExprKind::AlignOfExpr(expr)
            | ExprKind::Member { base: expr, .. } => self.expression(expr),
            ExprKind::Assign { op, target, value } => {
                self.expression(target);
                self.expression(value);
                if *op == AssignOp::Assign
                    && let Ok(target) = self.types.typed(target)
                    && target.lvalue
                    && self.types.require_modifiable_lvalue(target.c).is_ok()
                {
                    self.convert(value, target.c, ConversionContext::Assign);
                }
            }
            ExprKind::Binary { left, right, .. }
            | ExprKind::Comma { left, right }
            | ExprKind::Index {
                base: left,
                index: right,
            } => {
                self.expression(left);
                self.expression(right);
            }
            ExprKind::Conditional {
                condition,
                then_value,
                else_value,
            } => {
                self.expression(condition);
                if let Some(expr) = then_value {
                    self.expression(expr);
                }
                self.expression(else_value);
            }
            ExprKind::Call { callee, arguments } => {
                self.expression(callee);
                for argument in arguments {
                    self.expression(argument);
                }
                self.arguments(callee, arguments);
            }
            ExprKind::CompoundLiteral { ty, initializer } => {
                self.type_name(ty);
                for item in initializer {
                    self.initializer(&item.value);
                }
            }
            ExprKind::Generic {
                controlling,
                associations,
            } => {
                match controlling {
                    GenericControl::Expr(expr) => self.expression(expr),
                    GenericControl::Type { ty } => self.type_name(ty),
                }
                for association in associations {
                    match association {
                        GenericAssociation::Type { ty, value } => {
                            self.type_name(ty);
                            self.expression(value);
                        }
                        GenericAssociation::Default(value) => self.expression(value),
                    }
                }
            }
            _ => {}
        }
    }

    fn convert(&mut self, expr: &Expr, to: QualType, context: ConversionContext) {
        if let Ok(from) = self.types.operand_type(expr) {
            self.convert_at(expr, expr, from, to, context);
        }
    }

    fn convert_at(
        &mut self,
        at: &Expr,
        expr: &Expr,
        from: QualType,
        to: QualType,
        context: ConversionContext,
    ) {
        if let Err(reason) = self.types.record_conversion(expr, from, to, context) {
            self.errors
                .push(error(at.provenance, at.expansion, reason.to_string()));
        }
    }

    fn cast(&mut self, cast: &Expr, ty: &TypeName, value: &Expr) {
        let provisional = std::mem::replace(&mut self.types.provisional_extents, true);
        let to = self.types.resolve(&ty.specifiers, &ty.declarator);
        self.types.provisional_extents = provisional;
        let (Ok(to), Ok(from)) = (to, self.types.operand_type(value)) else {
            return;
        };
        let to = self.types.ctypes.unqualified(to);
        if self.types.ctypes.is_void(to)
            || !matches!(self.types.union_cast_member(to, from), Ok(None))
        {
            return;
        }
        self.convert_at(cast, value, from, to, ConversionContext::Cast);
    }

    fn arguments(&mut self, callee: &Expr, arguments: &[Expr]) {
        let Ok(Some(signature)) = self.types.argument_signature(callee, arguments) else {
            return;
        };
        let Some((_, parameters, ..)) = self.types.ctypes.function_parts(signature) else {
            return;
        };
        let parameters = parameters.to_vec();
        for (index, argument) in arguments.iter().enumerate() {
            let Ok(from) = self.types.operand_type(argument) else {
                continue;
            };
            match parameters.get(index) {
                Some(&parameter) => {
                    let to = self.types.ctypes.adjust_parameter(parameter);
                    self.convert_at(argument, argument, from, to, ConversionContext::Arg);
                }
                None => {
                    let from = self.types.ctypes.enum_underlying(from).unwrap_or(from);
                    let target = self.types.target_info().clone();
                    let to = self.types.ctypes.default_promotion(from, &target);
                    self.convert_at(argument, argument, from, to, ConversionContext::Arg);
                }
            }
        }
    }

    fn declarator(&mut self, declarator: &Declarator) {
        match declarator {
            Declarator::Grouped(inner)
            | Declarator::Attributed { inner, .. }
            | Declarator::Pointer { inner, .. } => self.declarator(inner),
            Declarator::Array { inner, size, .. } => {
                self.declarator(inner);
                if let ArraySize::Expression(size) = size {
                    self.expression(size);
                }
            }
            Declarator::Function { inner, parameters } => {
                self.declarator(inner);
                for parameter in parameters.parameters() {
                    self.specifier(&parameter.specifiers.ty);
                    self.declarator(&parameter.declarator);
                }
            }
            Declarator::Abstract | Declarator::Name(_) => {}
        }
    }

    fn specifier(&mut self, ty: &TypeSpecifier) {
        if let TypeSpecifier::TypeOf(TypeOfOperand::Expression(operand))
        | TypeSpecifier::TypeOfUnqual(TypeOfOperand::Expression(operand)) = ty
        {
            self.expression(operand);
        }
    }

    fn type_name(&mut self, ty: &TypeName) {
        self.specifier(&ty.specifiers.ty);
        self.declarator(&ty.declarator);
        self.tag(&ty.specifiers.ty);
        let _ = self.types.resolve(&ty.specifiers, &ty.declarator);
    }
}

struct InvalidConstantArithmetic<'a> {
    types: &'a mut TypeResolver,
    flavor: crate::compiler_args::CompilerFlavor,
}

impl Visitor for InvalidConstantArithmetic<'_> {
    type Error = (Expr, &'static str);

    fn visit_expr(&mut self, expr: &Expr) -> Result<(), Self::Error> {
        match &expr.value {
            ExprKind::Binary {
                op: BinaryOp::Div | BinaryOp::Rem,
                right,
                ..
            } if self.flavor != crate::compiler_args::CompilerFlavor::Msvc
                && self
                    .types
                    .constant_integer(right)
                    .is_ok_and(|value| value == 0.into()) =>
            {
                Err((expr.clone(), "division by zero"))
            }
            ExprKind::Binary {
                op: BinaryOp::ShiftLeft,
                left,
                right,
                ..
            } if self.flavor == crate::compiler_args::CompilerFlavor::Gcc
                && self
                    .types
                    .constant_integer(right)
                    .is_ok_and(|value| value.sign() == Sign::Minus)
                && self
                    .types
                    .constant_integer(left)
                    .is_ok_and(|value| value.sign() != Sign::NoSign) =>
            {
                Err((expr.clone(), "negative shift count"))
            }
            ExprKind::Binary {
                op: BinaryOp::And | BinaryOp::Or,
                left,
                right,
            } => {
                self.visit_expr(left)?;
                let truth = self
                    .types
                    .constant_integer(left)
                    .ok()
                    .map(|value| value.sign() != Sign::NoSign);
                if truth
                    != Some(matches!(
                        expr.value,
                        ExprKind::Binary {
                            op: BinaryOp::Or,
                            ..
                        }
                    ))
                {
                    self.visit_expr(right)?;
                }
                Ok(())
            }
            ExprKind::Conditional {
                condition,
                then_value,
                else_value,
            } => {
                self.visit_expr(condition)?;
                match self
                    .types
                    .constant_integer(condition)
                    .ok()
                    .map(|value| value.sign() != Sign::NoSign)
                {
                    Some(true) => {
                        if let Some(value) = then_value {
                            self.visit_expr(value)?;
                        }
                    }
                    Some(false) => self.visit_expr(else_value)?,
                    None => {
                        if let Some(value) = then_value {
                            self.visit_expr(value)?;
                        }
                        self.visit_expr(else_value)?;
                    }
                }
                Ok(())
            }
            ExprKind::SizeOfExpr(_) | ExprKind::AlignOfExpr(_) => Ok(()),
            ExprKind::Generic {
                controlling,
                associations,
            } => {
                if let Ok(selected) = self.types.generic_selection(controlling, associations) {
                    self.visit_expr(selected)?;
                }
                Ok(())
            }
            _ => visit::walk_expr(self, expr),
        }
    }
}

// Fold evaluates executed IR only, so enforce the supported ICE syntax separately.
#[derive(Clone, Copy)]
enum Shape<'e> {
    Constant,
    Skip,
    NotConstant(&'e Expr),
}

impl<'e> Shape<'e> {
    fn and(self, other: Self) -> Self {
        match (self, other) {
            (Shape::NotConstant(expr), _) | (_, Shape::NotConstant(expr)) => {
                Shape::NotConstant(expr)
            }
            (Shape::Constant, Shape::Constant) => Shape::Constant,
            _ => Shape::Skip,
        }
    }

    fn opaque(self) -> Self {
        self.and(Shape::Skip)
    }

    fn unevaluated(self) -> Self {
        match self {
            Shape::NotConstant(_) => Shape::Skip,
            shape => shape,
        }
    }

    fn is_constant(self) -> bool {
        matches!(self, Shape::Constant)
    }
}

fn evaluated_shape<'e>(types: &mut TypeResolver, expr: &'e Expr, evaluated: bool) -> Shape<'e> {
    let shape = ice_shape(types, expr);
    if evaluated {
        shape
    } else {
        shape.unevaluated()
    }
}

fn constant_truth(types: &mut TypeResolver, expr: &Expr, shape: Shape) -> Option<bool> {
    if !shape.is_constant() {
        return None;
    }
    types
        .constant_integer(expr)
        .ok()
        .map(|value| value.sign() != Sign::NoSign)
}

fn call_shape<'e>(expr: &'e Expr, callee: &Expr, arguments: &'e [Expr]) -> Shape<'e> {
    if super::expression::constant_p_operand(callee, arguments).is_some() {
        return Shape::Constant;
    }
    if super::expression::choose_expr_operands(callee, arguments).is_some() {
        return Shape::Skip;
    }
    let mut callee = callee;
    while let ExprKind::Paren(inner) = &callee.value {
        callee = inner;
    }
    match &callee.value {
        ExprKind::Identifier(name) if super::builtins::is_foldable_builtin(name) => Shape::Skip,
        _ => Shape::NotConstant(expr),
    }
}

fn ice_shape<'e>(types: &mut TypeResolver, expr: &'e Expr) -> Shape<'e> {
    match &expr.value {
        ExprKind::IntegerLiteral(_)
        | ExprKind::FloatLiteral(_)
        | ExprKind::CharLiteral(_)
        | ExprKind::BoolLiteral(_)
        | ExprKind::Identifier(_)
        | ExprKind::SizeOfType { .. }
        | ExprKind::AlignOf { .. }
        | ExprKind::SizeOfExpr(_)
        | ExprKind::AlignOfExpr(_)
        | ExprKind::OffsetOf { .. }
        | ExprKind::TypesCompatible { .. } => Shape::Constant,
        ExprKind::Call { callee, arguments } => call_shape(expr, callee, arguments),
        ExprKind::Paren(expr) | ExprKind::Cast { value: expr, .. } => ice_shape(types, expr),
        ExprKind::Unary { op, operand } => {
            let shape = ice_shape(types, operand);
            if matches!(
                op,
                UnaryOp::Plus | UnaryOp::Minus | UnaryOp::Not | UnaryOp::BitNot
            ) {
                shape
            } else {
                shape.opaque()
            }
        }
        ExprKind::Binary {
            op: op @ (BinaryOp::And | BinaryOp::Or),
            left,
            right,
        } => {
            let left_shape = ice_shape(types, left);
            let short_circuits = constant_truth(types, left, left_shape)
                .map(|truth| truth == matches!(op, BinaryOp::Or));
            left_shape.and(evaluated_shape(types, right, short_circuits == Some(false)))
        }
        ExprKind::Binary { left, right, .. } => ice_shape(types, left).and(ice_shape(types, right)),
        ExprKind::Comma { left, right } => {
            ice_shape(types, left).and(ice_shape(types, right)).opaque()
        }
        ExprKind::Conditional {
            condition,
            then_value,
            else_value,
        } => {
            let condition_shape = ice_shape(types, condition);
            let truth = constant_truth(types, condition, condition_shape);
            let then_shape = then_value.as_ref().map_or(Shape::Constant, |value| {
                evaluated_shape(types, value, truth == Some(true))
            });
            let else_shape = evaluated_shape(types, else_value, truth == Some(false));
            condition_shape.and(then_shape).and(else_shape)
        }
        ExprKind::Generic {
            controlling,
            associations,
        } => types
            .generic_selection(controlling, associations)
            .map_or(Shape::Skip, |selected| ice_shape(types, selected)),
        _ => Shape::Skip,
    }
}

fn linkage(storage: StorageClass) -> Option<Linkage> {
    match storage {
        StorageClass::Static => Some(Linkage::Internal),
        StorageClass::None | StorageClass::Extern => Some(Linkage::External),
        _ => None,
    }
}
