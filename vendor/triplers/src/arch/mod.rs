pub mod aarch64;
pub mod amd64;
pub mod arm;
pub mod dxil;
pub mod kalimba;
pub mod mips;
pub mod nvptx;
pub mod ppc;
pub mod riscv;
pub mod spirv;
pub mod ve;
pub mod wasm;
pub mod x86;

use crate::{BitnessObservable, Canonicalizable, Parsable};

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash, PartialOrd)]
#[cfg_attr(feature = "serde", derive(::serde::Serialize, ::serde::Deserialize))]
#[non_exhaustive]
pub enum ArchPart {
    X86(x86::X86),
    Amd64(amd64::Amd64),
    Arm(arm::Arm),
    Aarch64(aarch64::Aarch64),
    Kalimba(kalimba::Kalimba),
    Mips(mips::Mips),
    Ppc(ppc::Ppc),
    Riscv(riscv::Riscv),
    SpirV(spirv::SpirV),
    Dxil(dxil::Dxil),
    Wasm(wasm::Wasm),
    Ve(ve::Ve),
    Nvptx(nvptx::Nvptx),
}

impl Parsable for ArchPart {
    fn reduce(src: &'_ str) -> Option<(Self, usize)> {
        let results: [Option<(ArchPart, usize)>; 13] = [
            x86::X86::reduce(src).map(|(v, l)| (ArchPart::X86(v), l)),
            amd64::Amd64::reduce(src).map(|(v, l)| (ArchPart::Amd64(v), l)),
            arm::Arm::reduce(src).map(|(v, l)| (ArchPart::Arm(v), l)),
            aarch64::Aarch64::reduce(src).map(|(v, l)| (ArchPart::Aarch64(v), l)),
            kalimba::Kalimba::reduce(src).map(|(v, l)| (ArchPart::Kalimba(v), l)),
            mips::Mips::reduce(src).map(|(v, l)| (ArchPart::Mips(v), l)),
            ppc::Ppc::reduce(src).map(|(v, l)| (ArchPart::Ppc(v), l)),
            riscv::Riscv::reduce(src).map(|(v, l)| (ArchPart::Riscv(v), l)),
            spirv::SpirV::reduce(src).map(|(v, l)| (ArchPart::SpirV(v), l)),
            dxil::Dxil::reduce(src).map(|(v, l)| (ArchPart::Dxil(v), l)),
            wasm::Wasm::reduce(src).map(|(v, l)| (ArchPart::Wasm(v), l)),
            ve::Ve::reduce(src).map(|(v, l)| (ArchPart::Ve(v), l)),
            nvptx::Nvptx::reduce(src).map(|(v, l)| (ArchPart::Nvptx(v), l)),
        ];

        results.into_iter().flatten().max_by_key(|(_, len)| *len)
    }
}

impl Canonicalizable for ArchPart {
    fn canonicalize(&self) -> &'static str {
        match self {
            ArchPart::X86(x) => x.canonicalize(),
            ArchPart::Amd64(x) => x.canonicalize(),
            ArchPart::Arm(x) => x.canonicalize(),
            ArchPart::Aarch64(x) => x.canonicalize(),
            ArchPart::Kalimba(x) => x.canonicalize(),
            ArchPart::Mips(x) => x.canonicalize(),
            ArchPart::Ppc(x) => x.canonicalize(),
            ArchPart::Riscv(x) => x.canonicalize(),
            ArchPart::SpirV(x) => x.canonicalize(),
            ArchPart::Dxil(x) => x.canonicalize(),
            ArchPart::Wasm(x) => x.canonicalize(),
            ArchPart::Ve(x) => x.canonicalize(),
            ArchPart::Nvptx(x) => x.canonicalize(),
        }
    }
}

impl BitnessObservable for ArchPart {
    fn bitness(&self) -> u16 {
        match self {
            ArchPart::X86(x) => x.bitness(),
            ArchPart::Amd64(x) => x.bitness(),
            ArchPart::Arm(x) => x.bitness(),
            ArchPart::Aarch64(x) => x.bitness(),
            ArchPart::Kalimba(x) => x.bitness(),
            ArchPart::Mips(x) => x.bitness(),
            ArchPart::Ppc(x) => x.bitness(),
            ArchPart::Riscv(x) => x.bitness(),
            ArchPart::SpirV(x) => x.bitness(),
            ArchPart::Dxil(x) => x.bitness(),
            ArchPart::Wasm(x) => x.bitness(),
            ArchPart::Ve(x) => x.bitness(),
            ArchPart::Nvptx(x) => x.bitness(),
        }
    }
}
