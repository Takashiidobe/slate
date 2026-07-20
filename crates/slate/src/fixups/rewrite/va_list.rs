use crate::fixups::facts::{AstPath, BindingKind, FixupFacts, FunctionId, PathSegment};
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, function_path_location, stmts_snippet,
};
use crate::rust_ast::{FnDef, IndentStmt, Item, Program};

pub(in crate::fixups) fn fixup(program: &mut Program, facts: &FixupFacts) -> bool {
    let mut logger = crate::fixups::trace::NoopLogger;
    VaList::new(&mut logger).fixup(program, facts)
}

pub(in crate::fixups) struct VaList<'a> {
    logger: &'a mut dyn TraceLogger,
}

impl<'a> VaList<'a> {
    pub(in crate::fixups) fn new(logger: &'a mut dyn TraceLogger) -> Self {
        Self { logger }
    }

    pub(in crate::fixups) fn fixup(&mut self, program: &mut Program, facts: &FixupFacts) -> bool {
        let mut changed = false;
        for (item_index, item) in program.items.iter_mut().enumerate() {
            let Item::Fn(f) = item else {
                continue;
            };
            let Some(function) = facts.function_by_item_index(item_index) else {
                continue;
            };
            let before = self.logger.is_enabled().then(|| f.body.clone());
            let function_changed = fixup_fn(f, function, facts);
            if function_changed && let Some(before) = before {
                self.logger.rewrite(RewriteEvent {
                    pass: TracePass::VaList,
                    kind: "remove_va_list_clone_alias".into(),
                    location: function_path_location(facts, function, &[]),
                    before: vec![stmts_snippet("body", &before)],
                    after: vec![stmts_snippet("body", &f.body)],
                    facts: Vec::new(),
                });
            }
            changed |= function_changed;
        }
        changed
    }
}

fn fixup_impl(program: &mut Program, facts: &FixupFacts) -> bool {
    let mut changed = false;
    for (item_index, item) in program.items.iter_mut().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        changed |= fixup_fn(f, function, facts);
    }
    changed
}

fn fixup_fn(f: &mut FnDef, function: FunctionId, facts: &FixupFacts) -> bool {
    let aliases = facts
        .va_list_aliases
        .iter()
        .filter(|alias| alias.function == function)
        .collect::<Vec<_>>();
    let [alias] = aliases.as_slice() else {
        return false;
    };
    let Some(param_index) = facts.bindings.iter().find_map(|binding| {
        (binding.id == alias.param)
            .then_some(binding.kind.clone())
            .and_then(|kind| match kind {
                BindingKind::Param { index } => Some(index),
                BindingKind::Local => None,
            })
    }) else {
        return false;
    };
    let Some(local_name) = facts.binding_name(alias.local) else {
        return false;
    };
    let Some(param) = f.params.get_mut(param_index) else {
        return false;
    };

    if remove_alias_stmts(
        &mut f.body,
        &alias.local_decl_path,
        &alias.clone_assign_path,
        &mut Vec::new(),
    ) != 2
    {
        return false;
    }
    param.name = local_name.to_string();
    true
}

fn remove_alias_stmts(
    body: &mut Vec<IndentStmt>,
    local_decl_path: &AstPath,
    clone_assign_path: &AstPath,
    path: &mut Vec<PathSegment>,
) -> usize {
    let mut removed = 0;
    for index in (0..body.len()).rev() {
        let mut stmt_path = path.to_vec();
        stmt_path.push(PathSegment::Stmt(index));
        let stmt_path = AstPath(stmt_path);
        if local_decl_path == &stmt_path || clone_assign_path == &stmt_path {
            body.remove(index);
            removed += 1;
        }
    }

    if removed > 0 {
        return removed;
    }

    for (index, indent) in body.iter_mut().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            walk::nested_body_vecs_mut_with_path(&mut indent.stmt, path, &mut |body, path| {
                removed += remove_alias_stmts(body, local_decl_path, clone_assign_path, path);
            });
        });
        if removed > 0 {
            break;
        }
    }
    removed
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::facts;
    use crate::fixups::test_support::{assign, func, var};
    use crate::rust_ast::{Expr, FnParam, Item, Program, RustValue, Stmt, Type};

    fn variadic_param() -> FnParam {
        FnParam {
            name: "__slate_va_args".into(),
            mutable: true,
            ty: Type::Variadic,
            nonnull: false,
        }
    }

    fn va_list_decl(name: &str) -> Stmt {
        Stmt::Let {
            name: name.into(),
            mutable: true,
            ty: Some(Type::VaList),
            init: None,
        }
    }

    fn clone_hidden_args() -> Expr {
        Expr::MethodCall {
            recv: Box::new(var("__slate_va_args")),
            method: "clone".into(),
            args: vec![],
        }
    }

    fn next_arg(name: &str) -> Expr {
        Expr::Unsafe(Box::new(crate::rust_ast::Block {
            stmts: Vec::new(),
            tail: Some(Box::new(Expr::MethodCallGeneric {
                recv: Box::new(var(name)),
                method: "next_arg".into(),
                type_args: vec![Type::parse("i32")],
                args: vec![],
            })),
        }))
    }

    #[test]
    fn removes_simple_va_list_clone_alias() {
        let mut program = Program {
            items: vec![Item::Fn(func(
                vec![variadic_param()],
                Some("i32"),
                vec![
                    va_list_decl("ap"),
                    assign("ap", clone_hidden_args()),
                    Stmt::Let {
                        name: "value".into(),
                        mutable: false,
                        ty: Some(Type::parse("i32")),
                        init: Some(next_arg("ap")),
                    },
                    Stmt::Return(Some(var("value"))),
                ],
            ))],
        };
        let facts = facts::analyze(program.clone()).facts;

        assert!(fixup(&mut program, &facts));
        let out = program.emit();

        assert!(out.contains("mut ap: ..."));
        assert!(!out.contains("core::ffi::VaList"));
        assert!(!out.contains("__slate_va_args.clone()"));
        assert!(out.contains("ap.next_arg::<i32>()"));
    }

    #[test]
    fn keeps_clone_when_hidden_args_feed_multiple_cursors() {
        let mut program = Program {
            items: vec![Item::Fn(func(
                vec![variadic_param()],
                Some("i32"),
                vec![
                    va_list_decl("ap"),
                    assign("ap", clone_hidden_args()),
                    va_list_decl("copy"),
                    assign("copy", clone_hidden_args()),
                    Stmt::Return(Some(Expr::Value(RustValue::I64(0)))),
                ],
            ))],
        };
        let facts = facts::analyze(program.clone()).facts;

        assert!(!fixup(&mut program, &facts));
        let out = program.emit();

        assert!(out.contains("mut __slate_va_args: ..."));
        assert!(out.contains("__slate_va_args.clone()"));
    }

    #[test]
    fn keeps_clone_when_va_list_is_reassigned() {
        let mut program = Program {
            items: vec![Item::Fn(func(
                vec![variadic_param()],
                Some("i32"),
                vec![
                    va_list_decl("ap"),
                    assign("ap", clone_hidden_args()),
                    assign("ap", clone_hidden_args()),
                    Stmt::Return(Some(Expr::Value(RustValue::I64(0)))),
                ],
            ))],
        };
        let facts = facts::analyze(program.clone()).facts;

        assert!(!fixup(&mut program, &facts));
    }
}
