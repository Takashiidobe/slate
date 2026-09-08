use std::collections::{BTreeMap, BTreeSet};
use std::path::{Path, PathBuf};
use std::process::Command;

use super::libc_shim::{Architecture, LibcVariant, TestConfig, clang, libc_shim_dir};

const PROBE_SOURCE: &str = "tests/fixtures.abi-probe/linux-libc-abi.c";

pub const ARCHES: [Architecture; 4] = [
    Architecture::X86_64,
    Architecture::X86,
    Architecture::Arm,
    Architecture::Aarch64,
];

pub const ABI_FAMILIES: [&str; 5] = [
    "pthread",
    "setjmp-ucontext",
    "socket-epoll",
    "sched",
    "stat-time",
];

pub fn in_family(name: &str, family: &str) -> bool {
    match family {
        "pthread" => name.contains("pthread"),
        "setjmp-ucontext" => {
            matches!(name, "size:jmp_buf" | "size:sigjmp_buf" | "size:ucontext_t")
        }
        "socket-epoll" => ["struct_msghdr", "struct_cmsghdr", "struct_epoll_event"]
            .iter()
            .any(|needle| name.contains(needle)),
        "sched" => name == "size:struct_sched_param",
        "stat-time" => {
            name.contains("struct_stat")
                || name.contains("struct_timespec")
                || name.contains("struct_timeval")
                || name.ends_with(":time_t")
        }
        _ => true,
    }
}

pub fn arch_key(arch: Architecture) -> &'static str {
    match arch {
        Architecture::X86_64 => "x86_64",
        Architecture::X86 => "i386",
        Architecture::Arm => "arm",
        Architecture::Aarch64 => "aarch64",
        _ => unreachable!("libc ABI probes only cover x86_64/i386/arm/aarch64"),
    }
}

pub fn arch_from_key(key: &str) -> Option<Architecture> {
    ARCHES.into_iter().find(|arch| arch_key(*arch) == key)
}

fn home() -> PathBuf {
    PathBuf::from(std::env::var("HOME").unwrap_or_default())
}

fn probe_source() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join(PROBE_SOURCE)
}

fn require_dir(path: &Path, description: &str) -> Result<PathBuf, String> {
    if path.is_dir() {
        Ok(path.to_path_buf())
    } else {
        Err(format!("{description} is missing: {}", path.display()))
    }
}

fn require_file(path: &Path, description: &str) -> Result<PathBuf, String> {
    if path.is_file() {
        Ok(path.to_path_buf())
    } else {
        Err(format!("{description} is missing: {}", path.display()))
    }
}

fn is_executable(path: &Path) -> bool {
    use std::os::unix::fs::PermissionsExt;
    path.is_file()
        && std::fs::metadata(path)
            .map(|meta| meta.permissions().mode() & 0o111 != 0)
            .unwrap_or(false)
}

fn find_on_path(name: &str) -> Option<PathBuf> {
    let path_var = std::env::var("PATH").ok()?;
    std::env::split_paths(&path_var)
        .map(|dir| dir.join(name))
        .find(|candidate| is_executable(candidate))
}

fn require_executable(value: &str, description: &str) -> Result<PathBuf, String> {
    let path = PathBuf::from(value);
    if is_executable(&path) {
        return Ok(path);
    }
    find_on_path(value).ok_or_else(|| format!("{description} is missing: {value}"))
}

fn musl_target_triple(arch: Architecture) -> &'static str {
    match arch {
        Architecture::X86_64 => "x86_64-linux-musl",
        Architecture::X86 => "i686-linux-musl",
        Architecture::Arm => "arm-linux-musleabihf",
        Architecture::Aarch64 => "aarch64-linux-musl",
        _ => unreachable!("libc ABI probes only cover x86_64/i386/arm/aarch64"),
    }
}

fn glibc_target_triple(arch: Architecture) -> &'static str {
    match arch {
        Architecture::X86_64 => "x86_64-linux-gnu",
        Architecture::X86 => "i686-linux-gnu",
        Architecture::Arm => "armv7-linux-gnueabihf",
        Architecture::Aarch64 => "aarch64-linux-gnu",
        _ => unreachable!("libc ABI probes only cover x86_64/i386/arm/aarch64"),
    }
}

fn extra_compiler_args(arch: Architecture) -> Vec<String> {
    match arch {
        Architecture::X86 => vec!["-m32".to_string()],
        _ => Vec::new(),
    }
}

