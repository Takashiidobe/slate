//! Fold a parameter's stack spill directly into a mutable binding when the
//! parameter is spilled to a single local and never otherwise read.

use crate::fixups::idents::{expr_ident, stmt_ident_count};
use crate::rust_ast::{FnDef, Stmt};

pub(super) fn fixup(f: &mut FnDef) {
    let param_names: Vec<String> = f.params.iter().map(|p| p.name.clone()).collect();
    let mut claimed_locals: Vec<String> = Vec::new();
    let mut removed: Vec<usize> = Vec::new();

    for param_index in 0..f.params.len() {
        if f.params[param_index].mutable {
            continue;
        }
        let param_name = f.params[param_index].name.clone();
        let param_ty = f.params[param_index].ty.render();
        let body_uses: usize = f
            .body
            .iter()
            .map(|stmt| stmt_ident_count(&stmt.stmt, &param_name))
            .sum();
        if body_uses != 1 {
            continue;
        }

        let Some((store_index, local)) =
            f.body
                .iter()
                .enumerate()
                .find_map(|(index, stmt)| match &stmt.stmt {
                    Stmt::Assign { target, value } => {
                        let local = expr_ident(target)?;
                        (expr_ident(value) == Some(param_name.as_str()))
                            .then(|| (index, local.to_string()))
                    }
                    _ => None,
                })
        else {
            continue;
        };
        if param_names.iter().any(|name| name == &local)
            || claimed_locals.iter().any(|name| name == &local)
        {
            continue;
        }

        let Some(decl_index) =
            f.body
                .iter()
                .enumerate()
                .take(store_index)
                .find_map(|(index, stmt)| match &stmt.stmt {
                    Stmt::Let {
                        name,
                        mutable: true,
                        ty: Some(ty),
                        ..
                    } if name == &local && ty.render() == param_ty => Some(index),
                    _ => None,
                })
        else {
            continue;
        };

        if f.body[decl_index + 1..store_index]
            .iter()
            .any(|stmt| stmt_ident_count(&stmt.stmt, &local) > 0)
        {
            continue;
        }

        f.params[param_index].name = local.clone();
        f.params[param_index].mutable = true;
        claimed_locals.push(local);
        removed.push(decl_index);
        removed.push(store_index);
    }

    removed.sort_unstable();
    removed.dedup();
    for index in removed.into_iter().rev() {
        f.body.remove(index);
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::BinOp;

    #[test]
    fn folds_parameter_spills_into_direct_bindings() {
        let f = func(
            vec![param("arg0", "i32"), param("arg1", "i32")],
            Some("i32"),
            vec![
                let_mut("a", "i32", int(0)),
                let_mut("b", "i32", int(0)),
                let_mut("__retval", "i32", int(0)),
                let_mut("c", "i32", int(0)),
                assign("a", var("arg0")),
                assign("b", var("arg1")),
                assign("c", bin(BinOp::Add, var("a"), var("b"))),
                assign("__retval", var("c")),
                Stmt::Return(Some(var("__retval"))),
            ],
        );

        assert_eq!(
            after_fn(fixup, f),
            "\
fn f(mut a: i32, mut b: i32) -> i32 {
    let mut __retval: i32 = 0;
    let mut c: i32 = 0;
    c = a + b;
    __retval = c;
    return __retval;
}
"
        );
    }

    #[test]
    fn does_not_fold_when_parameter_is_read_again() {
        let f = func(
            vec![param("arg0", "i32")],
            Some("i32"),
            vec![
                let_mut("a", "i32", int(0)),
                assign("a", var("arg0")),
                Stmt::Return(Some(var("arg0"))),
            ],
        );
        let expected = emit(f.clone());

        assert_eq!(after_fn(fixup, f), expected);
    }

    #[test]
    fn does_not_fold_when_slot_read_before_spill() {
        let f = func(
            vec![param("arg0", "i32")],
            Some("i32"),
            vec![
                let_mut("a", "i32", int(0)),
                let_mut("b", "i32", var("a")),
                assign("a", var("arg0")),
                Stmt::Return(Some(var("b"))),
            ],
        );
        let expected = emit(f.clone());

        assert_eq!(after_fn(fixup, f), expected);
    }

    #[test]
    fn does_not_fold_when_slot_type_differs_from_parameter() {
        let f = func(
            vec![param("arg0", "i32")],
            Some("i64"),
            vec![
                let_mut("a", "i64", int(0)),
                assign("a", var("arg0")),
                Stmt::Return(Some(var("a"))),
            ],
        );
        let expected = emit(f.clone());

        assert_eq!(after_fn(fixup, f), expected);
    }
}
