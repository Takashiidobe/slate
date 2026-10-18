use std::collections::{BTreeMap, BTreeSet, HashMap};

use crate::backend::codegen;
use crate::backend::rust_ast::{
    Ident, Item, Path, Program, RecordDef, StructDef, StructFields, TraitRef, Type, Visibility,
};

const BITS_MODULE: &str = "__slate_bits";
const PLACEHOLDER: &str = "\0";
pub(in crate::backend) const SHARED_MODULE: &str = "__slate_types";

#[derive(Clone, PartialEq, Eq, Hash)]
enum Shape {
    Record(RecordDef),
    Struct(StructDef),
}

struct Node {
    unit: usize,
    name: String,
    shape: Option<(Shape, Vec<StructDef>)>,
    refs: Vec<String>,
    opaque: bool,
}

pub(in crate::backend) struct SharedTypes {
    class_of: Vec<HashMap<String, usize>>,
    shared_names: BTreeMap<usize, String>,
    representatives: Vec<(usize, String)>,
}

struct Scope<'a> {
    local: &'a BTreeSet<&'a str>,
    generics: BTreeSet<&'a str>,
    bits: &'a BTreeMap<&'a str, &'a StructDef>,
    implemented: &'a BTreeSet<String>,
}

impl SharedTypes {
    pub(in crate::backend) fn plan(programs: &[Program]) -> Self {
        let nodes: Vec<Node> = programs
            .iter()
            .enumerate()
            .flat_map(|(unit, program)| unit_nodes(unit, &program.items))
            .collect();
        let index: HashMap<(usize, &str), usize> = nodes
            .iter()
            .enumerate()
            .map(|(id, node)| ((node.unit, node.name.as_str()), id))
            .collect();
        let edges: Vec<Vec<usize>> = nodes
            .iter()
            .map(|node| {
                node.refs
                    .iter()
                    .map(|name| index[&(node.unit, name.as_str())])
                    .collect()
            })
            .collect();
        let mut redirect: Vec<usize> = (0..nodes.len()).collect();
        let first = refine(&nodes, &edges, &redirect);
        for (id, node) in nodes.iter().enumerate() {
            if !node.opaque || node.shape.is_none() {
                continue;
            }
            let complete: Vec<usize> = nodes
                .iter()
                .enumerate()
                .filter(|(_, other)| {
                    !other.opaque && other.shape.is_some() && other.name == node.name
                })
                .map(|(other, _)| other)
                .collect();
            let classes: BTreeSet<usize> = complete.iter().map(|&other| first[other]).collect();
            if classes.len() == 1 {
                redirect[id] = complete[0];
            }
        }
        let mut classes = refine(&nodes, &edges, &redirect);
        for id in 0..nodes.len() {
            classes[id] = classes[redirect[id]];
        }
        let mut members: BTreeMap<usize, Vec<usize>> = BTreeMap::new();
        for (id, &class) in classes.iter().enumerate() {
            members.entry(class).or_default().push(id);
        }
        let representative_of: BTreeMap<usize, usize> = members
            .iter()
            .map(|(&class, ids)| {
                let representative = ids
                    .iter()
                    .copied()
                    .find(|&id| !nodes[id].opaque)
                    .unwrap_or(ids[0]);
                (class, representative)
            })
            .collect();
        let mut closed = BTreeSet::new();
        for (&class, ids) in &members {
            let units: BTreeSet<usize> = ids.iter().map(|&id| nodes[id].unit).collect();
            if units.len() < 2 || closed.contains(&class) {
                continue;
            }
            let mut closure = BTreeSet::from([class]);
            let mut pending = vec![class];
            let mut hoistable = true;
            while let Some(class) = pending.pop() {
                let representative = representative_of[&class];
                if nodes[representative].shape.is_none() {
                    hoistable = false;
                    break;
                }
                for &edge in &edges[representative] {
                    if closure.insert(classes[edge]) {
                        pending.push(classes[edge]);
                    }
                }
            }
            if hoistable {
                closed.extend(closure);
            }
        }
        let mut hoisted: Vec<(usize, usize)> = closed
            .into_iter()
            .map(|class| (representative_of[&class], class))
            .collect();
        hoisted.sort();
        let mut taken = BTreeSet::new();
        let mut anonymous = 0usize;
        let mut shared_names = BTreeMap::new();
        let mut representatives = Vec::new();
        for &(representative, class) in &hoisted {
            let node = &nodes[representative];
            let base = if is_anonymous(&node.name) {
                anonymous += 1;
                format!("__SlateRecord{}", anonymous - 1)
            } else {
                node.name.clone()
            };
            let mut name = base.clone();
            let mut suffix = 0;
            while !taken.insert(name.clone()) {
                suffix += 1;
                name = format!("{base}_{suffix}");
            }
            shared_names.insert(class, name);
            representatives.push((node.unit, node.name.clone()));
        }
        let mut class_of = vec![HashMap::new(); programs.len()];
        for (node, &class) in nodes.iter().zip(&classes) {
            class_of[node.unit].insert(node.name.clone(), class);
        }
        Self {
            class_of,
            shared_names,
            representatives,
        }
    }

