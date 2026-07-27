use std::collections::{BTreeMap, BTreeSet};

use crate::fixups::facts::walk;
use crate::fixups::facts::{
    ArrayElementPointerOriginFact, AstPath, BindingId, FixupFacts, FunctionId, PathSegment, Site,
};
use crate::rust_ast::{Block, Expr, Ident, IndentStmt, Item, Program, RustValue, Stmt, Type};

pub(in crate::fixups) fn collect_facts(program: &Program, facts: &mut FixupFacts) {
    facts.array_element_pointer_origins.clear();
    let mut all = Vec::new();
    for (item_index, item) in program.items.iter().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        let mut collector = Collector::new(function, facts);
        collector.enter_scope();
        collector.body(&f.body, &mut Vec::new(), false);
        collector.exit_scope();
        all.extend(collector.finish());
    }
    facts.array_element_pointer_origins = all;
}

struct Collector<'a> {
    function: FunctionId,
    facts: &'a FixupFacts,
    scopes: Vec<BTreeMap<String, BindingId>>,
    candidates: Vec<Candidate>,
}

#[derive(Clone)]
struct Candidate {
    pointer: BindingId,
    base: BindingId,
    index: Expr,
    mutable: bool,
    path: AstPath,
    kind: CandidateKind,
}

#[derive(Clone, Copy)]
enum CandidateKind {
    LetInit,
    Assign,
}

struct OriginSource {
    base_name: Ident,
    index: Expr,
    mutable: bool,
}

impl<'a> Collector<'a> {
    fn new(function: FunctionId, facts: &'a FixupFacts) -> Self {
        Self {
            function,
            facts,
            scopes: Vec::new(),
            candidates: Vec::new(),
        }
    }

    fn enter_scope(&mut self) {
        self.scopes.push(BTreeMap::new());
    }

    fn exit_scope(&mut self) {
        self.scopes.pop();
    }

    fn finish(self) -> Vec<ArrayElementPointerOriginFact> {
        let Self {
            function,
            facts,
            candidates,
            ..
        } = self;
        let pointers_with_overwritten_init_origins =
            overwritten_init_origin_pointers(facts, &candidates);
        candidates
            .into_iter()
            .filter(|candidate| pointer_write_shape_is_unambiguous(facts, candidate))
            .filter(|candidate| {
                !pointers_with_overwritten_init_origins.contains(&candidate.pointer)
            })
            .map(|candidate| ArrayElementPointerOriginFact {
                site: Site {
                    function,
                    path: candidate.path,
                },
                pointer: candidate.pointer,
                base: candidate.base,
                index: candidate.index,
                mutable: candidate.mutable,
            })
            .collect()
    }

