use std::collections::{BTreeMap, BTreeSet};

use crate::backend::facts::walk;
use crate::backend::facts::{
    self, AstPath, BindingFact, BindingId, BindingKind, ConstValue, FunctionId, PathSegment, Site,
    ValueFact, ValueSubject,
};
use crate::backend::rust_ast::{
    BinOp, Block, Expr, FnDef, IndentStmt, Pattern, Prim, RustValue, Stmt, Type, UnaryOp,
};
/// Values for one function's body, independent of any other function's
/// facts - the entry point `slate-04q.75.56.8` (incremental facts) needs to
/// re-derive one function's values without a whole-program walk.
pub(in crate::backend) fn collect_for_function<'db>(
    function: FunctionId<'db>,
    f: &FnDef,
    bindings: &[BindingFact<'db>],
) -> Vec<ValueFact<'db>> {
    let mut collector = Collector::new(function, bindings);
    collector.enter_root_scope();
    collector.body(&f.body, &mut Vec::new(), false);
    collector.values
}

struct Collector<'db, 'a> {
    function: FunctionId<'db>,
    bindings: &'a [BindingFact<'db>],
    scopes: Vec<BTreeMap<String, Option<BindingId<'db>>>>,
    values_by_binding: BTreeMap<BindingId<'db>, BTreeSet<ConstValue>>,
    values: Vec<ValueFact<'db>>,
}

