use slate_parser::target_info::TargetOs;

use super::*;
use crate::backend::engine::shadow::visit_names;

const SUPPORT_CRATE: &str = "slate_support";
const FILE_RECORDS: &[&str] = &["_IO_FILE", "__sFILE", "_iobuf"];
const STREAMS: [&str; 3] = ["stdin", "stdout", "stderr"];

#[derive(Clone, Copy, PartialEq)]
enum Arg {
    File,
    Text,
    Int,
}

struct Binding {
    name: &'static str,
    params: &'static [Arg],
    variadic: bool,
    ret: Arg,
}

const fn binding(name: &'static str, params: &'static [Arg], variadic: bool, ret: Arg) -> Binding {
    Binding {
        name,
        params,
        variadic,
        ret,
    }
}

const BINDINGS: &[Binding] = &[
    binding("fprintf", &[Arg::File, Arg::Text], true, Arg::Int),
    binding("printf", &[Arg::Text], true, Arg::Int),
    binding("fputs", &[Arg::Text, Arg::File], false, Arg::Int),
    binding("puts", &[Arg::Text], false, Arg::Int),
    binding("fputc", &[Arg::Int, Arg::File], false, Arg::Int),
    binding("putc", &[Arg::Int, Arg::File], false, Arg::Int),
    binding("__sputc", &[Arg::Int, Arg::File], false, Arg::Int),
    binding("putchar", &[Arg::Int], false, Arg::Int),
    binding("fgetc", &[Arg::File], false, Arg::Int),
    binding("getc", &[Arg::File], false, Arg::Int),
    binding("ungetc", &[Arg::Int, Arg::File], false, Arg::Int),
    binding("feof", &[Arg::File], false, Arg::Int),
    binding("ferror", &[Arg::File], false, Arg::Int),
    binding("fclose", &[Arg::File], false, Arg::Int),
    binding("fopen", &[Arg::Text, Arg::Text], false, Arg::File),
];

pub(super) fn file_type() -> rust::Type {
    rust::Type::Custom(format!("{SUPPORT_CRATE}::FILE"))
}

pub(super) fn support_path(name: &str) -> Expr {
    Expr::Path(rust::Path::new(
        [SUPPORT_CRATE, name].into_iter().map(rust::Ident::from),
    ))
}

#[derive(Default)]
pub(super) struct StdioBindings {
    pub(super) file_records: HashSet<TypeId>,
    pub(super) functions: HashSet<BindingId>,
    pub(super) stream_globals: HashMap<BindingId, &'static str>,
    pub(super) android_streams: Option<BindingId>,
    pub(super) unreachable: HashSet<BindingId>,
}

fn referenced_names(item: &Item) -> BTreeSet<String> {
    let mut names = BTreeSet::new();
    let mut collect = |stmt: &Stmt| {
        visit_names(&mut stmt.clone(), |name, binds| {
            if !binds {
                names.insert(name.to_string());
            }
        });
    };
    match item {
        Item::Fn(definition) => definition.body.iter().for_each(&mut collect),
        Item::Static { init, .. } | Item::Const { init, .. } => collect(&Stmt::Expr(init.clone())),
        _ => {}
    }
    names
}

