use crate::backend::rust_ast::{
    BinOp, Block, Expr, FnDef, FnParam, Ident, ImplItem, IndentStmt, Item, Path, Prim, Program,
    RustValue, Stmt, Type, Visibility,
};
use crate::function_identity::CallBinding;

pub(super) const ATOI: &str = "__slate_atoi";
pub(super) const ATOL: &str = "__slate_atol";

pub(in crate::backend) fn inject(program: &mut Program) {
    let mut needed: Vec<&'static str> = Vec::new();
    {
        let mut calls = Vec::new();
        for item in &program.items {
            item_calls(item, &mut calls);
        }
        for name in [ATOI, ATOL] {
            if calls.iter().any(|(callee, _)| callee.as_str() == name) {
                needed.push(name);
            }
        }
    }
    if needed.is_empty() {
        return;
    }
    let helpers: Vec<Item> = needed.iter().map(|&name| build(name)).collect();
    let pos = insert_pos(&program.items);
    program.items.splice(pos..pos, helpers);
}

fn item_calls<'a>(item: &'a Item, out: &mut Vec<(&'a Ident, &'a [Expr])>) {
    match item {
        Item::Fn(func) => {
            for stmt in &func.body {
                stmt.stmt.collect_calls(out);
            }
        }
        Item::InlineMod { items, .. } => {
            for item in items {
                item_calls(item, out);
            }
        }
        Item::Impl(impl_block) => {
            for impl_item in &impl_block.items {
                if let ImplItem::Method(method) = impl_item
                    && let Expr::Block(block) = &method.body
                {
                    for stmt in &block.stmts {
                        stmt.stmt.collect_calls(out);
                    }
                    if let Some(tail) = &block.tail {
                        tail.collect_calls(out);
                    }
                }
            }
        }
        _ => {}
    }
}

fn insert_pos(items: &[Item]) -> usize {
    items
        .iter()
        .position(|item| matches!(item, Item::Fn(_) | Item::Impl(_) | Item::Static { .. }))
        .unwrap_or(items.len())
}

fn build(name: &str) -> Item {
    match name {
        ATOL => ato_int_prelude(ATOL, Prim::I64),
        _ => ato_int_prelude(ATOI, Prim::I32),
    }
}

fn var(name: &str) -> Expr {
    Expr::Var(name.into())
}

fn byte(b: u8) -> Expr {
    Expr::Value(RustValue::TypedUInt(b as u128, Prim::U8))
}

fn usize_lit(v: usize) -> Expr {
    Expr::Value(RustValue::Usize(v))
}

fn bin(op: BinOp, lhs: Expr, rhs: Expr) -> Expr {
    Expr::Binary {
        op,
        lhs: Box::new(lhs),
        rhs: Box::new(rhs),
    }
}

fn idx() -> Expr {
    Expr::Index {
        base: Box::new(var("bytes")),
        index: Box::new(var("i")),
    }
}

fn mcall(recv: Expr, method: &str, args: Vec<Expr>) -> Expr {
    Expr::MethodCall {
        recv: Box::new(recv),
        method: method.into(),
        args,
    }
}

fn stmt(s: Stmt) -> IndentStmt {
    IndentStmt { depth: 0, stmt: s }
}

fn step_i() -> Stmt {
    Stmt::CompoundAssign {
        target: var("i"),
        op: BinOp::Add,
        value: usize_lit(1),
    }
}

fn in_bounds() -> Expr {
    bin(BinOp::Lt, var("i"), var("n"))
}

fn is_c_space() -> Expr {
    bin(
        BinOp::Or,
        mcall(idx(), "is_ascii_whitespace", Vec::new()),
        bin(BinOp::Eq, idx(), byte(0x0b)),
    )
}

fn is_sign() -> Expr {
    bin(
        BinOp::Or,
        bin(BinOp::Eq, idx(), byte(b'+')),
        bin(BinOp::Eq, idx(), byte(b'-')),
    )
}

fn while_stmt(cond: Expr) -> Stmt {
    Stmt::While {
        cond,
        body: Block {
            stmts: vec![stmt(step_i())],
            tail: None,
        },
    }
}

fn let_stmt(name: &str, mutable: bool, init: Expr) -> Stmt {
    Stmt::Let {
        name: name.into(),
        mutable,
        ty: None,
        init: Some(init),
    }
}

fn ato_int_prelude(name: &str, ret: Prim) -> Item {
    let cstr = Expr::Call {
        binding: CallBinding::Generated,
        func: Box::new(Expr::Path(Path::new(
            ["std", "ffi", "CStr", "from_ptr"].map(Ident::from),
        ))),
        args: vec![var("s")],
    };
    let bytes = mcall(
        Expr::Unsafe(Box::new(Block {
            stmts: Vec::new(),
            tail: Some(Box::new(cstr)),
        })),
        "to_bytes",
        Vec::new(),
    );

    let slice = Expr::Ref {
        mutable: false,
        expr: Box::new(Expr::Index {
            base: Box::new(var("bytes")),
            index: Box::new(Expr::Range {
                start: Box::new(var("start")),
                end: Box::new(var("i")),
            }),
        }),
    };
    let text = Expr::Call {
        binding: CallBinding::Generated,
        func: Box::new(Expr::Path(Path::new(
            ["std", "str", "from_utf8"].map(Ident::from),
        ))),
        args: vec![slice],
    };
    let parsed = mcall(
        mcall(mcall(text, "unwrap", Vec::new()), "parse", Vec::new()),
        "unwrap_or",
        vec![Expr::Value(RustValue::TypedInt(0, ret))],
    );

    let body = vec![
        stmt(let_stmt("bytes", false, bytes)),
        stmt(let_stmt("n", false, mcall(var("bytes"), "len", Vec::new()))),
        stmt(let_stmt("i", true, usize_lit(0))),
        stmt(while_stmt(bin(BinOp::And, in_bounds(), is_c_space()))),
        stmt(let_stmt("start", false, var("i"))),
        stmt(Stmt::If {
            cond: bin(BinOp::And, in_bounds(), is_sign()),
            then_body: vec![stmt(step_i())],
            else_body: Vec::new(),
        }),
        stmt(let_stmt("digits", false, var("i"))),
        stmt(while_stmt(bin(
            BinOp::And,
            in_bounds(),
            mcall(idx(), "is_ascii_digit", Vec::new()),
        ))),
        stmt(Stmt::If {
            cond: bin(BinOp::Eq, var("i"), var("digits")),
            then_body: vec![stmt(Stmt::Return(Some(Expr::Value(RustValue::TypedInt(
                0, ret,
            )))))],
            else_body: Vec::new(),
        }),
        stmt(Stmt::Return(Some(parsed))),
    ];

    Item::Fn(FnDef {
        attrs: Vec::new(),
        vis: Visibility::Private,
        unsafe_: false,
        abi: None,
        name: name.into(),
        params: vec![FnParam {
            name: "s".into(),
            mutable: false,
            ty: Type::Ptr {
                mutable: false,
                inner: Box::new(Type::Prim(Prim::I8)),
            },
        }],
        ret: Some(Type::Prim(ret)),
        body,
    })
}
