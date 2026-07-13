use std::collections::BTreeMap;

use crate::fixups::facts::{
    AstPath, BindingId, BindingKind, DefUseFact, FixupFacts, FunctionId, PathSegment,
};
use crate::fixups::support::walk;
use crate::rust_ast::{Block, Expr, Ident, IndentStmt, Item, Pattern, Program, Stmt, UnaryOp};

pub(in crate::fixups) fn collect_facts(program: &Program, facts: &mut FixupFacts) {
    facts.def_use.clear();
    let mut all = Vec::new();
    for (item_index, item) in program.items.iter().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        let mut collector = Collector::new(function, facts);
        collector.enter_root_scope();
        collector.body(&f.body, &mut Vec::new(), false);
        all.extend(collector.finish());
    }
    facts.def_use = all;
}

struct Collector<'a> {
    function: FunctionId,
    facts: &'a FixupFacts,
    scopes: Vec<BTreeMap<String, Option<BindingId>>>,
    by_binding: BTreeMap<BindingId, BindingSummary>,
}

#[derive(Default)]
struct BindingSummary {
    reads: Vec<AstPath>,
    writes: Vec<AstPath>,
}

impl<'a> Collector<'a> {
    fn new(function: FunctionId, facts: &'a FixupFacts) -> Self {
        Self {
            function,
            facts,
            scopes: Vec::new(),
            by_binding: BTreeMap::new(),
        }
    }

    fn enter_root_scope(&mut self) {
        self.scopes.push(BTreeMap::new());
        let params: Vec<_> = self
            .facts
            .bindings
            .iter()
            .filter(|binding| binding.function == self.function)
            .filter_map(|binding| match binding.kind {
                BindingKind::Param { .. } => Some((binding.name.clone(), binding.id)),
                BindingKind::Local => None,
            })
            .collect();
        for (name, id) in params {
            self.bind(name, Some(id));
        }
    }

    fn finish(self) -> Vec<DefUseFact> {
        self.facts
            .bindings
            .iter()
            .filter(|binding| binding.function == self.function)
            .map(|binding| {
                let summary = self.by_binding.get(&binding.id);
                let reads = summary
                    .map(|summary| summary.reads.clone())
                    .unwrap_or_default();
                let writes = summary
                    .map(|summary| summary.writes.clone())
                    .unwrap_or_default();
                DefUseFact {
                    function: self.function,
                    binding: binding.id,
                    definition: binding.path.clone(),
                    last_use: reads.last().cloned(),
                    reads,
                    writes,
                }
            })
            .collect()
    }

