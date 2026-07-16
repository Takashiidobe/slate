//! Prove a local integer slot is only ever touched through atomic ops on its
//! address, so the rewrite can give it native `AtomicN` storage and safe
//! direct method calls.

use crate::fixups::facts::{AtomicLocalFact, FixupFacts, walk};
use crate::fixups::idents::stmt_ident_count;
use crate::rust_ast::{AtomicPlace, AtomicType, Expr, IndentStmt, Item, Prim, Program, Stmt, Type};

pub(in crate::fixups) fn collect_facts(program: &Program, facts: &mut FixupFacts) {
    facts.atomic_locals.clear();
    let mut all = Vec::new();
    for (item_index, item) in program.items.iter().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        for (name, ty) in promotable_locals(&f.body) {
            all.push(AtomicLocalFact { function, name, ty });
        }
    }
    facts.atomic_locals = all;
}

/// A local is promotable when it is declared `let mut <name>: <int> = <init>;`
/// and every other mention of the name is the pointer operand of an atomic op
/// of the matching width: `Ptr(AddrOf { mutable: true, expr: Var(name) })`.
/// Any plain read, write, or address escape leaves it on the baseline path.
fn promotable_locals(body: &[IndentStmt]) -> Vec<(String, AtomicType)> {
    let mut decls: Vec<(String, AtomicType)> = Vec::new();
    collect_decls(body, &mut decls);
    decls
        .into_iter()
        .filter(|(name, ty)| {
            let total: usize = body
                .iter()
                .map(|indent| stmt_ident_count(&indent.stmt, name))
                .sum();
            let mut qualifying = 0usize;
            let mut mismatched = false;
            walk::body_exprs(body, &mut |expr| match atomic_ptr_local(expr) {
                Some((local, node_ty)) if local == name => {
                    if node_ty == Some(*ty) {
                        qualifying += 1;
                    } else {
                        mismatched = true;
                    }
                }
                _ => {}
            });
            // the declaration binder itself accounts for one count
            !mismatched && qualifying > 0 && total == qualifying + 1
        })
        .collect()
}

fn collect_decls(body: &[IndentStmt], decls: &mut Vec<(String, AtomicType)>) {
    for indent in body {
        match &indent.stmt {
            Stmt::Let {
                name,
                mutable: true,
                ty: Some(Type::Prim(prim)),
                init: Some(_),
            } => {
                if let Some(atomic_ty) = prim_atomic_type(*prim) {
                    decls.push((name.clone(), atomic_ty));
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
                collect_decls(then_body, decls);
                collect_decls(else_body, decls);
            }
            Stmt::Loop { body, .. }
            | Stmt::For { body, .. }
            | Stmt::Scope { body }
            | Stmt::LabeledBlock { body, .. } => collect_decls(body, decls),
            Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
                collect_decls(&body.stmts, decls)
            }
            Stmt::Match { arms, .. } => {
                for arm in arms {
                    collect_decls(&arm.body, decls);
                }
            }
            _ => {}
        }
    }
}

/// The promoted local an atomic place points at, when the place is exactly
/// `Ptr(AddrOf { mutable: true, expr: Var(<promoted name>) })`.
pub(in crate::fixups) fn place_local_target(
    place: &AtomicPlace,
    promoted: &std::collections::BTreeMap<String, AtomicType>,
) -> Option<(crate::rust_ast::Ident, AtomicType)> {
    let AtomicPlace::Ptr(ptr) = place else {
        return None;
    };
    let Expr::AddrOf {
        mutable: true,
        expr,
    } = ptr.as_ref()
    else {
        return None;
    };
    let Expr::Var(name) = expr.as_ref() else {
        return None;
    };
    let ty = promoted.get(name.as_str())?;
    Some((name.clone(), *ty))
}

/// The local name an atomic op's pointer operand names, with the op's width.
/// `AtomicRef` yields a `&AtomicN` value whose uses this fact cannot see, so
/// it reports a width mismatch (`None`) to veto promotion.
fn atomic_ptr_local(expr: &Expr) -> Option<(&str, Option<AtomicType>)> {
    let (place, ty) = match expr {
        Expr::AtomicLoad { place, ty, .. }
        | Expr::AtomicStore { place, ty, .. }
        | Expr::AtomicFetch { place, ty, .. }
        | Expr::AtomicSwap { place, ty, .. }
        | Expr::AtomicCompareExchange { place, ty, .. } => (place, Some(*ty)),
        Expr::AtomicRef { place, .. } => (place, None),
        _ => return None,
    };
    let AtomicPlace::Ptr(ptr) = place else {
        return None;
    };
    let Expr::AddrOf {
        mutable: true,
        expr,
    } = ptr.as_ref()
    else {
        return None;
    };
    let Expr::Var(name) = expr.as_ref() else {
        return None;
    };
    Some((name.as_str(), ty))
}

fn prim_atomic_type(prim: Prim) -> Option<AtomicType> {
    Some(match prim {
        Prim::I8 => AtomicType::I8,
        Prim::U8 => AtomicType::U8,
        Prim::I16 => AtomicType::I16,
        Prim::U16 => AtomicType::U16,
        Prim::I32 => AtomicType::I32,
        Prim::U32 => AtomicType::U32,
        Prim::I64 => AtomicType::I64,
        Prim::U64 => AtomicType::U64,
        Prim::Isize => AtomicType::Isize,
        Prim::Usize => AtomicType::Usize,
        Prim::Bool => AtomicType::Bool,
        _ => return None,
    })
}
