//! Rust cleanup passes that run after faithful CIR lowering.

mod call_args;
mod compound_assign;
mod drop_call_results;
mod idents;
mod inline_temps;
mod param_spills;
mod retval;
mod zero_init;

#[cfg(test)]
mod test_support;

use crate::rust_ast::{Item, Program};

pub fn apply(program: Program) -> Program {
    let sigs = call_args::collect_signatures(&program);
    Program {
        items: program
            .items
            .into_iter()
            .map(|item| match item {
                Item::Fn(mut f) => {
                    inline_temps::fixup(&mut f.body);
                    param_spills::fixup(&mut f);
                    zero_init::fixup(&mut f.body);
                    compound_assign::fixup(&mut f.body);
                    call_args::fixup(&mut f.body, &sigs);
                    retval::fixup(&mut f.body);
                    drop_call_results::fixup(&mut f.body);
                    Item::Fn(f)
                }
                item => item,
            })
            .collect(),
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{BinOp, Expr, Prim, RustValue, Stmt, Type};

    #[test]
    fn apply_keeps_migrated_functions_structured() {
        let program = Program {
            items: vec![Item::Fn(migrated_fn(vec![
                Stmt::Let {
                    name: "a".into(),
                    mutable: true,
                    ty: Some(Type::Prim(Prim::I32)),
                    init: Some(Expr::Value(RustValue::I64(0))),
                },
                Stmt::Let {
                    name: "b".into(),
                    mutable: true,
                    ty: Some(Type::Prim(Prim::I32)),
                    init: Some(Expr::Value(RustValue::I64(0))),
                },
                Stmt::Let {
                    name: "__retval".into(),
                    mutable: true,
                    ty: Some(Type::Prim(Prim::I32)),
                    init: Some(Expr::Value(RustValue::I64(0))),
                },
                Stmt::Let {
                    name: "c".into(),
                    mutable: true,
                    ty: Some(Type::Prim(Prim::I32)),
                    init: Some(Expr::Value(RustValue::I64(0))),
                },
                Stmt::Assign {
                    target: Expr::Var("a".into()),
                    value: Expr::Var("arg0".into()),
                },
                Stmt::Assign {
                    target: Expr::Var("b".into()),
                    value: Expr::Var("arg1".into()),
                },
                Stmt::Assign {
                    target: Expr::Var("c".into()),
                    value: bin(BinOp::Add, Expr::Var("a".into()), Expr::Var("b".into())),
                },
                Stmt::Assign {
                    target: Expr::Var("__retval".into()),
                    value: Expr::Var("c".into()),
                },
                Stmt::Return(Some(Expr::Var("__retval".into()))),
            ]))],
        };

        let out = apply(program);
        let Item::Fn(f) = &out.items[0] else {
            panic!("migrated functions must remain structured");
        };
        assert_eq!(f.params[0].name, "a");
        assert!(f.params[0].mutable);
        assert_eq!(
            out.emit(),
            "\
fn add(mut a: i32, mut b: i32) -> i32 {
    let mut c: i32 = a + b;
    return c;
}
"
        );
    }
}
