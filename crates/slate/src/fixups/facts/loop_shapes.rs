use std::collections::{BTreeMap, BTreeSet};

use crate::fixups::facts::walk;
use crate::fixups::facts::{
    AstPath, BindingId, FixupFacts, FunctionId, LoopId, LoopKind, LoopShapeFact, LoopShapeKind,
    LoopShapeKindTag, LoopShapeRejection, LoopShapeRejectionFact, PathSegment, ReductionOp,
    SearchResult, SentinelTarget, SliceLoopAccess,
};
use crate::rust_ast::{BinOp, Expr, Ident, IndentStmt, Item, Program, RustValue, Stmt, Type};

pub(in crate::fixups) fn collect_facts(program: &Program, facts: &mut FixupFacts) {
    facts.loop_shapes.clear();
    facts.loop_shape_rejections.clear();
    for (item_index, item) in program.items.iter().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        let mut collector = Collector::new(function, facts);
        for (index, param) in f.params.iter().enumerate() {
            if slice_elem_ty(&param.ty).is_some()
                && let Some(binding) = collector.facts.binding_by_param_index(function, index)
            {
                collector.slices.insert(param.name.to_string(), binding);
            }
        }
        collector.body(&f.body, &mut Vec::new());
    }
}

struct Collector<'a> {
    function: FunctionId,
    facts: &'a mut FixupFacts,
    slices: BTreeMap<String, BindingId>,
    len_aliases: BTreeMap<String, BindingId>,
}

impl<'a> Collector<'a> {
    fn new(function: FunctionId, facts: &'a mut FixupFacts) -> Self {
        Self {
            function,
            facts,
            slices: BTreeMap::new(),
            len_aliases: BTreeMap::new(),
        }
    }

