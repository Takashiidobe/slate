use crate::ast::*;
use crate::compiler_args::CompilerFlavor;
use crate::const_expr::{
    CharLiteral, Encoding, IntegerLiteral, IntegerSizeSuffix, Radix, UnaryOp, resolve_float,
};
use crate::files::{Files, decode_source_bytes, display_path};
use crate::target_info::TargetInfo;
use miette::{Diagnostic, NamedSource, SourceSpan};
use num_bigint::BigUint;
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
                Decl::Declaration { declaration, .. }
                    if declaration.specifiers.storage == StorageClass::Typedef =>
                {
                    Some(declaration.names().map(str::to_string))
                }
                _ => None,
            })
            .flatten()
            .collect::<HashSet<_>>();
        let mut tags = HashSet::new();
        for tag in &self.tags {
            if let Some(name) = &tag.value.name {
                tags.insert(name.clone());
            }
            if let TagBody::Record(fields) = &tag.value.body {
                for field_item in fields {
                    if let FieldItem::Field(field) = &field_item.value {
                        collect_tag_names(&field.specifiers.ty, &mut tags);
                    }
                }
            }
        }
        for decl in &self.decls {
            match &decl.value {
                Decl::Function(function) => {
                    collect_tag_names(&function.ret_type, &mut tags);
                    for parameter in &function.parameters {
                        collect_tag_names(&parameter.ty, &mut tags);
                    }
                }
                Decl::Declaration { declaration, .. } => {
                    collect_tag_names(&declaration.specifiers.ty, &mut tags);
                }
                Decl::Comment(_) | Decl::StaticAssert { .. } | Decl::Asm { .. } => {}
            }
        }

        let mut errors = Vec::new();
        for decl in &self.decls {
            match &decl.value {
                Decl::Comment(_) | Decl::StaticAssert { .. } | Decl::Asm { .. } => {}
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
                        check_function_asm(self, function, &mut errors);
                    }
                    check_literals(function, &self.target, &mut errors);
                }
                Decl::Declaration {
                    declaration,
                    provenance,
                } => {
                    let specifiers = &declaration.specifiers;
                    if let CType::Tag(TagSpecifier::Definition(id)) = &specifiers.ty
                        && let Some(tag) = self.tag(*id)
                    {
                        check_tag_definition(
                            &tag.value,
                            &typedefs,
                            &tags,
                            decl.expansion,
                            &mut errors,
                        );
                    }
                    check_type(
                        &specifiers.ty,
                        &typedefs,
                        &tags,
                        *provenance,
                        decl.expansion,
                        &mut errors,
                    );
                    check_attributes(
                        &specifiers.attributes,
                        *provenance,
                        decl.expansion,
                        &mut errors,
                    );
                    check_declaration_literals(declaration, &self.target, *provenance, &mut errors);
                    for init_declarator in &declaration.declarators {
                        let declarator = &init_declarator.declarator;
                        if matches!(specifiers.ty, CType::Void)
                            && !matches!(
                                specifiers.storage,
                                StorageClass::Extern | StorageClass::Typedef
                            )
                            && declarator.name().is_some()
                            && !declarator_indirects_void(declarator)
                        {
                            errors.push(error(
                                *provenance,
                                decl.expansion,
                                "object cannot have type void",
                            ));
                        }
                        check_declarator(
                            declarator,
                            &typedefs,
                            &tags,
                            *provenance,
                            decl.expansion,
                            &mut errors,
                        );
                        check_attributes(
                            &init_declarator.attributes,
                            *provenance,
                            decl.expansion,
                            &mut errors,
                        );
                        if flavor == CompilerFlavor::Clang {
                            check_register_variable(
                                self,
                                specifiers,
                                init_declarator,
                                true,
                                *provenance,
                                decl.expansion,
                                &mut errors,
                            );
                        }
                    }
                }
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

