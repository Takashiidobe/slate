use super::*;

pub(super) fn collect_assembly_strings<'a>(op: &'a Op, out: &mut Vec<&'a str>) {
    if op.kind() == CirOpKind::Asm
        && let Some(template) = attr_str(op, "asm_string")
    {
        out.push(template);
    }
    if let Some(Attr::Array(values)) = op.attrs.get("cir.module_asm") {
        out.extend(values.iter().filter_map(Attr::as_str));
    }
    for region in &op.regions {
        for block in &region.blocks {
            for child in &block.ops {
                collect_assembly_strings(child, out);
            }
        }
    }
}

pub(super) fn assembly_mentions_symbol(assembly: &str, symbol: &str) -> bool {
    assembly.match_indices(symbol).any(|(start, _)| {
        let before = assembly[..start].chars().next_back();
        let after = assembly[start + symbol.len()..].chars().next();
        !before.is_some_and(is_asm_symbol_char) && !after.is_some_and(is_asm_symbol_char)
    })
}

pub(super) fn is_asm_symbol_char(ch: char) -> bool {
    ch.is_ascii_alphanumeric() || matches!(ch, '_' | '.' | '$')
}

pub(super) fn lower_module_asm(
    module_op: &Op,
    diagnostics: &mut crate::ctx::Diagnostics,
) -> Vec<Item> {
    let Some(Attr::Array(values)) = module_op.attrs.get("cir.module_asm") else {
        return Vec::new();
    };
    let mut templates = Vec::new();
    for raw in values.iter().filter_map(Attr::as_str) {
        match String::from_utf8(decode_cir_string(raw)) {
            Ok(template) if !template.is_empty() => templates.push(template),
            Ok(_) => {}
            Err(_) => diagnostics.error("lower: file-scope assembly template is not valid UTF-8"),
        }
    }
    if templates.is_empty() {
        return Vec::new();
    }
    let Some(triple) = attr_str(module_op, "cir.triple") else {
        diagnostics.error("lower: file-scope assembly has no CIR target triple");
        return Vec::new();
    };
    let Some(target_arch) = rust_target_arch(triple) else {
        diagnostics.error(format!(
            "lower: unsupported file-scope assembly target `{triple}`"
        ));
        return Vec::new();
    };
    let dialect = matches!(target_arch, "x86" | "x86_64").then_some(AsmDialect::Att);
    templates
        .iter()
        .map(|template| Item::Cfg {
            cfg: Cfg::Opt {
                key: "target_arch".into(),
                value: target_arch.into(),
            },
            item: Box::new(Item::Macro {
                name: "core::arch::global_asm".into(),
                args: asm_macro_args(template.clone(), dialect),
            }),
        })
        .collect()
}

pub(super) fn lower_weak_alias_asm(
    module_op: &Op,
    aliases: &BTreeMap<String, String>,
    diagnostics: &mut crate::ctx::Diagnostics,
) -> Vec<Item> {
    if aliases.is_empty() {
        return Vec::new();
    }
    let Some(triple) = attr_str(module_op, "cir.triple") else {
        diagnostics.error("lower: weak aliases require a CIR target triple");
        return Vec::new();
    };
    let Some(target_arch) = rust_target_arch(triple) else {
        diagnostics.error(format!("lower: unsupported weak alias target `{triple}`"));
        return Vec::new();
    };
    let dialect = matches!(target_arch, "x86" | "x86_64").then_some(AsmDialect::Att);
    aliases
        .iter()
        .map(|(name, target)| Item::Cfg {
            cfg: Cfg::Opt {
                key: "target_arch".into(),
                value: target_arch.into(),
            },
            item: Box::new(Item::Macro {
                name: "core::arch::global_asm".into(),
                args: asm_macro_args(format!(".weak {name}\n.set {name}, {target}"), dialect),
            }),
        })
        .collect()
}

pub(super) fn rust_target_arch(triple: &str) -> Option<&'static str> {
    let arch = triple.split('-').next()?;
    match arch {
        "x86_64" | "x86_64h" => Some("x86_64"),
        "i386" | "i486" | "i586" | "i686" => Some("x86"),
        "aarch64" | "aarch64_be" | "arm64" => Some("aarch64"),
        arch if arch.starts_with("arm") || arch.starts_with("thumb") => Some("arm"),
        "powerpc" => Some("powerpc"),
        "powerpc64" | "powerpc64le" => Some("powerpc64"),
        "riscv32" | "riscv32gc" | "riscv32imac" => Some("riscv32"),
        "riscv64" | "riscv64gc" | "riscv64imac" => Some("riscv64"),
        "s390x" => Some("s390x"),
        "wasm32" => Some("wasm32"),
        "wasm64" => Some("wasm64"),
        "mips" | "mipsel" => Some("mips"),
        "mips64" | "mips64el" => Some("mips64"),
        "sparc" => Some("sparc"),
        "sparc64" => Some("sparc64"),
        "loongarch64" => Some("loongarch64"),
        "hexagon" => Some("hexagon"),
        "bpfeb" | "bpfel" => Some("bpf"),
        "nvptx64" => Some("nvptx64"),
        "xtensa" => Some("xtensa"),
        _ => None,
    }
}

pub(super) fn cir_asm_dialect(op: &Op) -> Option<AsmDialect> {
    match attr_int(op, "asm_flavor") {
        Some(0) => Some(AsmDialect::Att),
        Some(1) => Some(AsmDialect::Intel),
        _ => None,
    }
}

