use super::*;

pub(super) fn walk_region_ops(region: &inst::Region, visit: &mut dyn FnMut(&Op) -> bool) {
    for block in &region.blocks {
        walk_block_ops(&block.ops, visit);
    }
}

pub(super) fn walk_block_ops(ops: &[Op], visit: &mut dyn FnMut(&Op) -> bool) {
    for op in ops {
        if visit(op) {
            op.for_each_region(|region| walk_region_ops(region, visit));
        }
    }
}

pub(super) fn attr_symbol_ref<'a>(op: &'a Operation, key: &str) -> Option<&'a str> {
    match op.attr(key)? {
        Attr::SymbolRef(value) => Some(value.trim_start_matches('@').trim_matches('"')),
        _ => None,
    }
}

pub(super) fn attr_int(op: &Operation, key: &str) -> Option<i64> {
    op.attr(key).and_then(Attr::as_int).map(|v| v as i64)
}

pub(super) fn attr_type<'a>(op: &'a Operation, key: &str) -> Option<&'a CirType> {
    op.attr(key).and_then(Attr::as_type)
}

pub(super) fn call_arg_byval_type_attr(attr: Option<&Attr>, operand_index: usize) -> Option<&str> {
    let Attr::Array(entries) = attr? else {
        return None;
    };
    let Attr::Dict(entries) = entries.get(operand_index)? else {
        return None;
    };
    let value = entries
        .iter()
        .find_map(|(key, value)| (key == "llvm.byval").then_some(value))?;
    value
        .as_str()
        .or_else(|| value.as_type().and_then(cir_record_name))
}

fn builtin_dense_array_ints(attr: &Attr) -> Option<Vec<i64>> {
    let Attr::Dialect {
        dialect,
        mnemonic,
        raw: Some(raw),
        ..
    } = attr
    else {
        return None;
    };
    if dialect != "builtin" || mnemonic != "array" {
        return None;
    }
    let digits = raw.split_once(':').map_or(raw.as_str(), |(_, rest)| rest);
    digits
        .split(',')
        .map(|part| part.trim().parse::<i64>().ok())
        .collect()
}

pub(super) fn int_array_attr(attr: &Attr) -> Option<Vec<u64>> {
    if let Some(values) = builtin_dense_array_ints(attr) {
        return values.into_iter().map(|n| u64::try_from(n).ok()).collect();
    }
    let Attr::Array(values) = attr else {
        return None;
    };
    values
        .iter()
        .map(|value| value.as_int().and_then(|value| u64::try_from(value).ok()))
        .collect()
}

pub(super) fn aggregate_member_index(op: &Operation) -> Option<usize> {
    attr_int(op, "index")
        .or_else(|| attr_int(op, "index_attr"))
        .and_then(|index| usize::try_from(index).ok())
}

pub(super) fn attr_bool(op: &Operation, key: &str) -> bool {
    op.attr(key).is_some_and(|value| {
        !matches!(
            value,
            Attr::Bool(false) | Attr::CirBool { value: false, .. }
        )
    })
}

/// Alloca results for clang-generated temps (`.atomictmp`, `atomic-temp`,
/// `cmpxchg.bool` — names no C identifier can spell) whose only uses are one
/// plain store followed by one plain load in the same block. Such a slot
/// carries a single SSA value, so the lowerer forwards it instead of declaring
/// one shadowed named local per call site.
pub(super) fn forwardable_temp_allocas(body: &Region) -> BTreeSet<String> {
    #[derive(Default)]
    struct Uses {
        stores: usize,
        loads: usize,
        store_at: Option<(usize, usize)>,
        load_at: Option<(usize, usize)>,
        escapes: bool,
    }
    fn plain_access(op: &Operation) -> bool {
        attr_int(op, "mem_order").is_none() && !attr_bool(op, "is_volatile")
    }
    fn walk(blocks: &[Block], next_block: &mut usize, uses: &mut BTreeMap<String, Uses>) {
        for block in blocks {
            let block_id = *next_block;
            *next_block += 1;
            for (pos, op) in block.ops.iter().enumerate() {
                if op.mnemonic() == "alloca"
                    && let Some(name) = op.attr("name").and_then(Attr::as_str)
                    && name.chars().any(|c| !c.is_ascii_alphanumeric() && c != '_')
                    && let Some((result, _)) = op.results.first()
                {
                    uses.entry(result.clone()).or_default();
                }
                for (i, operand) in op.operands.iter().enumerate() {
                    let Some(u) = uses.get_mut(operand) else {
                        continue;
                    };
                    match op.mnemonic() {
                        "store" if i == 1 && plain_access(op) => {
                            u.stores += 1;
                            u.store_at = Some((block_id, pos));
                        }
                        "load" if i == 0 && plain_access(op) => {
                            u.loads += 1;
                            u.load_at = Some((block_id, pos));
                        }
                        _ => u.escapes = true,
                    }
                }
                for region in &op.regions {
                    walk(&region.blocks, next_block, uses);
                }
            }
        }
    }
    let mut uses = BTreeMap::new();
    walk(&body.blocks, &mut 0, &mut uses);
    uses.into_iter()
        .filter(|(_, u)| {
            !u.escapes
                && u.stores == 1
                && u.loads == 1
                && matches!(
                    (u.store_at, u.load_at),
                    (Some((sb, sp)), Some((lb, lp))) if sb == lb && sp < lp
                )
        })
        .map(|(result, _)| result)
        .collect()
}
