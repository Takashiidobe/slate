use std::collections::{BTreeMap, BTreeSet};

use crate::fixups::facts::walk::{exprs, exprs_any};
use crate::fixups::facts::{AstPath, PathSegment};
use crate::fixups::idents::stmt_ident_count;
use crate::fixups::support::walk;
use crate::rust_ast::{Expr, FnDef, Ident, IndentStmt, Prim, RustValue, Stmt, Type, UnaryOp};

#[derive(Clone)]
pub(super) struct Plan {
    pub(super) path: AstPath,
    pub(super) pointer_name: String,
    pub(super) offset_name: String,
    pub(super) slice_name: String,
    pub(super) index_name: String,
    pub(super) mutable: bool,
}

pub(super) fn plans_for_function(function: &FnDef) -> Vec<Plan> {
    let mut collector = Collector {
        params: function.params.iter().map(|p| p.name.clone()).collect(),
        slices: BTreeSet::new(),
        len_aliases: BTreeMap::new(),
        index_aliases: BTreeMap::new(),
        range_by_index: BTreeMap::new(),
        pointer_views: BTreeMap::new(),
        plans: Vec::new(),
    };
    for param in &function.params {
        if slice_elem_ty(&param.ty).is_some() {
            collector.slices.insert(param.name.clone());
        }
    }
    collector.body(&function.body, &mut Vec::new());
    collector.plans
}

struct Collector {
    params: BTreeSet<String>,
    slices: BTreeSet<String>,
    len_aliases: BTreeMap<String, String>,
    index_aliases: BTreeMap<String, String>,
    range_by_index: BTreeMap<String, String>,
    pointer_views: BTreeMap<String, (String, bool)>,
    plans: Vec<Plan>,
}

