//! Rewrite a proven lazy-init singleton (see `facts/lazy_singleton.rs`) into
//! a safe `OnceLock::get_or_init` call: the payload static becomes
//! `std::sync::OnceLock<T>`, the function body collapses to a single
//! `return`, and the guard flag static — provably dead once its only reader
//! and writer are gone — is dropped outright (`unused_items` only prunes
//! unused types, not statics).

use crate::fixups::facts::FixupFacts;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLocation, TraceLogger, TraceSnippet, fact,
};
use crate::rust_ast::{Expr, IndentStmt, Item, Program, Stmt, Type, UnaryOp};
use std::collections::BTreeSet;

pub(in crate::fixups) fn fixup(program: &mut Program, facts: &FixupFacts) -> bool {
    let mut logger = crate::fixups::trace::NoopLogger;
    LazySingleton::new(&mut logger).fixup(program, facts)
}

pub(in crate::fixups) struct LazySingleton<'a> {
    logger: &'a mut dyn TraceLogger,
}

impl<'a> LazySingleton<'a> {
    pub(in crate::fixups) fn new(logger: &'a mut dyn TraceLogger) -> Self {
        Self { logger }
    }

    pub(in crate::fixups) fn fixup(&mut self, program: &mut Program, facts: &FixupFacts) -> bool {
        let before = self.logger.is_enabled().then(|| program.emit());
        let changed = fixup_impl(program, facts);
        if changed && let Some(before) = before {
            self.logger.rewrite(RewriteEvent {
                pass: TracePass::LazySingleton,
                kind: "rewrite_lazy_singletons".into(),
                location: TraceLocation::default(),
                before: vec![TraceSnippet::new("program", before.trim_end())],
                after: vec![TraceSnippet::new("program", program.emit().trim_end())],
                facts: vec![fact(
                    "lazy_singletons",
                    facts.lazy_init_singletons.len().to_string(),
                )],
            });
        }
        changed
    }
}

fn fixup_impl(program: &mut Program, facts: &FixupFacts) -> bool {
    let mut changed = false;
    let flag_names: BTreeSet<&str> = facts
        .lazy_init_singletons
        .iter()
        .map(|fact| fact.flag_name.as_str())
        .collect();
    for fact in &facts.lazy_init_singletons {
        for item in program.items.iter_mut() {
            if let Item::Static {
                mutable,
                name,
                ty,
                init,
                ..
            } = item
                && *name == fact.payload_name
            {
                *mutable = false;
                *ty = once_lock_type(fact.payload_ty.clone());
                *init = once_lock_new();
                changed = true;
            }
        }
        let Some(item_index) = facts.function_item_index(fact.function) else {
            continue;
        };
        if let Some(Item::Fn(f)) = program.items.get_mut(item_index) {
            f.body = vec![IndentStmt {
                depth: 1,
                stmt: Stmt::Return(Some(get_or_init_deref(
                    &fact.payload_name,
                    fact.init_expr.clone(),
                ))),
            }];
            changed = true;
        }
    }
    if !flag_names.is_empty() {
        let before = program.items.len();
        program.items.retain(
            |item| !matches!(item, Item::Static { name, .. } if flag_names.contains(name.as_str())),
        );
        changed |= program.items.len() != before;
    }
    changed
}

fn once_lock_type(payload_ty: Type) -> Type {
    Type::Generic {
        name: "std::sync::OnceLock".to_string(),
        args: vec![payload_ty],
    }
}

fn once_lock_new() -> Expr {
    Expr::Call {
        binding: crate::function_identity::CallBinding::Generated,
        func: Box::new(Expr::Var("std::sync::OnceLock::new".into())),
        args: Vec::new(),
    }
}

