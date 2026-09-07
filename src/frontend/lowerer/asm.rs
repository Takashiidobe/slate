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
enum ConstraintAtom {
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

fn parse_constraint_atoms(constraint: &str) -> impl Iterator<Item = ConstraintAtom> + '_ {
    constraint.chars().map(|ch| match ch {
        'r' => ConstraintAtom::Reg,
        'g' => ConstraintAtom::General,
        'a' => ConstraintAtom::FixedReg(X86Reg::Eax),
        'b' => ConstraintAtom::FixedReg(X86Reg::Ebx),
        'c' => ConstraintAtom::FixedReg(X86Reg::Ecx),
        'd' => ConstraintAtom::FixedReg(X86Reg::Edx),
        'S' => ConstraintAtom::FixedReg(X86Reg::Esi),
        'D' => ConstraintAtom::FixedReg(X86Reg::Edi),
        'i' | 'n' | 'I' | 'J' | 'K' | 'L' | 'M' | 'N' | 'O' => ConstraintAtom::ConstantEligible,
        'p' => ConstraintAtom::Address,
        'm' => ConstraintAtom::Memory,
        'o' => ConstraintAtom::Offsettable,
        'V' => ConstraintAtom::NonOffsettable,
        'x' => ConstraintAtom::SseReg,
        'y' => ConstraintAtom::AvxReg,
        'q' | 'Q' => ConstraintAtom::ByteAddressableReg,
        'A' => ConstraintAtom::EdxEaxPair,
        _ => ConstraintAtom::Other,
    })
}

fn constraint_allows_generic_reg(constraint: &str) -> bool {
    parse_constraint_atoms(constraint).any(|atom| {
        matches!(
            atom,
            ConstraintAtom::Reg | ConstraintAtom::General | ConstraintAtom::Address
        )
    })
}

fn constraint_is_constant_only(constraint: &str) -> bool {
    !constraint.is_empty()
        && parse_constraint_atoms(constraint).all(|atom| atom == ConstraintAtom::ConstantEligible)
}

fn strip_memory_marker(constraint: &str) -> Option<&str> {
    let rest = constraint.strip_prefix('=').unwrap_or(constraint);
    let rest = rest.strip_prefix('&').unwrap_or(rest);
    let rest = rest.strip_prefix('*').unwrap_or(rest);
    (!rest.is_empty() && parse_constraint_atoms(rest).all(|atom| atom == ConstraintAtom::Memory))
        .then_some(rest)
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
        [ConstraintAtom::FixedReg(reg)] => Some(AsmRegConstraint::FixedLetter(*reg)),
        _ => None,
    }
}