    fn body(&mut self, body: &[IndentStmt], path: &mut Vec<PathSegment>) {
        self.collect_canonical_loops(body, path, None);
        for (index, indent) in body.iter().enumerate() {
            walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
                self.stmt(&indent.stmt, path);
                walk::nested_bodies_with_path(&indent.stmt, path, &mut |body, path| {
                    self.body(body, path);
                });
            });
        }
    }

    fn stmt(&mut self, stmt: &Stmt, path: &mut Vec<PathSegment>) {
        match stmt {
            Stmt::Let {
                name,
                ty,
                init: Some(init),
                ..
            } => {
                let ast_path = AstPath(path.clone());
                if ty.as_ref().is_some_and(|ty| slice_elem_ty(ty).is_some())
                    && let Some(binding) =
                        self.facts
                            .binding_by_local_path(self.function, name.as_str(), &ast_path)
                {
                    self.slices.insert(name.to_string(), binding);
                }
                if let Some(slice) = self.slice_len_source(init) {
                    self.len_aliases.insert(name.to_string(), slice);
                }
            }
            Stmt::While { cond, body } => {
                self.collect_sentinel_while(cond, body, path);
            }
            Stmt::Let { init: None, .. }
            | Stmt::LetIf { .. }
            | Stmt::Assign { .. }
            | Stmt::CompoundAssign { .. }
            | Stmt::Expr(_)
            | Stmt::Return(_)
            | Stmt::Unsafe { .. }
            | Stmt::If { .. }
            | Stmt::Loop { .. }
            | Stmt::For { .. }
            | Stmt::Scope { .. }
            | Stmt::LabeledBlock { .. }
            | Stmt::Match { .. }
            | Stmt::Break(_)
            | Stmt::Continue(_)
            | Stmt::Block(_) => {}
        }
    }

    fn collect_canonical_loops(
        &mut self,
        body: &[IndentStmt],
        parent_path: &[PathSegment],
        body_segment: Option<PathSegment>,
    ) {
        for (index, pair) in body.windows(2).enumerate() {
            let Some(candidate) =
                self.canonical_loop(pair, parent_path, body_segment.clone(), index)
            else {
                continue;
            };
            self.record_canonical_shapes(candidate);
        }
    }

    fn canonical_loop<'b>(
        &self,
        pair: &'b [IndentStmt],
        parent_path: &[PathSegment],
        body_segment: Option<PathSegment>,
        index_stmt: usize,
    ) -> Option<CanonicalLoop<'b>> {
        let Stmt::Let {
            name: index_name,
            init: Some(init),
            ..
        } = &pair[0].stmt
        else {
            return None;
        };
        if !is_zero(init) {
            return None;
        }
        let Stmt::Loop { body, .. } = &pair[1].stmt else {
            return None;
        };
        let first = body.first()?;
        let Stmt::If {
            cond, then_body, ..
        } = &first.stmt
        else {
            return None;
        };
        if !is_break_only(then_body) {
            return None;
        }
        let (index, bound) = negated_less_than(cond)?;
        if index.as_str() != index_name {
            return None;
        }
        let bound_collection = match bound {
            Expr::Var(len) => self.len_aliases.get(len.as_str()).copied()?,
            expr => self.slice_len_source(expr)?,
        };
        let increment_stmt = body.len().checked_sub(1)?;
        if increment_stmt == 0 || !increments_by_one(&body[increment_stmt].stmt, index) {
            return None;
        }
        if body[1..increment_stmt]
            .iter()
            .any(|indent| increments_index(&indent.stmt, index))
        {
            return None;
        }

        let mut index_path = parent_path.to_vec();
        if let Some(segment) = &body_segment {
            index_path.push(segment.clone());
        }
        index_path.push(PathSegment::Stmt(index_stmt));
        let index_binding = self.facts.binding_by_local_path(
            self.function,
            index_name.as_str(),
            &AstPath(index_path),
        )?;

        let mut loop_path = parent_path.to_vec();
        if let Some(segment) = &body_segment {
            loop_path.push(segment.clone());
        }
        loop_path.push(PathSegment::Stmt(index_stmt + 1));
        let loop_path = AstPath(loop_path);
        let loop_id = self.loop_by_path(&loop_path, LoopKind::Loop)?;

        let mut body_path = loop_path.0.clone();
        body_path.push(PathSegment::LoopBody);

        Some(CanonicalLoop {
            loop_id,
            index_name: Ident::new(index_name.as_str()),
            index_binding,
            bound_collection,
            body: &body[1..increment_stmt],
            loop_path,
            body_path: AstPath(body_path),
        })
    }

    fn record_canonical_shapes(&mut self, candidate: CanonicalLoop<'_>) {
        let analysis = BodyAnalysis::new(candidate.body, &candidate.index_name, &self.slices);
        let access = if analysis.mutated_collections.is_empty() {
            SliceLoopAccess::ReadOnly
        } else {
            SliceLoopAccess::Mutable
        };
        if analysis.unsupported_control {
            self.reject(
                candidate.loop_id,
                LoopShapeKindTag::Counted,
                LoopShapeRejection::UnsupportedControlFlow,
                &candidate.loop_path,
            );
        } else if analysis.collections.is_empty() {
            self.reject(
                candidate.loop_id,
                LoopShapeKindTag::Counted,
                LoopShapeRejection::MissingCollection,
                &candidate.loop_path,
            );
        } else {
            self.push_shape(LoopShapeFact {
                function: self.function,
                loop_id: candidate.loop_id,
                kind: LoopShapeKind::Counted { access },
                induction: Some(candidate.index_binding),
                accumulators: Vec::new(),
                collections: sorted_bindings(&analysis.collections),
                mutation_targets: sorted_bindings(&analysis.mutated_collections),
                loop_path: candidate.loop_path.clone(),
                body_path: candidate.body_path.clone(),
            });
        }

        match reduction_shape(
            candidate.body,
            &candidate.index_name,
            &self.slices,
            &analysis,
        ) {
            Ok((accumulator, op)) => {
                if let Some(accumulator) = self.binding_by_name(accumulator.as_str()) {
                    self.push_shape(LoopShapeFact {
                        function: self.function,
                        loop_id: candidate.loop_id,
                        kind: LoopShapeKind::Reduction { op },
                        induction: Some(candidate.index_binding),
                        accumulators: vec![accumulator],
                        collections: sorted_bindings(&analysis.collections),
                        mutation_targets: Vec::new(),
                        loop_path: candidate.loop_path.clone(),
                        body_path: candidate.body_path.clone(),
                    });
                } else {
                    self.reject(
                        candidate.loop_id,
                        LoopShapeKindTag::Reduction,
                        LoopShapeRejection::MissingMutation,
                        &candidate.loop_path,
                    );
                }
            }
            Err(reason) => self.reject(
                candidate.loop_id,
                LoopShapeKindTag::Reduction,
                reason,
                &candidate.loop_path,
            ),
        }

        match search_shape(candidate.body, &candidate.index_name, &self.slices) {
            Ok(result) => self.push_shape(LoopShapeFact {
                function: self.function,
                loop_id: candidate.loop_id,
                kind: LoopShapeKind::Search { result },
                induction: Some(candidate.index_binding),
                accumulators: Vec::new(),
                collections: sorted_bindings(&analysis.collections),
                mutation_targets: Vec::new(),
                loop_path: candidate.loop_path.clone(),
                body_path: candidate.body_path.clone(),
            }),
            Err(reason) => self.reject(
                candidate.loop_id,
                LoopShapeKindTag::Search,
                reason,
                &candidate.loop_path,
            ),
        }

        match copy_shape(candidate.body, &candidate.index_name, &self.slices) {
            Ok((src, dst)) => self.push_shape(LoopShapeFact {
                function: self.function,
                loop_id: candidate.loop_id,
                kind: LoopShapeKind::Copy,
                induction: Some(candidate.index_binding),
                accumulators: Vec::new(),
                collections: sorted_bindings(&BTreeSet::from([src, dst])),
                mutation_targets: vec![dst],
                loop_path: candidate.loop_path.clone(),
                body_path: candidate.body_path.clone(),
            }),
            Err(reason) => self.reject(
                candidate.loop_id,
                LoopShapeKindTag::Copy,
                reason,
                &candidate.loop_path,
            ),
        }

        match fill_shape(candidate.body, &candidate.index_name, &self.slices) {
            Ok(dst) => self.push_shape(LoopShapeFact {
                function: self.function,
                loop_id: candidate.loop_id,
                kind: LoopShapeKind::Fill,
                induction: Some(candidate.index_binding),
                accumulators: Vec::new(),
                collections: vec![dst],
                mutation_targets: vec![dst],
                loop_path: candidate.loop_path.clone(),
                body_path: candidate.body_path.clone(),
            }),
            Err(reason) => self.reject(
                candidate.loop_id,
                LoopShapeKindTag::Fill,
                reason,
                &candidate.loop_path,
            ),
        }

        let _ = candidate.bound_collection;
    }

    fn collect_sentinel_while(
        &mut self,
        cond: &Expr,
        body: &crate::rust_ast::Block,
        path: &[PathSegment],
    ) {
        let loop_path = AstPath(path.to_vec());
        let Some(loop_id) = self.loop_by_path(&loop_path, LoopKind::While) else {
            return;
        };
        let Some((collection, index_name)) = sentinel_cond(cond, &self.slices) else {
            self.reject(
                loop_id,
                LoopShapeKindTag::Sentinel,
                LoopShapeRejection::MissingCollection,
                &loop_path,
            );
            return;
        };
        let Some(induction) = self.binding_by_name(index_name.as_str()) else {
            self.reject(
                loop_id,
                LoopShapeKindTag::Sentinel,
                LoopShapeRejection::MissingInduction,
                &loop_path,
            );
            return;
        };
        if !body
            .stmts
            .iter()
            .any(|indent| increments_index(&indent.stmt, &index_name))
        {
            self.reject(
                loop_id,
                LoopShapeKindTag::Sentinel,
                LoopShapeRejection::MissingInduction,
                &loop_path,
            );
            return;
        }
        let mut body_path = path.to_vec();
        body_path.push(PathSegment::WhileBody);
        self.push_shape(LoopShapeFact {
            function: self.function,
            loop_id,
            kind: LoopShapeKind::Sentinel {
                target: SentinelTarget::IndexedCollection,
            },
            induction: Some(induction),
            accumulators: Vec::new(),
            collections: vec![collection],
            mutation_targets: Vec::new(),
            loop_path,
            body_path: AstPath(body_path),
        });
    }

    fn push_shape(&mut self, fact: LoopShapeFact) {
        self.facts.loop_shapes.push(fact);
    }

    fn reject(
        &mut self,
        loop_id: LoopId,
        attempted: LoopShapeKindTag,
        reason: LoopShapeRejection,
        loop_path: &AstPath,
    ) {
        self.facts
            .loop_shape_rejections
            .push(LoopShapeRejectionFact {
                function: self.function,
                loop_id,
                attempted,
                reason,
                loop_path: loop_path.clone(),
            });
    }

    fn slice_len_source(&self, expr: &Expr) -> Option<BindingId> {
        let Expr::MethodCall { recv, method, args } = peel_casts(expr) else {
            return None;
        };
        if method != "len" || !args.is_empty() {
            return None;
        }
        let Expr::Var(name) = &**recv else {
            return None;
        };
        self.slices.get(name.as_str()).copied()
    }

    fn loop_by_path(&self, path: &AstPath, kind: LoopKind) -> Option<LoopId> {
        self.facts
            .loops
            .iter()
            .find(|loop_fact| {
                loop_fact.function == self.function
                    && loop_fact.kind == kind
                    && &loop_fact.path == path
            })
            .map(|loop_fact| loop_fact.id)
    }

    fn binding_by_name(&self, name: &str) -> Option<BindingId> {
        self.facts
            .bindings
            .iter()
            .find(|binding| binding.function == self.function && binding.name == name)
            .map(|binding| binding.id)
    }
}

