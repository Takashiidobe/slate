use crate::backend::rust_ast::{
    BinOp, Block, Expr, FnDef, Ident, Item, Prim, RustValue, Stmt, Type,
};
use std::collections::BTreeMap;

pub struct Hoisted {
    pub function: FnDef,
    pub bindings: Vec<Item>,
}

enum Slot<'a> {
    Expr(&'a mut Expr, Option<&'a Type>),
    Type(&'a mut Type),
}

#[derive(PartialEq, Clone)]
enum Value {
    Literal(Expr),
    Prim(Prim),
}

struct Leaf {
    value: Value,
    const_ty: Option<Type>,
}

fn walk_type(ty: &mut Type, f: &mut impl FnMut(Slot<'_>)) {
    f(Slot::Type(ty));
    match ty {
        Type::Ref { inner, .. } | Type::Ptr { inner, .. } => walk_type(inner, f),
        Type::Slice(inner) | Type::Complex(inner) => walk_type(inner, f),
        Type::Array { elem, .. } => walk_type(elem, f),
        Type::Generic { args, .. } => args.iter_mut().for_each(|arg| walk_type(arg, f)),
        Type::FnPtr { params, ret, .. } => {
            params.iter_mut().for_each(|param| walk_type(param, f));
            walk_type(ret, f);
        }
        _ => {}
    }
}

fn walk_exprs<'a>(
    exprs: impl IntoIterator<Item = &'a mut Expr>,
    ret: Option<&Type>,
    f: &mut impl FnMut(Slot<'_>),
) {
    for expr in exprs {
        walk_expr(expr, None, ret, f);
    }
}

fn walk_block(block: &mut Block, ret: Option<&Type>, f: &mut impl FnMut(Slot<'_>)) {
    walk_stmts(&mut block.stmts, ret, f);
    if let Some(tail) = &mut block.tail {
        walk_expr(tail, None, ret, f);
    }
}

fn walk_expr(
    expr: &mut Expr,
    expected: Option<&Type>,
    ret: Option<&Type>,
    f: &mut impl FnMut(Slot<'_>),
) {
    f(Slot::Expr(expr, expected));
    match expr {
        Expr::Unary { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::ConstBlock(expr) => walk_expr(expr, None, ret, f),
        Expr::Field { base, .. } | Expr::TupleField { base, .. } => walk_expr(base, None, ret, f),
        Expr::ArrayPtr { array, .. } => walk_expr(array, None, ret, f),
        Expr::Binary { op, lhs, rhs } => {
            let operands = expected.filter(|_| {
                matches!(
                    op,
                    BinOp::Add
                        | BinOp::Sub
                        | BinOp::Mul
                        | BinOp::Div
                        | BinOp::Rem
                        | BinOp::BitAnd
                        | BinOp::BitOr
                        | BinOp::BitXor
                )
            });
            walk_expr(lhs, operands, ret, f);
            walk_expr(rhs, operands, ret, f);
        }
        Expr::Range { start, end } => walk_exprs([&mut **start, &mut **end], ret, f),
        Expr::Index { base, index } => walk_exprs([&mut **base, &mut **index], ret, f),
        Expr::Call { func, args, .. } => {
            walk_expr(func, None, ret, f);
            walk_exprs(args, ret, f);
        }
        Expr::MethodCall { recv, args, .. } => {
            walk_expr(recv, None, ret, f);
            walk_exprs(args, ret, f);
        }
        Expr::MethodCallGeneric {
            recv,
            type_args,
            args,
            ..
        } => {
            walk_expr(recv, None, ret, f);
            type_args.iter_mut().for_each(|ty| walk_type(ty, f));
            walk_exprs(args, ret, f);
        }
        Expr::StructLit { fields, .. } => {
            walk_exprs(fields.iter_mut().map(|(_, value)| value), ret, f);
        }
        Expr::TupleStructLit { fields, .. } => walk_exprs(fields, ret, f),
        Expr::ArrayLit(items) | Expr::VecLit(items) => walk_exprs(items, ret, f),
        Expr::ArrayRepeat { elem, .. } => walk_expr(elem, None, ret, f),
        Expr::VecRepeat { elem, len } => walk_exprs([&mut **elem, &mut **len], ret, f),
        Expr::Macro { name, args } if matches!(name.as_str(), "print" | "println") => {
            walk_exprs(args, ret, f);
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            walk_expr(cond, None, ret, f);
            walk_expr(then_expr, expected, ret, f);
            walk_expr(else_expr, expected, ret, f);
        }
        Expr::Block(block) | Expr::Unsafe(block) => walk_block(block, ret, f),
        Expr::Cast { expr, ty } => {
            let operand = is_literal(expr).then_some(&*ty);
            walk_expr(expr, operand, ret, f);
            walk_type(ty, f);
        }
        Expr::Transmute { from, to, expr } => {
            walk_type(from, f);
            walk_type(to, f);
            walk_expr(expr, None, ret, f);
        }
        Expr::GenericPath { type_args, .. } => {
            type_args.iter_mut().for_each(|ty| walk_type(ty, f));
        }
        _ => {}
    }
}

fn walk_stmts(stmts: &mut [Stmt], ret: Option<&Type>, f: &mut impl FnMut(Slot<'_>)) {
    for stmt in stmts {
        match stmt {
            Stmt::Let { ty, init, .. } => {
                if let Some(init) = init {
                    walk_expr(init, ty.as_ref(), ret, f);
                }
                if let Some(ty) = ty {
                    walk_type(ty, f);
                }
            }
            Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
                walk_exprs([target, value], ret, f);
            }
            Stmt::Expr(expr) => walk_expr(expr, None, ret, f),
            Stmt::Return(Some(expr)) => walk_expr(expr, ret, ret, f),
            Stmt::Unsafe { body } | Stmt::Block(body) => walk_block(body, ret, f),
            Stmt::If {
                cond,
                then_body,
                else_body,
            } => {
                walk_expr(cond, None, ret, f);
                walk_stmts(then_body, ret, f);
                walk_stmts(else_body, ret, f);
            }
            Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
                walk_stmts(body, ret, f);
            }
            Stmt::For { iter, body, .. } => {
                walk_expr(iter, None, ret, f);
                walk_stmts(body, ret, f);
            }
            Stmt::While { cond, body, .. } => {
                walk_expr(cond, None, ret, f);
                walk_block(body, ret, f);
            }
            Stmt::Match { expr, arms } => {
                walk_expr(expr, None, ret, f);
                for arm in arms {
                    walk_stmts(&mut arm.body, ret, f);
                }
            }
            _ => {}
        }
    }
}

fn walk_fn(function: &mut FnDef, f: &mut impl FnMut(Slot<'_>)) {
    let ret = function.ret.clone();
    for param in &mut function.params {
        walk_type(&mut param.ty, f);
    }
    if let Some(ret) = &mut function.ret {
        walk_type(ret, f);
    }
    let tail = match function.body.split_last_mut() {
        Some((Stmt::Expr(tail), rest)) => {
            walk_stmts(rest, ret.as_ref(), f);
            tail
        }
        _ => return walk_stmts(&mut function.body, ret.as_ref(), f),
    };
    walk_expr(tail, ret.as_ref(), ret.as_ref(), f);
}

fn is_literal(expr: &Expr) -> bool {
    matches!(
        expr,
        Expr::HexFloat(_)
            | Expr::CStr(_)
            | Expr::Value(
                RustValue::I64(_)
                    | RustValue::Usize(_)
                    | RustValue::I128(_)
                    | RustValue::U128(_)
                    | RustValue::TypedInt(..)
                    | RustValue::TypedUInt(..)
                    | RustValue::Float(_)
                    | RustValue::Bool(_)
            )
    )
}

fn is_float(prim: Prim) -> bool {
    matches!(prim, Prim::F16 | Prim::F32 | Prim::F64 | Prim::F128)
}

fn is_integer(prim: Prim) -> bool {
    prim != Prim::Bool && !is_float(prim)
}

fn const_type(literal: &Expr, expected: Option<&Type>) -> Option<Type> {
    match literal {
        Expr::Value(RustValue::TypedInt(_, prim) | RustValue::TypedUInt(_, prim)) => {
            Some(Type::Prim(*prim))
        }
        Expr::Value(RustValue::Usize(_)) => Some(Type::Prim(Prim::Usize)),
        Expr::Value(RustValue::Bool(_)) => Some(Type::Prim(Prim::Bool)),
        Expr::CStr(_) => Some(Type::Ref {
            mutable: false,
            inner: Box::new(Type::Custom("core::ffi::CStr".into())),
        }),
        Expr::Value(RustValue::I64(_) | RustValue::I128(_) | RustValue::U128(_)) => expected
            .filter(|ty| matches!(ty, Type::Prim(prim) if is_integer(*prim)))
            .cloned(),
        Expr::Value(RustValue::Float(_)) | Expr::HexFloat(_) => expected
            .filter(|ty| matches!(ty, Type::Prim(prim) if is_float(*prim)))
            .cloned(),
        _ => None,
    }
}

fn shell_and_leaves(function: &FnDef) -> (FnDef, Vec<Leaf>) {
    let mut shell = function.clone();
    let mut leaves = Vec::new();
    walk_fn(&mut shell, &mut |slot| match slot {
        Slot::Expr(expr, expected) if is_literal(expr) => {
            let literal = std::mem::replace(expr, Expr::Value(RustValue::None));
            leaves.push(Leaf {
                const_ty: const_type(&literal, expected),
                value: Value::Literal(literal),
            });
        }
        Slot::Type(ty) => {
            if let Type::Prim(prim) = ty {
                leaves.push(Leaf {
                    value: Value::Prim(*prim),
                    const_ty: None,
                });
                *ty = Type::Unit;
            }
        }
        Slot::Expr(..) => {}
    });
    (shell, leaves)
}

fn binding(name: &str, sites: &[usize], leaves: &[Leaf]) -> Option<Item> {
    let first = &leaves[sites[0]];
    match &first.value {
        Value::Prim(prim) => Some(Item::TypeAlias {
            name: format!("__SlateHoist_{name}"),
            ty: Type::Prim(*prim),
        }),
        Value::Literal(literal) => {
            let ty = first.const_ty.as_ref()?;
            let agree = sites
                .iter()
                .all(|&at| leaves[at].const_ty.as_ref() == Some(ty));
            agree.then(|| Item::Const {
                attrs: Vec::new(),
                name: format!("__SLATE_HOIST_{name}"),
                ty: ty.clone(),
                init: literal.clone(),
            })
        }
    }
}

fn binding_name(item: &Item) -> &str {
    match item {
        Item::Const { name, .. } | Item::TypeAlias { name, .. } => name,
        _ => unreachable!("hoists bind only consts and type aliases"),
    }
}

pub fn hoist(variants: &[&FnDef]) -> Option<Hoisted> {
    let (shells, leaves): (Vec<FnDef>, Vec<Vec<Leaf>>) = variants
        .iter()
        .map(|variant| shell_and_leaves(variant))
        .unzip();
    if shells.iter().any(|shell| *shell != shells[0]) {
        return None;
    }
    let column = |at: usize| -> Vec<&Value> { leaves.iter().map(|leaf| &leaf[at].value).collect() };
    let sites: Vec<usize> = (0..leaves[0].len())
        .filter(|&at| {
            column(at)
                .iter()
                .any(|value| **value != leaves[0][at].value)
        })
        .collect();
    let first_site = *sites.first()?;
    if sites.iter().any(|&at| column(at) != column(first_site)) {
        return None;
    }
    let name = &variants[0].name;
    let bindings = leaves
        .iter()
        .map(|leaves| binding(name, &sites, leaves))
        .collect::<Option<Vec<Item>>>()?;
    let bound = binding_name(&bindings[0]);
    let mut function = variants[0].clone();
    let mut position = 0;
    walk_fn(&mut function, &mut |slot| match slot {
        Slot::Expr(expr, _) if is_literal(expr) => {
            if sites.contains(&position) {
                *expr = Expr::Var(Ident::new(bound));
            }
            position += 1;
        }
        Slot::Type(ty) if matches!(ty, Type::Prim(_)) => {
            if sites.contains(&position) {
                *ty = Type::Custom(bound.to_string());
            }
            position += 1;
        }
        _ => {}
    });
    Some(Hoisted { function, bindings })
}

pub fn expand(function: &mut FnDef, bindings: &BTreeMap<&str, &Item>) {
    walk_fn(function, &mut |slot| match slot {
        Slot::Expr(expr, _) => {
            if let Expr::Var(name) = expr
                && let Some(Item::Const { init, .. }) = bindings.get(name.as_str())
            {
                *expr = init.clone();
            }
        }
        Slot::Type(ty) => {
            if let Type::Custom(name) = ty
                && let Some(Item::TypeAlias { ty: bound, .. }) = bindings.get(name.as_str())
            {
                *ty = bound.clone();
            }
        }
    });
}