impl<'db, 'a> Collector<'db, 'a> {
    fn new(function: FunctionId<'db>, bindings: &'a [BindingFact<'db>]) -> Self {
        Self {
            function,
            bindings,
            scopes: Vec::new(),
            values_by_binding: BTreeMap::new(),
            values: Vec::new(),
        }
    }

    fn enter_root_scope(&mut self) {
        self.scopes.push(BTreeMap::new());
        let params: Vec<_> = self
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
                let values = init.as_ref().map(|init| self.expr(init, path));
                self.define_local(name, path, values.unwrap_or_default());
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
                self.define_local(name, path, BTreeSet::new());
            }
            Stmt::Assign { target, value } => {
                let values = self.expr(value, path);
                self.assign(target, values, path);
            }
            Stmt::CompoundAssign { target, value, .. } => {
                self.expr(value, path);
                self.kill_target(target, path);
            }
            Stmt::Expr(expr) | Stmt::Return(Some(expr)) => {
                self.expr(expr, path);
            }
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
            Stmt::For { pat, iter, body } => {
                self.expr(iter, path);
                let binding = self.local_binding(pat, path);
                walk::with_path_segment(path, PathSegment::ForBody, |path| {
                    self.scopes.push(BTreeMap::new());
                    self.bind(pat.to_string(), binding);
                    self.body(body, path, false);
                    self.scopes.pop();
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
            Stmt::InlineAsm(_) => {}
        }
    }

    fn expr(&mut self, expr: &Expr, path: &mut Vec<PathSegment>) -> BTreeSet<ConstValue> {
        let values = self.expr_inner(expr, path);
        self.record_expr(path, &values);
        values
    }

    fn child_expr(
        &mut self,
        expr: &Expr,
        path: &mut Vec<PathSegment>,
        index: usize,
    ) -> BTreeSet<ConstValue> {
        walk::with_path_segment(path, PathSegment::Expr(index), |path| self.expr(expr, path))
    }

    fn expr_inner(&mut self, expr: &Expr, path: &mut Vec<PathSegment>) -> BTreeSet<ConstValue> {
        match expr {
            Expr::Value(value) => values_for_rust_value(value),
            Expr::Str(s) => values_for_string(s),
            Expr::ByteStr(bytes) => values_for_bytes(bytes),
            Expr::CStr(bytes) => BTreeSet::from([ConstValue::CStringBytes(bytes.clone())]),
            Expr::Var(name) => self
                .binding_for_name(name.as_str())
                .and_then(|binding| self.values_by_binding.get(&binding).cloned())
                .unwrap_or_default(),
            Expr::Unary { op, expr } => {
                let child = self.child_expr(expr, path, 0);
                fold_unary(*op, &child).unwrap_or_default()
            }
            Expr::Binary { op, lhs, rhs } => {
                let lhs = self.child_expr(lhs, path, 0);
                let rhs = self.child_expr(rhs, path, 1);
                fold_binary(*op, &lhs, &rhs).unwrap_or_default()
            }
            Expr::Range { start, end } => {
                self.child_expr(start, path, 0);
                self.child_expr(end, path, 1);
                BTreeSet::new()
            }
            Expr::Cast { expr, ty } => {
                let child = self.child_expr(expr, path, 0);
                fold_cast(&child, ty)
            }
            Expr::ArrayRepeat { elem, len } => {
                let values = self.child_expr(elem, path, 0);
                let mut out = BTreeSet::from([ConstValue::ArrayLength(*len)]);
                if values.contains(&ConstValue::Zero) {
                    out.insert(ConstValue::Zero);
                }
                out
            }
            Expr::ArrayLit(elems) => {
                for (index, elem) in elems.iter().enumerate() {
                    self.child_expr(elem, path, index);
                }
                BTreeSet::from([ConstValue::ArrayLength(elems.len())])
            }
            Expr::VecRepeat { elem, len } => {
                self.child_expr(elem, path, 0);
                self.child_expr(len, path, 1)
            }
            Expr::VecLit(elems) => {
                for (index, elem) in elems.iter().enumerate() {
                    self.child_expr(elem, path, index);
                }
                BTreeSet::new()
            }
            Expr::Call { func, args, .. } => {
                self.child_expr(func, path, 0);
                for (index, arg) in args.iter().enumerate() {
                    self.child_expr(arg, path, index + 1);
                }
                BTreeSet::new()
            }
            Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
                self.child_expr(recv, path, 0);
                for (index, arg) in args.iter().enumerate() {
                    self.child_expr(arg, path, index + 1);
                }
                BTreeSet::new()
            }
            Expr::StructLit { fields, .. } => {
                let mut all_zero = !fields.is_empty();
                for (index, (_, value)) in fields.iter().enumerate() {
                    let values = self.child_expr(value, path, index);
                    all_zero &= values.contains(&ConstValue::Zero);
                }
                if all_zero {
                    BTreeSet::from([ConstValue::Zero])
                } else {
                    BTreeSet::new()
                }
            }
            Expr::TupleStructLit { fields, .. } => {
                let mut all_zero = !fields.is_empty();
                for (index, value) in fields.iter().enumerate() {
                    let values = self.child_expr(value, path, index);
                    all_zero &= values.contains(&ConstValue::Zero);
                }
                if all_zero {
                    BTreeSet::from([ConstValue::Zero])
                } else {
                    BTreeSet::new()
                }
            }
            Expr::Macro { args, .. } => {
                for (index, arg) in args.iter().enumerate() {
                    self.child_expr(arg, path, index);
                }
                BTreeSet::new()
            }
            Expr::Match { expr, arms } => {
                self.child_expr(expr, path, 0);
                for (index, arm) in arms.iter().enumerate() {
                    self.child_expr(&arm.value, path, index + 1);
                }
                BTreeSet::new()
            }
            Expr::If {
                cond,
                then_expr,
                else_expr,
            } => {
                self.child_expr(cond, path, 0);
                self.child_expr(then_expr, path, 1);
                self.child_expr(else_expr, path, 2);
                BTreeSet::new()
            }
            Expr::Field { base, .. }
            | Expr::TupleField { base, .. }
            | Expr::ArrayPtr { array: base, .. }
            | Expr::Ref { expr: base, .. }
            | Expr::AddrOf { expr: base, .. }
            | Expr::Transmute { expr: base, .. } => {
                self.child_expr(base, path, 0);
                BTreeSet::new()
            }
            Expr::Index { base, index } => {
                self.child_expr(base, path, 0);
                self.child_expr(index, path, 1);
                BTreeSet::new()
            }
            Expr::Closure { body, params } => {
                self.scopes.push(BTreeMap::new());
                for param in params {
                    self.bind(param.to_string(), None);
                }
                self.child_expr(body, path, 0);
                self.scopes.pop();
                BTreeSet::new()
            }
            Expr::Block(block) => {
                walk::with_path_segment(path, PathSegment::BlockBody, |path| {
                    self.block(block, path)
                });
                BTreeSet::new()
            }
            Expr::Unsafe(block) => {
                walk::with_path_segment(path, PathSegment::UnsafeBody, |path| {
                    self.block(block, path)
                });
                BTreeSet::new()
            }
            Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. } => {
                if let Some(ptr) = place.ptr_expr() {
                    self.child_expr(ptr, path, 0);
                }
                BTreeSet::new()
            }
            Expr::AtomicStore { place, value, .. }
            | Expr::AtomicFetch { place, value, .. }
            | Expr::AtomicSwap { place, value, .. } => {
                if let Some(ptr) = place.ptr_expr() {
                    self.child_expr(ptr, path, 0);
                }
                self.child_expr(value, path, 1);
                BTreeSet::new()
            }
            Expr::AtomicNew { value, .. } => {
                self.child_expr(value, path, 0);
                BTreeSet::new()
            }
            Expr::AtomicCompareExchange {
                place,
                expected,
                desired,
                ..
            } => {
                if let Some(ptr) = place.ptr_expr() {
                    self.child_expr(ptr, path, 0);
                }
                self.child_expr(expected, path, 1);
                self.child_expr(desired, path, 2);
                BTreeSet::new()
            }
            Expr::CopyNonoverlapping { src, dst, .. } => {
                self.child_expr(src, path, 0);
                self.child_expr(dst, path, 1);
                BTreeSet::new()
            }
            Expr::PtrCopy {
                src, dst, count, ..
            } => {
                self.child_expr(src, path, 0);
                self.child_expr(dst, path, 1);
                self.child_expr(count, path, 2);
                BTreeSet::new()
            }
            Expr::WriteBytes { dst, val, count } => {
                self.child_expr(dst, path, 0);
                self.child_expr(val, path, 1);
                self.child_expr(count, path, 2);
                BTreeSet::new()
            }
            Expr::HexFloat(_) | Expr::AtomicFence { .. } | Expr::Todo(_) | Expr::Path(_) => {
                BTreeSet::new()
            }
        }
    }

