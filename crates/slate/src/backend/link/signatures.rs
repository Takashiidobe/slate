use std::collections::{BTreeMap, BTreeSet, HashMap};

use super::types::{BITS_MODULE, is_anonymous};
use crate::backend::codegen::type_to_string;
use crate::backend::rust_ast::{
    Abi, Attr, ExternDecl, FnParam, Item, Program, Repr, StructDef, StructFields, Type,
};

pub struct UnitCompiles<'a> {
    pub stem: &'a str,
    pub relevant: &'a [String],
    pub compiled: Vec<(u32, &'a Program)>,
}

pub struct Mismatch {
    pub symbol: String,
    pub features: Vec<String>,
    pub left: (String, String),
    pub right: (String, String),
}

pub struct Outcome {
    pub shared: usize,
    pub mismatches: Vec<Mismatch>,
}

#[derive(Clone, PartialEq, Eq, PartialOrd, Ord)]
enum Signature {
    Function {
        abi: String,
        params: Vec<String>,
        variadic: bool,
        ret: String,
    },
    Data {
        ty: String,
        array: Option<(String, u64)>,
    },
}

impl Signature {
    fn compatible(&self, other: &Self) -> bool {
        match (self, other) {
            (
                Signature::Function {
                    abi,
                    params,
                    variadic,
                    ret,
                },
                Signature::Function {
                    abi: other_abi,
                    params: other_params,
                    variadic: other_variadic,
                    ret: other_ret,
                },
            ) => {
                let unprototyped = (*variadic && params.is_empty())
                    || (*other_variadic && other_params.is_empty());
                abi == other_abi
                    && ret == other_ret
                    && (unprototyped || (params == other_params && variadic == other_variadic))
            }
            (
                Signature::Data { ty, array },
                Signature::Data {
                    ty: other_ty,
                    array: other_array,
                },
            ) => {
                ty == other_ty && array.is_none() && other_array.is_none()
                    || matches!(
                        (array, other_array),
                        (Some((elem, len)), Some((other_elem, other_len)))
                            if elem == other_elem
                                && (len == other_len || *len == 0 || *other_len == 0)
                    )
            }
            _ => false,
        }
    }

    fn describe(&self) -> String {
        match self {
            Signature::Function {
                abi,
                params,
                variadic,
                ret,
            } => {
                let mut list = params.clone();
                if *variadic {
                    list.push("...".to_string());
                }
                format!("extern \"{abi}\" fn({}) -> {ret}", list.join(", "))
            }
            Signature::Data { ty, .. } => ty.clone(),
        }
    }
}

struct Scope<'a> {
    definitions: HashMap<String, &'a Item>,
}

impl<'a> Scope<'a> {
    fn of(program: &'a Program) -> Self {
        let mut definitions = HashMap::new();
        for item in &program.items {
            match item {
                Item::Record(record) => {
                    definitions.insert(record.name.clone(), item);
                }
                Item::Struct(definition) => {
                    definitions.insert(definition.name.clone(), item);
                }
                Item::InlineMod { name, items, .. } if name.as_str() == BITS_MODULE => {
                    for item in items {
                        if let Item::Struct(definition) = item {
                            definitions.insert(format!("{BITS_MODULE}::{}", definition.name), item);
                        }
                    }
                }
                _ => {}
            }
        }
        Scope { definitions }
    }

    fn named(&self, name: &str, args: &[Type], behind_pointer: bool) -> String {
        let args = args
            .iter()
            .map(|arg| self.render(arg, behind_pointer))
            .collect::<Vec<_>>();
        let args = if args.is_empty() {
            String::new()
        } else {
            format!("<{}>", args.join(", "))
        };
        match self.definitions.get(name) {
            Some(item) if !behind_pointer || is_anonymous(name) => {
                format!("{}{args}", self.layout(item))
            }
            _ => format!("{name}{args}"),
        }
    }

    fn layout(&self, item: &Item) -> String {
        match item {
            Item::Record(record) => {
                let fields: Vec<String> = record
                    .fields
                    .iter()
                    .map(|field| self.render(&field.ty, false))
                    .collect();
                format!(
                    "{}{{{}}}{}{}",
                    if record.is_union { "union" } else { "struct" },
                    fields.join(", "),
                    record
                        .packed
                        .map_or(String::new(), |packed| format!(" packed({packed})")),
                    record
                        .align
                        .map_or(String::new(), |align| format!(" align({align})")),
                )
            }
            Item::Struct(definition) => self.struct_layout(definition),
            _ => unreachable!("scopes only hold records and structs"),
        }
    }

