use super::*;

pub(super) struct SwitchArm<'a> {
    values: Vec<&'a ir::Value>,
    default: bool,
    body: Vec<&'a ir::Statement>,
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
        } if *owner == switch => {
            if end.is_some() {
                return Err(super::Error::Unsupported("switch case range".into()));
            }
            (Some(start), body)
        }
        ir::Statement::Default {
            switch: owner,
            body,
        } if *owner == switch => (None, body),
        _ => return Ok(None),
    };
    let mut arm = match body.first() {
        Some(first) => match switch_label(first, switch)? {
            Some(mut nested) => {
                nested
                    .body
                    .extend(body[1..].iter().map(|statement| &**statement));
                nested
            }
            None => SwitchArm {
                values: Vec::new(),
                default: false,
                body: body.iter().map(|statement| &**statement).collect(),
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
        ir::Statement::Block(body) => body.last().is_some_and(|last| ends_in_jump(last)),
        _ => false,
    }
}

pub(super) fn lower_switch(
    id: BindingId,
    discriminant: &ir::Value,
    body: &[slate_parser::ast::Span<ir::Statement>],
    cx: &Context,
) -> Result<Stmt> {
    let statements = match body {
        [single] => match &**single {
            ir::Statement::Block(inner) => inner.as_slice(),
            _ => body,
        },
        _ => body,
    };
    let mut arms: Vec<SwitchArm> = Vec::new();
    for statement in statements {
        match switch_label(statement, id)? {
            Some(arm) => arms.push(arm),
            None => match arms.last_mut() {
                Some(arm) => arm.body.push(statement),
                None => {
                    return Err(super::Error::Unsupported(
                        "statement before first switch case".into(),
                    ));
                }
            },
        }
    }
    for (index, arm) in arms.iter().enumerate() {
        if contains_switch_label(&arm.body, id) {
            return Err(super::Error::Unsupported("nested switch case label".into()));
        }
        if !arm.body.last().is_some_and(|last| ends_in_jump(last)) && index + 1 < arms.len() {
            return Err(super::Error::Unsupported("switch fallthrough".into()));
        }
    }
    let selector = cx.next_temp();
    let lower_arm = |arm: &SwitchArm| -> Result<Vec<Stmt>> {
        arm.body
            .iter()
            .map(|statement| lower_statement(statement, cx))
            .collect()
    };
    let mut chain = match arms.iter().find(|arm| arm.default) {
        Some(arm) => lower_arm(arm)?,
        None => Vec::new(),
    };
    for arm in arms.iter().rev().filter(|arm| !arm.default) {
        let mut cond: Option<Expr> = None;
        for value in &arm.values {
            let test = Expr::Binary {
                op: BinOp::Eq,
                lhs: Box::new(Expr::Var(selector.clone().into())),
                rhs: Box::new(lower_value(value, cx)?),
            };
            cond = Some(match cond {
                Some(previous) => Expr::Binary {
                    op: BinOp::Or,
                    lhs: Box::new(previous),
                    rhs: Box::new(test),
                },
                None => test,
            });
        }
        let Some(cond) = cond else {
            continue;
        };
        chain = vec![Stmt::If {
            cond,
            then_body: lower_arm(arm)?,
            else_body: chain,
        }];
    }
    let mut block = vec![Stmt::Let {
        name: selector,
        mutable: false,
        ty: Some(lower_type(cx, &discriminant.ty)?),
        init: Some(lower_value(discriminant, cx)?),
    }];
    block.extend(chain);
    Ok(Stmt::LabeledBlock {
        label: break_label(id),
        body: block,
    })
}