fn musl_linker_name(arch: Architecture) -> &'static str {
    match arch {
        Architecture::X86_64 | Architecture::X86 => "musl-clang",
        Architecture::Arm | Architecture::Aarch64 => "musl-gcc",
        _ => unreachable!("libc ABI probes only cover x86_64/i386/arm/aarch64"),
    }
}

fn qemu_command(arch: Architecture) -> Option<&'static str> {
    match arch {
        Architecture::Arm => Some("qemu-arm-static"),
        Architecture::Aarch64 => Some("qemu-aarch64-static"),
        _ => None,
    }
}

fn local_glibc_sysroot(arch: Architecture) -> PathBuf {
    match arch {
        Architecture::X86_64 | Architecture::X86 => PathBuf::from("/"),
        Architecture::Arm => home()
            .join("toolchains/arm-gnu-toolchain-15.2.rel1-x86_64-arm-none-linux-gnueabihf")
            .join("arm-none-linux-gnueabihf/libc"),
        Architecture::Aarch64 => PathBuf::from("/usr/aarch64-linux-gnu"),
        _ => unreachable!("libc ABI probes only cover x86_64/i386/arm/aarch64"),
    }
}

fn local_glibc_linker(arch: Architecture) -> Option<PathBuf> {
    match arch {
        Architecture::X86_64 => None,
        Architecture::X86 => Some(PathBuf::from("clang")),
        Architecture::Arm => Some(
            home()
                .join("toolchains/arm-gnu-toolchain-15.2.rel1-x86_64-arm-none-linux-gnueabihf")
                .join("bin/arm-none-linux-gnueabihf-gcc"),
        ),
        Architecture::Aarch64 => Some(PathBuf::from("/usr/bin/aarch64-linux-gnu-gcc")),
        _ => unreachable!("libc ABI probes only cover x86_64/i386/arm/aarch64"),
    }
}

fn glibc_sysroot_env_names(arch: Architecture) -> &'static [&'static str] {
    match arch {
        Architecture::X86_64 => &["SLATE_GLIBC_SYSROOT_X86_64"],
        Architecture::X86 => &["SLATE_GLIBC_SYSROOT_X86", "SLATE_I686_SYSROOT"],
        Architecture::Arm => &["SLATE_GLIBC_SYSROOT_ARM", "SLATE_ARM_SYSROOT"],
        Architecture::Aarch64 => &["SLATE_GLIBC_SYSROOT_AARCH64", "SLATE_AARCH64_SYSROOT"],
        _ => unreachable!("libc ABI probes only cover x86_64/i386/arm/aarch64"),
    }
}

fn glibc_linker_env_names(arch: Architecture) -> &'static [&'static str] {
    match arch {
        Architecture::X86_64 => &["SLATE_GLIBC_LINKER_X86_64"],
        Architecture::X86 => &["SLATE_GLIBC_LINKER_X86", "SLATE_I686_LINKER"],
        Architecture::Arm => &["SLATE_GLIBC_LINKER_ARM", "SLATE_ARM_LINKER"],
        Architecture::Aarch64 => &["SLATE_GLIBC_LINKER_AARCH64", "SLATE_AARCH64_LINKER"],
        _ => unreachable!("libc ABI probes only cover x86_64/i386/arm/aarch64"),
    }
}

fn first_env(names: &[&str]) -> Option<String> {
    names.iter().find_map(|name| std::env::var(name).ok())
}

fn musl_root() -> PathBuf {
    std::env::var("SLATE_MUSL_SYSROOT_ROOT")
        .map(PathBuf::from)
        .unwrap_or_else(|_| home().join("toolchains/slate-musl"))
}

fn qemu_for(arch: Architecture) -> Result<Option<PathBuf>, String> {
    let override_name = format!("SLATE_LIBC_QEMU_{}", arch_key(arch).to_uppercase());
    if let Ok(value) = std::env::var(&override_name) {
        return require_file(Path::new(&value), &format!("QEMU for {}", arch_key(arch))).map(Some);
    }
    let Some(command) = qemu_command(arch) else {
        return Ok(None);
    };
    find_on_path(command).map(Some).ok_or_else(|| {
        format!(
            "QEMU for {} is unavailable; set {override_name}",
            arch_key(arch)
        )
    })
}

pub struct ProbeConfig {
    pub label: String,
    pub compiler: PathBuf,
    pub compiler_args: Vec<String>,
    pub target: String,
    pub sysroot: PathBuf,
    pub linker: PathBuf,
    pub linker_args: Vec<String>,
    pub linker_post_args: Vec<String>,
    pub runner: Option<PathBuf>,
    pub runner_args: Vec<String>,
    pub defines: Vec<String>,
}

