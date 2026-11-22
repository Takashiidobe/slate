use super::rules::walk::{child_exprs, child_exprs_mut};
use crate::backend::rust_ast::{
    AsmOperand, AtomicPlace, Block, Expr, ImplItem, Item, Pointee, Program, RustValue, Stmt,
    UnaryOp,
};

fn needs_unsafe(expr: &Expr) -> bool {
    match expr {
        Expr::Unary {
            op: UnaryOp::Deref(Pointee::Raw),
            ..
        }
        | Expr::Transmute { .. }
        | Expr::CopyNonoverlapping { .. }
        | Expr::PtrCopy { .. }
        | Expr::WriteBytes { .. } => true,
        Expr::Var { unsafe_, .. }
        | Expr::Call { unsafe_, .. }
        | Expr::MethodCall { unsafe_, .. }
        | Expr::MethodCallGeneric { unsafe_, .. } => unsafe_.0,
        Expr::Field { union, .. } => union.0,
        Expr::AtomicRef { place, .. }
        | Expr::AtomicLoad { place, .. }
        | Expr::AtomicStore { place, .. }
        | Expr::AtomicFetch { place, .. }
        | Expr::AtomicSwap { place, .. }
        | Expr::AtomicCompareExchange { place, .. } => matches!(place, AtomicPlace::Ptr(_)),
        _ => false,
    }
}

pub(super) fn place(program: &mut Program) {
    for item in &mut program.items {
        place_item(item);
    }
}

fn place_item(item: &mut Item) {
    match item {
        Item::Fn(func) => place_stmts(&mut func.body, false),
        Item::MacroRules { body, .. } => place_stmts(body, false),
        Item::InlineMod { items, .. } => items.iter_mut().for_each(place_item),
        Item::Cfg { item, .. } => place_item(item),
        Item::Static { init, .. } | Item::Const { init, .. } => place_slot(init, false),
        Item::Impl(impl_block) => {
            for impl_item in &mut impl_block.items {
                if let ImplItem::Method(method) = impl_item {
                    place_slot(&mut method.body, false);
                }
            }
        }
        _ => {}
    }
}

fn has_let(stmts: &[Stmt]) -> bool {
    stmts
        .iter()
        .any(|stmt| matches!(stmt, Stmt::Let { .. } | Stmt::LetIf { .. }))
}

fn place_stmts(stmts: &mut Vec<Stmt>, covered: bool) {
    let mut out = Vec::with_capacity(stmts.len());
    let mut scoped_last = false;
    for stmt in std::mem::take(stmts) {
        scoped_last = false;
        match stmt {
            Stmt::Unsafe { mut body } | Stmt::Block(mut body) if !body.stmts.is_empty() => {
                place_block(&mut body, covered);
                if body.tail.is_none() && !has_let(&body.stmts) {
                    out.extend(body.stmts);
                } else {
                    scoped_last = body.tail.is_none();
                    out.push(Stmt::Block(body));
                }
            }
            mut stmt => {
                place_stmt(&mut stmt, covered);
                out.push(stmt);
            }
        }
    }
    if scoped_last && let Some(Stmt::Block(body)) = out.pop() {
        out.extend(body.stmts);
    }
    if !covered {
        merge_adjacent(&mut out);
    }
    *stmts = out;
}

fn merge_adjacent(stmts: &mut Vec<Stmt>) {
    let mut out: Vec<Stmt> = Vec::with_capacity(stmts.len());
    for stmt in std::mem::take(stmts) {
        if let Stmt::Unsafe { body } = &stmt
            && body.tail.is_none()
            && !has_let(&body.stmts)
            && let Some(Stmt::Unsafe { body: previous }) = out.last_mut()
            && previous.tail.is_none()
            && !has_let(&previous.stmts)
        {
            let Stmt::Unsafe { body } = stmt else {
                unreachable!()
            };
            previous.stmts.extend(body.stmts);
            continue;
        }
        out.push(stmt);
    }
    *stmts = out;
}

fn place_block(block: &mut Block, covered: bool) {
    place_stmts(&mut block.stmts, covered);
    if let Some(tail) = &mut block.tail {
        place_slot(tail, covered);
    }
}

fn wrap_stmt(stmt: &mut Stmt) {
    place_stmt(stmt, true);
    let inner = std::mem::replace(stmt, Stmt::Break(None));
    *stmt = Stmt::Unsafe {
        body: Block {
            stmts: vec![inner],
            tail: None,
        },
    };
}