    fn body(&mut self, body: &[IndentStmt], path: &mut Vec<PathSegment>, scoped: bool) {
        if scoped {
            self.scopes.push(BTreeMap::new());
        }
        for (index, indent) in body.iter().enumerate() {
            walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
                self.stmt(&indent.stmt, path);
            });
        }
        if scoped {
            self.scopes.pop();
        }
    }

    fn block(&mut self, block: &Block, path: &mut Vec<PathSegment>) {
        self.scopes.push(BTreeMap::new());
        self.body(&block.stmts, path, false);
        if let Some(tail) = &block.tail {
            walk::with_path_segment(path, PathSegment::BlockTail, |path| {
                self.expr(tail, path);
            });
        }
        self.scopes.pop();
    }

    fn stmt(&mut self, stmt: &Stmt, path: &mut Vec<PathSegment>) {
        match stmt {
            Stmt::Let { name, init, .. } => {
                if let Some(init) = init {
                    self.expr(init, path);
                }
                self.define_local(name, path);
            }
            Stmt::LetIf {
                name,
                cond,
                then_body,
                then_value,
                else_body,
                else_value,
                ..
            } => {
                self.expr(cond, path);
                walk::with_path_segment(path, PathSegment::Then, |path| {
                    self.scopes.push(BTreeMap::new());
                    self.body(then_body, path, false);
                    self.expr(then_value, path);
                    self.scopes.pop();
                });
                walk::with_path_segment(path, PathSegment::Else, |path| {
                    self.scopes.push(BTreeMap::new());
                    self.body(else_body, path, false);
                    self.expr(else_value, path);
                    self.scopes.pop();
                });
                self.define_local(name, path);
            }
            Stmt::Assign { target, value } => {
                self.place(target, PlaceAccess::Write, path);
                self.expr(value, path);
            }
            Stmt::CompoundAssign { target, value, .. } => {
                self.place(target, PlaceAccess::ReadWrite, path);
                self.expr(value, path);
            }
            Stmt::Expr(expr) | Stmt::Return(Some(expr)) => self.expr(expr, path),
            Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
            Stmt::If {
                cond,
                then_body,
                else_body,
            } => {
                self.expr(cond, path);
                walk::with_path_segment(path, PathSegment::Then, |path| {
                    self.body(then_body, path, true)
                });
                walk::with_path_segment(path, PathSegment::Else, |path| {
                    self.body(else_body, path, true)
                });
            }
            Stmt::Loop { body, .. } => {
                walk::with_path_segment(path, PathSegment::LoopBody, |path| {
                    self.body(body, path, true)
                });
            }
            Stmt::Scope { body } => {
                walk::with_path_segment(path, PathSegment::ScopeBody, |path| {
                    self.body(body, path, true)
                });
            }
            Stmt::LabeledBlock { body, .. } => {
                walk::with_path_segment(path, PathSegment::LabeledBody, |path| {
                    self.body(body, path, true)
                });
            }
            Stmt::Unsafe { body } => {
                walk::with_path_segment(path, PathSegment::UnsafeBody, |path| {
                    self.block(body, path)
                });
            }
            Stmt::While { cond, body } => {
                self.expr(cond, path);
                walk::with_path_segment(path, PathSegment::WhileBody, |path| {
                    self.block(body, path)
                });
            }
            Stmt::Block(body) => {
                walk::with_path_segment(path, PathSegment::BlockBody, |path| {
                    self.block(body, path)
                });
            }
            Stmt::Match { expr, arms } => {
                self.expr(expr, path);
                for (index, arm) in arms.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::MatchArm(index), |path| {
                        self.scopes.push(BTreeMap::new());
                        self.shadow_pattern(&arm.pattern);
                        self.body(&arm.body, path, false);
                        self.scopes.pop();
                    });
                }
            }
        }
    }

    fn expr(&mut self, expr: &Expr, path: &mut Vec<PathSegment>) {
        match expr {
            Expr::Var(name) => self.record_read(name, path),
            Expr::Unary { expr, .. } | Expr::Cast { expr, .. } | Expr::Transmute { expr, .. } => {
                self.expr(expr, path)
            }
            Expr::Binary { lhs, rhs, .. } => {
                self.expr(lhs, path);
                self.expr(rhs, path);
            }
            Expr::Call { func, args } => {
                self.expr(func, path);
                for arg in args {
                    self.expr(arg, path);
                }
            }
            Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
                self.expr(recv, path);
                for arg in args {
                    self.expr(arg, path);
                }
            }
            Expr::Field { base, .. } | Expr::TupleField { base, .. } => self.expr(base, path),
            Expr::ArrayPtr { array, .. } => self.expr(array, path),
            Expr::Index { base, index } => {
                self.expr(base, path);
                self.expr(index, path);
            }
            Expr::StructLit { fields, .. } => {
                for (_, value) in fields {
                    self.expr(value, path);
                }
            }
            Expr::ArrayLit(elems) | Expr::Macro { args: elems, .. } => {
                for elem in elems {
                    self.expr(elem, path);
                }
            }
            Expr::ArrayRepeat { elem, .. } => self.expr(elem, path),
            Expr::Closure { params, body } => {
                self.scopes.push(BTreeMap::new());
                for param in params {
                    self.bind(param.to_string(), None);
                }
                self.expr(body, path);
                self.scopes.pop();
            }
            Expr::Match { expr, arms } => {
                self.expr(expr, path);
                for arm in arms {
                    self.scopes.push(BTreeMap::new());
                    self.shadow_pattern(&arm.pattern);
                    self.expr(&arm.value, path);
                    self.scopes.pop();
                }
            }
            Expr::If {
                cond,
                then_expr,
                else_expr,
            } => {
                self.expr(cond, path);
                self.expr(then_expr, path);
                self.expr(else_expr, path);
            }
            Expr::Block(block) => {
                walk::with_path_segment(path, PathSegment::BlockBody, |path| {
                    self.block(block, path)
                });
            }
            Expr::Unsafe(block) => {
                walk::with_path_segment(path, PathSegment::UnsafeBody, |path| {
                    self.block(block, path)
                });
            }
            Expr::Ref { expr, .. } | Expr::AddrOf { expr, .. } => self.expr(expr, path),
            Expr::AtomicRef { ptr, .. } | Expr::AtomicLoad { ptr, .. } => self.expr(ptr, path),
            Expr::AtomicStore { ptr, value, .. }
            | Expr::AtomicFetch { ptr, value, .. }
            | Expr::AtomicSwap { ptr, value, .. } => {
                self.expr(ptr, path);
                self.expr(value, path);
            }
            Expr::AtomicCompareExchange {
                ptr,
                expected,
                desired,
                ..
            } => {
                self.expr(ptr, path);
                self.expr(expected, path);
                self.expr(desired, path);
            }
            Expr::CopyNonoverlapping { src, dst, .. } => {
                self.expr(src, path);
                self.expr(dst, path);
            }
            Expr::PtrCopy {
                src, dst, count, ..
            } => {
                self.expr(src, path);
                self.expr(dst, path);
                self.expr(count, path);
            }
            Expr::WriteBytes { dst, val, count } => {
                self.expr(dst, path);
                self.expr(val, path);
                self.expr(count, path);
            }
            Expr::Value(_)
            | Expr::Str(_)
            | Expr::HexFloat(_)
            | Expr::ByteStr(_)
            | Expr::AtomicFence { .. }
            | Expr::Todo(_)
            | Expr::Path(_) => {}
        }
    }

    fn place(&mut self, expr: &Expr, access: PlaceAccess, path: &mut Vec<PathSegment>) {
        match expr {
            Expr::Var(name) => match access {
                PlaceAccess::Write => self.record_write(name, path),
                PlaceAccess::ReadWrite => {
                    self.record_read(name, path);
                    self.record_write(name, path);
                }
            },
            Expr::Field { base, .. } | Expr::TupleField { base, .. } => {
                self.place(base, access, path)
            }
            Expr::Index { base, index } => {
                self.place(base, access, path);
                self.expr(index, path);
            }
            Expr::Unary {
                op: UnaryOp::Deref,
                expr,
            } => self.expr(expr, path),
            Expr::Ref { expr, .. } | Expr::AddrOf { expr, .. } | Expr::Cast { expr, .. } => {
                self.place(expr, access, path);
            }
            _ => self.expr(expr, path),
        }
    }

    fn define_local(&mut self, name: &str, path: &[PathSegment]) {
        let Some(id) =
            self.facts
                .binding_by_local_path(self.function, name, &AstPath(path.to_vec()))
        else {
            return;
        };
        self.bind(name.to_string(), Some(id));
    }

    fn shadow_pattern(&mut self, pattern: &Pattern) {
        match pattern {
            Pattern::Binding(name) => self.bind(name.to_string(), None),
            Pattern::TupleStruct { fields, .. } => {
                for field in fields {
                    self.shadow_pattern(field);
                }
            }
            Pattern::Wildcard | Pattern::I64(_) | Pattern::I128(_) => {}
        }
    }

    fn bind(&mut self, name: String, binding: Option<BindingId>) {
        if let Some(scope) = self.scopes.last_mut() {
            scope.insert(name, binding);
        }
    }

    fn record_read(&mut self, name: &Ident, path: &[PathSegment]) {
        let Some(binding) = self.binding_for_name(name.as_str()) else {
            return;
        };
        self.by_binding
            .entry(binding)
            .or_default()
            .reads
            .push(AstPath(path.to_vec()));
    }

    fn record_write(&mut self, name: &Ident, path: &[PathSegment]) {
        let Some(binding) = self.binding_for_name(name.as_str()) else {
            return;
        };
        self.by_binding
            .entry(binding)
            .or_default()
            .writes
            .push(AstPath(path.to_vec()));
    }

    fn binding_for_name(&self, name: &str) -> Option<BindingId> {
        self.scopes
            .iter()
            .rev()
            .find_map(|scope| scope.get(name).copied())
            .flatten()
    }
}

