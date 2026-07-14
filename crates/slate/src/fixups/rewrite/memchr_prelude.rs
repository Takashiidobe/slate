use crate::rust_ast::{
    BinOp, Block, CLibType, Expr, ExprMatchArm, FnDef, Ident, IndentStmt, Pattern, Prim, Stmt,
    Type, UnaryOp,
};

pub(in crate::fixups) fn fixup(f: &mut FnDef) -> bool {
    if f.name != "__slate_memchr" || f.params.len() != 3 {
        return false;
    }
    f.body = memchr_body();
    true
}

fn memchr_body() -> Vec<IndentStmt> {
    vec![
        indent(let_stmt(
            "b",
            Some(Type::Prim(Prim::U8)),
            cast(var("c"), Type::Prim(Prim::U8)),
        )),
        indent(let_stmt(
            "bytes",
            Some(ptr(false, Type::Prim(Prim::U8))),
            cast(var("s"), ptr(false, Type::Prim(Prim::U8))),
        )),
        indent(let_stmt(
            "haystack",
            None,
            unsafe_expr(call(
                path(["std", "slice", "from_raw_parts"]),
                vec![var("bytes"), var("n")],
            )),
        )),
        indent(Stmt::Return(Some(Expr::Match {
            expr: Box::new(position_expr()),
            arms: vec![
                ExprMatchArm {
                    pattern: Pattern::TupleStruct {
                        name: Ident::from("Some"),
                        fields: vec![Pattern::Binding(Ident::from("i"))],
                    },
                    value: unsafe_expr(cast(
                        method(var("bytes"), "add", vec![var("i")]),
                        void_ptr(true),
                    )),
                },
                ExprMatchArm {
                    pattern: Pattern::Binding(Ident::from("None")),
                    value: null_mut(),
                },
            ],
        }))),
    ]
}

fn position_expr() -> Expr {
    method(
        method(var("haystack"), "iter", vec![]),
        "position",
        vec![Expr::Closure {
            params: vec![Ident::from("x")],
            body: Box::new(Expr::Binary {
                op: BinOp::Eq,
                lhs: Box::new(Expr::Unary {
                    op: UnaryOp::Deref,
                    expr: Box::new(var("x")),
                }),
                rhs: Box::new(var("b")),
            }),
        }],
    )
}

fn indent(stmt: Stmt) -> IndentStmt {
    IndentStmt { depth: 1, stmt }
}

fn let_stmt(name: &str, ty: Option<Type>, init: Expr) -> Stmt {
    Stmt::Let {
        name: name.into(),
        mutable: false,
        ty,
        init: Some(init),
    }
}

fn var(name: &str) -> Expr {
    Expr::Var(Ident::from(name))
}

fn path<const N: usize>(parts: [&str; N]) -> Expr {
    Expr::Path(crate::rust_ast::Path::new(parts.map(Ident::from)))
}

fn call(func: Expr, args: Vec<Expr>) -> Expr {
    Expr::Call {
        func: Box::new(func),
        args,
    }
}

fn method(recv: Expr, method: &str, args: Vec<Expr>) -> Expr {
    Expr::MethodCall {
        recv: Box::new(recv),
        method: method.into(),
        args,
    }
}

fn cast(expr: Expr, ty: Type) -> Expr {
    Expr::Cast {
        expr: Box::new(expr),
        ty,
    }
}

fn unsafe_expr(value: Expr) -> Expr {
    Expr::Unsafe(Box::new(Block {
        stmts: Vec::new(),
        tail: Some(Box::new(value)),
    }))
}

fn ptr(mutable: bool, inner: Type) -> Type {
    Type::Ptr {
        mutable,
        inner: Box::new(inner),
    }
}

fn void_ptr(mutable: bool) -> Type {
    ptr(mutable, Type::CLib(CLibType::Void))
}

fn null_mut() -> Expr {
    call(path(["std", "ptr", "null_mut"]), Vec::new())
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::{emit, param};
    use crate::rust_ast::Visibility;

    #[test]
    fn rewrites_memchr_helper_body_to_position_match() {
        let mut f = FnDef {
            vis: Visibility::Private,
            unsafe_: false,
            extern_c: false,
            name: "__slate_memchr".into(),
            params: vec![
                param("s", "*const core::ffi::c_void"),
                param("c", "i32"),
                param("n", "usize"),
            ],
            ret: Some(void_ptr(true)),
            body: vec![indent(Stmt::Return(Some(null_mut())))],
        };

        assert!(fixup(&mut f));
        let out = emit(f);
        assert!(out.contains("let haystack = unsafe { std::slice::from_raw_parts(bytes, n) };"));
        assert!(out.contains("haystack.iter().position(|x| *x == b)"));
        assert!(out.contains("Some(i) => unsafe { bytes.add(i) as *mut core::ffi::c_void }"));
        assert!(out.contains("None => std::ptr::null_mut()"));
    }
}
