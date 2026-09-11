use crate::ast::*;
use crate::eval::Env;
use std::io::{self, Write};

pub struct Renderer<W> {
    out: W,
}

impl<W: Write> Renderer<W> {
    pub fn new(out: W) -> Self {
        Self { out }
    }

    pub fn render(&mut self, ast: &TranslationUnit, env: &Env) -> io::Result<()> {
        let concrete = ast.eval(env);
        self.line("polyvariant:")?;
        self.render_decls(&ast.decls, "", "decl")?;
        self.line("concrete:")?;
        self.render_concrete_decls(&concrete.decls, "", "decl")
    }

    pub fn into_inner(self) -> W {
        self.out
    }

    fn render_decls(&mut self, decls: &[Decl], indent: &str, label: &str) -> io::Result<()> {
        for (index, decl) in decls.iter().enumerate() {
            match decl {
                Decl::Function(function) => {
                    self.line(&format!(
                        "{indent}{label}[{index}]: function name={} return={}{}",
                        function.name,
                        Self::type_name(&function.ret_type),
                        Self::function_suffix(
                            function.qualifiers,
                            function.storage,
                            function.is_inline
                        ),
                    ))?;
                    self.render_stmts(&function.body, &format!("{indent}  "))?;
                }
                Decl::Declaration { declaration, .. } => self.line(&format!(
                    "{indent}{label}[{index}]: declaration type={}{} declarator={}",
                    Self::type_name(&declaration.specifiers.ty),
                    Self::specifier_suffix(&declaration.specifiers),
                    Self::render_declarator(&declaration.declarator)
                ))?,
                Decl::Typedef { name, ty, .. } => self.line(&format!(
                    "{indent}{label}[{index}]: typedef name={name} type={}",
                    Self::type_name(ty)
                ))?,
                Decl::Record(record) => {
                    self.line(&format!(
                        "{indent}{label}[{index}]: {} name={}",
                        Self::tag_name(record.kind),
                        record.name.as_deref().unwrap_or("<anonymous>")
                    ))?;
                    for field in &record.fields {
                        self.line(&format!(
                            "{indent}  field: type={} declarator={}",
                            Self::type_name(&field.declaration.specifiers.ty),
                            Self::render_declarator(&field.declaration.declarator)
                        ))?;
                    }
                }
                Decl::Enum(enumeration) => {
                    self.line(&format!(
                        "{indent}{label}[{index}]: enum name={}",
                        enumeration.name.as_deref().unwrap_or("<anonymous>")
                    ))?;
                    for enumerator in &enumeration.enumerators {
                        self.line(&format!(
                            "{indent}  enumerator: name={} value={}",
                            enumerator.name,
                            Self::enumerator_value(enumerator.value.as_ref())
                        ))?;
                    }
                }
                Decl::Conditional(conditional) => {
                    self.line(&format!("{indent}{label}[{index}]: conditional"))?;
                    self.render_conditional_decls(conditional, &format!("{indent}  "))?;
                }
            }
        }
        Ok(())
    }

    fn render_conditional_decls(
        &mut self,
        conditional: &Conditional<Vec<Decl>>,
        indent: &str,
    ) -> io::Result<()> {
        for (index, (condition, decls)) in conditional.branches.iter().enumerate() {
            self.line(&format!(
                "{indent}branch[{index}]: when={}",
                Self::condition_name(condition)
            ))?;
            self.render_decls(decls, &format!("{indent}  "), "decl")?;
        }
        Ok(())
    }

    fn render_stmts(&mut self, stmts: &[Stmt], indent: &str) -> io::Result<()> {
        for (index, stmt) in stmts.iter().enumerate() {
            match stmt {
                Stmt::Return(Expr::IntLit(value)) => {
                    self.line(&format!("{indent}stmt[{index}]: return {value}"))?;
                }
                Stmt::Conditional(conditional) => {
                    self.line(&format!("{indent}stmt[{index}]: conditional"))?;
                    for (branch_index, (condition, branch)) in
                        conditional.branches.iter().enumerate()
                    {
                        self.line(&format!(
                            "{indent}  branch[{branch_index}]: when={}",
                            Self::condition_name(condition)
                        ))?;
                        self.render_stmts(branch, &format!("{indent}    "))?;
                    }
                }
            }
        }
        Ok(())
    }

