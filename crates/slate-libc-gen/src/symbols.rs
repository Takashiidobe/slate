use crate::{
    decide::{Entry, Outcome},
    emit, rust_side,
};
use anyhow::Result;
use object::{Object, ObjectSection, ObjectSymbol, RelocationTarget};
use proc_macro2::{Literal, TokenStream};
use quote::{format_ident, quote};
use slate::libc_catalog::{self, Kind};
use slate_parser::ir::{self, TypeDefinitionKind};
use std::{
    collections::{BTreeMap, BTreeSet},
    fmt::Write,
    path::Path,
};

#[derive(Clone)]
pub struct Candidate {
    pub name: String,
    pub rust_path: String,
    pub kind: Kind,
    pub shape: String,
    pub link: String,
}

pub struct TargetSymbols {
    pub accepted: Vec<Candidate>,
    pub rejected: Vec<String>,
}

pub fn collect(
    target: &str,
    work: &Path,
    libc: &Path,
    entries: &[Entry],
    providers: &crate::providers::Inventory,
) -> Result<TargetSymbols> {
    let source = work.join(format!("symbols-{target}.c"));
    std::fs::write(&source, "")?;
    let mut candidates = BTreeMap::new();
    let mut rejected = BTreeSet::new();
    for header in crate::c_side::HEADERS
        .iter()
        .copied()
        .chain(providers.headers.iter().map(String::as_str))
    {
        let mut args = crate::c_side::target_args(target);
        args.extend(["-include".into(), header.to_string()]);
        let (unit, trace) =
            match slate::frontend::preprocess_traced(&source, slate::target::parse_args(&args)?) {
                Ok(result) => result,
                Err(_) => continue,
            };
        let mut definitions = BTreeMap::new();
        for event in &trace.events {
            if event.skipped || event.command_line {
                continue;
            }
            if event.kind == slate_parser::pp::MacroEventKind::Undef {
                definitions.remove(&event.name);
                continue;
            }
            if event.name.starts_with('_')
                || !event
                    .name
                    .chars()
                    .all(|c| c.is_ascii_uppercase() || c.is_ascii_digit() || c == '_')
            {
                continue;
            }
            let Some(path) = unit.files().get_path(event.loc.file) else {
                continue;
            };
            let Ok(text) = std::fs::read_to_string(path) else {
                continue;
            };
            let offset = event.loc.offset.min(text.len());
            let start = text[..offset].rfind('\n').map_or(0, |n| n + 1);
            let line = text[start..].lines().next().unwrap_or("");
            let Some(at) = line.find(&event.name) else {
                continue;
            };
            let replacement = &line[at + event.name.len()..];
            if replacement.starts_with('(') {
                continue;
            }
            let replacement = replacement
                .split("/*")
                .next()
                .unwrap_or("")
                .split("//")
                .next()
                .unwrap_or("")
                .trim();
            if replacement.chars().any(|c| c.is_ascii_digit())
                && replacement.chars().all(|c| {
                    c.is_ascii_digit()
                        || c.is_ascii_whitespace()
                        || "abcdefABCDEFxXuUlL()+-*/%~!<>&|^".contains(c)
                })
            {
                definitions.insert(event.name.clone(), replacement.to_string());
            }
        }
        let macros: Vec<String> = definitions.into_keys().collect();
        let module = unit.parse_module_all_declarations().ok().or_else(|| {
            let (unit, _) =
                slate::frontend::preprocess_traced(&source, slate::target::parse_args(&args).ok()?)
                    .ok()?;
            unit.parse_module().ok()
        });
        let Some((module, _)) = module else {
            continue;
        };
        let record = |name: &str| {
            let definition = module
                .types
                .iter()
                .find(|definition| definition.name.as_deref() == Some(name))?;
            let key = libc_catalog::record_key(definition)?;
            entries
                .iter()
                .find(|entry| {
                    !matches!(entry.outcome, Outcome::Excluded)
                        && entry.targets.contains(target)
                        && entry.layout_key == key
                        && (entry.name == name || entry.aliases.contains(name))
                })
                .map(|entry| entry.rust_path.clone())
        };
        let shape = |ty: &ir::Type, c| {
            libc_catalog::shape(
                ty,
                &module.types,
                &module.c_types,
                c,
                &record,
                module.target.pointer_width,
            )
        };
        for function in &module.functions {
            if function.body.is_some()
                || function.provenance.system_header.is_none()
                || !matches!(function.linkage, ir::Linkage::External)
            {
                continue;
            }
            let Some(ty) = libc_catalog::function_type(function) else {
                rejected.insert(format!(
                    "function `{}`: unsupported ABI or attributes",
                    function.name
                ));
                continue;
            };
            let Some(mut shape) = shape(
                &ty,
                module.c_types.bindings.get(&function.value.id).copied(),
            ) else {
                rejected.insert(format!(
                    "function `{}`: unverified signature type",
                    function.name
                ));
                continue;
            };
            if function.semantics.noreturn
                && let Some((prefix, _)) = shape.rsplit_once(" -> ")
            {
                shape = format!("{prefix} -> !");
            }
            candidates.insert(
                (Kind::Function, function.name.clone()),
                Candidate {
                    rust_path: String::new(),
                    name: function.name.clone(),
                    kind: Kind::Function,
                    shape,
                    link: libc_catalog::link_name(&function.name, &function.symbol).into(),
                },
            );
        }
        for global in &module.globals {
            if global.definition
                || global.provenance.system_header.is_none()
                || !matches!(global.linkage, ir::Linkage::External)
                || matches!(global.variable.storage, ir::StorageDuration::Thread)
                || global.symbol.weak
                || global.symbol.alias.is_some()
                || !global.variable.access.is_plain()
            {
                continue;
            }
            let Some(shape) = shape(
                &global.variable.ty,
                module.c_types.bindings.get(&global.variable.id).copied(),
            ) else {
                continue;
            };
            candidates.insert(
                (Kind::Global, global.variable.name.clone()),
                Candidate {
                    rust_path: String::new(),
                    name: global.variable.name.clone(),
                    kind: Kind::Global,
                    shape: format!(
                        "*{} {shape}",
                        if global.variable.is_const {
                            "const"
                        } else {
                            "mut"
                        }
                    ),
                    link: libc_catalog::link_name(&global.variable.name, &global.symbol).into(),
                },
            );
        }
        for definition in &module.types {
            if definition.provenance.system_header.is_none() {
                continue;
            }
            if let TypeDefinitionKind::Alias(ty) = &definition.kind
                && let Some(name) = &definition.name
                && let Some(shape) = shape(ty, None)
                && matches!(
                    shape.as_str(),
                    "i8" | "u8"
                        | "i16"
                        | "u16"
                        | "i32"
                        | "u32"
                        | "i64"
                        | "u64"
                        | "i128"
                        | "u128"
                        | "f32"
                        | "f64"
                )
            {
                candidates.insert(
                    (Kind::Type, name.clone()),
                    Candidate {
                        rust_path: String::new(),
                        name: name.clone(),
                        kind: Kind::Type,
                        shape,
                        link: String::new(),
                    },
                );
            }
            if let TypeDefinitionKind::Enum {
                enumerators: Some(enumerators),
                ..
            } = &definition.kind
            {
                for enumerator in enumerators {
                    if let Some(value) = integer(&enumerator.value.value) {
                        candidates.insert(
                            (Kind::Constant, enumerator.name.clone()),
                            Candidate {
                                rust_path: String::new(),
                                name: enumerator.name.clone(),
                                kind: Kind::Constant,
                                shape: value,
                                link: String::new(),
                            },
                        );
                    }
                }
            }
        }
        if !macros.is_empty() {
            let text = macros
                .iter()
                .enumerate()
                .map(|(i, definition)| format!("enum {{ __slate_constant_{i} = {definition} }};\n"))
                .collect::<String>();
            if let Ok((constants, _, _)) =
                slate::frontend::parse_module_with_source(&source, Some(text), &args)
            {
                for definition in &constants.types {
                    if let TypeDefinitionKind::Enum {
                        enumerators: Some(enumerators),
                        ..
                    } = &definition.kind
                    {
                        for enumerator in enumerators {
                            if let Some(index) = enumerator
                                .name
                                .strip_prefix("__slate_constant_")
                                .and_then(|s| s.parse::<usize>().ok())
                                && let Some(value) = integer(&enumerator.value.value)
                            {
                                let name = macros[index].clone();
                                candidates.insert(
                                    (Kind::Constant, name.clone()),
                                    Candidate {
                                        rust_path: String::new(),
                                        name,
                                        kind: Kind::Constant,
                                        shape: value,
                                        link: String::new(),
                                    },
                                );
                            }
                        }
                    }
                }
            }
        }
    }
    let candidates: Vec<_> = candidates
        .into_values()
        .flat_map(|mut candidate| {
            let Some(path) = providers.path(&candidate.name) else {
                rejected.insert(format!(
                    "{:?} `{}`: ambiguous provider paths",
                    candidate.kind, candidate.name
                ));
                return Vec::new();
            };
            if candidate.kind == Kind::Type {
                let support = format!("slate_support::{}", rust_side::ident(&candidate.name));
                candidate.rust_path = support.clone();
                if path.starts_with("slate_support::mach::") {
                    let mut alternative = candidate.clone();
                    alternative.rust_path = path;
                    return vec![candidate, alternative];
                }
                return vec![candidate];
            }
            candidate.rust_path = path;
            vec![candidate]
        })
        .collect();
    let (candidates, probes): (Vec<_>, Vec<_>) = candidates
        .into_iter()
        .filter_map(|candidate| match Probe::parse(&candidate) {
            Some(probe) => Some((candidate, probe)),
            None => {
                rejected.insert(format!(
                    "{:?} `{}`: Rust path or shape does not parse",
                    candidate.kind, candidate.name
                ));
                None
            }
        })
        .unzip();
    let lines = probes
        .iter()
        .enumerate()
        .map(|(index, probe)| probe.item(index).to_string())
        .collect::<Vec<_>>();
    let (object, passed) = rust_side::compile_lines(libc, work, target, &lines)?;
    let bytes = std::fs::read(object)?;
    let object = object::File::parse(&*bytes)?;
    let mut links = BTreeMap::new();
    for symbol in object.symbols() {
        let Ok(name) = symbol.name() else {
            continue;
        };
        let name = if object.format() == object::BinaryFormat::MachO
            || (object.format() == object::BinaryFormat::Coff && target.starts_with("i686-"))
        {
            name.strip_prefix('_').unwrap_or(name)
        } else {
            name
        };
        let Some(index) = name.strip_prefix('S').and_then(|s| s.parse::<usize>().ok()) else {
            continue;
        };
        let Some(section) = symbol
            .section_index()
            .and_then(|index| object.section_by_index(index).ok())
        else {
            continue;
        };
        let offset = symbol.address() - section.address();
        for (at, relocation) in section.relocations() {
            if at == offset
                && let RelocationTarget::Symbol(referenced) = relocation.target()
                && let Ok(symbol) = object.symbol_by_index(referenced)
                && let Ok(name) = symbol.name()
            {
                let name = if object.format() == object::BinaryFormat::MachO {
                    name.strip_prefix('_').unwrap_or(name)
                } else {
                    name
                };
                let name = if target.ends_with("-windows-msvc") {
                    let name = name.strip_prefix("__imp_").unwrap_or(name);
                    let name = if target.starts_with("i686-") {
                        name.strip_prefix('_').unwrap_or(name)
                    } else {
                        name
                    };
                    if target.starts_with("i686-") {
                        name.rsplit_once('@')
                            .filter(|(_, suffix)| suffix.parse::<u32>().is_ok())
                            .map_or(name, |(name, _)| name)
                    } else {
                        name
                    }
                } else {
                    name
                };
                links.insert(index, name.to_string());
            }
        }
    }
    let mut accepted = Vec::new();
    let mut chosen = BTreeSet::new();
    for (index, candidate) in candidates.into_iter().enumerate() {
        if !passed.contains(&index) {
            rejected.insert(format!(
                "{:?} `{}`: absent from provider or incompatible Rust type/value",
                candidate.kind, candidate.name
            ));
        } else if matches!(candidate.kind, Kind::Function | Kind::Global)
            && links.get(&index) != Some(&candidate.link)
        {
            rejected.insert(format!(
                "{:?} `{}`: C links to `{}` vs provider {:?}",
                candidate.kind,
                candidate.name,
                candidate.link,
                links.get(&index)
            ));
        } else if chosen.insert((candidate.kind, candidate.name.clone())) {
            accepted.push(candidate);
        }
    }
    Ok(TargetSymbols {
        accepted,
        rejected: rejected.into_iter().collect(),
    })
}

