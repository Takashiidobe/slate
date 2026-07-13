use std::collections::{BTreeMap, BTreeSet};

use crate::fixups::facts::{
    AstPath, BindingId, CountedLoopBound, CountedLoopIndexUse, CountedLoopStart, CountedLoopStep,
    CountedSliceLoopFact, FixupFacts, FunctionId, LoopId, LoopKind, PathSegment, SliceLoopAccess,
};
use crate::fixups::support::walk;
use crate::rust_ast::{BinOp, Expr, Ident, IndentStmt, Item, Program, RustValue, Stmt, Type};

pub(super) fn collect_facts(program: &Program, facts: &mut FixupFacts) {
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
            Stmt::Scope { body } => {
                self.collect_counted_loops(body, path, PathSegment::ScopeBody);
            }
            Stmt::LabeledBlock { body, .. } => {
                self.collect_counted_loops(body, path, PathSegment::LabeledBody);
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
            | Stmt::Match { .. }
            | Stmt::Break(_)
            | Stmt::Continue(_)
            | Stmt::While { .. }
            | Stmt::Block(_) => {}
        }
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

    fn collect_counted_loops(
        &mut self,
        body: &[IndentStmt],
        parent_path: &mut [PathSegment],
        body_segment: PathSegment,
    ) {
        for (index, pair) in body.windows(2).enumerate() {
            let Some(candidate) =
                self.loop_candidate(pair, parent_path, body_segment.clone(), index)
            else {
                continue;
            };
            self.facts.counted_slice_loops.push(candidate);
        }
    }

    fn loop_candidate(
        &self,
        pair: &[IndentStmt],
        parent_path: &[PathSegment],
        body_segment: PathSegment,
        index_stmt: usize,
    ) -> Option<CountedSliceLoopFact> {
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
        let Stmt::Loop {
            body: loop_body, ..
        } = &pair[1].stmt
        else {
            return None;
        };

        let range = canonical_loop_range(loop_body, index_name.as_str(), self)?;
        let mut index_path = parent_path.to_vec();
        index_path.push(body_segment.clone());
        index_path.push(PathSegment::Stmt(index_stmt));
        let index_path = AstPath(index_path);
        let index =
            self.facts
                .binding_by_local_path(self.function, index_name.as_str(), &index_path)?;

        let mut loop_path = parent_path.to_vec();
        loop_path.push(body_segment.clone());
        loop_path.push(PathSegment::Stmt(index_stmt + 1));
        let loop_path = AstPath(loop_path);
        let loop_id = self.loop_by_path(&loop_path)?;

        let mut body_path = loop_path.0.clone();
        body_path.push(PathSegment::LoopBody);
        let body_path = AstPath(body_path);

        let access = analyze_loop_body(
            &loop_body[range.body_start..range.increment_stmt],
            &Ident::new(index_name.as_str()),
            range.slice,
            &self.slices,
        )?;

        Some(CountedSliceLoopFact {
            function: self.function,
            loop_id,
            index,
            slice: range.slice,
            start: CountedLoopStart::Zero,
            bound: CountedLoopBound::SliceLen,
            step: CountedLoopStep::One,
            index_use: CountedLoopIndexUse::SliceIndexOnly,
            access,
            loop_path,
            body_path,
        })
    }

    fn loop_by_path(&self, path: &AstPath) -> Option<LoopId> {
        self.facts
            .loops
            .iter()
            .find(|loop_fact| {
                loop_fact.function == self.function
                    && loop_fact.kind == LoopKind::Loop
                    && &loop_fact.path == path
            })
            .map(|loop_fact| loop_fact.id)
    }
}

struct LoopRange {
    slice: BindingId,
    body_start: usize,
    increment_stmt: usize,
}

