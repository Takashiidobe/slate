use crate::ast::*;

pub trait Visitor {
    type Error;

    fn visit_translation_unit(&mut self, unit: &TranslationUnit) -> Result<(), Self::Error> {
        walk_translation_unit(self, unit)
    }

    fn visit_decl(&mut self, decl: &Decl) -> Result<(), Self::Error> {
        walk_decl(self, decl)
    }

    fn visit_function(&mut self, function: &FunctionDefinition) -> Result<(), Self::Error> {
        walk_function(self, function)
    }

    fn visit_declaration(&mut self, declaration: &Declaration) -> Result<(), Self::Error> {
        walk_declaration(self, declaration)
    }

    fn visit_stmt(&mut self, stmt: &Stmt) -> Result<(), Self::Error> {
        walk_stmt(self, stmt)
    }

    fn visit_expr(&mut self, expr: &Expr) -> Result<(), Self::Error> {
        walk_expr(self, expr)
    }

    fn visit_initializer(&mut self, initializer: &Initializer) -> Result<(), Self::Error> {
        walk_initializer(self, initializer)
    }

    fn visit_initializer_item(&mut self, item: &InitializerItem) -> Result<(), Self::Error> {
        walk_initializer_item(self, item)
    }

    fn visit_type_name(&mut self, ty: &TypeName) -> Result<(), Self::Error> {
        walk_type_name(self, ty)
    }

    fn visit_type_specifier(&mut self, ty: &TypeSpecifier) -> Result<(), Self::Error> {
        walk_type_specifier(self, ty)
    }

    fn visit_declarator(&mut self, declarator: &Declarator) -> Result<(), Self::Error> {
        walk_declarator(self, declarator)
    }

    fn visit_attribute(&mut self, attribute: &Attribute) -> Result<(), Self::Error> {
        walk_attribute(self, attribute)
    }

    fn visit_tag_definition(&mut self, tag: &Span<TagDefinition>) -> Result<(), Self::Error> {
        walk_tag_definition(self, tag)
    }
}

pub fn walk_translation_unit<V: Visitor + ?Sized>(
    visitor: &mut V,
    unit: &TranslationUnit,
) -> Result<(), V::Error> {
    for tag in &unit.tags {
        visitor.visit_tag_definition(tag)?;
    }
    for decl in &unit.decls {
        visitor.visit_decl(decl)?;
    }
    Ok(())
}

pub fn walk_decl<V: Visitor + ?Sized>(visitor: &mut V, decl: &Decl) -> Result<(), V::Error> {
    match &decl.value {
        DeclKind::Function(function) => visitor.visit_function(function),
        DeclKind::Declaration(declaration) => visitor.visit_declaration(declaration),
        DeclKind::StaticAssert(assertion) => visitor.visit_expr(&assertion.condition),
        DeclKind::Asm(asm) => walk_asm(visitor, asm),
        DeclKind::Attribute(attributes) => walk_attributes(visitor, attributes),
        DeclKind::Comment(_) | DeclKind::Pragma(_) => Ok(()),
    }
}

pub fn walk_function<V: Visitor + ?Sized>(
    visitor: &mut V,
    function: &FunctionDefinition,
) -> Result<(), V::Error> {
    walk_specifiers(visitor, &function.specifiers)?;
    visitor.visit_declarator(&function.declarator)?;
    walk_attributes(visitor, &function.attributes)?;
    walk_stmts(visitor, &function.body)
}

pub fn walk_declaration<V: Visitor + ?Sized>(
    visitor: &mut V,
    declaration: &Declaration,
) -> Result<(), V::Error> {
    walk_specifiers(visitor, &declaration.specifiers)?;
    for init in &declaration.declarators {
        visitor.visit_declarator(&init.declarator)?;
        walk_attributes(visitor, &init.attributes)?;
        if let Some(initializer) = &init.initializer {
            visitor.visit_initializer(initializer)?;
        }
    }
    Ok(())
}

pub fn walk_stmts<V: Visitor + ?Sized>(visitor: &mut V, stmts: &[Stmt]) -> Result<(), V::Error> {
    for stmt in stmts {
        visitor.visit_stmt(stmt)?;
    }
    Ok(())
}

