use crate::ast::{Expr, ExprKind, InitDeclarator, Initializer, Stmt, StmtKind};
use crate::const_expr::{AssignOp, BinaryOp, PostfixOp, UnaryOp};
use crate::ir::{BindingId, CompareOp};
use crate::visit::{self, Visitor};
use std::collections::HashMap;

use super::ctype::QualType;
use super::types::TypeResolver;

pub(super) fn step_type(types: &mut TypeResolver, variable: QualType) -> QualType {
    if types.ctypes.is_pointer(variable) {
        types.ptrdiff_type()
    } else {
        types.ctypes.unqualified(variable)
    }
}

pub(super) fn mentions(
    expr: &Expr,
    references: &HashMap<crate::ast::NodeId, BindingId>,
    bindings: &[BindingId],
) -> bool {
    struct Finder<'a> {
        references: &'a HashMap<crate::ast::NodeId, BindingId>,
        bindings: &'a [BindingId],
    }
    impl Visitor for Finder<'_> {
        type Error = ();
        fn visit_expr(&mut self, expr: &Expr) -> Result<(), ()> {
            if self
                .references
                .get(&expr.id)
                .is_some_and(|id| self.bindings.contains(id))
            {
                return Err(());
            }
            visit::walk_expr(self, expr)
        }
    }
    Finder {
        references,
        bindings,
    }
    .visit_expr(expr)
    .is_err()
}

pub(super) struct CanonicalLoop<'a> {
    pub(super) variable: BindingId,
    pub(super) lower: &'a Expr,
    pub(super) compare: CompareOp,
    pub(super) upper: &'a Expr,
    pub(super) step: Option<&'a Expr>,
    pub(super) decrement: bool,
}

pub(super) struct LoopError<'a> {
    pub(super) at: &'a Stmt,
    pub(super) expr: Option<&'a Expr>,
    pub(super) message: &'static str,
}

pub(super) fn unparenthesized(mut expr: &Expr) -> &Expr {
    while let ExprKind::Paren(inner) = &expr.value {
        expr = inner;
    }
    expr
}

pub(super) fn assigned_variable(init: Option<&Stmt>) -> Option<(&Expr, &Expr)> {
    let StmtKind::Expr(expr) = &init?.value else {
        return None;
    };
    let ExprKind::Assign {
        op: AssignOp::Assign,
        target,
        value,
    } = &unparenthesized(expr).value
    else {
        return None;
    };
    let target = unparenthesized(target);
    matches!(target.value, ExprKind::Identifier(_)).then_some((target, value))
}

pub(super) fn canonical_loop<'a>(
    statement: &'a Stmt,
    reference: impl Fn(&Expr) -> Option<BindingId>,
    declaration: impl Fn(&InitDeclarator) -> Option<BindingId>,
) -> Result<CanonicalLoop<'a>, LoopError<'a>> {
    let fail = |expr: Option<&'a Expr>, message| LoopError {
        at: statement,
        expr,
        message,
    };
    let StmtKind::For {
        init,
        condition,
        increment,
        ..
    } = &statement.value
    else {
        return Err(fail(
            None,
            "statement after an OpenMP loop directive must be a for loop",
        ));
    };
    let invalid_init =
        "invalid initialization in OpenMP `for` loop: expected `var = init` or `T var = init`";
    let (variable, lower) = match init.as_deref().map(|init| &init.value) {
        Some(StmtKind::Decl(declared)) => match &declared.declarators[..] {
            [declarator] => match &declarator.initializer {
                Some(Initializer::Expr(lower)) => (
                    declaration(declarator).ok_or_else(|| fail(None, invalid_init))?,
                    lower,
                ),
                _ => return Err(fail(None, invalid_init)),
            },
            _ => return Err(fail(None, invalid_init)),
        },
        _ => {
            let (target, value) =
                assigned_variable(init.as_deref()).ok_or_else(|| fail(None, invalid_init))?;
            (
                reference(target).ok_or_else(|| fail(Some(target), invalid_init))?,
                value,
            )
        }
    };
    let is_variable = |expr: &Expr| {
        matches!(unparenthesized(expr).value, ExprKind::Identifier(_))
            && reference(unparenthesized(expr)) == Some(variable)
    };
    let invalid_condition = "invalid controlling predicate in OpenMP `for` loop";
    let condition = condition
        .as_ref()
        .ok_or_else(|| fail(None, "missing controlling predicate in OpenMP `for` loop"))?;
    let ExprKind::Binary { op, left, right } = &unparenthesized(condition).value else {
        return Err(fail(Some(condition), invalid_condition));
    };
    let compare = match op {
        BinaryOp::Less => CompareOp::Lt,
        BinaryOp::LessEqual => CompareOp::Le,
        BinaryOp::Greater => CompareOp::Gt,
        BinaryOp::GreaterEqual => CompareOp::Ge,
        BinaryOp::NotEqual => CompareOp::Ne,
        _ => return Err(fail(Some(condition), invalid_condition)),
    };
    let (compare, upper) = if is_variable(left) {
        (compare, right)
    } else if is_variable(right) {
        let swapped = match compare {
            CompareOp::Lt => CompareOp::Gt,
            CompareOp::Le => CompareOp::Ge,
            CompareOp::Gt => CompareOp::Lt,
            CompareOp::Ge => CompareOp::Le,
            other => other,
        };
        (swapped, left)
    } else {
        return Err(fail(Some(condition), invalid_condition));
    };
    let invalid_increment = "invalid increment expression in OpenMP `for` loop";
    let increment = increment
        .as_ref()
        .ok_or_else(|| fail(None, "missing increment expression in OpenMP `for` loop"))?;
    let (step, decrement) = match &unparenthesized(increment).value {
        ExprKind::Unary {
            op: op @ (UnaryOp::PreIncrement | UnaryOp::PreDecrement),
            operand,
        } if is_variable(operand) => (None, *op == UnaryOp::PreDecrement),
        ExprKind::Postfix { op, operand } if is_variable(operand) => {
            (None, *op == PostfixOp::Decrement)
        }
        ExprKind::Assign {
            op: op @ (AssignOp::AddAssign | AssignOp::SubAssign),
            target,
            value,
        } if is_variable(target) => (Some(value), *op == AssignOp::SubAssign),
        ExprKind::Assign {
            op: AssignOp::Assign,
            target,
            value,
        } if is_variable(target) => match &unparenthesized(value).value {
            ExprKind::Binary {
                op: BinaryOp::Add,
                left,
                right,
            } if is_variable(left) => (Some(right), false),
            ExprKind::Binary {
                op: BinaryOp::Add,
                left,
                right,
            } if is_variable(right) => (Some(left), false),
            ExprKind::Binary {
                op: BinaryOp::Sub,
                left,
                right,
            } if is_variable(left) => (Some(right), true),
            _ => return Err(fail(Some(increment), invalid_increment)),
        },
        _ => return Err(fail(Some(increment), invalid_increment)),
    };
    Ok(CanonicalLoop {
        variable,
        lower,
        compare,
        upper,
        step,
        decrement,
    })
}