fn canonical_loop_range(
    body: &[IndentStmt],
    index_name: &str,
    collector: &Collector<'_>,
) -> Option<LoopRange> {
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
    let slice = match bound {
        Expr::Var(len) => collector.len_aliases.get(len.as_str()).copied()?,
        expr => collector.slice_len_source(expr)?,
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
    Some(LoopRange {
        slice,
        body_start: 1,
        increment_stmt,
    })
}

fn analyze_loop_body(
    body: &[IndentStmt],
    index_name: &Ident,
    expected_slice: BindingId,
    slices: &BTreeMap<String, BindingId>,
) -> Option<SliceLoopAccess> {
    let mut state = BodyAnalysis::new(index_name);
    for indent in body {
        if let Stmt::Let {
            name,
            init: Some(init),
            ..
        } = &indent.stmt
            && state.is_index_expr(init)
        {
            state.index_names.insert(name.to_string());
            continue;
        }
        if !analyze_stmt(&indent.stmt, slices, &mut state) {
            return None;
        }
    }
    if state.slices != BTreeSet::from([expected_slice]) {
        return None;
    }
    Some(if state.mutable {
        SliceLoopAccess::Mutable
    } else {
        SliceLoopAccess::ReadOnly
    })
}

struct BodyAnalysis {
    index_names: BTreeSet<String>,
    slices: BTreeSet<BindingId>,
    mutable: bool,
}

impl BodyAnalysis {
    fn new(index_name: &Ident) -> Self {
        Self {
            index_names: BTreeSet::from([index_name.as_str().to_string()]),
            slices: BTreeSet::new(),
            mutable: false,
        }
    }

    fn is_index_expr(&self, expr: &Expr) -> bool {
        matches!(peel_casts(expr), Expr::Var(name) if self.index_names.contains(name.as_str()))
    }
}

fn analyze_stmt(
    stmt: &Stmt,
    slices: &BTreeMap<String, BindingId>,
    state: &mut BodyAnalysis,
) -> bool {
    match stmt {
        Stmt::Let {
            init: Some(init), ..
        } => analyze_expr(init, AccessMode::Read, slices, state),
        Stmt::Let { init: None, .. } => true,
        Stmt::Assign { target, value } => {
            analyze_expr(target, AccessMode::Mutate, slices, state)
                && analyze_expr(value, AccessMode::Read, slices, state)
        }
        Stmt::CompoundAssign { target, value, .. } => {
            analyze_expr(target, AccessMode::Mutate, slices, state)
                && analyze_expr(value, AccessMode::Read, slices, state)
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => {
            analyze_expr(expr, AccessMode::Read, slices, state)
        }
        Stmt::Return(None) | Stmt::Break(_) | Stmt::Continue(_) => true,
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            analyze_expr(cond, AccessMode::Read, slices, state)
                && analyze_body(then_body, slices, state)
                && analyze_body(else_body, slices, state)
        }
        Stmt::LetIf {
            cond,
            then_body,
            then_value,
            else_body,
            else_value,
            ..
        } => {
            analyze_expr(cond, AccessMode::Read, slices, state)
                && analyze_body(then_body, slices, state)
                && analyze_expr(then_value, AccessMode::Read, slices, state)
                && analyze_body(else_body, slices, state)
                && analyze_expr(else_value, AccessMode::Read, slices, state)
        }
        Stmt::Loop { .. } | Stmt::While { .. } => false,
        Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => analyze_body(body, slices, state),
        Stmt::Unsafe { body } | Stmt::Block(body) => analyze_body(&body.stmts, slices, state),
        Stmt::Match { expr, arms } => {
            analyze_expr(expr, AccessMode::Read, slices, state)
                && arms
                    .iter()
                    .all(|arm| analyze_body(&arm.body, slices, state))
        }
    }
}

fn analyze_body(
    body: &[IndentStmt],
    slices: &BTreeMap<String, BindingId>,
    state: &mut BodyAnalysis,
) -> bool {
    for indent in body {
        if let Stmt::Let {
            name,
            init: Some(init),
            ..
        } = &indent.stmt
            && state.is_index_expr(init)
        {
            state.index_names.insert(name.to_string());
            continue;
        }
        if !analyze_stmt(&indent.stmt, slices, state) {
            return false;
        }
    }
    true
}

#[derive(Clone, Copy)]
enum AccessMode {
    Read,
    Mutate,
}

fn analyze_expr(
    expr: &Expr,
    mode: AccessMode,
    slices: &BTreeMap<String, BindingId>,
    state: &mut BodyAnalysis,
) -> bool {
    match expr {
        Expr::Var(name) => !state.index_names.contains(name.as_str()),
        Expr::Index { base, index } if state.is_index_expr(index) => {
            let Expr::Var(base) = &**base else {
                return false;
            };
            let Some(slice) = slices.get(base.as_str()).copied() else {
                return false;
            };
            if matches!(mode, AccessMode::Mutate) {
                state.mutable = true;
            }
            state.slices.insert(slice);
            true
        }
        Expr::Index { base, index } => {
            analyze_expr(base, AccessMode::Read, slices, state)
                && analyze_expr(index, AccessMode::Read, slices, state)
        }
        Expr::Ref { mutable, expr } | Expr::AddrOf { mutable, expr } => {
            let mode = if *mutable {
                AccessMode::Mutate
            } else {
                AccessMode::Read
            };
            analyze_expr(expr, mode, slices, state)
        }
        Expr::Unary { expr, .. } | Expr::Cast { expr, .. } => {
            analyze_expr(expr, mode, slices, state)
        }
        Expr::Binary { lhs, rhs, .. } => {
            analyze_expr(lhs, AccessMode::Read, slices, state)
                && analyze_expr(rhs, AccessMode::Read, slices, state)
        }
        Expr::Call { func, args } => {
            analyze_expr(func, AccessMode::Read, slices, state)
                && args
                    .iter()
                    .all(|arg| analyze_expr(arg, AccessMode::Read, slices, state))
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            analyze_expr(recv, AccessMode::Read, slices, state)
                && args
                    .iter()
                    .all(|arg| analyze_expr(arg, AccessMode::Read, slices, state))
        }
        Expr::Field { base, .. } | Expr::TupleField { base, .. } => {
            analyze_expr(base, mode, slices, state)
        }
        Expr::ArrayPtr { array, .. } => analyze_expr(array, AccessMode::Read, slices, state),
        Expr::StructLit { fields, .. } => fields
            .iter()
            .all(|(_, expr)| analyze_expr(expr, AccessMode::Read, slices, state)),
        Expr::ArrayLit(elems) | Expr::Macro { args: elems, .. } => elems
            .iter()
            .all(|expr| analyze_expr(expr, AccessMode::Read, slices, state)),
        Expr::ArrayRepeat { elem, len: _ } => analyze_expr(elem, AccessMode::Read, slices, state),
        Expr::Closure { body, .. } => analyze_expr(body, AccessMode::Read, slices, state),
        Expr::Match { expr, arms } => {
            analyze_expr(expr, AccessMode::Read, slices, state)
                && arms
                    .iter()
                    .all(|arm| analyze_expr(&arm.value, AccessMode::Read, slices, state))
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            analyze_expr(cond, AccessMode::Read, slices, state)
                && analyze_expr(then_expr, AccessMode::Read, slices, state)
                && analyze_expr(else_expr, AccessMode::Read, slices, state)
        }
        Expr::Block(block) | Expr::Unsafe(block) => analyze_body(&block.stmts, slices, state),
        Expr::AtomicRef { ptr, .. } | Expr::AtomicLoad { ptr, .. } => {
            analyze_expr(ptr, AccessMode::Read, slices, state)
        }
        Expr::AtomicStore { ptr, value, .. }
        | Expr::AtomicFetch { ptr, value, .. }
        | Expr::AtomicSwap { ptr, value, .. } => {
            analyze_expr(ptr, AccessMode::Mutate, slices, state)
                && analyze_expr(value, AccessMode::Read, slices, state)
        }
        Expr::AtomicCompareExchange {
            ptr,
            expected,
            desired,
            ..
        } => {
            analyze_expr(ptr, AccessMode::Mutate, slices, state)
                && analyze_expr(expected, AccessMode::Read, slices, state)
                && analyze_expr(desired, AccessMode::Read, slices, state)
        }
        Expr::Transmute { expr, .. } => analyze_expr(expr, AccessMode::Read, slices, state),
        Expr::CopyNonoverlapping { src, dst, .. } => {
            analyze_expr(src, AccessMode::Read, slices, state)
                && analyze_expr(dst, AccessMode::Mutate, slices, state)
        }
        Expr::PtrCopy {
            src, dst, count, ..
        } => {
            analyze_expr(src, AccessMode::Read, slices, state)
                && analyze_expr(dst, AccessMode::Mutate, slices, state)
                && analyze_expr(count, AccessMode::Read, slices, state)
        }
        Expr::WriteBytes { dst, val, count } => {
            analyze_expr(dst, AccessMode::Mutate, slices, state)
                && analyze_expr(val, AccessMode::Read, slices, state)
                && analyze_expr(count, AccessMode::Read, slices, state)
        }
        Expr::AtomicFence { .. } | Expr::Path(_) | Expr::Todo(_) => true,
        Expr::Value(_) | Expr::Str(_) | Expr::HexFloat(_) | Expr::ByteStr(_) => true,
    }
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
    use crate::rust_ast::{FnParam, Item, Program, UnaryOp};

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

    fn index(slice: &str, index: &str) -> Expr {
        Expr::Index {
            base: Box::new(var(slice)),
            index: Box::new(var(index)),
        }
    }

    fn loop_scope(body_stmt: Stmt, increment: Stmt) -> Stmt {
        Stmt::Scope {
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
                                stmt: increment,
                            },
                        ],
                    },
                },
            ],
        }
    }

    fn program(body_stmt: Stmt) -> Program {
        let mut f = func(
            vec![slice_param("items")],
            None,
            vec![
                temp("len", "i32", cast(len_call("items"), "i32")),
                loop_scope(
                    body_stmt,
                    Stmt::CompoundAssign {
                        target: var("i"),
                        op: BinOp::Add,
                        value: int(1),
                    },
                ),
            ],
        );
        f.name = "sum".into();
        Program {
            items: vec![Item::Fn(f)],
        }
    }

    #[test]
    fn records_read_only_counted_slice_loop() {
        let facts = analyze_collect(&program(temp("x", "i32", index("items", "i"))));

        assert_eq!(facts.counted_slice_loops.len(), 1);
        let fact = &facts.counted_slice_loops[0];
        assert_eq!(fact.start, CountedLoopStart::Zero);
        assert_eq!(fact.bound, CountedLoopBound::SliceLen);
        assert_eq!(fact.step, CountedLoopStep::One);
        assert_eq!(fact.index_use, CountedLoopIndexUse::SliceIndexOnly);
        assert_eq!(fact.access, SliceLoopAccess::ReadOnly);
    }

    #[test]
    fn records_mutable_counted_slice_loop() {
        let facts = analyze_collect(&program(Stmt::Assign {
            target: index("items", "i"),
            value: int(7),
        }));

        assert_eq!(facts.counted_slice_loops.len(), 1);
        assert_eq!(
            facts.counted_slice_loops[0].access,
            SliceLoopAccess::Mutable
        );
    }

    #[test]
    fn accepts_trivial_index_aliases() {
        let facts = analyze_collect(&program(Stmt::Scope {
            body: vec![
                IndentStmt {
                    depth: 4,
                    stmt: temp("idx", "i32", var("i")),
                },
                IndentStmt {
                    depth: 4,
                    stmt: temp("x", "i32", index("items", "idx")),
                },
            ],
        }));

        assert_eq!(facts.counted_slice_loops.len(), 1);
    }

    #[test]
    fn rejects_escaping_indices() {
        let facts = analyze_collect(&program(Stmt::Expr(call("observe", vec![var("i")]))));

        assert!(facts.counted_slice_loops.is_empty());
    }

    #[test]
    fn rejects_non_unit_steps() {
        let mut f = func(
            vec![slice_param("items")],
            None,
            vec![
                temp("len", "i32", cast(len_call("items"), "i32")),
                loop_scope(
                    temp("x", "i32", index("items", "i")),
                    Stmt::CompoundAssign {
                        target: var("i"),
                        op: BinOp::Add,
                        value: int(2),
                    },
                ),
            ],
        );
        f.name = "sum".into();
        let facts = analyze_collect(&Program {
            items: vec![Item::Fn(f)],
        });

        assert!(facts.counted_slice_loops.is_empty());
    }

    #[test]
    fn rejects_multiple_indexed_slices() {
        let mut f = func(
            vec![slice_param("items"), slice_param("other")],
            None,
            vec![
                temp("len", "i32", cast(len_call("items"), "i32")),
                loop_scope(
                    Stmt::Scope {
                        body: vec![
                            IndentStmt {
                                depth: 4,
                                stmt: temp("x", "i32", index("items", "i")),
                            },
                            IndentStmt {
                                depth: 4,
                                stmt: temp("y", "i32", index("other", "i")),
                            },
                        ],
                    },
                    Stmt::CompoundAssign {
                        target: var("i"),
                        op: BinOp::Add,
                        value: int(1),
                    },
                ),
            ],
        );
        f.name = "sum".into();
        let facts = analyze_collect(&Program {
            items: vec![Item::Fn(f)],
        });

        assert!(facts.counted_slice_loops.is_empty());
    }
}
