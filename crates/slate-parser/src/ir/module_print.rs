use super::{
    FloatType, Linkage, Metadata, Module, NumericType, Parameters, RecordKind, Statement,
    StorageDuration, Type, TypeDefinitionKind, Variable,
};
use crate::{
    ast::{NodeId, Span},
    ir::Fallthrough,
};
use std::fmt;

pub struct DisplayModule<'a> {
    module: &'a Module,
    show_metadata: bool,
    compact: bool,
}

impl Module {
    pub fn display(&self, show_metadata: bool) -> DisplayModule<'_> {
        DisplayModule {
            module: self,
            show_metadata,
            compact: false,
        }
    }
}

impl DisplayModule<'_> {
    pub fn compact(mut self) -> Self {
        self.compact = true;
        self
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
            write!(
                f,
                " = {}",
                value
                    .display_metadata(false, self.table())
                    .with_compact(self.compact)
            )?;
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
                    "write<{}>({}, {})",
                    place.ty,
                    place.display_mode(self.compact),
                    value
                        .display_metadata(false, self.table())
                        .with_compact(self.compact)
                )?,
                Statement::Expression(value) => write!(
                    f,
                    "{}",
                    value
                        .display_metadata(false, self.table())
                        .with_compact(self.compact)
                )?,
                Statement::Return(Some(value)) => write!(
                    f,
                    "return {}",
                    value
                        .display_metadata(false, self.table())
                        .with_compact(self.compact)
                )?,
                Statement::Return(None) => f.write_str("return")?,
                Statement::If {
                    condition,
                    then_body,
                    else_body,
                } => {
                    write!(
                        f,
                        "if {}",
                        condition
                            .display_metadata(false, self.table())
                            .with_compact(self.compact)
                    )?;
                    metadata(f, self.table(), statement.id)?;
                    writeln!(f)?;
                    self.statements(f, then_body, indent + 4)?;
                    if let Some(body) = else_body {
                        writeln!(f, "{:indent$}else", "")?;
                        self.statements(f, body, indent + 4)?;
                    }
                    continue;
                }
                Statement::While {
                    id,
                    condition,
                    body,
                } => {
                    write!(
                        f,
                        "while %{} {}",
                        id.0,
                        condition
                            .display_metadata(false, self.table())
                            .with_compact(self.compact)
                    )?;
                    metadata(f, self.table(), statement.id)?;
                    writeln!(f)?;
                    self.statements(f, body, indent + 4)?;
                    continue;
                }
                Statement::DoWhile {
                    id,
                    body,
                    condition,
                } => {
                    write!(f, "do %{}", id.0)?;
                    metadata(f, self.table(), statement.id)?;
                    writeln!(f)?;
                    self.statements(f, body, indent + 4)?;
                    writeln!(
                        f,
                        "{:indent$}while {};",
                        "",
                        condition
                            .display_metadata(false, self.table())
                            .with_compact(self.compact)
                    )?;
                    continue;
                }
                Statement::For {
                    id,
                    init,
                    condition,
                    increment,
                    body,
                } => {
                    write!(f, "for %{}", id.0)?;
                    metadata(f, self.table(), statement.id)?;
                    writeln!(f)?;
                    writeln!(f, "{:indent$}    init:", "")?;
                    self.statements(f, init, indent + 8)?;
                    write!(f, "{:indent$}    condition: ", "")?;
                    if let Some(value) = condition {
                        write!(
                            f,
                            "{}",
                            value
                                .display_metadata(false, self.table())
                                .with_compact(self.compact)
                        )?;
                    } else {
                        f.write_str("omitted")?;
                    }
                    writeln!(f)?;
                    write!(f, "{:indent$}    increment: ", "")?;
                    if let Some(value) = increment {
                        write!(
                            f,
                            "{}",
                            value
                                .display_metadata(false, self.table())
                                .with_compact(self.compact)
                        )?;
                    } else {
                        f.write_str("omitted")?;
                    }
                    writeln!(f)?;
                    writeln!(f, "{:indent$}    body:", "")?;
                    self.statements(f, body, indent + 8)?;
                    continue;
                }
                Statement::Break(id) => write!(f, "break %{}", id.0)?,
                Statement::Continue(id) => write!(f, "continue %{}", id.0)?,
                Statement::Switch {
                    id,
                    discriminant,
                    body,
                } => {
                    write!(
                        f,
                        "switch %{} {}",
                        id.0,
                        discriminant
                            .display_metadata(false, self.table())
                            .with_compact(self.compact)
                    )?;
                    metadata(f, self.table(), statement.id)?;
                    writeln!(f)?;
                    self.statements(f, body, indent + 4)?;
                    continue;
                }
                Statement::Case {
                    switch,
                    start,
                    end,
                    body,
                } => {
                    write!(
                        f,
                        "case %{} {}",
                        switch.0,
                        start
                            .display_metadata(false, self.table())
                            .with_compact(self.compact)
                    )?;
                    if let Some(end) = end {
                        write!(
                            f,
                            " ... {}",
                            end.display_metadata(false, self.table())
                                .with_compact(self.compact)
                        )?;
                    }
                    f.write_str(":")?;
                    metadata(f, self.table(), statement.id)?;
                    writeln!(f)?;
                    self.statements(f, body, indent + 4)?;
                    continue;
                }
                Statement::Default { switch, body } => {
                    write!(f, "default %{}:", switch.0)?;
                    metadata(f, self.table(), statement.id)?;
                    writeln!(f)?;
                    self.statements(f, body, indent + 4)?;
                    continue;
                }
                Statement::Null => {}
                Statement::Goto(id) => write!(f, "goto %{}", id.0)?,
                Statement::ComputedGoto(value) => write!(
                    f,
                    "goto *{}",
                    value
                        .display_metadata(false, self.table())
                        .with_compact(self.compact)
                )?,
                Statement::Label { id, name, body } => {
                    write!(f, "label %{} {name}:", id.0)?;
                    metadata(f, self.table(), statement.id)?;
                    writeln!(f)?;
                    self.statements(f, body, indent + 4)?;
                    continue;
                }
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
        let target = &self.module.target;
        writeln!(f, "    target \"{}\" {{", target.triple)?;
        writeln!(f, "        endian = {};", target.endian.as_str())?;
        writeln!(
            f,
            "        pointer [size={}, align={}];",
            target.pointer.size_bytes, target.pointer.alignment_bytes
        )?;
        writeln!(
            f,
            "        stack_alignment = {};",
            target.abi.preferred_stack_alignment
        )?;
        writeln!(
            f,
            "        long_double = {};",
            target.long_double.float_type()
        )?;
        for (names, ty) in [
            ("bool", Type::Bool),
            (
                "i8, u8",
                Type::Numeric(NumericType::Integer {
                    width: 8,
                    signed: true,
                }),
            ),
            (
                "i16, u16",
                Type::Numeric(NumericType::Integer {
                    width: 16,
                    signed: true,
                }),
            ),
            (
                "i32, u32",
                Type::Numeric(NumericType::Integer {
                    width: 32,
                    signed: true,
                }),
            ),
            (
                "i64, u64",
                Type::Numeric(NumericType::Integer {
                    width: 64,
                    signed: true,
                }),
            ),
            (
                "i128, u128",
                Type::Numeric(NumericType::Integer {
                    width: 128,
                    signed: true,
                }),
            ),
            ("f16", Type::Numeric(NumericType::Float(FloatType::F16))),
            ("f32", Type::Numeric(NumericType::Float(FloatType::F32))),
            ("f64", Type::Numeric(NumericType::Float(FloatType::F64))),
            ("f80", Type::Numeric(NumericType::Float(FloatType::F80))),
            ("f128", Type::Numeric(NumericType::Float(FloatType::F128))),
        ] {
            if let Ok(layout) = target.storage_of(ty) {
                writeln!(
                    f,
                    "        storage {names} [size={}, align={}];",
                    layout.size_bytes, layout.alignment_bytes
                )?;
            }
        }
        writeln!(f, "    }}")?;
        for definition in &self.module.types {
            write!(f, "    type @type{}", definition.value.id.0)?;
            if let Some(name) = &definition.name {
                write!(f, " {name}")?;
            }
            match &definition.kind {
                TypeDefinitionKind::Alias(ty) => write!(f, " = {ty}")?,
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
                            if let Some(width) = field.bit_width {
                                write!(f, " : {width}")?;
                            }
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
                            " [size={}, align={}, offsets={:?}",
                            layout.size, layout.align, layout.offsets
                        )?;
                        if layout.bit_offsets.iter().any(Option::is_some) {
                            write!(f, ", bit_offsets={:?}", layout.bit_offsets)?;
                        }
                        if !layout.bit_units.is_empty() {
                            let units: Vec<_> = layout
                                .bit_units
                                .iter()
                                .map(|unit| (unit.offset, unit.size))
                                .collect();
                            write!(f, ", bit_units={units:?}")?;
                        }
                        if layout.field_units.iter().any(Option::is_some) {
                            write!(f, ", field_units={:?}", layout.field_units)?;
                        }
                        f.write_str("]")?;
                    }
                }
                TypeDefinitionKind::Enum {
                    underlying,
                    enumerators,
                    layout,
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
                                enumerator
                                    .value
                                    .value
                                    .display_metadata(false, self.table())
                                    .with_compact(self.compact)
                            )?;
                            metadata(f, self.table(), enumerator.id)?;
                            writeln!(f, ";")?;
                        }
                        f.write_str("    }")?;
                    } else {
                        f.write_str(" incomplete")?;
                    }
                    if let Some(layout) = layout {
                        write!(
                            f,
                            " [size={}, align={}]",
                            layout.size_bytes, layout.alignment_bytes
                        )?;
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
            match &function.return_type {
                Some(ty) => write!(f, "{ty}")?,
                None => f.write_str("void")?,
            }
            write!(f, " [linkage={}]", function.linkage)?;
            if let Some(fallthrough) = function.fallthrough {
                write!(
                    f,
                    " [fallthrough={}]",
                    match fallthrough {
                        Fallthrough::ReturnZero => "ret_zero",
                        Fallthrough::ReturnVoid => "ret_void",
                        Fallthrough::UndefinedIfUsed => "ub_if_used",
                    }
                )?;
            }
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
