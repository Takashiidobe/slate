use super::*;

#[derive(Debug, Clone)]
pub(super) struct BitfieldField {
    pub(super) name: String,
    pub(super) ty: Type,
    pub(super) size: u32,
    pub(super) offset: u32,
}

#[derive(Debug, Clone)]
pub(super) struct BitfieldStorage {
    pub(super) wrapper: String,
    pub(super) backing: Type,
    pub(super) fields: BTreeMap<String, BitfieldField>,
}

pub(super) type BitfieldStorages = BTreeMap<(String, usize), BitfieldStorage>;

#[derive(Debug, Clone)]
struct MemberStorage {
    record: String,
    index: usize,
    backing: Type,
    field: String,
}

pub(super) fn collect_bitfield_storages(module: &Module) -> BitfieldStorages {
    let Some(module_op) = builtin_module(module) else {
        return BTreeMap::new();
    };
    let mut storages = BTreeMap::new();
    for function in region_ops(module_op)
        .into_iter()
        .filter(|op| op.mnemonic() == "func")
    {
        let mut members = BTreeMap::new();
        collect_function_bitfields(&function.regions, module, &mut members, &mut storages);
    }
    storages
}

fn collect_function_bitfields(
    regions: &[Region],
    module: &Module,
    members: &mut BTreeMap<String, MemberStorage>,
    storages: &mut BitfieldStorages,
) {
    let aliases = &module.type_aliases;
    for region in regions {
        for block in &region.blocks {
            for op in &block.ops {
                if op.mnemonic() == "get_member"
                    && let Some(member) = member_storage(op, aliases)
                {
                    for (result, _) in &op.results {
                        members.insert(result.clone(), member.clone());
                    }
                }
                if matches!(op.mnemonic(), "get_bitfield" | "set_bitfield")
                    && let Some(ptr) = op.operands.first()
                    && let Some(member) = members.get(ptr)
                    && let Some((size, offset)) = bitfield_info(op, module)
                    && let Some(result_ty) = op_result_type(op)
                {
                    let key = (member.record.clone(), member.index);
                    let wrapper = format!(
                        "__slate_bitfields::__SlateBitfield_{}_{}",
                        member.record, member.index
                    );
                    let storage = storages.entry(key).or_insert_with(|| BitfieldStorage {
                        wrapper,
                        backing: member.backing.clone(),
                        fields: BTreeMap::new(),
                    });
                    storage
                        .fields
                        .entry(member.field.clone())
                        .or_insert_with(|| BitfieldField {
                            name: member.field.clone(),
                            ty: rust_type_with_aliases(result_ty, aliases, false),
                            size,
                            offset,
                        });
                }
                collect_function_bitfields(&op.regions, module, members, storages);
            }
        }
    }
}

fn resolve_type_alias<'a>(ty: &'a CirType, aliases: &'a BTreeMap<String, CirType>) -> &'a CirType {
    let mut ty = ty;
    let mut seen = BTreeSet::new();
    while let CirType::Named(name) = ty {
        if !seen.insert(name.clone()) {
            break;
        }
        match aliases.get(name) {
            Some(expanded) => ty = expanded,
            None => break,
        }
    }
    ty
}

fn member_storage(op: &Operation, aliases: &BTreeMap<String, CirType>) -> Option<MemberStorage> {
    let record = op_operand_types(op)
        .first()
        .and_then(cir_ptr_pointee)
        .map(|ty| resolve_type_alias(ty, aliases))
        .and_then(cir_record_name)
        .map(|name| sanitize_ident(name).into_string())?;
    let index = aggregate_member_index(op)?;
    let backing = op_result_type(op)
        .and_then(cir_ptr_pointee)
        .map(|ty| rust_type_with_aliases(ty, aliases, false))?;
    let field = sanitize_ident(attr_str(op, "name")?).into_string();
    Some(MemberStorage {
        record,
        index,
        backing,
        field,
    })
}

fn bitfield_info(op: &Operation, module: &Module) -> Option<(u32, u32)> {
    match module.resolve_attr(op.attr("bitfield_info")?) {
        Attr::BitfieldInfo { size, offset, .. } => {
            Some((u32::try_from(*size).ok()?, u32::try_from(*offset).ok()?))
        }
        _ => None,
    }
}

pub(super) fn bitfield_items(storages: &BitfieldStorages) -> Vec<Item> {
    let mut wrappers = Vec::new();
    for storage in storages.values() {
        let Some(total) = bit_size(&storage.backing) else {
            continue;
        };
        let mut fields: Vec<_> = storage.fields.values().collect();
        fields.sort_by_key(|field| field.offset);
        let mut rust_fields = Vec::new();
        let mut cursor = 0;
        let mut reserved = 0;
        for field in fields {
            if cursor < field.offset {
                rust_fields.push(bitfield_struct_field(
                    format!("_reserved_{reserved}"),
                    Type::Prim(Prim::U128),
                    field.offset - cursor,
                ));
                reserved += 1;
            }
            rust_fields.push(bitfield_struct_field(
                field.name.clone(),
                field.ty.clone(),
                field.size,
            ));
            cursor = field.offset + field.size;
        }
        if cursor < total {
            rust_fields.push(bitfield_struct_field(
                format!("_reserved_{reserved}"),
                Type::Prim(Prim::U128),
                total - cursor,
            ));
        }
        let mut args = vec![AttrArg::Type(storage.backing.clone())];
        for name in [
            "new",
            "from_into_bits",
            "from_traits",
            "default",
            "debug",
            "builder",
            "bit_ops",
        ] {
            args.push(AttrArg::Named(name.into(), Box::new(AttrArg::Bool(false))));
        }
        wrappers.push(Item::Struct(StructDef {
            attrs: vec![RustAttr::Call {
                path: Path::new([Ident::from("bitfields"), Ident::from("bitfield")]),
                args,
            }],
            vis: Visibility::Pub,
            field_vis: Visibility::Pub,
            generics: Vec::new(),
            name: storage
                .wrapper
                .rsplit("::")
                .next()
                .expect("bitfield wrapper name")
                .to_string(),
            fields: StructFields::Named(rust_fields),
        }));
    }
    if wrappers.is_empty() {
        Vec::new()
    } else {
        vec![Item::InlineMod {
            name: "__slate_bitfields".into(),
            items: wrappers,
        }]
    }
}

fn bitfield_struct_field(name: String, ty: Type, bits: u32) -> StructField {
    StructField {
        attrs: vec![RustAttr::Call {
            path: Path::new([Ident::from("bits")]),
            args: vec![AttrArg::UInt(u64::from(bits))],
        }],
        name,
        ty,
    }
}

fn bit_size(ty: &Type) -> Option<u32> {
    match ty {
        Type::Prim(Prim::U8 | Prim::I8) => Some(8),
        Type::Prim(Prim::U16 | Prim::I16) => Some(16),
        Type::Prim(Prim::U32 | Prim::I32) => Some(32),
        Type::Prim(Prim::U64 | Prim::I64) => Some(64),
        Type::Prim(Prim::U128 | Prim::I128) => Some(128),
        Type::Array { elem, len } if matches!(**elem, Type::Prim(Prim::U8)) => {
            u32::try_from(*len).ok()?.checked_mul(8)
        }
        _ => None,
    }
}
