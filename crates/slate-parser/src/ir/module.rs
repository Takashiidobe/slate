use super::{BindingId, Global, Parameters, Place, Type, TypeDefinition, Value, Variable};
use crate::ast::{NodeId, Span};
use crate::target_info::TargetInfo;
use std::collections::HashMap;

pub type Metadata = HashMap<NodeId, Vec<(String, String)>>;

#[derive(Debug, Clone)]
pub struct Module {
    pub target: TargetInfo,
    pub types: Vec<Span<TypeDefinition>>,
    pub globals: Vec<Span<Global>>,
    pub functions: Vec<Span<Function>>,
    pub metadata: Metadata,
}

#[derive(Debug, Clone, Copy)]
pub enum Linkage {
    Internal,
    External,
}

#[derive(Debug, Clone)]
pub struct Function {
    pub id: BindingId,
    pub name: String,
    pub parameters: Parameters,
    pub return_type: Option<Type>,
    pub linkage: Linkage,
    pub body: Option<Vec<Span<Statement>>>,
}

#[derive(Debug, Clone)]
pub enum Statement {
    Let(Variable),
    Write { place: Place, value: Value },
    Expression(Value),
    Return(Option<Value>),
    Block(Vec<Span<Statement>>),
}

impl Module {
    pub fn new(target: TargetInfo) -> Self {
        Self {
            target,
            types: Vec::new(),
            globals: Vec::new(),
            functions: Vec::new(),
            metadata: Metadata::new(),
        }
    }
}
