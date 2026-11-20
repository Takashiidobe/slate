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
    exported: Vec<bool>,
}

impl<'m> Program<'m> {
    pub fn new(
        units: impl IntoIterator<Item = (&'m ir::Module, &'m Files)>,
        exported: impl Fn(&ir::Module, &ir::Function) -> bool,
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
        let exported = functions
            .iter()
            .map(|entry: &FunctionEntry<'m>| {
                exported(units[entry.unit.0 as usize].module, &entry.function.value)
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

fn link_name(function: &ir::Function) -> &str {
    function
        .symbol
        .asm_name
        .as_deref()
        .unwrap_or(&function.name)
}
