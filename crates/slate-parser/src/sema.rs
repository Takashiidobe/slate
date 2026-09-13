use crate::ast::*;
use crate::compiler_args::CompilerFlavor;
use crate::const_expr::ConstExpr;
use crate::files::{Files, decode_source_bytes, display_path};
use crate::lexer::Token;
use miette::{Diagnostic, NamedSource, SourceSpan};
use std::collections::{HashMap, HashSet};
use thiserror::Error;

#[derive(Debug, Error, Diagnostic, Clone)]
#[error("{message}")]
pub struct SemaError {
    pub message: String,
    pub provenance: Option<Provenance>,
    pub loc: Option<Loc>,
    #[source_code]
    pub source_code: NamedSource<String>,
    #[label]
    pub span: SourceSpan,
}

#[derive(Debug, Error, Diagnostic)]
#[error("semantic analysis failed")]
pub struct SemaErrors {
    #[related]
    pub errors: Vec<SemaError>,
}

impl TranslationUnit {
    pub fn analyze(&self, files: &Files) -> Result<(), SemaErrors> {
        let flavor = self.flavor;
        let typedefs = self
            .decls
            .iter()
            .filter_map(|decl| match &decl.value {
                Decl::Typedef { name, .. } => Some(name.clone()),
                _ => None,
            })
            .collect::<HashSet<_>>();
        let mut tags = HashSet::new();
        for decl in &self.decls {
            match &decl.value {
                Decl::Record(record) => {
                    if let Some(name) = &record.name {
                        tags.insert(name.clone());
                    }
                    for field_item in &record.fields {
                        if let FieldItem::Field(field) = &field_item.value {
                            collect_tag_names(&field.declaration.specifiers.ty, &mut tags);
                        }
                    }
                }
                Decl::Enum(enumeration) => {
                    if let Some(name) = &enumeration.name {
                        tags.insert(name.clone());
                    }
                }
                Decl::Function(function) => {
                    collect_tag_names(&function.ret_type, &mut tags);
                    for parameter in &function.parameters {
                        collect_tag_names(&parameter.ty, &mut tags);
                    }
                }
                Decl::Declaration { declaration, .. } => {
                    collect_tag_names(&declaration.specifiers.ty, &mut tags);
                }
                Decl::Typedef { ty, .. } => collect_tag_names(ty, &mut tags),
                Decl::Comment { .. } | Decl::StaticAssert { .. } | Decl::Asm { .. } => {}
            }
        }

        let mut errors = Vec::new();
        for decl in &self.decls {
            match &decl.value {
                Decl::Comment { .. } | Decl::StaticAssert { .. } | Decl::Asm { .. } => {}
                Decl::Function(function) => {
                    check_attributes(
                        &function.attributes,
                        function.provenance,
                        decl.expansion,
                        &mut errors,
                    );
                    check_type(
                        &function.ret_type,
                        &typedefs,
                        &tags,
                        function.provenance,
                        decl.expansion,
                        &mut errors,
                    );
                    if flavor == CompilerFlavor::Clang {
                        check_function_asm(function, &mut errors);
                    }
                }
                Decl::Declaration {
                    declaration,
                    provenance,
                } => {
                    if matches!(declaration.specifiers.ty, CType::Void)
                        && declaration.specifiers.storage != StorageClass::Extern
                        && declaration.declarator.name().is_some()
                        && !declarator_indirects_void(&declaration.declarator)
                    {
                        errors.push(error(
                            *provenance,
                            decl.expansion,
                            "object cannot have type void",
                        ));
                    }
                    check_type(
                        &declaration.specifiers.ty,
                        &typedefs,
                        &tags,
                        *provenance,
                        decl.expansion,
                        &mut errors,
                    );
                    check_declarator(
                        &declaration.declarator,
                        &typedefs,
                        &tags,
                        *provenance,
                        decl.expansion,
                        &mut errors,
                    );
                    check_attributes(
                        &declaration.attributes,
                        *provenance,
                        decl.expansion,
                        &mut errors,
                    );
                    if flavor == CompilerFlavor::Clang {
                        check_register_variable(
                            declaration,
                            true,
                            *provenance,
                            decl.expansion,
                            &mut errors,
                        );
                    }
                }
                Decl::Typedef {
                    ty,
                    provenance,
                    attributes,
                    ..
                } => {
                    check_type(
                        ty,
                        &typedefs,
                        &tags,
                        *provenance,
                        decl.expansion,
                        &mut errors,
                    );
                    check_attributes(attributes, *provenance, decl.expansion, &mut errors);
                }
                Decl::Record(record) => {
                    check_attributes(
                        &record.attributes,
                        record.provenance,
                        decl.expansion,
                        &mut errors,
                    );
                    for field_item in &record.fields {
                        let FieldItem::Field(field) = &field_item.value else {
                            continue;
                        };
                        check_type(
                            &field.declaration.specifiers.ty,
                            &typedefs,
                            &tags,
                            field.provenance,
                            field_item.expansion,
                            &mut errors,
                        );
                        check_attributes(
                            &field.declaration.attributes,
                            field.provenance,
                            field_item.expansion,
                            &mut errors,
                        );
                    }
                }
                Decl::Enum(_) => {}
            }
        }
        let errors: Vec<SemaError> = errors
            .into_iter()
            .map(|error| error.with_source(files))
            .collect();
        if errors.is_empty() {
            Ok(())
        } else {
            Err(SemaErrors { errors })
        }
    }
}

