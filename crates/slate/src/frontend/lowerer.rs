use super::long_double;
use crate::backend::rust_ast::{self as rust, Attr, BinOp, Expr, FnDef, FnParam, Item, Prim, Stmt};
use crate::function_identity::CallBinding;
use crate::function_identity::FunctionIdentity;
use slate_parser::ast::Span;
use slate_parser::ir::{self, BindingId, Number, PlaceKind, TypeId, ValueKind};
use slate_parser::target_info::TargetInfo;
use std::collections::{BTreeMap, BTreeSet, HashMap, HashSet};

mod arithmetic;
mod asm;
mod atomics;
mod calls;
mod comment_text;
mod comments;
mod control_flow;
mod errors;
mod f80;
mod functions;
mod globals;
mod intrinsics;
mod intrinsics_table;
mod names;
mod places;
mod pointers;
mod statements;
mod switch;
mod target_features;
mod types;
mod values;
mod vectors;
mod vla;

use arithmetic::*;
use calls::*;
pub use errors::{Barrier, Construct, Context, InvalidIr, Invariant, Site};
use errors::{Failure, variant_name};
use f80::*;
use functions::*;
use globals::*;
use names::*;
use pointers::*;
use statements::*;
use types::*;
use values::*;

type Result<T> = std::result::Result<T, Failure>;

struct FunctionName {
    rust: String,
    builtin: Option<String>,
    codegen_builtin: bool,
    is_extern: bool,
    is_unsafe: bool,
    is_variadic: bool,
    exported: bool,
}

struct Tables<'m> {
    names: HashMap<BindingId, FunctionName>,
    intrinsics: HashMap<BindingId, &'static intrinsics_table::IntrinsicSignature>,
    bindings: HashMap<BindingId, String>,
    strings: HashMap<BindingId, Vec<u8>>,
    statics: HashSet<BindingId>,
    register_globals: HashMap<BindingId, &'static str>,
    over_aligned: HashMap<BindingId, u64>,
    target: &'m TargetInfo,
    metadata: &'m ir::Metadata,
    comments: &'m ir::Comments,
    promote_docs: bool,
    unit: &'m str,
    types: HashMap<TypeId, &'m Span<ir::TypeDefinition>>,
    record_names: HashMap<TypeId, String>,
}

#[derive(Default)]
struct Dependencies {
    atomic_volatile: bool,
    asm_unwind: bool,
    atomic128: bool,
    long_double: bool,
    simd: bool,
    intrinsics: BTreeMap<String, rust::ExternFnDecl>,
    bridges: BTreeMap<String, rust::ExternFnDecl>,
    align_wrappers: BTreeSet<u32>,
    records: BTreeMap<u32, Record>,
    compound_literals: Vec<Item>,
    address_taken: BTreeSet<String>,
    taken_functions: HashSet<BindingId>,
    emitted_comments: HashSet<slate_parser::ast::NodeId>,
    native_entries: BTreeMap<String, rust::ExternFnDecl>,
    long_double_exports: BTreeMap<String, String>,
    bit_units: Vec<Item>,
}

enum Record {
    Building,
    Built(rust::RecordDef),
    Failed(Failure),
}

struct FunctionLowerer<'a, 'm> {
    tables: &'a Tables<'m>,
    dependencies: &'a mut Dependencies,
    temps: u32,
    hoisted: Vec<Stmt>,
    dispatch_bindings: HashSet<BindingId>,
    aligned_locals: HashSet<BindingId>,
    register_locals: HashSet<BindingId>,
    old_value: Option<Expr>,
}

impl FunctionLowerer<'_, '_> {
    fn next_temp(&mut self) -> String {
        let index = self.temps;
        self.temps += 1;
        format!("__t{index}")
    }
}

#[derive(Debug, Default, Clone)]
pub struct LowerOptions {
    pub export_symbols: bool,
    pub explicit_docs_only: bool,
    pub imported_commons: BTreeSet<String>,
    pub unit: String,
}

pub struct Lowered {
    pub program: rust::Program,
    pub barriers: Vec<Barrier>,
    pub address_taken: HashSet<BindingId>,
}

