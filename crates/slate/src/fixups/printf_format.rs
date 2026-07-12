use crate::rust_ast::{Block, Expr, ExternDecl, IndentStmt, Item, Program, Stmt};

pub(super) fn fixup(program: &mut Program) {
    if has_unsupported_printf(program) {
        return;
    }
    for item in &mut program.items {
        if let Item::Fn(f) = item {
            fixup_body(&mut f.body);
        }
    }
    if !program_has_printf_call(program) {
        prune_printf_extern(program);
    }
}

fn has_unsupported_printf(program: &Program) -> bool {
    program.items.iter().any(|item| match item {
        Item::Fn(f) => body_has_unsupported_printf(&f.body),
        _ => false,
    })
}

fn body_has_unsupported_printf(body: &[IndentStmt]) -> bool {
    body.iter()
        .any(|indent| stmt_has_unsupported_printf(&indent.stmt))
}

fn block_has_unsupported_printf(block: &Block) -> bool {
    body_has_unsupported_printf(&block.stmts)
        || block
            .tail
            .as_deref()
            .is_some_and(expr_has_unsupported_printf)
}

fn stmt_has_unsupported_printf(stmt: &Stmt) -> bool {
    match stmt {
        Stmt::Expr(expr) if rewrite_printf_expr(expr).is_some() => false,
        Stmt::Let { init, .. } => init.as_ref().is_some_and(expr_has_unsupported_printf),
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            expr_has_unsupported_printf(cond)
                || body_has_unsupported_printf(then_body)
                || expr_has_unsupported_printf(then_value)
                || body_has_unsupported_printf(else_body)
                || expr_has_unsupported_printf(else_value)
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            expr_has_unsupported_printf(target) || expr_has_unsupported_printf(value)
        }
        Stmt::Expr(expr) => expr_has_unsupported_printf(expr),
        Stmt::Return(expr) => expr.as_ref().is_some_and(expr_has_unsupported_printf),
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
            block_has_unsupported_printf(body)
        }
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            expr_has_unsupported_printf(cond)
                || body_has_unsupported_printf(then_body)
                || body_has_unsupported_printf(else_body)
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            body_has_unsupported_printf(body)
        }
        Stmt::Match { expr, arms } => {
            expr_has_unsupported_printf(expr)
                || arms
                    .iter()
                    .any(|arm| body_has_unsupported_printf(&arm.body))
        }
        Stmt::Break(_) | Stmt::Continue(_) => false,
    }
}

fn expr_has_unsupported_printf(expr: &Expr) -> bool {
    expr_has_printf_call(expr)
}

fn fixup_body(body: &mut [IndentStmt]) {
    for indent in body {
        fixup_stmt(&mut indent.stmt);
    }
}

fn fixup_stmt(stmt: &mut Stmt) {
    match stmt {
        Stmt::Expr(expr) => {
            if let Some(replacement) = rewrite_printf_expr(expr) {
                *expr = replacement;
            }
        }
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
            fixup_body(then_body);
            fixup_body(else_body);
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            fixup_body(body);
        }
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
            fixup_block(body);
        }
        Stmt::Match { arms, .. } => {
            for arm in arms {
                fixup_body(&mut arm.body);
            }
        }
        _ => {}
    }
}

fn fixup_block(block: &mut Block) {
    fixup_body(&mut block.stmts);
    if let Some(tail) = &mut block.tail {
        if let Some(replacement) = rewrite_printf_expr(tail) {
            *tail = Box::new(replacement);
        }
    }
}

fn rewrite_printf_expr(expr: &Expr) -> Option<Expr> {
    let call = peel_empty_unsafe(expr);
    let Expr::Call { func, args } = call else {
        return None;
    };
    if !matches!(&**func, Expr::Var(name) if name.as_str() == "printf") {
        return None;
    }
    let (fmt, rest) = args.split_first()?;
    let format = const_c_string(fmt)?;
    let macro_call = printf_macro(&format, rest)?;
    Some(macro_call)
}

fn peel_empty_unsafe(expr: &Expr) -> &Expr {
    if let Expr::Unsafe(block) = expr {
        if block.stmts.is_empty() {
            if let Some(tail) = &block.tail {
                return tail;
            }
        }
    }
    expr
}

