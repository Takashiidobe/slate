//! Rust cleanup passes that run after faithful CIR lowering.

mod borrow_alias;
mod call_args;
mod calls;
mod compound_assign;
mod control_flow;
mod counted_loop;
mod def_use;
mod drop_call_results;
mod effects;
mod facts;
mod idents;
mod inline_temps;
mod loop_shapes;
mod param_spills;
mod places;
mod printf_format;
mod ptr_len;
mod remove_mut;
mod retval;
mod slice_index;
mod string_copy;
mod string_libc;
mod string_lift;
mod strings;
mod support;
mod values;
mod zero_init;

#[cfg(test)]
mod test_support;

use crate::rust_ast::{Item, Program};

pub fn apply(program: Program) -> Program {
    let facts::AnalyzedProgram { program, .. } = facts::analyze(program);
    let sigs = call_args::collect_signatures(&program);
    let mut program = Program {
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
                    retval::fixup(&mut f);
                    drop_call_results::fixup(&mut f.body);
                    string_lift::fixup(&mut f.body);
                    Item::Fn(f)
                }
                item => item,
            })
            .collect(),
    };
    let facts::AnalyzedProgram {
        program: analyzed_program,
        mut facts,
    } = facts::analyze(program);
    program = analyzed_program;
    ptr_len::collect_facts(&program, &mut facts);
    ptr_len::fixup(&mut program, &facts);
    slice_index::collect_facts(&program, &mut facts);
    counted_loop::collect_facts(&program, &mut facts);
    loop_shapes::collect_facts(&program, &mut facts);
    string_copy::fixup(&mut program);
    string_libc::fixup(&mut program);
    let facts::AnalyzedProgram { mut program, facts } = facts::analyze(program);
    for (item_index, item) in program.items.iter_mut().enumerate() {
        if let Item::Fn(f) = item
            && let Some(function) = facts.function_by_item_index(item_index)
        {
            remove_mut::fixup(f, function, &facts);
        }
    }
    printf_format::fixup(&mut program);
    program
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
        assert!(!f.params[0].mutable);
        assert_eq!(
            out.emit(),
            "\
fn add(a: i32, b: i32) -> i32 {
    let c: i32 = a + b;
    return c;
}
"
        );
    }
}
