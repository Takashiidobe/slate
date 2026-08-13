use std::fs;
use std::hash::{DefaultHasher, Hash, Hasher};
use std::path::{Path, PathBuf};
use std::process::Command;
use std::sync::OnceLock;

pub fn clang() -> String {
    std::env::var("SLATE_CLANG").unwrap_or_else(|_| {
        PathBuf::from(std::env::var("HOME").unwrap_or_default())
            .join("llvm-project/build-cir/bin/clang")
            .to_string_lossy()
            .into_owned()
    })
}

pub fn libc_shim_dir() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("libc-shim/include")
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Architecture {
    X86_64,
    X86,
    Aarch64,
    Arm,
    Riscv64,
    Riscv32,
}

impl Architecture {
    pub fn arch_define(&self) -> &'static str {
        match self {
            Architecture::X86_64 => "__SLATE_ARCH_X86_64",
            Architecture::X86 => "__SLATE_ARCH_X86",
            Architecture::Aarch64 => "__SLATE_ARCH_AARCH64",
            Architecture::Arm => "__SLATE_ARCH_ARM",
            Architecture::Riscv64 => "__SLATE_ARCH_RISCV64",
            Architecture::Riscv32 => "__SLATE_ARCH_RISCV32",
        }
    }

    pub fn name(&self) -> &'static str {
        match self {
            Architecture::X86_64 => "x86_64",
            Architecture::X86 => "x86",
            Architecture::Aarch64 => "aarch64",
            Architecture::Arm => "arm",
            Architecture::Riscv64 => "riscv64",
            Architecture::Riscv32 => "riscv32",
        }
    }

    pub fn wordsize_define(&self) -> &'static str {
        match self {
            Architecture::X86 | Architecture::Arm | Architecture::Riscv32 => "__SLATE_WORDSIZE_32",
            Architecture::X86_64 | Architecture::Aarch64 | Architecture::Riscv64 => {
                "__SLATE_WORDSIZE_64"
            }
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum LibcVariant {
    Bionic,
    Darwin,
    Musl,
    Glibc,
    Msvc,
}

impl LibcVariant {
    pub fn name(&self) -> &'static str {
        match self {
            LibcVariant::Bionic => "bionic",
            LibcVariant::Darwin => "darwin",
            LibcVariant::Musl => "musl",
            LibcVariant::Glibc => "glibc",
            LibcVariant::Msvc => "msvc",
        }
    }
}

pub struct TestConfig {
    pub arch: Architecture,
    pub libc: LibcVariant,
}

impl TestConfig {
    pub fn new(arch: Architecture, libc: LibcVariant) -> Self {
        TestConfig { arch, libc }
    }

    pub fn name(&self) -> String {
        format!("{}_{}", self.arch.name(), self.libc.name())
    }

    pub fn preprocessor_defines(&self) -> Vec<String> {
        if self.libc == LibcVariant::Darwin {
            return vec![
                "-D_SLATE_LIBC".to_string(),
                "-D__SLATE_ARCH_AARCH64".to_string(),
                "-D__SLATE_VENDOR_APPLE".to_string(),
                "-D__SLATE_KERNEL_DARWIN".to_string(),
                "-D__SLATE_PLATFORM_MACOS".to_string(),
                "-D__SLATE_LIBC_DARWIN".to_string(),
                "-D__SLATE_OBJ_MACHO".to_string(),
                "-D__SLATE_WORDSIZE_64".to_string(),
                "-D__SLATE_ENDIAN_LITTLE".to_string(),
            ];
        }
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

        if self.libc == LibcVariant::Bionic {
            defines.push("-D__SLATE_PLATFORM_ANDROID".to_string());
        }

        defines.push(
            match self.libc {
                LibcVariant::Musl => "-D__SLATE_LIBC_MUSL",
                LibcVariant::Glibc => "-D__SLATE_LIBC_GLIBC",
                LibcVariant::Bionic => "-D__SLATE_LIBC_BIONIC",
                LibcVariant::Darwin => unreachable!(),
                LibcVariant::Msvc => unreachable!(),
            }
            .to_string(),
        );

        if self.libc == LibcVariant::Bionic {
            defines.push("-D__SLATE_ANDROID_API__=21".to_string());
        }

        defines
    }

    pub fn target(&self) -> Option<&'static str> {
        match self.libc {
            LibcVariant::Bionic => Some(match self.arch {
                Architecture::Aarch64 => "aarch64-linux-android21",
                Architecture::X86_64 => "x86_64-linux-android21",
                _ => unreachable!(),
            }),
            LibcVariant::Msvc => Some("x86_64-pc-windows-msvc"),
            LibcVariant::Darwin => Some("arm64-apple-macos11.0"),
            LibcVariant::Musl | LibcVariant::Glibc => None,
        }
    }
}

