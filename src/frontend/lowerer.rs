//! lower: combine the CIR Op-tree with the C AST oracle into Rust output.

use crate::backend::rust_ast::{
    Abi, AsmDialect, AsmOperand, AsmReg, AtomicOrdering, AtomicPlace, AtomicRmwOp, AtomicType,
    Attr as RustAttr, AttrArg, BinOp, CLIB_RECORD_TYPES, CLibInitializer, CLibType, Cfg, Comment,
    CrateAttr, Derive, EnumConst, EnumDef, Expr, ExprMatchArm, ExternDecl, ExternFnDecl, Feature,
    FnDef, FnParam, Ident, ImplBlock, ImplItem, IndentStmt, InlineAsm, Item, Label, Lint, MatchArm,
    Method, Path, Pattern, Prim, Program, Raw, RecordDef, RecordField, Repr, RustValue, SelfKind,
    StdTrait, Stmt, StructDef, StructField, StructFields, SupportModule, TraitRef, Type, UnaryOp,
    UsedKind, Visibility,
};
use crate::cir::ir::{Attr, Block, CirOpKind, CirType, Module, Op, OpKindExt, Region};
use crate::ctx::Ctx;
use crate::frontend::c_ast::{
    CType, EnumConstRef, FloatingLiteralFact, FloatingLiteralLoc, LayoutQuery, Loc, MacroConst,
    RecordKind, SourcePoint, Unit,
};
use crate::frontend::function_abi::repair_function_signature;
use crate::function_identity::{CallBinding, FunctionIdentity, Known};
use clang_ir::ast::SourceLocation;
use clang_ir::model::Op as TypedOp;
use std::collections::{BTreeMap, BTreeSet, HashMap, VecDeque};

mod analysis;
mod arithmetic;
mod asm;
mod atomic;
mod bitfields;
mod builtins;
mod calls;
mod cir_ops;
mod constants;
mod control_flow;
mod expressions;
mod intrinsics;
mod memory;
mod op_utils;
mod runtime_support;
mod storage;
mod types;
mod values;

use analysis::*;
use asm::*;
use atomic::*;
use bitfields::*;
use cir_ops::*;
use constants::*;
use op_utils::*;
use runtime_support::*;
pub use types::anon_local_records;
use types::*;

#[derive(Default, Clone)]
pub struct ProjectInfo {
    pub cross_module: BTreeMap<String, String>,
    pub cross_module_globals: BTreeMap<String, String>,
    pub child_modules: Vec<String>,
    pub shared_records: BTreeSet<String>,
    pub shared_enums: BTreeSet<String>,
    pub shared_type_module: Option<String>,
    pub shared_type_crate: Option<String>,
    pub shared_long_double: bool,
    pub cross_module_crate: Option<String>,
    pub unsafe_functions: BTreeSet<String>,
    pub crate_features: BTreeSet<Feature>,
    pub emit_pub: bool,
    pub cross_referenced_functions: BTreeSet<String>,
    pub cross_referenced_globals: BTreeSet<String>,
    pub address_taken_functions: BTreeSet<String>,
}

const WEAK_ANY_LINKAGE: i64 = 4;
const HIDDEN_VISIBILITY: i64 = 1;
const PROTECTED_VISIBILITY: i64 = 2;

fn attr_array_values(attr: &Attr) -> Option<&[Attr]> {
    match attr {
        Attr::Array(values) => Some(values),
        _ => None,
    }
}

fn source_location_text(loc: &SourceLocation) -> Option<String> {
    match loc {
        SourceLocation::File { file, line, column } => {
            Some(format!("loc(\"{file}\":{line}:{column})"))
        }
        SourceLocation::Loc(raw) => Some(raw.clone()),
        _ => None,
    }
}

fn rust_record_name(name: &str) -> String {
    sanitize_ident(name).into_string()
}

fn lowered_record_name(name: &str) -> Option<String> {
    (name != "__va_list_tag" && clib_record_type(name).is_none()).then(|| rust_record_name(name))
}

fn collect_record_dependencies(
    ty: &crate::frontend::c_ast::CType,
    dependencies: &mut BTreeSet<String>,
) {
    use crate::frontend::c_ast::CType;
    match ty {
        CType::Ptr(inner) | CType::Array(inner, _) => {
            collect_record_dependencies(inner, dependencies);
        }
        CType::FuncPtr { ret, params } => {
            collect_record_dependencies(ret, dependencies);
            for param in params {
                collect_record_dependencies(param, dependencies);
            }
        }
        CType::Record(name) => {
            if let Some(name) = lowered_record_name(name) {
                dependencies.insert(name);
            }
        }
        _ => {}
    }
}

fn widen_flexible_array_members(
    cir: &Module,
    records: &mut BTreeMap<String, crate::frontend::c_ast::Record>,
) {
    for op in module_ops(cir) {
        if op.kind() != CirOpKind::Global {
            continue;
        }
        let Some(sym_type) = attr_type(op, "sym_type") else {
            continue;
        };
        let expanded = cir.resolve_type(sym_type);
        let Some(record_name) = cir_record_name(expanded) else {
            continue;
        };
        let record_name = sanitize_ident(record_name).into_string();
        let Some(record) = records.get(&record_name) else {
            continue;
        };
        let is_flexible_array_candidate = record.kind == RecordKind::Struct
            && matches!(
                record.fields.last(),
                Some(field) if matches!(field.ty, CType::Array(_, None | Some(0) | Some(1)))
            );
        if !is_flexible_array_candidate {
            continue;
        }
        let Some(len) = op.attr("initial_value").and_then(|attr| match attr {
            Attr::ConstRecord { members, .. } => {
                attr_array_values(members)?
                    .last()
                    .and_then(|elem| match elem {
                        Attr::ConstArray { ty, .. } | Attr::Zero { ty } => {
                            parse_cir_array_type(ty).map(|(_, len)| len)
                        }
                        _ => None,
                    })
            }
            _ => None,
        }) else {
            continue;
        };
        let record = records.get_mut(&record_name).expect("checked above");
        if let Some(field) = record.fields.last_mut()
            && let CType::Array(_, declared_len) = &mut field.ty
            && declared_len.unwrap_or(0) < len
        {
            *declared_len = Some(len);
        }
    }
}

fn required_record_defs(
    cir: &Module,
    c: &Unit,
    records: &mut BTreeMap<String, crate::frontend::c_ast::Record>,
    shared_records: &BTreeSet<String>,
) -> BTreeSet<String> {
    let mut candidates = BTreeMap::new();
    for record in c
        .records
        .iter()
        .chain(&c.anonymous_header_records)
        .chain(&c.named_header_records)
    {
        candidates
            .entry(rust_record_name(&record.name))
            .or_insert_with(|| record.clone());
    }
    let cir_kinds: BTreeMap<String, RecordKind> = cir
        .type_aliases
        .iter()
        .filter_map(|(alias, ty)| {
            let name =
                lowered_record_name(cir_record_name(ty).or_else(|| alias.strip_prefix("rec_"))?)?;
            let kind = match ty {
                CirType::Struct { .. } => RecordKind::Struct,
                CirType::Union { .. } => RecordKind::Union,
                _ => return None,
            };
            Some((name, kind))
        })
        .collect();
    let mut required: BTreeSet<String> = cir_kinds.keys().cloned().collect();
    for query in c
        .functions
        .iter()
        .flat_map(|function| &function.layout_queries)
    {
        match query {
            LayoutQuery::Size(ty) | LayoutQuery::Align(ty) => {
                collect_record_dependencies(ty, &mut required);
            }
            LayoutQuery::Offset { record, .. } => {
                if let Some(name) = lowered_record_name(record) {
                    required.insert(name);
                }
            }
        }
    }
    let mut frontier: Vec<String> = required.iter().cloned().collect();
    while let Some(name) = frontier.pop() {
        if shared_records.contains(&name) {
            continue;
        }
        if !records.contains_key(&name) {
            let record = candidates
                .get(&name)
                .cloned()
                .map(|mut record| {
                    record.name = name.clone();
                    record
                })
                .unwrap_or_else(|| crate::frontend::c_ast::Record {
                    name: name.clone(),
                    comments: Vec::new(),
                    kind: cir_kinds.get(&name).copied().unwrap_or(RecordKind::Struct),
                    fields: Vec::new(),
                    packed: None,
                    align: None,
                });
            records.insert(name.clone(), record);
        }
        let mut dependencies = BTreeSet::new();
        for field in &records[&name].fields {
            collect_record_dependencies(&field.ty, &mut dependencies);
        }
        for dependency in dependencies {
            if required.insert(dependency.clone()) {
                frontier.push(dependency);
            }
        }
    }
    required
}

pub fn shim_records_for_module(cir: &Module, c: &Unit) -> Vec<crate::frontend::c_ast::Record> {
    let referenced: BTreeSet<&str> = cir
        .type_aliases
        .values()
        .filter_map(|ty| cir_record_name(ty))
        .collect();
    c.named_header_records
        .iter()
        .filter(|&record| referenced.contains(record.name.as_str()))
        .cloned()
        .collect()
}

fn linkage_is_external(op: &Op) -> bool {
    matches!(attr_int(op, "linkage").unwrap_or(0), 0 | WEAK_ANY_LINKAGE)
}

fn visibility_allows_export(op: &Op) -> bool {
    attr_int(op, "global_visibility").unwrap_or(0) != HIDDEN_VISIBILITY
}

fn externally_exported(op: &Op) -> bool {
    linkage_is_external(op) && visibility_allows_export(op)
}

fn visibility_is_protected(op: &Op) -> bool {
    attr_int(op, "global_visibility") == Some(PROTECTED_VISIBILITY)
}

fn linkage_is_weak(op: &Op) -> bool {
    attr_int(op, "linkage") == Some(WEAK_ANY_LINKAGE)
}

fn symbol_attrs(
    no_mangle: bool,
    weak_linkage: bool,
    section: Option<&str>,
    used: &[UsedKind],
) -> Vec<RustAttr> {
    let mut attrs = Vec::new();
    if no_mangle {
        attrs.push(RustAttr::NoMangle);
    }
    if weak_linkage {
        attrs.push(RustAttr::WeakLinkage);
    }
    if let Some(section) = section {
        attrs.push(RustAttr::LinkSection(section.to_string()));
    }
    attrs.extend(used.iter().copied().map(RustAttr::Used));
    attrs
}

fn insert_crate_feature(items: &mut [Item], feature: Feature) {
    let Some(Item::CrateAttrs(attrs)) = items.first_mut() else {
        return;
    };
    if !attrs
        .iter()
        .any(|attr| matches!(attr, CrateAttr::Feature(existing) if *existing == feature))
    {
        attrs.insert(0, CrateAttr::Feature(feature));
    }
}

fn region_ends_in_noreturn_call(region: &Region) -> bool {
    if region.blocks.len() != 1 {
        return false;
    }
    let Some(block) = region.blocks.first() else {
        return false;
    };
    let ops: &[Op] = match block.ops.last() {
        Some(last) if matches!(last.kind(), CirOpKind::Return | CirOpKind::Yield) => {
            &block.ops[..block.ops.len() - 1]
        }
        _ => &block.ops,
    };
    match ops.last() {
        Some(op) if op.kind() == CirOpKind::Call => attr_bool(op, "noreturn"),
        Some(op) if op.kind() == CirOpKind::Scope => {
            op.regions.first().is_some_and(region_ends_in_noreturn_call)
        }
        _ => false,
    }
}

fn function_requires_unsafe_contract(op: &Op) -> bool {
    let Some(function_type) = attr_type(op, "function_type") else {
        return false;
    };
    let (params, _) = parse_function_type(function_type);
    if !params
        .iter()
        .any(|ty| matches!(ty, CirType::Pointer { .. }))
    {
        return false;
    }

    let mut ops = Vec::new();
    collect_region_ops_recursive(op, &mut ops);
    let mut local_ptrs = BTreeSet::new();
    for op in ops {
        if op.kind() == CirOpKind::Alloca
            || (matches!(op.kind(), CirOpKind::GetMember | CirOpKind::GetElement)
                && op
                    .operands
                    .first()
                    .is_some_and(|base| local_ptrs.contains(base)))
        {
            local_ptrs.extend(op.results.iter().map(|(id, _)| id.clone()));
        } else if matches!(op.kind(), CirOpKind::Load | CirOpKind::Store)
            && op
                .operands
                .last()
                .is_some_and(|ptr| !local_ptrs.contains(ptr))
        {
            return true;
        }
    }
    false
}

fn builtin_module(module: &Module) -> Option<&Op> {
    module.ops.iter().find(|op| op.name == "builtin.module")
}

fn module_ops(module: &Module) -> Vec<&Op> {
    builtin_module(module).map(region_ops).unwrap_or_default()
}

/// Extracts `#cir.block_addr_info<"label", ...>` labels from a `#cir.const_array<[...]>`
/// element list, if every element carries one - anything else (a plain
/// numeric/aggregate array) isn't a block-address table.
fn block_addr_labels(items: &[Attr]) -> Option<Vec<String>> {
    let labels: Vec<String> = items
        .iter()
        .filter_map(|item| {
            let Attr::Dialect {
                dialect,
                mnemonic,
                raw: Some(raw),
                ..
            } = item
            else {
                return None;
            };
            if dialect != "cir" || mnemonic != "block_addr_info" {
                return None;
            }
            let start = raw.find('"')? + 1;
            let rest = &raw[start..];
            let end = rest.find('"')?;
            Some(rest[..end].to_string())
        })
        .collect();
    (labels.len() == items.len() && !labels.is_empty()).then_some(labels)
}

pub fn defined_functions(module: &Module) -> Vec<String> {
    module_ops(module)
        .iter()
        .filter(|op| {
            op.kind() == CirOpKind::Func
                && linkage_is_external(op)
                && (!region_ops(op).is_empty()
                    || attr_symbol_ref(op, "aliasee").is_some() && !linkage_is_weak(op))
        })
        .filter_map(|op| attr_str(op, "sym_name").map(str::to_string))
        .collect()
}

pub fn address_taken_functions(module: &Module) -> BTreeSet<String> {
    builtin_module(module)
        .map(c_abi_function_targets)
        .unwrap_or_default()
}

pub fn declared_functions(module: &Module) -> Vec<String> {
    module_ops(module)
        .iter()
        .filter(|op| op.kind() == CirOpKind::Func && region_ops(op).is_empty())
        .filter_map(|op| attr_str(op, "sym_name").map(str::to_string))
        .collect()
}

pub fn defined_globals(module: &Module) -> Vec<String> {
    module_ops(module)
        .iter()
        .filter(|op| {
            op.kind() == CirOpKind::Global
                && op.attr("initial_value").is_some()
                && linkage_is_external(op)
        })
        .filter_map(|op| attr_str(op, "sym_name").map(|name| sanitize_ident(name).into_string()))
        .collect()
}

pub fn declared_globals(module: &Module) -> Vec<String> {
    module_ops(module)
        .iter()
        .filter(|op| op.kind() == CirOpKind::Global && op.attr("initial_value").is_none())
        .filter_map(|op| attr_str(op, "sym_name").map(|name| sanitize_ident(name).into_string()))
        .collect()
}

fn allocate_global_rust_names(
    module: &Module,
    records: &BTreeMap<String, crate::frontend::c_ast::Record>,
    enums: &BTreeMap<String, crate::frontend::c_ast::Enum>,
) -> BTreeMap<String, String> {
    let ops = module_ops(module);
    if ops.is_empty() {
        return BTreeMap::new();
    }
    let symbols: BTreeSet<String> = ops
        .iter()
        .filter_map(|op| attr_str(op, "sym_name"))
        .map(|name| sanitize_ident(name).into_string())
        .collect();
    let type_names: BTreeSet<String> = records.keys().chain(enums.keys()).cloned().collect();
    let mut used = symbols.clone();
    used.extend(type_names.iter().cloned());
    let globals: BTreeSet<String> = ops
        .iter()
        .filter(|op| op.kind() == CirOpKind::Global)
        .filter_map(|op| attr_str(op, "sym_name").map(str::to_string))
        .collect();
    let mut names = BTreeMap::new();
    for source_name in globals {
        let base = sanitize_ident(&source_name).into_string();
        if !type_names.contains(&base) {
            continue;
        }
        let mut suffix = 0;
        let rust_name = loop {
            let candidate = if suffix == 0 {
                format!("{base}__value")
            } else {
                format!("{base}__value{suffix}")
            };
            if used.insert(candidate.clone()) {
                break candidate;
            }
            suffix += 1;
        };
        names.insert(source_name, rust_name);
    }
    names
}

pub fn unsafe_defined_functions(module: &Module) -> BTreeSet<String> {
    let ops = module_ops(module);
    let mut unsafe_functions: BTreeSet<String> = ops
        .iter()
        .filter(|op| {
            op.kind() == CirOpKind::Func && !region_ops(op).is_empty() && linkage_is_external(op)
        })
        .filter(|op| {
            function_requires_unsafe_contract(op)
                || attr_type(op, "function_type").is_some_and(function_type_is_variadic)
        })
        .filter_map(|op| attr_str(op, "sym_name").map(str::to_string))
        .filter(|name| name != "main")
        .collect();
    for op in ops {
        if op.kind() != CirOpKind::Func || !region_ops(op).is_empty() {
            continue;
        }
        let Some(name) = attr_str(op, "sym_name") else {
            continue;
        };
        let Some(target) = attr_symbol_ref(op, "aliasee") else {
            continue;
        };
        if unsafe_functions.contains(target) {
            unsafe_functions.insert(name.to_string());
        }
    }
    unsafe_functions
}

