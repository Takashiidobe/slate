use std::collections::{BTreeMap, BTreeSet};

use super::SharedTypes;
use crate::backend::interproc::{self, FnKey};
use crate::backend::rust_ast::{
    Attr, ExternDecl, ExternFnDecl, FnDef, Ident, Item, Path, Program, Type, Visibility,
};

pub(in crate::backend) fn rust_bound(
    programs: &[Program],
    liftable: &BTreeSet<FnKey>,
    types: &SharedTypes,
) -> BTreeSet<FnKey> {
    let defs = interproc::fn_defs(programs);
    let by_name: BTreeMap<&str, &FnKey> = liftable
        .iter()
        .filter(|key| {
            programs[key.unit]
                .items
                .iter()
                .any(|item| matches!(item, Item::Fn(f) if f.name == key.name))
        })
        .map(|key| (key.name.as_str(), key))
        .collect();
    let mut rejected = BTreeSet::new();
    for (unit, program) in programs.iter().enumerate() {
        visit_extern_fns(&program.items, true, &mut |decl, top_level| {
            let Some(&key) = by_name.get(decl.name.as_str()) else {
                return;
            };
            if key.unit == unit
                || !top_level
                || !decl_identical(types, key.unit, defs[key], unit, decl)
            {
                rejected.insert(key.clone());
            }
        });
    }
    by_name
        .into_values()
        .filter(|key| !rejected.contains(*key))
        .cloned()
        .collect()
}

pub(in crate::backend) fn bind_calls(
    programs: &mut [Program],
    rust_bound: &BTreeSet<FnKey>,
    c_bound: &BTreeSet<FnKey>,
    modules: &[Path],
) {
    if rust_bound.is_empty() && c_bound.is_empty() {
        return;
    }
    let by_name: BTreeMap<&str, &FnKey> = rust_bound
        .iter()
        .chain(c_bound)
        .map(|key| (key.name.as_str(), key))
        .collect();
    let mut imported = BTreeSet::new();
    for (unit, program) in programs.iter_mut().enumerate() {
        let mut uses = BTreeSet::new();
        program.items.retain_mut(|item| {
            let Item::ExternBlock { decls, .. } = item else {
                return true;
            };
            decls.retain(|decl| match decl {
                ExternDecl::Fn(decl) => match by_name.get(decl.name.as_str()) {
                    Some(&key) if key.unit != unit => {
                        uses.insert(key.clone());
                        false
                    }
                    _ => true,
                },
                ExternDecl::Static { .. } => true,
            });
            !decls.is_empty()
        });
        let imports = uses.iter().map(|key| {
            let mut path = modules[key.unit].clone();
            path.segments.push(Ident::new(&key.name));
            Item::Use {
                vis: Visibility::Private,
                path,
                alias: None,
            }
        });
        super::insert_imports(&mut program.items, imports.collect::<Vec<_>>());
        imported.extend(uses);
    }
    for (unit, program) in programs.iter_mut().enumerate() {
        for item in &mut program.items {
            let Item::Fn(f) = item else {
                continue;
            };
            let key = FnKey::new(unit, f.name.clone());
            if rust_bound.contains(&key) {
                f.attrs.retain(|attr| *attr != Attr::NoMangle);
                f.abi = None;
            }
            if imported.contains(&key) {
                f.vis = Visibility::Pub;
            }
        }
    }
}

fn visit_extern_fns<'a>(
    items: &'a [Item],
    top_level: bool,
    f: &mut impl FnMut(&'a ExternFnDecl, bool),
) {
    for item in items {
        match item {
            Item::ExternBlock { decls, .. } => {
                for decl in decls {
                    if let ExternDecl::Fn(decl) = decl {
                        f(decl, top_level);
                    }
                }
            }
            Item::InlineMod { items, .. } => visit_extern_fns(items, false, f),
            Item::Cfg { item, .. } => visit_extern_fns(std::slice::from_ref(item), false, f),
            _ => {}
        }
    }
}

fn decl_identical(
    types: &SharedTypes,
    def_unit: usize,
    def: &FnDef,
    decl_unit: usize,
    decl: &ExternFnDecl,
) -> bool {
    let unit_ret = |ret: &Option<Type>| match ret {
        Some(Type::Unit) | None => None,
        Some(ty) => Some(ty.clone()),
    };
    decl.attrs.is_empty()
        && !decl.variadic
        && decl.params.len() == def.params.len()
        && def
            .params
            .iter()
            .zip(&decl.params)
            .all(|(def, decl)| types.same_type(def_unit, &def.ty, decl_unit, &decl.ty))
        && match (unit_ret(&def.ret), unit_ret(&decl.ret)) {
            (None, None) => true,
            (Some(def), Some(decl)) => types.same_type(def_unit, &def, decl_unit, &decl),
            _ => false,
        }
}