struct CanonicalLoop<'a> {
    loop_id: LoopId,
    index_name: Ident,
    index_binding: BindingId,
    bound_collection: BindingId,
    body: &'a [IndentStmt],
    loop_path: AstPath,
    body_path: AstPath,
}

struct BodyAnalysis {
    collections: BTreeSet<BindingId>,
    mutated_collections: BTreeSet<BindingId>,
    unsupported_control: bool,
}

impl BodyAnalysis {
    fn new(body: &[IndentStmt], index: &Ident, slices: &BTreeMap<String, BindingId>) -> Self {
        let mut analysis = Self {
            collections: BTreeSet::new(),
            mutated_collections: BTreeSet::new(),
            unsupported_control: false,
        };
        for indent in body {
            analysis.stmt(&indent.stmt, index, slices);
        }
        analysis
    }

    fn stmt(&mut self, stmt: &Stmt, index: &Ident, slices: &BTreeMap<String, BindingId>) {
        match stmt {
            Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
                if let Some(collection) = indexed_collection(target, index, slices) {
                    self.collections.insert(collection);
                    self.mutated_collections.insert(collection);
                } else {
                    self.expr(target, index, slices);
                }
                self.expr(value, index, slices);
            }
            Stmt::If {
                cond,
                then_body,
                else_body,
            } => {
                self.expr(cond, index, slices);
                for indent in then_body.iter().chain(else_body) {
                    self.stmt(&indent.stmt, index, slices);
                }
            }
            Stmt::Loop { .. } | Stmt::While { .. } => {
                self.unsupported_control = true;
            }
            _ => walk::stmt_exprs(stmt, &mut |expr| self.expr(expr, index, slices)),
        }
    }

    fn expr(&mut self, expr: &Expr, index: &Ident, slices: &BTreeMap<String, BindingId>) {
        if let Some(collection) = indexed_collection(expr, index, slices) {
            self.collections.insert(collection);
        }
    }
}

