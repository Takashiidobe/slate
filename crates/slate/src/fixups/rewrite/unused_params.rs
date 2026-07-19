use crate::fixups::idents::stmt_ident_count;
use crate::fixups::support::walk::{body_expr_any, body_exprs_mut_with, expr_any, exprs_mut_with};
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLocation, TraceLogger, TraceSnippet, fact,
};
use crate::rust_ast::{
    Attr, Block, Expr, FnDef, ImplItem, IndentStmt, Item, Program, Stmt, Type, UnaryOp,
};

pub(in crate::fixups) fn fixup(program: &mut Program) -> bool {
    let mut logger = crate::fixups::trace::NoopLogger;
    UnusedParams::new(&mut logger).fixup(program)
}

pub(in crate::fixups) struct UnusedParams<'a> {
    logger: &'a mut dyn TraceLogger,
}

impl<'a> UnusedParams<'a> {
    pub(in crate::fixups) fn new(logger: &'a mut dyn TraceLogger) -> Self {
        Self { logger }
    }

    pub(in crate::fixups) fn fixup(&mut self, program: &mut Program) -> bool {
        let mut changed = false;
        while let Some((name, param_index)) = find_removable(program) {
            let before = self.logger.is_enabled().then(|| program.emit());
            let removed = removed_param(program, &name, param_index);
            remove_param(program, &name, param_index);
            if let (Some(before), Some((param_name, param_ty))) = (before, removed) {
                self.logger.rewrite(RewriteEvent {
                    pass: TracePass::UnusedParams,
                    kind: "remove_unused_param".into(),
                    location: TraceLocation {
                        function: Some(name.clone()),
                        ..TraceLocation::default()
                    },
                    before: vec![TraceSnippet::new("program", before.trim_end())],
                    after: vec![TraceSnippet::new("program", program.emit().trim_end())],
                    facts: vec![
                        fact("function", name),
                        fact("param", param_name),
                        fact("param_index", param_index.to_string()),
                        fact("param_type", param_ty),
                    ],
                });
            }
            changed = true;
        }
        changed
    }
}

fn removed_param(program: &Program, name: &str, param_index: usize) -> Option<(String, String)> {
    let mut removed = None;
    each_item(&program.items, &mut |item| {
        if removed.is_some() {
            return;
        }
        let Item::Fn(f) = item else { return };
        if f.name == name
            && let Some(param) = f.params.get(param_index)
        {
            removed = Some((param.name.clone(), param.ty.render()));
        }
    });
    removed
}

fn find_removable(program: &Program) -> Option<(String, usize)> {
    let mut found = None;
    each_item(&program.items, &mut |item| {
        if found.is_some() {
            return;
        }
        let Item::Fn(f) = item else { return };
        if !eligible_signature(f) {
            return;
        }
        for (index, param) in f.params.iter().enumerate() {
            if !trivially_droppable(&param.ty)
                || body_ident_count(&f.body, &param.name) != 0
                || program_has_unsafe_ref(program, &f.name)
                || any_undroppable_call(program, &f.name, index, f.params.len())
            {
                continue;
            }
            found = Some((f.name.clone(), index));
            break;
        }
    });
    found
}

fn eligible_signature(f: &FnDef) -> bool {
    f.name != "main" && f.abi.is_none() && !is_exported(&f.attrs)
}

fn is_exported(attrs: &[Attr]) -> bool {
    attrs
        .iter()
        .any(|attr| matches!(attr, Attr::Used(_) | Attr::NoMangle | Attr::WeakLinkage))
}

fn body_ident_count(body: &[IndentStmt], name: &str) -> usize {
    body.iter()
        .map(|stmt| stmt_ident_count(&stmt.stmt, name))
        .sum()
}

// excludes Vec/Box/File-like types: dropping them earlier than the call would move their Drop point
fn trivially_droppable(ty: &Type) -> bool {
    match ty {
        Type::Prim(_) | Type::Unit | Type::Ptr { .. } | Type::Ref { .. } => true,
        Type::Array { elem, .. } => trivially_droppable(elem),
        Type::Custom(_)
        | Type::LongDouble
        | Type::TyVar(_)
        | Type::CLib(_)
        | Type::Complex(_)
        | Type::Generic { .. }
        | Type::VaList
        | Type::Str
        | Type::Slice(_)
        | Type::FnPtr { .. }
        | Type::Variadic
        | Type::Never => false,
    }
}

