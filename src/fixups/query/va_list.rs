use crate::fixups::facts::{AstPath, PathSegment};
use crate::fixups::support::walk;
use crate::rust_ast::{Expr, IndentStmt};

pub(super) fn is_clone_of(expr: &Expr, param: &str) -> bool {
    matches!(
        expr,
        Expr::MethodCall { recv, method, args }
            if method == "clone"
                && args.is_empty()
                && matches!(&**recv, Expr::Var(name) if name.as_str() == param)
    )
}

pub(super) fn remove_alias_stmts(
    body: &mut Vec<IndentStmt>,
    local_decl_path: &AstPath,
    clone_assign_path: &AstPath,
) -> bool {
    remove_at(body, local_decl_path, clone_assign_path, &mut Vec::new()) == 2
}

fn remove_at(
    body: &mut Vec<IndentStmt>,
    local_decl_path: &AstPath,
    clone_assign_path: &AstPath,
    path: &mut Vec<PathSegment>,
) -> usize {
    let mut removed = 0;
    for index in (0..body.len()).rev() {
        let mut stmt_path = path.to_vec();
        stmt_path.push(PathSegment::Stmt(index));
        let stmt_path = AstPath(stmt_path);
        if local_decl_path == &stmt_path || clone_assign_path == &stmt_path {
            body.remove(index);
            removed += 1;
        }
    }

    if removed > 0 {
        return removed;
    }

    for (index, indent) in body.iter_mut().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            walk::nested_body_vecs_mut_with_path(&mut indent.stmt, path, &mut |body, path| {
                removed += remove_at(body, local_decl_path, clone_assign_path, path);
            });
        });
        if removed > 0 {
            break;
        }
    }
    removed
}
