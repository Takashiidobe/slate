mod support;

use std::fs;
use std::path::Path;
use support::libc_shim::{Architecture, LibcVariant, TestConfig, compile_test_program};

fn bionic_headers() -> Vec<String> {
    let manifest = Path::new(env!("CARGO_MANIFEST_DIR")).join("libc-shim/bionic-basic-headers.txt");
    fs::read_to_string(manifest)
        .expect("read Bionic header manifest")
        .lines()
        .filter(|line| !line.is_empty())
        .map(str::to_string)
        .collect()
}

fn msvc_headers() -> Vec<String> {
    let manifest = Path::new(env!("CARGO_MANIFEST_DIR")).join("libc-shim/msvc-basic-headers.txt");
    fs::read_to_string(manifest)
        .expect("read MSVC header manifest")
        .lines()
        .filter(|line| !line.is_empty())
        .map(str::to_string)
        .collect()
}

fn macos_headers() -> Vec<String> {
    let manifest = Path::new(env!("CARGO_MANIFEST_DIR")).join("libc-shim/macos-basic-headers.txt");
    fs::read_to_string(manifest)
        .expect("read macOS header manifest")
        .lines()
        .filter(|line| !line.is_empty())
        .map(str::to_string)
        .collect()
}

fn header_program(headers: &[String]) -> String {
    let includes = headers
        .iter()
        .map(|header| format!("#include <{header}>\n"))
        .collect::<String>();
    format!(
        "{includes}\nint main(void) {{ assert(sizeof(wchar_t) == 2); return errno == 0 ? 0 : 0; }}\n"
    )
}

#[test]
fn msvc_basic_header_manifest_compiles() {
    let headers = msvc_headers();
    for forbidden in ["pthread.h", "sys/ioctl.h", "sys/socket.h", "unistd.h"] {
        assert!(!headers.iter().any(|header| header == forbidden));
    }
    let config = TestConfig::new(Architecture::X86_64, LibcVariant::Msvc);
    compile_test_program(&config, &header_program(&headers)).unwrap();
}

#[test]
fn bionic_basic_header_manifest_compiles_for_64_bit_targets() {
    let headers = bionic_headers();
    for arch in [Architecture::Aarch64, Architecture::X86_64] {
        let config = TestConfig::new(arch, LibcVariant::Bionic);
        let includes = headers
            .iter()
            .map(|header| format!("#include <{header}>\n"))
            .collect::<String>();
        let source = format!(
            "{includes}\n_Static_assert(sizeof(wchar_t) == 4, \"wchar_t\");\n_Static_assert(__SLATE_ANDROID_API__ == 21, \"API\");\nint main(void) {{ return 0; }}\n"
        );
        compile_test_program(&config, &source).unwrap();
    }
}

#[test]
fn macos_basic_header_manifest_compiles_for_aarch64() {
    let headers = macos_headers();
    for forbidden in [
        "dirent.h",
        "pthread.h",
        "signal.h",
        "sys/socket.h",
        "sys/stat.h",
        "unistd.h",
    ] {
        assert!(!headers.iter().any(|header| header == forbidden));
    }
    let config = TestConfig::new(Architecture::Aarch64, LibcVariant::Darwin);
    let includes = headers
        .iter()
        .map(|header| format!("#include <{header}>\n"))
        .collect::<String>();
    let source = format!(
        "{includes}\n_Static_assert(sizeof(wchar_t) == 4, \"wchar_t\");\n_Static_assert(sizeof(long double) == 8, \"long double\");\n_Static_assert(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ == 110000, \"deployment\");\nint main(void) {{ return 0; }}\n"
    );
    compile_test_program(&config, &source).unwrap();
}

#[test]
fn msvc_linux_headers_are_rejected() {
    let config = TestConfig::new(Architecture::X86_64, LibcVariant::Msvc);
    for header in ["pthread.h", "sys/ioctl.h", "sys/socket.h", "unistd.h"] {
        let source = format!("#include <{header}>\n");
        let error = compile_test_program(&config, &source).unwrap_err();
        assert!(
            error.contains("is unavailable for the MSVC libc profile"),
            "unexpected diagnostic for {header}:\n{error}"
        );
    }
}
