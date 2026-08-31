//! Frontend: turn a C source file into ClangIR in MLIR *generic* form, the
//! regular `"op"(operands) <{attrs}> ({regions}) : type` syntax the parser reads.
//!
//! Tool paths default to the local CIR-enabled build and are overridable:
//!   SLATE_CLANG    (default ~/llvm-project/build-cir/bin/clang)
//!   SLATE_CIR_OPT  (default ~/llvm-project/build-cir/bin/cir-opt)
//!   SLATE_LIBC_SHIM (defaults to the repo's libc-shim/include; a directory
//!                    overrides it; an empty value disables the shim and
//!                    falls back to system libc headers. SLATE_CLANG parses
//!                    with -nostdlibinc -isystem <dir> against whichever
//!                    directory is active, while keeping clang's own builtin
//!                    freestanding headers such as stddef.h/stdint.h/stdatomic.h)

use std::collections::BTreeMap;
use std::path::Path;
use std::process::{Command, ExitStatus, Stdio};
use thiserror::Error;
use triplers::{ArchPart, Canonicalizable, Env, Kernel, Triple, Vendor};

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Tool {
    Clang,
}

impl std::fmt::Display for Tool {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::Clang => f.write_str("clang"),
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ToolOperation {
    Preprocess,
    MacroDump,
    EmitCir,
}

impl std::fmt::Display for ToolOperation {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::Preprocess => f.write_str("clang -E"),
            Self::MacroDump => f.write_str("clang -dM -E"),
            Self::EmitCir => f.write_str("clang -emit-cir"),
        }
    }
}

#[derive(Debug, Error)]
pub enum TargetError {
    #[error("invalid target `{target}`: {source}")]
    InvalidTriple {
        target: String,
        #[source]
        source: triplers::ParseError,
    },
    #[error("unsupported Slate target architecture {arch:?}")]
    UnsupportedArchitecture { arch: ArchPart },
    #[error("unsupported Slate target vendor {vendor:?}")]
    UnsupportedVendor { vendor: Vendor },
    #[error("unsupported Slate target kernel {kernel:?}")]
    UnsupportedKernel { kernel: Kernel },
    #[error("Android targets require SLATE_ANDROID_API")]
    MissingAndroidApi {
        #[source]
        source: std::env::VarError,
    },
    #[error("SLATE_ANDROID_API must be an integer, got `{value}`")]
    InvalidAndroidApi {
        value: String,
        #[source]
        source: std::num::ParseIntError,
    },
    #[error("SLATE_ANDROID_API must be at least 21 for the 64-bit Bionic profile, got {api}")]
    AndroidApiTooLow { api: u32 },
    #[error("unsupported SLATE_FREEBSD_RELEASE `{release}`; only {supported} is currently modeled")]
    UnsupportedFreeBsdRelease {
        release: String,
        supported: &'static str,
    },
}