pub fn lower(
    module: &ir::Module,
    options: &LowerOptions,
) -> std::result::Result<Lowered, InvalidIr> {
    let mut lowerer = ModuleLowerer::new(module, options)?;
    lowerer.declare()?;
    lowerer.lower_bodies()?;
    Ok(lowerer.assemble())
}

fn exports_symbol(options: &LowerOptions, function: &ir::Function) -> bool {
    options.export_symbols
        && matches!(function.linkage, ir::Linkage::External)
        && !function.semantics.inline_only
        && function.name != "main"
}

pub(crate) fn function_rust_name(function: &ir::Function) -> String {
    if function.name == "main" {
        "__slate_main".into()
    } else {
        function.name.clone()
    }
}

pub fn definition_items(module: &ir::Module) -> Vec<(String, slate_parser::ast::Loc)> {
    let function_names: Vec<_> = module
        .functions
        .iter()
        .map(|function| function_rust_name(function))
        .collect();
    let names = global_names(module, function_names.iter().map(String::as_str));
    let functions = module
        .functions
        .iter()
        .filter(|function| function.body.is_some())
        .flat_map(|function| {
            let rust = (
                format!("fn:{}", function_rust_name(function)),
                function.expansion,
            );
            let wrapper =
                (function.name == "main").then(|| ("fn:main".to_owned(), function.expansion));
            std::iter::once(rust).chain(wrapper)
        });
    let statics = module
        .globals
        .iter()
        .filter(|global| global.definition)
        .map(|global| {
            (
                format!("static:{}", names[&global.variable.id]),
                global.expansion,
            )
        });
    functions.chain(statics).collect()
}

pub fn describe_types(message: &str, module: &ir::Module) -> String {
    let mut out = String::with_capacity(message.len());
    let mut rest = message;
    while let Some(start) = rest.find("@type") {
        let digits = rest[start + 5..]
            .find(|c: char| !c.is_ascii_digit())
            .unwrap_or(rest.len() - start - 5);
        let end = start + 5 + digits;
        out.push_str(&rest[..end]);
        let definition = rest[start + 5..end].parse::<u32>().ok().and_then(|id| {
            module
                .types
                .iter()
                .find(|definition| definition.value.id.0 == id)
        });
        if let Some(definition) = definition {
            let kind = match &definition.kind {
                ir::TypeDefinitionKind::Alias(_) => "typedef",
                ir::TypeDefinitionKind::Record {
                    kind: ir::RecordKind::Struct,
                    ..
                } => "struct",
                ir::TypeDefinitionKind::Record {
                    kind: ir::RecordKind::Union,
                    ..
                } => "union",
                ir::TypeDefinitionKind::Enum { .. } => "enum",
            };
            out.push_str(&format!(
                "({kind} {})",
                definition.name.as_deref().unwrap_or("<anonymous>")
            ));
        }
        rest = &rest[end..];
    }
    out.push_str(rest);
    out
}

struct ModuleLowerer<'m> {
    module: &'m ir::Module,
    options: &'m LowerOptions,
    tables: Tables<'m>,
    dependencies: Dependencies,
    items: Vec<Item>,
    externs: Vec<rust::ExternDecl>,
    barriers: Vec<Barrier>,
}