pub fn resolve(arch: Architecture, libc: LibcVariant) -> Result<ProbeConfig, String> {
    let slate_clang = require_file(Path::new(&clang()), "SLATE_CLANG")?;
    let defines = TestConfig::new(arch, libc).preprocessor_defines();

    let (target, sysroot, compiler, linker, linker_args, linker_post_args) = match libc {
        LibcVariant::Musl => {
            let root = musl_root();
            let sysroot = require_dir(
                &root.join(arch_key(arch)),
                &format!("musl {} sysroot", arch_key(arch)),
            )?;
            if arch == Architecture::Aarch64 {
                let linker_value = std::env::var("SLATE_MUSL_AARCH64_LINKER")
                    .unwrap_or_else(|_| "aarch64-linux-gnu-gcc".to_string());
                let linker = require_executable(&linker_value, "AArch64 musl linker")?;
                let linker_args = vec![
                    "-static".to_string(),
                    "-nostdlib".to_string(),
                    "-nostartfiles".to_string(),
                    "-nodefaultlibs".to_string(),
                    sysroot.join("lib/crt1.o").to_string_lossy().into_owned(),
                    sysroot.join("lib/crti.o").to_string_lossy().into_owned(),
                    format!("-L{}", sysroot.join("lib").display()),
                ];
                let linker_post_args = vec![
                    "-Wl,--start-group".to_string(),
                    "-lc".to_string(),
                    "-lgcc".to_string(),
                    "-lgcc_eh".to_string(),
                    "-Wl,--end-group".to_string(),
                    sysroot.join("lib/crtn.o").to_string_lossy().into_owned(),
                ];
                (
                    musl_target_triple(arch).to_string(),
                    sysroot,
                    slate_clang.clone(),
                    linker,
                    linker_args,
                    linker_post_args,
                )
            } else {
                let linker = require_file(
                    &sysroot.join("bin").join(musl_linker_name(arch)),
                    &format!("musl {} linker", arch_key(arch)),
                )?;
                let linker_args = vec!["-static".to_string()];
                let linker_post_args = vec![
                    "-Wl,--start-group".to_string(),
                    "-lc".to_string(),
                    "-lgcc".to_string(),
                    "-Wl,--end-group".to_string(),
                ];
                (
                    musl_target_triple(arch).to_string(),
                    sysroot,
                    slate_clang.clone(),
                    linker,
                    linker_args,
                    linker_post_args,
                )
            }
        }
        LibcVariant::Glibc => {
            let sysroot_names = glibc_sysroot_env_names(arch);
            let sysroot_value = first_env(sysroot_names)
                .map(PathBuf::from)
                .unwrap_or_else(|| local_glibc_sysroot(arch));
            let sysroot = require_dir(
                &sysroot_value,
                &format!(
                    "glibc {} sysroot; set {}",
                    arch_key(arch),
                    sysroot_names.join(" or ")
                ),
            )?;
            let target_triple = glibc_target_triple(arch).to_string();
            let linker_names = glibc_linker_env_names(arch);
            let linker_env = first_env(linker_names)
                .map(PathBuf::from)
                .or_else(|| local_glibc_linker(arch));
            let (linker, mut linker_args, linker_post_args) = if let Some(linker_value) = linker_env
            {
                let linker = require_executable(
                    &linker_value.to_string_lossy(),
                    &format!("glibc {} linker", arch_key(arch)),
                )?;
                (
                    linker,
                    vec![format!("--sysroot={}", sysroot.display())],
                    Vec::new(),
                )
            } else if arch == Architecture::X86_64 {
                (
                    slate_clang.clone(),
                    vec![
                        format!("--target={target_triple}"),
                        format!("--sysroot={}", sysroot.display()),
                    ],
                    Vec::new(),
                )
            } else {
                return Err(format!(
                    "glibc {} linker is not configured; set {}",
                    arch_key(arch),
                    linker_names.join(" or ")
                ));
            };
            if linker.file_name().and_then(|name| name.to_str()) == Some("clang") {
                if !linker_args.iter().any(|arg| arg.starts_with("--target=")) {
                    linker_args.insert(0, format!("--target={target_triple}"));
                }
                if !linker_args.iter().any(|arg| arg.starts_with("--sysroot=")) {
                    linker_args.insert(1, format!("--sysroot={}", sysroot.display()));
                }
            }
            (
                target_triple,
                sysroot,
                slate_clang.clone(),
                linker,
                linker_args,
                linker_post_args,
            )
        }
        LibcVariant::Bionic | LibcVariant::Darwin | LibcVariant::FreeBsd | LibcVariant::Msvc => {
            unreachable!("libc ABI probes only cover musl and glibc")
        }
    };

    let runner = qemu_for(arch)?;
    let runner_args = runner
        .as_ref()
        .map(|_| vec!["-L".to_string(), sysroot.to_string_lossy().into_owned()])
        .unwrap_or_default();

    Ok(ProbeConfig {
        label: format!("{}/{}", libc.name(), arch_key(arch)),
        compiler,
        compiler_args: extra_compiler_args(arch),
        target,
        sysroot,
        linker,
        linker_args,
        linker_post_args,
        runner,
        runner_args,
        defines,
    })
}

