use crate::ast::{ArmRegisterWidth, Register, RegisterInfo};
use crate::target::x86::parse_auto_radix;

pub type ArmRegister = RegisterInfo<ArmRegisterWidth>;

const CORE_NAMES: &[&str] = &[
    "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r13", "r14",
    "r15",
];

const CORE_ALIASES: &[(&str, usize)] = &[
    ("a1", 0),
    ("a2", 1),
    ("a3", 2),
    ("a4", 3),
    ("v1", 4),
    ("v2", 5),
    ("v3", 6),
    ("v4", 7),
    ("v5", 8),
    ("v6", 9),
    ("v7", 10),
    ("sb", 9),
    ("sl", 10),
    ("fp", 11),
    ("ip", 12),
    ("sp", 13),
    ("lr", 14),
    ("pc", 15),
];

const VFP_BANKS: &[(char, usize, ArmRegisterWidth, &[&str])] = &[
    ('s', 16, ArmRegisterWidth::Bits32, &S_NAMES),
    ('d', 48, ArmRegisterWidth::Bits64, &D_NAMES),
    ('q', 80, ArmRegisterWidth::Bits128, &Q_NAMES),
];

macro_rules! numbered {
    ($prefix:literal; $($n:literal)*) => {
        [$(concat!($prefix, stringify!($n))),*]
    };
}

const S_NAMES: [&str; 32] = numbered!("s"; 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31);
pub const D_NAMES: [&str; 32] = numbered!("d"; 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31);
const Q_NAMES: [&str; 16] = numbered!("q"; 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15);

pub const FLAG_CONDITIONS: &[&str] = super::aarch64::FLAG_CONDITIONS;

pub fn flag_condition(letters: &str) -> Option<&'static str> {
    let condition = letters.strip_prefix("@cc")?;
    FLAG_CONDITIONS
        .iter()
        .copied()
        .find(|known| *known == condition)
}

pub fn decode_register(name: &str) -> Register {
    lookup(name).map_or_else(|| Register::Other(name.to_string()), Register::Arm)
}

fn lookup(name: &str) -> Option<ArmRegister> {
    let bare = name.strip_prefix(['%', '#']).unwrap_or(name);
    let core = |number: usize| RegisterInfo {
        spelling: name.to_string(),
        number,
        canonical: CORE_NAMES[number],
        width: Some(ArmRegisterWidth::Bits32),
    };
    if bare.starts_with(|c: char| c.is_ascii_digit()) {
        return parse_auto_radix(bare)
            .filter(|number| *number < CORE_NAMES.len())
            .map(core);
    }
    if let Some(number) = CORE_NAMES.iter().position(|known| *known == bare) {
        return Some(core(number));
    }
    if let Some((_, number)) = CORE_ALIASES.iter().find(|(alias, _)| *alias == bare) {
        return Some(core(*number));
    }
    if bare == "vfpcc" {
        return Some(RegisterInfo {
            spelling: name.to_string(),
            number: 96,
            canonical: "vfpcc",
            width: None,
        });
    }
    VFP_BANKS.iter().find_map(|(prefix, base, width, names)| {
        let number = bare.strip_prefix(*prefix)?.parse::<usize>().ok()?;
        let canonical = names.get(number)?;
        (bare == *canonical).then(|| RegisterInfo {
            spelling: name.to_string(),
            number: base + number,
            canonical,
            width: Some(*width),
        })
    })
}
