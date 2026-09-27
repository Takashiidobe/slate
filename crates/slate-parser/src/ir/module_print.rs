use super::{
    ArrayExtent, AsmOperandKind, DllStorage, Evaluation, FloatType, InlineAsm, Inlining, Linkage,
    MemoryEffects, Metadata, Module, NumericType, Parameters, RecordKind, Statement,
    StorageDuration, SymbolAttributes, TlsModel, Type, TypeDefinitionKind, Variable, Visibility,
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

impl fmt::Display for Visibility {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.write_str(match self {
            Self::Default => "default",
            Self::Hidden => "hidden",
            Self::Protected => "protected",
            Self::Internal => "internal",
        })
    }
}

impl fmt::Display for TlsModel {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.write_str(match self {
            Self::GlobalDynamic => "global-dynamic",
            Self::LocalDynamic => "local-dynamic",
            Self::InitialExec => "initial-exec",
            Self::LocalExec => "local-exec",
        })
    }
}

impl fmt::Display for SymbolAttributes {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        if let Some(name) = &self.asm_name {
            write!(f, " [asm_name={name:?}]")?;
        }
        if let Some(visibility) = self.visibility {
            write!(f, " [visibility={visibility}]")?;
        }
        if self.weak {
            f.write_str(" [weak]")?;
        }
        if let Some(target) = &self.alias {
            write!(f, " [alias={target:?}]")?;
        }
        if let Some(target) = &self.weakref {
            write!(f, " [weakref={target:?}]")?;
        }
        if let Some(section) = &self.section {
            write!(f, " [section={section:?}]")?;
        }
        if self.used {
            f.write_str(" [used]")?;
        }
        if self.retain {
            f.write_str(" [retain]")?;
        }
        if let Some(model) = self.tls_model {
            write!(f, " [tls_model={model}]")?;
        }
        match self.dll_storage {
            Some(DllStorage::Import) => f.write_str(" [dllimport]")?,
            Some(DllStorage::Export) => f.write_str(" [dllexport]")?,
            None => {}
        }
        if self.selectany {
            f.write_str(" [selectany]")?;
        }
        Ok(())
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

fn asm_dialect(f: &mut fmt::Formatter<'_>, asm: &InlineAsm) -> fmt::Result {
    match asm.dialect {
        Some(dialect) => write!(f, " [dialect={}]", dialect.as_str()),
        None => Ok(()),
    }
}

impl DisplayModule<'_> {
    fn table(&self) -> Option<&Metadata> {
        self.show_metadata.then_some(&self.module.metadata)
    }