fn reduction_shape(
    body: &[IndentStmt],
    index: &Ident,
    slices: &BTreeMap<String, BindingId>,
    analysis: &BodyAnalysis,
) -> Result<(String, ReductionOp), LoopShapeRejection> {
    if analysis.collections.is_empty() {
        return Err(LoopShapeRejection::MissingCollection);
    }
    let reductions = body
        .iter()
        .filter_map(|indent| reduction_stmt(&indent.stmt, index, slices))
        .collect::<Vec<_>>();
    match reductions.as_slice() {
        [] => Err(LoopShapeRejection::MissingMutation),
        [reduction] => Ok(reduction.clone()),
        _ => Err(LoopShapeRejection::MultipleMutations),
    }
}

fn reduction_stmt(
    stmt: &Stmt,
    index: &Ident,
    slices: &BTreeMap<String, BindingId>,
) -> Option<(String, ReductionOp)> {
    match stmt {
        Stmt::CompoundAssign { target, op, value }
            if has_indexed_collection(value, index, slices) =>
        {
            let Expr::Var(name) = target else {
                return None;
            };
            Some((name.as_str().to_string(), reduction_op(*op)?))
        }
        Stmt::Assign { target, value } => {
            let Expr::Var(name) = target else {
                return None;
            };
            let Expr::Binary { op, lhs, rhs } = value else {
                return None;
            };
            let lhs_acc = matches!(&**lhs, Expr::Var(lhs) if lhs == name);
            let rhs_acc = matches!(&**rhs, Expr::Var(rhs) if rhs == name);
            let has_collection = has_indexed_collection(lhs, index, slices)
                || has_indexed_collection(rhs, index, slices);
            if (lhs_acc || rhs_acc) && has_collection {
                Some((name.as_str().to_string(), reduction_op(*op)?))
            } else {
                None
            }
        }
        _ => None,
    }
}

