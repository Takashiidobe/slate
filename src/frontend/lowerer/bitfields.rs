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
        .filter(|op| op.kind() == CirOpKind::Func)
    {
        let mut members = BTreeMap::new();
        collect_function_bitfields(
            &function.regions,
            &module.aliases,
            &mut members,
            &mut storages,
        );
    }
    storages
}

fn collect_function_bitfields(
    regions: &[Region],
    aliases: &BTreeMap<String, String>,
    members: &mut BTreeMap<String, MemberStorage>,
    storages: &mut BitfieldStorages,
) {
    for region in regions {
        for block in &region.blocks {
            for op in &block.ops {
                if op.kind() == CirOpKind::GetMember
                    && let Some(member) = member_storage(op, aliases)
                {
                    for result in &op.results {
                        members.insert(result.clone(), member.clone());
                    }
                }
                if matches!(op.kind(), CirOpKind::GetBitfield | CirOpKind::SetBitfield)
                    && let Some(ptr) = op.operands.first()
                    && let Some(member) = members.get(ptr)
                    && let Some((size, offset)) = bitfield_info(op, aliases)
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
                collect_function_bitfields(&op.regions, aliases, members, storages);
            }
        }
    }
}

fn member_storage(op: &Op, aliases: &BTreeMap<String, String>) -> Option<MemberStorage> {
    let record = op
        .ty
        .as_deref()
        .and_then(split_top_level_arrow)
        .and_then(|(inputs, _)| inputs.trim().strip_prefix('(')?.strip_suffix(')'))
        .and_then(|inputs| split_top_level(inputs, ',').first().copied())
        .and_then(cir_ptr_pointee)
        .map(|ty| aliases.get(ty).map(String::as_str).unwrap_or(ty))
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

fn bitfield_info(op: &Op, aliases: &BTreeMap<String, String>) -> Option<(u32, u32)> {
    let raw = attr_str(op, "bitfield_info")?;
    let resolved = aliases.get(raw).map_or(raw, String::as_str);
    Some((
        bitfield_info_number(resolved, "size = ")?,
        bitfield_info_number(resolved, "offset = ")?,
    ))
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
