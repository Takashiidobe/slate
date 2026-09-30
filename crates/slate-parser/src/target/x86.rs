use crate::ast::{Register, RegisterInfo, X86RegisterWidth};
use X86RegisterWidth::{Bits16, Bits32, Bits64, High8, Low8};

pub type X86Register = RegisterInfo<X86RegisterWidth>;

pub const GCC_REGISTER_NAMES: &[&str] = &[
    "ax", "dx", "cx", "bx", "si", "di", "bp", "sp", //
    "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)", //
    "argp", "flags", "fpcr", "fpsr", "dirflag", "frame", "xmm0", "xmm1", //
    "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7", "mm0", "mm1", //
    "mm2", "mm3", "mm4", "mm5", "mm6", "mm7", "r8", "r9", //
    "r10", "r11", "r12", "r13", "r14", "r15", "xmm8", "xmm9", //
    "xmm10", "xmm11", "xmm12", "xmm13", "xmm14", "xmm15", "ymm0", "ymm1", //
    "ymm2", "ymm3", "ymm4", "ymm5", "ymm6", "ymm7", "ymm8", "ymm9", //
    "ymm10", "ymm11", "ymm12", "ymm13", "ymm14", "ymm15", "xmm16", "xmm17", //
    "xmm18", "xmm19", "xmm20", "xmm21", "xmm22", "xmm23", "xmm24", "xmm25", //
    "xmm26", "xmm27", "xmm28", "xmm29", "xmm30", "xmm31", "ymm16", "ymm17", //
    "ymm18", "ymm19", "ymm20", "ymm21", "ymm22", "ymm23", "ymm24", "ymm25", //
    "ymm26", "ymm27", "ymm28", "ymm29", "ymm30", "ymm31", "zmm0", "zmm1", //
    "zmm2", "zmm3", "zmm4", "zmm5", "zmm6", "zmm7", "zmm8", "zmm9", //
    "zmm10", "zmm11", "zmm12", "zmm13", "zmm14", "zmm15", "zmm16", "zmm17", //
    "zmm18", "zmm19", "zmm20", "zmm21", "zmm22", "zmm23", "zmm24", "zmm25", //
    "zmm26", "zmm27", "zmm28", "zmm29", "zmm30", "zmm31", "k0", "k1", //
    "k2", "k3", "k4", "k5", "k6", "k7", //
    "cr0", "cr2", "cr3", "cr4", "cr8", //
    "dr0", "dr1", "dr2", "dr3", "dr6", "dr7", //
    "bnd0", "bnd1", "bnd2", "bnd3", //
    "tmm0", "tmm1", "tmm2", "tmm3", "tmm4", "tmm5", "tmm6", "tmm7", //
    "r16", "r17", "r18", "r19", "r20", "r21", "r22", "r23", //
    "r24", "r25", "r26", "r27", "r28", "r29", "r30", "r31", //
];

