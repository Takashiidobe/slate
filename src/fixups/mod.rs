//! Rust cleanup passes that run after faithful CIR lowering.

use crate::rust_ast::{Item, Program};

pub fn apply(program: Program) -> Program {
    Program {
        items: program
            .items
            .into_iter()
            .map(|item| match item {
                Item::Raw(text) => Item::Raw(inline_single_use_temps(&text)),
                item => item,
            })
            .collect(),
    }
}

fn inline_single_use_temps(text: &str) -> String {
    let mut lines: Vec<String> = text.lines().map(str::to_string).collect();
    let trailing_newline = text.ends_with('\n');

    loop {
        let mut changed = false;
        for i in 0..lines.len() {
            let Some(def) = parse_temp_let(&lines[i]) else {
                continue;
            };
            if !is_pure_expr(def.expr) {
                continue;
            }
            let Some(use_index) = single_safe_use(&lines, i, def.name, def.expr) else {
                continue;
            };
            lines[use_index] = replace_ident_once(&lines[use_index], def.name, def.expr);
            lines.remove(i);
            changed = true;
            break;
        }
        if !changed {
            break;
        }
    }

    let mut out = lines.join("\n");
    if trailing_newline {
        out.push('\n');
    }
    out
}

#[derive(Debug, Clone, Copy)]
struct TempLet<'a> {
    name: &'a str,
    expr: &'a str,
}

fn parse_temp_let(line: &str) -> Option<TempLet<'_>> {
    let trimmed = line.trim_start();
    let rest = trimmed.strip_prefix("let ")?;
    let (name, rest) = rest.split_once(':')?;
    if !is_temp_name(name) {
        return None;
    }
    let (_, expr) = rest.split_once(" = ")?;
    Some(TempLet {
        name,
        expr: expr.strip_suffix(';')?.trim(),
    })
}

fn single_safe_use(lines: &[String], def_index: usize, name: &str, expr: &str) -> Option<usize> {
    let source_vars = source_vars(expr);
    let mut found = None;

    for (index, line) in lines.iter().enumerate().skip(def_index + 1) {
        let uses = ident_count(line, name);
        if uses > 0 {
            if uses == 1
                && found.is_none()
                && !contains_call(line)
                && !is_method_receiver_use(line, name)
            {
                found = Some(index);
                continue;
            }
            return None;
        }

        if found.is_some() {
            continue;
        }

        if let Some(def) = parse_temp_let(line) {
            if !is_pure_expr(def.expr) {
                return None;
            }
        } else {
            return None;
        }

        if source_vars.iter().any(|var| assigns_to(line, var)) {
            return None;
        }
    }

    found
}

fn is_temp_name(name: &str) -> bool {
    name.strip_prefix("_v")
        .is_some_and(|rest| !rest.is_empty() && rest.bytes().all(|b| b.is_ascii_digit()))
}

fn is_pure_expr(expr: &str) -> bool {
    if expr.contains("unsafe")
        || expr.contains('{')
        || expr.contains('}')
        || expr.contains('[')
        || expr.contains(']')
        || expr.contains("::")
        || expr.contains('.')
        || expr.contains('!')
    {
        return false;
    }
    !contains_call(expr)
}

fn contains_call(expr: &str) -> bool {
    let bytes = expr.as_bytes();
    let mut i = 0;
    while i < bytes.len() {
        if is_ident_start(bytes[i]) {
            let start = i;
            i += 1;
            while i < bytes.len() && is_ident_continue(bytes[i]) {
                i += 1;
            }
            let mut j = i;
            while j < bytes.len() && bytes[j].is_ascii_whitespace() {
                j += 1;
            }
            if j < bytes.len() && bytes[j] == b'(' && &expr[start..i] != "as" {
                return true;
            }
        } else {
            i += 1;
        }
    }
    false
}

fn source_vars(expr: &str) -> Vec<&str> {
    idents(expr)
        .into_iter()
        .filter(|ident| !is_temp_name(ident) && *ident != "as")
        .collect()
}

fn idents(expr: &str) -> Vec<&str> {
    let bytes = expr.as_bytes();
    let mut out = Vec::new();
    let mut i = 0;
    while i < bytes.len() {
        if is_ident_start(bytes[i]) {
            let start = i;
            i += 1;
            while i < bytes.len() && is_ident_continue(bytes[i]) {
                i += 1;
            }
            out.push(&expr[start..i]);
        } else {
            i += 1;
        }
    }
    out
}

