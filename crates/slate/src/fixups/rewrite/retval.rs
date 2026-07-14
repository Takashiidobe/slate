//! Collapse a return-value slot store into the final return or main exit when
//! the slot is used only for that round trip.

use crate::fixups::facts::{
    AstPath, BindingId, ControlFlowSubject, FixupFacts, FunctionId, PathSegment, PlaceAccess,
    PlaceKind,
};
use crate::fixups::idents::expr_ident;
use crate::rust_ast::{Expr, FnDef, IndentStmt, Path, Prim, Stmt, Type};

pub(in crate::fixups) fn fixup(f: &mut FnDef, function: FunctionId, facts: &FixupFacts) {
    collapse_return_slot(&mut f.body, function, facts);
    if f.name == "main" {
        collapse_main_exit_slot(&mut f.body, function, facts);
    }
}

fn collapse_return_slot(body: &mut Vec<IndentStmt>, function: FunctionId, facts: &FixupFacts) {
    let Some(fact) = facts
        .retval_collapses
        .iter()
        .find(|fact| fact.function == function)
    else {
        return;
    };
    let Some(ret_index) = direct_stmt_index(&fact.return_path) else {
        return;
    };
    let Some(value_index) = direct_stmt_index(&fact.value_path) else {
        return;
    };
    if ret_index >= body.len() || value_index >= body.len() {
        return;
    }
    let value = match &body[value_index].stmt {
        Stmt::Let {
            init: Some(init), ..
        } => init.clone(),
        Stmt::Assign { value, .. } => value.clone(),
        _ => return,
    };
    let mut remove = Vec::new();
    for path in &fact.remove_paths {
        let Some(index) = direct_stmt_index(path) else {
            return;
        };
        if index >= body.len() || index == ret_index {
            return;
        }
        remove.push(index);
    }
    remove.sort_unstable();
    remove.dedup();

    let Stmt::Return(Some(_)) = &body[ret_index].stmt else {
        return;
    };
    body[ret_index].stmt = Stmt::Return(Some(value));
    for index in remove.into_iter().rev() {
        body.remove(index);
    }
}

fn collapse_main_exit_slot(body: &mut Vec<IndentStmt>, function: FunctionId, facts: &FixupFacts) {
    let Some((exit_index, temp_name, cast_ty)) =
        body.iter()
            .enumerate()
            .find_map(|(index, stmt)| match &stmt.stmt {
                Stmt::Expr(expr) if reachable_stmt(function, facts, &stmt_path(index)) => {
                    main_exit_arg_temp(expr)
                        .map(|(name, ty)| (index, name.to_string(), ty.cloned()))
                }
                _ => None,
            })
    else {
        return;
    };
    if exit_index < 2 {
        return;
    }

    let temp_index = exit_index - 1;
    let retval_name = match &body[temp_index].stmt {
        Stmt::Let {
            name,
            mutable: false,
            init: Some(init),
            ..
        } if name == &temp_name => expr_ident(init).map(str::to_string),
        _ => None,
    };
    let Some(retval_name) = retval_name else {
        return;
    };
    let temp_path = AstPath(stmt_path(temp_index));
    let Some(temp_binding) = facts.binding_by_local_path(function, &temp_name, &temp_path) else {
        return;
    };
    let Some(retval_binding) = facts
        .bindings
        .iter()
        .find(|binding| binding.function == function && binding.name == retval_name)
        .map(|binding| binding.id)
    else {
        return;
    };

    let store_index = temp_index - 1;
    let value = match &body[store_index].stmt {
        Stmt::Assign { target, value }
            if store_writes_binding(function, facts, store_index, retval_binding, target) =>
        {
            value.clone()
        }
        _ => return,
    };

    let Some(retval_def_use) = facts.def_use(retval_binding) else {
        return;
    };
    let Some(temp_def_use) = facts.def_use(temp_binding) else {
        return;
    };
    if retval_def_use.definition.0.len() != 1
        || retval_def_use.reads != [AstPath(stmt_path(temp_index))]
        || retval_def_use.writes != [AstPath(stmt_path(store_index))]
        || temp_def_use.reads != [AstPath(stmt_path(exit_index))]
        || !temp_def_use.writes.is_empty()
    {
        return;
    }
    let [PathSegment::Stmt(decl_index)] = retval_def_use.definition.0.as_slice() else {
        return;
    };

    let replacement = if let Some(ty) = cast_ty {
        Expr::Cast {
            expr: Box::new(value),
            ty,
        }
    } else {
        value
    };

    let Stmt::Expr(expr) = &mut body[exit_index].stmt else {
        unreachable!();
    };
    replace_main_exit_arg(expr, replacement);
    let mut remove = [temp_index, store_index, *decl_index];
    remove.sort_unstable();
    for index in remove.into_iter().rev() {
        body.remove(index);
    }
}

