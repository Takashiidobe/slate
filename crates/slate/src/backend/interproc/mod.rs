pub(in crate::backend) mod timing;

use std::collections::{BTreeMap, BTreeSet};
use std::fmt;

use crate::backend::engine::macro_templates;
use crate::backend::link;
use crate::backend::rust_ast::{
    Abi, Attr, ExternDecl, ExternFnDecl, FnDef, Item, Path, Program, Type,
};
use crate::function_identity::Known;

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(in crate::backend) struct FnKey {
    pub(in crate::backend) unit: usize,
    pub(in crate::backend) name: String,
}

impl FnKey {
    pub(in crate::backend) fn new(unit: usize, name: impl Into<String>) -> Self {
        Self {
            unit,
            name: name.into(),
        }
    }
}

impl fmt::Display for FnKey {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.write_str(&self.name)
    }
}

pub enum Exports {
    Fixed,
    Liftable {
        address_taken: BTreeSet<String>,
        modules: Vec<Path>,
    },
}

pub(in crate::backend) fn run(programs: &mut [Program], exports: &Exports) -> Option<Program> {
    let Exports::Liftable {
        address_taken,
        modules,
    } = exports
    else {
        return None;
    };
    let mut timer = timing::PhaseTimer::new("interproc");
    let named_in_templates = functions_pinned_by_templates(programs);
    let types = link::SharedTypes::plan(programs);
    let (liftable, c_abi) = export_candidates(programs, address_taken, &named_in_templates);
    let rust_bound = link::rust_bound(programs, &liftable, &types);
    let c_bound = link::rust_bound(programs, &c_abi, &types);
    timer.lap("pinned");
    link::bind_calls(programs, &rust_bound, &c_bound, modules);
    for program in programs.iter() {
        for_each_extern_fn(&program.items, &mut |decl| {
            let sliced = decl.params.iter().any(|param| {
                matches!(&param.ty, Type::Ref { inner, .. } if matches!(&**inner, Type::Slice(_)) || **inner == Type::Custom("core::ffi::CStr".into()))
            });
            assert!(
                !sliced,
                "IR pointer lift of `{}` gave it a slice or CStr parameter, but link did not bind its cross-unit calls",
                decl.name
            );
        });
    }
    let hoisted = types.hoist(programs);
    timer.lap("link");
    hoisted
}

fn functions_pinned_by_templates(programs: &[Program]) -> BTreeSet<FnKey> {
    let symbols = Symbols::new(programs);
    let mut named = BTreeSet::new();
    let mut returning = BTreeSet::new();
    for (unit, program) in programs.iter().enumerate() {
        for item in &program.items {
            let Item::MacroRules {
                name,
                body,
                returns,
                ..
            } = item
            else {
                continue;
            };
            if *returns {
                returning.insert(name.clone());
            }
            let mut vars = Vec::new();
            for stmt in body {
                stmt.collect_vars(&mut vars);
            }
            named.extend(
                vars.iter()
                    .filter_map(|name| symbols.resolve(unit, name.as_str())),
            );
        }
    }
    named.extend(
        fn_defs(programs)
            .into_iter()
            .filter(|(_, def)| macro_templates::calls_template(&def.body, &returning))
            .map(|(key, _)| key),
    );
    named
}

pub(in crate::backend) struct Symbols {
    defined: BTreeSet<FnKey>,
    imports: Vec<BTreeMap<String, FnKey>>,
}

impl Symbols {
    pub(in crate::backend) fn new(programs: &[Program]) -> Self {
        let defs = fn_defs(programs);
        let exported = exported_symbols(&defs);
        let imports = programs
            .iter()
            .enumerate()
            .map(|(unit, program)| {
                let mut imports = BTreeMap::new();
                for_each_extern_fn(&program.items, &mut |decl| {
                    if link_name(decl).is_none()
                        && let Some([key]) = exported.get(decl.name.as_str()).map(Vec::as_slice)
                        && key.unit != unit
                    {
                        imports.insert(decl.name.clone(), key.clone());
                    }
                });
                imports
            })
            .collect();
        Self {
            defined: defs.into_keys().collect(),
            imports,
        }
    }

    pub(in crate::backend) fn resolve(&self, unit: usize, name: &str) -> Option<FnKey> {
        let key = FnKey::new(unit, name);
        if self.defined.contains(&key) {
            return Some(key);
        }
        self.imports.get(unit)?.get(name).cloned()
    }
}

pub(in crate::backend) fn fn_defs(programs: &[Program]) -> BTreeMap<FnKey, &FnDef> {
    let mut out = BTreeMap::new();
    for (unit, program) in programs.iter().enumerate() {
        collect_fn_defs_into(unit, &program.items, &mut out);
    }
    out
}

