use super::long_double;
use crate::backend::rust_ast::{self as rust, Attr, BinOp, Expr, FnDef, FnParam, Item, Prim, Stmt};
use crate::function_identity::CallBinding;
use crate::function_identity::FunctionIdentity;
use slate_parser::ast::Span;
use slate_parser::ir::{self, BindingId, Number, PlaceKind, TypeId, ValueKind};
use slate_parser::target_info::TargetInfo;
use std::collections::{BTreeMap, BTreeSet, HashMap, HashSet};

mod arithmetic;
mod calls;
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
mod types;
mod values;

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
    is_extern: bool,
    is_unsafe: bool,
    is_variadic: bool,
}

struct Tables<'m> {
    names: HashMap<BindingId, FunctionName>,
    intrinsics: HashMap<BindingId, &'static intrinsics_table::IntrinsicSignature>,
    bindings: HashMap<BindingId, String>,
    strings: HashMap<BindingId, Vec<u8>>,
    statics: HashSet<BindingId>,
    over_aligned: HashMap<BindingId, u64>,
    target: &'m TargetInfo,
    metadata: &'m ir::Metadata,
    types: HashMap<TypeId, &'m Span<ir::TypeDefinition>>,
    record_names: HashMap<TypeId, String>,
}

#[derive(Default)]
struct Dependencies {
    long_double: bool,
    intrinsics: BTreeMap<String, rust::ExternFnDecl>,
    bridges: BTreeMap<String, rust::ExternFnDecl>,
    align_wrappers: BTreeSet<u32>,
    records: BTreeMap<u32, Record>,
    compound_literals: Vec<Item>,
    address_taken: BTreeSet<String>,
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
    pub imported_commons: BTreeSet<String>,
}

pub struct Lowered {
    pub program: rust::Program,
    pub barriers: Vec<Barrier>,
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

fn function_rust_name(function: &ir::Function) -> String {
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
        for global in &module.globals {
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
                        is_extern: function.body.is_none(),
                        is_unsafe: function.body.is_none()
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
            over_aligned: HashMap::new(),
            target: &module.target,
            metadata: &module.metadata,
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
                Some((global.variable.id, tables.over_alignment(&global.variable)?))
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
            match self.lowerer().lower_static(global) {
                Ok((ty, Some(_))) if imported => self.externs.push(rust::ExternDecl::Static {
                    attrs: Vec::new(),
                    mutable: true,
                    name,
                    ty,
                }),
                Ok((ty, Some(init))) => self.items.push(Item::Static {
                    attrs: if exported {
                        vec![Attr::NoMangle]
                    } else {
                        Vec::new()
                    },
                    vis: rust::Visibility::Private,
                    mutable: true,
                    name,
                    ty,
                    init,
                }),
                Ok((ty, None)) => self.externs.push(rust::ExternDecl::Static {
                    attrs: Vec::new(),
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
            match self.lowerer().lower_function(function, body) {
                Ok(Item::Fn(mut definition))
                    if self.options.export_symbols
                        && matches!(function.linkage, ir::Linkage::External)
                        && !function.semantics.inline_only
                        && function.name != "main" =>
                {
                    definition.attrs.push(Attr::NoMangle);
                    definition.abi.get_or_insert(rust::Abi::CUnwind);
                    self.items.push(Item::Fn(definition));
                }
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
        }
        if dependencies.long_double {
            items.splice(
                0..0,
                long_double::long_double_prelude(rust::Visibility::Private),
            );
            externs.extend(
                long_double::f80_shim_decls()
                    .into_iter()
                    .filter(|decl| decl.name.starts_with("__slate_f80_"))
                    .chain(dependencies.bridges.into_values())
                    .map(rust::ExternDecl::Fn),
            );
        }
        if !dependencies.intrinsics.is_empty() {
            items.insert(
                0,
                Item::CrateAttrs(vec![rust::CrateAttr::Feature(
                    rust::Feature::LinkLlvmIntrinsics,
                )]),
            );
            items.insert(
                1,
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
                usize::from(matches!(items.first(), Some(Item::CrateAttrs(_)))),
                Item::ExternBlock {
                    abi: "C".into(),
                    decls: externs,
                },
            );
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
            if matches!(arity, 0 | 2) {
                items.push(main_wrapper(arity));
            }
        }
        Lowered {
            program: rust::Program { items },
            barriers,
        }
    }
}
