use std::collections::BTreeMap;

use crate::fixups::facts::walk;
use crate::fixups::facts::{
    self, AstPath, BindingFact, ControlFlowFact, ControlFlowSubject, FunctionId,
    NullCheckDominanceFact, NullCheckProof, PathSegment, Site,
};
use crate::rust_ast::{BinOp, Block, Expr, FnDef, IndentStmt, RustValue, Stmt, UnaryOp};
pub(in crate::fixups) fn collect_for_function(
    function: FunctionId,
    f: &FnDef,
    bindings: &[BindingFact],
    control_flow: &[ControlFlowFact],
) -> Vec<NullCheckDominanceFact> {
    let mut collector = Collector {
        function,
        bindings,
        control_flow,
        bool_defs: BTreeMap::new(),
        results: Vec::new(),
    };
    let mut proven = Proven::default();
    collector.body(&f.body, &mut Vec::new(), &mut proven);
    collector.results
}

#[derive(Default, Clone)]
struct Proven {
    entries: BTreeMap<String, ProvenEntry>,
}

#[derive(Clone)]
struct ProvenEntry {
    guard_site: Option<Site>,
    proof: NullCheckProof,
}

struct Collector<'a> {
    function: FunctionId,
    bindings: &'a [BindingFact],
    control_flow: &'a [ControlFlowFact],
    bool_defs: BTreeMap<String, Expr>,
    results: Vec<NullCheckDominanceFact>,
}

