use crate::fixups::facts::{
    AstPath, ConstValue, FixupFacts, FunctionId, NulTermination, PathSegment, StringBufferKind,
    ValueSubject,
};
use crate::fixups::support::walk;
use crate::rust_ast::{
    BinOp, Block, CLibType, Expr, ExprMatchArm, FnDef, Ident, IndentStmt, Item, Pattern, Prim,
    Program, RustValue, Stmt, Type, UnaryOp,
};

pub(in crate::fixups) fn fixup(f: &mut FnDef) -> bool {
    if f.name != "__slate_memchr" || f.params.len() != 3 {
        return false;
    }
    f.body = memchr_body();
    true
}

pub(in crate::fixups) fn fixup_calls(program: &mut Program, facts: &FixupFacts) -> bool {
    let mut changed = false;
    for (item_index, item) in program.items.iter_mut().enumerate() {
        if let Item::Fn(f) = item
            && let Some(function) = facts.function_by_item_index(item_index)
        {
            changed |= fixup_body_calls(&mut f.body, function, facts);
        }
    }
    changed
}

pub(in crate::fixups) fn prune_unused_helper(program: &mut Program) -> bool {
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

fn fixup_body_calls(body: &mut [IndentStmt], function: FunctionId, facts: &FixupFacts) -> bool {
    let mut changed = false;
    walk::body_exprs_mut_with_path(body, &mut Vec::new(), &mut |expr, path| {
        if let Some(replacement) = memchr_call_replacement(expr, function, facts, path) {
            *expr = replacement;
            changed = true;
            return false;
        }
        true
    });
    changed
}

#[derive(Clone)]
struct Source {
    name: String,
    kind: SourceKind,
    mutable: bool,
    nul_index: Option<NulIndex>,
}

#[derive(Clone, Copy, PartialEq, Eq)]
enum SourceKind {
    U8Collection,
    Bytes,
    CStr,
    Str,
}

#[derive(Clone, Copy)]
enum NulIndex {
    SourceLen,
    Const(usize),
}

fn memchr_call_replacement(
    expr: &Expr,
    function: FunctionId,
    facts: &FixupFacts,
    path: &[PathSegment],
) -> Option<Expr> {
    let Expr::Call { func, args } = expr else {
        return None;
    };
    let Expr::Var(name) = &**func else {
        return None;
    };
    if name.as_str() != "__slate_memchr" || args.len() != 3 {
        return None;
    }
    let source = source_for_arg(&args[0], function, facts)?;
    let needle = byte_value(&args[1], function, facts, &call_arg_path(path, 1));
    if needle == Some(0)
        && let Some(nul_index) = source.nul_index
    {
        return Some(pointer_search(
            source.clone(),
            some(nul_index_expr(source, nul_index)),
        ));
    }
    if !full_source_len(&source, &args[2], function, facts, &call_arg_path(path, 2)) {
        return None;
    }
    Some(pointer_search(
        source.clone(),
        byte_position(source_iter(source), byte_expr(args[1].clone())),
    ))
}

fn source_for_arg(expr: &Expr, function: FunctionId, facts: &FixupFacts) -> Option<Source> {
    let (name, mutable) = pointer_source(expr)?;
    let binding = facts
        .bindings
        .iter()
        .rev()
        .find(|binding| binding.function == function && binding.name == name.as_str())?
        .id;
    if let Some(buffer) = facts.string_buffer(binding) {
        let well_formed_string = buffer.ascii_only
            && matches!(
                buffer.nul_termination,
                NulTermination::Terminated | NulTermination::AllZero
            )
            && !buffer.interior_nul;
        let kind = match buffer.kind {
            StringBufferKind::BorrowedStr | StringBufferKind::OwnedString => SourceKind::Str,
            StringBufferKind::BorrowedCStr => SourceKind::CStr,
            StringBufferKind::BorrowedBytes => SourceKind::Bytes,
            StringBufferKind::CharArray => source_kind_for_type(facts.binding_type(binding)?)?,
        };
        let nul_index = well_formed_string.then(|| match buffer.kind {
            StringBufferKind::CharArray => {
                NulIndex::Const(buffer.bytes.as_ref().map_or(0, Vec::len))
            }
            StringBufferKind::BorrowedStr
            | StringBufferKind::BorrowedCStr
            | StringBufferKind::BorrowedBytes
            | StringBufferKind::OwnedString => NulIndex::SourceLen,
        });
        return Some(Source {
            name,
            kind,
            mutable,
            nul_index,
        });
    }
    Some(Source {
        name,
        kind: source_kind_for_type(facts.binding_type(binding)?)?,
        mutable,
        nul_index: None,
    })
}

fn pointer_source(expr: &Expr) -> Option<(String, bool)> {
    match expr {
        Expr::Cast { expr, .. } => pointer_source(expr),
        Expr::ArrayPtr { array, mutable } => {
            let Expr::Var(name) = &**array else {
                return None;
            };
            Some((name.to_string(), *mutable))
        }
        Expr::MethodCall { recv, method, args }
            if args.is_empty() && matches!(method.as_str(), "as_ptr" | "as_mut_ptr") =>
        {
            let Expr::Var(name) = &**recv else {
                return None;
            };
            Some((name.to_string(), method == "as_mut_ptr"))
        }
        _ => None,
    }
}

fn source_kind_for_type(rendered: &str) -> Option<SourceKind> {
    let ty = Type::parse(rendered);
    let elem = match &ty {
        Type::Array { elem, .. } | Type::Slice(elem) => elem.as_ref(),
        Type::Ref { inner, .. } => match inner.as_ref() {
            Type::Slice(elem) => elem.as_ref(),
            Type::Str => return Some(SourceKind::Str),
            _ => return None,
        },
        Type::Custom(name) if name == "String" => return Some(SourceKind::Str),
        _ => return None,
    };
    matches!(elem, Type::Prim(Prim::U8 | Prim::I8)).then_some(SourceKind::U8Collection)
}

fn full_source_len(
    source: &Source,
    len: &Expr,
    function: FunctionId,
    facts: &FixupFacts,
    path: &[PathSegment],
) -> bool {
    len_matches_source_len(source, len)
        || source_array_len(source, function, facts)
            .is_some_and(|array_len| usize_value(len, function, facts, path) == Some(array_len))
}

fn len_matches_source_len(source: &Source, len: &Expr) -> bool {
    match len {
        Expr::Cast { expr, .. } => len_matches_source_len(source, expr),
        Expr::MethodCall { recv, method, args } if args.is_empty() && method == "len" => {
            matches_source_expr(source, recv)
        }
        _ => false,
    }
}

fn matches_source_expr(source: &Source, expr: &Expr) -> bool {
    match expr {
        Expr::Var(name) => name.as_str() == source.name.as_str(),
        Expr::MethodCall { recv, method, args }
            if args.is_empty() && method == "as_bytes" && source.kind == SourceKind::Str =>
        {
            matches_source_expr(source, recv)
        }
        _ => false,
    }
}

fn source_array_len(source: &Source, function: FunctionId, facts: &FixupFacts) -> Option<usize> {
    let binding = facts
        .bindings
        .iter()
        .rev()
        .find(|binding| binding.function == function && binding.name == source.name.as_str())?
        .id;
    match Type::parse(facts.binding_type(binding)?) {
        Type::Array { len, .. } => usize::try_from(len).ok(),
        _ => None,
    }
}

fn usize_value(
    expr: &Expr,
    function: FunctionId,
    facts: &FixupFacts,
    path: &[PathSegment],
) -> Option<usize> {
    match expr {
        Expr::Value(RustValue::I64(n)) => usize::try_from(*n).ok(),
        Expr::Value(RustValue::I128(n)) => usize::try_from(*n).ok(),
        Expr::Cast { expr, .. } => usize_value(expr, function, facts, path),
        Expr::Var(name) => binding_values(name, function, facts).find_map(const_usize),
        _ => facts
            .values_at(function, ValueSubject::Expr, &AstPath(path.to_vec()))
            .find_map(const_usize),
    }
}

fn byte_value(
    expr: &Expr,
    function: FunctionId,
    facts: &FixupFacts,
    path: &[PathSegment],
) -> Option<u8> {
    match expr {
        Expr::Value(RustValue::I64(n)) => u8::try_from(*n).ok(),
        Expr::Value(RustValue::I128(n)) => u8::try_from(*n).ok(),
        Expr::Cast { expr, .. } => byte_value(expr, function, facts, path),
        Expr::Var(name) => binding_values(name, function, facts).find_map(const_u8),
        _ => facts
            .values_at(function, ValueSubject::Expr, &AstPath(path.to_vec()))
            .find_map(const_u8),
    }
}

fn binding_values<'a>(
    name: &Ident,
    function: FunctionId,
    facts: &'a FixupFacts,
) -> impl Iterator<Item = &'a ConstValue> {
    facts
        .bindings
        .iter()
        .filter(move |binding| binding.function == function && binding.name == name.as_str())
        .flat_map(move |binding| {
            facts.values.iter().filter_map(move |value| {
                (value.function == function && value.subject == ValueSubject::Binding(binding.id))
                    .then_some(&value.value)
            })
        })
}