#[derive(Clone, Copy)]
enum PlaceAccess {
    Write,
    ReadWrite,
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::facts;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{BinOp, Expr, Item, Program, Stmt};

    fn analyzed(stmts: Vec<Stmt>) -> facts::FixupFacts {
        facts::analyze(Program {
            items: vec![Item::Fn(func(vec![param("p", "i32")], None, stmts))],
        })
        .facts
    }

    fn binding_for(facts: &facts::FixupFacts, name: &str, path: AstPath) -> BindingId {
        facts
            .bindings
            .iter()
            .find(|binding| binding.name == name && binding.path == path)
            .unwrap()
            .id
    }

    fn fact_for(facts: &facts::FixupFacts, binding: BindingId) -> &DefUseFact {
        facts
            .def_use
            .iter()
            .find(|fact| fact.binding == binding)
            .unwrap()
    }

    #[test]
    fn records_straight_line_reads_writes_and_last_use() {
        let facts = analyzed(vec![
            let_mut("x", "i32", var("p")),
            assign("x", int(2)),
            temp("y", "i32", var("x")),
        ]);
        let x = binding_for(&facts, "x", AstPath(vec![PathSegment::Stmt(0)]));
        let fact = fact_for(&facts, x);

        assert_eq!(fact.definition, AstPath(vec![PathSegment::Stmt(0)]));
        assert_eq!(fact.reads, vec![AstPath(vec![PathSegment::Stmt(2)])]);
        assert_eq!(fact.writes, vec![AstPath(vec![PathSegment::Stmt(1)])]);
        assert_eq!(fact.last_use, Some(AstPath(vec![PathSegment::Stmt(2)])));
    }

