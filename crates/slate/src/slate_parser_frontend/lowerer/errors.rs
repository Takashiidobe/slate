use slate_parser::ast::{Loc, NodeId, Span};
use slate_parser::files::Files;
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

impl Site {
    pub fn render(&self, files: &Files) -> String {
        let expansion = render_loc(self.expansion, files);
        match self.spelling(files) {
            Some(spelling) => format!("{expansion} (spelled at {spelling})"),
            None => expansion,
        }
    }

    fn contains(&self, other: &Site) -> bool {
        self.expansion.file == other.expansion.file
            && (self.expansion.offset..=self.expansion.offset + self.expansion.length)
                .contains(&other.expansion.offset)
    }

    pub fn spelling(&self, files: &Files) -> Option<String> {
        ((self.spelling.file, self.spelling.offset) != (self.expansion.file, self.expansion.offset))
            .then(|| render_loc(self.spelling, files))
    }
}

fn render_loc(loc: Loc, files: &Files) -> String {
    let path = files.get_path(loc.file).map_or_else(
        || format!("<file {}>", loc.file.0),
        |path| path.display().to_string(),
    );
    match files.position(loc.file, loc.offset) {
        Some((line, column)) => format!("{path}:{}:{}", line + 1, column + 1),
        None => format!("{path}@{}", loc.offset),
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
    #[error("return type {ty}")]
    Return { returns: Option<String>, ty: String },
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
            Self::Return { .. } => "return",
            Self::Record { .. } => "record",
            Self::Switch { .. } => "switch",
            Self::LongDouble { .. } => "long-double",
        }
    }

    pub fn label(&self) -> String {
        match self {
            Self::Return { returns, .. } => return_label(returns.as_deref()),
            construct => format!("cannot lower {} to Rust", construct.kind()),
        }
    }
}

fn return_label(returns: Option<&str>) -> String {
    match returns {
        Some(returns) => format!("cannot lower a function returning `{returns}`"),
        None => "return type could not be lowered to Rust".into(),
    }
}

#[derive(Debug, Clone, Error)]
pub enum Invariant {
    #[error("unknown callee %{}", .0.0)]
    UnknownCallee(BindingId),
    #[error("unresolved type @type{}", .0.0)]
    UnresolvedType(TypeId),
    #[error("non-constant case value")]
    NonConstantCase,
}

#[derive(Debug, Clone, Error)]
#[error("{}{construct}", function_prefix(.function))]
pub struct Barrier {
    pub function: Option<String>,
    pub construct: Construct,
    pub site: Site,
    pub context: Vec<Context>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Context {
    pub site: Site,
    pub label: String,
}

#[derive(Debug, Clone, Error)]
#[error("{}{invariant}", function_prefix(.function))]
pub struct InvalidIr {
    pub function: Option<String>,
    pub invariant: Invariant,
    pub site: Site,
    pub context: Vec<Context>,
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
pub(super) struct Failure(Box<Failed>);

#[derive(Debug, Clone)]
struct Failed {
    kind: Kind,
    site: Option<Site>,
    context: Vec<Context>,
    used: bool,
}

impl Failure {
    pub(super) fn at(mut self, site: Site) -> Self {
        self.0.site.get_or_insert(site);
        self
    }

    pub(super) fn used_at(mut self, site: Site) -> Self {
        match self.0.site {
            None => self.0.site = Some(site),
            Some(inner) if !self.0.used && !site.contains(&inner) => {
                self.0.used = true;
                self.0.context.push(Context {
                    site,
                    label: "used here".into(),
                });
            }
            Some(_) => {}
        }
        self
    }

    pub(super) fn returned_by(self, site: Site, returns: Option<String>) -> Self {
        match &self.0.kind {
            Kind::Unsupported(Construct::Type { ty }) if self.0.site.is_none() => {
                Self::from(Construct::Return {
                    returns,
                    ty: ty.clone(),
                })
                .at(site)
            }
            _ => {
                let label = return_label(returns.as_deref());
                self.at(site).within(site, label)
            }
        }
    }

    pub(super) fn within(mut self, site: Site, label: String) -> Self {
        self.0.context.push(Context { site, label });
        self
    }

    pub(super) fn into_public(
        self,
        function: Option<&str>,
        fallback: Site,
    ) -> Result<Barrier, InvalidIr> {
        let function = function.map(str::to_owned);
        let site = self.0.site.unwrap_or(fallback);
        let mut context = self.0.context;
        let mut seen = vec![site];
        context.retain(|context| {
            let first = !seen.contains(&context.site);
            seen.push(context.site);
            first
        });
        match self.0.kind {
            Kind::Unsupported(construct) => Ok(Barrier {
                function,
                construct,
                site,
                context,
            }),
            Kind::Invalid(invariant) => Err(InvalidIr {
                function,
                invariant,
                site,
                context,
            }),
        }
    }
}

impl From<Construct> for Failure {
    fn from(construct: Construct) -> Self {
        Self(Box::new(Failed {
            kind: Kind::Unsupported(construct),
            site: None,
            context: Vec::new(),
            used: false,
        }))
    }
}

impl From<Invariant> for Failure {
    fn from(invariant: Invariant) -> Self {
        Self(Box::new(Failed {
            kind: Kind::Invalid(invariant),
            site: None,
            context: Vec::new(),
            used: false,
        }))
    }
}

pub(super) fn variant_name(value: &impl std::fmt::Debug) -> String {
    format!("{value:?}")
        .chars()
        .take_while(char::is_ascii_alphanumeric)
        .collect()
}
