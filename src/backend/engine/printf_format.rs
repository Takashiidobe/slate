use std::collections::HashSet;

use crate::backend::rust_ast::{
    Block, CLIB_RECORD_TYPES, Expr, ExternDecl, ExternFnDecl, FnParam, Ident, ImplItem, Item, Path,
    Program, RustValue, Stmt, Type,
};
use crate::function_identity::{CallBinding, FunctionIdentity, Known};

struct FormatPlan {
    macro_name: &'static str,
    format: String,
}

fn literal_c_string(expr: &Expr) -> Option<Vec<u8>> {
    let mut current = expr;
    loop {
        match current {
            Expr::Cast { expr, .. } => current = expr,
            Expr::MethodCall { recv, method, args } if method == "as_ptr" && args.is_empty() => {
                return match recv.as_ref() {
                    Expr::CStr(bytes) => Some(bytes.clone()),
                    Expr::ByteStr(bytes) => {
                        let (last, rest) = bytes.split_last()?;
                        (*last == 0 && !rest.contains(&0)).then(|| rest.to_vec())
                    }
                    _ => None,
                };
            }
            _ => return None,
        }
    }
}

fn decimal_format_plan(args: &[Expr]) -> Option<FormatPlan> {
    let (format_arg, value_args) = args.split_first()?;
    let bytes = literal_c_string(format_arg)?;
    let text = std::str::from_utf8(&bytes).ok()?;
    let mut format = String::new();
    let mut placeholder_count = 0usize;
    let mut chars = text.chars();
    while let Some(c) = chars.next() {
        match c {
            '%' => match chars.next()? {
                'd' => {
                    format.push_str("{}");
                    placeholder_count += 1;
                }
                _ => return None,
            },
            '{' => format.push_str("{{"),
            '}' => format.push_str("}}"),
            other => format.push(other),
        }
    }
    if placeholder_count != value_args.len() {
        return None;
    }
    let (macro_name, format) = match format.strip_suffix('\n') {
        Some(stripped) => ("println", stripped.to_string()),
        None => ("print", format),
    };
    Some(FormatPlan { macro_name, format })
}

const STDOUT_WRITER_KNOWNS: &[Known] = &[
    Known::Printf,
    Known::FPrintf,
    Known::Puts,
    Known::FPuts,
    Known::FWrite,
];

const STDOUT_WRITER_EXTRA_NAMES: &[&str] = &["putchar", "putc", "vprintf", "vfprintf"];

fn stdout_writer_shim_names(program: &Program) -> HashSet<String> {
    program
        .items
        .iter()
        .flat_map(|item| match item {
            Item::ExternBlock { decls, .. } => decls.as_slice(),
            _ => &[],
        })
        .filter_map(|decl| match decl {
            ExternDecl::Fn(f)
                if matches!(f.identity, FunctionIdentity::Known(known) if STDOUT_WRITER_KNOWNS.contains(&known))
                    || STDOUT_WRITER_EXTRA_NAMES.contains(&f.name.as_str()) =>
            {
                Some(f.name.clone())
            }
            _ => None,
        })
        .collect()
}

fn is_stdout_writer_call(expr: &Expr, writer_names: &HashSet<String>) -> bool {
    match expr {
        Expr::Call { binding, .. } if matches!(binding.known(), Some(known) if STDOUT_WRITER_KNOWNS.contains(&known)) => {
            true
        }
        Expr::Call { func, .. } => {
            matches!(func.as_ref(), Expr::Var(name) if writer_names.contains(name.as_str()))
        }
        _ => false,
    }
}

fn printf_call_args(expr: &Expr) -> Option<&[Expr]> {
    match expr {
        Expr::Call { binding, args, .. } if binding.known() == Some(Known::Printf) => Some(args),
        _ => None,
    }
}

fn is_printf_stmt(stmt: &Stmt, printf_names: &HashSet<String>) -> bool {
    let Stmt::Expr(Expr::Unsafe(block)) = stmt else {
        return false;
    };
    block.stmts.is_empty()
        && block
            .tail
            .as_deref()
            .is_some_and(|tail| is_stdout_writer_call(tail, printf_names))
}

