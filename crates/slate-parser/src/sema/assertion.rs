use crate::ast::*;
use crate::const_expr::{BinaryOp, UnaryOp};
use crate::ir::{Number, NumericType, Type, Value, ValueKind};
use crate::visit::{self, Visitor};
use num_bigint::{BigInt, Sign};

use super::numeric::ResolveError;
use super::types::{Ordinary, TypeResolver};
use super::validate::{SemaError, error};

pub(super) fn validate(unit: &TranslationUnit) -> Vec<SemaError> {
    let mut checker = Checker {
        unit,
        types: TypeResolver::with_tags(unit.options.effective_target(unit.target.clone()), unit),
        errors: Vec::new(),
    };
    checker.types.assertion_scope = true;
    for declaration in &unit.decls {
        match &declaration.value {
            DeclKind::StaticAssert(assertion) => checker.assertion(assertion),
            DeclKind::Declaration(declaration) => checker.declaration(declaration, true),
            DeclKind::Function(function) => checker.function(function),
            _ => {}
        }
    }
    checker.errors
}

struct Checker<'a> {
    unit: &'a TranslationUnit,
    types: TypeResolver,
    errors: Vec<SemaError>,
}

impl Checker<'_> {
    fn assertion(&mut self, assertion: &StaticAssert) {
        let condition = &assertion.condition;
        match ice_shape(&mut self.types, condition) {
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
                    super::fold::integer_constant(&value, self.unit.flavor).ok_or(
                        ResolveError::Unsupported("nonconstant or undefined integer expression"),
                    )
                }
                _ => Err(ResolveError::Unsupported("non-integer constant expression")),
            })
            .map_err(|error| match error {
                ResolveError::Unsupported(reason) => reason.to_owned(),
                error => error.to_string(),
            });
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

    fn function(&mut self, function: &FunctionDefinition) {
        self.types.push_scope();
        for parameter in function
            .declarator
            .function_parameters()
            .map_or(&[][..], ParameterList::parameters)
        {
            self.tag(&parameter.specifiers.ty);
            let resolved = self
                .types
                .resolve(&parameter.specifiers, &parameter.declarator);
            if let Some(name) = parameter.declarator.name() {
                self.types.declare(name, Ordinary::Declared);
                if let Ok(resolved) = resolved
                    && !self.types.ctypes.is_void(resolved)
                {
                    let adjusted = self
                        .types
                        .adjusted_parameter(resolved, super::ctype::Qualifiers::NONE);
                    self.types.declare(name, Ordinary::Object(adjusted));
                }
            }
        }
        for stmt in &function.body {
            self.statement(stmt);
        }
        self.types.pop_scope();
    }

    fn completed_array(
        &mut self,
        resolved: super::ctype::QualType,
        initializer: Option<&Initializer>,
    ) -> super::ctype::QualType {
        let Some((element, super::ctype::Extent::Incomplete)) = self.types.ctypes.element(resolved)
        else {
            return resolved;
        };
        let length = match initializer {
            Some(Initializer::Expr(expr)) => match self.types.assertion_operand_type(expr) {
                Ok(c) => match self.types.ctypes.element(c) {
                    Some((_, super::ctype::Extent::Fixed(length))) => Some(length),
                    _ => None,
                },
                _ => None,
            },
            Some(Initializer::List(items)) => self.types.inferred_array_length(element, items).ok(),
            None => None,
        };
        match length {
            Some(length) => self.types.ctypes.qual(super::ctype::CTypeKind::Array {
                element,
                extent: super::ctype::Extent::Fixed(length),
            }),
            None => resolved,
        }
    }

    fn declaration(&mut self, declaration: &Declaration, global: bool) {
        if declaration.declarators.is_empty()
            && self.types.declare_forward_tag(&declaration.specifiers)
        {
            return;
        }
        self.tag(&declaration.specifiers.ty);
        for declarator in &declaration.declarators {
            let Some(name) = declarator.declarator.name() else {
                continue;
            };
            if matches!(declaration.specifiers.ty, TypeSpecifier::Inferred)
                && let Some(Initializer::Expr(expr)) = &declarator.initializer
                && let Ok(value) = self.types.assertion_operand_type(expr)
                && let Ok((base, _)) = self.types.inferred_base(&declarator.declarator, value)
            {
                self.types.inferred = Some(base);
            }
            let resolved = self
                .types
                .resolve(&declaration.specifiers, &declarator.declarator);
            self.types.inferred = None;
            self.types.declare(name, Ordinary::Declared);
            if let Ok(resolved) = resolved {
                if declaration.specifiers.storage == StorageClass::Typedef {
                    let attributes = declaration
                        .specifiers
                        .attributes
                        .iter()
                        .chain(&declarator.attributes);
                    let _ = self
                        .types
                        .define_alias(name.to_owned(), resolved, attributes);
                } else if !self.types.ctypes.is_void(resolved) {
                    let completed = self.completed_array(resolved, declarator.initializer.as_ref());
                    self.types.declare(name, Ordinary::Object(completed));
                    if declaration.specifiers.is_constexpr
                        && let Some(Initializer::Expr(expr)) = &declarator.initializer
                        && ice_shape(&mut self.types, expr).is_constant()
                        && let Ok(value) = self.types.constant_value(expr)
                    {
                        self.types.declare(name, Ordinary::Constant(value));
                    }
                }
            }
            if let Some(initializer) = &declarator.initializer {
                if (global || declaration.specifiers.storage == StorageClass::Static)
                    && let Err((expr, reason)) = (InvalidConstantArithmetic {
                        types: &mut self.types,
                        flavor: self.unit.flavor,
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
                    Some(expr) if ice_shape(&mut self.types, expr).is_constant() => {
                        self.types.constant_integer(expr).ok()
                    }
                    Some(_) => None,
                    None => previous.as_ref().map(|value| value + 1),
                };
                self.types.declare(&enumerator.name, Ordinary::Declared);
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
                    self.types.declare(
                        &enumerator.name,
                        Ordinary::Constant(super::operand::Operand {
                            c,
                            value: Value {
                                ty,
                                node: item
                                    .clone()
                                    .derive(ValueKind::Constant(Number::SignedInteger(value))),
                            },
                        }),
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

    fn scoped(&mut self, stmt: &Stmt) {
        self.types.push_scope();
        self.statement(stmt);
        self.types.pop_scope();
    }

    fn statement(&mut self, stmt: &Stmt) {
        match &stmt.value {
            StmtKind::StaticAssert(assertion) => self.assertion(assertion),
            StmtKind::Decl(declaration) => self.declaration(declaration, false),
            StmtKind::Block(body) => {
                self.types.push_scope();
                for stmt in body {
                    self.statement(stmt);
                }
                self.types.pop_scope();
            }
            StmtKind::NestedFunction(function) => self.function(function),
            StmtKind::If {
                condition,
                then_branch,
                else_branch,
            } => {
                self.expression(condition);
                self.scoped(then_branch);
                if let Some(branch) = else_branch {
                    self.scoped(branch);
                }
            }
            StmtKind::While { condition, body }
            | StmtKind::DoWhile { condition, body }
            | StmtKind::Switch {
                discriminant: condition,
                body,
            } => {
                self.expression(condition);
                self.scoped(body);
            }
            StmtKind::For {
                init,
                condition,
                increment,
                body,
            } => {
                self.types.push_scope();
                if let Some(init) = init {
                    self.statement(init);
                }
                for expr in condition.iter().chain(increment) {
                    self.expression(expr);
                }
                self.scoped(body);
                self.types.pop_scope();
            }
            StmtKind::Labeled { body, .. }
            | StmtKind::Attributed { body, .. }
            | StmtKind::SwitchLabel { body, .. } => self.statement(body),
            StmtKind::Expr(expr) | StmtKind::Return(expr) | StmtKind::ComputedGoto(expr) => {
                self.expression(expr)
            }
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
                self.types.push_scope();
                for stmt in body {
                    self.statement(stmt);
                }
                self.types.pop_scope();
            }
            ExprKind::Paren(expr)
            | ExprKind::Unary { operand: expr, .. }
            | ExprKind::Postfix { operand: expr, .. }
            | ExprKind::Cast { value: expr, .. }
            | ExprKind::BitCast { value: expr, .. }
            | ExprKind::ConvertVector { value: expr, .. }
            | ExprKind::VaArg { list: expr, .. }
            | ExprKind::SizeOfExpr(expr)
            | ExprKind::AlignOfExpr(expr)
            | ExprKind::Member { base: expr, .. } => self.expression(expr),
            ExprKind::Binary { left, right, .. }
            | ExprKind::Assign {
                target: left,
                value: right,
                ..
            }
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
            }
            ExprKind::CompoundLiteral { initializer, .. } => {
                for item in initializer {
                    self.initializer(&item.value);
                }
            }
            ExprKind::Generic {
                controlling,
                associations,
            } => {
                if let GenericControl::Expr(expr) = controlling {
                    self.expression(expr);
                }
                for association in associations {
                    let (GenericAssociation::Type { value, .. }
                    | GenericAssociation::Default(value)) = association;
                    self.expression(value);
                }
            }
            _ => {}
        }
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