    pub(in crate::backend) fn same_type(
        &self,
        a_unit: usize,
        a: &Type,
        b_unit: usize,
        b: &Type,
    ) -> bool {
        let same = |a: &Type, b: &Type| self.same_type(a_unit, a, b_unit, b);
        match (a, b) {
            (Type::Custom(x), Type::Custom(y)) => self.same_name(a_unit, x, b_unit, y),
            (Type::Generic { name: x, args: xs }, Type::Generic { name: y, args: ys }) => {
                self.same_name(a_unit, x, b_unit, y)
                    && xs.len() == ys.len()
                    && xs.iter().zip(ys).all(|(x, y)| same(x, y))
            }
            (
                Type::Ref {
                    mutable: m,
                    inner: x,
                },
                Type::Ref {
                    mutable: n,
                    inner: y,
                },
            )
            | (
                Type::Ptr {
                    mutable: m,
                    inner: x,
                },
                Type::Ptr {
                    mutable: n,
                    inner: y,
                },
            ) => m == n && same(x, y),
            (Type::Slice(x), Type::Slice(y)) | (Type::Complex(x), Type::Complex(y)) => same(x, y),
            (Type::Array { elem: x, len: m }, Type::Array { elem: y, len: n }) => {
                m == n && same(x, y)
            }
            (
                Type::FnPtr {
                    abi: x_abi,
                    params: xs,
                    ret: x,
                },
                Type::FnPtr {
                    abi: y_abi,
                    params: ys,
                    ret: y,
                },
            ) => {
                x_abi == y_abi
                    && xs.len() == ys.len()
                    && xs.iter().zip(ys).all(|(x, y)| same(x, y))
                    && same(x, y)
            }
            (Type::LongDouble | Type::VaList, _) => false,
            (
                Type::Prim(_)
                | Type::String
                | Type::Str
                | Type::TyVar(_)
                | Type::CLib(_)
                | Type::Unit
                | Type::Variadic
                | Type::Never,
                _,
            ) => a == b,
            _ => false,
        }
    }

    fn same_name(&self, a_unit: usize, a: &str, b_unit: usize, b: &str) -> bool {
        match (self.class_of[a_unit].get(a), self.class_of[b_unit].get(b)) {
            (Some(x), Some(y)) => x == y && self.shared_names.contains_key(x),
            (None, None) => a == b && a.contains("::") && !a.starts_with(BITS_MODULE),
            _ => false,
        }
    }

    pub(in crate::backend) fn hoist(&self, programs: &mut [Program]) -> Option<Program> {
        if self.shared_names.is_empty() {
            return None;
        }
        let mut items = Vec::new();
        let mut bits = Vec::new();
        for (index, (unit, name)) in self.representatives.iter().enumerate() {
            let rename = |local: &str| {
                self.class_of[*unit]
                    .get(local)
                    .and_then(|class| self.shared_names.get(class))
                    .cloned()
            };
            let program = &programs[*unit];
            let mut item = program
                .items
                .iter()
                .find_map(|item| match item {
                    Item::Record(record) if record.name == *name => Some(item.clone()),
                    Item::Struct(definition) if definition.name == *name => Some(item.clone()),
                    _ => None,
                })
                .expect("representative is a top-level type item");
            let unit_bits = bit_units(&program.items);
            let mut wrappers = BTreeMap::new();
            let mut rename_bits = |local: &str| {
                let wrapper = local.strip_prefix(BITS_MODULE)?.strip_prefix("::")?;
                let shared = wrappers
                    .entry(wrapper.to_string())
                    .or_insert_with_key(|wrapper| {
                        let shared = format!("__SlateBits{index}U{}", bits.len());
                        if let Some(definition) = unit_bits.get(wrapper.as_str()) {
                            bits.push(Item::Struct(StructDef {
                                name: shared.clone(),
                                ..(*definition).clone()
                            }));
                        }
                        shared
                    });
                Some(format!("{BITS_MODULE}::{shared}"))
            };
            let mut rename_any = |local: &str| rename(local).or_else(|| rename_bits(local));
            match &mut item {
                Item::Record(record) => {
                    record.name = rename(name).expect("representative is hoisted");
                    record.vis = Visibility::Pub;
                    record.field_vis = Visibility::Pub;
                    for field in &mut record.fields {
                        rename_types(&mut field.ty, &mut rename_any);
                    }
                }
                Item::Struct(definition) => {
                    definition.name = rename(name).expect("representative is hoisted");
                    definition.vis = Visibility::Pub;
                    definition.field_vis = Visibility::Pub;
                    match &mut definition.fields {
                        StructFields::Named(fields) => {
                            for field in fields {
                                rename_types(&mut field.ty, &mut rename_any);
                            }
                        }
                        StructFields::Tuple(fields) => {
                            for ty in fields {
                                rename_types(ty, &mut rename_any);
                            }
                        }
                    }
                }
                _ => unreachable!("representatives are records or structs"),
            }
            items.push(item);
        }
        for (unit, program) in programs.iter_mut().enumerate() {
            self.replace_with_imports(unit, &mut program.items);
        }
        if !bits.is_empty() {
            items.push(Item::InlineMod {
                vis: Visibility::Pub,
                name: Ident::new(BITS_MODULE),
                items: bits,
            });
        }
        Some(Program { items })
    }