impl Collector {
    fn body(&mut self, body: &[IndentStmt], path: &mut Vec<PathSegment>) {
        for (index, indent) in body.iter().enumerate() {
            walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
                self.stmt(&indent.stmt, path);
                walk::nested_bodies_with_path(&indent.stmt, path, &mut |nested, path| {
                    self.body(nested, path);
                });
            });
        }
    }

    fn stmt(&mut self, stmt: &Stmt, path: &[PathSegment]) {
        match stmt {
            Stmt::Let {
                name,
                ty,
                init: Some(init),
                ..
            } => self.collect_let(name, ty.as_ref(), init, path),
            Stmt::Loop { body, .. } => self.collect_counted_loops(body),
            Stmt::For { pat, iter, body } => {
                self.collect_for_range(pat, iter, body);
                self.collect_expr_offsets(path, iter);
                self.collect_counted_loops(body);
            }
            Stmt::While { cond, .. } => self.collect_expr_offsets(path, cond),
            Stmt::Unsafe { .. } | Stmt::Block(_) => {}
            Stmt::LetIf {
                cond,
                then_value,
                else_value,
                ..
            } => {
                self.collect_expr_offsets(path, cond);
                self.collect_expr_offsets(path, then_value);
                self.collect_expr_offsets(path, else_value);
            }
            Stmt::If { cond, .. } => self.collect_expr_offsets(path, cond),
            Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
                self.collect_expr_offsets(path, target);
                self.collect_expr_offsets(path, value);
            }
            Stmt::Expr(expr) | Stmt::Return(Some(expr)) => self.collect_expr_offsets(path, expr),
            Stmt::Match { expr, .. } => self.collect_expr_offsets(path, expr),
            Stmt::Scope { body } => self.collect_counted_loops(body),
            Stmt::LabeledBlock { body, .. } => self.collect_counted_loops(body),
            Stmt::Let { init: None, .. }
            | Stmt::InlineAsm(_)
            | Stmt::Return(None)
            | Stmt::Break(_)
            | Stmt::Continue(_) => {}
        }
    }

    fn collect_let(&mut self, name: &str, ty: Option<&Type>, init: &Expr, path: &[PathSegment]) {
        if ty.and_then(slice_elem_ty).is_some() {
            self.slices.insert(name.to_string());
        }
        if let Some(slice) = self.slice_len_source(init) {
            self.len_aliases.insert(name.to_string(), slice);
        }
        if let Some((slice_name, mutable)) = slice_pointer_source(init)
            && self.slices.contains(slice_name.as_str())
        {
            self.pointer_views
                .insert(name.to_string(), (slice_name.as_str().to_string(), mutable));
        }
        if let Expr::Var(source) = init {
            self.index_aliases
                .insert(name.to_string(), source.as_str().to_string());
        }
        self.collect_expr_offsets(path, init);
    }

    fn slice_len_source(&self, expr: &Expr) -> Option<String> {
        let Expr::MethodCall { recv, method, args } = peel_casts(expr) else {
            return None;
        };
        if method != "len" || !args.is_empty() {
            return None;
        }
        let Expr::Var(name) = &**recv else {
            return None;
        };
        self.slices
            .contains(name.as_str())
            .then(|| name.as_str().to_string())
    }

    fn collect_counted_loops(&mut self, body: &[IndentStmt]) {
        for pair in body.windows(2) {
            let Stmt::Let {
                name: index_name,
                init: Some(init),
                ..
            } = &pair[0].stmt
            else {
                continue;
            };
            if !is_zero(init) {
                continue;
            }
            let Stmt::Loop {
                body: loop_body, ..
            } = &pair[1].stmt
            else {
                continue;
            };
            if let Some(slice) = loop_range_slice(
                loop_body,
                index_name.as_str(),
                &self.slices,
                &self.len_aliases,
                &self.params,
            ) {
                self.range_by_index.insert(index_name.to_string(), slice);
            }
        }
    }

    fn collect_for_range(&mut self, index_name: &str, iter: &Expr, body: &[IndentStmt]) {
        if let Some(slice) = for_range_slice(
            iter,
            body,
            index_name,
            &self.slices,
            &self.len_aliases,
            &self.params,
        ) {
            self.range_by_index.insert(index_name.to_string(), slice);
        }
    }

    fn collect_expr_offsets(&mut self, path: &[PathSegment], expr: &Expr) {
        exprs(expr, &mut |expr| {
            let Some((pointer, offset_name)) = pointer_offset(expr) else {
                return;
            };
            let Some((slice_name, mutable)) = self.pointer_views.get(pointer) else {
                return;
            };
            let ranged_index = self
                .index_aliases
                .get(offset_name)
                .cloned()
                .unwrap_or_else(|| offset_name.to_string());
            if self.range_by_index.get(&ranged_index) != Some(slice_name) {
                return;
            }
            self.plans.push(Plan {
                path: AstPath(path.to_vec()),
                pointer_name: pointer.to_string(),
                offset_name: offset_name.to_string(),
                slice_name: slice_name.clone(),
                index_name: ranged_index,
                mutable: *mutable,
            });
        });
    }
}

pub(super) fn slice_elem_ty(ty: &Type) -> Option<&Type> {
    match ty {
        Type::Ref { inner, .. } => match &**inner {
            Type::Slice(elem) => Some(elem),
            _ => None,
        },
        Type::Slice(elem) => Some(elem),
        _ => None,
    }
}

pub(super) fn slice_pointer_source(expr: &Expr) -> Option<(&Ident, bool)> {
    let Expr::MethodCall { recv, method, args } = expr else {
        return None;
    };
    if !args.is_empty() {
        return None;
    }
    let mutable = match method.as_str() {
        "as_ptr" => false,
        "as_mut_ptr" => true,
        _ => return None,
    };
    let Expr::Var(name) = &**recv else {
        return None;
    };
    Some((name, mutable))
}

pub(super) fn pointer_offset(expr: &Expr) -> Option<(&str, &str)> {
    let Expr::MethodCall { recv, method, args } = expr else {
        return None;
    };
    if method != "offset" || args.len() != 1 {
        return None;
    }
    let Expr::Var(pointer) = recv.as_ref() else {
        return None;
    };
    let Expr::Var(offset_index) = peel_casts(&args[0]) else {
        return None;
    };
    Some((pointer.as_str(), offset_index.as_str()))
}