#[derive(Debug, Error)]
pub enum EmitError {
    #[error(transparent)]
    Target(#[from] TargetError),
    #[error(transparent)]
    Normalize(#[from] clang_ir::Error),
    #[error("spawn {command}: {source}")]
    Spawn {
        tool: Tool,
        operation: ToolOperation,
        command: String,
        #[source]
        source: std::io::Error,
    },
    #[error("{operation} failed:\n{stderr}")]
    ToolFailed {
        tool: Tool,
        operation: ToolOperation,
        status: ExitStatus,
        stderr: String,
    },
}

fn home() -> String {
    std::env::var("HOME").expect("HOME not set")
}

pub fn clang() -> String {
    std::env::var("SLATE_CLANG")
        .unwrap_or_else(|_| format!("{}/llvm-project/build-cir/bin/clang", home()))
}

fn cir_opt() -> String {
    std::env::var("SLATE_CIR_OPT")
        .unwrap_or_else(|_| format!("{}/llvm-project/build-cir/bin/cir-opt", home()))
}

pub fn libc_shim_dir() -> Option<String> {
    match std::env::var("SLATE_LIBC_SHIM") {
        Ok(dir) if dir.trim().is_empty() => None,
        Ok(dir) => Some(dir),
        Err(_) => Some(format!("{}/libc-shim/include", env!("CARGO_MANIFEST_DIR"))),
    }
}

fn libc_shim_args(target: &str) -> Vec<String> {
    match libc_shim_dir() {
        Some(dir) => {
            let mut args = vec!["-nostdlibinc".into(), "-isystem".into(), dir];
            let kernel = Triple::parse(target).ok().map(|triple| triple.kernel);
            if !target.ends_with("windows-msvc")
                && !target.ends_with("-android")
                && kernel != Some(Kernel::Darwin)
                && kernel != Some(Kernel::FreeBSD)
            {
                for fallback in system_fallback_include_dirs() {
                    args.push("-idirafter".into());
                    args.push(fallback);
                }
            }
            args
        }
        None => Vec::new(),
    }
}

pub fn clang_resource_dir_include() -> Option<String> {
    static RESOURCE_DIR: std::sync::OnceLock<Option<String>> = std::sync::OnceLock::new();
    RESOURCE_DIR
        .get_or_init(|| {
            let out = Command::new(clang())
                .arg("-print-resource-dir")
                .output()
                .ok()?;
            if !out.status.success() {
                return None;
            }
            let dir = String::from_utf8_lossy(&out.stdout).trim().to_string();
            (!dir.is_empty()).then(|| format!("{dir}/include"))
        })
        .clone()
}

fn system_fallback_include_dirs() -> Vec<String> {
    static DIRS: std::sync::OnceLock<Vec<String>> = std::sync::OnceLock::new();
    DIRS.get_or_init(|| {
        let Ok(out) = Command::new(clang())
            .args(["-E", "-Wp,-v", "-x", "c", "/dev/null"])
            .output()
        else {
            return Vec::new();
        };
        let resource_dir_include = clang_resource_dir_include();
        let stderr = String::from_utf8_lossy(&out.stderr);
        let mut dirs = Vec::new();
        let mut in_list = false;
        for line in stderr.lines() {
            if line.starts_with("#include <...> search starts here") {
                in_list = true;
                continue;
            }
            if !in_list {
                continue;
            }
            if line.starts_with("End of search list") {
                break;
            }
            let dir = line.trim();
            if dir.is_empty() || Some(dir) == resource_dir_include.as_deref() {
                continue;
            }
            dirs.push(dir.to_string());
        }
        dirs
    })
    .clone()
}

#[derive(Clone)]
struct TargetFeatures {
    names: Vec<String>,
}

impl TargetFeatures {
    fn define_args(&self) -> Vec<String> {
        self.names
            .iter()
            .map(|name| format!("-D{name}=1"))
            .collect()
    }

    fn undef_args(&self) -> Vec<String> {
        self.names.iter().map(|name| format!("-U{name}")).collect()
    }
}

fn feature_suffix(value: &str) -> String {
    value
        .chars()
        .map(|c| {
            if c.is_ascii_alphanumeric() {
                c.to_ascii_uppercase()
            } else {
                '_'
            }
        })
        .collect()
}

fn arch_name(arch: ArchPart) -> Result<&'static str, TargetError> {
    match arch {
        ArchPart::X86(_) => Ok("x86"),
        ArchPart::Amd64(_) => Ok("x86_64"),
        ArchPart::Arm(_) => Ok("arm"),
        ArchPart::Aarch64(_) => Ok("aarch64"),
        ArchPart::Riscv(riscv) => Ok(riscv.canonicalize()),
        _ => Err(TargetError::UnsupportedArchitecture { arch }),
    }
}

fn libc_name(triple: &Triple) -> &'static str {
    let env = triple.env.map(|env| env.canonicalize());
    if triple.kernel == Kernel::Win32 {
        return match triple.env {
            Some(Env::MSVC) => "msvc",
            Some(Env::GNU) | Some(Env::LLVM) => "mingw",
            _ => "generic",
        };
    }
    if triple.kernel == Kernel::Linux && env.is_some_and(|env| env.starts_with("gnu")) {
        return "glibc";
    }
    if triple.kernel == Kernel::Darwin && triple.vendor == Some(Vendor::Apple) {
        return "darwin";
    }
    if triple.kernel == Kernel::FreeBSD {
        return "freebsd";
    }
    if env.is_some_and(|env| env.starts_with("musl")) {
        return "musl";
    }
    match triple.env {
        Some(Env::Android) => "bionic",
        Some(Env::Mlibc) => "mlibc",
        _ => "generic",
    }
}

fn vendor_name(vendor: Option<Vendor>) -> Result<&'static str, TargetError> {
    match vendor {
        None | Some(Vendor::Unknown) => Ok("unknown"),
        Some(Vendor::PC) => Ok("pc"),
        Some(Vendor::Apple) => Ok("apple"),
        Some(vendor) => Err(TargetError::UnsupportedVendor { vendor }),
    }
}

