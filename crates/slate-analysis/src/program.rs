use foldhash::{HashMap, HashMapExt};
use slate_parser::ast::Span;
use slate_parser::files::Files;
use slate_parser::ir::{self, BindingId, Callee, Linkage};

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash, PartialOrd, Ord)]
pub struct UnitId(pub u32);

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash, PartialOrd, Ord)]
pub struct FunctionId(pub u32);

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub struct ProgramBinding {
    pub unit: UnitId,
    pub binding: BindingId,
}

#[derive(Debug, Clone, Copy)]
pub struct FunctionEntry<'m> {
    pub unit: UnitId,
    pub function: &'m Span<ir::Function>,
}

#[derive(Clone, Copy)]
pub struct ProgramUnit<'m> {
    pub module: &'m ir::Module,
    pub files: &'m Files,
}

pub struct Program<'m> {
    units: Vec<ProgramUnit<'m>>,
    functions: Vec<FunctionEntry<'m>>,
    resolved: HashMap<ProgramBinding, FunctionId>,
    exported: Vec<Option<ExportPin>>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash, PartialOrd, Ord)]
pub enum ExportPin {
    Exported,
    Alias,
    Weakref,
    Ifunc,
    Fallback,
    DefinedTwice,
    GlobalAsm,
    Prototype,
    Section,
    Used,
    Retain,
    DllStorage,
    Selectany,
    Constructor,
    Destructor,
    Naked,
    Interrupt,
    WeakOdr,
    Discardable,
    TargetFeature,
    Sme,
    AbiPass,
    Variadic,
}

impl ExportPin {
    pub fn name(self) -> &'static str {
        match self {
            ExportPin::Exported => "exported",
            ExportPin::Alias => "alias",
            ExportPin::Weakref => "weakref",
            ExportPin::Ifunc => "ifunc",
            ExportPin::Fallback => "fallback",
            ExportPin::DefinedTwice => "defined_twice",
            ExportPin::GlobalAsm => "global_asm",
            ExportPin::Prototype => "declared_other_prototype",
            ExportPin::Section => "section",
            ExportPin::Used => "used",
            ExportPin::Retain => "retain",
            ExportPin::DllStorage => "dll_storage",
            ExportPin::Selectany => "selectany",
            ExportPin::Constructor => "constructor",
            ExportPin::Destructor => "destructor",
            ExportPin::Naked => "naked",
            ExportPin::Interrupt => "interrupt",
            ExportPin::WeakOdr => "weak_odr",
            ExportPin::Discardable => "discardable",
            ExportPin::TargetFeature => "target_feature",
            ExportPin::Sme => "sme",
            ExportPin::AbiPass => "abi_pass",
            ExportPin::Variadic => "variadic",
        }
    }
}

