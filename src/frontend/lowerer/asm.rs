use super::*;

#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub(super) enum X86Reg {
    Eax,
    Ebx,
    Ecx,
    Edx,
    Esi,
    Edi,
}

impl X86Reg {
    fn from_spelling(spelling: &str) -> Option<Self> {
        Some(match spelling {
            "al" | "ah" | "ax" | "eax" | "rax" => Self::Eax,
            "bl" | "bh" | "bx" | "ebx" | "rbx" => Self::Ebx,
            "cl" | "ch" | "cx" | "ecx" | "rcx" => Self::Ecx,
            "dl" | "dh" | "dx" | "edx" | "rdx" => Self::Edx,
            "sil" | "si" | "esi" | "rsi" => Self::Esi,
            "dil" | "di" | "edi" | "rdi" => Self::Edi,
            _ => return None,
        })
    }

    pub(super) fn sized_name(self, width: RegWidth) -> &'static str {
        match (self, width) {
            (Self::Eax, RegWidth::Byte) => "al",
            (Self::Eax, RegWidth::Word) => "ax",
            (Self::Eax, RegWidth::Dword) => "eax",
            (Self::Eax, RegWidth::Qword) => "rax",
            (Self::Ebx, RegWidth::Byte) => "bl",
            (Self::Ebx, RegWidth::Word) => "bx",
            (Self::Ebx, RegWidth::Dword) => "ebx",
            (Self::Ebx, RegWidth::Qword) => "rbx",
            (Self::Ecx, RegWidth::Byte) => "cl",
            (Self::Ecx, RegWidth::Word) => "cx",
            (Self::Ecx, RegWidth::Dword) => "ecx",
            (Self::Ecx, RegWidth::Qword) => "rcx",
            (Self::Edx, RegWidth::Byte) => "dl",
            (Self::Edx, RegWidth::Word) => "dx",
            (Self::Edx, RegWidth::Dword) => "edx",
            (Self::Edx, RegWidth::Qword) => "rdx",
            (Self::Esi, RegWidth::Byte) => "sil",
            (Self::Esi, RegWidth::Word) => "si",
            (Self::Esi, RegWidth::Dword) => "esi",
            (Self::Esi, RegWidth::Qword) => "rsi",
            (Self::Edi, RegWidth::Byte) => "dil",
            (Self::Edi, RegWidth::Word) => "di",
            (Self::Edi, RegWidth::Dword) => "edi",
            (Self::Edi, RegWidth::Qword) => "rdi",
        }
    }

    pub(super) fn is_ebx_like(self) -> bool {
        matches!(self, Self::Ebx)
    }

    pub(super) fn pick_ebx_scratch(used: &BTreeSet<X86Reg>) -> Option<X86Reg> {
        [Self::Edi, Self::Esi, Self::Eax, Self::Ecx, Self::Edx]
            .into_iter()
            .find(|reg| !used.contains(reg))
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum Constraint {
    Reg,
    General,
    FixedReg(X86Reg),
    ConstantEligible,
    Address,
    Memory,
    Offsettable,
    NonOffsettable,
    SseReg,
    AvxReg,
    ByteAddressableReg,
    EdxEaxPair,
    Other,
}

fn parse_constraint_atoms(constraint: &str) -> impl Iterator<Item = Constraint> + '_ {
    constraint.chars().map(|ch| match ch {
        'r' => Constraint::Reg,
        'g' => Constraint::General,
        'a' => Constraint::FixedReg(X86Reg::Eax),
        'b' => Constraint::FixedReg(X86Reg::Ebx),
        'c' => Constraint::FixedReg(X86Reg::Ecx),
        'd' => Constraint::FixedReg(X86Reg::Edx),
        'S' => Constraint::FixedReg(X86Reg::Esi),
        'D' => Constraint::FixedReg(X86Reg::Edi),
        'i' | 'n' | 'I' | 'J' | 'K' | 'L' | 'M' | 'N' | 'O' => Constraint::ConstantEligible,
        'p' => Constraint::Address,
        'm' => Constraint::Memory,
        'o' => Constraint::Offsettable,
        'V' => Constraint::NonOffsettable,
        'x' => Constraint::SseReg,
        'y' => Constraint::AvxReg,
        'q' | 'Q' => Constraint::ByteAddressableReg,
        'A' => Constraint::EdxEaxPair,
        _ => Constraint::Other,
    })
}

fn constraint_allows_generic_reg(constraint: &str) -> bool {
    parse_constraint_atoms(constraint)
        .any(|atom| matches!(atom, Constraint::Reg | Constraint::General))
}