fn is_pure_expr(expr: &Expr) -> bool {
    match expr {
        Expr::Value(_) | Expr::Str(_) | Expr::ByteStr(_) | Expr::CStr(_) | Expr::HexFloat(_) => {
            true
        }
        Expr::Var(_) | Expr::Path(_) => true,
        Expr::Unary { op, expr } => matches!(op, UnaryOp::Neg | UnaryOp::Not) && is_pure_expr(expr),
        Expr::Binary { lhs, rhs, .. }
        | Expr::Range {
            start: lhs,
            end: rhs,
        } => is_pure_expr(lhs) && is_pure_expr(rhs),
        Expr::Cast { expr, .. } => is_pure_expr(expr),
        Expr::Field { base, .. } | Expr::TupleField { base, .. } => is_pure_expr(base),
        Expr::Index { base, index } => is_pure_expr(base) && is_pure_expr(index),
        Expr::StructLit { fields, .. } => fields.iter().all(|(_, value)| is_pure_expr(value)),
        Expr::TupleStructLit { fields, .. } => fields.iter().all(is_pure_expr),
        Expr::ArrayLit(values) | Expr::VecLit(values) => values.iter().all(is_pure_expr),
        Expr::ArrayRepeat { elem, .. } => is_pure_expr(elem),
        _ => false,
    }
}

fn is_named_callee(expr: &Expr, name: &str) -> bool {
    match expr {
        Expr::Var(n) => n.as_str() == name,
        Expr::Path(path) => path.segments.len() == 1 && path.segments[0].as_str() == name,
        _ => false,
    }
}

// true if `name` is referenced anywhere other than as the callee of a direct call (address taken, stored, passed as a callback)
fn expr_has_unsafe_ref(expr: &Expr, name: &str) -> bool {
    match expr {
        Expr::Var(_) | Expr::Path(_) => is_named_callee(expr, name),
        Expr::Call { func, args } => {
            let func_bad = if is_named_callee(func, name) {
                false
            } else {
                expr_has_unsafe_ref(func, name)
            };
            func_bad || args.iter().any(|arg| expr_has_unsafe_ref(arg, name))
        }
        Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. } => expr_has_unsafe_ref(expr, name),
        Expr::Binary { lhs, rhs, .. }
        | Expr::Range {
            start: lhs,
            end: rhs,
        }
        | Expr::Index {
            base: lhs,
            index: rhs,
        } => expr_has_unsafe_ref(lhs, name) || expr_has_unsafe_ref(rhs, name),
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            expr_has_unsafe_ref(recv, name) || args.iter().any(|arg| expr_has_unsafe_ref(arg, name))
        }
        Expr::Field { base, .. }
        | Expr::TupleField { base, .. }
        | Expr::ArrayPtr { array: base, .. } => expr_has_unsafe_ref(base, name),
        Expr::StructLit { fields, .. } => fields
            .iter()
            .any(|(_, value)| expr_has_unsafe_ref(value, name)),
        Expr::TupleStructLit { fields, .. } => {
            fields.iter().any(|value| expr_has_unsafe_ref(value, name))
        }
        Expr::ArrayLit(elems) | Expr::VecLit(elems) => {
            elems.iter().any(|elem| expr_has_unsafe_ref(elem, name))
        }
        Expr::ArrayRepeat { elem, .. } => expr_has_unsafe_ref(elem, name),
        Expr::VecRepeat { elem, len } => {
            expr_has_unsafe_ref(elem, name) || expr_has_unsafe_ref(len, name)
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            expr_has_unsafe_ref(cond, name)
                || expr_has_unsafe_ref(then_expr, name)
                || expr_has_unsafe_ref(else_expr, name)
        }
        Expr::Match { expr, arms } => {
            expr_has_unsafe_ref(expr, name)
                || arms.iter().any(|arm| expr_has_unsafe_ref(&arm.value, name))
        }
        Expr::Block(block) | Expr::Unsafe(block) => block_has_unsafe_ref(block, name),
        Expr::Closure { body, .. } => expr_has_unsafe_ref(body, name),
        Expr::Macro { args, .. } => args.iter().any(|arg| expr_has_unsafe_ref(arg, name)),
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::CStr(_)
        | Expr::Todo(_) => false,
        // atomics/transmute/raw-pointer intrinsics: can't prove call-position here, so treat any occurrence as unsafe
        other => expr_any(other, &mut |e| is_named_callee(e, name)),
    }
}

fn block_has_unsafe_ref(block: &Block, name: &str) -> bool {
    body_has_unsafe_ref(&block.stmts, name)
        || block
            .tail
            .as_deref()
            .is_some_and(|tail| expr_has_unsafe_ref(tail, name))
}

