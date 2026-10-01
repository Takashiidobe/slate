use super::Error;
use super::long_double;
use crate::backend::rust_ast::{self as rust, Attr, BinOp, Expr, FnDef, FnParam, Item, Prim, Stmt};
use crate::function_identity::CallBinding;
use crate::function_identity::FunctionIdentity;
use slate_parser::ir::{self, BindingId, Number, PlaceKind, TypeId, ValueKind};
use slate_parser::target_info::TargetInfo;
use std::cell::{Cell, RefCell};
use std::collections::{BTreeMap, BTreeSet, HashMap, HashSet};

mod arithmetic;
mod calls;
mod f80;
mod functions;
mod globals;
mod names;
mod places;
mod pointers;
mod statements;
mod switch;
mod types;
mod values;

use arithmetic::*;
use calls::*;
use f80::*;
use functions::*;
use globals::*;
use names::*;
use places::*;
use pointers::*;
use statements::*;
use switch::*;
use types::*;
use values::*;

type Result<T> = std::result::Result<T, super::Error>;

struct FunctionName {
    rust: String,
    is_extern: bool,
    is_unsafe: bool,
    is_variadic: bool,
}

struct Context<'a> {
    names: HashMap<BindingId, FunctionName>,
    bindings: HashMap<BindingId, String>,
    strings: HashMap<BindingId, Vec<u8>>,
    statics: HashSet<BindingId>,
    over_aligned: HashMap<BindingId, u64>,
    target: &'a TargetInfo,
    types: HashMap<TypeId, &'a ir::TypeDefinition>,
    record_names: HashMap<TypeId, String>,
    records: RefCell<BTreeMap<u32, Option<std::result::Result<rust::RecordDef, String>>>>,
    temps: Cell<u32>,
    long_double: Cell<bool>,
    bridges: RefCell<BTreeMap<String, rust::ExternFnDecl>>,
}

impl Context<'_> {
    fn next_temp(&self) -> String {
        let index = self.temps.get();
        self.temps.set(index + 1);
        format!("__t{index}")
    }
}

#[derive(Default)]
pub struct Report {
    pub module: Vec<String>,
    pub declarations: Vec<(String, String)>,
    pub functions: Vec<(String, Option<String>)>,
}

impl Report {
    pub fn is_clean(&self) -> bool {
        self.module.is_empty()
            && self.declarations.is_empty()
            && self.functions.iter().all(|(_, barrier)| barrier.is_none())
    }
}

pub fn lower(module: &ir::Module) -> Result<rust::Program> {
    lower_module(module, None)
}

pub fn report(module: &ir::Module) -> Report {
    let mut report = Report::default();
    lower_module(module, Some(&mut report)).expect("report mode records barriers");
    let describe = |error: &mut String| *error = describe_types(error, module);
    report.module.iter_mut().for_each(describe);
    report
        .declarations
        .iter_mut()
        .for_each(|(_, error)| describe(error));
    report
        .functions
        .iter_mut()
        .filter_map(|(_, error)| error.as_mut())
        .for_each(describe);
    report
}