fn search_shape(
    body: &[IndentStmt],
    index: &Ident,
    slices: &BTreeMap<String, BindingId>,
) -> Result<SearchResult, LoopShapeRejection> {
    for indent in body {
        if let Stmt::If {
            cond, then_body, ..
        } = &indent.stmt
        {
            if !has_indexed_collection(cond, index, slices) {
                continue;
            }
            if is_break_only(then_body) {
                return Ok(SearchResult::BreaksOnMatch);
            }
            if then_body
                .iter()
                .any(|indent| matches!(indent.stmt, Stmt::Break(_)))
                && then_body.iter().any(|indent| {
                    matches!(
                        indent.stmt,
                        Stmt::Assign {
                            target: Expr::Var(_),
                            ..
                        }
                    )
                })
            {
                return Ok(SearchResult::AssignsFlag);
            }
        }
    }
    Err(LoopShapeRejection::MissingMutation)
}

fn copy_shape(
    body: &[IndentStmt],
    index: &Ident,
    slices: &BTreeMap<String, BindingId>,
) -> Result<(BindingId, BindingId), LoopShapeRejection> {
    let copies = body
        .iter()
        .filter_map(|indent| {
            let Stmt::Assign { target, value } = &indent.stmt else {
                return None;
            };
            let dst = indexed_collection(target, index, slices)?;
            let src = indexed_collection(value, index, slices)?;
            (src != dst).then_some((src, dst))
        })
        .collect::<Vec<_>>();
    match copies.as_slice() {
        [] => Err(LoopShapeRejection::MissingMutation),
        [copy] => Ok(*copy),
        _ => Err(LoopShapeRejection::MultipleMutations),
    }
}

fn fill_shape(
    body: &[IndentStmt],
    index: &Ident,
    slices: &BTreeMap<String, BindingId>,
) -> Result<BindingId, LoopShapeRejection> {
    let fills = body
        .iter()
        .filter_map(|indent| {
            let Stmt::Assign { target, value } = &indent.stmt else {
                return None;
            };
            let dst = indexed_collection(target, index, slices)?;
            (!has_indexed_collection(value, index, slices)).then_some(dst)
        })
        .collect::<Vec<_>>();
    match fills.as_slice() {
        [] => Err(LoopShapeRejection::MissingMutation),
        [dst] => Ok(*dst),
        _ => Err(LoopShapeRejection::MultipleMutations),
    }
}

fn sentinel_cond(cond: &Expr, slices: &BTreeMap<String, BindingId>) -> Option<(BindingId, Ident)> {
    let Expr::Binary { op, lhs, rhs } = peel_casts(cond) else {
        return None;
    };
    if !matches!(op, BinOp::Ne | BinOp::Gt) {
        return None;
    }
    if !is_zero(rhs) {
        return None;
    }
    let Expr::Index { base, index } = peel_casts(lhs) else {
        return None;
    };
    let Expr::Var(base) = peel_casts(base) else {
        return None;
    };
    let Expr::Var(index) = peel_casts(index) else {
        return None;
    };
    Some((slices.get(base.as_str()).copied()?, index.clone()))
}

