//! Give proven non-escaping `_Atomic` slots native atomic storage: the
//! declaration becomes `let <name> = std::sync::atomic::AtomicN::new(<init>);`
//! and every `unsafe { AtomicN::from_ptr(addr_of_mut!(<name>)).op(..) }` view
//! becomes a safe direct method call on the slot.

use std::collections::BTreeMap;

use crate::fixups::facts::{FixupFacts, atomic_locals};
use crate::fixups::support::walk;
use crate::fixups::trace::{
    Pass as TracePass, RewriteEvent, TraceLocation, TraceLogger, TraceSnippet, fact,
};
use crate::rust_ast::{AtomicPlace, AtomicType, Expr, IndentStmt, Item, Program, Stmt, Type};

pub(in crate::fixups) fn fixup(program: &mut Program, facts: &FixupFacts) -> bool {
    let mut logger = crate::fixups::trace::NoopLogger;
    AtomicLocals::new(&mut logger).fixup(program, facts)
}

pub(in crate::fixups) struct AtomicLocals<'a> {
    logger: &'a mut dyn TraceLogger,
}

impl<'a> AtomicLocals<'a> {
    pub(in crate::fixups) fn new(logger: &'a mut dyn TraceLogger) -> Self {
        Self { logger }
    }

    pub(in crate::fixups) fn fixup(&mut self, program: &mut Program, facts: &FixupFacts) -> bool {
        let before = self.logger.is_enabled().then(|| program.emit());
        let changed = fixup_impl(program, facts);
        if changed && let Some(before) = before {
            self.logger.rewrite(RewriteEvent {
                pass: TracePass::AtomicLocals,
                kind: "promote_atomic_locals".into(),
                location: TraceLocation::default(),
                before: vec![TraceSnippet::new("program", before.trim_end())],
                after: vec![TraceSnippet::new("program", program.emit().trim_end())],
                facts: vec![
                    fact("atomic_locals", facts.atomic_locals.len().to_string()),
                    fact("atomic_globals", facts.atomic_globals.len().to_string()),
                ],
            });
        }
        changed
    }
}

fn fixup_impl(program: &mut Program, facts: &FixupFacts) -> bool {
    let mut changed = false;
    let promoted_globals: BTreeMap<String, AtomicType> = facts
        .atomic_globals
        .iter()
        .map(|fact| (fact.name.clone(), fact.ty))
        .collect();
    rewrite_statics(program, &promoted_globals, &mut changed);
    for (item_index, item) in program.items.iter_mut().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        let mut promoted: BTreeMap<String, AtomicType> = facts
            .atomic_locals
            .iter()
            .filter(|fact| fact.function == function)
            .map(|fact| (fact.name.clone(), fact.ty))
            .collect();
        for (name, ty) in &promoted_globals {
            promoted.entry(name.clone()).or_insert(*ty);
        }
        if promoted.is_empty() {
            continue;
        }
        rewrite_decls(&mut f.body, &promoted, &mut changed);
        walk::body_exprs_mut_with(&mut f.body, &mut |expr| {
            rewrite_atomic_expr(expr, &promoted, &mut changed);
            true
        });
    }
    changed
}

fn rewrite_statics(
    program: &mut Program,
    promoted: &BTreeMap<String, AtomicType>,
    changed: &mut bool,
) {
    for item in &mut program.items {
        match item {
            Item::Static {
                mutable,
                name,
                ty,
                init,
                ..
            } => {
                if let Some(atomic_ty) = promoted.get(name) {
                    *mutable = false;
                    *ty = Type::Custom(atomic_type_path(*atomic_ty).into());
                    *init = Expr::AtomicNew {
                        ty: *atomic_ty,
                        value: Box::new(init.clone()),
                    };
                    *changed = true;
                }
            }
            Item::Cfg { item, .. } => {
                let mut nested = Program {
                    items: vec![item.as_ref().clone()],
                };
                rewrite_statics(&mut nested, promoted, changed);
                if let Some(rewritten) = nested.items.into_iter().next() {
                    **item = rewritten;
                }
            }
            _ => {}
        }
    }
}

