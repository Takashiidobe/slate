use std::fs;
use std::hash::{DefaultHasher, Hash, Hasher};
use std::path::{Path, PathBuf};
use std::process::Command;

fn clang() -> String {
    std::env::var("SLATE_CLANG").unwrap_or_else(|_| {
        std::path::PathBuf::from(std::env::var("HOME").unwrap_or_default())
            .join("llvm-project/build-cir/bin/clang")
            .to_string_lossy()
            .into_owned()
    })
}

fn libc_shim_dir() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("libc-shim/include")
}

fn test_cache_root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("target/test-cache")
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum Architecture {
    X86_64,
    X86,
    Aarch64,
    Arm,
    Riscv64,
    Riscv32,
}

impl Architecture {
    fn arch_define(&self) -> &'static str {
        match self {
            Architecture::X86_64 => "__SLATE_ARCH_X86_64",
            Architecture::X86 => "__SLATE_ARCH_X86",
            Architecture::Aarch64 => "__SLATE_ARCH_AARCH64",
            Architecture::Arm => "__SLATE_ARCH_ARM",
            Architecture::Riscv64 => "__SLATE_ARCH_RISCV64",
            Architecture::Riscv32 => "__SLATE_ARCH_RISCV32",
        }
    }

    fn name(&self) -> &'static str {
        match self {
            Architecture::X86_64 => "x86_64",
            Architecture::X86 => "x86",
            Architecture::Aarch64 => "aarch64",
            Architecture::Arm => "arm",
            Architecture::Riscv64 => "riscv64",
            Architecture::Riscv32 => "riscv32",
        }
    }

    fn wordsize_define(&self) -> &'static str {
        match self {
            Architecture::X86 | Architecture::Arm | Architecture::Riscv32 => "__SLATE_WORDSIZE_32",
            Architecture::X86_64 | Architecture::Aarch64 | Architecture::Riscv64 => {
                "__SLATE_WORDSIZE_64"
            }
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum LibcVariant {
    Musl,
    Glibc,
    Msvc,
}

impl LibcVariant {
    fn name(&self) -> &'static str {
        match self {
            LibcVariant::Musl => "musl",
            LibcVariant::Glibc => "glibc",
            LibcVariant::Msvc => "msvc",
        }
    }
}

struct TestConfig {
    arch: Architecture,
    libc: LibcVariant,
}

impl TestConfig {
    fn new(arch: Architecture, libc: LibcVariant) -> Self {
        TestConfig { arch, libc }
    }

    fn name(&self) -> String {
        format!("{}_{}", self.arch.name(), self.libc.name())
    }

    fn preprocessor_defines(&self) -> Vec<String> {
        if self.libc == LibcVariant::Msvc {
            return vec![
                "-D_SLATE_LIBC".to_string(),
                "-D__SLATE_ARCH_X86_64".to_string(),
                "-D__SLATE_VENDOR_PC".to_string(),
                "-D__SLATE_KERNEL_WINDOWS".to_string(),
                "-D__SLATE_LIBC_MSVC".to_string(),
                "-D__SLATE_OBJ_COFF".to_string(),
                "-D__SLATE_WORDSIZE_64".to_string(),
                "-D__SLATE_ENDIAN_LITTLE".to_string(),
            ];
        }
        let mut defines = vec![
            "-D_SLATE_LIBC".to_string(),
            format!("-D{}", self.arch.arch_define()),
            "-D__SLATE_VENDOR_UNKNOWN".to_string(),
            "-D__SLATE_KERNEL_LINUX".to_string(),
            "-D__SLATE_OBJ_ELF".to_string(),
            format!("-D{}", self.arch.wordsize_define()),
            "-D__SLATE_ENDIAN_LITTLE".to_string(),
        ];

        defines.push(
            match self.libc {
                LibcVariant::Musl => "-D__SLATE_LIBC_MUSL",
                LibcVariant::Glibc => "-D__SLATE_LIBC_GLIBC",
                LibcVariant::Msvc => unreachable!(),
            }
            .to_string(),
        );

        defines
    }

