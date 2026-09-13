use crate::ast::*;
use crate::const_expr::ConstExpr;
use crate::lexer::Token;
use std::collections::{HashMap, HashSet};

pub fn filter_translation_unit(tu: &TranslationUnit, root_file: FileId) -> TranslationUnit {
    let mut reachability = Reachability::new(tu);
    reachability.mark_roots(root_file);
    TranslationUnit {
        decls: tu
            .decls
            .iter()
            .enumerate()
            .filter(|(id, _)| reachability.reachable.contains(id))
            .map(|(_, decl)| decl.clone())
            .collect(),
        flavor: tu.flavor,
    }
}

struct Reachability<'a> {
    nodes: &'a [SpannedDecl],
    symbols: HashMap<String, Vec<usize>>,
    reachable: HashSet<usize>,
}

impl<'a> Reachability<'a> {
    fn new(tu: &'a TranslationUnit) -> Self {
        let mut symbols: HashMap<String, Vec<usize>> = HashMap::new();
        for (id, decl) in tu.decls.iter().enumerate() {
            for name in decl.names() {
                symbols.entry(name.to_string()).or_default().push(id);
            }
            if let Decl::Enum(enumeration) = &decl.value {
                for enumerator in &enumeration.enumerators {
                    symbols.entry(enumerator.name.clone()).or_default().push(id);
                }
            }
        }
        Self {
            nodes: &tu.decls,
            symbols,
            reachable: HashSet::new(),
        }
    }

    fn mark_roots(&mut self, root_file: FileId) {
        let roots = self
            .nodes
            .iter()
            .enumerate()
            .filter_map(|(id, decl)| (decl.provenance() == root_file).then_some(id))
            .collect::<Vec<_>>();
        for id in roots {
            self.mark(id);
        }
    }

    fn mark(&mut self, id: usize) {
        if !self.reachable.insert(id) {
            return;
        }
        match &self.nodes[id].value {
            Decl::Comment(_) | Decl::StaticAssert { .. } | Decl::Asm { .. } => {}
            Decl::Function(function) => self.mark_function(function),
            Decl::Declaration { declaration, .. } => self.mark_declaration(declaration),
            Decl::Record(record) => {
                for field in &record.fields {
                    if let FieldItem::Field(field) = &field.value {
                        self.mark_type(&field.specifiers.ty);
                        for declarator in &field.declarators {
                            self.mark_declarator(&declarator.declarator);
                        }
                    }
                }
            }
            Decl::Enum(enumeration) => {
                for value in enumeration
                    .enumerators
                    .iter()
                    .filter_map(|e| e.value.as_ref())
                {
                    self.mark_expr(value);
                }
            }
        }
    }

    fn mark_function(&mut self, function: &FunctionDecl) {
        self.mark_type(&function.ret_type);
        for parameter in &function.parameters {
            self.mark_parameter(parameter);
        }
        self.mark_stmts(&function.body);
    }

    fn mark_parameter(&mut self, parameter: &Parameter) {
        self.mark_type(&parameter.ty);
        if let Some(declarator) = &parameter.declarator {
            self.mark_declarator(declarator);
        }
    }

    fn mark_declaration(&mut self, declaration: &Declaration) {
        self.mark_type(&declaration.specifiers.ty);
        for declarator in &declaration.declarators {
            self.mark_declarator(&declarator.declarator);
            if let Some(initializer) = &declarator.initializer {
                self.mark_initializer(initializer);
            }
        }
    }

    fn mark_type_name(&mut self, ty: &CType, declarator: &Declarator) {
        self.mark_type(ty);
        self.mark_declarator(declarator);
    }

    fn mark_initializer(&mut self, initializer: &Initializer) {
        match initializer {
            Initializer::Expr(expr) => self.mark_expr(expr),
            Initializer::List(items) => {
                for item in items {
                    self.mark_initializer(&item.value);
                }
            }
        }
    }

    fn mark_stmts(&mut self, stmts: &[SpannedStmt]) {
        for stmt in stmts {
            self.mark_stmt(stmt);
        }
    }

