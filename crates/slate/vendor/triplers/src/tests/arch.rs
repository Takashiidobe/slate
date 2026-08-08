use crate::Canonicalizable;
use crate::Parsable;
use crate::arch::*;

#[test]
fn test_x86() {
    let cases = [
        ("x86", x86::X86::X86, 3),
        ("i386", x86::X86::I386, 4),
        ("i486", x86::X86::I486, 4),
        ("i586", x86::X86::I586, 4),
        ("i686", x86::X86::I686, 4),
    ];
    for (input, expected_variant, expected_len) in cases {
        let (variant, len) = x86::X86::reduce(input).unwrap();
        assert_eq!(variant, expected_variant);
        assert_eq!(len, expected_len);
    }
}

#[test]
fn test_amd64() {
    let cases = [
        ("x86_64", amd64::Amd64::X86_64, 6),
        ("amd64", amd64::Amd64::Amd64, 5),
        ("em64t", amd64::Amd64::Em64t, 5),
        ("x64", amd64::Amd64::X64, 3),
    ];
    for (input, expected_variant, expected_len) in cases {
        let (variant, len) = amd64::Amd64::reduce(input).unwrap();
        assert_eq!(variant, expected_variant);
        assert_eq!(len, expected_len);
        assert_eq!(variant.canonicalize(), "x86_64");
    }
}

#[test]
fn test_arm() {
    let cases = [
        ("arm", arm::Arm::Arm, 3),
        ("armv4t", arm::Arm::V4t, 6),
        ("armv5", arm::Arm::V5, 5),
        ("armv5te", arm::Arm::V5te, 7),
        ("armv6", arm::Arm::V6, 5),
        ("armv6m", arm::Arm::V6m, 6),
        ("armv6k", arm::Arm::V6k, 6),
        ("armv6t2", arm::Arm::V6t2, 7),
        ("armv7", arm::Arm::V7, 5),
        ("armv7em", arm::Arm::V7em, 7),
        ("armv7m", arm::Arm::V7m, 6),
        ("armv7s", arm::Arm::V7s, 6),
        ("armv7k", arm::Arm::V7k, 6),
        ("armv7ve", arm::Arm::V7ve, 7),
        ("armv8a", arm::Arm::V8, 6),
        ("armv8.1a", arm::Arm::V8_1a, 8),
        ("armv8.2a", arm::Arm::V8_2a, 8),
        ("armv8.3a", arm::Arm::V8_3a, 8),
        ("armv8.4a", arm::Arm::V8_4a, 8),
        ("armv8.5a", arm::Arm::V8_5a, 8),
        ("armv8.6a", arm::Arm::V8_6a, 8),
        ("armv8.7a", arm::Arm::V8_7a, 8),
        ("armv8.8a", arm::Arm::V8_8a, 8),
        ("armv8.9a", arm::Arm::V8_9a, 8),
        ("armv8r", arm::Arm::V8r, 6),
        ("armv9a", arm::Arm::V9, 6),
        ("armv9.1a", arm::Arm::V9_1a, 8),
        ("armv9.2a", arm::Arm::V9_2a, 8),
        ("armv9.3a", arm::Arm::V9_3a, 8),
        ("armv9.4a", arm::Arm::V9_4a, 8),
        ("armv9.5a", arm::Arm::V9_5a, 8),
        ("armv9.6a", arm::Arm::V9_6a, 8),
        ("armv9.7a", arm::Arm::V9_7a, 8),
    ];
    for (input, expected_variant, expected_len) in cases {
        let (variant, len) = arm::Arm::reduce(input).unwrap();
        assert_eq!(variant, expected_variant);
        assert_eq!(len, expected_len);
        let canonical = variant.canonicalize();
        if expected_variant != arm::Arm::Arm {
            assert!(canonical.starts_with("armv"));
        }
        if matches!(variant, arm::Arm::Arm) {
            assert_eq!(canonical, "arm");
        } else {
            assert!(canonical.len() > 3);
        }
    }
}

#[test]
fn test_aarch64() {
    let cases = [
        ("aarch64", aarch64::Aarch64::Aarch64, 7),
        ("arm64", aarch64::Aarch64::Arm64, 5),
        ("arm64e", aarch64::Aarch64::Arm64e, 6),
        ("arm64ec", aarch64::Aarch64::Arm64ec, 7),
    ];
    for (input, expected_variant, expected_len) in cases {
        let (variant, len) = aarch64::Aarch64::reduce(input).unwrap();
        assert_eq!(variant, expected_variant);
        assert_eq!(len, expected_len);
        let canonical = variant.canonicalize();
        if matches!(variant, aarch64::Aarch64::Arm64 | aarch64::Aarch64::Aarch64) {
            assert_eq!(canonical, "aarch64");
        } else {
            assert_eq!(
                canonical,
                match variant {
                    aarch64::Aarch64::Arm64e => "arm64e",
                    aarch64::Aarch64::Arm64ec => "arm64ec",
                    _ => unreachable!(),
                }
            );
        }
    }
}

