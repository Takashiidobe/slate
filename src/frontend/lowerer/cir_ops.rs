use super::*;

const SELECTOR_BIND: &str = "__switch_sel";

pub(super) fn switch_case<'a>(op: &'a Op, bitint_ty: Option<&Type>) -> Option<SwitchCase<'a>> {
    let Op::Case(case) = op else {
        return None;
    };
    let raw_values: &[Attr] = match &case.value {
        Attr::Array(values) => values,
        _ => &[],
    };
    let patterns = if case.kind == clang_ir::enums::CaseOpKind::Range {
        let [start, end] = raw_values else {
            return None;
        };
        vec![case_range_pattern(start, end, bitint_ty)?]
    } else {
        case_values_to_patterns(raw_values, bitint_ty)?
    };
    Some(SwitchCase {
        patterns,
        is_default: case.kind == clang_ir::enums::CaseOpKind::Default,
        region: &case.case_region,
    })
}

pub(super) fn switch_flat_case_patterns(
    case_values: &Attr,
    bitint_ty: Option<&Type>,
) -> Option<Vec<Pattern>> {
    let Attr::Array(raw_values) = case_values else {
        return None;
    };
    case_values_to_patterns(raw_values, bitint_ty)
}

fn case_values_to_patterns(raw_values: &[Attr], bitint_ty: Option<&Type>) -> Option<Vec<Pattern>> {
    match bitint_ty {
        Some(ty) => raw_values
            .iter()
            .map(|value| bitint_case_value_pattern(value, ty))
            .collect(),
        None => raw_values.iter().map(case_value_pattern).collect(),
    }
}

fn case_value_i128(value: &Attr) -> Option<i128> {
    value.as_int()
}

fn case_value_u128(value: &Attr) -> Option<u128> {
    case_value_digits(value)?.parse().ok()
}

fn case_value_pattern(value: &Attr) -> Option<Pattern> {
    match value.as_int() {
        Some(value) => Some(int_pattern(value)),
        None => case_value_u128(value).map(Pattern::U128),
    }
}

fn case_range_pattern(start: &Attr, end: &Attr, bitint_ty: Option<&Type>) -> Option<Pattern> {
    if let Some(ty) = bitint_ty {
        return bitint_case_range_pattern(start, end, ty);
    }
    match (case_value_i128(start), case_value_i128(end)) {
        (Some(start), Some(end)) => Some(Pattern::InclusiveRange { start, end }),
        _ => Some(Pattern::InclusiveRangeU128 {
            start: case_value_u128(start)?,
            end: case_value_u128(end)?,
        }),
    }
}

fn case_value_digits(value: &Attr) -> Option<String> {
    match value {
        Attr::Int { value, .. } => Some(value.to_string()),
        Attr::CirInt { value, .. } => Some(value.clone()),
        _ => None,
    }
}

fn bitint_case_const_expr(value: &Attr, ty: &Type) -> Option<Expr> {
    let expr = bitint_from_decimal_str_expr(ty, &case_value_digits(value)?)?;
    Some(Expr::ConstBlock(Box::new(expr)))
}

fn selector_bound(op: BinOp, bound: Expr) -> Expr {
    Expr::Binary {
        op,
        lhs: Box::new(Expr::Var(SELECTOR_BIND.into())),
        rhs: Box::new(bound),
    }
}

fn bitint_case_value_pattern(value: &Attr, ty: &Type) -> Option<Pattern> {
    Some(Pattern::Guarded {
        bind: SELECTOR_BIND.into(),
        cond: Box::new(selector_bound(
            BinOp::Eq,
            bitint_case_const_expr(value, ty)?,
        )),
    })
}

fn bitint_case_range_pattern(start: &Attr, end: &Attr, ty: &Type) -> Option<Pattern> {
    let start = bitint_case_const_expr(start, ty)?;
    let end = bitint_case_const_expr(end, ty)?;
    Some(Pattern::Guarded {
        bind: SELECTOR_BIND.into(),
        cond: Box::new(Expr::Binary {
            op: BinOp::And,
            lhs: Box::new(selector_bound(BinOp::Ge, start)),
            rhs: Box::new(selector_bound(BinOp::Le, end)),
        }),
    })
}

pub(super) fn duff_switch<'a>(
    region: &'a inst::Region,
    bitint_ty: Option<&Type>,
) -> Option<DuffSwitch<'a>> {
    let ops = transparent_region_ops(region)?;
    let [outer] = ops.as_slice() else {
        return None;
    };
    let outer_case = switch_case(outer, bitint_ty)?;
    let do_op = transparent_region_ops(outer_case.region)?;
    let [Op::Do(do_op)] = do_op.as_slice() else {
        return None;
    };
    let body_ops = transparent_region_ops(&do_op.body)?;
    let first_case = body_ops.iter().position(|op| matches!(op, Op::Case(_)))?;
    let prefix = body_ops[..first_case].to_vec();
    let nested: Option<Vec<_>> = body_ops[first_case..]
        .iter()
        .map(|op| switch_case(op, bitint_ty))
        .collect();
    let mut cases = vec![outer_case];
    cases.extend(nested?);
    Some(DuffSwitch {
        cases,
        prefix,
        condition: &do_op.cond,
    })
}

fn transparent_region_ops(region: &inst::Region) -> Option<Vec<&Op>> {
    let [block] = region.blocks.as_slice() else {
        return None;
    };
    let ops: Vec<_> = block
        .ops
        .iter()
        .filter(|op| !matches!(op, Op::Yield(_)))
        .collect();
    if let [Op::Scope(scope)] = ops.as_slice() {
        return transparent_region_ops(&scope.scope_region);
    }
    Some(ops)
}

pub(super) fn block_diverges(block: &inst::Block) -> bool {
    block.ops.last().is_some_and(|op| {
        matches!(
            op,
            Op::Return(_)
                | Op::Br(_)
                | Op::Brcond(_)
                | Op::IndirectBr(_)
                | Op::IndirectGoto(_)
                | Op::Goto(_)
                | Op::SwitchFlat(_)
                | Op::Trap(_)
                | Op::Unreachable(_)
        )
    })
}

pub(super) fn region_ends_control_flow(region: &inst::Region) -> bool {
    region
        .blocks
        .iter()
        .rev()
        .flat_map(|block| block.ops.iter().rev())
        .find(|op| !matches!(op, Op::Yield(_)))
        .is_some_and(|op| {
            matches!(
                op,
                Op::Break(_)
                    | Op::Continue(_)
                    | Op::Return(_)
                    | Op::Goto(_)
                    | Op::Trap(_)
                    | Op::Unreachable(_)
            )
        })
}

// `u32` -> 32; None for bool/isize/usize/non-integers (no fixed width to mask to).
pub(super) fn int_bits(rust_ty: &str) -> Option<u32> {
    rust_ty
        .strip_prefix('i')
        .or_else(|| rust_ty.strip_prefix('u'))?
        .parse()
        .ok()
}

pub(super) fn asm_output_types<'a>(
    result_ty: Option<&'a CirType>,
    aliases: &'a BTreeMap<String, CirType>,
    output_count: usize,
) -> Option<Vec<&'a CirType>> {
    if output_count == 1 {
        return Some(vec![result_ty?]);
    }
    let CirType::Named(name) = result_ty? else {
        return None;
    };
    let fields: Vec<&CirType> = match aliases.get(name)? {
        CirType::Struct {
            members: Some(members),
            ..
        }
        | CirType::Union {
            members: Some(members),
            ..
        } => members.iter().collect(),
        _ => return None,
    };
    (fields.len() == output_count).then_some(fields)
}