fn path_call(segments: &[&str], args: Vec<Expr>) -> Expr {
    Expr::Call {
        binding: CallBinding::Generated,
        func: Box::new(Expr::Path(Path::new(
            segments.iter().copied().map(Ident::from),
        ))),
        args,
    }
}

fn stdout_flush_stmt() -> Stmt {
    let stdout_ref = Expr::Ref {
        mutable: true,
        expr: Box::new(path_call(&["std", "io", "stdout"], Vec::new())),
    };
    Stmt::Let {
        name: "_".into(),
        mutable: false,
        ty: None,
        init: Some(path_call(
            &["std", "io", "Write", "flush"],
            vec![stdout_ref],
        )),
    }
}

fn file_ptr_type() -> Type {
    Type::Ptr {
        mutable: true,
        inner: Box::new(Type::CLib(CLIB_RECORD_TYPES[0])),
    }
}

fn fflush_all_stmt() -> Stmt {
    let call = Expr::Call {
        binding: CallBinding::Direct {
            identity: FunctionIdentity::Known(Known::FFlush),
            canonical_type: None,
            trusted_declaration: None,
            trusted_headers: Default::default(),
        },
        func: Box::new(Expr::Var("fflush".into())),
        args: vec![Expr::Value(RustValue::NullPtr)],
    };
    Stmt::Expr(Expr::Unsafe(Box::new(Block {
        stmts: Vec::new(),
        tail: Some(Box::new(call)),
    })))
}

fn try_convert(stmt: &Stmt) -> Option<Vec<Stmt>> {
    let Stmt::Expr(Expr::Unsafe(block)) = stmt else {
        return None;
    };
    if !block.stmts.is_empty() {
        return None;
    }
    let args = printf_call_args(block.tail.as_deref()?)?;
    let plan = decimal_format_plan(args)?;
    let mut macro_args = vec![Expr::Str(plan.format)];
    macro_args.extend(args[1..].iter().cloned());
    let print_stmt = Stmt::Expr(Expr::Macro {
        name: plan.macro_name.into(),
        args: macro_args,
    });
    Some(vec![print_stmt, stdout_flush_stmt()])
}

const STDOUT_TAIL_TEMP: &str = "__slate_stdout_writer_tail";

fn wrap_stdout_writer_preserving_tail(stmt: Stmt) -> Vec<Stmt> {
    let Stmt::Expr(inner_expr) = stmt else {
        unreachable!("caller only passes stmts matched by is_printf_stmt");
    };
    vec![
        stdout_flush_stmt(),
        Stmt::Let {
            name: STDOUT_TAIL_TEMP.into(),
            mutable: false,
            ty: None,
            init: Some(inner_expr),
        },
        fflush_all_stmt(),
        Stmt::Expr(Expr::Var(STDOUT_TAIL_TEMP.into())),
    ]
}

fn rewrite_block_stmts(
    stmts: &mut Vec<Stmt>,
    printf_names: &HashSet<String>,
    needs_fflush_extern: &mut bool,
    protect_tail: bool,
) {
    let original = std::mem::take(stmts);
    let last_index = original.len().checked_sub(1);
    let mut result = Vec::with_capacity(original.len());
    for (index, mut stmt) in original.into_iter().enumerate() {
        rewrite_stmt_nested(&mut stmt, printf_names, needs_fflush_extern);
        let is_tail = protect_tail && Some(index) == last_index;
        if !is_tail && let Some(replacement) = try_convert(&stmt) {
            result.extend(replacement);
        } else if is_printf_stmt(&stmt, printf_names) {
            *needs_fflush_extern = true;
            if is_tail {
                result.extend(wrap_stdout_writer_preserving_tail(stmt));
            } else {
                result.push(stdout_flush_stmt());
                result.push(stmt);
                result.push(fflush_all_stmt());
            }
        } else {
            result.push(stmt);
        }
    }
    *stmts = result;
}

