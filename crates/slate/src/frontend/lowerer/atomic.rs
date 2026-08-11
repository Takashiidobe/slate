use super::*;

pub(super) fn atomic_type(rust_ty: &Type) -> Option<AtomicType> {
    Some(match rust_ty {
        Type::Prim(Prim::I8) => AtomicType::I8,
        Type::Prim(Prim::U8) => AtomicType::U8,
        Type::Prim(Prim::I16) => AtomicType::I16,
        Type::Prim(Prim::U16) => AtomicType::U16,
        Type::Prim(Prim::I32) => AtomicType::I32,
        Type::Prim(Prim::U32) => AtomicType::U32,
        Type::Prim(Prim::I64) => AtomicType::I64,
        Type::Prim(Prim::U64) => AtomicType::U64,
        Type::Prim(Prim::Isize) => AtomicType::Isize,
        Type::Prim(Prim::Usize) => AtomicType::Usize,
        Type::Prim(Prim::Bool) => AtomicType::Bool,
        _ => return None,
    })
}

pub(super) fn atomic_rmw_op(binop: i64) -> AtomicRmwOp {
    match binop {
        0 => AtomicRmwOp::Add,
        1 => AtomicRmwOp::Sub,
        2 => AtomicRmwOp::And,
        3 => AtomicRmwOp::Xor,
        4 => AtomicRmwOp::Or,
        5 => AtomicRmwOp::Nand,
        6 => AtomicRmwOp::Max,
        _ => AtomicRmwOp::Min,
    }
}

// combine old value and operand to the value an atomic fetch op stores.
pub(super) fn atomic_combine(binop: i64, old: Expr, val: Expr) -> Expr {
    match binop {
        0 => Expr::Binary {
            op: BinOp::Add,
            lhs: Box::new(old),
            rhs: Box::new(val),
        },
        1 => Expr::Binary {
            op: BinOp::Sub,
            lhs: Box::new(old),
            rhs: Box::new(val),
        },
        2 => Expr::Binary {
            op: BinOp::BitAnd,
            lhs: Box::new(old),
            rhs: Box::new(val),
        },
        3 => Expr::Binary {
            op: BinOp::BitXor,
            lhs: Box::new(old),
            rhs: Box::new(val),
        },
        4 => Expr::Binary {
            op: BinOp::BitOr,
            lhs: Box::new(old),
            rhs: Box::new(val),
        },
        5 => Expr::Unary {
            op: UnaryOp::Not,
            expr: Box::new(Expr::Binary {
                op: BinOp::BitAnd,
                lhs: Box::new(old),
                rhs: Box::new(val),
            }),
        },
        6 => Expr::MethodCall {
            recv: Box::new(old),
            method: "max".into(),
            args: vec![val],
        },
        _ => Expr::MethodCall {
            recv: Box::new(old),
            method: "min".into(),
            args: vec![val],
        },
    }
}

// C `memory_order` (0 relaxed,1 consume,2 acquire,3 release,4 acq_rel,5 seq_cst)
// mapped to a fully-qualified Rust `Ordering`. Rust lacks Consume; use Acquire.
pub(super) fn rust_ordering(mem_order: i64) -> AtomicOrdering {
    match mem_order {
        0 => AtomicOrdering::Relaxed,
        1 | 2 => AtomicOrdering::Acquire,
        3 => AtomicOrdering::Release,
        4 => AtomicOrdering::AcqRel,
        _ => AtomicOrdering::SeqCst,
    }
}

// loads reject Release/AcqRel; clamp to a load-valid ordering.
pub(super) fn load_ordering(mem_order: i64) -> AtomicOrdering {
    match mem_order {
        0 => AtomicOrdering::Relaxed,
        1 | 2 => AtomicOrdering::Acquire,
        _ => AtomicOrdering::SeqCst,
    }
}

// stores reject Acquire/AcqRel; clamp to a store-valid ordering.
pub(super) fn store_ordering(mem_order: i64) -> AtomicOrdering {
    match mem_order {
        0 => AtomicOrdering::Relaxed,
        3 => AtomicOrdering::Release,
        _ => AtomicOrdering::SeqCst,
    }
}
