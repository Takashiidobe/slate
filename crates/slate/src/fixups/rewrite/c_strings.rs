use crate::fixups::facts::{AstPath, FixupFacts};
use crate::fixups::support::walk;
use crate::rust_ast::{Expr, Item, Program};

pub(in crate::fixups) fn fixup(program: &mut Program, facts: &FixupFacts) {
    for (item_index, item) in program.items.iter_mut().enumerate() {
        let Item::Fn(f) = item else {
            continue;
        };
        let Some(function) = facts.function_by_item_index(item_index) else {
            continue;
        };
        walk::body_exprs_mut_with_path(&mut f.body, &mut Vec::new(), &mut |expr, path| {
            if let Some(fact) = facts.c_string_literal(function, &AstPath(path.to_vec()))
                && matches!(expr, Expr::ByteStr(_))
            {
                *expr = Expr::CStr(fact.bytes.clone());
                return false;
            }
            true
        });
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::fixups::facts;
    use crate::fixups::test_support::{emit, func, temp};
    use crate::rust_ast::{Expr, Item, Program, Type};

    fn c_char_ptr(bytes: &[u8]) -> Expr {
        Expr::Cast {
            expr: Box::new(Expr::MethodCall {
                recv: Box::new(Expr::ByteStr(bytes.to_vec())),
                method: "as_ptr".to_string(),
                args: Vec::new(),
            }),
            ty: Type::parse("*mut i8"),
        }
    }

    fn run(stmts: Vec<crate::rust_ast::Stmt>) -> String {
        let mut program = Program {
            items: vec![Item::Fn(func(Vec::new(), None, stmts))],
        };
        let facts::AnalyzedProgram { facts, .. } = facts::analyze(program.clone());
        fixup(&mut program, &facts);
        let Item::Fn(f) = program.items.remove(0) else {
            unreachable!();
        };
        emit(f)
    }

    #[test]
    fn rewrites_ascii_nul_terminated_byte_string_pointer_receiver() {
        let out = run(vec![temp("p", "*mut i8", c_char_ptr(b"write error\0"))]);

        assert!(out.contains("let p: *mut i8 = c\"write error\".as_ptr() as *mut i8;"));
        assert!(!out.contains("b\"write error\\0\".as_ptr()"));
    }

    #[test]
    fn leaves_non_c_string_byte_strings_alone() {
        let interior_nul = run(vec![temp("p", "*mut i8", c_char_ptr(b"a\0b\0"))]);
        assert!(interior_nul.contains("b\"a\\0b\\0\".as_ptr() as *mut i8"));

        let non_ascii = run(vec![temp("p", "*mut i8", c_char_ptr(b"\xff\0"))]);
        assert!(non_ascii.contains("b\"\\xff\\0\".as_ptr() as *mut i8"));
    }
}