fn rewrite_decls(
    body: &mut [IndentStmt],
    promoted: &BTreeMap<String, AtomicType>,
    changed: &mut bool,
) {
    for indent in body.iter_mut() {
        match &mut indent.stmt {
            Stmt::Let {
                name,
                mutable: mutable @ true,
                ty,
                init: Some(init),
            } => {
                if let Some(atomic_ty) = promoted.get(name) {
                    *mutable = false;
                    *ty = None;
                    *init = Expr::AtomicNew {
                        ty: *atomic_ty,
                        value: Box::new(init.clone()),
                    };
                    *changed = true;
                }
            }
            Stmt::If {
                then_body,
                else_body,
                ..
            }
            | Stmt::LetIf {
                then_body,
                else_body,
                ..
            } => {
                rewrite_decls(then_body, promoted, changed);
                rewrite_decls(else_body, promoted, changed);
            }
            Stmt::Loop { body, .. }
            | Stmt::For { body, .. }
            | Stmt::Scope { body }
            | Stmt::LabeledBlock { body, .. } => rewrite_decls(body, promoted, changed),
            Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
                rewrite_decls(&mut body.stmts, promoted, changed)
            }
            Stmt::Match { arms, .. } => {
                for arm in arms {
                    rewrite_decls(&mut arm.body, promoted, changed);
                }
            }
            _ => {}
        }
    }
}

/// `unsafe { <atomic op on promoted local> }` unwraps to the bare op with a
/// `Local` place; a bare op (already unwrapped) just switches its place.
fn rewrite_atomic_expr(
    expr: &mut Expr,
    promoted: &BTreeMap<String, AtomicType>,
    changed: &mut bool,
) {
    if let Expr::Unsafe(block) = expr
        && block.stmts.is_empty()
        && let Some(tail) = &mut block.tail
        && promote_place(tail, promoted)
    {
        *expr = std::mem::replace(tail.as_mut(), Expr::Todo(String::new()));
        *changed = true;
        return;
    }
    if promote_place(expr, promoted) {
        *changed = true;
    }
}

fn promote_place(expr: &mut Expr, promoted: &BTreeMap<String, AtomicType>) -> bool {
    let (place, ty) = match expr {
        Expr::AtomicLoad { place, ty, .. }
        | Expr::AtomicStore { place, ty, .. }
        | Expr::AtomicFetch { place, ty, .. }
        | Expr::AtomicSwap { place, ty, .. }
        | Expr::AtomicCompareExchange { place, ty, .. } => (place, *ty),
        _ => return false,
    };
    let Some((local, local_ty)) = atomic_locals::place_local_target(place, promoted) else {
        return false;
    };
    if local_ty != ty {
        return false;
    }
    *place = AtomicPlace::Local(local);
    true
}