pub fn walk_stmt<V: Visitor + ?Sized>(visitor: &mut V, stmt: &Stmt) -> Result<(), V::Error> {
    match &stmt.value {
        StmtKind::Return(expr) | StmtKind::Expr(expr) | StmtKind::ComputedGoto(expr) => {
            visitor.visit_expr(expr)
        }
        StmtKind::Decl(declaration) => visitor.visit_declaration(declaration),
        StmtKind::StaticAssert(assertion) => visitor.visit_expr(&assertion.condition),
        StmtKind::Attribute(attributes) => walk_attributes(visitor, attributes),
        StmtKind::Attributed { attributes, body } => {
            walk_attributes(visitor, attributes)?;
            visitor.visit_stmt(body)
        }
        StmtKind::Block(body) => walk_stmts(visitor, body),
        StmtKind::If {
            condition,
            then_branch,
            else_branch,
        } => {
            visitor.visit_expr(condition)?;
            visitor.visit_stmt(then_branch)?;
            if let Some(branch) = else_branch {
                visitor.visit_stmt(branch)?;
            }
            Ok(())
        }
        StmtKind::IfDeclaration {
            declaration,
            condition,
            then_branch,
            else_branch,
        } => {
            visitor.visit_declaration(declaration)?;
            if let Some(condition) = condition {
                visitor.visit_expr(condition)?;
            }
            visitor.visit_stmt(then_branch)?;
            if let Some(branch) = else_branch {
                visitor.visit_stmt(branch)?;
            }
            Ok(())
        }
        StmtKind::SwitchDeclaration {
            declaration,
            discriminant,
            body,
        } => {
            visitor.visit_declaration(declaration)?;
            if let Some(discriminant) = discriminant {
                visitor.visit_expr(discriminant)?;
            }
            visitor.visit_stmt(body)
        }
        StmtKind::While { condition, body }
        | StmtKind::Switch {
            discriminant: condition,
            body,
        } => {
            visitor.visit_expr(condition)?;
            visitor.visit_stmt(body)
        }
        StmtKind::DoWhile { body, condition } => {
            visitor.visit_stmt(body)?;
            visitor.visit_expr(condition)
        }
        StmtKind::For {
            init,
            condition,
            increment,
            body,
        } => {
            if let Some(init) = init {
                visitor.visit_stmt(init)?;
            }
            if let Some(condition) = condition {
                visitor.visit_expr(condition)?;
            }
            if let Some(increment) = increment {
                visitor.visit_expr(increment)?;
            }
            visitor.visit_stmt(body)
        }
        StmtKind::Labeled { body, .. } => visitor.visit_stmt(body),
        StmtKind::SwitchLabel { label, body } => {
            match label {
                SwitchLabel::Case(value) => visitor.visit_expr(value)?,
                SwitchLabel::CaseRange { start, end } => {
                    visitor.visit_expr(start)?;
                    visitor.visit_expr(end)?;
                }
                SwitchLabel::Default => {}
            }
            visitor.visit_stmt(body)
        }
        StmtKind::Asm(asm) => walk_asm(visitor, asm),
        StmtKind::NestedFunction(function) => visitor.visit_function(function),
        StmtKind::Comment(_)
        | StmtKind::Null
        | StmtKind::ReturnVoid
        | StmtKind::LocalLabelDecl(_)
        | StmtKind::MsAsm(_)
        | StmtKind::Goto(_)
        | StmtKind::Pragma(_)
        | StmtKind::NamedBreak(_)
        | StmtKind::NamedContinue(_)
        | StmtKind::Break
        | StmtKind::Continue => Ok(()),
    }
}