impl SemaError {
    fn with_source(mut self, files: &Files) -> Self {
        let Some(loc) = self.loc else {
            return self;
        };
        let path = files.path(loc.file);
        let source = std::fs::read(path)
            .map(|bytes| decode_source_bytes(&bytes))
            .unwrap_or_default();
        self.source_code = NamedSource::new(display_path(path), source).with_language("C");
        self.span = SourceSpan::new(loc.offset.into(), loc.length.max(1));
        self
    }
}

fn declarator_indirects_void(declarator: &Declarator) -> bool {
    match declarator {
        Declarator::Grouped(inner)
        | Declarator::Attributed { inner, .. }
        | Declarator::Array { inner, .. } => declarator_indirects_void(inner),
        Declarator::Pointer { .. } | Declarator::Function { .. } => true,
        Declarator::Abstract | Declarator::Name(_) => false,
    }
}

fn check_declarator(
    declarator: &Declarator,
    typedefs: &HashSet<String>,
    tags: &HashSet<String>,
    provenance: Provenance,
    loc: Loc,
    errors: &mut Vec<SemaError>,
) {
    match declarator {
        Declarator::Function {
            parameters, inner, ..
        } => {
            check_declarator(inner, typedefs, tags, provenance, loc, errors);
            for parameter in parameters {
                check_type(&parameter.ty, typedefs, tags, provenance, loc, errors);
                check_attributes(&parameter.attributes, provenance, loc, errors);
                if let Some(declarator) = &parameter.declarator {
                    check_declarator(declarator, typedefs, tags, provenance, loc, errors);
                }
            }
        }
        Declarator::Attributed { inner, attributes } => {
            check_attributes(attributes, provenance, loc, errors);
            check_declarator(inner, typedefs, tags, provenance, loc, errors);
        }
        Declarator::Grouped(inner)
        | Declarator::Pointer { inner, .. }
        | Declarator::Array { inner, .. } => {
            check_declarator(inner, typedefs, tags, provenance, loc, errors)
        }
        Declarator::Abstract | Declarator::Name(_) => {}
    }
}

fn check_attributes(
    attributes: &[Attribute],
    provenance: Provenance,
    loc: Loc,
    errors: &mut Vec<SemaError>,
) {
    for attribute in attributes {
        if let Attribute::Invalid { name, .. } = attribute {
            errors.push(error(
                provenance,
                loc,
                format!("invalid arguments for attribute `{name}`"),
            ));
        }
        if let Attribute::Aligned(expression) | Attribute::VectorSize(expression) = attribute
            && !is_integer_constant_expression(expression)
        {
            errors.push(error(
                provenance,
                loc,
                "layout attribute requires an integer constant expression",
            ));
        }
        if let Attribute::AllocSize(expressions) = attribute
            && !(1..=2).contains(&expressions.len())
        {
            errors.push(error(
                provenance,
                loc,
                "alloc_size expects one or two arguments",
            ));
        }
    }
}