fn const_usize(value: &ConstValue) -> Option<usize> {
    match value {
        ConstValue::Integer(n) => usize::try_from(*n).ok(),
        ConstValue::Usize(n) | ConstValue::ArrayLength(n) => Some(*n),
        ConstValue::Zero => Some(0),
        ConstValue::Bool(_)
        | ConstValue::Bytes(_)
        | ConstValue::CStringBytes(_)
        | ConstValue::String(_) => None,
    }
}

fn const_u8(value: &ConstValue) -> Option<u8> {
    match value {
        ConstValue::Integer(n) => u8::try_from(*n).ok(),
        ConstValue::Usize(n) | ConstValue::ArrayLength(n) => u8::try_from(*n).ok(),
        ConstValue::Zero => Some(0),
        ConstValue::Bool(_)
        | ConstValue::Bytes(_)
        | ConstValue::CStringBytes(_)
        | ConstValue::String(_) => None,
    }
}

fn call_arg_path(path: &[PathSegment], arg_index: usize) -> Vec<PathSegment> {
    let mut out = path.to_vec();
    out.push(PathSegment::Expr(arg_index + 1));
    out
}

fn pointer_search(source: Source, index: Expr) -> Expr {
    let source_for_ptr = source.clone();
    method(
        index,
        "map_or",
        vec![null_mut(), index_to_ptr(source_for_ptr)],
    )
}

