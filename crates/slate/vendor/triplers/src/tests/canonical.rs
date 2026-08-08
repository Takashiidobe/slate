use crate::{
    Canonicalizable,
    arch::{self, ArchPart},
    env::Env,
    kernel::Kernel,
    obj::Obj,
    vendor::Vendor,
};

#[test]
fn test_arch_canonical() {
    assert_eq!(ArchPart::X86(arch::x86::X86::X86).canonicalize(), "i386");
    assert_eq!(ArchPart::X86(arch::x86::X86::I686).canonicalize(), "i686");
    assert_eq!(
        ArchPart::Amd64(arch::amd64::Amd64::X86_64).canonicalize(),
        "x86_64"
    );
    assert_eq!(ArchPart::Arm(arch::arm::Arm::V7).canonicalize(), "armv7");
    assert_eq!(
        ArchPart::Aarch64(arch::aarch64::Aarch64::Arm64).canonicalize(),
        "aarch64"
    );
    assert_eq!(
        ArchPart::Kalimba(arch::kalimba::Kalimba::V5).canonicalize(),
        "kalimba5"
    );
    assert_eq!(
        ArchPart::Mips(arch::mips::Mips::Mips64EL).canonicalize(),
        "mips64el"
    );
    assert_eq!(
        ArchPart::Ppc(arch::ppc::Ppc::Ppc64).canonicalize(),
        "powerpc64"
    );
    assert_eq!(
        ArchPart::SpirV(arch::spirv::SpirV::V16).canonicalize(),
        "spirv1.6"
    );
    assert_eq!(
        ArchPart::Dxil(arch::dxil::Dxil::V1_9).canonicalize(),
        "dxil1.9"
    );
}

#[test]
fn test_vendor_canonical() {
    assert_eq!(Vendor::Unknown.canonicalize(), "unknown");
    assert_eq!(Vendor::Apple.canonicalize(), "apple");
}

#[test]
fn test_kernel_canonical() {
    assert_eq!(Kernel::Linux.canonicalize(), "linux");
    assert_eq!(Kernel::FreeBSD.canonicalize(), "freebsd");
    assert_eq!(Kernel::WASI.canonicalize(), "wasi");
}

#[test]
fn test_env_canonical() {
    assert_eq!(Env::GNU.canonicalize(), "gnu");
    assert_eq!(Env::GNUEABI.canonicalize(), "gnueabi");
    assert_eq!(Env::Musl.canonicalize(), "musl");
}

#[test]
fn test_obj_canonical() {
    assert_eq!(Obj::ELF.canonicalize(), "elf");
    assert_eq!(Obj::MachO.canonicalize(), "macho");
}