fn is_integer_constant_expression(expression: &ConstExpr) -> bool {
    match expression {
        ConstExpr::Integer(_)
        | ConstExpr::WideInteger(_)
        | ConstExpr::SizeOf(_)
        | ConstExpr::SizeOfType { .. }
        | ConstExpr::AlignOf { .. } => true,
        ConstExpr::Unary { value, .. } | ConstExpr::Cast { value, .. } => {
            is_integer_constant_expression(value)
        }
        ConstExpr::Binary { left, right, .. } => {
            is_integer_constant_expression(left) && is_integer_constant_expression(right)
        }
        ConstExpr::Ternary {
            condition,
            then_value,
            else_value,
        } => {
            is_integer_constant_expression(condition)
                && is_integer_constant_expression(then_value)
                && is_integer_constant_expression(else_value)
        }
        ConstExpr::Elvis {
            condition,
            else_value,
        } => {
            is_integer_constant_expression(condition) && is_integer_constant_expression(else_value)
        }
        ConstExpr::Identifier(_)
        | ConstExpr::StringLit(_)
        | ConstExpr::Utf8StringLit(_)
        | ConstExpr::Utf16StringLit(_)
        | ConstExpr::Utf32StringLit(_)
        | ConstExpr::WideStringLit(_)
        | ConstExpr::Generic { .. }
        | ConstExpr::Float(_)
        | ConstExpr::Call { .. }
        | ConstExpr::Assign { .. }
        | ConstExpr::Comma(..)
        | ConstExpr::Member { .. }
        | ConstExpr::Arrow { .. }
        | ConstExpr::Index { .. }
        | ConstExpr::OffsetOf { .. }
        | ConstExpr::TypesCompatible { .. }
        | ConstExpr::PostIncrement(_)
        | ConstExpr::PostDecrement(_)
        | ConstExpr::PreIncrement(_)
        | ConstExpr::PreDecrement(_)
        | ConstExpr::AddrOf(_)
        | ConstExpr::Deref(_)
        | ConstExpr::CompoundLiteral { .. }
        | ConstExpr::BitCast { .. }
        | ConstExpr::VaArg { .. }
        | ConstExpr::LabelAddr(_)
        | ConstExpr::StatementExpression(_) => false,
    }
}

fn collect_tag_names(ty: &CType, tags: &mut HashSet<String>) {
    match ty {
        CType::Tagged { name, body, .. } => {
            if let Some(name) = name {
                tags.insert(name.clone());
            }
            match body {
                Some(TagBody::Fields(fields)) => {
                    for field in fields {
                        collect_tag_names(&field.declaration.specifiers.ty, tags);
                    }
                }
                Some(TagBody::Enumerators(_)) | None => {}
            }
        }
        CType::Qualified { ty, .. } | CType::Pointer { pointee: ty, .. } => {
            collect_tag_names(ty, tags)
        }
        CType::Atomic(ty) | CType::Complex(ty) | CType::Imaginary(ty) => {
            collect_tag_names(ty, tags)
        }
        CType::Vector(vector) => collect_tag_names(&vector.element, tags),
        CType::TypeOf(TypeOfOperand::Type(ty)) | CType::TypeOfUnqual(TypeOfOperand::Type(ty)) => {
            collect_tag_names(ty, tags)
        }
        CType::Array { element, .. } => collect_tag_names(element, tags),
        CType::Function {
            return_type,
            parameters,
            ..
        } => {
            collect_tag_names(return_type, tags);
            for parameter in parameters {
                collect_tag_names(&parameter.ty, tags);
            }
        }
        CType::Void
        | CType::Bool
        | CType::Integer(_)
        | CType::Floating(_)
        | CType::FixedPoint(_)
        | CType::TypeOf(TypeOfOperand::Expression(_))
        | CType::TypeOfUnqual(TypeOfOperand::Expression(_))
        | CType::TargetBuiltin(_)
        | CType::Named(_) => {}
    }
}

