use super::*;
use clang_ir::enums::RecordMemberKind as CirRecordMemberKind;

pub(super) fn reconcile_anonymous_member_types(
    module: &Module,
    records: &mut BTreeMap<String, crate::frontend::c_ast::Record>,
    anonymous_header_records: &[crate::frontend::c_ast::Record],
) -> Vec<crate::frontend::c_ast::Record> {
    use crate::frontend::c_ast::CType;

    let anonymous_header_records: BTreeMap<String, &crate::frontend::c_ast::Record> =
        anonymous_header_records
            .iter()
            .map(|record| (sanitize_ident(&record.name).into_string(), record))
            .collect();
    let mut reconciled = Vec::new();
    loop {
        let present_names: BTreeSet<String> = records.keys().cloned().collect();
        let bitfield_storage_names: BTreeSet<String> = records
            .iter()
            .filter(|(_, record)| {
                record
                    .fields
                    .iter()
                    .any(|field| field.name.starts_with("__bitfield_"))
            })
            .map(|(name, _)| name.clone())
            .collect();
        let mut additions = BTreeMap::new();
        for record in records.values_mut() {
            let Some(expanded) = module.generic.type_aliases.values().find_map(|expanded| {
                let (CirType::Struct { name, .. } | CirType::Union { name, .. }) = expanded else {
                    return None;
                };
                name.as_deref().filter(|name| {
                    sanitize_ident(name).as_str() == sanitize_ident(&record.name).as_str()
                })?;
                Some(expanded)
            }) else {
                continue;
            };
            let (CirType::Struct {
                members: Some(members),
                ..
            }
            | CirType::Union {
                members: Some(members),
                ..
            }) = expanded
            else {
                continue;
            };
            if members.len() != record.fields.len() {
                continue;
            }
            for (index, field) in record.fields.iter_mut().enumerate() {
                let cir_ty = cir_type_to_ctype(&members[index], &module.generic.type_aliases);
                if field.name == format!("__slate_anon_{index}") {
                    field.ty = cir_ty;
                    continue;
                }
                let (CType::Record(ast_name), CType::Record(cir_name)) = (&field.ty, &cir_ty)
                else {
                    continue;
                };
                let ast_name = sanitize_ident(ast_name).into_string();
                let cir_key = sanitize_ident(cir_name).into_string();
                if ast_name == cir_key {
                    continue;
                }
                if bitfield_storage_names.contains(&cir_key) {
                    field.ty = cir_ty;
                    continue;
                }
                if present_names.contains(&ast_name) {
                    continue;
                }
                let Some(header_record) = anonymous_header_records.get(&ast_name) else {
                    continue;
                };
                let mut header_record = (*header_record).clone();
                header_record.name = cir_name.clone();
                additions.entry(cir_key).or_insert(header_record);
                field.ty = cir_ty;
            }
        }
        if additions.is_empty() {
            break;
        }
        for (name, record) in additions {
            records.insert(name, record.clone());
            reconciled.push(record);
        }
    }
    reconciled
}

pub(super) fn collect_anon_record_info(
    body: &inst::Region,
    aliases: &BTreeMap<String, CirType>,
    needed: &mut BTreeSet<String>,
    field_names: &mut BTreeMap<(String, i64), String>,
) {
    walk_region_ops(body, &mut |op| {
        match op {
            Op::Alloca(op) => {
                if let Some(inner) = op.addr_ty.pointee() {
                    collect_anon_alias_keys(inner, aliases, needed);
                }
            }
            Op::GetMember(op) => {
                if let Some(pointee) = op.addr_ty.pointee()
                    && let Some(key) = anon_alias_key(pointee, aliases)
                {
                    needed.insert(key.to_string());
                    if let Some(index) = op.index_attr.as_int()
                        && !op.name.is_empty()
                    {
                        field_names.insert((key.to_string(), index as i64), op.name.clone());
                    }
                }
            }
            Op::CallLlvmIntrinsic(op) => {
                if let Some(ty) = &op.result_ty {
                    collect_anon_alias_keys(ty, aliases, needed);
                }
            }
            _ => {}
        }
        true
    });
}

