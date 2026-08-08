use std::collections::{BTreeMap, BTreeSet};

use crate::fixups::idents::stmt_ident_count;
use crate::fixups::support::walk as support_walk;
use crate::fixups::trace::Pass;
use crate::rust_ast::{BinOp, Block, Expr, Ident, IndentStmt, RustValue, Stmt, UnaryOp};

use super::super::{
    BufferPointerFields, EditSet, Function, FunctionRef, ItemCaseContext, QueryRule, Rejection,
};

pub(in crate::fixups) fn rewrite() -> QueryRule<Function> {
    QueryRule::new(
        Pass::BufferCursor,
        "rewrite_buffer_cursor",
        Function::default(),
    )
    .case("resolved", rewrite_case)
}

fn rewrite_case<'db>(
    case: &mut ItemCaseContext<'_, '_>,
    function: &FunctionRef<'db>,
) -> Result<EditSet, Rejection> {
    let fields = case.fact(|query| query.buffer_pointer_fields(function))?;
    let bindings = case.fact(|query| query.function_bindings(function))?;
    let _ = case.fact(|query| query.function_expressions(function))?;
    for field in &fields.fields {
        let _ = case.fact(|query| query.statement(&field.assignment))?;
        let _ = case.fact(|query| query.statement_expression(&field.assignment, 0))?;
        let _ = case.fact(|query| query.statement_expression(&field.assignment, 1))?;
        let _ = case.fact(|query| query.binding_uses(&field.buffer))?;
        let _ = case.fact(|query| query.binding_uses(&field.array))?;
    }
    let snapshot = case
        .fact(|query| query.function_snapshot(function))?
        .clone();
    let context = buffer_cursor_context(&fields);
    let mut body = snapshot.body.clone();
    let mut rewriter = BufferCursorRewriter::new(context.clone());
    case.require(rewriter.body(&mut body))?;
    for name in rewriter.aliases.keys() {
        let binding = bindings
            .iter()
            .find(|binding| binding.name == *name)
            .ok_or_else(|| case.reject())?;
        let initializer = case.fact(|query| query.binding_initializer(binding))?;
        let _ = case.fact(|query| query.expression_dependencies(&initializer))?;
        let _ = case.fact(|query| query.binding_uses(binding))?;
    }
    case.require(!buffer_cursor_contains_unresolved_uses(
        &body,
        &context,
        &rewriter.aliases,
    ))?;
    let mut replacement = snapshot;
    replacement.body = body;
    Ok(EditSet::replace_function(function.clone(), replacement))
}

#[derive(Clone)]
struct BufferCursorArrayInfo {
    len: usize,
}

#[derive(Clone, PartialEq, Eq)]
struct BufferCursorOrigin {
    array: String,
    index: usize,
}

#[derive(Clone, PartialEq, Eq, PartialOrd, Ord)]
struct BufferCursorFieldKey {
    buffer: String,
    field: String,
}

#[derive(Clone)]
struct BufferCursorContext {
    arrays: BTreeMap<String, BufferCursorArrayInfo>,
    buffers: BTreeSet<String>,
}

fn buffer_cursor_context(fields: &BufferPointerFields) -> BufferCursorContext {
    let mut arrays = BTreeMap::new();
    let mut buffers = BTreeSet::new();
    for field in &fields.fields {
        arrays.insert(
            field.array.name.clone(),
            BufferCursorArrayInfo {
                len: field.array_len,
            },
        );
        buffers.insert(field.buffer.name.clone());
    }
    BufferCursorContext { arrays, buffers }
}

struct BufferCursorRewriter {
    context: BufferCursorContext,
    fields: BTreeMap<BufferCursorFieldKey, BufferCursorOrigin>,
    aliases: BTreeMap<String, BufferCursorOrigin>,
    constants: BTreeMap<String, usize>,
}

impl BufferCursorRewriter {
    fn new(context: BufferCursorContext) -> Self {
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
        support_walk::nested_body_vecs_mut_with_path(
            &mut indent.stmt,
            &mut Vec::new(),
            &mut |body, _| {
                let mut nested = BufferCursorRewriter::new(BufferCursorContext {
                    arrays: self.context.arrays.clone(),
                    buffers: self.context.buffers.clone(),
                });
                nested.fields = self.fields.clone();
                nested.aliases = self.aliases.clone();
                nested.constants = self.constants.clone();
                changed |= nested.body(body);
            },
        );
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
        support_walk::stmt_exprs_mut_with(stmt, &mut |expr| {
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
            op: UnaryOp::Deref,
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
                op: UnaryOp::Deref,
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
        let Expr::MethodCall { recv, method, args } = buffer_cursor_peel_unsafe(expr) else {
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

    fn pointer_origin(&self, expr: &Expr) -> Option<BufferCursorOrigin> {
        match buffer_cursor_peel_unsafe(expr) {
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

    fn array_origin(&self, expr: &Expr, index: usize) -> Option<BufferCursorOrigin> {
        let Expr::Var(array) = expr else {
            return None;
        };
        self.context
            .arrays
            .contains_key(array.as_str())
            .then(|| BufferCursorOrigin {
                array: array.to_string(),
                index,
            })
    }

    fn field_key(&self, expr: &Expr) -> Option<BufferCursorFieldKey> {
        let Expr::Field { base, field } = expr else {
            return None;
        };
        let Expr::Var(buffer) = &**base else {
            return None;
        };
        self.context
            .buffers
            .contains(buffer.as_str())
            .then(|| BufferCursorFieldKey {
                buffer: buffer.to_string(),
                field: field.clone(),
            })
    }

    fn usize_value(&self, expr: &Expr) -> Option<usize> {
        match buffer_cursor_peel_unsafe(expr) {
            Expr::Var(name) => self.constants.get(name.as_str()).copied(),
            Expr::Value(RustValue::I64(n)) => usize::try_from(*n).ok(),
            Expr::Value(RustValue::I128(n)) => usize::try_from(*n).ok(),
            Expr::Value(RustValue::Usize(n)) => Some(*n),
            Expr::Cast { expr, .. } => self.usize_value(expr),
            _ => None,
        }
    }
}

fn buffer_cursor_peel_unsafe(expr: &Expr) -> &Expr {
    match expr {
        Expr::Unsafe(block) if block.stmts.is_empty() => block
            .tail
            .as_deref()
            .map_or(expr, buffer_cursor_peel_unsafe),
        _ => expr,
    }
}

fn buffer_cursor_contains_unresolved_uses(
    body: &[IndentStmt],
    context: &BufferCursorContext,
    aliases: &BTreeMap<String, BufferCursorOrigin>,
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
