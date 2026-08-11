use crate::backend::facts::{AstPath, PathSegment};
use crate::backend::rust_ast::{Expr, Path, Prim, RustValue, Stmt, Type};
use crate::backend::trace::Pass;

use super::super::{EditSet, Field, Function, FunctionRef, ItemCaseContext, QueryRule, Rejection};

pub(in crate::backend) fn rewrite() -> QueryRule<Function> {
    QueryRule::new(
        Pass::MainZeroExit,
        "remove_trailing_zero_exit",
        Function {
            name: Field::eq("main".into()),
            ..Default::default()
        },
    )
    .case("trailing_zero_exit", trailing_zero_exit_case)
}

fn trailing_zero_exit_case<'db>(
    case: &mut ItemCaseContext<'_, '_>,
    function: &FunctionRef<'db>,
) -> Result<EditSet, Rejection> {
    let definition = case.fact(|query| query.function_snapshot(function))?;
    let Some(last_index) = definition.body.len().checked_sub(1) else {
        return Err(case.reject());
    };
    case.require(is_zero_exit(&definition.body[last_index].stmt))?;
    let mut edits = EditSet::new();
    edits.push_replace_statement(
        function.item_index,
        AstPath(vec![PathSegment::Stmt(last_index)]),
        None,
    );
    Ok(edits)
}

fn is_zero_exit(stmt: &Stmt) -> bool {
    let Stmt::Expr(Expr::Call { func, args, .. }) = stmt else {
        return false;
    };
    if !is_std_process_exit(func) || args.len() != 1 {
        return false;
    }
    is_zero_exit_arg(&args[0])
}

fn is_zero_exit_arg(expr: &Expr) -> bool {
    match expr {
        Expr::Value(RustValue::I64(0) | RustValue::I128(0)) => true,
        Expr::Cast {
            expr,
            ty: Type::Prim(Prim::I32),
        } => is_zero_exit_arg(expr),
        _ => false,
    }
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