impl<'a> Collector<'a> {
    fn body(&mut self, body: &[IndentStmt], path: &mut Vec<PathSegment>, proven: &mut Proven) {
        for (index, indent) in body.iter().enumerate() {
            walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
                self.stmt(&indent.stmt, path, proven);
            });
        }
    }

    fn block(&mut self, block: &Block, path: &mut Vec<PathSegment>, proven: &mut Proven) {
        self.body(&block.stmts, path, proven);
        if let Some(tail) = &block.tail {
            self.expr(tail, path, proven);
        }
    }

    fn stmt(&mut self, stmt: &Stmt, path: &mut Vec<PathSegment>, proven: &mut Proven) {
        match stmt {
            Stmt::Let { name, init, .. } => {
                if let Some(init) = init {
                    walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                        self.expr(init, path, proven);
                    });
                    self.bool_defs.insert(name.clone(), init.clone());
                    self.update_construction_proof(name.as_str(), init, proven);
                } else {
                    proven.entries.remove(name.as_str());
                }
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
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(cond, path, proven);
                });
                let mut then_proven = proven.clone();
                walk::with_path_segment(path, PathSegment::Then, |path| {
                    self.body(then_body, path, &mut then_proven);
                    walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                        self.expr(then_value, path, &then_proven);
                    });
                });
                let mut else_proven = proven.clone();
                walk::with_path_segment(path, PathSegment::Else, |path| {
                    self.body(else_body, path, &mut else_proven);
                    walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                        self.expr(else_value, path, &else_proven);
                    });
                });
                proven.entries.remove(name.as_str());
            }
            Stmt::Assign { target, value } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(target, path, proven);
                });
                walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                    self.expr(value, path, proven);
                });
                if let Expr::Var(name) = target {
                    self.update_construction_proof(name.as_str(), value, proven);
                }
            }
            Stmt::CompoundAssign { target, value, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(target, path, proven);
                });
                walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                    self.expr(value, path, proven);
                });
            }
            Stmt::InlineAsm(_) => {}
            Stmt::Expr(expr) | Stmt::Return(Some(expr)) => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(expr, path, proven);
                });
            }
            Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => {}
            Stmt::If {
                cond,
                then_body,
                else_body,
            } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(cond, path, proven);
                });
                self.if_stmt(cond, then_body, else_body, path, proven);
            }
            Stmt::Loop { body, .. } => {
                let mut inner = proven.clone();
                walk::with_path_segment(path, PathSegment::LoopBody, |path| {
                    self.body(body, path, &mut inner);
                });
            }
            Stmt::For { iter, body, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(iter, path, proven);
                });
                let mut inner = proven.clone();
                walk::with_path_segment(path, PathSegment::ForBody, |path| {
                    self.body(body, path, &mut inner);
                });
            }
            Stmt::Scope { body } => {
                walk::with_path_segment(path, PathSegment::ScopeBody, |path| {
                    self.body(body, path, proven);
                });
            }
            Stmt::LabeledBlock { body, .. } => {
                let mut inner = proven.clone();
                walk::with_path_segment(path, PathSegment::LabeledBody, |path| {
                    self.body(body, path, &mut inner);
                });
            }
            Stmt::Unsafe { body } => {
                walk::with_path_segment(path, PathSegment::UnsafeBody, |path| {
                    self.block(body, path, proven);
                });
            }
            Stmt::While { cond, body } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(cond, path, proven);
                });
                let mut inner = proven.clone();
                walk::with_path_segment(path, PathSegment::WhileBody, |path| {
                    self.block(body, path, &mut inner);
                });
            }
            Stmt::Block(body) => {
                walk::with_path_segment(path, PathSegment::BlockBody, |path| {
                    self.block(body, path, proven);
                });
            }
            Stmt::Match { expr, arms } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(expr, path, proven);
                });
                for (index, arm) in arms.iter().enumerate() {
                    let mut inner = proven.clone();
                    walk::with_path_segment(path, PathSegment::MatchArm(index), |path| {
                        self.body(&arm.body, path, &mut inner);
                    });
                }
            }
        }
    }

    fn if_stmt(
        &mut self,
        cond: &Expr,
        then_body: &[IndentStmt],
        else_body: &[IndentStmt],
        path: &mut Vec<PathSegment>,
        proven: &mut Proven,
    ) {
        let guard_site = Site {
            function: self.function,
            path: AstPath(path.clone()),
        };
        match null_check_shape(&self.bool_defs, cond) {
            Some((name, true)) => {
                let mut then_proven = proven.clone();
                then_proven.entries.insert(
                    name,
                    ProvenEntry {
                        guard_site: Some(guard_site),
                        proof: NullCheckProof::StructuredGuard,
                    },
                );
                walk::with_path_segment(path, PathSegment::Then, |path| {
                    self.body(then_body, path, &mut then_proven);
                });
                let mut else_proven = proven.clone();
                walk::with_path_segment(path, PathSegment::Else, |path| {
                    self.body(else_body, path, &mut else_proven);
                });
            }
            Some((name, false)) => {
                let then_path = {
                    let mut segments = path.clone();
                    segments.push(PathSegment::Then);
                    AstPath(segments)
                };
                let mut then_proven = proven.clone();
                walk::with_path_segment(path, PathSegment::Then, |path| {
                    self.body(then_body, path, &mut then_proven);
                });
                let mut else_proven = proven.clone();
                walk::with_path_segment(path, PathSegment::Else, |path| {
                    self.body(else_body, path, &mut else_proven);
                });
                if else_body.is_empty() && self.then_branch_always_exits(&then_path) {
                    proven.entries.insert(
                        name,
                        ProvenEntry {
                            guard_site: Some(guard_site),
                            proof: NullCheckProof::GuardClauseExit,
                        },
                    );
                }
            }
            None => {
                let mut then_proven = proven.clone();
                walk::with_path_segment(path, PathSegment::Then, |path| {
                    self.body(then_body, path, &mut then_proven);
                });
                let mut else_proven = proven.clone();
                walk::with_path_segment(path, PathSegment::Else, |path| {
                    self.body(else_body, path, &mut else_proven);
                });
            }
        }
    }

    fn then_branch_always_exits(&self, then_path: &AstPath) -> bool {
        self.control_flow.iter().any(|fact| {
            fact.site.function == self.function
                && &fact.site.path == then_path
                && fact.subject == ControlFlowSubject::Body
                && fact.reachable
                && !fact.falls_through
        })
    }

    fn update_construction_proof(&self, name: &str, rhs: &Expr, proven: &mut Proven) {
        if is_provably_nonnull_construction(rhs, proven) {
            proven.entries.insert(
                name.to_string(),
                ProvenEntry {
                    guard_site: None,
                    proof: NullCheckProof::ConstructionNonNull,
                },
            );
        } else {
            proven.entries.remove(name);
        }
    }

    fn record_deref(&mut self, name: &str, path: &[PathSegment], proven: &Proven) {
        let Some(binding) = facts::binding_named(self.bindings, self.function, name) else {
            return;
        };
        let Some(entry) = proven.entries.get(name) else {
            return;
        };
        self.results.push(NullCheckDominanceFact {
            function: self.function,
            binding,
            deref_site: Site {
                function: self.function,
                path: AstPath(path.to_vec()),
            },
            guard_site: entry.guard_site.clone(),
            proof: entry.proof,
        });
    }

    fn expr(&mut self, expr: &Expr, path: &mut Vec<PathSegment>, proven: &Proven) {
        if let Expr::Unary {
            op: UnaryOp::Deref,
            expr: inner,
        } = expr
            && let Expr::Var(name) = peel_casts(inner)
        {
            self.record_deref(name.as_str(), path, proven);
        }
        match expr {
            Expr::Unary { expr: inner, .. }
            | Expr::Cast { expr: inner, .. }
            | Expr::Ref { expr: inner, .. }
            | Expr::AddrOf { expr: inner, .. }
            | Expr::Transmute { expr: inner, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(inner, path, proven);
                });
            }
            Expr::Binary { lhs, rhs, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(lhs, path, proven);
                });
                walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                    self.expr(rhs, path, proven);
                });
            }
            Expr::Range { start, end } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(start, path, proven);
                });
                walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                    self.expr(end, path, proven);
                });
            }
            Expr::Call { func, args, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(func, path, proven);
                });
                for (index, arg) in args.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index + 1), |path| {
                        self.expr(arg, path, proven);
                    });
                }
            }
            Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(recv, path, proven);
                });
                for (index, arg) in args.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index + 1), |path| {
                        self.expr(arg, path, proven);
                    });
                }
            }
            Expr::Field { base, .. }
            | Expr::TupleField { base, .. }
            | Expr::ArrayPtr { array: base, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(base, path, proven);
                });
            }
            Expr::Index { base, index } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(base, path, proven);
                });
                walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                    self.expr(index, path, proven);
                });
            }
            Expr::StructLit { fields, .. } => {
                for (index, (_, value)) in fields.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index), |path| {
                        self.expr(value, path, proven);
                    });
                }
            }
            Expr::TupleStructLit { fields, .. } | Expr::ArrayLit(fields) => {
                for (index, value) in fields.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index), |path| {
                        self.expr(value, path, proven);
                    });
                }
            }
            Expr::ArrayRepeat { elem, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(elem, path, proven);
                });
            }
            Expr::Macro { args, .. } => {
                for (index, arg) in args.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::Expr(index), |path| {
                        self.expr(arg, path, proven);
                    });
                }
            }
            Expr::Closure { body, .. } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(body, path, &proven.clone());
                });
            }
            Expr::If {
                cond,
                then_expr,
                else_expr,
            } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(cond, path, proven);
                });
                match null_check_shape(&self.bool_defs, cond) {
                    Some((name, true)) => {
                        let mut then_proven = proven.clone();
                        then_proven.entries.insert(
                            name,
                            ProvenEntry {
                                guard_site: None,
                                proof: NullCheckProof::StructuredGuard,
                            },
                        );
                        walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                            self.expr(then_expr, path, &then_proven);
                        });
                        walk::with_path_segment(path, PathSegment::Expr(2), |path| {
                            self.expr(else_expr, path, proven);
                        });
                    }
                    Some((name, false)) => {
                        let mut else_proven = proven.clone();
                        else_proven.entries.insert(
                            name,
                            ProvenEntry {
                                guard_site: None,
                                proof: NullCheckProof::StructuredGuard,
                            },
                        );
                        walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                            self.expr(then_expr, path, proven);
                        });
                        walk::with_path_segment(path, PathSegment::Expr(2), |path| {
                            self.expr(else_expr, path, &else_proven);
                        });
                    }
                    None => {
                        walk::with_path_segment(path, PathSegment::Expr(1), |path| {
                            self.expr(then_expr, path, proven);
                        });
                        walk::with_path_segment(path, PathSegment::Expr(2), |path| {
                            self.expr(else_expr, path, proven);
                        });
                    }
                }
            }
            Expr::Block(block) | Expr::Unsafe(block) => {
                let segment = if matches!(expr, Expr::Unsafe(_)) {
                    PathSegment::UnsafeBody
                } else {
                    PathSegment::BlockBody
                };
                let mut inner = proven.clone();
                walk::with_path_segment(path, segment, |path| {
                    self.block(block, path, &mut inner);
                });
            }
            Expr::Match {
                expr: scrutinee,
                arms,
            } => {
                walk::with_path_segment(path, PathSegment::Expr(0), |path| {
                    self.expr(scrutinee, path, proven);
                });
                for (index, arm) in arms.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::MatchArm(index), |path| {
                        self.expr(&arm.value, path, &proven.clone());
                    });
                }
            }
            _ => {}
        }
    }
}