    fn define_local(&mut self, name: &str, path: &[PathSegment], values: BTreeSet<ConstValue>) {
        let binding = self.local_binding(name, path);
        self.bind(name.to_string(), binding);
        let Some(binding) = binding else {
            return;
        };
        if values.is_empty() {
            self.values_by_binding.remove(&binding);
            return;
        }
        self.values_by_binding.insert(binding, values.clone());
        self.record_binding(binding, path, &values);
    }

    fn local_binding(&self, name: &str, path: &[PathSegment]) -> Option<BindingId<'db>> {
        facts::binding_by_local_path(self.bindings, self.function, name, &AstPath(path.to_vec()))
    }

    fn assign(&mut self, target: &Expr, values: BTreeSet<ConstValue>, path: &mut Vec<PathSegment>) {
        self.kill_target(target, path);
        if values.is_empty() {
            return;
        }
        let Expr::Var(name) = target else {
            return;
        };
        let Some(binding) = self.binding_for_name(name.as_str()) else {
            return;
        };
        self.values_by_binding.insert(binding, values.clone());
        self.record_binding(binding, path, &values);
    }

    fn kill_target(&mut self, target: &Expr, path: &mut Vec<PathSegment>) {
        match target {
            Expr::Var(name) => {
                if let Some(binding) = self.binding_for_name(name.as_str()) {
                    self.values_by_binding.remove(&binding);
                }
            }
            Expr::Field { base, .. } | Expr::TupleField { base, .. } => {
                self.kill_target(base, path)
            }
            Expr::Index { base, index } => {
                self.kill_target(base, path);
                self.child_expr(index, path, 1);
            }
            Expr::Unary {
                op: UnaryOp::Deref,
                expr,
            } => {
                self.child_expr(expr, path, 0);
            }
            Expr::Cast { expr, .. } => self.kill_target(expr, path),
            _ => {
                self.expr(target, path);
            }
        }
    }

    fn shadow_pattern(&mut self, pattern: &Pattern) {
        match pattern {
            Pattern::Binding(name) => self.bind(name.to_string(), None),
            Pattern::Guarded { bind, .. } => self.bind(bind.to_string(), None),
            Pattern::TupleStruct { fields, .. } => {
                for field in fields {
                    self.shadow_pattern(field);
                }
            }
            Pattern::Wildcard
            | Pattern::I64(_)
            | Pattern::I128(_)
            | Pattern::U128(_)
            | Pattern::InclusiveRange { .. } => {}
        }
    }

    fn bind(&mut self, name: String, binding: Option<BindingId<'db>>) {
        if let Some(scope) = self.scopes.last_mut() {
            scope.insert(name, binding);
        }
    }

    fn binding_for_name(&self, name: &str) -> Option<BindingId<'db>> {
        self.scopes
            .iter()
            .rev()
            .find_map(|scope| scope.get(name).copied())
            .flatten()
    }

    fn record_expr(&mut self, path: &[PathSegment], values: &BTreeSet<ConstValue>) {
        for value in values {
            self.values.push(ValueFact {
                site: Site {
                    function: self.function,
                    path: AstPath(path.to_vec()),
                },
                subject: ValueSubject::Expr,
                value: value.clone(),
            });
        }
    }

    fn record_binding(
        &mut self,
        binding: BindingId<'db>,
        path: &[PathSegment],
        values: &BTreeSet<ConstValue>,
    ) {
        for value in values {
            self.values.push(ValueFact {
                site: Site {
                    function: self.function,
                    path: AstPath(path.to_vec()),
                },
                subject: ValueSubject::Binding(binding),
                value: value.clone(),
            });
        }
    }
}

