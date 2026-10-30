use super::*;

pub(super) struct SwitchArm<'a> {
    values: Vec<CaseValue<'a>>,
    default: bool,
    body: Vec<&'a slate_parser::ast::Span<ir::Statement>>,
}

#[derive(Clone, Copy)]
struct CaseValue<'a> {
    start: &'a ir::Value,
    end: Option<&'a ir::Value>,
}

pub(super) fn switch_label<'a>(
    statement: &'a ir::Statement,
    switch: BindingId,
) -> Result<Option<SwitchArm<'a>>> {
    let (value, body) = match statement {
        ir::Statement::Case {
            switch: owner,
            start,
            end,
            body,
        } if *owner == switch => (
            Some(CaseValue {
                start,
                end: end.as_ref(),
            }),
            body,
        ),
        ir::Statement::Default {
            switch: owner,
            body,
        } if *owner == switch => (None, body),
        _ => return Ok(None),
    };
    let leading = body
        .iter()
        .take_while(|statement| matches!(statement.value, ir::Statement::Comment(_)))
        .count();
    let mut arm = match body.get(leading) {
        Some(first) => match switch_label(first, switch)? {
            Some(mut nested) => {
                nested.body.splice(0..0, body[..leading].iter());
                nested.body.extend(&body[leading + 1..]);
                nested
            }
            None => SwitchArm {
                values: Vec::new(),
                default: false,
                body: body.iter().collect(),
            },
        },
        None => SwitchArm {
            values: Vec::new(),
            default: false,
            body: Vec::new(),
        },
    };
    match value {
        Some(value) => arm.values.insert(0, value),
        None => arm.default = true,
    }
    Ok(Some(arm))
}

pub(super) fn contains_switch_label(statements: &[&ir::Statement], switch: BindingId) -> bool {
    statements.iter().any(|statement| match statement {
        ir::Statement::Case { switch: owner, .. }
        | ir::Statement::Default { switch: owner, .. }
            if *owner == switch =>
        {
            true
        }
        ir::Statement::Block(body)
        | ir::Statement::While { body, .. }
        | ir::Statement::DoWhile { body, .. }
        | ir::Statement::Switch { body, .. }
        | ir::Statement::Label { body, .. }
        | ir::Statement::Case { body, .. }
        | ir::Statement::Default { body, .. } => contains_switch_label(
            &body
                .iter()
                .map(|statement| &**statement)
                .collect::<Vec<_>>(),
            switch,
        ),
        ir::Statement::For { init, body, .. } => contains_switch_label(
            &init
                .iter()
                .chain(body)
                .map(|statement| &**statement)
                .collect::<Vec<_>>(),
            switch,
        ),
        ir::Statement::If {
            then_body,
            else_body,
            ..
        } => contains_switch_label(
            &then_body
                .iter()
                .chain(else_body.iter().flatten())
                .map(|statement| &**statement)
                .collect::<Vec<_>>(),
            switch,
        ),
        _ => false,
    })
}

pub(super) fn ends_in_jump(statement: &ir::Statement) -> bool {
    match statement {
        ir::Statement::Break(_) | ir::Statement::Continue(_) | ir::Statement::Return(_) => true,
        ir::Statement::Block(body) => body
            .iter()
            .rev()
            .find(|statement| !matches!(statement.value, ir::Statement::Comment(_)))
            .is_some_and(|last| ends_in_jump(last)),
        _ => false,
    }
}