pub(super) fn asm_macro_args(template: String, dialect: Option<AsmDialect>) -> Vec<Expr> {
    let mut options = Vec::new();
    if matches!(dialect, Some(AsmDialect::Att)) {
        options.push(Expr::Var("att_syntax".into()));
    }
    options.push(Expr::Var("raw".into()));
    vec![
        Expr::Str(template),
        Expr::Call {
            binding: CallBinding::Generated,
            func: Box::new(Expr::Var("options".into())),
            args: options,
        },
    ]
}

pub(super) fn asm_macro_expr(template: String, dialect: Option<AsmDialect>) -> Expr {
    Expr::Macro {
        name: "core::arch::asm".into(),
        args: asm_macro_args(template, dialect),
    }
}

pub(super) fn asm_template_has_placeholders(template: &str) -> bool {
    let mut chars = template.chars().peekable();
    while let Some(ch) = chars.next() {
        if ch != '$' {
            continue;
        }
        if chars.peek() == Some(&'$') {
            chars.next();
        } else {
            return true;
        }
    }
    false
}

pub(super) fn asm_template_has_labels(template: &str) -> bool {
    asm_template_label_count(template) != 0
}

pub(super) fn asm_template_label_count(template: &str) -> usize {
    let mut slots = BTreeSet::new();
    let mut chars = template.chars().peekable();
    while let Some(ch) = chars.next() {
        if ch != '$' || chars.peek() != Some(&'{') {
            continue;
        }
        chars.next();
        let body = chars
            .by_ref()
            .take_while(|next| *next != '}')
            .collect::<String>();
        if let Some((slot, "l")) = body.split_once(':')
            && let Ok(slot) = slot.parse::<usize>()
        {
            slots.insert(slot);
        }
    }
    slots.len()
}

pub(super) fn translate_asm_template(
    template: &str,
    slot_to_rust: &[usize],
    constraints: &[&str],
    types: &[&str],
) -> Option<String> {
    let mut translated = String::new();
    let mut referenced_operands = BTreeSet::new();
    let mut chars = template.char_indices().peekable();
    while let Some((_, ch)) = chars.next() {
        if ch != '$' {
            translated.push(ch);
            continue;
        }
        if chars.peek().is_some_and(|(_, next)| *next == '$') {
            chars.next();
            translated.push('$');
            continue;
        }
        let (slot, suppress_modifier) = if chars.peek().is_some_and(|(_, next)| *next == '{') {
            chars.next();
            let mut body = String::new();
            let mut closed = false;
            for (_, next) in chars.by_ref() {
                if next == '}' {
                    closed = true;
                    break;
                }
                body.push(next);
            }
            if !closed {
                return None;
            }
            let (slot, modifier) = body
                .split_once(':')
                .map(|(slot, modifier)| (slot, Some(modifier)))
                .unwrap_or((body.as_str(), None));
            (
                slot.parse::<usize>().ok()?,
                matches!(modifier, Some("c" | "l")),
            )
        } else {
            let mut digits = String::new();
            while let Some((_, next)) = chars.peek() {
                if !next.is_ascii_digit() {
                    break;
                }
                digits.push(*next);
                chars.next();
            }
            if digits.is_empty() {
                return None;
            }
            (digits.parse::<usize>().ok()?, false)
        };
        let rust_slot = *slot_to_rust.get(slot)?;
        let constraint = *constraints.get(slot)?;
        referenced_operands.insert(rust_slot);
        translated.push('{');
        translated.push_str(&rust_slot.to_string());
        if !suppress_modifier
            && (constraint.contains('r')
                || constraint
                    .parse::<usize>()
                    .ok()
                    .and_then(|output| constraints.get(output))
                    .is_some_and(|output| output.contains('r')))
            && let Some(modifier) = rust_asm_register_modifier(types.get(slot).copied()?)
        {
            translated.push(':');
            translated.push(modifier);
        }
        translated.push('}');
    }
    for rust_slot in 0..slot_to_rust.iter().copied().max()?.saturating_add(1) {
        if referenced_operands.contains(&rust_slot) {
            continue;
        }
        let source_slot = slot_to_rust
            .iter()
            .position(|mapped| *mapped == rust_slot)?;
        translated.push_str("\n/* {");
        translated.push_str(&rust_slot.to_string());
        let constraint = constraints[source_slot];
        if (constraint.contains('r')
            || constraint
                .parse::<usize>()
                .ok()
                .and_then(|output| constraints.get(output))
                .is_some_and(|output| output.contains('r')))
            && let Some(modifier) = rust_asm_register_modifier(types.get(source_slot).copied()?)
        {
            translated.push(':');
            translated.push(modifier);
        }
        translated.push_str("} */");
    }
    Some(translated)
}

pub(super) fn rust_asm_register_modifier(cir_ty: &str) -> Option<char> {
    let bits = cir_ty
        .trim()
        .strip_prefix("!s")
        .or_else(|| cir_ty.trim().strip_prefix("!u"))?
        .strip_suffix('i')?
        .parse::<u16>()
        .ok()?;
    match bits {
        8 => Some('l'),
        16 => Some('x'),
        32 => Some('e'),
        64 => Some('r'),
        _ => None,
    }
}
