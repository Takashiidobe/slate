use crate::ast::Span;
use std::fmt;

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub struct BindingId(pub u32);

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum BindingKind {
    Object,
    Function,
    Parameter,
    Typedef,
    Enumerator,
    Tag,
    Label,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Binding {
    pub id: BindingId,
    pub kind: BindingKind,
    pub name: String,
    pub display_name: String,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Reference {
    pub binding: BindingId,
    pub kind: BindingKind,
    pub name: String,
    pub display_name: String,
}

#[derive(Debug, Clone, Default)]
pub struct NameResolution {
    pub bindings: Vec<Span<Binding>>,
    pub references: Vec<Span<Reference>>,
}

impl fmt::Display for NameResolution {
    fn fmt(&self, formatter: &mut fmt::Formatter<'_>) -> fmt::Result {
        for binding in &self.bindings {
            writeln!(
                formatter,
                "bind {} {} = %{}",
                kind_name(binding.kind),
                binding.display_name,
                binding.value.id.0
            )?;
        }
        for reference in &self.references {
            writeln!(
                formatter,
                "ref {} {} -> %{}",
                kind_name(reference.kind),
                reference.display_name,
                reference.binding.0
            )?;
        }
        Ok(())
    }
}

fn kind_name(kind: BindingKind) -> &'static str {
    match kind {
        BindingKind::Object => "object",
        BindingKind::Function => "function",
        BindingKind::Parameter => "parameter",
        BindingKind::Typedef => "typedef",
        BindingKind::Enumerator => "enumerator",
        BindingKind::Tag => "tag",
        BindingKind::Label => "label",
    }
}
