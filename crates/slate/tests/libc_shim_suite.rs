mod support;

use support::libc_shim::{
    Architecture, LibcVariant, TestConfig, compile_test_program, discover_public_headers,
    header_include_program, libc_shim_dir,
};

fn discover_bits_headers(root: &std::path::Path) -> Vec<std::path::PathBuf> {
    let mut pending = vec![root.to_path_buf()];
    let mut headers = Vec::new();
    while let Some(directory) = pending.pop() {
        for entry in std::fs::read_dir(directory).expect("read bits directory") {
            let path = entry.expect("read bits entry").path();
            if path.is_dir() {
                pending.push(path);
            } else if path.extension().is_some_and(|extension| extension == "h") {
                headers.push(path);
            }
        }
    }
    headers.sort();
    headers
}

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

#[test]
fn bits_headers_reject_direct_inclusion() {
    let bits = libc_shim_dir().join("bits");
    let headers = discover_bits_headers(&bits);
    assert!(!headers.is_empty(), "no bits headers discovered");
    for header in headers {
        let contents = std::fs::read_to_string(&header).expect("read bits header");
        let prefix = contents.lines().take(9).collect::<Vec<_>>().join("\n");
        let include = header.strip_prefix(libc_shim_dir()).unwrap().display();
        assert!(
            prefix.contains("#if !defined(_SLATE_LIBC)"),
            "<{include}> has no direct-include guard in its first nine lines"
        );
        assert!(
            prefix.contains("#error")
                && prefix.contains(&format!("Never include <{include}> directly")),
            "<{include}> has no direct-include error in its first nine lines"
        );
    }
}
