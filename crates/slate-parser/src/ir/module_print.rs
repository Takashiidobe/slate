use super::{
    Linkage, Metadata, Module, Parameters, RecordKind, Statement, StorageDuration,
    TypeDefinitionKind, Variable,
};
use crate::ast::{NodeId, Span};
use std::fmt;

pub struct DisplayModule<'a> {
    module: &'a Module,
    show_metadata: bool,
}

impl Module {
    pub fn display(&self, show_metadata: bool) -> DisplayModule<'_> {
        DisplayModule {
            module: self,
            show_metadata,
        }
    }
}

impl fmt::Display for Linkage {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.write_str(match self {
            Self::Internal => "internal",
            Self::External => "external",
        })
    }
}

impl fmt::Display for Module {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        self.display(false).fmt(f)
    }
}

pub(super) fn metadata(
    f: &mut fmt::Formatter<'_>,
    table: Option<&Metadata>,
    id: NodeId,
) -> fmt::Result {
    if let Some(entries) = table.and_then(|table| table.get(&id)) {
        for (key, value) in entries {
            write!(f, " [{key}={value:?}]")?;
        }
    }
    Ok(())
}

impl DisplayModule<'_> {
    fn table(&self) -> Option<&Metadata> {
        self.show_metadata.then_some(&self.module.metadata)
    }

    fn variable(&self, f: &mut fmt::Formatter<'_>, variable: &Variable) -> fmt::Result {
        write!(
            f,
            "%{} {}: {} [storage={}]",
            variable.id.0,
            variable.name,
            variable.ty,
            match variable.storage {
                StorageDuration::Automatic => "automatic",
                StorageDuration::Static => "static",
                StorageDuration::Thread => "thread",
            }
        )?;
        if let Some(value) = &variable.initializer {
            write!(f, " = {}", value.display_metadata(false, self.table()))?;
        }
        Ok(())
    }

    fn statements(
        &self,
        f: &mut fmt::Formatter<'_>,
        body: &[Span<Statement>],
        indent: usize,
    ) -> fmt::Result {
        for statement in body {
            write!(f, "{:indent$}", "")?;
            match &statement.value {
                Statement::Let(variable) => {
                    f.write_str("let ")?;
                    self.variable(f, variable)?;
                }
                Statement::Write { place, value } => write!(
                    f,
                    "write<{}>(%{}, {})",
                    place.ty,
                    place.binding.0,
                    value.display_metadata(false, self.table())
                )?,
                Statement::Expression(value) => {
                    write!(f, "{}", value.display_metadata(false, self.table()))?
                }
                Statement::Return(Some(value)) => {
                    write!(f, "return {}", value.display_metadata(false, self.table()))?
                }
                Statement::Return(None) => f.write_str("return")?,
                Statement::Block(body) => {
                    f.write_str("{")?;
                    metadata(f, self.table(), statement.id)?;
                    writeln!(f)?;
                    self.statements(f, body, indent + 4)?;
                    writeln!(f, "{:indent$}}}", "")?;
                    continue;
                }
            }
            metadata(f, self.table(), statement.id)?;
            writeln!(f, ";")?;
        }
        Ok(())
    }
}

