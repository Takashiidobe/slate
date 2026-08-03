use std::collections::BTreeMap;

use crate::function_identity::{CallBinding, FunctionIdentity, Known};
use crate::rust_ast::{BinOp, Expr, IndentStmt, Prim, RustValue, Stmt, Type, UnaryOp};

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum Classification {
    Alpha,
    Digit,
    Space,
    Upper,
    Lower,
    Alnum,
    XDigit,
    Punct,
    Cntrl,
    Graph,
    Print,
}

impl Classification {
    pub(super) fn requires_locale_check(self) -> bool {
        !matches!(self, Self::Digit | Self::XDigit)
    }
}

pub(super) struct ClassifyPlan {
    pub(super) classification: Classification,
    pub(super) arg: Expr,
    pub(super) negate: bool,
}

struct LetInfo<'a> {
    init: &'a Expr,
    ty: Option<&'a Type>,
}

pub(super) struct ClassifyEnv<'a> {
    lets: BTreeMap<&'a str, LetInfo<'a>>,
}

impl<'a> ClassifyEnv<'a> {
    pub(super) fn from_body(body: &'a [IndentStmt]) -> Self {
        let mut lets = BTreeMap::new();
        for indent in body {
            if let Stmt::Let {
                name,
                init: Some(init),
                ty,
                ..
            } = &indent.stmt
            {
                lets.insert(
                    name.as_str(),
                    LetInfo {
                        init,
                        ty: ty.as_ref(),
                    },
                );
            }
        }
        Self { lets }
    }

    fn declared_byte_type(&self, name: &str) -> bool {
        matches!(
            self.lets.get(name).and_then(|info| info.ty),
            Some(Type::Prim(Prim::I8 | Prim::U8))
        )
    }
}

pub(super) fn classify_plan(expr: &Expr, env: &ClassifyEnv<'_>) -> Option<ClassifyPlan> {
    let (outer_negate, expr) = match expr {
        Expr::Unary {
            op: UnaryOp::Not,
            expr,
        } => (true, expr.as_ref()),
        other => (false, other),
    };
    let Expr::Binary { op, lhs, rhs } = expr else {
        return None;
    };
    let negate = match op {
        BinOp::Ne => false,
        BinOp::Eq => true,
        _ => return None,
    } ^ outer_negate;
    let call_operand = match (int_value(lhs), int_value(rhs)) {
        (Some(0), None) => rhs.as_ref(),
        (None, Some(0)) => lhs.as_ref(),
        _ => return None,
    };
    let call_expr = resolve_call_expr(call_operand, env)?;
    let Expr::Call { args, binding, .. } = call_expr else {
        return None;
    };
    let classification = direct_call_classification(binding)?;
    let [arg] = args.as_slice() else {
        return None;
    };

    let arg = peel_casts(arg);
    let arg_name = var_name(arg)?;
    if !env.declared_byte_type(arg_name) {
        return None;
    }

    Some(ClassifyPlan {
        classification,
        arg: arg.clone(),
        negate,
    })
}

fn direct_call_classification(binding: &CallBinding) -> Option<Classification> {
    let CallBinding::Direct {
        identity: FunctionIdentity::Known(known),
        ..
    } = binding
    else {
        return None;
    };
    Some(match known {
        Known::IsAlpha => Classification::Alpha,
        Known::IsDigit => Classification::Digit,
        Known::IsSpace => Classification::Space,
        Known::IsUpper => Classification::Upper,
        Known::IsLower => Classification::Lower,
        Known::IsAlnum => Classification::Alnum,
        Known::IsXDigit => Classification::XDigit,
        Known::IsPunct => Classification::Punct,
        Known::IsCntrl => Classification::Cntrl,
        Known::IsGraph => Classification::Graph,
        Known::IsPrint => Classification::Print,
        _ => return None,
    })
}

fn resolve_call_expr<'a>(operand: &'a Expr, env: &ClassifyEnv<'a>) -> Option<&'a Expr> {
    let peeled = peel_casts(operand);
    if let Expr::Var(name) = peeled {
        return env
            .lets
            .get(name.as_str())
            .map(|info| trivial_unsafe_tail(info.init).unwrap_or(info.init));
    }
    Some(peeled)
}

fn trivial_unsafe_tail(expr: &Expr) -> Option<&Expr> {
    match expr {
        Expr::Unsafe(block) if block.stmts.is_empty() => block.tail.as_deref(),
        other => Some(other),
    }
}

fn var_name(expr: &Expr) -> Option<&str> {
    match peel_casts(expr) {
        Expr::Var(name) => Some(name.as_str()),
        _ => None,
    }
}

fn peel_casts(expr: &Expr) -> &Expr {
    match expr {
        Expr::Cast { expr, .. } => peel_casts(expr),
        _ => expr,
    }
}

fn int_value(expr: &Expr) -> Option<i64> {
    match peel_casts(expr) {
        Expr::Value(RustValue::I64(value)) => Some(*value),
        Expr::Value(RustValue::I128(value)) => i64::try_from(*value).ok(),
        Expr::Value(RustValue::Usize(value)) => i64::try_from(*value).ok(),
        _ => None,
    }
}
