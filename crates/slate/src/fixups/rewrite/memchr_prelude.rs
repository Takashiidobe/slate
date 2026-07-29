use crate::fixups::Fixup;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLocation, TraceLogger, stmts_snippet,
};
use crate::rust_ast::{
    BinOp, Block, CLibType, Expr, ExprMatchArm, Ident, IndentStmt, Pattern, Prim, Stmt, Type,
    UnaryOp,
};

pub(in crate::fixups) struct MemchrPrelude<'a> {
    function_name: String,
    enabled: bool,
    logger: &'a mut dyn TraceLogger,
}

impl<'a> MemchrPrelude<'a> {
    pub(in crate::fixups) fn new(
        function_name: String,
        enabled: bool,
        logger: &'a mut dyn TraceLogger,
    ) -> Self {
        Self {
            function_name,
            enabled,
            logger,
        }
    }
}

impl Fixup for MemchrPrelude<'_> {
    fn fixup(&mut self, body: &mut Vec<IndentStmt>) -> bool {
        if !self.enabled {
            return false;
        }
        let before = self.logger.is_enabled().then(|| body.clone());
        *body = memchr_body();
        let changed = true;
        if changed && let Some(before) = before {
            self.logger.rewrite(RewriteEvent {
                pass: TracePass::MemchrPrelude,
                kind: "rewrite_memchr_helper_body".into(),
                location: TraceLocation {
                    function: Some(self.function_name.clone()),
                    ..TraceLocation::default()
                },
                before: vec![stmts_snippet("body", &before)],
                after: vec![stmts_snippet("body", body)],
                facts: Vec::new(),
            });
        }
        changed
    }
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
        binding: crate::function_identity::CallBinding::Generated,
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
