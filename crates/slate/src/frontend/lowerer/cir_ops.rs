use super::*;

pub(super) fn switch_case<'a>(op: &'a Op, bitint_ty: Option<&Type>) -> Option<SwitchCase<'a>> {
    let kind = attr_int(op, "kind");
    let is_default = kind == Some(0);
    let raw_values: &[Attr] = match op.attr("value") {
        Some(Attr::Array(values)) => values,
        _ => &[],
    };
    let patterns = if kind == Some(3) {
        let [start, end] = raw_values else {
            return None;
        };
        vec![case_range_pattern(start, end, bitint_ty)?]
    } else {
        case_values_to_patterns(raw_values, bitint_ty)?
    };
    let region = op.regions.first()?;
    Some(SwitchCase {
        patterns,
        is_default,
        region,
    })
}

pub(super) fn switch_flat_case_patterns(op: &Op, bitint_ty: Option<&Type>) -> Option<Vec<Pattern>> {
    let raw_values: &[Attr] = match op.attr("caseValues") {
        Some(Attr::Array(values)) => values,
        _ => &[],
    };
    case_values_to_patterns(raw_values, bitint_ty)
}

fn case_values_to_patterns(raw_values: &[Attr], bitint_ty: Option<&Type>) -> Option<Vec<Pattern>> {
    if let Some(ty) = bitint_ty {
        raw_values
            .iter()
            .map(|value| bitint_case_value_pattern(value, ty))
            .collect()
    } else {
        raw_values.iter().map(case_value_pattern).collect()
    }
}

fn case_value_i128(value: &Attr) -> Option<i128> {
    value.as_int()
}

fn case_value_pattern(value: &Attr) -> Option<Pattern> {
    value.as_int().map(int_pattern).or_else(|| {
        value
            .as_int()
            .and_then(|value| u128::try_from(value).ok())
            .map(Pattern::U128)
    })
}

fn case_range_pattern(start: &Attr, end: &Attr, bitint_ty: Option<&Type>) -> Option<Pattern> {
    match bitint_ty {
        Some(ty) => bitint_case_range_pattern(start, end, ty),
        None => Some(Pattern::InclusiveRange {
            start: case_value_i128(start)?,
            end: case_value_i128(end)?,
        }),
    }
}

fn case_value_digits(value: &Attr) -> Option<String> {
    match value {
        Attr::Int { value, .. } => Some(value.to_string()),
        _ => None,
    }
}

fn bitint_case_value_pattern(value: &Attr, ty: &Type) -> Option<Pattern> {
    let digits = case_value_digits(value)?;
    let cond = bitint_from_decimal_str_expr(ty, &digits)?;
    Some(Pattern::Guarded {
        bind: "v".into(),
        cond: Box::new(Expr::Binary {
            op: BinOp::Eq,
            lhs: Box::new(Expr::Var("v".into())),
            rhs: Box::new(cond),
        }),
    })
}

fn bitint_case_range_pattern(start: &Attr, end: &Attr, ty: &Type) -> Option<Pattern> {
    let start_expr = bitint_from_decimal_str_expr(ty, &case_value_digits(start)?)?;
    let end_expr = bitint_from_decimal_str_expr(ty, &case_value_digits(end)?)?;
    Some(Pattern::Guarded {
        bind: "v".into(),
        cond: Box::new(Expr::Binary {
            op: BinOp::And,
            lhs: Box::new(Expr::Binary {
                op: BinOp::Ge,
                lhs: Box::new(Expr::Var("v".into())),
                rhs: Box::new(start_expr),
            }),
            rhs: Box::new(Expr::Binary {
                op: BinOp::Le,
                lhs: Box::new(Expr::Var("v".into())),
                rhs: Box::new(end_expr),
            }),
        }),
    })
}

pub(super) fn duff_switch<'a>(
    region: &'a Region,
    bitint_ty: Option<&Type>,
) -> Option<DuffSwitch<'a>> {
    let ops = transparent_region_ops(region)?;
    let [outer] = ops.as_slice() else {
        return None;
    };
    let outer_case = switch_case(outer, bitint_ty)?;
    let do_op = transparent_single_op(outer_case.region, CirOpKind::Do)?;
    let [body, condition, ..] = do_op.regions.as_slice() else {
        return None;
    };
    let body_ops = transparent_region_ops(body)?;
    let first_case = body_ops
        .iter()
        .position(|op| op.kind() == CirOpKind::Case)?;
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
        condition,
    })
}

