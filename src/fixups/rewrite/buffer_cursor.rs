use std::collections::{BTreeMap, BTreeSet};

use crate::fixups::facts::{FixupFacts, FunctionId};
use crate::fixups::idents::stmt_ident_count;
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLogger, fact, function_path_location, stmts_snippet,
};
use crate::rust_ast::{BinOp, Block, Expr, Ident, IndentStmt, Item, Program, RustValue, Stmt};

pub(in crate::fixups) fn fixup(program: &mut Program, facts: &FixupFacts) -> bool {
    let mut logger = crate::fixups::trace::NoopLogger;
    BufferCursor::new(&mut logger).fixup(program, facts)
}

pub(in crate::fixups) struct BufferCursor<'a> {
    logger: &'a mut dyn TraceLogger,
}

impl<'a> BufferCursor<'a> {
    pub(in crate::fixups) fn new(logger: &'a mut dyn TraceLogger) -> Self {
        Self { logger }
    }

    pub(in crate::fixups) fn fixup(&mut self, program: &mut Program, facts: &FixupFacts) -> bool {
        fixup_impl(program, facts, self.logger)
    }
}

fn fixup_impl(program: &mut Program, facts: &FixupFacts, logger: &mut dyn TraceLogger) -> bool {
    let mut changed = false;
    for (item_index, item) in program.items.iter_mut().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        let context = Context::new(function, facts);
        if context.arrays.is_empty() || context.buffers.is_empty() {
            continue;
        }
        let before = logger.is_enabled().then(|| f.body.clone());
        let mut rewritten = f.body.clone();
        let mut rewriter = Rewriter::new(context.clone());
        if rewriter.body(&mut rewritten)
            && !contains_unresolved_pointer_uses(&rewritten, &context, &rewriter.aliases)
        {
            f.body = rewritten;
            if let Some(before) = before {
                logger.rewrite(RewriteEvent {
                    pass: TracePass::BufferCursor,
                    kind: "rewrite_buffer_cursor".into(),
                    location: function_path_location(facts, function, &[]),
                    before: vec![stmts_snippet("body", &before)],
                    after: vec![stmts_snippet("body", &f.body)],
                    facts: vec![
                        fact("arrays", context.arrays.len().to_string()),
                        fact("buffers", context.buffers.len().to_string()),
                    ],
                });
            }
            changed = true;
        }
    }
    changed
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

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::facts;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{Block, Item, Program, Type, UnaryOp};

    fn apply(mut program: Program) -> String {
        let analyzed = facts::analyze(program.clone());
        fixup(&mut program, &analyzed.facts);
        program.emit()
    }

    fn program(stmts: Vec<Stmt>) -> Program {
        Program {
            items: vec![Item::Fn(func(vec![], None, stmts))],
        }
    }

    fn array(name: &str) -> Stmt {
        Stmt::Let {
            name: name.to_string(),
            mutable: true,
            ty: Some(Type::parse("[i32; 4]")),
            init: Some(Expr::ArrayRepeat {
                elem: Box::new(int(0)),
                len: 4,
            }),
        }
    }

    fn as_mut_ptr(array: &str) -> Expr {
        Expr::MethodCall {
            recv: Box::new(var(array)),
            method: "as_mut_ptr".into(),
            args: Vec::new(),
        }
    }

    fn ptr_add(ptr: Expr, index: Expr) -> Expr {
        Expr::MethodCall {
            recv: Box::new(ptr),
            method: "add".into(),
            args: vec![index],
        }
    }

    fn field(buffer: &str, field: &str) -> Expr {
        Expr::Field {
            base: Box::new(var(buffer)),
            field: field.into(),
        }
    }

    fn unsafe_tail(expr: Expr) -> Expr {
        Expr::Unsafe(Box::new(Block {
            stmts: Vec::new(),
            tail: Some(Box::new(expr)),
        }))
    }

    fn unsafe_assign(target: Expr, value: Expr) -> Stmt {
        Stmt::Unsafe {
            body: Block {
                stmts: vec![IndentStmt {
                    depth: 2,
                    stmt: Stmt::Assign { target, value },
                }],
                tail: None,
            },
        }
    }

    fn deref(expr: Expr) -> Expr {
        Expr::Unary {
            op: UnaryOp::Deref,
            expr: Box::new(expr),
        }
    }

    fn offset_from(lhs: Expr, rhs: Expr) -> Expr {
        Expr::MethodCall {
            recv: Box::new(lhs),
            method: "offset_from".into(),
            args: vec![rhs],
        }
    }

    #[test]
    fn rewrites_cursor_writes_and_distances_without_field_name_roles() {
        let out = apply(program(vec![
            array("storage"),
            let_mut("buf", "buffer_t", var("zeroed")),
            Stmt::Assign {
                target: field("buf", "begin"),
                value: as_mut_ptr("storage"),
            },
            Stmt::Assign {
                target: field("buf", "pos"),
                value: as_mut_ptr("storage"),
            },
            Stmt::Assign {
                target: field("buf", "limit"),
                value: ptr_add(as_mut_ptr("storage"), int(4)),
            },
            temp("_p0", "*mut i32", field("buf", "pos")),
            unsafe_assign(deref(var("_p0")), int(7)),
            temp("_p1", "*mut i32", field("buf", "pos")),
            Stmt::Assign {
                target: field("buf", "pos"),
                value: unsafe_tail(ptr_add(var("_p1"), int(1))),
            },
            temp("_cursor", "*mut i32", field("buf", "pos")),
            temp("_start", "*mut i32", field("buf", "begin")),
            temp(
                "_distance",
                "i64",
                unsafe_tail(Expr::Cast {
                    expr: Box::new(offset_from(var("_cursor"), var("_start"))),
                    ty: Type::parse("i64"),
                }),
            ),
            temp("_end", "*mut i32", field("buf", "limit")),
            temp(
                "_capacity",
                "i64",
                unsafe_tail(Expr::Cast {
                    expr: Box::new(offset_from(var("_end"), var("_start"))),
                    ty: Type::parse("i64"),
                }),
            ),
        ]));

        assert!(out.contains("storage[0] = 7;"));
        assert!(out.contains("let _distance: i64 = 1 as i64;"));
        assert!(out.contains("let _capacity: i64 = 4 as i64;"));
        assert!(!out.contains("offset_from"));
        assert!(!out.contains("*_p0"));
        assert!(!out.contains(".pos"));
        assert!(!out.contains(".begin"));
        assert!(!out.contains(".limit"));
    }

    #[test]
    fn does_not_rewrite_one_past_end_deref() {
        let out = apply(program(vec![
            array("storage"),
            let_mut("buf", "buffer_t", var("zeroed")),
            Stmt::Assign {
                target: field("buf", "limit"),
                value: ptr_add(as_mut_ptr("storage"), int(4)),
            },
            temp("_end", "*mut i32", field("buf", "limit")),
            unsafe_assign(deref(var("_end")), int(7)),
        ]));

        assert!(out.contains("*_end = 7;"));
        assert!(!out.contains("storage[4] = 7;"));
    }

    #[test]
    fn rejects_partial_rewrite_when_pointer_alias_escapes() {
        let out = apply(program(vec![
            array("storage"),
            let_mut("buf", "buffer_t", var("zeroed")),
            Stmt::Assign {
                target: field("buf", "pos"),
                value: as_mut_ptr("storage"),
            },
            temp("_p", "*mut i32", field("buf", "pos")),
            Stmt::Expr(call("consume", vec![var("_p")])),
        ]));

        assert!(out.contains("buf.pos = storage.as_mut_ptr();"));
        assert!(out.contains("let _p: *mut i32 = buf.pos;"));
        assert!(out.contains("consume(_p);"));
    }
}
