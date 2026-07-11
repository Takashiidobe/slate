//! Rust cleanup passes that run after faithful CIR lowering.

use crate::rust_ast::{Item, Program};

pub fn apply(program: Program) -> Program {
    Program {
        items: program
            .items
            .into_iter()
            .map(|item| match item {
                Item::Raw(text) => {
                    let text = eliminate_param_spills(&text);
                    let text = inline_single_use_temps(&text);
                    Item::Raw(collapse_retval(&text))
                }
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

/// Folds the CIR parameter-spill prologue into direct parameter bindings.
///
/// CIR spills every C parameter into its own stack slot (`let mut a = 0; a = arg0;`)
/// because C parameters are mutable lvalues. When a spill slot is only ever seeded
/// from its parameter, the parameter can bind under the slot name directly.
fn eliminate_param_spills(text: &str) -> String {
    let trailing_newline = text.ends_with('\n');
    let mut lines: Vec<String> = text.lines().map(str::to_string).collect();

    let Some(sig_index) = lines.iter().position(|line| is_fn_signature(line)) else {
        return text.to_string();
    };
    let Some((open, close)) = paren_span(&lines[sig_index]) else {
        return text.to_string();
    };
    let params = split_params(&lines[sig_index][open + 1..close]);
    let param_names: Vec<&str> = params
        .iter()
        .filter_map(|p| parse_param(p).map(|(n, _)| n))
        .collect();

    let mut renames: Vec<(String, String)> = Vec::new();
    let mut removed: Vec<usize> = Vec::new();
    let mut claimed_locals: Vec<String> = Vec::new();

    for param in &params {
        let Some((param_name, param_ty)) = parse_param(param) else {
            continue;
        };
        let Some(fold) = spill_fold(&lines, sig_index, param_name, param_ty, &param_names) else {
            continue;
        };
        if claimed_locals.iter().any(|l| l == fold.local) {
            continue;
        }
        claimed_locals.push(fold.local.to_string());
        renames.push((
            param.to_string(),
            format!("mut {}: {}", fold.local, param_ty),
        ));
        removed.push(fold.decl_index);
        removed.push(fold.store_index);
    }

    if renames.is_empty() {
        return text.to_string();
    }

    let mut new_params: Vec<String> = params.iter().map(|p| p.to_string()).collect();
    for (old, new) in &renames {
        if let Some(slot) = new_params.iter_mut().find(|p| p.as_str() == old) {
            *slot = new.clone();
        }
    }
    let sig = &lines[sig_index];
    lines[sig_index] = format!(
        "{}{}{}",
        &sig[..=open],
        new_params.join(", "),
        &sig[close..]
    );

    removed.sort_unstable();
    for index in removed.into_iter().rev() {
        lines.remove(index);
    }

    let mut out = lines.join("\n");
    if trailing_newline {
        out.push('\n');
    }
    out
}

/// Collapses the CIR return-slot boilerplate into a direct `return`.
///
/// C returns lower through a dedicated slot: `let mut __retval = 0; ...
/// __retval = expr; return __retval;`. When the slot is assigned exactly once,
/// immediately before the return, that store can become `return expr;` and the
/// slot declaration drops out. `main` returns via `process::exit`, not `return
/// __retval`, so it is naturally excluded.
fn collapse_retval(text: &str) -> String {
    let trailing_newline = text.ends_with('\n');
    let mut lines: Vec<String> = text.lines().map(str::to_string).collect();

    let Some(ret_index) = lines.iter().position(|l| parse_return_ident(l).is_some()) else {
        return text.to_string();
    };
    let name = parse_return_ident(&lines[ret_index]).unwrap();
    if ret_index == 0 {
        return text.to_string();
    }

    let store_index = ret_index - 1;
    let Some((lhs, expr)) = parse_assign(&lines[store_index]) else {
        return text.to_string();
    };
    if lhs != name {
        return text.to_string();
    }

    // decl + single store + return: exactly three mentions means the slot is
    // never read or reassigned elsewhere, so folding is safe.
    let mentions: usize = lines.iter().map(|l| ident_count(l, name)).sum();
    if mentions != 3 {
        return text.to_string();
    }

    let Some(decl_index) = lines
        .iter()
        .position(|l| parse_local_decl(l).is_some_and(|(n, _)| n == name))
    else {
        return text.to_string();
    };

    let indent = leading_ws(&lines[ret_index]);
    lines[ret_index] = format!("{indent}return {expr};");
    lines.remove(store_index);
    lines.remove(decl_index);

    let mut out = lines.join("\n");
    if trailing_newline {
        out.push('\n');
    }
    out
}

fn parse_return_ident(line: &str) -> Option<&str> {
    let inner = line
        .trim()
        .strip_prefix("return ")?
        .strip_suffix(';')?
        .trim();
    is_ident(inner).then_some(inner)
}

fn parse_assign(line: &str) -> Option<(&str, &str)> {
    let stmt = line.trim().strip_suffix(';')?;
    let (lhs, rhs) = stmt.split_once('=')?;
    let lhs = lhs.trim();
    let rhs = rhs.trim();
    if is_ident(lhs) && !rhs.starts_with('=') {
        Some((lhs, rhs))
    } else {
        None
    }
}

fn leading_ws(line: &str) -> &str {
    &line[..line.len() - line.trim_start().len()]
}

struct SpillFold<'a> {
    local: &'a str,
    decl_index: usize,
    store_index: usize,
}

fn spill_fold<'a>(
    lines: &'a [String],
    sig_index: usize,
    param: &str,
    param_ty: &str,
    param_names: &[&str],
) -> Option<SpillFold<'a>> {
    let body_uses: usize = lines[sig_index + 1..]
        .iter()
        .map(|line| ident_count(line, param))
        .sum();
    if body_uses != 1 {
        return None;
    }

    let store_index = lines
        .iter()
        .enumerate()
        .skip(sig_index + 1)
        .find(|(_, line)| parse_spill_store(line).is_some_and(|(_, rhs)| rhs == param))?
        .0;
    let (local, _) = parse_spill_store(&lines[store_index])?;

    if param_names.contains(&local) {
        return None;
    }

    let decl_index = lines
        .iter()
        .enumerate()
        .take(store_index)
        .skip(sig_index + 1)
        .find(|(_, line)| {
            parse_local_decl(line).is_some_and(|(n, ty)| n == local && ty == param_ty)
        })?
        .0;

    // binding the parameter directly must not change what an earlier read of the slot saw.
    if lines[decl_index + 1..store_index]
        .iter()
        .any(|line| ident_count(line, local) > 0)
    {
        return None;
    }

    Some(SpillFold {
        local,
        decl_index,
        store_index,
    })
}

fn is_fn_signature(line: &str) -> bool {
    let mut rest = line.trim_start();
    for prefix in ["pub ", "unsafe extern \"C\" "] {
        rest = rest.strip_prefix(prefix).unwrap_or(rest);
    }
    rest.starts_with("fn ") && rest.contains('(') && rest.trim_end().ends_with('{')
}

fn paren_span(line: &str) -> Option<(usize, usize)> {
    let open = line.find('(')?;
    let bytes = line.as_bytes();
    let mut depth = 0i32;
    for i in open..bytes.len() {
        match bytes[i] {
            b'(' => depth += 1,
            b')' => {
                depth -= 1;
                if depth == 0 {
                    return Some((open, i));
                }
            }
            _ => {}
        }
    }
    None
}

fn split_params(params: &str) -> Vec<&str> {
    let bytes = params.as_bytes();
    let mut depth = 0i32;
    let mut start = 0usize;
    let mut out = Vec::new();
    for (i, &b) in bytes.iter().enumerate() {
        match b {
            b'(' | b'[' | b'<' => depth += 1,
            b')' | b']' | b'>' => depth -= 1,
            b',' if depth == 0 => {
                let part = params[start..i].trim();
                if !part.is_empty() {
                    out.push(part);
                }
                start = i + 1;
            }
            _ => {}
        }
    }
    let last = params[start..].trim();
    if !last.is_empty() {
        out.push(last);
    }
    out
}

fn parse_param(param: &str) -> Option<(&str, &str)> {
    if param.starts_with("mut ") {
        return None;
    }
    let (name, ty) = param.split_once(':')?;
    Some((name.trim(), ty.trim()))
}

fn parse_spill_store(line: &str) -> Option<(&str, &str)> {
    let trimmed = line.trim();
    let stmt = trimmed.strip_suffix(';')?;
    let (lhs, rhs) = stmt.split_once('=')?;
    let lhs = lhs.trim();
    let rhs = rhs.trim();
    if is_ident(lhs) && is_ident(rhs) {
        Some((lhs, rhs))
    } else {
        None
    }
}

fn parse_local_decl(line: &str) -> Option<(&str, &str)> {
    let rest = line.trim_start().strip_prefix("let mut ")?;
    let (name, rest) = rest.split_once(':')?;
    let (ty, _) = rest.split_once('=')?;
    Some((name.trim(), ty.trim()))
}

fn is_ident(s: &str) -> bool {
    let bytes = s.as_bytes();
    !bytes.is_empty() && is_ident_start(bytes[0]) && bytes.iter().all(|&b| is_ident_continue(b))
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
    fn collapses_retval_store_into_return() {
        let input = "\
fn add(mut a: i32, mut b: i32) -> i32 {
    let mut __retval: i32 = 0;
    let mut c: i32 = 0;
    c = ((a) + (b));
    __retval = c;
    return __retval;
}
";

        assert_eq!(
            collapse_retval(input),
            "\
fn add(mut a: i32, mut b: i32) -> i32 {
    let mut c: i32 = 0;
    c = ((a) + (b));
    return c;
}
"
        );
    }

    #[test]
    fn does_not_collapse_when_retval_read_elsewhere() {
        let input = "\
fn f() -> i32 {
    let mut __retval: i32 = 0;
    __retval = 1;
    let mut x: i32 = __retval;
    __retval = x;
    return __retval;
}
";

        assert_eq!(collapse_retval(input), input);
    }

    #[test]
    fn does_not_collapse_when_store_is_not_immediately_before_return() {
        let input = "\
fn f() -> i32 {
    let mut __retval: i32 = 0;
    __retval = 1;
    let mut x: i32 = 2;
    return __retval;
}
";

        assert_eq!(collapse_retval(input), input);
    }

    #[test]
    fn leaves_process_exit_main_untouched() {
        let input = "\
fn main() {
    let mut __retval: i32 = 0;
    __retval = 0;
    let _v0: i32 = __retval;
    std::process::exit(_v0 as i32);
}
";

        assert_eq!(collapse_retval(input), input);
    }

    #[test]
    fn folds_parameter_spills_into_direct_bindings() {
        let input = "\
fn add(arg0: i32, arg1: i32) -> i32 {
    let mut a: i32 = 0;
    let mut b: i32 = 0;
    let mut __retval: i32 = 0;
    let mut c: i32 = 0;
    a = arg0;
    b = arg1;
    c = ((a) + (b));
    __retval = c;
    return __retval;
}
";

        assert_eq!(
            eliminate_param_spills(input),
            "\
fn add(mut a: i32, mut b: i32) -> i32 {
    let mut __retval: i32 = 0;
    let mut c: i32 = 0;
    c = ((a) + (b));
    __retval = c;
    return __retval;
}
"
        );
    }

    #[test]
    fn folds_pointer_and_float_parameter_spills() {
        let input = "\
fn scale(arg0: *mut i32, arg1: f32) -> f32 {
    let mut slot: *mut i32 = std::ptr::null_mut();
    let mut factor: f32 = 0.0;
    slot = arg0;
    factor = arg1;
    return factor;
}
";

        assert_eq!(
            eliminate_param_spills(input),
            "\
fn scale(mut slot: *mut i32, mut factor: f32) -> f32 {
    return factor;
}
"
        );
    }

    #[test]
    fn does_not_fold_when_parameter_is_read_again() {
        let input = "\
fn f(arg0: i32) -> i32 {
    let mut a: i32 = 0;
    a = arg0;
    return arg0;
}
";

        assert_eq!(eliminate_param_spills(input), input);
    }

    #[test]
    fn does_not_fold_when_slot_read_before_spill() {
        let input = "\
fn f(arg0: i32) -> i32 {
    let mut a: i32 = 0;
    let mut b: i32 = a;
    a = arg0;
    return b;
}
";

        assert_eq!(eliminate_param_spills(input), input);
    }

    #[test]
    fn does_not_fold_when_slot_type_differs_from_parameter() {
        let input = "\
fn f(arg0: i32) -> i64 {
    let mut a: i64 = 0;
    a = arg0;
    return a;
}
";

        assert_eq!(eliminate_param_spills(input), input);
    }

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
