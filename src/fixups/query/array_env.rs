use std::collections::BTreeMap;

use crate::rust_ast::{BinOp, Expr, IndentStmt, Prim, RustValue, Stmt, Type};

#[derive(Clone)]
pub(super) struct CopyEnv {
    pub(super) arrays: BTreeMap<String, ArrayInfo>,
    pub(super) constants: BTreeMap<String, u64>,
}

#[derive(Clone)]
pub(super) struct ArrayInfo {
    pub(super) mutable: bool,
    pub(super) len: u64,
    pub(super) elem_size: u64,
    pub(super) byte_signed: Option<bool>,
}

#[derive(Clone)]
pub(super) struct CopyEndpoint {
    pub(super) base: String,
    pub(super) start: u64,
}

impl CopyEnv {
    pub(super) fn new() -> Self {
        Self {
            arrays: BTreeMap::new(),
            constants: BTreeMap::new(),
        }
    }

    pub(super) fn extend(&mut self, other: Self) {
        self.arrays.extend(other.arrays);
        self.constants.extend(other.constants);
    }

    pub(super) fn from_body(body: &[IndentStmt]) -> Self {
        let mut env = Self {
            arrays: BTreeMap::new(),
            constants: BTreeMap::new(),
        };
        for indent in body {
            let Stmt::Let {
                name,
                mutable,
                ty,
                init,
            } = &indent.stmt
            else {
                continue;
            };
            if let Some(Type::Array { elem, len }) = ty.as_ref().map(|ty| ty.peel_aligned())
                && let Some(elem_size) = type_size(elem)
            {
                env.arrays.insert(
                    name.clone(),
                    ArrayInfo {
                        mutable: *mutable,
                        len: *len,
                        elem_size,
                        byte_signed: match &**elem {
                            Type::Prim(Prim::I8) => Some(true),
                            Type::Prim(Prim::U8) => Some(false),
                            _ => None,
                        },
                    },
                );
            }
            if let Some(value) = init.as_ref().and_then(int_value).and_then(nonnegative) {
                env.constants.insert(name.clone(), value);
            }
        }
        env
    }
}

pub(super) fn endpoint(expr: &Expr) -> Option<CopyEndpoint> {
    match peel_casts(expr) {
        Expr::ArrayPtr { array, .. } => {
            let Expr::Var(base) = &**array else {
                return None;
            };
            Some(CopyEndpoint {
                base: base.to_string(),
                start: 0,
            })
        }
        Expr::MethodCall { recv, method, args } if method == "add" && args.len() == 1 => {
            let mut endpoint = endpoint(recv)?;
            endpoint.start = endpoint
                .start
                .checked_add(nonnegative(int_value(&args[0])?)?)?;
            Some(endpoint)
        }
        Expr::MethodCall { recv, method, args } if method == "as_mut_ptr" && args.is_empty() => {
            let Expr::Var(base) = &**recv else {
                return None;
            };
            Some(CopyEndpoint {
                base: base.to_string(),
                start: 0,
            })
        }
        Expr::MethodCall { recv, method, args } if method == "as_ptr" && args.is_empty() => {
            let Expr::Var(base) = &**recv else {
                return None;
            };
            Some(CopyEndpoint {
                base: base.to_string(),
                start: 0,
            })
        }
        _ => None,
    }
}

pub(super) fn peel_casts(expr: &Expr) -> &Expr {
    match expr {
        Expr::Cast { expr, .. } => peel_casts(expr),
        Expr::Unsafe(block) if block.stmts.is_empty() => match block.tail.as_deref() {
            Some(inner) => peel_casts(inner),
            None => expr,
        },
        _ => expr,
    }
}

pub(super) fn count_value(expr: &Expr, env: &CopyEnv) -> Option<u64> {
    match peel_casts(expr) {
        Expr::Var(name) => env.constants.get(name.as_str()).copied(),
        expr => int_value(expr).and_then(nonnegative),
    }
}

fn int_value(expr: &Expr) -> Option<i128> {
    match expr {
        Expr::Value(RustValue::I64(value)) => Some(*value as i128),
        Expr::Value(RustValue::I128(value)) => Some(*value),
        Expr::Value(RustValue::Usize(value)) => Some(*value as i128),
        Expr::Cast { expr, .. } => int_value(expr),
        Expr::Binary { op, lhs, rhs } => {
            let lhs = int_value(lhs)?;
            let rhs = int_value(rhs)?;
            Some(match op {
                BinOp::Add => lhs.checked_add(rhs)?,
                BinOp::Sub => lhs.checked_sub(rhs)?,
                BinOp::Mul => lhs.checked_mul(rhs)?,
                _ => return None,
            })
        }
        _ => None,
    }
}

fn nonnegative(value: i128) -> Option<u64> {
    (value >= 0).then_some(value as u64)
}

fn type_size(ty: &Type) -> Option<u64> {
    match ty {
        Type::Prim(Prim::I8 | Prim::U8 | Prim::Bool) => Some(1),
        Type::Prim(Prim::I16 | Prim::U16) => Some(2),
        Type::Prim(Prim::I32 | Prim::U32) => Some(4),
        Type::Prim(Prim::I64 | Prim::U64) => Some(8),
        Type::Prim(Prim::I128 | Prim::U128) => Some(16),
        _ => None,
    }
}

pub(super) fn slice_index(base: &str, start: u64, end: u64) -> Expr {
    Expr::Index {
        base: Box::new(Expr::Var(base.into())),
        index: Box::new(range(start, end)),
    }
}

pub(super) fn range(start: u64, end: u64) -> Expr {
    Expr::Range {
        start: Box::new(uint(start)),
        end: Box::new(uint(end)),
    }
}

pub(super) fn uint(value: u64) -> Expr {
    Expr::Value(RustValue::I64(value as i64))
}