    fn struct_layout(&self, definition: &StructDef) -> String {
        let fields: Vec<String> = match &definition.fields {
            StructFields::Tuple(types) => types.iter().map(|ty| self.render(ty, false)).collect(),
            StructFields::Named(fields) => fields
                .iter()
                .map(|field| self.render(&field.ty, false))
                .collect(),
        };
        let repr: Vec<String> = definition
            .attrs
            .iter()
            .filter(|attr| matches!(attr, Attr::Repr(_)))
            .map(|attr| format!("{attr:?}"))
            .collect();
        format!("struct{{{}}}{}", fields.join(", "), repr.join(""))
    }

    fn render(&self, ty: &Type, behind_pointer: bool) -> String {
        match ty {
            Type::Custom(name) => self.named(name, &[], behind_pointer),
            Type::Generic { name, args } => self.named(name, args, behind_pointer),
            Type::Ptr { mutable, inner } | Type::Ref { mutable, inner } => format!(
                "*{} {}",
                if *mutable { "mut" } else { "const" },
                self.render(inner, true)
            ),
            Type::Array { elem, len } => format!("[{}; {len}]", self.render(elem, behind_pointer)),
            Type::Slice(inner) => format!("[{}]", self.render(inner, behind_pointer)),
            Type::Complex(inner) => format!("complex {}", self.render(inner, behind_pointer)),
            Type::FnPtr { abi, params, ret } => {
                let params: Vec<String> = params
                    .iter()
                    .map(|param| match param {
                        Type::Variadic => "...".to_string(),
                        param => self.render(param, true),
                    })
                    .collect();
                format!(
                    "extern \"{}\" fn({}) -> {}",
                    call_convention(abi.spelling()),
                    params.join(", "),
                    self.ret(Some(ret), true)
                )
            }
            other => type_to_string(other),
        }
    }

    fn ret(&self, ret: Option<&Type>, behind_pointer: bool) -> String {
        match ret {
            None | Some(Type::Unit | Type::Never) => "()".to_string(),
            Some(ty) => self.render(ty, behind_pointer),
        }
    }

    fn without_alignment_wrapper<'t>(&self, ty: &'t Type) -> &'t Type {
        match ty {
            Type::Generic { name, args } => match (self.definitions.get(name), args.as_slice()) {
                (Some(Item::Struct(definition)), [inner]) if is_alignment_wrapper(definition) => {
                    inner
                }
                _ => ty,
            },
            _ => ty,
        }
    }

    fn function(
        &self,
        abi: &str,
        params: &[FnParam],
        variadic: bool,
        ret: Option<&Type>,
    ) -> Signature {
        let named: Vec<&FnParam> = params
            .iter()
            .filter(|param| param.ty != Type::Variadic)
            .collect();
        Signature::Function {
            abi: call_convention(abi).to_string(),
            variadic: variadic || named.len() != params.len(),
            params: named
                .iter()
                .map(|param| self.render(&param.ty, false))
                .collect(),
            ret: self.ret(ret, false),
        }
    }

    fn data(&self, ty: &Type) -> Signature {
        let ty = self.without_alignment_wrapper(ty);
        Signature::Data {
            ty: self.render(ty, false),
            array: match ty {
                Type::Array { elem, len } => Some((self.render(elem, false), *len)),
                _ => None,
            },
        }
    }
}

fn is_alignment_wrapper(definition: &StructDef) -> bool {
    matches!(definition.generics.as_slice(), [_])
        && matches!(&definition.fields, StructFields::Tuple(fields) if fields.len() == 1)
        && definition.attrs.iter().any(|attr| {
            matches!(attr, Attr::Repr(reprs) if reprs.iter().any(|repr| matches!(repr, Repr::Align(_))))
        })
}

fn call_convention(abi: &str) -> &str {
    abi.strip_suffix("-unwind").unwrap_or(abi)
}

fn defined_symbol<'a>(attrs: &'a [Attr], name: &'a str) -> Option<&'a str> {
    attrs.iter().find_map(|attr| match attr {
        Attr::NoMangle => Some(name),
        Attr::ExportName(exported) => Some(exported.as_str()),
        _ => None,
    })
}

