use crate::backend::rust_ast::{AtomicPlace, Block, Expr, FnParam, Ident, Pattern, Stmt};
use std::collections::HashSet;

trait Names {
    fn push(&mut self) {}
    fn pop(&mut self) {}
    fn bind(&mut self, name: &mut Ident, local: bool);
    fn var(&mut self, name: &mut Ident);
}

fn walk_stmts(stmts: &mut [Stmt], names: &mut impl Names) {
    names.push();
    for stmt in stmts {
        walk_stmt(stmt, names);
    }
    names.pop();
}

fn walk_block(block: &mut Block, names: &mut impl Names) {
    names.push();
    for stmt in &mut block.stmts {
        walk_stmt(stmt, names);
    }
    if let Some(tail) = &mut block.tail {
        walk_expr(tail, names);
    }
    names.pop();
}

fn walk_branch(stmts: &mut [Stmt], value: &mut Expr, names: &mut impl Names) {
    names.push();
    for stmt in stmts {
        walk_stmt(stmt, names);
    }
    walk_expr(value, names);
    names.pop();
}

fn bind_string(name: &mut String, local: bool, names: &mut impl Names) {
    let mut ident = Ident::new(name.as_str());
    names.bind(&mut ident, local);
    if ident.as_str() != name {
        *name = ident.as_str().to_string();
    }
}

fn walk_stmt(stmt: &mut Stmt, names: &mut impl Names) {
    match stmt {
        Stmt::Comment(_) | Stmt::Break(_) | Stmt::Continue(_) | Stmt::Return(None) => {}
        Stmt::Let { name, init, .. } => {
            if let Some(init) = init {
                walk_expr(init, names);
            }
            bind_string(name, true, names);
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
            walk_expr(cond, names);
            walk_branch(then_body, then_value, names);
            walk_branch(else_body, else_value, names);
            bind_string(name, true, names);
        }
        Stmt::Assign { target, value } | Stmt::CompoundAssign { target, value, .. } => {
            walk_expr(target, names);
            walk_expr(value, names);
        }
        Stmt::InlineAsm(asm) => {
            for operand in &mut asm.operands {
                operand.visit_exprs_mut(&mut |expr| walk_expr(expr, names));
            }
        }
        Stmt::Expr(expr) | Stmt::Return(Some(expr)) => walk_expr(expr, names),
        Stmt::Unsafe { body } | Stmt::Block(body) => walk_block(body, names),
        Stmt::While { cond, body, .. } => {
            walk_expr(cond, names);
            walk_block(body, names);
        }
        Stmt::If {
            cond,
            then_body,
            else_body,
        } => {
            walk_expr(cond, names);
            walk_stmts(then_body, names);
            walk_stmts(else_body, names);
        }
        Stmt::Loop { body, .. } | Stmt::Scope { body } | Stmt::LabeledBlock { body, .. } => {
            walk_stmts(body, names)
        }
        Stmt::For { pat, iter, body } => {
            walk_expr(iter, names);
            names.push();
            bind_string(pat, true, names);
            for stmt in body {
                walk_stmt(stmt, names);
            }
            names.pop();
        }
        Stmt::Match { expr, arms } => {
            walk_expr(expr, names);
            for arm in arms {
                names.push();
                walk_pattern(&mut arm.pattern, names);
                for stmt in &mut arm.body {
                    walk_stmt(stmt, names);
                }
                names.pop();
            }
        }
    }
}

fn walk_pattern(pattern: &mut Pattern, names: &mut impl Names) {
    match pattern {
        Pattern::Binding(name) => names.bind(name, false),
        Pattern::Guarded { bind, cond } => {
            names.bind(bind, false);
            walk_expr(cond, names);
        }
        Pattern::TupleStruct { fields, .. } | Pattern::Or(fields) => {
            for field in fields {
                walk_pattern(field, names);
            }
        }
        Pattern::Wildcard
        | Pattern::I64(_)
        | Pattern::I128(_)
        | Pattern::U128(_)
        | Pattern::InclusiveRange { .. }
        | Pattern::InclusiveRangeU128 { .. } => {}
    }
}

