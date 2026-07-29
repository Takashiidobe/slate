use crate::fixups::query::{
    ByteRepresentation, ByteSource, NulPosition, PointerMutability, Predicate, QueryContext,
    Rejection, RejectionReason, StableExpr,
};
use crate::rust_ast::{
    BinOp, Block, CLibType, Expr, ExprMatchArm, Ident, IndentStmt, Pattern, Prim, RustValue, Stmt,
    Type, UnaryOp,
};

pub(in crate::fixups) struct ExprRecipe<'snapshot> {
    source: ByteSource<'snapshot>,
    index: SearchIndex,
}

pub(in crate::fixups) enum SearchIndex {
    Known(NulPosition),
    Position(StableExpr),
}

pub(in crate::fixups) struct FunctionBodyRecipe {
    body: Vec<IndentStmt>,
}

pub(in crate::fixups) fn pointer_at_or_null(
    source: ByteSource<'_>,
    index: SearchIndex,
) -> ExprRecipe<'_> {
    ExprRecipe { source, index }
}

pub(in crate::fixups) fn known_index(position: NulPosition) -> SearchIndex {
    SearchIndex::Known(position)
}

pub(in crate::fixups) fn byte_position(needle: StableExpr) -> SearchIndex {
    SearchIndex::Position(needle)
}

pub(in crate::fixups) fn memchr_fallback_body() -> FunctionBodyRecipe {
    FunctionBodyRecipe {
        body: vec![
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
                expr: Box::new(helper_position()),
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
        ],
    }
}

impl FunctionBodyRecipe {
    pub(super) fn lower(self) -> Vec<IndentStmt> {
        self.body
    }
}

impl ExprRecipe<'_> {
    pub(super) fn lower(self, query: &QueryContext<'_>) -> Result<Expr, Rejection> {
        let index = match self.index {
            SearchIndex::Known(position) => some(nul_index_expr(self.source.clone(), position)),
            SearchIndex::Position(needle) => {
                let value = query.expr(&needle.site).cloned().ok_or_else(|| {
                    Rejection::new(
                        Predicate::MovablePure,
                        Some(needle.site),
                        RejectionReason::MissingEvidence,
                        Vec::new(),
                    )
                })?;
                position(byte_source_expr(self.source.clone()), byte_expr(value))
            }
        };
        Ok(pointer_search(self.source, index))
    }
}

fn pointer_search(source: ByteSource<'_>, index: Expr) -> Expr {
    method(index, "map_or", vec![null_mut(), index_to_ptr(source)])
}

fn byte_source_expr(source: ByteSource<'_>) -> Expr {
    match source.representation {
        ByteRepresentation::Collection => method(var(&source.name), "as_slice", Vec::new()),
        ByteRepresentation::Bytes => var(&source.name),
        ByteRepresentation::CStr => method(var(&source.name), "to_bytes", Vec::new()),
        ByteRepresentation::Str => method(var(&source.name), "as_bytes", Vec::new()),
    }
}

fn source_len(source: ByteSource<'_>) -> Expr {
    method(byte_source_expr(source), "len", Vec::new())
}

fn nul_index_expr(source: ByteSource<'_>, position: NulPosition) -> Expr {
    match position {
        NulPosition::ByteLength => source_len(source),
        NulPosition::Constant(position) => Expr::Value(RustValue::I64(position as i64)),
    }
}

fn position(source: Expr, needle: Expr) -> Expr {
    method(
        method(source, "iter", Vec::new()),
        "position",
        vec![Expr::Closure {
            params: vec![Ident::from("__slate_byte")],
            body: Box::new(Expr::Binary {
                op: BinOp::Eq,
                lhs: Box::new(byte_expr(Expr::Unary {
                    op: UnaryOp::Deref,
                    expr: Box::new(var("__slate_byte")),
                })),
                rhs: Box::new(needle),
            }),
        }],
    )
}

fn byte_expr(expr: Expr) -> Expr {
    cast(expr, Type::Prim(Prim::U8))
}

fn index_to_ptr(source: ByteSource<'_>) -> Expr {
    Expr::Closure {
        params: vec![Ident::from("__slate_index")],
        body: Box::new(unsafe_expr(cast(
            method(source_ptr(source), "add", vec![var("__slate_index")]),
            void_ptr(true),
        ))),
    }
}

fn source_ptr(source: ByteSource<'_>) -> Expr {
    let method_name = if source.mutability == PointerMutability::Mut {
        "as_mut_ptr"
    } else {
        "as_ptr"
    };
    match source.representation {
        ByteRepresentation::Collection | ByteRepresentation::Bytes => {
            method(var(&source.name), method_name, Vec::new())
        }
        ByteRepresentation::CStr | ByteRepresentation::Str => {
            method(var(&source.name), "as_ptr", Vec::new())
        }
    }
}

fn some(expr: Expr) -> Expr {
    call(var("Some"), vec![expr])
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

fn helper_position() -> Expr {
    method(
        method(var("haystack"), "iter", Vec::new()),
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