fn cir_type_mentions_f128(ty: &CirType) -> bool {
    match ty {
        CirType::Fp128 => true,
        CirType::LongDouble { underlying: inner }
        | CirType::Pointer { pointee: inner, .. }
        | CirType::Complex {
            element_type: inner,
        } => cir_type_mentions_f128(inner),
        CirType::Array {
            element_type: element,
            ..
        }
        | CirType::Vector {
            element_type: element,
            ..
        } => cir_type_mentions_f128(element),
        CirType::Func {
            inputs,
            optional_return_type,
            ..
        } => {
            inputs.iter().any(cir_type_mentions_f128)
                || optional_return_type
                    .as_deref()
                    .is_some_and(cir_type_mentions_f128)
        }
        CirType::FunctionType { inputs, results } => {
            inputs.iter().any(cir_type_mentions_f128) || results.iter().any(cir_type_mentions_f128)
        }
        CirType::Struct {
            members: Some(members),
            ..
        }
        | CirType::Union {
            members: Some(members),
            ..
        } => members.iter().any(cir_type_mentions_f128),
        _ => false,
    }
}

fn attr_mentions_f128(attr: &Attr) -> bool {
    match attr {
        Attr::Type(ty)
        | Attr::Float { ty: Some(ty), .. }
        | Attr::Int { ty: Some(ty), .. }
        | Attr::CirBool { ty, .. }
        | Attr::ConstArray { ty, .. }
        | Attr::ConstVector { ty, .. }
        | Attr::ConstRecord { ty, .. }
        | Attr::GlobalView { ty, .. }
        | Attr::Zero { ty }
        | Attr::Poison { ty } => cir_type_mentions_f128(ty),
        Attr::Dialect { ty: Some(ty), .. } => cir_type_mentions_f128(ty),
        Attr::BitfieldInfo { storage_type, .. } => cir_type_mentions_f128(storage_type),
        Attr::ConstComplex { real, imag, .. } => {
            attr_mentions_f128(real) || attr_mentions_f128(imag)
        }
        Attr::Array(items) => items.iter().any(attr_mentions_f128),
        Attr::Dict(entries) => entries.iter().any(|(_, v)| attr_mentions_f128(v)),
        _ => false,
    }
}

pub fn required_features(module: &Module) -> BTreeSet<Feature> {
    let mut features = BTreeSet::new();
    for op in module_ops(module) {
        if op.results.iter().any(|(_, ty)| cir_type_mentions_f128(ty))
            || op
                .properties
                .iter()
                .chain(op.attributes.iter())
                .any(|(_, attr)| attr_mentions_f128(attr))
        {
            features.insert(Feature::F128);
        }
        if linkage_is_weak(op) && attr_symbol_ref(op, "aliasee").is_none() {
            features.insert(Feature::Linkage);
        }
        if op.kind() == CirOpKind::Asm
            && !op.results.is_empty()
            && attr_str(op, "asm_string").is_some_and(asm_template_has_labels)
        {
            features.insert(Feature::AsmGotoWithOutputs);
        }
        if op.kind() == CirOpKind::Func {
            if let Some(function_type) = attr_type(op, "function_type")
                && (function_type_is_variadic(function_type)
                    || function_type_contains_va_list(function_type))
            {
                features.insert(Feature::CVariadic);
            }
        } else if op.kind() == CirOpKind::Global
            && matches!(attr_str(op, "sym_name"), Some("llvm.used"))
        {
            features.insert(Feature::UsedWithArg);
        }
    }
    features
}

pub fn lower(cir: &Module, c: &Unit, ctx: &mut Ctx) -> Program {
    lower_with_project(cir, c, ctx, &ProjectInfo::default())
}

pub fn lower_with_project(cir: &Module, c: &Unit, ctx: &mut Ctx, project: &ProjectInfo) -> Program {
    let mut anon_records = anon_local_records(cir);
    let shim_records = shim_records_for_module(cir, c);
    let cir_record_names: BTreeSet<String> = cir
        .type_aliases
        .values()
        .filter_map(|ty| cir_record_name(ty))
        .map(|name| sanitize_ident(name).into_string())
        .collect();
    let mut anon_record_names: BTreeSet<String> = anon_records
        .iter()
        .map(|record| sanitize_ident(&record.name).into_string())
        .collect();
    for record in &c.anonymous_header_records {
        let name = sanitize_ident(&record.name).into_string();
        if cir_record_names.contains(&name)
            && clib_record_type(&record.name).is_none()
            && anon_record_names.insert(name)
        {
            anon_records.push(record.clone());
        }
    }
    for record in &shim_records {
        let name = sanitize_ident(&record.name).into_string();
        if !project.shared_records.contains(&name) && anon_record_names.insert(name) {
            anon_records.push(record.clone());
        }
    }
    let mut records: BTreeMap<String, crate::frontend::c_ast::Record> = c
        .records
        .iter()
        .map(|record| (sanitize_ident(&record.name).into_string(), record.clone()))
        .collect();
    let local_collisions = resolve_local_record_collisions(cir, &c.records);
    records.extend(local_collisions);
    let enums: BTreeMap<String, crate::frontend::c_ast::Enum> = c
        .enums
        .iter()
        .map(|enm| (sanitize_ident(&enm.name).into_string(), enm.clone()))
        .collect();
    for record in &anon_records {
        let name = sanitize_ident(&record.name).into_string();
        if record
            .fields
            .iter()
            .any(|field| field.name.starts_with("__bitfield_"))
        {
            records.insert(name, record.clone());
        } else {
            records.entry(name).or_insert_with(|| record.clone());
        }
    }
    for record in shim_records {
        records
            .entry(sanitize_ident(&record.name).into_string())
            .or_insert(record);
    }
    for record in reconcile_anonymous_member_types(cir, &mut records, &c.anonymous_header_records) {
        let name = sanitize_ident(&record.name).into_string();
        if anon_record_names.insert(name) {
            anon_records.push(record);
        }
    }
    widen_flexible_array_members(cir, &mut records);
    let record_defs = required_record_defs(cir, c, &mut records, &project.shared_records);
    let global_rust_names = allocate_global_rust_names(cir, &records, &enums);
    let mut declaration_comments: BTreeMap<(String, String), Vec<Comment>> = BTreeMap::new();
    for declaration in &c.declaration_comments {
        let Some(name) = &declaration.name else {
            continue;
        };
        let name = sanitize_ident(name).into_string();
        let comment = Comment {
            lines: declaration.lines.clone(),
        };
        if declaration.kind != "FunctionDecl"
            && let Some(function) = &declaration.function
        {
            declaration_comments
                .entry((
                    "FunctionDecl".into(),
                    sanitize_ident(function).into_string(),
                ))
                .or_default()
                .push(comment);
        } else {
            declaration_comments
                .entry((declaration.kind.clone(), name))
                .or_default()
                .push(comment);
        }
    }
    let mut lowerer = Lowerer {
        ctx,
        aliases: cir.type_aliases.clone(),
        loc_aliases: cir
            .loc_aliases
            .iter()
            .filter_map(|(name, loc)| Some((name.clone(), source_location_text(loc)?)))
            .collect(),
        attr_aliases: cir.attr_aliases.clone(),
        call_bindings: c.call_bindings(),
        known_functions: c
            .call_bindings()
            .values()
            .filter_map(|binding| binding.known())
            .map(|known| (known.symbol().to_string(), FunctionIdentity::Known(known)))
            .collect(),
        function_types: c
            .function_types
            .iter()
            .map(|(name, ty)| (name.clone(), ty.clone()))
            .collect(),
        weak_refs: c
            .weak_refs
            .iter()
            .map(|attribute| (attribute.name.clone(), attribute.target.clone()))
            .collect(),
        naked_functions: c.naked_functions.clone(),
        external_weak_targets: BTreeSet::new(),
        weak_aliases: BTreeMap::new(),
        records,
        record_defs,
        enums,
        declaration_comments,
        global_rust_names,
        globals: BTreeMap::new(),
        extern_globals: BTreeMap::new(),
        strings: BTreeMap::new(),
        const_arrays: BTreeMap::new(),
        block_addr_globals: BTreeMap::new(),
        const_aggregates: BTreeMap::new(),
        const_zero_globals: BTreeSet::new(),
        used_symbols: BTreeMap::new(),
        externs: BTreeMap::new(),
        extern_returns: BTreeMap::new(),
        long_double_shims: BTreeMap::new(),
        uses_long_double: std::cell::Cell::new(false),
        uses_complex: std::cell::Cell::new(false),
        uses_f128: std::cell::Cell::new(false),
        uses_c_variadic: std::cell::Cell::new(false),
        uses_linkage: std::cell::Cell::new(false),
        uses_thread_local: std::cell::Cell::new(false),
        uses_used_with_arg: std::cell::Cell::new(false),
        uses_asm_goto_outputs: std::cell::Cell::new(false),
        uses_breakpoint: std::cell::Cell::new(false),
        uses_memchr: std::cell::Cell::new(false),
        target_arch: cir
            .ops
            .iter()
            .find_map(|op| attr_str(op, "cir.triple"))
            .and_then(rust_target_arch)
            .map(str::to_string),
        variadic_defs: BTreeSet::new(),
        boxed_variadic_defs: BTreeSet::new(),
        va_list_boxed: false,
        c_abi_functions: BTreeSet::new(),
        project: project.clone(),
        unsafe_functions: project.unsafe_functions.clone(),
        cross_uses: Vec::new(),
        ctor_calls: Vec::new(),
        dtor_calls: Vec::new(),
        generated_alloca_frames: Vec::new(),
        layout_queries: c
            .functions
            .iter()
            .map(|function| (function.name.clone(), function.layout_queries.clone()))
            .collect(),
        macro_consts: c
            .functions
            .iter()
            .map(|function| (function.name.clone(), function.macro_consts.clone()))
            .collect(),
        floating_literals: c.floating_literals.clone(),
        global_floating_literals: c.global_floating_literals.clone(),
        long_double_callback_trampolines: BTreeMap::new(),
        enum_consts: c
            .functions
            .iter()
            .map(|function| (function.name.clone(), function.enum_consts.clone()))
            .collect(),
        local_enum_decls: c
            .functions
            .iter()
            .map(|function| (function.name.clone(), function.local_enum_decls.clone()))
            .collect(),
        asm_gotos: c
            .functions
            .iter()
            .map(|function| (function.name.clone(), function.asm_gotos.clone()))
            .collect(),
        function_return_types: BTreeMap::new(),
        function_param_types: BTreeMap::new(),
        enum_wrapper_fns: std::cell::RefCell::new(BTreeMap::new()),
        needed_enum_from_impls: std::cell::RefCell::new(BTreeSet::new()),
        bitfield_storages: collect_bitfield_storages(cir),
        global_sym_types: BTreeMap::new(),
    };
    lowerer.lower_module(cir, c)
}

pub fn lower_shared_types(
    records: &[crate::frontend::c_ast::Record],
    enums: &[crate::frontend::c_ast::Enum],
) -> Program {
    let mut items = vec![Item::CrateAttrs(vec![CrateAttr::Allow(vec![
        Lint::DeadCode,
        Lint::Unused,
        Lint::NonCamelCaseTypes,
        Lint::NonSnakeCase,
        Lint::NonUpperCaseGlobals,
        Lint::SuspiciousRuntimeSymbolDefinitions,
        Lint::UnpredictableFunctionPointerComparisons,
        Lint::UnusedComparisons,
    ])])];
    if shared_types_use_long_double(records) {
        items.extend(long_double_prelude(Visibility::Pub));
        items.push(Item::ExternBlock {
            abi: "C".into(),
            decls: f80_shim_decls().into_iter().map(ExternDecl::Fn).collect(),
        });
    }
    items.extend(
        enums
            .iter()
            .filter_map(|enm| lower_enum_def(enm, Visibility::Pub).map(Item::Enum)),
    );
    items.extend(records.iter().flat_map(|record| {
        lower_record_def(record, Visibility::Pub, Visibility::Pub, true, false)
    }));
    Program { items }
}

pub fn long_double_f80_extern_block() -> Item {
    Item::ExternBlock {
        abi: "C".into(),
        decls: f80_shim_decls().into_iter().map(ExternDecl::Fn).collect(),
    }
}

pub fn shared_types_use_long_double(records: &[crate::frontend::c_ast::Record]) -> bool {
    records
        .iter()
        .flat_map(|record| &record.fields)
        .any(|field| ctype_uses_long_double(&field.ty))
}

fn lower_enum_def(enm: &crate::frontend::c_ast::Enum, vis: Visibility) -> Option<EnumDef> {
    if enm.variants.is_empty() {
        return None;
    }
    Some(EnumDef {
        comments: comments(&enm.comments),
        attrs: enum_attrs(),
        vis,
        name: sanitize_ident(&enm.name).into_string(),
        variants: enm
            .variants
            .iter()
            .map(|variant| EnumConst {
                comments: comments(&variant.comments),
                name: sanitize_ident(&variant.name).into_string(),
                value: variant.value,
            })
            .collect(),
    })
}

fn enum_attrs() -> Vec<RustAttr> {
    vec![
        RustAttr::Repr(vec![Repr::C]),
        RustAttr::Allow(vec![Lint::NonCamelCaseTypes]),
        RustAttr::Derive(vec![
            Derive::Clone,
            Derive::Copy,
            Derive::PartialEq,
            Derive::Eq,
            Derive::Debug,
            Derive::Hash,
        ]),
    ]
}

fn aligned_type(ty: Type, alignment: u32) -> Type {
    Type::Generic {
        name: "aligned::Aligned".into(),
        args: vec![Type::Custom(format!("aligned::A{alignment}")), ty],
    }
}

fn aligned_value(value: Expr, _alignment: u32) -> Expr {
    Expr::TupleStructLit {
        name: "aligned::Aligned".into(),
        fields: vec![value],
    }
}

fn type_alignment(ty: &Type) -> u32 {
    match ty {
        Type::Prim(Prim::Bool | Prim::I8 | Prim::U8) => 1,
        Type::Prim(Prim::I16 | Prim::U16) => 2,
        Type::Prim(Prim::I32 | Prim::U32 | Prim::F32) => 4,
        Type::Prim(Prim::I64 | Prim::U64 | Prim::Isize | Prim::Usize | Prim::F64)
        | Type::Ptr { .. }
        | Type::FnPtr { .. }
        | Type::Ref { .. }
        | Type::VaList => 8,
        Type::CLib(ty) => ty.alignment(),
        Type::Prim(Prim::I128 | Prim::U128 | Prim::F128) | Type::LongDouble => 16,
        Type::Array { elem, .. } => type_alignment(elem),
        Type::Complex(inner) => type_alignment(inner),
        _ => 1,
    }
}

fn effective_type_alignment(
    ty: &Type,
    records: &BTreeMap<String, crate::frontend::c_ast::Record>,
) -> u32 {
    if let Type::Custom(name) = ty
        && let Some(record) = records.get(name)
        && let Some(align) = record_natural_align(record, records)
    {
        return align as u32;
    }
    type_alignment(ty)
}

fn lower_record_def(
    record: &crate::frontend::c_ast::Record,
    vis: Visibility,
    field_vis: Visibility,
    allow_empty: bool,
    va_list_boxed: bool,
) -> Vec<Item> {
    if record.fields.is_empty() && !allow_empty {
        return Vec::new();
    }
    let is_union = record.kind == RecordKind::Union;
    let fields: Vec<RecordField> = record
        .fields
        .iter()
        .map(|field| RecordField {
            comments: comments(&field.comments),
            name: sanitize_ident(&field.name),
            ty: c_record_field_type(&field.ty, va_list_boxed),
        })
        .collect();
    let name = sanitize_ident(&record.name).into_string();

    if let Some(packed) = record.packed
        && let Some(align) = record.align
    {
        return lower_packed_aligned_wrapper(
            &name, fields, is_union, vis, field_vis, packed, align,
        );
    }

    vec![Item::Record(RecordDef {
        comments: comments(&record.comments),
        vis,
        field_vis,
        is_union,
        allow_non_camel_case: name == "__once_flag",
        name,
        fields,
        packed: record.packed,
        align: record.align,
    })]
}

fn packed_aligned_inner_name(name: &str) -> String {
    format!("{name}__packed")
}

fn lower_packed_aligned_wrapper(
    name: &str,
    fields: Vec<RecordField>,
    is_union: bool,
    vis: Visibility,
    field_vis: Visibility,
    packed: u32,
    align: u32,
) -> Vec<Item> {
    let inner_name = packed_aligned_inner_name(name);
    let inner = Item::Record(RecordDef {
        comments: Vec::new(),
        vis,
        field_vis,
        is_union,
        allow_non_camel_case: false,
        name: inner_name.clone(),
        fields,
        packed: Some(packed),
        align: None,
    });
    let outer = Item::Struct(StructDef {
        attrs: vec![
            RustAttr::Repr(vec![Repr::C, Repr::Align(align)]),
            RustAttr::Derive(vec![Derive::Clone, Derive::Copy]),
        ],
        vis,
        field_vis,
        generics: vec![],
        name: name.to_string(),
        fields: StructFields::Tuple(vec![Type::Custom(inner_name.clone())]),
    });
    let self_field = |mutable| Expr::Ref {
        mutable,
        expr: Box::new(Expr::Field {
            base: Box::new(Expr::Var("self".into())),
            field: "0".into(),
        }),
    };
    let deref = Item::Impl(ImplBlock {
        generics: vec![],
        trait_: Some(TraitRef::Std(StdTrait::Deref)),
        self_ty: Type::Custom(name.to_string()),
        items: vec![
            ImplItem::AssocType {
                name: "Target".into(),
                ty: Type::Custom(inner_name.clone()),
            },
            ImplItem::Method(Method {
                name: "deref".into(),
                self_kind: SelfKind::Ref,
                params: vec![],
                ret: Some(Type::Ref {
                    mutable: false,
                    inner: Box::new(Type::Custom(inner_name.clone())),
                }),
                body: self_field(false),
            }),
        ],
    });
    let deref_mut = Item::Impl(ImplBlock {
        generics: vec![],
        trait_: Some(TraitRef::Std(StdTrait::DerefMut)),
        self_ty: Type::Custom(name.to_string()),
        items: vec![ImplItem::Method(Method {
            name: "deref_mut".into(),
            self_kind: SelfKind::RefMut,
            params: vec![],
            ret: Some(Type::Ref {
                mutable: true,
                inner: Box::new(Type::Custom(inner_name)),
            }),
            body: self_field(true),
        })],
    });
    vec![inner, outer, deref, deref_mut]
}