fn body_has_unsafe_ref(body: &[IndentStmt], name: &str) -> bool {
    body.iter()
        .any(|stmt| stmt_has_unsafe_ref(&stmt.stmt, name))
}

fn stmt_has_unsafe_ref(stmt: &Stmt, name: &str) -> bool {
    match stmt {
        Stmt::Let { init, .. } => init
            .as_ref()
            .is_some_and(|expr| expr_has_unsafe_ref(expr, name)),
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            expr_has_unsafe_ref(cond, name)
                || body_has_unsafe_ref(then_body, name)
                || expr_has_unsafe_ref(then_value, name)
                || body_has_unsafe_ref(else_body, name)
                || expr_has_unsafe_ref(else_value, name)
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            expr_has_unsafe_ref(target, name) || expr_has_unsafe_ref(value, name)
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => expr_has_unsafe_ref(expr, name),
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => false,
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            expr_has_unsafe_ref(cond, name)
                || body_has_unsafe_ref(then_body, name)
                || body_has_unsafe_ref(else_body, name)
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            body_has_unsafe_ref(body, name)
        }
        Stmt::For { iter, body, .. } => {
            expr_has_unsafe_ref(iter, name) || body_has_unsafe_ref(body, name)
        }
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
            block_has_unsafe_ref(body, name)
        }
        Stmt::Match { expr, arms } => {
            expr_has_unsafe_ref(expr, name)
                || arms.iter().any(|arm| body_has_unsafe_ref(&arm.body, name))
        }
    }
}

fn program_has_unsafe_ref(program: &Program, name: &str) -> bool {
    let mut found = false;
    each_item(&program.items, &mut |item| {
        if found {
            return;
        }
        found = item_has_unsafe_ref(item, name);
    });
    found
}

fn item_has_unsafe_ref(item: &Item, name: &str) -> bool {
    match item {
        Item::Fn(f) => body_has_unsafe_ref(&f.body, name),
        Item::Static { init, .. } => expr_has_unsafe_ref(init, name),
        Item::Impl(block) => block.items.iter().any(|it| match it {
            ImplItem::Method(m) => expr_has_unsafe_ref(&m.body, name),
            ImplItem::AssocType { .. } => false,
        }),
        _ => false,
    }
}

fn any_undroppable_call(
    program: &Program,
    name: &str,
    param_index: usize,
    expected_len: usize,
) -> bool {
    let mut found = false;
    each_item(&program.items, &mut |item| {
        if found {
            return;
        }
        let pred = &mut |e: &Expr| is_undroppable_call(e, name, param_index, expected_len);
        found = match item {
            Item::Fn(f) => body_expr_any(&f.body, pred),
            Item::Static { init, .. } => expr_any(init, pred),
            Item::Impl(block) => block.items.iter().any(|it| match it {
                ImplItem::Method(m) => expr_any(&m.body, pred),
                ImplItem::AssocType { .. } => false,
            }),
            _ => false,
        };
    });
    found
}

fn is_undroppable_call(expr: &Expr, name: &str, param_index: usize, expected_len: usize) -> bool {
    matches!(expr, Expr::Call { func, args }
        if is_named_callee(func, name)
            && (args.len() != expected_len || !is_pure_expr(&args[param_index])))
}

fn remove_param(program: &mut Program, name: &str, param_index: usize) {
    each_item_mut(&mut program.items, &mut |item| {
        if let Item::Fn(f) = item
            && f.name == name
        {
            f.params.remove(param_index);
        }
    });
    remove_arg_everywhere(program, name, param_index);
}

fn remove_arg_everywhere(program: &mut Program, name: &str, param_index: usize) {
    each_item_mut(&mut program.items, &mut |item| {
        let visit = &mut |e: &mut Expr| {
            if let Expr::Call { func, args } = e
                && is_named_callee(func, name)
                && param_index < args.len()
            {
                args.remove(param_index);
            }
            true
        };
        match item {
            Item::Fn(f) => body_exprs_mut_with(&mut f.body, visit),
            Item::Static { init, .. } => exprs_mut_with(init, visit),
            Item::Impl(block) => {
                for it in &mut block.items {
                    if let ImplItem::Method(m) = it {
                        exprs_mut_with(&mut m.body, visit);
                    }
                }
            }
            _ => {}
        }
    });
}

fn each_item<'a>(items: &'a [Item], f: &mut impl FnMut(&'a Item)) {
    for item in items {
        match item {
            Item::Cfg { item, .. } => each_item(std::slice::from_ref(item.as_ref()), f),
            other => f(other),
        }
    }
}