    fn render_concrete_decls(
        &mut self,
        decls: &[ConcreteDecl],
        indent: &str,
        label: &str,
    ) -> io::Result<()> {
        for (index, decl) in decls.iter().enumerate() {
            match decl {
                ConcreteDecl::Function(function) => {
                    self.line(&format!(
                        "{indent}{label}[{index}]: function name={} return={}{}",
                        function.name,
                        Self::type_name(&function.ret_type),
                        Self::function_suffix(
                            function.qualifiers,
                            function.storage,
                            function.is_inline
                        )
                    ))?;
                    for (stmt_index, stmt) in function.body.iter().enumerate() {
                        match stmt {
                            ConcreteStmt::Return(Expr::IntLit(value)) => {
                                self.line(&format!("{indent}  stmt[{stmt_index}]: return {value}"))?
                            }
                        }
                    }
                }
                ConcreteDecl::Declaration { declaration, .. } => self.line(&format!(
                    "{indent}{label}[{index}]: declaration type={}{} declarator={}",
                    Self::type_name(&declaration.specifiers.ty),
                    Self::specifier_suffix(&declaration.specifiers),
                    Self::render_declarator(&declaration.declarator)
                ))?,
                ConcreteDecl::Typedef { name, ty, .. } => self.line(&format!(
                    "{indent}{label}[{index}]: typedef name={name} type={}",
                    Self::type_name(ty)
                ))?,
                ConcreteDecl::Record(record) => {
                    self.line(&format!(
                        "{indent}{label}[{index}]: {} name={}",
                        Self::tag_name(record.kind),
                        record.name.as_deref().unwrap_or("<anonymous>")
                    ))?;
                }
                ConcreteDecl::Enum(enumeration) => self.line(&format!(
                    "{indent}{label}[{index}]: enum name={}",
                    enumeration.name.as_deref().unwrap_or("<anonymous>")
                ))?,
            }
        }
        Ok(())
    }

    fn line(&mut self, text: &str) -> io::Result<()> {
        writeln!(self.out, "{text}")
    }

    fn condition_name(condition: &Condition) -> String {
        match condition {
            Condition::Defined(name) => format!("defined({name})"),
            Condition::Not(inner) => format!("not({})", Self::condition_name(inner)),
        }
    }

    fn type_name(ty: &CType) -> String {
        match ty {
            CType::Void => "void".into(),
            CType::Bool => "bool".into(),
            CType::Char => "char".into(),
            CType::Int => "int".into(),
            CType::Named(name) => name.clone(),
            CType::Tagged { kind, name } => format!(
                "{} {}",
                Self::tag_name(*kind),
                name.as_deref().unwrap_or("<anonymous>")
            ),
            CType::Qualified { ty, .. } => format!("qualified({})", Self::type_name(ty)),
            CType::Pointer { pointee, .. } => format!("ptr({})", Self::type_name(pointee)),
            CType::Array { element, size } => format!(
                "array({},size={})",
                Self::type_name(element),
                Self::array_size_name(size)
            ),
            CType::Function {
                return_type,
                parameters,
                variadic,
            } => format!(
                "function({},params={},variadic={variadic})",
                Self::type_name(return_type),
                parameters
                    .iter()
                    .map(Self::parameter_name)
                    .collect::<Vec<_>>()
                    .join(",")
            ),
        }
    }

    fn specifier_suffix(specifiers: &DeclarationSpecifiers) -> String {
        let mut parts = Vec::new();
        let qualifiers = Self::qualifier_name(specifiers.qualifiers);
        if !qualifiers.is_empty() {
            parts.push(format!("qualifiers={qualifiers}"));
        }
        if specifiers.storage != StorageClass::None {
            let storage: &'static str = specifiers.storage.into();
            parts.push(format!("storage={storage}"));
        }
        if specifiers.is_inline {
            parts.push("inline=true".into());
        }
        if parts.is_empty() {
            String::new()
        } else {
            format!(" [{}]", parts.join(","))
        }
    }