fn record_lit_name(record: &crate::frontend::c_ast::Record) -> String {
    let name = sanitize_ident(&record.name).into_string();
    if record.packed.is_some() && record.align.is_some() {
        packed_aligned_inner_name(&name)
    } else {
        name
    }
}

fn wrap_record_lit(record: &crate::frontend::c_ast::Record, lit: Expr) -> Expr {
    if record.packed.is_some() && record.align.is_some() {
        Expr::TupleStructLit {
            name: sanitize_ident(&record.name).into_string(),
            fields: vec![lit],
        }
    } else {
        lit
    }
}

fn comments(lines: &[String]) -> Vec<crate::backend::rust_ast::Comment> {
    if lines.is_empty() {
        Vec::new()
    } else {
        vec![crate::backend::rust_ast::Comment {
            lines: lines.to_vec(),
        }]
    }
}

struct Lowerer<'a> {
    ctx: &'a mut Ctx,
    aliases: BTreeMap<String, CirType>,
    loc_aliases: BTreeMap<String, String>,
    attr_aliases: BTreeMap<String, Attr>,
    call_bindings: HashMap<Loc, CallBinding>,
    known_functions: BTreeMap<String, FunctionIdentity>,
    function_types: BTreeMap<String, String>,
    weak_refs: BTreeMap<String, String>,
    naked_functions: BTreeSet<String>,
    external_weak_targets: BTreeSet<String>,
    weak_aliases: BTreeMap<String, String>,
    records: BTreeMap<String, crate::frontend::c_ast::Record>,
    record_defs: BTreeSet<String>,
    enums: BTreeMap<String, crate::frontend::c_ast::Enum>,
    declaration_comments: BTreeMap<(String, String), Vec<Comment>>,
    global_rust_names: BTreeMap<String, String>,
    globals: BTreeMap<String, GlobalVar>,
    extern_globals: BTreeMap<String, ExternGlobal>,
    strings: BTreeMap<String, Vec<u8>>,
    const_arrays: BTreeMap<String, Vec<Expr>>,
    block_addr_globals: BTreeMap<String, Vec<String>>,
    const_aggregates: BTreeMap<String, Attr>,
    const_zero_globals: BTreeSet<String>,
    used_symbols: BTreeMap<String, Vec<UsedKind>>,
    externs: BTreeMap<String, Vec<Type>>,
    extern_returns: BTreeMap<String, Option<String>>,
    long_double_shims: BTreeMap<String, ExternFnDecl>,
    uses_long_double: std::cell::Cell<bool>,
    uses_complex: std::cell::Cell<bool>,
    uses_f128: std::cell::Cell<bool>,
    uses_c_variadic: std::cell::Cell<bool>,
    uses_linkage: std::cell::Cell<bool>,
    uses_thread_local: std::cell::Cell<bool>,
    uses_used_with_arg: std::cell::Cell<bool>,
    uses_asm_goto_outputs: std::cell::Cell<bool>,
    uses_breakpoint: std::cell::Cell<bool>,
    uses_memchr: std::cell::Cell<bool>,
    target_arch: Option<String>,
    variadic_defs: BTreeSet<String>,
    boxed_variadic_defs: BTreeSet<String>,
    va_list_boxed: bool,
    c_abi_functions: BTreeSet<String>,
    project: ProjectInfo,
    unsafe_functions: BTreeSet<String>,
    cross_uses: Vec<Item>,
    ctor_calls: Vec<String>,
    dtor_calls: Vec<String>,
    generated_alloca_frames: Vec<StructDef>,
    layout_queries: BTreeMap<String, Vec<LayoutQuery>>,
    macro_consts: BTreeMap<String, Vec<MacroConst>>,
    floating_literals: HashMap<FloatingLiteralLoc, FloatingLiteralFact>,
    global_floating_literals: HashMap<String, Vec<FloatingLiteralFact>>,
    long_double_callback_trampolines: BTreeMap<String, String>,
    enum_consts: BTreeMap<String, Vec<EnumConstRef>>,
    local_enum_decls: BTreeMap<String, Vec<crate::frontend::c_ast::LocalEnumDecl>>,
    asm_gotos: BTreeMap<String, Vec<crate::frontend::c_ast::AsmGoto>>,
    function_return_types: BTreeMap<String, Type>,
    function_param_types: BTreeMap<String, Vec<Type>>,
    enum_wrapper_fns: std::cell::RefCell<BTreeMap<String, FnDef>>,
    needed_enum_from_impls: std::cell::RefCell<BTreeSet<(String, Type)>>,
    bitfield_storages: BitfieldStorages,
    global_sym_types: BTreeMap<String, CirType>,
}

struct FunctionLowerer<'a, 'b> {
    parent: &'a mut Lowerer<'b>,
    values: BTreeMap<String, Val>,
    const_int_values: BTreeMap<String, i128>,
    function_pointer_null_values: BTreeSet<String>,
    slots: BTreeMap<String, String>,
    slot_places: BTreeMap<String, Expr>,
    aligned_slots: BTreeSet<String>,
    slot_types: BTreeMap<String, Type>,
    member_ptrs: BTreeMap<String, MemberPtr>,
    element_ptrs: BTreeMap<String, ElementPtr>,
    block_addr_element_ptrs: BTreeMap<String, BlockAddrElementPtr>,
    indirect_target_values: BTreeMap<String, Expr>,
    temp_counter: usize,
    indent: usize,
    body: Vec<IndentStmt>,
    is_main: bool,
    loop_stack: Vec<LoopFrame>,
    label_counter: usize,
    dispatch: Option<DispatchCtx>,
    hoisting_allocas: bool,
    hoisted: BTreeSet<String>,
    declared_local_names: BTreeSet<String>,
    forward_allocas: BTreeSet<String>,
    forward_values: BTreeMap<String, Expr>,
    immutable_temps: BTreeSet<String>,
    va_allocas: BTreeSet<String>,
    va_places: BTreeMap<String, Expr>,
    va_args_param: Option<String>,
    layout_queries: VecDeque<LayoutQuery>,
    macro_consts: VecDeque<MacroConst>,
    enum_consts: VecDeque<EnumConstRef>,
    macro_arith_values: BTreeMap<String, i128>,
    asm_outputs: BTreeMap<String, Vec<Expr>>,
    asm_gotos: VecDeque<crate::frontend::c_ast::AsmGoto>,
    asm_output_places: BTreeMap<String, Expr>,
    local_enum_types: BTreeMap<String, String>,
    loaded_field_types: BTreeMap<String, Type>,
    load_ptr_operand: BTreeMap<String, String>,
    member_base_operand: BTreeMap<String, String>,
    coerce_alloca_real_type: BTreeMap<String, (String, String)>,
}

struct DispatchCtx {
    loop_label: Label,
    state_var: String,
    label_to_state: BTreeMap<String, usize>,
    block_to_state: BTreeMap<String, usize>,
    cross_block_names: BTreeMap<String, String>,
    block_args: BTreeMap<usize, Vec<String>>,
    pending_hoists: Vec<IndentStmt>,
}

struct LoopFrame {
    break_label: Option<Label>,
    continue_label: Option<Label>,
    is_loop: bool,
}

struct SwitchCase<'a> {
    patterns: Vec<Pattern>,
    is_default: bool,
    region: &'a Region,
}

struct DuffSwitch<'a> {
    cases: Vec<SwitchCase<'a>>,
    prefix: Vec<&'a Op>,
    condition: &'a Region,
}

#[derive(Debug, Clone)]
struct MemberPtr {
    base: Expr,
    field: String,
    field_ty: Option<Type>,
    unsafe_access: bool,
    bitfield_name: Option<String>,
    field_is_trailing: bool,
}

#[derive(Debug, Clone)]
struct ElementPtr {
    base: Expr,
    index: Expr,
    unsafe_access: bool,
    unbounded: bool,
    out_of_bounds: bool,
    elem_ty: Option<Type>,
}

#[derive(Debug, Clone)]
struct BlockAddrElementPtr {
    labels: Vec<String>,
    index: Expr,
}

#[derive(Debug, Clone)]
struct GlobalVar {
    source_name: String,
    name: String,
    ty: Type,
    init: Expr,
    alignment: Option<u32>,
    thread_local: bool,
    external: bool,
    weak: bool,
    section: Option<String>,
    used: Vec<UsedKind>,
}

#[derive(Debug, Clone)]
struct ExternGlobal {
    source_name: String,
    ty: Type,
    thread_local: bool,
}

#[derive(Debug, Clone)]
enum Val {
    Expr(Expr),
    Global(String),
}

impl Val {
    fn to_expr(&self, strings: &BTreeMap<String, Vec<u8>>) -> Expr {
        match self {
            Val::Expr(e) => e.clone(),
            Val::Global(name) => match strings.get(name) {
                Some(bytes) => Expr::Cast {
                    expr: Box::new(Expr::MethodCall {
                        recv: Box::new(Expr::ByteStr(bytes.clone())),
                        method: "as_ptr".into(),
                        args: Vec::new(),
                    }),
                    ty: Type::parse("*mut libc::c_char"),
                },
                None => Expr::Var(name.clone().into()),
            },
        }
    }
}

impl<'a> Lowerer<'a> {
    fn rust_global_name(&self, name: &str) -> String {
        self.global_rust_names
            .get(name)
            .cloned()
            .unwrap_or_else(|| sanitize_ident(name).into_string())
    }

    fn ast_floating_literal(&self, op: &Op) -> Option<FloatingLiteralFact> {
        let loc = op
            .loc
            .as_ref()
            .and_then(|raw| self.resolve_source_floating_literal_loc(raw))?;
        self.floating_literals.get(&loc).cloned()
    }

    fn declaration_comment_items(&self, kind: &str, name: &str) -> Vec<Item> {
        self.declaration_comments
            .get(&(kind.to_string(), sanitize_ident(name).into_string()))
            .into_iter()
            .flatten()
            .cloned()
            .map(Item::Comment)
            .collect()
    }

    fn cross_module_path(&self, module: &str, name: &str) -> Path {
        let root = self
            .project
            .cross_module_crate
            .as_deref()
            .unwrap_or("crate");
        if module.is_empty() {
            Path::new([Ident::from(root), Ident::from(name)])
        } else {
            Path::new([Ident::from(root), Ident::from(module), Ident::from(name)])
        }
    }

    fn call_binding(&self, op: &Op, direct: bool) -> CallBinding {
        if !direct {
            return CallBinding::Indirect;
        }
        op.loc
            .as_ref()
            .and_then(|raw| self.resolve_source_loc(raw))
            .and_then(|loc| self.call_bindings.get(&loc).cloned())
            .unwrap_or_else(|| CallBinding::direct_unknown(None))
    }

    fn resolve_loc(&self, raw: &str) -> Option<Loc> {
        let point = self.resolve_expansion_source_point(raw, 0)?;
        Some(Loc {
            line: point.line,
            col: point.col,
        })
    }

    fn resolve_source_loc(&self, raw: &SourceLocation) -> Option<Loc> {
        match raw {
            SourceLocation::File { line, column, .. } => Some(Loc {
                line: *line,
                col: *column,
            }),
            _ => None,
        }
    }

    fn resolve_source_point_value(&self, raw: &SourceLocation) -> Option<SourcePoint> {
        match raw {
            SourceLocation::File { file, line, column } => Some(SourcePoint {
                file: file.clone(),
                line: *line,
                col: *column,
            }),
            _ => None,
        }
    }

    fn resolve_source_floating_literal_loc(
        &self,
        raw: &SourceLocation,
    ) -> Option<FloatingLiteralLoc> {
        let point = self.resolve_source_point_value(raw)?;
        Some(FloatingLiteralLoc {
            spelling: point.clone(),
            expansion: point,
        })
    }

    fn resolve_floating_literal_loc(&self, raw: &str) -> Option<FloatingLiteralLoc> {
        let raw = raw.trim();
        if raw.starts_with('#') {
            return self.resolve_floating_literal_loc(
                self.loc_aliases.get(raw.strip_prefix('#').unwrap_or(raw))?,
            );
        }
        let inner = raw
            .strip_prefix("loc(")
            .and_then(|raw| raw.strip_suffix(')'))
            .unwrap_or(raw)
            .trim();
        if inner.starts_with('#') {
            return self.resolve_floating_literal_loc(
                self.loc_aliases
                    .get(inner.strip_prefix('#').unwrap_or(inner))?,
            );
        }
        if let Some(callsite) = inner
            .strip_prefix("callsite(")
            .and_then(|raw| raw.strip_suffix(')'))
            && let Some((spelling, expansion)) = callsite.split_once(" at ")
        {
            return Some(FloatingLiteralLoc {
                spelling: self.resolve_source_point(spelling, 0)?,
                expansion: self.resolve_source_point(expansion, 0)?,
            });
        }
        let point = self.resolve_source_point(inner, 0)?;
        Some(FloatingLiteralLoc {
            spelling: point.clone(),
            expansion: point,
        })
    }

    fn resolve_expansion_source_point(&self, raw: &str, depth: usize) -> Option<SourcePoint> {
        if depth == 8 {
            return None;
        }
        let raw = raw.trim();
        if raw.starts_with('#') {
            return self.resolve_expansion_source_point(
                self.loc_aliases.get(raw.strip_prefix('#').unwrap_or(raw))?,
                depth + 1,
            );
        }
        let inner = raw
            .strip_prefix("loc(")
            .and_then(|raw| raw.strip_suffix(')'))
            .unwrap_or(raw)
            .trim();
        if inner.starts_with('#') {
            return self.resolve_expansion_source_point(
                self.loc_aliases
                    .get(inner.strip_prefix('#').unwrap_or(inner))?,
                depth + 1,
            );
        }
        if let Some(callsite) = inner
            .strip_prefix("callsite(")
            .and_then(|raw| raw.strip_suffix(')'))
            && let Some((_, expansion)) = callsite.split_once(" at ")
        {
            return self.resolve_expansion_source_point(expansion, depth + 1);
        }
        self.resolve_source_point(inner, depth + 1)
    }

    fn resolve_macro_group_loc(&self, raw: &str, depth: usize) -> Option<SourcePoint> {
        if depth == 8 {
            return None;
        }
        let raw = raw.trim();
        if raw.starts_with('#') {
            return self.resolve_macro_group_loc(
                self.loc_aliases.get(raw.strip_prefix('#').unwrap_or(raw))?,
                depth + 1,
            );
        }
        let inner = raw
            .strip_prefix("loc(")
            .and_then(|raw| raw.strip_suffix(')'))
            .unwrap_or(raw)
            .trim();
        if inner.starts_with('#') {
            return self.resolve_macro_group_loc(
                self.loc_aliases
                    .get(inner.strip_prefix('#').unwrap_or(inner))?,
                depth + 1,
            );
        }
        if let Some(fused) = inner
            .strip_prefix("fused[")
            .and_then(|raw| raw.strip_suffix(']'))
            && let Some(first) = fused.split(',').next()
        {
            return self.resolve_macro_group_loc(first, depth + 1);
        }
        if let Some(callsite) = inner
            .strip_prefix("callsite(")
            .and_then(|raw| raw.strip_suffix(')'))
            && let Some((_, expansion)) = callsite.split_once(" at ")
        {
            return self.resolve_source_point(expansion, depth + 1);
        }
        self.resolve_source_point(inner, depth + 1)
    }

    fn resolve_source_point(&self, raw: &str, depth: usize) -> Option<SourcePoint> {
        if depth == 8 {
            return None;
        }
        let raw = raw.trim();
        if raw.starts_with('#') {
            return self.resolve_source_point(
                self.loc_aliases.get(raw.strip_prefix('#').unwrap_or(raw))?,
                depth + 1,
            );
        }
        let inner = raw
            .strip_prefix("loc(")
            .and_then(|raw| raw.strip_suffix(')'))
            .unwrap_or(raw)
            .trim();
        if inner.starts_with('#') {
            return self.resolve_source_point(
                self.loc_aliases
                    .get(inner.strip_prefix('#').unwrap_or(inner))?,
                depth + 1,
            );
        }
        let (file, line_col) = inner.rsplit_once("\":")?;
        let (line, col) = line_col.split_once(':')?;
        Some(SourcePoint {
            file: file.strip_prefix('"')?.to_string(),
            line: line.parse().ok()?,
            col: col.parse().ok()?,
        })
    }

    fn long_double_extern_pointer_shim(&mut self, name: &str, ty: &Type) -> Option<String> {
        if !self.externs.contains_key(name) {
            return None;
        }
        let Type::FnPtr { params, ret, .. } = ty else {
            return None;
        };
        if !params.iter().any(type_mentions_long_double) && !type_mentions_long_double(ret) {
            return None;
        }
        let mut segments = vec![format!("r{}", long_double_shim_type_tag(ret))];
        segments.extend(params.iter().map(long_double_shim_type_tag));
        let shim_name = format!("__slate_{name}__{}", segments.join("_"));
        let ret = ret.as_ref().clone();
        self.long_double_shims
            .entry(shim_name.clone())
            .or_insert_with(|| ExternFnDecl {
                identity: self
                    .known_functions
                    .get(name)
                    .copied()
                    .unwrap_or(FunctionIdentity::Unknown),
                name: shim_name.clone(),
                declared_type: self.function_types.get(name).cloned(),
                params: params
                    .iter()
                    .enumerate()
                    .map(|(i, ty)| FnParam {
                        name: format!("_{i}"),
                        mutable: false,
                        ty: ty.clone(),
                    })
                    .collect(),
                variadic: false,
                ret: (!ret.is_unit()).then_some(ret),
                safe: false,
            });
        Some(shim_name)
    }