fn collect_fn_defs_into<'a>(unit: usize, items: &'a [Item], out: &mut BTreeMap<FnKey, &'a FnDef>) {
    for item in items {
        match item {
            Item::Fn(f) => {
                out.insert(FnKey::new(unit, f.name.clone()), f);
            }
            Item::InlineMod { items, .. } => collect_fn_defs_into(unit, items, out),
            _ => {}
        }
    }
}

fn exported_symbols<'a>(defs: &BTreeMap<FnKey, &'a FnDef>) -> BTreeMap<&'a str, Vec<FnKey>> {
    let mut exported: BTreeMap<&str, Vec<FnKey>> = BTreeMap::new();
    for (key, f) in defs {
        if f.attrs.contains(&Attr::NoMangle) {
            exported
                .entry(f.name.as_str())
                .or_default()
                .push(key.clone());
        }
    }
    exported
}

fn link_name(decl: &ExternFnDecl) -> Option<&str> {
    decl.attrs.iter().find_map(|attr| match attr {
        Attr::LinkName(name) => Some(name.as_str()),
        _ => None,
    })
}

pub(in crate::backend) fn for_each_extern_fn<'a>(
    items: &'a [Item],
    f: &mut impl FnMut(&'a ExternFnDecl),
) {
    for item in items {
        match item {
            Item::ExternBlock { decls, .. } => {
                for decl in decls {
                    if let ExternDecl::Fn(decl) = decl {
                        f(decl);
                    }
                }
            }
            Item::InlineMod { items, .. } => for_each_extern_fn(items, f),
            _ => {}
        }
    }
}

fn export_candidates(
    programs: &[Program],
    address_taken: &BTreeSet<String>,
    named_in_templates: &BTreeSet<FnKey>,
) -> (BTreeSet<FnKey>, BTreeSet<FnKey>) {
    let defs = fn_defs(programs);
    let exported = exported_symbols(&defs);
    let mut pinned = BTreeSet::new();
    for program in programs {
        for item in &program.items {
            if let Item::GlobalAsm(asm) = item {
                pinned.extend(
                    exported
                        .keys()
                        .filter(|symbol| mentions_symbol(&asm.template.to_string(), symbol)),
                );
            }
        }
        for_each_extern_fn(&program.items, &mut |decl| {
            if let Some(target) = link_name(decl) {
                pinned.insert(target);
                pinned.insert(decl.name.as_str());
                return;
            }
            if let Some([key]) = exported.get(decl.name.as_str()).map(Vec::as_slice)
                && !decl_matches(decl, defs[key])
            {
                pinned.insert(decl.name.as_str());
            }
        });
    }
    let mut liftable = BTreeSet::new();
    let mut c_abi = BTreeSet::new();
    for (symbol, keys) in exported {
        let [key] = keys.as_slice() else {
            continue;
        };
        if pinned.contains(symbol) || !is_path_bindable(defs[key]) {
            continue;
        }
        if address_taken.contains(symbol)
            || named_in_templates.contains(key)
            || !is_liftable_export(defs[key])
        {
            c_abi.insert(key.clone());
        } else {
            liftable.insert(key.clone());
        }
    }
    (liftable, c_abi)
}

fn mentions_symbol(template: &str, symbol: &str) -> bool {
    template
        .split(|c: char| !(c.is_ascii_alphanumeric() || c == '_' || c == '$' || c == '.'))
        .any(|word| word == symbol)
}

fn is_path_bindable(f: &FnDef) -> bool {
    f.name != "main"
        && f.abi == Some(Abi::CUnwind)
        && Known::from_symbol(&f.name).is_none()
        && f.attrs.iter().all(|attr| {
            matches!(
                attr,
                Attr::NoMangle
                    | Attr::Inline(_)
                    | Attr::Cold
                    | Attr::MustUse
                    | Attr::Deprecated(_)
                    | Attr::Allow(_)
                    | Attr::LinkSection(_)
            )
        })
}

pub(in crate::backend) fn has_unsafe_signature(f: &FnDef) -> bool {
    f.unsafe_
        && (!f
            .attrs
            .iter()
            .any(|attr| matches!(attr, Attr::TargetFeature(_)))
            || f.params
                .iter()
                .any(|param| matches!(param.ty, Type::VaList | Type::Variadic)))
}

fn is_liftable_export(f: &FnDef) -> bool {
    !has_unsafe_signature(f)
        && !f
            .attrs
            .iter()
            .any(|attr| matches!(attr, Attr::LinkSection(_)))
}

fn decl_matches(decl: &ExternFnDecl, def: &FnDef) -> bool {
    !decl.variadic
        && decl.params.len() == def.params.len()
        && decl.params.iter().zip(&def.params).all(|(decl, def)| {
            !matches!(def.ty, Type::Ptr { .. }) || matches!(decl.ty, Type::Ptr { .. })
        })
}