fn parse_x86_flag_output_constraint(constraint: &str) -> Option<&str> {
    let condition = constraint.strip_prefix("={@cc")?.strip_suffix('}')?;
    (!condition.is_empty() && condition.chars().all(|ch| ch.is_ascii_alphabetic()))
        .then_some(condition)
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(super) enum Constraint {
    Tied(usize),
    FlagOutput(String),
    Constant,
    Reg {
        kind: AsmRegConstraint,
        early_clobber: bool,
        indirect: bool,
    },
    Memory,
    Unsupported,
}

impl Constraint {
    pub(super) fn parse(raw: &str, is_output: bool) -> Self {
        if !is_output && let Ok(index) = raw.parse::<usize>() {
            return Self::Tied(index);
        }
        if is_output {
            if let Some(condition) = parse_x86_flag_output_constraint(raw) {
                return Self::FlagOutput(condition.to_string());
            }
            let Some(rest) = raw.strip_prefix('=') else {
                return Self::Unsupported;
            };
            let (early_clobber, rest) = match rest.strip_prefix('&') {
                Some(rest) => (true, rest),
                None => (false, rest),
            };
            if strip_memory_marker(rest).is_some() {
                return Self::Memory;
            }
            let (indirect, rest) = match rest.strip_prefix('*') {
                Some(rest) => (true, rest),
                None => (false, rest),
            };
            return match parse_reg_constraint(rest) {
                Some(kind) => Self::Reg {
                    kind,
                    early_clobber,
                    indirect,
                },
                None => Self::Unsupported,
            };
        }
        if strip_memory_marker(raw).is_some() {
            return Self::Memory;
        }
        if constraint_is_constant_only(raw) {
            return Self::Constant;
        }
        match parse_reg_constraint(raw) {
            Some(kind) => Self::Reg {
                kind,
                early_clobber: false,
                indirect: false,
            },
            None => Self::Unsupported,
        }
    }

    pub(super) fn constant_only(&self) -> bool {
        matches!(self, Self::Constant)
    }

    pub(super) fn flag_condition(&self) -> Option<&str> {
        match self {
            Self::FlagOutput(condition) => Some(condition),
            _ => None,
        }
    }

    pub(super) fn reg_kind(&self) -> Option<&AsmRegConstraint> {
        match self {
            Self::Reg { kind, .. } => Some(kind),
            _ => None,
        }
    }

    pub(super) fn is_explicit_register(&self) -> bool {
        matches!(
            self.reg_kind(),
            Some(AsmRegConstraint::FixedLetter(_) | AsmRegConstraint::ExplicitName(_))
        )
    }

    pub(super) fn has_out_of_band_placement(&self) -> bool {
        self.is_explicit_register() || matches!(self, Self::FlagOutput(_))
    }

    pub(super) fn wants_register_modifier(&self, constraints: &[Constraint]) -> bool {
        match self {
            Self::Reg {
                kind: AsmRegConstraint::Generic,
                ..
            } => true,
            Self::Tied(output_index) => constraints
                .get(*output_index)
                .is_some_and(|target| target.wants_register_modifier(constraints)),
            _ => false,
        }
    }
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
    Address,
}

impl TemplateModifier {
    fn parse(modifier: &str) -> Option<Self> {
        match modifier {
            "c" => Some(Self::Const),
            "l" => Some(Self::Label),
            "a" => Some(Self::Address),
            _ => None,
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
enum TemplatePiece {
    Literal(String),
    Operand {
        slot: usize,
        modifier: Option<TemplateModifier>,
    },
}

fn parse_asm_template(template: &str) -> Option<Vec<TemplatePiece>> {
    let mut pieces = Vec::new();
    let mut literal = String::new();
    let mut chars = template.chars().peekable();
    while let Some(ch) = chars.next() {
        if ch != '$' {
            literal.push(ch);
            continue;
        }
        if chars.peek() == Some(&'$') {
            chars.next();
            literal.push('$');
            continue;
        }
        let (slot, modifier) = if chars.peek() == Some(&'{') {
            chars.next();
            let mut body = String::new();
            let mut closed = false;
            for next in chars.by_ref() {
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
                modifier.and_then(TemplateModifier::parse),
            )
        } else {
            let mut digits = String::new();
            while let Some(next) = chars.peek() {
                if !next.is_ascii_digit() {
                    break;
                }
                digits.push(*next);
                chars.next();
            }
            if digits.is_empty() {
                return None;
            }
            (digits.parse::<usize>().ok()?, None)
        };
        if !literal.is_empty() {
            pieces.push(TemplatePiece::Literal(std::mem::take(&mut literal)));
        }
        pieces.push(TemplatePiece::Operand { slot, modifier });
    }
    if !literal.is_empty() {
        pieces.push(TemplatePiece::Literal(literal));
    }
    Some(pieces)
}

fn dialect_address_brackets(dialect: Option<AsmDialect>) -> (char, char) {
    if matches!(dialect, Some(AsmDialect::Intel)) {
        ('[', ']')
    } else {
        ('(', ')')
    }
}

fn intel_ptr_size_keyword(bits: u32) -> &'static str {
    match RegWidth::from_bits(bits) {
        Some(RegWidth::Byte) => "byte ptr",
        Some(RegWidth::Word) => "word ptr",
        Some(RegWidth::Qword) => "qword ptr",
        Some(RegWidth::Dword) | None => "dword ptr",
    }
}

fn memory_operand_pointee_bits(ty: &Type) -> u32 {
    match ty {
        Type::Ptr { inner, .. } => int_bits(&inner.render()).unwrap_or(32),
        _ => asm_operand_bits(ty),
    }
}

fn register_modifier_suffix(
    constraint: &Constraint,
    constraints: &[Constraint],
    ty: &Type,
) -> Option<char> {
    constraint
        .wants_register_modifier(constraints)
        .then(|| rust_asm_register_modifier(ty))
        .flatten()
}

fn render_operand_reference(
    out: &mut String,
    rust_slot: usize,
    modifier: Option<TemplateModifier>,
    constraint: &Constraint,
    constraints: &[Constraint],
    ty: &Type,
    dialect: Option<AsmDialect>,
) -> Option<()> {
    if constraint.is_explicit_register() {
        let resolved = asm_reg_for_constraint(constraint.reg_kind()?.clone());
        let bits = asm_operand_bits(ty);
        let AsmReg::Explicit(name) = resolved_asm_reg_to_backend(&resolved, bits)? else {
            return None;
        };
        if matches!(dialect, Some(AsmDialect::Att)) {
            out.push('%');
        }
        out.push_str(&name);
        return Some(());
    }
    if matches!(modifier, Some(TemplateModifier::Address)) {
        let (open, close) = dialect_address_brackets(dialect);
        out.push(open);
        out.push('{');
        out.push_str(&rust_slot.to_string());
        out.push('}');
        out.push(close);
        return Some(());
    }
    if matches!(constraint, Constraint::Memory) {
        if modifier.is_some() {
            return None;
        }
        let (open, close) = dialect_address_brackets(dialect);
        if matches!(dialect, Some(AsmDialect::Intel)) {
            out.push_str(intel_ptr_size_keyword(memory_operand_pointee_bits(ty)));
            out.push(' ');
        }
        out.push(open);
        out.push('{');
        out.push_str(&rust_slot.to_string());
        out.push('}');
        out.push(close);
        return Some(());
    }
    out.push('{');
    out.push_str(&rust_slot.to_string());
    if modifier.is_none()
        && let Some(suffix) = register_modifier_suffix(constraint, constraints, ty)
    {
        out.push(':');
        out.push(suffix);
    }
    out.push('}');
    Some(())
}

pub(super) fn translate_asm_template(
    template: &str,
    slot_to_rust: &[usize],
    constraints: &[Constraint],
    types: &[Type],
    dialect: Option<AsmDialect>,
) -> Option<String> {
    let pieces = parse_asm_template(template)?;
    let mut memory_slot_reference_counts: BTreeMap<usize, usize> = BTreeMap::new();
    for piece in &pieces {
        if let TemplatePiece::Operand { slot, .. } = piece
            && matches!(constraints.get(*slot), Some(Constraint::Memory))
        {
            *memory_slot_reference_counts.entry(*slot).or_insert(0) += 1;
        }
    }
    if memory_slot_reference_counts
        .values()
        .any(|count| *count > 1)
    {
        return None;
    }
    let mut translated = String::new();
    let mut referenced_operands = BTreeSet::new();
    for piece in &pieces {
        match piece {
            TemplatePiece::Literal(text) => translated.push_str(text),
            TemplatePiece::Operand { slot, modifier } => {
                let rust_slot = *slot_to_rust.get(*slot)?;
                let constraint = constraints.get(*slot)?;
                let ty = types.get(*slot)?;
                referenced_operands.insert(rust_slot);
                render_operand_reference(
                    &mut translated,
                    rust_slot,
                    *modifier,
                    constraint,
                    constraints,
                    ty,
                    dialect,
                )?;
            }
        }
    }
    for rust_slot in 0..slot_to_rust.iter().copied().max()?.saturating_add(1) {
        if referenced_operands.contains(&rust_slot) {
            continue;
        }
        let source_slot = slot_to_rust
            .iter()
            .position(|mapped| *mapped == rust_slot)?;
        let constraint = &constraints[source_slot];
        if constraint.has_out_of_band_placement() {
            continue;
        }
        translated.push_str("\n/* {");
        translated.push_str(&rust_slot.to_string());
        if let Some(suffix) =
            register_modifier_suffix(constraint, constraints, types.get(source_slot)?)
        {
            translated.push(':');
            translated.push(suffix);
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