fn is_integer_constant_expression(expression: &Expr) -> bool {
    match &expression.value {
        ExprKind::IntegerLiteral(_)
        | ExprKind::CharLiteral(_)
        | ExprKind::SizeOfExpr(_)
        | ExprKind::SizeOfType { .. }
        | ExprKind::AlignOf { .. }
        | ExprKind::AlignOfExpr(_) => true,
        ExprKind::Unary { op, operand } => {
            matches!(
                op,
                UnaryOp::Plus
                    | UnaryOp::Minus
                    | UnaryOp::BitNot
                    | UnaryOp::Not
                    | UnaryOp::Real
                    | UnaryOp::Imag
            ) && is_integer_constant_expression(operand)
        }
        ExprKind::Paren(value) | ExprKind::Cast { value, .. } => {
            is_integer_constant_expression(value)
        }
        ExprKind::Binary { left, right, .. } => {
            is_integer_constant_expression(left) && is_integer_constant_expression(right)
        }
        ExprKind::Conditional {
            condition,
            then_value,
            else_value,
        } => {
            is_integer_constant_expression(condition)
                && then_value
                    .as_ref()
                    .is_none_or(is_integer_constant_expression)
                && is_integer_constant_expression(else_value)
        }
        ExprKind::Identifier(_)
        | ExprKind::StringLiteral(_)
        | ExprKind::Generic { .. }
        | ExprKind::FloatLiteral(_)
        | ExprKind::Call { .. }
        | ExprKind::Assign { .. }
        | ExprKind::Comma { .. }
        | ExprKind::Member { .. }
        | ExprKind::Index { .. }
        | ExprKind::OffsetOf { .. }
        | ExprKind::TypesCompatible { .. }
        | ExprKind::Postfix { .. }
        | ExprKind::CompoundLiteral { .. }
        | ExprKind::BitCast { .. }
        | ExprKind::VaArg { .. }
        | ExprKind::LabelAddress(_)
        | ExprKind::StatementExpression(_) => false,
    }
}

fn check_tag_definition(
    tag: &TagDefinition,
    typedefs: &HashSet<String>,
    tags: &HashSet<String>,
    loc: Loc,
    errors: &mut Vec<SemaError>,
) {
    check_attributes(&tag.attributes, tag.provenance, loc, errors);
    let TagBody::Record(fields) = &tag.body else {
        return;
    };
    for field_item in fields {
        let FieldItem::Field(field) = &field_item.value else {
            continue;
        };
        check_type(
            &field.specifiers.ty,
            typedefs,
            tags,
            field.provenance,
            field_item.expansion,
            errors,
        );
        let attributes = field.specifiers.attributes.iter().chain(
            field
                .declarators
                .iter()
                .flat_map(|declarator| &declarator.attributes),
        );
        check_attributes(
            &attributes.cloned().collect::<Vec<_>>(),
            field.provenance,
            field_item.expansion,
            errors,
        );
    }
}