    fn target(&self) -> Option<&'static str> {
        match self.libc {
            LibcVariant::Msvc => Some("x86_64-pc-windows-msvc"),
            LibcVariant::Musl | LibcVariant::Glibc => None,
        }
    }
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

fn header_program(headers: &[String]) -> String {
    let includes = headers
        .iter()
        .map(|header| format!("#include <{header}>\n"))
        .collect::<String>();
    format!(
        "{includes}\nint main(void) {{ assert(sizeof(wchar_t) == 2); return errno == 0 ? 0 : 0; }}\n"
    )
}

fn generate_header_test_program() -> Result<String, String> {
    let include_dir = libc_shim_dir();

    let mut headers = Vec::new();

    // Collect all public headers (everything except bits/ subdirectory and types.h)
    for entry in
        fs::read_dir(&include_dir).map_err(|e| format!("Failed to read include dir: {}", e))?
    {
        let entry = entry.map_err(|e| format!("Failed to read dir entry: {}", e))?;
        let path = entry.path();

        if path.is_file() && path.extension().is_some_and(|ext| ext == "h") {
            let name = path
                .file_name()
                .and_then(|n| n.to_str())
                .ok_or("Invalid header name")?
                .to_string();

            // Skip internal implementation details
            if name == "types.h" || name == "glibc.h" {
                continue;
            }

            // Skip headers that explicitly aren't implemented yet
            let content =
                fs::read_to_string(&path).map_err(|e| format!("Failed to read {}: {}", name, e))?;

            if content.contains("#error")
                && (content.contains("not yet defined")
                    || content.contains("not ported")
                    || content.contains("is not supported")
                    || content.contains("not implemented"))
            {
                continue;
            }

            headers.push(name);
        }
    }

    headers.sort();

    // Generate includes
    let mut includes = String::new();
    for header in headers {
        includes.push_str(&format!("#include <{}>\n", header));
    }

    let program = format!(
        r#"/* Test program to verify all supported libc-shim public headers compile */
#define _SLATE_LIBC

{includes}

int main(void) {{
    return 0;
}}
"#
    );

    Ok(program)
}