fn rewrite_stmt_nested(
    stmt: &mut Stmt,
    printf_names: &HashSet<String>,
    needs_fflush_extern: &mut bool,
) {
    match stmt {
        Stmt::If {
            then_body,
            else_body,
            ..
        }
        | Stmt::LetIf {
            then_body,
            else_body,
            ..
        } => {
            rewrite_block_stmts(then_body, printf_names, needs_fflush_extern, false);
            rewrite_block_stmts(else_body, printf_names, needs_fflush_extern, false);
        }
        Stmt::Loop { body, .. }
        | Stmt::For { body, .. }
        | Stmt::Scope { body }
        | Stmt::LabeledBlock { body, .. } => {
            rewrite_block_stmts(body, printf_names, needs_fflush_extern, false)
        }
        Stmt::While { body, .. } | Stmt::Unsafe { body } => {
            rewrite_block_stmts(&mut body.stmts, printf_names, needs_fflush_extern, false)
        }
        Stmt::Block(block) => {
            rewrite_block_stmts(&mut block.stmts, printf_names, needs_fflush_extern, false)
        }
        Stmt::Match { arms, .. } => {
            for arm in arms {
                rewrite_block_stmts(&mut arm.body, printf_names, needs_fflush_extern, false);
            }
        }
        _ => {}
    }
}

fn rewrite_item(item: &mut Item, printf_names: &HashSet<String>, needs_fflush_extern: &mut bool) {
    match item {
        Item::Fn(func) => {
            let protect_tail = func.ret.as_ref().is_some_and(|ty| !ty.is_unit());
            rewrite_block_stmts(
                &mut func.body,
                printf_names,
                needs_fflush_extern,
                protect_tail,
            );
        }
        Item::InlineMod { items, .. } => {
            for item in items {
                rewrite_item(item, printf_names, needs_fflush_extern);
            }
        }
        Item::Impl(impl_block) => {
            for impl_item in &mut impl_block.items {
                if let ImplItem::Method(method) = impl_item
                    && let Expr::Block(block) = &mut method.body
                {
                    rewrite_block_stmts(&mut block.stmts, printf_names, needs_fflush_extern, false);
                }
            }
        }
        _ => {}
    }
}

fn declares_fflush(program: &Program) -> bool {
    program.items.iter().any(|item| match item {
        Item::ExternBlock { decls, .. } => decls
            .iter()
            .any(|decl| matches!(decl, ExternDecl::Fn(f) if f.name == "fflush")),
        _ => false,
    })
}

fn fflush_extern_decl() -> ExternFnDecl {
    ExternFnDecl {
        attrs: Vec::new(),
        identity: FunctionIdentity::Known(Known::FFlush),
        name: "fflush".into(),
        declared_type: None,
        trusted_headers: Default::default(),
        params: vec![FnParam {
            name: "_0".into(),
            mutable: false,
            ty: file_ptr_type(),
        }],
        variadic: false,
        ret: Some(Type::Prim(crate::backend::rust_ast::Prim::I32)),
        safe: false,
    }
}

fn insert_pos(items: &[Item]) -> usize {
    if let Some(pos) = items
        .iter()
        .rposition(|item| matches!(item, Item::ExternBlock { .. }))
    {
        return pos + 1;
    }
    items
        .iter()
        .position(|item| matches!(item, Item::Fn(_) | Item::Impl(_) | Item::Static { .. }))
        .unwrap_or(items.len())
}

pub(super) fn rewrite(program: &mut Program) {
    let printf_names = stdout_writer_shim_names(program);
    let mut needs_fflush_extern = false;
    for item in &mut program.items {
        rewrite_item(item, &printf_names, &mut needs_fflush_extern);
    }
    if needs_fflush_extern && !declares_fflush(program) {
        let pos = insert_pos(&program.items);
        program.items.insert(
            pos,
            Item::ExternBlock {
                abi: "C".into(),
                decls: vec![ExternDecl::Fn(fflush_extern_decl())],
            },
        );
    }
}