    #[test]
    fn assignment_target_write_is_distinct_from_value_read() {
        let facts = analyzed(vec![let_mut("x", "i32", int(0)), assign("x", var("x"))]);
        let x = binding_for(&facts, "x", AstPath(vec![PathSegment::Stmt(0)]));
        let fact = fact_for(&facts, x);

        assert_eq!(fact.reads, vec![AstPath(vec![PathSegment::Stmt(1)])]);
        assert_eq!(fact.writes, vec![AstPath(vec![PathSegment::Stmt(1)])]);
    }

    #[test]
    fn compound_assignment_reads_and_writes_target() {
        let facts = analyzed(vec![
            let_mut("x", "i32", int(0)),
            Stmt::CompoundAssign {
                target: var("x"),
                op: BinOp::Add,
                value: int(1),
            },
        ]);
        let x = binding_for(&facts, "x", AstPath(vec![PathSegment::Stmt(0)]));
        let fact = fact_for(&facts, x);

        assert_eq!(fact.reads, vec![AstPath(vec![PathSegment::Stmt(1)])]);
        assert_eq!(fact.writes, vec![AstPath(vec![PathSegment::Stmt(1)])]);
    }

    #[test]
    fn branches_and_loops_keep_nested_paths() {
        let facts = analyzed(vec![
            let_mut("x", "i32", int(0)),
            Stmt::If {
                cond: var("p"),
                then_body: vec![crate::rust_ast::IndentStmt {
                    depth: 2,
                    stmt: temp("a", "i32", var("x")),
                }],
                else_body: vec![crate::rust_ast::IndentStmt {
                    depth: 2,
                    stmt: temp("b", "i32", var("x")),
                }],
            },
            Stmt::Loop {
                label: None,
                body: vec![crate::rust_ast::IndentStmt {
                    depth: 2,
                    stmt: Stmt::Expr(var("x")),
                }],
            },
        ]);
        let x = binding_for(&facts, "x", AstPath(vec![PathSegment::Stmt(0)]));
        let fact = fact_for(&facts, x);

        assert_eq!(
            fact.reads,
            vec![
                AstPath(vec![
                    PathSegment::Stmt(1),
                    PathSegment::Then,
                    PathSegment::Stmt(0)
                ]),
                AstPath(vec![
                    PathSegment::Stmt(1),
                    PathSegment::Else,
                    PathSegment::Stmt(0)
                ]),
                AstPath(vec![
                    PathSegment::Stmt(2),
                    PathSegment::LoopBody,
                    PathSegment::Stmt(0)
                ]),
            ]
        );
    }