    fn replace_with_imports(&self, unit: usize, items: &mut Vec<Item>) {
        let shared: BTreeMap<&str, &str> = self.class_of[unit]
            .iter()
            .filter_map(|(local, class)| {
                Some((local.as_str(), self.shared_names.get(class)?.as_str()))
            })
            .collect();
        if shared.is_empty() {
            return;
        }
        let mut wrappers = BTreeSet::new();
        items.retain(|item| {
            let (name, types): (&str, Vec<&Type>) = match item {
                Item::Record(record) => (
                    record.name.as_str(),
                    record.fields.iter().map(|field| &field.ty).collect(),
                ),
                Item::Struct(definition) => (definition.name.as_str(), struct_types(definition)),
                _ => return true,
            };
            if !shared.contains_key(name) {
                return true;
            }
            for ty in types {
                collect_bits(ty, &mut wrappers);
            }
            false
        });
        items.retain_mut(|item| match item {
            Item::InlineMod { name, items, .. } if name.as_str() == BITS_MODULE => {
                items.retain(|item| {
                    !matches!(item, Item::Struct(definition) if wrappers.contains(&definition.name))
                });
                !items.is_empty()
            }
            _ => true,
        });
        let imports = shared.into_iter().map(|(local, shared)| Item::Use {
            vis: Visibility::Private,
            path: Path::new(["crate", SHARED_MODULE, shared].map(Ident::new)),
            alias: (local != shared).then(|| Ident::new(local)),
        });
        super::insert_imports(items, imports);
    }
}

fn refine(nodes: &[Node], edges: &[Vec<usize>], redirect: &[usize]) -> Vec<usize> {
    let mut shapes: HashMap<&(Shape, Vec<StructDef>), usize> = HashMap::new();
    let mut fresh = 0usize;
    let mut classes: Vec<usize> = nodes
        .iter()
        .map(|node| match &node.shape {
            Some(shape) => {
                let next = shapes.len();
                *shapes.entry(shape).or_insert(next)
            }
            None => {
                fresh += 1;
                usize::MAX - fresh
            }
        })
        .collect();
    let mut count = classes.iter().collect::<BTreeSet<_>>().len();
    loop {
        let mut table: HashMap<(usize, Vec<usize>), usize> = HashMap::new();
        let next: Vec<usize> = (0..nodes.len())
            .map(|id| {
                let key = (
                    classes[id],
                    edges[id]
                        .iter()
                        .map(|&edge| classes[redirect[edge]])
                        .collect(),
                );
                let len = table.len();
                *table.entry(key).or_insert(len)
            })
            .collect();
        let done = table.len() == count;
        count = table.len();
        classes = next;
        if done {
            return classes;
        }
    }
}