const ADDITIONAL_REGISTER_NAMES: &[(&str, usize, X86RegisterWidth)] = &[
    ("al", 0, Low8),
    ("ah", 0, High8),
    ("eax", 0, Bits32),
    ("rax", 0, Bits64),
    ("bl", 3, Low8),
    ("bh", 3, High8),
    ("ebx", 3, Bits32),
    ("rbx", 3, Bits64),
    ("cl", 2, Low8),
    ("ch", 2, High8),
    ("ecx", 2, Bits32),
    ("rcx", 2, Bits64),
    ("dl", 1, Low8),
    ("dh", 1, High8),
    ("edx", 1, Bits32),
    ("rdx", 1, Bits64),
    ("esi", 4, Bits32),
    ("rsi", 4, Bits64),
    ("edi", 5, Bits32),
    ("rdi", 5, Bits64),
    ("esp", 7, Bits32),
    ("rsp", 7, Bits64),
    ("ebp", 6, Bits32),
    ("rbp", 6, Bits64),
    ("r8d", 38, Bits32),
    ("r8w", 38, Bits16),
    ("r8b", 38, Low8),
    ("r9d", 39, Bits32),
    ("r9w", 39, Bits16),
    ("r9b", 39, Low8),
    ("r10d", 40, Bits32),
    ("r10w", 40, Bits16),
    ("r10b", 40, Low8),
    ("r11d", 41, Bits32),
    ("r11w", 41, Bits16),
    ("r11b", 41, Low8),
    ("r12d", 42, Bits32),
    ("r12w", 42, Bits16),
    ("r12b", 42, Low8),
    ("r13d", 43, Bits32),
    ("r13w", 43, Bits16),
    ("r13b", 43, Low8),
    ("r14d", 44, Bits32),
    ("r14w", 44, Bits16),
    ("r14b", 44, Low8),
    ("r15d", 45, Bits32),
    ("r15w", 45, Bits16),
    ("r15b", 45, Low8),
    ("r16d", 165, Bits32),
    ("r16w", 165, Bits16),
    ("r16b", 165, Low8),
    ("r17d", 166, Bits32),
    ("r17w", 166, Bits16),
    ("r17b", 166, Low8),
    ("r18d", 167, Bits32),
    ("r18w", 167, Bits16),
    ("r18b", 167, Low8),
    ("r19d", 168, Bits32),
    ("r19w", 168, Bits16),
    ("r19b", 168, Low8),
    ("r20d", 169, Bits32),
    ("r20w", 169, Bits16),
    ("r20b", 169, Low8),
    ("r21d", 170, Bits32),
    ("r21w", 170, Bits16),
    ("r21b", 170, Low8),
    ("r22d", 171, Bits32),
    ("r22w", 171, Bits16),
    ("r22b", 171, Low8),
    ("r23d", 172, Bits32),
    ("r23w", 172, Bits16),
    ("r23b", 172, Low8),
    ("r24d", 173, Bits32),
    ("r24w", 173, Bits16),
    ("r24b", 173, Low8),
    ("r25d", 174, Bits32),
    ("r25w", 174, Bits16),
    ("r25b", 174, Low8),
    ("r26d", 175, Bits32),
    ("r26w", 175, Bits16),
    ("r26b", 175, Low8),
    ("r27d", 176, Bits32),
    ("r27w", 176, Bits16),
    ("r27b", 176, Low8),
    ("r28d", 177, Bits32),
    ("r28w", 177, Bits16),
    ("r28b", 177, Low8),
    ("r29d", 178, Bits32),
    ("r29w", 178, Bits16),
    ("r29b", 178, Low8),
    ("r30d", 179, Bits32),
    ("r30w", 179, Bits16),
    ("r30b", 179, Low8),
    ("r31d", 180, Bits32),
    ("r31w", 180, Bits16),
    ("r31b", 180, Low8),
];

pub fn decode_register(name: &str) -> Register {
    lookup(name).map_or_else(|| Register::Other(name.to_string()), Register::X86)
}

fn lookup(name: &str) -> Option<X86Register> {
    let bare = name.strip_prefix(['%', '#']).unwrap_or(name);
    if bare.starts_with(|c: char| c.is_ascii_digit())
        && let Some(number) = parse_auto_radix(bare)
    {
        return (number < GCC_REGISTER_NAMES.len()).then(|| register(name, number, None));
    }
    if let Some(number) = GCC_REGISTER_NAMES.iter().position(|&known| known == bare) {
        return Some(register(name, number, canonical_width(number)));
    }
    ADDITIONAL_REGISTER_NAMES
        .iter()
        .find(|(known, ..)| *known == bare)
        .map(|&(_, number, width)| register(name, number, Some(width)))
}

fn register(spelling: &str, number: usize, width: Option<X86RegisterWidth>) -> X86Register {
    X86Register {
        spelling: spelling.to_string(),
        number,
        canonical: GCC_REGISTER_NAMES[number],
        width,
    }
}

fn canonical_width(number: usize) -> Option<X86RegisterWidth> {
    match number {
        0..=7 => Some(Bits16),
        38..=45 | 165..=180 => Some(Bits64),
        _ => None,
    }
}

pub(crate) fn parse_auto_radix(text: &str) -> Option<usize> {
    let (digits, radix) =
        if let Some(rest) = text.strip_prefix("0x").or_else(|| text.strip_prefix("0X")) {
            (rest, 16)
        } else if let Some(rest) = text.strip_prefix("0b").or_else(|| text.strip_prefix("0B")) {
            (rest, 2)
        } else if let Some(rest) = text.strip_prefix("0o") {
            (rest, 8)
        } else if text.len() > 1 && text.starts_with('0') && text.as_bytes()[1].is_ascii_digit() {
            (&text[1..], 8)
        } else {
            (text, 10)
        };
    if digits.is_empty() || !digits.bytes().all(|byte| byte.is_ascii_alphanumeric()) {
        return None;
    }
    usize::from_str_radix(digits, radix).ok()
}