fn get_or_init_deref(payload_name: &str, init_expr: Expr) -> Expr {
    Expr::Unary {
        op: UnaryOp::Deref,
        expr: Box::new(Expr::MethodCall {
            recv: Box::new(Expr::Var(payload_name.into())),
            method: "get_or_init".to_string(),
            args: vec![Expr::Closure {
                params: Vec::new(),
                body: Box::new(init_expr),
            }],
        }),
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::facts;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{BinOp, Block, IndentStmt, UnaryOp as U, Visibility};

    fn static_item(name: &str, ty: &str, init: Expr) -> Item {
        Item::Static {
            attrs: Vec::new(),
            vis: Visibility::Private,
            mutable: true,
            name: name.into(),
            ty: Type::parse(ty),
            init,
        }
    }

    fn unsafe_assign_stmt(name: &str, value: Expr) -> Stmt {
        Stmt::Unsafe {
            body: Block {
                stmts: vec![IndentStmt {
                    depth: 0,
                    stmt: Stmt::Assign {
                        target: var(name),
                        value,
                    },
                }],
                tail: None,
            },
        }
    }

    fn unsafe_read(name: &str) -> Expr {
        Expr::Unsafe(Box::new(Block {
            stmts: Vec::new(),
            tail: Some(Box::new(var(name))),
        }))
    }

    fn qualifying_program() -> Program {
        Program {
            items: vec![
                static_item("cached_value", "i32", int(0)),
                static_item("computed", "i32", int(0)),
                Item::Fn({
                    let mut f = func(vec![], Some("i32"), Vec::new());
                    f.name = "get_value".into();
                    f.body = vec![
                        IndentStmt {
                            depth: 1,
                            stmt: Stmt::Scope {
                                body: vec![
                                    IndentStmt {
                                        depth: 2,
                                        stmt: temp("_v0", "i32", unsafe_read("computed")),
                                    },
                                    IndentStmt {
                                        depth: 2,
                                        stmt: Stmt::Let {
                                            name: "_v1".to_string(),
                                            mutable: false,
                                            ty: Some(Type::parse("bool")),
                                            init: Some(Expr::Unary {
                                                op: U::Not,
                                                expr: Box::new(bin(BinOp::Ne, var("_v0"), int(0))),
                                            }),
                                        },
                                    },
                                    IndentStmt {
                                        depth: 2,
                                        stmt: Stmt::If {
                                            cond: var("_v1"),
                                            then_body: vec![
                                                IndentStmt {
                                                    depth: 3,
                                                    stmt: temp(
                                                        "_v2",
                                                        "i32",
                                                        call("compute_value", vec![]),
                                                    ),
                                                },
                                                IndentStmt {
                                                    depth: 3,
                                                    stmt: unsafe_assign_stmt(
                                                        "cached_value",
                                                        var("_v2"),
                                                    ),
                                                },
                                                IndentStmt {
                                                    depth: 3,
                                                    stmt: unsafe_assign_stmt("computed", int(1)),
                                                },
                                            ],
                                            else_body: Vec::new(),
                                        },
                                    },
                                ],
                            },
                        },
                        IndentStmt {
                            depth: 1,
                            stmt: Stmt::Return(Some(unsafe_read("cached_value"))),
                        },
                    ];
                    f
                }),
            ],
        }
    }

    #[test]
    fn recovers_once_lock_get_or_init() {
        let mut program = qualifying_program();
        let analyzed = facts::analyze(program.clone());
        assert_eq!(analyzed.facts.lazy_init_singletons.len(), 1);
        let changed = fixup(&mut program, &analyzed.facts);
        assert!(changed);
        let out = program.emit();
        assert!(
            out.contains(
                "static cached_value: std::sync::OnceLock<i32> = std::sync::OnceLock::new();"
            ),
            "{out}"
        );
        assert!(
            out.contains("*cached_value.get_or_init(|| compute_value())"),
            "{out}"
        );
        assert!(!out.contains("static mut cached_value"), "{out}");
    }

    #[test]
    fn leaves_non_matching_program_untouched() {
        let mut program = qualifying_program();
        program.items.push(Item::Fn({
            let mut f = func(
                vec![],
                Some("i32"),
                vec![Stmt::Return(Some(unsafe_read("cached_value")))],
            );
            f.name = "peek_value".into();
            f
        }));
        let analyzed = facts::analyze(program.clone());
        assert!(analyzed.facts.lazy_init_singletons.is_empty());
        let changed = fixup(&mut program, &analyzed.facts);
        assert!(!changed);
    }
}