impl<'m> ModuleLowerer<'m> {
    fn new(
        module: &'m ir::Module,
        options: &'m LowerOptions,
    ) -> std::result::Result<Self, InvalidIr> {
        let mut barriers = Vec::new();
        if let Some(asm) = module.asm.first() {
            barriers.push(Failure::from(Construct::ModuleAsm).into_public(None, Site::of(asm))?);
        }
        let mut strings = HashMap::new();
        let mut statics = Vec::new();
        let mut register_globals = HashMap::new();
        for global in &module.globals {
            match register_global(global, &module.target) {
                Some(Ok(register)) => {
                    register_globals.insert(global.variable.id, register);
                    continue;
                }
                Some(Err(error)) => {
                    barriers.push(error.into_public(None, Site::of(global))?);
                    continue;
                }
                None => {}
            }
            match lower_string_global(global) {
                Some(Ok(bytes)) => {
                    strings.insert(global.variable.id, bytes);
                }
                Some(Err(error)) => barriers.push(error.into_public(None, Site::of(global))?),
                None => statics.push(&global.value),
            }
        }
        let names: HashMap<_, _> = module
            .functions
            .iter()
            .map(|function| {
                (
                    function.value.id,
                    FunctionName {
                        rust: function_rust_name(function),
                        builtin: module.metadata.get(&function.id).and_then(|entries| {
                            entries.iter().find_map(|(key, value)| {
                                (key == "c_builtin").then(|| value.clone())
                            })
                        }),
                        codegen_builtin: module
                            .metadata
                            .get(&function.id)
                            .is_some_and(|entries| codegen_builtin(entries)),
                        is_extern: function.body.is_none(),
                        is_unsafe: function.body.is_none()
                            || !function.semantics.target.is_empty()
                            || matches!(
                                function.parameters,
                                ir::Parameters::Prototype { variadic: true, .. }
                            ),
                        is_variadic: !matches!(
                            function.parameters,
                            ir::Parameters::Prototype {
                                variadic: false,
                                ..
                            }
                        ),
                        exported: function.body.is_some() && exports_symbol(options, function),
                    },
                )
            })
            .collect();
        let mut bindings = global_names(module, names.values().map(|name| name.rust.as_str()));
        let static_names: HashSet<_> = statics
            .iter()
            .map(|global| bindings[&global.variable.id].clone())
            .collect();
        for function in &module.functions {
            if let ir::Parameters::Prototype { fixed, .. } = &function.value.parameters {
                for parameter in fixed {
                    let id = parameter.value.id;
                    bindings.insert(
                        id,
                        parameter
                            .value
                            .name
                            .as_deref()
                            .map(|name| local_binding_name(name, id, &static_names))
                            .unwrap_or_else(|| format!("__v{}", id.0)),
                    );
                }
            }
            if let Some(body) = &function.value.body {
                collect_statement_names(body, &mut bindings, &static_names);
            }
        }
        let mut tables = Tables {
            names,
            intrinsics: module
                .functions
                .iter()
                .filter_map(|function| {
                    Some((
                        function.value.id,
                        intrinsics::builtin_intrinsic(module, function)?,
                    ))
                })
                .collect(),
            bindings,
            strings,
            statics: statics.iter().map(|global| global.variable.id).collect(),
            register_globals,
            over_aligned: HashMap::new(),
            target: &module.target,
            metadata: &module.metadata,
            comments: &module.comments,
            promote_docs: !options.explicit_docs_only,
            unit: &options.unit,
            types: module
                .types
                .iter()
                .map(|definition| (definition.value.id, definition))
                .collect(),
            record_names: record_names(module),
        };
        tables.over_aligned = statics
            .iter()
            .filter(|global| global.definition)
            .filter_map(|global| {
                Some((
                    global.variable.id,
                    tables.over_alignment(&global.variable.ty, global.variable.alignment)?,
                ))
            })
            .collect();
        Ok(Self {
            module,
            options,
            tables,
            dependencies: Dependencies::default(),
            items: Vec::new(),
            externs: Vec::new(),
            barriers,
        })
    }

    fn lowerer(&mut self) -> FunctionLowerer<'_, 'm> {
        FunctionLowerer {
            tables: &self.tables,
            dependencies: &mut self.dependencies,
            temps: 0,
            hoisted: Vec::new(),
            dispatch_bindings: HashSet::new(),
            aligned_locals: HashSet::new(),
            register_locals: HashSet::new(),
            old_value: None,
        }
    }