fn place_stmt(stmt: &mut Stmt, covered: bool) {
    if !covered {
        match stmt {
            Stmt::InlineAsm(_) => return wrap_stmt(stmt),
            Stmt::Expr(expr) => {
                strip(expr);
                if matches!(expr, Expr::Macro { .. }) && pending_in(expr, Context::Value) {
                    return wrap_stmt(stmt);
                }
            }
            Stmt::Assign { target, .. } => {
                strip(target);
                if pending_in(target, Context::Write) {
                    return wrap_stmt(stmt);
                }
            }
            Stmt::CompoundAssign { target, .. } => {
                strip(target);
                if pending_in(target, Context::Place) {
                    return wrap_stmt(stmt);
                }
            }
            _ => {}
        }
    }
    match stmt {
        Stmt::Cfg { body, .. }
        | Stmt::Loop { body, .. }
        | Stmt::Scope { body }
        | Stmt::LabeledBlock { body, .. } => place_stmts(body, covered),
        Stmt::Let {
            init: Some(expr), ..
        }
        | Stmt::Expr(expr)
        | Stmt::Return(Some(expr)) => place_slot(expr, covered),
        Stmt::Assign { target, value } => {
            place_target(target, Context::Write, covered);
            place_slot(value, covered);
        }
        Stmt::CompoundAssign { target, value, .. } => {
            place_target(target, Context::Place, covered);
            place_slot(value, covered);
        }
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            place_slot(cond, covered);
            place_stmts(then_body, covered);
            place_slot(then_value, covered);
            place_stmts(else_body, covered);
            place_slot(else_value, covered);
        }
        Stmt::InlineAsm(asm) => {
            for operand in &mut asm.operands {
                match operand {
                    AsmOperand::In { value, .. }
                    | AsmOperand::Out { value, .. }
                    | AsmOperand::Const(value)
                    | AsmOperand::Sym(value) => place_slot(value, covered),
                    AsmOperand::InOut { input, output, .. } => {
                        place_slot(input, covered);
                        place_slot(output, covered);
                    }
                    AsmOperand::Label { state, value } => {
                        place_slot(state, covered);
                        place_slot(value, covered);
                    }
                }
            }
        }
        Stmt::Unsafe { body } | Stmt::Block(body) => place_block(body, covered),
        Stmt::While { cond, body, .. } => {
            place_slot(cond, covered);
            place_block(body, covered);
        }
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            place_slot(cond, covered);
            place_stmts(then_body, covered);
            place_stmts(else_body, covered);
        }
        Stmt::For { iter, body, .. } => {
            place_slot(iter, covered);
            place_stmts(body, covered);
        }
        Stmt::Match { expr, arms } => {
            place_slot(expr, covered);
            for arm in arms {
                place_stmts(&mut arm.body, covered);
            }
        }
        Stmt::Let { init: None, .. }
        | Stmt::Return(None)
        | Stmt::Comment(_)
        | Stmt::Break(_)
        | Stmt::Continue(_) => {}
    }
}

fn strip(expr: &mut Expr) {
    while let Expr::Unsafe(block) = expr {
        let block = std::mem::take(block);
        *expr = match *block {
            Block {
                stmts,
                tail: Some(tail),
            } if stmts.is_empty() => *tail,
            block => Expr::Block(Box::new(block)),
        };
    }
}

const FORMAT_MACROS: &[&str] = &[
    "print", "println", "eprint", "eprintln", "format", "write", "writeln", "panic",
];

#[derive(Clone, Copy, PartialEq)]
enum Context {
    Value,
    Place,
    RawPlace,
    Write,
}

fn child_contexts(expr: &Expr, context: Context) -> Vec<Context> {
    let projected = match context {
        Context::Write => Context::Write,
        Context::Value | Context::Place | Context::RawPlace => Context::Place,
    };
    match expr {
        Expr::AddrOf { .. }
        | Expr::Unary {
            op: UnaryOp::Raw(_),
            ..
        } => vec![Context::RawPlace],
        Expr::Ref { .. } | Expr::ArrayPtr { .. } => vec![Context::Place],
        Expr::Field { .. } | Expr::TupleField { .. } => vec![projected],
        Expr::Index { .. } => vec![Context::Place, Context::Value],
        Expr::MethodCall { args, .. } | Expr::MethodCallGeneric { args, .. } => {
            std::iter::once(Context::Place)
                .chain(args.iter().map(|_| Context::Value))
                .collect()
        }
        Expr::Macro { name, args }
            if name.ends_with("addr_of") || name.ends_with("addr_of_mut") =>
        {
            vec![Context::RawPlace; args.len()]
        }
        Expr::Macro { args, .. } => vec![Context::Place; args.len()],
        Expr::Match { arms, .. } => std::iter::once(Context::Place)
            .chain(arms.iter().map(|_| Context::Value))
            .collect(),
        other => vec![Context::Value; child_exprs(other).len()],
    }
}