fn peel_casts(expr: &Expr) -> &Expr {
    match expr {
        Expr::Cast { expr, .. } => peel_casts(expr),
        _ => expr,
    }
}

fn is_null_expr(expr: &Expr) -> bool {
    matches!(peel_casts(expr), Expr::Value(RustValue::NullPtr))
}

fn is_provably_nonnull_construction(expr: &Expr, proven: &Proven) -> bool {
    match peel_casts(expr) {
        Expr::AddrOf { .. } => true,
        Expr::Var(name) => proven.entries.contains_key(name.as_str()),
        _ => false,
    }
}

fn resolve_cond<'e>(bool_defs: &'e BTreeMap<String, Expr>, expr: &'e Expr, depth: u32) -> &'e Expr {
    if depth == 0 {
        return expr;
    }
    if let Expr::Var(name) = expr
        && let Some(def) = bool_defs.get(name.as_str())
    {
        return resolve_cond(bool_defs, def, depth - 1);
    }
    expr
}

fn null_check_shape(bool_defs: &BTreeMap<String, Expr>, cond: &Expr) -> Option<(String, bool)> {
    match resolve_cond(bool_defs, cond, 4) {
        Expr::Binary {
            op: BinOp::Ne,
            lhs,
            rhs,
        } => pointer_against_null(bool_defs, lhs, rhs).map(|name| (name, true)),
        Expr::Binary {
            op: BinOp::Eq,
            lhs,
            rhs,
        } => pointer_against_null(bool_defs, lhs, rhs).map(|name| (name, false)),
        Expr::Unary {
            op: UnaryOp::Not,
            expr: inner,
        } => null_check_shape(bool_defs, inner).map(|(name, sense)| (name, !sense)),
        _ => None,
    }
}

fn resolve_pointer_alias<'e>(
    bool_defs: &'e BTreeMap<String, Expr>,
    name: &'e str,
    depth: u32,
) -> &'e str {
    if depth == 0 {
        return name;
    }
    match bool_defs.get(name).map(peel_casts) {
        Some(Expr::Var(next)) => resolve_pointer_alias(bool_defs, next.as_str(), depth - 1),
        _ => name,
    }
}

fn pointer_against_null(
    bool_defs: &BTreeMap<String, Expr>,
    lhs: &Expr,
    rhs: &Expr,
) -> Option<String> {
    if let Expr::Var(name) = peel_casts(lhs)
        && is_null_expr(rhs)
    {
        return Some(resolve_pointer_alias(bool_defs, name.as_str(), 4).to_string());
    }
    if let Expr::Var(name) = peel_casts(rhs)
        && is_null_expr(lhs)
    {
        return Some(resolve_pointer_alias(bool_defs, name.as_str(), 4).to_string());
    }
    None
}