    fn lower_module(&mut self, module: &Module, c: &Unit) -> Program {
        let mut items = vec![Item::CrateAttrs(vec![CrateAttr::Allow(vec![
            Lint::DeadCode,
            Lint::Unused,
            Lint::NonCamelCaseTypes,
            Lint::NonSnakeCase,
            Lint::NonUpperCaseGlobals,
            Lint::ArithmeticOverflow,
            Lint::SuspiciousRuntimeSymbolDefinitions,
            Lint::UnpredictableFunctionPointerComparisons,
            Lint::UnusedComparisons,
        ])])];

        if let Some(module_op) = builtin_module(module) {
            self.c_abi_functions = c_abi_function_targets(module_op);
            self.c_abi_functions
                .extend(self.project.address_taken_functions.iter().cloned());
            self.va_list_boxed = !module_requires_native_va_list(
                module_op,
                &self.c_abi_functions,
                self.project.emit_pub,
                &self.aliases,
            );
        }

        items.extend(bitfield_items(&self.bitfield_storages));

        for enm in &c.enums {
            let name = sanitize_ident(&enm.name).into_string();
            if self.project.shared_enums.contains(&name) {
                continue;
            }
            if let Some(item) = self.lower_enum(enm) {
                items.push(item);
            }
        }
        for name in self.record_defs.clone() {
            if self.project.shared_records.contains(&name) {
                continue;
            }
            if let Some(record) = self.records.get(&name).cloned() {
                items.extend(self.lower_record(&record));
            }
        }
        items.extend(self.standard_record_defs());

        let Some(module_op) = builtin_module(module) else {
            self.ctx.diagnostics.error("lower: no builtin.module op");
            return Program { items };
        };

        let ops = region_ops(module_op);
        self.weak_aliases = ops
            .iter()
            .filter(|op| op.kind() == CirOpKind::Func && linkage_is_weak(op))
            .filter_map(|op| {
                Some((
                    attr_str(op, "sym_name")?.to_string(),
                    attr_symbol_ref(op, "aliasee")?.to_string(),
                ))
            })
            .collect();
        self.external_weak_targets = self
            .weak_refs
            .values()
            .filter(|target| {
                !ops.iter().any(|op| {
                    op.kind() == CirOpKind::Func
                        && attr_str(op, "sym_name") == Some(target.as_str())
                })
            })
            .cloned()
            .collect();
        self.function_return_types =
            declared_function_return_types(module_op, &self.aliases, self.va_list_boxed);
        self.function_param_types =
            declared_function_param_types(module_op, &self.aliases, self.va_list_boxed);
        let mut assembly_strings = Vec::new();
        collect_assembly_strings(module_op, &mut assembly_strings);
        let asm_referenced_globals: BTreeSet<String> = ops
            .iter()
            .filter(|op| op.kind() == CirOpKind::Global)
            .filter_map(|op| attr_str(op, "sym_name"))
            .filter(|name| {
                assembly_strings
                    .iter()
                    .any(|assembly| assembly_mentions_symbol(assembly, name))
            })
            .map(|name| sanitize_ident(name).into_string())
            .collect();
        items.extend(lower_module_asm(module_op, &mut self.ctx.diagnostics));
        items.extend(lower_weak_alias_asm(
            module_op,
            &self.weak_aliases,
            &mut self.ctx.diagnostics,
        ));
        let has_main = ops.iter().any(|op| {
            op.kind() == CirOpKind::Func
                && attr_str(op, "sym_name") == Some("main")
                && !region_ops(op).is_empty()
        });
        let hooks = collect_lifecycle_hooks(&ops, has_main, &mut self.ctx.diagnostics);
        self.ctor_calls = hooks.ctors;
        self.dtor_calls = hooks.dtors;
        self.used_symbols = collect_used_symbols(&ops);
        self.global_sym_types = ops
            .iter()
            .filter(|op| op.kind() == CirOpKind::Global)
            .filter_map(|op| {
                Some((
                    attr_str(op, "sym_name")?.to_string(),
                    attr_type(op, "sym_type")?.clone(),
                ))
            })
            .collect();
        for op in ops.iter().filter(|op| {
            op.kind() == CirOpKind::Global
                && attr_str(op, "sym_name").is_some_and(|name| name.starts_with(".str"))
        }) {
            self.collect_global(op);
        }
        for op in ops.iter().filter(|op| {
            op.kind() == CirOpKind::Global
                && !attr_str(op, "sym_name").is_some_and(|name| name.starts_with(".str"))
        }) {
            self.collect_global(op);
        }
        for global in self.globals.values() {
            let global_external_def = self.project.emit_pub && global.external;
            let global_vis = if global_external_def {
                Visibility::Pub
            } else {
                Visibility::Private
            };
            if global
                .used
                .iter()
                .any(|kind| !matches!(kind, UsedKind::Plain))
            {
                self.uses_used_with_arg.set(true);
            }
            if global.weak {
                self.uses_linkage.set(true);
            }
            let ty = global
                .alignment
                .map(|alignment| aligned_type(global.ty.clone(), alignment))
                .unwrap_or_else(|| global.ty.clone());
            let init = global
                .alignment
                .map(|alignment| aligned_value(global.init.clone(), alignment))
                .unwrap_or_else(|| global.init.clone());
            let mut attrs = symbol_attrs(
                global_external_def || asm_referenced_globals.contains(&global.name),
                global.weak,
                global.section.as_deref(),
                &global.used,
            );
            if global.name != global.source_name
                && let Some(no_mangle) = attrs
                    .iter()
                    .position(|attr| matches!(attr, RustAttr::NoMangle))
            {
                attrs[no_mangle] = RustAttr::ExportName(global.source_name.clone());
            }
            if global.thread_local {
                attrs.push(RustAttr::ThreadLocal);
                self.uses_thread_local.set(true);
            }
            items.extend(self.declaration_comment_items("VarDecl", &global.source_name));
            items.push(Item::Static {
                attrs,
                vis: global_vis,
                mutable: true,
                name: global.name.clone(),
                ty,
                init,
            });
        }

        let mut extern_decls = Vec::new();
        for (name, global) in &self.extern_globals {
            if let Some(module) = self.project.cross_module_globals.get(&global.source_name) {
                self.cross_uses.push(Item::Use {
                    path: self.cross_module_path(module, name),
                });
                continue;
            }
            extern_decls.push(ExternDecl::Static {
                attrs: global
                    .thread_local
                    .then_some(RustAttr::ThreadLocal)
                    .into_iter()
                    .chain(
                        (name != &global.source_name)
                            .then(|| RustAttr::LinkName(global.source_name.clone())),
                    )
                    .collect(),
                mutable: true,
                name: name.clone(),
                ty: global.ty.clone(),
            });
        }
        let mut emitted_weak_targets = BTreeSet::new();
        for op in &ops {
            if op.kind() != CirOpKind::Func || !region_ops(op).is_empty() {
                continue;
            }
            if attr_symbol_ref(op, "aliasee").is_some() {
                if linkage_is_weak(op) {
                    let Some(name) = attr_str(op, "sym_name") else {
                        continue;
                    };
                    let function_type = attr_type(op, "function_type").unwrap_or(&CirType::Void);
                    let (decl, params, ret) = self.extern_fn_signature(name, function_type);
                    if decl.variadic {
                        self.uses_c_variadic.set(true);
                    }
                    self.externs.insert(name.to_string(), params);
                    self.extern_returns.insert(name.to_string(), ret);
                    extern_decls.push(ExternDecl::Fn(decl));
                }
                continue;
            }
            let Some(name) = attr_str(op, "sym_name") else {
                continue;
            };
            if let Some(target) = self.weak_refs.get(name).cloned() {
                if self.external_weak_targets.contains(&target)
                    && emitted_weak_targets.insert(target.clone())
                {
                    let function_type = attr_type(op, "function_type").unwrap_or(&CirType::Void);
                    let (decl, params, ret) = self.extern_fn_signature(&target, function_type);
                    if decl.variadic {
                        self.ctx.diagnostics.error(format!(
                            "lower: variadic weakref alias `{name}` to external target `{target}`"
                        ));
                    } else {
                        self.externs.insert(target.clone(), params);
                        self.extern_returns.insert(target.clone(), ret);
                        extern_decls.push(ExternDecl::Static {
                            attrs: vec![RustAttr::ExternWeakLinkage],
                            mutable: false,
                            name: target,
                            ty: Type::FnPtr {
                                abi: Abi::C,
                                params: decl.params.into_iter().map(|param| param.ty).collect(),
                                ret: Box::new(decl.ret.unwrap_or(Type::Unit)),
                            },
                        });
                        self.uses_linkage.set(true);
                    }
                }
                continue;
            }
            if is_complex_runtime_call(name) {
                continue;
            }
            if let Some(module) = self.project.cross_module.get(name) {
                self.cross_uses.push(Item::Use {
                    path: self.cross_module_path(module, name),
                });
                continue;
            }
            let function_type = attr_type(op, "function_type").unwrap_or(&CirType::Void);
            let (mut decl, params, ret) = self.extern_fn_signature(name, function_type);
            if attr_bool(op, "noreturn") {
                decl.ret = Some(Type::Never);
            }
            self.externs.insert(name.to_string(), params);
            self.extern_returns.insert(name.to_string(), ret.clone());
            extern_decls.push(ExternDecl::Fn(decl));
        }
        if !extern_decls.is_empty() {
            items.push(Item::ExternBlock {
                abi: "C".into(),
                decls: extern_decls,
            });
        }

        for op in &ops {
            if op.kind() == CirOpKind::Func
                && !region_ops(op).is_empty()
                && attr_str(op, "sym_name").is_some_and(|name| name != "main")
                && attr_type(op, "function_type").is_some_and(function_type_is_variadic)
            {
                let name = attr_str(op, "sym_name").unwrap().to_string();
                self.variadic_defs.insert(name.clone());
                if self.va_list_boxed {
                    self.boxed_variadic_defs.insert(name);
                }
            }
        }
        if !self.boxed_variadic_defs.is_empty() {
            items.push(Item::SupportModule(SupportModule {
                name: "__slate_variadic".into(),
                source: r#"struct __SlateVaArg {
    value: Box<dyn std::any::Any>,
    size: usize,
}

impl __SlateVaArg {
    fn new<T: 'static>(value: T) -> Self {
        Self { value: Box::new(value), size: std::mem::size_of::<T>() }
    }

    fn read<T: Copy + 'static>(&self) -> T {
        if let Some(value) = self.value.downcast_ref::<T>() {
            return *value;
        }
        assert!(self.size >= std::mem::size_of::<T>());
        unsafe {
            std::ptr::read_unaligned(
                (self.value.as_ref() as *const dyn std::any::Any) as *const () as *const T,
            )
        }
    }
}

#[derive(Clone)]
struct __SlateVaArgs {
    args: Option<std::rc::Rc<Vec<__SlateVaArg>>>,
    index: usize,
}

impl __SlateVaArgs {
    fn new(args: Vec<__SlateVaArg>) -> Self {
        Self { args: Some(std::rc::Rc::new(args)), index: 0 }
    }

    const fn empty() -> Self {
        Self { args: None, index: 0 }
    }

    fn next_arg<T: Copy + 'static>(&mut self) -> T {
        if std::mem::size_of::<T>() == 0 {
            return unsafe { std::mem::zeroed() };
        }
        let args = self.args.as_ref().expect("va_arg with no arguments");
        let value = args[self.index].read::<T>();
        self.index += 1;
        value
    }
}"#
                .into(),
                exports: Vec::new(),
            }));
        }
        self.unsafe_functions
            .extend(unsafe_defined_functions(module));

        for op in &ops {
            if op.kind() != CirOpKind::Func {
                continue;
            }
            let name = attr_str(op, "sym_name").unwrap_or_default();
            let item = if region_ops(op).is_empty() {
                self.lower_func_alias(op, &ops)
            } else {
                self.lower_func(op)
            };
            if let Some(item) = item {
                items.extend(self.declaration_comment_items("FunctionDecl", name));
                items.push(item);
            }
        }

        let storage_items: Vec<Item> = self
            .generated_alloca_frames
            .iter()
            .cloned()
            .map(Item::Struct)
            .collect();
        items.splice(1..1, storage_items);

        for wrapper in self.enum_wrapper_fns.borrow().values() {
            items.push(Item::Fn(wrapper.clone()));
        }
        for (enum_name, from_ty) in self.needed_enum_from_impls.borrow().iter() {
            if let Some(item) = self.enum_from_impl_item(enum_name, from_ty) {
                items.push(item);
            }
        }

        if self.uses_long_double.get() {
            for decl in f80_shim_decls() {
                self.long_double_shims
                    .entry(decl.name.clone())
                    .or_insert(decl);
            }
        }
        if !self.long_double_shims.is_empty() {
            items.push(Item::ExternBlock {
                abi: "C".into(),
                decls: self
                    .long_double_shims
                    .values()
                    .cloned()
                    .map(ExternDecl::Fn)
                    .collect(),
            });
        }
        if self.uses_long_double.get() && !self.project.shared_long_double {
            let vis = if self.project.emit_pub {
                Visibility::Pub
            } else {
                Visibility::Private
            };
            items.splice(1..1, long_double_prelude(vis));
        }
        if self.uses_complex.get() {
            items.splice(1..1, complex_prelude());
        }
        if self.uses_memchr.get() {
            items.splice(1..1, vec![memchr_prelude()]);
        }

        let mut wiring: Vec<Item> = self
            .project
            .child_modules
            .iter()
            .map(|name| Item::Mod {
                name: Ident::from(name.as_str()),
            })
            .collect();
        if let Some(module) = &self.project.shared_type_module {
            let root = self.project.shared_type_crate.as_deref().unwrap_or("crate");
            for enm in &self.project.shared_enums {
                wiring.push(Item::Use {
                    path: Path::new([
                        Ident::from(root),
                        Ident::from(module.as_str()),
                        Ident::from(enm.as_str()),
                    ]),
                });
            }
            for record in &self.project.shared_records {
                wiring.push(Item::Use {
                    path: Path::new([
                        Ident::from(root),
                        Ident::from(module.as_str()),
                        Ident::from(record.as_str()),
                    ]),
                });
            }
            if self.project.shared_long_double {
                wiring.push(Item::Use {
                    path: Path::new([
                        Ident::from(root),
                        Ident::from(module.as_str()),
                        Ident::from(LONG_DOUBLE_TY),
                    ]),
                });
            }
        }
        wiring.append(&mut self.cross_uses);
        for (offset, item) in wiring.into_iter().enumerate() {
            items.insert(1 + offset, item);
        }

        if self.uses_c_variadic.get() {
            insert_crate_feature(&mut items, Feature::CVariadic);
        }
        if self.uses_f128.get() {
            insert_crate_feature(&mut items, Feature::F128);
        }
        if self.uses_linkage.get() {
            insert_crate_feature(&mut items, Feature::Linkage);
        }
        if self.uses_thread_local.get() {
            insert_crate_feature(&mut items, Feature::ThreadLocal);
        }
        if self.uses_used_with_arg.get() {
            insert_crate_feature(&mut items, Feature::UsedWithArg);
        }
        if self.uses_asm_goto_outputs.get() {
            insert_crate_feature(&mut items, Feature::AsmGotoWithOutputs);
        }
        if self.uses_breakpoint.get() {
            insert_crate_feature(&mut items, Feature::Breakpoint);
        }
        for feature in &self.project.crate_features {
            insert_crate_feature(&mut items, *feature);
        }

        Program { items }
    }

    fn collect_global(&mut self, op: &Op) {
        let Some(name) = attr_str(op, "sym_name") else {
            return;
        };
        if matches!(name, "llvm.compiler.used" | "llvm.used") {
            return;
        }
        if let Some(target) = attr_symbol_ref(op, "aliasee") {
            self.ctx.diagnostics.error(
                format!(
                    "lower: unsupported global alias `{name}` to `{target}`; Rust has no faithful static alias representation"
                ));
            return;
        }
        let rust_name = self.rust_global_name(name);
        let ty = attr_type(op, "sym_type").map(|ty| self.rust_type(ty));
        let alignment = ty.as_ref().and_then(|ty| {
            attr_int(op, "alignment")
                .and_then(|alignment| u32::try_from(alignment).ok())
                .filter(|alignment| *alignment > effective_type_alignment(ty, &self.records))
        });
        let weak = linkage_is_weak(op);
        let thread_local = op.attr("tls_model").is_some();
        if thread_local {
            self.uses_thread_local.set(true);
        }
        self.warn_protected_visibility(op, name);
        let section = attr_str(op, "section").map(str::to_owned);
        let used = self
            .used_symbols
            .get(&sanitize_ident(name).into_string())
            .cloned()
            .unwrap_or_default();
        let is_c_global = !name.starts_with("__") && !name.starts_with(".str");
        let Some(init) = op.attr("initial_value").map(|attr| self.resolve_attr(attr)) else {
            let Some(ty) = ty else {
                return;
            };
            self.extern_globals.insert(
                rust_name,
                ExternGlobal {
                    source_name: name.to_string(),
                    ty,
                    thread_local,
                },
            );
            return;
        };
        if let Attr::ConstArray { elts, .. } = init
            && let Some(items) = attr_array_values(elts)
        {
            if let Some(labels) = block_addr_labels(items) {
                self.block_addr_globals.insert(rust_name.clone(), labels);
                if is_c_global && let Some(ty) = ty {
                    let init = self.default_value_expr(&ty);
                    self.globals.insert(
                        rust_name.clone(),
                        GlobalVar {
                            source_name: name.to_string(),
                            name: rust_name,
                            ty,
                            init,
                            alignment,
                            thread_local,
                            external: externally_exported(op)
                                || self.project.cross_referenced_globals.contains(name),
                            weak,
                            section: section.clone(),
                            used: used.clone(),
                        },
                    );
                }
                return;
            }
            let elem_is_byte_sized = matches!(
                &ty,
                Some(Type::Array { elem, .. }) if matches!(**elem, Type::Prim(Prim::I8 | Prim::U8))
            );
            // synthetic (non-`is_c_global`) element arrays of byte-typed
            // elements are narrow string literals in per-element CIR form -
            // render them the same way narrow string literals are.
            if !is_c_global
                && elem_is_byte_sized
                && let Some(bytes) = items
                    .iter()
                    .map(|item| item.as_int().and_then(|value| u8::try_from(value).ok()))
                    .collect::<Option<Vec<u8>>>()
            {
                self.strings.insert(name.to_string(), bytes);
                return;
            }
            // wide-char string literals (`s32i`/`s16i` code-point elements)
            // can't go through the byte-string path above - render them as
            // a plain integer-element const array instead.
            if !is_c_global
                && name.starts_with(".str")
                && !elem_is_byte_sized
                && let Some(values) = items
                    .iter()
                    .map(Attr::as_int)
                    .collect::<Option<Vec<i128>>>()
            {
                let total_len = ty
                    .as_ref()
                    .and_then(type_array_len)
                    .unwrap_or(values.len() as u64) as usize;
                let mut elems: Vec<Expr> = values
                    .into_iter()
                    .map(|value| Expr::Value(RustValue::I64(value as i64)))
                    .collect();
                elems.resize(total_len, Expr::Value(RustValue::I64(0)));
                self.const_arrays.insert(name.to_string(), elems);
                return;
            }
        }
        if let Attr::ConstArray { elts, .. } = init
            && let Some(bytes) = attr_array_values(elts)
        {
            let Some(bytes) = bytes
                .iter()
                .map(|value| value.as_int().and_then(|value| u8::try_from(value).ok()))
                .collect::<Option<Vec<u8>>>()
            else {
                return;
            };
            if is_c_global && let Some(ty) = ty {
                let len = type_array_len(&ty)
                    .and_then(|len| usize::try_from(len).ok())
                    .unwrap_or(bytes.len());
                let elems = byte_array_elems(&bytes, &ty);
                self.globals.insert(
                    rust_name.clone(),
                    GlobalVar {
                        source_name: name.to_string(),
                        name: rust_name,
                        ty,
                        init: render_array_literal_expr(
                            &elems,
                            len,
                            Expr::Value(RustValue::I64(0)),
                        ),
                        alignment,
                        thread_local,
                        external: externally_exported(op)
                            || self.project.cross_referenced_globals.contains(name),
                        weak,
                        section: section.clone(),
                        used: used.clone(),
                    },
                );
            } else {
                let mut bytes = bytes.clone();
                bytes.push(0);
                self.strings.insert(name.to_string(), bytes);
            }
            return;
        }
        if let Attr::Zero { .. } = init
            && !ty
                .as_ref()
                .is_some_and(|ty| matches!(ty, Type::VaList) || is_boxed_va_args_type(ty))
            && let Some((elem, len)) = attr_type(op, "sym_type").and_then(parse_cir_array_type)
        {
            if is_c_global && let Some(ty) = ty {
                self.globals.insert(
                    rust_name.clone(),
                    GlobalVar {
                        source_name: name.to_string(),
                        name: rust_name,
                        init: self.default_value_expr(&Type::Array {
                            elem: Box::new(self.rust_type(elem)),
                            len,
                        }),
                        ty,
                        alignment,
                        thread_local,
                        external: externally_exported(op)
                            || self.project.cross_referenced_globals.contains(name),
                        weak,
                        section: section.clone(),
                        used: used.clone(),
                    },
                );
            } else if matches!(elem, CirType::Named(n) if n == "s8i") && name.starts_with(".str") {
                self.strings.insert(name.to_string(), vec![0; len as usize]);
            } else if parse_cir_int_type(elem).is_some() {
                self.const_arrays.insert(name.to_string(), Vec::new());
            } else {
                self.const_zero_globals.insert(name.to_string());
            }
            return;
        }
        if let Attr::Zero { .. } = init {
            if is_c_global && let Some(ty) = ty {
                self.globals.insert(
                    rust_name.clone(),
                    GlobalVar {
                        source_name: name.to_string(),
                        name: rust_name,
                        init: self.default_value_expr(&ty),
                        ty,
                        alignment,
                        thread_local,
                        external: externally_exported(op)
                            || self.project.cross_referenced_globals.contains(name),
                        weak,
                        section: section.clone(),
                        used: used.clone(),
                    },
                );
            } else {
                self.const_zero_globals.insert(name.to_string());
            }
            return;
        }
        if is_c_global && let Some(ty) = ty {
            let mut facts: VecDeque<FloatingLiteralFact> = self
                .global_floating_literals
                .get(name)
                .cloned()
                .unwrap_or_default()
                .into();
            if let Some(init) = self.render_const_value_expr(&ty, init, &mut facts) {
                let external =
                    externally_exported(op) || self.project.cross_referenced_globals.contains(name);
                self.globals.insert(
                    rust_name.clone(),
                    GlobalVar {
                        source_name: name.to_string(),
                        name: rust_name,
                        ty,
                        init,
                        alignment,
                        thread_local,
                        external,
                        weak,
                        section,
                        used,
                    },
                );
            }
        } else if !is_c_global {
            self.const_aggregates.insert(name.to_string(), init.clone());
        }
    }

    fn lower_func_alias(&mut self, op: &Op, ops: &[&Op]) -> Option<Item> {
        let name = attr_str(op, "sym_name")?;
        let target = attr_symbol_ref(op, "aliasee")?;
        if linkage_is_weak(op) {
            return None;
        }
        let target_op = ops.iter().find(|candidate| {
            candidate.kind() == CirOpKind::Func
                && attr_str(candidate, "sym_name") == Some(target)
                && !region_ops(candidate).is_empty()
        });
        if target_op.is_none() {
            self.ctx.diagnostics.error(format!(
                "lower: unsupported function alias `{name}` to external target `{target}`"
            ));
            return None;
        }

        let function_type = attr_type(op, "function_type").unwrap_or(&CirType::Void);
        let (decl, _, _) = self.extern_fn_signature(name, function_type);
        if decl.variadic {
            self.ctx.diagnostics.error(format!(
                "lower: unsupported variadic function alias `{name}` to `{target}`"
            ));
            return None;
        }

        let external_def = self.project.emit_pub
            && (externally_exported(op) || self.project.cross_referenced_functions.contains(name));
        self.warn_protected_visibility(op, name);
        let attrs = symbol_attrs(
            external_def,
            linkage_is_weak(op),
            attr_str(op, "section"),
            &[],
        );
        if linkage_is_weak(op) {
            self.uses_linkage.set(true);
        }
        let args = decl
            .params
            .iter()
            .map(|param| Expr::Var(param.name.clone().into()))
            .collect();
        let mut call = Expr::Call {
            binding: crate::function_identity::CallBinding::Generated,
            func: Box::new(Expr::Var(target.into())),
            args,
        };
        let unsafe_ =
            self.unsafe_functions.contains(name) || self.unsafe_functions.contains(target);
        if unsafe_ {
            call = FunctionLowerer::unsafe_expr(call);
        }
        let stmt = if decl.ret.is_some() {
            Stmt::Return(Some(call))
        } else {
            Stmt::Expr(call)
        };

        Some(Item::Fn(FnDef {
            attrs,
            vis: if external_def {
                Visibility::Pub
            } else {
                Visibility::Private
            },
            unsafe_,
            abi: if external_def || self.c_abi_functions.contains(name) {
                Some(Abi::C)
            } else {
                None
            },
            name: name.to_string(),
            params: decl.params,
            ret: decl.ret,
            body: vec![IndentStmt { depth: 1, stmt }],
        }))
    }

    fn lower_enum(&mut self, enm: &crate::frontend::c_ast::Enum) -> Option<Item> {
        lower_enum_def(enm, Visibility::Private).map(Item::Enum)
    }

    fn warn_protected_visibility(&mut self, op: &Op, name: &str) {
        if visibility_is_protected(op) {
            self.ctx.diagnostics.warn(
                format!(
                    "lower: protected visibility on `{name}` has no faithful Rust representation; falling back to default exported visibility"
                ));
        }
    }

    fn lower_record(&mut self, record: &crate::frontend::c_ast::Record) -> Vec<Item> {
        if record
            .fields
            .iter()
            .any(|field| ctype_uses_long_double(&field.ty))
        {
            self.uses_long_double.set(true);
        }
        let storage_record;
        let record = if record.kind == RecordKind::Union && record.fields.is_empty() {
            storage_record = crate::frontend::c_ast::Record {
                fields: vec![crate::frontend::c_ast::Decl {
                    name: "__slate_empty".into(),
                    comments: Vec::new(),
                    ty: crate::frontend::c_ast::CType::Array(
                        Box::new(crate::frontend::c_ast::CType::Int {
                            signed: false,
                            bits: 8,
                        }),
                        Some(0),
                    ),
                    bit_width: None,
                }],
                ..record.clone()
            };
            &storage_record
        } else if let Some(fields) = self.bitfield_storage_fields(record) {
            storage_record = crate::frontend::c_ast::Record {
                fields,
                ..record.clone()
            };
            &storage_record
        } else {
            record
        };
        lower_record_def(
            record,
            Visibility::Private,
            Visibility::Private,
            true,
            self.va_list_boxed,
        )
    }

    fn bitfield_storage_fields(
        &self,
        record: &crate::frontend::c_ast::Record,
    ) -> Option<Vec<crate::frontend::c_ast::Decl>> {
        if record.fields.is_empty() || record.fields.iter().all(|field| field.bit_width.is_none()) {
            return None;
        }
        let expanded = self.aliases.values().find(|ty| {
            cir_record_name(ty).is_some_and(|name| {
                sanitize_ident(name).as_str() == sanitize_ident(&record.name).as_str()
            })
        })?;
        let CirType::Struct {
            members: Some(members),
            ..
        } = expanded
        else {
            return None;
        };
        let preserve_field_names = record
            .fields
            .iter()
            .any(|field| field.name.starts_with("__bitfield_"));
        let record_name = sanitize_ident(&record.name).into_string();
        Some(
            members
                .iter()
                .enumerate()
                .map(|(index, ty)| crate::frontend::c_ast::Decl {
                    name: if preserve_field_names {
                        record
                            .fields
                            .get(index)
                            .map(|field| field.name.clone())
                            .unwrap_or_else(|| format!("__bitfield_{index}"))
                    } else {
                        format!("__bitfield_{index}")
                    },
                    comments: Vec::new(),
                    ty: self
                        .bitfield_storages
                        .get(&(record_name.clone(), index))
                        .map(|storage| {
                            crate::frontend::c_ast::CType::Record(storage.wrapper.clone())
                        })
                        .unwrap_or_else(|| cir_type_to_ctype(ty, &self.aliases)),
                    bit_width: None,
                })
                .collect(),
        )
    }

    fn standard_record_defs(&self) -> Vec<Item> {
        let mut out = Vec::new();
        for name in ["div_t", "ldiv_t", "lldiv_t", "imaxdiv_t", "tm", "lconv"] {
            if self.records.contains_key(name) {
                continue;
            }
            if self
                .aliases
                .values()
                .any(|ty| cir_record_name(ty) == Some(name))
            {
                out.push(Item::Record(standard_record_def(name)));
            }
        }
        out
    }

    fn lower_func(&mut self, op: &Op) -> Option<Item> {
        let name = attr_str(op, "sym_name")?;
        let weak_alias_target = self.weak_aliases.values().any(|target| target == name);
        let function_type = attr_type(op, "function_type");
        let (param_types, ret_ty) = function_type.map(parse_function_type).unwrap_or_default();
        let entry = op.regions.first()?.blocks.first()?;
        let is_main = name == "main";
        let is_variadic = !is_main && function_type.is_some_and(function_type_is_variadic);
        let boxed_variadic = self.boxed_variadic_defs.contains(name);

        let mut declared_param_names = BTreeSet::new();
        let mut entry_arg_names = BTreeMap::new();
        let mut params = entry
            .args
            .iter()
            .enumerate()
            .map(|(i, (arg, ty))| {
                let ty = param_types.get(i).unwrap_or(ty);
                let base = sanitize_ident(arg).into_string();
                let rust_name = if is_main {
                    declared_param_names.insert(base.clone());
                    base
                } else {
                    let mut suffix = 1;
                    loop {
                        let candidate = if suffix == 1 {
                            base.clone()
                        } else {
                            format!("{base}{suffix}")
                        };
                        if !self.globals.contains_key(&candidate)
                            && !self.extern_globals.contains_key(&candidate)
                            && declared_param_names.insert(candidate.clone())
                        {
                            break candidate;
                        }
                        suffix += 1;
                    }
                };
                entry_arg_names.insert(arg.clone(), rust_name.clone());
                FnParam {
                    name: rust_name,
                    mutable: false,
                    ty: self.rust_type(ty),
                }
            })
            .collect::<Vec<_>>();

        let va_args_param = if is_variadic {
            let param = "__slate_va_args".to_string();
            params.push(FnParam {
                name: param.clone(),
                mutable: true,
                ty: if boxed_variadic {
                    Type::Custom("__SlateVaArgs".into())
                } else {
                    Type::Variadic
                },
            });
            Some(param)
        } else {
            None
        };

        let (vis, abi, ret, prelude) = if is_main {
            params.clear();
            let mut prelude = self.main_arg_bindings(entry);
            prelude.extend(
                self.ctor_calls
                    .iter()
                    .map(|name| hook_call_stmt(name, &self.unsafe_functions)),
            );
            (Visibility::Private, None, None, prelude)
        } else {
            let external_def = self.project.emit_pub
                && (externally_exported(op)
                    || self.project.cross_referenced_functions.contains(name))
                || weak_alias_target;
            let vis = if external_def {
                Visibility::Pub
            } else {
                Visibility::Private
            };
            let abi = if !boxed_variadic
                && (external_def || is_variadic || self.c_abi_functions.contains(name))
            {
                Some(Abi::C)
            } else {
                None
            };
            if is_variadic {
                if !boxed_variadic {
                    self.uses_c_variadic.set(true);
                }
                self.variadic_defs.insert(name.to_string());
            }
            let ret = Some(self.rust_type(ret_ty.as_ref().unwrap_or(&CirType::Void)))
                .filter(|ty| !matches!(ty, Type::CLib(c) if *c == CLibType::VOID));
            (vis, abi, ret, Vec::<Stmt>::new())
        };

        let diverges = !is_main
            && attr_bool(op, "noreturn")
            && op.regions.first().is_some_and(region_ends_in_noreturn_call);
        if !is_main && attr_bool(op, "noreturn") && !diverges {
            self.ctx.diagnostics.warn(
                format!(
                    "lower: __attribute__((noreturn)) on `{name}` does not structurally prove divergence; keeping its declared return type"
                ));
        }
        let ret = if diverges { Some(Type::Never) } else { ret };

        if self.c_abi_functions.contains(name)
            && (params
                .iter()
                .any(|param| type_mentions_long_double(&param.ty))
                || ret.as_ref().is_some_and(type_mentions_long_double))
        {
            let ret_shim_ty = ret.clone().unwrap_or(Type::Unit);
            let trampoline = format!("__slate_ld_{}", sanitize_ident(name));
            self.long_double_shims
                .entry(trampoline.clone())
                .or_insert_with(|| ExternFnDecl {
                    identity: FunctionIdentity::Unknown,
                    name: trampoline.clone(),
                    declared_type: None,
                    params: params
                        .iter()
                        .enumerate()
                        .map(|(i, param)| FnParam {
                            name: format!("_{i}"),
                            mutable: false,
                            ty: param.ty.clone(),
                        })
                        .collect(),
                    variadic: false,
                    ret: (!ret_shim_ty.is_unit()).then_some(ret_shim_ty),
                    safe: true,
                });
            self.long_double_callback_trampolines
                .insert(name.to_string(), trampoline);
        }

        let attrs = symbol_attrs(
            !is_main
                && (self.project.emit_pub
                    && (externally_exported(op)
                        || self.project.cross_referenced_functions.contains(name))
                    || weak_alias_target
                    || self.long_double_callback_trampolines.contains_key(name)),
            linkage_is_weak(op),
            attr_str(op, "section"),
            &[],
        );
        self.warn_protected_visibility(op, name);
        if linkage_is_weak(op) {
            self.uses_linkage.set(true);
        }
        let unsafe_ = is_variadic || self.unsafe_functions.contains(name);
        let layout_queries: VecDeque<_> = self
            .layout_queries
            .get(name)
            .cloned()
            .unwrap_or_default()
            .into();
        let macro_consts: VecDeque<_> = self
            .macro_consts
            .get(name)
            .cloned()
            .unwrap_or_default()
            .into();
        let enum_consts: VecDeque<_> = self
            .enum_consts
            .get(name)
            .cloned()
            .unwrap_or_default()
            .into();
        let asm_gotos: VecDeque<_> = self.asm_gotos.get(name).cloned().unwrap_or_default().into();
        let mut function_ops = Vec::new();
        collect_region_ops_recursive(op, &mut function_ops);
        if self.naked_functions.contains(name) {
            return self.lower_naked_func(name, &function_ops, attrs, vis, params, ret);
        }
        let local_enum_decls = self
            .local_enum_decls
            .get(name)
            .map_or(&[][..], Vec::as_slice);
        let integer_enum_locals =
            enum_locals_requiring_integer_storage(local_enum_decls, &self.enums, &function_ops);
        let local_enum_types: BTreeMap<String, String> = local_enum_decls
            .iter()
            .filter(|decl| self.enums.contains_key(&decl.enum_name))
            .filter(|decl| !integer_enum_locals.contains(&decl.name))
            .map(|decl| (decl.name.clone(), decl.enum_name.clone()))
            .collect();
        let va_allocas = function_ops
            .iter()
            .filter(|op| {
                matches!(
                    op.kind(),
                    CirOpKind::VaStart | CirOpKind::VaArg | CirOpKind::VaCopy
                )
            })
            .flat_map(|op| op.operands.iter().cloned())
            .collect();
        let mut f = FunctionLowerer {
            parent: self,
            values: BTreeMap::new(),
            const_int_values: BTreeMap::new(),
            function_pointer_null_values: BTreeSet::new(),
            slots: BTreeMap::new(),
            slot_places: BTreeMap::new(),
            aligned_slots: BTreeSet::new(),
            slot_types: BTreeMap::new(),
            member_ptrs: BTreeMap::new(),
            element_ptrs: BTreeMap::new(),
            block_addr_element_ptrs: BTreeMap::new(),
            indirect_target_values: BTreeMap::new(),
            temp_counter: 0,
            indent: 1,
            body: Vec::new(),
            is_main,
            loop_stack: Vec::new(),
            label_counter: 0,
            dispatch: None,
            hoisting_allocas: false,
            hoisted: BTreeSet::new(),
            declared_local_names: declared_param_names,
            forward_allocas: forwardable_temp_allocas(op.regions.first()?),
            forward_values: BTreeMap::new(),
            immutable_temps: BTreeSet::new(),
            va_allocas,
            va_places: BTreeMap::new(),
            va_args_param,
            layout_queries,
            macro_consts,
            enum_consts,
            macro_arith_values: BTreeMap::new(),
            asm_outputs: BTreeMap::new(),
            asm_gotos,
            asm_output_places: BTreeMap::new(),
            local_enum_types,
            loaded_field_types: BTreeMap::new(),
            load_ptr_operand: BTreeMap::new(),
            member_base_operand: BTreeMap::new(),
            coerce_alloca_real_type: BTreeMap::new(),
        };

        for stmt in prelude {
            f.push_stmt(stmt);
        }
        for (arg, arg_ty) in &entry.args {
            let rust_name = entry_arg_names
                .get(arg)
                .cloned()
                .unwrap_or_else(|| arg.clone());
            f.immutable_temps.insert(rust_name.clone());
            f.values
                .insert(arg.clone(), Val::Expr(Expr::Var(rust_name.into())));
            if let fn_ptr_ty @ Type::FnPtr { .. } = f.parent.rust_type(arg_ty) {
                f.loaded_field_types.insert(arg.clone(), fn_ptr_ty);
            }
        }
        let body = op.regions.first().unwrap();
        if body.blocks.len() > 1 {
            let returns_value = !matches!(ret, None | Some(Type::Unit));
            f.lower_dispatch(body, returns_value);
        } else {
            f.lower_block(entry);
        }
        if diverges && matches!(f.body.last().map(|s| &s.stmt), Some(Stmt::Return(None))) {
            f.body.pop();
        }
        Some(Item::Fn(FnDef {
            attrs,
            vis,
            unsafe_,
            abi,
            name: sanitize_ident(name).into_string(),
            params,
            ret,
            body: f.body,
        }))
    }

    fn lower_naked_func(
        &mut self,
        name: &str,
        function_ops: &[&Op],
        mut attrs: Vec<RustAttr>,
        vis: Visibility,
        params: Vec<FnParam>,
        ret: Option<Type>,
    ) -> Option<Item> {
        let asm_ops: Vec<&Op> = function_ops
            .iter()
            .filter(|op| op.kind() == CirOpKind::Asm)
            .copied()
            .collect();
        let Some(dialect) = asm_ops.first().map(|op| cir_asm_dialect(op)) else {
            self.ctx.diagnostics.error(format!(
                "lower: __attribute__((naked)) function `{name}` has no inline assembly body"
            ));
            return None;
        };
        let mut lines = Vec::with_capacity(asm_ops.len());
        for asm_op in asm_ops {
            let Some(raw) = attr_str(asm_op, "asm_string") else {
                continue;
            };
            let Ok(template) = String::from_utf8(decode_cir_string(raw)) else {
                self.ctx
                    .diagnostics
                    .error("lower: inline assembly template is not valid UTF-8");
                return None;
            };
            lines.push(template.replace("$$", "$"));
        }
        attrs.push(RustAttr::Naked);
        let stmt = Stmt::Expr(Expr::Macro {
            name: "core::arch::naked_asm".into(),
            args: asm_macro_args(lines.join("\n\t"), dialect),
        });
        Some(Item::Fn(FnDef {
            attrs,
            vis,
            unsafe_: false,
            abi: Some(Abi::C),
            name: name.to_string(),
            params,
            ret,
            body: vec![IndentStmt { depth: 1, stmt }],
        }))
    }

    fn main_arg_bindings(&self, entry: &Block) -> Vec<Stmt> {
        if entry.args.is_empty() {
            return Vec::new();
        }

        let call = |path: &str, args: Vec<Expr>| Expr::Call {
            binding: crate::function_identity::CallBinding::Generated,
            func: Box::new(Expr::Var(path.into())),
            args,
        };
        let method = |recv: Expr, name: &str, args: Vec<Expr>| Expr::MethodCall {
            recv: Box::new(recv),
            method: name.into(),
            args,
        };
        let char_ptr = Type::Ptr {
            mutable: true,
            inner: Box::new(Type::Prim(Prim::I8)),
        };

        let storage_init = method(
            method(
                call("std::env::args", vec![]),
                "map",
                vec![Expr::Closure {
                    params: vec!["arg".into()],
                    body: Box::new(method(
                        call("std::ffi::CString::new", vec![Expr::Var("arg".into())]),
                        "unwrap",
                        vec![],
                    )),
                }],
            ),
            "collect",
            vec![],
        );
        let ptrs_init = method(
            method(
                method(Expr::Var("__slate_argv_storage".into()), "iter", vec![]),
                "map",
                vec![Expr::Closure {
                    params: vec!["arg".into()],
                    body: Box::new(Expr::Cast {
                        expr: Box::new(method(Expr::Var("arg".into()), "as_ptr", vec![])),
                        ty: char_ptr.clone(),
                    }),
                }],
            ),
            "collect",
            vec![],
        );

        let mut stmts = vec![
            Stmt::Let {
                name: "__slate_argv_storage".into(),
                mutable: true,
                ty: Some(Type::Generic {
                    name: "Vec".into(),
                    args: vec![Type::Custom("std::ffi::CString".into())],
                }),
                init: Some(storage_init),
            },
            Stmt::Let {
                name: "__slate_argv_ptrs".into(),
                mutable: true,
                ty: Some(Type::Generic {
                    name: "Vec".into(),
                    args: vec![char_ptr],
                }),
                init: Some(ptrs_init),
            },
            Stmt::Expr(method(
                Expr::Var("__slate_argv_ptrs".into()),
                "push",
                vec![Expr::Value(RustValue::NullPtr)],
            )),
        ];

        for (i, (arg, ty)) in entry.args.iter().enumerate() {
            let value = match i {
                0 => Expr::Cast {
                    expr: Box::new(method(
                        Expr::Var("__slate_argv_storage".into()),
                        "len",
                        vec![],
                    )),
                    ty: Type::Prim(Prim::I32),
                },
                1 => method(Expr::Var("__slate_argv_ptrs".into()), "as_mut_ptr", vec![]),
                _ => Expr::Value(RustValue::NullPtr),
            };
            stmts.push(Stmt::Let {
                name: sanitize_ident(arg).into_string(),
                mutable: false,
                ty: Some(self.rust_type(ty)),
                init: Some(value),
            });
        }
        stmts
    }

    /// Build a Rust `extern "C"` signature for a body-less C declaration,
    /// returning `(line, fixed_param_rust_types, return_type)`. Trailing `...` becomes a Rust
    /// variadic; a missing return arrow means the C function returns `void`.
    fn extern_fn_signature(
        &self,
        name: &str,
        function_type: &CirType,
    ) -> (ExternFnDecl, Vec<Type>, Option<String>) {
        let (param_tys, ret_ty) = parse_function_type(function_type);
        let variadic = function_type_is_variadic(function_type);

        let params = param_tys
            .iter()
            .enumerate()
            .map(|(i, ty)| FnParam {
                name: format!("_{i}"),
                mutable: false,
                ty: self.rust_type(ty),
            })
            .collect::<Vec<_>>();
        let ret_ast = ret_ty
            .as_ref()
            .map(|ty| self.rust_type(ty))
            .filter(|ty| !matches!(ty, Type::CLib(c) if *c == CLibType::VOID));
        let identity = *self
            .known_functions
            .get(name)
            .unwrap_or(&FunctionIdentity::Unknown);
        let mut decl = ExternFnDecl {
            name: name.into(),
            identity,
            declared_type: self.function_types.get(name).cloned(),
            params,
            variadic,
            ret: ret_ast,
            safe: false,
        };
        repair_extern_function_signature(
            &mut decl,
            self.function_types.get(name).map(String::as_str),
        );
        let param_types = decl.params.iter().map(|param| param.ty.clone()).collect();
        let ret_ty = decl.ret.as_ref().map(Type::render);
        if decl.variadic || decl.params.iter().any(|param| param.ty == Type::VaList) {
            self.uses_c_variadic.set(true);
        }
        (decl, param_types, ret_ty)
    }

    fn rust_type(&self, cir_ty: &CirType) -> Type {
        let ty = rust_type_with_aliases(cir_ty, &self.aliases, self.va_list_boxed);
        if type_mentions_long_double(&ty) {
            self.uses_long_double.set(true);
        }
        if type_mentions_complex(&ty) {
            self.uses_complex.set(true);
        }
        if type_mentions_f128(&ty) {
            self.uses_f128.set(true);
        }
        ty
    }

    fn rust_type_has_long_double(&self, ty: &Type) -> bool {
        match ty {
            Type::LongDouble => true,
            Type::Custom(name) => self.records.get(name).is_some_and(|record| {
                record
                    .fields
                    .iter()
                    .any(|field| ctype_uses_long_double(&field.ty))
            }),
            Type::Complex(inner)
            | Type::Ref { inner, .. }
            | Type::Slice(inner)
            | Type::Ptr { inner, .. }
            | Type::Array { elem: inner, .. } => self.rust_type_has_long_double(inner),
            Type::FnPtr { params, ret, .. } => {
                params
                    .iter()
                    .any(|param| self.rust_type_has_long_double(param))
                    || self.rust_type_has_long_double(ret)
            }
            Type::Generic { args, .. } => {
                args.iter().any(|arg| self.rust_type_has_long_double(arg))
            }
            _ => false,
        }
    }

    fn cir_type_is_union(&self, ty: &CirType) -> bool {
        let ty = self.expand_alias(ty);
        if matches!(ty, CirType::Union { .. }) {
            return true;
        }
        cir_record_name(ty)
            .and_then(|name| self.records.get(sanitize_ident(name).as_str()))
            .is_some_and(|record| record.kind == RecordKind::Union)
    }

    fn resolve_attr<'b>(&'b self, attr: &'b Attr) -> &'b Attr {
        let mut current = attr;
        let mut seen = BTreeSet::new();
        while let Attr::Named(name) = current {
            if !seen.insert(name.clone()) {
                break;
            }
            match self.attr_aliases.get(name) {
                Some(next) => current = next,
                None => break,
            }
        }
        current
    }

    fn expand_alias<'b>(&'b self, ty: &'b CirType) -> &'b CirType {
        let mut ty = ty;
        let mut seen = BTreeSet::new();
        while let CirType::Named(name) = ty {
            if !seen.insert(name.clone()) {
                break;
            }
            match self.aliases.get(name) {
                Some(expanded) => ty = expanded,
                None => break,
            }
        }
        ty
    }

    fn record_field_type(&self, ty: &crate::frontend::c_ast::CType) -> Type {
        if ctype_uses_long_double(ty) {
            self.uses_long_double.set(true);
        }
        self.c_record_field_type(ty)
    }

    fn enum_return_mismatch_wrap(&self, fn_name: &str, target_ty: &Type) -> Option<Expr> {
        let Type::FnPtr { ret, params, .. } = target_ty else {
            return None;
        };
        let Type::Custom(enum_name) = ret.as_ref() else {
            return None;
        };
        if !self.enums.contains_key(enum_name) {
            return None;
        }
        let source_ret = self.function_return_types.get(fn_name)?;
        if source_ret == &Type::Custom(enum_name.clone()) {
            return None;
        }
        Some(self.wrap_enum_returning_function(fn_name, source_ret.clone(), enum_name, params))
    }

    fn wrap_enum_returning_function(
        &self,
        fn_name: &str,
        source_ret: Type,
        enum_name: &str,
        params: &[Type],
    ) -> Expr {
        self.needed_enum_from_impls
            .borrow_mut()
            .insert((enum_name.to_string(), source_ret));
        let wrapper_name = format!("__slate_enum_wrap_{fn_name}");
        let fn_params: Vec<FnParam> = params
            .iter()
            .enumerate()
            .map(|(i, ty)| FnParam {
                name: format!("_{i}"),
                mutable: false,
                ty: ty.clone(),
            })
            .collect();
        let args: Vec<Expr> = fn_params
            .iter()
            .map(|p| Expr::Var(p.name.clone().into()))
            .collect();
        self.enum_wrapper_fns
            .borrow_mut()
            .entry(wrapper_name.clone())
            .or_insert_with(|| FnDef {
                attrs: Vec::new(),
                vis: Visibility::Private,
                unsafe_: false,
                abi: Some(Abi::C),
                name: wrapper_name.clone(),
                params: fn_params,
                ret: Some(Type::Custom(enum_name.to_string())),
                body: vec![IndentStmt {
                    depth: 1,
                    stmt: Stmt::Return(Some(Expr::Call {
                        binding: crate::function_identity::CallBinding::Generated,
                        func: Box::new(Expr::Path(Path::new([enum_name, "from"].map(Ident::from)))),
                        args: vec![Expr::Call {
                            binding: crate::function_identity::CallBinding::Generated,
                            func: Box::new(Expr::Var(sanitize_ident(fn_name))),
                            args,
                        }],
                    })),
                }],
            });
        Expr::Call {
            binding: crate::function_identity::CallBinding::Generated,
            func: Box::new(Expr::Var("Some".into())),
            args: vec![Expr::Var(wrapper_name.into())],
        }
    }

    fn enum_from_impl_item(&self, enum_name: &str, from_ty: &Type) -> Option<Item> {
        let enm = self.enums.get(enum_name)?;
        let arms = enm
            .variants
            .iter()
            .map(|variant| ExprMatchArm {
                pattern: int_pattern(variant.value as i128),
                value: Expr::Path(Path::new([
                    Ident::from(enum_name),
                    Ident::from(sanitize_ident(&variant.name).as_str()),
                ])),
            })
            .chain(std::iter::once(ExprMatchArm {
                pattern: Pattern::Wildcard,
                value: Expr::Macro {
                    name: "unreachable".into(),
                    args: Vec::new(),
                },
            }))
            .collect();
        Some(Item::Impl(ImplBlock {
            generics: vec![],
            trait_: Some(TraitRef::From(from_ty.clone())),
            self_ty: Type::Custom(enum_name.to_string()),
            items: vec![ImplItem::Method(Method {
                name: "from".into(),
                self_kind: SelfKind::None,
                params: vec![FnParam {
                    name: "value".into(),
                    mutable: false,
                    ty: from_ty.clone(),
                }],
                ret: Some(Type::Custom(enum_name.to_string())),
                body: Expr::Match {
                    expr: Box::new(Expr::Var("value".into())),
                    arms,
                },
            })],
        }))
    }

    fn default_value_expr(&self, ty: &Type) -> Expr {
        if is_boxed_va_args_type(ty) {
            return empty_va_args_expr();
        }
        match ty {
            Type::Custom(name) => {
                if let Some(storage) = self
                    .bitfield_storages
                    .values()
                    .find(|storage| storage.wrapper == *name)
                {
                    return Expr::Transmute {
                        from: storage.backing.clone(),
                        to: ty.clone(),
                        expr: Box::new(self.default_value_expr(&storage.backing)),
                    };
                }
                if let Some(enm) = self.enums.get(name)
                    && let Some(variant) = enm.variants.first()
                {
                    return Expr::Path(Path::new([
                        Ident::from(name.as_str()),
                        Ident::from(sanitize_ident(&variant.name).as_str()),
                    ]));
                }
                if let Some(record) = self.records.get(name) {
                    if let Some(fields) = self.bitfield_storage_fields(record) {
                        if record.kind == RecordKind::Union {
                            let field = fields.first().expect("bitfield record has storage");
                            return Expr::StructLit {
                                name: record_lit_name(record),
                                fields: vec![(
                                    field.name.clone(),
                                    self.default_value_expr(&self.c_record_field_type(&field.ty)),
                                )],
                            };
                        }
                        return Expr::StructLit {
                            name: record_lit_name(record),
                            fields: fields
                                .iter()
                                .map(|field| {
                                    (
                                        field.name.clone(),
                                        self.default_value_expr(
                                            &self.c_record_field_type(&field.ty),
                                        ),
                                    )
                                })
                                .collect(),
                        };
                    }
                    match record.kind {
                        RecordKind::Struct => {
                            let fields = record
                                .fields
                                .iter()
                                .map(|field| {
                                    (
                                        sanitize_ident(&field.name).into_string(),
                                        self.default_value_expr(
                                            &self.c_record_field_type(&field.ty),
                                        ),
                                    )
                                })
                                .collect();
                            return wrap_record_lit(
                                record,
                                Expr::StructLit {
                                    name: record_lit_name(record),
                                    fields,
                                },
                            );
                        }
                        RecordKind::Union => {
                            if let Some(field) = record.fields.first() {
                                return wrap_record_lit(
                                    record,
                                    Expr::StructLit {
                                        name: record_lit_name(record),
                                        fields: vec![(
                                            sanitize_ident(&field.name).into_string(),
                                            self.default_value_expr(
                                                &self.c_record_field_type(&field.ty),
                                            ),
                                        )],
                                    },
                                );
                            }
                            return Expr::StructLit {
                                name: record_lit_name(record),
                                fields: vec![("__slate_empty".into(), Expr::ArrayLit(Vec::new()))],
                            };
                        }
                    }
                }
                standard_record_default_expr(name).unwrap_or_else(|| default_value_for_type(ty))
            }
            Type::LongDouble => long_double_zero_expr(),
            Type::Complex(inner) => {
                let d = default_value_for_type(inner);
                Expr::StructLit {
                    name: COMPLEX_TY.into(),
                    fields: vec![("re".into(), d.clone()), ("im".into(), d)],
                }
            }
            Type::Array { elem, .. } if is_boxed_va_args_type(elem) => Expr::Call {
                binding: crate::function_identity::CallBinding::Generated,
                func: Box::new(Expr::Var("core::array::from_fn".into())),
                args: vec![Expr::Closure {
                    params: vec![Ident::from("_")],
                    body: Box::new(self.default_value_expr(elem)),
                }],
            },
            Type::Array { elem, len } => Expr::ArrayRepeat {
                elem: Box::new(self.default_value_expr(elem)),
                len: *len as usize,
            },
            Type::CLib(ty) => match ty.initializer() {
                CLibInitializer::ScalarZero => Expr::Value(RustValue::I64(0)),
                CLibInitializer::Zeroed => {
                    Expr::Unsafe(Box::new(crate::backend::rust_ast::Block {
                        stmts: Vec::new(),
                        tail: Some(Box::new(Expr::Call {
                            binding: crate::function_identity::CallBinding::Generated,
                            func: Box::new(Expr::Var(
                                format!("std::mem::zeroed::<{}>", ty.path()).into(),
                            )),
                            args: Vec::new(),
                        })),
                    }))
                }
                CLibInitializer::Fields(fields) => Expr::StructLit {
                    name: ty.path().into(),
                    fields: fields
                        .iter()
                        .map(|field| ((*field).into(), Expr::Value(RustValue::I64(0))))
                        .collect(),
                },
            },
            _ => default_value_for_type(ty),
        }
    }

    fn type_is_enum(&self, ty: &Type) -> bool {
        matches!(ty, Type::Custom(name) if self.enums.contains_key(name))
    }

    fn type_is_enum_ptr(&self, ty: &Type) -> bool {
        matches!(ty, Type::Ptr { inner, .. } if self.type_is_enum(inner))
    }

    fn layout_query_value(&self, query: &LayoutQuery) -> Option<i128> {
        match query {
            LayoutQuery::Size(ty) => c_layout(ty, &self.records).map(|layout| layout.size),
            LayoutQuery::Align(ty) => c_layout(ty, &self.records).map(|layout| layout.align),
            LayoutQuery::Offset { record, field } => {
                record_field_offset(record, field, &self.records)
            }
        }
        .map(i128::from)
    }

    fn layout_query_expr(&self, query: &LayoutQuery) -> Option<Expr> {
        match query {
            LayoutQuery::Size(ty @ crate::frontend::c_ast::CType::Record(_)) => {
                Some(layout_call("size_of", &self.c_type_to_type(ty)))
            }
            LayoutQuery::Align(ty @ crate::frontend::c_ast::CType::Record(_)) => {
                Some(layout_call("align_of", &self.c_type_to_type(ty)))
            }
            LayoutQuery::Size(_) | LayoutQuery::Align(_) => None,
            LayoutQuery::Offset { record, field } => {
                let source_record = self.records.get(&sanitize_ident(record).into_string())?;
                if source_record.packed.is_some() && source_record.align.is_some() {
                    return None;
                }
                let record = sanitize_ident(record).into_string();
                Some(Expr::Macro {
                    name: "std::mem::offset_of".into(),
                    args: vec![Expr::Var(record.into()), Expr::Var(sanitize_ident(field))],
                })
            }
        }
    }

    fn render_const_value_expr(
        &self,
        ty: &Type,
        attr: &Attr,
        facts: &mut VecDeque<FloatingLiteralFact>,
    ) -> Option<Expr> {
        let attr = self.resolve_attr(attr);
        if let Type::Custom(name) = ty
            && let Some(storage) = self
                .bitfield_storages
                .values()
                .find(|storage| storage.wrapper == *name)
        {
            return Some(Expr::Transmute {
                from: storage.backing.clone(),
                to: ty.clone(),
                expr: Box::new(self.render_const_value_expr(&storage.backing, attr, facts)?),
            });
        }
        if let Type::Custom(name) = ty
            && self
                .records
                .get(name)
                .is_some_and(|record| record.kind == RecordKind::Union && record.fields.is_empty())
        {
            return Some(self.default_value_expr(ty));
        }
        if let Type::CLib(clib) = ty
            && clib.initializer() == CLibInitializer::Zeroed
        {
            return Some(self.default_value_expr(ty));
        }
        match attr {
            Attr::ConstComplex { real, imag, .. } => {
                let re = complex_component_from_attr(real)?;
                let im = complex_component_from_attr(imag)?;
                Some(complex_const_expr(Some(ty), re, im))
            }
            Attr::ConstRecord { members, .. } => {
                let elements = attr_array_values(members)?;
                let Type::Custom(name) = ty else {
                    return None;
                };
                let record = self.records.get(name)?;
                match record.kind {
                    RecordKind::Struct => {
                        if let Some(storage_fields) = self.bitfield_storage_fields(record) {
                            let fields = storage_fields
                                .iter()
                                .enumerate()
                                .map(|(i, field)| {
                                    let field_ty = self.c_record_field_type(&field.ty);
                                    let value = elements
                                        .get(i)
                                        .and_then(|e| {
                                            self.render_const_value_expr(&field_ty, e, facts)
                                        })
                                        .unwrap_or_else(|| self.default_value_expr(&field_ty));
                                    (field.name.clone(), value)
                                })
                                .collect();
                            return Some(Expr::StructLit {
                                name: record_lit_name(record),
                                fields,
                            });
                        }
                        let fields = record
                            .fields
                            .iter()
                            .enumerate()
                            .map(|(i, field)| {
                                let field_ty = self.c_record_field_type(&field.ty);
                                let value = elements
                                    .get(i)
                                    .and_then(|e| self.render_const_value_expr(&field_ty, e, facts))
                                    .unwrap_or_else(|| self.default_value_expr(&field_ty));
                                (sanitize_ident(&field.name).into_string(), value)
                            })
                            .collect();
                        Some(wrap_record_lit(
                            record,
                            Expr::StructLit {
                                name: record_lit_name(record),
                                fields,
                            },
                        ))
                    }
                    RecordKind::Union => {
                        let storage_fields = self.bitfield_storage_fields(record);
                        let field = storage_fields
                            .as_ref()
                            .and_then(|fields| fields.first())
                            .or_else(|| record.fields.first())?;
                        let field_ty = self.c_record_field_type(&field.ty);
                        let value = elements
                            .first()
                            .and_then(|e| self.render_const_value_expr(&field_ty, e, facts))
                            .unwrap_or_else(|| self.default_value_expr(&field_ty));
                        Some(wrap_record_lit(
                            record,
                            Expr::StructLit {
                                name: record_lit_name(record),
                                fields: vec![(sanitize_ident(&field.name).into_string(), value)],
                            },
                        ))
                    }
                }
            }
            Attr::ConstArray { elts, .. } => {
                let bytes = attr_array_values(elts)?;
                let Type::Array { elem, len } = ty else {
                    return None;
                };
                let bytes = bytes
                    .iter()
                    .map(|value| value.as_int().and_then(|value| u8::try_from(value).ok()))
                    .collect::<Option<Vec<u8>>>()?;
                let elems = byte_array_elems(&bytes, ty);
                Some(render_array_literal_expr(
                    &elems,
                    *len as usize,
                    self.default_value_expr(elem),
                ))
            }
            Attr::ConstVector { elts, .. } => Some(Expr::ArrayLit(
                attr_array_values(elts)?
                    .iter()
                    .map(scalar_attr_expr)
                    .collect::<Option<Vec<_>>>()?,
            )),
            Attr::Zero { .. } => Some(self.default_value_expr(ty)),
            Attr::Dialect {
                dialect,
                mnemonic,
                raw: Some(raw),
                ..
            } if dialect == "cir" && mnemonic == "ptr" => {
                let raw = raw.trim();
                if raw == "null" {
                    Some(if matches!(ty, Type::FnPtr { .. }) {
                        Expr::Value(RustValue::None)
                    } else {
                        Expr::Value(RustValue::NullPtr)
                    })
                } else {
                    // `#cir.ptr<N : ty>` embeds its own type suffix inside the
                    // body text, unlike most other `#cir.*` attrs.
                    let digits = raw.split(':').next().unwrap_or(raw).trim();
                    digits.parse::<i128>().ok().map(|addr| Expr::Cast {
                        expr: Box::new(int_value_expr(addr)),
                        ty: ty.clone(),
                    })
                }
            }
            Attr::Int { value, .. } if matches!(ty, Type::Custom(name) if self.enums.contains_key(name)) =>
            {
                let Type::Custom(name) = ty else {
                    unreachable!()
                };
                let enm = self.enums.get(name)?;
                let variant = enm
                    .variants
                    .iter()
                    .find(|variant| i128::from(variant.value) == *value)?;
                Some(Expr::Path(Path::new([
                    Ident::from(name.as_str()),
                    Ident::from(sanitize_ident(&variant.name).as_str()),
                ])))
            }
            Attr::GlobalView {
                symbol, indices, ..
            } => {
                let indices = match indices.as_deref() {
                    Some(Attr::Array(values)) => values
                        .iter()
                        .map(Attr::as_int)
                        .collect::<Option<Vec<_>>>()?,
                    _ => Vec::new(),
                };
                self.global_view_init_expr(symbol, &indices, ty)
            }
            _ if matches!(ty, Type::LongDouble) => {
                let fp_text = match attr {
                    Attr::Float { text, .. } => Some(text.as_str()),
                    _ => None,
                };
                let fact = facts.pop_front();
                if let Some(fact) = fact {
                    if fact.bit_width == 80 && !fact.bits.is_empty() {
                        let bits = fact.bits.trim_start_matches("0x").trim_start_matches("0X");
                        f80_literal_bits_expr(bits)
                            .or_else(|| fp_text.and_then(long_double_from_text))
                    } else if !fact.value.is_empty() {
                        f80_literal_expr(&fact.value)
                            .or_else(|| fp_text.and_then(long_double_from_text))
                    } else {
                        fp_text.and_then(long_double_from_text)
                    }
                } else {
                    fp_text.and_then(long_double_from_text)
                }
            }
            Attr::Float { text, .. } => Some(typed_fp_literal_expr(Some(ty), fp_text_value(text)?)),
            _ => scalar_attr_expr(attr),
        }
    }

    fn global_view_init_expr(&self, target: &str, indices: &[i128], ty: &Type) -> Option<Expr> {
        if let Some(bytes) = self.strings.get(target) {
            let ptr_expr = Expr::MethodCall {
                recv: Box::new(Expr::ByteStr(bytes.clone())),
                method: "as_ptr".into(),
                args: Vec::new(),
            };
            let ptr_expr = match indices {
                [] => ptr_expr,
                [offset] => Expr::Unsafe(Box::new(crate::backend::rust_ast::Block {
                    stmts: Vec::new(),
                    tail: Some(Box::new(Expr::MethodCall {
                        recv: Box::new(ptr_expr),
                        method: "add".into(),
                        args: vec![int_value_expr(*offset)],
                    })),
                })),
                _ => return None,
            };
            return Some(Expr::Cast {
                expr: Box::new(ptr_expr),
                ty: ty.clone(),
            });
        }
        if matches!(ty, Type::FnPtr { .. }) {
            if let Some(wrapped) = self.enum_return_mismatch_wrap(target, ty) {
                return Some(wrapped);
            }
            let raw_ptr = Type::Ptr {
                mutable: false,
                inner: Box::new(Type::Unit),
            };
            return Some(Expr::Transmute {
                from: raw_ptr.clone(),
                to: ty.clone(),
                expr: Box::new(Expr::Cast {
                    expr: Box::new(Expr::Var(self.rust_global_name(target).into())),
                    ty: raw_ptr,
                }),
            });
        }
        let Type::Ptr { mutable, .. } = ty else {
            return None;
        };
        if self.function_return_types.contains_key(target) {
            return Some(Expr::Cast {
                expr: Box::new(Expr::Cast {
                    expr: Box::new(Expr::Var(self.rust_global_name(target).into())),
                    ty: Type::Ptr {
                        mutable: false,
                        inner: Box::new(Type::Unit),
                    },
                }),
                ty: ty.clone(),
            });
        }
        if indices.is_empty() {
            return Some(Expr::Cast {
                expr: Box::new(Expr::AddrOf {
                    mutable: *mutable,
                    expr: Box::new(Expr::Var(self.rust_global_name(target).into())),
                }),
                ty: ty.clone(),
            });
        }
        let base_ty = self
            .global_sym_types
            .get(target)
            .map(|ty| self.rust_type(ty))?;
        let path = self.global_view_index_path(target, &base_ty, indices)?;
        let addr_expr = Expr::AddrOf {
            mutable: *mutable,
            expr: Box::new(path),
        };
        Some(Expr::Cast {
            expr: Box::new(Expr::Unsafe(Box::new(crate::backend::rust_ast::Block {
                stmts: Vec::new(),
                tail: Some(Box::new(addr_expr)),
            }))),
            ty: ty.clone(),
        })
    }

    fn global_view_index_path(
        &self,
        target: &str,
        base_ty: &Type,
        indices: &[i128],
    ) -> Option<Expr> {
        let raw_ptr_ty = Type::Ptr {
            mutable: true,
            inner: Box::new(base_ty.clone()),
        };
        let mut expr = Expr::Unary {
            op: UnaryOp::Deref,
            expr: Box::new(Expr::Cast {
                expr: Box::new(Expr::AddrOf {
                    mutable: true,
                    expr: Box::new(Expr::Var(self.rust_global_name(target).into())),
                }),
                ty: raw_ptr_ty,
            }),
        };
        let mut ty = base_ty.clone();
        for &index in indices {
            match ty {
                Type::Array { elem, .. } => {
                    expr = Expr::Index {
                        base: Box::new(expr),
                        index: Box::new(int_value_expr(index)),
                    };
                    ty = *elem;
                }
                Type::Custom(name) => {
                    let record = self.records.get(&name)?;
                    let field = record.fields.get(usize::try_from(index).ok()?)?;
                    expr = Expr::Field {
                        base: Box::new(expr),
                        field: sanitize_ident(&field.name).into_string(),
                    };
                    ty = self.record_field_type(&field.ty);
                }
                _ => return None,
            }
        }
        Some(expr)
    }
}