    fn body(&mut self, body: &[IndentStmt], path: &mut Vec<PathSegment>, scoped: bool) {
        if scoped {
            self.enter_scope();
        }
        for (index, indent) in body.iter().enumerate() {
            walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
                self.stmt(&indent.stmt, path);
            });
        }
        if scoped {
            self.exit_scope();
        }
    }

    fn block(&mut self, block: &Block, path: &mut Vec<PathSegment>) {
        self.enter_scope();
        self.body(&block.stmts, path, false);
        self.exit_scope();
    }

    fn stmt(&mut self, stmt: &Stmt, path: &mut Vec<PathSegment>) {
        match stmt {
            Stmt::Let { name, ty, init, .. } => {
                if let Some(init) = init {
                    self.collect_let_origin(name, init, path);
                }
                self.record_array_binding(name, ty.as_ref(), path);
            }
            Stmt::Assign { target, value } => self.collect_assign_origin(target, value, path),
            Stmt::LetIf {
                then_body,
                else_body,
                ..
            } => {
                walk::with_path_segment(path, PathSegment::Then, |path| {
                    self.body(then_body, path, true)
                });
                walk::with_path_segment(path, PathSegment::Else, |path| {
                    self.body(else_body, path, true)
                });
            }
            Stmt::If {
                then_body,
                else_body,
                ..
            } => {
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
            Stmt::For { body, .. } => {
                walk::with_path_segment(path, PathSegment::ForBody, |path| {
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
            Stmt::While { body, .. } => {
                walk::with_path_segment(path, PathSegment::WhileBody, |path| {
                    self.block(body, path)
                });
            }
            Stmt::Block(body) => {
                walk::with_path_segment(path, PathSegment::BlockBody, |path| {
                    self.block(body, path)
                });
            }
            Stmt::Match { arms, .. } => {
                for (index, arm) in arms.iter().enumerate() {
                    walk::with_path_segment(path, PathSegment::MatchArm(index), |path| {
                        self.body(&arm.body, path, true)
                    });
                }
            }
            Stmt::CompoundAssign { .. }
            | Stmt::InlineAsm(_)
            | Stmt::Expr(_)
            | Stmt::Return(_)
            | Stmt::Break(_)
            | Stmt::Continue(_) => {}
        }
    }

    fn collect_let_origin(&mut self, name: &str, init: &Expr, path: &[PathSegment]) {
        let ast_path = AstPath(path.to_vec());
        let Some(pointer) = self
            .facts
            .binding_by_local_path(self.function, name, &ast_path)
        else {
            return;
        };
        if !self.binding_is_pointer(pointer) {
            return;
        }
        let Some(source) = origin_source(init) else {
            return;
        };
        let Some(base) = self.array_binding(source.base_name.as_str()) else {
            return;
        };
        self.candidates.push(Candidate {
            pointer,
            base,
            index: source.index,
            mutable: source.mutable,
            path: ast_path,
            kind: CandidateKind::LetInit,
        });
    }

    fn collect_assign_origin(&mut self, target: &Expr, value: &Expr, path: &[PathSegment]) {
        let Expr::Var(name) = target else {
            return;
        };
        let ast_path = AstPath(path.to_vec());
        let Some(pointer) = self.binding_written_at(name.as_str(), &ast_path) else {
            return;
        };
        if !self.binding_is_pointer(pointer) {
            return;
        }
        let Some(source) = origin_source(value) else {
            return;
        };
        let Some(base) = self.array_binding(source.base_name.as_str()) else {
            return;
        };
        self.candidates.push(Candidate {
            pointer,
            base,
            index: source.index,
            mutable: source.mutable,
            path: ast_path,
            kind: CandidateKind::Assign,
        });
    }

    fn record_array_binding(&mut self, name: &str, ty: Option<&Type>, path: &[PathSegment]) {
        if !matches!(ty.map(Type::peel_aligned), Some(Type::Array { .. })) {
            return;
        }
        let ast_path = AstPath(path.to_vec());
        if let Some(binding) = self
            .facts
            .binding_by_local_path(self.function, name, &ast_path)
            && let Some(scope) = self.scopes.last_mut()
        {
            scope.insert(name.to_string(), binding);
        }
    }

    fn array_binding(&self, name: &str) -> Option<BindingId> {
        self.scopes
            .iter()
            .rev()
            .find_map(|scope| scope.get(name).copied())
    }

    fn binding_written_at(&self, name: &str, path: &AstPath) -> Option<BindingId> {
        self.facts
            .bindings
            .iter()
            .filter(|binding| binding.function == self.function && binding.name == name)
            .find(|binding| {
                self.facts
                    .def_use(binding.id)
                    .is_some_and(|def_use| def_use.writes.iter().any(|write| write == path))
            })
            .map(|binding| binding.id)
    }

    fn binding_is_pointer(&self, binding: BindingId) -> bool {
        self.facts
            .binding_types
            .iter()
            .find(|fact| fact.binding == binding)
            .is_some_and(|fact| fact.rendered.starts_with('*'))
    }
}

fn pointer_write_shape_is_unambiguous(facts: &FixupFacts, candidate: &Candidate) -> bool {
    let Some(def_use) = facts.def_use(candidate.pointer) else {
        return false;
    };
    match candidate.kind {
        CandidateKind::LetInit => def_use.writes.is_empty(),
        CandidateKind::Assign => {
            def_use.writes.len() == 1 && def_use.writes.first() == Some(&candidate.path)
        }
    }
}

fn overwritten_init_origin_pointers(
    facts: &FixupFacts,
    candidates: &[Candidate],
) -> BTreeSet<BindingId> {
    candidates
        .iter()
        .filter(|candidate| matches!(candidate.kind, CandidateKind::LetInit))
        .filter(|candidate| {
            facts
                .def_use(candidate.pointer)
                .is_some_and(|def_use| !def_use.writes.is_empty())
        })
        .map(|candidate| candidate.pointer)
        .collect()
}

fn origin_source(expr: &Expr) -> Option<OriginSource> {
    match peel_casts(expr) {
        Expr::AddrOf { mutable, expr } => indexed_array_origin(expr, *mutable),
        Expr::MethodCall { recv, method, args } if args.len() == 1 => {
            let mutable = match method.as_str() {
                "add" | "offset" => true,
                _ => return None,
            };
            let (base_name, base_mutable) = array_pointer_source(recv)?;
            let index = integer_expr(&args[0])?;
            Some(OriginSource {
                base_name: base_name.clone(),
                index,
                mutable: mutable || base_mutable,
            })
        }
        _ => None,
    }
}

fn indexed_array_origin(expr: &Expr, mutable: bool) -> Option<OriginSource> {
    let Expr::Index { base, index } = expr else {
        return None;
    };
    let base_name = array_base_name(base)?;
    Some(OriginSource {
        base_name: base_name.clone(),
        index: integer_expr(index)?,
        mutable,
    })
}

fn array_pointer_source(expr: &Expr) -> Option<(&Ident, bool)> {
    match peel_casts(expr) {
        Expr::ArrayPtr { array, mutable } => {
            let name = array_base_name(array)?;
            Some((name, *mutable))
        }
        Expr::MethodCall { recv, method, args } if args.is_empty() => {
            let mutable = match method.as_str() {
                "as_ptr" => false,
                "as_mut_ptr" => true,
                _ => return None,
            };
            let name = array_base_name(recv)?;
            Some((name, mutable))
        }
        _ => None,
    }
}

fn array_base_name(expr: &Expr) -> Option<&Ident> {
    match expr {
        Expr::Var(name) => Some(name),
        Expr::Unary {
            op: crate::rust_ast::UnaryOp::Deref,
            expr,
        } => array_base_name(expr),
        _ => None,
    }
}

fn peel_casts(expr: &Expr) -> &Expr {
    match expr {
        Expr::Cast { expr, .. } => peel_casts(expr),
        _ => expr,
    }
}

fn integer_expr(expr: &Expr) -> Option<Expr> {
    match peel_casts(expr) {
        Expr::Value(RustValue::I64(_)) | Expr::Value(RustValue::I128(_)) => Some(expr.clone()),
        _ => None,
    }
}