fn check_type(
    ty: &CType,
    typedefs: &HashSet<String>,
    tags: &HashSet<String>,
    provenance: Provenance,
    loc: Loc,
    errors: &mut Vec<SemaError>,
) {
    match ty {
        CType::Named(name) if !typedefs.contains(name) => errors.push(error(
            provenance,
            loc,
            format!("unknown type name `{name}`"),
        )),
        CType::Tagged {
            name: Some(name), ..
        } if !tags.contains(name) => {
            errors.push(error(provenance, loc, format!("unknown tag `{name}`")))
        }
        CType::Qualified { ty, .. } | CType::Pointer { pointee: ty, .. } => {
            check_type(ty, typedefs, tags, provenance, loc, errors)
        }
        CType::Atomic(ty) => check_type(ty, typedefs, tags, provenance, loc, errors),
        CType::Vector(vector) => {
            check_type(&vector.element, typedefs, tags, provenance, loc, errors)
        }
        CType::TypeOf(TypeOfOperand::Type(ty)) | CType::TypeOfUnqual(TypeOfOperand::Type(ty)) => {
            check_type(ty, typedefs, tags, provenance, loc, errors)
        }
        CType::Imaginary(ty) => check_type(ty, typedefs, tags, provenance, loc, errors),
        CType::Array { element, .. } => {
            check_type(element, typedefs, tags, provenance, loc, errors)
        }
        CType::Function {
            return_type,
            parameters,
            ..
        } => {
            check_type(return_type, typedefs, tags, provenance, loc, errors);
            for parameter in parameters {
                check_type(&parameter.ty, typedefs, tags, provenance, loc, errors);
            }
        }
        CType::Void
        | CType::Bool
        | CType::Integer(_)
        | CType::Floating(_)
        | CType::Complex(_)
        | CType::FixedPoint(_)
        | CType::TypeOf(TypeOfOperand::Expression(_))
        | CType::TypeOfUnqual(TypeOfOperand::Expression(_))
        | CType::TargetBuiltin(_)
        | CType::Named(_)
        | CType::Tagged { .. } => {}
    }
}

fn error(provenance: Provenance, loc: Loc, message: impl Into<String>) -> SemaError {
    SemaError {
        message: message.into(),
        provenance: Some(provenance),
        loc: Some(loc),
        source_code: NamedSource::new("<unknown>", String::new()),
        span: SourceSpan::new(0.into(), 0),
    }
}