fn kernel_name(kernel: Kernel) -> Result<&'static str, TargetError> {
    match kernel {
        Kernel::Linux | Kernel::Win32 | Kernel::Darwin | Kernel::FreeBSD => {
            Ok(kernel.canonicalize())
        }
        _ => Err(TargetError::UnsupportedKernel { kernel }),
    }
}

fn object_name(triple: &Triple) -> &'static str {
    if let Some(obj) = triple.obj {
        return obj.canonicalize();
    }
    match triple.kernel {
        Kernel::Win32 => "coff",
        Kernel::Darwin
        | Kernel::MacOSX
        | Kernel::IOS
        | Kernel::TvOS
        | Kernel::WatchOS
        | Kernel::VisionOS
        | Kernel::XROS => "macho",
        Kernel::WASI | Kernel::WASIp1 | Kernel::WASIp2 | Kernel::WASIp3 => "wasm",
        _ => "elf",
    }
}

fn clang_target(target: &str) -> (&str, &'static [&'static str]) {
    if target == "x86_64-pc-windows-msvc" {
        (
            "x86_64-unknown-uefi",
            &[
                "-fc++-abi=itanium",
                "-fno-ms-extensions",
                "-fwritable-strings",
            ],
        )
    } else if target == "aarch64-apple-darwin" {
        ("arm64-apple-macos11.0", &[])
    } else {
        (target, &[])
    }
}

fn parse_target(target: &str) -> Result<Triple, TargetError> {
    Triple::parse(target).map_err(|source| TargetError::InvalidTriple {
        target: target.to_string(),
        source,
    })
}

fn target_features(target: &str) -> Result<TargetFeatures, TargetError> {
    let triple = parse_target(target)?;
    let arch = arch_name(triple.arch)?;
    let vendor = vendor_name(triple.vendor)?;
    let kernel = kernel_name(triple.kernel)?;
    let libc = libc_name(&triple);
    let obj = object_name(&triple);
    let wordsize = triple.bitness().to_string();
    let endian = "little";
    let values = [
        ("ARCH", arch),
        ("VENDOR", vendor),
        ("KERNEL", kernel),
        ("LIBC", libc),
        ("OBJ", obj),
        ("WORDSIZE", &wordsize),
        ("ENDIAN", endian),
    ];
    let mut names = values
        .into_iter()
        .map(|(family, value)| format!("__SLATE_{family}_{}", feature_suffix(value)))
        .collect::<Vec<_>>();
    if triple.env == Some(Env::Android) {
        names.push("__SLATE_PLATFORM_ANDROID".into());
    }
    if triple.kernel == Kernel::Darwin && triple.vendor == Some(Vendor::Apple) {
        names.push("__SLATE_PLATFORM_MACOS".into());
    }
    if triple.kernel == Kernel::FreeBSD {
        names.push("__SLATE_PLATFORM_FREEBSD".into());
    }
    Ok(TargetFeatures { names })
}

pub fn active_target() -> String {
    std::env::var("SLATE_TARGET")
        .ok()
        .filter(|target| !target.trim().is_empty())
        .unwrap_or_else(|| env!("SLATE_BUILD_TARGET").to_string())
}

pub struct TargetConfig {
    pub arch: &'static str,
    pub endian: &'static str,
    pub env: &'static str,
    pub os: &'static str,
    pub pointer_width: String,
    pub vendor: &'static str,
}

pub fn target_config(target: &str) -> Result<TargetConfig, TargetError> {
    let triple = parse_target(target)?;
    let arch = arch_name(triple.arch)?;
    let vendor = vendor_name(triple.vendor)?;
    let os = if triple.env == Some(Env::Android) {
        "android"
    } else {
        match triple.kernel {
            Kernel::Linux => "linux",
            Kernel::Darwin => "macos",
            Kernel::Win32 => "windows",
            Kernel::FreeBSD => "freebsd",
            kernel => return Err(TargetError::UnsupportedKernel { kernel }),
        }
    };
    let env = match triple.env {
        Some(Env::MSVC) => "msvc",
        Some(env) if env.canonicalize().starts_with("gnu") => "gnu",
        Some(env) if env.canonicalize().starts_with("musl") => "musl",
        _ => "",
    };
    Ok(TargetConfig {
        arch,
        endian: "little",
        env,
        os,
        pointer_width: triple.bitness().to_string(),
        vendor,
    })
}