fn indexed_collection(
    expr: &Expr,
    index: &Ident,
    slices: &BTreeMap<String, BindingId>,
) -> Option<BindingId> {
    let Expr::Index {
        base,
        index: actual_index,
    } = peel_casts(expr)
    else {
        return None;
    };
    let Expr::Var(base) = peel_casts(base) else {
        return None;
    };
    let Expr::Var(actual_index) = peel_casts(actual_index) else {
        return None;
    };
    if actual_index != index {
        return None;
    }
    slices.get(base.as_str()).copied()
}

fn has_indexed_collection(
    expr: &Expr,
    index: &Ident,
    slices: &BTreeMap<String, BindingId>,
) -> bool {
    let mut found = false;
    walk::exprs(expr, &mut |expr| {
        found |= indexed_collection(expr, index, slices).is_some();
    });
    found
}

fn sorted_bindings(bindings: &BTreeSet<BindingId>) -> Vec<BindingId> {
    bindings.iter().copied().collect()
}

fn reduction_op(op: BinOp) -> Option<ReductionOp> {
    Some(match op {
        BinOp::Add => ReductionOp::Add,
        BinOp::Mul => ReductionOp::Mul,
        BinOp::BitAnd => ReductionOp::BitAnd,
        BinOp::BitOr => ReductionOp::BitOr,
        BinOp::BitXor => ReductionOp::BitXor,
        BinOp::And => ReductionOp::And,
        BinOp::Or => ReductionOp::Or,
        _ => return None,
    })
}

fn is_break_only(body: &[IndentStmt]) -> bool {
    matches!(
        body,
        [IndentStmt {
            stmt: Stmt::Break(None),
            ..
        }]
    )
}

fn negated_less_than(expr: &Expr) -> Option<(&Ident, &Expr)> {
    let Expr::Unary {
        op: crate::rust_ast::UnaryOp::Not,
        expr,
    } = expr
    else {
        return None;
    };
    let Expr::Binary {
        op: BinOp::Lt,
        lhs,
        rhs,
    } = &**expr
    else {
        return None;
    };
    let Expr::Var(index) = &**lhs else {
        return None;
    };
    Some((index, rhs))
}

fn increments_by_one(stmt: &Stmt, index: &Ident) -> bool {
    match stmt {
        Stmt::CompoundAssign {
            target,
            op: BinOp::Add,
            value,
        } => matches!(target, Expr::Var(name) if name == index) && is_one(value),
        Stmt::Assign { target, value } => {
            matches!(target, Expr::Var(name) if name == index) && adds_one(value, index)
        }
        _ => false,
    }
}

fn increments_index(stmt: &Stmt, index: &Ident) -> bool {
    match stmt {
        Stmt::CompoundAssign { target, .. } | Stmt::Assign { target, .. } => {
            matches!(target, Expr::Var(name) if name == index)
        }
        _ => false,
    }
}

fn adds_one(expr: &Expr, index: &Ident) -> bool {
    let Expr::Binary {
        op: BinOp::Add,
        lhs,
        rhs,
    } = expr
    else {
        return false;
    };
    matches!(&**lhs, Expr::Var(name) if name == index) && is_one(rhs)
}

fn is_zero(expr: &Expr) -> bool {
    integer_value(expr) == Some(0)
}

fn is_one(expr: &Expr) -> bool {
    integer_value(expr) == Some(1)
}

fn integer_value(expr: &Expr) -> Option<i128> {
    match expr {
        Expr::Value(RustValue::I64(n)) => Some(i128::from(*n)),
        Expr::Value(RustValue::I128(n)) => Some(*n),
        Expr::Cast { expr, .. } => integer_value(expr),
        _ => None,
    }
}

fn peel_casts(expr: &Expr) -> &Expr {
    match expr {
        Expr::Cast { expr, .. } => peel_casts(expr),
        _ => expr,
    }
}