impl fmt::Display for DisplayModule<'_> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        writeln!(f, "module {{")?;
        let target = self.module.target;
        writeln!(
            f,
            "    target [char_signed={}, short_width={}, int_width={}, long_width={}, long_long_width={}, pointer_width={}, wchar_signed={}, wchar_width={}, long_double={:?}];",
            target.char_signed,
            target.short_width,
            target.int_width,
            target.long_width,
            target.long_long_width,
            target.pointer_width,
            target.wchar_signed,
            target.wchar_width,
            target.long_double
        )?;
        for definition in &self.module.types {
            write!(f, "    type @type{}", definition.value.id.0)?;
            if let Some(name) = &definition.name {
                write!(f, " {name}")?;
            }
            match &definition.kind {
                TypeDefinitionKind::Alias(ty) => write!(f, " = {ty}")?,
                TypeDefinitionKind::Pointer { pointee, is_const } => write!(
                    f,
                    " = ptr<{}{pointee}>",
                    if *is_const { "const " } else { "" }
                )?,
                TypeDefinitionKind::Array { element, length } => {
                    write!(f, " = array<{element}, ")?;
                    match length {
                        Some(length) => write!(f, "{length}")?,
                        None => f.write_str("incomplete")?,
                    }
                    f.write_str(">")?;
                }
                TypeDefinitionKind::Record {
                    kind,
                    fields,
                    layout,
                } => {
                    write!(
                        f,
                        " = {}",
                        match kind {
                            RecordKind::Struct => "struct",
                            RecordKind::Union => "union",
                        }
                    )?;
                    if let Some(fields) = fields {
                        writeln!(f, " {{")?;
                        for (index, field) in fields.iter().enumerate() {
                            write!(
                                f,
                                "        field{index} {}: {}",
                                field.name.as_deref().unwrap_or("<anonymous>"),
                                field.ty
                            )?;
                            metadata(f, self.table(), field.id)?;
                            writeln!(f, ";")?;
                        }
                        f.write_str("    }")?;
                    } else {
                        f.write_str(" incomplete")?;
                    }
                    if let Some(layout) = layout {
                        write!(
                            f,
                            " [size={}, align={}, offsets={:?}]",
                            layout.size, layout.align, layout.offsets
                        )?;
                    }
                }
                TypeDefinitionKind::Enum {
                    underlying,
                    enumerators,
                } => {
                    f.write_str(" = enum")?;
                    if let Some(underlying) = underlying {
                        write!(f, " : {underlying}")?;
                    }
                    if let Some(enumerators) = enumerators {
                        writeln!(f, " {{")?;
                        for enumerator in enumerators {
                            write!(
                                f,
                                "        %{} {} = {}",
                                enumerator.value.id.0,
                                enumerator.name,
                                enumerator.value.value.display_metadata(false, self.table())
                            )?;
                            metadata(f, self.table(), enumerator.id)?;
                            writeln!(f, ";")?;
                        }
                        f.write_str("    }")?;
                    } else {
                        f.write_str(" incomplete")?;
                    }
                }
            }
            metadata(f, self.table(), definition.id)?;
            writeln!(f, ";")?;
        }
        for global in &self.module.globals {
            write!(
                f,
                "    {} ",
                if global.definition {
                    "global"
                } else {
                    "extern"
                }
            )?;
            self.variable(f, &global.variable)?;
            write!(f, " [linkage={}]", global.linkage)?;
            metadata(f, self.table(), global.id)?;
            writeln!(f, ";")?;
        }
        for function in &self.module.functions {
            write!(f, "    fn %{} @{}(", function.value.id.0, function.name)?;
            match &function.parameters {
                Parameters::Unprototyped => f.write_str("unprototyped")?,
                Parameters::Prototype { fixed, variadic } => {
                    for (index, parameter) in fixed.iter().enumerate() {
                        if index != 0 {
                            f.write_str(", ")?;
                        }
                        write!(
                            f,
                            "%{} {}: {}",
                            parameter.value.id.0,
                            parameter.name.as_deref().unwrap_or("<unnamed>"),
                            parameter.ty
                        )?;
                        metadata(f, self.table(), parameter.id)?;
                    }
                    if *variadic {
                        write!(f, "{}...", if fixed.is_empty() { "" } else { ", " })?;
                    }
                }
            }
            f.write_str(") -> ")?;
            match function.return_type {
                Some(ty) => write!(f, "{ty}")?,
                None => f.write_str("void")?,
            }
            write!(f, " [linkage={}]", function.linkage)?;
            metadata(f, self.table(), function.id)?;
            if let Some(body) = &function.body {
                writeln!(f, " {{")?;
                self.statements(f, body, 8)?;
                writeln!(f, "    }}")?;
            } else {
                writeln!(f, ";")?;
            }
        }
        writeln!(f, "}}")
    }
}