fn repair_extern_function_signature(decl: &mut ExternFnDecl, spelling: Option<&str>) {
    let mut params: Vec<Type> = decl.params.iter().map(|param| param.ty.clone()).collect();
    let mut ret = decl.ret.clone();
    if !repair_function_signature(spelling, &mut params, &mut ret) {
        return;
    }
    decl.params = params
        .into_iter()
        .enumerate()
        .map(|(index, ty)| FnParam {
            name: format!("_{index}"),
            mutable: false,
            ty,
        })
        .collect();
    decl.ret = ret;
}

pub fn is_clib_record_type(name: &str) -> bool {
    clib_record_type(name).is_some()
}

fn clib_record_type(name: &str) -> Option<CLibType> {
    let name = match name {
        "__mbstate_t" => "mbstate_t",
        _ => name,
    };
    CLIB_RECORD_TYPES
        .iter()
        .copied()
        .find(|ty| ty.c_name() == name)
}

fn c_va_list_shaped_type(ty: &crate::frontend::c_ast::CType, boxed: bool) -> Option<Type> {
    use crate::frontend::c_ast::CType;
    let value_ty = || {
        if boxed {
            Type::Custom("__SlateVaArgs".into())
        } else {
            Type::VaList
        }
    };
    let is_va_list_record =
        |ty: &CType| matches!(ty, CType::Record(name) if name == "__va_list_tag");
    if is_va_list_record(ty) {
        return Some(value_ty());
    }
    if let CType::Array(inner, Some(1)) = ty
        && is_va_list_record(inner)
    {
        return Some(value_ty());
    }
    if let CType::Array(inner, Some(len)) = ty
        && let Some(inner_ty) = c_va_list_shaped_type(inner, boxed)
    {
        return Some(Type::Array {
            elem: Box::new(inner_ty),
            len: *len,
        });
    }
    if let CType::Ptr(inner) = ty
        && let Some(inner_ty) = c_va_list_shaped_type(inner, boxed)
    {
        return Some(Type::Ptr {
            mutable: true,
            inner: Box::new(inner_ty),
        });
    }
    None
}