impl ModuleLowerer<'_> {
    fn unreachable_candidates(&self) -> HashMap<String, BindingId> {
        let tables = &self.tables;
        let functions = self.module.functions.iter().filter(|function| {
            function.provenance.system_header.is_some()
                && !function.symbol.used
                && !tables.stdio.functions.contains(&function.value.id)
                && (function.body.is_none()
                    || function.semantics.inlining.is_some()
                    || !matches!(function.linkage, ir::Linkage::External))
                && function.semantics.constructor.is_none()
                && function.semantics.destructor.is_none()
        });
        functions
            .filter_map(|function| {
                let name = tables.names.get(&function.value.id)?;
                Some((name.rust.clone(), function.value.id))
            })
            .chain(
                self.module
                    .globals
                    .iter()
                    .filter(|global| {
                        global.provenance.system_header.is_some()
                            && global.definition
                            && !global.symbol.used
                            && !matches!(global.linkage, ir::Linkage::External)
                    })
                    .map(|global| global.variable.id)
                    .chain(tables.stdio.stream_globals.keys().copied())
                    .map(|id| (binding_name(id, &tables.bindings), id)),
            )
            .collect()
    }

    pub(super) fn unreachable_stdio_helpers(&self) -> HashSet<BindingId> {
        if self.tables.stdio.file_records.is_empty() || !self.module.asm.is_empty() {
            return HashSet::new();
        }
        let candidates = self.unreachable_candidates();
        let mut pending: Vec<String> = Vec::new();
        let mut candidate_references: HashMap<&str, BTreeSet<String>> = HashMap::new();
        for item in self
            .items
            .iter()
            .chain(&self.dependencies.compound_literals)
        {
            let references = referenced_names(item);
            let name = match item {
                Item::Fn(definition) => Some(definition.name.as_str()),
                Item::Static { name, .. } => Some(name.as_str()),
                _ => None,
            };
            match name.filter(|name| candidates.contains_key(*name)) {
                Some(name) => {
                    candidate_references.insert(name, references);
                }
                None => pending.extend(references),
            }
        }
        let mut reached = HashSet::new();
        while let Some(name) = pending.pop() {
            if !reached.insert(name.clone()) {
                continue;
            }
            if let Some(references) = candidate_references.get(name.as_str()) {
                pending.extend(references.iter().cloned());
            }
        }
        candidates
            .into_iter()
            .filter(|(name, _)| !reached.contains(name))
            .map(|(_, id)| id)
            .collect()
    }
}

impl Tables<'_> {
    pub(super) fn is_file(&self, ty: &ir::Type) -> bool {
        matches!(self.resolve_type(ty), ir::Type::Defined(id) if self.stdio.file_records.contains(id))
    }

    fn is_file_pointer(&self, ty: &ir::Type) -> bool {
        matches!(self.resolve_type(ty), ir::Type::Pointer { pointee, .. } if self.is_file(pointee))
    }

    fn matches_arg(&self, ty: &ir::Type, arg: Arg) -> bool {
        match arg {
            Arg::File => self.is_file_pointer(ty),
            Arg::Text => matches!(
                self.resolve_type(ty),
                ir::Type::Pointer { pointee, is_const: true, .. }
                    if matches!(
                        self.resolve_type(pointee),
                        ir::Type::Numeric(ir::NumericType::Integer { width: 8, .. })
                    )
            ),
            Arg::Int => matches!(
                self.resolve_type(ty),
                ir::Type::Numeric(ir::NumericType::Integer {
                    width: 32,
                    signed: true,
                    ..
                })
            ),
        }
    }

    fn matches_binding(&self, function: &slate_parser::ast::Span<ir::Function>) -> bool {
        let Some(binding) = BINDINGS.iter().find(|known| known.name == function.name) else {
            return false;
        };
        let ir::Parameters::Prototype { fixed, variadic } = &function.parameters else {
            return false;
        };
        let symbol = function.symbol.asm_name.as_deref();
        let plain_symbol = symbol.is_none_or(|symbol| {
            symbol == function.name || symbol.strip_prefix('_') == Some(function.name.as_str())
        });
        let may_replace_body = function.body.is_none()
            || function.name == "__sputc"
            || matches!(self.target.os, TargetOs::Windows);
        function.provenance.system_header.is_some()
            && plain_symbol
            && may_replace_body
            && *variadic == binding.variadic
            && fixed.len() == binding.params.len()
            && fixed
                .iter()
                .zip(binding.params)
                .all(|(parameter, arg)| self.matches_arg(&parameter.value.ty, *arg))
            && function
                .return_type
                .as_ref()
                .is_some_and(|ret| self.matches_arg(ret, binding.ret))
    }

    pub(super) fn bind_stdio(&mut self, module: &ir::Module) {
        if self.target.char_signed != self.rust_layout.char_signed {
            return;
        }
        self.stdio.file_records = module
            .types
            .iter()
            .filter(|definition| {
                matches!(definition.kind, ir::TypeDefinitionKind::Record { .. })
                    && definition
                        .name
                        .as_deref()
                        .is_some_and(|name| FILE_RECORDS.contains(&name))
            })
            .map(|definition| definition.value.id)
            .collect();
        let symbols: Option<[&str; 3]> = match self.target.os {
            TargetOs::Linux => Some(STREAMS),
            TargetOs::Darwin | TargetOs::FreeBsd => Some(["__stdinp", "__stdoutp", "__stderrp"]),
            TargetOs::Android | TargetOs::Windows => None,
        };
        let mut stream_globals = HashMap::new();
        let mut android_streams = None;
        for global in module.globals.iter().filter(|global| !global.definition) {
            let variable = &global.variable;
            if let Some(index) = symbols
                .and_then(|symbols| symbols.iter().position(|symbol| *symbol == variable.name))
                && self.is_file_pointer(&variable.ty)
            {
                stream_globals.insert(variable.id, STREAMS[index]);
            }
            if matches!(self.target.os, TargetOs::Android)
                && variable.name == "__sF"
                && matches!(self.resolve_type(&variable.ty), ir::Type::Array { element, .. } if self.is_file(element))
            {
                android_streams = Some(variable.id);
            }
        }
        self.stdio.stream_globals = stream_globals;
        self.stdio.android_streams = android_streams;
        let functions: HashSet<BindingId> = module
            .functions
            .iter()
            .filter(|function| self.matches_binding(function))
            .map(|function| function.value.id)
            .collect();
        for id in &functions {
            if let Some(name) = self.names.get_mut(id) {
                name.is_unsafe = true;
            }
        }
        self.stdio.functions = functions;
    }
}

