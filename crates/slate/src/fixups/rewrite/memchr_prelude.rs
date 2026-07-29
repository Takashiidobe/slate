use crate::fixups::Fixup;
use crate::fixups::query::{
    ByteRepresentation, ByteSource, CallRecord, CallTarget, ExprRule, ExprSite, NulPosition,
    PointerMutability, Predicate, QueryContext, Rejection, RejectionReason, ReplaceExpr, RuleCase,
    RuleIdentity, RuleResult,
};
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLocation, TraceLogger, TraceSnippet, fact, stmts_snippet,
};
use crate::rust_ast::{
    BinOp, Block, CLibType, Expr, ExprMatchArm, Ident, IndentStmt, Item, Pattern, Prim, Program,
    RustValue, Stmt, Type, UnaryOp,
};

pub(in crate::fixups) struct MemchrPrelude<'a> {
    function_name: String,
    enabled: bool,
    logger: &'a mut dyn TraceLogger,
}

pub(in crate::fixups) struct MemchrPreludePruneUnusedHelper<'a> {
    logger: &'a mut dyn TraceLogger,
}

pub(in crate::fixups) struct MemchrCalls;

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

impl<'a> MemchrPreludePruneUnusedHelper<'a> {
    pub(in crate::fixups) fn new(logger: &'a mut dyn TraceLogger) -> Self {
        Self { logger }
    }

    pub(in crate::fixups) fn fixup(&mut self, program: &mut Program) -> bool {
        let before = self.logger.is_enabled().then(|| program.emit());
        let changed = prune_unused_helper_impl(program);
        if changed && let Some(before) = before {
            self.logger.rewrite(RewriteEvent {
                pass: TracePass::MemchrPreludePruneUnusedHelper,
                kind: "prune_unused_memchr_helper".into(),
                location: TraceLocation {
                    function: Some("__slate_memchr".into()),
                    ..TraceLocation::default()
                },
                before: vec![TraceSnippet::new("program", before.trim_end())],
                after: vec![TraceSnippet::new("program", program.emit().trim_end())],
                facts: vec![fact("helper", "__slate_memchr")],
            });
        }
        changed
    }
}

fn prune_unused_helper_impl(program: &mut Program) -> bool {
    if has_memchr_call(program) {
        return false;
    }
    let before = program.items.len();
    program.items.retain(|item| match item {
        Item::Fn(f) => f.name != "__slate_memchr",
        _ => true,
    });
    program.items.len() != before
}

impl ExprRule for MemchrCalls {
    type Candidate = CallRecord;

    fn identity(&self) -> RuleIdentity {
        RuleIdentity::new(TracePass::MemchrPreludeFixupCalls, "rewrite_memchr_call")
    }

    fn candidates(&self, query: &QueryContext<'_>) -> Vec<Self::Candidate> {
        query
            .calls(&CallTarget::Generated("__slate_memchr".into()), 3)
            .to_vec()
    }

    fn target(&self, candidate: &Self::Candidate) -> ExprSite {
        candidate.site.clone()
    }

    fn cases(&self, query: &QueryContext<'_>, candidate: &Self::Candidate) -> Vec<RuleCase> {
        vec![
            RuleCase::new("known_nul", RuleResult::from(nul_case(query, candidate))),
            RuleCase::new(
                "byte_position",
                RuleResult::from(position_case(query, candidate)),
            ),
        ]
    }
}

fn nul_case(query: &QueryContext<'_>, call: &CallRecord) -> Result<ReplaceExpr, Rejection> {
    let (source, source_evidence) = query.byte_source(&call.args[0])?.into_parts();
    let (needle, needle_evidence) = query.const_u8(&call.args[1])?.into_parts();
    if needle != 0 {
        return Err(Rejection::new(
            Predicate::ConstantU8,
            Some(call.args[1].clone()),
            RejectionReason::Contradicted,
            needle_evidence,
        ));
    }
    let needle_stable = query.pure(&call.args[1])?;
    let (nul, nul_evidence) = query.first_nul(&source)?.into_parts();
    let in_range = query.prefix_contains(&call.args[2], nul)?;
    let replacement = pointer_search(source.clone(), some(nul_index_expr(source, nul)));
    Ok(
        ReplaceExpr::new(call.site.clone(), replacement).with_evidence(
            call.evidence
                .iter()
                .cloned()
                .chain(source_evidence)
                .chain(needle_evidence)
                .chain(needle_stable.evidence)
                .chain(nul_evidence)
                .chain(in_range.evidence),
        ),
    )
}

fn position_case(query: &QueryContext<'_>, call: &CallRecord) -> Result<ReplaceExpr, Rejection> {
    let (source, source_evidence) = query.byte_source(&call.args[0])?.into_parts();
    let full_view = query.full_byte_view(&source, &call.args[2])?;
    let needle_stable = query.pure(&call.args[1])?;
    let needle = query
        .expr(&call.args[1])
        .expect("indexed memchr argument must resolve")
        .clone();
    let replacement = pointer_search(
        source.clone(),
        byte_position(source_iter(source), byte_expr(needle)),
    );
    Ok(
        ReplaceExpr::new(call.site.clone(), replacement).with_evidence(
            call.evidence
                .iter()
                .cloned()
                .chain(source_evidence)
                .chain(full_view.evidence)
                .chain(needle_stable.evidence),
        ),
    )
}

fn pointer_search(source: ByteSource<'_>, index: Expr) -> Expr {
    let source_for_ptr = source.clone();
    method(
        index,
        "map_or",
        vec![null_mut(), index_to_ptr(source_for_ptr)],
    )
}

fn source_iter(source: ByteSource<'_>) -> Expr {
    method(byte_source_expr(source), "iter", Vec::new())
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

fn nul_index_expr(source: ByteSource<'_>, nul_index: NulPosition) -> Expr {
    match nul_index {
        NulPosition::ByteLength => source_len(source),
        NulPosition::Constant(n) => Expr::Value(RustValue::I64(n as i64)),
    }
}

fn byte_position(source: Expr, needle: Expr) -> Expr {
    method(
        source,
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
            method(
                source_ptr(source.clone()),
                "add",
                vec![var("__slate_index")],
            ),
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

fn has_memchr_call(program: &Program) -> bool {
    program.items.iter().any(|item| {
        let Item::Fn(f) = item else {
            return false;
        };
        walk::body_expr_any(&f.body, &mut |expr| {
            matches!(
                expr,
                Expr::Call { func, .. }
                    if matches!(&**func, Expr::Var(name) if name.as_str() == "__slate_memchr")
            )
        })
    })
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