impl<'a> Lowerer<'a> {
    fn c_type_to_type(&self, ty: &crate::frontend::c_ast::CType) -> Type {
        c_type_to_type(ty, self.va_list_boxed)
    }

    fn c_record_field_type(&self, ty: &crate::frontend::c_ast::CType) -> Type {
        c_record_field_type(ty, self.va_list_boxed)
    }
}

fn c_type_to_type(ty: &crate::frontend::c_ast::CType, va_list_boxed: bool) -> Type {
    use crate::frontend::c_ast::CType;
    if let Some(va_list_ty) = c_va_list_shaped_type(ty, va_list_boxed) {
        return va_list_ty;
    }
    let ptr = |inner: &CType| Type::Ptr {
        mutable: true,
        inner: Box::new(c_type_to_type(inner, va_list_boxed)),
    };
    match ty {
        CType::Void => Type::Unit,
        CType::Bool => Type::Prim(Prim::Bool),
        CType::Int { signed, bits } => match (signed, bits) {
            (true, 8) => Type::Prim(Prim::I8),
            (false, 8) => Type::Prim(Prim::U8),
            (true, 16) => Type::Prim(Prim::I16),
            (false, 16) => Type::Prim(Prim::U16),
            (true, 32) => Type::Prim(Prim::I32),
            (false, 32) => Type::Prim(Prim::U32),
            (true, 64) => Type::Prim(Prim::I64),
            (false, 64) => Type::Prim(Prim::U64),
            (true, 128) => Type::Prim(Prim::I128),
            (false, 128) => Type::Prim(Prim::U128),
            (signed, bits) => bitint_type(*signed, *bits),
        },
        CType::Float { bits: 32 } => Type::Prim(Prim::F32),
        CType::Float { bits: 80 } if crate::cir::emit::uses_f64_long_double_abi() => {
            Type::Prim(Prim::F64)
        }
        CType::Float { bits: 80 } => Type::LongDouble,
        CType::Float { bits: 128 } => Type::Prim(Prim::F128),
        CType::Float { .. } => Type::Prim(Prim::F64),
        CType::Ptr(inner) if matches!(&**inner, CType::Void) => Type::Ptr {
            mutable: true,
            inner: Box::new(Type::CLib(CLibType::VOID)),
        },
        CType::Ptr(inner) => ptr(inner),
        CType::FuncPtr { ret, params } => Type::FnPtr {
            abi: Abi::C,
            params: params
                .iter()
                .map(|ty| c_type_to_type(ty, va_list_boxed))
                .collect(),
            ret: Box::new(c_type_to_type(ret, va_list_boxed)),
        },
        CType::Array(inner, Some(len)) => Type::Array {
            elem: Box::new(c_type_to_type(inner, va_list_boxed)),
            len: *len,
        },
        CType::Array(inner, None) => ptr(inner),
        CType::Record(name) if name.starts_with("__slate_bitfields::") => {
            Type::Custom(name.clone())
        }
        CType::Record(name) => clib_record_type(name)
            .map(Type::CLib)
            .unwrap_or_else(|| Type::Custom(rust_record_name(name))),
        CType::Enum(name) => Type::Custom(sanitize_ident(name).into_string()),
    }
}

