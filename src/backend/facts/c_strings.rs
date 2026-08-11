use crate::backend::facts::walk;
use crate::backend::facts::{AstPath, CStringLiteralFact, FunctionId, PathSegment};
use crate::backend::rust_ast::{Expr, FnDef, Prim, Type};
pub(in crate::backend) fn collect_for_function<'db>(
    function: FunctionId<'db>,
    f: &FnDef,
) -> Vec<CStringLiteralFact<'db>> {
    let mut out = Vec::new();
    walk::body_exprs_with_path(&f.body, &mut Vec::new(), &mut |expr, path| {
        if let Some(bytes) = c_string_literal_payload(expr) {
            out.push(CStringLiteralFact {
                function,
                receiver_path: receiver_path(path),
                bytes,
            });
        }
    });
    out
}

fn c_string_literal_payload(expr: &Expr) -> Option<Vec<u8>> {
    let Expr::Cast {
        expr: cast_expr,
        ty,
    } = expr
    else {
        return None;
    };
    if !is_char_ptr(ty) {
        return None;
    }
    let Expr::MethodCall { recv, method, args } = cast_expr.as_ref() else {
        return None;
    };
    if method != "as_ptr" || !args.is_empty() {
        return None;
    }
    let Expr::ByteStr(bytes) = recv.as_ref() else {
        return None;
    };
    c_string_payload(bytes)
}

fn receiver_path(path: &[PathSegment]) -> AstPath {
    let mut receiver_path = path.to_vec();
    receiver_path.push(PathSegment::Expr(0));
    receiver_path.push(PathSegment::Expr(0));
    AstPath(receiver_path)
}

fn is_char_ptr(ty: &Type) -> bool {
    let Type::Ptr { inner, .. } = ty else {
        return false;
    };
    match inner.as_ref() {
        Type::Prim(Prim::I8) => true,
        Type::Custom(name) => {
            matches!(
                name.as_str(),
                "libc::c_char" | "std::ffi::c_char" | "core::ffi::c_char"
            )
        }
        _ => false,
    }
}

fn c_string_payload(bytes: &[u8]) -> Option<Vec<u8>> {
    let (&last, payload) = bytes.split_last()?;
    if last != 0 {
        return None;
    }
    if !payload.iter().all(|b| ascii_c_string_byte(*b)) {
        return None;
    }
    Some(payload.to_vec())
}

fn ascii_c_string_byte(byte: u8) -> bool {
    matches!(byte, b'\n' | b'\r' | b'\t' | b'\\' | b'"' | 0x20..=0x7e)
}