fn unit_nodes(unit: usize, items: &[Item]) -> Vec<Node> {
    let mut implemented = BTreeSet::new();
    for item in items {
        if let Item::Impl(block) = item {
            type_names(&block.self_ty, &mut implemented);
            if let Some(TraitRef::From(ty)) = &block.trait_ {
                type_names(ty, &mut implemented);
            }
        }
    }
    let local: BTreeSet<&str> = items
        .iter()
        .filter_map(|item| match item {
            Item::Record(record) => Some(record.name.as_str()),
            Item::Struct(definition) => Some(definition.name.as_str()),
            _ => None,
        })
        .collect();
    let bits = bit_units(items);
    items
        .iter()
        .filter_map(|item| {
            let (name, generics) = match item {
                Item::Record(record) => (&record.name, BTreeSet::new()),
                Item::Struct(definition) => (
                    &definition.name,
                    definition
                        .generics
                        .iter()
                        .map(|param| param.name.as_str())
                        .collect(),
                ),
                _ => return None,
            };
            let scope = Scope {
                local: &local,
                generics,
                bits: &bits,
                implemented: &implemented,
            };
            let mut refs = Vec::new();
            let mut wrappers = Vec::new();
            let shape = (!implemented.contains(name))
                .then(|| shape_of(item, &scope, &mut refs, &mut wrappers))
                .flatten()
                .map(|shape| (shape, wrappers));
            if shape.is_none() {
                refs.clear();
            }
            Some(Node {
                unit,
                name: name.clone(),
                shape,
                refs,
                opaque: matches!(item, Item::Record(record) if record.opaque),
            })
        })
        .collect()
}

fn shape_of(
    item: &Item,
    scope: &Scope,
    refs: &mut Vec<String>,
    wrappers: &mut Vec<StructDef>,
) -> Option<Shape> {
    match item {
        Item::Record(record) => {
            let mut shape = RecordDef {
                comments: Vec::new(),
                trailing_comments: Vec::new(),
                name: if is_anonymous(&record.name) {
                    String::new()
                } else {
                    record.name.clone()
                },
                fields: Vec::new(),
                ..record.clone()
            };
            for field in &record.fields {
                let mut field = field.clone();
                field.comments.clear();
                field.ty = abstract_type(&field.ty, scope, refs, wrappers)?;
                shape.fields.push(field);
            }
            Some(Shape::Record(shape))
        }
        Item::Struct(definition) => {
            let mut shape = definition.clone();
            match &mut shape.fields {
                StructFields::Named(fields) => {
                    for field in fields {
                        field.ty = abstract_type(&field.ty, scope, refs, wrappers)?;
                    }
                }
                StructFields::Tuple(fields) => {
                    for ty in fields {
                        *ty = abstract_type(ty, scope, refs, wrappers)?;
                    }
                }
            }
            Some(Shape::Struct(shape))
        }
        _ => None,
    }
}

fn abstract_type(
    ty: &Type,
    scope: &Scope,
    refs: &mut Vec<String>,
    wrappers: &mut Vec<StructDef>,
) -> Option<Type> {
    let mut nested = |ty: &Type| abstract_type(ty, scope, refs, wrappers).map(Box::new);
    Some(match ty {
        Type::Custom(name) => Type::Custom(abstract_name(name, scope, refs, wrappers)?),
        Type::Generic { name, args } => {
            let name = abstract_name(name, scope, refs, wrappers)?;
            let args = args
                .iter()
                .map(|arg| abstract_type(arg, scope, refs, wrappers))
                .collect::<Option<_>>()?;
            Type::Generic { name, args }
        }
        Type::Ref { mutable, inner } => Type::Ref {
            mutable: *mutable,
            inner: nested(inner)?,
        },
        Type::Ptr { mutable, inner } => Type::Ptr {
            mutable: *mutable,
            inner: nested(inner)?,
        },
        Type::Slice(inner) => Type::Slice(nested(inner)?),
        Type::Complex(inner) => Type::Complex(nested(inner)?),
        Type::Array { elem, len } => Type::Array {
            elem: nested(elem)?,
            len: *len,
        },
        Type::FnPtr { abi, params, ret } => Type::FnPtr {
            abi: *abi,
            params: params
                .iter()
                .map(|param| abstract_type(param, scope, refs, wrappers))
                .collect::<Option<_>>()?,
            ret: Box::new(abstract_type(ret, scope, refs, wrappers)?),
        },
        Type::LongDouble | Type::VaList => return None,
        Type::Prim(_)
        | Type::String
        | Type::Str
        | Type::TyVar(_)
        | Type::CLib(_)
        | Type::Unit
        | Type::Variadic
        | Type::Never => ty.clone(),
    })
}

