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
                        "{indent}{label}[{index}]: function name={} return={}",
                        function.name,
                        Self::type_name(&function.ret_type)
                    ))?;
                    self.render_stmts(&function.body, &format!("{indent}  "))?;
                }
                Decl::Declaration { declaration, .. } => self.line(&format!(
                    "{indent}{label}[{index}]: declaration type={} declarator={}",
                    Self::type_name(&declaration.specifiers.ty),
                    Self::render_declarator(&declaration.declarator)
                ))?,
                Decl::Typedef { name, ty, .. } => self.line(&format!(
                    "{indent}{label}[{index}]: typedef name={name} type={}",
                    Self::type_name(ty)
                ))?,
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
                        "{indent}{label}[{index}]: function name={} return={}",
                        function.name,
                        Self::type_name(&function.ret_type)
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
                    "{indent}{label}[{index}]: declaration type={} declarator={}",
                    Self::type_name(&declaration.specifiers.ty),
                    Self::render_declarator(&declaration.declarator)
                ))?,
                ConcreteDecl::Typedef { name, ty, .. } => self.line(&format!(
                    "{indent}{label}[{index}]: typedef name={name} type={}",
                    Self::type_name(ty)
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

    fn tag_name(kind: TagKind) -> &'static str {
        match kind {
            TagKind::Struct => "struct",
            TagKind::Union => "union",
            TagKind::Enum => "enum",
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
            Declarator::Pointer { inner, .. } => {
                format!("pointer({})", Self::render_declarator(inner))
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