impl FunctionLowerer<'_, '_> {
    pub(super) fn lower_switch(
        &mut self,
        id: BindingId,
        discriminant: &ir::Value,
        body: &[slate_parser::ast::Span<ir::Statement>],
    ) -> Result<Stmt> {
        let statements = match body {
            [single] => match &**single {
                ir::Statement::Block(inner) => inner.as_slice(),
                _ => body,
            },
            _ => body,
        };
        let mut arms: Vec<SwitchArm> = Vec::new();
        let mut leading_comments = Vec::new();
        for statement in statements {
            match switch_label(statement, id)? {
                Some(mut arm) => {
                    arm.body.splice(0..0, leading_comments.drain(..));
                    arms.push(arm);
                }
                None => match arms.last_mut() {
                    Some(arm) => arm.body.push(statement),
                    None if matches!(statement.value, ir::Statement::Comment(_)) => {
                        leading_comments.push(statement);
                    }
                    None => {
                        return Err(unsupported_switch("statement before first case"));
                    }
                },
            }
        }
        for arm in &arms {
            if contains_switch_label(
                &arm.body
                    .iter()
                    .map(|statement| &statement.value)
                    .collect::<Vec<_>>(),
                id,
            ) {
                return Err(at_arm(
                    unsupported_switch("nested case label"),
                    arm.body.first(),
                ));
            }
        }
        let arms = merge_empty_arms(arms);
        if self
            .tables
            .wide_int(&discriminant.ty)
            .is_some_and(|wide| wide.exceeds_i128())
        {
            return self.lower_indexed_switch(id, discriminant, &arms);
        }
        if arms[..arms.len().saturating_sub(1)]
            .iter()
            .any(falls_through)
        {
            return self.lower_fallthrough_switch(id, discriminant, &arms);
        }
        let mut match_arms = Vec::new();
        for arm in arms.iter().filter(|arm| !arm.default) {
            let Some(pattern) = switch_arm_pattern(arm)? else {
                continue;
            };
            match_arms.push(rust::MatchArm {
                pattern,
                body: self.lower_switch_arm(arm)?,
            });
        }
        match_arms.push(rust::MatchArm {
            pattern: rust::Pattern::Wildcard,
            body: match arms.iter().find(|arm| arm.default) {
                Some(arm) => self.lower_switch_arm(arm)?,
                None => Vec::new(),
            },
        });
        Ok(Stmt::LabeledBlock {
            label: self.break_label(id),
            body: vec![Stmt::Match {
                expr: self.lower_match_scrutinee(discriminant)?,
                arms: match_arms,
            }],
        })
    }

    fn lower_fallthrough_switch(
        &mut self,
        id: BindingId,
        discriminant: &ir::Value,
        arms: &[SwitchArm],
    ) -> Result<Stmt> {
        let case = self.next_temp();
        let case_index = |index: usize| Expr::Value(rust::RustValue::I64(index as i64));
        let mut dispatch = Vec::new();
        for (index, arm) in arms.iter().enumerate().filter(|(_, arm)| !arm.default) {
            if let Some(pattern) = switch_arm_pattern(arm)? {
                dispatch.push(rust::ExprMatchArm {
                    pattern,
                    value: case_index(index),
                });
            }
        }
        dispatch.push(rust::ExprMatchArm {
            pattern: rust::Pattern::Wildcard,
            value: match arms.iter().position(|arm| arm.default) {
                Some(index) => case_index(index),
                None => Expr::Value(rust::RustValue::I64(-1)),
            },
        });
        let init = Expr::Match {
            expr: Box::new(self.lower_match_scrutinee(discriminant)?),
            arms: dispatch,
        };
        self.lower_case_loop(id, case, init, arms, Vec::new())
    }

    fn lower_indexed_switch(
        &mut self,
        id: BindingId,
        discriminant: &ir::Value,
        arms: &[SwitchArm],
    ) -> Result<Stmt> {
        let selector = self.next_temp();
        let case = self.next_temp();
        let selector_type = self.lower_type(&discriminant.ty)?;
        let selector_init = self.lower_value(discriminant)?;
        let case_index = |index: usize| Expr::Value(rust::RustValue::I64(index as i64));
        let mut init = match arms.iter().position(|arm| arm.default) {
            Some(index) => case_index(index),
            None => Expr::Value(rust::RustValue::I64(-1)),
        };
        for (index, arm) in arms
            .iter()
            .enumerate()
            .rev()
            .filter(|(_, arm)| !arm.default)
        {
            let mut condition = None;
            for value in &arm.values {
                let matches = self.lower_case_condition(&selector, *value)?;
                condition = Some(match condition {
                    None => matches,
                    Some(previous) => Expr::Binary {
                        op: BinOp::Or,
                        lhs: Box::new(previous),
                        rhs: Box::new(matches),
                    },
                });
            }
            if let Some(cond) = condition {
                init = Expr::If {
                    cond: Box::new(cond),
                    then_expr: Box::new(case_index(index)),
                    else_expr: Box::new(init),
                };
            }
        }
        let prelude = vec![Stmt::Let {
            name: selector,
            mutable: false,
            ty: Some(selector_type),
            init: Some(selector_init),
        }];
        self.lower_case_loop(id, case, init, arms, prelude)
    }