fn source_iter(source: Source) -> Expr {
    method(byte_source_expr(source), "iter", Vec::new())
}

fn byte_source_expr(source: Source) -> Expr {
    match source.kind {
        SourceKind::U8Collection => method(var(&source.name), "as_slice", Vec::new()),
        SourceKind::Bytes => var(&source.name),
        SourceKind::CStr => method(var(&source.name), "to_bytes", Vec::new()),
        SourceKind::Str => method(var(&source.name), "as_bytes", Vec::new()),
    }
}

fn source_len(source: Source) -> Expr {
    method(byte_source_expr(source), "len", Vec::new())
}

fn nul_index_expr(source: Source, nul_index: NulIndex) -> Expr {
    match nul_index {
        NulIndex::SourceLen => source_len(source),
        NulIndex::Const(n) => Expr::Value(RustValue::I64(n as i64)),
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

fn index_to_ptr(source: Source) -> Expr {
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

fn source_ptr(source: Source) -> Expr {
    let method_name = if source.mutable {
        "as_mut_ptr"
    } else {
        "as_ptr"
    };
    match source.kind {
        SourceKind::U8Collection | SourceKind::Bytes => {
            method(var(&source.name), method_name, Vec::new())
        }
        SourceKind::CStr | SourceKind::Str => method(var(&source.name), "as_ptr", Vec::new()),
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
            attrs: Vec::new(),
            vis: Visibility::Private,
            unsafe_: false,
            abi: None,
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