fn atomic_type_path(ty: AtomicType) -> &'static str {
    match ty {
        AtomicType::I8 => "std::sync::atomic::AtomicI8",
        AtomicType::U8 => "std::sync::atomic::AtomicU8",
        AtomicType::I16 => "std::sync::atomic::AtomicI16",
        AtomicType::U16 => "std::sync::atomic::AtomicU16",
        AtomicType::I32 => "std::sync::atomic::AtomicI32",
        AtomicType::U32 => "std::sync::atomic::AtomicU32",
        AtomicType::I64 => "std::sync::atomic::AtomicI64",
        AtomicType::U64 => "std::sync::atomic::AtomicU64",
        AtomicType::Isize => "std::sync::atomic::AtomicIsize",
        AtomicType::Usize => "std::sync::atomic::AtomicUsize",
        AtomicType::Bool => "std::sync::atomic::AtomicBool",
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::facts;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{AtomicOrdering, Block, Item, Stmt, Type, Visibility};

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

    fn addr_of_mut(name: &str) -> Expr {
        Expr::AddrOf {
            mutable: true,
            expr: Box::new(var(name)),
        }
    }

    fn unsafe_expr(expr: Expr) -> Expr {
        Expr::Unsafe(Box::new(Block {
            stmts: Vec::new(),
            tail: Some(Box::new(expr)),
        }))
    }

    fn atomic_store(name: &str, value: Expr) -> Expr {
        Expr::AtomicStore {
            ty: AtomicType::I32,
            place: AtomicPlace::Ptr(Box::new(addr_of_mut(name))),
            value: Box::new(value),
            ordering: AtomicOrdering::SeqCst,
        }
    }

    fn atomic_load(name: &str) -> Expr {
        Expr::AtomicLoad {
            ty: AtomicType::I32,
            place: AtomicPlace::Ptr(Box::new(addr_of_mut(name))),
            ordering: AtomicOrdering::SeqCst,
        }
    }

    #[test]
    fn promotes_local_used_only_through_atomic_ops() {
        let out = apply(program(vec![
            let_mut("a", "i32", int(0)),
            Stmt::Expr(unsafe_expr(atomic_store("a", int(100)))),
            temp("loaded", "i32", unsafe_expr(atomic_load("a"))),
        ]));
        assert_eq!(
            out,
            "\
fn f() {
    let a = std::sync::atomic::AtomicI32::new(0);
    a.store(100, std::sync::atomic::Ordering::SeqCst);
    let loaded: i32 = a.load(std::sync::atomic::Ordering::SeqCst);
}
"
        );
    }

    #[test]
    fn plain_read_keeps_baseline_from_ptr_path() {
        let out = apply(program(vec![
            let_mut("a", "i32", int(0)),
            Stmt::Expr(unsafe_expr(atomic_store("a", int(100)))),
            temp("copy", "i32", var("a")),
        ]));
        assert!(out.contains("let mut a: i32 = 0;"));
        assert!(out.contains("unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(a)).store(100, std::sync::atomic::Ordering::SeqCst) }"));
    }

    #[test]
    fn escaping_address_keeps_baseline_from_ptr_path() {
        let out = apply(program(vec![
            let_mut("a", "i32", int(0)),
            Stmt::Expr(unsafe_expr(atomic_store("a", int(1)))),
            Stmt::Expr(call("observe", vec![addr_of_mut("a")])),
        ]));
        assert!(out.contains("let mut a: i32 = 0;"));
        assert!(out.contains("from_ptr"));
    }

    #[test]
    fn width_mismatch_keeps_baseline_from_ptr_path() {
        let out = apply(program(vec![
            let_mut("a", "i64", int(0)),
            Stmt::Expr(unsafe_expr(atomic_store("a", int(1)))),
        ]));
        assert!(out.contains("let mut a: i64 = 0;"));
        assert!(out.contains("from_ptr"));
    }

    fn program_with_static(name: &str, stmts: Vec<Stmt>) -> Program {
        Program {
            items: vec![
                Item::Static {
                    attrs: Vec::new(),
                    vis: Visibility::Private,
                    mutable: true,
                    name: name.into(),
                    ty: Type::parse("i32"),
                    init: int(0),
                },
                Item::Fn(func(vec![], None, stmts)),
            ],
        }
    }

    #[test]
    fn promotes_global_used_only_through_atomic_ops() {
        let out = apply(program_with_static(
            "counter",
            vec![
                Stmt::Expr(unsafe_expr(atomic_store("counter", int(100)))),
                temp("loaded", "i32", unsafe_expr(atomic_load("counter"))),
            ],
        ));
        assert_eq!(
            out,
            "\
static counter: std::sync::atomic::AtomicI32 = std::sync::atomic::AtomicI32::new(0);

fn f() {
    counter.store(100, std::sync::atomic::Ordering::SeqCst);
    let loaded: i32 = counter.load(std::sync::atomic::Ordering::SeqCst);
}
"
        );
    }

    #[test]
    fn plain_global_read_keeps_baseline_from_ptr_path() {
        let out = apply(program_with_static(
            "counter",
            vec![
                Stmt::Expr(unsafe_expr(atomic_store("counter", int(100)))),
                temp("copy", "i32", var("counter")),
            ],
        ));
        assert!(out.contains("static mut counter: i32 = 0;"));
        assert!(out.contains("AtomicI32::from_ptr(std::ptr::addr_of_mut!(counter))"));
    }
}
