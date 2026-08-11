//! Prove an integer slot is only ever touched through atomic ops on its
//! address, so the rewrite can give it native `AtomicN` storage and safe
//! direct method calls.

use crate::backend::facts::walk::Bodies;
use crate::backend::facts::{AtomicGlobalFact, AtomicLocalFact, FunctionId, StaticDeclFact, walk};
use crate::backend::idents::{expr_ident_count, stmt_ident_count};
use crate::backend::rust_ast::{
    AtomicPlace, AtomicType, Expr, FnDef, IndentStmt, Prim, Stmt, Type,
};
pub(in crate::backend) fn collect_for_function<'db>(
    function: FunctionId<'db>,
    f: &FnDef,
) -> Vec<AtomicLocalFact<'db>> {
    promotable_locals(&f.body)
        .into_iter()
        .map(|(name, ty)| AtomicLocalFact { function, name, ty })
        .collect()
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

pub(in crate::backend) fn compute_atomic_globals(
    statics: &[StaticDeclFact],
    bodies: &Bodies,
) -> Vec<AtomicGlobalFact> {
    promotable_globals(statics, bodies)
        .into_iter()
        .map(|(name, ty)| AtomicGlobalFact { name, ty })
        .collect()
}

fn promotable_globals(statics: &[StaticDeclFact], bodies: &Bodies) -> Vec<(String, AtomicType)> {
    let mut decls = Vec::new();
    for static_decl in statics {
        if static_decl.mutable
            && let Type::Prim(prim) = &static_decl.ty
            && let Some(atomic_ty) = prim_atomic_type(*prim)
        {
            decls.push((static_decl.name.clone(), atomic_ty));
        }
    }
    decls
        .into_iter()
        .filter(|(name, ty)| {
            if bodies.values().any(|f| body_declares_local(&f.body, name)) {
                return false;
            }
            let mut total: usize = bodies
                .values()
                .map(|f| {
                    f.body
                        .iter()
                        .map(|indent| stmt_ident_count(&indent.stmt, name))
                        .sum::<usize>()
                })
                .sum();
            total += statics
                .iter()
                .filter(|static_decl| &static_decl.name != name)
                .map(|static_decl| expr_ident_count(&static_decl.init, name))
                .sum::<usize>();
            let mut qualifying = 0usize;
            let mut mismatched = false;
            for f in bodies.values() {
                walk::body_exprs(&f.body, &mut |expr| match atomic_ptr_local(expr) {
                    Some((global, node_ty)) if global == name => {
                        if node_ty == Some(*ty) {
                            qualifying += 1;
                        } else {
                            mismatched = true;
                        }
                    }
                    _ => {}
                });
            }
            !mismatched && qualifying > 0 && total == qualifying
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

fn body_declares_local(body: &[IndentStmt], name: &str) -> bool {
    body.iter().any(|indent| match &indent.stmt {
        Stmt::Let { name: local, .. } => local == name,
        Stmt::LetIf {
            name: local,
            then_body,
            else_body,
            ..
        } => {
            local == name
                || body_declares_local(then_body, name)
                || body_declares_local(else_body, name)
        }
        Stmt::If {
            then_body,
            else_body,
            ..
        } => body_declares_local(then_body, name) || body_declares_local(else_body, name),
        Stmt::Loop { body, .. }
        | Stmt::For { body, .. }
        | Stmt::Scope { body }
        | Stmt::LabeledBlock { body, .. } => body_declares_local(body, name),
        Stmt::Unsafe { body } | Stmt::While { body, .. } | Stmt::Block(body) => {
            body_declares_local(&body.stmts, name)
        }
        Stmt::Match { arms, .. } => arms.iter().any(|arm| body_declares_local(&arm.body, name)),
        _ => false,
    })
}

/// The promoted local an atomic place points at, when the place is exactly
/// `Ptr(AddrOf { mutable: true, expr: Var(<promoted name>) })`.
pub(in crate::backend) fn place_local_target(
    place: &AtomicPlace,
    promoted: &std::collections::BTreeMap<String, AtomicType>,
) -> Option<(crate::backend::rust_ast::Ident, AtomicType)> {
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
