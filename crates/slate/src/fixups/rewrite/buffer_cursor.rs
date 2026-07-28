use std::collections::{BTreeMap, BTreeSet};

use crate::fixups::Fixup;
use crate::fixups::facts::{FixupFacts, FunctionId};
use crate::fixups::idents::stmt_ident_count;
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, fact, function_path_location, stmts_snippet,
};
use crate::rust_ast::{BinOp, Block, Expr, Ident, IndentStmt, RustValue, Stmt};

pub(in crate::fixups) struct BufferCursor<'a> {
    function: FunctionId,
    facts: &'a FixupFacts,
    logger: &'a mut dyn TraceLogger,
}

impl Fixup for BufferCursor<'_> {
    fn fixup(&mut self, body: &mut Vec<IndentStmt>) -> bool {
        let context = Context::new(self.function, self.facts);
        if context.arrays.is_empty() || context.buffers.is_empty() {
            return false;
        }
        let before = self.logger.is_enabled().then(|| body.clone());
        let mut rewritten = body.clone();
        let mut rewriter = Rewriter::new(context.clone());
        if rewriter.body(&mut rewritten)
            && !contains_unresolved_pointer_uses(&rewritten, &context, &rewriter.aliases)
        {
            *body = rewritten;
            if let Some(before) = before {
                self.logger.rewrite(RewriteEvent {
                    pass: TracePass::BufferCursor,
                    kind: "rewrite_buffer_cursor".into(),
                    location: function_path_location(self.facts, self.function, &[]),
                    before: vec![stmts_snippet("body", &before)],
                    after: vec![stmts_snippet("body", body)],
                    facts: vec![
                        fact("arrays", context.arrays.len().to_string()),
                        fact("buffers", context.buffers.len().to_string()),
                    ],
                });
            }
            return true;
        }
        false
    }
}

impl<'a> BufferCursor<'a> {
    pub(in crate::fixups) fn new(
        function: FunctionId,
        facts: &'a FixupFacts,
        logger: &'a mut dyn TraceLogger,
    ) -> Self {
        Self {
            function,
            facts,
            logger,
        }
    }
}

#[derive(Clone)]
struct ArrayInfo {
    len: usize,
}

#[derive(Clone, PartialEq, Eq)]
struct Origin {
    array: String,
    index: usize,
}

#[derive(Clone, PartialEq, Eq, PartialOrd, Ord)]
struct FieldKey {
    buffer: String,
    field: String,
}

#[derive(Clone)]
struct Context {
    arrays: BTreeMap<String, ArrayInfo>,
    buffers: BTreeSet<String>,
}

impl Context {
    fn new(function: FunctionId, facts: &FixupFacts) -> Self {
        let mut arrays = BTreeMap::new();
        let mut buffers = BTreeSet::new();
        for fact in facts
            .buffer_pointer_fields
            .iter()
            .filter(|fact| fact.function == function)
        {
            let Some(array) = facts.binding_name(fact.array) else {
                continue;
            };
            let Some(buffer) = facts.binding_name(fact.buffer) else {
                continue;
            };
            let Some(len) = facts
                .binding_type(fact.array)
                .and_then(array_len_from_rendered_type)
            else {
                continue;
            };
            arrays.insert(array.to_string(), ArrayInfo { len });
            buffers.insert(buffer.to_string());
        }
        Self { arrays, buffers }
    }
}

struct Rewriter {
    context: Context,
    fields: BTreeMap<FieldKey, Origin>,
    aliases: BTreeMap<String, Origin>,
    constants: BTreeMap<String, usize>,
}

impl Rewriter {
    fn new(context: Context) -> Self {
        Self {
            context,
            fields: BTreeMap::new(),
            aliases: BTreeMap::new(),
            constants: BTreeMap::new(),
        }
    }

    fn body(&mut self, body: &mut Vec<IndentStmt>) -> bool {
        let mut changed = false;
        let mut index = 0;
        while index < body.len() {
            if self.rewrite_indent(&mut body[index]) {
                changed = true;
            }
            if self.remove_stmt(&body[index].stmt) {
                body.remove(index);
                changed = true;
            } else {
                index += 1;
            }
        }
        changed |= self.prune_unused_buffers(body);
        changed |= self.prune_unused_constants(body);
        changed
    }

