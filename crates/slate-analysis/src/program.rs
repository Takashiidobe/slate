use foldhash::{HashMap, HashMapExt, HashSet, HashSetExt};
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
    exported: Vec<bool>,
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
            HashSet::new()
        };
        let exported = functions
            .iter()
            .map(|entry: &FunctionEntry<'m>| {
                let function = &entry.function.value;
                exported(units[entry.unit.0 as usize].module, function)
                    && (!closed || pinned.contains(link_name(function)) || fixed_abi(function))
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
        self.exported.get(id.0 as usize).copied().unwrap_or(true)
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

fn fixed_abi(function: &ir::Function) -> bool {
    let symbol = &function.symbol;
    let semantics = &function.semantics;
    symbol.section.is_some()
        || symbol.used
        || symbol.retain
        || symbol.dll_storage.is_some()
        || symbol.selectany
        || semantics.constructor.is_some()
        || semantics.destructor.is_some()
        || semantics.naked
        || semantics.interrupt
        || semantics.weak_odr
        || semantics.discardable
        || !semantics.target.is_empty()
        || !semantics.sme.is_empty()
        || function
            .abi
            .as_ref()
            .is_some_and(ir::AbiSignature::has_nontrivial_pass)
        || !matches!(
            function.parameters,
            ir::Parameters::Prototype {
                variadic: false,
                ..
            }
        )
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
) -> HashSet<&'m str> {
    let mut pinned = HashSet::new();
    let mut defined: HashMap<&'m str, u32> = HashMap::new();
    for ProgramUnit { module, .. } in units {
        for function in &module.functions {
            let symbol = &function.symbol;
            pinned.extend(
                [
                    &symbol.alias,
                    &symbol.weakref,
                    &symbol.ifunc,
                    &symbol.fallback,
                ]
                .into_iter()
                .flatten()
                .map(String::as_str),
            );
            if function.body.is_some() && matches!(function.linkage, Linkage::External) {
                *defined.entry(link_name(function)).or_default() += 1;
            }
        }
        for global in &module.globals {
            let symbol = &global.symbol;
            pinned.extend(
                [&symbol.alias, &symbol.weakref, &symbol.fallback]
                    .into_iter()
                    .flatten()
                    .map(String::as_str),
            );
        }
    }
    pinned.extend(
        defined
            .iter()
            .filter(|(_, count)| **count > 1)
            .map(|(name, _)| *name),
    );
    for (unit, ProgramUnit { module, .. }) in (0u32..).map(UnitId).zip(units) {
        for function in functions.iter().map(|entry| &entry.function.value) {
            let name = link_name(function);
            if module
                .asm
                .iter()
                .any(|asm| mentions(&asm.value.template, name))
            {
                pinned.insert(name);
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
                pinned.insert(link_name(definition));
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
