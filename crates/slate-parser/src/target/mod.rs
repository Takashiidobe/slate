pub mod aarch64;
pub mod aarch64_isa;
pub mod arm;
pub mod arm_isa;
pub mod isa;
pub mod x86;
pub mod x86_isa;
mod x86_isa_gcc_tables;
mod x86_isa_tables;

use crate::{ast::Register, target_info::TargetFamily};

pub fn flag_condition(family: TargetFamily, letters: &str) -> Option<&'static str> {
    match family {
        TargetFamily::X86 | TargetFamily::X86_64 => x86::flag_condition(letters),
        TargetFamily::AArch64 => aarch64::flag_condition(letters),
        TargetFamily::Arm32 => arm::flag_condition(letters),
    }
}

pub fn decode_register(family: TargetFamily, name: &str) -> Register {
    match family {
        TargetFamily::X86 | TargetFamily::X86_64 => x86::decode_register(name),
        TargetFamily::AArch64 => aarch64::decode_register(name),
        TargetFamily::Arm32 => arm::decode_register(name),
    }
}