fn const_c_string(expr: &Expr) -> Option<Vec<u8>> {
    match expr {
        Expr::ByteStr(bytes) => Some(trim_c_nul(bytes)),
        Expr::Cast { expr, .. } => const_c_string(expr),
        Expr::MethodCall { recv, method, args } if method == "as_ptr" && args.is_empty() => {
            const_c_string(recv)
        }
        _ => None,
    }
}

fn trim_c_nul(bytes: &[u8]) -> Vec<u8> {
    bytes.strip_suffix(&[0]).unwrap_or(bytes).to_vec()
}

fn printf_macro(format: &[u8], args: &[Expr]) -> Option<Expr> {
    let parsed = parse_printf_format(format)?;
    if parsed.arg_count != args.len() {
        return None;
    }
    let mut macro_args = Vec::new();
    let name = if parsed.trailing_newline {
        if parsed.format.is_empty() && args.is_empty() {
            return Some(format_macro("println", vec![]));
        }
        "println"
    } else {
        "print"
    };
    macro_args.push(Expr::Str(parsed.format));
    macro_args.extend(args.iter().cloned());
    Some(format_macro(name, macro_args))
}

struct ParsedFormat {
    format: String,
    arg_count: usize,
    trailing_newline: bool,
}

fn parse_printf_format(bytes: &[u8]) -> Option<ParsedFormat> {
    let mut format = String::new();
    let mut arg_count = 0;
    let mut i = 0;
    while i < bytes.len() {
        match bytes[i] {
            b'%' => match bytes.get(i + 1).copied() {
                Some(b'd') => {
                    format.push_str("{}");
                    arg_count += 1;
                    i += 2;
                }
                Some(b'%') => {
                    format.push('%');
                    i += 2;
                }
                _ => return None,
            },
            b'{' => {
                format.push_str("{{");
                i += 1;
            }
            b'}' => {
                format.push_str("}}");
                i += 1;
            }
            b'\n' => {
                format.push('\n');
                i += 1;
            }
            0x20..=0x7e => {
                format.push(bytes[i] as char);
                i += 1;
            }
            _ => return None,
        }
    }
    let trailing_newline = format.ends_with('\n');
    if trailing_newline {
        format.pop();
    }
    Some(ParsedFormat {
        format,
        arg_count,
        trailing_newline,
    })
}

fn format_macro(name: &str, args: Vec<Expr>) -> Expr {
    Expr::Macro {
        name: name.into(),
        args,
    }
}

fn prune_printf_extern(program: &mut Program) {
    program.items.retain_mut(|item| match item {
        Item::ExternBlock { decls, .. } => {
            decls.retain(|decl| !matches!(decl, ExternDecl::Fn(f) if f.name == "printf"));
            !decls.is_empty()
        }
        _ => true,
    });
}

fn program_has_printf_call(program: &Program) -> bool {
    program.items.iter().any(|item| match item {
        Item::Fn(f) => body_has_printf_call(&f.body),
        _ => false,
    })
}

fn body_has_printf_call(body: &[IndentStmt]) -> bool {
    body.iter().any(|indent| stmt_has_printf_call(&indent.stmt))
}

fn block_has_printf_call(block: &Block) -> bool {
    body_has_printf_call(&block.stmts) || block.tail.as_deref().is_some_and(expr_has_printf_call)
}

fn stmt_has_printf_call(stmt: &Stmt) -> bool {
    match stmt {
        Stmt::Let { init, .. } => init.as_ref().is_some_and(expr_has_printf_call),
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            expr_has_printf_call(cond)
                || body_has_printf_call(then_body)
                || expr_has_printf_call(then_value)
                || body_has_printf_call(else_body)
                || expr_has_printf_call(else_value)
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            expr_has_printf_call(target) || expr_has_printf_call(value)
        }
        Stmt::Expr(expr) => expr_has_printf_call(expr),
        Stmt::Return(expr) => expr.as_ref().is_some_and(expr_has_printf_call),
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
            block_has_printf_call(body)
        }
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            expr_has_printf_call(cond)
                || body_has_printf_call(then_body)
                || body_has_printf_call(else_body)
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            body_has_printf_call(body)
        }
        Stmt::Match { expr, arms } => {
            expr_has_printf_call(expr) || arms.iter().any(|arm| body_has_printf_call(&arm.body))
        }
        Stmt::Break(_) | Stmt::Continue(_) => false,
    }
}