fn android_api(target: &str) -> Result<Option<u32>, TargetError> {
    let triple = parse_target(target)?;
    if triple.env != Some(Env::Android) {
        return Ok(None);
    }
    let value = std::env::var("SLATE_ANDROID_API")
        .map_err(|source| TargetError::MissingAndroidApi { source })?;
    let api = value
        .parse::<u32>()
        .map_err(|source| TargetError::InvalidAndroidApi {
            value: value.clone(),
            source,
        })?;
    if api < 21 {
        return Err(TargetError::AndroidApiTooLow { api });
    }
    Ok(Some(api))
}

const FREEBSD_SUPPORTED_RELEASE: &str = "15.1";
const FREEBSD_SUPPORTED_VERSION_NUMBER: u32 = 1_501_000;

fn freebsd_release(target: &str) -> Result<Option<(String, u32)>, TargetError> {
    let triple = parse_target(target)?;
    if triple.kernel != Kernel::FreeBSD {
        return Ok(None);
    }
    let release = std::env::var("SLATE_FREEBSD_RELEASE")
        .unwrap_or_else(|_| FREEBSD_SUPPORTED_RELEASE.to_string());
    if release != FREEBSD_SUPPORTED_RELEASE {
        return Err(TargetError::UnsupportedFreeBsdRelease {
            release,
            supported: FREEBSD_SUPPORTED_RELEASE,
        });
    }
    Ok(Some((release, FREEBSD_SUPPORTED_VERSION_NUMBER)))
}

pub fn uses_f64_long_double_abi() -> bool {
    matches!(
        active_target().as_str(),
        "aarch64-apple-darwin" | "x86_64-pc-windows-msvc"
    )
}

pub fn target_has_native_fma(bits: u32) -> bool {
    static MACROS: std::sync::OnceLock<String> = std::sync::OnceLock::new();
    let macros = MACROS.get_or_init(|| {
        let Ok(args) = target_args() else {
            return String::new();
        };
        let Ok(out) = Command::new(clang())
            .args(&args)
            .args(["-dM", "-E", "-x", "c", "/dev/null"])
            .output()
        else {
            return String::new();
        };
        if !out.status.success() {
            return String::new();
        }
        String::from_utf8_lossy(&out.stdout).into_owned()
    });
    if macros.contains("#define __FMA__ ") || macros.contains("#define __ARM_FEATURE_FMA ") {
        return true;
    }
    match bits {
        64 => macros.contains("#define __riscv_d "),
        32 => macros.contains("#define __riscv_f "),
        _ => false,
    }
}

pub fn target_override_args(target: &str) -> Result<Vec<String>, TargetError> {
    let mut args = target_features(&active_target())?.undef_args();
    args.extend(target_features(target)?.define_args());
    let (clang_target, abi_args) = clang_target(target);
    args.push("-target".into());
    args.push(clang_target.into());
    args.extend(abi_args.iter().map(|arg| (*arg).into()));
    Ok(args)
}

pub fn target_args() -> Result<Vec<String>, TargetError> {
    let target = active_target();
    let mut args = libc_shim_args(&target);
    args.extend(target_features(&target)?.define_args());
    let api = android_api(&target)?;
    if let Some(api) = api {
        args.push(format!("-D__SLATE_ANDROID_API__={api}"));
    }
    let freebsd = freebsd_release(&target)?;
    if let Some((_, version_number)) = &freebsd {
        args.push(format!("-D__SLATE_FREEBSD_VERSION__={version_number}"));
    }
    let clang_target_name = if let Some(api) = api {
        format!("{target}{api}")
    } else if let Some((release, _)) = &freebsd {
        format!("{target}{release}")
    } else {
        target.clone()
    };
    let (clang_target, abi_args) = clang_target(&clang_target_name);
    args.push("-target".into());
    args.push(clang_target.into());
    args.extend(abi_args.iter().map(|arg| (*arg).into()));

    if let Ok(extra) = std::env::var("SLATE_CLANG_ARGS") {
        args.extend(extra.split_whitespace().map(str::to_string));
    }
    Ok(args)
}

/// Query the macro environment Clang predefines for `extra_args` (target,
/// `-D`/`-U`, etc.). Used by the preprocessing oracle to decide which
/// conditional branch is active for a given invocation.
pub fn predefined_macros(extra_args: &[String]) -> Result<BTreeMap<String, String>, EmitError> {
    query_macros(Path::new("/dev/null"), extra_args)
}

pub fn preprocess_diagnostics(
    src: &Path,
    extra_args: &[String],
) -> Result<(bool, String), EmitError> {
    let command = clang();
    let out = Command::new(&command)
        .args(["-E", "-x", "c"])
        .args(target_args()?)
        .args(extra_args)
        .arg(src)
        .args(["-o", "/dev/null"])
        .output()
        .map_err(|source| EmitError::Spawn {
            tool: Tool::Clang,
            operation: ToolOperation::Preprocess,
            command,
            source,
        })?;
    Ok((
        out.status.success(),
        String::from_utf8_lossy(&out.stderr).into_owned(),
    ))
}