fn constant_index(value: &ir::Value) -> Option<usize> {
    match &value.node.value {
        ValueKind::Constant(ir::Number::Integer(number)) => {
            let index: usize = number.to_string().parse().ok()?;
            (index < STREAMS.len()).then_some(index)
        }
        ValueKind::Constant(ir::Number::SignedInteger(number)) => {
            let index: usize = number.to_string().parse().ok()?;
            (index < STREAMS.len()).then_some(index)
        }
        ValueKind::Copy { operand, .. } | ValueKind::Convert { operand, .. } => {
            constant_index(operand)
        }
        _ => None,
    }
}

impl FunctionLowerer<'_, '_> {
    fn stream_name(&self, value: &ir::Value) -> Option<&'static str> {
        let tables = self.tables;
        if tables.stdio.file_records.is_empty() {
            return None;
        }
        match &value.node.value {
            ValueKind::Read { place, .. } => {
                let PlaceKind::Binding(id) = place.kind else {
                    return None;
                };
                tables.stdio.stream_globals.get(&id).copied()
            }
            ValueKind::AddressOf(place) => {
                let PlaceKind::Deref(pointer) = &place.kind else {
                    return None;
                };
                let ValueKind::PointerOffset {
                    pointer,
                    amount,
                    subtract: false,
                    ..
                } = &pointer.node.value
                else {
                    return None;
                };
                let ValueKind::ArrayDecay { place, .. } = &pointer.node.value else {
                    return None;
                };
                let PlaceKind::Binding(id) = place.kind else {
                    return None;
                };
                (Some(id) == tables.stdio.android_streams)
                    .then(|| constant_index(amount))
                    .flatten()
                    .map(|index| STREAMS[index])
            }
            ValueKind::Call {
                callee: ir::Callee::Direct(id),
                arguments,
                ..
            } if matches!(tables.target.os, TargetOs::Windows)
                && tables.is_file_pointer(&value.ty)
                && tables
                    .names
                    .get(id)
                    .is_some_and(|name| name.is_extern && name.rust == "__acrt_iob_func") =>
            {
                let [index] = arguments.as_slice() else {
                    return None;
                };
                constant_index(index).map(|index| STREAMS[index])
            }
            _ => None,
        }
    }

    pub(super) fn lower_stdio_stream(&self, value: &ir::Value) -> Option<Expr> {
        self.stream_name(value).map(|name| Expr::Call {
            func: Box::new(support_path(name)),
            args: Vec::new(),
            binding: CallBinding::Generated,
        })
    }
}