fn expr_has_printf_call(expr: &Expr) -> bool {
    match expr {
        Expr::Call { func, args } => {
            matches!(&**func, Expr::Var(name) if name.as_str() == "printf")
                || expr_has_printf_call(func)
                || args.iter().any(expr_has_printf_call)
        }
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. } => expr_has_printf_call(expr),
        Expr::Binary { lhs, rhs, .. } => expr_has_printf_call(lhs) || expr_has_printf_call(rhs),
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            expr_has_printf_call(recv) || args.iter().any(expr_has_printf_call)
        }
        Expr::Field { base, .. } | Expr::TupleField { base, .. } => expr_has_printf_call(base),
        Expr::ArrayPtr { array, .. } => expr_has_printf_call(array),
        Expr::Index { base, index } => expr_has_printf_call(base) || expr_has_printf_call(index),
        Expr::StructLit { fields, .. } => {
            fields.iter().any(|(_, value)| expr_has_printf_call(value))
        }
        Expr::ArrayLit(elems) => elems.iter().any(expr_has_printf_call),
        Expr::ArrayRepeat { elem, .. } => expr_has_printf_call(elem),
        Expr::Macro { args, .. } => args.iter().any(expr_has_printf_call),
        Expr::Closure { body, .. } => expr_has_printf_call(body),
        Expr::Match { expr, arms } => {
            expr_has_printf_call(expr) || arms.iter().any(|arm| expr_has_printf_call(&arm.value))
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            expr_has_printf_call(cond)
                || expr_has_printf_call(then_expr)
                || expr_has_printf_call(else_expr)
        }
        Expr::Block(block) | Expr::Unsafe(block) => block_has_printf_call(block),
        Expr::CopyNonoverlapping { src, dst, .. } => {
            expr_has_printf_call(src) || expr_has_printf_call(dst)
        }
        Expr::PtrCopy {
            src, dst, count, ..
        } => expr_has_printf_call(src) || expr_has_printf_call(dst) || expr_has_printf_call(count),
        Expr::WriteBytes { dst, val, count } => {
            expr_has_printf_call(dst) || expr_has_printf_call(val) || expr_has_printf_call(count)
        }
        Expr::AtomicRef { ptr, .. } | Expr::AtomicLoad { ptr, .. } => expr_has_printf_call(ptr),
        Expr::AtomicStore { ptr, value, .. }
        | Expr::AtomicFetch { ptr, value, .. }
        | Expr::AtomicSwap { ptr, value, .. } => {
            expr_has_printf_call(ptr) || expr_has_printf_call(value)
        }
        Expr::AtomicCompareExchange {
            ptr,
            expected,
            desired,
            ..
        } => {
            expr_has_printf_call(ptr)
                || expr_has_printf_call(expected)
                || expr_has_printf_call(desired)
        }
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::Var(_)
        | Expr::Path(_)
        | Expr::AtomicFence { .. }
        | Expr::Todo(_) => false,
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{
        Block, Expr, ExternDecl, ExternFnDecl, FnParam, IndentStmt, Item, Prim, Program, Stmt,
        Type, Visibility,
    };

    fn printf_decl() -> ExternDecl {
        ExternDecl::Fn(ExternFnDecl {
            name: "printf".into(),
            params: vec![FnParam {
                name: "_0".into(),
                mutable: false,
                ty: Type::Ptr {
                    mutable: true,
                    inner: Box::new(Type::Prim(Prim::I8)),
                },
            }],
            variadic: true,
            ret: Some(Type::Prim(Prim::I32)),
        })
    }

    fn fmt_arg(bytes: &[u8]) -> Expr {
        Expr::Cast {
            expr: Box::new(Expr::Cast {
                expr: Box::new(Expr::MethodCall {
                    recv: Box::new(Expr::ByteStr(bytes.to_vec())),
                    method: "as_ptr".into(),
                    args: vec![],
                }),
                ty: Type::parse("*mut libc::c_char"),
            }),
            ty: Type::Ptr {
                mutable: true,
                inner: Box::new(Type::Prim(Prim::I8)),
            },
        }
    }

    fn printf_stmt(bytes: &[u8], value: Expr) -> Stmt {
        Stmt::Expr(Expr::Unsafe(Box::new(Block {
            stmts: vec![],
            tail: Some(Box::new(call("printf", vec![fmt_arg(bytes), value]))),
        })))
    }

    fn printf_stmt_args(bytes: &[u8], args: Vec<Expr>) -> Stmt {
        let mut call_args = vec![fmt_arg(bytes)];
        call_args.extend(args);
        Stmt::Expr(Expr::Unsafe(Box::new(Block {
            stmts: vec![],
            tail: Some(Box::new(call("printf", call_args))),
        })))
    }

    fn program(stmt: Stmt) -> Program {
        program_with_body(vec![stmt])
    }

    fn program_with_body(stmts: Vec<Stmt>) -> Program {
        Program {
            items: vec![
                Item::ExternBlock {
                    abi: "C".into(),
                    decls: vec![printf_decl()],
                },
                Item::Fn(crate::rust_ast::FnDef {
                    vis: Visibility::Private,
                    unsafe_extern_c: false,
                    name: "main".into(),
                    params: vec![],
                    ret: None,
                    body: stmts
                        .into_iter()
                        .map(|stmt| IndentStmt { depth: 1, stmt })
                        .collect(),
                }),
            ],
        }
    }

    fn run(stmt: Stmt) -> String {
        let mut program = program(stmt);
        fixup(&mut program);
        program.emit()
    }

    #[test]
    fn rewrites_percent_d_newline_to_println_and_removes_printf_extern() {
        let out = run(printf_stmt(b"%d\n\0", call("add", vec![int(2), int(3)])));

        assert_eq!(
            out,
            "\
fn main() {
    println!(\"{}\", add(2, 3));
}
"
        );
    }

    #[test]
    fn rewrites_plain_newline_to_println_without_arguments() {
        let out = run(Stmt::Expr(Expr::Unsafe(Box::new(Block {
            stmts: vec![],
            tail: Some(Box::new(call("printf", vec![fmt_arg(b"\n\0")]))),
        }))));

        assert_eq!(
            out,
            "\
fn main() {
    println!();
}
"
        );
    }

    #[test]
    fn rewrites_multiple_percent_d_conversions() {
        let out = run(printf_stmt_args(
            b"%d %d %d\n\0",
            vec![var("a"), var("b"), call("add", vec![int(2), int(3)])],
        ));

        assert_eq!(
            out,
            "\
fn main() {
    println!(\"{} {} {}\", a, b, add(2, 3));
}
"
        );
    }

    #[test]
    fn rewrites_percent_d_without_newline_to_print() {
        let out = run(printf_stmt(b"value=%d\0", var("x")));

        assert_eq!(
            out,
            "\
fn main() {
    print!(\"value={}\", x);
}
"
        );
    }

    #[test]
    fn rewrites_literal_percent_and_braces() {
        let out = run(Stmt::Expr(Expr::Unsafe(Box::new(Block {
            stmts: vec![],
            tail: Some(Box::new(call(
                "printf",
                vec![fmt_arg(b"ratio %% {ok}\n\0")],
            ))),
        }))));

        assert_eq!(
            out,
            "\
fn main() {
    println!(\"ratio % {{ok}}\");
}
"
        );
    }

    #[test]
    fn leaves_unsupported_formats_and_extern_declaration() {
        let out = run(printf_stmt(b"%u\n\0", var("x")));

        assert!(out.contains("fn printf(_0: *mut i8, ...) -> i32;"));
        assert!(out.contains("unsafe { printf("));
        assert!(!out.contains("println!"));
    }

    #[test]
    fn leaves_mismatched_argument_counts_unsupported() {
        let out = run(printf_stmt_args(b"%d %d\n\0", vec![var("x")]));

        assert!(out.contains("fn printf(_0: *mut i8, ...) -> i32;"));
        assert!(out.contains("unsafe { printf("));
        assert!(!out.contains("println!"));
    }

    #[test]
    fn leaves_supported_calls_when_any_printf_call_is_unsupported() {
        let mut program = program_with_body(vec![
            printf_stmt(b"%d\n\0", var("x")),
            printf_stmt(b"%f\n\0", var("y")),
        ]);
        fixup(&mut program);
        let out = program.emit();

        assert!(out.contains("fn printf(_0: *mut i8, ...) -> i32;"));
        assert!(out.contains("unsafe { printf("));
        assert!(!out.contains("println!"));
    }
}