fn query_macros(src: &Path, extra_args: &[String]) -> Result<BTreeMap<String, String>, EmitError> {
    let command = clang();
    let out = Command::new(&command)
        .args(["-dM", "-E", "-x", "c"])
        .args(target_args()?)
        .args(extra_args)
        .arg(src)
        .output()
        .map_err(|source| EmitError::Spawn {
            tool: Tool::Clang,
            operation: ToolOperation::MacroDump,
            command,
            source,
        })?;
    if !out.status.success() {
        return Err(EmitError::ToolFailed {
            tool: Tool::Clang,
            operation: ToolOperation::MacroDump,
            status: out.status,
            stderr: String::from_utf8_lossy(&out.stderr).into_owned(),
        });
    }
    let mut macros = BTreeMap::new();
    for line in String::from_utf8_lossy(&out.stdout).lines() {
        let mut parts = line.splitn(3, ' ');
        if parts.next() != Some("#define") {
            continue;
        }
        let Some(name) = parts.next() else { continue };
        // function-like macros carry a `(` in the name field; keep the bare name.
        let name = name.split('(').next().unwrap_or(name);
        macros.insert(name.to_string(), parts.next().unwrap_or("").to_string());
    }
    Ok(macros)
}

/// Emit high-level ClangIR (pre-CFG-flattening, passes disabled) for `src` and
/// return it in MLIR generic form.
pub fn emit_generic(src: &Path) -> Result<String, EmitError> {
    emit_generic_with_args(src, &[])
}

pub fn emit_generic_with_args(src: &Path, extra_args: &[String]) -> Result<String, EmitError> {
    emit_generic_with_args_and_cir_opt_flags(src, extra_args, &[])
}

/// Like `emit_generic_with_args`, but also flattens every function in the
/// translation unit into a plain multi-block CFG (`cir.switch.flat`,
/// `cir.brcond`, real `cir.br` edges in place of `cir.goto`/`cir.label`)
/// instead of ClangIR's usual nested structured form. Only meant to be
/// re-run on translation units that need it (see `frontend::cir_input`) — applying
/// this to every function unconditionally would flatten goto-free functions
/// too, which lowers to a correct but far uglier `loop { match state {..} }`
/// dispatch instead of native Rust control flow.
pub fn emit_generic_with_args_flattened(
    src: &Path,
    extra_args: &[String],
) -> Result<String, EmitError> {
    emit_generic_with_args_and_cir_opt_flags(
        src,
        extra_args,
        &[
            "--verify-each=false",
            "--cir-flatten-cfg",
            "--cir-goto-solver",
        ],
    )
}

pub fn emit_generic_with_args_cfg_flattened(
    src: &Path,
    extra_args: &[String],
) -> Result<String, EmitError> {
    emit_generic_with_args_and_cir_opt_flags(
        src,
        extra_args,
        &["--verify-each=false", "--cir-flatten-cfg"],
    )
}

fn emit_generic_with_args_and_cir_opt_flags(
    src: &Path,
    extra_args: &[String],
    cir_opt_flags: &[&str],
) -> Result<String, EmitError> {
    let clang_command = clang();
    let mut cmd = Command::new(&clang_command);
    let target_args = target_args()?;
    cmd.args([
        "-fclangir",
        "-emit-cir",
        "-std=gnu23",
        "-Xclang",
        "-disable-llvm-passes",
        "-o",
        "-",
    ])
    .args(target_args)
    .args(extra_args)
    .arg(src)
    .stderr(Stdio::piped());
    let clang_out = cmd.output().map_err(|source| EmitError::Spawn {
        tool: Tool::Clang,
        operation: ToolOperation::EmitCir,
        command: clang_command,
        source,
    })?;
    if !clang_out.status.success() {
        return Err(EmitError::ToolFailed {
            tool: Tool::Clang,
            operation: ToolOperation::EmitCir,
            status: clang_out.status,
            stderr: String::from_utf8_lossy(&clang_out.stderr).into_owned(),
        });
    }

    Ok(
        clang_ir::Toolchain::with_cir_opt(cir_opt()).normalize_to_generic_with_flags(
            &String::from_utf8_lossy(&clang_out.stdout),
            cir_opt_flags,
        )?,
    )
}