fn slice_elem_ty(ty: &Type) -> Option<&Type> {
    match ty {
        Type::Ref { inner, .. } => match &**inner {
            Type::Slice(elem) => Some(elem),
            _ => None,
        },
        Type::Slice(elem) => Some(elem),
        _ => None,
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::facts;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Block, FnParam, Item, Program, UnaryOp};

    fn analyze_collect(program: &Program) -> FixupFacts {
        let analyzed = facts::analyze(program.clone());
        let mut facts = analyzed.facts;
        collect_facts(program, &mut facts);
        facts
    }

    fn slice_param(name: &str) -> FnParam {
        FnParam {
            name: name.into(),
            mutable: true,
            ty: Type::parse("&mut [i32]"),
        }
    }

    fn index(slice: &str, index: &str) -> Expr {
        Expr::Index {
            base: Box::new(var(slice)),
            index: Box::new(var(index)),
        }
    }

    fn len_call(name: &str) -> Expr {
        Expr::MethodCall {
            recv: Box::new(var(name)),
            method: "len".into(),
            args: Vec::new(),
        }
    }

    fn cast(expr: Expr, ty: &str) -> Expr {
        Expr::Cast {
            expr: Box::new(expr),
            ty: Type::parse(ty),
        }
    }

    fn canonical_program(params: Vec<FnParam>, prefix: Vec<Stmt>, body_stmt: Stmt) -> Program {
        let mut stmts = vec![temp("len", "i32", cast(len_call("items"), "i32"))];
        stmts.extend(prefix);
        stmts.push(Stmt::Scope {
            body: vec![
                IndentStmt {
                    depth: 2,
                    stmt: let_mut("i", "i32", int(0)),
                },
                IndentStmt {
                    depth: 2,
                    stmt: Stmt::Loop {
                        label: None,
                        body: vec![
                            IndentStmt {
                                depth: 3,
                                stmt: Stmt::If {
                                    cond: Expr::Unary {
                                        op: UnaryOp::Not,
                                        expr: Box::new(bin(BinOp::Lt, var("i"), var("len"))),
                                    },
                                    then_body: vec![IndentStmt {
                                        depth: 4,
                                        stmt: Stmt::Break(None),
                                    }],
                                    else_body: Vec::new(),
                                },
                            },
                            IndentStmt {
                                depth: 3,
                                stmt: body_stmt,
                            },
                            IndentStmt {
                                depth: 3,
                                stmt: Stmt::CompoundAssign {
                                    target: var("i"),
                                    op: BinOp::Add,
                                    value: int(1),
                                },
                            },
                        ],
                    },
                },
            ],
        });
        let mut f = func(params, None, stmts);
        f.name = "shapes".into();
        Program {
            items: vec![Item::Fn(f)],
        }
    }

    fn canonical(body_stmt: Stmt) -> Program {
        canonical_program(vec![slice_param("items")], Vec::new(), body_stmt)
    }

    fn has_shape(facts: &FixupFacts, pred: impl Fn(&LoopShapeKind) -> bool) -> bool {
        facts.loop_shapes.iter().any(|fact| pred(&fact.kind))
    }

    fn has_rejection(
        facts: &FixupFacts,
        attempted: LoopShapeKindTag,
        reason: LoopShapeRejection,
    ) -> bool {
        facts
            .loop_shape_rejections
            .iter()
            .any(|fact| fact.attempted == attempted && fact.reason == reason)
    }

    #[test]
    fn records_counted_loop_shape() {
        let facts = analyze_collect(&canonical(temp("x", "i32", index("items", "i"))));

        assert!(has_shape(&facts, |kind| matches!(
            kind,
            LoopShapeKind::Counted {
                access: SliceLoopAccess::ReadOnly
            }
        )));
    }

    #[test]
    fn rejects_counted_loop_without_collection_access() {
        let facts = analyze_collect(&canonical(Stmt::Expr(call("observe", vec![int(1)]))));

        assert!(has_rejection(
            &facts,
            LoopShapeKindTag::Counted,
            LoopShapeRejection::MissingCollection
        ));
    }

    #[test]
    fn records_reduction_loop_shape() {
        let facts = analyze_collect(&canonical_program(
            vec![slice_param("items")],
            vec![let_mut("sum", "i32", int(0))],
            Stmt::CompoundAssign {
                target: var("sum"),
                op: BinOp::Add,
                value: index("items", "i"),
            },
        ));

        assert!(has_shape(&facts, |kind| matches!(
            kind,
            LoopShapeKind::Reduction {
                op: ReductionOp::Add
            }
        )));
    }

    #[test]
    fn rejects_reduction_without_accumulator_mutation() {
        let facts = analyze_collect(&canonical(temp("x", "i32", index("items", "i"))));

        assert!(has_rejection(
            &facts,
            LoopShapeKindTag::Reduction,
            LoopShapeRejection::MissingMutation
        ));
    }

    #[test]
    fn records_search_loop_shape() {
        let facts = analyze_collect(&canonical(Stmt::If {
            cond: bin(BinOp::Eq, index("items", "i"), int(7)),
            then_body: vec![IndentStmt {
                depth: 4,
                stmt: Stmt::Break(None),
            }],
            else_body: Vec::new(),
        }));

        assert!(has_shape(&facts, |kind| matches!(
            kind,
            LoopShapeKind::Search {
                result: SearchResult::BreaksOnMatch
            }
        )));
    }

    #[test]
    fn rejects_search_without_conditional_break() {
        let facts = analyze_collect(&canonical(temp("x", "i32", index("items", "i"))));

        assert!(has_rejection(
            &facts,
            LoopShapeKindTag::Search,
            LoopShapeRejection::MissingMutation
        ));
    }

    #[test]
    fn records_copy_loop_shape() {
        let facts = analyze_collect(&canonical_program(
            vec![slice_param("items"), slice_param("out")],
            Vec::new(),
            Stmt::Assign {
                target: index("out", "i"),
                value: index("items", "i"),
            },
        ));

        assert!(has_shape(&facts, |kind| matches!(
            kind,
            LoopShapeKind::Copy
        )));
    }

    #[test]
    fn rejects_copy_without_distinct_source_and_destination() {
        let facts = analyze_collect(&canonical(Stmt::Assign {
            target: index("items", "i"),
            value: int(3),
        }));

        assert!(has_rejection(
            &facts,
            LoopShapeKindTag::Copy,
            LoopShapeRejection::MissingMutation
        ));
    }

    #[test]
    fn records_fill_loop_shape() {
        let facts = analyze_collect(&canonical(Stmt::Assign {
            target: index("items", "i"),
            value: int(3),
        }));

        assert!(has_shape(&facts, |kind| matches!(
            kind,
            LoopShapeKind::Fill
        )));
    }

    #[test]
    fn rejects_fill_when_value_depends_on_collection_index() {
        let facts = analyze_collect(&canonical_program(
            vec![slice_param("items"), slice_param("out")],
            Vec::new(),
            Stmt::Assign {
                target: index("out", "i"),
                value: index("items", "i"),
            },
        ));

        assert!(has_rejection(
            &facts,
            LoopShapeKindTag::Fill,
            LoopShapeRejection::MissingMutation
        ));
    }

    #[test]
    fn records_sentinel_loop_shape() {
        let mut f = func(
            vec![slice_param("items")],
            None,
            vec![
                let_mut("i", "i32", int(0)),
                Stmt::While {
                    cond: bin(BinOp::Ne, index("items", "i"), int(0)),
                    body: Block {
                        stmts: vec![IndentStmt {
                            depth: 2,
                            stmt: Stmt::CompoundAssign {
                                target: var("i"),
                                op: BinOp::Add,
                                value: int(1),
                            },
                        }],
                        tail: None,
                    },
                },
            ],
        );
        f.name = "sentinel".into();
        let facts = analyze_collect(&Program {
            items: vec![Item::Fn(f)],
        });

        assert!(has_shape(&facts, |kind| matches!(
            kind,
            LoopShapeKind::Sentinel {
                target: SentinelTarget::IndexedCollection
            }
        )));
    }

    #[test]
    fn rejects_sentinel_loop_without_indexed_condition() {
        let mut f = func(
            vec![slice_param("items")],
            None,
            vec![Stmt::While {
                cond: bin(BinOp::Ne, var("keep_going"), int(0)),
                body: Block::default(),
            }],
        );
        f.name = "sentinel".into();
        let facts = analyze_collect(&Program {
            items: vec![Item::Fn(f)],
        });

        assert!(has_rejection(
            &facts,
            LoopShapeKindTag::Sentinel,
            LoopShapeRejection::MissingCollection
        ));
    }
}