fn c_record_field_type(ty: &crate::frontend::c_ast::CType, va_list_boxed: bool) -> Type {
    match ty {
        crate::frontend::c_ast::CType::Array(inner, None) => Type::Array {
            elem: Box::new(c_type_to_type(inner, va_list_boxed)),
            len: 0,
        },
        _ => c_type_to_type(ty, va_list_boxed),
    }
}

#[derive(Clone, Copy)]
struct CLayout {
    size: u64,
    align: u64,
}

fn layout_call(name: &str, ty: &Type) -> Expr {
    Expr::Call {
        binding: crate::function_identity::CallBinding::Generated,
        func: Box::new(Expr::Var(
            format!("std::mem::{name}::<{}>", ty.render()).into(),
        )),
        args: Vec::new(),
    }
}

fn c_layout(
    ty: &crate::frontend::c_ast::CType,
    records: &BTreeMap<String, crate::frontend::c_ast::Record>,
) -> Option<CLayout> {
    use crate::frontend::c_ast::CType;
    match ty {
        CType::Void => Some(CLayout { size: 0, align: 1 }),
        CType::Bool => Some(CLayout { size: 1, align: 1 }),
        CType::Int { bits, .. } => scalar_layout(*bits),
        CType::Float { bits: 80 } => Some(if crate::cir::emit::uses_f64_long_double_abi() {
            CLayout { size: 8, align: 8 }
        } else {
            CLayout {
                size: 16,
                align: 16,
            }
        }),
        CType::Float { bits } => scalar_layout(*bits),
        CType::Ptr(_) | CType::FuncPtr { .. } => Some(CLayout { size: 8, align: 8 }),
        CType::Array(elem, Some(len)) => {
            let elem = c_layout(elem, records)?;
            Some(CLayout {
                size: align_to(elem.size, elem.align) * len,
                align: elem.align,
            })
        }
        CType::Array(elem, None) => {
            let elem = c_layout(elem, records)?;
            Some(CLayout {
                size: 0,
                align: elem.align,
            })
        }
        CType::Record(name) => record_layout(name, records),
        CType::Enum(_) => scalar_layout(32),
    }
}

