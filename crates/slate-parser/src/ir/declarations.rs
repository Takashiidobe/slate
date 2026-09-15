use super::{BindingId, Type, Value};
use crate::ast::Span;

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub struct TypeId(pub u32);

#[derive(Debug, Clone)]
pub struct TypeDefinition {
    pub id: TypeId,
    pub name: Option<String>,
    pub kind: TypeDefinitionKind,
}

#[derive(Debug, Clone)]
pub enum TypeDefinitionKind {
    Alias(Type),
    Pointer {
        pointee: Type,
        is_const: bool,
    },
    Array {
        element: Type,
        length: Option<u64>,
    },
    Record {
        kind: RecordKind,
        fields: Option<Vec<Span<Field>>>,
        layout: Option<RecordLayout>,
    },
    Enum {
        underlying: Option<Type>,
        enumerators: Option<Vec<Span<Enumerator>>>,
    },
}

#[derive(Debug, Clone, Copy)]
pub enum RecordKind {
    Struct,
    Union,
}

#[derive(Debug, Clone)]
pub struct RecordLayout {
    pub size: u64,
    pub align: u64,
    pub offsets: Vec<u64>,
}

#[derive(Debug, Clone)]
pub struct Field {
    pub name: Option<String>,
    pub ty: Type,
}

#[derive(Debug, Clone)]
pub struct Enumerator {
    pub id: BindingId,
    pub name: String,
    pub value: Value,
}

#[derive(Debug, Clone, Copy)]
pub enum StorageDuration {
    Automatic,
    Static,
    Thread,
}

#[derive(Debug, Clone)]
pub struct Variable {
    pub id: BindingId,
    pub name: String,
    pub ty: Type,
    pub storage: StorageDuration,
    pub initializer: Option<Value>,
}

#[derive(Debug, Clone)]
pub struct Global {
    pub variable: Variable,
    pub linkage: super::Linkage,
    pub definition: bool,
}

#[derive(Debug, Clone)]
pub struct Parameter {
    pub id: BindingId,
    pub name: Option<String>,
    pub ty: Type,
}

#[derive(Debug, Clone)]
pub enum Parameters {
    Prototype {
        fixed: Vec<Span<Parameter>>,
        variadic: bool,
    },
    Unprototyped,
}

#[derive(Debug, Clone)]
pub struct Place {
    pub ty: Type,
    pub binding: BindingId,
}