pub fn discover_public_headers(include_dir: &Path) -> Result<Vec<String>, String> {
    fn walk(dir: &Path, root: &Path, out: &mut Vec<String>) -> Result<(), String> {
        for entry in fs::read_dir(dir).map_err(|e| format!("read {}: {e}", dir.display()))? {
            let path = entry
                .map_err(|e| format!("read entry in {}: {e}", dir.display()))?
                .path();
            if path.is_dir() {
                walk(&path, root, out)?;
                continue;
            }
            if path.extension().and_then(|e| e.to_str()) != Some("h") {
                continue;
            }
            let rel = path
                .strip_prefix(root)
                .expect("walked path under root")
                .to_string_lossy()
                .replace('\\', "/");
            if rel.starts_with("bits/") {
                continue;
            }
            let content =
                fs::read_to_string(&path).map_err(|e| format!("read {}: {e}", path.display()))?;
            if content.trim_start().starts_with("#error") {
                continue;
            }
            out.push(rel);
        }
        Ok(())
    }

    let mut headers = Vec::new();
    walk(include_dir, include_dir, &mut headers)?;
    headers.sort();
    Ok(headers)
}

pub fn header_include_program(headers: &[String]) -> String {
    let includes = headers
        .iter()
        .map(|header| format!("#include <{header}>\n"))
        .collect::<String>();
    format!("{includes}\nint main(void) {{ return 0; }}\n")
}

pub fn compile_test_program(config: &TestConfig, source: &str) -> Result<(), String> {
    compile_test_program_with_args(config, source, &[])
}

pub fn compile_test_program_with_args(
    config: &TestConfig,
    source: &str,
    extra_args: &[&str],
) -> Result<(), String> {
    let cache_root = super::test_cache_root();
    fs::create_dir_all(&cache_root).map_err(|e| format!("create cache dir: {e}"))?;

    let mut hasher = DefaultHasher::new();
    source.hash(&mut hasher);
    extra_args.hash(&mut hasher);
    libc_shim_fingerprint().hash(&mut hasher);
    let key = hasher.finish();
    let source_file = cache_root.join(format!("test_{}_{key:016x}.c", config.name()));
    let object_file = cache_root.join(format!("test_{}_{key:016x}.o", config.name()));

    if object_file.is_file() {
        return Ok(());
    }

    fs::write(&source_file, source).map_err(|e| format!("write source: {e}"))?;

    let temporary = object_file.with_extension(format!("o.tmp-{}", std::process::id()));
    let _ = fs::remove_file(&temporary);

    let mut cmd = Command::new(clang());
    cmd.arg("-xc")
        .arg("-c")
        .arg("-o")
        .arg(&temporary)
        .arg("-nostdlibinc")
        .arg("-isystem")
        .arg(libc_shim_dir().to_string_lossy().as_ref())
        .args(extra_args);

    if let Some(target) = config.target() {
        cmd.arg(format!("--target={target}"));
    }

    for define in config.preprocessor_defines() {
        cmd.arg(&define);
    }

    cmd.arg(&source_file);

    let output = cmd.output().map_err(|e| format!("execute clang: {e}"))?;

    if !output.status.success() {
        let _ = fs::remove_file(&temporary);
        let stderr = String::from_utf8_lossy(&output.stderr);
        let stdout = String::from_utf8_lossy(&output.stdout);
        return Err(format!(
            "compilation failed for {}:\nstderr:\n{stderr}\nstdout:\n{stdout}",
            config.name(),
        ));
    }

    fs::rename(&temporary, &object_file).map_err(|e| {
        format!(
            "rename {} to {}: {e}",
            temporary.display(),
            object_file.display()
        )
    })?;

    Ok(())
}

fn libc_shim_fingerprint() -> u64 {
    static FINGERPRINT: OnceLock<u64> = OnceLock::new();
    *FINGERPRINT.get_or_init(|| {
        fn hash_tree(dir: &Path, hasher: &mut DefaultHasher) {
            let mut paths: Vec<PathBuf> = fs::read_dir(dir)
                .into_iter()
                .flatten()
                .filter_map(Result::ok)
                .map(|entry| entry.path())
                .collect();
            paths.sort();
            for path in paths {
                path.hash(hasher);
                if path.is_dir() {
                    hash_tree(&path, hasher);
                } else if let Ok(contents) = fs::read(&path) {
                    contents.hash(hasher);
                }
            }
        }

        let mut hasher = DefaultHasher::new();
        hash_tree(&libc_shim_dir(), &mut hasher);
        hasher.finish()
    })
}