pub(super) fn collect_anon_bitfield_slots(
    body: &inst::Region,
    aliases: &BTreeMap<String, CirType>,
    member_slots: &mut BTreeMap<String, (String, i64)>,
    bitfield_slots: &mut BTreeSet<(String, i64)>,
) {
    walk_region_ops(body, &mut |op| {
        match op {
            Op::GetMember(op)
                if let (Some(key), Some(index)) = (
                    op.addr_ty
                        .pointee()
                        .and_then(|pointee| anon_alias_key(pointee, aliases)),
                    op.index_attr.as_int(),
                ) =>
            {
                member_slots.insert(op.result.clone(), (key.to_string(), index as i64));
            }
            Op::GetBitfield(op) => {
                if let Some(slot) = member_slots.get(&op.addr) {
                    bitfield_slots.insert(slot.clone());
                }
            }
            Op::SetBitfield(op) => {
                if let Some(slot) = member_slots.get(&op.addr) {
                    bitfield_slots.insert(slot.clone());
                }
            }
            _ => {}
        }
        true
    });
}

fn normalize_record_shape(ty: &crate::frontend::c_ast::CType) -> crate::frontend::c_ast::CType {
    use crate::frontend::c_ast::CType;
    match ty {
        CType::Record(_) => CType::Record(String::new()),
        CType::Ptr(inner) => CType::Ptr(Box::new(normalize_record_shape(inner))),
        CType::Array(inner, len) => CType::Array(Box::new(normalize_record_shape(inner)), *len),
        CType::FuncPtr { ret, params } => CType::FuncPtr {
            ret: Box::new(normalize_record_shape(ret)),
            params: params.iter().map(normalize_record_shape).collect(),
        },
        other => other.clone(),
    }
}

fn record_field_shape(
    fields: &[crate::frontend::c_ast::Decl],
) -> Vec<crate::frontend::c_ast::CType> {
    fields
        .iter()
        .map(|f| normalize_record_shape(&f.ty))
        .collect()
}

pub(super) fn resolve_local_record_collisions(
    cir: &Module,
    ast_records: &[crate::frontend::c_ast::Record],
) -> BTreeMap<String, crate::frontend::c_ast::Record> {
    let mut by_name: BTreeMap<String, Vec<&crate::frontend::c_ast::Record>> = BTreeMap::new();
    for record in ast_records {
        by_name
            .entry(sanitize_ident(&record.name).into_string())
            .or_default()
            .push(record);
    }
    by_name.retain(|_, records| records.len() > 1);
    if by_name.is_empty() {
        return BTreeMap::new();
    }

    type CType = crate::frontend::c_ast::CType;
    type RecordAlias = (usize, String, Vec<CType>, Vec<CType>);
    let mut aliases_by_base: BTreeMap<String, Vec<RecordAlias>> = BTreeMap::new();
    for expanded in cir.generic.type_aliases.values() {
        let (CirType::Struct {
            name,
            members: Some(members),
            ..
        }
        | CirType::Union {
            name,
            members: Some(members),
            ..
        }) = expanded
        else {
            continue;
        };
        let Some(cir_name) = name.as_deref() else {
            continue;
        };
        let base_sanitized = sanitize_ident(cir_record_base_name(cir_name)).into_string();
        if !by_name.contains_key(&base_sanitized) {
            continue;
        }
        let raw_types: Vec<CType> = members
            .iter()
            .map(|member| cir_type_to_ctype(member, &cir.generic.type_aliases))
            .collect();
        let shape: Vec<CType> = raw_types.iter().map(normalize_record_shape).collect();
        aliases_by_base.entry(base_sanitized).or_default().push((
            cir_record_occurrence(cir_name),
            sanitize_ident(cir_name).into_string(),
            shape,
            raw_types,
        ));
    }

    let mut resolved = BTreeMap::new();
    for (base_sanitized, mut aliases) in aliases_by_base {
        aliases.sort_by_key(|(occurrence, ..)| *occurrence);
        let candidates = &by_name[&base_sanitized];
        let mut next_candidate = 0;
        for (_, rust_name, shape, raw_types) in aliases {
            let Some(offset) = candidates[next_candidate..]
                .iter()
                .position(|record| record_field_shape(&record.fields) == shape)
            else {
                continue;
            };
            let position = next_candidate + offset;
            let mut matched = candidates[position].clone();
            matched.name = rust_name.clone();
            for (field, ty) in matched.fields.iter_mut().zip(raw_types) {
                field.ty = ty;
            }
            resolved.insert(rust_name, matched);
            next_candidate = position + 1;
        }
    }
    resolved
}

