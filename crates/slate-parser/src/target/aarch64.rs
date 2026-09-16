#![expect(dead_code)]
use crate::{
    ast::{AArch64RegisterWidth, RegisterInfo},
    target::x86::parse_auto_radix,
};
use AArch64RegisterWidth::{
    Bits8, Bits16, Bits32, Bits64, Bits128, ScalablePredicate, ScalableVector,
};

pub type AArch64Register = RegisterInfo<AArch64RegisterWidth>;

pub const GCC_REGISTER_NAMES: &[&str] = &[
    // x0..x30: general-purpose registers
    "x0",
    "x1",
    "x2",
    "x3",
    "x4",
    "x5",
    "x6",
    "x7",
    "x8",
    "x9",
    "x10",
    "x11",
    "x12",
    "x13",
    "x14",
    "x15",
    "x16",
    "x17",
    "x18",
    "x19",
    "x20",
    "x21",
    "x22",
    "x23",
    "x24",
    "x25",
    "x26",
    "x27",
    "x28",
    "x29",
    "x30",
    // x31/stack pointer
    "sp",
    // v32..v63: FP/SIMD registers
    "v0",
    "v1",
    "v2",
    "v3",
    "v4",
    "v5",
    "v6",
    "v7",
    "v8",
    "v9",
    "v10",
    "v11",
    "v12",
    "v13",
    "v14",
    "v15",
    "v16",
    "v17",
    "v18",
    "v19",
    "v20",
    "v21",
    "v22",
    "v23",
    "v24",
    "v25",
    "v26",
    "v27",
    "v28",
    "v29",
    "v30",
    "v31",
    // r64..r67: GCC internal / special registers
    "sfp",
    "ap",
    "cc",
    "vg",
    // r68..r83 = p0..p15: SVE predicate registers
    "p0",
    "p1",
    "p2",
    "p3",
    "p4",
    "p5",
    "p6",
    "p7",
    "p8",
    "p9",
    "p10",
    "p11",
    "p12",
    "p13",
    "p14",
    "p15",
    // 84..=86
    "fpmr",
    "ffr",
    "ffrt",
    // 87..=94: GCC internal registers, may be more
    "lowering",
    "tpidr2_block",
    "sme_state",
    "tpidr2_setup",
    "za_free",
    "za_saved",
    "za",
    "zt0",
];

macro_rules! additional_register_names {
    ($($n:literal),* $(,)?) => {
        &[
            $(
                // Core register aliases.
                (
                    concat!("r", stringify!($n)),
                    $n,
                    Bits64,
                ),
                (
                    concat!("w", stringify!($n)),
                    $n,
                    Bits32,
                ),

                // FP/SIMD/SVE aliases.
                (
                    concat!("q", stringify!($n)),
                    32 + $n,
                    Bits128,
                ),
                (
                    concat!("d", stringify!($n)),
                    32 + $n,
                    Bits64,
                ),
                (
                    concat!("s", stringify!($n)),
                    32 + $n,
                    Bits32,
                ),
                (
                    concat!("h", stringify!($n)),
                    32 + $n,
                    Bits16,
                ),
                (
                    concat!("b", stringify!($n)),
                    32 + $n,
                    Bits8,
                ),
                (
                    concat!("z", stringify!($n)),
                    32 + $n,
                    ScalableVector,
                ),
            )*
        ]
    };
}

const CORE_AND_VECTOR_ALIASES: &[(&str, usize, AArch64RegisterWidth)] = additional_register_names!(
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
    26, 27, 28, 29, 30
);

macro_rules! vector31_aliases {
    ($n:literal) => {
        &[
            (concat!("q", stringify!($n)), 32 + $n, Bits128),
            (concat!("d", stringify!($n)), 32 + $n, Bits64),
            (concat!("s", stringify!($n)), 32 + $n, Bits32),
            (concat!("h", stringify!($n)), 32 + $n, Bits16),
            (concat!("b", stringify!($n)), 32 + $n, Bits8),
            (concat!("z", stringify!($n)), 32 + $n, ScalableVector),
        ]
    };
}

const VECTOR31_ALIASES: &[(&str, usize, AArch64RegisterWidth)] = vector31_aliases!(31);

const SPECIAL_ALIASES: &[(&str, usize, AArch64RegisterWidth)] = &[("wsp", 31, Bits32)];

fn lookup(name: &str) -> Option<AArch64Register> {
    let bare = name.strip_prefix(['%', '#']).unwrap_or(name);

    // GCC hard-register number.
    if bare.starts_with(|c: char| c.is_ascii_digit())
        && let Some(number) = parse_auto_radix(bare)
    {
        return (number < GCC_REGISTER_NAMES.len())
            .then(|| register(name, number, canonical_width(number)));
    }

    // GCC canonical names.
    if let Some(number) = GCC_REGISTER_NAMES.iter().position(|&known| known == bare) {
        return Some(register(name, number, canonical_width(number)));
    }

    lookup_alias(name, bare)
}

fn lookup_alias(spelling: &str, name: &str) -> Option<AArch64Register> {
    // wsp is sp
    if name == "wsp" {
        return Some(register(spelling, 31, Some(Bits32)));
    }

    // r0..r30 / w0..w30
    if let Some(number) = parse_numbered(name, 'r', 30) {
        return Some(register(spelling, number, Some(Bits64)));
    }

    if let Some(number) = parse_numbered(name, 'w', 30) {
        return Some(register(spelling, number, Some(Bits32)));
    }

    // b/h/s/d/q/z0..31 all refer to the same GCC hard registers
    // as v0..v31.
    for (prefix, width) in [
        ('b', Bits8),
        ('h', Bits16),
        ('s', Bits32),
        ('d', Bits64),
        ('q', Bits128),
        ('z', ScalableVector),
    ] {
        if let Some(number) = parse_numbered(name, prefix, 31) {
            return Some(register(spelling, 32 + number, Some(width)));
        }
    }

    // pn0..pn15 aliases p0..p15.
    if let Some(rest) = name.strip_prefix("pn")
        && let Ok(number) = rest.parse::<usize>()
        && number <= 15
    {
        return Some(register(spelling, 68 + number, Some(ScalablePredicate)));
    }

    None
}

fn parse_numbered(name: &str, prefix: char, max: usize) -> Option<usize> {
    let rest = name.strip_prefix(prefix)?;
    let number = rest.parse::<usize>().ok()?;
    (number <= max).then_some(number)
}

fn register(
    spelling: &str,
    number: usize,
    width: Option<AArch64RegisterWidth>,
) -> RegisterInfo<AArch64RegisterWidth> {
    RegisterInfo {
        spelling: spelling.to_string(),
        number,
        canonical: GCC_REGISTER_NAMES[number],
        width,
    }
}

fn canonical_width(number: usize) -> Option<AArch64RegisterWidth> {
    match number {
        // x0..x30, sp
        0..=31 => Some(Bits64),

        // v0..v31
        32..=63 => Some(Bits128),

        // p0..p15
        68..=83 => Some(ScalablePredicate),

        // FPMR is a 64-bit system register.
        84 => Some(Bits64),

        // First-fault register is predicate-sized.
        85 => Some(ScalablePredicate),

        _ => None,
    }
}