pub(super) fn peel_casts(expr: &Expr) -> &Expr {
    match expr {
        Expr::Cast { expr, .. } => peel_casts(expr),
        _ => expr,
    }
}

pub(super) fn is_zero(expr: &Expr) -> bool {
    integer_value(expr) == Some(0)
}

fn integer_value(expr: &Expr) -> Option<i128> {
    match expr {
        Expr::Value(RustValue::I64(n)) => Some(i128::from(*n)),
        Expr::Value(RustValue::I128(n)) => Some(*n),
        Expr::Cast { expr, .. } => integer_value(expr),
        _ => None,
    }
}

/// `for i in 0..bound { .. }` variant of `loop_range_slice`: CIR sometimes
/// lowers a C `for` loop directly into this idiomatic Rust `for` form before
/// any fixup runs (not just via the later `range_loop` fixup), so this shape
/// is live at the point `slice_index` itself runs.
pub(super) fn for_range_slice(
    iter: &Expr,
    body: &[IndentStmt],
    index_name: &str,
    slices: &BTreeSet<String>,
    len_aliases: &BTreeMap<String, String>,
    params: &BTreeSet<String>,
) -> Option<String> {
    let Expr::Range { start, end } = iter else {
        return None;
    };
    if integer_value(start) != Some(0) {
        return None;
    }
    if !is_supported_range_bound(end, len_aliases, params) {
        return None;
    }
    indexed_slice_scan(body, index_name, slices)
}

/// `let i = 0; loop { if !(i < bound) { break; } .. i += 1; }` variant: the
/// pre-`range_loop` goto-derived counted-loop shape. `bound` is either a
/// `.len()` alias (tracked in `len_aliases`) or, when it's a bare parameter
/// or a literal constant, provable only by finding a single slice the loop
/// body itself indexes by `index_name` (`indexed_slice_scan`).
pub(super) fn loop_range_slice(
    loop_body: &[IndentStmt],
    index_name: &str,
    slices: &BTreeSet<String>,
    len_aliases: &BTreeMap<String, String>,
    params: &BTreeSet<String>,
) -> Option<String> {
    let bound = canonical_loop_bound(loop_body, index_name)?;
    if let Expr::Var(len) = bound {
        if let Some(slice) = len_aliases.get(len.as_str()) {
            return Some(slice.clone());
        }
        return params
            .contains(len.as_str())
            .then(|| indexed_slice_scan(loop_body, index_name, slices))
            .flatten();
    }
    integer_value(bound)
        .is_some()
        .then(|| indexed_slice_scan(loop_body, index_name, slices))
        .flatten()
}

fn canonical_loop_bound<'a>(loop_body: &'a [IndentStmt], index_name: &str) -> Option<&'a Expr> {
    let first = loop_body.first()?;
    let Stmt::If {
        cond, then_body, ..
    } = &first.stmt
    else {
        return None;
    };
    if !is_break_only(then_body) {
        return None;
    }
    let (index, bound) = negated_less_than_bound(cond)?;
    (index.as_str() == index_name).then_some(bound)
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