pub fn walk_expr<V: Visitor + ?Sized>(visitor: &mut V, expr: &Expr) -> Result<(), V::Error> {
    match &expr.value {
        ExprKind::Paren(value)
        | ExprKind::SizeOfExpr(value)
        | ExprKind::AlignOfExpr(value)
        | ExprKind::CountOfExpr(value)
        | ExprKind::Unary { operand: value, .. }
        | ExprKind::Postfix { operand: value, .. }
        | ExprKind::Member { base: value, .. } => visitor.visit_expr(value),
        ExprKind::Cast { ty, value }
        | ExprKind::BitCast { ty, value }
        | ExprKind::ConvertVector { ty, value }
        | ExprKind::VaArg { list: value, ty } => {
            visitor.visit_type_name(ty)?;
            visitor.visit_expr(value)
        }
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
            visitor.visit_expr(left)?;
            visitor.visit_expr(right)
        }
        ExprKind::Conditional {
            condition,
            then_value,
            else_value,
        } => {
            visitor.visit_expr(condition)?;
            if let Some(value) = then_value {
                visitor.visit_expr(value)?;
            }
            visitor.visit_expr(else_value)
        }
        ExprKind::Call { callee, arguments } => {
            visitor.visit_expr(callee)?;
            for argument in arguments {
                visitor.visit_expr(argument)?;
            }
            Ok(())
        }
        ExprKind::CompoundLiteral { ty, initializer } => {
            visitor.visit_type_name(ty)?;
            for item in initializer {
                visitor.visit_initializer_item(item)?;
            }
            Ok(())
        }
        ExprKind::SizeOfType { ty }
        | ExprKind::AlignOf { ty }
        | ExprKind::CountOfType { ty }
        | ExprKind::MaxOf { ty }
        | ExprKind::MinOf { ty } => visitor.visit_type_name(ty),
        ExprKind::OffsetOf { ty, member } => {
            visitor.visit_type_name(ty)?;
            visitor.visit_expr(member)
        }
        ExprKind::Generic {
            controlling,
            associations,
        } => {
            match controlling {
                GenericControl::Expr(value) => visitor.visit_expr(value)?,
                GenericControl::Type { ty } => visitor.visit_type_name(ty)?,
            }
            for association in associations {
                match association {
                    GenericAssociation::Type { ty, value } => {
                        visitor.visit_type_name(ty)?;
                        visitor.visit_expr(value)?;
                    }
                    GenericAssociation::Default(value) => visitor.visit_expr(value)?,
                }
            }
            Ok(())
        }
        ExprKind::TypesCompatible { left_ty, right_ty } => {
            visitor.visit_type_name(left_ty)?;
            visitor.visit_type_name(right_ty)
        }
        ExprKind::StatementExpression(body) => walk_stmts(visitor, body),
        ExprKind::StaticAssert(assertion) => visitor.visit_expr(&assertion.condition),
        ExprKind::Identifier(_)
        | ExprKind::IntegerLiteral(_)
        | ExprKind::FloatLiteral(_)
        | ExprKind::CharLiteral(_)
        | ExprKind::StringLiteral(_)
        | ExprKind::LabelAddress(_)
        | ExprKind::BoolLiteral(_)
        | ExprKind::NullPtrLiteral => Ok(()),
    }
}

pub fn walk_initializer<V: Visitor + ?Sized>(
    visitor: &mut V,
    initializer: &Initializer,
) -> Result<(), V::Error> {
    match initializer {
        Initializer::Expr(value) => visitor.visit_expr(value),
        Initializer::List(items) => {
            for item in items {
                visitor.visit_initializer_item(item)?;
            }
            Ok(())
        }
    }
}

pub fn walk_initializer_item<V: Visitor + ?Sized>(
    visitor: &mut V,
    item: &InitializerItem,
) -> Result<(), V::Error> {
    for designator in &item.designators {
        match designator {
            Designator::Array(value) => visitor.visit_expr(value)?,
            Designator::ArrayRange { start, end } => {
                visitor.visit_expr(start)?;
                visitor.visit_expr(end)?;
            }
            Designator::Field(_) => {}
        }
    }
    visitor.visit_initializer(&item.value)
}

pub fn walk_type_name<V: Visitor + ?Sized>(visitor: &mut V, ty: &TypeName) -> Result<(), V::Error> {
    walk_specifiers(visitor, &ty.specifiers)?;
    visitor.visit_declarator(&ty.declarator)
}

pub fn walk_specifiers<V: Visitor + ?Sized>(
    visitor: &mut V,
    specifiers: &DeclarationSpecifiers,
) -> Result<(), V::Error> {
    visitor.visit_type_specifier(&specifiers.ty)?;
    walk_attributes(visitor, &specifiers.attributes)
}