fn run_command(mut command: Command, label: &str) -> Result<String, String> {
    let output = command
        .output()
        .map_err(|error| format!("{label}: {error}"))?;
    if !output.status.success() {
        return Err(format!(
            "{label} failed ({}):\nstdout:\n{}\nstderr:\n{}",
            output.status,
            String::from_utf8_lossy(&output.stdout),
            String::from_utf8_lossy(&output.stderr),
        ));
    }
    Ok(String::from_utf8_lossy(&output.stdout).into_owned())
}

fn parse_probe_output(output: &str) -> Result<BTreeMap<String, i64>, String> {
    let mut records = BTreeMap::new();
    for (line_number, line) in output.lines().enumerate() {
        let fields: Vec<&str> = line.split('\t').collect();
        if fields.len() != 3 {
            return Err(format!(
                "line {}: expected three tab-separated fields",
                line_number + 1
            ));
        }
        let key = format!("{}:{}", fields[0], fields[1]);
        if records.contains_key(&key) {
            return Err(format!("line {}: duplicate record {key}", line_number + 1));
        }
        let value: i64 = fields[2].parse().map_err(|_| {
            format!(
                "line {}: non-numeric value {:?}",
                line_number + 1,
                fields[2]
            )
        })?;
        records.insert(key, value);
    }
    match records.remove("schema:version") {
        Some(1) => {}
        _ => return Err("missing or unsupported probe schema".to_string()),
    }
    Ok(records)
}

pub fn run_probe(config: &ProbeConfig, shim: bool) -> Result<BTreeMap<String, i64>, String> {
    let work_dir = Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("target/libc-abi-probe")
        .join(format!(
            "{}-{}",
            config.label.replace('/', "-"),
            if shim { "shim" } else { "oracle" }
        ));
    std::fs::create_dir_all(&work_dir)
        .map_err(|error| format!("create {}: {error}", work_dir.display()))?;

    let object = work_dir.join("probe.o");
    let executable = work_dir.join("probe");

    let mut compile = Command::new(&config.compiler);
    compile.args(&config.compiler_args);
    compile.arg(format!("--target={}", config.target));
    compile.arg(format!("--sysroot={}", config.sysroot.display()));
    compile.args(["-std=gnu23", "-O0"]);
    if shim {
        compile
            .arg("-nostdlibinc")
            .arg("-isystem")
            .arg(libc_shim_dir());
        compile.arg("-D__SLATE_LIBC_SHIM");
    }
    compile.args(&config.defines);
    compile.arg("-c").arg(probe_source()).arg("-o").arg(&object);
    run_command(compile, &format!("compile probe ({})", config.label))?;

    let mut link = Command::new(&config.linker);
    link.args(&config.linker_args);
    link.arg(&object);
    link.args(&config.linker_post_args);
    link.arg("-o").arg(&executable);
    run_command(link, &format!("link probe ({})", config.label))?;

    let run = if let Some(runner) = &config.runner {
        let mut cmd = Command::new(runner);
        cmd.args(&config.runner_args);
        cmd.arg(&executable);
        cmd
    } else {
        Command::new(&executable)
    };
    let stdout = run_command(run, &format!("run probe ({})", config.label))?;
    parse_probe_output(&stdout)
}

pub fn compare_records(
    oracle: &BTreeMap<String, i64>,
    candidate: &BTreeMap<String, i64>,
) -> Vec<(String, Option<i64>, Option<i64>)> {
    let names: BTreeSet<&String> = oracle.keys().chain(candidate.keys()).collect();
    names
        .into_iter()
        .map(|name| {
            (
                name.clone(),
                oracle.get(name).copied(),
                candidate.get(name).copied(),
            )
        })
        .collect()
}
