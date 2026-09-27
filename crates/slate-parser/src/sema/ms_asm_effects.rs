use crate::ast::{Register, Span};
use crate::ir::AsmAccess;

pub(super) struct Effects {
    pub(super) first: AsmAccess,
    pub(super) rest: AsmAccess,
    pub(super) implicit: &'static [&'static str],
    pub(super) x87: bool,
}

pub(super) const X87_STACK: &[&str] = &[
    "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)", "st(7)",
];

const EAX: &[&str] = &["eax"];
const EAX_EDX: &[&str] = &["eax", "edx"];

pub(super) fn effects(mnemonic: &str, prefixes: &[Span<String>], operands: usize) -> Effects {
    let x87 = mnemonic.starts_with('f') || matches!(mnemonic, "emms" | "femms");
    let first = if x87 {
        if x87_store(mnemonic) {
            AsmAccess::Write
        } else {
            AsmAccess::Read
        }
    } else if read_only(mnemonic, operands) {
        AsmAccess::Read
    } else if write_only(mnemonic) {
        AsmAccess::Write
    } else {
        AsmAccess::ReadWrite
    };
    let rest = if matches!(mnemonic, "xchg" | "xadd") {
        AsmAccess::ReadWrite
    } else {
        AsmAccess::Read
    };
    let repeated = prefixes.iter().any(|prefix| {
        matches!(
            prefix.value.to_lowercase().as_str(),
            "rep" | "repe" | "repz" | "repne" | "repnz"
        )
    });
    let implicit: &'static [&'static str] = match mnemonic {
        "mul" | "div" | "idiv" | "cwd" | "cdq" | "rdtsc" | "rdpmc" | "rdmsr" | "xgetbv"
        | "cmpxchg8b" => EAX_EDX,
        "imul" if operands == 1 => EAX_EDX,
        "cbw" | "cwde" | "lahf" | "xlat" | "xlatb" | "cmpxchg" => EAX,
        "rdtscp" => &["eax", "ecx", "edx"],
        "cpuid" => &["eax", "ebx", "ecx", "edx"],
        "loop" | "loope" | "loopz" | "loopne" | "loopnz" => &["ecx"],
        "enter" | "leave" => &["ebp"],
        "emms" | "femms" => &["mm0", "mm1", "mm2", "mm3", "mm4", "mm5", "mm6", "mm7"],
        "popa" | "popad" => &["eax", "ebx", "ecx", "edx", "esi", "edi", "ebp"],
        "call" => &["eax", "ecx", "edx"],
        "movsb" | "movsw" | "movsd" | "cmpsb" | "cmpsw" | "cmpsd" if operands == 0 => {
            if repeated {
                &["ecx", "esi", "edi"]
            } else {
                &["esi", "edi"]
            }
        }
        "stosb" | "stosw" | "stosd" | "scasb" | "scasw" | "scasd" | "insb" | "insw" | "insd" => {
            if repeated {
                &["ecx", "edi"]
            } else {
                &["edi"]
            }
        }
        "lodsb" | "lodsw" | "lodsd" => {
            if repeated {
                &["eax", "ecx", "esi"]
            } else {
                &["eax", "esi"]
            }
        }
        "outsb" | "outsw" | "outsd" => {
            if repeated {
                &["ecx", "esi"]
            } else {
                &["esi"]
            }
        }
        _ => &[],
    };
    Effects {
        first,
        rest,
        implicit,
        x87,
    }
}

fn read_only(mnemonic: &str, operands: usize) -> bool {
    mnemonic.starts_with('j')
        || mnemonic.starts_with("prefetch")
        || matches!(
            mnemonic,
            "cmp"
                | "test"
                | "bt"
                | "push"
                | "ptest"
                | "vptest"
                | "comiss"
                | "comisd"
                | "ucomiss"
                | "ucomisd"
                | "vcomiss"
                | "vcomisd"
                | "vucomiss"
                | "vucomisd"
                | "mul"
                | "div"
                | "idiv"
                | "call"
                | "int"
                | "out"
                | "ret"
                | "retn"
                | "retf"
                | "loop"
                | "loope"
                | "loopz"
                | "loopne"
                | "loopnz"
                | "ldmxcsr"
                | "fxrstor"
                | "lgdt"
                | "lidt"
                | "lldt"
                | "ltr"
                | "lmsw"
                | "invlpg"
                | "clflush"
                | "clflushopt"
                | "verr"
                | "verw"
                | "bound"
                | "_emit"
        )
        || (mnemonic == "imul" && operands == 1)
}

fn write_only(mnemonic: &str) -> bool {
    (mnemonic.starts_with("set") && mnemonic.len() > 3)
        || mnemonic.starts_with("cvt")
        || matches!(
            mnemonic,
            "mov"
                | "movzx"
                | "movsx"
                | "movsxd"
                | "lea"
                | "pop"
                | "in"
                | "movd"
                | "movq"
                | "movdqa"
                | "movdqu"
                | "movaps"
                | "movups"
                | "movapd"
                | "movupd"
                | "movss"
                | "movsd"
                | "movnti"
                | "movntdq"
                | "movntps"
                | "movntpd"
                | "movntq"
                | "stmxcsr"
                | "sgdt"
                | "sidt"
                | "sldt"
                | "smsw"
                | "str"
                | "rdrand"
                | "rdseed"
                | "popcnt"
                | "lzcnt"
                | "tzcnt"
        )
}

fn x87_store(mnemonic: &str) -> bool {
    matches!(
        mnemonic,
        "fst"
            | "fstp"
            | "fist"
            | "fistp"
            | "fisttp"
            | "fbstp"
            | "fstsw"
            | "fnstsw"
            | "fstcw"
            | "fnstcw"
            | "fstenv"
            | "fnstenv"
            | "fsave"
            | "fnsave"
            | "fxsave"
    )
}

pub(super) fn writes(access: AsmAccess) -> bool {
    matches!(access, AsmAccess::Write | AsmAccess::ReadWrite)
}

pub(super) fn union(lhs: AsmAccess, rhs: AsmAccess) -> AsmAccess {
    if lhs == rhs {
        lhs
    } else {
        AsmAccess::ReadWrite
    }
}

pub(super) fn clobbered(register: &Register) -> Option<&'static str> {
    const GENERAL: [&str; 7] = ["eax", "edx", "ecx", "ebx", "esi", "edi", "ebp"];
    match register {
        Register::X86(info) if info.number < GENERAL.len() => Some(GENERAL[info.number]),
        Register::X86(info) if info.number == 7 => None,
        Register::X86(info) => Some(info.canonical),
        Register::Aarch64(_) | Register::Other(_) => None,
    }
}