    #[test]
    fn block_tail_reads_are_represented() {
        let facts = analyzed(vec![Stmt::Block(crate::rust_ast::Block {
            stmts: vec![crate::rust_ast::IndentStmt {
                depth: 2,
                stmt: temp("inner", "i32", var("p")),
            }],
            tail: Some(Box::new(var("inner"))),
        })]);
        let inner = binding_for(
            &facts,
            "inner",
            AstPath(vec![
                PathSegment::Stmt(0),
                PathSegment::BlockBody,
                PathSegment::Stmt(0),
            ]),
        );
        let fact = fact_for(&facts, inner);

        assert_eq!(
            fact.last_use,
            Some(AstPath(vec![
                PathSegment::Stmt(0),
                PathSegment::BlockBody,
                PathSegment::BlockTail
            ]))
        );
    }

    #[test]
    fn expression_block_tails_are_represented_for_visible_bindings() {
        let facts = analyzed(vec![
            let_mut("x", "i32", int(0)),
            Stmt::Expr(Expr::Block(Box::new(crate::rust_ast::Block {
                stmts: vec![crate::rust_ast::IndentStmt {
                    depth: 2,
                    stmt: temp("ignored", "i32", var("p")),
                }],
                tail: Some(Box::new(var("x"))),
            }))),
        ]);
        let x = binding_for(&facts, "x", AstPath(vec![PathSegment::Stmt(0)]));
        let fact = fact_for(&facts, x);

        assert_eq!(
            fact.last_use,
            Some(AstPath(vec![
                PathSegment::Stmt(1),
                PathSegment::BlockBody,
                PathSegment::BlockTail
            ]))
        );
    }

    #[test]
    fn shadowed_locals_resolve_to_the_visible_binding() {
        let facts = analyzed(vec![
            let_mut("x", "i32", int(1)),
            Stmt::Scope {
                body: vec![
                    crate::rust_ast::IndentStmt {
                        depth: 2,
                        stmt: temp("before", "i32", var("x")),
                    },
                    crate::rust_ast::IndentStmt {
                        depth: 2,
                        stmt: let_mut("x", "i32", int(2)),
                    },
                    crate::rust_ast::IndentStmt {
                        depth: 2,
                        stmt: temp("after", "i32", var("x")),
                    },
                ],
            },
            temp("final", "i32", var("x")),
        ]);
        let outer = binding_for(&facts, "x", AstPath(vec![PathSegment::Stmt(0)]));
        let inner = binding_for(
            &facts,
            "x",
            AstPath(vec![
                PathSegment::Stmt(1),
                PathSegment::ScopeBody,
                PathSegment::Stmt(1),
            ]),
        );

        assert_eq!(
            fact_for(&facts, outer).reads,
            vec![
                AstPath(vec![
                    PathSegment::Stmt(1),
                    PathSegment::ScopeBody,
                    PathSegment::Stmt(0)
                ]),
                AstPath(vec![PathSegment::Stmt(2)]),
            ]
        );
        assert_eq!(
            fact_for(&facts, inner).reads,
            vec![AstPath(vec![
                PathSegment::Stmt(1),
                PathSegment::ScopeBody,
                PathSegment::Stmt(2)
            ])]
        );
    }
}