    fn rewrite_indent(&mut self, indent: &mut IndentStmt) -> bool {
        let mut changed = self.rewrite_stmt(&mut indent.stmt);
        walk::nested_body_vecs_mut_with_path(&mut indent.stmt, &mut Vec::new(), &mut |body, _| {
            let mut nested = Rewriter::new(Context {
                arrays: self.context.arrays.clone(),
                buffers: self.context.buffers.clone(),
            });
            nested.fields = self.fields.clone();
            nested.aliases = self.aliases.clone();
            nested.constants = self.constants.clone();
            changed |= nested.body(body);
        });
        changed
    }

    fn rewrite_stmt(&mut self, stmt: &mut Stmt) -> bool {
        match stmt {
            Stmt::Let {
                name,
                init: Some(init),
                ..
            } => {
                if let Some(origin) = self.pointer_origin(init) {
                    self.aliases.insert(name.clone(), origin);
                }
                if let Some(value) = self.usize_value(init) {
                    self.constants.insert(name.clone(), value);
                }
                self.rewrite_stmt_exprs(stmt)
            }
            Stmt::Assign { target, value } => {
                if let Some(key) = self.field_key(target)
                    && let Some(origin) = self.pointer_origin(value)
                {
                    self.fields.insert(key, origin);
                    return false;
                }
                self.rewrite_stmt_exprs(stmt)
            }
            Stmt::Unsafe { body } => {
                if let Some(replacement) = self.safe_unsafe_assignment(body) {
                    *stmt = replacement;
                    return true;
                }
                self.rewrite_stmt_exprs(stmt)
            }
            _ => self.rewrite_stmt_exprs(stmt),
        }
    }

    fn rewrite_stmt_exprs(&self, stmt: &mut Stmt) -> bool {
        let mut changed = false;
        walk::stmt_exprs_mut_with(stmt, &mut |expr| {
            if let Some(replacement) = self.safe_expr(expr) {
                *expr = replacement;
                changed = true;
                return false;
            }
            true
        });
        changed
    }

    fn remove_stmt(&self, stmt: &Stmt) -> bool {
        match stmt {
            Stmt::Let {
                init: Some(init), ..
            } => self.pointer_origin(init).is_some(),
            Stmt::Assign { target, value } => {
                self.field_key(target).is_some() && self.pointer_origin(value).is_some()
            }
            _ => false,
        }
    }

    fn prune_unused_buffers(&self, body: &mut Vec<IndentStmt>) -> bool {
        let mut changed = false;
        let mut index = 0;
        while index < body.len() {
            if let Stmt::Let { name, .. } = &body[index].stmt
                && self.context.buffers.contains(name.as_str())
                && body[index + 1..]
                    .iter()
                    .all(|later| stmt_ident_count(&later.stmt, name) == 0)
            {
                body.remove(index);
                changed = true;
                continue;
            }
            index += 1;
        }
        changed
    }

    fn prune_unused_constants(&self, body: &mut Vec<IndentStmt>) -> bool {
        let mut changed = false;
        let mut index = 0;
        while index < body.len() {
            if let Stmt::Let { name, .. } = &body[index].stmt
                && self.constants.contains_key(name.as_str())
                && body[index + 1..]
                    .iter()
                    .all(|later| stmt_ident_count(&later.stmt, name) == 0)
            {
                body.remove(index);
                changed = true;
                continue;
            }
            index += 1;
        }
        changed
    }

    fn safe_unsafe_assignment(&self, block: &Block) -> Option<Stmt> {
        if block.tail.is_some() || block.stmts.len() != 1 {
            return None;
        }
        let Stmt::Assign { target, value } = &block.stmts[0].stmt else {
            return None;
        };
        let Expr::Unary {
            op: crate::rust_ast::UnaryOp::Deref,
            expr,
        } = target
        else {
            return None;
        };
        Some(Stmt::Assign {
            target: self.indexed_origin(expr)?,
            value: value.clone(),
        })
    }

    fn safe_expr(&self, expr: &Expr) -> Option<Expr> {
        match expr {
            Expr::Unsafe(block) if block.stmts.is_empty() => self.safe_expr(block.tail.as_ref()?),
            Expr::Unary {
                op: crate::rust_ast::UnaryOp::Deref,
                expr,
            } => self.indexed_origin(expr),
            Expr::Cast { expr, ty } => self.pointer_diff(expr).map(|expr| Expr::Cast {
                expr: Box::new(expr),
                ty: ty.clone(),
            }),
            _ => self.pointer_diff(expr),
        }
    }