pub(super) fn cir_record_base_name(name: &str) -> &str {
    match name.rsplit_once('.') {
        Some((base, suffix))
            if !suffix.is_empty() && suffix.bytes().all(|b| b.is_ascii_digit()) =>
        {
            base
        }
        _ => name,
    }
}

pub(super) fn cir_record_occurrence(name: &str) -> usize {
    match name.rsplit_once('.') {
        Some((_, suffix)) if !suffix.is_empty() && suffix.bytes().all(|b| b.is_ascii_digit()) => {
            suffix.parse::<usize>().map_or(0, |n| n + 1)
        }
        _ => 0,
    }
}

pub fn anon_local_records(module: &Module) -> Vec<crate::frontend::c_ast::Record> {
    let mut needed = BTreeSet::new();
    let mut field_names = BTreeMap::new();
    let mut bitfield_slots = BTreeSet::new();
    let mut member_slots = BTreeMap::new();
    for global in &module.globals {
        collect_anon_alias_keys(&global.ty, &module.generic.type_aliases, &mut needed);
    }
    for function in &module.functions {
        if let Some(body) = &function.body {
            collect_anon_bitfield_slots(
                body,
                &module.generic.type_aliases,
                &mut member_slots,
                &mut bitfield_slots,
            );
            collect_anon_record_info(
                body,
                &module.generic.type_aliases,
                &mut needed,
                &mut field_names,
            );
        }
    }

    let mut frontier: Vec<String> = needed.iter().cloned().collect();
    while let Some(key) = frontier.pop() {
        let members = match module.generic.type_aliases.get(&key) {
            Some(CirType::Struct { members, .. }) | Some(CirType::Union { members, .. }) => {
                members.as_deref().unwrap_or_default()
            }
            _ => continue,
        };
        for field_ty in members {
            let mut field_keys = BTreeSet::new();
            collect_anon_alias_keys(field_ty, &module.generic.type_aliases, &mut field_keys);
            for field_key in field_keys {
                if needed.insert(field_key.clone()) {
                    frontier.push(field_key);
                }
            }
        }
    }

    let mut records = Vec::new();
    for key in &needed {
        let (name, members, member_kinds, is_union) = match module.generic.type_aliases.get(key) {
            Some(CirType::Struct {
                name,
                members: Some(members),
                member_kinds,
                ..
            }) => (name, members, member_kinds, false),
            Some(CirType::Union {
                name,
                members: Some(members),
                member_kinds,
                ..
            }) => (name, members, member_kinds, true),
            _ => continue,
        };
        let Some(name) = name.as_deref().or_else(|| key.strip_prefix("rec_")) else {
            continue;
        };
        let fields = members
            .iter()
            .enumerate()
            .filter(|(i, _)| {
                matches!(
                    member_kinds.get(*i),
                    Some(CirRecordMemberKind::Data | CirRecordMemberKind::Pad)
                )
            })
            .map(|(i, field_ty)| crate::frontend::c_ast::Decl {
                name: if bitfield_slots.contains(&(key.clone(), i as i64)) {
                    format!("__bitfield_{i}")
                } else {
                    field_names
                        .get(&(key.clone(), i as i64))
                        .cloned()
                        .unwrap_or_else(|| format!("__slate_anon_{i}"))
                },
                comments: Vec::new(),
                ty: cir_type_to_ctype(field_ty, &module.generic.type_aliases),
                bit_width: bitfield_slots
                    .contains(&(key.clone(), i as i64))
                    .then_some(0),
            })
            .collect();
        records.push(crate::frontend::c_ast::Record {
            name: name.to_string(),
            comments: Vec::new(),
            kind: if is_union {
                RecordKind::Union
            } else {
                RecordKind::Struct
            },
            fields,
            packed: None,
            align: None,
        });
    }
    records
}