fn negated_less_than_bound(expr: &Expr) -> Option<(&Ident, &Expr)> {
    let Expr::Unary {
        op: UnaryOp::Not,
        expr,
    } = expr
    else {
        return None;
    };
    let Expr::Binary {
        op: crate::rust_ast::BinOp::Lt,
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

fn indexed_slice_scan(
    body: &[IndentStmt],
    index_name: &str,
    slices: &BTreeSet<String>,
) -> Option<String> {
    let mut local = LoopSliceUse::new(index_name, slices);
    for indent in body {
        if local.stmt(&indent.stmt) {
            return local.found;
        }
    }
    local.found
}

fn is_supported_range_bound(
    expr: &Expr,
    len_aliases: &BTreeMap<String, String>,
    params: &BTreeSet<String>,
) -> bool {
    if integer_value(expr).is_some() {
        return true;
    }
    let Expr::Var(name) = peel_casts(expr) else {
        return false;
    };
    len_aliases.contains_key(name.as_str()) || params.contains(name.as_str())
}

struct LoopSliceUse<'a> {
    slices: &'a BTreeSet<String>,
    pointer_aliases: BTreeMap<String, String>,
    index_aliases: Vec<String>,
    found: Option<String>,
}

impl<'a> LoopSliceUse<'a> {
    fn new(index_name: &'a str, slices: &'a BTreeSet<String>) -> Self {
        Self {
            slices,
            pointer_aliases: BTreeMap::new(),
            index_aliases: vec![index_name.to_string()],
            found: None,
        }
    }

    fn stmt(&mut self, stmt: &Stmt) -> bool {
        match stmt {
            Stmt::Let {
                name,
                init: Some(init),
                ..
            } => {
                if self.expr(init) {
                    return true;
                }
                if let Some(slice) = self.slice_pointer_source(init) {
                    self.pointer_aliases.insert(name.clone(), slice);
                }
                if self.is_index_expr(init) {
                    self.index_aliases.push(name.clone());
                }
                false
            }
            Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
                self.expr(target) || self.expr(value)
            }
            Stmt::Expr(expr) | Stmt::Return(Some(expr)) => self.expr(expr),
            Stmt::If {
                cond,
                then_body,
                else_body,
            } => self.expr(cond) || self.body(then_body) || self.body(else_body),
            Stmt::LetIf {
                cond,
                then_body,
                then_value,
                else_body,
                else_value,
                ..
            } => {
                self.expr(cond)
                    || self.body(then_body)
                    || self.expr(then_value)
                    || self.body(else_body)
                    || self.expr(else_value)
            }
            Stmt::Loop { body, .. }
            | Stmt::For { body, .. }
            | Stmt::Scope { body }
            | Stmt::LabeledBlock { body, .. } => self.body(body),
            Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
                self.body(&body.stmts) || body.tail.as_deref().is_some_and(|expr| self.expr(expr))
            }
            Stmt::Match { expr, arms } => {
                self.expr(expr) || arms.iter().any(|arm| self.body(&arm.body))
            }
            Stmt::Let { init: None, .. }
            | Stmt::InlineAsm(_)
            | Stmt::Return(None)
            | Stmt::Break(_)
            | Stmt::Continue(_) => false,
        }
    }

    fn body(&mut self, body: &[IndentStmt]) -> bool {
        body.iter().any(|indent| self.stmt(&indent.stmt))
    }

    fn expr(&mut self, expr: &Expr) -> bool {
        if let Some(slice) = self.offset_slice(expr) {
            self.found = Some(slice);
            return true;
        }
        exprs_any(expr, &mut |expr| {
            if let Some(slice) = self.offset_slice(expr) {
                self.found = Some(slice);
                true
            } else {
                false
            }
        })
    }

    fn slice_pointer_source(&self, expr: &Expr) -> Option<String> {
        let (slice_name, _) = slice_pointer_source(peel_casts(expr))?;
        self.slices
            .contains(slice_name.as_str())
            .then(|| slice_name.as_str().to_string())
    }

    fn offset_slice(&self, expr: &Expr) -> Option<String> {
        let Expr::MethodCall { recv, method, args } = peel_casts(expr) else {
            return None;
        };
        if !matches!(
            method.as_str(),
            "offset" | "add" | "wrapping_offset" | "wrapping_add"
        ) || !matches!(args.as_slice(), [arg] if self.is_index_expr(arg))
        {
            return None;
        }
        let Expr::Var(pointer) = peel_casts(recv) else {
            return None;
        };
        self.pointer_aliases.get(pointer.as_str()).cloned()
    }

    fn is_index_expr(&self, expr: &Expr) -> bool {
        matches!(peel_casts(expr), Expr::Var(name) if self.index_aliases.iter().any(|alias| alias == name.as_str()))
    }
}