    fn indexed_origin(&self, expr: &Expr) -> Option<Expr> {
        let origin = self.pointer_origin(expr)?;
        let array = self.context.arrays.get(&origin.array)?;
        if origin.index >= array.len {
            return None;
        }
        Some(Expr::Index {
            base: Box::new(Expr::Var(Ident::from(origin.array.as_str()))),
            index: Box::new(Expr::Value(RustValue::I64(origin.index as i64))),
        })
    }

    fn pointer_diff(&self, expr: &Expr) -> Option<Expr> {
        let Expr::MethodCall { recv, method, args } = peel_unsafe(expr) else {
            return None;
        };
        if method != "offset_from" || args.len() != 1 {
            return None;
        }
        let lhs = self.pointer_origin(recv)?;
        let rhs = self.pointer_origin(&args[0])?;
        if lhs.array != rhs.array {
            return None;
        }
        let array = self.context.arrays.get(&lhs.array)?;
        if lhs.index > array.len || rhs.index > array.len {
            return None;
        }
        let lhs = Expr::Value(RustValue::I64(lhs.index as i64));
        if rhs.index == 0 {
            return Some(lhs);
        }
        Some(Expr::Binary {
            op: BinOp::Sub,
            lhs: Box::new(lhs),
            rhs: Box::new(Expr::Value(RustValue::I64(rhs.index as i64))),
        })
    }

    fn pointer_origin(&self, expr: &Expr) -> Option<Origin> {
        match peel_unsafe(expr) {
            Expr::Var(name) => self.aliases.get(name.as_str()).cloned(),
            Expr::Field { .. } => self
                .field_key(expr)
                .and_then(|key| self.fields.get(&key).cloned()),
            Expr::ArrayPtr {
                array,
                mutable: true,
            } => self.array_origin(array, 0),
            Expr::MethodCall { recv, method, args }
                if method == "as_mut_ptr" && args.is_empty() =>
            {
                self.array_origin(recv, 0)
            }
            Expr::MethodCall { recv, method, args } if method == "add" && args.len() == 1 => {
                let mut origin = self.pointer_origin(recv)?;
                origin.index += self.usize_value(&args[0])?;
                Some(origin)
            }
            _ => None,
        }
    }

    fn array_origin(&self, expr: &Expr, index: usize) -> Option<Origin> {
        let Expr::Var(array) = expr else {
            return None;
        };
        self.context
            .arrays
            .contains_key(array.as_str())
            .then(|| Origin {
                array: array.to_string(),
                index,
            })
    }

    fn field_key(&self, expr: &Expr) -> Option<FieldKey> {
        let Expr::Field { base, field } = expr else {
            return None;
        };
        let Expr::Var(buffer) = &**base else {
            return None;
        };
        self.context
            .buffers
            .contains(buffer.as_str())
            .then(|| FieldKey {
                buffer: buffer.to_string(),
                field: field.clone(),
            })
    }

    fn usize_value(&self, expr: &Expr) -> Option<usize> {
        match peel_unsafe(expr) {
            Expr::Var(name) => self.constants.get(name.as_str()).copied(),
            Expr::Value(RustValue::I64(n)) => usize::try_from(*n).ok(),
            Expr::Value(RustValue::I128(n)) => usize::try_from(*n).ok(),
            Expr::Value(RustValue::Usize(n)) => Some(*n),
            Expr::Cast { expr, .. } => self.usize_value(expr),
            _ => None,
        }
    }
}

fn array_len_from_rendered_type(ty: &str) -> Option<usize> {
    let (_, len) = ty.rsplit_once(';')?;
    len.trim_end_matches(']').trim().parse().ok()
}

fn peel_unsafe(expr: &Expr) -> &Expr {
    match expr {
        Expr::Unsafe(block) if block.stmts.is_empty() => {
            block.tail.as_deref().map_or(expr, peel_unsafe)
        }
        _ => expr,
    }
}

fn contains_unresolved_pointer_uses(
    body: &[IndentStmt],
    context: &Context,
    aliases: &BTreeMap<String, Origin>,
) -> bool {
    body.iter().any(|indent| {
        context
            .buffers
            .iter()
            .any(|buffer| stmt_ident_count(&indent.stmt, buffer) > 0)
            || aliases
                .keys()
                .any(|alias| stmt_ident_count(&indent.stmt, alias) > 0)
    })
}