fn values_for_rust_value(value: &RustValue) -> BTreeSet<ConstValue> {
    let mut values = BTreeSet::new();
    match value {
        RustValue::I64(n) => {
            values.insert(ConstValue::Integer(i128::from(*n)));
            if let Ok(n) = usize::try_from(*n) {
                values.insert(ConstValue::Usize(n));
            }
            if *n == 0 {
                values.insert(ConstValue::Zero);
            }
        }
        RustValue::Usize(n) => {
            values.insert(ConstValue::Usize(*n));
            if let Ok(n) = i128::try_from(*n) {
                values.insert(ConstValue::Integer(n));
            }
            if *n == 0 {
                values.insert(ConstValue::Zero);
            }
        }
        RustValue::I128(n) | RustValue::TypedInt(n, _) => {
            values.insert(ConstValue::Integer(*n));
            if let Ok(n) = usize::try_from(*n) {
                values.insert(ConstValue::Usize(n));
            }
            if *n == 0 {
                values.insert(ConstValue::Zero);
            }
        }
        RustValue::U128(n) | RustValue::TypedUInt(n, _) => {
            if let Ok(n) = i128::try_from(*n) {
                values.insert(ConstValue::Integer(n));
                if let Ok(n) = usize::try_from(n) {
                    values.insert(ConstValue::Usize(n));
                }
            }
            if *n == 0 {
                values.insert(ConstValue::Zero);
            }
        }
        RustValue::Bool(value) => {
            values.insert(ConstValue::Bool(*value));
        }
        RustValue::NullPtr | RustValue::None => {
            values.insert(ConstValue::Zero);
        }
        RustValue::Float(_) => {}
    }
    values
}

fn values_for_string(s: &str) -> BTreeSet<ConstValue> {
    BTreeSet::from([
        ConstValue::String(s.to_string()),
        ConstValue::Bytes(s.as_bytes().to_vec()),
    ])
}

fn values_for_bytes(bytes: &[u8]) -> BTreeSet<ConstValue> {
    let mut values = BTreeSet::from([ConstValue::Bytes(bytes.to_vec())]);
    if let Some(bytes) = bytes.strip_suffix(&[0]) {
        values.insert(ConstValue::CStringBytes(bytes.to_vec()));
    }
    values
}

fn fold_unary(op: UnaryOp, values: &BTreeSet<ConstValue>) -> Option<BTreeSet<ConstValue>> {
    match op {
        UnaryOp::Neg => values
            .iter()
            .find_map(integer_value)
            .and_then(|n| n.checked_neg())
            .map(values_for_integer),
        UnaryOp::Not => {
            if let Some(value) = values.iter().find_map(bool_value) {
                Some(BTreeSet::from([ConstValue::Bool(!value)]))
            } else {
                values
                    .iter()
                    .find_map(integer_value)
                    .map(|n| values_for_integer(!n))
            }
        }
        UnaryOp::Deref | UnaryOp::Raw(_) => None,
    }
}