fn declared_symbol<'a>(attrs: &'a [Attr], name: &'a str) -> &'a str {
    attrs
        .iter()
        .find_map(|attr| match attr {
            Attr::LinkName(linked) => Some(linked.as_str()),
            _ => None,
        })
        .unwrap_or(name)
}

fn collect(items: &[Item], scope: &Scope, out: &mut impl FnMut(&str, Signature)) {
    for item in items {
        match item {
            Item::Fn(function) => {
                if let Some(symbol) = defined_symbol(&function.attrs, &function.name) {
                    let abi = function.abi.unwrap_or(Abi::Rust).spelling();
                    out(
                        symbol,
                        scope.function(abi, &function.params, false, function.ret.as_ref()),
                    );
                }
            }
            Item::Static {
                attrs, name, ty, ..
            } => {
                if let Some(symbol) = defined_symbol(attrs, name) {
                    out(symbol, scope.data(ty));
                }
            }
            Item::ExternBlock { abi, decls } => {
                for decl in decls {
                    match decl {
                        ExternDecl::Fn(decl) => out(
                            declared_symbol(&decl.attrs, &decl.name),
                            scope.function(abi, &decl.params, decl.variadic, decl.ret.as_ref()),
                        ),
                        ExternDecl::Static {
                            attrs, name, ty, ..
                        } => out(declared_symbol(attrs, name), scope.data(ty)),
                    }
                }
            }
            Item::InlineMod { items, .. } => collect(items, scope, out),
            Item::Cfg { item, .. } => collect(std::slice::from_ref(item), scope, out),
            _ => {}
        }
    }
}

fn enabled(relevant: &[String], mask: u32) -> impl Iterator<Item = &String> {
    relevant
        .iter()
        .enumerate()
        .filter(move |(bit, _)| mask >> bit & 1 == 1)
        .map(|(_, name)| name)
}

fn witness(
    left: (&[String], &BTreeSet<u32>),
    right: (&[String], &BTreeSet<u32>),
    invalid: &impl Fn(&[String]) -> bool,
) -> Option<Vec<String>> {
    let common: Vec<(usize, usize)> = left
        .0
        .iter()
        .enumerate()
        .filter_map(|(bit, name)| {
            right
                .0
                .iter()
                .position(|other| other == name)
                .map(|other| (bit, other))
        })
        .collect();
    for &left_mask in left.1 {
        for &right_mask in right.1 {
            if common
                .iter()
                .any(|&(l, r)| left_mask >> l & 1 != right_mask >> r & 1)
            {
                continue;
            }
            let features: BTreeSet<String> = enabled(left.0, left_mask)
                .chain(enabled(right.0, right_mask))
                .cloned()
                .collect();
            let features: Vec<String> = features.into_iter().collect();
            if !invalid(&features) {
                return Some(features);
            }
        }
    }
    None
}

type Variants = BTreeMap<Signature, BTreeSet<u32>>;

pub fn check(units: &[UnitCompiles], invalid: &impl Fn(&[String]) -> bool) -> Outcome {
    let mut symbols: BTreeMap<String, BTreeMap<usize, Variants>> = BTreeMap::new();
    for (index, unit) in units.iter().enumerate() {
        for &(mask, program) in &unit.compiled {
            let scope = Scope::of(program);
            collect(&program.items, &scope, &mut |symbol, signature| {
                symbols
                    .entry(symbol.to_string())
                    .or_default()
                    .entry(index)
                    .or_default()
                    .entry(signature)
                    .or_default()
                    .insert(mask);
            });
        }
    }
    let mut outcome = Outcome {
        shared: symbols.values().filter(|units| units.len() > 1).count(),
        mismatches: Vec::new(),
    };
    for (symbol, by_unit) in &symbols {
        for (&left, left_variants) in by_unit {
            for (&right, right_variants) in by_unit.range(left + 1..) {
                for (left_signature, left_on) in left_variants {
                    for (right_signature, right_on) in right_variants {
                        if left_signature.compatible(right_signature) {
                            continue;
                        }
                        let Some(features) = witness(
                            (units[left].relevant, left_on),
                            (units[right].relevant, right_on),
                            invalid,
                        ) else {
                            continue;
                        };
                        outcome.mismatches.push(Mismatch {
                            symbol: symbol.clone(),
                            features,
                            left: (units[left].stem.to_string(), left_signature.describe()),
                            right: (units[right].stem.to_string(), right_signature.describe()),
                        });
                    }
                }
            }
        }
    }
    outcome
}