pub fn walk_type_specifier<V: Visitor + ?Sized>(
    visitor: &mut V,
    ty: &TypeSpecifier,
) -> Result<(), V::Error> {
    match ty {
        TypeSpecifier::Integer(IntegerType::BitInt { width, .. }) => visitor.visit_expr(width),
        TypeSpecifier::Complex(inner) | TypeSpecifier::Imaginary(inner) => {
            visitor.visit_type_specifier(inner)
        }
        TypeSpecifier::Atomic(ty)
        | TypeSpecifier::TypeOf(TypeOfOperand::Type(ty))
        | TypeSpecifier::TypeOfUnqual(TypeOfOperand::Type(ty)) => visitor.visit_type_name(ty),
        TypeSpecifier::TypeOf(TypeOfOperand::Expression(value))
        | TypeSpecifier::TypeOfUnqual(TypeOfOperand::Expression(value)) => {
            visitor.visit_expr(value)
        }
        TypeSpecifier::Mode(mode) => visitor.visit_type_specifier(&mode.base),
        TypeSpecifier::Vector(vector) => {
            visitor.visit_type_specifier(&vector.element)?;
            match &vector.size {
                VectorSize::Bytes(value) | VectorSize::Lanes(value) => visitor.visit_expr(value),
            }
        }
        TypeSpecifier::Tag(TagSpecifier::Reference {
            fixed_type: Some(ty),
            ..
        }) => visitor.visit_type_name(ty),
        TypeSpecifier::Void
        | TypeSpecifier::Bool
        | TypeSpecifier::Integer(_)
        | TypeSpecifier::Floating(_)
        | TypeSpecifier::FixedPoint(_)
        | TypeSpecifier::TargetBuiltin(_)
        | TypeSpecifier::Inferred
        | TypeSpecifier::Named(_)
        | TypeSpecifier::Tag(_) => Ok(()),
    }
}

pub fn walk_declarator<V: Visitor + ?Sized>(
    visitor: &mut V,
    declarator: &Declarator,
) -> Result<(), V::Error> {
    match declarator {
        Declarator::Grouped(inner) => visitor.visit_declarator(inner),
        Declarator::Attributed { inner, attributes }
        | Declarator::Pointer {
            inner, attributes, ..
        } => {
            visitor.visit_declarator(inner)?;
            walk_attributes(visitor, attributes)
        }
        Declarator::Array { inner, size, .. } => {
            visitor.visit_declarator(inner)?;
            if let ArraySize::Expression(value) = size {
                visitor.visit_expr(value)?;
            }
            Ok(())
        }
        Declarator::Function { inner, parameters } => {
            visitor.visit_declarator(inner)?;
            for parameter in parameters.parameters() {
                walk_specifiers(visitor, &parameter.specifiers)?;
                visitor.visit_declarator(&parameter.declarator)?;
                walk_attributes(visitor, &parameter.attributes)?;
            }
            Ok(())
        }
        Declarator::Abstract | Declarator::Name(_) => Ok(()),
    }
}

pub fn walk_attributes<V: Visitor + ?Sized>(
    visitor: &mut V,
    attributes: &[Span<Attribute>],
) -> Result<(), V::Error> {
    for attribute in attributes {
        visitor.visit_attribute(&attribute.value)?;
    }
    Ok(())
}

