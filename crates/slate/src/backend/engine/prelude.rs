use crate::backend::rust_ast::Prim;

pub(crate) fn char_prim(target: &slate_parser::target_info::TargetInfo) -> Prim {
    if target.char_signed {
        Prim::I8
    } else {
        Prim::U8
    }
}

pub(crate) fn long_prim(target: &slate_parser::target_info::TargetInfo) -> Prim {
    if target.long_width == 64 {
        Prim::I64
    } else {
        Prim::I32
    }
}