pub(super) fn rewrite_body(
    body: &mut [IndentStmt],
    plans: &[Plan],
    path: &mut Vec<PathSegment>,
) -> bool {
    let mut changed = false;
    for (index, indent) in body.iter_mut().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            changed |= rewrite_stmt(&mut indent.stmt, plans, path);
            walk::nested_bodies_mut_with_path(&mut indent.stmt, path, &mut |body, path| {
                changed |= rewrite_body(body, plans, path);
            });
            changed |= unwrap_safe_singleton_unsafe(&mut indent.stmt);
        });
    }
    changed
}

fn rewrite_stmt(stmt: &mut Stmt, plans: &[Plan], path: &[PathSegment]) -> bool {
    let mut changed = false;
    let stmt_plans = plans_for_path(plans, path);
    if stmt_plans.is_empty() {
        return false;
    }
    match stmt {
        Stmt::Let {
            init: Some(init), ..
        } => changed |= rewrite_expr(init, &stmt_plans, false),
        Stmt::Assign { target, value } => {
            changed |= rewrite_expr(target, &stmt_plans, true);
            changed |= rewrite_expr(value, &stmt_plans, false);
        }
        Stmt::CompoundAssign { target, value, .. } => {
            changed |= rewrite_expr(target, &stmt_plans, true);
            changed |= rewrite_expr(value, &stmt_plans, false);
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => {
            changed |= rewrite_expr(expr, &stmt_plans, false);
        }
        Stmt::If { cond, .. } | Stmt::While { cond, .. } => {
            changed |= rewrite_expr(cond, &stmt_plans, false);
        }
        Stmt::For { iter, .. } => {
            changed |= rewrite_expr(iter, &stmt_plans, false);
        }
        Stmt::LetIf {
            cond,
            then_value,
            else_value,
            ..
        } => {
            changed |= rewrite_expr(cond, &stmt_plans, false);
            changed |= rewrite_expr(then_value, &stmt_plans, false);
            changed |= rewrite_expr(else_value, &stmt_plans, false);
        }
        Stmt::Match { expr, .. } => changed |= rewrite_expr(expr, &stmt_plans, false),
        Stmt::Let { init: None, .. }
        | Stmt::InlineAsm(_)
        | Stmt::Loop { .. }
        | Stmt::Scope { .. }
        | Stmt::LabeledBlock { .. }
        | Stmt::Unsafe { .. }
        | Stmt::Block(_)
        | Stmt::Return(None)
        | Stmt::Break(_)
        | Stmt::Continue(_) => {}
    }
    changed
}

fn plans_for_path<'a>(plans: &'a [Plan], path: &[PathSegment]) -> Vec<&'a Plan> {
    plans
        .iter()
        .filter(|plan| plan.path == AstPath(path.to_vec()))
        .collect()
}

fn rewrite_expr(expr: &mut Expr, plans: &[&Plan], write_target: bool) -> bool {
    let mut changed = false;
    walk::exprs_mut_with(expr, &mut |expr| {
        if let Some(replacement) = indexed_expr(expr, plans, write_target) {
            *expr = replacement;
            changed = true;
            return false;
        }
        true
    });
    changed
}

fn indexed_expr(expr: &Expr, plans: &[&Plan], write_target: bool) -> Option<Expr> {
    let offset = deref_offset(expr)?;
    let pointer_name = receiver_var(offset.recv)?;
    let offset_name = offset_arg_var(offset.arg)?;
    let plan = plans.iter().find(|plan| {
        plan.pointer_name == pointer_name
            && plan.offset_name == offset_name
            && (!write_target || plan.mutable)
    })?;
    Some(Expr::Index {
        base: Box::new(Expr::Var(Ident::from(plan.slice_name.as_str()))),
        index: Box::new(Expr::Cast {
            expr: Box::new(Expr::Var(Ident::from(plan.index_name.as_str()))),
            ty: Type::Prim(Prim::Usize),
        }),
    })
}

struct OffsetParts<'a> {
    recv: &'a Expr,
    arg: &'a Expr,
}

