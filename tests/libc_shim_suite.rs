mod support;

use support::libc_shim::{
    Architecture, LibcVariant, TestConfig, compile_test_program, discover_public_headers,
    header_include_program, libc_shim_dir,
};

fn compile_all_headers(arch: Architecture, libc: LibcVariant) {
    let config = TestConfig::new(arch, libc);
    let headers = discover_public_headers(&libc_shim_dir())
        .unwrap_or_else(|e| panic!("discover headers for {}: {e}", config.name()));
    assert!(!headers.is_empty(), "no public headers discovered");
    let failures = headers
        .iter()
        .filter_map(|header| {
            let program = header_include_program(std::slice::from_ref(header));
            compile_test_program(&config, &program)
                .err()
                .map(|error| format!("{header}:\n{error}"))
        })
        .collect::<Vec<_>>();
    assert!(
        failures.is_empty(),
        "{} public headers failed to compile:\n{}",
        config.name(),
        failures.join("\n\n")
    );
}

#[test]
fn linux_x86_64_headers_compile() {
    compile_all_headers(Architecture::X86_64, LibcVariant::Glibc);
}

#[test]
fn linux_x86_headers_compile() {
    compile_all_headers(Architecture::X86, LibcVariant::Glibc);
}

#[test]
fn linux_aarch64_headers_compile() {
    compile_all_headers(Architecture::Aarch64, LibcVariant::Glibc);
}

#[test]
fn linux_arm_headers_compile() {
    compile_all_headers(Architecture::Arm, LibcVariant::Glibc);
}

#[test]
fn linux_riscv64_headers_compile() {
    compile_all_headers(Architecture::Riscv64, LibcVariant::Glibc);
}

#[test]
fn linux_riscv32_headers_compile() {
    compile_all_headers(Architecture::Riscv32, LibcVariant::Glibc);
}