fn format_macro(expr: &Expr) -> bool {
    matches!(expr, Expr::Macro { name, .. } if FORMAT_MACROS.contains(&name.as_str()))
}

fn self_contained(expr: &Expr) -> bool {
    matches!(expr, Expr::Block(_) | Expr::Closure { .. }) || format_macro(expr)
}

fn is_place(expr: &Expr) -> bool {
    matches!(
        expr,
        Expr::Var { .. }
            | Expr::Unary {
                op: UnaryOp::Deref(_),
                ..
            }
            | Expr::Field { .. }
            | Expr::TupleField { .. }
            | Expr::Index { .. }
    )
}

fn place_format_args(args: &mut [Expr], covered: bool) {
    for arg in args {
        place_slot(arg, covered);
        if covered && is_place(arg) && pending_in(arg, Context::Value) {
            let inner = std::mem::replace(arg, Expr::Value(RustValue::I64(0)));
            *arg = Expr::Block(Box::new(Block {
                stmts: Vec::new(),
                tail: Some(Box::new(inner)),
            }));
        }
    }
}

fn needs(expr: &Expr, context: Context) -> bool {
    needs_unsafe(expr)
        && !match context {
            Context::RawPlace => matches!(expr, Expr::Var { .. } | Expr::Field { .. }),
            Context::Write => matches!(expr, Expr::Field { .. }),
            Context::Value | Context::Place => false,
        }
}

fn view(expr: &Expr) -> &Expr {
    match expr {
        Expr::Unsafe(block) if block.stmts.is_empty() && block.tail.is_some() => {
            view(block.tail.as_deref().unwrap())
        }
        other => other,
    }
}

fn pending_in(expr: &Expr, context: Context) -> bool {
    let expr = view(expr);
    if matches!(expr, Expr::Unsafe(_)) || self_contained(expr) {
        return false;
    }
    if needs(expr, context) {
        return true;
    }
    let mut count = 0;
    for (child, child_context) in child_exprs(expr)
        .into_iter()
        .zip(child_contexts(expr, context))
    {
        if pending_in(child, child_context) {
            if child_context != Context::Value || matches!(expr, Expr::Cast { .. }) {
                return true;
            }
            count += 1;
        }
    }
    count >= 2
}

fn place_slot(expr: &mut Expr, covered: bool) {
    strip(expr);
    if !covered && pending_in(expr, Context::Value) {
        cover(expr);
    } else {
        descend(expr, Context::Value, covered);
    }
}

fn place_target(expr: &mut Expr, context: Context, covered: bool) {
    strip(expr);
    descend(expr, context, covered);
}

fn cover(expr: &mut Expr) {
    descend(expr, Context::Value, true);
    let inner = std::mem::replace(expr, Expr::Value(RustValue::I64(0)));
    *expr = Expr::Unsafe(Box::new(Block {
        stmts: Vec::new(),
        tail: Some(Box::new(inner)),
    }));
}

fn descend(expr: &mut Expr, context: Context, covered: bool) {
    strip(expr);
    match expr {
        Expr::Block(block) => return place_block(block, covered),
        Expr::Closure { body, .. } => return place_slot(body, covered),
        Expr::Macro { name, args } if FORMAT_MACROS.contains(&name.as_str()) => {
            return place_format_args(args, covered);
        }
        Expr::AtomicRef { place, .. }
        | Expr::AtomicLoad { place, .. }
        | Expr::AtomicStore { place, .. }
        | Expr::AtomicFetch { place, .. }
        | Expr::AtomicSwap { place, .. }
        | Expr::AtomicCompareExchange { place, .. } => {
            if let Some(ptr) = place.ptr_expr_mut() {
                place_slot(ptr, covered);
            }
        }
        _ => {}
    }
    let contexts = child_contexts(expr, context);
    for (child, child_context) in child_exprs_mut(expr).into_iter().zip(contexts) {
        strip(child);
        if !covered && child_context == Context::Value && pending_in(child, Context::Value) {
            cover(child);
        } else {
            descend(child, child_context, covered);
        }
    }
}