    fn mark_stmt(&mut self, stmt: &SpannedStmt) {
        match &stmt.value {
            Stmt::Return(expr) | Stmt::Expr(expr) | Stmt::Case(expr) | Stmt::ComputedGoto(expr) => {
                self.mark_expr(expr)
            }
            Stmt::CaseRange { start, end } => {
                self.mark_expr(start);
                self.mark_expr(end);
            }
            Stmt::Decl(declaration) => self.mark_declaration(declaration),
            Stmt::Block(body) => self.mark_stmts(body),
            Stmt::If {
                condition,
                then_branch,
                else_branch,
            } => {
                self.mark_expr(condition);
                self.mark_stmts(then_branch);
                if let Some(else_branch) = else_branch {
                    self.mark_stmts(else_branch);
                }
            }
            Stmt::While { condition, body }
            | Stmt::DoWhile { body, condition }
            | Stmt::Switch {
                discriminant: condition,
                body,
            } => {
                self.mark_expr(condition);
                self.mark_stmts(body);
            }
            Stmt::For {
                init,
                condition,
                increment,
                body,
            } => {
                if let Some(init) = init {
                    self.mark_stmt(init);
                }
                for expr in condition.iter().chain(increment) {
                    self.mark_expr(expr);
                }
                self.mark_stmts(body);
            }
            Stmt::NestedFunction(function) => self.mark_function(function),
            Stmt::Comment(_)
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

    fn mark_expr(&mut self, expr: &SpannedExpr) {
        match &expr.value {
            Expr::Const(value) => self.mark_const_expr(value),
            Expr::Identifier(name) => self.mark_name(name),
            Expr::Unary { value, .. } | Expr::SizeOf(value) => self.mark_expr(value),
            Expr::Binary { left, right, .. } => {
                self.mark_expr(left);
                self.mark_expr(right);
            }
            Expr::StatementExpression(body) => self.mark_stmts(body),
            Expr::IntLit(_)
            | Expr::StringLit(_)
            | Expr::Utf8StringLit(_)
            | Expr::Utf16StringLit(_)
            | Expr::Utf32StringLit(_)
            | Expr::WideStringLit(_) => {}
        }
    }

    fn mark_const_expr(&mut self, expr: &ConstExpr) {
        match expr {
            ConstExpr::Identifier(name) => self.mark_name(name),
            ConstExpr::Generic {
                controlling,
                associations,
            } => {
                self.mark_const_expr(controlling);
                for association in associations {
                    if let Some(type_name) = &association.type_name {
                        self.mark_name(type_name);
                    }
                    self.mark_const_expr(&association.expression);
                }
            }
            ConstExpr::SizeOfType { ty, declarator }
            | ConstExpr::AlignOf { ty, declarator }
            | ConstExpr::OffsetOf { ty, declarator, .. } => self.mark_type_name(ty, declarator),
            ConstExpr::TypesCompatible {
                left_ty,
                left_declarator,
                right_ty,
                right_declarator,
            } => {
                self.mark_type_name(left_ty, left_declarator);
                self.mark_type_name(right_ty, right_declarator);
            }
            ConstExpr::Cast {
                ty,
                declarator,
                value,
            }
            | ConstExpr::BitCast {
                ty,
                declarator,
                value,
            }
            | ConstExpr::VaArg {
                ap: value,
                ty,
                declarator,
            } => {
                self.mark_type_name(ty, declarator);
                self.mark_const_expr(value);
            }
            ConstExpr::CompoundLiteral {
                ty,
                declarator,
                initializer,
            } => {
                self.mark_type_name(ty, declarator);
                for item in initializer {
                    self.mark_initializer(&item.value);
                }
            }
            ConstExpr::SizeOf(value)
            | ConstExpr::Unary { value, .. }
            | ConstExpr::Member { base: value, .. }
            | ConstExpr::Arrow { base: value, .. }
            | ConstExpr::PostIncrement(value)
            | ConstExpr::PostDecrement(value)
            | ConstExpr::PreIncrement(value)
            | ConstExpr::PreDecrement(value)
            | ConstExpr::AddrOf(value)
            | ConstExpr::Deref(value) => self.mark_const_expr(value),
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
                self.mark_const_expr(left);
                self.mark_const_expr(right);
            }
            ConstExpr::Ternary {
                condition,
                then_value,
                else_value,
            } => {
                self.mark_const_expr(condition);
                self.mark_const_expr(then_value);
                self.mark_const_expr(else_value);
            }
            ConstExpr::Call { callee, arguments } => {
                self.mark_const_expr(callee);
                for argument in arguments {
                    self.mark_const_expr(argument);
                }
            }
            ConstExpr::StatementExpression(tokens) => {
                for token in tokens {
                    if let Token::Ident(name) = &token.value {
                        self.mark_name(name);
                    }
                }
            }
            ConstExpr::Integer(_)
            | ConstExpr::WideInteger(_)
            | ConstExpr::Float(_)
            | ConstExpr::StringLit(_)
            | ConstExpr::Utf8StringLit(_)
            | ConstExpr::Utf16StringLit(_)
            | ConstExpr::Utf32StringLit(_)
            | ConstExpr::WideStringLit(_)
            | ConstExpr::LabelAddr(_) => {}
        }
    }

    fn mark_name(&mut self, name: &str) {
        let ids = self.symbols.get(name).cloned().unwrap_or_default();
        for id in ids {
            self.mark(id);
        }
    }

    fn mark_type(&mut self, ty: &CType) {
        match ty {
            CType::Named(name) => self.mark_name(name),
            CType::Tagged { name, .. } => {
                if let Some(name) = name {
                    self.mark_name(name);
                }
            }
            CType::Qualified { ty, .. } | CType::Pointer { pointee: ty, .. } => self.mark_type(ty),
            CType::Atomic(ty) => self.mark_type(ty),
            CType::Vector(vector) => self.mark_type(&vector.element),
            CType::TypeOf(TypeOfOperand::Type(ty))
            | CType::TypeOfUnqual(TypeOfOperand::Type(ty)) => self.mark_type(ty),
            CType::Imaginary(ty) => self.mark_type(ty),
            CType::Array { element, .. } => self.mark_type(element),
            CType::Function {
                return_type,
                parameters,
                ..
            } => {
                self.mark_type(return_type);
                for parameter in parameters {
                    self.mark_type(&parameter.ty);
                }
            }
            CType::Void
            | CType::Bool
            | CType::Integer(_)
            | CType::Floating(_)
            | CType::Complex(_) => {}
            CType::FixedPoint(_) => {}
            CType::TypeOf(TypeOfOperand::Expression(expr))
            | CType::TypeOfUnqual(TypeOfOperand::Expression(expr)) => self.mark_expr(expr),
            CType::TargetBuiltin(_) => {}
        }
    }

    fn mark_declarator(&mut self, declarator: &Declarator) {
        match declarator {
            Declarator::Abstract | Declarator::Name(_) => {}
            Declarator::Grouped(inner)
            | Declarator::Attributed { inner, .. }
            | Declarator::Pointer { inner, .. } => self.mark_declarator(inner),
            Declarator::Array { inner, size } => {
                self.mark_declarator(inner);
                if let ArraySize::Expression(size) = size {
                    self.mark_expr(size);
                }
            }
            Declarator::Function {
                inner, parameters, ..
            } => {
                self.mark_declarator(inner);
                for parameter in parameters {
                    self.mark_parameter(parameter);
                }
            }
        }
    }
}