fn collect_tag_names(ty: &CType, tags: &mut HashSet<String>) {
    match ty {
        CType::Tag(TagSpecifier::Reference { name, .. }) => {
            tags.insert(name.clone());
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
        | CType::Named(_)
        | CType::Tag(TagSpecifier::Definition(_)) => {}
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
        CType::Tag(TagSpecifier::Reference { name, .. }) if !tags.contains(name) => {
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
        | CType::Tag(_) => {}
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
    Expr(&'a Expr),
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
        Stmt::Return(expr) | Stmt::Expr(expr) | Stmt::ComputedGoto(expr) => walk_expr(expr, visit),
        Stmt::Labeled { body, .. } => walk_stmt(body, visit),
        Stmt::SwitchLabel { label, body } => {
            match label {
                SwitchLabel::Case(expr) => walk_expr(expr, visit),
                SwitchLabel::CaseRange { start, end } => {
                    walk_expr(start, visit);
                    walk_expr(end, visit);
                }
                SwitchLabel::Default => {}
            }
            walk_stmt(body, visit);
        }
        Stmt::Decl(declaration) => {
            for initializer in declaration
                .declarators
                .iter()
                .filter_map(|declarator| declarator.initializer.as_ref())
            {
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
        Stmt::NestedFunction(_)
        | Stmt::Comment(_)
        | Stmt::ReturnVoid
        | Stmt::StaticAssert(_)
        | Stmt::Attribute(_)
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

fn walk_expr<'a>(expr: &'a Expr, visit: &mut impl FnMut(BodyNode<'a>)) {
    visit(BodyNode::Expr(expr));
    match &expr.value {
        ExprKind::StatementExpression(body) => {
            visit(BodyNode::EnterJumpScope);
            walk_stmts(body, visit);
            visit(BodyNode::ExitJumpScope);
        }
        ExprKind::Generic {
            controlling,
            associations,
        } => {
            if let GenericControl::Expr(controlling) = controlling {
                walk_expr(controlling, visit);
            }
            for association in associations {
                let (GenericAssociation::Type { value, .. } | GenericAssociation::Default(value)) =
                    association;
                walk_expr(value, visit);
            }
        }
        ExprKind::Paren(value)
        | ExprKind::SizeOfExpr(value)
        | ExprKind::AlignOfExpr(value)
        | ExprKind::Unary { operand: value, .. }
        | ExprKind::Postfix { operand: value, .. }
        | ExprKind::Cast { value, .. }
        | ExprKind::BitCast { value, .. }
        | ExprKind::Member { base: value, .. }
        | ExprKind::VaArg { list: value, .. } => walk_expr(value, visit),
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
            walk_expr(left, visit);
            walk_expr(right, visit);
        }
        ExprKind::Conditional {
            condition,
            then_value,
            else_value,
        } => {
            walk_expr(condition, visit);
            if let Some(then_value) = then_value {
                walk_expr(then_value, visit);
            }
            walk_expr(else_value, visit);
        }
        ExprKind::Call { callee, arguments } => {
            walk_expr(callee, visit);
            for argument in arguments {
                walk_expr(argument, visit);
            }
        }
        ExprKind::CompoundLiteral { initializer, .. } => {
            for item in initializer {
                walk_initializer(&item.value, visit);
            }
        }
        ExprKind::IntegerLiteral(_)
        | ExprKind::FloatLiteral(_)
        | ExprKind::CharLiteral(_)
        | ExprKind::Identifier(_)
        | ExprKind::StringLiteral(_)
        | ExprKind::SizeOfType { .. }
        | ExprKind::AlignOf { .. }
        | ExprKind::OffsetOf { .. }
        | ExprKind::TypesCompatible { .. }
        | ExprKind::LabelAddress(_) => {}
    }
}

fn integer_rank_width(rank: IntegerRank, target: &TargetInfo) -> u32 {
    match rank {
        IntegerRank::Short => target.short_width,
        IntegerRank::Int => target.int_width,
        IntegerRank::Long => target.long_width,
        IntegerRank::LongLong => target.long_long_width,
        IntegerRank::Int128 => 128,
    }
}

fn fits_rank(value: &BigUint, width: u32, signed: bool) -> bool {
    let limit = BigUint::from(1u32) << (width - u32::from(signed));
    *value < limit
}

fn integer_candidates(literal: &IntegerLiteral) -> Vec<(IntegerRank, bool)> {
    use IntegerRank::{Int, Long, LongLong};
    let decimal = literal.radix == Radix::Decimal;
    match (literal.suffix.size, literal.suffix.unsigned) {
        (IntegerSizeSuffix::None, false) if decimal => {
            vec![(Int, true), (Long, true), (LongLong, true)]
        }
        (IntegerSizeSuffix::None, false) => vec![
            (Int, true),
            (Int, false),
            (Long, true),
            (Long, false),
            (LongLong, true),
            (LongLong, false),
        ],
        (IntegerSizeSuffix::None, true) => vec![(Int, false), (Long, false), (LongLong, false)],
        (IntegerSizeSuffix::Long, false) if decimal => vec![(Long, true), (LongLong, true)],
        (IntegerSizeSuffix::Long, false) => vec![
            (Long, true),
            (Long, false),
            (LongLong, true),
            (LongLong, false),
        ],
        (IntegerSizeSuffix::Long, true) => vec![(Long, false), (LongLong, false)],
        (IntegerSizeSuffix::LongLong, false) if decimal => vec![(LongLong, true)],
        (IntegerSizeSuffix::LongLong, false) => vec![(LongLong, true), (LongLong, false)],
        (IntegerSizeSuffix::LongLong, true) => vec![(LongLong, false)],
        (IntegerSizeSuffix::BitInt, _) => Vec::new(),
    }
}

fn resolve_integer_literal(literal: &IntegerLiteral, target: &TargetInfo) -> Result<(), String> {
    if literal.suffix.size == IntegerSizeSuffix::BitInt {
        return Ok(());
    }
    integer_candidates(literal)
        .into_iter()
        .any(|(rank, signed)| fits_rank(&literal.value, integer_rank_width(rank, target), signed))
        .then_some(())
        .ok_or_else(|| {
            format!(
                "integer literal `{}` is too large to be represented in any integer type",
                literal.spelling
            )
        })
}

fn char_literal_max(encoding: Encoding, target: &TargetInfo) -> u32 {
    match encoding {
        Encoding::Plain | Encoding::Utf8 => 0xFF,
        Encoding::Utf16 => 0xFFFF,
        Encoding::Utf32 => u32::MAX,
        Encoding::Wide => {
            if target.wchar_width >= 32 {
                u32::MAX
            } else {
                0xFFFF
            }
        }
    }
}

fn resolve_char_literal(literal: &CharLiteral, target: &TargetInfo) -> Result<(), String> {
    if literal.encoding == Encoding::Plain {
        return Ok(());
    }
    if let [unit] = literal.code_units.as_slice()
        && *unit > char_literal_max(literal.encoding, target)
    {
        return Err("character too large for enclosing character literal type".to_string());
    }
    Ok(())
}

fn check_literal_expr(expr: &Expr, target: &TargetInfo) -> Option<String> {
    match &expr.value {
        ExprKind::IntegerLiteral(literal) => resolve_integer_literal(literal, target).err(),
        ExprKind::CharLiteral(literal) => resolve_char_literal(literal, target).err(),
        ExprKind::FloatLiteral(literal) => {
            resolve_float(literal).err().map(|error| error.to_string())
        }
        _ => None,
    }
}

fn check_literals(function: &FunctionDecl, target: &TargetInfo, errors: &mut Vec<SemaError>) {
    walk_stmts(&function.body, &mut |node| {
        let BodyNode::Expr(expr) = node else {
            return;
        };
        if let Some(message) = check_literal_expr(expr, target) {
            errors.push(error(function.provenance, expr.expansion, message));
        }
    });
}

fn check_declaration_literals(
    declaration: &Declaration,
    target: &TargetInfo,
    provenance: Provenance,
    errors: &mut Vec<SemaError>,
) {
    for init_declarator in &declaration.declarators {
        let Some(initializer) = &init_declarator.initializer else {
            continue;
        };
        walk_initializer(initializer, &mut |node| {
            let BodyNode::Expr(expr) = node else {
                return;
            };
            if let Some(message) = check_literal_expr(expr, target) {
                errors.push(error(provenance, expr.expansion, message));
            }
        });
    }
}

fn check_function_asm(
    unit: &TranslationUnit,
    function: &FunctionDecl,
    errors: &mut Vec<SemaError>,
) {
    let mut labels = HashMap::new();
    let mut scope = Vec::new();
    let mut next_scope = 0;
    walk_stmts(&function.body, &mut |node| match node {
        BodyNode::Stmt(stmt) => {
            if let Stmt::Labeled { label: name, .. } = &stmt.value {
                labels.insert(name.as_str(), scope.clone());
            }
        }
        BodyNode::Expr(_) => {}
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
            Stmt::Decl(declaration) => {
                for declarator in &declaration.declarators {
                    check_register_variable(
                        unit,
                        &declaration.specifiers,
                        declarator,
                        false,
                        function.provenance,
                        stmt.expansion,
                        errors,
                    );
                }
            }
            _ => {}
        },
        BodyNode::Expr(_) => {}
        BodyNode::EnterJumpScope => {
            scope.push(next_scope);
            next_scope += 1;
        }
        BodyNode::ExitJumpScope => {
            scope.pop();
        }
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
    let mut operand_names = HashSet::new();
    for label in &operands.labels {
        if !operand_names.insert(label.value.as_str()) {
            errors.push(error(
                provenance,
                label.expansion,
                format!("duplicate use of asm operand name \"{}\"", label.value),
            ));
        }
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
        match output_lvalue(&output.expr) {
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
    match &expr.value {
        ExprKind::Identifier(_)
        | ExprKind::StringLiteral(_)
        | ExprKind::Unary {
            op: UnaryOp::Deref, ..
        }
        | ExprKind::Index { .. }
        | ExprKind::Member { arrow: true, .. }
        | ExprKind::CompoundLiteral { .. }
        | ExprKind::Generic { .. } => OutputLvalue::Valid,
        ExprKind::Paren(inner) => output_lvalue(inner),
        ExprKind::Member { base, .. } => match output_lvalue(base) {
            OutputLvalue::Valid => OutputLvalue::Valid,
            OutputLvalue::Cast | OutputLvalue::Invalid => OutputLvalue::Invalid,
        },
        ExprKind::Cast { value, .. } | ExprKind::BitCast { value, .. } => {
            match output_lvalue(value) {
                OutputLvalue::Invalid => OutputLvalue::Invalid,
                OutputLvalue::Valid | OutputLvalue::Cast => OutputLvalue::Cast,
            }
        }
        _ => OutputLvalue::Invalid,
    }
}

fn check_register_variable(
    unit: &TranslationUnit,
    specifiers: &DeclarationSpecifiers,
    declarator: &InitDeclarator,
    file_scope: bool,
    provenance: Provenance,
    loc: Loc,
    errors: &mut Vec<SemaError>,
) {
    if !matches!(specifiers.storage, StorageClass::Register) {
        return;
    }
    let Some(label) = &declarator.asm_label else {
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
    if file_scope && !is_register_variable_type(unit, specifiers, &declarator.declarator) {
        errors.push(error(
            provenance,
            loc,
            "unsupported type for named register variable",
        ));
    }
}

fn is_register_variable_type(
    unit: &TranslationUnit,
    specifiers: &DeclarationSpecifiers,
    declarator: &Declarator,
) -> bool {
    let mut declarator = declarator;
    while let Declarator::Grouped(inner) | Declarator::Attributed { inner, .. } = declarator {
        declarator = inner;
    }
    match declarator {
        Declarator::Pointer { .. } => true,
        Declarator::Array { .. } | Declarator::Function { .. } => false,
        Declarator::Name(_) | Declarator::Abstract => is_register_scalar_type(&specifiers.ty, unit),
        Declarator::Grouped(_) | Declarator::Attributed { .. } => unreachable!(),
    }
}

fn is_register_scalar_type(ty: &CType, unit: &TranslationUnit) -> bool {
    match ty {
        CType::Qualified { ty, .. } | CType::Atomic(ty) => is_register_scalar_type(ty, unit),
        CType::Void
        | CType::Floating(_)
        | CType::Complex(_)
        | CType::Imaginary(_)
        | CType::FixedPoint(_)
        | CType::Vector(_)
        | CType::Array { .. }
        | CType::Function { .. }
        | CType::Tag(TagSpecifier::Reference {
            kind: TagKind::Struct | TagKind::Union,
            ..
        }) => false,
        CType::Tag(TagSpecifier::Definition(id)) => unit
            .tag(*id)
            .is_none_or(|tag| tag.value.kind == TagKind::Enum),
        _ => true,
    }
}