fn walk_place(place: &mut AtomicPlace, names: &mut impl Names) {
    match place {
        AtomicPlace::Ptr(ptr) => walk_expr(ptr, names),
        AtomicPlace::Local(name) => names.var(name),
    }
}

fn walk_expr(expr: &mut Expr, names: &mut impl Names) {
    match expr {
        Expr::Var(name) => names.var(name),
        Expr::Value(_)
        | Expr::Str(_)
        | Expr::HexFloat(_)
        | Expr::ByteStr(_)
        | Expr::CStr(_)
        | Expr::Path(_)
        | Expr::GenericPath { .. }
        | Expr::Todo(_)
        | Expr::AtomicFence { .. } => {}
        Expr::ConstBlock(expr)
        | Expr::Unary { expr, .. }
        | Expr::Cast { expr, .. }
        | Expr::Ref { expr, .. }
        | Expr::AddrOf { expr, .. }
        | Expr::Transmute { expr, .. }
        | Expr::Field { base: expr, .. }
        | Expr::TupleField { base: expr, .. }
        | Expr::ArrayPtr { array: expr, .. }
        | Expr::ArrayRepeat { elem: expr, .. }
        | Expr::AtomicNew { value: expr, .. } => walk_expr(expr, names),
        Expr::Binary { lhs, rhs, .. }
        | Expr::Range {
            start: lhs,
            end: rhs,
        }
        | Expr::Index {
            base: lhs,
            index: rhs,
        }
        | Expr::VecRepeat {
            elem: lhs,
            len: rhs,
        }
        | Expr::CopyNonoverlapping {
            src: lhs, dst: rhs, ..
        } => {
            walk_expr(lhs, names);
            walk_expr(rhs, names);
        }
        Expr::PtrCopy {
            src, dst, count, ..
        }
        | Expr::WriteBytes {
            dst: src,
            val: dst,
            count,
        } => {
            walk_expr(src, names);
            walk_expr(dst, names);
            walk_expr(count, names);
        }
        Expr::Call { func, args, .. } => {
            walk_expr(func, names);
            for arg in args {
                walk_expr(arg, names);
            }
        }
        Expr::MethodCall { recv, args, .. } | Expr::MethodCallGeneric { recv, args, .. } => {
            walk_expr(recv, names);
            for arg in args {
                walk_expr(arg, names);
            }
        }
        Expr::StructLit { fields, .. } => {
            for (_, value) in fields {
                walk_expr(value, names);
            }
        }
        Expr::TupleStructLit { fields, .. }
        | Expr::ArrayLit(fields)
        | Expr::VecLit(fields)
        | Expr::Macro { args: fields, .. } => {
            for field in fields {
                walk_expr(field, names);
            }
        }
        Expr::Closure { params, body } => {
            names.push();
            for param in params {
                names.bind(param, false);
            }
            walk_expr(body, names);
            names.pop();
        }
        Expr::Match { expr, arms } => {
            walk_expr(expr, names);
            for arm in arms {
                names.push();
                walk_pattern(&mut arm.pattern, names);
                walk_expr(&mut arm.value, names);
                names.pop();
            }
        }
        Expr::If {
            cond,
            then_expr,
            else_expr,
        } => {
            walk_expr(cond, names);
            walk_expr(then_expr, names);
            walk_expr(else_expr, names);
        }
        Expr::Block(block) | Expr::Unsafe(block) => walk_block(block, names),
        Expr::AtomicRef { place, .. } | Expr::AtomicLoad { place, .. } => walk_place(place, names),
        Expr::AtomicStore { place, value, .. }
        | Expr::AtomicFetch { place, value, .. }
        | Expr::AtomicSwap { place, value, .. } => {
            walk_place(place, names);
            walk_expr(value, names);
        }
        Expr::AtomicCompareExchange {
            place,
            expected,
            desired,
            ..
        } => {
            walk_place(place, names);
            walk_expr(expected, names);
            walk_expr(desired, names);
        }
    }
}

struct Uniquify {
    frames: Vec<Vec<(Ident, Ident)>>,
    seen: HashSet<Ident>,
    renamed: Vec<(Ident, Ident)>,
}

impl Names for Uniquify {
    fn push(&mut self) {
        self.frames.push(Vec::new());
    }