    fn function_suffix(qualifiers: Qualifiers, storage: StorageClass, is_inline: bool) -> String {
        let mut parts = Vec::new();
        let qualifier_name = Self::qualifier_name(qualifiers);
        if !qualifier_name.is_empty() {
            parts.push(format!("qualifiers={qualifier_name}"));
        }
        if storage != StorageClass::None {
            let storage_name: &'static str = storage.into();
            parts.push(format!("storage={storage_name}"));
        }
        if is_inline {
            parts.push("inline=true".into());
        }
        if parts.is_empty() {
            String::new()
        } else {
            format!(" [{}]", parts.join(","))
        }
    }

    fn qualifier_name(qualifiers: Qualifiers) -> String {
        [
            (qualifiers.is_const, "const"),
            (qualifiers.is_volatile, "volatile"),
            (qualifiers.is_restrict, "restrict"),
            (qualifiers.is_atomic, "_Atomic"),
        ]
        .into_iter()
        .filter_map(|(enabled, name)| enabled.then_some(name))
        .collect::<Vec<_>>()
        .join(" ")
    }

    fn tag_name(kind: TagKind) -> &'static str {
        match kind {
            TagKind::Struct => "struct",
            TagKind::Union => "union",
            TagKind::Enum => "enum",
        }
    }

    fn enumerator_value(value: Option<&Expr>) -> String {
        match value {
            None => "implicit".into(),
            Some(Expr::IntLit(value)) => value.to_string(),
        }
    }

    fn array_size_name(size: &ArraySize) -> String {
        match size {
            ArraySize::Unspecified => "unspecified".into(),
            ArraySize::Expression(value) => match value.as_ref() {
                Expr::IntLit(value) => value.to_string(),
            },
        }
    }

    fn render_declarator(declarator: &Declarator) -> String {
        match declarator {
            Declarator::Abstract => "_".into(),
            Declarator::Name(name) => format!("name={name}"),
            Declarator::Grouped(inner) => format!("group({})", Self::render_declarator(inner)),
            Declarator::Pointer { qualifiers, inner } => {
                let qualifier = Self::qualifier_name(*qualifiers);
                if qualifier.is_empty() {
                    format!("pointer({})", Self::render_declarator(inner))
                } else {
                    format!(
                        "pointer(qualifiers={qualifier};{})",
                        Self::render_declarator(inner)
                    )
                }
            }
            Declarator::Array { inner, size } => format!(
                "array({},size={})",
                Self::render_declarator(inner),
                Self::array_size_name(size)
            ),
            Declarator::Function {
                inner,
                parameters,
                variadic,
            } => format!(
                "function({},params={},variadic={variadic})",
                Self::render_declarator(inner),
                parameters
                    .iter()
                    .map(Self::parameter_name)
                    .collect::<Vec<_>>()
                    .join(",")
            ),
        }
    }

    fn parameter_name(parameter: &Parameter) -> String {
        match &parameter.declarator {
            None => Self::type_name(&parameter.ty),
            Some(Declarator::Name(name)) => format!("{} {name}", Self::type_name(&parameter.ty)),
            Some(declarator) => Self::parameter_declarator_name(&parameter.ty, declarator),
        }
    }

    fn parameter_declarator_name(ty: &CType, declarator: &Declarator) -> String {
        match declarator {
            Declarator::Abstract => Self::type_name(ty),
            Declarator::Name(name) => format!("{} {name}", Self::type_name(ty)),
            Declarator::Grouped(inner) => Self::parameter_declarator_name(ty, inner),
            Declarator::Pointer { inner, .. } => {
                format!("ptr({})", Self::parameter_declarator_name(ty, inner))
            }
            Declarator::Array { inner, size } => format!(
                "array({},size={})",
                Self::parameter_declarator_name(ty, inner),
                Self::array_size_name(size)
            ),
            Declarator::Function { inner, .. } => {
                format!("function({})", Self::parameter_declarator_name(ty, inner))
            }
        }
    }
}
