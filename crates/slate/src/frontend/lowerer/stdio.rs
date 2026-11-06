use slate_parser::target_info::{Endian, TargetOs};

use super::*;
use crate::backend::engine::shadow::visit_names;
use crate::function_identity::Known;

const SUPPORT_CRATE: &str = "slate_support";
const FILE_RECORDS: &[&str] = &["_IO_FILE", "__sFILE", "_iobuf"];
const STREAMS: [&str; 3] = ["stdin", "stdout", "stderr"];

#[derive(Clone, Copy, PartialEq)]
enum Arg {
    File,
    Text,
    Int,
    IntPointer,
}

struct Binding {
    name: &'static str,
    support: &'static str,
    params: &'static [Arg],
    variadic: bool,
    ret: Arg,
    replaces_body: bool,
}

const fn binding(name: &'static str, params: &'static [Arg], variadic: bool, ret: Arg) -> Binding {
    Binding {
        name,
        support: name,
        params,
        variadic,
        ret,
        replaces_body: false,
    }
}

const fn errno_binding(name: &'static str) -> Binding {
    Binding {
        name,
        support: "errno_location",
        params: &[],
        variadic: false,
        ret: Arg::IntPointer,
        replaces_body: false,
    }
}

const fn ctype_binding(name: &'static str, support: &'static str) -> Binding {
    Binding {
        name,
        support,
        params: &[Arg::Int],
        variadic: false,
        ret: Arg::Int,
        replaces_body: true,
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
    errno_binding("__errno_location"),
    errno_binding("__errno"),
    errno_binding("__error"),
    errno_binding("_errno"),
    ctype_binding("isalnum", "isalnum"),
    ctype_binding("isalpha", "isalpha"),
    ctype_binding("isblank", "isblank"),
    ctype_binding("iscntrl", "iscntrl"),
    ctype_binding("isdigit", "isdigit"),
    ctype_binding("isgraph", "isgraph"),
    ctype_binding("islower", "islower"),
    ctype_binding("isprint", "isprint"),
    ctype_binding("ispunct", "ispunct"),
    ctype_binding("isspace", "isspace"),
    ctype_binding("isupper", "isupper"),
    ctype_binding("isxdigit", "isxdigit"),
    ctype_binding("tolower", "tolower"),
    ctype_binding("toupper", "toupper"),
    ctype_binding("__sbtolower", "tolower"),
    ctype_binding("__sbtoupper", "toupper"),
];

pub(super) fn ctype_identity(support: &str) -> FunctionIdentity {
    Known::from_symbol(support).map_or(FunctionIdentity::Unknown, FunctionIdentity::Known)
}

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
    pub(super) functions: HashMap<BindingId, &'static str>,
    pub(super) stream_globals: HashMap<BindingId, &'static str>,
    pub(super) android_streams: Option<BindingId>,
    pub(super) glibc_ctype_table: Option<BindingId>,
    pub(super) freebsd_ctype_test: Option<BindingId>,
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
        Item::MacroRules { body, .. } => body.iter().for_each(&mut collect),
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
                && !tables.stdio.functions.contains_key(&function.value.id)
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
                            && !global.symbol.used
                            && (!global.definition
                                || !matches!(global.linkage, ir::Linkage::External))
                    })
                    .map(|global| global.variable.id)
                    .chain(tables.stdio.stream_globals.keys().copied())
                    .map(|id| (binding_name(id, &tables.bindings), id)),
            )
            .collect()
    }

    pub(super) fn unreachable_stdio_helpers(&self) -> HashSet<BindingId> {
        let stdio = &self.tables.stdio;
        if (stdio.file_records.is_empty() && stdio.functions.is_empty())
            || !self.module.asm.is_empty()
        {
            return HashSet::new();
        }
        let candidates = self.unreachable_candidates();
        let mut pending: Vec<String> = Vec::new();
        let mut candidate_references: HashMap<&str, BTreeSet<String>> = HashMap::new();
        for item in self
            .items
            .iter()
            .chain(&self.dependencies.compound_literals)
            .chain(&self.dependencies.block_items)
            .chain(self.dependencies.macro_templates.values())
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
            Arg::IntPointer => matches!(
                self.resolve_type(ty),
                ir::Type::Pointer { pointee, is_const: false, .. }
                    if self.matches_arg(pointee, Arg::Int)
            ),
        }
    }

    fn matching_binding(
        &self,
        function: &slate_parser::ast::Span<ir::Function>,
    ) -> Option<&'static Binding> {
        let binding = BINDINGS.iter().find(|known| known.name == function.name)?;
        let ir::Parameters::Prototype { fixed, variadic } = &function.parameters else {
            return None;
        };
        let symbol = function.symbol.asm_name.as_deref();
        let plain_symbol = symbol.is_none_or(|symbol| {
            symbol == function.name || symbol.strip_prefix('_') == Some(function.name.as_str())
        });
        let may_replace_body = function.body.is_none()
            || binding.replaces_body
            || function.name == "__sputc"
            || matches!(self.target.os, TargetOs::Windows);
        (function.provenance.system_header.is_some()
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
                .is_some_and(|ret| self.matches_arg(ret, binding.ret)))
        .then_some(binding)
    }

    fn system_function(&self, module: &ir::Module, name: &str) -> Option<BindingId> {
        module
            .functions
            .iter()
            .find(|function| function.name == name && function.provenance.system_header.is_some())
            .map(|function| function.value.id)
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
        let matched: Vec<(BindingId, &'static Binding)> = module
            .functions
            .iter()
            .filter_map(|function| Some((function.value.id, self.matching_binding(function)?)))
            .collect();
        for (id, binding) in &matched {
            if let Some(name) = self.names.get_mut(id) {
                name.is_unsafe = true;
                if binding.replaces_body {
                    name.identity = ctype_identity(binding.support);
                } else if let Some(known) = Known::from_symbol(binding.support) {
                    name.identity = FunctionIdentity::Known(known);
                }
            }
        }
        self.stdio.functions = matched
            .into_iter()
            .map(|(id, binding)| (id, binding.support))
            .collect();
        if matches!(self.target.os, TargetOs::Linux)
            && matches!(self.target.endian, Endian::Little)
            && !self.target.triple.contains("musl")
        {
            self.stdio.glibc_ctype_table = self.system_function(module, "__ctype_b_loc");
        }
        if matches!(self.target.os, TargetOs::FreeBsd) {
            self.stdio.freebsd_ctype_test = self.system_function(module, "__sbistype");
        }
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
