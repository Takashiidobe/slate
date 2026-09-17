use super::{BindingId, Type, Value};
use crate::ast::Span;
use crate::target_info::StorageLayout;
use custom_debug::Debug as CustomDebug;

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
    Function {
        return_type: Option<Type>,
        parameters: Vec<Type>,
        variadic: bool,
        prototyped: bool,
    },
    Record {
        kind: RecordKind,
        fields: Option<Vec<Span<Field>>>,
        layout: Option<RecordLayout>,
    },
    Enum {
        underlying: Option<Type>,
        enumerators: Option<Vec<Span<Enumerator>>>,
        layout: Option<StorageLayout>,
    },
}

#[derive(Debug, Clone, Copy)]
pub enum RecordKind {
    Struct,
    Union,
}

#[derive(CustomDebug, Clone)]
pub struct RecordLayout {
    pub size: u64,
    pub align: u64,
    pub offsets: Vec<u64>,
    #[debug(skip_if = all_none)]
    pub bit_offsets: Vec<Option<u64>>,
    #[debug(skip_if = Vec::is_empty)]
    pub bit_units: Vec<BitFieldUnit>,
    #[debug(skip_if = all_none)]
    pub field_units: Vec<Option<usize>>,
}

fn all_none<T>(values: &[Option<T>]) -> bool {
    values.iter().all(Option::is_none)
}

#[derive(Debug, Clone)]
pub struct BitFieldUnit {
    pub offset: u64,
    pub size: u64,
}

#[derive(CustomDebug, Clone)]
pub struct Field {
    pub name: Option<String>,
    pub ty: Type,
    #[debug(skip_if = Option::is_none)]
    pub bit_width: Option<u32>,
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
    pub kind: PlaceKind,
}

#[derive(Debug, Clone)]
pub enum PlaceKind {
    Binding(BindingId),
    Deref(Box<Value>),
    Field { base: Box<Place>, index: usize },
    Index { base: Box<Value>, index: Box<Value> },
}

impl std::fmt::Display for Place {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match &self.kind {
            PlaceKind::Binding(id) => write!(f, "%{}", id.0),
            PlaceKind::Deref(value) => write!(f, "deref({value})"),
            PlaceKind::Field { base, index } => write!(f, "field{index}({base})"),
            PlaceKind::Index { base, index } => write!(f, "index({base}, {index})"),
        }
    }
}