fn main_exit_arg_temp(expr: &Expr) -> Option<(&str, Option<&Type>)> {
    let Expr::Call { func, args } = expr else {
        return None;
    };
    if !is_std_process_exit(func) || args.len() != 1 {
        return None;
    }
    match &args[0] {
        Expr::Cast { expr, ty } if matches!(ty, Type::Prim(Prim::I32)) => {
            expr_ident(expr).map(|name| (name, Some(ty)))
        }
        arg => expr_ident(arg).map(|name| (name, None)),
    }
}

fn replace_main_exit_arg(expr: &mut Expr, replacement: Expr) {
    let Expr::Call { args, .. } = expr else {
        return;
    };
    args[0] = replacement;
}

fn is_std_process_exit(expr: &Expr) -> bool {
    let Expr::Path(Path { segments }) = expr else {
        return false;
    };
    let expected = ["std", "process", "exit"];
    segments.len() == expected.len()
        && segments
            .iter()
            .zip(expected)
            .all(|(segment, expected)| segment.as_str() == expected)
}

fn store_writes_binding(
    function: FunctionId,
    facts: &FixupFacts,
    index: usize,
    binding: BindingId,
    target: &Expr,
) -> bool {
    let Some(name) = facts.binding_name(binding) else {
        return false;
    };
    if expr_ident(target) != Some(name) {
        return false;
    }
    facts
        .place(function, &AstPath(stmt_path(index)))
        .is_some_and(|fact| {
            fact.access == PlaceAccess::Write
                && fact.ordinary_slot
                && matches!(&fact.kind, PlaceKind::Local { name: place } if place == name)
        })
}

fn reachable_stmt(function: FunctionId, facts: &FixupFacts, path: &[PathSegment]) -> bool {
    facts
        .control_flow(function, ControlFlowSubject::Stmt, &AstPath(path.to_vec()))
        .is_some_and(|fact| fact.reachable)
}

fn stmt_path(index: usize) -> Vec<PathSegment> {
    vec![PathSegment::Stmt(index)]
}

