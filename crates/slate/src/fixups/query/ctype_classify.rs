use std::collections::BTreeMap;

use crate::function_identity::{CallBinding, FunctionIdentity, Known};
use crate::rust_ast::{BinOp, Expr, IndentStmt, Prim, RustValue, Stmt, Type, UnaryOp};

use super::super::support::walk::body_expr_any;

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
    fn from_mask(mask: i64) -> Option<Self> {
        Some(match mask {
            1024 => Self::Alpha,
            2048 => Self::Digit,
            8192 => Self::Space,
            256 => Self::Upper,
            512 => Self::Lower,
            8 => Self::Alnum,
            4096 => Self::XDigit,
            4 => Self::Punct,
            2 => Self::Cntrl,
            32768 => Self::Graph,
            16384 => Self::Print,
            _ => return None,
        })
    }

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
    body: &'a [IndentStmt],
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
        Self { body, lets }
    }

    fn use_count(&self, name: &str) -> usize {
        let mut count = 0;
        body_expr_any(self.body, &mut |expr| {
            if matches!(expr, Expr::Var(candidate) if candidate.as_str() == name) {
                count += 1;
            }
            false
        });
        count
    }

    fn single_use_init(&self, name: &str) -> Option<&'a Expr> {
        (self.use_count(name) == 1).then_some(())?;
        self.lets.get(name).map(|info| info.init)
    }

    fn declared_byte_type(&self, name: &str) -> bool {
        matches!(
            self.lets.get(name).and_then(|info| info.ty),
            Some(Type::Prim(Prim::I8 | Prim::U8))
        )
    }
}

/// `expr` must be the *comparison against zero* that consumes the bitmask
/// value (`mask_expr != 0`, `0 == mask_expr`, ...), never the bare masked
/// value itself: C's classification functions return an unspecified nonzero
/// value (often the raw mask bit, not `1`), so collapsing straight to `bool`
/// would change output for code that prints or stores the raw return value.
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
    let and_operand = match (int_value(lhs), int_value(rhs)) {
        (Some(0), None) => rhs.as_ref(),
        (None, Some(0)) => lhs.as_ref(),
        _ => return None,
    };
    let and_expr = resolve_and_expr(and_operand, env)?;
    let Expr::Binary {
        op: BinOp::BitAnd,
        lhs: mask_lhs,
        rhs: mask_rhs,
    } = and_expr
    else {
        return None;
    };
    let mask = int_value(mask_rhs)?;
    let classification = Classification::from_mask(mask)?;

    let bits_name = var_name(peel_casts(mask_lhs))?;
    let bits_init = env.single_use_init(bits_name)?;
    let (row_name, idx) = deref_offset(trivial_unsafe_tail(bits_init)?)?;
    let row_init = env.single_use_init(row_name)?;
    let table_name = deref_var(trivial_unsafe_tail(row_init)?)?;
    let table_init = env.single_use_init(table_name)?;
    is_ctype_b_loc_call(trivial_unsafe_tail(table_init)?)?;

    let arg = peel_casts(idx);
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

/// The bitmask-and value being compared to zero: either an explicit
/// single-use temporary (the common case) or the `& mask` expression fused
/// directly into the comparison with no intermediate binding.
fn resolve_and_expr<'a>(operand: &'a Expr, env: &ClassifyEnv<'a>) -> Option<&'a Expr> {
    let peeled = peel_casts(operand);
    if let Expr::Var(name) = peeled {
        return env
            .single_use_init(name.as_str())
            .map(|init| trivial_unsafe_tail(init).unwrap_or(init));
    }
    Some(peeled)
}

fn trivial_unsafe_tail(expr: &Expr) -> Option<&Expr> {
    match expr {
        Expr::Unsafe(block) if block.stmts.is_empty() => block.tail.as_deref(),
        other => Some(other),
    }
}

fn deref_offset(expr: &Expr) -> Option<(&str, &Expr)> {
    let Expr::Unary {
        op: UnaryOp::Deref,
        expr,
    } = expr
    else {
        return None;
    };
    let Expr::MethodCall { recv, method, args } = expr.as_ref() else {
        return None;
    };
    if method != "offset" || args.len() != 1 {
        return None;
    }
    Some((var_name(recv)?, &args[0]))
}

fn deref_var(expr: &Expr) -> Option<&str> {
    let Expr::Unary {
        op: UnaryOp::Deref,
        expr,
    } = expr
    else {
        return None;
    };
    var_name(expr)
}

fn is_ctype_b_loc_call(expr: &Expr) -> Option<()> {
    let Expr::Call { args, binding, .. } = expr else {
        return None;
    };
    if !args.is_empty() {
        return None;
    }
    matches!(
        binding,
        CallBinding::Direct {
            identity: FunctionIdentity::Known(Known::CtypeBLoc),
            ..
        }
    )
    .then_some(())
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