fn assigns_to(line: &str, name: &str) -> bool {
    let trimmed = line.trim_start();
    trimmed
        .strip_prefix(name)
        .is_some_and(|rest| rest.trim_start().starts_with('='))
}

fn ident_count(line: &str, name: &str) -> usize {
    let bytes = line.as_bytes();
    let name_bytes = name.as_bytes();
    let mut count = 0;
    let mut i = 0;
    while i + name_bytes.len() <= bytes.len() {
        if &bytes[i..i + name_bytes.len()] == name_bytes
            && (i == 0 || !is_ident_continue(bytes[i - 1]))
            && (i + name_bytes.len() == bytes.len()
                || !is_ident_continue(bytes[i + name_bytes.len()]))
        {
            count += 1;
            i += name_bytes.len();
        } else {
            i += 1;
        }
    }
    count
}

fn is_method_receiver_use(line: &str, name: &str) -> bool {
    let bytes = line.as_bytes();
    let name_bytes = name.as_bytes();
    let mut i = 0;
    while i + name_bytes.len() <= bytes.len() {
        if &bytes[i..i + name_bytes.len()] == name_bytes
            && (i == 0 || !is_ident_continue(bytes[i - 1]))
            && (i + name_bytes.len() == bytes.len()
                || !is_ident_continue(bytes[i + name_bytes.len()]))
        {
            let mut j = i + name_bytes.len();
            while j < bytes.len() && (bytes[j].is_ascii_whitespace() || bytes[j] == b')') {
                j += 1;
            }
            return j < bytes.len() && bytes[j] == b'.';
        }
        i += 1;
    }
    false
}

fn replace_ident_once(line: &str, name: &str, expr: &str) -> String {
    let bytes = line.as_bytes();
    let name_bytes = name.as_bytes();
    let mut i = 0;
    while i + name_bytes.len() <= bytes.len() {
        if &bytes[i..i + name_bytes.len()] == name_bytes
            && (i == 0 || !is_ident_continue(bytes[i - 1]))
            && (i + name_bytes.len() == bytes.len()
                || !is_ident_continue(bytes[i + name_bytes.len()]))
        {
            let mut out = String::new();
            out.push_str(&line[..i]);
            out.push_str(expr);
            out.push_str(&line[i + name_bytes.len()..]);
            return out;
        }
        i += 1;
    }
    line.to_string()
}

fn is_ident_start(b: u8) -> bool {
    b == b'_' || b.is_ascii_alphabetic()
}

fn is_ident_continue(b: u8) -> bool {
    is_ident_start(b) || b.is_ascii_digit()
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn inlines_single_use_scalar_temps() {
        let input = "\
fn main() {
    let mut a: i32 = 0;
    let _v0: i32 = 20;
    a = _v0;
    let _v1: i32 = 5;
    let _v2: i32 = a;
    let _v3: i32 = ((_v2) - (_v1));
    a = _v3;
}
";

        assert_eq!(
            inline_single_use_temps(input),
            "\
fn main() {
    let mut a: i32 = 0;
    a = 20;
    a = ((a) - (5));
}
"
        );
    }

    #[test]
    fn does_not_inline_call_results() {
        let input = "\
fn main() {
    let _v0: i32 = f();
    let _v1: i32 = _v0;
}
";

        assert_eq!(inline_single_use_temps(input), input);
    }

    #[test]
    fn does_not_cross_side_effecting_statement() {
        let input = "\
fn main() {
    let _v0: i32 = a;
    unsafe { printf(_v1); };
    b = _v0;
}
";

        assert_eq!(inline_single_use_temps(input), input);
    }

    #[test]
    fn does_not_inline_volatile_or_pointer_intrinsics() {
        let input = "\
fn main() {
    let _v0: i32 = std::ptr::read_volatile(std::ptr::addr_of!(a));
    b = _v0;
}
";

        assert_eq!(inline_single_use_temps(input), input);
    }

    #[test]
    fn does_not_inline_method_receivers_that_need_type_annotations() {
        let input = "\
fn main() {
    let _v0: i32 = 2147483647;
    let _v1: i32 = 1;
    let _v2 = (_v0).overflowing_add(_v1);
}
";

        assert_eq!(inline_single_use_temps(input), input);
    }

    #[test]
    fn does_not_inline_call_arguments_that_need_type_annotations() {
        let input = "\
fn main() {
    let _v0: i64 = 9223372036854775807;
    let _v1: i32 = unsafe { printf(_v0) };
}
";

        assert_eq!(inline_single_use_temps(input), input);
    }
}