fn direct_stmt_index(path: &AstPath) -> Option<usize> {
    let [PathSegment::Stmt(index)] = path.0.as_slice() else {
        return None;
    };
    Some(*index)
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::test_support::*;
    use crate::rust_ast::{BinOp, Ident, Item, Program, Visibility};

    fn retval_body(program: &mut Program) {
        let analyzed = crate::fixups::facts::analyze(program.clone());
        let Item::Fn(f) = &mut program.items[0] else {
            unreachable!();
        };
        collapse_return_slot(&mut f.body, FunctionId(0), &analyzed.facts);
    }

    fn retval_after_body(
        params: Vec<crate::rust_ast::FnParam>,
        ret: Option<&str>,
        stmts: Vec<Stmt>,
    ) -> String {
        let mut program = Program {
            items: vec![Item::Fn(func(params, ret, stmts))],
        };
        retval_body(&mut program);
        program.emit()
    }

    fn fixed_fn(mut f: FnDef) -> String {
        let program = Program {
            items: vec![Item::Fn(f.clone())],
        };
        let analyzed = crate::fixups::facts::analyze(program);
        fixup(&mut f, FunctionId(0), &analyzed.facts);
        emit(f)
    }

    fn std_process_exit(expr: Expr) -> Expr {
        Expr::Call {
            func: Box::new(Expr::Path(Path::new(
                ["std", "process", "exit"].map(Ident::from),
            ))),
            args: vec![expr],
        }
    }

    #[test]
    fn collapses_retval_store_into_return() {
        let out = retval_after_body(
            vec![],
            Some("i32"),
            vec![
                let_mut("__retval", "i32", int(0)),
                let_mut("c", "i32", int(0)),
                assign("c", bin(BinOp::Add, var("a"), var("b"))),
                assign("__retval", var("c")),
                Stmt::Return(Some(var("__retval"))),
            ],
        );

        assert_eq!(
            out,
            "\
fn f() -> i32 {
    let mut c: i32 = 0;
    c = a + b;
    return c;
}
"
        );
    }

    #[test]
    fn collapses_declaration_initialized_retval_return() {
        let out = retval_after_body(
            vec![],
            Some("i32"),
            vec![
                let_mut("__retval", "i32", bin(BinOp::BitAnd, var("a"), var("b"))),
                Stmt::Return(Some(var("__retval"))),
            ],
        );

        assert_eq!(
            out,
            "\
fn f() -> i32 {
    return a & b;
}
"
        );
    }

    #[test]
    fn does_not_collapse_declaration_initialized_retval_when_read_elsewhere() {
        let stmts = vec![
            let_mut("__retval", "i32", bin(BinOp::BitAnd, var("a"), var("b"))),
            let_mut("x", "i32", var("__retval")),
            Stmt::Return(Some(var("__retval"))),
        ];
        let expected = emit(func(vec![], Some("i32"), stmts.clone()));

        assert_eq!(retval_after_body(vec![], Some("i32"), stmts), expected);
    }

    #[test]
    fn does_not_collapse_when_retval_read_elsewhere() {
        let stmts = vec![
            let_mut("__retval", "i32", int(0)),
            assign("__retval", int(1)),
            let_mut("x", "i32", var("__retval")),
            assign("__retval", var("x")),
            Stmt::Return(Some(var("__retval"))),
        ];
        let expected = emit(func(vec![], Some("i32"), stmts.clone()));

        assert_eq!(retval_after_body(vec![], Some("i32"), stmts), expected);
    }

    #[test]
    fn does_not_collapse_when_store_is_not_immediately_before_return() {
        let stmts = vec![
            let_mut("__retval", "i32", int(0)),
            assign("__retval", int(1)),
            let_mut("x", "i32", int(2)),
            Stmt::Return(Some(var("__retval"))),
        ];
        let expected = emit(func(vec![], Some("i32"), stmts.clone()));

        assert_eq!(retval_after_body(vec![], Some("i32"), stmts), expected);
    }

    #[test]
    fn collapses_main_retval_store_into_exit() {
        let out = fixed_fn(FnDef {
            vis: Visibility::Private,
            unsafe_: false,
            extern_c: false,
            name: "main".into(),
            params: vec![],
            ret: None,
            body: vec![
                let_mut("__retval", "i32", int(0)),
                assign("__retval", int(0)),
                temp("_v1", "i32", var("__retval")),
                Stmt::Expr(std_process_exit(Expr::Cast {
                    expr: Box::new(var("_v1")),
                    ty: Type::Prim(Prim::I32),
                })),
            ]
            .into_iter()
            .map(|stmt| IndentStmt { depth: 1, stmt })
            .collect(),
        });

        assert_eq!(
            out,
            "\
fn main() {
    std::process::exit(0 as i32);
}
"
        );
    }

    #[test]
    fn does_not_collapse_main_when_retval_read_elsewhere() {
        let f = FnDef {
            vis: Visibility::Private,
            unsafe_: false,
            extern_c: false,
            name: "main".into(),
            params: vec![],
            ret: None,
            body: vec![
                let_mut("__retval", "i32", int(0)),
                assign("__retval", int(1)),
                let_mut("x", "i32", var("__retval")),
                assign("__retval", var("x")),
                temp("_v1", "i32", var("__retval")),
                Stmt::Expr(std_process_exit(Expr::Cast {
                    expr: Box::new(var("_v1")),
                    ty: Type::Prim(Prim::I32),
                })),
            ]
            .into_iter()
            .map(|stmt| IndentStmt { depth: 1, stmt })
            .collect(),
        };
        let expected = emit(f.clone());

        assert_eq!(fixed_fn(f), expected);
    }
}
