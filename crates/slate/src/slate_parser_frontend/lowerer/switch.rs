use super::*;

pub(super) struct SwitchArm<'a> {
    values: Vec<&'a ir::Value>,
    default: bool,
    body: Vec<&'a slate_parser::ast::Span<ir::Statement>>,
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
                return Err(unsupported_switch("case range"));
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
                nested.body.extend(&body[1..]);
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
        ir::Statement::Block(body) => body.last().is_some_and(|last| ends_in_jump(last)),
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
        for statement in statements {
            match switch_label(statement, id)? {
                Some(arm) => arms.push(arm),
                None => match arms.last_mut() {
                    Some(arm) => arm.body.push(statement),
                    None => {
                        return Err(unsupported_switch("statement before first case"));
                    }
                },
            }
        }
        for (index, arm) in arms.iter().enumerate() {
            if contains_switch_label(
                &arm.body
                    .iter()
                    .map(|statement| &statement.value)
                    .collect::<Vec<_>>(),
                id,
            ) {
                return Err(unsupported_switch("nested case label"));
            }
            if !arm.body.last().is_some_and(|last| ends_in_jump(last)) && index + 1 < arms.len() {
                return Err(unsupported_switch("fallthrough"));
            }
        }
        let selector = self.next_temp();
        let mut chain = match arms.iter().find(|arm| arm.default) {
            Some(arm) => self.lower_switch_arm(arm)?,
            None => Vec::new(),
        };
        for arm in arms.iter().rev().filter(|arm| !arm.default) {
            let mut cond: Option<Expr> = None;
            for value in &arm.values {
                let test = Expr::Binary {
                    op: BinOp::Eq,
                    lhs: Box::new(Expr::Var(selector.clone().into())),
                    rhs: Box::new(self.lower_value(value)?),
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
                then_body: self.lower_switch_arm(arm)?,
                else_body: chain,
            }];
        }
        let mut block = vec![Stmt::Let {
            name: selector,
            mutable: false,
            ty: Some(self.lower_type(&discriminant.ty)?),
            init: Some(self.lower_value(discriminant)?),
        }];
        block.extend(chain);
        Ok(Stmt::LabeledBlock {
            label: break_label(id),
            body: block,
        })
    }

    fn lower_switch_arm(&mut self, arm: &SwitchArm) -> Result<Vec<Stmt>> {
        arm.body
            .iter()
            .map(|statement| self.lower_spanned_statement(statement))
            .collect()
    }
}

fn unsupported_switch(detail: &str) -> Failure {
    Construct::Switch {
        detail: detail.into(),
    }
    .into()
}