enum BodyNode<'a> {
    Stmt(&'a SpannedStmt),
    Tokens(&'a [Span<Token>]),
    EnterJumpScope,
    ExitJumpScope,
}

fn walk_stmts<'a>(stmts: &'a [SpannedStmt], visit: &mut impl FnMut(BodyNode<'a>)) {
    for stmt in stmts {
        walk_stmt(stmt, visit);
    }
}

fn walk_stmt<'a>(stmt: &'a SpannedStmt, visit: &mut impl FnMut(BodyNode<'a>)) {
    visit(BodyNode::Stmt(stmt));
    match &stmt.value {
        Stmt::Return(expr) | Stmt::Expr(expr) | Stmt::Case(expr) | Stmt::ComputedGoto(expr) => {
            walk_expr(expr, visit)
        }
        Stmt::CaseRange { start, end } => {
            walk_expr(start, visit);
            walk_expr(end, visit);
        }
        Stmt::Decl(declaration) => {
            if let Some(initializer) = &declaration.initializer {
                walk_initializer(initializer, visit);
            }
        }
        Stmt::Block(body) | Stmt::DoWhile { body, .. } => walk_stmts(body, visit),
        Stmt::While { condition, body }
        | Stmt::Switch {
            discriminant: condition,
            body,
        } => {
            walk_expr(condition, visit);
            walk_stmts(body, visit);
        }
        Stmt::If {
            condition,
            then_branch,
            else_branch,
        } => {
            walk_expr(condition, visit);
            walk_stmts(then_branch, visit);
            if let Some(else_branch) = else_branch {
                walk_stmts(else_branch, visit);
            }
        }
        Stmt::For {
            init,
            condition,
            increment,
            body,
        } => {
            if let Some(init) = init {
                walk_stmt(init, visit);
            }
            for expr in condition.iter().chain(increment) {
                walk_expr(expr, visit);
            }
            walk_stmts(body, visit);
        }
        Stmt::Unreachable(inner) => walk_stmt(inner, visit),
        Stmt::NestedFunction(_)
        | Stmt::Comment { .. }
        | Stmt::ReturnVoid
        | Stmt::StaticAssert(_)
        | Stmt::Attribute(_)
        | Stmt::Default
        | Stmt::Labeled(_)
        | Stmt::LocalLabelDecl(_)
        | Stmt::Asm(_)
        | Stmt::Goto(_)
        | Stmt::Break
        | Stmt::Continue => {}
    }
}

fn walk_initializer<'a>(initializer: &'a Initializer, visit: &mut impl FnMut(BodyNode<'a>)) {
    match initializer {
        Initializer::Expr(expr) => walk_expr(expr, visit),
        Initializer::List(items) => {
            for item in items {
                walk_initializer(&item.value, visit);
            }
        }
    }
}

fn walk_expr<'a>(expr: &'a SpannedExpr, visit: &mut impl FnMut(BodyNode<'a>)) {
    match &expr.value {
        Expr::Const(value) => walk_const_expr(value, visit),
        Expr::Unary { value, .. } | Expr::SizeOf(value) => walk_expr(value, visit),
        Expr::Binary { left, right, .. } => {
            walk_expr(left, visit);
            walk_expr(right, visit);
        }
        Expr::StatementExpression(body) => {
            visit(BodyNode::EnterJumpScope);
            walk_stmts(body, visit);
            visit(BodyNode::ExitJumpScope);
        }
        Expr::IntLit(_)
        | Expr::StringLit(_)
        | Expr::Utf8StringLit(_)
        | Expr::Utf16StringLit(_)
        | Expr::Utf32StringLit(_)
        | Expr::WideStringLit(_)
        | Expr::Identifier(_) => {}
    }
}

fn walk_const_expr<'a>(expr: &'a ConstExpr, visit: &mut impl FnMut(BodyNode<'a>)) {
    match expr {
        ConstExpr::StatementExpression(tokens) => {
            visit(BodyNode::EnterJumpScope);
            visit(BodyNode::Tokens(tokens));
            visit(BodyNode::ExitJumpScope);
        }
        ConstExpr::Generic {
            controlling,
            associations,
        } => {
            walk_const_expr(controlling, visit);
            for association in associations {
                walk_const_expr(&association.expression, visit);
            }
        }
        ConstExpr::SizeOf(value)
        | ConstExpr::Unary { value, .. }
        | ConstExpr::Cast { value, .. }
        | ConstExpr::BitCast { value, .. }
        | ConstExpr::Member { base: value, .. }
        | ConstExpr::Arrow { base: value, .. }
        | ConstExpr::PostIncrement(value)
        | ConstExpr::PostDecrement(value)
        | ConstExpr::PreIncrement(value)
        | ConstExpr::PreDecrement(value)
        | ConstExpr::AddrOf(value)
        | ConstExpr::Deref(value)
        | ConstExpr::VaArg { ap: value, .. } => walk_const_expr(value, visit),
        ConstExpr::Binary { left, right, .. }
        | ConstExpr::Assign {
            target: left,
            value: right,
            ..
        }
        | ConstExpr::Comma(left, right)
        | ConstExpr::Index {
            base: left,
            index: right,
        }
        | ConstExpr::Elvis {
            condition: left,
            else_value: right,
        } => {
            walk_const_expr(left, visit);
            walk_const_expr(right, visit);
        }
        ConstExpr::Ternary {
            condition,
            then_value,
            else_value,
        } => {
            walk_const_expr(condition, visit);
            walk_const_expr(then_value, visit);
            walk_const_expr(else_value, visit);
        }
        ConstExpr::Call { callee, arguments } => {
            walk_const_expr(callee, visit);
            for argument in arguments {
                walk_const_expr(argument, visit);
            }
        }
        ConstExpr::CompoundLiteral { initializer, .. } => {
            for item in initializer {
                walk_initializer(&item.value, visit);
            }
        }
        ConstExpr::Integer(_)
        | ConstExpr::WideInteger(_)
        | ConstExpr::Float(_)
        | ConstExpr::Identifier(_)
        | ConstExpr::StringLit(_)
        | ConstExpr::Utf8StringLit(_)
        | ConstExpr::Utf16StringLit(_)
        | ConstExpr::Utf32StringLit(_)
        | ConstExpr::WideStringLit(_)
        | ConstExpr::SizeOfType { .. }
        | ConstExpr::AlignOf { .. }
        | ConstExpr::OffsetOf { .. }
        | ConstExpr::TypesCompatible { .. }
        | ConstExpr::LabelAddr(_) => {}
    }
}

fn check_function_asm(function: &FunctionDecl, errors: &mut Vec<SemaError>) {
    let mut labels = HashMap::new();
    let mut scope = Vec::new();
    let mut next_scope = 0;
    walk_stmts(&function.body, &mut |node| match node {
        BodyNode::Stmt(stmt) => {
            if let Stmt::Labeled(name) = &stmt.value {
                labels.insert(name.as_str(), scope.clone());
            }
        }
        BodyNode::Tokens(tokens) => {
            for pair in tokens.windows(2) {
                if let (Token::Ident(name), Token::Colon) = (&pair[0].value, &pair[1].value) {
                    labels.insert(name.as_str(), scope.clone());
                }
            }
        }
        BodyNode::EnterJumpScope => {
            scope.push(next_scope);
            next_scope += 1;
        }
        BodyNode::ExitJumpScope => {
            scope.pop();
        }
    });
    scope.clear();
    next_scope = 0;
    walk_stmts(&function.body, &mut |node| match node {
        BodyNode::Stmt(stmt) => match &stmt.value {
            Stmt::Asm(asm) => check_asm_operands(
                asm,
                &labels,
                &scope,
                function.provenance,
                stmt.expansion,
                errors,
            ),
            Stmt::Decl(declaration) => check_register_variable(
                declaration,
                false,
                function.provenance,
                stmt.expansion,
                errors,
            ),
            _ => {}
        },
        BodyNode::EnterJumpScope => {
            scope.push(next_scope);
            next_scope += 1;
        }
        BodyNode::ExitJumpScope => {
            scope.pop();
        }
        BodyNode::Tokens(_) => {}
    });
}

fn check_asm_operands(
    asm: &GnuAsm,
    labels: &HashMap<&str, Vec<usize>>,
    scope: &[usize],
    provenance: Provenance,
    asm_loc: Loc,
    errors: &mut Vec<SemaError>,
) {
    let Some(operands) = &asm.operands else {
        return;
    };
    if let Some((loc, message)) = asm_operand_error(operands) {
        errors.push(error(provenance, loc, message));
    }
    let mut invalid_jump_scope = false;
    for label in &operands.labels {
        match labels.get(label.value.as_str()) {
            None => errors.push(error(
                provenance,
                label.expansion,
                format!("use of undeclared label '{}'", label.value),
            )),
            Some(label_scope) if !scope.starts_with(label_scope) => invalid_jump_scope = true,
            Some(_) => {}
        }
    }
    if invalid_jump_scope {
        errors.push(error(
            provenance,
            asm_loc,
            "cannot jump from this asm goto statement to one of its possible targets",
        ));
    }
}

fn asm_operand_error(operands: &AsmOperands) -> Option<(Loc, String)> {
    for output in &operands.outputs {
        match output_lvalue(&output.expr.value) {
            OutputLvalue::Valid => {}
            OutputLvalue::Cast => {
                return Some((
                    output.expr.expansion,
                    "invalid use of a cast in an inline asm context requiring an lvalue".into(),
                ));
            }
            OutputLvalue::Invalid => {
                return Some((output.expr.expansion, "invalid lvalue in asm output".into()));
            }
        }
    }
    let mut expected = None;
    for operand in operands.outputs.iter().chain(&operands.inputs) {
        let count = operand.constraint.alternatives.len();
        match expected {
            None => expected = Some(count),
            Some(expected) if expected != count => {
                return Some((
                    operand.expr.expansion,
                    format!(
                        "asm constraint has an unexpected number of alternatives: {expected} vs {count}"
                    ),
                ));
            }
            Some(_) => {}
        }
    }
    None
}

enum OutputLvalue {
    Valid,
    Cast,
    Invalid,
}

fn output_lvalue(expr: &Expr) -> OutputLvalue {
    match expr {
        Expr::Const(value) => const_output_lvalue(value),
        Expr::Identifier(_)
        | Expr::StringLit(_)
        | Expr::Utf8StringLit(_)
        | Expr::Utf16StringLit(_)
        | Expr::Utf32StringLit(_)
        | Expr::WideStringLit(_) => OutputLvalue::Valid,
        _ => OutputLvalue::Invalid,
    }
}

fn const_output_lvalue(expr: &ConstExpr) -> OutputLvalue {
    match expr {
        ConstExpr::Identifier(_)
        | ConstExpr::StringLit(_)
        | ConstExpr::Utf8StringLit(_)
        | ConstExpr::Utf16StringLit(_)
        | ConstExpr::Utf32StringLit(_)
        | ConstExpr::WideStringLit(_)
        | ConstExpr::Deref(_)
        | ConstExpr::Index { .. }
        | ConstExpr::Arrow { .. }
        | ConstExpr::CompoundLiteral { .. }
        | ConstExpr::Generic { .. } => OutputLvalue::Valid,
        ConstExpr::Member { base, .. } => match const_output_lvalue(base) {
            OutputLvalue::Valid => OutputLvalue::Valid,
            OutputLvalue::Cast | OutputLvalue::Invalid => OutputLvalue::Invalid,
        },
        ConstExpr::Cast { value, .. } | ConstExpr::BitCast { value, .. } => {
            match const_output_lvalue(value) {
                OutputLvalue::Invalid => OutputLvalue::Invalid,
                OutputLvalue::Valid | OutputLvalue::Cast => OutputLvalue::Cast,
            }
        }
        _ => OutputLvalue::Invalid,
    }
}

fn check_register_variable(
    declaration: &Declaration,
    file_scope: bool,
    provenance: Provenance,
    loc: Loc,
    errors: &mut Vec<SemaError>,
) {
    if !matches!(declaration.specifiers.storage, StorageClass::Register) {
        return;
    }
    let Some(label) = &declaration.asm_label else {
        if file_scope {
            errors.push(error(
                provenance,
                loc,
                "illegal storage class on file-scoped variable",
            ));
        }
        return;
    };
    let AsmLabel::Register(register) = &label.value else {
        return;
    };
    match register {
        Register::Other(name) => {
            if name.is_empty() {
                errors.push(error(
                    provenance,
                    label.expansion,
                    "cannot use an empty string literal in 'asm'",
                ));
            }
            errors.push(error(
                provenance,
                label.expansion,
                format!("unknown register name '{name}' in asm"),
            ));
        }
        Register::X86(x86) if file_scope && !matches!(x86.spelling.as_str(), "rsp" | "rbp") => {
            errors.push(error(
                provenance,
                label.expansion,
                format!(
                    "register '{}' unsuitable for global register variables on this target",
                    x86.spelling
                ),
            ));
        }
        Register::X86(_) => {}
    }
    if file_scope && !is_register_variable_type(declaration) {
        errors.push(error(
            provenance,
            loc,
            "unsupported type for named register variable",
        ));
    }
}

fn is_register_variable_type(declaration: &Declaration) -> bool {
    let mut declarator = &declaration.declarator;
    while let Declarator::Grouped(inner) | Declarator::Attributed { inner, .. } = declarator {
        declarator = inner;
    }
    match declarator {
        Declarator::Pointer { .. } => true,
        Declarator::Array { .. } | Declarator::Function { .. } => false,
        Declarator::Name(_) | Declarator::Abstract => {
            is_register_scalar_type(&declaration.specifiers.ty)
        }
        Declarator::Grouped(_) | Declarator::Attributed { .. } => unreachable!(),
    }
}

fn is_register_scalar_type(ty: &CType) -> bool {
    match ty {
        CType::Qualified { ty, .. } | CType::Atomic(ty) => is_register_scalar_type(ty),
        CType::Void
        | CType::Floating(_)
        | CType::Complex(_)
        | CType::Imaginary(_)
        | CType::FixedPoint(_)
        | CType::Vector(_)
        | CType::Array { .. }
        | CType::Function { .. }
        | CType::Tagged {
            kind: TagKind::Struct | TagKind::Union,
            ..
        } => false,
        _ => true,
    }
}
