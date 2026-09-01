use super::*;

pub(super) fn attr_array_values(attr: &Attr) -> Option<&[Attr]> {
    match attr {
        Attr::Array(values) => Some(values),
        _ => None,
    }
}

pub(super) fn source_point(loc: &SourceLocation) -> Option<SourcePoint> {
    match loc {
        SourceLocation::File { file, line, column } => Some(SourcePoint {
            file: file.clone(),
            line: *line as u64,
            col: *column as u64,
        }),
        SourceLocation::Fused(locations) => locations.iter().find_map(source_point),
        SourceLocation::Callsite { expansion, .. } => source_point(expansion),
        SourceLocation::Loc(_) | SourceLocation::Unknown => None,
    }
}

pub(super) fn rust_record_name(name: &str) -> String {
    sanitize_ident(name).into_string()
}

pub(super) fn lowered_record_name(name: &str) -> Option<String> {
    (name != "__va_list_tag" && clib_record_type(name).is_none()).then(|| rust_record_name(name))
}

pub(super) fn collect_record_dependencies(
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

pub(super) fn widen_flexible_array_members(
    cir: &Module,
    records: &mut BTreeMap<String, crate::frontend::c_ast::Record>,
) {
    for global in &cir.globals {
        let expanded = cir.resolve_type(&global.ty);
        let Some(record_name) = slate_record_name(expanded) else {
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
        let Some(len) = global.initial_value.as_ref().and_then(|attr| match attr {
            Attr::ConstRecord { members, .. } => {
                attr_array_values(members)?
                    .last()
                    .and_then(|elem| match elem {
                        Attr::ConstArray { ty, .. } | Attr::Zero { ty } => {
                            ty.as_array().map(|(_, len)| len)
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

pub(super) fn required_record_defs(
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
                lowered_record_name(&canonical_alias_record_name(alias, ty, &cir.type_aliases)?)?;
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

fn collect_referenced_record_names<'a>(ty: &'a CirType, out: &mut BTreeSet<&'a str>) {
    if let Some(name) = slate_record_name(ty) {
        out.insert(name);
    }
    if let Some((element, _)) = ty.as_array() {
        collect_referenced_record_names(element, out);
    }
    if let Some(pointee) = ty.pointee() {
        collect_referenced_record_names(pointee, out);
    }
}

pub fn shim_records_for_module(cir: &Module, c: &Unit) -> Vec<crate::frontend::c_ast::Record> {
    let mut referenced: BTreeSet<&str> = BTreeSet::new();
    for ty in cir.type_aliases.values() {
        collect_referenced_record_names(ty, &mut referenced);
    }
    c.named_header_records
        .iter()
        .chain(&c.anonymous_header_records)
        .filter(|&record| referenced.contains(record.name.as_str()))
        .cloned()
        .collect()
}

pub(super) fn typed_linkage_is_external(linkage: GlobalLinkageKind) -> bool {
    matches!(
        linkage,
        GlobalLinkageKind::External | GlobalLinkageKind::WeakAny
    )
}

pub(super) fn typed_global_is_exported(global: &CirGlobal) -> bool {
    typed_linkage_is_external(global.linkage) && global.visibility != Some(VisibilityKind::Hidden)
}

pub(super) fn typed_function_is_exported(function: &CirFunction) -> bool {
    typed_linkage_is_external(function.linkage)
        && function.visibility != Some(VisibilityKind::Hidden)
}

pub(super) fn symbol_attrs(
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

pub(super) fn insert_crate_feature(items: &mut [Item], feature: Feature) {
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

pub(super) fn region_ends_in_noreturn_call(region: &inst::Region) -> bool {
    if region.blocks.len() != 1 {
        return false;
    }
    let Some(block) = region.blocks.first() else {
        return false;
    };
    let ops: &[Op] = match block.ops.last() {
        Some(Op::Return(_) | Op::Yield(_)) => &block.ops[..block.ops.len() - 1],
        _ => &block.ops,
    };
    match ops.last() {
        Some(Op::Call(call)) => call.noreturn,
        Some(Op::Scope(scope)) => region_ends_in_noreturn_call(&scope.scope_region),
        _ => false,
    }
}

pub(super) fn function_requires_unsafe_contract(function: &CirFunction) -> bool {
    if !function
        .params
        .iter()
        .any(|(_, ty)| matches!(ty, CirType::Pointer { .. }))
    {
        return false;
    }
    let Some(body) = &function.body else {
        return false;
    };
    let mut local_ptrs = BTreeSet::new();
    let mut unsafe_access = false;
    walk_region_ops(body, &mut |op| {
        match op {
            Op::Alloca(alloca) => {
                local_ptrs.insert(alloca.addr.clone());
            }
            Op::GetMember(member) if local_ptrs.contains(&member.addr) => {
                local_ptrs.insert(member.result.clone());
            }
            Op::GetElement(element) if local_ptrs.contains(&element.base) => {
                local_ptrs.insert(element.result.clone());
            }
            Op::Load(load) if !local_ptrs.contains(&load.addr) => unsafe_access = true,
            Op::Store(store) if !local_ptrs.contains(&store.addr) => unsafe_access = true,
            _ => {}
        }
        !unsafe_access
    });
    unsafe_access
}

/// Extracts `#cir.block_addr_info<@func, "label">` labels from a
/// `#cir.const_array<[...]>` element list, if every element carries one -
/// anything else (a plain numeric/aggregate array) isn't a block-address
/// table.
pub(super) fn block_addr_labels(items: &[Attr]) -> Option<Vec<String>> {
    let labels: Vec<String> = items
        .iter()
        .filter_map(|item| match item {
            Attr::BlockAddrInfo { label, .. } => Some(label.clone()),
            Attr::BlockAddrDiff { lhs_label, .. } => Some(lhs_label.clone()),
            _ => None,
        })
        .collect();
    (labels.len() == items.len() && !labels.is_empty()).then_some(labels)
}

pub fn defined_functions(module: &Module) -> Vec<String> {
    module
        .functions
        .iter()
        .filter(|function| {
            typed_linkage_is_external(function.linkage)
                && (!function.is_declaration()
                    || function.aliasee.is_some() && function.linkage != GlobalLinkageKind::WeakAny)
        })
        .map(|function| function.name.clone())
        .collect()
}

pub fn address_taken_functions(module: &Module) -> BTreeSet<String> {
    c_abi_function_targets(module)
}

pub fn declared_functions(module: &Module) -> Vec<String> {
    module
        .functions
        .iter()
        .filter(|function| function.is_declaration())
        .map(|function| function.name.clone())
        .collect()
}

pub fn defined_globals(module: &Module) -> Vec<String> {
    module
        .globals
        .iter()
        .filter(|global| {
            global.initial_value.is_some() && typed_linkage_is_external(global.linkage)
        })
        .map(|global| sanitize_ident(&global.name).into_string())
        .collect()
}

pub fn declared_globals(module: &Module) -> Vec<String> {
    module
        .globals
        .iter()
        .filter(|global| global.is_declaration())
        .map(|global| sanitize_ident(&global.name).into_string())
        .collect()
}

pub(super) fn allocate_global_rust_names(
    module: &Module,
    records: &BTreeMap<String, crate::frontend::c_ast::Record>,
    enums: &BTreeMap<String, crate::frontend::c_ast::Enum>,
) -> BTreeMap<String, String> {
    if module.functions.is_empty() && module.globals.is_empty() {
        return BTreeMap::new();
    }
    let symbols: BTreeSet<String> = module
        .functions
        .iter()
        .map(|function| function.name.as_str())
        .chain(module.globals.iter().map(|global| global.name.as_str()))
        .map(|name| sanitize_ident(name).into_string())
        .collect();
    let type_names: BTreeSet<String> = records.keys().chain(enums.keys()).cloned().collect();
    let mut used = symbols.clone();
    used.extend(type_names.iter().cloned());
    let globals: BTreeSet<String> = module
        .globals
        .iter()
        .map(|global| global.name.clone())
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
    for global in &module.globals {
        let Some(target) = global.aliasee.as_deref() else {
            continue;
        };
        let target_name = names
            .get(target)
            .cloned()
            .unwrap_or_else(|| sanitize_ident(target).into_string());
        names.insert(global.name.clone(), target_name);
    }
    names
}

pub fn unsafe_defined_functions(module: &Module) -> BTreeSet<String> {
    let mut unsafe_functions: BTreeSet<String> = module
        .functions
        .iter()
        .filter(|function| {
            !function.is_declaration() && typed_linkage_is_external(function.linkage)
        })
        .filter(|function| function.varargs || function_requires_unsafe_contract(function))
        .map(|function| function.name.clone())
        .filter(|name| name != "main")
        .collect();
    for function in &module.functions {
        if !function.is_declaration() {
            continue;
        }
        let Some(target) = function.aliasee.as_deref() else {
            continue;
        };
        if unsafe_functions.contains(target) {
            unsafe_functions.insert(function.name.clone());
        }
    }
    unsafe_functions
}

pub fn target_feature_functions(module: &Module) -> BTreeMap<String, Vec<String>> {
    let mut baseline: Option<BTreeSet<&str>> = None;
    for function in &module.functions {
        if function.target_features.is_empty() {
            continue;
        }
        let enabled: BTreeSet<&str> = function
            .target_features
            .iter()
            .filter(|feature| feature.enabled)
            .map(|feature| feature.name.as_str())
            .collect();
        baseline = Some(match baseline {
            Some(existing) => existing.intersection(&enabled).copied().collect(),
            None => enabled,
        });
    }
    let baseline = baseline.unwrap_or_default();
    module
        .functions
        .iter()
        .filter_map(|function| {
            let extra: BTreeSet<String> = function
                .target_features
                .iter()
                .filter(|feature| feature.enabled && !baseline.contains(feature.name.as_str()))
                .map(|feature| rustc_target_feature_name(&feature.name).to_string())
                .collect();
            (!extra.is_empty()).then(|| (function.name.clone(), extra.into_iter().collect()))
        })
        .collect()
}

pub(super) fn rustc_target_feature_name(name: &str) -> &str {
    match name {
        "crc32" => "sse4.2",
        "bmi" => "bmi1",
        other => other,
    }
}

pub fn always_inline_functions(module: &Module) -> BTreeSet<String> {
    module
        .functions
        .iter()
        .filter(|function| function.inline_kind == Some(clang_ir::enums::InlineKind::AlwaysInline))
        .map(|function| function.name.clone())
        .collect()
}

pub(super) fn cir_type_mentions(ty: &CirType, is_target: &impl Fn(&CirType) -> bool) -> bool {
    if is_target(ty) {
        return true;
    }
    match ty {
        CirType::LongDouble { underlying: inner }
        | CirType::Pointer { pointee: inner, .. }
        | CirType::Complex {
            element_type: inner,
        } => cir_type_mentions(inner, is_target),
        CirType::Array {
            element_type: element,
            ..
        }
        | CirType::Vector {
            element_type: element,
            ..
        } => cir_type_mentions(element, is_target),
        CirType::Func {
            inputs,
            optional_return_type,
            ..
        } => {
            inputs.iter().any(|ty| cir_type_mentions(ty, is_target))
                || optional_return_type
                    .as_deref()
                    .is_some_and(|ty| cir_type_mentions(ty, is_target))
        }
        CirType::FunctionType { inputs, results } => {
            inputs.iter().any(|ty| cir_type_mentions(ty, is_target))
                || results.iter().any(|ty| cir_type_mentions(ty, is_target))
        }
        CirType::Struct {
            members: Some(members),
            ..
        }
        | CirType::Union {
            members: Some(members),
            ..
        } => members.iter().any(|ty| cir_type_mentions(ty, is_target)),
        _ => false,
    }
}

pub(super) fn cir_type_mentions_f128(ty: &CirType) -> bool {
    cir_type_mentions(ty, &|ty| matches!(ty, CirType::Fp128))
}

pub(super) fn cir_type_mentions_f16(ty: &CirType) -> bool {
    cir_type_mentions(ty, &|ty| matches!(ty, CirType::Fp16))
}

pub(super) fn attr_mentions(attr: &Attr, is_target: &impl Fn(&CirType) -> bool) -> bool {
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
        | Attr::Poison { ty } => cir_type_mentions(ty, is_target),
        Attr::Dialect { ty: Some(ty), .. } => cir_type_mentions(ty, is_target),
        Attr::BitfieldInfo { storage_type, .. } => cir_type_mentions(storage_type, is_target),
        Attr::ConstComplex { real, imag, .. } => {
            attr_mentions(real, is_target) || attr_mentions(imag, is_target)
        }
        Attr::Array(items) => items.iter().any(|item| attr_mentions(item, is_target)),
        Attr::Dict(entries) => entries.iter().any(|(_, v)| attr_mentions(v, is_target)),
        _ => false,
    }
}

pub(super) fn attr_mentions_f128(attr: &Attr) -> bool {
    attr_mentions(attr, &|ty| matches!(ty, CirType::Fp128))
}

pub(super) fn attr_mentions_f16(attr: &Attr) -> bool {
    attr_mentions(attr, &|ty| matches!(ty, CirType::Fp16))
}

pub fn required_features(module: &Module) -> BTreeSet<Feature> {
    let mut features = BTreeSet::new();
    for function in &module.functions {
        if cir_type_mentions_f128(&function.return_ty)
            || function
                .params
                .iter()
                .any(|(_, ty)| cir_type_mentions_f128(ty))
        {
            features.insert(Feature::F128);
        }
        if cir_type_mentions_f16(&function.return_ty)
            || function
                .params
                .iter()
                .any(|(_, ty)| cir_type_mentions_f16(ty))
        {
            features.insert(Feature::F16);
        }
        if function.linkage == GlobalLinkageKind::WeakAny && function.aliasee.is_none() {
            features.insert(Feature::Linkage);
        }
        if function.varargs
            || function
                .params
                .iter()
                .any(|(_, ty)| function_type_contains_va_list(ty))
            || function_type_contains_va_list(&function.return_ty)
        {
            features.insert(Feature::CVariadic);
        }
        if let Some(body) = &function.body {
            let mut has_llvm_intrinsic = false;
            walk_region_ops(body, &mut |op| {
                has_llvm_intrinsic |= matches!(op, Op::CallLlvmIntrinsic(_));
                true
            });
            if has_llvm_intrinsic {
                features.insert(Feature::LinkLlvmIntrinsics);
                features.insert(Feature::AbiUnadjusted);
                features.insert(Feature::PortableSimd);
                features.insert(Feature::SimdFfi);
            }
        }
    }
    for global in &module.globals {
        if cir_type_mentions_f128(&global.ty)
            || global
                .initial_value
                .as_ref()
                .is_some_and(attr_mentions_f128)
        {
            features.insert(Feature::F128);
        }
        if cir_type_mentions_f16(&global.ty)
            || global.initial_value.as_ref().is_some_and(attr_mentions_f16)
        {
            features.insert(Feature::F16);
        }
        if global.linkage == GlobalLinkageKind::WeakAny {
            features.insert(Feature::Linkage);
        }
        if global.name == "llvm.used" {
            features.insert(Feature::UsedWithArg);
        }
    }
    for op in &module.other {
        if let Op::Asm(asm) = op
            && asm.res.is_some()
            && asm_template_has_labels(&asm.asm_string)
        {
            features.insert(Feature::AsmGotoWithOutputs);
        }
    }
    features
}