impl<'m> Program<'m> {
    pub fn new(
        units: impl IntoIterator<Item = (&'m ir::Module, &'m Files)>,
        exported: impl Fn(&ir::Module, &ir::Function) -> bool,
        closed: bool,
    ) -> Self {
        let units: Vec<_> = units
            .into_iter()
            .map(|(module, files)| ProgramUnit { module, files })
            .collect();
        let mut functions = Vec::new();
        let mut resolved = HashMap::new();
        let mut external: HashMap<&'m str, FunctionId> = HashMap::new();
        for (unit, ProgramUnit { module, .. }) in (0u32..).map(UnitId).zip(&units) {
            for function in module.functions.iter().filter(|f| has_known_body(f)) {
                let id = FunctionId(functions.len() as u32);
                functions.push(FunctionEntry { unit, function });
                resolved.insert(
                    ProgramBinding {
                        unit,
                        binding: function.value.id,
                    },
                    id,
                );
                if matches!(function.linkage, Linkage::External) {
                    external.entry(link_name(function)).or_insert(id);
                }
            }
        }
        for (unit, ProgramUnit { module, .. }) in (0u32..).map(UnitId).zip(&units) {
            for function in &module.functions {
                let key = ProgramBinding {
                    unit,
                    binding: function.value.id,
                };
                if function.body.is_none()
                    && matches!(function.linkage, Linkage::External)
                    && !resolved.contains_key(&key)
                    && let Some(&id) = external.get(link_name(function))
                {
                    resolved.insert(key, id);
                }
            }
        }
        let pinned = if closed {
            pinned_symbols(&units, &resolved, &functions)
        } else {
            HashMap::new()
        };
        let exported = functions
            .iter()
            .map(|entry: &FunctionEntry<'m>| {
                let function = &entry.function.value;
                if !exported(units[entry.unit.0 as usize].module, function) {
                    return None;
                }
                if !closed {
                    return Some(ExportPin::Exported);
                }
                pinned
                    .get(link_name(function))
                    .copied()
                    .or_else(|| fixed_abi(function))
            })
            .collect();
        Self {
            units,
            functions,
            resolved,
            exported,
        }
    }

    pub fn exported(&self, id: FunctionId) -> bool {
        self.exported.get(id.0 as usize).is_none_or(Option::is_some)
    }

    pub fn export_pin(&self, id: FunctionId) -> Option<ExportPin> {
        self.exported
            .get(id.0 as usize)
            .map_or(Some(ExportPin::Exported), |pin| *pin)
    }

    pub fn units(&self) -> impl ExactSizeIterator<Item = (UnitId, ProgramUnit<'m>)> + '_ {
        self.units
            .iter()
            .enumerate()
            .map(|(index, unit)| (UnitId(index as u32), *unit))
    }

    pub fn functions(&self) -> impl ExactSizeIterator<Item = (FunctionId, FunctionEntry<'m>)> + '_ {
        self.functions
            .iter()
            .enumerate()
            .map(|(index, entry)| (FunctionId(index as u32), *entry))
    }

    pub fn unit(&self, id: UnitId) -> ProgramUnit<'m> {
        self.units[id.0 as usize]
    }

    pub fn function(&self, id: FunctionId) -> Option<FunctionEntry<'m>> {
        self.functions.get(id.0 as usize).copied()
    }

    pub fn same_type(&self, left: (UnitId, &ir::Type), right: (UnitId, &ir::Type)) -> bool {
        if left.0 == right.0 {
            return left.1 == right.1;
        }
        let (Some(left_definition), Some(right_definition)) = (
            self.definition(left.0, left.1),
            self.definition(right.0, right.1),
        ) else {
            return self.resolve(left.0, left.1) == self.resolve(right.0, right.1);
        };
        let layout = |definition: &ir::TypeDefinition| match &definition.kind {
            ir::TypeDefinitionKind::Record {
                kind,
                layout: Some(layout),
                ..
            } => Some((
                matches!(kind, ir::RecordKind::Union),
                layout.size,
                layout.align,
            )),
            ir::TypeDefinitionKind::Record { layout: None, .. }
            | ir::TypeDefinitionKind::Alias(_)
            | ir::TypeDefinitionKind::Enum { .. } => None,
        };
        left_definition.name.is_some()
            && left_definition.name == right_definition.name
            && layout(left_definition).is_some()
            && layout(left_definition) == layout(right_definition)
    }

    pub fn field_aligned(&self, unit: UnitId, record: &ir::Type, index: usize) -> bool {
        matches!(
            self.definition(unit, record).map(|definition| &definition.kind),
            Some(ir::TypeDefinitionKind::Record {
                fields: Some(fields),
                layout: Some(layout),
                ..
            }) if fields.get(index).is_some_and(|field| field.bit_width.is_none())
                && layout.underaligned.get(index) == Some(&false)
        )
    }

    fn resolve<'t>(&'t self, unit: UnitId, mut ty: &'t ir::Type) -> &'t ir::Type {
        while let ir::Type::Defined(id) = ty
            && let Some(definition) = self.type_definition(unit, *id)
            && let ir::TypeDefinitionKind::Alias(inner)
            | ir::TypeDefinitionKind::Enum {
                underlying: Some(inner),
                ..
            } = &definition.kind
        {
            ty = inner;
        }
        ty
    }

    fn definition<'t>(&'t self, unit: UnitId, ty: &'t ir::Type) -> Option<&'t ir::TypeDefinition> {
        match self.resolve(unit, ty) {
            ir::Type::Defined(id) => self.type_definition(unit, *id),
            #[expect(
                slate::sensitive_wildcard,
                reason = "only record definitions need a cross-unit identity"
            )]
            _ => None,
        }
    }

    fn type_definition(&self, unit: UnitId, id: ir::TypeId) -> Option<&'m ir::TypeDefinition> {
        self.units[unit.0 as usize]
            .module
            .types
            .iter()
            .map(|definition| &definition.value)
            .find(|definition| definition.id == id)
    }

    pub fn callee(&self, unit: UnitId, callee: &Callee) -> Option<FunctionId> {
        match callee {
            Callee::Direct(binding) => self
                .resolved
                .get(&ProgramBinding {
                    unit,
                    binding: *binding,
                })
                .copied(),
            Callee::Indirect(_) => None,
        }
    }
}

fn has_known_body(function: &ir::Function) -> bool {
    let symbol = &function.symbol;
    function.body.is_some()
        && !function.semantics.inline_only
        && !symbol.weak
        && symbol.alias.is_none()
        && symbol.weakref.is_none()
        && symbol.ifunc.is_none()
}