enum Probe {
    Function {
        path: syn::Path,
        ty: syn::Type,
    },
    Global {
        path: syn::Path,
        ty: syn::TypePtr,
    },
    Float {
        path: syn::Path,
        ty: syn::Type,
    },
    Integer {
        path: syn::Path,
        ty: syn::Type,
        signed: bool,
    },
    Constant {
        path: syn::Path,
        value: i128,
    },
}

impl Probe {
    fn parse(candidate: &Candidate) -> Option<Self> {
        let path = emit::parse(&candidate.rust_path)?;
        let shape = &candidate.shape;
        Some(match candidate.kind {
            Kind::Function => Self::Function {
                path,
                ty: emit::parse(shape)?,
            },
            Kind::Global => Self::Global {
                path,
                ty: emit::parse(shape)?,
            },
            Kind::Type if shape.starts_with('f') => Self::Float {
                path,
                ty: emit::parse(shape)?,
            },
            Kind::Type => Self::Integer {
                path,
                ty: emit::parse(shape)?,
                signed: shape.starts_with('i'),
            },
            Kind::Constant => Self::Constant {
                path,
                value: shape.parse().ok()?,
            },
        })
    }

    fn item(&self, index: usize) -> TokenStream {
        let symbol = format_ident!("S{index}");
        let marker = quote!(#[unsafe(no_mangle)] pub static #symbol: u8 = 0;);
        match self {
            Self::Function { path, ty } => {
                quote!(#[unsafe(no_mangle)] pub static #symbol: #ty = #path;)
            }
            Self::Global { path, ty } => {
                let access = &ty.mutability;
                quote!(#[unsafe(no_mangle)] pub static mut #symbol: #ty = unsafe { &raw #access #path };)
            }
            Self::Float { path, ty } => quote!(const _: fn(#path) = |_: #ty| {}; #marker),
            Self::Integer { path, ty, signed } => quote! {
                const _: () = assert!(
                    core::mem::size_of::<#path>() == core::mem::size_of::<#ty>()
                        && core::mem::align_of::<#path>() == core::mem::align_of::<#ty>()
                        && (#path::MIN < #path::MAX / 2 * 0) == #signed
                );
                #marker
            },
            Self::Constant { path, value } => {
                let value = Literal::i128_suffixed(*value);
                quote!(const _: () = assert!(#path as i128 == #value); #marker)
            }
        }
    }
}

pub fn integer(value: &ir::Value) -> Option<String> {
    match &value.node.value {
        ir::ValueKind::Constant(ir::Number::Integer(n)) => Some(n.to_string()),
        ir::ValueKind::Constant(ir::Number::SignedInteger(n)) => Some(n.to_string()),
        _ => None,
    }
}

pub fn table(targets: &BTreeMap<String, TargetSymbols>) -> Result<String> {
    let mut rows: BTreeMap<(Kind, &str, String, Option<&str>), BTreeSet<&str>> = BTreeMap::new();
    for (target, symbols) in targets {
        for candidate in &symbols.accepted {
            rows.entry((
                candidate.kind,
                &candidate.name,
                candidate.rust_path.replace("libc::", "slate_support::"),
                (candidate.kind == Kind::Function).then_some(candidate.shape.as_str()),
            ))
            .or_default()
            .insert(target);
        }
    }
    let symbols = rows
        .into_iter()
        .map(|((kind, name, rust_path, shape), targets)| {
            let targets = targets.iter();
            let kind = format_ident!("{kind:?}");
            let shape = match shape {
                Some(shape) => quote!(Some(#shape)),
                None => quote!(None),
            };
            quote! {
                Symbol {
                    name: #name,
                    rust_path: #rust_path,
                    targets: &[#(#targets),*],
                    kind: Kind::#kind,
                    shape: #shape,
                }
            }
        });
    emit::file(quote! {
        pub const SYMBOLS: &[Symbol] = &[#(#symbols),*];
    })
}

pub fn report(targets: &BTreeMap<String, TargetSymbols>) -> String {
    let mut out = String::from("\n## Symbols\n\n");
    for (target, symbols) in targets {
        let _ = writeln!(out, "### {target}\n");
        for kind in [Kind::Type, Kind::Function, Kind::Global, Kind::Constant] {
            let _ = writeln!(
                out,
                "- {kind:?}: {} accepted",
                symbols.accepted.iter().filter(|s| s.kind == kind).count()
            );
        }
        out.push_str("\nAccepted:\n\n");
        for symbol in &symbols.accepted {
            let _ = writeln!(
                out,
                "- {:?} `{}`: `{}`",
                symbol.kind, symbol.name, symbol.shape
            );
        }
        out.push_str("\nExcluded:\n\n");
        for reason in &symbols.rejected {
            let _ = writeln!(out, "- {reason}");
        }
        out.push('\n');
    }
    out
}