    fn pop(&mut self) {
        self.frames.pop();
    }

    fn bind(&mut self, name: &mut Ident, local: bool) {
        let original = *name;
        if local && original.as_str() != "_" && !self.seen.insert(original) {
            let mut counter = self.renamed.len();
            *name = loop {
                counter += 1;
                let candidate =
                    Ident::new(format!("{}__slate_shadow_{counter}", original.as_str()));
                if self.seen.insert(candidate) {
                    break candidate;
                }
            };
            self.renamed.push((*name, original));
        }
        if let Some(frame) = self.frames.last_mut() {
            frame.push((original, *name));
        }
    }

    fn var(&mut self, name: &mut Ident) {
        if let Some(&(_, current)) = self
            .frames
            .iter()
            .rev()
            .flat_map(|frame| frame.iter().rev())
            .find(|(original, _)| original == name)
        {
            *name = current;
        }
    }
}

pub(super) fn uniquify(body: &mut [Stmt], params: &[FnParam]) -> Vec<(Ident, Ident)> {
    let mut uniquify = Uniquify {
        frames: vec![Vec::new()],
        seen: HashSet::new(),
        renamed: Vec::new(),
    };
    for param in params {
        let mut name = Ident::new(param.name.as_str());
        uniquify.bind(&mut name, true);
    }
    walk_stmts(body, &mut uniquify);
    uniquify.renamed
}

struct Collect(HashSet<Ident>);

impl Names for Collect {
    fn bind(&mut self, name: &mut Ident, _: bool) {
        self.0.insert(*name);
    }

    fn var(&mut self, name: &mut Ident) {
        self.0.insert(*name);
    }
}

struct ScopeMentions {
    target: Ident,
    frames: Vec<bool>,
    binders: usize,
    mentions: HashSet<Ident>,
}

impl ScopeMentions {
    fn in_scope(&self) -> bool {
        self.frames.iter().any(|&bound| bound)
    }
}

impl Names for ScopeMentions {
    fn push(&mut self) {
        self.frames.push(false);
    }

    fn pop(&mut self) {
        self.frames.pop();
    }

    fn bind(&mut self, name: &mut Ident, _: bool) {
        if *name == self.target {
            self.binders += 1;
            if let Some(frame) = self.frames.last_mut() {
                *frame = true;
            }
        } else if self.in_scope() {
            self.mentions.insert(*name);
        }
    }

    fn var(&mut self, name: &mut Ident) {
        if *name != self.target && self.in_scope() {
            self.mentions.insert(*name);
        }
    }
}

struct Rename {
    from: Ident,
    to: Ident,
}

impl Names for Rename {
    fn bind(&mut self, name: &mut Ident, _: bool) {
        self.var(name);
    }

    fn var(&mut self, name: &mut Ident) {
        if *name == self.from {
            *name = self.to;
        }
    }
}

pub(super) fn restore(body: &mut [Stmt], params: &[FnParam], renamed: &[(Ident, Ident)]) {
    if renamed.is_empty() {
        return;
    }
    let mut taken = Collect(
        params
            .iter()
            .map(|param| Ident::new(param.name.as_str()))
            .collect(),
    );
    walk_stmts(body, &mut taken);
    let mut taken = taken.0;
    let mut pending: HashSet<Ident> = renamed.iter().map(|&(fresh, _)| fresh).collect();
    for &(fresh, original) in renamed {
        pending.remove(&fresh);
        let mut scope = ScopeMentions {
            target: fresh,
            frames: vec![false],
            binders: 0,
            mentions: HashSet::new(),
        };
        walk_stmts(body, &mut scope);
        let conflicts = scope.mentions.contains(&original)
            || renamed.iter().any(|&(other, base)| {
                base == original
                    && other != fresh
                    && pending.contains(&other)
                    && scope.mentions.contains(&other)
            });
        let to = if scope.binders == 1 && !conflicts {
            original
        } else {
            (1..)
                .map(|n| Ident::new(format!("{}_{n}", original.as_str())))
                .find(|candidate| !taken.contains(candidate))
                .unwrap_or(fresh)
        };
        taken.insert(to);
        walk_stmts(body, &mut Rename { from: fresh, to });
    }
}
