use crate::{Triple, arch::riscv::Riscv};

#[test]
fn parses_riscv_architectures() {
    for (input, arch, bits) in [
        ("riscv32-unknown-linux-gnu", Riscv::Riscv32, 32),
        ("riscv64-unknown-linux-gnu", Riscv::Riscv64, 64),
    ] {
        let triple = Triple::parse(input).unwrap();
        assert_eq!(triple.arch, crate::ArchPart::Riscv(arch));
        assert_eq!(triple.bitness(), bits);
    }
}
