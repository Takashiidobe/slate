use std::collections::BTreeSet;

use crate::fixups::facts::{AstPath, PathSegment};
use crate::fixups::support::walk;
use crate::rust_ast::{IndentStmt, Stmt};

pub(super) fn clear_local_mut(
    body: &mut [IndentStmt],
    eligible: &BTreeSet<AstPath>,
    path: &mut Vec<PathSegment>,
) -> bool {
    let mut changed = false;
    for (index, indent) in body.iter_mut().enumerate() {
        walk::with_path_segment(path, PathSegment::Stmt(index), |path| {
            if let Stmt::Let { mutable, .. } | Stmt::LetIf { mutable, .. } = &mut indent.stmt
                && eligible.contains(&AstPath(path.clone()))
            {
                changed |= *mutable;
                *mutable = false;
            }
            walk::nested_body_vecs_mut_with_path(&mut indent.stmt, path, &mut |body, path| {
                changed |= clear_local_mut(body, eligible, path);
            });
        });
    }
    changed
}