    fn declare(&mut self) -> std::result::Result<(), InvalidIr> {
        let module = self.module;
        for global in &module.globals {
            if !self.tables.statics.contains(&global.variable.id) {
                continue;
            }
            let name = binding_name(global.variable.id, &self.tables.bindings);
            let imported = global.common
                && self
                    .options
                    .imported_commons
                    .contains(&global.variable.name);
            let exported =
                self.options.export_symbols && matches!(global.linkage, ir::Linkage::External);
            let thread_local: Vec<Attr> =
                matches!(global.variable.storage, ir::StorageDuration::Thread)
                    .then_some(Attr::ThreadLocal)
                    .into_iter()
                    .collect();
            match self.lowerer().lower_static(global) {
                Ok((ty, Some(_))) if imported => self.externs.push(rust::ExternDecl::Static {
                    attrs: thread_local,
                    mutable: true,
                    name,
                    ty,
                }),
                Ok((ty, Some(init))) => {
                    let comments = self.lowerer().claim_comments(global.id);
                    self.items.push(Item::Static {
                        comments,
                        attrs: thread_local
                            .into_iter()
                            .chain(exported.then_some(Attr::NoMangle))
                            .collect(),
                        vis: rust::Visibility::Private,
                        mutable: true,
                        name,
                        ty,
                        init,
                    });
                }
                Ok((ty, None)) => self.externs.push(rust::ExternDecl::Static {
                    attrs: thread_local,
                    mutable: true,
                    name,
                    ty,
                }),
                Err(error) => self.barriers.push(
                    error
                        .within(
                            Site::of(global),
                            format!("in global `{}`", global.variable.name),
                        )
                        .into_public(None, Site::of(global))?,
                ),
            }
        }
        for function in &module.functions {
            if function.body.is_some()
                || self.tables.intrinsics.contains_key(&function.value.id)
                || self.tables.passes_long_double(function)
            {
                continue;
            }
            match self.lowerer().lower_extern(function) {
                Ok(decl) => self.externs.push(decl),
                Err(error) => self.barriers.push(
                    error
                        .within(
                            Site::of(function),
                            format!("in declaration of `{}`", function.name),
                        )
                        .into_public(Some(&function.name), Site::of(function))?,
                ),
            }
        }
        Ok(())
    }

    fn lower_bodies(&mut self) -> std::result::Result<(), InvalidIr> {
        let module = self.module;
        for function in &module.functions {
            let Some(body) = &function.body else {
                continue;
            };
            let lowered = if self.options.export_symbols
                && matches!(function.linkage, ir::Linkage::External)
                && !function.semantics.inline_only
                && self.tables.function_has_vector(function)
            {
                Err(Construct::Function {
                    name: function.name.clone(),
                    detail: "vector C ABI".into(),
                }
                .into())
            } else {
                self.lowerer().lower_function(function, body)
            };
            let exported = exports_symbol(self.options, function);
            let lowered = match lowered {
                Ok(Item::Fn(mut definition)) if exported => {
                    let export = if self.tables.passes_long_double(function)
                        && !function_is_variadic(function)
                    {
                        long_double_export_name(&function.name, &definition).map(Attr::ExportName)
                    } else {
                        Ok(Attr::NoMangle)
                    };
                    export.map(|attr| {
                        definition.attrs.push(attr);
                        definition.abi.get_or_insert(rust::Abi::CUnwind);
                        Item::Fn(definition)
                    })
                }
                lowered => lowered,
            };
            match lowered {
                Ok(item) => self.items.push(item),
                Err(error) => self.barriers.push(
                    error
                        .within(
                            Site::of(function),
                            format!("in function `{}`", function.name),
                        )
                        .into_public(Some(&function.name), Site::of(function))?,
                ),
            }
        }
        Ok(())
    }