fn fold_binary(
    op: BinOp,
    lhs: &BTreeSet<ConstValue>,
    rhs: &BTreeSet<ConstValue>,
) -> Option<BTreeSet<ConstValue>> {
    if matches!(op, BinOp::And | BinOp::Or) {
        let lhs = lhs.iter().find_map(bool_value)?;
        let rhs = rhs.iter().find_map(bool_value)?;
        return Some(BTreeSet::from([ConstValue::Bool(match op {
            BinOp::And => lhs && rhs,
            BinOp::Or => lhs || rhs,
            _ => unreachable!(),
        })]));
    }

    let lhs = lhs.iter().find_map(integer_value)?;
    let rhs = rhs.iter().find_map(integer_value)?;
    let value = match op {
        BinOp::Add => lhs.checked_add(rhs)?,
        BinOp::Sub => lhs.checked_sub(rhs)?,
        BinOp::Mul => lhs.checked_mul(rhs)?,
        BinOp::Div if rhs != 0 => lhs.checked_div(rhs)?,
        BinOp::Rem if rhs != 0 => lhs.checked_rem(rhs)?,
        BinOp::BitAnd => lhs & rhs,
        BinOp::BitOr => lhs | rhs,
        BinOp::BitXor => lhs ^ rhs,
        BinOp::Shl => lhs.checked_shl(u32::try_from(rhs).ok()?)?,
        BinOp::Shr => lhs.checked_shr(u32::try_from(rhs).ok()?)?,
        BinOp::Eq => return Some(BTreeSet::from([ConstValue::Bool(lhs == rhs)])),
        BinOp::Ne => return Some(BTreeSet::from([ConstValue::Bool(lhs != rhs)])),
        BinOp::Lt => return Some(BTreeSet::from([ConstValue::Bool(lhs < rhs)])),
        BinOp::Le => return Some(BTreeSet::from([ConstValue::Bool(lhs <= rhs)])),
        BinOp::Gt => return Some(BTreeSet::from([ConstValue::Bool(lhs > rhs)])),
        BinOp::Ge => return Some(BTreeSet::from([ConstValue::Bool(lhs >= rhs)])),
        BinOp::And | BinOp::Or | BinOp::Div | BinOp::Rem => return None,
    };
    Some(values_for_integer(value))
}

fn fold_cast(values: &BTreeSet<ConstValue>, ty: &Type) -> BTreeSet<ConstValue> {
    let Type::Prim(prim) = ty else {
        return BTreeSet::new();
    };
    match prim {
        Prim::Bool => values
            .iter()
            .find_map(integer_value)
            .map(|n| BTreeSet::from([ConstValue::Bool(n != 0)]))
            .unwrap_or_default(),
        Prim::Usize => values
            .iter()
            .find_map(integer_value)
            .and_then(|n| usize::try_from(n).ok())
            .map(values_for_usize)
            .unwrap_or_default(),
        Prim::I8
        | Prim::I16
        | Prim::I32
        | Prim::I64
        | Prim::I128
        | Prim::Isize
        | Prim::U8
        | Prim::U16
        | Prim::U32
        | Prim::U64
        | Prim::U128 => values
            .iter()
            .find_map(integer_value)
            .map(values_for_integer)
            .unwrap_or_default(),
        Prim::F16 | Prim::F32 | Prim::F64 | Prim::F128 => BTreeSet::new(),
    }
}

fn values_for_integer(n: i128) -> BTreeSet<ConstValue> {
    let mut values = BTreeSet::from([ConstValue::Integer(n)]);
    if let Ok(n) = usize::try_from(n) {
        values.insert(ConstValue::Usize(n));
    }
    if n == 0 {
        values.insert(ConstValue::Zero);
    }
    values
}

fn values_for_usize(n: usize) -> BTreeSet<ConstValue> {
    let mut values = BTreeSet::from([ConstValue::Usize(n)]);
    if n == 0 {
        values.insert(ConstValue::Zero);
    }
    values
}

fn integer_value(value: &ConstValue) -> Option<i128> {
    match value {
        ConstValue::Integer(n) => Some(*n),
        ConstValue::Usize(n) => i128::try_from(*n).ok(),
        ConstValue::Bool(_)
        | ConstValue::Bytes(_)
        | ConstValue::CStringBytes(_)
        | ConstValue::String(_)
        | ConstValue::Zero
        | ConstValue::ArrayLength(_) => None,
    }
}

fn bool_value(value: &ConstValue) -> Option<bool> {
    match value {
        ConstValue::Bool(value) => Some(*value),
        _ => None,
    }
}