#[test]
fn test_kalimba() {
    let cases = [
        ("kalimba", kalimba::Kalimba::Kalimba, 7),
        ("kalimba3", kalimba::Kalimba::V3, 8),
        ("kalimba4", kalimba::Kalimba::V4, 8),
        ("kalimba5", kalimba::Kalimba::V5, 8),
    ];
    for (input, expected_variant, expected_len) in cases {
        let (variant, len) = kalimba::Kalimba::reduce(input).unwrap();
        assert_eq!(variant, expected_variant);
        assert_eq!(len, expected_len);
        assert_eq!(variant.canonicalize(), input);
    }
}

#[test]
fn test_mips() {
    let cases = [
        ("mips", mips::Mips::Mips, 4),
        ("mipsel", mips::Mips::MipsEL, 6),
        ("mips64", mips::Mips::Mips64, 6),
        ("mips64el", mips::Mips::Mips64EL, 8),
        ("mips32r6", mips::Mips::Mips32R6, 8),
        ("mips64r6", mips::Mips::Mips64R6, 8),
    ];
    for (input, expected_variant, expected_len) in cases {
        let (variant, len) = mips::Mips::reduce(input).unwrap();
        assert_eq!(variant, expected_variant);
        assert_eq!(len, expected_len);
        assert_eq!(variant.canonicalize(), input);
    }
}

#[test]
fn test_ppc() {
    let cases = [
        ("ppc", ppc::Ppc::Ppc, 3),
        ("ppc64", ppc::Ppc::Ppc64, 5),
        ("ppcel", ppc::Ppc::PpcEL, 5),
        ("ppc64el", ppc::Ppc::Ppc64EL, 7),
        ("ppcspe", ppc::Ppc::PpcSPE, 6),
        ("powerpc", ppc::Ppc::Ppc, 7),
        ("powerpc64", ppc::Ppc::Ppc64, 9),
        ("powerpcel", ppc::Ppc::PpcEL, 9),
        ("powerpc64el", ppc::Ppc::Ppc64EL, 11),
        ("powerpcspe", ppc::Ppc::PpcSPE, 10),
    ];
    for (input, expected_variant, expected_len) in cases {
        let (variant, len) = ppc::Ppc::reduce(input).unwrap();
        assert_eq!(variant, expected_variant);
        assert_eq!(len, expected_len);
        let canonical = variant.canonicalize();
        assert!(canonical.starts_with("powerpc"));
    }
}

#[test]
fn test_spirv() {
    let cases = [
        ("spirv", spirv::SpirV::SpirV, 5),
        ("spirv1.0", spirv::SpirV::V10, 8),
        ("spirv1.1", spirv::SpirV::V11, 8),
        ("spirv1.2", spirv::SpirV::V12, 8),
        ("spirv1.3", spirv::SpirV::V13, 8),
        ("spirv1.4", spirv::SpirV::V14, 8),
        ("spirv1.5", spirv::SpirV::V15, 8),
        ("spirv1.6", spirv::SpirV::V16, 8),
    ];
    for (input, expected_variant, expected_len) in cases {
        let (variant, len) = spirv::SpirV::reduce(input).unwrap();
        assert_eq!(variant, expected_variant);
        assert_eq!(len, expected_len);
        assert_eq!(variant.canonicalize(), input);
    }
}

#[test]
fn test_dxil() {
    let cases = [
        ("dxil", dxil::Dxil::Dxil, 4),
        ("dxil1.0", dxil::Dxil::V1_0, 7),
        ("dxil1.1", dxil::Dxil::V1_1, 7),
        ("dxil1.2", dxil::Dxil::V1_2, 7),
        ("dxil1.3", dxil::Dxil::V1_3, 7),
        ("dxil1.4", dxil::Dxil::V1_4, 7),
        ("dxil1.5", dxil::Dxil::V1_5, 7),
        ("dxil1.6", dxil::Dxil::V1_6, 7),
        ("dxil1.7", dxil::Dxil::V1_7, 7),
        ("dxil1.8", dxil::Dxil::V1_8, 7),
        ("dxil1.9", dxil::Dxil::V1_9, 7),
    ];
    for (input, expected_variant, expected_len) in cases {
        let (variant, len) = dxil::Dxil::reduce(input).unwrap();
        assert_eq!(variant, expected_variant);
        assert_eq!(len, expected_len);
        assert_eq!(variant.canonicalize(), input);
    }
}

#[test]
fn test_arch_part() {
    let (arch, len) = ArchPart::reduce("arm64ec-unknown-linux-gnu").unwrap();
    assert!(matches!(arch, ArchPart::Aarch64(aarch64::Aarch64::Arm64ec)));
    assert_eq!(len, 7);

    let (arch, len) = ArchPart::reduce("arm64-unknown-linux-gnu").unwrap();
    assert!(matches!(arch, ArchPart::Aarch64(aarch64::Aarch64::Arm64)));
    assert_eq!(len, 5);

    let (arch, len) = ArchPart::reduce("x86_64-unknown-linux-gnu").unwrap();
    assert!(matches!(arch, ArchPart::Amd64(amd64::Amd64::X86_64)));
    assert_eq!(len, 6);

    let (arch, len) = ArchPart::reduce("powerpc64el").unwrap();
    assert!(matches!(arch, ArchPart::Ppc(ppc::Ppc::Ppc64EL)));
    assert_eq!(len, 11);
}