pub fn walk_attribute<V: Visitor + ?Sized>(
    visitor: &mut V,
    attribute: &Attribute,
) -> Result<(), V::Error> {
    match attribute {
        Attribute::AddressSpace(value)
        | Attribute::Aligned(value)
        | Attribute::VectorSize(value)
        | Attribute::AllocAlign(value)
        | Attribute::Cleanup(value)
        | Attribute::ExtVectorType(value)
        | Attribute::PassObjectSize {
            size_type: value, ..
        } => visitor.visit_expr(value),
        Attribute::AlignAs(AlignAsOperand::Expr(value)) => visitor.visit_expr(value),
        Attribute::AlignAs(AlignAsOperand::Type { ty }) => visitor.visit_type_name(ty),
        Attribute::Malloc {
            deallocator,
            argument,
        } => {
            for value in deallocator.iter().chain(argument) {
                visitor.visit_expr(value)?;
            }
            Ok(())
        }
        Attribute::AssumeAligned(values) | Attribute::AllocSize(values) => {
            for value in values {
                visitor.visit_expr(value)?;
            }
            Ok(())
        }
        Attribute::CallingConvention(CallingConvention::RegParm(value)) => {
            visitor.visit_expr(value)
        }
        Attribute::Packed
        | Attribute::LifetimeBound
        | Attribute::Overloadable
        | Attribute::GnuInline
        | Attribute::NoThrow
        | Attribute::SelectAny
        | Attribute::ThreadLocal
        | Attribute::NoAlias
        | Attribute::RestrictReturn
        | Attribute::CodeSeg(_)
        | Attribute::OptimizeNone
        | Attribute::Mode(_)
        | Attribute::Visibility(_)
        | Attribute::Section(_)
        | Attribute::Weak
        | Attribute::Used
        | Attribute::Retain
        | Attribute::NoInline
        | Attribute::AlwaysInline
        | Attribute::NoReturn
        | Attribute::Constructor(_)
        | Attribute::Destructor(_)
        | Attribute::NonNull(_)
        | Attribute::Annotate(_)
        | Attribute::Target(_)
        | Attribute::Alias(_)
        | Attribute::WeakRef(_)
        | Attribute::ReturnsNonNull
        | Attribute::WarnUnusedResult
        | Attribute::Sentinel(_)
        | Attribute::Cold
        | Attribute::Flatten
        | Attribute::Hot
        | Attribute::Leaf
        | Attribute::NoIpa
        | Attribute::NoClone
        | Attribute::Optimize(_)
        | Attribute::Naked
        | Attribute::Interrupt
        | Attribute::NoSplitStack
        | Attribute::ReturnsTwice
        | Attribute::CpuDispatch(_)
        | Attribute::CpuSpecific(_)
        | Attribute::TargetClones(_)
        | Attribute::Ifunc(_)
        | Attribute::DllImport
        | Attribute::DllExport
        | Attribute::WeakImport
        | Attribute::TlsModel(_)
        | Attribute::CountedBy(_)
        | Attribute::MsStruct
        | Attribute::CallingConvention(_)
        | Attribute::NoMips16
        | Attribute::Availability(_)
        | Attribute::ScalarStorageOrder(_)
        | Attribute::TransparentUnion
        | Attribute::Format(_)
        | Attribute::FormatArg(_)
        | Attribute::GccStruct
        | Attribute::Common
        | Attribute::NoCommon
        | Attribute::Pure
        | Attribute::Const
        | Attribute::MayAlias
        | Attribute::Deprecated(_)
        | Attribute::NoDiscard(_)
        | Attribute::MaybeUnused
        | Attribute::Fallthrough
        | Attribute::Unknown { .. }
        | Attribute::IgnoredDeclspec { .. }
        | Attribute::Invalid { .. } => Ok(()),
    }
}

pub fn walk_tag_definition<V: Visitor + ?Sized>(
    visitor: &mut V,
    tag: &Span<TagDefinition>,
) -> Result<(), V::Error> {
    walk_attributes(visitor, &tag.attributes)?;
    match &tag.body {
        TagBody::Record(fields) => {
            for item in fields {
                if let FieldItemKind::StaticAssert(assertion) = &item.value {
                    visitor.visit_expr(&assertion.condition)?;
                }
                if let FieldItemKind::Field(field) = &item.value {
                    walk_specifiers(visitor, &field.specifiers)?;
                    for declarator in &field.declarators {
                        visitor.visit_declarator(&declarator.declarator)?;
                        if let Some(width) = &declarator.bit_width {
                            visitor.visit_expr(width)?;
                        }
                        walk_attributes(visitor, &declarator.attributes)?;
                    }
                }
            }
        }
        TagBody::Enum {
            fixed_type,
            enumerators,
        } => {
            if let Some(ty) = fixed_type {
                visitor.visit_type_name(ty)?;
            }
            for item in enumerators {
                if let EnumItemKind::Enumerator(enumerator) = &item.value {
                    walk_attributes(visitor, &enumerator.attributes)?;
                    if let Some(value) = &enumerator.value {
                        visitor.visit_expr(value)?;
                    }
                }
            }
        }
    }
    Ok(())
}

fn walk_asm<V: Visitor + ?Sized>(visitor: &mut V, asm: &GnuAsm) -> Result<(), V::Error> {
    if let Some(operands) = &asm.operands {
        for operand in operands.outputs.iter().chain(&operands.inputs) {
            visitor.visit_expr(&operand.expr)?;
        }
    }
    Ok(())
}
