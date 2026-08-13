use crate::backend::rust_ast::{CLibType, Prim, Type};
use crate::function_identity::{FunctionIdentity, Known};

pub(super) fn repair_function_signature(
    identity: FunctionIdentity,
    params: &mut Vec<Type>,
    ret: &mut Option<Type>,
) -> bool {
    let void_ptr = |mutable| Type::Ptr {
        mutable,
        inner: Box::new(Type::CLib(CLibType::VOID)),
    };
    let char_ptr = |mutable| Type::Ptr {
        mutable,
        inner: Box::new(Type::Prim(Prim::I8)),
    };
    let (repaired_params, repaired_ret) = match identity {
        FunctionIdentity::Known(Known::MemSet) => (
            vec![
                void_ptr(true),
                Type::Prim(Prim::I32),
                Type::Prim(Prim::Usize),
            ],
            Some(void_ptr(true)),
        ),
        FunctionIdentity::Known(Known::MemCpy | Known::MemMove) => (
            vec![void_ptr(true), void_ptr(false), Type::Prim(Prim::Usize)],
            Some(void_ptr(true)),
        ),
        FunctionIdentity::Known(Known::MemCmp) => (
            vec![void_ptr(false), void_ptr(false), Type::Prim(Prim::Usize)],
            Some(Type::Prim(Prim::I32)),
        ),
        FunctionIdentity::Known(Known::StrLen) => {
            (vec![char_ptr(false)], Some(Type::Prim(Prim::Usize)))
        }
        FunctionIdentity::Known(Known::StrFromL) => (
            vec![
                char_ptr(true),
                Type::Prim(Prim::Usize),
                char_ptr(false),
                Type::LongDouble,
            ],
            Some(Type::Prim(Prim::I32)),
        ),
        FunctionIdentity::Known(_) | FunctionIdentity::Unknown => return false,
    };
    *params = repaired_params;
    *ret = repaired_ret;
    true
}
