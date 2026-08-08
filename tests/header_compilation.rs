use std::fs;
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
}

#[derive(Debug, Clone, Copy)]
enum LibcVariant {
    Musl,
    Glibc,
}

impl LibcVariant {
    fn name(&self) -> &'static str {
        match self {
            LibcVariant::Musl => "musl",
            LibcVariant::Glibc => "glibc",
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
        let mut defines = vec![
            "-D_SLATE_LIBC".to_string(),
            format!("-D{}", self.arch.arch_define()),
            "-D__SLATE_LITTLE_ENDIAN".to_string(),
        ];

        if matches!(self.libc, LibcVariant::Glibc) {
            defines.push("-D__SLATE_LIBC_GLIBC".to_string());
        }

        defines
    }
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

        if path.is_file() && path.extension().map_or(false, |ext| ext == "h") {
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

    // Generate unique filename for this test configuration
    let source_file = cache_root.join(format!("test_{}.c", config.name()));
    let object_file = cache_root.join(format!("test_{}.o", config.name()));

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
