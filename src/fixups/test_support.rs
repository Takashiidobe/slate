//! Shared builders for fixup-pass unit tests.

use crate::rust_ast::{
    BinOp, Expr, FnDef, FnParam, IndentStmt, Item, Prim, Program, RustValue, Stmt, Type, Visibility,
};

pub(super) fn temp(name: &str, ty: &str, init: Expr) -> Stmt {
    Stmt::Let {
        name: name.to_string(),
        mutable: false,
        ty: Some(Type::parse(ty)),
        init: Some(init),
    }
}

pub(super) fn let_mut(name: &str, ty: &str, init: Expr) -> Stmt {
    Stmt::Let {
        name: name.to_string(),
        mutable: true,
        ty: Some(Type::parse(ty)),
        init: Some(init),
    }
}

pub(super) fn assign(target: &str, value: Expr) -> Stmt {
    Stmt::Assign {
        target: Expr::Var(target.into()),
        value,
    }
}

pub(super) fn var(name: &str) -> Expr {
    Expr::Var(name.into())
}

pub(super) fn int(n: i64) -> Expr {
    Expr::Value(RustValue::I64(n))
}

pub(super) fn call(func: &str, args: Vec<Expr>) -> Expr {
    let binding = crate::function_identity::Known::for_test_symbol(func).map_or(
        crate::function_identity::CallBinding::Generated,
        |known| crate::function_identity::CallBinding::Direct {
            identity: crate::function_identity::FunctionIdentity::Known(known),
            canonical_type: None,
        },
    );
    Expr::Call {
        binding,
        func: Box::new(Expr::Var(func.into())),
        args,
    }
}

pub(super) fn bin(op: BinOp, lhs: Expr, rhs: Expr) -> Expr {
    Expr::Binary {
        op,
        lhs: Box::new(lhs),
        rhs: Box::new(rhs),
    }
}

pub(super) fn param(name: &str, ty: &str) -> FnParam {
    FnParam {
        name: name.into(),
        mutable: false,
        ty: Type::parse(ty),
        nonnull: false,
    }
}

pub(super) fn func(params: Vec<FnParam>, ret: Option<&str>, stmts: Vec<Stmt>) -> FnDef {
    FnDef {
        attrs: Vec::new(),
        vis: Visibility::Private,
        unsafe_: false,
        abi: None,
        name: "f".into(),
        params,
        ret: ret.map(Type::parse),
        body: stmts
            .into_iter()
            .map(|stmt| IndentStmt { depth: 1, stmt })
            .collect(),
        returns_nonnull: false,
    }
}

pub(super) fn emit(f: FnDef) -> String {
    Program {
        items: vec![Item::Fn(f)],
    }
    .emit()
}

pub(super) fn after_body(
    pass: fn(&mut Vec<IndentStmt>),
    params: Vec<FnParam>,
    ret: Option<&str>,
    stmts: Vec<Stmt>,
) -> String {
    let mut f = func(params, ret, stmts);
    pass(&mut f.body);
    emit(f)
}

pub(super) fn after_fn(pass: fn(&mut FnDef), f: FnDef) -> String {
    let mut f = f;
    pass(&mut f);
    emit(f)
}

pub(super) fn migrated_fn(body: Vec<Stmt>) -> FnDef {
    FnDef {
        attrs: Vec::new(),
        vis: Visibility::Private,
        unsafe_: false,
        abi: None,
        name: "add".into(),
        params: vec![
            FnParam {
                name: "arg0".into(),
                mutable: false,
                ty: Type::Prim(Prim::I32),
                nonnull: false,
            },
            FnParam {
                name: "arg1".into(),
                mutable: false,
                ty: Type::Prim(Prim::I32),
                nonnull: false,
            },
        ],
        ret: Some(Type::Prim(Prim::I32)),
        body: body
            .into_iter()
            .map(|stmt| IndentStmt { depth: 1, stmt })
            .collect(),
        returns_nonnull: false,
    }
}