    fn lower_case_condition(&mut self, selector: &str, value: CaseValue) -> Result<Expr> {
        let compare = |this: &mut Self, bound: &ir::Value, op: BinOp| -> Result<Expr> {
            Ok(Expr::Binary {
                op,
                lhs: Box::new(Expr::MethodCall {
                    recv: Box::new(Expr::Var(selector.into())),
                    method: "compare".into(),
                    args: vec![this.lower_value(bound)?],
                }),
                rhs: Box::new(Expr::Value(rust::RustValue::I64(0))),
            })
        };
        match value.end {
            None => compare(self, value.start, BinOp::Eq),
            Some(end) => Ok(Expr::Binary {
                op: BinOp::And,
                lhs: Box::new(compare(self, value.start, BinOp::Ge)?),
                rhs: Box::new(compare(self, end, BinOp::Le)?),
            }),
        }
    }

    fn lower_case_loop(
        &mut self,
        id: BindingId,
        case: String,
        init: Expr,
        arms: &[SwitchArm],
        mut body_prefix: Vec<Stmt>,
    ) -> Result<Stmt> {
        let label = self.break_label(id);
        let case_index = |index: usize| Expr::Value(rust::RustValue::I64(index as i64));
        let mut match_arms = Vec::new();
        for (index, arm) in arms.iter().enumerate() {
            let mut body = self.lower_switch_arm(arm)?;
            if falls_through(arm) {
                if index + 1 < arms.len() {
                    body.push(Stmt::Assign {
                        target: Expr::Var(case.clone().into()),
                        value: case_index(index + 1),
                    });
                    body.push(Stmt::Continue(Some(label.clone())));
                } else {
                    body.push(Stmt::Break(Some(label.clone())));
                }
            }
            match_arms.push(rust::MatchArm {
                pattern: rust::Pattern::I64(index as i64),
                body,
            });
        }
        match_arms.push(rust::MatchArm {
            pattern: rust::Pattern::Wildcard,
            body: vec![Stmt::Break(Some(label.clone()))],
        });
        body_prefix.push(Stmt::Let {
            name: case.clone(),
            mutable: true,
            ty: Some(rust::Type::Prim(Prim::I64)),
            init: Some(init),
        });
        body_prefix.push(Stmt::Loop {
            label: Some(label),
            body: vec![Stmt::Match {
                expr: Expr::Var(case.into()),
                arms: match_arms,
            }],
        });
        Ok(Stmt::Scope { body: body_prefix })
    }

    pub(super) fn lower_switch_operand(&mut self, value: &ir::Value) -> Result<(Expr, rust::Type)> {
        let lowered = self.lower_value(value)?;
        match self.tables.wide_int(&value.ty) {
            None => Ok((lowered, self.lower_type(&value.ty)?)),
            Some(wide) => Ok(wide.switch_operand(lowered)),
        }
    }

    fn lower_match_scrutinee(&mut self, discriminant: &ir::Value) -> Result<Expr> {
        Ok(self.lower_switch_operand(discriminant)?.0)
    }

    fn lower_switch_arm(&mut self, arm: &SwitchArm) -> Result<Vec<Stmt>> {
        let mut lowered = Vec::new();
        for statement in &arm.body {
            lowered.extend(self.lower_statement(statement)?);
        }
        Ok(lowered)
    }
}

