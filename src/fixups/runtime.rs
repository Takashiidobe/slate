use crate::fixups::support::walk;
use crate::rust_ast::{Expr, Ident, Item, Path, Program};

pub(super) fn ensure_numeric_parse(program: &mut Program) {
    if !uses_numeric_parse(program) || has_runtime_module(program) {
        return;
    }
    let index = program
        .items
        .iter()
        .take_while(|item| matches!(item, Item::CrateAttrs(_)))
        .count();
    program.items.insert(index, Item::Raw(NUMERIC_PARSE.into()));
}

pub(super) fn numeric_parse_path(name: &str) -> Expr {
    Expr::Path(Path::new(
        ["__slate_runtime", name].into_iter().map(Ident::new),
    ))
}

fn uses_numeric_parse(program: &mut Program) -> bool {
    program.items.iter_mut().any(|item| match item {
        Item::Fn(f) => {
            let mut found = false;
            walk::body_exprs_mut_with(&mut f.body, &mut |expr| {
                if is_numeric_parse_path(expr) {
                    found = true;
                }
                true
            });
            found
        }
        _ => false,
    })
}

fn is_numeric_parse_path(expr: &Expr) -> bool {
    let Expr::Path(path) = expr else {
        return false;
    };
    let segments = path.segments.iter().map(Ident::as_str).collect::<Vec<_>>();
    matches!(
        segments.as_slice(),
        [
            "__slate_runtime",
            "parse_i32" | "parse_i64" | "parse_u64" | "parse_f64"
        ]
    )
}

fn has_runtime_module(program: &Program) -> bool {
    program.items.iter().any(|item| match item {
        Item::Raw(raw) => raw.contains("mod __slate_runtime"),
        _ => false,
    })
}

const NUMERIC_PARSE: &str = r#"mod __slate_runtime {
    fn decimal_prefix(s: &str) -> &str {
        let s = s.trim_start();
        let bytes = s.as_bytes();
        let mut end = 0;
        if matches!(bytes.first(), Some(b'+' | b'-')) {
            end = 1;
        }
        let digits_start = end;
        while bytes.get(end).is_some_and(u8::is_ascii_digit) {
            end += 1;
        }
        if end == digits_start { "0" } else { &s[..end] }
    }

    fn unsigned_decimal_prefix(s: &str) -> &str {
        let s = s.trim_start();
        let bytes = s.as_bytes();
        let mut end = 0;
        if matches!(bytes.first(), Some(b'+' | b'-')) {
            end = 1;
        }
        let digits_start = end;
        while bytes.get(end).is_some_and(u8::is_ascii_digit) {
            end += 1;
        }
        if end == digits_start { "0" } else { &s[..end] }
    }

    fn float_prefix(s: &str) -> &str {
        let s = s.trim_start();
        let bytes = s.as_bytes();
        let mut end = 0;
        if matches!(bytes.first(), Some(b'+' | b'-')) {
            end = 1;
        }
        let mut digits = 0;
        while bytes.get(end).is_some_and(u8::is_ascii_digit) {
            end += 1;
            digits += 1;
        }
        if bytes.get(end) == Some(&b'.') {
            end += 1;
            while bytes.get(end).is_some_and(u8::is_ascii_digit) {
                end += 1;
                digits += 1;
            }
        }
        if digits == 0 {
            return "0";
        }
        if matches!(bytes.get(end), Some(b'e' | b'E')) {
            let exp = end;
            end += 1;
            if matches!(bytes.get(end), Some(b'+' | b'-')) {
                end += 1;
            }
            let exp_digits = end;
            while bytes.get(end).is_some_and(u8::is_ascii_digit) {
                end += 1;
            }
            if end == exp_digits {
                end = exp;
            }
        }
        &s[..end]
    }

    pub(crate) fn parse_i32(s: &str) -> i32 {
        decimal_prefix(s).parse::<i32>().unwrap_or(0)
    }

    pub(crate) fn parse_i64(s: &str) -> i64 {
        let prefix = decimal_prefix(s);
        match prefix.parse::<i64>() {
            Ok(value) => value,
            Err(_) if prefix.starts_with('-') => i64::MIN,
            Err(_) => i64::MAX,
        }
    }

    pub(crate) fn parse_u64(s: &str) -> u64 {
        let prefix = unsigned_decimal_prefix(s);
        let (negative, digits) = match prefix.strip_prefix('-') {
            Some(rest) => (true, rest),
            None => (false, prefix.strip_prefix('+').unwrap_or(prefix)),
        };
        match digits.parse::<u64>() {
            Ok(value) if negative => value.wrapping_neg(),
            Ok(value) => value,
            Err(_) => u64::MAX,
        }
    }

    pub(crate) fn parse_f64(s: &str) -> f64 {
        float_prefix(s).parse::<f64>().unwrap_or(0.0)
    }
}"#;