fn scalar_layout(bits: u32) -> Option<CLayout> {
    let bytes = u64::from(bits).checked_div(8)?;
    let bytes = bytes.max(1);
    Some(CLayout {
        size: bytes,
        align: bytes,
    })
}

fn record_layout(
    name: &str,
    records: &BTreeMap<String, crate::frontend::c_ast::Record>,
) -> Option<CLayout> {
    let record = records.get(&sanitize_ident(name).into_string())?;
    let natural_align = record_natural_align(record, records)?;
    let align = record
        .align
        .map(u64::from)
        .unwrap_or(natural_align)
        .max(natural_align);
    match record.kind {
        RecordKind::Struct => {
            let mut offset = 0;
            for field in &record.fields {
                let field_layout = c_layout(&field.ty, records)?;
                let field_align = record.packed.map_or(field_layout.align, |packed| {
                    field_layout.align.min(u64::from(packed))
                });
                offset = align_to(offset, field_align);
                offset += field_layout.size;
            }
            Some(CLayout {
                size: align_to(offset, align),
                align,
            })
        }
        RecordKind::Union => {
            let size = record
                .fields
                .iter()
                .filter_map(|field| c_layout(&field.ty, records).map(|layout| layout.size))
                .max()
                .unwrap_or(0);
            Some(CLayout {
                size: align_to(size, align),
                align,
            })
        }
    }
}

fn record_natural_align(
    record: &crate::frontend::c_ast::Record,
    records: &BTreeMap<String, crate::frontend::c_ast::Record>,
) -> Option<u64> {
    record
        .fields
        .iter()
        .map(|field| {
            c_layout(&field.ty, records).map(|layout| {
                record
                    .packed
                    .map_or(layout.align, |packed| layout.align.min(u64::from(packed)))
            })
        })
        .max()
        .unwrap_or(Some(1))
}

fn record_field_offset(
    record_name: &str,
    field_name: &str,
    records: &BTreeMap<String, crate::frontend::c_ast::Record>,
) -> Option<u64> {
    let record = records.get(&sanitize_ident(record_name).into_string())?;
    if record.kind != RecordKind::Struct {
        return None;
    }
    let mut offset = 0;
    for field in &record.fields {
        let field_layout = c_layout(&field.ty, records)?;
        let field_align = record.packed.map_or(field_layout.align, |packed| {
            field_layout.align.min(u64::from(packed))
        });
        offset = align_to(offset, field_align);
        if field.name == field_name {
            return Some(offset);
        }
        offset += field_layout.size;
    }
    None
}

fn align_to(value: u64, align: u64) -> u64 {
    if align <= 1 {
        value
    } else {
        value.div_ceil(align) * align
    }
}

fn ctype_uses_long_double(ty: &crate::frontend::c_ast::CType) -> bool {
    use crate::frontend::c_ast::CType;
    match ty {
        CType::Float { bits: 80 } => !crate::cir::emit::uses_f64_long_double_abi(),
        CType::Ptr(inner) | CType::Array(inner, _) => ctype_uses_long_double(inner),
        CType::FuncPtr { ret, params } => {
            ctype_uses_long_double(ret) || params.iter().any(ctype_uses_long_double)
        }
        CType::Enum(_) => false,
        _ => false,
    }
}

impl<'a, 'b> FunctionLowerer<'a, 'b> {
    fn ast_floating_literal(&self, op: &Op) -> Option<FloatingLiteralFact> {
        self.parent.ast_floating_literal(op)
    }

    fn lower_block(&mut self, block: &Block) {
        let mut index = 0;
        while index < block.ops.len() {
            let op = &block.ops[index];
            if op.kind() == CirOpKind::Alloca {
                let group_loc = op
                    .loc
                    .as_ref()
                    .and_then(|raw| self.parent.resolve_source_point_value(raw));
                let end = block.ops[index..]
                    .iter()
                    .take_while(|candidate| {
                        let candidate_group_loc = candidate
                            .loc
                            .as_ref()
                            .and_then(|raw| self.parent.resolve_source_point_value(raw));
                        candidate.kind() == CirOpKind::Alloca
                            && (candidate.loc == op.loc
                                || matches!(
                                    (&group_loc, candidate_group_loc),
                                    (Some(group_loc), Some(candidate_group_loc))
                                        if &candidate_group_loc == group_loc
                                ))
                    })
                    .count()
                    + index;
                if end - index > 1 && self.alloca_group_is_lowerable(&block.ops[index..end]) {
                    self.lower_alloca_group(&block.ops[index..end]);
                    index = end;
                    continue;
                }
            }
            self.asm_output_places.clear();
            if op.kind() == CirOpKind::Asm
                && attr_str(op, "asm_string").is_some_and(asm_template_has_labels)
            {
                for (result, _) in &op.results {
                    if let Some(store) = block.ops[index + 1..].iter().find(|candidate| {
                        candidate.kind() == CirOpKind::Store
                            && candidate.operands.first() == Some(result)
                    }) && let Some(pointer) = store.operands.get(1)
                        && let Some(place) = self.place_expr(pointer)
                    {
                        self.asm_output_places.insert(result.clone(), place);
                    }
                }
            }
            self.lower_op(op);
            self.force_cross_block_materialization(op);
            index += 1;
        }
    }

    fn force_cross_block_materialization(&mut self, op: &Op) {
        if self.dispatch.is_none() {
            return;
        }
        for (result, _) in &op.results {
            let Some(name) = self
                .dispatch
                .as_ref()
                .unwrap()
                .cross_block_names
                .get(result)
                .cloned()
            else {
                continue;
            };
            let already_materialized = matches!(
                self.values.get(result),
                Some(Val::Expr(Expr::Var(v))) if v.as_str() == name
            );
            if already_materialized {
                continue;
            }
            let Some(Val::Expr(current)) = self.values.get(result).cloned() else {
                continue;
            };
            let ty = op_result_type(op)
                .map(|ty| self.parent.rust_type(ty))
                .unwrap_or(Type::Prim(Prim::I32));
            let default = self.parent.default_value_expr(&ty);
            self.dispatch
                .as_mut()
                .unwrap()
                .pending_hoists
                .push(Self::indent_stmt(Stmt::Let {
                    name: name.clone(),
                    mutable: true,
                    ty: Some(ty),
                    init: Some(default),
                }));
            self.push_stmt(Self::assign_stmt(Expr::Var(name.clone().into()), current));
            self.values
                .insert(result.clone(), Val::Expr(Expr::Var(name.into())));
        }
    }

    fn lower_region_ops(&mut self, region: &Region) {
        for block in &region.blocks {
            self.lower_block(block);
        }
    }

    fn lower_op(&mut self, op: &Op) {
        if let Some(typed) = TypedOp::from_operation(op) {
            match typed {
                TypedOp::Add(_) => return self.lower_int_arith(op, BinOp::Add),
                TypedOp::Sub(_) => return self.lower_int_arith(op, BinOp::Sub),
                TypedOp::Mul(_) => return self.lower_int_arith(op, BinOp::Mul),
                TypedOp::Div(_) => return self.lower_int_arith(op, BinOp::Div),
                TypedOp::Rem(_) => return self.lower_int_arith(op, BinOp::Rem),
                TypedOp::And(_) => return self.lower_int_arith(op, BinOp::BitAnd),
                TypedOp::Or(_) => return self.lower_int_arith(op, BinOp::BitOr),
                TypedOp::Xor(_) => return self.lower_int_arith(op, BinOp::BitXor),
                TypedOp::Fadd(_) => return self.lower_binary(op, BinOp::Add),
                TypedOp::Fsub(_) => return self.lower_binary(op, BinOp::Sub),
                TypedOp::Fmul(_) => return self.lower_binary(op, BinOp::Mul),
                TypedOp::Fdiv(_) => return self.lower_binary(op, BinOp::Div),
                TypedOp::Inc(_) => return self.lower_step(op, BinOp::Add),
                TypedOp::Dec(_) => return self.lower_step(op, BinOp::Sub),
                TypedOp::Not(_) => return self.lower_not(op),
                TypedOp::Shift(value) => {
                    return self.lower_int_arith(
                        op,
                        if value.is_shiftleft {
                            BinOp::Shl
                        } else {
                            BinOp::Shr
                        },
                    );
                }
                TypedOp::AddOverflow(_) => return self.lower_overflow_arith(op, "overflowing_add"),
                TypedOp::SubOverflow(_) => return self.lower_overflow_arith(op, "overflowing_sub"),
                TypedOp::MulOverflow(_) => return self.lower_overflow_arith(op, "overflowing_mul"),
                TypedOp::Const(_) => return self.lower_const(op),
                TypedOp::GetGlobal(_) => return self.lower_get_global(op),
                TypedOp::Load(_) => return self.lower_load(op),
                TypedOp::Store(_) => return self.lower_store(op),
                TypedOp::Copy(_) => return self.lower_copy(op),
                TypedOp::Cast(_) => return self.lower_cast(op),
                TypedOp::GetElement(_) => return self.lower_get_element(op),
                TypedOp::PtrStride(_) => return self.lower_ptr_stride(op),
                TypedOp::PtrDiff(_) => return self.lower_ptr_diff(op),
                TypedOp::Cmp(_) => return self.lower_cmp(op),
                TypedOp::Select(_) => return self.lower_select(op),
                _ => {}
            }
        }
        match op.kind() {
            CirOpKind::Alloca => self.lower_alloca(op),
            CirOpKind::Asm => self.lower_asm(op),
            CirOpKind::Acos => self.lower_unary_method(op, "acos"),
            CirOpKind::Asin => self.lower_unary_method(op, "asin"),
            CirOpKind::Atan => self.lower_unary_method(op, "atan"),
            CirOpKind::Atan2 => self.lower_binary_method(op, "atan2"),
            CirOpKind::Cos => self.lower_known_unary_method(op, Known::Cos, "cos"),
            CirOpKind::Exp => self.lower_known_unary_method(op, Known::Exp, "exp"),
            CirOpKind::Exp2 => self.lower_known_unary_method(op, Known::Exp2, "exp2"),
            CirOpKind::Expect => self.lower_expect(op),
            CirOpKind::Fmaximum => self.lower_binary_method(op, "max"),
            CirOpKind::Fminimum => self.lower_binary_method(op, "min"),
            CirOpKind::Fmod => self.lower_known_binary(op, Known::Fmod, BinOp::Rem),
            CirOpKind::Llrint => self.lower_unary_cast_method(op, "round_ties_even"),
            CirOpKind::Llround => self.lower_known_unary_cast_method(op, Known::Llround, "round"),
            CirOpKind::Log => self.lower_known_unary_method(op, Known::Log, "ln"),
            CirOpKind::Log10 => self.lower_known_unary_method(op, Known::Log10, "log10"),
            CirOpKind::Log2 => self.lower_known_unary_method(op, Known::Log2, "log2"),
            CirOpKind::Lrint => self.lower_unary_cast_method(op, "round_ties_even"),
            CirOpKind::Lround => self.lower_known_unary_cast_method(op, Known::Lround, "round"),
            CirOpKind::Pow => self.lower_known_binary_method(op, Known::Pow, "powf"),
            CirOpKind::Roundeven => self.lower_unary_method(op, "round_ties_even"),
            CirOpKind::Sin => self.lower_known_unary_method(op, Known::Sin, "sin"),
            CirOpKind::Sqrt => self.lower_known_unary_method(op, Known::Sqrt, "sqrt"),
            CirOpKind::Stackrestore => {}
            CirOpKind::Tan => self.lower_known_unary_method(op, Known::Tan, "tan"),
            CirOpKind::Trap => self.lower_trap(),
            CirOpKind::Unreachable => self.lower_unreachable(),
            CirOpKind::ComplexMul => self.lower_complex_mul(op),
            CirOpKind::ComplexDiv => self.lower_complex_div(op),
            CirOpKind::ComplexConj => self.lower_complex_conj(op),
            CirOpKind::Ternary => self.lower_ternary(op),
            CirOpKind::GetMember => self.lower_get_member(op),
            CirOpKind::InsertMember => self.lower_insert_member(op),
            CirOpKind::Call => self.lower_call(op),
            CirOpKind::VaStart => self.lower_va_start(op),
            CirOpKind::VaArg => self.lower_va_arg(op),
            CirOpKind::VaCopy => self.lower_va_copy(op),
            CirOpKind::CallLlvmIntrinsic => self.lower_llvm_intrinsic(op),
            CirOpKind::Return => self.lower_return(op),
            CirOpKind::Scope => self.lower_scope(op),
            CirOpKind::CleanupScope => self.lower_cleanup_scope(op),
            CirOpKind::Switch => self.lower_switch(op),
            CirOpKind::SwitchFlat => self.lower_switch_flat(op),
            CirOpKind::For => self.lower_for(op),
            CirOpKind::While => self.lower_while(op),
            CirOpKind::Do => self.lower_do(op),
            CirOpKind::Break => self.lower_break(),
            CirOpKind::Continue => self.lower_continue(),
            CirOpKind::Goto => self.lower_goto(op),
            CirOpKind::Br => self.lower_br(op),
            CirOpKind::Brcond => self.lower_brcond(op),
            CirOpKind::IndirectBr => self.lower_indirect_br(op),
            CirOpKind::Label => {}
            CirOpKind::Yield | CirOpKind::Condition => {}
            _ => {
                let name = op.name.as_str();
                self.parent
                    .ctx
                    .diagnostics
                    .warn(format!("lower: unsupported CIR op {name}"));
                self.emit_todo(name);
            }
        }
    }
}
