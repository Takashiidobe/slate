use crate::{
    ParseError,
    arch::{self, ArchPart},
    env::Env,
    kernel::Kernel,
    obj::Obj,
    triple::Triple,
    vendor::Vendor,
};

#[test]
fn test_parse_full_triple() {
    let triple = Triple::parse("x86_64-unknown-linux-gnu").unwrap();
    assert!(matches!(
        triple.arch,
        ArchPart::Amd64(arch::amd64::Amd64::X86_64)
    ));
    assert_eq!(triple.vendor, Some(Vendor::Unknown));
    assert_eq!(triple.kernel, Kernel::Linux);
    assert_eq!(triple.env, Some(Env::GNU));
    assert_eq!(triple.obj, None);
}

#[test]
fn test_parse_without_vendor() {
    let triple = Triple::parse("armv7-linux-gnueabihf").unwrap();
    assert!(matches!(triple.arch, ArchPart::Arm(arch::arm::Arm::V7)));
    assert_eq!(triple.vendor, None);
    assert_eq!(triple.kernel, Kernel::Linux);
    assert_eq!(triple.env, Some(Env::GNUEABIHF));
    assert_eq!(triple.obj, None);
}

#[test]
fn test_parse_with_obj() {
    let triple = Triple::parse("aarch64-unknown-linux-gnu-elf").unwrap();
    assert!(matches!(
        triple.arch,
        ArchPart::Aarch64(arch::aarch64::Aarch64::Aarch64)
    ));
    assert_eq!(triple.vendor, Some(Vendor::Unknown));
    assert_eq!(triple.kernel, Kernel::Linux);
    assert_eq!(triple.env, Some(Env::GNU));
    assert_eq!(triple.obj, Some(Obj::ELF));
}

#[test]
fn test_parse_without_env() {
    let triple = Triple::parse("x86_64-unknown-linux-elf").unwrap();
    assert_eq!(triple.env, None);
    assert_eq!(triple.obj, Some(Obj::ELF));
}

#[test]
fn test_parse_only_arch_kernel() {
    let triple = Triple::parse("arm-none-eabi").unwrap();
    assert!(matches!(triple.arch, ArchPart::Arm(arch::arm::Arm::Arm)));
    assert_eq!(triple.vendor, None);
    assert_eq!(triple.kernel, Kernel::None);
    assert_eq!(triple.env, Some(Env::EABI));
    assert_eq!(triple.obj, None);
}

#[test]
fn test_parse_errors() {
    assert_eq!(Triple::parse(""), Err(ParseError::InvalidArch));
    assert_eq!(Triple::parse("x86_64"), Err(ParseError::UnexpectedEnd));
    assert_eq!(Triple::parse("x86_64-"), Err(ParseError::UnexpectedEnd));
    assert_eq!(
        Triple::parse("x86_64-unknown"),
        Err(ParseError::UnexpectedEnd)
    );
    assert_eq!(
        Triple::parse("x86_64-unknown-"),
        Err(ParseError::UnexpectedEnd)
    );
    assert_eq!(
        Triple::parse("x86_64?linux-gnu"),
        Err(ParseError::InvalidArch)
    );
    assert_eq!(
        Triple::parse("x86_64-unknown-linux-gnu-extra"),
        Err(ParseError::InvalidObj)
    );
    assert_eq!(
        Triple::parse("x86_64-unknown-linux-gnu-elf-extra"),
        Err(ParseError::TrailingCharacters)
    );
    assert_eq!(Triple::parse("invalid-arch"), Err(ParseError::InvalidArch));
    assert_eq!(
        Triple::parse("x86_64-invalidvendor"),
        Err(ParseError::InvalidKernel)
    );
    assert_eq!(
        Triple::parse("x86_64-unknown-invalidkernel"),
        Err(ParseError::InvalidKernel)
    );
    assert_eq!(
        Triple::parse("x86_64-unknown-linux-gnu-invalidobj"),
        Err(ParseError::InvalidObj)
    );
}

#[test]
fn test_roundtrip() {
    let original = "x86_64-unknown-linux-gnu";
    let triple = Triple::parse(original).unwrap();
    assert_eq!(triple.to_string(), original);

    let original = "armv7-linux-gnueabihf";
    let triple = Triple::parse(original).unwrap();
    assert_eq!(triple.to_string(), original);

    let original = "aarch64-unknown-linux-gnu-elf";
    let triple = Triple::parse(original).unwrap();
    assert_eq!(triple.to_string(), original);

    let original = "x86_64-linux-gnu";
    let triple = Triple::parse(original).unwrap();
    assert_eq!(triple.to_string(), original);

    let original = "arm-none-eabi";
    let triple = Triple::parse(original).unwrap();
    assert_eq!(triple.to_string(), original);
}

#[test]
fn test_display() {
    let triple = Triple::parse("x86_64-pc-windows-msvc").unwrap();
    assert_eq!(format!("{}", triple), "x86_64-pc-windows-msvc");
}