    fn variable(&self, f: &mut fmt::Formatter<'_>, variable: &Variable) -> fmt::Result {
        write!(
            f,
            "%{} {}: {}{} [storage={}]",
            variable.id.0,
            variable.name,
            variable.access.prefix(),
            variable.ty,
            match variable.storage {
                StorageDuration::Automatic => "automatic",
                StorageDuration::Static => "static",
                StorageDuration::Thread => "thread",
            }
        )?;
        if variable.restrict {
            f.write_str(" [restrict]")?;
        }
        if variable.is_const {
            f.write_str(" [const]")?;
        }
        if variable.constexpr {
            f.write_str(" [constexpr]")?;
        }
        if let Some(alignment) = variable.alignment {
            write!(f, " [align={alignment}]")?;
        }
        if let Some(function) = &variable.cleanup {
            write!(f, " [cleanup={function}]")?;
        }
        if let Some(register) = &variable.register {
            write!(f, " [register={:?}]", register.spelling)?;
        }
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

    fn evaluation(
        &self,
        f: &mut fmt::Formatter<'_>,
        evaluation: &Evaluation,
        indent: usize,
    ) -> fmt::Result {
        if evaluation.statements.is_empty() {
            return write!(
                f,
                "{}",
                evaluation
                    .value
                    .display_metadata(false, self.table())
                    .with_compact(self.compact)
            );
        }
        writeln!(f, "{{")?;
        self.statements(f, &evaluation.statements, indent + 4)?;
        writeln!(
            f,
            "{:width$}yield {};",
            "",
            evaluation
                .value
                .display_metadata(false, self.table())
                .with_compact(self.compact),
            width = indent + 4
        )?;
        write!(f, "{:indent$}}}", "")
    }

    fn asm_sections(
        &self,
        f: &mut fmt::Formatter<'_>,
        asm: &InlineAsm,
        indent: usize,
    ) -> fmt::Result {
        if !asm.pieces.is_empty() {
            write!(f, "{:indent$}template:", "")?;
            for piece in &asm.pieces {
                write!(f, " {piece}")?;
            }
            writeln!(f, ";")?;
        }
        for (index, operand) in asm.operands.iter().enumerate() {
            write!(f, "{:indent$}{} {index}", "", operand.direction().as_str())?;
            if let Some(name) = &operand.name {
                write!(f, " [{name}]")?;
            }
            write!(f, " {}", operand.constraint)?;
            let (place, input) = match &operand.kind {
                AsmOperandKind::In(value) => (None, Some(value)),
                AsmOperandKind::Out { place, .. } => (Some(place), None),
                AsmOperandKind::InOut { place, input, .. } => (Some(place), input.as_ref()),
            };
            if let Some(place) = place {
                write!(
                    f,
                    " place<{}{}>({})",
                    place.ty,
                    place.access,
                    place.display_mode(self.compact)
                )?;
            }
            if let Some(value) = input {
                write!(
                    f,
                    "{} {}",
                    if place.is_some() { " from" } else { "" },
                    value
                        .display_metadata(false, self.table())
                        .with_compact(self.compact)
                )?;
            }
            writeln!(f, ";")?;
        }
        if !asm.clobbers.is_empty() {
            write!(f, "{:indent$}clobbers:", "")?;
            for (index, clobber) in asm.clobbers.iter().enumerate() {
                write!(f, "{} {clobber}", if index > 0 { "," } else { "" })?;
            }
            writeln!(f, ";")?;
        }
        if !asm.labels.is_empty() {
            write!(f, "{:indent$}labels:", "")?;
            for (index, label) in asm.labels.iter().enumerate() {
                write!(f, "{} %{}", if index > 0 { "," } else { "" }, label.0)?;
            }
            writeln!(f, ";")?;
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
                Statement::Temporary {
                    id,
                    ty,
                    initializer,
                    unsequenced,
                } => {
                    let order = if *unsequenced { ", unsequenced" } else { "" };
                    write!(f, "let %{}: {} [synthetic{order}]", id.0, ty)?;
                    if let Some(value) = initializer {
                        write!(
                            f,
                            " = {}",
                            value
                                .display_metadata(false, self.table())
                                .with_compact(self.compact)
                        )?;
                    }
                }
                Statement::Let(variable) => {
                    f.write_str("let ")?;
                    self.variable(f, variable)?;
                }
                Statement::Write {
                    place,
                    value,
                    ordering,
                    unsequenced,
                } => {
                    write!(f, "write<{}{}", place.ty, place.access)?;
                    if *unsequenced {
                        f.write_str(", unsequenced")?;
                    }
                    super::atomic::format_ordering(f, ordering.as_ref(), self.compact)?;
                    write!(
                        f,
                        ">({}, {})",
                        place.display_mode(self.compact),
                        value
                            .display_metadata(false, self.table())
                            .with_compact(self.compact)
                    )?
                }
                Statement::Fence { ordering, scope } => write!(
                    f,
                    "fence<scope={scope}, order={}{}>",
                    ordering.order.display_mode(self.compact),
                    super::atomic::SyncScopeAttribute(&ordering.scope, self.compact)
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
                    write!(f, "while %{} ", id.0)?;
                    self.evaluation(f, condition, indent)?;
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
                    write!(f, "{:indent$}while ", "")?;
                    self.evaluation(f, condition, indent)?;
                    writeln!(f, ";")?;
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
                        self.evaluation(f, value, indent + 4)?;
                    } else {
                        f.write_str("omitted")?;
                    }
                    writeln!(f)?;
                    write!(f, "{:indent$}    increment: ", "")?;
                    if let Some(value) = increment {
                        self.evaluation(f, value, indent + 4)?;
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
                Statement::Asm(asm) => {
                    f.write_str("asm")?;
                    if asm.volatile {
                        f.write_str(" volatile")?;
                    }
                    if asm.inline {
                        f.write_str(" inline")?;
                    }
                    if asm.goto {
                        f.write_str(" goto")?;
                    }
                    write!(f, " {:?}", asm.template)?;
                    asm_dialect(f, asm)?;
                    if !asm.has_sections() {
                        metadata(f, self.table(), statement.id)?;
                        writeln!(f, ";")?;
                        continue;
                    }
                    f.write_str(" {")?;
                    metadata(f, self.table(), statement.id)?;
                    writeln!(f)?;
                    self.asm_sections(f, asm, indent + 4)?;
                    writeln!(f, "{:indent$}}}", "")?;
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
            ("i8, u8", Type::integer(8, true)),
            ("i16, u16", Type::integer(16, true)),
            ("i32, u32", Type::integer(32, true)),
            ("i64, u64", Type::integer(64, true)),
            ("i128, u128", Type::integer(128, true)),
            ("bf16", Type::Numeric(NumericType::Float(FloatType::BF16))),
            ("f16", Type::Numeric(NumericType::Float(FloatType::F16))),
            ("f32", Type::Numeric(NumericType::Float(FloatType::F32))),
            ("f64", Type::Numeric(NumericType::Float(FloatType::F64))),
            ("f80", Type::Numeric(NumericType::Float(FloatType::F80))),
            ("f128", Type::Numeric(NumericType::Float(FloatType::F128))),
            ("d32", Type::Numeric(NumericType::Float(FloatType::D32))),
            ("d64", Type::Numeric(NumericType::Float(FloatType::D64))),
            ("d128", Type::Numeric(NumericType::Float(FloatType::D128))),
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
        for asm in &self.module.asm {
            write!(f, "    asm {:?}", asm.template)?;
            asm_dialect(f, asm)?;
            if asm.has_sections() {
                f.write_str(" {")?;
                metadata(f, self.table(), asm.id)?;
                writeln!(f)?;
                self.asm_sections(f, asm, 8)?;
                writeln!(f, "    }}")?;
            } else {
                metadata(f, self.table(), asm.id)?;
                writeln!(f, ";")?;
            }
        }
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
                                "        field{index} {}: {}{}{}",
                                field.name.as_deref().unwrap_or("<anonymous>"),
                                if field.is_const { "const " } else { "" },
                                field.access.prefix(),
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
            write!(f, " [linkage={}]{}", global.linkage, global.symbol)?;
            if global.common {
                f.write_str(" [common]")?;
            }
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
                            "%{} {}: {}{}",
                            parameter.value.id.0,
                            parameter.name.as_deref().unwrap_or("<unnamed>"),
                            parameter.access.prefix(),
                            parameter.ty
                        )?;
                        if parameter.restrict {
                            f.write_str(" [restrict]")?;
                        }
                        if parameter.value.is_const {
                            f.write_str(" [const]")?;
                        }
                        if let Some(array) = parameter.value.array
                            && (array.guaranteed || array.extent != ArrayExtent::Unspecified)
                        {
                            write!(f, " {array}")?;
                        }
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
            write!(f, " [linkage={}]{}", function.linkage, function.symbol)?;
            if let Some(inlining) = function.semantics.inlining {
                write!(
                    f,
                    " [inline={}]",
                    match inlining {
                        Inlining::Hint => "hint",
                        Inlining::Always => "always",
                        Inlining::Never => "never",
                    }
                )?;
            }
            if function.body.is_some() && function.semantics.inlining.is_some() {
                write!(
                    f,
                    " [definition={}]",
                    if function.semantics.inline_only {
                        "inline_only"
                    } else {
                        "emitted"
                    }
                )?;
            }
            if function.semantics.noreturn {
                f.write_str(" [noreturn]")?;
            }
            match function.semantics.memory {
                Some(MemoryEffects::None) => f.write_str(" [memory=none]")?,
                Some(MemoryEffects::Read) => f.write_str(" [memory=read]")?,
                None => {}
            }
            if function.abi.has_nontrivial_pass() {
                write!(f, " [abi={}]", function.abi)?;
            }
            if let Some(fallthrough) = function.fallthrough {
                write!(
                    f,
                    " [fallthrough={}]",
                    match fallthrough {
                        Fallthrough::Undefined => "ub",
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
