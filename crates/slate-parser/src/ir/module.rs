use super::{Type, Value};
use crate::ast::Span;
use crate::target_info::TargetInfo;
use std::fmt;

#[derive(Debug, Clone)]
pub struct Module {
    pub target: TargetInfo,
    pub functions: Vec<Span<Function>>,
}

#[derive(Debug, Clone, Copy)]
pub enum Linkage {
    Internal,
    External,
}

#[derive(Debug, Clone)]
pub struct Function {
    pub name: String,
    pub return_type: Option<Type>,
    pub linkage: Linkage,
    pub body: Vec<Span<Statement>>,
}

#[derive(Debug, Clone)]
pub enum Statement {
    Expression(Value),
    Return(Option<Value>),
    Block(Vec<Span<Statement>>),
}

impl fmt::Display for Module {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        writeln!(f, "module {{")?;
        for function in &self.functions {
            let linkage = match function.linkage {
                Linkage::Internal => "internal",
                Linkage::External => "external",
            };
            write!(f, "    fn @{}() -> ", function.name)?;
            match function.return_type {
                Some(ty) => write!(f, "{ty}")?,
                None => f.write_str("void")?,
            }
            writeln!(f, " [linkage={linkage}] {{")?;
            statements(f, &function.body, 8)?;
            writeln!(f, "    }}")?;
        }
        writeln!(f, "}}")
    }
}

fn statements(f: &mut fmt::Formatter<'_>, body: &[Span<Statement>], indent: usize) -> fmt::Result {
    for statement in body {
        write!(f, "{:indent$}", "")?;
        match &statement.value {
            Statement::Expression(value) => writeln!(f, "{};", value.display(false))?,
            Statement::Return(Some(value)) => writeln!(f, "return {};", value.display(false))?,
            Statement::Return(None) => writeln!(f, "return;")?,
            Statement::Block(body) => {
                writeln!(f, "{{")?;
                statements(f, body, indent + 4)?;
                writeln!(f, "{:indent$}}}", "")?;
            }
        }
    }
    Ok(())
}
