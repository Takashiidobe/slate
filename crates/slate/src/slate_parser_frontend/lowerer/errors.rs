use slate_parser::ast::{Loc, NodeId, Span};
use slate_parser::ir::{BindingId, TypeId};
use thiserror::Error;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct Site {
    pub node: NodeId,
    pub spelling: Loc,
    pub expansion: Loc,
}

impl Site {
    pub fn of<T>(span: &Span<T>) -> Self {
        Self {
            node: span.id,
            spelling: span.spelling,
            expansion: span.expansion,
        }
    }
}

impl std::fmt::Display for Site {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(
            f,
            "#{} file {} offset {}",
            self.node.0, self.expansion.file.0, self.expansion.offset
        )
    }
}

#[derive(Debug, Clone, Error)]
pub enum Construct {
    #[error("module assembly")]
    ModuleAsm,
    #[error("global {name}: {detail}")]
    Global { name: String, detail: String },
    #[error("function {name}: {detail}")]
    Function { name: String, detail: String },
    #[error("statement {ir}")]
    Statement { kind: String, ir: String },
    #[error("value {ir}")]
    Value { kind: String, ir: String },
    #[error("place {ir}")]
    Place { ir: String },
    #[error("type {ty}")]
    Type { ty: String },
    #[error("record {name}: {detail}")]
    Record { name: String, detail: String },
    #[error("switch: {detail}")]
    Switch { detail: String },
    #[error("long double: {detail}")]
    LongDouble { detail: String },
}

impl Construct {
    pub fn kind(&self) -> &str {
        match self {
            Self::ModuleAsm => "module-asm",
            Self::Global { .. } => "global",
            Self::Function { .. } => "function",
            Self::Statement { kind, .. } | Self::Value { kind, .. } => kind,
            Self::Place { .. } => "place",
            Self::Type { .. } => "type",
            Self::Record { .. } => "record",
            Self::Switch { .. } => "switch",
            Self::LongDouble { .. } => "long-double",
        }
    }
}

#[derive(Debug, Clone, Error)]
pub enum Invariant {
    #[error("unknown callee %{}", .0.0)]
    UnknownCallee(BindingId),
    #[error("unresolved type @type{}", .0.0)]
    UnresolvedType(TypeId),
}

#[derive(Debug, Clone, Error)]
#[error("{}{construct} at {site}", function_prefix(.function))]
pub struct Barrier {
    pub function: Option<String>,
    pub construct: Construct,
    pub site: Site,
}

#[derive(Debug, Clone, Error)]
#[error("{}{invariant} at {site}", function_prefix(.function))]
pub struct InvalidIr {
    pub function: Option<String>,
    pub invariant: Invariant,
    pub site: Site,
}

fn function_prefix(function: &Option<String>) -> String {
    function
        .as_ref()
        .map(|function| format!("{function}: "))
        .unwrap_or_default()
}

#[derive(Debug, Clone)]
pub(super) enum Kind {
    Unsupported(Construct),
    Invalid(Invariant),
}

#[derive(Debug, Clone)]
pub(super) struct Failure {
    kind: Kind,
    site: Option<Site>,
}

impl Failure {
    pub(super) fn at(mut self, site: Site) -> Self {
        self.site.get_or_insert(site);
        self
    }

    pub(super) fn into_public(
        self,
        function: Option<&str>,
        fallback: Site,
    ) -> Result<Barrier, InvalidIr> {
        let function = function.map(str::to_owned);
        let site = self.site.unwrap_or(fallback);
        match self.kind {
            Kind::Unsupported(construct) => Ok(Barrier {
                function,
                construct,
                site,
            }),
            Kind::Invalid(invariant) => Err(InvalidIr {
                function,
                invariant,
                site,
            }),
        }
    }
}

impl From<Construct> for Failure {
    fn from(construct: Construct) -> Self {
        Self {
            kind: Kind::Unsupported(construct),
            site: None,
        }
    }
}

impl From<Invariant> for Failure {
    fn from(invariant: Invariant) -> Self {
        Self {
            kind: Kind::Invalid(invariant),
            site: None,
        }
    }
}

pub(super) fn variant_name(value: &impl std::fmt::Debug) -> String {
    format!("{value:?}")
        .chars()
        .take_while(char::is_ascii_alphanumeric)
        .collect()
}
