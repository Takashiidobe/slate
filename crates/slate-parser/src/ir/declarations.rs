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
    #[debug(skip_if = Access::is_plain)]
    pub access: Access,
    #[debug(skip_if = Option::is_none)]
    pub bit_width: Option<u32>,
}

#[derive(Debug, Clone, Copy, Default, PartialEq, Eq, Hash)]
pub struct Access {
    pub volatile: bool,
    pub atomic: bool,
}

impl Access {
    pub fn is_plain(&self) -> bool {
        *self == Self::default()
    }

    pub fn union(self, other: Self) -> Self {
        Self {
            volatile: self.volatile || other.volatile,
            atomic: self.atomic || other.atomic,
        }
    }

    pub fn prefix(self) -> &'static str {
        match (self.volatile, self.atomic) {
            (false, false) => "",
            (true, false) => "volatile ",
            (false, true) => "atomic ",
            (true, true) => "volatile atomic ",
        }
    }
}

impl std::fmt::Display for Access {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        if self.volatile {
            f.write_str(", volatile")?;
        }
        Ok(())
    }
}

#[derive(Debug, Clone)]
pub struct Enumerator {
    pub id: BindingId,
    pub name: String,
    pub value: Value,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
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
    pub restrict: bool,
    pub is_const: bool,
    pub access: Access,
    pub constexpr: bool,
    pub alignment: Option<u64>,
    pub initializer: Option<Value>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum AggregateTarget {
    Field(usize),
    Index(u64),
    Range { start: u64, end: u64 },
}

#[derive(Debug, Clone)]
pub struct AggregateMember {
    pub target: AggregateTarget,
    pub value: super::Value,
}

impl std::fmt::Display for AggregateTarget {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::Field(index) => write!(f, "field{index}"),
            Self::Index(index) => write!(f, "index{index}"),
            Self::Range { start, end } => write!(f, "index{start}..={end}"),
        }
    }
}

#[derive(Debug, Clone)]
pub struct Global {
    pub variable: Variable,
    pub linkage: super::Linkage,
    pub symbol: super::SymbolAttributes,
    pub definition: bool,
    pub common: bool,
}

#[derive(Debug, Clone)]
pub struct Parameter {
    pub id: BindingId,
    pub name: Option<String>,
    pub ty: Type,
    pub restrict: bool,
    pub is_const: bool,
    pub access: Access,
    pub array: Option<ArrayParameter>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct ArrayParameter {
    pub extent: ArrayExtent,
    pub guaranteed: bool,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ArrayExtent {
    Unspecified,
    Fixed(u64),
    Variable(super::VariableExtent),
}

impl std::fmt::Display for ArrayParameter {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.write_str("[array")?;
        if self.guaranteed {
            f.write_str("=static")?;
        } else if !matches!(self.extent, ArrayExtent::Unspecified) {
            f.write_str("=")?;
        }
        match self.extent {
            ArrayExtent::Unspecified => Ok(()),
            ArrayExtent::Fixed(length) => {
                write!(f, "{}{length}", if self.guaranteed { " " } else { "" })
            }
            ArrayExtent::Variable(super::VariableExtent::Captured(binding)) => {
                write!(
                    f,
                    "{}%{}",
                    if self.guaranteed { " " } else { "" },
                    binding.0
                )
            }
            ArrayExtent::Variable(super::VariableExtent::Unspecified) => {
                write!(f, "{}*", if self.guaranteed { " " } else { "" })
            }
        }?;
        f.write_str("]")
    }
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
    pub access: Access,
}

#[derive(Debug, Clone)]
pub enum PlaceKind {
    Binding(BindingId),
    Deref(Box<Value>),
    CompoundLiteral {
        object: BindingId,
        storage: StorageDuration,
        initializer: Box<Value>,
    },
    ComplexPart {
        base: Box<Place>,
        imaginary: bool,
    },
    Field {
        base: Box<Place>,
        index: usize,
        bits: Option<BitFieldAccess>,
    },
    Index {
        base: Box<Value>,
        index: Box<Value>,
    },
    Lane {
        base: Box<Place>,
        index: Box<Value>,
    },
    Swizzle {
        base: Box<Place>,
        lanes: Vec<u32>,
    },
}

#[derive(Debug, Clone)]
pub struct BitFieldAccess {
    pub unit: usize,
    pub unit_offset: u64,
    pub unit_size: u64,
    pub bit_offset: u64,
    pub width: u32,
}

impl Place {
    pub fn implicit_ordering(&self) -> Option<super::MemoryOrder> {
        self.access.atomic.then_some(super::MemoryOrder::SeqCst)
    }

    pub(super) fn display_mode(&self, compact: bool) -> impl std::fmt::Display + '_ {
        struct DisplayPlace<'a>(&'a Place, bool);

        impl std::fmt::Display for DisplayPlace<'_> {
            fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
                self.0.format(f, self.1)
            }
        }

        DisplayPlace(self, compact)
    }

    fn format(&self, f: &mut std::fmt::Formatter<'_>, compact: bool) -> std::fmt::Result {
        match &self.kind {
            PlaceKind::Binding(id) => write!(f, "%{}", id.0),
            PlaceKind::Deref(value) => write!(
                f,
                "deref({})",
                value.display_metadata(false, None).with_compact(compact)
            ),
            PlaceKind::CompoundLiteral {
                object,
                storage,
                initializer,
            } => write!(
                f,
                "compound_literal %{} [storage={}] = {}",
                object.0,
                match storage {
                    StorageDuration::Automatic => "automatic",
                    StorageDuration::Static => "static",
                    StorageDuration::Thread => "thread",
                },
                initializer
                    .display_metadata(false, None)
                    .with_compact(compact)
            ),
            PlaceKind::ComplexPart { base, imaginary } => {
                f.write_str(if *imaginary { "imag(" } else { "real(" })?;
                base.format(f, compact)?;
                f.write_str(")")
            }
            PlaceKind::Index { base, index } => write!(
                f,
                "index({}, {})",
                base.display_metadata(false, None).with_compact(compact),
                index.display_metadata(false, None).with_compact(compact)
            ),
            PlaceKind::Lane { base, index } => {
                f.write_str("lane(")?;
                base.format(f, compact)?;
                write!(
                    f,
                    ", {})",
                    index.display_metadata(false, None).with_compact(compact)
                )
            }
            PlaceKind::Swizzle { base, lanes } => {
                let lanes: Vec<String> = lanes.iter().map(u32::to_string).collect();
                write!(f, "swizzle<lanes=[{}]>(", lanes.join(", "))?;
                base.format(f, compact)?;
                f.write_str(")")
            }
            PlaceKind::Field { base, index, bits } => {
                match bits {
                    Some(bits) => write!(
                        f,
                        "bitfield{index}<unit={}, bytes={}..{}, bits={}..{}>(",
                        bits.unit,
                        bits.unit_offset,
                        bits.unit_offset + bits.unit_size,
                        bits.bit_offset,
                        bits.bit_offset + u64::from(bits.width)
                    )?,
                    None => write!(f, "field{index}(")?,
                }
                base.format(f, compact)?;
                f.write_str(")")
            }
        }
    }
}

impl std::fmt::Display for Place {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        self.format(f, false)
    }
}