    fn assemble(self) -> Lowered {
        let Self {
            tables,
            dependencies,
            mut items,
            mut externs,
            barriers,
            ..
        } = self;
        let wrappers = dependencies.align_wrappers.iter().map(|&alignment| {
            Item::Struct(rust::StructDef {
                attrs: vec![Attr::Repr(vec![
                    rust::Repr::C,
                    rust::Repr::Align(alignment),
                ])],
                vis: rust::Visibility::Private,
                field_vis: rust::Visibility::Private,
                generics: vec![rust::GenericParam {
                    name: "T".into(),
                    bounds: Vec::new(),
                }],
                name: align_wrapper(alignment.into()),
                fields: rust::StructFields::Tuple(vec![rust::Type::Custom("T".into())]),
            })
        });
        let records = dependencies
            .records
            .into_values()
            .filter_map(|record| match record {
                Record::Built(record) => Some(Item::Record(record)),
                Record::Building | Record::Failed(_) => None,
            });
        let bit_units = (!dependencies.bit_units.is_empty()).then(|| Item::InlineMod {
            vis: rust::Visibility::Private,
            name: rust::Ident::new(BIT_UNIT_MODULE),
            items: dependencies.bit_units,
        });
        items.splice(0..0, records.chain(wrappers).chain(bit_units));
        items.extend(dependencies.compound_literals);
        for item in &mut items {
            if let Item::Fn(function) = item
                && function.abi.is_none()
                && dependencies.address_taken.contains(&function.name)
            {
                function.abi = Some(rust::Abi::CUnwind);
            }
            if let Item::Fn(function) = item
                && let Some(export) = dependencies.long_double_exports.get(&function.name)
            {
                function.attrs.push(Attr::ExportName(export.clone()));
                function.abi = Some(rust::Abi::CUnwind);
            }
        }
        externs.extend(
            dependencies
                .native_entries
                .into_values()
                .map(rust::ExternDecl::Fn),
        );
        if dependencies.long_double {
            items.splice(
                0..0,
                long_double::long_double_prelude(rust::Visibility::Private),
            );
            externs.extend(
                long_double::f80_shim_decls()
                    .into_iter()
                    .filter(|decl| decl.name.starts_with("__slate_f80_"))
                    .map(rust::ExternDecl::Fn),
            );
        }
        externs.extend(dependencies.bridges.into_values().map(rust::ExternDecl::Fn));
        if !dependencies.intrinsics.is_empty() {
            items.insert(
                0,
                Item::ExternBlock {
                    abi: "llvm-intrinsic".into(),
                    decls: dependencies
                        .intrinsics
                        .into_values()
                        .map(rust::ExternDecl::Fn)
                        .collect(),
                },
            );
        }
        if !externs.is_empty() {
            items.insert(
                0,
                Item::ExternBlock {
                    abi: "C".into(),
                    decls: externs,
                },
            );
        }
        let mut features = vec![rust::CrateAttr::Allow(vec![rust::Lint::NonCamelCaseTypes])];
        if dependencies.asm_unwind {
            features.push(rust::CrateAttr::Feature(rust::Feature::AsmUnwind));
        }
        if dependencies.atomic128 {
            items.insert(
                0,
                Item::SupportModule(rust::SupportModule {
                    name: "__slate_atomic128".into(),
                    source: include_str!("support/atomic128.rs").into(),
                    exports: Vec::new(),
                }),
            );
        }
        if dependencies.atomic_volatile {
            features.push(rust::CrateAttr::Feature(rust::Feature::AtomicVolatile));
        }
        if items
            .iter()
            .any(|item| matches!(item, Item::ExternBlock { abi, .. } if abi == "llvm-intrinsic"))
        {
            features.push(rust::CrateAttr::Feature(rust::Feature::LinkLlvmIntrinsics));
        }
        if items.iter().any(|item| match item {
            Item::Static { attrs, .. } => attrs.contains(&Attr::ThreadLocal),
            Item::ExternBlock { decls, .. } => decls.iter().any(|decl| {
                matches!(decl, rust::ExternDecl::Static { attrs, .. } if attrs.contains(&Attr::ThreadLocal))
            }),
            _ => false,
        }) {
            features.push(rust::CrateAttr::Feature(rust::Feature::ThreadLocal));
        }
        if items.iter().any(|item| {
            matches!(item, Item::Fn(definition) if definition.attrs.iter().any(|attr| {
                matches!(attr, Attr::TargetFeature(enabled) if enabled.split(',').any(|feature| feature == "rtm"))
            }))
        }) {
            features.push(rust::CrateAttr::Feature(rust::Feature::RtmTargetFeature));
        }
        if dependencies.simd {
            features.extend([
                rust::CrateAttr::Feature(rust::Feature::PortableSimd),
                rust::CrateAttr::Feature(rust::Feature::SimdFfi),
            ]);
        }
        if let Some(main) = self
            .module
            .functions
            .iter()
            .find(|function| tables.names[&function.value.id].rust == "__slate_main")
        {
            let arity = match &main.parameters {
                ir::Parameters::Prototype { fixed, .. } => fixed.len(),
                _ => 0,
            };
            if matches!(arity, 0 | 2 | 3) {
                features.push(rust::CrateAttr::NoMain);
                items.push(main_wrapper(arity));
            }
        }
        items.insert(0, Item::CrateAttrs(features));
        let items =
            comments::module_comments(self.module, &tables, &dependencies.emitted_comments, items);
        Lowered {
            program: rust::Program { items },
            barriers,
            address_taken: dependencies.taken_functions,
        }
    }
}