/// Compile a header test program with specific flags
fn compile_test_program(config: &TestConfig, source: &str) -> Result<(), String> {
    let cache_root = test_cache_root();
    fs::create_dir_all(&cache_root).map_err(|e| format!("Failed to create cache dir: {}", e))?;

    let mut hasher = DefaultHasher::new();
    source.hash(&mut hasher);
    let key = hasher.finish();
    let source_file = cache_root.join(format!("test_{}_{key:016x}.c", config.name()));
    let object_file = cache_root.join(format!("test_{}_{key:016x}.o", config.name()));

    // Write source file
    fs::write(&source_file, source).map_err(|e| format!("Failed to write source: {}", e))?;

    // Build clang invocation
    let mut cmd = Command::new(clang());
    cmd.arg("-xc")
        .arg("-c") // Compile but don't link
        .arg("-o")
        .arg(&object_file)
        .arg("-I")
        .arg(libc_shim_dir().to_string_lossy().as_ref());

    if let Some(target) = config.target() {
        cmd.arg(format!("--target={target}"));
    }

    // Add preprocessor defines
    for define in config.preprocessor_defines() {
        cmd.arg(&define);
    }

    // Add the source file
    cmd.arg(&source_file);

    // Run compilation
    let output = cmd
        .output()
        .map_err(|e| format!("Failed to execute clang: {}", e))?;

    if !output.status.success() {
        let stderr = String::from_utf8_lossy(&output.stderr);
        let stdout = String::from_utf8_lossy(&output.stdout);
        return Err(format!(
            "Compilation failed for {}:\nStderr:\n{}\nStdout:\n{}",
            config.name(),
            stderr,
            stdout
        ));
    }

    Ok(())
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

#[test]
fn msvc_scalar_layout_matches_xwin() {
    let root = Path::new(env!("CARGO_MANIFEST_DIR"));
    let sysroot = root.join("target/msvc-sysroot");
    let crt = sysroot.join("crt/include");
    let ucrt = sysroot.join("sdk/include/ucrt");
    if !crt.is_dir() || !ucrt.is_dir() {
        return;
    }

    let source = r#"
#include <float.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
_Static_assert(sizeof(size_t) == 8, "size_t");
_Static_assert(sizeof(ptrdiff_t) == 8, "ptrdiff_t");
_Static_assert(sizeof(intptr_t) == 8, "intptr_t");
_Static_assert(sizeof(uintptr_t) == 8, "uintptr_t");
_Static_assert(sizeof(wchar_t) == 2, "wchar_t");
_Static_assert(sizeof(long) == 4, "long");
_Static_assert(sizeof(long long) == 8, "long long");
_Static_assert(sizeof(long double) == 8, "long double");
_Static_assert(LONG_MAX == 2147483647L, "LONG_MAX");
_Static_assert(WCHAR_MAX == 65535, "WCHAR_MAX");
_Static_assert(LDBL_MANT_DIG == 53, "LDBL_MANT_DIG");
int probe(void) { return 0; }
"#;

    let config = TestConfig::new(Architecture::X86_64, LibcVariant::Msvc);
    compile_test_program(&config, source).unwrap();

    let cache_root = test_cache_root();
    fs::create_dir_all(&cache_root).unwrap();
    let source_file = cache_root.join("test_xwin_scalar_layout.c");
    let object_file = cache_root.join("test_xwin_scalar_layout.o");
    fs::write(&source_file, source).unwrap();
    let output = Command::new(clang())
        .args([
            "-xc",
            "-c",
            "--target=x86_64-pc-windows-msvc",
            "-nostdlibinc",
        ])
        .arg("-isystem")
        .arg(&crt)
        .arg("-isystem")
        .arg(&ucrt)
        .arg("-o")
        .arg(object_file)
        .arg(source_file)
        .output()
        .unwrap();
    assert!(
        output.status.success(),
        "xwin scalar probe failed:\n{}",
        String::from_utf8_lossy(&output.stderr)
    );
}

#[test]
#[ignore = "This isn't compiling yet, have to fill out libc-shim before it does"]
fn test_header_compilation() {
    let architectures = vec![
        Architecture::X86_64,
        Architecture::X86,
        Architecture::Aarch64,
        Architecture::Arm,
        Architecture::Riscv64,
        Architecture::Riscv32,
    ];

    let libc_variants = vec![LibcVariant::Musl, LibcVariant::Glibc];

    let test_program = match generate_header_test_program() {
        Ok(prog) => prog,
        Err(e) => panic!("Failed to generate test program: {}", e),
    };

    let mut failures = Vec::new();
    let mut successes = Vec::new();

    for arch in &architectures {
        for libc in &libc_variants {
            let config = TestConfig::new(*arch, *libc);
            match compile_test_program(&config, &test_program) {
                Ok(()) => {
                    successes.push(config.name());
                }
                Err(e) => {
                    failures.push((config.name(), e));
                }
            }
        }
    }

    // Print summary
    eprintln!(
        "\n=== Header Compilation Test Results ===\n\
         Tested {} arch + libc combinations\n\
         ✓ {} passed\n\
         ✗ {} failed",
        architectures.len() * libc_variants.len(),
        successes.len(),
        failures.len()
    );

    if !failures.is_empty() {
        eprintln!("\n=== Failures ===");
        for (config, error) in &failures {
            eprintln!("\n❌ {}", config);
            eprintln!("{}", error);
        }
        eprintln!("\n=== End Failures ===\n");

        panic!(
            "Header compilation test failed for {} configurations",
            failures.len()
        );
    } else {
        eprintln!(
            "\n✓ All {} configurations compiled successfully\n",
            successes.len()
        );
    }
}
