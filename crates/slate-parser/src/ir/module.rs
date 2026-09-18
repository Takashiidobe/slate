use super::{
    AbiSignature, BindingId, Global, Parameters, Place, Type, TypeDefinition, Value, Variable,
};
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
    pub abi: AbiSignature,
    pub linkage: Linkage,
    pub body: Option<Vec<Span<Statement>>>,
    pub fallthrough: Option<Fallthrough>,
}

#[derive(Debug, Clone, Copy)]
pub enum Fallthrough {
    ReturnZero,
    ReturnVoid,
    UndefinedIfUsed,
}

#[derive(Debug, Clone)]
pub struct Evaluation {
    pub statements: Vec<Span<Statement>>,
    pub value: Value,
}

impl From<Value> for Evaluation {
    fn from(value: Value) -> Self {
        Self {
            statements: Vec::new(),
            value,
        }
    }
}

#[derive(Debug, Clone)]
pub enum Statement {
    Temporary {
        id: BindingId,
        ty: Type,
        initializer: Option<Value>,
    },
    Let(Variable),
    Write {
        place: Place,
        value: Value,
    },
    Expression(Value),
    Return(Option<Value>),
    Block(Vec<Span<Statement>>),
    Null,
    If {
        condition: Value,
        then_body: Vec<Span<Statement>>,
        else_body: Option<Vec<Span<Statement>>>,
    },
    While {
        id: BindingId,
        condition: Evaluation,
        body: Vec<Span<Statement>>,
    },
    DoWhile {
        id: BindingId,
        body: Vec<Span<Statement>>,
        condition: Evaluation,
    },
    For {
        id: BindingId,
        init: Vec<Span<Statement>>,
        condition: Option<Evaluation>,
        increment: Option<Evaluation>,
        body: Vec<Span<Statement>>,
    },
    Switch {
        id: BindingId,
        discriminant: Value,
        body: Vec<Span<Statement>>,
    },
    Case {
        switch: BindingId,
        start: Value,
        end: Option<Value>,
        body: Vec<Span<Statement>>,
    },
    Default {
        switch: BindingId,
        body: Vec<Span<Statement>>,
    },
    Break(BindingId),
    Continue(BindingId),
    Goto(BindingId),
    ComputedGoto(Value),
    Label {
        id: BindingId,
        name: String,
        body: Vec<Span<Statement>>,
    },
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