fn fixed_abi(function: &ir::Function) -> Option<ExportPin> {
    let symbol = &function.symbol;
    let semantics = &function.semantics;
    [
        (symbol.section.is_some(), ExportPin::Section),
        (symbol.used, ExportPin::Used),
        (symbol.retain, ExportPin::Retain),
        (symbol.dll_storage.is_some(), ExportPin::DllStorage),
        (symbol.selectany, ExportPin::Selectany),
        (semantics.constructor.is_some(), ExportPin::Constructor),
        (semantics.destructor.is_some(), ExportPin::Destructor),
        (semantics.naked, ExportPin::Naked),
        (semantics.interrupt, ExportPin::Interrupt),
        (semantics.weak_odr, ExportPin::WeakOdr),
        (semantics.discardable, ExportPin::Discardable),
        (!semantics.target.is_empty(), ExportPin::TargetFeature),
        (!semantics.sme.is_empty(), ExportPin::Sme),
        (
            function
                .abi
                .as_ref()
                .is_some_and(ir::AbiSignature::has_nontrivial_pass),
            ExportPin::AbiPass,
        ),
        (
            !matches!(
                function.parameters,
                ir::Parameters::Prototype {
                    variadic: false,
                    ..
                }
            ),
            ExportPin::Variadic,
        ),
    ]
    .into_iter()
    .find_map(|(fixed, pin)| fixed.then_some(pin))
}

fn mentions(template: &str, symbol: &str) -> bool {
    template
        .split(|c: char| !(c.is_ascii_alphanumeric() || c == '_' || c == '$' || c == '.'))
        .any(|word| word == symbol)
}

fn pinned_symbols<'m>(
    units: &[ProgramUnit<'m>],
    resolved: &HashMap<ProgramBinding, FunctionId>,
    functions: &[FunctionEntry<'m>],
) -> HashMap<&'m str, ExportPin> {
    let mut pinned = HashMap::new();
    let mut pin = |name: &'m str, reason: ExportPin| {
        pinned.entry(name).or_insert(reason);
    };
    let mut defined: HashMap<&'m str, u32> = HashMap::new();
    for ProgramUnit { module, .. } in units {
        for function in &module.functions {
            let symbol = &function.symbol;
            for (target, reason) in [
                (&symbol.alias, ExportPin::Alias),
                (&symbol.weakref, ExportPin::Weakref),
                (&symbol.ifunc, ExportPin::Ifunc),
                (&symbol.fallback, ExportPin::Fallback),
            ] {
                if let Some(target) = target {
                    pin(target, reason);
                }
            }
            if function.body.is_some() && matches!(function.linkage, Linkage::External) {
                *defined.entry(link_name(function)).or_default() += 1;
            }
        }
        for global in &module.globals {
            let symbol = &global.symbol;
            for (target, reason) in [
                (&symbol.alias, ExportPin::Alias),
                (&symbol.weakref, ExportPin::Weakref),
                (&symbol.fallback, ExportPin::Fallback),
            ] {
                if let Some(target) = target {
                    pin(target, reason);
                }
            }
        }
    }
    for (name, count) in &defined {
        if *count > 1 {
            pin(name, ExportPin::DefinedTwice);
        }
    }
    for (unit, ProgramUnit { module, .. }) in (0u32..).map(UnitId).zip(units) {
        for function in functions.iter().map(|entry| &entry.function.value) {
            let name = link_name(function);
            if module
                .asm
                .iter()
                .any(|asm| mentions(&asm.value.template, name))
            {
                pin(name, ExportPin::GlobalAsm);
            }
        }
        for declaration in module.functions.iter().filter(|f| f.body.is_none()) {
            let Some(id) = resolved.get(&ProgramBinding {
                unit,
                binding: declaration.value.id,
            }) else {
                continue;
            };
            let definition = &functions[id.0 as usize].function.value;
            if !same_prototype(&declaration.value, definition) {
                pin(link_name(definition), ExportPin::Prototype);
            }
        }
    }
    pinned
}

fn same_prototype(declaration: &ir::Function, definition: &ir::Function) -> bool {
    let (
        ir::Parameters::Prototype {
            fixed: left,
            variadic: false,
        },
        ir::Parameters::Prototype {
            fixed: right,
            variadic: false,
        },
    ) = (&declaration.parameters, &definition.parameters)
    else {
        return false;
    };
    declaration.name == definition.name
        && left.len() == right.len()
        && left.iter().zip(right).all(|(left, right)| {
            matches!(left.value.ty, ir::Type::Pointer { .. })
                == matches!(right.value.ty, ir::Type::Pointer { .. })
        })
}

fn link_name(function: &ir::Function) -> &str {
    function
        .symbol
        .asm_name
        .as_deref()
        .unwrap_or(&function.name)
}
