use crate::{
    c_side::HEADERS,
    decide::{Entry, Outcome},
    rust_side,
};
use anyhow::Result;
use object::{Object, ObjectSection, ObjectSymbol, RelocationTarget};
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
    pub kind: Kind,
    pub shape: String,
    pub link: String,
}

pub struct TargetSymbols {
    pub accepted: Vec<Candidate>,
    pub rejected: Vec<String>,
}

pub fn collect(target: &str, work: &Path, libc: &Path, entries: &[Entry]) -> Result<TargetSymbols> {
    let source = work.join(format!("symbols-{target}.c"));
    std::fs::write(&source, "")?;
    let mut candidates = BTreeMap::new();
    let mut rejected = BTreeSet::new();
    for header in HEADERS {
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
        let Ok((module, _)) = unit.parse_module() else {
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
                .map(|entry| entry.name.clone())
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
    let candidates: Vec<_> = candidates.into_values().collect();
    let lines = candidates.iter().enumerate().map(|(index, candidate)| {
        let name = rust_side::ident(&candidate.name);
        match candidate.kind {
            Kind::Function => format!("#[unsafe(no_mangle)] pub static S{index}: {} = libc::{name};", candidate.shape),
            Kind::Global => format!("#[unsafe(no_mangle)] pub static mut S{index}: {} = unsafe {{ &raw {} libc::{name} }};", candidate.shape,
                if candidate.shape.starts_with("*mut") { "mut" } else { "const" }),
            Kind::Type => {
                let ty = &candidate.shape;
                let signed = ty.starts_with('i');
                if ty.starts_with(['f']) {
                    format!("const _: fn(slate_support::{name}) = |_: {ty}| {{}}; #[unsafe(no_mangle)] pub static S{index}: u8 = 0;")
                } else {
                    format!("const _: () = assert!(core::mem::size_of::<slate_support::{name}>() == core::mem::size_of::<{ty}>() && core::mem::align_of::<slate_support::{name}>() == core::mem::align_of::<{ty}>() && (slate_support::{name}::MIN < slate_support::{name}::MAX / 2 * 0) == {signed}); #[unsafe(no_mangle)] pub static S{index}: u8 = 0;")
                }
            }
            Kind::Constant => format!("const _: () = assert!(libc::{name} as i128 == {}i128); #[unsafe(no_mangle)] pub static S{index}: u8 = 0;", candidate.shape),
        }
    }).collect::<Vec<_>>();
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
                let name = if object.format() == object::BinaryFormat::MachO
                    || (object.format() == object::BinaryFormat::Coff
                        && target.starts_with("i686-"))
                {
                    name.strip_prefix('_').unwrap_or(name)
                } else {
                    name
                };
                links.insert(index, name.to_string());
            }
        }
    }
    let mut accepted = Vec::new();
    for (index, candidate) in candidates.into_iter().enumerate() {
        if !passed.contains(&index) {
            rejected.insert(format!(
                "{:?} `{}`: absent from libc or incompatible Rust type/value",
                candidate.kind, candidate.name
            ));
        } else if matches!(candidate.kind, Kind::Function | Kind::Global)
            && links.get(&index) != Some(&candidate.link)
        {
            rejected.insert(format!(
                "{:?} `{}`: C links to `{}` vs libc {:?}",
                candidate.kind,
                candidate.name,
                candidate.link,
                links.get(&index)
            ));
        } else {
            accepted.push(candidate);
        }
    }
    Ok(TargetSymbols {
        accepted,
        rejected: rejected.into_iter().collect(),
    })
}

pub fn integer(value: &ir::Value) -> Option<String> {
    match &value.node.value {
        ir::ValueKind::Constant(ir::Number::Integer(n)) => Some(n.to_string()),
        ir::ValueKind::Constant(ir::Number::SignedInteger(n)) => Some(n.to_string()),
        _ => None,
    }
}

pub fn table(targets: &BTreeMap<String, TargetSymbols>) -> String {
    let mut rows: BTreeMap<(Kind, &str), BTreeSet<&str>> = BTreeMap::new();
    for (target, symbols) in targets {
        for candidate in &symbols.accepted {
            rows.entry((candidate.kind, &candidate.name))
                .or_default()
                .insert(target);
        }
    }
    let mut out = String::from("pub const SYMBOLS: &[Symbol] = &[\n");
    for ((kind, name), targets) in rows {
        let targets = targets.into_iter().collect::<Vec<_>>();
        let _ = writeln!(
            out,
            "    Symbol {{ name: {name:?}, targets: &{targets:?}, kind: Kind::{kind:?} }},"
        );
    }
    out.push_str("];\n");
    out
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