fn merge_empty_arms(arms: Vec<SwitchArm>) -> Vec<SwitchArm> {
    let count = arms.len();
    let mut merged: Vec<SwitchArm> = Vec::new();
    let mut pending = SwitchArm {
        values: Vec::new(),
        default: false,
        body: Vec::new(),
    };
    for (index, mut arm) in arms.into_iter().enumerate() {
        pending.values.append(&mut arm.values);
        pending.default |= arm.default;
        if index + 1 < count && arm.body.iter().all(|statement| is_empty(statement)) {
            pending.body.extend(arm.body);
            continue;
        }
        arm.body.splice(0..0, pending.body.drain(..));
        arm.values = std::mem::take(&mut pending.values);
        arm.default = std::mem::take(&mut pending.default);
        merged.push(arm);
    }
    merged
}

fn is_empty(statement: &ir::Statement) -> bool {
    match statement {
        ir::Statement::Null | ir::Statement::Comment(_) => true,
        ir::Statement::Block(body) => body.iter().all(|statement| is_empty(statement)),
        _ => false,
    }
}

fn switch_arm_pattern(arm: &SwitchArm) -> Result<Option<rust::Pattern>> {
    let mut patterns = Vec::new();
    for value in &arm.values {
        let pattern =
            case_pattern(*value).map_err(|error| error.at(Site::of(&value.start.node)))?;
        patterns.extend(pattern);
    }
    Ok(match patterns.len() {
        0 => None,
        1 => patterns.pop(),
        _ => Some(rust::Pattern::Or(patterns)),
    })
}

#[derive(Clone, Copy)]
enum CaseNumber {
    Signed(i128),
    Unsigned(u128),
}

impl CaseNumber {
    fn signed(self) -> Option<i128> {
        match self {
            CaseNumber::Signed(number) => Some(number),
            CaseNumber::Unsigned(number) => i128::try_from(number).ok(),
        }
    }

    fn unsigned(self) -> Option<u128> {
        match self {
            CaseNumber::Signed(number) => u128::try_from(number).ok(),
            CaseNumber::Unsigned(number) => Some(number),
        }
    }
}

fn case_number(value: &ir::Value) -> Result<CaseNumber> {
    match &value.node.value {
        ValueKind::Constant(Number::Integer(number)) => number
            .to_string()
            .parse()
            .map(CaseNumber::Unsigned)
            .map_err(|_| unsupported_switch("case value")),
        ValueKind::Constant(Number::SignedInteger(number)) => number
            .to_string()
            .parse()
            .map(CaseNumber::Signed)
            .map_err(|_| unsupported_switch("case value")),
        ValueKind::Constant(_) => Err(unsupported_switch("case value")),
        _ => Err(Invariant::NonConstantCase.into()),
    }
}

fn case_pattern(value: CaseValue) -> Result<Option<rust::Pattern>> {
    let start = case_number(value.start)?;
    let Some(end) = value.end else {
        return Ok(Some(match start {
            CaseNumber::Signed(number) => {
                i64::try_from(number).map_or(rust::Pattern::I128(number), rust::Pattern::I64)
            }
            CaseNumber::Unsigned(number) => {
                i64::try_from(number).map_or(rust::Pattern::U128(number), rust::Pattern::I64)
            }
        }));
    };
    let end = case_number(end)?;
    if let (Some(start), Some(end)) = (start.signed(), end.signed()) {
        return Ok((start <= end).then_some(rust::Pattern::InclusiveRange { start, end }));
    }
    match (start.unsigned(), end.unsigned()) {
        (Some(start), Some(end)) => {
            Ok((start <= end).then_some(rust::Pattern::InclusiveRangeU128 { start, end }))
        }
        _ => Err(unsupported_switch("case range")),
    }
}

fn falls_through(arm: &SwitchArm) -> bool {
    !arm.body
        .iter()
        .rev()
        .find(|statement| !matches!(statement.value, ir::Statement::Comment(_)))
        .is_some_and(|last| ends_in_jump(last))
}

fn unsupported_switch(detail: &str) -> Failure {
    Construct::Switch {
        detail: detail.into(),
    }
    .into()
}

fn at_arm(
    failure: Failure,
    statement: Option<&&slate_parser::ast::Span<ir::Statement>>,
) -> Failure {
    match statement {
        Some(statement) => failure.at(Site::of(*statement)),
        None => failure,
    }
}
