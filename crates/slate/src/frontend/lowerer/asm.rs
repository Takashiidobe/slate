use super::*;

pub(super) fn collect_assembly_strings(module: &Module, out: &mut Vec<String>) {
    out.extend(module.module_asm.iter().cloned());
    for function in &module.functions {
        if let Some(body) = &function.body {
            walk_region_ops(body, &mut |op| {
                if let Op::Asm(asm) = op {
                    out.push(asm.asm_string.clone());
                }
                true
            });
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
    module: &Module,
    diagnostics: &mut crate::ctx::Diagnostics,
) -> Vec<Item> {
    let mut templates = Vec::new();
    for raw in &module.module_asm {
        match String::from_utf8(decode_cir_string(raw)) {
            Ok(template) if !template.is_empty() => templates.push(template),
            Ok(_) => {}
            Err(_) => diagnostics.error("lower: file-scope assembly template is not valid UTF-8"),
        }
    }
    if templates.is_empty() {
        return Vec::new();
    }
    let Some(triple) = module.triple.as_deref() else {
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
    module: &Module,
    aliases: &BTreeMap<String, String>,
    diagnostics: &mut crate::ctx::Diagnostics,
) -> Vec<Item> {
    if aliases.is_empty() {
        return Vec::new();
    }
    let Some(triple) = module.triple.as_deref() else {
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

pub(super) fn cir_asm_dialect(flavor: clang_ir::enums::AsmFlavor) -> Option<AsmDialect> {
    match flavor {
        clang_ir::enums::AsmFlavor::X86Att => Some(AsmDialect::Att),
        clang_ir::enums::AsmFlavor::X86Intel => Some(AsmDialect::Intel),
    }
}

pub(super) fn normalize_asm_dialect_wrapper(
    template: String,
    dialect: Option<AsmDialect>,
) -> (String, Option<AsmDialect>) {
    if !matches!(dialect, Some(AsmDialect::Att)) {
        return (template, dialect);
    }
    let trimmed = template.trim();
    let Some(body) = trimmed.strip_prefix(".intel_syntax noprefix") else {
        return (template, dialect);
    };
    let Some(body) = body.trim().strip_suffix(".att_syntax prefix") else {
        return (template, dialect);
    };
    (body.trim().to_string(), Some(AsmDialect::Intel))
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
    types: &[Type],
    dialect: Option<AsmDialect>,
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
        if constraint_is_explicit_register(constraint) {
            let kind = parse_reg_constraint(constraint.strip_prefix('=').unwrap_or(constraint))?;
            let AsmReg::Explicit(name) = asm_reg_for_constraint(kind, types.get(slot)?)? else {
                return None;
            };
            if matches!(dialect, Some(AsmDialect::Att)) {
                translated.push('%');
            }
            translated.push_str(&name);
            continue;
        }
        translated.push('{');
        translated.push_str(&rust_slot.to_string());
        if !suppress_modifier
            && (constraint.contains('r')
                || constraint
                    .parse::<usize>()
                    .ok()
                    .and_then(|output| constraints.get(output))
                    .is_some_and(|output| output.contains('r')))
            && let Some(modifier) = rust_asm_register_modifier(types.get(slot)?)
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
        let constraint = constraints[source_slot];
        if constraint_is_explicit_register(constraint) {
            continue;
        }
        translated.push_str("\n/* {");
        translated.push_str(&rust_slot.to_string());
        if (constraint.contains('r')
            || constraint
                .parse::<usize>()
                .ok()
                .and_then(|output| constraints.get(output))
                .is_some_and(|output| output.contains('r')))
            && let Some(modifier) = rust_asm_register_modifier(types.get(source_slot)?)
        {
            translated.push(':');
            translated.push(modifier);
        }
        translated.push_str("} */");
    }
    Some(translated)
}

fn constraint_is_explicit_register(mut constraint: &str) -> bool {
    constraint = constraint.strip_prefix('=').unwrap_or(constraint);
    constraint = constraint.strip_prefix('&').unwrap_or(constraint);
    constraint.starts_with('{') || matches!(constraint, "a" | "b" | "c" | "d" | "S" | "D")
}

pub(super) fn rust_asm_register_modifier(ty: &Type) -> Option<char> {
    match int_bits(&ty.render())? {
        8 => Some('l'),
        16 => Some('x'),
        32 => Some('e'),
        64 => Some('r'),
        _ => None,
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) enum AsmRegConstraint {
    Generic,
    FixedLetter(char),
    ExplicitName(String),
}

pub(super) fn parse_output_reg_constraint(constraint: &str) -> Option<(AsmRegConstraint, bool)> {
    let rest = constraint.strip_prefix('=')?;
    let (early_clobber, rest) = match rest.strip_prefix('&') {
        Some(rest) => (true, rest),
        None => (false, rest),
    };
    let kind = parse_reg_constraint(rest)?;
    Some((kind, early_clobber))
}

pub(super) fn parse_input_reg_constraint(constraint: &str) -> Option<AsmRegConstraint> {
    parse_reg_constraint(constraint)
}

fn parse_reg_constraint(constraint: &str) -> Option<AsmRegConstraint> {
    if let Some(name) = constraint
        .strip_prefix('{')
        .and_then(|rest| rest.strip_suffix('}'))
    {
        return (!name.is_empty()).then(|| AsmRegConstraint::ExplicitName(name.to_string()));
    }
    match constraint {
        "r" | "g" | "imr" => Some(AsmRegConstraint::Generic),
        "a" | "b" | "c" | "d" | "S" | "D" => {
            Some(AsmRegConstraint::FixedLetter(constraint.chars().next()?))
        }
        _ => None,
    }
}

pub(super) fn asm_reg_for_constraint(kind: AsmRegConstraint, ty: &Type) -> Option<AsmReg> {
    let letter = match &kind {
        AsmRegConstraint::Generic => return Some(AsmReg::Class("reg".into())),
        AsmRegConstraint::FixedLetter(letter) => *letter,
        AsmRegConstraint::ExplicitName(name) => match x86_register_letter_from_spelling(name) {
            Some(letter) => letter,
            None => return Some(AsmReg::Explicit(name.clone())),
        },
    };
    x86_fixed_register_name(letter, asm_operand_bits(ty)).map(|name| AsmReg::Explicit(name.into()))
}

pub(super) fn asm_operand_bits(ty: &Type) -> u32 {
    match ty {
        Type::Ptr { .. } | Type::FnPtr { .. } => 64,
        _ => int_bits(&ty.render()).unwrap_or(32),
    }
}

pub(super) fn reg_constraint_letter(kind: &AsmRegConstraint) -> Option<char> {
    match kind {
        AsmRegConstraint::Generic => None,
        AsmRegConstraint::FixedLetter(letter) => Some(*letter),
        AsmRegConstraint::ExplicitName(name) => x86_register_letter_from_spelling(name),
    }
}

pub(super) fn pick_ebx_scratch_letter(used: &BTreeSet<char>) -> Option<char> {
    ['D', 'S', 'a', 'c', 'd']
        .into_iter()
        .find(|letter| !used.contains(letter))
}

fn x86_register_letter_from_spelling(spelling: &str) -> Option<char> {
    Some(match spelling {
        "al" | "ah" | "ax" | "eax" | "rax" => 'a',
        "bl" | "bh" | "bx" | "ebx" | "rbx" => 'b',
        "cl" | "ch" | "cx" | "ecx" | "rcx" => 'c',
        "dl" | "dh" | "dx" | "edx" | "rdx" => 'd',
        "sil" | "si" | "esi" | "rsi" => 'S',
        "dil" | "di" | "edi" | "rdi" => 'D',
        _ => return None,
    })
}

pub(super) fn is_ebx_family_reg(name: &str) -> bool {
    matches!(name, "bl" | "bh" | "bx" | "ebx" | "rbx")
}

pub(super) fn x86_fixed_register_name(letter: char, bits: u32) -> Option<&'static str> {
    Some(match (letter, bits) {
        ('a', 8) => "al",
        ('a', 16) => "ax",
        ('a', 32) => "eax",
        ('a', 64) => "rax",
        ('b', 8) => "bl",
        ('b', 16) => "bx",
        ('b', 32) => "ebx",
        ('b', 64) => "rbx",
        ('c', 8) => "cl",
        ('c', 16) => "cx",
        ('c', 32) => "ecx",
        ('c', 64) => "rcx",
        ('d', 8) => "dl",
        ('d', 16) => "dx",
        ('d', 32) => "edx",
        ('d', 64) => "rdx",
        ('S', 8) => "sil",
        ('S', 16) => "si",
        ('S', 32) => "esi",
        ('S', 64) => "rsi",
        ('D', 8) => "dil",
        ('D', 16) => "di",
        ('D', 32) => "edi",
        ('D', 64) => "rdi",
        _ => return None,
    })
}