pub(super) fn transparent_single_op(region: &Region, kind: CirOpKind) -> Option<&Op> {
    let ops = transparent_region_ops(region)?;
    let [op] = ops.as_slice() else {
        return None;
    };
    (op.kind() == kind).then_some(*op)
}

pub(super) fn transparent_region_ops(region: &Region) -> Option<Vec<&Op>> {
    let [block] = region.blocks.as_slice() else {
        return None;
    };
    let ops: Vec<_> = block
        .ops
        .iter()
        .filter(|op| op.kind() != CirOpKind::Yield)
        .collect();
    if let [op] = ops.as_slice()
        && op.kind() == CirOpKind::Scope
    {
        return transparent_region_ops(op.regions.first()?);
    }
    Some(ops)
}

/// Whether a dispatch block ends in its own control transfer (so the dispatch
/// loop must not append a fall-through to the next state).
pub(super) fn block_diverges(block: &Block) -> bool {
    block.ops.last().is_some_and(|op| {
        matches!(
            op.kind(),
            CirOpKind::Return
                | CirOpKind::Br
                | CirOpKind::Brcond
                | CirOpKind::IndirectBr
                | CirOpKind::Goto
                | CirOpKind::SwitchFlat
                | CirOpKind::Trap
                | CirOpKind::Unreachable
        )
    })
}

pub(super) fn region_ends_control_flow(region: &Region) -> bool {
    region
        .blocks
        .iter()
        .rev()
        .flat_map(|block| block.ops.iter().rev())
        .find(|op| op.kind() != CirOpKind::Yield)
        .is_some_and(|op| {
            matches!(
                op.kind(),
                CirOpKind::Break
                    | CirOpKind::Continue
                    | CirOpKind::Return
                    | CirOpKind::Goto
                    | CirOpKind::Trap
                    | CirOpKind::Unreachable
            )
        })
}

pub(super) fn op_result_type(op: &Op) -> Option<&CirType> {
    op.results.first().map(|(_, ty)| ty)
}

// `u32` -> 32; None for bool/isize/usize/non-integers (no fixed width to mask to).
pub(super) fn int_bits(rust_ty: &str) -> Option<u32> {
    rust_ty
        .strip_prefix('i')
        .or_else(|| rust_ty.strip_prefix('u'))?
        .parse()
        .ok()
}

pub(super) fn op_result_types(op: &Op) -> Vec<&CirType> {
    op.results.iter().map(|(_, ty)| ty).collect()
}

pub(super) fn asm_output_types<'a>(
    op: &'a Op,
    aliases: &'a BTreeMap<String, CirType>,
    output_count: usize,
) -> Option<Vec<&'a CirType>> {
    let direct = op_result_types(op);
    if direct.len() == output_count {
        return Some(direct);
    }
    let CirType::Named(name) = op_result_type(op)? else {
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

pub(super) fn op_operand_types(op: &Op) -> &[CirType] {
    &op.operand_types
}

pub(super) fn cir_ptr_inner(ty: &CirType) -> Option<&CirType> {
    match ty {
        CirType::Pointer { pointee: inner, .. } => Some(inner),
        _ => None,
    }
}

pub(super) fn parse_function_type(ty: &CirType) -> (Vec<CirType>, Option<CirType>) {
    match ty {
        CirType::Func {
            inputs,
            optional_return_type,
            ..
        } => (inputs.clone(), optional_return_type.as_deref().cloned()),
        _ => (Vec::new(), None),
    }
}

pub(super) fn function_type_has_params(ty: &CirType) -> bool {
    matches!(ty, CirType::Func { inputs, .. } if !inputs.is_empty())
}

/// Whether a `!cir.func<..>` type ends its parameter list with `...`.
pub(super) fn function_type_is_variadic(ty: &CirType) -> bool {
    matches!(ty, CirType::Func { var_arg: true, .. })
}