fn constraint_wants_register_modifier(constraint: &str, constraints: &[&str]) -> bool {
    constraint_allows_generic_reg(constraint)
        || constraint
            .parse::<usize>()
            .ok()
            .and_then(|output| constraints.get(output))
            .is_some_and(|output| constraint_allows_generic_reg(output))
}

fn constraint_is_explicit_register(mut constraint: &str) -> bool {
    constraint = constraint.strip_prefix('=').unwrap_or(constraint);
    constraint = constraint.strip_prefix('&').unwrap_or(constraint);
    if constraint.starts_with('{') {
        return true;
    }
    matches!(
        parse_constraint_atoms(constraint)
            .collect::<Vec<_>>()
            .as_slice(),
        [Constraint::FixedReg(_)]
    )
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum RegWidth {
    Byte,
    Word,
    Dword,
    Qword,
}

impl RegWidth {
    pub(super) fn from_bits(bits: u32) -> Option<Self> {
        Some(match bits {
            8 => Self::Byte,
            16 => Self::Word,
            32 => Self::Dword,
            64 => Self::Qword,
            _ => return None,
        })
    }

    fn att_size_modifier(self) -> char {
        match self {
            Self::Byte => 'l',
            Self::Word => 'x',
            Self::Dword => 'e',
            Self::Qword => 'r',
        }
    }

    fn zero_extend_from_byte_mnemonic(self) -> Option<&'static str> {
        match self {
            Self::Byte => None,
            Self::Word => Some("movzbw"),
            Self::Dword => Some("movzbl"),
            Self::Qword => Some("movzbq"),
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) enum AsmRegConstraint {
    Generic,
    FixedLetter(X86Reg),
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
    if constraint_allows_generic_reg(constraint) {
        return Some(AsmRegConstraint::Generic);
    }
    match parse_constraint_atoms(constraint)
        .collect::<Vec<_>>()
        .as_slice()
    {
        [Constraint::FixedReg(reg)] => Some(AsmRegConstraint::FixedLetter(*reg)),
        _ => None,
    }
}

pub(super) fn parse_x86_flag_output_constraint(constraint: &str) -> Option<&str> {
    let condition = constraint.strip_prefix("={@cc")?.strip_suffix('}')?;
    (!condition.is_empty() && condition.chars().all(|ch| ch.is_ascii_alphabetic()))
        .then_some(condition)
}

#[derive(Debug, Clone)]
pub(super) enum ResolvedAsmReg {
    Generic,
    Family(X86Reg),
    Literal(String),
}

pub(super) fn asm_reg_for_constraint(kind: AsmRegConstraint) -> ResolvedAsmReg {
    match kind {
        AsmRegConstraint::Generic => ResolvedAsmReg::Generic,
        AsmRegConstraint::FixedLetter(reg) => ResolvedAsmReg::Family(reg),
        AsmRegConstraint::ExplicitName(name) => match X86Reg::from_spelling(&name) {
            Some(reg) => ResolvedAsmReg::Family(reg),
            None => ResolvedAsmReg::Literal(name),
        },
    }
}

pub(super) fn resolved_asm_reg_to_backend(resolved: &ResolvedAsmReg, bits: u32) -> Option<AsmReg> {
    Some(match resolved {
        ResolvedAsmReg::Generic => AsmReg::Class("reg".into()),
        ResolvedAsmReg::Family(reg) => {
            AsmReg::Explicit(reg.sized_name(RegWidth::from_bits(bits)?).into())
        }
        ResolvedAsmReg::Literal(name) => AsmReg::Explicit(name.clone()),
    })
}

pub(super) fn reg_constraint_family(kind: &AsmRegConstraint) -> Option<X86Reg> {
    match kind {
        AsmRegConstraint::Generic => None,
        AsmRegConstraint::FixedLetter(reg) => Some(*reg),
        AsmRegConstraint::ExplicitName(name) => X86Reg::from_spelling(name),
    }
}

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
    let Ok(target_arch) = TargetArch::try_from(triple) else {
        diagnostics.error(format!(
            "lower: unsupported file-scope assembly target `{triple}`"
        ));
        return Vec::new();
    };
    let dialect = target_arch.is_x86().then_some(AsmDialect::Att);
    templates
        .iter()
        .map(|template| Item::Cfg {
            cfg: Cfg::Opt {
                key: "target_arch".into(),
                value: target_arch.rustc_name().into(),
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
    let Ok(target_arch) = TargetArch::try_from(triple) else {
        diagnostics.error(format!("lower: unsupported weak alias target `{triple}`"));
        return Vec::new();
    };
    let dialect = target_arch.is_x86().then_some(AsmDialect::Att);
    aliases
        .iter()
        .map(|(name, target)| Item::Cfg {
            cfg: Cfg::Opt {
                key: "target_arch".into(),
                value: target_arch.rustc_name().into(),
            },
            item: Box::new(Item::Macro {
                name: "core::arch::global_asm".into(),
                args: asm_macro_args(format!(".weak {name}\n.set {name}, {target}"), dialect),
            }),
        })
        .collect()
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(super) enum TargetArch {
    X86,
    X86_64,
    Arm,
    Arm64,
    RiscV32,
    RiscV64,
}

impl TargetArch {
    pub(super) fn rustc_name(self) -> &'static str {
        match self {
            Self::X86 => "x86",
            Self::X86_64 => "x86_64",
            Self::Arm => "arm",
            Self::Arm64 => "aarch64",
            Self::RiscV32 => "riscv32",
            Self::RiscV64 => "riscv64",
        }
    }

    pub(super) fn is_x86(self) -> bool {
        matches!(self, Self::X86 | Self::X86_64)
    }
}

impl TryFrom<&str> for TargetArch {
    type Error = ();

    fn try_from(triple: &str) -> Result<Self, Self::Error> {
        let arch = triple.split('-').next().ok_or(())?;
        match arch {
            "x86_64" | "x86_64h" => Ok(Self::X86_64),
            "i386" | "i486" | "i586" | "i686" => Ok(Self::X86),
            "aarch64" | "aarch64_be" | "arm64" => Ok(Self::Arm64),
            arch if arch.starts_with("arm") || arch.starts_with("thumb") => Ok(Self::Arm),
            "riscv32" | "riscv32gc" | "riscv32imac" => Ok(Self::RiscV32),
            "riscv64" | "riscv64gc" | "riscv64imac" => Ok(Self::RiscV64),
            _ => Err(()),
        }
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

pub(super) fn x86_flag_output_suffix(
    rust_slot: usize,
    condition: &str,
    ty: &Type,
    dialect: Option<AsmDialect>,
) -> Option<String> {
    if !matches!(
        condition,
        "a" | "ae"
            | "b"
            | "be"
            | "c"
            | "e"
            | "g"
            | "ge"
            | "l"
            | "le"
            | "na"
            | "nae"
            | "nb"
            | "nbe"
            | "nc"
            | "ne"
            | "ng"
            | "nge"
            | "nl"
            | "nle"
            | "no"
            | "np"
            | "ns"
            | "nz"
            | "o"
            | "p"
            | "pe"
            | "po"
            | "s"
            | "z"
    ) {
        return None;
    }
    let width = RegWidth::from_bits(asm_operand_bits(ty))?;
    let att_mov = width.zero_extend_from_byte_mnemonic()?;
    let modifier = width.att_size_modifier();
    let set = format!("set{condition} {{{rust_slot}:l}}");
    let extend = if matches!(dialect, Some(AsmDialect::Att)) {
        format!("{att_mov} {{{rust_slot}:l}}, {{{rust_slot}:{modifier}}}")
    } else {
        format!("movzx {{{rust_slot}:{modifier}}}, {{{rust_slot}:l}}")
    };
    Some(format!("{set}\n\t{extend}"))
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

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum TemplateModifier {
    Const,
    Label,
}

impl TemplateModifier {
    fn parse(modifier: &str) -> Option<Self> {
        match modifier {
            "c" => Some(Self::Const),
            "l" => Some(Self::Label),
            _ => None,
        }
    }
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
                modifier.and_then(TemplateModifier::parse).is_some(),
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
            let resolved = asm_reg_for_constraint(kind);
            let bits = asm_operand_bits(types.get(slot)?);
            let AsmReg::Explicit(name) = resolved_asm_reg_to_backend(&resolved, bits)? else {
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
            && constraint_wants_register_modifier(constraint, constraints)
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
        if constraint_wants_register_modifier(constraint, constraints)
            && let Some(modifier) = rust_asm_register_modifier(types.get(source_slot)?)
        {
            translated.push(':');
            translated.push(modifier);
        }
        translated.push_str("} */");
    }
    Some(translated)
}

pub(super) fn rust_asm_register_modifier(ty: &Type) -> Option<char> {
    Some(RegWidth::from_bits(int_bits(&ty.render())?)?.att_size_modifier())
}

pub(super) fn asm_operand_bits(ty: &Type) -> u32 {
    match ty {
        Type::Ptr { .. } | Type::FnPtr { .. } => 64,
        _ => int_bits(&ty.render()).unwrap_or(32),
    }
}
