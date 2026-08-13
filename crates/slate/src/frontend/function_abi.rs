use crate::backend::rust_ast::{Prim, Type};

pub(super) fn repair_function_signature(
    spelling: Option<&str>,
    params: &mut [Type],
    ret: &mut Option<Type>,
) -> bool {
    let Some(spelling) = spelling else {
        return false;
    };
    let Some((ret_spelling, param_spellings)) = split_function_type(spelling) else {
        return false;
    };
    let param_spellings = param_spellings
        .into_iter()
        .filter(|param| *param != "void" && *param != "...")
        .collect::<Vec<_>>();
    if param_spellings.len() != params.len() {
        return false;
    }
    let mut repaired = false;
    for (param, spelling) in params.iter_mut().zip(param_spellings) {
        repaired |= repair_typedef_type(spelling, param);
    }
    if let Some(ret) = ret {
        repaired |= repair_typedef_type(ret_spelling, ret);
    }
    repaired
}

fn split_function_type(spelling: &str) -> Option<(&str, Vec<&str>)> {
    let open = spelling.find('(')?;
    let close = spelling.rfind(')')?;
    if close < open {
        return None;
    }
    let ret = spelling[..open].trim();
    let params = split_top_level(&spelling[open + 1..close]);
    Some((ret, params))
}

fn split_top_level(types: &str) -> Vec<&str> {
    let mut result = Vec::new();
    let mut start = 0;
    let mut parens = 0;
    let mut brackets = 0;
    for (index, character) in types.char_indices() {
        match character {
            '(' => parens += 1,
            ')' => parens -= 1,
            '[' => brackets += 1,
            ']' => brackets -= 1,
            ',' if parens == 0 && brackets == 0 => {
                result.push(types[start..index].trim());
                start = index + 1;
            }
            _ => {}
        }
    }
    if start < types.len() || !types.trim().is_empty() {
        result.push(types[start..].trim());
    }
    result
}

fn repair_typedef_type(spelling: &str, ty: &mut Type) -> bool {
    if let Some(repaired) = pointer_sized_type(spelling) {
        *ty = repaired;
        return true;
    }
    match ty {
        Type::Ptr { mutable, inner } => spelling.rfind('*').is_some_and(|star| {
            let pointee = spelling[..star].trim();
            let pointee_const = pointee.split_whitespace().any(|word| word == "const");
            let changed = *mutable == pointee_const;
            *mutable = !pointee_const;
            repair_typedef_type(pointee, inner.as_mut()) || changed
        }),
        Type::Array { elem, .. } => spelling
            .find('[')
            .is_some_and(|bracket| repair_typedef_type(spelling[..bracket].trim(), elem.as_mut())),
        _ => false,
    }
}

fn pointer_sized_type(spelling: &str) -> Option<Type> {
    let spelling = spelling
        .split_whitespace()
        .filter(|word| !matches!(*word, "const" | "volatile" | "restrict" | "_Atomic"))
        .collect::<Vec<_>>()
        .join(" ");
    match spelling.as_str() {
        "size_t" | "__size_t" | "rsize_t" | "uintptr_t" => Some(Type::Prim(Prim::Usize)),
        "ptrdiff_t" | "ssize_t" | "__ssize_t" | "intptr_t" => Some(Type::Prim(Prim::Isize)),
        _ => None,
    }
}