fn deref_offset(expr: &Expr) -> Option<OffsetParts<'_>> {
    match expr {
        Expr::Unsafe(block) => deref_offset(block.tail.as_deref()?),
        Expr::Unary {
            op: UnaryOp::Deref,
            expr,
        } => offset_parts(peel_unsafe(expr)),
        _ => None,
    }
}

fn peel_unsafe(expr: &Expr) -> &Expr {
    match expr {
        Expr::Unsafe(block) => block.tail.as_deref().map_or(expr, peel_unsafe),
        _ => expr,
    }
}

fn offset_parts(expr: &Expr) -> Option<OffsetParts<'_>> {
    let Expr::MethodCall { recv, method, args } = expr else {
        return None;
    };
    if method != "offset" || args.len() != 1 {
        return None;
    }
    Some(OffsetParts {
        recv,
        arg: &args[0],
    })
}

fn receiver_var(expr: &Expr) -> Option<&str> {
    let Expr::Var(name) = expr else {
        return None;
    };
    Some(name.as_str())
}

fn offset_arg_var(expr: &Expr) -> Option<&str> {
    match expr {
        Expr::Cast { expr, .. } => offset_arg_var(expr),
        Expr::Var(name) => Some(name.as_str()),
        _ => None,
    }
}

fn unwrap_safe_singleton_unsafe(stmt: &mut Stmt) -> bool {
    let Stmt::Unsafe { body } = stmt else {
        return false;
    };
    if body.tail.is_some() || body.stmts.len() != 1 {
        return false;
    }
    let inner = body.stmts[0].stmt.clone();
    if !is_safe_slice_index_stmt(&inner) {
        return false;
    }
    *stmt = inner;
    true
}

fn is_safe_slice_index_stmt(stmt: &Stmt) -> bool {
    match stmt {
        Stmt::Assign { target, value } => is_slice_index(target) && !contains_unsafe(value),
        Stmt::CompoundAssign { target, value, .. } => {
            is_slice_index(target) && !contains_unsafe(value)
        }
        _ => false,
    }
}

fn is_slice_index(expr: &Expr) -> bool {
    matches!(expr, Expr::Index { .. })
}

fn contains_unsafe(expr: &Expr) -> bool {
    let mut found = false;
    walk::expr_any(expr, &mut |expr| {
        if matches!(
            expr,
            Expr::Unsafe(_)
                | Expr::Unary {
                    op: UnaryOp::Deref,
                    ..
                }
        ) {
            found = true;
        }
        found
    });
    found
}

pub(super) fn prune_dead_fact_temps(body: &mut Vec<IndentStmt>, plans: &[Plan]) -> bool {
    let removable = removable_temp_names(plans);
    prune_dead_fact_temps_in_body(body, &removable)
}

fn prune_dead_fact_temps_in_body(body: &mut Vec<IndentStmt>, removable: &BTreeSet<String>) -> bool {
    let mut changed = false;
    for indent in body.iter_mut() {
        walk::nested_body_vecs_mut_with_path(&mut indent.stmt, &mut Vec::new(), &mut |body, _| {
            changed |= prune_dead_fact_temps_in_body(body, removable);
        });
    }
    let mut index = 0;
    while index < body.len() {
        let remove = match &body[index].stmt {
            Stmt::Let {
                name,
                init: Some(_),
                ..
            } if removable.contains(name) => body[index + 1..]
                .iter()
                .all(|indent| stmt_ident_count(&indent.stmt, name) == 0),
            _ => false,
        };
        if remove {
            body.remove(index);
            changed = true;
        } else {
            index += 1;
        }
    }
    changed
}

fn removable_temp_names(plans: &[Plan]) -> BTreeSet<String> {
    plans
        .iter()
        .flat_map(|plan| [&plan.pointer_name, &plan.offset_name])
        .filter(|name| is_temp_name(name))
        .cloned()
        .collect()
}

pub(super) fn is_temp_name(name: &str) -> bool {
    name.strip_prefix("_v")
        .is_some_and(|rest| !rest.is_empty() && rest.bytes().all(|byte| byte.is_ascii_digit()))
}