fn abstract_name(
    name: &str,
    scope: &Scope,
    refs: &mut Vec<String>,
    wrappers: &mut Vec<StructDef>,
) -> Option<String> {
    if let Some(wrapper) = name
        .strip_prefix(BITS_MODULE)
        .and_then(|rest| rest.strip_prefix("::"))
    {
        let definition = scope.bits.get(wrapper)?;
        if scope.implemented.contains(name) {
            return None;
        }
        let leaf = Scope {
            local: &BTreeSet::new(),
            generics: BTreeSet::new(),
            bits: &BTreeMap::new(),
            implemented: scope.implemented,
        };
        let mut shape = StructDef {
            name: String::new(),
            ..(*definition).clone()
        };
        let mut nested_refs = Vec::new();
        let mut nested_wrappers = Vec::new();
        if let StructFields::Named(fields) = &mut shape.fields {
            for field in fields {
                field.ty = abstract_type(&field.ty, &leaf, &mut nested_refs, &mut nested_wrappers)?;
            }
        }
        wrappers.push(shape);
        return Some(PLACEHOLDER.to_string());
    }
    if scope.generics.contains(name) || name.contains("::") {
        return Some(name.to_string());
    }
    scope.local.contains(name).then(|| {
        refs.push(name.to_string());
        PLACEHOLDER.to_string()
    })
}

fn bit_units(items: &[Item]) -> BTreeMap<&str, &StructDef> {
    items
        .iter()
        .filter_map(|item| match item {
            Item::InlineMod { name, items, .. } if name.as_str() == BITS_MODULE => Some(items),
            _ => None,
        })
        .flatten()
        .filter_map(|item| match item {
            Item::Struct(definition) => Some((definition.name.as_str(), definition)),
            _ => None,
        })
        .collect()
}

fn struct_types(definition: &StructDef) -> Vec<&Type> {
    match &definition.fields {
        StructFields::Named(fields) => fields.iter().map(|field| &field.ty).collect(),
        StructFields::Tuple(fields) => fields.iter().collect(),
    }
}

fn type_names(ty: &Type, out: &mut BTreeSet<String>) {
    match ty {
        Type::Custom(name) => {
            out.insert(name.clone());
        }
        Type::LongDouble => {
            out.insert(codegen::type_to_string(ty));
        }
        Type::Generic { name, args } => {
            out.insert(name.clone());
            for arg in args {
                type_names(arg, out);
            }
        }
        Type::Ref { inner, .. }
        | Type::Ptr { inner, .. }
        | Type::Slice(inner)
        | Type::Complex(inner)
        | Type::Array { elem: inner, .. } => type_names(inner, out),
        Type::FnPtr { params, ret, .. } => {
            for param in params {
                type_names(param, out);
            }
            type_names(ret, out);
        }
        _ => {}
    }
}

fn collect_bits(ty: &Type, out: &mut BTreeSet<String>) {
    match ty {
        Type::Custom(name) => {
            if let Some(wrapper) = name
                .strip_prefix(BITS_MODULE)
                .and_then(|rest| rest.strip_prefix("::"))
            {
                out.insert(wrapper.to_string());
            }
        }
        Type::Generic { args, .. } => {
            for arg in args {
                collect_bits(arg, out);
            }
        }
        Type::Ref { inner, .. }
        | Type::Ptr { inner, .. }
        | Type::Slice(inner)
        | Type::Complex(inner)
        | Type::Array { elem: inner, .. } => collect_bits(inner, out),
        Type::FnPtr { params, ret, .. } => {
            for param in params {
                collect_bits(param, out);
            }
            collect_bits(ret, out);
        }
        _ => {}
    }
}

fn rename_types(ty: &mut Type, rename: &mut impl FnMut(&str) -> Option<String>) {
    match ty {
        Type::Custom(name) => {
            if let Some(renamed) = rename(name) {
                *name = renamed;
            }
        }
        Type::Generic { name, args } => {
            if let Some(renamed) = rename(name) {
                *name = renamed;
            }
            for arg in args {
                rename_types(arg, rename);
            }
        }
        Type::Ref { inner, .. }
        | Type::Ptr { inner, .. }
        | Type::Slice(inner)
        | Type::Complex(inner)
        | Type::Array { elem: inner, .. } => rename_types(inner, rename),
        Type::FnPtr { params, ret, .. } => {
            for param in params {
                rename_types(param, rename);
            }
            rename_types(ret, rename);
        }
        _ => {}
    }
}

fn is_anonymous(name: &str) -> bool {
    name.strip_prefix("__SlateRecord")
        .is_some_and(|id| !id.is_empty() && id.bytes().all(|byte| byte.is_ascii_digit()))
}