fn describe_types(message: &str, module: &ir::Module) -> String {
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

fn module_barrier(report: &mut Option<&mut Report>, error: super::Error) -> Result<()> {
    match report {
        Some(report) => {
            report.module.push(error.to_string());
            Ok(())
        }
        None => Err(error),
    }
}

fn function_barrier<T>(
    report: &mut Option<&mut Report>,
    name: &str,
    defined: bool,
    result: Result<T>,
) -> Result<Option<T>> {
    match (result, report) {
        (Ok(value), Some(report)) => {
            if defined {
                report.functions.push((name.to_owned(), None));
            }
            Ok(Some(value))
        }
        (Ok(value), None) => Ok(Some(value)),
        (Err(error), Some(report)) if defined => {
            report
                .functions
                .push((name.to_owned(), Some(error.to_string())));
            Ok(None)
        }
        (Err(error), Some(report)) => {
            report
                .declarations
                .push((name.to_owned(), error.to_string()));
            Ok(None)
        }
        (Err(error), None) => Err(error),
    }
}

fn lower_module(module: &ir::Module, mut report: Option<&mut Report>) -> Result<rust::Program> {
    if !module.asm.is_empty() {
        module_barrier(
            &mut report,
            super::Error::Unsupported("module assembly".into()),
        )?;
    }
    let mut strings = HashMap::new();
    let mut statics = Vec::new();
    for global in &module.globals {
        match lower_string_global(global) {
            Some(Ok(bytes)) => {
                strings.insert(global.variable.id, bytes);
            }
            Some(Err(error)) => module_barrier(&mut report, error)?,
            None => statics.push(&global.value),
        }
    }
    let static_names: HashSet<_> = statics
        .iter()
        .map(|global| rust_binding_name(&global.variable.name))
        .collect();
    let names: HashMap<_, _> = module
        .functions
        .iter()
        .map(|function| {
            (
                function.value.id,
                FunctionName {
                    rust: if function.name == "main" {
                        "__slate_main".into()
                    } else {
                        function.name.clone()
                    },
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
    let mut bindings = HashMap::new();
    for global in &module.globals {
        bindings.insert(
            global.value.variable.id,
            rust_binding_name(&global.value.variable.name),
        );
    }
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
    let mut cx = Context {
        names,
        bindings,
        strings,
        statics: statics.iter().map(|global| global.variable.id).collect(),
        over_aligned: HashMap::new(),
        target: &module.target,
        types: module
            .types
            .iter()
            .map(|definition| (definition.value.id, &definition.value))
            .collect(),
        record_names: record_names(module),
        records: RefCell::default(),
        temps: Cell::new(0),
        long_double: Cell::new(false),
        bridges: RefCell::default(),
    };
    cx.over_aligned = statics
        .iter()
        .filter(|global| global.definition)
        .filter_map(|global| Some((global.variable.id, over_alignment(&cx, &global.variable)?)))
        .collect();
    let mut items = Vec::new();
    let mut externs = Vec::new();
    for global in &statics {
        let name = binding_name(global.variable.id, &cx.bindings);
        match lower_static(global, &cx) {
            Ok((ty, Some(init))) => items.push(Item::Static {
                attrs: Vec::new(),
                vis: rust::Visibility::Private,
                mutable: true,
                name,
                ty,
                init,
            }),
            Ok((ty, None)) => externs.push(rust::ExternDecl::Static {
                attrs: Vec::new(),
                mutable: true,
                name,
                ty,
            }),
            Err(error) => module_barrier(&mut report, error)?,
        }
    }
    for function in &module.functions {
        let Some(body) = &function.body else {
            if passes_long_double(function, &cx) {
                continue;
            }
            let decl = lower_extern(function, &cx);
            if let Some(decl) = function_barrier(&mut report, &function.name, false, decl)? {
                externs.push(decl);
            }
            continue;
        };
        let item = lower_function(function, body, &cx);
        if let Some(item) = function_barrier(&mut report, &function.name, true, item)? {
            items.push(item);
        }
    }
    let wrappers = cx
        .over_aligned
        .values()
        .copied()
        .collect::<BTreeSet<_>>()
        .into_iter()
        .map(|alignment| {
            Ok(Item::Struct(rust::StructDef {
                attrs: vec![Attr::Repr(vec![
                    rust::Repr::C,
                    rust::Repr::Align(u32::try_from(alignment).map_err(|_| {
                        super::Error::Unsupported(format!("alignment {alignment}"))
                    })?),
                ])],
                vis: rust::Visibility::Private,
                field_vis: rust::Visibility::Private,
                generics: vec![rust::GenericParam {
                    name: "T".into(),
                    bounds: Vec::new(),
                }],
                name: align_wrapper(alignment),
                fields: rust::StructFields::Tuple(vec![rust::Type::Custom("T".into())]),
            }))
        })
        .collect::<Result<Vec<_>>>()?;
    let records = cx
        .records
        .into_inner()
        .into_values()
        .filter_map(|record| record.and_then(|record| record.ok()))
        .map(Item::Record);
    items.splice(0..0, records.chain(wrappers));
    if cx.long_double.get() {
        items.splice(
            0..0,
            long_double::long_double_prelude(rust::Visibility::Private),
        );
        externs.extend(
            long_double::f80_shim_decls()
                .into_iter()
                .filter(|decl| decl.name.starts_with("__slate_f80_"))
                .chain(cx.bridges.take().into_values())
                .map(rust::ExternDecl::Fn),
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
    if cx.names.values().any(|name| name.rust == "__slate_main") {
        items.push(Item::Fn(FnDef {
            attrs: Vec::new(),
            vis: rust::Visibility::Private,
            unsafe_: false,
            abi: None,
            name: "main".into(),
            params: Vec::new(),
            ret: None,
            body: vec![Stmt::Expr(Expr::Call {
                func: Box::new(Expr::Var("std::process::exit".into())),
                args: vec![Expr::Call {
                    func: Box::new(Expr::Var("__slate_main".into())),
                    args: Vec::new(),
                    binding: CallBinding::Generated,
                }],
                binding: CallBinding::Generated,
            })],
        }));
    }
    Ok(rust::Program { items })
}