fn each_item_mut(items: &mut [Item], f: &mut impl FnMut(&mut Item)) {
    for item in items {
        match item {
            Item::Cfg { item, .. } => each_item_mut(std::slice::from_mut(item.as_mut()), f),
            other => f(other),
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Expr, Prim, Stmt, Type};

    fn program_with(functions: Vec<FnDef>) -> Program {
        Program {
            items: functions.into_iter().map(Item::Fn).collect(),
        }
    }

    fn simple_fn(name: &str, params: Vec<(&str, Type)>, body: Vec<Stmt>) -> FnDef {
        FnDef {
            attrs: vec![],
            vis: crate::rust_ast::Visibility::Private,
            unsafe_: false,
            abi: None,
            name: name.into(),
            params: params
                .into_iter()
                .map(|(name, ty)| crate::rust_ast::FnParam {
                    name: name.into(),
                    mutable: false,
                    ty,
                })
                .collect(),
            ret: None,
            body: body
                .into_iter()
                .map(|stmt| IndentStmt { depth: 1, stmt })
                .collect(),
        }
    }

    #[test]
    fn removes_dead_param_and_rewrites_the_only_call_site() {
        let callee = simple_fn(
            "sum_items",
            vec![
                ("items", Type::Slice(Box::new(Type::Prim(Prim::I32)))),
                ("len", Type::Prim(Prim::I32)),
            ],
            vec![Stmt::Return(Some(var("items")))],
        );
        let caller = simple_fn(
            "main",
            vec![],
            vec![Stmt::Expr(call("sum_items", vec![var("values"), int(4)]))],
        );
        let mut program = program_with(vec![callee, caller]);

        assert!(fixup(&mut program));

        let Item::Fn(callee) = &program.items[0] else {
            unreachable!()
        };
        assert_eq!(callee.params.len(), 1);
        assert_eq!(callee.params[0].name, "items");

        let Item::Fn(caller) = &program.items[1] else {
            unreachable!()
        };
        let Stmt::Expr(Expr::Call { args, .. }) = &caller.body[0].stmt else {
            unreachable!()
        };
        assert_eq!(args.len(), 1);
    }

    #[test]
    fn leaves_param_alone_when_call_site_argument_is_impure() {
        let callee = simple_fn(
            "f",
            vec![("unused", Type::Prim(Prim::I32))],
            vec![Stmt::Return(None)],
        );
        let caller = simple_fn(
            "main",
            vec![],
            vec![Stmt::Expr(call("f", vec![call("next", vec![])]))],
        );
        let mut program = program_with(vec![callee, caller]);

        assert!(!fixup(&mut program));

        let Item::Fn(callee) = &program.items[0] else {
            unreachable!()
        };
        assert_eq!(callee.params.len(), 1);
    }

    #[test]
    fn leaves_param_alone_when_function_value_is_taken() {
        let callee = simple_fn(
            "f",
            vec![("unused", Type::Prim(Prim::I32))],
            vec![Stmt::Return(None)],
        );
        let caller = simple_fn(
            "main",
            vec![],
            vec![Stmt::Let {
                name: "ptr".into(),
                mutable: false,
                ty: None,
                init: Some(var("f")),
            }],
        );
        let mut program = program_with(vec![callee, caller]);

        assert!(!fixup(&mut program));
    }

    #[test]
    fn leaves_non_trivially_droppable_by_value_param_alone() {
        let callee = simple_fn(
            "f",
            vec![("unused", Type::Custom("Vec<i32>".into()))],
            vec![Stmt::Return(None)],
        );
        let caller = simple_fn(
            "main",
            vec![],
            vec![Stmt::Expr(call("f", vec![call("make_vec", vec![])]))],
        );
        let mut program = program_with(vec![callee, caller]);

        assert!(!fixup(&mut program));
    }

    #[test]
    fn leaves_used_param_alone() {
        let callee = simple_fn(
            "f",
            vec![("x", Type::Prim(Prim::I32))],
            vec![Stmt::Return(Some(var("x")))],
        );
        let mut program = program_with(vec![callee]);

        assert!(!fixup(&mut program));
    }

    #[test]
    fn leaves_main_alone() {
        let main = simple_fn(
            "main",
            vec![("argc", Type::Prim(Prim::I32))],
            vec![Stmt::Return(None)],
        );
        let mut program = program_with(vec![main]);

        assert!(!fixup(&mut program));
    }
}
