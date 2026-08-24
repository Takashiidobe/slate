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
        .or_else(|| value.as_type().and_then(slate_record_name))
}

pub(super) fn int_array_attr(attr: &Attr) -> Option<Vec<u64>> {
    if let Some(values) = attr.as_dense_array_ints() {
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

/// Alloca results for clang-generated temps (`.atomictmp`, `atomic-temp`,
/// `cmpxchg.bool` — names no C identifier can spell) whose only uses are one
/// plain store followed by one plain load in the same block. Such a slot
/// carries a single SSA value, so the lowerer forwards it instead of declaring
/// one shadowed named local per call site.
pub(super) fn forwardable_temp_allocas(body: &inst::Region) -> BTreeSet<String> {
    #[derive(Default)]
    struct Uses {
        stores: usize,
        loads: usize,
        store_at: Option<(usize, usize)>,
        load_at: Option<(usize, usize)>,
        escapes: bool,
    }
    fn walk(blocks: &[inst::Block], next_block: &mut usize, uses: &mut BTreeMap<String, Uses>) {
        for block in blocks {
            let block_id = *next_block;
            *next_block += 1;
            for (pos, op) in block.ops.iter().enumerate() {
                if let Op::Alloca(alloca) = op
                    && alloca
                        .name
                        .chars()
                        .any(|c| !c.is_ascii_alphanumeric() && c != '_')
                {
                    uses.entry(alloca.addr.clone()).or_default();
                }
                op.for_each_operand(|operand| {
                    let Some(u) = uses.get_mut(operand) else {
                        return;
                    };
                    match op {
                        Op::Store(store)
                            if store.addr == *operand
                                && store.mem_order.is_none()
                                && !store.is_volatile =>
                        {
                            u.stores += 1;
                            u.store_at = Some((block_id, pos));
                        }
                        Op::Load(load)
                            if load.addr == *operand
                                && load.mem_order.is_none()
                                && !load.is_volatile =>
                        {
                            u.loads += 1;
                            u.load_at = Some((block_id, pos));
                        }
                        _ => u.escapes = true,
                    }
                });
                op.for_each_region(|region| walk(&region.blocks, next_block, uses));
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
